#ifdef $XCC_h
/*
 * !!!!	MDT.xcc
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

#ifdef $XCC_h
/**
 * @defgroup MDT Machine Description T...(echnology)?
 * @ingroup TSL
 * @brief Make available the results of the MDS (Machine Description System) as
 * static tables and wrapper functions.
 */
#endif//$XCC_h


#include "MDT/MDT_.h"

#ifdef $XCC__h
#include "MDT/MDT.h"
#endif//$XCC__h

#ifdef $XCC_h
#include "CCL.h"
#include "TSL.h"
#endif//$XCC_h

#ifdef $XCC_h
#ifdef _NTRACE
#ifndef NDECODE
#define NDECODE
#endif//NDECODE
#endif//_NTRACE
#endif//$XCC_h

#ifdef $XCC_h
extern const char *MDT_Family;
bool
MDT_isFamily(const char *family);
#endif//$XCC_h

const char *MDT_Family = MDT_TARGET;

bool
MDT_isFamily(const char *family)
{
  return (family != NULL) && !strcmp(family, MDT_TARGET);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Platform;
extern const int Platform__Count;
#define Platform__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Platform__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Resource;
extern const int Resource__Count;
#define Resource__COUNT (2*sizeof(uint64_t)/sizeof(uint8_t)+1)
#define Resource__UNDEF 0
#define Resource__ISSUE 1
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Resource__Count <= UINT8_MAX);
  Except_ALWAYS(Resource__Count <= Resource__COUNT);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Processor;
extern const int Processor__Count;
#define Processor__UNDEF 0
#define Processor__DEFAULT 1
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Processor__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Decoding;
extern const int Decoding__Count;
#define Decoding__UNDEF 0
#define Decoding_MAXSIZE 16
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Decoding__Count <= UINT8_MAX);
  Except_ALWAYS(Decoding_SIZE_MAX <= Decoding_MAXSIZE);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t NativeType;
extern const int NativeType__Count;
#define NativeType__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(NativeType__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint16_t StorageCell;
extern const int StorageCell__Count;
extern const int StorageCell__Volatile;
extern const int StorageCell__Control;
extern const int StorageCell__Memory;
#define StorageCell__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(StorageCell__Count <= UINT16_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint16_t Register;
extern const int Register__Count;
extern const int Register__MaxStorageCells;
#define Register__COUNT 8192
#define Register__UNDEF 0
#define Register__Volatile (Register__Count + 0)
#define Register__Control (Register__Count + 1)
#define Register__Memory (Register__Count + 2)
#define Register__XCOUNT (Register__Count + 3)
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Register__Count <= UINT16_MAX);
  Except_ALWAYS(Register__Count <= Register__COUNT);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t RegFile;
extern const int RegFile__Count;
#define RegFile__COUNT 16
#define RegFile__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(RegFile__Count <= UINT8_MAX);
  Except_ALWAYS(RegFile__Count <= RegFile__COUNT);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t RegClass;
extern const int RegClass__Count;
#define RegClass__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(RegClass__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t RegMask;
extern const int RegMask__Count;
#define RegMask__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(RegMask__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Modifier;
extern const int Modifier__Count;
#define Modifier__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Modifier__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t ModifierMember;
extern const int ModifierMember__Count;
#define ModifierMember__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(ModifierMember__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Relocation;
extern const int Relocation__Count;
#define Relocation__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Relocation__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Immediate;
extern const int Immediate__Count;
#define Immediate__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Immediate__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Operand;
extern const int Operand__Count;
#define Operand__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Operand__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Operands;
extern const int Operands__Count;
#define Operands__UNDEF 0
#define Operands__MAXOPERANDS 12
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Operands__Count <= UINT8_MAX);
  Except_ALWAYS(Operands_MAXCOUNT <= Operands__MAXOPERANDS);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Format;
