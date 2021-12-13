#ifdef $XCC_h
/*
 * !!!!	SSAForm.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Fabrice Rastello (Fabrice.Rastello@ens-lyon.fr).
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
 * @brief SSA Form services.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/SSAForm.h"
#endif//$XCC__h

#ifdef $XCC__h
//#define SSAForm_MERGESETS 0
#ifndef SSAForm_MERGESETS
extern int SSAForm_MERGESETS;
#endif//SSAForm_MERGESETS
#endif//$XCC__h

#ifndef SSAForm_MERGESETS
int SSAForm_MERGESETS = 0;
#endif//SSAForm_MERGESETS

#ifdef $XCC__h
/**
 * +1 => use LiveSets,
 * +2 => use LiveBitSets,
 * +4 => use Liveness. If &3 && &4 => check.
 * Perform on global SSA variables.
 */
#if _OPTIMIZE
//#define SSAForm_USELIVESETS 0
#endif//_OPTIMIZE
#ifndef SSAForm_USELIVESETS
extern int SSAForm_USELIVESETS;
#endif//SSAForm_USELIVESETS
#endif//$XCC__h

#ifndef SSAForm_USELIVESETS
int SSAForm_USELIVESETS = 0;
#endif//SSAForm_USELIVESETS

#ifdef $XCC__h
/**
 * +1 => use LiveSets,
 * +2 => use Liveness. If &3 => check.
 * Perform on global SSA variables.
 */
#if _OPTIMIZE
//#define SSAForm_USELIVECHECK 0
#endif//_OPTIMIZE
#ifndef SSAForm_USELIVECHECK
extern int SSAForm_USELIVECHECK;
#endif//SSAForm_USELIVECHECK
#endif//$XCC__h

#ifndef SSAForm_USELIVECHECK
int SSAForm_USELIVECHECK = 0;
#endif//SSAForm_USELIVECHECK

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * SSA Form for a CodeRegion.
 */
struct SSAForm_;
#endif//$XCC_h

#ifdef $XCC__h
struct SSAForm_ {
  //@args	Memory memory, CodeRegion codeRegion
  Memory MEMORY;		//!< This SSAForm Memory.
  CodeRegion CODEREGION;	//!< The CodeRegion this SSAForm is for.
  //@access VARIABLES	CodeRegion_variables(SSAForm_CODEREGION(this))
  Dominance DOMINANCE;		//!< The pre-dominance for this SSAForm.
};
#endif//$XCC__h

SSAForm
SSAForm_Ctor(SSAForm this,
             Memory memory, CodeRegion codeRegion)
{
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  *SSAForm__MEMORY(this) = memory;
  *SSAForm__CODEREGION(this) = codeRegion;
  *SSAForm__DOMINANCE(this) = NULL;
  return this;
}

void
SSAForm_Dtor(SSAForm this)
{
  Memory memory = SSAForm_MEMORY(this);
  Dominance_delete(SSAForm_DOMINANCE(this));
}

size_t
SSAForm_Size(Memory memory, CodeRegion codeRegion)
{
  return sizeof(SSAForm_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(SSAForm_) = %zu\n", sizeof(SSAForm_));
}
#endif//$XCC__c

#ifdef $XCC_h
SSAForm
SSAForm_new(Memory parent, CodeRegion codeRegion);
#endif//$XCC_h

SSAForm
SSAForm_new(Memory parent, CodeRegion codeRegion)
{
  Memory memory = Memory_new(parent, true);
  size_t size = SSAForm_Size(memory, codeRegion);
  void *alloc = Memory_alloc(memory, size);
  SSAForm this = SSAForm_Ctor(alloc, memory, codeRegion);
  return this;
}

#ifdef $XCC_h
SSAForm
SSAForm_delete(SSAForm this);
#endif//$XCC_h

SSAForm
SSAForm_delete(SSAForm this)
{
  if (this != NULL) {
    Memory memory = SSAForm_MEMORY(this);
#ifndef _NDTOR
    SSAForm_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Build the @c SSAForm for this @p codeRegion
 *
 * @param parent is the memory location where the @c SSAForm will reside
 * @param codeRegion is the function to convert to SSA
 * @param sortEnterEdges controls wether the arrays of entry edges of basic
 * blocks should be sorted or not.
 */
SSAForm
SSAForm_construct(Memory parent, CodeRegion codeRegion, bool sortEnterEdges);
#endif//$XCC_h

SSAForm
SSAForm_construct(Memory parent, CodeRegion codeRegion, bool sortEnterEdges)
{
  Memory memory = Memory_new(parent, true);
  size_t size = SSAForm_Size(memory, codeRegion);
  void *alloc = Memory_alloc(memory, size);
  SSAForm this = SSAForm_Ctor(alloc, memory, codeRegion);
  CodeRegion_setSSAForm(codeRegion, this);
  // Make the BasicBlock ENTEREDGES of BasicBlock(s).
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_makeEnterEdges(basicBlock, memory, sortEnterEdges);
  } CodeRegion_ENDEACH_BasicBlock;
  // Ensure a call is followed by a BB with no more than one predecessor
  if ( Optimize_CONVERSION(CodeRegion_optimize(codeRegion)) & OptimizeConversion_Dedicated ) 
    SSAForm_splitAfterCall(this);
  // Prepare the TEMPORARYTABLE and DOMINANCE for SSAForm.
  {
    TemporaryFlags mask = TemporaryFlag_Constant|TemporaryFlag_RenameOK;
    TemporaryTable globalTable = CodeRegion_makeGlobalTable(codeRegion, mask,
                                                            TemporaryFlag_RenameOK);
    Dominance dominance = SSAForm_makeDominance(this, DominanceFlag_Frontiers);
    SSAConstruct ssaConstruct = SSAConstruct_new(memory, this); // side-effect ⇒ builds SSA for CodeRegion
    SSAConstruct_delete(ssaConstruct);
    // Test bench for LiveSets.
    if (SSAForm_USELIVESETS>0) {
      Temporary removed = NULL;
      Liveness liveness = NULL; 
      do {
        removed = NULL;
        TemporaryTable_FOREACH_Temporary(globalTable, temporary) {
          if (!Temporary_isVariable(temporary)) {
            TemporaryTable_remove(globalTable, temporary);
            removed = temporary;
            break;
          }
        } TemporaryTable_ENDEACH_Temporary;
      } while (removed);
      if (SSAForm_USELIVESETS&0x4) {
        // Test non-SSA dataflow liveness 
        liveness = Liveness_make(memory, codeRegion, globalTable, true);  
      }
      if (SSAForm_USELIVESETS&0x1) {
        // Test PtrSet version
        LoopForest loopForest = LoopForest_new(memory, codeRegion);
        LiveSets liveSets = LiveSets_make(memory, loopForest, globalTable);
        if (liveness) LiveSets_check(liveSets, liveness);
        LiveSets_kill(liveSets);
      } 
      if (SSAForm_USELIVESETS&0x2) {
        // Test BitSet version
        LoopForest loopForest = LoopForest_new(memory, codeRegion);
        LiveBitSets liveBitSets = LiveBitSets_make(memory, loopForest, globalTable);
        if (liveness) LiveBitSets_check(liveBitSets, liveness);
        LiveBitSets_kill(liveBitSets);
      }
      if (liveness) Liveness_kill(liveness);
    }
  }
  return this;
}

#ifdef $XCC_h
/**
 * Finish building the @c SSAForm for all functions & code regions in a @p 
 * program produced by @p minir. This finish bulding the global table and the 
 * dominance.
 *
 * @pre The Minir program should have "SSA: true" in its yaml root mapping.
 * (Can be checked using Minir_isSSA())
 *
 * @param sortEnterEdges controls wether the arrays of entry edges of basic
 * blocks should be sorted or not.
 *
 * SSAForm constructed are attached to the CodeRegion they belong to.
 */
void
/*SSAForm_constructAfterMinir(Memory parent, Program program, bool sortEnterEdges);*/
SSAForm_constructAfterMinir(Minir minir);
#endif//$XCC_h

void
/*SSAForm_constructAfterMinir(Memory parent, Program program, bool sortEnterEdges)*/
SSAForm_constructAfterMinir(Minir minir)
{
  Memory parent = Memory_Root;
  Program program = Minir_program(minir);
  Except_CHECK(Minir_isSSA(minir));

  Program_FOREACH_Procedure(program, procedure) {
    Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
      TemporaryFlags mask = TemporaryFlag_Constant|TemporaryFlag_RenameOK;
      TemporaryTable globalTable = CodeRegion_makeGlobalTable(codeRegion, mask,
                                                            TemporaryFlag_RenameOK);
      SSAForm ssaForm = SSAForm_new(parent, codeRegion);
      Dominance dominance = SSAForm_makeDominance(ssaForm, DominanceFlag_Frontiers);
      CodeRegion_setSSAForm(codeRegion, ssaForm);
    } Procedure_ENDEACH_CodeRegion;
  } Program_ENDEACH_Procedure;
}



#ifdef $XCC__h
/**
 * Check that SSA is in conventional form.
 *
 * Checks that all SSA variables in an SSA web can be coalesced (i.e., no
 * interference between any two variables).
 */
void
SSAForm_checkConventional(SSAForm this, TemporaryTable globalTable);
#endif//$XCC__h

void
SSAForm_checkConventional(SSAForm this, TemporaryTable globalTable)
{
  if (!TemporaryTable_isEmpty(globalTable)) {
    Coalescer_ coalescer[1];
    VariableClass class = NULL;
    Memory memory = SSAForm_MEMORY(this);
    CodeRegion codeRegion = SSAForm_codeRegion(this);
    Dominance dominance = SSAForm_makeDominance(this, 0);
    Coalescer_Ctor(coalescer, memory, codeRegion, globalTable, dominance, 0);
    Coalescer_complete(coalescer, false);
    Coalescer_classesFromSSAwebs(coalescer);
    class = Coalescer_findInterferingClass(coalescer);
    if (class != NULL) {
#ifndef _NTRACE
      fprintf(stderr, "Interfering class: ");
      VariableClass_pretty(class, stderr, true);
      fprintf(stderr, "\n");
#endif//_NTRACE
      Except_CHECK(class == NULL);
    }
    Coalescer_Dtor(coalescer);
  }
}

#ifdef $XCC_h
/**
 * Destruct this SSAForm.
 */
SSAForm
SSAForm_destruct(SSAForm this);
#endif//$XCC_h

SSAForm
SSAForm_destruct(SSAForm this)
{
  if (this != NULL) {
    Memory memory = SSAForm_MEMORY(this);
    CodeRegion codeRegion = SSAForm_CODEREGION(this);
    TemporaryTable globalTable = CodeRegion_globalTable(codeRegion);
    SSAForm_makeConventional(this, globalTable);
    if (!_OPTIMIZE) {
      SSAForm_checkConventional(this, globalTable);
    }
    // Prepare for the SSA elimination: rebuild web links.
    // Coalesce phis & Coalesce copies that have Variable_isCoalesced Flag.
    SSAForm_weblinksInit(this, globalTable);
    SSAForm_weblinksFromSSAwebs(this, globalTable);

    // Eliminate the SSA operations and variables.
    SSAForm_eliminate(this);

    // Kill the BasicBlock ENTEREDGES of BasicBlock(s).
    CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
      BasicBlock_killEnterEdges(basicBlock, memory);
    } CodeRegion_ENDEACH_BasicBlock;

    CodeRegion_setSSAForm(codeRegion, NULL);
    PFA_STATS && SSAForm_dumpStats(this, PFA_STATS);
#ifndef _NDTOR
    SSAForm_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
    PFA_TRACE && (GETENV("SSA_TRACE"))
              && fprintf(PFA_TRACE, " *** After SSA destruct\n")
              && CodeRegion_pretty(codeRegion, PFA_TRACE);
  }
  return NULL;
}


