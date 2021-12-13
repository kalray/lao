#ifdef $XCC_h
/*
 * !!!!	Insertion.xcc
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
 * @brief Insertion instruction scheduling.
 */
#endif//$XCC_h


#include "SCD/SCD_.h"

#ifdef $XCC__h
#include "SCD/Insertion.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Information for a particular Insertion slot.
 */
struct InsertionSlot_ {
  //@args	const uint8_t *availability, int tau, int phi
  IssueGroup_ GROUP_M_;	// IssueGroup_M from group splitting during insertion.
  //@access GROUP_M	InsertionSlot__GROUP_M_(this)
  IssueGroup_ GROUP_P_;	// IssueGroup_P from group splitting during insertion.
  //@access GROUP_P	InsertionSlot__GROUP_P_(this)
  int16_t TAU;			// STARTDATE mod LAMBDA.
  int16_t PHI;			// STARTDATE div LAMBDA.
  int8_t DELTA_M;		// DELTA_M value for Insertion.
  int8_t DELTA_P;		// DELTA_P value for Insertion.
};
#endif//$XCC__h

InsertionSlot
InsertionSlot_Ctor(InsertionSlot this,
                   const uint8_t *availability, int tau, int phi)
{
  IssueGroup_Ctor(InsertionSlot_GROUP_M(this), availability);
  IssueGroup_Ctor(InsertionSlot_GROUP_P(this), availability);
  *InsertionSlot__TAU(this) = tau;
  *InsertionSlot__PHI(this) = phi;
  *InsertionSlot__DELTA_M(this) = 0;
  *InsertionSlot__DELTA_P(this) = 0;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(InsertionSlot_)\t%zu\n", sizeof(InsertionSlot_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Context for the insertion scheduling engine.
 */
struct Insertion_;
#endif//$XCC_h

#ifdef $XCC__h
struct Insertion_ {
  //@args	Memory memory, Scheduler scheduler, int maxRange
  Memory MEMORY;
  Scheduler SCHEDULER;		// This Insertion Scheduler.
  int16_t MAXRANGE;		// Maximum range of start dates tried.
  int16_t SLOTCOUNT;		// The count of Insertion SLOTS.
  InsertionSlot_ SLOTS[2];	// The candidate InsertionSlots.
};
#endif//$XCC__h

Insertion
Insertion_Ctor(Insertion this,
               Memory memory, Scheduler scheduler, int maxRange)
{
  *Insertion__MEMORY(this) = memory;
  *Insertion__SCHEDULER(this) = scheduler;
  *Insertion__MAXRANGE(this) = maxRange;
  *Insertion__SLOTCOUNT(this) = 0;
  memset(Insertion_SLOTS(this), 0, maxRange*sizeof(InsertionSlot_));
  return this;
}

void
Insertion_Dtor(Insertion this)
{
}

size_t
Insertion_Size(Memory memory, Scheduler scheduler, int maxRange)
{
  return (sizeof(Insertion_) + (maxRange - 2)*sizeof(InsertionSlot_));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Insertion_)\t%zu\n", sizeof(Insertion_));
}
#endif//$XCC__c

#ifdef $XCC_h
Insertion
Insertion_new(Memory parent, Scheduler scheduler, int maxRange);
#endif//$XCC_h

Insertion
Insertion_new(Memory parent, Scheduler scheduler, int maxRange)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Insertion_Size(memory, scheduler, maxRange);
  Insertion this = Memory_alloc(memory, size);
  return Insertion_Ctor(this, memory, scheduler, maxRange);
}

#ifdef $XCC_h
Insertion
Insertion_delete(Insertion this);
#endif//$XCC_h

