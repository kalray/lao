#ifdef $XCC_h
/*
 * !!!!	Opcode.xcc
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
#include "MDT/Opcode.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Opcode(ID, SCHEDULING, DECODING, CODEWORDS, OPERANDS, \
               INCREMENT, MNEMONIC, SYNTAX, ACTIONARRAY) \
  Opcode_##ID,
  Opcode_,
#include /*MDT*/"Opcode.tuple"
  Opcode__
#undef Opcode
} enum_Opcode;
#endif//$XCC__h

const int Opcode__Count = Opcode__;
const int Opcode__MaxActions = OpcodeAction_MAXCOUNT;

////////////////////////////////////////////////////////////////////////////////////////////////////

RegisterAction_
Opcode_Actions[] = {
#define REGISTER(name) Register_##name
#define REGCLASS(name) RegClass_##name
#define XXXINDEX(d) d
#define RANK(d) d
#define STAGE(d) d
#define ACTION(register, regClass, xxxIndex, rank, stage) \
  { register, regClass, xxxIndex, rank, stage },
#define ACTIONS(actions) actions
#define OpcodeActions(INDEX, ACTIONS) \
  ACTIONS
  // No dummy entry because only used as Opcode_Actions + actionsIndex.
#include "Opcode.tuple"
#undef ACTIONS
};

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Opcode_Actions)\t%zu\n", sizeof(Opcode_Actions));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Opcode enumeration.
 */
