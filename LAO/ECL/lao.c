#ifdef $XCC_h
/*
 * !!!!	lao.c
 *
 * Florent Bouchez (florent.bouchez@kalray.eu).
 * Artur Pietrek   (artur.pietrek@kalray.eu).
 *
 * Copyright 2010 - 2011 Kalray.
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
#include <getopt.h>

#include "LIR.h"
#include "CGO.h"

static FILE *input_file = NULL;
static FILE *output_file = NULL;
static char *output_file_name = NULL;
static bool closefile = false;
static bool closefile_output = false;

/* Do not try to do anything fancy, just read minir/tirex and output asm */
static bool option_fast_dump = false;

// Are we in debug mode
static bool DEBUG_MODE = false;
/*static bool DEBUG_MODE = true;*/

/**
 *  Print help
 */
static void
printHelp(char *name)
{
  printf("Usage: %s [options]... file...\n", name);
  puts("Options:");
  puts("  -h, --help            Display this help");
  puts("  -d, --debug           Print debug information");
  puts("  -f, --fast-dump       Dump assembly without performing any optimization");
  puts("  -o <file>             Place the output into <file> (stdout if '-' given)");
  puts("  -frandom-seed <seed>");
  puts("  --random-seed <seed>  GCC compatibility option");
  puts("  -fdump-final-insns");
  puts("  --dump-final-insns <file>");
  puts("                        GCC compatibility option, pretty print in file");
  puts("");
  printf("Try running `HELP=1 %s' for more information on activation flags.\n", name);
}


/**
 * Some options passed from gcc need to be modified so that getopt can handle 
 * them.
 */
static void
convertOptions(int argc, char *argv[])
{
  int i,s;
  const char *to_modify[] = {
    "frandom-seed",
    "fdump-final-insns",
  };

  for (i=0; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] != '-')
      for (s=0; s < 2; s++) {
        if (!strncmp(argv[i] + 1, to_modify[s], strlen(to_modify[s]))) {
          argv[i][1] = '-'; /* convert to --<option without first letter> */
        }
      }
  }
}


/**
 *  Parse options using getopt
 */
static void
parseOptions(int argc, char *argv[])
{
  int c;
  do
  {
    static struct option long_options[] =
    {
      {"help",             no_argument,       0, 'h'},
      {"debug",            no_argument,       0, 'd'},
      {"output",           required_argument, 0, 'o'},
      {"fast-dump",        no_argument,	      0, 'f'},
      {"random-seed",      required_argument, 0, 's'},
      {"dump-final-insns", required_argument, 0, 'i'},
      {0, 0, 0, 0}
    };
    int option_index = 0;
    c = getopt_long (argc, argv, "hdo:f",
                     long_options, &option_index);
    switch (c)
    {
    case 'h':
      printHelp(argv[0]);
      exit(EXIT_SUCCESS);
    case 'd':
      DEBUG_MODE = true;
      break;
    case 'o':
      if(!strcmp(optarg, "-")) {
        output_file = stdout;
        break;
      }
      output_file_name = optarg;
      break;
    case 'f':
      option_fast_dump = true;
      break;
    case 's':
      DEBUG_MODE && printf("Opts: not using random-seed\n");
      break; 
    case 'i':
      DEBUG_MODE && printf("Opts: not using dump-final-insns\n");
      break; 
    default:
      break;
    }
  } while (c!=-1); // Detect the end of the options.

  if (optind < argc)
    while (optind < argc)
    {
      if(!strcmp(argv[optind],"-"))
      {
        if (input_file == NULL) {
          input_file = stdin;
          closefile = false;
        }
      } else if (input_file == NULL) {
//        printf("Opening file '%s' for reading.\n", argv[optind]);
        if ((input_file = fopen(argv[optind],"rb")) == NULL)
        {
          fprintf(stderr, "Error while opening file %s!\n", argv[optind]);
          exit(EXIT_FAILURE);
        }
        closefile = true;
      }
      ++optind;
    }

  if (input_file == NULL)
  {
    fprintf(stderr, "Error: no input file specified!\n");
    printHelp(argv[0]);
    exit(EXIT_FAILURE);
  }
}

