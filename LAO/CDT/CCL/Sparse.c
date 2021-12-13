#ifdef $XCC_h
/*
 * !!!!	Sparse.xcc
 *
 * Nikola Puzovic (Nikola.Puzovic@st.com)
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2007 STMicroelectronics.
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
 * @brief Integer set implement at sparse set.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/Sparse.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef uint32_t SparseMember;
#endif//$XCC_h

#ifdef $XCC_h
struct SparseEntry_ {
  uint32_t POSITION;		// Called SPARSE in Briggs & Torczon.
  SparseMember MEMBER;		// Called DENSE in Briggs & Torczon.
};
#endif//$XCC_h

#ifdef $XCC__c
{
  // Check it is safe to use Memory_alloc_ on sizeof(SparseEntry_).
  Except_ALWAYS(_ALIGNED(sizeof(SparseEntry_)));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Unsigned integer set implemented as sparse set (Briggs & Torczon).
 */
struct Sparse_ {
  //@args	Memory memory, int32_t universe
  Memory MEMORY;
  int32_t UNIVERSE;
  int32_t COUNT;
  struct SparseEntry_ *ENTRIES;
};
#endif//$XCC_h

Sparse
Sparse_Ctor(Sparse this, Memory memory, int32_t universe)
{
  size_t allocSize = universe*sizeof(SparseEntry_);
  void *entries = Memory_alloc(memory, allocSize);
  void *s;
  *Sparse__MEMORY(this) = memory;
  *Sparse__UNIVERSE(this) = universe;
  *Sparse__COUNT(this) = 0;
  *Sparse__ENTRIES(this) = entries;
  s = memset(entries, -1, allocSize);
  Except_CHECK(!entries || s);
  return this;
}

