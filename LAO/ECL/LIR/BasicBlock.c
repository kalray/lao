#ifdef $XCC_h
/*
 * !!!!	BasicBlock.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu).
 * Duco van Amstel (duco.van-amstel@kalray.eu)
 *
 * Copyright 2010 - 2013 Kalray
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
 * @ingroup LIR
 * @brief Basic Block implementation.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/BasicBlock.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * IndexedFactory for BasicBlock(s).
 */
extern IndexedFactory_ BasicBlock_Factory[1];
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerate BasicBlockEdge flags.
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Control flow edge between two BasicBlock(s).
 */
struct BasicBlockEdge_ {
  //@args	struct BasicBlock_ *origBlock, struct BasicBlock_ *destBlock, float probability
  struct BasicBlock_ *ORIGBLOCK;	//!< Origin BasicBlock of this BasicBlockEdge.
  struct BasicBlock_ *DESTBLOCK;	//!< Destination BasicBlock of this BasicBlockEdge.
  struct BasicBlockEdge_ *ENTERNEXT;	//!< Next BasicBlockEdge in destBlock ENTERFIRST.
  float PROBABILITY;			//!< Probability to execute this BasicBlockEdge.
  int32_t INDEX;			//!< Index of this BasicBlockEdge (user-defined).
  bool ISLOOP;				//!< This BasicBlockEdge is Looping (LoopForest).
  bool ISJOIN;				//!< This BasicBlockEdge is Joining (Dominance).
  bool ISBACK;				//!< This BasicBlockEdge is back (CFG DFS).
  int8_t ELAPSED;			//!< Cycles elapsed along this BasicBlockEdge.
  int32_t ARGINDEX;			//!< Index for use by PHI arguments.
  int32_t RESINDEX;			//!< Index for use by SIGMA results.
};
#endif//$XCC_h

BasicBlockEdge
BasicBlockEdge_Ctor(BasicBlockEdge this,
                    struct BasicBlock_ *origBlock, struct BasicBlock_ *destBlock, float probability)
{
  *BasicBlockEdge__ORIGBLOCK(this) = origBlock;
  *BasicBlockEdge__DESTBLOCK(this) = destBlock;
  *BasicBlockEdge__ENTERNEXT(this) = NULL;
  *BasicBlockEdge__PROBABILITY(this) = probability;
  *BasicBlockEdge__INDEX(this) = -1;
  *BasicBlockEdge__ISLOOP(this) = false;
  *BasicBlockEdge__ISJOIN(this) = false;
  *BasicBlockEdge__ISBACK(this) = false;
  *BasicBlockEdge__ELAPSED(this) = -1;
  *BasicBlockEdge__ARGINDEX(this) = -1;
  *BasicBlockEdge__RESINDEX(this) = -1;
  return this;
}

