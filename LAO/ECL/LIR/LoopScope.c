#ifdef $XCC_h
/*
 * !!!!	LoopScope.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu)
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
 * @brief Loop Scope information.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/LoopScope.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * IndexedFactory for LoopScope(s).
 */
extern IndexedFactory_ LoopScope_Factory[1];
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the LoopScope flags.
 */
typedef enum {
  LoopScopeFlag_Inner = 0x1,		//!< LoopScope of inner loop.
  LoopScopeFlag_Simple = 0x2,		//!< LoopScope of simple loop.
  LoopScopeFlag_Hardware = 0x4,		//!< LoopScope of hardware loop.
  LoopScopeFlag_Remainder = 0x8,	//!< LoopScope of remainder loop.
} enum_LoopScopeFlag;
typedef uint8_t LoopScopeFlags;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ LoopScopeFlag_Table[1];
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ LoopScopeFlagEntries[] = {
  ETableENTRY(LoopScopeFlag, Inner),
  ETableENTRY(LoopScopeFlag, Simple),
  ETableENTRY(LoopScopeFlag, Hardware),
  ETableENTRY(LoopScopeFlag, Remainder),
  ETableENTRY_NULL
};
ETable_ LoopScopeFlag_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Persistent loop scope information.
 *
 * A LoopScope maintains loop scope information accross optimizations, in particular
 * dependences between memory access operations, user loop pragma, and diagnostics.
 */
struct LoopScope_ {
  //@args	Memory memory, BasicBlock headerBlock, Configure configure
  Indexed_ __;			//!< A LoopScope_ ISA Indexed_.
  //@access IDENTITY	Indexed_IDENTITY(LoopScope____(this))
  //@access INDEX	Indexed_INDEX(LoopScope____(this))
  //@mutate INDEX	Indexed__INDEX(LoopScope____(this))
  Memory MEMORY;			//!< This LoopScope Memory.
  BasicBlock HEADERBLOCK;		//!< The loop header BasicBlock.
  Temporary TRIPCOUNT;			//!< The Temporary that holds the loop trip count.
  struct LoopScope_ *PARENT;		//!< The parent of this LoopScope.
  DependenceTable DEPENDENCETABLE;	//!< The supplied dependences between operations.
  Configure_ CONFIGURE[1];		//!< Configure of this LoopScope.
  //@access COMPENSATION	Configure_COMPENSATION(LoopScope_configure(this))
  //@access SPECULATION		Configure_SPECULATION(LoopScope_configure(this))
  //@access RELAXATION		Configure_RELAXATION(LoopScope_configure(this))
  //@access RENAMING		Configure_RENAMING(LoopScope_configure(this))
  //@access BOOSTING		Configure_BOOSTING(LoopScope_configure(this))
  //@access ALIASING		Configure_ALIASING(LoopScope_configure(this))
  //@access TRIPMODULUS		Configure_TRIPMODULUS(LoopScope_configure(this))
  //@access TRIPRESIDUE		Configure_TRIPRESIDUE(LoopScope_configure(this))
  //@access LOGPREPAD		Configure_LOGPREPAD(LoopScope_configure(this))
  //@access LOGPOSTPAD		Configure_LOGPOSTPAD(LoopScope_configure(this))
  //@access PIPELINING		Configure_PIPELINING(LoopScope_configure(this))
  //@access PRELOADING		Configure_PRELOADING(LoopScope_configure(this))
  //@access L1MISSEXTRA		Configure_L1MISSEXTRA(LoopScope_configure(this))
  LoopScopeFlags FLAGS;		//!< This LoopScope flags.
  //@access isInner		((LoopScope_flags(this) & LoopScopeFlag_Inner) != 0)
  //@access isSimple		((LoopScope_flags(this) & LoopScopeFlag_Simple) != 0)
  //@access isHardware		((LoopScope_flags(this) & LoopScopeFlag_Hardware) != 0)
  //@access isRemainder		((LoopScope_flags(this) & LoopScopeFlag_Remainder) != 0)
  //@access isCounted		(LoopScope_tripCount(this) != NULL)
  int16_t NESTING;			//!< Nesting level of this LoopScope.
  int16_t BLOCKCOUNT;			//!< Count of BasicBlocks(s) with same HEADERBLOCK.
  int16_t PEELCOUNT;			//!< The amount of loop peeling applied.
  int16_t UNROLLED;			//!< The amount of loop unrolling applied.
  int16_t KUNROLL;			//!< The amount of kernel unrolling applied.
  int16_t LAMBDA;			//!< II after loop scheduling or pipelining.
  int16_t LENGTH;			//!< Makespan after loop scheduling or pipelining.
  int16_t LATENCY;			//!< Critical cycle latency in cycles.
  int16_t DISTANCE;			//!< Critical cycle distance in iterations.
};
#endif//$XCC_h

