#ifdef $XCC_h
/*
 * !!!!	LiveSets.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Fabrice Rastello (Fabrice.Rastello@ens-lyon.fr).
 *
 * Copyright 2007 - 2009 STMicroelectronics.
 * Copyright 2007 - 2009 INRIA Rhone-Alpes.
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
 * @brief SSA Live sets.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/LiveSets.h"
#endif//$XCC__h

#ifdef $XCC__h
/**
 * 0 => Loop forest based algorithm;
                                1 => Tiger Book (Appel) Use by Use;
                                2 => Tiger Book (Appel) Var by Var.
 */
#if _OPTIMIZE
//#define LiveSets_UPANDMARK 0
#endif//_OPTIMIZE
#ifndef LiveSets_UPANDMARK
extern int LiveSets_UPANDMARK;
#endif//LiveSets_UPANDMARK
#endif//$XCC__h

#ifndef LiveSets_UPANDMARK
int LiveSets_UPANDMARK = 0;
#endif//LiveSets_UPANDMARK

#ifdef $XCC__h
/**
 * 0 => Keep sets ordered;
 * 				1 => Compute unordered and reorder at the end;
 * 				2 => Preorder the Variable(s) and compute unordered.
 */
#if _OPTIMIZE
//#define LiveSets_UNORDERED 0
#endif//_OPTIMIZE
#ifndef LiveSets_UNORDERED
extern int LiveSets_UNORDERED;
#endif//LiveSets_UNORDERED
#endif//$XCC__h

#ifndef LiveSets_UNORDERED
int LiveSets_UNORDERED = 0;
#endif//LiveSets_UNORDERED

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Live sets for SSA Variable(s) using LoopForest(s).
 *
 * The LiveSets object maintains the contributions of PHI to live sets in separate sets, so
 * clients may use the results of analysis either in Multiplex mode or in Copy mode. In order to
 * achieve this, the PHI arguments are put in PHIUSESETS and also the upward exposed uses of each
 * block are initialized with the PHI arguments of the successor blocks. After the live sets are
 * computed, the PHIDEFSETS are filled with the PHI results of the block. Users in Multiplex mode
 * use LIVEINSETS U PHININSETS and LIVEOUTSETS U PHIUSESETS in place of LIVEINSETS and LIVEOUTSETS.
 */
struct LiveSets_;
#endif//$XCC_h

#ifdef $XCC__h
struct LiveSets_ {
  //@args	Memory memory, LoopForest loopForest, TemporaryTable temporaryTable
  Memory MEMORY;
  int32_t BLOCKCOUNT;
  LoopForest LOOPFOREST;
  //@access CODEREGION	LoopForest_CODEREGION(LiveSets_LOOPFOREST(this))
  TemporaryTable TEMPORARYTABLE;
  TemporarySet_ *LIVEINSETS;		// LiveInSets array.
  TemporarySet_ *LIVEOUTSETS;		// LiveOutSets array.
  TemporarySet_ *PHIDEFSETS;		// PhiInSets array.
  TemporarySet_ *PHIUSESETS;		// PhiOutSets array.
  TemporarySparse_ SPARSESET[1];	// Sparse set for computing Live in BasicBlock.
};
#endif//$XCC__h

LiveSets
LiveSets_Ctor(LiveSets this,
                 Memory memory, LoopForest loopForest, TemporaryTable temporaryTable)
{
  CodeRegion codeRegion = LoopForest_CODEREGION(loopForest);
  int32_t temporaryCount = TemporaryTable_count(temporaryTable);
  int32_t blockCount = CodeRegion_blockCount(codeRegion), index;
  TemporarySet_ *liveInSets = Memory_alloc(memory, sizeof(TemporarySet_)*blockCount);
  TemporarySet_ *liveOutSets = Memory_alloc(memory, sizeof(TemporarySet_)*blockCount);
  TemporarySet_ *phiDefSets = Memory_alloc(memory, sizeof(TemporarySet_)*blockCount);
  TemporarySet_ *phiUseSets = Memory_alloc(memory, sizeof(TemporarySet_)*blockCount);
  TemporarySparse_Ctor(LiveSets_SPARSESET(this), memory, temporaryTable);
  for (index = 0; index < blockCount; index++) {
    TemporarySet_Ctor(liveInSets + index, memory, Liveness_SETCOUNT);
    TemporarySet_Ctor(liveOutSets + index, memory, Liveness_SETCOUNT);
    TemporarySet_Ctor(phiDefSets + index, memory, Liveness_SETCOUNT);
    TemporarySet_Ctor(phiUseSets + index, memory, Liveness_SETCOUNT);
  }
  *LiveSets__MEMORY(this) = memory;
  *LiveSets__BLOCKCOUNT(this) = blockCount;
  *LiveSets__LOOPFOREST(this) = loopForest;
  *LiveSets__TEMPORARYTABLE(this) = temporaryTable;
  *LiveSets__LIVEINSETS(this) = liveInSets;
  *LiveSets__LIVEOUTSETS(this) = liveOutSets;
  *LiveSets__PHIDEFSETS(this) = phiDefSets;
  *LiveSets__PHIUSESETS(this) = phiUseSets;
  if (LiveSets_UPANDMARK==2) {
    if (LiveSets_UNORDERED==2) {
      TemporaryTable_makeOrder(temporaryTable);
    }
    LiveSets_computeSetsByVar(this, codeRegion, temporaryTable);
    if (LiveSets_UNORDERED==1) {
      LiveSets_orderPhiSets(this);
      LiveSets_orderLiveSets(this);
    }
  } else if (LiveSets_UPANDMARK==1) {
    LiveSets_computeSetsByUse(this, codeRegion, temporaryTable);
  } else {
    BasicBlock rootBlock = CodeRegion_rootBlock(codeRegion);
    LoopForestNode rootNode = LoopForest_ROOTNODE(loopForest);
    bool *visited = memset(alloca(sizeof(bool)*blockCount), false, sizeof(bool)*blockCount);
    LiveSets_computePhiSets(this, codeRegion, temporaryTable);
    if (LiveSets_UNORDERED==1) {
      LiveSets_orderPhiSets(this);
    }
    LiveSets_computeSetsBackwardDAG(this, rootBlock, visited);
    if (LiveSets_UNORDERED==1) {
      LiveSets_orderLiveSets(this);
    }
    LiveSets_computeSetsForwardForest(this, rootNode);
  }
  return this;
}

