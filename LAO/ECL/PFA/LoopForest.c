#ifdef $XCC_h
/*
 * !!!!	LoopForest
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
 * @brief Loop Forest analysis.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/LoopForest.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerate the LoopForestNode types.
 */
typedef enum {
  LoopForestNodeType_Block,
  LoopForestNodeType_Root,
  LoopForestNodeType_SCC,
  LoopForestNodeType_Loop,
  LoopForestNodeType__
} LoopForestNodeType;
typedef uint8_t short_LoopForestNodeType;
const char *
LoopForestNodeType_Id(LoopForestNodeType this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
LoopForestNodeType_Id(LoopForestNodeType this)
{
  static const char *_[] = {
    "Block",
    "Root",
    "SCC",
    "Loop",
  };
  Except_REQUIRE((unsigned)this < LoopForestNodeType__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Region in a LoopForest of a DGraph.
 */
struct LoopForestNode_ {
  //@args	LoopForestNodeType type, BasicBlock block
  short_LoopForestNodeType TYPE;	// This LoopForestNode type.
  int8_t DEPTH;				// Nesting depth of this LoopForestNode.
  int16_t HEADID;			// Loop header (Root or SCC or Loop) ID;
  BasicBlock BLOCK;			// BasicBlock of this LoopForestNode.
  struct LoopForestNode_ *PARENT;	// Parent of this LoopForestNode.
  struct LoopForestNode_ *FIRSTCHILD;	// First child of this LoopForestNode.
  struct LoopForestNode_ *NEXTNODE;	// Next node of the parent LoopForestNode children.
  BitSet ANCESTORS;			// BitSet of ancestors of this LoopForestNode.
};
#endif//$XCC_h

LoopForestNode
LoopForestNode_Ctor(LoopForestNode this,
                    LoopForestNodeType type, BasicBlock block)
{
  *LoopForestNode__TYPE(this) = type;
  *LoopForestNode__DEPTH(this) = -1;
  *LoopForestNode__HEADID(this) = -1;
  *LoopForestNode__BLOCK(this) = block;
  *LoopForestNode__PARENT(this) = NULL;
  *LoopForestNode__FIRSTCHILD(this) = NULL;
  *LoopForestNode__NEXTNODE(this) = NULL;
  *LoopForestNode__ANCESTORS(this) = NULL;
  return this;
}

void
LoopForestNode_Dtor(LoopForestNode this)
{
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LoopForestNode_)\t%zu\n", sizeof(LoopForestNode_));
}
#endif//$XCC__c

#ifdef $XCC_h
static inline LoopForestNodeType
LoopForestNode_type(const_LoopForestNode this)
{
  return LoopForestNode_TYPE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int
LoopForestNode_depth(const_LoopForestNode this)
{
  return LoopForestNode_DEPTH(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline BasicBlock
LoopForestNode_block(const_LoopForestNode this)
{
  return LoopForestNode_BLOCK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline LoopForestNode
LoopForestNode_firstChild(const_LoopForestNode this)
{
  return LoopForestNode_FIRSTCHILD(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline LoopForestNode
LoopForestNode_nextNode(const_LoopForestNode this)
{
  return LoopForestNode_NEXTNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
#define LoopForestNode_CHILDREN_FOREACH_LoopForestNode(this, node) { \
  LoopForestNode node = LoopForestNode_firstChild(this); \
  for (; node != NULL; node = LoopForestNode_nextNode(node)) {
#define LoopForestNode_CHILDREN_ENDEACH_LoopForestNode \
  } \
}
#endif//$XCC_h

static inline void
LoopForestNode_putChild(LoopForestNode this, LoopForestNode childNode)
{
  int depth = LoopForestNode_DEPTH(this) + 1;
  *LoopForestNode__DEPTH(childNode) = depth;
  *LoopForestNode__PARENT(childNode) = this;
  *LoopForestNode__NEXTNODE(childNode) = LoopForestNode_FIRSTCHILD(this);
  *LoopForestNode__FIRSTCHILD(this) = childNode;
  Except_CHECK(LoopForestNode_DEPTH(childNode) == depth);
}

#ifdef $XCC_h
bool
LoopForestNode_pretty(const_LoopForestNode this, FILE *file,
                      bool (*block_pretty)(BasicBlock, FILE*));
#endif//$XCC_h

#ifndef _NTRACE
bool
LoopForestNode_pretty(const_LoopForestNode this, FILE *file,
                      bool (*block_pretty)(BasicBlock, FILE*))
{
  LoopForestNodeType type = LoopForestNode_TYPE(this);
  int headId = LoopForestNode_HEADID(this);
  if (file == NULL) file = stderr;
  if (type == LoopForestNodeType_Block) {
    BasicBlock block = LoopForestNode_BLOCK(this);
    if (block_pretty != NULL) (*block_pretty)(block, file);
    else {
      fprintf(file, " %d", BasicBlock_index(block));
      if (headId >= 0) fprintf(file, "_%c", 'a' + headId);
    }
  } else {
    if (headId >= 0) fprintf(file, " %c", 'a' + headId);
    if (type == LoopForestNodeType_SCC) fprintf(file, "(");
    if (type == LoopForestNodeType_Loop) fprintf(file, "[");
    if (type == LoopForestNodeType_Root) fprintf(file, "{");
    LoopForestNode_CHILDREN_FOREACH_LoopForestNode(this, node) {
      LoopForestNode_pretty(node, file, block_pretty);
    } LoopForestNode_CHILDREN_ENDEACH_LoopForestNode;
    if (type == LoopForestNodeType_Root) fprintf(file, " }");
    if (type == LoopForestNodeType_Loop) fprintf(file, " ]");
    if (type == LoopForestNodeType_SCC) fprintf(file, " )");
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Loop forest for a CodeRegion.
 *
 * The LoopForest maintains two collections of LoopForestNodes:
 * - BLOCKNODEARRAY, that correspond to the BasicBlock(s); This array can be indexed by the
 *   BasicBlock_index.
 * - HEADNODESTACK, that correspond to the inner nodes of the LoopForest. These are created in
 *   topological sort order of the LoopForest, so their index can be used in BitSet.
 * See ./1/LoopForest.1 for examples of these collections.
 */
struct LoopForest_;
#endif//$XCC_h

#ifdef $XCC__h
struct LoopForest_ {
  //@args	Memory memory, CodeRegion codeRegion
  CodeRegion CODEREGION;	// This LoopForest CodeRegion.
  //@access BLOCKCOUNT	CodeRegion_blockCount(LoopForest_CODEREGION(this))
  int32_t *PREDFN;		// Array from BasicBlock INDEX to preorder DFS numbers.
  BasicBlock *BASEBLOCKS;	// Array from BasicBlock INDEX to loop head BasicBlock.
  BasicBlock *PASTBLOCKS;	// Pointer into BASEBLOCKS to implement a stack.
  IArray_ BLOCKNODEARRAY[1];	// Container for the Block LoopForestNode(s).
  //@access MEMORY	IArray_memory(LoopForest_BLOCKNODEARRAY(this))
  //@access BLOCKNODES	((LoopForestNode_ *)IArray_base(LoopForest_BLOCKNODEARRAY(this)))
  IStack_ HEADNODESTACK[1];		// Container for the loop head LoopForestNode(s).
  //@access ROOTNODE	((LoopForestNode)IStack_base(LoopForest_HEADNODESTACK(this)))
  //@access HEADNODES	((LoopForestNode_ *)(IStack_base(LoopForest_HEADNODESTACK(this))))
  BitSet_ *ANCESTORSARRAY;	// Ancestors of header LoopForestNode.
};
#endif//$XCC__h

LoopForest
LoopForest_Ctor(LoopForest this,
                Memory memory, CodeRegion codeRegion)
{
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  BasicBlock rootBlock = CodeRegion_rootBlock(codeRegion);
  *LoopForest__CODEREGION(this) = codeRegion;
  *LoopForest__PREDFN(this) = Memory_alloc(memory, sizeof(int32_t)*blockCount);
  *LoopForest__BASEBLOCKS(this) = Memory_alloc(memory, sizeof(BasicBlock)*blockCount);
  *LoopForest__PASTBLOCKS(this) = LoopForest_BASEBLOCKS(this);
  IArray_Ctor(LoopForest_BLOCKNODEARRAY(this), memory, sizeof(LoopForestNode_), blockCount);
  IStack_Ctor(LoopForest_HEADNODESTACK(this), memory, sizeof(LoopForestNode_), blockCount);
  *LoopForest__ANCESTORSARRAY(this) = NULL;
  LoopForestNode_putChild(LoopForest_makeNode(this, LoopForestNodeType_Root, rootBlock),
                          LoopForest_makeNode(this, LoopForestNodeType_Block, rootBlock));
  LoopForest_makeNestedSCC(this);
  LoopForest_makeAncestors(this);
  return this;
}

void
LoopForest_Dtor(LoopForest this)
{
  Memory memory = LoopForest_MEMORY(this);
  Memory_free_(memory, LoopForest_PREDFN(this));
  Memory_free_(memory, LoopForest_BASEBLOCKS(this));
  IArray_empty(LoopForest_BLOCKNODEARRAY(this), (IArrayItemRelease)LoopForestNode_Dtor);
  IStack_empty(LoopForest_HEADNODESTACK(this), (IStackItemRelease)LoopForestNode_Dtor);
  IStack_Dtor(LoopForest_HEADNODESTACK(this));
  IArray_Dtor(LoopForest_BLOCKNODEARRAY(this));
  Memory_free(memory, LoopForest_ANCESTORSARRAY(this));
}

size_t
LoopForest_Size(Memory memory, CodeRegion codeRegion)
{
  return sizeof(LoopForest_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LoopForest_)\t%zu\n", sizeof(LoopForest_));
}
#endif//$XCC__c

/**
 * Only used for testing.
 */
static void
LoopForest_reset(LoopForest this)
{
  CodeRegion codeRegion = LoopForest_CODEREGION(this);
  BasicBlock rootBlock = CodeRegion_rootBlock(codeRegion);
  *LoopForest__PASTBLOCKS(this) = LoopForest_BASEBLOCKS(this);
  IArray_empty(LoopForest_BLOCKNODEARRAY(this), (IArrayItemRelease)LoopForestNode_Dtor);
  IStack_empty(LoopForest_HEADNODESTACK(this), (IStackItemRelease)LoopForestNode_Dtor);
  LoopForestNode_putChild(LoopForest_makeNode(this, LoopForestNodeType_Root, rootBlock),
                          LoopForest_makeNode(this, LoopForestNodeType_Block, rootBlock));
}

#ifdef $XCC_h
LoopForest
LoopForest_new(Memory parent, CodeRegion codeRegion);
#endif//$XCC_h

LoopForest
LoopForest_new(Memory parent, CodeRegion codeRegion)
{
  Memory memory = Memory_new(parent, true);
  size_t size = LoopForest_Size(memory, codeRegion);
  LoopForest this = Memory_alloc(memory, size);
  return LoopForest_Ctor(this, memory, codeRegion);
}

#ifdef $XCC_h
LoopForest
LoopForest_delete(LoopForest this);
#endif//$XCC_h

LoopForest
LoopForest_delete(LoopForest this)
{
  if (this != NULL) {
    Memory memory = LoopForest_MEMORY(this);
#ifndef _NDTOR
    LoopForest_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This LoopForest Memory.
 */
Memory
LoopForest_memory(LoopForest this);
#endif//$XCC_h

Memory
LoopForest_memory(LoopForest this)
{
  return LoopForest_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This LoopForest CodeRegion.
 */
CodeRegion
LoopForest_codeRegion(LoopForest this);
#endif//$XCC_h

CodeRegion
LoopForest_codeRegion(LoopForest this)
{
  return LoopForest_CODEREGION(this);
}

#ifdef $XCC_h
LoopForestNode
LoopForest_rootNode(LoopForest this);
#endif//$XCC_h

LoopForestNode
LoopForest_rootNode(LoopForest this)
{
  return LoopForest_ROOTNODE(this);
}

#ifdef $XCC_h
const BasicBlock *
LoopForest_baseBlocks(const_LoopForest this);
#endif//$XCC_h

const BasicBlock *
LoopForest_baseBlocks(const_LoopForest this)
{
  return LoopForest_BASEBLOCKS(this);
}

#ifdef $XCC_h
/**
 * For use by LoopForest_BLOCKNODEARRAY_FOREACH_LoopForestNode.
 */
const_IArray
LoopForest_blockNodeArray(const_LoopForest this);
#endif//$XCC_h

const_IArray
LoopForest_blockNodeArray(const_LoopForest this)
{
  return LoopForest_BLOCKNODEARRAY(this);
}

#ifdef $XCC_h
/**
 * Iterate over this LoopForest LoopForestNode(s).
 */
#define LoopForest_BLOCKNODEARRAY_FOREACH_LoopForestNode(this, node) { \
  const_IArray LoopForest_BLOCKNODEARRAY = LoopForest_blockNodeArray(this); \
  IArray_FOREACH(LoopForest_BLOCKNODEARRAY, LoopForestNode_, node) {
#define LoopForest_BLOCKNODEARRAY_ENDEACH_LoopForestNode \
  } IArray_ENDEACH; \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by LoopForest_HEADNODESTACK_FOREACH_LoopForestNode.
 */
const_IStack
LoopForest_headNodeStack(const_LoopForest this);
#endif//$XCC_h

const_IStack
LoopForest_headNodeStack(const_LoopForest this)
{
  return LoopForest_HEADNODESTACK(this);
}

#ifdef $XCC_h
/**
 * Iterate over this LoopForest LoopForestNode(s).
 */
#define LoopForest_HEADNODESTACK_FOREACH_LoopForestNode(this, node) { \
  const_IStack LoopForest_HEADNODESTACK = LoopForest_headNodeStack(this); \
  IStack_FOREACH(LoopForest_HEADNODESTACK, LoopForestNode_, node) {
#define LoopForest_HEADNODESTACK_ENDEACH_LoopForestNode \
  } IStack_ENDEACH; \
}
#endif//$XCC_h

#ifdef $XCC_h
bool
LoopForest_pretty(const_LoopForest this, FILE *file, bool (*block_pretty)(BasicBlock, FILE*));
#endif//$XCC_h

#ifndef _NTRACE
bool
LoopForest_pretty(const_LoopForest this, FILE *file, bool (*block_pretty)(BasicBlock, FILE*))
{
  const_LoopForestNode rootNode = LoopForest_ROOTNODE(this);
  if (file == NULL) file = stderr;
  fprintf(file, "LoopForest:\n");
  LoopForestNode_pretty(rootNode, file, block_pretty);
  fprintf(file, "\n");
  fprintf(file, "LoopForest HEADS:\n");
  LoopForest_HEADNODESTACK_FOREACH_LoopForestNode(this, header) {
    LoopForestNodeType type = LoopForestNode_TYPE(header);
    BasicBlock block = LoopForestNode_BLOCK(header);
    int32_t block_index = BasicBlock_index(block);
    int32_t headId = LoopForestNode_HEADID(header);
    fprintf(file, "  %s_%d \t%c\n", LoopForestNodeType_Id(type), block_index, 'a' + headId);
  } LoopForest_HEADNODESTACK_ENDEACH_LoopForestNode;
  fprintf(file, "LoopForest NODES:\n");
  LoopForest_BLOCKNODEARRAY_FOREACH_LoopForestNode(this, node) {
    LoopForestNodeType type = LoopForestNode_TYPE(node);
    BasicBlock block = LoopForestNode_BLOCK(node);
    int32_t block_index = BasicBlock_index(block);
    fprintf(file, "  %s_%d \t{", LoopForestNodeType_Id(type), block_index);
    BitSet_FOREACH(LoopForestNode_ANCESTORS(node), headId) {
      fprintf(file, " %c", 'a' + headId);
    } BitSet_ENDEACH;
    fprintf(file, " }\n");
  } LoopForest_BLOCKNODEARRAY_ENDEACH_LoopForestNode;
  return true;
}
#endif//_NTRACE

#ifdef $XCC__h
static inline LoopForestNode
LoopForest_makeNode(LoopForest this, LoopForestNodeType type, BasicBlock block)
{
  LoopForestNode node = NULL;
  if (type > LoopForestNodeType_Block) {
    // LoopForestNodeType is Root, SCC or Loop.
    int headId = IStack_count(LoopForest_HEADNODESTACK(this));
    void *item = IStack_push(LoopForest_HEADNODESTACK(this));
    node = LoopForestNode_Ctor(item, type, block);
    *LoopForestNode__HEADID(node) = headId;
  } else {
    // LoopForestNodeType is Block.
    int32_t index = BasicBlock_index(block);
    LoopForestNode_ *blockNodes = LoopForest_BLOCKNODES(this);
    node = LoopForestNode_Ctor(blockNodes + index, type, block);
  }
  return node;
}
#endif//$XCC__h

/*
  This is the classic SCC algorithm from Tarjan adapted from F. Bourdoncle's
  Ph. D. thesis ``Semantique des langages imperatifs d'ordre superieur et interpretation
  abstraite'', 1992, Figure 3.8 p. 41.
 */
static int32_t
LoopForest_visitTarjan(LoopForest this, BasicBlock block,
                       LoopForestNode parent_node, int32_t *_number)
{
  bool loop = false;
  int32_t index = BasicBlock_index(block);
  int32_t *preDFN = LoopForest_PREDFN(this);
  int32_t headDFN = preDFN[index] = ++*_number;
  *LoopForest_PASTBLOCKS(this) = block;
  ++*LoopForest__PASTBLOCKS(this);
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
    int32_t succ_index = BasicBlock_index(succ_block), minDFN = 0;
    if (!succ_index) continue;	// skip rootNode
    if (preDFN[succ_index] <= 0) {
      minDFN = LoopForest_visitTarjan(this, succ_block, parent_node, _number);
    } else {
      minDFN = preDFN[succ_index];
    }
    if (minDFN <= headDFN) {
      headDFN = minDFN;
      loop = true;
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  if (headDFN == preDFN[index]) {
    BasicBlock scc_block = NULL;
    LoopForestNode node = NULL;
    if (loop) {
      node = LoopForest_makeNode(this, LoopForestNodeType_SCC, block);
      LoopForestNode_putChild(parent_node, node);
      parent_node = node;
    }
    do {
      --*LoopForest__PASTBLOCKS(this);
      scc_block = *LoopForest_PASTBLOCKS(this);
      preDFN[BasicBlock_index(scc_block)] = INT32_MAX;
      node = LoopForest_makeNode(this, LoopForestNodeType_Block, scc_block);
      LoopForestNode_putChild(parent_node, node);
    } while (scc_block != block);
  }
  return headDFN;
}

/*
 * This implementation is adapted from F. Bourdoncle's paper, ``Efficient Chaotic
 * Iteration Strategies with Widenings'', Proc. of the International Conference
 * on Formal Methods in Programming and their Applications, Lecture Notes in
 * Computer Science 735, Springer-Verlag 1993. The Tarjan-Bourdoncle algorithm
 * runs in O(de), where d is the maximum loop nesting depth.
 *
 * The algorithm is extended to detect the extra loop entry blocks as proposed by
 * Steensgaard in ``Sequentializing Program Dependence Graphs for Irreducible
 * Programs''. The resulting loop forest is a Havlak loop forest, because the
 * SCC root blocks are removed one at a time. In Steensgaard's forest, all the
 * heads of looping arcs are removed before reprocessing the current SCC.
 *
 * TODO: replace the Boudoncle algorithm by the Havlak algorithm of Ramalingam 1999.
 */
static int32_t
LoopForest_visitBourdoncle(LoopForest this, BasicBlock block,
                           LoopForestNode parent_node, int32_t *_number)
{
  bool loop = false;
  int32_t index = BasicBlock_index(block);
  int32_t *preDFN = LoopForest_PREDFN(this);
  int32_t headDFN = preDFN[index] = ++*_number;
  *LoopForest_PASTBLOCKS(this) = block;
  ++*LoopForest__PASTBLOCKS(this);
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
    int32_t succ_index = BasicBlock_index(succ_block), minDFN = 0;
    if (!succ_index) continue;	// skip rootNode
    if (preDFN[succ_index] <= 0) {
      minDFN = LoopForest_visitBourdoncle(this, succ_block, parent_node, _number);
    } else {
      minDFN = preDFN[succ_index];
    }
    if (minDFN <= headDFN) {
      headDFN = minDFN;
      loop = true;
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  if (headDFN == preDFN[index]) {
    BasicBlock *_last = --*LoopForest__PASTBLOCKS(this);
    BasicBlock scc_block = *_last, *_block = NULL;
    LoopForestNode node = NULL;
    preDFN[index] = INT32_MAX;
    if (loop) {
      int32_t stamp = -1;
      LoopForestNodeType node_type = LoopForestNodeType_Loop;
      BasicBlockFlags block_flags = BasicBlockFlag_Widening+BasicBlockFlag_Natural;
      // Erase the preDFN numbers of the SCC to prepare for recursive SCC search.
      while (scc_block != block) {
        preDFN[BasicBlock_index(scc_block)] = stamp;
        --*LoopForest__PASTBLOCKS(this);
        scc_block = *LoopForest_PASTBLOCKS(this);
      }
      // Traverse the SCC excluding its root to identify the other loop entry blocks.
      for (_block = LoopForest_PASTBLOCKS(this) + 1; _block <= _last; _block++) {
        BasicBlock_clearFlags(*_block, block_flags);
        BasicBlock_ENTER_FOREACH_BasicBlockEdge(*_block, edge) {
          BasicBlock orig_block = BasicBlockEdge_origBlock(edge);
          int32_t orig_index = BasicBlock_index(orig_block);
          *BasicBlockEdge__ISLOOP(edge) = false;
          if (orig_block != block && preDFN[orig_index] != stamp) {
            BasicBlock_setFlags(*_block, BasicBlockFlag_Widening);
            node_type = LoopForestNodeType_SCC;
            block_flags = BasicBlockFlag_Widening;
          }
        } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
      }
      // Add attributes to the SCC root and its entering edges.
      BasicBlock_setFlags(block, block_flags);
      BasicBlock_ENTER_FOREACH_BasicBlockEdge(block, edge) {
        BasicBlock orig_block = BasicBlockEdge_origBlock(edge);
        int32_t orig_index = BasicBlock_index(orig_block);
        *BasicBlockEdge__ISLOOP(edge) = false;
        if (orig_block == block || preDFN[orig_index] == stamp) {
          *BasicBlockEdge__ISLOOP(edge) = true;
//fprintf(stderr, "IsLoop(Block_%d, Block_%d)\n", orig_index, index);
        }
      } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
      // Create the LoopForestNode for the current SCC.
      node = LoopForest_makeNode(this, node_type, block);
      LoopForestNode_putChild(parent_node, node);
      parent_node = node;
      BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
        BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
        int32_t succ_index = BasicBlock_index(succ_block);
        if (preDFN[succ_index] <= 0) {
          LoopForest_visitBourdoncle(this, succ_block, parent_node, _number);
        }
      } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    }
    node = LoopForest_makeNode(this, LoopForestNodeType_Block, scc_block);
    LoopForestNode_putChild(parent_node, node);
  }
  return headDFN;
}

#ifdef $XCC__h
/**
 * Make a LoopForest of Strongly Connected Components.
 */
void
LoopForest_makeSCC(LoopForest this);
#endif//$XCC__h

void
LoopForest_makeSCC(LoopForest this)
{
  int32_t number = 0;
  int32_t *preDFN = LoopForest_PREDFN(this);
  int32_t blockCount = LoopForest_BLOCKCOUNT(this);
  CodeRegion codeRegion = LoopForest_CODEREGION(this);
  memset(LoopForest_PREDFN(this), 0, sizeof(int32_t)*blockCount);
  CodeRegion_FORENTER_BasicBlock(codeRegion, entryBlock) {
    if (preDFN[BasicBlock_index(entryBlock)] <= 0) {
      LoopForest_visitTarjan(this, entryBlock, LoopForest_ROOTNODE(this), &number);
    }
  } CodeRegion_ENDENTER_BasicBlock;
}

#ifdef $XCC__h
/**
 * Make a LoopForest of Nested Strongly Connected Components.
 *
 * The Nested Strongly Connected Components are a for of Loop Forest as defined by
 * G. Ramalingam, ``Identifying loops in almost linear time'', Transactions on
 * Programming Languages and Systems, Volume 21, Number 2, March 1999. F.
 * Bourdoncle introduced this decomposition in his Ph. D. thesis in order to
 * minimize the number of widening points in a control-flow graph.
 */
void
LoopForest_makeNestedSCC(LoopForest this);
#endif//$XCC__h

void
LoopForest_makeNestedSCC(LoopForest this)
{
  int32_t number = 0;
  int32_t *preDFN = LoopForest_PREDFN(this);
  int32_t blockCount = LoopForest_BLOCKCOUNT(this);
  CodeRegion codeRegion = LoopForest_CODEREGION(this);
  memset(LoopForest_PREDFN(this), 0, sizeof(int32_t)*blockCount);
  CodeRegion_FORENTER_BasicBlock(codeRegion, entryBlock) {
    if (preDFN[BasicBlock_index(entryBlock)] <= 0) {
      LoopForest_visitBourdoncle(this, entryBlock, LoopForest_ROOTNODE(this), &number);
    }
  } CodeRegion_ENDENTER_BasicBlock;
}

#ifdef $XCC__h
/**
 * Make the LoopForestNode_ANCESTORS.
 */
void
LoopForest_makeAncestors(LoopForest this);
#endif//$XCC__h

void
LoopForest_makeAncestors(LoopForest this)
{
  Memory memory = LoopForest_MEMORY(this);
  LoopForestNode rootNode = LoopForest_ROOTNODE(this);
  int headerCount = IStack_count(LoopForest_HEADNODESTACK(this));
  BitSet_ *ancestorsArray = Memory_alloc(memory, sizeof(BitSet_)*headerCount);
  // First pass: compute ANCESTORS for headers.
  LoopForest_HEADNODESTACK_FOREACH_LoopForestNode(this, header) {
    int headId = LoopForestNode_HEADID(header);
    BitSet ancestors = ancestorsArray + headId;
    LoopForestNode parent = LoopForestNode_PARENT(header);
    Except_CHECK(headId >= 0 && headId < headerCount);
    BitSet_Ctor(ancestors, memory, headerCount);
    if (parent != NULL) {
      BitSet parent_ancestors = LoopForestNode_ANCESTORS(parent);
      BitSet_assign(ancestors, parent_ancestors);
    }
    BitSet_insert(ancestors, headId);
    *LoopForestNode__ANCESTORS(header) = ancestors;
  } LoopForest_HEADNODESTACK_ENDEACH_LoopForestNode;
  // Second pass: Block nodes use ANCESTORS of their header.
  LoopForest_BLOCKNODEARRAY_FOREACH_LoopForestNode(this, node) {
    BitSet ancestors = LoopForestNode_ANCESTORS(node);
    LoopForestNodeType type = LoopForestNode_TYPE(node);
    if (ancestors == NULL) {
      LoopForestNode parent = LoopForestNode_PARENT(node);
      ancestors = LoopForestNode_ANCESTORS(parent);
      *LoopForestNode__ANCESTORS(node) = ancestors;
      Except_CHECK(type == LoopForestNodeType_Block);
    }
  } LoopForest_BLOCKNODEARRAY_ENDEACH_LoopForestNode;
  *LoopForest__ANCESTORSARRAY(this) = ancestorsArray;
}

#ifdef $XCC__h
/**
 * Find the highest non common ancestor of two Block nodes.
 */
static inline LoopForestNode
LoopForest_findHNCA(LoopForest this, LoopForestNode node_b, LoopForestNode node_a)
{
  LoopForestNode_ *headNodes = LoopForest_HEADNODES(this);
  BitSet ancestors_a = LoopForestNode_ANCESTORS(node_a);
  BitSet ancestors_b = LoopForestNode_ANCESTORS(node_b);
  // nCA = bitset_and ( A(a), bitset_not ( A(b) ) )
  // return node ( bitset_leading_set ( nCA ) )
  int ncaId = BitSet_diffFirst(ancestors_a, ancestors_b);
  if (ncaId == (BitSetMember)-1) return node_a;
  return headNodes + ncaId;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
// Minir initialization for tests
{
  Minir minir = Minir_new(Memory_Root, stdin);
  Program program;
#endif//$XCC__c

#ifdef $XCC__c
  // Test DGraph_makeNestedSCC.
  program = Minir_nextProgram(minir);
  {
    // Example taken from F. Bourdoncle's Ph. D. thesis ``Semantique des langages
    // imperatifs d'ordre superieur et interpretation abstraite'', 1992, Figure
    // 3.6 at page 39.
    Procedure procedure = Program_findProcedure(program, "dgraph-makenested");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    LoopForest loopForest = LoopForest_new(Memory_Root, codeRegion);
    printf("\nIrreducible:\n");
    printf("NSCC ");
    LoopForest_pretty(loopForest, stdout, NULL);
    printf("SCC ");
    LoopForest_reset(loopForest);
    LoopForest_makeSCC(loopForest);
    LoopForest_makeAncestors(loopForest);
    LoopForest_pretty(loopForest, stdout, NULL);
    LoopForest_delete(loopForest);
  }
#endif//$XCC__c

#ifdef $XCC__c
  // Test Procedure_makeNestedSCC.
  {
    // Example taken from F. Bourdoncle's Ph. D. thesis ``Semantique des langages
    // imperatifs d'ordre superieur et interpretation abstraite'', 1992, Figure
    // 3.5 at page 39.
    Procedure procedure = Program_findProcedure(program, "procedure-makenested");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    LoopForest loopForest = LoopForest_new(Memory_Root, codeRegion);
    printf("\nBourdoncle figure 3.5:\n");
    printf("NSCC ");
    LoopForest_pretty(loopForest, stdout, NULL);
    printf("SCC ");
    LoopForest_reset(loopForest);
    LoopForest_makeSCC(loopForest);
    LoopForest_makeAncestors(loopForest);
    LoopForest_pretty(loopForest, stdout, NULL);
    LoopForest_delete(loopForest);
  }
#endif//$XCC__c

#ifdef $XCC__c
  // Test LoopForest_makeNestedSCC
  {
    // Example taken from F. Bourdoncle's paper ``Efficient Chaotic Iteration
    // Strategies with Widenings'', Figure 2.
    Procedure procedure = Program_findProcedure(program, "loopforest-makenested");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    LoopForest loopForest = LoopForest_new(Memory_Root, codeRegion);
    printf("\nBourdoncle figure 2:\n");
    printf("NSCC ");
    LoopForest_pretty(loopForest, stdout, NULL);
    printf("SCC ");
    LoopForest_reset(loopForest);
    LoopForest_makeSCC(loopForest);
    LoopForest_makeAncestors(loopForest);
    LoopForest_pretty(loopForest, stdout, NULL);
    LoopForest_delete(loopForest);
  }
#endif//$XCC__c

#ifdef $XCC__c
  // Test LoopForest_makeNestedSCC
  {
    // Example taken from F. Bourdoncle's paper ``Efficient Chaotic Iteration
    // Strategies with Widenings'', Figure 5.
    Procedure procedure = Program_findProcedure(program, "loopforest-makenested-fig5");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    LoopForest loopForest = LoopForest_new(Memory_Root, codeRegion);
    printf("\nBourdoncle figure 5:\n");
    printf("NSCC ");
    LoopForest_pretty(loopForest, stdout, NULL);
    printf("SCC ");
    LoopForest_reset(loopForest);
    LoopForest_makeSCC(loopForest);
    LoopForest_makeAncestors(loopForest);
    LoopForest_pretty(loopForest, stdout, NULL);
    LoopForest_delete(loopForest);
  }
#endif//$XCC__c

#ifdef $XCC__c
  // Test LoopForest_makeNestedSCC
  {
    // Example taken from G. Ramalingam's report ``On Loops, Dominators, and
    // Dominance Frontiers'', figure 1.
    Procedure procedure = Program_findProcedure(program, "loopforest-makenested-ramalingam");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    LoopForest loopForest = LoopForest_new(Memory_Root, codeRegion);
    printf("\nRamalingam figure 1:\n");
    printf("NSCC ");
    LoopForest_pretty(loopForest, stdout, NULL);
    printf("SCC ");
    LoopForest_reset(loopForest);
    LoopForest_makeSCC(loopForest);
    LoopForest_makeAncestors(loopForest);
    LoopForest_pretty(loopForest, stdout, NULL);
    LoopForest_delete(loopForest);
  }
#endif//$XCC__c

#ifdef $XCC__c
  // Test LoopForest_makeNestedSCC
  {
    // Example taken from G. Ramalingam's report ``On Loops, Dominators, and
    // Dominance Frontiers'', figure 1.
    Procedure procedure = Program_findProcedure(program, "loopforest-makenested-lasttest");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    LoopForest loopForest = LoopForest_new(Memory_Root, codeRegion);
    printf("\nQuicksort:\n");
    printf("NSCC ");
    LoopForest_pretty(loopForest, stdout, NULL);
    printf("SCC ");
    LoopForest_reset(loopForest);
    LoopForest_makeSCC(loopForest);
    LoopForest_makeAncestors(loopForest);
    LoopForest_pretty(loopForest, stdout, NULL);
    LoopForest_delete(loopForest);
  }
#endif//$XCC__c

#ifdef $XCC__c
  Program_delete(program);
// Delete Minir constructs
  Minir_delete(minir);
}
#endif//$XCC__c


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the LoopForest module.
 */
#define LoopForest_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the LoopForest module.
 */
#define LoopForest_FINI()
#endif//$XCC__h

#if XCC__C
static void
LoopForest_TEST(void)
{
#include "PFA/LoopForest_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(LoopForest_TEST);
  return 0;
}
#endif

