#ifdef $XCC_h
/*
 * !!!!	Dominance.xcc
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
 * @ingroup PFA
 * @brief Dominance analysis.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/Dominance.h"
#endif//$XCC__h

#ifdef $XCC__h
#ifndef Dominance_TDMSC
#define Dominance_TDMSC 2
#endif//Dominance_TDMSC
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Dominance flags.
 */
typedef enum {
  DominanceFlag_Forward = 0x0,		//!< Dominance on forward control-flow.
  DominanceFlag_Backward = 0x1,		//!< Post-dominance. Dominance on backward control-flow.
  DominanceFlag_JoinEdges = 0x2,	//!< Dominance computes BasicBlockEdge_ISJOIN.
  DominanceFlag_TreeOrder = 0x4,	//!< Dominance with tree order.
  DominanceFlag_Frontiers = 0x8,	//!< Dominance with frontiers.
  DominanceFlag_MergeSets = 0x10,	//!< Dominance with merge sets.
  DominanceFlag_Dominators = 0x20,	//!< Dominance with dominator sets.
} DominanceFlag;
typedef unsigned DominanceFlags;
typedef uint8_t short_DominanceFlags;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Forward dominance over a CodeRegion.
 */
struct Dominance_;
  //@access isForward	((Dominance_flags(this) & DominanceFlag_Backward) == 0)
  //@access isBackward	((Dominance_flags(this) & DominanceFlag_Backward) != 0)
  //@access hasJoinEdges	((Dominance_flags(this) & DominanceFlag_JoinEdges) == 0)
#endif//$XCC_h

#ifdef $XCC__h
struct Dominance_ {
  //@args	Memory memory, CodeRegion codeRegion, DominanceFlags flags
  Memory MEMORY;
  CodeRegion CODEREGION;
  int32_t BLOCKCOUNT;		//!< Cache of CodeRegion_blockCount.
  const BasicBlock *BLOCKMAP;	//!< Map BasicBlock INDEX to BasicBlock.
  BasicBlock *CFGPORDER;	//!< The BasicBlock(s) in DFS post-order of the CFG.
  BasicBlock *DFSORDER;		//!< BasicBlock(s) in DFS order of the Dominance tree.
  int32_t POSTCOUNT;		//!< Counter used in control graph DFS.
  int32_t *CFGPONUMBER;		//!< The DFS post-order numbers of the CFG.
  int32_t *IDOMIDX;		//!< Immediate dominator indices.
  BitSet_ *CHILDREN;		//!< Immediate dominance children sets.
  BitSet_ *DOMINATORS;		//!< The dominators of each node.
  int32_t *PREDFN;		//!< The pre-order dominance DFS numbers.
  int32_t *LASTDFN;		//!< The pre-order dominance last DFS numbers.
  int32_t *TREELEVEL;		//!< Level in the dominator tree.
  BasicBlock *TREEORDER;	//!< The BasicBlock(s) ordered by non-decreasing TREELEVEL.
  BitSet_ *FRONTIERS;		//!< The dominance frontiers.
  BitSet_ *MERGESETS;		//!< The merge sets (iterated dominance frontiers).
  int32_t JOINTCOUNT;		//!< Count of Join edges.
  DominanceFlags FLAGS;		//!< This Dominance flags.
};
#endif//$XCC__h

Dominance
Dominance_Ctor(Dominance this,
               Memory memory, CodeRegion codeRegion, DominanceFlags flags)
{
  int32_t blockCount = CodeRegion_blockCount(codeRegion), index;
  BasicBlock rootBlock = CodeRegion_rootBlock(codeRegion);
  size_t cfgPOrderSize = sizeof(BasicBlock)*3*blockCount;
  size_t cfgPONumberSize = sizeof(int32_t)*5*blockCount;
  size_t childrenSize = sizeof(BitSet_)*blockCount;
  size_t dominatorsSize = sizeof(BitSet_)*blockCount;
  flags |= DominanceFlag_Dominators;
  if (flags & DominanceFlag_MergeSets) {
    if (Dominance_TDMSC) {
      flags |= DominanceFlag_TreeOrder | DominanceFlag_JoinEdges;
    } else {
      flags |= DominanceFlag_Frontiers;
    }
  }
  *Dominance__MEMORY(this) = memory;
  *Dominance__CODEREGION(this) = codeRegion;
  *Dominance__BLOCKCOUNT(this) = blockCount;
  *Dominance__BLOCKMAP(this) = CodeRegion_blockTableBase(codeRegion);
  *Dominance__CFGPORDER(this) = Memory_alloc(memory, cfgPOrderSize);
  memset(Dominance_CFGPORDER(this), 0, cfgPOrderSize);
  *Dominance__DFSORDER(this) = Dominance_CFGPORDER(this) + blockCount;
  *Dominance__POSTCOUNT(this) = 0;
  *Dominance__CFGPONUMBER(this) = Memory_alloc(memory, cfgPONumberSize);
  memset(Dominance_CFGPONUMBER(this), -1, cfgPONumberSize);
  *Dominance__IDOMIDX(this) = Dominance_CFGPONUMBER(this) + blockCount;
  *Dominance__CHILDREN(this) = Memory_alloc(memory, childrenSize);
  for (index = 0; index < blockCount; index++) {
    BitSet_Ctor(Dominance_CHILDREN(this) + index, memory, blockCount);
  }
  *Dominance__DOMINATORS(this) = NULL;
  if (flags & DominanceFlag_Dominators) {
    *Dominance__DOMINATORS(this) = Memory_alloc(memory, dominatorsSize);
    for (index = 0; index < blockCount; index++) {
      BitSet_Ctor(Dominance_DOMINATORS(this) + index, memory, blockCount);
    }
  }
  *Dominance__PREDFN(this) = Dominance_CFGPONUMBER(this) + 2*blockCount;
  *Dominance__LASTDFN(this) = Dominance_CFGPONUMBER(this) + 3*blockCount;
  *Dominance__TREELEVEL(this) = NULL;
  *Dominance__TREEORDER(this) = NULL;
  if (flags & DominanceFlag_TreeOrder) {
    *Dominance__TREELEVEL(this) = Dominance_CFGPONUMBER(this) + 4*blockCount;
    *Dominance__TREEORDER(this) = Dominance_CFGPORDER(this) + 2*blockCount;
  }
  *Dominance__FRONTIERS(this) = NULL;
  *Dominance__MERGESETS(this) = NULL;
  *Dominance__JOINTCOUNT(this) = -1;
  *Dominance__FLAGS(this) = flags;
  // Make IDOMIDX with the Cooper-Harvey-Kennedy algorithm.
  Dominance_makeIDOMIDX(this, rootBlock, flags);
  // Make CHILDREN from IDOMIDX.
  Dominance_makeCHILDREN(this);
  // Make DOMINATORS from CHILDREN if requested.
  if (flags & DominanceFlag_Dominators) {
    Dominance_makeDOMINATORS(this, rootBlock);
  }
  // Make PREDFN and LASTDFN from CHILDREN.
  Dominance_makePREDFN(this, rootBlock);
  // Mark DJ-Graph Join Edges if requested.
  if (flags & DominanceFlag_JoinEdges) {
    Dominance_markJoinEdges(this);
  }
  // Make TREEORDER and TREELEVEL from CHILDREN if requested.
  if (flags & DominanceFlag_TreeOrder) {
    Dominance_makeTREEORDER(this, rootBlock);
  }
  // Make FRONTIERS from CHILDREN if requested.
  if (flags & DominanceFlag_Frontiers) {
    Dominance_makeFRONTIERS(this, flags);
  }
  // Make MERGESETS from CHILDREN if requested.
  if (flags & DominanceFlag_MergeSets) {
    Dominance_makeMERGESETS(this, flags);
  }
  return this;
}

