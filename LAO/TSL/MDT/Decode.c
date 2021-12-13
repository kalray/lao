#ifdef $XCC_h
/*
 * !!!!	Decode.xcc
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
#include "MDT/Decode.h"
#endif//$XCC__h

#ifdef $XCC_h
#ifdef _NTRACE
#ifndef NDECODE
#define NDECODE
#endif//NDECODE
#endif//_NTRACE
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * Decode_Decoding function prototypes.
 */
#ifndef NDECODE
#define Decoding(ID, PROCESSOR, WORDTYPE, WORDCOUNT) \
extern Opcode \
Decode_Decoding_##ID(const void *buffer);
#include /*MDT*/"Decoding.tuple"
#endif//NDECODE
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
 * Initialize the Decode module.
 */
#define Decode_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Decode module.
 */
#define Decode_FINI()
#endif//$XCC__h

#if XCC__C
static void
Decode_TEST(void)
{
#include "MDT/Decode_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Decode_TEST);
  return 0;
}
#endif

