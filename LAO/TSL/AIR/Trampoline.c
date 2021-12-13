#ifdef $XCC_h
/*
 * !!!!	Trampoline.xcc
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
 * @file
 * @ingroup AIR
 * @brief Trampoline for JIT (to do).
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/Trampoline.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * TODO!
 */

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Trampoline module.
 */
#define Trampoline_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Trampoline module.
 */
#define Trampoline_FINI()
#endif//$XCC__h

#if XCC__C
static void
Trampoline_TEST(void)
{
#include "AIR/Trampoline_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(Trampoline_TEST);
  return 0;
}
#endif

