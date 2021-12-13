#ifdef $XCC_h
/*
 * !!!!	JIT.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com,
 *                            Benoit.Dinechin@kalray.eu).
 * Artur Pietrek (Artur.Pietrek@kalray.eu).
 *
 * Copyright 2010 - 2010 Kalray.
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
 * @defgroup JIT Just-Int-Time compiler
 * @ingroup ECL
 * @brief The JIT (Just-In-Time) compiler library
 */
#endif//$XCC_h

#include "JIT/JIT_.h"

#ifdef $XCC__h
#include "JIT/JIT.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CCL.h"
#include "ECL.h"
#include "MDT/MDT.h"
#include "AIR/AIR.h"
#include "LIR/LIR.h"
#include "PFA/PFA.h"
#include "CGO/CGO.h"
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////



#ifdef $XCC_h
typedef uint32_t reg_t;
//BD3 #include "JIT/Interpreter.h"
//BD3 #include "JIT/ExecutionEngine.h"
#include "JIT/Compiler.h"
//BD3 #include "JIT/Context.h"
#include "JIT/CodeBuffer.h"
extern int trampoline(void *regset,
                      void *stack,
                      void *func
                     );
#endif//$XCC_h

#ifdef $XCC__h
//BD3 #include "JIT/Interpreter_.h"
//BD3 #include "JIT/ExecutionEngine_.h"
#include "JIT/Compiler_.h"
//BD3 #include "JIT/Context_.h"
#include "JIT/CodeBuffer_.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * File handle used to print the JIT traces.
 */
extern FILE *JIT_TRACE;
#ifdef _NTRACE
#define JIT_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef JIT_TRACE
FILE *JIT_TRACE = NULL;

#ifdef $XCC_h
/*
 * Variables used to control JIT in non-release mode.
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Initialize the JIT library.
 */
void
JIT_INIT(void);
#endif//$XCC_h

static int JIT_Initialized = 0;

void
JIT_INIT(void)
{
  if (JIT_Initialized++ == 0) {
    if (GETENV("JIT_TRACE")) {
      int file = 0;
      sscanf(GETENV("JIT_TRACE"), "%d", &file);
      if (file == 1) JIT_TRACE = stdout;
      else if (file == 2) JIT_TRACE = stderr;
    }
    CCL_INIT();
    MDT_INIT();
    AIR_INIT();
    LIR_INIT();
    PFA_INIT();
    CGO_INIT();
    Compiler_INIT();
  }
}

#ifdef $XCC_h
/**
 * Finalize the JIT library.
 */
void
JIT_FINI(void);
#endif//$XCC_h

void
JIT_FINI(void)
{
  if (--JIT_Initialized == 0) {
    Compiler_FINI();
    CGO_FINI();
    PFA_FINI();
    LIR_FINI();
    AIR_FINI();
    MDT_FINI();
    CCL_FINI();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
  /*  MINIR_DEBUG = true;*/
  fprintf(stderr, "TESTING JIT...\n");
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC__h
/**
 * Setup and execute a JIT test.
 */
void
JIT_Test(void (*test)(void));
#endif//$XCC__h

void
JIT_Test(void (*test)(void))
{
  JIT_INIT();
  (*test)();
  JIT_FINI();
}

#if XCC__C
static void
JIT_TEST(void)
{
#include "JIT/JIT_.c"
}

int
main(int argc, char **argv)
{
  JIT_Test(JIT_TEST);
  return 0;
}
#endif