#ifdef $XCC_h
/**
 * Make this SSAForm conventional.
 * All variables of an SSA-web (i.e., phi-functions related) will not be
 * interfering with one another.
 *
 * @note The @p temporaryTable will be emptied and filled with PHI-related resources 
 * and other candidates for coalescing.
 */
void
SSAForm_makeConventional(SSAForm this, TemporaryTable temporaryTable);
#endif//$XCC_h

void
SSAForm_makeConventional(SSAForm this, TemporaryTable temporaryTable)
{
  Memory memory = SSAForm_MEMORY(this);
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  Optimize optimize = CodeRegion_optimize(codeRegion);
  OptimizeFlags coalescingFlags = Optimize_COALESCING(optimize);
  Dominance dominance = SSAForm_makeDominance(this, 0);
  SSAForm_prepareDedicated(this);
  // Fill the temporaryTable with PHI-related resources, and optionally
  // other candidates to coalescing. Here we include pcopy operands
  // that may have been inserted for register constraints.
  TemporaryTable_empty(temporaryTable);
  Dominance_FORDFSORDER_BasicBlock(dominance, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      if (Operator_isSsa(operator) || Operator_isParCopy(operator)) {
        Operation_OPERANDS_FOREACH_Temporary(operation, operand) {
          Except_CHECK(Temporary_isVariable(operand));
          TemporaryTable_insert(temporaryTable, operand);
        } Operation_OPERANDS_ENDEACH_Temporary;
      } else if (Operator_isSeqCopy(operator)) {
        Temporary result = Operation_getResult(operation, 0);
        Temporary argument = Operation_getArgument(operation, 0);
        if (Temporary_isVariable(result) && Temporary_isVariable(argument)) {
          TemporaryTable_insert(temporaryTable, result);
          TemporaryTable_insert(temporaryTable, argument);
        }
      }
    } BasicBlock_ENDEACH_Operation;
  } Dominance_ENDDFSORDER_BasicBlock;
  // Make Operation ORDERING(s), with steps 2
  SSAForm_makeOrderings(this, 2);
  // Nothing else if no variables to conventionalize.
  if (TemporaryTable_isEmpty(temporaryTable)) return;
  // Test bench for LiveCheck
  if (SSAForm_USELIVECHECK>0) {
    Liveness liveness = NULL;
    CodeRegion_makeConsumers(codeRegion, temporaryTable);
    if (SSAForm_USELIVECHECK&0x2) {
      liveness = Liveness_make(memory, codeRegion, temporaryTable, true);  
    }
    if (SSAForm_USELIVECHECK&0x1) {
      LoopForest loopForest = LoopForest_new(memory, codeRegion);
      LiveCheck liveCheck = LiveCheck_make(memory, dominance, loopForest);
      if (liveness) LiveCheck_check(liveCheck, temporaryTable);
      LiveCheck_kill(liveCheck);
    } 
    if (liveness) Liveness_kill(liveness);
  }
  if (coalescingFlags & OptimizeCoalescing_Decoalesce) {
    Decoalescer decoalescer = Decoalescer_new(memory, codeRegion, dominance);
    Decoalescer_delete(decoalescer);
  } 
  else if (coalescingFlags & OptimizeCoalescing_Boissinot) {
    Boissinot boissinot = Boissinot_new(memory, codeRegion, dominance);
    Boissinot_delete(boissinot);
  } 
  else if (coalescingFlags & OptimizeCoalescing_Sreedhar) {
    Sreedhar sreedhar = Sreedhar_new(memory, codeRegion, dominance);
    Sreedhar_delete(sreedhar);
  }
  Except_CHECK(SSAForm_checkOrderings(this));
  Except_CHECK(SSAForm_checkProducerConsumers(this));
#ifdef FLO_DEBUG
  fprintf(stderr, " *** After SSA make conventional\n");
  CodeRegion_pretty(codeRegion, stderr);
#endif//FLO_DEBUG
}

