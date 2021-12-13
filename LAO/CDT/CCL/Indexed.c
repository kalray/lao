#ifdef $XCC_h
/*
 * !!!!	Indexed.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
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
 * Base class for indexed objects.
 *
 * An @a Indexed object contains two fields: an immutable @c IDENTITY and a mutable @c INDEX.
 * The @c IDENTITY is unique and can be used as a key for the object. The @c INDEX is managed
 * by a client who needs to attach information Indexed objects by means of an array.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/Indexed.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Base structure for Indexed objects.
 */
union Indexed_ {
  uint64_t UINT64;			//!< Force int64_t aligment.
  struct {
    uint32_t IDENTITY;			//!< Identity of this Indexed.
    int32_t INDEX;			//!< Index of this Indexed.
  } STRUCT_;
};
typedef union Indexed_ Indexed_, *Indexed;
typedef const union Indexed_ *const_Indexed;
#define Indexed_IDENTITY(this)	(this->STRUCT_.IDENTITY)
#define Indexed__IDENTITY(this)	(&this->STRUCT_.IDENTITY)
#define Indexed_INDEX(this)	(this->STRUCT_.INDEX)
#define Indexed__INDEX(this)	(&this->STRUCT_.INDEX)
#define Indexed_Ctor(this, factory) { \
  *Indexed__IDENTITY(this) = IndexedFactory_makeIdentity(factory); \
  *Indexed__INDEX(this) = -1; \
}
#endif//$XCC_h

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Indexed_)\t%zu\n", sizeof(Indexed_));
}
#endif//$XCC__c

#ifdef $XCC_h
typedef int (*IndexedCompare)(const_Indexed, const_Indexed);
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * An @a IndexedFactory Manages the IDENTITY(ies) of Indexed(s).
 */
struct IndexedFactory_ {
  //@args	
  uint32_t COUNTER;			//!< IDENTITY for the next Indexed object.
  uint32_t MAXCOUNT;			//!< Record of maximum COUNTER value reached.
};
#endif//$XCC_h

/**
 * Constructor of @c IndexedFactory
 */
IndexedFactory
IndexedFactory_Ctor(IndexedFactory this)
{
  *IndexedFactory__COUNTER(this) = 0;
  *IndexedFactory__MAXCOUNT(this) = 0;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IndexedFactory_)\t%zu\n", sizeof(IndexedFactory_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This IndexedFactory @c MAXCOUNT value.
 */
