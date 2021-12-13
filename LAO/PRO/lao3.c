#ifdef $XCC_h
/*
 * !!!!	lao3.c
 *
 * Benoit Dupont de Dinechin   (benoit.dinechin@kalray.eu).
 * Duco van Amstel (duco.van-amstel@kalray.eu).
 *
 * Copyright 2010 - 2011 Kalray S.A.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#include <error.h>
#include <errno.h>
#include <getopt.h>

#include "BSL.h"
#include "CCL.h"
#include "CAL.h"
#include "MDT.h"
#include "AIR.h"
#include "LIR.h"
#include "PFA.h"
#include "CGO.h"
#include "XFA.h"
#include "SCD.h"
#include "JIT.h"

static bool debug_dump = false;

static void
lao_init(void)
{
  BSL_INIT();
  CCL_INIT();
  CAL_INIT();
  MDT_INIT();
  AIR_INIT();
  LIR_INIT();
  PFA_INIT();
  CGO_INIT();
  XFA_INIT();
  SCD_INIT();
}

static void
lao_fini(void)
{
  SCD_FINI();
  XFA_FINI();
  CGO_FINI();
  PFA_FINI();
  LIR_FINI();
  AIR_FINI();
  MDT_FINI();
  CAL_FINI();
  CCL_FINI();
  BSL_FINI();
}

static void
lao_reset(void)
{
  IndexedFactory_reset(Label_Factory);
  IndexedFactory_reset(Symbol_Factory);
  IndexedFactory_reset(Temporary_Factory);
  IndexedFactory_reset(Operation_Factory);
  IndexedFactory_reset(BasicBlock_Factory);
  IndexedFactory_reset(LoopScope_Factory);
}


void optimizeSSAForm(Program program, Tirex tirex)
{
  if (debug_dump) {
    fprintf(stderr, "\n--Before SSAForm--\n");
    Program_pretty(program, stderr);
  }
  Program_FOREACH_Procedure(program, procedure) {
    Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
      SSAForm_construct(Procedure_memory(procedure), codeRegion, false);
    } Procedure_ENDEACH_CodeRegion;
  } Program_ENDEACH_Procedure;
}


void optimizePrePass(Program program, Tirex tirex)
{
  Memory memory;
  RegionFlow regionFlow;
  RegionFlowCover regionFlowCover;

  if (debug_dump) {
    fprintf(stderr, "\n--Before PrePass--\n");
    Program_pretty(program, stderr);
  }
  Program_FOREACH_Procedure(program, procedure) {
    memory = Procedure_memory(procedure);

    Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
      // Remove any eventual generic operations that are left before scheduling
      CodeRegion_expandGenericsSimulated(codeRegion);

      // Construct scheduling information about this basic-block
      TemporaryTable globalTable = CodeRegion_makeGlobalTable(codeRegion, TemporaryFlag_Constant, 0);
      Liveness liveness = Liveness_make(memory, codeRegion, globalTable, false);
      regionFlow = RegionFlow_new(memory, codeRegion);
      regionFlowCover = RegionFlowCover_new(memory, regionFlow, 0x0);

      // Schedule each RegionFlow seperatly
      RegionFlowCover_FOREACH_BlockTrace(regionFlowCover, blockTrace) {
        Scheduler scheduler = Scheduler_new(memory, blockTrace);

        Scheduler_schedule(scheduler, regionFlow);

        Scheduler_delete(scheduler);
      } RegionFlowCover_ENDEACH_BlockTrace;

      RegionFlowCover_delete(regionFlowCover);
      RegionFlow_reorderBasicBlocks(regionFlow);
      RegionFlow_finalize(regionFlow);
      RegionFlow_delete(regionFlow);
      Liveness_kill(liveness);
    } Procedure_ENDEACH_CodeRegion;
  } Program_ENDEACH_Procedure;
}


void optimizeRegAlloc(Program program, Tirex tirex)
{
  if (debug_dump) {
    fprintf(stderr, "\n--Before RegAlloc--\n");
    Program_pretty(program, stderr);
  }
  Program_FOREACH_Procedure(program, procedure) {
    Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
      Spill_allGlobalEverywhereLocalBelady(codeRegion);
      CodeRegion_removeDeadCopies(codeRegion);
    } Procedure_ENDEACH_CodeRegion;
  } Program_ENDEACH_Procedure;
}


void optimizePostPass(Program program, Tirex tirex)
{
  Memory memory;
  RegionFlow regionFlow;
  RegionFlowCover regionFlowCover;

  if (debug_dump) {
    fprintf(stderr, "\n--Before PostPass--\n");
    Program_pretty(program, stderr);
  }
  Program_FOREACH_Procedure(program, procedure) {
    memory = Procedure_memory(procedure);

    Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
      // Construct scheduling information about this basic-block
      TemporaryTable globalTable = CodeRegion_makeGlobalTable(codeRegion, TemporaryFlag_Constant, 0);
      Liveness liveness = Liveness_make(memory, codeRegion, globalTable, false);
      regionFlow = RegionFlow_new(memory, codeRegion);
      regionFlowCover = RegionFlowCover_new(memory, regionFlow, 0x0);

      // Schedule each RegionFlow seperatly
      RegionFlowCover_FOREACH_BlockTrace(regionFlowCover, blockTrace) {
        Scheduler scheduler = Scheduler_new(memory, blockTrace);

        Scheduler_schedule(scheduler, regionFlow);

        Scheduler_delete(scheduler);
      } RegionFlowCover_ENDEACH_BlockTrace;

      RegionFlowCover_delete(regionFlowCover);
      RegionFlow_reorderBasicBlocks(regionFlow);
      RegionFlow_finalize(regionFlow);
      RegionFlow_delete(regionFlow);
      Liveness_kill(liveness);
    } Procedure_ENDEACH_CodeRegion;
  } Program_ENDEACH_Procedure;
}


void performOptimizations(Program program, Tirex tirex)
{
  Selector selector;
  Optimize optimize = Program_optimize(program);
  OptimizeFlags activationFlags = Optimize_ACTIVATION(optimize);

  // Prepare the LIR for optimization
  Program_FOREACH_Procedure(program, procedure) {
    // Construct the code-regions
    Procedure_buildCodeRegions(procedure);

    // Mark basic-blocks when necessary on intermediary labels
    Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
      CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
        bool firstOperation = true;
        BasicBlock_FOREACH_Operation(basicBlock, operation) {
          if (Operator_isLabel(Operation_operator(operation))) {
            Temporary target = Operation_targetTemporary(operation);

            if (firstOperation && (Temporary_isLabel(target))) {
              Label targetLabel = Literal_label(Temporary_LITERAL(target));
              BasicBlock_addLabel(basicBlock, targetLabel);
              BasicBlock_detachOperation(basicBlock, operation);
            } else {
              BasicBlock_setFlags(basicBlock, BasicBlockFlag_GNUASM);
            }
          } else
            firstOperation = false;
        } BasicBlock_ENDEACH_Operation;
      } CodeRegion_ENDEACH_BasicBlock;
    } Procedure_ENDEACH_CodeRegion;
  } Program_ENDEACH_Procedure;

  Program_FOREACH_Procedure(program, procedure) {
    // Expand all multi-operations before starting optimizations
    Procedure_FOREACH_BasicBlock(procedure, basicBlock) {
      selector = BasicBlock_selector(basicBlock);
      BasicBlock_FOREACH_Operation(basicBlock, operation) {
        Tirex_expandToMulti(tirex, operation, selector);
      } BasicBlock_ENDEACH_Operation;
    } Procedure_ENDEACH_BasicBlock;
  } Program_ENDEACH_Procedure;

  // Perform the SSAForm optimization phase
  if (activationFlags & OptimizeActivation_SSAForm)
    optimizeSSAForm(program, tirex);

  // Perform the PrePass optimization phase
  if (activationFlags & OptimizeActivation_PrePass)
    optimizePrePass(program, tirex);

  // Perform the Register Allocation phase
  if (activationFlags & OptimizeActivation_RegAlloc)
    optimizeRegAlloc(program, tirex);

  // Perform the PostPass optimization phase
  if (activationFlags & OptimizeActivation_PostPass)
    optimizePostPass(program, tirex);

  // Finalize the LIR after all optimizations
  Program_FOREACH_Procedure(program, procedure) {
    // Put together all expanded multi operations
    Procedure_FOREACH_BasicBlock(procedure, basicBlock) {
      selector = BasicBlock_selector(basicBlock);
      BasicBlock_FOREACH_Operation(basicBlock, operation) {
        Tirex_fuseMulti(tirex, operation, selector);
      } BasicBlock_ENDEACH_Operation;
    } Procedure_ENDEACH_BasicBlock;
  } Program_ENDEACH_Procedure;
}


int
main(int argc, char *argv[])
{
  lao_init();
#ifdef YAML_ENABLED
  {
    int i;
    char c;
    char *ptr;
    char inFilename[256] = "\0";
    char outFilename[256] = "\0";
    char asmFilename[256] = "\0";
    bool asmDump = false;
    FILE *inputFile = NULL;
    FILE *outFile = NULL;
    FILE *asmFile = NULL;
    FILE *parseFile = NULL;

    /* Test & Debug */
    bool deallocate = false;
    /* Test & Debug */

    if (argc == 1) {
      fprintf(stderr, "Usage :\n \
          \tk1-lao3 [-h|--help]\n \
          \tk1-lao3 [-i|--input FILE] [-o|--output FILE] [-a|--asm]\n \
          \t        [-p|--parse [FILE]]\n");
      exit(EXIT_SUCCESS);
    }

    while (1) {
      static struct option long_options[] =
      {
        {"help",  no_argument,       NULL, 'h'},
        {"deallocate", no_argument, NULL, 'd'},
        {"input", required_argument, NULL, 'i'},
        {"output",   required_argument, NULL, 'o'},
        {"asm",   optional_argument, NULL, 'a'},
        {"parse", optional_argument, NULL, 'p'},
        {0, 0, 0, 0}
      };

      int option_index = 0;
      
      c = getopt_long(argc, argv, "hi:o:a::p::", long_options, &option_index);

      if (c == -1)
        break;

      switch (c) {
        case 'h':
          fprintf(stderr, "Usage :\n \
              \tk1-lao3 [-h|--help]\n \
              \tk1-lao3 [-i|--input FILE] [-o|--output FILE] [-a|--asm FILE]\n \
              \t        [-p|--parse [FILE]]\n");
          exit(EXIT_SUCCESS);
          break;
        case 'i':
          strcpy(inFilename, optarg);
          if (!(inputFile = fopen(inFilename, "r+"))) {
            fprintf(stderr, "Problem while opening the specified input file.\n");
            exit(EXIT_FAILURE);
          }
          break;
        case 'd':
          deallocate = true;
          debug_dump = true;
          break;
        case 'o':
          strcpy(outFilename, optarg);
          if (!(outFile = fopen(outFilename, "w+"))) {
            fprintf(stderr, "Problem while opening the output file for pretty-printing.\n");
            exit(EXIT_FAILURE);
          }
          break;
        case 'a':
          asmDump = true;
          if (optarg != NULL) {
            strcpy(asmFilename, optarg);
            if (!(asmFile = fopen(asmFilename, "w+"))) {
              fprintf(stderr, "Problem while opening the output file for ASM printing.\n");
              exit(EXIT_FAILURE);
            }
          }
          break;
        case 'p':
          if (optarg == NULL) {
            parseFile = stdout;
          } else {
            if (!(parseFile = fopen(optarg, "w+"))) {
              fprintf(stderr, "Problem while opening the output file for the post-parse dumping.\n");
              exit(EXIT_FAILURE);
            }
          }
          break;
        default:
          fprintf(stderr, "Encountered unknown option %c\n", c);
          fprintf(stderr, "Usage :\n \
              \tk1-lao3 [-h|--help]\n \
              \tk1-lao3 [-i|--input FILE] [-o|--output FILE] [-a|--asm FILE]\n \
              \t        [-p|--parse [FILE]]\n");
          exit(EXIT_FAILURE);
          break;
      }
    }

    if (!inputFile) {
      fprintf(stderr, "No input file is specified. Use the -i|--input FILE argument.\n");
      exit(EXIT_FAILURE);
    }

    if (asmDump && (asmFilename[0] == '\0')) {
      strcpy(asmFilename, inFilename);
      if ((ptr = strrchr(asmFilename, '.')))
        strcpy(ptr, ".s");
      else
        strcat(asmFilename, ".s");

      if (!(asmFile = fopen(asmFilename, "w+"))) {
        fprintf(stderr, "Problem while opening the assembler output file for Tirex \
            from '%s'\n", asmFilename);
        exit(EXIT_FAILURE);
      }
    }
    lao_reset();

    // Prepare the Tirex object for parsing
    Tirex tirex = Tirex_new(Memory_Root, inputFile);
    Program program;
    
    // While the input-file contains YAML documents
    // parse them to create LIR representations.
    while ((program = Tirex_makeProgram(tirex))){
      if (deallocate) {
        Deallocator_deallocate(program);

        Optimize optimize = Program_optimize(program);
        *Optimize__ACTIVATION(optimize) |= OptimizeActivation_PrePass;
        *Optimize__ACTIVATION(optimize) |= OptimizeActivation_RegAlloc;
        *Optimize__ACTIVATION(optimize) |= OptimizeActivation_SSAForm;
        
        performOptimizations(program, tirex);
      } else {
        // Pretty-print the LIR when requested
        if (outFile)
          Program_pretty(program, outFile);

        // Do optimizations on the LIR
        performOptimizations(program, tirex);
      }

      // Pretty-print the optimized LIR when requested
      if (outFile)
        Program_pretty(program, outFile);

      // Dump the objects to the assembler file when requested
      if (asmDump) {
        strcpy(asmFilename, inFilename);
        if ((ptr = strrchr(asmFilename, '.')))
          strcpy(ptr, "-lao.s");
        else
          strcat(asmFilename, "-lao.s");

        // Dump the assembler representation
        Program_asmPrint(program, asmFile);

        // Finalize the dumping of this Tirex document
        fclose(asmFile);
        Program_delete(program);
      }
    }

    // Destroy the Tirex object used for parsing
    Tirex_delete(tirex);
  }
#else//!YAML_ENABLED
  error(EXIT_FAILURE, 0, "need to build with libyaml");
#endif//YAML_ENABLED
  lao_fini();
  return 0;
}

