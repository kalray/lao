#ifdef $XCC_h
/*
 * !!!!	LiveCheck.xcc
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
 * @brief SSA Live checks.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/LiveCheck.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Live checking for SSA Variable(s) using LoopForest(s).
 *
 */
struct LiveCheck_;
#endif//$XCC_h

#ifdef $XCC__h
struct LiveCheck_ {
  //@args	Memory memory, Dominance dominance, LoopForest loopForest
  Memory MEMORY;
  int32_t BLOCKCOUNT;
  Dominance DOMINANCE;
  //@access CODEREGION	Dominance_CODEREGION(LiveCheck_DOMINANCE(this))
  //@access PREDFN	Dominance_PREDFN(LiveCheck_DOMINANCE(this))
  //@access LASTDFN	Dominance_LASTDFN(LiveCheck_DOMINANCE(this))
  //@access BLOCKMAP	Dominance_BLOCKMAP(LiveCheck_DOMINANCE(this))
  //@access ROOTBLOCK	(LiveCheck_BLOCKMAP(this)[0])
  LoopForest LOOPFOREST;
  BitSet_ *REACHSETS;			// Reduced reachable nodes.
};
#endif//$XCC__h

LiveCheck
LiveCheck_Ctor(LiveCheck this,
                  Memory memory, Dominance dominance, LoopForest loopForest)
{
  CodeRegion codeRegion = Dominance_CODEREGION(dominance);
  const BasicBlock *blockMap = Dominance_BLOCKMAP(dominance);
  int32_t blockCount = Dominance_BLOCKCOUNT(dominance), index;
  BitSet_ *reachSets = Memory_alloc(memory, sizeof(BitSet_)*blockCount);
  bool *visited = memset(alloca(sizeof(bool)*blockCount), false, sizeof(bool)*blockCount);
  *LiveCheck__MEMORY(this) = memory;
  *LiveCheck__BLOCKCOUNT(this) = blockCount;
  *LiveCheck__DOMINANCE(this) = dominance;
  *LiveCheck__LOOPFOREST(this) = loopForest;
  for (index = 0; index < blockCount; index++) {
    BitSet_Ctor(reachSets + index, memory, blockCount);
  }
  *LiveCheck__REACHSETS(this) = reachSets;
  // Compute the reachSets by DFS of the modified CFG.
  LiveCheck_computeR(this, blockMap[0], visited);
  return this;
}

void
LiveCheck_Dtor(LiveCheck this)
{
  Memory memory = LiveCheck_MEMORY(this);
  int32_t blockCount = LiveCheck_BLOCKCOUNT(this), index;
  BitSet_ *reachSets = LiveCheck_REACHSETS(this);
  for (index = 0; index < blockCount; index++) {
    BitSet_Dtor(reachSets + index);
  }
  Memory_free(memory, reachSets);
}

