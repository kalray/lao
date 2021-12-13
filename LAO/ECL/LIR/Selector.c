#ifdef $XCC_h
/*
 * !!!!	Selector.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu).
 * Christophe Guillon (Christophe.Guillon@st.com).
 * Duco van Amstel (duco.van-amstel@kalray.eu).
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
 * @brief Factory for Operation(s).
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/Selector.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Operation selector also used as Operation container of BasicBlock(s).
 *
 * @c isBlock:	True iff this Selector belongs to a BasicBlock.
 */
struct Selector_ {
  //@args	Memory memory, struct Procedure_ *procedure, bool isBlock
  IDList_ OPERATIONS[1];	//!< List of Operation(s) in this Selector.
  //@access MEMORY	IDList_memory(Selector_OPERATIONS(this))
  //@access COUNT	(IDList_count(Selector_OPERATIONS(this)))
  struct Procedure_ *PROCEDURE;	//!< The Procedure this Selector belongs to.
  Processor PROCESSOR;	        //!< Processor for which to select Operation(s).
  Convention CONVENTION;        //!< Convention for which to select Operation(s).
  int8_t ISBLOCK;		//!< Equals -1 if this Selector is base of a BasicBlock.
  //@access BLOCKMASK	(intptr_t)Selector_ISBLOCK(this)
  //@access UGLYHACK	(uintptr_t)((Indexed)(this) - 1)
  //@access BASICBLOCK	(BasicBlock)(Selector_UGLYHACK(this) & Selector_BLOCKMASK(this))
  int32_t PHICOUNT;		//!< Number of PHI-operations in this Selector.
};
#endif//$XCC_h

Selector
Selector_Ctor(Selector this,
              Memory memory, struct Procedure_ *procedure, bool isBlock)
{
  IDList_Ctor(Selector_OPERATIONS(this), memory);
  *Selector__PROCEDURE(this) = procedure;
  *Selector__PROCESSOR(this) = Procedure_PROCESSOR(procedure);
  *Selector__CONVENTION(this) = Procedure_CONVENTION(procedure);
  *Selector__ISBLOCK(this) = -(isBlock != 0);
  *Selector__PHICOUNT(this) = 0;
  Except_CHECK(memory == Procedure_memory(procedure));
  return this;
}

void
Selector_Dtor(Selector this)
{
  IDList_Dtor(Selector_OPERATIONS(this));
  *Selector__PROCEDURE(this) = NULL;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Selector_)\t%zu\n", sizeof(Selector_));
}
#endif//$XCC__c

