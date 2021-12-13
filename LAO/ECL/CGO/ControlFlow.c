#ifdef $XCC_h
/*
 * !!!!	ControlFlow.xcc
 *
 * Nikola Puzovic (nikola.puzovic@st.com).
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 *
 * Copyright 2002 - 2007 STMicroelectronics.
 * Copyright 1995 - 1998 Commissariat a l'Energie Atomique.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup CGO
 * @brief Control Flow optimizations.
 */
#endif//$XCC_h


#include "CGO/CGO_.h"

#ifdef $XCC__h
#include "CGO/ControlFlow.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Control flow analysis and optimizations.
 */
struct ControlFlow_;
#endif//$XCC_h

#ifdef $XCC__h
struct ControlFlow_ {
  //@args	Memory memory, CodeRegion codeRegion
  CodeRegion CODEREGION;
  PQueue CP_WORKLIST;
  Sparse CP_CHAINS;
  uintptr_t *CP_BLOCK_PRIO;
  PQueue CP_EDGE_QUEUE;
  uintptr_t *CP_EDGE_PRIO;
  bool *CP_RPO_VISITED;   //!< Used by code ordering and BB frequency calculation
  LoopForest LOOP_FOREST;
  BasicBlockStack_ CP_RPO_STACK_;
  //@access CP_RPO_STACK	ControlFlow__CP_RPO_STACK_(this)
  IStack_ PROBABILITYMAP_;
    //@access PROBABILITYMAP	ControlFlow__PROBABILITYMAP_(this)
    //@access MEMORY	IStack_memory(ControlFlow_PROBABILITYMAP(this))
    //@access PROBABILITIES	(float *)IStack_base(ControlFlow_PROBABILITYMAP(this))
};
#endif//$XCC__h

ControlFlow
ControlFlow_Ctor(ControlFlow this,
                 Memory memory, CodeRegion codeRegion)
{
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  *ControlFlow__CODEREGION(this) = codeRegion;
  *ControlFlow__CP_WORKLIST(this) = NULL;
  *ControlFlow__CP_CHAINS(this) = NULL;
  *ControlFlow__CP_BLOCK_PRIO(this) = NULL;
  *ControlFlow__CP_EDGE_QUEUE(this) = NULL;
  *ControlFlow__CP_EDGE_PRIO(this) = NULL;
  *ControlFlow__CP_RPO_VISITED(this) = NULL;
  *ControlFlow__LOOP_FOREST(this) = NULL;
  BasicBlockStack_Ctor(ControlFlow_CP_RPO_STACK(this), memory, blockCount);
  IStack_Ctor(ControlFlow_PROBABILITYMAP(this),
              memory, sizeof(float), blockCount*2);
  return this;
}

void
ControlFlow_Dtor(ControlFlow this)
{
  BasicBlockStack_Dtor(ControlFlow_CP_RPO_STACK(this));
  IStack_Dtor(ControlFlow_PROBABILITYMAP(this));
}

size_t
ControlFlow_Size(Memory memory, CodeRegion codeRegion)
{
  return sizeof(ControlFlow_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ControlFlow_)\t%zu\n", sizeof(ControlFlow_));
}
#endif//$XCC__c

#ifdef $XCC_h
ControlFlow
ControlFlow_new(Memory parent, CodeRegion codeRegion);
#endif//$XCC_h

ControlFlow
ControlFlow_new(Memory parent, CodeRegion codeRegion)
{
  Memory memory = Memory_new(parent, true);
  size_t size = ControlFlow_Size(memory, codeRegion);
  ControlFlow this = Memory_alloc(memory, size);
  return ControlFlow_Ctor(this, memory, codeRegion);
}

#ifdef $XCC_h
ControlFlow
ControlFlow_delete(ControlFlow this);
#endif//$XCC_h

