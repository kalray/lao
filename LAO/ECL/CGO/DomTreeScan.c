#ifdef $XCC_h
/*
 * !!!!	DomTreeScan.xcc
 *
 * Fabrice Rastello (Fabrice.Rastello@ens-lyon.fr).
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
 * @brief Dominance Tree Scan allocator.
 */
#endif//$XCC_h


#include "CGO/CGO_.h"

#ifdef $XCC__h
#include "CGO/DomTreeScan.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Tree scan register allocator.
 */
struct DomTreeScan_ {
  //@args	Memory memory, SSAForm ssaForm
  Memory MEMORY;
  SSAForm SSAFORM;
};
#endif//$XCC_h

DomTreeScan
DomTreeScan_Ctor(DomTreeScan this,
                Memory memory, SSAForm ssaForm)
{
  CodeRegion codeRegion = SSAForm_codeRegion(ssaForm);
  TemporaryTable globalTable = CodeRegion_globalTable(codeRegion);
  *DomTreeScan__MEMORY(this) = memory;
  *DomTreeScan__SSAFORM(this) = ssaForm;
  //
  SSAForm_makeConventional(ssaForm, globalTable);
  //
  //
  return this;
}

void
DomTreeScan_Dtor(DomTreeScan this)
{
}

size_t
DomTreeScan_Size(Memory memory, SSAForm ssaForm)
{
  return sizeof(DomTreeScan_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(DomTreeScan_)\t%zu\n", sizeof(DomTreeScan_));
}
#endif//$XCC__c

#ifdef $XCC_h
DomTreeScan
DomTreeScan_new(Memory parent, SSAForm ssaForm);
#endif//$XCC_h

DomTreeScan
DomTreeScan_new(Memory parent, SSAForm ssaForm)
{
  Memory memory = Memory_new(parent, true);
  size_t size = DomTreeScan_Size(memory, ssaForm);
  DomTreeScan this = Memory_alloc(memory, size);
  return DomTreeScan_Ctor(this, memory, ssaForm);
}

#ifdef $XCC_h
DomTreeScan
DomTreeScan_delete(DomTreeScan this);
#endif//$XCC_h

DomTreeScan
DomTreeScan_delete(DomTreeScan this)
{
  if (this != NULL) {
    Memory memory = DomTreeScan_MEMORY(this);
#ifndef _NDTOR
    DomTreeScan_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This DomTreeScan Memory.
 */
Memory
DomTreeScan_memory(DomTreeScan this);
#endif//$XCC_h

Memory
DomTreeScan_memory(DomTreeScan this)
{
  return DomTreeScan_MEMORY(this);
}

#ifdef $XCC_h
/**
 * Pretty-print this DomTreeScan.
 */
bool
DomTreeScan_pretty(DomTreeScan this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
DomTreeScan_pretty(DomTreeScan this, FILE *file)
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
DomTreeScan_allocate(DomTreeScan this);
#endif//$XCC_h

void
DomTreeScan_allocate(DomTreeScan this)
{
  Except_NEVER(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the DomTreeScan module.
 */
#define DomTreeScan_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the DomTreeScan module.
 */
#define DomTreeScan_FINI()
#endif//$XCC__h

#if XCC__C
static void
DomTreeScan_TEST(void)
{
#include "CGO/DomTreeScan_.c"
}

int
main(int argc, char **argv)
{
  CGO_Test(DomTreeScan_TEST);
  return 0;
}
#endif

