#ifdef $XCC_h
/*
 * !!!!	RegClass.xcc
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
#include "MDT/RegClass.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define RegClass(ID, REGFILE, REGISTERS, ENCODE, DECODE, MINREADSTAGES) \
  RegClass_##ID,
  RegClass_,
#include /*MDT*/"RegClass.tuple"
  RegClass__
#undef RegClass
} enum_RegClass;
#endif//$XCC__h

const int RegClass__Count = RegClass__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the RegClass enumeration.
 */
extern const char *
RegClass_Id(RegClass this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
RegClass_Id(RegClass this)
{
  const char *_[] = {
#define RegClass(ID, REGFILE, REGISTERS, ENCODE, DECODE, MINREADSTAGES) \
    #ID,
    "_UNDEF",
#include "RegClass.tuple"
  };
  Except_REQUIRE((unsigned)this < RegClass__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int regClass = 0;
  for (; regClass < RegClass__; regClass++) {
    printf("RegClass_%s\t%d\n", RegClass_Id(regClass), regClass);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The RegFile this RegClass belongs to.
 */
RegFile
RegClass_regFile(RegClass this);
#endif//$XCC_h

extern const RegFile
RegClass_RegFile[];
RegFile
RegClass_regFile(RegClass this)
{
  Except_REQUIRE((unsigned)this < RegClass__);
  return (RegFile)RegClass_RegFile[this];
}

const RegFile
RegClass_RegFile[] = {
#define REGFILE(ID) RegFile_##ID
#define RegClass(ID, REGFILE, REGISTERS, ENCODE, DECODE, MINREADSTAGES) \
  REGFILE,
  REGFILE(_UNDEF),
#include "RegClass.tuple"
};

#ifdef $XCC__c
{
  int regClass = 0;
  for (; regClass < RegClass__; regClass++) {
    int regFile = RegClass_regFile(regClass);
    printf("RegFile(%s)\t%s\n", RegClass_Id(regClass), RegFile_Id(regFile));
  }
  fprintf(stderr, "sizeof(RegClass_RegFile)\t%zu\n", sizeof(RegClass_RegFile));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * This RegClass Register(s) as a RegisterList.
 */
const_RegisterList
RegClass_registerList(RegClass this);
#define RegClass_firstRegister(this) RegisterList_first(RegClass_registerList(this))
#define RegClass_lastRegister(this) RegisterList_last(RegClass_registerList(this))
#endif//$XCC__h

extern const RegisterList_
RegClass_RegisterList[];
const_RegisterList
RegClass_registerList(RegClass this)
{
  Except_REQUIRE((unsigned)this < RegClass__);
  return &RegClass_RegisterList[this];
}

const RegisterList_
RegClass_RegisterList[] = {
#define REGISTER(ID) Register_##ID,
#define REGISTERS(count, items) count, { items }
#define RegClass(ID, REGFILE, REGISTERS, ENCODE, DECODE, MINREADSTAGES) \
  { REGISTERS },
  { REGISTERS(0, /**/) },
#include "RegClass.tuple"
};

#ifdef $XCC__c
{
  int regClass = 0;
  for (; regClass < RegClass__; regClass++) {
    printf("RegisterList(%s)", RegClass_Id(regClass));
    RegisterList_FOREACH_Register(RegClass_registerList(regClass), registre) {
      printf("\t%s", Register_Id(registre));
    } RegisterList_ENDEACH_Register;
    printf("\n");
  }
  fprintf(stderr, "sizeof(RegClass_RegisterList)\t%zu\n", sizeof(RegClass_RegisterList));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * RegClass_Encode functions.
 */
#define ENCODE(e) e
#define RegClass(ID, REGFILE, REGISTERS, ENCODE, DECODE, MINREADSTAGES) \
static inline OperandEncoded \
RegClass_Encode_##ID(OperandDecoded decoded) \
{ \
  OperandEncoded VALUE = decoded - Register__BIAS; \
  ENCODE; \
  return VALUE; \
}
#include "RegClass.tuple"
#undef ENCODE
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * RegClass_Decode functions.
 */
#define DECODE(e) e
#define RegClass(ID, REGISTERS, REGFILE, ENCODE, DECODE, MINREADSTAGES) \
static inline OperandDecoded \
RegClass_Decode_##ID(OperandEncoded encoded) \
{ \
  OperandDecoded VALUE = encoded; \
  DECODE; \
  return VALUE + Register__BIAS; \
}
#include "RegClass.tuple"
#undef DECODE
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This RegClass Register(s) as a RegisterSet.
 */
const_RegisterSet
RegClass_registerSet(RegClass this);
#endif//$XCC_h

extern RegisterSet_
RegClass_RegisterSet[];
const_RegisterSet
RegClass_registerSet(RegClass this)
{
  Except_REQUIRE((unsigned)this < RegClass__);
  return &RegClass_RegisterSet[this];
}

RegisterSet_
RegClass_RegisterSet[RegClass__];

#ifdef $XCC__c
{
  int regClass = 0;
  for (; regClass < RegClass__; regClass++) {
    printf("RegisterSet(%s)", RegClass_Id(regClass));
    RegisterSet_FOREACH_Register(RegClass_registerSet(regClass), registre) {
      printf("\t%s", Register_Id(registre));
    } RegisterSet_ENDEACH_Register;
    printf("\n");
  }
  fprintf(stderr, "sizeof(RegClass_RegisterSet)\t%zu\n", sizeof(RegClass_RegisterSet));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This RegClass contains this Register.
 */
bool
RegClass_contains(RegClass this, Register reg);
#endif//$XCC_h

bool
RegClass_contains(RegClass this, Register reg)
{
  return RegisterList_contains(RegClass_registerList(this), reg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This RegClass minimum read stage for each Processor.
 */
int
RegClass_minReadStage(RegClass this, Processor processor);
#endif//$XCC_h

extern const uint8_t
RegClass_MinReadStages[][Processor__];
int
RegClass_minReadStage(RegClass this, Processor processor)
{
  Except_REQUIRE((unsigned)this < RegClass__);
  Except_REQUIRE((unsigned)processor < Processor__);
  return RegClass_MinReadStages[this][processor];
}

const uint8_t
RegClass_MinReadStages[][Processor__] = {
#define MINREADSTAGE(processor, stage) (uint8_t)(stage),
#define MINREADSTAGES(items) { 0/*Processor_*/, items }
#define RegClass(ID, REGFILE, REGISTERS, ENCODE, DECODE, MINREADSTAGES) \
  MINREADSTAGES,
  MINREADSTAGES(/**/),
#include "RegClass.tuple"
#undef MINREADSTAGES
};

#ifdef $XCC__c
{
  int regClass;
  int processor;
  printf("\t\t\t");
  for (processor = 0; processor < Processor__; processor++) {
    printf("\t%s", Processor_Id(processor));
  }
  printf("\n");
  for (regClass = 0; regClass < RegClass__; regClass++) {
    printf("MinReadStages(%s)", RegClass_Id(regClass));
    for (processor = 0; processor < Processor__; processor++) {
      int stage = RegClass_minReadStage(regClass, processor);
      printf("\t\t%d", stage);
    }
    printf("\n");
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the RegClass module.
 */
void
RegClass_INIT(void);
#endif//$XCC__h

void
RegClass_INIT(void)
{
  int regClass = 0;
  RegisterSet registerSet = NULL;
  for (; regClass < RegClass__; regClass++) {
    registerSet = RegisterSet_Ctor(&RegClass_RegisterSet[regClass]);
    RegisterList_FOREACH_Register(RegClass_registerList(regClass), registre) {
      RegisterSet_insert(&RegClass_RegisterSet[regClass], registre);
    } RegisterList_ENDEACH_Register;
  }
}

#ifdef $XCC__h
/**
 * Finalize the RegClass module.
 */
#define RegClass_FINI()
#endif//$XCC__h

#if XCC__C
static void
RegClass_TEST(void)
{
#include "MDT/RegClass_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(RegClass_TEST);
  return 0;
}
#endif

