#ifdef $XCC_h
/*
 * !!!!	SSAConstruct.xcc
 *
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
 * @ingroup PFA
 * @brief SSA form Construction.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/SSAConstruct.h"
#endif//$XCC__h

#ifdef $XCC__h
//#define SSAConstruct_SPILLABLE 0
#ifndef SSAConstruct_SPILLABLE
extern int SSAConstruct_SPILLABLE;
#endif//SSAConstruct_SPILLABLE
#endif//$XCC__h

#ifndef SSAConstruct_SPILLABLE
int SSAConstruct_SPILLABLE = 0;
#endif//SSAConstruct_SPILLABLE

#ifdef $XCC__h
//#define SSAConstruct_REMATERIALIZE 0
#ifndef SSAConstruct_REMATERIALIZE
extern int SSAConstruct_REMATERIALIZE;
#endif//SSAConstruct_REMATERIALIZE
#endif//$XCC__h

#ifndef SSAConstruct_REMATERIALIZE
int SSAConstruct_REMATERIALIZE = 0;
#endif//SSAConstruct_REMATERIALIZE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * SSA Form construction algorithms.
 */
struct SSAConstruct_;
#endif//$XCC_h

#ifdef $XCC__h
struct SSAConstruct_ {
  //@args	Memory memory, SSAForm ssaForm
  CodeRegion CODEREGION;		//!< The CodeRegion this SSAConstruct is for.
  //@access SSAFORM	CodeRegion_ssaForm(SSAConstruct_CODEREGION(this))
  TemporaryTable GLOBALTABLE;		//!< The global Temporary(ies) considered.
  Dominance DOMINANCE;			//!< Pre-dominance with frontiers or merge sets.
  BasicBlockStack_ *DEFBLOCKSMAP;	//!< Global Temporary INDEX to def BasicBlock(s).
  TemporaryStack_ *GLOBALRENAMES;	//!< Stacks to rename global SSA Temporary(ies).
  TemporaryStack_ LOCALRENAMES[1];	//!< Maps Temporary_INDEX to local SSA Temporary.
  TemporaryStack_ GLOBALPUSHES[1];	//!< Temporary_INDEX to local SSA Temporary.
  bool *APHISET;			//!< Array indexed by BasicBlock_index.
  bool **KILLSETS;			//!< Array of array indexed by BasicBlock_index.
  OperationStack_ WORKLIST[1];		//!< Operation worklist for dead code elimination.
  //@access MEMORY	OperationStack_memory(SSAConstruct_WORKLIST(this))
};
#endif//$XCC__h

