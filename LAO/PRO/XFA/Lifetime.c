#ifdef $XCC_h
/*
 * !!!!	Lifetime.xcc
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
 * @brief Temporary Lifetime in a schedule.
 */
#endif//$XCC_h


#include "XFA/XFA_.h"

#ifdef $XCC__h
#include "XFA/Lifetime.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Lifetime flags.
 */
typedef enum {
  LifetimeFlag_OnlyUses = 0x1,		// Lifetime contains only uses.
  LifetimeFlag_Wrapping = 0x2,		// Lifetime has a wrapping effect.
  LifetimeFlag_Relaxable = 0x4,		// Lifetime has a relaxing def.
  LifetimeFlag_Complement = 0x8,	// Lifetime may be complemented.
  LifetimeFlag_Induction = 0x10,	// Lifetime has an induction def.
  LifetimeFlag_WriteOnce = 0x20,	// Lifetime has a single write.
  LifetimeFlag_ReadAfter = 0x40,	// Lifetime has all reads after write.
  LifetimeFlag_Renaming = 0x80,		// Lifetime has renaming registers.
  LifetimeFlag_InnerLoop = 0x100,	// Lifetime is for inner loop.
  LifetimeFlag_PrePass = 0x200,		// Lifetime is for prepass scheduling.
} enum_LifetimeFlag;
typedef uint16_t LifetimeFlags;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ LifetimeFlag_Table[1];
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ LifetimeFlag_Entries[] = {
  ETableENTRY(LifetimeFlag, OnlyUses),
  ETableENTRY(LifetimeFlag, Wrapping),
  ETableENTRY(LifetimeFlag, Relaxable),
  ETableENTRY(LifetimeFlag, Complement),
  ETableENTRY(LifetimeFlag, Induction),
  ETableENTRY(LifetimeFlag, WriteOnce),
  ETableENTRY(LifetimeFlag, ReadAfter),
  ETableENTRY(LifetimeFlag, Renaming),
  ETableENTRY(LifetimeFlag, InnerLoop),
  ETableENTRY(LifetimeFlag, PrePass),
  ETableENTRY_NULL
};
ETable_ LifetimeFlag_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Lifetime of a Temporary inside a BlockTrace.
 */
struct Lifetime_ {
  //@access NEXT	(Lifetime)IDListItem_NEXT(this)
  //@access PREV	(Lifetime)IDListItem_PREV(this)
  //@args	Memory memory, Temporary temporary, EffectSeq effectSeq,
  //@args	int firstIndex, unsigned lifetimeFlags
  Temporary TEMPORARY;		// Temporary for this Lifetime.
  //@access hasRegFile	Temporary_hasRegFile(Lifetime_TEMPORARY(this))
  EffectSeq EFFECTSEQ;		// The EffectSeq used by this Lifetime.
  int16_t FIRSTINDEX;		//
  int16_t LASTINDEX;		//
  LifetimeFlags FLAGS;
  //@access isOnlyUses	((Lifetime_FLAGS(this) & LifetimeFlag_OnlyUses) != 0)
  //@access isWrapping	((Lifetime_FLAGS(this) & LifetimeFlag_Wrapping) != 0)
  //@access hasRelaxable	((Lifetime_FLAGS(this) & LifetimeFlag_Relaxable) != 0)
  //@access hasComplement	((Lifetime_FLAGS(this) & LifetimeFlag_Complement) != 0)
  //@access isInduction	((Lifetime_FLAGS(this) & LifetimeFlag_Induction) != 0)
  //@access isWriteOnce	((Lifetime_FLAGS(this) & LifetimeFlag_WriteOnce) != 0)
  //@access isReadAfter	((Lifetime_FLAGS(this) & LifetimeFlag_ReadAfter) != 0)
  //@access hasRenaming	((Lifetime_FLAGS(this) & LifetimeFlag_Renaming) != 0)
  //@access isInnerLoop	((Lifetime_FLAGS(this) & LifetimeFlag_InnerLoop) != 0)
  //@access isPrePass	((Lifetime_FLAGS(this) & LifetimeFlag_PrePass) != 0)
  int16_t LIFESPAN;		//
  TemporaryStack_ RENAMESTACK_;
  //@access RENAMESTACK	Lifetime__RENAMESTACK_(this)
  int16_t RENAMECOUNT;		// Count of temporaries in this RENAMESTACK.
  int16_t DEFINSTANCE;		// Last def instance number when renaming this Lifetime.
  Temporary DEFINITION;		// Last Temporary def seen when renaming this Lifetime.
  Effect STEPEFFECT;		// Step Effect in case of simple inductions.
};
#endif//$XCC_h

Lifetime
Lifetime_Ctor(Lifetime this,
              Memory memory, Temporary temporary, EffectSeq effectSeq,
              int firstIndex, unsigned lifetimeFlags)
{
  *Lifetime__TEMPORARY(this) = temporary;
  *Lifetime__EFFECTSEQ(this) = effectSeq;
  *Lifetime__FIRSTINDEX(this) = firstIndex;
  *Lifetime__LASTINDEX(this) = -1;
  *Lifetime__FLAGS(this) = lifetimeFlags;
  *Lifetime__LIFESPAN(this) = -1;
  TemporaryStack_Ctor(Lifetime_RENAMESTACK(this), memory, 0);
  *Lifetime__RENAMECOUNT(this) = 0;
  *Lifetime__DEFINSTANCE(this) = -1;
  *Lifetime__DEFINITION(this) = NULL;
  *Lifetime__STEPEFFECT(this) = NULL;
  return this;
}

void
Lifetime_Dtor(Lifetime this)
{
  TemporaryStack_Dtor(Lifetime_RENAMESTACK(this));
}

