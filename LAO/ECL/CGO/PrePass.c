#ifdef $XCC_h
/*
 * !!!!	PrePass.xcc
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
 * @ingroup CGO
 * @brief Software PrePass (to do).
 */
#endif//$XCC_h


#include "CGO/CGO_.h"

#ifdef $XCC__h
#include "CGO/PrePass.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Prepass instruction scheduling in a CodeRegion.
 */
struct PrePass_ {
  //@args	Memory memory, CodeRegion codeRegion
  Memory MEMORY;
  CodeRegion CODEREGION;
};
#endif//$XCC_h

PrePass
PrePass_Ctor(PrePass this,
               Memory memory, CodeRegion codeRegion)
{
  *PrePass__MEMORY(this) = memory;
  *PrePass__CODEREGION(this) = codeRegion;
  return this;
}

void
PrePass_Dtor(PrePass this)
{
}

size_t
PrePass_Size(Memory memory, CodeRegion codeRegion)
{
  return sizeof(PrePass_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(PrePass_)\t%zu\n", sizeof(PrePass_));
}
#endif//$XCC__c

#ifdef $XCC_h
PrePass
PrePass_new(Memory parent, CodeRegion codeRegion);
#endif//$XCC_h

PrePass
PrePass_new(Memory parent, CodeRegion codeRegion)
{
  Memory memory = Memory_new(parent, true);
  size_t size = PrePass_Size(memory, codeRegion);
  PrePass this = Memory_alloc(memory, size);
  return PrePass_Ctor(this, memory, codeRegion);
}

#ifdef $XCC_h
PrePass
PrePass_delete(PrePass this);
#endif//$XCC_h

PrePass
PrePass_delete(PrePass this)
{
  if (this != NULL) {
    Memory memory = PrePass_MEMORY(this);
#ifndef _NDTOR
    PrePass_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This PrePass Memory.
 */
Memory
PrePass_memory(PrePass this);
#endif//$XCC_h

Memory
PrePass_memory(PrePass this)
{
  return PrePass_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This PrePass CodeRegion.
 */
CodeRegion
PrePass_codeRegion(PrePass this);
#endif//$XCC_h

CodeRegion
PrePass_codeRegion(PrePass this)
{
  return PrePass_CODEREGION(this);
}

#ifdef $XCC_h
/**
 * Pretty-print this PrePass.
 */
bool
PrePass_pretty(PrePass this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
PrePass_pretty(PrePass this, FILE *file)
{
  if (file == NULL) file = stderr;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the PrePass module.
 */
#define PrePass_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the PrePass module.
 */
#define PrePass_FINI()
#endif//$XCC__h

#if XCC__C
static void
PrePass_TEST(void)
{
#include "CGO/PrePass_.c"
}

int
main(int argc, char **argv)
{
  CGO_Test(PrePass_TEST);
  return 0;
}
#endif

