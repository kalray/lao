#ifdef $XCC_h
/*
 * !!!!	CCL.xcc
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
 * @defgroup C Container Library
 * @ingroup CDT
 * @brief Zoo of containers, plus the base class Indexed with an immutable
 * IDENTITY field and a user-defined INDEX field.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/CCL.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CDT.h"
#include "BSL/BSL.h"
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Include all the exported CCL headers.
 */
#include "CCL/PQueue.h"
#include "CCL/IArray.h"
#include "CCL/IStack.h"
#include "CCL/IQueue.h"
#include "CCL/PtrSeq.h"
#include "CCL/IDList.h"
#include "CCL/IBList.h"
#include "CCL/TSTree.h"
#include "CCL/PtrSet.h"
#include "CCL/BitSet.h"
#include "CCL/Sparse.h"
#include "CCL/IETree.h"
#include "CCL/HTable.h"
#include "CCL/PtrMap.h"
#include "CCL/Scoped.h"
#include "CCL/Indexed.h"
#endif//$XCC_h

#ifdef $XCC__h
/*
 * Include all the shared CCL headers.
 */
#include "CCL/PQueue_.h"
#include "CCL/IArray_.h"
#include "CCL/IStack_.h"
#include "CCL/IQueue_.h"
#include "CCL/PtrSeq_.h"
#include "CCL/IDList_.h"
#include "CCL/IBList_.h"
#include "CCL/TSTree_.h"
#include "CCL/PtrSet_.h"
#include "CCL/BitSet_.h"
#include "CCL/Sparse_.h"
#include "CCL/IETree_.h"
#include "CCL/HTable_.h"
#include "CCL/PtrMap_.h"
#include "CCL/Scoped_.h"
#include "CCL/Indexed_.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * File handle used to print the CCL traces.
 */
extern FILE *CCL_TRACE;
#ifdef _NTRACE
#define CCL_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef CCL_TRACE 
FILE *CCL_TRACE = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Initialize the CCL library.
 */
void
CCL_INIT(void);
#endif//$XCC_h

static int CCL_Initialized = 0;

void
CCL_INIT(void)
{
  if (CCL_Initialized++ == 0) {
    if (GETENV("CCL_TRACE")) {
      int file = 0;
      sscanf(GETENV("CCL_TRACE"), "%d", &file);
      if (file == 1) CCL_TRACE = stdout;
      else if (file == 2) CCL_TRACE = stderr;
    }
    if (CCL_TRACE) fprintf(CCL_TRACE, "*** CCL INIT\n");
    BSL_INIT();
    PQueue_INIT();
    IArray_INIT();
    IStack_INIT();
    IQueue_INIT();
    PtrSeq_INIT();
    IDList_INIT();
    IBList_INIT();
    TSTree_INIT();
    PtrSet_INIT();
    BitSet_INIT();
    Sparse_INIT();
    IETree_INIT();
    HTable_INIT();
    Scoped_INIT();
    PtrMap_INIT();
    Indexed_INIT();
  }
}

#ifdef $XCC_h
/**
 * Finalize CCL library.
 */
void
CCL_FINI(void);
#endif//$XCC_h

void
CCL_FINI(void)
{
  if (--CCL_Initialized == 0) {
    if (CCL_TRACE) fprintf(CCL_TRACE, "*** CCL FINI\n");
    Indexed_FINI();
    PtrMap_FINI();
    Scoped_FINI();
    HTable_FINI();
    IETree_FINI();
    Sparse_FINI();
    BitSet_FINI();
    PtrSet_FINI();
    TSTree_FINI();
    IBList_FINI();
    IDList_FINI();
    PtrSeq_FINI();
    IQueue_FINI();
    IStack_FINI();
    IArray_FINI();
    PQueue_FINI();
    BSL_FINI();
  }
}

#ifdef $XCC__h
/**
 * Setup and execute a CCL test.
 */
void
CCL_Test(void (*test)(void));
#endif//$XCC__h

void
CCL_Test(void (*test)(void))
{
  CCL_INIT();
  (*test)();
  CCL_FINI();
}

#if XCC__C
static void
CCL_TEST(void)
{
#include "CCL/CCL_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(CCL_TEST);
  return 0;
}
#endif