extern const char *
Opcode_Id(Opcode this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Opcode_Id(Opcode this)
{
  static const char *_[] = {
#define Opcode(ID, SCHEDULING, DECODING, CODEWORDS, OPERANDS, \
               INCREMENT, MNEMONIC, SYNTAX, ACTIONARRAY) \
    #ID,
    "_UNDEF",
#include "Opcode.tuple"
  };
  Except_REQUIRE((unsigned)this < Opcode__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int opcode = 0;
  for (opcode = 0; opcode < Opcode__; opcode++) {
    printf("Opcode_%s\t%d\n", Opcode_Id(opcode), opcode);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Opcode Scheduling class.
 */
Scheduling
Opcode_scheduling(Opcode this);
#endif//$XCC_h

extern const Scheduling
Opcode_Scheduling[];
Scheduling
Opcode_scheduling(Opcode this)
{
  Except_REQUIRE((unsigned)this < Opcode__);
  return (Scheduling)Opcode_Scheduling[this];
}

const Scheduling
Opcode_Scheduling[] = {
#define SCHEDULING(ID) Scheduling_##ID
#define Opcode(ID, SCHEDULING, DECODING, CODEWORDS, OPERANDS, \
               INCREMENT, MNEMONIC, SYNTAX, ACTIONARRAY) \
  SCHEDULING,
  SCHEDULING(_UNDEF),
#include "Opcode.tuple"
};

#ifdef $XCC__c
{
  int opcode = 0;
  for (opcode = 0; opcode < Opcode__; opcode++) {
    Scheduling scheduling = Opcode_scheduling(opcode);
    printf("\nScheduling(%s)\t%s", Opcode_Id(opcode), Scheduling_Id(scheduling));
  }
  printf("\n");
  fprintf(stderr, "sizeof(Opcode_Scheduling)\t%zu\n", sizeof(Opcode_Scheduling));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Opcode Decoding.
 */
Decoding
Opcode_decoding(Opcode this);
#endif//$XCC_h

extern const Decoding
Opcode_Decoding[];
Decoding
Opcode_decoding(Opcode this)
{
  Except_REQUIRE((unsigned)this < Opcode__);
  return (Decoding)Opcode_Decoding[this];
}

const Decoding
Opcode_Decoding[] = {
#define DECODING(ID) Decoding_##ID
#define Opcode(ID, SCHEDULING, DECODING, CODEWORDS, OPERANDS, \
               INCREMENT, MNEMONIC, SYNTAX, ACTIONARRAY) \
  DECODING,
  DECODING(_UNDEF),
#include "Opcode.tuple"
};

#ifdef $XCC__c
{
  int opcode = 0;
  for (opcode = 0; opcode < Opcode__; opcode++) {
    int decoding = Opcode_decoding(opcode);
    printf("\nDecoding(%s)\t%s", Opcode_Id(opcode), Decoding_Id(decoding));
  }
  printf("\n");
  fprintf(stderr, "sizeof(Opcode_Decoding)\t%zu\n", sizeof(Opcode_Decoding));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#define OpcodeEncoded_MAXWORDS ((Decoding_MAXSIZE + sizeof(uint32_t) - 1)/sizeof(uint32_t))
struct OpcodeEncoded_ {
  //@args	
  uint32_t WORDS[OpcodeEncoded_MAXWORDS];
};
#endif//$XCC_h

OpcodeEncoded
OpcodeEncoded_Ctor(OpcodeEncoded this)
{
  memset(this, 0, sizeof(OpcodeEncoded_));
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(OpcodeEncoded_)\t%zu\n", sizeof(OpcodeEncoded_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Opcode codeWords.
 */
const_OpcodeEncoded
Opcode_encoded(Opcode this);
#endif//$XCC_h

extern const OpcodeEncoded_
Opcode_Encoded[];
const_OpcodeEncoded
Opcode_encoded(Opcode this)
{
  Except_REQUIRE((unsigned)this < Opcode__);
  return &Opcode_Encoded[this];
}

const OpcodeEncoded_
Opcode_Encoded[] = {
#define CODEWORD(codeWord) codeWord,
#define CODEWORDS(count, items) { { items } },
#define Opcode(ID, SCHEDULING, DECODING, CODEWORDS, OPERANDS, \
               INCREMENT, MNEMONIC, SYNTAX, ACTIONARRAY) \
  CODEWORDS
  CODEWORDS(1, CODEWORD(0))
#include "Opcode.tuple"
};

#ifdef $XCC__c
{
  int opcode = 0;
  for (opcode = 0; opcode < Opcode__; opcode++) {
    int decoding = Opcode_decoding(opcode);
    const_OpcodeEncoded codeWords = Opcode_encoded(opcode);
    Opcode (*decode)(const void *) = Decoding_decode(decoding);
    if (decode != NULL) {
      Opcode result = (*decode)(codeWords);
      if (result != opcode) {
        printf("\nDecode(Encode(%s))\t%s",
               Opcode_Id(opcode),
               Opcode_Id(result));
        if (result == Opcode_) {
          printf("\t%x\t%x\n", codeWords->WORDS[0], codeWords->WORDS[1]);
        }
      }
    }
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Opcode Operands.
 */
Operands
Opcode_operands(Opcode this);
#endif//$XCC_h

extern const Operands
Opcode_Operands[];
Operands
Opcode_operands(Opcode this)
{
  Except_REQUIRE((unsigned)this < Opcode__);
  return (Operands)Opcode_Operands[this];
}

const Operands
Opcode_Operands[] = {
#define OPERANDS(ID) Operands_##ID
#define Opcode(ID, SCHEDULING, DECODING, CODEWORDS, OPERANDS, \
               INCREMENT, MNEMONIC, SYNTAX, ACTIONARRAY) \
  OPERANDS,
  OPERANDS(_UNDEF),
#include "Opcode.tuple"
};

#ifdef $XCC__c
{
  int opcode = 0;
  for (opcode = 0; opcode < Opcode__; opcode++) {
    int operands = Opcode_operands(opcode);
    printf("\nOperands(%s)\t%s", Opcode_Id(opcode), Operands_Id(operands));
  }
  printf("\n");
  fprintf(stderr, "sizeof(Opcode_Operands)\t%zu\n", sizeof(Opcode_Operands));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Opcode PC Increment.
 */
int
Opcode_increment(Opcode this);
#endif//$XCC_h

extern const int8_t
Opcode_Increment[];
int
Opcode_increment(Opcode this)
{
  Except_REQUIRE((unsigned)this < Opcode__);
  return Opcode_Increment[this];
}

const int8_t
Opcode_Increment[] = {
#define INCREMENT(d) d
#define Opcode(ID, SCHEDULING, DECODING, CODEWORDS, OPERANDS, \
               INCREMENT, MNEMONIC, SYNTAX, ACTIONARRAY) \
  INCREMENT,
  INCREMENT(0),
#include "Opcode.tuple"
};

#ifdef $XCC__c
#if !Bundle_INSTRUCTIONS_MAX
{
  int opcode = 0;
  for (opcode = 0; opcode < Opcode__; opcode++) {
    int increment = Opcode_increment(opcode);
    printf("\nIncrement(%s)\t%d", Opcode_Id(opcode), increment);
  }
  printf("\n");
  fprintf(stderr, "sizeof(Opcode_Increment)\t%zu\n", sizeof(Opcode_Increment));
}
#endif//!Bundle_INSTRUCTIONS_MAX
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Opcode mnemonic string.
 */
const char *
Opcode_mnemonic(Opcode this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Opcode_Mnemonic[] = {
#define MNEMONIC(mnemonic) mnemonic
#define Opcode(ID, SCHEDULING, DECODING, CODEWORDS, OPERANDS, \
               INCREMENT, MNEMONIC, SYNTAX, ACTIONARRAY) \
  MNEMONIC,
  NULL,
#include "Opcode.tuple"
};
#endif//_NTRACE

#ifndef _NTRACE
const char *
Opcode_mnemonic(Opcode this)
{
  Except_REQUIRE((unsigned)this < Opcode__);
  return Opcode_Mnemonic[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int opcode = 0;
  for (opcode = 0; opcode < Opcode__; opcode++) {
    printf("\nMnemonic(%s)\t%s", Opcode_Id(opcode), Opcode_mnemonic(opcode));
  }
  printf("\n");
  fprintf(stderr, "sizeof(Opcode_Mnemonic)\t%zu\n", sizeof(Opcode_Mnemonic));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Opcode syntax string.
 */
const char *
Opcode_syntax(Opcode this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Opcode_Syntax[] = {
#define SYNTAX(syntax) syntax
#define Opcode(ID, SCHEDULING, DECODING, CODEWORDS, OPERANDS, \
               INCREMENT, MNEMONIC, SYNTAX, ACTIONARRAY) \
  SYNTAX,
  NULL,
#include "Opcode.tuple"
};
#endif//_NTRACE

#ifndef _NTRACE
const char *
Opcode_syntax(Opcode this)
{
  Except_REQUIRE((unsigned)this < Opcode__);
  return Opcode_Syntax[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int opcode = 0;
  for (opcode = 0; opcode < Opcode__; opcode++) {
    printf("\nSyntax(%s)\t%s", Opcode_Id(opcode), Opcode_syntax(opcode));
  }
  printf("\n");
  fprintf(stderr, "sizeof(Opcode_Syntax)\t%zu\n", sizeof(Opcode_Syntax));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Opcode unit cost estimate.
 */
int
Opcode_unitCost(Opcode this);
#endif//$XCC_h

/*
  Default spill cost values.
  TODO: Should be defined by architecture.
  The cost is compute as:
  cost = latency+issue/max_issue.
  The default costs are for a machine with:
  issue of remat: 1/4
  issue of load or store: 1/1
  latency of load: 3
  latency of store: 0
  This is the ST200 model.
 */
int
Opcode_unitCost(Opcode this)
{
  // FIXME! Use scheduling classes for this computation.
  int scheduling = Opcode_scheduling(this);
  int processor = Scheduling_processor(scheduling);
  int maxIssue = Resource_availability(Resource__ISSUE, processor);
  if (Opcode_isControl(this)) return (maxIssue*1)/1;
  if (Opcode_isMemoryWrite(this)) return (maxIssue*1)/1;
  if (Opcode_isMemoryRead(this)) return (maxIssue*(1+3))/1;
  return 1;	// (maxIssue*1)/maxIssue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Opcode RegisterActionArray.
 */
const_RegisterActionArray
Opcode_actionArray(Opcode this);
#endif//$XCC_h

extern /*const*/ RegisterActionArray_
Opcode_ActionArray[];
const_RegisterActionArray
Opcode_actionArray(Opcode this)
{
  Except_REQUIRE((unsigned)this < Opcode__);
  return Opcode_ActionArray + this;
}

/*const*/ RegisterActionArray_
Opcode_ActionArray[] = {
#define READCOUNT(d) d
#define READSTART(d) d
#define WRITECOUNT(d) d
#define WRITESTART(d) d
#define ACTIONSINDEX(index) index
#define ACTIONARRAY(readCount, readStart, writeCount, writeStart, actionsIndex) \
  { readCount, readStart, writeCount, writeStart, Opcode_Actions + actionsIndex }
#define Opcode(ID, SCHEDULING, DECODING, CODEWORDS, OPERANDS, \
               INCREMENT, MNEMONIC, SYNTAX, ACTIONARRAY) \
  ACTIONARRAY,
  { 0 },
#include "Opcode.tuple"
};

#ifdef $XCC__c
{
  int opcode = 0;
  for (; opcode < Opcode__; opcode++) {
    const_RegisterActionArray actionArray = Opcode_actionArray(opcode);
    printf("Opcode_%s:\n", Opcode_Id(opcode));
    printf("\treadCount=%d\treadStart=%d\twriteCount=%d\twriteStart=%d\n",
           RegisterActionArray_readCount(actionArray),
           RegisterActionArray_readStart(actionArray),
           RegisterActionArray_writeCount(actionArray),
           RegisterActionArray_writeStart(actionArray));
    RegisterActionArray_FORREAD_RegisterAction(actionArray, action) {
      int registre = RegisterAction_register(action);
      int regClass = RegisterAction_regClass(action);
      int xxxIndex = RegisterAction_xxxIndex(action);
      int rank = RegisterAction_rank(action);
      int stage = RegisterAction_stage(action);
      printf("\tRegister(%s)\tRegClass(%s)\txxxIndex=%d\trank=%d\tstage=%d\n",
             Register_Id(registre), RegClass_Id(regClass), xxxIndex, rank, stage);
    } RegisterActionArray_ENDREAD_RegisterAction;
    RegisterActionArray_FORWRITE_RegisterAction(actionArray, action) {
      int registre = RegisterAction_register(action);
      int regClass = RegisterAction_regClass(action);
      int xxxIndex = RegisterAction_xxxIndex(action);
      int rank = RegisterAction_rank(action);
      int stage = RegisterAction_stage(action);
      printf("\tRegister(%s)\tRegClass(%s)\txxxIndex=%d\trank=%d\tstage=%d\n",
             Register_Id(registre), RegClass_Id(regClass), xxxIndex, rank, stage);
    } RegisterActionArray_ENDWRITE_RegisterAction;
  }
  fprintf(stderr, "sizeof(Opcode_ActionArray)\t%zu\n", sizeof(Opcode_ActionArray));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make this Opcode RegisterActionArray_ITEMS.
 *
 * Only used to replace the MDT-initialized ITEMS with a writeable copy for patching.
 */
RegisterAction_ *
Opcode_makeRegisterActionItems(Opcode this, Memory memory);
#endif//$XCC_h

RegisterAction_ *
Opcode_makeRegisterActionItems(Opcode this, Memory memory)
{
  RegisterActionArray actionArray = Opcode_ActionArray + this;
  RegisterAction_ *old_items = RegisterActionArray_ITEMS(actionArray);
  int Opcode_Actions_count = sizeof(Opcode_Actions)/sizeof(Opcode_Actions[0]);
  if (   old_items >= Opcode_Actions
      && old_items < Opcode_Actions + Opcode_Actions_count) {
    size_t size = sizeof(RegisterAction_)*RegisterActionArray_count(actionArray);
    RegisterAction_ *new_items = Memory_alloc(memory, size);
    *RegisterActionArray__ITEMS(actionArray) = new_items;
    return memcpy(new_items, old_items, size);
  }
  return old_items;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef enum {
  OpcodeAttribute_Skip,
  OpcodeAttribute_Control,
  OpcodeAttribute_MemoryRead,
  OpcodeAttribute_MemoryWrite,
  OpcodeAttribute_FixupRAW,
} OpcodeAttribute;
typedef uint8_t OpcodeAttributes;
#endif//$XCC_h

#ifdef $XCC_h
#define Opcode_isSkip(this) \
  (Opcode_attributes(this)>>OpcodeAttribute_Skip & 1)
#define Opcode_isControl(this) \
  (Opcode_attributes(this)>>OpcodeAttribute_Control & 1)
#define Opcode_isMemoryRead(this) \
  (Opcode_attributes(this)>>OpcodeAttribute_MemoryRead & 1)
#define Opcode_isMemoryWrite(this) \
  (Opcode_attributes(this)>>OpcodeAttribute_MemoryWrite & 1)
#define Opcode_isFixupRAW(this) \
  (Opcode_attributes(this)>>OpcodeAttribute_FixupRAW & 1)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access this Opcode attributes.
 */
OpcodeAttributes
Opcode_attributes(Opcode this);
#endif//$XCC_h

extern /*const*/ OpcodeAttributes
Opcode_Attributes[];
OpcodeAttributes
Opcode_attributes(Opcode this)
{
  Except_REQUIRE((unsigned)this < Opcode__);
  return Opcode_Attributes[this];
}

/*const*/ OpcodeAttributes
Opcode_Attributes[Opcode__];

#ifdef $XCC__c
{
  int opcode = 0;
  for (; opcode < Opcode__; opcode++) {
    if (Opcode_attributes(opcode)) {
      printf("Opcode_%s:", Opcode_Id(opcode));
      if (Opcode_isControl(opcode)) printf("\tisControl");
      if (Opcode_isMemoryRead(opcode)) printf("\tisMemoryRead");
      if (Opcode_isMemoryWrite(opcode)) printf("\tisMemoryWrite");
      if (Opcode_isFixupRAW(opcode)) printf("\tisFixupRAW");
      printf("\n");
    }
  }
  fprintf(stderr, "sizeof(Opcode_Attributes)\t%zu\n", sizeof(Opcode_Attributes));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
unsigned
Opcode_fixupRAW(Opcode this, Opcode that);
#endif//$XCC_h

unsigned
Opcode_fixupRAW(Opcode this, Opcode that)
{
  unsigned attributes;
  Except_CHECK((unsigned)this < Opcode__);
  attributes = Opcode_Attributes[this] | Opcode_Attributes[that];
  return attributes & (1<<OpcodeAttribute_FixupRAW);
}

#ifdef $XCC_h
int
Opcode_biasRAW(Opcode this_write, Opcode that_read, StorageCell cell, int biasRAW);
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Use count of each Opcode.
 */
extern unsigned
Opcode_UseCount[];
#endif//$XCC_h

#ifndef MDT_USECOUNT
unsigned
Opcode_UseCount[Opcode__];
#endif//MDT_USECOUNT

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#define OpcodeBuffer_MAXWORDS ((Decoding_MAXSIZE + sizeof(uint32_t) - 1)/sizeof(uint32_t))
struct OpcodeBuffer_ {
  //@args	
  uint32_t WORDS[OpcodeBuffer_MAXWORDS];
  Decoding DECODING;
};
#endif//$XCC_h

OpcodeBuffer
OpcodeBuffer_Ctor(OpcodeBuffer this)
{
  memset(this, 0, sizeof(OpcodeBuffer_));
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(OpcodeBuffer_)\t%zu\n", sizeof(OpcodeBuffer_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Opcode module.
 */
void
Opcode_INIT(void);
#endif//$XCC__h

void
Opcode_INIT(void)
{
  int operator = 0;
  int processor = 0;
  OpcodeAttributes *attributes = Opcode_Attributes;
  for (; operator < Operator__; operator++) {
    for (processor = 1; processor < Processor__; processor++) {
      int opcode = Operator_opcode(operator, processor);
      if (opcode != Opcode_) {
        if (Operator_isControl(operator)) {
          attributes[opcode] |= 1<<OpcodeAttribute_Control;
        }
        if (Operator_isMemoryRead(operator) || Operator_isMemoryEffect(operator)) {
          attributes[opcode] |= 1<<OpcodeAttribute_MemoryRead;
        }
        if (Operator_isMemoryWrite(operator) || Operator_isMemoryEffect(operator)) {
          attributes[opcode] |= 1<<OpcodeAttribute_MemoryWrite;
        }
        if (Operator_isFixupRAW(operator)) {
          attributes[opcode] |= 1<<OpcodeAttribute_FixupRAW;
        }
      }
    }
  }
#ifndef MDT_USECOUNT
  if (MDT_USECOUNT) {
    memset(Opcode_UseCount, 0, sizeof(Opcode_UseCount));
  }
#endif//MDT_USECOUNT
}

#ifdef $XCC__h
/**
 * Finalize the Opcode module.
 */
void
Opcode_FINI(void);
#endif//$XCC__h

void
Opcode_FINI(void)
{
#ifndef MDT_USECOUNT
  if (MDT_USECOUNT) {
    int opcode = 0;
    for (; opcode < Opcode__; opcode++) {
      fprintf(MDT_USECOUNT, "\tOpcode_%s\t%u\n", Opcode_Id(opcode), Opcode_UseCount[opcode]);
    }
  }
#endif//MDT_USECOUNT
}

#if XCC__C
static void
Opcode_TEST(void)
{
#include "MDT/Opcode_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Opcode_TEST);
  return 0;
}
#endif

