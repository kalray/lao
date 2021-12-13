#ifdef $XCC_h
/*
 * !!!!	CAL.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2007 STMicroelectronics.
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
 * @defgroup CAL Combinatorial Algorithm Library
 * @ingroup CDT
 * @brief 
 *
 * Most interesting are the Network module, that implements primal and dual
 * network simplex for solving network flow algorithms, and the LProgram
 * module, a linear programming wrapper that abstracts the differences between
 * CPLEX and GLPK.  
 */
#endif//$XCC_h

#ifdef $XCC_h
/*
 */
#endif//$XCC_h


#include "CAL/CAL_.h"

#ifdef $XCC__h
#include "CAL/CAL.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CDT.h"
#include "CCL/CCL.h"
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Include all the exported CAL headers.
 */
#include "CAL/DGraph.h"
#include "CAL/Dominators.h"
#include "CAL/Hierarchy.h"
#include "CAL/Network.h"
#include "CAL/LProgram.h"
#endif//$XCC_h

#ifdef $XCC__h
/*
 * Include all the shared CAL headers.
 */
#include "CAL/DGraph_.h"
#include "CAL/Dominators_.h"
#include "CAL/Hierarchy_.h"
#include "CAL/Network_.h"
#include "CAL/LProgram_.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * File handle used to print the CAL traces.
 */
extern FILE *CAL_TRACE;
#ifdef _NTRACE
#define CAL_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef CAL_TRACE
FILE *CAL_TRACE = NULL;

#ifdef $XCC_h
/**
 * File handle to output Integer Programming Formulations.
 */
extern FILE *IPF_TRACE;
#ifdef _NTRACE
#define IPF_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef IPF_TRACE
FILE *IPF_TRACE = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Initialize the CAL library.
 */
void
CAL_INIT(void);
#endif//$XCC_h

static int CAL_Initialized = 0;

void
CAL_INIT(void)
{
  if (CAL_Initialized++ == 0) {
    if (GETENV("CAL_TRACE")) {
      int file = 0;
      sscanf(GETENV("CAL_TRACE"), "%d", &file);
      if (file == 1) CAL_TRACE = stdout;
      else if (file == 2) CAL_TRACE = stderr;
    }
    if (GETENV("IPF_TRACE")) {
      int file = 0;
      sscanf(GETENV("IPF_TRACE"), "%d", &file);
      if (file == 1) IPF_TRACE = stdout;
      else if (file == 2) IPF_TRACE = stderr;
    }
    if (CAL_TRACE) fprintf(CAL_TRACE, "*** CAL INIT\n");
    CCL_INIT();
    DGraph_INIT();
    Dominators_INIT();
    Hierarchy_INIT();
    Network_INIT();
    LProgram_INIT();
  }
}

#ifdef $XCC_h
/**
 * Finalize CAL library.
 */
void
CAL_FINI(void);
#endif//$XCC_h

void
CAL_FINI(void)
{
  if (--CAL_Initialized == 0) {
    if (CAL_TRACE) fprintf(CAL_TRACE, "*** CAL FINI\n");
    LProgram_FINI();
    Network_FINI();
    Hierarchy_FINI();
    Dominators_FINI();
    DGraph_FINI();
    CCL_FINI();
  }
}

#ifdef $XCC__h
/**
 * Setup and execute a CAL test.
 */
void
CAL_Test(void (*test)(void));
#endif//$XCC__h

void
CAL_Test(void (*test)(void))
{
  CAL_INIT();
  (*test)();
  CAL_FINI();
}

#if XCC__C
static void
CAL_TEST(void)
{
#include "CAL/CAL_.c"
}

int
main(int argc, char **argv)
{
  CAL_Test(CAL_TEST);
  return 0;
}
#endif

