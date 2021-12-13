#ifdef $XCC_h
/*
 * !!!!	PtrSet.xcc
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
 * @brief Pointer set implement as dense sorted array.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/PtrSet.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef void *PtrSetMember;
typedef const void *const_PtrSetMember;
typedef int (*PtrSetMemberCompare)(const_PtrSetMember, const_PtrSetMember);
#endif//$XCC_h

#ifdef $XCC_h
#define PtrSetMember_GT(a, b) ((uintptr_t)(a) > (uintptr_t)(b))
#define PtrSetMember_LT(a, b) ((uintptr_t)(a) < (uintptr_t)(b))
#define PtrSetMember_EQ(a, b) ((a) == (b))
#define PtrSetMember_NE(a, b) ((a) != (b))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pointer Set implemented as an array decreasing numeric order.
 */
struct PtrSet_ {
  //@args	Memory memory, int32_t maxCount
  Memory MEMORY;		// Where the PtrSet members are allocated.
  uint32_t STATUS;		// MAXCOUNT:31;ISORDERED:1;
  //@access ISORDERED	(PtrSet_STATUS(this)&0x1)
  //@access MAXCOUNT	(int32_t)(PtrSet_STATUS(this)>>1)
  //@access MAXSIZE	(PtrSet_MAXCOUNT(this)*sizeof(PtrSetMember))
  PtrSetMember *BASE;
  PtrSetMember *PAST;
  //@access USEDSIZE	((char*)PtrSet_PAST(this) - (char*)PtrSet_BASE(this))
};
#endif//$XCC_h

PtrSet
PtrSet_Ctor(PtrSet this, Memory memory, int32_t maxCount)
{
  *PtrSet__MEMORY(this) = memory;
  *PtrSet__STATUS(this) = 0x1;
  *PtrSet__BASE(this) = NULL;
  *PtrSet__PAST(this) = NULL;
  if (maxCount > 0) PtrSet_resize(this, maxCount);
  return this;
}

