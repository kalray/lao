#ifdef $XCC_h
/*
 * !!!!	XFA.xcc
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
 * @defgroup XFA eXtra Flow Analyses
 * @ingroup PRO
 * @brief Support for instruction scheduling, in particular dependence graphs
 * and superblock regions.
 */
#endif//$XCC_h


#include "XFA/XFA_.h"

#ifdef $XCC__h
#include "XFA/XFA.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CCL.h"
#include "CAL.h"
#include "MDT.h"
#include "AIR.h"
#include "LIR.h"
#include "PFA.h"
#include "PRO.h"
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include "XFA/Issue.h"
#include "XFA/Effect.h"
#include "XFA/CFGraph.h"
#include "XFA/RegionFlow.h"
#include "XFA/Lifetime.h"
#include "XFA/BlockTrace.h"
#include "XFA/DDGraph.h"
#endif//$XCC_h

#ifdef $XCC__h
#include "XFA/Issue_.h"
#include "XFA/Effect_.h"
#include "XFA/CFGraph_.h"
#include "XFA/RegionFlow_.h"
#include "XFA/Lifetime_.h"
#include "XFA/BlockTrace_.h"
#include "XFA/DDGraph_.h"
#endif//$XCC__h

#undef PFA_TRACE 
extern FILE *PFA_TRACE;

#undef MAXLIVE_TRACE
extern FILE* MAXLIVE_TRACE;

#ifdef $XCC_h
/*
 * Variables used to control XFA in non-release mode.
 */
extern int FORCE_POSTCOMP;
extern int FORCE_POSTSPEC;
extern int FORCE_KUNROLL;
extern int FORCE_PRESERIAL;
extern int FORCE_POSTSERIAL;
extern unsigned DEBUG_BOOSTABLE;
extern unsigned DEBUG_SPECULABLE;
extern unsigned DEBUG_COMPENSABLE;
#endif//$XCC_h

int FORCE_POSTCOMP;
int FORCE_POSTSPEC;
int FORCE_KUNROLL;
int FORCE_PRESERIAL;
int FORCE_POSTSERIAL;
unsigned DEBUG_BOOSTABLE;
unsigned DEBUG_SPECULABLE;
unsigned DEBUG_COMPENSABLE;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Initialize the XFA library.
 */
void
XFA_INIT(void);
#endif//$XCC_h

static int XFA_Initialized = 0;

void
XFA_INIT(void)
{
  if (XFA_Initialized++ == 0) {
    if (GETENV("PFA_TRACE")) {
      int file = 0;
      sscanf(GETENV("PFA_TRACE"), "%d", &file);
      if (file == 1) PFA_TRACE = stdout;
      else if (file == 2) PFA_TRACE = stderr;
    }
    if (GETENV("MAXLIVE_TRACE")) {
      int file = 0;
      sscanf(GETENV("MAXLIVE_TRACE"), "%d", &file);
      if (file == 1) MAXLIVE_TRACE = stdout;
      else if (file == 2) MAXLIVE_TRACE = stderr;
    }

    FORCE_POSTCOMP = ConfigureCompensation_Local;
    if (GETENV("FORCE_POSTCOMP")) {
      sscanf(GETENV("FORCE_POSTCOMP"), "%d", &FORCE_POSTCOMP);
      fprintf(stderr, "*** FORCE_POSTCOMP=%d\n", FORCE_POSTCOMP);
    }
    FORCE_POSTSPEC = ConfigureSpeculation_MemorySafe;
    if (GETENV("FORCE_POSTSPEC")) {
      sscanf(GETENV("FORCE_POSTSPEC"), "%d", &FORCE_POSTSPEC);
      fprintf(stderr, "*** FORCE_POSTSPEC=%d\n", FORCE_POSTSPEC);
    }
    FORCE_KUNROLL = 0;
    if (GETENV("FORCE_KUNROLL")) {
      sscanf(GETENV("FORCE_KUNROLL"), "%d", &FORCE_KUNROLL);
      fprintf(stderr, "*** FORCE_KUNROLL=%d\n", FORCE_KUNROLL);
    }
    FORCE_PRESERIAL = 0;
    if (GETENV("FORCE_PRESERIAL")) {
      sscanf(GETENV("FORCE_PRESERIAL"), "%d", &FORCE_PRESERIAL);
      fprintf(stderr, "*** FORCE_PRESERIAL=%d\n", FORCE_PRESERIAL);
    }
    FORCE_POSTSERIAL = 0;
    if (GETENV("FORCE_POSTSERIAL")) {
      sscanf(GETENV("FORCE_POSTSERIAL"), "%d", &FORCE_POSTSERIAL);
      fprintf(stderr, "*** FORCE_POSTSERIAL=%d\n", FORCE_POSTSERIAL);
    }
    DEBUG_BOOSTABLE = -1;
    if (GETENV("DEBUG_BOOSTABLE")) {
      sscanf(GETENV("DEBUG_BOOSTABLE"), "%d", &DEBUG_BOOSTABLE);
      fprintf(stderr, "*** DEBUG_BOOSTABLE=%d\n", DEBUG_BOOSTABLE);
    }
    DEBUG_SPECULABLE = -1;
    if (GETENV("DEBUG_SPECULABLE")) {
      sscanf(GETENV("DEBUG_SPECULABLE"), "%d", &DEBUG_SPECULABLE);
      fprintf(stderr, "*** DEBUG_SPECULABLE=%d\n", DEBUG_SPECULABLE);
    }
    DEBUG_COMPENSABLE = -1;
    if (GETENV("DEBUG_COMPENSABLE")) {
      sscanf(GETENV("DEBUG_COMPENSABLE"), "%d", &DEBUG_COMPENSABLE);
      fprintf(stderr, "*** DEBUG_COMPENSABLE=%d\n", DEBUG_COMPENSABLE);
    }
    CCL_INIT();
    CAL_INIT();
    MDT_INIT();
    AIR_INIT();
    LIR_INIT();
    PFA_INIT();
    Issue_INIT();
    Effect_INIT();
    CFGraph_INIT();
    RegionFlow_INIT();
    Lifetime_INIT();
    BlockTrace_INIT();
    DDGraph_INIT();
  }
}

#ifdef $XCC_h
/**
 * Finalize the XFA library.
 */
void
XFA_FINI(void);
#endif//$XCC_h

void
XFA_FINI(void)
{
  if (--XFA_Initialized == 0) {
    DDGraph_FINI();
    BlockTrace_FINI();
    Lifetime_FINI();
    RegionFlow_FINI();
    CFGraph_FINI();
    Effect_FINI();
    Issue_FINI();
    PFA_FINI();
    LIR_FINI();
    AIR_FINI();
    MDT_FINI();
    CAL_FINI();
    CCL_FINI();
  }
}

#ifdef $XCC__h
/**
 * Setup and execute a XFA test.
 */
void
XFA_Test(void (*test)(void));
#endif//$XCC__h

void
XFA_Test(void (*test)(void))
{
  XFA_INIT();
  (*test)();
  XFA_FINI();
}

#if XCC__C
static void
XFA_TEST(void)
{
#include "XFA/XFA_.c"
}

int
main(int argc, char **argv)
{
  XFA_Test(XFA_TEST);
  return 0;
}
#endif

