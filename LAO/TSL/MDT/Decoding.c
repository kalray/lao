#ifdef $XCC_h
/*
 * !!!!	Decoding.xcc
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
#include "MDT/Decoding.h"
#endif//$XCC__h

#ifdef $XCC_h
/*
 * Expand MDT-generated macros that typedef PlatformInt and PlatformUInt.
 */
typedef int32_t PlatformInt;	//FIXME! PlatformInt_TYPEDEF
typedef uint32_t PlatformUInt;	//FIXME! PlatformUInt_TYPEDEF
#endif//$XCC_h

#ifdef $XCC__h
typedef enum {
#define Decoding(ID, PROCESSOR, WORDTYPE, WORDCOUNT) \
  Decoding_##ID,
  Decoding_,
#include /*MDT*/"Decoding.tuple"
  Decoding__
#undef Decoding
} enum_Decoding;
#endif//$XCC__h

const int Decoding__Count = Decoding__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Decoding enumeration.
 */
extern const char *
Decoding_Id(Decoding this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Decoding_Id(Decoding this)
{
  static const char *_[] = {
#define Decoding(ID, PROCESSOR, WORDTYPE, WORDCOUNT) \
    #ID,
    "_UNDEF",
#include "Decoding.tuple"
  };
  Except_REQUIRE((unsigned)this < Decoding__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int decoding = 0;
  for (; decoding < Decoding__; decoding++) {
    printf("Decoding_%s\t%d\n", Decoding_Id(decoding), decoding);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Decoding Processor.
 */
Processor
Decoding_processor(Decoding this);
#endif//$XCC_h

extern const Processor
Decoding_Processor[];
Processor
Decoding_processor(Decoding this)
{
  Except_REQUIRE((unsigned)this < Decoding__);
  return (Processor)Decoding_Processor[this];
}

const Processor
Decoding_Processor[] = {
#define PROCESSOR(ID) Processor_##ID
#define Decoding(ID, PROCESSOR, WORDTYPE, WORDCOUNT) \
  PROCESSOR,
  PROCESSOR(_UNDEF),
#include "Decoding.tuple"
};

#ifdef $XCC__c
{
  int decoding = 0;
  for (; decoding < Decoding__; decoding++) {
    printf("Processor(%s)\t%s\n", Decoding_Id(decoding), Processor_Id(Decoding_processor(decoding)));
  }
  fprintf(stderr, "sizeof(Decoding_Processor)\t%zu\n", sizeof(Decoding_Processor));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * Decoding word types.
 */
#define WORDTYPE(type) type
#define Decoding(ID, PROCESSOR, WORDTYPE, WORDCOUNT) \
  typedef WORDTYPE Decoding_##ID##_t;
#include "Decoding.tuple"
#undef WORDTYPE
#endif//$XCC__h

#ifdef $XCC__h
/*
 * Decoding word counts.
 */
enum {
#define WORDCOUNT(d) d
#define Decoding(ID, PROCESSOR, WORDTYPE, WORDCOUNT) \
  Decoding_##ID##_count = WORDCOUNT,
#include "Decoding.tuple"
#undef WORDCOUNT
};
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Decoding word count.
 */
uint8_t
Decoding_wordCount(Decoding this);
#endif//$XCC_h

extern const uint8_t
Decoding_WordCount[];
uint8_t
Decoding_wordCount(Decoding this)
{
  Except_REQUIRE((unsigned)this < Decoding__);
  return Decoding_WordCount[this];
}

const uint8_t
Decoding_WordCount[] = {
#define WORDCOUNT(d) d
#define Decoding(ID, PROCESSOR, WORDTYPE, WORDCOUNT) \
  WORDCOUNT,
  WORDCOUNT(0),
#include "Decoding.tuple"
};

#ifdef $XCC__c
{
  int decoding = 0;
  for (; decoding < Decoding__; decoding++) {
    printf("WordCount(%s)\t%d\n", Decoding_Id(decoding), Decoding_wordCount(decoding));
  }
  fprintf(stderr, "sizeof(Decoding_WordCount)\t%zu\n", sizeof(Decoding_WordCount));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Decoding word size.
 */
uint8_t
Decoding_wordSize(Decoding this);
#endif//$XCC_h

extern const uint8_t
Decoding_WordSize[];
uint8_t
Decoding_wordSize(Decoding this)
{
  Except_REQUIRE((unsigned)this < Decoding__);
  return Decoding_WordSize[this];
}

const uint8_t
Decoding_WordSize[] = {
#define WORDTYPE(d) sizeof(d)
#define Decoding(ID, PROCESSOR, WORDTYPE, WORDCOUNT) \
  WORDTYPE,
  0,
#include "Decoding.tuple"
};

#ifdef $XCC__c
{
  int decoding = 0;
  for (; decoding < Decoding__; decoding++) {
    printf("wordSize(%s)\t%d\n", Decoding_Id(decoding), Decoding_wordSize(decoding));
  }
  fprintf(stderr, "sizeof(Decoding_WordSize)\t%zu\n", sizeof(Decoding_WordSize));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Decoding decode function.
 */
typedef Opcode (*DecodingDecode)(const void *buffer);
DecodingDecode
Decoding_decode(Decoding this);
#endif//$XCC_h

#ifndef NDECODE
extern const DecodingDecode
Decoding_Decode[];
DecodingDecode
Decoding_decode(Decoding this)
{
  Except_REQUIRE((unsigned)this < Decoding__);
  return Decoding_Decode[this];
}
#endif//NDECODE

#ifndef NDECODE
const DecodingDecode
Decoding_Decode[] = {
#define Decoding(ID, PROCESSOR, WORDTYPE, WORDCOUNT) \
  Decode_Decoding_##ID,
  NULL,
#include "Decoding.tuple"
};
#endif//NDECODE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Decoding module.
 */
#define Decoding_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Decoding module.
 */
#define Decoding_FINI()
#endif//$XCC__h

#if XCC__C
static void
Decoding_TEST(void)
{
#include "MDT/Decoding_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Decoding_TEST);
  return 0;
}
#endif