void
PtrSet_Dtor(PtrSet this)
{
  Memory memory = PtrSet_MEMORY(this);
  PtrSet_empty(this);
  Memory_free(memory, PtrSet_BASE(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(PtrSet_)\t%zu\n", sizeof(PtrSet_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Resize this PtrSet.
 *
 * maxCount:	The new @c maxCount.
 */
void
PtrSet_resize(PtrSet this, int32_t maxCount);
#endif//$XCC_h

void
PtrSet_resize(PtrSet this, int32_t maxCount)
{
  Memory memory = PtrSet_MEMORY(this);
  size_t memberSize = sizeof(PtrSetMember);
  bool old_isOrdered = PtrSet_ISORDERED(this);
  int32_t old_maxCount = PtrSet_MAXCOUNT(this);
  if (maxCount > old_maxCount) {
    size_t maxSize = maxCount*memberSize;
    size_t alignedSize = _ALIGN_NEXT(maxSize);
    char *new_base = Memory_alloc_(memory, alignedSize);
    size_t old_usedSize = PtrSet_USEDSIZE(this);
    if (old_usedSize != 0) {
      char *old_base = (char *)PtrSet_BASE(this);
      memcpy(new_base, old_base, old_usedSize);
      Memory_free_(memory, old_base);
    }
    *PtrSet__PAST(this) = (PtrSetMember *)(new_base + old_usedSize);
    *PtrSet__BASE(this) = (PtrSetMember *)(new_base);
    while ((alignedSize -= memberSize) >= maxSize) ++maxCount;
    *PtrSet__STATUS(this) = ((maxCount<<1)|old_isOrdered);
  }
  Except_ENSURE(PtrSet_PAST(this) >= PtrSet_BASE(this));
  Except_ENSURE(PtrSet_USEDSIZE(this) <= PtrSet_MAXSIZE(this));
}

#ifdef $XCC_h
/**
 * Empty this PtrSet.
 */
void
PtrSet_empty(PtrSet this);
#endif//$XCC_h

void
PtrSet_empty(PtrSet this)
{
  *PtrSet__PAST(this) = PtrSet_BASE(this);
  Except_CHECK(PtrSet_isEmpty(this));
}

#ifdef $XCC_h
/**
 * For use by PtrSet_FOREACH.
 */
static inline const PtrSetMember *
PtrSet_base(const_PtrSet this)
{
  return (const PtrSetMember *)PtrSet_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by PtrSet_FOREACH.
 */
static inline const PtrSetMember *
PtrSet_past(const_PtrSet this)
{
  return (const PtrSetMember *)PtrSet_PAST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/*
  PtrSet_FOREACH --	Iterates over the PtrSet members.
 */
#define PtrSet_FOREACH(this, Type, member) { \
  const PtrSetMember *PtrSet_BASE = PtrSet_base(this); \
  const PtrSetMember *PtrSet_PAST = PtrSet_past(this); \
  if (PtrSet_BASE != PtrSet_PAST) { \
    const PtrSetMember *(PtrSet_ITER) = PtrSet_PAST - 1; \
    for (; PtrSet_ITER >= PtrSet_BASE; --(PtrSet_ITER)) { \
      Type member = (Type)*(PtrSet_ITER);
#define PtrSet_ENDEACH \
    } \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/*
  PtrSet_FORSORT --	Iterates over the sorted PtrSet members.
 */
#define PtrSet_FORSORT(this, compare, Type, member) { \
  PtrSetMemberCompare PtrSet_COMPARE = (PtrSetMemberCompare)(compare); \
  uint32_t PtrSet_COUNT = PtrSet_count(this), PtrSet_I = 0; \
  const PtrSetMember *PtrSet_BASE = PtrSet_base(this); \
  const PtrSetMember *PtrSet_PAST = PtrSet_past(this), *(PtrSet_ITER); \
  PtrSetMember *PtrSet_ARRAY = alloca(sizeof(PtrSetMember)*PtrSet_COUNT); \
  for (PtrSet_I = 0, PtrSet_ITER = PtrSet_BASE; \
       PtrSet_ITER < PtrSet_PAST; ++(PtrSet_ITER), ++PtrSet_I) \
    PtrSet_ARRAY[PtrSet_I] = *(PtrSet_ITER); \
  HackerPtr_SortArray(PtrSet_ARRAY, 0, PtrSet_COUNT - 1, PtrSet_COMPARE); \
  for (PtrSet_I = 0; PtrSet_I < PtrSet_COUNT; PtrSet_I++) { \
    Type member = (Type)PtrSet_ARRAY[PtrSet_I];
#define PtrSet_ENDSORT \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * True iff this PtrSet is full.
 */
static inline bool
PtrSet_isFull(const_PtrSet this)
{
  int32_t maxCount = PtrSet_MAXCOUNT(this);
  PtrSetMember *base = PtrSet_BASE(this);
  PtrSetMember *past = PtrSet_PAST(this);
  return past - base == maxCount;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check order of this PtrSet.
 */
bool
PtrSet_checkOrder(const_PtrSet this);
#endif//$XCC_h

bool
PtrSet_checkOrder(const_PtrSet this)
{
  int32_t count = PtrSet_count(this), i = 0;
  PtrSetMember *base = PtrSet_BASE(this);
  if (base != NULL) {
    for (i = 1; i < count; i++) {
      if (!PtrSetMember_GT(base[i - 1], base[i])) return false;
    }
  }
  return true;
}

#ifdef $XCC_h
/**
 * Push as last member on the PtrSet, resizing the PtrSet if necessary.
 *
 * As the PtrSet may be resized, it is an error to maintain pointers to existing items.
 */
void
PtrSet_pushMember(PtrSet this, PtrSetMember member);
#endif//$XCC_h

#ifndef PtrSet_MAGICCOUNT
#define PtrSet_MAGICCOUNT 16
#endif//PtrSet_MAGICCOUNT

void
PtrSet_pushMember(PtrSet this, PtrSetMember member)
{
  int32_t maxCount = PtrSet_MAXCOUNT(this);
  PtrSetMember *base = PtrSet_BASE(this);
  PtrSetMember *past = PtrSet_PAST(this);
  int32_t count = past - base;
  if (count == maxCount) {
    int32_t newMaxCount = count*2;
    if (newMaxCount < PtrSet_MAGICCOUNT) {
      newMaxCount = PtrSet_MAGICCOUNT;
    }
    PtrSet_resize(this, newMaxCount);
    past = PtrSet_PAST(this);
    maxCount = PtrSet_MAXCOUNT(this);
    Except_CHECK(!PtrSet_isFull(this));
  }
  *past = member;
  *PtrSet__PAST(this) = past + 1;
  if (count && !PtrSetMember_LT(past[-1], member)) {
    *PtrSet__STATUS(this) = (maxCount<<1)|0x1;
  }
}

#ifdef $XCC_h
/**
 * Reorder this PtrSet members after calls to PtrSet_pushMember.
 *
 * Use the en.wikibook.org heapsort, then call PtrSet_uniq.
 */
int32_t
PtrSet_makeOrder(PtrSet this);
#endif//$XCC_h

#if 0
int32_t
PtrSet_sort(PtrSet this)
{
  PtrSetMember *members = PtrSet_BASE(this);
  PtrSetMember *past = PtrSet_PAST(this);
  int32_t count = past - members;
  int32_t n = count, i = count/2;
  while (true) {
    int32_t parent, child;
    PtrSetMember temp_member;
    if (i > 0) {
      temp_member = members[--i];
    } else if (n > 1) {
      temp_member = members[--n];
      members[n] = members[0];
    } else break;
    parent = i, child = i*2 + 1;
    while (child < n) {
      if (   child + 1 < n
          && PtrSetMember_LT(members[child + 1], members[child])) {
        child = child + 1;
      }
      if (PtrSetMember_LT(members[child], temp_member)) {
        members[parent] = members[child];
        parent = child;
        child = child*2 + 1;
      } else break;
    }
    members[parent] = temp_member;
  }
  *PtrSet__STATUS(this) |= 0x1;
  return PtrSet_uniq(this);
}
#else
int32_t
PtrSet_makeOrder(PtrSet this)
{
  PtrSetMember *base = PtrSet_BASE(this);
  PtrSetMember *past = PtrSet_PAST(this);
  PtrSetMember *members = past - 1;
  int32_t count = past - base;
  int32_t n = -count, i = -(count/2), d = 0;
  while (i++ < 0) {
    int32_t parent = i, child = i*2 - 1;
    PtrSetMember temp_member = members[i];
    while (child > n) {
      if (   child - 1 > n
          && PtrSetMember_GT(members[child - 1], members[child])) {
        child = child - 1;
      }
      if (PtrSetMember_GT(members[child], temp_member)) {
        members[parent] = members[child];
        parent = child;
        child = child*2 - 1;
      } else break;
    }
    members[parent] = temp_member;
  }
  while (n++ < -1) {
    int32_t parent = 0, child = -1;
    PtrSetMember temp_member = members[n];
    members[n] = members[0];
    while (child > n) {
      if (   child - 1 > n
          && PtrSetMember_GT(members[child - 1], members[child])) {
        child = child - 1;
      }
      if (PtrSetMember_GT(members[child], temp_member)) {
        members[parent] = members[child];
        parent = child;
        child = child*2 - 1;
      } else break;
    }
    members[parent] = temp_member;
  }
  // This PtrSet is ordered now.
  *PtrSet__STATUS(this) |= 0x1;
  return PtrSet_uniq(this);
}
#endif

#ifdef $XCC__h
int32_t
PtrSet_uniq(PtrSet this);
#endif//$XCC__h

int32_t
PtrSet_uniq(PtrSet this)
{
  PtrSetMember *base = PtrSet_BASE(this);
  PtrSetMember *past = PtrSet_PAST(this);
  int32_t count = past - base, d = 0, i;
  if (base != NULL) {
    for (i = 0; i + d + 1 < count;) {
      base[i] = base[i + d];
      if (PtrSetMember_EQ(base[i + d], base[i + d + 1])) d++;
      else i++;
    }
    base[i] = base[i + d];
    *PtrSet__PAST(this) = base + count - d;
  }
  Except_DEBUG(PtrSet_ISORDERED(this) && PtrSet_checkOrder(this));
  Except_CHECK(PtrSet_count(this) == count - d);
  return count - d;
}

#ifdef $XCC__c
{
  PtrSet_ this_set[1];
  long int tosort[] = {
    4, 45, 67, 4, 2, 2, 3, 4, 6, 8,
    9, 9, 7, 5, 4, 32, 3, 4, 5, 6, 6,
    4, 3, 2, 3, 5, 7, 8, 9, 8, 7, 6, 54
  };
  int count = sizeof(tosort)/sizeof(*tosort), i;
  PtrSet_Ctor(this_set, Memory_Root, 4);
  for (i = 0; i < count; i++) {
    PtrSet_pushMember(this_set, (PtrSetMember)tosort[count - 1 - i]);
  }
  printf("unsorted = { ");
  PtrSet_FOREACH(this_set, intptr_t, member) {
    printf("%d ", (int)member);
  } PtrSet_ENDEACH;
  printf("}\n");
  PtrSet_makeOrder(this_set);
  printf("restored = { ");
  PtrSet_FOREACH(this_set, intptr_t, member) {
    printf("%d ", (int)member);
  } PtrSet_ENDEACH;
  printf("}\n");
  {
    PtrSetMember *base = PtrSet_BASE(this_set);
    int32_t count = PtrSet_count(this_set), i;
    printf("dumped = { ");
    for (i = 0; i < count; i++) {
      printf("%ld ", (long int)base[i]);
    }
    printf("}\n");
  }
  PtrSet_Dtor(this_set);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Function used to merge sort a PtrSetMember array.
 */
void
PtrSet_SortArray(PtrSetMember array[], int32_t first, int32_t last);
#endif//$XCC_h

void
PtrSet_SortArray(PtrSetMember array[], int32_t first, int32_t last)
{
  int32_t count = last - first + 1;
  int32_t mid = (first + last) >> 1;
  int32_t idx1 = first, idx2 = mid + 1, i;
  if (idx1 < mid) PtrSet_SortArray(array, idx1, mid);
  if (idx2 < last) PtrSet_SortArray(array, idx2, last);
  // Merge step.
  if (count > 1) {
    PtrSetMember *tempArray = alloca(sizeof(PtrSetMember)*count);
    for (i = 0; i < count; i++) {
      if (idx1 > mid) {
        tempArray[i] = array[idx2++];
      } else if (idx2 > last) {
        tempArray[i] = array[idx1++];
      } else if (!PtrSetMember_LT(array[idx1], array[idx2])) {
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
  long int tosort[] = {
    4, 45, 67, 4, 2, 2, 3, 4, 6, 8,
    9, 9, 7, 5, 4, 32, 3, 4, 5, 6, 6,
    4, 3, 2, 3, 5, 7, 8, 9, 8, 7, 6, 54
  };
  PtrSetMember array[sizeof(tosort)/sizeof(*tosort)];
  int count = sizeof(tosort)/sizeof(*tosort), i;
  printf("array = { ");
  for (i = 0; i < count; i++) {
    array[i] = (PtrSetMember)tosort[i];
    printf("%ld ", (long int)array[i]);
  }
  printf("}\n");
  PtrSet_SortArray(array, 0, count - 1);
  printf("sorted = { ");
  for (i = 0; i < count; i++) {
    printf("%ld ", (long int)array[i]);
  }
  printf("}\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This PtrSet memory.
 */
static inline Memory
PtrSet_memory(const_PtrSet this)
{
  return PtrSet_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this PtrSet is empty.
 */
static inline bool
PtrSet_isEmpty(const_PtrSet this)
{
  PtrSetMember *base = PtrSet_BASE(this);
  PtrSetMember *past = PtrSet_PAST(this);
  return past == base;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this PtrSet contains a single member.
 */
static inline bool
PtrSet_isSingle(const_PtrSet this)
{
  PtrSetMember *base = PtrSet_BASE(this);
  PtrSetMember *past = PtrSet_PAST(this);
  return past - base == 1;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count members contained in the PtrSet.
 *
 * Return:	The count of members in this PtrSet.
 */
static inline int32_t
PtrSet_count(const_PtrSet this)
{
  PtrSetMember *base = PtrSet_BASE(this);
  PtrSetMember *past = PtrSet_PAST(this);
  return past - base;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This PtrSet ordered status.
 */
static inline bool
PtrSet_isOrdered(const_PtrSet this)
{
  return PtrSet_ISORDERED(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this PtrSet status as unordered.
 */
static inline void
PtrSet_clearOrdered(PtrSet this)
{
  *PtrSet__STATUS(this) &= ~0x1;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this PtrSet status as ordered.
 */
static inline void
PtrSet_raiseOrdered(PtrSet this)
{
  *PtrSet__STATUS(this) |= 0x1;
  Except_CHECK(PtrSet_checkOrder(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Base of this PtrSet for use by external sort.
 */
static inline PtrSetMember *
PtrSet_base_(PtrSet this)
{
  *PtrSet__STATUS(this) &= ~0x1;
  return PtrSet_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test a member for containment.
 *
 * Return:	True if this PtrSet contains member.
 */
bool
PtrSet_contains(const_PtrSet this, PtrSetMember member);
#endif//$XCC_h

bool
PtrSet_contains(const_PtrSet this, PtrSetMember member)
{
  int32_t count = PtrSet_count(this);
  int32_t l = 0, r = count - 1, i = 0;
  PtrSetMember *base = PtrSet_BASE(this);
  Except_REQUIRE(PtrSet_isOrdered(this));
  while (r >= l) {
    i = (l + r) >> 1;
    if (PtrSetMember_LT(base[i], member)) r = i - 1;
    else if (PtrSetMember_GT(base[i],  member)) l = i + 1;
    else return true;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Insert a member in this PtrSet.
 *
 * Return:	False iff member was already contained in this PtrSet.
 */
bool
PtrSet_insert(PtrSet this, PtrSetMember member);
#endif//$XCC_h

bool
PtrSet_insert(PtrSet this, PtrSetMember member)
{
#if 0
  int32_t count = PtrSet_count(this);
  PtrSet_pushMember(this, member);
  PtrSet_makeOrder(this);
  return count == PtrSet_count(this);
#else
  if (PtrSet_isEmpty(this)) {
    PtrSet_pushMember(this, member);
  } else {
    PtrSetMember *base = PtrSet_BASE(this);
    PtrSetMember *past = PtrSet_PAST(this);
    int32_t count = past - base;
    int32_t l = 0, r = count - 1, i = 0, j;
    Except_REQUIRE(PtrSet_isOrdered(this));
    while (r >= l) {
      i = (l + r) >> 1;
      if (PtrSetMember_LT(base[i], member)) r = i - 1;
      else if (PtrSetMember_GT(base[i], member)) l = i + 1;
      else return false;
    }
    if (PtrSetMember_GT(base[i], member)) i++;
    if (count == PtrSet_MAXCOUNT(this)) {
      int32_t newMaxCount = PtrSet_MAGICCOUNT > count*2?
                            PtrSet_MAGICCOUNT: count*2;
      PtrSet_resize(this, newMaxCount);
      // Reload base because of the PtrSet_resize side-effects.
      base = PtrSet_BASE(this);
    }
    *PtrSet__PAST(this) = base + count + 1;
    for (j = count; j > i; j--) base[j] = base[j - 1];
    base[i] = member;
    Except_DEBUG(PtrSet_ISORDERED(this) && PtrSet_checkOrder(this));
  }
  return true;
#endif
}

#ifdef $XCC_h
/**
 * Remove a member from this PtrSet.
 *
 * Return:	False iff member was not contained in this PtrSet.
 */
bool
PtrSet_remove(PtrSet this, PtrSetMember member);
#endif//$XCC_h

bool
PtrSet_remove(PtrSet this, PtrSetMember member)
{
  PtrSetMember *base = PtrSet_BASE(this);
  PtrSetMember *past = PtrSet_PAST(this);
  int32_t count = past - base;
  int32_t l = 0, r = count - 1, i = 0;
  Except_REQUIRE(PtrSet_isOrdered(this));
  while (r >= l) {
    i = (l + r) >> 1;
    if (PtrSetMember_LT(base[i], member)) r = i - 1;
    else if (PtrSetMember_GT(base[i], member)) l = i + 1;
    else {
      for (; i < count - 1; i++) base[i] = base[i + 1];
      *PtrSet__PAST(this) = past - 1;
      Except_DEBUG(PtrSet_ISORDERED(this) && PtrSet_checkOrder(this));
      return true;
    }
  }
  Except_DEBUG(PtrSet_ISORDERED(this) && PtrSet_checkOrder(this));
  return false;
}

#ifdef $XCC_h
/**
 * Choose and remove a member of the PtrSet.
 */
static inline PtrSetMember
PtrSet_choose(PtrSet this)
{
  PtrSetMember *past = PtrSet_PAST(this);
  Except_REQUIRE(!PtrSet_isEmpty(this));
  Except_REQUIRE(PtrSet_isOrdered(this));
  *PtrSet__PAST(this) = past - 1;
  return *(PtrSetMember *)(past - 1);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test for PtrSet equality.
 */
bool
PtrSet_equals(const_PtrSet this, const_PtrSet that);
#endif//$XCC_h

bool
PtrSet_equals(const_PtrSet this, const_PtrSet that)
{
  int32_t this_count = PtrSet_count(this);
  int32_t that_count = PtrSet_count(that);
  PtrSetMember *this_base = PtrSet_BASE(this);
  PtrSetMember *that_base = PtrSet_BASE(that);
  Except_REQUIRE(PtrSet_isOrdered(this));
  Except_REQUIRE(PtrSet_isOrdered(that));
  if (this_count != that_count) return false;
  if (this_count == 0) return true;
  return (!memcmp(this_base, that_base, sizeof(PtrSetMember)*this_count));
}

#ifdef $XCC_h
/**
 * Assign the contents of that PtrSet to this PtrSet.
 */
void
PtrSet_assign(PtrSet this, const_PtrSet that);
#endif//$XCC_h

void
PtrSet_assign(PtrSet this, const_PtrSet that)
{
  Memory memory = PtrSet_MEMORY(this);
  int32_t maxCount = PtrSet_MAXCOUNT(this);
  PtrSetMember *this_base = PtrSet_BASE(this);
  PtrSetMember *that_base = PtrSet_BASE(that);
  int32_t that_count = PtrSet_count(that), i = 0;
  bool that_isOrdered = PtrSet_ISORDERED(that);
  if (maxCount < that_count) {
    size_t that_size = that_count*sizeof(PtrSetMember);
    size_t alignedSize = _ALIGN_NEXT(that_size);
    Memory_free(memory, this_base);
    this_base = Memory_alloc_(memory, alignedSize);
    *PtrSet__STATUS(this) = ((that_count<<1)|that_isOrdered);
    *PtrSet__BASE(this) = this_base;
  }
  for (i = 0; i < that_count; i++) {
    this_base[i] = that_base[i];
  }
  *PtrSet__PAST(this) = this_base + that_count;
}

#ifdef $XCC__c
{
  PtrSet_ this_set[1];
  PtrSet_ that_set[1];
  PtrSet_Ctor(this_set, Memory_Root, 4);
  PtrSet_Ctor(that_set, Memory_Root, 3);
  PtrSet_insert(this_set, (PtrSetMember)1);
  PtrSet_insert(this_set, (PtrSetMember)2);
  PtrSet_insert(this_set, (PtrSetMember)3);
  PtrSet_insert(this_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)3);
  PtrSet_insert(that_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)5);
  PtrSet_assign(that_set, this_set);
  printf("{ 3 4 5 } assign { 1 2 3 4 } = { ");
  PtrSet_FOREACH(that_set, intptr_t, member) {
    printf("%d ", (int)member);
  } PtrSet_ENDEACH;
  printf("}\n");
  PtrSet_Dtor(that_set);
  PtrSet_Dtor(this_set);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * True iff this PtrSet comprises that PtrSet.
 */
bool
PtrSet_comprises(const_PtrSet this, const_PtrSet that);
#endif//$XCC_h

bool
PtrSet_comprises(const_PtrSet this, const_PtrSet that)
{
  int32_t this_count = PtrSet_count(this);
  int32_t that_count = PtrSet_count(that);
  Except_REQUIRE(PtrSet_isOrdered(this));
  Except_REQUIRE(PtrSet_isOrdered(that));
  if (this_count >= that_count) {
    int32_t i = 0, j = 0, k = 0;
    const PtrSetMember *this_base = PtrSet_BASE(this);
    const PtrSetMember *that_base = PtrSet_BASE(that);
    while (i < this_count && j < that_count) {
      PtrSetMember this_member = this_base[i];
      PtrSetMember that_member = that_base[j];
      if (PtrSetMember_GT(this_member, that_member)) i++, k++;
      else if (PtrSetMember_LT(this_member, that_member)) j++, k++;
      else i++, j++, k++;
    }
    if (this_count - i > 0) k += this_count - i;
    if (that_count - j > 0) k += that_count - j;
    return k == this_count;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Union of this PtrSet with that PtrSet.
 */
bool
PtrSet_union(PtrSet this, const_PtrSet that);
#endif//$XCC_h

bool
PtrSet_union(PtrSet this, const_PtrSet that)
{
  Memory memory = PtrSet_MEMORY(this);
  int32_t this_count = PtrSet_count(this), i = 0;
  int32_t that_count = PtrSet_count(that), j = 0;
  int32_t temp_count = this_count + that_count, k = 0;
  size_t tempSize = temp_count*sizeof(PtrSetMember);
  size_t alignedSize = _ALIGN_NEXT(tempSize);
  PtrSetMember *this_base = PtrSet_BASE(this);
  PtrSetMember *that_base = PtrSet_BASE(that);
  PtrSetMember *temp_base = alloca(alignedSize);
  Except_REQUIRE(PtrSet_isOrdered(this));
  Except_REQUIRE(PtrSet_isOrdered(that));
  while (i < this_count && j < that_count) {
    PtrSetMember this_member = this_base[i];
    PtrSetMember that_member = that_base[j];
    if (PtrSetMember_GT(this_member, that_member)) {
      temp_base[k++] = this_member, i++;
    } else if (PtrSetMember_LT(this_member, that_member)) {
      temp_base[k++] = that_member, j++;
    } else temp_base[k++] = this_member, i++, j++;
  }
  while (i < this_count) temp_base[k++] = this_base[i++];
  while (j < that_count) temp_base[k++] = that_base[j++];
  if (k == this_count) {
    // This set is unchanged.
    return false;
  }
  if (k > PtrSet_MAXCOUNT(this)) {
    // Must free then allocate this_base and copy temp_base into it.
    Memory_free(memory, this_base);
    this_base = Memory_alloc_(memory, alignedSize);
    for (i = 0; i < k; i++) this_base[i] = temp_base[i];
    *PtrSet__STATUS(this) = ((temp_count<<1)|true);
    *PtrSet__BASE(this) = this_base;
    *PtrSet__PAST(this) = this_base + k;
  } else {
    // Here k <= maxCount, so just copy temp_base into this_base.
    for (i = 0; i < k; i++) this_base[i] = temp_base[i];
    *PtrSet__PAST(this) = this_base + k;
  }
  Except_DEBUG(PtrSet_ISORDERED(this) && PtrSet_checkOrder(this));
  return true;
}

#ifdef $XCC__c
{
  PtrSet_ this_set[1];
  PtrSet_ that_set[1];
  PtrSet_Ctor(this_set, Memory_Root, 4);
  PtrSet_Ctor(that_set, Memory_Root, 3);
  PtrSet_insert(this_set, (PtrSetMember)1);
  PtrSet_insert(this_set, (PtrSetMember)2);
  PtrSet_insert(this_set, (PtrSetMember)3);
  PtrSet_insert(this_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)3);
  PtrSet_insert(that_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)5);
  PtrSet_union(this_set, that_set);
  printf("{ 1 2 3 4 } union { 3 4 5 } = { ");
  PtrSet_FOREACH(this_set, intptr_t, member) {
    printf("%d ", (int)member);
  } PtrSet_ENDEACH;
  printf("}\n");
  PtrSet_Dtor(that_set);
  PtrSet_Dtor(this_set);
}
#endif//$XCC__c

#ifdef $XCC__c
{
  PtrSet_ this_set[1];
  PtrSet_ that_set[1];
  PtrSet_Ctor(this_set, Memory_Root, 4);
  PtrSet_Ctor(that_set, Memory_Root, 3);
  PtrSet_insert(this_set, (PtrSetMember)1);
  PtrSet_insert(this_set, (PtrSetMember)2);
  PtrSet_insert(this_set, (PtrSetMember)3);
  PtrSet_insert(this_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)3);
  PtrSet_insert(that_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)5);
  PtrSet_union(that_set, this_set);
  printf("{ 3 4 5 } union { 1 2 3 4 } = { ");
  PtrSet_FOREACH(that_set, intptr_t, member) {
    printf("%d ", (int)member);
  } PtrSet_ENDEACH;
  printf("}\n");
  PtrSet_Dtor(that_set);
  PtrSet_Dtor(this_set);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Intersect this PtrSet with that PtrSet.
 */
bool
PtrSet_inter(PtrSet this, const_PtrSet that);
#endif//$XCC_h

bool
PtrSet_inter(PtrSet this, const_PtrSet that)
{
  int32_t this_count = PtrSet_count(this), i = 0;
  int32_t that_count = PtrSet_count(that), j = 0, k = 0;
  PtrSetMember *this_base = PtrSet_BASE(this);
  PtrSetMember *that_base = PtrSet_BASE(that);
  PtrSetMember *temp_base = this_base;
  Except_REQUIRE(PtrSet_isOrdered(this));
  Except_REQUIRE(PtrSet_isOrdered(that));
  while (i < this_count && j < that_count) {
    PtrSetMember this_member = this_base[i];
    PtrSetMember that_member = that_base[j];
    if (PtrSetMember_GT(this_member, that_member)) i++;
    else if (PtrSetMember_LT(this_member, that_member)) j++;
    else temp_base[k++] = this_member, i++, j++;
  }
  if (k == this_count) {
    return false;
  } else if (k != 0) {
    *PtrSet__PAST(this) = this_base + k;
  } else {
    PtrSet_empty(this);
  }
  Except_DEBUG(PtrSet_ISORDERED(this) && PtrSet_checkOrder(this));
  return true;
}

#ifdef $XCC__c
{
  PtrSet_ this_set[1];
  PtrSet_ that_set[1];
  PtrSet_Ctor(this_set, Memory_Root, 4);
  PtrSet_Ctor(that_set, Memory_Root, 3);
  PtrSet_insert(this_set, (PtrSetMember)1);
  PtrSet_insert(this_set, (PtrSetMember)2);
  PtrSet_insert(this_set, (PtrSetMember)3);
  PtrSet_insert(this_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)3);
  PtrSet_insert(that_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)5);
  PtrSet_inter(this_set, that_set);
  printf("{ 1 2 3 4 } inter { 3 4 5 } = { ");
  PtrSet_FOREACH(this_set, intptr_t, member) {
    printf("%d ", (int)member);
  } PtrSet_ENDEACH;
  printf("}\n");
  PtrSet_Dtor(that_set);
  PtrSet_Dtor(this_set);
}
#endif//$XCC__c

#ifdef $XCC__c
{
  PtrSet_ this_set[1];
  PtrSet_ that_set[1];
  PtrSet_Ctor(this_set, Memory_Root, 4);
  PtrSet_Ctor(that_set, Memory_Root, 3);
  PtrSet_insert(this_set, (PtrSetMember)1);
  PtrSet_insert(this_set, (PtrSetMember)2);
  PtrSet_insert(this_set, (PtrSetMember)3);
  PtrSet_insert(this_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)3);
  PtrSet_insert(that_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)5);
  PtrSet_inter(that_set, this_set);
  printf("{ 3 4 5 } inter { 1 2 3 4 } = { ");
  PtrSet_FOREACH(that_set, intptr_t, member) {
    printf("%d ", (int)member);
  } PtrSet_ENDEACH;
  printf("}\n");
  PtrSet_Dtor(that_set);
  PtrSet_Dtor(this_set);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Check if this PtrSet and that PtrSet overlap.
 */
bool
PtrSet_overlaps(const_PtrSet this, const_PtrSet that);
#endif//$XCC_h

bool
PtrSet_overlaps(const_PtrSet this, const_PtrSet that)
{
  int32_t this_count = PtrSet_count(this), i = 0;
  int32_t that_count = PtrSet_count(that), j = 0;
  PtrSetMember *this_base = PtrSet_BASE(this);
  PtrSetMember *that_base = PtrSet_BASE(that);
  Except_REQUIRE(PtrSet_isOrdered(this));
  Except_REQUIRE(PtrSet_isOrdered(that));
  while (i < this_count && j < that_count) {
    PtrSetMember this_member = this_base[i];
    PtrSetMember that_member = that_base[j];
    if (PtrSetMember_GT(this_member, that_member)) i++;
    else if (PtrSetMember_LT(this_member, that_member)) j++;
    else return true;
  }
  return false;
}

#ifdef $XCC__c
{
  PtrSet_ this_set[1];
  PtrSet_ that_set[1];
  PtrSet_Ctor(this_set, Memory_Root, 4);
  PtrSet_Ctor(that_set, Memory_Root, 3);
  PtrSet_insert(this_set, (PtrSetMember)1);
  PtrSet_insert(this_set, (PtrSetMember)2);
  PtrSet_insert(this_set, (PtrSetMember)3);
  PtrSet_insert(this_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)3);
  PtrSet_insert(that_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)5);
  printf("{ 3 4 5 } overlap { 1 2 3 4 } = %d\n",
         PtrSet_overlaps(this_set, that_set));
  PtrSet_remove(that_set, (PtrSetMember)3);
  printf("{ 4 5 } overlap { 1 2 3 4 } = %d\n",
         PtrSet_overlaps(this_set, that_set));
  PtrSet_remove(this_set, (PtrSetMember)4);
  printf("{ 4 5 } overlap { 1 2 3 } = %d\n",
         PtrSet_overlaps(this_set, that_set));
  PtrSet_Dtor(that_set);
  PtrSet_Dtor(this_set);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Remove that PtrSet members from this PtrSet.
 */
bool
PtrSet_diff(PtrSet this, const_PtrSet that);
#endif//$XCC_h

bool
PtrSet_diff(PtrSet this, const_PtrSet that)
{
  int32_t this_count = PtrSet_count(this), i = 0;
  int32_t that_count = PtrSet_count(that), j = 0, k = 0;
  PtrSetMember *this_base = PtrSet_BASE(this);
  PtrSetMember *that_base = PtrSet_BASE(that);
  PtrSetMember *temp_base = this_base;
  Except_REQUIRE(PtrSet_isOrdered(this));
  Except_REQUIRE(PtrSet_isOrdered(that));
  while (i < this_count && j < that_count) {
    if (PtrSetMember_GT(this_base[i], that_base[j]))
      temp_base[k++] = this_base[i++];
    else if (PtrSetMember_LT(this_base[i], that_base[j])) j++;
    else i++, j++;
  }
  while (i < this_count) temp_base[k++] = this_base[i++];
  if (k == this_count) {
    return false;
  } else if (k != 0) {
    *PtrSet__PAST(this) = this_base + k;
  } else {
    PtrSet_empty(this);
  }
  Except_DEBUG(PtrSet_ISORDERED(this) && PtrSet_checkOrder(this));
  return true;
}

#ifdef $XCC__c
{
  PtrSet_ this_set[1];
  PtrSet_ that_set[1];
  PtrSet_Ctor(this_set, Memory_Root, 4);
  PtrSet_Ctor(that_set, Memory_Root, 3);
  PtrSet_insert(this_set, (PtrSetMember)1);
  PtrSet_insert(this_set, (PtrSetMember)2);
  PtrSet_insert(this_set, (PtrSetMember)3);
  PtrSet_insert(this_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)3);
  PtrSet_insert(that_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)5);
  PtrSet_diff(this_set, that_set);
  printf("{ 1 2 3 4 } diff { 3 4 5 } = { ");
  PtrSet_FOREACH(this_set, intptr_t, member) {
    printf("%d ", (int)member);
  } PtrSet_ENDEACH;
  printf("}\n");
  PtrSet_Dtor(that_set);
  PtrSet_Dtor(this_set);
}
#endif//$XCC__c

#ifdef $XCC__c
{
  PtrSet_ this_set[1];
  PtrSet_ that_set[1];
  PtrSet_Ctor(this_set, Memory_Root, 4);
  PtrSet_Ctor(that_set, Memory_Root, 3);
  PtrSet_insert(this_set, (PtrSetMember)1);
  PtrSet_insert(this_set, (PtrSetMember)2);
  PtrSet_insert(this_set, (PtrSetMember)3);
  PtrSet_insert(this_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)3);
  PtrSet_insert(that_set, (PtrSetMember)4);
  PtrSet_insert(that_set, (PtrSetMember)5);
  PtrSet_diff(that_set, this_set);
  printf("{ 3 4 5 } diff { 1 2 3 4 } = { ");
  PtrSet_FOREACH(that_set, intptr_t, member) {
    printf("%d ", (int)member);
  } PtrSet_ENDEACH;
  printf("}\n");
  PtrSet_Dtor(that_set);
  PtrSet_Dtor(this_set);
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int max = 100;
  long int i;
  PtrSet_ sieve[1];
  PtrSet_ primes[1];
  PtrSet_Ctor(sieve, Memory_Root, 5);
  PtrSet_Ctor(primes, Memory_Root, 5);
  for (i = 2; i < max/2; i++) PtrSet_insert(sieve, (PtrSetMember)i);
  for (i = max - 1; i >= max/2; i--) PtrSet_insert(sieve, (PtrSetMember)i);
  while (!PtrSet_isEmpty(sieve)) {
    long int j = (long int)PtrSet_choose(sieve);
    printf("prime %ld\n", j);
    PtrSet_insert(primes, (PtrSetMember)j);
    for (i = j + j; i < max; i += j) PtrSet_remove(sieve, (PtrSetMember)i);
    printf("sieve: ");
    PtrSet_FOREACH(sieve, intptr_t, member) {
      printf("%d ", (int)member);
    } PtrSet_ENDEACH;
    printf("\n");
  }
  printf("primes: ");
  PtrSet_FOREACH(primes, intptr_t, member) {
    printf("%d ", (int)member);
  } PtrSet_ENDEACH;
  printf("\n");
  PtrSet_Dtor(primes);
  PtrSet_Dtor(sieve);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the PtrSet module.
 */
#define PtrSet_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the PtrSet module.
 */
#define PtrSet_FINI()
#endif//$XCC__h

#if XCC__C
static void
PtrSet_TEST(void)
{
#include "CCL/PtrSet_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(PtrSet_TEST);
  return 0;
}
#endif

