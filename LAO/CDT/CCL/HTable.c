#ifdef $XCC_h
/*
 * !!!!	HTable.xcc
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
 * @brief Hash table implementation.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/HTable.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Multiplicative hash using the golden ratio (Knuth).
 *
 * The input number interpreted as Q0.32 fractional is multiplied by the Q0.32
 * fractional representation of the golden ratio (sqrt(5) - 1)/2. Actually, the
 * constant used is 2654435761, a prime close enough to (sqrt(5) - 1)/2 * 2**32.
 * As a final step, we apply a pseudo modular reduction by the prime 2**31 - 1.
 */
#define HTable_Hash(number) \
  (sizeof(number) > sizeof(uint32_t)? HTable_Hash64((intptr_t)(number)): HTable_Hash32((intptr_t)(number)))
static inline uint32_t
HTable_Hash32(uint32_t number)
{
  uint64_t result = number*(uint64_t)2654435761U;
  uint32_t hash = result + (result >> 31);
  return hash + (hash >> 31);
}
static inline uint32_t
HTable_Hash64(uint64_t number)
{
  return HTable_Hash32(number>>32) ^ HTable_Hash32((uint32_t)number);
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int i, j;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      uint32_t number = (1<<i)*j;
      uint32_t hash = HTable_Hash32(number);
      printf("%4x=>%4x\t", number, hash & 0xffff);
    }
    printf("\n");
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef const void *HTableKey;
typedef void *HTableValue;
typedef void (*HTableValueRelease)(HTableValue);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The HTableEntry_ structure implements the HTable entries.
 * This structure is published for use in HTable_FOREACH.
 */
struct HTableEntry_ {
  //@args	HTableKey key, size_t valueSize, struct HTableEntry_ *link
  HTableKey KEY;		//!< The key, which holds a pointer.
  struct HTableEntry_ *LINK;	//!< Link to the next HTableEntry in bucket.
  //@access VALUE	((HTableValue)((HTableEntry_ *)(this) + 1))
};
#endif//$XCC_h

HTableEntry
HTableEntry_Ctor(HTableEntry this,
                 HTableKey key, size_t valueSize, struct HTableEntry_ *link)
{
  *HTableEntry__KEY(this) = key;
  *HTableEntry__LINK(this) = link;
  return this;
}

size_t
HTableEntry_Size(HTableKey key, size_t valueSize, struct HTableEntry_ *link)
{
  return (sizeof(HTableEntry_) + valueSize);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(HTableEntry_)\t%zu\n", sizeof(HTableEntry_));
  Except_ALWAYS(_ALIGNED(sizeof(HTableEntry_)));
}
#endif//$XCC__c

