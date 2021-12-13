#ifdef $XCC_h
/*
 * !!!!	PtrMap.xcc
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
 *
 * The PtrMap implements a map of pointers using balanced binary tree.
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup CCL
 * @brief Pointer mapping implemented as binary tree.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/PtrMap.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Sequence of pointers implemented as stack.
 *
 * If @c maxCount is zero, no memory is allocated at construction time.
 */
struct PtrMap_ {
  //@args	Memory memory
  Memory MEMORY;
};
#endif//$XCC_h

PtrMap
PtrMap_Ctor(PtrMap this, Memory memory)
{
  *PtrMap__MEMORY(this) = memory;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(PtrMap_)\t%zu\n", sizeof(PtrMap_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the PtrMap module.
 */
#define PtrMap_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the PtrMap module.
 */
#define PtrMap_FINI()
#endif//$XCC__h

#if XCC__C
static void
PtrMap_TEST(void)
{
#include "CCL/PtrMap_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(PtrMap_TEST);
  return 0;
}
#endif