void
Dominance_Dtor(Dominance this)
{
  Memory memory = Dominance_MEMORY(this);
  int32_t blockCount = Dominance_BLOCKCOUNT(this), index;
  for (index = 0; index < blockCount; index++) {
    BitSet_Dtor(Dominance_DOMINATORS(this) + index);
  }
  for (index = 0; index < blockCount; index++) {
    BitSet_Dtor(Dominance_CHILDREN(this) + index);
  }
  Memory_free_(memory, Dominance_CHILDREN(this));
  Memory_free_(memory, Dominance_CFGPONUMBER(this));
  Memory_free_(memory, Dominance_CFGPORDER(this));
  if (Dominance_FRONTIERS(this) != NULL) {
    for (index = 0; index < blockCount; index++) {
      BitSet_Dtor(Dominance_FRONTIERS(this) + index);
    }
    Memory_free_(memory, Dominance_FRONTIERS(this));
  }
  if (Dominance_MERGESETS(this) != NULL) {
    for (index = 0; index < blockCount; index++) {
      BitSet_Dtor(Dominance_MERGESETS(this) + index);
    }
    Memory_free_(memory, Dominance_MERGESETS(this));
  }
}

size_t
Dominance_Size(Memory memory, CodeRegion codeRegion, DominanceFlags flags)
{
  return sizeof(Dominance_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Dominance_)\t%zu\n", sizeof(Dominance_));
}
#endif//$XCC__c

#ifdef $XCC_h
Dominance
Dominance_new(Memory parent, CodeRegion codeRegion, DominanceFlags flags);
#endif//$XCC_h

Dominance
Dominance_new(Memory parent, CodeRegion codeRegion, DominanceFlags flags)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Dominance_Size(memory, codeRegion, flags);
  Dominance this = Memory_alloc(memory, size);
  return Dominance_Ctor(this, memory, codeRegion, flags);
}

#ifdef $XCC_h
Dominance
Dominance_delete(Dominance this);
#endif//$XCC_h