#ifdef $XCC_h
static inline HTableKey
HTableEntry_key(const_HTableEntry this)
{
  return HTableEntry_KEY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline HTableValue
HTableEntry_value(const_HTableEntry this)
{
  return HTableEntry_VALUE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline  struct HTableEntry_ *
HTableEntry_link(HTableEntry this)
{
  return HTableEntry_LINK(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Hash table implementation with chaining.
 *
 * @param memory Where the HTable entries are allocated.
 * @param estimate Estimation of maximum number of entries in the hash table.
 */
struct HTable_ {
  //@args	Memory memory, uint32_t estimate
  Memory MEMORY;
  uint32_t MASK;		//!< Mask for modulo clp2.
  int32_t COUNT;		//!< Count this HTable ENTRIES.
  HTableEntry *BUCKETS;		//!< This HTable buckets.
};
#endif//$XCC_h

HTable
HTable_Ctor(HTable this, Memory memory, uint32_t estimate)
{
  *HTable__MEMORY(this) = memory;
  *HTable__MASK(this) = 0;
  *HTable__COUNT(this) = 0;
  *HTable__BUCKETS(this) = NULL;
  if (estimate > 0) HTable_resize(this, estimate);
  return this;
}

void
HTable_Dtor(HTable this)
{
  Memory memory = HTable_MEMORY(this);
  HTable_empty(this, NULL);
  Memory_free(memory, HTable_BUCKETS(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(HTable_)\t%zu\n", sizeof(HTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Resize this HTable for the given estimated count.
 */
void
HTable_resize(HTable this, uint32_t estimate);
#endif//$XCC_h

void
HTable_resize(HTable this, uint32_t estimate)
{
  Memory memory = HTable_MEMORY(this);
  HTableEntry *buckets = HTable_BUCKETS(this);
  uint32_t clp2 = HackerU32_clp2(estimate), mask = clp2 - 1;
  size_t size = clp2*sizeof(HTableEntry);
  HTableEntry *new_buckets = Memory_alloc_(memory, size);
  memset(new_buckets, 0, size);
  if (buckets != NULL) {
    HTable_FOREACH(this, key, void, _) {
      uint32_t hash = HTable_Hash(key);
      int32_t index = hash & mask;
      *HTableEntry__LINK(HTable_ENTRY) = new_buckets[index];
      new_buckets[index] = HTable_ENTRY;
    } HTable_ENDEACH;
    Memory_free(memory, buckets);
  }
  *HTable__MASK(this) = mask;
  *HTable__BUCKETS(this) = new_buckets;
}

#ifdef $XCC_h
/**
 * Empty this HTable.
 *
 * @param release	function to apply to every value contained in the table
 * (NULL if not necessary).
 */
void
HTable_empty(HTable this, HTableValueRelease release);
#endif//$XCC_h

void
HTable_empty(HTable this, HTableValueRelease release)
{
  HTableEntry *buckets = HTable_BUCKETS(this);
  if (buckets != NULL) {
    Memory memory = HTable_MEMORY(this);
    uint32_t mask = HTable_MASK(this), i;
    for (i = 0; i <= mask; i++) {
      HTableEntry entry = buckets[i];
      while (entry != NULL) {
        HTableEntry link = HTableEntry_LINK(entry);
        HTableValue value = HTableEntry_VALUE(entry);
        if (release != NULL) (*release)(value);
        Memory_free_(memory, entry);
        entry = link;
      }
      buckets[i] = NULL;
    }
  }
  *HTable__COUNT(this) = 0;
  Except_CHECK(HTable_isEmpty(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This HTable Memory.
 */
static inline Memory
HTable_memory(const_HTable this)
{
  return HTable_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @retval True iff this HTable is empty.
 */
#define HTable_isEmpty(this)	(HTable_count(this) == 0)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @retval True iff this HTable has a single entry.
 */
#define HTable_isSingle(this)	(HTable_count(this) == 1)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count entries in the HTable.
 * @return the count of entries in the HTable.
 */
static inline int32_t
HTable_count(const_HTable this)
{
  return HTable_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by HTable_FOREACH.
 */
static inline uint32_t
HTable_mask(const_HTable this)
{
  return HTable_MASK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by HTable_FOREACH.
 * @return this HTable_ENTRIES.
 */
static inline HTableEntry *
HTable_buckets(const_HTable this)
{
  return HTable_BUCKETS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates over the HTable entries.
 *
 * @param key	variable set for each key in this HTable.
 * @param Type	the @c value type.
 * @param iter	pointer set to each value associated to @p key.
 *
 * @note Exiting HTable_FOREACH with @c break or @c return is allowed.
 * @note Calling HTable_remove on the current entry is allowed.
 */
#define HTable_FOREACH(this, key, Type, iter) { \
  HTableEntry *HTable_BUCKETS = HTable_buckets(this); \
  int32_t HTable_MASK = HTable_mask(this), HTable_INDEX; \
  for (HTable_INDEX = 0; HTable_INDEX <= HTable_MASK; HTable_INDEX++) { \
    HTableEntry HTable_ENTRY = HTable_BUCKETS[HTable_INDEX], HTableEntry_LINK; \
    for (; HTable_ENTRY != NULL; HTable_ENTRY = HTableEntry_LINK) { \
      __attribute__ ((unused)) HTableKey key = HTableEntry_key(HTable_ENTRY); \
      Type *(iter) = (Type *)HTableEntry_value(HTable_ENTRY); \
      HTableEntry_LINK = HTableEntry_link(HTable_ENTRY);
#define HTable_ENDEACH \
    } \
    if (HTable_ENTRY != NULL) break; \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Search the value associated to @p key.
 * @return	the associated value if @p key is in HTable, else NULL.
 */
HTableValue
HTable_search(const_HTable this, HTableKey key);
#endif//$XCC_h

HTableValue
HTable_search(const_HTable this, HTableKey key)
{
  HTableEntry *buckets = HTable_BUCKETS(this);
  uint32_t hash = HTable_Hash(key);
  uint32_t mask = HTable_MASK(this);
  int32_t index = hash & mask;
  HTableEntry entry = buckets[index];
  while (entry != NULL) {
    HTableEntry link = HTableEntry_LINK(entry);
    if (HTableEntry_KEY(entry) == key) {
      return HTableEntry_VALUE(entry);
    }
    entry = link;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Generic lookup @c key and insert if not found.
 * @return	a pointer to the associated value (never return NULL).
 *
 * @note	In case of insertion, the new value is left uninitialized.
 * @sa HTable_lookup.
 */
HTableValue
HTable_lookup_(HTable this, HTableKey key, size_t valueSize);
#endif//$XCC_h

HTableValue
HTable_lookup_(HTable this, HTableKey key, size_t valueSize)
{
  Memory memory = HTable_MEMORY(this);
  HTableEntry *buckets = HTable_BUCKETS(this);
  uint32_t hash = HTable_Hash(key);
  uint32_t mask = HTable_MASK(this);
  int32_t count = HTable_COUNT(this);
  int32_t index = hash & mask;
  HTableEntry entry = buckets[index];
  while (entry != NULL) {
    HTableEntry link = HTableEntry_LINK(entry);
    if (HTableEntry_KEY(entry) == key) {
      return HTableEntry_VALUE(entry);
    }
    entry = link;
  }
  entry = Memory_alloc(memory, HTableEntry_Size(key, valueSize, buckets[index]));
  HTableEntry_Ctor(entry, key, valueSize, buckets[index]);
  buckets[index] = entry;
  *HTable__COUNT(this) = count + 1;
  if (count > mask) HTable_resize(this, count + 1);
  return HTableEntry_VALUE(entry);
}

#ifdef $XCC_h
/**
 * Lookup @p key and insert if not found.
 * @return	a pointer to the associated value (never return NULL).
 *
 * @note	In case of insertion, the new value is zeroed.
 */
static inline HTableValue
HTable_lookup(HTable this, HTableKey key, size_t valueSize)
{
  int32_t count = HTable_count(this);
  HTableValue value = HTable_lookup_(this, key, valueSize);
  if (count != HTable_count(this)) memset(value, 0, valueSize);
  return value;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Insert @p key and return the associated value.
 * @return	a pointer to the associated value if @p key was
 * inserted in HTable, NULL if @p key is already in HTable.
 */
HTableValue
HTable_insert(HTable this, HTableKey key, size_t valueSize);
#endif//$XCC_h

HTableValue
HTable_insert(HTable this, HTableKey key, size_t valueSize)
{
  Memory memory = HTable_MEMORY(this);
  HTableEntry *buckets = HTable_BUCKETS(this);
  uint32_t hash = HTable_Hash(key);
  uint32_t mask = HTable_MASK(this);
  int32_t count = HTable_COUNT(this);
  int32_t index = hash & mask;
  HTableEntry entry = buckets[index];
  while (entry != NULL) {
    HTableEntry link = HTableEntry_LINK(entry);
    if (HTableEntry_KEY(entry) == key) {
      return NULL;
    }
    entry = link;
  }
  entry = Memory_alloc(memory, HTableEntry_Size(key, valueSize, buckets[index]));
  HTableEntry_Ctor(entry, key, valueSize, buckets[index]);
  buckets[index] = entry;
  *HTable__COUNT(this) = count + 1;
  if (count > mask) HTable_resize(this, count + 1);
  return HTableEntry_VALUE(entry);
}

#ifdef $XCC_h
/**
 * Specialized HTable_remove() that does not release the HTableEntry.
 */
HTableEntry
HTable_remove_(HTable this, HTableKey key);
#endif//$XCC_h

HTableEntry
HTable_remove_(HTable this, HTableKey key)
{
  HTableEntry *buckets = HTable_BUCKETS(this);
  uint32_t hash = HTable_Hash(key);
  uint32_t mask = HTable_MASK(this);
  int32_t index = hash & mask;
  HTableEntry entry = buckets[index], prev_entry = NULL;
  while (entry != NULL) {
    HTableEntry link = HTableEntry_LINK(entry);
    HTableValue value = HTableEntry_VALUE(entry);
    if (HTableEntry_KEY(entry) == key) {
      if (prev_entry != NULL) {
        *HTableEntry__LINK(prev_entry) = link;
      } else {
        buckets[index] = link;
      }
      --*HTable__COUNT(this);
      return entry;
    }
    prev_entry = entry;
    entry = link;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Remove @p key and the associated value.
 * @retval true	if @c key was in HTable
 * @retval false otherwise.
 */
static inline bool
HTable_remove(HTable this, HTableKey key, HTableValueRelease release)
{
  Memory memory = HTable_MEMORY(this);
  HTableEntry entry = HTable_remove_(this, key);
  if (entry != NULL) {
    HTableValue value = HTableEntry_VALUE(entry);
    if (release != NULL) (*release)(value);
    Memory_free_(memory, entry);
    return true;
  }
  return false;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Map the HTable entries using the supplied @p map function.
 *
 * @param map	The function that will be called for each entry in the HTable.
 *              First argument of @p (*map) is the current key. Second argument
 *              of @p (*map) is the @c void* value associated to the current key.
 *              Third argument of @p (*map) is the @c va_list corresponding to the
 *              "..." arguments of HTable_map.
 * @param ...	The other arguments (if any) to be passed to @p (*map) as @c va_list.
 */
void
HTable_map(HTable this, void (*map)(HTableKey, HTableValue, va_list), ...);
#endif//$XCC_h

void
HTable_map(HTable this, void (*map)(HTableKey, HTableValue, va_list), ...)
{
  va_list va;
  HTable_FOREACH(this, key, void, value) {
    va_start(va, map);
    (*map)(key, value, va);
    va_end(va);
  } HTable_ENDEACH;
}

#ifdef $XCC__c
{
  long int i, j, k;
  HTable_ htable[1];
  HTable_Ctor(htable, Memory_Root, 10);
#define NumItems 500
  printf("Insert:\n");
  for (i = 0, j = 0; i < NumItems; i++, j = (j + 7)%NumItems) {
    *(long int *)HTable_insert(htable, (HTableKey)(j), sizeof(int *)) = j;
    k = *(long int *)HTable_search(htable, (HTableKey)(j));
    if (k != j) printf("Error0 at %ld\n", j);
  }
  printf("Entry count = %d\n", HTable_COUNT(htable));
  for (j = 0; j < 2; j++) {
    for (i = 0; i < NumItems; i++) {
      k = *(int *)HTable_search(htable, (HTableKey)(i));
      if (k != i) printf("Error1 at %ld\n", i);
    }
  }
  printf("Remove\n");
  for (i = 0; i < NumItems; i += 2) {
    if (HTable_remove(htable, (HTableKey)(i), NULL));
    else printf("Error3 at %ld\n", i);
  }
  for (i = 1; i < NumItems; i += 2) {
    k = *(int *)HTable_search(htable, (HTableKey)(i));
    if (k != i) printf("Error4 at %ld\n", i);
  }
  for (i = 0; i < NumItems; i += 2) {
    if (HTable_search(htable, (HTableKey)(i)) != NULL) {
      printf("Error5 at %ld\n", i);
    }
  }
  {
    int array[NumItems], index = 0;
    HTable_map(htable, HTable_VAL, array, &index);
    qsort((char *) array, index, sizeof(int), HTable_compare);
    printf("[ ");
    for (i = 0; i < index; i++) {
      printf("%d ", array[i]);
    }
    printf("]\n");
  }
  printf("Entry count = %d\n", HTable_COUNT(htable));
  for (i = NumItems - 1; i > 0; i -= 2) {
    if (HTable_remove(htable, (HTableKey)(i), NULL));
    else printf("Error6 at %ld\n", i);
  }
  printf("Node count = %d\n", HTable_COUNT(htable));
  HTable_Dtor(htable);
}
#endif//$XCC__c

#ifdef $XCC__c
{
  long int i, j;
  HTable_ htable[1];
  HTable_Ctor(htable, Memory_Root, 10);
  for (i = 0, j = 0; i < NumItems; i++, j = (j + 7)%NumItems) {
    HTable_insert(htable, (HTableKey)(j), 0);
  }
  for (i = 0; i < NumItems; i += 2) {
    HTable_remove(htable, (HTableKey)(i), NULL);
  }
  HTable_Dtor(htable);
}
#endif//$XCC__c

#if XCC__C
static int
HTable_compare(HTableKey a, HTableKey b)
{
  return (*(const int *)a > *(const int *)b) -
      (*(const int *)a < *(const int *)b);
}

static void
HTable_VAL(HTableKey key, HTableValue value, va_list va)
{
  int *array = va_arg(va, int *);
  int *index = va_arg(va, int *);
  array[*index] = *(int *)value;
  ++*index;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
  HTable_INIT -- Initialize the HTable module.
 */
#define HTable_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/*
  HTable_FINI -- Finalize the HTable module.
 */
#define HTable_FINI()
#endif//$XCC__h

#if XCC__C
static void
HTable_TEST(void)
{
#include "CCL/HTable_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(HTable_TEST);
  return 0;
}
#endif

