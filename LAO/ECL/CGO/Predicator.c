#ifdef $XCC_h
/*
 * !!!!	Predicator.xcc
 *
 * Nikola Puzovic (Nikola.Puzovic@st.com).
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
 * @brief Predication optimizations.
 */
#endif//$XCC_h


#include "CGO/CGO_.h"

#ifdef $XCC__h
#include "CGO/Predicator.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Predication and IF-conversion for a CodeRegion.
 */
struct Predicator_ {
  //@args	Memory memory, SSAForm ssaForm
  Memory MEMORY;
  SSAForm SSAFORM;
  float AGRESSIVNESS_FACTOR;
  CodeRegion CODEREGION;
  BasicBlockStack_ POSTORDER_;
  //@access POSTORDER	Predicator__POSTORDER_(this)
  bool *VISITED;
};
#endif//$XCC_h

Predicator
Predicator_Ctor(Predicator this,
                Memory memory, SSAForm ssaForm)
{
  CodeRegion codeRegion = SSAForm_codeRegion(ssaForm);
  int32_t blockCount = CodeRegion_blockCount(codeRegion), i;
  size_t visitedSize = sizeof(bool)*blockCount;
  bool *visited = Memory_alloc(memory, visitedSize);
  bool continue_iteration = true;
  bool recalculate_frequency = true;
  *Predicator__MEMORY(this) = memory;
  *Predicator__SSAFORM(this) = ssaForm;
  *Predicator__AGRESSIVNESS_FACTOR(this) = 1.0; 
  *Predicator__CODEREGION(this) = codeRegion;
  BasicBlockStack_Ctor(Predicator_POSTORDER(this), memory, visitedSize);
  *Predicator__VISITED(this) = visited;
  // FIXME! Sometimes even with frequences set-up properly, frequences of entry blocks can
  //        be zero. This can happen in case when there are nested loops in the region,
  //        and the frequencies are normalized with respect to the most frequent basic
  //        block. This should be signaled with a flag, not like this.
  CodeRegion_FORENTER_BasicBlock(codeRegion, block) {
    if (BasicBlock_FREQUENCY(block) != 0) {
      recalculate_frequency = false;
      break;
    }
  } CodeRegion_ENDENTER_BasicBlock;
  if (recalculate_frequency) {
    ControlFlow controlFlow = ControlFlow_new(Predicator_memory(this), codeRegion);
    CGO_TRACE && fprintf(CGO_TRACE, 
                         "Predicator: Recalculating edge and block frequencies\n");
    ControlFlow_makeDummyEdgeProbabilities(controlFlow);
    ControlFlow_computeBlockFrequency(controlFlow);
    ControlFlow_delete(controlFlow);
  }
  CGO_TRACE && fprintf(CGO_TRACE, "\nCode prior to IF conversion:\n\n")
            && CodeRegion_pretty(codeRegion, CGO_TRACE); 
  for(i = 0; i < blockCount; i++)
    visited[i] = false;
  CodeRegion_FORENTER_BasicBlock(codeRegion, enter_block) {
    Predicator_DFS(this, enter_block);
  } CodeRegion_ENDENTER_BasicBlock;
  Predicator_traverseRegion(this);
  CGO_TRACE && fprintf(CGO_TRACE, "\nCode after IF conversion:\n\n")
            && CodeRegion_pretty(codeRegion, CGO_TRACE); 
  return this;
}

void
Predicator_Dtor(Predicator this)
{
  Memory memory = Predicator_MEMORY(this);
  BasicBlockStack_Dtor(Predicator_POSTORDER(this));
  Memory_free_(memory, Predicator_VISITED(this));
}

size_t
Predicator_Size(Memory memory, SSAForm ssaForm)
{
  return sizeof(Predicator_);
}

#ifdef $XCC__c
{
  CGO_TRACE = stdout;
  fprintf(stderr, "sizeof(Predicator_)\t%zu\n", sizeof(Predicator_));
}
#endif//$XCC__c

#ifdef $XCC_h
Predicator
Predicator_new(Memory parent, SSAForm ssaForm);
#endif//$XCC_h

Predicator
Predicator_new(Memory parent, SSAForm ssaForm)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Predicator_Size(memory, ssaForm);
  Predicator this = Memory_alloc(memory, size);
  return Predicator_Ctor(this, memory, ssaForm);
}

#ifdef $XCC_h
Predicator
Predicator_delete(Predicator this);
#endif//$XCC_h

