#ifdef $XCC_h
/*
 * !!!!	RegFile.xcc
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
#include "MDT/RegFile.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define RegFile(ID, WIDTH, LOWREGISTER, HIGHREGISTER, NATIVETYPES) \
  RegFile_##ID,
  RegFile_,
#include /*MDT*/"RegFile.tuple"
  RegFile__
#undef RegFile
} enum_RegFile;
#endif//$XCC__h

const int RegFile__Count = RegFile__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Identifier of a register file.
 *
 * The identifier is defined by the MDS in the RegFile.tuple file.
 */
extern const char *
RegFile_Id(RegFile this);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The name of a register file, stripped from the family prefix.
 */
extern const char *
RegFile_name(RegFile this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
RegFile_Id(RegFile this)
{
  static const char *_[] = {
#define RegFile(ID, WIDTH, LOWREGISTER, HIGHREGISTER, NATIVETYPES) \
    #ID,
    "_UNDEF",
#include "RegFile.tuple"
  };
  Except_REQUIRE((unsigned)this < RegFile__);
  return _[this];
}
const char *
RegFile_name(RegFile this)
{
  return this? strchr(RegFile_Id(this) + sizeof(MDT_TARGET) - 1, '_') + 1: "_UNDEF";
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int regFile = 0;
  for (; regFile < RegFile__; regFile++) {
    printf("RegFile_%s\t%d\n", RegFile_Id(regFile), regFile);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This RegFile Width attribute.
 */
unsigned
RegFile_width(RegFile this);
#endif//$XCC_h

extern const uint16_t
RegFile_Width[];
unsigned
RegFile_width(RegFile this)
{
  Except_REQUIRE((unsigned)this < RegFile__);
  return RegFile_Width[this];
}

const uint16_t
RegFile_Width[] = {
#define WIDTH(d) d
#define RegFile(ID, WIDTH, LOWREGISTER, HIGHREGISTER, NATIVETYPES) \
  WIDTH,
  WIDTH(0),
#include "RegFile.tuple"
};

#ifdef $XCC__c
{
  int regFile = 0;
  for (; regFile < RegFile__; regFile++) {
    printf("Width(%s)\t%d\n", RegFile_Id(regFile), RegFile_width(regFile));
  }
  fprintf(stderr, "sizeof(RegFile_Width)\t%zu\n", sizeof(RegFile_Width));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Return the default NativeType for this RegFile.
 * If this regFile has no associated storage type, return NativeType_.
 */
NativeType
RegFile_getNativeType(RegFile this);
#endif//$XCC_h

extern const NativeType
RegFile_NativeType[][NativeType__];
NativeType
RegFile_getNativeType(RegFile this)
{
  Except_REQUIRE((unsigned)this < RegFile__);
  return (NativeType)RegFile_NativeType[this][0];
}

const NativeType
RegFile_NativeType[][NativeType__] = {
#define NATIVETYPES(count, items) { items NativeType_ }
#define NATIVETYPE(ID) NativeType_##ID,
#define RegFile(ID, WIDTH, LOWREGISTER, HIGHREGISTER, NATIVETYPES) \
  NATIVETYPES,
  NATIVETYPES(0, /**/),
#include "RegFile.tuple"
};

#ifdef $XCC__c
{
  int regFile = 0;
  for (; regFile < RegFile__; regFile++) {
    printf("GetNativeType(%s)\t%s\n", RegFile_Id(regFile),
           NativeType_Id(RegFile_getNativeType(regFile)));
  }
  fprintf(stderr, "sizeof(RegFile_NativeType)\t%zu\n", sizeof(RegFile_NativeType));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This RegFile low Register.
 */
Register
RegFile_lowRegister(RegFile this);
#endif//$XCC_h

extern const Register
RegFile_LowReg[];
Register
RegFile_lowRegister(RegFile this)
{
  Except_REQUIRE((unsigned)this < RegFile__);
  return (Register)RegFile_LowReg[this];
}

const Register
RegFile_LowReg[] = {
#define LOWREGISTER(ID) Register_##ID
#define RegFile(ID, WIDTH, LOWREGISTER, HIGHREGISTER, NATIVETYPES) \
  LOWREGISTER,
  LOWREGISTER(_UNDEF),
#include "RegFile.tuple"
};

#ifdef $XCC_h
/**
 * This RegFile high Register.
 */
Register
RegFile_highRegister(RegFile this);
#endif//$XCC_h

extern const Register
RegFile_HighReg[];
Register
RegFile_highRegister(RegFile this)
{
  Except_REQUIRE((unsigned)this < RegFile__);
  return (Register)RegFile_HighReg[this];
}

const Register
RegFile_HighReg[] = {
#define HIGHREGISTER(ID) Register_##ID
#define RegFile(ID, WIDTH, LOWREGISTER, HIGHREGISTER, NATIVETYPES) \
  HIGHREGISTER,
  HIGHREGISTER(_UNDEF),
#include "RegFile.tuple"
};

#ifdef $XCC__c
{
  int regFile = 0;
  for (; regFile < RegFile__; regFile++) {
    printf("LowRegister(%s)\t%s\t", RegFile_Id(regFile), Register_Id(RegFile_lowRegister(regFile)));
    printf("HighRegister(%s)\t%s\n", RegFile_Id(regFile), Register_Id(RegFile_highRegister(regFile)));
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Iterate in this RegFile Registers
 */
#define RegFile_FOREACH_Register(this, reg) { \
  int RegFile_LOWREGISTER = RegFile_lowRegister(this), reg; \
  int RegFile_HIGHREGISTER = RegFile_highRegister(this); \
  for (reg = RegFile_LOWREGISTER; reg <= RegFile_HIGHREGISTER; reg++) {
#define RegFile_ENDEACH_Register \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * A RegisterSet with the members of this RegFile.
 */
const_RegisterSet
RegFile_registerSet(RegFile this);
#endif//$XCC_h

extern /*const*/ RegisterSet_
RegFile_RegisterSet[];
const_RegisterSet
RegFile_registerSet(RegFile this)
{
  Except_REQUIRE((unsigned)this < RegFile__);
  return RegFile_RegisterSet + this;
}

/*const*/ RegisterSet_
RegFile_RegisterSet[RegFile__];

#ifdef $XCC__c
{
  int regFile = 0;
  for (; regFile < RegFile__; regFile++) {
    const_RegisterSet registerSet = RegFile_registerSet(regFile);
    printf("RegisterSet(%s)\t", RegFile_Id(regFile));
    RegisterSet_pretty(registerSet, stdout, (Processor)1/*FIXME!*/);
    printf("\n");
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the RegFile module.
 */
void
RegFile_INIT(void);
#endif//$XCC__h

void
RegFile_INIT(void)
{
  int regFile = 0, registre = 0;
  for (; regFile < RegFile__; regFile++) {
    int lowRegister = RegFile_lowRegister(regFile);
    int highRegister = RegFile_highRegister(regFile);
    RegisterSet registerSet = RegFile_RegisterSet + regFile;
    for (registre = lowRegister; registre <= highRegister; registre++) {
      RegisterSet_insert(registerSet, registre);
    }
  }
}

#ifdef $XCC__h
/**
 * Finalize the RegFile module.
 */
#define RegFile_FINI()
#endif//$XCC__h

#if XCC__C
static void
RegFile_TEST(void)
{
#include "MDT/RegFile_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(RegFile_TEST);
  return 0;
}
#endif

