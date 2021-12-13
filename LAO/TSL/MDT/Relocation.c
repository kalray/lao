#ifdef $XCC_h
/*
 * !!!!	Relocation.xcc
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
#include "MDT/Relocation.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Relocation(ID, RELATIVE, ENCODE, DECODE, LINKER) \
  Relocation_##ID,
  Relocation_,
#include /*MDT*/"Relocation.tuple"
  Relocation__
#undef Relocation
} enum_Relocation;
#endif//$XCC__h

const int Relocation__Count = Relocation__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Relocation enumeration.
 */
extern const char *
Relocation_Id(Relocation this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Relocation_Id(Relocation this)
{
  static const char *_[] = {
#define Relocation(ID, RELATIVE, ENCODE, DECODE, LINKER) \
    #ID,
    "_UNDEF",
#include "Relocation.tuple"
  };
  Except_REQUIRE((unsigned)this < Relocation__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int relocation = 0;
  for (; relocation < Relocation__; relocation++) {
    printf("Relocation_%s\t%d\n", Relocation_Id(relocation), relocation);
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Get a Relocation from its id string.
 */
Relocation
Relocation_ofId(const char *string);
#endif//$XCC_h

Relocation
Relocation_ofId(const char *string)
{
#define Relocation(ID, RELATIVE, ENCODE, DECODE, LINKER) \
  else if(!strcmp(#ID, string)) { \
    return Relocation_ ## ID; \
  }
  if(!strcmp("",string)) {
    return Relocation__UNDEF;
  }
#include "Relocation.tuple"
  else {
    Except_THROW("Cannot find a relocation corresponding to id \"%s\"!\n", string);
  }
  return Relocation__UNDEF;
#undef Relocation
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration for the Relocation_relative values.
 */
typedef enum {
  RelocationRelative__UNDEF,
  RelocationRelative_PC,
  RelocationRelative_GP,
  RelocationRelative_GOT,
  RelocationRelative_TP,
  RelocationRelative_BASE,
} enum_RelocationRelative;
typedef uint8_t RelocationRelative;
#define Relocation_isRelativePC(this) (Relocation_relative(this) == RelocationRelative_PC)
#define Relocation_isRelativeGP(this) (Relocation_relative(this) == RelocationRelative_GP)
#define Relocation_isRelativeTP(this) (Relocation_relative(this) == RelocationRelative_TP)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Relocation RelocationRelative.
 */
RelocationRelative
Relocation_relative(Relocation this);
#endif//$XCC_h

extern const RelocationRelative
Relocation_Relative[];
RelocationRelative
Relocation_relative(Relocation this)
{
  Except_REQUIRE((unsigned)this < Relocation__);
  return (RelocationRelative)Relocation_Relative[this];
}

const RelocationRelative
Relocation_Relative[] = {
#define RELATIVE(relative) RelocationRelative_##relative
#define Relocation(ID, RELATIVE, ENCODE, DECODE, LINKER) \
  RELATIVE,
  RELATIVE(_UNDEF),
#include "Relocation.tuple"
};

#ifdef $XCC__c
{
  int relocation = 0;
  for (; relocation < Relocation__; relocation++) {
    printf("Relative(%s)\t0x%x\n", Relocation_Id(relocation), Relocation_relative(relocation));
  }
  fprintf(stderr, "sizeof(Relocation_Relative)\t%zu\n", sizeof(Relocation_Relative));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Relocation module.
 */
#define Relocation_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Relocation module.
 */
#define Relocation_FINI()
#endif//$XCC__h

#if XCC__C
static void
Relocation_TEST(void)
{
#include "MDT/Relocation_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Relocation_TEST);
  return 0;
}
#endif

