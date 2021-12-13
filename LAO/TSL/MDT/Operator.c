#ifdef $XCC_h
/*
 * !!!!	Operator.xcc
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
#include "MDT/Operator.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Operator(ID, ATTRIBUTES, MODIFIED, OPCODES, MNEMONIC, SYNTAX, PARAMETERARRAY) \
  Operator_##ID,
  Operator_,
#include /*MDT*/"Operator.tuple"
  Operator__
#undef Operator
} enum_Operator;
#endif//$XCC__h

const int Operator__Count = Operator__;
const int Operator__MaxParameters = OperatorParameterArray_MAXCOUNT;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef enum {
  OperatorParameterKind_Read,
  OperatorParameterKind_ReadCond,
  OperatorParameterKind_ReadPred,
  OperatorParameterKind_ReadBase,
  OperatorParameterKind_ReadOffset,
  OperatorParameterKind_ReadStored,
  OperatorParameterKind_ReadTested,
  OperatorParameterKind_ReadTarget,
  OperatorParameterKind_ReadPartial,
  OperatorParameterKind_ReadLoaded,
  OperatorParameterKind_ReadLeft,
  OperatorParameterKind_ReadRight,
  OperatorParameterKind_ReadList,
  OperatorParameterKind_ReadMulti,
  OperatorParameterKind_ReadMultiBase,
  OperatorParameterKind_ReadMultiOffset,
  OperatorParameterKind_ReadMultiTested,
  OperatorParameterKind_ReadMultiStored,
  OperatorParameterKind_ReadMultiPartial,
  OperatorParameterKind_ReadMultiLeft,
  OperatorParameterKind_ReadMultiRight,
  OperatorParameterKind_Write,
  OperatorParameterKind_WriteList,
  OperatorParameterKind_WriteMulti,
  OperatorParameterKind__
} enum_OperatorParameterKind;
typedef uint8_t OperatorParameterKind;
extern const char *
OperatorParameterKind_Id(OperatorParameterKind this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
OperatorParameterKind_Id(OperatorParameterKind this)
{
  static const char *_[] = {
    "Read",
    "ReadCond",
    "ReadPred",
    "ReadBase",
    "ReadOffset",
    "ReadStored",
    "ReadTested",
    "ReadTarget",
    "ReadPartial",
    "ReadLoaded",
    "ReadLeft",
    "ReadRight",
    "ReadList",
    "ReadMulti",
    "ReadMultiBase",
    "ReadMultiOffset",
    "ReadMultiTested",
    "ReadMultiStored",
    "ReadMultiPartial",
    "ReadMultiLeft",
    "ReadMultiRight",
    "Write",
    "WriteList",
    "WriteMulti",
  };
  Except_REQUIRE((unsigned)this < OperatorParameterKind__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
struct OperatorParameter_ {
  OperatorParameterKind KIND;
  //@access isRead	(OperatorParameter_kind(this) < OperatorParameterKind_Write)
  //@access isWrite	(OperatorParameter_kind(this) >= OperatorParameterKind_Write)
  int8_t RANK;			//!< The rank of this OperatorParameter in the Operand list.
  OperandType TYPE;		//!< The OperandType of this OperatorParameter.
  int16_t ENUM;			//!< Modifier|Immediate|RegMask|RegClass|Register enum.
  int8_t SAME;			//!< OperatorParameter xxxIndex with same operand.
};
#endif//$XCC_h

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(OperatorParameter_)\t%zu\n", sizeof(OperatorParameter_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * OperatorParameterKind of this OperatorParameter.
 */
static inline OperatorParameterKind
OperatorParameter_kind(const_OperatorParameter this)
{
  return (OperatorParameterKind)OperatorParameter_KIND(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Rank of this OperatorParameter in the Opcode Operand list.
 * @return 0 if the OperatorParameter has no explicit Operand.
 */
static inline int
OperatorParameter_rank(const_OperatorParameter this)
{
  return OperatorParameter_RANK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The OperandType of this OperatorParameter.
 */
static inline OperandType
OperatorParameter_type(const_OperatorParameter this)
{
  return (OperandType)OperatorParameter_TYPE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The Modifier/Immediate/RegClass/Regmask of this OperatorParameter.
 */
static inline int
OperatorParameter_enum(const_OperatorParameter this)
{
  return OperatorParameter_ENUM(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The Index of the OperatorParameter that must get
 * the same encoding as this OperatorParameter;  -1 if none.  Used to identify
 * Opcode Operands that are both read and written.
 */
static inline int
OperatorParameter_same(const_OperatorParameter this)
{
  return OperatorParameter_SAME(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

const OperatorParameter_
Operator_Parameters[] = {
#define KIND(name) OperatorParameterKind_##name
#define RANK(d) d
#define METHOD(type, name) OperandType_##type, type##_##name
#define SAME(d) d
#define PARAMETER(kind, rank, method, same) \
  { kind, rank, method, same },
#define PARAMETERS(parameters) parameters
#define OperatorParameters(INDEX, PARAMETERS) \
  PARAMETERS
  // No dummy entry because only used as Operator_Parameters + parametersIndex.
#include "Operator.tuple"
#undef PARAMETERS
#undef SAME
#undef METHOD
#undef RANK
#undef KIND
};

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Operator_Parameters)\t%zu\n", sizeof(Operator_Parameters));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Array of OperatorParameter(s).
 */
struct OperatorParameterArray_ {
  int16_t WRITECOUNT;			//!< Number of results
  int16_t READCOUNT;			//!< Number of arguments
  const OperatorParameter_ *ITEMS;	//!< Types of parameters
};
#endif//$XCC_h

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(OperatorParameterArray_)\t%zu\n", sizeof(OperatorParameterArray_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Argument count in this OperatorParameterArray.
 */
static inline int
OperatorParameterArray_readCount(const_OperatorParameterArray this)
{
  return OperatorParameterArray_READCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Result count in this OperatorParameterArray.
 */
static inline int
OperatorParameterArray_writeCount(const_OperatorParameterArray this)
{
  return OperatorParameterArray_WRITECOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * OperatorParameter count in this OperatorParameterArray.
 */
static inline int
OperatorParameterArray_count(const_OperatorParameterArray this)
{
  return OperatorParameterArray_READCOUNT(this) + OperatorParameterArray_WRITECOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by OperatorParameterArray_FOREACH_OperatorParameter.
 */
static inline const OperatorParameter_ *
OperatorParameterArray_items(const_OperatorParameterArray this)
{
  return OperatorParameterArray_ITEMS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pointer to the result OperatorParameter(s).
 */
static inline const OperatorParameter_ *
OperatorParameterArray_writeParameters(const_OperatorParameterArray this)
{
  return OperatorParameterArray_ITEMS(this) + 0;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this OperatorParameterArray write OperatorParameter(s).
 */
#define OperatorParameterArray_FORWRITE_OperatorParameter(this, parameter) { \
  const OperatorParameter_ *OperatorParameterArray_WRITES = OperatorParameterArray_writeParameters(this); \
  int OperatorParameterArray_WRITECOUNT = OperatorParameterArray_writeCount(this), OperatorParameterArray_INDEX = 0; \
  for (; OperatorParameterArray_INDEX < OperatorParameterArray_WRITECOUNT; OperatorParameterArray_INDEX++) { \
    const_OperatorParameter parameter = OperatorParameterArray_WRITES + OperatorParameterArray_INDEX;
#define OperatorParameterArray_ENDWRITE_OperatorParameter \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pointer to the argument OperatorParameter(s).
 */
static inline const OperatorParameter_ *
OperatorParameterArray_readParameters(const_OperatorParameterArray this)
{
  return OperatorParameterArray_ITEMS(this) + OperatorParameterArray_WRITECOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this OperatorParameterArray read OperatorParameter(s).
 */
#define OperatorParameterArray_FORREAD_OperatorParameter(this, parameter) { \
  const OperatorParameter_ *OperatorParameterArray_READS = OperatorParameterArray_readParameters(this); \
  int OperatorParameterArray_READCOUNT = OperatorParameterArray_readCount(this), OperatorParameterArray_INDEX = 0; \
  for (; OperatorParameterArray_INDEX < OperatorParameterArray_READCOUNT; OperatorParameterArray_INDEX++) { \
    const_OperatorParameter parameter = OperatorParameterArray_READS + OperatorParameterArray_INDEX;
#define OperatorParameterArray_ENDREAD_OperatorParameter \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this OperatorParameterArray OperatorParameter(s).
 */
#define OperatorParameterArray_FOREACH_OperatorParameter(this, parameter) { \
  const OperatorParameter_ *OperatorParameterArray_ITEMS = OperatorParameterArray_items(this); \
  int OperatorParameterArray_COUNT = OperatorParameterArray_count(this), OperatorParameterArray_INDEX = 0; \
  for (; OperatorParameterArray_INDEX < OperatorParameterArray_COUNT; OperatorParameterArray_INDEX++) { \
    const_OperatorParameter parameter = OperatorParameterArray_ITEMS + OperatorParameterArray_INDEX;
#define OperatorParameterArray_ENDEACH_OperatorParameter \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Operator enumeration.
 */
extern const char *
Operator_Id(Operator this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Operator_Id(Operator this)
{
  static const char *_[] = {
#define Operator(ID, ATTRIBUTES, MODIFIED, OPCODES, MNEMONIC, SYNTAX, PARAMETERARRAY) \
    #ID,
    "_UNDEF",
#include "Operator.tuple"
  };
  Except_REQUIRE((unsigned)this < Operator__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    printf("Operator_%s\t%d\n", Operator_Id(operator), operator);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef enum {
  OperatorAttribute_Pseudo,		//!< Pseudo operation.
  OperatorAttribute_Privilege,		//!< Requires kernel privilege.
  OperatorAttribute_ReadList,		//!< Has a variable length read list.
  OperatorAttribute_WriteList,		//!< Has a variable length write list.
  OperatorAttribute_MemorySafe,		//!< Memory access to safe address.
  OperatorAttribute_MemoryRead,		//!< Reads the contents of memory.
  OperatorAttribute_MemoryWrite,	//!< Writes the contents of memory.
  OperatorAttribute_MemoryTouch,	//!< Memory access for performance.
  OperatorAttribute_MemoryLevel2,	//!< Memory access at level 2 (bypass L1 cache).
  OperatorAttribute_MemoryEffect,	//!< Memory access has side-effects.
  OperatorAttribute_MemoryFence,	//!< Has memory fence semantics.
  OperatorAttribute_MemorySpill,	//!< Memory access for spill code.
  OperatorAttribute_MemorySize1,	//!< Memory access size is 1 byte.
  OperatorAttribute_MemorySize2,	//!< Memory access size is 2 bytes.
  OperatorAttribute_MemorySize4,	//!< Memory access size is 4 bytes.
  OperatorAttribute_MemorySize8,	//!< Memory access size is 8 bytes.
  OperatorAttribute_MemorySize16,	//!< Memory access size is 16 bytes.
  OperatorAttribute_MemorySize32,	//!< Memory access size is 32 bytes.
  OperatorAttribute_MemorySize64,	//!< Memory access size is 64 bytes.
  OperatorAttribute_MemoryAligned,	//!< Memory access is aligned on size.
  OperatorAttribute_Dismissible,	//!< Dismissible operation, may speculate.
  OperatorAttribute_Constrained,	//!< Operand renaming is constrained.
  OperatorAttribute_Conditional,	//!< Has some effects, whatever the condition.
  OperatorAttribute_Predicated,		//!< Has no effects when the condition is false.
  OperatorAttribute_Extension,		//!< Extension operation (executes in coprocessor).
  OperatorAttribute_FixupRAW,		//!< Need to fixup the RAW dependence latencies.
  OperatorAttribute_Stalling,		//!< May stall the processor, better not speculate.
  OperatorAttribute_Volatile,		//!< Volatile operations must be kept in sequence.
  OperatorAttribute_Control,		//!< May change the program control flow, ends basic block.
  OperatorAttribute_Bitwise,		//!< Bitwise logic operator such as AND, IOR, XOR, NOT, etc.
  OperatorAttribute_Multi,		//!< An operation variant where register t-uples are expanded.
  OperatorAttribute_Skip,		//!< Operation has no effects.
} OperatorAttribute;
typedef uint32_t OperatorAttributes;
#define Operator_isPseudo(this) \
  (Operator_attributes(this)>>OperatorAttribute_Pseudo & 1)
#define Operator_isPrivilege(this) \
  (Operator_attributes(this)>>OperatorAttribute_Privilege & 1)
#define Operator_isReadList(this) \
  (Operator_attributes(this)>>OperatorAttribute_ReadList & 1)
#define Operator_isWriteList(this) \
  (Operator_attributes(this)>>OperatorAttribute_WriteList & 1)
#define Operator_isMemorySafe(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemorySafe & 1)
#define Operator_isMemoryRead(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemoryRead & 1)
#define Operator_isMemoryWrite(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemoryWrite & 1)
#define Operator_isMemoryTouch(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemoryTouch & 1)
#define Operator_isMemoryLevel2(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemoryLevel2 & 1)
#define Operator_isMemoryEffect(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemoryEffect & 1)
#define Operator_isMemoryFence(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemoryFence & 1)
#define Operator_isMemorySpill(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemorySpill & 1)
#define Operator_isMemorySize1(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemorySize1 & 1)
#define Operator_isMemorySize2(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemorySize2 & 1)
#define Operator_isMemorySize4(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemorySize4 & 1)
#define Operator_isMemorySize8(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemorySize8 & 1)
#define Operator_isMemorySize16(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemorySize16 & 1)
#define Operator_isMemorySize32(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemorySize32 & 1)
#define Operator_isMemorySize64(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemorySize64 & 1)
#define Operator_isMemoryAligned(this) \
  (Operator_attributes(this)>>OperatorAttribute_MemoryAligned & 1)
#define Operator_isDismissible(this) \
  (Operator_attributes(this)>>OperatorAttribute_Dismissible & 1)
//#define Operator_isConstrained(this)
//  (Operator_attributes(this)>>OperatorAttribute_Constrained & 1)
#define Operator_isConditional(this) \
  (Operator_attributes(this)>>OperatorAttribute_Conditional & 1)
#define Operator_isPredicated(this) \
  (Operator_attributes(this)>>OperatorAttribute_Predicated & 1)
#define Operator_isExtension(this) \
  (Operator_attributes(this)>>OperatorAttribute_Extension & 1)
#define Operator_isFixupRAW(this) \
  (Operator_attributes(this)>>OperatorAttribute_FixupRAW & 1)
#define Operator_isStalling(this) \
  (Operator_attributes(this)>>OperatorAttribute_Stalling & 1)
#define Operator_isVolatile(this) \
  (Operator_attributes(this)>>OperatorAttribute_Volatile & 1)
#define Operator_isControl(this) \
  (Operator_attributes(this)>>OperatorAttribute_Control & 1)
#define Operator_isBitwise(this) \
  (Operator_attributes(this)>>OperatorAttribute_Bitwise & 1)
#define Operator_isMulti(this) \
  (Operator_attributes(this)>>OperatorAttribute_Multi & 1)
#define Operator_isSkip(this) \
  (Operator_attributes(this)>>OperatorAttribute_Skip & 1)
#define Operator_isMemory(this) ( \
     (Operator_attributes(this)>>OperatorAttribute_MemorySafe & 1) \
  || (Operator_attributes(this)>>OperatorAttribute_MemoryRead & 1) \
  || (Operator_attributes(this)>>OperatorAttribute_MemoryWrite & 1) \
  || (Operator_attributes(this)>>OperatorAttribute_MemoryTouch & 1) \
  || (Operator_attributes(this)>>OperatorAttribute_MemoryLevel2 & 1) \
  || (Operator_attributes(this)>>OperatorAttribute_MemoryEffect & 1) \
  || (Operator_attributes(this)>>OperatorAttribute_MemoryFence & 1) \
  || (Operator_attributes(this)>>OperatorAttribute_MemorySpill & 1) \
)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access this Operator attributes.
 */
OperatorAttributes
Operator_attributes(Operator this);
#endif//$XCC_h

extern const OperatorAttributes
Operator_Attributes[];
OperatorAttributes
Operator_attributes(Operator this)
{
  Except_REQUIRE((unsigned)this < Operator__);
  return Operator_Attributes[this];
}

const OperatorAttributes
Operator_Attributes[] = {
#define ATTRIBUTE(attribute) (1 << OperatorAttribute_##attribute) |
#define ATTRIBUTES(count, attributes) attributes 0
#define Operator(ID, ATTRIBUTES, MODIFIED, OPCODES, MNEMONIC, SYNTAX, PARAMETERARRAY) \
  ATTRIBUTES,
  0,
#include "Operator.tuple"
};


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef struct {
  int8_t RANK;			//!< The rank of this OperatorModify in the Operand list.
  ModifierMember MEMBER;	//!< The OperandType of this OperatorParameter.
} OperatorModify_, *OperatorModify;
#endif//$XCC_h

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(OperatorModify_)\t%zu\n", sizeof(OperatorModify_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This Operator expanded modifiers.
 */
OperatorModify_
Operator_modify_(Operator this, int index);
#endif//$XCC_h

#if OperatorModify_MAXCOUNT > 0
extern OperatorModify_
Operator_Modified[][OperatorModify_MAXCOUNT];
#endif//OperatorModify_MAXCOUNT

#if OperatorModify_MAXCOUNT > 0
OperatorModify_
Operator_Modified[][OperatorModify_MAXCOUNT] = {
#define MODIFIED(count, array) { array }
#define MODIFY(RANK, ID) { RANK, ModifierMember_##ID },
#define RANK(n) n
#define Operator(ID, ATTRIBUTES, MODIFIED, OPCODES, MNEMONIC, SYNTAX, PARAMETERARRAY) \
  MODIFIED,
  MODIFIED(0, /**/),
#include "Operator.tuple"
#undef RANK
#undef MODIFY
#undef MODIFIED
};
#endif//OperatorModify_MAXCOUNT

OperatorModify_
Operator_modify_(Operator this, int index)
{
  static OperatorModify_ null_;
  Except_REQUIRE((unsigned)this < Operator__);
#if OperatorModify_MAXCOUNT > 0
  if (index < OperatorModify_MAXCOUNT) {
    return Operator_Modified[this][index];
  }
#endif//OperatorModify_MAXCOUNT
  return null_;
}

#ifdef $XCC__c
#if OperatorModify_MAXCOUNT > 0
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int index = 0;
    OperatorModify_ modify_ = Operator_modify_(operator, index++);
    if (modify_.RANK) {
      printf("Modify(%s)\t", Operator_Id(operator));
      do {
         printf("%d:%s\t", modify_.RANK, ModifierMember_name(modify_.MEMBER));
         modify_ = Operator_modify_(operator, index++);
      } while (modify_.RANK);
      printf("\n");
    }
  }
  fprintf(stderr, "sizeof(Operator_Modified)\t%zu\n", sizeof(Operator_Modified));
}
#endif//OperatorModify_MAXCOUNT
#endif//$XCC__c


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Operator Opcode for a Processor.
 */
Opcode 
Operator_opcode(Operator this, Processor processor);
#endif//$XCC_h

extern const Opcode
Operator_Opcodes[][Processor__];
Opcode 
Operator_opcode(Operator this, Processor processor)
{
  Except_REQUIRE((unsigned)this < Operator__);
  Except_REQUIRE((unsigned)processor < Processor__);
  return (Opcode)Operator_Opcodes[this][processor];
}

const Opcode
Operator_Opcodes[][Processor__] = {
#define OPCODES(array) { Opcode_/*Processor_*/, array }
#define OPCODE(CORE, ID) Opcode_##ID,
#define Operator(ID, ATTRIBUTES, MODIFIED, OPCODES, MNEMONIC, SYNTAX, PARAMETERARRAY) \
  OPCODES,
  OPCODES(/**/),
#include "Operator.tuple"
#undef OPCODE
#undef OPCODES
};

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int processor = 0;
    printf("Opcodes(%s)\t", Operator_Id(operator));
    for (processor = Processor_ + 1; processor < Processor__; processor++) {
      int opcode = Operator_opcode(operator, processor);
      printf("%s\t", Opcode_Id(opcode));
    }
    printf("\n");
  }
  fprintf(stderr, "sizeof(Operator_Opcodes)\t%zu\n", sizeof(Operator_Opcodes));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Operator Mnemonic attribute.
 */
const char *
Operator_mnemonic(Operator this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Operator_Mnemonic[] = {
#define MNEMONIC(NAME) NAME
#define Operator(ID, ATTRIBUTES, MODIFIED, OPCODES, MNEMONIC, SYNTAX, PARAMETERARRAY) \
  MNEMONIC,
  NULL,
#include "Operator.tuple"
};
#endif//_NTRACE

#ifndef _NTRACE
/**
 * Get the mnemonic of an operator.
 */
const char *
Operator_mnemonic(Operator this)
{
  Except_REQUIRE((unsigned)this < Operator__);
  return Operator_Mnemonic[this];
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Get an Operator from a string mnemonic.
 *
 * @return the first operator that matches the mnemonic.
 */
Operator
Operator_ofMnemonic(const char *string, Processor processor);
#endif//$XCC_h

Operator
Operator_ofMnemonic(const char *string, Processor processor)
{
  Operator opgeneric = Operator__UNDEF;
#define MNEMONIC(NAME) NAME
#define Operator(ID, ATTRIBUTES, MODIFIED, OPCODES, MNEMONIC, SYNTAX, PARAMETERARRAY) \
  if(!strcmp(MNEMONIC, string)) { \
    if (Operator_opcode(Operator_ ##ID, processor) != Opcode__UNDEF ) { \
      return Operator_ ##ID; \
    } \
    else { \
      opgeneric = Operator_ ##ID; \
    }\
  }
  if(!strcmp("",string)) {
    return Operator__UNDEF;
  }
#include "Operator.tuple"
  return opgeneric;
}

#ifdef $XCC__c
{
  Operator operator;
  Processor processor;
#define TESTMNEM(MNEMONIC, ID) \
  { \
    for (processor = Processor_ + 1; processor < Processor__; processor++) {\
      operator = Operator_ofMnemonic(MNEMONIC, processor); \
      Except_CHECK(!strcmp(MNEMONIC, Operator_Mnemonic[operator])); \
      printf("Operator mnemonic of string '%s' for processor '%s' is Operator_%s\n", \
             MNEMONIC, Processor_getName(processor), #ID); \
    }\
  }
#define MNEMONIC(NAME) NAME
#define Operator(ID, ATTRIBUTES, MODIFIED, OPCODES, MNEMONIC, SYNTAX, PARAMETERARRAY) \
  TESTMNEM(MNEMONIC, ID)
#include "Operator.tuple"
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Check if following operators have the same mnemonic.
 */
bool
Operator_otherMnemonic(Operator this);
#endif//$XCC_h

bool
Operator_otherMnemonic(Operator this)
{
  if(this+1 == Operator__) return false;
  if(!strcmp(Operator_mnemonic(this),
             Operator_mnemonic(this+1))) return true;
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Operator Syntax attribute.
 */
const char *
Operator_syntax(Operator this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Operator_Syntax[] = {
#define SYNTAX(NAME) NAME
#define Operator(ID, ATTRIBUTES, MODIFIED, OPCODES, MNEMONIC, SYNTAX, PARAMETERARRAY) \
  SYNTAX,
  NULL,
#include "Operator.tuple"
};
#endif//_NTRACE

#ifndef _NTRACE
/**
 * Get the syntax of an operator.
 */
const char *
Operator_syntax(Operator this)
{
  Except_REQUIRE((unsigned)this < Operator__);
  return Operator_Syntax[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Operator shortName attribute.
*/
const char *
Operator_shortName(Operator this);
#endif//$XCC_h

const char *
Operator_shortName(Operator this)
{
  return Operator_Id(this);
}

#ifdef $XCC_h
/**
 * Get an Operator from a string shortName.
 *
 * @return the operator that matches the shortName.
 * @pre the shortNames (generated by MDS) should be unique.
 */
Operator
Operator_fromShortName(const char *name, Processor processor);
#endif//$XCC_h

Operator
Operator_fromShortName(const char *name, Processor processor)
{
  Operator operator;
  Operator opgeneric = Operator__UNDEF;
  size_t size = strlen(MDT_TARGET) + strlen(name) + 1;
  char prename[size]; strcat(strcpy(prename, MDT_TARGET), name);
  for (operator = 1; operator < Operator__; operator++) {
    if (   !strcmp(Operator_Id(operator), name)
        || !strcmp(Operator_Id(operator), prename)) {
//       try first to find operator for a specified processor
      if (Operator_opcode(operator, processor) != Opcode__UNDEF) {
        return operator;
      } else {
//FIXME: if we found valid operator but not for specified processor
//       does it mean we have a generic operator??
        opgeneric = operator;
      }
    }
  }
  return opgeneric;
}


#ifdef $XCC_h
/**
 * Get an Operator from a string shortName, or mnemonic if it fails.
 *
 */
Operator
Operator_fromShortName_or_Mnemonic(const char *string, Processor processor);
#endif//$XCC_h

Operator
Operator_fromShortName_or_Mnemonic(const char *string, Processor processor)
{
  Operator operator;
  operator = Operator_fromShortName(string, processor);
  if(operator == Operator__UNDEF) {
    operator = Operator_ofMnemonic(string, processor);
  }
  return operator;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Operator OperatorParameterArray.
 */
const_OperatorParameterArray
Operator_parameterArray(Operator this);
#endif//$XCC_h

extern const OperatorParameterArray_
Operator_ParameterArray[];
const_OperatorParameterArray
Operator_parameterArray(Operator this)
{
  Except_REQUIRE((unsigned)this < Operator__);
  return Operator_ParameterArray + this;
}

const OperatorParameterArray_
Operator_ParameterArray[] = {
#define WRITECOUNT(d) d
#define READCOUNT(d) d
#define PARAMETERSINDEX(index) index
#define PARAMETERARRAY(writeCount, readCount, parametersIndex) \
  { writeCount, readCount, Operator_Parameters + parametersIndex }
#define Operator(ID, ATTRIBUTES, MODIFIED, OPCODES, MNEMONIC, SYNTAX, PARAMETERARRAY) \
  PARAMETERARRAY,
  { 0 },
#include "Operator.tuple"
};

#ifdef $XCC_h
/**
 * This Operator count of argument OperatorParameter(s).
 */
static inline int
Operator_readCount(Operator this) {
  const_OperatorParameterArray parameterArray = Operator_parameterArray(this);
  return OperatorParameterArray_readCount(parameterArray);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Operator count of result OperatorParameter(s).
 */
static inline int
Operator_writeCount(Operator this) {
  const_OperatorParameterArray parameterArray = Operator_parameterArray(this);
  return OperatorParameterArray_writeCount(parameterArray);
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    const_OperatorParameterArray parameterArray = Operator_parameterArray(operator);
    printf("Parameters(%s)\n\twriteCount=%d\treadCount=%d\n", Operator_Id(operator),
           Operator_writeCount(operator), Operator_readCount(operator));
    OperatorParameterArray_FOREACH_OperatorParameter(parameterArray, parameter) {
      OperatorParameterKind kind = OperatorParameter_kind(parameter);
      int rank = OperatorParameter_rank(parameter);
      OperandType type = OperatorParameter_type(parameter);
      int same = OperatorParameter_same(parameter);
      printf("\tkind=%s\trank=%d\ttype=%s",
             OperatorParameterKind_Id(kind), rank, OperandType_Id(type));
      if (type == OperandType_Register) {
        int registre = OperatorParameter_enum(parameter);
        printf("\tRegister %s", Register_Id(registre));
      } else if (type == OperandType_RegClass) {
        int regClass = OperatorParameter_enum(parameter);
        printf("\tRegClass_%s", RegClass_Id(regClass));
      } else if (type == OperandType_Immediate) {
        int immediate = OperatorParameter_enum(parameter);
        printf("\tImmediate_%s", Immediate_Id(immediate));
      } else if (type == OperandType_Modifier) {
        int modifier = OperatorParameter_enum(parameter);
        printf("\tModifier_%s", Modifier_Id(modifier));
      } else if (type == OperandType_RegMask) {
        int regMask = OperatorParameter_enum(parameter);
        printf("\tRegMask_%s", RegMask_Id(regMask));
      } else Except_NEVER(true);
      if (same >= 0) printf("\tsame=%d", same);
      printf("\n");
    } OperatorParameterArray_ENDEACH_OperatorParameter;
  }
  fprintf(stderr, "sizeof(Operator_ParameterArray)\t%zu\n", sizeof(Operator_ParameterArray));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Access the read OperatorParameter at readIndex.
 */
static inline const_OperatorParameter
Operator_readParameter(Operator this, int readIndex)
{
  const_OperatorParameterArray parameterArray = Operator_parameterArray(this);
  Except_REQUIRE(   readIndex >= 0 
                 && readIndex < OperatorParameterArray_READCOUNT(parameterArray));
  return OperatorParameterArray_readParameters(parameterArray) + readIndex;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access the write OperatorParameter at writeIndex.
 */
static inline const_OperatorParameter
Operator_writeParameter(Operator this, int writeIndex)
{
  const_OperatorParameterArray parameterArray = Operator_parameterArray(this);
  Except_REQUIRE(   writeIndex >= 0
                 && writeIndex < OperatorParameterArray_WRITECOUNT(parameterArray));
  return OperatorParameterArray_writeParameters(parameterArray) + writeIndex;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The Operator read index where the OperandType is.
 */
int8_t
Operator_getReadOperandType(Operator this, OperandType type);
#endif//$XCC_h

int8_t
Operator_getReadOperandType(Operator this, OperandType type)
{
  const_OperatorParameterArray parameterArray = Operator_parameterArray(this);
  const OperatorParameter_ *readParameters =
      OperatorParameterArray_readParameters(parameterArray);
  int readCount = OperatorParameterArray_readCount(parameterArray), index;
  for (index = 0; index < readCount; index++) {
    if (OperatorParameter_type(readParameters + index) == type) return index;
  }
  return -1;
}

#ifdef $XCC_h
#define Operator_immediateIndex(this)\
  Operator_getReadOperandType(this, OperandType_Immediate)
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int8_t immediateIndex = Operator_immediateIndex(operator);
    if (immediateIndex >= 0) {
      printf("ImmediateIndex(%s)\t%d\n", Operator_Id(operator), immediateIndex);
    }
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The Operator read index where the OperatorParameterKind is.
 */
int8_t
Operator_getReadParameterKind(Operator this, OperatorParameterKind kind);
#endif//$XCC_h

int8_t
Operator_getReadParameterKind(Operator this, OperatorParameterKind kind)
{
  const_OperatorParameterArray parameterArray = Operator_parameterArray(this);
  const OperatorParameter_ *readParameters =
      OperatorParameterArray_readParameters(parameterArray);
  int readCount = OperatorParameterArray_readCount(parameterArray), index;
  for (index = 0; index < readCount; index++) {
    if (OperatorParameter_kind(readParameters + index) == kind) return index;
  }
  return -1;
}

#ifdef $XCC_h
#define Operator_targetIndex(this)\
  Operator_getReadParameterKind(this, OperatorParameterKind_ReadTarget)
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int8_t targetIndex = Operator_targetIndex(operator);
    if (targetIndex >= 0) {
      printf("TargetIndex(%s)\t%d\n", Operator_Id(operator), targetIndex);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
#define Operator_baseIndex(this)\
  Operator_getReadParameterKind(this, OperatorParameterKind_ReadBase)
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int8_t baseIndex = Operator_baseIndex(operator);
    if (baseIndex >= 0) {
      printf("BaseIndex(%s)\t%d\n", Operator_Id(operator), baseIndex);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
#define Operator_offsetIndex(this)\
  Operator_getReadParameterKind(this, OperatorParameterKind_ReadOffset)
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int8_t offsetIndex = Operator_offsetIndex(operator);
    if (offsetIndex >= 0) {
      printf("OffsetIndex(%s)\t%d\n", Operator_Id(operator), offsetIndex);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
#define Operator_storedIndex(this)\
  Operator_getReadParameterKind(this, OperatorParameterKind_ReadStored)
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int8_t storedIndex = Operator_storedIndex(operator);
    if (storedIndex >= 0) {
      printf("StoredIndex(%s)\t%d\n", Operator_Id(operator), storedIndex);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
#define Operator_leftIndex(this)\
  Operator_getReadParameterKind(this, OperatorParameterKind_ReadLeft)
#define Operator_rightIndex(this)\
  Operator_getReadParameterKind(this, OperatorParameterKind_ReadRight)
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int8_t leftIndex = Operator_leftIndex(operator);
    int8_t rightIndex = Operator_rightIndex(operator);
    if (rightIndex >= 0 || leftIndex >= 0) {
      printf("LeftRightIndex(%s)\t%d\t%d\n", Operator_Id(operator), leftIndex, rightIndex);
    }
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * True if Register RAW latency needs a fix.
 */
bool
Operator_isFixRegisterRAW(Operator this);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The register lifetime flow requirements.
 */
int
Operator_require(Operator this, RegFile regFile);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The index of this Operator base address OperatorParameter(s).
 * @return The index starting at 0 of base address OperatorParameter among reads.
 */
int
Operator_baseAddressIndex(Operator this);
#endif//$XCC_h

int
Operator_baseAddressIndex(Operator this)
{
  int index = 0;
  const_OperatorParameterArray parameterArray = Operator_parameterArray(this);
  OperatorParameterArray_FORREAD_OperatorParameter(parameterArray, parameter) {
    OperatorParameterKind kind = OperatorParameter_kind(parameter);
    if (kind == OperatorParameterKind_ReadBase) return index;
    index++;
  } OperatorParameterArray_ENDREAD_OperatorParameter;
  return -1;
}

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int8_t baseAddressIndex = Operator_baseAddressIndex(operator);
    if (baseAddressIndex >= 0) {
      printf("BaseAddressIndex(%s)\t%d\n", Operator_Id(operator), baseAddressIndex);
    }
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Test if this Operator is a XXX.
 */
#define Operator_isEntry(this)	((this) == Operator_ENTRY)
#define Operator_isLabel(this)	((this) == Operator_LABEL)
#define Operator_isFall(this)	((this) == Operator_FALLTO)
#define Operator_isLoop(this)	((this) == Operator_LOOPTO)
#define Operator_isPhi(this)	((this) == Operator_PHI)
#define Operator_isPsi(this)	((this) == Operator_PSI)
#define Operator_isSigma(this)	((this) == Operator_SIGMA)
#define Operator_isSsa(this)	((this) >= Operator_PHI && (this) <= Operator_SIGMA)
#define Operator_isKill(this)	((this) == Operator_KILL)
#define Operator_isECopy(this)	((this) == Operator_ECOPY)
#define Operator_isLCopy(this)	((this) == Operator_LCOPY)
#define Operator_isPCopy(this)	((this) == Operator_PCOPY)
#define Operator_isParCopy(this) \
  ((unsigned)((this) - Operator_PCOPY) <= (unsigned)(Operator_LCOPY - Operator_PCOPY))
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isSsa(operator)) {
      printf("IsSsa(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isPseudo(operator)) {
      printf("IsPseudo(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isPrivilege(operator)) {
      printf("IsPrivilege(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemorySafe(operator)) {
      printf("IsMemorySafe(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemoryRead(operator)) {
      printf("IsMemoryRead(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemoryWrite(operator)) {
      printf("IsMemoryWrite(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemoryTouch(operator)) {
      printf("IsMemoryTouch(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemoryLevel2(operator)) {
      printf("IsMemoryLevel2(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemoryEffect(operator)) {
      printf("IsMemoryEffect(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemoryFence(operator)) {
      printf("IsMemoryFence(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemorySize1(operator)) {
      printf("IsMemorySize1(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemorySize2(operator)) {
      printf("IsMemorySize2(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemorySize4(operator)) {
      printf("IsMemorySize4(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemorySize8(operator)) {
      printf("IsMemorySize8(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemorySize16(operator)) {
      printf("IsMemorySize16(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemorySize32(operator)) {
      printf("IsMemorySize32(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemorySize64(operator)) {
      printf("IsMemorySize64(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isMemoryAligned(operator)) {
      printf("IsMemoryAligned(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Get the memory access size in bytes.
 */
static inline int
Operator_memorySize(Operator this)
{
  return Operator_isMemorySize1(this)*1 +
         Operator_isMemorySize2(this)*2 +
         Operator_isMemorySize4(this)*4 +
         Operator_isMemorySize8(this)*8 +
         Operator_isMemorySize16(this)*16 +
         Operator_isMemorySize32(this)*32 +
         Operator_isMemorySize64(this)*64;
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int8_t memorySize = Operator_memorySize(operator);
    if (memorySize > 0) {
      printf("MemorySize(%s)\t%d\n", Operator_Id(operator), memorySize);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Get the memory access alignment in bytes.
 */
static inline int
Operator_memoryAlign(Operator this)
{
  if (Operator_isMemoryAligned(this)) {
    return Operator_memorySize(this);
  } else {
    return Operator_isMemorySize1(this);
  }
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int8_t memoryAlign = Operator_memoryAlign(operator);
    if (memoryAlign > 0) {
      printf("MemoryAlign(%s)\t%d\n", Operator_Id(operator), memoryAlign);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isDismissible(operator)) {
      printf("IsDismissible(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is constrained on Operand(s) (a Parameter with SAME constraint.)
 */
bool
Operator_isConstrained(Operator this);
#endif//$XCC_h

bool
Operator_isConstrained(Operator this)
{
  int writeCount = Operator_writeCount(this), sameSum = 0, i = 0;
  const_OperatorParameterArray parameterArray = Operator_parameterArray(this);
  const OperatorParameter_ *writeParameters = OperatorParameterArray_writeParameters(parameterArray);
  for (; i < writeCount; i++) {
    sameSum -= OperatorParameter_same(writeParameters + i);
  }
  return sameSum != writeCount;
}

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isConstrained(operator)) {
      printf("IsConstrained(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isConditional(operator)) {
      printf("IsConditional(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isPredicated(operator)) {
      printf("IsPredicated(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isExtension(operator)) {
      printf("IsExtension(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isFixupRAW(operator)) {
      printf("IsFixupRAW(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isStalling(operator)) {
      printf("IsStalling(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isVolatile(operator)) {
      printf("IsVolatile(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isBitwise(operator)) {
      printf("IsBitwise(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isControl(operator)) {
      printf("IsControl(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isParCopy(operator)) {
      printf("IsParCopy(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is a COPY.
 *
 * In this case result 0 is the destination and argument 0 is the source.
 */
bool
Operator_isSeqCopy(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isSeqCopy(operator)) {
      printf("IsCopy(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is an ADD.
 */
bool
Operator_isAddressAdd(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isAddressAdd(operator)) {
      printf("IsAdd(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is an ADJUST.
 */
bool
Operator_isAdjust(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isAdjust(operator)) {
      printf("IsAdjust(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is a GOTO operator.
 *
 * A GOTO operator branches to a PC-relative or an absolute address, and has one
 * or two targets.
 */
bool
Operator_isGoTo(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isGoTo(operator)) {
      printf("IsGoTo(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is a JUMP operator.
 *
 * A JUMP operator branches to an address contained in a register.
 */
bool
Operator_isJump(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isJump(operator)) {
      printf("IsJump(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c


#ifdef $XCC_h
/**
 * Test if this Operator is a branch operator.
 *
 * A branch is a control-flow change that remains inside the function.
 */
bool
Operator_isBranch(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isBranch(operator)) {
      printf("IsBranch(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is a CALL operator.
 *
 * A CALL operator branches to a PC-relative or an absolute address.
 */
bool
Operator_isCall(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isCall(operator)) {
      printf("IsCall(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is a LINK operator.
 */
bool
Operator_isLink(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isLink(operator)) {
      printf("IsLink(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is a function call operator.
 */
bool
Operator_isRoutine(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isRoutine(operator)) {
      printf("IsRoutine(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is a function return operator.
 */
bool
Operator_isReturn(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isReturn(operator)) {
      printf("IsReturn(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is a GNU ASM.
 */
bool
Operator_isGNUASM(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isGNUASM(operator)) {
      printf("IsGNUASM(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is Simulated
 */
bool
Operator_isSimulated(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    if (Operator_isSimulated(operator)) {
      printf("IsSimulated(%s)\n", Operator_Id(operator));
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Speculate this Operator.
 */
Operator
Operator_speculate(Operator this, Processor processor);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int processor = 0;
    for (processor = Processor_ + 1; processor < Processor__; processor++) {
      int speculate = Operator_speculate(operator, processor);
      if (speculate != Operator_ && speculate != operator) {
        printf("Speculate(%s,%s)=\t%s\n", Operator_Id(operator),
               Processor_Id(processor), Operator_Id(speculate));
      }
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * The predicated on true variant of this Operator.
 */
Operator
Operator_predicatePositive(Operator this, Processor processor);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int processor = 0;
    for (processor = Processor_ + 1; processor < Processor__; processor++) {
      int predicatePositive = Operator_predicatePositive(operator, processor);
      if (predicatePositive != Operator_) {
        printf("PredicatePositive(%s,%s)=\t%s\n", Operator_Id(operator),
               Processor_Id(processor), Operator_Id(predicatePositive));
      }
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * The predicated on false variant of this Operator.
 */
Operator
Operator_predicateNegative(Operator this, Processor processor);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int processor = 0;
    for (processor = Processor_ + 1; processor < Processor__; processor++) {
      int predicateNegative = Operator_predicateNegative(operator, processor);
      if (predicateNegative != Operator_) {
        printf("PredicateNegative(%s,%s)=\t%s\n", Operator_Id(operator),
               Processor_Id(processor), Operator_Id(predicateNegative));
      }
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This Operator predicate argument index.
 */
int
Operator_predicateIndex(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int predicateIndex = Operator_predicateIndex(operator);
    if (predicateIndex >= 0) {
      printf("PredicateIndex(%s)=\t%d\n", Operator_Id(operator), predicateIndex);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This Operator predicate polarity.
 */
int
Operator_predicatePolarity(Operator this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int predicatePolarity = Operator_predicatePolarity(operator);
    if (predicatePolarity != 0) {
      printf("PredicatePolarity(%s)=\t%d\n", Operator_Id(operator), predicatePolarity);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * The predicated on false variant of this Operator.
 */
Operator
Operator_unpredicate(Operator this, Processor processor);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int processor = 0;
    for (processor = Processor_ + 1; processor < Processor__; processor++) {
      int unpredicate = Operator_unpredicate(operator, processor);
      if (unpredicate != Operator_) {
        printf("Unpredicate(%s,%s)=\t%s\n", Operator_Id(operator),
               Processor_Id(processor), Operator_Id(unpredicate));
      }
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Complement this operator on the designated write parameter.
 */
Operator
Operator_complementWrite(Operator this, Processor processor, int32_t writeIndex);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int processor = 0;
    for (processor = Processor_ + 1; processor < Processor__; processor++) {
      int writeCount = Operator_writeCount(operator), writeIndex = 0;
      for (; writeIndex < writeCount; writeIndex++) {
        int complementWrite = Operator_complementWrite(operator, processor, writeIndex);
        if (complementWrite != Operator_) {
          printf("ComplementWrite(%s,%s,%d)=\t%s\n", Operator_Id(operator),
                 Processor_Id(processor), writeIndex, Operator_Id(complementWrite));
        }
      }
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Complement this operator on the designated read parameter.
 */
Operator
Operator_complementRead(Operator this, Processor processor, int32_t readIndex);
#endif//$XCC_h

#ifdef $XCC__c
{
  int operator = 0;
  for (; operator < Operator__; operator++) {
    int processor = 0;
    for (processor = Processor_ + 1; processor < Processor__; processor++) {
      int readCount = Operator_readCount(operator), readIndex = 0;
      for (; readIndex < readCount; readIndex++) {
        int complementRead = Operator_complementRead(operator, processor, readIndex);
        if (complementRead != Operator_) {
          printf("ComplementRead(%s,%s,%d)=\t%s\n", Operator_Id(operator),
                 Processor_Id(processor), readIndex, Operator_Id(complementRead));
        }
      }
    }
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test if this Operator is a Hardware Loop Skip.
 */
bool
Operator_isHWLSkip(Operator this);
#endif//$XCC_h

bool
Operator_isHWLSkip(Operator this)
{
  return false;
}

#ifdef $XCC_h
/**
 * Test if this Operator is a Hardware Loop Break.
 */
bool
Operator_isHWLBreak(Operator this);
#endif//$XCC_h

bool
Operator_isHWLBreak(Operator this)
{
  return false;
}

#ifdef $XCC_h
/**
 * Test if this Operator is a Hardware Loop Iter.
 */
bool
Operator_isHWLIter(Operator this);
#endif//$XCC_h

bool
Operator_isHWLIter(Operator this)
{
  return false;
}

#ifdef $XCC_h
/**
 * Test if this Operator is a Hardware Loop Branch.
 */
bool
Operator_isHWLBranch(Operator this);
#endif//$XCC_h

bool
Operator_isHWLBranch(Operator this)
{
  if (this == Operator_LOOPTO)
    return true;
  else
    return false;
}

#ifdef $XCC_h
/**
 * Test if this Operator is a Hardware Loop Pseudo.
 */
bool
Operator_isHWLPseudo(Operator this);
#endif//$XCC_h

bool
Operator_isHWLPseudo(Operator this)
{
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Operator module.
 */
#define Operator_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Operator module.
 */
#define Operator_FINI()
#endif//$XCC__h

#if XCC__C
static void
Operator_TEST(void)
{
#include "MDT/Operator_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Operator_TEST);
  return 0;
}
#endif

