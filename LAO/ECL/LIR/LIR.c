#ifdef $XCC_h
/*
 * !!!!	LIR.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu)
 * Duco van Amstel (duco.van-amstel@kalray.eu)
 *
 * Copyright 2010 - 2013 Kalray
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
 * @defgroup LIR Layered Intermediate Representation
 * @ingroup ECL
 * @brief Includes (SSA) Variable as first-class citizen.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/LIR.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CCL.h"
#include "ECL.h"
#include "MDT/MDT.h"
#include "AIR/AIR.h"
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include "LIR/Data.h"
#include "LIR/Optimize.h"
#include "LIR/Configure.h"
#include "LIR/Temporary.h"
#include "LIR/Operation.h"
#include "LIR/Variable.h"
#include "LIR/Selector.h"
#include "LIR/BasicBlock.h"
#include "LIR/Dependence.h"
#include "LIR/LoopScope.h"
#include "LIR/CodeRegion.h"
#include "LIR/Procedure.h"
#include "LIR/Program.h"
#include "LIR/Minir.h"
#include "LIR/Tirex.h"
#endif//$XCC_h

#ifdef $XCC__h
#include "LIR/Data_.h"
#include "LIR/Optimize_.h"
#include "LIR/Configure_.h"
#include "LIR/Temporary_.h"
#include "LIR/Operation_.h"
#include "LIR/Variable_.h"
#include "LIR/Selector_.h"
#include "LIR/BasicBlock_.h"
#include "LIR/Dependence_.h"
#include "LIR/LoopScope_.h"
#include "LIR/CodeRegion_.h"
#include "LIR/Procedure_.h"
#include "LIR/Program_.h"
#include "LIR/Minir_.h"
#include "LIR/Tirex_.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * !!!	LIR Interface Summary
 *
 * !!	Configure
 *
 *	void Configure_load(Configure this, ConfigureItem item, ...);
 *	void Configure_loadVA(Configure this, int item, va_list va);
 *	void Configure_merge(Configure this, Configure that);
 *
 * !!	Symbol
 *
 *	Symbol SymbolTable_lookup(SymbolTable this, uint32_t index, const char *name);
 *
 * !!	Program
 *
 *	Program Program_new(Memory parent, Processor processor, Convention convention);
 *	Configure Program_configure(Program this);
 *	LabelTable Program_labelTable(Program this);
 *	SymbolTable Program_symbolTable(Program this);
 *	Procedure Program_makeProcedure(Program this, Symbol symbol);
 *	void Program_killProcedure(Program this, Procedure procedure);
 *	Program Program_delete(Program this);
 *
 * !!	Procedure
 *
 *	Temporary Procedure_makeDedicatedTemporary(Procedure this, Register registre);
 *	Temporary Procedure_makeAssignedTemporary(Procedure this, Register registre);
 *	Temporary Procedure_makeVirtualTemporary(Procedure this, RegFile regFile);
 *	Temporary Procedure_makeAbsoluteTemporary(Procedure this,
 *	                                          int64_t value, Immediate immediate);
 *	Temporary Procedure_makeSymbolTemporary(Procedure this, Symbol symbol,
 *	                                        int64_t offset, Immediate immediate);
 *	Temporary Procedure_makeLabelTemporary(Procedure this, Label label,
 *	                                       Immediate immediate);
 *	Temporary Procedure_makeModifierTemporary(Procedure this, Modifier modifier,
 *                                                ModifierMember member);
 *	BasicBlock Procedure_makeBasicBlock(Procedure this, Processor processor,
 *	                                    intptr_t regionId, float frequency);
 *	void Procedure_setEntryBlock(Procedure this, BasicBlock basicBlock);
 *	void Procedure_setExitBlock(Procedure this, BasicBlock basicBlock);
 *	void Procedure_setStartBlock(Procedure this, BasicBlock basicBlock);
 *	void Procedure_buildCodeRegions(Procedure this);
 *	LoopScope Procedure_makeLoopScope(Procedure this, BasicBlock headerBlock,
 *	                                  Configure configure);
 *
 * !!	LoopScope
 *
 *	void LoopScope_makeDependenceTable(LoopScope this);
 *
 * !!	BasicBlock
 *
 *	void BasicBlock_addLabel(BasicBlock this, Label label);
 *	BasicBlockEdge BasicBlock_makeEdge(BasicBlock this,
 *	                                   BasicBlock that, float probability)
 *
 * !!	Selector
 *
 *	Operation
 *	Selector_makeOperation_0_0(Selector this,
 *	                           Operator operator);
 *	Operation
 *	Selector_makeOperation_0_1(Selector this,
 *	                           Operator operator,
 *	                           Temporary argument_0);
 *	Operation
 *	Selector_makeOperation_0_2(Selector this,
 *	                           Operator operator,
 *	                           Temporary argument_0,
 *	                           Temporary argument_1);
 *	Operation
 *	Selector_makeOperation_0_3(Selector this,
 *	                           Operator operator,
 *	                           Temporary argument_0,
 *	                           Temporary argument_1,
 *	                           Temporary argument_2);
 *	Operation
 *	Selector_makeOperation_1_0(Selector this,
 *	                           Operator operator,
 *	                           Temporary result_0);
 *	Operation
 *	Selector_makeOperation_1_1(Selector this,
 *	                           Operator operator,
 *	                           Temporary result_0,
 *	                           Temporary argument_0);
 *	Operation
 *	Selector_makeOperation_1_2(Selector this,
 *	                           Operator operator,
 *	                           Temporary result_0,
 *	                           Temporary argument_0,
 *	                           Temporary argument_1);
 *	Operation
 *	Selector_makeOperation_1_3(Selector this,
 *	                           Operator operator,
 *	                           Temporary result_0,
 *	                           Temporary argument_0,
 *	                           Temporary argument_1,
 *	                           Temporary argument_2);
 *	Operation
 *	Selector_makeOperation_2_3(Selector this,
 *	                           Operator operator,
 *	                           Temporary result_0,
 *	                           Temporary result_1,
 *	                           Temporary argument_0,
 *	                           Temporary argument_1,
 *	                           Temporary argument_2);
 *
 * !!	Operation
 *
 *	void Operation_setVolatile(Operation operation);
 *	void Operation_setMemorySafe(Operation operation);
 *	void Operation_setMemoryTouch(Operation operation);
 *	void Operation_setMemoryFence(Operation operation);
 *	void Operation_setMemorySpill(Operation operation);
 */
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * !!!	LIR Declarations
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * File handle used to print the LIR traces.
 */