#ifdef $XCC__h
/**
 * if successor block of a call has several predecessors, split the edge
 * This is a requirement for:
 *  - SSADestruct when dealing with dedicated
 *  - Out of colored SSA
 */
void 
SSAForm_splitAfterCall(SSAForm this);
#endif//$XCC__h

void 
SSAForm_splitAfterCall(SSAForm this)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  fprintf(stderr, "Function SSAForm_splitAfterCall: higly likely to be buggy!");
  Except_CHECK(false);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    Operation lastOp = BasicBlock_lastOperation(basicBlock);
    /* Note that exit block do not need to be followed by an empty block. */
    if (   !BasicBlock_isExit(basicBlock)
        && lastOp && Operator_isRoutine(Operation_OPERATOR(lastOp))) {
      BasicBlockEdge leaveEdge = BasicBlock_leaveFirst(basicBlock);
      Except_CHECK(leaveEdge);
      if(BasicBlockEdge_isFalse(leaveEdge, NULL)) {
        BasicBlockEdge_splitFalse(codeRegion, leaveEdge);
      } else {
        BasicBlockEdge_splitCritical(codeRegion, leaveEdge);
      }
    }
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * Add parallel copies to materialize dedicated constraints.
 */
void
SSAForm_prepareDedicated(SSAForm this);
#endif//$XCC__h

void
SSAForm_prepareDedicated(SSAForm this)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlockEdge leaveEdge = BasicBlock_leaveFirst(basicBlock);
    BasicBlock destBlock = BasicBlockEdge_destBlock(leaveEdge);
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Selector selector = BasicBlock_selector(basicBlock);
      // Materialize register constraints of operation.
      if (   Operation_hasRegisters(operation)
          && !Operator_isParCopy(Operation_operator(operation))) {
        Operation pcopy_operation = NULL;
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          int32_t argIdx = Operation_ARGUMENTS_INDEX, copyIdx = 0;
          Register registre = Operation_getUseRegister(operation, argIdx);
          if (Temporary_isVariable(argument) && registre != Register__UNDEF) {
            Variable variable = Temporary_VARIABLE(argument);
            Temporary origin = Variable_ORIGIN(variable);
            Temporary new_argument = CodeRegion_makeVariableTemporary(codeRegion, origin);
            Variable new_variable = Temporary_VARIABLE(new_argument);
            *Variable__PINNED(new_variable) = registre;
            if (pcopy_operation == NULL) {
              int32_t argCount = Operation_argCount(operation);
              pcopy_operation = Selector_makeOperationParCopy(selector, Operator_PCOPY,
                                                              argCount, true);
              Selector_moveOperationBefore(selector, operation, pcopy_operation);
            }
            copyIdx = Operation_addtoParCopy(pcopy_operation, new_argument, argument);
            Variable_setOperationArgument(new_variable, operation, argIdx);
            Variable_setOperationArgument(variable, pcopy_operation, copyIdx);
            Variable_setOperationResult(new_variable, pcopy_operation, copyIdx);
            Operation_setDefRegister(pcopy_operation, copyIdx, registre);
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
        pcopy_operation = NULL;
        if (!Operator_isRoutine(Operation_OPERATOR(operation))) {
          Operation_RESULTS_FOREACH_Temporary(operation, result) {
            int32_t resIdx = Operation_RESULTS_INDEX, copyIdx = 0;
            Register registre = Operation_getDefRegister(operation, resIdx);
            if (Temporary_isVariable(result) && registre != Register__UNDEF) {
              Variable variable = Temporary_VARIABLE(result);
              Temporary origin = Variable_ORIGIN(variable);
              Temporary new_result = CodeRegion_makeVariableTemporary(codeRegion, origin);
              Variable new_variable = Temporary_VARIABLE(new_result);
              *Variable__PINNED(new_variable) = registre;
              if (pcopy_operation == NULL) {
                int32_t resCount = Operation_resCount(operation);
                pcopy_operation = Selector_makeOperationParCopy(selector, Operator_PCOPY,
                                                                resCount, true);
                Selector_moveOperationAfter(selector, operation, pcopy_operation);
              }
              copyIdx = Operation_addtoParCopy(pcopy_operation, result, new_result);
              Variable_setOperationResult(new_variable, operation, resIdx);
              Variable_setOperationArgument(new_variable, pcopy_operation, copyIdx);
              Variable_setOperationResult(variable, pcopy_operation, copyIdx);
              Operation_setUseRegister(pcopy_operation, copyIdx, registre);
            }
          } Operation_RESULTS_ENDEACH_Temporary;
        } else /*Operator_isRoutine(Operation_OPERATOR(operation))*/
        if (   BasicBlock_leaveCount(basicBlock) == 1
            && BasicBlock_enterCount(destBlock) == 1
            && BasicBlock_index(destBlock) > 0) {
          Operation_RESULTS_FOREACH_Temporary(operation, result) {
            int32_t resIdx = Operation_RESULTS_INDEX, copyIdx = 0;
            Register registre = Operation_getDefRegister(operation, resIdx);
            if (Temporary_isVariable(result) && registre != Register__UNDEF) {
              Variable variable = Temporary_VARIABLE(result);
              Temporary origin = Variable_ORIGIN(variable);
              Temporary new_result = CodeRegion_makeVariableTemporary(codeRegion, origin);
              Variable new_variable = Temporary_VARIABLE(new_result);
              *Variable__PINNED(new_variable) = registre;
              if (pcopy_operation == NULL) {
                int32_t resCount = Operation_resCount(operation);
                Selector nextSelector = BasicBlock_selector(destBlock);
                pcopy_operation = Selector_makeOperationParCopy(nextSelector, Operator_PCOPY,
                                                                resCount, true);
                Selector_moveOperationFirst(nextSelector, pcopy_operation);
              }
              copyIdx = Operation_addtoParCopy(pcopy_operation, result, new_result);
              Variable_setOperationResult(new_variable, operation, resIdx);
              Variable_setOperationArgument(new_variable, pcopy_operation, copyIdx);
              Variable_setOperationResult(variable, pcopy_operation, copyIdx);
              Operation_setUseRegister(pcopy_operation, copyIdx, registre);
            }
          } Operation_RESULTS_ENDEACH_Temporary;
        }
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This SSAForm Memory.
 */
Memory
SSAForm_memory(SSAForm this);
#endif//$XCC_h

Memory
SSAForm_memory(SSAForm this)
{
  return SSAForm_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This SSAForm CodeRegion.
 */
CodeRegion
SSAForm_codeRegion(SSAForm this);
#endif//$XCC_h

CodeRegion
SSAForm_codeRegion(SSAForm this)
{
  return SSAForm_CODEREGION(this);
}

#ifdef $XCC_h
/**
 * Make Operation ordering for this SSAForm.
 */
void
SSAForm_makeOrderings(SSAForm this, int32_t increment);
#endif//$XCC_h

void
SSAForm_makeOrderings(SSAForm this, int32_t increment)
{
  int32_t ordering = 0;
  Dominance dominance = SSAForm_DOMINANCE(this);
  Dominance_FORDFSORDER_BasicBlock(dominance, basicBlock) {
    ordering = BasicBlock_makeOrderings(basicBlock, ordering, increment);
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        if (Temporary_isVariable(result)) {
          Variable variable = Temporary_VARIABLE(result);
          *Variable__ORDERING(variable) = Operation_ordering(operation);
        }
      } Operation_RESULTS_ENDEACH_Temporary;
    } BasicBlock_ENDEACH_Operation;
  } Dominance_ENDDFSORDER_BasicBlock;
}

#ifdef $XCC_h
/**
 * Make pre-dominance for this SSAForm.
 */
Dominance
SSAForm_makeDominance(SSAForm this, DominanceFlags flags);
#endif//$XCC_h

Dominance
SSAForm_makeDominance(SSAForm this, DominanceFlags flags)
{
  Memory memory = SSAForm_MEMORY(this);
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  Dominance dominance = SSAForm_DOMINANCE(this);
  flags &= ~DominanceFlag_Backward;
  flags |= DominanceFlag_Forward;
  if (SSAForm_MERGESETS>0 || OldLiveCheck_MERGESETS>0) {
    flags &= ~DominanceFlag_Frontiers;
    flags |= DominanceFlag_MergeSets;
  }
  if (   dominance == NULL
      || (Dominance_FLAGS(dominance) & flags) != flags) {
    dominance = Dominance_new(memory, codeRegion, flags);
    *SSAForm__DOMINANCE(this) = dominance;
  }
  return dominance;
}

#ifdef $XCC_h
/**
 * Kill this SSAForm pre-dominance.
 *
 * Must be called to invalidate DOMINANCE after the control-flow graph is changed.
 */
void
SSAForm_killDominance(SSAForm this);
#endif//$XCC_h

void
SSAForm_killDominance(SSAForm this)
{
  if (SSAForm_DOMINANCE(this) != NULL) {
    Dominance_delete(SSAForm_DOMINANCE(this));
  }
  *SSAForm__DOMINANCE(this) = NULL;
}

#ifdef $XCC_h
/**
 * For use by SSAForm_FOREACH_Variable.
 */
IBList
SSAForm_variables(SSAForm this);
#endif//$XCC_h

IBList
SSAForm_variables(SSAForm this)
{
  return SSAForm_VARIABLES(this);
}

#ifdef $XCC_h
/**
 * Iterate this SSAForm Variable(s).
 */
#define SSAForm_FOREACH_Variable(this, variable) \
  IBList_FOREACH(SSAForm_variables(this), Variable_, variable)
#define SSAForm_ENDEACH_Variable \
  IBList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Verify that this SSAForm is strict.
 *
 * Require:	SSAForm_makeDominance updated.
 */
bool
SSAForm_isStrict(const_SSAForm this);
#endif//$XCC_h

bool
SSAForm_isStrict(const_SSAForm this)
{
  bool result = true;
  Dominance dominance = SSAForm_DOMINANCE(this);
  int32_t *preDFN = Dominance_PREDFN(dominance);
  int32_t *lastDFN = Dominance_LASTDFN(dominance);
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    int32_t consumer_index = BasicBlock_index(basicBlock);
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Except_CHECK(basicBlock == Operation_basicBlock(operation));
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        if (Temporary_isVariable(argument)) {
          Variable variable = Temporary_VARIABLE(argument);
          OperationResult producer = Variable_PRODUCER(variable);
          Operation producer_operation = OperationResult_OPERATION(producer);
          BasicBlock producer_block = Operation_basicBlock(producer_operation);
          int32_t producer_index = BasicBlock_index(producer_block);
          result |= Dominance_DOMINATES(preDFN, lastDFN, producer_index, consumer_index);
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Fill the TemporaryTable with Variable(s).
*/
void
SSAForm_fillTemporaryTable(SSAForm this, TemporaryTable table);
#endif//$XCC_h

void
SSAForm_fillTemporaryTable(SSAForm this, TemporaryTable table)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        if (Temporary_isVariable(result)) {
          TemporaryTable_insert(table, result);
        }
      } Operation_RESULTS_ENDEACH_Temporary;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC_h
/**
 * Check validity Variable PRODUCER and CONSUMER.
 */
bool
SSAForm_checkOrderings(SSAForm this);
#endif//$XCC_h

bool
SSAForm_checkOrderings(SSAForm this)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    if (!BasicBlock_checkOrderings(basicBlock)) return false;
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      int32_t ordering = Operation_ordering(operation);
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        if (Temporary_isVariable(result)) {
          Variable variable = Temporary_VARIABLE(result);
          if (Variable_ORDERING(variable) != ordering) {
            Except_CHECK(Variable_ORDERING(variable) == ordering);
            return false;
          }
        }
      } Operation_RESULTS_ENDEACH_Temporary;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  return true;
}

