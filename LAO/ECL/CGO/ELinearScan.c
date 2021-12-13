#ifdef $XCC_h
/*
 * !!!!	ELinearScan.xcc
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
 * @brief Extended Linear Scan allocator.
 */
#endif//$XCC_h


#include "CGO/CGO_.h"

#ifdef $XCC__h
#include "CGO/ELinearScan.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Linear scan register allocator.
 */
struct ELinearScan_ {
  //@args	Memory memory, SSAForm ssaForm
  Memory MEMORY;
  SSAForm SSAFORM;
};
#endif//$XCC_h

ELinearScan
ELinearScan_Ctor(ELinearScan this,
               Memory memory, SSAForm ssaForm)
{
  *ELinearScan__MEMORY(this) = memory;
  *ELinearScan__SSAFORM(this) = ssaForm;
  return this;
}

void
ELinearScan_Dtor(ELinearScan this)
{
}

size_t
ELinearScan_Size(Memory memory, SSAForm ssaForm)
{
  return sizeof(ELinearScan_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ELinearScan_)\t%zu\n", sizeof(ELinearScan_));
}
#endif//$XCC__c

#ifdef $XCC_h
ELinearScan
ELinearScan_new(Memory parent, SSAForm ssaForm);
#endif//$XCC_h

ELinearScan
ELinearScan_new(Memory parent, SSAForm ssaForm)
{
  Memory memory = Memory_new(parent, true);
  size_t size = ELinearScan_Size(memory, ssaForm);
  ELinearScan this = Memory_alloc(memory, size);
  return ELinearScan_Ctor(this, memory, ssaForm);
}

#ifdef $XCC_h
ELinearScan
ELinearScan_delete(ELinearScan this);
#endif//$XCC_h

ELinearScan
ELinearScan_delete(ELinearScan this)
{
  if (this != NULL) {
    Memory memory = ELinearScan_MEMORY(this);
#ifndef _NDTOR
    ELinearScan_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This ELinearScan Memory.
 */
Memory
ELinearScan_memory(ELinearScan this);
#endif//$XCC_h

Memory
ELinearScan_memory(ELinearScan this)
{
  return ELinearScan_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This ELinearScan SSAForm.
 */
SSAForm
ELinearScan_ssaForm(ELinearScan this);
#endif//$XCC_h

SSAForm
ELinearScan_ssaForm(ELinearScan this)
{
  return ELinearScan_SSAFORM(this);
}

#ifdef $XCC_h
/**
 * Pretty-print this ELinearScan.
 */
bool
ELinearScan_pretty(ELinearScan this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
ELinearScan_pretty(ELinearScan this, FILE *file)
{
  if (file == NULL) file = stderr;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Top-level driver for register allocation.
 */
void
ELinearScan_allocate(ELinearScan this);
#endif//$XCC_h

void
ELinearScan_allocate(ELinearScan this)
{
  Except_NEVER(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the ELinearScan module.
 */
#define ELinearScan_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the ELinearScan module.
 */
#define ELinearScan_FINI()
#endif//$XCC__h

#if XCC__C
static void
ELinearScan_TEST(void)
{
#include "CGO/ELinearScan_.c"
}

int
main(int argc, char **argv)
{
  CGO_Test(ELinearScan_TEST);
  return 0;
}
#endif

