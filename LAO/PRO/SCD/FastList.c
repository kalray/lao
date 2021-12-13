#ifdef $XCC_h
/*
 * !!!!	FastList.xcc
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

#ifdef $XCC_h
/**
 * @file
 * @ingroup SCD
 * @brief Fast List scheduling of Leung-Palem-Pnueli.
 */
#endif//$XCC_h


#include "SCD/SCD_.h"

#ifdef $XCC__h
#include "SCD/FastList.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * FastList scheduling rank.
 */
struct FastListRank_;
#endif//$XCC_h

#ifdef $XCC__h
struct FastListRank_ {
  //@args	int release
  uint32_t CAPACITY;		// The resource capacity of this FastListRank.
  uint32_t CONSUMED;		// The resource consumed in this FastListRank.
  int16_t RELEASE;		// The release time of this FastListRank.
  FastListRank PARENT;		// The parent of this FastListRank for the Union-Find.
  //@access NEXT	((FastListRank)(this) + 1)
};
#endif//$XCC__h

FastListRank
FastListRank_Ctor(FastListRank this,
                  int release)
{
  *FastListRank__CAPACITY(this) = (uint32_t)(int32_t)-1;
  *FastListRank__CONSUMED(this) = 0;
  *FastListRank__RELEASE(this) = release;
  *FastListRank__PARENT(this) = NULL;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(FastListRank_)\t%zu\n", sizeof(FastListRank_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Find of the Union-Find data structure.
 */
static inline FastListRank
FastListRank_find(FastListRank this)
{
  FastListRank root = this, that = this;
  while (FastListRank_PARENT(root) != NULL) {
    root = FastListRank_PARENT(root);
  }
  while (that != root) {
    FastListRank parent = FastListRank_PARENT(that);
    *FastListRank__PARENT(that) = root;
    that = parent;
  }
  return root;
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Schedule at this FastListRank.
 */
unsigned
FastListRank_schedule(FastListRank this, unsigned available, unsigned required);
#endif//$XCC__h

unsigned
FastListRank_schedule(FastListRank this, unsigned available, unsigned required)
{
  FastListRank rank = FastListRank_find(this);
  uint32_t capacity = FastListRank_CAPACITY(rank);
  uint32_t consumed = FastListRank_CONSUMED(rank);
  uint32_t cumulated = consumed + required;
  Except_REQUIRE(consumed < capacity && required > 0);
  if (cumulated < capacity) {
    *FastListRank__CONSUMED(rank) = cumulated;
    return FastListRank_RELEASE(rank) + (cumulated - 1)/available;
  }
  *FastListRank__CONSUMED(rank) = capacity;
  *FastListRank__PARENT(rank) = FastListRank_find(FastListRank_NEXT(rank));
  if (cumulated == capacity) {
    return FastListRank_RELEASE(FastListRank_NEXT(rank)) - 1;
  }
  return FastListRank_schedule(rank, available, cumulated - capacity);
}

#ifdef $XCC__h
/**
 * Specialize FastListRank_schedule when available = 2^shift.
 */
unsigned
FastListRank_scheduleS(FastListRank this, int shift, unsigned required);
#endif//$XCC__h

unsigned
FastListRank_scheduleS(FastListRank this, int shift, unsigned required)
{
  FastListRank rank = FastListRank_find(this);
  uint32_t capacity = FastListRank_CAPACITY(rank);
  uint32_t consumed = FastListRank_CONSUMED(rank);
  uint32_t cumulated = consumed + required;
  Except_REQUIRE(consumed < capacity && required > 0);
  if (cumulated < capacity) {
    *FastListRank__CONSUMED(rank) = cumulated;
    return FastListRank_RELEASE(rank) + ((cumulated - 1)>>shift);
  }
  *FastListRank__CONSUMED(rank) = capacity;
  *FastListRank__PARENT(rank) = FastListRank_find(FastListRank_NEXT(rank));
  if (cumulated == capacity) {
    return FastListRank_RELEASE(FastListRank_NEXT(rank)) - 1;
  }
  return FastListRank_scheduleS(rank, shift, cumulated - capacity);
}

#ifdef $XCC_h
/**
 * Enumeration of the FastList date types.
 */
typedef enum {
  FastListType_Release,		// Release date.
  FastListType_Schedule,	// Schedule date.
  FastListType_Deadline,	// Deadline date.
  FastListType__
} enum_FastListType;
typedef uint8_t FastListType;
extern const char *
FastListType_Id(FastListType this);
#endif//$XCC_h

const char *
FastListType_Id(FastListType this)
{
  static const char *_[] = {
    "Release",
    "Schedule",
    "Deadline",
  };
  Except_REQUIRE((unsigned)this < FastListType__);
  return _[this];
}

#ifdef $XCC_h
/**
 * FastList task to FastList schedule.
 */
struct FastListTask_;
#endif//$XCC_h

#ifdef $XCC__h
struct FastListTask_ {
  //@args	const uint8_t *requirements, RelaxationItem item
  const uint8_t *REQUIREMENTS;		// The resource requirements of this FastListTask.
  struct RelaxationItem_ *ITEM;		// The corresponding RelaxationItem.
  //@access INDEX	RelaxationItem_INDEX(FastListTask_ITEM(this))
  FastListRank RANK;			// The FastListRank of this FastListTask.
  FastListTask LINKS[FastListType__];	// The links to order FastListTask(s) by increasing date.
  int16_t DATES[FastListType__];	// The release date and the deadline date.
  //@access RELEASE	(FastListTask_DATES(this)[FastListType_Release])
  //@mutate RELEASE	(FastListTask_DATES(this) + FastListType_Release)
  //@access SCHEDULE	(FastListTask_DATES(this)[FastListType_Schedule])
  //@mutate SCHEDULE	(FastListTask_DATES(this) + FastListType_Schedule)
  //@access DEADLINE	(FastListTask_DATES(this)[FastListType_Deadline])
  //@mutate DEADLINE	(FastListTask_DATES(this) + FastListType_Deadline)
};
#endif//$XCC__h

FastListTask
FastListTask_Ctor(FastListTask this,
                  const uint8_t *requirements, RelaxationItem item)
{
  *FastListTask__REQUIREMENTS(this) = requirements;
  *FastListTask__ITEM(this) = item;
  *FastListTask__RANK(this) = NULL;
  FastListTask_LINKS(this)[FastListType_Release] = NULL;
  FastListTask_LINKS(this)[FastListType_Schedule] = NULL;
  FastListTask_LINKS(this)[FastListType_Deadline] = NULL;
  FastListTask_DATES(this)[FastListType_Release] = -1;
  FastListTask_DATES(this)[FastListType_Schedule] = -1;
  FastListTask_DATES(this)[FastListType_Deadline] = -1;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(FastListTask_)\t%zu\n", sizeof(FastListTask_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Used by FastList_FOREACH_(Type)_FastListTask.
 */
static inline FastListTask
FastListTask_link(FastListTask this, FastListType type)
{
  return FastListTask_LINKS(this)[type];
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Compare two FastListTask(s) on their DATES.
 */
static inline int
FastListTask_compare(FastListTask this, FastListTask that, FastListType type)
{
  return FastListTask_DATES(this)[type] - FastListTask_DATES(that)[type];
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Meld function for the merge-sort of FastListTasks.
 */
FastListTask
FastListTask_meld(FastListTask this, FastListTask that, FastListType type);
#endif//$XCC__h

FastListTask
FastListTask_meld(FastListTask this, FastListTask that, FastListType type)
{
  FastListTask_ meld_task_, *meld_task = &meld_task_;
  FastListTask_LINKS(meld_task)[type] = NULL;
  while (this != NULL && that != NULL) {
    if (FastListTask_compare(this, that, type) <= 0) {
      FastListTask_LINKS(meld_task)[type] = this;
      this = FastListTask_LINKS((meld_task = this))[type];
    } else {
      FastListTask_LINKS(meld_task)[type] = that;
      that = FastListTask_LINKS((meld_task = that))[type];
    }
  }
  if (this != NULL) {
    FastListTask_LINKS(meld_task)[type] = this;
  }
  if (that != NULL) {
    FastListTask_LINKS(meld_task)[type] = that;
  }
  return FastListTask_LINKS(&meld_task_)[type];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * FastList scheduling implementation.
 *
 * Fast List scheduling solves the following problem: Given a set of tasks to
 * schedule, each with a release date, a deadline, and required resources, list
 * schedule (Graham) as Unit Execution Tasks (UET) on a parallel machine with m
 * processors. The outcome is false if any task misses its deadline.
 */
struct FastList_;
#endif//$XCC_h

#ifdef $XCC__h
struct FastList_ {
  //@args	Memory memory, int32_t count, const uint8_t *availability
  const uint8_t *AVAILABILITY;	// The resource availability.
  FastListTask_ *TASKS;		// Array of FastListTasks in creation order.
  //@access FIRSTTASK	FastList_TASKS(this)
  FastListTask ENDTASK;		// The end FastListTask in this TASKS.
  //@access COUNT	(FastList_ENDTASK(this) - FastList_TASKS(this))
  FastListRank_ *RANKS;		// The FastListRanks in increasing RELEASE time.
  FastListRank ENDRANK;		// The end FastListRank in this RANKS.
  FastListTask_ LISTS_;			// The list of FastListTask(s) by increasing date.
  //@access LISTS	FastList__LISTS_(this)
  //@access LINKS	FastListTask_LINKS(FastList_LISTS(this))
  IQueue_ IQUEUE_;		// The IQueue used for sorting the LISTS.
  //@access IQUEUE	FastList__IQUEUE_(this)
  //@access MEMORY	IQueue_memory(FastList_IQUEUE(this))
};
#endif//$XCC__h

FastList
FastList_Ctor(FastList this,
              Memory memory, int32_t count, const uint8_t *availability)
{
  *FastList__AVAILABILITY(this) = availability;
  *FastList__TASKS(this) = Memory_alloc(memory, count*sizeof(FastListTask_));
  *FastList__ENDTASK(this) = FastList_TASKS(this);
  *FastList__RANKS(this) = Memory_alloc(memory, count*sizeof(FastListRank_));
  *FastList__ENDRANK(this) = FastList_RANKS(this);
  FastListTask_Ctor(FastList_LISTS(this), NULL, NULL);
  IQueue_Ctor(FastList_IQUEUE(this), memory, sizeof(FastListTask), count);
  return this;
}

void
FastList_Dtor(FastList this)
{
  Memory memory = FastList_MEMORY(this);
  Memory_free_(memory, FastList_TASKS(this));
  Memory_free_(memory, FastList_RANKS(this));
  IQueue_Dtor(FastList_IQUEUE(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(FastList_)\t%zu\n", sizeof(FastList_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Used by FastList_FOREACH_FastListTask.
 */
static inline FastListTask_ *
FastList_tasks(FastList this)
{
  return FastList_TASKS(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Used by FastList_FOREACH_FastListTask.
 */
static inline FastListTask
FastList_endTask(FastList this)
{
  return FastList_ENDTASK(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this FastList TASKS in index order.
 */
#define FastList_FOREACH_FastListTask(this, task) { \
  FastListTask task = FastList_tasks(this); \
  FastListTask FastList_ENDTASK = FastList_endTask(this); \
  for (; task < FastList_ENDTASK; task++) {
#define FastList_ENDEACH_FastListTask \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Used by FastList_FOREACH_FastListRank.
 */
static inline FastListRank_ *
FastList_ranks(FastList this)
{
  return FastList_RANKS(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Used by FastList_FOREACH_FastListRank.
 */
static inline FastListRank
FastList_endRank(FastList this)
{
  return FastList_ENDRANK(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this FastList RANKS in RELEASE order.
 */
#define FastList_FOREACH_FastListRank(this, rank) { \
  FastListRank rank = FastList_ranks(this); \
  FastListRank FastList_ENDRANK = FastList_endRank(this); \
  for (; rank < FastList_ENDRANK; rank++) {
#define FastList_ENDEACH_FastListRank \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Used by FastList_FOREACH_(Type)_FastListTask.
 */
static inline FastListTask
FastList_list(FastList this, FastListType type)
{
  return FastList_LINKS(this)[type];
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this FastList TASKS in Release order.
 */
#define FastList_FOREACH_Release_FastListTask(this, task) { \
  FastListTask task = FastList_list(this, FastListType_Release); \
  for (; task != NULL; task = FastListTask_link(task, FastListType_Release)) {
#define FastList_ENDEACH_Release_FastListTask \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this FastList TASKS in Deadline order.
 */
#define FastList_FOREACH_Deadline_FastListTask(this, task) { \
  FastListTask task = FastList_list(this, FastListType_Deadline); \
  for (; task != NULL; task = FastListTask_link(task, FastListType_Deadline)) {
#define FastList_ENDEACH_Deadline_FastListTask \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this FastList TASKS in Schedule order.
 */
#define FastList_FOREACH_Schedule_FastListTask(this, task) { \
  FastListTask task = FastList_list(this, FastListType_Schedule); \
  for (; task != NULL; task = FastListTask_link(task, FastListType_Schedule)) {
#define FastList_ENDEACH_Schedule_FastListTask \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Create a new FastListTask in this TASKS array.
 */
static inline FastListTask
FastList_makeTask(FastList this, const uint8_t *requirements, RelaxationItem item)
{
  FastListTask task = FastList_ENDTASK(this);
  int index = task - FastList_TASKS(this);
  FastListTask_Ctor(task, requirements, item);
  if (index > 0) {
    // Not the first FastListTask created.
    FastListTask prev_task = FastList_TASKS(this) + index - 1;
    FastListTask_LINKS(prev_task)[FastListType_Release] = task;
    FastListTask_LINKS(prev_task)[FastListType_Schedule] = task;
    FastListTask_LINKS(prev_task)[FastListType_Deadline] = task;
  } else {
    // This is the first FastListTask created.
    FastList_LINKS(this)[FastListType_Release] = task;
    FastList_LINKS(this)[FastListType_Schedule] = task;
    FastList_LINKS(this)[FastListType_Deadline] = task;
  }
  ++*FastList__ENDTASK(this);
  return task;
}
#endif//$XCC__h

#ifdef $XCC_h
/**
 * Update the release dates and keep the list in order.
 *
 * In order to save sorting time, we extract the FastListTask(s) whose release
 * date was updated into an auxiliary list, sort the auxiliary list, and merge it
 * back. Thus the list must be sorted when entering FastList_updateRelease.
 */
void
FastList_updateRelease(FastList this, int p, int q);
#endif//$XCC_h

void
FastList_updateRelease(FastList this, int p, int q)
{
  FastListType type = FastListType_Release;
  FastListTask firstTask = FastList_FIRSTTASK(this);
  FastListTask prev_task = FastList_LISTS(this);
  FastListTask task = FastListTask_LINKS(prev_task)[type];
  FastListTask_ head_task[1], *tail_task = head_task;
  Except_CHECK(!FastList_sortTasks(this, type, FastList_LISTS(this)));
  FastListTask_LINKS(head_task)[type] = NULL;
  // Loop over the FastListTask(s) in sorted Release order. When updating a
  // release date, move the corresponding FastListTask into the head_task list.
  while (task != NULL) {
    FastListTask next_task = FastListTask_LINKS(task)[type];
    RelaxationItem item = FastListTask_ITEM(task);
    int release = RelaxationItem_RELEASE(item);
    int latency = RelaxationItem_LATENCY(item);
    if (release < latency + q) {
      release = latency + q;
      Except_CHECK(latency >= 0);
    }
    if (task == firstTask) release = p;
    if (FastListTask_RELEASE(task) != release) {
      *FastListTask__RELEASE(task) = release;
      FastListTask_LINKS(prev_task)[type] = next_task;
      FastListTask_LINKS(tail_task)[type] = task;
      tail_task = task;
    } else {
      prev_task = task;
    }
    task = next_task;
  }
  // Here FastList_LINKS points to the unchanged FastListTask(s), and
  // head_task points to the FastListTask(s) with changed release dates.
  if (tail_task != head_task) {
    // Sort the head_task list and merge with FastList_LINKS.
    FastListTask_LINKS(tail_task)[type] = NULL;
    FastList_sortTasks(this, type, head_task);
    FastList_LINKS(this)[type] =
        FastListTask_meld(
            FastListTask_LINKS(head_task)[type],
            FastList_LINKS(this)[type], type);
  }
}

#ifdef $XCC_h
/**
 * Update the deadline dates and keep the list in order.
 *
 * Similar to FastList_updateRelease, in particular the list must be sorted when
 * entering FastList_updateDeadline. And additional specialization is that at
 * most one deadline is modified, so the update loop can be exited early.
 */
void
FastList_updateDeadline(FastList this, int p, int q);
#endif//$XCC_h

void
FastList_updateDeadline(FastList this, int p, int q)
{
  FastListType type = FastListType_Deadline;
  FastListTask firstTask = FastList_FIRSTTASK(this);
  FastListTask prev_task = FastList_LISTS(this);
  FastListTask task = FastListTask_LINKS(prev_task)[type];
  FastListTask_ head_task[1], *tail_task = head_task;
  Except_CHECK(!FastList_sortTasks(this, type, FastList_LISTS(this)));
  FastListTask_LINKS(head_task)[type] = NULL;
  // Loop over the FastListTask(s) in sorted Deadline order. When updating a
  // release date, move the corresponding FastListTask into the head_task list.
  // Deadline modification may only happen for firstTask, we so exit early.
  while (task != NULL) {
    FastListTask next_task = FastListTask_LINKS(task)[type];
    RelaxationItem item = FastListTask_ITEM(task);
    int deadline = RelaxationItem_DEADLINE(item);
    if (task == firstTask) {
      deadline = q + 1;
      if (FastListTask_DEADLINE(task) != deadline) {
        *FastListTask__DEADLINE(task) = deadline;
        FastListTask_LINKS(prev_task)[type] = next_task;
        FastListTask_LINKS(tail_task)[type] = task;
        tail_task = task;
      }
      break;
    }
    prev_task = task;
    task = next_task;
  }
  // Here FastList_LINKS points to the unchanged FastListTask(s), and
  // head_task points to the FastListTask with changed deadline date.
  if (tail_task != head_task) {
    // Because there is only firstTask in the head_task list, just merge it.
    FastListTask_LINKS(tail_task)[type] = NULL;
    FastList_LINKS(this)[type] =
        FastListTask_meld(
            FastListTask_LINKS(head_task)[type],
            FastList_LINKS(this)[type], type);
  }
}

#ifdef $XCC_h
/**
 * Sort this FastList TASKS on the specified list.
 * Return:	true if sorting changed the list, false if already sorted.
 *
 * See CCL/IDList.xcc:IDList_sort().
 */
bool
FastList_sortTasks(FastList this, FastListType type, FastListTask lists);
#endif//$XCC_h

bool
FastList_sortTasks(FastList this, FastListType type, FastListTask lists)
{
  FastListTask task = FastListTask_LINKS(lists)[type];
  if (task != NULL && FastListTask_LINKS(task)[type] != NULL) {
    IQueue iqueue = FastList_IQUEUE(this);
    FastListTask next_task = FastListTask_LINKS(task)[type];
    // Split the list into sorted chains.
    IQueue_PUT(iqueue, FastListTask, task);
    while (next_task != NULL) {
      if (FastListTask_compare(task, next_task, type) > 0) {
        IQueue_PUT(iqueue, FastListTask, next_task);
        FastListTask_LINKS(task)[type] = NULL;
      }
      next_task = FastListTask_LINKS((task = next_task))[type];
    }
    if (IQueue_isSingle(iqueue)) {
      IQueue_empty(iqueue, NULL);
      return false;
    }
    // Merge the sorted chains together.
    do {
      FastListTask task1 = NULL, task2 = NULL;
      IQueue_POP(iqueue, FastListTask, task1);
      IQueue_POP(iqueue, FastListTask, task2);
      IQueue_PUT(iqueue, FastListTask, FastListTask_meld(task1, task2, type));
    } while (!IQueue_isSingle(iqueue));
    FastListTask_LINKS(lists)[type] = IQueue_TOP(iqueue, FastListTask);
    IQueue_empty(iqueue, NULL);
    return true;
  }
  return false;
}

#ifdef $XCC__h
/**
 * Create a new FastListRank in this RANKS array.
 */
FastListRank
FastList_makeRank(FastList this, int release);
#endif//$XCC__h

FastListRank
FastList_makeRank(FastList this, int release)
{
  FastListRank rank = FastList_ENDRANK(this);
  FastListRank_Ctor(rank, release);
  ++*FastList__ENDRANK(this);
  return rank;
}

#ifdef $XCC__h
/**
 * Make this FastList RANKS.
 */
void
FastList_makeRanks(FastList this, unsigned available);
#endif//$XCC__h

void
FastList_makeRanks(FastList this, unsigned available)
{
  int prev_release = -1;
  FastListRank prev_rank = NULL;
  *FastList__ENDRANK(this) = FastList_RANKS(this);
  FastList_FOREACH_Release_FastListTask(this, task) {
    int release = FastListTask_RELEASE(task);
    if (release > prev_release) {
      FastListRank rank = FastList_makeRank(this, release);
      if (prev_rank != NULL) {
        Except_CHECK(FastListRank_NEXT(prev_rank) == rank);
        *FastListRank__CAPACITY(prev_rank) = (release - prev_release)*available;
      }
      prev_release = release;
      prev_rank = rank;
    }
    *FastListTask__RANK(task) = prev_rank;
  } FastList_ENDEACH_Release_FastListTask;
  Except_ENSURE(FastListRank_CAPACITY(prev_rank) == (uint32_t)(int32_t)-1);
}

#ifdef $XCC_h
/**
 * Test if this FastList schedule is feasible.
 */
bool
FastList_schedule(FastList this, Resource resource);
#endif//$XCC_h

bool
FastList_schedule(FastList this, Resource resource)
{
  static int8_t log2[] = { -1, 0, 1, -1, 2, -1, -1, -1, 3 };
  unsigned available = FastList_AVAILABILITY(this)[resource];
  int shift = (available <= 8)? log2[available]: -1;
  FastListTask lists = FastList_LISTS(this);
  Except_CHECK(!FastList_sortTasks(this, FastListType_Release, lists));
  Except_CHECK(!FastList_sortTasks(this, FastListType_Deadline, lists));
  FastList_makeRanks(this, available);
  if (shift < 0) {
    FastList_FOREACH_Deadline_FastListTask(this, task) {
      unsigned required = FastListTask_REQUIREMENTS(task)[resource];
      int schedule = FastListTask_RELEASE(task);
      int deadline = FastListTask_DEADLINE(task);
      if (required > 0) {
        FastListRank rank = FastListTask_RANK(task);
        schedule = FastListRank_schedule(rank, available, required);
        if (schedule >= deadline) return false;
      }
      *FastListTask__SCHEDULE(task) = schedule;
    } FastList_ENDEACH_Deadline_FastListTask;
  } else {
    Except_CHECK(available == (1 << shift));
    FastList_FOREACH_Deadline_FastListTask(this, task) {
      unsigned required = FastListTask_REQUIREMENTS(task)[resource];
      int schedule = FastListTask_RELEASE(task);
      int deadline = FastListTask_DEADLINE(task);
      if (required > 0) {
        FastListRank rank = FastListTask_RANK(task);
        schedule = FastListRank_scheduleS(rank, shift, required);
        if (schedule >= deadline) return false;
      }
      *FastListTask__SCHEDULE(task) = schedule;
    } FastList_ENDEACH_Deadline_FastListTask;
  }
  return true;
}

#ifdef $XCC_h
/**
 * Check that this FastList schedule is feasible.
 */
bool
FastList_check(FastList this, Resource resource);
#endif//$XCC_h

bool
FastList_check(FastList this, Resource resource)
{
  int time = -1, consumed = 0;
  int available = FastList_AVAILABILITY(this)[resource];
  FastList_sortTasks(this, FastListType_Schedule, FastList_LISTS(this));
  FastList_FOREACH_Schedule_FastListTask(this, task) {
    int schedule = FastListTask_SCHEDULE(task);
    int required = FastListTask_REQUIREMENTS(task)[resource];
    if (time < schedule) {
      time = schedule;
      consumed = 0;
    }
    consumed += required;
    if (consumed > available) {
      return false;
    }
  } FastList_ENDEACH_Schedule_FastListTask;
  return true;
}

#ifdef $XCC_h
/**
 * Pretty-print this FastList.
 */
bool
FastList_pretty(FastList this, FILE *file);
#endif//$XCC_h

bool
FastList_pretty(FastList this, FILE *file)
{
  int time = -1;
  if (file == NULL) file = stderr;
  FastList_sortTasks(this, FastListType_Schedule, FastList_LISTS(this));
  FastList_FOREACH_Schedule_FastListTask(this, task) {
    int32_t index = FastListTask_INDEX(task);
    int schedule = FastListTask_SCHEDULE(task);
    if (time < schedule) {
      if (time >= 0) fprintf(file, "}");
      for (++time; time < schedule; time++) fprintf(file, "\t.");
      fprintf(file, "\t{%d", index);
    } else fprintf(file, ",%d", index);
  } FastList_ENDEACH_Schedule_FastListTask;
  fprintf(file, "}");
  return true;
}

#ifdef $XCC__c
{
  int n = 8, resource = 1;
  uint8_t availability[] = { 0, 1, 3 };
  uint8_t requirements[] = { 0, 1, 2 };
  FastListTask tasks[8];
  FastList_ fastList[1];
  FastList_Ctor(fastList, Memory_Root, n, availability);
  tasks[0] = FastList_makeTask(fastList, requirements, NULL);
  tasks[1] = FastList_makeTask(fastList, requirements, NULL);
  tasks[2] = FastList_makeTask(fastList, requirements, NULL);
  tasks[3] = FastList_makeTask(fastList, requirements, NULL);
  tasks[4] = FastList_makeTask(fastList, requirements, NULL);
  tasks[5] = FastList_makeTask(fastList, requirements, NULL);
  tasks[6] = FastList_makeTask(fastList, requirements, NULL);
  tasks[7] = FastList_makeTask(fastList, requirements, NULL);
  *FastListTask__RELEASE(tasks[0]) = 0;
  *FastListTask__RELEASE(tasks[1]) = 1;
  *FastListTask__RELEASE(tasks[2]) = 3;
  *FastListTask__RELEASE(tasks[3]) = 1;
  *FastListTask__RELEASE(tasks[4]) = 3;
  *FastListTask__RELEASE(tasks[5]) = 7;
  *FastListTask__RELEASE(tasks[6]) = 6;
  *FastListTask__RELEASE(tasks[7]) = 6;
  *FastListTask__DEADLINE(tasks[0]) = 1;
  *FastListTask__DEADLINE(tasks[1]) = 2;
  *FastListTask__DEADLINE(tasks[2]) = 7;
  *FastListTask__DEADLINE(tasks[3]) = 5;
  *FastListTask__DEADLINE(tasks[4]) = 7;
  *FastListTask__DEADLINE(tasks[5]) = 9;
  *FastListTask__DEADLINE(tasks[6]) = 8;
  *FastListTask__DEADLINE(tasks[7]) = 8;
  printf("\nreleases:");
  FastList_sortTasks(fastList, FastListType_Release, FastList_LISTS(fastList));
  FastList_FOREACH_Release_FastListTask(fastList, task) {
    int index = task - FastList_TASKS(fastList);
    int release = FastListTask_RELEASE(task);
    printf("\tr_%d=%d", index+1, release);
  } FastList_ENDEACH_Release_FastListTask;
  printf("\ndeadlines:");
  FastList_sortTasks(fastList, FastListType_Deadline, FastList_LISTS(fastList));
  FastList_FOREACH_Deadline_FastListTask(fastList, task) {
    int index = task - FastList_TASKS(fastList);
    int deadline = FastListTask_DEADLINE(task);
    printf("\td_%d=%d", index+1, deadline);
  } FastList_ENDEACH_Deadline_FastListTask;
  printf("\nranks before:\n");
  FastList_makeRanks(fastList, availability[resource]);
  FastList_FOREACH_FastListRank(fastList, rank) {
    int release = FastListRank_RELEASE(rank);
    int capacity = FastListRank_CAPACITY(rank);
    int consumed = FastListRank_CONSUMED(rank);
    printf("\trelease=%d\tcapacity=%d\t", release, capacity);
    FastList_FOREACH_FastListTask(fastList, task) {
      if (FastListTask_RANK(task) == rank) {
        int index = task - FastList_TASKS(fastList);
        printf("I_%d ", index+1);
      }
    } FastList_ENDEACH_FastListTask;
    printf("\n");
  } FastList_ENDEACH_FastListRank;
  for (resource = 1; resource < 3; resource++) {
    printf("\n%d processors, %d units per task:\n", availability[resource], requirements[resource]);
    FastList_schedule(fastList, resource);
    printf("ranks after:\n");
    FastList_FOREACH_FastListRank(fastList, rank) {
      int release = FastListRank_RELEASE(rank);
      int capacity = FastListRank_CAPACITY(rank);
      int consumed = FastListRank_CONSUMED(rank);
      printf("\trelease=%d\tcapacity=%d\tconsumed=%d\t", release, capacity, consumed);
      printf("\n");
    } FastList_ENDEACH_FastListRank;
    printf("schedule:\n");
    FastList_sortTasks(fastList, FastListType_Schedule, FastList_LISTS(fastList));
    FastList_FOREACH_Schedule_FastListTask(fastList, task) {
      int index = task - FastList_TASKS(fastList);
      int release = FastListTask_RELEASE(task);
      int schedule = FastListTask_SCHEDULE(task);
      int deadline = FastListTask_DEADLINE(task);
      printf("\trelease=%d\tschedule_%d=%d\tdeadline=%d\t", release, index+1, schedule, deadline);
      printf("\n");
    } FastList_ENDEACH_Schedule_FastListTask;
  }
  FastList_Dtor(fastList);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the FastList module.
 */
#define FastList_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the FastList module.
 */
#define FastList_FINI()
#endif//$XCC__h

#if XCC__C
static void
FastList_TEST(void)
{
#include "SCD/FastList_.c"
}

int
main(int argc, char **argv)
{
  SCD_Test(FastList_TEST);
  return 0;
}
#endif