ControlFlow
ControlFlow_delete(ControlFlow this)
{
  if (this != NULL) {
    Memory memory = ControlFlow_MEMORY(this);
#ifndef _NDTOR
    ControlFlow_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This ControlFlow Memory.
 */
Memory
ControlFlow_memory(ControlFlow this);
#endif//$XCC_h

Memory
ControlFlow_memory(ControlFlow this)
{
  return ControlFlow_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This ControlFlow CodeRegion.
 */
CodeRegion
ControlFlow_codeRegion(ControlFlow this);
#endif//$XCC_h

CodeRegion
ControlFlow_codeRegion(ControlFlow this)
{
  return ControlFlow_CODEREGION(this);
}

#ifdef $XCC_h
/**
 * Pretty-print this ControlFlow.
 */
bool
ControlFlow_pretty(ControlFlow this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
ControlFlow_pretty(ControlFlow this, FILE *file)
{
  if (file == NULL) file = stderr;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////
// CFG sanity check

#ifdef $XCC_h
/**
 * CLEAN algorithm.
 */
void
ControlFlow_checkCFGsanity(ControlFlow this, bool is_initializer);
#endif//$XCC_h

void
ControlFlow_checkCFGsanity(ControlFlow this, bool is_initializer)
{
  CodeRegion codeRegion = ControlFlow_codeRegion(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    // 0. Exit blocks are special, since they have one successor, and no branch...
    if (BasicBlock_isExit(block)) {
      BasicBlockEdge out_edge;
      if (BasicBlock_leaveCount(block) != 1) {
        fprintf(stderr, "CFG Error: Exit block shoud have exactly 1 outgoing edge\n");
        Except_ALWAYS(false);
      }
      if (!is_initializer && !Operator_isReturn(Operation_operator(BasicBlock_lastOperation(block)))) {
        fprintf(stderr, "CFG Error: Exit block shoud have return at the end\n");
        Except_ALWAYS(false);
      }
      out_edge = IDList_firstItem(BasicBlock_LEAVEEDGES(block));
      if (BasicBlockEdge_DESTBLOCK(out_edge) != CodeRegion_rootBlock(codeRegion)) {
        fprintf(stderr, "CFG Error: Exit block shoud have an edge to the root block\n");
        Except_ALWAYS(false);
      }
      continue;
    }
    // 1. Each basic block must have one or two successors
    if (BasicBlock_leaveCount(block) < 1 || BasicBlock_leaveCount(block) > 2) {
      fprintf(stderr, "CFG Error: Basic block with %d outgoing edges found\n",
          BasicBlock_leaveCount(block));
      Except_ALWAYS(false);
    }
    // 2. Each block with more then one successor must have branch
    if (BasicBlock_leaveCount(block) > 1) {
      Operation last_operation = BasicBlock_lastOperation(block);
      if (!Operator_isBranch(Operation_operator(last_operation))) {
        fprintf(stderr, "CFG Error: Basic block with 2 outgoing edges and no branch found\n");
        Except_ALWAYS(false);
      }
    }
    // Check branch targets and fall-trough edges
    if (BasicBlock_leaveCount(block) == 1) {
      BasicBlockEdge out_edge = IDList_firstItem(BasicBlock_LEAVEEDGES(block));
      Operation last_operation = BasicBlock_lastOperation(block);
      if (BasicBlockEdge_DESTBLOCK(out_edge) == BasicBlock_next(block)) {
        if (   last_operation != NULL 
            && Operator_isBranch(Operation_operator(last_operation))) {
          //fprintf(stderr, "CFG Warning: 1 outgoing fall-trough edge, and control instruction\n");
        }
      } else {
        if (last_operation == NULL || !Operator_isBranch(Operation_operator(last_operation))) {
          fprintf(stderr, "CFG Error: No branch where it should be! (1 outgoing edge)\n");
          Except_ALWAYS(false);
        }

        if (!BasicBlock_hasLabel(BasicBlockEdge_DESTBLOCK(out_edge),
                                 Operation_getTargetLabel(last_operation))) {
          fprintf(stderr, "CFG Error: Malformed branch! (1-1 case)\n");
          Except_ALWAYS(false);
        }
      }
    } else {
      BasicBlockEdge target_edge  = BasicBlock_getTargetEdge(block);
      BasicBlockEdge default_edge = BasicBlock_getDefaultEdge(block);
      Operation last_operation = BasicBlock_lastOperation(block);
      if (target_edge == NULL || default_edge == NULL) {
        fprintf(stderr, "CFG Error: Either target or fall-trough edge is missing!\n");
        Except_ALWAYS(false);
      }
      // Redundant Now! BasicBlock_getTargetEdge() checks for this!
      if (!BasicBlock_hasLabel(BasicBlockEdge_DESTBLOCK(target_edge),
                               Operation_getTargetLabel(last_operation))) {
        fprintf(stderr, "CFG Error: Branch has a wrong label!\n");
        Except_ALWAYS(false);
      }
      if (BasicBlock_next(block) != BasicBlockEdge_DESTBLOCK(default_edge)) {
        fprintf(stderr, "CFG Error: Malformed BasicBlock_next()!\n");
        Except_ALWAYS(false);
      }
    }
  } CodeRegion_ENDEACH_BasicBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// CLEAN algorithm

static void
ControlFlow_removeOutgoingEdges(ControlFlow this, BasicBlock block)
{
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    BasicBlock_killEdge(block, edge);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
}

static bool
ControlFlow_eliminateRedundantBranch(ControlFlow this, BasicBlock block)
{
  Except_REQUIRE(BasicBlock_index(block) != 0);
  if (   BasicBlock_getDefaultEdge(block) != NULL
      && BasicBlock_getTargetEdge(block) != NULL
      && (   BasicBlockEdge_DESTBLOCK(BasicBlock_getDefaultEdge(block)) 
          == BasicBlockEdge_DESTBLOCK(BasicBlock_getTargetEdge(block)))) {
    Operation last_operation = BasicBlock_lastOperation(block);
    Except_REQUIRE(   last_operation != NULL
                   && Operator_isBranch(Operation_operator(last_operation)));
    LIR_TRACE && fprintf (LIR_TRACE, " Removing edge %d --> %d\n", 
                                     BasicBlock_index(block), 
                                     BasicBlock_index(BasicBlockEdge_DESTBLOCK(
                                                      BasicBlock_getTargetEdge(block))));
    BasicBlock_killEdge(block, BasicBlock_getTargetEdge(block));
    BasicBlock_detachOperation(block, last_operation);
    return true;
  }
  return false;
}

static bool
ControlFlow_isBlockOnlyBranches(ControlFlow this, BasicBlock block)
{
  bool label_present = false;
  Except_REQUIRE(BasicBlock_index(block) != 0);
  BasicBlock_FOREACH_Operation(block, operation) {
    if (!Operator_isBranch(Operation_operator(operation))) {
      return false;
    } else {
      // Checks for inter-procedural jump
      if (Operator_isControl(Operation_operator(operation))) {
        Operation_ARGUMENTS_FOREACH_Temporary(operation, temporary) {
          if (Temporary_isLabel(temporary)) {
            label_present = true;
            break;
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
        return !label_present;
      }
    }
  } BasicBlock_ENDEACH_Operation;
  //LIR_TRACE && fprintf(LIR_TRACE, "     --- Block %d is only branches\n", 
  //                                BasicBlock_index(block));
  return true;
}

static bool
ControlFlow_eliminateEmptyBlock(ControlFlow this, BasicBlock block)
{
  BasicBlockEdge edge;
  BasicBlock succ_block;
  Except_REQUIRE(BasicBlock_index(block) != 0);
  if (BasicBlock_leaveCount(block) != 1) return false;	// more than one successor
  edge = IDList_firstItem(BasicBlock_LEAVEEDGES(block)); 
  succ_block = BasicBlockEdge_DESTBLOCK(edge);
  if (!ControlFlow_isBlockOnlyBranches(this, block)) return false;
  BasicBlock_killEdge(block, edge);
  BasicBlock_ENTER_FOREACH_BasicBlockEdge(block, edge) {
    BasicBlock orig_block = BasicBlockEdge_ORIGBLOCK(edge);
    float probability = BasicBlockEdge_PROBABILITY(edge);
    if (BasicBlock_getDefaultEdge(orig_block) != edge) {
      // Modify the branch instruction!
      Operation operation = BasicBlock_lastOperation(orig_block);
      Label label = BasicBlock_makeLabel(succ_block);
      Temporary l_temporary = Procedure_makeLabelTemporary(BasicBlock_procedure(block),
                                                           label, Immediate__UNDEF);
      // HACK
      Operation_setArgument(operation, Operation_ARGCOUNT(operation) - 1, l_temporary);
    }
    BasicBlock_makeEdge(orig_block, succ_block, probability);
    BasicBlock_killEdge(orig_block, edge); 
  } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  ControlFlow_removeOutgoingEdges(this, block);
  Procedure_unchainBasicBlock(BasicBlock_procedure(block), block); 
  LIR_TRACE && fprintf(LIR_TRACE, " Eliminating empty block %d\n", BasicBlock_index(block));
  return true;
}

/**
 * Part of CLEAN algorithm.
 *
 * Hoist branches from succ_block to pred_block without checking if it's safe or not.
 * Checking is done in calers. Actual moving of branch to pred_block is not done here,
 * this is also responsibility of the caller.
 */
static void
ControlFlow_hoistLeaveEdges(ControlFlow this, BasicBlock pred_block, BasicBlock succ_block, BasicBlockEdge edge)
{
  BasicBlock_killEdge(pred_block, edge); 
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(succ_block, succ_edge) {
    BasicBlock destBlock = BasicBlockEdge_DESTBLOCK(succ_edge);
    float probability = BasicBlockEdge_PROBABILITY(succ_edge);
    BasicBlock_makeEdge(pred_block, destBlock, probability);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
}

static bool
ControlFlow_hoistBranches(ControlFlow this, BasicBlock block)
{
  BasicBlockEdge edge;
  BasicBlock succ_block;
  Except_REQUIRE(BasicBlock_index(block) != 0);
  if (BasicBlock_leaveCount(block) != 1) return false;
  edge = IDList_firstItem(BasicBlock_LEAVEEDGES(block)); 
  succ_block = BasicBlockEdge_DESTBLOCK(edge);
  if (BasicBlock_index(succ_block) == 0) return false;
  if (BasicBlock_enterCount(succ_block) != 1) return false;
  if (!ControlFlow_isBlockOnlyBranches(this, succ_block)) return false;
  if (edge == BasicBlock_getTargetEdge(block)) {
    // It was a GoTo. Removing it.
    Operation goto_operation = BasicBlock_lastOperation(block);
    BasicBlock_detachOperation(block, goto_operation);
  }
  ControlFlow_hoistLeaveEdges(this, block, succ_block, edge);
  LIR_TRACE && fprintf(LIR_TRACE, " Hoisting branches from %d to %d\n", 
                                  BasicBlock_index(succ_block), 
                                  BasicBlock_index(block));
  if (BasicBlock_enterCount(succ_block) == 0) {
    ControlFlow_removeOutgoingEdges(this, succ_block);
    Procedure_unchainBasicBlock(BasicBlock_procedure(succ_block), succ_block);
  }
  return true;
}

/**
 * Checks if current block can be merged with following basic block, and merges them. 
 * @retval false if two blocks cannot be merged.
 * @retval true if merge succeds.
 */
static bool
ControlFlow_mergeBasicBlocks(ControlFlow this, BasicBlock block)
{
  BasicBlock succ_block;
  BasicBlockEdge edge;
  Except_REQUIRE(BasicBlock_index(block) != 0);
  if (BasicBlock_leaveCount(block) != 1) return false;
  edge = IDList_firstItem(BasicBlock_LEAVEEDGES(block)); 
  succ_block = BasicBlockEdge_DESTBLOCK(edge);
  if (BasicBlock_index(succ_block) == 0) return false;
  if (BasicBlock_enterCount(succ_block) != 1) return false;
  if (edge == BasicBlock_getTargetEdge(block)) {
    // It was a GoTo. Removing it.
    Operation goto_operation = BasicBlock_lastOperation(block);
    BasicBlock_detachOperation(block, goto_operation);
  }
  // Now do the real merge
  BasicBlock_pushSelector(block, BasicBlock_SELECTOR(succ_block));
  ControlFlow_hoistLeaveEdges(this, block, succ_block, edge);
  ControlFlow_removeOutgoingEdges(this, succ_block);
  Procedure_unchainBasicBlock(BasicBlock_procedure(succ_block), succ_block);
  LIR_TRACE && fprintf (LIR_TRACE, " Merging blocks %d and %d\n", 
                                   BasicBlock_index(block),
                                   BasicBlock_index(succ_block));
  return true;
}

static bool
ControlFlow_performClean(ControlFlow this, BasicBlock block)
{
  bool change = false;
  *BasicBlock__MARK(block) = 1;
  Except_REQUIRE(BasicBlock_index(block) != 0);
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    BasicBlock dest_block = BasicBlockEdge_DESTBLOCK(edge);
    if (   BasicBlock_index(dest_block) != 0
        && BasicBlock_MARK(dest_block) == 0) {
      change |= ControlFlow_performClean(this, dest_block);
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  change |= ControlFlow_eliminateRedundantBranch(this, block);
  if (ControlFlow_eliminateEmptyBlock(this, block))
    return true;
  change |= ControlFlow_mergeBasicBlocks(this, block);
  change |= ControlFlow_hoistBranches(this, block);
  return change;
}

#ifdef $XCC_h
/**
 * CLEAN algorithm.
 */
void
ControlFlow_clean(ControlFlow this);
#endif//$XCC_h

void
ControlFlow_clean(ControlFlow this)
{
  bool again = true;
  CodeRegion codeRegion = ControlFlow_CODEREGION(this);
  LIR_TRACE && fprintf(LIR_TRACE, "\n*** CLEAN algorithm (begin) ***\n");
  while (again) {
    CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
      *BasicBlock__MARK(block) = 0;
    } CodeRegion_ENDEACH_BasicBlock;
    again = false;
    CodeRegion_FORENTER_BasicBlock(codeRegion, block) { 
      again |= ControlFlow_performClean(this, block);
    } CodeRegion_ENDENTER_BasicBlock;
    //Procedure_buildCodeRegions(CodeRegion_procedure(codeRegion));
    if (again) {
      LIR_TRACE && fprintf (LIR_TRACE, "\n\n\n*** CLEAN : After one interation ***\n\n\n")
                && CodeRegion_pretty(codeRegion, LIR_TRACE);
    }
  }
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    *BasicBlock__MARK(block) = 0;
  } CodeRegion_ENDEACH_BasicBlock;
  LIR_TRACE && fprintf(LIR_TRACE, "\n*** CLEAN algorithm (end) ***\n");
}

#ifdef $XCC__c
if (0)
{
  // Testing the CLEAN algorithm
  Program program = Program_new(Memory_Root, (Processor)1, (Convention)1);
  Memory memory = Program_memory(program);
  SymbolTable symbolTable = Program_symbolTable(program);
  Symbol symbol = SymbolTable_lookup(symbolTable, 1, "lost-copy");
  Procedure procedure = Program_makeProcedure(program, symbol);
  LabelTable labelTable = Program_labelTable(program);
  Label L1 = LabelTable_lookup(labelTable, 1, "L1");
  Label L2 = LabelTable_lookup(labelTable, 2, "L2");
  Label L3 = LabelTable_lookup(labelTable, 3, "L3");
  Label L4 = LabelTable_lookup(labelTable, 4, "L4");
  Label L5 = LabelTable_lookup(labelTable, 5, "L5");
  Label L6 = LabelTable_lookup(labelTable, 6, "L6");
  Label L7 = LabelTable_lookup(labelTable, 7, "L7");
  Label L8 = LabelTable_lookup(labelTable, 8, "L8");
  BasicBlock block_l1 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l2 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l3 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l4 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l5 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l6 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l7 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l8 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock_addLabel(block_l1, L1);
  BasicBlock_addLabel(block_l2, L2);
  BasicBlock_addLabel(block_l3, L3);
  BasicBlock_addLabel(block_l4, L4);
  BasicBlock_addLabel(block_l5, L5);
  BasicBlock_addLabel(block_l6, L6);
  BasicBlock_addLabel(block_l7, L7);
  BasicBlock_addLabel(block_l8, L8);
  BasicBlock_makeEdge(block_l1, block_l2, 0.5);
  BasicBlock_makeEdge(block_l1, block_l2, 0.5);
  BasicBlock_makeEdge(block_l2, block_l3, 0.5);
  BasicBlock_makeEdge(block_l2, block_l5, 0.5);
  BasicBlock_makeEdge(block_l3, block_l4, 1.0);
  BasicBlock_makeEdge(block_l4, block_l7, 1.0);
  BasicBlock_makeEdge(block_l5, block_l7, 0.5);
  BasicBlock_makeEdge(block_l5, block_l6, 0.5);
  BasicBlock_makeEdge(block_l7, block_l8, 1.0);
  BasicBlock_makeEdge(block_l6, block_l8, 1.0);
  Procedure_buildCodeRegions(procedure);
  {
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Temporary temporary_x = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_a1 = Procedure_makeAbsoluteTemporary(procedure, 1, Immediate__UNDEF);
    Temporary temporary_l2 = Procedure_makeLabelTemporary(procedure, L2, Immediate__UNDEF);
    Temporary temporary_l5 = Procedure_makeLabelTemporary(procedure, L5, Immediate__UNDEF);
    Temporary temporary_l6 = Procedure_makeLabelTemporary(procedure, L6, Immediate__UNDEF);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l1), temporary_x, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l2), temporary_x, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l3), temporary_x, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l5), temporary_x, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l6), temporary_x, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l8), temporary_x, temporary_a1);
    Selector_makeOperation_0_2(BasicBlock_selector(block_l1),
                               Operator_IFTRUE, temporary_x, temporary_l2);
    Selector_makeOperation_0_2(BasicBlock_selector(block_l2),
                               Operator_IFTRUE, temporary_x, temporary_l5);
    Selector_makeOperation_0_2(BasicBlock_selector(block_l5),
                               Operator_IFTRUE, temporary_x, temporary_l6);
    CodeRegion_pretty(codeRegion, stdout);
    printf("\n*** Prior to cleaning the CodeRegion ***\n");
    {
      ControlFlow controlFlow = ControlFlow_new(memory, codeRegion);
      ControlFlow_clean(controlFlow);
      ControlFlow_delete(controlFlow);
    }
    printf("\n*** After cleaning the CodeRegion ***\n");
    CodeRegion_pretty(codeRegion, stdout);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////
// Dead Code Elimination

static inline void
ControlFlow_markAndPush(ControlFlow this, IDList worklist, Operation operation)
{
  Operation_clearFlags(operation, OperationFlag_DeadCode);
  IDList_PUSH(worklist, Operation, operation);
}

static void
ControlFlow_processReverseFrontier(ControlFlow this, IDList worklist,
                                   Dominance post_dominance,
                                   Operation live_operation)
{
  //TODO: This can be improved in terms of speed. We could mark each basic block that has
  //      been processed and not process it for more then one time...
  BasicBlock block = Operation_basicBlock(live_operation);
  Dominance_BasicBlock_FORFRONTIER_BasicBlock(post_dominance, block, child) {
    Operation last_operation = BasicBlock_lastOperation(child);
      if (   last_operation != NULL
          && Operator_isControl(Operation_operator(last_operation)))
        ControlFlow_markAndPush(this, worklist, last_operation);
  } Dominance_BasicBlock_ENDFRONTIER_BasicBlock;
}

static void
ControlFlow_markDeadCode(ControlFlow this)
{
  Memory memory = ControlFlow_MEMORY(this);
  CodeRegion codeRegion = ControlFlow_CODEREGION(this);
  DominanceFlags dominance_flags = DominanceFlag_Backward+DominanceFlag_Frontiers;
  Dominance post_dominance = Dominance_new(memory, codeRegion, dominance_flags);
  IDList_ worklist[1];
  IDList_Ctor(worklist, memory);
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    BasicBlock_FOREACH_Operation(block, operation) {
      if (Operation_isCritical(operation)) {
        ControlFlow_markAndPush(this, worklist, operation);
      } else {
        Operation_setFlags(operation, OperationFlag_DeadCode);
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  while (!IDList_isEmpty(worklist)) {
    Operation current_operation = IDList_BOT(worklist, Operation);
    IDList_drop_(worklist);
#if 0
    CGO_TRACE && fprintf(CGO_TRACE, "  Processing operation : ");
    CGO_TRACE && Operation_pretty(current_operation, CGO_TRACE);
#endif
    Operation_ARGUMENTS_FOREACH_Temporary(current_operation, argument) {
      if (Temporary_isVariable(argument)) {
        Variable variable = Temporary_VARIABLE(argument);
        OperationResult producer = Variable_PRODUCER(variable);
        Operation producer_operation = OperationResult_OPERATION(producer);
        if (Operation_isDeadCode(producer_operation)) {
#if 0
          CGO_TRACE && fprintf(CGO_TRACE, "    Adding to worklist : ");
          CGO_TRACE && Operation_pretty(producer_operation, CGO_TRACE);
#endif
          ControlFlow_markAndPush(this, worklist, producer_operation);
          ControlFlow_processReverseFrontier(this, worklist, post_dominance,
                                             producer_operation);
        }
      }
    } Operation_ARGUMENTS_ENDEACH_Temporary;
  }
  IDList_Dtor(worklist);
  Dominance_delete(post_dominance);
}

//AAA
static int32_t counter;

static void
ControlFlow_sweepDeadCode(ControlFlow this)
{
  CodeRegion codeRegion = ControlFlow_CODEREGION(this);
  //AAA  int32_t counter = 0;
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    BasicBlock_FOREACH_Operation(block, operation) {
      if (Operation_isDeadCode(operation)) {
        CGO_TRACE && fprintf(CGO_TRACE, "  DEADCODE : ");
        CGO_TRACE && Operation_pretty(operation, CGO_TRACE);
        counter++;
        if (Operator_isControl(Operation_operator(operation))) {
          BasicBlock current_block = Operation_basicBlock(operation);
          BasicBlockEdge branch_edge = BasicBlock_getTargetEdge(current_block);
          Except_CHECK(branch_edge != NULL);
          BasicBlock_killEdge(current_block, branch_edge);
        }
        BasicBlock_detachOperation(block, operation);
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  CGO_TRACE && fprintf(CGO_TRACE, "  DEAD COUNTER : %d instructions \n", counter);
}

#ifdef $XCC_h
/*
  ControlFlow_eliminate -- Eliminate dead code.
 */
void
ControlFlow_eliminate(ControlFlow this);
#endif//$XCC_h

void
ControlFlow_eliminate(ControlFlow this)
{
  ControlFlow_markDeadCode(this);
  ControlFlow_sweepDeadCode(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Pettis-Hansen

static BasicBlockEdge
ControlFlow_getMaxEdge(ControlFlow this)
{
  return (BasicBlockEdge)PQueue_extractMin(ControlFlow_CP_EDGE_QUEUE(this));
}

static void
ControlFlow_putEdge(ControlFlow this, BasicBlockEdge edge)
{
  int32_t index = BasicBlockEdge_INDEX(edge), weight;
  PQueue edge_queue = ControlFlow_CP_EDGE_QUEUE(this);
  //BD3 uintptr_t *edge_prio = ControlFlow_CP_EDGE_PRIO(this);
  union { float FLOAT; int32_t INT32; } _;
  const_BasicBlock origBlock = BasicBlockEdge_ORIGBLOCK(edge);
  float frequency = BasicBlock_FREQUENCY(origBlock)*BasicBlockEdge_PROBABILITY(edge);
  _.FLOAT = frequency, weight = _.INT32;
  Except_CHECK(weight >= 0);
  //BD3 edge_prio[index] = PQueuePriority_MAX - weight;
  PQueue_addEntry(edge_queue, edge, PQueuePriority_MAX - weight);
}

static BasicBlock
ControlFlow_chainEnd(ControlFlow this, BasicBlock chain_head)
{
  BasicBlock curr_block = chain_head;
  if (curr_block == NULL)
    return NULL;
  while (BasicBlock_CHAINNEXT(curr_block) != NULL) {
    curr_block = BasicBlock_CHAINNEXT(curr_block);
  }
  return curr_block;
}

static void
ControlFlow_initialize(ControlFlow this)
{
  Memory memory = ControlFlow_MEMORY(this);
  CodeRegion codeRegion = ControlFlow_CODEREGION(this);
  PQueue edge_queue = ControlFlow_CP_EDGE_QUEUE(this);
  Sparse chains = ControlFlow_CP_CHAINS(this);
  uintptr_t *block_prio = ControlFlow_CP_BLOCK_PRIO(this);
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  int32_t edgeCount = 0, edgeIndex = 0;
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    *BasicBlock__CHAINNEXT(block) = NULL; 
    Sparse_insert(chains, BasicBlock_index(block));
    block_prio[BasicBlock_index(block)] = blockCount;
    edgeCount += BasicBlock_leaveCount(block);
  } CodeRegion_ENDEACH_BasicBlock;
  PQueue_Ctor(edge_queue, memory, edgeCount);
  //BD3 *ControlFlow__CP_EDGE_PRIO(this) = Memory_alloc(edgeCount*sizeof(uintptr_t));
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
      *BasicBlockEdge__INDEX(edge) = edgeIndex++;
      ControlFlow_putEdge(this, edge);
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  } CodeRegion_ENDEACH_BasicBlock;
}

static void
ControlFlow_buildChains(ControlFlow this)
{
  CodeRegion codeRegion = ControlFlow_CODEREGION(this);
  uintptr_t *block_prio = ControlFlow_CP_BLOCK_PRIO(this);
  Sparse chains = ControlFlow_CP_CHAINS(this);
  PQueue edge_queue = ControlFlow_CP_EDGE_QUEUE(this);
  int32_t P = 1;
  BasicBlockEdge edge;
  PQueue_makeHeap(edge_queue);
  while (PQueue_count(edge_queue) > 0 && Sparse_count(chains) > 1) { 
    BasicBlock orig_block;
    BasicBlock dest_block;
    edge = ControlFlow_getMaxEdge(this);
    orig_block = BasicBlockEdge_ORIGBLOCK(edge);
    dest_block = BasicBlockEdge_DESTBLOCK(edge);
    if (   Sparse_contains(chains, BasicBlock_index(dest_block))
        && BasicBlock_CHAINNEXT(orig_block) == NULL) {
      if (ControlFlow_chainEnd(this, dest_block) != orig_block) {
        Sparse_remove(chains, BasicBlock_index(BasicBlockEdge_DESTBLOCK(edge)));
        *BasicBlock__CHAINNEXT(orig_block) = dest_block; 
      }
    }
    else
      block_prio[BasicBlock_index(dest_block)] = _MIN(block_prio[BasicBlock_index(dest_block)], P);
  };
}

/**
 * This will fix the case when we have that target edge is going to BasicBlock_next of the
 * pred_block by removing the jump instruction in case when there is only one outgoing
 * edge or by modifying the condition of the branch instruction in case when there are two
 * edges. 
 * @pre BasicBlock_next(pred_block) == succ_block
 */
static void
ControlFlow_fixBranch(ControlFlow this, BasicBlock pred_block)
{
  BasicBlock succ_block = BasicBlock_next(pred_block);
  BasicBlockEdge target_edge = BasicBlock_getTargetEdge(pred_block);
  // Don;t try to fix ecit block...
  if (BasicBlock_isExit(pred_block))
    return;
  if (BasicBlock_leaveCount(pred_block) == 1) {
    BasicBlockEdge edge = (BasicBlockEdge)IDList_firstItem(BasicBlock_LEAVEEDGES(pred_block));
    // TODO! Maybe check if this branch was OK (if existed in both cases)
    if (succ_block != BasicBlockEdge_DESTBLOCK(edge)) {
      // Branch is needed... Check if exists...
      if (    BasicBlock_lastOperation(pred_block) == NULL
          || !Operator_isBranch(Operation_operator(BasicBlock_lastOperation(pred_block)))) {
        Label label = BasicBlock_makeLabel(BasicBlockEdge_DESTBLOCK(edge));
        Selector_selectGOTO(BasicBlock_selector(pred_block), label);
      }
    } else {
      // No branch needed...
      if (   BasicBlock_lastOperation(pred_block) != NULL
          && Operator_isBranch(Operation_operator(BasicBlock_lastOperation(pred_block))))
        BasicBlock_detachOperation(pred_block, BasicBlock_lastOperation(pred_block));
    }
  } else if (   succ_block != NULL && target_edge != NULL
             && BasicBlockEdge_DESTBLOCK(target_edge) == succ_block) {
    Processor processor = BasicBlock_processor(pred_block);
    Operation branch_operation = BasicBlock_lastOperation(pred_block);
    if (Operation_tryInvertGoTo(branch_operation, processor)) {
      BasicBlock_LEAVE_FOREACH_BasicBlockEdge(pred_block, edge) {
        if (edge != target_edge) {
          Label label = BasicBlock_makeLabel(BasicBlockEdge_DESTBLOCK(edge));
          int32_t targetIndex = Operator_targetIndex(Operation_operator(branch_operation));
          Temporary target = Operation_getArgument(branch_operation, targetIndex);
          Literal target_literal = Temporary_LITERAL(target);
          Immediate immediate = Literal_IMMEDIATE(target_literal);
          Temporary l_temp = Procedure_makeLabelTemporary(BasicBlock_procedure(pred_block), label, immediate);
          Operation_setArgument(branch_operation, targetIndex, l_temp);
          break;
        }
      } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    } else Except_NEVER(true);
  }
}

static void
ControlFlow_fixBlockOrder(ControlFlow this, BasicBlock pred_block)
{
  BasicBlock succ_block = BasicBlock_next(pred_block);
  BasicBlock new_block, fall_block = NULL;
  BasicBlockEdge fall_edge = NULL, new_edge;
  Label target_label;
  bool found = false;
  float frequency;
  // Do not try to fix order from exit to root block!
  // Also, block with one successor will be fixed in CodeRegion_fixBranch()
  if (BasicBlock_isExit(pred_block) || BasicBlock_leaveCount(pred_block) == 1)
    return;
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(pred_block, edge) {
    if (BasicBlockEdge_DESTBLOCK(edge) == succ_block) {
      found = true;
      break;
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  if (found)
    return;
  // This is a situation where altering the branch didn't help, and we must insert new
  // block to fix the problem.
  target_label = Operation_getTargetLabel(BasicBlock_lastOperation(pred_block));
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(pred_block, edge) {
    if (!BasicBlock_hasLabel(BasicBlockEdge_DESTBLOCK(edge), target_label)) {
      fall_edge = edge;
      fall_block = BasicBlockEdge_DESTBLOCK(edge);
      break;
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  Except_CHECK(fall_edge != NULL);
  new_block = Procedure_makeBasicBlock(BasicBlock_procedure(pred_block), BasicBlock_regionId(pred_block), 
                                       BasicBlock_frequency(pred_block) * BasicBlockEdge_PROBABILITY(fall_edge));
  BasicBlock_killEdge(pred_block, fall_edge);
  BasicBlock_makeEdge(pred_block, new_block, BasicBlockEdge_PROBABILITY(fall_edge));
  new_edge = BasicBlock_makeEdge(new_block, fall_block, 1.0);
  *BasicBlockEdge__ARGINDEX(new_edge) = BasicBlockEdge_ARGINDEX(fall_edge);
  Except_CHECK(fall_block != NULL);
  Selector_selectGOTO(BasicBlock_selector(new_block), BasicBlock_makeLabel(fall_block));
  if (pred_block != new_block) {
    Procedure_unchainBasicBlock(BasicBlock_procedure(new_block), new_block);
    Procedure_chainBasicBlocks(BasicBlock_procedure(pred_block), pred_block, new_block);
  }
}

// TODO! Very slow check. See how to speed it up...
static BasicBlock
ControlFlow_findBeginningOfChain(ControlFlow this, Sparse chains, BasicBlock block) {
  CodeRegion codeRegion = ControlFlow_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, head_block) {
    BasicBlock curr_block = head_block;
    if (!Sparse_contains(chains, BasicBlock_index(head_block)))
      continue;
    while (curr_block != NULL) {
      if (block == curr_block)
        return head_block;
      curr_block = BasicBlock_CHAINNEXT(curr_block);
    }
  } CodeRegion_ENDEACH_BasicBlock;
  return NULL;
}

static void
ControlFlow_rearrangeChain(ControlFlow this, BasicBlock block)
{
  BasicBlock curr_block = block;
  Procedure procedure = BasicBlock_procedure(block);
  PQueue worklist = ControlFlow_CP_WORKLIST(this);
  uintptr_t *block_prio = ControlFlow_CP_BLOCK_PRIO(this);
  Sparse chains = ControlFlow_CP_CHAINS(this);
  // fprintf (stderr, "Chain begins with %d\n", BasicBlock_index(curr_block));
  while (curr_block != NULL) {
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(curr_block, edge) {
      BasicBlock dest_block = BasicBlockEdge_DESTBLOCK(edge);
      if (dest_block != BasicBlock_CHAINNEXT(curr_block)) {
        BasicBlock head_block = ControlFlow_findBeginningOfChain(this, chains, dest_block);
        if (head_block != NULL) {
          Sparse_remove(chains, BasicBlock_index(head_block));
          PQueue_insert(worklist, head_block, block_prio[BasicBlock_index(head_block)]);
        }
      }
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    curr_block = BasicBlock_CHAINNEXT(curr_block);
  }
}

static void
ControlFlow_makeLayout(ControlFlow this)
{
  CodeRegion codeRegion = ControlFlow_CODEREGION(this);
  uintptr_t *block_prio = ControlFlow_CP_BLOCK_PRIO(this);
  BasicBlock prev_block = NULL;
  BasicBlock head_of_chain = NULL;
  Memory memory = ControlFlow_MEMORY(this);
  Procedure procedure = CodeRegion_procedure(codeRegion);
  Sparse chains = ControlFlow_CP_CHAINS(this);
  // Initialize the worklist
  PQueue_ worklist[1];
  *ControlFlow__CP_WORKLIST(this) = worklist;
  PQueue_Ctor(worklist, memory, Sparse_count(chains));
  CodeRegion_FORENTER_BasicBlock(codeRegion, block) {
    Except_REQUIRE(Sparse_contains(chains, BasicBlock_index(block)));
    PQueue_insert(worklist, block, block_prio[BasicBlock_index(block)]);
    Sparse_remove(chains, BasicBlock_index(block));
  } CodeRegion_ENDENTER_BasicBlock;
  // Put the chains together
  while (PQueue_count(worklist) != 0) {
    BasicBlock head_block = (BasicBlock) PQueue_extractMin(worklist);
    if (head_of_chain == NULL)
      head_of_chain = head_block;
    if (prev_block != NULL) { 
      *BasicBlock__CHAINNEXT(prev_block) = head_block;
    }
    ControlFlow_rearrangeChain(this, head_block);
    prev_block = ControlFlow_chainEnd(this, head_block);
  }
  // Make the new layout
  Except_REQUIRE(Sparse_count(chains) == 0);
  while (head_of_chain != NULL) {
    if (   BasicBlock_CHAINNEXT(head_of_chain) != NULL
        && BasicBlock_next(head_of_chain) != BasicBlock_CHAINNEXT(head_of_chain)) {
      Procedure_unchainBasicBlock(procedure, BasicBlock_CHAINNEXT(head_of_chain));
      Procedure_chainBasicBlocks(procedure, head_of_chain, BasicBlock_CHAINNEXT(head_of_chain));
    }
    ControlFlow_fixBranch(this, head_of_chain);
    ControlFlow_fixBlockOrder(this, head_of_chain);
    head_of_chain = BasicBlock_CHAINNEXT(head_of_chain);
  }
  PQueue_Dtor(worklist);
  *ControlFlow__CP_WORKLIST(this) = NULL;
}

//Late construction of all items that are needed for code ordering.
static void
ControlFlow_codeOrder_ctor_(ControlFlow this)
{
  Memory memory = ControlFlow_MEMORY(this);
  int32_t blockCount = CodeRegion_blockCount(ControlFlow_codeRegion(this));
  Sparse chains = Memory_alloc(memory, sizeof(Sparse_));
  Sparse_Ctor(chains, memory, blockCount);
  *ControlFlow__CP_CHAINS(this) = chains;
  *ControlFlow__CP_BLOCK_PRIO(this) = Memory_alloc(memory, blockCount*sizeof(uintptr_t));
  //NOTE: CTOR is called later for edge_queue
  *ControlFlow__CP_EDGE_QUEUE(this) = Memory_alloc(memory, sizeof(PQueue_));
}

static void
ControlFlow_codeOrder_dtor_(ControlFlow this)
{
  Memory memory = ControlFlow_MEMORY(this);
  Sparse_Dtor(ControlFlow_CP_CHAINS(this));
  *ControlFlow__CP_CHAINS(this) = NULL;
  Memory_free(memory, ControlFlow_CP_BLOCK_PRIO(this));
  *ControlFlow__CP_BLOCK_PRIO(this) = NULL;
  PQueue_Dtor(ControlFlow_CP_EDGE_QUEUE(this));
  Memory_free(memory, ControlFlow_CP_EDGE_QUEUE(this));
  Memory_free(memory, ControlFlow_CP_EDGE_PRIO(this));
}

#ifdef $XCC_h
/*
 * ControlFlow_bottomUpOrderCode -- Pettis-Hansen profile guided code positioning.
 */
void
ControlFlow_bottomUpOrderCode(ControlFlow this);
#endif//$XCC_h

void
ControlFlow_bottomUpOrderCode(ControlFlow this)
{
  Memory memory = ControlFlow_MEMORY(this);
  CodeRegion codeRegion = ControlFlow_CODEREGION(this);
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  ControlFlow_codeOrder_ctor_(this);
  ControlFlow_initialize(this);
  ControlFlow_buildChains(this);
#ifdef XCC__C
{
  Sparse chains = ControlFlow_CP_CHAINS(this);
  printf("Chains : \n");
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    BasicBlock curr_block = block;
    if (!Sparse_contains(chains, BasicBlock_index(block)))
      continue;
    printf ("   ");
    while (curr_block != NULL) {
      printf ("%d", BasicBlock_index(curr_block));
      if (BasicBlock_CHAINNEXT(curr_block) != NULL)
        printf(" --> ");
      curr_block = BasicBlock_CHAINNEXT(curr_block);
    }
    printf("\n");
  } CodeRegion_ENDEACH_BasicBlock;
}
#endif
  ControlFlow_makeLayout(this);
  ControlFlow_codeOrder_dtor_(this);
}

//Late construction of all items that are needed for code ordering and that include
//reverse postorder walk of the code region.
static void
ControlFlow_codeOrder_rpo_ctor_(ControlFlow this)
{
  Memory memory = ControlFlow_MEMORY(this);
  int32_t blockCount = CodeRegion_blockCount(ControlFlow_codeRegion(this)), i;
  bool *visited = Memory_alloc(memory, blockCount*sizeof(bool));
  *ControlFlow__CP_RPO_VISITED(this) = visited;
  for(i = 1; i < blockCount; i++) visited[i] = false;
  visited[0] = true;
}

static void
ControlFlow_codeOrder_rpo_dtor_(ControlFlow this)
{
  Memory memory = ControlFlow_MEMORY(this);
  BasicBlockStack_empty(ControlFlow_CP_RPO_STACK(this));
  Memory_free(memory, ControlFlow_CP_RPO_VISITED(this));
}

static void
ControlFlow_performPostorder(ControlFlow this, BasicBlock block)
{
  bool *visited = ControlFlow_CP_RPO_VISITED(this);
  BasicBlockStack stack = ControlFlow_CP_RPO_STACK(this);
  visited[BasicBlock_index(block)] = true;
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    BasicBlock succ_block = BasicBlockEdge_DESTBLOCK(edge);
    if (!visited[BasicBlock_index(succ_block)]) {
      ControlFlow_performPostorder(this, succ_block);
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  BasicBlockStack_push(stack, block);
}

#ifdef $XCC_h
/**
 * This function performs code positioning by creating block chains (standard
 * Pettis-Hansen approach) and by giving priority according to reverse post-order.
 */
void
ControlFlow_bottomUpPostorder(ControlFlow this);
#endif//$XCC_h

void
ControlFlow_bottomUpPostorder(ControlFlow this)
{
  Memory memory = ControlFlow_MEMORY(this);
  CodeRegion codeRegion = ControlFlow_codeRegion(this);
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  uintptr_t *block_prio, priority = 0;
  BasicBlockStack block_stack;
  BasicBlock curr_block;
  // Creating the chains
  ControlFlow_codeOrder_ctor_(this);
  ControlFlow_codeOrder_rpo_ctor_(this);
  block_stack = ControlFlow_CP_RPO_STACK(this);
  ControlFlow_initialize(this);
  ControlFlow_buildChains(this);
  // Performing post-order
  CodeRegion_FORENTER_BasicBlock(codeRegion, block) {
    ControlFlow_performPostorder(this, block);
  } CodeRegion_ENDENTER_BasicBlock;
  // Changing the block_prio to match post-order DFS
  block_prio = ControlFlow_CP_BLOCK_PRIO(this);
  while (BasicBlockStack_count(block_stack) != 0) {
    curr_block = BasicBlockStack_top(block_stack);
    BasicBlockStack_pop(block_stack);
    block_prio[BasicBlock_index(curr_block)] = priority++;
  }
  // Rearranging the blocks
  ControlFlow_makeLayout(this);
  ControlFlow_codeOrder_dtor_(this);
  ControlFlow_codeOrder_rpo_dtor_(this);
}

#ifdef $XCC_h
/**
 * Reverse postorder code positioning.
 */
void
ControlFlow_reversePostOrderCode(ControlFlow this);
#endif//$XCC_h

void
ControlFlow_reversePostOrderCode(ControlFlow this)
{
  Memory memory = ControlFlow_MEMORY(this);
  CodeRegion codeRegion = ControlFlow_CODEREGION(this);
  int32_t blockCount = CodeRegion_blockCount(codeRegion), i;
  BasicBlockStack block_stack;
  BasicBlock pred_block, curr_block;
  Procedure procedure = CodeRegion_procedure(codeRegion);
  ControlFlow_codeOrder_rpo_ctor_(this);
  block_stack = ControlFlow_CP_RPO_STACK(this);
  CodeRegion_FORENTER_BasicBlock(codeRegion, block) {
    ControlFlow_performPostorder(this, block);
  } CodeRegion_ENDENTER_BasicBlock;
  if (NULL == (pred_block = BasicBlockStack_top(block_stack)))
    return;
  BasicBlockStack_pop(block_stack);
  while (BasicBlockStack_count(block_stack) != 0) {
    curr_block = BasicBlockStack_top(block_stack);
    BasicBlockStack_pop(block_stack);
#ifdef XCC__C
    printf("Chaining %d --> %d ...", BasicBlock_index(pred_block), BasicBlock_index(curr_block));
#endif
    Procedure_unchainBasicBlock(procedure, curr_block);
    Procedure_chainBasicBlocks(procedure, pred_block, curr_block);
    ControlFlow_fixBranch(this, pred_block);
    ControlFlow_fixBlockOrder(this, pred_block);
#ifdef XCC__C
    printf("done\n");
#endif
    pred_block = curr_block;
  }
  ControlFlow_codeOrder_rpo_dtor_(this);
}

#ifdef $XCC__c
if (0)
{
  // Testing the Pettis-Hansen algorithm
  Program program = Program_new(Memory_Root, (Processor)1, (Convention)1);
  Memory memory = Program_memory(program);
  SymbolTable symbolTable = Program_symbolTable(program);
  Symbol symbol = SymbolTable_lookup(symbolTable, 1, "lost-copy");
  Procedure procedure = Program_makeProcedure(program, symbol);
  LabelTable labelTable = Program_labelTable(program);
  Label L1 = LabelTable_lookup(labelTable, 1, "L1");
  Label L2 = LabelTable_lookup(labelTable, 2, "L2");
  Label L3 = LabelTable_lookup(labelTable, 3, "L3");
  Label L4 = LabelTable_lookup(labelTable, 4, "L4");
  Label L5 = LabelTable_lookup(labelTable, 5, "L5");
  Label L6 = LabelTable_lookup(labelTable, 6, "L6");
  Label L7 = LabelTable_lookup(labelTable, 7, "L7");
  Label L8 = LabelTable_lookup(labelTable, 8, "L8");
  BasicBlock block_l1 = Procedure_makeBasicBlock(procedure, 0, 100.0);
  BasicBlock block_l2 = Procedure_makeBasicBlock(procedure, 0, 100.0);
  BasicBlock block_l3 = Procedure_makeBasicBlock(procedure, 0, 100.0);
  BasicBlock block_l4 = Procedure_makeBasicBlock(procedure, 0, 100.0);
  BasicBlock block_l5 = Procedure_makeBasicBlock(procedure, 0, 100.0);
  BasicBlock block_l6 = Procedure_makeBasicBlock(procedure, 0, 100.0);
  BasicBlock block_l7 = Procedure_makeBasicBlock(procedure, 0, 100.0);
  BasicBlock block_l8 = Procedure_makeBasicBlock(procedure, 0, 100.0);
  BasicBlock_addLabel(block_l1, L1);
  BasicBlock_addLabel(block_l2, L2);
  BasicBlock_addLabel(block_l3, L3);
  BasicBlock_addLabel(block_l4, L4);
  BasicBlock_addLabel(block_l5, L5);
  BasicBlock_addLabel(block_l6, L6);
  BasicBlock_addLabel(block_l7, L7);
  BasicBlock_addLabel(block_l8, L8);
  BasicBlock_makeEdge(block_l1, block_l2, 1.0);
  BasicBlock_makeEdge(block_l2, block_l3, 0.5);
  BasicBlock_makeEdge(block_l2, block_l5, 0.5);
  BasicBlock_makeEdge(block_l3, block_l4, 1.0);
  BasicBlock_makeEdge(block_l4, block_l7, 1.0);
  BasicBlock_makeEdge(block_l5, block_l7, 0.5);
  BasicBlock_makeEdge(block_l5, block_l6, 0.5);
  BasicBlock_makeEdge(block_l7, block_l8, 1.0);
  BasicBlock_makeEdge(block_l6, block_l8, 1.0);
  Procedure_setEntryBlock(procedure, block_l1);
  Procedure_setExitBlock(procedure, block_l8);
  Procedure_buildCodeRegions(procedure);
  {
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Temporary temporary_x = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_a1 = Procedure_makeAbsoluteTemporary(procedure, 1, Immediate__UNDEF);
    Temporary temporary_l2 = Procedure_makeLabelTemporary(procedure, L2, Immediate__UNDEF);
    Temporary temporary_l5 = Procedure_makeLabelTemporary(procedure, L5, Immediate__UNDEF);
    Temporary temporary_l6 = Procedure_makeLabelTemporary(procedure, L6, Immediate__UNDEF);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l1), temporary_x, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l2), temporary_x, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l3), temporary_x, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l5), temporary_x, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l6), temporary_x, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l8), temporary_x, temporary_a1);
    Selector_makeOperation_0_2(BasicBlock_selector(block_l2),
                               Operator_IFTRUE, temporary_x, temporary_l5);
    Selector_makeOperation_0_2(BasicBlock_selector(block_l5),
                               Operator_IFTRUE, temporary_x, temporary_l6);
    printf("\n*** Prior to positioning ***\n");
    CodeRegion_pretty(codeRegion, stdout);
    {
      ControlFlow controlFlow = ControlFlow_new(memory, codeRegion);
      printf ("Original Pettis-Hansen code ordering\n\n");
      ControlFlow_bottomUpOrderCode(controlFlow);
      CodeRegion_pretty(codeRegion, stdout);
      printf ("\n\nReverse postorder code ordering\n\n");
      ControlFlow_reversePostOrderCode(controlFlow);
      CodeRegion_pretty(codeRegion, stdout);
      printf ("\n\nPettis-Hansen + reverse postorder code ordering\n\n");
      ControlFlow_bottomUpPostorder(controlFlow);
      CodeRegion_pretty(codeRegion, stdout);
      ControlFlow_delete(controlFlow);
    }
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * TODO.
 */
void
ControlFlow_makeDummyEdgeProbabilities(ControlFlow this);
#endif//$XCC_h

void 
ControlFlow_makeDummyEdgeProbabilities(ControlFlow this)
{
  CodeRegion codeRegion = ControlFlow_codeRegion(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    BasicBlockEdge edges[2];
    bool back_0, back_1;
    int32_t index = 0;
    if (BasicBlock_leaveCount(block) == 1) {
      edges[0] = (BasicBlockEdge)IDList_firstItem(BasicBlock_LEAVEEDGES(block));
      *BasicBlockEdge__PROBABILITY(edges[0]) = 1.0;
      return;
    }
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
      edges[index++] = edge;
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    back_0 = BasicBlockEdge_ISLOOP(edges[0]);
    back_1 = BasicBlockEdge_ISLOOP(edges[1]);
    if (back_0 && !back_1) {
      *BasicBlockEdge__PROBABILITY(edges[0]) = 0.8;
      *BasicBlockEdge__PROBABILITY(edges[1]) = 0.2;
    } else if (!back_0 && back_1) {
      *BasicBlockEdge__PROBABILITY(edges[0]) = 0.2;
      *BasicBlockEdge__PROBABILITY(edges[1]) = 0.8;
    } else { // (!back_0 && !back_1) || (back_0 && back_1)
      *BasicBlockEdge__PROBABILITY(edges[0]) = 0.5;
      *BasicBlockEdge__PROBABILITY(edges[1]) = 0.5;
    }
  } CodeRegion_ENDEACH_BasicBlock;
}

//TODO! Check if special field is needed for this in BasicBlockEdge. Seems to be working
//      also like this, but may  create problems.
static inline float
ControlFlow_CBF_getEdgeFrequency(BasicBlockEdge edge)
{
  return BasicBlockEdge_PROBABILITY(edge) *
         BasicBlock_FREQUENCY(BasicBlockEdge_ORIGBLOCK(edge));
}

static void
ControlFlow_CBF_propagateFrequency(ControlFlow this, BasicBlock block, BasicBlock head)
{
  bool *visited = ControlFlow_CP_RPO_VISITED(this);
  float *probabilities = ControlFlow_PROBABILITIES(this);
  if (visited[BasicBlock_index(block)])
    return;
  // 1. Find the branch frequency of the block
  if (block == head) {
    *BasicBlock__FREQUENCY(block) = 1.0;
  } else {
    float cyclic_p = 0.0;
    float new_frequency = 0.0;
    BasicBlock_ENTER_FOREACH_BasicBlockEdge(block, edge) {
      BasicBlock pred_block = BasicBlockEdge_ORIGBLOCK(edge);
      if (!visited[BasicBlock_index(pred_block)] && !BasicBlockEdge_ISLOOP(edge))
        return;
      if (BasicBlockEdge_ISLOOP(edge)) {
        Except_CHECK(BasicBlockEdge_INDEX(edge) >= 0);
        cyclic_p += probabilities[BasicBlockEdge_INDEX(edge)];
      } else {
        new_frequency += ControlFlow_CBF_getEdgeFrequency(edge);
      }
    } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
    if (cyclic_p > 1.0 - 0.01)
      cyclic_p = 1.0 - 0.01; // TODO! Check for value of epsilon
    *BasicBlock__FREQUENCY(block) = new_frequency/(1.0 - cyclic_p);
  }
  // 2. Calculate the frequencies of outgoing edges
  visited[BasicBlock_index(block)] = true;
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    // 1. TODO! Check if you need frequency here
    if (BasicBlockEdge_DESTBLOCK(edge) == head) {
      Except_CHECK(BasicBlockEdge_INDEX(edge) >= 0);
      probabilities[BasicBlockEdge_INDEX(edge)] = BasicBlockEdge_PROBABILITY(edge)
                                                * BasicBlock_FREQUENCY(block);
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // 3. Propagate to successor blocks
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    if (!BasicBlockEdge_ISLOOP(edge))
      ControlFlow_CBF_propagateFrequency(this, BasicBlockEdge_DESTBLOCK(edge), head);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
}

/**
 * Iterates over the loops in a CFG from the inner-most to the outer-most
 * one and calls function to update BB frequencies.
 */
static void
ControlFlow_CBF_iterateLoops(ControlFlow this, LoopForestNode head)
{
  CodeRegion codeRegion = ControlFlow_codeRegion(this);
  BasicBlock head_block = LoopForestNode_BLOCK(head);
  bool *visited = ControlFlow_CP_RPO_VISITED(this);
  // 1. Visit all inner loops of this loop
  LoopForestNode_CHILDREN_FOREACH_LoopForestNode(head, child_node) {
    if (   LoopForestNode_TYPE(child_node) == LoopForestNodeType_Loop
        || LoopForestNode_TYPE(child_node) == LoopForestNodeType_SCC)
      ControlFlow_CBF_iterateLoops(this, child_node);
  } LoopForestNode_CHILDREN_ENDEACH_LoopForestNode;
  // 2. Calculate BB frequencies in this loop
  memset (visited, 0xFF, sizeof(bool) * CodeRegion_blockCount(codeRegion));
  if (LoopForestNode_TYPE(head) == LoopForestNodeType_Root) {
    //printf ("Root: ");
    LoopForestNode_CHILDREN_FOREACH_LoopForestNode(head, child_node) {
      if (LoopForestNode_TYPE(child_node) == LoopForestNodeType_Block) {
        BasicBlock block = LoopForestNode_BLOCK(child_node);
        //printf("BB%d ", BasicBlock_index(block));
        visited[BasicBlock_index(block)] = false;
        ControlFlow_CBF_propagateFrequency(this, block, head_block);
      }
    } LoopForestNode_CHILDREN_ENDEACH_LoopForestNode;
    //printf ("\n");
  } else {
    //printf ("Current loop [BB%d]: ", BasicBlock_index(LoopForestNode_BLOCK(head)));
    LoopForestNode_CHILDREN_FOREACH_LoopForestNode(head, child_node) {
      if (LoopForestNode_TYPE(child_node) == LoopForestNodeType_Block) {
        BasicBlock block = LoopForestNode_BLOCK(child_node);
        //printf("BB%d ", BasicBlock_index(block));
        visited[BasicBlock_index(block)] = false;
      }
    } LoopForestNode_CHILDREN_ENDEACH_LoopForestNode;
    ControlFlow_CBF_propagateFrequency(this, head_block, head_block);
    //printf ("\n");
  }
}

#ifdef $XCC__h
/**
 * Computes frequency of all blocks and edges in a CFG.
 *
 * @pre CFG must have all edge probabilities properly set up.
 */
void
ControlFlow_computeBlockFrequency(ControlFlow this);
#endif//$XCC__h

void
ControlFlow_computeBlockFrequency(ControlFlow this)
{
  Memory memory = ControlFlow_MEMORY(this);
  CodeRegion codeRegion = ControlFlow_CODEREGION(this);
  BasicBlock rootBlock = CodeRegion_rootBlock(codeRegion);
  IStack probabilityMap = ControlFlow_PROBABILITYMAP(this);
  BasicBlock root_block = CodeRegion_rootBlock(codeRegion);
  int32_t blockCount = CodeRegion_blockCount(codeRegion), edgeCount = 0;
  bool *visited = Memory_alloc(memory, blockCount * sizeof(bool));
  // LoopForest in order to get loops
  LoopForest loopForest = LoopForest_new(memory, codeRegion);
  LoopForestNode rootNode = LoopForest_rootNode(loopForest);
  memset (visited, 0, sizeof(bool) * blockCount);
  *ControlFlow__CP_RPO_VISITED(this) = visited;
  // TODO! Very slow loop!
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(rootBlock, edge) {
    *BasicBlockEdge__INDEX(edge) = edgeCount++;
    IStack_PUSH2(probabilityMap, float, BasicBlockEdge_PROBABILITY(edge));
    Except_CHECK(IStack_count(probabilityMap) == edgeCount);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
      *BasicBlockEdge__INDEX(edge) = edgeCount++;
      IStack_PUSH2(probabilityMap, float, BasicBlockEdge_PROBABILITY(edge));
      Except_CHECK(IStack_count(probabilityMap) == edgeCount);
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  } CodeRegion_ENDEACH_BasicBlock;
  // Cleanup
  ControlFlow_CBF_iterateLoops(this, rootNode);
  memset (visited, 0, sizeof(bool) * blockCount);
  CodeRegion_FORENTER_BasicBlock(codeRegion, block) {
    ControlFlow_CBF_propagateFrequency(this, block, block);
  } CodeRegion_ENDENTER_BasicBlock;
  LoopForest_delete(loopForest);
  Memory_free(memory, visited);
  *ControlFlow__CP_RPO_VISITED(this) = NULL;
}

#ifdef $XCC__c
if (0) {
  // Example graph of atoi function to test BB frequency computation taken from 
  // Y. Wu, J. R. Larus "Static Branch Frequency and Program Profile Analysis"
  Symbol_ symbol[1];
  Program program = Program_new(Memory_Root, (Processor)1, (Convention)1);
  Procedure procedure = Program_makeProcedure(program, symbol);
  BasicBlock block_1 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_2 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_3 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_4 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_5 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_6 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock_makeEdge(block_1, block_2, 0.948);
  BasicBlock_makeEdge(block_1, block_6, 0.052);
  BasicBlock_makeEdge(block_2, block_3, 0.885);
  BasicBlock_makeEdge(block_2, block_5, 0.115);
  BasicBlock_makeEdge(block_3, block_4, 1.0);
  BasicBlock_makeEdge(block_4, block_4, 0.88);
  BasicBlock_makeEdge(block_4, block_5, 0.12);
  Procedure_setEntryBlock(procedure, block_1);
  Procedure_buildCodeRegions(procedure);
  {
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    ControlFlow controlFlow = ControlFlow_new(Memory_Root, codeRegion);
    printf("\n\n Testing Block Frequency computation : atoi():\n");
    CodeRegion_pretty(codeRegion, stdout);
    ControlFlow_computeBlockFrequency(controlFlow);
    printf("\n\n After frequency computation :\n");
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
}
#endif//$XCC__c

#ifdef $XCC__c
if (0) {
  // Example taken from F. Bourdoncle's PhD thesis ``Semantique des langages
  // imperatifs d'ordre superieur et interpretation abstraite'', 1992, Figure
  // 3.6 at page 39.
  // Expected frequencies at output:
  //   block_1 : 1.0
  //   block_2 : 2.185512695
  //   block_3 : 2.140869141
  //   block_4 : 1.0
  Minir minir = Minir_new(Memory_Root, stdin);
  Program program;
  Minir_KEEP_INDEX(minir) = true;
  program = Minir_nextProgram(minir);
  Program_FOREACH_Procedure(program, procedure) {
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    ControlFlow controlFlow = ControlFlow_new(Memory_Root, codeRegion);
    CodeRegion_pretty(codeRegion, stdout);
    ControlFlow_computeBlockFrequency(controlFlow);
    printf("\n\n After frequency computation :\n");
    CodeRegion_pretty(codeRegion, stdout);
  } Program_ENDEACH_Procedure;
  Program_delete(program);
  Minir_delete(minir);
}
#endif//$XCC__c

#ifdef $XCC__c
if (0) {
  Program program = Program_new(Memory_Root, (Processor)1, (Convention)1);
  SymbolTable symbolTable = Program_symbolTable(program);
  Symbol symbol = SymbolTable_lookup(symbolTable, 1, "Bourdoncle-p39");
  Procedure procedure = Program_makeProcedure(program, symbol);
  // Example taken from F. Bourdoncle's PhD thesis ``Semantique des langages
  // imperatifs d'ordre superieur et interpretation abstraite'', 1992, Figure
  // 3.6 at page 39.
  // Expected frequencies at output:
  //   block_1 : 1.0
  //   block_2 : 2.185512695
  //   block_3 : 2.140869141
  //   block_4 : 1.0
  BasicBlock block_1 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_2 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_3 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_4 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock_makeEdge(block_1, block_2, 0.43);
  BasicBlock_makeEdge(block_1, block_3, 0.57);
  BasicBlock_makeEdge(block_2, block_3, 0.72);
  BasicBlock_makeEdge(block_2, block_4, 0.28);
  BasicBlock_makeEdge(block_3, block_2, 0.82);
  BasicBlock_makeEdge(block_3, block_4, 0.18);
  Procedure_setEntryBlock(procedure, block_1);
  Procedure_buildCodeRegions(procedure);
  {
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    ControlFlow controlFlow = ControlFlow_new(Memory_Root, codeRegion);
    printf("\n\n Testing Block Frequency computation : Irreducible loop:\n");
    CodeRegion_pretty(codeRegion, stdout);
    ControlFlow_computeBlockFrequency(controlFlow);
    printf("\n\n After frequency computation :\n");
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the ControlFlow module.
 */
void
ControlFlow_INIT(void);
#endif//$XCC__h

void ControlFlow_INIT(void)
{
  counter = 0;
}

//#define ControlFlow_INIT()

#ifdef $XCC__h
/**
 * Finalize the ControlFlow module.
 */
void
ControlFlow_FINI(void);
#endif//$XCC__h

void
ControlFlow_FINI(void)
{
//  printf("-- Dead Code : %d\n", counter);
}

//#define ControlFlow_FINI()

#if XCC__C
static void
ControlFlow_TEST(void)
{
#include "CGO/ControlFlow_.c"
}

int
main(int argc, char **argv)
{
  CGO_Test(ControlFlow_TEST);
  return 0;
}
#endif

