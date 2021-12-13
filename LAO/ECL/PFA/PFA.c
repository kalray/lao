#ifdef $XCC_h
/*
 * !!!!	PFA.xcc
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
 * @defgroup PFA  Program(?) Flow Analyzes
 * @ingroup ECL
 * @brief Contains control-flow analyzes, and includes in
 *   particular the LoopForest of Ramalingam for recovering the loop nesting
 *   structure, and also SSA coalescing and out-of-SSA techniques including
 *   Sreedhar III (SAS'99), Boissinot (CGO'09 best paper), Decoalescer
 *   (generalization of Budimlic PLDI'02). Fast liveness check and set
 *   computation on the SSA form (CGO'08 best paper) are also included.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/PFA.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CCL.h"
#include "ECL.h"
#include "MDT/MDT.h"
#include "AIR/AIR.h"
#include "LIR/LIR.h"
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
#if defined (linux) || defined (__linux__) || defined (__linux)
#include <sys/resource.h>
#endif//linux
#endif//$XCC__h

#ifdef $XCC_h
/**
 * A multi valued timer
 */
struct Timer_;
#endif//$XCC_h

#ifdef $XCC__h
struct Timer_ {
  //@args       Memory memory, int count
  Memory MEMORY;
  int COUNT;
  struct timeval *TIMERS;     		// Timers.
};
#endif//$XCC__h

Timer
Timer_Ctor(Timer this,
           Memory memory, int count)
{
  *Timer__MEMORY(this) = memory;
  *Timer__COUNT(this) = count;
#if defined (linux) || defined (__linux__) || defined (__linux)
  *Timer__TIMERS(this) = Memory_alloc(memory, count*2*sizeof(struct timeval));
#endif//linux
  return this;
}

void
Timer_Dtor(Timer this)
{
     Memory_free(Timer_MEMORY(this), Timer_TIMERS(this));
}

#ifdef $XCC_h
extern int64_t Timer_start(Timer this, int timer_id);
extern int64_t Timer_stop(Timer this, int timer_id);
extern void Timer_reset(Timer this, int timer_id);
extern void Timer_resetAll(Timer this);
extern int64_t Timer_value(Timer this, int timer_id);
extern int64_t Timer_lastValue(Timer this, int timer_id);
#endif//$XCC_h

#ifdef $XCC_h
#include "PFA/Liveness.h"
#include "PFA/Dominance.h"
#include "PFA/LoopForest.h"
#include "PFA/CodeLayout.h"
#include "PFA/SuperBlock.h"
#include "PFA/ValueNumber.h"
#include "PFA/Interference.h"
#include "PFA/SSAForm.h"
#include "PFA/Deallocator.h"
#include "PFA/LiveSets.h"
#include "PFA/LiveCheck.h"
#include "PFA/SSAConstruct.h"
#include "PFA/OldLiveCheck.h"
#include "PFA/Coalescer.h"
#include "PFA/Decoalescer.h"
#include "PFA/Boissinot.h"
#include "PFA/Sreedhar.h"
#endif//$XCC_h

#ifdef $XCC__h
#include "PFA/Liveness_.h"
#include "PFA/Dominance_.h"
#include "PFA/LoopForest_.h"
#include "PFA/CodeLayout_.h"
#include "PFA/SuperBlock_.h"
#include "PFA/ValueNumber_.h"
#include "PFA/Interference_.h"
#include "PFA/SSAForm_.h"
#include "PFA/Deallocator_.h"
#include "PFA/LiveSets_.h"
#include "PFA/LiveCheck_.h"
#include "PFA/SSAConstruct_.h"
#include "PFA/OldLiveCheck_.h"
#include "PFA/Coalescer_.h"
#include "PFA/Decoalescer_.h"
#include "PFA/Boissinot_.h"
#include "PFA/Sreedhar_.h"
#endif//$XCC__h

#if defined (linux) || defined (__linux__) || defined (__linux)

#define TIMEVAL_TO_USEC(x) ((int64_t)(x)->tv_usec + (int64_t)(x)->tv_sec*1000000)

int64_t
Timer_value(Timer this, int timer_id)
{
  struct timeval *total_tv = &Timer_TIMERS(this)[(timer_id<<1) + 1];
  return TIMEVAL_TO_USEC(total_tv);
}

int64_t
Timer_lastValue(Timer this, int timer_id)
{
  struct timeval *tv = &Timer_TIMERS(this)[timer_id<<1];
  return TIMEVAL_TO_USEC(tv);
}

int64_t
Timer_start(Timer this, int timer_id)
{
  struct rusage r;
  struct timeval *tv = &Timer_TIMERS(this)[timer_id<<1];
  struct timeval *total_tv = &Timer_TIMERS(this)[(timer_id<<1) + 1];
  getrusage(RUSAGE_SELF, &r);
  *tv = r.ru_utime;
  return TIMEVAL_TO_USEC(total_tv);
}

int64_t
Timer_stop(Timer this, int timer_id)
{
  struct timeval stop;
  struct rusage r;
  struct timeval *tv = &Timer_TIMERS(this)[timer_id<<1];
  struct timeval *total_tv = &Timer_TIMERS(this)[(timer_id<<1) + 1];
  getrusage(RUSAGE_SELF, &r);
  stop = r.ru_utime;
  tv->tv_sec = stop.tv_sec - tv->tv_sec;
  tv->tv_usec = stop.tv_usec - tv->tv_usec;
  total_tv->tv_sec += tv->tv_sec;
  total_tv->tv_usec += tv->tv_usec;
  return TIMEVAL_TO_USEC(total_tv);
}