static void
printDebug(Program program, Minir minir)
{
  // TODO: these flags should be in options
  OptimizeFlags rewriting = 0;
  OptimizeFlags conversion = 0;
  OptimizeFlags coalescing = 0;

  //DataAll dataAll;
  DataList dataList;

  Program_FOREACH_Procedure(program, procedure)
  {
    /*        printf("Pretty-print Procedure*****************************\n");*/
    /*        Procedure_pretty(procedure, stdout);*/
    Memory memory = Procedure_memory(procedure);
    /*        printf("*** Building Code Regions...\n");*/
    /*        Procedure_buildCodeRegions(procedure);*/
    Procedure_FOREACH_CodeRegion(procedure, codeRegion)
    {
      DEBUG_MODE && printf("*** Printing Code Region...\n");
      CodeRegion_pretty(codeRegion, stdout);

      CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock)
      {
        BasicBlockEdge edge = BasicBlock_getFallEdge(basicBlock);
        if(edge)
          printf("Fallthrough of BB %s is %s\n",
                 Label_name(BasicBlock_labels(basicBlock)[0]),
                 Label_name(BasicBlock_labels(
                              BasicBlockEdge_destBlock(edge)
                              )[0])
                 );
      } CodeRegion_ENDEACH_BasicBlock;

      if (0)
      {
        SSAForm ssaForm = SSAForm_construct(memory, codeRegion, false);
        PFA_TRACE && fprintf(PFA_TRACE, "*** SSA Conversion=")
            && OptimizeConversion_prettyFlags(conversion, PFA_TRACE)
            && fprintf(PFA_TRACE, "\n");
        PFA_TRACE && fprintf(PFA_TRACE, "*** SSA Coalescing=")
            && OptimizeCoalescing_prettyFlags(coalescing, PFA_TRACE)
            && fprintf(PFA_TRACE, "\n");
        if (rewriting != 0) {
          Accelerator accelerator = Accelerator_new(memory, codeRegion);
          Accelerator_optimize(accelerator);
          Accelerator_delete(accelerator);
        }
        SSAForm_destruct(ssaForm);
      }

      // TODO: use SSAForm_constructAfterMinir
      if(Minir_isSSA(minir))
      {
        Memory memory = Procedure_memory(procedure);
        SSAForm ssaForm = SSAForm_new(memory, codeRegion);
        Dominance dominance;
        CodeRegion_setSSAForm(codeRegion, ssaForm);
        dominance = SSAForm_makeDominance(ssaForm,
                                          DominanceFlag_Frontiers
                                          );
        //SSAForm_pretty(ssaForm, stdout);
        SSAForm_delete(ssaForm); //FIXME: really??
      }
    } Procedure_ENDEACH_CodeRegion;
    dataList = Procedure_dataList(procedure);
    if(!DataList_isEmpty(dataList))
      DataList_pretty(dataList, stdout);
  } Program_ENDEACH_Procedure;
  //dataAll = Program_dataAll(program);
  dataList = Program_dataList(program);
  if(!DataList_isEmpty(dataList))
    DataList_pretty(dataList, stdout);
}

/**
 *  Do whatever you want to try on the program here (test mode).
 */
static void
Lao_processProgram(Program this)
{
  Optimize optimize = Program_optimize(this);
  // TODO: understand why Sreedhar & Boissinot do not work
  *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;

  // Warning: with SeqCopies, will fail if variables interfere in one SSA-web
/*  *Optimize__COALESCING(optimize) = OptimizeCoalescing_SeqCopies;*/

  Program_FOREACH_Procedure(this, procedure) { 
#ifdef FLO_DEBUG
    fprintf(stderr, "%s:%d procedure before spilling:\n", __FILE__, __LINE__);
    Procedure_pretty(procedure, stderr);
#endif//FLO_DEBUG

    Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
      SSAForm ssaForm = CodeRegion_ssaForm(codeRegion);
      if(false) {
      // Perform spill everywhere on every variable
        Spill_allVariableEverywhere(codeRegion);
      } else {

        // Perform spill everywhere on global variables
        // and assign variables basicblock per basicblock using a
        // Belady-like algorithm.
        Spill_allGlobalEverywhereLocalBelady(codeRegion);
      }

      //PFA_TRACE = stdout;

#ifdef FLO_DEBUG
      fflush(stdout);
      fprintf(stderr, "%s:%d coderegion before SSA destruction:\n", __FILE__, __LINE__);
      CodeRegion_pretty(codeRegion, stderr);
      fflush(stderr);
#endif

      // Destruct colored SSA form, with φ-operands in memory
      SSAForm_destruct(ssaForm);

#ifdef FLO_DEBUG
      fflush(stdout);
      fprintf(stderr, "%s:%d coderegion after SSA destruction:\n", __FILE__, __LINE__);
      CodeRegion_pretty(codeRegion, stderr);
      fflush(stderr);
#endif//FLO_DEBUG

    } Procedure_ENDEACH_CodeRegion;

#ifdef FLO_DEBUG
    fflush(stdout);
    fprintf(stderr, "%s:%d Whole procedure after ssa+spilling+ssa destruct:\n", __FILE__, __LINE__);
    Procedure_pretty(procedure, stderr);
    fflush(stderr);
#endif//FLO_DEBUG

  } Program_ENDEACH_Procedure;


  // Determining offset of local symbol in procedures
  Program_FOREACH_Procedure(this, procedure) { 
    Frame frame = Procedure_frame(procedure);
#ifdef FLO_DEBUG
    fprintf(stderr, "%s:%d Resolving symbols in Procedure %s\n", __FILE__, __LINE__, Procedure_name(procedure));
#endif//FLO_DEBUG
    Frame_resolveSpillSymbols(frame);
    Procedure_updateFrameSize(procedure);
    Frame_resolveFormalParameters(frame);

#ifdef FLO_DEBUG
    fprintf(stderr, "%s:%d Before Expanding generics and simulated of %s\n", __FILE__, __LINE__, Procedure_name(procedure));
    Procedure_pretty(procedure, stderr);
    fflush(stderr);
#endif//FLO_DEBUG

    Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
      // Expand Generics and Simulated
      CodeRegion_expandGenericsSimulated(codeRegion);
    } Procedure_ENDEACH_CodeRegion;