size_t
LiveCheck_Size(Memory memory, Dominance dominance, LoopForest loopForest)
{
  return sizeof(LiveCheck_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LiveCheck_)\t%zu\n", sizeof(LiveCheck_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Make a LiveCheck object.
 */
LiveCheck
LiveCheck_make(Memory parent, Dominance dominance, LoopForest loopForest);
#endif//$XCC_h

LiveCheck
LiveCheck_make(Memory parent, Dominance dominance, LoopForest loopForest)
{
  Memory memory = Memory_new(parent, true);
  size_t size = LiveCheck_Size(memory, dominance, loopForest);
  LiveCheck this = Memory_alloc(memory, size);
  LiveCheck_Ctor(this, memory, dominance, loopForest);
  return this;
}

#ifdef $XCC_h
LiveCheck
LiveCheck_kill(LiveCheck this);
#endif//$XCC_h

LiveCheck
LiveCheck_kill(LiveCheck this)
{
  if (this != NULL) {
    Memory memory = LiveCheck_MEMORY(this);
#ifndef _NDTOR
    LiveCheck_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
bool
LiveCheck_pretty(const_LiveCheck this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
LiveCheck_pretty(const_LiveCheck this, FILE *file)
{
  BitSet_ *reachSets = LiveCheck_REACHSETS(this);
  int32_t blockCount = LiveCheck_BLOCKCOUNT(this), index;
  if (file == NULL) file = stderr;
  fprintf(file, "Reduced Reachable Sets:\n");
  for (index = 0; index < blockCount; index++) {
    fprintf(file, "   Block_%d:\t{ ", index);
    BitSet_FOREACH(reachSets + index, index_r) {
      fprintf(file, " Block_%d", index_r);
    } BitSet_ENDEACH;
    fprintf(file, " }\n");
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Compute the reduced reachability sets by DFS on the modified CFG.
 */
void
LiveCheck_computeR(LiveCheck this, BasicBlock block_b, bool visited[]);
#endif//$XCC__h

void
LiveCheck_computeR(LiveCheck this, BasicBlock block_b, bool visited[])
{
  int32_t index_b = BasicBlock_index(block_b);
  BitSet_ *reachSets = LiveCheck_REACHSETS(this);
  LoopForest loopForest = LiveCheck_LOOPFOREST(this);
  LoopForestNode_ *blockNodes = LoopForest_BLOCKNODES(loopForest);
  LoopForestNode node_b = blockNodes + index_b;
  // bitset_set( R(b), index(b) )
  BitSet_insert(reachSets + index_b, index_b);
  // forall s in succ(b) \ loopTree_ancestor(b)
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block_b, edge) {
    BasicBlock block_s = BasicBlockEdge_destBlock(edge);
    int32_t index_s = BasicBlock_index(block_s);
    LoopForestNode node_s = blockNodes + index_s;
    // In case of the Bourdoncle / Havlak forest, enough to filter out the loop back edges.
    if (index_s && !BasicBlockEdge_ISLOOP(edge)) {
      // h := HnCA(b, s)
      LoopForestNode node_h = LoopForest_findHNCA(loopForest, node_b, node_s);
      BasicBlock block_h = LoopForestNode_BLOCK(node_h);
      int32_t index_h = BasicBlock_index(block_h);
      // DFS_Compute_R(h)
      if (!visited[index_s]) {
        LiveCheck_computeR(this, block_h, visited);
      }
      // R(b) := bitset_or( R(b), R(h) )
      BitSet_union(reachSets + index_b, reachSets + index_h);
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // mark b as visited
  visited[index_b] = true;
}

#ifdef $XCC_h
/**
 * Live-In Check according to Algorithm 1.
 */
bool
LiveCheck_isLive(const_LiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable variable_a);
#define LiveCheck_isLiveInOrdering(this, block_q, ordering_q, variable_a, mode) \
	LiveCheck_isLive(this, block_q, ordering_q, variable_a)
#define LiveCheck_isLiveOutOrdering(this, block_q, ordering_q, variable_a, mode) \
	LiveCheck_isLive(this, block_q, (ordering_q) + 1, variable_a)
#define LiveCheck_isLiveInBasicBlock(this, block_q, variable_a, mode) \
	LiveCheck_isLive(this, block_q, BasicBlock_firstOrdering(block_q), variable_a)
#define LiveCheck_isLiveOutBasicBlock(this, block_q, variable_a, mode) \
	LiveCheck_isLive(this, block_q, BasicBlock_lastOrdering(block_q) + 1, variable_a)
#endif//$XCC_h

bool
LiveCheck_isLive(const_LiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable variable_a)
{
  int32_t *preDfn = LiveCheck_PREDFN(this);
  int32_t *lastDfn = LiveCheck_LASTDFN(this);
  // d := def(a)
  Operation operation_d = OperationResult_OPERATION(Variable_PRODUCER(variable_a));
  BasicBlock block_d = Operation_basicBlock(operation_d);
  // if not (d.BB dom q.BB) then return false
  int32_t index_d = BasicBlock_index(block_d);
  int32_t index_q = BasicBlock_index(block_q);
  int32_t preDfn_d = preDfn[index_d];
  int32_t lastDfn_d = lastDfn[index_d];
  int32_t preDfn_q = preDfn[index_q];
  if (preDfn_d > preDfn_q || lastDfn_d < preDfn_q) return false;
  Except_CHECK(Dominance_DOMINATES(preDfn, lastDfn, index_d, index_q));
  // if q.BB == d.BB then
  if (block_q == block_d) {
    int32_t ordering_d = Operation_ordering(operation_d);
    // Correct ordering_d in case of PHI.
    if (Operator_isPhi(Operation_operator(operation_d))) {
      if (Operation_isCopyMode(operation_d)) {
        ordering_d = BasicBlock_earlyOrdering(block_d);
      } else {
        ordering_d = BasicBlock_firstOrdering(block_d) - 1;
      }
    }
    // if not (d.BB == q.BB and d.order < q.order) then return false
    if (ordering_d >= ordering_q) return false;
    // for u in uses(a) do
    Variable_CONSUMERS_FOREACH_OperationArgument(variable_a, argument) {
      Operation operation_u = OperationArgument_OPERATION(argument);
      BasicBlock block_u = Operation_basicBlock(operation_u);
      int32_t ordering_u = Operation_ordering(operation_u);
      // Correct block_u and ordering_u in case of PHI.
      if (Operator_isPhi(Operation_operator(operation_u))) {
        int32_t argIndex = OperationArgument_ARGINDEX(argument);
        BasicBlockEdge edge = BasicBlock_getPhiEnterEdge(block_u, argIndex);
        block_u = BasicBlockEdge_origBlock(edge);
        if (Operation_isCopyMode(operation_u)) {
          ordering_u = BasicBlock_lateOrdering(block_u);
        } else {
          ordering_u = BasicBlock_lastOrdering(block_u) + 1;
        }
      }
      // if u.BB != d.BB or q.order <= u.order then return true
      if (block_u != block_d || ordering_q <= ordering_u) return true;
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
    return false;
  } else {
    BitSet_ *reachSets = LiveCheck_REACHSETS(this);
    // H := q.BB.outermostLoopExcluding(d.BB)
    LoopForest loopForest = LiveCheck_LOOPFOREST(this);
    LoopForestNode_ *blockNodes = LoopForest_BLOCKNODES(loopForest);
    LoopForestNode node_q = blockNodes + index_q, node_d = blockNodes + index_d;
    LoopForestNode node_h = LoopForest_findHNCA(loopForest, node_d, node_q);
    BasicBlock block_h = LoopForestNode_BLOCK(node_h);
    int32_t index_h = BasicBlock_index(block_h);
    // for u in uses(a) do
    Variable_CONSUMERS_FOREACH_OperationArgument(variable_a, argument) {
      Operation operation_u = OperationArgument_OPERATION(argument);
      BasicBlock block_u = Operation_basicBlock(operation_u);
      int32_t ordering_u = Operation_ordering(operation_u);
      // Correct block_u and ordering_u in case of PHI.
      if (Operator_isPhi(Operation_operator(operation_u))) {
        int32_t argIndex = OperationArgument_ARGINDEX(argument);
        BasicBlockEdge edge = BasicBlock_getPhiEnterEdge(block_u, argIndex);
        block_u = BasicBlockEdge_origBlock(edge);
        if (Operation_isCopyMode(operation_u)) {
          ordering_u = BasicBlock_lateOrdering(block_u);
        } else {
          ordering_u = BasicBlock_lastOrdering(block_u) + 1;
        }
      }
      // if (not H.isLoopHeader) and u.BB == q.BB and u.order < q.order then continue
      if (!BasicBlock_isWidening(block_h) && block_u == block_q && ordering_u < ordering_q) continue;
      // if u.BB in H.reducedReachable then return true
      if (BitSet_contains(reachSets + index_h, BasicBlock_index(block_u))) return true;
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
    return false;
  }
}

#ifdef $XCC_h
/**
 * Check the correctness of the live-in and live-out BitSet(s).
 */
bool
LiveCheck_check(const_LiveCheck this, const_TemporaryTable temporaryTable);
#endif//$XCC_h

bool
LiveCheck_check(const_LiveCheck this, const_TemporaryTable temporaryTable)
{
  CodeRegion codeRegion = LiveCheck_CODEREGION(this);
  // Getting live-in set of first basic block (index = 1)
  if (!TemporarySet_isEmpty(CodeRegion_liveInSets(codeRegion) + 1)) {
    fprintf(stderr, "*** NON STRICT PROGRAM, skipping checks! (%s:%d)\n", __FILE__, __LINE__);
    fprintf(stderr, "*** liveIn of CodeRegion contains the following temporaries:");
    TemporarySet_FOREACH_Temporary(CodeRegion_liveInSets(codeRegion) + 1,
                                   temporary) {
      fprintf(stderr, "\n\t");
      Temporary_pretty(temporary, stderr);
    } TemporarySet_ENDEACH_Temporary;
    fprintf(stderr, "\n");
    return false;
  }
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    int32_t index = BasicBlock_index(block);
    TemporarySet liveInSet = CodeRegion_liveInSets(codeRegion) + index;
    TemporarySet liveOutSet = CodeRegion_liveOutSets(codeRegion) + index;
    TemporarySet phiDefSet = CodeRegion_phiDefSets(codeRegion) + index;
    TemporarySet phiUseSet = CodeRegion_phiUseSets(codeRegion) + index;
    TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
      Variable variable = Temporary_VARIABLE(temporary);
      Except_CHECK(Temporary_isVariable(temporary));
      if (IDList_count(Variable_consumers(variable))) {
        if (   TemporarySet_contains(liveInSet, temporary)
            || TemporarySet_contains(phiDefSet, temporary)) {
          Except_CHECK(LiveCheck_isLiveInBasicBlock(this, block, variable, 0));
        } else {
          Except_CHECK(!LiveCheck_isLiveInBasicBlock(this, block, variable, 0));
        }
        if (   TemporarySet_contains(liveOutSet, temporary)
            || TemporarySet_contains(phiUseSet, temporary)) {
          Except_CHECK(LiveCheck_isLiveOutBasicBlock(this, block, variable, 0));
        } else {
          Except_CHECK(!LiveCheck_isLiveOutBasicBlock(this, block, variable, 0));
        }
      }
    } TemporaryTable_ENDEACH_Temporary;
  } CodeRegion_ENDEACH_BasicBlock;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
// Minir initialization for tests
{
  Minir minir = Minir_new(Memory_Root, stdin);
  Program program;
#endif//$XCC__c

#ifdef $XCC__c
  // Test LiveCheck_ComputeR.
  program = Minir_nextProgram(minir);
  {
    Procedure procedure = Program_findProcedure(program, "live-check");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Dominance dominance = Dominance_new(Memory_Root, codeRegion, DominanceFlag_Forward);
    LoopForest loopForest = LoopForest_new(Memory_Root, codeRegion);
    {
      LiveCheck liveCheck = LiveCheck_make(Memory_Root, dominance, loopForest);
      LiveCheck_pretty(liveCheck, stdout);
      LiveCheck_kill(liveCheck);
    }
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
// Delete Minir constructs
  Minir_delete(minir);
}
#endif//$XCC__c



////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the LiveCheck module.
 */
void
LiveCheck_INIT(void);
#endif//$XCC__h

void
LiveCheck_INIT(void)
{
}

#ifdef $XCC__h
/**
 * Finalize the LiveCheck module.
 */
void
LiveCheck_FINI(void);
#endif//$XCC__h

void
LiveCheck_FINI(void)
{
}

#if XCC__C
static void
LiveCheck_TEST(void)
{
#include "PFA/LiveCheck_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(LiveCheck_TEST);
  return 0;
}
#endif

