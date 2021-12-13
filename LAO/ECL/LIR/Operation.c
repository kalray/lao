#ifdef $XCC_h
/*
 * !!!!	Operation.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu)
 * Christophe Guillon (Christophe.Guillon@st.com).
 * Duco van Amstel (duco.van-amstel@kalray.eu)
 *
 * Copyright 2010 - 2013 Kalray.
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
 * @brief Operation implementation.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/Operation.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * IndexedFactory for Operation(s).
 */
extern IndexedFactory_ Operation_Factory[1];
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Operation flags.
 */
typedef enum {
  OperationFlag_MemorySpill = 0x2,	//!< Operation is spill code.
  OperationFlag_Volatile = 0x4,		//!< Operation has volatile effects.
  OperationFlag_SPUpdate = 0x8,		//!< Operation is a SP update.
  OperationFlag_MemoryTouch = 0x10,	//!< Operation is a memory prefetch.
  OperationFlag_MemoryLevel2 = 0x20,	//!< Operation is a memory preload.
  OperationFlag_MemoryFence = 0x40,	//!< Operation is a memory barrier.
  OperationFlag_Clobber = 0x80,		//!< Operation has a clobber RegisterSet.
  OperationFlag_Hoisted = 0x100,	//!< Operation has been hoisted from loop.
  OperationFlag_DeadCode = 0x200,	//!< Operation is dead code.
  OperationFlag_CopyMode = 0x400,	//!< Operation is in copy mode (PHI).
  OperationFlag_MemorySafe = 0x800,	//!< Operation is safe for side-effects.
  OperationFlag_PureCopy = 0x2000,	//!< Operation is a pure copy (no clipping).
  OperationFlag_EntryCode = 0x4000,	//!< Operation is part of Procedure entry.
  OperationFlag_ExitCode = 0x8000,	//!< Operation is part of Procedure exit.
  OperationFlag_Visited = 0x10000,	//!< Operation has beed visited.
} enum_OperationFlag;
typedef uint32_t OperationFlags;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OperationFlag_Table[1];
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ OperationFlagEntries[] = {
  ETableENTRY(OperationFlag, MemorySpill),
  ETableENTRY(OperationFlag, Volatile),
  ETableENTRY(OperationFlag, SPUpdate),
  ETableENTRY(OperationFlag, MemoryTouch),
  ETableENTRY(OperationFlag, MemoryLevel2),
  ETableENTRY(OperationFlag, MemoryFence),
  ETableENTRY(OperationFlag, Clobber),
  ETableENTRY(OperationFlag, Hoisted),
  ETableENTRY(OperationFlag, DeadCode),
  ETableENTRY(OperationFlag, CopyMode),
  ETableENTRY(OperationFlag, MemorySafe),
  ETableENTRY(OperationFlag, PureCopy),
  ETableENTRY(OperationFlag, EntryCode),
  ETableENTRY(OperationFlag, ExitCode),
  ETableENTRY(OperationFlag, Visited),
  ETableENTRY_NULL
};
ETable_ OperationFlag_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
static inline InstructionFlags
OperationFlags_makeInstructionFlags(OperationFlags operationFlags)
{
  InstructionFlags flags = 0;
  if (operationFlags & OperationFlag_MemorySpill) flags |= InstructionFlag_MemorySpill;
  if (operationFlags & OperationFlag_Volatile) flags |= InstructionFlag_Volatile;
  return flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Union for the variant part of Operation.
 */
union OperationUnion_ {
  intptr_t MAXCOPIES;			//!< Maximum count of parallel copy arguments and results.
  struct Variable_ *PHICOLOR;		//!< Color of a PHI-Operation.
  struct VariableClass_ *PHICLASS;	//!< Congruence class of a PHI-Operation
  struct Effect_ *MAINEFFECTS;		//!< This Operation main Effect(s).
};
typedef union OperationUnion_ OperationUnion_, *OperationUnion;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Represent an instruction in the compiler intermediate representation.
 */
struct Operation_ {
  //@args	Operator operator, int32_t resCount, int32_t arcCount,
  //@args	const_RegisterSet clobberSet, struct BasicBlock_ *basicBlock
  Indexed_ __;			//!< An Operation_ ISA Indexed_.
  //@access IDENTITY	Indexed_IDENTITY(Operation____(this))
  //@access INDEX	Indexed_INDEX(Operation____(this))
  //@mutate INDEX	Indexed__INDEX(Operation____(this))
  const struct Operation_ *REPLICATED;	//!< Operation this Operation was replicated from.
  struct BasicBlock_ *BASICBLOCK;	//!< BasicBlock this Operation belongs to.
  InstructionTask TASK;			//!< InstructionTask for scheduling.
  //@access INSTRUCTION	(Instruction)Operation_TASK(this)
  //@mutate INSTRUCTION	(Instruction*)Operation__TASK(this)
  //@access ISSUEITEM	(struct IssueItem_ *)Operation_TASK(this)
  //@mutate ISSUEITEM	(struct IssueItem_ **)Operation__TASK(this)
  //@access isInductive		InstructionTask_isInductive(Operation_TASK(this))
  //@access isSpeculable	InstructionTask_isSpeculable(Operation_TASK(this))
  //@access isCompensable	InstructionTask_isCompensable(Operation_TASK(this))
  Operator OPERATOR;		//!< Operator of this Operation.
  OperationFlags FLAGS;		//!< Operation flags.
  //@access isMemorySpill		((Operation_flags(this) & OperationFlag_MemorySpill) != 0)
  //@access isVolatile		((Operation_flags(this) & OperationFlag_Volatile) != 0)
  //@access isSPUpdate		((Operation_flags(this) & OperationFlag_SPUpdate) != 0)
  //@access isMemoryTouch		((Operation_flags(this) & OperationFlag_MemoryTouch) != 0)
  //@access isMemoryLevel2		((Operation_flags(this) & OperationFlag_MemoryLevel2) != 0)
  //@access isMemoryFence		((Operation_flags(this) & OperationFlag_MemoryFence) != 0)
  //@access hasClobber		((Operation_flags(this) & OperationFlag_Clobber) != 0)
  //@access isHoisted		((Operation_flags(this) & OperationFlag_Hoisted) != 0)
  //@access isDeadCode		((Operation_flags(this) & OperationFlag_DeadCode) != 0)
  //@access isCopyMode		((Operation_flags(this) & OperationFlag_CopyMode) != 0)
  //@access isMemorySafe	((Operation_flags(this) & OperationFlag_MemorySafe) != 0)
  //@access isPureCopy	((Operation_flags(this) & OperationFlag_PureCopy) != 0)
  //@access isEntryCode	((Operation_flags(this) & OperationFlag_EntryCode) != 0)
  //@access isExitCode	((Operation_flags(this) & OperationFlag_ExitCode) != 0)
  //@access isVisited	((Operation_flags(this) & OperationFlag_Visited) != 0)
  int32_t ORDERING;			//!< Operation ordering in BasicBlock.
  int8_t BOOSTING;			//!< Number of control speculations in trace.
  uint8_t INSTANCE;			//!< This Operation instance after loop unrolling.
  int16_t STARTDATE;			//!< Start date relative to BasicBlock start.
  int32_t RESCOUNT;			//!< Count of the Temporary results.
  int32_t ARGCOUNT;			//!< Count of the Temporary arguments.
  Temporary *OPERANDS;			//!< This Operation results and arguments.
  //@access RESULTS	(Operation_OPERANDS(this) + 0)
  //@access ARGUMENTS	(Operation_OPERANDS(this) + Operation_RESCOUNT(this))
  Register *REGISTERS;		//!< This Operation Register(s).
  //@access DEFREGISTERS	(Operation_REGISTERS(this) + 0)
  //@access USEREGISTERS	(Operation_REGISTERS(this) + Operation_RESCOUNT(this))
  OperationUnion_ UNION_;
  //@access MAXCOPIES	(Operation_UNION_(this).MAXCOPIES)
  //@mutate MAXCOPIES	(&Operation_UNION_(this).MAXCOPIES)
  //@access PHICOLOR	(Operation_UNION_(this).PHICOLOR)
  //@mutate PHICOLOR	(&Operation_UNION_(this).PHICOLOR)
  //@access PHICLASS	(Operation_UNION_(this).PHICLASS)
  //@mutate PHICLASS	(&Operation_UNION_(this).PHICLASS)
  //@access MAINEFFECTS	(Operation_UNION_(this).MAINEFFECTS)
  //@mutate MAINEFFECTS	(&Operation_UNION_(this).MAINEFFECTS)
  //@access RESEFFECTS	(Operation_MAINEFFECTS(this) + 0)
  //@access ARGEFFECTS	(Operation_MAINEFFECTS(this) + Operation_RESCOUNT(this))
  //@access CLOBBERSET	(RegisterSet)((Operation)(this) + 1)
};
#endif//$XCC_h

#ifdef $XCC_h
static inline Indexed
Operation__(Operation this) {
  return Operation____(this);
}
static inline const_Indexed
Operation__const(const_Operation this) {
  return Operation____(this);
}
#endif//$XCC_h

Operation
Operation_Ctor(Operation this,
               Operator operator, int32_t resCount, int32_t arcCount,
               const_RegisterSet clobberSet, struct BasicBlock_ *basicBlock)
{
  Indexed_Ctor(Operation____(this), Operation_Factory);
  *Operation__REPLICATED(this) = NULL;
  *Operation__BASICBLOCK(this) = basicBlock;
  *Operation__TASK(this) = NULL;
  *Operation__OPERATOR(this) = operator;
  *Operation__FLAGS(this) = clobberSet != NULL? OperationFlag_Clobber: 0;
  *Operation__ORDERING(this) = 0;
  *Operation__BOOSTING(this) = 0;
  *Operation__INSTANCE(this) = 0;
  *Operation__STARTDATE(this) = -1;
  *Operation__RESCOUNT(this) = resCount;
  *Operation__ARGCOUNT(this) = arcCount;
  *Operation__OPERANDS(this) = NULL;
  *Operation__REGISTERS(this) = NULL;
  *Operation__MAXCOPIES(this) = 0;
  if (clobberSet != NULL) {
    RegisterSet_Copy(Operation_CLOBBERSET(this), clobberSet);
  }
  return this;
}

size_t
Operation_Size(Operator operator, int32_t resCount, int32_t arcCount,
               const_RegisterSet clobberSet, struct BasicBlock_ *basicBlock)
{
  return (sizeof(Operation_) + (clobberSet != NULL? sizeof(RegisterSet_): 0));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Operation_)\t%zu\n", sizeof(Operation_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Next Operation in Selector or BasicBlock.
 */
static inline Operation
Operation_next(const_Operation this)
{
  return (Operation)IDListItem_NEXT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Previous Operation in Selector or BasicBlock.
 */
static inline Operation
Operation_prev(const_Operation this)
{
  return (Operation)IDListItem_PREV(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation identity.
 */
static inline uint32_t
Operation_identity(const_Operation this)
{
  return Operation_IDENTITY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation index.
 */
static inline int32_t
Operation_index(const_Operation this)
{
  return Operation_INDEX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Operation index.
 */
static inline void
Operation_setIndex(Operation this, int32_t index)
{
  *Operation__INDEX(this) = index;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation Operator.
 */
static inline Operator
Operation_operator(const_Operation this)
{
  return Operation_OPERATOR(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the operator of this Operation.
 */
static inline void
Operation_setOperator(Operation this, Operator operator)
{
  *Operation__OPERATOR(this) = operator;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The Operation this Operation was replicated from.
 */
static inline const_Operation
Operation_replicated(const_Operation this)
{
  return Operation_REPLICATED(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation BasicBlock.
 */
static inline struct BasicBlock_ *
Operation_basicBlock(const_Operation this)
{
  return Operation_BASICBLOCK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Operation basicBlock.
 */
static inline void
Operation_setBasicBlock(Operation this, struct BasicBlock_ *basicBlock)
{
  *Operation__BASICBLOCK(this) = basicBlock;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation InstructionTask.
 */
static inline InstructionTask
Operation_task(const_Operation this)
{
  return Operation_TASK(this);
}
#define Operation_instruction(this)	(Instruction)Operation_task(this)
#define Operation_issueItem(this)	(IssueItem)Operation_task(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Operation InstructionTask.
 */
static inline void
Operation_setTask(Operation this, InstructionTask task)
{
  *Operation__TASK(this) = task;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation flags.
 */
static inline unsigned
Operation_flags(const_Operation this)
{
  return Operation_FLAGS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Operation flags.
 */
static inline void
Operation_setFlags(Operation this, OperationFlags flags)
{
  *Operation__FLAGS(this) |= flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Reset this Operation flags.
 */
static inline void
Operation_clearFlags(Operation this, OperationFlags flags)
{
  *Operation__FLAGS(this) &= ~flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation ordering.
 */
static inline int32_t
Operation_ordering(const_Operation this)
{
  return Operation_ORDERING(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Operation ordering.
 */
static inline void
Operation_setOrdering(Operation this, int32_t ordering)
{
  *Operation__ORDERING(this) = ordering;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation boosting.
 */
static inline int
Operation_boosting(const_Operation this)
{
  return Operation_BOOSTING(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Operation boosting.
 */
static inline void
Operation_setBoosting(Operation this, int boosting)
{
  *Operation__BOOSTING(this) = boosting;
  Except_CHECK(Operation_BOOSTING(this) == boosting);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation instance.
 */
static inline int
Operation_instance(const_Operation this)
{
  return Operation_INSTANCE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Operation instance.
 */
static inline void
Operation_setInstance(Operation this, int instance)
{
  *Operation__INSTANCE(this) = instance;
  Except_CHECK(Operation_INSTANCE(this) == instance);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation start date.
 */
static inline int
Operation_startDate(const_Operation this)
{
  return Operation_STARTDATE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Operation start date.
 */
static inline void
Operation_setStartDate(Operation this, int startDate)
{
  *Operation__STARTDATE(this) = startDate;
  Except_CHECK(Operation_STARTDATE(this) == startDate);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count this Operation operands.
 */
static inline int32_t
Operation_operandCount(const_Operation this)
{
  return Operation_ARGCOUNT(this) + Operation_RESCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access this Operation operands.
 */
static inline Temporary *
Operation_operands(const_Operation this)
{
  return Operation_OPERANDS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this Operation OPERANDS.
 */
#define Operation_OPERANDS_FOREACH_Temporary(this, temporary) { \
  Temporary *Operation_OPERANDS = Operation_operands(this); \
  int32_t Operation_OPERANDCOUNT = Operation_operandCount(this); \
  int32_t Operation_OPERANDS_INDEX = 0; \
  for (; Operation_OPERANDS_INDEX < Operation_OPERANDCOUNT; \
         Operation_OPERANDS_INDEX++) { \
    Temporary temporary = Operation_OPERANDS[Operation_OPERANDS_INDEX];
#define Operation_OPERANDS_ENDEACH_Temporary \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count this Operation arguments.
 */
static inline int32_t
Operation_resCount(const_Operation this)
{
  return Operation_RESCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation results.
 */
static inline Temporary*
Operation_results(const_Operation this)
{
  return Operation_RESULTS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Operation RESULTS.
 */
#define Operation_RESULTS_FOREACH_Temporary(this, temporary) \
  Operation_RESULTS_FOREACH_Temporary_Index(this, temporary, Operation_RESULTS_INDEX)
#define Operation_RESULTS_ENDEACH_Temporary \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Operation RESULTS, defining the index of the result.
 */
#define Operation_RESULTS_FOREACH_Temporary_Index(this, temporary, index) { \
  Temporary *Operation_RESULTS = Operation_results(this); \
  int32_t Operation_RESCOUNT = Operation_resCount(this), index = 0; \
  for (; index < Operation_RESCOUNT; index++) { \
    Temporary temporary = Operation_RESULTS[index];
#define Operation_RESULTS_ENDEACH_Temporary_Index \
  } \
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Count this Operation arguments.
 */
static inline int32_t
Operation_argCount(const_Operation this)
{
  return Operation_ARGCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access this Operation arguments.
 */
static inline Temporary*
Operation_arguments(const_Operation this)
{
  return Operation_ARGUMENTS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Operation ARGUMENTS.
 */
#define Operation_ARGUMENTS_FOREACH_Temporary(this, temporary) { \
  Temporary *Operation_ARGUMENTS = Operation_arguments(this); \
  int32_t Operation_ARGCOUNT = Operation_argCount(this), Operation_ARGUMENTS_INDEX = 0; \
  for (; Operation_ARGUMENTS_INDEX < Operation_ARGCOUNT; Operation_ARGUMENTS_INDEX++) { \
    Temporary temporary = Operation_ARGUMENTS[Operation_ARGUMENTS_INDEX];
#define Operation_ARGUMENTS_ENDEACH_Temporary \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Operation ARGUMENTS, defining the index of the argument.
 */
#define Operation_ARGUMENTS_FOREACH_Temporary_Index(this, temporary, index) { \
  Temporary *Operation_ARGUMENTS = Operation_arguments(this); \
  int32_t Operation_ARGCOUNT = Operation_argCount(this), index = 0; \
  for (; index < Operation_ARGCOUNT; index++) { \
    Temporary temporary = Operation_ARGUMENTS[index];
#define Operation_ARGUMENTS_ENDEACH_Temporary_Index \
  } \
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Access this Operation Register(s).
 *   Returns NULL is this operation does not have register constraints.
 */
static inline Register *
Operation_registers(const_Operation this)
{
  return Operation_REGISTERS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns true if this operation has register constraints.
 */
static inline bool
Operation_hasRegisters(const_Operation this)
{
  return Operation_registers(this) != NULL;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Fix this Operation OPERANDS Temporary(ies) with OperatorParameter.
 */
void
Operation_fixOperandsWithParameters(Operation this);
#endif//$XCC_h

void
Operation_fixOperandsWithParameters(Operation this)
{
  int resCount = Operation_resCount(this);
  int argCount = Operation_argCount(this);
  Temporary *results = Operation_RESULTS(this);
  Temporary *arguments = Operation_ARGUMENTS(this);
  Operator operator = Operation_OPERATOR(this);
  const_OperatorParameterArray parameterArray = Operator_parameterArray(operator);
  Except_CHECK(resCount >= OperatorParameterArray_writeCount(parameterArray));
  Except_CHECK(argCount >= OperatorParameterArray_readCount(parameterArray));
  //
  OperatorParameterArray_FORWRITE_OperatorParameter(parameterArray, parameter) {
    int index = OperatorParameterArray_INDEX;
    if (index < resCount) {
      Temporary_fixWithParameter(results[index], parameter);
    } else break;
  } OperatorParameterArray_ENDWRITE_OperatorParameter;
  //
  OperatorParameterArray_FORREAD_OperatorParameter(parameterArray, parameter) {
    int index = OperatorParameterArray_INDEX;
    if (index < argCount) {
      Temporary_fixWithParameter(arguments[index], parameter);
    } else break;
  } OperatorParameterArray_ENDREAD_OperatorParameter;
}

#ifdef $XCC_h
/**
 * Make this Operation REGISTERS reflect the OPERANDS.
 */
void
Operation_makeRegisters(Operation this);
#endif//$XCC_h

void
Operation_makeRegisters(Operation this)
{
  int32_t operandCount = Operation_operandCount(this);
  Register *registers = Operation_REGISTERS(this);
  BasicBlock basicBlock = Operation_BASICBLOCK(this);
  Selector selector = BasicBlock_SELECTOR(basicBlock);
  Memory memory = Selector_MEMORY(selector);
  Except_CHECK(registers == NULL);
  Except_CHECK(!Operator_isParCopy(Operation_operator(this)));
  Except_CHECK(Operation_operator(this) != Operator_PHI);
  registers = Memory_alloc(memory, operandCount*sizeof(Register));
  Operation_OPERANDS_FOREACH_Temporary(this, temporary) {
    Register registre = Register__UNDEF;
    if (Temporary_hasRegister(temporary)) {
      registre = Temporary_REGISTER(temporary);
    }
    registers[Operation_OPERANDS_INDEX] = registre;
  } Operation_OPERANDS_ENDEACH_Temporary;
  *Operation__REGISTERS(this) = registers;
}

#ifdef $XCC_h
/**
 * Access this Operation def Register(s).
 */
static inline Register *
Operation_defRegisters(const_Operation this)
{
  if (!Operation_hasRegisters(this)) return NULL;
  return Operation_DEFREGISTERS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the def Register at resIndex in this Operation.
 */
static inline void
Operation_setDefRegister(Operation this, int32_t resIndex, Register reg)
{
  Except_REQUIRE(resIndex >= 0 && resIndex < Operation_RESCOUNT(this));
  //Except_REQUIRE(Operation_MAINEFFECTS(this) == NULL);
  Operation_DEFREGISTERS(this)[resIndex] = reg;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the register constraint on the given result.
 * Register__UNDEF is returned when there is no constraint, .
 */
static inline Register
Operation_getDefRegister(const_Operation this, int32_t resIndex)
{
  if (!Operation_hasRegisters(this)) return Register__UNDEF;
  return Operation_DEFREGISTERS(this)[resIndex];
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access this Operation use Register(s).
 */
static inline Register *
Operation_useRegisters(const_Operation this)
{
  if (!Operation_hasRegisters(this)) return NULL;
  return Operation_USEREGISTERS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the use Register at argIndex in this Operation.
 */
static inline void
Operation_setUseRegister(Operation this, int32_t argIndex, Register reg)
{
  Except_REQUIRE(argIndex >= 0 && argIndex < Operation_ARGCOUNT(this));
  //Except_REQUIRE(Operation_MAINEFFECTS(this) == NULL);
  Operation_USEREGISTERS(this)[argIndex] = reg;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the register constraint on the given argument.
 * Register__UNDEF is returned when there is no constraint, .
 */
static inline Register
Operation_getUseRegister(const_Operation this, int32_t argIndex)
{
  if (!Operation_hasRegisters(this)) return Register__UNDEF;
  return  Operation_USEREGISTERS(this)[argIndex];
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access this Operation main Effect(s).
 */
static inline struct Effect_ *
Operation_mainEffects(const_Operation this)
{
  return Operation_MAINEFFECTS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Operation main Effect(s).
 */
static inline void
Operation_setMainEffects(Operation this, struct Effect_ *mainEffects)
{
  *Operation__MAINEFFECTS(this) = mainEffects;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access this Operation result Effect(s).
 */
#define Operation_resEffects(this) \
  (Operation_mainEffects(this) + 0)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access this Operation argument Effect(s).
 */
#define Operation_argEffects(this) \
  (Operation_mainEffects(this) + Operation_resCount(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Operation RESEFFECTS.
 */
#define Operation_RESULTS_FOREACH_Effect(this, effect) { \
  struct Effect_ *Operation_RESEFFECTS = Operation_resEffects(this); \
  int32_t Operation_RESCOUNT = Operation_resCount(this), Operation_RESULTS_INDEX = 0; \
  for (; Operation_RESULTS_INDEX < Operation_RESCOUNT; Operation_RESULTS_INDEX++) { \
    Effect effect = Operation_RESEFFECTS + Operation_RESULTS_INDEX;
#define Operation_RESULTS_ENDEACH_Effect \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Operation ARGEFFECTS.
 */
#define Operation_ARGUMENTS_FOREACH_Effect(this, effect) { \
  struct Effect_ *Operation_ARGEFFECTS = Operation_argEffects(this); \
  int32_t Operation_ARGCOUNT = Operation_argCount(this), Operation_ARGUMENTS_INDEX = 0; \
  for (; Operation_ARGUMENTS_INDEX < Operation_ARGCOUNT; Operation_ARGUMENTS_INDEX++) { \
    Effect effect = Operation_ARGEFFECTS + Operation_ARGUMENTS_INDEX;
#define Operation_ARGUMENTS_ENDEACH_Effect \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access an Operation result by index.
 *
 * struct Effect_ *
 * Operation_getResEffect(const_Operation this, int32_t resIndex);
 */
#define Operation_getResEffect(this, resIndex) \
  (Operation_resEffects(this) + resIndex)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access an Operation argument by index.
 *
 * struct Effect_ *
 * Operation_getArgEffect(const_Operation this, int32_t argIndex);
 */
#define Operation_getArgEffect(this, argIndex) \
(Operation_argEffects(this) + argIndex)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operation clobber RegisterSet if any.
 */
static inline RegisterSet
Operation_clobberSet(const_Operation this)
{
  if (Operation_hasClobber(this)) {
    return Operation_CLOBBERSET(this);
  }
  return NULL;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Refresh this Operation flags that depend on operands.
 *
 * Must call before Operation_isCritical, Operation_maySpeculate, Operation_mayCompensate.
 */
void
Operation_refreshFlags(Operation this, Register stackRegister);
#endif//$XCC_h

void
Operation_refreshFlags(Operation this, Register stackRegister)
{
  Operation_RESULTS_FOREACH_Temporary(this, result) {
    if (   Temporary_hasRegister(result)
        && Temporary_REGISTER(result) == stackRegister) {
      *Operation__FLAGS(this) |= OperationFlag_SPUpdate;
      break;
    }
  } Operation_RESULTS_ENDEACH_Temporary;
}

#ifdef $XCC_h
/**
 * Check if this Operation is critical for dead code elimination.
 */
bool
Operation_isCritical(const_Operation this);
#endif//$XCC_h

bool
Operation_isCritical(const_Operation this)
{
  Operator operator = Operation_OPERATOR(this);
  unsigned flags = Operation_FLAGS(this);
  if (  flags
      & (  OperationFlag_Volatile
         | OperationFlag_SPUpdate
         | OperationFlag_MemoryTouch
         | OperationFlag_MemoryFence /*
         | OperationFlag_EntryCode
         | OperationFlag_ExitCode */)) return true;
  if (Operation_resCount(this) == 0) return true;
  Operation_RESULTS_FOREACH_Temporary(this, result) {
    if (!Temporary_isVariable(result)) return true;
  } Operation_RESULTS_ENDEACH_Temporary;
  if (Operator_isControl(operator)) return true;
  if (Operator_isMemoryWrite(operator)) return true;
  if (Operator_isMemoryEffect(operator)) return true;
  return false;
}

#ifdef $XCC_h
/**
 * Test if this Operation has a Dedicated result.
 *
 * Do not consider results on constant registers unless @c alsoConstant is true.
 */
bool
Operation_hasDedicatedResult(const_Operation this, bool alsoConstant);
#endif//$XCC_h

bool
Operation_hasDedicatedResult(const_Operation this, bool alsoConstant)
{
  if (alsoConstant) {
    Operation_RESULTS_FOREACH_Temporary(this, temporary) {
      if (Temporary_isDedicated(temporary)) return true;
    } Operation_RESULTS_ENDEACH_Temporary;
  } else {
    Operation_RESULTS_FOREACH_Temporary(this, temporary) {
      if (Temporary_isDedicated(temporary) &&
          !Temporary_isConstant(temporary)) return true;
    } Operation_RESULTS_ENDEACH_Temporary;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Test if this Operation has a Dedicated argument.
 *
 * Do not consider arguments on constant registers unless @c alsoConstant is true.
 */
bool
Operation_hasDedicatedArgument(const_Operation this, bool alsoConstant);
#endif//$XCC_h

bool
Operation_hasDedicatedArgument(const_Operation this, bool alsoConstant)
{
  if (alsoConstant) {
    Operation_ARGUMENTS_FOREACH_Temporary(this, temporary) {
      if (Temporary_isDedicated(temporary)) return true;
    } Operation_ARGUMENTS_ENDEACH_Temporary;
  } else {
    Operation_ARGUMENTS_FOREACH_Temporary(this, temporary) {
      if (Temporary_isDedicated(temporary) &&
          !Temporary_isConstant(temporary)) return true;
    } Operation_ARGUMENTS_ENDEACH_Temporary;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Get the Dedicated results in a RegisterSet.
 */
void
Operation_getDedicatedResults(const_Operation this, RegisterSet dedicatedSet);
#endif//$XCC_h

void
Operation_getDedicatedResults(const_Operation this, RegisterSet dedicatedSet)
{
  Operation_RESULTS_FOREACH_Temporary(this, temporary) {
    if (Temporary_isDedicated(temporary)) {
      Register registre = Temporary_REGISTER(temporary);
      RegisterSet_insert(dedicatedSet, registre);
    }
  } Operation_RESULTS_ENDEACH_Temporary;
}

#ifdef $XCC_h
/**
 * Get the Dedicated arguments in a RegisterSet.
 */
void
Operation_getDedicatedArguments(const_Operation this, RegisterSet dedicatedSet);
#endif//$XCC_h

void
Operation_getDedicatedArguments(const_Operation this, RegisterSet dedicatedSet)
{
  Operation_ARGUMENTS_FOREACH_Temporary(this, temporary) {
    if (Temporary_isDedicated(temporary)) {
      Register registre = Temporary_REGISTER(temporary);
      RegisterSet_insert(dedicatedSet, registre);
    }
  } Operation_ARGUMENTS_ENDEACH_Temporary;
}

#ifdef $XCC_h
/**
 * Add a result Temporary to an Operation.
 */
int32_t
Operation_appendResult(Operation this, Temporary temporary);
#endif//$XCC_h

int32_t
Operation_appendResult(Operation this, Temporary temporary)
{
  int32_t resCount = Operation_RESCOUNT(this), i;
  Temporary *results = Operation_RESULTS(this);
  Except_REQUIRE(Operation_MAINEFFECTS(this) == NULL);
  for (i = 0; i < resCount; i++) {
    if (results[i] == NULL) {
      results[i] = temporary;
      return i;
    }
  }
  Except_NEVER(true);
  return -1;
}

#ifdef $XCC_h
/**
 * Add an argument Temporary to an Operation.
 */
int32_t
Operation_appendArgument(Operation this, Temporary temporary);
#endif//$XCC_h

int32_t
Operation_appendArgument(Operation this, Temporary temporary)
{
  int32_t argCount = Operation_ARGCOUNT(this), i;
  Temporary *arguments = Operation_ARGUMENTS(this);
  Except_REQUIRE(Operation_MAINEFFECTS(this) == NULL);
  for (i = 0; i < argCount; i++) {
    if (arguments[i] == NULL) {
      arguments[i] = temporary;
      return i;
    }
  }
  Except_NEVER(true);
  return -1;
}

#ifdef $XCC_h
/**
 * Get the result Temporary at resIndex in this Operation.
 */
static inline Temporary
Operation_getResult(Operation this, int32_t resIndex)
{
  Except_REQUIRE(resIndex >= 0 && resIndex < Operation_RESCOUNT(this));
  return Operation_RESULTS(this)[resIndex];
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the result Temporary at resIndex in this Operation.
 */
static inline void
Operation_setResult(Operation this, int32_t resIndex, Temporary temporary)
{
  Except_REQUIRE(resIndex >= 0 && resIndex < Operation_RESCOUNT(this));
  //Except_REQUIRE(Operation_MAINEFFECTS(this) == NULL);
  Operation_RESULTS(this)[resIndex] = temporary;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the argument Temporary at argIndex in this Operation.
 */
static inline Temporary
Operation_getArgument(Operation this, int32_t argIndex)
{
  Except_REQUIRE(argIndex >= 0 && argIndex < Operation_ARGCOUNT(this));
  return Operation_ARGUMENTS(this)[argIndex];
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the argument Temporary at argIndex in this Operation.
 */
static inline void
Operation_setArgument(Operation this, int32_t argIndex, Temporary temporary)
{
  Except_REQUIRE(argIndex >= 0 && argIndex < Operation_ARGCOUNT(this));
  //Except_REQUIRE(Operation_MAINEFFECTS(this) == NULL);
  Operation_ARGUMENTS(this)[argIndex] = temporary;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Resize this Operation operand array.
 */
void
Operation_resizeOperandArray_(Operation this, int32_t opCount);
#endif//$XCC_h

void
Operation_resizeOperandArray_(Operation this, int32_t opCount)
{
  int32_t old_resCount = Operation_RESCOUNT(this), i;
  int32_t old_argCount = Operation_ARGCOUNT(this), j;
  int32_t old_clp2 = HackerU32_clp2(old_resCount + old_argCount);
  if (opCount > old_clp2) {
    // Need to allocate new OPERANDS array.
    BasicBlock basicBlock = Operation_BASICBLOCK(this);
    Selector selector = BasicBlock_SELECTOR(basicBlock);
    Memory memory = Selector_MEMORY(selector);
    int32_t clp2Size = HackerU32_clp2(opCount)*sizeof(Temporary);
    Temporary *old_operands = Operation_OPERANDS(this);
    Temporary *operands = memset(Memory_alloc(memory, clp2Size), 0, clp2Size);
    // Copy operands
    for (i = 0; i < old_clp2; i++) operands[i] = old_operands[i];
    *Operation__OPERANDS(this) = operands;
    Memory_free(memory, old_operands);
  }
}



#ifdef $XCC_h
/**
 * Resize this Operation arguments or results.
 *
 * Invariant:	The @c operands array has @c results followed by @c arguments.
 */
void
Operation_resizeOperands(Operation this, int32_t resCount, int32_t argCount);
#endif//$XCC_h

void
Operation_resizeOperands(Operation this, int32_t resCount, int32_t argCount)
{
  int32_t old_resCount = Operation_RESCOUNT(this), i;
  int32_t old_argCount = Operation_ARGCOUNT(this), j;
  int32_t old_clp2 = HackerU32_clp2(old_resCount + old_argCount);
  Temporary *old_operands = Operation_OPERANDS(this);
  Temporary *old_results = old_operands + 0;
  Temporary *old_arguments = old_operands + old_resCount;
  if (resCount + argCount <= old_clp2) {
    // No need to reallocate operands.
    Temporary *results = old_results;
    Temporary *arguments = old_operands + resCount;
    int32_t min_argCount = old_argCount < argCount? old_argCount: argCount;
    if (resCount > old_resCount) {
      // Move right the arguments by old_resCount - resCount positions.
      for (j = min_argCount - 1; j >= 0; j--) arguments[j] = old_arguments[j];
    } else
    if (resCount < old_resCount) {
      // Move left the arguments by old_resCount - resCount positions.
      for (j = 0; j < min_argCount; j++) arguments[j] = old_arguments[j];
    }
    // Clear the unused results.
    for (i = old_resCount; i < resCount; i++) results[i] = NULL;
    // Clear the unused arguments.
    for (j = min_argCount; j < old_argCount + old_resCount - resCount; j++) {
      arguments[j] = NULL;
    }
  } else {
    // Need to allocate new OPERANDS array.
    BasicBlock basicBlock = Operation_BASICBLOCK(this);
    Selector selector = BasicBlock_SELECTOR(basicBlock);
    Memory memory = Selector_MEMORY(selector);
    int32_t clp2Size = HackerU32_clp2(resCount + argCount)*sizeof(Temporary);
    Temporary *operands = memset(Memory_alloc(memory, clp2Size), 0, clp2Size);
    Temporary *results = operands + 0;
    Temporary *arguments = operands + resCount;
    // Copy results.
    if (old_resCount > resCount) old_resCount = resCount;
    for (i = 0; i < old_resCount; i++) results[i] = old_results[i];
    // Copy arguments.
    if (old_argCount > argCount) old_argCount = argCount;
    for (j = 0; j < old_argCount; j++) arguments[j] = old_arguments[j];
    *Operation__OPERANDS(this) = operands;
    Memory_free(memory, old_operands);
  }
  *Operation__ARGCOUNT(this) = argCount;
  *Operation__RESCOUNT(this) = resCount;
}


#ifdef $XCC__c
{
  Operation_ operation[1];
  Temporary operands[8];
  int resCount, i, argCount, j;
  int old_resCount = 2, old_argCount = 3;
  *Operation__OPERANDS(operation) = operands;
  printf("Test Operation_resizeOperands\n");
  for (resCount = old_resCount - 1; resCount <= old_resCount + 1; resCount++) {
    for (argCount = old_argCount - 1; argCount <= old_argCount + 1; argCount++) {
      memset(operands, 0, sizeof(operands));
      *Operation__RESCOUNT(operation) = old_resCount;
      *Operation__ARGCOUNT(operation) = old_argCount;
      for (i = 0; i < old_resCount; i++) operands[i] = (void *)(intptr_t)('a' + i);
      for (j = 0; j < old_argCount; j++) operands[old_resCount + j] = (void *)(intptr_t)('x' + j);
      printf("Operation\t:");
      for (i = 0; i < sizeof(operands)/sizeof(*operands); i++) {
        printf("   %c\t%c",
               operands[i]? (int)(intptr_t)operands[i]: '_',
               i == Operation_RESCOUNT(operation) - 1? '|': '.');
      }
      printf("\n");
      Operation_resizeOperands(operation, resCount, argCount);
      printf("resize(%d,%d)\t:", resCount, argCount);
      for (i = 0; i < sizeof(operands)/sizeof(*operands); i++) {
        printf("   %c\t%c",
               operands[i]? (int)(intptr_t)operands[i]: '_',
               i == Operation_RESCOUNT(operation) - 1? '|': '.');
      }
      printf("\n");
    }
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Check if this Operation is a branch
 */
static inline bool
Operation_isBranch(const_Operation this)
{
  return Operator_isBranch(Operation_OPERATOR(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check if this Operation is a control operation
 */
static inline bool
Operation_isControl(const_Operation this)
{
  return Operator_isControl(Operation_OPERATOR(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check if this Operation is GoTo.
 */
static inline bool
Operation_isGoTo(const_Operation this)
{
  return Operator_isGoTo(Operation_OPERATOR(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check if this Operation is Routine.
 */
static inline bool
Operation_isRoutine(const_Operation this)
{
  return Operator_isRoutine(Operation_OPERATOR(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check if this Operation is Return.
 */
static inline bool
Operation_isReturn(const_Operation this)
{
  return Operator_isReturn(Operation_OPERATOR(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check if this Operation is Conditional.
 */
static inline bool
Operation_isConditional(const_Operation this)
{
  return Operator_isConditional(Operation_OPERATOR(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check if this Operation may be speculated.
 */
bool
Operation_maySpeculate(const_Operation this, Processor processor, int speculation);
#endif//$XCC_h

bool
Operation_maySpeculate(const_Operation this, Processor processor, int speculation)
{
  Operator operator = Operation_OPERATOR(this);
  OperationFlags this_flags = Operation_FLAGS(this);
  OperationFlags forbid_flags = OperationFlag_Volatile
                              | OperationFlag_SPUpdate
                              | OperationFlag_MemoryTouch
                              | OperationFlag_MemoryFence
                              | OperationFlag_EntryCode
                              | OperationFlag_ExitCode;
  OperationFlags allow_flags = OperationFlag_MemorySafe;
  int this_speculation = ConfigureSpeculation_SafeArith;
  if (this_flags & forbid_flags) return false;
  if (Operator_isControl(operator)) return false;
  if (Operator_isStalling(operator)) return false;
  if (Operator_isMemoryWrite(operator)) return false;
  if (Operator_isMemoryEffect(operator)) return false;
  if (Operator_isMemoryRead(operator)) {
    this_speculation = ConfigureSpeculation_MayExcept;
    if (   (this_flags & allow_flags)
        || Operator_isDismissible(operator)) {
      this_speculation = ConfigureSpeculation_MemorySafe;
    } else {
      Operator speculate = Operator_speculate(operator, processor);
      if (speculate != operator && speculate != Operator__UNDEF) {
        this_speculation = ConfigureSpeculation_SafeVariant;
      }
    }
  }
  if (this_speculation > speculation) return false;
  return true;
}

#ifdef $XCC_h
/**
 * Speculate this Operation.
 *
 * Require:	Operation_maySpeculate is true.
 */
void
Operation_speculate(Operation this, Processor processor);
#endif//$XCC_h

void
Operation_speculate(Operation this, Processor processor)
{
  if (!Operation_isMemorySafe(this)) {
    Operator operator = Operator_speculate(Operation_OPERATOR(this), processor);
    Except_CHECK(operator != Operator__UNDEF);
    *Operation__OPERATOR(this) = operator;
  }
}

#ifdef $XCC_h
/**
 * Check if this Operation may be compensated.
 */
bool
Operation_mayCompensate(const_Operation this, Processor processor, int compensation);
#endif//$XCC_h

bool
Operation_mayCompensate(const_Operation this, Processor processor, int compensation)
{
  Operator operator = Operation_OPERATOR(this);
  OperationFlags this_flags = Operation_FLAGS(this);
  OperationFlags forbid_flags = OperationFlag_Volatile
                              | OperationFlag_SPUpdate
                              | OperationFlag_MemoryTouch
                              | OperationFlag_MemoryFence
                              | OperationFlag_EntryCode
                              | OperationFlag_ExitCode;
  OperationFlags allow_flags = OperationFlag_MemorySafe;
  int this_compensation = ConfigureCompensation_Local;
  if (this_flags & forbid_flags) return false;
  if (Operator_isControl(operator)) return false;
  if (Operator_isMemory(operator)) {
    this_compensation = ConfigureCompensation_Global;
  }
  if (Operator_isMemoryRead(operator)) {
    if (   (this_flags & allow_flags)
        || Operator_isDismissible(operator)) {
      this_compensation = ConfigureCompensation_Local;
    }
  }
  if (this_compensation > compensation) return false;
  return true;
}

#ifdef $XCC_h
/**
 * Check if this Operation may be predicated positive.
 */
bool
Operation_mayPredicatePositive(const_Operation this, Processor processor);
#endif//$XCC_h

bool
Operation_mayPredicatePositive(const_Operation this, Processor processor)
{
  Operator operator = Operator_predicatePositive(Operation_OPERATOR(this), processor);
  return operator != Operator__UNDEF;
}

#ifdef $XCC_h
/**
 * Check if this Operation may be predicated negative.
 */
bool
Operation_mayPredicateNegative(const_Operation this, Processor processor);
#endif//$XCC_h

bool
Operation_mayPredicateNegative(const_Operation this, Processor processor)
{
  Operator operator = Operator_predicateNegative(Operation_OPERATOR(this), processor);
  return operator != Operator__UNDEF;
}

#ifdef $XCC_h
/**
 * Predication this Operation.
 */
bool
Operation_predicate(Operation this, Processor processor,
                    Temporary predicate, int polarity);
#endif//$XCC_h

bool
Operation_predicate(Operation this, Processor processor,
                    Temporary predicate, int polarity)
{
  int32_t resCount = Operation_RESCOUNT(this);
  int32_t argCount = Operation_ARGCOUNT(this);
  Operator operator = Operation_OPERATOR(this);
  Operator predicated = polarity > 0?
                        Operator_predicatePositive(operator, processor):
                        Operator_predicateNegative(operator, processor);
  int32_t predicateIndex = Operator_predicateIndex(predicated), index;
  Temporary *arguments = NULL, *results = NULL;
  if ((predicated != Operator__UNDEF && predicateIndex >= 0)) {
    int32_t readCount = Operator_readCount(predicated);
    int32_t writeCount = Operator_writeCount(predicated);
    Except_CHECK(readCount == argCount + 1 + resCount && writeCount == resCount);
    Operation_resizeOperands(this, resCount, argCount + 1 + resCount);
    arguments = Operation_ARGUMENTS(this);
    // Insert predicate argument.
    for (index = argCount - 1; index >= predicateIndex; index--) {
      arguments[index + 1] = arguments[index];
    }
    arguments[predicateIndex] = predicate;
    // Copy results Temporary(ies) to extra arguments.
    results = Operation_RESULTS(this);
    for (index = 0; index < resCount; index++) {
      arguments[argCount + 1 + index] = results[index];
    }
    // Update Operator.
    *Operation__OPERATOR(this) = predicated;
    return true;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Check if this Operation may be predicated negative.
 */
bool
Operation_mayUnpredicate(const_Operation this, Processor processor);
#endif//$XCC_h

bool
Operation_mayUnpredicate(const_Operation this, Processor processor)
{
  Operator operator = Operator_unpredicate(Operation_OPERATOR(this), processor);
  return operator != Operator__UNDEF;
}

#ifdef $XCC_h
/**
 * Un-predicate this Operation.
 */
void
Operation_unpredicate(Operation this, Processor processor);
#endif//$XCC_h

void
Operation_unpredicate(Operation this, Processor processor)
{
  Temporary *arguments = Operation_ARGUMENTS(this);
  int32_t resCount = Operation_RESCOUNT(this);
  int32_t argCount = Operation_ARGCOUNT(this);
  Operator operator = Operation_OPERATOR(this);
  Operator unpredicated = Operator_unpredicate(operator, processor);
  int32_t predicateIndex = Operator_predicateIndex(operator), index;
  Except_CHECK(unpredicated != Operator__UNDEF && predicateIndex >= 0);
  for (index = predicateIndex; index < argCount - 1; index++) {
    arguments[index] = arguments[index + 1];
  }
  Operation_resizeOperands(this, resCount, argCount - 1 - resCount);
  *Operation__OPERATOR(this) = unpredicated;
}

#ifdef $XCC_h
/**
 * Get the predicate of this Operation.
 */
Temporary
Operation_getPredicate(Operation this);
#endif//$XCC_h

Temporary
Operation_getPredicate(Operation this)
{
  Operator operator = Operation_OPERATOR(this);
  int predicateIndex = Operator_predicateIndex(operator);
  if (predicateIndex >= 0) {
    Temporary *arguments = Operation_ARGUMENTS(this);
    Temporary predicate = arguments[predicateIndex];
    return predicate;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Test if this Operation is additive.
 *
 * An operator OP is additive if OP(a, b) = OP(a+c, b-c).
 */
bool
Operation_isAdditive(const_Operation this, int aIndex, int bIndex);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test if this Operation is subtractive.
 *
 * An operator OP is subtractive if OP(a, b) = OP(a+c, b+c).
 */
bool
Operation_isSubtractive(const_Operation this, int aIndex, int bIndex);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test if this Operation is translative.
 *
 * An operator OP is translative if OP(a+c, b+c) = OP(a, b) + c.
 */
bool
Operation_isTranslative(const_Operation this, int aIndex, int bIndex);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test if this Operation is associative.
 *
 * An operator OP is associative if OP(OP(a, b), c) = OP(a, OP(b, c)).
 */
bool
Operation_isAssociative(const_Operation this, int aIndex, int bIndex);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test if this Operation is commutative.
 *
 * An operator OP is commutative if OP(a, b) = OP(b, a).
 */
bool
Operation_isCommutative(const_Operation this, int aIndex, int bIndex);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check this Operation may be complemented on result.
 */
bool
Operation_mayComplementResult(const_Operation this, Processor processor, int32_t resIndex);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Complement this operation on the designated result.
 */
void
Operation_complementResult(Operation this, Processor processor, int32_t resIndex);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check this Operation may be complemented on argument.
 */
bool
Operation_mayComplementArgument(const_Operation this, Processor processor, int32_t argIndex);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Complement this operation on the designated argument.
 */
void
Operation_complementArgument(Operation this, Processor processor, int32_t argIndex);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Try to invert a GoTo Operation.
 */
bool
Operation_tryInvertGoTo(Operation this, Processor processor);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check this Operation may become a pre-load.
 */
bool
Operation_mayMemoryLevel2(const_Operation this, int32_t regIndex);
#endif//$XCC_h

bool
Operation_mayMemoryLevel2(const_Operation this, int32_t regIndex)
{
  Operator operator = Operation_OPERATOR(this);
  if (Operator_isMemoryRead(operator)) {
    return regIndex == Operator_baseAddressIndex(operator);
  }
  return false;
}

#ifdef $XCC_h
/**
 * The unit cost estimate of this Operation.
 */
int
Operation_unitCost(const_Operation this, Processor processor);
#endif//$XCC_h

int
Operation_unitCost(const_Operation this, Processor processor)
{
  Operator operator = Operation_OPERATOR(this);
  Opcode opcode = Operator_opcode(operator, processor);
  return Opcode_unitCost(opcode);
}

#ifdef $XCC_h
/**
 * The target Temporary of control or label * Operation.
 */
static inline Temporary
Operation_targetTemporary(const_Operation this)
{
  int32_t targetIndex = Operator_targetIndex(Operation_OPERATOR(this));
  const Temporary *arguments = Operation_ARGUMENTS(this);
  Temporary target_temporary = arguments[targetIndex];
  Except_CHECK(targetIndex >= 0);
  return target_temporary;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the target label of an operation.
 */
static inline Label
Operation_getTargetLabel(const_Operation this)
{
  if (Operator_isGNUASM(Operation_operator(this))) {
    int i;
    Temporary * arguments = Operation_arguments(this);

    for (i = 0; i < Operation_argCount(this); i++) {
      if (Temporary_isLabel(arguments[i])) {
        Literal literal = Temporary_LITERAL(arguments[i]);
        return Literal_label(literal);
      }
    }
  } else {
    Temporary target_temporary = Operation_targetTemporary(this);
    if (Temporary_isLabel(target_temporary)) {
      Literal literal = Temporary_LITERAL(target_temporary);
      return Literal_label(literal);
    }
  }
  return NULL;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get the step argIndex if this Operation adds an Absolute.
 */
static inline int32_t
Operation_getStepIndex(const_Operation this)
{
  Operator operator = Operation_OPERATOR(this);
  if (Operator_isAddressAdd(operator)) {
    int32_t immediateIndex = Operator_immediateIndex(operator);
    if (immediateIndex >= 0) {
      Temporary *arguments = Operation_ARGUMENTS(this);
      Temporary temporary = arguments[immediateIndex];
      if (Temporary_isAbsolute(temporary)) {
        return immediateIndex;
      }
    }
  }
  return -1;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get this Operation relaxable value for inductive relaxation.
 * @c overlap:	The maximum overlap possible in the schedule.
 * @c step:	The simple induction step value.
 * Return:	The amount of inductive relaxation applicable.
 *
 * Inductive relaxation enables the definition of a simple induction to be
 * scheduled after the use of that induction, provided the use is an additive /
 * substractive operator with an immediate value. This function computes the
 * relaxable value, that is, the coefficient of LAMBDA in the RAW dependence from
 * the definition to the use.
 *
 * TODO: support for relaxing the WAR between Operation use and induction def.
 */
int
Operation_getRelaxable(const_Operation this, int32_t regIndex, int overlap, int64_t step);
#endif//$XCC_h

int
Operation_getRelaxable(const_Operation this, int32_t regIndex, int overlap, int64_t step)
{
  int relaxable = overlap + 1;
  Operator operator = Operation_OPERATOR(this);
  int32_t immediateIndex = Operator_immediateIndex(operator);
  Except_REQUIRE(overlap >= 0);
  if (immediateIndex >= 0) {
    const Temporary *arguments = Operation_ARGUMENTS(this);
    Temporary temporary = arguments[immediateIndex];
    if (Temporary_isRelaxable(temporary)) {
      const_OperatorParameter immediateParameter =
          Operator_readParameter(operator, immediateIndex);
      Immediate immediate = OperatorParameter_enum(immediateParameter);
      int64_t min = Immediate_minValue(immediate);
      int64_t max = Immediate_maxValue(immediate);
      ImmediateValue m = Temporary_isAbsolute(temporary)?
                         Literal_value(Temporary_LITERAL(temporary)): 0;
      Except_CHECK(OperatorParameter_type(immediateParameter) == OperandType_Immediate);
      if (Operation_isAdditive(this, regIndex, immediateIndex)) {
        // Cases like LOAD(ADD(x, step), m): m -> m+relaxable*step
        if (step > 0) {
          for (; relaxable > 0; --relaxable) {
            if (m + relaxable*step <= max) return relaxable;
          }
        } else if (step < 0) {
          for (; relaxable > 0; --relaxable) {
            if (m + relaxable*step >= min) return relaxable;
          }
        } else return relaxable;
      } else if (Operation_isSubtractive(this, regIndex, immediateIndex)) {
        // Cases like CMPEQ(ADD(x, step), m): m -> m-relaxable*step
        if (step > 0) {
          for (; relaxable > 0; --relaxable) {
            if (m - relaxable*step >= min) return relaxable;
          }
        } else if (step < 0) {
          for (; relaxable > 0; --relaxable) {
            if (m - relaxable*step <= max) return relaxable;
          }
        } else return relaxable;
      }
    }
  }
  return 0;
}

#ifdef $XCC_h
/**
 * Check if this Operation is a padded memory load.
 */
bool
Operation_isPaddedLoad(const_Operation this,
                       int prePadding,
                       int postPadding,
                       int64_t distance);
#endif//$XCC_h

bool
Operation_isPaddedLoad(const_Operation this,
                       int prePadding,
                       int postPadding,
                       int64_t distance)
{
  Operator operator = Operation_OPERATOR(this);
  if (Operator_isMemoryRead(operator)) {
    int memorySize = Operator_memorySize(operator);
    if ((distance >= 0 && distance + memorySize <= postPadding) ||
        (distance < 0 && -distance + memorySize <= prePadding)) {
      return true;
    }
  }
  return false;
}

#ifdef $XCC_h
/**
 * The minimum omega for two memory accesses to
 * overlap, assuming a same base address, and a simple induction step.
 * @c adjust:	+1 if the induction update is between this and that, -1 if the
 *               induction update is between that and this, else 0.
 * @c overlap:	The maximum overlap of the loop iterations.
 * @c step:	The simple induction step
 * Return:	The omega value, or overlap + 1 if no overlap detected.
 */
int
Operation_getMemoryDistance(const_Operation this, const_Operation that,
                            int adjust, int overlap, int64_t step);
#endif//$XCC_h

int
Operation_getMemoryDistance(const_Operation this, const_Operation that,
                            int adjust, int overlap, int64_t step)
{
  Operator this_operator = Operation_OPERATOR(this);
  Operator that_operator = Operation_OPERATOR(that);
  int this_memorySize = Operator_memorySize(this_operator);
  int that_memorySize = Operator_memorySize(that_operator);
  int carried = Operation_INDEX(this) >= Operation_INDEX(that);
  Except_REQUIRE(this_memorySize != 0 && that_memorySize != 0 && overlap >= 0);
  if (this_memorySize > 0 && that_memorySize > 0) {
    int this_immediateIndex = Operator_immediateIndex(this_operator);
    int that_immediateIndex = Operator_immediateIndex(that_operator);
    if (this_immediateIndex >= 0 && that_immediateIndex >= 0) {
      Temporary this_temporary = Operation_ARGUMENTS(this)[this_immediateIndex];
      Temporary that_temporary = Operation_ARGUMENTS(that)[that_immediateIndex];
      if (Temporary_isAbsolute(this_temporary) &&
          Temporary_isAbsolute(that_temporary)) {
        Literal this_literal = Temporary_LITERAL(this_temporary);
        Literal that_literal = Temporary_LITERAL(that_temporary);
        int64_t this_offset = Literal_value(this_literal);
        int64_t that_offset = Literal_value(that_literal)
                            + step*(adjust + carried);
        if (this_offset == that_offset) return carried;
        if (this_offset < that_offset) {
          if (step >= 0) {
            if (this_offset + this_memorySize <= that_offset) return overlap + 1;
            else return carried;
          } else /* step < 0 */ {
            int omega = 0;
            for (; omega <= overlap - carried; omega++) {
              int64_t bias_offset = that_offset + step*omega;
              if (this_offset + this_memorySize > bias_offset) break;
            }
            return omega + carried;
          }
        } else /* this_offset > that_offset */ {
          if (step <= 0) {
            if (that_offset + that_memorySize <= this_offset) return overlap + 1;
            else return carried;
          } else /* step > 0 */ {
            int omega = 0;
            for (; omega <= overlap - carried; omega++) {
              int64_t bias_offset = that_offset + step*omega;
              if (bias_offset + that_memorySize <= this_offset) break;
            }
            return omega + carried;
          }
        }
      }
    }
  }
  return carried;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make this Operation Instruction and push on NativeBlock.
 */
Instruction
Operation_makeInstruction(Operation this, Processor processor, NativeBlock block);
#endif//$XCC_h

Instruction
Operation_makeInstruction(Operation this, Processor processor, NativeBlock block)
{
  Operator operator = Operation_OPERATOR(this);
  Opcode opcode = Operator_opcode(operator, processor);
  NativeStream stream = NativeBlock_nativeStream(block);
  Instruction instruction = NativeBlock_makeInstruction(block, opcode, NULL);
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded = Operation_getOperandDecoded(this, decoded, instruction);
  {
    // FIXME: comment this and uncomment in PostPass_activateState.
    Memory memory = NativeBlock_MEMORY(block);
    InstructionAction_ *actions = Memory_alloc(memory, Instruction_sizeActions(instruction));
    InstructionFlags flags = OperationFlags_makeInstructionFlags(Operation_FLAGS(this));
    //if (Operator_isControl(operator)) flags |= InstructionFlag_Control;
    Instruction_setIndexed(instruction, Operation__(this));
    Instruction_setFlags(instruction, flags);
    Instruction_makeActions(instruction, actions);
    return instruction;
  }
}

#ifdef $XCC_h
/**
 * Get decoded operands
 */
OperandDecoded*
Operation_getOperandDecoded(Operation this, OperandDecoded *decoded, 
                            Instruction instruction);
#endif//$XCC_h

OperandDecoded*
Operation_getOperandDecoded(Operation this, OperandDecoded *decoded,
                            Instruction instruction)
{
  Operator operator = Operation_OPERATOR(this);  
  const_OperatorParameterArray parameterArray = Operator_parameterArray(operator);
  const OperatorParameter_ *writeParameters =
      OperatorParameterArray_writeParameters(parameterArray);
  const OperatorParameter_ *readParameters =
      OperatorParameterArray_readParameters(parameterArray);
  int readCount = OperatorParameterArray_readCount(parameterArray);
  int writeCount = OperatorParameterArray_writeCount(parameterArray);
  Temporary *arguments = Operation_ARGUMENTS(this);
  Temporary *results = Operation_RESULTS(this);
  int argCount = Operation_argCount(this);
  int resCount = Operation_resCount(this);
  //
  Operation_RESULTS_FOREACH_Temporary(this, temporary) {
    const_OperatorParameter parameter = writeParameters;
    int rank = OperatorParameter_rank(parameter), index = rank - 1;
    if (--writeCount < 0) break;
    if (rank > 0) {
      if (Temporary_hasRegister(temporary)) {
        decoded[index] = Temporary_REGISTER(temporary);
      } else Except_NEVER(true);
    }
    ++writeParameters;
    ++results, --resCount;
  } Operation_RESULTS_ENDEACH_Temporary;
  //
  Operation_ARGUMENTS_FOREACH_Temporary(this, temporary) {
    const_OperatorParameter parameter = readParameters;
    int rank = OperatorParameter_rank(parameter), index = rank - 1;
    if (--readCount < 0) break;
    if (rank > 0) {
      OperandType operandType = OperatorParameter_type(parameter);
      if (Temporary_hasRegister(temporary)) {
        decoded[index] = Temporary_REGISTER(temporary);
      } else if (operandType == OperandType_Immediate) {
        Literal literal = Temporary_LITERAL(temporary);
        if (Temporary_isAbsolute(temporary)) {
          ImmediateValue value = Literal_value(literal);
          decoded[index] = (OperandDecoded)value;
        } else if (Temporary_isSymbol(temporary)) {
          decoded[index] = (OperandDecoded)Literal_offset(literal);
          if (instruction != NULL)
          {
            Instruction_setLiteral(instruction, literal);
            Instruction_setLiteralIndex(instruction, index);
          }
        } else if (Temporary_isLabel(temporary)) {          
          decoded[index] = (OperandDecoded)Literal_offset(literal);
          if (instruction != NULL)
          {
            Instruction_setLiteral(instruction, literal);
            Instruction_setLiteralIndex(instruction, index);
          }
        } else Except_NEVER(true);
//       } else if (operandType == OperandType_Modifier) {
//         ModifierMember member = Temporary_MEMBER(temporary);
//         decoded[index] = (OperandDecoded)member;
      } else Except_NEVER(true);
    }
    ++readParameters;
    ++arguments, --argCount;
  } Operation_ARGUMENTS_ENDEACH_Temporary;
  
  {
    int index = 0;
    OperatorModify_ modify_ = Operator_modify_(operator, index++);
    if (modify_.RANK) {
      do {
         decoded[modify_.RANK-1] = (OperandDecoded)modify_.MEMBER;         
         modify_ = Operator_modify_(operator, index++);
      } while (modify_.RANK);
    }
  }  
  return decoded;
}

#ifdef $XCC_h
/**
 * Compare for sorting Operations by INDEX.
 */
int
Operation_compareIndices(const void *this_pointer, const void *that_pointer);
#endif//$XCC_h

int
Operation_compareIndices(const void *this_pointer, const void *that_pointer)
{
  const_Operation this = this_pointer;
  const_Operation that = that_pointer;
  intptr_t this_index = Operation_INDEX(this);
  intptr_t that_index = Operation_INDEX(that);
  return (this_index > that_index) - (this_index < that_index);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty-print this Operation.
 */
bool
Operation_pretty(const_Operation this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Operation_pretty(const_Operation this, FILE *file)
{
  const char *sep = " ";
  Operator operator = Operation_OPERATOR(this);
  BasicBlock basicBlock = Operation_BASICBLOCK(this);
  Processor processor = basicBlock != NULL?
                        BasicBlock_PROCESSOR(basicBlock):
                        Processor__UNDEF;
  const_OperatorParameterArray parameterArray = Operator_parameterArray(operator);
  const OperatorParameter_ *writeParameters = OperatorParameterArray_writeParameters(parameterArray);
  const OperatorParameter_ *readParameters = OperatorParameterArray_readParameters(parameterArray);
  Temporary *decoded = alloca(sizeof(Temporary)*Operands__MAXOPERANDS);
  Temporary *arguments = Operation_ARGUMENTS(this);
  Temporary *results = Operation_RESULTS(this);
  if (file == NULL) file = stderr;
  fprintf(file, "\t%3d:%s", Operation_IDENTITY(this), Operator_mnemonic(operator));
  if (Operation_INSTANCE(this) > 0) fprintf(file, "#%d", Operation_INSTANCE(this));
  if (Operation_INDEX(this) >= 0) fprintf(file, "_%d", Operation_INDEX(this));
  if (Operation_ORDERING(this) > 0) fprintf(file, "[%d]", Operation_ORDERING(this));
  if (   Operation_INDEX(this) >= 0
      || Operation_INSTANCE(this) > 0
      || Operation_ORDERING(this) > 0) fprintf(file, "\t");
  else fprintf(file, "  \t");
  Operation_RESULTS_FOREACH_Temporary(this, temporary) {
    Register defRegister = Operation_getDefRegister(this, Operation_RESULTS_INDEX);
    if (Operation_RESULTS_INDEX < Operator_writeCount(operator)) {
      int rank = OperatorParameter_rank(writeParameters + Operation_RESULTS_INDEX);
      if (rank > 0) decoded[rank - 1] = temporary;
    }
    fprintf(file, "%s", sep);
    Temporary_pretty(temporary, file);
    if (defRegister != Register__UNDEF  &&
        (!Temporary_hasRegFile(temporary) ||
         defRegister != Temporary_REGISTER(temporary))) {
      fprintf(file, "^%s", Register_names(defRegister)[0]);
    }
    sep = ", ";
  } Operation_RESULTS_ENDEACH_Temporary;
  sep = (*sep == ' ')? " ": " = ";
  Operation_ARGUMENTS_FOREACH_Temporary(this, temporary) {
    Register useRegister = Operation_getUseRegister(this, Operation_ARGUMENTS_INDEX);
    if (Operation_ARGUMENTS_INDEX < Operator_readCount(operator)) {
      int rank = OperatorParameter_rank(readParameters + Operation_ARGUMENTS_INDEX);
      if (rank > 0) decoded[rank - 1] = temporary;
    }
    fprintf(file, "%s", sep);
    Temporary_pretty(temporary, file);
    if (useRegister != Register__UNDEF &&
        (!Temporary_hasRegFile(temporary) ||
         useRegister != Temporary_REGISTER(temporary))) {
      fprintf(file, "^%s", Register_names(useRegister)[0]);
    }
    sep = ", ";
  } Operation_ARGUMENTS_ENDEACH_Temporary;
  if (Operation_hasClobber(this)) {
    RegisterSet clobberSet = Operation_CLOBBERSET(this);
    fprintf(file, "\t{");
    RegisterSet_pretty(clobberSet, file, processor);
    fprintf(file, "}");
  }
  if (Operation_STARTDATE(this) >= 0) {
    fprintf(file, "\tdate=%d", Operation_STARTDATE(this));
  }
  {
    OperationFlags flags = Operation_FLAGS(this) & ~(OperationFlag_Clobber);
    if (flags != 0) {
      fputs("\t", file);
      ETable_prettyFlags(OperationFlag_Table, flags, ",", file);
    }
  }
#if 0
{
  Opcode opcode = Operator_opcode(operator, processor);
  if (Opcode_syntax(opcode) != NULL) {
    const char *syntax = Opcode_syntax(opcode);
    fputs("\t\"", file);
    while (*syntax) {
      if (*syntax == '%') {
        if (isdigit(*++syntax)) {
          int rank = (*syntax - '0');
          while (isdigit(*++syntax)) rank = rank*10 + (*syntax - '0');
          if (rank == 0) fputs(Opcode_mnemonic(opcode), file);
          else if (rank <= Operands__MAXOPERANDS) {
            Temporary_pretty(decoded[rank - 1], file);
          } else fprintf(file, "<error:%%%d>", rank);
        } else fputc(*syntax++, file);
      }
      else fputc(*syntax++, file);
    }
    fputs("\"", file);
  }
}
#endif
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Assembly printing of this Operation.
 */
bool
Operation_asmPrint(const_Operation this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
static int asm_number_label_id = 1;

bool
Operation_asmPrint(const_Operation this, FILE *file)
{
  bool hexMemory = false;
  const char *sep = " ";
  Operator operator = Operation_OPERATOR(this);
  BasicBlock basicBlock = Operation_BASICBLOCK(this);
  Processor processor = basicBlock != NULL?
                        BasicBlock_PROCESSOR(basicBlock):
                        Processor__UNDEF;

  const char* syntax = Operator_syntax(operator);

  const_OperatorParameterArray parameterArray = Operator_parameterArray(operator);
  const OperatorParameter_ *writeParameters = OperatorParameterArray_writeParameters(parameterArray);
  const OperatorParameter_ *readParameters = OperatorParameterArray_readParameters(parameterArray);
  Temporary *decoded = alloca(sizeof(Temporary)*Operands__MAXOPERANDS);
  Temporary *arguments = Operation_ARGUMENTS(this);
  Temporary *results = Operation_RESULTS(this);
  if (file == NULL) file = stderr;

  if (Operator_isGNUASM(operator)) {
    int operandNumber = 0;

    Operation_RESULTS_FOREACH_Temporary(this, temporary) {
      if (Temporary_type(temporary) != TemporaryType_Virtual)
        decoded[operandNumber++] = temporary;
    } Operation_RESULTS_ENDEACH_Temporary;

    Operation_ARGUMENTS_FOREACH_Temporary(this, temporary) {
      if ((Temporary_type(temporary) != TemporaryType_Virtual) &&
          (Operation_ARGUMENTS_INDEX > 0))
        decoded[operandNumber++] = temporary;
    } Operation_ARGUMENTS_ENDEACH_Temporary;
  } else {
    Operation_RESULTS_FOREACH_Temporary(this, temporary) {
      if (Operation_RESULTS_INDEX < Operator_writeCount(operator)) {
        int rank = OperatorParameter_rank(writeParameters + Operation_RESULTS_INDEX);
        if (rank > 0) decoded[rank - 1] = temporary;
      }
    } Operation_RESULTS_ENDEACH_Temporary;

    Operation_ARGUMENTS_FOREACH_Temporary(this, temporary) {
      if (Operation_ARGUMENTS_INDEX < Operator_readCount(operator)) {
        int rank = OperatorParameter_rank(readParameters + Operation_ARGUMENTS_INDEX);
        if (rank > 0) decoded[rank - 1] = temporary;
      }
    } Operation_ARGUMENTS_ENDEACH_Temporary;
  }

  // FIXME
  if (Operator_isSimulated(operator) && !Operator_isGNUASM(operator)) {
    fputs("\tget ", file);
    Temporary_asmPrint(decoded[0], file, false, 0);
    fputs(" = ", file);
    Temporary_asmPrint(decoded[1], file, false, 0);
    fputs("\n\tmake ", file);
    Temporary_asmPrint(decoded[2], file, false, 0);
    fputs(" = ", file);
    Temporary_asmPrint(decoded[3], file, false, 0);
    fputs("\n", file);
    return true;
  }
  // FIXME end

  Except_THROWIF(syntax == NULL,
      "Cannot find syntax for operator %d (shortname %s)!\n\
      Please check the target architecture (current is `%s')!",
      operator, Operator_shortName(operator), Processor_Id(processor));
  if (syntax != NULL) {
    fputs("\t", file);
    while (*syntax) {
      if (*syntax == '%') {
        if (isdigit(*++syntax)) {
          int rank = (*syntax - '0');
          while (isdigit(*++syntax)) rank = rank*10 + (*syntax - '0');
          if (rank == 0) {
            if (Operator_isGNUASM(operator)) {
              Symbol symbol = Literal_SYMBOL(Temporary_LITERAL(arguments[0]));
              int dummyCount = 0;
              int resCount = Operation_resCount(this);
              char * pos, * asm_str = (char *) Symbol_name(symbol);
              static char triggers[] = "\a\b\f\n\r\t\v\\\"\'";
              static char escapes[] = "abfnrtv\\\"\'";

              fputs("\t# Start of GNU ASM\n", file);
              while (*asm_str) {
                if (*asm_str == '\\') {
                  asm_str++;
                  pos = strchr(escapes, *(asm_str++));
                  fputc(triggers[pos - escapes], file);
                } else if (*asm_str == '%') {
                  if (!strncmp(asm_str - 5, "label", 5) && (*++asm_str == '=')) {
                    // Case of a compiler generated number-label
                    fprintf(file, "%d", asm_number_label_id);
                    
                    if (*++asm_str == ':')
                      asm_number_label_id++;
                  } else {
                    // Case of user-specified operand
                    int asm_rank = 0;
                    int op_modifier = 0;

                    if (!isdigit(*++asm_str)) { // Get eventual modifier
                      op_modifier = (int) *asm_str;
                      asm_str++;
                    }

                    asm_rank = (*asm_str - '0');
                    while (isdigit(*++asm_str))
                      asm_rank = asm_rank * 10 + (*asm_str - '0');

                    Temporary_asmPrint(decoded[asm_rank], file, false, op_modifier);
                  }
                } else
                  fputc(*(asm_str++), file);
              }
              fputs("\n\t# End of GNU ASM\n", file);
            } else
              fputs(Operator_mnemonic(operator), file);
          } else if (rank <= Operands__MAXOPERANDS) {
            if (hexMemory) {
              Temporary_asmPrint(decoded[rank - 1], file, true, 0);
              hexMemory = false;
            } else if (*syntax == ':') {
              Temporary_asmPrint(decoded[rank - 1], file, true, 0);
              hexMemory = true;
            } else if ((*syntax == '[') || !Operator_isSeqCopy(operator)) {
              Temporary_asmPrint(decoded[rank - 1], file, false, 0);
            } else {
              Temporary_asmPrint(decoded[rank - 1], file, false, 0);
            }
          } else fprintf(file, "<error:%%%d>", rank);
        } else fputc(*syntax++, file);
      }
      else fputc(*syntax++, file);
    }
    fputs("\n", file);
  } else {
    printf("%s: NULL SYNTAX !!!\n", __FILE__);
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Pretty-print this Operation in Minir format.
 */
bool
Operation_prettyMinir(const_Operation this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Operation_prettyMinir(const_Operation this, FILE *file)
{
  const char *sep = "";
  Operator operator = Operation_OPERATOR(this);
  BasicBlock basicBlock = Operation_BASICBLOCK(this);
  Processor processor = basicBlock != NULL?
                        BasicBlock_PROCESSOR(basicBlock):
                        Processor__UNDEF;
  const_OperatorParameterArray parameterArray = Operator_parameterArray(operator);
  const OperatorParameter_ *writeParameters = OperatorParameterArray_writeParameters(parameterArray);
  const OperatorParameter_ *readParameters = OperatorParameterArray_readParameters(parameterArray);
  Temporary *decoded = alloca(sizeof(Temporary)*Operands__MAXOPERANDS);
  Temporary *arguments = Operation_ARGUMENTS(this);
  Temporary *results = Operation_RESULTS(this);
  if (file == NULL) file = stderr;
  fprintf(file, "          - { O_%d: %s",
      Operation_identity(this),
      Operator_shortName(operator));
  /* TODO: Instance(no!), index?, ordering? */
  //
  /* Is there any modifier? */
  Operation_ARGUMENTS_FOREACH_Temporary(this, temporary) {
    if (Temporary_isModifier(temporary)) {
      ModifierMember member = Temporary_MEMBER(temporary);
      fprintf(file, ".%s", ModifierMember_lcname(member));
    }
  } Operation_ARGUMENTS_ENDEACH_Temporary;
  //
  fprintf(file, ", defs: [");
  sep = "";
  Operation_RESULTS_FOREACH_Temporary(this, temporary) {
    Register defRegister = Operation_getDefRegister(this, Operation_RESULTS_INDEX);
    if (Operation_RESULTS_INDEX < Operator_writeCount(operator)) {
      int rank = OperatorParameter_rank(writeParameters + Operation_RESULTS_INDEX);
      if (rank > 0) decoded[rank - 1] = temporary;
    }
    fprintf(file, "%s", sep);
    Temporary_prettyMinir(temporary, file);
    if (defRegister != Register__UNDEF  &&
        (!Temporary_hasRegFile(temporary) ||
         defRegister != Temporary_REGISTER(temporary))) {
      fprintf(file, "^%s", Register_names(defRegister)[0]);
    }
    sep = ", ";
  } Operation_RESULTS_ENDEACH_Temporary;
  //
  fprintf(file, "], uses: [");
  sep = "";
  Operation_ARGUMENTS_FOREACH_Temporary(this, temporary) {
    if (!Temporary_isModifier(temporary)) {
      Register useRegister = Operation_getUseRegister(this, Operation_ARGUMENTS_INDEX);
      if (Operation_ARGUMENTS_INDEX < Operator_readCount(operator)) {
        int rank = OperatorParameter_rank(readParameters + Operation_ARGUMENTS_INDEX);
        if (rank > 0) decoded[rank - 1] = temporary;
      }
      fprintf(file, "%s", sep);
      Temporary_prettyMinir(temporary, file);
      if (useRegister != Register__UNDEF &&
          (!Temporary_hasRegFile(temporary) ||
           useRegister != Temporary_REGISTER(temporary))) {
        fprintf(file, "^%s", Register_names(useRegister)[0]);
      }
      sep = ", ";
    }
  } Operation_ARGUMENTS_ENDEACH_Temporary;
  //
  fprintf(file, "]");
  //
  if (Operation_hasClobber(this)) {
    RegisterSet clobberSet = Operation_CLOBBERSET(this);
    fprintf(file, ", implicit_defs: [");
    RegisterSet_prettyMinir(clobberSet, file);
    fprintf(file, "]");
  }
  //
  if (Operation_isBranch(this)) {
    Label label = Operation_getTargetLabel(this);
    /* Label == NULL if target is a symbol, hence branch is to an external 
     * function (e.g., tail call) */
    if (label != NULL) {
      fprintf(file, ", target: Block_%d", BasicBlock_identity(Label_block(label)));
      if(Operation_isConditional(this)) {
        fprintf(file, ", fallthru: .next");
      }
    }
  }
  //
  fprintf(file, "}\n");
  //
  /* TODO: startdate, operationflags, syntax */
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Augment an ENTRY Operation with a new argument.
 * Note that no check is done whether the argument count overflows the
 * allocated number of arguments. We cannot use the MAXCOPIES entry as a ENTRY
 * operation can have dedicated constraints which is stored as an union
 * with MAXCOPIES.
 */
int32_t
OperationENTRY_appendResult(Operation this, Temporary result);
#endif//$XCC_h

int32_t
OperationENTRY_appendResult(Operation this, Temporary result)
{
  int32_t resCount = Operation_RESCOUNT(this);
  int32_t argCount = Operation_ARGCOUNT(this);
  Temporary *operands = Operation_OPERANDS(this);
  Except_CHECK(Operation_OPERATOR(this) == Operator_ENTRY);
  Except_CHECK(argCount == 0);
  operands[resCount] =  result;
  *Operation__RESCOUNT(this) = resCount + 1;
  return resCount + 1;
}

#ifdef $XCC_h
/**
 * Augment a parallel copy Operation with a new pair to copy.
 */
int32_t
Operation_addtoParCopy(Operation this, Temporary destination, Temporary origin);
#endif//$XCC_h

int32_t
Operation_addtoParCopy(Operation this, Temporary destination, Temporary origin)
{
  int32_t maxCopies = Operation_MAXCOPIES(this);
  int32_t resCount = Operation_RESCOUNT(this);
  int32_t argCount = Operation_ARGCOUNT(this);
  int32_t index = resCount + argCount + 1;
  Temporary *operands = Operation_OPERANDS(this);
  Register *registers = Operation_REGISTERS(this);
  Except_CHECK(Operator_isParCopy(Operation_OPERATOR(this)));
  if(!(resCount < maxCopies && argCount < maxCopies)) {
    Except_CHECK(registers == NULL);
    maxCopies *= 2;
    Operation_resizeOperandArray_(this, maxCopies*2);
    *Operation__RESCOUNT(this) = resCount;
    *Operation__ARGCOUNT(this) = argCount;
    operands = Operation_OPERANDS(this);
    *Operation__MAXCOPIES(this) = maxCopies;
    Except_CHECK(operands[index] == NULL);
  }
  *Operation__RESCOUNT(this) = resCount + 1;
  *Operation__ARGCOUNT(this) = argCount + 1;
  if(Temporary_isVariable(origin)) {
    Variable var = Temporary_VARIABLE(origin);
    Except_CHECK(operands[index] == NULL);
    operands[index] = NULL;
    Variable_setOperationArgument(var, this, (--index - resCount));
  } else {
    operands[index--] = origin;
  }
  if (registers != NULL) {
    for (; index > resCount; index--) {
      operands[index] = operands[index - 1];
      registers[index] = registers[index - 1];
    }
    registers[resCount] = Register__UNDEF;
  } else {
    for (; index > resCount; index--) {
      operands[index] = operands[index - 1];
    }
  }
  if(Temporary_isVariable(destination)) {
    Variable var = Temporary_VARIABLE(destination);
    operands[index] = NULL;
    Variable_setOperationResult(var, this, index);
  } else {
    operands[index] = destination;
  }
  return index;
}

#ifdef $XCC_h
/**
 * Augment a PCOPY Operation with a new pair to copy in front of the operation.
 * WARNING! This function desynchronizes the OperationArgument and OperationResult of SSA.
 */
int32_t
OperationPCOPY_putCopy(Operation this, Temporary destination, Temporary origin);
#endif//$XCC_h

int32_t
OperationPCOPY_putCopy(Operation this, Temporary destination, Temporary origin)
{
  int32_t maxCopies = Operation_MAXCOPIES(this);
  int32_t resCount = Operation_RESCOUNT(this);
  int32_t argCount = Operation_ARGCOUNT(this);
  int32_t index = resCount + argCount + 1;
  Temporary *operands = Operation_OPERANDS(this);
  Except_CHECK(Operator_isParCopy(Operation_OPERATOR(this)));
  Except_CHECK(resCount < maxCopies && argCount < maxCopies);
  for (; index > resCount + 1; index--) {
    operands[index] = operands[index - 2];
  }
  operands[index--] = origin;
  for (; index > 0; index--) {
    operands[index] = operands[index - 1];
  }
  operands[index] = destination;
  *Operation__RESCOUNT(this) = resCount + 1;
  *Operation__ARGCOUNT(this) = argCount + 1;
  return index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Stack of Operation(s).
 */
struct OperationStack_ {
  //@args	Memory memory, int maxCount
  PtrSeq_ __;		//!< Underlying PtrSeq.
};
#endif//$XCC_h

#ifdef $XCC_h
static inline PtrSeq
OperationStack__(OperationStack this) {
  return OperationStack____(this);
}
static inline const_PtrSeq
OperationStack__const(const_OperationStack this) {
  return OperationStack____(this);
}
#endif//$XCC_h

OperationStack
OperationStack_Ctor(OperationStack this,
                    Memory memory, int maxCount)
{
  PtrSeq_Ctor(OperationStack____(this), memory, maxCount);
  return this;
}

void
OperationStack_Dtor(OperationStack this)
{
  PtrSeq_Dtor(OperationStack____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(OperationStack_)\t%zu\n", sizeof(OperationStack_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Iterate over this OperationStack Operation(s).
 */
#define OperationStack_FOREACH_Operation(this, operation) { \
  const_PtrSeq OperationStack____ = OperationStack__const(this); \
  PtrSeq_FOREACH(OperationStack____, Operation, operation) {
#define OperationStack_ENDEACH_Operation \
  } PtrSeq_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this OperationStack Operation(s).
 */
#define OperationStack_FORBACK_Operation(this, operation) { \
  const_PtrSeq OperationStack____ = OperationStack__const(this); \
  PtrSeq_FORBACK(OperationStack____, Operation, operation) {
#define OperationStack_ENDBACK_Operation \
  } PtrSeq_ENDBACK \
}
#endif//$XCC_h

#ifdef $XCC_h
#define OperationStack_resize(this, maxCount) \
  PtrSeq_resize(OperationStack__(this), maxCount)
#define OperationStack_empty(this) \
  PtrSeq_empty(OperationStack__(this))
#define OperationStack_memory(this) \
  PtrSeq_memory(OperationStack__const(this))
#define OperationStack_base_(this) (Operation *) \
  PtrSeq_base_(OperationStack__(this))
#define OperationStack_base(this) (const Operation *) \
  PtrSeq_base(OperationStack__const(this))
#define OperationStack_past(this) (const Operation *) \
  PtrSeq_past(OperationStack__const(this))
#define OperationStack_isEmpty(this) \
  PtrSeq_isEmpty(OperationStack__const(this))
#define OperationStack_isSingle(this) \
  PtrSeq_isSingle(OperationStack__const(this))
#define OperationStack_isFull(this) \
  PtrSeq_isFull(OperationStack__const(this))
#define OperationStack_count(this) \
  PtrSeq_count(OperationStack__const(this))
#define OperationStack_access(this, index) (Operation) \
  PtrSeq_access(OperationStack__const(this), index)
#define OperationStack_bottom(this) (Operation) \
  PtrSeq_bottom(OperationStack__const(this))
#define OperationStack_top(this) (Operation) \
  PtrSeq_top(OperationStack__const(this))
#define OperationStack_pop(this) \
  PtrSeq_pop(OperationStack__(this))
#define OperationStack_push(this, operation) \
  PtrSeq_push(OperationStack__(this), operation)
#define OperationStack_push2(this, operation) \
  PtrSeq_push2(OperationStack__(this), operation)
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pair of Operation and argument index.
 */
union OperationArgument_ {
  uint64_t UINT64;
  struct {
    Operation OPERATION;
    int32_t ARGINDEX;
  } PAIR;
};
typedef union OperationArgument_ OperationArgument_, *OperationArgument;
typedef const union OperationArgument_ *restrict const_OperationArgument;
#define OperationArgument_UINT64(this) ((this)->UINT64)
#define OperationArgument__UINT64(this) (&(this)->UINT64)
#define OperationArgument_OPERATION(this) ((this)->PAIR.OPERATION)
#define OperationArgument__OPERATION(this) (&(this)->PAIR.OPERATION)
#define OperationArgument_ARGINDEX(this) ((this)->PAIR.ARGINDEX)
#define OperationArgument__ARGINDEX(this) (&(this)->PAIR.ARGINDEX)
static inline OperationArgument
OperationArgument_Ctor(OperationArgument this, Operation operation, int32_t argIndex)
{
  *OperationArgument__OPERATION(this) = operation;
  *OperationArgument__ARGINDEX(this) = argIndex;
  return this;
}
#define OperationArgument_Clear(this) ((this)->UINT64 = 0L)
#define OperationArgument_Dtor(this) { }
#define OperationArgument_equal(this, that) \
  (OperationArgument_UINT64(this) == OperationArgument_UINT64(that))
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pair of Operation and result index.
 */
union OperationResult_ {
  uint64_t UINT64;
  struct {
    Operation OPERATION;
    int32_t RESINDEX;
  } PAIR;
};
typedef union OperationResult_ OperationResult_, *OperationResult;
typedef const union OperationResult_ *restrict const_OperationResult;
#define OperationResult_UINT64(this) ((this)->UINT64)
#define OperationResult__UINT64(this) (&(this)->UINT64)
#define OperationResult_OPERATION(this) ((this)->PAIR.OPERATION)
#define OperationResult__OPERATION(this) (&(this)->PAIR.OPERATION)
#define OperationResult_RESINDEX(this) ((this)->PAIR.RESINDEX)
#define OperationResult__RESINDEX(this) (&(this)->PAIR.RESINDEX)
static inline OperationResult
OperationResult_Ctor(OperationResult this, Operation operation, int32_t resIndex)
{
  *OperationResult__OPERATION(this) = operation;
  *OperationResult__RESINDEX(this) = resIndex;
  return this;
}
#define OperationResult_Clear(this) ((this)->UINT64 = 0L)
#define OperationResult_Dtor(this) { }
#define OperationResult_equal(this, that) \
  (OperationResult_UINT64(this) == OperationResult_UINT64(that))
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize Operation.
 */
void
Operation_INIT(void);
#endif//$XCC__h

IndexedFactory_ Operation_Factory[1];

void
Operation_INIT(void)
{
  IndexedFactory_Ctor(Operation_Factory);
#ifndef _NTRACE
  ETable_Ctor(OperationFlag_Table, Memory_Root, OperationFlagEntries);
#endif//_NTRACE
}

#ifdef $XCC__h
/**
 * Finalize Operation.
 */
#ifndef _NTRACE
void
Operation_FINI(void);
#else//_NTRACE
#define Operation_FINI()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
Operation_FINI(void)
{
  ETable_Dtor(OperationFlag_Table);
}
#endif//_NTRACE

#if XCC__C
static void
Operation_TEST(void)
{
#include "LIR/Operation_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Operation_TEST);
  return 0;
}
#endif

