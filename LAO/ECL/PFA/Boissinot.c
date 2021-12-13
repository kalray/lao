#ifdef $XCC_h
/*
 * !!!! Boissinot.xcc
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
 * @brief Boissinot out-of-SSA algorithm.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/Boissinot_.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Boissinot --       SSA Form destruction using Boissinot et al. algorithms.
 */
struct Boissinot_;
#endif//$XCC_h

#ifdef $XCC__h
#define Boissinot_TIMER_T1 0
#define Boissinot_TIMER_T2 1
#define Boissinot_TIMER_T3 2
#define Boissinot_TIMER_COUNT 3
#endif//$XCC__h

#ifdef $XCC__h
struct Boissinot_ {
  //@args       Memory memory, CodeRegion codeRegion, Dominance dominance
  Coalescer_ COALESCER_;     		// SSA Coalescer.
  //@access COALESCER Boissinot__COALESCER_(this)
  //@access CODEREGION		Coalescer_CODEREGION(Boissinot_COALESCER(this))
  //@access TEMPORARYTABLE	Coalescer_TEMPORARYTABLE(Boissinot_COALESCER(this))
  //@access CONGRUENCES		Coalescer_CONGRUENCES(Boissinot_COALESCER(this))
  OperationStack_ PHIOPERATIONS_;	// Phi-Operations to process.
  //@access PHIOPERATIONS	Boissinot__PHIOPERATIONS_(this)
  //@access MEMORY      OperationStack_memory(Boissinot_PHIOPERATIONS(this))
};
#endif//$XCC__h

Boissinot
Boissinot_Ctor(Boissinot this,
               Memory memory, CodeRegion codeRegion, Dominance dominance)
{
  Optimize optimize = CodeRegion_optimize(codeRegion);
  OptimizeFlags coalescingFlags = Optimize_COALESCING(optimize);
  TemporaryTable temporaryTable = CodeRegion_globalTable(codeRegion);
  int32_t phiCount = TemporaryTable_count(temporaryTable);
  Coalescer coalescer = Boissinot_COALESCER(this);
  Coalescer_Ctor(coalescer, memory, codeRegion, temporaryTable, dominance, coalescingFlags);
  if (Coalescer_CONSUMING(coalescer)) {
    CodeRegion_makeConsumers(codeRegion, temporaryTable);
  }
  OperationStack_Ctor(Boissinot_PHIOPERATIONS(this), memory, phiCount);
  Boissinot_fillPhiOperations(this);
  PFA_TRACE && fprintf(PFA_TRACE, "*** Before Boissinot CSSA\n")
            && CodeRegion_pretty(codeRegion, PFA_TRACE)
            && SSAForm_pretty(CodeRegion_ssaForm(Boissinot_CODEREGION(this)), PFA_TRACE);
  Except_CHECK(coalescingFlags & OptimizeCoalescing_Boissinot);
  if (coalescingFlags & OptimizeCoalescing_Virtualize) {
    Boissinot_doBoissinot_III_Coalescer(this);
  } else {
    Boissinot_doBoissinot_I_Coalescer(this);
  }
  if (coalescingFlags & OptimizeCoalescing_Congruence) {
    Coalescer_coalesceCongruences(coalescer);
  }
  PFA_TRACE && (GETENV("SSA_TRACE"))
            && fprintf(PFA_TRACE, " *** After Boissinot CSSA\n")
            && CodeRegion_pretty(codeRegion, PFA_TRACE);
  return this;
}

void
Boissinot_Dtor(Boissinot this)
{
  Memory memory = Boissinot_MEMORY(this);
  Coalescer_Dtor(Boissinot_COALESCER(this));
  OperationStack_Dtor(Boissinot_PHIOPERATIONS(this));
}

size_t
Boissinot_Size(Memory memory, CodeRegion codeRegion, Dominance dominance)
{
  return sizeof(Boissinot_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Boissinot_) = %zu\n", sizeof(Boissinot_));
}
#endif//$XCC__c

