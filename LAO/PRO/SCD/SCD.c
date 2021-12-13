#ifdef $XCC_h
/*
 * !!!!	SCD.xcc
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
 * @defgroup SCD SCheDuling
 * @ingroup PRO
 * @brief The Relaxation module implements scheduling
 *   relaxation of Leung Palem Pnueli TOPLAS 2002. The Scheduler module is the
 *   instruction scheduler base class. The Insertion module implements register
 *   lifetime-sensitive software pipelining. The Formulation module implements
 *   optimal integer programming formulation for modulo scheduling. The
 *   Construction module implements software pipeline construction after modulo
 *   scheduling.
 * 
 */
#endif//$XCC_h


#include "SCD/SCD_.h"

#ifdef $XCC__h
#include "SCD/SCD.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CCL.h"
#include "CAL.h"
#include "MDT.h"
#include "AIR.h"
#include "LIR.h"
#include "PFA.h"
#include "PRO.h"
#include "XFA/XFA.h"
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include "SCD/FastList.h"
#include "SCD/GrahamList.h"
#include "SCD/Relaxation.h"
#include "SCD/Scheduler.h"
#include "SCD/Insertion.h"
#include "SCD/Acyclic.h"
#include "SCD/Formulation.h"
#include "SCD/Construction.h"
#include "SCD/RCMS.h"
#endif//$XCC_h

#ifdef $XCC__h
#include "SCD/FastList_.h"
#include "SCD/GrahamList_.h"
#include "SCD/Relaxation_.h"
#include "SCD/Scheduler_.h"
#include "SCD/Insertion_.h"
#include "SCD/Acyclic_.h"
#include "SCD/Formulation_.h"
#include "SCD/Construction_.h"
#include "SCD/RCMS_.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * File handle used to trace the SCD.
 */
extern FILE *SCD_TRACE;
#ifdef _NTRACE
#define SCD_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef SCD_TRACE 
FILE *SCD_TRACE = NULL;

#ifdef $XCC_h
/*
 * Variables used to control SCD in non-release mode.
 */
extern int LAO_STARTSCD;
extern int LAO_STARTSWP;
extern int LAO_IDENTITY;
extern int LAO_SERIALIZE;
extern int FORCE_LLPALEVEL;
extern int FORCE_UNWINDING;
extern int LAO_MAXITEMCOUNT;
extern int FORCE_SCANNING;
extern int FORCE_EXITWEIGHT;
#endif//$XCC_h

int LAO_STARTSCD;
int LAO_STARTSWP;
int LAO_IDENTITY;
int LAO_SERIALIZE;
int FORCE_LLPALEVEL;
int FORCE_UNWINDING;
int LAO_MAXITEMCOUNT;
int FORCE_SCANNING;
int FORCE_EXITWEIGHT;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * SCD init function.
 */
void
SCD_INIT(void);
#endif//$XCC_h

static int SCD_Initialized = 0;

void
SCD_INIT(void)
{
  if (SCD_Initialized++ == 0) {
    if (GETENV("SCD_TRACE")) {
      int file = 0;
      sscanf(GETENV("SCD_TRACE"), "%d", &file);
      if (file == 1) SCD_TRACE = stdout;
      else if (file == 2) SCD_TRACE = stderr;
    }
    LAO_STARTSCD = -1;
    if (GETENV("LAO_STARTSCD")) {
      sscanf(GETENV("LAO_STARTSCD"), "%d", &LAO_STARTSCD);
      fprintf(stderr, "*** LAO_STARTSCD=%d\n", LAO_STARTSCD);
    }
    LAO_STARTSWP = -1;
    if (GETENV("LAO_STARTSWP")) {
      sscanf(GETENV("LAO_STARTSWP"), "%d", &LAO_STARTSWP);
      fprintf(stderr, "*** LAO_STARTSWP=%d\n", LAO_STARTSWP);
    }
    LAO_IDENTITY = -1;
    if (GETENV("LAO_IDENTITY")) {
      sscanf(GETENV("LAO_IDENTITY"), "%d", &LAO_IDENTITY);
      fprintf(stderr, "*** LAO_IDENTITY=%d\n", LAO_IDENTITY);
    }
    LAO_SERIALIZE = 0;
    if (GETENV("LAO_SERIALIZE")) {
      sscanf(GETENV("LAO_SERIALIZE"), "%d", &LAO_SERIALIZE);
      fprintf(stderr, "*** LAO_SERIALIZE=%d\n", LAO_SERIALIZE);
    }
    FORCE_LLPALEVEL = 2;
    if (GETENV("FORCE_LLPALEVEL")) {
      sscanf(GETENV("FORCE_LLPALEVEL"), "%d", &FORCE_LLPALEVEL);
      fprintf(stderr, "*** FORCE_LLPALEVEL=%d\n", FORCE_LLPALEVEL);
    }
    FORCE_UNWINDING = -1;
    if (GETENV("FORCE_UNWINDING")) {
      sscanf(GETENV("FORCE_UNWINDING"), "%d", &FORCE_UNWINDING);
      fprintf(stderr, "*** FORCE_UNWINDING=%d\n", FORCE_UNWINDING);
    }
    LAO_MAXITEMCOUNT = 512;
    if (GETENV("LAO_MAXITEMCOUNT")) {
      sscanf(GETENV("LAO_MAXITEMCOUNT"), "%d", &LAO_MAXITEMCOUNT);
      fprintf(stderr, "*** LAO_MAXITEMCOUNT=%d\n", LAO_MAXITEMCOUNT);
    }
    FORCE_SCANNING = -1;
    if (GETENV("FORCE_SCANNING")) {
      sscanf(GETENV("FORCE_SCANNING"), "%d", &FORCE_SCANNING);
      fprintf(stderr, "*** FORCE_SCANNING=%d\n", FORCE_SCANNING);
    }
    FORCE_EXITWEIGHT = 1;//-1;
    if (GETENV("FORCE_EXITWEIGHT")) {
      sscanf(GETENV("FORCE_EXITWEIGHT"), "%d", &FORCE_EXITWEIGHT);
      fprintf(stderr, "*** FORCE_EXITWEIGHT=%d\n", FORCE_EXITWEIGHT);
    }
    CCL_INIT();
    CAL_INIT();
    MDT_INIT();
    AIR_INIT();
    LIR_INIT();
    PFA_INIT();
    XFA_INIT();
    FastList_INIT();
    GrahamList_INIT();
    Relaxation_INIT();
    Scheduler_INIT();
    Insertion_INIT();
    Acyclic_INIT();
    Formulation_INIT();
    Construction_INIT();
    RCMS_INIT();
  }
}

#ifdef $XCC_h
/**
 * SCD fini function.
 */
void
SCD_FINI(void);
#endif//$XCC_h

void
SCD_FINI(void)
{
  if (--SCD_Initialized == 0) {
    RCMS_FINI();
    Construction_FINI();
    Formulation_FINI();
    Acyclic_FINI();
    Insertion_FINI();
    Scheduler_FINI();
    Relaxation_FINI();
    GrahamList_FINI();
    FastList_FINI();
    XFA_FINI();
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
 * Setup and execute a SCD test.
 */
void
SCD_Test(void (*test)(void));
#endif//$XCC__h

void
SCD_Test(void (*test)(void))
{
  SCD_INIT();
  (*test)();
  SCD_FINI();
}

#if XCC__C
static void
SCD_TEST(void)
{
#include "SCD/SCD_.c"
}

int
main(int argc, char **argv)
{
  SCD_Test(SCD_TEST);
  return 0;
}
#endif

