#ifdef $XCC_h
/*
 * !!!!	Variable.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu).
 * Christophe Guillon (Christophe.Guillon@st.com).
 * Duco van Amstel (duco.van-amstel@kalray.eu).
 *
 * Copyright 2010 - 2013 Kalray.
 * Copyright 2007 - 2008 STMicroelectronics.
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
 * @brief SSA Variable and VariableClass.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/Variable.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Variable flags.
 */
typedef enum {
  VariableFlag_Consuming = 0x1,		//!< Variable has a CONSUMERS list.
  VariableFlag_Coalesced = 0x2,		//!< Definition has been coalesced.
  VariableFlag_Unresolved = 0x4,	//!< Unresolved in Sreedhar.
  VariableFlag_NoSpill = 0x8,		//!< This variable must not be spilled
  VariableFlag_PCOPYDef = 0x10,		//!< Defined in parallel COPY (PCOPY removal only)
  VariableFlag_PCOPYUse = 0x20,		//!< Used in parallel COPY (PCOPY removal only)
  VariableFlag_Isolated = 0x40,         //!< For Decoalescing, mark an isolated variable
} enum_VariableFlag;
typedef uint8_t VariableFlags;
#define Variable_isConsuming(this) \
  ((Variable_FLAGS(this) & VariableFlag_Consuming) != 0)
#define Variable_raiseConsuming(this) \
  (*Variable__FLAGS(this) |= VariableFlag_Consuming)
#define Variable_clearConsuming(this) \
  (*Variable__FLAGS(this) &= ~VariableFlag_Consuming)
#define Variable_isCoalesced(this) \
  ((Variable_FLAGS(this) & VariableFlag_Coalesced) != 0)
#define Variable_raiseCoalesced(this) \
  (*Variable__FLAGS(this) |= VariableFlag_Coalesced)
#define Variable_clearCoalesced(this) \
  (*Variable__FLAGS(this) &= ~VariableFlag_Coalesced)
#define Variable_isUnresolved(this) \
  ((Variable_FLAGS(this) & VariableFlag_Unresolved) != 0)
#define Variable_raiseUnresolved(this) \
  (*Variable__FLAGS(this) |= VariableFlag_Unresolved)
#define Variable_clearUnresolved(this) \
  (*Variable__FLAGS(this) &= ~VariableFlag_Unresolved)
#define Variable_isNoSpill(this) \
  ((Variable_FLAGS(this) & VariableFlag_NoSpill) != 0)
#define Variable_raiseNoSpill(this) \
  (*Variable__FLAGS(this) |= VariableFlag_NoSpill)
#define Variable_clearNoSpill(this) \
  (*Variable__FLAGS(this) &= ~VariableFlag_NoSpill)
#define Variable_isPCOPYDef(this) \
  ((Variable_FLAGS(this) & VariableFlag_PCOPYDef) != 0)
#define Variable_raisePCOPYDef(this) \
  (*Variable__FLAGS(this) |= VariableFlag_PCOPYDef)
#define Variable_clearPCOPYDef(this) \
  (*Variable__FLAGS(this) &= ~VariableFlag_PCOPYDef)
#define Variable_isPCOPYUse(this) \
  ((Variable_FLAGS(this) & VariableFlag_PCOPYUse) != 0)
#define Variable_raisePCOPYUse(this) \
  (*Variable__FLAGS(this) |= VariableFlag_PCOPYUse)
#define Variable_clearPCOPYUse(this) \
  (*Variable__FLAGS(this) &= ~VariableFlag_PCOPYUse)
#define Variable_isIsolated(this) \
  ((Variable_FLAGS(this) & VariableFlag_Isolated) != 0)
#define Variable_raiseIsolated(this) \
  (*Variable__FLAGS(this) |= VariableFlag_Isolated)
#define Variable_clearIsolated(this) \
  (*Variable__FLAGS(this) &= ~VariableFlag_Isolated)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * SSA form variable, ISA Temporary.
 */
struct Variable_ {
  //@args	Temporary origin
  Temporary_ __;			//!< Base of this Variable.
  //@access TEMPORARY	Variable____(this)
  //@access IDENTITY	Temporary_identity(Variable_TEMPORARY(this))
  //@access INDEX	Temporary_index(Variable_TEMPORARY(this))
  OperationResult_ PRODUCER[1];		//!< The producer OperationResult.
  //@access DEFUSES	((DefUses)Variable_PRODUCER(this))
  IDList_ CONSUMERS_;			//!< The consumers list of OperationArgument(s).
  //@access CONSUMERS	Variable__CONSUMERS_(this)
  Temporary ORIGIN;			//!< The origin Temporary.
  struct Variable_ *WEBLINK;		//!< Uplink for union-find in the SSA web.
  int32_t WEBRANK;			//!< Rank for union-find in the SSA web.
  VariableFlags FLAGS;			//!< This Variable flags.
  Register PINNED;			//!< Variable is pinned to this register.
  Register LOCALLY;			//!< Variable is locally assigned to this register.
  Register GLOBALLY;			//!< Variable is globally assigned to this register.
  //@access isAssigned 	(Temporary_hasRegister(Variable_TEMPORARY(this)))
  Symbol MEMORY;			//!< Variable is assigned to memory at this location.
  //@access hasMemory (Variable_MEMORY(this) != NULL)
  struct Variable_ *CLASSNEXT;		//!< Next Variable in congruence class.
  struct VariableClass_ *CONGRUENCE;	//!< Coalescer congruence class.
  PtrSet_ *NEIGHBORS;			//!< Sreedhar unresolved neighbors.
  int32_t ORDERING;			//!< Ordering in VariableClass.
  struct Variable_ *IDOM;		//!< (Decoalescer)
  struct Variable_ *EANC;		//!< (Decoalescer)
  struct Variable_ *EQUIVALUE;		//!< Root of class of Variable(s) of same value.
  struct Variable_ *EQUALANCIN;		//!< For use by interference with equalities.
  struct Variable_ *EQUALANCOUT;	//!< For use by interference with equalities.
  struct Variable_ *CURDOM;		//!< (Decoalescer)
  struct Operation_ *CURPHI;	        //!< (Decoalescer)
};
#endif//$XCC_h