#ifdef $XCC_h
static inline Indexed
LoopScope__(LoopScope this) {
  return LoopScope____(this);
}
static inline const_Indexed
LoopScope__const(const_LoopScope this) {
  return LoopScope____(this);
}
#endif//$XCC_h

LoopScope
LoopScope_Ctor(LoopScope this,
               Memory memory, BasicBlock headerBlock, Configure configure)
{
  Indexed_Ctor(LoopScope____(this), LoopScope_Factory);
  *LoopScope__MEMORY(this) = memory;
  *LoopScope__HEADERBLOCK(this) = headerBlock;
  *LoopScope__TRIPCOUNT(this) = NULL;
  *LoopScope__PARENT(this) = NULL;
  *LoopScope__DEPENDENCETABLE(this) = NULL;
  Configure_Copy(LoopScope_CONFIGURE(this), configure);
  *LoopScope__FLAGS(this) = 0;
  *LoopScope__NESTING(this) = 0;
  *LoopScope__BLOCKCOUNT(this) = 0;
  *LoopScope__PEELCOUNT(this) = 0;
  *LoopScope__UNROLLED(this) = 0;
  *LoopScope__KUNROLL(this) = 0;
  *LoopScope__LAMBDA(this) = 0;
  *LoopScope__LENGTH(this) = 0;
  *LoopScope__LATENCY(this) = 0;
  *LoopScope__DISTANCE(this) = 0;
  return this;
}

void
LoopScope_Dtor(LoopScope this)
{
  LoopScope_killDependenceTable(this);
}

