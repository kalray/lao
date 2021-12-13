#ifdef $XCC_h
/*
 * !!!!	Coalescer.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Fabrice Rastello (Fabrice.Rastello@ens-lyon.fr).
 * Christophe Guillon (Christophe.Guillon@ens-lyon.fr).
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
 * @ingroup PFA
 * @brief Variable Coalescer base class.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/Coalescer.h"
#endif//$XCC__h

#ifdef $XCC__h
/**
 * 1 => use LiveSets.
 */
#if _OPTIMIZE
//#define Coalescer_USELIVESETS 0
#endif//_OPTIMIZE
#ifndef Coalescer_USELIVESETS
extern int Coalescer_USELIVESETS;
#endif//Coalescer_USELIVESETS
#endif//$XCC__h

#ifndef Coalescer_USELIVESETS
int Coalescer_USELIVESETS = 0;
#endif//Coalescer_USELIVESETS

#ifdef $XCC__h
/**
 * If not zero, use explicit intersection graph.
 * 0 => ordering-based intersection, 1 => intersection graph, 2 => both for debug.
 */
#if _OPTIMIZE
//#define Coalescer_INTERGRAPH 0
#endif//_OPTIMIZE
#ifndef Coalescer_INTERGRAPH
extern int Coalescer_INTERGRAPH;
#endif//Coalescer_INTERGRAPH
#endif//$XCC__h

#ifndef Coalescer_INTERGRAPH
int Coalescer_INTERGRAPH = 1;
#endif//Coalescer_INTERGRAPH

#ifdef $XCC__h
/**
 * If not zero, use value equivalence between variables.
 */
#if _OPTIMIZE
//#define Coalescer_EQUIVALUE 1
#endif//_OPTIMIZE
#ifndef Coalescer_EQUIVALUE
extern int Coalescer_EQUIVALUE;
#endif//Coalescer_EQUIVALUE
#endif//$XCC__h

#ifndef Coalescer_EQUIVALUE
int Coalescer_EQUIVALUE = 1;
#endif//Coalescer_EQUIVALUE

#ifdef $XCC__h
/**
 * If not zero, use lexical order for intersection between variables.
 */
#if _OPTIMIZE
//#define Coalescer_DOMFOREST 1
#endif//_OPTIMIZE
#ifndef Coalescer_DOMFOREST
extern int Coalescer_DOMFOREST;
#endif//Coalescer_DOMFOREST
#endif//$XCC__h

#ifndef Coalescer_DOMFOREST
int Coalescer_DOMFOREST = 1;
#endif//Coalescer_DOMFOREST

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Base class for SSA-based coalescer.
 */
struct Coalescer_;
#endif//$XCC_h

#ifdef $XCC__h
struct Coalescer_ {
  //@args	Memory memory, CodeRegion codeRegion, TemporaryTable temporaryTable,
  //@args	Dominance dominance, OptimizeFlags coalescingFlags
  CodeRegion CODEREGION;		//!< The CodeRegion of this Coalescer.
  TemporaryTable TEMPORARYTABLE;	//!< The global Variable(s) considered.
  Dominance DOMINANCE;			//!< This SSAForm pre-dominance if requested.
  IBList_ CONGRUENCES[1];		//!< Container for the congruence classes.
  //@access MEMORY	IBList_memory(Coalescer_CONGRUENCES(this))
  VariableStack_ DOMSTACK[1];		//!< Stack used for DomForest interference checks.
  Interference INTERFERENCE;		//!< Interference graph of the PHI and COPY related variables.
  LiveSets LIVESETS;			//!< Live sets of the PHI and COPY related variables.
  //@access LIVEINSETS	LiveSets_LIVEINSETS(Coalescer_LIVESETS(this))
  //@access LIVEOUTSETS	LiveSets_LIVEOUTSETS(Coalescer_LIVESETS(this))
  //@access PHIDEFSETS	LiveSets_PHIDEFSETS(Coalescer_LIVESETS(this))
  //@access PHIUSESETS	LiveSets_PHIUSESETS(Coalescer_LIVESETS(this))
  LiveCheck LIVECHECK;			//!< Live-check of the PHI and COPY related variables.
  bool CONSUMING;			//!< Whether Variable(s) are consuming.
  OptimizeFlags FLAGS;			//!< This OptimizeCoalescing flags.
  //@access isSreedhar	((Coalescer_FLAGS(this)&OptimizeCoalescing_Sreedhar) != 0)
  //@access isBoissinot	((Coalescer_FLAGS(this)&OptimizeCoalescing_Boissinot) != 0)
  //@access isDecoalesce	((Coalescer_FLAGS(this)&OptimizeCoalescing_Decoalesce) != 0)
  //@access isVirtualize	((Coalescer_FLAGS(this)&OptimizeCoalescing_Virtualize) != 0)
  //@access isCongruence	((Coalescer_FLAGS(this)&OptimizeCoalescing_Congruence) != 0)
};
#endif//$XCC__h

Coalescer
Coalescer_Ctor(Coalescer this,
               Memory memory, CodeRegion codeRegion, TemporaryTable temporaryTable,
               Dominance dominance, OptimizeFlags coalescingFlags)
{
  *Coalescer__CODEREGION(this) = codeRegion;
  *Coalescer__TEMPORARYTABLE(this) = temporaryTable;
  *Coalescer__DOMINANCE(this) = dominance;
  IBList_Ctor(Coalescer_CONGRUENCES(this), memory, sizeof(VariableClass_), 32);
  VariableStack_Ctor(Coalescer_DOMSTACK(this), memory, 0);
  *Coalescer__INTERFERENCE(this) = NULL;
  *Coalescer__LIVESETS(this) = NULL;
  *Coalescer__LIVECHECK(this) = NULL;
  *Coalescer__CONSUMING(this) = false;
  if (   (Coalescer_INTERGRAPH==0 || Coalescer_INTERGRAPH==2 || Coalescer_USELIVESETS==0)
      || (Coalescer_USELIVESETS==1 && LiveSets_UPANDMARK==2)
      || (coalescingFlags & (OptimizeCoalescing_Virtualize | OptimizeCoalescing_Decoalesce) )  ) {
    *Coalescer__CONSUMING(this) = true;
  }
  0&&
  PFA_TRACE && fprintf(PFA_TRACE, "*** Coalescer_Ctor: variables=%d blocks=%d\n",
                       TemporaryTable_count(temporaryTable), CodeRegion_blockCount(codeRegion) - 1);
  *Coalescer__FLAGS(this) = coalescingFlags;
  if (   (coalescingFlags & OptimizeCoalescing_Decoalesce)
      || (   coalescingFlags & OptimizeCoalescing_Boissinot
          && coalescingFlags & OptimizeCoalescing_Virtualize)) {
    // Initialize PHI-Operations in CopyMode
    CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
      BasicBlock_FOREACH_Operation(basicBlock, operation) {
        if (Operator_isPhi(Operation_operator(operation))) {
          Operation_setFlags(operation, OperationFlag_CopyMode);
        } else break;
      } BasicBlock_ENDEACH_Operation;
    } CodeRegion_ENDEACH_BasicBlock;
  }
  Coalescer_initClasses(this);
  return this;
}