#ifdef $XCC__c
{
  // Check that UGLYHACK works.
  BasicBlock_ basicBlock[1];
  Selector selector = BasicBlock_SELECTOR(basicBlock);
  *Selector__ISBLOCK(selector) = -1;
  Except_ALWAYS(Selector_BASICBLOCK(selector) == basicBlock);
  *Selector__ISBLOCK(selector) = 0;
  Except_ALWAYS(Selector_BASICBLOCK(selector) == NULL);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Selector Memory.
 */
static inline Memory
Selector_memory(const_Selector this)
{
  return Selector_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Selector Procedure.
 */
static inline struct Procedure_ *
Selector_procedure(const_Selector this)
{
  return Selector_PROCEDURE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Selector Procedure.
 */
static inline void
Selector_setProcedure(Selector this, struct Procedure_ *procedure)
{
  *Selector__PROCEDURE(this) = procedure;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Selector Processor.
 */
static inline Processor
Selector_processor(const_Selector this)
{
  return Selector_PROCESSOR(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test if this Selector is empty.
 */
static inline bool
Selector_isEmpty(const_Selector this)
{
  return IDList_isEmpty(Selector_OPERATIONS(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of Operation(s) in this Selector.
 */
static inline int32_t
Selector_countOperations(const_Selector this)
{
  return IDList_count(Selector_OPERATIONS(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * First operation of this Selector or NULL.
 */
static inline Operation
Selector_firstOperation(const_Selector this)
{
  return (Operation)IDList_firstItem(Selector_OPERATIONS(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Last operation of this Selector or NULL.
 */
static inline Operation
Selector_lastOperation(const_Selector this)
{
  return (Operation)IDList_lastItem(Selector_OPERATIONS(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by Selector_FOREACH_Operation
 */
static inline IDList
Selector_operations(Selector this)
{
  return Selector_OPERATIONS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate from first to last operation in this Selector.
 */
#define Selector_FOREACH_Operation(this, operation) \
  IDList_FOREACH(Selector_operations(this), Operation_, operation)
#define Selector_ENDEACH_Operation \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate from last to first operation in this Selector.
 */
#define Selector_FORBACK_Operation(this, operation) \
  IDList_FORBACK(Selector_operations(this), Operation_, operation)
#define Selector_ENDBACK_Operation \
  IDList_ENDBACK;
#endif//$XCC_h

#ifdef $XCC__h
/**
 * Increment the PHICOUNT of this Selector.
 */
static inline int
Selector_incrementPhiCount(Selector this)
{
  int phiCount = Selector_PHICOUNT(this) + 1;
  *Selector__PHICOUNT(this) = phiCount;
  Except_CHECK(phiCount == Selector_PHICOUNT(this));
  return phiCount;
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Decrement the PHICOUNT of this Selector.
 */
static inline int
Selector_decrementPhiCount(Selector this)
{
  int phiCount = Selector_PHICOUNT(this) - 1;
  *Selector__PHICOUNT(this) = phiCount;
  Except_CHECK(phiCount == Selector_PHICOUNT(this));
  return phiCount;
}
#endif//$XCC__h

#ifdef $XCC_h
/**
 * Pretty-print this selector
 */
void
Selector_pretty(Selector this, FILE *file);
#endif//$XCC_h

void
Selector_pretty(Selector this, FILE *file)
{
  Selector_FOREACH_Operation(this, operation) {
    fprintf(file, "\t"), Operation_pretty(operation, file);
  } Selector_ENDEACH_Operation;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Attach a detached Operation first on this Selector.
 */
void
Selector_attachFirst(Selector this, Operation operation);
#endif//$XCC_h

void
Selector_attachFirst(Selector this, Operation operation)
{
  BasicBlock basicBlock = Selector_BASICBLOCK(this);
  Except_REQUIRE(Operation_BASICBLOCK(operation) == NULL);
  IDList_put_(Selector_OPERATIONS(this), operation);
  *Operation__BASICBLOCK(operation) = basicBlock;
}

#ifdef $XCC_h
/**
 * Attach a detached Operation last on this Selector.
 */
void
Selector_attachLast(Selector this, Operation operation);
#endif//$XCC_h

void
Selector_attachLast(Selector this, Operation operation)
{
  BasicBlock basicBlock = Selector_BASICBLOCK(this);
  Except_REQUIRE(Operation_BASICBLOCK(operation) == NULL);
  IDList_push_(Selector_OPERATIONS(this), operation);
  *Operation__BASICBLOCK(operation) = basicBlock;
}

#ifdef $XCC_h
/**
 * Detach the first Operation of this Selector.
 */
Operation
Selector_detachFirst(Selector this);
#endif//$XCC_h

Operation
Selector_detachFirst(Selector this)
{
  Operation operation = IDList_firstItem(Selector_OPERATIONS(this));
  Except_REQUIRE(Operation_BASICBLOCK(operation) == Selector_BASICBLOCK(this));
  IDList_drop_(Selector_OPERATIONS(this));
  *Operation__BASICBLOCK(operation) = NULL;
  return operation;
}

#ifdef $XCC_h
/**
 * Detach the last Operation of this Selector.
 */
Operation
Selector_detachLast(Selector this);
#endif//$XCC_h

Operation
Selector_detachLast(Selector this)
{
  Operation operation = IDList_lastItem(Selector_OPERATIONS(this));
  Except_REQUIRE(Operation_BASICBLOCK(operation) == Selector_BASICBLOCK(this));
  IDList_pop_(Selector_OPERATIONS(this));
  *Operation__BASICBLOCK(operation) = NULL;
  return operation;
}

#ifdef $XCC_h
/**
 * Detach an Operation from this Selector.
 *
 * The detached Operation is left outside any Selector.
 */
void
Selector_detachOperation(Selector this, Operation operation);
#endif//$XCC_h

void
Selector_detachOperation(Selector this, Operation operation)
{
  Except_REQUIRE(Operation_BASICBLOCK(operation) == Selector_BASICBLOCK(this));
  Except_CHECK(IDList_contains(Selector_OPERATIONS(this), operation));
  IDList_remove_(Selector_OPERATIONS(this), operation);
  *Operation__BASICBLOCK(operation) = NULL;
}

#ifdef $XCC_h
/**
 * Move this Selector Operation in first position.
 */
void
Selector_moveOperationFirst(Selector this, Operation operation);
#endif//$XCC_h

void
Selector_moveOperationFirst(Selector this, Operation operation)
{
  IDList operations = Selector_OPERATIONS(this);
  Except_CHECK(Operation_BASICBLOCK(operation) == Selector_BASICBLOCK(this));
  IDList_remove_(operations, operation);
  IDList_put_(operations, operation);
}

#ifdef $XCC_h
/**
 * Move this Selector Operation in last position.
 */
void
Selector_moveOperationLast(Selector this, Operation operation);
#endif//$XCC_h

void
Selector_moveOperationLast(Selector this, Operation operation)
{
  IDList operations = Selector_OPERATIONS(this);
  Except_CHECK(Operation_BASICBLOCK(operation) == Selector_BASICBLOCK(this));
  IDList_remove_(operations, operation);
  IDList_push_(operations, operation);
}

#ifdef $XCC_h
/**
 * Move this Selector Operation before another.
 */
void
Selector_moveOperationBefore(Selector this, Operation before, Operation operation);
#endif//$XCC_h

void
Selector_moveOperationBefore(Selector this, Operation before, Operation operation)
{
  IDList operations = Selector_OPERATIONS(this);
  Except_CHECK(Operation_BASICBLOCK(before) == Selector_BASICBLOCK(this));
  Except_CHECK(Operation_BASICBLOCK(operation) == Selector_BASICBLOCK(this));
  IDList_remove_(operations, operation);
  IDList_insert_(operations, before, operation);
}

#ifdef $XCC_h
/**
 * Move this Selector Operation after another.
 */
void
Selector_moveOperationAfter(Selector this, Operation after, Operation operation);
#endif//$XCC_h

void
Selector_moveOperationAfter(Selector this, Operation after, Operation operation)
{
  IDList operations = Selector_OPERATIONS(this);
  Except_CHECK(Operation_BASICBLOCK(after) == Selector_BASICBLOCK(this));
  Except_CHECK(Operation_BASICBLOCK(operation) == Selector_BASICBLOCK(this));
  IDList_remove_(operations, operation);
  IDList_append_(operations, after, operation);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make a new Operation last on this Selector.
 *
 * Low-level call, does not initialize the Operation arguments and results.
 * The space for OPERANDS are allocated during this call. However, after calling
 * Operation_resizeArguments or Operation_resizeResults, the OPERANDS may be
 * reallocated in a different memory block.
 */
Operation
Selector_makeOperation_(Selector this, Operator operator,
                        int32_t resCount, int32_t argCount,
                        const_RegisterSet clobberSet);
#endif//$XCC_h

Operation
Selector_makeOperation_(Selector this, Operator operator,
                        int32_t resCount, int32_t argCount,
                        const_RegisterSet clobberSet)
{
  Memory memory = Selector_MEMORY(this);
  BasicBlock basicBlock = Selector_BASICBLOCK(this);
  int32_t clp2Size = HackerU32_clp2(resCount + argCount)*sizeof(Temporary);
  size_t size = Operation_Size(operator, resCount, argCount, clobberSet, basicBlock);
  Operation operation = IDList_push(Selector_OPERATIONS(this), size);
  Operation_Ctor(operation, operator, resCount, argCount, clobberSet, basicBlock);
  Except_CHECK(resCount >= 0 && argCount >= 0);
  if (clp2Size > 0) {
    Temporary *operands = memset(Memory_alloc(memory, clp2Size), 0, clp2Size);
    *Operation__OPERANDS(operation) = operands;
  }
  return operation;
}

#ifdef $XCC_h
/*
  Selector_makeOperation_n_m -- Specializations of Selector_makeOperation.
 */
#endif//$XCC_h

#ifdef $XCC_h
static inline Operation
Selector_makeOperation_0_0(Selector this,
                           Operator operator)
{
  Operation operation = Selector_makeOperation_(this, operator, 0, 0, NULL);
  return operation;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Operation
Selector_makeOperation_0_1(Selector this,
                           Operator operator,
                           Temporary argument_0)
{
  Operation operation = Selector_makeOperation_(this, operator, 0, 1, NULL);
  Temporary *arguments = Operation_arguments(operation);
  arguments[0] = argument_0;
  return operation;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Operation
Selector_makeOperation_0_2(Selector this,
                           Operator operator,
                           Temporary argument_0,
                           Temporary argument_1)
{
  Operation operation = Selector_makeOperation_(this, operator, 0, 2, NULL);
  Temporary *arguments = Operation_arguments(operation);
  arguments[0] = argument_0;
  arguments[1] = argument_1;
  return operation;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Operation
Selector_makeOperation_0_3(Selector this,
                           Operator operator,
                           Temporary argument_0,
                           Temporary argument_1,
                           Temporary argument_2)
{
  Operation operation = Selector_makeOperation_(this, operator, 0, 3, NULL);
  Temporary *arguments = Operation_arguments(operation);
  arguments[0] = argument_0;
  arguments[1] = argument_1;
  arguments[2] = argument_2;
  return operation;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Operation
Selector_makeOperation_1_0(Selector this,
                           Operator operator,
                           Temporary result_0)
{
  Operation operation = Selector_makeOperation_(this, operator, 1, 0, NULL);
  Temporary *results = Operation_results(operation);
  results[0] = result_0;
  return operation;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Operation
Selector_makeOperation_1_1(Selector this,
                           Operator operator,
                           Temporary result_0,
                           Temporary argument_0)
{
  Operation operation = Selector_makeOperation_(this, operator, 1, 1, NULL);
  Temporary *arguments = Operation_arguments(operation);
  Temporary *results = Operation_results(operation);
  results[0] = result_0;
  arguments[0] = argument_0;
  return operation;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Operation
Selector_makeOperation_1_2(Selector this,
                           Operator operator,
                           Temporary result_0,
                           Temporary argument_0,
                           Temporary argument_1)
{
  Operation operation = Selector_makeOperation_(this, operator, 1, 2, NULL);
  Temporary *arguments = Operation_arguments(operation);
  Temporary *results = Operation_results(operation);
  results[0] = result_0;
  arguments[0] = argument_0;
  arguments[1] = argument_1;
  return operation;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Operation
Selector_makeOperation_1_3(Selector this,
                           Operator operator,
                           Temporary result_0,
                           Temporary argument_0,
                           Temporary argument_1,
                           Temporary argument_2)
{
  Operation operation = Selector_makeOperation_(this, operator, 1, 3, NULL);
  Temporary *arguments = Operation_arguments(operation);
  Temporary *results = Operation_results(operation);
  results[0] = result_0;
  arguments[0] = argument_0;
  arguments[1] = argument_1;
  arguments[2] = argument_2;
  return operation;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Operation
Selector_makeOperation_2_2(Selector this,
                           Operator operator,
                           Temporary result_0,
                           Temporary result_1,
                           Temporary argument_0,
                           Temporary argument_1)
{
  Operation operation = Selector_makeOperation_(this, operator, 2, 2, NULL);
  Temporary *arguments = Operation_arguments(operation);
  Temporary *results = Operation_results(operation);
  results[0] = result_0;
  results[1] = result_1;
  arguments[0] = argument_0;
  arguments[1] = argument_1;
  return operation;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Operation
Selector_makeOperation_2_3(Selector this,
                           Operator operator,
                           Temporary result_0,
                           Temporary result_1,
                           Temporary argument_0,
                           Temporary argument_1,
                           Temporary argument_2)
{
  Operation operation = Selector_makeOperation_(this, operator, 2, 3, NULL);
  Temporary *arguments = Operation_arguments(operation);
  Temporary *results = Operation_results(operation);
  results[0] = result_0;
  results[1] = result_1;
  arguments[0] = argument_0;
  arguments[1] = argument_1;
  arguments[2] = argument_2;
  return operation;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make a new Operation from this Selector.
 */
Operation
Selector_makeOperation(Selector this, Operator operator,
                       int32_t resCount, int32_t argCount,
                       Temporary *results, Temporary *arguments,
                       const_RegisterSet clobberSet);
#endif//$XCC_h

Operation
Selector_makeOperation(Selector this, Operator operator,
                       int32_t resCount, int32_t argCount,
                       Temporary *results, Temporary *arguments,
                       const_RegisterSet clobberSet)
{
  Operation operation = Selector_makeOperation_(this, operator, resCount, argCount, clobberSet);
  Temporary *this_arguments = Operation_arguments(operation);
  Temporary *this_results = Operation_results(operation);
  int32_t i;
  if (arguments != NULL) {
    for (i = 0; i < argCount; i++) {
      this_arguments[i] = arguments[i];
    }
  }
  if (results != NULL) {
    for (i = 0; i < resCount; i++) {
      this_results[i] = results[i];
    }
  }
  return operation;
}


#ifdef $XCC_h
/**
 * Make a new Operation from this Selector, possibly by changing the operator.
 *
 * If the operator has not a unique mnemonic, may choose a different operator
 * with the same mnemonic that fits the parameters count and type.
 *
 * @pre @p operator should be the first operator with this mnemonic in the
 * operator enum. All operators with the same mnemonic must appear
 * consecutively in the operator enum. The operators with the same mnemonic
 * must be ordered from the most restrictive to the least restrictive (e.g.,
 * with parameter `signed10' before `signed32'.
 */
Operation
Selector_makeAppropriateOperation(Selector this, Operator operator,
                       int32_t resCount, int32_t argCount,
                       Temporary *results, Temporary *arguments,
                       const_RegisterSet clobberSet);
#endif//$XCC_h

Operation
Selector_makeAppropriateOperation(Selector this, Operator operator,
                       int32_t resCount, int32_t argCount,
                       Temporary *results, Temporary *arguments,
                       const_RegisterSet clobberSet)
{
  Operation operation;
  const_OperatorParameterArray parameterArray;
  int writeCount, readCount;

  /* Check if multiple operators share the same mnemonic */
  // Maybe also skip this part and check all parameters are correct?
  if(!Operator_otherMnemonic(operator)) {
    parameterArray = Operator_parameterArray(operator);
    writeCount = OperatorParameterArray_writeCount(parameterArray);
    if(resCount != writeCount) {
      const_OperatorParameter writeParameters =
        OperatorParameterArray_writeParameters(parameterArray);
      /* Still ok if result is writeList */
      if(writeCount != 1 || resCount == 0 ||
         OperatorParameter_kind(writeParameters) != OperatorParameterKind_WriteList)
      {
        Except_THROW(
            "Wrong number of results for operator `%s': %d expected but %d given!",
            Operator_mnemonic(operator), writeCount, resCount);
      }
    }
    readCount = OperatorParameterArray_readCount(parameterArray);
    if(argCount != readCount) {
      const_OperatorParameter readParameters =
        OperatorParameterArray_readParameters(parameterArray);
      /* Still ok if result is readList */
      if(readCount != 1 || argCount == 0 ||
         OperatorParameter_kind(readParameters) != OperatorParameterKind_ReadList)
      {
        Except_THROW(
            "Wrong number of arguments for operator `%s': %d expected but %d given (counting modifiers if any)!",
            Operator_mnemonic(operator), readCount, argCount);
      }
    }
  } else {
    /* Find appropriate operator */
    const_OperatorParameter writeParameters;
    const_OperatorParameter readParameters;
    Temporary temp;
    int count;
    bool matchingOp;
    Operator newOperator = operator-1;

/*    printf("Looking for `%s' operator with %d writes and %d reads\n",*/
/*        Operator_mnemonic(operator),*/
/*        resCount, argCount);*/

    while(newOperator++) {
      Except_THROWIF(strcmp(Operator_mnemonic(operator),
                            Operator_mnemonic(newOperator)),
          "Could not find an operator with mnemoric `%s' matching parameters!",
          Operator_mnemonic(operator)
          );
      parameterArray = Operator_parameterArray(newOperator);
/*      printf("Checking operator `%s' with enum #%d\n", Operator_mnemonic(newOperator), newOperator);*/
      writeCount = OperatorParameterArray_writeCount(parameterArray);
      readCount = OperatorParameterArray_readCount(parameterArray);
/*      printf("  %d writes and %d reads\n", writeCount, readCount);*/
      if(resCount != writeCount || argCount != readCount) continue;
/*      printf("  correct number of parameters\n");*/

      /* Check all parameters */
      matchingOp = true;
      OperatorParameterArray_FOREACH_OperatorParameter(parameterArray, parameter) {
        count = OperatorParameterArray_INDEX;
        if(count < resCount) {
          temp = results[count];
        } else {
          temp = arguments[count - resCount];
        }
        if(!Temporary_matchParameter(temp, parameter)) { 
          matchingOp = false;
          break;
        }
      }  OperatorParameterArray_ENDEACH_OperatorParameter

      if(matchingOp) break;
    }
    operator = newOperator;
  }

  operation = Selector_makeOperation(this, operator, resCount, argCount,
      results, arguments, clobberSet);

  return operation;
}



#ifdef $XCC_h
/**
 * Replicate an Operation from this Selector.
 *
 * The Operation_INDEX of the new operation is the same.
 */
Operation
Selector_replicateOperation(Selector this, const_Operation operation);
#endif//$XCC_h

Operation
Selector_replicateOperation(Selector this, const_Operation operation)
{
  int32_t argCount = Operation_ARGCOUNT(operation);
  int32_t resCount = Operation_RESCOUNT(operation);
  Operation that = Selector_makeOperation(this, Operation_OPERATOR(operation),
                                          resCount, argCount,
                                          Operation_RESULTS(operation),
                                          Operation_ARGUMENTS(operation),
                                          Operation_clobberSet(operation));
  *Operation__INDEX(that) = Operation_INDEX(operation);
  *Operation__FLAGS(that) |= Operation_FLAGS(operation);
  *Operation__REPLICATED(that) = operation;
  return that;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make a LABEL Operation on this Selector.
 */
Operation
Selector_makeOperationLABEL(Selector this, Label label);
#endif//$XCC_h

Operation
Selector_makeOperationLABEL(Selector this, Label label)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Temporary temporary = Procedure_makeLabelTemporary(procedure, label, Immediate__UNDEF);
  return Selector_makeOperation_0_1(this, Operator_LABEL, temporary);
}

#ifdef $XCC_h
/**
 * Make a GOTO Operation on this Selector.
 */
Operation
Selector_makeOperationGOTO(Selector this, Label label);
#endif//$XCC_h

Operation
Selector_makeOperationGOTO(Selector this, Label label)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Temporary temporary = Procedure_makeLabelTemporary(procedure, label, Immediate__UNDEF);
  return Selector_makeOperation_0_1(this, Operator_GOTO, temporary);
}

#ifdef $XCC_h
/**
 * Make a LOOP Operation on this Selector.
 */
Operation
Selector_makeOperationLOOP(Selector this, Label label);
#endif//$XCC_h

Operation
Selector_makeOperationLOOP(Selector this, Label label)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Temporary temporary = Procedure_makeLabelTemporary(procedure, label, Immediate__UNDEF);
  return Selector_makeOperation_0_1(this, Operator_LOOPTO, temporary);
}

#ifdef $XCC_h
/**
 * Make a FALL Operation on this Selector.
 */
Operation
Selector_makeOperationFALL(Selector this, Label label);
#endif//$XCC_h

Operation
Selector_makeOperationFALL(Selector this, Label label)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Temporary temporary = Procedure_makeLabelTemporary(procedure, label, Immediate__UNDEF);
  return Selector_makeOperation_0_1(this, Operator_FALLTO, temporary);
}

#ifdef $XCC_h
/**
 * Make a SKIP Operation on this Selector.
 */
static inline Operation
Selector_makeOperationSKIP(Selector this)
{
  return Selector_makeOperation_0_0(this, Operator_SKIP);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a KILL Operation on this Selector.
 */
static inline Operation
Selector_makeOperationKILL(Selector this, Temporary result)
{
  return Selector_makeOperation_1_0(this, Operator_KILL, result);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a COPY Operation on this Selector.
 */
static inline Operation
Selector_makeOperationCOPY(Selector this, Temporary result, Temporary argument)
{
  return Selector_makeOperation_1_1(this, Operator_COPY, result, argument);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a SELECT Operation on this Selector.
 */
static inline Operation
Selector_makeOperationSELECT(Selector this,
                             Temporary result, Temporary condition,
                             Temporary if_true, Temporary if_false)
{
  return Selector_makeOperation_1_3(this, Operator_SELECT,
                                    result, condition, if_true, if_false);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a ENTRY Operation on this Selector.
 */
static inline Operation
Selector_makeOperationENTRY(Selector this, int32_t maxCount)
{
  Operation operation =  Selector_makeOperation_(this, Operator_ENTRY, maxCount, 0, NULL);
  *Operation__RESCOUNT(operation) = *Operation__ARGCOUNT(operation) = 0;
  return operation;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a parallel copy Operation on this Selector.
 */
Operation
Selector_makeOperationParCopy(Selector this, Operator operator, int32_t maxCount, bool registers);
#endif//$XCC_h

Operation
Selector_makeOperationParCopy(Selector this, Operator operator, int32_t maxCopies, bool registers)
{
  Memory memory = Selector_MEMORY(this);
  Operation operation = Selector_makeOperation_(this, operator, maxCopies, maxCopies, NULL);
  *Operation__RESCOUNT(operation) = *Operation__ARGCOUNT(operation) = 0;
  *Operation__MAXCOPIES(operation) = maxCopies;
  Except_CHECK(Operator_isParCopy(operator));
  if (registers) {
    size_t allocSize = maxCopies*2*sizeof(Register);
    Register *registers = Memory_alloc(memory, allocSize);
    memset(registers, Register__UNDEF, allocSize);
    Except_CHECK(Operation_REGISTERS(operation) == NULL);
    *Operation__REGISTERS(operation) = registers;
  }
  return operation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC_h
/**
 * Return the Register File corresponding to general-purpose registers.
 */
RegFile
Selector_genPurposeRegFile(void);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select a load from memory at base+offset
 */
void
Selector_selectLOAD(Selector this, NativeType nativeType, Temporary dst, Temporary base,
                    Temporary offset);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select a store from memory at base+offset.
 */
void
Selector_selectSTORE(Selector this, NativeType nativeType, Temporary src, Temporary base,
                     Temporary offset);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select a direct load at address symbol+offset.
 */
void
Selector_selectDirectLOAD(Selector this, NativeType nativeType, Temporary dst,
                          Symbol symbol, int64_t offset);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select a direct store at address symbol+offset
 */
void
Selector_selectDirectSTORE(Selector this, NativeType nativeType, Temporary src,
                           Symbol symbol, int64_t offset);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Selector a function call.
 */
void
Selector_selectCALL(Selector this, Temporary dst);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Generates code to implement a function RETURN.
 */
void
Selector_selectRETURN(Selector this);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Generates code to implement a stack frame ADJUST.
 */
void
Selector_selectADJUST(Selector this, int32_t size);
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Select a copy between same RegFile Temporary(ies).
 */
void
Selector_selectCOPY(Selector this, Temporary dst, Temporary src);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select a copy between same RegFile Temporary(ies) predicated on true.
 */
void
Selector_selectCOPYOnTrue(Selector this, Temporary predicate, Temporary dst, Temporary src);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select a copy between same RegFile Temporary(ies) predicated on false.
 */
void
Selector_selectCOPYOnFalse(Selector this, Temporary predicate, Temporary dst, Temporary src);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select a swap operation between same RegFile Temporary(ies).
 *
 * The sequence must not use additional temporaries.
 */
void
Selector_selectSWAP(Selector this, Temporary dst1, Temporary dst2);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select a transfer between different RegFile Temporary(ies).
 */
void
Selector_selectMOVE(Selector this, Temporary dst, Temporary src);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select a control transfer to label.
 */
void
Selector_selectGOTO(Selector this, Label label);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Generates code to implement a Go True
 */
void
Selector_selectIFTRUE(Selector this, Temporary condition, Label label);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Generates code to implement a Go False
 */
void
Selector_selectIFFALSE(Selector this, Temporary condition, Label label);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Generates code to implement a MAKE (APPLY).
 * Operation that move a value into a register.
 */
void
Selector_selectMAKE(Selector this, Temporary dst, Temporary src);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select an absolute value into dst.
 */
void
Selector_selectMAKEAbsolute(Selector this, NativeType nativeType, Temporary dst,
                            int64_t value);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Select a symbolic value+offset into dst.
 */
void
Selector_selectMAKEAddress(Selector this, NativeType nativeType, Temporary dst,
                           Symbol symbol, int64_t offset);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Generates code to implement an ADD
 */
void
Selector_selectADD(Selector this, Temporary dst, Temporary src1, Temporary src2);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Generates code to implement a SUB.
 * Implement the substraction @p src1 - @p src2.
 */
void
Selector_selectSUB(Selector this, Temporary dst, Temporary src1, Temporary src2);
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Generates code to implement a logical AND
 */
void
Selector_selectLAND(Selector this, Temporary dst, Temporary src1, Temporary src2);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Generates code to implement a logical NOT
 */
void
Selector_selectLNOT(Selector this, Temporary dst, Temporary src);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Generates code to implement a logical OR.
 */
void
Selector_selectLOR(Selector this, Temporary dst, Temporary src1, Temporary src2);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Generates code to implement a select.
 */
void
Selector_selectSELECT(Selector this, Temporary dst, Temporary cond,
                      Temporary src1, Temporary src2);
#endif//$XCC_h


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
// Initialization for tests
{
  Tirex tirex = Tirex_new(Memory_Root, stdin);
  Program program = Tirex_makeProgram(tirex);

  {
    Procedure procedure = Program_findProcedure(program, "replace-generics");
    Procedure_buildCodeRegions(procedure);
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    CodeRegion_pretty(codeRegion, stdout);
    CodeRegion_expandGenericsSimulated(codeRegion);
    CodeRegion_pretty(codeRegion, stdout);
  }

  // Delete constructs
  Program_delete(program);
  Tirex_delete(tirex);
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Initialize Selector module.
 */
#define Selector_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize Selector module.
 */
#define Selector_FINI()
#endif//$XCC__h

#if XCC__C
static void
Selector_TEST(void)
{
#include "LIR/Selector_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Selector_TEST);
  return 0;
}
#endif

