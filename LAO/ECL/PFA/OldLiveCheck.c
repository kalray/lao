#ifdef $XCC_h
/*
 * !!!!	OldLiveCheck.xcc
 *
 * Benoit Boissinot (Benoit.Boissinot@ens-lyon.fr).
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Sebastian Hack (hackseb@gmail.com).
 * Fabrice Rastello (Fabrice.Rastello@ens-lyon.fr).
 *
 * Copyright 2007 - 2008 STMicroelectronics.
 * Copyright 2007 - 2008 INRIA Rhone-Alpes.
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
 * @brief Variable Live Check.
 */
#endif//$XCC_h


#ifdef $XCC_h
/*
 * This module implements the "fast liveness checking" method published in CGO 2008.
 * Benoit Boissinot, Benoit Dupont-de-Dinechin, Daniel Grund, Sebastian Hack, and Fabrice Rastello.
 * Fast Liveness Checking for SSA-Form Programs.
 * International Symposium on Code Generation and Optimization (CGO'08).
 * http://perso.ens-lyon.fr/fabrice.rastello/Biblio_Perso/Articles/CGO2008.pdf
 *
 * OldLiveCheck_make requires the dominance. It precomputes some
 *  sets that only depend on the structure of the CFG.
 * In practice, if the CFG is modified, or if basicBlock_INDEX is modified those sets
 *   have to be recomputed.
 * Then using those precomputed sets, checking if a Variable (it must be an SSA variable!)
 *   is live{in/out} of a basicBlock/instruction, using either the mplex or the copy
 *   mode (this mode defines the semantic of a phi function. See below) can be done in O(#uses).
 * The liveness check requires the def-use chains to be updated.
 * The liveness check at operation granularity requires Operation_ordering to be updated.
 * Notice that checking if a variable is livein of a phi will generate an exception failure.
 */
#endif//$XCC_h

#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/OldLiveCheck.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * If not zero, copy live sets from CodeRegion.
 * 0 => use OldLiveCheck, 1 => copy from CodeRegion, 2 => both for debug.
 */
#if _OPTIMIZE
//#define OldLiveCheck_LIVENESS 0
#endif//_OPTIMIZE
#ifndef OldLiveCheck_LIVENESS
extern int OldLiveCheck_LIVENESS;
#endif//OldLiveCheck_LIVENESS
#endif//$XCC_h

#ifndef OldLiveCheck_LIVENESS
int OldLiveCheck_LIVENESS = 1;
#endif//OldLiveCheck_LIVENESS

#ifdef $XCC__h
/**
 * If not zero, use Das & Ramakrishna improvements.
 * 1 => use merge sets and DJ graph technique, 2 => replace dominator tree climb-up.
 */
//#define OldLiveCheck_MERGESETS 0
#ifndef OldLiveCheck_MERGESETS
extern int OldLiveCheck_MERGESETS;
#endif//OldLiveCheck_MERGESETS
#endif//$XCC__h

#ifndef OldLiveCheck_MERGESETS
int OldLiveCheck_MERGESETS = 0;
#endif//OldLiveCheck_MERGESETS

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Liveness check.
 */
struct OldLiveCheck_;
#endif//$XCC_h

#ifdef $XCC__h
struct OldLiveCheck_ {
  //@args	Memory memory, Dominance dominance, TemporaryTable temporaryTable
  Memory MEMORY;
  int32_t BLOCKCOUNT;
  Dominance DOMINANCE;
  //@access CODEREGION	Dominance_CODEREGION(OldLiveCheck_DOMINANCE(this))
  //@access PREDFN	Dominance_PREDFN(OldLiveCheck_DOMINANCE(this))
  //@access LASTDFN	Dominance_LASTDFN(OldLiveCheck_DOMINANCE(this))
  //@access BLOCKMAP	Dominance_BLOCKMAP(OldLiveCheck_DOMINANCE(this))
  //@access ROOTBLOCK	(OldLiveCheck_BLOCKMAP(this)[0])
  uint8_t *DFSFLAGS;	// 0: not processed yet; 1: currently processing it; 2: processed.
  int32_t *DFNMAP;	// Map PREDFN to BasicBlock index: DFNMAP = PREDFN^(-1).
  BitSet_ BACKHEADS[1];	// Heads of back-edges.
  BitSet_ *RSETS;	// Reduced reachable nodes (as opposed to the paper a node is not reduced-reachable from itself)
  BitSet_ *HSETS;	// Heads of reduced reachable back-edges: Tt^ (as opposed to the paper Tt^ can contain t)
  BitSet_ *TSETS;	// Sets of highest nodes: Tq
  BitSet_ *MSSETS;	// M(s,a) sets for Das New LiveOut.
  TemporarySet_ *LIVEINSETS;	// Live-in sets from CodeRegion.
  TemporarySet_ *LIVEOUTSETS;	// Live-out sets from CodeRegion.
  TemporarySet_ *PHIDEFSETS;	// Phi-in sets from CodeRegion.
  TemporarySet_ *PHIUSESETS;	// Phi-out sets from CodeRegion.
};
#endif//$XCC__h

// For experiments in CC'2010 submission.
static int SumInstanceCount = 0;
static int SumVariableCount = 0;
static int SumBlockCount = 0;
static int SumMergeSize = 0;
static int SumMsSize = 0;
static int SumHqSize = 0;
static int SumTqSize = 0;
static int SumRtSize = 0;

OldLiveCheck
OldLiveCheck_Ctor(OldLiveCheck this,
               Memory memory, Dominance dominance, TemporaryTable temporaryTable)
{
  int32_t blockCount = Dominance_BLOCKCOUNT(dominance), index;
  const BasicBlock *blockMap = Dominance_BLOCKMAP(dominance);
  *OldLiveCheck__MEMORY(this) = memory;
  *OldLiveCheck__BLOCKCOUNT(this) = blockCount;
  *OldLiveCheck__DOMINANCE(this) = dominance;
  SumInstanceCount++;
  SumVariableCount += TemporaryTable_count(temporaryTable);
  SumBlockCount += blockCount;
  Except_CHECK(Dominance_isForward(dominance));
  if (OldLiveCheck_LIVENESS==0 || OldLiveCheck_LIVENESS==2) {
    if (OldLiveCheck_MERGESETS>0) {
      int countArrays = 2;
      // Compute the MSSETS.
      BitSet_ *dominators = Dominance_DOMINATORS(dominance);
      BitSet_ *mergeSets = Dominance_MERGESETS(dominance);
      BitSet_ *msSets = Memory_alloc(memory, blockCount*sizeof(BitSet_));
      Except_CHECK(Dominance_hasMergeSets(dominance));
      *OldLiveCheck__MSSETS(this) = msSets;
      for (index = 0; index < blockCount; index++) {
        BasicBlock block = blockMap[index];
        BitSet_Ctor(msSets + index, memory, blockCount);
        BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
          BasicBlock block_w = BasicBlockEdge_destBlock(edge);
          int32_t index_w = BasicBlock_index(block_w);
          BitSet_union(msSets + index, mergeSets + index_w);
          BitSet_insert(msSets + index, index_w);
        } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
      }
      *OldLiveCheck__DFSFLAGS(this) = NULL;
      *OldLiveCheck__DFNMAP(this) = NULL;
      *OldLiveCheck__RSETS(this) = NULL;
      *OldLiveCheck__HSETS(this) = NULL;
      *OldLiveCheck__TSETS(this) = NULL;
      for (index = 0; index < blockCount; index++) {
        SumMergeSize += BitSet_count(mergeSets + index);
        SumMsSize += BitSet_count(msSets + index);
      }
    } else {
      int countArrays = 3;
      BitSet backHeads = OldLiveCheck_BACKHEADS(this);
      int32_t *preDfn = Dominance_PREDFN(dominance);
      uint8_t *dfsFlags = Memory_alloc(memory, blockCount*sizeof(uint8_t));
      int32_t *dfnMap = Memory_alloc(memory, blockCount*sizeof(int32_t));
      BitSet_ *rSets = Memory_alloc(memory, countArrays*blockCount*sizeof(BitSet_));
      BitSet_ *hSets = rSets + blockCount;
      BitSet_ *tSets = hSets + blockCount;
      *OldLiveCheck__DFSFLAGS(this) = dfsFlags;
      *OldLiveCheck__DFNMAP(this) = dfnMap;
      *OldLiveCheck__RSETS(this) = rSets;
      *OldLiveCheck__HSETS(this) = hSets;
      *OldLiveCheck__TSETS(this) = tSets;
      // Compute information used for fast liveness checking.
      // PREDFN, LASTDFN, RSETS and TSETS will be used.
      for (index = 0; index < countArrays*blockCount; index++) {
        BitSet_Ctor(rSets + index, memory, blockCount);
      }
      // Compute dfnMap = preDfn^(-1)
      memset(dfnMap, 0, blockCount*sizeof(int32_t));
      for (index = 0; index < blockCount; index++) {
        dfnMap[preDfn[index]] = index;
      }
      // Compute backedge targets, Rv & Tv^
      BitSet_Ctor(backHeads, memory, blockCount);
      memset(dfsFlags, 0, blockCount*sizeof(uint8_t));
      OldLiveCheck_firstDFS(this, blockMap[0], backHeads);
      // Compute Tv
      memset(dfsFlags, 0, blockCount*sizeof(uint8_t));
      OldLiveCheck_secondDFS(this, blockMap[0]);
      for (index = 0; index < blockCount; index++) {
        BitSet_insert(tSets + index, preDfn[index]);
      }
      *OldLiveCheck__MSSETS(this) = NULL;
      //0&& PFA_TRACE && fprintf(PFA_TRACE, "*** Sum|T_q,a|=%d\n", totalCount);
      for (index = 0; index < blockCount; index++) {
        SumHqSize += BitSet_count(hSets + index);
        SumTqSize += BitSet_count(tSets + index);
        SumRtSize += BitSet_count(rSets + index);
      }
    }
    if (OldLiveCheck_MERGESETS<0) {
      int totalCount = 0;
      int32_t *preDfn = Dominance_PREDFN(dominance);
      // Remplacement of tSet(n) by Merge(n) U {n}
      BitSet_ *tSets = OldLiveCheck_TSETS(this);
      BitSet_ *mergeSets = Dominance_MERGESETS(dominance);
      // Intitialize tSets[q] as preDFN[Merge[q] U q]
      for (index = 0; index < blockCount; index++) {
        BitSet_insert(tSets + index, preDfn[index]);
        BitSet_FOREACH(mergeSets + index, w) {
          BitSet_insert(tSets + index, preDfn[w]);
        } BitSet_ENDEACH;
      }
      //0&& PFA_TRACE && fprintf(PFA_TRACE, "*** Sum|M_q,a|=%d\n", totalCount);
    }
  }
  if (OldLiveCheck_LIVENESS>0) {
    CodeRegion codeRegion = Dominance_CODEREGION(dominance);
    *OldLiveCheck__LIVEINSETS(this) = CodeRegion_liveInSets(codeRegion);
    *OldLiveCheck__LIVEOUTSETS(this) = CodeRegion_liveOutSets(codeRegion);
    *OldLiveCheck__PHIDEFSETS(this) = CodeRegion_phiDefSets(codeRegion);
    *OldLiveCheck__PHIUSESETS(this) = CodeRegion_phiUseSets(codeRegion);
    *OldLiveCheck__BLOCKCOUNT(this) = CodeRegion_blockCount(codeRegion);
  }
  return this;
}

void
OldLiveCheck_Dtor(OldLiveCheck this)
{
  if (OldLiveCheck_LIVENESS==0 || OldLiveCheck_LIVENESS==2) {
    Memory memory = OldLiveCheck_MEMORY(this);
    int32_t blockCount = OldLiveCheck_BLOCKCOUNT(this), index;
    if (OldLiveCheck_MERGESETS>0) {
      int countArrays = 2;
      for (index = 0; index < blockCount; index++) {
        BitSet_Dtor(OldLiveCheck_MSSETS(this) + index);
      }
      Memory_free(memory, OldLiveCheck_MSSETS(this));
    } else {
      int countArrays = 3;
      Memory_free(memory, OldLiveCheck_DFSFLAGS(this));
      Memory_free(memory, OldLiveCheck_DFNMAP(this));
      BitSet_Dtor(OldLiveCheck_BACKHEADS(this));
      for (index = 0; index < countArrays*blockCount; index++) {
        BitSet_Dtor(OldLiveCheck_RSETS(this) + index);
      }
      Memory_free(memory, OldLiveCheck_RSETS(this));
    }
  }
}

