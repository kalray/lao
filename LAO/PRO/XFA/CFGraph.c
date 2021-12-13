#ifdef $XCC_h
/*
 * !!!!	CFGraph.xcc
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
 * @ingroup XFA
 * @brief Control-Flow Graph (to be replaced).
 */
#endif//$XCC_h


#include "XFA/XFA_.h"

#ifdef $XCC__h
#include "XFA/CFGraph.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Arc of the control-flow graph.
 */
struct CFGArc_;
  //@access TAIL	(CFGNode)DGraphArc_TAILNODE(this)
  //@access HEAD	(CFGNode)DGraphArc_HEADNODE(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Node of the control-flow graph.
 */
struct CFGNode_;
  //@access INDEX	DGraphNode_INDEX(this)
  //@access NUMBER	DGraphNode_NUMBER(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Enumerates the CFGArc flags.
 */
typedef enum {
  CFGArcFlag_Fall = 0x1,		// CFGArc is fall-through.
} enum_CFGArcFlag;
typedef uint8_t CFGArcFlags;
#endif//$XCC_h

#ifdef $XCC_h
struct CFGArc_ {
  //@args	float probability, CFGArcFlags flags
  float PROBABILITY;		// Probability to take this CFGArc to successor.
  int32_t ARGINDEX;		// Index for use by PHI arguments.
  int32_t RESINDEX;		// Index for use by SIGMA results.
  int16_t ELAPSED;		// Cycles elapsed along this CFGArc.
  CFGArcFlags FLAGS;	// This CFGArc flags.
  //@access isFall	((CFGArc_FLAGS(this) & CFGArcFlag_Fall) != 0)
};
#endif//$XCC_h

CFGArc
CFGArc_Ctor(CFGArc this, float probability, CFGArcFlags flags)
{
  *CFGArc__PROBABILITY(this) = probability;
  *CFGArc__ARGINDEX(this) = -1;
  *CFGArc__RESINDEX(this) = -1;
  *CFGArc__ELAPSED(this) = -1;
  *CFGArc__FLAGS(this) = flags;
  return this;
}

