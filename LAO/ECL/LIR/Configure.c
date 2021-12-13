#ifdef $XCC_h
/*
 * !!!!	Configure.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu).
 * Duco van Amstel (duco.van-amstel@kalray.eu)
 *
 * Copyright 2010 - 2013 Kalray
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
 * @file
 * @ingroup LIR
 * @brief Configuration flags.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/Configure.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration of the code compensation levels.
 *
 * Code compensation results from code motion down a branch.
 */
typedef enum {
  ConfigureCompensation_None,		//!< No code compensation.
  ConfigureCompensation_Local,		//!< Local code compensation.
  ConfigureCompensation_Global,		//!< Global code compensation.
  ConfigureCompensation__
} enum_ConfigureCompensation;
typedef uint8_t ConfigureCompensation;
extern const char *
ConfigureCompensation_Id(ConfigureCompensation this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ConfigureCompensation_Id(ConfigureCompensation this)
{
  static const char *_[] = {
    "None",
    "Local",
    "Global",
  };
  Except_REQUIRE((unsigned)this < ConfigureCompensation__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration of the control speculation levels.
 *
 * Speculation is the execution of code under a more general condition than original.
 */
typedef enum {
  ConfigureSpeculation_None,		//!< No control speculation.
  ConfigureSpeculation_SafeArith,	//!< Speculation of safe arithmetic operations.
  ConfigureSpeculation_MemorySafe,	//!< Speculation of safe memory accesses.
  ConfigureSpeculation_SafeVariant,	//!< Speculation of operations with safe variant.
  ConfigureSpeculation_MayExcept,	//!< Speculation of may be excepting operations.
  ConfigureSpeculation__
} enum_ConfigureSpeculation;
typedef uint8_t ConfigureSpeculation;
extern const char *
ConfigureSpeculation_Id(ConfigureSpeculation this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ConfigureSpeculation_Id(ConfigureSpeculation this)
{
  static const char *_[] = {
    "None",
    "SafeArith",
    "MemorySafe",
    "SafeVariant",
    "MayExcept",
  };
  Except_REQUIRE((unsigned)this < ConfigureSpeculation__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration of the inductive relaxation levels.
 *
 * Inductive relaxation enables to relax dependences on simple induction variables.
 */
typedef enum {
  ConfigureRelaxation_None,		//!< No inductive relaxation.
  ConfigureRelaxation_Local,		//!< Local inductive relaxation.
  ConfigureRelaxation_Global,		//!< Global inductive relaxation.
  ConfigureRelaxation__
} enum_ConfigureRelaxation;
typedef uint8_t ConfigureRelaxation;
extern const char *
ConfigureRelaxation_Id(ConfigureRelaxation this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ConfigureRelaxation_Id(ConfigureRelaxation this)
{
  static const char *_[] = {
    "None",
    "Local",
    "Global",
  };
  Except_REQUIRE((unsigned)this < ConfigureRelaxation__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration of the register renaming levels.
 */
typedef enum {
  ConfigureRenaming_None,		//!< No register renaming.
  ConfigureRenaming_Normal,		//!< Normal register renaming.
  ConfigureRenaming_Local,		//!< Local register modulo renaming.
  ConfigureRenaming_Global,		//!< Global register modulo renaming.
  ConfigureRenaming_Shuffle,		//!< Suffle global registers.
  ConfigureRenaming__
} enum_ConfigureRenaming;
typedef uint8_t ConfigureRenaming;
extern const char *
ConfigureRenaming_Id(ConfigureRenaming this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ConfigureRenaming_Id(ConfigureRenaming this)
{
  static const char *_[] = {
    "None",
    "Normal",
    "Local",
    "Global",
    "Shuffle",
  };
  Except_REQUIRE((unsigned)this < ConfigureRenaming__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration of the operation boosting levels.
 *
 * Boosting is the enabling of control speculation by code predication.
 */
typedef enum {
  ConfigureBoosting_None,		//!< No operation boosting.
  ConfigureBoosting_Reuse,		//!< Reuse branch predicates.
  ConfigureBoosting_Combine,		//!< Combine branch predicates.
  ConfigureBoosting__
} enum_ConfigureBoosting;
typedef uint8_t ConfigureBoosting;
extern const char *
ConfigureBoosting_Id(ConfigureBoosting this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ConfigureBoosting_Id(ConfigureBoosting this)
{
  static const char *_[] = {
    "None",
    "Reuse",
    "Combine",
  };
  Except_REQUIRE((unsigned)this < ConfigureBoosting__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Memory aliasing level.
 *
 * Enumerate how memory aliasing inside a loop can be overridden.
 */
typedef enum {
  ConfigureAliasing_Serial,		//!< Serialize all memory accesses.
  ConfigureAliasing_Normal,		//!< Normal aliasing of memory accesses.
  ConfigureAliasing_Vector,		//!< Assume vector loop memory aliasing.
  ConfigureAliasing_Parallel,		//!< Assume parallel loop memory aliasing.
  ConfigureAliasing_Liberal,		//!< Assume no alias of variant memory accesses.
  ConfigureAliasing__
} enum_ConfigureAliasing;
typedef uint8_t ConfigureAliasing;
extern const char *
ConfigureAliasing_Id(ConfigureAliasing this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ConfigureAliasing_Id(ConfigureAliasing this)
{
  static const char *_[] = {
    "Serial",
    "Normal",
    "Vector",
    "Parallel",
    "Liberal",
  };
  Except_REQUIRE((unsigned)this < ConfigureAliasing__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Memory pre-loading level.
 */
typedef enum {
  ConfigurePreLoading_None,		//!< Disable pre-loading.
  ConfigurePreLoading_Simple,		//!< Pre-loading of simple induction LOADs.
  ConfigurePreLoading_Variant,		//!< Pre-loading of variant address LOADs.
  ConfigurePreLoading_Indirect,		//!< Pre-loading of indirect LOADs.
  ConfigurePreLoading_NonSpill,		//!< Pre-loading of non-spill LOADs.
  ConfigurePreLoading__
} enum_ConfigurePreLoading;
typedef uint8_t ConfigurePreLoading;
extern const char *
ConfigurePreLoading_Id(ConfigurePreLoading this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ConfigurePreLoading_Id(ConfigurePreLoading this)
{
  static const char *_[] = {
    "None",
    "Simple",
    "Variant",
    "Indirect",
    "NonSpill",
  };
  Except_REQUIRE((unsigned)this < ConfigurePreLoading__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerate the Configure items.
 *
 * Each ConfigureItem can have a negative value, meaning that the value is undefined.
 */
typedef enum {
  ConfigureItem_Compensation,		//!< See ConfigureCompensation.
  ConfigureItem_Speculation,		//!< See ConfigureSpeculation.
  ConfigureItem_Relaxation,		//!< See ConfigureRelaxation.
  ConfigureItem_Renaming,		//!< See ConfigureRenaming.
  ConfigureItem_Boosting,		//!< See ConfigureBoosting.
  ConfigureItem_Aliasing,		//!< See ConfigureAliasing.
  ConfigureItem_TripModulus,		//!< Modulus of loop trip count.
  ConfigureItem_TripResidue,		//!< Residue of loop trip count.
  ConfigureItem_Pipelining,		//!< Software pipelining level.
    //! 0 => cyclic instruction schedule,
    //! 1 => software pipelining with overlap 1,
    //! n => software pipelining with overlap (1<<n)-1.
  ConfigureItem_PreLoading,		//!< See ConfigurePreLoading.
  ConfigureItem_L1MissExtra,		//!< Extra latency of L1 miss.
  ConfigureItem__
} enum_ConfigureItem;
typedef uint8_t ConfigureItem;
extern const char *
ConfigureItem_Id(ConfigureItem this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ConfigureItem_Id(ConfigureItem this)
{
  static const char *_[] = {
    "Compensation",
    "Speculation",
    "Relaxation",
    "Renaming",
    "Boosting",
    "Aliasing",
    "TripModulus",
    "TripResidue",
    "Pipelining",
    "PreLoading",
    "L1MissExtra",
  };
  Except_REQUIRE((unsigned)this < ConfigureItem__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Array of ConfigureItem values.
 */
struct Configure_ {
  //@args
  int8_t ITEMS[ConfigureItem__];
  //@access COMPENSATION	(*Configure__COMPENSATION(this) + 0)
  //@mutate COMPENSATION	(Configure_ITEMS(this) + ConfigureItem_Compensation)
  //@access SPECULATION	(*Configure__SPECULATION(this) + 0)
  //@mutate SPECULATION	(Configure_ITEMS(this) + ConfigureItem_Speculation)
  //@access RELAXATION	(*Configure__RELAXATION(this) + 0)
  //@mutate RELAXATION	(Configure_ITEMS(this) + ConfigureItem_Relaxation)
  //@access RENAMING	(*Configure__RENAMING(this) + 0)
  //@mutate RENAMING	(Configure_ITEMS(this) + ConfigureItem_Renaming)
  //@access BOOSTING	(*Configure__BOOSTING(this) + 0)
  //@mutate BOOSTING	(Configure_ITEMS(this) + ConfigureItem_Boosting)
  //@access ALIASING	(*Configure__ALIASING(this) + 0)
  //@mutate ALIASING	(Configure_ITEMS(this) + ConfigureItem_Aliasing)
  //@access TRIPMODULUS	(*Configure__TRIPMODULUS(this) + 0)
  //@mutate TRIPMODULUS	(Configure_ITEMS(this) + ConfigureItem_TripModulus)
  //@access TRIPRESIDUE	(*Configure__TRIPRESIDUE(this) + 0)
  //@mutate TRIPRESIDUE	(Configure_ITEMS(this) + ConfigureItem_TripResidue)
  //@access PIPELINING	(*Configure__PIPELINING(this) + 0)
  //@mutate PIPELINING	(Configure_ITEMS(this) + ConfigureItem_Pipelining)
  //@access PRELOADING	(*Configure__PRELOADING(this) + 0)
  //@mutate PRELOADING	(Configure_ITEMS(this) + ConfigureItem_PreLoading)
  //@access L1MISSEXTRA	(*Configure__L1MISSEXTRA(this) + 0)
  //@mutate L1MISSEXTRA	(Configure_ITEMS(this) + ConfigureItem_L1MissExtra)
  //@access OVERLAP	((1<<Configure_PIPELINING(this)) - 1)
};
#endif//$XCC_h

Configure
Configure_Ctor(Configure this)
{
  memset(this, -1, sizeof(Configure_));
  Configure_load(this,
                 ConfigureItem_Compensation, ConfigureCompensation_None,
                 ConfigureItem_Speculation, ConfigureSpeculation_MemorySafe,
                 ConfigureItem_Relaxation, ConfigureRelaxation_Local,
                 ConfigureItem_Renaming, ConfigureRenaming_Normal,
                 ConfigureItem_Boosting, ConfigureBoosting_None,
                 ConfigureItem_Aliasing, ConfigureAliasing_Normal,
                 ConfigureItem__);
  return this;
}

Configure
Configure_Copy(Configure this, const_Configure that)
{
  *(this) = *(that);
  return this;
}

void
Configure_Dtor(Configure this)
{
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Configure_)\t%zu\n", sizeof(Configure_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Print this Configure item values.
 */
bool
Configure_pretty(const_Configure this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Configure_pretty(const_Configure this, FILE *file)
{
  int i = 0;
  if (file == NULL) file = stderr;
  for (; i < ConfigureItem__; i++) {
    const char *name = ConfigureItem_Id(i);
    int value = Configure_ITEMS(this)[i];
    if (value >= 0) fprintf(file, "\t%s=%d", name, value);
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Force item values in this Configure.
 */
void
Configure_force(Configure this);
#endif//$XCC_h

void
Configure_force(Configure this)
{
  if (FORCE_COMPENSATION >= 0) *Configure__COMPENSATION(this) = FORCE_COMPENSATION;
  if (FORCE_SPECULATION >= 0) *Configure__SPECULATION(this) = FORCE_SPECULATION;
  if (FORCE_RELAXATION >= 0) *Configure__RELAXATION(this) = FORCE_RELAXATION;
  if (FORCE_PIPELINING >= 0) *Configure__PIPELINING(this) = FORCE_PIPELINING;
  if (FORCE_RENAMING >= 0) *Configure__RENAMING(this) = FORCE_RENAMING;
  if (FORCE_BOOSTING >= 0) *Configure__BOOSTING(this) = FORCE_BOOSTING;
  if (FORCE_ALIASING >= 0) *Configure__ALIASING(this) = FORCE_ALIASING;
  if (FORCE_PRELOADING >= 0) *Configure__PRELOADING(this) = FORCE_PRELOADING;
  if (FORCE_L1MISSEXTRA >= 0) *Configure__L1MISSEXTRA(this) = FORCE_L1MISSEXTRA;
}

#ifdef $XCC_h
/**
 * Load this Configure from a va_list.
 */
void
Configure_loadVA(Configure this, int item, va_list va);
#endif//$XCC_h

void
Configure_loadVA(Configure this, int item, va_list va)
{
  while (item != ConfigureItem__) {
    int value = (int)va_arg(va, int);
    Except_REQUIRE(item >= 0 && item < ConfigureItem__);
    if (value >= 0) {
      Configure_ITEMS(this)[item] = value;
    }
    item = (int)va_arg(va, int);
  }
  Configure_force(this);
}

#ifdef $XCC_h
/**
 * Load this Configure from variable arguments.
 */
void
Configure_load(Configure this, ConfigureItem item, ...);
#endif//$XCC_h

void
Configure_load(Configure this, ConfigureItem item, ...)
{
  va_list va;
  va_start(va, item);
  Configure_loadVA(this, item, va);
  va_end(va);
}

#ifdef $XCC_h
/**
 * Print this Configure item values.
 */
bool
Configure_print(Configure this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Configure_print(Configure this, FILE *file)
{
  int n = 0, i;
  for (i = 0; i < ConfigureItem__; i++) {
    const char *name = ConfigureItem_Id(i);
    int value = Configure_ITEMS(this)[i];
    if (value >= 0) fprintf(file, "ConfigureItem_%s, %d, ", name, value);
  }
  fprintf(file, "ConfigureItem__");
  return true;
}
#endif//_NTRACE

#ifdef $XCC__c
{
  Configure_ configure[1];
  Configure_Ctor(configure);
  printf("Configure(");
  Configure_print(configure, stdout);
  printf(")\n");
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Merge that Configure items into this Configure items.
 *
 * The item values of that Configure override the item values of this
 * Configure, unless for the values of that Configure that are negative.
 */
void
Configure_merge(Configure this, Configure that);
#endif//$XCC_h

void
Configure_merge(Configure this, Configure that)
{
  int i = 0;
  for (; i < sizeof(Configure)/sizeof(int8_t); i++) {
    if (Configure_ITEMS(that)[i] >= 0) {
      Configure_ITEMS(this)[i] = Configure_ITEMS(that)[i];
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize Configure.
 */
#define Configure_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize Configure.
 */
#define Configure_FINI()
#endif//$XCC__h

#if XCC__C
static void
Configure_TEST(void)
{
#include "LIR/Configure_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Configure_TEST);
  return 0;
}
#endif