#ifdef $XCC_h
static inline Temporary
Variable__(Variable this)
{
  Temporary temporary = Variable____(this);
  Except_CHECK(Temporary_isVariable(temporary));
  return temporary;
}
static inline const_Temporary
Variable__const(const_Variable this)
{
  const_Temporary temporary = Variable____(this);
  Except_CHECK(Temporary_isVariable(temporary));
  return temporary;
}
#endif//$XCC_h

Variable
Variable_Ctor(Variable this,
              Temporary origin)
{
  Temporary temporary = Variable____(this);
  Temporary_Ctor(temporary, TemporaryType_Variable, TemporaryFlag_RenameOK);
     // Rename_OK flags is reused here to mark temporaries that are SSA 
     // variables.
  *Temporary__REGFILE(temporary) = Temporary_REGFILE(origin);
  *Temporary__INDEX(temporary) = Temporary_INDEX(origin);
  *Variable__ORIGIN(this) = origin;
  OperationResult_Clear(Variable_PRODUCER(this));
  IDList_Ctor(Variable_CONSUMERS(this), NULL);
  *Variable__WEBLINK(this) = NULL;
  *Variable__WEBRANK(this) = 0;
  *Variable__FLAGS(this) = VariableFlag_Isolated;
  *Variable__PINNED(this) = Register__UNDEF;
  *Variable__LOCALLY(this) = Register__UNDEF;
  *Variable__GLOBALLY(this) = Register__UNDEF;
  *Variable__MEMORY(this) = NULL;
  *Variable__CLASSNEXT(this) = NULL;
  *Variable__CONGRUENCE(this) = NULL;
  *Variable__NEIGHBORS(this) = NULL;
  *Variable__ORDERING(this) = 0;
  *Variable__IDOM(this) = NULL;
  *Variable__EANC(this) = NULL;
  *Variable__EQUIVALUE(this) = this;
  *Variable__EQUALANCIN(this) = NULL;
  *Variable__EQUALANCOUT(this) = NULL;
  *Variable__CURDOM(this) = NULL;
  *Variable__CURPHI(this) = NULL;
  return this;
}