size_t
BasicBlockEdge_Size(struct BasicBlock_ *origBlock, struct BasicBlock_ *destBlock, float probability)
{
  return sizeof(BasicBlockEdge_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(BasicBlockEdge_)\t%zu\n", sizeof(BasicBlockEdge_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Edge comparison. 
 *
 * This is used for sorting edges and is currently based on the @c INDEX of the
 * origin blocks.
 * @return true if edge @p this is greater that edge @p that.
 */
static inline bool
BasicBlockEdge_gt(const_BasicBlockEdge this, const_BasicBlockEdge that)
{
  BasicBlock this_block = BasicBlockEdge_ORIGBLOCK(this);
  BasicBlock that_block = BasicBlockEdge_ORIGBLOCK(that);
  int32_t this_index = BasicBlock_INDEX(this_block);
  int32_t that_index = BasicBlock_INDEX(that_block);
  return this_index > that_index;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * BasicBlock this BasicBlockEdge leaves.
 */
static inline struct BasicBlock_*
BasicBlockEdge_origBlock(BasicBlockEdge this)
{
  return BasicBlockEdge_ORIGBLOCK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * BasicBlock this BasicBlockEdge enters.
 */
static inline struct BasicBlock_*
BasicBlockEdge_destBlock(BasicBlockEdge this)
{
  return BasicBlockEdge_DESTBLOCK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Successor probability
 */
static inline float
BasicBlockEdge_probability(BasicBlockEdge this)
{
  return BasicBlockEdge_PROBABILITY(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the BasicBlock flags.
 */
typedef enum {
  BasicBlockFlag_Entry = 0x1,		//!< BasicBlock is Procedure entry.
  BasicBlockFlag_Exit = 0x2,		//!< BasicBlock is Procedure exit.
  BasicBlockFlag_Start = 0x4,		//!< BasicBlock is CodeTrace start.
  BasicBlockFlag_GNUASM = 0x8,		//!< BasicBlock has GNU ASM operation
  BasicBlockFlag_Natural = 0x10,	//!< BasicBlock is a natural loop header.
  BasicBlockFlag_Widening = 0x20,	//!< BasicBlock is a forward widening point.
  BasicBlockFlag_Scheduled = 0x40,	//!< BasicBlock is instruction scheduled.
  BasicBlockFlag_Allocated = 0x80,	//!< BasicBlock is register allocated.
} enum_BasicBlockFlag;
typedef uint8_t BasicBlockFlags;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ BasicBlockFlag_Table[1];
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ BasicBlockFlagEntries[] = {
  ETableENTRY(BasicBlockFlag, Entry),
  ETableENTRY(BasicBlockFlag, Exit),
  ETableENTRY(BasicBlockFlag, Start),
  ETableENTRY(BasicBlockFlag, GNUASM),
  ETableENTRY(BasicBlockFlag, Natural),
  ETableENTRY(BasicBlockFlag, Widening),
  ETableENTRY(BasicBlockFlag, Scheduled),
  ETableENTRY(BasicBlockFlag, Allocated),
  ETableENTRY_NULL
};
ETable_ BasicBlockFlag_Table[1];
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Union for the variant part of BasicBlock.
 */
union BasicBlockUnion_ {
  void *POINTER;			//!< Client pointer.
  struct BasicBlockEdge_ **ENTEREDGES;	//!< The edges entering this BasicBlock in SSAForm.
  struct BasicBlock_ *CHAINNEXT;	//!< The next BasicBlock in chain in block layout.
  struct NativeBlock_ *NATIVEBLOCK;	//!< The associated NativeBlock.
  struct CFGNode_ *CFGNODE;		//!< @deprecated The CFGNode of this BasicBlock.
};
typedef union BasicBlockUnion_ BasicBlockUnion_, *BasicBlockUnion;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Basic block of Operations.
 *
 * A BasicBlock contains one control-flow operation at most, which must be the 
 * last of the BasicBlock. This operation satisfies Operation_isControl(operation).
 */
struct BasicBlock_ {
  //@args	Memory memory, void *procedure, struct CodeRegion_ *codeRegion, float frequency
  Indexed_ __;				//!< A BasicBlock_ ISA Indexed_.
  //@access IDENTITY	Indexed_IDENTITY(BasicBlock____(this))
  //@access INDEX	Indexed_INDEX(BasicBlock____(this))
  //@mutate INDEX	Indexed__INDEX(BasicBlock____(this))
  Selector_ SELECTOR[1];		//!< Selector of this BasicBlock.
  //@access OPERATIONS	Selector_OPERATIONS(BasicBlock_SELECTOR(this))
  //@access PROCEDURE	Selector_PROCEDURE(BasicBlock_SELECTOR(this))
  //@access PROCESSOR	Selector_PROCESSOR(BasicBlock_SELECTOR(this))
  //@mutate PROCESSOR	Selector__PROCESSOR(BasicBlock_SELECTOR(this))
  //@access CONVENTION	Selector_CONVENTION(BasicBlock_SELECTOR(this))
  //@mutate CONVENTION	Selector__CONVENTION(BasicBlock_SELECTOR(this))
  //@access PHICOUNT	Selector_PHICOUNT(BasicBlock_SELECTOR(this))
  //@access MEMORY	Selector_MEMORY(BasicBlock_SELECTOR(this))
  IDList_ LEAVEEDGES[1];		//!< The edges leaving this BasicBlock. @note the first edge of this list act as fall-through if the BasicBlock has one (e.g., the last operation is a function call, a conditional branch, etc.)
  //@access LEAVECOUNT	IDList_count(BasicBlock_LEAVEEDGES(this))
  //@access LEAVEFIRST	IDList_firstItem(BasicBlock_LEAVEEDGES(this))
  struct BasicBlockEdge_ *ENTERFIRST;	//!< Start of BasicBlockEdge_ENTERNEXT list.
  int32_t ENTERCOUNT;			//!< Count of entering BasicBlockEdge(s).
  int32_t FIRSTORDERING;		//!< ORDERING of Label and Phi Operation(s).
  int32_t EARLYORDERING;		//!< ORDERING of early COPY Operation(s).
  int32_t LATEORDERING;			//!< ORDERING of late COPY Operation(s).
  int32_t LASTORDERING;			//!< ORDERING of Control and Sigma Operation(s).
  struct CodeRegion_ *CODEREGION;	//!< This BasicBlock CodeRegion.
  struct LoopScope_ *LOOPSCOPE;		//!< LoopScope of this BasicBlock.
  int32_t TRACEID;			//!< TraceID also used to sort the BasicBlock(s).
  float FREQUENCY;			//!< Execution frequency of this BasicBlock.
  BasicBlockUnion_ UNION_;
  //@access POINTER	(BasicBlock_UNION_(this).POINTER)
  //@mutate POINTER	(&BasicBlock_UNION_(this).POINTER)
  //@access ENTEREDGES	(BasicBlock_UNION_(this).ENTEREDGES)
  //@mutate ENTEREDGES	(&BasicBlock_UNION_(this).ENTEREDGES)
  //@access CHAINNEXT	(BasicBlock_UNION_(this).CHAINNEXT)
  //@mutate CHAINNEXT	(&BasicBlock_UNION_(this).CHAINNEXT)
  //@access NATIVEBLOCK	(BasicBlock_UNION_(this).NATIVEBLOCK)
  //@mutate NATIVEBLOCK	(&BasicBlock_UNION_(this).NATIVEBLOCK)
  //@access CFGNODE	(BasicBlock_UNION_(this).CFGNODE)
  //@mutate CFGNODE	(&BasicBlock_UNION_(this).CFGNODE)
  uint16_t INSTANCE;			//!< BasicBlock instance after unrolling.
  BasicBlockFlags FLAGS;		//!< This BasicBlock flags.
  //@access isEntry	((BasicBlock_flags(this) & BasicBlockFlag_Entry) != 0)
  //@access isExit	((BasicBlock_flags(this) & BasicBlockFlag_Exit) != 0)
  //@access isStart	((BasicBlock_flags(this) & BasicBlockFlag_Start) != 0)
  //@access hasGNUASM	((BasicBlock_flags(this) & BasicBlockFlag_GNUASM) != 0)
  //@access isNatural	((BasicBlock_flags(this) & BasicBlockFlag_Natural) != 0)
  //@access isWidening	((BasicBlock_flags(this) & BasicBlockFlag_Widening) != 0)
  //@access isScheduled	((BasicBlock_flags(this) & BasicBlockFlag_Scheduled) != 0)
  //@access isAllocated	((BasicBlock_flags(this) & BasicBlockFlag_Allocated) != 0)
  int8_t MARK;				//!< For use by clients, must be cleared after use.
  uint16_t PADDING;			//!< NOP padding at the end of this BasicBlock.
  int16_t LABELCOUNT;			//!< Count of Label(s) for this BasicBlock.
  Label *LABELS;			//!< Label(s) for this BasicBlock.
};
#endif//$XCC_h

#ifdef $XCC_h
static inline Indexed
BasicBlock__(BasicBlock this) {
  return BasicBlock____(this);
}
static inline const_Indexed
BasicBlock__const(const_BasicBlock this) {
  return BasicBlock____(this);
}
#endif//$XCC_h

BasicBlock
BasicBlock_Ctor(BasicBlock this,
                Memory memory, void *procedure, struct CodeRegion_ *codeRegion, float frequency)
{
  Indexed_Ctor(BasicBlock____(this), BasicBlock_Factory);
  Selector_Ctor(BasicBlock_SELECTOR(this), memory, procedure, true);
  IDList_Ctor(BasicBlock_LEAVEEDGES(this), memory);
  *BasicBlock__ENTERFIRST(this) = NULL;
  *BasicBlock__ENTERCOUNT(this) = 0;
  *BasicBlock__FIRSTORDERING(this) = 0;
  *BasicBlock__EARLYORDERING(this) = 0;
  *BasicBlock__LATEORDERING(this) = 0;
  *BasicBlock__LASTORDERING(this) = 0;
  *BasicBlock__CODEREGION(this) = codeRegion;
  *BasicBlock__LOOPSCOPE(this) = NULL;
  *BasicBlock__TRACEID(this) = 0;
  *BasicBlock__FREQUENCY(this) = frequency;
  *BasicBlock__POINTER(this) = NULL;
  *BasicBlock__INSTANCE(this) = 0;
  *BasicBlock__FLAGS(this) = 0;
  *BasicBlock__MARK(this) = 0;
  *BasicBlock__PADDING(this) = 0;
  *BasicBlock__LABELCOUNT(this) = 0;
  *BasicBlock__LABELS(this) = NULL;
  return this;
}

void
BasicBlock_Dtor(BasicBlock this)
{
  Selector_Dtor(BasicBlock_SELECTOR(this));
  IDList_Dtor(BasicBlock_LEAVEEDGES(this));
  if (BasicBlock_LABELCOUNT(this) > 1) {
    Memory memory = BasicBlock_MEMORY(this);
    Memory_free_(memory, BasicBlock_LABELS(this));
  }
}

size_t
BasicBlock_Size(Memory memory, void *procedure, struct CodeRegion_ *codeRegion, float frequency)
{
  return sizeof(BasicBlock_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(BasicBlock_)\t%zu\n", sizeof(BasicBlock_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The next BasicBlock in Procedure.
 */
static inline BasicBlock
BasicBlock_next(const_BasicBlock this)
{
  return (BasicBlock)IDListItem_NEXT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The previous BasicBlock in Procedure.
 */
static inline BasicBlock
BasicBlock_prev(const_BasicBlock this)
{
  return (BasicBlock)IDListItem_PREV(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * the Processor of this BasicBlock.
 */
static inline Processor
BasicBlock_processor(const_BasicBlock this)
{
  return BasicBlock_PROCESSOR(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock count of PHI-Operations.
 */
static inline int
BasicBlock_phiCount(BasicBlock this)
{
  return BasicBlock_PHICOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * the Selector of this BasicBlock.
 */
static inline Selector
BasicBlock_selector(BasicBlock this)
{
  return BasicBlock_SELECTOR(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by BasicBlock_FOREACH_Operation.
 */
static inline const_IDList
BasicBlock_operations(const_BasicBlock this)
{
  return BasicBlock_OPERATIONS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate from first to last operation in this BasicBlock.
 */
#define BasicBlock_FOREACH_Operation(this, operation) \
  IDList_FOREACH(BasicBlock_operations(this), Operation_, operation)
#define BasicBlock_FOREACH_Operation_SYNC IDList_FOREACH_SYNC
#define BasicBlock_ENDEACH_Operation \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate from last to first operation in this BasicBlock.
 */
#define BasicBlock_FORBACK_Operation(this, operation) \
  IDList_FORBACK(BasicBlock_operations(this), Operation_, operation)
#define BasicBlock_FORBACK_Operation_SYNC IDList_FORBACK_SYNC
#define BasicBlock_ENDBACK_Operation \
  IDList_ENDBACK;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by BasicBlock_LEAVE_FOREACH_BasicBlockEdge.
 */
static inline const_IDList
BasicBlock_leaveEdges(const_BasicBlock this)
{
  return BasicBlock_LEAVEEDGES(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate the leaving BasicBlockEdge(s)
 */
#define BasicBlock_LEAVE_FOREACH_BasicBlockEdge(this, edge) \
  IDList_FOREACH(BasicBlock_leaveEdges(this), BasicBlockEdge_, edge)
#define BasicBlock_LEAVE_ENDEACH_BasicBlockEdge \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate from last to first leaving BasicBlockEdge.
 */
#define BasicBlock_LEAVE_FORBACK_BasicBlockEdge(this, edge) \
  IDList_FORBACK(BasicBlock_leaveEdges(this), BasicBlockEdge_, edge)
#define BasicBlock_LEAVE_ENDBACK_BasicBlockEdge \
  IDList_ENDBACK;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of leaveEdges of this BasicBlock.
 */
static inline int32_t
BasicBlock_leaveCount(const_BasicBlock this)
{
  return BasicBlock_LEAVECOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * First leaveEdge of this BasicBlock.
 */
static inline struct BasicBlockEdge_ *
BasicBlock_leaveFirst(const_BasicBlock this)
{
  return BasicBlock_LEAVEFIRST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of enterEdges of this BasicBlock.
 */
static inline int32_t
BasicBlock_enterCount(const_BasicBlock this)
{
  return BasicBlock_ENTERCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock ENTEREDGES.
 */
static inline const BasicBlockEdge *
BasicBlock_enterEdges(const_BasicBlock this)
{
  return BasicBlock_ENTEREDGES(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by BasicBlock_ENTER_FOREACH_BasicBlockEdge
 */
static inline struct BasicBlockEdge_ *
BasicBlock_enterFirst(const_BasicBlock this)
{
  return BasicBlock_ENTERFIRST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate the entering BasicBlockEdge(s)
 */
#define BasicBlock_ENTER_FOREACH_BasicBlockEdge(this, edge) { \
  struct BasicBlockEdge_ *BasicBlock_ENTERNEXT = NULL; \
  struct BasicBlockEdge_ *edge = BasicBlock_enterFirst(this); \
  for (; edge != NULL; edge = BasicBlock_ENTERNEXT) { \
    BasicBlock_ENTERNEXT = BasicBlockEdge_ENTERNEXT(edge);
#define BasicBlock_ENTER_ENDEACH_BasicBlockEdge \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Identity of this BasicBlock.
 */
static inline uint32_t
BasicBlock_identity(const_BasicBlock this)
{
  return BasicBlock_IDENTITY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Index of this BasicBlock within its CodeRegion.
 */
static inline int32_t
BasicBlock_index(const_BasicBlock this)
{
  return BasicBlock_INDEX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock CodeRegion.
 */
static inline struct CodeRegion_ *
BasicBlock_codeRegion(const_BasicBlock this)
{
  return BasicBlock_CODEREGION(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock regionId.
 */
intptr_t
BasicBlock_regionId(const_BasicBlock this);
#endif//$XCC_h

intptr_t
BasicBlock_regionId(const_BasicBlock this)
{
  CodeRegion codeRegion = BasicBlock_CODEREGION(this);
  return CodeRegion_REGIONID(codeRegion);
}

#ifdef $XCC_h
/**
 * This BasicBlock LoopScope.
 */
static inline struct LoopScope_ *
BasicBlock_loopScope(const_BasicBlock this)
{
  return BasicBlock_LOOPSCOPE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this BasicBlock LoopScope.
 */
static inline void
BasicBlock_setLoopScope(BasicBlock this, struct LoopScope_ *loopScope)
{
  *BasicBlock__LOOPSCOPE(this) = loopScope;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * TraceId of this BasicBlock.
 */
static inline int32_t
BasicBlock_traceId(const_BasicBlock this)
{
  return BasicBlock_TRACEID(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the traceId of this BasicBlock.
 */
static inline void
BasicBlock_setTraceId(BasicBlock this, int32_t traceId)
{
  *BasicBlock__TRACEID(this) = traceId;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock frequency.
 */
static inline float
BasicBlock_frequency(const_BasicBlock this)
{
  return BasicBlock_FREQUENCY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this BasicBlock frequency.
 */
static inline void
BasicBlock_setFrequency(BasicBlock this, float frequency)
{
  *BasicBlock__FREQUENCY(this) = frequency;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock Procedure.
 */
static inline struct Procedure_ *
BasicBlock_procedure(const_BasicBlock this)
{
  return BasicBlock_PROCEDURE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock NativeBlock.
 */
static inline NativeBlock
BasicBlock_nativeBlock(const_BasicBlock this)
{
  return BasicBlock_NATIVEBLOCK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock CFGNode.
 */
static inline struct CFGNode_ *
BasicBlock_cfgNode(const_BasicBlock this)
{
  return BasicBlock_CFGNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this BasicBlock CFGNode.
 */
static inline void
BasicBlock_setCFGNode(BasicBlock this, struct CFGNode_ *cfgNode)
{
  *BasicBlock__CFGNODE(this) = cfgNode;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make a new BasicBlockEdge between two BasicBlocks.
 *
 * This creates a control-flow edge from BasicBlocks @p this to BasicBlocks 
 * @p that. If @p that appears just after @p this in the list of BasicBlocks, 
 * the edge is a fall-through edge and is hence put as first item of the 
 * LEAVEEDGES list of BasicBlock @p this. Otherwise, the edge is appended to 
 * this list.
 */
BasicBlockEdge
BasicBlock_makeEdge(BasicBlock this, BasicBlock that, float probability);
#endif//$XCC_h

BasicBlockEdge
BasicBlock_makeEdge(BasicBlock this, BasicBlock that, float probability)
{
  IDList leaveEdges = BasicBlock_LEAVEEDGES(this);
  size_t size = BasicBlockEdge_Size(this, that, probability);
  BasicBlockEdge edge = that == BasicBlock_next(this)?
                        IDList_put(leaveEdges, size):
                        IDList_push(leaveEdges, size);
  BasicBlockEdge_Ctor(edge, this, that, probability);
  *BasicBlockEdge__ENTERNEXT(edge) = BasicBlock_ENTERFIRST(that);
  *BasicBlock__ENTERFIRST(that) = edge;
  ++*BasicBlock__ENTERCOUNT(that);
  return edge;
}

#ifdef $XCC_h
/**
 * Check if @p target is a successor of basic block.
 */
BasicBlockEdge
BasicBlock_hasSuccessor(BasicBlock this, BasicBlock target);
#endif//$XCC_h

BasicBlockEdge
BasicBlock_hasSuccessor(BasicBlock this, BasicBlock target)
{
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(this, edge) {
    BasicBlock destBlock = BasicBlockEdge_destBlock(edge);
    if(target == destBlock)
      return edge;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  return NULL;
}

#ifdef $XCC_h
/**
 * * Get the PHI argument index coresponding to an edge
 */
int32_t
BasicBlock_getPHIArgIndex(BasicBlock this, BasicBlock origin);
#endif//$XCC_h

int32_t
BasicBlock_getPHIArgIndex(BasicBlock this, BasicBlock origin)
{
  BasicBlock_ENTER_FOREACH_BasicBlockEdge(this, edge) {
    BasicBlock origBlock = BasicBlockEdge_origBlock(edge);
    if(origin == origBlock)
      return BasicBlockEdge_ARGINDEX(edge);
  } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  return -1;
}

#ifdef $XCC_h
/**
 * Kill a successor BasicBlockEdge of this BasicBlock.
 *
 * This removes the control flow @p edge between BasicBlock @p this and the 
 * target of @p edge.
 */
void
BasicBlock_killEdge(BasicBlock this, BasicBlockEdge edge);
#endif//$XCC_h

void
BasicBlock_killEdge(BasicBlock this, BasicBlockEdge edge)
{
  IDList leaveEdges = BasicBlock_LEAVEEDGES(this);
  BasicBlock that = BasicBlockEdge_DESTBLOCK(edge);
  BasicBlockEdge next_edge = BasicBlockEdge_ENTERNEXT(edge);
  BasicBlockEdge that_edge = BasicBlock_ENTERFIRST(that);
  BasicBlockEdge prev_edge = NULL;
  while (that_edge != edge) {
    Except_CHECK(that_edge != NULL);
    prev_edge = that_edge;
    that_edge = BasicBlockEdge_ENTERNEXT(that_edge);
  }
  if (prev_edge != NULL) {
    *BasicBlockEdge__ENTERNEXT(prev_edge) = next_edge;
  } else {
    *BasicBlock__ENTERFIRST(that) = next_edge;
  }
  --*BasicBlock__ENTERCOUNT(that);
#ifndef _NDTOR
  IDList_remove(leaveEdges, edge, NULL);
#else//!_NDTOR
  IDList_remove_(leaveEdges, edge);
#endif//_NDTOR
}

#ifdef $XCC_h
/**
 * Get the target BasicBlockEdge of this basic block.
 *
 * @retval NULL if the target BasicBlockEdge does not exist.
 */
BasicBlockEdge
BasicBlock_getTargetEdge(const_BasicBlock this);
#endif//$XCC_h

BasicBlockEdge
BasicBlock_getTargetEdge(const_BasicBlock this)
{
  Operation operation = BasicBlock_lastOperation(this);
  if (operation != NULL) {
    Label label = Operation_getTargetLabel(operation);
    if (label != NULL) {
      BasicBlock_LEAVE_FOREACH_BasicBlockEdge(this, edge) {
        if (BasicBlock_hasLabel(BasicBlockEdge_DESTBLOCK(edge), label)) return edge;
      } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    }
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Get the default BasicBlockEdge of this BasicBlock.
 *
 * @retval NULL if default BasicBlockEdge does not exist.
 */
BasicBlockEdge
BasicBlock_getDefaultEdge(const_BasicBlock this);
#endif//$XCC_h

BasicBlockEdge
BasicBlock_getDefaultEdge(const_BasicBlock this)
{
  const_IDList leaveEdges = BasicBlock_LEAVEEDGES(this);
  int32_t leaveCount = IDList_count(leaveEdges);
  BasicBlockEdge edge = IDList_firstItem(leaveEdges);
  if (leaveCount > 0 && leaveCount <= 2) {
    BasicBlock block = BasicBlockEdge_DESTBLOCK(edge);
    if (leaveCount == 1)
      return edge;
    else {
      Operation last_operation = BasicBlock_lastOperation(this);
      Label label = Operation_getTargetLabel(last_operation);
      if (!BasicBlock_hasLabel(BasicBlockEdge_DESTBLOCK(edge), label)) return edge;
      return IDList_lastItem(leaveEdges);
    }
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Get the fall-through BasicBlockEdge of this BasicBlock.
 *
 * @retval NULL if the fall-through BasicBlockEdge does not exist.
 */
BasicBlockEdge
BasicBlock_getFallEdge(const_BasicBlock this);
#endif//$XCC_h

BasicBlockEdge
BasicBlock_getFallEdge(const_BasicBlock this)
{
  const_IDList leaveEdges = BasicBlock_LEAVEEDGES(this);
  int32_t leaveCount = IDList_count(leaveEdges);
  BasicBlockEdge edge = IDList_firstItem(leaveEdges);
  if (leaveCount > 0 && leaveCount <= 2) {
    BasicBlock block = BasicBlockEdge_DESTBLOCK(edge);
    if (leaveCount == 1) {
      Operation last_operation = BasicBlock_lastOperation(this);
      if (last_operation != NULL &&
          Operation_isBranch(last_operation)) return NULL;
    }
    if (BasicBlock_next(this) == block) return edge;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Fill this BasicBlock @c ENTEREDGES array.
 *
 * @param sort Sort the entering edges using @c BasicBlock_sortEnterEdges() if true.
 * @warning For use by SSAForm only.
 */
void
BasicBlock_makeEnterEdges(BasicBlock this, Memory memory, bool sort);
#endif//$XCC_h

void
BasicBlock_makeEnterEdges(BasicBlock this, Memory memory, bool sort)
{
  int32_t enterCount = BasicBlock_ENTERCOUNT(this);
  *BasicBlock__ENTEREDGES(this) = NULL;
  if (enterCount > 0) {
    int32_t argIndex = 0;
    size_t enterEdgesSize = enterCount*sizeof(BasicBlockEdge);
    BasicBlockEdge *enterEdges = Memory_alloc(memory, enterEdgesSize);
    BasicBlock_ENTER_FOREACH_BasicBlockEdge(this, edge) {
      *BasicBlockEdge__ARGINDEX(edge) = argIndex;
      enterEdges[argIndex] = edge;
      argIndex++;
    } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
    Except_CHECK(argIndex == enterCount);
    *BasicBlock__ENTEREDGES(this) = enterEdges;
    if (sort) {
      BasicBlockEdge next_edge = NULL;
      BasicBlock_sortEnterEdges(this);
      for (argIndex = enterCount - 1; argIndex >= 0; argIndex--) {
        BasicBlockEdge edge = enterEdges[argIndex];
        *BasicBlockEdge__ARGINDEX(edge) = argIndex;
        *BasicBlockEdge__ENTERNEXT(edge) = next_edge;
        Except_CHECK(!next_edge || BasicBlockEdge_gt(next_edge, edge));
        next_edge = edge;
      }
      *BasicBlock__ENTERFIRST(this) = next_edge;
    }
  }
}

#ifdef $XCC__h
/**
 * Sort this @c BasicBlock @c ENTEREDGES array.
 *
 * Comparison for sorting is using function @c BasicBlockEdge_gt().
 * See the en.wikibook.org heapsort.
 */
void
BasicBlock_sortEnterEdges(BasicBlock this);
#endif//$XCC__h

void
BasicBlock_sortEnterEdges(BasicBlock this)
{
  int32_t count = BasicBlock_ENTERCOUNT(this);
  BasicBlockEdge *base = BasicBlock_ENTEREDGES(this);
  int32_t n = count, i = count/2;
  while (true) {
    int32_t parent, child;
    BasicBlockEdge temp;
    if (i > 0) {
      temp = base[--i];
    } else if (n > 1) {
      temp = base[--n];
      base[n] = base[0];
    } else return;
    parent = i, child = i*2 + 1;
    while (child < n) {
      if (   child + 1 < n
          && BasicBlockEdge_gt(base[child + 1], base[child])) {
        child = child + 1;
      }
      if (BasicBlockEdge_gt(base[child], temp)) {
        base[parent] = base[child];
        parent = child;
        child = child*2 + 1;
      } else break;
    }
    base[parent] = temp;
  }
}

#ifdef $XCC_h
/**
 * Kill this BasicBlock ENTEREDGES array.
 *
 * For use by SSAForm only.
 */
static inline void
BasicBlock_killEnterEdges(BasicBlock this, Memory memory)
{
#ifndef _NDTOR
  Memory_free(memory, BasicBlock_ENTEREDGES(this));
#endif//_NDTOR
  *BasicBlock__ENTEREDGES(this) = NULL;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the BasicBlockEdge corresponding to a PHI argument.
 */
static inline BasicBlockEdge
BasicBlock_getPhiEnterEdge(const_BasicBlock this, int32_t argIndex)
{
  BasicBlockEdge *enterEdges = BasicBlock_ENTEREDGES(this);
  Except_CHECK(argIndex >= 0 && argIndex < BasicBlock_ENTERCOUNT(this));
  return enterEdges[argIndex];
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The live-in for this BasicBlock.
 */
TemporarySet
BasicBlock_liveInSet(const_BasicBlock this);
#define BasicBlock_isLiveIn(this, temporary) \
  TemporarySet_contains(BasicBlock_liveInSet(this), temporary)
#endif//$XCC_h

TemporarySet
BasicBlock_liveInSet(const_BasicBlock this)
{
  CodeRegion codeRegion = BasicBlock_CODEREGION(this);
  TemporarySet_ *liveInSets = CodeRegion_LIVEINSETS(codeRegion);
  int32_t liveCount = CodeRegion_LIVECOUNT(codeRegion);
  int32_t index = BasicBlock_INDEX(this);
  if (liveInSets != NULL && index < liveCount) {
    return liveInSets + index;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * The phi-in for this BasicBlock.
 */
TemporarySet
BasicBlock_phiDefSet(const_BasicBlock this);
#define BasicBlock_isPhiIn(this, temporary) \
  TemporarySet_contains(BasicBlock_phiDefSet(this), temporary)
#endif//$XCC_h

TemporarySet
BasicBlock_phiDefSet(const_BasicBlock this)
{
  CodeRegion codeRegion = BasicBlock_CODEREGION(this);
  TemporarySet_ *phiDefSets = CodeRegion_PHIDEFSETS(codeRegion);
  int32_t liveCount = CodeRegion_LIVECOUNT(codeRegion);
  int32_t index = BasicBlock_INDEX(this);
  if (phiDefSets != NULL && index < liveCount) {
    return phiDefSets + index;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * The live-out for this BasicBlock.
 */
TemporarySet
BasicBlock_liveOutSet(const_BasicBlock this);
#define BasicBlock_isLiveOut(this, temporary) \
  TemporarySet_contains(BasicBlock_liveOutSet(this), temporary)
#endif//$XCC_h

TemporarySet
BasicBlock_liveOutSet(const_BasicBlock this)
{
  CodeRegion codeRegion = BasicBlock_CODEREGION(this);
  TemporarySet_ *liveOutSets = CodeRegion_LIVEOUTSETS(codeRegion);
  int32_t liveCount = CodeRegion_LIVECOUNT(codeRegion);
  int32_t index = BasicBlock_INDEX(this);
  if (liveOutSets != NULL && index < liveCount) {
    return liveOutSets + index;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * The phi-out for this BasicBlock.
 */
TemporarySet
BasicBlock_phiUseSet(const_BasicBlock this);
#define BasicBlock_isPhiOut(this, temporary) \
  TemporarySet_contains(BasicBlock_phiUseSet(this), temporary)
#endif//$XCC_h

TemporarySet
BasicBlock_phiUseSet(const_BasicBlock this)
{
  CodeRegion codeRegion = BasicBlock_CODEREGION(this);
  TemporarySet_ *phiUseSets = CodeRegion_PHIUSESETS(codeRegion);
  int32_t liveCount = CodeRegion_LIVECOUNT(codeRegion);
  int32_t index = BasicBlock_INDEX(this);
  if (phiUseSets != NULL && index < liveCount) {
    return phiUseSets + index;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * This BasicBlock flags.
 */
static inline BasicBlockFlags
BasicBlock_flags(const_BasicBlock this)
{
  return BasicBlock_FLAGS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this BasicBlock flags.
 */
static inline void
BasicBlock_setFlags(BasicBlock this, BasicBlockFlags flags)
{
  *BasicBlock__FLAGS(this) |= flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Clear this BasicBlock flags.
 */
static inline void
BasicBlock_clearFlags(BasicBlock this, BasicBlockFlags flags)
{
  *BasicBlock__FLAGS(this) &= ~flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock mark.
 */
static inline int
BasicBlock_mark(const_BasicBlock this)
{
  return BasicBlock_MARK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this BasicBlock mark address.
 */
static inline void
BasicBlock_setMark(BasicBlock this, int mark)
{
  *BasicBlock__MARK(this) = mark;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock instance value.
 */
static inline int
BasicBlock_instance(const_BasicBlock this)
{
  return BasicBlock_INSTANCE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this BasicBlock instance value.
 */
static inline void
BasicBlock_setInstance(BasicBlock this, int instance)
{
  *BasicBlock__INSTANCE(this) = instance;
  Except_ENSURE(BasicBlock_INSTANCE(this) == instance);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This BasicBlock NOP padding.
 */
static inline int
BasicBlock_padding(const_BasicBlock this)
{
  return BasicBlock_PADDING(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this BasicBlock NOP padding.
 */
static inline void
BasicBlock_setPadding(BasicBlock this, int padding)
{
  *BasicBlock__PADDING(this) = padding;
  Except_ENSURE(BasicBlock_PADDING(this) == padding);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Count of labels in this BasicBlock.
 */
static inline int
BasicBlock_labelCount(const_BasicBlock this)
{
  return BasicBlock_LABELCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by BasicBlock_FOREACH_Label.
 */
static inline const Label*
BasicBlock_labels(const_BasicBlock this)
{
  return BasicBlock_LABELS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this BasicBlock Label(s).
 */
#define BasicBlock_FOREACH_Label(this, label) { \
  const Label *BasicBlock_LABELS = BasicBlock_labels(this); \
  int BasicBlock_LABELCOUNT = BasicBlock_labelCount(this), BasicBlock_I = 0; \
  for (; BasicBlock_I < BasicBlock_LABELCOUNT; BasicBlock_I++) { \
    Label label = BasicBlock_LABELS[BasicBlock_I];
#define BasicBlock_ENDEACH_Label \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the first label of this BasicBlock
 * or create one if there is none.
 */
Label
BasicBlock_makeLabel(BasicBlock this);
#endif//$XCC_h

Label
BasicBlock_makeLabel(BasicBlock this)
{
  int labelCount = BasicBlock_LABELCOUNT(this);
  if (labelCount == 0) {
    Procedure procedure = BasicBlock_PROCEDURE(this);
    Program program = Procedure_PROGRAM(procedure);
    LabelTable labelTable = Program_LABELTABLE(program);
    Label label = LabelTable_makeLabel(labelTable);
    BasicBlock_addLabel(this, label);
  }
  return BasicBlock_LABELS(this)[0];
}

#ifdef $XCC_h
/**
 * Add a new label for this BasicBlock.
 */
void
BasicBlock_addLabel(BasicBlock this, Label label);
#endif//$XCC_h

void
BasicBlock_addLabel(BasicBlock this, Label label)
{
  int new_maxLabelCount = 4;
  Label *old_labels = BasicBlock_LABELS(this);
  int old_labelCount = BasicBlock_LABELCOUNT(this);
  *Label__BLOCK(label) = this;
  if (old_labels == NULL) {
    Memory memory = BasicBlock_MEMORY(this);
    Label *labels = Memory_alloc(memory, new_maxLabelCount*sizeof(Label));
    *BasicBlock__LABELS(this) = labels;
    Except_CHECK(old_labelCount == 0);
    old_labels = labels;
  } else if (old_labelCount >= new_maxLabelCount) {
    int old_clp2Count = HackerU32_clp2(old_labelCount), i;
    if (old_labelCount == old_clp2Count) {
      Memory memory = BasicBlock_MEMORY(this);
      Label *labels = Memory_alloc(memory, old_clp2Count*2*sizeof(Label));
      Except_CHECK(old_clp2Count*2 == HackerU32_clp2(old_labelCount + 1));
      for (i = 0; i < old_labelCount; i++) labels[i] = old_labels[i];
      *BasicBlock__LABELS(this) = labels;
      Memory_free(memory, old_labels);
      old_labels = labels;
    }
  }
  *BasicBlock__LABELCOUNT(this) = old_labelCount + 1;
  old_labels[old_labelCount] = label;
}

#ifdef $XCC_h
/**
 * Return whether label is a label for the basic block.
 */
bool
BasicBlock_hasLabel(const_BasicBlock this, Label label);
#endif//$XCC_h

bool
BasicBlock_hasLabel(const_BasicBlock this, Label label)
{
  BasicBlock_FOREACH_Label(this, LABEL) {
    if (LABEL == label) return true;
  } BasicBlock_ENDEACH_Label;
  return false;
}

#ifdef $XCC_h
/**
 * Return the first Label of this BasicBlock if any, else NULL.
 */
static inline Label
BasicBlock_label(const_BasicBlock this)
{
  int labelCount = BasicBlock_LABELCOUNT(this);
  const Label *labels = BasicBlock_LABELS(this);
  return labelCount > 0 ? labels[0] : NULL;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Static estimation of contributed execution cycles.
 */
float
BasicBlock_contributed(const_BasicBlock this);
#endif//$XCC_h

float
BasicBlock_contributed(const_BasicBlock this)
{
  if (BasicBlock_isScheduled(this) && !BasicBlock_isEmpty(this)) {
    int padding = BasicBlock_PADDING(this);
    int firstDate = Operation_STARTDATE(BasicBlock_firstOperation(this));
    int lastDate = Operation_STARTDATE(BasicBlock_lastOperation(this));
    return BasicBlock_FREQUENCY(this)*(lastDate - firstDate + 1 + padding);
  }
  return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty-print this BasicBlock.
 */
bool
BasicBlock_pretty(const_BasicBlock this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
BasicBlock_pretty(const_BasicBlock this, FILE *file)
{
  LoopScope loopScope = BasicBlock_LOOPSCOPE(this);
  BasicBlock headerBlock = loopScope != NULL?
                           LoopScope_HEADERBLOCK(loopScope): NULL;
  if (file == NULL) file = stderr;
  fprintf(file, "%4d:Block_%d\n", BasicBlock_identity(this), BasicBlock_INDEX(this));
  if (BasicBlock_FLAGS(this) != 0) {
    BasicBlockFlags flags = BasicBlock_FLAGS(this);
    fprintf(file, "\t");
    ETable_prettyFlags(BasicBlockFlag_Table, flags, "+", file);
    fprintf(file, "\n");
  }
  fprintf(file, "\tfrequency=%g", BasicBlock_FREQUENCY(this));
  fprintf(file, "\tregionId=0x%zx", BasicBlock_regionId(this));
  fprintf(file, "\ttraceId=%d", BasicBlock_TRACEID(this));
  if (headerBlock != NULL) {
    fprintf(file, "\theaderBlock=%d:Block", BasicBlock_IDENTITY(headerBlock));
  }
  if (BasicBlock_INSTANCE(this) != 0) {
    fprintf(file, "\tinstance=%d", BasicBlock_INSTANCE(this));
  }
  fprintf(file, "\n\tpredecessors:\t");
  BasicBlock_ENTER_FOREACH_BasicBlockEdge(this, edge) {
    BasicBlock origBlock = BasicBlockEdge_origBlock(edge);
    fprintf(file, "%d:Block_%d(%s%s%+d)\t",
            BasicBlock_identity(origBlock), BasicBlock_INDEX(origBlock),
            BasicBlockEdge_ISLOOP(edge)? "Loop,": "",
            BasicBlockEdge_ISJOIN(edge)? "Join,": "",
            BasicBlockEdge_ELAPSED(edge));
  } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  fprintf(file, "\n\tsuccessors:\t");
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(this, edge) {
    BasicBlock destBlock = BasicBlockEdge_destBlock(edge);
    fprintf(file, "%d:Block_%d(%2.0f%%)\t",
            BasicBlock_identity(destBlock), BasicBlock_INDEX(destBlock),
            100.0*BasicBlockEdge_PROBABILITY(edge));
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  fprintf(file, "\n");
  if (BasicBlock_liveInSet(this) != NULL) {
    fprintf(file, "\tlive-in:\t");
    TemporarySet_pretty(BasicBlock_liveInSet(this), file);
    fprintf(file, "\n");
  }
  if (BasicBlock_phiDefSet(this) != NULL) {
    fprintf(file, "\tphi-def:\t");
    TemporarySet_pretty(BasicBlock_phiDefSet(this), file);
    fprintf(file, "\n");
  }
  if (BasicBlock_LABELCOUNT(this) != 0) {
    const char *sep = "\t";
    BasicBlock_FOREACH_Label(this, label) {
      fprintf(file, "%s%s", sep, Label_NAME(label));
      sep = " ";
    } BasicBlock_ENDEACH_Label;
    fprintf(file, ":\n");
  }
  BasicBlock_FOREACH_Operation(this, operation) {
    Except_CHECK(Operation_BASICBLOCK(operation) == this);
    fprintf(file, "\t"), Operation_pretty(operation, file);
  } BasicBlock_ENDEACH_Operation;
  if (BasicBlock_liveOutSet(this) != NULL) {
    fprintf(file, "\tlive-out:\t");
    TemporarySet_pretty(BasicBlock_liveOutSet(this), file);
    fprintf(file, "\n");
  }
  if (BasicBlock_phiUseSet(this) != NULL) {
    fprintf(file, "\tphi-use:\t");
    TemporarySet_pretty(BasicBlock_phiUseSet(this), file);
    fprintf(file, "\n");
  }
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Assembly printing of this BasicBlock.
 */
bool
BasicBlock_asmPrint(const_BasicBlock this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
BasicBlock_asmPrint(const_BasicBlock this, FILE *file)
{
  int16_t bundleDate = -2;
  bool label_passed = false;
  bool first_operation = true;
  /* TODO: loopscope */
  if (file == NULL) file = stderr;

  BasicBlock_FOREACH_Label(this, label) {
    const char* str = Label_name(label);
    Except_WARNIF(str[0] != '.' || str[1] != 'L',
        "The label does not start with `.L'!");
    if (!strstr(str, ".LAO"))
      fprintf(file, "%s:\n", str);
  } BasicBlock_ENDEACH_Label;

  if (IDList_isEmpty(BasicBlock_operations(this)) == false) {
    BasicBlock_FOREACH_Operation(this, operation) {
      Except_CHECK(Operation_BASICBLOCK(operation) == this);
      Operator operator = Operation_operator(operation);

      // Skip the first set of labels as they have already been printed
      if (!label_passed) {
        if ((operator == Operator_LABEL)
            && Temporary_isLabel(Operation_targetTemporary(operation)))
          continue;
        else
          label_passed = true;
      }

      // First deal with the possibility of a pseudo-operator
      if (Operator_isPseudo(operator)) {
        switch (operator) {
          case Operator_LABEL:
            {
              Temporary target = Operation_targetTemporary(operation);
              Except_CHECK(Temporary_isSymbol(target) || Temporary_isLabel(target));
              if (Temporary_isSymbol(target)) {
                Symbol targetSymbol = Literal_symbol(Temporary_LITERAL(target));
                fprintf(file, "\t;;\n%s:\n", Symbol_name(targetSymbol));
              } else {
                Label targetLabel = Literal_label(Temporary_LITERAL(target));
                fprintf(file, "\t;;\n%s:\n", Label_name(targetLabel));
              }
              break;
            }

          default:
            // Skip the operator as nothing should be printed
            break;
        }
        continue;
      }

      // Print all real operations while dealing with bundling
      if (first_operation) {
        bundleDate = Operation_startDate(operation);
        first_operation = false;
      } else {
        if (Operation_startDate(operation) != bundleDate) {
          bundleDate = Operation_startDate(operation);
          fprintf(file, "\t;;\n");
        } else if (bundleDate == -1) {
          fprintf(file, "\t;;\n");
        }
      }

      Operation_asmPrint(operation, file);
    } BasicBlock_ENDEACH_Operation;
    fprintf(file, "\t;;\n");
  }

  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Get a label for basic block for MiniR format.
 */
bool
BasicBlock_prettyLabelMinir(const_BasicBlock this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
BasicBlock_prettyLabelMinir(const_BasicBlock this, FILE *file)
{
  Label label = BasicBlock_label(this);
  if (label != NULL) {
    fprintf(file, "%s", Label_name(label));
  } else {
    fprintf(file, "Block_%d", BasicBlock_identity(this));
  }
  return true;
}
#endif//_NTRACE


#ifdef $XCC_h
/**
 * Pretty-print this BasicBlock in Minir format
 */
bool
BasicBlock_prettyMinir(const_BasicBlock this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
BasicBlock_prettyMinir(const_BasicBlock this, FILE *file)
{
  /* TODO: loopscope */
  if (file == NULL) file = stderr;

  fprintf(file, "      - label: Block_%d\n", BasicBlock_identity(this));
  if (BasicBlock_LABELCOUNT(this) != 0) {
    const char *sep = "";
    fprintf(file, "        annotations: { labels: [");
    BasicBlock_FOREACH_Label(this, label) {
      fprintf(file, "%s'%s'", sep, Label_name(label));
      sep = ", ";
    } BasicBlock_ENDEACH_Label;
    fprintf(file, "] }\n");
  }
  /* TODO: flags, headerblock, instance, predecessors, successors, live-in, 
   * phidefset, labels */

  if (IDList_isEmpty(BasicBlock_operations(this))) {
    fprintf(file, "        ops: [ { op: NOP } ]\n");
  } else {
    fprintf(file, "        ops:\n");
    BasicBlock_FOREACH_Operation(this, operation) {
      Except_CHECK(Operation_BASICBLOCK(operation) == this);
      Operation_prettyMinir(operation, file);
    } BasicBlock_ENDEACH_Operation;
  }
  /* TODO: live-out, phiuseset */
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Check if this BasicBlock has effects on Dedicated.
 */
bool
BasicBlock_hasDedicatedEffects(const_BasicBlock this);
#endif//$XCC_h

bool
BasicBlock_hasDedicatedEffects(const_BasicBlock this)
{
  BasicBlock_FORBACK_Operation(this, OPERATION) {
    if (Operation_hasDedicatedArgument(OPERATION, false)) return true;
    if (Operation_hasDedicatedResult(OPERATION, false)) return true;
  } BasicBlock_ENDBACK_Operation;
  return false;
}

#ifdef $XCC_h
/**
 * Test if this BasicBlock is empty.
 */
static inline bool
BasicBlock_isEmpty(const_BasicBlock this)
{
  return Selector_isEmpty(BasicBlock_SELECTOR(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of operations in this BasicBlock.
 */
static inline uint32_t
BasicBlock_countOperations(const_BasicBlock this)
{
  return Selector_countOperations(BasicBlock_SELECTOR(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * First operation in this BasicBlock.
 */
static inline Operation
BasicBlock_firstOperation(const_BasicBlock this)
{
  return Selector_firstOperation(BasicBlock_SELECTOR(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Last operation in this BasicBlock.
 */
static inline Operation
BasicBlock_lastOperation(const_BasicBlock this)
{
  return Selector_lastOperation(BasicBlock_SELECTOR(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the control operation of this BasicBlock.
 */
static inline Operation
BasicBlock_getControlOperation(const_BasicBlock this)
{
  Operation last_operation = BasicBlock_lastOperation(this);
  if (last_operation != NULL &&
      Operator_isControl(Operation_OPERATOR(last_operation))) {
    return last_operation;
  }
  return NULL;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test if this BasicBlock has a control operation.
 */
#define BasicBlock_hasControlOperation(this) \
  (BasicBlock_getControlOperation(this) != NULL)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the Routine operation of this BasicBlock.
 */
static inline Operation
BasicBlock_getRoutineOperation(const_BasicBlock this)
{
  Operation last_operation = BasicBlock_lastOperation(this);
  if (last_operation != NULL &&
      Operator_isRoutine(Operation_OPERATOR(last_operation))) {
    return last_operation;
  }
  return NULL;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the Jump operation of this BasicBlock.
 */
static inline Operation
BasicBlock_getJumpOperation(const_BasicBlock this)
{
  Operation last_operation = BasicBlock_lastOperation(this);
  if (last_operation != NULL &&
      Operator_isJump(Operation_OPERATOR(last_operation))) {
    return last_operation;
  }
  return NULL;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make a SSA Phi operation.
 */
Operation
BasicBlock_makeOperationPHI(BasicBlock this, Temporary temporary);
#endif//$XCC_h

Operation
BasicBlock_makeOperationPHI(BasicBlock this, Temporary temporary)
{
  Selector selector = BasicBlock_SELECTOR(this);
  int32_t enterCount = BasicBlock_ENTERCOUNT(this), i;
  Operation operation = Selector_makeOperation_(selector,
                                                Operator_PHI, 1, enterCount, NULL);
  Temporary *array = Operation_OPERANDS(operation);
  for (i = 0; i <= enterCount; i++) array[i] = temporary;
  Selector_moveOperationFirst(selector, operation);
  *Operation__ORDERING(operation) = BasicBlock_FIRSTORDERING(this);
  Selector_incrementPhiCount(selector);
  return operation;
}

#ifdef $XCC_h
/**
 * Make a SSI Sigma operation.
 */
Operation
BasicBlock_makeOperationSIGMA(BasicBlock this, Temporary temporary);
#endif//$XCC_h

Operation
BasicBlock_makeOperationSIGMA(BasicBlock this, Temporary temporary) {
  Selector selector = BasicBlock_SELECTOR(this);
  int leaveCount = BasicBlock_LEAVECOUNT(this), i;
  Operation operation = Selector_makeOperation_(selector,
                                                Operator_SIGMA, leaveCount, 1, NULL);
  Temporary *array = Operation_OPERANDS(operation);
  for (i = 0; i <= leaveCount; i++) array[i] = temporary;
  *Operation__ORDERING(operation) = BasicBlock_LASTORDERING(this);
  return operation;
}

#ifdef $XCC_h
/**
 * return this BasicBlock early Operation.
 *
 * An early operation is the first that is not a PHI or a LABEL Operation.
 */
Operation
BasicBlock_earlyOperation(const_BasicBlock this);
#endif//$XCC_h

Operation
BasicBlock_earlyOperation(const_BasicBlock this)
{
  BasicBlock_FOREACH_Operation(this, operation) {
    Operator operator = Operation_OPERATOR(operation);
    if (!Operator_isPhi(operator) && !Operator_isLabel(operator)) {
      return operation;
    }
  } BasicBlock_ENDEACH_Operation;
  return NULL;
}

#ifdef $XCC_h
/**
 * Move this BasicBlock Operation early.
 *
 * Ensure:	BasicBlock invariants.
 */
void
BasicBlock_moveOperationEarly(BasicBlock this, Operation operation);
#endif//$XCC_h

void
BasicBlock_moveOperationEarly(BasicBlock this, Operation operation)
{
  Operation before_operation = NULL;
  IDList operations = BasicBlock_OPERATIONS(this);
  int32_t earlyOrdering = BasicBlock_EARLYORDERING(this);
  Except_REQUIRE(Operation_BASICBLOCK(operation) == this);
  IDList_remove_(operations, operation);
  before_operation = BasicBlock_earlyOperation(this);
  if (before_operation != NULL) {
    IDList_insert_(operations, before_operation, operation);
  } else {
    // There are only PHI or LABEL or no Operations in this BasicBlock.
    IDList_push_(operations, operation);
  }
  *Operation__ORDERING(operation) = *BasicBlock__EARLYORDERING(this)
                                  = earlyOrdering - 1;
}

#ifdef $XCC_h
/**
 * return this BasicBlock late Operation.
 *
 * A late operation is the last that is not a SIGMA or a Control Operation.
 */
Operation
BasicBlock_lateOperation(const_BasicBlock this);
#endif//$XCC_h

Operation
BasicBlock_lateOperation(const_BasicBlock this)
{
  BasicBlock_FORBACK_Operation(this, operation) {
    Operator operator = Operation_OPERATOR(operation);
    if (!Operator_isSigma(operator) && !Operator_isControl(operator)) {
      return operation;
    }
  } BasicBlock_ENDBACK_Operation;
  return NULL;
}

#ifdef $XCC_h
/**
 * Move this BasicBlock Operation late.
 *
 * Ensure:	BasicBlock invariants.
 */
void
BasicBlock_moveOperationLate(BasicBlock this, Operation operation);
#endif//$XCC_h

void
BasicBlock_moveOperationLate(BasicBlock this, Operation operation)
{
  Operation after_operation = NULL;
  IDList operations = BasicBlock_OPERATIONS(this);
  int32_t lateOrdering = BasicBlock_LATEORDERING(this);
  Except_REQUIRE(Operation_BASICBLOCK(operation) == this);
  IDList_remove_(operations, operation);
  after_operation = BasicBlock_lateOperation(this);
  if (after_operation != NULL) {
    IDList_append_(operations, after_operation, operation);
  } else {
    // There are only SIGMA or Control or no Operations in this BasicBlock.
    IDList_put_(operations, operation);
  }
  *Operation__ORDERING(operation) = *BasicBlock__LATEORDERING(this)
                                  = lateOrdering + 1;
}

#ifdef $XCC_h
/**
 * Make a parallel COPY Operation before or after @c point operation.
 * @param before tells whether to add it before or after
 * @param maxCount is the maximum number of arguments+results
 * @param increment is used for the basic block ordering (only if > 0)
 */
Operation
BasicBlock_makeOperationPCOPY(BasicBlock this, Operation point, bool before,
                              int32_t maxCount, int32_t increment);
#endif//$XCC_h

Operation
BasicBlock_makeOperationPCOPY(BasicBlock this, Operation point, bool before,
                              int32_t maxCount, int32_t increment) {
  Operation pcopy_operation = NULL;
  Selector selector = BasicBlock_SELECTOR(this);

  pcopy_operation = Selector_makeOperationParCopy(selector, Operator_PCOPY, maxCount, false);
  if(point != NULL) {
    if (before == true) {
      Selector_moveOperationBefore(selector, point, pcopy_operation);
    } else {
      Selector_moveOperationAfter(selector, point, pcopy_operation);
    }
  } else {
    if(before == true) {
      Selector_moveOperationFirst(selector, pcopy_operation);
    } else {
      // Inserts the operation last in selector, do nothing.
      // Selector_moveOperationLast(selector, pcopy_operation);
    }
  }
  if(increment > 0) {
    BasicBlock_makeOrderings(this, 0, increment);
  }
  return pcopy_operation;
}

#ifdef $XCC_h
/**
 * Make a sequential Operation at early position.
 */
Operation
BasicBlock_makeEarlyOperationCOPY(BasicBlock this, Temporary result, Temporary argument);
#endif//$XCC_h

Operation
BasicBlock_makeEarlyOperationCOPY(BasicBlock this, Temporary result, Temporary argument)
{
  Selector selector = BasicBlock_selector(this);
  Operation copy_operation = Selector_makeOperationCOPY(selector, result, argument);
  BasicBlock_moveOperationEarly(this, copy_operation);
  return copy_operation;
}

#ifdef $XCC_h
/**
 * Make a parallel copy Operation at early position.
 */
Operation
BasicBlock_makeOperationECOPY(BasicBlock this);
#endif//$XCC_h

Operation
BasicBlock_makeOperationECOPY(BasicBlock this)
{
  Operation pcopy_operation = NULL;
  int32_t earlyArity = BasicBlock_PHICOUNT(this);
  if (earlyArity > 0) {
    Operation after_operation = NULL;
    Selector selector = BasicBlock_SELECTOR(this);
    int32_t earlyOrdering = BasicBlock_EARLYORDERING(this);
    BasicBlock_FOREACH_Operation(this, operation) {
      Operator operator = Operation_OPERATOR(operation);
      if (Operator_isPhi(operator));
      else if (Operator_isLabel(operator));
      else if (Operator_isECopy(operator)) return operation;
      else break;
      after_operation = operation;
    } BasicBlock_ENDEACH_Operation;
    pcopy_operation = Selector_makeOperationParCopy(selector, Operator_ECOPY, earlyArity, false);
    if (after_operation != NULL) {
      Selector_moveOperationAfter(selector, after_operation, pcopy_operation);
    }
    *Operation__ORDERING(pcopy_operation) = *BasicBlock__EARLYORDERING(this)
                                          = earlyOrdering - 1;
    Except_CHECK(pcopy_operation == BasicBlock_earlyOperation(this));
  }
  return pcopy_operation;
}

#ifdef $XCC_h
/**
 * Make a sequential copy Operation at late position.
 */
Operation
BasicBlock_makeLateOperationCOPY(BasicBlock this, Temporary result, Temporary argument);
#endif//$XCC_h

Operation
BasicBlock_makeLateOperationCOPY(BasicBlock this, Temporary result, Temporary argument)
{
  Selector selector = BasicBlock_selector(this);
  Operation copy_operation = Selector_makeOperationCOPY(selector, result, argument);
  BasicBlock_moveOperationLate(this, copy_operation);
  return copy_operation;
}

#ifdef $XCC_h
/**
 * Make a parallel copy Operation at late position.
 */
Operation
BasicBlock_makeOperationLCOPY(BasicBlock this);
#endif//$XCC_h

Operation
BasicBlock_makeOperationLCOPY(BasicBlock this)
{
  Operation pcopy_operation = NULL;
  int32_t lateArity = 0;
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(this, edge) {
    BasicBlock destBlock = BasicBlockEdge_DESTBLOCK(edge);
    lateArity += BasicBlock_PHICOUNT(destBlock);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  if (lateArity > 0) {
    Operation before_operation = NULL;
    Selector selector = BasicBlock_SELECTOR(this);
    int32_t lateOrdering = BasicBlock_LATEORDERING(this);
    BasicBlock_FORBACK_Operation(this, operation) {
      Operator operator = Operation_OPERATOR(operation);
      if (Operator_isControl(operator));
      else if (Operator_isSigma(operator));
      else if (Operator_isLCopy(operator)) return operation;
      else break;
      before_operation = operation;
    } BasicBlock_ENDBACK_Operation;
    pcopy_operation = Selector_makeOperationParCopy(selector, Operator_LCOPY, lateArity, false);
    if (before_operation != NULL) {
      Selector_moveOperationBefore(selector, before_operation, pcopy_operation);
    }
    *Operation__ORDERING(pcopy_operation) = *BasicBlock__LATEORDERING(this)
                                          = lateOrdering + 1;
    Except_CHECK(pcopy_operation == BasicBlock_lateOperation(this));
  }
  return pcopy_operation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Remove an Operation from this BasicBlock.
 */
void
BasicBlock_detachOperation(BasicBlock this, Operation operation);
#endif//$XCC_h

void
BasicBlock_detachOperation(BasicBlock this, Operation operation)
{
  Selector selector = BasicBlock_SELECTOR(this);
  if (Operator_isPhi(Operation_OPERATOR(operation))) {
    Selector_decrementPhiCount(selector);
  }
  Selector_detachOperation(selector, operation);
}

#ifdef $XCC_h
/**
 * Put the contents of a Selector first on this BasicBlock.
 * @note This removes the content from the Selector
 */
void
BasicBlock_putSelector(BasicBlock this, Selector selector);
#endif//$XCC_h

void
BasicBlock_putSelector(BasicBlock this, Selector selector)
{
  while (!Selector_isEmpty(selector)) {
    Operation operation = Selector_lastOperation(selector);
    IDList_pop_(Selector_OPERATIONS(selector));
    IDList_put_(BasicBlock_OPERATIONS(this), operation);
    *Operation__BASICBLOCK(operation) = this;
  }
}

#ifdef $XCC_h
/**
 * Push the contents of a Selector last on this BasicBlock.
 * @note This removes the content from the Selector
 */
void
BasicBlock_pushSelector(BasicBlock this, Selector selector);
#endif//$XCC_h

void
BasicBlock_pushSelector(BasicBlock this, Selector selector)
{
  while (!Selector_isEmpty(selector)) {
    Operation operation = Selector_firstOperation(selector);
    IDList_drop_(Selector_OPERATIONS(selector));
    IDList_push_(BasicBlock_OPERATIONS(this), operation);
    *Operation__BASICBLOCK(operation) = this;
  }
}

#ifdef $XCC_h
/**
 * Insert a Selector before Operation in this BasicBlock.
 * @note This removes the content from the Selector
 */
void
BasicBlock_insertSelector(BasicBlock this, Operation before, Selector selector);
#endif//$XCC_h

void
BasicBlock_insertSelector(BasicBlock this, Operation before, Selector selector)
{
  while (!Selector_isEmpty(selector)) {
    Operation operation = Selector_firstOperation(selector);
    IDList_drop_(Selector_OPERATIONS(selector));
    IDList_insert_(BasicBlock_OPERATIONS(this), before, operation);
    *Operation__BASICBLOCK(operation) = this;
  }
}

#ifdef $XCC_h
/**
 * Append a Selector after Operation in this BasicBlock.
 * @note This removes the content from the Selector
 */
void
BasicBlock_appendSelector(BasicBlock this, Operation after, Selector selector);
#endif//$XCC_h

void
BasicBlock_appendSelector(BasicBlock this, Operation after, Selector selector)
{
  while (!Selector_isEmpty(selector)) {
    Operation operation = Selector_lastOperation(selector);
    IDList_pop_(Selector_OPERATIONS(selector));
    IDList_append_(BasicBlock_OPERATIONS(this), after, operation);
    *Operation__BASICBLOCK(operation) = this;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Return the operation adjusting SP in this block.
 *
 * @param reverse if search should start from the bottom of the block (bottom-up).
 *
 * @note the BasicBlock @p this must be either an entry or exit block.
 * @return @c NULL if no ADJUST operation can be found.
 */
Operation
BasicBlock_findStackAdjust(BasicBlock this, bool reverse);
#endif//$XCC__h

Operation
BasicBlock_findStackAdjust(BasicBlock this, bool reverse)
{
  Procedure procedure = BasicBlock_procedure(this);
  Program program = Procedure_program(procedure);
  Convention convention = Program_convention(program);

  Except_CHECK(BasicBlock_isEntry(this) || BasicBlock_isExit(this));
  if(reverse) {
    BasicBlock_FORBACK_Operation(this, operation) {
      if(!Operation_isExitCode(operation)) break;
      if(Operator_isAdjust(Operation_operator(operation))) {
        Temporary temp = Operation_getArgument(operation, 0);
        Except_CHECK(Temporary_hasRegister(temp));
        if(Convention_isStackRegister(convention, Temporary_register(temp))) 
          return operation;
      }
    } BasicBlock_ENDBACK_Operation;
  }
  else {
    BasicBlock_FOREACH_Operation(this, operation) {
      if(!Operation_isEntryCode(operation)) break;
      if(Operator_isAdjust(Operation_operator(operation))) {
        Temporary temp = Operation_getArgument(operation, 0);
        Except_CHECK(Temporary_hasRegister(temp));
        if(Convention_isStackRegister(convention, Temporary_register(temp))) 
          return operation;
      }
    } BasicBlock_ENDEACH_Operation;
  }
  return NULL;
}


#ifdef $XCC__h
/**
 * Flag the Procedure entry code of this BasicBlock.
 *
 * The Procedure entry code sequence includes:
 * - any copy from a dedicated register that is not constant.
 * - copy, add or adjust that defines the frame pointer
 * - first copy, add or adjust that defines the stack pointer
 */
void
BasicBlock_flagEntryCode(BasicBlock this, Convention convention);
#endif//$XCC__h

void
BasicBlock_flagEntryCode(BasicBlock this, Convention convention)
{
  Operation stack_operation = NULL;
  Register stackRegister = Convention_stackRegister(convention);
  Register frameRegister = Convention_frameRegister(convention);
  Except_REQUIRE(BasicBlock_isEntry(this));
  BasicBlock_FOREACH_Operation(this, operation) {
    Operator operator = Operation_OPERATOR(operation);
    if (Operation_isExitCode(operation)) break;
    // Any copy from a dedicated register that is not constant.
    if (Operator_isSeqCopy(operator)) {
      Temporary argument = Operation_getArgument(operation, 0);
      Except_CHECK(Operation_argCount(operation) == 1);
      if (   Temporary_isDedicated(argument)
          && !Temporary_isConstant(argument)) {
        *Operation__FLAGS(operation) |= OperationFlag_EntryCode;
        LIR_TRACE && fprintf(LIR_TRACE, "*** EntryCode")
                  && Operation_pretty(operation, LIR_TRACE);
        continue;
      }
    }
    if (Operation_resCount(operation) == 1) {
      Temporary result = Operation_getResult(operation, 0);
      if (Temporary_isDedicated(result)) {
        Register registre = Temporary_REGISTER(result);
        // Move or adjust that defines the frame pointer.
        if (registre == frameRegister) {
          *Operation__FLAGS(operation) |= OperationFlag_EntryCode;
          LIR_TRACE && fprintf(LIR_TRACE, "*** EntryCode")
                    && Operation_pretty(operation, LIR_TRACE);
          continue;
        }
        // First copy or adjust that defines the stack pointer.
        if (registre == stackRegister && stack_operation == NULL) {
          *Operation__FLAGS(operation) |= OperationFlag_EntryCode;
          stack_operation = operation;
          LIR_TRACE && fprintf(LIR_TRACE, "*** EntryCode")
                    && Operation_pretty(operation, LIR_TRACE);
          continue;
        }
      }
    }
    break;
  } BasicBlock_ENDEACH_Operation;
}

#ifdef $XCC__h
/**
 * Flag the Procedure exit code of this BasicBlock.
 *
 * The Procedure exit code sequence includes:
 * - last control operation
 * - any copy to a dedicated register
 * - last copy, add or adjust of stack pointer
 */
void
BasicBlock_flagExitCode(BasicBlock this, Convention convention);
#endif//$XCC__h

void
BasicBlock_flagExitCode(BasicBlock this, Convention convention)
{
  Register stackRegister = Convention_stackRegister(convention);
  Operation control_operation = NULL;
  Operation stack_operation = NULL;
  Except_REQUIRE(BasicBlock_isExit(this));
  BasicBlock_FORBACK_Operation(this, operation) {
    Operator operator = Operation_OPERATOR(operation);
    if (Operation_isEntryCode(operation)) break;
    // Last control operation.
    if (Operator_isControl(operator) &&
        control_operation == NULL && (control_operation = operation)) {
      *Operation__FLAGS(operation) |= OperationFlag_ExitCode;
      LIR_TRACE && fprintf(LIR_TRACE, "*** ExitCode")
                && Operation_pretty(operation, LIR_TRACE);
      continue;
    }
    // Any copy to a dedicated register.
    if (Operator_isSeqCopy(operator)) {
      Temporary result = Operation_getResult(operation, 0);
      Except_CHECK(Operation_argCount(operation) == 1);
      if (Temporary_isDedicated(result)) {
        *Operation__FLAGS(operation) |= OperationFlag_ExitCode;
        LIR_TRACE && fprintf(LIR_TRACE, "*** ExitCode")
                  && Operation_pretty(operation, LIR_TRACE);
        continue;
      }
    }
    // Last copy or adjust of stack pointer.
    if (Operation_resCount(operation) == 1) {
      Temporary result = Operation_getResult(operation, 0);
      if (Temporary_isDedicated(result)) {
        Register registre = Temporary_REGISTER(result);
        if (registre == stackRegister && stack_operation == NULL) {
          *Operation__FLAGS(operation) |= OperationFlag_ExitCode;
          stack_operation = operation;
          LIR_TRACE && fprintf(LIR_TRACE, "*** ExitCode")
                    && Operation_pretty(operation, LIR_TRACE);
          continue;
        }
      }
    }
    break;
  } BasicBlock_ENDBACK_Operation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make this BasicBlock Operation ORDERING(s).
 *
 * There are several ordering values to consider:
 * - firstOrdering is the first valid ordering, used by Phi and Label.
 * - early ordering is where the early PCOPY or its sequentialization appears.
 * - late ordering is where the late PCOPY or its sequentialization appears.
 * - last ordering is the last valid ordering, used by Control and Sigma.
 */
int32_t
BasicBlock_makeOrderings(BasicBlock this, int32_t ordering, int32_t increment);
#endif//$XCC_h

int32_t
BasicBlock_makeOrderings(BasicBlock this, int32_t ordering, int32_t increment)
{
  int32_t firstOrdering = ordering + increment;
  int32_t earlyArity = BasicBlock_PHICOUNT(this), lateArity = 0;
  int32_t earlyOrdering, lateOrdering, lastOrdering;
  Except_REQUIRE(firstOrdering > 0 && increment > 0);
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(this, edge) {
    BasicBlock destBlock = BasicBlockEdge_DESTBLOCK(edge);
    lateArity += BasicBlock_PHICOUNT(destBlock);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  earlyOrdering = firstOrdering + increment + earlyArity;
  lateOrdering = earlyOrdering + increment;
  lastOrdering = lateOrdering + increment + lateArity;
  BasicBlock_FOREACH_Operation(this, operation) {
    Operator operator = Operation_OPERATOR(operation);
    if (   Operator_isPhi(operator)
        || Operator_isLabel(operator)) {
      ordering = firstOrdering;
    } else
    if (   Operator_isControl(operator)
        || Operator_isSigma(operator)) {
      ordering = lastOrdering;
    } else {
      ordering = lateOrdering;
      lateOrdering += increment;
      lastOrdering += increment;
    }
    *Operation__ORDERING(operation) = ordering;
  } BasicBlock_ENDEACH_Operation;
  *BasicBlock__FIRSTORDERING(this) = firstOrdering;
  *BasicBlock__EARLYORDERING(this) = earlyOrdering;
  *BasicBlock__LATEORDERING(this) = lateOrdering;
  *BasicBlock__LASTORDERING(this) = lastOrdering;
  return lastOrdering;
}

#ifdef $XCC_h
static inline int32_t
BasicBlock_firstOrdering(const_BasicBlock this)
{
  return BasicBlock_FIRSTORDERING(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
BasicBlock_earlyOrdering(const_BasicBlock this)
{
  return BasicBlock_EARLYORDERING(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
BasicBlock_lateOrdering(const_BasicBlock this)
{
  return BasicBlock_LATEORDERING(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
BasicBlock_lastOrdering(const_BasicBlock this)
{
  return BasicBlock_LASTORDERING(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check this BasicBlock Operation ORDERING(s).
 */
bool
BasicBlock_checkOrderings(const_BasicBlock this);
#endif//$XCC_h

bool
BasicBlock_checkOrderings(const_BasicBlock this)
{
  int32_t firstOrdering = BasicBlock_FIRSTORDERING(this);
  int32_t lastOrdering = BasicBlock_LASTORDERING(this);
  int32_t previous_ordering = firstOrdering;
  BasicBlock_FOREACH_Operation(this, operation) {
    int32_t ordering = Operation_ORDERING(operation);
    Operator operator = Operation_OPERATOR(operation);
    if (   Operator_isPhi(operator)
        || Operator_isLabel(operator)) {
      Except_CHECK(ordering == firstOrdering);
      if (ordering != firstOrdering) return false;
    } else
    if (   Operator_isControl(operator)
        || Operator_isSigma(operator)) {
      Except_CHECK(ordering == lastOrdering);
      if (ordering != lastOrdering) return false;
      Except_CHECK(lastOrdering > previous_ordering);
      if (lastOrdering <= previous_ordering) return false;
    } else {
      Except_CHECK(ordering > previous_ordering);
      if (ordering <= previous_ordering) return false;
    }
    previous_ordering = ordering;
  } BasicBlock_ENDEACH_Operation;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * True iff this FREQUENCY is less than that FREQUENCY.
 */
bool
BasicBlock_frequencyLT(const_BasicBlock this, const_BasicBlock that);
#endif//$XCC_h

bool
BasicBlock_frequencyLT(const_BasicBlock this, const_BasicBlock that)
{
  // HACK! uint32_t order is same as float order for non-negative float(s).
  uint32_t this_frequency = *(uint32_t *)BasicBlock__FREQUENCY(this);
  uint32_t that_frequency = *(uint32_t *)BasicBlock__FREQUENCY(that);
  if (this_frequency == that_frequency) {
    return BasicBlock_index(this) > BasicBlock_index(that);
  }
  return this_frequency < that_frequency;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Stack of BasicBlock(s).
 */
struct BasicBlockStack_ {
  //@args	Memory memory, int maxCount
  PtrSeq_ __;		//!< Underlying PtrSeq.
};
#endif//$XCC_h

#ifdef $XCC_h
static inline PtrSeq
BasicBlockStack__(BasicBlockStack this) {
  return BasicBlockStack____(this);
}
static inline const_PtrSeq
BasicBlockStack__const(const_BasicBlockStack this) {
  return BasicBlockStack____(this);
}
#endif//$XCC_h

BasicBlockStack
BasicBlockStack_Ctor(BasicBlockStack this,
                     Memory memory, int maxCount)
{
  PtrSeq_Ctor(BasicBlockStack____(this), memory, maxCount);
  return this;
}

void
BasicBlockStack_Dtor(BasicBlockStack this)
{
  PtrSeq_Dtor(BasicBlockStack____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(BasicBlockStack_)\t%zu\n", sizeof(BasicBlockStack_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Iterate over this BasicBlockStack BasicBlock(s).
 */
#define BasicBlockStack_FOREACH_BasicBlock(this, basicBlock) { \
  const_PtrSeq BasicBlockStack____ = BasicBlockStack__const(this); \
  PtrSeq_FOREACH(BasicBlockStack____, BasicBlock, basicBlock) {
#define BasicBlockStack_ENDEACH_BasicBlock \
  } PtrSeq_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this BasicBlockStack BasicBlock(s).
 */
#define BasicBlockStack_FORBACK_BasicBlock(this, basicBlock) { \
  const_PtrSeq BasicBlockStack____ = BasicBlockStack__const(this); \
  PtrSeq_FORBACK(BasicBlockStack____, BasicBlock, basicBlock) {
#define BasicBlockStack_ENDBACK_BasicBlock \
  } PtrSeq_ENDBACK \
}
#endif//$XCC_h

#ifdef $XCC_h
#define BasicBlockStack_resize(this, maxCount) \
  PtrSeq_resize(BasicBlockStack__(this), maxCount)
#define BasicBlockStack_empty(this) \
  PtrSeq_empty(BasicBlockStack__(this))
#define BasicBlockStack_memory(this) \
  PtrSeq_memory(BasicBlockStack__const(this))
#define BasicBlockStack_base_(this) (BasicBlock *) \
  PtrSeq_base_(BasicBlockStack__(this))
#define BasicBlockStack_base(this) (const BasicBlock *) \
  PtrSeq_base(BasicBlockStack__const(this))
#define BasicBlockStack_past(this) (const BasicBlock *) \
  PtrSeq_past(BasicBlockStack__const(this))
#define BasicBlockStack_isEmpty(this) \
  PtrSeq_isEmpty(BasicBlockStack__const(this))
#define BasicBlockStack_isSingle(this) \
  PtrSeq_isSingle(BasicBlockStack__const(this))
#define BasicBlockStack_isFull(this) \
  PtrSeq_isFull(BasicBlockStack__const(this))
#define BasicBlockStack_count(this) \
  PtrSeq_count(BasicBlockStack__const(this))
#define BasicBlockStack_access(this, index) (BasicBlock) \
  PtrSeq_access(BasicBlockStack__const(this), index)
#define BasicBlockStack_bottom(this) (BasicBlock) \
  PtrSeq_bottom(BasicBlockStack__const(this))
#define BasicBlockStack_top(this) (BasicBlock) \
  PtrSeq_top(BasicBlockStack__const(this))
#define BasicBlockStack_pop(this) \
  PtrSeq_pop(BasicBlockStack__(this))
#define BasicBlockStack_push(this, basicBlock) \
  PtrSeq_push(BasicBlockStack__(this), basicBlock)
#define BasicBlockStack_push2(this, basicBlock) \
  PtrSeq_push2(BasicBlockStack__(this), basicBlock)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Sort this BasicBlockStack.
 *
 * See the en.wikibook.org heapsort.
 */
typedef bool (*BasicBlockLT)(const_BasicBlock, const_BasicBlock);
void
BasicBlockStack_sort(BasicBlockStack this, BasicBlockLT lt);
#endif//$XCC_h

void
BasicBlockStack_sort(BasicBlockStack this, BasicBlockLT lt)
{
  BasicBlock *base = BasicBlockStack_base_(this);
  int32_t count = BasicBlockStack_count(this);
  BasicBlock *past = base + count;
  int32_t n = count, i = count/2;
  while (true) {
    int32_t parent, child;
    BasicBlock temp;
    if (i > 0) {
      temp = base[--i];
    } else if (n > 1) {
      temp = base[--n];
      base[n] = base[0];
    } else return;
    parent = i, child = i*2 + 1;
    while (child < n) {
      if (   child + 1 < n
          && (lt)(base[child + 1], base[child])) {
        child = child + 1;
      }
      if ((lt)(base[child], temp)) {
        base[parent] = base[child];
        parent = child;
        child = child*2 + 1;
      } else break;
    }
    base[parent] = temp;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Specialization of IndexedTable for BasicBlock(s).
 */
struct BasicBlockTable_ {
  //@args	Memory memory, int32_t estimate
  IndexedTable_ __;
  //@access MEMORY	IndexedTable_memory(BasicBlockTable____(this))
  //@access COUNT	IndexedTable_count(BasicBlockTable____(this))
  //@access BASE	(const BasicBlock *)IndexedTable_base(BasicBlockTable____(this))
};
#endif//$XCC_h

#ifdef $XCC_h
static inline IndexedTable
BasicBlockTable__(BasicBlockTable this) {
  return BasicBlockTable____(this);
}
static inline const_IndexedTable
BasicBlockTable__const(const_BasicBlockTable this) {
  return BasicBlockTable____(this);
}
#endif//$XCC_h

BasicBlockTable
BasicBlockTable_Ctor(BasicBlockTable this,
                     Memory memory, int32_t estimate)
{
  IndexedTable_Ctor(BasicBlockTable____(this), memory, estimate);
  return this;
}

void
BasicBlockTable_Dtor(BasicBlockTable this)
{
  IndexedTable_Dtor(BasicBlockTable____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(BasicBlockTable_)\t%zu\n", sizeof(BasicBlockTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
#define BasicBlockTable_memory(this) \
  IndexedTable_memory(BasicBlockTable__const(this))
#define BasicBlockTable_count(this) \
  IndexedTable_count(BasicBlockTable__const(this))
#define BasicBlockTable_empty(this) \
  IndexedTable_empty(BasicBlockTable__(this))
#define BasicBlockTable_contains(this, basicBlock) \
  IndexedTable_contains(BasicBlockTable__const(this), BasicBlock__const(basicBlock))
#define BasicBlockTable_insert(this, basicBlock) \
  IndexedTable_insert(BasicBlockTable__(this), BasicBlock__(basicBlock))
#define BasicBlockTable_remove(this, basicBlock) \
  IndexedTable_remove(BasicBlockTable__(this), BasicBlock__const(basicBlock))
#define BasicBlockTable_removeHigh(this, basicBlock, high) \
  IndexedTable_removeHigh(BasicBlockTable__(this), BasicBlock__const(basicBlock), high)
#endif//$XCC_h

#ifdef $XCC_h
static inline const BasicBlock *
BasicBlockTable_base(const_BasicBlockTable this)
{
  return BasicBlockTable_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this BasicBlockTable BasicBlock(ies).
 */
#define BasicBlockTable_FOREACH_BasicBlock(this, basicBlock) { \
  IndexedTable_FOREACH_Indexed(BasicBlockTable__const(this), \
                                   BasicBlock, basicBlock) { \
    int32_t BasicBlockTable_BasicBlock_INDEX = IndexedTable_INDEX;
#define BasicBlockTable_ENDEACH_BasicBlock \
  } IndexedTable_ENDEACH_Indexed; \
}
#endif//$XCC_h

#ifdef $XCC_h
bool
BasicBlockTable_pretty(const_BasicBlockTable this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
BasicBlockTable_pretty(const_BasicBlockTable this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "{");
  BasicBlockTable_FOREACH_BasicBlock(this, basicBlock) {
    fprintf(file, " [%d]", BasicBlock_INDEX(basicBlock));
  } BasicBlockTable_ENDEACH_BasicBlock;
  fprintf(file, " }");
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Test if this is a back edge?
 */
extern bool
BasicBlockEdge_isBack(BasicBlockEdge edge);
#endif//$XCC_h

bool
BasicBlockEdge_isBack(BasicBlockEdge edge)
{
  BasicBlock destBB = BasicBlockEdge_destBlock(edge);
  BasicBlock origBB = BasicBlockEdge_origBlock(edge);
  LoopScope loopscope = BasicBlock_loopScope(destBB);
  return loopscope != NULL
      && LoopScope_headerBlock(loopscope) == destBB
      && BasicBlock_loopScope(origBB) == loopscope;
}

#ifdef $XCC_h
/*
 * this = (A,B)
 * check that B follows A in memory (codeRegion)
 */
extern bool
BasicBlockEdge_isFalse(BasicBlockEdge this, float *weight);
#endif//$XCC_h

bool
BasicBlockEdge_isFalse(BasicBlockEdge this, float *weight)
{
  BasicBlock bbA = BasicBlockEdge_origBlock(this);
  BasicBlock bbB = BasicBlockEdge_destBlock(this);
  if(weight != NULL) {
    *weight = BasicBlock_frequency(bbA) * BasicBlockEdge_probability(this);
  }
  return BasicBlock_next(bbA) == bbB;
}

#ifdef $XCC_h
/**
 * this=(A,B)
 * check that B has only one predecessor
 */
extern bool
BasicBlockEdge_isSweetBottom(BasicBlockEdge this, float *weight);
#endif//$XCC_h

bool
BasicBlockEdge_isSweetBottom(BasicBlockEdge this, float *weight)
{
  BasicBlock B=BasicBlockEdge_destBlock(this);
  BasicBlock_ENTER_FOREACH_BasicBlockEdge(B, edge) {
    if (edge!=this) return false;
  } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  if(weight != NULL) {
    *weight=BasicBlock_frequency(B);
  }
  return true;
}

#ifdef $XCC_h
/**
 * this=(A,B)
 * check if A has only one successor
 */
extern bool
BasicBlockEdge_isSweetTop(BasicBlockEdge this, float *weight);
#endif//$XCC_h

bool
BasicBlockEdge_isSweetTop(BasicBlockEdge this, float *weight)
{
  BasicBlock A=BasicBlockEdge_origBlock(this);
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(A, edge) {
    if (edge!=this) return false;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  if(weight != NULL) {
    *weight=BasicBlock_frequency(A);
  }
  return true;
}

#ifdef $XCC_h
/*
 * Check if an edge is bottom-weak
 * this@c E(A,B)
 * check that each incomming edge of B is SweetTop or false but E
 */
extern bool
BasicBlockEdge_isWeakBottom(BasicBlockEdge this, float *weight);
#endif//$XCC_h

bool
BasicBlockEdge_isWeakBottom(BasicBlockEdge this, float *weight)
{
  BasicBlock B;
  if(weight != NULL) {
    *weight=0;
  }
  if (!BasicBlockEdge_isCritical(this)) return false;
  B = BasicBlockEdge_destBlock(this);
  BasicBlock_ENTER_FOREACH_BasicBlockEdge(B, edge) {
    if (edge == this) {
     if(weight != NULL) {
       *weight += BasicBlock_frequency(B);
     }
    } else {
      float arcweight;
      if (   !BasicBlockEdge_isSweetTop(edge,&arcweight)
          && !BasicBlockEdge_isFalse(edge,&arcweight)) return false;
      if(weight != NULL) {
        *weight += arcweight;
      }
    }
  } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  return true;
}

#ifdef $XCC_h
/*
 * Same with Top
 * this@c E(A,B)
 * check that each outgoing edge of A is SweetBottom or false but E
 */
extern bool
BasicBlockEdge_isWeakTop (BasicBlockEdge this, float *weight);
#endif//$XCC_h

bool
BasicBlockEdge_isWeakTop (BasicBlockEdge this, float *weight)
{
  BasicBlock A;
  if(weight != NULL) {
    *weight=0;
  }
  if (!BasicBlockEdge_isCritical(this)) return false;
  A = BasicBlockEdge_origBlock(this);
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(A, edge) {
    if (edge == this) {
      if(weight != NULL) {
        *weight += BasicBlock_frequency(A);
      }
    } else {
      float arcweight;
      if (   !BasicBlockEdge_isSweetBottom(edge,&arcweight)
          && !BasicBlockEdge_isFalse(edge,&arcweight)) return false;
      if(weight != NULL) {
        *weight += arcweight;
      }
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  return true;
}

#ifdef $XCC_h
/**
 * Check if edge is critical.
 * 
 * An edge (A->B) is critical if A has more than one successor and B has
 * more than one predecessor.
 */
extern bool
BasicBlockEdge_isCritical(BasicBlockEdge this);
#endif//$XCC_h

bool
BasicBlockEdge_isCritical(BasicBlockEdge this)
{
  float devnull;
  return (!BasicBlockEdge_isSweetBottom(this,&devnull) &&
          !BasicBlockEdge_isSweetTop(this,&devnull));
}

#ifdef $XCC_h
/**
 * Check if edge is strong critical.
 *
 * An edge is strong if it is critical, it is neither a "false edge", nor a
 * "weak edge".
 *
 * \sa BasicBlockEdge_isCritical, BasicBlockEdge_isFalse, 
 * BasicBlockEdge_isWeakTop, BasicBlockEdge_isWeakBottom.
 */
extern bool
BasicBlockEdge_isStrong(BasicBlockEdge this);
#endif//$XCC_h

bool
BasicBlockEdge_isStrong(BasicBlockEdge this)
{
  float devnull;
  return BasicBlockEdge_isCritical(this)
      && !BasicBlockEdge_isFalse(this,&devnull)
      && !BasicBlockEdge_isWeakBottom(this,&devnull)
      && !BasicBlockEdge_isWeakTop(this,&devnull);
}

#ifdef $XCC_h
/**
 * Is this edge suitable for conditional parallel copy.
 */
extern bool
BasicBlockEdge_isConditional(BasicBlockEdge E);
#endif//$XCC_h

bool
BasicBlockEdge_isConditional(BasicBlockEdge E)
{
  BasicBlock orig = BasicBlockEdge_origBlock(E);
  Operation goto_operation =  BasicBlock_getControlOperation(orig);
  return goto_operation != NULL
      && Operation_isConditional(goto_operation);
}

static void
BasicBlock_reflectEnterEdgesOrder(BasicBlock this)
{
  int32_t argIndex;
  Except_CHECK(BasicBlock_ENTERCOUNT(this) > 0);
  if(BasicBlock_ENTERCOUNT(this) > 1) {
    for(argIndex = 0; argIndex < BasicBlock_ENTERCOUNT(this) - 1; ++argIndex) {
      BasicBlockEdge edge = BasicBlock_ENTEREDGES(this)[argIndex];
      *BasicBlockEdge__ENTERNEXT(edge) = BasicBlock_ENTEREDGES(this)[argIndex + 1];
    }
    *BasicBlockEdge__ENTERNEXT(BasicBlock_ENTEREDGES(this)[argIndex]) = NULL;
    *BasicBlock__ENTERFIRST(this) = BasicBlock_ENTEREDGES(this)[0];
  }
}

#ifdef $XCC_h
/*
 * this = E = (A,B). Create C on E
 */
extern BasicBlock
BasicBlockEdge_splitFalse(struct CodeRegion_* codeRegion, BasicBlockEdge this);
#endif//$XCC_h

BasicBlock
BasicBlockEdge_splitFalse(struct CodeRegion_* codeRegion, BasicBlockEdge this)
{
  float probAB = BasicBlockEdge_probability(this);
  BasicBlock bbA = BasicBlockEdge_origBlock(this);
  BasicBlock bbB = BasicBlockEdge_destBlock(this);
  float freqC = probAB * BasicBlock_frequency(bbA);
  // Create the new basic block on the critical edge
  BasicBlock bbC = CodeRegion_makeBasicBlock(codeRegion, freqC);
  int32_t enterIdx = BasicBlockEdge_ARGINDEX(this);
  BasicBlockEdge edgeBC;
  Except_CHECK(enterIdx >= 0);
  Except_CHECK(BasicBlock_ENTEREDGES(bbB)[enterIdx] == this);
  Except_CHECK(BasicBlockEdge_isFalse(this, NULL));
  BasicBlock_makeLabel(bbC); // Add a label to the basic block
  // Inserted new block between A and B
  Procedure_chainBasicBlocks(CodeRegion_procedure(codeRegion), bbA, bbC);
  //
  BasicBlock_killEdge(bbA, this);
  BasicBlock_makeEdge(bbA, bbC, probAB);
  edgeBC = BasicBlock_makeEdge(bbC, bbB, 1.0);
  // Update basic block edge information
  // TODO MERGE: It seems that this function discard the
  // codeRegion field of basic block. Is it ok to call it??
  if (CodeRegion_ssaForm(codeRegion) != NULL) {
    BasicBlock_makeEnterEdges(bbB, Procedure_memory(CodeRegion_procedure(codeRegion)), 0);
  }
  // update enter edges. Be careful to not break the list order, since we use it in PHI
  //BasicBlock_makeEnterEdges(bbB, Procedure_memory(CodeRegion_procedure(codeRegion)), 0);
  if(enterIdx != -1) {
    BasicBlock_ENTEREDGES(bbB)[enterIdx] = edgeBC;
  } else {
    Except_CHECK(BasicBlock_ENTEREDGES(bbB) == NULL);
    Except_CHECK(BasicBlock_ENTERFIRST(bbB) == edgeBC);
  }
  *BasicBlockEdge__ARGINDEX(edgeBC) = enterIdx;
  // To have a display that keeps PHI arguments order
  BasicBlock_reflectEnterEdgesOrder(bbB);
  return bbC;
}

#ifdef $XCC_h
/*
 * this = E = (A,B). Create C on E
 */
extern BasicBlock
BasicBlockEdge_splitCritical(struct CodeRegion_* codeRegion, BasicBlockEdge this);
#endif//$XCC_h

BasicBlock
BasicBlockEdge_splitCritical(struct CodeRegion_* codeRegion, BasicBlockEdge this)
{
  Operation branch;
  BasicBlock bbA = BasicBlockEdge_origBlock(this);
  BasicBlock bbB = BasicBlockEdge_destBlock(this);
  float probAB = BasicBlockEdge_probability(this);
  float freqC = probAB * BasicBlock_frequency(bbA);
  int32_t enterIdx = BasicBlockEdge_ARGINDEX(this);
  BasicBlockEdge edgeBC;
  //
  // Create the new basic block on the critical edge.
  // The basic block will be inserted last in the linear block list.
  BasicBlock bbC = CodeRegion_makeBasicBlock(codeRegion, freqC);
  Label labelC = BasicBlock_makeLabel(bbC);
  //
  // Insert a branch from bbC to bbB
  Label labelB = BasicBlock_makeLabel(bbB);
  Except_CHECK(enterIdx >= 0);
  Except_CHECK(BasicBlock_ENTEREDGES(bbB)[enterIdx] == this);
  //
  Selector_selectGOTO(BasicBlock_selector(bbC), labelB);
  //
  // Reroute bbA branch to bbC
  branch = BasicBlock_getControlOperation(bbA);
  Except_CHECK(branch != NULL);
  Except_CHECK(!BasicBlockEdge_isFalse(this, NULL) && "no target to update");
  Procedure_updateTargetedOperation(CodeRegion_procedure(codeRegion), branch, labelC);
    //
  BasicBlock_killEdge(bbA, this);
  BasicBlock_makeEdge(bbA, bbC, probAB);
  edgeBC = BasicBlock_makeEdge(bbC, bbB, 1.0);
  //
  // Update basic block edge information
  // TODO MERGE: It seems that this function discard the
  // codeRegion field of basic block. Is it ok to call it??
  BasicBlock_makeEnterEdges(bbC, Procedure_memory(CodeRegion_procedure(codeRegion)), 0);
  //BasicBlock_makeEnterEdges(bbB, Procedure_memory(CodeRegion_procedure(codeRegion)), 0);
  if(enterIdx != -1) {
    BasicBlock_ENTEREDGES(bbB)[enterIdx] = edgeBC;
  } else {
    Except_CHECK(BasicBlock_ENTEREDGES(bbB) == NULL);
    Except_CHECK(BasicBlock_ENTERFIRST(bbB) == edgeBC);
  }
  *BasicBlockEdge__ARGINDEX(edgeBC) = enterIdx;
  // To have a display that keeps PHI arguments order
  BasicBlock_reflectEnterEdgesOrder(bbB);
  //
  return bbC;
}

#ifdef $XCC_h
/**
 * Split edge and perform action on new basic block.
 *
 * This splits an edge A->B by inserting a new basic block C so that the flow
 * becomes A->C->B.
 *
 * @return C, the new basic block.
 *
 * @note Splitting is not mandatory when:
 * - destination block has only one predecessor
 * - or source block has only one successor and edge is not false (goto)
 *   or control operation is NULL
 *
 * If @p action is not @c NULL, it is applied to C, hence the @p action
 * function passed must take a BasicBlock as first argument.
 *
 * If splitting is unnecessary (see below), the second argument of @p action is
 * used as the @p point (operation) after which it is safe to insert code that
 * should be on C.
 *
 * Whenever @p point is @c NULL, the insertion point is first in given basic
 * block.
 *
 * Third argument of @p action is a pointer on anything. Use it to pass whatever
 * you want. To fill it, put the related thing in structParam argument
 * "point" argument of splitAndAction will contain finding insertion point.
 * If you are not interessed in this information, put a NULL pointer.
 *
 * @param canReuseBB @c true if an already existing basicBlock can be used
 * when splitting is not mandatory.
 *
 */
extern BasicBlock
BasicBlockEdge_splitAndAction(BasicBlockEdge edge, bool canReuseBB, 
                              Operation *point, void* structParam,
                              void (*action)(BasicBlock, Operation, void*));
#endif//$XCC_h

BasicBlock
BasicBlockEdge_splitAndAction(BasicBlockEdge edge, bool canReuseBB, 
                              Operation *point, void* structParam,
                              void (*action)(BasicBlock, Operation, void*)) {
  BasicBlock destinationBlock = BasicBlockEdge_destBlock(edge);
  // NULL means begin of the basic block
  Operation pointForInsertionAfter = NULL;
  if(canReuseBB && (!BasicBlockEdge_isFalse(edge, NULL)
                    || BasicBlock_getControlOperation(BasicBlockEdge_origBlock(edge)) == NULL)
     && BasicBlock_leaveCount(BasicBlockEdge_origBlock(edge)) == 1
     && BasicBlock_enterCount(destinationBlock) != 1) {
    // edge is not false || no control operation -> control operation is not call
    // orig's leave count == 1 && dest's enter count != 1 ->
    // control operation is a goto. So it is stupid to split the edge whereas
    // we can put code in orig block with a gain on scheduling
    destinationBlock = BasicBlockEdge_origBlock(edge);
    pointForInsertionAfter = BasicBlock_lastOperation(destinationBlock);
    if(pointForInsertionAfter != NULL
       && Operator_isControl(Operation_operator(pointForInsertionAfter))) {
      pointForInsertionAfter = Operation_prev(pointForInsertionAfter);
    }
  } else {
    if(BasicBlock_enterCount(destinationBlock) != 1 || !canReuseBB) {
      // Create a new basic block between them.
      // Update jump origin (done be split call)
      // Create jump to destination in new basic block (done be split call)
      if(BasicBlockEdge_isFalse(edge, NULL)) {
        destinationBlock =
          BasicBlockEdge_splitFalse(BasicBlock_codeRegion(destinationBlock), edge);
      } else {
        destinationBlock =
          BasicBlockEdge_splitCritical(BasicBlock_codeRegion(destinationBlock), edge);
      }
    }
  }
  if(action != NULL) {
    action(destinationBlock, pointForInsertionAfter, structParam);
  }
  if(point != NULL) {
    *point = pointForInsertionAfter;
  }
  return destinationBlock;
}

#ifdef $XCC_h
/*
 * BasicBlockEdge_split(BasicBlockEdge edge)  -- split the edge if the destination has several predecessors
 */
extern void
BasicBlockEdge_split(BasicBlockEdge edge);
#endif//$XCC_h

void
BasicBlockEdge_split(BasicBlockEdge edge)
{
  BasicBlock destinationBlock = BasicBlockEdge_destBlock(edge);
  CodeRegion codeRegion = BasicBlock_codeRegion(destinationBlock);
  if(BasicBlock_enterCount(destinationBlock) != 1) {
    if(BasicBlockEdge_isFalse(edge, NULL)) {
      destinationBlock = BasicBlockEdge_splitFalse(codeRegion, edge);
    } else {
      destinationBlock = BasicBlockEdge_splitCritical(codeRegion, edge);
    }
  }
}

#ifdef $XCC_h
// Applies BasicBlockEdge_splitAndAction to each leave edge
// see BasicBlockEdge_splitAndAction
extern void
BasicBlock_splitLeaveEdgeAndAction(BasicBlock block, bool canReuseBB,
                                   void* structParam,
                                   void (*action)(BasicBlock, Operation, void*));
#endif//$XCC_h

void
BasicBlock_splitLeaveEdgeAndAction(BasicBlock block, bool canReuseBB,
                                   void* structParam,
                                   void (*action)(BasicBlock, Operation, void*))
{
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
    BasicBlockEdge_splitAndAction(edge, canReuseBB, NULL, structParam, action);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize BasicBlock.
 */
void
BasicBlock_INIT(void);
#endif//$XCC__h

IndexedFactory_ BasicBlock_Factory[1];

void
BasicBlock_INIT(void)
{
  IndexedFactory_Ctor(BasicBlock_Factory);
#ifndef _NTRACE
  ETable_Ctor(BasicBlockFlag_Table, Memory_Root, BasicBlockFlagEntries);
#endif//_NTRACE
}

#ifdef $XCC__h
/**
 * Finalize BasicBlock.
 */
#ifndef _NTRACE
void
BasicBlock_FINI(void);
#else//_NTRACE
#define BasicBlock_FINI()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
BasicBlock_FINI(void)
{
  ETable_Dtor(BasicBlockFlag_Table);
}
#endif//_NTRACE

#if XCC__C
static void
BasicBlock_TEST(void)
{
#include "LIR/BasicBlock_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(BasicBlock_TEST);
  return 0;
}
#endif