extern const int Format__Count;
#define Format__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Format__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint16_t Opcode;
extern const int Opcode__Count;
extern const int Opcode__MaxActions;
#define Opcode__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Opcode__Count <= UINT16_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint16_t Operator;
extern const int Operator__Count;
extern const int Operator__MaxParameters;
#define Operator__UNDEF  0	//FIXME!
#define Operator_ENTRY   1	//FIXME!
#define Operator_LABEL   2	//FIXME!
#define Operator_FALLTO  3	//FIXME!
#define Operator_LOOPTO  4	//FIXME!
#define Operator_HWLOOP  5	//FIXME!
#define Operator_HWLNEZ  6	//FIXME!
#define Operator_HWLGTZ  7	//FIXME!
#define Operator_GOTO    8	//FIXME!
#define Operator_IFTRUE  9	//FIXME!
#define Operator_IFFALSE 10	//FIXME!
#define Operator_IFNEZ   11	//FIXME!
#define Operator_IFEQZ   12	//FIXME!
#define Operator_IFLTZ   13	//FIXME!
#define Operator_IFGEZ   14	//FIXME!
#define Operator_IFLEZ   15	//FIXME!
#define Operator_IFGTZ   16	//FIXME!
#define Operator_IFODD   17	//FIXME!
#define Operator_IFEVEN  18	//FIXME!
#define Operator_JUMP    19	//FIXME!
#define Operator_CALL    20	//FIXME!
#define Operator_LINK    21	//FIXME!
#define Operator_RETURN  22	//FIXME!
#define Operator_PHI     23	//FIXME!
#define Operator_PSI     24	//FIXME!
#define Operator_SIGMA   25	//FIXME!
#define Operator_KILL    26	//FIXME!
#define Operator_PCOPY   27	//FIXME!
#define Operator_ECOPY   28	//FIXME!
#define Operator_LCOPY   29	//FIXME!
#define Operator_COPY    30	//FIXME!
#define Operator_SKIP    31	//FIXME!
#define Operator_LNOT    32	//FIXME!
#define Operator_BNOT    33	//FIXME!
#define Operator_NEG     34	//FIXME!
#define Operator_ADD     35	//FIXME!
#define Operator_SUB     36	//FIXME!
#define Operator_MUL     37	//FIXME!
#define Operator_DIV     38	//FIXME!
#define Operator_REM     39	//FIXME!
#define Operator_SHL     40	//FIXME!
#define Operator_ASHR    41	//FIXME!
#define Operator_LSHR    42	//FIXME!
#define Operator_LAND    43	//FIXME!
#define Operator_LIOR    44	//FIXME!
#define Operator_LXOR    45	//FIXME!
#define Operator_BAND    46	//FIXME!
#define Operator_BIOR    47	//FIXME!
#define Operator_BXOR    48	//FIXME!
#define Operator_MIN     49	//FIXME!
#define Operator_MAX     50	//FIXME!
#define Operator_SELECT  51	//FIXME!
#define Operator_APPLY   52	//FIXME!
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Operator__Count <= UINT16_MAX);
  Except_ALWAYS(Operator_ENTRY == Operator__ENTRY);
  Except_ALWAYS(Operator_LABEL == Operator__LABEL);
  Except_ALWAYS(Operator_FALLTO == Operator__FALLTO);
  Except_ALWAYS(Operator_LOOPTO == Operator__LOOPTO);
  Except_ALWAYS(Operator_HWLOOP == Operator__HWLOOP);
  Except_ALWAYS(Operator_HWLNEZ == Operator__HWLNEZ);
  Except_ALWAYS(Operator_HWLGTZ == Operator__HWLGTZ);
  Except_ALWAYS(Operator_IFTRUE == Operator__IFTRUE);
  Except_ALWAYS(Operator_IFFALSE == Operator__IFFALSE);
  Except_ALWAYS(Operator_IFNEZ == Operator__IFNEZ);
  Except_ALWAYS(Operator_IFEQZ == Operator__IFEQZ);
  Except_ALWAYS(Operator_IFLTZ == Operator__IFLTZ);
  Except_ALWAYS(Operator_IFGEZ == Operator__IFGEZ);
  Except_ALWAYS(Operator_IFLEZ == Operator__IFLEZ);
  Except_ALWAYS(Operator_IFGTZ == Operator__IFGTZ);
  Except_ALWAYS(Operator_IFODD == Operator__IFODD);
  Except_ALWAYS(Operator_IFEVEN == Operator__IFEVEN);
  Except_ALWAYS(Operator_GOTO == Operator__GOTO);
  Except_ALWAYS(Operator_JUMP == Operator__JUMP);
  Except_ALWAYS(Operator_CALL == Operator__CALL);
  Except_ALWAYS(Operator_LINK == Operator__LINK);
  Except_ALWAYS(Operator_RETURN == Operator__RETURN);
  Except_ALWAYS(Operator_PHI == Operator__PHI);
  Except_ALWAYS(Operator_PSI == Operator__PSI);
  Except_ALWAYS(Operator_SIGMA == Operator__SIGMA);
  Except_ALWAYS(Operator_KILL == Operator__KILL);
  Except_ALWAYS(Operator_PCOPY == Operator__PCOPY);
  Except_ALWAYS(Operator_ECOPY == Operator__ECOPY);
  Except_ALWAYS(Operator_LCOPY == Operator__LCOPY);
  Except_ALWAYS(Operator_COPY == Operator__COPY);
  Except_ALWAYS(Operator_SKIP == Operator__SKIP);
  Except_ALWAYS(Operator_LNOT == Operator__LNOT);
  Except_ALWAYS(Operator_BNOT == Operator__BNOT);
  Except_ALWAYS(Operator_NEG == Operator__NEG);
  Except_ALWAYS(Operator_ADD == Operator__ADD);
  Except_ALWAYS(Operator_SUB == Operator__SUB);
  Except_ALWAYS(Operator_MUL == Operator__MUL);
  Except_ALWAYS(Operator_DIV == Operator__DIV);
  Except_ALWAYS(Operator_REM == Operator__REM);
  Except_ALWAYS(Operator_SHL == Operator__SHL);
  Except_ALWAYS(Operator_ASHR == Operator__ASHR);
  Except_ALWAYS(Operator_LSHR == Operator__LSHR);
  Except_ALWAYS(Operator_LAND == Operator__LAND);
  Except_ALWAYS(Operator_LIOR == Operator__LIOR);
  Except_ALWAYS(Operator_LXOR == Operator__LXOR);
  Except_ALWAYS(Operator_BAND == Operator__BAND);
  Except_ALWAYS(Operator_BIOR == Operator__BIOR);
  Except_ALWAYS(Operator_BXOR == Operator__BXOR);
  Except_ALWAYS(Operator_MIN == Operator__MIN);
  Except_ALWAYS(Operator_MAX == Operator__MAX);
  Except_ALWAYS(Operator_SELECT == Operator__SELECT);
  Except_ALWAYS(Operator_APPLY == Operator__APPLY);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Bundling;
