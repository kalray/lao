#ifdef $XCC_h
/*
 * !!!!	Common.xcc
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
 * @brief Common for data stream.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/Common.h"
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
 * Initialize the Common module.
 */
#define Common_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Common module.
 */
#define Common_FINI()
#endif//$XCC__h

#if XCC__C
static void
Common_TEST(void)
{
#include "AIR/Common_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(Common_TEST);
  return 0;
}
#endif

