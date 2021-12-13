#ifdef $XCC_h
/*
 * !!!!	Object.xcc
 *
 * Author: Benoit Dupont de Dinechin (Benoit.Dinechin@kalray.eu)
 *
 * Copyright 2011 Kalray
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
 * @brief Object for data stream.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/Object.h"
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
 * Initialize the Object module.
 */
#define Object_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Object module.
 */
#define Object_FINI()
#endif//$XCC__h

#if XCC__C
static void
Object_TEST(void)
{
#include "AIR/Object_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(Object_TEST);
  return 0;
}
#endif