#ifdef $XCC_h
/**
 * Check validity Variable PRODUCER and CONSUMER.
 */
bool
SSAForm_checkProducerConsumers(SSAForm this);
#endif//$XCC_h

bool
SSAForm_checkProducerConsumers(SSAForm this)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        if (Temporary_isVariable(result)) {
          Variable variable = Temporary_VARIABLE(result);
          OperationResult producer = Variable_PRODUCER(variable);
          if (   OperationResult_OPERATION(producer) != operation
              || OperationResult_RESINDEX(producer) != Operation_RESULTS_INDEX) {
            Except_CHECK(OperationResult_OPERATION(producer) == operation);
            Except_CHECK(OperationResult_RESINDEX(producer) == Operation_RESULTS_INDEX);
            return false;
          }
        }
      } Operation_RESULTS_ENDEACH_Temporary;
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        if (Temporary_isVariable(argument)) {
          Variable variable = Temporary_VARIABLE(argument);
          if (Variable_isConsuming(variable)) {
            bool inUse = false;
            Variable_CONSUMERS_FOREACH_OperationArgument(variable, consumer) {
              if (    OperationArgument_OPERATION(consumer) == operation
                  &&  OperationArgument_ARGINDEX(consumer) == Operation_ARGUMENTS_INDEX) {
                inUse = true;
              }
            } Variable_CONSUMERS_ENDEACH_OperationArgument;
            if (!inUse) {
              Except_CHECK(variable == NULL);
              return false;
            }
          }
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Empty all variable web link of the global table.
 */
void 
SSAForm_emptyVariableWebs(SSAForm this, TemporaryTable globalTable);
#endif//$XCC__h

void 
SSAForm_emptyVariableWebs (SSAForm this, TemporaryTable globalTable)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  TemporaryTable_FOREACH_Temporary(globalTable, temporary) {
    Variable variable = Temporary_VARIABLE(temporary);
    *Variable__WEBLINK(variable) = NULL;
    *Variable__WEBRANK(variable) = 0;
  } TemporaryTable_ENDEACH_Temporary;
}

#ifdef $XCC__h
/**
 * Initialize web links from pinned variables
 */
void
SSAForm_weblinksInit(SSAForm this, TemporaryTable temporaryTable);
#endif//$XCC__h

void
SSAForm_weblinksInit(SSAForm this, TemporaryTable temporaryTable)
{
  Variable *dedicatedRoot = NULL;
  Memory memory = SSAForm_MEMORY(this);
  CodeRegion codeRegion = SSAForm_codeRegion(this);
  TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
    Variable variable = Temporary_VARIABLE(temporary);
    Register registre = Variable_PINNED(variable);
    // Ensure the congruence fields are cleared.
    *Variable__WEBLINK(variable) = NULL;
    *Variable__WEBRANK(variable) = 0;
    // If the variable is constrained to a specific register we must mark the congruence class.
    if (registre != Register__UNDEF) {
      if (dedicatedRoot == NULL) {
        dedicatedRoot = Memory_alloc(memory, Register__Count*sizeof(Variable));
        memset(dedicatedRoot, 0, Register__Count*sizeof(Variable));
      }
      if (dedicatedRoot[registre] == NULL) {
        dedicatedRoot[registre] = variable;
      } else {
        Variable_linkWebRoots(dedicatedRoot[registre], variable);
      }
    }
  } TemporaryTable_ENDEACH_Temporary;
  if (0&& PFA_TRACE && dedicatedRoot != NULL) {
    fprintf(PFA_TRACE, "*** Weblink initialized\n");
    TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
      Variable variable = Temporary_VARIABLE(temporary);
      PFA_TRACE && Variable_pretty(variable, PFA_TRACE, false) && fprintf(PFA_TRACE, " => ")
                && Variable_pretty(Variable_findWebRoot(variable), PFA_TRACE, false)
                && fprintf(PFA_TRACE, "\n");
    } TemporaryTable_ENDEACH_Temporary;
  }
  if (dedicatedRoot != NULL) {
    Memory_free(memory, dedicatedRoot);
  }
}

