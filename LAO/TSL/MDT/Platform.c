#ifdef $XCC_h
/*
 * !!!!	Platform.xcc
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
#include "MDT/Platform.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  Platform_##ID,
  Platform_,
#include /*MDT*/"Platform.tuple"
  Platform__
#undef Platform
} enum_Platform;
#endif//$XCC__h

const int Platform__Count = Platform__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Platform enumeration.
 */
extern const char *
Platform_Id(Platform this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Platform_Id(Platform this)
{
  static const char *_[] = {
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
    #ID,
    "_UNDEF",
#include "Platform.tuple"
  };
  Except_REQUIRE((unsigned)this < Platform__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int platform = 0;
  for (; platform < Platform__; platform++) {
    printf("Platform_%s\t%d\n", Platform_Id(platform), platform);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Platform charWidth.
 */
int
Platform_charWidth(Platform this);
#endif//$XCC_h

extern const int8_t
Platform_CharWidth[];
int
Platform_charWidth(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return Platform_CharWidth[this];
}

const int8_t
Platform_CharWidth[] = {
#define CHARWIDTH(d) d
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  CHARWIDTH,
  CHARWIDTH(0),
#include "Platform.tuple"
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Platform addrWidth.
 */
int
Platform_addrWidth(Platform this);
#endif//$XCC_h

extern const int8_t
Platform_AddrWidth[];
int
Platform_addrWidth(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return Platform_AddrWidth[this];
}

const int8_t
Platform_AddrWidth[] = {
#define ADDRWIDTH(d) d
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  ADDRWIDTH,
  ADDRWIDTH(0),
#include "Platform.tuple"
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef enum {
  PlatformEndian__UNDEF,
  PlatformEndian_Little,
  PlatformEndian_Big,
  PlatformEndian__
} enum_PlatformEndian;
typedef uint8_t PlatformEndian;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Endian of this Platform.
 */
PlatformEndian
Platform_endian(Platform this);
#endif//$XCC_h

extern const PlatformEndian
Platform_Endian[];
PlatformEndian
Platform_endian(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return (PlatformEndian)Platform_Endian[this];
}

const PlatformEndian
Platform_Endian[] = {
#define ENDIAN(endian) PlatformEndian_##endian
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  ENDIAN,
  ENDIAN(_UNDEF),
#include "Platform.tuple"
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Platform Text Alignment Base.
 */
unsigned
Platform_alignText(Platform this);
#endif//$XCC_h

extern const uint8_t
Platform_AlignText[];
unsigned
Platform_alignText(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return Platform_AlignText[this];
}

const uint8_t
Platform_AlignText[] = {
#define ALIGNTEXT(base) base,
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  ALIGNTEXT
  ALIGNTEXT(0)
#include "Platform.tuple"
#undef ALIGNTEXT
};

#ifdef $XCC__c
{
  int platform;
  for (platform = 0; platform < Platform__; platform++) {
    printf("AlignText(%s)\t%d\n", Platform_Id(platform), Platform_alignText(platform));
  }
  fprintf(stderr, "sizeof(Platform_AlignText)\t%zu\n", sizeof(Platform_AlignText));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Platform Data Alignment Base.
 */
unsigned
Platform_alignData(Platform this);
#endif//$XCC_h

extern const uint8_t
Platform_AlignData[];
unsigned
Platform_alignData(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return Platform_AlignData[this];
}

const uint8_t
Platform_AlignData[] = {
#define ALIGNDATA(base) base,
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  ALIGNDATA
  ALIGNDATA(0)
#include "Platform.tuple"
#undef ALIGNDATA
};

#ifdef $XCC__c
{
  int platform;
  for (platform = 0; platform < Platform__; platform++) {
    printf("AlignData(%s)\t%d\n", Platform_Id(platform), Platform_alignData(platform));
  }
  fprintf(stderr, "sizeof(Platform_AlignData)\t%zu\n", sizeof(Platform_AlignData));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Platform Heap Alignment Base.
 */
unsigned
Platform_alignHeap(Platform this);
#endif//$XCC_h

extern const uint8_t
Platform_AlignHeap[];
unsigned
Platform_alignHeap(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return Platform_AlignHeap[this];
}

const uint8_t
Platform_AlignHeap[] = {
#define ALIGNHEAP(base) base,
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  ALIGNHEAP
  ALIGNHEAP(0)
#include "Platform.tuple"
#undef ALIGNHEAP
};

#ifdef $XCC__c
{
  int platform;
  for (platform = 0; platform < Platform__; platform++) {
    printf("AlignHeap(%s)\t%d\n", Platform_Id(platform), Platform_alignHeap(platform));
  }
  fprintf(stderr, "sizeof(Platform_AlignHeap)\t%zu\n", sizeof(Platform_AlignHeap));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Platform Stack Alignment Base.
 */
unsigned
Platform_alignStack(Platform this);
#endif//$XCC_h

extern const uint8_t
Platform_AlignStack[];
unsigned
Platform_alignStack(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return Platform_AlignStack[this];
}

const uint8_t
Platform_AlignStack[] = {
#define ALIGNSTACK(base) base,
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  ALIGNSTACK
  ALIGNSTACK(0)
#include "Platform.tuple"
#undef ALIGNSTACK
};

#ifdef $XCC__c
{
  int platform;
  for (platform = 0; platform < Platform__; platform++) {
    printf("AlignStack(%s)\t%d\n", Platform_Id(platform), Platform_alignStack(platform));
  }
  fprintf(stderr, "sizeof(Platform_AlignStack)\t%zu\n", sizeof(Platform_AlignStack));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Platform Native Int.
 */
NativeType
Platform_nativeInt(Platform this);
#endif//$XCC_h

extern const NativeType
Platform_NativeInt[];
NativeType
Platform_nativeInt(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return (NativeType)Platform_NativeInt[this];
}

const NativeType
Platform_NativeInt[] = {
#define NATIVEINT(name) NativeType_##name,
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  NATIVEINT
  NativeType_,
#include "Platform.tuple"
#undef NATIVEINT
};

#ifdef $XCC__c
{
  int platform;
  for (platform = 0; platform < Platform__; platform++) {
    printf("NativeInt(%s)\t%s\n", Platform_Id(platform),
           NativeType_Id(Platform_nativeInt(platform)));
  }
  fprintf(stderr, "sizeof(Platform_NativeInt)\t%zu\n", sizeof(Platform_NativeInt));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Platform Native UInt.
 */
NativeType
Platform_nativeUInt(Platform this);
#endif//$XCC_h

extern const NativeType
Platform_NativeUInt[];
NativeType
Platform_nativeUInt(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return (NativeType)Platform_NativeUInt[this];
}

const NativeType
Platform_NativeUInt[] = {
#define NATIVEUINT(name) NativeType_##name,
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  NATIVEUINT
  NativeType_,
#include "Platform.tuple"
#undef NATIVEUINT
};

#ifdef $XCC__c
{
  int platform;
  for (platform = 0; platform < Platform__; platform++) {
    printf("NativeUInt(%s)\t%s\n", Platform_Id(platform),
           NativeType_Id(Platform_nativeUInt(platform)));
  }
  fprintf(stderr, "sizeof(Platform_NativeUInt)\t%zu\n", sizeof(Platform_NativeUInt));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Platform Native Float.
 */
NativeType
Platform_nativeFloat(Platform this);
#endif//$XCC_h

extern const NativeType
Platform_NativeFloat[];
NativeType
Platform_nativeFloat(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return (NativeType)Platform_NativeFloat[this];
}

const NativeType
Platform_NativeFloat[] = {
#define NATIVEFLOAT(name) NativeType_##name,
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  NATIVEFLOAT
  NativeType_,
#include "Platform.tuple"
#undef NATIVEFLOAT
};

#ifdef $XCC__c
{
  int platform;
  for (platform = 0; platform < Platform__; platform++) {
    printf("NativeFloat(%s)\t%s\n", Platform_Id(platform),
           NativeType_Id(Platform_nativeFloat(platform)));
  }
  fprintf(stderr, "sizeof(Platform_NativeFloat)\t%zu\n", sizeof(Platform_NativeFloat));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Platform Native Ptr.
 */
NativeType
Platform_nativePtr(Platform this);
#endif//$XCC_h

extern const NativeType
Platform_NativePtr[];
NativeType
Platform_nativePtr(Platform this)
{
  Except_REQUIRE((unsigned)this < Platform__);
  return (NativeType)Platform_NativePtr[this];
}

const NativeType
Platform_NativePtr[] = {
#define NATIVEPTR(name) NativeType_##name,
#define Platform(ID, CHARWIDTH, ADDRWIDTH, ENDIAN, \
                 ALIGNTEXT, ALIGNDATA, ALIGNHEAP, ALIGNSTACK, \
                 NATIVEINT, NATIVEUINT, NATIVEFLOAT, NATIVEPTR) \
  NATIVEPTR
  NativeType_,
#include "Platform.tuple"
#undef NATIVEPTR
};

#ifdef $XCC__c
{
  int platform;
  for (platform = 0; platform < Platform__; platform++) {
    printf("NativePtr(%s)\t%s\n", Platform_Id(platform),
           NativeType_Id(Platform_nativePtr(platform)));
  }
  fprintf(stderr, "sizeof(Platform_NativePtr)\t%zu\n", sizeof(Platform_NativePtr));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Platform module.
 */
#define Platform_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Platform module.
 */
#define Platform_FINI()
#endif//$XCC__h

#if XCC__C
static void
Platform_TEST(void)
{
#include "MDT/Platform_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Platform_TEST);
  return 0;
}
#endif