#ifdef $XCC_h
Boissinot
Boissinot_new(Memory parent, CodeRegion codeRegion, Dominance dominance);
#endif//$XCC_h

Boissinot
Boissinot_new(Memory parent, CodeRegion codeRegion, Dominance dominance)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Boissinot_Size(memory, codeRegion, dominance);
  void *alloc = Memory_alloc(memory, size);
  return Boissinot_Ctor(alloc, memory, codeRegion, dominance);
}

#ifdef $XCC_h
Boissinot
Boissinot_delete(Boissinot this);
#endif//$XCC_h

Boissinot
Boissinot_delete(Boissinot this)
{
  if (this != NULL) {
    Memory memory = Boissinot_MEMORY(this);
    //#ifndef _NDTOR
    Boissinot_Dtor(this);
    Memory_free_(memory, this);
    //#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
int32_t
Boissinot_fillPhiOperations(Boissinot this);
#endif//$XCC__h

int32_t
Boissinot_fillPhiOperations(Boissinot this)
{
  int32_t maxPhiArity = 0;
  CodeRegion codeRegion = Boissinot_CODEREGION(this);
  TemporaryTable temporaryTable = Boissinot_TEMPORARYTABLE(this);
  OperationStack phiOperations = Boissinot_PHIOPERATIONS(this);
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

typedef struct PhiBlocks {
  BasicBlock basicBlock;
  int32_t i;
} PhiBlocks;

static inline int
comparePhiBlocks(const void *a, const void *b) {
  uint32_t freq_a = BasicBlock_frequency(((PhiBlocks *) a)->basicBlock);
  uint32_t freq_b = BasicBlock_frequency(((PhiBlocks *) b)->basicBlock);
  return (freq_a > freq_b) - (freq_a < freq_b);
}

#ifdef $XCC_h
/**
 * Exact out of SSA method with phi virtualization
 */
void
Boissinot_doBoissinot_III(Boissinot this);
/* Require:  Interference free CONGRUENCES
 * Output: Generated move operations around PHIs
 *         All newly created variables put in the same phi congruence class
 */
#endif//$XCC_h

void
Boissinot_doBoissinot_III(Boissinot this)
{
  CodeRegion codeRegion = Boissinot_CODEREGION(this);
  Memory memory = Boissinot_MEMORY(this);
  Coalescer coalescer = Boissinot_COALESCER(this);
  IBList congruences = Boissinot_CONGRUENCES(this);
  //
  CodeRegion_makeConsumers(codeRegion, Boissinot_TEMPORARYTABLE(this));
  //foreach phi: B0:a0=phi(B1:a1,...,Bn:an)
  CodeRegion_FOREACH_BasicBlock(codeRegion, B0) {
    1 && PFA_TRACE && fprintf(PFA_TRACE, "Boissinot_III treating BB %d\n", BasicBlock_INDEX(B0));
    BasicBlock_FOREACH_Operation(B0, phi) {
      if (!Operator_isPhi(Operation_operator(phi))) break;
      1 && PFA_TRACE && fprintf(PFA_TRACE, "\tBoissinot_III treating ")
              && Operation_pretty(phi, PFA_TRACE) && fprintf(PFA_TRACE,"\n");
      //congruence class of the phi variable (variables merged with the phi) initially empty
      Except_CHECK(Operation_PHICLASS(phi)==NULL);
      //foreach Bi in order of decreasing frequency
      {
        int32_t n=Operation_operandCount(phi);
        int32_t i, k;
        PhiBlocks BBList[n];
        BBList[0].basicBlock=B0;
        BBList[0].i=0;
        for (i=1 ; i<n ; i++) {
          BBList[i].basicBlock=BasicBlockEdge_origBlock(BasicBlock_getPhiEnterEdge(B0, i-1));
          BBList[i].i=i;
        }
        qsort(BBList, n, sizeof(PhiBlocks), comparePhiBlocks);
        for (k=n-1 ; k>=0 ; k--) {
          bool merge = true;
          int32_t i = BBList[k].i;
          //let ai be the i-th argument
          Variable ai = Temporary_VARIABLE(Operation_OPERANDS(phi)[i]);
          VariableClass congruence_ai = Variable_CONGRUENCE(ai);
          PFA_TRACE && fprintf(PFA_TRACE,"\t\ttreating argument %d",i)
                    && fprintf(PFA_TRACE,", ai=") && Variable_pretty(ai, PFA_TRACE, 1)
                    && fprintf(PFA_TRACE,", Ai=") &&
                    VariableClass_pretty(Variable_CONGRUENCE(ai), PFA_TRACE, false)
                    && Operation_PHICLASS(phi) && fprintf(PFA_TRACE,", PHIVAR=")
                    && VariableClass_pretty(Operation_PHICLASS(phi), PFA_TRACE, false)
                    && fprintf(PFA_TRACE, "\n");
          //if class(ai) interfere phi
          //then goto split
          if (Coalescer_interferesCP(coalescer, congruence_ai, phi)) {
            PFA_TRACE && fprintf(PFA_TRACE, "\t\t\tAi interferes with phi!\n");
            merge = false;
          }
          //if class(ai) interfere phimerged
          //then goto split
          if (   merge && Operation_PHICLASS(phi) != NULL
              && Coalescer_interferesCC(coalescer, congruence_ai, Operation_PHICLASS(phi))) {
 	    PFA_TRACE && fprintf(PFA_TRACE, "\t\t\tAi interferes with PHIVAR!\n");
            merge = false;
          }
          //merge class(ai) & phimerged
          if (merge) {
            VariableClass phiClass = Operation_PHICLASS(phi);
 	    PFA_TRACE && fprintf(PFA_TRACE, "\t\t\tno interference. Merge.!\n");
            if (phiClass == NULL) phiClass = congruence_ai;
            else if (phiClass != congruence_ai)
              phiClass =  Coalescer_mergeCongruencesCC(coalescer, congruence_ai, phiClass);
            *Operation__PHICLASS(phi) = phiClass;
          }
        // endforeach Bi
        }
      }
      Coalescer_explicitPhi(coalescer, phi);
      Operation_clearFlags(phi, OperationFlag_CopyMode);
      // endforeach phi
    }  BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock ;
}

#ifdef $XCC__h
/**
 * Main processing of Boissinot CSSA method
 */
void
Boissinot_doBoissinot_I_Coalescer(Boissinot this);
#endif//$XCC__h

/**
 * Main processing of Boissinot CSSA method
 *
 * @pre  empty CONGRUENCES
 * 
 * Generated move operations around PHIs.
 * All newly created variables put in the same phi congruence class
 */
static void
Boissinot_doBoissinot_I(Boissinot this);

static void
Boissinot_doBoissinot_I(Boissinot this)
{
  Memory memory = Boissinot_MEMORY(this);
  Coalescer coalescer = Boissinot_COALESCER(this);
  IBList congruences = Boissinot_CONGRUENCES(this);
  CodeRegion codeRegion = Boissinot_CODEREGION(this);
  OperationStack phiOperations = Boissinot_PHIOPERATIONS(this);
  TemporaryTable temporaryTable = Boissinot_TEMPORARYTABLE(this);
  //
  // Step 1. Insert copies for each phi operand (result and arguments)
  // and merge all newly created variables in the same congruence class
  OperationStack_FOREACH_Operation(phiOperations, phi_operation) {
    int resIndex = 0;
    Operation copy_operation = NULL;
    BasicBlock block_0 = Operation_basicBlock(phi_operation);
    Temporary resource_0 = Operation_getResult(phi_operation, 0);
    Variable variable_0 = Temporary_VARIABLE(resource_0);
    Temporary origin = Variable_ORIGIN(variable_0);
    Temporary new_resource_0 = CodeRegion_makeVariableTemporary(codeRegion, origin);
    Variable new_variable_0 = Temporary_VARIABLE(new_resource_0);
    VariableClass congruence_0 = IBList_push(congruences);
    VariableClass_Ctor(congruence_0, new_variable_0);
    TemporaryTable_insert(temporaryTable, new_resource_0);
    // Insert a copy x0 = xnew_0 in the PHI block.
    copy_operation = BasicBlock_makeOperationECOPY(block_0);
    resIndex = Operation_addtoParCopy(copy_operation, resource_0, new_resource_0);
    // Replace x0 with xnew_0 as the target.
    // Add xnew_0 in phiCongruenceClass of xnew_0.
    Variable_setOperationResult(variable_0, copy_operation, resIndex);
    Variable_updateCongruence(variable_0);
    Variable_setOperationArgument(new_variable_0, copy_operation, resIndex);
    Variable_setOperationResult(new_variable_0, phi_operation, 0);
    if (Coalescer_CONSUMING(coalescer)) Variable_raiseConsuming(new_variable_0);
    //
    Operation_ARGUMENTS_FOREACH_Temporary(phi_operation, resource_i) {
      Variable variable_i = Temporary_VARIABLE(resource_i);
      Temporary origin = Variable_ORIGIN(variable_i);
      BasicBlockEdge edge_k = BasicBlock_getPhiEnterEdge(block_0, Operation_ARGUMENTS_INDEX);
      BasicBlock block_k = BasicBlockEdge_origBlock(edge_k);
      Temporary new_resource_i = CodeRegion_makeVariableTemporary(codeRegion, origin);
      Variable new_variable_i = Temporary_VARIABLE(new_resource_i);
      TemporaryTable_insert(temporaryTable, new_resource_i);
      // Insert a copy x_newi = xi in the corresponding block.
      copy_operation = BasicBlock_makeOperationLCOPY(block_k);
      resIndex = Operation_addtoParCopy(copy_operation, new_resource_i, resource_i);
      Variable_setOperationArgument(variable_i, copy_operation, resIndex);
      Variable_setOperationArgument(new_variable_i, phi_operation, Operation_ARGUMENTS_INDEX);
      Variable_setOperationResult(new_variable_i, copy_operation, resIndex);
      VariableClass_insert(congruence_0, new_variable_i);
      if (Coalescer_CONSUMING(coalescer)) Variable_raiseConsuming(new_variable_i);
    } Operation_ARGUMENTS_ENDEACH_Temporary;
  } OperationStack_ENDEACH_Operation;
  //
  if (PFA_TRACE) {
    TemporaryTable temporaryTable = Boissinot_TEMPORARYTABLE(this);
    fprintf(PFA_TRACE, "*** SSA Congruences\n");
    TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
      Variable variable = Temporary_VARIABLE(temporary);
      VariableClass phiCongruence = Variable_CONGRUENCE(variable);
      PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE)
        && fprintf(PFA_TRACE, " => ")
        && VariableClass_pretty(phiCongruence, PFA_TRACE, false)
        && fprintf(PFA_TRACE, "\n");
    } TemporaryTable_ENDEACH_Temporary;
  }
}

#ifdef $XCC__h
/*
 * Boissinot_doBoissinot_III_Coalescer --
 */
void
Boissinot_doBoissinot_III_Coalescer(Boissinot this);
#endif//$XCC__h

void
Boissinot_doBoissinot_III_Coalescer(Boissinot this)
{
  Coalescer coalescer = Boissinot_COALESCER(this);
  Coalescer_complete(coalescer, true);
  Boissinot_doBoissinot_III(this);
  if (PFA_TRACE) {
    Memory memory = Boissinot_MEMORY(this);
    CodeRegion codeRegion = Boissinot_CODEREGION(this);
    TemporaryTable temporaryTable = Boissinot_TEMPORARYTABLE(this);
    Liveness liveness = Liveness_make(memory, codeRegion, temporaryTable, true);
    fprintf(PFA_TRACE, " *** After Boissinot III\n");
    CodeRegion_pretty(Boissinot_CODEREGION(this), PFA_TRACE);
    Liveness_kill(liveness);
    fprintf(PFA_TRACE, "*** SSA Congruences\n");
    TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
      Variable variable = Temporary_VARIABLE(temporary);
      VariableClass phiCongruence = Variable_CONGRUENCE(variable);
      PFA_TRACE && Temporary_pretty(temporary, PFA_TRACE)
        && fprintf(PFA_TRACE, " => ")
        && VariableClass_pretty(phiCongruence, PFA_TRACE, false)
        && fprintf(PFA_TRACE, "\n");
    } TemporaryTable_ENDEACH_Temporary;
  }
}

void
Boissinot_doBoissinot_I_Coalescer(Boissinot this)
{
  String name = Procedure_name(CodeRegion_procedure(Boissinot_CODEREGION(this)));
  //Coalescer build in OptimizeCoalescing_Dominance
  PFA_TRACE && fprintf(PFA_TRACE, "*** Exact Method:\n")
            && fprintf(PFA_TRACE, "*** Exact COPY insertion:\n");
  // Step 1. Insert copies for each phi operand (result and arguments)
  Boissinot_doBoissinot_I(this);
  PFA_TRACE && fprintf(PFA_TRACE, " *** After Exact Step I\n")
            && CodeRegion_pretty(Boissinot_CODEREGION(this), PFA_TRACE);
#ifndef _NTRACE
  // TODO: fix bug in dumpAppelGraph
  PFA_STATS && 0 && SSAForm_dumpAppelGraph(CodeRegion_ssaForm(Boissinot_CODEREGION(this)), PFA_STATS, name);
#endif
  // Step 2.Interference Graph building
  PFA_TRACE && fprintf(PFA_TRACE, "*** Exact Interference Build:\n");
  {
    Coalescer coalescer = Boissinot_COALESCER(this);
    Coalescer_complete(coalescer, false);
    if (!_OPTIMIZE) {
      VariableClass class = Coalescer_findIntersectingClass(coalescer);
      Except_CHECK(class == NULL);
    }
    if (PFA_TRACE && Coalescer_INTERFERENCE(coalescer)) {
      Interference interference = Coalescer_INTERFERENCE(coalescer);
      TemporaryTable temporaryTable = Coalescer_TEMPORARYTABLE(coalescer);
      fprintf(PFA_TRACE, " *** After Exact Step 2\n");
      TemporaryTable_FOREACH_Temporary(temporaryTable, temporary_1) {
        TemporaryTable_FOREACH_Temporary(temporaryTable, temporary_2) {
          if (Interference_exists(interference, temporary_1, temporary_2)) {
            if (Temporary_IDENTITY(temporary_1) > Temporary_IDENTITY(temporary_2)) {
              fprintf(PFA_TRACE, "Intersection ");
              if (   Coalescer_EQUIVALUE > 0 &&
                         Variable_EQUIVALUE(Temporary_VARIABLE(temporary_1))
                      == Variable_EQUIVALUE(Temporary_VARIABLE(temporary_2))) {
                fprintf(PFA_TRACE, "But Same Value ");
              }
              Temporary_pretty(temporary_1, PFA_TRACE)
                && fprintf(PFA_TRACE, " ")
                && Temporary_pretty(temporary_2, PFA_TRACE)
                && fprintf(PFA_TRACE, "\n");
            }
          }
        } TemporaryTable_ENDEACH_Temporary;
      } TemporaryTable_ENDEACH_Temporary;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
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
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
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
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
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
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
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
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
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
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
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
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
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
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
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
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
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
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
    *Optimize__COALESCING(optimize) |= OptimizeCoalescing_Congruence;
    SSAForm_destruct(ssaForm);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c


#ifdef $XCC__c
  Minir_delete(minir);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Boissinot module.
 */
#define Boissinot_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Boissinot module.
 */
#define Boissinot_FINI()
#endif//$XCC__h

#if XCC__C
static void
Boissinot_TEST(void)
{
#include "PFA/Boissinot_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(Boissinot_TEST);
  return 0;
}
#endif