Dominance
Dominance_delete(Dominance this)
{
  if (this != NULL) {
    Memory memory = Dominance_MEMORY(this);
#ifndef _NDTOR
    Dominance_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * The Cooper-Harvey-Kennedy algorithm for computing immediate dominators.
 *
 * Pseudo-code:
 *
 * @code
 *  1:  for all nodes, n
 *  2:    DOM[n] = {1 ... N}
 *  3:  Changed = true
 *  4:  while (Changed)
 *  5:    Changed = false
 *  6:    for all nodes, n, in reverse postorder
 *  7:    new set = ((for all p from preds(n)) intersection DOM[p]) union {n}
 *  8:    if (new set != DOM[n])
 *  9:      DOM[n] = new set
 * 10:      Changed = true
 * @endcode
 */
void
Dominance_makeIDOMIDX(Dominance this, BasicBlock rootBlock, DominanceFlags flags);
#endif//$XCC__h

void
Dominance_makeIDOMIDX(Dominance this, BasicBlock rootBlock, DominanceFlags flags)
{
  bool changed = true;
  int32_t *idomIdx = Dominance_IDOMIDX(this);
  BasicBlock *cfgPOrder = Dominance_CFGPORDER(this);
  int32_t *cfgPONumber = Dominance_CFGPONUMBER(this);
  int32_t *_postCount = Dominance__POSTCOUNT(this), i;
  *Dominance__POSTCOUNT(this) = idomIdx[0] = 0;
  if (!(flags & DominanceFlag_Backward)) {
    Dominance_doForwardDFS(this, rootBlock, _postCount, cfgPOrder, cfgPONumber);
    Except_CHECK(*_postCount <= Dominance_BLOCKCOUNT(this));
    while (changed) {
      changed = false;
      for (i = *_postCount - 2; i >= 0; --i) {
        BasicBlock block = cfgPOrder[i];
        int32_t b = BasicBlock_index(block), new_idom = -1;
        BasicBlock_ENTER_FOREACH_BasicBlockEdge(block, edge) {
          BasicBlock pred_block = BasicBlockEdge_origBlock(edge);
          int32_t p = BasicBlock_index(pred_block);
          if (idomIdx[p] >= 0) {
            if (new_idom < 0) new_idom = p;
            else {
              // new_idom = intersect(p, new_idom)
              int32_t finger1 = p, finger2 = new_idom;
              while (finger1 != finger2) {
                if (cfgPONumber[finger1] < cfgPONumber[finger2]) finger1 = idomIdx[finger1];
                else if (cfgPONumber[finger2] < cfgPONumber[finger1]) finger2 = idomIdx[finger2];
              }
              new_idom = finger1;
            }
          }
        } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
        if (idomIdx[b] != new_idom) {
          idomIdx[b] = new_idom;
          changed = true;
        }
      }
    }
  } else {
    Dominance_doBackwardDFS(this, rootBlock, _postCount, cfgPOrder, cfgPONumber);
    Except_CHECK(*_postCount <= Dominance_BLOCKCOUNT(this));
    while (changed) {
      changed = false;
      for (i = *_postCount - 2; i >= 0; --i) {
        BasicBlock block = cfgPOrder[i];
        int32_t b = BasicBlock_index(block), new_idom = -1;
        BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
          BasicBlock pred_block = BasicBlockEdge_destBlock(edge);
          int32_t p = BasicBlock_index(pred_block);
          if (idomIdx[p] >= 0) {
            if (new_idom < 0) new_idom = p;
            else {
              // new_idom = intersect(p, new_idom)
              int32_t finger1 = p, finger2 = new_idom;
              while (finger1 != finger2) {
                if (cfgPONumber[finger1] < cfgPONumber[finger2]) finger1 = idomIdx[finger1];
                else if (cfgPONumber[finger2] < cfgPONumber[finger1]) finger2 = idomIdx[finger2];
              }
              new_idom = finger1;
            }
          }
        } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
        if (idomIdx[b] != new_idom) {
          idomIdx[b] = new_idom;
          changed = true;
        }
      }
    }
  }
}

#ifdef $XCC__h
/**
 * DFS of the forward control graph to compute CFGPONUMBER and CFGPORDER.
 */
void
Dominance_doForwardDFS(Dominance this, BasicBlock block,
                       int32_t *_postCount, BasicBlock *cfgPOrder, int32_t *cfgPONumber);
#endif//$XCC__h

void
Dominance_doForwardDFS(Dominance this, BasicBlock block,
                       int32_t *_postCount, BasicBlock *cfgPOrder, int32_t *cfgPONumber)
{
  int32_t index = BasicBlock_index(block);
  cfgPONumber[index] = 0;
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    BasicBlock dest_block = BasicBlockEdge_destBlock(edge);
    int32_t dest_index = BasicBlock_index(dest_block);
    if (cfgPONumber[dest_index] < 0) {
      Dominance_doForwardDFS(this, dest_block, _postCount, cfgPOrder, cfgPONumber);
    }
    *BasicBlockEdge__ISJOIN(edge) = false;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  cfgPONumber[index] = *_postCount;
  cfgPOrder[*_postCount] = block;
  ++*_postCount;
}

#ifdef $XCC__h
/**
 * DFS of the backward control graph to compute CFGPONUMBER and CFGPORDER.
 */
void
Dominance_doBackwardDFS(Dominance this, BasicBlock block,
                        int32_t *_postCount, BasicBlock *cfgPOrder, int32_t *cfgPONumber);
#endif//$XCC__h

void
Dominance_doBackwardDFS(Dominance this, BasicBlock block,
                        int32_t *_postCount, BasicBlock *cfgPOrder, int32_t *cfgPONumber)
{
  int32_t index = BasicBlock_index(block);
  cfgPONumber[index] = 0;
  BasicBlock_ENTER_FOREACH_BasicBlockEdge(block, edge) {
    BasicBlock orig_block = BasicBlockEdge_origBlock(edge);
    int32_t orig_index = BasicBlock_index(orig_block);
    if (cfgPONumber[orig_index] < 0) {
      Dominance_doBackwardDFS(this, orig_block, _postCount, cfgPOrder, cfgPONumber);
    }
    *BasicBlockEdge__ISJOIN(edge) = false;
  } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  cfgPONumber[index] = *_postCount;
  cfgPOrder[*_postCount] = block;
  ++*_postCount;
}

#ifdef $XCC__h
/**
 * Make CHILDREN from IDOMIDX.
 */
void
Dominance_makeCHILDREN(Dominance this);
#endif//$XCC__h

void
Dominance_makeCHILDREN(Dominance this)
{
  int32_t *idomIdx = Dominance_IDOMIDX(this);
  BitSet_ *children = Dominance_CHILDREN(this);
  CodeRegion codeRegion = Dominance_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    int32_t index = BasicBlock_index(block);
    if (idomIdx[index] < 0) idomIdx[index] = 0;
    BitSet_insert(children + idomIdx[index], index);
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * Make DOMINATORS from CHILDREN.
 */
void
Dominance_makeDOMINATORS(Dominance this, BasicBlock rootBlock);
#endif//$XCC__h

void
Dominance_makeDOMINATORS(Dominance this, BasicBlock rootBlock)
{
  BitSet_ *dominators = Dominance_DOMINATORS(this);
  int32_t root_index = BasicBlock_index(rootBlock);
  Except_CHECK(BasicBlock_index(rootBlock) == 0);
  BitSet_insert(dominators + root_index, root_index);
  Dominance_doDOMINATORS(this, rootBlock, dominators);
}

#ifdef $XCC__h
void
Dominance_doDOMINATORS(Dominance this, BasicBlock block, BitSet_ *dominators);
#endif//$XCC__h

void
Dominance_doDOMINATORS(Dominance this, BasicBlock block, BitSet_ *dominators)
{
  int32_t block_index = BasicBlock_index(block);
  Dominance_BasicBlock_FORCHILDREN_BasicBlock(this, block, child_block) {
    int32_t child_index = Dominance_BasicBlock_CHILD_INDEX;
    Except_CHECK(child_index == BasicBlock_index(child_block));
    BitSet_union(dominators + child_index, dominators + block_index);
    BitSet_insert(dominators + child_index, child_index);
    Dominance_doDOMINATORS(this, child_block, dominators);
  } Dominance_BasicBlock_ENDCHILDREN_BasicBlock;
}

#ifdef $XCC__h
/**
 * Make PREDFN and LASTDFN from CHILDREN.
 */
void
Dominance_makePREDFN(Dominance this, BasicBlock rootBlock);
#endif//$XCC__h

void
Dominance_makePREDFN(Dominance this, BasicBlock rootBlock)
{
  int32_t preCount = 0;
  int32_t *preDFN = Dominance_PREDFN(this);
  int32_t *lastDFN = Dominance_LASTDFN(this);
  BasicBlock *dfsOrder = Dominance_DFSORDER(this);
  Dominance_doPREDFN(this, rootBlock, &preCount, preDFN, lastDFN, dfsOrder);
}

#ifdef $XCC__h
/**
 * DFS of the dominator tree to compute PREDFN and LASTDFN and TREELEVEL.
 */
void
Dominance_doPREDFN(Dominance this, BasicBlock block, int32_t *_preCount,
                   int32_t *preDFN, int32_t *lastDFN, BasicBlock *dfsOrder);
#endif//$XCC__h

void
Dominance_doPREDFN(Dominance this, BasicBlock block, int32_t *_preCount,
                   int32_t *preDFN, int32_t *lastDFN, BasicBlock *dfsOrder)
{
  int32_t index = BasicBlock_index(block);
  dfsOrder[*_preCount] = block;
  preDFN[index] = (*_preCount)++;
  Dominance_BasicBlock_FORCHILDREN_BasicBlock(this, block, child_block) {
    Dominance_doPREDFN(this, child_block, _preCount, preDFN, lastDFN, dfsOrder);
  } Dominance_BasicBlock_ENDCHILDREN_BasicBlock;
  lastDFN[index] = *_preCount - 1;
}

#ifdef $XCC__h
/**
 * Set the BasicBlockEdge_ISJOIN and BasicBlockEdge_INDEX for DJ-Graph Join edges.
 */
void
Dominance_markJoinEdges(Dominance this);
#endif//$XCC__h

void
Dominance_markJoinEdges(Dominance this)
{
  int32_t joinCount = 0;
  int32_t *preDFN = Dominance_PREDFN(this);
  int32_t *lastDFN = Dominance_LASTDFN(this);
  int32_t *cfgPONumber = Dominance_CFGPONUMBER(this);
  BasicBlock *cfgPOrder = Dominance_CFGPORDER(this);
  int32_t postCount = Dominance_POSTCOUNT(this), i;
  for (i = postCount - 1; i >= 0; --i) {
    BasicBlock this_block = cfgPOrder[i];
    int32_t this_index = BasicBlock_index(this_block);
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(this_block, edge) {
      BasicBlock that_block = BasicBlockEdge_destBlock(edge);
      int32_t that_index = BasicBlock_index(that_block);
      if (   this_block == that_block
          || !Dominance_DOMINATES(preDFN, lastDFN, this_index, that_index)) {
        *BasicBlockEdge__ISJOIN(edge) = true;
        *BasicBlockEdge__INDEX(edge) = joinCount++;
      }
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  }
  *Dominance__JOINTCOUNT(this) = joinCount;
}

#ifdef $XCC__h
/**
 * BFS of the dominator tree to fill the TREELEVEL and TREEORDER arrays.
 */
void
Dominance_makeTREEORDER(Dominance this, BasicBlock rootBlock);
#endif//$XCC__h

void
Dominance_makeTREEORDER(Dominance this, BasicBlock rootBlock)
{
  BasicBlock *treeOrder = Dominance_TREEORDER(this);
  int32_t *treeLevel = Dominance_TREELEVEL(this);
  BasicBlock *queueFirst = treeOrder;
  BasicBlock *queuePast = treeOrder;
  *queuePast++ = rootBlock;
  treeLevel[0] = 0;
  while (queueFirst != queuePast) {
    BasicBlock block = *queueFirst++;
    int32_t index = BasicBlock_index(block);
    int32_t child_treeLevel = treeLevel[index] + 1;
    Dominance_BasicBlock_FORCHILDREN_BasicBlock(this, block, child_block) {
      int32_t child_index = BasicBlock_index(child_block);
      treeLevel[child_index] = child_treeLevel;
      *queuePast++ = child_block;
    } Dominance_BasicBlock_ENDCHILDREN_BasicBlock;
  }
  Except_CHECK(queuePast - treeOrder == Dominance_POSTCOUNT(this));
}

#ifdef $XCC__h
/**
 * The Cooper-Harvey-Kennedy algorithm for computing dominance frontiers.
 *
 * Pseudo-code:
 *
 * @code
 *  1:  for all nodes, b
 *  2:    if the number of predecessors of b >= 2
 *  3:      for all predecessors, p, of b
 *  4:        runner = p
 *  5:        while runner != doms[b]
 *  6:          add b to runner's dominance frontier set
 *  7:          runner = doms[runner]
 * @endcode
 */
void
Dominance_makeFRONTIERS(Dominance this, DominanceFlags flags);
#endif//$XCC__h

void
Dominance_makeFRONTIERS(Dominance this, DominanceFlags flags)
{
  Memory memory = Dominance_MEMORY(this);
  int32_t *idomIdx = Dominance_IDOMIDX(this);
  CodeRegion codeRegion = Dominance_CODEREGION(this);
  int32_t blockCount = Dominance_BLOCKCOUNT(this), index;
  size_t frontiersSize = sizeof(BitSet_)*blockCount;
  BitSet_ *frontiers = Memory_alloc(memory, frontiersSize);
  for (index = 0; index < blockCount; index++) {
    BitSet_Ctor(frontiers + index, memory, blockCount);
  }
  *Dominance__FRONTIERS(this) = frontiers;
  // Creating the frontiers.
  if (!(flags & DominanceFlag_Backward)) {
    CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
      if (BasicBlock_enterCount(block) >= 2) {
        int32_t b = BasicBlock_index(block);
        BasicBlock_ENTER_FOREACH_BasicBlockEdge(block, edge) {
          BasicBlock pred_block = BasicBlockEdge_origBlock(edge);
          int32_t runner = BasicBlock_index(pred_block);
          Except_CHECK(runner != -1);
          while (runner != idomIdx[b]) {
            BitSet_insert(frontiers + runner, b);
            runner = idomIdx[runner];
            Except_CHECK(runner != -1);
          }
        } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
      }
    } CodeRegion_ENDEACH_BasicBlock;
  } else {
    CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
      if (BasicBlock_leaveCount(block) >= 2) {
        int32_t b = BasicBlock_index(block);
        BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
          BasicBlock pred_block = BasicBlockEdge_destBlock(edge);
          int32_t runner = BasicBlock_index(pred_block);
          Except_CHECK(runner != -1);
          while (runner != idomIdx[b]) {
            BitSet_insert(frontiers + runner, b);
            runner = idomIdx[runner];
            Except_CHECK(runner != -1);
          }
        } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
      }
    } CodeRegion_ENDEACH_BasicBlock;
  }
}

