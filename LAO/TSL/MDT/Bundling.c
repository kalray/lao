#ifdef $XCC_h
/*
 * !!!!	Bundling.xcc
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
#include "MDT/Bundling.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Bundling(NAME) \
  Bundling_##NAME,
  Bundling_,
#include /*MDT*/"Bundling.tuple"
  Bundling__
#undef Bundling
} enum_Bundling;
#endif//$XCC__h

const int Bundling__Count = Bundling__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Bundling enumeration.
 */
extern const char *
Bundling_Id(Bundling this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Bundling_Id(Bundling this)
{
  static const char *_[] = {
#define Bundling(NAME) \
    #NAME,
    "_UNDEF",
#include "Bundling.tuple"
  };
  Except_REQUIRE((unsigned)this < Bundling__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int bundling = 0;
  for (bundling = 0; bundling < Bundling__; bundling++) {
    printf("Bundling_%s\t%d\n", Bundling_Id(bundling), bundling);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Buffer used to reorder Bundling(s).
 */
struct BundlingBuffer_ {
  Bundling BUNDLINGS[Bundle__MAXINSTRUCTIONS];
  uint8_t PERMUTE[Bundle__MAXINSTRUCTIONS];
};
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Sort the BUNDLINGS array and fill the PERMUTE array.
 *
 * Return:	hash value suitable for use by @c BundleMatch_FindBundle.
 *
 * The hash value is computed as follows:
 *
 *	hash = -1;
 *	for (i = 0; i < count; i++) {
 *	  hash++; hash *= (Bundling__ - Bundling__BIAS);
 *	  hash += (bundlings[i] - Bundling__BIAS);
 *	}
 *	hash++;
 */
int32_t
BundlingBuffer_sort(BundlingBuffer this, int count);
#endif//$XCC_h

#if Bundle_INSTRUCTIONS_MAX
int32_t
BundlingBuffer_sort(BundlingBuffer this, int count)
{
  Bundling *bundlings = BundlingBuffer_BUNDLINGS(this);
  uint8_t *permute = BundlingBuffer_PERMUTE(this);
  uint32_t hash = -1;
  if (count >= 1) {
    hash = bundlings[0] - Bundling__BIAS;
    permute[0] = 0;
    if (count >= 2) {
      int bundling_0 = bundlings[0];
      int bundling_1 = bundlings[1];
      permute[1] = 1;
      if (bundling_1 < bundling_0) {
        int bundling_t = bundling_0;
        bundlings[0] = bundling_1;
        bundlings[1] = bundling_0;
        permute[0] = 1;
        permute[1] = 0;
        bundling_0 = bundling_1;
        bundling_1 = bundling_t;
      }
      if (count >= 3) {
        int bundling_2 = bundlings[2];
        int permute_2 = 2;
        permute[2] = permute_2;
        if (bundling_2 < bundling_0) {
          int bundling_t = bundling_0;
          bundlings[0] = bundling_2;
          bundlings[2] = bundling_0;
          permute[2] = permute[0];
          permute[0] = permute_2;
          permute_2 = permute[2];
          bundling_2 = bundling_t;
        }
        if (bundling_2 < bundling_1) {
          bundlings[1] = bundling_2;
          bundlings[2] = bundling_1;
          permute[2] = permute[1];
          permute[1] = permute_2;
        }
        if (count >= 4) {
          int sortedCount = 3, i, j;
          // Insertion sort. TODO: implement a better sort algorithm.
          for (i = sortedCount; i < count; i++) {
            int bundling_t = bundlings[i];
            int permute_t = i;
            permute[i] = permute_t;
            for (j = i-1; j >= 0 && bundling_t < bundlings[j]; j--) {
              bundlings[j+1] = bundlings[j];
              permute[j+1] = permute[j];
            }
            bundlings[j+1] = bundling_t;
            permute[j+1] = permute_t;
          }
          hash = bundlings[0] - Bundling__BIAS;
          for (i = 1; i < count; i++) {
            hash++; hash *= (Bundling__ - Bundling__BIAS);
            hash += (bundlings[i] - Bundling__BIAS);
          }
        } else {
          // count == 3
          hash = bundlings[0] - Bundling__BIAS;
          hash++; hash *= (Bundling__ - Bundling__BIAS);
          hash += bundlings[1] - Bundling__BIAS;
          hash++; hash *= (Bundling__ - Bundling__BIAS);
          hash += bundlings[2] - Bundling__BIAS;
        }
      } else {
        // count == 2
        hash = bundlings[0] - Bundling__BIAS;
        hash++; hash *= (Bundling__ - Bundling__BIAS);
        hash += bundlings[1] - Bundling__BIAS;
      }
    }
  }
  return ++hash;
}
#endif//Bundle_INSTRUCTIONS_MAX

#ifdef $XCC__c
#if Bundle_INSTRUCTIONS_MAX
{
  int i, j, k, l;
  int count = Bundle_INSTRUCTIONS_MAX;
  k = 0;
  l = Bundling__ != 7? 7: 11;
  for (j = 0; j < 20; j++) {
    BundlingBuffer_ buffer[1];
    Bundling *bundlings = BundlingBuffer_BUNDLINGS(buffer);
    uint8_t *permute = BundlingBuffer_PERMUTE(buffer);
    Bundling unsorted_bundlings[Bundle_INSTRUCTIONS_MAX];
    printf("Bundlings:");
    for (i = 0; i < count; i++) {
      unsorted_bundlings[i] = bundlings[i] = k%(Bundling__ - 1) + 1;
      printf("\t%s", Bundling_Id(bundlings[i]));
      k += l;
    }
    printf("\nSorted:   ");
    BundlingBuffer_sort(buffer, count);
    for (i = 0; i < count; i++) {
      printf("\t%s", Bundling_Id(bundlings[i]));
      if (i > 0) Except_ALWAYS(bundlings[i] >= bundlings[i - 1]);
    }
    printf("\nPermute:  ");
    for (i = 0; i < count; i++) {
      printf("\t%d\t", permute[i]);
      Except_ALWAYS(unsorted_bundlings[permute[i]] == bundlings[i]);
    }
    printf("\n");
  }
}
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Bundling module.
 */
#define Bundling_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Bundling module.
 */
#define Bundling_FINI()
#endif//$XCC__h

#if XCC__C
static void
Bundling_TEST(void)
{
#include "MDT/Bundling_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Bundling_TEST);
  return 0;
}
#endif