void
LiveSets_Dtor(LiveSets this)
{
  Memory memory = LiveSets_MEMORY(this);
  int32_t blockCount = LiveSets_BLOCKCOUNT(this), index;
  TemporaryTable temporaryTable = LiveSets_TEMPORARYTABLE(this);
  int32_t temporaryCount = TemporaryTable_count(temporaryTable), maxLiveCount = 0;
  TemporarySparse_Dtor(LiveSets_SPARSESET(this));
  for (index = 0; index < blockCount; index++) {
    int liveInCount = TemporarySet_count(LiveSets_LIVEINSETS(this) + index);
    int liveOutCount = TemporarySet_count(LiveSets_LIVEOUTSETS(this) + index);
    int phiDefCount = TemporarySet_count(LiveSets_PHIDEFSETS(this) + index);
    int phiUseCount = TemporarySet_count(LiveSets_PHIUSESETS(this) + index);
    TemporarySet_Dtor(LiveSets_LIVEINSETS(this) + index);
    TemporarySet_Dtor(LiveSets_LIVEOUTSETS(this) + index);
    TemporarySet_Dtor(LiveSets_PHIDEFSETS(this) + index);
    TemporarySet_Dtor(LiveSets_PHIUSESETS(this) + index);
    if (maxLiveCount < liveInCount) maxLiveCount = liveInCount;
    if (maxLiveCount < liveOutCount) maxLiveCount = liveOutCount;
    if (maxLiveCount < phiDefCount) maxLiveCount = phiDefCount;
    if (maxLiveCount < phiUseCount) maxLiveCount = phiUseCount;
  }
//fprintf(stderr, "*** temporaryCount=%d\tmaxLiveCount=%d\n", temporaryCount, maxLiveCount);
}

