#ifdef $XCC_h
/*
 * !!!! Sreedhar.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Nikola Puzovic (Nikola.Puzovic@st.com).
 * Christophe Guillon (Christophe.Guillon@st.com).
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
 * @brief Sreedhar out-of-SSA algorithm.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/Sreedhar_.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * SSA Form destruction using Sreedhar et al. SAS'99 algorithms.
 */
struct Sreedhar_;
#endif//$XCC_h

#ifdef $XCC__h
#define Sreedhar_TIMER_T1 0
#define Sreedhar_TIMER_T2 1
#define Sreedhar_TIMER_T3 2
#define Sreedhar_TIMER_COUNT 3
#endif//$XCC__h

#ifdef $XCC__h
struct Sreedhar_ {
  //@args	Memory memory, CodeRegion codeRegion, Dominance dominance
  Coalescer_ COALESCER[1];		//!< SSA Coalescer.
  //@access MEMORY		Coalescer_MEMORY(Sreedhar_COALESCER(this))
  //@access CODEREGION		Coalescer_CODEREGION(Sreedhar_COALESCER(this))
  //@access TEMPORARYTABLE	Coalescer_TEMPORARYTABLE(Sreedhar_COALESCER(this))
  //@access CONGRUENCES		Coalescer_CONGRUENCES(Sreedhar_COALESCER(this))
  OperationStack_ PHIOPERATIONS[1];	//!< Phi-Operations to process.
  int32_t MAXPHIARITY;			//!< Maximum PHI arity (operand count).
  PtrSet_ *NEIGHBORSETS;		//!< The neighbor sets, one per PHI operand.
  PtrSet_ CANDIDATESET[1];		//!< The current candidate resource set.
  PtrSeq_ UNRESOLVED[1];		//!< Set of unresolved resources.
};
#endif//$XCC__h

Sreedhar
Sreedhar_Ctor(Sreedhar this,
              Memory memory, CodeRegion codeRegion, Dominance dominance)
{
  Optimize optimize = CodeRegion_optimize(codeRegion);
  OptimizeFlags coalescingFlags = Optimize_COALESCING(optimize);
  TemporaryTable temporaryTable = CodeRegion_globalTable(codeRegion);
  int32_t phiCount = TemporaryTable_count(temporaryTable);
  Coalescer coalescer = Sreedhar_COALESCER(this);
  Coalescer_Ctor(coalescer, memory, codeRegion, temporaryTable, dominance, coalescingFlags);
  if (Coalescer_CONSUMING(coalescer)) {
    CodeRegion_makeConsumers(codeRegion, temporaryTable);
  }
  OperationStack_Ctor(Sreedhar_PHIOPERATIONS(this), memory, phiCount);
  *Sreedhar__MAXPHIARITY(this) = Sreedhar_fillPhiOperations(this);
  PtrSet_Ctor(Sreedhar_CANDIDATESET(this), memory, 0);
  PtrSeq_Ctor(Sreedhar_UNRESOLVED(this), memory, Sreedhar_MAXPHIARITY(this));
  *Sreedhar__NEIGHBORSETS(this) = NULL;
  if (Sreedhar_MAXPHIARITY(this) > 0) {
    int32_t maxPhiArity = Sreedhar_MAXPHIARITY(this), i;
    size_t neighborSetsSize = sizeof(PtrSet_)*maxPhiArity;
    PtrSet_ *neighborSets = Memory_alloc(memory, neighborSetsSize);
    for (i = 0; i < maxPhiArity; i++) {
      PtrSet_Ctor(neighborSets + i, memory, 0);
    }
    *Sreedhar__NEIGHBORSETS(this) = neighborSets;
  }
  PFA_TRACE && fprintf(PFA_TRACE, "*** Before Sreedhar CSSA\n")
            && CodeRegion_pretty(codeRegion, PFA_TRACE)
            && SSAForm_pretty(CodeRegion_ssaForm(Sreedhar_CODEREGION(this)), PFA_TRACE);
  // Sreedhar I or III.
  Coalescer_complete(coalescer, false);
  {
    // Method III 2: for each phi operation.
    OperationStack phiOperations = Sreedhar_PHIOPERATIONS(this);
    OperationStack_FOREACH_Operation(phiOperations, operation) {
      Sreedhar_doMethod_III_3_4_5_6_7(this, operation, coalescingFlags);
    } OperationStack_ENDEACH_Operation;
    if (PFA_TRACE) {
      fprintf(PFA_TRACE, "*** SSA Congruences\n");
      TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
        Variable variable = Temporary_VARIABLE(temporary);
        VariableClass congruence = Variable_CONGRUENCE(variable);
        PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE)
                  && fprintf(PFA_TRACE, " => ")
                  && VariableClass_pretty(congruence, PFA_TRACE, false)
                  && fprintf(PFA_TRACE, "\n");
      } TemporaryTable_ENDEACH_Temporary;
    }
  }
  if (coalescingFlags & OptimizeCoalescing_Congruence) {
    Coalescer_coalesceCongruences(coalescer);
  }
  PFA_TRACE && (GETENV("SSA_TRACE"))
            && fprintf(PFA_TRACE, " *** After Sreedhar CSSA\n")
            && CodeRegion_pretty(codeRegion, PFA_TRACE);
  return this;
}