#ifdef FLO_DEBUG
    fflush(stdout);
    fprintf(stderr, "%s:%d Whole procedure after all:\n", __FILE__, __LINE__);
    Procedure_pretty(procedure, stderr);
    fflush(stderr);
#endif//FLO_DEBUG

  } Program_ENDEACH_Procedure;

#ifdef FLO_DEBUG
  fprintf(stderr, "%s:%d Checking if some temps are symbols:\n", __FILE__, __LINE__);
  Program_FOREACH_Procedure(this, procedure) {
    fprintf(stderr, "        In procedure %s\n", Procedure_name(procedure));
    Procedure_pretty(procedure, stderr);
    Procedure_FOREACH_Temporary(procedure, temporary) {
      if(Temporary_isSymbol(temporary)) {
        fprintf(stderr, "           Temporary is a symbol: ");
        Temporary_pretty(temporary, stderr);
      }
    } Procedure_ENDEACH_Temporary;
  } Program_ENDEACH_Procedure;
#endif//FLO_DEBUG
}

/**
 *  Entry point of the LAO compiler.
 */
int main(int argc, char *argv[])
{
  LIR_INIT();
#ifdef YAML_ENABLED
  {

#ifdef FLO_DEBUG
    PFA_TRACE = stderr;
    BWLU_TRACE = stderr;
    CGO_TRACE = stderr;
#endif//FLO_DEBUG

    convertOptions (argc, argv);
    parseOptions (argc, argv);
    {
      Minir minir = Minir_new(Memory_Root, input_file);
      Program program;
      while( (program = Minir_nextProgram(minir)) ) {
        if (!option_fast_dump) {
          if (DEBUG_MODE)
            printDebug(program, minir);
          if(Minir_isSSA(minir))
            SSAForm_constructAfterMinir(minir);
          else {
            // Go to SSA
            Memory memory = Program_memory(program);
            Optimize optimize = Program_optimize(program);
#ifdef FLO_DEBUG
            fprintf(stderr, "***\n");
            fprintf(stderr, "Converting program to SSA form using default settings.\n");
            fprintf(stderr, "***\n");
#endif//FLO_DEBUG
            *Optimize__CONVERSION(optimize) = OptimizeConversion_default;
            *Optimize__COALESCING(optimize) = OptimizeCoalescing_Boissinot;
            *Optimize__COALESCING(optimize) = OptimizeCoalescing_Congruence;
            Program_FOREACH_Procedure(program, procedure) {
              Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
                SSAForm_construct(memory, codeRegion, true);
                {
                  // Not passing a global table, so that consumers will be 
                  // constructed also for local variables
                  TemporaryTable globalTable = CodeRegion_makeGlobalTable(codeRegion,
                      TemporaryFlag_Constant|TemporaryFlag_RenameOK,
                      TemporaryFlag_RenameOK);
                  // Note: Rename_OK flag is set to 1 for all SSA variables 
                  // during SSA construction
#ifdef FLO_DEBUG
                  fprintf(stderr, "%s:%d Global table ", __FILE__, __LINE__);
                  TemporaryTable_pretty(globalTable, stderr);
                  fprintf(stderr, "\n");
#endif//FLO_DEBUG
                  CodeRegion_makeConsumers(codeRegion, globalTable);
                }

              } Procedure_ENDEACH_CodeRegion;
            } Program_ENDEACH_Procedure;
          }
#ifdef FLO_DEBUG
          fprintf(stderr, "***\n");
          fprintf(stderr, "Processing Tirex program in LAO.\n");
          fprintf(stderr, "***\n");
#endif//FLO_DEBUG
          Lao_processProgram(program);
        }

        // Write assembly
        if(!output_file) {
          if(!output_file_name) {
            output_file = fopen("out.s", "w+");
          } else {
            output_file = fopen(output_file_name, "w+");
          }
          if(!output_file) {
            perror("Error while opening file!\n");
            exit(EXIT_FAILURE);
          }
          closefile_output = true;
        }
        Program_asmPrint(program, output_file);
        if(closefile_output) fclose(output_file);
      }
      Minir_delete(minir);
    }
  }
#else//YAML_ENABLED
  fprintf(stderr, "LAO has been compiled without yaml support.\n");
  fprintf(stderr, "Please add YAML=<path to libyaml> to the build command!\n");
#endif//YAML_ENABLED
  LIR_FINI();
  return 0;
}
