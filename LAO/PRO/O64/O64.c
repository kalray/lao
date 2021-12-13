#ifdef $XCC_h
/*
 * !!!!	O64.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Christophe Guillon (Christophe.Guillon@st.com).
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
 * @defgroup O64 Open64
 * @ingroup PRO
 * @brief Interface between the LAO and the Open64 CGIR (code generator
 * intermediate representation).
 */
#endif//$XCC_h


#include "O64/O64_.h"

#ifdef $XCC__h
#include "O64/O64.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CCL.h"
#include "CAL.h"
#include "MDT.h"
#include "AIR.h"
#include "LIR.h"
#include "PFA.h"
#include "CGO.h"
#include "PRO.h"
#include "XFA/XFA.h"
#include "SCD/SCD.h"
#endif//$XCC_h

#ifdef $XCC_h
#include "MDT/MDT_.h"  // For use by LAO_INPUT/*/*.input.c files.
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include "O64/CGIR.h"
#include "O64/Interface.h"
#endif//$XCC_h

#ifdef $XCC__h
#include "O64/CGIR_.h"
#include "O64/Interface_.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * File handle used to trace the O64 INPUT.
 */
extern FILE *O64_INPUT;
#ifdef _NTRACE
#define O64_INPUT NULL
#endif//_NTRACE
#endif//$XCC__h

#undef O64_INPUT 
FILE *O64_INPUT = NULL;

#ifdef $XCC__h
/**
 * File handle used to trace the O64 OUTPUT
 */
extern FILE *O64_OUTPUT;
#ifdef _NTRACE
#define O64_OUTPUT NULL
#endif//_NTRACE
#endif//$XCC__h

#undef O64_OUTPUT 
FILE *O64_OUTPUT = NULL;

#ifdef $XCC__h
/**
 * File handle used to trace the O64.
 */
extern FILE *O64_TRACE;
#ifdef _NTRACE
#define O64_TRACE NULL
#endif//_NTRACE
#endif//$XCC__h

#undef O64_TRACE 
FILE *O64_TRACE = NULL;

#ifdef $XCC_h
/**
 * File handle used to print the XML before optimizations.
 */
extern FILE *O64_XMLIN;
#ifdef _NTRACE
#define O64_XMLIN NULL
#endif//_NTRACE
#endif//$XCC_h

#undef O64_XMLIN
FILE *O64_XMLIN = NULL;

#ifdef $XCC_h
/**
 * File handle used to print the XML after optimizations.
 */
extern FILE *O64_XMLOUT;
#ifdef _NTRACE
#define O64_XMLOUT NULL
#endif//_NTRACE
#endif//$XCC_h

#undef O64_XMLOUT
FILE *O64_XMLOUT = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Call the INIT functions of the libraries.
 */
void
O64_Initialize(void);
#endif//$XCC_h

void
O64_Initialize(void)
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
  O64_INIT();
}

#ifdef $XCC_h
/**
 * Open the libraries.
 */
void
O64_Open(void);
#endif//$XCC_h

void
O64_Open(void)
{
  IndexedFactory_reset(Label_Factory);
  IndexedFactory_reset(Symbol_Factory);
  IndexedFactory_reset(Temporary_Factory);
  IndexedFactory_reset(Operation_Factory);
  IndexedFactory_reset(BasicBlock_Factory);
  IndexedFactory_reset(LoopScope_Factory);
}

#ifdef $XCC_h
/**
 * Close the libraries.
 */
void
O64_Close(void);
#endif//$XCC_h

void
O64_Close(void)
{
  IndexedFactory_reset(LoopScope_Factory);
  IndexedFactory_reset(BasicBlock_Factory);
  IndexedFactory_reset(Operation_Factory);
  IndexedFactory_reset(Temporary_Factory);
  IndexedFactory_reset(Symbol_Factory);
  IndexedFactory_reset(Label_Factory);
}

#ifdef $XCC_h
/**
 * Call the FINI functions of the libraries.
 */
void
O64_Finalize(void);
#endif//$XCC_h