void
Sreedhar_Dtor(Sreedhar this)
{
  Memory memory = Sreedhar_MEMORY(this);
  Coalescer_Dtor(Sreedhar_COALESCER(this));
  OperationStack_Dtor(Sreedhar_PHIOPERATIONS(this));
  PtrSet_Dtor(Sreedhar_CANDIDATESET(this));
  PtrSeq_Dtor(Sreedhar_UNRESOLVED(this));
  if (Sreedhar_MAXPHIARITY(this) > 0) {
    int32_t maxPhiArity = Sreedhar_MAXPHIARITY(this), i;
    PtrSet_ *neighborSets = Sreedhar_NEIGHBORSETS(this);
    for (i = 0; i < maxPhiArity; i++) {
      PtrSet_Dtor(neighborSets + i);
    }
    Memory_free_(memory, neighborSets);
  }
}

size_t
Sreedhar_Size(Memory memory, CodeRegion codeRegion, Dominance dominance)
{
  return sizeof(Sreedhar_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Sreedhar_)\t%zu\n", sizeof(Sreedhar_));
}
#endif//$XCC__c

#ifdef $XCC_h
Sreedhar
Sreedhar_new(Memory parent, CodeRegion codeRegion, Dominance dominance);
#endif//$XCC_h

Sreedhar
Sreedhar_new(Memory parent, CodeRegion codeRegion, Dominance dominance)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Sreedhar_Size(memory, codeRegion, dominance);
  void *alloc = Memory_alloc(memory, size);
  return Sreedhar_Ctor(alloc, memory, codeRegion, dominance);
}

#ifdef $XCC_h
Sreedhar
Sreedhar_delete(Sreedhar this);
#endif//$XCC_h

