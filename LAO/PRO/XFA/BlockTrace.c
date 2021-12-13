#ifdef $XCC_h
/*
 * !!!!	BlockTrace.xcc
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
 * @brief Basic Block Trace (to be replaced).
 */
#endif//$XCC_h


#include "XFA/XFA_.h"

#ifdef $XCC__h
#include "XFA/BlockTrace.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the BlockTrace flags.
 */
typedef enum {
  BlockTraceFlag_InnerLoop = 0x1,	// This BlockTrace is an inner loop body.
  BlockTraceFlag_PrePass = 0x10	// This BlockTrace was made in pre-pass context.
} enum_BlockTraceFlag;
typedef uint8_t BlockTraceFlags;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Region for instruction scheduling.
 *
 * A BlockTrace results from the partition of a RegionFlow for the purposes
 * of IF-conversion, instruction scheduling, register allocation, or basic block alignment.
 * Operation maps, that use Operation_index to index arrays, are defined relative
 * to a particular BlockTrace.
 */
struct BlockTrace_ {
  //@args	Memory memory, BlockTraceFlags traceFlags
  BlockTraceFlags FLAGS;		// This BlockTrace flags.
  //@access isInnerLoop	((BlockTrace_FLAGS(this) & BlockTraceFlag_InnerLoop) != 0)
  //@access isAcyclic	((BlockTrace_FLAGS(this) & BlockTraceFlag_InnerLoop) == 0)
  //@access isPrePass	((BlockTrace_FLAGS(this) & BlockTraceFlag_PrePass) != 0)
  struct RegionFlow_ *REGIONFLOW;	// Encloding RegionFlow.
  //@access PROCEDURE	RegionFlow_procedure(BlockTrace_REGIONFLOW(this))
  //@access CODEREGION	RegionFlow_codeRegion(BlockTrace_REGIONFLOW(this))
  //@access PROCESSOR	Procedure_processor(BlockTrace_PROCEDURE(this))
  //@access CONVENTION	CodeRegion_convention(BlockTrace_CODEREGION(this))
  //@access OPTIMIZE	CodeRegion_optimize(BlockTrace_CODEREGION(this))
  CFGNode HEADNODE;		// Head CFGNode in this BlockTrace.
  CFGNode TAILNODE;		// Tail CFGNode in this BlockTrace.
  BasicBlock HEADBLOCK;		// Head BasicBlock in this BlockTrace.
  BasicBlock TAILBLOCK;		// Tail BasicBlock in this BlockTrace.
  PtrSet_ CFGNODES_;		// This BlockTrace CFGNodes.
  //@access CFGNODES	BlockTrace__CFGNODES_(this)
  //@access NODECOUNT 	PtrSet_count(BlockTrace_CFGNODES(this))
  IssueItem FIRSTITEM;		// FIRST IssueItem of this BlockTrace.
  IssueItem STARTITEM;		// START IssueItem of this BlockTrace.
  IssueItem STOPITEM;		// STOP IssueItem of this BlockTrace.
  IssueItem LASTITEM;		// LAST IssueItem of this BlockTrace.
  IDList_ ISSUEITEMS_;		// This BlockTrace IssueItem list.
  //@access ISSUEITEMS	BlockTrace__ISSUEITEMS_(this)
  //@access ITEMCOUNT	IDList_count(BlockTrace_ISSUEITEMS(this))
  //@access MEMORY	IDList_memory(BlockTrace_ISSUEITEMS(this))
  PtrSet_ DDGNODES_;		// This BlockTrace Dependence nodes.
  //@access DDGNODES	BlockTrace__DDGNODES_(this)
  LoopScope LOOPSCOPE;		// The LoopScope for this BlockTrace.
  EffectTable EFFECTTABLE;	// The EffectTable of this BlockTrace.
  LifetimeTable LIFETIMETABLE;	// The LifetimeTable of this BlockTrace.
  OptimizeRCMSSolving SOLVING;
  uint16_t TIMEOUT;	// Time-out value for integer linear programming.
  int16_t LAMBDA;	// Loop initiation interval after loop scheduling or pipelining.
  int16_t LENGTH;	// Loop iteration length after loop scheduling or pipelining.
  int8_t FACTOR;	// The power of two unroll factor that satisfies kernel unrolling.
  int8_t KUNROLL;	// Kernel unrolling degree (non-zero only when loop is pipelined).
  Configure_ CONFIGURE_;	// Local copy of LOOPSCOPE CONFIGURE.
  //@access CONFIGURE	BlockTrace__CONFIGURE_(this)
  //@access COMPENSATION	Configure_COMPENSATION(BlockTrace_CONFIGURE(this))
  //@mutate COMPENSATION	Configure__COMPENSATION(BlockTrace_CONFIGURE(this))
  //@access SPECULATION	Configure_SPECULATION(BlockTrace_CONFIGURE(this))
  //@mutate SPECULATION	Configure__SPECULATION(BlockTrace_CONFIGURE(this))
  //@access RELAXATION	Configure_RELAXATION(BlockTrace_CONFIGURE(this))
  //@mutate RELAXATION	Configure__RELAXATION(BlockTrace_CONFIGURE(this))
  //@access RENAMING	Configure_RENAMING(BlockTrace_CONFIGURE(this))
  //@mutate RENAMING	Configure__RENAMING(BlockTrace_CONFIGURE(this))
  //@access BOOSTING	Configure_BOOSTING(BlockTrace_CONFIGURE(this))
  //@mutate BOOSTING	Configure__BOOSTING(BlockTrace_CONFIGURE(this))
  //@access ALIASING	Configure_ALIASING(BlockTrace_CONFIGURE(this))
  //@mutate ALIASING	Configure__ALIASING(BlockTrace_CONFIGURE(this))
  //@access PIPELINING	Configure_PIPELINING(BlockTrace_CONFIGURE(this))
  //@mutate PIPELINING	Configure__PIPELINING(BlockTrace_CONFIGURE(this))
  //@access PRELOADING	Configure_PRELOADING(BlockTrace_CONFIGURE(this))
  //@mutate PRELOADING	Configure__PRELOADING(BlockTrace_CONFIGURE(this))
  //@access L1MISSEXTRA	Configure_L1MISSEXTRA(BlockTrace_CONFIGURE(this))
  //@mutate L1MISSEXTRA	Configure__L1MISSEXTRA(BlockTrace_CONFIGURE(this))
  //@access OVERLAP	Configure_OVERLAP(BlockTrace_CONFIGURE(this))
  //@access isPipeline	(BlockTrace_PIPELINING(this) > 0)
};
#endif//$XCC_h

BlockTrace
BlockTrace_Ctor(BlockTrace this,
                Memory memory, BlockTraceFlags traceFlags)
{
  *BlockTrace__FLAGS(this) = traceFlags;
  *BlockTrace__REGIONFLOW(this) = NULL;
  *BlockTrace__HEADNODE(this) = NULL;
  *BlockTrace__TAILNODE(this) = NULL;
  *BlockTrace__HEADBLOCK(this) = NULL;
  *BlockTrace__TAILBLOCK(this) = NULL;
  PtrSet_Ctor(BlockTrace_CFGNODES(this), memory, 0);
  *BlockTrace__FIRSTITEM(this) = NULL;
  *BlockTrace__STARTITEM(this) = NULL;
  *BlockTrace__STOPITEM(this) = NULL;
  *BlockTrace__LASTITEM(this) = NULL;
  IDList_Ctor(BlockTrace_ISSUEITEMS(this), memory);
  PtrSet_Ctor(BlockTrace_DDGNODES(this), memory, 256);
  *BlockTrace__LOOPSCOPE(this) = NULL;
  *BlockTrace__EFFECTTABLE(this) = NULL;
  *BlockTrace__LIFETIMETABLE(this) = NULL;
  *BlockTrace__SOLVING(this) = 0;
  *BlockTrace__TIMEOUT(this) = 0;
  *BlockTrace__LAMBDA(this) = 0;
  *BlockTrace__LENGTH(this) = 0;
  *BlockTrace__FACTOR(this) = 0;
  *BlockTrace__KUNROLL(this) = -1;
  return this;
}

void
BlockTrace_Dtor(BlockTrace this)
{
  PtrSet_Dtor(BlockTrace_CFGNODES(this));
  IDList_empty(BlockTrace_ISSUEITEMS(this),
               (IDListItemRelease)IssueItem_Dtor);
  IDList_Dtor(BlockTrace_ISSUEITEMS(this));
  PtrSet_Dtor(BlockTrace_DDGNODES(this));
  EffectTable_delete(BlockTrace_EFFECTTABLE(this));
  LifetimeTable_delete(BlockTrace_LIFETIMETABLE(this));
}

