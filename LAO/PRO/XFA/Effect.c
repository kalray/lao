#ifdef $XCC_h
/*
 * !!!!	Effect.xcc
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
 * @brief Operation Effect and EffectTable.
 */
#endif//$XCC_h


#include "XFA/XFA_.h"

#ifdef $XCC__h
#include "XFA/Effect.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Effect flags.
 *
 * Side Effect(s):	not in the Operation arguments or results.
 * Indirect Effect(s):	special cases for latency computations.
 */
typedef enum {
  EffectFlag_Read = 0x0,	// Effect is a read to the target resource.
  EffectFlag_Write = 0x1,	// Effect is a write to the target resource.
  EffectFlag_Kill = 0x2,	// Effect is a write that kills the previous value.
  EffectFlag_Clobber = 0x4,	// Effect is a clobber: creates an undefined value.
  EffectFlag_Indirect = 0x8,	// Effect is indirect: from Label or Control.
  EffectFlag_Carried = 0x10,	// Effect is loop-carried read or partial def.
  EffectFlag_Supplied = 0x20,	// Effect is in the supplied dependence set.
  EffectFlag_InTrace = 0x40,	// Effect is indirect in the same BlockTrace.
} enum_EffectFlag;
typedef uint8_t EffectFlags;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ EffectFlag_Table[1];
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ EffectFlag_Entries[] = {
  ETableENTRY(EffectFlag, Write),
  ETableENTRY(EffectFlag, Kill),
  ETableENTRY(EffectFlag, Clobber),
  ETableENTRY(EffectFlag, Indirect),
  ETableENTRY(EffectFlag, Carried),
  ETableENTRY(EffectFlag, Supplied),
  ETableENTRY(EffectFlag, InTrace),
  ETableENTRY_NULL
};
ETable_ EffectFlag_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Action of an Operation on a processor resource.
 *
 * An effect materializes a read or a write on a state-holding processor resource
 * (register, control, memory, etc.) represented as a Temporary.
 *
 * An Effect is a side effect when its XXXINDEX is negative. Else the XXXINDEX is
 * either the ARGINDEX or the RESINDEX, depending on the position in Operation.
 */
struct Effect_ {
  //@args	Operation operation, Temporary temporary, EffectFlags flags, int index
  //@access NEXT	(Effect)(IDListItem_NEXT(this))
  Operation OPERATION;		// The Operation that creates this Effect.
  Temporary TEMPORARY;		// The Temporary this Effect applies to.
  int16_t XXXINDEX;		// Index in the Operation_xxxEFFECTS.
  //@access isSide	(Effect_XXXINDEX(this) < 0)
  EffectFlags FLAGS;	// This Effect flags.
  //@access isRead	((Effect_FLAGS(this) & EffectFlag_Write) == 0)
  //@access isWrite	((Effect_FLAGS(this) & EffectFlag_Write) != 0)
  //@access isKill	((Effect_FLAGS(this) & EffectFlag_Kill) != 0)
  //@access isClobber	((Effect_FLAGS(this) & EffectFlag_Clobber) != 0)
  //@access isIndirect	((Effect_FLAGS(this) & EffectFlag_Indirect) != 0)
  //@access isCarried	((Effect_FLAGS(this) & EffectFlag_Carried) != 0)
  //@access isSupplied	((Effect_FLAGS(this) & EffectFlag_Supplied) != 0)
  //@access isInTrace	((Effect_FLAGS(this) & EffectFlag_InTrace) != 0)
  int8_t RELAXABLE;		// The amount of inductive relaxation applicable.
  struct Lifetime_ *LIFETIME;	// The Lifetime this Effect belongs to.
  struct Effect_ *PRED;		// Dependence predecessor of this Effect.
  struct Effect_ *SUCC;		// Dependence successor of this Effect.
};
#endif//$XCC_h

