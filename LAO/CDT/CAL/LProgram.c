#ifdef $XCC_h
/*
 * !!	LProgram.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Florent Blachot (Florent.Blachot@st.com)
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
 * @brief Linear Program wrapper for CPLEX and GLPK.
 */
#endif//$XCC_h


#include "CAL/CAL_.h"

#ifdef $XCC__h
#include "CAL/LProgram.h"
#ifdef CPLEX_ENABLED
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <ilcplex/cplex.h>
#else//!CPLEX_ENABLED
#ifdef GLPK_ENABLED
#include "glpk.h"
#endif//GLPK_ENABLED
#endif//CPLEX_ENABLED
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include <float.h>
typedef double LProgramValue;
#ifndef LProgramValue_MIN
#define LProgramValue_MIN	(-DBL_MAX)
#endif//LProgramValue_MIN
#ifndef LProgramValue_MAX
#define LProgramValue_MAX	(DBL_MAX)
#endif//LProgramValue_MAX
#endif//$XCC_h

#ifdef $XCC_h
typedef enum {
  LProgramVariableType_Continuous,
  LProgramVariableType_Integer,
  LProgramVariableType_Binary,
  LProgramVariableType__
} LProgramVariableType;
typedef int8_t short_LProgramVariableType;
#endif//$XCC_h

#ifdef $XCC_h
typedef enum {
  LProgramVariableFlag_Lower = 0x1,
  LProgramVariableFlag_Upper = 0x2,
  LProgramVariableFlag_Value = 0x4,
} LProgramVariableFlag;
typedef unsigned LProgramVariableFlags;
typedef uint8_t short_LProgramVariableFlags;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Variable of a LProgram.
 */
struct LProgramVariable_ {
  //@args	LProgramVariableType type, const char *name
  short_LProgramVariableType TYPE;	// This LProgramVariable type.
  short_LProgramVariableFlags FLAGS;	// This LProgramVariable flags.
  //@access hasLower	((LProgramVariable_FLAGS(this) & LProgramVariableFlag_Lower) != 0)
  //@access hasUpper	((LProgramVariable_FLAGS(this) & LProgramVariableFlag_Upper) != 0)
  //@access hasValue	((LProgramVariable_FLAGS(this) & LProgramVariableFlag_Value) != 0)
  const char *NAME;			// User name for this LProgramVariable.
  LProgramValue LOWER;			// Lower bound of this LProgramVariable.
  LProgramValue UPPER;			// Upper bound of this LProgramVariable.
  LProgramValue VALUE;			// Value of this LProgramVariable.
  LProgramValue WEIGHT;			// Weight of this LProgramVariable in objective.
  int32_t INDEX;
};
#endif//$XCC_h

LProgramVariable
LProgramVariable_Ctor(LProgramVariable this,
                      LProgramVariableType type, const char *name)
{
  *LProgramVariable__TYPE(this) = type;
  *LProgramVariable__FLAGS(this) = 0;
  *LProgramVariable__NAME(this) = name;
  *LProgramVariable__LOWER(this) = (type==LProgramVariableType_Binary)?
                                 (LProgramValue)0: LProgramValue_MIN;
  *LProgramVariable__UPPER(this) = (type==LProgramVariableType_Binary)?
                                 (LProgramValue)1: LProgramValue_MAX;
  *LProgramVariable__VALUE(this) = (LProgramValue)0;
  *LProgramVariable__WEIGHT(this) = (LProgramValue)0;
  *LProgramVariable__INDEX(this) = 0;
  return this;
}

#define LProgramVariable_Dtor 0

