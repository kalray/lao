#ifdef $XCC_h
/*
 * !!!!	Accelerator.xcc
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
 * @ingroup CGO
 * @brief Configurable Accelerator mapping.
 *
 * @warning This module is currently work in progress.
 */
#endif//$XCC_h


#include "CGO/CGO_.h"

#ifdef $XCC__h
#include "CGO/Accelerator.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Map code to configurable accelerator for a CodeRegion.
 */
struct Accelerator_ {
  //@args	Memory memory, CodeRegion codeRegion
  Memory MEMORY;
  CodeRegion CODEREGION;
  OperationStack_ OPLIST[1];
  PtrSeq_ BWLULIST[1];
};
#endif//$XCC_h

Accelerator
Accelerator_Ctor(Accelerator this,
                 Memory memory, CodeRegion codeRegion)
{
  *Accelerator__MEMORY(this) = memory;
  *Accelerator__CODEREGION(this) = codeRegion;
  OperationStack_Ctor(Accelerator_OPLIST(this), memory, 128);
  PtrSeq_Ctor(Accelerator_BWLULIST(this), memory, 128);
  return this;
}

void
Accelerator_Dtor(Accelerator this)
{
  OperationStack_Dtor(Accelerator_OPLIST(this));
  PtrSeq_Dtor(Accelerator_BWLULIST(this));
}

size_t
Accelerator_Size(Memory memory, CodeRegion codeRegion)
{
  return sizeof(Accelerator_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Accelerator_)\t%zu\n", sizeof(Accelerator_));
}
#endif//$XCC__c

#ifdef $XCC_h
Accelerator
Accelerator_new(Memory parent, CodeRegion codeRegion);
#endif//$XCC_h

Accelerator
Accelerator_new(Memory parent, CodeRegion codeRegion)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Accelerator_Size(memory, codeRegion);
  Accelerator this = Memory_alloc(memory, size);
  return Accelerator_Ctor(this, memory, codeRegion);
}

#ifdef $XCC_h
Accelerator
Accelerator_delete(Accelerator this);
#endif//$XCC_h

