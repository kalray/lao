#ifdef $XCC_h
/*
 * !!!!	DGraph.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2007 STMicroelectronics.
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
 * @ingroup CAL
 * @brief Directed Graph implementation.
 */
#endif//$XCC_h


#include "CAL/CAL_.h"

#ifdef $XCC__h
#include "CAL/DGraph.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pseudo type for DGraph arcs.
 */
typedef void DGraphArc_, *DGraphArc;
typedef const void *const_DGraphArc;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pseudo type for DGraph nodes.
 */
typedef void DGraphNode_, *DGraphNode;
typedef const void *const_DGraphNode;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access to the DGraphArcCell under @c this arc.
 */
#define DGraphArc_CELL(this) ((DGraphArcCell)(this) - 1)
/**
 * Tail DGraphNode of @c this DGraphArc.
 */
#define DGraphArc_TAILNODE(this) DGraphArcCell_TAILNODE(DGraphArc_CELL(this))
/**
 * Head DGraphNode of @c this DGraphArc.
 */
#define DGraphArc_HEADNODE(this) DGraphArcCell_HEADNODE(DGraphArc_CELL(this))
/**
 * Next DGraphArc in the list of leaving arcs.
 */
#define DGraphArc_LEAVEARC(this) DGraphArcCell_LEAVEARC(DGraphArc_CELL(this))
/**
 * Next DGraphArc in the list of entering arcs.
 */
#define DGraphArc_ENTERARC(this) DGraphArcCell_ENTERARC(DGraphArc_CELL(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Mutator for the next DGraphArc in the list of leaving arcs.
 */
#define DGraphArc__LEAVEARC(this) DGraphArcCell__LEAVEARC(DGraphArc_CELL(this))
/**
 * Mutator for the next DGraphArc in the list of entering arcs.
 */
#define DGraphArc__ENTERARC(this) DGraphArcCell__ENTERARC(DGraphArc_CELL(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Index of the tail DGraphNode of @c this DGraphArc.
 */
#define DGraphArc_tailIndex(this) DGraphNode_INDEX(DGraphArc_TAILNODE(this))
/**
 * Index of the head DGraphNode of @c this DGraphArc.
 */
#define DGraphArc_headIndex(this) DGraphNode_INDEX(DGraphArc_HEADNODE(this))
/**
 * Number of the tail DGraphNode of @c this DGraphArc.
 */
#define DGraphArc_tailNumber(this) DGraphNode_NUMBER(DGraphArc_TAILNODE(this))
/**
 * Number of the head DGraphNode of @c this DGraphArc.
 */
#define DGraphArc_headNumber(this) DGraphNode_NUMBER(DGraphArc_HEADNODE(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Attach this DGraphArc to a tail DGraphNode.
 */
void
DGraphArc_attachTailNode(DGraphArc this, DGraphNode tailNode);
#endif//$XCC_h

void
DGraphArc_attachTailNode(DGraphArc this, DGraphNode tailNode)
{
  DGraphArcCell this_cell = DGraphArc_CELL(this);
  DGraphNodeCell tail_nodeCell = DGraphNode_CELL(tailNode);
  Except_REQUIRE(DGraphArcCell_TAILNODE(this_cell) == NULL);
  *DGraphArcCell__TAILNODE(this_cell) = tailNode;
  *DGraphArcCell__LEAVEARC(this_cell) = DGraphNodeCell_LEAVEARC(tail_nodeCell);
  *DGraphNodeCell__LEAVEARC(tail_nodeCell) = this;
}

#ifdef $XCC_h
/**
 * Attach this DGraphArc to a head DGraphNode.
 */
void
DGraphArc_attachHeadNode(DGraphArc this, DGraphNode headNode);
#endif//$XCC_h