#ifdef $XCC__h
/**
 * Make the SSAForm web links as union-find trees.
 *   
 * @pre
 *   - all phi operands are supposed to be coalesced.
 *   - coalesced copies have @c Variable_isCoalesced()
 */
void
SSAForm_weblinksFromSSAwebs(SSAForm this, TemporaryTable temporaryTable);
#endif//$XCC__h

void
SSAForm_weblinksFromSSAwebs(SSAForm this, TemporaryTable temporaryTable)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      if (Operator_isSsa(operator)) {
        int32_t operandCount = Operation_operandCount(operation), i;
        Temporary *operands = Operation_operands(operation);
        Variable variable_0 = Temporary_VARIABLE(operands[0]);
        Variable webRoot_0 = Variable_findWebRoot(variable_0);
        for (i = 1; i < operandCount; i++) {
          Variable variable_i = Temporary_VARIABLE(operands[i]);
          Variable webRoot_i = Variable_findWebRoot(variable_i);
          if (webRoot_0 != webRoot_i) {
            if (   TemporaryTable_contains(temporaryTable, Variable_TEMPORARY(webRoot_0)) 
                && TemporaryTable_contains(temporaryTable, Variable_TEMPORARY(webRoot_i))) { 
              webRoot_0 = Variable_linkWebRoots(webRoot_0, webRoot_i);
            }
          }
        }
      } else
      if (   Operator_isSeqCopy(operator)
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
              Variable source_webRoot = Variable_findWebRoot(source);
              Variable target_webRoot = Variable_findWebRoot(target);
              if (source_webRoot != target_webRoot) {
                Variable_linkWebRoots(source_webRoot, target_webRoot);
              }
            }
          }
        }
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * Eliminate the PHI, PSI and SIGMA Operation(s) and SSA rename-back.
 *
 * @pre the Variable webs are constructed with @c SSAForm_weblinksFromSSAwebs()
 *
 * @note if Variables are assigned to registers, their renames will have the
 * same assignment. This will only work if all PHI operands are in memory or
 * this could lead to destructions of values in registers when PHI are replaced
 * by copies.
 */
void
SSAForm_eliminate(SSAForm this);
#endif//$XCC__h

void
SSAForm_eliminate(SSAForm this)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  Optimize optimize = CodeRegion_optimize(codeRegion);
  Procedure procedure = CodeRegion_procedure(codeRegion);
  // Optionally limit the reuse of Virtual Temporary(ies) when eliminating variables.
  if (FORCE_REUSELIMIT > 0) {
    int32_t limit = FORCE_REUSELIMIT;
    CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
      BasicBlock_FOREACH_Operation(basicBlock, operation) {
        Operation_OPERANDS_FOREACH_Temporary(operation, temporary) {
          if (Temporary_isVariable(temporary)) {
            Variable variable = Temporary_VARIABLE(temporary);
            Temporary origin = Variable_ORIGIN(variable);
            Except_CHECK(Temporary_isVirtual(origin));
            if (Temporary_WEBROOT(origin) == NULL) {
              *Temporary__WEBROOT(origin) = (void *)0x1;
              if (--limit < 0) return;
            }
          }
        } Operation_OPERANDS_ENDEACH_Temporary;
      } BasicBlock_ENDEACH_Operation;
    } CodeRegion_ENDEACH_BasicBlock;
  }
  // First, eliminate PCOPY as the elimination algorithm requires Variable(s).
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      if (Operator_isParCopy(operator)) {
        SSAForm_expandParCopyOperation(this, operation);
        BasicBlock_detachOperation(basicBlock, operation);
      } else if (Operator_isSeqCopy(operator)) {
        Temporary result = Operation_getResult(operation, 0);
        Temporary argument = Operation_getArgument(operation, 0);
        if (Temporary_isVariable(result) && Temporary_isVariable(argument)) {
          Variable variable_0 = Temporary_VARIABLE(result);
          Variable variable_1 = Temporary_VARIABLE(argument);
          if (Variable_findWebRoot(variable_0) == Variable_findWebRoot(variable_1)) {
            BasicBlock_detachOperation(basicBlock, operation);
          }
        }
      } else if (Operator_isSsa(operator)) {
        BasicBlock_detachOperation(basicBlock, operation);
      }
    } BasicBlock_ENDEACH_Operation;
    Except_CHECK(BasicBlock_phiCount(basicBlock) == 0);
  } CodeRegion_ENDEACH_BasicBlock;
  // Then, rename the SSA webs into non-Variable Temporary(ies).
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operation_OPERANDS_FOREACH_Temporary(operation, temporary) {
        if (Temporary_isVariable(temporary)) {
          Variable variable = Temporary_VARIABLE(temporary);
          Variable webRoot = Variable_findWebRoot(variable);
          Temporary origin = Variable_ORIGIN(webRoot);
          if (Variable_PINNED(webRoot) != Register__UNDEF) {
            // Use dedicated temporary.
            origin = Procedure_makeDedicatedTemporary(procedure, Variable_PINNED(webRoot));
          } else {
            Except_CHECK(Temporary_isVirtual(origin));
            if (Temporary_WEBROOT(origin) != webRoot) {
              // Virtual Temporary already in use, make another like it.
              origin = Procedure_copyVirtualTemporary(procedure, origin);
              *Temporary__WEBROOT(origin) = webRoot;
              *Variable__ORIGIN(webRoot) = origin;
            } else {
              // Virtual Temporary not used yet for rename back.
              *Temporary__WEBROOT(origin) = webRoot;
            }
            Except_CHECK(Temporary_isRenameOK(origin));
          }
          if(Variable_isAssigned(variable)) {
            if(Temporary_isAssigned(origin)) {
              Except_CHECK(Variable_register(variable) == Temporary_register(origin));
            } else {
              Temporary_assignRegister(origin, Variable_register(variable));
            }
          }
          Operation_OPERANDS[Operation_OPERANDS_INDEX] = origin;
          0 &&
          PFA_TRACE && fprintf(PFA_TRACE, "Find webRoots")
                    && Temporary_pretty(temporary, PFA_TRACE)
                    && Temporary_pretty(origin, PFA_TRACE) 
                    && fprintf(PFA_TRACE, "\n");
        }
      } Operation_OPERANDS_ENDEACH_Temporary;
      // Reset Operation ORDERING.
      Operation_setOrdering(operation, -1);
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * Eliminate the PHI, PSI and SIGMA Operation(s) and SSA rename-back, keeping
 * the memory locations of Variables.
 *
 * @pre the Variables operands of SSA operations must have @c Variable_hasMemory()
 */
void
SSAForm_eliminateColoredWithMemory(SSAForm this);
#endif//$XCC__h