Effect
Effect_Ctor(Effect this,
            Operation operation, Temporary temporary, EffectFlags flags, int index)
{
  *Effect__OPERATION(this) = operation;
  *Effect__TEMPORARY(this) = temporary;
  *Effect__XXXINDEX(this) = index;
  *Effect__FLAGS(this) = flags;
  *Effect__RELAXABLE(this) = 0;
  *Effect__LIFETIME(this) = NULL;
  *Effect__PRED(this) = NULL;
  *Effect__SUCC(this) = NULL;
  return this;
}

Effect
Effect_Copy(Effect this, const_Effect that)
{
  *Effect__OPERATION(this) = Effect_OPERATION(that);
  *Effect__TEMPORARY(this) = Effect_TEMPORARY(that);
  *Effect__XXXINDEX(this) = Effect_XXXINDEX(that);
  *Effect__FLAGS(this) = Effect_FLAGS(that);
  *Effect__RELAXABLE(this) = 0;
  *Effect__LIFETIME(this) = NULL;
  *Effect__PRED(this) = NULL;
  *Effect__SUCC(this) = NULL;
  return this;
}

size_t
Effect_Size(Operation operation, Temporary temporary, EffectFlags flags, int index)
{
        return sizeof(Effect_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Effect_)\t%zu\n", sizeof(Effect_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Pretty-print this Effet.
 */
bool
Effect_pretty(Effect this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Effect_pretty(Effect this, FILE *file)
{
  Operation operation = Effect_OPERATION(this);
  Operator operator = Operation_operator(operation);
  EffectFlags flags = Effect_FLAGS(this);
  int relaxable = Effect_RELAXABLE(this);
  if (file == NULL) file = stderr;
  fprintf(file, "%d:%s_%d\t",
      Operation_identity(operation),
      Operator_mnemonic(operator),
      Operation_index(operation));
  if (flags & EffectFlag_Write) {
    fprintf(file, "Write(%d)\t", Effect_XXXINDEX(this));
  } else {
    fprintf(file, "Read(%d) \t", Effect_XXXINDEX(this));
  }
  ETable_prettyFlags(EffectFlag_Table, flags & ~EffectFlag_Write, "+", file);
  if (relaxable > 0) {
    fprintf(file, "\trelaxable=%d", relaxable);
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * This Effect Operation.
 */
static inline Operation
Effect_operation(Effect this)
{
  return Effect_OPERATION(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Effect Temporary.
 */
static inline Temporary
Effect_temporary(Effect this)
{
  return Effect_TEMPORARY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Effect flags.
 */
static inline void
Effect_setFlags(Effect this, unsigned flags)
{
  *Effect__FLAGS(this) |= flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Reset this Effect flags.
 */
static inline void
Effect_clearFlags(Effect this, unsigned flags)
{
  *Effect__FLAGS(this) &= ~flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the Register RAW latency between two effects. Negative means no dependence.
 */
int
Effect_getRegisterRAWLatency(Effect this, Effect that, Processor processor, int extra);
#endif//$XCC_h

int
Effect_getRegisterRAWLatency(Effect this, Effect that, Processor processor, int extra)
{
  int theta = -1;
  int readStage = 2, writeStage = 2;	// HACK! Should be min readStage.
  int this_xxxIndex = Effect_XXXINDEX(this);
  int that_xxxIndex = Effect_XXXINDEX(that);
  int minTheta = Processor_minTheta(processor);
  Temporary temporary = Effect_TEMPORARY(this);
  Operation this_operation = Effect_OPERATION(this);
  Operation that_operation = Effect_OPERATION(that);
  Operator this_operator = Operation_operator(this_operation);
  Operator that_operator = Operation_operator(that_operation);
  const_RegisterAction writeAction = NULL, readAction = NULL;
  Opcode write_opcode = Opcode__UNDEF, read_opcode = Opcode__UNDEF;
  if (Effect_isIndirect(this) || Effect_isIndirect(that)) {
    if (Processor_interlocks(processor)) return 0;
  }
  if (this_xxxIndex >= 0) {
    Opcode this_opcode = Operator_opcode(this_operator, processor); // FIXME!
    const_RegisterActionArray this_actionArray = Opcode_actionArray(this_opcode);
    RegisterActionArray_FORWRITE_RegisterAction(this_actionArray, action) {
      if (RegisterAction_xxxIndex(action) == this_xxxIndex) writeAction = action;
    } RegisterActionArray_ENDWRITE_RegisterAction;
    if (writeAction != NULL) writeStage = RegisterAction_stage(writeAction);
    write_opcode = this_opcode;
  }
  if (that_xxxIndex >= 0) {
    Opcode that_opcode = Operator_opcode(that_operator, processor); // FIXME!
    const_RegisterActionArray that_actionArray = Opcode_actionArray(that_opcode);
    RegisterActionArray_FORREAD_RegisterAction(that_actionArray, action) {
      if (RegisterAction_xxxIndex(action) == that_xxxIndex) readAction = action;
    } RegisterActionArray_ENDREAD_RegisterAction;
    if (readAction != NULL) readStage = RegisterAction_stage(readAction);
    read_opcode = that_opcode;
  }
  if (Temporary_hasRegister(temporary)) {
    Register registre = Temporary_REGISTER(temporary);
    if (Opcode_fixupRAW(write_opcode, read_opcode)) {
      const_RegisterStorageCells storageCells = Register_storageCells(registre);
      StorageCell cell = RegisterStorageCells_access(storageCells, 0);
      int biasRAW = Opcode_biasRAW(write_opcode, read_opcode, cell, 0);
      if (biasRAW < 0) return -1;
      readStage -= biasRAW;
    }
    if (Effect_isIndirect(that)) {
      readStage = Register_minReadStage(registre, processor);
    }
  }
  if (Effect_isIndirect(this)) writeStage = readStage;
  Except_CHECK(readStage >= 0 && writeStage >= 0);
  theta = writeStage - readStage;
  if (theta < minTheta) theta = minTheta;
  if (Effect_isIndirect(that)) {
    int indirectElapsed = Processor_minTaken(processor) + 1;
    if (Effect_isInTrace(that)) theta = 0;
    else if (indirectElapsed > theta) theta = 0;
    else theta -= indirectElapsed;
  } else theta += extra;
  Except_CHECK(theta >= 0);
  return theta;
}

#ifdef $XCC_h
/**
 * The Register extra WAR latency between two effects.
 *
 * Add an extra cycle to the WAR latency for operations that are positioned late
 * in an issue group like control-flow operations. This prevents the scheduler to
 * produce semantic invalid issue groups like:
 *
 *	[0]  CMP $b1 = ...
 *	[0]  BR $b1, target
 *
 * Here, Operation BR actually gets the old $b1 but it appears to get the new $b1.
 */
static inline int
Effect_getRegisterExtraWARLatency(Effect this)
{
  Operation this_operation = Effect_OPERATION(this);
  Operator this_operator = Operation_operator(this_operation);
  return Operator_isControl(this_operator);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the Register WAR latency between two effects.
 */
int
Effect_getRegisterWARLatency(Effect this, Effect that, Processor processor);
#endif//$XCC_h

int
Effect_getRegisterWARLatency(Effect this, Effect that, Processor processor)
{
  if (Effect_isIndirect(this)) return 1;
  return Effect_getRegisterExtraWARLatency(this);
}

#ifdef $XCC_h
/**
 * Get the Register WAW latency between two effects.
 */
int
Effect_getRegisterWAWLatency(Effect this, Effect that, Processor processor);
#endif//$XCC_h

int
Effect_getRegisterWAWLatency(Effect this, Effect that, Processor processor)
{
  if (Effect_isIndirect(this)) return 0;
  if (Effect_isIndirect(that)) return 0;
  return 1;
}

#ifdef $XCC_h
/**
 * Get the Control FLow latency between two effects.
 */
static inline int
Effect_getRegionFlowLatency(Effect this, Effect that, Processor processor)
{
  // Control operations are positioned last in an issue group, so latency must be > 0.
  return 1;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the Control Anti latency between two effects.
 */
static inline int
Effect_getControlAntiLatency(Effect this, Effect that, Processor processor)
{
  // TODO
  return 0;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the Control Output latency between two effects.
 */
static inline int
Effect_getControlOutputLatency(Effect this, Effect that, Processor processor)
{
  Operation this_operation = Effect_OPERATION(this);
  Operation that_operation = Effect_OPERATION(that);
  if (Operator_isFall(Operation_operator(this_operation))) return 0;
  if (Operator_isFall(Operation_operator(that_operation))) return 0;
  // TODO
  return 1;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the Memory FLow latency between two effects.
 */
int
Effect_getMemoryFlowLatency(Effect this, Effect that, Processor processor);
#endif//$XCC_h

int
Effect_getMemoryFlowLatency(Effect this, Effect that, Processor processor)
{
  // TODO
  return 1;
}

#ifdef $XCC_h
/**
 * Get the Memory Anti latency between two effects.
 */
int
Effect_getMemoryAntiLatency(Effect this, Effect that, Processor processor);
#endif//$XCC_h

int
Effect_getMemoryAntiLatency(Effect this, Effect that, Processor processor)
{
  // TODO
  return 1;
}

#ifdef $XCC_h
/**
 * Get the Memory Output latency between two effects.
 */
int
Effect_getMemoryOutputLatency(Effect this, Effect that, Processor processor);
#endif//$XCC_h

int
Effect_getMemoryOutputLatency(Effect this, Effect that, Processor processor)
{
  // TODO
  return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Sequence of effects on a particular Temporary.
 */
struct EffectSeq_ {
  //@args Memory memory, int32_t estimate
  IStack_ ISTACK_;		// Stack of Effect(s).
  //@access ISTACK	EffectSeq__ISTACK_(this)
  //@access COUNT	(IStack_usedSize(EffectSeq_ISTACK(this))/sizeof(Effect))
  //@access MEMORY	IStack_memory(EffectSeq_ISTACK(this))
};
#endif//$XCC_h

EffectSeq
EffectSeq_Ctor(EffectSeq this,
               Memory memory, int32_t estimate)
{
  IStack_Ctor(EffectSeq_ISTACK(this), memory, sizeof(Effect), estimate);
  return this;
}

void
EffectSeq_Dtor(EffectSeq this)
{
  IStack_Dtor(EffectSeq_ISTACK(this));
}

size_t
EffectSeq_Size(Memory memory, int32_t estimate) {
        return sizeof(EffectSeq_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(EffectSeq_)\t%zu\n", sizeof(EffectSeq_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * For use by EffectSeq_FOREACH_Effect.
 */
static inline IStack
EffectSeq_istack(EffectSeq this)
{
  return EffectSeq_ISTACK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this EffectSeq Effect(s).
 */
#define EffectSeq_FOREACH_Effect(this, effect) { \
  IStack EffectSeq_ISTACK = EffectSeq_istack(this); \
  IStack_FOREACH(EffectSeq_ISTACK, Effect, EffectSeq__EFFECT) { \
    Effect effect = *EffectSeq__EFFECT;
#define EffectSeq_ENDEACH_Effect \
  } IStack_ENDEACH; \
}
#endif//$XCC_h

#ifdef $XCC_h
static inline bool
EffectSeq_isEmpty(EffectSeq this)
{
  return IStack_isEmpty(EffectSeq_ISTACK(this));
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
EffectSeq_count(EffectSeq this)
{
  return EffectSeq_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Effect *
EffectSeq_base(EffectSeq this)
{
  return (Effect *)IStack_base(EffectSeq_ISTACK(this));
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Effect
EffectSeq_lastEffect(EffectSeq this)
{
  return *(Effect *)IStack_lastItem(EffectSeq_ISTACK(this));
}
#endif//$XCC_h

#ifdef $XCC_h
static inline void
EffectSeq_pushEffect(EffectSeq this, Effect effect)
{
  IStack_PUSH2(EffectSeq_ISTACK(this), Effect, effect);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Tabulate the Effect(s) by Operation.
 *
 * The purpose of an EffectTable is to tabulate the Effect(s) of a sequence of
 * Operation(s), for use in local optimizations. When an Operation is entered
 * into an EffectTable, its side Effect(s) are computed if requested.
 */
struct EffectTable_ {
  //@args	Memory memory, Procedure procedure, int32_t estimate
  IStack_ OPERATIONS_;		// This EffectTable Operation(s).
  //@access OPERATIONS	EffectTable__OPERATIONS_(this)
  //@access MEMORY	IStack_memory(EffectTable_OPERATIONS(this))
  //@access COUNT	(IStack_usedSize(EffectTable_OPERATIONS(this))/sizeof(Operation))
  HTable_ EFFECTSEQS_;		// Hash from Temporary to EffectSeq.
  //@access EFFECTSEQS	EffectTable__EFFECTSEQS_(this)
  IDList_ SIDEEFFECTS_;			//< This EffectTable list of side-Effect(s).
  //@access SIDEEFFECTS	EffectTable__SIDEEFFECTS_(this)
  Procedure PROCEDURE;
  struct Temporary_ *DEDICATED_;
};
#endif//$XCC_h

EffectTable
EffectTable_Ctor(EffectTable this,
                 Memory memory, Procedure procedure, int32_t estimate)
{
  Temporary_ *dedicated = Procedure_dedicated(procedure);
  IStack_Ctor(EffectTable_OPERATIONS(this),
              memory, sizeof(Operation), estimate);
  HTable_Ctor(EffectTable_EFFECTSEQS(this), memory, estimate);
  IDList_Ctor(EffectTable_SIDEEFFECTS(this), memory);
  *EffectTable__PROCEDURE(this) = procedure;
  *EffectTable__DEDICATED_(this) = dedicated;
  return this;
}

void
EffectTable_Dtor(EffectTable this)
{
  Memory memory = EffectTable_MEMORY(this);
  EffectTable_FORBACK_Operation(this, operation) {
    Memory_free(memory, Operation_mainEffects(operation));
  } EffectTable_ENDBACK_Operation;
  IStack_Dtor(EffectTable_OPERATIONS(this));
  HTable_empty(EffectTable_EFFECTSEQS(this),
               (HTableValueRelease)EffectSeq_Dtor);
  HTable_Dtor(EffectTable_EFFECTSEQS(this));
  IDList_Dtor(EffectTable_SIDEEFFECTS(this));
}

size_t
EffectTable_Size(Memory memory, Procedure procedure, int32_t estimate)
{
  return sizeof(EffectTable_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(EffectTable_)\t%zu\n", sizeof(EffectTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
EffectTable
EffectTable_new(Memory parent, Procedure procedure, int32_t estimate);
#endif//$XCC_h

EffectTable
EffectTable_new(Memory parent, Procedure procedure, int32_t estimate)
{
  Memory memory = Memory_new(parent, true);
  size_t size = EffectTable_Size(memory, procedure, estimate);
  EffectTable this = Memory_alloc(memory, size);
  return EffectTable_Ctor(this, memory, procedure, estimate);
}

#ifdef $XCC_h
EffectTable
EffectTable_delete(EffectTable this);
#endif//$XCC_h

EffectTable
EffectTable_delete(EffectTable this)
{
  if (this != NULL) {
    Memory memory = EffectTable_MEMORY(this);
#ifndef _NDTOR
    EffectTable_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * For use by EffectTable_FOREACH_Operation.
 */
static inline IStack
EffectTable_operations(EffectTable this)
{
  return EffectTable_OPERATIONS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this EffectTable Operation(s).
 */
#define EffectTable_FOREACH_Operation(this, operation) { \
  IStack EffectTable_OPERATIONS = EffectTable_operations(this); \
  IStack_FOREACH(EffectTable_OPERATIONS, Operation, EffectTable__ITER) { \
    Operation operation = *EffectTable__ITER;
#define EffectTable_ENDEACH_Operation \
  } IStack_ENDEACH; \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this EffectTable Operation(s).
 */
#define EffectTable_FORBACK_Operation(this, operation) { \
  IStack EffectTable_OPERATIONS = EffectTable_operations(this); \
  IStack_FORBACK(EffectTable_OPERATIONS, Operation, EffectTable__ITER) { \
    Operation operation = *EffectTable__ITER;
#define EffectTable_ENDBACK_Operation \
  } IStack_ENDBACK; \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by EffectTable_FOREACH_EffectSeq.
 */
static inline HTable
EffectTable_effectSeqs(EffectTable this)
{
  return EffectTable_EFFECTSEQS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this EffectTable EffectSeq(s).
 */
#define EffectTable_FOREACH_EffectSeq(this, effectSeq) { \
  HTable EffectTable_EFFECTSEQS = EffectTable_effectSeqs(this); \
  HTable_FOREACH(EffectTable_EFFECTSEQS, HTable_KEY, EffectSeq_, effectSeq) { \
    Temporary EffectTable_TEMPORARY = (Temporary)HTable_KEY;
#define EffectTable_ENDEACH_EffectSeq \
  } HTable_ENDEACH; \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Return the EffectSeq associated to temporary.
 */
EffectSeq
EffectTable_lookupEffectSeq(EffectTable this, Temporary temporary);
#endif//$XCC_h

EffectSeq
EffectTable_lookupEffectSeq(EffectTable this, Temporary temporary)
{
  Memory memory = EffectTable_MEMORY(this);
  Temporary dedicated = EffectTable_DEDICATED_(this);
  int32_t estimate = (EffectTable_COUNT(this) + 3)/4;
  size_t size = EffectSeq_Size(memory, estimate);
  HTable effectSeqs = EffectTable_EFFECTSEQS(this);
  int32_t count = HTable_count(effectSeqs);
  HTableKey key = (Temporary_isAssigned(temporary) || Temporary_isDedicated(temporary))?
      dedicated + Temporary_REGISTER(temporary): temporary;
  EffectSeq effectSeq = HTable_lookup_(effectSeqs, key, size);
  Except_CHECK(key != NULL);
  if (count != HTable_count(effectSeqs)) {
    if (Temporary_isControl(temporary)) estimate *= 8;
    if (estimate < 8) estimate = 8;
    EffectSeq_Ctor(effectSeq, memory, estimate);
  }
  return effectSeq;
}

#ifdef $XCC__h
/**
 * Enter an Effect in this EffectTable.
 */
static inline void
EffectTable_enterEffect(EffectTable this, Effect effect)
{
  Temporary temporary = Effect_TEMPORARY(effect);
  EffectSeq effectSeq = EffectTable_lookupEffectSeq(this, temporary);
  EffectSeq_pushEffect(effectSeq, effect);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Make the argument Effect(s) of Operation and enter them.
 *
 * In case there are extra arguments beyond the number given by Operation
 * operator, flag the Effect(s) as indirect as these correspond to CALL and
 * RETURN arguments that have been made explicit.
 */
void
EffectTable_makeArgEffects(EffectTable this, Operation operation);
#endif//$XCC__h

void
EffectTable_makeArgEffects(EffectTable this, Operation operation)
{
  EffectFlags flags = EffectFlag_Read;
  Effect_ *argEffects = Operation_argEffects(operation);
  int readCount = Operator_readCount(Operation_operator(operation));
  Operation_ARGUMENTS_FOREACH_Temporary(operation, temporary) {
    Effect effect = argEffects + Operation_ARGUMENTS_INDEX;
    if (Operation_ARGUMENTS_INDEX >= readCount) flags |= EffectFlag_Indirect;
    Effect_Ctor(effect, operation, temporary, flags, Operation_ARGUMENTS_INDEX);
    EffectTable_enterEffect(this, effect);
  } Operation_ARGUMENTS_ENDEACH_Temporary;
}

#ifdef $XCC__h
/**
 * Make the result Effect(s) of Operation and enter them.
 *
 * In case there are extra results beyond the number given by Operation
 * operator, flag the Effect(s) as indirect as these correspond to CALL and
 * RETURN arguments that have been made explicit.
 */
void
EffectTable_makeResEffects(EffectTable this, Operation operation);
#endif//$XCC__h

void
EffectTable_makeResEffects(EffectTable this, Operation operation)
{
  EffectFlags flags = EffectFlag_Write;
  Operator operator = Operation_operator(operation);
  int writeCount = Operator_writeCount(operator);
  Effect_ *resEffects = Operation_resEffects(operation);
  if (!Operator_isPredicated(operator) && !Operator_isConstrained(operator)) flags |= EffectFlag_Kill;
  Operation_RESULTS_FOREACH_Temporary(operation, temporary) {
    Effect effect = resEffects + Operation_RESULTS_INDEX;
    if (Operation_RESULTS_INDEX >= writeCount) flags |= EffectFlag_Indirect;
    Effect_Ctor(effect, operation, temporary, flags, Operation_RESULTS_INDEX);
    EffectTable_enterEffect(this, effect);
  } Operation_RESULTS_ENDEACH_Temporary;
}

#ifdef $XCC__h
/**
 * Make a new side Effect for an Operation and enter it.
 */
Effect
EffectTable_makeSideEffect(EffectTable this, Operation operation,
                           Temporary temporary, EffectFlags flags);
#endif//$XCC__h

Effect
EffectTable_makeSideEffect(EffectTable this, Operation operation,
                           Temporary temporary, EffectFlags flags)
{
  Effect effect = (Effect)IDList_push(EffectTable_SIDEEFFECTS(this), sizeof(Effect_));
  Effect_Ctor(effect, operation, temporary, flags, -1);
  EffectTable_enterEffect(this, effect);
  return effect;
}

#ifdef $XCC_h
/**
 * Enter the side Effects of an Operation from a
 * RegisterSet.
 */
void
EffectTable_enterRegisterSetSideEffects(EffectTable this, Operation operation,
                                        const_RegisterSet registerSet, EffectFlags flags);
#endif//$XCC_h

void
EffectTable_enterRegisterSetSideEffects(EffectTable this, Operation operation,
                                        const_RegisterSet registerSet, EffectFlags flags)
{
  Temporary dedicated = EffectTable_DEDICATED_(this);
  RegisterSet_FOREACH_Register(registerSet, registre) {
    Temporary temporary = dedicated + registre;
    if (temporary != NULL) {
      EffectTable_makeSideEffect(this, operation, temporary, flags);
    }
  } RegisterSet_ENDEACH_Register
}

#ifdef $XCC_h
/**
 * Enter the side Effects of an Operation from a
 * TemporarySet.
 */
void
EffectTable_enterTemporarySetSideEffects(EffectTable this, Operation operation,
                                         TemporarySet temporarySet, EffectFlags flags);
#endif//$XCC_h

void
EffectTable_enterTemporarySetSideEffects(EffectTable this, Operation operation,
                                         TemporarySet temporarySet, EffectFlags flags)
{
  PFA_TRACE && fprintf(PFA_TRACE, "    %d:%s_%d\t%s\t",
                       Operation_identity(operation),
                       Operator_mnemonic(Operation_operator(operation)),
                       Operation_index(operation),
                       flags&EffectFlag_Write? "Write": "Read");
  // Loop over the temporaries and enter their Effect.
  TemporarySet_FOREACH_Temporary(temporarySet, temporary) {
    bool pushEffect = true;
    EffectSeq effectSeq = EffectTable_lookupEffectSeq(this, temporary);
    if (!EffectSeq_isEmpty(effectSeq)) {
      // Avoid duplicate side effects.
      Effect lastEffect = EffectSeq_lastEffect(effectSeq);
      if (Effect_OPERATION(lastEffect) == operation &&
          Effect_isSide(lastEffect)) pushEffect = false;
    }
    if (pushEffect) {
      // Enter the indirect effect as a side-effect.
      EffectTable_makeSideEffect(this, operation, temporary, flags);
    }
  } TemporarySet_ENDEACH_Temporary;
  PFA_TRACE && TemporarySet_pretty(temporarySet, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
}

#ifdef $XCC_h
/**
 * Enter an Operation into this EffectTable.
 * @c sideEffects:	If true, compute the operation side Effect(s).
 */
void
EffectTable_enterOperation(EffectTable this, Operation operation, bool sideEffects);
#endif//$XCC_h

void
EffectTable_enterOperation(EffectTable this, Operation operation, bool sideEffects)
{
  Memory memory = EffectTable_MEMORY(this);
  Operator operator = Operation_operator(operation);
  RegisterSet clobberSet = Operation_clobberSet(operation);
  int32_t effectCount = Operation_argCount(operation) + Operation_resCount(operation);
  struct Temporary_ *dedicated = EffectTable_DEDICATED_(this);
  Operation_setMainEffects(operation,
                           Memory_alloc(memory, sizeof(Effect_)*(effectCount)));
  // Push the Operation.
  IStack_PUSH2(EffectTable_OPERATIONS(this), Operation, operation);
  // Argument effects.
  EffectTable_makeArgEffects(this, operation);
  // Side effects.
  if (sideEffects) {
    // Control effects.
    Temporary control_temporary = dedicated + Register__Control;
    if (Operator_isControl(operator)) {
      EffectTable_makeSideEffect(this, operation, control_temporary, EffectFlag_Write);
    } else {
      EffectTable_makeSideEffect(this, operation, control_temporary, 0);
    }
    // Memory effects.
    if (Operator_isMemory(operator) || Operation_isMemoryFence(operation)) {
      Temporary memory_temporary = dedicated + Register__Memory;
      if (Operator_isMemoryRead(operator) || Operation_isMemoryTouch(operation)) {
        EffectTable_makeSideEffect(this, operation, memory_temporary, 0);
      } else if (Operator_isMemoryWrite(operator)) {
        EffectTable_makeSideEffect(this, operation, memory_temporary, EffectFlag_Write);
      } else {
        EffectTable_makeSideEffect(this, operation, memory_temporary, 0);
        EffectTable_makeSideEffect(this, operation, memory_temporary, EffectFlag_Write);
      }
    }
    // Volatile effects.
    if (Operation_isVolatile(operation)) {
      Temporary volatile_temporary = dedicated + Register__Volatile;
      EffectTable_makeSideEffect(this, operation, volatile_temporary, EffectFlag_Write);
    }
    // Clobber effects.
    if (clobberSet != NULL) {
      Operator operator = Operation_operator(operation);
      EffectFlags flags = EffectFlag_Write | EffectFlag_Clobber;
      if (!Operator_isPredicated(operator) && !Operator_isConstrained(operator)) flags |= EffectFlag_Kill;
      EffectTable_enterRegisterSetSideEffects(this, operation, clobberSet, flags);
    }
  }
  // Result effects.
  EffectTable_makeResEffects(this, operation);
}

#ifdef $XCC_h
void
EffectTable_releaseOperation(EffectTable this, Operation operation);
#endif//$XCC_h

void
EffectTable_releaseOperation(EffectTable this, Operation operation)
{
  Memory memory = EffectTable_MEMORY(this);
  Memory_free(memory, Operation_mainEffects(operation));
  Operation_setMainEffects(operation, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize Effect.
 */
void
Effect_INIT(void);
#endif//$XCC__h

void
Effect_INIT(void)
{
#ifndef _NTRACE
  ETable_Ctor(EffectFlag_Table, Memory_Root, EffectFlag_Entries);
#endif//_NTRACE
}

#ifdef $XCC__h
/**
 * Finalize Effect.
 */
void
Effect_FINI(void);
#endif//$XCC__h

void
Effect_FINI(void)
{
#ifndef _NTRACE
  ETable_Dtor(EffectFlag_Table);
#endif//_NTRACE
}

#if XCC__C
static void
Effect_TEST(void)
{
#include "XFA/Effect_.c"
}

int
main(int argc, char **argv)
{
  XFA_Test(Effect_TEST);
  return 0;
}
#endif