extern const int Bundling__Count;
#define Bundling__UNDEF 0
#define Bundling__BIAS	1
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Bundling__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint16_t Template;
extern const int Template__Count;
#define Template__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Template__Count <= UINT16_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint16_t Bundle;
extern const int Bundle__Count;
extern const int Bundle__MaxInstructions;
#define Bundle__MAXINSTRUCTIONS 8
#define Bundle__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Bundle__Count <= UINT16_MAX);
  Except_ALWAYS(Bundle_INSTRUCTIONS_MAX <= Bundle__MAXINSTRUCTIONS);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Reservation;
extern const int Reservation__Count;
extern const int Reservation__MaxColumnsCount;
#define Reservation__COUNT 64
#define Reservation__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Reservation__Count <= UINT8_MAX);
  Except_ALWAYS(Reservation__Count <= Reservation__COUNT);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Scheduling;
extern const int Scheduling__Count;
#define Scheduling__UNDEF 0
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Scheduling__Count <= UINT8_MAX);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Enumeration.
 */
typedef uint8_t Convention;
extern const int Convention__Count;
#define Convention__UNDEF 0
#define Convention__DEFAULT 1
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Convention__Count <= UINT8_MAX);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include "MDT/Platform.h"
#include "MDT/Resource.h"
#include "MDT/Processor.h"
#include "MDT/Decoding.h"
#include "MDT/NativeType.h"
#include "MDT/Storage.h"
#include "MDT/Register.h"
#include "MDT/RegFile.h"
#include "MDT/RegClass.h"
#include "MDT/RegMask.h"
#include "MDT/Modifier.h"
#include "MDT/Relocation.h"
#include "MDT/Immediate.h"
#include "MDT/Operand.h"
#include "MDT/Format.h"
#include "MDT/Behavior.h"
#include "MDT/Opcode.h"
#include "MDT/Operator.h"
#include "MDT/Bundling.h"
#include "MDT/Template.h"
#include "MDT/Bundle.h"
#include "MDT/Reservation.h"
#include "MDT/Scheduling.h"
#include "MDT/Convention.h"
#include "MDT/Decode.h"
#endif//$XCC_h