static inline uint32_t
IndexedFactory_maxCount(const_IndexedFactory this)
{
  return IndexedFactory_MAXCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make an @c IDENTITY for the next Indexed object.
 */
static inline int32_t
IndexedFactory_makeIdentity(IndexedFactory this)
{
  int32_t identity = (*IndexedFactory__COUNTER(this))++;
  if (IndexedFactory_MAXCOUNT(this) < IndexedFactory_COUNTER(this)) {
    *IndexedFactory__MAXCOUNT(this) = IndexedFactory_COUNTER(this);
  }
  return identity;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Reset this IndexedFactory COUNTER.
 */
static inline void
IndexedFactory_reset(IndexedFactory this)
{
  *IndexedFactory__COUNTER(this) = 0;
  *IndexedFactory__MAXCOUNT(this) = 0;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Table of Indexed objects managed by their @c INDEX.
 *
 * The IndexedTable has two purposes: 
 * -# manage the @c INDEX fields of Indexed(s) that belong to it;
 * -# provide the ground set for @c IndexedSparse subsets.
 */
struct IndexedTable_ {
  //@args	Memory memory, int32_t estimate
  PtrSeq_ __;           //!< PtrSeq
  //@access MEMORY	PtrSeq_memory(IndexedTable____(this))
  //@access ISEMPTY	PtrSeq_isEmpty(IndexedTable____(this))
  //@access ISSINGLE	PtrSeq_isSingle(IndexedTable____(this))
  //@access COUNT	PtrSeq_count(IndexedTable____(this))
  //@access BASE	(Indexed *)PtrSeq_base(IndexedTable____(this))
};
#endif//$XCC_h

/**
 * Constructor of IndexedTable.
 */
IndexedTable
IndexedTable_Ctor(IndexedTable this, Memory memory, int32_t estimate)
{
  PtrSeq_Ctor(IndexedTable____(this), memory, estimate);
  return this;
}

/**
 * Destructor of IndexedTable.
 */
void
IndexedTable_Dtor(IndexedTable this)
{
  PtrSeq_Dtor(IndexedTable____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IndexedTable_)\t%zu\n", sizeof(IndexedTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
static inline PtrSeq
IndexedTable__(IndexedTable this) {
  return IndexedTable____(this);
}
static inline const_PtrSeq
IndexedTable__const(const_IndexedTable this) {
  return IndexedTable____(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Empty this IndexedTable.
 */
static inline void
IndexedTable_empty(IndexedTable this)
{
  PtrSeq_empty(IndexedTable____(this));
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Memory
IndexedTable_memory(const_IndexedTable this)
{
  return IndexedTable_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
IndexedTable_isEmpty(const_IndexedTable this)
{
  return IndexedTable_ISEMPTY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
IndexedTable_isSingle(const_IndexedTable this)
{
  return IndexedTable_ISSINGLE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
IndexedTable_count(const_IndexedTable this)
{
  return IndexedTable_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Direct access to this IndexedTable Indexed array.
 */
static inline const Indexed *
IndexedTable_base(const_IndexedTable this)
{
  return IndexedTable_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterator this IndexedTable Indexed(ies).
 */
#define IndexedTable_FOREACH_Indexed(this, Type, object) { \
  const Indexed *IndexedTable_BASE = IndexedTable_base(this); \
  int32_t IndexedTable_COUNT = IndexedTable_count(this), IndexedTable_INDEX = 0; \
  for (; IndexedTable_INDEX < IndexedTable_COUNT; IndexedTable_INDEX++) { \
    Type object = (Type)IndexedTable_BASE[IndexedTable_INDEX];
#define IndexedTable_ENDEACH_Indexed \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Macro used in IndexedTable_contains.
 *
 * For use in cases IndexedTable_base is constant.
 */
#define IndexedTable_CONTAINS(object) ( \
  Indexed_INDEX((const_Indexed)(object)) >= 0 && \
  Indexed_INDEX((const_Indexed)(object)) < IndexedTable_COUNT && \
  IndexedTable_BASE[Indexed_INDEX((const_Indexed)(object))] == (const_Indexed)(object) \
)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test that this IndexedTable contains a Indexed.
 */
static inline bool
IndexedTable_contains(const_IndexedTable this, const_Indexed object)
{
  int32_t count = IndexedTable_COUNT(this);
  int32_t index = Indexed_INDEX(object);
  const Indexed *base = IndexedTable_BASE(this);
  if (index >= 0 && index < count) {
    return base[index] == object;
  }
  return false;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Insert an object into this IndexedTable if not there.
 *
 * table:	IndexedTable already initialized.
 * object:	Indexed object to add to the table.
 */
static inline bool
IndexedTable_insert(IndexedTable this, Indexed object)
{
  int32_t count = IndexedTable_COUNT(this);
  int32_t index = Indexed_INDEX(object);
  Indexed *base = IndexedTable_BASE(this);
  if (index >= 0 && index < count) {
    if (base[index] == object) return false;
  }
  PtrSeq_push2(IndexedTable____(this), object);
  *Indexed__INDEX(object) = count;
  return true;
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Remove a Indexed with high INDEX from IndexedTable.
 *
 * @warning The INDEX of some other Indexed object will change.
 */
bool
IndexedTable_removeHigh(IndexedTable this, const_Indexed object, int32_t high);
#endif//$XCC_h

bool
IndexedTable_removeHigh(IndexedTable this, const_Indexed object, int32_t high)
{
  int32_t count = IndexedTable_COUNT(this);
  int32_t index = Indexed_INDEX(object);
  Indexed *base = IndexedTable_BASE(this);
  if (index >= high && index < count && base[index] == object) {
    Indexed last_object = base[count - 1];
    *Indexed__INDEX(last_object) = index;
    base[index] = last_object;
    PtrSeq_pop(IndexedTable____(this));
    return true;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Remove a Indexed from IndexedTable.
 *
 * @warning The INDEX of some other Indexed will change.
 */
static inline bool
IndexedTable_remove(IndexedTable this, const_Indexed object)
{
  return IndexedTable_removeHigh(this, object, 0);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make the IndexedTable order same as a IndexedSet.
 *
 * @warning The INDEX of Indexed will change.
 */
void
IndexedTable_makeIndexedSetOrder(IndexedTable this);
#endif//$XCC_h

void
IndexedTable_makeIndexedSetOrder(IndexedTable this)
{
  int32_t count = IndexedTable_COUNT(this), index;
  Indexed *array = IndexedTable_BASE(this);
  IndexedSet_SortArray(array, 0, count - 1);
  for (index = 0; index < count; index++) {
    *Indexed__INDEX(array[index]) = index;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Sparse set of Indexed objects based on INDEX Sparse set.
 */
struct IndexedSparse_ {
  //@args	Memory memory, IndexedTable table
  Sparse_ __;		//!< A IndexedSparse_ ISA Sparse_.
  //@access MEMORY	Sparse_MEMORY(IndexedSparse____(this))
  IndexedTable TABLE;	//!< The IndexedTable this IndexedSparse refers to.
  //@access BASE	IndexedTable_base(IndexedSparse_TABLE(this))
};
#endif//$XCC_h

#ifdef $XCC_h
static inline Sparse
IndexedSparse__(IndexedSparse this) {
  return IndexedSparse____(this);
}
static inline const_Sparse
IndexedSparse__const(const_IndexedSparse this) {
  return IndexedSparse____(this);
}
#endif//$XCC_h

IndexedSparse
IndexedSparse_Ctor(IndexedSparse this, Memory memory, IndexedTable table)
{
  int32_t count = IndexedTable_count(table);
  Sparse_Ctor(IndexedSparse____(this), memory, count);
  *IndexedSparse__TABLE(this) = table;
  return this;
}

void
IndexedSparse_Dtor(IndexedSparse this)
{
  Sparse_Dtor(IndexedSparse____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IndexedSparse_)\t%zu\n", sizeof(IndexedSparse_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * The IndexedTable this IndexedSparse refers to.
 */
static inline IndexedTable
IndexedSparse_table(IndexedSparse this) {
  return IndexedSparse_TABLE(this);
}
static inline const_IndexedTable
IndexedSparse_table_const(const_IndexedSparse this) {
  return IndexedSparse_TABLE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by IndexedSparse_FOREACH_Indexed.
 */
static inline const Indexed *
IndexedSparse_base(const_IndexedSparse this) {
  return IndexedSparse_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this IndexedSparse objects.
 */
#define IndexedSparse_FOREACH_Indexed(this, Type, object) { \
  const Indexed *IndexedSparse_BASE = IndexedSparse_base(this); \
  const_Sparse IndexedSparse____ = IndexedSparse__const(this); \
  Sparse_FOREACH(IndexedSparse____, IndexedSparse_INDEX) { \
    int32_t IndexedSparse_POSITION = Sparse_POSITION; \
    Type object = (Type)IndexedSparse_BASE[IndexedSparse_INDEX];
#define IndexedSparse_ENDEACH_Indexed \
  } Sparse_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
#define IndexedSparse_empty(this) \
  Sparse_empty(IndexedSparse__(this))
#define IndexedSparse_memory(this) \
  Sparse_memory(IndexedSparse__const(this))
#define IndexedSparse_isEmpty(this) \
  Sparse_isEmpty(IndexedSparse__const(this))
#define IndexedSparse_isSingle(this) \
  Sparse_isSingle(IndexedSparse__const(this))
#define IndexedSparse_count(this) \
  Sparse_count(IndexedSparse__const(this))
#endif//$XCC_h

#ifdef $XCC_h
static inline Indexed
IndexedSparse_choose(IndexedSparse this)
{
  return IndexedSparse_BASE(this)[Sparse_choose(IndexedSparse__(this))];
}
#endif//$XCC_h

#ifdef $XCC_h
static inline bool
IndexedSparse_contains(const_IndexedSparse this, const_Indexed object)
{
  Except_CHECK(IndexedTable_contains(IndexedSparse_table_const(this), object));
  return Sparse_contains(IndexedSparse__const(this), Indexed_INDEX(object));
}
#endif//$XCC_h

#ifdef $XCC_h
static inline bool
IndexedSparse_insert(IndexedSparse this, const_Indexed object)
{
  Except_CHECK(IndexedTable_contains(IndexedSparse_table(this), object));
  return Sparse_insert(IndexedSparse__(this), Indexed_INDEX(object));
}
#endif//$XCC_h

#ifdef $XCC_h
static inline bool
IndexedSparse_remove(IndexedSparse this, const_Indexed object)
{
  Except_CHECK(IndexedTable_contains(IndexedSparse_table(this), object));
  return Sparse_remove(IndexedSparse__(this), Indexed_INDEX(object));
}
#endif//$XCC_h

#ifdef $XCC_h
#define IndexedSparse_equals(this, that) \
  Sparse_equals(IndexedSparse__const(this), IndexedSparse__const(that))
#define IndexedSparse_assign(this, that) \
  Sparse_assign(IndexedSparse__(this), IndexedSparse__const(that))
#define IndexedSparse_union(this, that) \
  Sparse_union(IndexedSparse__(this), IndexedSparse__const(that))
#define IndexedSparse_inter(this, that) \
  Sparse_inter(IndexedSparse__(this), IndexedSparse__const(that))
#define IndexedSparse_diff(this, that) \
  Sparse_diff(IndexedSparse__(this), IndexedSparse__const(that))
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
IndexedSparse_position(const_IndexedSparse this, const_Indexed object)
{
  Except_CHECK(IndexedTable_contains(IndexedSparse_table_const(this), object));
  return Sparse_position(IndexedSparse__const(this), Indexed_INDEX(object));
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#define Indexed_GT(a, b) (Indexed_IDENTITY(a) > Indexed_IDENTITY(b))
#define Indexed_LT(a, b) (Indexed_IDENTITY(a) < Indexed_IDENTITY(b))
#define Indexed_EQ(a, b) (Indexed_IDENTITY(a) == Indexed_IDENTITY(b))
#define Indexed_NE(a, b) (Indexed_IDENTITY(a) != Indexed_IDENTITY(b))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set of Indexed objects implemented as array ordered by IDENTITY.
 */
struct IndexedSet_ {
  //@args	Memory memory, int32_t estimate
  PtrSet_ __;
  //@access MEMORY	PtrSet_memory(IndexedSet____(this))
  //@access STATUS	PtrSet_STATUS(IndexedSet____(this))
  //@mutate STATUS	PtrSet__STATUS(IndexedSet____(this))
  //@access MAXCOUNT	PtrSet_MAXCOUNT(IndexedSet____(this))
  //@access ISORDERED	PtrSet_ISORDERED(IndexedSet____(this))
  //@access ISEMPTY	PtrSet_isEmpty(IndexedSet____(this))
  //@access ISSINGLE	PtrSet_isSingle(IndexedSet____(this))
  //@access COUNT	PtrSet_count(IndexedSet____(this))
  //@access BASE	(Indexed *)PtrSet_BASE(IndexedSet____(this))
  //@mutate BASE	(Indexed **)PtrSet__BASE(IndexedSet____(this))
  //@access PAST	(Indexed *)PtrSet_PAST(IndexedSet____(this))
  //@mutate PAST	(Indexed **)PtrSet__PAST(IndexedSet____(this))
};
#endif//$XCC_h

IndexedSet
IndexedSet_Ctor(IndexedSet this, Memory memory, int32_t estimate)
{
  PtrSet_Ctor(IndexedSet____(this), memory, estimate);
  return this;
}

void
IndexedSet_Dtor(IndexedSet this)
{
  PtrSet_Dtor(IndexedSet____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IndexedSet_)\t%zu\n", sizeof(IndexedSet_));
}
#endif//$XCC__c

#ifdef $XCC_h
static inline PtrSet
IndexedSet__(IndexedSet this) {
  return IndexedSet____(this);
}
static inline const_PtrSet
IndexedSet__const(const_IndexedSet this) {
  return IndexedSet____(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline void
IndexedSet_empty(IndexedSet this)
{
  PtrSet_empty(IndexedSet____(this));
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Memory
IndexedSet_memory(const_IndexedSet this)
{
  return IndexedSet_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
IndexedSet_isEmpty(const_IndexedSet this)
{
  return IndexedSet_ISEMPTY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
IndexedSet_isSingle(const_IndexedSet this)
{
  return IndexedSet_ISSINGLE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int32_t
IndexedSet_count(const_IndexedSet this)
{
  return IndexedSet_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by IndexedSet_FOREACH.
 */
static inline const Indexed *
IndexedSet_base(const_IndexedSet this)
{
  return IndexedSet_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by IndexedSet_FOREACH.
 */
static inline const Indexed *
IndexedSet_past(const_IndexedSet this)
{
  return IndexedSet_PAST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/*
  IndexedSet_FOREACH_Indexed --	Iterates over the IndexedSet members.
 */
#define IndexedSet_FOREACH_Indexed(this, member) { \
  const Indexed *IndexedSet_BASE = IndexedSet_base(this); \
  const Indexed *IndexedSet_PAST = IndexedSet_past(this); \
  if (IndexedSet_BASE != IndexedSet_PAST) { \
    const Indexed *IndexedSet_ITER = IndexedSet_PAST - 1; \
    for (; IndexedSet_ITER >= IndexedSet_BASE; --(IndexedSet_ITER)) { \
      Indexed member = *IndexedSet_ITER;
#define IndexedSet_ENDEACH_Indexed \
    } \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/*
  IndexedSet_FORSORT_Indexed --	Iterates over the sorted IndexedSet members.
 */
#define IndexedSet_FORSORT_Indexed(this, compare, member) { \
  IndexedCompare IndexedSet_COMPARE = (IndexedCompare)(compare); \
  uint32_t IndexedSet_COUNT = IndexedSet_count(this), IndexedSet_I = 0; \
  const Indexed *IndexedSet_BASE = IndexedSet_base(this); \
  const Indexed *IndexedSet_PAST = IndexedSet_past(this), *(IndexedSet_ITER); \
  Indexed *IndexedSet_ARRAY = alloca(sizeof(Indexed)*IndexedSet_COUNT); \
  for (IndexedSet_I = 0, IndexedSet_ITER = IndexedSet_BASE; \
       IndexedSet_ITER < IndexedSet_PAST; ++(IndexedSet_ITER), ++IndexedSet_I) \
    IndexedSet_ARRAY[IndexedSet_I] = *(IndexedSet_ITER); \
  HackerPtr_SortArray((HackerPtr *)IndexedSet_ARRAY, 0, IndexedSet_COUNT - 1, (HackerPtrCompare) IndexedSet_COMPARE); \
  for (IndexedSet_I = 0; IndexedSet_I < IndexedSet_COUNT; IndexedSet_I++) { \
    Indexed member = IndexedSet_ARRAY[IndexedSet_I];
#define IndexedSet_ENDSORT_Indexed \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check order of this IndexedSet.
 */
bool
IndexedSet_checkOrder(const_IndexedSet this);
#endif//$XCC_h

bool
IndexedSet_checkOrder(const_IndexedSet this)
{
  int32_t count = IndexedSet_count(this), i = 0;
  Indexed *base = IndexedSet_BASE(this);
  if (base != NULL) {
    for (i = 1; i < count; i++) {
      if (!Indexed_GT(base[i - 1], base[i])) return false;
    }
  }
  return true;
}

#ifdef $XCC_h
/**
 * Push as last member on the IndexedSet, resizing the IndexedSet if necessary.
 *
 * As the IndexedSet may be resized, it is an error to maintain pointers to existing items.
 */
void
IndexedSet_pushMember(IndexedSet this, Indexed member);
#endif//$XCC_h

#ifndef IndexedSet_MAGICCOUNT
#define IndexedSet_MAGICCOUNT 16
#endif//IndexedSet_MAGICCOUNT

void
IndexedSet_pushMember(IndexedSet this, Indexed member)
{
  int32_t maxCount = IndexedSet_MAXCOUNT(this);
  Indexed *base = IndexedSet_BASE(this);
  Indexed *past = IndexedSet_PAST(this);
  int32_t count = past - base;
  if (count == maxCount) {
    int32_t newMaxCount = count*2;
    if (newMaxCount < IndexedSet_MAGICCOUNT) {
      newMaxCount = IndexedSet_MAGICCOUNT;
    }
    PtrSet_resize(IndexedSet____(this), newMaxCount);
    past = IndexedSet_PAST(this);
    maxCount = IndexedSet_MAXCOUNT(this);
    Except_CHECK(!PtrSet_isFull(IndexedSet____(this)));
  }
  *past = member;
  *IndexedSet__PAST(this) = past + 1;
  if (count && !Indexed_LT(past[-1], member)) {
    *IndexedSet__STATUS(this) = (maxCount<<1)|0x1;
  }
}

#ifdef $XCC_h
/**
 * Reorder this IndexedSet members after calls to IndexedSet_pushMember.
 *
 * Use the en.wikibook.org heapsort, then call IndexedSet_uniq.
 */
int32_t
IndexedSet_makeOrder(IndexedSet this);
#endif//$XCC_h

#if 0
int32_t
IndexedSet_sort(IndexedSet this)
{
  Indexed *members = IndexedSet_BASE(this);
  Indexed *past = IndexedSet_PAST(this);
  int32_t count = past - members;
  int32_t n = count, i = count/2;
  while (true) {
    int32_t parent, child;
    Indexed temp_member;
    if (i > 0) {
      temp_member = members[--i];
    } else if (n > 1) {
      temp_member = members[--n];
      members[n] = members[0];
    } else break;
    parent = i, child = i*2 + 1;
    while (child < n) {
      if (   child + 1 < n
          && Indexed_LT(members[child + 1], members[child])) {
        child = child + 1;
      }
      if (Indexed_LT(members[child], temp_member)) {
        members[parent] = members[child];
        parent = child;
        child = child*2 + 1;
      } else break;
    }
    members[parent] = temp_member;
  }
  *IndexedSet__STATUS(this) |= 0x1;
  return IndexedSet_uniq(this);
}
#else
int32_t
IndexedSet_makeOrder(IndexedSet this)
{
  Indexed *base = IndexedSet_BASE(this);
  Indexed *past = IndexedSet_PAST(this);
  Indexed *members = past - 1;
  int32_t count = past - base;
  int32_t n = -count, i = -(count/2), d = 0;
  while (i++ < 0) {
    int32_t parent = i, child = i*2 - 1;
    Indexed temp_member = members[i];
    while (child > n) {
      if (   child - 1 > n
          && Indexed_GT(members[child - 1], members[child])) {
        child = child - 1;
      }
      if (Indexed_GT(members[child], temp_member)) {
        members[parent] = members[child];
        parent = child;
        child = child*2 - 1;
      } else break;
    }
    members[parent] = temp_member;
  }
  while (n++ < -1) {
    int32_t parent = 0, child = -1;
    Indexed temp_member = members[n];
    members[n] = members[0];
    while (child > n) {
      if (   child - 1 > n
          && Indexed_GT(members[child - 1], members[child])) {
        child = child - 1;
      }
      if (Indexed_GT(members[child], temp_member)) {
        members[parent] = members[child];
        parent = child;
        child = child*2 - 1;
      } else break;
    }
    members[parent] = temp_member;
  }
  // This IndexedSet is ordered now.
  *IndexedSet__STATUS(this) |= 0x1;
  return IndexedSet_uniq(this);
}
#endif

#ifdef $XCC__h
int32_t
IndexedSet_uniq(IndexedSet this);
#endif//$XCC__h

int32_t
IndexedSet_uniq(IndexedSet this)
{
  Indexed *base = IndexedSet_BASE(this);
  Indexed *past = IndexedSet_PAST(this);
  int32_t count = past - base, d = 0, i;
  if (base != NULL) {
    for (i = 0; i + d + 1 < count;) {
      base[i] = base[i + d];
      if (Indexed_EQ(base[i + d], base[i + d + 1])) d++;
      else i++;
    }
    base[i] = base[i + d];
    *IndexedSet__PAST(this) = base + count - d;
  }
  Except_DEBUG(IndexedSet_ISORDERED(this) && IndexedSet_checkOrder(this));
  Except_CHECK(IndexedSet_count(this) == count - d);
  return count - d;
}

#ifdef $XCC_h
/**
 * Function used to merge sort a Indexed array.
 */
void
IndexedSet_SortArray(Indexed array[], int32_t first, int32_t last);
#endif//$XCC_h

void
IndexedSet_SortArray(Indexed array[], int32_t first, int32_t last)
{
  int32_t count = last - first + 1;
  int32_t mid = (first + last) >> 1;
  int32_t idx1 = first, idx2 = mid + 1, i;
  if (idx1 < mid) IndexedSet_SortArray(array, idx1, mid);
  if (idx2 < last) IndexedSet_SortArray(array, idx2, last);
  // Merge step.
  if (count > 1) {
    Indexed *tempArray = alloca(sizeof(Indexed)*count);
    for (i = 0; i < count; i++) {
      if (idx1 > mid) {
        tempArray[i] = array[idx2++];
      } else if (idx2 > last) {
        tempArray[i] = array[idx1++];
      } else if (!Indexed_LT(array[idx1], array[idx2])) {
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

#ifdef $XCC_h
/**
 * This IndexedSet ordered status.
 */
static inline bool
IndexedSet_isOrdered(const_IndexedSet this)
{
  return IndexedSet_ISORDERED(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this IndexedSet status as unordered.
 */
static inline void
IndexedSet_clearOrdered(IndexedSet this)
{
  *IndexedSet__STATUS(this) &= ~0x1;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this IndexedSet status as ordered.
 */
static inline void
IndexedSet_raiseOrdered(IndexedSet this)
{
  *IndexedSet__STATUS(this) |= 0x1;
  Except_CHECK(IndexedSet_checkOrder(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Base of this IndexedSet for use by external sort.
 */
static inline Indexed *
IndexedSet_base_(IndexedSet this)
{
  *IndexedSet__STATUS(this) &= ~0x1;
  return IndexedSet_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test a member for containment.
 *
 * Return:	True if this IndexedSet contains member.
 */
bool
IndexedSet_contains(const_IndexedSet this, Indexed member);
#endif//$XCC_h

bool
IndexedSet_contains(const_IndexedSet this, Indexed member)
{
  int32_t count = IndexedSet_count(this);
  int32_t l = 0, r = count - 1, i = 0;
  Indexed *base = IndexedSet_BASE(this);
  Except_REQUIRE(IndexedSet_isOrdered(this));
  while (r >= l) {
    i = (l + r) >> 1;
    if (Indexed_LT(base[i], member)) r = i - 1;
    else if (Indexed_GT(base[i],  member)) l = i + 1;
    else return true;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Insert a member in this IndexedSet.
 *
 * Return:	False iff member was already contained in this IndexedSet.
 */
bool
IndexedSet_insert(IndexedSet this, Indexed member);
#endif//$XCC_h

bool
IndexedSet_insert(IndexedSet this, Indexed member)
{
#if 0
  int32_t count = IndexedSet_count(this);
  IndexedSet_pushMember(this, member);
  IndexedSet_makeOrder(this);
  return count == IndexedSet_count(this);
#else
  if (IndexedSet_isEmpty(this)) {
    IndexedSet_pushMember(this, member);
  } else {
    Indexed *base = IndexedSet_BASE(this);
    Indexed *past = IndexedSet_PAST(this);
    int32_t count = past - base;
    int32_t l = 0, r = count - 1, i = 0, j;
    Except_REQUIRE(IndexedSet_isOrdered(this));
    while (r >= l) {
      i = (l + r) >> 1;
      if (Indexed_LT(base[i], member)) r = i - 1;
      else if (Indexed_GT(base[i], member)) l = i + 1;
      else return false;
    }
    if (Indexed_GT(base[i], member)) i++;
    if (count == IndexedSet_MAXCOUNT(this)) {
      int32_t newMaxCount = IndexedSet_MAGICCOUNT > count*2?
                            IndexedSet_MAGICCOUNT: count*2;
      PtrSet_resize(IndexedSet____(this), newMaxCount);
      // Reload base because of the IndexedSet_resize side-effects.
      base = IndexedSet_BASE(this);
    }
    *IndexedSet__PAST(this) = base + count + 1;
    for (j = count; j > i; j--) base[j] = base[j - 1];
    base[i] = member;
    Except_DEBUG(IndexedSet_ISORDERED(this) && IndexedSet_checkOrder(this));
  }
  return true;
#endif
}

#ifdef $XCC_h
/**
 * Remove a member from this IndexedSet.
 *
 * Return:	False iff member was not contained in this IndexedSet.
 */
bool
IndexedSet_remove(IndexedSet this, Indexed member);
#endif//$XCC_h

bool
IndexedSet_remove(IndexedSet this, Indexed member)
{
  Indexed *base = IndexedSet_BASE(this);
  Indexed *past = IndexedSet_PAST(this);
  int32_t count = past - base;
  int32_t l = 0, r = count - 1, i = 0;
  Except_REQUIRE(IndexedSet_isOrdered(this));
  while (r >= l) {
    i = (l + r) >> 1;
    if (Indexed_LT(base[i], member)) r = i - 1;
    else if (Indexed_GT(base[i], member)) l = i + 1;
    else {
      for (; i < count - 1; i++) base[i] = base[i + 1];
      *IndexedSet__PAST(this) = past - 1;
      Except_DEBUG(IndexedSet_ISORDERED(this) && IndexedSet_checkOrder(this));
      return true;
    }
  }
  Except_DEBUG(IndexedSet_ISORDERED(this) && IndexedSet_checkOrder(this));
  return false;
}

#ifdef $XCC_h
/**
 * Choose and remove a member of the IndexedSet.
 */
static inline Indexed
IndexedSet_choose(IndexedSet this)
{
  return (Indexed)PtrSet_choose(IndexedSet____(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test for IndexedSet equality.
 */
bool
IndexedSet_equals(const_IndexedSet this, const_IndexedSet that);
#endif//$XCC_h

bool
IndexedSet_equals(const_IndexedSet this, const_IndexedSet that)
{
  return PtrSet_equals(IndexedSet____(this), IndexedSet____(that));
}

#ifdef $XCC_h
/**
 * Assign the contents of that IndexedSet to this IndexedSet.
 */
void
IndexedSet_assign(IndexedSet this, const_IndexedSet that);
#endif//$XCC_h

void
IndexedSet_assign(IndexedSet this, const_IndexedSet that)
{
  PtrSet_assign(IndexedSet____(this), IndexedSet____(that));
}

#ifdef $XCC_h
/**
 * True iff this IndexedSet comprises that IndexedSet.
 */
bool
IndexedSet_comprises(const_IndexedSet this, const_IndexedSet that);
#endif//$XCC_h

bool
IndexedSet_comprises(const_IndexedSet this, const_IndexedSet that)
{
  int32_t this_count = IndexedSet_count(this);
  int32_t that_count = IndexedSet_count(that);
  Except_REQUIRE(IndexedSet_isOrdered(this));
  Except_REQUIRE(IndexedSet_isOrdered(that));
  if (this_count >= that_count) {
    int32_t i = 0, j = 0, k = 0;
    const Indexed *this_base = IndexedSet_BASE(this);
    const Indexed *that_base = IndexedSet_BASE(that);
    while (i < this_count && j < that_count) {
      Indexed this_member = this_base[i];
      Indexed that_member = that_base[j];
      if (Indexed_GT(this_member, that_member)) i++, k++;
      else if (Indexed_LT(this_member, that_member)) j++, k++;
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
 * Union of this IndexedSet with that IndexedSet.
 */
bool
IndexedSet_union(IndexedSet this, const_IndexedSet that);
#endif//$XCC_h

bool
IndexedSet_union(IndexedSet this, const_IndexedSet that)
{
  Memory memory = IndexedSet_MEMORY(this);
  int32_t this_count = IndexedSet_count(this), i = 0;
  int32_t that_count = IndexedSet_count(that), j = 0;
  int32_t temp_count = this_count + that_count, k = 0;
  size_t tempSize = temp_count*sizeof(Indexed);
  size_t alignedSize = _ALIGN_NEXT(tempSize);
  Indexed *this_base = IndexedSet_BASE(this);
  Indexed *that_base = IndexedSet_BASE(that);
  Indexed *temp_base = alloca(alignedSize);
  Except_REQUIRE(IndexedSet_isOrdered(this));
  Except_REQUIRE(IndexedSet_isOrdered(that));
  while (i < this_count && j < that_count) {
    Indexed this_member = this_base[i];
    Indexed that_member = that_base[j];
    if (Indexed_GT(this_member, that_member)) {
      temp_base[k++] = this_member, i++;
    } else if (Indexed_LT(this_member, that_member)) {
      temp_base[k++] = that_member, j++;
    } else temp_base[k++] = this_member, i++, j++;
  }
  while (i < this_count) temp_base[k++] = this_base[i++];
  while (j < that_count) temp_base[k++] = that_base[j++];
  if (k == this_count) {
    // This set is unchanged.
    return false;
  }
  if (k > IndexedSet_MAXCOUNT(this)) {
    // Must free then allocate this_base and copy temp_base into it.
    Memory_free(memory, this_base);
    this_base = Memory_alloc_(memory, alignedSize);
    for (i = 0; i < k; i++) this_base[i] = temp_base[i];
    *IndexedSet__STATUS(this) = ((temp_count<<1)|true);
    *IndexedSet__BASE(this) = this_base;
    *IndexedSet__PAST(this) = this_base + k;
  } else {
    // Here k <= maxCount, so just copy temp_base into this_base.
    for (i = 0; i < k; i++) this_base[i] = temp_base[i];
    *IndexedSet__PAST(this) = this_base + k;
  }
  Except_DEBUG(IndexedSet_ISORDERED(this) && IndexedSet_checkOrder(this));
  return true;
}

#ifdef $XCC_h
/**
 * Intersect this IndexedSet with that IndexedSet.
 */
bool
IndexedSet_inter(IndexedSet this, const_IndexedSet that);
#endif//$XCC_h

bool
IndexedSet_inter(IndexedSet this, const_IndexedSet that)
{
  int32_t this_count = IndexedSet_count(this), i = 0;
  int32_t that_count = IndexedSet_count(that), j = 0, k = 0;
  Indexed *this_base = IndexedSet_BASE(this);
  Indexed *that_base = IndexedSet_BASE(that);
  Indexed *temp_base = this_base;
  Except_REQUIRE(IndexedSet_isOrdered(this));
  Except_REQUIRE(IndexedSet_isOrdered(that));
  while (i < this_count && j < that_count) {
    Indexed this_member = this_base[i];
    Indexed that_member = that_base[j];
    if (Indexed_GT(this_member, that_member)) i++;
    else if (Indexed_LT(this_member, that_member)) j++;
    else temp_base[k++] = this_member, i++, j++;
  }
  if (k == this_count) {
    return false;
  } else if (k != 0) {
    *IndexedSet__PAST(this) = this_base + k;
  } else {
    IndexedSet_empty(this);
  }
  Except_DEBUG(IndexedSet_ISORDERED(this) && IndexedSet_checkOrder(this));
  return true;
}

#ifdef $XCC_h
/**
 * Check if this IndexedSet and that IndexedSet overlap.
 */
bool
IndexedSet_overlaps(const_IndexedSet this, const_IndexedSet that);
#endif//$XCC_h

bool
IndexedSet_overlaps(const_IndexedSet this, const_IndexedSet that)
{
  int32_t this_count = IndexedSet_count(this), i = 0;
  int32_t that_count = IndexedSet_count(that), j = 0;
  Indexed *this_base = IndexedSet_BASE(this);
  Indexed *that_base = IndexedSet_BASE(that);
  Except_REQUIRE(IndexedSet_isOrdered(this));
  Except_REQUIRE(IndexedSet_isOrdered(that));
  while (i < this_count && j < that_count) {
    Indexed this_member = this_base[i];
    Indexed that_member = that_base[j];
    if (Indexed_GT(this_member, that_member)) i++;
    else if (Indexed_LT(this_member, that_member)) j++;
    else return true;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Remove that IndexedSet members from this IndexedSet.
 */
bool
IndexedSet_diff(IndexedSet this, const_IndexedSet that);
#endif//$XCC_h

bool
IndexedSet_diff(IndexedSet this, const_IndexedSet that)
{
  int32_t this_count = IndexedSet_count(this), i = 0;
  int32_t that_count = IndexedSet_count(that), j = 0, k = 0;
  Indexed *this_base = IndexedSet_BASE(this);
  Indexed *that_base = IndexedSet_BASE(that);
  Indexed *temp_base = this_base;
  Except_REQUIRE(IndexedSet_isOrdered(this));
  Except_REQUIRE(IndexedSet_isOrdered(that));
  while (i < this_count && j < that_count) {
    if (Indexed_GT(this_base[i], that_base[j]))
      temp_base[k++] = this_base[i++];
    else if (Indexed_LT(this_base[i], that_base[j])) j++;
    else i++, j++;
  }
  while (i < this_count) temp_base[k++] = this_base[i++];
  if (k == this_count) {
    return false;
  } else if (k != 0) {
    *IndexedSet__PAST(this) = this_base + k;
  } else {
    IndexedSet_empty(this);
  }
  Except_DEBUG(IndexedSet_ISORDERED(this) && IndexedSet_checkOrder(this));
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Indexed module.
 */
#define Indexed_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Indexed module.
 */
#define Indexed_FINI()
#endif//$XCC__h

#if XCC__C
static void
Indexed_TEST(void)
{
#include "CCL/Indexed_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(Indexed_TEST);
  return 0;
}
#endif