void
Coalescer_Dtor(Coalescer this)
{
  /* Memory statistics */
  if (!FORCE_POOLED) {
    fprintf(stderr, "*** Coalescer Variable(s):\tcount=%d\n",
            TemporaryTable_count(Coalescer_TEMPORARYTABLE(this)));
  }
  if (!FORCE_POOLED && Coalescer_LIVECHECK(this) != NULL) {
    fprintf(stderr, "*** Coalescer LiveCheck:\ttotalAlloc=%zu\n",
            Memory_TOTALSIZE(LiveCheck_MEMORY(Coalescer_LIVECHECK(this))));
  }
  if (!FORCE_POOLED && Coalescer_INTERFERENCE(this) != NULL) {
    fprintf(stderr, "*** Coalescer Interference:\ttotalAlloc=%zu\n",
            Memory_TOTALSIZE(Interference_MEMORY(Coalescer_INTERFERENCE(this))));
  }
  if (!FORCE_POOLED) {
    fprintf(stderr, "*** Coalescer:\tblock_count=%d\n",
            CodeRegion_blockCount(Coalescer_CODEREGION(this)));
  }
  IBList_empty(Coalescer_CONGRUENCES(this), (IBListItemRelease)VariableClass_Dtor);
  IBList_Dtor(Coalescer_CONGRUENCES(this));
  VariableStack_Dtor(Coalescer_DOMSTACK(this));
  Interference_kill(Coalescer_INTERFERENCE(this));
  LiveSets_kill(Coalescer_LIVESETS(this));
  LiveCheck_kill(Coalescer_LIVECHECK(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Coalescer_) = %zu\n", sizeof(Coalescer_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize congruence classes from pinned Variable(s).
 */
void
Coalescer_initClasses(Coalescer this);
#endif//$XCC__h

void
Coalescer_initClasses(Coalescer this)
{
  VariableClass *dedicatedClass = NULL;
  Memory memory = Coalescer_MEMORY(this);
  IBList congruences = Coalescer_CONGRUENCES(this);
  TemporaryTable temporaryTable = Coalescer_TEMPORARYTABLE(this);
  TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
    Variable variable = Temporary_VARIABLE(temporary);
    Register registre = Variable_PINNED(variable);
    VariableClass congruence = NULL;
    // Ensure the congruence fields are cleared.
    *Variable__CLASSNEXT(variable) = NULL;
    *Variable__CONGRUENCE(variable) = NULL;
    // If the variable is constrained to a specific register we must mark the congruence class.
    if (registre != Register__UNDEF) {
      if (dedicatedClass == NULL) {
        dedicatedClass = Memory_alloc(memory, Register__Count*sizeof(VariableClass));
        memset(dedicatedClass, 0, Register__Count*sizeof(VariableClass));
      }
      if (dedicatedClass[registre] == NULL) {
        congruence = IBList_push(congruences);
        dedicatedClass[registre] = congruence;
        VariableClass_Ctor(congruence, variable);
        *VariableClass__REGISTER(congruence) = registre;
      } else {
        congruence = dedicatedClass[registre];
        VariableClass_insert(congruence, variable);
      }
    } else {
      congruence = IBList_push(congruences);
      VariableClass_Ctor(congruence, variable);
    }
  } TemporaryTable_ENDEACH_Temporary;
  if (0&& PFA_TRACE && dedicatedClass != NULL) {
    fprintf(PFA_TRACE, "*** Congruences Precoalesced\n");
    TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
      Variable variable = Temporary_VARIABLE(temporary);
      VariableClass congruence = Variable_CONGRUENCE(variable);
      PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE)
                && fprintf(PFA_TRACE, " => ")
                && VariableClass_pretty(congruence, PFA_TRACE, false)
                && fprintf(PFA_TRACE, "\n");
    } TemporaryTable_ENDEACH_Temporary;
  }
  if (dedicatedClass != NULL) {
    Memory_free(memory, dedicatedClass);
  }
}

#ifdef $XCC__h
/**
 * Complete this Coalescer construction.
 * Creates livesets/livecheck in the right mode & interference if necessary
 */
void
Coalescer_complete(Coalescer this, bool copyMode);
#endif//$XCC__h