void
SSAForm_eliminateColoredWithMemory(SSAForm this)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  Optimize optimize = CodeRegion_optimize(codeRegion);
  Procedure procedure = CodeRegion_procedure(codeRegion);
  
  // First, eliminate PCOPY as the elimination algorithm requires Variable(s).
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      if (Operator_isParCopy(operator)) {
        SSAForm_expandParCopyOperation(this, operation);
        BasicBlock_detachOperation(basicBlock, operation);
      } else if (Operator_isSeqCopy(operator)) {
        Temporary result = Operation_getResult(operation, 0);
        Temporary argument = Operation_getArgument(operation, 0);
        if (Temporary_isVariable(result) && Temporary_isVariable(argument)) {
          Variable variable_0 = Temporary_VARIABLE(result);
          Variable variable_1 = Temporary_VARIABLE(argument);
          if (Variable_findWebRoot(variable_0) == Variable_findWebRoot(variable_1)) {
            BasicBlock_detachOperation(basicBlock, operation);
          }
        }
      } else if (Operator_isSsa(operator)) {
        BasicBlock_detachOperation(basicBlock, operation);
      }
    } BasicBlock_ENDEACH_Operation;
    Except_CHECK(BasicBlock_phiCount(basicBlock) == 0);
  } CodeRegion_ENDEACH_BasicBlock;

  // Then, rename the SSA webs into non-Variable Temporary(ies).
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operation_OPERANDS_FOREACH_Temporary(operation, temporary) {
        if (Temporary_isVariable(temporary)) {
          Variable variable = Temporary_VARIABLE(temporary);
          Variable webRoot = Variable_findWebRoot(variable);
          Temporary origin = Variable_ORIGIN(webRoot);
          if (Variable_PINNED(webRoot) != Register__UNDEF) {
            // Use dedicated temporary.
            origin = Procedure_makeDedicatedTemporary(procedure, Variable_PINNED(webRoot));
          } else {
            Except_CHECK(Temporary_isVirtual(origin));
            if (Temporary_WEBROOT(origin) != webRoot) {
              // Virtual Temporary already in use, make another like it.
              origin = Procedure_copyVirtualTemporary(procedure, origin);
              *Temporary__WEBROOT(origin) = webRoot;
              *Variable__ORIGIN(webRoot) = origin;
            } else {
              // Virtual Temporary not used yet for rename back.
              *Temporary__WEBROOT(origin) = webRoot;
            }
            Except_CHECK(Temporary_isRenameOK(origin));
          }
          Operation_OPERANDS[Operation_OPERANDS_INDEX] = origin;
          0 &&
          PFA_TRACE && fprintf(PFA_TRACE, "Find webRoots")
                    && Temporary_pretty(temporary, PFA_TRACE)
                    && Temporary_pretty(origin, PFA_TRACE) 
                    && fprintf(PFA_TRACE, "\n");
        }
      } Operation_OPERANDS_ENDEACH_Temporary;
      // Reset Operation ORDERING.
      Operation_setOrdering(operation, -1);
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC__h
/**
 * Generate instructions to transfer the data in @p argument to @p result. 
 * These location can be memory location, in which case appropriate @c load 
 * and @c store instruction will be generated. The instructions will be added 
 * to the @p selector.
 */
void
SSAForm_generateCopyOrMem(SSAForm this, Selector selector, Temporary result,
   Variable V_dst, Temporary argument, Variable V_src);
#endif//$XCC__h

void
SSAForm_generateCopyOrMem(SSAForm this, Selector selector, Temporary result,
   Variable V_dst, Temporary argument, Variable V_src)
{
  Variable varResult = Temporary_VARIABLE(result);
  Variable varArgument = Temporary_VARIABLE(argument);
  if(!Variable_hasMemory(varResult) && !Variable_hasMemory(varArgument)) {
    // Generate regular copy
    Operation copy = Selector_makeOperationCOPY(selector, result, argument);
    Variable_setOperationResult(V_dst, copy, 0);
    Variable_setOperationArgument(V_src, copy, 0);
  }
  else if(Variable_hasMemory(varResult) && Variable_hasMemory(varArgument)) {
    Except_THROW("Cannot handle mem-to-mem copy generation!");
  }
  else if(Variable_hasMemory(varResult)) {
    // Copy value from register to memory => STORE
    Operation store;
    Symbol memory = Variable_memory(varResult);
    Except_CHECK(Temporary_hasRegFile(argument));
    Selector_selectDirectSTORE(selector,
        RegFile_getNativeType(Temporary_regFile(argument)),
        argument, memory, 0); // No offset
    store = Selector_lastOperation(selector);

    Operation_ARGUMENTS_FOREACH_Temporary_Index(store, temporary, index) {
      if(temporary == argument) {
        Variable_setOperationArgument(V_src, store, index);
        break;
      }
    } Operation_ARGUMENTS_ENDEACH_Temporary_Index;
  }
  else {
    // Copy value from memory to register => LOAD
    Operation load;
    Symbol memory = Variable_memory(varArgument);
    Except_CHECK(Temporary_hasRegFile(result));
    Selector_selectDirectLOAD(selector,
        RegFile_getNativeType(Temporary_regFile(result)),
        result, memory, 0); // No offset
    load = Selector_lastOperation(selector);
    Variable_setOperationResult(V_dst, load, 0);
  }
/*  if( Selector_countOperations(selector) != 1) assert(false);*/
/*  Except_THROWIF(*/
/*      Selector_countOperations(selector) != 1,*/
/*      "Callers of function %s expect it to return only one operation in the selector!",*/
/*      __FUNCTION__*/
/*      );*/
}

#ifdef $XCC__h
/**
 * Expand a parallel copy Operation.
 *
 * Note that in the case of multiple definitions of the same temporary,
 * only the first definition is kept.
 * This means that:
 * - the case PCOPY a,...,a = b,...,b is correctly handled
 * - the case PCOPY a,...,a = b,...,c is undefined
 *
 * Note also that this is not optimized. For instance, the copy
 *   b, d, c = a, c, b
 * will generate
 *   tmp = b
 *   b = a
 *   d = c
 *   c = tmp
 *
 *  WARNING: this parallel copy expansion cannot handle the case
 *  where the same register (for different variables) is used as source
 *  and definition of the copy, other than in a load/store
 * 
 *  e.g.: V1($r1), V2($r2) = V3($r3), V4($r1)
 *
 */
void
SSAForm_expandParCopyOperation(SSAForm this, Operation pcopy);
#endif//$XCC__h

void
SSAForm_expandParCopyOperation(SSAForm this, Operation pcopy)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  BasicBlock basicBlock = Operation_basicBlock(pcopy);
  Selector selector = BasicBlock_selector(basicBlock);
  Operation first_point = pcopy, last_point = pcopy;
  Selector_ tempSelector_;
  Selector tempSelector = &tempSelector_;
  // In case register allocation is already done
  RegisterSet_ usedRegisters_;
  RegisterSet usedRegisters = &usedRegisters_;
  RegisterSet_Ctor(usedRegisters);


  Selector_Ctor(tempSelector, Selector_memory(selector),
      Selector_procedure(selector), false);
  
  Except_CHECK(Operator_isParCopy(Operation_operator(pcopy)));
  //
  // First, clear flags `used' and `defined' on operands.
  // These are used to determine cycles in the parallel copy.
  //
  Operation_OPERANDS_FOREACH_Temporary(pcopy, operand) {
    Variable V;
    Except_CHECK(Temporary_isVariable(operand));
    V = Variable_findWebRoot(Temporary_VARIABLE(operand));
    Variable_clearPCOPYDef(V);
    Variable_clearPCOPYUse(V);
  } Operation_OPERANDS_ENDEACH_Temporary;
  Operation_RESULTS_FOREACH_Temporary_Index(pcopy, result, index) {
    Variable V_dst = Variable_findWebRoot(Temporary_VARIABLE(result));
    Temporary argument = Operation_getArgument(pcopy, index);
    Variable V_src = Variable_findWebRoot(Temporary_VARIABLE(argument));
    //
    // First, handle all stores and loads
    //
    if(Variable_hasMemory(V_dst)) {
      // Insert store before first_point and do not update it.
      Except_CHECK(!Variable_hasMemory(V_src));
      SSAForm_generateCopyOrMem(this, tempSelector, result, V_dst, argument, V_src);
      BasicBlock_insertSelector(basicBlock, first_point, tempSelector);
    }
    else if(Variable_hasMemory(V_src)) {
      // Append load after last_point and do not update it.
      SSAForm_generateCopyOrMem(this, tempSelector, result, V_dst, argument, V_src);
      BasicBlock_appendSelector(basicBlock, last_point, tempSelector);
    }
    else {
      // Nothing to do if variables are assigned to the same register
      if(!(Variable_isAssigned(V_src) &&
           Variable_isAssigned(V_dst) &&
           Variable_register(V_src) == Variable_register(V_dst))) {
      //
      // Nothing to do if source is the same as dest, or if dest has already been 
      // defined in the pcopy
        if (V_dst != V_src && !Variable_isPCOPYDef(V_dst)) {
          if (   Variable_isPCOPYDef(V_src)
              && Variable_isPCOPYUse(V_dst)) {
            // Break cycle.
            Temporary origin = Variable_ORIGIN(V_dst);
            Temporary temporary = CodeRegion_makeVariableTemporary(codeRegion, origin);
            Variable V_tmp = Temporary_VARIABLE(temporary);
            Operation copy = NULL;
            //
            copy = Selector_makeOperationCOPY(selector, temporary, argument);
            Selector_moveOperationBefore(selector, first_point, copy);
            Variable_setOperationResult(V_tmp, copy, 0);
            Variable_setOperationArgument(V_src, copy, 0);
            first_point = copy;
            //
            copy = Selector_makeOperationCOPY(selector, result, temporary);
            Selector_moveOperationAfter(selector, last_point, copy);
            Variable_setOperationResult(V_dst, copy, 0);
            Variable_setOperationArgument(V_tmp, copy, 0);
            last_point = copy;
          }
          else if (Variable_isPCOPYDef(V_src)) {
            // Insert copy before first_point.
            Operation first_op;
            SSAForm_generateCopyOrMem(this, tempSelector, result, V_dst, argument, V_src);
            first_op = Selector_firstOperation(tempSelector);
            BasicBlock_insertSelector(basicBlock, first_point, tempSelector);
            first_point = first_op;
          }
          else {
            // Append copy after last_point.
            Operation last_op;
            SSAForm_generateCopyOrMem(this, tempSelector, result, V_dst, argument, V_src);
            last_op = Selector_lastOperation(tempSelector);
            BasicBlock_appendSelector(basicBlock, last_point, tempSelector);
            last_point = last_op;
          }
        }
        Variable_raisePCOPYUse(V_src);
        Variable_raisePCOPYDef(V_dst);
        //
        // WARNING: this parallel copy expansion cannot handle the case
        // where the same register (for different variables) is used as source
        // and definition of the copy, other than in a load/store
        //
        // e.g.: V1($r1), V2($r2) = V3($r3), V4($r1)
        //
        if(Variable_isAssigned(V_src)) {
          Except_NEVER(RegisterSet_contains(usedRegisters, Variable_register(V_src)));
          RegisterSet_insert(usedRegisters, Variable_register(V_src));
        }
        if(Variable_isAssigned(V_dst)) {
          Except_NEVER(RegisterSet_contains(usedRegisters, Variable_register(V_dst)));
          RegisterSet_insert(usedRegisters, Variable_register(V_dst));
        }
      }
    }
  } Operation_RESULTS_ENDEACH_Temporary_Index;
  
  Selector_Dtor(tempSelector);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty-print this SSAForm.
 */
