#ifdef $XCC_h
/*
 * !!!!	Network.xcc
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
 * @ingroup CAL
 * @brief Network simplex algorithms.
 */
#endif//$XCC_h


#include "CAL/CAL_.h"

#ifdef $XCC__h
#include "CAL/Network.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the NetArc flags.
 */
typedef enum {
  NetArcFlag_Queued = 0x1,	// NetArc is in the QUEUED set of arcs.
  NetArcFlag_Active = 0x2,	// NetArc is in the ACTIVE set of arcs.
  NetArcFlag_Acyclic = 0x4,	// NetArc is in the acyclic subset of arcs.
  NetArcFlag_Longest = 0x8,	// NetArc is part of longest path.
  NetArcFlag_Useless = 0x10,	// NetArc is useless because found redundant.
  NetArcFlag_Precious = 0x20,	// NetArc to keep even if found redundant.
} NetArcFlag;
typedef unsigned NetArcFlags;
typedef uint8_t short_NetArcFlags;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ NetArcFlag_Table[1];
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ NetArcFlag_Entries[] = {
  ETableENTRY(NetArcFlag, Queued),
  ETableENTRY(NetArcFlag, Active),
  ETableENTRY(NetArcFlag, Acyclic),
  ETableENTRY(NetArcFlag, Longest),
  ETableENTRY(NetArcFlag, Useless),
  ETableENTRY(NetArcFlag, Precious),
  ETableENTRY_NULL
};
ETable_ NetArcFlag_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Network Arc.
 */
struct NetArc_ {
  //@args	int32_t delay, NetArcFlags flags
  //@access TAILNODE	(NetNode)DGraphArc_TAILNODE(this)
  //@access HEADNODE	(NetNode)DGraphArc_HEADNODE(this)
  int32_t DELAY;	// Delay along this arc.
  int16_t FLOW;			// Network flow through this NetArc.
  short_NetArcFlags FLAGS;
  //@access isQueued	((NetArc_FLAGS(this) & NetArcFlag_Queued))
  //@access isActive	((NetArc_FLAGS(this) & NetArcFlag_Active) != 0)
  //@access isAcyclic	((NetArc_FLAGS(this) & NetArcFlag_Acyclic) != 0)
  //@access isLongest	((NetArc_FLAGS(this) & NetArcFlag_Longest) != 0)
  //@access isUseless	((NetArc_FLAGS(this) & NetArcFlag_Useless) != 0)
  //@access isPrecious	((NetArc_FLAGS(this) & NetArcFlag_Precious) != 0)
  uint8_t TYPE;			// Type of this NetArc.
  //@access REDUCED	NetArc_reduced(this)
};
#endif//$XCC_h

NetArc
NetArc_Ctor(NetArc this, int32_t delay, NetArcFlags flags)
{
  *NetArc__DELAY(this) = delay;
  *NetArc__FLOW(this) = 0;
  *NetArc__FLAGS(this) = flags;
  *NetArc__TYPE(this) = 0;
  return this;
}