size_t
LProgramVariable_Size(LProgramVariableType type, const char *name)
{
  return sizeof(LProgramVariable_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LProgramVariable_)\t%zu\n", sizeof(LProgramVariable_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This LProgramVariable name.
 */
static inline const char *
LProgramVariable_name(const_LProgramVariable this)
{
  return LProgramVariable_NAME(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline LProgramVariableType
LProgramVariable_type(const_LProgramVariable this)
{
  return LProgramVariable_TYPE(this);
}
#endif//$XCC_h

static inline char
LProgramVariable_typeCPLEX(const_LProgramVariable this)
{
  LProgramVariableType type = LProgramVariable_TYPE(this);
#ifdef CPLEX_ENABLED
  if (type == LProgramVariableType_Continuous) return CPX_CONTINUOUS;
  if (type == LProgramVariableType_Integer) return CPX_INTEGER;
  if (type == LProgramVariableType_Binary) return CPX_BINARY;
  Except_NEVER(true);
#endif//CPLEX_ENABLED
  return ' ';
}

static inline int
LProgramVariable_typeGLPK(const_LProgramVariable this)
{
  LProgramVariableType type = LProgramVariable_TYPE(this);
#ifdef GLPK_ENABLED
  if (type == LProgramVariableType_Continuous) return GLP_CV;
  if (type == LProgramVariableType_Integer) return GLP_IV;
  if (type == LProgramVariableType_Binary) return GLP_BV;
  Except_NEVER(true);
#endif//GLPK_ENABLED
  return -1;
}

#ifdef $XCC_h
/**
 * lower (resp. upper) bound set to 0 (resp. 1) if type is LProgramVariableType_Binary
 */
static inline void
LProgramVariable_setType(LProgramVariable this, LProgramVariableType type)
{
  *LProgramVariable__TYPE(this) = type;
  if (type == LProgramVariableType_Binary) {
    *LProgramVariable__LOWER(this) = (LProgramValue)0;
    *LProgramVariable__UPPER(this) = (LProgramValue)1;
  }
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * LProgramVariable_setFlags --
 */
static inline void
LProgramVariable_setFlags(LProgramVariable this, LProgramVariableFlags flags)
{
  *LProgramVariable__FLAGS(this) |= flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This LProgramVariable lower bound.
 */
static inline LProgramValue
LProgramVariable_lower(const_LProgramVariable this)
{
  return LProgramVariable_LOWER(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this LProgramVariable lower bound.
 */
static inline void
LProgramVariable_setLower(LProgramVariable this, LProgramValue lower)
{
  *LProgramVariable__FLAGS(this) |= LProgramVariableFlag_Lower;
  *LProgramVariable__LOWER(this) = lower;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This LProgramVariable upper bound.
 */
static inline LProgramValue
LProgramVariable_upper(const_LProgramVariable this)
{
  return LProgramVariable_UPPER(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this LProgramVariable upper bound.
 */
static inline void
LProgramVariable_setUpper(LProgramVariable this, LProgramValue upper)
{
  *LProgramVariable__FLAGS(this) |= LProgramVariableFlag_Upper;
  *LProgramVariable__UPPER(this) = upper;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This LProgramVariable value.
 */
static inline LProgramValue
LProgramVariable_value(const_LProgramVariable this)
{
  return LProgramVariable_VALUE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this LProgramVariable value.
 */
static inline void
LProgramVariable_setValue(LProgramVariable this, LProgramValue value)
{
  *LProgramVariable__FLAGS(this) |= LProgramVariableFlag_Value;
  *LProgramVariable__VALUE(this) = value;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This LProgramVariable weight.
 */
static inline LProgramValue
LProgramVariable_weight(const_LProgramVariable this)
{
  return LProgramVariable_WEIGHT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this LProgramVariable weight.
 */
static inline void
LProgramVariable_setWeight(LProgramVariable this, LProgramValue weight)
{
  *LProgramVariable__WEIGHT(this) = weight;
}
#endif//$XCC_h

#ifdef $XCC_h
bool
LProgramVariable_pretty(const_LProgramVariable this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
LProgramVariable_pretty(const_LProgramVariable this, FILE *file)
{
  if (file == NULL) file = stderr;
  switch (LProgramVariable_TYPE(this)) {
  case LProgramVariableType_Continuous:
    fprintf(file, "  Continuous");
    if (LProgramVariable_hasLower(this)) {
      fprintf(file, "\t%g <=", LProgramVariable_LOWER(this));
    }
    fprintf(file, "\t%s", LProgramVariable_NAME(this));
    if (LProgramVariable_hasUpper(this)) {
      fprintf(file, "\t<= %g", LProgramVariable_UPPER(this));
    }
    break;
  case LProgramVariableType_Integer:
    fprintf(file, "  Integer");
    if (LProgramVariable_hasLower(this)) {
      fprintf(file, "\t%g <=", LProgramVariable_LOWER(this));
    }
    fprintf(file, "\t%s", LProgramVariable_NAME(this));
    if (LProgramVariable_hasUpper(this)) {
      fprintf(file, "\t<= %g", LProgramVariable_UPPER(this));
    }
    break;
  case LProgramVariableType_Binary:
    fprintf(file, "  Binary \t%s", LProgramVariable_NAME(this));
    break;
  default:
    fprintf(file, "?? %s", LProgramVariable_NAME(this));
    break;
  }
  if (LProgramVariable_hasValue(this)) {
    fprintf(file, "\tValue\t%g", LProgramVariable_VALUE(this));
  }
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Term  of a LProgram.
 */
struct LProgramTerm_ {
  //@args	LProgramVariable variable, LProgramValue factor
  LProgramVariable VARIABLE;
  LProgramValue FACTOR;
};
#endif//$XCC_h

LProgramTerm
LProgramTerm_Ctor(LProgramTerm this,
                  LProgramVariable variable, LProgramValue factor)
{
  *LProgramTerm__VARIABLE(this) = variable;
  *LProgramTerm__FACTOR(this) = factor;
  return this;
}

size_t
LProgramTerm_Size(LProgramVariable variable, LProgramValue factor)
{
  return sizeof(LProgramTerm_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LProgramTerm_)\t%zu\n", sizeof(LProgramTerm_));
}
#endif//$XCC__c

#ifdef $XCC_h
/*
 * LProgramTerm_variable
 */
static inline LProgramVariable
LProgramTerm_variable(LProgramTerm this)
{
  return LProgramTerm_VARIABLE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * LProgramTerm_factor
 */
static inline LProgramValue
LProgramTerm_factor(LProgramTerm this)
{
  return LProgramTerm_FACTOR(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef enum {
  LProgramConstraintType_LE,
  LProgramConstraintType_GE,
  LProgramConstraintType_EQ,
  LProgramConstraintType__
} LProgramConstraintType;
typedef int8_t short_LProgramConstraintType;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Constraint of a LProgram.
 */
struct LProgramConstraint_;
#endif//$XCC_h

#ifdef $XCC__h
struct LProgramConstraint_ {
  //@args	Memory memory, int32_t count, LProgramConstraintType type,
  //@args	LProgramValue bound, const char *name
  IStack_ TERMS_;	// The LProgramTerms of this LProgramConstraint.
  //@access TERMS	LProgramConstraint__TERMS_(this)
  //@access TERMCOUNT	(IStack_usedSize(LProgramConstraint_TERMS(this))/sizeof(LProgramTerm_))
  short_LProgramConstraintType TYPE;	// The type of this LProgramConstraint.
  //@access isLE	(LProgramConstraint_TYPE(this) == LProgramConstraintType_LE)
  //@access isGE	(LProgramConstraint_TYPE(this) == LProgramConstraintType_GE)
  //@access isEQ	(LProgramConstraint_TYPE(this) == LProgramConstraintType_EQ)
  LProgramValue BOUND;	// The bound (RHS) of this LProgramConstraint.
  const char *NAME;	// User name for this LProgramConstraint.
};
#endif//$XCC__h

LProgramConstraint
LProgramConstraint_Ctor(LProgramConstraint this,
                        Memory memory, int32_t count, LProgramConstraintType type,
                        LProgramValue bound, const char *name)
{
  IStack_Ctor(LProgramConstraint_TERMS(this),
              memory, sizeof(LProgramTerm_), count);
  *LProgramConstraint__TYPE(this) = type;
  *LProgramConstraint__BOUND(this) = bound;
  *LProgramConstraint__NAME(this) = name;
  return this;
}

void
LProgramConstraint_Dtor(LProgramConstraint this)
{
  IStack_Dtor(LProgramConstraint_TERMS(this));
}

size_t
LProgramConstraint_Size(Memory memory, int32_t count, LProgramConstraintType type,
                        LProgramValue bound, const char *name)
{
        return sizeof(LProgramConstraint_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LProgramConstraint_)\t%zu\n", sizeof(LProgramConstraint_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * For use by LProgramConstraint_FOREACH_LProgramTerm.
 */
const_IStack
LProgramConstraint_terms(const_LProgramConstraint this);
#endif//$XCC_h

const_IStack
LProgramConstraint_terms(const_LProgramConstraint this)
{
  return LProgramConstraint_TERMS(this);
}

#ifdef $XCC_h
int32_t
LProgramConstraint_termCount(const_LProgramConstraint this);
#endif//$XCC_h

int32_t
LProgramConstraint_termCount(const_LProgramConstraint this)
{
  return LProgramConstraint_TERMCOUNT(this);
}

#ifdef $XCC_h
LProgramValue
LProgramConstraint_bound(const_LProgramConstraint this);
#endif//$XCC_h

LProgramValue
LProgramConstraint_bound(const_LProgramConstraint this)
{
  return LProgramConstraint_BOUND(this);
}

#ifdef $XCC_h
LProgramConstraintType
LProgramConstraint_type(const_LProgramConstraint this);
#endif//$XCC_h

LProgramConstraintType
LProgramConstraint_type(const_LProgramConstraint this)
{
  return LProgramConstraint_TYPE(this);
}

static inline char
LProgramConstraint_typeCPLEX(const_LProgramConstraint this)
{
  LProgramConstraintType type = LProgramConstraint_TYPE(this);
#ifdef CPLEX_ENABLED
  if (type == LProgramConstraintType_LE) return 'L';
  if (type == LProgramConstraintType_GE) return 'G';
  if (type == LProgramConstraintType_EQ) return 'E';
  Except_NEVER(true);
#endif//CPLEX_ENABLED
  return ' ';
}

#ifdef $XCC_h
/**
 * Iterate over this LProgramConstraint LProgramTerms.
 */
#define LProgramConstraint_FOREACH_LProgramTerm(this, term) \
  IStack_FOREACH(LProgramConstraint_terms(this), LProgramTerm_, term)
#define LProgramConstraint_ENDEACH_LProgramTerm \
  IStack_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
bool
LProgramConstraint_pretty(const_LProgramConstraint this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
LProgramConstraint_pretty(const_LProgramConstraint this, FILE *file)
{
  if (file == NULL) file = stderr;
  if (LProgramConstraint_NAME(this) != NULL) {
    fprintf(file, " %s:", LProgramConstraint_NAME(this));
  }
  LProgramConstraint_FOREACH_LProgramTerm(this, term) {
    LProgramVariable variable = LProgramTerm_VARIABLE(term);
    fprintf(file, "\t%+g %s",
            LProgramTerm_FACTOR(term),
            LProgramVariable_NAME(variable));
  } LProgramConstraint_ENDEACH_LProgramTerm;
  switch (LProgramConstraint_TYPE(this)) {
  case LProgramConstraintType_LE:
    fprintf(file, "\t<=\t"); break;
  case LProgramConstraintType_GE:
    fprintf(file, "\t>=\t"); break;
  case LProgramConstraintType_EQ:
    fprintf(file, "\t=\t"); break;
  default:
    fprintf(file, "\t??\t"); break;
  }
  fprintf(file, "%g\n", LProgramConstraint_BOUND(this));
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * This LProgramConstraint name.
 */
const char *
LProgramConstraint_name(const_LProgramConstraint this);
#endif//$XCC_h

const char *
LProgramConstraint_name(const_LProgramConstraint this)
{
  return LProgramConstraint_NAME(this);
}

#ifdef $XCC_h
/**
 * Set this LProgramConstraint bound.
 */
void
LProgramConstraint_setBound(LProgramConstraint this, LProgramValue bound);
#endif//$XCC_h

void
LProgramConstraint_setBound(LProgramConstraint this, LProgramValue bound)
{
  *LProgramConstraint__BOUND(this) = bound;
}

#ifdef $XCC_h
/**
 * Access a LProgramTerm of this LProgramConstraint from a LProgramVariable.
 */
LProgramTerm
LProgramConstraint_accessTerm(const_LProgramConstraint this,
                              const_LProgramVariable variable);
#endif//$XCC_h

LProgramTerm
LProgramConstraint_accessTerm(const_LProgramConstraint this,
                              const_LProgramVariable variable)
{
  LProgramConstraint_FOREACH_LProgramTerm(this, term) {
    if (LProgramTerm_VARIABLE(term) == variable) return term;
  } LProgramConstraint_ENDEACH_LProgramTerm;
  return NULL;
}

#ifdef $XCC_h
/**
 * Add a term to this LProgramConstraint.
 */
void
LProgramConstraint_addTerm(LProgramConstraint this,
                           LProgramVariable variable,
                           LProgramValue factor);
#endif//$XCC_h

void
LProgramConstraint_addTerm(LProgramConstraint this,
                           LProgramVariable variable,
                           LProgramValue factor)
{
  LProgramTerm term = LProgramConstraint_accessTerm(this, variable);
  if (term == NULL) {
    term = (LProgramTerm)IStack_push2(LProgramConstraint_TERMS(this));
    LProgramTerm_Ctor(term, variable, factor);
  } else {
    *LProgramTerm__FACTOR(term) += factor;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef enum {
  LProgramType_Feasibility,
  LProgramType_Minimize,
  LProgramType_Maximize,
  LProgramType__
} LProgramType;
typedef int8_t short_LProgramType;
#endif//$XCC_h

#ifdef $XCC_h
bool
LProgramType_pretty(LProgramType this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
LProgramType_pretty(LProgramType this, FILE *file)
{
  if (file == NULL) file = stderr;
  switch (this) {
  case LProgramType_Feasibility: fprintf(file, "Feasibility"); break;
  case LProgramType_Minimize: fprintf(file, "Minimize"); break;
  case LProgramType_Maximize: fprintf(file, "Maximize"); break;
  default: fprintf(file, "Type?");  
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
typedef enum {
  LProgramStatus_Initial,
  LProgramStatus_Solved,
  LProgramStatus_Feasible,
  LProgramStatus_Infeasible,
  LProgramStatus_Unbounded,
  LProgramStatus_Undefined,
  LProgramStatus_TimedOut,
  LProgramStatus_Error,
  LProgramStatus__
} LProgramStatus;
typedef int8_t short_LProgramStatus;
#endif//$XCC_h

#ifdef $XCC_h
bool
LProgramStatus_pretty(LProgramStatus this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
LProgramStatus_pretty(LProgramStatus this, FILE *file)
{
  if (file == NULL) file = stderr;
  switch (this) {
  case LProgramStatus_Initial: fprintf(file, "Initial"); break;
  case LProgramStatus_Solved: fprintf(file, "Solved"); break;
  case LProgramStatus_Feasible: fprintf(file, "Feasible"); break;
  case LProgramStatus_Infeasible: fprintf(file, "Infeasible"); break;
  case LProgramStatus_Unbounded: fprintf(file, "Unbounded"); break;
  case LProgramStatus_Undefined: fprintf(file, "Undefined"); break;
  case LProgramStatus_TimedOut: fprintf(file, "TimedOut"); break;
  case LProgramStatus_Error: fprintf(file, "Error"); break;
  default: fprintf(file, "Status?");  
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * LProgram representation.
 */
struct LProgram_;
#endif//$XCC_h

#ifdef $XCC__h
struct LProgram_ {
  //@args	Memory memory, LProgramType type, double timeOut
  HTable_ VARIABLEMAP_;
  //@access VARIABLEMAP	LProgram__VARIABLEMAP_(this)
  //@access MEMORY	HTable_memory(LProgram_VARIABLEMAP(this))
  IDList_ VARIABLES_;
  //@access VARIABLES	LProgram__VARIABLES_(this)
  //@access VARIABLECOUNT	IDList_count(LProgram_VARIABLES(this))
  IDList_ CONSTRAINTS_;
  //@access CONSTRAINTS	LProgram__CONSTRAINTS_(this)
  //@access CONSTRAINTCOUNT	IDList_count(LProgram_CONSTRAINTS(this))
  double TIMEOUT;
  double TIMESPENT;
  LProgramValue SOLUTION;
  short_LProgramType TYPE;
  //@access isFeasibility	(LProgram_TYPE(this) == LProgramType_Feasibility)
  //@access isMinimize	(LProgram_TYPE(this) == LProgramType_Minimize)
  //@access isMaximize	(LProgram_TYPE(this) == LProgramType_Maximize)
  short_LProgramStatus STATUS;
#ifdef CPLEX_ENABLED
  struct cpxenv *CPXENV;
  struct cpxlp *CPXLP;
#else//!CPLEX_ENABLED
#ifdef GLPK_ENABLED
  glp_prob *GLP;
#endif//GLPK_ENABLED
#endif//CPLEX_ENABLED
};
#endif//$XCC__h

LProgram
LProgram_Ctor(LProgram this,
              Memory memory, LProgramType type, double timeOut)
{
  HTable_Ctor(LProgram_VARIABLEMAP(this), memory, 512);
  IDList_Ctor(LProgram_VARIABLES(this), memory);
  IDList_Ctor(LProgram_CONSTRAINTS(this), memory);
  *LProgram__TIMEOUT(this) = timeOut;
  *LProgram__TIMESPENT(this) = 0.0;
  *LProgram__SOLUTION(this) = 0.0;
  *LProgram__TYPE(this) = type;
  *LProgram__STATUS(this) = LProgramStatus_Initial;
#ifdef CPLEX_ENABLED
  *LProgram__CPXENV(this) = NULL;
  *LProgram__CPXLP(this) = NULL;
#else//!CPLEX_ENABLED
#ifdef GLPK_ENABLED
  *LProgram__GLP(this) = NULL;
#endif//GLPK_ENABLED
#endif//CPLEX_ENABLED
  return this;
}

void
LProgram_Dtor(LProgram this)
{
  HTable_Dtor(LProgram_VARIABLEMAP(this));
  IDList_empty(LProgram_VARIABLES(this),
               (IDListItemRelease)LProgramVariable_Dtor);
  IDList_Dtor(LProgram_VARIABLES(this));
  IDList_empty(LProgram_CONSTRAINTS(this),
               (void (*)(IDListItem))LProgramConstraint_Dtor);
  IDList_Dtor(LProgram_CONSTRAINTS(this));
}

size_t
LProgram_Size(Memory memory, LProgramType type, double timeOut)
{
  return sizeof(LProgram_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LProgram_)\t%zu\n", sizeof(LProgram_));
}
#endif//$XCC__c

#ifdef $XCC_h
LProgram
LProgram_new(Memory parent, LProgramType type, double timeOut);
#endif//$XCC_h

LProgram
LProgram_new(Memory parent, LProgramType type, double timeOut)
{
  Memory memory = Memory_new(parent, true);
  size_t size = LProgram_Size(memory, type, timeOut);
  LProgram this = Memory_alloc(memory, size);
  return LProgram_Ctor(this, memory, type, timeOut);
}

#ifdef $XCC_h
LProgram
LProgram_delete(LProgram this);
#endif//$XCC_h

LProgram
LProgram_delete(LProgram this)
{
  if (this != NULL) {
    Memory memory = LProgram_MEMORY(this);
#ifndef _NDTOR
    LProgram_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/*
 * LProgram_type
 */
LProgramType
LProgram_type(const_LProgram this);
#endif//$XCC_h

LProgramType
LProgram_type(const_LProgram this)
{
  return LProgram_TYPE(this);
}

#ifdef $XCC_h
LProgramStatus
LProgram_status(const_LProgram this);
#endif//$XCC_h

LProgramStatus
LProgram_status(const_LProgram this)
{
  return LProgram_STATUS(this);
}

#ifdef $XCC_h
double
LProgram_timeOut(const_LProgram this);
#endif//$XCC_h

double
LProgram_timeOut(const_LProgram this)
{
  return LProgram_TIMEOUT(this);
}

#ifdef $XCC_h
void
LProgram_setTimeLimit(LProgram this, double time);
#endif//$XCC_h

void
LProgram_setTimeLimit(LProgram this, double time)
{
  *LProgram__TIMEOUT(this) = time;
}

#ifdef $XCC_h
double
LProgram_timeSpent(const_LProgram this);
#endif//$XCC_h

double
LProgram_timeSpent(const_LProgram this)
{
  return LProgram_TIMESPENT(this);
}

#ifdef $XCC_h
void
LProgram_setTimeSpent(LProgram this, double time);
#endif//$XCC_h

void
LProgram_setTimeSpent(LProgram this, double time)
{
  *LProgram__TIMESPENT(this) = time;
}

#ifdef $XCC_h
/**
 * For use by LProgram_FOREACH_LProgramVariable.
 */
const_IDList
LProgram_variables(const_LProgram this);
#endif//$XCC_h

const_IDList
LProgram_variables(const_LProgram this)
{
  return LProgram_VARIABLES(this);
}

#ifdef $XCC_h
/**
 * Number of variables in a LProgram
 */
int32_t
LProgram_variableCount(const_LProgram this);
#endif//$XCC_h

int32_t
LProgram_variableCount(const_LProgram this)
{
  return LProgram_VARIABLECOUNT(this);
}

#ifdef $XCC_h
/**
 * Iterate over this LProgram LProgramVariables.
 */
#define LProgram_FOREACH_LProgramVariable(this, variable) \
  IDList_FOREACH(LProgram_variables(this), LProgramVariable_, variable)
#define LProgram_ENDEACH_LProgramVariable \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by LProgram_FOREACH_LProgramConstraint.
 */
const_IDList
LProgram_constraints(const_LProgram this);
#endif//$XCC_h

const_IDList
LProgram_constraints(const_LProgram this)
{
  return LProgram_CONSTRAINTS(this);
}

#ifdef $XCC_h
int32_t
LProgram_constraintCount(const_LProgram this);
#endif//$XCC_h

int32_t
LProgram_constraintCount(const_LProgram this)
{
  return LProgram_CONSTRAINTCOUNT(this);
}

#ifdef $XCC_h
/**
 * Iterate over this LProgram LProgramConstraints.
 */
#define LProgram_FOREACH_LProgramConstraint(this, constraint) \
  IDList_FOREACH(LProgram_constraints(this), LProgramConstraint_, constraint)
#define LProgram_ENDEACH_LProgramConstraint \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * */
LProgramValue
LProgram_solution(const_LProgram this);
#endif//$XCC_h

LProgramValue
LProgram_solution(const_LProgram this)
{
  return LProgram_SOLUTION(this);
}

#ifdef $XCC_h
bool
LProgram_prettyObjective(const_LProgram this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
LProgram_prettyObjective(const_LProgram this, FILE *file)
{
  LProgramType type = LProgram_TYPE(this);
  if (type == LProgramType_Feasibility) fprintf(file, "Feasibility problem\n");
  else {
    LProgramType_pretty(type, file);
    fprintf(file, "\n");
    LProgram_FOREACH_LProgramVariable(this, variable) {
      fprintf(file, "\t%+g %s",
              LProgramVariable_WEIGHT(variable),
              LProgramVariable_NAME(variable));
    } LProgram_ENDEACH_LProgramVariable;
    fprintf(file, "\n");
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
bool
LProgram_pretty(const_LProgram this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
LProgram_pretty(const_LProgram this, FILE *file)
{
  LProgramStatus status = LProgram_STATUS(this);
  if (file == NULL) file = stderr;
  fprintf(file, "\\ Status: ");
  LProgramStatus_pretty(status, file);
  fprintf(file, "\n");
  LProgram_prettyObjective(this, file);
  fprintf(file, "Subject To\n");
  LProgram_FOREACH_LProgramConstraint(this, constraint) {
    LProgramConstraint_pretty(constraint, file);
  } LProgram_ENDEACH_LProgramConstraint;
  fprintf(file, "Bounds\n");
  LProgram_FOREACH_LProgramVariable(this, variable) {
    LProgramVariable_pretty(variable, file);
  } LProgram_ENDEACH_LProgramVariable;
  fprintf(file, "End\n\n");
  fflush(file);
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Make a new LProgramVariable for this LProgram.
 */
LProgramVariable
LProgram_makeVariable(LProgram this, LProgramVariableType type, const char *name);
#define LProgram_makeContinuousVariable(this, name) \
  LProgram_makeVariable(this, LProgramVariableType_Continuous, name)
#define LProgram_makeIntegerVariable(this, name) \
  LProgram_makeVariable(this, LProgramVariableType_Integer, name)
#define LProgram_makeBinaryVariable(this, name) \
  LProgram_makeVariable(this, LProgramVariableType_Binary, name)
#endif//$XCC_h

LProgramVariable
LProgram_makeVariable(LProgram this, LProgramVariableType type, const char *name)
{
  size_t size = LProgramVariable_Size(type, name);
  LProgramVariable variable = IDList_push(LProgram_VARIABLES(this), size);
  return LProgramVariable_Ctor(variable, type, name);
}

#ifdef $XCC_h
/**
 * Make a new LProgramConstraint for this LProgram.
 */
LProgramConstraint
LProgram_makeConstraint(LProgram this, LProgramConstraintType type,
                             LProgramValue bound, const char *name);
#define LProgram_makeLEConstraint(this, bound, name) \
  LProgram_makeConstraint(this, LProgramConstraintType_LE, bound, name)
#define LProgram_makeGEConstraint(this, bound, name) \
  LProgram_makeConstraint(this, LProgramConstraintType_GE, bound, name)
#define LProgram_makeEQConstraint(this, bound, name) \
  LProgram_makeConstraint(this, LProgramConstraintType_EQ, bound, name)
#endif//$XCC_h

LProgramConstraint
LProgram_makeConstraint(LProgram this, LProgramConstraintType type,
                             LProgramValue bound, const char *name)
{
  int32_t count = 128;
  Memory memory = LProgram_MEMORY(this);
  size_t size = LProgramConstraint_Size(memory, count, type, bound, name);
  LProgramConstraint constraint = IDList_push(LProgram_CONSTRAINTS(this), size);
  return LProgramConstraint_Ctor(constraint, memory, count, type, bound, name);
}

#ifdef $XCC_h
/**
 * Kill a LProgramConstraint of this LProgram.
 */
void
LProgram_killConstraint(LProgram this, LProgramConstraint constraint);
#endif//$XCC_h

void
LProgram_killConstraint(LProgram this, LProgramConstraint constraint)
{
#ifndef _NDTOR
  IDList_remove(LProgram_CONSTRAINTS(this), constraint,
                (void (*)(IDListItem))LProgramConstraint_Dtor);
#else//!_NDTOR
  IDList_remove_(LProgram_CONSTRAINTS(this), constraint);
#endif//_NDTOR
}

#ifdef $XCC_h
/**
 * Enter a LProgramVariable in this LProgram for keyed access.
 */
LProgramVariable
LProgram_enterVariable(LProgram this, LProgramVariable variable, const void *key);
#endif//$XCC_h

LProgramVariable
LProgram_enterVariable(LProgram this, LProgramVariable variable, const void *key)
{
  HTable variableMap = LProgram_VARIABLEMAP(this);
  LProgramVariable *_variable = HTable_insert(variableMap, key,
                                              sizeof(LProgramVariable));
  if (_variable != NULL) {
    return *_variable = variable;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Access a LProgramVariable in this LProgram from its key.
 */
LProgramVariable
LProgram_accessVariable(LProgram this, const void *key);
#endif//$XCC_h

LProgramVariable
LProgram_accessVariable(LProgram this, const void *key)
{
  HTable variableMap = LProgram_VARIABLEMAP(this);
  LProgramVariable *_variable = HTable_search(variableMap, key);
  if (_variable != NULL) {
    return *_variable;
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
void
LProgram_open(LProgram this, const char *name);
#endif//$XCC_h

void
LProgram_open(LProgram this, const char *name)
{
#ifdef CPLEX_ENABLED
  int cpx_error = 0;
  struct cpxenv *cpxenv = CPXopenCPLEX(&cpx_error);
  if (cpxenv != NULL) {
    struct cpxlp *cpxlp = CPXcreateprob(cpxenv, &cpx_error, name);
    if (cpxlp != NULL) {
      *LProgram__CPXENV(this) = cpxenv;
      *LProgram__CPXLP(this) = cpxlp;
    } else {
      fprintf(stderr, "CPXcreateprob failed: error %d.\n", cpx_error);
    }
  } else {
    char errorstring[CPX_STR_PARAM_MAX];
    CPXgeterrorstring(cpxenv, cpx_error, errorstring);
    fprintf(stderr, "CPXopenCPLEX failed: %s\n", errorstring);
  }
#else//!CPLEX_ENABLED
#ifdef GLPK_ENABLED
  glp_prob *glp = glp_create_prob(); 
  *LProgram__GLP(this) = glp;
  glp_set_prob_name(glp, name);
  if (!CAL_TRACE) {
    //int msg_lev = 0;
    //msg_lev = GLP_MSG_OFF;
    //msg_lev = GLP_MSG_ERR;
    //msg_lev = GLP_MSG_ON;
    //msg_lev = GLP_MSG_ALL;
    //lpx_set_int_parm(glp, LPX_K_MSGLEV, msg_lev);
  }
#endif//GLPK_ENABLED
#endif//CPLEX_ENABLED
}

#ifdef $XCC_h
void
LProgram_close(LProgram this);
#endif//$XCC_h

void
LProgram_close(LProgram this)
{
#ifdef CPLEX_ENABLED
  int cpx_error = 0;
  struct cpxenv *cpxenv = LProgram_CPXENV(this);
  struct cpxlp *cpxlp = LProgram_CPXLP(this);
  if (cpxlp != NULL) {
    cpx_error = CPXfreeprob(cpxenv, &cpxlp);
    if (cpx_error != 0) {
      fprintf(stderr, "CPXfreeprob failed: error %d.\n", cpx_error);
    }
  }
  if (cpxenv != NULL) {
    cpx_error = CPXcloseCPLEX (&cpxenv);
    if (cpx_error != 0) {
      char errorstring[CPX_STR_PARAM_MAX];
      CPXgeterrorstring(cpxenv, cpx_error, errorstring);
      fprintf(stderr, "CPXcloseCPLEX failed: %s", errorstring);
    }
  }
  *LProgram__CPXENV(this) = NULL;
  *LProgram__CPXLP(this) = NULL;
#else//!CPLEX_ENABLED
#ifdef GLPK_ENABLED
  glp_prob *glp = LProgram_GLP(this);
  glp_delete_prob(glp);
  *LProgram__GLP(this) = NULL;
#endif//GLPK_ENABLED
#endif//CPLEX_ENABLED
}

#ifdef $XCC__h
bool
LProgram_enterVariables(LProgram this);
#endif//$XCC__h

bool
LProgram_enterVariables(LProgram this)
{
  bool success = false;
#ifdef CPLEX_ENABLED
  int cpx_error = 0;
  struct cpxenv *cpxenv = LProgram_CPXENV(this);
  struct cpxlp *cpxlp = LProgram_CPXLP(this);
  int32_t variableCount = LProgram_VARIABLECOUNT(this);
  double *lower = malloc(variableCount*sizeof(double));
  double *upper = malloc(variableCount*sizeof(double));
  char *type = malloc(variableCount*sizeof(char));
  double *coeff = malloc(variableCount*sizeof(double));
  const char **name = malloc(variableCount*sizeof(char*));
  double sign = LProgram_isMaximize(this)? -1.0: 1.0;
  int32_t variable_index = 0;
  // Fill the above arrays in continuous variable_index order.
  LProgram_FOREACH_LProgramVariable(this, variable) {
    lower[variable_index] = LProgramVariable_lower(variable);
    upper[variable_index] = LProgramVariable_upper(variable);
    type[variable_index] = LProgramVariable_typeCPLEX(variable);
    coeff[variable_index] = LProgramVariable_weight(variable)*sign;
    name[variable_index] = LProgramVariable_name(variable);
    *LProgramVariable__INDEX(variable) = variable_index++;
  } LProgram_ENDEACH_LProgramVariable;
  Except_CHECK(variable_index == variableCount);
  // Enter the column into CPLEX.
  cpx_error = CPXnewcols(cpxenv, cpxlp, variableCount,
                         coeff, lower, upper, type, (char **)name);
  free(name); free(coeff); free(type); free(upper); free(lower);
  if (cpx_error != 0) {
    fprintf(stderr, "CPXnewcols failed: error %d.\n", cpx_error);
  }
  success = cpx_error == 0;
#else//!CPLEX_ENABLED
#ifdef GLPK_ENABLED
  int32_t variableCount = LProgram_VARIABLECOUNT(this);
  int32_t variable_index = 0;
  glp_prob *glp = LProgram_GLP(this);
  if (LProgram_isMinimize(this)) {
    glp_set_obj_dir(glp, GLP_MIN);
  } else
  if (LProgram_isMaximize(this)) {
    glp_set_obj_dir(glp, GLP_MAX);
  } else Except_CHECK(false);
  glp_add_cols(glp, variableCount);
  LProgram_FOREACH_LProgramVariable(this, variable) {
    LProgramValue weight = LProgramVariable_WEIGHT(variable);
    LProgramValue lower = 0.0, upper = 0.0;
    int type = GLP_FR;
    if (LProgramVariable_hasLower(variable)) {
      lower = LProgramVariable_LOWER(variable);
      type = GLP_LO;
      if (LProgramVariable_hasUpper(variable)) {
        upper = LProgramVariable_UPPER(variable);
        type = GLP_DB;
      }
    } else {
      if (LProgramVariable_hasUpper(variable)) {
        upper = LProgramVariable_UPPER(variable);
        type = GLP_UP;
      }
    }
    if (LProgramVariable_NAME(variable) != NULL) {
      const char *name = LProgramVariable_NAME(variable);
      glp_set_col_name(glp, 1+variable_index, name);
    }
    glp_set_col_bnds(glp, 1+variable_index, type, lower, upper);
    glp_set_obj_coef(glp, 1+variable_index, weight);
    glp_set_col_kind(glp, 1+variable_index,
                     LProgramVariable_typeGLPK(variable));
    *LProgramVariable__INDEX(variable) = variable_index++;
  } LProgram_ENDEACH_LProgramVariable;
  Except_CHECK(variable_index == variableCount);
  success = true;
#endif//GLPK_ENABLED
#endif//CPLEX_ENABLED
  return success;
}

#ifdef $XCC__h
bool
LProgram_enterConstraints(LProgram this);
#endif//$XCC__h

bool
LProgram_enterConstraints(LProgram this)
{
  bool success = false;
#ifdef CPLEX_ENABLED
  int cpx_error = 0;
  struct cpxenv *cpxenv = LProgram_CPXENV(this);
  struct cpxlp *cpxlp = LProgram_CPXLP(this);
  LProgram_FOREACH_LProgramConstraint(this, constraint) {
    int zero = 0;
    double bound = LProgramConstraint_BOUND(constraint);
    char sense = LProgramConstraint_typeCPLEX(constraint);
    int termCount = LProgramConstraint_termCount(constraint);
    int *indices = malloc(termCount*sizeof(int));
    double *values = malloc(termCount*sizeof(double));
    const char *name = LProgramConstraint_name(constraint);
    int32_t term_index = 0;
    LProgramConstraint_FOREACH_LProgramTerm(constraint, term) {
      LProgramVariable variable = LProgramTerm_variable(term);
      indices[term_index] = LProgramVariable_INDEX(variable);
      values[term_index] = LProgramTerm_factor(term);
      term_index++;
    } LProgramConstraint_ENDEACH_LProgramTerm;
    Except_CHECK(term_index == termCount);
    // Enter the row into CPLEX.
    cpx_error = CPXaddrows(cpxenv, cpxlp, 0, 1, termCount,
                       &bound, &sense, &zero, indices, values, NULL, (char **)&name);
    free(values); free(indices);
    if (cpx_error != 0) {
      fprintf(stderr, "CPXaddrows failed: error %d.\n", cpx_error);
      break;
    }
  } LProgram_ENDEACH_LProgramConstraint;
  success = cpx_error == 0;
#else//!CPLEX_ENABLED
#ifdef GLPK_ENABLED
  glp_prob *glp = LProgram_GLP(this);
  int32_t constraintCount = LProgram_constraintCount(this);
  int32_t entry_count = 0, constraint_index = 0;
  glp_add_rows(glp, constraintCount);
  LProgram_FOREACH_LProgramConstraint(this, constraint) {
    LProgramValue bound = LProgramConstraint_BOUND(constraint);
    LProgramConstraint_FOREACH_LProgramTerm(constraint, term) {
      entry_count++;
    } LProgramConstraint_ENDEACH_LProgramTerm;
    if (LProgramConstraint_NAME(constraint) != NULL) {
      const char *name = LProgramConstraint_NAME(constraint);
      glp_set_row_name(glp, 1+constraint_index, name);
    }
    if (LProgramConstraint_isLE(constraint)) {
      glp_set_row_bnds(glp, 1+constraint_index, GLP_UP, 0.0, bound);
    } else
    if (LProgramConstraint_isGE(constraint)) {
      glp_set_row_bnds(glp, 1+constraint_index, GLP_LO, bound, 0.0);
    } else
    if (LProgramConstraint_isEQ(constraint)) {
      glp_set_row_bnds(glp, 1+constraint_index, GLP_FX, bound, bound);
    } else Except_CHECK(false);
    constraint_index++;
  } LProgram_ENDEACH_LProgramConstraint;
  Except_CHECK(constraint_index == constraintCount);
  if (entry_count > 0) {
    int32_t *ia = malloc((1+entry_count)*sizeof(int32_t));
    int32_t *ja = malloc((1+entry_count)*sizeof(int32_t));
    double *ar = malloc((1+entry_count)*sizeof(double));
    int32_t entry_index = 0, constraint_index = 0;
    LProgram_FOREACH_LProgramConstraint(this, constraint) {
      LProgramConstraint_FOREACH_LProgramTerm(constraint, term) {
        LProgramVariable variable = LProgramTerm_VARIABLE(term);
        int32_t variable_index = LProgramVariable_INDEX(variable);
        LProgramValue factor = LProgramTerm_FACTOR(term);
        ia[1+entry_index] = 1+constraint_index;
        ja[1+entry_index] = 1+variable_index;
        ar[1+entry_index] = factor;
        entry_index++;
      } LProgramConstraint_ENDEACH_LProgramTerm;
      constraint_index++;
    } LProgram_ENDEACH_LProgramConstraint;
    Except_CHECK(constraint_index == constraintCount);
    Except_CHECK(entry_index == entry_count);
    glp_load_matrix(glp, entry_count, ia, ja, ar);
    free(ar); free(ja); free(ia);
  }
  success = true;
#endif//GLPK_ENABLED
#endif//CPLEX_ENABLED
  return success;
}

#ifdef $XCC__h
void
LProgram_setStatusCPLEX(LProgram this);
#endif//$XCC__h

void
LProgram_setStatusCPLEX(LProgram this) 
{
#ifdef CPLEX_ENABLED
  struct cpxenv *cpxenv = LProgram_CPXENV(this);
  struct cpxlp *cpxlp = LProgram_CPXLP(this);
  LProgramStatus status = LProgramStatus__;
  int lpstat = CPXgetstat(cpxenv, cpxlp);
  switch(lpstat) {
  case CPX_STAT_OPTIMAL: 
  case CPXMIP_OPTIMAL:
  case CPXMIP_OPTIMAL_TOL:
    status = LProgramStatus_Solved; break;
  case CPXMIP_TIME_LIM_FEAS:
  case CPXMIP_ABORT_FEAS:
    status = LProgramStatus_Feasible; break;
  case CPX_STAT_ABORT_TIME_LIM:
  case CPXMIP_TIME_LIM_INFEAS:
    status = LProgramStatus_TimedOut; break;
  case CPX_STAT_INFEASIBLE:
  case CPX_STAT_INForUNBD:
  case CPXMIP_INFEASIBLE:
    status = LProgramStatus_Infeasible; break;
  case CPXMIP_INForUNBD:
    status = LProgramStatus_Unbounded; break;
  default:
    status = LProgramStatus_Error; break;
  }
  *LProgram__STATUS(this) = status;
#endif//CPLEX_ENABLED
}

#ifdef $XCC_h
void
LProgram_optimize(LProgram this, bool mip);
#endif//$XCC_h

void
LProgram_optimize(LProgram this, bool mip)
{
  time_t start = time(NULL), stop = 0;
#ifdef CPLEX_ENABLED
  int cpx_error = -1;
  struct cpxenv *cpxenv = LProgram_CPXENV(this);
  struct cpxlp *cpxlp = LProgram_CPXLP(this);
  double timeOut = LProgram_TIMEOUT(this);
  *LProgram__STATUS(this) = LProgramStatus_Error;
  if (timeOut > 0.0) {
    if (CPXsetdblparam(cpxenv, CPX_PARAM_TILIM, timeOut)) return;
  }
  if (   LProgram_enterVariables(this)
      && LProgram_enterConstraints(this)) {
    if (mip) {
      if (CPXchgprobtype(cpxenv, cpxlp, CPXPROB_MILP)) return;
      if (CPXsetintparam(cpxenv, CPX_PARAM_MIPEMPHASIS,
                         CPX_MIPEMPHASIS_FEASIBILITY)) return;
      cpx_error = CPXmipopt(cpxenv, cpxlp);
      LProgram_setStatusCPLEX(this);
      if (!cpx_error) {
        double solution = 0;
        int count = CPXgetnumcols(cpxenv, cpxlp);
        double *values = malloc(count*sizeof(double));
        cpx_error |= CPXgetmipobjval(cpxenv, cpxlp, &solution);
        cpx_error |= CPXgetmipx(cpxenv, cpxlp, values, 0, count-1);
        if (!cpx_error) {
          *LProgram__SOLUTION(this) = solution;
          LProgram_FOREACH_LProgramVariable(this, variable) {
            int32_t variable_index = LProgramVariable_INDEX(variable);
            LProgramVariable_setValue(variable, values[variable_index]);
          } LProgram_ENDEACH_LProgramVariable;
        }
        free(values);
      }
    } else {
      if (CPXchgprobtype(cpxenv, cpxlp, CPXPROB_LP)) return;
      cpx_error = CPXlpopt(cpxenv, cpxlp);
      LProgram_setStatusCPLEX(this);
      if (!cpx_error) {
        double solution = 0;
        int count = CPXgetnumcols(cpxenv, cpxlp);
        double *values = malloc(count*sizeof(double));
        cpx_error |= CPXgetobjval(cpxenv, cpxlp, &solution);
        cpx_error |= CPXgetx(cpxenv, cpxlp, values, 0, count-1);
        if (!cpx_error) {
          *LProgram__SOLUTION(this) = solution;
          LProgram_FOREACH_LProgramVariable(this, variable) {
            int32_t variable_index = LProgramVariable_INDEX(variable);
            LProgramVariable_setValue(variable, values[variable_index]);
          } LProgram_ENDEACH_LProgramVariable;
        }
        free(values);
      }
    }
  }
#else//!CPLEX_ENABLED
#ifdef GLPK_ENABLED
  glp_prob *glp = LProgram_GLP(this);
  double timeOut = LProgram_TIMEOUT(this);
  glp_smcp smcp_, *smcp = NULL;
  glp_iocp iocp_, *iocp = NULL;
  if (timeOut > 0.0) {
    smcp = &smcp_;
    glp_init_smcp(smcp);
    smcp->tm_lim = (int)(timeOut*1000.0);
    iocp = &iocp_;
    glp_init_iocp(iocp);
    iocp->tm_lim = (int)(timeOut*1000.0);
  }
  if (   LProgram_enterVariables(this)
      && LProgram_enterConstraints(this)) {
    int glp_err = glp_simplex(glp, smcp);
    if (!mip) {
      if (!glp_err) {
        int glp_status = glp_get_status(glp);
        *LProgram__SOLUTION(this) = glp_get_obj_val(glp);
        LProgram_FOREACH_LProgramVariable(this, variable) {
          int32_t variable_index = LProgramVariable_INDEX(variable);
          LProgramValue value = glp_get_col_prim(glp, 1+variable_index);
          LProgramVariable_setValue(variable, value);
        } LProgram_ENDEACH_LProgramVariable;
        switch (glp_status) {
          case GLP_OPT:
            *LProgram__STATUS(this) = LProgramStatus_Solved; break;
          case GLP_FEAS:
            *LProgram__STATUS(this) = LProgramStatus_Feasible; break;
          case GLP_INFEAS:
            *LProgram__STATUS(this) = LProgramStatus_Infeasible; break;
          case GLP_UNBND:
            *LProgram__STATUS(this) = LProgramStatus_Unbounded; break;
          case GLP_UNDEF:
            *LProgram__STATUS(this) = LProgramStatus_Undefined; break;
          default:
            *LProgram__STATUS(this) = LProgramStatus_Error;
        }
      } else if (glp_err == GLP_EITLIM) {
        *LProgram__STATUS(this) = LProgramStatus_TimedOut;
      } else {
        *LProgram__STATUS(this) = LProgramStatus_Error;
      }
    }
    if (mip && !glp_err) {
      glp_err = glp_intopt(glp, iocp);
      if (!glp_err) {
        int glp_status = glp_mip_status(glp);
        *LProgram__SOLUTION(this) = glp_mip_obj_val(glp);
        LProgram_FOREACH_LProgramVariable(this, variable) {
          int32_t variable_index = LProgramVariable_INDEX(variable);
          LProgramValue value = glp_mip_col_val(glp, 1+variable_index);
          LProgramVariable_setValue(variable, value);
        } LProgram_ENDEACH_LProgramVariable;
        switch (glp_status) {
          case GLP_OPT:
            *LProgram__STATUS(this) = LProgramStatus_Solved; break;
          case GLP_FEAS:
            *LProgram__STATUS(this) = LProgramStatus_Feasible; break;
          case GLP_NOFEAS:
            *LProgram__STATUS(this) = LProgramStatus_Infeasible; break;
          case GLP_UNDEF:
            *LProgram__STATUS(this) = LProgramStatus_Undefined; break;
          default:
            *LProgram__STATUS(this) = LProgramStatus_Error;
        }
      } else if (glp_err == GLP_EITLIM) {
        *LProgram__STATUS(this) = LProgramStatus_TimedOut;
      } else {
        *LProgram__STATUS(this) = LProgramStatus_Error;
      }
    }
  }
#else//GLPK_ENABLED
  Except_NEVER("No linear programming solver (need CPLEX or GLPK)");
#endif//GLPK_ENABLED
#endif//CPLEX_ENABLED
  stop = time(NULL);
  //  fprintf(stdout, "CPLEX time\t%e seconds\n", difftime(stop, start));
  *LProgram__TIMESPENT(this) = difftime(stop, start);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  LProgram program = LProgram_new(Memory_Root, LProgramType_Maximize, 0.0);
  LProgramVariable x_1 = LProgram_makeIntegerVariable(program, "x_1");
  LProgramVariable x_2 = LProgram_makeIntegerVariable(program, "x_2");
  LProgramVariable x_3 = LProgram_makeIntegerVariable(program, "x_3");
  LProgramConstraint constraint_1 =
      LProgram_makeLEConstraint(program, (LProgramValue)20, NULL);
  LProgramConstraint constraint_2 =
      LProgram_makeLEConstraint(program, (LProgramValue)30, NULL);
  LProgramVariable_setWeight(x_1, (LProgramValue)1);
  LProgramVariable_setWeight(x_2, (LProgramValue)2);
  LProgramVariable_setWeight(x_3, (LProgramValue)3);
  LProgramConstraint_addTerm(constraint_1, x_1, (LProgramValue)-1);
  LProgramConstraint_addTerm(constraint_1, x_2, (LProgramValue)1);
  LProgramConstraint_addTerm(constraint_1, x_3, (LProgramValue)1);
  LProgramConstraint_addTerm(constraint_2, x_1, (LProgramValue)1);
  LProgramConstraint_addTerm(constraint_2, x_2, (LProgramValue)-3);
  LProgramConstraint_addTerm(constraint_2, x_3, (LProgramValue)1);
  LProgramVariable_setUpper(x_1, (LProgramValue)40);
  printf("Initial Program:\n");
  LProgram_pretty(program, stdout);
  LProgram_open(program, "problem");
#if defined(CPLEX_ENABLED) || defined(GLPK_ENABLED)
  fprintf(stderr,"One of CPLEX or GLPK is ENABLED\n");
  LProgram_optimize(program, true);
#else
  fprintf(stderr,"CPLEX and GLPK are both DISABLED.\n");
  *LProgram__STATUS(program) = LProgramStatus_Solved;
  LProgramVariable_setValue(x_1, (LProgramValue) 40);
  LProgramVariable_setValue(x_2, (LProgramValue) 18);
  LProgramVariable_setValue(x_3, (LProgramValue) 42);
#endif//
  LProgram_close(program);
  printf("Solved Program:\n");
  LProgram_pretty(program, stdout);
  LProgram_delete(program);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the LProgram module.
 */
#define LProgram_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the LProgram module.
 */
#define LProgram_FINI()
#endif//$XCC__h

#if XCC__C
static void
LProgram_TEST(void)
{
#include "CAL/LProgram_.c"
}

int
main(int argc, char **argv)
{
  CAL_Test(LProgram_TEST);
  return 0;
}
#endif