bool
SSAForm_pretty(SSAForm this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
SSAForm_pretty(SSAForm this, FILE *file)
{
  if (file == NULL) file = stderr;
  SSAForm_FOREACH_Variable(this, variable) {
    Temporary temporary = Variable_TEMPORARY(variable);
    OperationResult producer = Variable_PRODUCER(variable);
    Operation producer_operation = OperationResult_OPERATION(producer);
    fprintf(file, "    ") && Temporary_pretty(temporary, file) && fprintf(file, ":\n");
    fprintf(file, "\tProducer") && Operation_pretty(producer_operation, file);
    Variable_CONSUMERS_FOREACH_OperationArgument(variable, consumer) {
      Operation consumer_operation = OperationArgument_OPERATION(consumer);
      fprintf(file, "\tConsumer") && Operation_pretty(consumer_operation, file);
    } Variable_CONSUMERS_ENDEACH_OperationArgument;
  } SSAForm_ENDEACH_Variable;
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Display this SSAForm in VCG format.
 */
bool
SSAForm_display(SSAForm this, FILE *file, const char *name);
#endif//$XCC_h

#ifndef _NTRACE
bool
SSAForm_display(SSAForm this, FILE *file, const char *name)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  if (file == NULL) file = stderr;
  // Start the graph
  fprintf(file, "digraph \"%s\" {\n", name);
  fprintf(file, "  node [shape=record];");
  // Add all basic blocks to the graph
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    int index = BasicBlock_index(block);
    fprintf(file, "  BB%d [label=\"{BB%d|", index, index);
    TemporarySet_display(BasicBlock_phiDefSet(block), file);
    fprintf(file, "|");
    TemporarySet_display(BasicBlock_liveInSet(block), file);
    fprintf(file, "|");
    BasicBlock_FOREACH_Operation(block, operation) {
      if (   Operator_isPhi(Operation_operator(operation))
          || Operator_isSeqCopy(Operation_operator(operation))) {
        //Operation_pretty(operation, file);
        if (Operator_isSeqCopy(Operation_operator(operation)))
          fprintf (file, "COPY\\ :\\ ");
        Temporary_pretty(Operation_getResult(operation, 0), file);
        fprintf(file, "\\ =\\ ");
        if (Operator_isPhi(Operation_operator(operation)))
          fprintf(file, "PHI(");
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          Temporary_pretty(argument, file);
          if (Operation_ARGUMENTS_INDEX != Operation_ARGCOUNT-1)
            fprintf(file, ",\\ ");
        } Operation_ARGUMENTS_ENDEACH_Temporary;
        if (Operator_isPhi(Operation_operator(operation)))
          fprintf(file, ")");
        fprintf(file, "\\n");
      }
    } BasicBlock_ENDEACH_Operation;
    fprintf(file, "|");
    TemporarySet_display(BasicBlock_liveOutSet(block), file);
    fprintf(file, "|");
    TemporarySet_display(BasicBlock_phiUseSet(block), file);
    fprintf(file, "}\"];\n");
    // Now dump all the outgoing edges
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
        fprintf(file, "  BB%d -> BB%d [label=\"%d\"];\n",
                BasicBlock_index(BasicBlockEdge_origBlock(edge)),
                BasicBlock_index(BasicBlockEdge_destBlock(edge)),
                BasicBlockEdge_ARGINDEX(edge));
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  } CodeRegion_ENDEACH_BasicBlock;
  fprintf(file, "}\n");
  return true;
}
#endif//_NTRACE

#ifdef $XCC__h
/**
 * Dumps the Appel Graph.
 */
bool
SSAForm_dumpAppelGraph(SSAForm this, FILE *file, const char *name);
#endif//$XCC__h