extern FILE *LIR_TRACE;
#ifdef _NTRACE
#define LIR_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef LIR_TRACE
FILE *LIR_TRACE = NULL;

#ifdef $XCC_h
/*
 * Override for the Optimize settings.
 */
extern int FORCE_ACTIVATION;
extern int FORCE_CONVERSION;
extern int FORCE_COALESCING;
extern int FORCE_REWRITING;
extern int FORCE_PREDICATION;
extern int FORCE_SCHEDULING;
extern int FORCE_ALLOCATION;
extern int FORCE_RCMSSOLVING;
extern int FORCE_RCMSPROBLEM;
extern int FORCE_LOGTIMEOUT;
extern int FORCE_SCOREBOARDING;
extern int FORCE_WINDOWSIZE;
extern int FORCE_PREPADDING;
extern int FORCE_POSTPADDING;
extern int FORCE_PROFILING;
#endif//$XCC_h

int FORCE_ACTIVATION;
int FORCE_CONVERSION;
int FORCE_COALESCING;
int FORCE_REWRITING;
int FORCE_PREDICATION;
int FORCE_SCHEDULING;
int FORCE_ALLOCATION;
int FORCE_RCMSSOLVING;
int FORCE_RCMSPROBLEM;
int FORCE_LOGTIMEOUT;
int FORCE_SCOREBOARDING;
int FORCE_WINDOWSIZE;
int FORCE_PREPADDING;
int FORCE_POSTPADDING;
int FORCE_PROFILING;

