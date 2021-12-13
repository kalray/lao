#ifdef $XCC_h
/*
 * !!!!	Deallocator.xcc
 *
 * Duco van Amstel (duco.van.amstel@kalray.eu)
 *
 * Copyright 2014 Kalray S.A.
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
 * @brief Register Deallocation services.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/Deallocator.h"
#endif//$XCC__h

bool
Deallocator_registerIsStackPointer(Register registre);

RegFile
Deallocator_getGeneralPurposeRegFile(void);

#ifdef $XCC_h
/*
 * Deallocator structure
 */
struct Deallocator_;
#endif//$XCC_h

#ifdef $XCC__h
struct Deallocator_ {
  //@args Memory memory, Procedure procedure
  Memory MEMORY;   //!< This Deallocator Memory.
  Procedure PROCEDURE;   //!< The Procedure on which this Deallocator applies.
  int OUTPUTS;   //!< The number of 4-byte words that are used by the procedure for parameter passing at calls.
  HTable_ REPRESENTATIVES[1];   //!< The representative variables of allocated registers.
  HTable_ SPILLS[1];   //!< The representatives for spilled memory addresses.
  PtrSet_ VISITED[1];   //!< Set to record basic-block visits.
};
#endif//$XCC__h


Deallocator
Deallocator_Ctor(Deallocator this,
                 Memory memory, Procedure procedure)
{
  *Deallocator__MEMORY(this) = memory;
  *Deallocator__PROCEDURE(this) = procedure;
  *Deallocator__OUTPUTS(this) = 0;
  HTable_Ctor(Deallocator_REPRESENTATIVES(this), memory, 32);
  HTable_Ctor(Deallocator_SPILLS(this), memory, 16);
  PtrSet_Ctor(Deallocator_visited(this), memory, 16);
  return this;
}


void
Deallocator_Dtor(Deallocator this)
{
  HTable_empty(Deallocator_SPILLS(this), NULL);
  HTable_Dtor(Deallocator_SPILLS(this));
  HTable_empty(Deallocator_REPRESENTATIVES(this), NULL);
  HTable_Dtor(Deallocator_REPRESENTATIVES(this));
  PtrSet_Dtor(Deallocator_visited(this));
}


#ifdef $XCC_h
/*
 * Return the procedure associated with the deallocator
 */
Procedure
Deallocator_procedure(Deallocator this);
#endif//$XCC_h

Procedure
Deallocator_procedure(Deallocator this)
{
  return Deallocator_PROCEDURE(this);
}


#ifdef $XCC_h
/*
 * Return the number of output parameters associated with the deallocator
 */
int
Deallocator_outputs(Deallocator this);
#endif//$XCC_h

int
Deallocator_outputs(Deallocator this)
{
  return Deallocator_OUTPUTS(this);
}


#ifdef $XCC_h
/*
 * Add an output parameter to the deallocator
 */
void
Deallocator_addOutput(Deallocator this);
#endif//$XCC_h

void
Deallocator_addOutput(Deallocator this)
{
  *Deallocator__OUTPUTS(this) += 1;
}


#ifdef $XCC_h
/*
 * Return the visited PtrSet associated with the deallocator
 */
PtrSet
Deallocator_visited(Deallocator this);
#endif//$XCC_h