void
DGraphArc_attachHeadNode(DGraphArc this, DGraphNode headNode)
{
  DGraphArcCell this_cell = DGraphArc_CELL(this);
  DGraphNodeCell head_nodeCell = DGraphNode_CELL(headNode);
  Except_REQUIRE(DGraphArcCell_HEADNODE(this_cell) == NULL);
  *DGraphArcCell__HEADNODE(this_cell) = headNode;
  *DGraphArcCell__ENTERARC(this_cell) = DGraphNodeCell_ENTERARC(head_nodeCell);
  *DGraphNodeCell__ENTERARC(head_nodeCell) = this;
}

#ifdef $XCC_h
/**
 * Detach this DGraphArc from its tail DGraphNode.
 */
void
DGraphArc_detachTailNode(DGraphArc this);
#endif//$XCC_h

void
DGraphArc_detachTailNode(DGraphArc this)
{
  DGraphArcCell this_cell = DGraphArc_CELL(this);
  DGraphNode tailNode = DGraphArcCell_TAILNODE(this_cell);
  DGraphNodeCell tail_nodeCell = DGraphNode_CELL(tailNode);
  DGraphArc arc = DGraphNodeCell_LEAVEARC(tail_nodeCell), prev_arc = NULL;
  while (arc != this) {
    Except_CHECK(arc != NULL);
    prev_arc = arc;
    arc = DGraphArc_LEAVEARC(arc);
  }
  if (prev_arc != NULL) {
    DGraphArcCell prev_arcCell = DGraphArc_CELL(prev_arc);
    *DGraphArcCell__LEAVEARC(prev_arcCell) = DGraphArc_LEAVEARC(arc);
  } else {
    *DGraphNodeCell__LEAVEARC(tail_nodeCell) = DGraphArc_LEAVEARC(arc);
  }
  *DGraphArcCell__LEAVEARC(this_cell) = NULL;
  *DGraphArcCell__TAILNODE(this_cell) = NULL;
}

#ifdef $XCC_h
/**
 * Detach this DGraphArc from its head DGraphNode.
 */
void
DGraphArc_detachHeadNode(DGraphArc this);
#endif//$XCC_h

void
DGraphArc_detachHeadNode(DGraphArc this)
{
  DGraphArcCell this_cell = DGraphArc_CELL(this);
  DGraphNode headNode = DGraphArcCell_HEADNODE(this_cell);
  DGraphNodeCell head_nodeCell = DGraphNode_CELL(headNode);
  DGraphArc arc = DGraphNodeCell_ENTERARC(head_nodeCell), prev_arc = NULL;
  while (arc != this) {
    Except_CHECK(arc != NULL);
    prev_arc = arc;
    arc = DGraphArc_ENTERARC(arc);
  }
  if (prev_arc != NULL) {
    DGraphArcCell prev_arcCell = DGraphArc_CELL(prev_arc);
    *DGraphArcCell__ENTERARC(prev_arcCell) = DGraphArc_ENTERARC(arc);
  } else {
    *DGraphNodeCell__ENTERARC(head_nodeCell) = DGraphArc_ENTERARC(arc);
  }
  *DGraphArcCell__ENTERARC(this_cell) = NULL;
  *DGraphArcCell__HEADNODE(this_cell) = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Access to the DGraphNodeCell under @c this node.
 */
#define DGraphNode_CELL(this) ((DGraphNodeCell)(this) - 1)
/**
 * Index of the node in the DGraph, starts at 0.
 */
#define DGraphNode_INDEX(this) DGraphNodeCell_INDEX(DGraphNode_CELL(this))
/**
 * Number of the node in the DGraph, starts at 1.
 */
#define DGraphNode_NUMBER(this) (DGraphNode_INDEX(this) + 1)
/**
 * Accessor for the next node in scanning order.
 */
#define DGraphNode_SCANNODE(this) DGraphNodeCell_SCANNODE(DGraphNode_CELL(this))
/**
 * Mutator for the next node in scanning order.
 */
#define DGraphNode__SCANNODE(this) DGraphNodeCell__SCANNODE(DGraphNode_CELL(this))
/**
 * First arc to leave this DGraphNode.
 */
#define DGraphNode_LEAVEARC(this) DGraphNodeCell_LEAVEARC(DGraphNode_CELL(this))
/**
 * First arc to enter this DGraphNode.
 */
#define DGraphNode_ENTERARC(this) DGraphNodeCell_ENTERARC(DGraphNode_CELL(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Mutator for the first arc to leave this DGraphNode.
 */
#define DGraphNode__LEAVEARC(this) DGraphNodeCell__LEAVEARC(DGraphNode_CELL(this))
/**
 * Mutator for the first arc to enter this DGraphNode.
 */
#define DGraphNode__ENTERARC(this) DGraphNodeCell__ENTERARC(DGraphNode_CELL(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the leaving arc of this node.
 */
#define DGraphNode_FORLEAVE(this, Type, arc) { \
  Type *DGraphNode_LEAVEARC = DGraphNode_LEAVEARC(this); \
  while (DGraphNode_LEAVEARC != NULL) { \
    Type *(arc) = DGraphNode_LEAVEARC; \
    DGraphNode_LEAVEARC = DGraphArc_LEAVEARC(DGraphNode_LEAVEARC);
#define DGraphNode_ENDLEAVE \
  } \
} \
/* DGraphNode_ENDLEAVE */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the entering arc of this node.
 */