#ifdef $XCC_h
/*
 * Override for the Configure settings.
 */
extern int FORCE_COMPENSATION;
extern int FORCE_SPECULATION;
extern int FORCE_RELAXATION;
extern int FORCE_PIPELINING;
extern int FORCE_RENAMING;
extern int FORCE_BOOSTING;
extern int FORCE_ALIASING;
extern int FORCE_PRELOADING;
extern int FORCE_L1MISSEXTRA;
#endif//$XCC_h

int FORCE_COMPENSATION;
int FORCE_SPECULATION;
int FORCE_RELAXATION;
int FORCE_PIPELINING;
int FORCE_RENAMING;
int FORCE_BOOSTING;
int FORCE_ALIASING;
int FORCE_PRELOADING;
int FORCE_L1MISSEXTRA;

#ifdef $XCC_h
bool
LIR_IndentXML(FILE *file, int indent);
#endif//$XCC_h

bool
LIR_IndentXML(FILE *file, int indent)
{
  int i;
  for (i = 0; i < indent; i++) {
    fputc(' ', file);
    fputc(' ', file);
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Outputs help on LIR environment variable options to file.
 */
bool
LIR_Help(FILE *file);
#endif//$XCC_h

bool
LIR_Help(FILE *file)
{
  fprintf(file, "FORCE_ACTIVATION= comma separated list of:\n");
  ETable_pretty(OptimizeActivation_Table, " ", file);
  fprintf(file, "  Force the activation of LAO phases.\n"
                "  The default is given by the context of invocation.\n\n");
  fprintf(file, "FORCE_CONVERSION= comma separated list of:\n");
  ETable_pretty(OptimizeConversion_Table, " ", file);
  fprintf(file, "  Force flags for the SSA construction.\n"
                "  The default value is: ");
  OptimizeConversion_prettyFlags(OptimizeConversion_default, file);
  fprintf(file, "\n\n");
  fprintf(file, "FORCE_COALESCING= comma separated list of:\n");
  ETable_pretty(OptimizeCoalescing_Table, " ", file);
  fprintf(file, "  Force flags for the SSA variable coalescing.\n"
                "  The default value is: ");
  OptimizeCoalescing_prettyFlags(OptimizeCoalescing_default, file);
  fprintf(file, "\n\n");
  fprintf(file, "FORCE_REWRITING= comma separated list of:\n");
  ETable_pretty(OptimizeRewriting_Table, " ", file);
  fprintf(file, "  Force flags for the instruction rewriting.\n"
                "  The default value is: ");
  OptimizeRewriting_prettyFlags(OptimizeRewriting_default, file);
  fprintf(file, "\n\n");
  fprintf(file, "FORCE_ALLOCATION= comma separated list of:\n");
  ETable_pretty(OptimizeAllocation_Table, " ", file);
  fprintf(file, "  Force flags for the register allocation.\n"
                "  The default value is: ");
  OptimizeAllocation_prettyFlags(OptimizeAllocation_default, file);
  fprintf(file, "\n\n");
  fprintf(file, "FORCE_SPILLING= comma separated list of:\n");
  ETable_pretty(OptimizeSpilling_Table, " ", file);
  fprintf(file, "  Force flags for the register spilling.\n"
                "  The default value is: ");
  OptimizeSpilling_prettyFlags(OptimizeSpilling_default, file);
  fprintf(file, "\n\n");
  fprintf(file, "FORCE_COLORING= comma separated list of:\n");
  ETable_pretty(OptimizeColoring_Table, " ", file);
  fprintf(file, "  Force flags for the register coloring.\n"
                "  The default value is: ");
  OptimizeColoring_prettyFlags(OptimizeColoring_default, file);
  fprintf(file, "\n\n");
  fprintf(file, "FORCE_RCMSSOLVING= comma separated list of:\n");
  ETable_pretty(OptimizeRCMSSolving_Table, " ", file);
  fprintf(file, "  Force flags for the linear programming formulation of modulo scheduling.\n"
                "  The default value is: ");
  OptimizeRCMSSolving_prettyFlags(OptimizeRCMSSolving_default, file);
  fprintf(file, "\n\n");
  fprintf(file, "FORCE_RCMSPROBLEM= comma separated list of:\n");
  ETable_pretty(OptimizeRCMSProblem_Table, " ", file);
  fprintf(file, "  Force flags for the resource-constrained modulo scheduling plugin.\n"
                "  The default value is: ");
  OptimizeRCMSProblem_prettyFlags(OptimizeRCMSProblem_default, file);
  fprintf(file, "\n\n");
  fprintf(file, "FORCE_PROFILING= comma separated list of:\n");
  ETable_pretty(OptimizeProfiling_Table, " ", file);
  fprintf(file, "  Force flags for the profiling information type.\n"
                "  The default value is: ");
  OptimizeProfiling_prettyFlags(OptimizeProfiling_default, file);
  fprintf(file, "\n\n");
  return true;
}

#ifdef $XCC__c
{
  LIR_Help(stdout);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Initialize the LIR library.
 */
void
LIR_INIT(void);
#endif//$XCC_h

static int LIR_Initialized = 0;

void
LIR_INIT(void)
{
  if (LIR_Initialized++ == 0) {
    CCL_INIT();
    MDT_INIT();
    AIR_INIT();
    Optimize_INIT();
    Configure_INIT();
    Temporary_INIT();
    Operation_INIT();
    Variable_INIT();
    Selector_INIT();
    BasicBlock_INIT();
    Dependence_INIT();
    LoopScope_INIT();
    CodeRegion_INIT();
    Procedure_INIT();
    Program_INIT();
    Minir_INIT();
    Tirex_INIT();

    if (GETENV("HELP")) {
      LIR_Help(stderr);
    }
    FORCE_ACTIVATION = -1;
    if (GETENV("FORCE_ACTIVATION")) {
      FORCE_ACTIVATION = OptimizeActivation_parseFlags(GETENV("FORCE_ACTIVATION"));
      fprintf(stderr, "*** FORCE_ACTIVATION=");
      OptimizeActivation_prettyFlags(FORCE_ACTIVATION, stderr);
      fprintf(stderr, "\n");
    }
    FORCE_CONVERSION = -1;
    if (GETENV("FORCE_CONVERSION")) {
      FORCE_CONVERSION = OptimizeConversion_parseFlags(GETENV("FORCE_CONVERSION"));
      fprintf(stderr, "*** FORCE_CONVERSION=");
      OptimizeConversion_prettyFlags(FORCE_CONVERSION, stderr);
      fprintf(stderr, "\n");
    }
    FORCE_COALESCING = -1;
    if (GETENV("FORCE_COALESCING")) {
      FORCE_COALESCING = OptimizeCoalescing_parseFlags(GETENV("FORCE_COALESCING"));
      fprintf(stderr, "*** FORCE_COALESCING=");
      OptimizeCoalescing_prettyFlags(FORCE_COALESCING, stderr);
      fprintf(stderr, "\n");
    }
    FORCE_REWRITING = -1;
    if (GETENV("FORCE_REWRITING")) {
      FORCE_REWRITING = OptimizeRewriting_parseFlags(GETENV("FORCE_REWRITING"));
      fprintf(stderr, "*** FORCE_REWRITING=");
      OptimizeRewriting_prettyFlags(FORCE_REWRITING, stderr);
      fprintf(stderr, "\n");
    }
    FORCE_PREDICATION = -1;
    if (GETENV("FORCE_PREDICATION")) {
      sscanf(GETENV("FORCE_PREDICATION"), "%d", &FORCE_PREDICATION);
      fprintf(stderr, "*** FORCE_PREDICATION=%d\n", FORCE_PREDICATION);
    }
    FORCE_SCHEDULING = -1;
    if (GETENV("FORCE_SCHEDULING")) {
      sscanf(GETENV("FORCE_SCHEDULING"), "%d", &FORCE_SCHEDULING);
      fprintf(stderr, "*** FORCE_SCHEDULING=%d\n", FORCE_SCHEDULING);
    }
    FORCE_ALLOCATION = -1;
    if (GETENV("FORCE_ALLOCATION")) {
      FORCE_ALLOCATION = OptimizeAllocation_parseFlags(GETENV("FORCE_ALLOCATION"));
      fprintf(stderr, "*** FORCE_ALLOCATION=");
      OptimizeAllocation_prettyFlags(FORCE_ALLOCATION, stderr);
      fprintf(stderr, "\n");
    }
    FORCE_RCMSSOLVING = -1;
    if (GETENV("FORCE_RCMSSOLVING")) {
      FORCE_RCMSSOLVING = OptimizeRCMSSolving_parseFlags(GETENV("FORCE_RCMSSOLVING"));
      fprintf(stderr, "*** FORCE_RCMSSOLVING=");
      OptimizeRCMSSolving_prettyFlags(FORCE_RCMSSOLVING, stderr);
      fprintf(stderr, "\n");
    }
    FORCE_RCMSPROBLEM = -1;
    if (GETENV("FORCE_RCMSPROBLEM")) {
      FORCE_RCMSPROBLEM = OptimizeRCMSProblem_parseFlags(GETENV("FORCE_RCMSPROBLEM"));
      fprintf(stderr, "*** FORCE_RCMSPROBLEM=");
      OptimizeRCMSProblem_prettyFlags(FORCE_RCMSPROBLEM, stderr);
      fprintf(stderr, "\n");
    }
    FORCE_LOGTIMEOUT = -1;
    if (GETENV("FORCE_LOGTIMEOUT")) {
      FORCE_LOGTIMEOUT = OptimizeLogTimeOut_parseFlags(GETENV("FORCE_LOGTIMEOUT"));
      fprintf(stderr, "*** FORCE_LOGTIMEOUT=");
      OptimizeLogTimeOut_prettyFlags(FORCE_LOGTIMEOUT, stderr);
      fprintf(stderr, "\n");
    }
    FORCE_PROFILING = -1;
    if (GETENV("FORCE_PROFILING")) {
      FORCE_PROFILING = OptimizeProfiling_parseFlags(GETENV("FORCE_PROFILING"));
      fprintf(stderr, "*** FORCE_PROFILING=");
      OptimizeProfiling_prettyFlags(FORCE_PROFILING, stderr);
      fprintf(stderr, "\n");
    }
    FORCE_SCOREBOARDING = -1;
    if (GETENV("FORCE_SCOREBOARDING")) {
      sscanf(GETENV("FORCE_SCOREBOARDING"), "%d", &FORCE_SCOREBOARDING);
      fprintf(stderr, "*** FORCE_SCOREBOARDING=%d\n", FORCE_SCOREBOARDING);
    }
    FORCE_WINDOWSIZE = -1;
    if (GETENV("FORCE_WINDOWSIZE")) {
      sscanf(GETENV("FORCE_WINDOWSIZE"), "%d", &FORCE_WINDOWSIZE);
      fprintf(stderr, "*** FORCE_WINDOWSIZE=%d\n", FORCE_WINDOWSIZE);
    }
    FORCE_PREPADDING = -1;
    if (GETENV("FORCE_PREPADDING")) {
      sscanf(GETENV("FORCE_PREPADDING"), "%d", &FORCE_PREPADDING);
      fprintf(stderr, "*** FORCE_PREPADDING=%d\n", FORCE_PREPADDING);
    }
    FORCE_POSTPADDING = -1;
    if (GETENV("FORCE_POSTPADDING")) {
      sscanf(GETENV("FORCE_POSTPADDING"), "%d", &FORCE_POSTPADDING);
      fprintf(stderr, "*** FORCE_POSTPADDING=%d\n", FORCE_POSTPADDING);
    }
    //
    FORCE_COMPENSATION = -1;
    if (GETENV("FORCE_COMPENSATION")) {
      sscanf(GETENV("FORCE_COMPENSATION"), "%d", &FORCE_COMPENSATION);
      fprintf(stderr, "*** FORCE_COMPENSATION=%d\n", FORCE_COMPENSATION);
    }
    FORCE_SPECULATION = -1;
    if (GETENV("FORCE_SPECULATION")) {
      sscanf(GETENV("FORCE_SPECULATION"), "%d", &FORCE_SPECULATION);
      fprintf(stderr, "*** FORCE_SPECULATION=%d\n", FORCE_SPECULATION);
    }
    FORCE_RELAXATION = -1;
    if (GETENV("FORCE_RELAXATION")) {
      sscanf(GETENV("FORCE_RELAXATION"), "%d", &FORCE_RELAXATION);
      fprintf(stderr, "*** FORCE_RELAXATION=%d\n", FORCE_RELAXATION);
    }
    FORCE_PIPELINING = -1;
    if (GETENV("FORCE_PIPELINING")) {
      sscanf(GETENV("FORCE_PIPELINING"), "%d", &FORCE_PIPELINING);
      fprintf(stderr, "*** FORCE_PIPELINING=%d\n", FORCE_PIPELINING);
    }
    FORCE_RENAMING = -1;
    if (GETENV("FORCE_RENAMING")) {
      sscanf(GETENV("FORCE_RENAMING"), "%d", &FORCE_RENAMING);
      fprintf(stderr, "*** FORCE_RENAMING=%d\n", FORCE_RENAMING);
    }
    FORCE_BOOSTING = -1;
    if (GETENV("FORCE_BOOSTING")) {
      sscanf(GETENV("FORCE_BOOSTING"), "%d", &FORCE_BOOSTING);
      fprintf(stderr, "*** FORCE_BOOSTING=%d\n", FORCE_BOOSTING);
    }
    FORCE_ALIASING = -1;
    if (GETENV("FORCE_ALIASING")) {
      sscanf(GETENV("FORCE_ALIASING"), "%d", &FORCE_ALIASING);
      fprintf(stderr, "*** FORCE_ALIASING=%d\n", FORCE_ALIASING);
    }
    FORCE_PRELOADING = -1;
    if (GETENV("FORCE_PRELOADING")) {
      sscanf(GETENV("FORCE_PRELOADING"), "%d", &FORCE_PRELOADING);
      fprintf(stderr, "*** FORCE_PRELOADING=%d\n", FORCE_PRELOADING);
    }
    FORCE_L1MISSEXTRA = -1;
    if (GETENV("FORCE_L1MISSEXTRA")) {
      sscanf(GETENV("FORCE_L1MISSEXTRA"), "%d", &FORCE_L1MISSEXTRA);
      fprintf(stderr, "*** FORCE_L1MISSEXTRA=%d\n", FORCE_L1MISSEXTRA);
    }
    if (GETENV("LIR_TRACE")) {
      int file = 0;
      sscanf(GETENV("LIR_TRACE"), "%d", &file);
      if (file == 1) LIR_TRACE = stdout;
      else if (file == 2) LIR_TRACE = stderr;
    }
  }
}

#ifdef $XCC_h
/**
 * Finalize the LIR library.
 */
void
LIR_FINI(void);
#endif//$XCC_h

void
LIR_FINI(void)
{
  if (--LIR_Initialized == 0) {
    Tirex_FINI();
    Minir_FINI();
    Program_FINI();
    Procedure_FINI();
    CodeRegion_FINI();
    LoopScope_FINI();
    Dependence_FINI();
    BasicBlock_FINI();
    Selector_FINI();
    Variable_FINI();
    Operation_FINI();
    Temporary_FINI();
    Configure_FINI();
    Optimize_FINI();
    AIR_FINI();
    MDT_FINI();
    CCL_FINI();
  }
}

#ifdef $XCC__h
/**
 * Setup and execute a LIR test.
 */
void
LIR_Test(void (*test)(void));
#endif//$XCC__h

void
LIR_Test(void (*test)(void))
{
  LIR_INIT();
  (*test)();
  LIR_FINI();
}

#if XCC__C
static void
LIR_TEST(void)
{
#include "LIR/LIR_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(LIR_TEST);
  return 0;
}
#endif