Sreedhar
Sreedhar_delete(Sreedhar this)
{
  if (this != NULL) {
    Memory memory = Sreedhar_MEMORY(this);
    //#ifndef _NDTOR
    Sreedhar_Dtor(this);
    Memory_free_(memory, this);
    //#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
int32_t
Sreedhar_fillPhiOperations(Sreedhar this);
#endif//$XCC__h

int32_t
Sreedhar_fillPhiOperations(Sreedhar this)
{
  int32_t maxPhiArity = 0;
  CodeRegion codeRegion = Sreedhar_CODEREGION(this);
  TemporaryTable temporaryTable = Sreedhar_TEMPORARYTABLE(this);
  OperationStack phiOperations = Sreedhar_PHIOPERATIONS(this);
  // Fill the sortedBlocks.
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      if (Operator_isPhi(Operation_operator(operation))) {
        int32_t operandCount = Operation_operandCount(operation);
        if (maxPhiArity < operandCount) maxPhiArity = operandCount;
        if (!_OPTIMIZE) {
          Operation_OPERANDS_FOREACH_Temporary(operation, operand) {
            Except_ALWAYS(TemporaryTable_contains(temporaryTable, operand));
          } Operation_OPERANDS_ENDEACH_Temporary;
        }
        OperationStack_push(phiOperations, operation);
      } else break;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  if (PFA_TRACE) {
    BasicBlock prev_block = NULL;
    fprintf(PFA_TRACE, "*** PHI Operations\tmaxPhiArity=%d\n", maxPhiArity);
    OperationStack_FOREACH_Operation(phiOperations, operation) {
      BasicBlock basicBlock = Operation_basicBlock(operation);
      if (basicBlock != prev_block) {
        fprintf(PFA_TRACE, "    Block_%d\tfrequency=%g\n",
                BasicBlock_index(basicBlock), BasicBlock_frequency(basicBlock));
        prev_block = basicBlock;
      }
      Operation_pretty(operation, PFA_TRACE);
    } OperationStack_ENDEACH_Operation;
  }
  return maxPhiArity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Main processing of Sreedhar's Method III.
 */
void
Sreedhar_doMethod_III_3_4_5_6_7(Sreedhar this,
                                Operation phi_operation,
                                OptimizeFlags coalescingFlags);
#endif//$XCC__h

void
Sreedhar_doMethod_III_3_4_5_6_7(Sreedhar this,
                                Operation phi_operation,
                                OptimizeFlags coalescingFlags)
{
  Coalescer coalescer = Sreedhar_COALESCER(this);
  PtrSet candidateSet = Sreedhar_CANDIDATESET(this);
  PtrSet_ *neighborSets = Sreedhar_NEIGHBORSETS(this);
  PtrSet_empty(candidateSet);
  // Method III 3: empty the unresolved neighbor sets.
  Operation_OPERANDS_FOREACH_Temporary(phi_operation, operand) {
    Variable variable = Temporary_VARIABLE(operand);
    PtrSet neighbors = neighborSets + Operation_OPERANDS_INDEX;
    Variable_clearUnresolved(variable);
    *Variable__NEIGHBORS(variable) = neighbors;
    PtrSet_empty(neighbors);
  } Operation_OPERANDS_ENDEACH_Temporary;
  if (coalescingFlags & OptimizeCoalescing_Virtualize) {
    PFA_TRACE && fprintf(PFA_TRACE, "*** Sreedhar Method III:\n");
    // Method III 4: process interferences among phi_operation resources.
    Sreedhar_processInterferences(this, phi_operation);
    // Method III 5: process unresolved resources.
    Sreedhar_processUnresolvedResources(this, phi_operation);
  } else {
    PFA_TRACE && fprintf(PFA_TRACE, "*** Sreedhar Method I:\n");
    // Method I: put each resource in candidate resource set.
    Operation_OPERANDS_FOREACH_Temporary(phi_operation, x_i) {
      PtrSet_insert(candidateSet, Temporary_VARIABLE(x_i));
    } Operation_OPERANDS_ENDEACH_Temporary;
  }
  // Method III 6: insertCopy for each in candidate resource set.
  PFA_TRACE && fprintf(PFA_TRACE, "*** Sreedhar COPY insertion:\n");
  if (PFA_TRACE) {
    PtrSet_FORSORT(candidateSet, Variable_compare, Variable, variable_i) {
      Sreedhar_insertCopy(this, phi_operation, Variable_TEMPORARY(variable_i));
    } PtrSet_ENDSORT;
  } else {
    PtrSet_FOREACH(candidateSet, Variable, variable_i) {
      Sreedhar_insertCopy(this, phi_operation, Variable_TEMPORARY(variable_i));
    } PtrSet_ENDEACH;
  }
  // Method III 7: merge phi congruence classes of PHI resources.
  Coalescer_mergePhiCongruences(coalescer, phi_operation, NULL);
}

#ifdef $XCC__h
/*
 * Sreedhar method III.4 --	Check for interference between the members of congruence
 * classes of resources in a phi operation and break interferences between them.
 */
void
Sreedhar_processInterferences(Sreedhar this, Operation phi_operation);
#endif//$XCC__h

void
Sreedhar_processInterferences(Sreedhar this, Operation phi_operation)
{
  Coalescer coalescer = Sreedhar_COALESCER(this);
  BasicBlock phi_block = Operation_basicBlock(phi_operation);
  Temporary *phi_operands = Operation_operands(phi_operation);
  int32_t operandCount = Operation_operandCount(phi_operation);
  int32_t operandIndex_i = 0, operandIndex_j = 0;
  {
    Variable v_i = Temporary_VARIABLE(phi_operands[operandIndex_i]);
    VariableClass congruence_i = Variable_CONGRUENCE(v_i);
    for (operandIndex_j = operandIndex_i + 1; operandIndex_j < operandCount; operandIndex_j++) {
      Variable v_j = Temporary_VARIABLE(phi_operands[operandIndex_j]);
      VariableClass congruence_j = Variable_CONGRUENCE(v_j);
      if (congruence_i != congruence_j) {
        if (Coalescer_interferesCC(coalescer, congruence_i, congruence_j)) {
          BasicBlockEdge edge_j = BasicBlock_getPhiEnterEdge(phi_block, operandIndex_j - 1);
          BasicBlock block_j = BasicBlockEdge_origBlock(edge_j);
          Sreedhar_breakInterferencesInOut(this, v_i, v_j, phi_block, block_j);
        }
      }
    }
  }
  for (operandIndex_i = 1; operandIndex_i < operandCount; operandIndex_i++) {
    Variable v_i = Temporary_VARIABLE(phi_operands[operandIndex_i]);
    VariableClass congruence_i = Variable_CONGRUENCE(v_i);
    BasicBlockEdge edge_i = BasicBlock_getPhiEnterEdge(phi_block, operandIndex_i - 1);
    BasicBlock block_i = BasicBlockEdge_origBlock(edge_i);
    for (operandIndex_j = operandIndex_i + 1; operandIndex_j < operandCount; operandIndex_j++) {
      Variable v_j = Temporary_VARIABLE(phi_operands[operandIndex_j]);
      VariableClass congruence_j = Variable_CONGRUENCE(v_j);
      if (congruence_i != congruence_j) {
        if (Coalescer_interferesCC(coalescer, congruence_i, congruence_j)) {
          BasicBlockEdge edge_j = BasicBlock_getPhiEnterEdge(phi_block, operandIndex_j - 1);
          BasicBlock block_j = BasicBlockEdge_origBlock(edge_j);
          Sreedhar_breakInterferencesOutOut(this, v_i, v_j, block_i, block_j);
        }
      }
    }
  }
}

#ifdef $XCC__h
void
Sreedhar_breakInterferencesInOut(Sreedhar this, Variable v_i, Variable v_j, BasicBlock block_i, BasicBlock block_j);
#endif//$XCC__h

void
Sreedhar_breakInterferencesInOut(Sreedhar this, Variable v_i, Variable v_j, BasicBlock block_i, BasicBlock block_j)
{
  Coalescer coalescer = Sreedhar_COALESCER(this);
  VariableClass congruence_i = Variable_CONGRUENCE(v_i);
  VariableClass congruence_j = Variable_CONGRUENCE(v_j);
  bool congruence_i_live_j = false, congruence_j_live_i = false;
  if (Coalescer_USELIVESETS==1) {
    int32_t index_i = BasicBlock_index(block_i);
    int32_t index_j = BasicBlock_index(block_j);
    TemporarySet_ *liveInSets = Coalescer_LIVEINSETS(coalescer);
    TemporarySet_ *liveOutSets = Coalescer_LIVEOUTSETS(coalescer);
    TemporarySet_ *phiUseSets = Coalescer_PHIUSESETS(coalescer);
    TemporarySet_ *phiDefSets = Coalescer_PHIDEFSETS(coalescer);
    VariableClass_FOREACH_Variable(congruence_i, variable) {
      Temporary temporary = Variable_TEMPORARY(variable);
      if (   TemporarySet_contains(phiUseSets + index_j, temporary)
          || TemporarySet_contains(liveOutSets + index_j, temporary)) {
        congruence_i_live_j = true;
        break;
      }
    } VariableClass_ENDEACH_Variable;
    VariableClass_FOREACH_Variable(congruence_j, variable) {
      Temporary temporary = Variable_TEMPORARY(variable);
      if (   TemporarySet_contains(phiDefSets + index_i, temporary)
          || TemporarySet_contains(liveInSets + index_i, temporary)) {
        congruence_j_live_i = true;
        break;
      }
    } VariableClass_ENDEACH_Variable;
  } else {
    LiveCheck liveCheck = Coalescer_LIVECHECK(coalescer);
    VariableClass_FOREACH_Variable(congruence_i, variable) {
      if (LiveCheck_isLiveOutBasicBlock(liveCheck, block_j, variable, LivenessMode_Mplex)) {
        congruence_i_live_j = true;
        break;
      }
    } VariableClass_ENDEACH_Variable;
    VariableClass_FOREACH_Variable(congruence_j, variable) {
      if (LiveCheck_isLiveInBasicBlock(liveCheck, block_i, variable, LivenessMode_Mplex)) {
        congruence_j_live_i = true;
        break;
      }
    } VariableClass_ENDEACH_Variable;
  }
  Sreedhar_breakInterferences(this, v_i, v_j, congruence_i_live_j, congruence_j_live_i);
}

#ifdef $XCC__h
void
Sreedhar_breakInterferencesOutOut(Sreedhar this, Variable v_i, Variable v_j, BasicBlock block_i, BasicBlock block_j);
#endif//$XCC__h

void
Sreedhar_breakInterferencesOutOut(Sreedhar this, Variable v_i, Variable v_j, BasicBlock block_i, BasicBlock block_j)
{
  Coalescer coalescer = Sreedhar_COALESCER(this);
  VariableClass congruence_i = Variable_CONGRUENCE(v_i);
  VariableClass congruence_j = Variable_CONGRUENCE(v_j);
  bool congruence_i_live_j = false, congruence_j_live_i = false;
  if (Coalescer_USELIVESETS==1) {
    int32_t index_i = BasicBlock_index(block_i);
    int32_t index_j = BasicBlock_index(block_j);
    TemporarySet_ *liveOutSets = Coalescer_LIVEOUTSETS(coalescer);
    TemporarySet_ *phiUseSets = Coalescer_PHIUSESETS(coalescer);
    VariableClass_FOREACH_Variable(congruence_i, variable) {
      Temporary temporary = Variable_TEMPORARY(variable);
      if (   TemporarySet_contains(phiUseSets + index_j, temporary)
          || TemporarySet_contains(liveOutSets + index_j, temporary)) {
        congruence_i_live_j = true;
        break;
      }
    } VariableClass_ENDEACH_Variable;
    VariableClass_FOREACH_Variable(congruence_j, variable) {
      Temporary temporary = Variable_TEMPORARY(variable);
      if (   TemporarySet_contains(phiUseSets + index_i, temporary)
          || TemporarySet_contains(liveOutSets + index_i, temporary)) {
        congruence_j_live_i = true;
        break;
      }
    } VariableClass_ENDEACH_Variable;
  } else {
    LiveCheck liveCheck = Coalescer_LIVECHECK(coalescer);
    VariableClass_FOREACH_Variable(congruence_i, variable) {
      if (LiveCheck_isLiveOutBasicBlock(liveCheck, block_j, variable, LivenessMode_Mplex)) {
        congruence_i_live_j = true;
        break;
      }
    } VariableClass_ENDEACH_Variable;
    VariableClass_FOREACH_Variable(congruence_j, variable) {
      if (LiveCheck_isLiveOutBasicBlock(liveCheck, block_i, variable, LivenessMode_Mplex)) {
        congruence_j_live_i = true;
        break;
      }
    } VariableClass_ENDEACH_Variable;
  }
  Sreedhar_breakInterferences(this, v_i, v_j, congruence_i_live_j, congruence_j_live_i);
}

#ifdef $XCC__h
void
Sreedhar_breakInterferences(Sreedhar this, Variable v_i, Variable v_j,
                            bool congruence_i_live_j, bool congruence_j_live_i);
#endif//$XCC__h

void
Sreedhar_breakInterferences(Sreedhar this, Variable v_i, Variable v_j,
                            bool congruence_i_live_j, bool congruence_j_live_i)
{
  PtrSet candidateSet = Sreedhar_CANDIDATESET(this);
  PFA_TRACE && fprintf(PFA_TRACE, " -- Breaking interference between\t")
            && Temporary_pretty(Variable_TEMPORARY(v_i), PFA_TRACE)
            && fprintf(PFA_TRACE, "  and  ")
            && Temporary_pretty(Variable_TEMPORARY(v_j), PFA_TRACE)
            && fprintf(PFA_TRACE, "\n");
  if (congruence_i_live_j && !congruence_j_live_i) {
    // Case 1:    congruenceClass[xi] * LiveOutSet[Lj] != {}
    //        and LiveOutSet[Li] * congruenceClass[xj] == {}
    PtrSet_insert(candidateSet, v_i);
    PFA_TRACE && fprintf(PFA_TRACE, "*** Case 1: candidate\t")
              && Temporary_pretty(Variable_TEMPORARY(v_i), PFA_TRACE)
              && fprintf(PFA_TRACE, "\n");
  } else
  if (!congruence_i_live_j && congruence_j_live_i) {
    // Case 2:    congruenceClass[xi] * LiveOutSet[Lj] == {}
    //        and LiveOutSet[Li] * congruenceClass[xj] != {}
    PtrSet_insert(candidateSet, v_j);
    PFA_TRACE && fprintf(PFA_TRACE, "*** Case 2: candidate\t")
              && Temporary_pretty(Variable_TEMPORARY(v_j), PFA_TRACE)
              && fprintf(PFA_TRACE, "\n");
  } else
  if (congruence_i_live_j && congruence_j_live_i) {
    // Case 3:    congruenceClass[xi] * LiveOutSet[Lj] != {}
    //        and LiveOutSet[Li] * congruenceClass[xj] != {}
    PtrSet_insert(candidateSet, v_i);
    PtrSet_insert(candidateSet, v_j);
    PFA_TRACE && fprintf(PFA_TRACE, "*** Case 3: candidates\t")
              && Temporary_pretty(Variable_TEMPORARY(v_i), PFA_TRACE)
              && fprintf(PFA_TRACE, "\t")
              && Temporary_pretty(Variable_TEMPORARY(v_j), PFA_TRACE)
              && fprintf(PFA_TRACE, "\n");
  } else
  /* (!congruence_i_live_j && !congruence_j_live_i) */ {
    // Case 4:    congruenceClass[xi] * LiveOutSet[Lj] == {}
    //        and LiveOutSet[Li] * congruenceClass[xj] == {}
    PtrSet_insert(Variable_NEIGHBORS(v_i), v_j);
    PtrSet_insert(Variable_NEIGHBORS(v_j), v_i);
    PFA_TRACE && fprintf(PFA_TRACE, "*** Case 4: unresolved\t")
              && Temporary_pretty(Variable_TEMPORARY(v_i), PFA_TRACE)
              && fprintf(PFA_TRACE, "\t")
              && Temporary_pretty(Variable_TEMPORARY(v_j), PFA_TRACE)
              && fprintf(PFA_TRACE, "\n");
  }
}

#ifdef $XCC__h
/**
 * Inserts copies that are needed for conventional SSA.
 */
void
Sreedhar_insertCopy(Sreedhar this, Operation phi_operation, Temporary resource_i);
#endif//$XCC__h

void
Sreedhar_insertCopy(Sreedhar this, Operation phi_operation, Temporary resource_i)
{
  Coalescer coalescer = Sreedhar_COALESCER(this);
  IBList congruences = Sreedhar_CONGRUENCES(this);
  CodeRegion codeRegion = Sreedhar_CODEREGION(this);
  TemporaryTable temporaryTable = Sreedhar_TEMPORARYTABLE(this);
  BasicBlock block_0 = Operation_basicBlock(phi_operation);
  Temporary resource_0 = Operation_getResult(phi_operation, 0);
  Variable variable_i = Temporary_VARIABLE(resource_i);
  Temporary origin = Variable_ORIGIN(variable_i);
  PFA_TRACE && fprintf(PFA_TRACE, "\tinsert COPY for ")
            && Temporary_pretty(resource_i, PFA_TRACE)
            && fprintf(PFA_TRACE, "\n");
  Operation_ARGUMENTS_FOREACH_Temporary(phi_operation, argument) {
    if (argument == resource_i) {
      int resIndex = 0;
      Operation copy_operation = NULL;
      BasicBlockEdge edge_k = BasicBlock_getPhiEnterEdge(block_0, Operation_ARGUMENTS_INDEX);
      BasicBlock block_k = BasicBlockEdge_origBlock(edge_k);
      Temporary new_resource_i = CodeRegion_makeVariableTemporary(codeRegion, origin);
      Variable new_variable_i = Temporary_VARIABLE(new_resource_i);
      VariableClass congruence = IBList_push(congruences);
      VariableClass_Ctor(congruence, new_variable_i);
      TemporaryTable_insert(temporaryTable, new_resource_i);
      // 1. Insert a copy x_newi = xi in the corresponding block.
      copy_operation = BasicBlock_makeLateOperationCOPY(block_k, new_resource_i, resource_i);
      Variable_setOperationResult(new_variable_i, copy_operation, resIndex);
      if (Coalescer_CONSUMING(coalescer)) Variable_raiseConsuming(new_variable_i);
      Variable_setOperationArgument(variable_i, copy_operation, resIndex);
      // 2. Replace xi with x_newi.
      Variable_setOperationArgument(new_variable_i, phi_operation, Operation_ARGUMENTS_INDEX);
      // 3. Add x_newi in congruenceClass of x_newi.
      // if (for Lj an immediate successor of Lk, xi not in LiveInSet[Lj] and
      //     not used in a phi instruction associated with Lk in Lj)
      //        LiveOutSet[Lk] -= xi
      // Corrected test as: (xi not a PHI target in Lj)
      /* FAB: we could avoid doing this test by simply
         duplicating variables in phiOutClass and removing them one by one */
      if (Coalescer_USELIVESETS>0) {
        TemporarySet liveOutSet_k = Coalescer_LIVEOUTSETS(coalescer) + BasicBlock_index(block_k);
        TemporarySet phiUseSet_k = Coalescer_PHIUSESETS(coalescer) + BasicBlock_index(block_k);
        if (!Sreedhar_isNeededPhiOutClass(this, block_k, resource_i)) {
          TemporarySet_remove(phiUseSet_k, resource_i);
        }
        // Add interference edges between x_newi and LiveOutSet[Lk].
        /* Fixme: addInterference with the branch bool */
        Coalescer_addInterferenceVariableLiveOut(coalescer, new_variable_i, block_k);
        // 4. LiveOutSet[Lk] += x_newi.
        TemporarySet_insert(phiUseSet_k, new_resource_i);
      } else {
        // Add interference edges between x_newi and LiveOutSet[Lk].
        /* Fixme: addInterference with the branch bool */
        Coalescer_addInterferenceVariableLiveOut(coalescer, new_variable_i, block_k);
      }
    }
  } Operation_ARGUMENTS_ENDEACH_Temporary;
  if (resource_i == resource_0) {
    int resIndex = 0;
    Operation copy_operation = NULL;
    Variable variable_0 = Temporary_VARIABLE(resource_0);
    Temporary new_resource_0 = CodeRegion_makeVariableTemporary(codeRegion, origin);
    Variable new_variable_0 = Temporary_VARIABLE(new_resource_0);
    VariableClass congruence = IBList_push(congruences);
    VariableClass_Ctor(congruence, new_variable_0);
    TemporaryTable_insert(temporaryTable, new_resource_0);
    // Insert a copy x0 = xnew_0 in the PHI block.
    copy_operation = BasicBlock_makeEarlyOperationCOPY(block_0, resource_0, new_resource_0);
    // Replace x0 with xnew_0 as the target.
    Variable_setOperationResult(variable_0, copy_operation, resIndex);
    Variable_updateCongruence(variable_0);
    Variable_setOperationArgument(new_variable_0, copy_operation, resIndex);
    Variable_setOperationResult(new_variable_0, phi_operation, 0);
    if (Coalescer_CONSUMING(coalescer)) Variable_raiseConsuming(new_variable_0);
    // LiveInSet[L0] -= x0.
    if (Coalescer_USELIVESETS>0) {
      TemporarySet liveInSet_0 = Coalescer_LIVEINSETS(coalescer) + BasicBlock_index(block_0);
      TemporarySet phiDefSet_0 = Coalescer_PHIDEFSETS(coalescer) + BasicBlock_index(block_0);
      TemporarySet_remove(phiDefSet_0, resource_0);
      // Add interference edges between xnew_0 and LiveInSet[L0].
      /* FAB: here Sreedhar is too conservative. It should make variable_0
         interfere with livein and with variables used in the parallel copy */
      Coalescer_addInterferenceVariableLiveIn(coalescer, new_variable_0, block_0);
      // LiveInSet[L0] += xnew_0.
      TemporarySet_insert(phiDefSet_0, new_resource_0);
    } else {
      // Add interference edges between xnew_0 and LiveInSet[L0].
      /* FAB: here Sreedhar is too conservative. It should make variable_0
         interfere with livein and with variables used in the parallel copy */
      Coalescer_addInterferenceVariableLiveIn(coalescer, new_variable_0, block_0);
    }
  }
}

#ifdef $XCC__h
/**
 * Check if a Temporary is needed in BasicBlock phi-out.
 */
bool
Sreedhar_isNeededPhiOutClass(Sreedhar this, BasicBlock basicBlock, Temporary temporary);
#endif//$XCC__h

bool
Sreedhar_isNeededPhiOutClass(Sreedhar this, BasicBlock basicBlock, Temporary temporary)
{
  // Sreedhar version: Not needed if
  // if (for Lj an immediate successor of Lk, xi not in LiveInSet[Lj] and
  //     not used in a phi instruction associated with Lk in Lj)
  //        LiveOutSet[Lk] -= xi
  // Corrected test as: (xi not in LiveInSet[Lj] or is a PHI target in Lj) and ...
  // Note that the difference is on the interpretation of livein
  // in the case where livein contains the PHI targets as it is the case,
  // we must indeed remove the PHI target from the check.
  // FAB: Corrected again as:
  //   if (for Lj an immediate successor of Lk xi not used
  //       in a phi instruction associated with Lk in Lj)
  //   then PhiOutSet -= xi
  // note that this test is not necessary for phiinset. Indeed a definition
  //   appears only once while a use may appears several times.
  // For all the basicBlock successors.
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(basicBlock, edge) {
    BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
    bool isPhiResult = false;
    // Forall the PHI Operation(s) of successor BasicBlock.
    BasicBlock_FOREACH_Operation(succ_block, operation) {
      if (Operator_isPhi(Operation_operator(operation))) {
        // Needed if used as PHI argument of the successor block associated to basicBlock.
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (argument == temporary) {
            BasicBlockEdge edge = BasicBlock_getPhiEnterEdge(succ_block,
                                                             Operation_ARGUMENTS_INDEX);
            if (BasicBlockEdge_origBlock(edge) == basicBlock) return true;
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
        if (Operation_getResult(operation, 0) == temporary) isPhiResult = true;
      } else break;
    } BasicBlock_ENDEACH_Operation;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  return false;
}

#ifdef $XCC__h
/*
 * Sreedhar method III.5 --	Processes unresolved interferences among the neighbors.
 *
 * Problem is as follow: The NEIGHBORS represent an interference graph between variables
 * where an independent set is required. We want to maximize the size of the independent
 * set, since each vertex (interfering variable) not part of the independent set is put
 * into the candidateSet and this produces a COPY operation in the final code. Maximum
 * independent set is NP-complete, so we use an heuristic.
 *
 * Sreedhar's proposed heuristic is to remove largest degrees first ('mark as resolved')
 * until independent set is obtained, thent then re-insert the removed vertices that have
 * no neighbors in the independent set.
 *
 * We use instead the classic independet set heuristic, that is: find the lowest-degree
 * vertex, add it to the independent set, and delete it and all vertices adjacent to it;
 * repeat this process until the graph is empty. -- BD3
 */
void
Sreedhar_processUnresolvedResources(Sreedhar this, Operation phi_operation);
#endif//$XCC__h

void
Sreedhar_processUnresolvedResources(Sreedhar this, Operation phi_operation)
{
  PtrSet candidateSet = Sreedhar_CANDIDATESET(this);
  PtrSeq unresolved = Sreedhar_UNRESOLVED(this);
  PFA_TRACE && fprintf(PFA_TRACE, " === Processing unresolved resources === \n");
  // Fill the unresolved resources set.
  Operation_OPERANDS_FOREACH_Temporary(phi_operation, resource) {
    Variable variable = Temporary_VARIABLE(resource);
    // Do not process a resource already in candidateSet (fix to Sreedhar).
    if (!PtrSet_contains(candidateSet, variable)) {
      PtrSet neighbors = Variable_NEIGHBORS(variable);
      // Filter-out candidateSet from neighbors (fix to Sreedhar).
      PtrSet_diff(neighbors, candidateSet);
      if (PtrSet_count(neighbors) > 0) {
        if (!Variable_isUnresolved(variable)) {
          Variable_raiseUnresolved(variable);
          PtrSeq_push2(unresolved, variable);
          PFA_TRACE && fprintf(PFA_TRACE, "\tunresolved ")
                    && Temporary_pretty(resource, PFA_TRACE)
                    && fprintf(PFA_TRACE, "\tneighbors ")
                    && Variable_prettyNeighbors(variable, PFA_TRACE, false)
                    && fprintf(PFA_TRACE, "\n");
        }
      }
    }
  } Operation_OPERANDS_ENDEACH_Temporary;
  // Process unresolved resources in non-decreasing NEIGHBORS size.
  while (!PtrSeq_isEmpty(unresolved)) {
    Variable variable = (Variable)PtrSeq_top(unresolved);
    // Extract the resource with the minimal NEIGHBORS size from unresolved.
    PtrSeq_pop(unresolved);
    if (Variable_isUnresolved(variable)) {
      PtrSet neighbors = Variable_NEIGHBORS(variable);
      int32_t degree = PtrSet_count(neighbors);
      Variable *unresolved_base = (Variable *)PtrSeq_base_(unresolved);
      int32_t unresolved_count = PtrSeq_count(unresolved), index;
      for (index = 0; index < unresolved_count; index++) {
        Variable *_vertex = unresolved_base + index;
        Variable vertex = *_vertex;
        if (Variable_isUnresolved(vertex)) {
          PtrSet vertex_neighbors = Variable_NEIGHBORS(vertex);
          int32_t vertex_degree = PtrSet_count(vertex_neighbors);
          if (degree > vertex_degree) {
            *_vertex = variable;
            variable = vertex;
            neighbors = vertex_neighbors;
            degree = vertex_degree;
          }
        }
      }
      Except_CHECK(Variable_isUnresolved(variable));
      Variable_clearUnresolved(variable);
      PFA_TRACE && fprintf(PFA_TRACE, "\tindependent ")
                && Temporary_pretty(Variable_TEMPORARY(variable), PFA_TRACE)
                && fprintf(PFA_TRACE, "\tneighbors ")
                && Variable_prettyNeighbors(variable, PFA_TRACE, false)
                && fprintf(PFA_TRACE, "\n");
      // The resource goes to the independent set and its neighbors to the candidateSet.
      PtrSet_FOREACH(neighbors, Variable, vertex) {
        PtrSet vertex_neighbors = Variable_NEIGHBORS(vertex);
        // Add neighbor vertex to the candidateSet and remove it from unresolved.
        PtrSet_insert(candidateSet, vertex);
        Variable_clearUnresolved(vertex);
        // Remove vertex from the NEIGHBORS of its neighbors.
        PtrSet_FOREACH(vertex_neighbors, Variable, neighbor) {
          PtrSet neighbor_neighbors = Variable_NEIGHBORS(neighbor);
          PtrSet_remove(neighbor_neighbors, vertex);
        } PtrSet_ENDEACH;
      } PtrSet_ENDEACH;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
// Minir initialization for tests
{
  Minir minir = Minir_new(Memory_Root, stdin);
  Program program;
  Minir_KEEP_INDEX(minir) = true;
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
  SSAForm_constructAfterMinir(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "sreedhar-fig-1");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    SSAForm ssaForm = CodeRegion_ssaForm(codeRegion);
    /*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    PFA_TRACE = stdout;
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Sreedhar;
    SSAForm_destruct(ssaForm);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
  SSAForm_constructAfterMinir(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "sreedhar-fig-3");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    SSAForm ssaForm = CodeRegion_ssaForm(codeRegion);
/*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    PFA_TRACE = stdout;
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Sreedhar;
    SSAForm_destruct(ssaForm);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
  SSAForm_constructAfterMinir(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "sreedhar-fig-6");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    SSAForm ssaForm = CodeRegion_ssaForm(codeRegion);
    /*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    PFA_TRACE = stdout;
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Sreedhar;
    SSAForm_destruct(ssaForm);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "lost-copy");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    SSAForm ssaForm;
    *Optimize__CONVERSION(optimize) = OptimizeConversion_Folding;
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Sreedhar;
    ssaForm = SSAForm_construct(Memory_Root, codeRegion, true);
    /*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    PFA_TRACE = stdout;
    SSAForm_destruct(ssaForm);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "swap-problem");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    SSAForm ssaForm;
    *Optimize__CONVERSION(optimize) = OptimizeConversion_Folding;
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Sreedhar;
    ssaForm = SSAForm_construct(Memory_Root, codeRegion, true);
    /*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    PFA_TRACE = stdout;
    SSAForm_destruct(ssaForm);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "virtual-swap-problem");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    SSAForm ssaForm;
    *Optimize__CONVERSION(optimize) = OptimizeConversion_Folding;
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Sreedhar;
    ssaForm = SSAForm_construct(Memory_Root, codeRegion, true);
    /*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    PFA_TRACE = stdout;
    SSAForm_destruct(ssaForm);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "weird-case");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    SSAForm ssaForm;
    *Optimize__CONVERSION(optimize) = OptimizeConversion_Folding;
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Sreedhar;
    ssaForm = SSAForm_construct(Memory_Root, codeRegion, true);
    /*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    PFA_TRACE = stdout;
    SSAForm_destruct(ssaForm);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
  SSAForm_constructAfterMinir(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "ssa-coalescing");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    SSAForm ssaForm = CodeRegion_ssaForm(codeRegion);
    /*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    PFA_TRACE = stdout;
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Sreedhar;
    *Optimize__COALESCING(optimize) |= OptimizeCoalescing_Congruence;
    SSAForm_destruct(ssaForm);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  // Test the SSA-based coalescing.
  // Figure 10 in Rastello's report.
  // FIXME! Output is unexpected! (Flo: really?)
  program = Minir_nextProgram(minir);
  SSAForm_constructAfterMinir(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "rastello-test-coalescing");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    SSAForm ssaForm = CodeRegion_ssaForm(codeRegion);
    /*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    PFA_TRACE = stdout;
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Sreedhar;
    *Optimize__COALESCING(optimize) |= OptimizeCoalescing_Congruence;
    SSAForm_destruct(ssaForm);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
// Delete Minir constructs
  Minir_delete(minir);
}
#endif//$XCC__c


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Sreedhar module.
 */
#define Sreedhar_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Sreedhar module.
 */
#define Sreedhar_FINI()
#endif//$XCC__h

#if XCC__C
static void
Sreedhar_TEST(void)
{
#include "PFA/Sreedhar_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(Sreedhar_TEST);
  return 0;
}
#endif