PtrSet
Deallocator_visited(Deallocator this)
{
  return Deallocator_VISITED(this);
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Test if the specified Register is the Stack Pointer register
 * Architecture dependent.
 */
bool
Deallocator_registerIsStackPointer(Register reg);
#endif//$XCC_h

#ifdef $XCC__c
{
  int reg = 0;
  for (; reg < Register__Count; reg++) {
    if (Deallocator_registerIsStackPointer(reg)) {
      printf("RegisterIsStackPointer(%s)\n", Register_Id(reg));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/*
 * Get the RegFile that is used for General Purpose registers
 */
RegFile
Deallocator_getGeneralPurposeRegFile(void);
#endif//$XCC_h

#ifdef $XCC__c
{
  printf("GeneralPurposeRegFile(%s)\n", RegFile_Id(Deallocator_getGeneralPurposeRegFile()));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC_h
/*
 * Transform a code-region from register allocated into virtual variable
 * allocated code. This does not modify Dedicated Temporaries and such.
 */
void
Deallocator_deallocate(Program program);
#endif//$XCC_h

void
Deallocator_deallocate(Program program)
{
  Program_FOREACH_Procedure(program, procedure) {
    Deallocator_ deallocator[1];
    Memory  memory = Procedure_memory(procedure);

    Deallocator_Ctor(deallocator, memory, procedure);

    Deallocator_removeSpills(deallocator);
    Deallocator_virtualizeRegisters(deallocator);
    Deallocator_analyzeFrame(deallocator);

    Deallocator_Dtor(deallocator);
  } Program_ENDEACH_Procedure;
}


#ifdef $XCC_h
/*
 * See if there exists a forward path from the current operation to a call
 * or a return without an intermediary kill of the specified register.
 */
bool
Deallocator_isLastDefBeforeCall(Deallocator this, BasicBlock basicBlock, Operation startOperation, Register definedRegister);
#endif//$XCC_h

bool
Deallocator_isLastDefBeforeCall(Deallocator this, BasicBlock basicBlock, Operation startOperation, Register definedRegister)
{
  bool check = startOperation ? false : true;
  bool returnValue = false;
  Operator operator;
  BasicBlock nextBlock;

  /* Reinitialize the visited set if this is a non-recursive call to the function */
  if (startOperation)
    PtrSet_empty(Deallocator_visited(this));

  if (PtrSet_contains(Deallocator_visited(this), (PtrSetMember) basicBlock)) {
    return false;
  } else {
    PtrSet_insert(Deallocator_visited(this), (PtrSetMember) basicBlock);
  }

  BasicBlock_FOREACH_Operation(basicBlock, operation) {
    if (operation == startOperation) {
      check = true;
    } else if (check) {
      operator = Operation_operator(operation);
      if (Operator_isCall(operator))
        return true;

      Operation_RESULTS_FOREACH_Temporary(operation, temporary) {
        if (Temporary_hasRegister(temporary) && (Temporary_register(temporary) == definedRegister))
          return false;
      } Operation_RESULTS_ENDEACH_Temporary;
    }
  } BasicBlock_ENDEACH_Operation;

  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(basicBlock, edge) {
    nextBlock = BasicBlockEdge_destBlock(edge);
    if (Deallocator_isLastDefBeforeCall(this, nextBlock, NULL, definedRegister))
      return true;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;

  if (BasicBlock_leaveCount(basicBlock) > 0)
    return false;
  else
    return true;
}


#ifdef $XCC_h
/*
 * Move all hardware registers to Virtual Temporaries
 */
void
Deallocator_virtualizeRegisters(Deallocator this);
#endif//$XCC_h

void
Deallocator_virtualizeRegisters(Deallocator this)
{
  bool modified = false;
  Procedure procedure = Deallocator_procedure(this);
  Convention convention = Procedure_convention(procedure);
  Temporary * virtualTemporary;

  Procedure_FOREACH_BasicBlock(procedure, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      modified = false;

      Operation_RESULTS_FOREACH_Temporary_Index(operation, temporary, index) {
        if (Temporary_hasRegister(temporary) && (Temporary_regFile(temporary) == Deallocator_getGeneralPurposeRegFile())) {
          Register reg = Temporary_register(temporary);
          
          if (Convention_isReservedRegister(convention, reg)) {
            continue;
          } else
          if (Convention_isResultRegister(convention, reg) ||
              Convention_isArgumentRegister(convention, reg) ||
              Convention_isStructRegister(convention, reg)) {
            if (!Deallocator_isLastDefBeforeCall(this, basicBlock, operation, reg)) {
              virtualTemporary =
                (Temporary *) HTable_lookup(Deallocator_REPRESENTATIVES(this), (HTableKey)(uintptr_t)reg, sizeof(Temporary));
              if (*virtualTemporary == NULL)
                *virtualTemporary = Procedure_makeVirtualTemporary(procedure, Temporary_regFile(temporary));

              Operation_results(operation)[index] = *virtualTemporary;
              Deallocator_replaceAssignedDownwards(this, temporary, *virtualTemporary, basicBlock, operation, false);
              modified = true;
            }
          } else {
            virtualTemporary =
              (Temporary *) HTable_lookup(Deallocator_REPRESENTATIVES(this), (HTableKey)(uintptr_t)reg, sizeof(Temporary));
            if (*virtualTemporary == NULL)
              *virtualTemporary = Procedure_makeVirtualTemporary(procedure, Temporary_regFile(temporary));

            Operation_results(operation)[index] = *virtualTemporary;
            Deallocator_replaceAssignedDownwards(this, temporary, *virtualTemporary, basicBlock, operation, false);
            modified = true;
          }
        }
      } Operation_RESULTS_ENDEACH_Temporary_Index;

      if (modified)
        Operation_fixOperandsWithParameters(operation);
    } BasicBlock_ENDEACH_Operation;
  } Procedure_ENDEACH_BasicBlock;
}


/* Helper function to do the dirty work */
void
Deallocator_replaceAssignedUpwards2(Deallocator this, Temporary old, Temporary new,
                                    BasicBlock basicBlock, Operation startOperation)
{
  bool modify = startOperation ? false : true;
  bool modified = false;
  Selector selector = BasicBlock_selector(basicBlock);
  Convention convention = BasicBlock_CONVENTION(basicBlock);

  if (!PtrSet_isEmpty(Deallocator_visited(this))) {
    if (PtrSet_contains(Deallocator_visited(this), (PtrSetMember) basicBlock))
      return;
  }

  PtrSet_insert(Deallocator_visited(this), (PtrSetMember) basicBlock);
  
  BasicBlock_FORBACK_Operation(basicBlock, operation) {
    if (operation == startOperation)
      modify = true;
    else if (!modify)
      continue;

    if (Operator_isCall(Operation_operator(operation))) {
      if (Convention_isResultRegister(convention, Temporary_register(old)) ||
          Convention_isStructRegister(convention, Temporary_register(old))) {
        Operation copyOperation = Selector_makeOperationCOPY(selector, new, old);
        Selector_moveOperationAfter(selector, operation, copyOperation);
        modify = false;
        break;
      }
    }

    if (modify) {
      Operation_RESULTS_FOREACH_Temporary_Index(operation, temporary, index) {
        if (temporary == old) {
          Operation_results(operation)[index] = new;
          modify = false;
          break;
        }
      } Operation_RESULTS_ENDEACH_Temporary_Index;
    }

    if (modify) {
      Operation_ARGUMENTS_FOREACH_Temporary_Index(operation, temporary, index) {
        if (temporary == old) {
          Operation_arguments(operation)[index] = new;
          modified = true;
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary_Index;
    }

    if (modified) {
      Operation_fixOperandsWithParameters(operation);
      modified = false;
    }
  } BasicBlock_ENDBACK_Operation;

  if (modify) {
    if (BasicBlock_enterCount(basicBlock)) {
      BasicBlock nextBlock;
      BasicBlock_ENTER_FOREACH_BasicBlockEdge(basicBlock, edge) {
        nextBlock = BasicBlockEdge_origBlock(edge);
        Deallocator_replaceAssignedUpwards2(this, old, new, nextBlock, NULL);
      } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
    } else {
      Operation copyOperation = Selector_makeOperationCOPY(selector, new, old);
      Selector_moveOperationFirst(selector, copyOperation);
    }
  }
}


#ifdef $XCC_h
/*
 * Replace a Temporary by another from end to front
 * in a BasicBlock. Finish when encountering the first write.
 * It is possible to specify an operation at which to start
 * within the BasicBlock. If start of block is reached iterate
 * over the predecessors independently.
 */
void
Deallocator_replaceAssignedUpwards(Deallocator this, Temporary old, Temporary new,
                                   BasicBlock basicBlock, Operation operation);
#endif//$XCC_h
void
Deallocator_replaceAssignedUpwards(Deallocator this, Temporary old, Temporary new,
                                   BasicBlock basicBlock, Operation operation)
{
  PtrSet_empty(Deallocator_visited(this));
  Deallocator_replaceAssignedUpwards2(this, old, new, basicBlock, operation);
}


/* Helper function to do the dirty work */
void
Deallocator_replaceAssignedDownwards2(Deallocator this, Temporary old, Temporary new,
                                      BasicBlock basicBlock, Operation startOperation,
                                      bool tailCopy)
{
  bool modify = startOperation ? false : true;
  bool modified = false;
  Selector selector = BasicBlock_selector(basicBlock);
  Convention convention = BasicBlock_CONVENTION(basicBlock);

  if (!PtrSet_isEmpty(Deallocator_visited(this))) {
    if (PtrSet_contains(Deallocator_visited(this), (PtrSetMember) basicBlock))
      return;
  }

  PtrSet_insert(Deallocator_visited(this), (PtrSetMember) basicBlock);

  BasicBlock_FOREACH_Operation(basicBlock, operation) {
    if (operation == startOperation)
      modify = true;
    else if (!modify)
      continue;

    if (Operator_isCall(Operation_operator(operation))) {
      if (Convention_isResultRegister(convention, Temporary_register(old)) ||
          Convention_isStructRegister(convention, Temporary_register(old))) {
        Operation copyOperation = Selector_makeOperationCOPY(selector, old, new);
        Selector_moveOperationBefore(selector, operation, copyOperation);
        modify = false;
        break;
      }
    }

    if (modify) {
      Operation_ARGUMENTS_FOREACH_Temporary_Index(operation, temporary, index) {
        if (temporary == old) {
          Operation_arguments(operation)[index] = new;
          modified = true;
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary_Index;

      Operation_RESULTS_FOREACH_Temporary_Index(operation, temporary, index) {
        if (temporary == old) {
          modify = false;
          break;
        }
      } Operation_RESULTS_ENDEACH_Temporary_Index;
    }

    if (modified) {
      Operation_fixOperandsWithParameters(operation);
      modified = false;
    }
  } BasicBlock_ENDEACH_Operation;

  if (modify) {
    if (BasicBlock_leaveCount(basicBlock)) {
      BasicBlock nextBlock;
      BasicBlock_LEAVE_FOREACH_BasicBlockEdge(basicBlock, edge) {
        nextBlock = BasicBlockEdge_destBlock(edge);
        Deallocator_replaceAssignedDownwards2(this, old, new, nextBlock, NULL, tailCopy);
      } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    } else if (tailCopy
        && (Convention_isResultRegister(convention, Temporary_register(old))
          || Convention_isStructRegister(convention, Temporary_register(old)))) {
      Operation returnOperation = BasicBlock_getControlOperation(basicBlock);
      Operation copyOperation = Selector_makeOperationCOPY(selector, old, new);
      Selector_moveOperationBefore(selector, returnOperation, copyOperation);
    }
  }
}


#ifdef $XCC_h
/*
 * Replace a Temporary by another from front to end
 * in a BasicBlock. Finish when encountering the first write.
 * It is possible to specify an operation at which to start
 * within the BasicBlock. If start of block is reached iterate
 * over the predecessors independently.
 */
void
Deallocator_replaceAssignedDownwards(Deallocator this, Temporary old, Temporary new,
                                     BasicBlock basicBlock, Operation operation,
                                     bool tailCopy);
#endif//$XCC_h

void
Deallocator_replaceAssignedDownwards(Deallocator this, Temporary old, Temporary new,
                                     BasicBlock basicBlock, Operation operation,
                                     bool tailCopy)
{
  PtrSet_empty(Deallocator_visited(this));
  Deallocator_replaceAssignedDownwards2(this, old, new, basicBlock, operation, tailCopy);
}


/* Helper functions to do the dirty work */
int
compareBlocks(IDListItem item1, IDListItem item2)
{
  return *((int *) item1) - *((int *) item2);
}

void
Deallocator_findSpillOffsets2(Deallocator this, IDList blockList)
{
  int offsetValue;
  BasicBlock nextBlock, * listItem;
  BasicBlock currentBlock = *((BasicBlock *) IDList_lastItem(blockList));
  Convention convention = BasicBlock_CONVENTION(currentBlock);
  Temporary baseTemporary, offsetTemporary;
  Temporary * virtualTemporary;
  Register baseRegister;
  Operator operator;

  BasicBlock_FOREACH_Operation(currentBlock, operation) {
    operator = Operation_operator(operation);

    if (Operator_isMemoryWrite(operator)) {
      baseTemporary = Operation_arguments(operation)[Operator_baseIndex(operator)];
      baseRegister = Temporary_register(baseTemporary);

      if (Convention_isStackRegister(convention, baseRegister)) {
        offsetTemporary = Operation_arguments(operation)[Operator_offsetIndex(operator)];
        offsetValue = Literal_value(Temporary_LITERAL(offsetTemporary));

        for (int i = 0; i < Operator_memorySize(operator) / 4; i++)
          HTable_lookup(Deallocator_SPILLS(this), (HTableKey)(uintptr_t)(offsetValue + 4 * i), sizeof(Temporary));
      }
    } else if (Operator_isMemoryRead(operator)) {
      baseTemporary = Operation_arguments(operation)[Operator_baseIndex(operator)];
      baseRegister = Temporary_register(baseTemporary);

      if (Convention_isStackRegister(convention, baseRegister)) {
        offsetTemporary = Operation_arguments(operation)[Operator_offsetIndex(operator)];
        offsetValue = Literal_value(Temporary_LITERAL(offsetTemporary));

        for (int i = 0; i < Operator_memorySize(operator) / 4; i++) {
          virtualTemporary = (Temporary *) HTable_search(Deallocator_SPILLS(this), (HTableKey)(uintptr_t)(offsetValue + 4 * i));
          if (virtualTemporary && (*virtualTemporary == NULL)) {
            *virtualTemporary = Procedure_makeVirtualTemporary(BasicBlock_procedure(currentBlock),
                Temporary_regFile(Operation_results(operation)[0]));
          }
        }
      }
    }
  } BasicBlock_ENDEACH_Operation;

  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(currentBlock, edge) {
    nextBlock = BasicBlockEdge_destBlock(edge);
    if (!IDList_containsSame(blockList, &compareBlocks, &nextBlock)) {
      listItem = (BasicBlock *) IDList_push(blockList, sizeof(BasicBlock));
      *listItem = nextBlock;
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;

  while (*((BasicBlock *) IDList_lastItem(blockList)) != currentBlock)
    Deallocator_findSpillOffsets2(this, blockList);

  IDList_pop_(blockList);
}


#ifdef $XCC_h
/* 
 * Find what stack-frame pointer offsets correspond to spills
 * and create their Virtual Temporary replacements
 */
void
Deallocator_findSpillOffsets(Deallocator this);
#endif//$XCC_h

void
Deallocator_findSpillOffsets(Deallocator this)
{
  Procedure procedure = Deallocator_procedure(this);
  BasicBlock firstBlock = (BasicBlock) IDList_firstItem(Procedure_blockChain(procedure));
  IDList_ blockList[1];
  BasicBlock * listItem;

  IDList_Ctor(blockList, Deallocator_MEMORY(this));
  listItem = (BasicBlock *) IDList_push(blockList, sizeof(BasicBlock));
  *listItem = firstBlock;

  /* Do a depth-first traversal of the basic-block graph */
  Deallocator_findSpillOffsets2(this, blockList);

  /* Clean up the uninitialized variables */
  HTable_FOREACH(Deallocator_SPILLS(this), key, Temporary, temporary) {
    if (*temporary == NULL) {
      HTable_remove(Deallocator_SPILLS(this), key, NULL);
      Deallocator_addOutput(this);
    }
  } HTable_ENDEACH;

  IDList_Dtor(blockList);
}


#ifdef $XCC_h
/*
 * Remove spill code in a procedure by replacing the loaded data by virtual temporaries
 */
void
Deallocator_removeSpills(Deallocator this);
#endif//$XCC_h

void
Deallocator_removeSpills(Deallocator this)
{
  Procedure procedure = Deallocator_procedure(this);
  Convention convention = Procedure_convention(procedure);
  Operator operator;
  int sizeIndex;
  const OperatorParameter_ * parameters;
  OperatorParameterArray parameterArray;
  Temporary baseTemporary, offsetTemporary;
  Temporary * virtualTemporary;
  Register baseRegister;
  ImmediateValue offsetValue;

  /* Prepare the spill replacement variables */
  Deallocator_findSpillOffsets(this);

  /* Iterate over BasicBlocks to remove spills */
  Procedure_FOREACH_BasicBlock(procedure, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      operator = Operation_operator(operation);

      if (Operator_isMemoryWrite(operator)) {
        baseTemporary = Operation_arguments(operation)[Operator_baseIndex(operator)];
        baseRegister = Temporary_register(baseTemporary);

        if (Convention_isStackRegister(convention, baseRegister)) {
          parameterArray = (OperatorParameterArray) Operator_parameterArray(operator);
          parameters = OperatorParameterArray_readParameters(parameterArray);

          offsetTemporary = Operation_arguments(operation)[Operator_offsetIndex(operator)];
          offsetValue = Literal_value(Temporary_LITERAL(offsetTemporary));

          if (HTable_search(Deallocator_SPILLS(this), (HTableKey) offsetValue)) {
            Operation_ARGUMENTS_FOREACH_Temporary_Index(operation, temporary, index) {
              if (OperatorParameter_kind(&parameters[index]) == OperatorParameterKind_ReadStored) {
                virtualTemporary = (Temporary *) HTable_search(Deallocator_SPILLS(this),
                    (HTableKey) offsetValue);
                *virtualTemporary = Procedure_makeVirtualTemporary(procedure, Temporary_regFile(temporary));

                Deallocator_replaceAssignedUpwards(this, temporary, *virtualTemporary,
                    basicBlock, Operation_prev(operation));
                offsetValue += 4;
              }
            } Operation_ARGUMENTS_ENDEACH_Temporary_Index;

            BasicBlock_detachOperation(basicBlock, operation);
          }
        }
      } else if (Operator_isMemoryRead(operator)) {
        baseTemporary = Operation_arguments(operation)[Operator_baseIndex(operator)];
        baseRegister = Temporary_register(baseTemporary);

        if (Convention_isStackRegister(convention, baseRegister)) {
          offsetTemporary = Operation_arguments(operation)[Operator_offsetIndex(operator)];
          offsetValue = Literal_value(Temporary_LITERAL(offsetTemporary));

          if (HTable_search(Deallocator_SPILLS(this), (HTableKey) offsetValue)) {
            Operation_RESULTS_FOREACH_Temporary_Index(operation, temporary, index) {
              virtualTemporary = (Temporary *) HTable_search(Deallocator_SPILLS(this),
                  (HTableKey) offsetValue);

              Deallocator_replaceAssignedDownwards(this, temporary, *virtualTemporary,
                  basicBlock, Operation_next(operation), true);
              offsetValue += 4;
            } Operation_RESULTS_ENDEACH_Temporary_Index;

            BasicBlock_detachOperation(basicBlock, operation);
          }
        }
      }
    } BasicBlock_ENDEACH_Operation;
  } Procedure_ENDEACH_BasicBlock;
}


#ifdef $XCC_h
/*
 * Analyze a procedure after deallocation to update the sizes of the
 * various stack-frame regions
 */
void
Deallocator_analyzeFrame(Deallocator this);
#endif//$XCC_h

void
Deallocator_analyzeFrame(Deallocator this)
{
  bool isTail = true;
  bool isDynamic = false;
  Procedure procedure = Deallocator_procedure(this);
  Convention convention = Procedure_convention(procedure);
  Frame frame = Procedure_frame(procedure);

  /* The local storage is of null size after deallocation */
  Frame_setSpillSize(frame, 0);

  /* Check if the function is a tail (i.e does not make calls) and
   * if a frame marker is present (indicating a dynamically sized
   * stack-frame) */
  Procedure_FOREACH_BasicBlock(procedure, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);

      if (BasicBlock_isEntry(basicBlock) && Operator_isAdjust(operator)) {
        Temporary arg = Operation_getArgument(operation, 0);
        Temporary res = Operation_getResult(operation, 0);
        Except_CHECK(Temporary_hasRegister(arg));
        Except_CHECK(Temporary_hasRegister(res));
        if (Convention_isStackRegister(convention, Temporary_register(arg)) &&
          Convention_isFrameRegister(convention, Temporary_register(res)))
          isDynamic = true;
      }

      if (Operator_isCall(operator)) {
        isTail = false;
        break;
      }
    } BasicBlock_ENDEACH_Operation;

    if (!isTail) /* Shortcut the rest if call has been found */
      break;
  } Procedure_ENDEACH_BasicBlock;
  
  Frame_setFixed(frame, isDynamic);
  Procedure_setTail(procedure, isTail);

  /* Set the output section size */
  Frame_setArgsSize(frame, Deallocator_outputs(this));
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Deallocator module.
 */
#define Deallocator_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Deallocator module.
 */
#define Deallocator_FINI()
#endif//$XCC__h

#if XCC__C
static void
Deallocator_TEST(void)
{
#include "PFA/Deallocator_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(Deallocator_TEST);
  return 0;
}
#endif

