#ifdef $XCC_h
/*
 * !!!!	SuperBlock.xcc
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
 * @file
 * @ingroup PFA
 * @brief SuperBlock partition.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/SuperBlock.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the SuperBlock flags.
 */
typedef enum {
  SuperBlockFlag_Acyclic = 0x0,		// SuperBlock is acyclic.
  SuperBlockFlag_InnerLoop = 0x1,	// SuperBlock is an inner loop.
  SuperBlockFlag_Scheduling = 0x2,	// SuperBlock used for instruction scheduling.
  SuperBlockFlag_Pipelining = 0x4,	// SuperBlock used for software pipelining.
} SuperBlockFlag;
typedef unsigned SuperBlockFlags;
typedef uint8_t short_SuperBlockFlags;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * A single-entry linear sequence of BasicBlock(s) with GOTO as side exits.
 *
 * SuperBlock(s) come in two flavors: acyclic, or inner loops. SuperBlock(s) provide an lightweight
 * optimization scope for basic block alignment, prepass scheduling, and software pipelining.
 * The SuperBlock(s) do not cross LoopScope boundaries, in order to access dependence information.
 * Each SuperBlock has a trace ID, which is set on its BasicBlock(s).
 */
struct SuperBlock_;
#endif//$XCC_h

#ifdef $XCC__h
struct SuperBlock_ {
  short_SuperBlockFlags FLAGS;
  int16_t TRACEID;
  LoopScope LOOPSCOPE;
  BasicBlock HEADBLOCK;
  BasicBlock TAILBLOCK;
  IndexedSet_ BASICBLOCKS[1];
};
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the SuperBlock module.
 */
#define SuperBlock_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the SuperBlock module.
 */
#define SuperBlock_FINI()
#endif//$XCC__h

#if XCC__C
static void
SuperBlock_TEST(void)
{
#include "PFA/SuperBlock_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(SuperBlock_TEST);
  return 0;
}
#endif