size_t
OldLiveCheck_Size(Memory memory, Dominance dominance, TemporaryTable temporaryTable)
{
  return sizeof(OldLiveCheck_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(OldLiveCheck_)\t%zu\n", sizeof(OldLiveCheck_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Make a OldLiveCheck object.
 */
OldLiveCheck
OldLiveCheck_make(Memory parent, Dominance dominance, TemporaryTable temporaryTable);
#endif//$XCC_h

OldLiveCheck
OldLiveCheck_make(Memory parent, Dominance dominance, TemporaryTable temporaryTable)
{
  Memory memory = Memory_new(parent, true);
  size_t size = OldLiveCheck_Size(memory, dominance, temporaryTable);
  OldLiveCheck this = Memory_alloc(memory, size);
  OldLiveCheck_Ctor(this, memory, dominance, temporaryTable);
  return this;
}

#ifdef $XCC_h
OldLiveCheck
OldLiveCheck_kill(OldLiveCheck this);
#endif//$XCC_h

OldLiveCheck
OldLiveCheck_kill(OldLiveCheck this)
{
  if (this != NULL) {
    Memory memory = OldLiveCheck_MEMORY(this);
#ifndef _NDTOR
    OldLiveCheck_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
void
OldLiveCheck_firstDFS(OldLiveCheck this, BasicBlock block, BitSet backHeads);
#endif//$XCC__h

void
OldLiveCheck_firstDFS(OldLiveCheck this, BasicBlock block, BitSet backHeads)
{
  // Perform Depth First Search post order traversal
  // and update RSETS & HSETS & BACKHEADS & BasicBlockEdge_ISBACK
  uint8_t *dfsFlags = OldLiveCheck_DFSFLAGS(this);
  int32_t *preDfn = OldLiveCheck_PREDFN(this);
  BitSet_ *rSets = OldLiveCheck_RSETS(this);
  BitSet_ *hSets = OldLiveCheck_HSETS(this);
  int32_t block_index = BasicBlock_index(block);
  dfsFlags[block_index] = 1;
  // Now visit all the other children (fall through is guaranteed to be the first one in the list)
  BitSet_insert(rSets + block_index, preDfn[block_index]);
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, successor) {
    BasicBlock succ = BasicBlockEdge_destBlock(successor);
    int32_t succ_index = BasicBlock_index(succ);
    if (dfsFlags[succ_index] == 0) {
      OldLiveCheck_firstDFS(this, succ, backHeads);
    }
    Except_CHECK(dfsFlags[succ_index] != 0);
    if (dfsFlags[succ_index] == 2) {
      // processed: tree-edge, cross-edge or forward-edge
      BitSet_union(rSets + block_index, rSets + succ_index); // propagates rSets of successors
      BitSet_union(hSets + block_index, hSets + succ_index); // propagates hSets
    }
    if (dfsFlags[succ_index] == 1 && succ_index != 0) {
      // currently processing it: succ is an ancestor
      // This is a back edge to a block different than r
      BitSet_insert(hSets + block_index, preDfn[succ_index]);
      *BasicBlockEdge__ISBACK(successor) = true;
      BitSet_insert(backHeads, succ_index);
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  BitSet_diff(hSets + block_index, rSets + block_index); // remove Rt from Tt^
  dfsFlags[block_index] = 2;
}

#ifdef $XCC__h
void
OldLiveCheck_secondDFS(OldLiveCheck this, BasicBlock block);
#endif//$XCC__h

void
OldLiveCheck_secondDFS(OldLiveCheck this, BasicBlock block)
{
  // using a preorder traversal, propagate Tv on back edge targets
  // using a postorder traversal, propagate Tv\{v} on other nodes
  // We could have simply propagated Tv on all nodes using a preorder traversal
  //    using equation 1. But we suppose that Tv^ is ususally larger than succ(v)
  // /!\ There remains to add {v} to each Tv for non back-edge targets
  BitSet backHeads = OldLiveCheck_BACKHEADS(this);
  uint8_t *dfsFlags = OldLiveCheck_DFSFLAGS(this);
  BitSet_ *rSets = OldLiveCheck_RSETS(this);
  BitSet_ *tSets = OldLiveCheck_TSETS(this);
  BitSet_ *hSets = OldLiveCheck_HSETS(this);
  int32_t *preDfn = OldLiveCheck_PREDFN(this);
  int32_t *dfnMap = OldLiveCheck_DFNMAP(this);
  int32_t block_index = BasicBlock_index(block);
  // DFS flag
  if (dfsFlags[block_index]) return;
  dfsFlags[block_index] = 1;
  // back-edge target. propagate along a DFS pre-order traversal.
  // using equation 1 of the paper:  Tv = {v} union Union_{w\in Tv^} Tw
  if (BitSet_contains(backHeads, block_index)) {
    int32_t v_index = block_index;
    BitSet_insert(tSets + v_index, preDfn[v_index]);
    BitSet_FOREACH(hSets + v_index, w) {
      Except_CHECK(!BitSet_contains(rSets + v_index, w));
      BitSet_union(tSets + v_index, tSets + dfnMap[w]);
    } BitSet_ENDEACH;
    dfsFlags[v_index] = 2;
  }
  // DFS traversal
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, successor) {
    BasicBlock succ = BasicBlockEdge_destBlock(successor);
    int32_t succ_index = BasicBlock_index(succ);
    if (succ_index == 0 /* rootblock */) continue;
    OldLiveCheck_secondDFS(this, succ);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // not a back-edge target. propagate along a DFS postorder traversal.
  // using equation: Tv \ {v}= Union_{w\in succ(v)} Tw \ Rv
  if (!BitSet_contains(backHeads, block_index)) {
    int32_t v_index = block_index;
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, outedge) {
      BasicBlock w = BasicBlockEdge_destBlock(outedge);
      int32_t w_index = BasicBlock_index(w);
      if (w_index == 0 /* rootblock */) continue;
      BitSet_union(tSets + block_index, tSets + w_index);
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    BitSet_diff(tSets + block_index, rSets + v_index);
    dfsFlags[block_index] = 2;
  }
  // /!\ we need to add {v} to each Tv for each non back-edge target
}

#ifdef $XCC__h
/**
 * 
 */
static inline BasicBlock
OldLiveCheck_getUseInBasicBlock(const OldLiveCheck this, const_OperationArgument oparg)
{
  Operation u = OperationArgument_OPERATION(oparg);
  BasicBlock block_u = Operation_basicBlock(u);
  if (Operator_isPhi(Operation_operator(u))) {
    // For a phi, block_u is the predecessor's block
    int32_t argIndex = OperationArgument_ARGINDEX(oparg);
    BasicBlockEdge edge = BasicBlock_getPhiEnterEdge(block_u, argIndex);
    block_u = BasicBlockEdge_origBlock(edge);
  }
  return block_u;
}
#endif//$XCC__h

#ifdef $XCC__h
/*
 * OldLiveCheck_getUseOutBasicBlock --
 */
static inline BasicBlock
OldLiveCheck_getUseOutBasicBlock(const OldLiveCheck this, const_OperationArgument oparg, unsigned mode, BasicBlock block_q)
{
  Operation u = OperationArgument_OPERATION(oparg);
  BasicBlock block_u = Operation_basicBlock(u);
  if (Operator_isPhi(Operation_operator(u))) {
    // For a phi, block_u is the predecessor's block
    int32_t argIndex = OperationArgument_ARGINDEX(oparg);
    BasicBlockEdge edge_i = BasicBlock_getPhiEnterEdge(block_u, argIndex);
    block_u = BasicBlockEdge_origBlock(edge_i);
    if (block_u == block_q && !Operation_isCopyMode(u)){
      // MPLEX mode: a use is liveout of the predecessor's block
      return NULL;
    }
  }
  return block_u;
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * 
 */
static inline BasicBlock
OldLiveCheck_getUseOutOrdering(const OldLiveCheck this, const_OperationArgument oparg, unsigned mode, int32_t *_ordering)
{
  Operation u = OperationArgument_OPERATION(oparg);
  int32_t ordering_u = Operation_ordering(u);
  BasicBlock block_u = Operation_basicBlock(u);
  if (Operator_isPhi(Operation_operator(u))) {
    // For a phi, block_u is the predecessor's block
    int32_t argIndex = OperationArgument_ARGINDEX(oparg);
    BasicBlockEdge edge = BasicBlock_getPhiEnterEdge(block_u, argIndex);
    block_u = BasicBlockEdge_origBlock(edge);
    if (Operation_isCopyMode(u)) {
      ordering_u = BasicBlock_lateOrdering(block_u);
    } else {
      ordering_u = BasicBlock_lastOrdering(block_u) + 1;
    }
  }
  *_ordering = ordering_u;
  return block_u;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
* OldLiveCheck_isSSALiveInBasicBlock --	Liveness check.
*/
#define OldLiveCheck_isSSALiveInBasicBlock_copy(this, block_q, a) \
  OldLiveCheck_isSSALiveInBasicBlock(this, block_q, a, LivenessMode_Copy)
#define OldLiveCheck_isSSALiveInBasicBlock_mplex(this, block_q, a) \
  OldLiveCheck_isSSALiveInBasicBlock(this, block_q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSSALiveInBasicBlock_mixed(this, block_q, a) \
  OldLiveCheck_isSSALiveInBasicBlock(this, block_q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSSALiveInBasicBlock(OldLiveCheck this, BasicBlock block_q, Variable a, int mode);
// Check for livein at block_q (the query) for a defined at block_d and used at u \in uses.
#endif//$XCC_h

bool
OldLiveCheck_isSSALiveInBasicBlock(OldLiveCheck this, BasicBlock block_q, Variable a, int mode)
// To activate trace during self-test do:
// change 0 && OldLiveCheck_TRACE... into 1 && OldLiveCheck_TRACE...
// run env  PFA_LiveCheck_TRACE=1 ./OldLiveCheck.tst
{
  bool ret = false;
  int32_t *preDfn = OldLiveCheck_PREDFN(this);
  int32_t *lastDfn = OldLiveCheck_LASTDFN(this);
  Operation d = OperationResult_OPERATION(Variable_PRODUCER(a));
  BasicBlock block_d = Operation_basicBlock(d);
  int32_t index_d = BasicBlock_index(block_d);
  int32_t index_q = BasicBlock_index(block_q);
  int32_t pre_dfn = preDfn[index_d];
  int32_t last_dfn = lastDfn[index_d];
  int32_t next_dfn = -1;
  // block_q == block_d, d is a phi, mode == LivenessMode_Mplex
  if (   block_q == block_d
      && Operator_isPhi(Operation_operator(d))
      && !Operation_isCopyMode(d)) {
    ret = true;
    goto end;
  }
  // block_q not strictly dominated by block_d
  if (preDfn[index_q] <= pre_dfn || last_dfn < preDfn[index_q]) {
    ret = false;
    goto end;
  }
  if (OldLiveCheck_MERGESETS>=1) {
    Dominance dominance = OldLiveCheck_DOMINANCE(this);
    BitSet_ *mergeSets = Dominance_MERGESETS(dominance);
    int32_t *idomIdx = Dominance_IDOMIDX(dominance);
    const BasicBlock *blockMap = Dominance_BLOCKMAP(dominance);
    BitSet mergeSet_q = mergeSets + index_q;
    Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
      BasicBlock block_t = OldLiveCheck_getUseInBasicBlock(this, oparg);
      int32_t index_t = BasicBlock_index(block_t);
      while (block_t != block_d) {
        if (index_t == index_q || BitSet_contains(mergeSet_q, index_t)) {
          ret = true;
          goto end;
        }
        index_t = idomIdx[index_t];
        block_t =  blockMap[index_t];
      }
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
#if 0
    Dominance dominance = OldLiveCheck_DOMINANCE(this);
    int32_t *idomIdx = Dominance_IDOMIDX(dominance);
    BitSet_ *mergeSets = Dominance_MERGESETS(dominance);
    int32_t *treeLevel = Dominance_TREELEVEL(dominance);
    const BasicBlock *blockMap = Dominance_BLOCKMAP(dominance);
    int32_t level_d = treeLevel[index_d];
    BitSet merge_q = mergeSets + index_q;
    Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
      BasicBlock block_u = OldLiveCheck_getUseInBasicBlock(this, oparg);
      BasicBlock block_t = block_u;
      int32_t index_t = BasicBlock_index(block_t);
      while (treeLevel[index_t] >= level_d && block_t != block_d) {
        if (block_t == block_q || BitSet_contains(merge_q, index_t)) {
          ret = true;
          goto end;
        }
        index_t = idomIdx[index_t];
        block_t =  blockMap[index_t];
      }
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
#endif
  } else {
    int32_t *dfnMap = OldLiveCheck_DFNMAP(this);
    BitSet_ *rSets = OldLiveCheck_RSETS(this);
    BitSet_ *tSets = OldLiveCheck_TSETS(this);
    BitSet tq_set = tSets + index_q; // BitSet of t from which reachability should be checked
    // Tqa traversal
    while (   (pre_dfn < last_dfn)
           && (next_dfn = BitSet_nextMember(tq_set, pre_dfn)) >= 0
           && (next_dfn <= last_dfn)) {
      int32_t index_t = dfnMap[next_dfn];
      pre_dfn = lastDfn[index_t];
      // traverse uses
      Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
        // find block_u
        BasicBlock block_u = OldLiveCheck_getUseInBasicBlock(this, oparg);
        if (BitSet_contains(rSets + index_t, preDfn[BasicBlock_index(block_u)])) {
          ret = true;
          goto end;
        }
      } Variable_CONSUMERS_ENDEACH_OperationArgument;
    }
  }
end:
  if (OldLiveCheck_LIVENESS>=2 && OldLiveCheck_MERGESETS<1) {
    Except_CHECK(ret == OldLiveCheck_isSetLiveInOrdering(this, block_q, BasicBlock_firstOrdering(block_q), a, mode));
  }
  return ret;
}

#ifdef $XCC_h
/*
 * OldLiveCheck_isSetLiveInBasicBlock -- Liveness check using live sets.
 */
#define OldLiveCheck_isSetLiveInBasicBlock_copy(this, block_q, a) \
  OldLiveCheck_isSetLiveInBasicBlock(this, block_q, a, LivenessMode_Copy)
#define OldLiveCheck_isSetLiveInBasicBlock_mplex(this, block_q, a) \
  OldLiveCheck_isSetLiveInBasicBlock(this, block_q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSetLiveInBasicBlock_mixed(this, block_q, a) \
  OldLiveCheck_isSetLiveInBasicBlock(this, block_q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSetLiveInBasicBlock(OldLiveCheck this, BasicBlock block_q, Variable a, int mode);
// Check for livein at basic block block_q; mode can be MPLEX or COPY (see above)
#endif//$XCC_h

bool
OldLiveCheck_isSetLiveInBasicBlock(OldLiveCheck this, BasicBlock block_q, Variable a, int mode)
{
  bool ret;
  int32_t index = BasicBlock_index(block_q);
  Temporary temporary = Variable_TEMPORARY(a);
  Except_CHECK(OldLiveCheck_LIVENESS>0);
  if (mode == LivenessMode_Mplex) {
    ret = TemporarySet_contains(OldLiveCheck_LIVEINSETS(this) + index, temporary)
         || TemporarySet_contains(OldLiveCheck_PHIDEFSETS(this) + index, temporary);
  } else {
    ret = TemporarySet_contains(OldLiveCheck_LIVEINSETS(this) + index, temporary);
  }
  Except_CHECK(ret == OldLiveCheck_isSetLiveInOrdering(this, block_q, BasicBlock_firstOrdering(block_q), a, mode));
  return ret;
}

#ifdef $XCC_h
/*
 * OldLiveCheck_isLiveInBasicBlock	--
 */
static inline bool
OldLiveCheck_isLiveInBasicBlock(OldLiveCheck this, BasicBlock block_q, Variable a, int mode)
{
  bool liveincheck = false;
  bool liveinset = false;
  if (OldLiveCheck_LIVENESS==0 || OldLiveCheck_LIVENESS==2)
    liveincheck = OldLiveCheck_isSSALiveInBasicBlock(this, block_q, a, mode);
  if (OldLiveCheck_LIVENESS>0)
    liveinset = OldLiveCheck_isSetLiveInBasicBlock(this, block_q , a, mode);
  Except_CHECK(OldLiveCheck_LIVENESS!=2 || (liveinset == liveincheck));
  return liveincheck || liveinset;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
* OldLiveCheck_isSSALiveOutBasicBlock --	Liveness check.
*/
#define OldLiveCheck_isSSALiveOutBasicBlock_copy(this, block_q, a) \
  OldLiveCheck_isSSALiveOutBasicBlock(this, block_q, a, LivenessMode_Copy)
#define OldLiveCheck_isSSALiveOutBasicBlock_mplex(this, block_q, a) \
  OldLiveCheck_isSSALiveOutBasicBlock(this, block_q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSSALiveOutBasicBlock_mixed(this, block_q, a) \
  OldLiveCheck_isSSALiveOutBasicBlock(this, block_q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSSALiveOutBasicBlock(OldLiveCheck this, BasicBlock block_q, Variable a, int mode);
// Check for liveout at block_q (the query) for a defined at block_d and used at u \in uses.
#endif//$XCC_h

bool
OldLiveCheck_isSSALiveOutBasicBlock(OldLiveCheck this, BasicBlock block_q, Variable a, int mode)
{
  bool ret = false;
  int32_t *preDfn = OldLiveCheck_PREDFN(this);
  int32_t *lastDfn = OldLiveCheck_LASTDFN(this);
  Operation d = OperationResult_OPERATION(Variable_PRODUCER(a));
  BasicBlock block_d = Operation_basicBlock(d);
  int32_t index_d = BasicBlock_index(block_d);
  int32_t index_q = BasicBlock_index(block_q);
  int32_t pre_dfn = preDfn[index_d]; // tests can be restricted to some dominance subtree which have indexes between pre_dfn and last
  int32_t last_dfn = lastDfn[index_d];
  int32_t next_dfn = -1;
  // block_q not dominated by block_d
  if (preDfn[index_q] < pre_dfn || last_dfn < preDfn[index_q]) {
    ret = false;
    goto end;
  }
  if (block_d == block_q) {
    // traverse uses
    Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
      Operation u = OperationArgument_OPERATION(oparg);
      BasicBlock block_u = NULL;
      if (Operator_isPhi(Operation_operator(u))) {
        if (!Operation_isCopyMode(u)) {
          // MPLEXmodel, used in a phi => liveout of block_d
          ret = true;
          goto end;
        }
        {
          // COPY mode, block_u is the predecessor block
          int32_t argIndex = OperationArgument_ARGINDEX(oparg);
          BasicBlockEdge edge_i = BasicBlock_getPhiEnterEdge(Operation_basicBlock(u), argIndex);
          block_u = BasicBlockEdge_origBlock(edge_i);
        }
      } else {
        block_u = Operation_basicBlock(u);
      }
      if (block_u != block_d) {
        ret = true;
        goto end;
      }
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
    ret = false;
    goto end;
  }
  if (OldLiveCheck_MERGESETS>=2) {
    BitSet_ *msSets = OldLiveCheck_MSSETS(this);
    Dominance dominance = OldLiveCheck_DOMINANCE(this);
    BitSet_ *dominators = Dominance_DOMINATORS(dominance);
    BitSet dominators_d = dominators + index_d;
    BitSet msSet = msSets + index_q;
    Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
      BasicBlock block_t = OldLiveCheck_getUseOutBasicBlock(this, oparg, mode, block_q);
      if (block_t == NULL) {
        // MPLEX mode: a use is liveout of the predecessor's block
        ret = true;
        goto end;
      } else {
        BitSet dominators_t = dominators + BasicBlock_index(block_t);
        if (BitSet_diffOverlaps(msSet, dominators_d, dominators_t)) {
          ret = true;
          goto end;
        }
      }
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
  } else if (OldLiveCheck_MERGESETS>=1) {
    BitSet_ *msSets = OldLiveCheck_MSSETS(this);
    Dominance dominance = OldLiveCheck_DOMINANCE(this);
    int32_t *idomIdx = Dominance_IDOMIDX(dominance);
    const BasicBlock *blockMap = Dominance_BLOCKMAP(dominance);
    BitSet msSet = msSets + index_q;
    Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
      BasicBlock block_t = OldLiveCheck_getUseOutBasicBlock(this, oparg, mode, block_q);
      if (block_t == NULL) {
        // MPLEX mode: a use is liveout of the predecessor's block
        ret = true;
        goto end;
      } else {
        int32_t index_t = BasicBlock_index(block_t);
        while (block_t != block_d) {
          if (BitSet_contains(msSet, index_t)) {
            ret = true;
            goto end;
          }
          index_t = idomIdx[index_t];
          block_t =  blockMap[index_t];
        }
      }
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
  } else {
    int32_t *dfnMap = OldLiveCheck_DFNMAP(this);
    BitSet_ *rSets = OldLiveCheck_RSETS(this);
    BitSet_ *tSets = OldLiveCheck_TSETS(this);
    BitSet backHeads = OldLiveCheck_BACKHEADS(this);
    BitSet tq_set = tSets + index_q; // BitSet of t from which reachability should be checked
    bool isNotBackEdge = !BitSet_contains(backHeads, index_q);
    // Tqa traversal
    while (   (pre_dfn < last_dfn)
           && (next_dfn = BitSet_nextMember(tq_set, pre_dfn)) >= 0
           && (next_dfn <= last_dfn)) {
      int32_t index_t = dfnMap[next_dfn];
      pre_dfn = lastDfn[index_t];
      // traverse uses
      Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
        // find u
        BasicBlock block_u = OldLiveCheck_getUseOutBasicBlock(this, oparg, mode, block_q);
        if (block_u == NULL) {
          // MPLEX mode: a use is liveout of the predecessor's block
          ret = true;
          goto end;
        }
        if (index_t == index_q && isNotBackEdge && block_u == block_q) continue;
        if (BitSet_contains(rSets + index_t, preDfn[BasicBlock_index(block_u)])) {
          ret = true;
          goto end;
        }
      } Variable_CONSUMERS_ENDEACH_OperationArgument;
    }
  }
end:
  if (OldLiveCheck_LIVENESS>=2 && OldLiveCheck_MERGESETS<1) {
    Except_CHECK(ret == OldLiveCheck_isSetLiveOutOrdering(this, block_q, BasicBlock_lastOrdering(block_q), a, mode));
  }
  return ret;
}

#ifdef $XCC_h
/*
 * OldLiveCheck_isSetLiveOutBasicBlock -- OldLiveCheck check using livesets.
 */
#define OldLiveCheck_isSetLiveOutBasicBlock_copy(this, block_q, a) \
  OldLiveCheck_isSetLiveOutBasicBlock(this, block_q, a, LivenessMode_Copy)
#define OldLiveCheck_isSetLiveOutBasicBlock_mplex(this, block_q, a) \
  OldLiveCheck_isSetLiveOutBasicBlock(this, block_q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSetLiveOutBasicBlock_mixed(this, block_q, a) \
  OldLiveCheck_isSetLiveOutBasicBlock(this, block_q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSetLiveOutBasicBlock(OldLiveCheck this, BasicBlock block_q, Variable a, int mode);
// Check for liveout at block_q, mode can be MPLEX or COPY (see above)
#endif//$XCC_h

bool
OldLiveCheck_isSetLiveOutBasicBlock(OldLiveCheck this, BasicBlock block_q, Variable a, int mode)
{
  bool ret;
  int32_t index = BasicBlock_index(block_q);
  Temporary temporary = Variable_TEMPORARY(a);
  Except_CHECK(OldLiveCheck_LIVENESS>0);
  if (mode == LivenessMode_Mplex) {
    ret = TemporarySet_contains(OldLiveCheck_LIVEOUTSETS(this) + index, temporary)
         || TemporarySet_contains(OldLiveCheck_PHIUSESETS(this) + index, temporary);
  } else {
    ret = TemporarySet_contains(OldLiveCheck_LIVEOUTSETS(this) + index, temporary);
  }
  Except_CHECK(ret == OldLiveCheck_isSetLiveOutOrdering(this, block_q, BasicBlock_lastOrdering(block_q), a, mode));
  return ret;
}

#ifdef $XCC_h
/*
 * OldLiveCheck_isLiveOutBasicBlock	--
 */
static inline bool
OldLiveCheck_isLiveOutBasicBlock(OldLiveCheck this, BasicBlock block_q, Variable a, int mode)
{
  bool liveoutcheck = false;
  bool liveoutset = false;
  if (OldLiveCheck_LIVENESS==0 || OldLiveCheck_LIVENESS==2)
    liveoutcheck = OldLiveCheck_isSSALiveOutBasicBlock(this, block_q, a, mode);
  if (OldLiveCheck_LIVENESS>0)
    liveoutset = OldLiveCheck_isSetLiveOutBasicBlock(this, block_q , a, mode);
  Except_CHECK(OldLiveCheck_LIVENESS!=2 || (liveoutset == liveoutcheck));
  return liveoutcheck || liveoutset;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
* OldLiveCheck_isSSALiveInOperation --	Liveness check.
*/
#define OldLiveCheck_isSSALiveInOperation_copy(this, q, a) \
  OldLiveCheck_isSSALiveInOperation(this, q, a, LivenessMode_Copy)
#define OldLiveCheck_isSSALiveInOperation_mplex(this, q, a) \
  OldLiveCheck_isSSALiveInOperation(this, q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSSALiveInOperation_mixed(this, q, a) \
  OldLiveCheck_isSSALiveInOperation(this, q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSSALiveInOperation(OldLiveCheck this, Operation q, Variable a, int mode);
// Check for livein at operation q (the query) for a defined at d and used at u \in uses.
// /!\ If q is a phi function then livein(q) is undefined.
#endif//$XCC_h

bool
OldLiveCheck_isSSALiveInOperation(OldLiveCheck this, Operation q, Variable a, int mode)
// To activate trace during self-test do:
// change 0 && OldLiveCheck_TRACE... into 1 && OldLiveCheck_TRACE...
// run env  PFA_LiveCheck_TRACE=1 ./OldLiveCheck.tst
{
  BasicBlock block_q = Operation_basicBlock(q);
  int32_t ordering_q = Operation_ordering(q);
  Operation d = OperationResult_OPERATION(Variable_PRODUCER(a));
  int32_t ordering_d = Operation_ordering(d);
  Except_CHECK(ordering_d != -1 && ordering_q != -1);
  Except_CHECK(!Operator_isPhi(Operation_operator(q)));
  return OldLiveCheck_isSSALiveInOrdering(this, block_q, ordering_q, a, mode);
}

#ifdef $XCC_h
/*
 * OldLiveCheck_isSetLiveInOperation -- OldLiveCheck check using livesets.
 */
#define OldLiveCheck_isSetLiveInOperation_copy(this, q, a) \
  OldLiveCheck_isSetLiveInOperation(this, q, a, LivenessMode_Copy)
#define OldLiveCheck_isSetLiveInOperation_mplex(this, q, a) \
  OldLiveCheck_isSetLiveInOperation(this, q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSetLiveInOperation_mixed(this, q, a) \
  OldLiveCheck_isSetLiveInOperation(this, q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSetLiveInOperation(OldLiveCheck this, Operation q, Variable a, int mode);
// Check for livein at operation q (the query) for a defined at d and used at u \in uses
// /!\ If q is a phi function then livein(q) is undefined
// def-use chain and Operation_ORDERING must be up to date!
#endif//$XCC_h

bool
OldLiveCheck_isSetLiveInOperation(OldLiveCheck this, Operation q, Variable a, int mode)
{
  int32_t ordering_q = Operation_ordering(q);
  BasicBlock block_q = Operation_basicBlock(q);
  return OldLiveCheck_isSetLiveInOrdering(this, block_q, ordering_q, a, mode);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
* OldLiveCheck_isSSALiveOutOperation --	Liveness check.
*/
#define OldLiveCheck_isSSALiveOutOperation_copy(this, q, a) \
  OldLiveCheck_isSSALiveOutOperation(this, q, a, LivenessMode_Copy)
#define OldLiveCheck_isSSALiveOutOperation_mplex(this, q, a) \
  OldLiveCheck_isSSALiveOutOperation(this, q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSSALiveOutOperation_mixed(this, q, a) \
  OldLiveCheck_isSSALiveOutOperation(this, q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSSALiveOutOperation(OldLiveCheck this, Operation q, Variable a, int mode);
// Check for liveout at operation q (the query) for a defined at defq and used at u \in uses.
// The mode makes no difference. Indeed, for the copy mode, the a use in a phi is not
//   liveout of the corresponding predecessor basicBlock but liveout of its last instruction.
#endif//$XCC_h

bool
OldLiveCheck_isSSALiveOutOperation(OldLiveCheck this, Operation q, Variable a, int mode)
// To activate trace during self-test do:
// change 0 && OldLiveCheck_TRACE... into 1 && OldLiveCheck_TRACE...
// run env  PFA_LiveCheck_TRACE=1 ./OldLiveCheck.tst
{
  int32_t ordering_q = Operation_ordering(q);
  BasicBlock block_q = Operation_basicBlock(q);
  Operation d = OperationResult_OPERATION(Variable_PRODUCER(a));
  int32_t ordering_d = Operation_ordering(d);
  Except_CHECK(ordering_d != -1 && ordering_q != -1);
  return OldLiveCheck_isSSALiveOutOrdering(this, block_q, ordering_q, a, mode);
}

#ifdef $XCC_h
/*
 * OldLiveCheck_isSetLiveOutOperation -- OldLiveCheck check using livesets.
 */
#define OldLiveCheck_isSetLiveOutOperation_copy(this, q, a) \
  OldLiveCheck_isSetLiveOutOperation(this, q, a, LivenessMode_Copy)
#define OldLiveCheck_isSetLiveOutOperation_mplex(this, q, a) \
  OldLiveCheck_isSetLiveOutOperation(this, q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSetLiveOutOperation_mixed(this, q, a) \
  OldLiveCheck_isSetLiveOutOperation(this, q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSetLiveOutOperation(OldLiveCheck this, Operation q, Variable a, int mode);
// Check for liveout at operation q (the query) for a defined at d and used at u \in uses
// def-use chain and Operation_ORDERING must be up to date!
#endif//$XCC_h

bool
OldLiveCheck_isSetLiveOutOperation(OldLiveCheck this, Operation q, Variable a, int mode)
{
  BasicBlock block_q = Operation_basicBlock(q);
  int32_t ordering_q = Operation_ordering(q);
  return OldLiveCheck_isSetLiveOutOrdering(this, block_q, ordering_q, a, mode);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
* OldLiveCheck_isSSALiveInOrdering --        Liveness check.
*/
#define OldLiveCheck_isSSALiveInOrdering_copy(this, block_q, ordering_q, a) \
  OldLiveCheck_isSSALiveInOrdering(this, block_q, ordering_q, a, LivenessMode_Copy)
#define OldLiveCheck_isSSALiveInOrdering_mplex(this, block_q, ordering_q, a) \
  OldLiveCheck_isSSALiveInOrdering(this, block_q, ordering_q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSSALiveInOrdering_mixed(this, block_q, ordering_q, a) \
  OldLiveCheck_isSSALiveInOrdering(this, block_q, ordering_q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSSALiveInOrdering(OldLiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable a, int mode);
// Check for livein at operation q (the query) for a defined at d and used at u \in uses
// we must have min_ordering(block_q) <= ordering_q <= max_ordering(block_q)
// if ordering_q = min_ordering(block_q) provides the liveincheck for basicBlock block_q.
// the semantic for the case ordering_q > max_ordering(block_q) is undefined for livein check.
#endif//$XCC_h

bool
OldLiveCheck_isSSALiveInOrdering(OldLiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable a, int mode)
// To activate trace during self-test do:
// change 0 && OldLiveCheck_TRACE... into 1 && OldLiveCheck_TRACE...
// run env  PFA_LiveCheck_TRACE=1 ./OldLiveCheck.tst
{
  bool ret = false;
  int32_t *preDfn = OldLiveCheck_PREDFN(this);
  int32_t *lastDfn = OldLiveCheck_LASTDFN(this);
  Operation d = OperationResult_OPERATION(Variable_PRODUCER(a));
  BasicBlock block_d = Operation_basicBlock(d);
  int32_t index_d = BasicBlock_index(block_d);
  int32_t index_q = BasicBlock_index(block_q);
  int32_t pre_dfn = preDfn[index_d];
  int32_t last_dfn = lastDfn[index_d];
  int32_t next_dfn = -1;
  int32_t ordering_d = -1;
  if (Operator_isPhi(Operation_operator(d))) {
      if (Operation_isCopyMode(d))
        ordering_d = BasicBlock_earlyOrdering(block_d);
      else ordering_d = BasicBlock_firstOrdering(block_d) - 1;
  } else ordering_d = Operation_ordering(d);
  Except_CHECK(ordering_q != -1 && ordering_d != -1);
  Except_CHECK(BasicBlock_firstOrdering(block_q) <= ordering_q);
  Except_CHECK(ordering_q <= BasicBlock_lastOrdering(block_q));
  // q not strictly dominated by d
  if (   preDfn[index_q] < pre_dfn
      || last_dfn < preDfn[index_q]
      || (block_q == block_d && ordering_q <= ordering_d)) {
    ret = false;
    goto end;
  }
  if (block_d == block_q) {
    // traverse uses
    Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
      Operation u = OperationArgument_OPERATION(oparg);
      BasicBlock block_u = NULL;
      int32_t ordering_u = -1;
      if (Operator_isPhi(Operation_operator(u))) {
        if (!Operation_isCopyMode(u)) {
          ret = true;
          goto end;
        } else {
          int32_t argIndex = OperationArgument_ARGINDEX(oparg);
          BasicBlockEdge edge_i = BasicBlock_getPhiEnterEdge(Operation_basicBlock(u), argIndex);
          block_u = BasicBlockEdge_origBlock(edge_i);
          ordering_u = BasicBlock_lateOrdering(block_u);
        }
      } else {
        block_u = Operation_basicBlock(u);
        ordering_u = Operation_ordering(u);
      }
      Except_CHECK(ordering_u != -1);
      if (block_u != block_d || ordering_q <= ordering_u) {
        ret = true;
        goto end;
      }
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
    ret = false;
    goto end;
  }
  if (true) {
    int32_t *dfnMap = OldLiveCheck_DFNMAP(this);
    BitSet_ *rSets = OldLiveCheck_RSETS(this);
    BitSet_ *tSets = OldLiveCheck_TSETS(this);
    BitSet backHeads = OldLiveCheck_BACKHEADS(this);
    BitSet tq_set = tSets + index_q; // BitSet of t from which reachability should be checked
    bool isNotBackEdge = !BitSet_contains(backHeads, index_q);
    // Tqa traversal
    while (   (pre_dfn < last_dfn)
           && (next_dfn = BitSet_nextMember(tq_set, pre_dfn)) >= 0
           && (next_dfn <= last_dfn)) {
      int32_t index_t = dfnMap[next_dfn];
      pre_dfn = lastDfn[index_t];
      // traverse uses
      Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
        // find u
        int32_t ordering_u = -1;
        BasicBlock block_u = OldLiveCheck_getUseOutOrdering(this, oparg, mode, &ordering_u);
        if (index_t == index_q && isNotBackEdge && block_u == block_q && ordering_u < ordering_q) continue;
        if (BitSet_contains(rSets + index_t, preDfn[BasicBlock_index(block_u)])) {
          ret = true;
          goto end;
        }
      } Variable_CONSUMERS_ENDEACH_OperationArgument;
    }
  } else {
    Except_ALWAYS(false);
  }
end:
  return ret;
}

#ifdef $XCC_h
/*
 * OldLiveCheck_isSetLiveInOrdering -- OldLiveCheck check using livesets.
 */
#define OldLiveCheck_isSetLiveInOrdering_copy(this, block_q, ordering_q, a) \
  OldLiveCheck_isSetLiveInOrdering(this, block_q, ordering_q, a, LivenessMode_Copy)
#define OldLiveCheck_isSetLiveInOrdering_mplex(this, block_q, ordering_q, a) \
  OldLiveCheck_isSetLiveInOrdering(this, block_q, ordering_q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSetLiveInOrdering_mixed(this, block_q, ordering_q, a) \
  OldLiveCheck_isSetLiveInOrdering(this, block_q, ordering_q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSetLiveInOrdering(OldLiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable a, int mode);
// Check for livein at operation q (the query) for a defined at d and used at u \in uses
// /!\ If q is a phi function then livein(q) is undefined
// def-use chain and Operation_ORDERING must be updated! 
#endif//$XCC_h

bool
OldLiveCheck_isSetLiveInOrdering(OldLiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable a, int mode)
{
  Temporary temporary = Variable_TEMPORARY(a);
  int32_t index = BasicBlock_index(block_q);
  bool isbblivein, isbbliveout;
  bool ret = true;
  Except_CHECK(OldLiveCheck_LIVENESS>0);
  if (mode == LivenessMode_Mplex) {
    isbblivein = TemporarySet_contains(OldLiveCheck_LIVEINSETS(this) + index, temporary)
               || TemporarySet_contains(OldLiveCheck_PHIDEFSETS(this) + index, temporary);
    isbbliveout = TemporarySet_contains(OldLiveCheck_LIVEOUTSETS(this) + index, temporary)
                || TemporarySet_contains(OldLiveCheck_PHIUSESETS(this) + index, temporary);
  } else {
    isbblivein = TemporarySet_contains(OldLiveCheck_LIVEINSETS(this) + index, temporary);
    isbbliveout = TemporarySet_contains(OldLiveCheck_LIVEOUTSETS(this) + index, temporary);
  }
  Except_CHECK(BasicBlock_firstOrdering(block_q) <= ordering_q);
  Except_CHECK(ordering_q <= BasicBlock_lastOrdering(block_q));
  if (!isbblivein) {
    // No (strict) reaching definition
    Operation d = OperationResult_OPERATION(Variable_PRODUCER(a));
    BasicBlock block_d = Operation_basicBlock(d);
    int32_t ordering_d = -1;
    if (   block_d != block_q
        || ordering_q == BasicBlock_firstOrdering(block_q)) {
      ret = false;
      goto end;
    }
    if (Operator_isPhi(Operation_operator(d))) {
      ordering_d = BasicBlock_earlyOrdering(block_d);
    } else ordering_d = Operation_ordering(d);
    if (ordering_d >= ordering_q) {
      ret = false;
      goto end;
    }
  }
  if (!isbbliveout) {
    // Not upward exposed use
    bool isusedfurther = false;
    // Search for a use u in uses(a) and in block(q)
    Except_CHECK(Variable_isConsuming(a));
    Variable_CONSUMERS_FOREACH_OperationArgument(a, argument) {
      int32_t ordering_u = -1;
      Operation u = OperationArgument_OPERATION(argument);
      BasicBlock block_u = Operation_basicBlock(u);
      if (Operator_isPhi(Operation_operator(u))) {
        int32_t argIndex = OperationArgument_ARGINDEX(argument);
        BasicBlockEdge edge_i = BasicBlock_getPhiEnterEdge(block_u, argIndex);
        block_u = BasicBlockEdge_origBlock(edge_i);
        ordering_u = BasicBlock_lateOrdering(block_u);
      } else {
        ordering_u = Operation_ordering(u);
      }
      if (block_u == block_q && ordering_q <= ordering_u) {
        isusedfurther = true;
        break;
      }
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
    if (!isusedfurther) {
      ret = false;
      goto end;
    }
  }
  // Strict reaching def and updward exposed use
end:
  return ret;
}

#ifdef $XCC_h
/*
 * OldLiveCheck_isLiveInOrdering	--
 */
static inline bool
OldLiveCheck_isLiveInOrdering(OldLiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable a, int mode)
{
  bool liveincheck = false;
  bool liveinset = false;
  if (OldLiveCheck_LIVENESS==0 || OldLiveCheck_LIVENESS==2)
    liveincheck = OldLiveCheck_isSSALiveInOrdering(this, block_q, ordering_q, a, mode);
  if (OldLiveCheck_LIVENESS>0)
    liveinset = OldLiveCheck_isSetLiveInOrdering(this, block_q , ordering_q, a, mode);
  Except_CHECK(OldLiveCheck_LIVENESS!=2 || (liveinset == liveincheck));
  return liveincheck || liveinset;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
* OldLiveCheck_isSSALiveOutOrdering --        Liveness check.
*/
#define OldLiveCheck_isSSALiveOutOrdering_copy(this, block_q, ordering_q, a) \
  OldLiveCheck_isSSALiveOutOrdering(this, block_q, ordering_q, a, LivenessMode_Copy)
#define OldLiveCheck_isSSALiveOutOrdering_mplex(this, block_q, ordering_q, a) \
  OldLiveCheck_isSSALiveOutOrdering(this, block_q, ordering_q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSSALiveOutOrdering_mixed(this, block_q, ordering_q, a) \
  OldLiveCheck_isSSALiveOutOrdering(this, block_q, ordering_q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSSALiveOutOrdering(OldLiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable a, int mode);
// Check for liveout at operation q (the query) for a defined at d and used at u \in uses
// block_q = BasicBlock(q)
// ordering_q = ordering(a)
// We must have min_ordering(block_q) <= ordering_q <= max_ordering(block_q)
// if ordering_q == max_ordering(block_q) provides the liveout query for basicBlock block_q
#endif//$XCC_h

bool
OldLiveCheck_isSSALiveOutOrdering(OldLiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable a, int mode)
// To activate trace during self-test do:
// change 0 && OldLiveCheck_TRACE... into 1 && OldLiveCheck_TRACE...
// run env  PFA_LiveCheck_TRACE=1 ./OldLiveCheck.tst
{
  bool ret = false;
  int32_t *preDfn = OldLiveCheck_PREDFN(this);
  int32_t *lastDfn = OldLiveCheck_LASTDFN(this);
  Operation d = OperationResult_OPERATION(Variable_PRODUCER(a));
  BasicBlock block_d = Operation_basicBlock(d);
  int32_t index_d = BasicBlock_index(block_d);
  int32_t index_q = BasicBlock_index(block_q);
  int32_t pre_dfn = preDfn[index_d];
  int32_t last_dfn = lastDfn[index_d];
  int32_t next_dfn = -1;
  int32_t ordering_d = -1;
  if (Operator_isPhi(Operation_operator(d))) {
    if (Operation_isCopyMode(d))
      ordering_d = BasicBlock_earlyOrdering(block_d);
    else ordering_d = BasicBlock_firstOrdering(block_d) - 1;
  } else ordering_d = Operation_ordering(d);
  Except_CHECK(ordering_q != -1 && ordering_d != -1);
  Except_CHECK(BasicBlock_firstOrdering(block_q) <= ordering_q);
  Except_CHECK(ordering_q <= BasicBlock_lastOrdering(block_q)); // q not dominated by d
  if (   preDfn[index_q] < pre_dfn
      || last_dfn < preDfn[index_q]
      || (block_q == block_d && ordering_q < ordering_d)) {
    ret = false;
    goto end;
  }
  if (block_d == block_q) {
    // traverse uses
    Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
      Operation u = OperationArgument_OPERATION(oparg);
      BasicBlock block_u = NULL;
      int32_t ordering_u = -1;
      if (Operator_isPhi(Operation_operator(u))) {
        if (!Operation_isCopyMode(u)) {
          ret = true;
          goto end;
        } else {
          int32_t argIndex = OperationArgument_ARGINDEX(oparg);
          BasicBlockEdge edge_i = BasicBlock_getPhiEnterEdge(Operation_basicBlock(u), argIndex);
          block_u = BasicBlockEdge_origBlock(edge_i);
          ordering_u = BasicBlock_lateOrdering(block_u);
        }
      } else {
        block_u = Operation_basicBlock(u);
        ordering_u = Operation_ordering(u);
      }
      Except_CHECK(ordering_u != -1);
      if (block_u != block_d || ordering_q < ordering_u) {
        ret = true;
        goto end;
      }
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
    ret = false;
    goto end;
  }
  if (true) {
    int32_t *dfnMap = OldLiveCheck_DFNMAP(this);
    BitSet_ *rSets = OldLiveCheck_RSETS(this);
    BitSet_ *tSets = OldLiveCheck_TSETS(this);
    BitSet backHeads = OldLiveCheck_BACKHEADS(this);
    BitSet tq_set = tSets + index_q; // BitSet of t from which reachability should be checked
    bool isNotBackEdge = !BitSet_contains(backHeads, index_q);
    // Tqa traversal
    while (   (pre_dfn < last_dfn)
           && (next_dfn = BitSet_nextMember(tq_set, pre_dfn)) >= 0
           && (next_dfn <= last_dfn)) {
      int32_t index_t = dfnMap[next_dfn];
      pre_dfn = lastDfn[index_t];
      // traverse uses
      Variable_CONSUMERS_FOREACH_OperationArgument(a, oparg) {
        // find u
        int32_t ordering_u = -1;
        BasicBlock block_u = OldLiveCheck_getUseOutOrdering(this, oparg, mode, &ordering_u);
        if (index_t == index_q && isNotBackEdge && block_u == block_q && ordering_u <= ordering_q) continue;
        if (BitSet_contains(rSets + index_t, preDfn[BasicBlock_index(block_u)])) {
          ret = true;
          goto end;
        }
      } Variable_CONSUMERS_ENDEACH_OperationArgument;
    }
  }
end:
  return ret;
}

#ifdef $XCC_h
/*
 * OldLiveCheck_isSetLiveOutOrdering -- OldLiveCheck check using livesets.
 */
#define OldLiveCheck_isSetLiveOutOrdering_copy(this, block_q, ordering_q, a) \
  OldLiveCheck_isSetLiveOutOrdering(this, block_q, ordering_q, a, LivenessMode_Copy)
#define OldLiveCheck_isSetLiveOutOrdering_mplex(this, block_q, ordering_q, a) \
  OldLiveCheck_isSetLiveOutOrdering(this, block_q, ordering_q, a, LivenessMode_Mplex)
#define OldLiveCheck_isSetLiveOutOrdering_mixed(this, block_q, ordering_q, a) \
  OldLiveCheck_isSetLiveOutOrdering(this, block_q, ordering_q, a, LivenessMode_Mixed)
bool
OldLiveCheck_isSetLiveOutOrdering(OldLiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable a, int mode);
// Check for liveout at operation q (the query) for a defined at d and used at u \in uses
// def-use chain and Operation_ORDERING must be updated!
#endif//$XCC_h

bool
OldLiveCheck_isSetLiveOutOrdering(OldLiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable a, int mode)
{
  Temporary temporary = Variable_TEMPORARY(a);
  int32_t index = BasicBlock_index(block_q);
  bool isbblivein, isbbliveout;
  bool ret = true;
  Except_CHECK(OldLiveCheck_LIVENESS>0);
  if (mode == LivenessMode_Mplex) {
    isbblivein = TemporarySet_contains(OldLiveCheck_LIVEINSETS(this) + index, temporary)
               || TemporarySet_contains(OldLiveCheck_PHIDEFSETS(this) + index, temporary);
    isbbliveout = TemporarySet_contains(OldLiveCheck_LIVEOUTSETS(this) + index, temporary)
                || TemporarySet_contains(OldLiveCheck_PHIUSESETS(this) + index, temporary);
  } else {
    isbblivein = TemporarySet_contains(OldLiveCheck_LIVEINSETS(this) + index, temporary);
    isbbliveout = TemporarySet_contains(OldLiveCheck_LIVEOUTSETS(this) + index, temporary);
  }
  if (!isbblivein) {
    // No reaching definition
    Operation d = OperationResult_OPERATION(Variable_PRODUCER(a));
    BasicBlock block_d = Operation_basicBlock(d);
    int32_t ordering_d = -1;
    if (block_d != block_q) {
      ret = false;
      goto end;
    }
    if (Operator_isPhi(Operation_operator(d))) {
      ordering_d = BasicBlock_earlyOrdering(block_d);
    } else ordering_d = Operation_ordering(d);
    if (ordering_d > ordering_q) {
      ret = false;
      goto end;
    }
  }
  if (!isbbliveout) {
    // Not (strict) upward exposed use
    bool isusedfurther = false;
    // search for a use u (not a phi use) in use(a) and in block(q)
    Except_CHECK(Variable_isConsuming(a));
    Variable_CONSUMERS_FOREACH_OperationArgument(a, argument) {
      int32_t ordering_u = -1;
      Operation u = OperationArgument_OPERATION(argument);
      BasicBlock block_u = Operation_basicBlock(u);
      if (Operator_isPhi(Operation_operator(u))) {
        int32_t argIndex = OperationArgument_ARGINDEX(argument);
        BasicBlockEdge edge_i = BasicBlock_getPhiEnterEdge(block_u, argIndex);
        block_u = BasicBlockEdge_origBlock(edge_i);
        ordering_u = BasicBlock_lateOrdering(block_u);
      } else {
        ordering_u = Operation_ordering(u);
      }
      if (block_u == block_q && ordering_q < ordering_u) {
        isusedfurther = true;
        break;
      }
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
    if (!isusedfurther) {
      ret = false;
      goto end;
    }
  }
  // Reaching def and strict updward exposed use
end:
  return ret;
}

#ifdef $XCC_h
/*
 * OldLiveCheck_isLiveOutOrdering	--
 */
static inline bool
OldLiveCheck_isLiveOutOrdering(OldLiveCheck this, BasicBlock block_q, int32_t ordering_q, Variable a, int mode)
{
  bool liveoutcheck = false;
  bool liveoutset = false;
  if (OldLiveCheck_LIVENESS==0 || OldLiveCheck_LIVENESS==2)
    liveoutcheck = OldLiveCheck_isSSALiveOutOrdering(this, block_q, ordering_q, a, mode);
  if (OldLiveCheck_LIVENESS>0)
    liveoutset = OldLiveCheck_isSetLiveOutOrdering(this, block_q , ordering_q, a, mode);
  Except_CHECK(OldLiveCheck_LIVENESS!=2 || (liveoutset == liveoutcheck));
  return liveoutcheck || liveoutset;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static void
prettySet(OldLiveCheck this, BitSet bitSet, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
static void
prettySet(OldLiveCheck this, BitSet bitSet, FILE *file)
{
  int32_t *dfnMap = OldLiveCheck_DFNMAP(this);
  BitSet_FOREACH(bitSet, preDfn) {
    if (1) { // Dump preDfn index
      fprintf(file, "%d ", preDfn);
    } else { // Dump block_index
      fprintf(file, "%d ", dfnMap[preDfn]);
    }
  } BitSet_ENDEACH;
}
#endif//_NTRACE

#ifdef $XCC__h
void
OldLiveCheck_prettyBasicBlock(OldLiveCheck this, BasicBlock block, uint32_t dfsIndex, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
void
OldLiveCheck_prettyBasicBlock(OldLiveCheck this, BasicBlock block, uint32_t dfsIndex, FILE *file)
// Pretty print the dotty node of the basic block block.
// Provides:
// - its DFS number (/!\ this is not the label of the node)
// - its basic block index (this is the dotty label of the node)
// - The set of reduced reachable nodes Rq (rset)
// - The set of targets of reduced reachable back-edges Tq^ (hSets)
// - The set of reachable backedge targets not reduced reachable Tq (tset)
// If the BasicBlock is a backedge target, draw in red.
{
  uint32_t block_index = BasicBlock_index(block);
  int32_t block_preDfn = OldLiveCheck_PREDFN(this)[block_index];
  BitSet block_rset = OldLiveCheck_RSETS(this) + block_index;
  BitSet block_tset = OldLiveCheck_TSETS(this) + block_index;
  BitSet block_hSets = OldLiveCheck_HSETS(this) + block_index;
  fprintf(file, "%d ", block_index); // dotty label is block_index
  fprintf(file, "[label=\""); // what will be dumped in the box
  fprintf(file, "%d (dfs:%d) (idx:%d)\\n", block_preDfn, dfsIndex, BasicBlock_index(block)); // preDfn and dfsindex labels
  fprintf(file, "R="); prettySet(this, block_rset, file); fprintf(file, "\\l");
  fprintf(file, "T="); prettySet(this, block_tset, file); fprintf(file, "\\l");
  fprintf(file, "T^="); prettySet(this, block_hSets, file); fprintf(file, "\\l");
  fprintf(file, "\""); // End of content
  if (BitSet_contains(OldLiveCheck_BACKHEADS(this), block_index)) {
    fprintf(file, ",color=red");
  }
  fprintf(file, "]\n");
}
#endif//_NTRACE

#ifdef $XCC__h
void
OldLiveCheck_prettyEdge(OldLiveCheck this, BasicBlock source, BasicBlock dest, const char *attributes, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
void
OldLiveCheck_prettyEdge(OldLiveCheck this, BasicBlock source, BasicBlock dest, const char *attributes, FILE *file)
{
  uint32_t source_index = BasicBlock_index(source); // The dotty label of a node is its BasicBlock_index
  uint32_t dest_index = BasicBlock_index(dest);
  fprintf(file, "%d -> %d [%s]\n", source_index, dest_index, attributes);
}
#endif//_NTRACE

#ifdef $XCC__h
void
OldLiveCheck_DFS4pretty(OldLiveCheck this, BasicBlock block, uint32_t *_dfsIndex, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
void
OldLiveCheck_DFS4pretty(OldLiveCheck this, BasicBlock block, uint32_t *_dfsIndex, FILE *file)
// Here we perform the same DFS traversal as for the OldLiveCheck precomputation
// We print in dotty format edges, and nodes of the CFG.
// We differentiate tree-edges, cross-edges & forward, and back-edges.
{
  int32_t block_index = BasicBlock_index(block);
  uint8_t *dfsFlags = OldLiveCheck_DFSFLAGS(this);
  uint32_t dfsIndex = *_dfsIndex; // Actual DFS for node
  dfsFlags[block_index] = 1;
  (*_dfsIndex)++; // increments the DFS numbering of nodes.
  OldLiveCheck_prettyBasicBlock(this, block, dfsIndex, file);
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, successor) {
    BasicBlock succ = BasicBlockEdge_destBlock(successor);
    int32_t succ_index = BasicBlock_index(succ);
    if (dfsFlags[succ_index] == 0) {
      OldLiveCheck_DFS4pretty(this, succ, _dfsIndex, file);
      OldLiveCheck_prettyEdge(this, block, succ, "style=bold,color=red,weight=100", file);
      continue;
    }
    if (dfsFlags[succ_index] == 2) {
      // processed: cross-edge or forward-edge
      OldLiveCheck_prettyEdge(this, block, succ, "style=filled", file);
      continue;
    }
    if (dfsFlags[succ_index] == 1 && succ_index != 0) {
      // currently processing it: succ is an ancestor
      // This is a back edge to a block different than r
      OldLiveCheck_prettyEdge(this, succ, block, "style=dotted,dir=back", file);
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  dfsFlags[block_index] = 2; // mark processed
}
#endif//_NTRACE

#ifdef $XCC_h
/*
 * OldLiveCheck_prettyCFG -- PrettyPrint in dotty format of the CFG.
 */
void
OldLiveCheck_prettyCFG(OldLiveCheck this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
void
OldLiveCheck_prettyCFG(OldLiveCheck this, FILE *file)
{
  int32_t blockCount = OldLiveCheck_BLOCKCOUNT(this);
  BasicBlock rootBlock = OldLiveCheck_ROOTBLOCK(this);
  uint32_t dfsIndex = 0;
  // There are 3 indexing for basic blocks:
  //  - "PREDFN" corresponds to a DFS numbering of the dominance tree
  //  - "dfsIndex" corresponds to a DFS numbering of the CFG. It corresponds to the DFS traversal
  //    used in OldLiveCheck precomputation. There is no special reason why it should be equal to PREDFN.
  //  - "BasicBlock_index" corresponds to an indexing of basic blocks as nodes of the CFG. This
  //    indexing probably corresponds to the one given by Open64...?
  //  - For each BasicBlock, rset[block_index], tset[block_index] & hSets[block_index] are BitSet(s) indexed using
  //    PREDFN.
  fprintf(file, "digraph G {\n");
  memset(OldLiveCheck_DFSFLAGS(this), 0, blockCount*sizeof(uint8_t));
  OldLiveCheck_DFS4pretty(this, rootBlock, &dfsIndex, file);
  fprintf(file, "}\n");
}
#endif//_NTRACE

#ifdef $XCC_h
/*
 * OldLiveCheck_dumpCFG -- DumpCFG into a <functionname>.dotty file
 */
void
OldLiveCheck_dumpCFG(OldLiveCheck this);
#endif//$XCC_h

#ifndef _NTRACE
void
OldLiveCheck_dumpCFG(OldLiveCheck this)
{
  FILE *file;
  CodeRegion codeRegion = OldLiveCheck_CODEREGION(this);
  Procedure procedure = CodeRegion_procedure(codeRegion);
  const char *procname = Procedure_name(procedure);
  char *filename = (char *)Memory_alloc(Memory_Root, strlen(procname) + 4 + 1);
  strcpy(filename, procname); strcat(filename, ".dot");
  OldLiveCheck_TRACE && fprintf(OldLiveCheck_TRACE, "Output dotty file into %s\n", filename);
  file = fopen(filename, "w");
  Except_ALWAYS(file != NULL);
  OldLiveCheck_prettyCFG(this, file);
  fclose(file);
  Memory_free(Memory_Root, filename);
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////
//@XCC.h
//
// Verify correctness of OldLiveCheck with respect to the CodeRegion liveSets.
//
// Preconditions:
// - CodeRegion_globalTable() must be initialized to the set of variables to test.
// - Consumers must be setup (for instance by a call to SSAForm_makeConsumers()) for
// all variables in the global table.
// - Live sets in CodeRegion must be updated for all variables in the global table.

#ifdef $XCC__h
void
OldLiveCheck_check(OldLiveCheck this);
#endif//$XCC__h

#ifndef _NTRACE
void
OldLiveCheck_check(OldLiveCheck this)
{
  CodeRegion codeRegion = OldLiveCheck_CODEREGION(this);
  TemporaryTable globalTable = CodeRegion_globalTable(codeRegion);
  TemporarySet_ *liveInSets = OldLiveCheck_LIVEINSETS(this);
  TemporarySet_ *liveOutSets = OldLiveCheck_LIVEOUTSETS(this);
  TemporarySet_ *phiUseSets = OldLiveCheck_PHIUSESETS(this);
  TemporarySet_ *phiDefSets = OldLiveCheck_PHIDEFSETS(this);
  if (OldLiveCheck_LIVENESS != 2) return;
  if (PFA_TRACE) {
    CodeRegion_pretty(codeRegion, PFA_TRACE);
    fprintf(PFA_TRACE, "Variables and Consumers\n");
    TemporaryTable_FOREACH_Temporary(globalTable, temporary) {
      if (Temporary_isVariable(temporary)) {
        Temporary_pretty(temporary, PFA_TRACE);
        fprintf(PFA_TRACE, "\n producer: ");
        Operation_pretty(OperationResult_OPERATION(Variable_PRODUCER(Temporary_VARIABLE(temporary))), PFA_TRACE);
        fprintf(PFA_TRACE, " consumer(s): ");
        Variable_CONSUMERS_FOREACH_OperationArgument(Temporary_VARIABLE(temporary), oparg) {
          Operation op = OperationArgument_OPERATION(oparg);
          Operation_pretty(op, PFA_TRACE);
        } Variable_CONSUMERS_ENDEACH_OperationArgument;
      }
    } TemporaryTable_ENDEACH_Temporary;
    fprintf(PFA_TRACE, "\n");
  }
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    int32_t index = BasicBlock_index(basicBlock);
    PFA_TRACE && fprintf(PFA_TRACE, "BB: %d\nlive-in:", index)
              && TemporarySet_pretty(liveInSets + index,PFA_TRACE)
              && fprintf(PFA_TRACE, "\nphi-in:")
              && TemporarySet_pretty(phiDefSets + index,PFA_TRACE)
              && fprintf(PFA_TRACE, "\nlive-out:")
              && TemporarySet_pretty(liveOutSets + index,PFA_TRACE)
              && fprintf(PFA_TRACE, "\nphi-out:")
              && TemporarySet_pretty(phiUseSets + index,PFA_TRACE)
              && fprintf(PFA_TRACE, "\n");
    TemporaryTable_FOREACH_Temporary(globalTable, temporary) {
      if (Temporary_isVariable(temporary)) {
        Variable a = Temporary_VARIABLE(temporary);
        //0 && PFA_TRACE && fprintf(PFA_TRACE, "BB: %d:", index) && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
        {
          /* Livein and Liveout check on basic block boundaries */
          bool isLiveincheck_copy = OldLiveCheck_isSSALiveInBasicBlock_copy(this, basicBlock, a);
          bool isLiveoutcheck_copy = OldLiveCheck_isSSALiveOutBasicBlock_copy(this, basicBlock, a);
          bool isLiveincheck_mixed = OldLiveCheck_isSSALiveInBasicBlock_mixed(this, basicBlock, a);
          bool isLiveoutcheck_mixed = OldLiveCheck_isSSALiveOutBasicBlock_mixed(this, basicBlock, a);
          bool isLiveincheck_mplex = OldLiveCheck_isSSALiveInBasicBlock_mplex(this, basicBlock, a);
          bool isLiveoutcheck_mplex = OldLiveCheck_isSSALiveOutBasicBlock_mplex(this, basicBlock, a);
          bool isLiveinset_copy = OldLiveCheck_isSetLiveInBasicBlock_copy(this, basicBlock, a);
          bool isLiveoutset_copy = OldLiveCheck_isSetLiveOutBasicBlock_copy(this, basicBlock, a);
          bool isLiveinset_mixed = OldLiveCheck_isSetLiveInBasicBlock_mixed(this, basicBlock, a);
          bool isLiveoutset_mixed = OldLiveCheck_isSetLiveOutBasicBlock_mixed(this, basicBlock, a);
          bool isLiveinset_mplex = OldLiveCheck_isSetLiveInBasicBlock_mplex(this, basicBlock, a);
          bool isLiveoutset_mplex = OldLiveCheck_isSetLiveOutBasicBlock_mplex(this, basicBlock, a);
          bool correct = true;
          if (isLiveoutset_copy != isLiveoutcheck_copy) {
            fprintf(stderr, "Live out mismatch (liveset_copy: %d, liveCheck_copy: %d) in block %d, temporary: ", isLiveoutset_copy, isLiveoutcheck_copy, index);
            Temporary_pretty(temporary, stderr); fprintf(stderr, "\n");
            correct = false;
          }
          if (isLiveoutset_mixed != isLiveoutcheck_mixed) {
            fprintf(stderr, "Live out mismatch (liveset_mixed: %d, liveCheck_mixed: %d) in block %d, temporary: ", isLiveoutset_mixed, isLiveoutcheck_mixed, index);
            Temporary_pretty(temporary, stderr); fprintf(stderr, "\n");
            correct = false;
          }
          if (isLiveoutset_mplex != isLiveoutcheck_mplex) {
            fprintf(stderr, "Live out mismatch (liveset_mplex: %d, liveCheck_mplex: %d) in block %d, temporary: ", isLiveoutset_mplex, isLiveoutcheck_mplex, index);
            Temporary_pretty(temporary, stderr); fprintf(stderr, "\n");
            correct = false;
          }
          if (isLiveinset_copy != isLiveincheck_copy) {
            fprintf(stderr, "Live in mismatch (liveset_copy: %d, liveCheck_copy: %d) in block %d, temporary: ", isLiveinset_copy, isLiveincheck_copy, index);
            Temporary_pretty(temporary, stderr); fprintf(stderr, "\n");
            correct = false;
          }
          if (isLiveinset_mixed != isLiveincheck_mixed) {
            fprintf(stderr, "Live in mismatch (liveset_mixed: %d, liveCheck_mixed: %d) in block %d, temporary: ", isLiveinset_mixed, isLiveincheck_mixed, index);
            Temporary_pretty(temporary, stderr); fprintf(stderr, "\n");
            correct = false;
          }
          if (isLiveinset_mplex != isLiveincheck_mplex) {
            fprintf(stderr, "Live in mismatch (liveset_mplex: %d, liveCheck_mplex: %d) in block %d, temporary: ", isLiveinset_mplex, isLiveincheck_mplex, index);
            Temporary_pretty(temporary, stderr); fprintf(stderr, "\n");
            correct = false;
          }
          Except_CHECK(correct);
        }
        {
          /* Livein and Liveout check at operations boundaries */
          BasicBlock_FOREACH_Operation(basicBlock, operation) {
            {
              bool isLiveoutcheck_mplex = OldLiveCheck_isSSALiveOutOperation_mplex(this, operation, a);
              bool isLiveoutset_mplex = OldLiveCheck_isSetLiveOutOperation_mplex(this, operation, a);
              bool isLiveoutcheck_copy = OldLiveCheck_isSSALiveOutOperation_copy(this, operation, a);
              bool isLiveoutset_copy = OldLiveCheck_isSetLiveOutOperation_copy(this, operation, a);
              bool isLiveoutcheck_mixed = OldLiveCheck_isSSALiveOutOperation_mixed(this, operation, a);
              bool isLiveoutset_mixed = OldLiveCheck_isSetLiveOutOperation_mixed(this, operation, a);
              bool correct = true;
              if (isLiveoutcheck_mplex != isLiveoutset_mplex) {
                fprintf(stderr, "Live out mismatch (liveoutcheck_mplex: %d, liveoutset_mplex: %d) for Temporary: ", \
                        isLiveoutcheck_mplex, isLiveoutset_mplex);
                Temporary_pretty(temporary, stderr);
                fprintf(stderr, " in block %d, operation: [", index);
                Operation_pretty(operation, stderr);
                fprintf(stderr, "]\n");
                correct = false;
              }
              if (isLiveoutcheck_copy != isLiveoutset_copy) {
                fprintf(stderr, "Live out mismatch (liveoutcheck_copy: %d, liveoutset_copy: %d) for Temporary: ", \
                        isLiveoutcheck_copy, isLiveoutset_copy);
                Temporary_pretty(temporary, stderr);
                fprintf(stderr, " in block %d, operation: [", index);
                Operation_pretty(operation, stderr);
                fprintf(stderr, "]\n");
                correct = false;
              }
              if (isLiveoutcheck_mixed != isLiveoutset_mixed) {
                fprintf(stderr, "Live out mismatch (liveoutcheck_mixed: %d, liveoutset_mixed: %d) for Temporary: ", \
                        isLiveoutcheck_mixed, isLiveoutset_mixed);
                Temporary_pretty(temporary, stderr);
                fprintf(stderr, " in block %d, operation: [", index);
                Operation_pretty(operation, stderr);
                fprintf(stderr, "]\n");
                correct = false;
              }
              if (Operator_isPhi(Operation_operator(operation))) continue; // Livein for phi is undefined
              Except_CHECK(!Operator_isSigma(Operation_operator(operation))); // SSI is not supported
              {
                bool isLiveincheck_mplex = OldLiveCheck_isSSALiveInOperation_mplex(this, operation, a);
                bool isLiveinset_mplex = OldLiveCheck_isSetLiveInOperation_mplex(this, operation, a);
                bool isLiveincheck_copy = OldLiveCheck_isSSALiveInOperation_copy(this, operation, a);
                bool isLiveinset_copy = OldLiveCheck_isSetLiveInOperation_copy(this, operation, a);
                bool isLiveincheck_mixed = OldLiveCheck_isSSALiveInOperation_mixed(this, operation, a);
                bool isLiveinset_mixed = OldLiveCheck_isSetLiveInOperation_mixed(this, operation, a);
                bool correct = true;
                if (isLiveincheck_mplex != isLiveinset_mplex) {
                  fprintf(stderr, "Live in mismatch (liveincheck_mplex: %d, liveinset_mplex: %d) for Temporary: ", \
                          isLiveincheck_mplex, isLiveinset_mplex);
                  Temporary_pretty(temporary, stderr);
                  fprintf(stderr, " in block %d, operation: [", index);
                  Operation_pretty(operation, stderr);
                  fprintf(stderr, "]\n");
                  correct = false;
                }
                if (isLiveincheck_copy != isLiveinset_copy) {
                  fprintf(stderr, "Live in mismatch (liveincheck_copy: %d, liveinset_copy: %d) for Temporary: ", \
                          isLiveincheck_copy, isLiveinset_copy);
                  Temporary_pretty(temporary, stderr);
                  fprintf(stderr, " in block %d, operation: [", index);
                  Operation_pretty(operation, stderr);
                  fprintf(stderr, "]\n");
                  correct = false;
                }
                if (isLiveincheck_mixed != isLiveinset_mixed) {
                  fprintf(stderr, "Live in mismatch (liveincheck_mixed: %d, liveinset_mixed: %d) for Temporary: ", \
                          isLiveincheck_mixed, isLiveinset_mixed);
                  Temporary_pretty(temporary, stderr);
                  fprintf(stderr, " in block %d, operation: [", index);
                  Operation_pretty(operation, stderr);
                  fprintf(stderr, "]\n");
                  correct = false;
                }
                Except_CHECK(correct);
              }
            }
          } BasicBlock_ENDBACK_Operation;
        }
      } /*endif (Temporary_isVariable(temporary)) */
    } TemporaryTable_ENDEACH_Temporary;
  } CodeRegion_ENDEACH_BasicBlock;
}
#else//_NTRACE
void
OldLiveCheck_check(OldLiveCheck this) { }
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // Run OldLiveCheck_check() on a simple if hammock.
  // Figure 1 in Sreedhar's paper.
  Program program = Program_new(Memory_Root, (Processor)1, (Convention)1);
  SymbolTable symbolTable = Program_symbolTable(program);
  Symbol symbol = SymbolTable_lookup(symbolTable, 1, "sreedhar-figure-1");
  Procedure procedure = Program_makeProcedure(program, symbol);
  LabelTable labelTable = Program_labelTable(program);
  Label L1 = LabelTable_lookup(labelTable, 1, "L1");
  Label L2 = LabelTable_lookup(labelTable, 2, "L2");
  Label L3 = LabelTable_lookup(labelTable, 3, "L3");
  BasicBlock block_l1 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l2 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l3 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock_addLabel(block_l1, L1);
  BasicBlock_addLabel(block_l2, L2);
  BasicBlock_addLabel(block_l3, L3);
  BasicBlock_makeEdge(block_l1, block_l2, 0.0);
  BasicBlock_makeEdge(block_l1, block_l3, 0.0);
  BasicBlock_makeEdge(block_l2, block_l3, 0.0);
  Procedure_buildCodeRegions(procedure);
  printf("\nFigure 1 in Sreedhar's paper:\n");
  {
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Temporary temporary_null = Procedure_makeAssignedTemporary(procedure, Register__UNDEF);
    Temporary temporary_x = Procedure_makeVirtualTemporary(procedure, 1);
    Temporary temporary_y = Procedure_makeVirtualTemporary(procedure, 1);
    Temporary temporary_a1 = Procedure_makeAbsoluteTemporary(procedure, 1, Immediate__UNDEF);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l1),
                               Operator_APPLY, temporary_x, temporary_a1);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l1),
                               Operator_APPLY, temporary_y, temporary_a1);
    Selector_makeOperationCOPY(BasicBlock_selector(block_l2), temporary_x, temporary_y);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l3),
                               Operator_APPLY, temporary_null, temporary_x);
    {
      OldLiveCheck liveCheck;
      Coalescer_ coalescer[1];
      SSAForm ssaForm = SSAForm_construct(Memory_Root, codeRegion, true);
      Dominance dominance = SSAForm_DOMINANCE(ssaForm);
      TemporaryTable_empty(CodeRegion_globalTable(codeRegion));
      SSAForm_fillTemporaryTable(ssaForm, CodeRegion_globalTable(codeRegion));
      CodeRegion_makeConsumers(codeRegion, CodeRegion_globalTable(codeRegion));
      SSAForm_makeOrderings(ssaForm, 4);
#if !_OPTIMIZE
      OldLiveCheck_LIVENESS = 2;
#endif//_OPTIMIZE
      Coalescer_Ctor(coalescer, Memory_Root, codeRegion,
                     CodeRegion_globalTable(codeRegion), dominance, OptimizeCoalescing_Sreedhar);
      Coalescer_complete(coalescer, false);
      liveCheck = (OldLiveCheck)Coalescer_LIVECHECK(coalescer);
#ifndef _NTRACE
      if (OldLiveCheck_TRACE) OldLiveCheck_dumpCFG(liveCheck);
#endif
      //OldLiveCheck_check(liveCheck);
      Coalescer_Dtor(coalescer);
      SSAForm_destruct(ssaForm);
    }
  }
  Program_delete(program);
}
#endif//$XCC__c

#ifdef $XCC__c
{
  // Run Livecheck_check().
  // Figure 3 in Boissinot's CGO paper.
  Program program = Program_new(Memory_Root, (Processor)1, (Convention)1);
  Optimize optimize = Program_optimize(program);
  SymbolTable symbolTable = Program_symbolTable(program);
  Symbol symbol = SymbolTable_lookup(symbolTable, 1, "boissinot-figure-3");
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
  Label L9 = LabelTable_lookup(labelTable, 9, "L9");
  Label L10 = LabelTable_lookup(labelTable, 10, "L10");
  Label L11 = LabelTable_lookup(labelTable, 11, "L11");
  Label LEXIT = LabelTable_lookup(labelTable, 12, "Lexit");
  BasicBlock block_l1 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l2 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l3 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l4 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l5 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l6 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l7 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l8 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l9 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l10 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l11 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_lexit = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock_addLabel(block_l1, L1);
  BasicBlock_addLabel(block_l2, L2);
  BasicBlock_addLabel(block_l3, L3);
  BasicBlock_addLabel(block_l4, L4);
  BasicBlock_addLabel(block_l5, L5);
  BasicBlock_addLabel(block_l6, L6);
  BasicBlock_addLabel(block_l7, L7);
  BasicBlock_addLabel(block_l8, L8);
  BasicBlock_addLabel(block_l9, L9);
  BasicBlock_addLabel(block_l10, L10);
  BasicBlock_addLabel(block_l11, L11);
  BasicBlock_addLabel(block_lexit, LEXIT);
  BasicBlock_makeEdge(block_l1, block_l2, 0.0);
  BasicBlock_makeEdge(block_l2, block_l3, 0.0);
  BasicBlock_makeEdge(block_l3, block_l4, 0.0);
  BasicBlock_makeEdge(block_l4, block_l5, 0.0);
  BasicBlock_makeEdge(block_l5, block_l6, 0.0);
  BasicBlock_makeEdge(block_l6, block_l5, 0.0);
  BasicBlock_makeEdge(block_l6, block_l7, 0.0);
  BasicBlock_makeEdge(block_l7, block_l2, 0.0);
  BasicBlock_makeEdge(block_l7, block_lexit, 0.0);
  BasicBlock_makeEdge(block_l3, block_l8, 0.0);
  BasicBlock_makeEdge(block_l8, block_l9, 0.0);
  BasicBlock_makeEdge(block_l9, block_l6, 0.0);
  BasicBlock_makeEdge(block_l9, block_l10, 0.0);
  BasicBlock_makeEdge(block_l10, block_l8, 0.0);
  BasicBlock_makeEdge(block_l2, block_l11, 0.0);
  BasicBlock_makeEdge(block_l11, block_lexit, 0.0);
  Procedure_buildCodeRegions(procedure);
  *Optimize__CONVERSION(optimize) |= OptimizeConversion_Cleaning;
  printf("\nFigure 3 in Boissinot's CGO paper:\n");
  {
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Temporary temporary_null = Procedure_makeAssignedTemporary(procedure, Register__UNDEF);
    Temporary temporary_w = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_x = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_y = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_z = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_a1 = Procedure_makeAbsoluteTemporary(procedure, 1, Immediate__UNDEF);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l3),
                               Operator_APPLY, temporary_w, temporary_a1);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l3),
                               Operator_APPLY, temporary_x, temporary_a1);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l3),
                               Operator_APPLY, temporary_y, temporary_a1);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l4),
                               Operator_APPLY, temporary_null, temporary_w);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l5),
                               Operator_APPLY, temporary_null, temporary_y);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l9),
                               Operator_APPLY, temporary_null, temporary_x);
    {
      OldLiveCheck liveCheck;
      Coalescer_ coalescer[1];
      SSAForm ssaForm = SSAForm_construct(Memory_Root, codeRegion, true);
      Dominance dominance = SSAForm_DOMINANCE(ssaForm);
      TemporaryTable_empty(CodeRegion_globalTable(codeRegion));
      SSAForm_fillTemporaryTable(ssaForm, CodeRegion_globalTable(codeRegion));
      CodeRegion_makeConsumers(codeRegion, CodeRegion_globalTable(codeRegion));
      SSAForm_makeOrderings(ssaForm, 4);
#if !_OPTIMIZE
      OldLiveCheck_LIVENESS = 2;
#endif//_OPTIMIZE
      Coalescer_Ctor(coalescer, Memory_Root, codeRegion, CodeRegion_globalTable(codeRegion), dominance, OptimizeCoalescing_Sreedhar);
      Coalescer_complete(coalescer, false);
      liveCheck = (OldLiveCheck)Coalescer_LIVECHECK(coalescer);
#ifndef _NTRACE
      if (OldLiveCheck_TRACE) OldLiveCheck_dumpCFG(liveCheck);
#endif
      //OldLiveCheck_check(liveCheck);
      Coalescer_Dtor(coalescer);
      SSAForm_destruct(ssaForm);
    }
  }
  Program_delete(program);
}
#endif//$XCC__c

#ifdef $XCC__c
{
  // Run Livecheck_check().
  // Figure 1 in Boissinot's presentation.
  Program program = Program_new(Memory_Root, (Processor)1, (Convention)1);
  SymbolTable symbolTable = Program_symbolTable(program);
  Symbol symbol = SymbolTable_lookup(symbolTable, 1, "boissinot-presentation-1");
  Procedure procedure = Program_makeProcedure(program, symbol);
  Optimize optimize = Program_optimize(program);
  LabelTable labelTable = Program_labelTable(program);
  Label LE = LabelTable_lookup(labelTable, 1, "LE");
  Label L0 = LabelTable_lookup(labelTable, 2, "L0");
  Label L1 = LabelTable_lookup(labelTable, 3, "L1");
  Label L2 = LabelTable_lookup(labelTable, 4, "L2");
  Label L3 = LabelTable_lookup(labelTable, 5, "L3");
  Label L4 = LabelTable_lookup(labelTable, 6, "L4");
  Label L5 = LabelTable_lookup(labelTable, 7, "L5");
  Label L6 = LabelTable_lookup(labelTable, 8, "L6");
  Label L7 = LabelTable_lookup(labelTable, 9, "L7");
  Label L8 = LabelTable_lookup(labelTable, 10, "L8");
  Label L9 = LabelTable_lookup(labelTable, 11, "L9");
  Label LEXIT = LabelTable_lookup(labelTable, 12, "Lexit");
  BasicBlock block_le = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l0 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l1 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l2 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l3 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l4 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l5 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l6 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l7 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l8 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_l9 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock block_lexit = Procedure_makeBasicBlock(procedure, 0, 0.0);
  BasicBlock_addLabel(block_le, LE);
  BasicBlock_addLabel(block_l0, L0);
  BasicBlock_addLabel(block_l1, L1);
  BasicBlock_addLabel(block_l2, L2);
  BasicBlock_addLabel(block_l3, L3);
  BasicBlock_addLabel(block_l4, L4);
  BasicBlock_addLabel(block_l5, L5);
  BasicBlock_addLabel(block_l6, L6);
  BasicBlock_addLabel(block_l7, L7);
  BasicBlock_addLabel(block_l8, L8);
  BasicBlock_addLabel(block_l9, L9);
  BasicBlock_addLabel(block_lexit, LEXIT);
  BasicBlock_makeEdge(block_le, block_l0, 0.0);
  BasicBlock_makeEdge(block_l0, block_l1, 0.0);
  BasicBlock_makeEdge(block_l1, block_l2, 0.0);
  BasicBlock_makeEdge(block_l2, block_l3, 0.0);
  BasicBlock_makeEdge(block_l3, block_l2, 0.0);
  BasicBlock_makeEdge(block_l3, block_l4, 0.0);
  BasicBlock_makeEdge(block_l4, block_l0, 0.0);
  BasicBlock_makeEdge(block_l4, block_lexit, 0.0);
  BasicBlock_makeEdge(block_l1, block_l5, 0.0);
  BasicBlock_makeEdge(block_l5, block_l6, 0.0);
  BasicBlock_makeEdge(block_l6, block_l7, 0.0);
  BasicBlock_makeEdge(block_l7, block_l5, 0.0);
  BasicBlock_makeEdge(block_l7, block_l8, 0.0);
  BasicBlock_makeEdge(block_l8, block_l3, 0.0);
  BasicBlock_makeEdge(block_l8, block_l6, 0.0);
  BasicBlock_makeEdge(block_l0, block_l9, 0.0);
  BasicBlock_makeEdge(block_l9, block_lexit, 0.0);
  Procedure_buildCodeRegions(procedure);
  *Optimize__CONVERSION(optimize) |= OptimizeConversion_Cleaning;
  printf("\nFigure 1 in Boissinot's presentation:\n");
  {
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Temporary temporary_null = Procedure_makeAssignedTemporary(procedure, Register__UNDEF);
    Temporary temporary_x = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_y = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_z = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_a1 = Procedure_makeAbsoluteTemporary(procedure, 1, Immediate__UNDEF);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l1),
                               Operator_APPLY, temporary_x, temporary_a1);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l1),
                               Operator_APPLY, temporary_y, temporary_a1);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l2),
                               Operator_APPLY, temporary_null, temporary_y);
    Selector_makeOperation_1_1(BasicBlock_selector(block_l6),
                               Operator_APPLY, temporary_null, temporary_x);
    {
      OldLiveCheck liveCheck;
      Coalescer_ coalescer[1];
      SSAForm ssaForm = SSAForm_construct(Memory_Root, codeRegion, true);
      Dominance dominance = SSAForm_DOMINANCE(ssaForm);
      TemporaryTable_empty(CodeRegion_globalTable(codeRegion));
      SSAForm_fillTemporaryTable(ssaForm, CodeRegion_globalTable(codeRegion));
      CodeRegion_makeConsumers(codeRegion, CodeRegion_globalTable(codeRegion));
      SSAForm_makeOrderings(ssaForm, 4);
#if !_OPTIMIZE
      OldLiveCheck_LIVENESS = 2;
#endif//_OPTIMIZE
      Coalescer_Ctor(coalescer, Memory_Root, codeRegion, CodeRegion_globalTable(codeRegion), dominance, OptimizeCoalescing_Sreedhar);
      Coalescer_complete(coalescer, false);
      liveCheck = (OldLiveCheck)Coalescer_LIVECHECK(coalescer);
#ifndef _NTRACE
      if (OldLiveCheck_TRACE) OldLiveCheck_dumpCFG(liveCheck);
#endif
      //OldLiveCheck_check(liveCheck);
      Coalescer_Dtor(coalescer);
      SSAForm_destruct(ssaForm);
    }
  }
  Program_delete(program);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * OldLiveCheck_TRACE -- File handle used to print the PFA_LiveCheck traces.
 */
#ifndef _NTRACE
static FILE *OldLiveCheck_TRACE;
#else
#define OldLiveCheck_TRACE NULL
#endif//_NTRACE
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Initialize the OldLiveCheck module.
 */
void
OldLiveCheck_INIT(void);
#endif//$XCC__h

void
OldLiveCheck_INIT(void)
{
#ifndef OldLiveCheck_LIVENESS
  if (GETENV("OldLiveCheck_LIVENESS")) {
    sscanf(GETENV("OldLiveCheck_LIVENESS"), "%d", &OldLiveCheck_LIVENESS);
    fprintf(stderr, "*** OldLiveCheck_LIVENESS=%d\n", OldLiveCheck_LIVENESS);
  }
#endif//OldLiveCheck_LIVENESS
#ifndef OldLiveCheck_MERGESETS
  if (GETENV("OldLiveCheck_MERGESETS")) {
    sscanf(GETENV("OldLiveCheck_MERGESETS"), "%d", &OldLiveCheck_MERGESETS);
    fprintf(stderr, "*** OldLiveCheck_MERGESETS=%d\n", OldLiveCheck_MERGESETS);
    if (SSAForm_MERGESETS <= 0) fprintf(stderr, "*** SSAForm_MERGESETS > 0 Required!\n");
  }
#endif//OldLiveCheck_MERGESETS
#ifndef _NTRACE
  if (GETENV("PFA_LiveCheck_TRACE")) {
    int file = 0;
    sscanf(GETENV("PFA_LiveCheck_TRACE"), "%d", &file);
    if (file == 1) OldLiveCheck_TRACE = stdout;
    else if (file == 2) OldLiveCheck_TRACE = stderr;
  }
#endif
}

#ifdef $XCC__h
/**
 * Finalize the OldLiveCheck module.
 */
void
OldLiveCheck_FINI(void);
#endif//$XCC__h

void
OldLiveCheck_FINI(void)
{
#if 0
  // For experiments in CC'2010 submission.
  fprintf(stderr, "SumInstanceCount=%d SumVariableCount=%d SumBlockCount=%d SumMergeSize=%d SumMsSize=%d SumHqSize=%d SumTqSize=%d SumRtSize=%d\n",
                   SumInstanceCount, SumVariableCount, SumBlockCount, SumMergeSize, SumMsSize, SumHqSize, SumTqSize, SumRtSize);
#endif
}

#if XCC__C
static void
OldLiveCheck_TEST(void)
{
#include "PFA/OldLiveCheck_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(OldLiveCheck_TEST);
  return 0;
}
#endif

