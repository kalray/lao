#ifdef $XCC_h
/*
 * !!!!	Memory.xcc
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
 *
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup BSL
 * @brief Memory allocator used by all the CDT containers and data structures.
 *
 * It basically provides the "reap" interface proposed by "Reconsidering Custom
 * Memory Allocation" by E. D. Berger, B. G. Zorn, K. S. McKinley. This is done
 * on top of the system malloc(), realloc(), and free().
 *
 * The "reap" interface provides a 'freeAll' method to free all the memory of a
 * Memory allocator at once, and also supports parent-child relationships between
 * Memory allocators so that deleting one recursively deletes its children.
 *
 * In addition to the "reap" interface, we enable memory pooling of small-sized
 * allocation requests. Pooling is enabled by a boolean flag supplied to Memory_new.
 * Currently pooled memory is only released when freeAll or Memory_delete is called.
 */
#endif//$XCC_h

#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/Memory.h"
#endif//$XCC__h

#ifdef $XCC__h
/**
 * The Memory pool block size.
 */
//#define Memory_BLOCKSIZE 4096
#ifndef Memory_BLOCKSIZE
extern int Memory_BLOCKSIZE;
#endif//Memory_BLOCKSIZE
#endif//$XCC__h

#ifndef Memory_BLOCKSIZE
int Memory_BLOCKSIZE = 4096;
#endif//Memory_BLOCKSIZE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#define MemoryUsage_SMALLCOUNT 1024/_ALIGN_MAX
#define MemoryUsage_LARGECOUNT 32-10			// log(2^32 / 1024).
#endif//$XCC_h

#ifdef $XCC_h
struct MemoryUsageRecord_ {
  //@args
  uint32_t OPENED;		// Number of opened allocations in this size range.
  uint32_t ALLOCS;		// Total number of allocations in this size range.
};
#endif//$XCC_h

#ifdef $XCC_h
/**
 * State to track memory usage.
 */
struct MemoryUsage_ {
  //@args
  uint32_t INUSE;		// Current memory space in use.
  uint32_t MAXUSE;		// Maximum memory space used.
  uint32_t OPENED;		// Current number of allocations in all size ranges.
  uint32_t ALLOCS;		// Total number of allocations in all size ranges.
  MemoryUsageRecord_ SMALLRECORDS[MemoryUsage_SMALLCOUNT];
  MemoryUsageRecord_ LARGERECORDS[MemoryUsage_LARGECOUNT];
};
#endif//$XCC_h

