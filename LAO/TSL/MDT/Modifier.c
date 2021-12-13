#ifdef $XCC_h
/*
 * !!!!	Modifier.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Stefan Freudenberger (Stefan.Freudenberger@st.com)
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
#include "MDT/Modifier.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Modifier(ID, MEMBERS, ENCODE, DECODE) \
  Modifier_##ID,
  Modifier_,
#include "Modifier.tuple"
  Modifier__
#undef Modifier
} enum_Modifier;
#endif//$XCC__h

const int Modifier__Count = Modifier__;

#ifdef $XCC__h
typedef enum {
#define MEMBER(ID, NAME, LCNAME, VALUE) \
  ModifierMember_##ID,
#define MEMBERS(count, items) items
#define Modifier(ID, MEMBERS, ENCODE, DECODE) \
  MEMBERS
  ModifierMember_,
#include "Modifier.tuple"
  ModifierMember__
#undef Modifier
#undef MEMBERS
#undef MEMBER
} enum_ModifierMember;
#endif//$XCC__h

const int ModifierMember__Count = ModifierMember__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Modifier enumeration.
 */
const char *
Modifier_Id(Modifier this);
const char *
Modifier_name(Modifier this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Modifier_Id(Modifier this)
{
  static const char *_[] = {
#define Modifier(ID, MEMBERS, ENCODE, DECODE) \
    #ID,
    "_UNDEF",
#include "Modifier.tuple"
  };
  Except_REQUIRE((unsigned)this < Modifier__);
  return _[this];
}
const char *
Modifier_name(Modifier this)
{
  return this? strchr(Modifier_Id(this) + sizeof(MDT_TARGET) - 1, '_') + 1: "_UNDEF";
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int modifier = 0;
  for (; modifier < Modifier__; modifier++) {
    printf("Modifier_%s\t%d\n", Modifier_Id(modifier), modifier);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * Modifier_Encode functions.
 */
#define ENCODE(e) e
#define Modifier(ID, VALUES, ENCODE, DECODE) \
static inline OperandEncoded \
Modifier_Encode_##ID(OperandDecoded decoded) \
{ \
  OperandEncoded VALUE = decoded; \
  ENCODE; \
  return VALUE; \
}
#include "Modifier.tuple"
#undef ENCODE
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * Modifier_Decode functions.
 */
#define DECODE(e) e
#define Modifier(ID, VALUES, ENCODE, DECODE) \
static inline OperandDecoded \
Modifier_Decode_##ID(OperandEncoded encoded) \
{ \
  OperandDecoded VALUE = encoded; \
  DECODE; \
  return VALUE; \
}
#include "Modifier.tuple"
#undef DECODE
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the ModifierMember enumeration.
 */
const char *
ModifierMember_Id(ModifierMember this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ModifierMember_Id(ModifierMember this)
{
  static const char *_[] = {
#define MEMBER(ID, NAME, LCNAME, VALUE) #ID,
#define MEMBERS(count, items) items
#define Modifier(ID, MEMBERS, ENCODE, DECODE) \
    MEMBERS
    "_UNDEF",
#include "Modifier.tuple"
#undef Modifier
#undef MEMBERS
#undef MEMBER
  };
  Except_REQUIRE((unsigned)this < ModifierMember__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int member = 0;
  for (; member < ModifierMember__; member++) {
    printf("ModifierMember_%s\t%d\n", ModifierMember_Id(member), member);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the ModifierMember enumeration.
 */
const char *
ModifierMember_name(ModifierMember this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ModifierMember_name(ModifierMember this)
{
  static const char *_[] = {
#define NAME(n) #n
#define MEMBER(ID, NAME, LCNAME, VALUE) NAME,
#define MEMBERS(count, items) items
#define Modifier(ID, MEMBERS, ENCODE, DECODE) \
    MEMBERS
    "_UNDEF",
#include "Modifier.tuple"
#undef Modifier
#undef MEMBERS
#undef MEMBER
#undef NAME
  };
  Except_REQUIRE((unsigned)this < ModifierMember__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int member = 0;
  for (; member < ModifierMember__; member++) {
    printf("Name(%s)\t%s\n", ModifierMember_Id(member), ModifierMember_name(member));
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Lower case names for the ModifierMember enumeration.
 */
const char *
ModifierMember_lcname(ModifierMember this);
#endif//$XCC_h

#ifndef _NTRACE
static const char *
ModifierMember_Lcname[] = {
#define LCNAME(n) #n
#define MEMBER(ID, NAME, LCNAME, VALUE) LCNAME,
#define MEMBERS(count, items) items
#define Modifier(ID, MEMBERS, ENCODE, DECODE) \
    MEMBERS
    "_UNDEF",
#include "Modifier.tuple"
#undef Modifier
#undef MEMBERS
#undef MEMBER
#undef NAME
};
#endif//_NTRACE

#ifndef _NTRACE
/**
 * Get the lower-case name of a ::ModifierMember
 */
const char *
ModifierMember_lcname(ModifierMember this)
{
  Except_REQUIRE((unsigned)this < ModifierMember__);
  return ModifierMember_Lcname[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int member = 0;
  for (; member < ModifierMember__; member++) {
    printf("LCName(%s)\t%s\n", ModifierMember_Id(member), ModifierMember_lcname(member));
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Find a modifier given its lower case name.
 */
ModifierMember
ModifierMember_ofLcname(const char *string);
#endif//$XCC_h

#ifndef _NTRACE
ModifierMember
ModifierMember_ofLcname(const char *string)
{
#define LCNAME(n) #n
#define MEMBER(ID, NAME, LCNAME, VALUE) \
  else if(!strcmp(LCNAME, string)) { \
    return ModifierMember_ ## ID; \
  }
#define MEMBERS(count, items) items
#define Modifier(ID, MEMBERS, ENCODE, DECODE) \
  MEMBERS
  if(!strcmp("_UNDEF",string)) {
    return ModifierMember__UNDEF;
  }
#include "Modifier.tuple"
#undef Modifier
#undef MEMBERS
#undef MEMBER
#undef NAME
  return ModifierMember__UNDEF;
}
#endif//_NTRACE

#ifdef $XCC__c
{
  ModifierMember mod;
#define TESTMODMEM(MODMEM, ID) \
  { \
    mod = ModifierMember_ofLcname(MODMEM); \
    Except_CHECK(!strcmp(MODMEM, ModifierMember_Lcname[mod])); \
    printf("ModifierMember of string '%s' is ModifierMember_%s\n", MODMEM, #ID); \
  }
#define LCNAME(n) #n
#define MEMBER(ID, NAME, LCNAME, VALUE) \
  TESTMODMEM(LCNAME, ID)
#define MEMBERS(count, items) items
#define Modifier(ID, MEMBERS, ENCODE, DECODE) \
  MEMBERS
  TESTMODMEM("_UNDEF", _UNDEF)
#include "Modifier.tuple"
#undef TESTMODMEM
#undef MEMBERS
#undef MEMBER
#undef NAME
}
#endif//$XCC__c


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Values for the ModifierMember enumeration.
 */
int64_t
ModifierMember_value(ModifierMember this);
#endif//$XCC_h

extern const int64_t
ModifierMember_Value[];
int64_t
ModifierMember_value(ModifierMember this)
{
  Except_REQUIRE((unsigned)this < ModifierMember__);
  return ModifierMember_Value[this];
}

const int64_t
ModifierMember_Value[] = {
#define VALUE(v) v##LL
#define MEMBER(ID, NAME, LCNAME, VALUE) VALUE,
#define MEMBERS(count, items) items
#define Modifier(ID, MEMBERS, ENCODE, DECODE) \
    MEMBERS
    VALUE(0),
#include "Modifier.tuple"
#undef Modifier
#undef MEMBERS
#undef MEMBER
#undef VALUE
};

#ifdef $XCC__c
{
  int member = 0;
  for (; member < ModifierMember__; member++) {
    printf("Value(%s)\t%llu\n", ModifierMember_Id(member), (long long)ModifierMember_value(member));
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Complement the condition of this ModifierMember.
 */
ModifierMember
ModifierMember_complement(ModifierMember this);
#endif//$XCC_h

#ifdef $XCC__c
{
  int member = 0;
  for (; member < ModifierMember__; ++member) {
    int complement = ModifierMember_complement(member);
    if (complement != ModifierMember_) {
      printf("Complement(%s) = %s\n", ModifierMember_Id(member), ModifierMember_Id(complement));
    }
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Modifier module.
 */
#define Modifier_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Modifier module.
 */
#define Modifier_FINI()
#endif//$XCC__h

#if XCC__C
static void
Modifier_TEST(void)
{
#include "MDT/Modifier_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Modifier_TEST);
  return 0;
}
#endif

