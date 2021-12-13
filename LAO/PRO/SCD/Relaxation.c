#ifdef $XCC_h
/*
 * !!!!	Relaxation.xcc
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
 * @brief Scheduling Relaxation based on  Leung-Palem-Pnueli.
 */
#endif//$XCC_h


#include "SCD/SCD_.h"

#ifdef $XCC__h
#include "SCD/Relaxation.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Implement a relaxation item to schedule.
 */
struct RelaxationItem_;
  //@args	const uint8_t *requirements, DDGraphNode node, int instance
#endif//$XCC_h

#ifdef $XCC__h
struct RelaxationItem_ {
  DDGraphNode NODE;		// The associated IssueItem.
  //@access INDEX	DDGraphNode_INDEX(RelaxationItem_NODE(this))
  const uint8_t *REQUIREMENTS;	// The Resource requirements.
  int16_t RELEASE;		// The release time of this RelaxationItem.
  int16_t DEADLINE;		// The release time of this RelaxationItem.
  int16_t LATENCY;		// Transitive latency from the first RelaxationItem.
  int16_t INSTANCE;		// Instance number in case of unwinding.
  FastListTask TASK;		// The associated FastListTask.
  GrahamListDates DATES;	// The EARLYDATE and LATEDATE of this RelaxationItem.
  //@access EARLYDATE	GrahamListDates_EARLYDATE(RelaxationItem_DATES(this))
  //@mutate EARLYDATE	GrahamListDates__EARLYDATE(RelaxationItem_DATES(this))
  //@access LATEDATE	GrahamListDates_LATEDATE(RelaxationItem_DATES(this))
  //@mutate LATEDATE	GrahamListDates__LATEDATE(RelaxationItem_DATES(this))
};
#endif//$XCC__h