#define DGraphNode_FORENTER(this, Type, arc) { \
  Type *DGraphNode_ENTERARC = DGraphNode_ENTERARC(this); \
  while (DGraphNode_ENTERARC != NULL) { \
    Type *(arc) = DGraphNode_ENTERARC; \
    DGraphNode_ENTERARC = DGraphArc_ENTERARC(DGraphNode_ENTERARC);
#define DGraphNode_ENDENTER \
  } \
} \
/* DGraphNode_ENDENTER */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Detach this DGraphNode enter arc list.
 */
void
DGraphNode_detachEnterArcs(DGraphNode this);
#endif//$XCC_h

void
DGraphNode_detachEnterArcs(DGraphNode this)
{
  DGraphArc enter_arc = DGraphNode_ENTERARC(this);
  while (enter_arc != NULL) {
    DGraphArcCell arcCell = DGraphArc_CELL(enter_arc);
    enter_arc = DGraphArc_ENTERARC(enter_arc);
    *DGraphArcCell__ENTERARC(arcCell) = NULL;
    *DGraphArcCell__HEADNODE(arcCell) = NULL;
  }
  *DGraphNode__ENTERARC(this) = NULL;
}

#ifdef $XCC_h
/**
 * Detach this DGraphNode leave arc list.
 */
void
DGraphNode_detachLeaveArcs(DGraphNode this);
#endif//$XCC_h

void
DGraphNode_detachLeaveArcs(DGraphNode this)
{
  DGraphArc leave_arc = DGraphNode_LEAVEARC(this);
  while (leave_arc != NULL) {
    DGraphArcCell arcCell = DGraphArc_CELL(leave_arc);
    leave_arc = DGraphArc_LEAVEARC(leave_arc);
    *DGraphArcCell__LEAVEARC(arcCell) = NULL;
    *DGraphArcCell__TAILNODE(arcCell) = NULL;
  }
  *DGraphNode__LEAVEARC(this) = NULL;
}

#ifdef $XCC_h
/**
 * Sort this DGraphNode leave arc list.
 */
void
DGraphNode_sortLeaveArcs(DGraphNode this, int (*cmp)(const_DGraphArc, const_DGraphArc));
#endif//$XCC_h