SSAConstruct
SSAConstruct_Ctor(SSAConstruct this,
                  Memory memory, SSAForm ssaForm)
{
  Dominance dominance = SSAForm_DOMINANCE(ssaForm);
  CodeRegion codeRegion = SSAForm_codeRegion(ssaForm);
  TemporaryTable globalTable = CodeRegion_globalTable(codeRegion);
  BasicBlock rootBlock = CodeRegion_rootBlock(codeRegion);
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  Optimize optimize = CodeRegion_optimize(codeRegion);
  OptimizeFlags conversion = Optimize_CONVERSION(optimize);
  size_t aPhiSetSize = sizeof(bool)*blockCount;
  *SSAConstruct__CODEREGION(this) = codeRegion;
  *SSAConstruct__GLOBALTABLE(this) = globalTable;
  *SSAConstruct__DOMINANCE(this) = dominance;
  *SSAConstruct__DEFBLOCKSMAP(this) = NULL;
  *SSAConstruct__GLOBALRENAMES(this) = NULL;
  TemporaryStack_Ctor(SSAConstruct_LOCALRENAMES(this), memory, 128);
  TemporaryStack_Ctor(SSAConstruct_GLOBALPUSHES(this), memory, 128);
  *SSAConstruct__APHISET(this) = Memory_alloc(memory, aPhiSetSize);
  *SSAConstruct__KILLSETS(this) = NULL;
  OperationStack_Ctor(SSAConstruct_WORKLIST(this), memory, 4*blockCount);
  // Compute the DEFBLOCKSMAP and KILLSETS.
  SSAConstruct_makeDefBlocksMap(this);
  PFA_TRACE && fprintf(PFA_TRACE, "*** Before SSA construct\n")
            && CodeRegion_pretty(codeRegion, PFA_TRACE);
  // Place the PHI functions.
  SSAConstruct_placePhiFunctions(this);
  // Initialize the GLOBALRENAMES entries.
  // HACK! We alias GLOBALRENAMES to DEFBLOCKSMAP as BasicBlockStack and TemporaryStack
  // are just typed versions of pointer IStack(s) and the BasicBlockStack(s) are empty.
  *SSAConstruct__GLOBALRENAMES(this) = (TemporaryStack_ *)SSAConstruct_DEFBLOCKSMAP(this);
  // Recursive replacement of the Temporary(ies) by SSA variables.
  SSAConstruct_renameBasicBlockTemporaries(this, rootBlock, conversion);
  PFA_TRACE && fprintf(PFA_TRACE, "*** After SSA construct\n")
            && CodeRegion_pretty(codeRegion, PFA_TRACE);
  Except_ENSURE(TemporaryStack_isEmpty(SSAConstruct_GLOBALPUSHES(this)));
  // Simple dead code elimination.
  if (conversion & OptimizeConversion_Cleaning) {
    PFA_TRACE && fprintf(PFA_TRACE, "*** Code Cleaning:\n");
    SSAConstruct_removeDeadCode(this);
  }
  return this;
}

void
SSAConstruct_Dtor(SSAConstruct this)
{
  Memory memory = SSAConstruct_MEMORY(this);
  Memory_free(memory, SSAConstruct_DEFBLOCKSMAP(this));
  *SSAConstruct__DEFBLOCKSMAP(this) = NULL;
  TemporaryStack_Dtor(SSAConstruct_LOCALRENAMES(this));
  TemporaryStack_Dtor(SSAConstruct_GLOBALPUSHES(this));
  Memory_free(memory, SSAConstruct_APHISET(this));
  if (SSAConstruct_KILLSETS(this) != NULL) {
    bool **killSets = SSAConstruct_KILLSETS(this);
    CodeRegion codeRegion = SSAConstruct_CODEREGION(this);
    int32_t blockCount = CodeRegion_blockCount(codeRegion), i;
    for (i = 1; i < blockCount; i++) {
      Memory_free(memory, killSets[i]);
    }
  }
  OperationStack_Dtor(SSAConstruct_WORKLIST(this));
}

