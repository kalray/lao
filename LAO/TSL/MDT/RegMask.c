#ifdef $XCC_h
/*
 * !!!!	RegMask.xcc
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
#include "MDT/RegMask.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define RegMask(ID, REGISTERS, REGFILE, ENCODE, DECODE) \
  RegMask_##ID,
  RegMask_,
#include /*MDT*/"RegMask.tuple"
  RegMask__
#undef RegMask
} enum_RegMask;
#endif//$XCC__h

const int RegMask__Count = RegMask__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the RegMask enumeration.
 */
extern const char *
RegMask_Id(RegMask this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
RegMask_Id(RegMask this)
{
  static const char *_[] = {
#define RegMask(ID, REGISTERS, REGFILE, ENCODE, DECODE) \
    #ID,
    "_UNDEF",
#include "RegMask.tuple"
  };
  Except_REQUIRE((unsigned)this < RegMask__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int regMask = 0;
  for (; regMask < RegMask__; regMask++) {
    printf("RegMask_%s\t%d\n", RegMask_Id(regMask), regMask);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * This RegMask Register(s) as a RegisterList.
 */
const_RegisterList
RegMask_registerList(RegMask this);
#endif//$XCC__h

extern const RegisterList_
RegMask_RegisterList[];
const_RegisterList
RegMask_registerList(RegMask this)
{
  Except_REQUIRE((unsigned)this < RegMask__);
  return &RegMask_RegisterList[this];
}

const RegisterList_
RegMask_RegisterList[] = {
#define REGISTER(ID) Register_##ID,
#define REGISTERS(count, items) count, { items }
#define RegMask(ID, REGISTERS, REGFILE, ENCODE, DECODE) \
  { REGISTERS },
  { REGISTERS(0, /**/) },
#include "RegMask.tuple"
};

#ifdef $XCC__c
{
  int regMask = 0;
  for (; regMask < RegMask__; regMask++) {
    printf("RegisterList(%s)", RegMask_Id(regMask));
    RegisterList_FOREACH_Register(RegMask_registerList(regMask), registre) {
      printf("\t%s", Register_Id(registre));
    } RegisterList_ENDEACH_Register;
    printf("\n");
  }
  fprintf(stderr, "sizeof(RegMask_RegisterList)\t%zu\n", sizeof(RegMask_RegisterList));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The RegFile this RegMask belongs to.
 */
RegFile
RegMask_regFile(RegMask this);
#endif//$XCC_h

extern const RegFile
RegMask_RegFile[];
RegFile
RegMask_regFile(RegMask this)
{
  Except_REQUIRE((unsigned)this < RegMask__);
  return (RegFile)RegMask_RegFile[this];
}

const RegFile
RegMask_RegFile[] = {
#define REGFILE(ID) RegFile_##ID
#define RegMask(ID, REGISTERS, REGFILE, ENCODE, DECODE) \
  REGFILE,
  REGFILE(_UNDEF),
#include "RegMask.tuple"
};

#ifdef $XCC__c
{
  int regMask = 0;
  for (; regMask < RegMask__; regMask++) {
    int regFile = RegMask_regFile(regMask);
    printf("RegFile(%s)\t%s\n", RegMask_Id(regMask), RegFile_Id(regFile));
  }
  fprintf(stderr, "sizeof(RegMask_RegFile)\t%zu\n", sizeof(RegMask_RegFile));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * RegMask_Encode functions.
 */
#define ENCODE(e) e
#define RegMask(ID, REGISTERS, REGFILE, ENCODE, DECODE) \
static inline OperandEncoded \
RegMask_Encode_##ID(OperandDecoded decoded) \
{ \
  RegisterSet registerSet = (RegisterSet)decoded; \
  RegisterSetWord *WORDS = RegisterSet_WORDS(registerSet); \
  RegisterSetWord WORD = 0; \
  ENCODE; \
  return 0; \
}
#include "RegMask.tuple"
#undef ENCODE
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * RegMask_Decode functions.
 */
#define DECODE(e) e
#define RegMask(ID, REGISTERS, REGFILE, ENCODE, DECODE) \
static inline OperandDecoded \
RegMask_Decode_##ID(OperandEncoded encoded) \
{ \
  RegisterSetWord WORD = (RegisterSetWord)encoded; \
  RegisterSetWord *WORDS = &WORD; \
  DECODE; \
  return 0; \
}
#include "RegMask.tuple"
#undef DECODE
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This RegMask Register(s) as a RegisterSet.
 */
const_RegisterSet
RegMask_registerSet(RegMask this);
#endif//$XCC_h

extern RegisterSet_
RegMask_RegisterSet[];
const_RegisterSet
RegMask_registerSet(RegMask this)
{
  Except_REQUIRE((unsigned)this < RegMask__);
  return &RegMask_RegisterSet[this];
}

RegisterSet_
RegMask_RegisterSet[RegMask__];

#ifdef $XCC__c
{
  int regMask = 0;
  for (; regMask < RegMask__; regMask++) {
    printf("RegisterSet(%s)", RegMask_Id(regMask));
    RegisterSet_FOREACH_Register(RegMask_registerSet(regMask), registre) {
      printf("\t%s", Register_Id(registre));
    } RegisterSet_ENDEACH_Register;
    printf("\n");
  }
  fprintf(stderr, "sizeof(RegMask_RegisterSet)\t%zu\n", sizeof(RegMask_RegisterSet));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the RegMask module.
 */
void
RegMask_INIT(void);
#endif//$XCC__h

void
RegMask_INIT(void)
{
  int regMask = 0;
  RegisterSet registerSet = NULL;
  for (; regMask < RegMask__; regMask++) {
    registerSet = RegisterSet_Ctor(&RegMask_RegisterSet[regMask]);
    RegisterList_FOREACH_Register(RegMask_registerList(regMask), registre) {
      RegisterSet_insert(&RegMask_RegisterSet[regMask], registre);
    } RegisterList_ENDEACH_Register;
  }
}

#ifdef $XCC__h
/**
 * Finalize the RegMask module.
 */
#define RegMask_FINI()
#endif//$XCC__h

#if XCC__C
static void
RegMask_TEST(void)
{
#include "MDT/RegMask_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(RegMask_TEST);
  return 0;
}
#endif