#ifdef $XCC__h
#include "MDT/Platform_.h"
#include "MDT/Resource_.h"
#include "MDT/Processor_.h"
#include "MDT/Decoding_.h"
#include "MDT/NativeType_.h"
#include "MDT/Storage_.h"
#include "MDT/Register_.h"
#include "MDT/RegFile_.h"
#include "MDT/RegClass_.h"
#include "MDT/RegMask_.h"
#include "MDT/Modifier_.h"
#include "MDT/Relocation_.h"
#include "MDT/Immediate_.h"
#include "MDT/Operand_.h"
#include "MDT/Format_.h"
#include "MDT/Behavior_.h"
#include "MDT/Opcode_.h"
#include "MDT/Operator_.h"
#include "MDT/Bundling_.h"
#include "MDT/Template_.h"
#include "MDT/Bundle_.h"
#include "MDT/Reservation_.h"
#include "MDT/Scheduling_.h"
#include "MDT/Convention_.h"
#include "MDT/Decode_.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * File handle used to trace the MDT.
 */
extern FILE *MDT_TRACE;
#ifdef _NTRACE
#define MDT_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef MDT_TRACE
FILE *MDT_TRACE = NULL;

#ifdef $XCC_h
/**
 * File handle used to enable the MDT use counts.
 */
extern FILE *MDT_USECOUNT;
#ifdef _NTRACE
#define MDT_USECOUNT NULL
#endif//_NTRACE
#endif//$XCC_h

#undef MDT_USECOUNT
FILE *MDT_USECOUNT = NULL;

#ifdef $XCC_h
/*
 * Variables used to control MDT in non-release mode.
 */
extern int FORCE_MAXISSUE;
extern int FORCE_MINTAKEN;
#endif//$XCC_h

int FORCE_MAXISSUE;
int FORCE_MINTAKEN;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
void
MDT_setMaxIssue(Processor processor, int maxIssue);
#endif//$XCC_h

void
MDT_setMaxIssue(Processor processor, int maxIssue)
{
#if Bundle_INSTRUCTIONS_MAX
  Processor_setMaxIssue(processor, maxIssue);
#endif//Bundle_INSTRUCTIONS_MAX
  Reservation_InitializeODot();
}

#ifdef $XCC_h
void
MDT_setMinTaken(Processor processor, int minTaken);
#endif//$XCC_h

void
MDT_setMinTaken(Processor processor, int minTaken)
{
  Except_ALWAYS(minTaken >= 0);
  Processor_setMinTaken(processor, minTaken);
}

#ifdef $XCC_h
void
MDT_setReserved(Convention convention, Register registre);
#endif//$XCC_h