#ifdef $XCC__h
void
Dominance_makeMERGESETS(Dominance this, DominanceFlags flags);
#endif//$XCC__h

void
Dominance_makeMERGESETS(Dominance this, DominanceFlags flags)
{
  Memory memory = Dominance_MEMORY(this);
  CodeRegion codeRegion = Dominance_CODEREGION(this);
  int32_t blockCount = Dominance_BLOCKCOUNT(this), index;
  size_t mergeSetsSize = sizeof(BitSet_)*blockCount;
  BitSet_ *mergeSets = Memory_alloc(memory, mergeSetsSize);
  for (index = 0; index < blockCount; index++) {
    BitSet_Ctor(mergeSets + index, memory, blockCount);
  }
  *Dominance__MERGESETS(this) = mergeSets;
  if (Dominance_TDMSC > 0 && !(flags & DominanceFlag_Backward)) {
    int32_t joinCount = Dominance_JOINTCOUNT(this);
    bool *visited = alloca(sizeof(bool)*joinCount);
    Except_CHECK(flags & DominanceFlag_JoinEdges);
    Except_CHECK(flags & DominanceFlag_TreeOrder);
    do {
      memset(visited, false, sizeof(bool)*joinCount);
    } while (Dominance_doForwardTDMSC(this, visited));
  } else {
    CodeRegion codeRegion = Dominance_CODEREGION(this);
    int32_t blockCount = Dominance_BLOCKCOUNT(this), index;
    bool *visited = alloca(sizeof(bool)*blockCount);
    BasicBlockStack_ worklist[1];
    BasicBlockStack_Ctor(worklist, memory, blockCount);
    // Worklist algorithm to build the dominance frontiers.
    CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
      memset(visited, false, sizeof(bool)*blockCount);
      BasicBlockStack_push(worklist, block);
      while (!BasicBlockStack_isEmpty(worklist)) {
        BasicBlock current_block = BasicBlockStack_top(worklist);
        BasicBlockStack_pop(worklist);
        Dominance_BasicBlock_FORFRONTIER_BasicBlock(this, current_block, frontier_block) {
          int32_t frontier_index = BasicBlock_index(frontier_block);
          if (!visited[frontier_index]) {
            visited[frontier_index] = true;
            BasicBlockStack_push(worklist, frontier_block);
            BitSet_insert(mergeSets + BasicBlock_index(block), frontier_index);
          }
        } Dominance_BasicBlock_ENDFRONTIER_BasicBlock;
      }
    } CodeRegion_ENDEACH_BasicBlock;
    // Release resources.
    BasicBlockStack_Dtor(worklist);
  }
}

#ifdef $XCC__h
bool
Dominance_doForwardTDMSC(Dominance this, bool visited[]);
#endif//$XCC__h

/*
 * The Das-Ramakrishna algorithm Top Down Merge Set Computation (TOPLAS 27, 3).
 *
 * We replace the BFS to 'traverse the DJ graph top down, level by level' by iterator
 * @c Dominance_FORDOWNLEVEL_BasicBlock.
 *
 * TODO: Implement the algorithm for backward control-flow too!
 */