void
O64_Finalize(void)
{
  O64_FINI();
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

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * O64 init function.
 */
void
O64_INIT(void);
#endif//$XCC_h

static int O64_Initialized = 0;

void
O64_INIT(void)
{
  Except_DEBUG("Except_DEBUG");
  if (O64_Initialized++ == 0) {
    if (GETENV("O64_INPUT")) {
      int file = 0;
      sscanf(GETENV("O64_INPUT"), "%d", &file);
      if (file == 1) O64_INPUT = stdout;
      else if (file == 2) O64_INPUT = stderr;
    }
    if (GETENV("O64_OUTPUT")) {
      int file = 0;
      sscanf(GETENV("O64_OUTPUT"), "%d", &file);
      if (file == 1) O64_OUTPUT = stdout;
      else if (file == 2) O64_OUTPUT = stderr;
    }
    if (GETENV("O64_TRACE")) {
      int file = 0;
      sscanf(GETENV("O64_TRACE"), "%d", &file);
      if (file == 1) O64_TRACE = stdout;
      else if (file == 2) O64_TRACE = stderr;
    }
    if (GETENV("O64_XMLIN")) {
      int file = 0;
      sscanf(GETENV("O64_XMLIN"), "%d", &file);
      if (file == 1) O64_XMLIN = stdout;
      else if (file == 2) O64_XMLIN = stderr;
    }
    if (GETENV("O64_XMLOUT")) {
      int file = 0;
      sscanf(GETENV("O64_XMLOUT"), "%d", &file);
      if (file == 1) O64_XMLOUT = stdout;
      else if (file == 2) O64_XMLOUT = stderr;
    }
    if (O64_INPUT != NULL) {
      fprintf(O64_INPUT, "#include \"O64.h\"\n");
      fprintf(O64_INPUT, "\n");
      fprintf(O64_INPUT, "#define CGIRLAB(cgirlab)\t(CGIRLAB)(cgirlab)\n");
      fprintf(O64_INPUT, "#define CGIRSYM(cgirsym)\t(CGIRSYM)(cgirsym)\n");
      fprintf(O64_INPUT, "#define CGIRWN(cgirwn)\t(CGIRWN)(cgirwn)\n");
      fprintf(O64_INPUT, "#define CGIRTN(cgirtn)\t(CGIRTN)(cgirtn)\n");
      fprintf(O64_INPUT, "#define CGIROP(cgirop)\t(CGIROP)(cgirop)\n");
      fprintf(O64_INPUT, "#define CGIRBB(cgirbb)\t(CGIRBB)(cgirbb)\n");
      fprintf(O64_INPUT, "#define CGIRLD(cgirld)\t(CGIRLD)(cgirld)\n");
      fprintf(O64_INPUT, "\n");
      fprintf(O64_INPUT, "extern Label label[];\n");
      fprintf(O64_INPUT, "extern Symbol symbol[];\n");
      fprintf(O64_INPUT, "extern Temporary temporary[];\n");
      fprintf(O64_INPUT, "extern Operation operation[];\n");
      fprintf(O64_INPUT, "extern BasicBlock basicBlock[];\n");
      fprintf(O64_INPUT, "extern LoopScope loopScope[];\n");
      fprintf(O64_INPUT, "extern Interface interface;\n");
      fprintf(O64_INPUT, "\n");
      fprintf(O64_INPUT, "void Interface_0(void) {\n");
    }
    Interface_Counter = 0;
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
    CGIR_INIT();
    Interface_INIT();
  }
}

#ifdef $XCC_h
/**
 * O64 fini function.
 */
void
O64_FINI(void);
#endif//$XCC_h

void
O64_FINI(void)
{
  if (--O64_Initialized == 0) {
    Interface_FINI();
    CGIR_FINI();
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
    if (O64_INPUT != NULL) {
      int i;
      fprintf(O64_INPUT, "int main(int argc, char **argv) {\n");
      fprintf(O64_INPUT, "  Interface_Initialize();\n");
      fprintf(O64_INPUT, "  Interface_0();\n");
      for (i = 0; i < Interface_Counter; i++) {
        fprintf(O64_INPUT, "  Interface_%d();\n", i + 1);
      }
      fprintf(O64_INPUT, "  Interface_Finalize();\n");
      fprintf(O64_INPUT, "  return 0;\n");
      fprintf(O64_INPUT, "}\n\n");
      if (IndexedFactory_maxCount(Label_Factory) != 0) {
        fprintf(O64_INPUT, "Label label[%d];\n",
                IndexedFactory_maxCount(Label_Factory));
      }
      if (IndexedFactory_maxCount(Symbol_Factory) != 0) {
        fprintf(O64_INPUT, "Symbol symbol[%d];\n",
                IndexedFactory_maxCount(Symbol_Factory));
      }
      if (IndexedFactory_maxCount(Temporary_Factory) != 0) {
        fprintf(O64_INPUT, "Temporary temporary[%d];\n",
                IndexedFactory_maxCount(Temporary_Factory));
      }
      if (IndexedFactory_maxCount(Operation_Factory) != 0) {
        fprintf(O64_INPUT, "Operation operation[%d];\n",
                IndexedFactory_maxCount(Operation_Factory));
      }
      if (IndexedFactory_maxCount(BasicBlock_Factory) != 0) {
        fprintf(O64_INPUT, "BasicBlock basicBlock[%d];\n",
                IndexedFactory_maxCount(BasicBlock_Factory));
      }
      if (IndexedFactory_maxCount(LoopScope_Factory) != 0) {
        fprintf(O64_INPUT, "LoopScope loopScope[%d];\n",
                IndexedFactory_maxCount(LoopScope_Factory));
      }
      fprintf(O64_INPUT, "\n");
    }
  }
}

#ifdef $XCC__h
/**
 * Setup and execute a O64 test.
 */
void
O64_Test(void (*test)(void));
#endif//$XCC__h

void
O64_Test(void (*test)(void))
{
  O64_INIT();
  (*test)();
  O64_FINI();
}

#if XCC__C
static void
O64_TEST(void)
{
#include "O64/O64_.c"
}

int
main(int argc, char **argv)
{
  O64_Test(O64_TEST);
  return 0;
}
#endif