void
Timer_reset(Timer this, int timer_id)
{
  struct timeval *tv = &Timer_TIMERS(this)[timer_id<<1];
  struct timeval *total_tv = &Timer_TIMERS(this)[(timer_id<<1) + 1];
  tv->tv_sec = tv->tv_usec = 0;
  total_tv->tv_sec = total_tv->tv_usec = 0;
}

void
Timer_resetAll(Timer this)
{
  int i;
  for (i = 0; i < Timer_COUNT(this); i++)
    Timer_reset(this, i);
}

#endif//linux

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * File handle used to print the PFA traces.
 */
extern FILE *PFA_TRACE;
#ifdef _NTRACE
#define PFA_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef PFA_TRACE
FILE *PFA_TRACE = NULL;

#ifdef $XCC_h
/**
 * File handle used to print the MAXLIVE traces.
 */
extern FILE *MAXLIVE_TRACE;
#ifdef _NTRACE
#define MAXLIVE_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef MAXLIVE_TRACE
FILE *MAXLIVE_TRACE = NULL;

#ifdef $XCC_h
/**
 * File handle used to print the PFA time information.
 */
extern FILE *PFA_TIMER;
#ifdef _NTIMER
#define PFA_TIMER NULL
#endif//_NTIMER
#endif//$XCC_h

#undef PFA_TIMER
FILE *PFA_TIMER = NULL;

#ifdef $XCC_h
/**
 * File handle used to print the PFA stats.
 */
extern FILE *PFA_STATS;
#ifdef _NTRACE
#define PFA_STATS NULL
#endif//_NTRACE
#endif//$XCC_h

#undef PFA_STATS
FILE *PFA_STATS = NULL;

#ifdef $XCC_h
/*
 * Variables used to control PFA in non-release mode.
 */
extern int FORCE_SUPERGROW;
extern int FORCE_REUSELIMIT;
#endif//$XCC_h

int FORCE_SUPERGROW;
int FORCE_REUSELIMIT;

#ifdef $XCC_h
/**
 * Initialize the PFA library.
 */
void
PFA_INIT(void);
#endif//$XCC_h

static int PFA_Initialized = 0;

void
PFA_INIT(void)
{
  if (PFA_Initialized++ == 0) {
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
    if (GETENV("PFA_TIMER")) {
      int file = 0;
      sscanf(GETENV("PFA_TIMER"), "%d", &file);
      if (file == 1) PFA_TIMER = stdout;
      else if (file == 2) PFA_TIMER = stderr;
    }
    if (GETENV("PFA_STATS")) {
      int file = 0;
      sscanf(GETENV("PFA_STATS"), "%d", &file);
      if (file == 1) PFA_STATS = stdout;
      else if (file == 2) PFA_STATS = stderr;
    }
    FORCE_SUPERGROW = 2;
    if (GETENV("FORCE_SUPERGROW")) {
      sscanf(GETENV("FORCE_SUPERGROW"), "%d", &FORCE_SUPERGROW);
      fprintf(stderr, "*** FORCE_SUPERGROW=%d\n", FORCE_SUPERGROW);
    }
    FORCE_REUSELIMIT = 0;
    if (GETENV("FORCE_REUSELIMIT")) {
      sscanf(GETENV("FORCE_REUSELIMIT"), "%d", &FORCE_REUSELIMIT);
      fprintf(stderr, "*** FORCE_REUSELIMIT=%d\n", FORCE_REUSELIMIT);
    }
    CCL_INIT();
    MDT_INIT();
    AIR_INIT();
    LIR_INIT();
    Liveness_INIT();
    Dominance_INIT();
    LoopForest_INIT();
    CodeLayout_INIT();
    SuperBlock_INIT();
    ValueNumber_INIT();
    Interference_INIT();
    SSAForm_INIT();
    Deallocator_INIT();
    LiveSets_INIT();
    LiveCheck_INIT();
    SSAConstruct_INIT();
    OldLiveCheck_INIT();
    Coalescer_INIT();
    Decoalescer_INIT();
    Boissinot_INIT();
    Sreedhar_INIT();
  }
}

#ifdef $XCC_h
/**
 * Finalize the PFA library.
 */
void
PFA_FINI(void);
#endif//$XCC_h

void
PFA_FINI(void)
{
  if (--PFA_Initialized == 0) {
    Sreedhar_FINI();
    Boissinot_FINI();
    Decoalescer_FINI();
    Coalescer_FINI();
    OldLiveCheck_FINI();
    SSAConstruct_FINI();
    LiveCheck_FINI();
    LiveSets_FINI();
    SSAForm_FINI();
    Deallocator_FINI();
    Interference_FINI();
    ValueNumber_FINI();
    SuperBlock_FINI();
    CodeLayout_FINI();
    LoopForest_FINI();
    Dominance_FINI();
    Liveness_FINI();
    LIR_FINI();
    AIR_FINI();
    MDT_FINI();
    CCL_FINI();
  }
}

#ifdef $XCC__h
/**
 * Setup and execute a PFA test.
 */
void
PFA_Test(void (*test)(void));
#endif//$XCC__h

void
PFA_Test(void (*test)(void))
{
  PFA_INIT();
  (*test)();
  PFA_FINI();
}

#if XCC__C
static void
PFA_TEST(void)
{
#include "PFA/PFA_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(PFA_TEST);
  return 0;
}
#endif

