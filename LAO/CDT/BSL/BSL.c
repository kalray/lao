#ifdef $XCC_h
/*
 * !!!!	BSL.xcc
 *
 * Copyright 2007 STMicroelectronics.
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
 * @defgroup BSL Basic Services Library
 * @ingroup CDT
 * @brief Handles basic services for the LAO compiler: memory allocator, some
 * XML stuff, emulation of 128-bit integers, emulation of 32-bit and 64-bit
 * IEEE floating-point for integer processors.
 */
#endif//$XCC_h

#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/BSL.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CDT.h"
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Include all the exported BSL headers.
 */
#include "BSL/Except.h"
#include "BSL/Int128.h"
#include "BSL/Int256.h"
#include "BSL/Flip16.h"
#include "BSL/Flip32.h"
#include "BSL/Flip64.h"
#include "BSL/Hacker.h"
#include "BSL/Memory.h"
#include "BSL/BBTree.h"
#include "BSL/String.h"
#include "BSL/Buffer.h"
#include "BSL/ETable.h"
#include "BSL/Escape.h"
#include "BSL/XMLCan.h"
#include "BSL/XMLGen.h"
#endif//$XCC_h

#ifdef $XCC__h
/*
 * Include all the shared BSL headers.
 */
#include "BSL/Except_.h"
#include "BSL/Int128_.h"
#include "BSL/Int256_.h"
#include "BSL/Flip16_.h"
#include "BSL/Flip32_.h"
#include "BSL/Flip64_.h"
#include "BSL/Hacker_.h"
#include "BSL/Memory_.h"
#include "BSL/BBTree_.h"
#include "BSL/String_.h"
#include "BSL/Buffer_.h"
#include "BSL/ETable_.h"
#include "BSL/Escape_.h"
#include "BSL/XMLCan_.h"
#include "BSL/XMLGen_.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * File handle used to print the BSL traces.
 */
extern FILE *BSL_TRACE;
#ifdef _NTRACE
#define BSL_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef BSL_TRACE 
FILE *BSL_TRACE = NULL;

#ifdef $XCC_h
/*
 * Variables used to control BSL in non-release mode.
 */
extern int32_t BSL_MEMORY_STATS;
extern int32_t FORCE_POOLED;
#endif//$XCC_h

int32_t BSL_MEMORY_STATS;
int32_t FORCE_POOLED;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Initialize the BSL library.
 */
void
BSL_INIT(void);
#endif//$XCC_h

static int BSL_Initialized = 0;

void
BSL_INIT(void)
{
  if (BSL_Initialized++ == 0) {
    if (GETENV("BSL_TRACE")) {
      int file = 0;
      sscanf(GETENV("BSL_TRACE"), "%d", &file);
      if (file == 1) BSL_TRACE = stdout;
      else if (file == 2) BSL_TRACE = stderr;
    }
    BSL_MEMORY_STATS = _MEMORY_STATS;
    if (GETENV("BSL_MEMORY_STATS")) {
      sscanf(GETENV("BSL_MEMORY_STATS"), "%d", &BSL_MEMORY_STATS);
      fprintf(stderr, "*** BSL_MEMORY_STATS=%d\n", BSL_MEMORY_STATS);
    }
    FORCE_POOLED = -1;
    if (GETENV("FORCE_POOLED")) {
      sscanf(GETENV("FORCE_POOLED"), "%d", &FORCE_POOLED);
      fprintf(stderr, "*** FORCE_POOLED=%d\n", FORCE_POOLED);
    }
    if (BSL_TRACE) fprintf(BSL_TRACE, "*** BSL INIT\n");
    Except_INIT();
    Int128_INIT();
    Int256_INIT();
    Flip16_INIT();
    Flip32_INIT();
    Flip64_INIT();
    Hacker_INIT();
    Memory_INIT();
    BBTree_INIT();
    String_INIT();
    Buffer_INIT();
    ETable_INIT();
    Escape_INIT();
    XMLCan_INIT();
    XMLGen_INIT();
  }
}

#ifdef $XCC_h
/**
 * Finalize BSL library.
 */
void
BSL_FINI(void);
#endif//$XCC_h

void
BSL_FINI(void)
{
  if (--BSL_Initialized == 0) {
    if (BSL_TRACE) fprintf(BSL_TRACE, "*** BSL FINI\n");
    XMLGen_FINI();
    XMLCan_FINI();
    Escape_FINI();
    ETable_FINI();
    Buffer_FINI();
    String_FINI();
    BBTree_FINI();
    Memory_FINI();
    Hacker_FINI();
    Flip64_FINI();
    Flip32_FINI();
    Flip16_FINI();
    Int256_FINI();
    Int128_FINI();
    Except_FINI();
  }
}

#ifdef $XCC__h
/**
 * Setup and execute a BSL test.
 */
void
BSL_Test(void (*test)(void));
#endif//$XCC__h

void
BSL_Test(void (*test)(void))
{
  BSL_INIT();
  (*test)();
  BSL_FINI();
}

#if XCC__C
static void
BSL_TEST(void)
{
#include "BSL/BSL_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(BSL_TEST);
  return 0;
}
#endif

