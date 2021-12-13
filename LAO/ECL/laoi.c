#ifdef $XCC_h
/*
 * !!!!	laoi.c
 *
 * Artur Pietrek   (artur.pietrek@kalray.eu)
 *
 * Copyright 2011 Kalray
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#include <errno.h>
#include <stdio.h>

// #define CYCLE_COUNT

#ifdef CYCLE_COUNT
#include "utils/Perfs.h"
#include "utils/Verbose.h"
#endif//CYCLE_COUNT

#include "LIR.h"
#include "CGO.h"
#include "JIT/JIT.h"

#include "laoi_cmdline.h"

// Are we in debug mode
static bool DEBUG_MODE = false;

int 
main(int argc, char **argv) 
{
  struct laoi_args_info               args_info;
  struct laoi_cmdline_params          cmdline_params;
  int                                 laoi_argc;
  char                                **laoi_argv;  
  char                                all_for_ee = 0;
  char                                *filename = NULL;
  FILE                                *minir_file = NULL;
  char                                **argv_to_pass = NULL;
  int                                 argc_to_pass = 0;
#ifdef CYCLE_COUNT
  TIMER_DATA(startupCounter);
  TIMER_PROLOGUE_VERBOSE(stderr, startupCounter);
  TIMER_START(startupCounter);
#endif//CYCLE_COUNT
  /* Allocate max size for the laoi command line options */
  // argc+1 -- workaround the bug in all tools where argv[argc] != NULL while it should
  laoi_argv = (char**)malloc((argc+1) * sizeof(char*));
  laoi_argv[0] = argv[0];
  laoi_argc = 1;
  for( unsigned int i = 1 ; i < argc ; ++i ) {
    if( strcmp(argv[i],"--") == 0 ) 
    {
        /* After -- there's a MinIR filename -- all the options are 
         * for the execution engine... */
        filename = argv[i+1];
        argv_to_pass = &argv[i+1];
        argc_to_pass = argc-i-1;
        break;
    } else
//       if( (strcmp(argv[i],"-h") == 0)
//           ||  (strcmp(argv[i],"--help") == 0)  
//           ||  (strcmp(argv[i],"-V") == 0)  
//           ||  (strcmp(argv[i],"--version") == 0)
//           ||  (strcmp(argv[i],"-d") == 0)
//           ||  (strcmp(argv[i],"--debug") == 0)
//           ||  (strcmp(argv[i],"-S") == 0)
//           ||  (strcmp(argv[i],"--stable") == 0)
//           ||  (strcmp(argv[i],"--step") == 0)
//           ||  (strcmp(argv[i],"-s") == 0)
//           ||  (strcmp(argv[i],"--jit") == 0)
//           ||  (strcmp(argv[i],"-j") == 0))
    {
      laoi_argv[laoi_argc] = argv[i];
      laoi_argc++;
    } 
    
  }
  // workaround of the bug
  laoi_argv[laoi_argc] = NULL;
 
  cmdline_params.override = 0;
  cmdline_params.initialize = 1;
  cmdline_params.check_required = 1;
  cmdline_params.check_ambiguity = 0;
  cmdline_params.print_errors = 0;
  if (laoi_cmdline_ext(laoi_argc, laoi_argv, &args_info,&cmdline_params) != 0)
    exit(1);
  if (args_info.help_given)
    laoi_cmdline_print_help();
  if (args_info.version_given)
    laoi_cmdline_print_version();
  
  DEBUG_MODE = args_info.debug_given;
  if (DEBUG_MODE) JIT_TRACE = stderr;
   
  free(laoi_argv);
  
  if (filename == NULL) {
    perror("Error: No input MinIR file name given\n");
    laoi_cmdline_print_help();
    exit(EXIT_FAILURE);
  }
  
  DEBUG_MODE && printf("Opening file %s\n", filename);
  minir_file = fopen(filename, "rb");
  Except_THROWIF(!minir_file, "Cannot open MinIR file!");  
  
  LIR_INIT();
#ifdef YAML_ENABLED
  {
    PFA_TRACE = stdout;
    BWLU_TRACE = stdout;
    CGO_TRACE = stdout;
    {
#ifdef CYCLE_COUNT
      TIMER_DATA(interpreterCounter);
#endif//CYCLE_COUNT
      int result = -1;
      Program program = NULL;
      Minir minir = Minir_new(Memory_Root, minir_file);
      Minir_KEEP_INDEX(minir) = false;
      program = Minir_nextProgram(minir);
      Except_THROWIF(!program, "Cannot read program");
//       Minir_OPERATIONCOUNTER(minir);
      fclose(minir_file);
      if(Minir_isSSA(minir)) 
      {        
        SSAForm_constructAfterMinir(minir);
      } /*else {
        // Go to SSA
        Memory memory = Program_memory(program);
        Optimize optimize = Program_optimize(program);

        *Optimize__CONVERSION(optimize) = OptimizeConversion_default;
        *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
        *Optimize__COALESCING(optimize) = OptimizeCoalescing_Congruence;
        Program_FOREACH_Procedure(program, procedure) {
          Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
            SSAForm_construct(memory, codeRegion, true);
            {
              // Not passing a global table, so that consumers will be 
              // constructed also for local variables
              TemporaryTable globalTable = 
                CodeRegion_makeGlobalTable(codeRegion,
                                           TemporaryFlag_Constant |
                                           TemporaryFlag_RenameOK,
                                           TemporaryFlag_RenameOK);
              // Note: Rename_OK flag is set to 1 for all SSA variables 
              // during SSA construction
              CodeRegion_makeConsumers(codeRegion, globalTable);
            }
          } Procedure_ENDEACH_CodeRegion;
        } Program_ENDEACH_Procedure;
      }      */
      Minir_delete(minir);
      
//       {
//         void *codeBuffer = Memory_alloc(Memory_Root, 1024);
//         char *ptr = (char*)codeBuffer;
//         size_t codeSize = 0;
//         Program_FOREACH_Procedure(program, procedure) {
//           codeSize = Compiler_compileProcedure(procedure, ptr);
//           ptr += codeSize;
//         } Program_ENDEACH_Procedure;
//       }
//       return 0;
            

#ifdef CYCLE_COUNT
      TIMER_STOP(startupCounter);
      TIMER_EPILOGUE_VERBOSE(stderr, startupCounter);
      
      TIMER_PROLOGUE_VERBOSE(stderr, interpreterCounter);
      TIMER_START(interpreterCounter);
#endif//CYCLE_COUNT
      JIT_INIT();
      {
        ExecutionEngine EE = ExecutionEngine_new(Memory_Root, program,
                                                 args_info.jit_given,
                                                 argc_to_pass, argv_to_pass);
        if ( args_info.treshold_given)
          ExecutionEngine_setTreshold(EE, args_info.treshold_arg);
        if ( args_info.step_given )
          result = ExecutionEngine_stepRun(EE);
        else
          result = ExecutionEngine_run(EE);      
      
        if ( args_info.stable_given ) {
          ExecutionEngine_dumpSymbolTable(EE, stderr);
        }
        
        ExecutionEngine_delete(EE);
      }
#ifdef CYCLE_COUNT
      TIMER_STOP(interpreterCounter);
      TIMER_EPILOGUE_VERBOSE(stderr, interpreterCounter);
#endif//CYCLE_COUNT
      return result;
    }
  }
#else//YAML_ENABLED
  perror("LAO has been compiled without yaml support.\n");
  perror("Please add YAML=<path to libyaml> to the build command!\n");
#endif//YAML_ENABLED
  return 0;
}