bool
Dominance_doForwardTDMSC(Dominance this, bool visited[])
{
  bool anotherPass = false;
  int32_t *idomIdx = Dominance_IDOMIDX(this);
  BitSet_ *mergeSets = Dominance_MERGESETS(this);
  int32_t *treeLevel = Dominance_TREELEVEL(this);
  const BasicBlock *blockMap = Dominance_BLOCKMAP(this);
  Dominance_FORDOWNLEVEL_BasicBlock(this, n) {
    BasicBlock_ENTER_FOREACH_BasicBlockEdge(n, e) {
      int32_t e_index = BasicBlockEdge_INDEX(e);
      if (BasicBlockEdge_ISJOIN(e) && !visited[e_index]) {
        BasicBlock s = BasicBlockEdge_ORIGBLOCK(e);
        BasicBlock t = BasicBlockEdge_DESTBLOCK(e);
        int32_t s_index = BasicBlock_index(s);
        int32_t t_index = BasicBlock_index(t);
        int32_t tmp = s_index, l_index = -1;
        visited[e_index] = true;
        while (treeLevel[tmp] >= treeLevel[t_index]) {
          BitSet_union(mergeSets + tmp, mergeSets + t_index);
          BitSet_insert(mergeSets + tmp, t_index);
          l_index = tmp, tmp = idomIdx[tmp];
        }
        Except_CHECK(l_index >= 0);
        BasicBlock_ENTER_FOREACH_BasicBlockEdge(blockMap[l_index], e1) {
          int32_t e1_index = BasicBlockEdge_INDEX(e1);
          if (BasicBlockEdge_ISJOIN(e1) && visited[e1_index]) {
            BasicBlock s1 = BasicBlockEdge_ORIGBLOCK(e1);
            int32_t s1_index = BasicBlock_index(s1);
            if (BitSet_comprises(mergeSets + s1_index, mergeSets + l_index));
            else if (Dominance_TDMSC > 1) {
              // Code for TDMSC-II algorithm.
              int32_t tmp1 = s1_index, l1_index = -1;
              while (treeLevel[tmp1] >= treeLevel[l_index]) {
                BitSet_union(mergeSets + tmp1, mergeSets + l_index);
                l1_index = tmp1, tmp1 = idomIdx[tmp1];
              }
              // Code for AnIncomingJEdgeInconsistent(l1_index);
              Except_CHECK(l1_index >= 0);
              BasicBlock_ENTER_FOREACH_BasicBlockEdge(blockMap[l1_index], e2) {
                int32_t e2_index = BasicBlockEdge_INDEX(e2);
                if (BasicBlockEdge_ISJOIN(e2) && visited[e2_index]) {
                  BasicBlock s2 = BasicBlockEdge_ORIGBLOCK(e2);
                  int32_t s2_index = BasicBlock_index(s2);
                  if (BitSet_comprises(mergeSets + s2_index, mergeSets + l1_index));
                  else {
                    anotherPass = true;
                    break;
                  }
                }
              } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
            } else {
              // Code for TDMSC-I algorithm.
              anotherPass = true;
            }
          }
        } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
      }
    } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  } Dominance_ENDDOWNLEVEL_BasicBlock;
  return anotherPass;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Dominance Memory.
 */
Memory
Dominance_memory(Dominance this);
#endif//$XCC_h

Memory
Dominance_memory(Dominance this)
{
  return Dominance_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This Dominance CodeRegion.
 */
CodeRegion
Dominance_codeRegion(Dominance this);
#endif//$XCC_h

CodeRegion
Dominance_codeRegion(Dominance this)
{
  return Dominance_CODEREGION(this);
}

#ifdef $XCC_h
/**
 * This Dominance flags.
 */
unsigned
Dominance_flags(Dominance this);
#endif//$XCC_h

unsigned
Dominance_flags(Dominance this)
{
  return Dominance_FLAGS(this);
}

#ifdef $XCC_h
/**
 * This Dominance BasicBlock count.
 */
int32_t
Dominance_blockCount(Dominance this);
#endif//$XCC_h

int32_t
Dominance_blockCount(Dominance this)
{
  return Dominance_BLOCKCOUNT(this);
}

#ifdef $XCC_h
/**
 * This Dominance BasicBlock map.
 */
const BasicBlock *
Dominance_blockMap(Dominance this);
#endif//$XCC_h

const BasicBlock *
Dominance_blockMap(Dominance this)
{
  return Dominance_BLOCKMAP(this);
}

#ifdef $XCC_h
/**
 * For use by Dominance_FORCFGPORDER_BasicBlock.
 */
const BasicBlock *
Dominance_cfgPOrder(Dominance this);
#endif//$XCC_h

const BasicBlock *
Dominance_cfgPOrder(Dominance this)
{
  return Dominance_CFGPORDER(this);
}

#ifdef $XCC_h
/**
 * For use by Dominance_FORDFSORDER_BasicBlock.
 */
const BasicBlock *
Dominance_dfsOrder(Dominance this);
#endif//$XCC_h

const BasicBlock *
Dominance_dfsOrder(Dominance this)
{
  return Dominance_DFSORDER(this);
}

#ifdef $XCC_h
/**
 * For use by Dominance_FORCFGPORDER_BasicBlock.
 */
int32_t
Dominance_postCount(Dominance this);
#endif//$XCC_h

int32_t
Dominance_postCount(Dominance this)
{
  return Dominance_POSTCOUNT(this);
}

#ifdef $XCC_h
/**
 * Iterate BasicBlock(s) in CFG Post-Order.
 *
 * Iteration omits the CodeRegion ROOTBLOCK (whose BasicBlock_INDEX = 0).
 */
#define Dominance_FORCFGPORDER_BasicBlock(this, basicBlock) { \
  const BasicBlock *Dominance_CFGPORDER = Dominance_cfgPOrder(this); \
  int32_t Dominance_POSTCOUNT = Dominance_postCount(this), Dominance_INDEX = 0; \
  for (; Dominance_INDEX < Dominance_POSTCOUNT - 1; Dominance_INDEX++) { \
    BasicBlock basicBlock = Dominance_CFGPORDER[Dominance_INDEX];
#define Dominance_ENDCFGPORDER_BasicBlock \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate BasicBlock(s) in CFG Reverse Post-Order.
 *
 * Iteration omits the CodeRegion ROOTBLOCK (whose BasicBlock_INDEX = 0).
 */
#define Dominance_FORCFGRPORDER_BasicBlock(this, basicBlock) { \
  const BasicBlock *Dominance_CFGPORDER = Dominance_cfgPOrder(this); \
  int32_t Dominance_POSTCOUNT = Dominance_postCount(this); \
  int32_t Dominance_INDEX = Dominance_POSTCOUNT - 2; \
  for (; Dominance_INDEX >= 0; --Dominance_INDEX) { \
    BasicBlock basicBlock = Dominance_CFGPORDER[Dominance_INDEX];
#define Dominance_ENDCFGRPORDER_BasicBlock \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate BasicBlock(s) in Dominance DFS Order.
 *
 * Iteration omits the CodeRegion ROOTBLOCK (whose BasicBlock_INDEX = 0).
 */
#define Dominance_FORDFSORDER_BasicBlock(this, basicBlock) { \
  const BasicBlock *Dominance_DFSORDER = Dominance_dfsOrder(this); \
  int32_t Dominance_POSTCOUNT = Dominance_postCount(this), Dominance_DFSINDEX = 1; \
  for (; Dominance_DFSINDEX < Dominance_POSTCOUNT; Dominance_DFSINDEX++) { \
    BasicBlock basicBlock = Dominance_DFSORDER[Dominance_DFSINDEX];
#define Dominance_ENDDFSORDER_BasicBlock \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Dominance immediate dominator indices.
 */
const int32_t *
Dominance_idomIdx(const_Dominance this);
#endif//$XCC_h

const int32_t *
Dominance_idomIdx(const_Dominance this)
{
  return Dominance_IDOMIDX(this);
}

#ifdef $XCC_h
/**
 * Immediate dominator of a BasicBlock.
 */
BasicBlock
Dominance_basicBlockDominator(const_Dominance this, const_BasicBlock basicBlock);
#endif//$XCC_h

BasicBlock
Dominance_basicBlockDominator(const_Dominance this, const_BasicBlock basicBlock)
{
  const BasicBlock *blockMap = Dominance_BLOCKMAP(this);
  const int32_t *idomIdx = Dominance_IDOMIDX(this);
  int32_t idomIndex = idomIdx[BasicBlock_index(basicBlock)];
  return blockMap[idomIndex];
}

#ifdef $XCC_h
/**
 * This Dominance tree DFS preorder numbers.
 */
const int32_t *
Dominance_preDFN(const_Dominance this);
#endif//$XCC_h

const int32_t *
Dominance_preDFN(const_Dominance this)
{
  return Dominance_PREDFN(this);
}

#ifdef $XCC_h
/**
 * This Dominance tree DFS preorder number of last descendent.
 */
const int32_t *
Dominance_lastDFN(const_Dominance this);
#endif//$XCC_h

const int32_t *
Dominance_lastDFN(const_Dominance this)
{
  return Dominance_LASTDFN(this);
}

#ifdef $XCC_h
/**
 * This Dominance tree levels.
 */
const int32_t *
Dominance_treeLevel(const_Dominance this);
#endif//$XCC_h

const int32_t *
Dominance_treeLevel(const_Dominance this)
{
  return Dominance_TREELEVEL(this);
}

#ifdef $XCC_h
/**
 * For use by Dominance_BasicBlock_FORCHILDREN_BasicBlock.
 */
BitSet_ *
Dominance_children(Dominance this);
#define Dominance_hasChildren(this)	(Dominance_children(this) != NULL)
#endif//$XCC_h

BitSet_ *
Dominance_children(Dominance this)
{
  return Dominance_CHILDREN(this);
}

#ifdef $XCC_h
/**
 * Iterate over CHILDREN BasicBlock(s).
 */
#define Dominance_BasicBlock_FORCHILDREN_BasicBlock(this, block, child_block) { \
  BitSet Dominance_CHILDREN = Dominance_children(this) + BasicBlock_index(block); \
  const BasicBlock *Dominance_BLOCKMAP = Dominance_blockMap(this); \
  BitSet_FOREACH(Dominance_CHILDREN, Dominance_BasicBlock_CHILD_INDEX) { \
    BasicBlock child_block = Dominance_BLOCKMAP[Dominance_BasicBlock_CHILD_INDEX];
#define Dominance_BasicBlock_ENDCHILDREN_BasicBlock \
  } BitSet_ENDEACH; \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by Dominance_BasicBlock_FORDOMINATORS_BasicBlock.
 */
BitSet_ *
Dominance_dominators(Dominance this);
#define Dominance_hasDominators(this)	(Dominance_dominators(this) != NULL)
#endif//$XCC_h

BitSet_ *
Dominance_dominators(Dominance this)
{
  return Dominance_DOMINATORS(this);
}

#ifdef $XCC_h
/**
 * Iterate over DOMINATORS BasicBlock(s).
 */
#define Dominance_BasicBlock_FORDOMINATORS_BasicBlock(this, block, dominator_block) { \
  BitSet Dominance_DOMINATORS = Dominance_dominators(this) + BasicBlock_index(block); \
  const BasicBlock *Dominance_BLOCKMAP = Dominance_blockMap(this); \
  BitSet_FOREACH(Dominance_DOMINATORS, Dominance_BasicBlock_DOMINATOR_INDEX) { \
    BasicBlock dominator_block = Dominance_BLOCKMAP[Dominance_BasicBlock_DOMINATOR_INDEX];
#define Dominance_BasicBlock_ENDDOMINATORS_BasicBlock \
  } BitSet_ENDEACH; \
}
#endif//$XCC_h

#ifdef $XCC_h
BasicBlock
Dominance_basicBlockIdom(Dominance this, BasicBlock block);
#endif//$XCC_h

BasicBlock
Dominance_basicBlockIdom(Dominance this, BasicBlock block) {
  int32_t idomIndex = Dominance_IDOMIDX(this)[BasicBlock_index(block)];
  return idomIndex >=0? Dominance_blockMap(this)[idomIndex]: NULL;
}

#ifdef $XCC_h
/**
 * For use by Dominance_FORDOWNLEVEL_BasicBlock.
 */
const BasicBlock *
Dominance_treeOrder(Dominance this);
#define Dominance_hasTreeOrder(this)	(Dominance_treeOrder(this) != NULL)
#endif//$XCC_h

const BasicBlock *
Dominance_treeOrder(Dominance this)
{
  return Dominance_TREEORDER(this);
}

#ifdef $XCC_h
/**
 * Iterate over BasicBlock(s) by increasing TREELEVEL.
 *
 * Iteration omits the CodeRegion ROOTBLOCK (whose BasicBlock_INDEX = 0).
 */
#define Dominance_FORDOWNLEVEL_BasicBlock(this, block) { \
  const BasicBlock *Dominance_TREEORDER = Dominance_treeOrder(this); \
  int32_t Dominance_POSTCOUNT = Dominance_postCount(this); \
  int32_t Dominance_INDEX = 1; \
  for (; Dominance_INDEX < Dominance_POSTCOUNT; Dominance_INDEX++) { \
    BasicBlock block = Dominance_TREEORDER[Dominance_INDEX];
#define Dominance_ENDDOWNLEVEL_BasicBlock \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over BasicBlock(s) by decreasing TREELEVEL.
 *
 * Iteration omits the CodeRegion ROOTBLOCK (whose BasicBlock_INDEX = 0).
 */
#define Dominance_FORUPLEVEL_BasicBlock(this, block) { \
  const BasicBlock *Dominance_TREEORDER = Dominance_treeOrder(this); \
  int32_t Dominance_POSTCOUNT = Dominance_postCount(this); \
  int32_t Dominance_INDEX = Dominance_POSTCOUNT - 1; \
  for (; Dominance_INDEX > 0; Dominance_INDEX--) { \
    BasicBlock block = Dominance_TREEORDER[Dominance_INDEX];
#define Dominance_ENDUPLEVEL_BasicBlock \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by Dominance_BasicBlock_FORFRONTIER_BasicBlock.
 */
BitSet_ *
Dominance_frontiers(Dominance this);
#define Dominance_hasFrontiers(this)	(Dominance_frontiers(this) != NULL)
#endif//$XCC_h

BitSet_ *
Dominance_frontiers(Dominance this)
{
  return Dominance_FRONTIERS(this);
}

#ifdef $XCC_h
/**
 * Iterate over frontier BasicBlock(s).
 */
#define Dominance_BasicBlock_FORFRONTIER_BasicBlock(this, block, frontier_block) { \
  BitSet Dominance_FRONTIER = Dominance_frontiers(this) + BasicBlock_index(block); \
  const BasicBlock *Dominance_BLOCKMAP = Dominance_blockMap(this); \
  BitSet_FOREACH(Dominance_FRONTIER, Dominance_BasicBlock_FRONTIER_INDEX) { \
    BasicBlock frontier_block = Dominance_BLOCKMAP[Dominance_BasicBlock_FRONTIER_INDEX];
#define Dominance_BasicBlock_ENDFRONTIER_BasicBlock \
  } BitSet_ENDEACH; \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by Dominance_BasicBlock_FORMERGESET_BasicBlock.
 */
BitSet_ *
Dominance_mergeSets(Dominance this);
#define Dominance_hasMergeSets(this)	(Dominance_mergeSets(this) != NULL)
#endif//$XCC_h

BitSet_ *
Dominance_mergeSets(Dominance this)
{
  return Dominance_MERGESETS(this);
}

#ifdef $XCC_h
/**
 * Iterate over merge set BasicBlock(s).
 */
#define Dominance_BasicBlock_FORMERGESET_BasicBlock(this, block, mergeset_block) { \
  BitSet Dominance_MERGESET = Dominance_mergeSets(this) + BasicBlock_index(block); \
  const BasicBlock *Dominance_BLOCKMAP = Dominance_blockMap(this); \
  BitSet_FOREACH(Dominance_MERGESET, Dominance_BasicBlock_MERGE_INDEX) { \
    BasicBlock mergeset_block = Dominance_BLOCKMAP[Dominance_BasicBlock_MERGE_INDEX];
#define Dominance_BasicBlock_ENDMERGESET_BasicBlock \
  } BitSet_ENDEACH; \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Non-strict dominance relation between two BasicBlocks.
 *
 * Application of the (not so) well-known properties of DFS numbering.
 *
 * For the strict dominance relation, use the following:
 *	 (   this_block != that_block
 *	  && Dominance_dominates(this, this_block, that_block))
 */
bool
Dominance_dominates(const_Dominance this, const_BasicBlock this_block, const_BasicBlock that_block);
#define Dominance_DOMINATES(preDFN, lastDFN, this_index, that_index) ( \
     preDFN[this_index] <= preDFN[that_index] \
  && lastDFN[this_index] >= preDFN[that_index] \
)
#endif//$XCC_h

bool
Dominance_dominates(const_Dominance this, const_BasicBlock this_block, const_BasicBlock that_block)
{
  const int32_t *preDFN = Dominance_PREDFN(this);
  const int32_t *lastDFN = Dominance_LASTDFN(this);
  int32_t this_index = BasicBlock_index(this_block);
  int32_t that_index = BasicBlock_index(that_block);
  return Dominance_DOMINATES(preDFN, lastDFN, this_index, that_index);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty-print this Dominance.
 */
bool
Dominance_pretty(Dominance this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Dominance_pretty(Dominance this, FILE *file)
{
  CodeRegion codeRegion = Dominance_CODEREGION(this);
  int32_t *treeLevel = Dominance_TREELEVEL(this);
  if (file == NULL) file = stderr;
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    fprintf(file, "block_%d:", BasicBlock_index(basicBlock));
    if (Dominance_hasChildren(this)) {
      fprintf(file, "\tCHILDREN={");
      Dominance_BasicBlock_FORCHILDREN_BasicBlock(this, basicBlock, child_block) {
        fprintf(file, " block_%d", BasicBlock_index(child_block));
      } Dominance_BasicBlock_ENDCHILDREN_BasicBlock;
    }
    if (Dominance_hasDominators(this)) {
      fprintf(file, " }");
      fprintf(file, "\tDOMINATORS={");
      Dominance_BasicBlock_FORDOMINATORS_BasicBlock(this, basicBlock, dominator_block) {
        fprintf(file, " block_%d", BasicBlock_index(dominator_block));
      } Dominance_BasicBlock_ENDDOMINATORS_BasicBlock;
    }
    if (Dominance_hasFrontiers(this)) {
      fprintf(file, " }");
      fprintf(file, "\tFRONTIER={");
      Dominance_BasicBlock_FORFRONTIER_BasicBlock(this, basicBlock, frontier_block) {
        fprintf(file, " block_%d", BasicBlock_index(frontier_block));
      } Dominance_BasicBlock_ENDFRONTIER_BasicBlock;
    }
    if (Dominance_hasMergeSets(this)) {
      fprintf(file, " }");
      fprintf(file, "\tMERGESET={");
      Dominance_BasicBlock_FORMERGESET_BasicBlock(this, basicBlock, mergeset_block) {
        fprintf(file, " block_%d", BasicBlock_index(mergeset_block));
      } Dominance_BasicBlock_ENDMERGESET_BasicBlock;
    }
    fprintf(file, " }");
    if (Dominance_hasTreeOrder(this)) {
      fprintf(file, "\tTREELEVEL=%d", treeLevel[BasicBlock_index(basicBlock)]);
    }
    fprintf(file, "\n");
  } CodeRegion_ENDEACH_BasicBlock;
  if (Dominance_hasTreeOrder(this)) {
    fprintf(file, "treeOrder:");
    Dominance_FORDOWNLEVEL_BasicBlock(this, basicBlock) {
      fprintf(file, "\tblock_%d=>%d", BasicBlock_index(basicBlock),
              treeLevel[BasicBlock_index(basicBlock)]);
    } Dominance_ENDDOWNLEVEL_BasicBlock;
    fprintf(file, "\n");
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC__c
// Test forward Dominance_makeFrontiers.
{
  // Initialize Minir parser
  Minir minir = Minir_new(Memory_Root, stdin);
  Program program;
  Minir_KEEP_INDEX(minir) = true;
  program = Minir_nextProgram(minir);
  {
    Procedure procedure = Program_findProcedure(program, "dominance-forward");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Dominance dom = Dominance_new(Memory_Root, codeRegion,
                                  DominanceFlag_Forward+
                                  DominanceFlag_TreeOrder+
                                  DominanceFlag_Frontiers+
                                  DominanceFlag_MergeSets);
    printf("\nBriggs-Harvey Forward:\n");
    Dominance_pretty(dom, stdout);
    CodeRegion_FOREACH_BasicBlock(codeRegion, this_block) {
      printf("block_%d dominates:", BasicBlock_index(this_block));
      CodeRegion_FOREACH_BasicBlock(codeRegion, that_block) {
        if (Dominance_dominates(dom, this_block, that_block)) {
          printf("\tblock_%d", BasicBlock_index(that_block));
        }
      } CodeRegion_ENDEACH_BasicBlock;
      printf("\n");
    } CodeRegion_ENDEACH_BasicBlock;
    Dominance_delete(dom);
  }
#endif//$XCC__c

#ifdef $XCC__c
// Test forward Dominance_makeFrontiers.
  {
    Procedure procedure = Program_findProcedure(program, "dominance-forward2");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Dominance dom = Dominance_new(Memory_Root, codeRegion,
                                  DominanceFlag_Forward+
                                  DominanceFlag_TreeOrder+
                                  DominanceFlag_Frontiers+
                                  DominanceFlag_MergeSets);
    printf("\nCooper-Harvey-Kennedy Figure 2:\n");
    Dominance_pretty(dom, stdout);
    CodeRegion_FOREACH_BasicBlock(codeRegion, this_block) {
      printf("block_%d dominates:", BasicBlock_index(this_block));
      CodeRegion_FOREACH_BasicBlock(codeRegion, that_block) {
        if (Dominance_dominates(dom, this_block, that_block)) {
          printf("\tblock_%d", BasicBlock_index(that_block));
        }
      } CodeRegion_ENDEACH_BasicBlock;
      printf("\n");
    } CodeRegion_ENDEACH_BasicBlock;
    printf("post-order:\n");
    Dominance_FORCFGPORDER_BasicBlock(dom, block) {
      printf("\tblock_%d", BasicBlock_index(block));
    } Dominance_ENDCFGPORDER_BasicBlock;
    printf("\n");
    printf("reverse post-order:\n");
    Dominance_FORCFGRPORDER_BasicBlock(dom, block) {
      printf("\tblock_%d", BasicBlock_index(block));
    } Dominance_ENDCFGRPORDER_BasicBlock;
    printf("\n");
    Dominance_delete(dom);
  }
#endif//$XCC__c

#ifdef $XCC__c
// Test forward Dominance_makeFrontiers.
  {
    Procedure procedure = Program_findProcedure(program, "dominance-forward3");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Dominance dom = Dominance_new(Memory_Root, codeRegion,
                                  DominanceFlag_Forward+
                                  DominanceFlag_TreeOrder+
                                  DominanceFlag_Frontiers+
                                  DominanceFlag_MergeSets);
    printf("\nCooper-Harvey-Kennedy Figure 4:\n");
    Dominance_pretty(dom, stdout);
    CodeRegion_FOREACH_BasicBlock(codeRegion, this_block) {
      printf("block_%d dominates:", BasicBlock_index(this_block));
      CodeRegion_FOREACH_BasicBlock(codeRegion, that_block) {
        if (Dominance_dominates(dom, this_block, that_block)) {
          printf("\tblock_%d", BasicBlock_index(that_block));
        }
      } CodeRegion_ENDEACH_BasicBlock;
      printf("\n");
    } CodeRegion_ENDEACH_BasicBlock;
    Dominance_delete(dom);
  }
#endif//$XCC__c

#ifdef $XCC__c
// Test backward Dominance
  {
    Procedure procedure = Program_findProcedure(program, "dominance-backward");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Dominance dom = Dominance_new(Memory_Root, codeRegion,
                                  DominanceFlag_Backward+
                                  DominanceFlag_TreeOrder+
                                  DominanceFlag_Frontiers+
                                  DominanceFlag_MergeSets);
    printf("\nBriggs-Harvey Backward:\n");
    Dominance_pretty(dom, stdout);
    CodeRegion_FOREACH_BasicBlock(codeRegion, this_block) {
      printf("block_%d dominates:", BasicBlock_index(this_block));
      CodeRegion_FOREACH_BasicBlock(codeRegion, that_block) {
        if (Dominance_dominates(dom, this_block, that_block)) {
          printf("\tblock_%d", BasicBlock_index(that_block));
        }
      } CodeRegion_ENDEACH_BasicBlock;
      printf("\n");
    } CodeRegion_ENDEACH_BasicBlock;
    Dominance_delete(dom);
  }
#endif//$XCC__c

#ifdef $XCC__c
// Test backward Dominance with infinite loop
  if (0) {
    Procedure procedure = Program_findProcedure(program, "dominance-backward-inf-loop");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Dominance dom = Dominance_new(Memory_Root, codeRegion,
                                  DominanceFlag_Backward+
                                  DominanceFlag_TreeOrder+
                                  DominanceFlag_Frontiers+
                                  DominanceFlag_MergeSets);
    printf("\nBriggs-Harvey Backward on infinite loop:\n");
    Dominance_pretty(dom, stdout);
    CodeRegion_FOREACH_BasicBlock(codeRegion, this_block) {
      printf("block_%d dominates:", BasicBlock_index(this_block));
      CodeRegion_FOREACH_BasicBlock(codeRegion, that_block) {
        if (Dominance_dominates(dom, this_block, that_block)) {
          printf("\tblock_%d", BasicBlock_index(that_block));
        }
      } CodeRegion_ENDEACH_BasicBlock;
      printf("\n");
    } CodeRegion_ENDEACH_BasicBlock;
    Dominance_delete(dom);
  }
#endif//$XCC__c


#ifdef $XCC__c
// Test forward Dominance_makeFrontiers.
  {
    Procedure procedure = Program_findProcedure(program, "dominance-forward4");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    Dominance dom = Dominance_new(Memory_Root, codeRegion,
                                  DominanceFlag_Forward+
                                  DominanceFlag_TreeOrder+
                                  DominanceFlag_Frontiers+
                                  DominanceFlag_MergeSets);
    printf("Das-Ramakrishna Forward:\n");
    Dominance_pretty(dom, stdout);
    //CodeRegion_pretty(codeRegion, stdout);
    CodeRegion_FOREACH_BasicBlock(codeRegion, this_block) {
      printf("block_%d dominates:", BasicBlock_index(this_block));
      CodeRegion_FOREACH_BasicBlock(codeRegion, that_block) {
        if (Dominance_dominates(dom, this_block, that_block)) {
          printf("\tblock_%d", BasicBlock_index(that_block));
        }
      } CodeRegion_ENDEACH_BasicBlock;
      printf("\n");
    } CodeRegion_ENDEACH_BasicBlock;
    Dominance_delete(dom);
  }
  Program_delete(program);
  Minir_delete(minir);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Dominance module.
 */
#define Dominance_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Dominance module.
 */
#define Dominance_FINI()
#endif//$XCC__h

#if XCC__C
static void
Dominance_TEST(void)
{
#include "PFA/Dominance_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(Dominance_TEST);
  return 0;
}
#endif