void
MDT_setReserved(Convention convention, Register registre)
{
  Convention_setRole(convention, ConventionRole_Reserved, registre);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * MDT init function.
 */
void
MDT_INIT(void);
#endif//$XCC_h

static int MDT_Initialized = 0;

void
MDT_INIT(void)
{
  if (MDT_Initialized++ == 0) {
    if (GETENV("MDT_TRACE")) {
      int file = 0;
      sscanf(GETENV("MDT_TRACE"), "%d", &file);
      if (file == 1) MDT_TRACE = stdout;
      else if (file == 2) MDT_TRACE = stderr;
    }
    if (GETENV("MDT_USECOUNT")) {
      int file = 0;
      sscanf(GETENV("MDT_USECOUNT"), "%d", &file);
      if (file == 1) MDT_USECOUNT = stdout;
      else if (file == 2) MDT_USECOUNT = stderr;
    }
    FORCE_MAXISSUE = -1;
    if (GETENV("FORCE_MAXISSUE")) {
      sscanf(GETENV("FORCE_MAXISSUE"), "%d", &FORCE_MAXISSUE);
      fprintf(stderr, "*** FORCE_MAXISSUE=%d\n", FORCE_MAXISSUE);
    }
    FORCE_MINTAKEN = 1;
    if (GETENV("FORCE_MINTAKEN")) {
      sscanf(GETENV("FORCE_MINTAKEN"), "%d", &FORCE_MINTAKEN);
      fprintf(stderr, "*** FORCE_MINTAKEN=%d\n", FORCE_MINTAKEN);
    }
    Platform_INIT();
    Processor_INIT();
    Decoding_INIT();
    NativeType_INIT();
    Storage_INIT();
    Register_INIT();
    RegFile_INIT();
    RegClass_INIT();
    RegMask_INIT();
    Modifier_INIT();
    Relocation_INIT();
    Immediate_INIT();
    Operand_INIT();
    Format_INIT();
    Behavior_INIT();
    Opcode_INIT();
    Operator_INIT();
    Bundling_INIT();
    Template_INIT();
    Bundle_INIT();
    Resource_INIT();
    Reservation_INIT();
    Scheduling_INIT();
    Convention_INIT();
    Decode_INIT();
    if (FORCE_MAXISSUE > 0) {
      int processor = 0;
      for (; processor < Processor__; processor++) {
        MDT_setMaxIssue(processor, FORCE_MAXISSUE);
      }
    }
    if (FORCE_MINTAKEN >= 0) {
      int processor = 0;
      for (; processor < Processor__; processor++) {
        MDT_setMinTaken(processor, FORCE_MINTAKEN);
      }
    }
  }
}

#ifdef $XCC_h
/**
 * MDT fini function.
 */
void
MDT_FINI(void);
#endif//$XCC_h

void
MDT_FINI(void)
{
  if (--MDT_Initialized == 0) {
    Decode_FINI();
    Convention_FINI();
    Scheduling_FINI();
    Reservation_FINI();
    Resource_FINI();
    Bundle_FINI();
    Template_FINI();
    Bundling_FINI();
    Operator_FINI();
    Opcode_FINI();
    Behavior_FINI();
    Format_FINI();
    Operand_FINI();
    Immediate_FINI();
    Relocation_FINI();
    Modifier_FINI();
    RegMask_FINI();
    RegClass_FINI();
    RegFile_FINI();
    Register_FINI();
    Storage_FINI();
    NativeType_FINI();
    Decoding_FINI();
    Processor_FINI();
    Platform_FINI();
  }
}

#ifdef $XCC__h
/**
 * Setup and execute a MDT test.
 */
void
MDT_Test(void (*test)(void));
#endif//$XCC__h

void
MDT_Test(void (*test)(void))
{
  MDT_INIT();
  (*test)();
  MDT_FINI();
}

#if XCC__C
static void
MDT_TEST(void)
{
#include "MDT/MDT_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(MDT_TEST);
  return 0;
}
#endif