void
Coalescer_complete(Coalescer this, bool copyMode)
{
  Memory memory = Coalescer_MEMORY(this);
  Dominance dominance = Coalescer_DOMINANCE(this);
  CodeRegion codeRegion = Coalescer_CODEREGION(this);
  LoopForest loopForest = LoopForest_new(memory, codeRegion);
  TemporaryTable temporaryTable = Coalescer_TEMPORARYTABLE(this);
  LiveCheck liveCheck = LiveCheck_make(memory, dominance, loopForest);
  if (Coalescer_INTERGRAPH>0 || Coalescer_USELIVESETS>0) {
    LiveSets liveSets = LiveSets_make(memory, loopForest, temporaryTable);
    if (!copyMode) LiveCheck_check(liveCheck, temporaryTable);
    *Coalescer__LIVESETS(this) = liveSets;
  }
  if (Coalescer_INTERGRAPH>0) {
    *Coalescer__INTERFERENCE(this) = Interference_make(memory, codeRegion, temporaryTable, false);
  }
  *Coalescer__LIVECHECK(this) = liveCheck;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * For use by Coalescer_CONGRUENCES_FOREACH_VariableClass.
 */
IBList
Coalescer_congruences(Coalescer this);
#endif//$XCC_h

IBList
Coalescer_congruences(Coalescer this)
{
  return Coalescer_CONGRUENCES(this);
}

#ifdef $XCC_h
/**
 * Iterate this Coalescer VariableClass(es).
 */
#define Coalescer_CONGRUENCES_FOREACH_VariableClass(this, congruence) \
  IBList_FOREACH(Coalescer_congruences(this), VariableClass_, congruence)
#define Coalescer_CONGRUENCES_ENDEACH_VariableClass \
  IBList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC__h
/**
 * SAS'99 congruence-based copy coalescing by Sreedhar et al. 
 *
 * Simpler than Sreedhar because did not nullify singleton congruence classes.
 *
 * @note fab: This is not Sreedhar's coalescing.
 * This is a more general congruence-based coalescing under SSA.
 * It can handle different interference modes: VALUE (quadratic | linear), Sreedhar.
 * To be exhaustive we may add the intersection test (Interference_CC).
 * Maybe the mode should be given as an argument with a cross check. 
 */
void
Coalescer_coalesceCongruences(Coalescer this);
#endif//$XCC__h

void
Coalescer_coalesceCongruences(Coalescer this)
{
  CodeRegion codeRegion = Coalescer_CODEREGION(this);
  PFA_TRACE && fprintf(PFA_TRACE, "*** Congruence Coalescing:\n");
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      if (Operator_isSeqCopy(operator) || Operator_isParCopy(operator)) {
        int32_t argCount = Operation_argCount(operation), index;
        for (index = 0; index < argCount; index++) {
          Temporary temporary_x = Operation_getResult(operation, index);
          Temporary temporary_y = Operation_getArgument(operation, index);
          if (   Temporary_isVariable(temporary_x)
              && Temporary_isVariable(temporary_y)) {
            Variable variable_x = Temporary_VARIABLE(temporary_x);
            Variable variable_y = Temporary_VARIABLE(temporary_y);
            VariableClass congruence_x = Variable_CONGRUENCE(variable_x);
            VariableClass congruence_y = Variable_CONGRUENCE(variable_y);
            if (congruence_x == congruence_y) {
              Variable_raiseCoalesced(variable_x);
              PFA_TRACE && fprintf (PFA_TRACE, "Coalescing ");
            } else {
              bool result = false;
              PFA_TIMER && fprintf(PFA_TIMER, "@@@ STAT ICC\n");
              if (Coalescer_interferesCCRegisters(this, congruence_x, congruence_y)) {
                result = true;
              } else
              if (Coalescer_EQUIVALUE || Coalescer_DOMFOREST) {
                result = Coalescer_interferesDomForest(this, congruence_x, congruence_y);
              } else {
                // Sreedhar trick: knows that COPYies are being coalesced.
                VariableClass_FOREACH_Variable(congruence_x, resource_x) {
                  VariableClass_FOREACH_Variable(congruence_y, resource_y) {
                    if (   resource_x != variable_x
                        && Coalescer_interferesVV(this, resource_x, resource_y)) {
                      result = true;
                      goto leave;
                    }
                    if (   resource_y != variable_y
                        && Coalescer_interferesVV(this, resource_x, resource_y)) {
                      result = true;
                      goto leave;
                    }
                  } VariableClass_ENDEACH_Variable;
                } VariableClass_ENDEACH_Variable;
              }
              leave:
              if (result) {
                PFA_TRACE && fprintf (PFA_TRACE, "Interfere ");
              } else {
                Variable_raiseCoalesced(variable_x);
                PFA_TRACE && fprintf (PFA_TRACE, "Coalescing ");
                // Merge the congruence classes.
                Coalescer_mergeCongruencesCC(this, congruence_y, congruence_x);
              }
            }
            PFA_TRACE && Temporary_pretty(temporary_x, PFA_TRACE)
                      && fprintf (PFA_TRACE, " ")
                      && Temporary_pretty(temporary_y, PFA_TRACE)
                      && fprintf (PFA_TRACE, "\n");
          }
        }
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * If the source & dest of a copy have the same web link then mark the source as isCoalesced
 */
void
Coalescer_markCoalescedCopies(Coalescer this);
#endif//$XCC__h

void
Coalescer_markCoalescedCopies(Coalescer this)
{
  CodeRegion codeRegion = Coalescer_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      if (Operator_isSeqCopy(operator) || Operator_isParCopy(operator)) {
        int32_t argCount = Operation_argCount(operation), index;
        for (index = 0; index < argCount; index++) {
          Temporary temporary_x = Operation_getResult(operation, index);
          Temporary temporary_y = Operation_getArgument(operation, index);
          if (   Temporary_isVariable(temporary_x)
              && Temporary_isVariable(temporary_y)) {
            Variable variable_x = Temporary_VARIABLE(temporary_x);
            VariableClass class_x = Variable_CONGRUENCE(variable_x);
            VariableClass class_y = Variable_CONGRUENCE(Temporary_VARIABLE(temporary_y));
            if ( class_x && class_y && class_x==class_y ) {
              Variable_raiseCoalesced(variable_x);
            } else {
              Except_CHECK(!Variable_isCoalesced(variable_x));
            }
          }
        }
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Dominance between Variable(s) a and b.
 */
bool
Coalescer_dominatesVV(const_Coalescer this, const_Variable a, const_Variable b);
#endif//$XCC__h

bool
Coalescer_dominatesVV(const_Coalescer this, const_Variable a, const_Variable b)
{
  const_OperationResult producer_a = Variable_PRODUCER(a);
  const_OperationResult producer_b = Variable_PRODUCER(b);
  Operation operation_a = OperationResult_OPERATION(producer_a);
  Operation operation_b = OperationResult_OPERATION(producer_b);
  BasicBlock block_a = Operation_basicBlock(operation_a);
  BasicBlock block_b = Operation_basicBlock(operation_b);
  if (block_a != block_b) {
    Dominance dominance = Coalescer_DOMINANCE(this);
    return Dominance_dominates(dominance, block_a, block_b);
  }
  return Variable_ORDERING(a) <= Variable_ORDERING(b);
}

#ifdef $XCC__h
/**
 * Dominance between Variable and BasicBlock.
 */
bool
Coalescer_dominatesVB(const_Coalescer this, const_Variable a, const_BasicBlock block_b);
#endif//$XCC__h

bool
Coalescer_dominatesVB(const_Coalescer this, const_Variable a, const_BasicBlock block_b)
{
  const_Dominance dominance = Coalescer_DOMINANCE(this);
  const_OperationResult producer_a = Variable_PRODUCER(a);
  Operation operation_a = OperationResult_OPERATION(producer_a);
  BasicBlock block_a = Operation_basicBlock(operation_a);
  return Dominance_dominates(dominance, block_a, block_b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Intersection between Variable(s) a and b.
 */
bool
Coalescer_intersectsVV(Coalescer this, Variable a, Variable b);
#endif//$XCC__h

bool
Coalescer_intersectsVV(Coalescer this, Variable a, Variable b)
{
  bool exists = false;
  Except_CHECK(a != b);
  PFA_TIMER && fprintf(PFA_TIMER, "@@@ STAT XVV\n");
  if (Variable_ORDERING(a) < Variable_ORDERING(b)) {
    Variable tmp = a; a = b; b = tmp;
  }
  if (Coalescer_INTERGRAPH>0) { //fab: we do not know if INTERGRAPH is an intersection graph or an interference graph. Maybe we should rename it to avoid confusion.
    // Compute intersection with Interference.
    Interference interference = Coalescer_INTERFERENCE(this);
    exists = Interference_exists(interference, Variable_TEMPORARY(a), Variable_TEMPORARY(b));
  }
  if (Coalescer_INTERGRAPH==0 || Coalescer_INTERGRAPH==2) {
    // Compute intersection with ordering(s).
    LiveCheck liveCheck = Coalescer_LIVECHECK(this);
    Operation operation_a = OperationResult_OPERATION(Variable_PRODUCER(a));
    Operation operation_b = OperationResult_OPERATION(Variable_PRODUCER(b));
    int32_t ordering_a = Variable_ORDERING(a);
    int32_t ordering_b = Variable_ORDERING(b);
    BasicBlock block_a = Operation_basicBlock(operation_a);
    Except_CHECK(Variable_ORDERING(a) >= Variable_ORDERING(b)); /* a is defined after b */
    if (ordering_a == ordering_b) {
      Except_CHECK(Coalescer_INTERGRAPH <= 0 || exists == true);
      return true;
    }
    // LivenessMode_Mixed.
    if (LiveCheck_isLiveOutOrdering(liveCheck, block_a, ordering_a, b, LivenessMode_Mixed)) return true;
    Except_CHECK(Coalescer_INTERGRAPH <= 0 || exists == false);
    return false;
  } else {
    Except_CHECK(Coalescer_INTERGRAPH>0);
    return exists;
  }
}

#ifdef $XCC__h
/**
 * Interference between Variable(s) a and b.
 */
bool
Coalescer_interferesVV(Coalescer this, Variable a, Variable b);
#endif//$XCC__h

bool
Coalescer_interferesVV(Coalescer this, Variable a, Variable b)
{
  bool result;
  if (   Coalescer_EQUIVALUE
      && Variable_EQUIVALUE(a) == Variable_EQUIVALUE(b)) {
    result = false;
  } else {
    result = Coalescer_intersectsVV(this, a, b);
  }
  PFA_TRACE && 0 && fprintf(PFA_TRACE, "I(")
            && Variable_pretty(a, PFA_TRACE, 1)
            && fprintf(PFA_TRACE, ",")
            && Variable_pretty(b, PFA_TRACE, 1)
            && fprintf(PFA_TRACE, ",%d)\n", result);
  return result;
}

#ifdef $XCC__h
/**
 * Interference between Variable(s) a and b or one ancestor.
 *
 * @pre @p b is null or @p a ORDERING >= @p b ORDERING
 */
static bool
Coalescer_interferesDomForestVV(Coalescer this, Variable a, Variable b);
#endif//$XCC__h

static bool
Coalescer_interferesDomForestVV(Coalescer this, Variable a, Variable b)
{
  bool result;
  Except_CHECK(a != NULL);
  Except_CHECK(Coalescer_DOMFOREST);
  if (Coalescer_EQUIVALUE) {
    Except_CHECK(b == NULL || Variable_ORDERING(a) >= Variable_ORDERING(b));
    *Variable__EQUALANCOUT(a) = NULL;
    if (b != NULL && Variable_CONGRUENCE(a) == Variable_CONGRUENCE(b))
      b = Variable_EQUALANCOUT(b);
    if (b == NULL) return false;
    if (Variable_EQUIVALUE(a) != Variable_EQUIVALUE(b)) {
      // return chain_intersect(a, b)
      Variable tmp = b;
      while (   tmp != NULL
             && !Coalescer_intersectsVV(this, a, tmp)) {
        tmp = Variable_EQUALANCIN(tmp);
      }
      result = tmp != NULL;
    } else {
      // update equal_anc_out(a, b).
      Variable tmp = b;
      while (   tmp != NULL
             && !Coalescer_intersectsVV(this, a, tmp)) {
        tmp = Variable_EQUALANCIN(tmp);
      }
      *Variable__EQUALANCOUT(a) = tmp;
      result = false;
    }
  } else {
    if (b == NULL) {
      return false;
    } else if (Variable_CONGRUENCE(a) == Variable_CONGRUENCE(b)) {
      result = false;
    } else {
      result = Coalescer_intersectsVV(this, a, b);
    }
  }
  PFA_TRACE && 0 && fprintf(PFA_TRACE, "I(")
    && Variable_pretty(a, PFA_TRACE, 1)
    && fprintf(PFA_TRACE, ",")
    && Variable_pretty(b, PFA_TRACE, 1)
    && fprintf(PFA_TRACE, ",%d)\n", result);
  return result;
}

#ifdef $XCC__h
/**
 * Dominance forest interference between Variable(s).
 *
 * @pre	The Variable(s) have a valid lexicographic ORDERING.
 */
bool
Coalescer_interferesDomForest(Coalescer this,
                              VariableClass class_x,
                              VariableClass class_y);
#endif//$XCC__h

bool
Coalescer_interferesDomForest(Coalescer this,
                              VariableClass class_x,
                              VariableClass class_y)
{
  int32_t i = 0, j = 0, nb_x = 0, nb_y = 0;
  int32_t count_x = VariableClass_count(class_x);
  int32_t count_y = VariableClass_count(class_y);
  VariableStack domStack = Coalescer_DOMSTACK(this);
  Dominance dominance = Coalescer_DOMINANCE(this);
  const int32_t *preDFN = Dominance_PREDFN(dominance);
  const int32_t *lastDFN = Dominance_LASTDFN(dominance);
  Variable ordered_x = VariableClass_FIRST(class_x);
  Variable ordered_y = VariableClass_FIRST(class_y);
  Except_CHECK(count_x > 0 && count_y > 0);
  Except_CHECK(Coalescer_DOMFOREST);
  VariableStack_empty(domStack);
  VariableStack_resize(domStack, count_x + count_y);
  /* Was: (i < count_x || j < count_y) */
  while (   i < count_x + nb_x
         && j < count_y + nb_y
         && (i < count_x || j < count_y)) {
    Variable curr = NULL, top = NULL;
    Operation curr_operation = NULL;
    int32_t curr_block_index = 0;
    Except_CHECK(i < count_x || j < count_y);
    if (j == count_y) {
      curr = ordered_x;
      ordered_x = Variable_CLASSNEXT(ordered_x);
      i++, nb_x++;
    } else if (i == count_x) {
      curr = ordered_y;
      ordered_y = Variable_CLASSNEXT(ordered_y);
      j++, nb_y++;
    } else if (Variable_ORDERING(ordered_x) < Variable_ORDERING(ordered_y)) {
      curr = ordered_x;
      ordered_x = Variable_CLASSNEXT(ordered_x);
      i++, nb_x++;
    } else {
      curr = ordered_y;
      ordered_y = Variable_CLASSNEXT(ordered_y);
      j++, nb_y++;
    }
    curr_operation = OperationResult_OPERATION(Variable_PRODUCER(curr));
    curr_block_index = BasicBlock_index(Operation_basicBlock(curr_operation));
    while (!VariableStack_isEmpty(domStack)) {
      top = VariableStack_top(domStack);
      // Break iff top dominates curr.
      if (Variable_ORDERING(top) <= Variable_ORDERING(curr)) {
        Operation top_operation = OperationResult_OPERATION(Variable_PRODUCER(top));
        int32_t top_index = BasicBlock_index(Operation_basicBlock(top_operation));
        if (lastDFN[top_index] >= preDFN[curr_block_index]) break;
      }
      if (Variable_CONGRUENCE(top) == class_x) nb_x--;
      else nb_y--;
      VariableStack_pop(domStack);
    }
    VariableStack_push(domStack, curr);
    if (Coalescer_interferesDomForestVV(this, curr, top)) return true;
  }
  return false;
}

#ifdef $XCC__h
/**
 * Merge classes after a succesful coalescing.
 *
 * @pre	@p class_x and @p class_y two non interfering classes.
 * @return @p class_x in which @p class_y has been merged.
 */
VariableClass
Coalescer_mergeCongruencesCC(Coalescer this, VariableClass class_x, VariableClass class_y);
#endif//$XCC__h

VariableClass
Coalescer_mergeCongruencesCC(Coalescer this, VariableClass class_x, VariableClass class_y)
{
  PFA_TIMER && fprintf(PFA_TIMER, "@@@ STAT MCC\n");
  VariableClass_merge(class_x, class_y);
  if (Coalescer_DOMFOREST && Coalescer_EQUIVALUE) {
    // Require class_x and class_y with EQUALANCIN and EQUALANCOUT set.
    // EQUALANCIN points to the same class.
    // EQUALANCOUT points to the other class before the merge.
    VariableClass_FOREACH_Variable(class_x, variable) {
      Variable in = Variable_EQUALANCIN(variable);
      Variable out = Variable_EQUALANCOUT(variable);
      if (   in == NULL
          || (   out != NULL
              && Variable_ORDERING(out) > Variable_ORDERING(in))) {
        *Variable__EQUALANCIN(variable) = out;
      }
    } VariableClass_ENDEACH_Variable;
  }
  return class_x;
}

#ifdef $XCC__h
/**
 * Coalesce phi closure and isCoalesced variables.
 *
 * @pre initialized congruence classes.
 */
void
Coalescer_classesFromSSAwebs(Coalescer this);
#endif//$XCC__h

void
Coalescer_classesFromSSAwebs(Coalescer this)
{
  CodeRegion codeRegion = Coalescer_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      if (Operator_isPhi(operator)) {
        Coalescer_mergePhiCongruences(this, operation, NULL);
      }
      else if (   Operator_isSeqCopy(operator)
               || Operator_isParCopy(operator)) {
        int32_t argCount = Operation_argCount(operation), i;
        Temporary *arguments = Operation_arguments(operation);
        Temporary *results = Operation_results(operation);
        for (i = 0; i < argCount; i++) {
          if (   Temporary_isVariable(arguments[i])
              && Temporary_isVariable(results[i])) {
            Variable source = Temporary_VARIABLE(arguments[i]);
            Variable target = Temporary_VARIABLE(results[i]);
            if (Variable_isCoalesced(target)) {
              VariableClass congruence_source = Variable_CONGRUENCE(source);
              VariableClass congruence_target = Variable_CONGRUENCE(target);
              if ((congruence_source != congruence_target))
                VariableClass_merge(congruence_source, congruence_target);
           }
         }
       }
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC_h
/**
 * Type of filtering functions for Coalescer
 */
typedef bool (*CoalescerFilter)(Coalescer, Variable, VariableClass, Variable, VariableClass);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Merge the congruence classes of Operation operands.
 */
void
Coalescer_mergePhiCongruences(Coalescer this, Operation operation, CoalescerFilter filter);
#endif//$XCC_h

void
Coalescer_mergePhiCongruences(Coalescer this, Operation operation, CoalescerFilter filter)
{
  Temporary *operands = Operation_operands(operation);
  Variable variable_0 = Temporary_VARIABLE(operands[0]);
  VariableClass congruence_0 = Variable_CONGRUENCE(variable_0);
  int32_t operandCount = Operation_operandCount(operation), i;
  for (i = 1; i < operandCount; i++) {
    Variable variable_i = Temporary_VARIABLE(operands[i]);
    VariableClass congruence_i = Variable_CONGRUENCE(variable_i);
    if ((congruence_i != congruence_0) &&
        ((filter == NULL) || filter(this, variable_0, congruence_0, variable_i, congruence_i))) {
      VariableClass_merge(congruence_0, congruence_i);
    }
  }
}

#ifdef $XCC_h
/**
 * Merge the congruence classes of copy operation.
 */
void
Coalescer_mergeCopyCongruences(Coalescer this, Operation operation, CoalescerFilter filter);
#endif//$XCC_h

void
Coalescer_mergeCopyCongruences(Coalescer this, Operation operation, CoalescerFilter filter)
{
  int32_t argCount = Operation_argCount(operation), index;
  for (index = 0; index < argCount; index++) {
    Temporary temporary_x = Operation_getResult(operation, index);
    Temporary temporary_y = Operation_getArgument(operation, index);
    if (   Temporary_isVariable(temporary_x)
        && Temporary_isVariable(temporary_y)) {
      Variable variable_x = Temporary_VARIABLE(temporary_x);
      Variable variable_y = Temporary_VARIABLE(temporary_y);
      VariableClass congruence_x = Variable_CONGRUENCE(variable_x);
      VariableClass congruence_y = Variable_CONGRUENCE(variable_y);
      if ((congruence_x != congruence_y) &&
          ((filter == NULL) || filter(this, variable_x, congruence_x, variable_y, congruence_y))) {
        VariableClass_merge(congruence_x, congruence_y);
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Interference between class_x and class_y due to REGISTERs.
 */
static inline bool
Coalescer_interferesCCRegisters(Coalescer this, VariableClass class_x, VariableClass class_y)
{
  Register register_x = VariableClass_REGISTER(class_x);
  Register register_y = VariableClass_REGISTER(class_y);
  // There should be no two congruence classes with the same register.
  Except_CHECK(   register_x != register_y
               || register_x == Register__UNDEF);
  return   register_x != Register__UNDEF
        && register_y != Register__UNDEF;
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Interference between class_x and class_y.
 */
bool
Coalescer_interferesCC(Coalescer this, VariableClass class_x, VariableClass class_y);
#endif//$XCC__h

bool
Coalescer_interferesCC(Coalescer this, VariableClass class_x, VariableClass class_y)
{
  bool result = false;
  Except_CHECK(class_x != NULL && class_y != NULL);
  if (class_y == class_x) goto leave;
  // There is only one dedicated variable class per register
  // and two dedicated variable class interfere.
  if (Coalescer_interferesCCRegisters(this, class_x, class_y)) {
    result = true;
  } else
  if (Coalescer_DOMFOREST) {
    result = Coalescer_interferesDomForest(this, class_x, class_y);
  } else {
    VariableClass_FOREACH_Variable(class_x, resource_x) {
      VariableClass_FOREACH_Variable(class_y, resource_y) {
        if (Coalescer_interferesVV(this, resource_x, resource_y)) {
          result = true;
          goto leave;
        }
      } VariableClass_ENDEACH_Variable;
    } VariableClass_ENDEACH_Variable;
  }
leave:
  return result;
}

#ifdef $XCC__h
/**
 * Interference between a variable and a phi.
 */
bool
Coalescer_interferesVP(Coalescer this, Variable v, Operation phi);
#endif//$XCC__h

bool
Coalescer_interferesVP(Coalescer this, Variable v, Operation phi)
{
  // FIXME: use livein & liveout sets with a trick for branch bool instead
  BasicBlock B0 = Operation_basicBlock(phi);
  LiveCheck liveCheck = Coalescer_LIVECHECK(this);
  PFA_TRACE && 0 && fprintf(PFA_TRACE, "Checking interferesVphi phi:")
            && Operation_pretty(phi, PFA_TRACE) && fprintf(PFA_TRACE, "v: ")
            && Variable_pretty(v, PFA_TRACE, 0) && fprintf(PFA_TRACE, "\n");
  //foreach Bj
  Operation_OPERANDS_FOREACH_Temporary(phi, temp_aj) {
    int32_t j = Operation_OPERANDS_INDEX;
    Variable aj = Temporary_VARIABLE(temp_aj);
    if (Coalescer_EQUIVALUE && (Variable_EQUIVALUE(aj) == Variable_EQUIVALUE(v))) continue;
    if (!Coalescer_EQUIVALUE && (aj == v)) continue;
    if (j == 0) {
      // LivenessMode_Mixed.
      if (LiveCheck_isLiveInBasicBlock(liveCheck, B0, v, LivenessMode_Mixed)) return true;
    }
    else {
      BasicBlock Bj = BasicBlockEdge_origBlock(BasicBlock_getPhiEnterEdge(B0, j - 1));
      //PFA_TRACE && fprintf(PFA_TRACE,"DEBUG: j=%d, Bj=%d\n",j, BasicBlock_index(Bj));
      // LivenessMode_Mixed.
      if (LiveCheck_isLiveOutBasicBlock(liveCheck, Bj, v, LivenessMode_Mixed)) return true;
    }
  } Operation_OPERANDS_ENDEACH_Temporary;
  return false;
}

#ifdef $XCC__h
/**
 * Interference between a class and a phi.
 */
bool
Coalescer_interferesCP(Coalescer this, VariableClass X, Operation phi);
#endif//$XCC__h

bool
Coalescer_interferesCP(Coalescer this, VariableClass X, Operation phi)
{
  // FIXME: use livein & liveout sets with a trick for branch bool instead
  BasicBlock B0 = Operation_basicBlock(phi);
  LiveCheck liveCheck = Coalescer_LIVECHECK(this);
  PFA_TRACE && 0 && fprintf(PFA_TRACE, "Checking interferesCphi phi:")
            && Operation_pretty(phi, PFA_TRACE) && fprintf(PFA_TRACE, "X: ")
            && VariableClass_pretty(X, PFA_TRACE, 0) && fprintf(PFA_TRACE, "\n");
  //foreach xi in X
  VariableClass_FOREACH_Variable(X, xi) {
    //foreach Bj
    Operation_OPERANDS_FOREACH_Temporary(phi, temp_aj) {
      int32_t j = Operation_OPERANDS_INDEX;
      Variable aj = Temporary_VARIABLE(temp_aj);
      if (Coalescer_EQUIVALUE && (Variable_EQUIVALUE(aj) == Variable_EQUIVALUE(xi))) continue;
      if (!Coalescer_EQUIVALUE && (aj == xi)) continue;
      if (j == 0) {
        // LivenessMode_Mixed.
        if (LiveCheck_isLiveInBasicBlock(liveCheck, B0, xi, LivenessMode_Mixed)) return true;
      }
      else {
        BasicBlock Bj = BasicBlockEdge_origBlock(BasicBlock_getPhiEnterEdge(B0, j - 1));
        //PFA_TRACE && fprintf(PFA_TRACE,"DEBUG: j=%d, Bj=%d\n",j, BasicBlock_index(Bj));
        // LivenessMode_Mixed.
        if (LiveCheck_isLiveOutBasicBlock(liveCheck, Bj, xi, LivenessMode_Mixed)) return true;
      }
    } Operation_OPERANDS_ENDEACH_Temporary;
  } VariableClass_ENDEACH_Variable;
  return false;
}

#ifdef $XCC__h
/**
 * Add interferences between {x} and set_y \ {x} (Sreedhar).
 */
void
Coalescer_addInterferenceVS(Coalescer this, Variable variable_x, TemporarySet set_y);
#endif//$XCC__h

void
Coalescer_addInterferenceVS(Coalescer this, Variable variable_x, TemporarySet set_y)
{
  if (Coalescer_INTERGRAPH <= 0) {
  } else {
    Temporary temporary_x = Variable_TEMPORARY(variable_x);
    Interference interference = Coalescer_INTERFERENCE(this);
    PFA_TRACE && fprintf(PFA_TRACE, "addInterferenceVC: ") && Variable_pretty(variable_x, PFA_TRACE, 0)
              && TemporarySet_pretty(set_y, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
    TemporarySet_FOREACH_Temporary(set_y, temporary_y) {
      if (temporary_x != temporary_y) {
        Interference_add(interference, temporary_x, temporary_y);
      }
    } TemporarySet_ENDEACH_Temporary;
  }
}

#ifdef $XCC__h
/**
 * Add interferences between {x} and LiveIn(y) \ {x} (Sreedhar).
 */
void
Coalescer_addInterferenceVariableLiveIn(Coalescer this, Variable variable_x, BasicBlock block_y);
#endif//$XCC__h

void
Coalescer_addInterferenceVariableLiveIn(Coalescer this, Variable variable_x, BasicBlock block_y)
{
  if (Coalescer_INTERGRAPH>0) {
    Temporary temporary_x = Variable_TEMPORARY(variable_x);
    Interference interference = Coalescer_INTERFERENCE(this);
    if (Coalescer_USELIVESETS>0) {
      int32_t index_y = BasicBlock_index(block_y);
      TemporarySet liveInSet_y = Coalescer_LIVEINSETS(this) + index_y;
      TemporarySet phiDefSet_y = Coalescer_PHIDEFSETS(this) + index_y;
      PFA_TRACE && fprintf(PFA_TRACE, "addInterferenceVC: ") && Variable_pretty(variable_x, PFA_TRACE, 0)
                && TemporarySet_pretty(liveInSet_y, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
      TemporarySet_FOREACH_Temporary(liveInSet_y, temporary_y) {
        if (temporary_x != temporary_y) {
          Interference_add(interference, temporary_x, temporary_y);
        }
      } TemporarySet_ENDEACH_Temporary;
      PFA_TRACE && fprintf(PFA_TRACE, "addInterferenceVC: ") && Variable_pretty(variable_x, PFA_TRACE, 0)
                && TemporarySet_pretty(phiDefSet_y, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
      TemporarySet_FOREACH_Temporary(phiDefSet_y, temporary_y) {
        if (temporary_x != temporary_y) {
          Interference_add(interference, temporary_x, temporary_y);
        }
      } TemporarySet_ENDEACH_Temporary;
    } else {
      // No live sets, use live check.
      LiveCheck liveCheck = Coalescer_LIVECHECK(this);
      TemporaryTable temporaryTable = Coalescer_TEMPORARYTABLE(this);
      TemporaryTable_FOREACH_Temporary(temporaryTable, temporary_y) {
        Variable variable_y = Temporary_VARIABLE(temporary_y);
        if (   temporary_x != temporary_y
            && LiveCheck_isLiveInBasicBlock(liveCheck, block_y, variable_y, LivenessMode_Mplex)) {
          Interference_add(interference, temporary_x, temporary_y);
        }
      } TemporaryTable_ENDEACH_Temporary;
    }
  }
}

#ifdef $XCC__h
/**
 * Add interferences between {x} and LiveOut(y) \ {x} (Sreedhar).
 */
void
Coalescer_addInterferenceVariableLiveOut(Coalescer this, Variable variable_x, BasicBlock block_y);
#endif//$XCC__h

void
Coalescer_addInterferenceVariableLiveOut(Coalescer this, Variable variable_x, BasicBlock block_y)
{
  if (Coalescer_INTERGRAPH>0) {
    Temporary temporary_x = Variable_TEMPORARY(variable_x);
    Interference interference = Coalescer_INTERFERENCE(this);
    if (Coalescer_USELIVESETS>0) {
      int32_t index_y = BasicBlock_index(block_y);
      TemporarySet liveOutSet_y = Coalescer_LIVEOUTSETS(this) + index_y;
      TemporarySet phiUseSet_y = Coalescer_PHIUSESETS(this) + index_y;
      PFA_TRACE && fprintf(PFA_TRACE, "addInterferenceVC: ") && Variable_pretty(variable_x, PFA_TRACE, 0)
                && TemporarySet_pretty(liveOutSet_y, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
      TemporarySet_FOREACH_Temporary(liveOutSet_y, temporary_y) {
        if (temporary_x != temporary_y) {
          Interference_add(interference, temporary_x, temporary_y);
        }
      } TemporarySet_ENDEACH_Temporary;
      PFA_TRACE && fprintf(PFA_TRACE, "addInterferenceVC: ") && Variable_pretty(variable_x, PFA_TRACE, 0)
                && TemporarySet_pretty(phiUseSet_y, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
      TemporarySet_FOREACH_Temporary(phiUseSet_y, temporary_y) {
        if (temporary_x != temporary_y) {
          Interference_add(interference, temporary_x, temporary_y);
        }
      } TemporarySet_ENDEACH_Temporary;
    } else {
      // No live sets, use live check.
      LiveCheck liveCheck = Coalescer_LIVECHECK(this);
      TemporaryTable temporaryTable = Coalescer_TEMPORARYTABLE(this);
      TemporaryTable_FOREACH_Temporary(temporaryTable, temporary_y) {
        Variable variable_y = Temporary_VARIABLE(temporary_y);
        if (   temporary_x != temporary_y
            && LiveCheck_isLiveOutBasicBlock(liveCheck, block_y, variable_y, LivenessMode_Mplex)) {
          Interference_add(interference, temporary_x, temporary_y);
        }
      } TemporaryTable_ENDEACH_Temporary;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Initialize Congruence Classes to NULL.
 */
void
Coalescer_emptyClasses(Coalescer this);
#endif//$XCC_h

void
Coalescer_emptyClasses(Coalescer this)
{
  TemporaryTable temporaryTable = Coalescer_TEMPORARYTABLE(this);
  TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
    Variable variable = Temporary_VARIABLE(temporary);
    Except_CHECK(Temporary_isVariable(temporary));
    *Variable__CONGRUENCE(variable) = NULL;
    *Variable__CLASSNEXT(variable) = NULL;
  } TemporaryTable_ENDEACH_Temporary;
}

#ifdef $XCC_h
/**
 * Fill this Coalescer CONGRUENCES using the Variable web links.
 */
void
Coalescer_fillFromWebs(Coalescer this);
#endif//$XCC_h

void
Coalescer_fillFromWebs(Coalescer this)
{
  IBList congruences = Coalescer_CONGRUENCES(this);
  TemporaryTable temporaryTable = Coalescer_TEMPORARYTABLE(this);
  TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
    Variable variable = Temporary_VARIABLE(temporary);
    Except_CHECK(Temporary_isVariable(temporary));
    *Variable__CONGRUENCE(variable) = NULL;
    *Variable__CLASSNEXT(variable) = NULL;
  } TemporaryTable_ENDEACH_Temporary;
  TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
    Variable variable = Temporary_VARIABLE(temporary);
    Variable root = Variable_findWebRoot(variable);
    VariableClass congruence = Variable_CONGRUENCE(root);
    if (congruence == NULL) {
      congruence = IBList_push(congruences);
      VariableClass_Ctor(congruence, root);
    }
    if (variable != root) {
      VariableClass_insert(congruence, variable);
    }
  } TemporaryTable_ENDEACH_Temporary;
}

static bool
OperationPHI_isTrivial(Operation phi, Variable variable) {
  Temporary temporary = Variable_TEMPORARY(variable);
  Operation_ARGUMENTS_FOREACH_Temporary(phi, argument) {
    if (argument != temporary) return false;
  } Operation_ARGUMENTS_ENDEACH_Temporary;
  return true;
}

#ifdef $XCC__h
/**
 * Find an interfering congruence class.
 */
VariableClass
Coalescer_findInterferingClass(Coalescer this);
#endif//$XCC__h

VariableClass
Coalescer_findInterferingClass(Coalescer this)
{
  Coalescer_CONGRUENCES_FOREACH_VariableClass(this, congruence) {
    VariableClass_FOREACH_Variable(congruence, variable_1) {
      VariableClass_FOREACH_Variable(congruence, variable_2) {
        if (   variable_1 != variable_2
            && Coalescer_interferesVV(this, variable_1, variable_2)) {
          // No interference between two PHIs in the same basic block with the same arguments.
          Operation def_1 = OperationResult_OPERATION(Variable_PRODUCER(variable_1));
          Operation def_2 = OperationResult_OPERATION(Variable_PRODUCER(variable_2));
          bool interfere = true;
          if (   (Operation_BASICBLOCK(def_1) == Operation_BASICBLOCK(def_2))
              && (   Operator_isPhi(Operation_operator(def_1))
                  && Operator_isPhi(Operation_operator(def_2)))) {
            interfere = false;
            Operation_ARGUMENTS_FOREACH_Temporary(def_1, argument) {
              if (argument != Operation_getArgument(def_2, Operation_ARGUMENTS_INDEX)) {
                interfere = true;
                break;
              }
            } Operation_ARGUMENTS_ENDEACH_Temporary;
          }
          // No interference between a PHI and its arguments if they are all the same.
          else if (   (   Operator_isPhi(Operation_operator(def_1))
                       && OperationPHI_isTrivial(def_1, variable_2))
                   || (   Operator_isPhi(Operation_operator(def_2))
                       && OperationPHI_isTrivial(def_2, variable_1)))
            interfere = false;
          if (!interfere) continue;
          PFA_TRACE && fprintf(PFA_TRACE, " *** SSA Interference: ")
                    && Temporary_pretty(Variable_TEMPORARY(variable_1), PFA_TRACE)
                    && Temporary_pretty(Variable_TEMPORARY(variable_2), PFA_TRACE)
                    && fprintf(PFA_TRACE, "\n");
          PFA_TRACE && fflush(PFA_TRACE);
          return congruence;
        }
      } VariableClass_ENDEACH_Variable;
    } VariableClass_ENDEACH_Variable;
  } Coalescer_CONGRUENCES_ENDEACH_VariableClass;
  return NULL;
}

#ifdef $XCC__h
/**
 * Find an intersecting congruence class.
 */
VariableClass
Coalescer_findIntersectingClass(Coalescer this);
#endif//$XCC__h

VariableClass
Coalescer_findIntersectingClass(Coalescer this)
{
  Coalescer_CONGRUENCES_FOREACH_VariableClass(this, congruence) {
    VariableClass_FOREACH_Variable(congruence, variable_1) {
      VariableClass_FOREACH_Variable(congruence, variable_2) {
        if (   variable_1 != variable_2
            && Coalescer_intersectsVV(this, variable_1, variable_2)) {
          PFA_TRACE && fprintf(PFA_TRACE, " *** SSA Intersection ")
                    && Temporary_pretty(Variable_TEMPORARY(variable_1), PFA_TRACE)
                    && Temporary_pretty(Variable_TEMPORARY(variable_2), PFA_TRACE)
                    && fprintf(PFA_TRACE, "\n");
          return congruence;
        }
      } VariableClass_ENDEACH_Variable;
    } VariableClass_ENDEACH_Variable;
  } Coalescer_CONGRUENCES_ENDEACH_VariableClass;
  return NULL;
}

#ifdef $XCC__h
/**
 * Check if a Temporary is needed in BasicBlock phi-out.
 */
bool
Coalescer_isNeededPhiOutClass(Coalescer this, BasicBlock basicBlock, Temporary temporary);
#endif//$XCC__h

bool
Coalescer_isNeededPhiOutClass(Coalescer this, BasicBlock basicBlock, Temporary temporary)
{
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(basicBlock, edge) {
    BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
    BasicBlock_FOREACH_Operation(succ_block, operation) {
      if (Operator_isPhi(Operation_operator(operation))) {
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (argument == temporary) {
            BasicBlockEdge edge = BasicBlock_getPhiEnterEdge(succ_block, Operation_ARGUMENTS_INDEX);
            if (BasicBlockEdge_origBlock(edge) == basicBlock) return true;
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
      } else break;
    } BasicBlock_ENDEACH_Operation;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  return false;
}

#ifdef $XCC__h
/**
 * Main processing for Generate_Copies.
 */
void
Coalescer_explicitPhi(Coalescer this, Operation phi);
#endif//$XCC__h

void
Coalescer_explicitPhi(Coalescer this, Operation phi)
{
  CodeRegion codeRegion = Coalescer_CODEREGION(this);
  BasicBlock L0 = Operation_basicBlock(phi);
  VariableClass phiClass;
  // Set phiClass: the congruence class of the phi variable
  if (Operation_PHICLASS(phi) == NULL) {
    Memory memory = Coalescer_MEMORY(this);
    VariableClass congruence = IBList_push(Coalescer_CONGRUENCES(this));
    *Operation__PHICLASS(phi) = VariableClass_Ctor(congruence, NULL);
  }
  phiClass = Operation_PHICLASS(phi);
  PFA_TRACE && fprintf(PFA_TRACE, "\t\t\t\tEXPLICIT") && Operation_pretty(phi, PFA_TRACE)
            && fprintf(PFA_TRACE, "\t\t\t\t\tPHIVAR: ")
            && VariableClass_pretty(phiClass, PFA_TRACE, 0) && fprintf(PFA_TRACE, "\n");
  // Foreach Lj, j-th predecessor of L:
  Operation_ARGUMENTS_FOREACH_Temporary(phi, temp_aj) {
    Operation pcj = NULL;
    Variable aj = Temporary_VARIABLE(temp_aj);
    VariableClass class_aj = Variable_CONGRUENCE(aj);
    BasicBlock Lj = BasicBlockEdge_origBlock(BasicBlock_getPhiEnterEdge(L0, Operation_ARGUMENTS_INDEX));
    PFA_TRACE && fprintf(PFA_TRACE, "\t\t\t\t\tAj: ")
              && ( (class_aj && VariableClass_pretty(class_aj, PFA_TRACE, 0) && fprintf(PFA_TRACE, "\n"))
                   || (Variable_pretty(aj, PFA_TRACE, 1) && fprintf(PFA_TRACE, "\n")) );
    if (phiClass != class_aj) { // split
      Variable ajp = NULL;
      // Create ajp and coalesce to phiClass.
      // LivenessMode_Mixed.
      // Check if the copy already exists.
      pcj = BasicBlock_makeOperationLCOPY(Lj);
      Operation_RESULTS_FOREACH_Temporary(pcj, result) {
        Variable V_dst = Temporary_VARIABLE(result);
        VariableClass classDst = Variable_CONGRUENCE(V_dst);
        if (classDst == phiClass) {
          Except_CHECK(Variable_EQUIVALUE(aj) == Variable_EQUIVALUE(Temporary_VARIABLE(Operation_getArgument(pcj, Operation_RESULTS_INDEX))));
          ajp = V_dst;
          break;
        }
      } Operation_RESULTS_ENDEACH_Temporary;
      if (ajp == NULL) {
        // Create the copy.
        int resIndex;
        Operation copy;
        Temporary temp_ajp = CodeRegion_makeVariableTemporary(codeRegion, Variable_ORIGIN(aj));
        ajp = Temporary_VARIABLE(temp_ajp);
        TemporaryTable_insert(Coalescer_TEMPORARYTABLE(this), temp_ajp);
        if (pcj != NULL) {
          copy = pcj;
          resIndex = Operation_addtoParCopy(pcj, temp_ajp, temp_aj);
        } else {
          copy = BasicBlock_makeLateOperationCOPY(Lj, temp_ajp, temp_aj);
          resIndex = 0;
        }
        Variable_setOperationResult(ajp, copy, resIndex);
        VariableClass_insert(phiClass, ajp);
        if (Coalescer_CONSUMING(this)) Variable_raiseConsuming(ajp);
        Variable_setOperationArgument(aj, copy, resIndex);
        0 &&
        PFA_TRACE && fprintf(PFA_TRACE, "\t\t\t\t\tCOPY: ") && Operation_pretty(copy, PFA_TRACE);
      }
      // Update the phi operand
      Variable_setOperationArgument(ajp, phi, Operation_ARGUMENTS_INDEX);
      // Update live sets
      if (Coalescer_USELIVESETS>0) {
        // LivenessMode_Mixed.
        TemporarySet_insert(Coalescer_LIVEOUTSETS(this)+BasicBlock_index(Lj), ajp);
      }
      // Update interference graph
      if (Coalescer_INTERGRAPH>0) {
        Except_CHECK(Coalescer_USELIVESETS>0);
        // LivenessMode_Mixed.
        // ajp interferes with all variables liveout of the parallel copy
        Coalescer_addInterferenceVS(this, ajp, Coalescer_LIVEOUTSETS(this)+BasicBlock_index(Lj));
        // FIXME: addInterference with branching bool
      }
    } else { // merge
      // Update the live sets
      if (Coalescer_USELIVESETS>0) {
        TemporarySet_insert(Coalescer_LIVEOUTSETS(this)+BasicBlock_index(Lj), aj);
        // Update the interference graph
        if (Coalescer_INTERGRAPH>0) {
          // LivenessMode_Mixed.
          // Additional interference with variables defined by the pcopy.
          // Already interferes with LIVEOUTCLASS + branch bool.
          Coalescer_addInterferenceVS(this, aj, Coalescer_LIVEOUTSETS(this)+BasicBlock_index(Lj));
          // FIXME: restrict to PCOPY defs.
        } // else nothing to do.
      }
    }
  } Operation_ARGUMENTS_ENDEACH_Temporary;
  // L0 (j == 0)
  {
    Temporary temp_a0 = Operation_getResult(phi, 0);
    Variable a0 = Temporary_VARIABLE(temp_a0);
    VariableClass class_a0 = Variable_CONGRUENCE(a0);
    PFA_TRACE && fprintf(PFA_TRACE, "\t\t\t\t\tA0: ")
              && ( (class_a0 && VariableClass_pretty(class_a0, PFA_TRACE, 0) && fprintf(PFA_TRACE, "\n"))
                   || (Variable_pretty(a0, PFA_TRACE, 1) && fprintf(PFA_TRACE, "\n")) );
    if (class_a0 != phiClass) { //split
      // Create a0p and coalesce to phiClass
      int resIndex;
      Operation copy;
      Operation pc0 = NULL;
      Temporary temp_a0p = CodeRegion_makeVariableTemporary(codeRegion, Variable_ORIGIN(a0));
      Variable a0p = Temporary_VARIABLE(temp_a0p);
      TemporaryTable_insert(Coalescer_TEMPORARYTABLE(this), temp_a0p);
      // Create the copy
      // LivenessMode_Mixed.
      pc0 = BasicBlock_makeOperationECOPY(L0);
      copy = pc0;
      resIndex = Operation_addtoParCopy(pc0, temp_a0p, temp_a0);
      Variable_setOperationResult(a0, copy, resIndex);
      if (class_a0) Variable_updateCongruence(a0);
      Variable_setOperationArgument(a0p, copy, resIndex);
      Variable_setOperationResult(a0p, phi, 0);
      VariableClass_insert(phiClass, a0p);
      if (Coalescer_CONSUMING(this)) Variable_raiseConsuming(a0p);
      // Update the live sets
      if (Coalescer_USELIVESETS>0) {
        // LivenessMode_Mixed.
        TemporarySet_insert(Coalescer_LIVEINSETS(this)+BasicBlock_index(L0), a0p);
      }
      // update interference graph
      if (Coalescer_INTERGRAPH>0) {
        // LivenessMode_Mixed.
        // a0p interferes with all variables livein of the parallel copy
        Coalescer_addInterferenceVS(this, a0p, Coalescer_LIVEINSETS(this)+BasicBlock_index(L0));
      }
    } else { //merge
      // update live sets
      if (Coalescer_USELIVESETS>0) {
        // LivenessMode_Mixed.
        TemporarySet_insert(Coalescer_LIVEINSETS(this)+BasicBlock_index(L0), a0);
        // update interference graph
        if (Coalescer_INTERGRAPH>0) {
          // LivenessMode_Mixed.
          // Additional interference with variables used by the pcopy.
          // Already interferes with LIVEINCLASS.
          // FIXME: use pcopy uses instead this trick
          if (Coalescer_PHIDEFSETS(this)) {
            Coalescer_addInterferenceVS(this, a0, Coalescer_PHIDEFSETS(this)+BasicBlock_index(L0));
          } else {
            Coalescer_addInterferenceVS(this, a0, Coalescer_LIVEINSETS(this)+BasicBlock_index(L0));
          }
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Coalescer module.
 */
#if defined(Coalescer_INTERGRAPH) \
 && defined(Coalescer_EQUIVALUE) \
 && defined(Coalescer_DOMFOREST)
#define Coalescer_INIT()
#else
void
Coalescer_INIT(void);
#endif
#endif//$XCC__h

#ifndef Coalescer_INIT
void
Coalescer_INIT(void)
{
#ifndef Coalescer_USELIVESETS
  if (GETENV("Coalescer_USELIVESETS")) {
    sscanf(GETENV("Coalescer_USELIVESETS"), "%d", &Coalescer_USELIVESETS);
    fprintf(stderr, "*** Coalescer_USELIVESETS=%d\n", Coalescer_USELIVESETS);
  }
#endif//Coalescer_USELIVESETS
#ifndef Coalescer_INTERGRAPH
  if (GETENV("Coalescer_INTERGRAPH")) {
    sscanf(GETENV("Coalescer_INTERGRAPH"), "%d", &Coalescer_INTERGRAPH);
    fprintf(stderr, "*** Coalescer_INTERGRAPH=%d\n", Coalescer_INTERGRAPH);
  }
#endif//Coalescer_INTERGRAPH
#ifndef Coalescer_EQUIVALUE
  if (GETENV("Coalescer_EQUIVALUE")) {
    sscanf(GETENV("Coalescer_EQUIVALUE"), "%d", &Coalescer_EQUIVALUE);
    fprintf(stderr, "*** Coalescer_EQUIVALUE=%d\n", Coalescer_EQUIVALUE);
  }
#endif//Coalescer_EQUIVALUE
#ifndef Coalescer_DOMFOREST
  if (GETENV("Coalescer_DOMFOREST")) {
    sscanf(GETENV("Coalescer_DOMFOREST"), "%d", &Coalescer_DOMFOREST);
    fprintf(stderr, "*** Coalescer_DOMFOREST=%d\n", Coalescer_DOMFOREST);
  }
#endif//Coalescer_DOMFOREST
  // There is a lurking bug according to Fabrice -- BD3 2008-08-01.
  if (Coalescer_DOMFOREST > 0 && Coalescer_EQUIVALUE <= 0) {
    fprintf(stderr, "*** BUG ALERT! DOMFOREST only works with EQUIVALUE set!\n");
  }
}
#endif//Coalescer_INIT

#ifdef $XCC__h
/**
 * Finalize the Coalescer module.
 */
#define Coalescer_FINI()
#endif//$XCC__h

#if XCC__C
static void
Coalescer_TEST(void)
{
#include "PFA/Coalescer_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(Coalescer_TEST);
  return 0;
}
#endif