MemoryUsage
MemoryUsage_Ctor(MemoryUsage this)
{
  memset(this, 0, sizeof(MemoryUsage_));
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(MemoryUsage_)\t%zu\n", sizeof(MemoryUsage_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Pretty-print this MemoryUsage.
 */
bool
MemoryUsage_pretty(const_MemoryUsage this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
MemoryUsage_pretty(const_MemoryUsage this, FILE *file)
{
  double sofar = 0.0;
  double total = MemoryUsage_ALLOCS(this);
  if (file == NULL) file = stderr;
  fprintf(file, "*** MemoryUsage\tINUSE=%u\tMAXUSE=%u\tALLOCS=%u\n",
          MemoryUsage_INUSE(this), MemoryUsage_MAXUSE(this), MemoryUsage_ALLOCS(this));
  {
    uint32_t size;
    for (size = 1; size <= 1024; size += _ALIGN_MAX) {
      int index = (size + _ALIGN_MAX - 1)/_ALIGN_MAX - 1;
      const_MemoryUsageRecord record = MemoryUsage_SMALLRECORDS(this) + index;
      int opened = MemoryUsageRecord_OPENED(record);
      int allocs = MemoryUsageRecord_ALLOCS(record);
      if (allocs != 0) {
        double percent = (sofar += allocs)/total;
        fprintf(file, "    %u < size <= %u\tallocs=%u\tcumul=%2.0f%%",
                size - 1, size + _ALIGN_MAX - 1, allocs, percent*100);
        fprintf(file, opened != 0? "\topened=%u\n": "\n", opened);
      }
    }
  }
  {
    int index;
    for (index = 0; index < 32-10; index++) {
      uint32_t size = 1024<<index;
      const_MemoryUsageRecord record = MemoryUsage_LARGERECORDS(this) + index;
      int opened = MemoryUsageRecord_OPENED(record);
      int allocs = MemoryUsageRecord_ALLOCS(record);
      if (allocs != 0) {
        double percent = (sofar += allocs)/total;
        fprintf(file, "    %u <= size < %u\tallocs=%u\tcumul=%2.0f%%",
                size, size<<1, allocs, percent*100);
        fprintf(file, opened != 0? "\topened=%u\n": "\n", opened);
      }
    }
  }
  return true;
}
#endif//!defined(_NTRACE) || defined(XCC__C)

static MemoryUsageRecord
MemoryUsage_getRecord(MemoryUsage this, uint32_t size)
{
  if (size <= 1024) {
    int index = (size + _ALIGN_MAX - 1)/_ALIGN_MAX - 1;
    Except_CHECK(index >= 0 && index < MemoryUsage_SMALLCOUNT);
    return MemoryUsage_SMALLRECORDS(this) + index;
  } else {
    int index = -1;
    for (size >>= 10; size != 0; index++, size >>= 1);
    Except_CHECK(index >= 0 && index < MemoryUsage_LARGECOUNT);
    return MemoryUsage_LARGERECORDS(this) + index;
  }
}

#ifdef $XCC_h
/**
 * Trace usage of @c malloc.
 */
void
MemoryUsage_alloc(MemoryUsage this, uint32_t size);
#endif//$XCC_h

void
MemoryUsage_alloc(MemoryUsage this, uint32_t size)
{
  uint32_t inUse = MemoryUsage_INUSE(this);
  uint32_t maxUse = MemoryUsage_MAXUSE(this);
  MemoryUsageRecord record = MemoryUsage_getRecord(this, size);
  *MemoryUsage__INUSE(this) = (inUse += size);
  if (inUse > maxUse) *MemoryUsage__MAXUSE(this) = inUse;
  ++*MemoryUsageRecord__OPENED(record);
  ++*MemoryUsageRecord__ALLOCS(record);
  ++*MemoryUsage__OPENED(this);
  ++*MemoryUsage__ALLOCS(this);
}

#ifdef $XCC_h
/**
 * Trace usage of @c free.
 */
void
MemoryUsage_free(MemoryUsage this, uint32_t size);
#endif//$XCC_h

void
MemoryUsage_free(MemoryUsage this, uint32_t size)
{
  uint32_t inUse = MemoryUsage_INUSE(this);
  MemoryUsageRecord record = MemoryUsage_getRecord(this, size);
  *MemoryUsage__INUSE(this) = (inUse -= size);
  --*MemoryUsageRecord__OPENED(record);
  --*MemoryUsage__OPENED(this);
}

#ifdef $XCC__c
if (0) { // Different output on 64-bits arch.
  MemoryUsage_ usage[1];
  MemoryUsage_Ctor(usage);
  MemoryUsage_alloc(usage, 1);
  MemoryUsage_alloc(usage, _ALIGN_MAX);
  MemoryUsage_alloc(usage, _ALIGN_MAX+1);
  MemoryUsage_alloc(usage, 3*_ALIGN_MAX-1);
  MemoryUsage_alloc(usage, 3*_ALIGN_MAX);
  MemoryUsage_alloc(usage, 3*_ALIGN_MAX+1);
  MemoryUsage_alloc(usage, 1024-1);
  MemoryUsage_alloc(usage, 1024);
  MemoryUsage_alloc(usage, 1024+1);
  MemoryUsage_alloc(usage, 1024+_ALIGN_MAX);
  MemoryUsage_alloc(usage, 1024+_ALIGN_MAX+1);
  MemoryUsage_alloc(usage, 2096);
  MemoryUsage_alloc(usage, (uint32_t)INT32_MAX);
  MemoryUsage_alloc(usage, (uint32_t)INT32_MAX+1);
  MemoryUsage_alloc(usage, (uint32_t)UINT32_MAX);
  MemoryUsage_pretty(usage, stdout);
}
#endif//$XCC__c

#ifdef $XCC_h
extern MemoryUsage Memory_Usage;
#endif//$XCC_h

MemoryUsage_ Memory_Usage_, *Memory_Usage = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Fields needed by the items of a MemoryList.
 */
struct MemoryListItem_ {
  //@args
  struct MemoryListItem_ *NEXT;
  struct MemoryListItem_ *PREV;
};
#endif//$XCC_h

MemoryListItem
MemoryListItem_Ctor(MemoryListItem this)
{
  *MemoryListItem__NEXT(this) = NULL;
  *MemoryListItem__PREV(this) = NULL;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(MemoryListItem_)\t%zu\n", sizeof(MemoryListItem_));
  Except_ALWAYS(_ALIGNED(sizeof(MemoryListItem_)));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Manage a list of items derived from MemoryListItem.
 */
struct MemoryList_ {
  //@args
  struct MemoryListItem_ *FIRST;
  struct MemoryListItem_ *LAST;
};
#endif//$XCC_h

MemoryList
MemoryList_Ctor(MemoryList this)
{
  *MemoryList__FIRST(this) = NULL;
  *MemoryList__LAST(this) = NULL;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(MemoryList_)\t%zu\n", sizeof(MemoryList_));
  Except_ALWAYS(_ALIGNED(sizeof(MemoryList_)));
}
#endif//$XCC__c

#ifdef $XCC_h
/*
 * MemoryList_first -- For use by MemoryList_FOREACH_MemoryListItem.
 */
static inline MemoryListItem
MemoryList_first(MemoryList this)
{
  return MemoryList_FIRST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
bool
MemoryList_check(MemoryList this, MemoryListItem item);
#endif//$XCC_h

bool
MemoryList_check(MemoryList this, MemoryListItem item)
{
  MemoryListItem first = MemoryList_FIRST(this);
  MemoryListItem last = MemoryList_LAST(this);
  if (BSL_TRACE)
    fprintf(BSL_TRACE, "MemoryList %p<%p,%p>:", this, first, last);
  if (first ==  NULL) {
    Except_ALWAYS(last == NULL);
    Except_ALWAYS(item == NULL);
  } else {
    int found = 0;
    MemoryListItem curr = first, prev = NULL;
    while (curr != NULL) {
      if (curr == item) found++;
      if (BSL_TRACE)
        fprintf(BSL_TRACE, "\t%p<%p,%p>",
                curr, MemoryListItem_PREV(curr), MemoryListItem_NEXT(curr));
      Except_ALWAYS(MemoryListItem_PREV(curr) == prev);
      prev = curr;
      curr = MemoryListItem_NEXT(curr);
    }
    if (item != NULL) Except_ALWAYS(found == 1);
    Except_ALWAYS(prev == last);
  }
  if (BSL_TRACE)
    fprintf(BSL_TRACE, "\n");
  return true;
}

#ifdef $XCC_h
/**
 * Iterator.
 */
#define MemoryList_FOREACH_MemoryListItem(this, item) { \
  MemoryListItem MemoryList_NEXT = NULL; \
  MemoryListItem item = MemoryList_first(this); \
  for (; item != NULL; item = MemoryList_NEXT) { \
    MemoryList_NEXT = MemoryListItem_NEXT(item);
#define MemoryList_ENDEACH_MemoryListItem \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Append item to this MemoryList.
 */
void
MemoryList_append(MemoryList this, MemoryListItem item);
#endif//$XCC_h

void
MemoryList_append(MemoryList this, MemoryListItem item)
{
  MemoryListItem first = MemoryList_FIRST(this);
  MemoryListItem last = MemoryList_LAST(this);
  Except_DEBUG(MemoryList_check(this, NULL));
  BSL_TRACE && fprintf(BSL_TRACE, "MemoryList_append(%p, %p)\n", this, item);
  if (last != NULL) *MemoryListItem__NEXT(last) = item;
  if (first == NULL) *MemoryList__FIRST(this) = item;
  *MemoryListItem__PREV(item) = last;
  *MemoryListItem__NEXT(item) = NULL;
  *MemoryList__LAST(this) = item;
}

#ifdef $XCC_h
/**
 * Remove item from this MemoryList.
 */
void
MemoryList_remove(MemoryList this, MemoryListItem item);
#endif//$XCC_h

void
MemoryList_remove(MemoryList this, MemoryListItem item)
{
  MemoryListItem first = MemoryList_FIRST(this);
  MemoryListItem last = MemoryList_LAST(this);
  MemoryListItem prev = MemoryListItem_PREV(item);
  MemoryListItem next = MemoryListItem_NEXT(item);
  Except_DEBUG(MemoryList_check(this, item));
  BSL_TRACE && fprintf(BSL_TRACE, "MemoryList_remove(%p, %p)\n", this, item);
  if (item == first) {
    *MemoryList__FIRST(this) = next;
    if (next == NULL) {
      *MemoryList__LAST(this) = NULL;
    } else {
      *MemoryListItem__PREV(next) = NULL;
    }
  } else if (item == last) {
    *MemoryList__LAST(this) = prev;
    if (prev == NULL) {
      *MemoryList__FIRST(this) = NULL;
    } else {
      *MemoryListItem__NEXT(prev) = NULL;
    }
  } else {
    *MemoryListItem__PREV(next) = prev;
    *MemoryListItem__NEXT(prev) = next;
    *MemoryListItem__PREV(item) = NULL;
    *MemoryListItem__NEXT(item) = NULL;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Entry in a MemoryTable.
 */
struct MemoryTableEntry_ {
  //@args	void *start, size_t useSize, int level,
  //@args	struct MemoryTableEntry_ *leaf
  size_t USESIZE;			// Size of memory available for use.
  uint32_t LEVEL;			// Level to keep the binary search tree balanced.
  int32_t REFCOUNT;			// Reference counter of this MemoryTableEntry_.
  struct MemoryTableEntry_ *LEFT;	// The left kid in the binary search tree.
  struct MemoryTableEntry_ *RIGHT;	// The right kid in the binary search tree.
  char *START;				// The start memory address.
  //@access PAST	(MemoryTableEntry_START(this) + MemoryTableEntry_USESIZE(this))
  //@access BLOCK	(MemoryBlock)((MemoryListItem)(this) - 1)
};
#endif//$XCC_h

MemoryTableEntry
MemoryTableEntry_Ctor(MemoryTableEntry this, void *start, size_t useSize, int level,
                      struct MemoryTableEntry_ *leaf)
{
  *MemoryTableEntry__USESIZE(this) = useSize;
  *MemoryTableEntry__LEVEL(this) = level;
  *MemoryTableEntry__REFCOUNT(this) = 0;
  *MemoryTableEntry__LEFT(this) = leaf;
  *MemoryTableEntry__RIGHT(this) = leaf;
  *MemoryTableEntry__START(this) = start;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(MemoryTableEntry_)\t%zu\n", sizeof(MemoryTableEntry_));
  Except_ALWAYS(_ALIGNED(sizeof(MemoryTableEntry_)));
}
#endif//$XCC__c

static inline MemoryTableEntry
MemoryTableEntry_skew(MemoryTableEntry this)
{
  MemoryTableEntry t = this;
  MemoryTableEntry left = MemoryTableEntry_LEFT(t);
  if (MemoryTableEntry_LEVEL(left) == MemoryTableEntry_LEVEL(t)) {
    // Rotate right.
    MemoryTableEntry temp = t;
    t = left;
    *MemoryTableEntry__LEFT(temp) = MemoryTableEntry_RIGHT(t);
    *MemoryTableEntry__RIGHT(t) = temp;
#ifdef XCC__C
    printf("skew\tuseSize=%zu\n", MemoryTableEntry_USESIZE(this));
#endif//XCC__C
  }
  return t;
}

static inline MemoryTableEntry
MemoryTableEntry_split(MemoryTableEntry this)
{
  MemoryTableEntry t = this;
  MemoryTableEntry right = MemoryTableEntry_RIGHT(t);
  MemoryTableEntry right_right = MemoryTableEntry_RIGHT(right);
  if (MemoryTableEntry_LEVEL(right_right) == MemoryTableEntry_LEVEL(t)) {
    // Rotate left.
    MemoryTableEntry temp = t;
    t = right;
    *MemoryTableEntry__RIGHT(temp) = MemoryTableEntry_LEFT(t);
    *MemoryTableEntry__LEFT(t) = temp;
    ++*MemoryTableEntry__LEVEL(t);
#ifdef XCC__C
    printf("split\tuseSize=%zu\n", MemoryTableEntry_USESIZE(this));
#endif//XCC__C
  }
  return t;
}

static MemoryTableEntry
MemoryTableEntry_insert(MemoryTableEntry this, MemoryTableEntry entry,
                        MemoryTableEntry leaf)
{
  MemoryTableEntry t = this;
  const char *entry_start = MemoryTableEntry_START(entry);
  const char *start = MemoryTableEntry_START(t);
  const char *past = MemoryTableEntry_PAST(t);
  //printf("entry %p useSize=%u\n", this, MemoryTableEntry_USESIZE(this));
  if (entry_start < start) {
    MemoryTableEntry left = MemoryTableEntry_LEFT(t);
    if (left != leaf) {
      left = MemoryTableEntry_insert(left, entry, leaf);
    } else {
      left = entry;
    }
    *MemoryTableEntry__LEFT(t) = left;
  } else if (entry_start >= past) {
    MemoryTableEntry right = MemoryTableEntry_RIGHT(t);
    if (right != leaf) {
      right = MemoryTableEntry_insert(right, entry, leaf);
    } else {
      right = entry;
    }
    *MemoryTableEntry__RIGHT(t) = right;
  } else Except_NEVER(true);
  t = MemoryTableEntry_skew(t);
  t = MemoryTableEntry_split(t);
  return t;
}

#if !defined(_NTRACE) || defined(XCC__C)
static bool
MemoryTableEntry_pretty(MemoryTableEntry this, FILE *file)
{
  int level = MemoryTableEntry_LEVEL(this), i;
  MemoryTableEntry left = MemoryTableEntry_LEFT(this);
  MemoryTableEntry right = MemoryTableEntry_RIGHT(this);
  if (MemoryTableEntry_LEVEL(left) != 0) MemoryTableEntry_pretty(left, file);
  fprintf(file, "\tuseSize=%zu", MemoryTableEntry_USESIZE(this));
  for (i = 0; i < level; i++) fprintf(file, "  ");
  fprintf(file, "level=%u\n", level);
  if (MemoryTableEntry_LEVEL(right) != 0) MemoryTableEntry_pretty(right, file);
  return true;
}
#endif//!defined(_NTRACE) || defined(XCC__C)

#ifdef $XCC__c
{
  int i;
  MemoryTableEntry entries[16];
  MemoryTableEntry_ entries_[16];
  for (i = 0; i < 16; i++) {
    entries[i] = entries_ + i;
  }
  MemoryTableEntry_Ctor(entries[0], entries[0]+1, 0, 0, entries[0]);
  for (i = 1; i < 16; i++) {
    MemoryTableEntry_Ctor(entries[i], entries[i]+1, i, 1, entries[0]);
  }
  *MemoryTableEntry__RIGHT(entries[5]) = entries[7];
  *MemoryTableEntry__LEVEL(entries[2]) = 2;
  *MemoryTableEntry__LEFT(entries[2]) = entries[1];
  *MemoryTableEntry__RIGHT(entries[2]) = entries[3];
  *MemoryTableEntry__LEVEL(entries[8]) = 2;
  *MemoryTableEntry__LEFT(entries[8]) = entries[5];
  *MemoryTableEntry__RIGHT(entries[8]) = entries[9];
  *MemoryTableEntry__LEVEL(entries[12]) = 2;
  *MemoryTableEntry__LEFT(entries[12]) = entries[11];
  *MemoryTableEntry__RIGHT(entries[12]) = entries[13];
  *MemoryTableEntry__LEVEL(entries[4]) = 3;
  *MemoryTableEntry__LEFT(entries[4]) = entries[2];
  *MemoryTableEntry__RIGHT(entries[4]) = entries[10];
  *MemoryTableEntry__LEVEL(entries[10]) = 3;
  *MemoryTableEntry__LEFT(entries[10]) = entries[8];
  *MemoryTableEntry__RIGHT(entries[10]) = entries[12];
  printf("Before insert(6):\n");
  MemoryTableEntry_pretty(entries[4], stdout);
  MemoryTableEntry_insert(entries[4], entries[6], entries[0]);
  printf("After insert(6):\n");
  MemoryTableEntry_pretty(entries[4], stdout);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Maps addresses to MemoryBlock(s).
 *
 * Currently the MemoryTable only allows search and insert operations.
 *
 * Uses a balanced search tree as described in "Balanced Search Trees Made Simple"
 * by Arne Andersson (http://user.it.uu.se/~arnea/research.html).
 */
struct MemoryTable_ {
  //@args	void *memory
  MemoryTableEntry_ LEAF_;
  //@access LEAF	MemoryTable__LEAF_(this)
  MemoryTableEntry ROOT;
};
#endif//$XCC_h

MemoryTable
MemoryTable_Ctor(MemoryTable this, void *memory)
{
  MemoryTableEntry leaf = MemoryTable_LEAF(this);
  MemoryTableEntry_Ctor(MemoryTable_LEAF(this), NULL, 0, 0, leaf);
  *MemoryTable__ROOT(this) = leaf;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(MemoryTable_)\t%zu\n", sizeof(MemoryTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Search this MemoryTable for the MemoryBlock containing @c address.
 */
struct MemoryBlock_ *
MemoryTable_search(const_MemoryTable this, const void *address);
#endif//$XCC_h

struct MemoryBlock_ *
MemoryTable_search(const_MemoryTable this, const void *address)
{
  const char *pointer = address;
  const_MemoryTableEntry t = MemoryTable_ROOT(this);
  const_MemoryTableEntry leaf = MemoryTable_LEAF(this);
  while (t != leaf) {
    const char *start = MemoryTableEntry_START(t);
    const char *past = MemoryTableEntry_PAST(t);
    if (pointer < start) t = MemoryTableEntry_LEFT(t);
    else if (pointer >= past) t = MemoryTableEntry_RIGHT(t);
    else return MemoryTableEntry_BLOCK(t);
  }
  return NULL;
}

#ifdef $XCC__h
/**
 * Insert a MemoryTableEntry into this MemoryTable.
 */
void
MemoryTable_insert(MemoryTable this, MemoryTableEntry entry);
#endif//$XCC__h

void
MemoryTable_insert(MemoryTable this, MemoryTableEntry entry)
{
  MemoryTableEntry root = MemoryTable_ROOT(this);
  MemoryTableEntry leaf = MemoryTable_LEAF(this);
  Except_REQUIRE(MemoryTableEntry_LEFT(entry) == leaf);
  Except_REQUIRE(MemoryTableEntry_RIGHT(entry) == leaf);
  Except_REQUIRE(MemoryTableEntry_LEVEL(entry) == 1);
  if (root == leaf) {
    *MemoryTable__ROOT(this) = entry;
  } else {
    *MemoryTable__ROOT(this) = MemoryTableEntry_insert(root, entry, leaf);
  }
}

#ifdef $XCC__c
if(0) { // Test behaviour different on 32-bits & 64-bits architectures
  int i;
  MemoryTable_ table[1];
  MemoryBlock_ blocks_[128];
  const char *address = (const char *)blocks_;
  MemoryTable_Ctor(table, NULL);
  for (i = 0; i < 128; i++) {
    MemoryBlock_Ctor(blocks_ + i, 2 + (i&1? 1: -1), MemoryTable_LEAF(table));
  }
  for (i = 0; i < 128; i += 2) {
    MemoryTable_insert(table, MemoryBlock_ENTRY(blocks_ + i));
    MemoryTable_insert(table, MemoryBlock_ENTRY(blocks_ + 128 - i - 1));
  }
  MemoryTableEntry_pretty(MemoryTable_ROOT(table), stdout);
  for (; address < (const char *)(blocks_ + 128); address++) {
    MemoryBlock block = MemoryTable_search(table, address);
    if (block != NULL) {
      printf("address=%zu\tblock=%zu\n", address - (const char *)blocks_, block - blocks_);
    }
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Block of memory.
 */
struct MemoryBlock_ {
  //@args	size_t useSize, MemoryTableEntry leaf
  MemoryListItem_ ITEM_;
  //@access ITEM	MemoryBlock__ITEM_(this)
  //@access NEXT	(MemoryBlock)MemoryListItem_NEXT(MemoryBlock_ITEM(this))
  //@access PREV	(MemoryBlock)MemoryListItem_PREV(MemoryBlock_ITEM(this))
  MemoryTableEntry_ ENTRY_;
  //@access ENTRY	MemoryBlock__ENTRY_(this)
  //@access REALSIZE	(sizeof(MemoryBlock_) + MemoryBlock_USESIZE(this))
  //@access USESIZE	MemoryTableEntry_USESIZE(MemoryBlock_ENTRY(this))
  //@mutate USESIZE	MemoryTableEntry__USESIZE(MemoryBlock_ENTRY(this))
  //@access START	MemoryTableEntry_START(MemoryBlock_ENTRY(this))
  //@mutate START	MemoryTableEntry__START(MemoryBlock_ENTRY(this))
  //@access PAST	MemoryTableEntry_PAST(MemoryBlock_ENTRY(this))
  //@access REFCOUNT	MemoryTableEntry_REFCOUNT(MemoryBlock_ENTRY(this))
  //@mutate REFCOUNT	MemoryTableEntry__REFCOUNT(MemoryBlock_ENTRY(this))
};
#endif//$XCC_h

MemoryBlock
MemoryBlock_Ctor(MemoryBlock this, size_t useSize, MemoryTableEntry leaf)
{
  char *start = (char *)(this + 1);
  MemoryListItem_Ctor(MemoryBlock_ITEM(this));
  MemoryTableEntry_Ctor(MemoryBlock_ENTRY(this), start, useSize, 1, leaf);
  return this;
}

#ifdef $XCC__c
{
  MemoryBlock_ block[1];
  fprintf(stderr, "sizeof(MemoryBlock_)\t%zu\n", sizeof(MemoryBlock_));
  Except_ALWAYS(_ALIGNED(sizeof(MemoryBlock_)));
  Except_ALWAYS(block == MemoryTableEntry_BLOCK(MemoryBlock_ENTRY(block)));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Memory -- Memory allocator.
 */
struct Memory_ {
  //@args	Memory parent, bool pooled
  MemoryListItem_ ITEM_;	// Used to embed this Memory in a MemoryList.
  //@access ITEM	Memory__ITEM_(this)
  //@access NEXT	(Memory)MemoryListItem_NEXT(Memory_ITEM(this))
  //@access PREV	(Memory)MemoryListItem_PREV(Memory_ITEM(this))
  struct Memory_ *PARENT;	// Parent in the Memory parent-child relationships.
  size_t POOLSIZE;		// Maximum alloc size for request to be pooled.
  size_t TOTALSIZE;		// Total allocated size (for statistics).
  size_t TOTALCOUNT;		// Total allocated count (for statistics).
  MemoryBlock POOLBLOCK;	// Currently active pool block.
  char *POOLFIRST;		// First available address in pooling block.
  char *POOLPAST;		// Past available address in pooling block.
  MemoryList_ CHILDREN[1];	// Children of this Memory allocator.
  MemoryList_ BLOCKS[1];	// MemoryBlock(s) managed by this Memory allocator.
  //@access TABLE	(MemoryTable)(this + 1)
};
#endif//$XCC_h

Memory
Memory_Ctor(Memory this, Memory parent, bool pooled)
{
  size_t poolSize = Memory_BLOCKSIZE - sizeof(MemoryBlock_);
  Except_CHECK(Memory_BLOCKSIZE > sizeof(MemoryBlock_));
  MemoryListItem_Ctor(Memory_ITEM(this));
  *Memory__PARENT(this) = parent;
  *Memory__POOLSIZE(this) = pooled? poolSize: 0;
  *Memory__TOTALSIZE(this) = 0;
  *Memory__TOTALCOUNT(this) = 0;
  *Memory__POOLBLOCK(this) = NULL;
  *Memory__POOLFIRST(this) = NULL;
  *Memory__POOLPAST(this) = NULL;
  MemoryList_Ctor(Memory_CHILDREN(this));
  if (parent != NULL) {
    MemoryList_append(Memory_CHILDREN(parent), Memory_ITEM(this));
  }
  MemoryList_Ctor(Memory_BLOCKS(this));
  if (pooled) {
    MemoryTableEntry leaf = MemoryTable_LEAF(Memory_TABLE(this));
    *Memory__POOLBLOCK(this) = MemoryTableEntry_BLOCK(leaf);
    MemoryTable_Ctor(Memory_TABLE(this), this);
  }
  return this;
}

void
Memory_Dtor(Memory this)
{
  size_t poolSize = Memory_POOLSIZE(this);
  memset(this, -1, sizeof(Memory_) + (poolSize > 0? sizeof(MemoryTable_): 0));
}

size_t
Memory_Size(Memory parent, bool pooled)
{
  return sizeof(Memory_) + (pooled && Memory_BLOCKSIZE > 0? sizeof(MemoryTable_): 0);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Memory_)\t%zu\n", sizeof(Memory_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Make a new Memory allocator.
 *
 * @c parent:	The parent Memory allocator.
 * @c pooled:	If true, enable pooling in this Memory allocator.
 *
 * Memory allocators are organized in tree thanks to the @c parent parameter. When a
 * Memory allocator is deleted or does freeAll, it recursively deletes all its children.
 *
 * If @c pooled parameter is true, only allocation sizes beyond Memory_POOLSIZE are
 * directly forwarded to @c malloc, @c realloc, and @c free. Other requests are pooled by
 * the Memory allocator.
 */
Memory
Memory_new(Memory parent, bool pooled);
#endif//$XCC_h

Memory
Memory_new(Memory parent, bool pooled)
{
  bool force_pooled = FORCE_POOLED >= 0? (FORCE_POOLED > 0): pooled;
  size_t alignedSize = _ALIGN_NEXT(Memory_Size(parent, force_pooled));
  Memory this = Memory_alloc_(parent, alignedSize);
  return Memory_Ctor(this, parent, force_pooled);
}

#ifdef $XCC_h
/**
 * Delete this Memory allocator.
 */
Memory
Memory_delete(Memory this);
#endif//$XCC_h

Memory
Memory_delete(Memory this)
{
  if (this != NULL) {
    Memory parent = Memory_PARENT(this);
    if (parent != this) {
      MemoryList_remove(Memory_CHILDREN(parent), Memory_ITEM(this));
    }
    Memory_freeAll(this);
    Memory_Dtor(this);
    if (parent != this) {
      Memory_free_(parent, this);
    }
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * For use by Memory_CHILDREN_FOREACH_Memory.
 */
static inline MemoryList
Memory_children(Memory this)
{
  return Memory_CHILDREN(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterator for Memory_CHILDREN.
 */
#define Memory_CHILDREN_FOREACH_Memory(this, child) { \
  MemoryList Memory_CHILDREN = Memory_children(this); \
  MemoryList_FOREACH_MemoryListItem(Memory_CHILDREN, item) { \
    Memory child = (Memory)item;
#define Memory_CHILDREN_ENDEACH_Memory \
  } MemoryList_ENDEACH_MemoryListItem; \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by Memory_FOREACH_MemoryBlock.
 */
static inline MemoryList
Memory_blocks(Memory this)
{
  return Memory_BLOCKS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterator for Memory_BLOCKS.
 */
#define Memory_FOREACH_MemoryBlock(this, block) { \
  MemoryList Memory_BLOCKS = Memory_blocks(this); \
  MemoryList_FOREACH_MemoryListItem(Memory_BLOCKS, item) { \
    MemoryBlock block = (MemoryBlock)item;
#define Memory_ENDEACH_MemoryBlock \
  } MemoryList_ENDEACH_MemoryListItem; \
}
#endif//$XCC_h

static inline MemoryBlock
Memory_allocBlock_(Memory this, size_t useSize, bool inTable)
{
  size_t realSize = sizeof(MemoryBlock_) + useSize;
  MemoryBlock block = malloc(realSize);
//fprintf(stderr, "*** malloc(%d)=>\t0x%p\n", useSize, block);
  BSL_TRACE && fprintf(BSL_TRACE, "*** malloc(%zu)=>\t0x%p\n", useSize, block);
  *Memory__TOTALSIZE(this) += useSize;
  *Memory__TOTALCOUNT(this) += 1;
  Except_CHECK(useSize > 0 && _ALIGNED(useSize));
  Except_ALWAYS(block != NULL);
  if (inTable) {
    MemoryTable table = Memory_TABLE(this);
    MemoryBlock_Ctor(block, useSize, MemoryTable_LEAF(table));
    MemoryTable_insert(table, MemoryBlock_ENTRY(block));
  } else {
    MemoryBlock_Ctor(block, useSize, NULL);
  }
  MemoryList_append(Memory_BLOCKS(this), MemoryBlock_ITEM(block));
#if _MEMORY_STATS
  if (BSL_MEMORY_STATS) MemoryUsage_alloc(Memory_Usage);
#endif//_MEMORY_STATS
  Except_CHECK(_ALIGNED(block));
  return block;
}

#ifdef $XCC_h
/**
 * Specialized @c Memory_alloc for non-zero aligned @p alignedSize.
 */
void *
Memory_alloc_(Memory this, size_t alignedSize);
#endif//$XCC_h

void *
Memory_alloc_(Memory this, size_t alignedSize)
{
#ifndef Memory_DEBUG
  size_t poolSize = Memory_POOLSIZE(this);
  char *poolFirst = Memory_POOLFIRST(this);
  char *poolPast = Memory_POOLPAST(this);
  Except_CHECK(alignedSize > 0 && _ALIGNED(alignedSize));
//fprintf(stderr, "*** poolSize=%d\n", poolSize);
  if (alignedSize <= poolSize) {
    MemoryBlock poolBlock = Memory_POOLBLOCK(this);
    int refCount = MemoryBlock_REFCOUNT(poolBlock);
    if (poolFirst + alignedSize >= poolPast) {
      poolBlock = Memory_allocBlock_(this, poolSize, true);
      poolFirst = MemoryBlock_START(poolBlock);
      poolPast = MemoryBlock_PAST(poolBlock);
      *Memory__POOLBLOCK(this) = poolBlock;
      *Memory__POOLPAST(this) = poolPast;
      refCount = 0;
    }
    *Memory__POOLFIRST(this) = poolFirst + alignedSize;
    *MemoryBlock__REFCOUNT(poolBlock) = refCount + 1;
    Except_CHECK(MemoryBlock_REFCOUNT(poolBlock) > 0);
    Except_CHECK(_ALIGNED(poolFirst));
    return poolFirst;
  } else {
    MemoryBlock block = Memory_allocBlock_(this, alignedSize, false);
    void *alloc = block + 1;
    Except_CHECK(_ALIGNED(alloc));
    return alloc;
  }
#else//Memory_DEBUG
  void *alloc = malloc(alignedSize);
  BSL_TRACE && fprintf(BSL_TRACE, "*** malloc(%d)=>\t0x%p\n", alignedSize, alloc);
  Except_CHECK(_ALIGNED(alloc));
  return alloc;
#endif//Memory_DEBUG
}

#ifdef $XCC_h
/**
 * Allocate memory in its own MemoryBlock or in a pool.
 *
 * @param size 	The requested allocation size in bytes.
 * @return	Allocated address that holds @p size bytes.
 * Ensure:	Returned address is non-NULL.
 */
static inline void *
Memory_alloc(Memory this, size_t size)
{
  Except_CHECK(this != NULL);
  if (size > 0) {
    size_t alignedSize = _ALIGN_NEXT(size);
    void *alloc = Memory_alloc_(this, alignedSize);
    return alloc;
  }
  return NULL;
}
#endif//$XCC_h

static inline void
Memory_freeBlock_(Memory this, MemoryBlock block)
{
  size_t realSize = MemoryBlock_REALSIZE(block);
#if _MEMORY_STATS
  if (BSL_MEMORY_STATS) MemoryUsage_free(Memory_Usage, realSize);
#endif//_MEMORY_STATS
  MemoryList_remove(Memory_BLOCKS(this), MemoryBlock_ITEM(block));
#ifdef _DEBUG
  memset(block, -1, realSize);
#endif//_DEBUG
  BSL_TRACE && fprintf(BSL_TRACE, "*** free(0x%p)\n", block);
  free(block);
}

static inline void
Memory_freePoolBlock_(Memory this, MemoryBlock block)
{
#ifdef _DEBUG
  size_t useSize = MemoryBlock_USESIZE(block);
  char *start = MemoryBlock_START(block);
  memset(start, -1, useSize);
#endif//_DEBUG
  Except_CHECK(MemoryBlock_REFCOUNT(block) == 0);
  if (block == Memory_POOLBLOCK(this)) {
    *Memory__POOLFIRST(this) = MemoryBlock_START(block);
  } else {
    //TODO: Remove from MemoryTable.
    //Memory_freeBlock_(this, block);
  }
}

#ifdef $XCC_h
/**
 * Specialized @c Memory_free for non-NULL @c address.
 */
void
Memory_free_(Memory this, void *address);
#endif//$XCC_h

void
Memory_free_(Memory this, void *address)
{
#ifndef Memory_DEBUG
  MemoryBlock block = NULL;
  MemoryTable table = Memory_TABLE(this);
  size_t poolSize = Memory_POOLSIZE(this);
  Except_CHECK(address != NULL && _ALIGNED(address));
  if (poolSize > 0 && (block = MemoryTable_search(table, address)) != NULL) {
    int refCount = MemoryBlock_REFCOUNT(block);
    *MemoryBlock__REFCOUNT(block) = refCount - 1;
    if (refCount == 1) Memory_freePoolBlock_(this, block);
  } else {
    MemoryBlock block = (MemoryBlock)address - 1;
    Memory_freeBlock_(this, block);
  }
#else//Memory_DEBUG
  Except_CHECK(_ALIGNED(address));
  BSL_TRACE && fprintf(BSL_TRACE, "*** free(0x%p)\n", address);
  free(address);
#endif//Memory_DEBUG
}

#ifdef $XCC_h
/**
 * Free the memory at given @p address unless it is pooled.
 *
 * Reference counting of pooling memory blocks is maintained so when all
 * the allocations of such a block are freed it is returned to the system allocator.
 */
static inline void *
Memory_free(Memory this, void *address)
{
  Except_CHECK(this != NULL);
  if (address != NULL) {
    Except_CHECK(_ALIGNED(address));
    Memory_free_(this, address);
  }
  return NULL;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Free all the MemoryBlock(s) and children of this Memory allocator.
 */
void
Memory_freeAll(Memory this);
#endif//$XCC_h

void
Memory_freeAll(Memory this) {
  Except_CHECK(this != NULL);
  Memory_CHILDREN_FOREACH_Memory(this, memory) {
    if (memory != this) {
      Memory_freeAll(memory);
      Memory_Dtor(memory);
      Memory_free_(this, memory);
    }
  } Memory_CHILDREN_ENDEACH_Memory;
  MemoryList_Ctor(Memory_CHILDREN(this));
  Memory_FOREACH_MemoryBlock(this, block) {
    Memory_freeBlock_(this, block);
  } Memory_ENDEACH_MemoryBlock;
  MemoryList_Ctor(Memory_BLOCKS(this));
}

#ifdef $XCC_h
void *
Memory_realloc_(Memory this, void* address, size_t useSize);
#endif//$XCC_h

void *
Memory_realloc_(Memory this, void* address, size_t alignedSize)
{
#ifndef Memory_DEBUG
  MemoryBlock block = NULL;
  MemoryTable table = Memory_TABLE(this);
  size_t poolSize = Memory_POOLSIZE(this);
  Except_CHECK(_ALIGNED(alignedSize));
  if (poolSize > 0 && (block = MemoryTable_search(table, address)) != NULL) {
    char *alloc = Memory_alloc_(this, alignedSize);
    int refCount = MemoryBlock_REFCOUNT(block);
    const char *past = MemoryBlock_PAST(block);
    size_t copy_size = past - (char *)address;
    if (alloc >= (char *)address && alloc < past) {
      // alloc is in the same pooling block as address.
      copy_size = alloc - (char *)address;
    }
    if (copy_size > alignedSize) copy_size = alignedSize;
    memcpy(alloc, address, copy_size);
    *MemoryBlock__REFCOUNT(block) = refCount - 1;
    if (refCount == 1) Memory_freePoolBlock_(this, block);
    return alloc;
  } else {
    size_t realSize = sizeof(MemoryBlock_) + alignedSize;
    MemoryBlock old_block = (MemoryBlock)address - 1;
#if _MEMORY_STATS
    size_t old_realSize = MemoryBlock_REALSIZE(old_block);
#endif//_MEMORY_STATS
    MemoryList_remove(Memory_BLOCKS(this), MemoryBlock_ITEM(old_block));
    block = realloc(old_block, realSize);
    BSL_TRACE && fprintf(BSL_TRACE, "*** realloc(0x%p, %zu)=>\t0x%p\n", old_block, realSize, block);
    Except_ALWAYS(block != NULL);
    *MemoryBlock__USESIZE(block) = alignedSize;
    MemoryList_append(Memory_BLOCKS(this), MemoryBlock_ITEM(block));
#if _MEMORY_STATS
    if (BSL_MEMORY_STATS) {
      MemoryUsage_free(Memory_Usage, old_realSize);
      MemoryUsage_alloc(Memory_Usage);
    }
#endif//_MEMORY_STATS
    return block + 1;
  }
#else//Memory_DEBUG
  void *alloc = realloc(address, alignedSize);
  BSL_TRACE && fprintf(BSL_TRACE, "*** realloc(0x%p, %d)=>\t0x%p\n", address, alignedSize, alloc);
  return alloc;
#endif//Memory_DEBUG
}

#ifdef $XCC_h
/**
 * Reallocate data allocated by this Memory allocator.
 */
void *
Memory_realloc(Memory this, void* address, size_t size);
#endif//$XCC_h

void *
Memory_realloc(Memory this, void* address, size_t size)
{
  Except_CHECK(this != NULL);
  Except_CHECK(_ALIGNED(address));
  if (size == 0) {
    if (address != NULL) Memory_free_(this, address);
    return NULL;
  } else {
    size_t alignedSize = _ALIGN_NEXT(size);
    if (address != NULL) return Memory_realloc_(this, address, alignedSize);
    return Memory_alloc_(this, alignedSize);
  }
}

#ifdef $XCC__c
if(0) { // Not 64-bits compliant
  printf("Begin new/delete tests:\n");
  {
    Memory memory_1 = Memory_new(Memory_Root, true);
    Memory memory_2 = Memory_new(memory_1, true);
    Memory memory_3 = Memory_new(memory_1, true);
    Memory memory_4 = Memory_new(memory_2, true);
    Memory_alloc(memory_1, 10);
    Memory_alloc(memory_2, 20);
    Memory_alloc(memory_3, 30);
    Memory_alloc(memory_4, 40);
    printf("memory_1:");
    Memory_CHILDREN_FOREACH_Memory(memory_1, memory) {
      printf("\tmemory_%zu", Memory_POOLSIZE(memory));
    } Memory_CHILDREN_ENDEACH_Memory;
    printf("\nmemory_2:");
    Memory_CHILDREN_FOREACH_Memory(memory_2, memory) {
      printf("\tmemory_%zu", Memory_POOLSIZE(memory));
    } Memory_CHILDREN_ENDEACH_Memory;
    printf("\nmemory_3:");
    Memory_CHILDREN_FOREACH_Memory(memory_3, memory) {
      printf("\tmemory_%zu", Memory_POOLSIZE(memory));
    } Memory_CHILDREN_ENDEACH_Memory;
    printf("\nmemory_4:");
    Memory_CHILDREN_FOREACH_Memory(memory_4, memory) {
      printf("\tmemory_%zu", Memory_POOLSIZE(memory));
    } Memory_CHILDREN_ENDEACH_Memory;
    printf("\n");
    Memory_delete(memory_2);
    printf("memory_1:");
    Memory_CHILDREN_FOREACH_Memory(memory_1, memory) {
      printf("\tmemory_%zu", Memory_POOLSIZE(memory));
    } Memory_CHILDREN_ENDEACH_Memory;
    printf("\n");
    Memory_delete(memory_1);
    printf("\n");
  }
  printf("End new/delete tests.\n");
}
#endif//$XCC__c

#ifdef $XCC_h
extern Memory Memory_Root;
#endif//$XCC_h

Memory Memory_Root;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
uint32_t
Memory_usageMax(void);
#endif//$XCC_h

uint32_t
Memory_usageMax(void)
{
  return Memory_Usage != NULL? MemoryUsage_MAXUSE(Memory_Usage): 0;
}

#ifdef $XCC_h
uint32_t
Memory_usageCurrent(void);
#endif//$XCC_h

uint32_t
Memory_usageCurrent(void)
{
  return Memory_Usage != NULL? MemoryUsage_INUSE(Memory_Usage): 0;
}

#ifdef $XCC__h
/**
 * Initialize the Memory module.
 */
void
Memory_INIT(void);
#endif//$XCC__h

void
Memory_INIT(void)
{
  static Memory_ Memory_Root_;
  Memory_Root = Memory_Ctor(&Memory_Root_, &Memory_Root_, 0);
  Memory_Usage = MemoryUsage_Ctor(&Memory_Usage_);
#ifndef Memory_BLOCKSIZE
  if (GETENV("Memory_BLOCKSIZE")) {
    sscanf(GETENV("Memory_BLOCKSIZE"), "%d", &Memory_BLOCKSIZE);
    fprintf(stderr, "*** Memory_BLOCKSIZE=%d\n", Memory_BLOCKSIZE);
  }
#endif//Memory_BLOCKSIZE
}

#ifdef $XCC__h
/**
 * Finalize the Memory module.
 */
void
Memory_FINI(void);
#endif//$XCC__h

void
Memory_FINI(void)
{
  if (BSL_TRACE) {
#ifndef _NTRACE
    MemoryUsage_pretty(Memory_Usage, BSL_TRACE);
#endif//_NTRACE
  }
  Memory_freeAll(Memory_Root);
  Memory_Dtor(Memory_Root);
  Memory_Usage = NULL;
  Memory_Root = NULL;
}

#if XCC__C
static void
Memory_TEST(void)
{
#include "BSL/Memory_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(Memory_TEST);
  return 0;
}
#endif

