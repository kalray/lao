#ifdef $XCC_h
/*
 * !!!!	DDGraph.xcc
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
 * @brief Data Dependence Graph.
 */
#endif//$XCC_h


#include "XFA/XFA_.h"

#ifdef $XCC__h
#include "XFA/DDGraph.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * If non-zero, use PROCESSING times in DDGraphArc latencies.
 */
//#define DDGraph_PROCESSING 0
#ifndef DDGraph_PROCESSING
extern int DDGraph_PROCESSING;
#endif//DDGraph_PROCESSING
#endif//$XCC_h

#ifndef DDGraph_PROCESSING
int DDGraph_PROCESSING = 1;
#endif//DDGraph_PROCESSING

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#define DDGraph_EXTENT     (1<<14)
#endif//$XCC_h

#ifdef $XCC_h
struct DDGraphArc_;
  //@access TAIL	(DDGraphNode)DGraphArc_TAILNODE(this)
  //@access HEAD	(DDGraphNode)DGraphArc_HEADNODE(this)
#endif//$XCC_h

#ifdef $XCC_h
struct DDGraphArc_ {
  //@args	int theta, int omega, DependenceKind kind,
  //@args	Temporary temporary, NetArcFlags flags
  NetArc_ NETARC_;		// Base NetArc_: must be first in struct DDGraphArc_.
  //@access NETARC	DDGraphArc__NETARC_(this)
  //@access DELAY	NetArc_DELAY(DDGraphArc_NETARC(this))
  //@mutate DELAY	NetArc__DELAY(DDGraphArc_NETARC(this))
  //@access KIND	NetArc_TYPE(DDGraphArc_NETARC(this))
  //@mutate KIND	NetArc__TYPE(DDGraphArc_NETARC(this))
  //@access FLAGS	NetArc_FLAGS(DDGraphArc_NETARC(this))
  //@mutate FLAGS	NetArc__FLAGS(DDGraphArc_NETARC(this))
  //@access isUseless	NetArc_isUseless(DDGraphArc_NETARC(this))
  int16_t THETA;		// Dependence latency.
  int16_t OMEGA;		// Dependence distance.
  Temporary TEMPORARY;		// The Temporary involved in the arc.
};
#endif//$XCC_h

DDGraphArc
DDGraphArc_Ctor(DDGraphArc this,
                int theta, int omega, DependenceKind kind,
                Temporary temporary, NetArcFlags flags)
{
  NetArc_Ctor(DDGraphArc_NETARC(this), theta, flags);
  *DDGraphArc__KIND(this) = kind;
  *DDGraphArc__THETA(this) = theta;
  *DDGraphArc__OMEGA(this) = omega;
  *DDGraphArc__TEMPORARY(this) = temporary;
  return this;
}