void
Variable_Dtor(Variable this)
{
  IDList_Dtor(Variable_CONSUMERS(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Variable_)\t%zu\n", sizeof(Variable_));
}
#endif//$XCC__c

#ifdef $XCC_h
static inline VariableFlags
Variable_flags(const_Variable this)
{
  return Variable_FLAGS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the producer of this variable
 */
static inline const_OperationResult
Variable_producer(const_Variable this)
{
  return Variable_PRODUCER(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Used by Variable_CONSUMERS_FOREACH_OperationArgument.
 */
static inline const_IDList
Variable_consumers(const_Variable this)
{
  return Variable_CONSUMERS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
#define Variable_CONSUMERS_FOREACH_OperationArgument(this, argument) \
  IDList_FOREACH(Variable_consumers(this), OperationArgument_, argument)
#define Variable_CONSUMERS_ENDEACH_OperationArgument \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The register associated with a Variable.
 *
* @return the Register, or Register__UNDEF for non-Register Variables.
 */
static inline Register
Variable_register(const_Variable this)
{
  const_Temporary temporary = Variable_TEMPORARY(this);
  Except_CHECK(Temporary_hasRegister(temporary) || Temporary_REGISTER(temporary) == Register__UNDEF);
  return Temporary_REGISTER(temporary);
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Assign a Register to this Variable.
 */
void
Variable_assignRegister(Variable this, Register registre);
#endif//$XCC_h

void
Variable_assignRegister(Variable this, Register registre)
{
  Temporary temporary = Variable_TEMPORARY(this);
  Except_REQUIRE(Temporary_hasRegFile(temporary));
  *Temporary__FLAGS(temporary) |= TemporaryFlag_Register;
  /* FIXME! Check that registre belongs to Temporary_REGFILE. */
  *Temporary__REGISTER(temporary) = registre;
}

#ifdef $XCC_h
/**
 * Un-assign this Variable.
 */
void
Variable_unassignRegister(Variable this);
#endif//$XCC_h

void
Variable_unassignRegister(Variable this)
{
  Temporary temporary = Variable_TEMPORARY(this);
  Register registre = Temporary_REGISTER(temporary);
  Except_REQUIRE(Temporary_hasRegFile(temporary));
  *Temporary__FLAGS(temporary) &= ~TemporaryFlag_Register;
  *Temporary__REGISTER(temporary) = Register__UNDEF;
}

#ifdef $XCC_h
/**
 * Return the memory symbol for this variable.
 */
Symbol
Variable_memory(const_Variable this);
#endif//$XCC_h

Symbol
Variable_memory(const_Variable this)
{
  return Variable_MEMORY(this);
}

#ifdef $XCC_h
/**
 * Set the memory symbol for this variable.
 */
void
Variable_setMemory(Variable this, Symbol symbol);
#endif//$XCC_h

void
Variable_setMemory(Variable this, Symbol symbol)
{
  *Variable__MEMORY(this) = symbol;
}




#ifdef $XCC_h
/**
 * Add an OperationArgument to this Variable consumers.
 */
void
Variable_addConsumer(Variable this, Operation operation, int32_t argIndex);
#endif//$XCC_h

void
Variable_addConsumer(Variable this, Operation operation, int32_t argIndex)
{
  IDList consumers = Variable_CONSUMERS(this);
  Except_CHECK(Variable_isConsuming(this) && IDList_memory(consumers) != NULL);
  OperationArgument_Ctor(IDList_push(consumers, sizeof(OperationArgument_)),
                         operation, argIndex);
}

#ifdef $XCC_h
/**
 * Make this Variable consumers list.
 */
void
Variable_makeConsumers(Variable this, Memory memory);
#endif//$XCC_h

void
Variable_makeConsumers(Variable this, Memory memory)
{
  IDList consumers = Variable_CONSUMERS(this);
  if (IDList_memory(consumers) != NULL) {
    IDList_empty(consumers, NULL);
  } else {
    IDList_Ctor(consumers, memory);
  }
  Variable_raiseConsuming(this);
}

#ifdef $XCC_h
/**
 * Kill this Variable consumers list.
 */
void
Variable_killConsumers(Variable this);
#endif//$XCC_h

void
Variable_killConsumers(Variable this)
{
  IDList consumers = Variable_CONSUMERS(this);
  if (IDList_memory(consumers) != NULL) {
    IDList_empty(consumers, NULL);
    IDList_Ctor(consumers, NULL);
  }
  Variable_clearConsuming(this);
}

#ifdef $XCC_h
/**
 * Set an OperationResult for this Variable.
 *
 * In SSA form, should always use this function instead of Operation_setResult.
 */
void
Variable_setOperationResult(Variable this, Operation operation, int32_t resIndex);
#endif//$XCC_h

void
Variable_setOperationResult(Variable this, Operation operation, int32_t resIndex)
{
  Operator operator = Operation_operator(operation);
  OperationResult_Ctor(Variable_PRODUCER(this), operation, resIndex);
#if 0
  Except_CHECK(   Variable_ORDERING(this) == 0
               || Variable_CONGRUENCE(this) == NULL
               || Variable_ORDERING(this) == Operation_ordering(operation));
#endif
  *Variable__ORDERING(this) = Operation_ordering(operation);
  Operation_setResult(operation, resIndex, Variable_TEMPORARY(this));
  if (Operator_isSeqCopy(operator) || Operator_isParCopy(operator)) {
    Temporary argument = Operation_getArgument(operation, resIndex);
    if (Temporary_isVariable(argument)) {
      Variable v1 = this, v2 = Temporary_VARIABLE(argument);
      if (Variable_EQUIVALUE(v1) != v1) *Variable__EQUIVALUE(v2) = Variable_EQUIVALUE(v1);
      else if (Variable_EQUIVALUE(v2) != v2) *Variable__EQUIVALUE(v1) = Variable_EQUIVALUE(v2);
      else {
        uint32_t id1 = Temporary_IDENTITY(Variable_TEMPORARY(v1));
        uint32_t id2 = Temporary_IDENTITY(Variable_TEMPORARY(v2));
        if (id1 > id2) *Variable__EQUIVALUE(v1) = Variable_EQUIVALUE(v2);
        else *Variable__EQUIVALUE(v2) = Variable_EQUIVALUE(v1);
      }
    }
  }
}

#ifdef $XCC_h
/**
 * Set an OperationArgument for this Variable.
 *
 * If another Variable was used as argument at the same index, this operation 
 * is removed from its list of consumers.
 * In SSA form, should always use this function instead of Operation_setArgument.
 */
void
Variable_setOperationArgument(Variable this, Operation operation, int32_t argIndex);
#endif//$XCC_h

void
Variable_setOperationArgument(Variable this, Operation operation, int32_t argIndex)
{
  if (Variable_isConsuming(this)) {
    Temporary oldTemp = Operation_getArgument(operation, argIndex);
    Variable oldVar = oldTemp? Temporary_VARIABLE(oldTemp): NULL;
    if (oldVar) {
      OperationArgument oldOpArg = NULL;
      Variable_CONSUMERS_FOREACH_OperationArgument(oldVar, argument) {
        Operation thisOp = OperationArgument_OPERATION(argument);
        int32_t thisArgIndex = OperationArgument_ARGINDEX(argument);
        if (thisOp == operation && thisArgIndex == argIndex) {
          oldOpArg = argument;
          break;
        }
      } Variable_CONSUMERS_ENDEACH_OperationArgument;
      if (oldOpArg) IDList_remove(Variable_CONSUMERS(oldVar), oldOpArg, NULL);
    }
  }
  Operation_setArgument(operation, argIndex, Variable____(this));
  if (Variable_isConsuming(this)) {
    Variable_addConsumer(this, operation, argIndex);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Returns the root of a variable SSA web.
 */
Variable
Variable_findWebRoot(Variable this);
#endif//$XCC_h

Variable
Variable_findWebRoot(Variable this)
{
  Variable root = this;
  Variable webLink = Variable_WEBLINK(this);
  /* Find root */
  while (webLink != NULL) {
    root = webLink;
    webLink = Variable_WEBLINK(webLink);
  }
  /* Update weblinks */
  webLink = this;
  while (webLink != root) {
    *Variable__WEBLINK(webLink) = root;
    webLink = Variable_WEBLINK(webLink);
  }
  return root;
}

#ifdef $XCC_h
/**
 * Link the roots of two variable SSA webs.
 */
Variable
Variable_linkWebRoots(Variable this, Variable that);
#endif//$XCC_h

Variable
Variable_linkWebRoots(Variable this, Variable that)
{
  Variable root = this;
  Register pinned = Register__UNDEF;
  int this_rank = Variable_WEBRANK(this);
  int that_rank = Variable_WEBRANK(that);
  Register this_pinned = Variable_PINNED(this);
  Register that_pinned = Variable_PINNED(that);
  // FdF 20100625: Fixed assert
  Except_CHECK(this != that || (this_pinned == that_pinned));
  Except_CHECK(Variable_WEBLINK(this) == NULL);
  Except_CHECK(Variable_WEBLINK(that) == NULL);
  Except_CHECK((this_pinned == Register__UNDEF) || (that_pinned == Register__UNDEF) ||
               (this_pinned == that_pinned));
  // FdF 20100629: For use in Decoalescer, we cannot change the pinning of a variable here
#if 0
  if (this_pinned != Register__UNDEF) pinned = this_pinned;
  if (that_pinned != Register__UNDEF) pinned = that_pinned;
#endif
  if (this_pinned != that_pinned) {
    if (this_pinned != Register__UNDEF) {
      Except_CHECK(that_pinned == Register__UNDEF);
      *Variable__WEBLINK(that) = this;
    }
    else {
      Except_CHECK(that_pinned != Register__UNDEF);
      *Variable__WEBLINK(this) = that;
      root = that;
    }
  }
  else if (this_rank > that_rank) {
    *Variable__WEBLINK(that) = this;
  }
  else if (this_rank < that_rank) {
    *Variable__WEBLINK(this) = that;
    root = that;
  }
  else if (this != that) {
    *Variable__WEBLINK(that) = this;
    *Variable__WEBRANK(this) = this_rank + 1;
  }
#if 0
  *Variable__PINNED(root) = pinned;
#endif
0 &&
  LIR_TRACE && fprintf(LIR_TRACE, "LinkWebRoots(") && Variable_pretty(this, LIR_TRACE, false)
            && fprintf(LIR_TRACE, "[%d],", this_rank) &&  Variable_pretty(that, LIR_TRACE, false)
            && fprintf(LIR_TRACE, "[%d])=", that_rank) &&  Variable_pretty(root, LIR_TRACE, false)
            && fprintf(LIR_TRACE, "[%d]\n", Variable_WEBRANK(root));
  return root;
}

#ifdef $XCC_h
/**
 * Compare two Variable(s) for total ordering.
 */
int
Variable_compare(const_Variable this, const_Variable that);
#endif//$XCC_h

int
Variable_compare(const_Variable this, const_Variable that)
{
  int32_t this_ordering = Variable_ORDERING(this);
  int32_t that_ordering = Variable_ORDERING(that);
  uint32_t this_identity = Variable_IDENTITY(this);
  uint32_t that_identity = Variable_IDENTITY(that);
  if (sizeof(int) >= sizeof(int32_t)) {
    if (this_ordering != that_ordering) {
      return this_ordering - that_ordering;
    }
    return (int)(this_identity - that_identity);
  } else {
    if (this_ordering != that_ordering) {
      return (this_ordering > that_ordering)
           - (this_ordering < that_ordering);
    }
    return (this_identity > that_identity)
         - (this_identity < that_identity);
  }
}

static inline bool
Variable_GT(const_Variable this, const_Variable that)
{
  int32_t this_ordering = Variable_ORDERING(this);
  int32_t that_ordering = Variable_ORDERING(that);
  if (this_ordering > that_ordering) return true;
  else if (this_ordering == that_ordering) {
    uint32_t this_identity = Variable_IDENTITY(this);
    uint32_t that_identity = Variable_IDENTITY(that);
    if (this_identity > that_identity) return true;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Sort this array of Variable(s) in increasing Variable ORDERING.
 *
 * See the en.wikibook.org heapsort.
 */
void
Variable_Sort(Variable *base, int32_t count);
#endif//$XCC_h

void
Variable_Sort(Variable *base, int32_t count)
{
  int32_t n = count, i = count/2;
  while (true) {
    int32_t parent, child;
    Variable temp;
    if (i > 0) {
      temp = base[--i];
    } else if (n > 1) {
      temp = base[--n];
      base[n] = base[0];
    } else break;
    parent = i, child = i*2 + 1;
    while (child < n) {
      if (   child + 1 < n
          && Variable_GT(base[child + 1], base[child])) {
        child = child + 1;
      }
      if (Variable_GT(base[child], temp)) {
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
 * Pretty print of this Variable.
 */
bool
Variable_pretty(const_Variable this, FILE *file, int flags);
#endif//$XCC_h

#ifndef _NTRACE
bool
Variable_pretty(const_Variable this, FILE *file, int flags)
{
  const_Temporary temporary = Variable____(this);
  const_Temporary origin = Variable_ORIGIN(this);
  if (file == NULL) file = stderr;
  fprintf(file, "V%d", Temporary_IDENTITY(temporary));
  if (origin != NULL) {
    fprintf(file, "(");
    Temporary_pretty(origin, file);
    fprintf(file, ")");
  }
  if (Temporary_hasRegister(temporary)) {
    Register registre = Temporary_REGISTER(temporary);
    fprintf(file, "^%s", Register_names(registre)[0]);
  }
  if (Variable_hasMemory(this)) {
    Symbol symbol = Variable_memory(this);
    fprintf(file, "^[%s]", Symbol_name(symbol));
  }
  
  if (flags) fprintf(file, "@%d", Variable_ORDERING(this));
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Pretty print of this Variable in Minir format.
 */
bool
Variable_prettyMinir(const_Variable this, FILE *file, int flags);
#endif//$XCC_h

#ifndef _NTRACE
bool
Variable_prettyMinir(const_Variable this, FILE *file, int flags)
{
  const_Temporary temporary = Variable____(this);
  const_Temporary origin = Variable_ORIGIN(this);
  if (file == NULL) file = stderr;
  fprintf(file, "V%d", Temporary_IDENTITY(temporary));
  /* TODO: origin, ordering? */
  if (Temporary_hasRegister(temporary)) {
    Register registre = Temporary_REGISTER(temporary);
    fprintf(file, "^%s", Register_names(registre)[0]);
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
bool
Variable_prettyNeighbors(const_Variable this, FILE *file, int flags);
#endif//$XCC_h

#ifndef _NTRACE
bool
Variable_prettyNeighbors(const_Variable this, FILE *file, int flags)
{
  PtrSet neighbors = Variable_NEIGHBORS(this);
  if (file == NULL) file = stderr;
  fprintf(file, "{");
  PtrSet_FORSORT(neighbors, Variable_compare, Variable, variable) {
    fprintf(file, " ");
    if (flags) {
      fprintf(file, "@%d", Variable_ORDERING(variable));
    }
    Variable_pretty(variable, file, 0);
  } PtrSet_ENDSORT;
  fprintf(file, " }");
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
#define Variable_updateCongruence(this) \
  VariableClass_update(Variable_CONGRUENCE(this), this)
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Stack of Variable(s).
 */
struct VariableStack_ {
  //@args	Memory memory, int maxCount
  PtrSeq_ __;		// Underlying PtrSeq.
};
#endif//$XCC_h

#ifdef $XCC_h
static inline PtrSeq
VariableStack__(VariableStack this)
{
  return VariableStack____(this);
}
static inline const_PtrSeq
VariableStack__const(const_VariableStack this)
{
  return VariableStack____(this);
}
#endif//$XCC_h

VariableStack
VariableStack_Ctor(VariableStack this,
                   Memory memory, int maxCount)
{
  PtrSeq_Ctor(VariableStack____(this), memory, maxCount);
  return this;
}

void
VariableStack_Dtor(VariableStack this)
{
  PtrSeq_Dtor(VariableStack____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(VariableStack_)\t%zu\n", sizeof(VariableStack_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Iterate over this VariableStack Variable(s).
 */
#define VariableStack_FOREACH_Variable(this, variable) { \
  const_PtrSeq VariableStack____ = VariableStack__const(this); \
  PtrSeq_FOREACH(VariableStack____, Variable, variable) {
#define VariableStack_ENDEACH_Variable \
  } PtrSeq_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this VariableStack Variable(s).
 */
#define VariableStack_FORBACK_Variable(this, variable) { \
  const_PtrSeq VariableStack____ = VariableStack__const(this); \
  PtrSeq_FORBACK(VariableStack____, Variable, variable) {
#define VariableStack_ENDBACK_Variable \
  } PtrSeq_ENDBACK \
}
#endif//$XCC_h

#ifdef $XCC_h
#define VariableStack_resize(this, maxCount) \
  PtrSeq_resize(VariableStack__(this), maxCount)
#define VariableStack_empty(this) \
  PtrSeq_empty(VariableStack__(this))
#define VariableStack_memory(this) \
  PtrSeq_memory(VariableStack__const(this))
#define VariableStack_base_(this) (Variable *) \
  PtrSeq_base_(VariableStack__(this))
#define VariableStack_base(this) (const Variable *) \
  PtrSeq_base(VariableStack__const(this))
#define VariableStack_past(this) (const Variable *) \
  PtrSeq_past(VariableStack__const(this))
#define VariableStack_isEmpty(this) \
  PtrSeq_isEmpty(VariableStack__const(this))
#define VariableStack_isSingle(this) \
  PtrSeq_isSingle(VariableStack__const(this))
#define VariableStack_isFull(this) \
  PtrSeq_isFull(VariableStack__const(this))
#define VariableStack_count(this) \
  PtrSeq_count(VariableStack__const(this))
#define VariableStack_access(this, index) (Variable) \
  PtrSeq_access(VariableStack__const(this), index)
#define VariableStack_bottom(this) (Variable) \
  PtrSeq_bottom(VariableStack__const(this))
#define VariableStack_top(this) (Variable) \
  PtrSeq_top(VariableStack__const(this))
#define VariableStack_pop(this) \
  PtrSeq_pop(VariableStack__(this))
#define VariableStack_push(this, variable) \
  PtrSeq_push(VariableStack__(this), variable)
#define VariableStack_push2(this, variable) \
  PtrSeq_push2(VariableStack__(this), variable)
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Class of Variable(s) used for congruence classes.
 * Implemented as simple linked lists sorted by Variable_ORDERING, Variable_IDENTITY.
 */
struct VariableClass_ {
  //@args	Variable variable
  Variable FIRST;	// First Variable in this VariableClass.
  Variable LAST;	// Last Variable in this VariableClass.
  int32_t COUNT;	// Count of Variable(s) in this VariableClass.
  Register REGISTER;	// Used for register targeting constraints.
};
#endif//$XCC_h

VariableClass
VariableClass_Ctor(VariableClass this,
                   Variable variable)
{
  if (variable != NULL) {
    Except_CHECK(Variable_CLASSNEXT(variable) == NULL);
    *Variable__CONGRUENCE(variable) = this;
    *VariableClass__FIRST(this) = variable;
    *VariableClass__LAST(this) = variable;
    *VariableClass__COUNT(this) = 1;
  } else {
    *VariableClass__FIRST(this) = NULL;
    *VariableClass__LAST(this) = NULL;
    *VariableClass__COUNT(this) = 0;
  }
  *VariableClass__REGISTER(this) = Register__UNDEF;
  return this;
};

void
VariableClass_Dtor(VariableClass this)
{
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(VariableClass_)\t%zu\n", sizeof(VariableClass_));
}
#endif//$XCC__c

#ifdef $XCC_h
static inline Variable
VariableClass_first(const_VariableClass this)
{
  return VariableClass_FIRST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Variable
VariableClass_last(const_VariableClass this)
{
  return VariableClass_LAST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
#define VariableClass_FOREACH_Variable(this, variable) { \
  Variable VariableClass_NEXT = NULL; \
  Variable variable = VariableClass_first(this); \
  for (; variable != NULL; variable = VariableClass_NEXT) { \
    VariableClass_NEXT = Variable_CLASSNEXT(variable);
#define VariableClass_ENDEACH_Variable \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
VariableClass_count(const_VariableClass this)
{
  return VariableClass_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline bool
VariableClass_isEmpty(const_VariableClass this)
{
  return VariableClass_COUNT(this) == 0;
}
#endif//$XCC_h

#ifdef $XCC_h
bool
VariableClass_contains(const_VariableClass this, const_Variable variable);
#endif//$XCC_h

bool
VariableClass_contains(const_VariableClass this, const_Variable variable)
{
  int32_t ordering = Variable_ORDERING(variable);
  uint32_t identity = Variable_IDENTITY(variable);
  Except_CHECK(Variable_ORDERING(variable) != 0);
  VariableClass_FOREACH_Variable(this, member) {
    if (member == variable) return true;
    if (   (Variable_ORDERING(member) > ordering)
        || (   Variable_ORDERING(member) == ordering
            && Variable_IDENTITY(member) > identity)) return false;
  } VariableClass_ENDEACH_Variable;
  return false;
}

#ifdef $XCC_h
bool
VariableClass_insert(VariableClass this, Variable variable);
#endif//$XCC_h

bool
VariableClass_insert(VariableClass this, Variable variable)
{
  Variable prev_member = NULL;
  int32_t ordering = Variable_ORDERING(variable);
  uint32_t identity = Variable_IDENTITY(variable);
  Except_CHECK(Variable_ORDERING(variable) != 0);
  Except_CHECK(Variable_CLASSNEXT(variable) == NULL);
  VariableClass_FOREACH_Variable(this, member) {
    if (member == variable) return false;
    if (   (Variable_ORDERING(member) > ordering)
        || (   Variable_ORDERING(member) == ordering
            && Variable_IDENTITY(member) > identity)) {
      if (prev_member == NULL) {
        // Insert as first member.
        *VariableClass__FIRST(this) = variable;
        *Variable__CLASSNEXT(variable) = member;
      } else {
        // Insert between prev_member and member.
        *Variable__CLASSNEXT(prev_member) = variable;
        *Variable__CLASSNEXT(variable) = member;
      }
      ++*VariableClass__COUNT(this);
      *Variable__CONGRUENCE(variable) = this;
      Except_DEBUG(VariableClass_check(this));
      return true;
    }
    prev_member = member;
  } VariableClass_ENDEACH_Variable;
  // Insert as last member.
  if (prev_member != NULL) {
    *Variable__CLASSNEXT(prev_member) = variable;
    *VariableClass__LAST(this) = variable;
  } else {
    *VariableClass__FIRST(this) = variable;
    *VariableClass__LAST(this) = variable;
  }
  ++*VariableClass__COUNT(this);
  *Variable__CONGRUENCE(variable) = this;
  Except_DEBUG(VariableClass_check(this));
  return true;
}

#ifdef $XCC_h
bool
VariableClass_remove(VariableClass this, Variable variable);
#endif//$XCC_h

bool
VariableClass_remove(VariableClass this, Variable variable)
{
  Variable prev_member = NULL;
  int32_t ordering = Variable_ORDERING(variable);
  uint32_t identity = Variable_IDENTITY(variable);
  Except_CHECK(Variable_ORDERING(variable) != 0);
  VariableClass_FOREACH_Variable(this, member) {
    if (member == variable) {
      Variable next_member = Variable_CLASSNEXT(member);
      if (prev_member == NULL) {
        // Remove first member.
        *VariableClass__FIRST(this) = next_member;
      } else {
        *Variable__CLASSNEXT(prev_member) = next_member;
      }
      if (next_member == NULL) {
        // Remove last member.
        *VariableClass__LAST(this) = prev_member;
      }
      --*VariableClass__COUNT(this);
      *Variable__CLASSNEXT(variable) = NULL;
      *Variable__CONGRUENCE(variable) = NULL;
      Except_DEBUG(VariableClass_check(this));
      return true;
    }
    if (   (Variable_ORDERING(member) > ordering)
        || (   Variable_ORDERING(member) == ordering
            && Variable_IDENTITY(member) > identity)) return false;
    prev_member = member;
  } VariableClass_ENDEACH_Variable;
  return false;
}

#ifdef $XCC_h
void
VariableClass_merge(VariableClass this, VariableClass that);
#endif//$XCC_h

void
VariableClass_merge(VariableClass this, VariableClass that)
{
  Variable this_first = VariableClass_FIRST(this);
  Variable that_first = VariableClass_FIRST(that);
  Variable this_last = VariableClass_LAST(this);
  Variable that_last = VariableClass_LAST(that);
  int32_t that_count = VariableClass_COUNT(that);
  Except_CHECK(this != that);
  if (that_first == NULL) return;
  if (this_last == NULL) {
    Except_CHECK(this_first == NULL);
    *VariableClass__FIRST(this) = that_first;
    *VariableClass__LAST(this) = that_last;
    *VariableClass__COUNT(this) = that_count;
  } else {
    Variable_ meld_;
    Variable meld = &meld_;
    Variable item1 = this_first;
    Variable item2 = that_first;
    Except_CHECK(item1 != NULL && item2 != NULL);
    do {
      if (Variable_GT(item2, item1)) {
        *Variable__CLASSNEXT(meld) = item1;
        item1 = Variable_CLASSNEXT((meld = item1));
        *Variable__CONGRUENCE(meld) = this;
      } else {
        *Variable__CLASSNEXT(meld) = item2;
        item2 = Variable_CLASSNEXT((meld = item2));
        *Variable__CONGRUENCE(meld) = this;
      }
    } while (item1 != NULL && item2 != NULL);
    if (item1 != NULL) {
      *Variable__CLASSNEXT(meld) = item1;
      do {
        item1 = Variable_CLASSNEXT((meld = item1));
        *Variable__CONGRUENCE(meld) = this;
      } while (item1 != NULL);
    }
    if (item2 != NULL) {
      *Variable__CLASSNEXT(meld) = item2;
      do {
        item2 = Variable_CLASSNEXT((meld = item2));
        *Variable__CONGRUENCE(meld) = this;
      } while (item2 != NULL);
    }
    *VariableClass__FIRST(this) = Variable_CLASSNEXT(&meld_);
    *VariableClass__LAST(this) = meld;
    *VariableClass__COUNT(this) += that_count;
  }
  *VariableClass__FIRST(that) = NULL;
  *VariableClass__LAST(that) = NULL;
  *VariableClass__COUNT(that) = 0;
  if (VariableClass_REGISTER(this) == Register__UNDEF)
    *VariableClass__REGISTER(this) = VariableClass_REGISTER(that);
  else
    Except_CHECK((VariableClass_REGISTER(that) == Register__UNDEF) ||
                 (VariableClass_REGISTER(this) == VariableClass_REGISTER(that)));

  Except_DEBUG(VariableClass_check(this));
}

#ifdef $XCC_h
void
VariableClass_update(VariableClass this, Variable variable);
#endif//$XCC_h

void
VariableClass_update(VariableClass this, Variable variable)
{
  Variable prev_member = NULL;
  Variable member = VariableClass_FIRST(this);
  while (member != NULL) {
    Variable next_member = Variable_CLASSNEXT(member);
    if (member == variable) {
      if (   (prev_member == NULL || Variable_GT(member, prev_member))
          && (next_member == NULL || Variable_GT(next_member, member))) {
        Except_DEBUG(VariableClass_check(this));
        return;
      }
      if (prev_member == NULL) {
        // Remove first member.
        *VariableClass__FIRST(this) = next_member;
      } else {
        *Variable__CLASSNEXT(prev_member) = next_member;
      }
      if (next_member == NULL) {
        // Remove last member.
        *VariableClass__LAST(this) = prev_member;
      }
      --*VariableClass__COUNT(this);
      *Variable__CLASSNEXT(variable) = NULL;
      *Variable__CONGRUENCE(variable) = NULL;
      Except_DEBUG(VariableClass_check(this));
      break;
    }
    prev_member = member;
    member = next_member;
  }
  Except_CHECK(member != NULL);
  VariableClass_insert(this, variable);
}

#ifdef $XCC_h
bool
VariableClass_hasStrongInterference(VariableClass congruence0, VariableClass congruence1);
#endif//$XCC_h

bool
VariableClass_hasStrongInterference(VariableClass congruence0, VariableClass congruence1) {
  //if ((congruence0 == NULL) || (congruence1 == NULL)) return false;
  if (   (congruence0 != congruence1)
      && (VariableClass_REGISTER(congruence0) != Register__UNDEF)
      && (VariableClass_REGISTER(congruence1) != Register__UNDEF)) {
    Except_CHECK(VariableClass_REGISTER(congruence0) != VariableClass_REGISTER(congruence1));
    return true;
  }
  return false;
}

#ifdef $XCC_h
bool
VariableClass_check(const_VariableClass this);
#endif//$XCC_h

bool
VariableClass_check(const_VariableClass this)
{
  int32_t count = 0;
  Variable prev_member = NULL;
  Variable member = VariableClass_FIRST(this);
  while (member != NULL) {
    Variable next_member = Variable_CLASSNEXT(member);
    if (prev_member != NULL) {
      Except_ALWAYS(Variable_GT(member, prev_member));
    }
    Except_ALWAYS(Variable_CONGRUENCE(member) == this);
    Except_CHECK(Variable_ORDERING(member) != 0);
    prev_member = member;
    member = next_member;
    count++;
  }
  Except_ALWAYS(prev_member == VariableClass_LAST(this));
  Except_ALWAYS(count == VariableClass_COUNT(this));
  return true;
}

#ifdef $XCC_h
bool
VariableClass_pretty(const_VariableClass this, FILE *file, bool flags);
#endif//$XCC_h

#ifndef _NTRACE
bool
VariableClass_pretty(const_VariableClass this, FILE *file, bool flags)
{
  if (file == NULL) file = stderr;
  fprintf(file, "{");
  VariableClass_FOREACH_Variable(this, variable) {
    fprintf(file, " ");
    if (flags) {
      fprintf(file, "@%d", Variable_ORDERING(variable));
    }
    Variable_pretty(variable, file, 0);
  } VariableClass_ENDEACH_Variable;
  fprintf(file, " }");
  if (VariableClass_REGISTER(this) != Register__UNDEF) {
    fprintf(file, "^%s", Register_names(VariableClass_REGISTER(this))[0]);
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
bool
Variable_dumpXML(Variable this, FILE *file, int indent, XMLGen gen);
#endif//$XCC_h

#ifndef _NTRACE
bool
Variable_dumpXML(Variable this, FILE *file, int indent, XMLGen gen)
{
  char attval[64]="";
  const_Temporary temporary = Variable____(this);
  const_Temporary origin = Variable_ORIGIN(this);
  XMLGenElement elemVar = XMLGen_declareElement(gen, "Variable");
  XMLGenAttribute attID = XMLGen_declareAttribute(gen, "ID");
  XMLGenAttribute attOrigin = XMLGen_declareAttribute(gen, "Origin");
  XMLGen_startElement(gen, elemVar);
  sprintf(attval, "%d", Temporary_IDENTITY(temporary));
  XMLGen_addAttribute(gen, attID, attval);
  if (origin != NULL) {
    sprintf(attval, "Temporary_%d", Temporary_IDENTITY(temporary));
    XMLGen_addAttribute(gen, attOrigin, attval);
  }
  XMLGen_endElement(gen);
  return true;

}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Variable module.
 */
#define Variable_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Variable module.
 */
#define Variable_FINI()
#endif//$XCC__h

#if XCC__C
static void
Variable_TEST(void)
{
#include "LIR/Variable_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Variable_TEST);
  return 0;
}
#endif

