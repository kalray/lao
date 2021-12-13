#ifdef $XCC_h
/*
 * !!!!	Behavior.xcc
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

#include "MDT/MDT_.h"

#ifdef $XCC__h
#include "MDT/Behavior.h"
#endif//$XCC__h

#ifdef $XCC_h
#define Behavior_MAXRANK 13
//
#define Behavior_ACCESS_NONE       0
#define Behavior_ACCESS_READ       1
#define Behavior_ACCESS_WRITE      2
#define Behavior_ACCESS_UNMODIFIED 4
#endif//$XCC_h

#ifdef $XCC_h
typedef void (*Behavior)(void *this, OperandDecoded *decoded, Processor processor);
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerate Behavior phases.
 */
typedef enum {
  BehaviorPhase_Fetch,
  BehaviorPhase_Execute,
  BehaviorPhase_Commit,
} enum_BehaviorPhase;
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Behavior module.
 */
#define Behavior_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Behavior module.
 */
#define Behavior_FINI()
#endif//$XCC__h

#if XCC__C
static void
Behavior_TEST(void)
{
#include "MDT/Behavior_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Behavior_TEST);
  return 0;
}
#endif

