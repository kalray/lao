#ifdef $XCC_h
/*
 * !!!!	NativeType.xcc
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
#include "MDT/NativeType.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define NativeType(ID, WIDTH, SIZEOF, ALIGN, PRINTF, SLICE) \
  NativeType_##ID,
  NativeType_,
#include /*MDT*/"NativeType.tuple"
  NativeType__
#undef NativeType
} enum_NativeType;
#endif//$XCC__h

const int NativeType__Count = NativeType__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the NativeType enumeration.
 */
extern const char *
NativeType_Id(NativeType this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
NativeType_Id(NativeType this)
{
  static const char *_[] = {
#define NativeType(ID, WIDTH, SIZEOF, ALIGN, PRINTF, SLICE) \
    #ID,
    "_UNDEF",
#include "NativeType.tuple"
  };
  Except_REQUIRE((unsigned)this < NativeType__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int nativeType = 0;
  for (; nativeType < NativeType__; nativeType++) {
    printf("NativeType_%s\t%d\n", NativeType_Id(nativeType), nativeType);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This NativeType printf format.
 */
const char *
NativeType_printf(NativeType this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
NativeType_Printf[] = {
#define PRINTF(s) s
#define NativeType(ID, WIDTH, SIZEOF, ALIGN, PRINTF, SLICE) \
  PRINTF,
  PRINTF(NULL),
#include "NativeType.tuple"
};
#endif//_NTRACE

#ifndef _NTRACE
const char *
NativeType_printf(NativeType this)
{
  Except_REQUIRE((unsigned)this < NativeType__);
  return NativeType_Printf[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int nativeType = 0;
  for (; nativeType < NativeType__; nativeType++) {
    printf("\nPrintf(%s)\t%s", NativeType_Id(nativeType), NativeType_printf(nativeType));
  }
  printf("\n");
  fprintf(stderr, "sizeof(NativeType_Printf)\t%zu\n", sizeof(NativeType_Printf));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This NativeType bit width.
 */
unsigned
NativeType_width(NativeType this);
#endif//$XCC_h

extern const uint16_t
NativeType_Width[];
unsigned
NativeType_width(NativeType this)
{
  Except_REQUIRE((unsigned)this < NativeType__);
  return NativeType_Width[this];
}

const uint16_t
NativeType_Width[] = {
#define WIDTH(d) d
#define NativeType(ID, WIDTH, SIZEOF, ALIGN, PRINTF, SLICE) \
  WIDTH,
  WIDTH(0),
#include "NativeType.tuple"
};

#ifdef $XCC__c
{
  int nativeType = 0;
  for (; nativeType < NativeType__; nativeType++) {
    printf("\nWidth(%s)\t%d", NativeType_Id(nativeType), NativeType_width(nativeType));
  }
  printf("\n");
  fprintf(stderr, "sizeof(NativeType_Width)\t%zu\n", sizeof(NativeType_Width));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This NativeType size in bytes when stored to memory.
 */
unsigned
NativeType_sizeOf(NativeType this);
#endif//$XCC_h

extern const uint16_t
NativeType_SizeOf[];
unsigned
NativeType_sizeOf(NativeType this)
{
  Except_REQUIRE((unsigned)this < NativeType__);
  return NativeType_SizeOf[this];
}

const uint16_t
NativeType_SizeOf[] = {
#define SIZEOF(d) d
#define NativeType(ID, WIDTH, SIZEOF, ALIGN, PRINTF, SLICE) \
  SIZEOF,
  SIZEOF(0),
#include "NativeType.tuple"
};

#ifdef $XCC__c
{
  int nativeType = 0;
  for (; nativeType < NativeType__; nativeType++) {
    printf("\nSizeOf(%s)\t%d", NativeType_Id(nativeType), NativeType_sizeOf(nativeType));
  }
  printf("\n");
  fprintf(stderr, "sizeof(NativeType_SizeOf)\t%zu\n", sizeof(NativeType_SizeOf));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This NativeType Alignment Base
 */
unsigned
NativeType_align(NativeType this);
#endif//$XCC_h

extern const uint16_t
NativeType_Align[];
unsigned
NativeType_align(NativeType this)
{
  Except_REQUIRE((unsigned)this < NativeType__);
  return NativeType_Align[this];
}

const uint16_t
NativeType_Align[] = {
#define ALIGN(base) base,
#define NativeType(ID, WIDTH, SIZEOF, ALIGN, PRINTF, SLICE) \
  ALIGN
  ALIGN(0)
#include "NativeType.tuple"
#undef ALIGN
};

#ifdef $XCC__c
{
  int nativeType = 0;
  for (; nativeType < NativeType__; nativeType++) {
    printf("\nAlign(%s)\t%d", NativeType_Id(nativeType), NativeType_align(nativeType));
  }
  printf("\n");
  fprintf(stderr, "sizeof(NativeType_Align)\t%zu\n", sizeof(NativeType_Align));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This NativeType slice width in bits.
 */
unsigned
NativeType_slice(NativeType this);
#endif//$XCC_h

extern const uint16_t
NativeType_Slice[];
unsigned
NativeType_slice(NativeType this)
{
  Except_REQUIRE((unsigned)this < NativeType__);
  return NativeType_Slice[this];
}

const uint16_t
NativeType_Slice[] = {
#define SLICE(d) d
#define NativeType(ID, WIDTH, SIZEOF, ALIGN, PRINTF, SLICE) \
  SLICE,
  SLICE(0),
#include "NativeType.tuple"
};

#ifdef $XCC__c
{
  int nativeType = 0;
  for (; nativeType < NativeType__; nativeType++) {
    printf("\nSlice(%s)\t%d", NativeType_Id(nativeType), NativeType_slice(nativeType));
  }
  printf("\n");
  fprintf(stderr, "sizeof(NativeType_Slice)\t%zu\n", sizeof(NativeType_Slice));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the NativeType module.
 */
#define NativeType_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the NativeType module.
 */
#define NativeType_FINI()
#endif//$XCC__h

#if XCC__C
static void
NativeType_TEST(void)
{
#include "MDT/NativeType_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(NativeType_TEST);
  return 0;
}
#endif