Predicator
Predicator_delete(Predicator this)
{
  if (this != NULL) {
    Memory memory = Predicator_MEMORY(this);
#ifndef _NDTOR
    Predicator_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static inline void
Predicator_markSelectorSpeculated(Selector selector)
{
  Selector_FOREACH_Operation(selector, operation) {
    Operation_speculate(operation, Selector_processor(selector));
  } Selector_ENDEACH_Operation;
}

static inline int
Predicator_getBasicBlockCost(BasicBlock block)
{
  int32_t cost = 0;
  Processor processor = BasicBlock_processor(block);
  BasicBlock_FOREACH_Operation(block, operation) {
    if (Operator_isControl(Operation_operator(operation)))
      break;
    cost += Operation_unitCost(operation, processor);
  } BasicBlock_ENDEACH_Operation;
  return cost;
}

static bool
Predicator_isChild(BasicBlock parent, BasicBlock child)
{
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(parent, edge) {
    if (BasicBlockEdge_DESTBLOCK(edge) == child)
      return true;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  return false;
}

/* Checks if a block is a beggining of extended hammock, as defined in Bruel's paper.
 * Explanation of variable names used in this function:
 *   block
 *    |\
 *    | \
 *    | other_child
 *    | /
 *    |/
 *   common_child
 */
static bool
Predicator_isTriangleHammock(Predicator this, BasicBlock block, 
                              BasicBlock *common_child, BasicBlock *other_child)
{
  BasicBlock child[2];
  int32_t i = 0;
  if (BasicBlock_leaveCount(block) != 2)
    return false;
  // TODO! - See if you can get children in a different way.
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    child[i++] = BasicBlockEdge_DESTBLOCK(edge);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  if (   Predicator_isChild(child[0], child[1])
      && BasicBlock_enterCount(child[0]) == 1) {
    *common_child = child[1];
    *other_child = child[0];
    return true;
  } else if (Predicator_isChild(child[1], child[0])
             && BasicBlock_enterCount(child[1]) == 1) {
    *common_child = child[0];
    *other_child = child[1];
    return true;
  }
  return false;
}

static bool
Predicator_maySpeculateBlock(Predicator this, BasicBlock block)
{
  CodeRegion codeRegion = Predicator_CODEREGION(this);
  Processor processor = CodeRegion_processor(codeRegion);
  Configure configure = CodeRegion_configure(codeRegion);
  int32_t speculation_level = Configure_SPECULATION(configure);
  BasicBlock_FOREACH_Operation(block, operation) {
    //TODO! - Check if it is OK.
    if (Operator_isControl(Operation_operator(operation))) {
      bool label_present = false;
      Operation_ARGUMENTS_FOREACH_Temporary(operation, temporary) {
        if (Temporary_isLabel(temporary)) {
          label_present = true;
          break;
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
      if (!label_present)
        return false;
      else
        continue;
    }
    if (  !Operation_maySpeculate(operation, processor, speculation_level)
        || Operation_hasDedicatedResult(operation, false))
      return false;
  } BasicBlock_ENDEACH_Operation; 
  return true;
}

// Is it profitable to if-convert a triangle hammock?
// TODO! This code can be improved in terms of easier readability.
static inline bool
Predicator_isTriangleProfitable(Predicator this, BasicBlock parent, 
                                 BasicBlock common_child, BasicBlock other_child)
{
  Processor processor = BasicBlock_processor(parent);
  int parent_cost = Predicator_getBasicBlockCost(parent);
  int other_cost = Predicator_getBasicBlockCost(other_child);
  float latency_cost;         // Cost from latency from parent to one of the children.
  float child_latency_cost;   // Cost from latency from other child to common child
  float total_cost, new_cost;
  bool common_is_target =    BasicBlock_next(parent) != common_child
                          && BasicBlock_next(other_child) != common_child;
  CGO_TRACE && fprintf(CGO_TRACE, "*** Estimating cost for triangle BB%d --> BB%d --> BB%d ***\n",
                                  BasicBlock_index(parent),
                                  BasicBlock_index(other_child),
                                  BasicBlock_index(common_child));
  // Latency from parent to one of the children.
  latency_cost =  BasicBlock_FREQUENCY(parent)
                * BasicBlockEdge_PROBABILITY(BasicBlock_getTargetEdge(parent))
                * (  Processor_maxTakenCost(processor)
                   + Operation_unitCost(BasicBlock_lastOperation(parent), processor));
  // Latency from child that will be moved to parent
  if (BasicBlock_getTargetEdge(other_child) != NULL &&
      common_child == BasicBlockEdge_DESTBLOCK(BasicBlock_getTargetEdge(other_child))) {
    child_latency_cost = BasicBlock_FREQUENCY(other_child)
                       * BasicBlockEdge_PROBABILITY(BasicBlock_getTargetEdge(other_child))
                       * (  Processor_maxTakenCost(processor) 
                          + Operation_unitCost(BasicBlock_lastOperation(other_child), processor));
  } else {
    child_latency_cost = 0.0;
  }
  total_cost =  BasicBlock_FREQUENCY(parent) * parent_cost 
              + BasicBlock_FREQUENCY(other_child) * other_cost 
              + latency_cost 
              + child_latency_cost;
  new_cost = BasicBlock_FREQUENCY(parent) * (parent_cost + other_cost);
  if (BasicBlock_leaveCount(other_child) == 2) {
    new_cost +=   BasicBlock_FREQUENCY(parent) 
                * Operation_unitCost(BasicBlock_lastOperation(other_child), processor);
    if (common_is_target)
      new_cost += BasicBlock_FREQUENCY(common_child) * Processor_maxTakenCost(processor);
    else
      new_cost +=  (BasicBlock_FREQUENCY(other_child) - BasicBlock_FREQUENCY(common_child))
                 * Processor_maxTakenCost(processor);
  } else if (common_is_target) {
    Opcode opcode = Operator_opcode(Operator_GOTO, processor);
    if (opcode != Opcode__UNDEF)
      new_cost += BasicBlock_FREQUENCY(parent) * Opcode_unitCost(opcode);
    new_cost += BasicBlock_FREQUENCY(parent) * Processor_maxTakenCost(processor);
  }
  CGO_TRACE && fprintf(CGO_TRACE, "  Original cost : %f\n", total_cost);
  CGO_TRACE && fprintf(CGO_TRACE, "  New      cost : %f\n", new_cost);
  return total_cost > Predicator_AGRESSIVNESS_FACTOR(this) * new_cost;
}

// Is it possible to if-convert a triangle hammock?
static inline bool
Predicator_isTrianglePossible(Predicator this, BasicBlock parent,
                               BasicBlock common_child, BasicBlock other_child)
{
  if (Predicator_maySpeculateBlock(this, other_child))
    return true;
  return false;
}

/* Checks if a block is a beggining of diamond hammock.
 *       parent
 *         /\
 *        /  \
 *       c0  c1
 *        \  /
 *         \/
 *    common_child
 */
static bool
Predicator_isDiamondHammock(Predicator this, BasicBlock block, BasicBlock *common_child)
{
  BasicBlock child[2];
  BasicBlockEdge edge0, edge1;
  int32_t i = 0;
  if (BasicBlock_leaveCount(block) != 2)
    return false;
  // TODO! - See if you can get children in a different way.
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    child[i++] = BasicBlockEdge_DESTBLOCK(edge);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  if (   BasicBlock_enterCount(child[0]) != 1
      || BasicBlock_enterCount(child[1]) != 1
      || BasicBlock_leaveCount(child[0]) != 1
      || BasicBlock_leaveCount(child[1]) != 1)
    return false;
  edge0 = (BasicBlockEdge)IDList_firstItem(BasicBlock_LEAVEEDGES(child[0]));
  edge1 = (BasicBlockEdge)IDList_firstItem(BasicBlock_LEAVEEDGES(child[1]));
  if (edge1 == NULL || edge0 == NULL)
    return false; 
  if (   BasicBlockEdge_DESTBLOCK(edge0)
      != BasicBlockEdge_DESTBLOCK(edge1))
    return false;
  *common_child = BasicBlockEdge_DESTBLOCK(edge1);
  return true;
}

// Is it profitable to if-convert a diamond hammock?
static inline bool
Predicator_isDiamondProfitable(Predicator this, BasicBlock parent, BasicBlock common_child)
{
  // 2 branches... at the parent and at the end of one child...
  Processor processor = BasicBlock_processor(parent);
  int32_t parent_cost, child_cost[2], child_branch_costs = 0, child_index = 0;
  float child_frequency[2], total_cost, new_cost;
  BasicBlock child[2];
  bool need_goto = true;
  CGO_TRACE && fprintf(CGO_TRACE, "*** Estimating cost for diamond BB%d --> BB%d***\n",
                                  BasicBlock_index(parent),
                                  BasicBlock_index(common_child));
  parent_cost = Predicator_getBasicBlockCost(parent);
  CGO_TRACE && fprintf(CGO_TRACE, "  BB%d, F = %f, cost = %d",
                                  BasicBlock_index(parent),
                                  BasicBlock_FREQUENCY(parent),
                                  parent_cost);
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(parent, edge) {
    BasicBlock child_block = BasicBlockEdge_DESTBLOCK(edge);
    child[child_index] = child_block; 
    child_cost[child_index] = Predicator_getBasicBlockCost(child_block);
    child_frequency[child_index] = BasicBlock_FREQUENCY(child_block);
    CGO_TRACE && fprintf(CGO_TRACE, "  BB%d, F = %f, cost = %d",
                                    BasicBlock_index(child_block),
                                    BasicBlock_FREQUENCY(child_block),
                                    child_cost[child_index]);
    // We rely on the fact that each child can have only one successor...
    if (BasicBlock_getTargetEdge(child_block) != NULL) {
      Operation child_branch = BasicBlock_lastOperation(child_block);
      child_branch_costs +=   child_frequency[child_index]
                            * Operation_unitCost(child_branch, processor);
      child_branch_costs +=   child_frequency[child_index]
                            * Processor_maxTakenCost(processor);
      CGO_TRACE && fprintf(CGO_TRACE, ", branch cost = %d + %d", 
                                      Operation_unitCost(child_branch, processor), 
                                      Processor_maxTakenCost(processor));
    } else if (edge != BasicBlock_getTargetEdge(parent)) {
      need_goto = false;
    }
    CGO_TRACE && fprintf(CGO_TRACE, "\n");
    child_index++;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  total_cost =  BasicBlock_FREQUENCY(parent) * parent_cost
              + (  BasicBlock_FREQUENCY(parent)
                 * BasicBlockEdge_PROBABILITY(BasicBlock_getTargetEdge(parent))
                 * Processor_maxTakenCost(processor))
              + child_frequency[0] * child_cost[0]
              + child_frequency[1] * child_cost[1]
              + child_branch_costs;
  new_cost =  BasicBlock_FREQUENCY(parent) * 
                  (  parent_cost 
                   + child_cost[0]
                   + child_cost[1]);
  if (need_goto) {
    Opcode opcode = Operator_opcode(Operator_GOTO, processor);
    if (opcode != Opcode__UNDEF)
      new_cost += BasicBlock_FREQUENCY(parent) * Opcode_unitCost(opcode);
    new_cost += BasicBlock_FREQUENCY(parent) * Processor_maxTakenCost(processor);
  }
  return total_cost > Predicator_AGRESSIVNESS_FACTOR(this) * new_cost;
}

// Is it possible to if-convert a diamond hammock?
static inline bool
Predicator_isDiamondPossible(Predicator this, BasicBlock parent, BasicBlock other_child)
{
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(parent, child) {
    if (!Predicator_maySpeculateBlock(this, BasicBlockEdge_DESTBLOCK(child)))
      return false;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  return true;
}

// Fixes PHI operations!
// NOTE! All edges must be in a correct order before calling this one!!!
static void
Predicator_fixPhiOperations(Predicator this, BasicBlock parent, BasicBlock child, 
                             Temporary condition, int32_t common_index,
                             int32_t true_index, int32_t false_index)
{
  Memory memory = Predicator_memory(this);
  SSAForm ssaForm = Predicator_SSAFORM(this);
  CodeRegion codeRegion = SSAForm_codeRegion(ssaForm);
  bool modify_phi = BasicBlock_enterCount(child) != 1;
  bool first_phi = true;
  BasicBlock_FOREACH_Operation(child, phi_operation) {
    Operation select_operation;
    Temporary result = NULL;
    Temporary if_true, if_false;
    if (!Operator_isPhi(Operation_operator(phi_operation)))
      break;
    if_true = Operation_getArgument(phi_operation, true_index);    
    if_false = Operation_getArgument(phi_operation, false_index);
    // See if we need a new variable and accompanying SELECT
    if (!modify_phi) {
      result = Operation_getResult(phi_operation, 0);
    } else if (if_true != if_false) {
      Temporary origin = Variable_ORIGIN(Temporary_VARIABLE(if_true));
      result = CodeRegion_makeVariableTemporary(codeRegion, origin);
    } else {
      result = if_true;
    }
    if (if_true != if_false)
      select_operation = Selector_makeOperationSELECT(BasicBlock_SELECTOR(parent), 
                                                 result, condition, if_true, if_false);
    if (modify_phi) {
      Selector child_selector = BasicBlock_selector(child);
      int32_t deleted_index = (common_index == true_index) ? false_index : true_index;
      int32_t num_arguments = Operation_argCount(phi_operation) - 1;
      Operation new_phi = Selector_makeOperation_(child_selector,
                                                  Operator_PHI, 1, num_arguments, NULL);
      int32_t new_index = 0;
      if (first_phi) {
        // Re-enumerate enter-edges to get the correct ARGINDEX
        struct BasicBlockEdge_ **enteredges = BasicBlock_ENTEREDGES(child);
        CGO_TRACE && fprintf(CGO_TRACE, "Fixing PHI: common=%d, deleted=%d, true=%d, false=%d\n", 
                                        common_index, deleted_index, true_index, false_index);
        CGO_TRACE && fprintf(CGO_TRACE, "Modifying indexes : ");
        BasicBlock_ENTER_FOREACH_BasicBlockEdge(child, enter_edge) {
          int32_t argindex = BasicBlockEdge_ARGINDEX(enter_edge);
          CGO_TRACE && fprintf(CGO_TRACE, " %d --> ", argindex);
          if (argindex > deleted_index)
            argindex--;
          CGO_TRACE && fprintf(CGO_TRACE, "%d,", argindex);
          *BasicBlockEdge__ARGINDEX(enter_edge) = argindex;
          enteredges[argindex] = enter_edge;
        } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
        first_phi = false;
      }
      // Create new PHI operation with the proper arguments.
      Selector_moveOperationBefore(child_selector, phi_operation, new_phi);
      Operation_setResult(new_phi, 0, Operation_getResult(phi_operation, 0));
      Operation_setArgument(phi_operation, common_index, result);
      CGO_TRACE && fprintf (CGO_TRACE, "\nModifying arguments : ");
      Operation_ARGUMENTS_FOREACH_Temporary(phi_operation, argument) {
        if (Operation_ARGUMENTS_INDEX == deleted_index)
          continue;
        CGO_TRACE && fprintf (CGO_TRACE, "%d --> %d, ", Operation_ARGUMENTS_INDEX, new_index);
        Operation_setArgument(new_phi, new_index++, argument);
      } Operation_ARGUMENTS_ENDEACH_Temporary;
      CGO_TRACE && fprintf(CGO_TRACE, "\n");
    }
    BasicBlock_detachOperation(child, phi_operation);
 } BasicBlock_ENDEACH_Operation;
}

// Adds GoTo operation if necessary to fix the order between two basic blocks
static inline void
Predicator_fixTwoBlockOrder(BasicBlock parent, BasicBlock child)
{
  if (child != BasicBlock_next(parent)) {
    Selector_selectGOTO(BasicBlock_selector(parent), BasicBlock_makeLabel(child));
  }
}

/* TODO! 
 * The idea was to make machine dependent operations immediately, but there was a
 * problem with register files that are not kept in the variable temporary, but in the
 * ORIGIN temporary. It needs to be resolved wether it will be done with machine or
 * generic instructions (and in later case, to see which generics to use).
 */
#if 0
static inline void
Predicator_makeBranch(Predicator this,
                       BasicBlock block, Operator condition_operator, 
                       Temporary condition1, bool negate_condition1,
                       Temporary condition2, bool negate_condition2,
                       Label target_label)
{
  SSAForm ssaForm = Predicator_SSAFORM(this);
  Selector selector = BasicBlock_selector(block);
  Temporary condition = CodeRegion_makeVariableTemporary(codeRegion,
                                                      Temporary_ORIGIN(condition1));
  Temporary n_cond1 = condition1;
  Temporary n_cond2 = condition2;
  Temporary label_temporary = Procedure_makeLabelTemporary(BasicBlock_procedure(block),
                                                           target_label, Immediate__UNDEF);
  if (negate_condition1) {
    n_cond1 = CodeRegion_makeVariableTemporary(codeRegion,
                                            Temporary_ORIGIN(condition1));
    Selector_NotL(selector, n_cond1, condition1);
  }
  if (negate_condition2) {
    n_cond2 = CodeRegion_makeVariableTemporary(codeRegion,
                                            Temporary_ORIGIN(condition2));
    Selector_NotL(selector, n_cond2, condition2);
  }
  if (condition_operator == Operator_LAND)
    Selector_AndL(selector, condition, n_cond1, n_cond2);
  else if (condition_operator == Operator_LIOR)
    Selector_OrL(selector, condition, n_cond1, n_cond2);
  Selector_makeOperation_0_2(selector, Operator_IFTRUE, condition, label_temporary);
}
#endif

static inline void
Predicator_makeBranch(Predicator this,
                       BasicBlock block, Operator condition_operator, 
                       Temporary condition1, bool negate_condition1,
                       Temporary condition2, bool negate_condition2,
                       Label target_label)
{
  SSAForm ssaForm = Predicator_SSAFORM(this);
  CodeRegion codeRegion = SSAForm_codeRegion(ssaForm);
  Selector selector = BasicBlock_selector(block);
  Temporary origin1 = Variable_ORIGIN(Temporary_VARIABLE(condition1));
  Temporary origin2 = Variable_ORIGIN(Temporary_VARIABLE(condition2));
  Temporary condition = CodeRegion_makeVariableTemporary(codeRegion, origin1);
  Temporary n_cond1 = condition1;
  Temporary n_cond2 = condition2;
  Temporary label_temporary = Procedure_makeLabelTemporary(BasicBlock_procedure(block),
                                                           target_label, Immediate__UNDEF);
  if (negate_condition1) {
    n_cond1 = CodeRegion_makeVariableTemporary(codeRegion, origin1);
    Selector_makeOperation_1_1(selector, Operator_LNOT, n_cond1, condition1);
  }
  if (negate_condition2) {
    n_cond2 = CodeRegion_makeVariableTemporary(codeRegion, origin2);
    Selector_makeOperation_1_1(selector, Operator_LNOT, n_cond2, condition2);
  }
  Selector_makeOperation_1_2(selector, condition_operator, condition, n_cond1, n_cond2);
  Selector_makeOperation_0_2(selector, Operator_IFTRUE, condition, label_temporary);
}

// Checks if the last operation is a branch, and if it is removes it.
static inline Operation
Predicator_removeLastBranch(BasicBlock block)
{
  Operation last_operation = BasicBlock_lastOperation(block);
  if (BasicBlock_countOperations(block) == 0)
    return NULL;
  if (!Operator_isControl(Operation_operator(last_operation)))
    return NULL;
  BasicBlock_detachOperation(block, last_operation);
  return last_operation;
}

static inline void
Predicator_swapIndexes(Operation branch_operation, int32_t *index1, int32_t *index2)
{
  if (Operator_predicatePolarity(Operation_operator(branch_operation)) == -1) {
    int32_t temp_index = *index1;
    *index1 = *index2;
    *index2 = temp_index;
  }
}

// Performs the if-conversion of three blocks.
//   parent_block <-- beginning of a hammock
//   common_child <-- join point of a hammock
//   other_child  <-- child that will be merged
static void
Predicator_convertTriangleHammock(Predicator this,
                                   BasicBlock parent_block, 
                                   BasicBlock common_child, 
                                   BasicBlock other_child)
{
  // Edges going from other_child
  BasicBlockEdge parent_common_edge = NULL; // From parent to common child
  BasicBlockEdge parent_other_edge = NULL;  // From parent to other child
  BasicBlockEdge other_common_edge = NULL;  // From other child to common child
  BasicBlockEdge other_out_edge = NULL;     // From other child out of the hammock
  Temporary condition, out_condition;
  bool common_is_target;              // If target edge leads to common successor?
  int32_t if_true_index = -1;
  int32_t if_false_index = -1;
  int32_t common_index;
  Operation parent_branch, other_branch;
  common_is_target = (   BasicBlockEdge_DESTBLOCK(BasicBlock_getTargetEdge(parent_block))
                      == common_child);
  // Get the edges.
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(other_child, edge) {
    if (BasicBlockEdge_DESTBLOCK(edge) == common_child)
      other_common_edge = edge;
    else
      other_out_edge = edge;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(parent_block, edge) {
    if (BasicBlockEdge_DESTBLOCK(edge) == common_child)
      parent_common_edge = edge;
    else
      parent_other_edge = edge;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // TODO! ovo moze da se zameni sa common_is_target
  if (common_child == BasicBlockEdge_DESTBLOCK(BasicBlock_getTargetEdge(parent_block))) {
    if_true_index = BasicBlockEdge_ARGINDEX(BasicBlock_getTargetEdge(parent_block));
    if_false_index = BasicBlockEdge_ARGINDEX(other_common_edge);
  } else {
    if_false_index = BasicBlockEdge_ARGINDEX(BasicBlock_getDefaultEdge(parent_block));
    if_true_index = BasicBlockEdge_ARGINDEX(other_common_edge);
  }
  // Remove both branches, save them for later and merge, and remove blocks an edges
  parent_branch = Predicator_removeLastBranch(parent_block);
  // TODO! MD - maybe can merge with previuos if?
  Predicator_swapIndexes(parent_branch, &if_true_index, &if_false_index);
  other_branch = Predicator_removeLastBranch(other_child);
  Predicator_markSelectorSpeculated(BasicBlock_SELECTOR(other_child));
  BasicBlock_pushSelector(parent_block, BasicBlock_SELECTOR(other_child));
  BasicBlock_killEdge(parent_block, parent_other_edge);
  BasicBlock_killEdge(other_child, other_common_edge);
  if (other_out_edge != NULL)
    BasicBlock_killEdge(other_child, other_out_edge);
  Procedure_unchainBasicBlock(BasicBlock_procedure(other_child), other_child);
  // Get arguments for fixing-up PHI instructions in common_child
  condition = Operation_getArgument(parent_branch, 0); // FIXME!
  common_index = BasicBlockEdge_ARGINDEX(parent_common_edge);
  //common_index = if_true_index < if_false_index ? if_true_index : if_false_index;
  Predicator_fixPhiOperations(this, parent_block, common_child, condition, 
                               common_index, if_true_index, if_false_index);
  // Fix order if needed
  if (other_out_edge == NULL) {
    // This is a pure triangle hammock, easy case
    Predicator_fixTwoBlockOrder(parent_block, common_child);
    *BasicBlockEdge__PROBABILITY(parent_common_edge) = 1.0;
  } else {
    // This is triangle hammock with one outgoing edge (other_out_edge)
    BasicBlock out_block  = BasicBlockEdge_DESTBLOCK(other_out_edge);
    bool common_is_target = BasicBlock_hasLabel(common_child, 
                                                Operation_getTargetLabel(parent_branch));
    bool out_is_target = BasicBlock_hasLabel(out_block, 
                                             Operation_getTargetLabel(other_branch));
    // Variables used to make new condition and branch.
    bool negate_first = false, negate_second = false;
    Operator condition_operator = Operator_LAND; // TODO! See if it is correct...
    Label branch_label = NULL;
    // This edge must be moved to the parent_block
    BasicBlockEdge new_edge = BasicBlock_makeEdge(parent_block,
                                                  out_block,
                                                  BasicBlockEdge_PROBABILITY(other_out_edge) * 
                                                  BasicBlockEdge_PROBABILITY(parent_other_edge)); 
    *BasicBlockEdge__ARGINDEX(new_edge) = BasicBlockEdge_ARGINDEX(other_out_edge);
    *BasicBlockEdge__PROBABILITY(parent_common_edge) = 1.0 - BasicBlockEdge_PROBABILITY(new_edge);
    // TODO! MD
    if (Operator_predicatePolarity(Operation_operator(other_branch)) == -1)
      negate_second = true;
    if (Operator_predicatePolarity(Operation_operator(parent_branch)) == -1)
      negate_first = true;
    if (common_is_target && out_is_target) {
      // Both are target blocks. - Case 4)
      condition_operator = Operator_LAND;
      negate_first = !negate_first;
//      negate_second = false;
      branch_label = BasicBlock_makeLabel(out_block);
    } else if (!common_is_target && !out_is_target) {
      // Both are fall-trough blocks. - Case 1)
      condition_operator = Operator_LAND;
//      negate_first = false;
      negate_second = !negate_second;
      branch_label = BasicBlock_makeLabel(out_block);
    } else if (!common_is_target && out_is_target){
      // This situation is not allowed because we have two incoming fall-trough edges 
      Except_NEVER(true);
    } else if (common_is_target && !out_is_target) {
      // Case 3)
      condition_operator = Operator_LIOR;
//      negate_first = false;
//      negate_second = false;
      branch_label = BasicBlock_makeLabel(common_child);
    }
    out_condition = Operation_getArgument(other_branch, 0); // FIXME! .
    Predicator_makeBranch(this,
                           parent_block, condition_operator,
                           condition, negate_first,
                           out_condition, negate_second,
                           branch_label);
  }
}

static void
Predicator_convertDiamondHammock(Predicator this, 
                                  BasicBlock parent, BasicBlock common_child)
{
  Operation parent_branch = Predicator_removeLastBranch(parent);
  Label label = Operation_getTargetLabel(parent_branch);
  Temporary condition = Operation_getArgument(parent_branch, 0); // FIXME! - HACK!
  BasicBlockEdge new_edge;
  int32_t if_true_index = -1;   // PHI variable selected on true
  int32_t if_false_index = -1;  // PHI variable selected on false
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(parent, edge) {
    BasicBlock child = BasicBlockEdge_DESTBLOCK(edge);
    BasicBlockEdge outgoing_edge = IDList_firstItem(BasicBlock_LEAVEEDGES(child));
    Selector child_selector = BasicBlock_selector(child);
    // Merge the blocks
    Predicator_removeLastBranch(child);
    Predicator_markSelectorSpeculated(child_selector);
    BasicBlock_pushSelector(parent, child_selector);
    if (BasicBlock_hasLabel(child, label)) {
      if_true_index = BasicBlockEdge_ARGINDEX(outgoing_edge);
    } else {
      if_false_index = BasicBlockEdge_ARGINDEX(outgoing_edge);
    }
    BasicBlock_killEdge(parent, edge);
    BasicBlock_killEdge(child, outgoing_edge);
    Procedure_unchainBasicBlock(BasicBlock_procedure(child), child);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // TODO! MD
  Predicator_swapIndexes(parent_branch, &if_true_index, &if_false_index);
  new_edge = BasicBlock_makeEdge(parent, common_child, 1.0);
  *BasicBlockEdge__ARGINDEX(new_edge) = if_true_index;
  // TODO! Check if it is OK to always use if_true index for common parameter.
  Predicator_fixPhiOperations(this, parent, common_child, 
                               condition, if_true_index, if_true_index, if_false_index);
  Predicator_fixTwoBlockOrder(parent, common_child);
}

#ifdef $XCC__h
void
Predicator_traverseRegion(Predicator this);
#endif//$XCC__h

void
Predicator_traverseRegion(Predicator this)
{
  BasicBlockStack postorder = Predicator_POSTORDER(this);
  BasicBlockStack_FOREACH_BasicBlock(postorder, block) {
    BasicBlock common_child = NULL;
    BasicBlock other_child = NULL;
    if (Predicator_isTriangleHammock(this, block, &common_child, &other_child)) {
      if (!Predicator_isTrianglePossible(this, block, common_child, other_child))
        continue;
      if (!Predicator_isTriangleProfitable(this, block, common_child, other_child))
        continue;
      CGO_TRACE && fprintf(CGO_TRACE, "*** Found triangle hammock : BB%d --> BB%d --> BB%d***\n",
                           BasicBlock_index(block),
                           BasicBlock_index(other_child),
                           BasicBlock_index(common_child));
      CGO_TRACE && fprintf(CGO_TRACE, "Parent Block: \n")
                && BasicBlock_pretty(block, CGO_TRACE)
                && fprintf(CGO_TRACE, "\n\n other_child: \n")
                && BasicBlock_pretty(other_child, CGO_TRACE)
                && fprintf(CGO_TRACE, "\n\n common_child: \n")
                && BasicBlock_pretty(common_child, CGO_TRACE);
      // Do the if-conversion!!!
      Predicator_convertTriangleHammock(this, block, common_child, other_child);
      CGO_TRACE && fprintf(CGO_TRACE, "\n\n After conversion: \n")
                && BasicBlock_pretty(block, CGO_TRACE)
                && fprintf(CGO_TRACE, "\n\n common_child: \n")
                && BasicBlock_pretty(common_child, CGO_TRACE);
#if 0
      CGO_TRACE && fprintf(CGO_TRACE, "\nAfter converting diamond hammock : \n")
                && CodeRegion_pretty(Predicator_CODEREGION(this), CGO_TRACE);
#endif
    } else if (Predicator_isDiamondHammock(this, block, &common_child)) {
      BasicBlock children[2]; 
      int32_t index = 0;
      if (!Predicator_isDiamondPossible(this, block, common_child))
        continue;
      if (!Predicator_isDiamondProfitable(this, block, common_child))
        continue;
      CGO_TRACE && fprintf(CGO_TRACE, "*** Found diamond hammock: BB%d --> BB%d ***\n",
                                      BasicBlock_index(block),
                                      BasicBlock_index(common_child))
                && fprintf(CGO_TRACE, "Parent Block: \n")
                && BasicBlock_pretty(block, CGO_TRACE)
                && fprintf(CGO_TRACE, "\n\n common_child: \n")
                && BasicBlock_pretty(common_child, CGO_TRACE);
      BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
        children[index++] = BasicBlockEdge_DESTBLOCK(edge);
      } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
      CGO_TRACE && fprintf(CGO_TRACE, "\n\nChild 0: \n")
                && BasicBlock_pretty(children[0], CGO_TRACE)
                && fprintf(CGO_TRACE, "\n\nChild 1: \n")
                && BasicBlock_pretty(children[1], CGO_TRACE);
      Predicator_convertDiamondHammock(this, block, common_child);
      CGO_TRACE && fprintf(CGO_TRACE, "\n\n After conversion: \n")
                && BasicBlock_pretty(block, CGO_TRACE)
                && fprintf(CGO_TRACE, "\n\n common_child: \n")
                && BasicBlock_pretty(common_child, CGO_TRACE);
#if 0
      CGO_TRACE && fprintf(CGO_TRACE, "\nAfter converting diamond hammock : \n");
                && CodeRegion_pretty(Predicator_CODEREGION(this), CGO_TRACE);
#endif
    } else {
#if 0
      CGO_TRACE && fprintf (CGO_TRACE, "BB%d is not a beginning of a hammock\n", 
                                       BasicBlock_index(block));
#endif
    }
  } BasicBlockStack_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
void
Predicator_DFS(Predicator this, BasicBlock block);
#endif//$XCC__h

void
Predicator_DFS(Predicator this, BasicBlock block)
{
  BasicBlockStack POSTORDER = Predicator_POSTORDER(this);
  bool *VISITED = Predicator_VISITED(this);
  int32_t block_index = BasicBlock_index(block);
  Except_CHECK(!VISITED[block_index]);  // Block should be entry of the region
  VISITED[block_index] = true;
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    BasicBlock dest_block = BasicBlockEdge_destBlock(edge);
    int32_t dest_index = BasicBlock_index(dest_block);
    if (!VISITED[dest_index]) {
      Predicator_DFS(this, dest_block);
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  BasicBlockStack_push(POSTORDER, block);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Predicator Memory.
 */
Memory
Predicator_memory(Predicator this);
#endif//$XCC_h

Memory
Predicator_memory(Predicator this)
{
  return Predicator_MEMORY(this);
}

#ifdef $XCC_h
/**
 * Pretty-print this Predicator.
 */
bool
Predicator_pretty(Predicator this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Predicator_pretty(Predicator this, FILE *file)
{
  if (file == NULL) file = stderr;
  return true;
}
#endif//_NTRACE

#ifdef $XCC__c
// Initialize Minir parser
{
  Minir minir = Minir_new(Memory_Root, stdin);
  Minir_KEEP_INDEX(minir) = true;
#endif//$XCC__c

#ifdef $XCC__c
if(0) {
  // Test the IF-conversion
  // Small example - one triangle hammock
  Predicator_next_test(minir);
}
#endif//$XCC__c

#ifdef $XCC__c
if(0) {
  // Test the IF-conversion
  // One triangle hammock with back-edge
  Predicator_next_test(minir);
}
#endif//$XCC__c

#ifdef $XCC__c
if(0) {
  // Test the IF-conversion
  // Small example - basic diamond hammock
  Predicator_next_test(minir);
}
#endif//$XCC__c

#ifdef $XCC__c
if(0) {
  // Test the IF-conversion
  // Example from Bruel's presentation.
  Predicator_next_test(minir);
}
#endif//$XCC__c

#ifdef $XCC__c
// Finalize Minir reading
  Minir_delete(minir);
}
#endif//$XCC__c


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Predicator module.
 */
#define Predicator_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Predicator module.
 */
#define Predicator_FINI()
#endif//$XCC__h

#if XCC__C
static void
Predicator_next_test(Minir minir)
{
  Program program = Minir_nextProgram(minir);
  Optimize optimize = Program_optimize(program);
  *Optimize__CONVERSION(optimize) = OptimizeConversion_Folding;
  Program_FOREACH_Procedure(program, procedure) {
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    SSAForm ssaForm = SSAForm_construct(Memory_Root, codeRegion, true);
    Predicator_delete(Predicator_new(Memory_Root, ssaForm));
  } Program_ENDEACH_Procedure;
  // FIXME: deletion makes next program segfault (probably Minir problem with 
  // handling of temporaries containing absolute values)
  //Program_delete(program);
}

static void
Predicator_TEST(void)
{
#include "CGO/Predicator_.c"
}

int
main(int argc, char **argv)
{
  CGO_Test(Predicator_TEST);
  return 0;
}
#endif