size_t
SSAConstruct_Size(Memory memory, SSAForm ssaForm)
{
  return sizeof(SSAConstruct_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(SSAConstruct_)\t%zu\n", sizeof(SSAConstruct_));
}
#endif//$XCC__c

#ifdef $XCC_h
SSAConstruct
SSAConstruct_new(Memory parent, SSAForm ssaForm);
#endif//$XCC_h

SSAConstruct
SSAConstruct_new(Memory parent, SSAForm ssaForm)
{
  Memory memory = Memory_new(parent, true);
  size_t size = SSAConstruct_Size(memory, ssaForm);
  void *alloc = Memory_alloc(memory, size);
  return SSAConstruct_Ctor(alloc, memory, ssaForm);
}

#ifdef $XCC_h
SSAConstruct
SSAConstruct_delete(SSAConstruct this);
#endif//$XCC_h

SSAConstruct
SSAConstruct_delete(SSAConstruct this)
{
  if (this != NULL) {
    Memory memory = SSAConstruct_MEMORY(this);
#ifndef _NDTOR
    SSAConstruct_Dtor(this);
    Memory_free(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Make the DEFBLOCKSMAP and other structures.
 */
void
SSAConstruct_makeDefBlocksMap(SSAConstruct this);
#endif//$XCC__h

void
SSAConstruct_makeDefBlocksMap(SSAConstruct this)
{
  Memory memory = SSAConstruct_MEMORY(this);
  CodeRegion codeRegion = SSAConstruct_CODEREGION(this);
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  TemporaryTable globalTable = SSAConstruct_GLOBALTABLE(this);
  int32_t globalCount = TemporaryTable_count(globalTable);
  size_t defBlocksMapSize = globalCount*sizeof(BasicBlockStack_);
  BasicBlockStack_ *defBlocksMap = Memory_alloc(memory, defBlocksMapSize);
  size_t killSetsSize = blockCount*sizeof(bool*);
  bool **killSets = Memory_alloc(memory, killSetsSize);
  *SSAConstruct__DEFBLOCKSMAP(this) = defBlocksMap;
  *SSAConstruct__KILLSETS(this) = killSets;
  killSets[0] = NULL;	// Because CodeRegion_FOREACH_BasicBlock omits the root block.
  // Initialization of defBlocksMap.
  TemporaryTable_FOREACH_Temporary(globalTable, temporary) {
    BasicBlockStack defBlockStack = defBlocksMap + TemporaryTable_Temporary_INDEX;
    BasicBlockStack_Ctor(defBlockStack, memory, 32);
  } TemporaryTable_ENDEACH_Temporary;
  // Traverse the CodeRegion and collect definitions in defBlocksMap.
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    size_t killSetSize = globalCount*sizeof(bool);
    bool *killSet = Memory_alloc(memory, killSetSize);
    memset(killSet, false, killSetSize);
    killSets[BasicBlock_index(basicBlock)] = killSet;
    BasicBlock_FORBACK_Operation(basicBlock, operation) {
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        if (   TemporaryTable_contains(globalTable, result)
            // FIXME! Can be removed when PartialDef(s) are handled by SSA.
            && Temporary_isRenameOK(result)) {
          int32_t result_index = Temporary_INDEX(result);
          BasicBlockStack defBlockStack = defBlocksMap + result_index;
          BasicBlockStack_push2(defBlockStack, basicBlock);
          // Update the killSet for basicBlock and result.
          killSet[result_index] = true;
        }
      } Operation_RESULTS_ENDEACH_Temporary;
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        if (   TemporaryTable_contains(globalTable, argument)
            // FIXME! Can be removed when PartialDef(s) are handled by SSA.
            && Temporary_isRenameOK(argument)) {
          int32_t argument_index = Temporary_INDEX(argument);
          // Update the killSet for basicBlock and result.
          killSet[argument_index] = false;
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
      if (SSAConstruct_SPILLABLE > 0) {
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (   TemporaryTable_contains(globalTable, argument)
              // FIXME! Can be removed when PartialDef(s) are handled by SSA.
              && Temporary_isRenameOK(argument)) {
            int32_t argument_index = Temporary_INDEX(argument);
            BasicBlockStack defBlockStack = defBlocksMap + argument_index;
            BasicBlockStack_push2(defBlockStack, basicBlock);
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
      }
    } BasicBlock_ENDBACK_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * SSA placement of PHI functions.
 *
 * See Appel's book "Modern Compiler Implementation in C" for !SSAForm_MERGESETS.
 *
 * See "A Practical and Fast Iterative Algorithm for PHI-Function Computation Using
 * DJ Graphs", D. Das, U. Ramakrishna, TOPLAS 27, 3 for SSAForm_MERGESETS.
 */
void
SSAConstruct_placePhiFunctions(SSAConstruct this);
#endif//$XCC__h

void
SSAConstruct_placePhiFunctions(SSAConstruct this)
{
  CodeRegion codeRegion = SSAConstruct_CODEREGION(this);
  TemporaryTable globalTable = SSAConstruct_GLOBALTABLE(this);
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  BasicBlockStack_ *defBlocksMap = SSAConstruct_DEFBLOCKSMAP(this);
  Dominance dominance = SSAConstruct_DOMINANCE(this);
  bool *aPhiSet = SSAConstruct_APHISET(this);
  bool **killSets = SSAConstruct_KILLSETS(this);
  // Place PHI-functions for each Temporary in globalTable.
  TemporaryTable_FOREACH_Temporary(globalTable, temporary) {
    int32_t temporary_index = TemporaryTable_Temporary_INDEX;
    BasicBlockStack defBlockStack = defBlocksMap + temporary_index;
    Except_CHECK(temporary_index == Temporary_INDEX(temporary));
    // aPhiSet[BasicBlock_index(b)] is true iff b has a PHI for the temporary.
    memset(aPhiSet, false, sizeof(bool)*blockCount);
    if (SSAForm_MERGESETS) {
      BasicBlockStack_FOREACH_BasicBlock(defBlockStack, n) {
        Dominance_BasicBlock_FORMERGESET_BasicBlock(dominance, n, y) {
          int32_t y_index = Dominance_BasicBlock_MERGE_INDEX;
          Except_CHECK(y_index == BasicBlock_index(y));
          if (!aPhiSet[y_index]) {
            bool *killSet = killSets[y_index];
            if (Temporary_isGlobal(temporary) && !killSet[temporary_index]) {
              BasicBlock_makeOperationPHI(y, temporary);
            }
            aPhiSet[y_index] = true;
          }
        } Dominance_BasicBlock_ENDFRONTIER_BasicBlock;
      } BasicBlockStack_ENDEACH_BasicBlock;
      // Ensure defBlockStack is empty for the GLOBALRENAMES hack.
      BasicBlockStack_empty(defBlockStack);
    } else {
      // The BasicBlock_mark(b) is true iff b is in defBlockStack.
      BasicBlockStack_FOREACH_BasicBlock(defBlockStack, basicBlock) {
        BasicBlock_setMark(basicBlock, true);
      } BasicBlockStack_ENDEACH_BasicBlock;
      // The defBlockStack is directly used as the BasicBlock worklist.
      while (!BasicBlockStack_isEmpty(defBlockStack)) {
        BasicBlock n = BasicBlockStack_top(defBlockStack);
        BasicBlockStack_pop(defBlockStack);
        BasicBlock_setMark(n, false);
        // Traverse the pre-dominance frontier of the definition block n.
        Dominance_BasicBlock_FORFRONTIER_BasicBlock(dominance, n, y) {
          int32_t y_index = Dominance_BasicBlock_FRONTIER_INDEX;
          Except_CHECK(y_index == BasicBlock_index(y));
          if (!aPhiSet[y_index]) {
            bool *killSet = killSets[y_index];
            if (Temporary_isGlobal(temporary) && !killSet[temporary_index]) {
              BasicBlock_makeOperationPHI(y, temporary);
              if (!BasicBlock_mark(y)) {
                BasicBlockStack_push2(defBlockStack, y);
                BasicBlock_setMark(y, true);
              }
            }
            aPhiSet[y_index] = true;
          }
        } Dominance_BasicBlock_ENDFRONTIER_BasicBlock;
      }
      // Ensure defBlockStack is empty for GLOBALRENAMES hack.
      Except_CHECK(BasicBlockStack_isEmpty(defBlockStack));
    }
  } TemporaryTable_ENDEACH_Temporary;
}

static inline void
SSAConstruct_killVariable(SSAConstruct this, Temporary temporary,
                          BasicBlock basicBlock, Operation before_operation)
{
  Selector selector = BasicBlock_selector(basicBlock);
  Variable variable = Temporary_VARIABLE(temporary);
  Operation kill_operation = Selector_makeOperationKILL(selector, temporary);
  if (before_operation != NULL) {
    Except_CHECK(Operation_basicBlock(before_operation) == basicBlock);
    Selector_moveOperationBefore(selector, before_operation, kill_operation);
  }
  Variable_setOperationResult(variable, kill_operation, 0);
}

/**
 * Rename the result with the temporary.
 *
 * Moved here from SSAConstruct_renameBasicBlockTemporaries().
 */
static inline void
SSAConstruct_renameVariable(SSAConstruct this,
                            Temporary result,
                            Temporary temporary,
                            int32_t *globalPushCount)
{
  TemporaryStack_ *globalRenames = SSAConstruct_GLOBALRENAMES(this);
  TemporaryStack localRenames = SSAConstruct_LOCALRENAMES(this);
  TemporaryStack globalPushes = SSAConstruct_GLOBALPUSHES(this);
  if (Temporary_isGlobal(result)) {
    int32_t result_index = Temporary_INDEX(result);
    TemporaryStack globalStack = globalRenames + result_index;
    TemporaryStack_push2(globalStack, temporary);
    TemporaryStack_push2(globalPushes, result);
    (*globalPushCount)++;
  } else {
    *Temporary__INDEX(result) = TemporaryStack_count(localRenames);
    TemporaryStack_push2(localRenames, temporary);
  }
}

#ifdef $XCC__h
/**
 * Replace the SSA variables for a BasicBlock.
 *
 * This is the traditional SSA renaming with segregation between global and local
 * Temporary(ies), as rename stacks are only needed for global Temporary(ies).
 */
void
SSAConstruct_renameBasicBlockTemporaries(SSAConstruct this,
                                         BasicBlock basicBlock,
                                         OptimizeFlags conversion);
#endif//$XCC__h

void
SSAConstruct_renameBasicBlockTemporaries(SSAConstruct this,
                                         BasicBlock basicBlock,
                                         OptimizeFlags conversion)
{
  int32_t globalPushCount = 0;
  SSAForm ssaForm = SSAConstruct_SSAFORM(this);
  CodeRegion codeRegion = SSAForm_CODEREGION(ssaForm);
  Dominance dominance = SSAConstruct_DOMINANCE(this);
  TemporaryStack_ *globalRenames = SSAConstruct_GLOBALRENAMES(this);
  TemporaryStack localRenames = SSAConstruct_LOCALRENAMES(this);
  TemporaryStack globalPushes = SSAConstruct_GLOBALPUSHES(this);
  // Process the basicBlock Operation(s).
  TemporaryStack_empty(localRenames);
  BasicBlock_FOREACH_Operation(basicBlock, operation) {
    Operator operator = Operation_operator(operation);
    if (!Operator_isPhi(operator)) {
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        if (Temporary_isRenameOK(argument)) {
          Temporary temporary = NULL;
          int32_t argument_index = Temporary_INDEX(argument);
          if (Temporary_isGlobal(argument)) {
            TemporaryStack globalStack = globalRenames + argument_index;
            if (TemporaryStack_isEmpty(globalStack)) {
              temporary = CodeRegion_makeVariableTemporary(codeRegion, argument);
              SSAConstruct_killVariable(this, temporary, basicBlock, operation);
              TemporaryStack_push2(globalStack, temporary);
              TemporaryStack_push2(globalPushes, temporary);
              globalPushCount++;
            } else {
              temporary = TemporaryStack_top(globalStack);
            }
          } else {
            // Valid because there must be a earlier definition in basicBlock.
            // Else the argument temporary would be tagged Global by Liveness.
            temporary = TemporaryStack_access(localRenames, argument_index);
          }
          Operation_ARGUMENTS[Operation_ARGUMENTS_INDEX] = temporary;
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
    }
    if (   (conversion & OptimizeConversion_Folding)
        && Operator_isSeqCopy(operator)) {
      // Copy folding:
      Temporary argument = Operation_getArgument(operation, 0);
      Temporary result = Operation_getResult(operation, 0);
      Except_CHECK(Operation_argCount(operation) == 1);
      if (Temporary_isVariable(argument) && Temporary_isRenameOK(result)) {
          PFA_TRACE && fprintf(PFA_TRACE, "*** Folding")
                    && Operation_pretty(operation, PFA_TRACE);
        SSAConstruct_renameVariable(this, result, argument, &globalPushCount);
        BasicBlock_detachOperation(basicBlock, operation);
        continue;
      }
    }
    Operation_RESULTS_FOREACH_Temporary(operation, result) {
      if (Temporary_isRenameOK(result)) {
        Temporary temporary = CodeRegion_makeVariableTemporary(codeRegion, result);
        Variable variable = Temporary_VARIABLE(temporary);
        if (Temporary_isGlobal(result)) {
          int32_t result_index = Temporary_INDEX(result);
          TemporaryStack globalStack = globalRenames + result_index;
          Temporary_setFlags(temporary, TemporaryFlag_Global);
          TemporaryStack_push2(globalStack, temporary);
          TemporaryStack_push2(globalPushes, temporary);
          globalPushCount++;
        } else {
          *Temporary__INDEX(result) = TemporaryStack_count(localRenames);
          TemporaryStack_push2(localRenames, temporary);
        }
        Operation_RESULTS[Operation_RESULTS_INDEX] = temporary;
        Variable_setOperationResult(variable, operation, Operation_RESULTS_INDEX);
        if (   SSAConstruct_REMATERIALIZE != 0
            && (Operator_isSeqCopy(operator) || Operator_isParCopy(operator))) {
          Temporary argument = Operation_getArgument(operation, Operation_RESULTS_INDEX);
          if (Temporary_hasLiteral(argument) || Temporary_isConstant(argument)) {
            Procedure_setRemater(CodeRegion_procedure(codeRegion), temporary, argument);
          } else if (Temporary_isRemater(argument)) {
            Procedure_setRemater(CodeRegion_procedure(codeRegion), temporary,
                                 Procedure_getRemater(CodeRegion_procedure(codeRegion), argument));
          }
        }
      }
    } Operation_RESULTS_ENDEACH_Temporary;
  } BasicBlock_ENDEACH_Operation;
  // Set arguments of PHI-functions in the successor BasicBlock(s).
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(basicBlock, edge) {
    int32_t argIndex = BasicBlockEdge_ARGINDEX(edge);
    BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
    BasicBlock_FOREACH_Operation(succ_block, operation) {
      if (Operator_isPhi(Operation_operator(operation))) {
        Temporary *arguments = Operation_arguments(operation);
        Temporary argument = arguments[argIndex], temporary = NULL;
        int32_t argument_index = Temporary_INDEX(argument);
        TemporaryStack globalStack = globalRenames + argument_index;
        if (TemporaryStack_isEmpty(globalStack)) {
          Operation last_operation = BasicBlock_lastOperation(basicBlock);
          temporary = CodeRegion_makeVariableTemporary(codeRegion, argument);
          if (last_operation != NULL) {
            Operator last_operator = Operation_operator(last_operation);
            if (Operator_isPhi(last_operator) || Operator_isLabel(last_operator)) {
              last_operation = NULL;
            }
          }
          SSAConstruct_killVariable(this, temporary, basicBlock, last_operation);
          Temporary_setFlags(temporary, TemporaryFlag_Global);
          TemporaryStack_push2(globalStack, temporary);
          TemporaryStack_push2(globalPushes, temporary);
          globalPushCount++;
        } else {
          temporary = TemporaryStack_top(globalStack);
        }
        arguments[argIndex] = temporary;
      } else break;
    } BasicBlock_ENDEACH_Operation;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // Recursively process children BasicBlock(s) in the dominator tree.
  Dominance_BasicBlock_FORCHILDREN_BasicBlock(dominance, basicBlock, child_block) {
    SSAConstruct_renameBasicBlockTemporaries(this, child_block, conversion);
  } Dominance_BasicBlock_ENDCHILDREN_BasicBlock;
  // Pop the basicBlock scope.
  while (globalPushCount-- > 0) {
    Temporary temporary = TemporaryStack_top(globalPushes);
    int32_t index = Temporary_INDEX(temporary);
    TemporaryStack globalStack = globalRenames + index;
    TemporaryStack_pop(globalStack);
    TemporaryStack_pop(globalPushes);
  }
}

#ifdef $XCC__h
/**
 * Remove dead Operation(s) with a worklist algorithm.
 */
void
SSAConstruct_removeDeadCode(SSAConstruct this);
#endif//$XCC__h

void
SSAConstruct_removeDeadCode(SSAConstruct this)
{
  CodeRegion codeRegion = SSAConstruct_CODEREGION(this);
  OperationStack worklist = SSAConstruct_WORKLIST(this);
  int32_t operationCount = 0, usefulCount = 0;
  // Initialize the worklist.
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      if (Operation_isCritical(operation)) {
        Operation_clearFlags(operation, OperationFlag_DeadCode);
        OperationStack_push2(worklist, operation);
        //PFA_TRACE && fprintf(PFA_TRACE, "*** Useful 1")
                  //&& Operation_pretty(operation, PFA_TRACE);
        usefulCount++;
      } else {
        Operation_setFlags(operation, OperationFlag_DeadCode);
      }
      operationCount++;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  // Process the worklist.
  while (!OperationStack_isEmpty(worklist)) {
    Operation operation = OperationStack_top(worklist);
    OperationStack_pop(worklist);
    Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
      if (Temporary_isVariable(argument)) {
        Variable variable = Temporary_VARIABLE(argument);
        OperationResult producer = Variable_PRODUCER(variable);
        Operation producer_operation = OperationResult_OPERATION(producer);
        if (Operation_isDeadCode(producer_operation)) {
          Operation_clearFlags(producer_operation, OperationFlag_DeadCode);
          OperationStack_push2(worklist, producer_operation);
          //PFA_TRACE && fprintf(PFA_TRACE, "*** Useful 2")
                    //&& Operation_pretty(producer_operation, PFA_TRACE);
          usefulCount++;
        }
      }
    } Operation_ARGUMENTS_ENDEACH_Temporary;
  }
  PFA_TRACE && fprintf(PFA_TRACE, "*** Useful/Total Operations %d/%d\n",
                       usefulCount, operationCount);
  if (usefulCount == operationCount) return;
  // Remove dead code.
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      if (Operation_isDeadCode(operation)) {
        PFA_TRACE && fprintf(PFA_TRACE, "*** DeadCode")
                  && Operation_pretty(operation, PFA_TRACE);
        BasicBlock_detachOperation(basicBlock, operation);
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the SSAConstruct module.
 */
#if defined(SSAConstruct_SPILLABLE)
#define SSAConstruct_INIT()
#else
void
SSAConstruct_INIT(void);
#endif
#endif//$XCC__h

#ifndef SSAConstruct_INIT
void
SSAConstruct_INIT(void)
{
#ifndef SSAConstruct_SPILLABLE
  if (GETENV("SSAConstruct_SPILLABLE")) {
    sscanf(GETENV("SSAConstruct_SPILLABLE"), "%d", &SSAConstruct_SPILLABLE);
    fprintf(stderr, "*** SSAConstruct_SPILLABLE=%d\n", SSAConstruct_SPILLABLE);
  }
#endif//SSAConstruct_SPILLABLE
#ifndef SSAConstruct_REMATERIALIZE
  if (GETENV("SSAConstruct_REMATERIALIZE")) {
    sscanf(GETENV("SSAConstruct_REMATERIALIZE"), "%d", &SSAConstruct_REMATERIALIZE);
    fprintf(stderr, "*** SSAConstruct_REMATERIALIZE=%d\n", SSAConstruct_REMATERIALIZE);
  }
#endif//SSAConstruct_REMATERIALIZE
}
#endif//SSAConstruct_INIT

#ifdef $XCC__h
/**
 * Finalize the SSAConstruct module.
 */
#define SSAConstruct_FINI()
#endif//$XCC__h

#if XCC__C
static void
SSAConstruct_TEST(void)
{
#include "PFA/SSAConstruct_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(SSAConstruct_TEST);
  return 0;
}
#endif

