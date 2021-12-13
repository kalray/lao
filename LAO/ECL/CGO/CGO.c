#ifdef $XCC_h
/*
 * !!!!	CGO.xcc
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
 * @defgroup CGO Code Generator Optimizations
 * @ingroup ECL
 * @brief Optimizations for code generation. Misses the SSA register allocation
 * of INRIA.
 */
#endif//$XCC_h


#include "CGO/CGO_.h"

#ifdef $XCC__h
#include "CGO/CGO.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CCL.h"
#include "ECL.h"
#include "MDT/MDT.h"
#include "AIR/AIR.h"
#include "LIR/LIR.h"
#include "PFA/PFA.h"
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include "CGO/ControlFlow.h"
#include "CGO/Predicator.h"
#include "CGO/Accelerator.h"
#include "CGO/PrePass.h"
#include "CGO/Allocator.h"
#include "CGO/ELinearScan.h"
#include "CGO/DomTreeScan.h"
#include "CGO/PostPass.h"
#include "CGO/Spill.h"
#endif//$XCC_h

#ifdef $XCC__h
#include "CGO/ControlFlow_.h"
#include "CGO/Predicator_.h"
#include "CGO/Accelerator_.h"
#include "CGO/PrePass_.h"
#include "CGO/Allocator_.h"
#include "CGO/ELinearScan_.h"
#include "CGO/DomTreeScan_.h"
#include "CGO/PostPass_.h"
#include "CGO/Spill_.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * File handle used to print the CGO traces.
 */
extern FILE *CGO_TRACE;
#ifdef _NTRACE
#define CGO_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef CGO_TRACE 
FILE *CGO_TRACE = NULL;

#ifdef $XCC_h
/**
 * File handle used to print the BWLU traces.
 */
extern FILE *BWLU_TRACE;
#ifdef _NTRACE
#define BWLU_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef BWLU_TRACE 
FILE *BWLU_TRACE = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Initialize the CGO library.
 */
void
CGO_INIT(void);
#endif//$XCC_h

static int CGO_Initialized = 0;

void
CGO_INIT(void)
{
  if (CGO_Initialized++ == 0) {
    if (GETENV("CGO_TRACE")) {
      int file = 0;
      sscanf(GETENV("CGO_TRACE"), "%d", &file);
      if (file == 1) CGO_TRACE = stdout;
      else if (file == 2) CGO_TRACE = stderr;
    }
    if (GETENV("BWLU_TRACE")) {
      int file = 0;
      sscanf(GETENV("BWLU_TRACE"), "%d", &file);
      if (file == 1) BWLU_TRACE = stdout;
      else if (file == 2) BWLU_TRACE = stderr;
    }
    CCL_INIT();
    MDT_INIT();
    AIR_INIT();
    LIR_INIT();
    PFA_INIT();
    ControlFlow_INIT();
    Predicator_INIT();
    Accelerator_INIT();
    PrePass_INIT();
    Allocator_INIT();
    ELinearScan_INIT();
    DomTreeScan_INIT();
    PostPass_INIT();
  }
}

#ifdef $XCC_h
/**
 * Finalize the CGO library.
 */
void
CGO_FINI(void);
#endif//$XCC_h

void
CGO_FINI(void)
{
  if (--CGO_Initialized == 0) {
    PostPass_FINI();
    DomTreeScan_FINI();
    ELinearScan_FINI();
    Allocator_FINI();
    PrePass_FINI();
    Accelerator_FINI();
    Predicator_FINI();
    ControlFlow_FINI();
    PFA_FINI();
    LIR_FINI();
    AIR_FINI();
    MDT_FINI();
    CCL_FINI();
  }
}

#ifdef $XCC__h
/**
 * Setup and execute a CGO test.
 */
void
CGO_Test(void (*test)(void));
#endif//$XCC__h

void
CGO_Test(void (*test)(void))
{
  CGO_INIT();
  (*test)();
  CGO_FINI();
}

#if XCC__C
static void
CGO_TEST(void)
{
#include "CGO/CGO_.c"
}

int
main(int argc, char **argv)
{
  CGO_Test(CGO_TEST);
  return 0;
}
#endif

