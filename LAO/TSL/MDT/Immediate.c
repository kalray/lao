#ifdef $XCC_h
/*
 * !!!!	Immediate.xcc
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
#include "MDT/Immediate.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Immediate(ID, MINVALUE, MAXVALUE, EXTEND, RELOCATIONS, ENCODE, DECODE) \
  Immediate_##ID,
  Immediate_,
#include /*MDT*/"Immediate.tuple"
  Immediate__
#undef Immediate
} enum_Immediate;
#endif//$XCC__h

const int Immediate__Count = Immediate__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Type for the largest Immediate value.  Must be signed value.
 */
typedef int64_t ImmediateValue;
#define ImmediateValue(x)	(ImmediateValue)(x)
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Immediate enumeration.
 */
extern const char *
Immediate_Id(Immediate this);
#endif//$XCC_h

#ifndef _NTRACE
extern const char *
Immediate_Id(Immediate this)
{
  static const char *_[] = {
#define Immediate(ID, MINVALUE, MAXVALUE, EXTEND, RELOCATIONS, ENCODE, DECODE) \
    #ID,
    "_UNDEF",
#include "Immediate.tuple"
  };
  Except_REQUIRE((unsigned)this < Immediate__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int immediate = 0;
  for (; immediate < Immediate__; immediate++) {
    printf("Immediate_%s\t%d\n", Immediate_Id(immediate), immediate);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Immediate MinValue attribute.
 */
int64_t
Immediate_minValue(Immediate this);
#endif//$XCC_h

extern const int64_t
Immediate_MinValue[];
int64_t
Immediate_minValue(Immediate this)
{
  Except_REQUIRE((unsigned)this < Immediate__);
  return Immediate_MinValue[this];
}

const int64_t
Immediate_MinValue[] = {
#define MINVALUE(d) d##LL
#define Immediate(ID, MINVALUE, MAXVALUE, EXTEND, RELOCATIONS, ENCODE, DECODE) \
  MINVALUE,
  MINVALUE(INT64_MAX+0),
#include "Immediate.tuple"
};

#ifdef $XCC__c
{
  int immediate = 0;
  for (; immediate < Immediate__; immediate++) {
    printf("Immediate_minValue(%s)\t%lld\n", Immediate_Id(immediate),
           (long long)Immediate_minValue(immediate));
  }
  fprintf(stderr, "sizeof(Immediate_MinValue)\t%zu\n", sizeof(Immediate_MinValue));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Immediate MaxValue attribute.
 */
int64_t
Immediate_maxValue(Immediate this);
#endif//$XCC_h

extern const int64_t
Immediate_MaxValue[];
int64_t
Immediate_maxValue(Immediate this)
{
  Except_REQUIRE((unsigned)this < Immediate__);
  return Immediate_MaxValue[this];
}

const int64_t
Immediate_MaxValue[] = {
#define MAXVALUE(d) d##LL
#define Immediate(ID, MINVALUE, MAXVALUE, EXTEND, RELOCATIONS, ENCODE, DECODE) \
  MAXVALUE,
  MAXVALUE(INT64_MIN+0),
#include "Immediate.tuple"
};

#ifdef $XCC__c
{
  int immediate = 0;
  for (; immediate < Immediate__; immediate++) {
    printf("Immediate_maxValue(%s)\t%lld\n", Immediate_Id(immediate),
           (long long)Immediate_maxValue(immediate));
  }
  fprintf(stderr, "sizeof(Immediate_MaxValue)\t%zu\n", sizeof(Immediate_MaxValue));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Immediate extend.
 */
typedef enum {
  ImmediateExtend_,
  ImmediateExtend_Signed,
  ImmediateExtend_Unsigned,
  ImmediateExtend_Wrap,
  ImmediateExtend__
} enum_ImmediateExtend;
typedef uint8_t ImmediateExtend;
extern const char *
ImmediateExtend_Id(ImmediateExtend this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ImmediateExtend_Id(ImmediateExtend this)
{
  static const char *_[] = {
    "_UNDEF",
    "Signed",
    "Unsigned",
    "Wrap",
  };
  Except_REQUIRE((unsigned)this < ImmediateExtend__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * This Immediate extend.
 */
ImmediateExtend
Immediate_extend(Immediate this);
#endif//$XCC_h

extern const ImmediateExtend
Immediate_Extend[];
ImmediateExtend
Immediate_extend(Immediate this)
{
  Except_REQUIRE((unsigned)this < Immediate__);
  return Immediate_Extend[this];
}

const ImmediateExtend
Immediate_Extend[] = {
#define EXTEND(e) ImmediateExtend_##e
#define Immediate(ID, MINVALUE, MAXVALUE, EXTEND, RELOCATIONS, ENCODE, DECODE) \
  EXTEND,
  EXTEND(/**/),
#include "Immediate.tuple"
};

#ifdef $XCC__c
{
  int immediate = 0;
  for (; immediate < Immediate__; immediate++) {
    printf("\nExtend(%s)\t%s", Immediate_Id(immediate),
                               ImmediateExtend_Id(Immediate_extend(immediate)));
  }
  printf("\n");
  fprintf(stderr, "sizeof(Immediate_Extend)\t%zu\n", sizeof(Immediate_Extend));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Sequence of Relocation(s).
 */
struct ImmediateRelocations_;
#endif//$XCC_h

#ifdef $XCC__h
struct ImmediateRelocations_ {
  int8_t COUNT;
  Relocation ITEMS[ImmediateRelocations_MAXCOUNT];
};
#endif//$XCC__h

#ifdef $XCC_h
/**
 * Count of Relocation(s) in this ImmediateRelocations.
 */
int
ImmediateRelocations_count(const_ImmediateRelocations this);
#endif//$XCC_h

int
ImmediateRelocations_count(const_ImmediateRelocations this)
{
  return ImmediateRelocations_COUNT(this);
}

#ifdef $XCC_h
/**
 * Relocation(s) in this ImmediateRelocations.
 */
const Relocation *
ImmediateRelocations_items(const_ImmediateRelocations this);
#endif//$XCC_h

const Relocation *
ImmediateRelocations_items(const_ImmediateRelocations this)
{
  return ImmediateRelocations_ITEMS(this);
}

#ifdef $XCC_h
/**
 * Access a Relocation in this ImmediateRelocations.
 */
Relocation
ImmediateRelocations_access(const_ImmediateRelocations this, int index);
#endif//$XCC_h

Relocation
ImmediateRelocations_access(const_ImmediateRelocations this, int index)
{
  Except_CHECK(index >=0 && index < ImmediateRelocations_COUNT(this));
  return (Relocation)ImmediateRelocations_ITEMS(this)[index];
}

#ifdef $XCC_h
/**
 * Iterate this ImmediateRelocations.
 */
#define ImmediateRelocations_FOREACH_Relocation(this, relocation) { \
  const Relocation *ImmediateRelocations_ITEMS = ImmediateRelocations_items(this); \
  int ImmediateRelocations_COUNT = ImmediateRelocations_count(this); \
  int ImmediateRelocations_INDEX = 0; \
  for (; ImmediateRelocations_INDEX < ImmediateRelocations_COUNT; \
       ImmediateRelocations_INDEX++) { \
    int relocation = ImmediateRelocations_ITEMS[ImmediateRelocations_INDEX];
#define ImmediateRelocations_ENDEACH_Relocation \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Immediate Relocations.
 */
const_ImmediateRelocations
Immediate_relocations(Immediate this);
#endif//$XCC_h

extern const ImmediateRelocations_
Immediate_Relocations[];
const_ImmediateRelocations
Immediate_relocations(Immediate this)
{
  Except_REQUIRE((unsigned)this < Immediate__);
  return &Immediate_Relocations[this];
}

const ImmediateRelocations_
Immediate_Relocations[] = {
#define RELOCATION(ID) Relocation_##ID,
#define RELOCATIONS(count, items) count, { items }
#define Immediate(ID, MINVALUE, MAXVALUE, EXTEND, RELOCATIONS, ENCODE, DECODE) \
  { RELOCATIONS },
  { RELOCATIONS(0, /**/) },
#include "Immediate.tuple"
};

#ifdef $XCC__c
{
  int immediate = 0;
  for (; immediate < Immediate__; immediate++) {
    const_ImmediateRelocations relocations = Immediate_relocations(immediate);
    printf("\nImmediate_relocations(%s)", Immediate_Id(immediate));
    ImmediateRelocations_FOREACH_Relocation(relocations, relocation) {
      printf("\t%s", Relocation_Id(relocation));
    } ImmediateRelocations_ENDEACH_Relocation;
  }
  printf("\n");
  fprintf(stderr, "sizeof(Immediate_Relocations)\t%zu\n", sizeof(Immediate_Relocations));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Check whether this Immediate can encode value.
 */
bool
Immediate_canEncode(Immediate this, int64_t value);
#endif//$XCC_h

bool
Immediate_canEncode(Immediate this, int64_t value)
{
  int64_t VALUE = value;
  switch (this) {
#define ENCODE(e) e
#define DECODE(e) e
#define Immediate(ID, MINVALUE, MAXVALUE, EXTEND, RELOCATIONS, ENCODE, DECODE) \
  case Immediate_##ID: { \
    { ENCODE; } \
    { DECODE; } \
    return VALUE == value; \
    break; \
  }
#include "Immediate.tuple"
  default:
    break;
  }
  return false;
}

#ifdef $XCC__c
{
  int immediate = 0;
  for (; immediate < Immediate__; immediate++) {
    long long minValue = Immediate_minValue(immediate);
    long long maxValue = Immediate_maxValue(immediate);
    if (minValue > maxValue) {
    } else if (maxValue - minValue >= 65536) {
    } else {
      long long value = minValue - 1;
      for (; value <= maxValue + 1; value++) {
        if (Immediate_canEncode(immediate, value)) {
        } else {
          printf("\nCannotEncode(%s,%lld)\tminValue=%lld\tmaxValue=%lld",
                 Immediate_Id(immediate), value, minValue, maxValue);
        }
      }
    }
  }
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * Immediate_Encode functions.
 */
#define ENCODE(e) e
#define Immediate(ID, MINVALUE, MAXVALUE, EXTEND, RELOCATIONS, ENCODE, DECODE) \
static inline OperandEncoded \
Immediate_Encode_##ID(OperandDecoded decoded) \
{ \
  ImmediateValue VALUE = (ImmediateValue)(decoded); \
  { ENCODE; } \
  return (OperandEncoded)VALUE; \
}
#include "Immediate.tuple"
#undef ENCODE
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * Immediate_Decode functions.
 */
#define DECODE(e) e
#define Immediate(ID, MINVALUE, MAXVALUE, EXTEND, RELOCATIONS, ENCODE, DECODE) \
static inline OperandDecoded \
Immediate_Decode_##ID(OperandEncoded encoded) \
{ \
  ImmediateValue VALUE = (ImmediateValue)encoded; \
  DECODE; \
  return VALUE; \
}
#include "Immediate.tuple"
#undef DECODE
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Immediate module.
 */
#define Immediate_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Immediate module.
 */
#define Immediate_FINI()
#endif//$XCC__h

#if XCC__C
static void
Immediate_TEST(void)
{
#include "MDT/Immediate_.c"
}

int
main(int argc, char **argv)
{
  MDT_INIT();
  Immediate_TEST();
  MDT_FINI();
  return 0;
}
#endif

