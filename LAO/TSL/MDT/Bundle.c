#ifdef $XCC_h
/*
 * !!!!	Bundle.xcc
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
#include "MDT/Bundle.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Bundle(ID, ALIGNMENT, CONTENTS, INVERSE, TEMPLATE) \
  Bundle_##ID,
  Bundle_,
#include /*MDT*/"Bundle.tuple"
  Bundle__
#undef Bundle
} enum_Bundle;
#endif//$XCC__h

const int Bundle__Count = Bundle__;
const int Bundle__MaxInstructions = Bundle_INSTRUCTIONS_MAX;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Bundle enumeration.
 */
extern const char *
Bundle_Id(Bundle this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Bundle_Id(Bundle this)
{
  static const char *_[] = {
#define Bundle(ID, ALIGNMENT, CONTENTS, INVERSE, TEMPLATE) \
    #ID,
    "_UNDEF",
#include "Bundle.tuple"
  };
  Except_REQUIRE((unsigned)this < Bundle__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int bundle = 0;
  for (bundle = 0; bundle < Bundle__; bundle++) {
    printf("Bundle_%s\t%d\n", Bundle_Id(bundle), bundle);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Bundle align bias.
 */
int
Bundle_alignBias(Bundle this);
#endif//$XCC_h

#if Bundle_INSTRUCTIONS_MAX
extern const uint8_t
Bundle_AlignBias[];
int
Bundle_alignBias(Bundle this)
{
  Except_REQUIRE((unsigned)this < Bundle__);
  return Bundle_AlignBias[this];
}
#endif//Bundle_INSTRUCTIONS_MAX

#if Bundle_INSTRUCTIONS_MAX
const uint8_t
Bundle_AlignBias[] = {
#define BIAS(d) d
#define ALIGNMENT(bias, base) bias
#define Bundle(ID, ALIGNMENT, CONTENTS, INVERSE, TEMPLATE) \
  ALIGNMENT,
  0,
#include "Bundle.tuple"
#undef ALIGNMENT
};
#endif//Bundle_INSTRUCTIONS_MAX

#ifdef $XCC__c
#if Bundle_INSTRUCTIONS_MAX
{
  int bundle = 0;
  for (bundle = 0; bundle < Bundle__; bundle++) {
    int alignBias = Bundle_alignBias(bundle);
    printf("\nAlignBias(%s)\t%d", Bundle_Id(bundle), alignBias);
  }
  printf("\n");
  fprintf(stderr, "sizeof(Bundle_AlignBias)\t%zu\n", sizeof(Bundle_AlignBias));
}
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Bundle align bias.
 */
int
Bundle_alignBase(Bundle this);
#endif//$XCC_h

#if Bundle_INSTRUCTIONS_MAX
extern const uint8_t
Bundle_AlignBase[];
int
Bundle_alignBase(Bundle this)
{
  Except_REQUIRE((unsigned)this < Bundle__);
  return Bundle_AlignBase[this];
}
#endif//Bundle_INSTRUCTIONS_MAX

#if Bundle_INSTRUCTIONS_MAX
const uint8_t
Bundle_AlignBase[] = {
#define BASE(d) d
#define ALIGNMENT(bias, base) base
#define Bundle(ID, ALIGNMENT, CONTENTS, INVERSE, TEMPLATE) \
  ALIGNMENT,
  0,
#include "Bundle.tuple"
#undef ALIGNMENT
};
#endif//Bundle_INSTRUCTIONS_MAX

#ifdef $XCC__c
#if Bundle_INSTRUCTIONS_MAX
{
  int bundle = 0;
  for (bundle = 0; bundle < Bundle__; bundle++) {
    int alignBase = Bundle_alignBase(bundle);
    printf("\nAlignBase(%s)\t%d", Bundle_Id(bundle), alignBase);
  }
  printf("\n");
  fprintf(stderr, "sizeof(Bundle_AlignBase)\t%zu\n", sizeof(Bundle_AlignBase));
}
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Bundle count of Bundling classes.
 */
int
Bundle_count(Bundle this);
#endif//$XCC_h

#if Bundle_INSTRUCTIONS_MAX
extern const uint8_t
Bundle_Count[];
int
Bundle_count(Bundle this)
{
  Except_REQUIRE((unsigned)this < Bundle__);
  return Bundle_Count[this];
}
#endif//Bundle_INSTRUCTIONS_MAX

#if Bundle_INSTRUCTIONS_MAX
const uint8_t
Bundle_Count[] = {
#define CONTENTS(count, items) count
#define Bundle(ID, ALIGNMENT, CONTENTS, INVERSE, TEMPLATE) \
  CONTENTS,
  CONTENTS(0, /**/),
#include "Bundle.tuple"
#undef CONTENTS
};
#endif//Bundle_INSTRUCTIONS_MAX

#ifdef $XCC__c
#if Bundle_INSTRUCTIONS_MAX
{
  int bundle = 0;
  for (bundle = 0; bundle < Bundle__; bundle++) {
    int count = Bundle_count(bundle);
    printf("Count(%s)\t%d\n", Bundle_Id(bundle), count);
  }
  fprintf(stderr, "sizeof(Bundle_Count)\t%zu\n", sizeof(Bundle_Count));
}
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Bundle contents.
 */
const Bundling *
Bundle_contents(Bundle this);
#endif//$XCC_h

#if Bundle_INSTRUCTIONS_MAX
extern const Bundling
Bundle_Contents[][Bundle_INSTRUCTIONS_MAX];
const Bundling *
Bundle_contents(Bundle this)
{
  Except_REQUIRE((unsigned)this < Bundle__);
  return Bundle_Contents[this];
}
#endif//Bundle_INSTRUCTIONS_MAX

#if Bundle_INSTRUCTIONS_MAX
const Bundling
Bundle_Contents[][Bundle_INSTRUCTIONS_MAX] = {
#define BUNDLING(name) Bundling_##name,
#define CONTENTS(count, items) { items }
#define Bundle(ID, ALIGNMENT, CONTENTS, INVERSE, TEMPLATE) \
  CONTENTS,
  CONTENTS(0, /**/),
#include "Bundle.tuple"
#undef CONTENTS
};
#endif//Bundle_INSTRUCTIONS_MAX

#ifdef $XCC__c
#if Bundle_INSTRUCTIONS_MAX
{
  int bundle = 0;
  for (bundle = 0; bundle < Bundle__; bundle++) {
    const Bundling *contents = Bundle_contents(bundle);
    int count = Bundle_count(bundle), i;
    printf("Contents(%s)\t", Bundle_Id(bundle));
    for (i = 0; i < count; i++) {
      printf("%s\t", Bundling_Id(contents[i]));
    }
    printf("\n");
  }
  fprintf(stderr, "sizeof(Bundle_Contents)\t%zu\n", sizeof(Bundle_Contents));
}
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Bundle inverse permutation.
 */
const uint8_t *
Bundle_inverse(Bundle this);
#endif//$XCC_h

#if Bundle_INSTRUCTIONS_MAX
extern const uint8_t
Bundle_Inverse[][Bundle_INSTRUCTIONS_MAX];
const uint8_t *
Bundle_inverse(Bundle this)
{
  Except_REQUIRE((unsigned)this < Bundle__);
  return Bundle_Inverse[this];
}
#endif//Bundle_INSTRUCTIONS_MAX

#if Bundle_INSTRUCTIONS_MAX
const uint8_t
Bundle_Inverse[][Bundle_INSTRUCTIONS_MAX] = {
#define INDEX(d) d,
#define INVERSE(indices) { indices }
#define Bundle(ID, ALIGNMENT, CONTENTS, INVERSE, TEMPLATE) \
  INVERSE,
  { 0 },
#include "Bundle.tuple"
};
#endif//Bundle_INSTRUCTIONS_MAX

#ifdef $XCC__c
#if Bundle_INSTRUCTIONS_MAX
{
  int bundle = 0;
  for (bundle = 0; bundle < Bundle__; bundle++) {
    const uint8_t *inverse = Bundle_inverse(bundle);
    int count = Bundle_count(bundle), i;
    printf("Inverse(%s)\t", Bundle_Id(bundle));
    for (i = 0; i < count; i++) {
      printf("%4d", inverse[i]);
    }
    printf("\n");
  }
  fprintf(stderr, "sizeof(Bundle_Inverse)\t%zu\n", sizeof(Bundle_Inverse));
}
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Bundle Template.
 */
Template
Bundle_template(Bundle this);
#endif//$XCC_h

#if Template_ENABLE
extern const Template
Bundle_Template[];
Template
Bundle_template(Bundle this)
{
  Except_REQUIRE((unsigned)this < Bundle__);
  return (Template)Bundle_Template[this];
}
#endif//Template_ENABLE

#if Template_ENABLE
const Template
Bundle_Template[] = {
#define BASE(d) d
#define TEMPLATE(name) Template_##name
#define Bundle(ID, ALIGNMENT, CONTENTS, INVERSE, TEMPLATE) \
  TEMPLATE,
  TEMPLATE(_UNDEF),
#include "Bundle.tuple"
};
#endif//Template_ENABLE

#ifdef $XCC__c
#if Template_ENABLE
{
  int bundle = 0;
  for (bundle = 0; bundle < Bundle__; bundle++) {
    int template = Bundle_template(bundle);
    printf("\nTemplate(%s)\t%s", Bundle_Id(bundle), Template_Id(template));
  }
  printf("\n");
  fprintf(stderr, "sizeof(Bundle_Template)\t%zu\n", sizeof(Bundle_Template));
}
#endif//Template_ENABLE
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Match between a sorted sequence of Bundling classes and Bundle(s).
 *
 * The HASH value is computed from a sorted sequence of Bundling classes as follows:
 *
 *	HASH = -1;
 *	for (i = 0; i < length; i++) {
 *	  HASH++; HASH *= (Bundling__ - Bundling__BIAS);
 *	  HASH += (bundlings[i] -- Bundling__BIAS);
 *	}
 *	HASH++;
 */
struct BundleMatch_;
#endif//$XCC_h

#ifdef $XCC__h
#if BundleMatch_ENABLE
struct BundleMatch_ {
  uint32_t HASH;	// HASH value for this BundleMatch.
  uint32_t COUNT;	// Count of Bundle(s) in BUNDLES.
  Bundle BUNDLES[BundleMatch_MAXCOUNT];
};
#endif//BundleMatch_ENABLE
#endif//$XCC__h

#ifdef $XCC__c
{
#if BundleMatch_ENABLE
  fprintf(stderr, "sizeof(BundleMatch_)\t%zu\n", sizeof(BundleMatch_));
#endif//BundleMatch_ENABLE
}
#endif//$XCC__c

#if BundleMatch_ENABLE
const BundleMatch_
BundleMatch_Dense[] = {
#define HASH(d) d
#define BUNDLE(name) Bundle_##name,
#define BUNDLES(length, items) length, { items }
#define BundleMatch(HASH, CANONIC, BUNDLES) \
  { HASH, BUNDLES },
#include "Bundle.tuple"
};
#endif//BundleMatch_ENABLE

#ifdef $XCC__c
#if BundleMatch_ENABLE
{
  fprintf(stderr, "sizeof(BundleMatch_Dense)\t%zu\n", sizeof(BundleMatch_Dense));
}
#endif//BundleMatch_ENABLE
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#if BundleMatch_ENABLE
const_BundleMatch
BundleMatch_Table[BundleMatch_HASH_MAX+1];
#endif//BundleMatch_ENABLE

#ifdef $XCC_h
/**
 * Given a HASH and nativePC, find a matching Bundle.
 */
Bundle
BundleMatch_FindBundle(uint32_t hash, PlatformUInt nativePC);
#endif//$XCC_h

#if BundleMatch_ENABLE
Bundle
BundleMatch_FindBundle(uint32_t hash, PlatformUInt nativePC)
{
  if (hash < sizeof(BundleMatch_Table)/sizeof(BundleMatch_Table[0])) {
    const_BundleMatch match = BundleMatch_Table[hash];
    if (match != NULL) {
      const Bundle *bundles = BundleMatch_BUNDLES(match);
      int count = BundleMatch_COUNT(match), i = 0;
      Except_CHECK(count > 0);
      do {
        int bundle = bundles[i];
        unsigned alignBase = Bundle_alignBase(bundle);
        unsigned alignBias = Bundle_alignBias(bundle);
        if ((nativePC & (alignBase - 1)) == alignBias) {
          return bundle;
        }
      } while (++i < count);
    }
  }
  return Bundle_;
}
#endif//BundleMatch_ENABLE

#ifdef $XCC__c
#if BundleMatch_ENABLE
{
  int bundle = 0;
  for (bundle = 1; bundle < Bundle__; bundle++) {
    uint32_t hash = -1;
    int found = Bundle_;
    BundlingBuffer_ buffer[1];
    Bundling *bundlings = BundlingBuffer_BUNDLINGS(buffer);
    unsigned nativePC = Bundle_alignBase(bundle) + Bundle_alignBias(bundle);
    const Bundling *contents = Bundle_contents(bundle);
    int count = Bundle_count(bundle), i;
    for (i = 0; i < count; i++) {
      bundlings[i] = contents[i];
    }
    hash = BundlingBuffer_sort(buffer, count);
    printf("Match(%s)\t(", Bundle_Id(bundle));
    for (i = 0; i < count; i++) {
      printf("%s", Bundling_Id(bundlings[i]));
      if (i < count - 1) printf(",");
    }
    printf(")\tHASH(%u)\n", hash);
    found = BundleMatch_FindBundle(hash, nativePC);
    Except_ALWAYS(found == bundle);
  }
}
#endif//BundleMatch_ENABLE
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
#if Bundle_INSTRUCTIONS_MAX
/**
 * BundleSlot -- Bundle words for an issue slot.
 */
struct BundleSlot_ {
  uint32_t MAINWORD;
  uint32_t IMMXWORDS[2];
  uint8_t IMMXCOUNT;
  Decoding DECODING;
};
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__h

#ifdef $XCC__c
{
#if Bundle_INSTRUCTIONS_MAX
  fprintf(stderr, "sizeof(BundleSlot_)\t%zu\n", sizeof(BundleSlot_));
#endif//Bundle_INSTRUCTIONS_MAX
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * BundleEncoded -- Bundle encoded representation assembled or dispersed.
 */
struct BundleEncoded_;
#endif//$XCC_h

#ifdef $XCC__h
/*
 * BundleEncoded_gather -- Gather the BundleEncoded syllables.
 *
 * |return| The number of |words| consumed or <= 0 if error.
 */
#if Bundle_INSTRUCTIONS_MAX
int
BundleEncoded_gather(BundleEncoded this, const uint32_t *words, int count);
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__h

#ifdef $XCC__h
/*
 * BundleEncoded_transfer -- Transfer the BundleEncoded to OpcodeBuffer(s).
 *
 * |return| The total number of code words or <= 0 if error.
 */
#if Bundle_INSTRUCTIONS_MAX
int
BundleEncoded_transfer(BundleEncoded this, OpcodeBuffer_ opcodeBuffers[]);
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Bundle module.
 */
#if BundleMatch_ENABLE
void
Bundle_INIT(void);
#else//!BundleMatch_ENABLE
#define Bundle_INIT()
#endif//BundleMatch_ENABLE
#endif//$XCC__h

#if BundleMatch_ENABLE
void
Bundle_INIT(void)
{
  int count = sizeof(BundleMatch_Dense)/sizeof(BundleMatch_Dense[0]), i;
  for (i = 0; i < count; i++) {
    const_BundleMatch match = BundleMatch_Dense + i;
    uint32_t hash = BundleMatch_HASH(match);
    Except_CHECK(hash <= BundleMatch_HASH_MAX);
    Except_CHECK(BundleMatch_Table[hash] == NULL);
    BundleMatch_Table[hash] = match;
  }
}
#endif//BundleMatch_ENABLE

#ifdef $XCC__h
/**
 * Finalize the Bundle module.
 */
#define Bundle_FINI()
#endif//$XCC__h

#if XCC__C
static void
Bundle_TEST(void)
{
#include "MDT/Bundle_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Bundle_TEST);
  return 0;
}
#endif