size_t
LoopScope_Size(Memory memory, BasicBlock headerBlock, Configure configure)
{
  return sizeof(LoopScope_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LoopScope_)\t%zu\n", sizeof(LoopScope_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This LoopScope identity.
 */
static inline uint32_t
LoopScope_identity(const_LoopScope this)
{
  return LoopScope_IDENTITY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This LoopScope loop head block.
 */
static inline BasicBlock
LoopScope_headerBlock(const_LoopScope this)
{
  return LoopScope_HEADERBLOCK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The loop tripcount Temporary when known, else NULL.
 */
static inline Temporary
LoopScope_tripCount(const_LoopScope this)
{
  return LoopScope_TRIPCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the loop tripcount.
 */
static inline void
LoopScope_setTripCount(LoopScope this, Temporary tripCount)
{
  *LoopScope__TRIPCOUNT(this) = tripCount;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The loop configure.
 */
static inline Configure
LoopScope_configure(LoopScope this)
{
  return LoopScope_CONFIGURE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This LoopScope flags.
 */
static inline LoopScopeFlags
LoopScope_flags(const_LoopScope this)
{
  return LoopScope_FLAGS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this LoopScope flags.
 */
static inline void
LoopScope_setFlags(LoopScope this, LoopScopeFlags flags)
{
  *LoopScope__FLAGS(this) |= flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Reset this LoopScope flags.
 */
static inline void
LoopScope_clearFlags(LoopScope this, LoopScopeFlags flags)
{
  *LoopScope__FLAGS(this) &= ~flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Nesting of this LoopScope.
 */
static inline int
LoopScope_nesting(const_LoopScope this)
{
  return LoopScope_NESTING(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of BasicBlock(s) with same header block as this LoopScope.
 */
static inline int
LoopScope_blockCount(const_LoopScope this)
{
  return LoopScope_BLOCKCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The amount of loop peeling applied.
 */
static inline int
LoopScope_peelCount(const_LoopScope this)
{
  return LoopScope_PEELCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The amount of loop unrolling applied.
 */
static inline int
LoopScope_unrolled(const_LoopScope this)
{
  return LoopScope_UNROLLED(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the amount of loop unrolling.
 */
static inline void
LoopScope_setUnrolled(LoopScope this, int unrolled)
{
  *LoopScope__UNROLLED(this) = unrolled;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Update the amount of loop unrolling.
 */
static inline void
LoopScope_updateUnrolled(LoopScope this, int kunroll)
{
  int unrolled = LoopScope_UNROLLED(this);
  Except_CHECK(kunroll > 0);
  if (unrolled > 0) {
    *LoopScope__UNROLLED(this) = unrolled*kunroll;
  } else {
    *LoopScope__UNROLLED(this) = kunroll;
  }
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This LoopScope parent.
 */
static inline LoopScope
LoopScope_parent(const_LoopScope this)
{
  return LoopScope_PARENT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a DependenceTable for this LoopScope.
 */
DependenceTable
LoopScope_makeDependenceTable(LoopScope this);
#endif//$XCC_h

DependenceTable
LoopScope_makeDependenceTable(LoopScope this)
{
  DependenceTable dependenceTable = LoopScope_DEPENDENCETABLE(this);
  if (dependenceTable == NULL) {
    Memory memory = LoopScope_MEMORY(this);
    size_t size = DependenceTable_Size(memory);
    dependenceTable = Memory_alloc(memory, size);
    DependenceTable_Ctor(dependenceTable, memory);
    *LoopScope__DEPENDENCETABLE(this) = dependenceTable;
  }
  return dependenceTable;
}

#ifdef $XCC_h
/**
 * Kill the DependenceTable of this LoopScope.
 */
void
LoopScope_killDependenceTable(LoopScope this);
#endif//$XCC_h

void
LoopScope_killDependenceTable(LoopScope this)
{
  DependenceTable dependenceTable = LoopScope_DEPENDENCETABLE(this);
  if (dependenceTable != NULL) {
    Memory memory = LoopScope_MEMORY(this);
    DependenceTable_Dtor(dependenceTable);
    Memory_free_(memory, dependenceTable);
  }
  *LoopScope__DEPENDENCETABLE(this) = NULL;
}

#ifdef $XCC_h
/**
 * This LoopScope dependenceTable.
 */
static inline DependenceTable
LoopScope_dependenceTable(const_LoopScope this)
{
  return LoopScope_DEPENDENCETABLE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Transfer the contents of that LoopScope to this LoopScope.
 */
void
LoopScope_transfer(LoopScope this, LoopScope that);
#endif//$XCC_h

void
LoopScope_transfer(LoopScope this, LoopScope that)
{
  Except_REQUIRE(LoopScope_DEPENDENCETABLE(this) == NULL);
  *LoopScope__DEPENDENCETABLE(this) = LoopScope_DEPENDENCETABLE(that);
  *LoopScope__DEPENDENCETABLE(that) = NULL;
  *LoopScope_CONFIGURE(this) = *LoopScope_CONFIGURE(that);
}

#ifdef $XCC_h
/**
 * Update this LoopScope after control-flow change.
 */
void
LoopScope_update(LoopScope this, LoopScope parent_scope,
                int blockCount, bool isSimpleLoop, bool isInnerLoop);
#endif//$XCC_h

void
LoopScope_update(LoopScope this, LoopScope parent_scope,
                int blockCount, bool isSimpleLoop, bool isInnerLoop)
{
  LoopScopeFlags flags = LoopScope_FLAGS(this);
  Except_REQUIRE(parent_scope != this);
  *LoopScope__PARENT(this) = parent_scope;
  *LoopScope__BLOCKCOUNT(this) = blockCount;
  *LoopScope__NESTING(this) = LoopScope_NESTING(parent_scope) + 1;
  flags &= ~(LoopScopeFlag_Inner | LoopScopeFlag_Simple);
  if (isInnerLoop) flags |= LoopScopeFlag_Inner;
  if (isSimpleLoop) flags |= LoopScopeFlag_Simple;
  *LoopScope__FLAGS(this) = flags;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty-print this LoopScope.
 */
bool
LoopScope_pretty(const_LoopScope this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
LoopScope_pretty(const_LoopScope this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "    LoopScope_%d:", LoopScope_IDENTITY(this));
  fprintf(file, "\n\tconfigure:");
  Configure_pretty(LoopScope_CONFIGURE(this), file);
  fprintf(file, "\n\tattributes:");
  if (LoopScope_HEADERBLOCK(this) != NULL) {
    BasicBlock headerBlock = LoopScope_HEADERBLOCK(this);
    fprintf(file, "\theader=%d:Block_%d",
        BasicBlock_identity(headerBlock),
        BasicBlock_index(headerBlock));
  }
  if (LoopScope_PARENT(this) != NULL) {
    fprintf(file, "\tparent=LoopScope_%d", LoopScope_IDENTITY(LoopScope_PARENT(this)));
  }
  if (LoopScope_isInner(this)) fprintf(file, "\tisInner");
  if (LoopScope_isSimple(this)) fprintf(file, "\tisSimple");
  if (LoopScope_isHardware(this)) fprintf(file, "\tisHardware");
  if (LoopScope_isRemainder(this)) fprintf(file, "\tisRemainder");
  if (LoopScope_TRIPCOUNT(this) != NULL) {
    fprintf(file, "\ttripCount=");
    Temporary_pretty(LoopScope_TRIPCOUNT(this), file);
  }
  if (LoopScope_UNROLLED(this) != 0) {
    fprintf(file, "\tunrolled=%d", LoopScope_UNROLLED(this));
  }
  fprintf(file, "\n");
  if (LoopScope_DEPENDENCETABLE(this) != NULL) {
    DependenceTable_FOREACH_DependenceArc(LoopScope_DEPENDENCETABLE(this), arc) {
      Operation tail_operation = DependenceArc_TAIL(arc);
      Operation head_operation = DependenceArc_HEAD(arc);
      uint32_t tail_identity = Operation_identity(tail_operation);
      uint32_t head_identity = Operation_identity(head_operation);
      int omega = DependenceArc_OMEGA(arc);
      fprintf(file, "\t(%d:,%d:)=>%d", tail_identity, head_identity, omega);
    } DependenceTable_ENDEACH_DependenceArc;
    fprintf(file, "\n");
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize LoopScope module.
 */
void
LoopScope_INIT(void);
#endif//$XCC__h

IndexedFactory_ LoopScope_Factory[1];

void
LoopScope_INIT(void)
{
  IndexedFactory_Ctor(LoopScope_Factory);
#ifndef _NTRACE
  ETable_Ctor(LoopScopeFlag_Table, Memory_Root, LoopScopeFlagEntries);
#endif//_NTRACE
}

#ifdef $XCC__h
/**
 * Finalize LoopScope module.
 */
#ifndef _NTRACE
void
LoopScope_FINI(void);
#else//_NTRACE
#define LoopScope_FINI()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
LoopScope_FINI(void)
{
  ETable_Dtor(LoopScopeFlag_Table);
}
#endif//_NTRACE

#if XCC__C
static void
LoopScope_TEST(void)
{
#include "LIR/LoopScope_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(LoopScope_TEST);
  return 0;
}
#endif