void
Sparse_Dtor(Sparse this)
{
  Memory memory = Sparse_MEMORY(this);
  Memory_free(memory, Sparse_ENTRIES(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Sparse_)\t%zu\n", sizeof(Sparse_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Empty this Sparse
 */
void
Sparse_empty(Sparse this);
#endif//$XCC_h

void
Sparse_empty(Sparse this)
{
  *Sparse__COUNT(this) = 0;
  Except_CHECK(Sparse_isEmpty(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * True iff this Sparse is empty.
 */
static inline bool
Sparse_isEmpty(const_Sparse this)
{
  return Sparse_COUNT(this) == 0;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this Sparse contains a single member.
 */
#define Sparse_isSingle(this)	(Sparse_count(this) == 1)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count members contained in the Sparse set.
 *
 * @return The count of members in this Sparse set.
 */
static inline int32_t
Sparse_count(const_Sparse this)
{
  return Sparse_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test a member for containment.
 *
 * @return True if this Sparse contains member.
 */
bool
Sparse_contains(const_Sparse this, SparseMember member);
#endif//$XCC_h

bool
Sparse_contains(const_Sparse this, SparseMember member)
{
  int32_t count = Sparse_COUNT(this);
  SparseEntry entries = Sparse_ENTRIES(this);
  uint32_t position = SparseEntry_POSITION(entries + member);
  Except_CHECK(member < Sparse_UNIVERSE(this));
  return position < count
      && SparseEntry_MEMBER(entries + position) == member;
}

#ifdef $XCC_h
/**
 * Insert a member in this Sparse set.
 *
 * @return False iff member was already contained in this Sparse set.
 */
bool
Sparse_insert(Sparse this, SparseMember member);
#endif//$XCC_h

bool
Sparse_insert(Sparse this, SparseMember member)
{
  int32_t count = Sparse_COUNT(this);
  SparseEntry entries = Sparse_ENTRIES(this);
  uint32_t position = SparseEntry_POSITION(entries + member);
  Except_CHECK(member < Sparse_UNIVERSE(this));
  if (   position < count
      && SparseEntry_MEMBER(entries + position) == member) return false;
  *SparseEntry__POSITION(entries + member) = count;
  *SparseEntry__MEMBER(entries + count) = member;
  ++*Sparse__COUNT(this);
  return true;
}

#ifdef $XCC_h
/**
 * Remove a member from this Sparse set.
 *
 * @return False iff member was not contained in this Sparse set.
 */
bool
Sparse_remove(Sparse this, SparseMember member);
#endif//$XCC_h

bool
Sparse_remove(Sparse this, SparseMember member)
{
  int32_t count = Sparse_COUNT(this);
  SparseEntry entries = Sparse_ENTRIES(this);
  uint32_t position = SparseEntry_POSITION(entries + member);
  Except_CHECK(member < Sparse_UNIVERSE(this));
  if (   position < count
      && SparseEntry_MEMBER(entries + position) == member) {
    member = SparseEntry_MEMBER(entries + count - 1);
    *SparseEntry__POSITION(entries + member) = position;
    *SparseEntry__MEMBER(entries + position) = member;
    --*Sparse__COUNT(this);
    return true;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Choose and remove a member of the Sparse set.
 */
SparseMember
Sparse_choose(Sparse this);
#endif//$XCC_h

SparseMember
Sparse_choose(Sparse this)
{
  int32_t count = Sparse_COUNT(this);
  if (count > 0) {
    SparseEntry entries = Sparse_ENTRIES(this);
    SparseMember member = SparseEntry_MEMBER(entries + count - 1);
    --*Sparse__COUNT(this);
    return member;
  }
  return (SparseMember)-1;
}

#ifdef $XCC_h
/**
 * Test for Sparse equality.
 */
bool
Sparse_equals(const_Sparse this, const_Sparse that);
#endif//$XCC_h

bool
Sparse_equals(const_Sparse this, const_Sparse that)
{
  if (Sparse_COUNT(this) != Sparse_COUNT(that)) return false;
  Sparse_FOREACH(this, member) {
    if (!Sparse_contains(that, member)) return false;
  } Sparse_ENDEACH;
  return true;
}

#ifdef $XCC_h
/**
 * Union of this Sparse with that Sparse set.
 */
bool
Sparse_union(Sparse this, const_Sparse that);
#endif//$XCC_h

bool
Sparse_union(Sparse this, const_Sparse that)
{
  bool change = false;
  Except_CHECK(Sparse_COUNT(this) == Sparse_COUNT(that));
  Sparse_FOREACH(that, element) {
    if (Sparse_insert(this, element)) change = true;
  } Sparse_ENDEACH;
  return change;
}

#ifdef $XCC_h
/**
 * Intersect this Sparse with that Sparse set.
 */
bool
Sparse_inter(Sparse this, const_Sparse that);
#endif//$XCC_h

bool
Sparse_inter(Sparse this, const_Sparse that)
{
  bool change = false;
  Sparse_ new[1];
  Except_CHECK(Sparse_COUNT(this) == Sparse_COUNT(that));
  Sparse_Ctor(new, Sparse_MEMORY(this), Sparse_COUNT(this));
  Sparse_FOREACH(this, member) {
    if (!Sparse_contains(that, member)) Sparse_insert(new, member);
  } Sparse_ENDEACH;
  change = Sparse_COUNT(new) != 0;
  Sparse_FOREACH(new, remove) {
    Sparse_remove(this, remove);
  } Sparse_ENDEACH;
  Sparse_Dtor(new);
  return change;
}

#ifdef $XCC_h
/**
 * Remove that Sparse members from this Sparse set.
 */
bool
Sparse_diff(Sparse this, const_Sparse that);
#endif//$XCC_h

bool
Sparse_diff(Sparse this, const_Sparse that)
{
  bool change = false;
  Sparse_FOREACH(that, element) {
    if (Sparse_remove(this, element)) change = true;
  } Sparse_ENDEACH;
  return change;
}

#ifdef $XCC_h
/**
 * Return the position of a member in this Sparse set.
 *
 * Useful to get a dense index for the member.
 */
static inline uint32_t
Sparse_position(const_Sparse this, SparseMember member)
{
  SparseEntry entries = Sparse_ENTRIES(this);
  return SparseEntry_POSITION(entries + member);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by Sparse_FOREACH.
 */
static inline const struct SparseEntry_ *
Sparse_entries(const_Sparse this)
{
  return Sparse_ENTRIES(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates over the Sparse entries.
 */
#define Sparse_FOREACH(this, iter) { \
  const struct SparseEntry_ *Sparse_ENTRIES = Sparse_entries(this); \
  int32_t Sparse_COUNT = Sparse_count(this), Sparse_POSITION = 0; \
  for (; Sparse_POSITION < Sparse_COUNT; Sparse_POSITION++) { \
    const_SparseEntry Sparse_ENTRY = Sparse_ENTRIES + Sparse_POSITION; \
    SparseMember iter = SparseEntry_MEMBER(Sparse_ENTRY);
#define Sparse_ENDEACH \
  } \
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int max = 100, i;
  Sparse_ sieve[1];
  Sparse_ primes[1];
  Sparse_Ctor(sieve, Memory_Root, max);
  Sparse_Ctor(primes, Memory_Root, max);
  for (i = max - 1; i >= 2; i--) Sparse_insert(sieve, (SparseMember)i);
//  for (i = 2; i < max/2; i++) Sparse_insert(sieve, (SparseMember)i);
//  for (i = max - 1; i >= max/2; i--) Sparse_insert(sieve, (SparseMember)i);
  while (!Sparse_isEmpty(sieve)) {
    int j = Sparse_choose(sieve);
    printf("prime %d\n", j);
    Sparse_insert(primes, (SparseMember)j);
    for (i = j + j; i < max; i += j) Sparse_remove(sieve, (SparseMember)i);
    printf("sieve: ");
    Sparse_FOREACH(sieve, iter) {
      printf("%d ", iter);
    } Sparse_ENDEACH;
    printf("\n");
  }
  printf("primes: ");
  Sparse_FOREACH(primes, iter) {
    printf("%d ", iter);
  } Sparse_ENDEACH;
  printf("\n");
  Sparse_Dtor(primes);
  Sparse_Dtor(sieve);
}
#endif//$XCC__c

#ifdef $XCC__c
{
#if 0
  int max = 100, i;
  Sparse_ set[1];
  Sparse_Ctor(set, Memory_Root, max);
  for (i = 0; i < 10; i++)
    if (Sparse_contains(set, i))
      printf("ERROR : Empty set contains number %d\n", i);
  for (i = 0; i < 10; i++) {
    if (!Sparse_insert(set, i))
      printf("ERROR : Can't insert number %d in to the set\n", i);
    else {
      printf ("Inserting number %d ; Set [size=%d] : ", i, Sparse_COUNT(set));
      Sparse_pretty(set, stdout);
      printf ("\n");
    }
  }
  for (i = 9; i >= 0; i--) {
    if (!Sparse_remove(set, i))
      printf("ERROR : Can't remove number %d from the set\n", i);
    else {
      printf ("Removing number %d ; Set [size=%d] : ", i, Sparse_COUNT(set));
      Sparse_pretty(set, stdout);
      printf ("\n");
    }
  }
  Sparse_Dtor(set);
#endif
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Pretty print members of the sparse set.
 */
bool
Sparse_pretty(Sparse this, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
bool
Sparse_pretty(Sparse this, FILE *file)
{
  if (file == NULL) file = stderr;
  Sparse_FOREACH(this, member) {
    fprintf (file, "%d ", member);
  } Sparse_ENDEACH;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Sparse module.
 */
#define Sparse_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Sparse module.
 */
#define Sparse_FINI()
#endif//$XCC__h

#if XCC__C
static void
Sparse_TEST(void)
{
#include "CCL/Sparse_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(Sparse_TEST);
  return 0;
}
#endif

