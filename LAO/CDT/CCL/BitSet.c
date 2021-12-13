#ifdef $XCC_h
/*
 * !!!!	BitSet.xcc
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
 * @ingroup CCL
 * @brief Integer set implement at bit-vector.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/BitSet.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef uint32_t BitSetMember;
typedef uint64_t BitSetWord;
#define BitSet_WORDBITS (sizeof(BitSetWord)*8)
#define BitSet_MAJOR(member) ((BitSetMember)(member)/(BitSet_WORDBITS))
#define BitSet_MINOR(member) ((BitSetMember)(member)&(BitSet_WORDBITS - 1))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Integer Set implemented as a bit vector.
 * @param memory The Memory where the BitSet members are stored.
 * @param universe This BitSet universe.
 */
struct BitSet_ {
  //@args	Memory memory, int32_t universe
  IArray_ ARRAY_;
  //@access ARRAY	BitSet__ARRAY_(this)
  //@access MEMORY	IArray_MEMORY(BitSet_ARRAY(this))
  //@access WORDS	((BitSetWord *)IArray_BASE(BitSet_ARRAY(this)))
};
#endif//$XCC_h

BitSet
BitSet_Ctor(BitSet this, Memory memory, int32_t universe)
{
  int32_t wordCount = BitSet_MAJOR(universe + BitSet_WORDBITS - 1);
  IArray_Ctor(BitSet_ARRAY(this), memory, sizeof(BitSetWord), wordCount);
  return this;
}