size_t
BlockTrace_Size(Memory memory, BlockTraceFlags traceFlags)
{
  return sizeof(BlockTrace_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(BlockTrace_)\t%zu\n", sizeof(BlockTrace_));
}
#endif//$XCC__c

#ifdef $XCC__h
int
BlockTrace_compare(const void *this_pointer, const void *that_pointer);
#endif//$XCC__h

int
BlockTrace_compare(const void *this_pointer, const void *that_pointer)
{
  const_BlockTrace this = this_pointer;
  const_BlockTrace that = that_pointer;
  LoopScope this_scope = BlockTrace_LOOPSCOPE(this);
  LoopScope that_scope = BlockTrace_LOOPSCOPE(that);
  int result = LoopScope_nesting(that_scope) - LoopScope_nesting(this_scope);
  if (result == 0 && this != that) {
    CFGNode this_headNode = BlockTrace_HEADNODE(this);
    CFGNode that_headNode = BlockTrace_HEADNODE(that);
    BasicBlock this_basicBlock = CFGNode_basicBlock(this_headNode);
    BasicBlock that_basicBlock = CFGNode_basicBlock(that_headNode);
    intptr_t this_index = BasicBlock_index(this_basicBlock);
    intptr_t that_index = BasicBlock_index(that_basicBlock);
    return (this_index < that_index) - (this_index > that_index);
  }
  return result;
}

#ifdef $XCC_h
/**
 * Create a BlockTrace containing a single basic block
 */
BlockTrace
BlockTrace_createSingleton(Memory memory, RegionFlow regionFlow, CFGNode cfgNode,
                           LoopScope loopScope, BlockTraceFlags traceFlags);
#endif//$XCC_h

BlockTrace
BlockTrace_createSingleton(Memory memory, RegionFlow regionFlow, CFGNode cfgNode,
                           LoopScope loopScope, BlockTraceFlags traceFlags)
{
  size_t size = BlockTraceSize(memory, traceFlags);
  BasicBlock basicBlock = CFGNode_BASICBLOCK(cfgNode);
  BlockTrace blockTrace = Memory_alloc(memory, size);
  EntryCode(blockTrace, memory, traceFlags);
  BlockTraceGrow(blockTrace, cfgNode);
  BlockTraceStop(blockTrace, regionFlow, loopScope);
  return blockTrace;
}

#ifdef $XCC_h
/**
 * This BlockTrace RegionFlow
 */
static inline RegionFlow
BlockTrace_regionFlow(BlockTrace this)
{
  return BlockTrace_REGIONFLOW(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by BlockTrace_FOREACH_CFGNode.
 */
static inline CFGNode
BlockTrace_headNode(BlockTrace this)
{
  return BlockTrace_HEADNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this BlockTrace CFGNodes.
 */
#define BlockTrace_FOREACH_CFGNode(this, node) { \
  CFGNode BlockTrace_HEADNODE = BlockTrace_headNode(this); \
  CFGNode BlockTrace_NODE = BlockTrace_HEADNODE; \
  while (BlockTrace_NODE != NULL) { \
    CFGNode node = BlockTrace_NODE; \
    BlockTrace_NODE = CFGNode_nextNode(BlockTrace_NODE);
#define BlockTrace_ENDEACH_CFGNode \
    if (BlockTrace_NODE == BlockTrace_HEADNODE) break; \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The CFGNode set of this BlockTrace.
 */
static inline PtrSet
BlockTrace_cfgNodes(BlockTrace this)
{
  return BlockTrace_CFGNODES(this);
}
#endif//$XCC_h

#ifdef $XCC_h
#define BlockTrace_contains(this, node) \
  PtrSet_contains(BlockTrace_cfgNodes(this), node)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by BlockTrace_FOREACH_IssueItem.
 */
static inline IDList
BlockTrace_issueItems(BlockTrace this)
{
  return BlockTrace_ISSUEITEMS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this BlockTrace IssueItems.
 */
#define BlockTrace_FOREACH_IssueItem(this, issueItem) \
  IDList_FOREACH(BlockTrace_issueItems(this), IssueItem_, issueItem) {
#define BlockTrace_ENDEACH_IssueItem \
  } IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this BlockTrace IssueItems.
 */
#define BlockTrace_FORBACK_IssueItem(this, issueItem) \
  IDList_FORBACK(BlockTrace_issueItems(this), IssueItem_, issueItem) {
#define BlockTrace_ENDBACK_IssueItem \
  } IDList_ENDBACK;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pretty-print this BlockTrace IssueItems.
 */
bool
BlockTrace_prettyIssueItems(BlockTrace this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
BlockTrace_prettyIssueItems(BlockTrace this, FILE *file)
{
  if (file == NULL) file = stderr;
  BlockTrace_FOREACH_IssueItem(this, item) {
    int controlId = IssueItem_CONTROLID(item);
    int date = IssueItem_STARTDATE(item);
    fprintf(file, "    [%d] %d\t", date, controlId);
    IssueItem_pretty(item, file);
  } BlockTrace_ENDEACH_IssueItem;
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Pretty-print this BlockTrace.
 */
bool
BlockTrace_pretty(BlockTrace this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
BlockTrace_pretty(BlockTrace this, FILE *file)
{
  unsigned flags = BlockTrace_FLAGS(this);
  if (file == NULL) file = stderr;
  if (BlockTrace_isInnerLoop(this)) {
    fprintf(file, "InnerLoop\t");
  }
  BlockTrace_FOREACH_CFGNode(this, node) {
    int32_t numbering = CFGNode_NUMBERING(node);
    BasicBlock block = CFGNode_basicBlock(node);
    if (block != NULL) {
      fprintf(file, "%d:Block_%d", BasicBlock_identity(block), CFGNode_INDEX(node));
    } else {
      fprintf(file, "node_%d", CFGNode_INDEX(node));
    }
    fprintf(file, "[%d]\t", numbering);
  } BlockTrace_ENDEACH_CFGNode;
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

#ifdef $XCC__h
/**
 * Call-back for BlockTrace_SuperBlocks.
 */
size_t
BlockTraceSize(Memory memory, BlockTraceFlags traceFlags);
#endif//$XCC__h

size_t
BlockTraceSize(Memory memory, BlockTraceFlags traceFlags)
{
  return BlockTrace_Size(memory, traceFlags);
}

#ifdef $XCC__h
/**
 * Call-back for BlockTrace_SuperBlocks.
 */
void
EntryCode(BlockTrace this, Memory memory, BlockTraceFlags traceFlags);
#endif//$XCC__h

void
EntryCode(BlockTrace this, Memory memory, BlockTraceFlags traceFlags)
{
  BlockTrace_Ctor(this, memory, traceFlags);
}

#ifdef $XCC__h
/**
 * Call-back for BlockTrace_SuperBlocks.
 */
bool
BlockTraceGrow(BlockTrace this, CFGNode cfgNode);
#endif//$XCC__h

bool
BlockTraceGrow(BlockTrace this, CFGNode cfgNode)
{
  CFGNode lastNode = BlockTrace_TAILNODE(this);
  *CFGNode__NEXTNODE(cfgNode) = NULL;
  *CFGNode__PREVNODE(cfgNode) = NULL;
  if (lastNode == NULL) {
    // This is the first cfgNode added to this BlockTrace.
    *BlockTrace__HEADNODE(this) = cfgNode;
    *CFGNode__NUMBERING(cfgNode) = 0;
  } else {
    // Adding another cfgNode to this BlockTrace.
    *CFGNode__NEXTNODE(lastNode) = cfgNode;
    *CFGNode__NUMBERING(cfgNode) = CFGNode_NUMBERING(lastNode) + 1;
  }
  *BlockTrace__TAILNODE(this) = cfgNode;
  PtrSet_insert(BlockTrace_CFGNODES(this), cfgNode);
  return true;
}

#ifdef $XCC__h
/**
 * Call-back for BlockTrace_SuperBlocks.
 */
void
BlockTraceStop(BlockTrace this, RegionFlow regionFlow, LoopScope loopScope);
#endif//$XCC__h

void
BlockTraceStop(BlockTrace this, RegionFlow regionFlow, LoopScope loopScope)
{
  CFGNode prev_node = NULL;
  int nodeCount = BlockTrace_NODECOUNT(this);
  // Refine this BlockTrace REGIONTYPE and FLAGS.
  CodeRegion codeRegion = RegionFlow_codeRegion(regionFlow);
  Configure_Copy(BlockTrace_CONFIGURE(this), LoopScope_configure(loopScope));
  Configure_merge(BlockTrace_CONFIGURE(this), CodeRegion_configure(codeRegion));
  if (nodeCount == LoopScope_blockCount(loopScope)) {
    *BlockTrace__FLAGS(this) |= BlockTraceFlag_InnerLoop;
    prev_node = BlockTrace_TAILNODE(this);
  } else {
    *BlockTrace__FLAGS(this) &= ~BlockTraceFlag_InnerLoop;
  }
  *BlockTrace__REGIONFLOW(this) = regionFlow;
  *BlockTrace__LOOPSCOPE(this) = loopScope;
  // Set the CFGNode PREV and NEXT fields, and fill CFGNODES.
  BlockTrace_FOREACH_CFGNode(this, node) {
    BasicBlock basicBlock = CFGNode_basicBlock(node);
    intptr_t traceId = CodeRegion_makeTraceId(codeRegion);
    BasicBlock_setTraceId(basicBlock, traceId);
    if (prev_node != NULL) {
      *CFGNode__PREVNODE(node) = prev_node;
      *CFGNode__NEXTNODE(prev_node) = node;
    }
    prev_node = node;
  } BlockTrace_ENDEACH_CFGNode;
#ifdef XCC__C
  BlockTrace_pretty(this, stdout);
#endif
  // Complete this BlockTrace.
  BlockTrace_complete(this);
}

#ifdef $XCC__h
/**
 * Complete this BlockTrace.
 */
void
BlockTrace_complete(BlockTrace this);
#endif//$XCC__h

void
BlockTrace_complete(BlockTrace this)
{
  LoopScope loopScope = BlockTrace_LOOPSCOPE(this);
  Processor processor = BlockTrace_PROCESSOR(this);
  Optimize optimize = BlockTrace_OPTIMIZE(this);
  unsigned solving = Optimize_RCMSSOLVING(optimize);
  unsigned timeOut = 1<<Optimize_LOGTIMEOUT(optimize);
  bool isPrePass = BlockTrace_isPrePass(this);
  bool isInnerLoop = BlockTrace_isInnerLoop(this);
  int compensation = LoopScope_COMPENSATION(loopScope);
  int speculation = LoopScope_SPECULATION(loopScope);
  int pipelining = LoopScope_PIPELINING(loopScope);
  int renaming = LoopScope_RENAMING(loopScope), factor = 0;
  if (pipelining >= 4) {
    // Software pipelining levels between 4 and 7: override Optimize_RCMSSOLVING.
    solving |= (OptimizeRCMSSolving_Dinechin|OptimizeRCMSSolving_PostPass);
    solving &= ~(OptimizeRCMSSolving_LNSScan1|OptimizeRCMSSolving_LNSScan2);
    if (pipelining == 4) solving |= OptimizeRCMSSolving_LNSScan1;
    if (pipelining == 5) solving |= OptimizeRCMSSolving_LNSScan2;
    if (pipelining == 6) solving |= OptimizeRCMSSolving_LNSScan1|OptimizeRCMSSolving_LNSScan2;
    pipelining = 4;
  }
  if (isPrePass) {
    *BlockTrace__FLAGS(this) |= BlockTraceFlag_PrePass;
    if (isInnerLoop && !LoopScope_isRemainder(loopScope)) {
      // Pre-pass scheduling of an inner loop body.
      Temporary tripCount = LoopScope_tripCount(loopScope);
      if (LoopScope_isHardware(loopScope) && tripCount != NULL) {
        if (Temporary_isAbsolute(tripCount)) {
          // Constant loop trip count, get its lowest power of 2.
          Literal trip_invariant = Temporary_LITERAL(tripCount);
          unsigned value = (unsigned)Literal_VALUE(trip_invariant);
          factor = value != 0? (value & -value): 1;
        } else {
          // Find how much power of 2 is left available for kernel unrolling.
          int modulus = LoopScope_TRIPMODULUS(loopScope);
          int residue = LoopScope_TRIPRESIDUE(loopScope);
          int unrolled = LoopScope_unrolled(loopScope);
          if (unrolled == 0) unrolled = 1;
          if (residue == 0 && modulus > 1 && modulus%unrolled == 0) {
            modulus /= unrolled;
            factor = modulus & -modulus;
          } else factor = 1;
        }
        // Shift factor until lower than or equal to renaming.
        while (factor > renaming) factor >>= 1; //FIXME! This is wrong.
        if (factor != 0) renaming = factor;
        else factor = 1;
      } else {
        // Not a hardware counted loop: no need to constrain modulo renaming.
      }
      if (pipelining < 1) {
        // No software pipelining: disable modulo renaming and kernel unrolling.
        renaming = factor = 0;
      }
    } else {
      // Pre-pass scheduling of non pipelinable loop.
      pipelining = renaming = factor = 0;
    }
  } else {
    // Post-pass scheduling.
    pipelining = renaming = factor = 0;
    if (compensation > FORCE_POSTCOMP) compensation = FORCE_POSTCOMP;
    if (speculation > FORCE_POSTSPEC) speculation = FORCE_POSTSPEC;
  }
  // Store the configured values.
  *BlockTrace__SOLVING(this) = solving;
  *BlockTrace__TIMEOUT(this) = timeOut;
  *BlockTrace__COMPENSATION(this) = compensation;
  *BlockTrace__SPECULATION(this) = speculation;
  *BlockTrace__PIPELINING(this) = pipelining;
  *BlockTrace__RENAMING(this) = renaming;
  *BlockTrace__FACTOR(this) = factor;
}

#ifdef $XCC_h
void
BlockTrace_disablePipelining(BlockTrace this);
#endif//$XCC_h

void
BlockTrace_disablePipelining(BlockTrace this)
{
  *BlockTrace__PIPELINING(this) = 0;
  *BlockTrace__RENAMING(this) = 0;
  *BlockTrace__FACTOR(this) = 0;
  PFA_TRACE && fprintf(PFA_TRACE, "*** DISABLE\tpipelining=%d\trenaming=%d\n",
                                  BlockTrace_PIPELINING(this), BlockTrace_RENAMING(this));
}

#ifdef $XCC_h
/**
 * True if this BlockTrace has no Operations.
 */
bool
BlockTrace_isEmpty(BlockTrace this);
#endif//$XCC_h

bool
BlockTrace_isEmpty(BlockTrace this)
{
  BlockTrace_FOREACH_CFGNode(this, cfgNode) {
    BasicBlock basicBlock = CFGNode_basicBlock(cfgNode);
    if (basicBlock != NULL && !BasicBlock_isEmpty(basicBlock)) return false;
  } BlockTrace_ENDEACH_CFGNode;
  return true;
}

#ifdef $XCC_h
/**
 * Get the frequency this BlockTrace is entered.
 */
float
BlockTrace_getEnterFrequency(BlockTrace this);
#endif//$XCC_h

float
BlockTrace_getEnterFrequency(BlockTrace this)
{
  float frequency = 0.0;
  CFGNode prev_node = NULL;
  PtrSet cfgNodes = BlockTrace_CFGNODES(this);
  BlockTrace_FOREACH_CFGNode(this, cfgNode) {
    CFGNode_FORENTER_CFGArc(cfgNode, arc) {
      CFGNode tail_node = CFGArc_TAIL(arc);
      BasicBlock tail_block = CFGNode_basicBlock(tail_node);
      if (tail_block != NULL && tail_node != prev_node &&
          !PtrSet_contains(cfgNodes, tail_node)) {
        frequency += BasicBlock_frequency(tail_block);
      }
    } CFGNode_ENDENTER_CFGArc;
    prev_node = cfgNode;
  } BlockTrace_ENDEACH_CFGNode;
  return frequency;
}

#ifdef $XCC_h
/**
 * Get the probalility this BlockTrace loops.
 */
float
BlockTrace_getLoopProbalility(BlockTrace this);
#endif//$XCC_h

float
BlockTrace_getLoopProbalility(BlockTrace this)
{
  float probability = 0.0;
  CFGNode headNode = BlockTrace_HEADNODE(this);
  CFGNode_FORENTER_CFGArc(headNode, arc) {
    CFGNode tail_node = CFGArc_TAIL(arc);
    if (BlockTrace_contains(this, tail_node)) {
      probability += CFGArc_PROBABILITY(arc);
    }
  } CFGNode_ENDENTER_CFGArc;
  return probability;
}

#ifdef $XCC_h
/**
 * Return this BlockTrace DependenceTable.
 */
const_DependenceTable
BlockTrace_getDependenceTable(const_BlockTrace this);
#endif//$XCC_h

const_DependenceTable
BlockTrace_getDependenceTable(const_BlockTrace this)
{
  LoopScope loopScope = BlockTrace_LOOPSCOPE(this);
  LoopScope parent_scope = LoopScope_parent(loopScope);
  const_DependenceTable dependenceTable = LoopScope_dependenceTable(loopScope);
  while (   parent_scope != NULL
         && dependenceTable == NULL) {
    // Improper loops are detected at a coarser granularity in the Open64.
    // So visit the parent LoopScope(s) (/bugs/st200/pro-release-1-3-0-B/15).
    dependenceTable = LoopScope_dependenceTable(parent_scope);
    parent_scope = LoopScope_parent(parent_scope);
  }
  return dependenceTable;
}

#ifdef $XCC_h
/**
 * Normalize this BlockTrace control-flow.
 * Return:	True if middle CFGNodes were discarded.
 */
bool
BlockTrace_normalizeRegionFlow(BlockTrace this);
#endif//$XCC_h

bool
BlockTrace_normalizeRegionFlow(BlockTrace this)
{
  bool isPrePass = BlockTrace_isPrePass(this), discarded = false;
  Procedure procedure = BlockTrace_PROCEDURE(this);
  CFGNode headNode = BlockTrace_HEADNODE(this);
  CFGNode tailNode = BlockTrace_TAILNODE(this);
  CFGNode cfgNode = NULL, prev_node = NULL;
  BasicBlock headBlock = NULL, tailBlock = NULL, prev_block = NULL;
  BlockTrace_FOREACH_CFGNode(this, cfgNode) {
    BasicBlock basicBlock = CFGNode_basicBlock(cfgNode);
    if (basicBlock != NULL && prev_block != NULL) {
      // Straighten the BasicBlocks.
      CFGArc fall_arc = CFGNode_getFallArc(prev_node);
      Procedure_unchainBasicBlock(procedure, basicBlock);
      Procedure_chainBasicBlocks(procedure, prev_block, basicBlock);
      if (fall_arc != NULL) {
        // Conditional GOTO fall-through arc.
        if (CFGArc_HEAD(fall_arc) != cfgNode) {
          if (CFGNode_invertGoToOperation(prev_node));
          else Except_NEVER(true);
        }
        Except_CHECK(CFGArc_HEAD(CFGNode_getFallArc(prev_node)) == cfgNode);
      } else {
        // No fall-through arc: remove the unconditional GOTO.
        CFGNode_discardGoToOperation(prev_node);
      }
    }
    if (headBlock == NULL) headBlock = basicBlock;
    if (basicBlock != NULL) tailBlock = basicBlock;
    prev_block = basicBlock;
    prev_node = cfgNode;
  } BlockTrace_ENDEACH_CFGNode;
  // Set this BlockTrace HEADBLOCK and TAILBLOCK.
  *BlockTrace__HEADBLOCK(this) = headBlock;
  *BlockTrace__TAILBLOCK(this) = tailBlock;
  Except_ENSURE(CFGNode_basicBlock(BlockTrace_HEADNODE(this)) == headBlock);
  Except_ENSURE(CFGNode_basicBlock(BlockTrace_TAILNODE(this)) == tailBlock);
  // Discard the middle CFGNodes.
  prev_node = headNode;
  cfgNode = CFGNode_NEXTNODE(headNode);
  while (cfgNode != NULL && cfgNode != tailNode) {
    CFGNode next_node = CFGNode_NEXTNODE(cfgNode);
    CFGNode succ_node = CFGNode_getOnlySuccessor(cfgNode);
    CFGNode pred_node = CFGNode_getOnlyPredecessor(cfgNode);
    if (next_node != NULL && next_node == succ_node && pred_node != NULL) {
      BasicBlock basicBlock = CFGNode_basicBlock(cfgNode);
      if (BasicBlock_isEntry(basicBlock) || BasicBlock_isExit(basicBlock));
      else if (isPrePass && BasicBlock_hasDedicatedEffects(basicBlock));
      else {
        // Empty and unchain the basicBlock.
        BasicBlock next_block = CFGNode_basicBlock(next_node);
        Selector selector = BasicBlock_selector(basicBlock);
        BasicBlock_putSelector(next_block, selector);
        Procedure_unchainBasicBlock(procedure, basicBlock);
        // Bypass the cfgNode.
        DGraphNode_detachEnterArcs((DGraphNode)cfgNode);
        CFGNode_FORLEAVE_CFGArc(pred_node, arc) {
          if (CFGArc_HEAD(arc) == NULL) {
            CFGArc_attachHeadNode(arc, succ_node);
          }
        } CFGNode_ENDLEAVE_CFGArc;
        CFGNode_FORLEAVE_CFGArc(cfgNode, arc) {
          CFGArc_detachHeadNode(arc);
        } CFGNode_ENDLEAVE_CFGArc;
        DGraphNode_detachLeaveArcs((DGraphNode)cfgNode);
        // Remove the cfgNode from this BlockTrace.
        *CFGNode__NEXTNODE(pred_node) = succ_node;
        *CFGNode__PREVNODE(succ_node) = pred_node;
        PtrSet_remove(BlockTrace_CFGNODES(this), cfgNode);
        discarded = true;
        PFA_TRACE && fprintf(PFA_TRACE, "*** DISCARD %d:Block_%d\n",
                                        BasicBlock_identity(basicBlock),
                                        CFGNode_INDEX(cfgNode));
      }
    }
    cfgNode = next_node;
  }
  if (discarded) {
    // Ensure the CFGNode_NUMBERING are contiguous.
    int numbering = 0;
    BlockTrace_FOREACH_CFGNode(this, cfgNode) {
      *CFGNode__NUMBERING(cfgNode) = numbering++;
    } BlockTrace_ENDEACH_CFGNode;
    Except_CHECK(numbering == BlockTrace_NODECOUNT(this));
  }
  return discarded;
}

#ifdef $XCC_h
/**
 * Prepare this BlockTrace for scheduling.
 *
 * The BlockTrace is normalized to ensure the following conditions:
 *	* Case of block scheduling and loop scheduling: the FIRST node of the
 * DDGraph is the first LABEL operation, and the LAST node is the last
 * operation, which is a FALL if there was no control operation.
 *	* Case of software pipelining: the FIRST node of the DDGraph is the first
 * LABEL operation, and the LAST node is an unconditional branch to the loop
 * header. All other branches must be conditional GOTO operations.
 */
void
BlockTrace_prepareScheduling(BlockTrace this);
#endif//$XCC_h

void
BlockTrace_prepareScheduling(BlockTrace this)
{
  BasicBlock headBlock = BlockTrace_HEADBLOCK(this);
  BasicBlock tailBlock = BlockTrace_TAILBLOCK(this);
  CFGNode headNode = BlockTrace_HEADNODE(this);
  CFGNode tailNode = BlockTrace_TAILNODE(this);
  Operation prev_control_operation = NULL;
  BasicBlock prev_block = NULL;
  // Add the missing LABEL and FALL Operations into this BlockTrace region.
  BlockTrace_FOREACH_CFGNode(this, cfgNode) {
    BasicBlock basicBlock = CFGNode_basicBlock(cfgNode);
    Operation control_operation = BasicBlock_getControlOperation(basicBlock);
    if (prev_block == NULL) {
      // Add the first LABEL Operation.
      Label label = BasicBlock_makeLabel(basicBlock);
      Selector selector = BasicBlock_selector(basicBlock);
      Operation label_operation = Selector_makeOperationLABEL(selector, label);
      Selector_moveOperationFirst(selector, label_operation);
      Operation_setFlags(label_operation, OperationFlag_EntryCode);
    } else if (prev_control_operation == NULL) {
      // Need a FALL Operation between two BasicBlocks.
      Label label = BasicBlock_makeLabel(basicBlock);
      Selector prev_selector = BasicBlock_selector(prev_block);
      Selector_makeOperationFALL(prev_selector, label);
    }
    if (cfgNode != tailNode && CFGNode_isSuccessor(cfgNode, headNode)) {
      // Multiple loop arcs: disable software pipelining.
      BlockTrace_disablePipelining(this);
    }
    if (BasicBlock_hasGNUASM(basicBlock)) {
      // Disable software pipelining because unsafe to duplicate a GNUASM.
      BlockTrace_disablePipelining(this);
    }
    prev_control_operation = control_operation;
    prev_block = basicBlock;
  } BlockTrace_ENDEACH_CFGNode;
  // Further adjust the control-flow, depending on the type of scheduling.
  if (BlockTrace_isPipeline(this)) {
    // Software pipelining: convert the loop branch to exit branch.
    Operation last_operation = BasicBlock_lastOperation(tailBlock);
    Operator last_operator = Operation_operator(last_operation);
    Label label = Operation_getTargetLabel(last_operation);
    if (Operator_isGoTo(last_operator)) {
      if (!BasicBlock_hasLabel(headBlock, label)) {
        // Odd loop layout: disable software pipelining.
        BlockTrace_disablePipelining(this);
        Except_CHECK(BasicBlock_getControlOperation(tailBlock) != NULL);
      } else if (CFGNode_invertGoToOperation(tailNode)) {
        // There was a conditional loop back branch, it is now an exit.
        // It will be inverted again if necessary in software pipeline Construction.
      } else {
        // Last operator is unconditional: disable software pipelining.
        BlockTrace_disablePipelining(this);
        Except_CHECK(!Operator_isConditional(last_operator));
      }
    } else if (Operator_isHWLIter(last_operator)) {
      // Ensure this operation branches to the loop header.
      Except_CHECK(BasicBlock_hasLabel(headBlock, label));
    } else {
      // Neither a GOTO, nor a HWLIter: disable software pipelining.
      BlockTrace_disablePipelining(this);
    }
  }
  if (!BlockTrace_isPipeline(this)) {
    // Not software pipelining, or demoted to loop scheduling.
    if (BasicBlock_getControlOperation(tailBlock) == NULL) {
      // Append a FALL pseudo-operation to provide a LAST control operation.
      CFGArc fall_arc = CFGNode_getFallArc(tailNode);
      CFGNode head_node = CFGArc_HEAD(fall_arc);
      BasicBlock head_block = CFGNode_basicBlock(head_node);
      Label label = BasicBlock_makeLabel(head_block);
      Selector tailSelector = BasicBlock_selector(tailBlock);
      Selector_makeOperationFALL(tailSelector, label);
    }
  }
}

#ifdef $XCC__h
static inline IssueItem
BlockTrace_makeIssueItem(BlockTrace this, Operation operation)
{
  Processor processor = BlockTrace_PROCESSOR(this);
  Operator operator = Operation_operator(operation);
  Opcode opcode = Operator_opcode(operator, processor);
  IDList issueItems = BlockTrace_ISSUEITEMS(this);
  size_t size = IssueItem_Size(operation, opcode);
  void *alloc = IDList_push(issueItems, size);
  IssueItem issueItem = IssueItem_Ctor(alloc, operation, opcode);
  *IssueItem__ORDERING(issueItem) = IDList_count(issueItems);
  if (Operator_isControl(operator)) {
    *IssueItem__FLAGS(issueItem) |= InstructionFlag_Control;
  }
  Except_CHECK(   Operator_isPseudo(operator)
               || Opcode_scheduling(opcode) != Scheduling__UNDEF);
  return issueItem;
}
#endif//$XCC__h

#ifdef $XCC_h
/**
 * Enter Operations into this BlockTrace region.
 *
 * Sets the BlockTrace_FIRSTITEM, BlockTrace_STARTITEM, BlockTrace_STOPITEM
 * Operations, and BlockTrace_LASTITEM. Also fill the BlockTrace_DDGNODES.
 */
void
BlockTrace_enterOperations(BlockTrace this);
#endif//$XCC_h

void
BlockTrace_enterOperations(BlockTrace this)
{
  int controlId = 0;
  int32_t operationIndex = 0;
  PtrSet ddgNodes = BlockTrace_DDGNODES(this);
  LoopScope loopScope = BlockTrace_LOOPSCOPE(this);
  Configure configure = LoopScope_configure(loopScope);
  BasicBlock headBlock = BlockTrace_HEADBLOCK(this);
  BasicBlock tailBlock = BlockTrace_TAILBLOCK(this);
  Operation first_operation = BasicBlock_firstOperation(headBlock);
  Operation last_operation = BasicBlock_lastOperation(tailBlock);
  const_DependenceTable dependenceTable = BlockTrace_getDependenceTable(this);
  // Enter the Operations into this BlockTrace_ISSUEITEMS.
  BlockTrace_FOREACH_CFGNode(this, cfgNode) {
    IssueItem last_item = NULL;
    Operator last_operator = Operator__UNDEF;
    BasicBlock basicBlock = CFGNode_basicBlock(cfgNode);
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      IssueItem item = BlockTrace_makeIssueItem(this, operation);
      *IssueItem__CONTROLID(item) = controlId;
      Operation_setIndex(operation, ++operationIndex);
      // Fill the BlockTrace_DDGNODES.
      if (   Configure_ALIASING(configure) > 0
          && dependenceTable != NULL
          && DependenceTable_isNode(dependenceTable, operation)) {
        PtrSet_insert(ddgNodes, operation);
      }
      last_operator = operator;
      last_item = item;
    } BasicBlock_ENDEACH_Operation;
    // Compute EXITARC et FALLARC.
    if (Operator_isBranch(last_operator)) {
      if (Operator_isConditional(last_operator)) {
        *IssueItem__EXITARC(last_item) = CFGNode_getTargetArc(cfgNode);
        ++controlId;
      }
    }
    CFGNode_FORLEAVE_CFGArc(cfgNode, arc) {
      if (arc != IssueItem_EXITARC(last_item)) {
        if (IssueItem_FALLARC(last_item) == NULL) {
          *IssueItem__FALLARC(last_item) = arc;
        } else {
          *IssueItem__FALLARC(last_item) = NULL;
          break;
        }
      }
    } CFGNode_ENDLEAVE_CFGArc;
  } BlockTrace_ENDEACH_CFGNode;
  // Set this BlockTrace FIRSTITEM, STARTITEM, STOPITEM, LASTITEM.
  Except_REQUIRE(BlockTrace_FIRSTITEM(this) == NULL);
  *BlockTrace__FIRSTITEM(this) = *BlockTrace__STARTITEM(this)
                               = Operation_issueItem(first_operation);
  *BlockTrace__LASTITEM(this) = *BlockTrace__STOPITEM(this)
                              = Operation_issueItem(last_operation);
  if (BasicBlock_isEntry(headBlock)) {
    Operation start_operation = first_operation;
    BasicBlock_FOREACH_Operation(headBlock, operation) {
      if (Operation_isEntryCode(operation)) start_operation = operation;
      else break;
    } BasicBlock_ENDEACH_Operation;
    *BlockTrace__STARTITEM(this) = Operation_issueItem(start_operation);
  }
  if (BlockTrace_isPipeline(this)) {
    // Append a LOOP pseudo-operation as the last operation.
    Label label = BasicBlock_makeLabel(headBlock);
    Selector tailSelector = BasicBlock_selector(tailBlock);
    Operation loop_operation = Selector_makeOperationLOOP(tailSelector, label);
    IssueItem loop_item = BlockTrace_makeIssueItem(this, loop_operation);
    Operation_setIndex(loop_operation, ++operationIndex);
    *BlockTrace__STOPITEM(this) = *BlockTrace__LASTITEM(this) = loop_item;
  } else
  if (BasicBlock_isExit(tailBlock)) {
    Operation stop_operation = last_operation;
    BasicBlock_FORBACK_Operation(tailBlock, operation) {
      if (Operation_isExitCode(operation)) stop_operation = operation;
      else break;
    } BasicBlock_ENDBACK_Operation;
    *BlockTrace__STOPITEM(this) = Operation_issueItem(stop_operation);
  }
  PFA_TRACE && (fprintf(PFA_TRACE, "FIRST:\t"),
      Operation_pretty(IssueItem_OPERATION(BlockTrace_FIRSTITEM(this)), PFA_TRACE));
  PFA_TRACE && (fprintf(PFA_TRACE, "START:\t"),
      Operation_pretty(IssueItem_OPERATION(BlockTrace_STARTITEM(this)), PFA_TRACE));
  PFA_TRACE && (fprintf(PFA_TRACE, "STOP:\t"),
      Operation_pretty(IssueItem_OPERATION(BlockTrace_STOPITEM(this)), PFA_TRACE));
  PFA_TRACE && (fprintf(PFA_TRACE, "LAST:\t"),
      Operation_pretty(IssueItem_OPERATION(BlockTrace_LASTITEM(this)), PFA_TRACE));
}

#ifdef $XCC_h
/**
 * Relax scheduling constraints with speculation.
 */
void
BlockTrace_relaxConstraints(BlockTrace this);
#endif//$XCC_h

void
BlockTrace_relaxConstraints(BlockTrace this)
{
  bool isPrePass = BlockTrace_isPrePass(this);
  int speculation = BlockTrace_SPECULATION(this);
  int compensation = BlockTrace_COMPENSATION(this);
  Processor processor = BlockTrace_PROCESSOR(this);
  Convention convention = BlockTrace_CONVENTION(this);
  Register stackRegister = Convention_stackRegister(convention);
  // Flag speculable and compensable Operation(s).
  BlockTrace_FOREACH_IssueItem(this, ITEM) {
    InstructionTask task = IssueItem_TASK(ITEM);
    Operation operation = IssueItem_OPERATION(ITEM);
    Operation_refreshFlags(operation, stackRegister);
    if (   Operation_maySpeculate(operation, processor, speculation)
        && (!isPrePass || !Operation_hasDedicatedResult(operation, false))) {
#ifdef _DEBUG
      if (DEBUG_SPECULABLE && DEBUG_SPECULABLE--)
#endif//_DEBUG
      {
        *InstructionTask__FLAGS(task) |= InstructionFlag_Speculable;
        //PFA_TRACE && fprintf(PFA_TRACE, "*** Speculable")
                  //&& Operation_pretty(operation, PFA_TRACE);
      }
    }
    if (   Operation_mayCompensate(operation, processor, compensation)
        && (!isPrePass || !Operation_hasDedicatedArgument(operation, false))) {
#ifdef _DEBUG
      if (DEBUG_COMPENSABLE && DEBUG_COMPENSABLE--)
#endif//_DEBUG
      {
        *InstructionTask__FLAGS(task) |= InstructionFlag_Compensable;
        //PFA_TRACE && fprintf(PFA_TRACE, "*** Compensable")
                  //&& Operation_pretty(operation, PFA_TRACE);
      }
    }
  } BlockTrace_ENDEACH_IssueItem;
  // Prepare boostable Operation(s).
  if (BlockTrace_isPrePass(this)) {
    BlockTrace_prepareBoosting(this);
  }
}

#ifdef $XCC__h
/**
 * Prepare for Operation boosting.
 */
void
BlockTrace_prepareBoosting(BlockTrace this);
#endif//$XCC__h

void
BlockTrace_prepareBoosting(BlockTrace this)
{
  int goto_polarity = 0;
  Temporary goto_predicate = NULL;
  int boosting = BlockTrace_BOOSTING(this);
  Processor processor = BlockTrace_PROCESSOR(this);
  if (boosting <= ConfigureBoosting_None) return;
  // Identify the last control Operation before LOOP.
  if (BlockTrace_isPipeline(this)) {
    BlockTrace_FORBACK_IssueItem(this, ITEM) {
      Operation operation = IssueItem_OPERATION(ITEM);
      Operator operator = Operation_operator(operation);
      if (Operator_isLoop(operator)) continue;
      if (Operator_isControl(operator)) {
        goto_polarity = 0;
        goto_predicate = NULL;
        if (Operator_isGoTo(operator)) {
          goto_polarity = Operator_predicatePolarity(operator);
          goto_predicate = Operation_getPredicate(operation);
        }
        break;
      }
    } BlockTrace_ENDBACK_IssueItem;
  }
  // Process the boostable Operation(s).
  BlockTrace_FOREACH_IssueItem(this, ITEM) {
    Operation operation = IssueItem_OPERATION(ITEM);
    Operator operator = Operation_operator(operation);
    if (Operator_isControl(operator)) {
      goto_polarity = 0;
      goto_predicate = NULL;
      if (Operator_isGoTo(operator)) {
        goto_polarity = Operator_predicatePolarity(operator);
        goto_predicate = Operation_getPredicate(operation);
      }
    } else
    if (   !Operation_isSpeculable(operation)
        && !Operation_hasDedicatedResult(operation, false)) {
      if (   (   goto_polarity > 0
              && Operation_mayPredicateNegative(operation, processor))
          || (   goto_polarity < 0
              && Operation_mayPredicatePositive(operation, processor))) {
#ifdef _DEBUG
      if (DEBUG_BOOSTABLE && DEBUG_BOOSTABLE--)
#endif//_DEBUG
        {
          Operation_predicate(operation, processor, goto_predicate, -goto_polarity);
          Operation_setBoosting(operation, 1);
          PFA_TRACE && fprintf(PFA_TRACE, "*** Boosting")
                    && Operation_pretty(operation, PFA_TRACE);
        }
      }
      if (   boosting >= ConfigureBoosting_Combine && false/*FIXME*/
          && Operation_mayUnpredicate(operation, processor)) {
#ifdef _DEBUG
      if (DEBUG_BOOSTABLE && DEBUG_BOOSTABLE--)
#endif//_DEBUG
        {
          Operation_setBoosting(operation, 1);
          PFA_TRACE && fprintf(PFA_TRACE, "*** Boosting")
                    && Operation_pretty(operation, PFA_TRACE);
        }
      }
    }
  } BlockTrace_ENDEACH_IssueItem;
}

#ifdef $XCC_h
/**
 * Build this BlockTrace_EFFECTTABLE.
 */
void
BlockTrace_buildEffects(BlockTrace this);
#endif//$XCC_h

void
BlockTrace_buildEffects(BlockTrace this)
{
  Memory memory = BlockTrace_MEMORY(this);
  Procedure procedure = BlockTrace_PROCEDURE(this);
  CodeRegion codeRegion = BlockTrace_CODEREGION(this);
  Convention convention = BlockTrace_CONVENTION(this);
  const_RegisterSet argumentSet = Convention_roleRegisterSet(convention, ConventionRole_Argument);
  const_RegisterSet resultSet = Convention_roleRegisterSet(convention, ConventionRole_Result);
  const_RegisterSet structSet = Convention_roleRegisterSet(convention, ConventionRole_Struct);
  const_RegisterSet calleeSet = Convention_roleRegisterSet(convention, ConventionRole_Callee);
  const_RegisterSet callerSet = Convention_roleRegisterSet(convention, ConventionRole_Caller);
  int32_t estimate = BlockTrace_ITEMCOUNT(this)*2;
  EffectTable effectTable = EffectTable_new(memory, procedure, estimate);
  Processor processor = BlockTrace_PROCESSOR(this);
  bool interlocks = Processor_interlocks(processor);
  IssueItem firstItem = BlockTrace_FIRSTITEM(this);
  IssueItem lastItem = BlockTrace_LASTITEM(this);
  int speculation = BlockTrace_SPECULATION(this);
  PFA_TRACE && fprintf(PFA_TRACE, "*** BUILD EFFECTS\n");
  BlockTrace_FOREACH_IssueItem(this, ITEM) {
    Operation OPERATION = IssueItem_OPERATION(ITEM);
    if (BlockTrace_isInnerLoop(this) && ITEM == firstItem) {
      // In case of InnerLoop, do not consider the write effects of the
      // first operation which is a LABEL pseudo-operation.
    } else if (BlockTrace_isPipeline(this) && ITEM == lastItem) {
      // In case of Pipeline, do not consider the read effects of the last
      // operation, as it has been normalized into a LOOP pseudo-operation.
    } else {
      Operator OPERATOR = Operation_operator(OPERATION);
      BasicBlock basicBlock = Operation_basicBlock(OPERATION);
      // Enter OPERATION Effect(s) including side-Effect(s).
      EffectTable_enterOperation(effectTable, OPERATION, true);
      // Label indirect write effects.
      if (Operator_isLabel(OPERATOR)) {
        CFGNode cfgNode = BasicBlock_cfgNode(basicBlock);
        CFGNode prevNode = CFGNode_PREVNODE(cfgNode);
        CFGNode_FORENTER_CFGArc(cfgNode, cfgArc) {
          if (CFGArc_TAIL(cfgArc) != prevNode) {
            // HACK ALERT! We need the reaching defs along cfgArc.
            TemporarySet liveOnEntry = CFGNode_LIVEINSET(cfgNode);
            EffectTable_enterTemporarySetSideEffects(effectTable,
                OPERATION, liveOnEntry, EffectFlag_Indirect|EffectFlag_Write);
            break;
          }
        } CFGNode_ENDENTER_CFGArc;
      }
      // Branch indirect read effects.
      if (Operator_isBranch(OPERATOR)) {
        CFGNode cfgNode = BasicBlock_cfgNode(basicBlock);
        CFGNode nextNode = CFGNode_NEXTNODE(cfgNode);
        CFGNode_FORLEAVE_CFGArc(cfgNode, cfgArc) {
          CFGNode headNode = CFGArc_HEAD(cfgArc);
          if (headNode != nextNode &&
              CFGNode_basicBlock(headNode) != NULL) {
            // Compute the live on exit along cfgArc.
            TemporarySet liveInSet = CFGNode_LIVEINSET(headNode);
            TemporarySet liveOutSet = CFGNode_LIVEOUTSET(cfgNode);
            int32_t count = TemporarySet_count(liveOutSet);
            TemporarySet liveOnExit = TemporarySet_make(memory, count);
            TemporarySet_union(liveOnExit, liveOutSet);
            if (liveInSet != NULL) TemporarySet_inter(liveOnExit, liveInSet);
            else;	// HACK! Must be a new headNode created by the pipeliner.
            EffectTable_enterTemporarySetSideEffects(effectTable,
                OPERATION, liveOnExit, EffectFlag_Indirect);
            TemporarySet_kill(liveOnExit);
          }
        } CFGNode_ENDLEAVE_CFGArc;
        if (BasicBlock_isExit(basicBlock)) {
          // Non-local branch, assume it results from tail-call optimization.
          // Include Indirect read effects of Callee-saved registers in case of
          // non-interlocked registers.
          if (!interlocks) {
            EffectTable_enterRegisterSetSideEffects(effectTable, OPERATION,
                                                    argumentSet, EffectFlag_Indirect);
            EffectTable_enterRegisterSetSideEffects(effectTable, OPERATION,
                                                    structSet, EffectFlag_Indirect);
            EffectTable_enterRegisterSetSideEffects(effectTable, OPERATION,
                                                    calleeSet, EffectFlag_Indirect);
          }
        }
      }
      if (MDT_isFamily("k1")) {
        // FIXME DUCO In case of k1, fix the missing clobbers in Tirex.
        if (Operator_isRoutine(OPERATOR) && Processor_minTheta(processor)) {
          EffectTable_enterRegisterSetSideEffects(effectTable, OPERATION,
                                                  callerSet, EffectFlag_Kill);
          EffectTable_enterRegisterSetSideEffects(effectTable, OPERATION,
                                                  structSet, EffectFlag_Write);
          EffectTable_enterRegisterSetSideEffects(effectTable, OPERATION,
                                                  resultSet, EffectFlag_Write);
        }
      } // END of FIXME DUCO
      if (Operator_isRoutine(OPERATOR) || Operator_isReturn(OPERATOR)) {
        // Routine call and Return indirect read effects of Callee-saved registers.
        // Only include these indirect effects in case of non-interlocked registers.
        // For Routine call, the Argument and possibly Struct registers are already
        // in the call explicit effects so there is no need to include them.
        if (!interlocks) {
          EffectTable_enterRegisterSetSideEffects(effectTable, OPERATION,
                                                  calleeSet, EffectFlag_Indirect);
        }
      }
    }
  } BlockTrace_ENDEACH_IssueItem;
  *BlockTrace__EFFECTTABLE(this) = effectTable;
}

#ifdef $XCC_h
/**
 * Build this BlockTrace_LIFETIMETABLE.
 */
void
BlockTrace_buildLifetimes(BlockTrace this);
#endif//$XCC_h

void
BlockTrace_buildLifetimes(BlockTrace this)
{
  PtrSet ddgNodes = BlockTrace_DDGNODES(this);
  Optimize optimize = BlockTrace_OPTIMIZE(this);
  Configure configure = BlockTrace_CONFIGURE(this);
  Processor processor = BlockTrace_PROCESSOR(this);
  EffectTable effectTable = BlockTrace_EFFECTTABLE(this);
  Memory memory = BlockTrace_MEMORY(this);
  LifetimeTable lifetimeTable = LifetimeTable_new(memory, optimize, configure);
  // Build the LifetimeTable.
  bool isPrePass = BlockTrace_isPrePass(this);
  bool isInnerLoop = BlockTrace_isInnerLoop(this);
  const_DependenceTable dependenceTable = BlockTrace_getDependenceTable(this);
  LifetimeTable_build(lifetimeTable, effectTable, isInnerLoop, isPrePass, processor);
  LifetimeTable_linkEffects(lifetimeTable, ddgNodes);
  if (!PtrSet_isEmpty(ddgNodes)) {
    LifetimeTable_refineDependences(lifetimeTable, ddgNodes, dependenceTable);
  }
  *BlockTrace__LIFETIMETABLE(this) = lifetimeTable;
}

#ifdef $XCC_h
/**
 * Sort this BlockTrace IssueItems using InstructionTask_compare.
 */
static inline void
BlockTrace_sortIssueItems(BlockTrace this)
{
  IDList_sort(BlockTrace_ISSUEITEMS(this), InstructionTask_compare);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Find the loop-invariants.
 *
 * According to Muchnich in "Compiler Design and Implementation", p. 398,
 * loop-invariant operations are detected as follows:
 * * Mark as loop-invariant the constant operands in the instructions.
 * * Mark the operands that have all definitions that reach them located
 * outside the loop.
 * * Mark as loop-invariant instructions that: have all operands marked as
 * loop-invariant; or operands have only one reaching definition, and that
 * definition is an already marked loop-invariant instruction, and there are no
 * uses of the variable assigned to by that instruction in the loop located
 * before the instruction.
 */
void
BlockTrace_findInvariants(BlockTrace this);
#endif//$XCC_h

#if 0
void
BlockTrace_findInvariants(BlockTrace this)
{
  bool change = false;
  LifetimeTable lifetimeTable = BlockTrace_LIFETIMETABLE(this);
  TemporaryMap temporaryMap = LifetimeTable_TEMPORARYMAP(lifetimeTable);
  Except_REQUIRE(BlockTrace_isInnerLoop(this));
  do {
    change = false;
    BlockTrace_FOREACH_IssueItem(this, ITEM) {
      Operation operation = IssueItem_OPERATION(ITEM);
      if (!IssueItem_isInvariant(ITEM) && !IssueItem_isCritical(ITEM)) {
        IssueItemFlag invariant = IssueItemFlag_Invariant;
        Operation_ARGUMENTS_FOREACH_Temporary(operation, temporary) {
          if (Temporary_hasRegFile(temporary) && !Temporary_isConstant(temporary)) {
            // Only the register arguments that are not wired may prevent loop invariance.
            Lifetime *_lifetime = (Lifetime *)TemporaryMap_access(temporaryMap, temporary);
            Lifetime lifetime = Lifetime_findReachingLifetime(*_lifetime, operation,
                                                              Operation_ARGUMENTS_INDEX);
            if (Lifetime_isOnlyUses(lifetime)) {
              // Defined outside the loop.
            } else if (Lifetime_isWriteOnce(lifetime) && Lifetime_isReadAfter(lifetime)) {
              // Single reaching definition, with all uses after the definition.
              Effect write_effect = Lifetime_firstEffect(lifetime);
              Operation write_operation = Effect_OPERATION(write_effect);
              if (!IssueItem_isInvariant(Operation_issueItem(write_operation))) {
                // The single reaching definition is not invariant.
                invariant = 0;
                break;
              }
            } else {
              // Has multiple reaching definitions, or has a use before the definition.
              invariant = 0;
              break;
            }
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
        if (invariant != 0) {
          *IssueItem__FLAGS(ITEM) |= invariant;
          change = true;
        }
      }
    } BlockTrace_ENDEACH_IssueItem;
  } while (change);
}
#endif

#ifdef $XCC_h
/**
 * Compute this BlockTrace_KUNROLL.
 *
 * As a side-effect, compute the Lifetime_LIFESPAN.
 */
int
BlockTrace_computeKunroll(BlockTrace this, int lambda);
#endif//$XCC_h

int
BlockTrace_computeKunroll(BlockTrace this, int lambda)
{
  bool isAcyclic = BlockTrace_isAcyclic(this);
  Procedure procedure = BlockTrace_PROCEDURE(this);
  LifetimeTable lifetimeTable = BlockTrace_LIFETIMETABLE(this);
  int maxSpan = LifetimeTable_computeMaxSpan(lifetimeTable, lambda, isAcyclic);
  *BlockTrace__KUNROLL(this) = 0;
  if (BlockTrace_isPipeline(this)) {
    // Case of software pipelining.
    int factor = BlockTrace_FACTOR(this);
    int16_t kunroll = (maxSpan + lambda - 1)/lambda;
    if (kunroll <= 0) kunroll = 1;
    if (kunroll < FORCE_KUNROLL) kunroll = FORCE_KUNROLL;
    PFA_TRACE && fprintf(PFA_TRACE,
        "*** PREPARE:\tmaxSpan=%d\tlambda=%d\tkunroll=%d\n",
        maxSpan, lambda, kunroll);
    if (factor > 1) {
      // Counted hardware loop: adjust kunroll.
      Except_CHECK(kunroll > 0 && kunroll <= factor);
      // Find the smallest divisor of factor not lower than kunroll.
      while (factor >> 1 >= kunroll) factor >>= 1;
      *BlockTrace__FACTOR(this) = kunroll = factor;
      PFA_TRACE && fprintf(PFA_TRACE, "*** ADJUST:\tkunroll=%d\n", kunroll);
    }
    if (kunroll > 0) {
      LifetimeTable_fillRenaming(lifetimeTable, procedure, lambda, kunroll);
    }
    *BlockTrace__KUNROLL(this) = kunroll;
  }
  return BlockTrace_KUNROLL(this);
} 

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Cover a RegionFlow with BlockTraces.
 */
struct RegionFlowCover_;
#endif//$XCC_h

#ifdef $XCC__h
struct RegionFlowCover_ {
  //@args	Memory memory, RegionFlow regionFlow, BlockTraceFlags traceFlags
  RegionFlow REGIONFLOW;
  BlockTraceFlags TRACEFLAGS;
  IDList_ BLOCKTRACES_;
  //@access BLOCKTRACES	RegionFlowCover__BLOCKTRACES_(this)
  //@access MEMORY	IDList_memory(RegionFlowCover_BLOCKTRACES(this))
};
#endif//$XCC__h

RegionFlowCover
RegionFlowCover_Ctor(RegionFlowCover this,
                     Memory memory, RegionFlow regionFlow, BlockTraceFlags traceFlags)
{
  *RegionFlowCover__REGIONFLOW(this) = regionFlow;
  *RegionFlowCover__TRACEFLAGS(this) = traceFlags;
  IDList_Ctor(RegionFlowCover_BLOCKTRACES(this), memory);
  {
    Hierarchy hierarchy = RegionFlow_makeHierarchy(regionFlow);
    HierarchyComponent root = Hierarchy_root(hierarchy);
    CodeRegion_resetTraceCounter(RegionFlow_codeRegion(regionFlow));
    RegionFlowCover_processHierarchyComponent(this, root, traceFlags);
    // Sort the BlockTrace(s) with inner later loops first.
    IDList_sort(RegionFlowCover_BLOCKTRACES(this), BlockTrace_compare);
  }
  return this;
}

void
RegionFlowCover_Dtor(RegionFlowCover this)
{
  IDList_empty(RegionFlowCover_BLOCKTRACES(this),
               (IDListItemRelease)BlockTrace_Dtor);
  IDList_Dtor(RegionFlowCover_BLOCKTRACES(this));
}

size_t
RegionFlowCover_Size(Memory memory, RegionFlow regionFlow, BlockTraceFlags traceFlags) {
        return sizeof(RegionFlowCover_);
}

#ifdef $XCC__h
void
RegionFlowCover_processNode(RegionFlowCover this,
                             BlockTraceFlags traceFlags, CFGNode cfgNode, LoopScope loopScope);
#endif//$XCC__h

void
RegionFlowCover_processNode(RegionFlowCover this,
                             BlockTraceFlags traceFlags, CFGNode cfgNode, LoopScope loopScope)
{
  Memory memory = RegionFlowCover_MEMORY(this);
  IDList blockTraces = RegionFlowCover_BLOCKTRACES(this);
  RegionFlow regionFlow = RegionFlowCover_REGIONFLOW(this);
  float superGrow = (float)FORCE_SUPERGROW;
  if (CFGNode_isGrow(cfgNode)) {
    CFGNode curr_node = cfgNode;
    float curr_probability = 1.0;
    size_t size = BlockTraceSize(memory, traceFlags);
    BlockTrace blockTrace = IDList_push(blockTraces, size);
    EntryCode(blockTrace, memory, traceFlags);
    PFA_TRACE && fprintf(PFA_TRACE, "*** BLOCKTRACE:\tStart(%s)",
                         traceFlags&BlockTraceFlag_InnerLoop? "inner":"");
    do {
      bool growOK = BlockTraceGrow(blockTrace, curr_node);
      *CFGNode__FLAGS(curr_node) &= ~CFGNodeFlag_Grow;
      PFA_TRACE && fprintf(PFA_TRACE, "Grow(_%d)", CFGNode_INDEX(curr_node));
      if (!CFGNode_isStop(curr_node)) {
        BasicBlock curr_block = CFGNode_basicBlock(curr_node);
        CFGNode_FORLEAVE_CFGArc(curr_node, arc) {
          float probability = CFGArc_PROBABILITY(arc);
          float head_probability = curr_probability*probability;
          CFGNode head_node = CFGArc_HEAD(arc);
          BasicBlock head_block = CFGNode_basicBlock(head_node);
          if (CFGNode_isGrow(head_node) && !CFGNode_isStart(head_node) &&
              BasicBlock_regionId(head_block) == BasicBlock_regionId(curr_block) &&
              BasicBlock_loopScope(head_block) == BasicBlock_loopScope(curr_block) &&
              BasicBlock_index(head_block) == BasicBlock_index(curr_block) + 1 &&
              (probability*superGrow >= 1.0 || CFGNode_isStop(head_node)) &&
              CFGNode_isOnlyPredecessor(head_node, curr_node)) {
            curr_probability = head_probability;
            curr_node = head_node;
            break;
          }
        } CFGNode_ENDLEAVE_CFGArc;
      }
    } while (CFGNode_isGrow(curr_node));
    BlockTraceStop(blockTrace, regionFlow, loopScope);
    PFA_TRACE && fprintf(PFA_TRACE, "Stop\n");
  }
}

#ifdef $XCC__h
void
RegionFlowCover_processHierarchyComponent(RegionFlowCover this,
                                          HierarchyComponent component,
                                          BlockTraceFlags traceFlags);
#endif//$XCC__h

void
RegionFlowCover_processHierarchyComponent(RegionFlowCover this,
                                          HierarchyComponent component,
                                          BlockTraceFlags traceFlags)
{
  RegionFlow regionFlow = RegionFlowCover_REGIONFLOW(this);
  Procedure procedure = RegionFlow_procedure(regionFlow);
  HierarchyComponentType type = HierarchyComponent_type(component);
  BasicBlock headerBlock = NULL;
  LoopScope loopScope = NULL;
  CFGNode cfgNode = HierarchyComponent_node(component);
  // Access the HierarchyComponent LoopScope.
  if (cfgNode != NULL && type != HierarchyComponentType_Node) {
    headerBlock = CFGNode_basicBlock(cfgNode);
  }
  loopScope = Procedure_findLoopScope(procedure, headerBlock);
  // Adjust the flags in case of inner loop.
  if (LoopScope_isInner(loopScope) > 0) {
    traceFlags |= BlockTraceFlag_InnerLoop;
  }
  // Traversal of component: prepare BasicBlock and CFGNode flags.
  HierarchyComponent_CHILDREN_FOREACH_HierarchyComponent(component, child) {
    if (HierarchyComponent_type(child) == HierarchyComponentType_Node) {
      CFGNode cfgNode = HierarchyComponent_node(child);
      RegionFlowCover_processNode(this, traceFlags, cfgNode, loopScope);
    } else {
      RegionFlowCover_processHierarchyComponent(this, child, traceFlags);
    }
  } HierarchyComponent_CHILDREN_ENDEACH_HierarchyComponent;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(RegionFlowCover_)\t%zu\n", sizeof(RegionFlowCover_));
}
#endif//$XCC__c

#ifdef $XCC_h
RegionFlowCover
RegionFlowCover_new(Memory parent, RegionFlow regionFlow, BlockTraceFlags traceFlags);
#endif//$XCC_h

RegionFlowCover
RegionFlowCover_new(Memory parent, RegionFlow regionFlow, BlockTraceFlags traceFlags)
{
  Memory memory = Memory_new(parent, true);
  size_t size = RegionFlowCover_Size(memory, regionFlow, traceFlags);
  RegionFlowCover this = Memory_alloc(memory, size);
  return RegionFlowCover_Ctor(this, memory, regionFlow, traceFlags);
}

#ifdef $XCC_h
RegionFlowCover
RegionFlowCover_delete(RegionFlowCover this);
#endif//$XCC_h

RegionFlowCover
RegionFlowCover_delete(RegionFlowCover this)
{
  if (this != NULL) {
    Memory memory = RegionFlowCover_MEMORY(this);
#ifndef _NDTOR
    RegionFlowCover_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * For use by RegionFlowCover_FOREACH_BlockTrace.
 */
IDList
RegionFlowCover_blockTraces(RegionFlowCover this);
#endif//$XCC_h

IDList
RegionFlowCover_blockTraces(RegionFlowCover this)
{
  return RegionFlowCover_BLOCKTRACES(this);
}

#ifdef $XCC_h
/**
 * Iterate this RegionFlowCover BlockTraces.
 */
#define RegionFlowCover_FOREACH_BlockTrace(this, blockTrace) \
  IDList_FOREACH(RegionFlowCover_blockTraces(this), BlockTrace_, blockTrace)
#define RegionFlowCover_ENDEACH_BlockTrace \
  IDList_ENDEACH;
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the BlockTrace module.
 */
#define BlockTrace_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the BlockTrace module.
 */
#define BlockTrace_FINI()
#endif//$XCC__h

#if XCC__C
static void
BlockTrace_TEST(void)
{
#include "XFA/BlockTrace_.c"
}

int
main(int argc, char **argv)
{
  XFA_Test(BlockTrace_TEST);
  return 0;
}
#endif

