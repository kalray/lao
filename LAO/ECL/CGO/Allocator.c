#ifdef $XCC_h
/*
 * !!!!	Allocator.xcc
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
 * @brief Register Allocator base class (to do).
 */
#endif//$XCC_h


#include "CGO/CGO_.h"

#ifdef $XCC__h
#include "CGO/Allocator.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Scalar optimizer for a CodeRegion.
 */
struct Allocator_ {
  //@args	Memory memory, CodeRegion codeRegion
  Memory MEMORY;
  CodeRegion CODEREGION;
};
#endif//$XCC_h

Allocator
Allocator_Ctor(Allocator this,
               Memory memory, CodeRegion codeRegion)
{
  *Allocator__MEMORY(this) = memory;
  *Allocator__CODEREGION(this) = codeRegion;
  return this;
}

void
Allocator_Dtor(Allocator this)
{
}

size_t
Allocator_Size(Memory memory, CodeRegion codeRegion)
{
  return sizeof(Allocator_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Allocator_)\t%zu\n", sizeof(Allocator_));
}
#endif//$XCC__c

#ifdef $XCC_h
Allocator
Allocator_new(Memory parent, CodeRegion codeRegion);
#endif//$XCC_h

Allocator
Allocator_new(Memory parent, CodeRegion codeRegion)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Allocator_Size(memory, codeRegion);
  Allocator this = Memory_alloc(memory, size);
  return Allocator_Ctor(this, memory, codeRegion);
}

#ifdef $XCC_h
Allocator
Allocator_delete(Allocator this);
#endif//$XCC_h

Allocator
Allocator_delete(Allocator this)
{
  if (this != NULL) {
    Memory memory = Allocator_MEMORY(this);
#ifndef _NDTOR
    Allocator_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Allocator Memory.
 */
Memory
Allocator_memory(Allocator this);
#endif//$XCC_h

Memory
Allocator_memory(Allocator this)
{
  return Allocator_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This Allocator CodeRegion.
 */
CodeRegion
Allocator_codeRegion(Allocator this);
#endif//$XCC_h

CodeRegion
Allocator_codeRegion(Allocator this)
{
  return Allocator_CODEREGION(this);
}

#ifdef $XCC_h
/**
 * Pretty-print this Allocator.
 */
bool
Allocator_pretty(Allocator this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Allocator_pretty(Allocator this, FILE *file)
{
  if (file == NULL) file = stderr;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Allocator module.
 */
#define Allocator_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Allocator module.
 */
#define Allocator_FINI()
#endif//$XCC__h

#if XCC__C
static void
Allocator_TEST(void)
{
#include "CGO/Allocator_.c"
}

int
main(int argc, char **argv)
{
  CGO_Test(Allocator_TEST);
  return 0;
}
#endif