void
DGraphNode_sortLeaveArcs(DGraphNode this, int (*cmp)(const_DGraphArc, const_DGraphArc))
{
  void *leave = DGraphNode_LEAVEARC(this);
  if (leave != NULL && DGraphArc_LEAVEARC(leave) != NULL) {
    bool sorted = true;
    int32_t arcCount = 0, i = 0;
    DGraphArc prev_arc = leave;
    DGraphNode_FORLEAVE(this, DGraphArc_, arc) {
      if ((*cmp)(prev_arc, arc) > 0) sorted = false;
      prev_arc = arc;
      arcCount++;
    } DGraphNode_ENDLEAVE;
    if (!sorted) {
      DGraphArc arcArray[128];
      DGraphArc *arcs = arcCount > 128?
                        Memory_alloc(Memory_Root, sizeof(DGraphArc)*arcCount):
                        arcArray;
      DGraphNode_FORLEAVE(this, DGraphArc_, arc) {
        arcs[i++] = arc;
      } DGraphNode_ENDLEAVE;
      HackerPtr_SortArray(arcs, 0, arcCount - 1, cmp);
      *DGraphNode__LEAVEARC(this) = arcs[0];
      for (i = 0; i < arcCount - 1; i++) {
        *DGraphArc__LEAVEARC(arcs[i]) = arcs[i + 1];
      }
      *DGraphArc__LEAVEARC(arcs[i]) = NULL;
      if (arcCount > 128) Memory_free_(Memory_Root, arcs);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
struct DGraphArcCell_ {
  //@args
  DGraphNode TAILNODE;	// Tail DGraphNode.
  DGraphNode HEADNODE;	// Head DGraphNode.
  DGraphArc LEAVEARC;	// Next in the leaving DGraphArc list.
  DGraphArc ENTERARC;	// Next in the entering DGraphArc list.
  //@access ARC	((DGraphArc)((DGraphArcCell)(this) + 1))
};
#endif//$XCC_h

DGraphArcCell
DGraphArcCell_Ctor(DGraphArcCell this)
{
  *DGraphArcCell__TAILNODE(this) = NULL;
  *DGraphArcCell__HEADNODE(this) = NULL;
  *DGraphArcCell__LEAVEARC(this) = NULL;
  *DGraphArcCell__ENTERARC(this) = NULL;
  return this;
}

void
DGraphArcCell_Dtor(DGraphArcCell this)
{
  *DGraphArcCell__TAILNODE(this) = NULL;
  *DGraphArcCell__HEADNODE(this) = NULL;
  *DGraphArcCell__LEAVEARC(this) = NULL;
  *DGraphArcCell__ENTERARC(this) = NULL;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(DGraphArcCell_)\t%zu\n", sizeof(DGraphArcCell_));
  Except_ALWAYS(_ALIGNED(sizeof(DGraphArcCell_)));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
struct DGraphNodeCell_ {
  //@args	int32_t index
  int32_t INDEX;	// Node index in DGraph_NODES.
  DGraphNode SCANNODE;	// Next node in the node scanning list.
  DGraphArc LEAVEARC;	// First in the leaving DGraphArc list.
  DGraphArc ENTERARC;	// Last in the leaving DGraphArc list.
  //@access NODE	((DGraphNode)((DGraphNodeCell)(this) + 1))
};
#endif//$XCC_h

DGraphNodeCell
DGraphNodeCell_Ctor(DGraphNodeCell this, int32_t index)
{
  *DGraphNodeCell__INDEX(this) = index;
  *DGraphNodeCell__SCANNODE(this) = NULL;
  *DGraphNodeCell__LEAVEARC(this) = NULL;
  *DGraphNodeCell__ENTERARC(this) = NULL;
  return this;
}

void
DGraphNodeCell_Dtor(DGraphNodeCell this)
{
  *DGraphNodeCell__SCANNODE(this) = NULL;
  *DGraphNodeCell__LEAVEARC(this) = NULL;
  *DGraphNodeCell__ENTERARC(this) = NULL;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(DGraphNodeCell_)\t%zu\n", sizeof(DGraphNodeCell_));
  Except_ALWAYS(_ALIGNED(sizeof(DGraphNodeCell_)));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
struct DGraph_ {
  //@args	Memory memory, int32_t maxNodes
  //  memory:	Where the DGraph nodes and arcs are allocated.
  //  maxNodes:	An estimate of the maximum number of nodes.
  DGraphNode ROOTNODE;	// Root node, defaulted to the first node added.
  DGraphNode SCANNODE;	// Tail of the node scanning list (defined by clients).
  int32_t NODECOUNT;
  int32_t ARCCOUNT;
  PtrSeq_ NODESTACK_;
  //@access NODESTACK	DGraph__NODESTACK_(this)
  //@access NODES	PtrSeq_BASE(DGraph_NODESTACK(this))
  //@access MEMORY	PtrSeq_MEMORY(DGraph_NODESTACK(this))
};
#endif//$XCC_h

DGraph
DGraph_Ctor(DGraph this, Memory memory, int32_t maxNodes)
{
  *DGraph__ROOTNODE(this) = NULL;
  *DGraph__SCANNODE(this) = NULL;
  *DGraph__NODECOUNT(this) = 0;
  *DGraph__ARCCOUNT(this) = 0;
  PtrSeq_Ctor(DGraph_NODESTACK(this), memory, maxNodes);
  return this;
}

void
DGraph_Dtor(DGraph this)
{
  PtrSeq_Dtor(DGraph_NODESTACK(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(DGraph_)\t%zu\n", sizeof(DGraph_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Empty this DGraph.
 */
void
DGraph_empty(DGraph this,
             void (*release_node)(DGraphNode),
             void (*release_arc)(DGraphArc));
#endif//$XCC_h

void
DGraph_empty(DGraph this,
             void (*release_node)(DGraphNode),
             void (*release_arc)(DGraphArc))
{
  Memory memory = DGraph_MEMORY(this);
  DGraph_FOREACH(this, DGraphNode_, node) {
    DGraphArcCell prev_arcCell = NULL;
    DGraphNodeCell nodeCell = DGraphNode_CELL(node);
    DGraphNode_FORLEAVE(node, DGraphArc_, arc) {
      DGraphArcCell arcCell = DGraphArc_CELL(arc);
      if (prev_arcCell != NULL) {
        DGraphArcCell_Dtor(prev_arcCell);
        if (release_arc != NULL) (*release_arc)(arc);
        Memory_free_(memory, prev_arcCell);
      }
      prev_arcCell = arcCell;
    } DGraphNode_ENDLEAVE;
    if (prev_arcCell != NULL) {
      DGraphArcCell_Dtor(prev_arcCell);
      Memory_free_(memory, prev_arcCell);
    }
    DGraphNodeCell_Dtor(nodeCell);
    if (release_node != NULL) (*release_node)(node);
    Memory_free(memory, nodeCell);
  } DGraph_ENDEACH;
  *DGraph__ROOTNODE(this) = NULL;
  *DGraph__SCANNODE(this) = NULL;
  *DGraph__NODECOUNT(this) = 0;
  *DGraph__ARCCOUNT(this) = 0;
  PtrSeq_empty(DGraph_NODESTACK(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Iterate over this DGraph nodes.
 */
#define DGraph_FOREACH(this, Type, node) { \
  DGraphNode *DGraph_NODES = DGraph_nodes(this); \
  int32_t DGraph_COUNT = DGraph_count(this), DGraph_I; \
  for (DGraph_I = 0; DGraph_I < DGraph_COUNT; DGraph_I++) { \
    Type *(node) = DGraph_NODES[DGraph_I];
#define DGraph_ENDEACH \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Scan this DGraph nodes.
 */
#define DGraph_FORSCAN(this, Type, node) { \
  Type *(node) = (Type *)DGraph_rootNode(this); \
  for (; node != NULL; node = (Type *)DGraphNode_SCANNODE(node)) {
#define DGraph_ENDSCAN \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access this DGraph memory.
 */
static inline Memory
DGraph_memory(DGraph this)
{
  return DGraph_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access the DGraph root node.
 */
static inline DGraphNode
DGraph_rootNode(DGraph this)
{
  return DGraph_ROOTNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Mutate the DGraph root node.
 */
static inline DGraphNode *
DGraph__rootNode(DGraph this)
{
  return DGraph__ROOTNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by DGraph_FORSCAN.
 */
static inline DGraphNode
DGraph_scanNode(DGraph this)
{
  return DGraph_SCANNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Mutate the DGraph scan node.
 */
static inline DGraphNode *
DGraph__scanNode(DGraph this)
{
  return DGraph__SCANNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This DGraph count of DGraphNodes.
 */
static inline int32_t
DGraph_nodeCount(DGraph this)
{
  return DGraph_NODECOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This DGraph count of DGraphArcs.
 */
static inline int32_t
DGraph_arcCount(DGraph this)
{
  return DGraph_ARCCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by DGraph_FOREACH.
 */
static inline int32_t
DGraph_count(DGraph this)
{
  PtrSeq nodeStack = DGraph_NODESTACK(this);
  return PtrSeq_count(nodeStack);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Array of DGraphNode(s) indexed by DGraphNode_NUMBER.
 */
static inline DGraphNode *
DGraph_nodeOf(DGraph this)
{
  return (DGraphNode *)DGraph_NODES(this) - 1;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by DGraph_FOREACH.
 */
static inline DGraphNode *
DGraph_nodes(DGraph this)
{
  return DGraph_NODES(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make a new DGraph node.
 */
DGraphNode
DGraph_makeNode(DGraph this, size_t nodeSize);
#endif//$XCC_h

DGraphNode
DGraph_makeNode(DGraph this, size_t nodeSize)
{
  Memory memory = DGraph_MEMORY(this);
  size_t adjustedSize = sizeof(DGraphNodeCell_) + nodeSize;
  size_t align_size = _ALIGN_NEXT(adjustedSize);
  DGraphNodeCell nodeCell = Memory_alloc_(memory, align_size);
  DGraphNode node = DGraphNodeCell_NODE(nodeCell);
  int32_t count = DGraph_count(this);
  PtrSeq_push2(DGraph_NODESTACK(this), node);
  DGraphNodeCell_Ctor(DGraphNode_CELL(node), count);
  if (count == 0) {
    *DGraph__ROOTNODE(this) = node;
  }
  ++*DGraph__NODECOUNT(this);
  return node;
}

#ifdef $XCC_h
/**
 * Access a DGraph node from its index.
 */
static inline DGraphNode
DGraph_access(DGraph this, int32_t nodeIndex)
{
  Except_REQUIRE(nodeIndex >= 0 && nodeIndex < DGraph_count(this));
  return DGraph_nodes(this)[nodeIndex];
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Attach an already constructed DGraph arc in this DGraph.
 */
static inline DGraphArc
DGraph_attachArc(DGraph this, DGraphArc arc, DGraphNode tail_node, DGraphNode head_node)
{
  if (tail_node != NULL) DGraphArc_attachTailNode(arc, tail_node);
  if (head_node != NULL) DGraphArc_attachHeadNode(arc, head_node);
  ++*DGraph__ARCCOUNT(this);
  return arc;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make and attach a new DGraph arc in this DGraph.
 */
DGraphArc
DGraph_makeArc(DGraph this, DGraphNode tail_node, DGraphNode head_node, size_t arcSize);
#endif//$XCC_h

DGraphArc
DGraph_makeArc(DGraph this, DGraphNode tail_node, DGraphNode head_node, size_t arcSize)
{
  Memory memory = DGraph_MEMORY(this);
  size_t adjustedSize = sizeof(DGraphArcCell_) + arcSize;
  size_t align_size = _ALIGN_NEXT(adjustedSize);
  DGraphArcCell arcCell = DGraphArcCell_Ctor(Memory_alloc_(memory, align_size));
  return DGraph_attachArc(this, DGraphArcCell_ARC(arcCell), tail_node, head_node);
}

#ifdef $XCC_h
/**
 * Get an arc between two designated nodes.
 */
DGraphArc
DGraph_getArc(DGraph this, DGraphNode tail_node, DGraphNode head_node, int position);
#endif//$XCC_h

DGraphArc
DGraph_getArc(DGraph this, DGraphNode tail_node, DGraphNode head_node, int position)
{
  DGraphNode_FORLEAVE(tail_node, DGraphArc_, arc) {
    if (DGraphArc_HEADNODE(arc) == head_node && position-- == 0) return arc;
  } DGraphNode_ENDLEAVE;
  return NULL;
}

#ifdef $XCC_h
/**
 * Detach a DGraphArc.
 */
static inline void
DGraph_detachArc(DGraph this, DGraphArc arc)
{
  if (DGraphArc_TAILNODE(arc) != NULL) {
    DGraphArc_detachTailNode(arc);
  }
  if (DGraphArc_HEADNODE(arc) != NULL) {
    DGraphArc_detachHeadNode(arc);
  }
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Remove a DGraphArc from this DGraph.
 */
void
DGraph_removeArc(DGraph this, DGraphArc arc);
#endif//$XCC_h

void
DGraph_removeArc(DGraph this, DGraphArc arc)
{
  Memory memory = DGraph_MEMORY(this);
  DGraphArcCell arcCell = DGraphArc_CELL(arc);
  DGraph_detachArc(this, arc);
  DGraphArcCell_Dtor(arcCell);
  Memory_free_(memory, arcCell);
  --*DGraph__ARCCOUNT(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  DGraphArc arc = NULL;
  DGraph_ dgraph_;
  DGraph dgraph = DGraph_Ctor(&dgraph_, Memory_Root, 10);
  DGraphNode node_0 = DGraph_makeNode(dgraph, sizeof(char));
  DGraphNode node_1 = DGraph_makeNode(dgraph, sizeof(char));
  DGraphNode node_2 = DGraph_makeNode(dgraph, sizeof(char));
  DGraphNode node_3 = DGraph_makeNode(dgraph, sizeof(char));
  DGraphNode node_4 = DGraph_makeNode(dgraph, sizeof(char));
  DGraphNode node_5 = DGraph_makeNode(dgraph, sizeof(char));
  DGraphNode node_6 = DGraph_makeNode(dgraph, sizeof(char));
  DGraphNode node_7 = DGraph_makeNode(dgraph, sizeof(char));
  *(char *)node_0 = 'a'; *(char *)node_1 = 'b';
  *(char *)node_2 = 'c'; *(char *)node_3 = 'd';
  *(char *)node_4 = 'e'; *(char *)node_5 = 'f';
  *(char *)node_6 = 'g'; *(char *)node_7 = 'h';
  DGraph_makeArc(dgraph, node_0, node_1, 0);	// a -> b
  DGraph_makeArc(dgraph, node_1, node_2, 0);	// b -> c
  DGraph_makeArc(dgraph, node_1, node_4, 0);	// b -> e
  DGraph_makeArc(dgraph, node_1, node_5, 0);	// b -> f
  DGraph_makeArc(dgraph, node_2, node_3, 0);	// c -> d
  DGraph_makeArc(dgraph, node_2, node_6, 0);	// c -> g
  DGraph_makeArc(dgraph, node_3, node_2, 0);	// d -> c
  DGraph_makeArc(dgraph, node_3, node_7, 0);	// d -> h
  DGraph_makeArc(dgraph, node_4, node_0, 0);	// e -> a
  DGraph_makeArc(dgraph, node_4, node_5, 0);	// e -> f
  DGraph_makeArc(dgraph, node_5, node_6, 0);	// f -> g
  DGraph_makeArc(dgraph, node_6, node_5, 0);	// g -> f
  DGraph_makeArc(dgraph, node_6, node_7, 0);	// g -> h
  DGraph_makeArc(dgraph, node_7, node_7, 0);	// h -> h
  printf("dgraph 1:\n");
  DGraph_print(dgraph);
  printf("dgraph 1 SCCs:\n");
  DGraph_visit(dgraph);
  arc = DGraph_getArc(dgraph, node_4, node_5, 0);	// e -> f
  printf("del %c -> %c\n", *(char *)DGraphArc_TAILNODE(arc), *(char *)DGraphArc_HEADNODE(arc));
  DGraph_removeArc(dgraph, arc);
  arc = DGraph_makeArc(dgraph, node_5, node_4, 0);	// f -> e
  printf("new %c -> %c\n", *(char *)DGraphArc_TAILNODE(arc), *(char *)DGraphArc_HEADNODE(arc));
  printf("dgraph 2:\n");
  DGraph_print(dgraph);
  printf("dgraph 2 SCCs:\n");
  DGraph_visit(dgraph);
  DGraph_Dtor(dgraph);
}
#endif//$XCC__c

#if XCC__C
int id;
int V;
int *val;
PtrSeq stack;
DGraph dgraph;

static void
DGraph_print(DGraph this)
{
  printf("forward stars:\n");
  DGraph_FOREACH(this, DGraphNode_, node) {
    printf("  node %c:\n", *(char *)node);
    DGraphNode_FORLEAVE(node, DGraphArc_, leave) {
      DGraphNode tailNode = DGraphArc_TAILNODE(leave);
      DGraphNode headNode = DGraphArc_HEADNODE(leave);
      printf("    %c -> %c\n", *(char *)tailNode, *(char *)headNode);
    } DGraphNode_ENDLEAVE;
  } DGraph_ENDEACH;
  printf("backward stars:\n");
  DGraph_FOREACH(this, DGraphNode_, node) {
    printf("  node %c:\n", *(char *)node);
    DGraphNode_FORENTER(node, DGraphArc_, enter) {
      DGraphNode tailNode = DGraphArc_TAILNODE(enter);
      DGraphNode headNode = DGraphArc_HEADNODE(enter);
      printf("    %c <- %c\n", *(char *)headNode, *(char *)tailNode);
    } DGraphNode_ENDENTER;
  } DGraph_ENDEACH;
}

static int
DGraphNode_visit(DGraphNode this)
{
  int m, min;
  int k = DGraphNode_INDEX(this);
  val[k] = min = ++id;
  PtrSeq_push(stack, (void *)(intptr_t)k);
  DGraphNode_FORLEAVE(this, DGraphArc_, arc) {
    DGraphNode head = DGraphArc_HEADNODE(arc);
    int index = DGraphNode_INDEX(head);
    if (val[index] == 0) {
      m = DGraphNode_visit(head);
    } else {
      m = val[index];
    }
    if (m < min) min = m;
  } DGraphNode_ENDLEAVE;
  if (min == val[k]) {
    do {
      m = (intptr_t)PtrSeq_lastItem(stack);
      PtrSeq_pop(stack);
      printf("%c ", *((char *)DGraph_access(dgraph, m)));
      val[m] = V + 1;
    } while (m != k);
    printf("\n");
  }
  return min;
}

static void
DGraph_visit(DGraph this)
{
  PtrSeq_ stack_;
  id = 0;
  V = DGraph_count(this);
  val = malloc(sizeof(int)*V);
  dgraph = this;
  memset(val, 0, V*sizeof(int));
  stack = &stack_;
  PtrSeq_Ctor(stack, Memory_Root, V);
  DGraphNode_visit(DGraph_rootNode(dgraph));
  PtrSeq_Dtor(stack);
  stack = NULL;
  free(val);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the DGraph module.
 */
#define DGraph_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the DGraph module.
 */
#define DGraph_FINI()
#endif//$XCC__h

#if XCC__C
static void
DGraph_TEST(void)
{
#include "CAL/DGraph_.c"
}

int
main(int argc, char **argv)
{
  CAL_Test(DGraph_TEST);
  return 0;
}
#endif