size_t
DDGraphArc_Size(int theta, int omega, DependenceKind kind,
                Temporary temporary, NetArcFlags flags)
{
  return sizeof(DDGraphArc_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(DDGraphArc_)\t%zu\n", sizeof(DDGraphArc_));
}
#endif//$XCC__c

/**
 * DDGraphArc compare function, used by DDGraph_sortLeaveArcs.
 *
 * Compare on lower DELAY then lower head INDEX.
 */
static int
DDGraphArc_compare(DDGraphArc arc1, DDGraphArc arc2)
{
  int result = 0;
  DDGraphNode head1 = DDGraphArc_HEAD(arc1);
  DDGraphNode head2 = DDGraphArc_HEAD(arc2);
  result = DDGraphArc_DELAY(arc2) - DDGraphArc_DELAY(arc1);
  if (result != 0) return result;
  result = DDGraphArc_KIND(arc1) - DDGraphArc_KIND(arc2);
  if (result != 0) return result;
  result = DDGraphNode_INDEX(head1) - DDGraphNode_INDEX(head2);
  return result;
}

#ifdef $XCC_h
/**
 * Pretty-print this DDGraphArc.
 */
bool
DDGraphArc_pretty(DDGraphArc this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
DDGraphArc_pretty(DDGraphArc this, FILE *file)
{
  NetArcFlags flags = DDGraphArc_FLAGS(this);
  if (file == NULL) file = stderr;
  fprintf(file, "(_%d, _%d)\ttheta=%d\tomega=%d\tdelay=%d\t%s ",
      DDGraphNode_INDEX(DDGraphArc_TAIL(this)), DDGraphNode_INDEX(DDGraphArc_HEAD(this)),
      DDGraphArc_THETA(this), DDGraphArc_OMEGA(this), DDGraphArc_DELAY(this),
      DependenceKind_Id(DDGraphArc_KIND(this)));
  Temporary_pretty(DDGraphArc_TEMPORARY(this), file);
  if (flags != 0) {
    fprintf(file, "\t");
    ETable_prettyFlags(NetArcFlag_Table, flags, "+", file);
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
struct DDGraphNode_;
  //@access INDEX	DGraphNode_INDEX(this)
#endif//$XCC_h

#ifdef $XCC_h
struct DDGraphNode_ {
  //@args	Operation operation
  //@access EDGE	(DDGraphArc)NetNode_EDGE(DDGraphNode_NETNODE(this))
  NetNode_ NETNODE_;		// Base NetNode_: must be first in struct DDGraphNode_.
  //@access NETNODE	DDGraphNode__NETNODE_(this)
  //@access REQUIRE	NetNode_REQUIRE(DDGraphNode_NETNODE(this))
  //@mutate REQUIRE	NetNode__REQUIRE(DDGraphNode_NETNODE(this))
  //@access DATE	NetNode_DATE(DDGraphNode_NETNODE(this))
  Operation OPERATION;		// The DDGraphNode involved in the node.
  DDGraphArc ROOT2NODE;		// Arc from the root node to this node.
  DDGraphArc NODE2ROOT;		// Arc from this node to the root node.
};
#endif//$XCC_h

DDGraphNode
DDGraphNode_Ctor(DDGraphNode this,
                 Operation operation)
{
  NetNode_Ctor(DDGraphNode_NETNODE(this), 0);
  *DDGraphNode__OPERATION(this) = operation;
  *DDGraphNode__ROOT2NODE(this) = NULL;
  *DDGraphNode__NODE2ROOT(this) = NULL;
  return this;
}

size_t
DDGraphNode_Size(Operation operation)
{
  return sizeof(DDGraphNode_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(DDGraphNode_)\t%zu\n", sizeof(DDGraphNode_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This DDGraphNode index in the DDGraph.
 */
static inline uint32_t
DDGraphNode_index(struct DDGraphNode_ *this)
{
  return DDGraphNode_INDEX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the entering DDGraphArcs.
 */
#define DDGraphNode_FORENTER_DDGraphArc(this, arc) \
  DGraphNode_FORENTER(this, DDGraphArc_, arc) {
#define DDGraphNode_ENDENTER_DDGraphArc \
  } DGraphNode_ENDENTER;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the leaving DDGraphArcs.
 */
#define DDGraphNode_FORLEAVE_DDGraphArc(this, arc) \
  DGraphNode_FORLEAVE(this, DDGraphArc_, arc) {
#define DDGraphNode_ENDLEAVE_DDGraphArc \
  } DGraphNode_ENDLEAVE;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pretty-print this DDGraphNode.
 */
bool
DDGraphNode_pretty(DDGraphNode this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
DDGraphNode_pretty(DDGraphNode this, FILE *file)
{
  DDGraphArc edge = DDGraphNode_EDGE(this);
  Operation operation = DDGraphNode_OPERATION(this);
  if (file == NULL) file = stderr;
  fprintf(file, "node_%d\t", DDGraphNode_INDEX(this));
  if (edge != NULL) {
    if (DDGraphArc_HEAD(edge) == this) {
      fprintf(file, "<- _%d\t", DDGraphNode_INDEX(DDGraphArc_TAIL(edge)));
    } else {
      fprintf(file, "-> _%d\t", DDGraphNode_INDEX(DDGraphArc_HEAD(edge)));
    }
  } else fprintf(file, "\t");
  if (DDGraphNode_REQUIRE(this) != 0) {
    fprintf(file, "require=%d\t", DDGraphNode_REQUIRE(this));
  } else fprintf(file, "\t\t");
  if (operation != NULL) {
    Operator operator = Operation_operator(operation);
    fprintf(file, "%d:%s_%d",
        Operation_identity(operation), Operator_mnemonic(operator),
        Operation_index(operation));
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Display this DDGraphNode name for Integer Programming formulations.
 */
const char *
DDGraphNode_name(DDGraphNode this);
#endif//$XCC_h

const char *
DDGraphNode_name(DDGraphNode this)
{
  static char buffer[32];
  int32_t index = DDGraphNode_INDEX(this);
  Operation operation = DDGraphNode_OPERATION(this);
  IssueItem item  = Operation_issueItem(operation);
  Scheduling scheduling = IssueItem_SCHEDULING(item);
  if (Operation_index(operation) == index) {
    if (scheduling != Scheduling__UNDEF) {
#ifndef _NTRACE
      sprintf(buffer, "%s_%d", Scheduling_name(scheduling), index);
#else//_NTRACE
      sprintf(buffer, "Oper_%d", index);
#endif//_NTRACE
    } else {
      sprintf(buffer, "Node_%d", index);
    }
  } else {
    sprintf(buffer, "Life_%d", index);
  }
  Except_ENSURE(strlen(buffer) < sizeof(buffer));
  return buffer;
}

#ifdef $XCC_h
/**
 * enumerates the DDGraph kinds.
 */
typedef enum {
  DDGraphKind_Early,	// Early dates DDGraph.
  DDGraphKind_Late,	// Late dates DDGraph.
  DDGraphKind_MCRL,	// MCRL dates DDGraph.
  DDGraphKind__
} enum_DDGraphKind;
typedef uint8_t DDGraphKind;
extern const char *
DDGraphKind_Id(DDGraphKind this);
#endif//$XCC_h

const char *
DDGraphKind_Id(DDGraphKind this)
{
  static const char *_[] = {
    "Early",
    "Late",
    "MCRL",
  };
  Except_REQUIRE((unsigned)this < DDGraphKind__);
  return _[this];
}

#ifdef $XCC_h
/**
 * Enumerates the DDGraph flags.
 */
typedef enum {
  DDGraphFlag_Forward = 0x0,	// DDGraph is forward.
  DDGraphFlag_Backward = 0x1,	// DDGraph is backward.
  DDGraphFlag_Lifetime = 0x2,	// DDGraph minimizes lifetimes.
  DDGraphFlag_Duplicate = 0x4,	// DDGraph is a duplicate.
} enum_DDGraphFlag;
typedef uint8_t DDGraphFlags;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Data Dependence graph.
 *
 * The DDGraph always contains a ROOT node, whose OPERATION is NULL. It also
 * contains the FIRST, LAST, START, STOP nodes, which are connected as follows:
 *
 * *	ROOT is connected to FIRST with latency 0,
 * *	FIRST is sequentially connected to START with latency 0,
 * *	START is connected to all nodes whose KEY is lower than STOP KEY with latency 0,
 * *	all nodes whose KEY is lower than STOP KEY are connected to STOP with latency 0,
 * *	STOP is sequentially connected to LAST with latency 0,
 * *	LAST is connected to FIRST with latency 1 - (OVERLAP + 1)*LAMBDA.
 *
 * The purpose of this LAST to FIRST arc is to limit the amount of software
 * pipelining. When non software pipelining, OVERLAP is zero. In most cases,
 * FIRST and START are the same node, and STOP and LAST are the same node. In
 * such cases the sequential connection means no arc at all. When two nodes in
 * these pairs differ, for instance FIRST and START, then sequential connection
 * means that all nodes whose KEY is between FIRST KEY and START KEY inclusive are
 * connected to the next in increasing KEY order. This feature is used in cases
 * Operations such as hardware-loop pseudos must remain a some fixed place in the
 * resulting schedule, such as before or after the real Operations.
 */
struct DDGraph_ {
  //@args	Memory memory, DDGraphKind kind, DDGraphFlags flags,
  //@args	Processor processor, int32_t maxNodes, int16_t *_lambda
  //@access ROOT	(DDGraphNode)Network_ROOTNODE(DDGraph_NETWORK(this))
  //@access MEMORY	Network_MEMORY(DDGraph_NETWORK(this))
  Network_ NETWORK[1];		// Base Network_: must be first in struct DDGraph_.
  //@access DGRAPH	Network_DGRAPH(DDGraph_NETWORK(this))
  //@access CRITICAL	Network_CRITICAL(DDGraph_NETWORK(this))
  //@mutate CRITICAL	Network__CRITICAL(DDGraph_NETWORK(this))
  PtrSeq_ CARRIED_;		// Stack of loop-carried DDGraphArc(s).
  //@access CARRIED	DDGraph__CARRIED_(this)
  DDGraphNode FIRST;		// First DDGraphNode of this DDGraph.
  //@access FIRSTINDEX	DDGraphNode_INDEX(DDGraph_FIRST(this))
  DDGraphNode START;		// Start DDGraphNode of this DDGraph.
  //@access STARTINDEX	DDGraphNode_INDEX(DDGraph_START(this))
  DDGraphNode STOP;		// Stop DDGraphNode of this DDGraph.
  //@access STOPINDEX	DDGraphNode_INDEX(DDGraph_STOP(this))
  DDGraphNode LAST;		// Last DDGraphNode of this DDGraph.
  //@access LASTINDEX	DDGraphNode_INDEX(DDGraph_LAST(this))
  DDGraphKind KIND;	// This DDGraph DDGraphKind.
  DDGraphFlags FLAGS;	// This DDGraph DDGraphFlags.
  //@access isForward	((DDGraph_flags(this) & DDGraphFlag_Backward) == 0)
  //@access isBackward	((DDGraph_flags(this) & DDGraphFlag_Backward) != 0)
  //@access isLifetime	((DDGraph_flags(this) & DDGraphFlag_Lifetime) != 0)
  //@access isDuplicate	((DDGraph_flags(this) & DDGraphFlag_Duplicate) != 0)
  Processor PROCESSOR;
  int8_t BAROMEGA;		// This DDGraph maximum OMEGA.
  int16_t *_LAMBDA;		// This DDGraph current LAMBDA.
  //@access LAMBDA	(*DDGraph__LAMBDA(this))
};
#endif//$XCC_h

DDGraph
DDGraph_Ctor(DDGraph this,
             Memory memory, DDGraphKind kind, DDGraphFlags flags,
             Processor processor, int32_t maxNodes, int16_t *_lambda)
{
  Network_Ctor(DDGraph_NETWORK(this), memory, maxNodes);
  PtrSeq_Ctor(DDGraph_CARRIED(this), memory, maxNodes);
  *DDGraph__FIRST(this) = NULL;
  *DDGraph__START(this) = NULL;
  *DDGraph__STOP(this) = NULL;
  *DDGraph__LAST(this) = NULL;
  *DDGraph__KIND(this) = kind;
  *DDGraph__FLAGS(this) = flags;
  *DDGraph__PROCESSOR(this) = processor;
  *DDGraph__BAROMEGA(this) = -1;
  *DDGraph___LAMBDA(this) = _lambda;
  return this;
}

void
DDGraph_Dtor(DDGraph this)
{
  Network_Dtor(DDGraph_NETWORK(this));
  PtrSeq_Dtor(DDGraph_CARRIED(this));
}

size_t
DDGraph_Size(Memory memory, DDGraphKind kind, DDGraphFlags flags,
             Processor processor, int32_t maxNodes, int16_t *_lambda)
{
  return sizeof(DDGraph_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(DDGraph_)\t%zu\n", sizeof(DDGraph_));
}
#endif//$XCC__c

#ifdef $XCC_h
DDGraph
DDGraph_new(Memory parent, DDGraphKind kind, DDGraphFlags flags,
            Processor processor, int32_t maxNodes, int16_t *_lambda);
#endif//$XCC_h

DDGraph
DDGraph_new(Memory parent, DDGraphKind kind, DDGraphFlags flags,
            Processor processor, int32_t maxNodes, int16_t *_lambda)
{
  Memory memory = Memory_new(parent, true);
  size_t size = DDGraph_Size(memory, kind, flags, processor, maxNodes, _lambda);
  DDGraph this = Memory_alloc(memory, size);
  return DDGraph_Ctor(this, memory, kind, flags, processor, maxNodes, _lambda);
}

#ifdef $XCC_h
DDGraph
DDGraph_delete(DDGraph this);
#endif//$XCC_h

DDGraph
DDGraph_delete(DDGraph this)
{
  if (this != NULL) {
    Memory memory = DDGraph_MEMORY(this);
#ifndef _NDTOR
    DDGraph_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * This DDGraph DGraph.
 */
static inline DGraph
DDGraph_dgraph(DDGraph this)
{
  return DDGraph_DGRAPH(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this DDGraph DDGraphNode(s).
 */
#define DDGraph_FOREACH_DDGraphNode(this, node) { \
  DGraph DDGraph_DGRAPH = DDGraph_dgraph(this); \
  DGraph_FOREACH(DDGraph_DGRAPH, DDGraphNode_, node)
#define DDGraph_ENDEACH_DDGraphNode \
  DGraph_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This DDGraph DDGraphNode array.
 */
static inline DDGraphNode *
DDGraph_nodes(DDGraph this)
{
  return (DDGraphNode *)DGraph_nodes(DDGraph_DGRAPH(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of DDGraph_nodes.
 */
static inline int32_t
DDGraph_nodeCount(DDGraph this)
{
  return DGraph_count(DDGraph_DGRAPH(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Index of the LAST DDGraphNode.
 *
 * Used to count nodes excluding the ROOT and lifetime nodes.
 */
static inline int32_t
DDGraph_lastIndex(DDGraph this)
{
  return DDGraph_LASTINDEX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access a DDGraphNode of this DDGraph by index.
 */
static inline DDGraphNode
DDGraph_access(DDGraph this, int32_t index)
{
  return (DDGraphNode)DGraph_access(DDGraph_DGRAPH(this), index);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This DDGraph flags.
 */
static inline DDGraphFlags
DDGraph_flags(DDGraph this)
{
  return DDGraph_FLAGS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This DDGraph Network.
 */
static inline Network
DDGraph_network(DDGraph this)
{
  return DDGraph_NETWORK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This DDGraph CARRIED PtrSeq.
 */
static inline PtrSeq
DDGraph_carried(DDGraph this)
{
  return DDGraph_CARRIED(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this DDGraph CARRIED arcs.
 */
#define DDGraph_FORCARRIED_DDGraphArc(this, arc) { \
  PtrSeq DDGraph_CARRIED = DDGraph_carried(this); \
  PtrSeq_FOREACH(DDGraph_CARRIED, DDGraphArc, arc) {
#define DDGraph_ENDCARRIED_DDGraphArc \
  } PtrSeq_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pretty-print this DDGraph.
 */
bool
DDGraph_pretty(DDGraph this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
DDGraph_pretty(DDGraph this, FILE *file)
{
  if (file == NULL) file = stderr;
  DDGraph_FOREACH_DDGraphNode(this, node) {
    fprintf(file, "    ");
    DDGraphNode_pretty(node, file);
    fprintf(file, "\n");
    DDGraphNode_FORLEAVE_DDGraphArc(node, arc) {
      fprintf(file, "\t");
      DDGraphArc_pretty(arc, file);
      fprintf(file, "\n");
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  } DDGraph_ENDEACH_DDGraphNode;
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Make a DDGraphNode in this DDGraph.
 */
DDGraphNode
DDGraph_makeNode(DDGraph this, Operation operation);
#endif//$XCC_h

DDGraphNode
DDGraph_makeNode(DDGraph this, Operation operation)
{
  size_t size = DDGraphNode_Size(operation);
  DDGraphNode node = DGraph_makeNode(DDGraph_DGRAPH(this), size);
  DDGraphNode_Ctor(node, operation);
  return node;
}

#ifdef $XCC_h
/**
 * Make a DDGraphArc in this DDGraph.
 */
DDGraphArc
DDGraph_makeArc(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                int theta, int omega, DependenceKind kind, Temporary temporary);
#endif//$XCC_h

DDGraphArc
DDGraph_makeArc(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                int theta, int omega, DependenceKind kind, Temporary temporary)
{
  NetArcFlags flags = 0;
  PtrSeq carried = DDGraph_CARRIED(this);
  size_t size = DDGraphArc_Size(theta, omega, kind, temporary, flags);
  DDGraphArc arc = DGraph_makeArc(DDGraph_DGRAPH(this), tail_node, head_node, size);
  if (kind < DependenceKind_Margin) {
    if (omega == 0) flags |= NetArcFlag_Acyclic;
    else PtrSeq_push2(carried, arc);
  }
  DDGraphArc_Ctor(arc, theta, omega, kind, temporary, flags);
  return arc;
}

#ifdef $XCC_h
/**
 * Sort the leaving arcs of this DDGraph.
 */
void
DDGraph_sortLeaveArcs(DDGraph this);
#endif//$XCC_h

void
DDGraph_sortLeaveArcs(DDGraph this)
{
  DDGraphNode root = DDGraph_ROOT(this);
  DDGraph_FOREACH_DDGraphNode(this, NODE) {
    if (NODE != root) {
      DGraphNode_sortLeaveArcs((DGraphNode)NODE,
          (int (*)(const_DGraphArc, const_DGraphArc))DDGraphArc_compare);
    }
  } DDGraph_ENDEACH_DDGraphNode;
}

#ifdef $XCC_h
/**
 * Ajust the arcs latencies and active status.
 *
 * TODO: adjust latencies across several arcs.
 */
void
DDGraph_adjustArcs(DDGraph this, Processor processor);
#endif//$XCC_h

void
DDGraph_adjustArcs(DDGraph this, Processor processor)
{
  if (!DDGraph_isDuplicate(this)) {
    PFA_TRACE && fprintf(PFA_TRACE, "*** ADJUST LATENCIES in %s for Processor_%s\n",
        DDGraphKind_Id(DDGraph_KIND(this)), Processor_Id(processor));
    DDGraph_FOREACH_DDGraphNode(this, tail_node) {
      Operation tail_operation = DDGraphNode_OPERATION(tail_node);
      if (tail_operation != NULL) {
        DDGraphNode_FORLEAVE_DDGraphArc(tail_node, arc) {
          int theta = DDGraphArc_THETA(arc);
          DependenceKind kind = DDGraphArc_KIND(arc);
          DDGraphNode head_node = DDGraphArc_HEAD(arc);
          Operation head_operation = DDGraphNode_OPERATION(head_node);
          if (head_operation != NULL && DependenceKind_mayAdjust(kind)) {
            IssueItem tail_item = Operation_issueItem(tail_operation);
            IssueItem head_item = Operation_issueItem(head_operation);
            Reservation tail_reservation = IssueItem_RESERVATION(tail_item);
            Reservation head_reservation = IssueItem_RESERVATION(head_item);
            int odot = Reservation_odot(tail_reservation, head_reservation, processor);
            if (odot > theta) {
              *DDGraphArc__THETA(arc) = *DDGraphArc__DELAY(arc) = odot;
              PFA_TRACE && fprintf(PFA_TRACE,
                                   "\t(_%d, _%d)\ttheta=%d\tomega=%d\t%s\todot=%d\n",
                                   DDGraphNode_INDEX(tail_node), DDGraphNode_INDEX(head_node),
                                   theta, DDGraphArc_OMEGA(arc), DependenceKind_Id(kind),
                                   odot);
            }
          }
        } DDGraphNode_ENDLEAVE_DDGraphArc;
      }
    } DDGraph_ENDEACH_DDGraphNode;
  }
}

#ifdef $XCC_h
/**
 * Complete the DDGraph with the auxiliary nodes and arcs.
 */
void
DDGraph_complete(DDGraph this, int overlap, int32_t serialize, Processor processor);
#endif//$XCC_h

void
DDGraph_complete(DDGraph this, int overlap, int32_t serialize, Processor processor)
{
  int32_t index = 0;
  DDGraphNode root = DDGraph_ROOT(this);
  DDGraphNode first = DDGraph_FIRST(this);
  DDGraphNode start = DDGraph_START(this);
  DDGraphNode stop = DDGraph_STOP(this);
  DDGraphNode last = DDGraph_LAST(this);
  int32_t firstIndex = DDGraph_FIRSTINDEX(this);
  int32_t startIndex = DDGraph_STARTINDEX(this);
  int32_t stop_index = DDGraph_STOPINDEX(this);
  int32_t lastIndex = DDGraph_LASTINDEX(this);
  Operation stop_operation = DDGraphNode_OPERATION(stop);
  IssueItem stop_issueItem = Operation_issueItem(stop_operation);
  int stop_processing = IssueItem_PROCESSING(stop_issueItem);
  // Make the arc ROOT -> FIRST or ROOT -> LAST.
  DDGraphArc arc = DDGraph_isForward(this)?
      DDGraph_makeArc(this, root, first, 0, 0, DependenceKind_Margin, NULL):
      DDGraph_makeArc(this, root, last, 0, 0, DependenceKind_Margin, NULL);
  *DDGraphArc__FLAGS(arc) |= NetArcFlag_Acyclic;
  Except_CHECK(   firstIndex <= startIndex
               && startIndex <= stop_index
               && stop_index <= lastIndex);
  if (!DDGraph_isDuplicate(this)) {
    // Make the sequence arcs FIRST -> ... START.
    for (index = firstIndex; index < startIndex; index++) {
      DDGraphNode tail_node = DDGraph_access(this, index);
      DDGraphNode head_node = DDGraph_access(this, index + 1);
      DDGraph_makeArc(this, tail_node, head_node, 0, 0, DependenceKind_Some, NULL);
    }
    // Make the serialization arcs START -> ... STOP.
    for (index = startIndex; serialize > 0 && index < stop_index; index++, serialize--) {
      DDGraphNode tail_node = DDGraph_access(this, index);
      DDGraphNode head_node = DDGraph_access(this, index + 1);
      DDGraph_makeArc(this, tail_node, head_node, 0, 0, DependenceKind_Some, NULL);
    }
    // Make the sequence arcs STOP -> ... LAST.
    for (index = stop_index; index < lastIndex; index++) {
      DDGraphNode tail_node = DDGraph_access(this, index);
      DDGraphNode head_node = DDGraph_access(this, index + 1);
      DDGraph_makeArc(this, tail_node, head_node, 0, 0, DependenceKind_Some, NULL);
    }
  }
  DDGraph_FOREACH_DDGraphNode(this, node) {
    int32_t nodeIndex = DDGraphNode_INDEX(node);
    if (!DDGraph_isDuplicate(this)) {
      // Add the arcs START -> NODE -> STOP.
      if (nodeIndex > startIndex && nodeIndex < stop_index) {
        Operation operation = DDGraphNode_OPERATION(node);
        IssueItem issueItem = Operation_issueItem(operation);
        int latency = IssueItem_PROCESSING(issueItem) - stop_processing;
        if (latency < 0) latency = 0;
        DDGraph_makeArc(this, start, node, 0, 0, DependenceKind_Some, NULL);
if (DDGraph_PROCESSING) {
        DDGraph_makeArc(this, node, stop, latency, 0, DependenceKind_Some, NULL);
} else {
        DDGraph_makeArc(this, node, stop, 0, 0, DependenceKind_Some, NULL);
}
      }
    }
    // Add the arcs ROOT -> NODE and NODE -> ROOT.
    if (nodeIndex >= firstIndex && nodeIndex <= lastIndex) {
      *DDGraphNode__ROOT2NODE(node) =
          DDGraph_makeArc(this, root, node, 0, 0, DependenceKind_Margin, NULL);
      *DDGraphNode__NODE2ROOT(node) =
          DDGraph_makeArc(this, node, root, INT16_MIN, 0, DependenceKind_Margin, NULL);
    }
  } DDGraph_ENDEACH_DDGraphNode;
  if (!DDGraph_isDuplicate(this)) {
    // Add an arc START -> STOP or STOP <- START.
    DDGraph_makeArc(this, start, stop, 0, 0, DependenceKind_Some, NULL);
    // Add the overlap-limiting arc unless overlap < 0.
    if (overlap >= 0) {
      Operation operation = DDGraphNode_OPERATION(stop);
if (DDGraph_PROCESSING) {
      DDGraph_makeArc(this, last, first, stop_processing, overlap + 1, DependenceKind_Some, NULL);
} else {
      DDGraph_makeArc(this, last, first, 1, overlap + 1, DependenceKind_Some, NULL);
}
    }
  }
  DDGraph_adjustArcs(this, processor);
  DDGraph_sortLeaveArcs(this);
}

#ifdef $XCC__h
/**
 * Return the lifetime node associated to @c prod_node.
 *
 * If the lifetime node does not exist, it is flow requirements are initialized,
 * and the flow requirements of the producer node are updated.
 */
DDGraphNode
DDGraph_makeLifeNode(DDGraph this, DDGraphNode prod_node, Temporary temporary);
#endif//$XCC__h

DDGraphNode
DDGraph_makeLifeNode(DDGraph this, DDGraphNode prod_node, Temporary temporary)
{
  DDGraphNode life_node = NULL;
  Operation operation = DDGraphNode_OPERATION(prod_node);
  Operator operator = Operation_operator(operation);
  int require = Operator_require(operator, Temporary_REGFILE(temporary));
  if (require > 0 && Temporary_isLifetime(temporary)) {
    DDGraphNode_FORLEAVE_DDGraphArc(prod_node, ARC) {
      if (   DDGraphArc_KIND(ARC) == DependenceKind_Link
          && DDGraphArc_TEMPORARY(ARC) == temporary) {
        life_node = DDGraphArc_HEAD(ARC);
        break;
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
    if (life_node == NULL) {
      DDGraphArc link_arc = NULL;
      life_node = DDGraph_makeNode(this, DDGraphNode_OPERATION(prod_node));
      // Create the flow requirements between the producer node and the lifetime node.
      *DDGraphNode__REQUIRE(prod_node) += require;
      *DDGraphNode__REQUIRE(life_node) -= require;
      // Link these two nodes so the problem can be solved by the primal network simplex.
      link_arc = DDGraph_makeArc(this, prod_node, life_node, 0, 0,
                                 DependenceKind_Link, temporary);
      PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\tLink ",
          DDGraphNode_INDEX(prod_node), DDGraphNode_INDEX(life_node), 0, 0);
      PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
      *DDGraphArc__FLAGS(link_arc) |= NetArcFlag_Precious;
    }
  }
  return life_node;
}

#ifdef $XCC__h
/**
 * Add a RAW register dependence arc between two DDGraphNodes.
 *
 * If @c lifeOK, a @c life_node is created and a Life arc is added between
 * @c head_node and @c life_node. This has the effect of pulling @c tail_node and
 * @c head_node together, when the network flow problem is solved, see B. Dupont de
 * Dinechin "Parametric Computation of Margins and of Minimum Cumulative Register
 * Lifetime Dates", LCPC'96 --	9th International Workshop on Languages and
 * Compilers for Parallel Computing, LNCS #1239.
 */
void
DDGraph_addRegisterRAW(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                       int theta, int omega, Temporary temporary, bool lifeOK, int relaxable);
#endif//$XCC__h

void
DDGraph_addRegisterRAW(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                       int theta, int omega, Temporary temporary, bool lifeOK, int relaxable)
{
  DependenceKind arc_type = relaxable > 0? DependenceKind_Relax: DependenceKind_RAW;
  DDGraph_makeArc(this, tail_node, head_node, theta, omega, arc_type, temporary);
  PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\t%s ",
                       DDGraphNode_INDEX(tail_node), DDGraphNode_INDEX(head_node),
                       theta, omega, DependenceKind_Id(arc_type));
  PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
  if (lifeOK && !relaxable) {
    DDGraphNode life_node = DDGraph_makeLifeNode(this, tail_node, temporary);
    if (life_node != NULL) {
      DDGraph_makeArc(this, head_node, life_node, 0, -omega,
                      DependenceKind_Life, temporary);
      PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\tLife ",
          DDGraphNode_INDEX(head_node), DDGraphNode_INDEX(life_node), 0, -omega);
      PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
    }
  }
}

#ifdef $XCC__h
/**
 * Add a WAR register dependence arc between two DDGraphNodes.
 *
 * The arc is omitted in case it is a self-arc that is guaranteed non-critical.
 */
void
DDGraph_addRegisterWAR(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                       int theta, int omega, Temporary temporary);
#endif//$XCC__h

void
DDGraph_addRegisterWAR(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                       int theta, int omega, Temporary temporary)
{
  if (tail_node != head_node || theta > omega) {
    DDGraph_makeArc(this, tail_node, head_node, theta, omega, DependenceKind_WAR, temporary);
    PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\tWAR ",
        DDGraphNode_INDEX(tail_node), DDGraphNode_INDEX(head_node), theta, omega);
    PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
  }
}

#ifdef $XCC__h
/**
 * Add a WAW register dependence arc between two DDGraphNodes.
 *
 * The arc is omitted in case it is a self-arc that is guaranteed non-critical.
 * If @c lifeOK, a @c life_node is created and a Life arc is added between
 * @c head_node and @c life_node. This has the effect of pulling @c tail_node and
 * @c head_node together. Even though both nodes write into the same register (WAW),
 * if they belong to the same Lifetime then @c tail_node does not kill the
 * register. In such case it makes sense to pull the nodes together.
 */
void
DDGraph_addRegisterWAW(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                       int theta, int omega, Temporary temporary, bool lifeOK, bool isKill);
#endif//$XCC__h

void
DDGraph_addRegisterWAW(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                       int theta, int omega, Temporary temporary, bool lifeOK, bool isKill)
{
  if (tail_node != head_node || theta > omega) {
    DependenceKind arc_type = DependenceKind_WAW;
    DDGraph_makeArc(this, tail_node, head_node, theta, omega, arc_type, temporary);
    PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\tWAW ",
        DDGraphNode_INDEX(tail_node), DDGraphNode_INDEX(head_node), theta, omega);
    PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
    if (lifeOK && !isKill) {
      DDGraphNode life_node = DDGraph_makeLifeNode(this, tail_node, temporary);
      if (life_node != NULL) {
        DDGraph_makeArc(this, head_node, life_node, 0, -omega,
                        DependenceKind_Life, temporary);
        PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\tLife ",
            DDGraphNode_INDEX(head_node), DDGraphNode_INDEX(life_node), 0, -omega);
        PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Add a Flow control dependence between two DDGraphNodes.
 *
 * The arc is omitted in case it is a self-arc that is guaranteed non-critical.
 */
void
DDGraph_addRegionFlow(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                      int theta, int omega, Temporary temporary);
#endif//$XCC__h

void
DDGraph_addRegionFlow(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                      int theta, int omega, Temporary temporary)
{
  if (tail_node != head_node || theta > omega) {
    DDGraph_makeArc(this, tail_node, head_node, theta, omega,
                    DependenceKind_Flow, temporary);
    PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\tFlow ",
        DDGraphNode_INDEX(tail_node), DDGraphNode_INDEX(head_node), theta, omega);
    PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
  }
}

#ifdef $XCC__h
/**
 * Add a Output control dependence between two DDGraphNodes.
 *
 * The arc is omitted in case it is a self-arc that is guaranteed non-critical.
 */
void
DDGraph_addControlOutput(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                         int theta, int omega, Temporary temporary);
#endif//$XCC__h

void
DDGraph_addControlOutput(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                         int theta, int omega, Temporary temporary)
{
  if (tail_node != head_node || theta > omega) {
    DDGraph_makeArc(this, tail_node, head_node, theta, omega,
                    DependenceKind_Output, temporary);
    PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\tOutput ",
        DDGraphNode_INDEX(tail_node), DDGraphNode_INDEX(head_node), theta, omega);
    PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
  }
}

#ifdef $XCC__h
/**
 * Add a Anti control dependence between two DDGraphNodes.
 *
 * The arc is omitted in case it is a self-arc that is guaranteed non-critical.
 */
void
DDGraph_addControlAnti(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                       int theta, int omega, Temporary temporary);
#endif//$XCC__h

void
DDGraph_addControlAnti(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                       int theta, int omega, Temporary temporary)
{
  if (tail_node != head_node || theta > omega) {
    DDGraph_makeArc(this, tail_node, head_node, theta, omega,
                    DependenceKind_Anti, temporary);
    PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\tAnti ",
        DDGraphNode_INDEX(tail_node), DDGraphNode_INDEX(head_node), theta, omega);
    PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
  }
}

#ifdef $XCC__h
/**
 * Add a memory dependence arc between two DDGraphNodes.
 */
void
DDGraph_addMemoryArc(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                     int theta, int omega, Temporary temporary, DependenceKind kind);
#endif//$XCC__h

void
DDGraph_addMemoryArc(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                     int theta, int omega, Temporary temporary, DependenceKind kind)
{
  DDGraph_makeArc(this, tail_node, head_node, theta, omega, kind, temporary);
  PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\t%s ",
      DDGraphNode_INDEX(tail_node), DDGraphNode_INDEX(head_node), theta, omega,
      DependenceKind_Id(kind));
  PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
}

#ifdef $XCC__h
/**
 * Add a sequence arc.
 */
void
DDGraph_addSequenceArc(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                       int theta, int omega, Temporary temporary);
#endif//$XCC__h

void
DDGraph_addSequenceArc(DDGraph this, DDGraphNode tail_node, DDGraphNode head_node,
                       int theta, int omega, Temporary temporary)
{
  DDGraph_makeArc(this, tail_node, head_node, theta, omega, DependenceKind_Other, temporary);
  PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\tSeq ",
      DDGraphNode_INDEX(tail_node), DDGraphNode_INDEX(head_node), theta, omega);
  PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
}

#ifdef $XCC__h
/**
 * Dependency computation for register resources.
 *
 * The outer loop iterates over the Write effects (effect1), and the two inner
 * loops that over the use effects (effect2 and effect4).
 */
void
DDGraph_addRegisterArcs(DDGraph this, Lifetime lifetime,
                        bool isAcyclic, bool isPrePass, int l1MissExtra);
#endif//$XCC__h

void
DDGraph_addRegisterArcs(DDGraph this, Lifetime lifetime,
                        bool isAcyclic, bool isPrePass, int l1MissExtra)
{
  DDGraphKind kind = DDGraph_KIND(this);
  Processor processor = DDGraph_PROCESSOR(this);
  bool isLifetime = DDGraph_isLifetime(this);
  bool isInduction = Lifetime_isInduction(lifetime);
  bool isLifeOK = isPrePass && isLifetime;
  Effect *_firstEffect = Lifetime__firstEffect(lifetime);
  Effect *_lastEffect = Lifetime__lastEffect(lifetime);
  Temporary temporary = Lifetime_TEMPORARY(lifetime);
  Effect *_effect = _firstEffect;
  for (; _effect <= _lastEffect; _effect++) {
    Operation operation = Effect_OPERATION(*_effect);
    DDGraphNode node = IssueItem_DDGNODES(Operation_issueItem(operation))[kind];
    if (Effect_isClobber(*_effect)) continue;
    if (Effect_isWrite(*_effect)) {
      Effect *_effect1 = _effect;
      Effect effect3 = Effect_SUCC(*_effect);
      if (effect3 != NULL && !Effect_isClobber(effect3)) {
        Operation operation1 = operation, operation3 = Effect_OPERATION(effect3);
        DDGraphNode node1 = node;
        DDGraphNode node3 = IssueItem_DDGNODES(Operation_issueItem(operation3))[kind];
        int theta = Effect_getRegisterWAWLatency(*_effect1, effect3, processor);
        int carried = DDGraphNode_INDEX(node1) >= DDGraphNode_INDEX(node3);
        if (theta >= 0 && (!isAcyclic || !carried)) {
          DDGraph_addRegisterWAW(this, node1, node3, theta, carried, temporary,
                                 isLifeOK, Effect_isKill(effect3));
        }
        Except_CHECK(Effect_isWrite(effect3));
      }
    } else {
      Effect effect1 = Effect_PRED(*_effect);
      Effect *_effect2 = _effect, *_effect4 = _effect;
      Effect effect3 = Effect_SUCC(*_effect);
      if (/*effect1 != NULL*/1 && !Effect_isClobber(effect1)) {
        int relaxable = Effect_RELAXABLE(*_effect2);
        Operation operation1 = Effect_OPERATION(effect1), operation2 = operation;
        DDGraphNode node1 = IssueItem_DDGNODES(Operation_issueItem(operation1))[kind];
        DDGraphNode node2 = node;
        int extra = Operation_isMemoryLevel2(operation1)? l1MissExtra: 0;
        int theta = Effect_getRegisterRAWLatency(effect1, *_effect2, processor, extra);
        int carried = (DDGraphNode_INDEX(node1) >= DDGraphNode_INDEX(node2)) + relaxable;
        if (theta >= 0 && (!isAcyclic || !carried)) {
          DDGraph_addRegisterRAW(this, node1, node2, theta, carried, temporary,
                                 isLifeOK, relaxable);
        }
        Except_CHECK(!relaxable || isInduction);
        Except_CHECK(Effect_isWrite(effect1));
      }
      if (effect3 != NULL && !Effect_isClobber(effect3)) {
        Operation operation3 = Effect_OPERATION(effect3), operation4 = operation;
        DDGraphNode node3 = IssueItem_DDGNODES(Operation_issueItem(operation3))[kind];
        DDGraphNode node4 = node;
        int theta = Effect_getRegisterWARLatency(*_effect4, effect3, processor);
        int carried = DDGraphNode_INDEX(node4) >= DDGraphNode_INDEX(node3);
        if (theta >= 0 && (!isAcyclic || !carried)) {
          DDGraph_addRegisterWAR(this, node4, node3, theta, carried, temporary);
        }
        Except_CHECK(Effect_isWrite(effect3));
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Dependency computation for control resources.
 *
 * The main differences with the Register resources are: control speculation is
 * enabled by removing some control dependences, depending on the value of
 * speculation; and anti dependences are not enforced if compensation is allowed
 * because an instruction can be pushed below an exit branch.
 */
void
DDGraph_addControlArcs(DDGraph this, Lifetime lifetime, bool isAcyclic);
#endif//$XCC__h

void
DDGraph_addControlArcs(DDGraph this, Lifetime lifetime, bool isAcyclic)
{
  DDGraphKind kind = DDGraph_KIND(this);
  Processor processor = DDGraph_PROCESSOR(this);
  Effect *_firstEffect = Lifetime__firstEffect(lifetime);
  Effect *_lastEffect = Lifetime__lastEffect(lifetime);
  Temporary temporary = Lifetime_TEMPORARY(lifetime);
  Effect *_effect = _firstEffect;
  for (; _effect <= _lastEffect; _effect++) {
    Operation operation = Effect_OPERATION(*_effect);
    Operator operator = Operation_operator(operation);
    DDGraphNode node = IssueItem_DDGNODES(Operation_issueItem(operation))[kind];
    if (Effect_isWrite(*_effect)) {
      Effect *_effect1 = _effect;
      Effect effect3 = Effect_SUCC(*_effect);
      if (/*effect3 != NULL*/1) {
        Operation operation1 = operation, operation3 = Effect_OPERATION(effect3);
        DDGraphNode node1 = node;
        DDGraphNode node3 = IssueItem_DDGNODES(Operation_issueItem(operation3))[kind];
        int theta = Effect_getControlOutputLatency(*_effect1, effect3, processor);
        int carried = DDGraphNode_INDEX(node1) >= DDGraphNode_INDEX(node3);
        if (theta >= 0 && (!isAcyclic || !carried)) {
          DDGraph_addControlOutput(this, node1, node3, theta, carried, temporary);
        }
        Except_CHECK(Effect_isWrite(effect3));
      }
    } else {
      Effect effect1 = Effect_PRED(*_effect);
      Effect *_effect2 = _effect, *_effect4 = _effect;
      Effect effect3 = Effect_SUCC(*_effect);
      if (/*effect1 != NULL*/1) {
        Operation operation2 = operation;
        // Add the flow control dependence unless operation2 is speculable.
        if (Operation_isSpeculable(operation2)) {
        } else {
          Effect effect0 = effect1;
          int boosting = Operation_boosting(operation2), carried = 0;
          while (boosting--) {
            effect1 = Effect_PRED(effect1);
            if (effect1 == effect0) carried++;
          }
          {
            Operation operation1 = Effect_OPERATION(effect1);
            DDGraphNode node1 = IssueItem_DDGNODES(Operation_issueItem(operation1))[kind];
            DDGraphNode node2 = node;
            int theta = Effect_getRegionFlowLatency(effect1, *_effect2, processor);
            carried += DDGraphNode_INDEX(node1) >= DDGraphNode_INDEX(node2);
            if (theta >= 0 && (!isAcyclic || !carried)) {
              DDGraph_addRegionFlow(this, node1, node2, theta, carried, temporary);
            }
          }
        }
        Except_CHECK(Effect_isWrite(effect1));
      }
      // Add the anti control dependence unless operation4 is compensable.
      if (/*effect3 != NULL*/1) {
        Operation operation3 = Effect_OPERATION(effect3), operation4 = operation;
        Operator operator3 = Operation_operator(operation3), operator4 = operator;
        if (Operation_isCompensable(operation4)) {
        } else {
          DDGraphNode node3 = IssueItem_DDGNODES(Operation_issueItem(operation3))[kind];
          DDGraphNode node4 = node;
          int theta = Effect_getControlAntiLatency(*_effect4, effect3, processor);
          int carried = DDGraphNode_INDEX(node4) >= DDGraphNode_INDEX(node3);
          if (theta >= 0 && (!isAcyclic || !carried)) {
            DDGraph_addControlAnti(this, node4, node3, theta, carried, temporary);
          }
        }
        Except_CHECK(Effect_isWrite(effect3));
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Dependency computation for the memory resources.
 *
 * The computation is similar to DDGraph_addRegisterArcs.
 */
void
DDGraph_addMemoryArcs(DDGraph this, Lifetime lifetime, bool isAcyclic, const_Configure configure);
#endif//$XCC__h

void
DDGraph_addMemoryArcs(DDGraph this, Lifetime lifetime, bool isAcyclic, const_Configure configure)
{
  DDGraphKind kind = DDGraph_KIND(this);
  Processor processor = DDGraph_PROCESSOR(this);
  Effect *_firstEffect = Lifetime__firstEffect(lifetime);
  Effect *_lastEffect = Lifetime__lastEffect(lifetime);
  Temporary temporary = Lifetime_TEMPORARY(lifetime);
  ConfigureAliasing aliasing = Configure_ALIASING(configure);
  Effect *_effect = _firstEffect;
  for (; _effect <= _lastEffect; _effect++) {
    Operation operation = Effect_OPERATION(*_effect);
    DDGraphNode node = IssueItem_DDGNODES(Operation_issueItem(operation))[kind];
    if (Effect_isWrite(*_effect)) {
      Effect *_effect1 = _effect;
      Effect effect3 = Effect_SUCC(*_effect);
      if (effect3 != NULL) {
        Operation operation1 = operation, operation3 = Effect_OPERATION(effect3);
        DDGraphNode node1 = node;
        DDGraphNode node3 = IssueItem_DDGNODES(Operation_issueItem(operation3))[kind];
        int carried = DDGraphNode_INDEX(node1) >= DDGraphNode_INDEX(node3);
        if (!carried || (!isAcyclic /*&& aliasing < ConfigureAliasing_Vector*/)) {
          int theta = Effect_getMemoryOutputLatency(*_effect1, effect3, processor);
          DDGraph_addMemoryArc(this, node1, node3, theta, carried, temporary,
                               DependenceKind_Output);
        }
        Except_CHECK(Effect_isWrite(effect3));
      }
    } else {
      Effect effect1 = Effect_PRED(*_effect);
      Effect *_effect2 = _effect, *_effect4 = _effect;
      Effect effect3 = Effect_SUCC(*_effect);
      if (effect1 != NULL) {
        Operation operation1 = Effect_OPERATION(effect1), operation2 = operation;
        DDGraphNode node1 = IssueItem_DDGNODES(Operation_issueItem(operation1))[kind];
        DDGraphNode node2 = node;
        int carried = DDGraphNode_INDEX(node1) >= DDGraphNode_INDEX(node2);
        if (!carried || (!isAcyclic /*&& aliasing < ConfigureAliasing_Vector*/)) {
          int theta = Effect_getMemoryFlowLatency(effect1, *_effect2, processor);
          DDGraph_addMemoryArc(this, node1, node2, theta, carried, temporary,
                               DependenceKind_Flow);
        }
        Except_CHECK(Effect_isWrite(effect1));
      }
      if (effect3 != NULL) {
        Operation operation3 = Effect_OPERATION(effect3), operation4 = operation;
        DDGraphNode node3 = IssueItem_DDGNODES(Operation_issueItem(operation3))[kind];
        DDGraphNode node4 = node;
        int carried = DDGraphNode_INDEX(node4) >= DDGraphNode_INDEX(node3);
        if (!carried || (!isAcyclic /*&& aliasing < ConfigureAliasing_Vector*/)) {
          int theta = Effect_getMemoryAntiLatency(*_effect4, effect3, processor);
          DDGraph_addMemoryArc(this, node4, node3, theta, carried, temporary,
                               DependenceKind_Anti);
        }
        Except_CHECK(Effect_isWrite(effect3));
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Add the LoopScope Dependences to this DDGraph.
 */
void
DDGraph_addDependences(DDGraph this,
                       PtrSet ddgNodes, const_DependenceTable dependenceTable,
                       Temporary temporary, bool isAcyclic, const_Configure configure);
#endif//$XCC__h

void
DDGraph_addDependences(DDGraph this,
                       PtrSet ddgNodes, const_DependenceTable dependenceTable,
                       Temporary temporary, bool isAcyclic, const_Configure configure)
{
  int barOmega = !isAcyclic;
  DDGraphKind kind = DDGraph_KIND(this);
  int overlap = Configure_OVERLAP(configure);
  ConfigureAliasing aliasing = Configure_ALIASING(configure);
  if (PFA_TRACE) {
    fprintf(PFA_TRACE, "*** DEPENDENCE NODES\n");
    fprintf(PFA_TRACE, "\t");
    PtrSet_FORSORT(ddgNodes, Operation_compareIndices, Operation, operation) {
      fprintf(PFA_TRACE, "_%d ", Operation_index(operation));
    } PtrSet_ENDSORT;
    fprintf(PFA_TRACE, "\n");
  }
  PFA_TRACE && fprintf(PFA_TRACE, "*** ASSUMED DEPENDENCES\n");
  DependenceTable_FOREACH_DependenceArc(dependenceTable, arc) {
    Operation tail_operation = DependenceArc_TAIL(arc);
    Operation head_operation = DependenceArc_HEAD(arc);
    if (PtrSet_contains(ddgNodes, tail_operation) &&
        PtrSet_contains(ddgNodes, head_operation)) {
      DDGraphNode tail_node = IssueItem_DDGNODES(Operation_issueItem(tail_operation))[kind];
      DDGraphNode head_node = IssueItem_DDGNODES(Operation_issueItem(head_operation))[kind];
      int carried = DDGraphNode_INDEX(tail_node) >= DDGraphNode_INDEX(head_node);
      int theta = DependenceArc_LATENCY(arc);
      int omega = DependenceArc_OMEGA(arc);
      if (omega <= overlap) {
        if (carried < omega) carried = omega;
        if (!carried || (!isAcyclic /*&& aliasing < ConfigureAliasing_Vector*/)) {
          if (tail_node != head_node || theta > carried) {
            DependenceKind kind = DependenceArc_KIND(arc);
            DDGraph_makeArc(this, tail_node, head_node, theta, carried, kind, temporary);
            PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\ttheta=%d\tomega=%d\t%s Memory\n",
                                 DDGraphNode_INDEX(tail_node), DDGraphNode_INDEX(head_node),
                                 theta, carried, DependenceKind_Id(kind));
            if (barOmega < carried) barOmega = carried;
          }
        }
      }
    }
  } DependenceTable_ENDEACH_DependenceArc;
  Except_CHECK(barOmega == 0 || !isAcyclic);
  *DDGraph__BAROMEGA(this) = barOmega;
}

#ifdef $XCC__h
/**
 * Add dependences between SP update and memory operations.
 */
void
DDGraph_addSPUpdateArcs(DDGraph this, BlockTrace blockTrace,
                        bool isAcyclic, Register stackRegister);
#endif//$XCC__h

void
DDGraph_addSPUpdateArcs(DDGraph this, BlockTrace blockTrace,
                        bool isAcyclic, Register stackRegister)
{
  DDGraphKind kind = DDGraph_KIND(this);
  DDGraphNode write_node = NULL, save_node = NULL;
  BlockTrace_FORBACK_IssueItem(blockTrace, item) {
    Operation operation = IssueItem_OPERATION(item);
    Operation_RESULTS_FOREACH_Temporary(operation, result) {
      if (   Temporary_hasRegister(result)
          && Temporary_REGISTER(result) == stackRegister) {
        write_node = IssueItem_DDGNODES(Operation_issueItem(operation))[kind];
        break;
      }
    } Operation_RESULTS_ENDEACH_Temporary;
    if (write_node != NULL) break;
  } BlockTrace_ENDBACK_IssueItem;
  if (write_node == NULL) return;
  // Forward sweep for ADJUST -> memory_operation arcs.
  BlockTrace_FOREACH_IssueItem(blockTrace, item) {
    Operation operation = IssueItem_OPERATION(item);
    Operator operator = Operation_operator(operation);
    if (Operator_isMemory(operator)) {
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        if (   Temporary_hasRegister(argument)
            && Temporary_REGISTER(argument) == stackRegister) {
          DDGraphNode read_node = IssueItem_DDGNODES(Operation_issueItem(operation))[kind];
          int carried = (DDGraphNode_INDEX(write_node) >= DDGraphNode_INDEX(read_node));
          if (!carried || !isAcyclic) {
            DDGraph_addSequenceArc(this, write_node, read_node, 0, carried, argument);
          }
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
    }
    Operation_RESULTS_FOREACH_Temporary(operation, result) {
      if (   Temporary_hasRegister(result)
          && Temporary_REGISTER(result) == stackRegister) {
        write_node = IssueItem_DDGNODES(Operation_issueItem(operation))[kind];
        if (save_node == NULL) save_node = write_node;
      }
    } Operation_RESULTS_ENDEACH_Temporary;
  } BlockTrace_ENDEACH_IssueItem;
  // Backward sweep for memory_operation -> ADJUST arcs.
  write_node = save_node;
  BlockTrace_FORBACK_IssueItem(blockTrace, item) {
    Operation operation = IssueItem_OPERATION(item);
    Operator operator = Operation_operator(operation);
    Operation_RESULTS_FOREACH_Temporary(operation, result) {
      if (   Temporary_hasRegister(result)
          && Temporary_REGISTER(result) == stackRegister) {
        write_node = IssueItem_DDGNODES(Operation_issueItem(operation))[kind];
      }
    } Operation_RESULTS_ENDEACH_Temporary;
    if (Operator_isMemory(operator)) {
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        if (   Temporary_hasRegister(argument)
            && Temporary_REGISTER(argument) == stackRegister) {
          DDGraphNode read_node = IssueItem_DDGNODES(Operation_issueItem(operation))[kind];
          int carried = (DDGraphNode_INDEX(read_node) >= DDGraphNode_INDEX(write_node));
          if (!carried || !isAcyclic) {
            DDGraph_addSequenceArc(this, read_node, write_node, 0, carried, argument);
          }
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
    }
  } BlockTrace_ENDBACK_IssueItem;
}

#ifdef $XCC__h
/**
 * Add the volatile arcs.
 */
void
DDGraph_addVolatileArcs(DDGraph this, Lifetime lifetime, bool isAcyclic);
#endif//$XCC__h

void
DDGraph_addVolatileArcs(DDGraph this, Lifetime lifetime, bool isAcyclic)
{
  DDGraphKind kind = DDGraph_KIND(this);
  Effect *_firstEffect = Lifetime__firstEffect(lifetime);
  Effect *_lastEffect = Lifetime__lastEffect(lifetime);
  Temporary temporary = Lifetime_TEMPORARY(lifetime);
  Effect *_effect1 = _firstEffect, *_effect2 = _effect1 + 1;
  for (; _effect2 <= _lastEffect; _effect1++, _effect2++) {
    Operation operation1 = Effect_OPERATION(*_effect1);
    Operation operation2 = Effect_OPERATION(*_effect2);
    DDGraphNode node1 = IssueItem_DDGNODES(Operation_issueItem(operation1))[kind];
    DDGraphNode node2 = IssueItem_DDGNODES(Operation_issueItem(operation2))[kind];
    int carried = DDGraphNode_INDEX(node1) >= DDGraphNode_INDEX(node2);
    if (!isAcyclic || !carried) {
      DDGraph_addSequenceArc(this, node1, node2, 1, carried, temporary);
    }
  }
}

#ifdef $XCC_h
/**
 * Enter the nodes in this DDGraph.
 */
void
DDGraph_enterNodes(DDGraph this, BlockTrace blockTrace);
#endif//$XCC_h

void
DDGraph_enterNodes(DDGraph this, BlockTrace blockTrace)
{
  int requirePush = 0; //BlockTrace_isPrePass(blockTrace)? -4: 0;
  int requireTotal = 0;
  DDGraphKind kind = DDGraph_KIND(this);
  DDGraphNode first = NULL, last = NULL;
  DDGraphNode start = NULL, stop = NULL;
  DDGraphNode root = DDGraph_makeNode(this, NULL);
  if (!DDGraph_isLifetime(this)) requirePush = 0;
  // Make this DDGraph nodes from the IssueItems.
  BlockTrace_FOREACH_IssueItem(blockTrace, ITEM) {
    Operation OPERATION = IssueItem_OPERATION(ITEM);
    DDGraphNode NODE = DDGraph_makeNode(this, OPERATION);
    Except_CHECK(Operation_index(OPERATION) == DDGraphNode_INDEX(NODE));
    IssueItem_DDGNODES(ITEM)[kind] = NODE;
    // Add the NODE flow requirements.
    if (DDGraph_isLifetime(this)) {
      if (IssueItem_EXITARC(ITEM) != NULL) {
        *DDGraphNode__REQUIRE(NODE) += requirePush;
        requireTotal += requirePush;
      }
    }
    // Set the first, start, stop, and last.
    if (OPERATION == IssueItem_OPERATION(BlockTrace_FIRSTITEM(blockTrace))) {
      *DDGraph__FIRST(this) = first = NODE;
    }
    if (OPERATION == IssueItem_OPERATION(BlockTrace_STARTITEM(blockTrace))) {
      *DDGraph__START(this) = start = NODE;
    }
    if (OPERATION == IssueItem_OPERATION(BlockTrace_STOPITEM(blockTrace))) {
      *DDGraph__STOP(this) = stop = NODE;
    }
    if (OPERATION == IssueItem_OPERATION(BlockTrace_LASTITEM(blockTrace))) {
      *DDGraph__LAST(this) = last = NODE;
    }
  } BlockTrace_ENDEACH_IssueItem;
  // Set the first flow requirements.
  if (DDGraph_isLifetime(this)) {
    if (requirePush > 0) {
      // Push branches late in the schedule.
      *DDGraphNode__REQUIRE(last) -= requireTotal;
    } else if (requirePush < 0) {
      // Pull branches early in the schedule.
      *DDGraphNode__REQUIRE(first) -= requireTotal;
    }
  }
}

#ifdef $XCC_h
/**
 * Enter the arcs in this DDGraph.
 */
void
DDGraph_enterArcs(DDGraph this, BlockTrace blockTrace);
#endif//$XCC_h

void
DDGraph_enterArcs(DDGraph this, BlockTrace blockTrace)
{
  PtrSet ddgNodes = BlockTrace_DDGNODES(blockTrace);
  bool isAcyclic = BlockTrace_isAcyclic(blockTrace);
  bool isPrePass = BlockTrace_isPrePass(blockTrace);
  int overlap = BlockTrace_OVERLAP(blockTrace);
  Optimize optimize = BlockTrace_OPTIMIZE(blockTrace);
  Processor processor = BlockTrace_PROCESSOR(blockTrace);
  Configure configure = BlockTrace_CONFIGURE(blockTrace);
  Convention convention = BlockTrace_CONVENTION(blockTrace);
  Register stackRegister = Convention_stackRegister(convention);
  int l1MissExtra = Configure_L1MISSEXTRA(configure);
  if (l1MissExtra < 0) l1MissExtra = 0;
  *DDGraph__BAROMEGA(this) = !isAcyclic;
  if (DDGraph_isForward(this)) {
    // Enter the explicit memory dependences.
    const_DependenceTable dependenceTable = BlockTrace_getDependenceTable(blockTrace);
    if (   !PtrSet_isEmpty(ddgNodes) && dependenceTable != NULL
        && !DependenceTable_isEmpty(dependenceTable)) {
      RegionFlow regionFlow = BlockTrace_regionFlow(blockTrace);
      Procedure procedure = RegionFlow_procedure(regionFlow);
      Temporary temporary = Procedure_dedicated(procedure) + Register__Memory;
      DDGraph_addDependences(this, ddgNodes, dependenceTable, temporary, isAcyclic, configure);
    }
    // Enter the arcs implied by each lifetime.
    PFA_TRACE && fprintf(PFA_TRACE, "*** COMPUTED DEPENDENCES\n");
    LifetimeTable_FOREACH_Lifetime(BlockTrace_LIFETIMETABLE(blockTrace), LIFETIME) {
      Temporary temporary = Lifetime_TEMPORARY(LIFETIME);
      PFA_TRACE && Lifetime_pretty(LIFETIME, PFA_TRACE);
      if (Temporary_hasRegFile(temporary)) {
        if (!Lifetime_isOnlyUses(LIFETIME) && !Temporary_isConstant(temporary)) {
          DDGraph_addRegisterArcs(this, LIFETIME, isAcyclic, isPrePass, l1MissExtra);
        }
        if (   Temporary_hasRegister(temporary)
            && Temporary_REGISTER(temporary) == stackRegister) {
          DDGraph_addSPUpdateArcs(this, blockTrace, isAcyclic, stackRegister);
        }
      } else if (Temporary_isControl(temporary)) {
        if (!Lifetime_isOnlyUses(LIFETIME)) {
          DDGraph_addControlArcs(this, LIFETIME, isAcyclic);
        }
      } else if (Temporary_isMemory(temporary)) {
        if (!Lifetime_isOnlyUses(LIFETIME)) {
          DDGraph_addMemoryArcs(this, LIFETIME, isAcyclic, configure);
        }
      } else if (Temporary_isVolatile(temporary)) {
        DDGraph_addVolatileArcs(this, LIFETIME, isAcyclic);
      } else Except_NEVER(true);
    } LifetimeTable_ENDEACH_Lifetime;
  } else Except_NEVER(true);
}

#ifdef $XCC_h
/**
 * Enter the schedule arcs into this DDGraph from that DDGraph.
 */
void
DDGraph_copyArcs(DDGraph this, DDGraph that);
#endif//$XCC_h

void
DDGraph_copyArcs(DDGraph this, DDGraph that)
{
  int32_t lastIndex = DDGraph_LASTINDEX(this);
  Except_REQUIRE(DDGraph_isForward(that));
  *DDGraph__FLAGS(this) |= DDGraphFlag_Duplicate;
  DDGraph_FOREACH_DDGraphNode(that, that_tail) {
    int32_t tailIndex = DDGraphNode_INDEX(that_tail);
    if (tailIndex <= lastIndex) {
      DDGraphNode this_tail = DDGraph_access(this, tailIndex);
      Except_CHECK(DDGraphNode_OPERATION(this_tail) == DDGraphNode_OPERATION(that_tail));
      DDGraphNode_FORLEAVE_DDGraphArc(that_tail, that_arc) {
        DDGraphNode that_head = DDGraphArc_HEAD(that_arc);
        int32_t headIndex = DDGraphNode_INDEX(that_head);
        DependenceKind kind = DDGraphArc_KIND(that_arc);
        if (headIndex <= lastIndex &&
            !DDGraphArc_isUseless(that_arc) &&
            kind < DependenceKind_Margin) {
          DDGraphNode this_head = DDGraph_access(this, headIndex);
          int theta = DDGraphArc_THETA(that_arc), omega = DDGraphArc_OMEGA(that_arc);
          Temporary temporary = DDGraphArc_TEMPORARY(that_arc);
          DDGraphArc arc = DDGraph_isForward(this)?
              DDGraph_makeArc(this, this_tail, this_head, theta, omega, kind, temporary):
              DDGraph_makeArc(this, this_head, this_tail, theta, omega, kind, temporary);
        }
      } DDGraphNode_ENDLEAVE_DDGraphArc;
    }
  } DDGraph_ENDEACH_DDGraphNode;
}

#ifdef $XCC_h
/**
 * Synchronize the underlying Network for a new LAMBDA.
 *
 * Network primal feasibility (correct flows) is maintained. Network dual
 * feasibility is likely to be destroyed because the arcs that are not on the
 * spanning tree may end up with a positive reduced delay.
 */
void
DDGraph_synchronize(DDGraph this);
#endif//$XCC_h

void
DDGraph_synchronize(DDGraph this)
{
  DDGraphNode root = DDGraph_ROOT(this);
  int16_t lambda = DDGraph_LAMBDA(this);
  DDGraph_FORCARRIED_DDGraphArc(this, ARC) {
    *DDGraphArc__DELAY(ARC) = DDGraphArc_THETA(ARC) - lambda*DDGraphArc_OMEGA(ARC);
  } DDGraph_ENDCARRIED_DDGraphArc;
  DDGraphNode_FORLEAVE_DDGraphArc(root, ARC) {
    *DDGraphArc__DELAY(ARC) = DDGraphArc_THETA(ARC) - lambda*DDGraphArc_OMEGA(ARC);
  } DDGraphNode_ENDLEAVE_DDGraphArc;
  DDGraphNode_FORENTER_DDGraphArc(root, ARC) {
    *DDGraphArc__DELAY(ARC) = DDGraphArc_THETA(ARC) - lambda*DDGraphArc_OMEGA(ARC);
  } DDGraphNode_ENDENTER_DDGraphArc;
  Network_doCNP(DDGraph_NETWORK(this));
}

#ifdef $XCC_h
/**
 * Critical cycle call-back for the Simplex Label Correcting.
 */
bool
DDGraph_Critical(Network network);
#endif//$XCC_h

bool
DDGraph_Critical(Network network)
{
  DDGraph this = (DDGraph)network;
  int theta = 0, omega = 0, lambda = 0;
  PtrSeq circuit = Network_circuit(network);
  Except_CHECK(DDGraph_NETWORK(this) == network);
  PFA_TRACE && fprintf(PFA_TRACE, "*** CRITICAL CYCLE in %s",
      DDGraphKind_Id(DDGraph_KIND(this)));
  PtrSeq_FORBACK(circuit, DDGraphArc, arc) {
    theta += DDGraphArc_THETA(arc);
    omega += DDGraphArc_OMEGA(arc);
    PFA_TRACE && fprintf(PFA_TRACE, "\n\t") && DDGraphArc_pretty(arc, PFA_TRACE);
  } PtrSeq_ENDBACK;
  lambda = (theta + omega - 1)/omega;
  PFA_TRACE && fprintf(PFA_TRACE, "\n");
  PFA_TRACE && fprintf(PFA_TRACE, "*** CRITICAL LAMBDA=%d\n", lambda);
  if (lambda > DDGraph_LAMBDA(this)) {
    *DDGraph__LAMBDA(this) = lambda;
    DDGraph_synchronize(this);
  }
  return true;
}

#ifdef $XCC_h
/**
 * Solve the initial network flow problem.
 */
void
DDGraph_initializeFlows(DDGraph this, bool lifetime);
#endif//$XCC_h

void
DDGraph_initializeFlows(DDGraph this, bool lifetime)
{
  Network network = DDGraph_NETWORK(this);
  DDGraphNode first = DDGraph_FIRST(this);
  DDGraphArc root2node = DDGraphNode_ROOT2NODE(first);
  DDGraphArc node2root = DDGraphNode_NODE2ROOT(first);
  int theta = DDGraph_isForward(this)? 0: DDGraph_EXTENT;
  *DDGraphArc__THETA(root2node) = theta;
  *DDGraphArc__THETA(node2root) = -theta;
  *DDGraphArc__OMEGA(root2node) = 0;
  *DDGraphArc__OMEGA(node2root) = 0;
  *Network__CRITICAL(network) = DDGraph_Critical;
  DDGraph_synchronize(this);
  Network_activate(network);
  Network_doTSO(network);
  Network_doFLP(network);
  Network_doSLC(network, false);
  if (lifetime && DDGraph_isLifetime(this)) {
    Network_doCEF(network);
    Network_dualSolve(network);
    // TODO: restore strong feasibility.
  }
  Except_CHECK(Network_checkSolved(network));
}

#ifdef $XCC_h
/**
 * Increase LAMBDA just enough to satisfy dependences.
 */
void
DDGraph_increaseLambda(DDGraph this);
#endif//$XCC_h

void
DDGraph_increaseLambda(DDGraph this)
{
  Network network = DDGraph_NETWORK(this);
  //*Network__CRITICAL(network) = NULL;
  Network_doSLC(network, true);
}

#ifdef $XCC_h
/**
 * Optimize the network flow problem.
 */
void
DDGraph_optimizeFlows(DDGraph this, bool refill);
#endif//$XCC_h

void
DDGraph_optimizeFlows(DDGraph this, bool refill)
{
  Network network = DDGraph_NETWORK(this);
  if (refill) DDGraph_synchronize(this);
  if (DDGraph_isLifetime(this)) {
    Network_primalSolve(network, refill);
  } else {
    Network_doSLC(network, refill);
  }
}

#ifdef $XCC_h
/**
 * Freeze @c node at the supplied values of @c theta and
 * @c omega from the root node.
 *
 * Freezing a node is achieved by setting the opposite parametric lengths on the
 * pair of arcs between the root node and the frozen node. We call Network_updateDelay
 * in order to maintain consistency of the Network flows.
 */
void
DDGraph_freezeNode(DDGraph this, DDGraphNode node, int theta, int omega);
#endif//$XCC_h

void
DDGraph_freezeNode(DDGraph this, DDGraphNode node, int theta, int omega)
{
  int lambda = DDGraph_LAMBDA(this);
  int32_t delay = theta - lambda*omega;
  DDGraphArc root2node = DDGraphNode_ROOT2NODE(node);
  DDGraphArc node2root = DDGraphNode_NODE2ROOT(node);
  Network network = DDGraph_NETWORK(this);
  if (DDGraphArc_THETA(root2node) != theta ||
      DDGraphArc_OMEGA(root2node) != omega) {
    NetArc netArc = DDGraphArc_NETARC(root2node);
    *DDGraphArc__THETA(root2node) = theta;
    *DDGraphArc__OMEGA(root2node) = omega;
    Network_updateDelay(network, netArc, delay, true);
  }
  if (DDGraphArc_THETA(node2root) != -theta ||
      DDGraphArc_OMEGA(node2root) != -omega) {
    NetArc netArc = DDGraphArc_NETARC(node2root);
    *DDGraphArc__THETA(node2root) = -theta;
    *DDGraphArc__OMEGA(node2root) = -omega;
    Network_updateDelay(network, netArc, -delay, true);
  }
}

#ifdef $XCC_h
/**
 * Undo the freeze of all this DDGraph nodes.
 */
void
DDGraph_unfreeze(DDGraph this);
#endif//$XCC_h

void
DDGraph_unfreeze(DDGraph this)
{
  int32_t firstIndex = DDGraph_FIRSTINDEX(this);
  int32_t lastIndex = DDGraph_LASTINDEX(this);
  DDGraph_FOREACH_DDGraphNode(this, node) {
    int32_t nodeIndex = DDGraphNode_INDEX(node);
    if (nodeIndex >= firstIndex && nodeIndex <= lastIndex) {
      DDGraphArc root2node = DDGraphNode_ROOT2NODE(node);
      DDGraphArc node2root = DDGraphNode_NODE2ROOT(node);
      *DDGraphArc__THETA(root2node) = 0;
      *DDGraphArc__THETA(node2root) = INT16_MIN;
    }
  } DDGraph_ENDEACH_DDGraphNode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the DDGraph module.
 */
void
DDGraph_INIT(void);
#endif//$XCC__h

void
DDGraph_INIT(void)
{
#ifndef DDGraph_PROCESSING
  if (GETENV("DDGraph_PROCESSING")) {
    sscanf(GETENV("DDGraph_PROCESSING"), "%d", &DDGraph_PROCESSING);
    fprintf(stderr, "*** DDGraph_PROCESSING=%d\n", DDGraph_PROCESSING);
  }
#endif//DDGraph_PROCESSING
}

#ifdef $XCC__h
/**
 * Finalize the DDGraph module.
 */
#define DDGraph_FINI()
#endif//$XCC__h

#if XCC__C
static void
DDGraph_TEST(void)
{
#include "XFA/DDGraph_.c"
}

int
main(int argc, char **argv)
{
  XFA_Test(DDGraph_TEST);
  return 0;
}
#endif

