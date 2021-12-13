#ifdef $XCC_h
/*
 * !!!!	PQueue.xcc
 *
 * Nikola Puzovic (Nikola.Puzovic@st.com)
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
 * @brief Priority queue implementation.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/PQueue.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef void *PQueueMember;
typedef uint64_t PQueuePriority;
#define PQueuePriority_MAX ((uint64_t)(int64_t)-1)
#endif//$XCC_h

#ifdef $XCC_h
struct PQueueEntry_ {
  PQueueMember MEMBER;
  PQueuePriority PRIORITY;
};
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Priority queue implemented as a radix-4 heap.
 *
 * See R.E. Tarjan "Data Structures and Network Algorithms".
 *
 * The main difference with Tarjan's implementation is that we index the heap array
 * starting at index 0 instead of 1. In Tarjan's implementation, the parent of node x
 * is (x - 1 + d - 1)/d and children its are (d*(x - 1) + 2) ... (d*(x - 1) + d + 1).
 * In our implementation, the parent of node x is x > 0? (x - 1)/d: -1 and its children
 * are at (d*x + 1) ... (d*x + d).
 *
 * For d==4 and Tarjan's implementation, these are (x + 2)/4 and (4*x - 2) ... (4*x + 1).
 * For d==4 and here implementation, these are (x - 1)>>2 and (4*x + 1) ... (4*x + 4).
 */
struct PQueue_ {
  //@args	Memory memory, int32_t maxCount
  Memory MEMORY;
  uint32_t STATUS;		// MAXCOUNT:31;ISORDERED:1;
  //@access ISORDERED	(PQueue_STATUS(this)&0x1)
  //@access MAXCOUNT	(int32_t)(PQueue_STATUS(this)>>1)
  int32_t COUNT;		// Count of entries in PQueue.
  struct PQueueEntry_ *ENTRIES;
};
#endif//$XCC_h

PQueue
PQueue_Ctor(PQueue this, Memory memory, int32_t maxCount)
{
  *PQueue__MEMORY(this) = memory;
  *PQueue__STATUS(this) = (maxCount<<1)|0x1;
  *PQueue__COUNT(this) = 0;
  *PQueue__ENTRIES(this) = Memory_alloc(memory, maxCount*sizeof(PQueueEntry_));
  return this;
}

