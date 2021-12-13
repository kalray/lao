#ifdef $XCC_h
/*
 * !!!!	AIR.xcc
 *
 * Author: Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2001 - 2007 STMicroelectronics
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
 * @defgroup AIR Assembler Intermediate Representation
 * @ingroup TSL
 * @brief Low-level representation for use by the ISS and JIT compiler,
 * includes in particular the Scoreboard basic block scheduler and the
 * InterBlock postpass scheduler.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "MDT/MDT_.h"
#include "AIR/AIR.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CCL.h"
#include "TSL.h"
#include "MDT/MDT.h"
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Include all the exported AIR headers.
 */
#include "AIR/Label.h"
#include "AIR/Symbol.h"
#include "AIR/Literal.h"
#include "AIR/Frame.h"
#include "AIR/Instruction.h"
#include "AIR/CodeTrace.h"
#include "AIR/TopDown.h"
#include "AIR/Scoreboard.h"
#include "AIR/NativeBlock.h"
#include "AIR/NativeStream.h"
#include "AIR/Trampoline.h"
#endif//$XCC_h

#ifdef $XCC__h
/*
 * Include all the shared AIR headers.
 */
#include "AIR/Label_.h"
#include "AIR/Symbol_.h"
#include "AIR/Literal_.h"
#include "AIR/Frame_.h"
#include "AIR/Instruction_.h"
#include "AIR/CodeTrace_.h"
#include "AIR/TopDown_.h"
#include "AIR/Scoreboard_.h"
#include "AIR/NativeBlock_.h"
#include "AIR/NativeStream_.h"
#include "AIR/Trampoline_.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * File handle used to print the AIR traces.
 */
extern FILE *AIR_TRACE;
#ifdef _NTRACE
#define AIR_TRACE NULL
#endif//NULL
#endif//$XCC_h

#undef AIR_TRACE
FILE *AIR_TRACE = NULL;

#ifdef $XCC_h
extern int FORCE_PIPELINE;
extern int FORCE_ONLYCOUNT;
#endif//$XCC_h

int FORCE_PIPELINE;
int FORCE_ONLYCOUNT;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Initialize the AIR library.
 */
void
AIR_INIT(void);
#endif//$XCC_h

static int AIR_Initialized = 0;

void
AIR_INIT(void)
{
  if (AIR_Initialized++ == 0) {
    FORCE_ONLYCOUNT = 0;
    if (GETENV("FORCE_ONLYCOUNT")) {
      sscanf(GETENV("FORCE_ONLYCOUNT"), "%d", &FORCE_ONLYCOUNT);
      fprintf(stderr, "*** FORCE_ONLYCOUNT=%d\n", FORCE_ONLYCOUNT);
    }
    FORCE_PIPELINE = 0;
    if (GETENV("FORCE_PIPELINE")) {
      sscanf(GETENV("FORCE_PIPELINE"), "%d", &FORCE_PIPELINE);
      fprintf(stderr, "*** FORCE_PIPELINE=%d\n", FORCE_PIPELINE);
    }
    if (GETENV("AIR_TRACE")) {
      int file = 0;
      sscanf(GETENV("AIR_TRACE"), "%d", &file);
      if (file == 1) AIR_TRACE = stdout;
      else if (file == 2) AIR_TRACE = stderr;
    }
    AIR_TRACE && fprintf(AIR_TRACE, "*** AIR INIT\n");
    CCL_INIT();
    MDT_INIT();
    Label_INIT();
    Symbol_INIT();
    Literal_INIT();
    Frame_INIT();
    Instruction_INIT();
    CodeTrace_INIT();
    TopDown_INIT();
    Scoreboard_INIT();
    NativeBlock_INIT();
    NativeStream_INIT();
    Trampoline_INIT();
  }
}

#ifdef $XCC_h
/**
 * Finalize the AIR library.
 */
void
AIR_FINI(void);
#endif//$XCC_h

void
AIR_FINI(void)
{
  if (--AIR_Initialized == 0) {
    AIR_TRACE && fprintf(AIR_TRACE, "*** AIR FINI\n");
    Trampoline_FINI();
    NativeStream_FINI();
    NativeBlock_FINI();
    TopDown_FINI();
    Scoreboard_FINI();
    CodeTrace_FINI();
    Instruction_FINI();
    Frame_FINI();
    Literal_FINI();
    Symbol_FINI();
    Label_FINI();
    MDT_FINI();
    CCL_FINI();
  }
}

#ifdef $XCC__h
/*
  AIR_Test -- Setup and execute a AIR test.
 */
void
AIR_Test(void (*test)(void));
#endif//$XCC__h

void
AIR_Test(void (*test)(void))
{
  AIR_INIT();
  (*test)();
  AIR_FINI();
}

#if XCC__C
static void
AIR_TEST(void)
{
#include "AIR/AIR_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(AIR_TEST);
  return 0;
}
#endif