size_t
NetArc_Size(int32_t delay, NetArcFlags flags)
{
  return sizeof(NetArc_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(NetArc_)\t%zu\n", sizeof(NetArc_));
}
#endif//$XCC__c

#ifdef $XCC__h
static inline int32_t
NetArc_reduced(NetArc this)
{
  NetNode t_node = NetArc_TAILNODE(this), h_node = NetArc_HEADNODE(this);
  return NetArc_DELAY(this) + NetNode_DATE(t_node) - NetNode_DATE(h_node);
}
#endif//$XCC__h

#ifdef $XCC_h
/**
 * Pretty-print this NetArc.
 */
bool
NetArc_pretty(NetArc this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
NetArc_pretty(NetArc this, FILE *file)
{
  NetArcFlags flags = NetArc_FLAGS(this);
  int32_t TAILKEY = NetNode_INDEX(NetArc_TAILNODE(this));
  int32_t HEADKEY = NetNode_INDEX(NetArc_HEADNODE(this));
  if (file == NULL) file = stderr;
  fprintf(file, "(%d, %d)\tdelay=%d",
      TAILKEY, HEADKEY, NetArc_DELAY(this));
  if (flags != 0) {
    fprintf(file, "\t");
#ifndef XCC__C
    ETable_prettyFlags(NetArcFlag_Table, flags, "+", file);
#else
    // ETable_prettyFlags is not available if _NTRACE && XCC__C
    fprintf(file, "%u", flags);
#endif
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Attach this NetArc to a tail NetNode.
 */
void
NetArc_attachTailNode(NetArc this, DGraphNode cfgNode);
#define NetArc_attachTailNode(this, node) DGraphArc_attachTailNode((DGraphArc)(this), (DGraphNode)(node))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Attach this NetArc to a head NetNode.
 */
void
NetArc_attachHeadNode(NetArc this, DGraphNode cfgNode);
#define NetArc_attachHeadNode(this, node) DGraphArc_attachHeadNode((DGraphArc)(this), (DGraphNode)(node))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Detach this NetArc from its tail NetNode.
 */
void
NetArc_detachTailNode(NetArc this);
#define NetArc_detachTailNode(this) DGraphArc_detachTailNode((DGraphArc)(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Detach this NetArc from its head NetNode.
 */
void
NetArc_detachHeadNode(NetArc this);
#define NetArc_detachHeadNode(this) DGraphArc_detachHeadNode((DGraphArc)(this))
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Network Node.
 */
struct NetNode_ {
  //@args	int require
  //@access INDEX	DGraphNode_INDEX(this)
  //@access SCANNODE	(NetNode)DGraphNode_SCANNODE(this)
  //@mutate SCANNODE	(NetNode *)DGraphNode__SCANNODE(this)
  NetArc EDGE;			// Spanning tree edge from parent node.
  int32_t DATE;			// Date of this node.
  int16_t REQUIRE;		// Network flow requirement.
  int16_t BALANCE;		// Network flow balance.
  struct NetNode_ *NEXT;	// Next NetNode in spanning tree thread order.
  struct NetNode_ *PREV;	// Previous NetNode in spanning tree thread order.
  int32_t DEPTH;		// Depth in the spanning tree.
  uint32_t STAMP;		// Used for cut-set identification in the dual network simplex.
  int32_t ORDER;		// Order in the topological scan.
};
#endif//$XCC_h

NetNode
NetNode_Ctor(NetNode this, int require)
{
  *NetNode__EDGE(this) = NULL;
  *NetNode__DATE(this) = 0;
  *NetNode__REQUIRE(this) = require;
  Except_CHECK(NetNode_REQUIRE(this) == require);
  *NetNode__BALANCE(this) = 0;
  *NetNode__NEXT(this) = NULL;
  *NetNode__PREV(this) = NULL;
  *NetNode__DEPTH(this) = -1;
  *NetNode__STAMP(this) = 0;
  *NetNode__ORDER(this) = 0;
  return this;
}

size_t
NetNode_Size(int require) {
        return sizeof(NetNode_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(NetNode_)\t%zu\n", sizeof(NetNode_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Iterate over the entering NetArcs.
 */
#define NetNode_FORENTER_NetArc(this, cfgArc) \
  DGraphNode_FORENTER(this, NetArc_, cfgArc) {
#define NetNode_ENDENTER_NetArc \
  } DGraphNode_ENDENTER;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the leaving NetArcs.
 */
#define NetNode_FORLEAVE_NetArc(this, cfgArc) \
  DGraphNode_FORLEAVE(this, NetArc_, cfgArc) {
#define NetNode_ENDLEAVE_NetArc \
  } DGraphNode_ENDLEAVE;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pretty-print this NetNode.
 */
bool
NetNode_pretty(NetNode this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
NetNode_pretty(NetNode this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "NODE=%d\trequire=%d",
      NetNode_INDEX(this), NetNode_REQUIRE(this));
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Network over a DGraph.
 */
struct Network_;
#define Network_node(this, number) (NetNode)DGraph_node(Network_DGRAPH(this), number)
#define Network_access(this, index) (NetNode)DGraph_access(Network_DGRAPH(this), index)
#endif//$XCC_h

#ifdef $XCC_h
struct Network_ {
  //@args	Memory memory, int32_t maxNodes
  //@access ROOTNODE	(NetNode)DGraph_rootNode(Network_DGRAPH(this))
  //@access SCANNODE	(NetNode)DGraph_scanNode(Network_DGRAPH(this))
  //@mutate SCANNODE	(NetNode *)DGraph__scanNode(Network_DGRAPH(this))
  //@access NODES	(NetNode *)DGraph_nodes(Network_DGRAPH(this))
  //@access COUNT	DGraph_count(Network_DGRAPH(this))
  //@access NODECOUNT	DGraph_nodeCount(Network_DGRAPH(this))
  //@access ARCCOUNT	DGraph_arcCount(Network_DGRAPH(this))
  //@access MEMORY	DGraph_memory(Network_DGRAPH(this))
  DGraph_ DGRAPH_;	// Base DGraph_: must be first in struct Network_.
  //@access DGRAPH	Network__DGRAPH_(this)
  PtrSeq_ ACTIVE_;	// Storage for PtrSeq_ACTIVE.
  //@access ACTIVE	Network__ACTIVE_(this)
  IQueue_ QUEUED_;	// Storage for IQueue_QUEUED.
  //@access QUEUED	Network__QUEUED_(this)
  PtrSeq_ CIRCUIT_;	// Storage for PtrSeq_CIRCUIT.
  //@access CIRCUIT	Network__CIRCUIT_(this)
  NetArc Q;		// The entering arc of the network simplex.
  NetNode L;		// Such that EDGE(L) == Q after network simplex pivoting.
  NetNode K;		// The other end of the entering arc Q.
  NetArc P;		// The leaving arc (spanning tree edge) of the network simplex.
  NetNode J;		// Such that EDGE(J) = P before network simplex pivoting.
  uint32_t STAMP;	// Used for cut-set identification in the dual network simplex.
  bool (*CRITICAL)(Network this);
};
#endif//$XCC_h

Network
Network_Ctor(Network this, Memory memory, int32_t maxNodes)
{
  DGraph_Ctor(Network_DGRAPH(this), memory, maxNodes);
  PtrSeq_Ctor(Network_ACTIVE(this), memory, 0);
  IQueue_Ctor(Network_QUEUED(this), memory, sizeof(NetArc), 0);
  PtrSeq_Ctor(Network_CIRCUIT(this), memory, 0);
  *Network__Q(this) = NULL;
  *Network__L(this) = NULL;
  *Network__K(this) = NULL;
  *Network__P(this) = NULL;
  *Network__J(this) = NULL;
  *Network__STAMP(this) = 0;
  *Network__CRITICAL(this) = NULL;
  return this;
}

void
Network_Dtor(Network this)
{
  DGraph_Dtor(Network_DGRAPH(this));
  PtrSeq_Dtor(Network_ACTIVE(this));
  IQueue_Dtor(Network_QUEUED(this));
  PtrSeq_Dtor(Network_CIRCUIT(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Network_)\t%zu\n", sizeof(Network_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This Network Memory.
 */
static inline Memory
Network_memory(Network this)
{
  return Network_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Network DGraph.
 */
static inline DGraph
Network_dgraph(Network this)
{
  return Network_DGRAPH(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Network root node.
 */
static inline NetNode
Network_rootNode(Network this)
{
  return Network_ROOTNODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Network node count.
 */
static inline int32_t
Network_nodeCount(Network this)
{
  return Network_NODECOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Network NetNodes.
 */
#define Network_FOREACH_NetNode(this, node) \
  DGraph_FOREACH(Network_dgraph(this), NetNode_, node)
#define Network_ENDEACH_NetNode \
  DGraph_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Network NetNodes.
 */
#define Network_FORSCAN_NetNode(this, node) { \
  NetNode node = Network_rootNode(this); \
  Except_CHECK(NetNode_SCANNODE(node) != NULL); \
  for (; node != NULL; node = NetNode_SCANNODE(node)) {
#define Network_ENDSCAN_NetNode \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the spanning tree in forward thread order.
 */
#define Network_FORNEXT_NetNode(this, node) { \
  NetNode Network_ROOTNODE = Network_rootNode(this); \
  NetNode node = NetNode_NEXT(Network_ROOTNODE); \
  for (; node != Network_ROOTNODE; node = NetNode_NEXT(node)) {
#define Network_ENDNEXT_NetNode \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over the spanning tree in backward thread order.
 */
#define Network_FORPREV_NetNode(this, node) { \
  NetNode Network_ROOTNODE = Network_rootNode(this); \
  NetNode node = NetNode_PREV(Network_ROOTNODE); \
  for (; node != Network_ROOTNODE; node = NetNode_PREV(node)) {
#define Network_ENDPREV_NetNode \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a NetNode in this Network.
 */
NetNode
Network_makeNode(Network this, int require);
#endif//$XCC_h

NetNode
Network_makeNode(Network this, int require)
{
  size_t size = NetNode_Size(require);
  NetNode node = DGraph_makeNode(Network_DGRAPH(this), size);
  NetNode_Ctor(node, require);
  return node;
}

#ifdef $XCC_h
/**
 * Make a NetArc in this Network.
 */
NetArc
Network_makeArc(Network this, NetNode t_node, NetNode h_node, int32_t delay, NetArcFlags flags);
#endif//$XCC_h

NetArc
Network_makeArc(Network this, NetNode t_node, NetNode h_node, int32_t delay, NetArcFlags flags)
{
  size_t size = NetArc_Size(delay, flags);
  DGraphArc arc = DGraph_makeArc(Network_DGRAPH(this), t_node, h_node, size);
  return NetArc_Ctor((NetArc)arc, delay, flags);
  return arc;
}

#ifdef $XCC_h
/**
 * Fill this Network ACTIVE arcs.
 */
void
Network_activate(Network this);
#endif//$XCC_h

void
Network_activate(Network this)
{
  PtrSeq active = Network_ACTIVE(this);
  IQueue queued = Network_QUEUED(this);
  PtrSeq circuit = Network_CIRCUIT(this);
  int32_t arcCount = Network_ARCCOUNT(this), activeCount = 0;
  if (PtrSeq_maxCount(active) < arcCount) {
    PtrSeq_empty(active);
    IQueue_empty(queued, NULL);
    PtrSeq_empty(circuit);
    PtrSeq_resize(active, arcCount);
    Network_FOREACH_NetNode(this, NODE) {
      NetNode_FORLEAVE_NetArc(NODE, ARC) {
        if (!NetArc_isUseless(ARC)) {
          PtrSeq_push(active, ARC);
          *NetArc__FLAGS(ARC) |= NetArcFlag_Active;
          activeCount++;
        } else {
          *NetArc__FLAGS(ARC) &= ~NetArcFlag_Active;
        }
      } NetNode_ENDLEAVE_NetArc;
    } Network_ENDEACH_NetNode;
    IQueue_resize(queued, activeCount);
    PtrSeq_resize(circuit, _MAX(activeCount/8, 32));
    CAL_TRACE && fprintf(CAL_TRACE, "*** READY ARCS\tcount=%d\tactive=%d\n",
                                    arcCount, activeCount);
  }
}

#ifdef $XCC_h
/**
 * This Network ACTIVE PtrSeq.
 */
static inline PtrSeq
Network_active(Network this)
{
  return Network_ACTIVE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Network ACTIVE arcs.
 */
#define Network_ACTIVE_FOREACH_NetArc(this, arc) { \
  PtrSeq Network_ACTIVE = Network_active(this); \
  PtrSeq_FOREACH(Network_ACTIVE, NetArc, arc) {
#define Network_ACTIVE_ENDEACH_NetArc \
  } PtrSeq_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC__h
/**
 * True if this Network_QUEUED is empty.
 */
static inline bool
Network_QUEUED_isEmpty(Network this)
{
  return IQueue_isEmpty(Network_QUEUED(this));
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Put a NetArc on this Network_QUEUED.
 */
static inline void
Network_QUEUED_putArc(Network this, NetArc netarc)
{
  if (!NetArc_isQueued(netarc)) {
    *NetArc__FLAGS(netarc) |= NetArcFlag_Queued;
    IQueue_PUT(Network_QUEUED(this), NetArc, netarc);
  }
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Put the incident arcs of a NetNode on this Network_QUEUED.
 */
static inline void
Network_QUEUED_putNode(Network this, NetNode netnode)
{
  NetNode_FORENTER_NetArc(netnode, netarc) {
    Network_QUEUED_putArc(this, netarc);
  } NetNode_ENDENTER_NetArc;
  NetNode_FORLEAVE_NetArc(netnode, netarc) {
    Network_QUEUED_putArc(this, netarc);
  } NetNode_ENDLEAVE_NetArc;
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * The top NetArc of this Network_QUEUED.
 */
static inline NetArc
Network_QUEUED_topArc(Network this)
{
  return IQueue_TOP(Network_QUEUED(this), NetArc);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * The bottom NetArc of this Network_QUEUED.
 */
static inline NetArc
Network_QUEUED_bottomArc(Network this)
{
  return IQueue_BOT(Network_QUEUED(this), NetArc);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Pop the top NetArc from this Network_QUEUED.
 */
static inline NetArc
Network_QUEUED_popArc(Network this)
{
  NetArc netarc = NULL;
  IQueue_POP(Network_QUEUED(this), NetArc, netarc);
  Except_CHECK(NetArc_isQueued(netarc));
  *NetArc__FLAGS(netarc) &= ~NetArcFlag_Queued;
  return netarc;
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Drop the bottom NetArc from this Network_QUEUED.
 */
static inline NetArc
Network_QUEUED_dropArc(Network this)
{
  NetArc netarc = NULL;
  IQueue_DROP(Network_QUEUED(this), NetArc, netarc);
  Except_CHECK(NetArc_isQueued(netarc));
  *NetArc__FLAGS(netarc) &= ~NetArcFlag_Queued;
  return netarc;
}
#endif//$XCC__h

#ifdef $XCC_h
/**
 * This Network QUEUED IQueue.
 */
static inline IQueue
Network_queued(Network this)
{
  return Network_QUEUED(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Network QUEUED arcs.
 */
#define Network_QUEUED_FOREACH_NetArc(this, arc) { \
  IQueue Network_QUEUED = Network_queued(this); \
  IQueue_FOREACH(Network_QUEUED, NetArc, Network__ARC) { \
    NetArc arc = *Network__ARC;
#define Network_QUEUED_ENDEACH_NetArc \
  } IQueue_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Network CIRCUIT PtrSeq.
 */
static inline PtrSeq
Network_circuit(Network this)
{
  return Network_CIRCUIT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pretty-print this Network nodes and arcs.
 */
bool
Network_pretty(Network this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
Network_pretty(Network this, FILE *file)
{
  if (file == NULL) file = stderr;
  Network_FOREACH_NetNode(this, NODE) {
    fprintf(file, "    ");
    NetNode_pretty(NODE, file);
    fprintf(file, "\n");
    NetNode_FORLEAVE_NetArc(NODE, ARC) {
      fprintf(file, "\t");
      NetArc_pretty(ARC, file);
      fprintf(file, "\n");
    } NetNode_ENDLEAVE_NetArc;
  } Network_ENDEACH_NetNode;
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Pretty-print this Network spanning tree.
 */
bool
Network_prettyTree(Network this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
Network_prettyTree(Network this, FILE *file)
{
  if (file == NULL) file = stderr;
  Network_FORNEXT_NetNode(this, NODE) {
    NetArc edge = NetNode_EDGE(NODE);
    fprintf(file, "    NODE=%d\trequire=%d\n", NetNode_INDEX(NODE), NetNode_REQUIRE(NODE));
    fprintf(file, "\tdepth=%d\tdate=%d\n", NetNode_DEPTH(NODE), NetNode_DATE(NODE));
    if (edge != NULL) {
      NetNode t_node = NetArc_TAILNODE(edge);
      NetNode h_node = NetArc_HEADNODE(edge);
      fprintf(file, "\tEDGE\tTAILNODE=%d\tHEADNODE=%d\n", NetNode_INDEX(t_node), NetNode_INDEX(h_node));
      fprintf(file, "\t    delay=%d\tflow=%d\n", NetArc_DELAY(edge), NetArc_FLOW(edge));
    }
  } Network_ENDNEXT_NetNode;
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Check this Network spanning tree.
 */
bool
Network_checkTree(Network this, bool flows);
#endif//$XCC_h

bool
Network_checkTree(Network this, bool flows)
{
  int32_t nodeCount = 1;
  NetNode j = Network_ROOTNODE(this);
  int require = NetNode_REQUIRE(j);
  ASSERT(NetNode_EDGE(j) == NULL);
  ASSERT(NetNode_DATE(j) == 0);
  Network_FORNEXT_NetNode(this, NODE) {
    NetNode j = NODE;
    NetArc p = NetNode_EDGE(j);
    if (NetArc_TAILNODE(p) != j) {
      NetNode i = NetArc_TAILNODE(p);
      if (NetNode_DATE(j) != NetNode_DATE(i) + NetArc_DELAY(p)) {
        fprintf(stderr, "i=%d j=%d\n", NetNode_INDEX(i), NetNode_INDEX(j));
#ifndef _NTRACE
        Network_prettyTree(this, stderr);
#endif//_NTRACE
      }
      ASSERT(NetNode_DATE(j) == NetNode_DATE(i) + NetArc_DELAY(p));
    } else {
      NetNode i = NetArc_HEADNODE(p);
      if (NetNode_DATE(j) != NetNode_DATE(i) - NetArc_DELAY(p)) {
        fprintf(stderr, "i=%d j=%d\n", NetNode_INDEX(i), NetNode_INDEX(j));
#ifndef _NTRACE
        Network_prettyTree(this, stderr);
#endif//_NTRACE
      }
      ASSERT(NetNode_DATE(j) == NetNode_DATE(i) - NetArc_DELAY(p));
    }
    *NetNode__BALANCE(j) = NetNode_REQUIRE(j);
    require += NetNode_REQUIRE(j);
    ASSERT(NetNode_PREV(NetNode_NEXT(j)) == j);
    nodeCount++;
  } Network_ENDNEXT_NetNode;
  ASSERT(Network_NODECOUNT(this) == nodeCount);
  if (flows) {
    ASSERT(require == 0);
    Network_FORPREV_NetNode(this, NODE) {
      NetNode j = NODE;
      NetArc p = NetNode_EDGE(j);
      if (NetArc_TAILNODE(p) != j) {
        NetNode i = NetArc_TAILNODE(p);
        ASSERT(NetArc_FLOW(p) == -NetNode_BALANCE(j));
        *NetNode__BALANCE(i) = NetNode_BALANCE(i) + NetNode_BALANCE(j);
      } else {
        NetNode i = NetArc_HEADNODE(p);
        ASSERT(NetArc_FLOW(p) == NetNode_BALANCE(j));
        *NetNode__BALANCE(i) = NetNode_BALANCE(i) + NetNode_BALANCE(j);
      }
    } Network_ENDPREV_NetNode;
  }
  return true;
}

#ifdef $XCC_h
/**
 * Check this Network is primal solved.
 *
 * A network is primal solved when all arc flows are non-negative.
 */
bool
Network_checkPrimal(Network this);
#endif//$XCC_h

bool
Network_checkPrimal(Network this)
{
  Network_ACTIVE_FOREACH_NetArc(this, ARC) {
    if (NetArc_FLOW(ARC) < 0) {
      int32_t TAILKEY = NetNode_INDEX(NetArc_TAILNODE(ARC));
      int32_t HEADKEY = NetNode_INDEX(NetArc_HEADNODE(ARC));
      fprintf(stderr, "NEGATIVE FLOW:\t(%d, %d)\n", TAILKEY, HEADKEY);
#ifndef _NTRACE
      Network_pretty(this, stdout);
      Network_prettyTree(this, stdout);
#endif//_NTRACE
    }
    ASSERT(NetArc_FLOW(ARC) >= 0);
  } Network_ACTIVE_ENDEACH_NetArc;
  return true;
}

#ifdef $XCC_h
/**
 * Check this Network is dual solved.
 *
 * A network is dual solved when it has valid node potentials. In our case this
 * means that no dependence is violated, that is, the tail and head nodes have
 * DATEs whose difference is no smaller than the dependence DELAY.
 */
bool
Network_checkDual(Network this);
#endif//$XCC_h

bool
Network_checkDual(Network this)
{
  Network_ACTIVE_FOREACH_NetArc(this, ARC) {
    int32_t reduced = NetArc_REDUCED(ARC);
    if (reduced > 0) {
      int32_t TAILKEY = NetNode_INDEX(NetArc_TAILNODE(ARC));
      int32_t HEADKEY = NetNode_INDEX(NetArc_HEADNODE(ARC));
      fprintf(stderr, "VIOLATED ARC:\t(%d, %d)\n", TAILKEY, HEADKEY);
#ifndef _NTRACE
      Network_pretty(this, stdout);
      Network_prettyTree(this, stdout);
#endif//_NTRACE
    }
    ASSERT(reduced <= 0);
  } Network_ACTIVE_ENDEACH_NetArc;
  return true;
}

#ifdef $XCC_h
/**
 * Check this Network is primal and dual solved.
 */
bool
Network_checkSolved(Network this);
#endif//$XCC_h

bool
Network_checkSolved(Network this)
{
  return Network_checkPrimal(this) && Network_checkDual(this);
}

#ifdef $XCC__h
/**
 * Grow Spanning Tree.
 *
 * Function used to add node @c l to the spanning tree from node @c k, taking arc
 * @c q as the new tree edge. The tree indices (NEXT, PREV, DEPTH) and the node
 * potentials (DATE) are updated. (No longer used.)
 */
static inline void
Network_doGST(Network this, NetArc q, NetNode k, NetNode l)
{
  *NetNode__EDGE(l) = q;
  *NetNode__DEPTH(l) = NetNode_DEPTH(k) + 1;
  *NetNode__DATE(l) = NetNode_DATE(k) + NetArc_DELAY(q);
  {
    NetNode n = NetNode_NEXT(k);
    *NetNode__NEXT(l) = n, *NetNode__PREV(n) = l;
    *NetNode__NEXT(k) = l, *NetNode__PREV(l) = k;
  }
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Reorder Spanning Tree for a network simplex pivot.
 *
 * Function used to enter arc @c q as the tree edge between node @c k and node @p
 * l in the spanning tree. The leaving arc is node @c j spanning tree edge.
 */
void
Network_doRST(Network this, bool queue);
#endif//$XCC__h

void
Network_doRST(Network this, bool queue)
{
  NetArc q = Network_Q(this);
  NetNode k = Network_K(this);
  NetNode l = Network_L(this);
  NetNode j = Network_J(this);
  NetArc e = NetNode_EDGE(l);
  NetNode z = l, a = l, b = l;
  NetNode x = NetNode_NEXT(a), r = NetNode_PREV(a);
  int32_t depth = NetNode_DEPTH(k) - NetNode_DEPTH(l) + 1;
  int32_t date = NetNode_DATE(k) - NetNode_DATE(l);
  if (NetArc_TAILNODE(q) == k) {
    date += NetArc_DELAY(q);
  } else {
    date -= NetArc_DELAY(q);
  }
  b = NetArc_TAILNODE(e) != b? NetArc_TAILNODE(e): NetArc_HEADNODE(e);
  for (;;) {
    while (NetNode_DEPTH(x) > NetNode_DEPTH(a)) {
      *NetNode__DEPTH(x) = NetNode_DEPTH(x) + depth;
      *NetNode__DATE(x) = NetNode_DATE(x) + date;
      if (queue) Network_QUEUED_putNode(this, x);
      z = x, x = NetNode_NEXT(x);
    }
    *NetNode__DEPTH(a) = NetNode_DEPTH(a) + depth;
    *NetNode__DATE(a) = NetNode_DATE(a) + date;
    if (queue) Network_QUEUED_putNode(this, a);
    if (a == j) break;
    *NetNode__NEXT(r) = x, *NetNode__PREV(x) = r;
    x = NetNode_NEXT(b), r = NetNode_PREV(b);
    *NetNode__NEXT(z) = b, *NetNode__PREV(b) = z;
    z = a = b;
    depth += 2;
    {
      NetArc t = NetNode_EDGE(a);
      *NetNode__EDGE(a) = e, e = t;
    }
    b = NetArc_TAILNODE(e) != b? NetArc_TAILNODE(e): NetArc_HEADNODE(e);
  }
  if (NetNode_NEXT(k) != j) {
    NetNode n = NetNode_NEXT(k);
    *NetNode__NEXT(r) = x, *NetNode__PREV(x) = r;
    *NetNode__NEXT(z) = n, *NetNode__PREV(n) = z;
  }
  *NetNode__NEXT(k) = l, *NetNode__PREV(l) = k;
  *NetNode__EDGE(l) = q;
}

/**
 * Topological Sort Order recursive Search.
 */
static void
Network_doTSOSearch(Network this, NetNode node)
{
  *NetNode__ORDER(node) = -1;	// Use ORDER < 0 as visited flag.
  NetNode_FORLEAVE_NetArc(node, ARC) {
    if (NetArc_isAcyclic(ARC)) {
      NetNode h_node = NetArc_HEADNODE(ARC);
      if (NetNode_ORDER(h_node) >= 0) {
        Network_doTSOSearch(this, h_node);
      }
    }
  } NetNode_ENDLEAVE_NetArc;
  *NetNode__SCANNODE(node) = Network_SCANNODE(this);
  *Network__SCANNODE(this) = node;
}

#ifdef $XCC_h
/**
 * Topological Sort Ordering of the Acyclic NetArc(s).
 */
void
Network_doTSO(Network this);
#endif//$XCC_h

void
Network_doTSO(Network this)
{
  int32_t order = 0;
  NetNode root = Network_ROOTNODE(this);
  // Recursive DFS to set the SCANNODE indices.
  *Network__SCANNODE(this) = NULL;
  Network_doTSOSearch(this, root);
  // Set the ORDER fields.
  Network_FORSCAN_NetNode(this, NODE) {
    *NetNode__ORDER(NODE) = order++;
  } Network_ENDSCAN_NetNode;
  Except_CHECK(order == Network_COUNT(this));
}

#ifdef $XCC_h
/**
 * Forward Longest Path.
 *
 * Build the longest-path spanning tree over the forward dependence graph in two
 * steps. First step is to grow the spanning tree in increasing topological order
 * of the forward dependence graph. This computes the longest path on the forward
 * dependence graph in linear-time. Second step is to set the spanning tree
 * indices (DEPTH, PREV, NEXT). The topological ordering must have been computed
 * before in the SCANNODE by calling Network_doTSO.
 */
void
Network_doFLP(Network this);
#endif//$XCC_h

void
Network_doFLP(Network this)
{
  NetNode root = Network_ROOTNODE(this);
  // Initialize the non-root nodes.
  Network_FOREACH_NetNode(this, NODE) {
    *NetNode__NEXT(NODE) = *NetNode__PREV(NODE) = NULL;
    *NetNode__EDGE(NODE) = NULL, *NetNode__DEPTH(NODE) = -1;
    *NetNode__DATE(NODE) = -1;
  } Network_ENDEACH_NetNode;
  // Initialize the root node.
  *NetNode__NEXT(root) = root, *NetNode__PREV(root) = root;
  *NetNode__EDGE(root) = NULL, *NetNode__DEPTH(root) = 0;
  *NetNode__DATE(root) = 0;
  // First step: grow the spanning tree.
  Network_FORSCAN_NetNode(this, NODE) {
    NetNode_FORLEAVE_NetArc(NODE, ARC) {
      if (NetArc_isAcyclic(ARC)) {
        NetNode t_node = NetArc_TAILNODE(ARC);
        NetNode h_node = NetArc_HEADNODE(ARC);
        if (NetNode_DATE(h_node) < NetNode_DATE(t_node) + NetArc_DELAY(ARC)) {
          *NetNode__DATE(h_node) = NetNode_DATE(t_node) + NetArc_DELAY(ARC);
          *NetNode__EDGE(h_node) = ARC;
        }
      } else {
        Network_QUEUED_putArc(this, ARC);
      }
    } NetNode_ENDLEAVE_NetArc;
  } Network_ENDSCAN_NetNode;
  // Second step: set the spanning tree indices.
  Network_FORSCAN_NetNode(this, NODE) {
    NetArc EDGE = NetNode_EDGE(NODE);
    if (EDGE != NULL) {
      NetNode t_node = NetArc_TAILNODE(EDGE);
      NetNode h_node = NetArc_HEADNODE(EDGE);
      NetNode next = NetNode_NEXT(t_node);
      *NetNode__DEPTH(h_node) = NetNode_DEPTH(t_node) + 1;
      *NetNode__NEXT(h_node) = next, *NetNode__PREV(next) = h_node;
      *NetNode__NEXT(t_node) = h_node, *NetNode__PREV(h_node) = t_node;
    } else Except_CHECK(NODE == root);
  } Network_ENDSCAN_NetNode;
  // Check consistency of the indices.
  Except_DEBUG(Network_checkTree(this, false));
}

#ifdef $XCC__h
/**
 * Critical Directed Cycles.
 *
 * Find a critical directed cycle. The first step is
 * to identify the directed cycle by going up the spanning tree from @c k. If
 * there is a directed cycle, it is infeasible in case its delay > 0. The
 * CRITICAL call-back is then called to modify the arc delays.
 */
bool
Network_doCDC(Network this, NetArc q, NetNode k, NetNode l);
#endif//$XCC__h

bool
Network_doCDC(Network this, NetArc q, NetNode k, NetNode l)
{
  NetNode a = NULL;
  int32_t arcCount = 1;
  int32_t delay = NetArc_DELAY(q);
  Except_REQUIRE(NetArc_TAILNODE(q) == k);
  Except_REQUIRE(NetArc_HEADNODE(q) == l);
  Except_DEBUG(Network_checkTree(this, false));
  // Check if there is a critical cycle.
  for (a = k; NetNode_DEPTH(a) > NetNode_DEPTH(l);) {
    NetArc e = NetNode_EDGE(a);
    delay += NetArc_DELAY(e);
    a = NetArc_TAILNODE(e);
    arcCount++;
  }
  if (a == l && delay > 0) {
    // Stack the arcs of the critical circuit.
    PtrSeq circuit = Network_CIRCUIT(this);
    PtrSeq_empty(circuit);
    PtrSeq_push2(circuit, q);
    for (a = k; NetNode_DEPTH(a) > NetNode_DEPTH(l);) {
      NetArc e = NetNode_EDGE(a);
      PtrSeq_push2(circuit, e);
      a = NetArc_TAILNODE(e);
    }
    (*Network_CRITICAL(this))(this);
    return true;
  }
  return false;
}

#ifdef $XCC__h
/**
 * Fill Arc Queue.
 *
 * Fill the Network_QUEUED with the arcs that violate Bellman's inequalities.
 */
bool
Network_doFAQ(Network this, bool refill);
#endif//$XCC__h

bool
Network_doFAQ(Network this, bool refill)
{
  if (refill || Network_QUEUED_isEmpty(this)) {
    Network_ACTIVE_FOREACH_NetArc(this, ARC) {
      if (NetArc_REDUCED(ARC) > 0) {
        Network_QUEUED_putArc(this, ARC);
      }
    } Network_ACTIVE_ENDEACH_NetArc;
  }
  *Network__Q(this) = NULL;
  while (!Network_QUEUED_isEmpty(this)) {
    NetArc arc = Network_QUEUED_popArc(this);
    if (NetArc_REDUCED(arc) > 0) {
      *Network__Q(this) = arc;
      break;
    }
  }
  return Network_Q(this) != NULL;
}

#ifdef $XCC_h
/**
 * Simplex Label Correcting algorithm.
 *
 * See "Efficient Shortest Path Simplex Algorithms" by D. Goldfarb, J. Hao, S.-R.
 * Kai, in Operations Research, Vol. 38, No. 4, July-August 1990.
 */
void
Network_doSLC(Network this, bool refill);
#endif//$XCC_h

void
Network_doSLC(Network this, bool refill)
{
  while (Network_doFAQ(this, refill)) {
    NetArc q = Network_Q(this);
    NetNode t_node = NetArc_TAILNODE(q);
    NetNode h_node = NetArc_HEADNODE(q);
    NetArc p = NetNode_EDGE(h_node);
    Except_CHECK(p != NULL || h_node == Network_ROOTNODE(this));
    if (Network_doCDC(this, q, t_node, h_node)) {
      refill = true;
    } else {
      *Network__Q(this) = q;
      *Network__K(this) = t_node;
      *Network__L(this) = h_node;
      *Network__J(this) = h_node;
      *Network__P(this) = p;
      Network_doRST(this, true);
      refill = false;
    }
  }
  Except_DEBUG(Network_checkTree(this, false));
  Except_DEBUG(Network_checkDual(this));
}

#ifdef $XCC__c
// Test of Network_doSLC.
{
  int16_t lambda = 11;	// HACK ALERT!
  Network_ network[1];
  Network_Ctor(network, Memory_Root, 100);
  {
    NetNode node_0 = Network_makeNode(network, 10);
    NetNode node_1 = Network_makeNode(network, -1);
    NetNode node_2 = Network_makeNode(network, -1);
    NetNode node_3 = Network_makeNode(network, -1);
    NetNode node_4 = Network_makeNode(network, -1);
    NetNode node_5 = Network_makeNode(network, -1);
    NetNode node_6 = Network_makeNode(network, -1);
    NetNode node_7 = Network_makeNode(network, -1);
    NetNode node_8 = Network_makeNode(network, -1);
    NetNode node_9 = Network_makeNode(network, -1);
    NetNode node_10 = Network_makeNode(network, -1);
    Network_makeArc(network, node_0, node_1, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_1, node_9, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_9, node_10, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_1, node_2, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_2, node_9, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_1, node_3, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_3, node_9, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_1, node_4, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_4, node_9, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_1, node_5, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_5, node_9, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_1, node_6, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_6, node_9, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_1, node_7, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_7, node_9, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_1, node_8, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_8, node_9, 0 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_2, node_7, 9 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_2, node_8, 9 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_2, node_9, 1 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_3, node_9, 1 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_4, node_9, 1 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_5, node_9, 1 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_6, node_9, 1 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_7, node_8, 1 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_8, node_9, 1 - lambda*0, NetArcFlag_Acyclic);
    Network_makeArc(network, node_10, node_1, 0 - lambda*1, 0);
  }
  printf("\nNetwork test 1:\n");
  Network_pretty(network, stdout);
  Network_doTSO(network);
  Network_activate(network);
  Network_doFLP(network);
  Network_doSLC(network, false);
  printf("After doSLC:\n");
  Network_pretty(network, stdout);
  Network_Dtor(network);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Compute Node Potentials from scratch, given a spanning tree.
 *
 * The spanning tree is traversed top-down using the NEXT indices.
 */
void
Network_doCNP(Network this);
#endif//$XCC_h

void
Network_doCNP(Network this)
{
  NetNode root = Network_ROOTNODE(this);
  Except_REQUIRE(NetNode_EDGE(root) == NULL);
  Except_REQUIRE(NetNode_DATE(root) == 0);
  if (NetNode_NEXT(root) != NULL) {
    Network_FORNEXT_NetNode(this, j) {
      NetArc p = NetNode_EDGE(j);
      if (NetArc_TAILNODE(p) != j) {
        NetNode i = NetArc_TAILNODE(p);
        *NetNode__DATE(j) = NetNode_DATE(i) + NetArc_DELAY(p);
        // if (ENTERARCS) PUSHARCS(this, j);
      } else {
        NetNode i = NetArc_HEADNODE(p);
        *NetNode__DATE(j) = NetNode_DATE(i) - NetArc_DELAY(p);
        // if (ENTERARCS) PUSHARCS(this, j);
      }
    } Network_ENDNEXT_NetNode;
  }
}

#ifdef $XCC_h
/**
 * Compute Edge Flows from scratch, given a spanning tree.
 *
 * The tree is traversed bottom-up using the PREV indices.
 */
void
Network_doCEF(Network this);
#endif//$XCC_h

void
Network_doCEF(Network this)
{
  NetNode r = Network_ROOTNODE(this);
  *NetNode__BALANCE(r) = NetNode_REQUIRE(r);
  Network_FORNEXT_NetNode(this, j) {
    *NetNode__BALANCE(j) = NetNode_REQUIRE(j);
  } Network_ENDNEXT_NetNode;
  Network_FORPREV_NetNode(this, j) {
    NetArc p = NetNode_EDGE(j);
    if (NetArc_TAILNODE(p) != j) {
      NetNode i = NetArc_TAILNODE(p);
      *NetArc__FLOW(p) = -NetNode_BALANCE(j);
      *NetNode__BALANCE(i) = NetNode_BALANCE(i) + NetNode_BALANCE(j);
    } else {
      NetNode i = NetArc_HEADNODE(p);
      *NetArc__FLOW(p) = NetNode_BALANCE(j);
      *NetNode__BALANCE(i) = NetNode_BALANCE(i) + NetNode_BALANCE(j);
    }
    // if (LEAVEARCS) PUSHARC(this, p);
  } Network_ENDPREV_NetNode;
}

#ifdef $XCC__h
/**
 * Update Edge Flows for a network simplex pivot.
 *
 * Given the entering arc @c q and the leaving edge @c p, the flows are updated.
 * This function assumes that the spanning tree is not yet reordered by @c
 * Network_doRST.
 */
void
Network_doUEF(Network this, bool queue);
#endif//$XCC__h

void
Network_doUEF(Network this, bool queue)
{
  NetArc q = Network_Q(this);
  NetArc p = Network_P(this);
  int delta = _ABS(NetArc_FLOW(p));
  NetNode a = NetArc_TAILNODE(q), b = NetArc_HEADNODE(q);
  while (a != b) {
    if (NetNode_DEPTH(a) >= NetNode_DEPTH(b)) {
      NetArc e = NetNode_EDGE(a);
      if (NetArc_TAILNODE(e) == a) {
        *NetArc__FLOW(e) = NetArc_FLOW(e) - delta;
        a = NetArc_HEADNODE(e);
      } else {
        *NetArc__FLOW(e) = NetArc_FLOW(e) + delta;
        a = NetArc_TAILNODE(e);
      }
      if (queue) Network_QUEUED_putArc(this, e);
    } else {
      NetArc f = NetNode_EDGE(b);
      if (NetArc_HEADNODE(f) == b) {
        *NetArc__FLOW(f) = NetArc_FLOW(f) - delta;
        b = NetArc_TAILNODE(f);
      } else {
        *NetArc__FLOW(f) = NetArc_FLOW(f) + delta;
        b = NetArc_HEADNODE(f);
      }
      if (queue) Network_QUEUED_putArc(this, f);
    }
  }
  *NetArc__FLOW(q) = delta;
}

#ifdef $XCC__h
/**
 * Find Leaving Edge of the dual network simplex algorithm.
 *
 * Ideally, this is an edge with the minimum negative flow. This is what @c
 * Network_dualFLE actually does, but this implementation is slow. TODO:
 * implement a cached scheme.
 */
bool
Network_dualFLE(Network this);
#endif//$XCC__h

bool
Network_dualFLE(Network this)
{
  int flow = 0;
  NetArc p = NULL;
  Network_ACTIVE_FOREACH_NetArc(this, ARC) {
    if (NetArc_FLOW(ARC) < flow) {
      flow = NetArc_FLOW(ARC);
      p = ARC;
    }
  } Network_ACTIVE_ENDEACH_NetArc;
  *Network__P(this) = p;
  return p != NULL;
}

#ifdef $XCC__h
/**
 * Find Entering Arc of the dual network simplex algorithm.
 *
 * We first identify the cut-set of arcs with the opposite direction. In this
 * set, the entering arc has the maximum (non-positive) reduced delay. To our
 * knowledge, the strongly feasible basis property cannot be maintained.
 */
void
Network_dualFEA(Network this);
#endif//$XCC__h

void
Network_dualFEA(Network this)
{
  NetArc p = Network_P(this), q = NULL;
  int32_t redmax = INT32_MIN;
  int32_t arcCount = Network_ARCCOUNT(this)*2;
  PtrSeq_ cutSet[1];
  PtrSeq_Ctor(cutSet, Memory_Root, arcCount);
  *Network__STAMP(this) = Network_STAMP(this) + 1;
  if (NetNode_EDGE(NetArc_HEADNODE(p)) == p) {
    int32_t stamp = Network_STAMP(this);
    NetNode j = NetArc_HEADNODE(p), x = j;
    do {
      *NetNode__STAMP(x) = stamp;
      NetNode_FORLEAVE_NetArc(x, ARC) {
        PtrSeq_push(cutSet, ARC);
      } NetNode_ENDLEAVE_NetArc;
      x = NetNode_NEXT(x);
    } while (NetNode_DEPTH(x) > NetNode_DEPTH(j));
    *Network__J(this) = j;
  } else if (NetNode_EDGE(NetArc_TAILNODE(p)) == p) {
    int32_t stamp = Network_STAMP(this);
    NetNode j = NetArc_TAILNODE(p), x = j;
    do {
      *NetNode__STAMP(x) = stamp;
      NetNode_FORENTER_NetArc(x, ARC) {
        PtrSeq_push(cutSet, ARC);
      } NetNode_ENDENTER_NetArc;
      x = NetNode_NEXT(x);
    } while (NetNode_DEPTH(x) > NetNode_DEPTH(j));
    *Network__J(this) = j;
  } else Except_NEVER(true);
  PtrSeq_FOREACH(cutSet, NetArc, a) {
    NetNode o = NetArc_TAILNODE(a), d = NetArc_HEADNODE(a);
    if (NetNode_STAMP(o) != NetNode_STAMP(d)) {
      int32_t reduced = NetArc_REDUCED(a);
      Except_CHECK(reduced <= 0);
      if (reduced > redmax) {
        redmax = reduced;
        q = a;
      }
    }
  } PtrSeq_ENDEACH;
  Except_CHECK(q != NULL);
  if (NetNode_EDGE(NetArc_HEADNODE(p)) == p) {
    *Network__Q(this) = q;
    *Network__K(this) = NetArc_HEADNODE(Network_Q(this));
    *Network__L(this) = NetArc_TAILNODE(Network_Q(this));
  } else {
    *Network__Q(this) = q;
    *Network__K(this) = NetArc_TAILNODE(Network_Q(this));
    *Network__L(this) = NetArc_HEADNODE(Network_Q(this));
  }
  PtrSeq_Dtor(cutSet);
}

#ifdef $XCC_h
/**
 * Network simplex dual solver.
 */
void
Network_dualSolve(Network this);
#endif//$XCC_h

void
Network_dualSolve(Network this)
{
  Except_DEBUG(Network_checkDual(this));
  Except_DEBUG(Network_checkTree(this, true));
  while (Network_dualFLE(this)) {
    Network_dualFEA(this);
    Except_CHECK(NetNode_EDGE(Network_J(this)) == Network_P(this));
    Network_doUEF(this, false);
    Network_doRST(this, false);
    Except_DEBUG(Network_checkTree(this, true));
  }
  Except_DEBUG(Network_checkSolved(this));
}

#ifdef $XCC__h
/**
 * Find Leaving Edge of the primal network simplex algorithm.
 *
 * This algorithm maintains strongly feasible bases, as defined by Cunningham in
 * "A Network Simplex Method", Mathematical Programming 11 (1976): tree edges
 * with zero flow are directed away from the root. This is achieved by starting
 * from the apex, and selecting the first blocking arc in the cycle following the
 * direction of the entering arc. Please note that Ahuja, Magnanti Orlin in
 * "Network Flows", Prentice-Hall 1993, use the opposite definition of strongly
 * feasible bases, so they select the last blocking arc instead.
 */
void
Network_primalFLE(Network this);
#endif//$XCC__h

void
Network_primalFLE(Network this)
{
  int32_t delta = INT32_MAX;
  NetArc u = NULL, v = NULL;
  NetNode a = NetArc_TAILNODE(Network_Q(this));
  NetNode b = NetArc_HEADNODE(Network_Q(this));
  while (a != b) {
    if (NetNode_DEPTH(a) >= NetNode_DEPTH(b)) {
      NetArc e = NetNode_EDGE(a);
      if (NetArc_TAILNODE(e) == a) {
        if (NetArc_FLOW(e) <= delta) {
          delta = NetArc_FLOW(e);
          u = e;
        }
      }
      a = NetArc_TAILNODE(e) != a? NetArc_TAILNODE(e): NetArc_HEADNODE(e);
    } else {
      NetArc f = NetNode_EDGE(b);
      if (NetArc_HEADNODE(f) == b) {
        if (NetArc_FLOW(f) < delta) {
          delta = NetArc_FLOW(f);
          v = f;
        }
      }
      b = NetArc_HEADNODE(f) != b? NetArc_HEADNODE(f): NetArc_TAILNODE(f);
    }
  }
  ASSERT(u != NULL || v != NULL);
  if (v == NULL) {
    *Network__K(this) = NetArc_HEADNODE(Network_Q(this));
    *Network__L(this) = NetArc_TAILNODE(Network_Q(this));
    *Network__J(this) = NetArc_TAILNODE(u);
    *Network__P(this) = u;
  } else if (u == NULL) {
    *Network__K(this) = NetArc_TAILNODE(Network_Q(this));
    *Network__L(this) = NetArc_HEADNODE(Network_Q(this));
    *Network__J(this) = NetArc_HEADNODE(v);
    *Network__P(this) = v;
  } else if (NetArc_FLOW(u) <= NetArc_FLOW(v)) {
    *Network__K(this) = NetArc_HEADNODE(Network_Q(this));
    *Network__L(this) = NetArc_TAILNODE(Network_Q(this));
    *Network__J(this) = NetArc_TAILNODE(u);
    *Network__P(this) = u;
  } else {
    *Network__K(this) = NetArc_TAILNODE(Network_Q(this));
    *Network__L(this) = NetArc_HEADNODE(Network_Q(this));
    *Network__J(this) = NetArc_HEADNODE(v);
    *Network__P(this) = v;
  }
}

#ifdef $XCC__h
/**
 * Find Entering Arc of the primal network simplex algorithm.
 *
 * Ideally, this is an arc with the maximum reduced delay. In order to save
 * computations, we apply a cached major / minor iteration scheme. This relies on
 * the Network_QUEUED where the arcs that are likely to violate Bellman's
 * inequalities are put by @c Network_doUEF and @c Network_doRST.
 */
bool
Network_primalFEA(Network this, bool refill);
#endif//$XCC__h

#ifndef Network_MINORCOUNT
#define Network_MINORCOUNT 64
#endif

bool
Network_primalFEA(Network this, bool refill)
{
  NetArc q = NULL;
  int32_t redmax = 0;
  int32_t minorCount = Network_MINORCOUNT;
  if (refill || Network_QUEUED_isEmpty(this)) {
    Network_ACTIVE_FOREACH_NetArc(this, ARC) {
      int32_t reduced = NetArc_REDUCED(ARC);
      if (reduced > redmax) {
        redmax = reduced;
        q = ARC;
      }
      if (reduced > 0) {
        Network_QUEUED_putArc(this, ARC);
      }
    } Network_ACTIVE_ENDEACH_NetArc;
  } else {
    while (!Network_QUEUED_isEmpty(this)) {
      NetArc ARC = Network_QUEUED_bottomArc(this);
      int32_t reduced = NetArc_REDUCED(ARC);
      if (reduced <= 0) {
        Network_QUEUED_dropArc(this);
      } else break;
    }
    // Here the bottom arc has its REDUCED > 0, so q will be found within minorCount.
    Network_QUEUED_FOREACH_NetArc(this, ARC) {
      int32_t reduced = NetArc_REDUCED(ARC);
      if (reduced > redmax) {
        redmax = reduced;
        q = ARC;
      }
      if (minorCount-- == 0) break;
    } Network_QUEUED_ENDEACH_NetArc;
  }
  *Network__Q(this) = q;
  return q != NULL;
}

#ifdef $XCC_h
/**
 * Network simplex primal solver.
 */
void
Network_primalSolve(Network this, bool refill);
#endif//$XCC_h

void
Network_primalSolve(Network this, bool refill)
{
  Except_DEBUG(Network_checkPrimal(this));
  Except_DEBUG(Network_checkTree(this, true));
  while (Network_primalFEA(this, refill)) {
    Network_primalFLE(this);
    Except_CHECK(NetNode_EDGE(Network_J(this)) == Network_P(this));
    Network_doUEF(this, true);
    Network_doRST(this, true);
    Except_DEBUG(Network_checkTree(this, true));
    refill = false;
  }
  Except_DEBUG(Network_checkSolved(this));
}

#ifdef $XCC_h
/**
 * Update an arc delay and the node dates.
 */
void
Network_updateDelay(Network this, NetArc q, int32_t delay, bool queue);
#endif//$XCC_h

void
Network_updateDelay(Network this, NetArc q, int32_t delay, bool queue)
{
  NetNode j = NULL, k = NULL, l = NULL;
  *NetArc__DELAY(q) = delay;
  if (NetNode_EDGE(NetArc_HEADNODE(q)) == q) {
    k = NetArc_TAILNODE(q), l = NetArc_HEADNODE(q);
    *NetNode__DATE(l) = NetNode_DATE(k) + delay;
    if (queue) Network_QUEUED_putNode(this, l);
  } else if (NetNode_EDGE(NetArc_TAILNODE(q)) == q) {
    k = NetArc_HEADNODE(q), l = NetArc_TAILNODE(q);
    *NetNode__DATE(l) = NetNode_DATE(k) - delay;
    if (queue) Network_QUEUED_putNode(this, l);
  } else {
    if (queue) Network_QUEUED_putArc(this, q);
    return;
  }
  for (j = NetNode_NEXT(l); NetNode_DEPTH(j) > NetNode_DEPTH(l); j = NetNode_NEXT(j)) {
    NetArc p = NetNode_EDGE(j);
    if (NetArc_TAILNODE(p) != j) {
      NetNode i = NetArc_TAILNODE(p);
      *NetNode__DATE(j) = NetNode_DATE(i) + NetArc_DELAY(p);
      if (queue) Network_QUEUED_putNode(this, j);
    } else {
      NetNode i = NetArc_HEADNODE(p);
      *NetNode__DATE(j) = NetNode_DATE(i) - NetArc_DELAY(p);
      if (queue) Network_QUEUED_putNode(this, j);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Network module.
 */
#ifndef _NTRACE
void
Network_INIT(void);
#else//_NTRACE
#define Network_INIT()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
Network_INIT(void)
{
  ETable_Ctor(NetArcFlag_Table, Memory_Root, NetArcFlag_Entries);
}
#endif//_NTRACE

#ifdef $XCC__h
/*
  Network_FINI -- Finalize the Network module.
 */
#ifndef _NTRACE
void
Network_FINI(void);
#else//_NTRACE
#define Network_FINI()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
Network_FINI(void)
{
  ETable_Dtor(NetArcFlag_Table);
}
#endif//_NTRACE

#if XCC__C
static void
Network_TEST(void)
{
#include "CAL/Network_.c"
}

int
main(int argc, char **argv)
{
  CAL_Test(Network_TEST);
  return 0;
}
#endif