Accelerator
Accelerator_delete(Accelerator this)
{
  if (this != NULL) {
    Memory memory = Accelerator_MEMORY(this);
#ifndef _NDTOR
    Accelerator_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * BitWise Logic Unit
 */
struct BWLU_ {
  //@args	Memory memory, Operation root
  Memory MEMORY;
  OperationStack_ MEMBERS[1];    // Operations members of this BWLU
  int32_t NUM_INPUTS;            // Number of operands
  int32_t NUM_OUTPUTS;           // Number of operands
  Temporary ROOT;                // Main output
};
#endif//$XCC_h

BWLU
BWLU_Ctor(BWLU this, Memory memory, Operation root)
{
  *BWLU__MEMORY(this) = memory;
  OperationStack_Ctor(BWLU_MEMBERS(this), memory, 5);
  OperationStack_push(BWLU_MEMBERS(this), root);
  BWLU_NUM_INPUTS(this) = 2;
  BWLU_NUM_OUTPUTS(this) = 1;
  return this;
}

void
BWLU_Dtor(BWLU this)
{
  OperationStack_Dtor(BWLU_MEMBERS(this));
}

size_t
BWLU_Size(Memory memory, Operation root)
{
  return sizeof(BWLU_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(BWLU_)\t%zu\n", sizeof(BWLU_));
}
#endif//$XCC__c

#ifdef $XCC_h
BWLU
BWLU_new(Memory parent, Operation root);
#endif//$XCC_h

BWLU
BWLU_new(Memory parent, Operation root)
{
  Memory memory = Memory_new(parent, true);
  size_t size = BWLU_Size(memory, root);
  BWLU this = Memory_alloc(memory, size);
  return BWLU_Ctor(this, memory, root);
}

#ifdef $XCC_h
BWLU
BWLU_delete(BWLU this);
#endif//$XCC_h

BWLU
BWLU_delete(BWLU this)
{
  if (this != NULL) {
    Memory memory = BWLU_MEMORY(this);
#ifndef _NDTOR
    BWLU_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Pretty print of this BWLU.
 */
bool
BWLU_pretty(BWLU this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
BWLU_pretty(BWLU this, FILE *file)
{
  OperationStack oplist = BWLU_MEMBERS(this);
  fprintf(file, "BWLU contains %d instructions:\n", OperationStack_count(oplist));
  OperationStack_FOREACH_Operation(oplist, operation) {
    Operation_pretty(operation, file);
  } OperationStack_ENDEACH_Operation ;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////
/* Auxiliary functions for greedy BWLU selection */
///////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * greedyBWLU_add_prod_to_visit --   Add producer of temporary to the to_visit list.
 * Only when temporary is a variable whose producer is in current basic block.
 */
static bool
greedyBWLU_add_prod_to_visit(OperationStack to_visit, Temporary temporary, BasicBlock basicBlock);
#endif//$XCC_h

static bool
greedyBWLU_add_prod_to_visit(OperationStack to_visit, Temporary temporary, BasicBlock basicBlock)
{
  if (!Temporary_isVariable(temporary)) {
    CGO_TRACE && fprintf(CGO_TRACE, "    not adding producer of %d (not a variable)\n", Temporary_identity(temporary));
    return false;
  }
  {
    Variable variable = Temporary_VARIABLE(temporary);
    OperationResult producer = Variable_PRODUCER(variable);
    Operation producer_operation = OperationResult_OPERATION(producer);
    Except_CHECK(producer_operation != NULL);
    Except_CHECK(!Temporary_isConstant(temporary));
    if (Operation_basicBlock(producer_operation) != basicBlock) {
      CGO_TRACE && fprintf(CGO_TRACE, "    not adding producer of %d (op %d) (is not is same basic block\n", Temporary_identity(temporary), Operation_identity(producer_operation));
      return false;
    }
    CGO_TRACE && fprintf(CGO_TRACE, "    adding producer of %d (op %d)\n", Temporary_identity(temporary), Operation_identity(producer_operation));
    OperationStack_push2(to_visit, producer_operation);
    return true;
  }
}

#ifdef $XCC_h
/*
 * greedyBWLU_select_best_BWLU --  Create a BWLU with output root (at least) 
 * containing the most number of bitwise instructions.
 */
static BWLU
greedyBWLU_select_best_BWLU(OperationStack to_visit, Operation root, BasicBlock basicBlock);
#endif//$XCC_h

static BWLU
greedyBWLU_select_best_BWLU(OperationStack to_visit, Operation root, BasicBlock basicBlock)
{
  CodeRegion codeRegion = BasicBlock_codeRegion(basicBlock);
  Memory memory = CodeRegion_memory(codeRegion);
  BWLU bwlu = BWLU_new(memory, root);
  CGO_TRACE && BWLU_pretty(bwlu, CGO_TRACE);
  return bwlu;
}
 
///////////////////////////////////////////////////////////////////////////////
/* Main functions for greedy BWLU selection */
///////////////////////////////////////////////////////////////////////////////
#ifdef $XCC_h
/*
 * SSAForm_greedy_visit --     Greedy visit from leaves
 */
static bool
SSAForm_greedy_visit(Accelerator accelerator, BasicBlock basicBlock, OperationStack oplist);
#endif//$XCC_h

static bool
SSAForm_greedy_visit(Accelerator accelerator, BasicBlock basicBlock, OperationStack oplist)
{
  CGO_TRACE && fprintf(CGO_TRACE, "Operation list to visit:\n");
  while(!OperationStack_isEmpty(oplist)) {
    Operation operation = OperationStack_top(oplist);
    Operator op = Operation_operator(operation);
    if(Operation_flags(operation) & OperationFlag_Visited) {
      CGO_TRACE && fprintf(CGO_TRACE, "Operation already flagged processed!\n");
      break;
    }
    Operation_setFlags(operation, OperationFlag_Visited);
    OperationStack_pop(oplist);
    CGO_TRACE && fprintf(CGO_TRACE, "Visiting next operation in list:\n");
    CGO_TRACE && Operation_pretty(operation, CGO_TRACE);

    if(!Operator_isBitwise(op)) {
      CGO_TRACE && fprintf(CGO_TRACE, "  Operator not bitwise. Adding inputs to to_visit\n");
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        greedyBWLU_add_prod_to_visit(oplist, argument, basicBlock);
      } Operation_ARGUMENTS_ENDEACH_Temporary;
      CGO_TRACE && fprintf(CGO_TRACE, "\n");
    } else {
      BWLU bwlu;
      fprintf(CGO_TRACE, "  Operator is bitwise. Trying to put it in BWLU\n");
      bwlu = greedyBWLU_select_best_BWLU(oplist, operation, basicBlock);
      PtrSeq_push2(Accelerator_BWLULIST(accelerator), (PtrSeq*)bwlu);
    }
  }
  CGO_TRACE && fprintf(CGO_TRACE, "End Operation list.\n");
  /* Reset Visited flags */
  BasicBlock_FOREACH_Operation(basicBlock, operation) {
    Operation_clearFlags(operation, OperationFlag_Visited);
  } BasicBlock_ENDEACH_Operation;
  return true;
}

#ifdef $XCC_h
/*
 * SSAForm_basicBlock_greedyBWLU --     Greedy selection of BWLU
 */
static bool
SSAForm_basicBlock_greedyBWLU(Accelerator accelerator, BasicBlock this);
#endif//$XCC_h

static bool
SSAForm_basicBlock_greedyBWLU(Accelerator accelerator, BasicBlock this)
{
  /* Start a new DAG */
  /* Instructions leaves of the DAG are :
   * - instructions producing live-out variables (this includes
   *   instructions whose output has no consumer inside bb)
   * - instructions with no output
   *
   * Greedy covering start from bottom of DAG, i.e., from leaves
   */
  TemporarySet liveOut = BasicBlock_liveOutSet(this);
  OperationStack leaves = Accelerator_OPLIST(accelerator);

  if (liveOut == NULL) {
    CGO_TRACE && fprintf(CGO_TRACE, "!!! liveOUT is NULL !!!\n");
  } else if(TemporarySet_isEmpty(liveOut)) {
    CGO_TRACE && fprintf(CGO_TRACE, "!!! liveOUT is EMPTY !!!\n");
  } else {
    CGO_TRACE && fprintf(CGO_TRACE, "LiveOut variables:");
    TemporarySet_FOREACH_Temporary(liveOut, temporary) {
      greedyBWLU_add_prod_to_visit(leaves, temporary, this);
    } TemporarySet_ENDEACH_Temporary;
    CGO_TRACE && fprintf(CGO_TRACE, "\n");
  }

  BasicBlock_FOREACH_Operation(this, operation) {
    if(Operation_resCount(operation) == 0) {
      OperationStack_push2(leaves, operation);
    }
  } BasicBlock_ENDEACH_Operation;

  SSAForm_greedy_visit(accelerator, this, leaves);

  return true;
}



#ifdef $XCC_h
/*
 * SSAForm_selectBWLU --   Select BitWise Logic Units
 */
static bool
SSAForm_selectBWLU(Accelerator accelerator, SSAForm this);
#endif//$XCC_h

static bool
SSAForm_selectBWLU(Accelerator accelerator, SSAForm this)
{
  CodeRegion codeRegion = SSAForm_codeRegion(this);
  Procedure procedure = CodeRegion_procedure(codeRegion);
  const char *procedureName = Procedure_name(procedure);
  intptr_t regionId = CodeRegion_regionId(codeRegion);

  CGO_TRACE && fprintf(CGO_TRACE, "*** Accelerator begin BWLU selection:\n");

  // Visit all basic blocks
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    int index = BasicBlock_identity(block);
    int constCount = 0;

    CGO_TRACE && fprintf(CGO_TRACE, "--- BWLU selection on %s, region %0"PRIdPTR", BasicBlock %d\n", procedureName, regionId, index);
    SSAForm_basicBlock_greedyBWLU(accelerator, block);

    /* display all selected BWLU */
    CGO_TRACE && fprintf(CGO_TRACE, "List of BWLU selected:\n");
    PtrSeq_FOREACH(Accelerator_BWLULIST(accelerator), BWLU, bwlu) {
      CGO_TRACE && BWLU_pretty(bwlu, CGO_TRACE);
    } PtrSeq_ENDEACH;
    CGO_TRACE && fprintf(CGO_TRACE, "End list of BWLU selected.\n");

    PtrSeq_empty(Accelerator_BWLULIST(accelerator));

  } CodeRegion_ENDEACH_BasicBlock;

  CGO_TRACE && fprintf(CGO_TRACE, "*** Accelerator end BWLU selection:\n");
  return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * SSAForm_getDFGmagicbox --   Print data-flow graph for magic boxes in easy to 
 * parse format (yaml)
 */
static bool
SSAForm_getDFGmagicbox(SSAForm this, FILE *file);
#endif//$XCC_h

static bool
SSAForm_getDFGmagicbox(SSAForm this, FILE *file)
{
  CodeRegion codeRegion = SSAForm_codeRegion(this);
  intptr_t regionId = CodeRegion_regionId(codeRegion);

  if (file == NULL) file = stderr;
    // Add all basic blocks to the graph
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    int index = BasicBlock_identity(block);
    int constCount = 0;
    // Start a new DFG
    fprintf(file, "--- # Coderegion %0"PRIdPTR", BasicBlock %d\n", regionId, index);
    fprintf(file, "coderegion: %0"PRIdPTR"\n", regionId);
    fprintf(file, "basicblock: %d\n", index);
    // All live-in variables of block are in V_in (inputs of the DFG)
    {
      TemporarySet liveIn = BasicBlock_liveInSet(block);
      fprintf(file, "livein:\n");
      if (liveIn == NULL) {
        fprintf(file, "# liveIN is NULL\n");
      } else if(TemporarySet_isEmpty(liveIn)) {
        fprintf(file, "# NO NODE IN LIVEIN\n");
      } else {
        TemporarySet_FOREACH_Temporary(liveIn, temporary) {
          fprintf(file, "  - id: %d\n", Temporary_identity(temporary));
          fprintf(file, "    name: ");
          Temporary_pretty(temporary, file);
          fprintf(file, "\n");
        } TemporarySet_ENDEACH_Temporary;
      }
    }

    fprintf(file, "instructions:\n");
    BasicBlock_FOREACH_Operation(block, operation) {
      int rescount = Operation_resCount(operation);
      char nodename[255];
      char style[255];
      Operator op = Operation_operator(operation);
      fprintf(file, "  - id: %d\n", Operation_identity(operation));

      fprintf(file, "    bitwise: %s\n", Operator_isBitwise(op)? "true":"false");

      fprintf(file, "    mnemonic: %s\n", Operator_mnemonic(Operation_operator(operation)));
      // Arguments
      {
        fprintf(file, "    inputs:\n");
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if(Temporary_isConstant(argument)) {
            fprintf(file, "     - constant: ");
            Temporary_pretty(argument, file);
            fprintf(file, "\n");
          } else {
            fprintf(file, "     - variable: %d\n", Temporary_identity(argument));
          } 
        } Operation_ARGUMENTS_ENDEACH_Temporary;
      }
      // Results
      {
        fprintf(file, "    outputs:\n");
        Operation_RESULTS_FOREACH_Temporary(operation, result) {
          fprintf(file, "     - id: %d\n", Temporary_identity(result));
          fprintf(file, "       name: ");
          Temporary_pretty(result, file);
          fprintf(file, "\n");
        } Operation_RESULTS_ENDEACH_Temporary;
      }
    } BasicBlock_ENDEACH_Operation;

      // For all results, create more nodes (todo)
/*      if (rescount > 1) {*/
/*        Operation_RESULTS_FOREACH_Temporary(operation, result) {*/
/*          fprintf(file, "  n%d [shape=box, style=filled, color=lightgray, label=\"", Temporary_identity(result));*/
/*          Temporary_pretty(result, file);*/
/*          fprintf(file, "\"];\n");*/
/*          fprintf(file, "  %s:n%d -> n%d [headport=\"n\"] ;\n", nodename, Temporary_identity(result), Temporary_identity(result));*/
/*        } Operation_RESULTS_ENDEACH_Temporary;*/
/*      }*/

    // End of DFG
    fprintf(file, "\n");
  } CodeRegion_ENDEACH_BasicBlock;
  return true;
}



#ifdef $XCC_h
/*
 * SSAForm_getdottyDFGmagicbox --   Print data-flow graph for magic boxes in dot 
 * format
 */
static bool
SSAForm_getdottyDFGmagicbox(SSAForm this, FILE *file);
#endif//$XCC_h

static bool
SSAForm_getdottyDFGmagicbox(SSAForm this, FILE *file)
{
  CodeRegion codeRegion = SSAForm_codeRegion(this);
  intptr_t regionId = CodeRegion_regionId(codeRegion);

  if (file == NULL) file = stderr;
    // Add all basic blocks to the graph
  CodeRegion_FOREACH_BasicBlock(codeRegion, block) {
    int index = BasicBlock_identity(block);
    char filename[256];
    int constCount = 0;
    sprintf(filename, "dfg/dfg-%02"PRIdPTR"-%03d.dot", regionId, index);
    fprintf(stderr, "Writing Coderegion %0"PRIdPTR", BB %d in file %s\n", regionId, index, filename);
    file = fopen(filename, "w");

    // Start a new DFG
    fprintf(file, "digraph \"BB %d\" {\n", index);

    
    // All live-in variables of block are in V_in (inputs of the DFG)
    {
      TemporarySet liveIn = BasicBlock_liveInSet(block);
      fprintf(file, "  { node [shape=invtriangle];\n  rank = source;\n");
      if (liveIn == NULL) {
        fprintf(file, "/* liveIN is NULL */\n");
      } else if(TemporarySet_isEmpty(liveIn)) {
        fprintf(file, "/* NO NODE IN LIVEIN */\n");
      } else {
        TemporarySet_FOREACH_Temporary(liveIn, temporary) {
          fprintf(file, "  n%d [style=filled, color=red1, label=\"", Temporary_identity(temporary));
          Temporary_pretty(temporary, file);
          fprintf(file, "\"];\n");
        } TemporarySet_ENDEACH_Temporary;
      }
      fprintf(file, " }\n");
    }

    // All variables in phiDefSet
/*    {*/
/*      TemporarySet phidef = BasicBlock_phiDefSet(block);*/
/*      fprintf(file, "  { node [shape=hexagon];\n  rank = source;\n");*/
/*      if (phidef == NULL) {*/
/*        fprintf(file, "|+ phidef is NULL +|\n");*/
/*      } else if(TemporarySet_isEmpty(phidef)) {*/
/*        fprintf(file, "|+ NO NODE IN phidef +|\n");*/
/*      } else {*/
/*        TemporarySet_FOREACH_Temporary(phidef, temporary) {*/
/*          fprintf(file, "  n%d [style=filled, color=blue, label=\"", Temporary_identity(temporary));*/
/*          Temporary_pretty(temporary, file);*/
/*          fprintf(file, "\"];\n");*/
/*        } TemporarySet_ENDEACH_Temporary;*/
/*      }*/
/*      fprintf(file, " }\n");*/
/*    }*/

    BasicBlock_FOREACH_Operation(block, operation) {
      int rescount = Operation_resCount(operation);
      char nodename[255];
      char style[255];
      Operator op = Operation_operator(operation);
      if (rescount == 1) {
        Temporary result = Operation_getResult(operation, 0);
        sprintf(nodename, "n%d", Temporary_identity(result));
      } else {
        sprintf(nodename, "i%d", Operation_identity(operation));
      }

      if (Operator_isMemory(op)) {
        sprintf(style,"%s","style=filled, color=coral,");
      } else if (Operator_isBitwise(op)) {
        // fprintf(stderr, "  operator %s, %s\n", Operator_name_(op), Operator_isBitwise(op)? "LOGICAL" : "not log");
        sprintf(style,"%s","style=filled, color=chartreuse,");
      } else {
        sprintf(style,"%s","style=filled, color=lightblue,");
      }


      if (Operator_isPhi(op)) {
        fprintf(file, "  { rank = min ;\n");
      }

      fprintf(file, "  %s [shape=record, %s label=\"{", nodename, style);
      // Arguments
      {
        int narg=0;
        fprintf(file, "{");
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (narg == 0) { narg = 1; } else { fprintf(file, "|"); }
          fprintf(file, "<n%d> ", Temporary_identity(argument));
          Temporary_pretty(argument, file);
        } Operation_ARGUMENTS_ENDEACH_Temporary;
      }
      // fprintf(file, "}|%s|V%d }\"];\n", Operator_mnemonic(Operation_operator(operation)), Temporary_identity(result));
      // Operator
      fprintf(file, "}|%s|{", Operator_mnemonic(Operation_operator(operation)));
      // Results
      {
        int nres=0;
        Operation_RESULTS_FOREACH_Temporary(operation, result) {
          if (nres == 0) { nres = 1; } else { fprintf(file, "|"); }
          fprintf(file, "<n%d> ", Temporary_identity(result));
          Temporary_pretty(result, file);
        } Operation_RESULTS_ENDEACH_Temporary;
        fprintf(file, "}");
      }
      fprintf(file, "}\"];\n");

      // If more than 1 result, create more nodes
      if (rescount > 1) {
        Operation_RESULTS_FOREACH_Temporary(operation, result) {
          fprintf(file, "  n%d [shape=box, style=filled, color=lightgray, label=\"", Temporary_identity(result));
          Temporary_pretty(result, file);
          fprintf(file, "\"];\n");
          fprintf(file, "  %s:n%d -> n%d [headport=\"n\"] ;\n", nodename, Temporary_identity(result), Temporary_identity(result));
        } Operation_RESULTS_ENDEACH_Temporary;
      }

      if (Operator_isPhi(op)) {
        fprintf(file, "  }\n"); // close 'rank=min'
      }

      // Second node for "use" of phi (to break loops) ?
/*      if (Operator_isPhi(Operation_operator(operation))) {*/
/*        int narg=0;*/
/*        Temporary result = Operation_getResult(operation, 0);*/
/*        fprintf(file, " {rank=max;  phin%d [shape=record, label=\"{", Temporary_identity(result));*/
/*        // arguments*/
/*        fprintf(file, "{");*/
/*        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {*/
/*          if (narg == 0) { narg = 1; } else { fprintf(file, "|"); }*/
/*          fprintf(file, "<n%d> V%d", Temporary_identity(argument), Temporary_identity(argument));*/
/*        } Operation_ARGUMENTS_ENDEACH_Temporary;*/
/*        // operator + result*/
/*        fprintf(file, "}|use of %s|V%d }\"]; }\n", Operator_mnemonic(Operation_operator(operation)), Temporary_identity(result));*/
/*      }*/

      // For each argument, add an edge arg -> res
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        char nodetype = 'n';
        int nodeid = Temporary_identity(argument);
        // new constant node
        if(Temporary_isConstant(argument)) {
          // fprintf(file, "  n%d [shape=Mrecord,style=filled,color=lightgray,label=\"",Temporary_identity(argument));
          nodetype = 'c';
          nodeid = constCount++;
          fprintf(file, "  %c%d [shape=box,style=filled,color=lightgray,label=\"",nodetype,nodeid);
          Temporary_pretty(argument, file);
          fprintf(file, "\"];\n");
        }
        fprintf(file, "  %c%d -> %s:n%d [tailport=\"s\"%s] ;\n",
            nodetype,
            nodeid,
            nodename,
            Temporary_identity(argument),
            (Operator_isPhi(Operation_operator(operation)) ? ", weight=0" :
             (nodetype == 'c' ? ", weight=10" : ""))
            );
      } Operation_ARGUMENTS_ENDEACH_Temporary;
      fprintf(file, "\n");
    } BasicBlock_ENDEACH_Operation;

    // Print live-out variables for checking
    {
      TemporarySet liveOut = BasicBlock_liveOutSet(block);

      fprintf(file, "  { node [shape=triangle, style=filled, color=red2];\n"); // } sink rank not working  rank = sink;\n");
      if (liveOut == NULL) {
        fprintf(file, "/* liveOUT is NULL */\n");
      } else if(TemporarySet_isEmpty(liveOut)) {
        fprintf(file, "/* NO NODE IN LIVEOUT */\n");
      } else {
        TemporarySet_FOREACH_Temporary(liveOut, temporary) {
          int id = Temporary_identity(temporary);
          fprintf(file, "  nout%d [label=\"", id);
          Temporary_pretty(temporary, file);
          fprintf(file, "\"];\n");
          fprintf(file, "  n%d -> nout%d [tailport=\"s\"] ;\n", id, id);
        } TemporarySet_ENDEACH_Temporary;
      }
      fprintf(file, " }\n");
    }

    // End of DFG
    fprintf(file, "}\n");
    fclose(file);
  } CodeRegion_ENDEACH_BasicBlock;
  return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Optimize code for configurable accelerator.
 */
bool
Accelerator_optimize(Accelerator this);
#endif//$XCC_h

bool
Accelerator_optimize(Accelerator this)
{
  CodeRegion codeRegion = Accelerator_CODEREGION(this);
  SSAForm ssaForm = CodeRegion_ssaForm(codeRegion);
  Memory memory = CodeRegion_memory(codeRegion);
  TemporaryTable globalTable = CodeRegion_makeGlobalTable(codeRegion, TemporaryFlag_Constant, 0);
  Liveness liveness = Liveness_make(memory, codeRegion, globalTable, true);
  CGO_TRACE && fprintf(CGO_TRACE, "*** Accelerator begin optimize:\n");
  BWLU_TRACE && fprintf(BWLU_TRACE, "----8<----YAML Begin----8<----\n");
  BWLU_TRACE && SSAForm_getDFGmagicbox(ssaForm, BWLU_TRACE);
  BWLU_TRACE && fprintf(BWLU_TRACE, "----8<----YAML End----8<----\n");
  BWLU_TRACE && fprintf(BWLU_TRACE, "----8<----MiniR Begin----8<----\n");
  /* BWLU_TRACE && fprintf(BWLU_TRACE, "functions:\n");*/
  BWLU_TRACE && CodeRegion_prettyMinir(codeRegion, BWLU_TRACE);
  BWLU_TRACE && fprintf(BWLU_TRACE, "----8<----MiniR End----8<----\n");
  SSAForm_selectBWLU(this, ssaForm);
  CGO_TRACE && fprintf(CGO_TRACE, "*** Accelerator end optimize:\n");
  Liveness_kill(liveness);
  return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Accelerator Memory.
 */
Memory
Accelerator_memory(Accelerator this);
#endif//$XCC_h

Memory
Accelerator_memory(Accelerator this)
{
  return Accelerator_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This Accelerator CodeRegion.
 */
CodeRegion
Accelerator_codeRegion(Accelerator this);
#endif//$XCC_h

CodeRegion
Accelerator_codeRegion(Accelerator this)
{
  return Accelerator_CODEREGION(this);
}

#ifdef $XCC_h
/**
 * Pretty-print this Accelerator.
 */
bool
Accelerator_pretty(Accelerator this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Accelerator_pretty(Accelerator this, FILE *file)
{
  if (file == NULL) file = stderr;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Accelerator module.
 */
#define Accelerator_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Accelerator module.
 */
#define Accelerator_FINI()
#endif//$XCC__h

#if XCC__C
static void
Accelerator_TEST(void)
{
#include "CGO/Accelerator_.c"
}

int
main(int argc, char **argv)
{
  CGO_Test(Accelerator_TEST);
  return 0;
}
#endif

