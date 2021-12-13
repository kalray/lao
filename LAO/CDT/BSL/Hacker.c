#ifdef $XCC_h
/*
 * !!!!	Hacker.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2007 STMicroelectronics.
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
 * @ingroup BSL
 * @brief This module provides an implementation of Hacker's Delights
 * algorithms.
 *
 * Hacker's Delight is a book by Henry S. Warren, Jr.
 * It discusses a variety of programming algorithms for common tasks involving
 * integer types, often with the aim of performing the minimum number of
 * operations or replacing slow operations by faster ones.
 */
#endif//$XCC_h

#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/Hacker.h"
#endif//$XCC__h

#ifdef $XCC_h
/*
 * Macro to round a uint16_t to the nearest upper power of 2.
 *
 * From ``Hacker's Delight'', H. S. Warren, Addison Wesley Professional 2003.
 * Due to the large size of the expanded macro, only apply to constant arguments.
 */
#define HackerU16_CLP2a(x)	((x) - 1)
#define HackerU16_CLP2b(x)	((x) | ((x)>>1))
#define HackerU16_CLP2c(x)	((x) | ((x)>>2))
#define HackerU16_CLP2d(x)	((x) | ((x)>>4))
#define HackerU16_CLP2e(x)	((x) | ((x)>>8))
#define uint16_CLP2(x)	\
  (HackerU16_CLP2e(HackerU16_CLP2d(HackerU16_CLP2c(HackerU16_CLP2b(HackerU16_CLP2a(x))))) + 1)
#endif//$XCC_h

#if XCC__C
static unsigned args[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  15, 16, 17,
  31, 32, 33,
  63, 64, 65,
  127, 128, 129,
  255, 256, 257,
  511, 512, 513,
  1023, 1024, 1025,
  2047, 2048, 2049,
  4095, 4096, 4097,
  8191, 8192, 8193,
};
#endif