#ifndef _NTRACE
bool
SSAForm_dumpAppelGraph(SSAForm this, FILE *file, const char *name)
{
  int dump_phi = 1;
  int n_tmps = IndexedFactory_maxCount(Temporary_Factory);
  int n_dedicated;
  Memory memory = SSAForm_MEMORY(this);
  // We must first compute interferences
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  Program program = Procedure_program(CodeRegion_procedure(codeRegion));
  TemporaryTable globalTable = CodeRegion_globalTable(codeRegion);
  Liveness liveness = Liveness_make(memory, codeRegion, globalTable, true);
  Interference interference = Interference_make(memory, codeRegion, globalTable, false);
  TemporarySet_ universe[1];
  int i;
  IBList_ congruences[1];
  VariableClass *dedicatedClass = NULL;
  n_dedicated = Register__Count > 1?
                Temporary_IDENTITY(Program_dedicated(program) + Register__Count -1) + 1: 0;
  TemporarySet_Ctor(universe, memory, Liveness_SETCOUNT);
  fprintf(file, "(* Procedure %s *)\n", name);
  fprintf(file, "%d %d\n\n", n_tmps, n_dedicated);
  fprintf(file, "(* INTERFERENCE format : x y -1 *)\n");
  fprintf(file, "(* COPY format         : res arg weight *)\n");
  fprintf(file, "(* PCOPY format        : res0 arg0 weight ... resn argn weight *)\n");
  fprintf(file, "(* PHI format          : (*@ PHIbb res arg0 ... argn @*) *)\n");
  fprintf(file, "(* DED format          : (*@ DED ded var0 ... varn @*) *)\n");
  fprintf(file, "\n");
  IBList_Ctor(congruences, memory, sizeof(VariableClass_), Register__Count);
  TemporaryTable_FOREACH_Temporary(globalTable, temporary) {
    Variable variable = Temporary_VARIABLE(temporary);
    VariableClass congruence;
    Register registre = Variable_PINNED(variable);
    if (registre != Register__UNDEF) {
      if (dedicatedClass == NULL) {
        int i;
        dedicatedClass = Memory_alloc(memory, Register__Count*sizeof(VariableClass));
        memset(dedicatedClass, 0, Register__Count*sizeof(VariableClass));
      }
      if (dedicatedClass[registre] == NULL) {
        congruence = IBList_push(congruences);
        dedicatedClass[registre] = congruence;
        VariableClass_Ctor(congruence, variable);
      } else {
        congruence = dedicatedClass[registre];
        VariableClass_insert(congruence, variable);
      }
    } 
  } TemporaryTable_ENDEACH_Temporary;
  if (dedicatedClass != NULL) {
    for(i = 0; i < Register__Count; i++) {
      if (dedicatedClass[i] != NULL) {
        fprintf(file, "(@ DED %d", Temporary_IDENTITY(&Program_dedicated(program)[i]));
        VariableClass_FOREACH_Variable(dedicatedClass[i], variable) {
          fprintf(file, " %d", Temporary_IDENTITY(Variable_TEMPORARY(variable)));
        } VariableClass_ENDEACH_Variable;
        fprintf(file, " @)\n");
      }
    }
    Memory_free(memory, dedicatedClass);
  }
  IBList_empty(congruences, (IBListItemRelease)VariableClass_Dtor);
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    BasicBlock_FOREACH_Operation(block, operation) {
      if (Operator_isSeqCopy(Operation_operator(operation))) {
        Temporary result = Operation_getResult(operation, 0);
        Temporary argument = Operation_getArgument(operation, 0);
        fprintf(file, " %d %d %f\n", Temporary_IDENTITY(result),
                Temporary_IDENTITY(argument), BasicBlock_frequency(block));
        TemporarySet_pushMember(universe, result);
        TemporarySet_pushMember(universe, argument);
      } else if (Operator_isParCopy(Operation_operator(operation))) {
        const char *sep = " ";
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          Temporary result = Operation_getResult(operation, Operation_ARGUMENTS_INDEX);
          fprintf(file, "%s%d %d %f", sep, Temporary_IDENTITY(result),
                  Temporary_IDENTITY(argument), BasicBlock_frequency(block));
          TemporarySet_pushMember(universe, result);
          TemporarySet_pushMember(universe, argument);
        } Operation_ARGUMENTS_ENDEACH_Temporary;
        fprintf(file, "\n");
      } else if (dump_phi && Operator_isPhi(Operation_operator(operation))) {
        const char *sep = " ";
        fprintf(file, " (*@ PHI%d", BasicBlock_index(block));
        fprintf(file, " %d", Temporary_IDENTITY(Operation_getResult(operation, 0)));
        TemporarySet_pushMember(universe, Operation_getResult(operation, 0));
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          fprintf(file, "%s%d", sep, Temporary_IDENTITY(argument));
          TemporarySet_pushMember(universe, argument);
        } Operation_ARGUMENTS_ENDEACH_Temporary;
        fprintf(file, " @*)\n");
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  // Dump Interferences
  TemporarySet_FOREACH_Temporary(universe, temporary_1) {
    TemporarySet_FOREACH_Temporary(universe, temporary_2) {
      if (Interference_exists(interference, temporary_1, temporary_2)) {
        if (Temporary_IDENTITY(temporary_1) > Temporary_IDENTITY(temporary_2)) {
          fprintf(file, " %d %d -1\n", Temporary_IDENTITY(temporary_1),
                  Temporary_IDENTITY(temporary_2));
        }
      }
    } TemporarySet_ENDEACH_Temporary;
  } TemporarySet_ENDEACH_Temporary;
  
  fprintf(file, "(* End Procedure %s *)\n", name);
  Liveness_kill(liveness);
  Interference_kill(interference);
  TemporarySet_Dtor(universe);
  return true;
}
#endif//_NTRACE

#ifdef $XCC__h
/**
 * Dump the statistics on the SSA form.
 */
static bool
SSAForm_dumpStats(const_SSAForm this, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
static bool
SSAForm_dumpStats(const_SSAForm this, FILE *file)
{
  CodeRegion codeRegion = SSAForm_CODEREGION(this);
  Procedure procedure = CodeRegion_procedure(codeRegion);
  const char *name = Procedure_name(procedure);
  int n_moves = 0;
  float weighted_moves = 0;
  int n_ops = 0;
  float n_weighted_ops = 0;
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    BasicBlock_FOREACH_Operation(block, operation) {
      n_ops++;
      n_weighted_ops += BasicBlock_frequency(block);
      if (Operator_isSeqCopy(Operation_operator(operation))) {
        n_moves++;
        weighted_moves += BasicBlock_frequency(block);
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;

  fprintf(file, "<ECL.PFA.SSAForm.stats.n_moves> %d %s\n", n_moves, name);
  fprintf(file, "<ECL.PFA.SSAForm.stats.weighted_moves> %f %s\n", weighted_moves, name);
  fprintf(file, "<ECL.PFA.SSAForm.stats.n_ops> %d %s\n", n_ops, name);
  fprintf(file, "<ECL.PFA.SSAForm.stats.weighted_ops> %f %s\n", n_weighted_ops, name);
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  Minir minir = Minir_new(Memory_Root, stdin);
  Program program;
  Minir_KEEP_INDEX(minir) = true;
  program = Minir_nextProgram(minir);
  {
    Procedure procedure = Program_findProcedure(program, "lost-copy");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    //SSAForm ssaForm = SSAForm_construct(Memory_Root, codeRegion, true);
    // cannot construct after all operations are parsed!
    SSAForm ssaForm = SSAForm_new(Program_memory(program), codeRegion);
    /*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    SSAForm_display(ssaForm, stdout, "Swap Problem");
    SSAForm_delete(ssaForm);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the SSAForm module.
 */
#if defined(SSAForm_MERGESETS) && defined(SSAForm_USELIVESETS) && defined(SSAForm_USELIVECHECK)
#define SSAForm_INIT()
#else
void
SSAForm_INIT(void);
#endif
#endif//$XCC__h

#ifndef SSAForm_INIT
void
SSAForm_INIT(void)
{
#ifndef SSAForm_MERGESETS
  if (GETENV("SSAForm_MERGESETS")) {
    sscanf(GETENV("SSAForm_MERGESETS"), "%d", &SSAForm_MERGESETS);
    fprintf(stderr, "*** SSAForm_MERGESETS=%d\n", SSAForm_MERGESETS);
  }
#endif//SSAForm_MERGESETS
#ifndef SSAForm_USELIVESETS
  if (GETENV("SSAForm_USELIVESETS")) {
    sscanf(GETENV("SSAForm_USELIVESETS"), "%d", &SSAForm_USELIVESETS);
    fprintf(stderr, "*** SSAForm_USELIVESETS=%d\n", SSAForm_USELIVESETS);
  }
#endif//SSAForm_USELIVESETS
#ifndef SSAForm_USELIVECHECK
  if (GETENV("SSAForm_USELIVECHECK")) {
    sscanf(GETENV("SSAForm_USELIVECHECK"), "%d", &SSAForm_USELIVECHECK);
    fprintf(stderr, "*** SSAForm_USELIVECHECK=%d\n", SSAForm_USELIVECHECK);
  }
#endif//SSAForm_USELIVECHECK
}
#endif//SSAForm_INIT

#ifdef $XCC__h
/**
 * Finalize the SSAForm module.
 */
#define SSAForm_FINI()
#endif//$XCC__h

#if XCC__C
static void
SSAForm_TEST(void)
{
#include "PFA/SSAForm_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(SSAForm_TEST);
  return 0;
}
#endif//XCC__C

