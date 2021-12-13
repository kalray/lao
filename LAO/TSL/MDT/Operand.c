#ifdef $XCC_h
/*
 * !!!!	Operand.xcc
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
#include "MDT/Operand.h"
#endif//$XCC__h

#ifdef $XCC_h
typedef uint64_t OperandEncoded;
typedef uint64_t OperandDecoded;
#endif//$XCC_h

#ifdef $XCC__h
typedef enum {
#define Operand(ID, METHOD, WORDTYPE, ENCODE, DECODE) \
  Operand_##ID,
  Operand_,
#include /*MDT*/"Operand.tuple"
  Operand__
#undef Operand
} enum_Operand;
#endif//$XCC__h

const int Operand__Count = Operand__;

#ifdef $XCC__h
typedef enum {
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
  Operands_##ID,
  Operands_,
#include /*MDT*/"Operand.tuple"
  Operands__
#undef Operand
} enum_Operands;
#endif//$XCC__h

const int Operands__Count = Operands__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Operand enumeration.
 */
extern const char *
Operand_Id(Operand this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Operand_Id(Operand this)
{
  static const char *_[] = {
#define Operand(ID, METHOD, WORDTYPE, ENCODE, DECODE) \
    #ID,
    "_UNDEF",
#include "Operand.tuple"
  };
  Except_REQUIRE((unsigned)this < Operand__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int operand = 0;
  for (operand = 0; operand < Operand__; operand++) {
    printf("Operand_%s\t%d\n", Operand_Id(operand), operand);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef enum {
  OperandType_,
  OperandType_Register,
  OperandType_RegClass,
  OperandType_Immediate,
  OperandType_Modifier,
  OperandType_RegMask,
  OperandType__
} enum_OperandType;
typedef uint8_t OperandType;
extern const char *
OperandType_Id(OperandType this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
OperandType_Id(OperandType this)
{
  static const char *_[] = {
    "_UNDEF",
    "Register",
    "RegClass",
    "Immediate",
    "Modifier",
    "RegMask",
  };
  Except_REQUIRE((unsigned)this < OperandType__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * The OperandType of this Operand.
 */
OperandType
Operand_type(Operand this);
#endif//$XCC_h

extern const OperandType
Operand_Type[];
OperandType
Operand_type(Operand this)
{
  Except_REQUIRE((unsigned)this < Operand__);
  return (OperandType)Operand_Type[this];
}

const OperandType
Operand_Type[] = {
#define METHOD(type, name) OperandType_##type
#define Operand(ID, METHOD, WORDTYPE, ENCODE, DECODE) \
  METHOD,
  OperandType_,
#include "Operand.tuple"
#undef METHOD
};

#ifdef $XCC__c
{
  int operand = 0;
  for (operand = 0; operand < Operand__; operand++) {
    printf("\nOperandType(%s)\t%s", Operand_Id(operand), OperandType_Id(Operand_type(operand)));
  }
  printf("\n");
  fprintf(stderr, "sizeof(Operand_Type)\t%zu\n", sizeof(Operand_Type));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The RegClass or Immediate or Modifier or Regmask of this Operand.
 */
int
Operand_enum(Operand this);
#endif//$XCC_h

extern const int8_t
Operand_Enum[];
int
Operand_enum(Operand this)
{
  Except_REQUIRE((unsigned)this < Operand__);
  return Operand_Enum[this];
}

const int8_t
Operand_Enum[] = {
#define METHOD(type, name) type##_##name
#define Operand(ID, METHOD, WORDTYPE, ENCODE, DECODE) \
  METHOD,
  OperandType_,
#include "Operand.tuple"
#undef METHOD
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The Modifier of this Operand.
 */
Modifier
Operand_modifier(Operand this);
#endif//$XCC_h

Modifier
Operand_modifier(Operand this)
{
  if (Operand_type(this) == OperandType_Modifier) {
    return (Modifier)Operand_Enum[this];
  }
  return Modifier_;
}

#ifdef $XCC__c
{
  int operand = 0;
  for (operand = 0; operand < Operand__; operand++) {
    printf("\nOperand_modifier(%s)\t%s", Operand_Id(operand),
           Modifier_Id(Operand_modifier(operand)));
  }
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The Immediate of this Operand.
 */
Immediate
Operand_immediate(Operand this);
#endif//$XCC_h

Immediate
Operand_immediate(Operand this)
{
  if (Operand_type(this) == OperandType_Immediate) {
    return (Immediate)Operand_Enum[this];
  }
  return Immediate_;
}

#ifdef $XCC__c
{
  int operand = 0;
  for (operand = 0; operand < Operand__; operand++) {
    printf("\nOperand_immediate(%s)\t%s", Operand_Id(operand),
           Immediate_Id(Operand_immediate(operand)));
  }
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The RegClass of this Operand.
 */
RegClass
Operand_regClass(Operand this);
#endif//$XCC_h

RegClass
Operand_regClass(Operand this)
{
  if (Operand_type(this) == OperandType_RegClass) {
    return (RegClass)Operand_Enum[this];
  }
  return RegClass_;
}

#ifdef $XCC__c
{
  int operand = 0;
  for (operand = 0; operand < Operand__; operand++) {
    printf("\nOperand_regClass(%s)\t%s", Operand_Id(operand),
           RegClass_Id(Operand_regClass(operand)));
  }
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The RegMask of this Operand.
 */
RegMask
Operand_regMask(Operand this);
#endif//$XCC_h

RegMask
Operand_regMask(Operand this)
{
  if (Operand_type(this) == OperandType_RegMask) {
    return (RegMask)Operand_Enum[this];
  }
  return RegMask_;
}

#ifdef $XCC__c
{
  int operand = 0;
  for (operand = 0; operand < Operand__; operand++) {
    printf("\nOperand_regMask(%s)\t%s", Operand_Id(operand),
           RegMask_Id(Operand_regMask(operand)));
  }
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Operand encode function.
 */
typedef void (*OperandEncode)(OperandDecoded decoded, void *restrict codeWords);
OperandEncode
Operand_encode(Operand this);
#endif//$XCC_h

extern const OperandEncode
Operand_Encode[];
OperandEncode
Operand_encode(Operand this)
{
  Except_REQUIRE((unsigned)this < Operand__);
  return Operand_Encode[this];
}

#define METHOD(type, name) type##_Encode_##name(decoded)
#define WORDTYPE(type) type
#define ENCODE(e) e
#define Operand(ID, METHOD, WORDTYPE, ENCODE, DECODE) \
static void \
Operand_Encode_##ID(OperandDecoded decoded, void *restrict codeWords) \
{ \
  OperandEncoded VALUE = METHOD; \
  WORDTYPE *restrict WORDS = codeWords; \
  ENCODE; \
}
#include "Operand.tuple"
#undef ENCODE
#undef WORDTYPE
#undef METHOD

static void
Operand_Encode_(OperandDecoded decoded, void *restrict codeWords) { }

const OperandEncode
Operand_Encode[] = {
#define Operand(ID, METHOD, WORDTYPE, ENCODE, DECODE) \
  Operand_Encode_##ID,
  Operand_Encode_,
#include "Operand.tuple"
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  int operand = 0;
  for (operand = 0; operand < Operand__; operand++) {
    int immediate = Operand_immediate(operand);
    int64_t minValue = Immediate_minValue(immediate);
    int64_t maxValue = Immediate_maxValue(immediate);
    ImmediateValue value = (ImmediateValue)minValue;
    if (immediate != Immediate_) {
      long long encoded[2] = {0, 0};
      (*Operand_encode(operand))((OperandDecoded)(value), encoded);
      if(encoded[1]) {
        printf("\nOperand_encodeImmediateValue(%s,0x%llx)\t0x%llx%016llx",
            Operand_Id(operand), (long long)minValue, (long long)encoded[1], (long long)encoded[0]);
      } else {
        printf("\nOperand_encodeImmediateValue(%s,0x%llx)\t0x%llx",
            Operand_Id(operand), (long long)minValue, encoded[0]);
      }
      value = (ImmediateValue)maxValue;
      (*Operand_encode(operand))((OperandDecoded)(value), encoded);
      if(encoded[1]) {
        printf("\nOperand_encodeImmediateValue(%s,0x%llx)\t0x%llx%016llx",
            Operand_Id(operand), (long long)maxValue, (long long)encoded[1], (long long)encoded[0]);
      } else {
        printf("\nOperand_encodeImmediateValue(%s,0x%llx)\t0x%llx",
            Operand_Id(operand), (long long)maxValue, encoded[0]);
      }
    }
  }
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Operand decode function.
 */
typedef OperandDecoded (*OperandDecode)(const void *restrict codeWords);
OperandDecode
Operand_decode(Operand this);
#endif//$XCC_h

extern const OperandDecode
Operand_Decode[];
OperandDecode
Operand_decode(Operand this)
{
  Except_REQUIRE((unsigned)this < Operand__);
  return Operand_Decode[this];
}

#ifdef $XCC__h
/*
 * Operand_Decode function.
 */
#define WORDTYPE(type) type
#define DECODE(e) e
#define METHOD(type, name) type##_Decode_##name(VALUE)
#define Operand(ID, METHOD, WORDTYPE, ENCODE, DECODE) \
static inline OperandDecoded \
Operand_Decode_##ID(const void *codeWords) \
{ \
  const WORDTYPE *restrict WORDS = (const WORDTYPE *)codeWords; \
  OperandEncoded VALUE = 0; \
  DECODE; \
  return METHOD; \
}
#include "Operand.tuple"
#undef METHOD
#undef DECODE
#undef WORDTYPE
#endif//$XCC__h

const OperandDecode
Operand_Decode[] = {
#define Operand(ID, METHOD, WORDTYPE, ENCODE, DECODE) \
  Operand_Decode_##ID,
  NULL,
#include "Operand.tuple"
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  int operand = 0;
  OpcodeEncoded_ codeWords[1];
  for (operand = 0; operand < Operand__; operand++) {
    OperandEncode operandEncode = Operand_encode(operand);
    OperandDecode operandDecode = Operand_decode(operand);
    OperandType type = Operand_type(operand);
    printf("\nTest OperandEncode(%s)", Operand_Id(operand));
    if (type == OperandType_RegClass) {
      int regClass = Operand_regClass(operand);
      const_RegisterSet registerSet = RegClass_registerSet(regClass);
      RegisterSet_FOREACH_Register(registerSet, registre) {
        OperandDecoded result = -1;
        OperandDecoded decoded = registre;
        OpcodeEncoded_Ctor(codeWords);
        (*operandEncode)(decoded, codeWords);
        result = (*operandDecode)(codeWords);
        if (decoded != result) {
          printf("\nDecode(Encode(%s,%llx))\t%llx",
                 Operand_Id(operand), (long long)decoded, (long long)result);
        }
      } RegisterSet_ENDEACH_Register;
    } else if (type == OperandType_Immediate) {
      int immediate = Operand_immediate(operand);
      int64_t minValue = Immediate_minValue(immediate);
      int64_t maxValue = Immediate_maxValue(immediate);
      ImmediateExtend extend = Immediate_extend(immediate);
      if (minValue > maxValue) {
      } else if (   extend == ImmediateExtend_Signed
                 || extend == ImmediateExtend_Unsigned) {
        int maxIter = 65536;
        int64_t value = minValue;
        for (; value <= maxValue; value++) {
          OperandDecoded result = -1;
          OperandDecoded decoded = value;
          OpcodeEncoded_Ctor(codeWords);
          (*operandEncode)(decoded, codeWords);
          result = (*operandDecode)(codeWords);
          if (decoded != result) {
            printf("\nDecode(Encode(%s,%llx))\t%llx",
                   Operand_Id(operand), (long long)decoded, (long long)result);
          }
          if (--maxIter <= 0) break;
        }
        if (maxIter <= 0) {
          OperandDecoded result = -1;
          OperandDecoded decoded = maxValue;
          OpcodeEncoded_Ctor(codeWords);
          (*operandEncode)(decoded, codeWords);
          result = (*operandDecode)(codeWords);
          if (decoded != result) {
            printf("\nDecode(Encode(%s,%llx))\t%llx",
                   Operand_Id(operand), (long long)decoded, (long long)result);
          }
        }
      }
    }
  }
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Sequence of Operand(s).
 */
struct OperandList_ {
  uint8_t COUNT;
  Operand ITEMS[Operands__MAXOPERANDS];
};
#endif//$XCC__h

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(OperandList_)\t%zu\n", sizeof(OperandList_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Count the Operand(s) in this OperandList.
 */
static inline int
OperandList_count(const_OperandList this)
{
  return OperandList_COUNT(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Access the Operand at the given index.
 */
static inline Operand
OperandList_access(const_OperandList this, int index)
{
  Except_REQUIRE(index >= 0 && index < OperandList_COUNT(this));
  return (Operand)OperandList_ITEMS(this)[index];
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * For use by OperandList_FOREACH_Operand.
 */
static inline const Operand *
OperandList_items(const_OperandList this)
{
  return OperandList_ITEMS(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this OperandList Operand(s).
 */
#define OperandList_FOREACH_Operand(this, operand) { \
  const Operand *OperandList_ITEMS = OperandList_items(this); \
  int OperandList_COUNT = OperandList_count(this), OperandList_INDEX = 0; \
  for (; OperandList_INDEX < OperandList_COUNT; OperandList_INDEX++) { \
    int operand = OperandList_ITEMS[OperandList_INDEX];
#define OperandList_ENDEACH_Operand \
  } \
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Operands enumeration.
 */
extern const char *
Operands_Id(Operands this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Operands_Id(Operands this)
{
  static const char *_[] = {
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
    #ID,
    MDT_TARGET "_",
#include "Operand.tuple"
  };
  Except_REQUIRE((unsigned)this < Operands__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int operands = 0;
  for (operands = 0; operands < Operands__; operands++) {
    printf("Operands_%s\t%d\n", Operands_Id(operands), operands);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * This Operands OperandList.
 */
const_OperandList
Operands_operandList(Operands this);
#endif//$XCC__h

extern const OperandList_
Operands_OperandList[];
const_OperandList
Operands_operandList(Operands this)
{
  Except_REQUIRE((unsigned)this < Operands__);
  return &Operands_OperandList[this];
}

const OperandList_
Operands_OperandList[] = {
#define OPERAND(ID) Operand_##ID,
#define OPERANDS(count, items) count, { items }
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
  { OPERANDS },
  { OPERANDS(0, /**/) },
#include "Operand.tuple"
#undef OPERAND
};

#ifdef $XCC__h
/**
 * Iterate over this Operands Operand(s).
 */
#define Operands_FOREACH_Operand(this, operand) \
  OperandList_FOREACH_Operand(Operands_operandList(this), operand) { \
  int Operands_INDEX = OperandList_INDEX;
#define Operands_ENDEACH_Operand \
  } OperandList_ENDEACH_Operand;
#endif//$XCC__h

#ifdef $XCC__c
{
  int operands = 0;
  for (operands = 0; operands < Operands__; operands++) {
    printf("\nOperandList(%s)", Operands_Id(operands));
    Operands_FOREACH_Operand(operands, operand) {
      printf("\t%s", Operand_Id(operand));
    } Operands_ENDEACH_Operand;
  }
  printf("\n");
  fprintf(stderr, "sizeof(Operands_OperandList)\t%zu\n", sizeof(Operands_OperandList));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Operands count of Operand(s).
 */
int
Operands_count(Operands this);
#endif//$XCC_h

int
Operands_count(Operands this)
{
  Except_REQUIRE((unsigned)this < Operands__);
  return OperandList_count(&Operands_OperandList[this]);
}

#ifdef $XCC_h
/**
 * Access the Operand at the given index.
 */
Operand
Operands_access(Operands this, int index);
#endif//$XCC_h

Operand
Operands_access(Operands this, int index)
{
  Except_REQUIRE((unsigned)this < Operands__);
  return OperandList_access(&Operands_OperandList[this], index);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Buffer used to encode or decode Operands.
 */
struct OperandsBuffer_ {
  OperandDecoded DECODED[Operands__MAXOPERANDS];
};
#endif//$XCC_h

#ifdef $XCC__c
{
  printf("\n");
  fprintf(stderr, "sizeof(OperandsBuffer_)\t%zu\n", sizeof(OperandsBuffer_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This Operands encode function.
 */
typedef void (*OperandsEncode)(const_OperandsBuffer buffer, void *restrict codeWords);
OperandsEncode
Operands_encode(Operands this);
#endif//$XCC_h

extern const OperandsEncode
Operands_Encode[];
OperandsEncode
Operands_encode(Operands this)
{
  Except_REQUIRE((unsigned)this < Operands__);
  return Operands_Encode[this];
}

#ifdef $XCC__h
/*
 * Operands_Encode functions
 */
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
void \
Operands_Encode_##ID(const_OperandsBuffer buffer, void *restrict codeWords);
#include "Operand.tuple"
void
Operands_Encode_(const_OperandsBuffer buffer, void *restrict codeWords);
#endif//$XCC__h

#define ENCODE(e) e
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
void \
Operands_Encode_##ID(const_OperandsBuffer buffer, void *restrict codeWords) \
{ \
  const OperandDecoded *decoded = OperandsBuffer_DECODED(buffer); \
  ENCODE; \
}
#include "Operand.tuple"
#undef ENCODE
void
Operands_Encode_(const_OperandsBuffer buffer, void *restrict codeWords) { }

const OperandsEncode
Operands_Encode[] = {
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
  Operands_Encode_##ID,
  Operands_Encode_,
#include "Operand.tuple"
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Rank of relocatable Immediate in this Operands.
 */
int
Operands_relocatableRank(Operands this);
#endif//$XCC_h

extern int8_t
Operands_RelocatableRank[];
int
Operands_relocatableRank(Operands this)
{
  Except_REQUIRE((unsigned)this < Operands__);
  return Operands_RelocatableRank[this];
}

int8_t
Operands_RelocatableRank[] = {
#define RANK(d) d
#define RELOCATABLE(rank, operand, immediate) rank
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
  RELOCATABLE,
  RELOCATABLE(RANK(0), OPERAND(_UNDEF), IMMEDIATE(_UNDEF)),
#include "Operand.tuple"
#undef RELOCATABLE
#undef RANK
};

#ifdef $XCC__c
{
  int operands = 0;
  for (operands = 0; operands < Operands__; operands++) {
    int relocatableRank = Operands_relocatableRank(operands);
    printf("\nRelocatableRank(%s)\t%d", Operands_Id(operands), relocatableRank);
  }
  printf("\n");
  fprintf(stderr, "sizeof(Operands_RelocatableRank)\t%zu\n", sizeof(Operands_RelocatableRank));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Relocatable Operand in this Operands.
 */
Operand
Operands_relocatableOperand(Operands this);
#endif//$XCC_h

extern const Operand
Operands_RelocatableOperand[];
Operand
Operands_relocatableOperand(Operands this)
{
  Except_REQUIRE((unsigned)this < Operands__);
  return (Operand)Operands_RelocatableOperand[this];
}

const Operand
Operands_RelocatableOperand[] = {
#define OPERAND(name) Operand_##name
#define RELOCATABLE(rank, operand, immediate) operand
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
  RELOCATABLE,
  RELOCATABLE(RANK(0), OPERAND(_UNDEF), IMMEDIATE(_UNDEF)),
#include "Operand.tuple"
#undef RELOCATABLE
#undef OPERAND
};

#ifdef $XCC__c
{
  int operands = 0;
  for (operands = 0; operands < Operands__; operands++) {
    int relocatableOperand = Operands_relocatableOperand(operands);
    printf("RelocatableOperand(%s)\t%s\n", Operands_Id(operands), Operand_Id(relocatableOperand));
  }
  fprintf(stderr, "sizeof(Operands_RelocatableOperand)\t%zu\n", sizeof(Operands_RelocatableOperand));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Relocatable Immediate in this Operands.
 */
Immediate
Operands_relocatableImmediate(Operands this);
#endif//$XCC_h

extern const Immediate
Operands_RelocatableImmediate[];
Immediate
Operands_relocatableImmediate(Operands this)
{
  Except_REQUIRE((unsigned)this < Operands__);
  return (Immediate)Operands_RelocatableImmediate[this];
}

const Immediate
Operands_RelocatableImmediate[] = {
#define IMMEDIATE(name) Immediate_##name
#define RELOCATABLE(rank, operand, immediate) immediate
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
  RELOCATABLE,
  RELOCATABLE(RANK(0), OPERAND(_UNDEF), IMMEDIATE(_UNDEF)),
#include "Operand.tuple"
#undef RELOCATABLE
#undef IMMEDIATE
};

#ifdef $XCC__c
{
  int operands = 0;
  for (operands = 0; operands < Operands__; operands++) {
    int relocatableImmediate = Operands_relocatableImmediate(operands);
    printf("RelocatableImmediate(%s)\t%s\n", Operands_Id(operands), Immediate_Id(relocatableImmediate));
  }
  fprintf(stderr, "sizeof(Operands_RelocatableImmediate)\t%zu\n", sizeof(Operands_RelocatableImmediate));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#ifdef _NTRACE
#ifndef NDECODE
#define NDECODE
#endif//NDECODE
#endif//_NTRACE
#endif//$XCC_h

#ifdef $XCC_h
/*
  Operands_decode -- This Operands decode function.
 */
typedef void (*OperandsDecode)(const void *codeWords, OperandsBuffer buffer);
OperandsDecode
Operands_decode(Operands this);
#endif//$XCC_h

#ifndef NDECODE
extern OperandsDecode
Operands_Decode[];
OperandsDecode
Operands_decode(Operands this)
{
  Except_REQUIRE((unsigned)this < Operands__);
  return Operands_Decode[this];
}
#endif//NDECODE

#ifdef $XCC__h
/*
 * Operands_Decode functions.
 */
#ifndef NDECODE
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
void \
Operands_Decode_##ID(const void *codeWords, OperandsBuffer buffer);
#include "Operand.tuple"
#endif//NDECODE
#endif//$XCC__h

#ifndef NDECODE
#define DECODE(e) e
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
void \
Operands_Decode_##ID(const void *codeWords, OperandsBuffer buffer) \
{ \
  DECODE; \
}
#include "Operand.tuple"
#undef DECODE
void
Operands_Decode_(const void *restrict codeWords, OperandsBuffer buffer) { }
#endif//NDECODE

#ifdef $XCC_h
#ifndef NDECODE
void
Operands_Decode_(const void *codeWords, OperandsBuffer buffer);
#endif//NDECODE
#endif//$XCC_h

#ifndef NDECODE
OperandsDecode
Operands_Decode[] = {
#define Operands(ID, OPERANDS, RELOCATABLE, ENCODE, DECODE) \
  Operands_Decode_##ID,
  Operands_Decode_,
#include "Operand.tuple"
};
#endif//NDECODE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Operand module.
 */
#define Operand_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Operand module.
 */
#define Operand_FINI()
#endif//$XCC__h

#if XCC__C
static void
Operand_TEST(void)
{
#include "MDT/Operand_.c"
}


int
main(int argc, char **argv)
{
  MDT_Test(Operand_TEST);
  return 0;
}
#endif