size_t
CFGArc_Size(float probability, CFGArcFlags flags)
{
  return sizeof(CFGArc_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(CFGArc_)\t%zu\n", sizeof(CFGArc_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This CFGArc probability.
 */
static inline float
CFGArc_probability(CFGArc this)
{
  return CFGArc_PROBABILITY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this CFGArc probability.
 */
static inline void
CFGArc_setProbability(CFGArc this, float probability)
{
  *CFGArc__PROBABILITY(this) = probability;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Attach this CFGArc to a tail CFGNode.
 */
void
CFGArc_attachTailNode(CFGArc this, CFGNode node);
#define CFGArc_attachTailNode(this, node) \
  DGraphArc_attachTailNode((DGraphArc)(this), (DGraphNode)(node))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Attach this CFGArc to a head CFGNode.
 */
void
CFGArc_attachHeadNode(CFGArc this, CFGNode node);
#define CFGArc_attachHeadNode(this, node) \
  DGraphArc_attachHeadNode((DGraphArc)(this), (DGraphNode)(node))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Detach this CFGArc from its tail CFGNode.
 */
void
CFGArc_detachTailNode(CFGArc this);
#define CFGArc_detachTailNode(this) \
  DGraphArc_detachTailNode((DGraphArc)(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Detach this CFGArc from its head CFGNode.
 */
void
CFGArc_detachHeadNode(CFGArc this);
#define CFGArc_detachHeadNode(this) \
  DGraphArc_detachHeadNode((DGraphArc)(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the flags of the arc.
 */
static inline void
CFGArc_setFlags(CFGArc this, CFGArcFlags flags)
{
    *CFGArc__FLAGS(this) = flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Add the following flags to the arc. (union)
 */
static inline void
CFGArc_addFlags(CFGArc this, CFGArcFlags flags)
{
    *CFGArc__FLAGS(this) |= flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Clear the following flags from the arc. (difference)
 */
static inline void
CFGArc_clearFlags(CFGArc this, CFGArcFlags flags)
{
  *CFGArc__FLAGS(this) &= ~flags;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the CFGNode flags.
 */
typedef enum {
  CFGNodeFlag_Stop = 0x1,		// CFGNode is a superblock stop node.
  CFGNodeFlag_Grow = 0x2,		// CFGNode is a superblock grow node.
  CFGNodeFlag_Start = 0x4,		// CFGNode must start a superblock.
  CFGNodeFlag_Widen = 0x8,		// CFGNode is a widening point.
  CFGNodeFlag_Inner = 0x10		// CFGNode is a inner loop header.
} enum_CFGNodeFlag;
typedef uint8_t CFGNodeFlags;
#endif//$XCC_h

#ifdef $XCC_h
struct CFGNode_ {
  //@args	Memory memory, BasicBlock basicBlock
  BasicBlock BASICBLOCK;	// BasicBlock associated to this CFGNode.
  //@access LIVEINSET	BasicBlock_liveInSet(CFGNode_BASICBLOCK(this))
  //@access LIVEOUTSET	BasicBlock_liveOutSet(CFGNode_BASICBLOCK(this))
  CFGNode PREVNODE;		//<! Previous node in the BlockTrace.
  CFGNode NEXTNODE;		//<! Next node in the BlockTrace.
  int16_t NUMBERING;		//<! Node numbering in the BlockTrace.
  CFGNodeFlags FLAGS;	// Flags for this CFGNode.
    //@access isStop	((CFGNode_FLAGS(this) & CFGNodeFlag_Stop) != 0)
    //@access isGrow	((CFGNode_FLAGS(this) & CFGNodeFlag_Grow) != 0)
    //@access isStart	((CFGNode_FLAGS(this) & CFGNodeFlag_Start) != 0)
    //@access isWiden	((CFGNode_FLAGS(this) & CFGNodeFlag_Widen) != 0)
    //@access isInner	((CFGNode_FLAGS(this) & CFGNodeFlag_Inner) != 0)
};
#endif//$XCC_h

CFGNode
CFGNode_Ctor(CFGNode this,
             Memory memory, BasicBlock basicBlock)
{
  *CFGNode__BASICBLOCK(this) = basicBlock;
  *CFGNode__PREVNODE(this) = NULL;
  *CFGNode__NEXTNODE(this) = NULL;
  *CFGNode__NUMBERING(this) = 0;
  *CFGNode__FLAGS(this) = 0;
  return this;
}

size_t
CFGNode_Size(Memory memory, BasicBlock basicBlock)
{
  return sizeof(CFGNode_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(CFGNode_)\t%zu\n", sizeof(CFGNode_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This CFGNode index in the CFGraph.
 */
static inline uint32_t
CFGNode_index(CFGNode this)
{
  return CFGNode_INDEX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This CFGNode number in the CFGraph.
 */
static inline uint32_t
CFGNode_number(CFGNode this)
{
  return CFGNode_NUMBER(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This CFGNode BasicBlock.
 */
static inline BasicBlock
CFGNode_basicBlock(CFGNode this)
{
  return CFGNode_BASICBLOCK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The live-in for this CFGNode.
 */
static inline TemporarySet
CFGNode_liveInSet(CFGNode this)
{
  return CFGNode_LIVEINSET(this);
}
#define CFGNode_isLiveInSet(this, temporary) \
  TemporarySet_contains(CFGNode_liveInSet(this), temporary)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The live-out for this CFGNode.
 */
static inline TemporarySet
CFGNode_liveOutSet(CFGNode this)
{
  return CFGNode_LIVEOUTSET(this);
}
#define CFGNode_isLiveOutSet(this, temporary) \
  TemporarySet_contains(CFGNode_liveOutSet(this), temporary)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The next CFGNode in the BlockTrace.
 */
static inline CFGNode
CFGNode_nextNode(CFGNode this)
{
  return CFGNode_NEXTNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The previous CFGNode in the BlockTrace.
 */
static inline CFGNode
CFGNode_prevNode(CFGNode this)
{
  return CFGNode_PREVNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This CFGNode numbering in the BlockTrace.
 */
static inline int
CFGNode_numbering(CFGNode this)
{
  return CFGNode_NUMBERING(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the entering CFGArcs.
 */
#define CFGNode_FORENTER_CFGArc(this, arc) \
  DGraphNode_FORENTER(this, CFGArc_, arc)
#define CFGNode_ENDENTER_CFGArc \
  DGraphNode_ENDENTER;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the leaving CFGArcs.
 */
#define CFGNode_FORLEAVE_CFGArc(this, arc) \
  DGraphNode_FORLEAVE(this, CFGArc_, arc)
#define CFGNode_ENDLEAVE_CFGArc \
  DGraphNode_ENDLEAVE;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the liveInSet Temporaries.
 */
#define CFGNode_liveInSet_FORSORT_Temporary(this, temporary) \
  TemporarySet_FORSORT_Temporary(CFGNode_liveInSet(this), Temporary_compare, temporary)
#define CFGNode_liveInSet_ENDSORT_Temporary \
  TemporarySet_ENDSORT_Temporary;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the liveOutSet Temporaries.
 */
#define CFGNode_liveOutSet_FORSORT_Temporary(this, temporary) \
  TemporarySet_FORSORT_Temporary(CFGNode_liveOutSet(this), Temporary_compare, temporary)
#define CFGNode_liveOutSet_ENDSORT_Temporary \
  TemporarySet_ENDSORT_Temporary;
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Check this CFGNode.
 */
bool
CFGNode_check(const_CFGNode this);
#endif//$XCC_h

bool
CFGNode_check(const_CFGNode this)
{
  int leaveCount = 0;
  CFGNode prev_head = NULL;
  float probability = 0.0;
  CFGNode_FORLEAVE_CFGArc(this, arc) {
    CFGNode head = CFGArc_HEAD(arc);
    probability += CFGArc_PROBABILITY(arc);
    //FIXME! Except_ALWAYS(probability <= 1.001);
    Except_ALWAYS(probability >= 0);
    if (++leaveCount > 2) continue;
    Except_ALWAYS(head != prev_head);
    prev_head = head;
  } CFGNode_ENDLEAVE_CFGArc;
  return true;
}

#ifdef $XCC_h
/**
 * Pretty-print this CFGNode.
 */
bool
CFGNode_pretty(CFGNode this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
CFGNode_pretty(CFGNode this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "_%d", CFGNode_INDEX(this));
  fprintf(file, "\n\tpredecessors:\t");
  CFGNode_FORENTER_CFGArc(this, arc) {
    CFGNode tail_node = CFGArc_TAIL(arc);
    if (tail_node != NULL) {
      BasicBlock tail_block = CFGNode_basicBlock(tail_node);
      if (tail_block != NULL) {
        fprintf(file, "%d:Block_%d(%d)\t",
            BasicBlock_identity(tail_block), CFGNode_INDEX(tail_node),
            CFGArc_ELAPSED(arc));
      } else {
        fprintf(file, "node_%d\t", CFGNode_INDEX(tail_node));
      }
    } else {
      fprintf(file, "(NULL)\t");
    }
  } CFGNode_ENDENTER_CFGArc;
  fprintf(file, "\n\tsuccessors:\t");
  CFGNode_FORLEAVE_CFGArc(this, arc) {
    CFGNode head_node = CFGArc_HEAD(arc);
    if (head_node != NULL) {
      BasicBlock head_block = CFGNode_basicBlock(head_node);
      if (head_block != NULL) {
        fprintf(file, "%d:Block_%d(%2.0f%%)\t",
            BasicBlock_identity(head_block), CFGNode_INDEX(head_node),
            100.0*CFGArc_PROBABILITY(arc));
      } else {
        fprintf(file, "node_%d(%2.0f%%)\t",
            CFGNode_INDEX(head_node), 100.0*CFGArc_PROBABILITY(arc));
      }
    } else {
      fprintf(file, "(NULL)(%2.0f%%)\t", 100.0*CFGArc_PROBABILITY(arc));
    }
  } CFGNode_ENDLEAVE_CFGArc;
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Tests if that CFGNode is a predecessor of this CFGNode.
 */
bool
CFGNode_isPredecessor(CFGNode this, CFGNode that);
#endif//$XCC_h

bool
CFGNode_isPredecessor(CFGNode this, CFGNode that)
{
  CFGNode_FORENTER_CFGArc(this, arc) {
    if (CFGArc_TAIL(arc) == that) return true;
  } CFGNode_ENDENTER_CFGArc;
  return false;
}

#ifdef $XCC_h
/**
 * Tests if that CFGNode is a successor of this CFGNode.
 */
bool
CFGNode_isSuccessor(CFGNode this, CFGNode that);
#endif//$XCC_h

bool
CFGNode_isSuccessor(CFGNode this, CFGNode that)
{
  CFGNode_FORLEAVE_CFGArc(this, arc) {
    if (CFGArc_HEAD(arc) == that) return true;
  } CFGNode_ENDLEAVE_CFGArc;
  return false;
}

#ifdef $XCC_h
/**
 * Tests if that CFGNode is a successor of this CFGNode.
 */
bool
CFGNode_hasSuccessor(CFGNode this, CFGNode that);
#endif//$XCC_h

bool
CFGNode_hasSuccessor(CFGNode this, CFGNode that)
{
  CFGNode_FORENTER_CFGArc(this, arc) {
    if (CFGArc_TAIL(arc) == that) return true;
  } CFGNode_ENDENTER_CFGArc;
  return false;
}

#ifdef $XCC_h
/**
 * Get the only predecessor of this CFGNode.
 */
CFGNode
CFGNode_getOnlyPredecessor(CFGNode this);
#endif//$XCC_h

CFGNode
CFGNode_getOnlyPredecessor(CFGNode this)
{
  int enterCount = 0;
  CFGNode that = NULL;
  CFGNode_FORENTER_CFGArc(this, arc) {
    that = CFGArc_TAIL(arc);
    if (++enterCount > 1) return NULL;
  } CFGNode_ENDENTER_CFGArc;
  return that;
}

#ifdef $XCC_h
/**
 * Test if that CFGNode is the only predecessor of this CFGNode.
 */
#define CFGNode_isOnlyPredecessor(this, that) \
  (CFGNode_getOnlyPredecessor(this) == that)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the only successor of this CFGNode.
 */
CFGNode
CFGNode_getOnlySuccessor(CFGNode this);
#endif//$XCC_h

CFGNode
CFGNode_getOnlySuccessor(CFGNode this)
{
  int leaveCount = 0;
  CFGNode that = NULL;
  CFGNode_FORLEAVE_CFGArc(this, arc) {
    that = CFGArc_HEAD(arc);
    if (++leaveCount > 1) return NULL;
  } CFGNode_ENDLEAVE_CFGArc;
  return that;
}

#ifdef $XCC_h
/**
 * Test if that CFGNode is the only successor of this CFGNode.
 */
#define CFGNode_isOnlySuccessor(this, that) \
  (CFGNode_getOnlySuccessor(this) == that)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the CFGArc between two CFGNodes.
 * position:	The position of the CFGArc (0 means first arc).
 */
CFGArc
CFGNode_getCFGArc(CFGNode this, CFGNode that, int position);
#endif//$XCC_h

CFGArc
CFGNode_getCFGArc(CFGNode this, CFGNode that, int position)
{
  CFGNode_FORLEAVE_CFGArc(this, arc) {
    if (CFGArc_HEAD(arc) == that && position-- == 0) return arc;
  } CFGNode_ENDLEAVE_CFGArc;
  return NULL;
}

#ifdef $XCC_h
/*
 * CFGNode_getGoToOperation - Get the GOTO operation of this CFGNode.
 */
Operation
CFGNode_getGoToOperation(CFGNode this);
#endif//$XCC_h

Operation
CFGNode_getGoToOperation(CFGNode this)
{
  BasicBlock basicBlock = CFGNode_basicBlock(this);
  Operation last_operation = BasicBlock_lastOperation(basicBlock);
  if (last_operation != NULL && Operation_isGoTo(last_operation)) return last_operation;
  return NULL;
}

#ifdef $XCC_h
/**
 * Test if this CFGNode has a GOTO operation.
 */
#define CFGNode_hasGoToOperation(this) \
  (CFGNode_getGoToOperation(this) != NULL)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Invert this CFGNode conditional GOTO operation.
 */
bool
CFGNode_invertGoToOperation(CFGNode this);
#endif//$XCC_h

bool
CFGNode_invertGoToOperation(CFGNode this)
{
  BasicBlock basicBlock = CFGNode_basicBlock(this);
  Procedure procedure = BasicBlock_procedure(basicBlock);
  Processor processor = BasicBlock_processor(basicBlock);
  Operation goto_operation = CFGNode_getGoToOperation(this);
  if (goto_operation != NULL) {
    if (   Operation_isConditional(goto_operation)
        && Operation_tryInvertGoTo(goto_operation, processor)) {
      CFGArc fall_arc = CFGNode_getFallArc(this);
      CFGNode fall_node = CFGArc_HEAD(fall_arc);
      BasicBlock fall_block = CFGNode_basicBlock(fall_node);
      Label label = BasicBlock_makeLabel(fall_block);
      Procedure_updateTargetedOperation(procedure, goto_operation, label);
      CFGNode_FORLEAVE_CFGArc(this, arc) {
        *CFGArc__FLAGS(arc) ^= CFGArcFlag_Fall;
      } CFGNode_ENDLEAVE_CFGArc
      return true;
    }
  }
  return false;
}

#ifdef $XCC_h
/**
 * Discard this CFGNode unconditional GOTO operation.
 */
bool
CFGNode_discardGoToOperation(CFGNode this);
#endif//$XCC_h

bool
CFGNode_discardGoToOperation(CFGNode this)
{
  BasicBlock basicBlock = CFGNode_basicBlock(this);
  Operation last_operation = BasicBlock_lastOperation(basicBlock);
  if (last_operation != NULL && Operation_isGoTo(last_operation)) {
    BasicBlock_detachOperation(basicBlock, last_operation);
    CFGNode_FORLEAVE_CFGArc(this, arc) {
      *CFGArc__FLAGS(arc) |= CFGArcFlag_Fall;
    } CFGNode_ENDLEAVE_CFGArc;
    return true;
  }
  return false;
}

#ifdef $XCC_h
/*!
 * CFGNode_compare --	Compare function for CFGNode(s).
 */
int
CFGNode_compare(CFGNode this, CFGNode that);
#endif//$XCC_h

int
CFGNode_compare(CFGNode this, CFGNode that) {
  uint32_t this_index = CFGNode_INDEX(this);
  uint32_t that_index = CFGNode_INDEX(that);
  return (this_index > that_index) - (this_index < that_index);
}

#ifdef $XCC_h
/**
 * Get the target arc of this CFGNode.
 */
CFGArc
CFGNode_getTargetArc(CFGNode this);
#endif//$XCC_h

CFGArc
CFGNode_getTargetArc(CFGNode this)
{
  BasicBlock basicBlock = CFGNode_basicBlock(this);
  Operation operation = BasicBlock_lastOperation(basicBlock);
  if (operation != NULL) {
    Label label = Operation_getTargetLabel(operation);
    if (label != NULL) {
      CFGNode_FORLEAVE_CFGArc(this, arc) {
        CFGNode head_node = CFGArc_HEAD(arc);
        BasicBlock head_block = CFGNode_basicBlock(head_node);
        if (BasicBlock_hasLabel(head_block, label)) return arc;
      } CFGNode_ENDLEAVE_CFGArc;
    }
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Get the fall-thru arc of this CFGNode.
 *
 * Relies on CFGArc_isFall that must be properly set.
 */
CFGArc
CFGNode_getFallArc(const_CFGNode this);
#endif//$XCC_h

CFGArc
CFGNode_getFallArc(const_CFGNode this)
{
  int leaveCount = 0;
  CFGNode_FORLEAVE_CFGArc(this, arc) {
    if (CFGArc_isFall(arc)) return arc;
    if (++leaveCount > 2) return NULL;
  } CFGNode_ENDLEAVE_CFGArc;
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Set of CFGNode(s).
 */
struct CFGNodeSet_ {
  //@args	Memory memory, int32_t estimate
  PtrSet_ PTRSET_;		// The PtrSet_ that implements the set.
  //@access PTRSET	CFGNodeSet__PTRSET_(this)
  //@access MEMORY	PtrSet_memory(CFGNodeSet_PTRSET(this))
};
#endif//$XCC_h

CFGNodeSet
CFGNodeSet_Ctor(CFGNodeSet this,
                Memory memory, int32_t estimate)
{
  PtrSet_Ctor(CFGNodeSet_PTRSET(this), memory, estimate);
  return this;
}

void
CFGNodeSet_Dtor(CFGNodeSet this)
{
  PtrSet_Dtor(CFGNodeSet_PTRSET(this));
}

size_t
CFGNodeSet_Size(Memory memory, int32_t estimate)
{
  return sizeof(CFGNodeSet_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(CFGNodeSet_)\t%zu\n", sizeof(CFGNodeSet_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * For use in CFGNodeSet_FORSORT_CFGNode.
 */
static inline PtrSet
CFGNodeSet_ptrset(CFGNodeSet this)
{
  return CFGNodeSet_PTRSET(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this CFGNodeSet CFGNode(s).
 */
#define CFGNodeSet_FOREACH_CFGNode(this, cfgNode) { \
  PtrSet CFGNodeSet_PTRSET = CFGNodeSet_ptrset(this); \
  PtrSet_FOREACH(CFGNodeSet_PTRSET, CFGNode, cfgNode) {
#define CFGNodeSet_ENDEACH_CFGNode \
  } PtrSet_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this CFGNodeSet sorted CFGNode(s).
 */
#define CFGNodeSet_FORSORT_CFGNode(this, cfgNode) { \
  PtrSet CFGNodeSet_PTRSET = CFGNodeSet_ptrset(this); \
  PtrSet_FORSORT(CFGNodeSet_PTRSET, CFGNode_compare, CFGNode, cfgNode) {
#define CFGNodeSet_ENDSORT_CFGNode \
  } PtrSet_ENDSORT \
}
#endif//$XCC_h

#ifdef $XCC_h
#define CFGNodeSet_isEmpty(this)	PtrSet_isEmpty(CFGNodeSet_ptrset(this))
#define CFGNodeSet_isSingle(this)	PtrSet_isSingle(CFGNodeSet_ptrset(this))
#define CFGNodeSet_choose(this)	PtrSet_choose(CFGNodeSet_ptrset(this))
#define CFGNodeSet_count(this)	PtrSet_count(CFGNodeSet_ptrset(this))
#define CFGNodeSet_empty(this)	PtrSet_empty(CFGNodeSet_ptrset(this))
#define CFGNodeSet_contains(this, cfgNode)	\
  PtrSet_contains(CFGNodeSet_ptrset(this), (PtrSetMember)cfgNode)
#define CFGNodeSet_insert(this, cfgNode)	\
  PtrSet_insert(CFGNodeSet_ptrset(this), (PtrSetMember)cfgNode)
#define CFGNodeSet_remove(this, cfgNode)	\
  PtrSet_remove(CFGNodeSet_ptrset(this), (PtrSetMember)cfgNode)
#define CFGNodeSet_equals(this, that)	\
  PtrSet_equals(CFGNodeSet_ptrset(this), CFGNodeSet_ptrset(that))
#define CFGNodeSet_assign(this, that)	\
  PtrSet_assign(CFGNodeSet_ptrset(this), CFGNodeSet_ptrset(that))
#define CFGNodeSet_union(this, that)	\
  PtrSet_union(CFGNodeSet_ptrset(this), CFGNodeSet_ptrset(that))
#define CFGNodeSet_inter(this, that)	\
  PtrSet_inter(CFGNodeSet_ptrset(this), CFGNodeSet_ptrset(that))
#define CFGNodeSet_diff(this, that)	\
  PtrSet_diff(CFGNodeSet_ptrset(this), CFGNodeSet_ptrset(that))
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Control-flow graph.
 *
 * The CFGraph always contains a ROOT CFGNode, whose associated basic block is NULL.
 * This ROOT CFGNode is connected to the Control-flow graph entries. Likewise, the
 * CFGraph always contains a SINK CFGNode, whose associated basic block is NULL.
 * The Control-flow exits are connected to this SINK CFGNode.
 */
struct CFGraph_ {
  //@args	Memory memory, int32_t maxNodes
  //@access MEMORY	DGraph_memory(CFGraph_DGRAPH(this))
  //@access NODECOUNT	DGraph_count(CFGraph_DGRAPH(this))
  DGraph_ DGRAPH_;		// The base DGraph.
  //@access DGRAPH	CFGraph__DGRAPH_(this)
  //@access NODES	(CFGNode *)DGraph_nodes(CFGraph_DGRAPH(this))
  //@access NODEOF	(CFGraph_NODES(this) - 1)
  //@access ROOTNODE	(CFGNode)DGraph_rootNode(CFGraph_DGRAPH(this))
  CFGNode SINKNODE;
};
#endif//$XCC_h

CFGraph
CFGraph_Ctor(CFGraph this,
             Memory memory, int32_t maxNodes)
{
  DGraph_Ctor(CFGraph_DGRAPH(this), memory, maxNodes);
  CFGraph_makeNode(this, NULL);
  *CFGraph__SINKNODE(this) = NULL;
  return this;
}

void
CFGraph_Dtor(CFGraph this)
{
  DGraph_Dtor(CFGraph_DGRAPH(this));
}

size_t
CFGraph_Size(Memory memory, int32_t maxNodes)
{
  return sizeof(CFGraph_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(CFGraph_)\t%zu\n", sizeof(CFGraph_));
}
#endif//$XCC__c

#ifdef $XCC_h
CFGraph
CFGraph_new(Memory parent, int32_t maxNodes);
#endif//$XCC_h

CFGraph
CFGraph_new(Memory parent, int32_t maxNodes)
{
  Memory memory = Memory_new(parent, true);
  size_t size = CFGraph_Size(memory, maxNodes);
  CFGraph this = Memory_alloc(memory, size);
  return CFGraph_Ctor(this, memory, maxNodes);
}

#ifdef $XCC_h
CFGraph
CFGraph_delete(CFGraph this);
#endif//$XCC_h

CFGraph
CFGraph_delete(CFGraph this)
{
  if (this != NULL) {
    Memory memory = CFGraph_MEMORY(this);
#ifndef _NDTOR
    CFGraph_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * This CFGraph DGraph.
 */
static inline DGraph
CFGraph_dgraph(CFGraph this)
{
  return CFGraph_DGRAPH(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This CFGraph Memory.
 */
static inline Memory
CFGraph_memory(CFGraph this)
{
  return CFGraph_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The start CFGNode of this CFGraph.
 */
static inline CFGNode
CFGraph_rootNode(CFGraph this)
{
  return CFGraph_ROOTNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Return the stop CFGNode of this CFGraph.
 */
static inline CFGNode
CFGraph_sinkNode(CFGraph this)
{
  return CFGraph_SINKNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this CFGraph CFGNodes.
 */
#define CFGraph_FOREACH_CFGNode(this, node) { \
  DGraph CFGraph_DGRAPH = CFGraph_dgraph(this); \
  DGraph_FOREACH(CFGraph_DGRAPH, CFGNode_, node)
#define CFGraph_ENDEACH_CFGNode \
  DGraph_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a CFGArc in this CFGraph.
 */
CFGArc
CFGraph_makeArc(CFGraph this, CFGNode tail_node, CFGNode head_node,
                float probability, CFGArcFlags flags);
#endif//$XCC_h

CFGArc
CFGraph_makeArc(CFGraph this, CFGNode tail_node, CFGNode head_node,
                float probability, CFGArcFlags flags)
{
  size_t size = CFGArc_Size(probability, flags);
  CFGArc arc = DGraph_makeArc(CFGraph_DGRAPH(this), tail_node, head_node, size);
  Except_REQUIRE(probability >= 0.0 && probability <= 1.0);
  CFGArc_Ctor(arc, probability, flags);
  Except_CHECK(CFGNode_check(tail_node));
  return arc;
}

#ifdef $XCC_h
/**
 * Make a CFGNode in this CFGraph.
 */
CFGNode
CFGraph_makeNode(CFGraph this, BasicBlock basicBlock);
#endif//$XCC_h

CFGNode
CFGraph_makeNode(CFGraph this, BasicBlock basicBlock)
{
  Memory memory = CFGraph_MEMORY(this);
  size_t size = CFGNode_Size(memory, basicBlock);
  CFGNode node = DGraph_makeNode(CFGraph_DGRAPH(this), size);
  CFGNode_Ctor(node, memory, basicBlock);
  if (basicBlock != NULL) BasicBlock_setCFGNode(basicBlock, node);
  return node;
}

#ifdef $XCC_h
/**
 * Complete the CFGraph with the SINKNODE.
 */
CFGNode
CFGraph_complete(CFGraph this);
#endif//$XCC_h

CFGNode
CFGraph_complete(CFGraph this)
{
  CFGArcFlags flags = 0;
  CFGNode rootNode = CFGraph_ROOTNODE(this);
  CFGNode sinkNode = CFGraph_makeNode(this, NULL);
  CFGraph_FOREACH_CFGNode(this, cfgNode) {
    if (cfgNode != rootNode && cfgNode != sinkNode) {
      if (!DGraphNode_ENTERARC(cfgNode)) {
        CFGraph_makeArc(this, rootNode, cfgNode, 0.0, flags);
      }
      if (!DGraphNode_LEAVEARC(cfgNode)) {
        CFGraph_makeArc(this, cfgNode, sinkNode, 0.0, flags);
      }
    }
  } CFGraph_ENDEACH_CFGNode;
  *CFGraph__SINKNODE(this) = sinkNode;
  return sinkNode;
}

#ifdef $XCC_h
/**
 * Sort this CFGraph leaving arcs.
 *
 * The leaving CFGArcs are sorted using CFGArc_compare, thus ensuring
 * that the highest frequency arc appears first in the leaving arc list.
 * It there are more than two leaving arcs the order is unchanged.
 */
CFGraph
CFGraph_sortLeaveArcs(CFGraph this);
#endif//$XCC_h

CFGraph
CFGraph_sortLeaveArcs(CFGraph this)
{
  CFGraph_FOREACH_CFGNode(this, node) {
    CFGArc arc_1 = DGraphNode_LEAVEARC(node);
    if (arc_1 != NULL) {
      CFGArc arc_2 = DGraphArc_LEAVEARC(arc_1);
      if (arc_2 != NULL) {
        CFGArc arc_3 = DGraphArc_LEAVEARC(arc_2);
        if (arc_3 == NULL) {
          // For non-negative 32-bit IEEE, integer compare works.
          int32_t probability_1 = *(int32_t *)CFGArc__PROBABILITY(arc_1);
          int32_t probability_2 = *(int32_t *)CFGArc__PROBABILITY(arc_2);
          if (probability_1 < probability_2) {
            *DGraphNode__LEAVEARC(node) = arc_2;
            *DGraphArc__LEAVEARC(arc_2) = arc_1;
            *DGraphArc__LEAVEARC(arc_1) = NULL;
          }
        }
      }
    }
  } CFGraph_ENDEACH_CFGNode;
  return this;
}

#ifdef $XCC_h
/**
 * Pretty-print this CFGraph in VCG format.
 */
bool
CFGraph_prettyVCG(CFGraph this, FILE *file, const char *name);
#endif//$XCC_h

#ifndef _NTRACE
bool
CFGraph_prettyVCG(CFGraph this, FILE *file, const char *name)
{
  if (file == NULL) file = stderr;
  //
  fprintf(file, "graph: {\n");
  fprintf(file, "title: \"%s\"\n", name);
  fprintf(file, "shape: box\n");
  fprintf(file, "color: white\n");
  // Dump all the nodes
  CFGraph_FOREACH_CFGNode(this, node) {
    BasicBlock block = CFGNode_BASICBLOCK(node);
    fprintf(file, "node: { title: \"%d\" label: \"BB%d\n ",
            CFGNode_INDEX(node),CFGNode_INDEX(node));
    if (block != NULL) {
      // Dump all the instructions into the block
      BasicBlock_pretty(block, file);
    }
    fprintf(file, "\"}\n");
    // Now dump all the outgoing edges
    CFGNode_FORLEAVE_CFGArc(node, arc) {
      if (CFGArc_isFall(arc)) {
        fprintf(file, "edge: { sourcename: \"%d\" targetname: \"%d\" color: blue}\n",
                CFGNode_INDEX(CFGArc_TAIL(arc)),CFGNode_INDEX(CFGArc_HEAD(arc)));
      }
      else {
         fprintf(file, "edge: { sourcename: \"%d\" targetname: \"%d\" }\n",
                CFGNode_INDEX(CFGArc_TAIL(arc)),CFGNode_INDEX(CFGArc_HEAD(arc)));
      }
    } CFGNode_ENDLEAVE_CFGArc;
  } CFGraph_ENDEACH_CFGNode;
  fprintf(file, "}\n");
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the CFGraph module.
 */
#define CFGraph_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the CFGraph module.
 */
#define CFGraph_FINI()
#endif//$XCC__h

#if XCC__C
static void
CFGraph_TEST(void)
{
#include "XFA/CFGraph_.c"
}

int
main(int argc, char **argv)
{
  XFA_Test(CFGraph_TEST);
  return 0;
}
#endif