void
PQueue_Dtor(PQueue this)
{
  Memory memory = PQueue_MEMORY(this);
  Memory_free_(memory, PQueue_ENTRIES(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(PQueue_)\t%zu\n", sizeof(PQueue_));
}
#endif//$XCC__c

#ifdef $XCC_h
bool
PQueue_check(const_PQueue this);
#endif//$XCC_h

bool
PQueue_check(const_PQueue this)
{
  PQueueEntry entries = PQueue_ENTRIES(this);
  int32_t count = PQueue_COUNT(this), x;
  for (x = 0; x < count; x++) {
    int32_t child_1 = (4*x + 1);
    int32_t child_2 = (4*x + 2);
    int32_t child_3 = (4*x + 3);
    int32_t child_4 = (4*x + 4);
    if (child_1 < count) {
      Except_ALWAYS(PQueueEntry_PRIORITY(entries + x) <= PQueueEntry_PRIORITY(entries + child_1));
    }
    if (child_2 < count) {
      Except_ALWAYS(PQueueEntry_PRIORITY(entries + x) <= PQueueEntry_PRIORITY(entries + child_2));
    }
    if (child_3 < count) {
      Except_ALWAYS(PQueueEntry_PRIORITY(entries + x) <= PQueueEntry_PRIORITY(entries + child_3));
    }
    if (child_4 < count) {
      Except_ALWAYS(PQueueEntry_PRIORITY(entries + x) <= PQueueEntry_PRIORITY(entries + child_4));
    }
  }
  return true;
}

#ifdef $XCC_h
/**
 * Empty this PQueue.
 */
void
PQueue_empty(PQueue this);
#endif//$XCC_h

void
PQueue_empty(PQueue this)
{
  *PQueue__STATUS(this) |= 0x1;
  *PQueue__COUNT(this) = 0;
}

#ifdef $XCC_h
/**
 * True iff this PQueue is empty.
 */
static inline bool
PQueue_isEmpty(const_PQueue this)
{
  return PQueue_COUNT(this) == 0;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count members contained in the PQueue set.
 *
 * Return:	The count of members in this PQueue set.
 */
static inline int32_t
PQueue_count(const_PQueue this)
{
  return PQueue_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This PQueue ordered status.
 */
static inline bool
PQueue_isOrdered(const_PQueue this)
{
  return PQueue_ISORDERED(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this PQueue status as unordered.
 */
static inline void
PQueue_clearOrdered(PQueue this)
{
  *PQueue__STATUS(this) &= ~0x1;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Add new entry into this priority queue without preserving the
 * the heap invariant. This should be used only for initial insertion into the IQueue.
 */
void
PQueue_addEntry(PQueue this, PQueueMember member, PQueuePriority priority);
#endif//$XCC_h

void
PQueue_addEntry(PQueue this, PQueueMember member, PQueuePriority priority)
{
  int32_t count = PQueue_COUNT(this);
  PQueueEntry entry = PQueue_ENTRIES(this) + count;
  Except_REQUIRE(count < PQueue_MAXCOUNT(this));
  *PQueueEntry__PRIORITY(entry) = priority;
  *PQueueEntry__MEMBER(entry) = member;
  *PQueue__COUNT(this) = count + 1;
  if (count) PQueue_clearOrdered(this);
}

// PQueue_minChild --	The index of the minimal priority child of entry at index x.
static int32_t
PQueue_minChild(PQueue this, int32_t x)
{
  PQueuePriority min_priority = PQueuePriority_MAX;
  int32_t min_index = -1, max_index = PQueue_COUNT(this) - 1;
  int32_t first_child = 4*x + 1, last_child = 4*x + 4, index;
  PQueueEntry_ *entries = PQueue_ENTRIES(this);
  Except_CHECK(x >= 0 && x < PQueue_COUNT(this));
  if (last_child > max_index) last_child = max_index;
  for (index = first_child; index <= last_child; index++) {
    PQueuePriority priority = PQueueEntry_PRIORITY(entries + index);
    // Use >= instead of > for case priority == PQueuePriority_MAX.
    if (min_priority >= priority) {
      min_priority = priority;
      min_index = index;
    }
  }
  return min_index;
}

static void
PQueue_siftUp(PQueue this, int32_t x, PQueueEntry entry)
{
  int32_t p = (x - 1)>>2;
  PQueueEntry_ *entries = PQueue_ENTRIES(this);
  Except_CHECK(x >= 0 && x < PQueue_COUNT(this));
  while (   p >= 0
         && PQueueEntry_PRIORITY(entries + p) > PQueueEntry_PRIORITY(entry)) {
    entries[x] = entries[p];
    x = p;
    p = (x - 1)>>2;
  }
  entries[x] = *entry;
}

static void
PQueue_siftDown(PQueue this, int32_t x, PQueueEntry entry)
{
  int32_t c = PQueue_minChild(this, x);
  PQueueEntry_ *entries = PQueue_ENTRIES(this);
  Except_CHECK(x >= 0 && x < PQueue_COUNT(this));
  while (   c >= 0
         && PQueueEntry_PRIORITY(entries + c) < PQueueEntry_PRIORITY(entry)) {
    entries[x] = entries[c];
    x = c;
    c = PQueue_minChild(this, c);
  }
  entries[x] = *entry;
}

#ifdef $XCC_h
/**
 * Make heap order for this PQueue.
 */
int32_t
PQueue_makeHeap(PQueue this);
#endif//$XCC_h

int32_t
PQueue_makeHeap(PQueue this)
{
  int32_t count = PQueue_COUNT(this), i;
  PQueueEntry_ *entries = PQueue_ENTRIES(this);
  *PQueue__COUNT(this) = count;
  for (i = count - 1; i >= 0; i--) {
    PQueueEntry_ entry_ = entries[i];
    PQueue_siftDown(this, i, &entry_);
  }
  Except_DEBUG(PQueue_check(this));
  *PQueue__STATUS(this) |= 0x1;
  return count;
}

#ifdef $XCC_h
/**
 * Insert new member to this priority queue.
 */
void
PQueue_insert(PQueue this, PQueueMember member, PQueuePriority priority);
#endif//$XCC_h

void
PQueue_insert(PQueue this, PQueueMember member, PQueuePriority priority)
{
  int32_t count = PQueue_COUNT(this);
  struct PQueueEntry_ entry[1];
  *PQueueEntry__MEMBER(entry) = member;
  *PQueueEntry__PRIORITY(entry) = priority;
  Except_REQUIRE(PQueue_isOrdered(this));
  Except_REQUIRE(count >= 0 && count < PQueue_MAXCOUNT(this));
  *PQueue__COUNT(this) = count + 1;
  PQueue_siftUp(this, count, entry);
  Except_DEBUG(PQueue_check(this));
}

#ifdef $XCC_h
/**
 * Remove the entry at given index.
 */
void
PQueue_remove(PQueue this, int32_t index);
#endif//$XCC_h

void
PQueue_remove(PQueue this, int32_t index)
{
  int32_t count = PQueue_COUNT(this);
  PQueueEntry_ *entries = PQueue_ENTRIES(this);
  PQueueEntry last = entries + count - 1;
  Except_REQUIRE(PQueue_isOrdered(this));
  Except_REQUIRE(count > 0);
  *PQueue__COUNT(this) = count - 1;
  if (index < count - 1) {
    if (PQueueEntry_PRIORITY(last) <= PQueueEntry_PRIORITY(entries + index)) {
      PQueue_siftUp(this, index, last);
    } else {
      PQueue_siftDown(this, index, last);
    }
  }
  Except_DEBUG(PQueue_check(this));
}

#ifdef $XCC_h
/**
 * Update the priority of an entry.
 */
void
PQueue_update(PQueue this, const_PQueueEntry entry, PQueuePriority priority);
#endif//$XCC_h

void
PQueue_update(PQueue this, const_PQueueEntry entry, PQueuePriority priority)
{
  PQueueEntry_ entry_ = *entry;
  int32_t index = entry - PQueue_ENTRIES(this);
  *PQueueEntry__PRIORITY(&entry_) = priority;
  Except_REQUIRE(index >= 0 && index < PQueue_COUNT(this));
  Except_REQUIRE(PQueue_isOrdered(this));
  if (priority <= PQueueEntry_PRIORITY(entry)) {
    PQueue_siftUp(this, index, &entry_);
  } else {
    PQueue_siftDown(this, index, &entry_);
  }
  Except_DEBUG(PQueue_check(this));
}

#ifdef $XCC_h
/**
 * Extract the member with minimum priority from this PQueue.
 */
PQueueMember
PQueue_extractMin(PQueue this);
#endif//$XCC_h

#if 1
PQueueMember
PQueue_extractMin(PQueue this)
{
  int32_t count = PQueue_COUNT(this);
  PQueueEntry_ *entries = PQueue_ENTRIES(this);
  PQueueEntry last = entries + count - 1;
  PQueueMember member = entries[0].MEMBER;
  Except_REQUIRE(PQueue_isOrdered(this));
  *PQueue__COUNT(this) = count - 1;
  if (count > 1) {
    PQueue_siftDown(this, 0, last);
  }
  Except_DEBUG(PQueue_check(this));
  return member;
}
#else
PQueueMember
PQueue_extractMin(PQueue this)
{
  int32_t count = PQueue_COUNT(this);
  PQueueEntry_ *entries = PQueue_ENTRIES(this);
  PQueueMember member = entries[0].MEMBER;
  Except_REQUIRE(PQueue_isOrdered(this));
  PQueue_remove(this, 0);
  Except_DEBUG(PQueue_check(this));
  return member;
}
#endif

#ifdef $XCC_h
/**
 * Access the the member with minimum priority from this PQueue.
 */
static inline PQueueMember
PQueue_accessMin(const_PQueue this)
{
  PQueueEntry_ *entries = PQueue_ENTRIES(this);
  PQueueMember member = entries[0].MEMBER;
  return member;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by PQueue_FOREACH.
 */
static inline PQueueEntry_ *
PQueue_entries(const_PQueue this)
{
  return PQueue_ENTRIES(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates over this PQueue ENTRIES.
 *
 * For each iteration, PQueue_INDEX is index to the internal array of the queue,
 * and PQueue_ENTRY is the current PQueueEntry.
 */
#define PQueue_FOREACH(this, Type, member) { \
  PQueueEntry_ *PQueue_ENTRIES = PQueue_entries(this); \
  int32_t PQueue_COUNT = PQueue_count(this), PQueue_INDEX; \
  if (PQueue_COUNT < 0) PQueue_COUNT = -PQueue_COUNT; \
  for (PQueue_INDEX = 0; PQueue_INDEX < PQueue_COUNT; PQueue_INDEX++) { \
    PQueueEntry PQueue_ENTRY = PQueue_ENTRIES + PQueue_INDEX; \
    Type member = (Type)PQueueEntry_MEMBER(PQueue_ENTRY);
#define PQueue_ENDEACH \
  } \
}
#endif//$XCC_h

#ifdef $XCC__h
/**
 * Pretty print members of the queue.
 */
bool
PQueue_pretty(PQueue this, FILE *file);
#endif//$XCC__h

#if !defined(_NTRACE) || defined(XCC__C)
bool
PQueue_pretty(PQueue this, FILE *file)
{
  if (file == NULL) file = stderr;
  PQueue_FOREACH(this, void*, member) {
    int priority = PQueueEntry_PRIORITY(PQueue_ENTRY);
    fprintf (file, "%d ", priority);
  } PQueue_ENDEACH;
  return true;
}
#endif//_NTRACE

#ifdef $XCC__c
{
  PQueue_ queue[1];
  PQueuePriority e1 = 1, e2 = 2, e3 = 3, e4 = 4, e5 = 5, e6 = 6, e7 = 7, e8 = 8;
  PQueue_Ctor(queue, Memory_Root, 8);
  PQueue_addEntry(queue, &e7, 7);
  printf("Inserting 7, pqueue=[ ");PQueue_pretty(queue, stdout);printf("]\n");
  PQueue_addEntry(queue, &e4, 4);
  printf("Inserting 4, pqueue=[ ");PQueue_pretty(queue, stdout);printf("]\n");
  PQueue_addEntry(queue, &e5, 5);
  printf("Inserting 5, pqueue=[ ");PQueue_pretty(queue, stdout);printf("]\n");
  PQueue_addEntry(queue, &e3, 3);
  printf("Inserting 3, pqueue=[ ");PQueue_pretty(queue, stdout);printf("]\n");
  PQueue_makeHeap(queue);
  PQueue_insert(queue, &e6, 6);
  printf("Inserting 6, pqueue=[ ");PQueue_pretty(queue, stdout);printf("]\n");
  PQueue_insert(queue, &e2, 2);
  printf("Inserting 2, pqueue=[ ");PQueue_pretty(queue, stdout);printf("]\n");
  PQueue_insert(queue, &e1, 1);
  printf("Inserting 1, pqueue=[ ");PQueue_pretty(queue, stdout);printf("]\n");
  printf("\n\n");
  while (!PQueue_isEmpty(queue)) {
    int32_t *current = (int32_t*)PQueue_extractMin(queue);
    printf("Getting %d, pqueue=[ ", *current);
    PQueue_pretty(queue, stdout);
    printf("]\n");
  }
  PQueue_Dtor(queue);
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int i;
  PQueue_ queue[1];
  PQueuePriority array[51];
  PQueue_Ctor(queue, Memory_Root, 50);
  for (i = 50; i > 0; i--) {
    array[i] = i;
    PQueue_insert(queue, &array[i], i);
  }
  printf("\n\nAfter inserting 50 elements: pqueue=[");
  PQueue_pretty(queue, stdout);
  printf("\n\nExtracted elements: \n");
  while (!PQueue_isEmpty(queue)) {
    int32_t *current = (int32_t*)PQueue_extractMin(queue);
    printf("%d ", *current);
  }
  printf("\n"); 
  PQueue_Dtor(queue);
}
#endif//$XCC__c

#ifdef $XCC__c
{
  long int i;
  PQueue_ queue[1];
  PQueuePriority priorities[51];
  PQueue_Ctor(queue, Memory_Root, 50);
  for (i = 50; i > 0; i--) {
    priorities[i] = i;
    PQueue_insert(queue, (PQueueMember)i, priorities[i]);
  }
  printf("\n\nAfter inserting 50 elements: pqueue=[");
  PQueue_pretty(queue, stdout);
  printf("]");
  for (i = 0; i < 25; i++) {
    PQueue_FOREACH(queue, long int, member) {
      if (member == i) {
        PQueuePriority priority = PQueueEntry_PRIORITY(PQueue_ENTRY);
        *PQueueEntry__MEMBER(PQueue_ENTRY) = (PQueueMember)((long int)priority + 25);
        PQueue_update(queue, PQueue_ENTRY, priority + 25);
        break;
      }
    } PQueue_ENDEACH;
  }
  printf("\n\nAfter updating 50 elements: pqueue=[");
  PQueue_pretty(queue, stdout);
  printf("]");
  printf("\n\nExtracted elements: \n");
  for (i = 0; i < 50; i++) {
    long int current = (long int)PQueue_extractMin(queue);
    printf("%ld ", current);
  }
  printf("\n"); 
  PQueue_Dtor(queue);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the PQueue module.
 */
#define PQueue_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the PQueue module.
 */
#define PQueue_FINI()
#endif//$XCC__h

#if XCC__C
static void
PQueue_TEST(void)
{
#include "CCL/PQueue_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(PQueue_TEST);
  return 0;
}
#endif