RelaxationItem
RelaxationItem_Ctor(RelaxationItem this,
                    const uint8_t *requirements, DDGraphNode node, int instance)
{
  *RelaxationItem__NODE(this) = node;
  *RelaxationItem__REQUIREMENTS(this) = requirements;
  *RelaxationItem__RELEASE(this) = -1;
  *RelaxationItem__DEADLINE(this) = -1;
  *RelaxationItem__LATENCY(this) = INT16_MIN;
  *RelaxationItem__INSTANCE(this) = instance;
  *RelaxationItem__TASK(this) = NULL;
  *RelaxationItem__DATES(this) = NULL;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(RelaxationItem_)\t%zu\n", sizeof(RelaxationItem_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Pretty-print this RelaxationItem.
 */
bool
RelaxationItem_pretty(RelaxationItem this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
RelaxationItem_pretty(RelaxationItem this, FILE *file)
{
  int index = RelaxationItem_INDEX(this);
  int release = RelaxationItem_RELEASE(this);
  int deadline = RelaxationItem_DEADLINE(this);
  int instance = RelaxationItem_INSTANCE(this);
  int latency = RelaxationItem_LATENCY(this);
  int earlydate = RelaxationItem_EARLYDATE(this);
  int latedate = RelaxationItem_LATEDATE(this);
  if (file == NULL) file = stderr;
  if (instance == 0) fprintf(file, "    _%d  ", index);
  else fprintf(file, "    _%d[%d]", index, instance);
  fprintf(file, "\tearlydate=%d\tlatedate=%d", earlydate, latedate);
  if (latency >= 0) fprintf(file, "\tlatency=%d", latency);
  else fprintf(file, "\t\t");
  fprintf(file, "\trelease=%d\tdeadline=%d", release, deadline);
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Table of RelaxationItem(s) indexed by node and instance.
 */
struct RelaxationItemTable_;
  //@args	Memory memory, int unwinding,
  //@args	int32_t listCount, int16_t *_lambda
#endif//$XCC_h

#ifdef $XCC__h
struct RelaxationItemTable_ {
  IStack_ ITEMS_;		// The inlined RelaxationItem(s).
  //@access ITEMS	RelaxationItemTable__ITEMS_(this)
  //@access MEMORY	IStack_memory(RelaxationItemTable_ITEMS(this))
  //@access BASEITEM	(RelaxationItem)IStack_base(RelaxationItemTable_ITEMS(this))
  //@access PASTITEM	(RelaxationItem)IStack_past(RelaxationItemTable_ITEMS(this))
  IArray_ IARRAY_;		// Mapping array from (INDEX, ITER) to ITEM.
  //@access IARRAY	RelaxationItemTable__IARRAY_(this)
  //@access MAPITEMS	(RelaxationItem*)IArray_base(RelaxationItemTable_IARRAY(this))
  int32_t LISTCOUNT;		// The number of items of the underlying list scheduler.
  //@access DIMENSION	(RelaxationItemTable_LISTCOUNT(this) + 1)
  int16_t *_LAMBDA;		// This Relaxation LAMBDA.
  //@access LAMBDA	(*RelaxationItemTable__LAMBDA(this))
};
#endif//$XCC__h

RelaxationItemTable
RelaxationItemTable_Ctor(RelaxationItemTable this,
                         Memory memory, int unwinding,
                         int32_t listCount, int16_t *_lambda)
{
  int32_t tableCount = listCount*(unwinding + 1);
  int32_t arrayCount = (listCount + 1)*(unwinding + 1);
  Except_REQUIRE(unwinding >= 0);
  IStack_Ctor(RelaxationItemTable_ITEMS(this),
              memory, sizeof(RelaxationItem_), tableCount);
  IArray_Ctor(RelaxationItemTable_IARRAY(this), memory,
               sizeof(RelaxationItem), arrayCount);
  *RelaxationItemTable__LISTCOUNT(this) = listCount;
  *RelaxationItemTable___LAMBDA(this) = _lambda;
  return this;
}

void
RelaxationItemTable_Dtor(RelaxationItemTable this)
{
  IStack_Dtor(RelaxationItemTable_ITEMS(this));
  IArray_Dtor(RelaxationItemTable_IARRAY(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(RelaxationItemTable_)\t%zu\n", sizeof(RelaxationItemTable_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * For use by RelaxationItemTable_FOREACH_RelaxationItem.
 */
static inline IStack
RelaxationItemTable_items(RelaxationItemTable this)
{
  return RelaxationItemTable_ITEMS(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this RelaxationItemTable RelaxationItems.
 */
#define RelaxationItemTable_FOREACH_RelaxationItem(this, item) { \
  IStack RelaxationItemTable_ITEMS = RelaxationItemTable_items(this); \
  IStack_FOREACH(RelaxationItemTable_ITEMS, RelaxationItem_, item)
#define RelaxationItemTable_ENDEACH_RelaxationItem \
  IStack_ENDEACH \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this RelaxationItemTable RelaxationItems.
 */
#define RelaxationItemTable_FORBACK_RelaxationItem(this, item) { \
  IStack RelaxationItemTable_ITEMS = RelaxationItemTable_items(this); \
  IStack_FORBACK(RelaxationItemTable_ITEMS, RelaxationItem_, item)
#define RelaxationItemTable_ENDBACK_RelaxationItem \
  IStack_ENDBACK \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Find the next RelaxationItem in case of unwinding.
 */
static inline RelaxationItem
RelaxationItemTable_nextItem(RelaxationItemTable this, RelaxationItem item)
{
  int32_t listCount = RelaxationItemTable_LISTCOUNT(this);
  RelaxationItem pastItem = RelaxationItemTable_PASTITEM(this);
  RelaxationItem nextItem = item + listCount;
  if (nextItem >= pastItem) return NULL;
  Except_CHECK(RelaxationItem_NODE(nextItem) == RelaxationItem_NODE(item));
  Except_CHECK(RelaxationItem_INSTANCE(nextItem) == RelaxationItem_INSTANCE(item) + 1);
  return nextItem;
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Find the previous RelaxationItem in case of unwinding.
 */
static inline RelaxationItem
RelaxationItemTable_prevItem(RelaxationItemTable this, RelaxationItem item)
{
  int32_t listCount = RelaxationItemTable_LISTCOUNT(this);
  RelaxationItem baseItem = RelaxationItemTable_BASEITEM(this);
  RelaxationItem nextItem = item - listCount;
  if (nextItem < baseItem) return NULL;
  Except_CHECK(RelaxationItem_NODE(nextItem) == RelaxationItem_NODE(item));
  Except_CHECK(RelaxationItem_INSTANCE(nextItem) == RelaxationItem_INSTANCE(item) - 1);
  return nextItem;
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * RelaxationItemTable entry corresponding to index and instance.
 */
static inline RelaxationItem *
RelaxationItemTable_mapItem(RelaxationItemTable this, int32_t index, int instance)
{
  RelaxationItem *mapItems = RelaxationItemTable_MAPITEMS(this);
  int32_t dimension = RelaxationItemTable_DIMENSION(this);
  return mapItems + instance*dimension + index;
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Make a new RelaxationItem for this RelaxationItemTable.
 */
RelaxationItem
RelaxationItemTable_makeItem(RelaxationItemTable this, const uint8_t *requirements,
                             DDGraphNode node, int instance);
#endif//$XCC__h

RelaxationItem
RelaxationItemTable_makeItem(RelaxationItemTable this, const uint8_t *requirements,
                             DDGraphNode node, int instance)
{
  RelaxationItem newItem = IStack_push(RelaxationItemTable_ITEMS(this));
  RelaxationItem *mapItem = RelaxationItemTable_mapItem(this, DDGraphNode_INDEX(node),
                                                        instance);
  *mapItem = RelaxationItem_Ctor(newItem, requirements, node, instance);
  return newItem;
}

#ifdef $XCC__h
/**
 * Compute the transitive latencies from firstItem.
 */
void
RelaxationItemTable_computeLatencies(RelaxationItemTable this,
                                     RelaxationItem firstItem, int unwinding);
#endif//$XCC__h

void
RelaxationItemTable_computeLatencies(RelaxationItemTable this,
                                     RelaxationItem firstItem, int unwinding)
{
  int16_t lambda = RelaxationItemTable_LAMBDA(this);
  RelaxationItem pastItem = RelaxationItemTable_PASTITEM(this), tail_item;
  // Initialize the RelaxationItem LATENCY fields.
  for (tail_item = firstItem; tail_item < pastItem; tail_item++) {
    *RelaxationItem__LATENCY(tail_item) = INT16_MIN;
  }
  *RelaxationItem__LATENCY(firstItem) = 0;
  // Search the firstItem successors to make their transitive LATENCY.
  for (tail_item = firstItem; tail_item < pastItem; tail_item++) {
    int tail_latency = RelaxationItem_LATENCY(tail_item);
    if (tail_latency >= 0) {
      // This node is a transitive successor of firstItem.
      int tail_instance = RelaxationItem_INSTANCE(tail_item);
      DDGraphNode tail_node = RelaxationItem_NODE(tail_item);
      DDGraphNode_FORLEAVE_DDGraphArc(tail_node, arc) {
        int delay = DDGraphArc_DELAY(arc);
        DDGraphNode head_node = DDGraphArc_HEAD(arc);
        int32_t head_index = DDGraphNode_INDEX(head_node);
        RelaxationItem head_item = *RelaxationItemTable_mapItem(this, head_index, tail_instance);
        while (head_item != NULL && delay < 0) {
          head_item = RelaxationItemTable_nextItem(this, head_item);
          delay += lambda;
        }
        if (head_item != NULL && delay >= 0) {
          int new_latency = tail_latency + delay;
          if (RelaxationItem_LATENCY(head_item) < new_latency) {
            *RelaxationItem__LATENCY(head_item) = new_latency;
            Except_CHECK(new_latency >= 0);
          }
        }
      } DDGraphNode_ENDLEAVE_DDGraphArc;
      if (tail_instance < unwinding) {
        // Include the regularizing constraint.
        RelaxationItem next_item = RelaxationItemTable_nextItem(this, tail_item);
        if (next_item != NULL) {
          int new_latency = tail_latency + lambda;
          if (RelaxationItem_LATENCY(next_item) < new_latency) {
            *RelaxationItem__LATENCY(next_item) = new_latency;
          }
        }
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Update the deadlines to be consistent from firstItem.
 */
void
RelaxationItemTable_updateDeadlines(RelaxationItemTable this, RelaxationItem firstItem, int deadline);
#endif//$XCC__h

void
RelaxationItemTable_updateDeadlines(RelaxationItemTable this, RelaxationItem firstItem, int deadline)
{
  int16_t lambda = RelaxationItemTable_LAMBDA(this);
  RelaxationItem baseItem = RelaxationItemTable_BASEITEM(this), head_item;
  *RelaxationItem__DEADLINE(firstItem) = deadline;
  // Search the firstItem predecessors to make their DEADLINE consistent.
  for (head_item = firstItem; head_item >= baseItem; --head_item) {
    int head_deadline = RelaxationItem_DEADLINE(head_item);
    DDGraphNode head_node = RelaxationItem_NODE(head_item);
    DDGraphNode_FORENTER_DDGraphArc(head_node, arc) {
      int delay = DDGraphArc_DELAY(arc);
      DDGraphNode tail_node = DDGraphArc_TAIL(arc);
      int tail_index = DDGraphNode_INDEX(tail_node);
      RelaxationItem tail_item = *RelaxationItemTable_mapItem(this, tail_index, 0);
      if (tail_item != NULL && delay >= 0) {
        int tail_deadline = RelaxationItem_DEADLINE(tail_item);
        if (tail_deadline > head_deadline - delay) {
          *RelaxationItem__DEADLINE(tail_item) = head_deadline - delay;
        }
      }
    } DDGraphNode_ENDENTER_DDGraphArc;
  }
}

#ifdef $XCC_h
/**
 * Complete the deadlines in the stationary case.
 */
void
RelaxationItemTable_completeDeadlines(RelaxationItemTable this, RelaxationItem firstItem);
#endif//$XCC_h

void
RelaxationItemTable_completeDeadlines(RelaxationItemTable this, RelaxationItem firstItem)
{
  int16_t lambda = RelaxationItemTable_LAMBDA(this);
  RelaxationItem baseItem = RelaxationItemTable_BASEITEM(this), item;
  for (item = firstItem; item >= baseItem; --item) {
    int prev_deadline = RelaxationItem_DEADLINE(item) - lambda;
    RelaxationItem prev_item = RelaxationItemTable_prevItem(this, item);
    if (prev_item != NULL) *RelaxationItem__DEADLINE(prev_item) = prev_deadline;
    else break;
  }
}

#ifdef $XCC_h
/**
 * Pretty-print this RelaxationItemTable.
 */
bool
RelaxationItemTable_pretty(RelaxationItemTable this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
RelaxationItemTable_pretty(RelaxationItemTable this, FILE *file)
{
  if (file == NULL) file = stderr;
  RelaxationItemTable_FOREACH_RelaxationItem(this, item) {
    RelaxationItem_pretty(item, file);
    fprintf(file, "\n");
  } RelaxationItemTable_ENDEACH_RelaxationItem;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the RelaxationStep flags.
 */
typedef enum {
  RelaxationStepFlag_Sorted = 0x1	// RelaxationStep RelaxationItem(s) are sorted.
} enum_RelaxationStepFlag;
typedef uint8_t RelaxationStepFlags;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Implement a relaxation step.
 *
 * In a relaxation step, a list of RelaxationItems is available with their
 * release and deadline dates initialized. We first compute the transitive
 * latencies between the first RelaxationItem, and all the others. Then at each
 * scheduling step the release and the deadline dates of the FastListTask(s) are
 * updated using the transitive latencies, and the FastList scheduler is called
 * over the resources to check the feasibility of this RelaxationStep.
 *
 * Usage:
 * * RelaxationStep_new
 * * RelaxationStep_enterItems
 * * Relaxation_runRelaxationStep
 * * RelaxationStep_delete
 */
struct RelaxationStep_;
  //@args	Memory memory, RelaxationItemTable itemTable,
  //@args	RelaxationItem firstItem, const uint8_t *availability
#endif//$XCC_h

#ifdef $XCC__h
struct RelaxationStep_ {
  RelaxationItemTable ITEMTABLE;	// The RelaxationItemTable of this RelaxationStep.
  //@access PASTITEM	RelaxationItemTable_PASTITEM(RelaxationStep_ITEMTABLE(this))
  RelaxationItem FIRSTITEM;	// The first RelaxationItem of this RelaxationStep.
  //@access ITEMCOUNT	(RelaxationStep_PASTITEM(this) - RelaxationStep_FIRSTITEM(this))
  FastList_ FASTLIST_;		// This RelaxationStep FastList scheduler.
  //@access FASTLIST	RelaxationStep__FASTLIST_(this)
  //@access MEMORY	FastList_MEMORY(RelaxationStep_FASTLIST(this))
  Resource RESOURCE;	// The Resource used in this RelaxationStep.
  RelaxationStepFlags FLAGS;	// This RelaxationStep flags.
  //@access isSorted	((RelaxationStep_FLAGS(this) & RelaxationStepFlag_Sorted) != 0)
};
#endif//$XCC__h

RelaxationStep
RelaxationStep_Ctor(RelaxationStep this,
                    Memory memory, RelaxationItemTable itemTable,
                    RelaxationItem firstItem, const uint8_t *availability)
{
  int32_t itemCount = RelaxationItemTable_PASTITEM(itemTable) - firstItem;
  *RelaxationStep__ITEMTABLE(this) = itemTable;
  *RelaxationStep__FIRSTITEM(this) = firstItem;
  FastList_Ctor(RelaxationStep_FASTLIST(this), memory, itemCount, availability);
  *RelaxationStep__RESOURCE(this) = Resource__UNDEF;
  *RelaxationStep__FLAGS(this) = 0;
  return this;
}

void
RelaxationStep_Dtor(RelaxationStep this)
{
  FastList_Dtor(RelaxationStep_FASTLIST(this));
}

size_t
RelaxationStep_Size(Memory memory, RelaxationItemTable itemTable,
                    RelaxationItem firstItem, const uint8_t *availability) {
  return sizeof(RelaxationStep_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(RelaxationStep_)\t%zu\n", sizeof(RelaxationStep_));
}
#endif//$XCC__c

#ifdef $XCC_h
RelaxationStep
RelaxationStep_new(Memory parent, RelaxationItemTable itemTable,
                   RelaxationItem firstItem, const uint8_t *availability);
#endif//$XCC_h

RelaxationStep
RelaxationStep_new(Memory parent, RelaxationItemTable itemTable,
                   RelaxationItem firstItem, const uint8_t *availability)
{
  Memory memory = Memory_new(parent, true);
  size_t size = RelaxationStep_Size(memory, itemTable, firstItem, availability);
  RelaxationStep this = Memory_alloc(memory, size);
  return RelaxationStep_Ctor(this, memory, itemTable, firstItem, availability);
}

#ifdef $XCC_h
RelaxationStep
RelaxationStep_delete(RelaxationStep this);
#endif//$XCC_h

RelaxationStep
RelaxationStep_delete(RelaxationStep this)
{
  if (this != NULL) {
    Memory memory = RelaxationStep_MEMORY(this);
#ifndef _NDTOR
    RelaxationStep_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC__h
/**
 * For use by RelaxationStep_FOREACH_RelaxationItem.
 */
static inline RelaxationItem
RelaxationStep_firstItem(RelaxationStep this)
{
  return RelaxationStep_FIRSTITEM(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * For use by RelaxationStep_FOREACH_RelaxationItem.
 */
static inline RelaxationItem
RelaxationStep_pastItem(RelaxationStep this)
{
  return RelaxationStep_PASTITEM(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this RelaxationStep RelaxationItems.
 */
#define RelaxationStep_FOREACH_RelaxationItem(this, item) { \
  RelaxationItem RelaxationStep_FIRSTITEM = RelaxationStep_firstItem(this); \
  RelaxationItem RelaxationStep_PASTITEM = RelaxationStep_pastItem(this), item; \
  for (item = RelaxationStep_FIRSTITEM; item < RelaxationStep_PASTITEM; item++) {
#define RelaxationStep_ENDEACH_RelaxationItem \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Enter the RelaxationItem(s) of this RelaxationStep.
 */
bool
RelaxationStep_enterItems(RelaxationStep this);
#endif//$XCC__h

bool
RelaxationStep_enterItems(RelaxationStep this)
{
  FastList fastList = RelaxationStep_FASTLIST(this);
  RelaxationStep_FOREACH_RelaxationItem(this, item) {
    int release = RelaxationItem_RELEASE(item);
    int deadline = RelaxationItem_DEADLINE(item);
    if (release >= deadline) return false;
    else {
      const uint8_t *requirements = RelaxationItem_REQUIREMENTS(item);
      FastListTask task = FastList_makeTask(fastList, requirements, item);
      *RelaxationItem__TASK(item) = task;
    }
  } RelaxationStep_ENDEACH_RelaxationItem;
  return true;
}

#ifdef $XCC__h
/**
 * Try to schedule for the given release and deadline dates.
 */
bool
RelaxationStep_trySchedule(RelaxationStep this, int p, int q);
#endif//$XCC__h

#ifdef XCC__C
#define SCD_TRACE stdout
#endif

bool
RelaxationStep_trySchedule(RelaxationStep this, int p, int q)
{
  FastList fastList = RelaxationStep_FASTLIST(this);
  Resource resource = RelaxationStep_RESOURCE(this);
  Except_REQUIRE(resource != Resource__UNDEF);
  if (RelaxationStep_isSorted(this)) {
    // Update the release dates of the FastListTasks.
    FastList_updateRelease(fastList, p, q);
    // Update the deadline dates of the FastListTasks.
    FastList_updateDeadline(fastList, p, q);
  } else {
    FastListTask lists = FastList_LISTS(fastList);
    RelaxationItem firstItem = RelaxationStep_FIRSTITEM(this);
    // Initialize the release and the deadline dates of the FastListTasks.
    RelaxationStep_FOREACH_RelaxationItem(this, item) {
      FastListTask task = RelaxationItem_TASK(item);
      int release = RelaxationItem_RELEASE(item);
      int deadline = RelaxationItem_DEADLINE(item);
      int latency = RelaxationItem_LATENCY(item);
      if (item == firstItem) {
        release = p, deadline = q + 1;
      } else if (release < latency + q) {
        release = latency + q;
        Except_CHECK(latency >= 0);
      }
      if (FastListTask_RELEASE(task) != release) {
        *FastListTask__RELEASE(task) = release;
      }
      if (FastListTask_DEADLINE(task) != deadline) {
        *FastListTask__DEADLINE(task) = deadline;
      }
    } RelaxationStep_ENDEACH_RelaxationItem;
    FastList_sortTasks(fastList, FastListType_Release, lists);
    FastList_sortTasks(fastList, FastListType_Deadline, lists);
    *RelaxationStep__FLAGS(this) |= RelaxationStepFlag_Sorted;
  }
  // FastList schedule each resource.
  if (!FastList_schedule(fastList, resource)) return false;
#ifdef XCC__C
  SCD_TRACE && fprintf(SCD_TRACE, "\n"), FastList_pretty(fastList, SCD_TRACE), fprintf(SCD_TRACE, "\t");
#endif
  return true;
}

#ifdef $XCC__h
/**
 * Compute the modified deadline of the first RelaxationItem.
 * Return:	The modified deadline, or negative value if infeasible.
 */
int
RelaxationStep_modifyDeadline(RelaxationStep this, Resource resource);
#endif//$XCC__h

int
RelaxationStep_modifyDeadline(RelaxationStep this, Resource resource)
{
  RelaxationItem firstItem = RelaxationStep_FIRSTITEM(this);
  FastListTask first_task = RelaxationItem_TASK(firstItem);
  int first_instance = RelaxationItem_INSTANCE(firstItem);
  const uint8_t *first_requirements = FastListTask_REQUIREMENTS(first_task);
  static uint8_t zero_requirements[Resource__COUNT];
  int p = RelaxationItem_RELEASE(firstItem);
  int q = RelaxationItem_DEADLINE(firstItem) - 1;
  *RelaxationStep__RESOURCE(this) = resource;
  *RelaxationStep__FLAGS(this) &= ~RelaxationStepFlag_Sorted;
  SCD_TRACE && !first_instance && fprintf(SCD_TRACE, "    %s_%d\t",
      Resource_name(resource), RelaxationItem_INDEX(firstItem));
  SCD_TRACE && first_instance && fprintf(SCD_TRACE, "    %s_%d[%d]\t",
      Resource_name(resource), RelaxationItem_INDEX(firstItem), first_instance);
  Except_CHECK((unsigned)(resource - 1) < Resource__Count - 1);
  {
    // Phase 1: binary search for the maximal q without the firstTask.
    int l = 0, r = q - p, s = -1;
    *FastListTask__REQUIREMENTS(first_task) = zero_requirements;
    while (r >= l) {
      int i = (l + r) >> 1;
      bool feasible = RelaxationStep_trySchedule(this, p, p + i);
      if (feasible) {
        s = p + i;
        l = i + 1;
        SCD_TRACE && fprintf(SCD_TRACE, "OK1[%d,%d] ", p, p + i);
      } else {
        r = i - 1;
        SCD_TRACE && fprintf(SCD_TRACE, "KO1[%d,%d] ", p, p + i);
      }
    }
    *FastListTask__REQUIREMENTS(first_task) = first_requirements;
    SCD_TRACE && fprintf(SCD_TRACE, s < 0? "INFEASIBLE\n": "");
    if (s < 0) return s;
    q = s;
  }
  if (first_requirements[resource] != 0) {
    // Phase 2: binary search for the maximal p with the firstTask.
    int l = 0, r = q - p, s = -1;
    while (r >= l) {
      int i = (l + r) >> 1;
      bool feasible = RelaxationStep_trySchedule(this, p + i, q);
      if (feasible) {
        s = p + i;
        l = i + 1;
        SCD_TRACE && fprintf(SCD_TRACE, "OK2[%d,%d] ", p + i, q);
      } else {
        r = i - 1;
        SCD_TRACE && fprintf(SCD_TRACE, "KO2[%d,%d] ", p + i, q);
      }
    }
    SCD_TRACE && fprintf(SCD_TRACE, s < 0? "INFEASIBLE\n": "");
    if (s < 0) return s;
    p = s;
  } else {
    p = q;
  }
  SCD_TRACE && fprintf(SCD_TRACE, "%s=%d\n",
      p + 1 == RelaxationItem_DEADLINE(firstItem)? "modified": "MODIFIED", p + 1);
  return p + 1;
}

#ifdef $XCC__h
/**
 * Test if this RelaxationStep is stationary.
 */
bool
RelaxationStep_isStationary(RelaxationStep this, int distance);
#endif//$XCC__h

bool
RelaxationStep_isStationary(RelaxationStep this, int distance)
{
  int32_t itemCount = RelaxationStep_ITEMCOUNT(this);
  RelaxationItem firstItem = RelaxationStep_FIRSTITEM(this);
  RelaxationItemTable itemTable = RelaxationStep_ITEMTABLE(this);
  RelaxationItem baseItem = RelaxationItemTable_BASEITEM(itemTable);
  RelaxationItem pastItem = RelaxationItemTable_PASTITEM(itemTable);
  int32_t listCount = RelaxationItemTable_LISTCOUNT(itemTable);
  int16_t lambda = RelaxationItemTable_LAMBDA(itemTable);
  int32_t maxCount = listCount*distance;
  if (maxCount > itemCount) return false;
  RelaxationStep_FOREACH_RelaxationItem(this, item) {
    int next_deadline = RelaxationItem_DEADLINE(item) + lambda;
    RelaxationItem next_item = RelaxationItemTable_nextItem(itemTable, item);
    if (next_item != NULL && RelaxationItem_DEADLINE(next_item) == next_deadline) {
      if (--maxCount <= 0) return true;
    } else return false;
  } RelaxationStep_ENDEACH_RelaxationItem;
  return false;
}

#ifdef $XCC_h
/**
 * Pretty-print this RelaxationStep.
 */
bool
RelaxationStep_pretty(RelaxationStep this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
RelaxationStep_pretty(RelaxationStep this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "RelaxationStep\n");
  RelaxationStep_FOREACH_RelaxationItem(this, item) {
    RelaxationItem_pretty(item, file);
    fprintf(file, "\n");
  } RelaxationStep_ENDEACH_RelaxationItem;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * enumerates the Relaxation kinds.
 */
typedef enum {
  RelaxationKind_Forward,	// Forward scheduling Relaxation.
  RelaxationKind_Backward,	// Backward scheduling Relaxation.
  RelaxationKind__
} enum_RelaxationKind;
typedef uint8_t RelaxationKind;
extern const char *
RelaxationKind_Id(RelaxationKind this);
#endif//$XCC_h

const char *
RelaxationKind_Id(RelaxationKind this)
{
  static const char *_[] = {
    "Forward",
    "Backward",
  };
  Except_REQUIRE((unsigned)this < RelaxationKind__);
  return _[this];
}

#ifdef $XCC_h
/**
 * Implementation of a forward or backward relaxation for
 * scheduling problems.
 *
 * Based on "Scheduling Time-Constrained Instructions on Pipelined Processors" by
 * A. Leung, K. V. Palem, A. Pnueli. This relaxation framework has been extended
 * for dealing with multiple resources (Resources) and with forward relaxation.
 *
 * Usage of a Relaxation:
 * * Relaxation_new
 * * Relaxation_enterGrahamList
 * * Relaxation_readDates
 * * Relaxation_testFeasibility
 * * Relaxation_writeDates
 * * Relaxation_delete
 */
struct Relaxation_;
  //@args	Memory memory, RelaxationKind kind,
  //@args	GrahamList grahamList, int unwinding
#endif//$XCC_h

#ifdef $XCC__h
struct Relaxation_ {
  RelaxationItemTable_ ITEMTABLE_;
  //@access ITEMTABLE	Relaxation__ITEMTABLE_(this)
  //@access MEMORY	RelaxationItemTable_MEMORY(Relaxation_ITEMTABLE(this))
  const uint8_t *AVAILABILITY;	// This Relaxation resource availability.
  int16_t UNWINDING;		// This Relaxation unwinding level.
  RelaxationKind KIND;	// This RelaxationKind.
};
#endif//$XCC__h

Relaxation
Relaxation_Ctor(Relaxation this,
                Memory memory, RelaxationKind kind,
                GrahamList grahamList, int unwinding)
{
  int32_t listCount = GrahamList_COUNT(grahamList);
  const uint8_t *availability = GrahamList_AVAILABILITY(grahamList);
  int16_t *_lambda = GrahamList__LAMBDA(grahamList);
  RelaxationItemTable_Ctor(Relaxation_ITEMTABLE(this),
                           memory, unwinding, listCount, _lambda);
  *Relaxation__AVAILABILITY(this) = availability;
  *Relaxation__UNWINDING(this) = unwinding;
  *Relaxation__KIND(this) = kind;
  return this;
}

void
Relaxation_Dtor(Relaxation this)
{
  RelaxationItemTable_Dtor(Relaxation_ITEMTABLE(this));
}

size_t
Relaxation_Size(Memory memory, RelaxationKind kind,
                GrahamList grahamList, int unwinding) {
        return sizeof(Relaxation_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Relaxation_)\t%zu\n", sizeof(Relaxation_));
}
#endif//$XCC__c

#ifdef $XCC_h
Relaxation
Relaxation_new(Memory parent, RelaxationKind kind, GrahamList grahamList, int unwinding);
#endif//$XCC_h

Relaxation
Relaxation_new(Memory parent, RelaxationKind kind, GrahamList grahamList, int unwinding)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Relaxation_Size(memory, kind, grahamList, unwinding);
  Relaxation this = Memory_alloc(memory, size);
  Relaxation_Ctor(this, memory, kind, grahamList, unwinding);
  return this;
}

#ifdef $XCC_h
Relaxation
Relaxation_delete(Relaxation this);
#endif//$XCC_h

Relaxation
Relaxation_delete(Relaxation this)
{
  if (this != NULL) {
    Memory memory = Relaxation_MEMORY(this);
#ifndef _NDTOR
    Relaxation_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC__h
/**
 * For use by Relaxation_FOREACH_RelaxationItem.
 */
static inline RelaxationItemTable
Relaxation_itemTable(Relaxation this)
{
  return Relaxation_ITEMTABLE(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Relaxation RelaxationItems.
 */
#define Relaxation_FOREACH_RelaxationItem(this, item) { \
  RelaxationItemTable Relaxation_ITEMTABLE = Relaxation_itemTable(this); \
  RelaxationItemTable_FOREACH_RelaxationItem(Relaxation_ITEMTABLE, item)
#define Relaxation_ENDEACH_RelaxationItem \
  RelaxationItemTable_ENDEACH_RelaxationItem \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Relaxation RelaxationItems.
 */
#define Relaxation_FORBACK_RelaxationItem(this, item) { \
  RelaxationItemTable Relaxation_ITEMTABLE = Relaxation_itemTable(this); \
  RelaxationItemTable_FORBACK_RelaxationItem(Relaxation_ITEMTABLE, item)
#define Relaxation_ENDBACK_RelaxationItem \
  RelaxationItemTable_ENDBACK_RelaxationItem \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Enter a GrahamList for this Relaxation.
 */
void
Relaxation_enterGrahamList(Relaxation this, GrahamList grahamList);
#endif//$XCC__h

void
Relaxation_enterGrahamList(Relaxation this, GrahamList grahamList)
{
  RelaxationItemTable itemTable = Relaxation_itemTable(this);
  int16_t lambda = RelaxationItemTable_LAMBDA(itemTable);
  int unwinding = Relaxation_UNWINDING(this), instance = 0;
  do {
    if (Relaxation_KIND(this) == RelaxationKind_Backward) {
      GrahamList_FOREACH_GrahamListTask(grahamList, task) {
        IssueItem issueItem = GrahamListTask_ITEM(task);
        const uint8_t *requirements = IssueItem_REQUIREMENTS(issueItem);
        DDGraphNode early_node = IssueItem_EARLYNODE(issueItem);
        DDGraphNode late_node = IssueItem_LATENODE(issueItem);
        RelaxationItem item = RelaxationItemTable_makeItem(itemTable, requirements,
                                                           early_node, instance);
        *RelaxationItem__DATES(item) = GrahamListTask_DATES(task) + instance;
        *RelaxationItem__EARLYDATE(item) = DDGraphNode_DATE(early_node)
                                         + lambda*instance;
        *RelaxationItem__LATEDATE(item) = DDGraph_EXTENT - DDGraphNode_DATE(late_node)
                                        + lambda*instance;
      } GrahamList_ENDEACH_GrahamListTask;
    } else if (Relaxation_KIND(this) == RelaxationKind_Forward) {
      GrahamList_FORBACK_GrahamListTask(grahamList, task) {
        IssueItem issueItem = GrahamListTask_ITEM(task);
        const uint8_t *requirements = IssueItem_REQUIREMENTS(issueItem);
        DDGraphNode early_node = IssueItem_EARLYNODE(issueItem);
        DDGraphNode late_node = IssueItem_LATENODE(issueItem);
        RelaxationItem item = RelaxationItemTable_makeItem(itemTable, requirements,
                                                           late_node, instance);
        *RelaxationItem__DATES(item) = GrahamListTask_DATES(task) + instance;
        *RelaxationItem__EARLYDATE(item) = DDGraphNode_DATE(early_node)
                                         + lambda*instance;
        *RelaxationItem__LATEDATE(item) = DDGraph_EXTENT - DDGraphNode_DATE(late_node)
                                        + lambda*instance;
      } GrahamList_ENDBACK_GrahamListTask;
    } else Except_NEVER(true);
  } while (instance++ < unwinding);
}

#ifdef $XCC__h
/**
 * Read the EARLYDATEs/LATEDATEs into the RELEASE/DEADLINE dates.
 */
void
Relaxation_readDates(Relaxation this);
#endif//$XCC__h

void
Relaxation_readDates(Relaxation this)
{
  RelaxationKind kind = Relaxation_KIND(this);
  if (kind == RelaxationKind_Backward) {
    Relaxation_FOREACH_RelaxationItem(this, item) {
      int release = RelaxationItem_EARLYDATE(item);
      int deadline = RelaxationItem_LATEDATE(item) + 1;
      Except_CHECK(release >= 0 && release < deadline);
      *RelaxationItem__RELEASE(item) = release;
      *RelaxationItem__DEADLINE(item) = deadline;
    } Relaxation_ENDEACH_RelaxationItem;
  } else if (kind == RelaxationKind_Forward) {
    Relaxation_FOREACH_RelaxationItem(this, item) {
      int release = DDGraph_EXTENT - RelaxationItem_LATEDATE(item);
      int deadline = DDGraph_EXTENT - RelaxationItem_EARLYDATE(item) + 1;
      Except_CHECK(release >= 0 && release < deadline);
      *RelaxationItem__RELEASE(item) = release;
      *RelaxationItem__DEADLINE(item) = deadline;
    } Relaxation_ENDEACH_RelaxationItem;
  } else Except_NEVER(true);
}

#ifdef $XCC__h
/**
 * Write the EARLYDATEs/LATEDATEs from the RELEASE/DEADLINE dates.
 */
void
Relaxation_writeDates(Relaxation this);
#endif//$XCC__h

void
Relaxation_writeDates(Relaxation this)
{
  RelaxationKind kind = Relaxation_KIND(this);
  if (kind == RelaxationKind_Backward) {
    Relaxation_FOREACH_RelaxationItem(this, item) {
      int deadline = RelaxationItem_DEADLINE(item);
      *RelaxationItem__LATEDATE(item) = deadline - 1;
    } Relaxation_ENDEACH_RelaxationItem;
  } else if (kind == RelaxationKind_Forward) {
    Relaxation_FOREACH_RelaxationItem(this, item) {
      int deadline = RelaxationItem_DEADLINE(item);
      *RelaxationItem__EARLYDATE(item) = DDGraph_EXTENT - deadline + 1;
    } Relaxation_ENDEACH_RelaxationItem;
  } else Except_NEVER(true);
}

#ifdef $XCC__h
/**
 * Run a RelaxationStep of this Relaxation.
 */
bool
Relaxation_runRelaxationStep(Relaxation this, RelaxationStep relaxationStep,
                             int issueCount, const Resource *resources);
#endif//$XCC__h

bool
Relaxation_runRelaxationStep(Relaxation this, RelaxationStep relaxationStep,
                             int issueCount, const Resource *resources)
{
  RelaxationItemTable itemTable = Relaxation_ITEMTABLE(this);
  RelaxationItem baseItem = RelaxationItemTable_BASEITEM(itemTable);
  RelaxationItem firstItem = RelaxationStep_FIRSTITEM(relaxationStep);
  const uint8_t *requirements = RelaxationItem_REQUIREMENTS(firstItem);
  int unwinding = Relaxation_UNWINDING(this), issuePriority = 0;
  RelaxationItemTable_computeLatencies(itemTable, firstItem, unwinding);
  for (issuePriority = 0; issuePriority < issueCount; issuePriority++) {
    Resource resource = resources[issuePriority];
    Except_CHECK((unsigned)(resource - 1) < Resource__Count - 1);
    if (firstItem == baseItem || requirements[resource] != 0) {
      int deadline = RelaxationStep_modifyDeadline(relaxationStep, resource);
      if (deadline < 0) return false;
      Except_CHECK(deadline <= RelaxationItem_DEADLINE(firstItem));
      if (deadline < RelaxationItem_DEADLINE(firstItem)) {
        RelaxationItemTable itemTable = Relaxation_ITEMTABLE(this);
        RelaxationItemTable_updateDeadlines(itemTable, firstItem, deadline);
      }
    }
  }
  return true;
}

#ifdef $XCC__h
/**
 * Test this Relaxation feasibility status.
 */
bool
Relaxation_testFeasibility(Relaxation this, int issueCount, const Resource *resources);
#endif//$XCC__h

bool
Relaxation_testFeasibility(Relaxation this, int issueCount, const Resource *resources)
{
  int distance = 1;	//FIXME! should be _MIN(_MAX(OMEGA_i), OVERLAP+1)
  int unwinding = Relaxation_UNWINDING(this);
  const uint8_t *availability = Relaxation_AVAILABILITY(this);
  RelaxationItemTable itemTable = Relaxation_itemTable(this);
  int16_t lambda = RelaxationItemTable_LAMBDA(itemTable);
  RelaxationItemTable_FORBACK_RelaxationItem(itemTable, firstItem) {
    RelaxationStep relaxationStep = RelaxationStep_new(Relaxation_MEMORY(this), itemTable,
                                                       firstItem, availability);
    // Enter the RelaxationItem(s) and run the relaxationStep.
    bool feasible = RelaxationStep_enterItems(relaxationStep) &&
        Relaxation_runRelaxationStep(this, relaxationStep, issueCount, resources);
    if (feasible && distance > 0 &&
        RelaxationItem_INSTANCE(firstItem) < unwinding &&
        RelaxationStep_isStationary(relaxationStep, distance)) {
      SCD_TRACE && fprintf(SCD_TRACE, "*** STATIONARY _%d[%d]\tdistance=%d\n",
          RelaxationItem_INDEX(firstItem), RelaxationItem_INSTANCE(firstItem), distance);
      // Stationary RelaxationStep detected: complete the deadlines.
      RelaxationItemTable_completeDeadlines(itemTable, firstItem);
      RelaxationStep_delete(relaxationStep);
      return true;
    }
    RelaxationStep_delete(relaxationStep);
    if (!feasible) return false;
  } RelaxationItemTable_ENDBACK_RelaxationItem;
  return true;
}

#ifdef $XCC_h
/**
 * Pretty-print this Relaxation.
 */
bool
Relaxation_pretty(Relaxation this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Relaxation_pretty(Relaxation this, FILE *file)
{
  RelaxationKind kind = Relaxation_KIND(this);
  int unwinding = Relaxation_UNWINDING(this);
  RelaxationItemTable itemTable = Relaxation_itemTable(this);
  int16_t lambda = RelaxationItemTable_LAMBDA(itemTable);
  if (file == NULL) file = stderr;
  fprintf(file, "Relaxation_%s\tunwinding=%d\tlambda=%d\n",
      RelaxationKind_Id(kind), unwinding, lambda);
  RelaxationItemTable_pretty(itemTable, file);
  return true;
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int i, j, k;
  int16_t lambda = 0;
  static DDGraphNode nodes[10];
  static Resource resources[1] = { 1 };
  static uint8_t availability[] = { 0, 1, 3 };
  static uint8_t requirements[] =  { 0, 1, 2 };
  static int8_t releases[] =  { -1, 0, 1, 3, 1, 3, 7, 6, 6 };
  static int8_t deadlines[] = { -1, 1, 2, 7, 5, 7, 9, 8, 8 };
  DDGraph ddgraph = DDGraph_new(Memory_Root, DDGraphKind_Early, 0,
                                Processor__UNDEF + 1, 10, &lambda);
  DDGraphNode node_0 = DDGraph_makeNode(ddgraph, NULL);
  DDGraphNode node_1 = DDGraph_makeNode(ddgraph, NULL);
  DDGraphNode node_2 = DDGraph_makeNode(ddgraph, NULL);
  DDGraphNode node_3 = DDGraph_makeNode(ddgraph, NULL);
  DDGraphNode node_4 = DDGraph_makeNode(ddgraph, NULL);
  DDGraphNode node_5 = DDGraph_makeNode(ddgraph, NULL);
  DDGraphNode node_6 = DDGraph_makeNode(ddgraph, NULL);
  DDGraphNode node_7 = DDGraph_makeNode(ddgraph, NULL);
  DDGraphNode node_8 = DDGraph_makeNode(ddgraph, NULL);
  GrahamList grahamList = GrahamList_new(Memory_Root, ddgraph, availability);
  Relaxation_ relaxation[1];
  for (i = 0; i < 9; i++) IStack_PUSH(GrahamList_TASKS(grahamList), GrahamListTask, NULL);
  {
    DDGraphArc arc_1_2 = DDGraph_makeArc(ddgraph, node_1, node_2, 1+0, 0, DependenceKind_Some, NULL);
    DDGraphArc arc_1_4 = DDGraph_makeArc(ddgraph, node_1, node_4, 1+0, 0, DependenceKind_Some, NULL);
    DDGraphArc arc_2_3 = DDGraph_makeArc(ddgraph, node_2, node_3, 1+0, 0, DependenceKind_Some, NULL);
    DDGraphArc arc_2_5 = DDGraph_makeArc(ddgraph, node_2, node_5, 1+0, 0, DependenceKind_Some, NULL);
    DDGraphArc arc_3_6 = DDGraph_makeArc(ddgraph, node_3, node_6, 1+1, 0, DependenceKind_Some, NULL);
    DDGraphArc arc_4_3 = DDGraph_makeArc(ddgraph, node_4, node_3, 1+1, 0, DependenceKind_Some, NULL);
    DDGraphArc arc_4_5 = DDGraph_makeArc(ddgraph, node_4, node_5, 1+1, 0, DependenceKind_Some, NULL);
    DDGraphArc arc_7_6 = DDGraph_makeArc(ddgraph, node_7, node_6, 1+0, 0, DependenceKind_Some, NULL);
    nodes[0] = node_0; nodes[1] = node_1; nodes[2] = node_2; nodes[3] = node_3; nodes[4] = node_4;
    nodes[5] = node_5; nodes[6] = node_6; nodes[7] = node_7; nodes[8] = node_8; nodes[9] = NULL;
  }
  Relaxation_Ctor(relaxation, Memory_Root, RelaxationKind_Backward, grahamList, 0);
  // Enter in lower release date first order.
  for (i = 1; i < 9; i++) {
    int release = INT_MAX, k = -1;
    for (j = 1; j < 9; j++) {
      if (nodes[j] == NULL) continue;
      if (release > releases[j]) {
        release = releases[j];
        k = j;
      }
    }
    if (k >= 0) {
      RelaxationItemTable itemTable = Relaxation_itemTable(relaxation);
      RelaxationItem item = RelaxationItemTable_makeItem(itemTable, requirements, nodes[k], 0);
      *RelaxationItem__RELEASE(item) = releases[k];
      *RelaxationItem__DEADLINE(item) = deadlines[k];
      nodes[k] = NULL;
    }
  }
  printf("\n");
  Relaxation_FORBACK_RelaxationItem(relaxation, firstItem) {
    bool feasible = false;
    RelaxationItemTable itemTable = Relaxation_itemTable(relaxation);
    RelaxationStep_ relaxationStep[1];
    RelaxationStep_Ctor(relaxationStep, Memory_Root, itemTable, firstItem, availability);
    feasible = RelaxationStep_enterItems(relaxationStep);
    RelaxationStep_FOREACH_RelaxationItem(relaxationStep, item) {
      FastListTask task = RelaxationItem_TASK(item);
      int release = RelaxationItem_RELEASE(item);
      int deadline = RelaxationItem_DEADLINE(item);
      int latency = RelaxationItem_LATENCY(item);
      int index = RelaxationItem_INDEX(item);
      printf("{r=%d d=%d}_%d\t", release, deadline, index);
    } RelaxationStep_ENDEACH_RelaxationItem;
    Relaxation_runRelaxationStep(relaxation, relaxationStep, 1, resources);
    printf("\n");
    RelaxationStep_Dtor(relaxationStep);
  } Relaxation_ENDBACK_RelaxationItem;
  Relaxation_Dtor(relaxation);
  GrahamList_delete(grahamList);
  DDGraph_delete(ddgraph);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Relaxation module.
 */
void
Relaxation_INIT(void);
#endif//$XCC__h

void
Relaxation_INIT(void)
{
}

#ifdef $XCC__h
/**
 * Finalize the Relaxation module.
 */
void
Relaxation_FINI(void);
#endif//$XCC__h

void
Relaxation_FINI(void)
{
}

#if XCC__C
static void
Relaxation_TEST(void)
{
#include "SCD/Relaxation_.c"
}

int
main(int argc, char **argv)
{
  SCD_Test(Relaxation_TEST);
  return 0;
}
#endif