Insertion
Insertion_delete(Insertion this)
{
  if (this != NULL) {
    Memory memory = Insertion_MEMORY(this);
#ifndef _NDTOR
    Insertion_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC__h
/**
 * Select the next GrahamListTask to issue.
 */
GrahamListTask
Insertion_selectTask(Insertion this);
#endif//$XCC__h

GrahamListTask
Insertion_selectTask(Insertion this)
{
  Scheduler scheduler = Insertion_SCHEDULER(this);
  GrahamList grahamList = Scheduler_GRAHAMLIST(scheduler);
  int16_t lambda = Scheduler_LAMBDA(scheduler);
  bool isPrePass = Scheduler_isPrePass(scheduler);
  GrahamList_FOREACH_TOISSUE_GrahamListTask(grahamList, TASK) {
    IssueItem ITEM = GrahamListTask_ITEM(TASK);
    DDGraphNode mcrl_node = IssueItem_MCRLNODE(ITEM);
    DDGraphNode early_node = IssueItem_EARLYNODE(ITEM);
    DDGraphNode late_node = IssueItem_LATENODE(ITEM);
    Operation operation = IssueItem_OPERATION(ITEM);
    bool isInductive = Operation_isInductive(operation);
    int mcrlDate = DDGraphNode_DATE(mcrl_node);
    int earlyDate = DDGraphNode_DATE(early_node);
    int lateDate = DDGraph_EXTENT - DDGraphNode_DATE(late_node);
    int slackCycles = lateDate - earlyDate;
    if (isPrePass) {
      // Previous best PRIORITY1(TASK) = mcrlDate;
      // Absolute best PRIORITY1(TASK) = mcrlDate + slackCycles/lambda;
      *GrahamListTask__PRIORITY1(TASK) = isInductive? lateDate + slackCycles/lambda:
                                                      mcrlDate + slackCycles/lambda;
      *GrahamListTask__PRIORITY2(TASK) = GrahamListTask_LATEDATE(TASK) + slackCycles;
    } else {
      *GrahamListTask__PRIORITY1(TASK) = earlyDate + (slackCycles + 1)/2;
      *GrahamListTask__PRIORITY2(TASK) = GrahamListTask_LATEDATE(TASK);
    }
  } GrahamList_ENDEACH_TOISSUE_GrahamListTask;
  IDList_sort(GrahamList_TOISSUE(grahamList), GrahamListTask_compare);
  return (GrahamListTask)IDList_firstItem(GrahamList_TOISSUE(grahamList));
}

#ifdef $XCC__h
InsertionSlot
Insertion_makeBestSlot(Insertion this, IssueItem item);
#endif//$XCC__h

InsertionSlot
Insertion_makeBestSlot(Insertion this, IssueItem item)
{
  int slotCount = Insertion_SLOTCOUNT(this);
  InsertionSlot slots = Insertion_SLOTS(this);
  Scheduler scheduler = Insertion_SCHEDULER(this);
  IssueTable issueTable = Scheduler_ISSUETABLE(scheduler);
  GrahamList grahamList = Scheduler_GRAHAMLIST(scheduler);
  GrahamListTask *index2task = GrahamList_INDEX2TASK(grahamList);
  int delta = INT_MAX, bestDelta = INT_MAX, bestIndex = 0, i;
  SCD_TRACE && fprintf(SCD_TRACE, "deltas:");
  for (i = 0; i < slotCount; i++) {
    InsertionSlot slot = slots + i;
    int16_t tau = InsertionSlot_TAU(slot);
    IssueGroup group = IssueTable_access(issueTable, tau);
    if (IssueGroup_accept(group, item)) {
      *InsertionSlot__DELTA_M(slot) = 0;
      *InsertionSlot__DELTA_P(slot) = 0;
    } else {
      // Split the group according to the insertion order.
      IssueGroup_FOREACH_IssueItem(group, ITEM) {
        GrahamListTask TASK = index2task[IssueItem_INDEX(ITEM)];
        Except_CHECK(GrahamListTask_ITEM(TASK) == ITEM);
        if (   GrahamListTask_PHI(TASK) > InsertionSlot_PHI(slot)
            || (   GrahamListTask_PHI(TASK) == InsertionSlot_PHI(slot)
                && IssueItem_ORDERING(ITEM) < IssueItem_ORDERING(item))) {
          IssueGroup_insert(InsertionSlot_GROUP_M(slot), ITEM);
        } else {
          IssueGroup_insert(InsertionSlot_GROUP_P(slot), ITEM);
        }
      } IssueGroup_ENDEACH_IssueItem;
      if (IssueGroup_accept(InsertionSlot_GROUP_M(slot), item)) {
        *InsertionSlot__DELTA_M(slot) = 0;
        *InsertionSlot__DELTA_P(slot) = 1;
      } else if (IssueGroup_accept(InsertionSlot_GROUP_P(slot), item)) {
        *InsertionSlot__DELTA_M(slot) = 1;
        *InsertionSlot__DELTA_P(slot) = 0;
      } else {
        *InsertionSlot__DELTA_M(slot) = 1;
        *InsertionSlot__DELTA_P(slot) = 1;
      }
    }
    delta = InsertionSlot_DELTA_M(slot) + InsertionSlot_DELTA_P(slot);
    SCD_TRACE && fprintf(SCD_TRACE, "%2d", delta);
    if (delta < bestDelta) {
      bestDelta = delta, bestIndex = i;
      if (bestDelta == 0) break;
    }
  }
  Except_CHECK(slotCount > 0 && bestIndex < slotCount);
  SCD_TRACE && fprintf(SCD_TRACE, "\n");
  return slots + bestIndex;
}

#ifdef $XCC__h
/**
 * Select an InsertionSlot for the task.
 */
InsertionSlot
Insertion_selectSlot(Insertion this, GrahamListTask task);
#endif//$XCC__h

InsertionSlot
Insertion_selectSlot(Insertion this, GrahamListTask task)
{
  Scheduler scheduler = Insertion_SCHEDULER(this);
  IssueTable issueTable = Scheduler_ISSUETABLE(scheduler);
  const uint8_t *availability = IssueTable_availability(issueTable);
  InsertionSlot slots = Insertion_SLOTS(this);
  IssueItem item = GrahamListTask_ITEM(task);
  int16_t lambda = Scheduler_LAMBDA(scheduler);
  int16_t slotCount = 1, tau1, tau2, phi1, phi2;
  DDGraphNode mcrl_node = IssueItem_MCRLNODE(item);
  DDGraphNode early_node = IssueItem_EARLYNODE(item);
  DDGraphNode late_node = IssueItem_LATENODE(item);
  int mcrlDate = DDGraphNode_DATE(mcrl_node);
  int earlyDate = DDGraphNode_DATE(early_node);
  int lateDate = DDGraph_EXTENT - DDGraphNode_DATE(late_node);
  int16_t tau = DDGraphNode_DATE(mcrl_node)%lambda;
  int16_t phi = DDGraphNode_DATE(mcrl_node)/lambda;
  int range0 = _MIN(Insertion_MAXRANGE(this) - 1, lambda - 1), i;
  int range1 = _MIN(lateDate - mcrlDate, range0);
  int range2 = _MIN(mcrlDate - earlyDate, range0 - range1);
  while (tau >= lambda) tau -= lambda, phi += 1;
  while (tau < 0) tau += lambda, phi -= 1;
  InsertionSlot_Ctor(slots, availability, tau, phi);
  tau1 = tau + 1, tau2 = tau - 1, phi1 = phi, phi2 = phi;
  for (i = 0; i < range1; i++, tau1++, slotCount++) {
    if (tau1 >= lambda) tau1 -= lambda, phi1 += 1;
    InsertionSlot_Ctor(slots + slotCount, availability, tau1, phi1);
  }
  for (i = 0; i < range2; i++, tau2--, slotCount++) {
    if (tau2 < 0) tau2 += lambda, phi2 -= 1;
    InsertionSlot_Ctor(slots + slotCount, availability, tau2, phi2);
  }
  if (slotCount == 0) {
    InsertionSlot_Ctor(slots + slotCount, availability, tau, phi);
    slotCount++;
  }
  Except_CHECK(slotCount <= Insertion_MAXRANGE(this));
  *Insertion__SLOTCOUNT(this) = slotCount;
  SCD_TRACE && fprintf(SCD_TRACE, "    %s_%d\t",
      Scheduling_name(IssueItem_SCHEDULING(item)), IssueItem_INDEX(item));
  SCD_TRACE && fprintf(SCD_TRACE, "phi=%d\ttau=%d\tdates:%3d+[%d,%d] \t",
      phi, tau, mcrlDate, earlyDate - mcrlDate, lateDate - mcrlDate);
  return Insertion_makeBestSlot(this, item);
}

#ifdef $XCC__h
/**
 * Schedule the previously selected GrahamListTask.
 */
int
Insertion_scheduleTaskAtSlot(Insertion this, GrahamListTask task, InsertionSlot slot);
#endif//$XCC__h

int
Insertion_scheduleTaskAtSlot(Insertion this, GrahamListTask task, InsertionSlot slot)
{
  IssueItem item = GrahamListTask_ITEM(task);
  Scheduler scheduler = Insertion_SCHEDULER(this);
  GrahamList grahamList = Scheduler_GRAHAMLIST(scheduler);
  IssueTable issueTable = Scheduler_ISSUETABLE(scheduler);
  int16_t delta_m = InsertionSlot_DELTA_M(slot);
  int16_t delta_p = InsertionSlot_DELTA_P(slot);
  int16_t lambda = Scheduler_LAMBDA(scheduler);
  int32_t ordering = IssueItem_ORDERING(item);
  int16_t tau = InsertionSlot_TAU(slot);
  int16_t phi = InsertionSlot_PHI(slot);
  int16_t delta = delta_m + delta_p;
  int16_t increase = delta;
  *GrahamListTask__TAU(task) = tau;
  *GrahamListTask__PHI(task) = phi;
  Except_CHECK(phi >= 0 && tau >= 0 && tau < lambda);
  Scheduler_freezeTask(scheduler, task, delta_m);
  if (delta != 0) {
    IssueGroup group_m = NULL, group_p = NULL;
    // Grow the IssueTable at tau.
    IssueTable_growAt(issueTable, tau, delta);
    // Split the IssueGroup at tau into the IssueGroups at tau and tau + delta.
    group_m = IssueTable_assign(issueTable, tau, InsertionSlot_GROUP_M(slot));
    group_p = IssueTable_assign(issueTable, tau + delta, InsertionSlot_GROUP_P(slot));
    // Freeze the issued tasks that are after the task to be scheduled.
    GrahamList_FOREACH_ISSUED_GrahamListTask(grahamList, TASK) {
      IssueItem ITEM = GrahamListTask_ITEM(TASK);
      if (GrahamListTask_TAU(TASK) < tau) continue;
      if (GrahamListTask_TAU(TASK) == tau) {
        if (GrahamListTask_PHI(TASK) > phi) continue;
        if (GrahamListTask_PHI(TASK) == phi) {
          if (IssueItem_ORDERING(ITEM) < ordering) continue;
        }
      }
      Scheduler_freezeTask(scheduler, TASK, delta);
    } GrahamList_ENDEACH_ISSUED_GrahamListTask;
    if (delta > 0 && tau + delta < lambda) {
      // Try to reduce the amount of LAMBDA increase.
      int increaseLambda = Scheduler_increaseLambda(scheduler, delta);
      IssueTable_shrinkTo(issueTable, lambda + increaseLambda);
    }
    // Get the new LAMBDA from the issueTable.
    increase = IssueTable_lambda(issueTable) - lambda;
    *Scheduler__LAMBDA(scheduler) = lambda + increase;
  }
  SCD_TRACE && fprintf(SCD_TRACE,
      "\t\tphi=%d\ttau=%d\tdate=%d  \tincrease=%d\n",
      phi, tau, tau + delta_m + lambda*phi, increase);
  Scheduler_optimizeFlows(scheduler, true, delta != 0);
  return tau + delta_m;
}

#ifdef $XCC__h
/**
 * Main driver of the Insertion scheduling.
 *
 * The GrahamListTasks to issue are selected in heuristic order by
 * Insertion_selectTask.
 */
void
Insertion_schedule(Insertion this);
#endif//$XCC__h

void
Insertion_schedule(Insertion this)
{
  Scheduler scheduler = Insertion_SCHEDULER(this);
  GrahamList grahamList = Scheduler_GRAHAMLIST(scheduler);
  IssueTable issueTable = Scheduler_ISSUETABLE(scheduler);
  IssueItem firstItem = Scheduler_FIRSTITEM(scheduler);
  GrahamListTask firstTask = GrahamList_FIRSTTASK(grahamList);
  Except_REQUIRE(GrahamListTask_ITEM(firstTask) == firstItem);
  SCD_TRACE && fprintf(SCD_TRACE, "*** INSERTION SCHEDULE:\n");
  Scheduler_initializeFlows(scheduler, true);
  *GrahamListTask__TAU(firstTask) = 0;
  *GrahamListTask__PHI(firstTask) = 0;
  IssueTable_ready(issueTable, Scheduler_LAMBDA(scheduler));
  IssueTable_insert(issueTable, firstItem, GrahamListTask_TAU(firstTask));
  // Insertion scheduling main loop.
  while (!IDList_isEmpty(GrahamList_TOISSUE(grahamList))) {
    GrahamListTask task = Insertion_selectTask(this);
    InsertionSlot slot = Insertion_selectSlot(this, task);
    int tau = Insertion_scheduleTaskAtSlot(this, task, slot);
    // Move task to the issued state.
    IssueTable_insert(issueTable, GrahamListTask_ITEM(task), tau);
    Except_CHECK(task == IDList_firstItem(GrahamList_TOISSUE(grahamList)));
    IDList_drop_(GrahamList_TOISSUE(grahamList));
    IDList_push_(GrahamList_ISSUED(grahamList), task);
  }
  // Schedule the NOISSUE tasks using the MCRL dates.
  GrahamList_scheduleNoIssue(grahamList);
}

#ifdef $XCC_h
/**
 * Schedule with Insertion Scheduling.
 */
int
Insertion_Schedule(Scheduler scheduler, int min_lambda, int max_lambda, int length);
#endif//$XCC_h

int
Insertion_Schedule(Scheduler scheduler, int min_lambda, int max_lambda, int length)
{
  Insertion insertion = Insertion_new(Scheduler_MEMORY(scheduler), scheduler, 32);
  Insertion_schedule(insertion);
  Insertion_delete(insertion);
  return Scheduler_LAMBDA(scheduler);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Insertion module.
 */
void
Insertion_INIT(void);
#endif//$XCC__h

void
Insertion_INIT(void)
{
  Scheduler_SetSchedule(1, Insertion_Schedule);
}

#ifdef $XCC__h
/**
 * Finalize the Insertion module.
 */
void
Insertion_FINI(void);
#endif//$XCC__h

void
Insertion_FINI(void)
{
}

#if XCC__C
static void
Insertion_TEST(void)
{
#include "SCD/Insertion_.c"
}

int
main(int argc, char **argv)
{
  SCD_Test(Insertion_TEST);
  return 0;
}
#endif