size_t
LiveSets_Size(Memory memory, LoopForest loopForest, TemporaryTable temporaryTable)
{
  return sizeof(LiveSets_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LiveSets_)\t%zu\n", sizeof(LiveSets_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Make a LiveSets object.
 */
LiveSets
LiveSets_make(Memory parent, LoopForest loopForest, TemporaryTable temporaryTable);
#endif//$XCC_h

LiveSets
LiveSets_make(Memory parent, LoopForest loopForest, TemporaryTable temporaryTable)
{
  Memory memory = Memory_new(parent, true);
  size_t size = LiveSets_Size(memory, loopForest, temporaryTable);
  LiveSets this = Memory_alloc(memory, size);
  LiveSets_Ctor(this, memory, loopForest, temporaryTable);
  CodeRegion_setLiveSets(LiveSets_CODEREGION(this),
                         LiveSets_BLOCKCOUNT(this),
                         LiveSets_LIVEINSETS(this),
                         LiveSets_LIVEOUTSETS(this),
                         LiveSets_PHIDEFSETS(this),
                         LiveSets_PHIUSESETS(this));
  return this;
}

#ifdef $XCC_h
LiveSets
LiveSets_kill(LiveSets this);
#endif//$XCC_h

LiveSets
LiveSets_kill(LiveSets this)
{
  if (this != NULL) {
    Memory memory = LiveSets_MEMORY(this);
    CodeRegion codeRegion = LiveSets_CODEREGION(this);
    CodeRegion_resetLiveSets(codeRegion);
#ifndef _NDTOR
    LiveSets_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Compute the phiDefSets and the phiUseSets.
 */
void
LiveSets_computePhiSets(LiveSets this, CodeRegion codeRegion, TemporaryTable temporaryTable);
#endif//$XCC__h

void
LiveSets_computePhiSets(LiveSets this, CodeRegion codeRegion, TemporaryTable temporaryTable)
{
  TemporarySet_ *phiDefSets = LiveSets_PHIDEFSETS(this);
  TemporarySet_ *phiUseSets = LiveSets_PHIUSESETS(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, block_b) {
    int32_t index_b = BasicBlock_index(block_b);
    BasicBlock_FOREACH_Operation(block_b, operation) {
      if (Operator_isPhi(Operation_operator(operation))) {
        int32_t argCount = Operation_argCount(operation), i;
        Temporary *arguments = Operation_arguments(operation);
        Temporary result = Operation_results(operation)[0];
        BasicBlock_ENTER_FOREACH_BasicBlockEdge(block_b, edge) {
          int32_t argIndex = BasicBlockEdge_ARGINDEX(edge);
          BasicBlock block_p = BasicBlockEdge_origBlock(edge);
          int32_t index_p = BasicBlock_index(block_p);
          Temporary argument = arguments[argIndex];
          if (TemporaryTable_contains(temporaryTable, argument)) {
            if (LiveSets_UNORDERED) {
              TemporarySet_pushMember(phiUseSets + index_p, argument);
            } else {
              TemporarySet_insert(phiUseSets + index_p, argument);
            }
          }
        } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
        if (TemporaryTable_contains(temporaryTable, result)) {
          if (LiveSets_UNORDERED) {
            TemporarySet_pushMember(phiDefSets + index_b, result);
          } else {
            TemporarySet_insert(phiDefSets + index_b, result);
          }
        }
      } else break;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC_h
/**
 * In case phi-sets are not ordered, order them.
 */
void
LiveSets_orderPhiSets(LiveSets this);
#endif//$XCC_h

void
LiveSets_orderPhiSets(LiveSets this)
{
  CodeRegion codeRegion = LiveSets_CODEREGION(this);
  TemporarySet_ *phiDefSets = LiveSets_PHIDEFSETS(this);
  TemporarySet_ *phiUseSets = LiveSets_PHIUSESETS(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    int32_t index = BasicBlock_index(basicBlock);
    TemporarySet_makeOrder(phiDefSets + index);
    TemporarySet_makeOrder(phiUseSets + index);
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC_h
/**
 * In case live-sets are not ordered, order them.
 */
void
LiveSets_orderLiveSets(LiveSets this);
#endif//$XCC_h

void
LiveSets_orderLiveSets(LiveSets this)
{
  CodeRegion codeRegion = LiveSets_CODEREGION(this);
  TemporarySet_ *liveInSets = LiveSets_LIVEINSETS(this);
  TemporarySet_ *liveOutSets = LiveSets_LIVEOUTSETS(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    int32_t index = BasicBlock_index(basicBlock);
    TemporarySet_makeOrder(liveInSets + index);
    TemporarySet_makeOrder(liveOutSets + index);
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * Compute live sets using the Appel (Tiger Book) method.
 *
 * Require ordered phi-sets and construct ordered live-sets.
 */
void
LiveSets_computeSetsByUse(LiveSets this, CodeRegion codeRegion, TemporaryTable temporaryTable);
#endif//$XCC__h

void
LiveSets_computeSetsByUse(LiveSets this, CodeRegion codeRegion, TemporaryTable temporaryTable)
{
  TemporarySet_ *phiUseSets = LiveSets_PHIUSESETS(this);
  // for each basic block B in CFG do
  CodeRegion_FOREACH_BasicBlock(codeRegion, block_b) {
    int32_t index_b = BasicBlock_index(block_b);
    TemporarySet phiUseSet_b = phiUseSets + index_b;
    const BasicBlockEdge *enterEdges = BasicBlock_enterEdges(block_b);
    // for each v used at exit of B do
      // LiveOut(B) = LiveOut(B) U { v } (not necessary with phiUseSets)
      // Up_and_Mark(B, v)
    BasicBlock_FOREACH_Operation(block_b, operation) {
      if (Operator_isPhi(Operation_operator(operation))) {
        Temporary *arguments = Operation_arguments(operation);
        int32_t argCount = Operation_argCount(operation), argIndex;
        for (argIndex = 0; argIndex < argCount; argIndex++) {
          Temporary argument = arguments[argIndex];
          BasicBlockEdge edge = enterEdges[argIndex];
          BasicBlock block_p = BasicBlockEdge_origBlock(edge);
          int32_t index_p = BasicBlock_index(block_p);
          if (TemporaryTable_contains(temporaryTable, argument)) {
            TemporarySet_insert(phiUseSets + index_p, argument);
            LiveSets_walkUpAndMarkByUse(this, temporaryTable, block_p, argument);
          }
        }
      } else {
    // for each v used in B (\phi excluded) do
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (TemporaryTable_contains(temporaryTable, argument)) {
            // Up_and_Mark(B, v)
            LiveSets_walkUpAndMarkByUse(this, temporaryTable, block_b, argument);
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * Called by LiveSets_computeSetsByUse.
 */
void
LiveSets_walkUpAndMarkByUse(LiveSets this, TemporaryTable temporaryTable, BasicBlock block_b, Temporary temporary);
#endif//$XCC__h

void
LiveSets_walkUpAndMarkByUse(LiveSets this, TemporaryTable temporaryTable, BasicBlock block_b, Temporary temporary)
{
  int32_t index_b = BasicBlock_index(block_b);
  Variable variable = Temporary_VARIABLE(temporary);
  TemporarySet_ *liveOutSets = LiveSets_LIVEOUTSETS(this);
  TemporarySet liveInSet_b = LiveSets_LIVEINSETS(this) + index_b;
  TemporarySet phiDefSet_b = LiveSets_PHIDEFSETS(this) + index_b;
  Operation operation_d = OperationResult_OPERATION(Variable_PRODUCER(variable));
  Except_CHECK(TemporaryTable_contains(temporaryTable, temporary));
  if (Operation_basicBlock(operation_d) == block_b) {
    // if def(v) \in B (entry excluded) then return
    if (Operator_isPhi(Operation_operator(operation_d))) {
      Temporary result = Operation_results(operation_d)[0];
      // LiveIn(B) = LiveIn(B) U { v }
      if (TemporaryTable_contains(temporaryTable, result)) {
        TemporarySet_insert(phiDefSet_b, result);
      }
      // if v defined at the entry of B then return
    }
    return;
  }
  // if v \in LiveIn(B) then return
  if (TemporarySet_insert(liveInSet_b, temporary)) {
    // for P \in CFG_preds(B) do
    BasicBlock_ENTER_FOREACH_BasicBlockEdge(block_b, edge) {
      BasicBlock block_p = BasicBlockEdge_origBlock(edge);
      TemporarySet liveOutSet_p = liveOutSets + BasicBlock_index(block_p);
      // LiveOut(P) = LiveOut(P) U { v }
      TemporarySet_insert(liveOutSet_p, temporary);
      // Up_and_Mark(P, v)
      LiveSets_walkUpAndMarkByUse(this, temporaryTable, block_p, temporary);
    } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  }
}

#ifdef $XCC__h
/**
 * Improved Appel technique.
 */
void
LiveSets_computeSetsByVar(LiveSets this, CodeRegion codeRegion, TemporaryTable temporaryTable);
#endif//$XCC__h

void
LiveSets_computeSetsByVar(LiveSets this, CodeRegion codeRegion, TemporaryTable temporaryTable)
{
  TemporarySet_ *phiUseSets = LiveSets_PHIUSESETS(this);
  // for each variable v do
  TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
    Variable variable = Temporary_VARIABLE(temporary);
    Except_CHECK(Temporary_isVariable(temporary));
    Except_CHECK(Variable_isConsuming(variable));
    // for each use of v, in a block B do
    Variable_CONSUMERS_FOREACH_OperationArgument(variable, argument) {
      // if v used at exit of B do
      Operation operation_u = OperationArgument_OPERATION(argument);
      BasicBlock block_b = Operation_basicBlock(operation_u);
      if (Operator_isPhi(Operation_operator(operation_u))) {
        int32_t argIndex = OperationArgument_ARGINDEX(argument);
        BasicBlockEdge edge = BasicBlock_getPhiEnterEdge(block_b, argIndex);
        block_b = BasicBlockEdge_origBlock(edge);
        // if top(LiveOut(B)) != v then push(LiveOut(B),v)
        if (LiveSets_UNORDERED) {
          int32_t index_b = BasicBlock_index(block_b);
          TemporarySet phiUseSet_b = phiUseSets + index_b;
          if (   TemporarySet_isEmpty(phiUseSet_b)
              || TemporarySet_past(phiUseSet_b)[-1]  != temporary) {
            TemporarySet_pushMember(phiUseSet_b, temporary);
          }
        } else {
          int32_t index_b = BasicBlock_index(block_b);
          TemporarySet phiUseSet_b = phiUseSets + index_b;
          TemporarySet_insert(phiUseSet_b, temporary);
        }
      }
      // Up_and_Mark_Stack(B, v)
      LiveSets_walkUpAndMarkByVar(this, block_b, temporary);
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
  } TemporaryTable_ENDEACH_Temporary;
}

#ifdef $XCC__h
/**
 * Called by LiveSets_computeSetsByVar.
 */
void
LiveSets_walkUpAndMarkByVar(LiveSets this, BasicBlock block_b, Temporary temporary);
#endif//$XCC__h

void
LiveSets_walkUpAndMarkByVar(LiveSets this, BasicBlock block_b, Temporary temporary)
{
  int32_t index_b = BasicBlock_index(block_b);
  Variable variable = Temporary_VARIABLE(temporary);
  TemporarySet_ *liveOutSets = LiveSets_LIVEOUTSETS(this);
  TemporarySet liveInSet_b = LiveSets_LIVEINSETS(this) + index_b;
  TemporarySet phiDefSet_b = LiveSets_PHIDEFSETS(this) + index_b;
  Operation operation_d = OperationResult_OPERATION(Variable_PRODUCER(variable));
  if (Operation_basicBlock(operation_d) == block_b) {
    // if def(v) \in B (entry excluded) then return
    if (Operator_isPhi(Operation_operator(operation_d))) {
      Temporary result = Operation_results(operation_d)[0];
      // LiveIn(B) = LiveIn(B) U { v }
      if (LiveSets_UNORDERED) {
        if (   TemporarySet_isEmpty(phiDefSet_b)
            || TemporarySet_past(phiDefSet_b)[-1]  != temporary) {
          TemporarySet_pushMember(phiDefSet_b, temporary);
        }
      } else {
        TemporarySet_insert(phiDefSet_b, result);
      }
      // if v defined at the entry of B then return
    }
    return;
  }
  // if top(LiveIn(B)) = v then return
  // push(LiveIn(B),v)
  if (LiveSets_UNORDERED) {
    if (   TemporarySet_isEmpty(liveInSet_b)
        || TemporarySet_past(liveInSet_b)[-1] != temporary) {
      TemporarySet_pushMember(liveInSet_b, temporary);
      // for P \in CFG_preds(B) do
      BasicBlock_ENTER_FOREACH_BasicBlockEdge(block_b, edge) {
        BasicBlock block_p = BasicBlockEdge_origBlock(edge);
        TemporarySet liveOutSet_p = liveOutSets + BasicBlock_index(block_p);
        // if (top(LiveOut(P)) != v then push(LiveOut(P),v)
        // LiveOut(P) U= { v }
        if (   TemporarySet_isEmpty(liveOutSet_p)
            || TemporarySet_past(liveOutSet_p)[-1]  != temporary) {
          TemporarySet_pushMember(liveOutSet_p, temporary);
        }
        // Up_and_Mark_Stack(P, v)
        LiveSets_walkUpAndMarkByVar(this, block_p, temporary);
      } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
    }
  } else {
    if (TemporarySet_insert(liveInSet_b, temporary)) {
      // for P \in CFG_preds(B) do
      BasicBlock_ENTER_FOREACH_BasicBlockEdge(block_b, edge) {
        BasicBlock block_p = BasicBlockEdge_origBlock(edge);
        TemporarySet liveOutSet_p = liveOutSets + BasicBlock_index(block_p);
        // if (top(LiveOut(P)) != v then push(LiveOut(P),v)
        // LiveOut(P) U= { v }
        TemporarySet_insert(liveOutSet_p, temporary);
        // Up_and_Mark_Stack(P, v)
        LiveSets_walkUpAndMarkByVar(this, block_p, temporary);
      } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Implement DAG_DFS from Boissinot et al.
 */
void
LiveSets_computeSetsBackwardDAG(LiveSets this, BasicBlock block_b, bool visited[]);
#endif//$XCC__h

void
LiveSets_computeSetsBackwardDAG(LiveSets this, BasicBlock block_b, bool visited[])
{
  LoopForest loopForest = LiveSets_LOOPFOREST(this);
  LoopForestNode_ *blockNodes = LoopForest_BLOCKNODES(loopForest);
  TemporaryTable temporaryTable = LiveSets_TEMPORARYTABLE(this);
  TemporarySparse sparseLive = LiveSets_SPARSESET(this);
  TemporarySet_ *liveInSets = LiveSets_LIVEINSETS(this);
  TemporarySet_ *liveOutSets = LiveSets_LIVEOUTSETS(this);
  TemporarySet_ *phiDefSets = LiveSets_PHIDEFSETS(this);
  TemporarySet_ *phiUseSets = LiveSets_PHIUSESETS(this);
  int32_t index_b = BasicBlock_index(block_b);
  LoopForestNode node_b = blockNodes + index_b;
  TemporarySet liveOutSet_b = liveOutSets + index_b;
  TemporarySet liveInSet_b = liveInSets + index_b;
  TemporarySet phiUseSet_b = phiUseSets + index_b;
  TemporarySet phiDefSet_b = phiDefSets + index_b;
  // for each S \in succ(B) such that (B, S) is not a loop edge do
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block_b, edge) {
    BasicBlock block_s = BasicBlockEdge_destBlock(edge);
    int32_t index_s = BasicBlock_index(block_s);
    LoopForestNode node_s = blockNodes + index_s;
    if (index_s && !BasicBlockEdge_ISLOOP(edge)) {
      // H := HnCA(B, S)
      LoopForestNode node_h = LoopForest_findHNCA(loopForest, node_b, node_s);
      BasicBlock block_h = LoopForestNode_BLOCK(node_h);
      int32_t index_h = BasicBlock_index(block_h);
      // if S not processed then DAG_DFS(S)
      if (!visited[index_h]) {
        LiveSets_computeSetsBackwardDAG(this, block_h, visited);
      }
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  TemporarySparse_empty(sparseLive);
  // for each S \in succs(B) such that (B, S) is not a loop edge do
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block_b, edge) {
    BasicBlock block_s = BasicBlockEdge_destBlock(edge);
    int32_t index_s = BasicBlock_index(block_s);
    LoopForestNode node_s = blockNodes + index_s;
    if (index_s && !BasicBlockEdge_ISLOOP(edge)) {
      // h := HnCA(b, s)
      LoopForestNode node_h = LoopForest_findHNCA(loopForest, node_b, node_s);
      BasicBlock block_h = LoopForestNode_BLOCK(node_h);
      // Live U= LiveIn(S) - PhiDefs(S)
      int32_t index_h = BasicBlock_index(block_h);
      TemporarySet liveInSet_h = liveInSets + index_h;
      TemporarySet_FOREACH_Temporary(liveInSet_h, temporary) {
        TemporarySparse_insert(sparseLive, temporary);
      } TemporarySet_ENDEACH_Temporary;
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // LiveOut(B) = Live
  Except_CHECK(TemporarySet_isEmpty(liveOutSet_b));
  TemporarySparse_FOREACH_Temporary(sparseLive, temporary) {
    TemporarySet_insert(liveOutSet_b, temporary);
  } TemporarySparse_ENDEACH_Temporary;
  // Live U= the set of variables used at the exit of B
  TemporarySet_FOREACH_Temporary(phiUseSet_b, temporary) {
    TemporarySparse_insert(sparseLive, temporary);
  } TemporarySet_ENDEACH_Temporary;
  // for each p \in B backward do
  BasicBlock_FORBACK_Operation(block_b, operation) {
    if (!Operator_isPhi(Operation_operator(operation))) {
      // remove killing definitions at p from Live
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        if (TemporaryTable_contains(temporaryTable, result)) {
          TemporarySparse_remove(sparseLive, result);
        }
      } Operation_RESULTS_ENDEACH_Temporary;
      // add uses at p in Live
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        if (TemporaryTable_contains(temporaryTable, argument)) {
          TemporarySparse_insert(sparseLive, argument);
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
    } else break;
  } BasicBlock_ENDBACK_Operation;
  // LiveIn(B) = Live U PhiDefs(B)
  TemporarySet_FOREACH_Temporary(phiDefSet_b, temporary) {
    TemporarySparse_remove(sparseLive, temporary);
  } TemporarySet_ENDEACH_Temporary;
  TemporarySparse_FOREACH_Temporary(sparseLive, temporary) {
    TemporarySet_insert(liveInSet_b, temporary);
  } TemporarySparse_ENDEACH_Temporary;
  // mark b as visited
  visited[index_b] = true;
}

#ifdef $XCC__h
/**
 * Implement LoopTree_DFS from Boissinot CC'10.
 */
void
LiveSets_computeSetsForwardForest(LiveSets this, LoopForestNode node_b);
#endif//$XCC__h

void
LiveSets_computeSetsForwardForest(LiveSets this, LoopForestNode node_b)
{
  TemporarySet_ *liveInSets = LiveSets_LIVEINSETS(this);
  TemporarySet_ *liveOutSets = LiveSets_LIVEOUTSETS(this);
  BasicBlock block_b = LoopForestNode_BLOCK(node_b);
  int32_t index_b = BasicBlock_index(block_b);
  TemporarySet liveInSet_b = liveInSets + index_b;
  TemporarySet liveOutSet_b = liveOutSets + index_b;
  Except_CHECK(LoopForestNode_TYPE(node_b) > LoopForestNodeType_Block);
//fprintf(stderr, "    ForwardForest(Block_%d)\n", index_b);
  LoopForestNode_CHILDREN_FOREACH_LoopForestNode(node_b, node_c) {
    BasicBlock block_c = LoopForestNode_BLOCK(node_c);
    int32_t index_c = BasicBlock_index(block_c);
    if (index_b > 0) {
      // Do not propagate root live sets to non-loop children.
      if (index_c != index_b) {
//fprintf(stderr, "\tliveInSet_%d ", index_c), TemporarySet_pretty(liveInSets + index_c, stderr), fprintf(stderr, " U= ");
        TemporarySet_union(liveInSets + index_c, liveInSet_b);
//fprintf(stderr, "liveInSet_%d ", index_b), TemporarySet_pretty(liveInSet_b, stderr), fprintf(stderr, "\n");
      }
//fprintf(stderr, "\tliveOutSet_%d ", index_c), TemporarySet_pretty(liveOutSets + index_c, stderr), fprintf(stderr, " U= ");
      TemporarySet_union(liveOutSets + index_c, liveInSet_b);
//fprintf(stderr, "liveOutSet_%d ", index_b), TemporarySet_pretty(liveInSet_b, stderr), fprintf(stderr, "\n");
    }
    if (LoopForestNode_TYPE(node_c) > LoopForestNodeType_Block) {
      LiveSets_computeSetsForwardForest(this, node_c);
    }
  } LoopForestNode_CHILDREN_ENDEACH_LoopForestNode;
}

#ifdef $XCC_h
/**
 * Check the correctness of the live-in and live-out sets.
 */
bool
LiveSets_check(const_LiveSets this, const_Liveness liveness);
#endif//$XCC_h

bool
LiveSets_check(const_LiveSets this, const_Liveness liveness)
{
  CodeRegion codeRegion = LiveSets_CODEREGION(this);
  if (!TemporarySet_isEmpty(Liveness_LIVEINSETS(liveness) + 1)) {
    fprintf(stderr, "*** NON STRICT PROGRAM, skipping checks! (%s:%d)\n", __FILE__, __LINE__);
    return false;
  }
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    int32_t index = BasicBlock_index(block);
    TemporarySet this_liveInSet = LiveSets_LIVEINSETS(this) + index;
    TemporarySet this_liveOutSet = LiveSets_LIVEOUTSETS(this) + index;
    TemporarySet this_phiDefSet = LiveSets_PHIDEFSETS(this) + index;
    TemporarySet this_phiUseSet = LiveSets_PHIUSESETS(this) + index;
    TemporarySet that_liveInSet = Liveness_LIVEINSETS(liveness) + index;
    TemporarySet that_liveOutSet = Liveness_LIVEOUTSETS(liveness) + index;
    TemporarySet that_phiDefSet = Liveness_PHIDEFSETS(liveness) + index;
    TemporarySet that_phiUseSet = Liveness_PHIUSESETS(liveness) + index;
    TemporaryTable temporaryTable = LiveSets_TEMPORARYTABLE(this);
    TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
      if (TemporarySet_contains(that_liveInSet, temporary)) {
        Except_CHECK(TemporarySet_contains(this_liveInSet, (temporary)));
      } else {
        Except_CHECK(!TemporarySet_contains(this_liveInSet, (temporary)));
      }
      if (TemporarySet_contains(that_liveOutSet, temporary)) {
        Except_CHECK(TemporarySet_contains(this_liveOutSet, (temporary)));
      } else {
        Except_CHECK(!TemporarySet_contains(this_liveOutSet, (temporary)));
      }
      if (TemporarySet_contains(that_phiDefSet, temporary)) {
        Except_CHECK(TemporarySet_contains(this_phiDefSet, (temporary)));
      } else {
        Except_CHECK(!TemporarySet_contains(this_phiDefSet, (temporary)));
      }
      if (TemporarySet_contains(that_phiUseSet, temporary)) {
        Except_CHECK(TemporarySet_contains(this_phiUseSet, (temporary)));
      } else {
        Except_CHECK(!TemporarySet_contains(this_phiUseSet, (temporary)));
      }
    } TemporaryTable_ENDEACH_Temporary;
  } CodeRegion_ENDEACH_BasicBlock;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Live bitsets for SSA Variable(s) using LoopForest(s).
 *
 * The LiveBitSets object maintains the contributions of PHI to live sets in separate sets, so
 * clients may use the results of analysis either in Multiplex mode or in Copy mode. In order to
 * achieve this, the PHI arguments are put in PHIUSESETS and also the upward exposed uses of each
 * block are initialized with the PHI arguments of the successor blocks. After the live sets are
 * computed, the PHIDEFSETS are filled with the PHI results of the block. Users in Multiplex mode
 * use LIVEINSETS U PHININSETS and LIVEOUTSETS U PHIUSESETS in place of LIVEINSETS and LIVEOUTSETS.
 */
struct LiveBitSets_;
#endif//$XCC_h

#ifdef $XCC__h
struct LiveBitSets_ {
  //@args	Memory memory, LoopForest loopForest, TemporaryTable temporaryTable
  Memory MEMORY;
  int32_t BLOCKCOUNT;
  LoopForest LOOPFOREST;
  //@access CODEREGION	LoopForest_CODEREGION(LiveBitSets_LOOPFOREST(this))
  TemporaryTable TEMPORARYTABLE;
  BitSet_ *LIVEINSETS;		// LiveInSets array.
  BitSet_ *LIVEOUTSETS;		// LiveOutSets array.
  BitSet_ *PHIDEFSETS;		// PhiInSets array.
  BitSet_ *PHIUSESETS;		// PhiOutSets array.
  TemporarySparse_ SPARSESET[1];	// Sparse set for computing Live in BasicBlock.
};
#endif//$XCC__h

LiveBitSets
LiveBitSets_Ctor(LiveBitSets this,
                    Memory memory, LoopForest loopForest, TemporaryTable temporaryTable)
{
  CodeRegion codeRegion = LoopForest_CODEREGION(loopForest);
  int32_t temporaryCount = TemporaryTable_count(temporaryTable);
  int32_t blockCount = CodeRegion_blockCount(codeRegion), index;
  BitSet_ *liveInSets = Memory_alloc(memory, sizeof(BitSet_)*blockCount);
  BitSet_ *liveOutSets = Memory_alloc(memory, sizeof(BitSet_)*blockCount);
  BitSet_ *phiDefSets = Memory_alloc(memory, sizeof(BitSet_)*blockCount);
  BitSet_ *phiUseSets = Memory_alloc(memory, sizeof(BitSet_)*blockCount);
  TemporarySparse_Ctor(LiveBitSets_SPARSESET(this), memory, temporaryTable);
  for (index = 0; index < blockCount; index++) {
    BitSet_Ctor(liveInSets + index, memory, temporaryCount);
    BitSet_Ctor(liveOutSets + index, memory, temporaryCount);
    BitSet_Ctor(phiDefSets + index, memory, temporaryCount);
    BitSet_Ctor(phiUseSets + index, memory, temporaryCount);
  }
  *LiveBitSets__MEMORY(this) = memory;
  *LiveBitSets__BLOCKCOUNT(this) = blockCount;
  *LiveBitSets__LOOPFOREST(this) = loopForest;
  *LiveBitSets__TEMPORARYTABLE(this) = temporaryTable;
  *LiveBitSets__LIVEINSETS(this) = liveInSets;
  *LiveBitSets__LIVEOUTSETS(this) = liveOutSets;
  *LiveBitSets__PHIDEFSETS(this) = phiDefSets;
  *LiveBitSets__PHIUSESETS(this) = phiUseSets;
  LiveBitSets_computePhiSets(this, codeRegion, temporaryTable);
  if (LiveSets_UPANDMARK==2) {
    LiveBitSets_computeSetsByVar(this, codeRegion, temporaryTable);
  } else if (LiveSets_UPANDMARK==1) {
    LiveBitSets_computeSetsByUse(this, codeRegion, temporaryTable);
  } else {
    BasicBlock rootBlock = CodeRegion_rootBlock(codeRegion);
    LoopForestNode rootNode = LoopForest_ROOTNODE(loopForest);
    bool *visited = memset(alloca(sizeof(bool)*blockCount), false, sizeof(bool)*blockCount);
//LoopForest_pretty(loopForest, stderr, NULL);
    LiveBitSets_computeSetsBackwardDAG(this, rootBlock, visited);
    LiveBitSets_computeSetsForwardForest(this, rootNode);
  }
  return this;
}

void
LiveBitSets_Dtor(LiveBitSets this)
{
  Memory memory = LiveBitSets_MEMORY(this);
  int32_t blockCount = LiveBitSets_BLOCKCOUNT(this), index;
  TemporaryTable temporaryTable = LiveBitSets_TEMPORARYTABLE(this);
  TemporarySparse_Dtor(LiveBitSets_SPARSESET(this));
  for (index = 0; index < blockCount; index++) {
    BitSet_Dtor(LiveBitSets_LIVEINSETS(this) + index);
    BitSet_Dtor(LiveBitSets_LIVEOUTSETS(this) + index);
    BitSet_Dtor(LiveBitSets_PHIDEFSETS(this) + index);
    BitSet_Dtor(LiveBitSets_PHIUSESETS(this) + index);
  }
}

size_t
LiveBitSets_Size(Memory memory, LoopForest loopForest, TemporaryTable temporaryTable)
{
  return sizeof(LiveBitSets_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LiveBitSets_)\t%zu\n", sizeof(LiveBitSets_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Make a LiveBitSets object.
 */
LiveBitSets
LiveBitSets_make(Memory parent, LoopForest loopForest, TemporaryTable temporaryTable);
#endif//$XCC_h

LiveBitSets
LiveBitSets_make(Memory parent, LoopForest loopForest, TemporaryTable temporaryTable)
{
  Memory memory = Memory_new(parent, true);
  size_t size = LiveBitSets_Size(memory, loopForest, temporaryTable);
  LiveBitSets this = Memory_alloc(memory, size);
  LiveBitSets_Ctor(this, memory, loopForest, temporaryTable);
#if 0
  CodeRegion_setLiveSets(LiveBitSets_CODEREGION(this),
                         LiveBitSets_BLOCKCOUNT(this),
                         LiveBitSets_LIVEINSETS(this),
                         LiveBitSets_LIVEOUTSETS(this),
                         LiveBitSets_PHIDEFSETS(this),
                         LiveBitSets_PHIUSESETS(this));
#endif
  return this;
}

#ifdef $XCC_h
LiveBitSets
LiveBitSets_kill(LiveBitSets this);
#endif//$XCC_h

LiveBitSets
LiveBitSets_kill(LiveBitSets this)
{
  if (this != NULL) {
    Memory memory = LiveBitSets_MEMORY(this);
    CodeRegion codeRegion = LiveBitSets_CODEREGION(this);
#if 0
    CodeRegion_resetLiveSets(codeRegion);
#endif
#ifndef _NDTOR
    LiveBitSets_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Compute the phiDefSets and the phiUseSets.
 */
void
LiveBitSets_computePhiSets(LiveBitSets this, CodeRegion codeRegion, TemporaryTable temporaryTable);
#endif//$XCC__h

void
LiveBitSets_computePhiSets(LiveBitSets this, CodeRegion codeRegion, TemporaryTable temporaryTable)
{
  BitSet_ *phiDefSets = LiveBitSets_PHIDEFSETS(this);
  BitSet_ *phiUseSets = LiveBitSets_PHIUSESETS(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, block_b) {
    int32_t index_b = BasicBlock_index(block_b);
    BasicBlock_FOREACH_Operation(block_b, operation) {
      if (Operator_isPhi(Operation_operator(operation))) {
        int32_t argCount = Operation_argCount(operation), i;
        Temporary *arguments = Operation_arguments(operation);
        Temporary result = Operation_results(operation)[0];
        BasicBlock_ENTER_FOREACH_BasicBlockEdge(block_b, edge) {
          int32_t argIndex = BasicBlockEdge_ARGINDEX(edge);
          BasicBlock block_p = BasicBlockEdge_origBlock(edge);
          int32_t index_p = BasicBlock_index(block_p);
          Temporary argument = arguments[argIndex];
          if (TemporaryTable_contains(temporaryTable, argument)) {
            BitSet_insert(phiUseSets + index_p, Temporary_INDEX(argument));
          }
        } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
        if (TemporaryTable_contains(temporaryTable, result)) {
          BitSet_insert(phiDefSets + index_b, Temporary_INDEX(result));
        }
      } else break;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * Compute live sets using the Appel (Tiger Book) method.
 */
void
LiveBitSets_computeSetsByUse(LiveBitSets this, CodeRegion codeRegion, TemporaryTable temporaryTable);
#endif//$XCC__h

void
LiveBitSets_computeSetsByUse(LiveBitSets this, CodeRegion codeRegion, TemporaryTable temporaryTable)
{
  const Temporary *temporaries = TemporaryTable_base(temporaryTable);
  BitSet_ *phiUseSets = LiveBitSets_PHIUSESETS(this);
  // for B basic block in code region do
  CodeRegion_FOREACH_BasicBlock(codeRegion, block_b) {
    int32_t index_b = BasicBlock_index(block_b);
    BitSet phiUseSet_b = phiUseSets + index_b;
    // for v used in phiuse_edge(B) do
    BitSet_FOREACH(phiUseSet_b, temporary_index) {
      Temporary temporary = temporaries[temporary_index];
      // LiveOut(B) U= { v } (not necessary with phiUseSets)
      // Up_and_Mark(B, v)
      LiveBitSets_walkUpAndMark(this, block_b, temporary);
    } BitSet_ENDEACH;
    // for v used in B (\phi excluded) do
    BasicBlock_FOREACH_Operation(block_b, operation) {
      if (!Operator_isPhi(Operation_operator(operation))) {
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (TemporaryTable_contains(temporaryTable, argument)) {
            // Up_and_Mark(B, v)
            LiveBitSets_walkUpAndMark(this, block_b, argument);
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * Walk up the CFG and update the live sets for a particular Variable.
 */
void
LiveBitSets_walkUpAndMark(LiveBitSets this, BasicBlock block_b, Temporary temporary);
#endif//$XCC__h

void
LiveBitSets_walkUpAndMark(LiveBitSets this, BasicBlock block_b, Temporary temporary)
{
  int32_t index_b = BasicBlock_index(block_b);
  Variable variable = Temporary_VARIABLE(temporary);
  BitSet liveInSet_b = LiveBitSets_LIVEINSETS(this) + index_b;
  // if def(v) \in B (phi excluded) then return
  Operation operation = OperationResult_OPERATION(Variable_PRODUCER(variable));
  if (Operation_basicBlock(operation) == block_b) return;
  // if v \in LiveIn(B) then return
  // LiveIn(B) U= { v }
  if (BitSet_insert(liveInSet_b, Temporary_INDEX(temporary))) {
    // for P \in CFG_preds(B) do
    BitSet_ *liveOutSets = LiveBitSets_LIVEOUTSETS(this);
    BasicBlock_ENTER_FOREACH_BasicBlockEdge(block_b, edge) {
      BasicBlock block_p = BasicBlockEdge_origBlock(edge);
      BitSet liveOutSet_p = liveOutSets + BasicBlock_index(block_p);
      // LiveOut(P) U= { v }
      BitSet_insert(liveOutSet_p, Temporary_INDEX(temporary));
      // Up_and_Mark(P, v)
      LiveBitSets_walkUpAndMark(this, block_p, temporary);
    } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  }
}

#ifdef $XCC__h
/**
 * Improved Appel technique.
 */
void
LiveBitSets_computeSetsByVar(LiveBitSets this, CodeRegion codeRegion, TemporaryTable temporaryTable);
#endif//$XCC__h

void
LiveBitSets_computeSetsByVar(LiveBitSets this, CodeRegion codeRegion, TemporaryTable temporaryTable)
{
  int32_t blockCount = LiveBitSets_BLOCKCOUNT(this);
  Temporary *processed = memset(alloca(sizeof(Temporary)*blockCount), 0, sizeof(Temporary)*blockCount);
  BitSet_ *liveInSets = LiveBitSets_LIVEINSETS(this);
  BitSet_ *liveOutSets = LiveBitSets_LIVEOUTSETS(this);
  TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
    Variable variable = Temporary_VARIABLE(temporary);
    Except_CHECK(Temporary_isVariable(temporary));
    Except_CHECK(Variable_isConsuming(variable));
    Variable_CONSUMERS_FOREACH_OperationArgument(variable, argument) {
      Operation operation_u = OperationArgument_OPERATION(argument);
      BasicBlock block_u = Operation_basicBlock(operation_u);
      if (Operator_isPhi(Operation_operator(operation_u))) {
        // Correct block_u in case of PHI argument.
        int32_t argIndex = OperationArgument_ARGINDEX(argument);
        BasicBlockEdge edge = BasicBlock_getPhiEnterEdge(block_u, argIndex);
        block_u = BasicBlockEdge_origBlock(edge);
      }
      LiveBitSets_walkUpAndMark2(this, block_u, temporary, processed);
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
  } TemporaryTable_ENDEACH_Temporary;
}

#ifdef $XCC__h
/**
 * 
 */
void
LiveBitSets_walkUpAndMark2(LiveBitSets this, BasicBlock block_b, Temporary temporary, Temporary processed[]);
#endif//$XCC__h

void
LiveBitSets_walkUpAndMark2(LiveBitSets this, BasicBlock block_b, Temporary temporary, Temporary processed[])
{
  int32_t index_b = BasicBlock_index(block_b);
  Variable variable = Temporary_VARIABLE(temporary);
  BitSet_ *liveOutSets = LiveBitSets_LIVEOUTSETS(this);
  BitSet liveInSet_b = LiveBitSets_LIVEINSETS(this) + index_b;
  // if def(v) \in B (entry excluded) then return
  Operation operation_d = OperationResult_OPERATION(Variable_PRODUCER(variable));
  if (Operation_basicBlock(operation_d) == block_b) return;
  // if v \in LiveIn(B) then return
  // LiveIn(B) U= { v }
  if (processed[index_b] != temporary) {
    processed[index_b] = temporary;
    BitSet_insert(liveInSet_b, Temporary_INDEX(temporary));
    // for P \in CFG_preds(B) do
    BasicBlock_ENTER_FOREACH_BasicBlockEdge(block_b, edge) {
      BasicBlock block_p = BasicBlockEdge_origBlock(edge);
      BitSet liveOutSet_p = liveOutSets + BasicBlock_index(block_p);
      // LiveOut(P) U= { v }
      BitSet_insert(liveOutSet_p, Temporary_INDEX(temporary));
      // Up_and_Mark(P, v)
      LiveBitSets_walkUpAndMark2(this, block_p, temporary, processed);
    } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Implement DAG_DFS from Boissinot CC'10.
 */
void
LiveBitSets_computeSetsBackwardDAG(LiveBitSets this, BasicBlock block_b, bool visited[]);
#endif//$XCC__h

void
LiveBitSets_computeSetsBackwardDAG(LiveBitSets this, BasicBlock block_b, bool visited[])
{
  LoopForest loopForest = LiveBitSets_LOOPFOREST(this);
  LoopForestNode_ *blockNodes = LoopForest_BLOCKNODES(loopForest);
  TemporaryTable temporaryTable = LiveBitSets_TEMPORARYTABLE(this);
  const Temporary *temporaries = TemporaryTable_base(temporaryTable);
  BitSet_ *liveInSets = LiveBitSets_LIVEINSETS(this);
  BitSet_ *liveOutSets = LiveBitSets_LIVEOUTSETS(this);
  BitSet_ *phiDefSets = LiveBitSets_PHIDEFSETS(this);
  BitSet_ *phiUseSets = LiveBitSets_PHIUSESETS(this);
  int32_t index_b = BasicBlock_index(block_b);
  LoopForestNode node_b = blockNodes + index_b;
  BitSet liveOutSet_b = liveOutSets + index_b;
  BitSet liveInSet_b = liveInSets + index_b;
  BitSet phiUseSet_b = phiUseSets + index_b;
  BitSet phiDefSet_b = phiDefSets + index_b;
  Except_CHECK(BitSet_isEmpty(liveOutSet_b));
  // forall s in succ(b) such that (b, s) is not a back edge
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block_b, edge) {
    BasicBlock block_s = BasicBlockEdge_destBlock(edge);
    int32_t index_s = BasicBlock_index(block_s);
    LoopForestNode node_s = blockNodes + index_s;
    if (index_s && !BasicBlockEdge_ISLOOP(edge)) {
      // h := HnCA(b, s)
      LoopForestNode node_h = LoopForest_findHNCA(loopForest, node_b, node_s);
      BasicBlock block_h = LoopForestNode_BLOCK(node_h);
      int32_t index_h = BasicBlock_index(block_h);
      BitSet liveInSet_h = liveInSets + index_h;
      // if not visited DAG_DFS(s)
      if (!visited[index_h]) {
        LiveBitSets_computeSetsBackwardDAG(this, block_h, visited);
      }
      // LiveOut(b) U= LiveIn(s) - PhiDefs(s) (PhiDefs already removed from LiveIn)`
      BitSet_union(liveOutSet_b, liveInSet_h);
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // Let Live-in: variables used in phiuse_edge(b)
  Except_CHECK(BitSet_isEmpty(liveInSet_b));
  BitSet_assign(liveInSet_b, phiUseSet_b);
  // Live-in U= LiveOut(b)
  BitSet_union(liveInSet_b, liveOutSet_b);
  // for back (non-phi) inst in b do
  BasicBlock_FORBACK_Operation(block_b, operation) {
    if (!Operator_isPhi(Operation_operator(operation))) {
      // remove killing def of inst from Live
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        if (TemporaryTable_contains(temporaryTable, result)) {
          BitSet_remove(liveInSet_b, Temporary_INDEX(result));
        }
      } Operation_RESULTS_ENDEACH_Temporary;
      // add uses of inst (if not phi) in Live
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        if (TemporaryTable_contains(temporaryTable, argument)) {
          BitSet_insert(liveInSet_b, Temporary_INDEX(argument));
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
    } else break;
  } BasicBlock_ENDBACK_Operation;
  // LiveIn(b) = Live - PhiDef(b)
  BitSet_diff(liveInSet_b, phiDefSet_b);
//fprintf(stderr, "\tliveInSet_%d := ", index_b), BitSet_pretty(liveInSet_b, stderr), fprintf(stderr, "\n");
  // mark b as visited
  visited[index_b] = true;
}

#ifdef $XCC__h
/**
 * Implement LoopTree_DFS from Boissinot CC'10.
 */
void
LiveBitSets_computeSetsForwardForest(LiveBitSets this, LoopForestNode node_b);
#endif//$XCC__h

void
LiveBitSets_computeSetsForwardForest(LiveBitSets this, LoopForestNode node_b)
{
  BitSet_ *liveInSets = LiveBitSets_LIVEINSETS(this);
  BitSet_ *liveOutSets = LiveBitSets_LIVEOUTSETS(this);
  BasicBlock block_b = LoopForestNode_BLOCK(node_b);
  int32_t index_b = BasicBlock_index(block_b);
  BitSet liveInSet_b = liveInSets + index_b;
  BitSet liveOutSet_b = liveOutSets + index_b;
  Except_CHECK(LoopForestNode_TYPE(node_b) > LoopForestNodeType_Block);
//fprintf(stderr, "    ForwardForest(Block_%d)\n", index_b);
  LoopForestNode_CHILDREN_FOREACH_LoopForestNode(node_b, node_c) {
    BasicBlock block_c = LoopForestNode_BLOCK(node_c);
    int32_t index_c = BasicBlock_index(block_c);
    if (index_b > 0) {
      // Do not propagate root live sets to non-loop children.
      if (index_c != index_b) {
//fprintf(stderr, "\tliveInSet_%d ", index_c), BitSet_pretty(liveInSets + index_c, stderr), fprintf(stderr, " U= ");
        BitSet_union(liveInSets + index_c, liveInSet_b);
//fprintf(stderr, "liveInSet_%d ", index_b), BitSet_pretty(liveInSet_b, stderr), fprintf(stderr, "\n");
      }
//fprintf(stderr, "\tliveOutSet_%d ", index_c), BitSet_pretty(liveOutSets + index_c, stderr), fprintf(stderr, " U= ");
      BitSet_union(liveOutSets + index_c, liveInSet_b);
//fprintf(stderr, "liveOutSet_%d ", index_b), BitSet_pretty(liveInSet_b, stderr), fprintf(stderr, "\n");
    }
    if (LoopForestNode_TYPE(node_c) > LoopForestNodeType_Block) {
      LiveBitSets_computeSetsForwardForest(this, node_c);
    }
  } LoopForestNode_CHILDREN_ENDEACH_LoopForestNode;
}

#ifdef $XCC_h
/**
 * Check the correctness of the live-in and live-out BitSet(s).
 */
bool
LiveBitSets_check(const_LiveBitSets this, const_Liveness liveness);
#endif//$XCC_h

bool
LiveBitSets_check(const_LiveBitSets this, const_Liveness liveness)
{
  CodeRegion codeRegion = LiveBitSets_CODEREGION(this);
  if (!TemporarySet_isEmpty(Liveness_LIVEINSETS(liveness) + 1)) {
    fprintf(stderr, "*** NON STRICT PROGRAM, skipping checks! (%s:%d)\n", __FILE__, __LINE__);
    return false;
  }
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    int32_t index = BasicBlock_index(block);
    BitSet this_liveInSet = LiveBitSets_LIVEINSETS(this) + index;
    BitSet this_liveOutSet = LiveBitSets_LIVEOUTSETS(this) + index;
    BitSet this_phiDefSet = LiveBitSets_PHIDEFSETS(this) + index;
    BitSet this_phiUseSet = LiveBitSets_PHIUSESETS(this) + index;
    TemporarySet that_liveInSet = Liveness_LIVEINSETS(liveness) + index;
    TemporarySet that_liveOutSet = Liveness_LIVEOUTSETS(liveness) + index;
    TemporarySet that_phiDefSet = Liveness_PHIDEFSETS(liveness) + index;
    TemporarySet that_phiUseSet = Liveness_PHIUSESETS(liveness) + index;
    TemporaryTable temporaryTable = LiveBitSets_TEMPORARYTABLE(this);
    TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
      if (TemporarySet_contains(that_liveInSet, temporary)) {
        Except_CHECK(BitSet_contains(this_liveInSet, Temporary_INDEX(temporary)));
      } else {
        Except_CHECK(!BitSet_contains(this_liveInSet, Temporary_INDEX(temporary)));
      }
      if (TemporarySet_contains(that_liveOutSet, temporary)) {
        Except_CHECK(BitSet_contains(this_liveOutSet, Temporary_INDEX(temporary)));
      } else {
        Except_CHECK(!BitSet_contains(this_liveOutSet, Temporary_INDEX(temporary)));
      }
      if (TemporarySet_contains(that_phiDefSet, temporary)) {
        Except_CHECK(BitSet_contains(this_phiDefSet, Temporary_INDEX(temporary)));
      } else {
        Except_CHECK(!BitSet_contains(this_phiDefSet, Temporary_INDEX(temporary)));
      }
      if (TemporarySet_contains(that_phiUseSet, temporary)) {
        Except_CHECK(BitSet_contains(this_phiUseSet, Temporary_INDEX(temporary)));
      } else {
        Except_CHECK(!BitSet_contains(this_phiUseSet, Temporary_INDEX(temporary)));
      }
    } TemporaryTable_ENDEACH_Temporary;
  } CodeRegion_ENDEACH_BasicBlock;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the LiveSets module.
 */
void
LiveSets_INIT(void);
#endif//$XCC__h

void
LiveSets_INIT(void)
{
#ifndef LiveSets_UPANDMARK
  if (GETENV("LiveSets_UPANDMARK")) {
    sscanf(GETENV("LiveSets_UPANDMARK"), "%d", &LiveSets_UPANDMARK);
    fprintf(stderr, "*** LiveSets_UPANDMARK=%d\n", LiveSets_UPANDMARK);
  }
#endif//LiveSets_UPANDMARK
#ifndef LiveSets_UNORDERED
  if (GETENV("LiveSets_UNORDERED")) {
    sscanf(GETENV("LiveSets_UNORDERED"), "%d", &LiveSets_UNORDERED);
    fprintf(stderr, "*** LiveSets_UNORDERED=%d\n", LiveSets_UNORDERED);
  }
#endif//LiveSets_UNORDERED
}

#ifdef $XCC__h
/**
 * Finalize the LiveSets module.
 */
void
LiveSets_FINI(void);
#endif//$XCC__h

void
LiveSets_FINI(void)
{
}

#if XCC__C
static void
LiveSets_TEST(void)
{
#include "PFA/LiveSets_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(LiveSets_TEST);
  return 0;
}
#endif

