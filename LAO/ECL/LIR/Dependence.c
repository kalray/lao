#ifdef $XCC_h
/*
 * !!!!	Dependence.xcc
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
 * @brief Dependence nodes and arcs.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/Dependence.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Dependence kinds.
 */
typedef enum {
  DependenceKind_Flow,		//!< Flow memory dependence arc.
  DependenceKind_Anti,		//!< Anti memory dependence arc.
  DependenceKind_Input,		//!< Input memory dependence arc.
  DependenceKind_Output,	//!< Output memory dependence arc.
  DependenceKind_Spill,		//!< Spill memory dependence arc.
  DependenceKind_Other,		//!< Other memory dependence arc.
  DependenceKind_Relax,		//!< Induction relaxed dependence arc.
  DependenceKind_RAW,		//!< Register Read After Write dependence arc.
  DependenceKind_WAR,		//!< Register Write After Read dependence arc.
  DependenceKind_WAW,		//!< Register Write After Write dependence arc.
  DependenceKind_Life,		//!< Register consumer node to its lifetime node.
  DependenceKind_Link,		//!< Register producer node to its lifetime node.
  DependenceKind_Reuse,		//!< Register reuse arc.
  DependenceKind_Some,		//!< Inserted by DepGraph_complete.
  DependenceKind_Margin,	//!< Inserted by DepGraph_complete.
  DependenceKind__,
  DependenceKind_Definite = 16,
} enum_DependenceKind;
typedef uint8_t DependenceKind;
extern const char *
DependenceKind_Id(DependenceKind this);
#define DependenceKind(kind) ((kind) & (DependenceKind_Definite - 1))
#define DependenceKind_mayAdjust(kind) ((kind) <= DependenceKind_WAW)
#define DependenceKind_mayRefine(kind) ((kind) <= DependenceKind_Spill)
#endif//$XCC_h

static const char *DependenceKind_Ids[] = {
  "Flow",
  "Anti",
  "Input",
  "Output",
  "Spill",
  "Other",
  "Relax",
  "RAW",
  "WAR",
  "WAW",
  "Life",
  "Link",
  "Reuse",
  "Some",
  "Margin",
};


#ifndef _NTRACE
const char *
DependenceKind_Id(DependenceKind kind)
{
  DependenceKind this = DependenceKind(kind);
  return DependenceKind_Ids[this];
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Representation of a dependence arc.
 */
DependenceKind
DependenceKind_ofName(const char *name);
#endif//$XCC_h

DependenceKind
DependenceKind_ofName(const char *name)
{
  DependenceKind kind = 0;
  while(kind < DependenceKind__ &&
      strcmp(name, DependenceKind_Ids[kind]))
    kind++;
  return kind;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Representation of a Dependence node.
 */
typedef void *DependenceNode;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Representation of a dependence arc.
 */
struct DependenceArc_ {
  //@args	struct Operation_ *tail, struct Operation_ *head,
  //@args	int latency, int omega, DependenceKind kind
  struct Operation_ *TAIL;
  struct Operation_ *HEAD;
  int16_t LATENCY;
  int8_t OMEGA;
  DependenceKind KIND;
};
#endif//$XCC_h

DependenceArc
DependenceArc_Ctor(DependenceArc this,
                   struct Operation_ *tail, struct Operation_ *head,
                   int latency, int omega, DependenceKind kind)
{
  *DependenceArc__TAIL(this) = tail;
  *DependenceArc__HEAD(this) = head;
  *DependenceArc__LATENCY(this) = latency;
  *DependenceArc__OMEGA(this) = omega;
  *DependenceArc__KIND(this) = kind;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(DependenceArc_)\t%zu\n", sizeof(DependenceArc_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Maintains DependenceNode(s) and DependenceArc(s).
 */
struct DependenceTable_ {
  //@args	Memory memory
  PtrSet_ NODES_;
  //@access NODES	DependenceTable__NODES_(this)
  //@access MEMORY	PtrSet_memory(DependenceTable_NODES(this))
  IBList_ ARCS_;
  //@access ARCS	DependenceTable__ARCS_(this)
  //@access ARCCOUNT	(IBList_count(DependenceTable_ARCS(this)))
};
#endif//$XCC_h

DependenceTable
DependenceTable_Ctor(DependenceTable this,
                     Memory memory)
{
  PtrSet_Ctor(DependenceTable_NODES(this), memory, 0);
  IBList_Ctor(DependenceTable_ARCS(this),
              memory, sizeof(DependenceArc_), 30);
  return this;
}

void
DependenceTable_Dtor(DependenceTable this)
{
  PtrSet_Dtor(DependenceTable_NODES(this));
  IBList_Dtor(DependenceTable_ARCS(this));
}

size_t
DependenceTable_Size(Memory memory)
{
  return sizeof(DependenceTable_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(DependenceTable_)\t%zu\n", sizeof(DependenceTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * For use by DependenceTable_FOREACH_DependenceNode
 */
static inline const_PtrSet
DependenceTable_nodes(const_DependenceTable this)
{
  return DependenceTable_NODES(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 *  Enter a DependenceNode in this DependenceTable.
 */
static inline DependenceNode
DependenceTable_enterNode(DependenceTable this, DependenceNode node)
{
  PtrSet_insert(DependenceTable_NODES(this), node);
  return node;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff DependenceTable has no DependenceNode(s).
 */
static inline bool
DependenceTable_isEmpty(const_DependenceTable this)
{
  return PtrSet_isEmpty(DependenceTable_NODES(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff Dependence node in this DependenceTable.
 */
static inline bool
DependenceTable_isNode(const_DependenceTable this, DependenceNode node)
{
  return PtrSet_contains(DependenceTable_NODES(this), node);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by DependenceTable_FOREACH_DependenceArc
 */
static inline const_IBList
DependenceTable_arcs(const_DependenceTable this)
{
  return DependenceTable_ARCS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this DependenceTable DependenceArc(s).
 */
#define DependenceTable_FOREACH_DependenceArc(this, arc) \
  IBList_FOREACH(DependenceTable_arcs(this), DependenceArc_, arc)
#define DependenceTable_ENDEACH_DependenceArc \
  IBList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a new DependenceArc in this DependenceTable.
 */
DependenceArc
DependenceTable_makeArc(DependenceTable this,
                        Operation tail_operation, Operation head_operation,
                        int latency, int omega, DependenceKind kind);
#endif//$XCC_h

DependenceArc
DependenceTable_makeArc(DependenceTable this,
                        Operation tail_operation, Operation head_operation,
                        int latency, int omega, DependenceKind kind) {
  DependenceArc arc = IBList_push(DependenceTable_ARCS(this));
  return DependenceArc_Ctor(arc, tail_operation, head_operation, latency, omega, kind);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize Dependence module.
 */
#define Dependence_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize Dependence module.
 */
#define Dependence_FINI()
#endif//$XCC__h

#if XCC__C
static void
Dependence_TEST(void)
{
#include "LIR/Dependence_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Dependence_TEST);
  return 0;
}
#endif