size_t
Lifetime_Size(Memory memory, Temporary temporary, EffectSeq effectSeq,
              int firstIndex, unsigned lifetimeFlags)
{
  return sizeof(Lifetime_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Lifetime_)\t%zu\n", sizeof(Lifetime_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * The first Effect pointer of this Lifetime.
 */
static inline Effect *
Lifetime__firstEffect(Lifetime this)
{
  int32_t firstIndex = Lifetime_FIRSTINDEX(this);
  return EffectSeq_base(Lifetime_EFFECTSEQ(this)) + firstIndex;
}
#define Lifetime_firstEffect(this)	(*Lifetime__firstEffect(this))
#endif//$XCC__h

#ifdef $XCC__h
/**
 * The last Effect pointer of this Lifetime.
 */
static inline Effect *
Lifetime__lastEffect(Lifetime this)
{
  int32_t lastIndex = Lifetime_LASTINDEX(this);
  return EffectSeq_base(Lifetime_EFFECTSEQ(this)) + lastIndex;
}
#define Lifetime_lastEffect(this)	(*Lifetime__lastEffect(this))
#endif//$XCC__h

#ifdef $XCC_h
/**
 * Iterate over this Lifetime Effects.
 */
#define Lifetime_FOREACH_Effect(this, effect) { \
  int32_t Lifetime_INDEX = Lifetime_FIRSTINDEX(this); \
  int32_t Lifetime_LASTINDEX = Lifetime_LASTINDEX(this); \
  EffectSeq Lifetime_EFFECTSEQ = Lifetime_EFFECTSEQ(this); \
  Effect *Lifetime_BASE = EffectSeq_base(Lifetime_EFFECTSEQ); \
  for (; Lifetime_INDEX <= Lifetime_LASTINDEX; Lifetime_INDEX++) { \
    Effect effect = Lifetime_BASE[Lifetime_INDEX];
#define Lifetime_ENDEACH_Effect \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Lifetime Effects.
 *
 * This iterator works even if Lifetime_EFFECTSEQ is modified.
 */
#define Lifetime_FORSAFE_Effect(this, effect) { \
  int32_t Lifetime_INDEX = Lifetime_FIRSTINDEX(this); \
  int32_t Lifetime_LASTINDEX = Lifetime_LASTINDEX(this); \
  EffectSeq Lifetime_EFFECTSEQ = Lifetime_EFFECTSEQ(this); \
  for (; Lifetime_INDEX <= Lifetime_LASTINDEX; Lifetime_INDEX++) { \
    Effect effect = EffectSeq_base(Lifetime_EFFECTSEQ)[Lifetime_INDEX];
#define Lifetime_ENDSAFE_Effect \
  } \
}
#endif//$XCC_h

#ifdef $XCC__h
/**
 * Find if this Lifetime may be ignored.
 *
 * We may ignore the Lifetime(s) that only have clobber or side effects.
 */
bool
Lifetime_mayIgnore(Lifetime this);
#endif//$XCC__h

bool
Lifetime_mayIgnore(Lifetime this)
{
  Temporary temporary = Lifetime_TEMPORARY(this);
  //if (Temporary_isWired(temporary)) return true;
  if (Temporary_hasRegFile(temporary)) {
    Lifetime_FOREACH_Effect(this, effect) {
      if (!Effect_isSide(effect) &&
          !Effect_isClobber(effect)) return false;
    } Lifetime_ENDEACH_Effect;
    // Only side or clobber effects.
    return true;
  } else if (Lifetime_isOnlyUses(this)) {
    if (!Temporary_isMemory(temporary)) return true;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Pretty print of this Lifetime.
 */
bool
Lifetime_pretty(Lifetime this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Lifetime_pretty(Lifetime this, FILE *file)
{
  Temporary temporary = Lifetime_TEMPORARY(this);
  Effect stepEffect = Lifetime_STEPEFFECT(this);
  LifetimeFlags lifetimeFlags = Lifetime_FLAGS(this);
  if (file == NULL) file = stderr;
  fprintf(file, "    Lifetime(");
  Temporary_pretty(temporary, file);
  fprintf(file, ")\t");
  ETable_prettyFlags(LifetimeFlag_Table, lifetimeFlags, "+", file);
  if (Lifetime_mayIgnore(this)) fprintf(file, "\t(may ignore)");
  if (stepEffect != NULL) {
    Operation step_operation = Effect_OPERATION(stepEffect);
    Temporary step_temporary = Effect_TEMPORARY(stepEffect);
    fprintf(file, "\tstep=");
    Temporary_pretty(step_temporary, file);
  }
  if (Temporary_isRemater(temporary)) {
    fprintf(file, "\t(rematerializable)");
  }
  fprintf(file, "\n");
  Lifetime_FOREACH_Effect(this, effect) {
    fprintf(file, "\t");
    Effect_pretty(effect, file);
    fprintf(file, "\n");
  } Lifetime_ENDEACH_Effect;
  return true;
}
#endif//_NTRACE

#ifdef $XCC__h
/**
 * Compare function for sorting the Lifetime(s).
 */
int
Lifetime_compare(const void *this_pointer, const void *that_pointer);
#endif//$XCC__h

int
Lifetime_compare(const void *this_pointer, const void *that_pointer)
{
  const_Lifetime this = this_pointer;
  const_Lifetime that = that_pointer;
  Temporary this_temporary = Lifetime_TEMPORARY(this);
  Temporary that_temporary = Lifetime_TEMPORARY(that);
  int result = Temporary_compare(this_temporary, that_temporary);
  if (result != 0) return result;
  return Lifetime_FIRSTINDEX(this) - Lifetime_FIRSTINDEX(that);
}

#ifdef $XCC__h
/**
 * Complete this Lifetime.
 *
 * Completion rotates the effects of this Lifetime so the starting effect is a
 * Def (Write) if there is any. For all the Lifetimes that are not on register with
 * renaming, the first Def is replicated at the end of this Lifetime effects.
 */
void
Lifetime_complete(Lifetime this, Optimize optimize, Configure configure, Processor processor);
#endif//$XCC__h

void
Lifetime_complete(Lifetime this, Optimize optimize, Configure configure, Processor processor)
{
  Temporary temporary = Lifetime_TEMPORARY(this);
  if (Lifetime_hasRenaming(this)) {
    // Renaming register: assume the Write is first.
    Effect firstEffect = Lifetime_firstEffect(this);
    if (Effect_isRead(firstEffect)) {
      *Lifetime__FLAGS(this) |= LifetimeFlag_OnlyUses;
    }
  } else {
    // Case without renaming registers.
    EffectSeq EFFECTSEQ = Lifetime_EFFECTSEQ(this);
    int firstIndex = -1, lastIndex = -1, INDEX = 0;
    Except_CHECK(INDEX == Lifetime_FIRSTINDEX(this));
    // Rotate this lifetime so that its first Effect is a Write.
    // Also ensure that the last effect is a duplicate of the first Effect.
    Lifetime_FORSAFE_Effect(this, effect) {
      bool isWrite = Effect_isWrite(effect);
      EffectSeq_pushEffect(EFFECTSEQ, effect);
      if (isWrite) {
        // Found the first Effect.
        firstIndex = INDEX;
        break;
      }
      INDEX++;
    } Lifetime_ENDSAFE_Effect;
    // Compute this Lifetime attributes.
    if (firstIndex >= 0) {
      // First Effect was found in the loop above.
      lastIndex = EffectSeq_count(EFFECTSEQ) - 1;
      *Lifetime__FIRSTINDEX(this) = firstIndex;
      *Lifetime__LASTINDEX(this) = lastIndex;
      *Lifetime__FLAGS(this) |= LifetimeFlag_Wrapping;
    } else {
      *Lifetime__FLAGS(this) |= LifetimeFlag_OnlyUses;
    }
  }
  if (!Lifetime_isOnlyUses(this)) {
    Lifetime_analyze(this, optimize, configure, processor);
    if (Lifetime_isInduction(this)) {
      Lifetime_setSimpleInductionFlags(this, optimize, configure, processor);
    }
  }
  if (Lifetime_isPrePass(this)) {
    Lifetime_setMemoryLevel2ing(this, optimize, configure, processor);
  }
}

#ifdef $XCC__h
/**
 * Analyze this Lifetime and set attributes.
 */
void
Lifetime_analyze(Lifetime this, Optimize optimize, Configure configure, Processor processor);
#endif//$XCC__h

void
Lifetime_analyze(Lifetime this, Optimize optimize, Configure configure, Processor processor)
{
  Temporary temporary = Lifetime_TEMPORARY(this);
  Except_CHECK(!Lifetime_isOnlyUses(this));
  if (Temporary_hasRegFile(temporary)) {
    // Compute the Carried, ReadAfter, and WriteOnce flags.
    Effect firstEffect = Lifetime_firstEffect(this);
    Operation first_operation = Effect_OPERATION(firstEffect);
    int32_t first_index = Operation_index(first_operation);
    Operator first_operator = Operation_operator(first_operation);
    Operation ignore_operation = Operator_isLabel(first_operator)?
                                 first_operation: NULL;
    Operation write_operation = NULL;
    LifetimeFlags readAfter = LifetimeFlag_ReadAfter;
    LifetimeFlags writeOnce = LifetimeFlag_WriteOnce;
    LifetimeFlags complement = LifetimeFlag_Complement;
    Lifetime_FOREACH_Effect(this, effect) {
      Operation operation = Effect_OPERATION(effect);
      if (Effect_isWrite(effect)) {
        if (operation != ignore_operation) {
          if (write_operation == NULL) write_operation = operation;
          if (operation != write_operation) writeOnce = 0;
          if (   !Effect_isKill(effect)
              && first_index > Operation_index(operation)) {
            *Effect__FLAGS(effect) |= EffectFlag_Carried;
          }
        }
      } else /* Effect_isRead(effect) */ {
        if (first_index >= Operation_index(operation)) {
          *Effect__FLAGS(effect) |= EffectFlag_Carried;
          readAfter = 0;
        }
      }
      // Record which Lifetime the effect is associated to.
      *Effect__LIFETIME(effect) = this;
    } Lifetime_ENDEACH_Effect;
    // Compute the Complement flag.
    if (complement) {
      Lifetime_FOREACH_Effect(this, effect) {
        Operation operation = Effect_OPERATION(effect);
        int xxxIndex  = Effect_XXXINDEX(effect);
        if (Effect_isWrite(effect)) {
          if (Operation_mayComplementResult(operation, processor, xxxIndex));
          else complement = 0;
        } else /* Effect_isRead(effect) */ {
          if (Operation_mayComplementArgument(operation, processor, xxxIndex));
          else complement = 0;
        }
        if (!complement) break;
      } Lifetime_ENDEACH_Effect;
    }
    // Set this Lifetime flags.
    *Lifetime__FLAGS(this) |= readAfter | writeOnce | complement;
    if (writeOnce) {
      // Find and set the STEPEFFECT of this Lifetime.
      Lifetime_FOREACH_Effect(this, effect) {
        Operation operation = Effect_OPERATION(effect);
        if (Effect_isKill(effect)) {
          Effect_ *argEffects = Operation_argEffects(operation);
          int32_t stepIndex = Operation_getStepIndex(operation);
          if (stepIndex >= 0) {
            *Lifetime__STEPEFFECT(this) = argEffects + stepIndex;
          }
          break;
        }
      } Lifetime_ENDEACH_Effect;
      if (Lifetime_STEPEFFECT(this) != NULL) {
        // Detect a simple induction even in case of block scheduling.
        Effect stepEffect = Lifetime_STEPEFFECT(this);
        Operation stepOperation = Effect_OPERATION(stepEffect);
        Lifetime_FOREACH_Effect(this, effect) {
          Operation operation = Effect_OPERATION(effect);
          if (!Effect_isWrite(effect)) {
            if (operation == stepOperation) {
              // Induction since the increment operation also uses this temporary.
              // An Inductive operation will appear later for sequential semantics.
              // This works because the increment operation has no relaxable use
              // itself, and also because it defines the induction.
              InstructionTask task = Operation_task(operation);
              *InstructionTask__FLAGS(task) |= InstructionFlag_Inductive;
              *Lifetime__FLAGS(this) |= LifetimeFlag_Induction;
            }
          }
        } Lifetime_ENDEACH_Effect;
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Set Lifetime and Operation flags for simple inductions.
 *
 * A simple induction is detected even in case of block scheduling. However in
 * such cases only the uses after the induction update will be relaxable. This is
 * because the Label operation introduces a fake definition in the Lifetime.
 */
void
Lifetime_setSimpleInductionFlags(Lifetime this, Optimize optimize, Configure configure, Processor processor);
#endif//$XCC__h

void
Lifetime_setSimpleInductionFlags(Lifetime this, Optimize optimize, Configure configure, Processor processor)
{
  Effect stepEffect = Lifetime_STEPEFFECT(this);
  Operation stepOperation = Effect_OPERATION(stepEffect);
  int speculation = Configure_SPECULATION(configure);
  int relaxation = Configure_RELAXATION(configure);
  int overlap = Configure_OVERLAP(configure);
  int prePadding = Optimize_PREPADDING(optimize);
  int postPadding = Optimize_POSTPADDING(optimize);
  bool padding = postPadding > 0 || prePadding > 0;
  Temporary step_temporary = Effect_TEMPORARY(stepEffect);
  Literal step_literal = Temporary_LITERAL(step_temporary);
  int64_t step = Literal_VALUE(step_literal);
  int64_t distance = overlap*step;
  bool relaxationOK = false;
  // Find the uses by the increment operation.
  Lifetime_FOREACH_Effect(this, effect) {
    Operation operation = Effect_OPERATION(effect);
    if (Effect_isWrite(effect)) {
      relaxationOK =  relaxation > ConfigureRelaxation_None
                   && operation == stepOperation;
    } else /* Effect_isRead(effect) */ {
      bool omega = Effect_isCarried(effect);
      int regIndex = Effect_XXXINDEX(effect);
      if (operation != stepOperation) {
        if (relaxationOK && regIndex >= 0 && overlap - omega >= 0) {
          // Candidate operation for inductive relaxation.
          int relaxable = Operation_getRelaxable(operation, regIndex, overlap - omega, step);
          *Lifetime__FLAGS(this) |= LifetimeFlag_Relaxable;
          *Effect__RELAXABLE(effect) = relaxable;
        }
        if (   speculation >= ConfigureSpeculation_MemorySafe
            && padding && !Operation_isMemorySafe(operation)
            && Operation_isPaddedLoad(operation, prePadding, postPadding, distance)) {
          Operation_setFlags(operation, OperationFlag_MemorySafe);
          if (   Operation_maySpeculate(operation, processor, speculation)
              && !Operation_hasDedicatedResult(operation, false)) {
#ifdef _DEBUG
            if (DEBUG_SPECULABLE && DEBUG_SPECULABLE--)
#endif//_DEBUG
            {
              InstructionTask task = Operation_task(operation);
              *InstructionTask__FLAGS(task) |= InstructionFlag_Speculable;
              // FIXME! If Operation_boosting then boosting should be undone.
            }
          }
        }
      }
    }
  } Lifetime_ENDEACH_Effect;
}

#ifdef $XCC__h
/**
 * Set Operation_isMemoryLevel2
 */
void
Lifetime_setMemoryLevel2ing(Lifetime this, Optimize optimize, Configure configure, Processor processor);
#endif//$XCC__h

void
Lifetime_setMemoryLevel2ing(Lifetime this, Optimize optimize, Configure configure, Processor processor)
{
  Effect stepEffect = Lifetime_STEPEFFECT(this);
  Temporary temporary = Lifetime_TEMPORARY(this);
  int preLoading = Configure_PRELOADING(configure);
  bool isInnerLoop = Lifetime_isInnerLoop(this);
  bool mayIndirect = Temporary_hasRegFile(temporary);
  bool mayVariant = mayIndirect && !Lifetime_isOnlyUses(this) && isInnerLoop;
  bool maySimple = mayVariant && stepEffect != NULL;
  Except_CHECK(Lifetime_isPrePass(this));
  if (   (preLoading <= ConfigurePreLoading_None)
      || (preLoading <= ConfigurePreLoading_Simple && !maySimple)
      || (preLoading <= ConfigurePreLoading_Variant && !mayVariant)
      || (preLoading <= ConfigurePreLoading_Indirect && !mayIndirect)) return;
  Lifetime_FOREACH_Effect(this, effect) {
    Operation operation = Effect_OPERATION(effect);
    int regIndex = Effect_XXXINDEX(effect);
    if (  !Effect_isWrite(effect)
        && Operation_mayMemoryLevel2(operation, regIndex)) {
      Operation_setFlags(operation, OperationFlag_MemoryLevel2);
    }
  } Lifetime_ENDEACH_Effect;
}

#ifdef $XCC__h
/**
 * Link this Lifetime Effect PRED and SUCC fields for
 * later use by dependence graph construction.
 *
 * The linking of effects crates a poor man's Def-Use and Use-Def chains within a
 * Lifetime (which is local to a BlockTrace). The Defs (Write) are double-linked
 * together. The Uses (non-Write) are linked to the previous and the next Defs.
 */
void
Lifetime_linkEffects(Lifetime this);
#endif//$XCC__h

void
Lifetime_linkEffects(Lifetime this)
{
  Effect *_firstEffect = Lifetime__firstEffect(this);
  Effect *_lastEffect = Lifetime__lastEffect(this);
  Effect *_effect1 = _firstEffect;
  // Iterate over the Def *_effect1.
  while (_effect1 <= _lastEffect) {
    Effect *_effect2 = _effect1 + 1, *_effect3 = NULL;
    Except_CHECK(Effect_isWrite(*_effect1));
    // Iterate over the Use *_effect2.
    while (_effect2 <= _lastEffect && !Effect_isWrite(*_effect2)) {
      *Effect__PRED(*_effect2) = *_effect1;
      _effect2++;
    }
    // Next Def is *_effect3.
    _effect3 = _effect2;
    if (_effect3 <= _lastEffect) {
      Effect *_effect4 = _effect1 + 1;
      *Effect__SUCC(*_effect1) = *_effect3;
      *Effect__PRED(*_effect3) = *_effect1;
      Except_CHECK(Effect_isWrite(*_effect3));
      // Iterate over the Use *_effect4.
      while (_effect4 < _effect3) {
        *Effect__SUCC(*_effect4) = *_effect3;
        _effect4++;
      }
    }
    _effect1 = _effect3;
  }
}

#ifdef $XCC__h
/**
 * Relink this Lifetime Effect PRED and SUCC
 * fields according to the supplied ddgNodes passed in.
 *
 * The supplied operation set contains the tails and the head operation of the
 * dependences that are already accounted for, such as the externally supplied
 * memory dependences. The purpose of Lifetime_relinkEffects is to reroute the
 * PRED and SUCC chains around the supplied operations.
 */
void
Lifetime_relinkEffects(Lifetime this, PtrSet ddgNodes);
#endif//$XCC__h

void
Lifetime_relinkEffects(Lifetime this, PtrSet ddgNodes)
{
  bool flagAll = true;
  // Scan the effects to flag those that are in the supplied ddgNodes.
  Lifetime_FOREACH_Effect(this, effect) {
    Operation OPERATION = Effect_OPERATION(effect);
    if (PtrSet_contains(ddgNodes, OPERATION)) {
      *Effect__FLAGS(effect) |= EffectFlag_Supplied;
    } else {
      *Effect__FLAGS(effect) &= ~EffectFlag_Supplied;
      flagAll = false;
    }
  } Lifetime_ENDEACH_Effect;
  if (flagAll) {
    // All the Effects are flagged: unlink all the PRED and SUCC fields.
    Lifetime_FOREACH_Effect(this, effect) {
      *Effect__PRED(effect) = *Effect__SUCC(effect) = NULL;
    } Lifetime_ENDEACH_Effect;
  } else {
    // Some Effects are flagged: relink their PRED and SUCC fields.
    bool relink = false;
    do {
      relink = false;
      Lifetime_FOREACH_Effect(this, effect) {
        Effect pred_effect = Effect_PRED(effect);
        Effect succ_effect = Effect_SUCC(effect);
        if (pred_effect != NULL && Effect_isSupplied(pred_effect)) {
          *Effect__PRED(effect) = Effect_PRED(pred_effect);
          if (effect == pred_effect) {
            *Effect__PRED(effect) = NULL;
          }
          relink = true;
        }
        if (succ_effect != NULL && Effect_isSupplied(succ_effect)) {
          *Effect__SUCC(effect) = Effect_SUCC(succ_effect);
          if (effect == succ_effect) {
            *Effect__SUCC(effect) = NULL;
          }
          relink = true;
        }
      } Lifetime_ENDEACH_Effect;
    } while (relink);

    Lifetime_FOREACH_Effect(this, effect) {
      if (Effect_isSupplied(effect)) {
        *Effect__PRED(effect) = NULL;
        *Effect__SUCC(effect) = NULL;
      }
    } Lifetime_ENDEACH_Effect;
  }
}

#ifdef $XCC__h
/**
 * Compute and return this Lifetime LIFESPAN.
 *
 * The lifespan is assumed to start at the earliest start date, and stop at the
 * latest start date in this Lifetime. TODO: correct the lifespan value depending
 * on the operand read and write time offsets.
 */
int
Lifetime_computeLifespan(Lifetime this, int16_t lambda, bool isAcyclic);
#endif//$XCC__h

int
Lifetime_computeLifespan(Lifetime this, int16_t lambda, bool isAcyclic)
{
  int minDate = INT_MAX/2, maxDate = INT_MIN/2;
  bool hasRenaming = Lifetime_hasRenaming(this);
  Temporary temporary = Lifetime_TEMPORARY(this);
  if (Temporary_isLifetime(temporary)) {
    if (Lifetime_isOnlyUses(this)) {
      minDate = 0, maxDate = lambda;
    } else {
      Lifetime_FOREACH_Effect(this, effect) {
        if (!Effect_RELAXABLE(effect)) {
          Operation operation = Effect_OPERATION(effect);
          IssueItem item = Operation_issueItem(operation);
          int date = IssueItem_STARTDATE(item);
          if (Effect_isRead(effect) && hasRenaming) {
            // Extend the use in case of renaming because there are no WAR.
            date += Effect_getRegisterExtraWARLatency(effect);
          }
          if (Effect_isCarried(effect)) {
            if (isAcyclic) continue;
            // Assume OMEGA is 1 for loop-carried register dependences.
            date += lambda;
          }
          minDate = _MIN(date, minDate);
          maxDate = _MAX(date, maxDate);
        }
      } Lifetime_ENDEACH_Effect;
      Except_CHECK(maxDate >= minDate);
    }
    *Lifetime__LIFESPAN(this) = maxDate - minDate;
    PFA_TRACE && fprintf(PFA_TRACE, "    ") && Temporary_pretty(temporary, PFA_TRACE);
    PFA_TRACE && fprintf(PFA_TRACE, ":\tlifespan=%d\tmindate=%d\tmaxdate=%d\n",
        Lifetime_LIFESPAN(this), minDate, maxDate);
    return Lifetime_LIFESPAN(this);
  }
  return -1;
}

#ifdef $XCC__h
/**
 * Make the renaming Temporaries needed by modulo expansion.
 *
 * Require that this Lifetime be inlined within an IDList sorted with Lifetime_compare.
 */
void
Lifetime_fillRenaming(Lifetime this, Procedure procedure, int lambda, int kunroll);
#endif//$XCC__h

void
Lifetime_fillRenaming(Lifetime this, Procedure procedure, int lambda, int kunroll)
{
  Lifetime next_lifetime = Lifetime_NEXT(this);
  Temporary temporary = Lifetime_TEMPORARY(this);
  TemporaryStack renameStack = Lifetime_RENAMESTACK(this);
  // Set isLast when OK to reuse temporary for renaming.
  bool isLast = next_lifetime == NULL ||
      Lifetime_TEMPORARY(next_lifetime) != temporary;
  int lifespan = Lifetime_LIFESPAN(this), i = 0;
  int renameCount = (lifespan + lambda - 1)/lambda;
  if (renameCount == 0) renameCount = 1;
  else if (kunroll%renameCount != 0) renameCount = kunroll;
  PFA_TRACE && fprintf(PFA_TRACE, "    ") && Temporary_pretty(temporary, PFA_TRACE);
  PFA_TRACE && fprintf(PFA_TRACE, ":\tlifespan=%d", lifespan);
  for (i = 0; i < renameCount - isLast; ++i) {
    Temporary new_temporary = Procedure_copyVirtualTemporary(procedure, temporary);
    TemporaryStack_push2(renameStack, new_temporary);	// HACK! TemporaryStack_push.
    PFA_TRACE && fprintf(PFA_TRACE, "\t") && Temporary_pretty(new_temporary, PFA_TRACE);
  }
  if (isLast) {
    TemporaryStack_push2(renameStack, temporary);	// HACK! TemporaryStack_push.
    PFA_TRACE && fprintf(PFA_TRACE, "\t") && Temporary_pretty(temporary, PFA_TRACE);
  }
  *Lifetime__RENAMECOUNT(this) = TemporaryStack_count(renameStack);
  PFA_TRACE && fprintf(PFA_TRACE, "\n");
}

#ifdef $XCC_h
/**
 * Get the renaming Temporary for a given instance.
 */
Temporary
Lifetime_getRename(Lifetime this, int instance);
#endif//$XCC_h

Temporary
Lifetime_getRename(Lifetime this, int instance)
{
  TemporaryStack renameStack = Lifetime_RENAMESTACK(this);
  int renameCount = Lifetime_RENAMECOUNT(this);
  int index = renameCount > 1? (instance + renameCount)%renameCount: 0;
  return TemporaryStack_access(renameStack, index);
}

#ifdef $XCC_h
/**
 * Set the Temporary INDEX of this Lifetime.
 *
 * Set the INDEX of this Lifetime_TEMPORARY for use in TemporaryMap. In case of
 * an assigned or a dedicated temporary, all the Effect_TEMPORARY must have their
 * INDEX set to the same value as well.
 */
void
Lifetime_setTemporaryIndex(Lifetime this, int32_t index);
#endif//$XCC_h

void
Lifetime_setTemporaryIndex(Lifetime this, int32_t index)
{
  Temporary temporary = Lifetime_TEMPORARY(this);
  if (Temporary_hasRegister(temporary)) {
    // The lifetime may contain different temporary with the same register.
    Lifetime_FOREACH_Effect(this, effect) {
      Temporary temporary = Effect_TEMPORARY(effect);
      *Temporary__INDEX(temporary) = index;
    } Lifetime_ENDEACH_Effect;
  }
  *Temporary__INDEX(temporary) = index;
}

#ifdef $XCC__h
/**
 * Find the Lifetime that reaches an operation argument.
 *
 * Require that this Lifetime be inlined within an IDList sorted with Lifetime_compare.
 */
Lifetime
Lifetime_findReachingLifetime(Lifetime this, Operation operation, int argIndex);
#endif//$XCC__h

#if 0
Lifetime
Lifetime_findReachingLifetime(Lifetime this, Operation operation, int argIndex)
{
  Lifetime lifetime = this;
  Temporary temporary = Lifetime_TEMPORARY(this);
  Lifetime next_lifetime = Lifetime_NEXT(lifetime);
  while (next_lifetime != NULL && Lifetime_TEMPORARY(next_lifetime) == temporary) {
    // Check if lifetime is reaching the operation argument.
    Lifetime_FOREACH_Effect(lifetime, effect) {
      if (Effect_isRead(effect) &&
          Effect_OPERATION(effect) == operation &&
          Effect_XXXINDEX(effect) == argIndex) break;
    } Lifetime_ENDEACH_Effect;
    lifetime = next_lifetime;
    next_lifetime = Lifetime_NEXT(lifetime);
  }
  return lifetime;
}
#endif

/*XCC_.h
 * Lifetime_getMemoryDistance --	Call Operation_getMemoryDistance for a pair of base address effects.
 */
int
Lifetime_getMemoryDistance(Lifetime this, Effect tail_effect, Effect head_effect, int overlap);

int
Lifetime_getMemoryDistance(Lifetime this, Effect tail_effect, Effect head_effect, int overlap)
{
  Effect stepEffect = Lifetime_STEPEFFECT(this);
  Operation tail_operation = Effect_OPERATION(tail_effect);
  Operation head_operation = Effect_OPERATION(head_effect);
  if (stepEffect != NULL) {
    // Case of simple induction, can recompute dependence distance.
    Temporary step_temporary = Effect_TEMPORARY(stepEffect);
    Literal step_literal = Temporary_LITERAL(step_temporary);
    int64_t step = Literal_VALUE(step_literal);
    int adjust = Effect_isCarried(tail_effect) - Effect_isCarried(head_effect);
    PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\tstep=%0"PRId64"\tadjust=%d",
        Operation_index(tail_operation), Operation_index(head_operation), step, adjust);
    return Operation_getMemoryDistance(tail_operation, head_operation, adjust, overlap, step);
  } else {
    // Not a simple induction, only valid to disambiguate loop-independent dependences.
    int write_ordering = 0, tail_ordering = -1, head_ordering = -1;
    if (!Lifetime_isOnlyUses(this)) {
      // Find the write ordering of each use.
      Lifetime_FOREACH_Effect(this, effect) {
        if (Effect_isWrite(effect)) {
          write_ordering++;
        } else {
          if (effect == tail_effect) tail_ordering = write_ordering;
          if (effect == head_effect) head_ordering = write_ordering;
        }
      } Lifetime_ENDEACH_Effect;
    }
    if (tail_ordering == head_ordering) {
      // The two Effect(s) use the value produced by the same Write.
      PFA_TRACE && fprintf(PFA_TRACE, "\t(_%d, _%d)\tstep=%lld\tadjust=%d",
          Operation_index(tail_operation), Operation_index(head_operation), 0ll, 0);
      return Operation_getMemoryDistance(tail_operation, head_operation, 0, 0ll, 0);
    }
  }
  return -1;
}

/*XCC_.h
 * Lifetime_findEffect --        Find the effect in the lifetime that corresponds to effect.
 */
Effect
Lifetime_findEffect(Lifetime this, Effect effect);

Effect
Lifetime_findEffect(Lifetime this, Effect effect)
{
  Operation operation = Effect_OPERATION(effect);
  int32_t index = Operation_index(operation);
  bool isRead = Effect_isRead(effect);
  int xxxIndex = Effect_XXXINDEX(effect);
  Lifetime_FOREACH_Effect(this, lifeEffect) {
    if (   isRead == Effect_isRead(lifeEffect)
        && xxxIndex == Effect_XXXINDEX(lifeEffect)) {
      Operation OPERATION = Effect_OPERATION(lifeEffect);
      if (index == Operation_index(OPERATION)) {
        return lifeEffect;
      }
    }
  } Lifetime_ENDEACH_Effect;
  return NULL;
}

#ifdef $XCC_h
/**
 * Fix a result effect for modulo expansion and induction relaxation.
 */
void
Lifetime_fixResult(Lifetime this, Procedure procedure, Effect effect, int instance);
#endif//$XCC_h

void
Lifetime_fixResult(Lifetime this, Procedure procedure, Effect effect, int instance)
{
  Except_CHECK(Effect_isWrite(effect));
  if (Lifetime_hasRenaming(this)) {
    Operation operation = Effect_OPERATION(effect);
    int resIndex = Effect_XXXINDEX(effect);
    Effect lifeEffect = Lifetime_findEffect(this, effect);
    int isCarried = Effect_isCarried(lifeEffect);
    int rename_instance = instance - isCarried;
    Temporary temporary = Lifetime_getRename(this, rename_instance);
    Operation_setResult(operation, resIndex, temporary);
  }
  if (Lifetime_isInduction(this) && Lifetime_hasRelaxable(this)) {
    *Lifetime__DEFINSTANCE(this) = instance;
  }
}

#ifdef $XCC_h
/**
 * Fix an argument effect for modulo expansion and induction relaxation.
 */
void
Lifetime_fixArgument(Lifetime this, Procedure procedure, Effect effect, int instance);
#endif//$XCC_h

void
Lifetime_fixArgument(Lifetime this, Procedure procedure, Effect effect, int instance)
{
  Operation operation = Effect_OPERATION(effect);
  int argIndex = Effect_XXXINDEX(effect);
  Except_CHECK(Effect_isRead(effect));
  if (Lifetime_isInduction(this) && Lifetime_hasRelaxable(this)) {
    Effect lifeEffect = Lifetime_findEffect(this, effect);
    if (Effect_RELAXABLE(lifeEffect)) {
      int isCarried = Effect_isCarried(lifeEffect);
      int rename_instance = instance - isCarried;
      int def_instance = Lifetime_DEFINSTANCE(this);
      if (Lifetime_hasRenaming(this)) {
        Temporary temporary = Lifetime_getRename(this, def_instance);
        Operation_setArgument(operation, argIndex, temporary);
      }
      rename_instance -= def_instance;
      if (rename_instance != 0) {
        Effect stepEffect = Lifetime_STEPEFFECT(this);
        Temporary step_temporary = Effect_TEMPORARY(stepEffect);
        Literal step_literal = Temporary_LITERAL(step_temporary);
        int64_t step = Literal_VALUE(step_literal);
        Procedure_updateRelaxedOperation(procedure, operation, argIndex, step, rename_instance);
        PFA_TRACE && fprintf(PFA_TRACE, "*** RELAXED(%d)\t", rename_instance);
        PFA_TRACE && Operation_pretty(operation, PFA_TRACE);
      }
      return;
    }
  }
  if (Lifetime_hasRenaming(this)) {
    Effect lifeEffect = Lifetime_findEffect(this, effect);
    int isCarried = Effect_isCarried(lifeEffect);
    int rename_instance = instance - isCarried;
    Temporary temporary = Lifetime_getRename(this, rename_instance);
    Operation_setArgument(operation, argIndex, temporary);
  }
  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Table of Lifetime(s).
 */
struct LifetimeTable_;
  //@args	Memory memory, Optimize optimize, Configure configure
#endif//$XCC_h

#ifdef $XCC__h
struct LifetimeTable_ {
  IDList_ LIFETIMES_;		//< The IDList where the Lifetime(s) are inlined.
  //@access LIFETIMES	LifetimeTable__LIFETIMES_(this)
  //@access MEMORY	IDList_memory(LifetimeTable_LIFETIMES(this))
  //@access COUNT	IDList_count(LifetimeTable_LIFETIMES(this))
  //@access FIRST	(Lifetime)(IDList_firstItem(LifetimeTable_LIFETIMES(this)))
  //@access LAST	(Lifetime)(IDList_lastItem(LifetimeTable_LIFETIMES(this)))
  TemporaryMap_ TEMPORARYMAP_;	//< Map from Temporary INDEX to Lifetime.
  //@access TEMPORARYMAP	LifetimeTable__TEMPORARYMAP_(this)
  Optimize OPTIMIZE;
  Configure CONFIGURE;
};
#endif//$XCC__h

LifetimeTable
LifetimeTable_Ctor(LifetimeTable this,
                   Memory memory, Optimize optimize, Configure configure)
{
  IDList_Ctor(LifetimeTable_LIFETIMES(this), memory);
  TemporaryMap_Ctor(LifetimeTable_TEMPORARYMAP(this),
                     memory, 0, sizeof(Lifetime));
  *LifetimeTable__OPTIMIZE(this) = optimize;
  *LifetimeTable__CONFIGURE(this) = configure;
  return this;
}

void
LifetimeTable_Dtor(LifetimeTable this)
{
  IDList_Dtor(LifetimeTable_LIFETIMES(this));
  TemporaryMap_Dtor(LifetimeTable_TEMPORARYMAP(this));
}

size_t
LifetimeTable_Size(Memory memory, Optimize optimize, Configure configure)
{
  return sizeof(LifetimeTable_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LifetimeTable_)\t%zu\n", sizeof(LifetimeTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
LifetimeTable
LifetimeTable_new(Memory parent, Optimize optimize, Configure configure);
#endif//$XCC_h

LifetimeTable
LifetimeTable_new(Memory parent, Optimize optimize, Configure configure)
{
  Memory memory = Memory_new(parent, true);
  size_t size = LifetimeTable_Size(memory, optimize, configure);
  LifetimeTable this = Memory_alloc(memory, size);
  return LifetimeTable_Ctor(this, memory, optimize, configure);
}

#ifdef $XCC_h
LifetimeTable
LifetimeTable_delete(LifetimeTable this);
#endif//$XCC_h

LifetimeTable
LifetimeTable_delete(LifetimeTable this)
{
  if (this != NULL) {
    Memory memory = LifetimeTable_MEMORY(this);
#ifndef _NDTOR
    LifetimeTable_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * For use by LifetimeTable_FOREACH_Lifetime.
 */
IDList
LifetimeTable_lifetimes(LifetimeTable this);
#endif//$XCC_h

IDList
LifetimeTable_lifetimes(LifetimeTable this)
{
  return LifetimeTable_LIFETIMES(this);
}

#ifdef $XCC_h
/**
 * Iterate over this LifetimeTable Lifetimes.
 */
#define LifetimeTable_FOREACH_Lifetime(this, lifetime) \
  IDList_FOREACH(LifetimeTable_lifetimes(this), Lifetime_, lifetime) {
#define LifetimeTable_ENDEACH_Lifetime \
  } IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC__h
/**
 * First lifetime or NULL.
 */
static inline Lifetime
LifetimeTable_first(LifetimeTable this)
{
  return LifetimeTable_FIRST(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Last lifetime or NULL.
 */
static inline Lifetime
LifetimeTable_last(LifetimeTable this)
{
  return LifetimeTable_LAST(this);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
LifetimeTable_isEmpty(LifetimeTable this)
{
  return IDList_isEmpty(LifetimeTable_LIFETIMES(this));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline int32_t
LifetimeTable_count(LifetimeTable this)
{
  return IDList_count(LifetimeTable_LIFETIMES(this));
}
#endif//$XCC__h

static inline Lifetime
LifetimeTable_push(LifetimeTable this, Temporary temporary, EffectSeq effectSeq,
                   int32_t firstIndex, unsigned lifetimeFlags)
{
  Memory memory = LifetimeTable_MEMORY(this);
  size_t size = Lifetime_Size(memory, temporary, effectSeq, firstIndex, lifetimeFlags);
  Lifetime lifetime = IDList_push(LifetimeTable_LIFETIMES(this), size);
  Lifetime_Ctor(lifetime, memory, temporary, effectSeq, firstIndex, lifetimeFlags);
  return lifetime;
}

static inline Lifetime
LifetimeTable_put(LifetimeTable this, Temporary temporary, EffectSeq effectSeq,
                  int32_t firstIndex, unsigned lifetimeFlags)
{
  Memory memory = LifetimeTable_MEMORY(this);
  size_t size = Lifetime_Size(memory, temporary, effectSeq, firstIndex, lifetimeFlags);
  Lifetime lifetime = IDList_put(LifetimeTable_LIFETIMES(this), size);
  Lifetime_Ctor(lifetime, memory, temporary, effectSeq, firstIndex, lifetimeFlags);
  return lifetime;
}

static inline void
LifetimeTable_pop(LifetimeTable this)
{
#ifndef _NDTOR
  IDList_pop(LifetimeTable_LIFETIMES(this), (IDListItemRelease)Lifetime_Dtor);
#else//!_NDTOR
  IDList_pop_(LifetimeTable_LIFETIMES(this));
#endif//_NDTOR
}

static inline void
LifetimeTable_drop(LifetimeTable this)
{
#ifndef _NDTOR
  IDList_drop(LifetimeTable_LIFETIMES(this), (IDListItemRelease)Lifetime_Dtor);
#else//!_NDTOR
  IDList_drop_(LifetimeTable_LIFETIMES(this));
#endif//_NDTOR
}

static inline void
LifetimeTable_rotate(LifetimeTable this, int32_t index)
{
  IDList_rotate(LifetimeTable_LIFETIMES(this), index);
}

static inline void
LifetimeTable_sort(LifetimeTable this)
{
  IDList_sort(LifetimeTable_LIFETIMES(this), Lifetime_compare);
}

static inline Lifetime *
LifetimeTable_access(LifetimeTable this, Temporary temporary)
{
  int32_t index = Temporary_INDEX(temporary);
  TemporaryMap temporaryMap = LifetimeTable_TEMPORARYMAP(this);
  Lifetime *baseItem = TemporaryMap_BASE(temporaryMap);
  int32_t maxCount = TemporaryMap_MAXCOUNT(temporaryMap);
  Lifetime *_lifetime = baseItem + index;
  Except_REQUIRE(index >= 0 && index < maxCount);
  return _lifetime;
}

#ifdef $XCC__h
/**
 * Grow this LifetimeTable from @c effectSeq.
 */
void
LifetimeTable_grow(LifetimeTable this, Temporary temporary,
                   EffectSeq effectSeq, unsigned lifetimeFlags);
#endif//$XCC__h

void
LifetimeTable_grow(LifetimeTable this, Temporary temporary,
                   EffectSeq effectSeq, unsigned lifetimeFlags)
{
  int firstIndex = 0;
  Configure configure = LifetimeTable_CONFIGURE(this);
  int overlap = Configure_OVERLAP(configure);
  int renaming = Configure_RENAMING(configure);
  bool isRenameReg = Temporary_isRenameReg(temporary);
  Lifetime first_lifetime = NULL, last_lifetime = NULL;
  if (renaming < ConfigureRenaming_Local ||
      (renaming < ConfigureRenaming_Global &&
        Temporary_isGlobal(temporary))) isRenameReg = false;
  EffectSeq_FOREACH_Effect(effectSeq, effect) {
    bool newLifetime = isRenameReg && Effect_isKill(effect);
    if (first_lifetime == NULL) {
      last_lifetime = LifetimeTable_put(this, temporary, effectSeq,
                                        firstIndex, lifetimeFlags);
      first_lifetime = last_lifetime;
    } else if (newLifetime) {
      last_lifetime = LifetimeTable_push(this, temporary, effectSeq,
                                         firstIndex, lifetimeFlags);
    }
    if (newLifetime && !Lifetime_hasRenaming(last_lifetime)) {
      // Initialize the register RENAMESTACK of the last lifetime.
      TemporaryStack_resize(Lifetime_RENAMESTACK(last_lifetime), overlap + 1);
      *Lifetime__FLAGS(last_lifetime) |= LifetimeFlag_Renaming;
    }
    *Lifetime__LASTINDEX(last_lifetime) = firstIndex++;
  } EffectSeq_ENDEACH_Effect;
  if (first_lifetime == NULL) return;
  if (Lifetime_hasRenaming(last_lifetime)) {
    // Case with renaming registers: one or more lifetimes were created.
    if (first_lifetime != last_lifetime) {
      // Several lifetimes were created for temporary.
      Effect firstEffect = Lifetime_firstEffect(first_lifetime);
      Temporary temporary = Effect_TEMPORARY(firstEffect);
      if (!Effect_isWrite(firstEffect)) {
        // The first lifetime does not start with a Write: merge it with the last.
        Lifetime_FORSAFE_Effect(first_lifetime, effect) {
          EffectSeq_pushEffect(effectSeq, effect);
        } Lifetime_ENDSAFE_Effect;
        *Lifetime__LASTINDEX(last_lifetime) = EffectSeq_count(effectSeq) - 1;
        // Drop the first lifetime, as its effects have been merged.
        Except_CHECK(first_lifetime == LifetimeTable_first(this));
        LifetimeTable_drop(this);
      }
    }
  } else if (first_lifetime == last_lifetime) {
    // Case without renaming registers: a single lifetime was created.
  } else Except_NEVER(true);
}

#ifdef $XCC__h
/**
 * Build this LifetimeTable from an EffectTable.
 */
void
LifetimeTable_build(LifetimeTable this, EffectTable effectTable,
                    bool isInnerLoop, bool isPrePass, Processor processor);
#endif//$XCC__h

void
LifetimeTable_build(LifetimeTable this, EffectTable effectTable,
                    bool isInnerLoop, bool isPrePass, Processor processor)
{
  Optimize optimize = LifetimeTable_OPTIMIZE(this);
  Configure configure = LifetimeTable_CONFIGURE(this);
  unsigned lifetimeFlags = isInnerLoop*LifetimeFlag_InnerLoop
                         + isPrePass*LifetimeFlag_PrePass;
  Lifetime first_lifetime = NULL, last_lifetime = NULL;
  Temporary prev_temporary = NULL;
  int32_t temporaryIndex = 0;
  // Build the Lifetime list.
  EffectTable_FOREACH_EffectSeq(effectTable, effectSeq) {
    LifetimeTable_grow(this, EffectTable_TEMPORARY, effectSeq, lifetimeFlags);
  } EffectTable_ENDEACH_EffectSeq;
  // First loop: pop all the lifetimes that may be ignored.
  while (!LifetimeTable_isEmpty(this)) {
    last_lifetime = LifetimeTable_last(this);
    // Complete the construction of last_lifetime.
    Lifetime_complete(last_lifetime, optimize, configure, processor);
    if (!Lifetime_mayIgnore(last_lifetime)) break;
    LifetimeTable_pop(this);
  }
  if (LifetimeTable_count(this) <= 1) return;
  // Here last_lifetime is not to be ignored; move it to first_lifetime.
  LifetimeTable_rotate(this, -1);
  first_lifetime = last_lifetime;
  // Second loop: either pop the last_lifetime, or rotate the lifetimes.
  last_lifetime = LifetimeTable_last(this);
  while (last_lifetime != first_lifetime) {
    // Complete the construction of last_lifetime.
    Lifetime_complete(last_lifetime, optimize, configure, processor);
    if (Lifetime_mayIgnore(last_lifetime)) {
      LifetimeTable_pop(this);
    } else {
      LifetimeTable_rotate(this, -1);
    }
    last_lifetime = LifetimeTable_last(this);
  }
  // Sort the lifetimes to satisfy the precondition of Lifetime_fillRenaming.
  LifetimeTable_sort(this);
  // Prepare for this BlockTrace optimizations.
  TemporaryMap_Ctor(LifetimeTable_TEMPORARYMAP(this), LifetimeTable_MEMORY(this),
                    LifetimeTable_COUNT(this), sizeof(Lifetime));
  // Initialize the Temporary_INDEX fields and the temporaryMap contents.
  // The test (temporary != prev_temporary) protects against the cases where a
  // Temporary has several Lifetimes; in such case we take the first lifetime.
  LifetimeTable_FOREACH_Lifetime(this, lifetime) {
    Temporary temporary = Lifetime_TEMPORARY(lifetime);
    if (temporary != prev_temporary) {
      Lifetime_setTemporaryIndex(lifetime, temporaryIndex++);
      *LifetimeTable_access(this, temporary) = lifetime;
    }
    prev_temporary = temporary;
  } LifetimeTable_ENDEACH_Lifetime;
}

#ifdef $XCC__h
/**
 * Link the LifetimeTable Effect(s).
 */
void
LifetimeTable_linkEffects(LifetimeTable this, PtrSet ddgNodes);
#endif//$XCC__h

void
LifetimeTable_linkEffects(LifetimeTable this, PtrSet ddgNodes)
{
  Lifetime memory_lifetime = NULL;
  // Link the non-memory Lifetime(s).
  LifetimeTable_FOREACH_Lifetime(this, LIFETIME) {
    Temporary temporary = Lifetime_TEMPORARY(LIFETIME);
    if (!Lifetime_isOnlyUses(LIFETIME)) {
      Lifetime_linkEffects(LIFETIME);
      if (Temporary_isMemory(temporary)) {
        memory_lifetime = LIFETIME;
      }
    }
  } LifetimeTable_ENDEACH_Lifetime;
  // Relink and refine the memory_lifetime.
  if (memory_lifetime != NULL) {
    if (PtrSet_count(ddgNodes) > 0) {
      Lifetime_relinkEffects(memory_lifetime, ddgNodes);
    }
  }
}

#ifdef $XCC__h
/**
 * Refine the memory dependences by analysing addresses.
 */
void
LifetimeTable_refineDependences(LifetimeTable this, PtrSet ddgNodes,
                                const_DependenceTable dependenceTable);
#endif//$XCC__h

void
LifetimeTable_refineDependences(LifetimeTable this, PtrSet ddgNodes,
                                const_DependenceTable dependenceTable)
{
  Configure configure = LifetimeTable_CONFIGURE(this);
  int overlap = Configure_OVERLAP(configure);
  PFA_TRACE && fprintf(PFA_TRACE, "*** REFINED DEPENDENCES\n");
  DependenceTable_FOREACH_DependenceArc(dependenceTable, arc) {
    Operation tail_operation = DependenceArc_TAIL(arc);
    Operation head_operation = DependenceArc_HEAD(arc);
    DependenceKind kind = DependenceArc_KIND(arc);
    if (   DependenceKind_mayRefine(kind)
        && DependenceArc_OMEGA(arc) <= overlap
        && PtrSet_contains(ddgNodes, tail_operation)
        && PtrSet_contains(ddgNodes, head_operation)) {
      Operator tail_operator = Operation_OPERATOR(tail_operation);
      Operator head_operator = Operation_OPERATOR(head_operation);
      int32_t tail_baseIndex = Operator_baseAddressIndex(tail_operator);
      int32_t head_baseIndex = Operator_baseAddressIndex(head_operator);
      if (tail_baseIndex >= 0 && head_baseIndex >= 0) {
        Effect tail_baseEffect = Operation_argEffects(tail_operation) + tail_baseIndex;
        Effect head_baseEffect = Operation_argEffects(head_operation) + head_baseIndex;
        Temporary tail_base_temporary = Effect_TEMPORARY(tail_baseEffect);
        Temporary head_base_temporary = Effect_TEMPORARY(head_baseEffect);
        if (tail_base_temporary == head_base_temporary) {
          Lifetime base_lifetime = *LifetimeTable_access(this, tail_base_temporary);
          int distance = Lifetime_getMemoryDistance(base_lifetime, tail_baseEffect,
                                                    head_baseEffect, overlap);
          if (distance > DependenceArc_OMEGA(arc)) {
            PFA_TRACE && fprintf(PFA_TRACE, "\tomega\t%d -> %d\n",
                                 DependenceArc_OMEGA(arc), distance);
            PFA_TRACE && fprintf(PFA_TRACE, "\t")
                      && Operation_pretty(tail_operation, PFA_TRACE);
            PFA_TRACE && fprintf(PFA_TRACE, "\t")
                      && Operation_pretty(head_operation, PFA_TRACE);
            *DependenceArc__OMEGA(arc) = distance;
          }
          PFA_TRACE && distance >= 0 && PFA_TRACE && fprintf(PFA_TRACE, "\n");
        }
      }
    }
  } DependenceTable_ENDEACH_DependenceArc;
}

#ifdef $XCC__h
/**
 * Compute the maximum Lifetime span.
 */
int
LifetimeTable_computeMaxSpan(LifetimeTable this, int lambda, bool isAcyclic);
#endif//$XCC__h

int
LifetimeTable_computeMaxSpan(LifetimeTable this, int lambda, bool isAcyclic)
{
  int maxSpan = -1;
  PFA_TRACE && fprintf(PFA_TRACE, "*** LIFESPANS:\n");
  // Loop to compute the Lifetime_LIFESPANs and maxSpan.
  LifetimeTable_FOREACH_Lifetime(this, LIFETIME) {
    Temporary temporary = Lifetime_TEMPORARY(LIFETIME);
    int lifespan = Lifetime_computeLifespan(LIFETIME, lambda, isAcyclic);
    if (Temporary_isRenameReg(temporary)) maxSpan = _MAX(lifespan, maxSpan);
  } LifetimeTable_ENDEACH_Lifetime;
  return maxSpan;
}

#ifdef $XCC__h
/**
 * Fill the renaming stacks.
 */
void
LifetimeTable_fillRenaming(LifetimeTable this, Procedure procedure,
                           int lambda, int kunroll);
#endif//$XCC__h

void
LifetimeTable_fillRenaming(LifetimeTable this, Procedure procedure,
                           int lambda, int kunroll)
{
  Except_REQUIRE(kunroll > 0);
  PFA_TRACE && fprintf(PFA_TRACE, "*** RENAMING:\n");
  LifetimeTable_FOREACH_Lifetime(this, LIFETIME) {
    if (Lifetime_hasRenaming(LIFETIME)) {
      Lifetime_fillRenaming(LIFETIME, procedure, lambda, kunroll);
    }
  } LifetimeTable_ENDEACH_Lifetime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Lifetime module.
 */
void
Lifetime_INIT(void);
#endif//$XCC__h

void
Lifetime_INIT(void)
{
#ifndef _NTRACE
  ETable_Ctor(LifetimeFlag_Table, Memory_Root, LifetimeFlag_Entries);
#endif//_NTRACE
}

#ifdef $XCC__h
/**
 * Finalize the Lifetime module.
 */
void
Lifetime_FINI(void);
#endif//$XCC__h

void
Lifetime_FINI(void)
{
#ifndef _NTRACE
  ETable_Dtor(LifetimeFlag_Table);
#endif//_NTRACE
}

#if XCC__C
static void
Lifetime_TEST(void)
{
#include "XFA/Lifetime_.c"
}

int
main(int argc, char **argv)
{
  XFA_Test(Lifetime_TEST);
  return 0;
}
#endif