#ifdef $XCC__c
{
  int i;
  for (i = 0; i < sizeof(args)/sizeof(*args); i++) {
    unsigned j = (uint16_t)args[i];
    printf("CLP2(%u) = %u\n", j, uint16_CLP2(j));
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Count the number of bits set in a uint32_t.
 */
static inline int
HackerU32_popc(uint32_t word)
{
  uint32_t v = word;
  uint32_t w = v - ((v >> 1) & 0x55555555);
  uint32_t x = (w & 0x33333333) + ((w >> 2) & 0x33333333);
  uint32_t y = (x + (x >> 4)) & 0x0F0F0F0F;
  uint16_t z = (y * 0x01010101) >> 24;
  return z;
}
#endif//$XCC_h

#if 0
static inline int
HackerU32_popc(uint32_t word)
{
  uint32_t v = word;
  uint32_t w = v - ((v >> 1) & 0x55555555);
  uint32_t x = (w & 0x33333333) + ((w >> 2) & 0x33333333);
  uint32_t y = (x + (x >> 4)) & 0x0F0F0F0F;
  uint32_t z = y + (y << 8);
  uint32_t t = z + (z << 16);
  return t >> 24;
}
#endif

#ifdef $XCC_h
/**
 * Round down to the nearest power of 2.
 */
static inline int
HackerU32_flp2(uint32_t word)
{
  uint32_t x = word;
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  x -= (x >> 1);
  return x;
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int i;
  for (i = 0; i < sizeof(args)/sizeof(*args); i++) {
    unsigned j = args[i];
    printf("flp2(%u) = %u\n", j, HackerU32_flp2(j));
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Round up to the nearest power of 2.
 */
static inline int
HackerU32_clp2(uint32_t word)
{
  uint32_t x = word - 1;
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  return x + 1;
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int i;
  for (i = 0; i < sizeof(args)/sizeof(*args); i++) {
    unsigned j = args[i];
    printf("clp2(%u) = %u\n", j, HackerU32_clp2(j));
  }
}
#endif//$XCC__c

#ifdef $XCC_h
static inline int
HackerU64_popcBasic(uint64_t dword)
{
  int bitCount = 0;
  while (dword) {
    if (dword & 1) ++bitCount;
    dword >>= 1;
  }
  return bitCount;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int
HackerU64_popcSparse(uint64_t dword)
{
  int bitCount = 0;
  while (dword) {
    // Clear the rightmost 1 bit in dword.
    dword &= dword - 1;
    ++bitCount;
  }
  return bitCount;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count the number of bits set in a uint64_t.
 *
 * See <http://www.caam.rice.edu/~dougm/twiddle/HackerU64_popc.html>.
 */
static inline int
HackerU64_popc(uint64_t dword)
{
  uint32_t word0 = dword, word1 = dword >> 32;
  int popc = HackerU32_popc(word0) + HackerU32_popc(word1);
  Except_CHECK(popc == HackerU64_popcBasic(dword));
  return popc;
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int64_t dword = 0xdeadbeef12345678LL;
  while (dword != 0) {
    int popc = HackerU64_popc(dword);
    int popcBasic = HackerU64_popcBasic(dword);
    int popcSparse = HackerU64_popcSparse(dword);
    printf("popc(%0"PRIx64")=%d\n", dword, popc);
    Except_ALWAYS(popc == popcBasic && popc == popcSparse);
    dword <<= 1;
  }
  printf("popc(%0"PRIx64")=%d\n", dword, HackerU64_popc(dword));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * The published hash algorithm used in the UNIX ELF format.
 *
 * Accepts a pointer to a string to be hashed and returns an 32-bit unsigned.
 */
uint32_t
Hacker_hashELF(const char *name);
#endif//$XCC_h

uint32_t
Hacker_hashELF(const char *name)
{
  const uint8_t *bytes = (const uint8_t *)name;
  uint32_t h = 0, g;
  while (*bytes) {
    h = (h << 4) + *bytes++;
    if ((g = h & 0xf0000000)) {
      h ^= g >> 24;
    }
    h &= ~g;
  }
  return h;
}

#ifdef $XCC_h
/**
 * 32-bit Fowler / Noll / Vo (FNV) hash algorithm.
 *
 * See http://www.isthe.com/chongo/tech/comp/fnv/.
 */
#define Hacker_hashFNV32_PRIME	16777619U	// FNV prime for 32-bit hash.
#define Hacker_hashFNV32_OFFSET	2166136261U	// FNV offset for 32-bit hash.
uint32_t
Hacker_hashFNV32(const char *name);
#endif//$XCC_h

uint32_t
Hacker_hashFNV32(const char *name)
{
  const uint8_t *bytes = (const uint8_t *)name;
  uint32_t hash = Hacker_hashFNV32_OFFSET;
  while (*bytes) {
    hash *= Hacker_hashFNV32_PRIME;
    hash ^= *bytes++;
  }
  return hash;
}

#ifdef $XCC_h
/**
 * mix 3 32-bit values reversibly.
 *
 * By Bob Jenkins, 1996. bob_jenkins@burtleburtle.net. You may use this code any
 * way you wish, private, educational, or commercial. It's free.
 *
 * For every delta with one or two bits set, and the deltas of all three high
 * bits or all three low bits, whether the original value of a,b,c is almost all
 * zero or is uniformly distributed,
 *
 * * If Hacker_mixJenkins is run forward or backward, at least 32 bits in a,b,c have
 * at least 1/4 probability of changing.
 *
 * * If Hacker_mixJenkins is run forward, every bit of c will change between 1/3 and
 * 2/3 of the time. (Well, 22/100 and 78/100 for some 2-bit deltas.)
 *
 * Hacker_mixJenkins was built out of 36 single-cycle latency instructions in a
 * structure that could supported 2x parallelism, like so:
 *
 *	a -= b;
 *	a -= c; x = (c>>13);
 *	b -= c; a ^= x;
 *	b -= a; x = (a<<8);
 *	c -= a; b ^= x;
 *	c -= b; x = (b>>13);
 *	...
 *
 * Unfortunately, superscalar Pentiums and Sparcs can't take advantage of that
 * parallelism. They've also turned some of those single-cycle latency
 * instructions into multi-cycle latency instructions. Still, this is the fastest
 * good hash I could find. There were about 2^^68 to choose from. I only looked
 * at a billion or so.
 */
#define Hacker_mixJenkins(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12); \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3); \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Hash a variable-length key into a 32-bit value.
 *
 * By Bob Jenkins, 1996. bob_jenkins@burtleburtle.net. You may use this code any
 * way you wish, private, educational, or commercial. It's free.
 *
 * key:	The key (the unaligned variable-length array of bytes).
 * length:	The length of the key, counting by bytes.
 * initval:	Can be any 4-byte value.
 * Return:	a 32-bit value.
 *
 * Every bit of the key affects every bit of the return value. Every 1-bit and
 * 2-bit delta achieves avalanche. About 6*len+35 instructions.
 *
 * The best hash table sizes are powers of 2. There is no need to do mod a prime
 * (mod is sooo slow!). If you need less than 32 bits, use a bitmask. For
 * example, if you need only 10 bits, do
 *
 *	h = (h & hashmask(10));
 *
 * In which case, the hash table should have hashsize(10) elements.
 *
 * If you are hashing n strings (ub1 **)k, do it like this:
 *
 * @verbatim
 *   for (i=0, h=0; i<n; ++i) h = Hacker_hashJenkins( k[i], len[i], h);
 * @endverbatim
 *
 * See http://burtleburtle.net/bob/hash/evahash.html
 *
 * Use for hash table lookup, or anything where one collision in 2^^32 is
 * acceptable.  Do NOT use for cryptographic purposes.
 */
uint32_t
Hacker_hashJenkins(const uint8_t *key, uint32_t length, uint32_t initval);
#endif//$XCC_h

uint32_t
Hacker_hashJenkins(const uint8_t *key, uint32_t length, uint32_t initval)
{
  register uint32_t a,b,c,len;
  /* set up the internal state */
  len = length;
  a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
  c = initval;         /* the previous hash value */
  /*---------------------------------------- handle most of the key */
  while (len >= 12)
  {
    a += (key[0] +((uint32_t)key[1]<<8) +((uint32_t)key[2]<<16) +((uint32_t)key[3]<<24));
    b += (key[4] +((uint32_t)key[5]<<8) +((uint32_t)key[6]<<16) +((uint32_t)key[7]<<24));
    c += (key[8] +((uint32_t)key[9]<<8) +((uint32_t)key[10]<<16)+((uint32_t)key[11]<<24));
    Hacker_mixJenkins(a,b,c);
    key += 12; len -= 12;
  }
  /*------------------------------------- handle the last 11 bytes */
  c += length;
  switch(len)          /* all the case statements fall through */
  {
  case 11: c+=((uint32_t)key[10]<<24);
  case 10: c+=((uint32_t)key[9]<<16);
  case 9 : c+=((uint32_t)key[8]<<8);
    /* the first byte of c is reserved for the length */
  case 8 : b+=((uint32_t)key[7]<<24);
  case 7 : b+=((uint32_t)key[6]<<16);
  case 6 : b+=((uint32_t)key[5]<<8);
  case 5 : b+=key[4];
  case 4 : a+=((uint32_t)key[3]<<24);
  case 3 : a+=((uint32_t)key[2]<<16);
  case 2 : a+=((uint32_t)key[1]<<8);
  case 1 : a+=key[0];
    /* case 0: nothing left to add */
  }
  Hacker_mixJenkins(a,b,c);
  /*-------------------------------------------- report the result */
  return c;
}

#ifdef $XCC_h
typedef void *HackerPtr;
typedef const void *const_HackerPtr;
typedef int (*HackerPtrCompare)(const_HackerPtr, const_HackerPtr);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Function used to merge sort a HackerPtr array.
 */
void
HackerPtr_SortArray(HackerPtr array[], int32_t first, int32_t last, HackerPtrCompare cmp);
#endif//$XCC_h

void
HackerPtr_SortArray(HackerPtr array[], int32_t first, int32_t last, HackerPtrCompare cmp)
{
  int32_t count = last - first + 1;
  int32_t mid = (first + last) >> 1;
  int32_t idx1 = first, idx2 = mid + 1, i;
  if (idx1 < mid) HackerPtr_SortArray(array, idx1, mid, cmp);
  if (idx2 < last) HackerPtr_SortArray(array, idx2, last, cmp);
  // Merge step.
  if (count > 1) {
    HackerPtr *tempArray = alloca(sizeof(HackerPtr)*count);
    for (i = 0; i < count; i++) {
      if (idx1 > mid) {
        tempArray[i] = array[idx2++];
      } else if (idx2 > last) {
        tempArray[i] = array[idx1++];
      } else if ((*cmp)(array[idx1], array[idx2]) <= 0) {
        tempArray[i] = array[idx1++];
      } else {
        tempArray[i] = array[idx2++];
      }
    }
    for (i = 0; i < count; i++) {
      array[first + i] = tempArray[i];
    }
  }
}

#ifdef $XCC__c
{
  int i = 0, count = 0;
  HackerPtr array[] = {
    (HackerPtr)13,
    (HackerPtr)25,
    (HackerPtr)5,
    (HackerPtr)4,
    (HackerPtr)3,
    (HackerPtr)10,
    (HackerPtr)11,
    (HackerPtr)12,
  };
  count = sizeof(array)/sizeof(array[0]);
  printf("array:  ");
  for (i = 0; i < count; i++) {
    printf(" %2"PRId64, ((int64_t) ((intptr_t) array[i])));
  }
  printf("\n");
  HackerPtr_SortArray(array, 0, count - 1, HackerPtr_compare);
  printf("sorted: ");
  for (i = 0; i < count; i++) {
    printf(" %2"PRId64, ((int64_t) ((intptr_t) array[i])));
  }
  printf("\n");
}
#endif//$XCC__c

#if XCC__C
static int
HackerPtr_compare(const_HackerPtr a, const_HackerPtr b)
{
  return (a > b) - (a < b);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Hacker module.
 */
#define Hacker_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Hacker module.
 */
#define Hacker_FINI()
#endif//$XCC__h

#if XCC__C
static void
Hacker_TEST(void)
{
#include "BSL/Hacker_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(Hacker_TEST);
  return 0;
}
#endif