void
BitSet_Dtor(BitSet this)
{
  IArray_Dtor(BitSet_ARRAY(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(BitSet_)\t%zu\n", sizeof(BitSet_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Resize the BitSet.
 * maxCount:	The new @c maxCount.
 */
void
BitSet_resize(BitSet this, int32_t universe);
#endif//$XCC_h

void
BitSet_resize(BitSet this, int32_t universe)
{
  int32_t wordCount = BitSet_MAJOR(universe + BitSet_WORDBITS - 1);
  IArray_resize(BitSet_ARRAY(this), wordCount);
}

#ifdef $XCC_h
/**
 * Empty this BitSet
 */
void
BitSet_empty(BitSet this);
#endif//$XCC_h

void
BitSet_empty(BitSet this)
{
  IArray_empty(BitSet_ARRAY(this), NULL);
  Except_CHECK(BitSet_isEmpty(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This BitSet memory.
 */
static inline Memory
BitSet_memory(const_BitSet this)
{
  return BitSet_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count the number of BitSet_WORDs in this BitSet.
 */
static inline uint32_t
BitSet_wordCount(const_BitSet this)
{
  const_IArray array = BitSet_ARRAY(this);
  const char *base = (char *)IArray_BASE(array);
  const char *past = (char *)IArray_PAST(array);
  return (past - base)/sizeof(BitSetWord);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Return the BitSet word array.
 */
static inline const BitSetWord *
BitSet_words(const_BitSet this)
{
  return BitSet_WORDS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this BitSet is empty.
 */
bool
BitSet_isEmpty(const_BitSet this);
#endif//$XCC_h

bool
BitSet_isEmpty(const_BitSet this)
{
  const BitSetWord *words = BitSet_words(this);
  int32_t wordCount = BitSet_wordCount(this), i;
  for (i = 0; i < wordCount; i++) {
    if (words[i] != 0) return false;
  }
  return true;
}

#ifdef $XCC_h
/**
 * True iff this BitSet contains a single member.
 */
#define BitSet_isSingle(this)	(BitSet_count(this) == 1)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count members contained in the BitSet.
 * @return The count of members in this BitSet.
 */
int32_t
BitSet_count(const_BitSet this);
#endif//$XCC_h

int32_t
BitSet_count(const_BitSet this)
{
  const BitSetWord *words = BitSet_words(this);
  int32_t wordCount = BitSet_wordCount(this), count = 0, i;
  if (sizeof(BitSetWord) == sizeof(uint64_t)) {
    for (i = 0; i < wordCount; i++) count += HackerU64_popc(words[i]);
  } else
  if (sizeof(BitSetWord) == sizeof(uint32_t)) {
    for (i = 0; i < wordCount; i++) count += HackerU32_popc(words[i]);
  } else Except_NEVER(true);
  return count;
}

#ifdef $XCC_h
/**
 * Test a member for containment.
 * @return True if this BitSet contains member.
 */
static inline bool
BitSet_contains(const_BitSet this, BitSetMember member)
{
  BitSetWord old_word, new_word;
  uint32_t major = BitSet_MAJOR(member);
  uint32_t minor = BitSet_MINOR(member);
  Except_REQUIRE(major < BitSet_wordCount(this));
  old_word = new_word = BitSet_WORDS(this)[major];
  new_word |= ((BitSetWord)1 << minor);
  return old_word == new_word;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Insert a member in this BitSet.
 * @return False iff member was already contained in this BitSet.
 */
static inline bool
BitSet_insert(BitSet this, BitSetMember member)
{
  BitSetWord old_word, new_word;
  uint32_t major = BitSet_MAJOR(member);
  uint32_t minor = BitSet_MINOR(member);
  Except_REQUIRE(major < BitSet_wordCount(this));
  old_word = new_word = BitSet_WORDS(this)[major];
  new_word |= ((BitSetWord)1 << minor);
  BitSet_WORDS(this)[major] = new_word;
  return old_word != new_word;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Remove a member from this BitSet.
 * @return False iff member was not contained in this BitSet.
 */
static inline bool
BitSet_remove(BitSet this, BitSetMember member)
{
  BitSetWord old_word, new_word;
  uint32_t major = BitSet_MAJOR(member);
  uint32_t minor = BitSet_MINOR(member);
  Except_REQUIRE(major < BitSet_wordCount(this));
  old_word = new_word = BitSet_WORDS(this)[major];
  new_word &= ~((BitSetWord)1 << minor);
  BitSet_WORDS(this)[major] = new_word;
  return old_word != new_word;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Choose and remove a member of the BitSet.
 */
BitSetMember
BitSet_choose(BitSet this);
#endif//$XCC_h

BitSetMember
BitSet_choose(BitSet this)
{
  BitSetWord *words = BitSet_WORDS(this);
  int32_t wordCount = BitSet_wordCount(this), i, j = 0;
  for (i = 0; i < wordCount; i++) {
    BitSetWord word = words[i];
    BitSetWord least = word & -word;
#if __GNUC__ > 2
    if (word != 0) {
      if (sizeof(word) <= sizeof(int)) {
        j = __builtin_ctz(word);
      } else if (sizeof(word) <= sizeof(long long)) {
        j = __builtin_ctzll(word);
      } else Except_NEVER(true);
      words[i] = word ^ least;
      return i*BitSet_WORDBITS + j;
    }
#else//__GNUC__
    for (j = 0; word != 0; j++, word >>= 1) {
      if (word & 1) {
        words[i] &= ~((BitSetWord)1 << j);
        return i*BitSet_WORDBITS + j;
      }
    }
#endif//__GNUC__
  }
  return (BitSetMember)-1;
}

#ifdef $XCC_h
/**
 * Get the next member of the BitSet starting from member inclusive.
 */
BitSetMember
BitSet_nextMember(const_BitSet this, BitSetMember member);
#endif//$XCC_h

BitSetMember
BitSet_nextMember(const_BitSet this, BitSetMember member)
{
  BitSetWord *words = BitSet_WORDS(this);
  int32_t wordCount = BitSet_wordCount(this);
  uint32_t i = BitSet_MAJOR(member + 1);
  uint32_t j = BitSet_MINOR(member + 1);
  if (i < wordCount) {
    BitSetWord word = words[i] >> j;
#if __GNUC__ > 2
    word <<= j;
    if (word != 0) {
      if (sizeof(word) <= sizeof(int)) {
        j = __builtin_ctz(word);
      } else if (sizeof(word) <= sizeof(long long)) {
        j = __builtin_ctzll(word);
      } else Except_NEVER(true);
      return i*BitSet_WORDBITS + j;
    }
#else//__GNUC__
    for (; word != 0; j++, word >>= 1) {
      if (word & 1) {
        return i*BitSet_WORDBITS + j;
      }
    }
#endif//__GNUC__
  }
  for (i++; i < wordCount; i++) {
    BitSetWord word = words[i];
#if __GNUC__ > 2
    if (word != 0) {
      if (sizeof(word) <= sizeof(int)) {
        j = __builtin_ctz(word);
      } else if (sizeof(word) <= sizeof(long long)) {
        j = __builtin_ctzll(word);
      } else Except_NEVER(true);
      return i*BitSet_WORDBITS + j;
    }
#else//__GNUC__
    for (j = 0; word != 0; j++, word >>= 1) {
      if (word & 1) {
        return i*BitSet_WORDBITS + j;
      }
    }
#endif//__GNUC__
  }
  return (BitSetMember)-1;
}

#ifdef $XCC_h
/**
 * Test for BitSet equality.
 */
bool
BitSet_equals(const_BitSet this, const_BitSet that);
#endif//$XCC_h

bool
BitSet_equals(const_BitSet this, const_BitSet that)
{
  Except_REQUIRE(BitSet_wordCount(this) == BitSet_wordCount(that));
  return memcmp(BitSet_WORDS(this), BitSet_WORDS(that),
                BitSet_wordCount(this)*sizeof(BitSetWord));
}

#ifdef $XCC_h
/**
 * Assign the contents of that BitSet to this BitSet.
 */
void
BitSet_assign(BitSet this, const_BitSet that);
#endif//$XCC_h

void
BitSet_assign(BitSet this, const_BitSet that)
{
  Except_REQUIRE(BitSet_wordCount(this) == BitSet_wordCount(that));
  memcpy(BitSet_WORDS(this), BitSet_WORDS(that),
         BitSet_wordCount(this)*sizeof(BitSetWord));
}

#ifdef $XCC_h
/**
 * True iff this BitSet comprises that BitSet.
 */
bool
BitSet_comprises(const_BitSet this, const_BitSet that);
#endif//$XCC_h

bool
BitSet_comprises(const_BitSet this, const_BitSet that)
{
  int32_t wordCount = BitSet_wordCount(this), i;
  Except_REQUIRE(BitSet_wordCount(this) == BitSet_wordCount(that));
  for (i = 0; i < wordCount; i++) {
    BitSetWord this_word = BitSet_WORDS(this)[i];
    BitSetWord that_word = BitSet_WORDS(that)[i];
    if ((this_word | that_word) != this_word) return false;
  }
  return true;
}

#ifdef $XCC_h
/**
 * Union of this BitSet with that BitSet.
 */
bool
BitSet_union(BitSet this, const_BitSet that);
#endif//$XCC_h

bool
BitSet_union(BitSet this, const_BitSet that)
{
  BitSetWord change = 0;
  int32_t wordCount = BitSet_wordCount(this), i;
  Except_REQUIRE(BitSet_wordCount(this) == BitSet_wordCount(that));
  // Assume non used bits in the last word are all zero.
  for (i = 0; i < wordCount; i++) {
    BitSetWord old_word = BitSet_WORDS(this)[i];
    BitSetWord new_word = old_word | BitSet_WORDS(that)[i];
    BitSet_WORDS(this)[i] = new_word;
    change |= new_word ^ old_word;
  }
  return change != 0;
}

#ifdef $XCC_h
/**
 * Intersect this BitSet with that BitSet.
 */
bool
BitSet_inter(BitSet this, const_BitSet that);
#endif//$XCC_h

bool
BitSet_inter(BitSet this, const_BitSet that)
{
  BitSetWord change = false;
  int32_t wordCount = BitSet_wordCount(this), i;
  Except_REQUIRE(BitSet_wordCount(this) == BitSet_wordCount(that));
  // Assume non used bits in the last word are all zero.
  for (i = 0; i < wordCount; i++) {
    BitSetWord old_word = BitSet_WORDS(this)[i];
    BitSetWord new_word = old_word & BitSet_WORDS(that)[i];
    BitSet_WORDS(this)[i] = new_word;
    change |= new_word ^ old_word;
  }
  return change != 0;
}

#ifdef $XCC_h
/**
 * Check if this BitSet and that BitSet overlap.
 */
bool
BitSet_overlaps(const_BitSet this, const_BitSet that);
#endif//$XCC_h

bool
BitSet_overlaps(const_BitSet this, const_BitSet that)
{
  BitSetWord change = false;
  int32_t wordCount = BitSet_wordCount(this), i;
  Except_REQUIRE(BitSet_wordCount(this) == BitSet_wordCount(that));
  // Assume non used bits in the last word are all zero.
  for (i = 0; i < wordCount; i++) {
    BitSetWord old_word = BitSet_WORDS(this)[i];
    BitSetWord new_word = old_word & BitSet_WORDS(that)[i];
    change |= new_word;
  }
  return change != 0;
}

#ifdef $XCC_h
/**
 * Remove that BitSet members from this BitSet.
 */
bool
BitSet_diff(BitSet this, const_BitSet that);
#endif//$XCC_h

bool
BitSet_diff(BitSet this, const_BitSet that)
{
  BitSetWord change = false;
  int32_t wordCount = BitSet_wordCount(this), i;
  Except_REQUIRE(BitSet_wordCount(this) == BitSet_wordCount(that));
  // Assume non used bits in the last word are all zero.
  for (i = 0; i < wordCount; i++) {
    BitSetWord old_word = BitSet_WORDS(this)[i];
    BitSetWord new_word = old_word & ~BitSet_WORDS(that)[i];
    BitSet_WORDS(this)[i] = new_word;
    change |= new_word ^ old_word;
  }
  return change != 0;
}

#ifdef $XCC_h
/**
 * Return first BitSet member of the difference of BitSet(s).
 */
BitSetMember
BitSet_diffFirst(const_BitSet this, const_BitSet that);
#endif//$XCC_h

BitSetMember
BitSet_diffFirst(const_BitSet this, const_BitSet that)
{
  int32_t wordCount = BitSet_wordCount(this), i, j;
  Except_REQUIRE(BitSet_wordCount(this) == BitSet_wordCount(that));
  // Assume non used bits in the last word are all zero.
  for (i = 0; i < wordCount; i++) {
    BitSetWord word = BitSet_WORDS(this)[i] & ~BitSet_WORDS(that)[i];
    for (j = 0; word != 0; j++, word >>= 1) {
      if (word & 1) {
        return i*BitSet_WORDBITS + j;
      }
    }
  }
  return (BitSetMember)-1;
}

#ifdef $XCC_h
/**
 * True if the difference of this Bitset and that BitSet overlaps some BitSet.
 */
bool
BitSet_diffOverlaps(const_BitSet this, const_BitSet that, const_BitSet some);
#endif//$XCC_h

bool
BitSet_diffOverlaps(const_BitSet this, const_BitSet that, const_BitSet some)
{
  BitSetWord change = false;
  int32_t wordCount = BitSet_wordCount(this), i;
  Except_REQUIRE(BitSet_wordCount(this) == BitSet_wordCount(that));
  Except_REQUIRE(BitSet_wordCount(this) == BitSet_wordCount(some));
  // Assume non used bits in the last word are all zero.
  for (i = 0; i < wordCount; i++) {
    BitSetWord old_word = BitSet_WORDS(this)[i];
    BitSetWord new_word = old_word & ~BitSet_WORDS(that)[i];
    change |= new_word & BitSet_WORDS(some)[i];
  }
  return change != 0;
}

#ifdef $XCC_h
/**
 * Iterates over the BitSet entries.
 * this:	This BitSet.
 */
#define BitSet_FOREACH(this, iter) { \
  BitSetMember iter = 0; \
  int32_t BitSet_COUNT = BitSet_wordCount(this), BitSet_I, BitSet_J; \
  for (BitSet_I = 0; BitSet_I < BitSet_COUNT; BitSet_I++) { \
    BitSetWord BitSet_WORD = BitSet_words(this)[BitSet_I]; \
    if (BitSet_WORD == 0) { \
      iter += BitSet_WORDBITS; \
    } else { \
      for (BitSet_J = 0; BitSet_J < BitSet_WORDBITS; BitSet_J++, BitSet_WORD >>= 1, iter++) { \
        if (BitSet_WORD & 1) {
#define BitSet_ENDEACH \
        } \
      } \
      if (BitSet_J != BitSet_WORDBITS) break; \
    } \
  } \
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int max = 100, i;
  BitSet_ sieve[1];
  BitSet_ primes[1];
  BitSet_Ctor(sieve, Memory_Root, max);
  BitSet_Ctor(primes, Memory_Root, max);
  for (i = 2; i < max/2; i++) BitSet_insert(sieve, (BitSetMember)i);
  for (i = max - 1; i >= max/2; i--) BitSet_insert(sieve, (BitSetMember)i);
  while (!BitSet_isEmpty(sieve)) {
    int j = BitSet_choose(sieve);
    printf("prime %d\n", j);
    BitSet_insert(primes, (BitSetMember)j);
    for (i = j + j; i < max; i += j) BitSet_remove(sieve, (BitSetMember)i);
    printf("sieve: ");
    BitSet_FOREACH(sieve, iter) {
      printf("%d ", iter);
    } BitSet_ENDEACH;
    printf("\n");
  }
  printf("primes: ");
  BitSet_FOREACH(primes, iter) {
    printf("%d ", iter);
  } BitSet_ENDEACH;
  printf("\n");
  printf("primes2: ");
  i = (BitSetMember)-1;
  while ((i = BitSet_nextMember(primes, i)) >= 0) {
    printf("%d ", i);
  }
  printf("\n");
  BitSet_Dtor(primes);
  BitSet_Dtor(sieve);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the BitSet module.
 */
#define BitSet_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the BitSet module.
 */
#define BitSet_FINI()
#endif//$XCC__h

#if XCC__C
static void
BitSet_TEST(void)
{
#include "CCL/BitSet_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(BitSet_TEST);
  return 0;
}
#endif

