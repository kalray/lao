#ifdef $XCC_h
/*
 * !!!!	GrahamList.xcc
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
 * @brief Graham List scheduling support.
 */
#endif//$XCC_h


#include "SCD/SCD_.h"

#ifdef $XCC__h
#include "SCD/GrahamList.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * GrahamList early date and late date.
 */
struct GrahamListDates_ {
  //@args	
  int16_t EARLYDATE;		// The early schedule date as computed by Relaxation.
  int16_t LATEDATE;		// The late schedule date as computed by Relaxation.
};
#endif//$XCC__h

GrahamListDates
GrahamListDates_Ctor(GrahamListDates this)
{
  *GrahamListDates__EARLYDATE(this) = 0;
  *GrahamListDates__LATEDATE(this) = 0;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(GrahamListDates_)\t%zu\n", sizeof(GrahamListDates_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * GrahamList task.
 */
struct GrahamListTask_ {
  //@args	IssueItem issueItem, int unwinding
  IssueItem ITEM;		// The IssueItem of this GrahamListTask.
  //@access INDEX	IssueItem_INDEX(GrahamListTask_ITEM(this))
  //@access OPERATION	IssueItem_OPERATION(GrahamListTask_ITEM(this))
  //@access ORDERING	IssueItem_ORDERING(GrahamListTask_ITEM(this))
  int16_t PRIORITY1;		// First priority value for this task.
  int16_t PRIORITY2;		// Second priority value for this task.
  int16_t TAU;			// DATE MOD LAMBDA.
  int16_t PHI;			// DATE DIV LAMBDA.
  GrahamListDates_ DATES[1];	// GrahamListDates for each unwinded instance.
  //@access EARLYDATE	GrahamListDates_EARLYDATE(GrahamListTask_DATES(this))
  //@access LATEDATE	GrahamListDates_LATEDATE(GrahamListTask_DATES(this))
};
#endif//$XCC__h

GrahamListTask
GrahamListTask_Ctor(GrahamListTask this, IssueItem issueItem, int unwinding)
{
  Except_REQUIRE(unwinding >= 0);
  *GrahamListTask__ITEM(this) = issueItem;
  *GrahamListTask__PRIORITY1(this) = 0;
  *GrahamListTask__PRIORITY2(this) = 0;
  *GrahamListTask__TAU(this) = 0;
  *GrahamListTask__PHI(this) = 0;
  memset(GrahamListTask_DATES(this), 0, (unwinding + 1)*sizeof(GrahamListDates_));
  return this;
}

size_t
GrahamListTask_Size(IssueItem issueItem, int unwinding)
{
  return sizeof(GrahamListTask_) + unwinding*sizeof(GrahamListDates_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(GrahamListTask_)\t%zu\n", sizeof(GrahamListTask_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Compare function for use by Insertion_selectTask.
 */
int
GrahamListTask_compare(const void *this_pointer, const void *that_pointer);
#endif//$XCC__h

int
GrahamListTask_compare(const void *this_pointer, const void *that_pointer)
{
  const_GrahamListTask this = this_pointer;
  const_GrahamListTask that = that_pointer;
  int result = GrahamListTask_PRIORITY1(this) - GrahamListTask_PRIORITY1(that);
  if (result != 0) return result;
  result = GrahamListTask_PRIORITY2(this) - GrahamListTask_PRIORITY2(that);
  if (result != 0) return result;
  result = IssueItem_ORDERING(GrahamListTask_ITEM(this)) -
      IssueItem_ORDERING(GrahamListTask_ITEM(that));
  return result;
}

#ifdef $XCC_h
/**
 * enumerates the GrahamList kinds.
 */
typedef enum {
  GrahamListKind_Forward,	// Forward GrahamList scheduling.
  GrahamListKind_Backward,	// Backward GrahamList scheduling.
  GrahamListKind__
} enum_GrahamListKind;
typedef uint8_t GrahamListKind;
extern const char *
GrahamListKind_type(GrahamListKind this);
#endif//$XCC_h

const char *
GrahamListKind_type(GrahamListKind this)
{
  static const char *_[] = {
    "Forward",
    "Backward",
  };
  Except_REQUIRE((unsigned)this < GrahamListKind__);
  return _[this];
}

#ifdef $XCC_h
/**
 * Graham List Scheduling support.
 */
struct GrahamList_;
#endif//$XCC_h

#ifdef $XCC__h
struct GrahamList_ {
  //@args	Memory memory, DDGraph ddgraph, const uint8_t *availability
  DDGraph DDGRAPH;		// The dependence graph.
  //@access LAMBDA	DDGraph_LAMBDA(GrahamList_DDGRAPH(this))
  //@mutate LAMBDA	DDGraph__LAMBDA(GrahamList_DDGRAPH(this))
  IssueTable ISSUETABLE;	// This GrahamList IssueTable.
  //@access AVAILABILITY	IssueTable_availability(GrahamList_ISSUETABLE(this))
  IStack_ TASKS_;		// All the GrahamListTask(s).
    //@access TASKS	GrahamList__TASKS_(this)
  //@access INDEX2TASK	((GrahamListTask *)IStack_base(GrahamList_TASKS(this)) - 1)
  //@access FIRSTTASK	(*(GrahamListTask *)IStack_firstItem(GrahamList_TASKS(this)))
  //@access LASTTASK	(*(GrahamListTask *)IStack_lastItem(GrahamList_TASKS(this)))
  //@access MEMORY	IStack_memory(GrahamList_TASKS(this))
  //@access COUNT	(IStack_usedSize(GrahamList_TASKS(this))/sizeof(GrahamListTask))
  IDList_ TOISSUE_;		// The GrahamListTask(s) to issue.
  //@access TOISSUE	GrahamList__TOISSUE_(this)
  IDList_ NOISSUE_;		// The GrahamListTask(s) that dont'need issue.
  //@access NOISSUE	GrahamList__NOISSUE_(this)
  IDList_ ISSUED_;		// The GrahamListTask(s) issued.
  //@access ISSUED	GrahamList__ISSUED_(this)
};
#endif//$XCC__h

GrahamList
GrahamList_Ctor(GrahamList this,
                Memory memory, DDGraph ddgraph, const uint8_t *availability)
{
  int32_t nodeCount = DDGraph_nodeCount(ddgraph);
  *GrahamList__DDGRAPH(this) = ddgraph;
  *GrahamList__ISSUETABLE(this) = IssueTable_new(memory, availability);
  IStack_Ctor(GrahamList_TASKS(this),
              memory, sizeof(GrahamListTask), nodeCount);
  IDList_Ctor(GrahamList_TOISSUE(this), memory);
  IDList_Ctor(GrahamList_NOISSUE(this), memory);
  IDList_Ctor(GrahamList_ISSUED(this), memory);
  return this;
}

void
GrahamList_Dtor(GrahamList this)
{
  IssueTable_delete(GrahamList_ISSUETABLE(this));
  IStack_Dtor(GrahamList_TASKS(this));
  IDList_Dtor(GrahamList_TOISSUE(this));
  IDList_Dtor(GrahamList_NOISSUE(this));
  IDList_Dtor(GrahamList_ISSUED(this));
}

size_t
GrahamList_Size(Memory memory, DDGraph ddgraph, const uint8_t *availability)
{
  return sizeof(GrahamList_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(GrahamList_)\t%zu\n", sizeof(GrahamList_));
}
#endif//$XCC__c

#ifdef $XCC_h
GrahamList
GrahamList_new(Memory parent, DDGraph ddgraph, const uint8_t *availability);
#endif//$XCC_h

GrahamList
GrahamList_new(Memory parent, DDGraph ddgraph, const uint8_t *availability)
{
  Memory memory = Memory_new(parent, true);
  size_t size = GrahamList_Size(memory, ddgraph, availability);
  GrahamList this = Memory_alloc(memory, size);
  return GrahamList_Ctor(this, memory, ddgraph, availability);
}

#ifdef $XCC_h
GrahamList
GrahamList_delete(GrahamList this);
#endif//$XCC_h

GrahamList
GrahamList_delete(GrahamList this)
{
  if (this != NULL) {
    Memory memory = GrahamList_MEMORY(this);
#ifndef _NDTOR
    GrahamList_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC__h
/**
 * For use by GrahamList_FOREACH_GrahamListTask.
 */
static inline IStack
GrahamList_tasks(GrahamList this)
{
  return GrahamList_TASKS(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this GrahamListTask(s) in topological sort.
 */
#define GrahamList_FOREACH_GrahamListTask(this, task) { \
  IStack GrahamList_TASKS = GrahamList_tasks(this); \
  IStack_FOREACH(GrahamList_TASKS, GrahamListTask, GrahamList__TASK) { \
    GrahamListTask task = *GrahamList__TASK;
#define GrahamList_ENDEACH_GrahamListTask \
  } IStack_ENDEACH \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this GrahamListTask(s) in topological sort.
 */
#define GrahamList_FORBACK_GrahamListTask(this, task) { \
  IStack GrahamList_TASKS = GrahamList_tasks(this); \
  IStack_FORBACK(GrahamList_TASKS, GrahamListTask, GrahamList__TASK) { \
    GrahamListTask task = *GrahamList__TASK;
#define GrahamList_ENDBACK_GrahamListTask \
  } IStack_ENDBACK \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Needed by GrahamList_FOREACH_TOISSUE_GrahamListTask.
 */
static inline IDList
GrahamList_toIssue(GrahamList this)
{
  return GrahamList_TOISSUE(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this TOISSUE GrahamListTask(s).
 */
#define GrahamList_FOREACH_TOISSUE_GrahamListTask(this, task) \
  IDList_FOREACH(GrahamList_toIssue(this), GrahamListTask_ , task)
#define GrahamList_ENDEACH_TOISSUE_GrahamListTask \
  IDList_ENDEACH;
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Needed by GrahamList_FOREACH_NOISSUE_GrahamListTask.
 */
static inline IDList
GrahamList_noIssue(GrahamList this)
{
  return GrahamList_NOISSUE(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this NOISSUE GrahamListTask(s).
 */
#define GrahamList_FOREACH_NOISSUE_GrahamListTask(this, task) \
  IDList_FOREACH(GrahamList_noIssue(this), GrahamListTask_ , task)
#define GrahamList_ENDEACH_NOISSUE_GrahamListTask \
  IDList_ENDEACH;
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Needed by GrahamList_FOREACH_ISSUED_GrahamListTask.
 */
static inline IDList
GrahamList_issued(GrahamList this)
{
  return GrahamList_ISSUED(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this ISSUED GrahamListTask(s).
 */
#define GrahamList_FOREACH_ISSUED_GrahamListTask(this, task) \
  IDList_FOREACH(GrahamList_issued(this), GrahamListTask_ , task)
#define GrahamList_ENDEACH_ISSUED_GrahamListTask \
  IDList_ENDEACH;
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Enter an IssueItem in this GrahamList.
 */
void
GrahamList_enterIssueItem(GrahamList this, IssueItem issueItem, int unwinding);
#endif//$XCC__h

void
GrahamList_enterIssueItem(GrahamList this, IssueItem issueItem, int unwinding)
{
  IDList toIssue = GrahamList_TOISSUE(this);
  IDList noIssue = GrahamList_NOISSUE(this);
  Scheduling scheduling = IssueItem_SCHEDULING(issueItem);
  size_t size = GrahamListTask_Size(issueItem, unwinding);
  GrahamListTask task = NULL;
  if (scheduling != Scheduling__UNDEF) {
    task = GrahamListTask_Ctor(IDList_push(toIssue, size), issueItem, unwinding);
  } else {
    task = GrahamListTask_Ctor(IDList_push(noIssue, size), issueItem, unwinding);
    if (Operator_isPseudo(Operation_operator(IssueItem_OPERATION(issueItem))));
    else {
#ifndef _NTRACE
      fprintf(stderr, "*** No Scheduling for %s\n",
              Operator_Id(Operation_operator(IssueItem_OPERATION(issueItem))));
#endif//_NTRACE
      Except_NEVER(true);
    }
  }
  IStack_PUSH(GrahamList_TASKS(this), GrahamListTask, task);
  IssueTable_enter(GrahamList_ISSUETABLE(this), issueItem);
}

#ifdef $XCC__h
/**
 * Schedule the NOISSUE tasks.
 */
void
GrahamList_scheduleNoIssue(GrahamList this);
#endif//$XCC__h

void
GrahamList_scheduleNoIssue(GrahamList this)
{
  DDGraph ddgraph = GrahamList_DDGRAPH(this);
  DDGraphKind kind = DDGraph_KIND(ddgraph);
  int16_t lambda = DDGraph_LAMBDA(ddgraph);
  GrahamList_FOREACH_NOISSUE_GrahamListTask(this, TASK) {
    IssueItem ITEM = GrahamListTask_ITEM(TASK);
    DDGraphNode NODE = IssueItem_DDGNODES(ITEM)[kind];
    int16_t tau = DDGraphNode_DATE(NODE)%lambda;
    int16_t phi = DDGraphNode_DATE(NODE)/lambda;
    while (tau >= lambda) tau -= lambda, phi += 1;
    while (tau < 0) tau += lambda, phi -= 1;
    *GrahamListTask__TAU(TASK) = tau;
    *GrahamListTask__PHI(TASK) = phi;
  } GrahamList_ENDEACH_NOISSUE_GrahamListTask;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the GrahamList module.
 */
void
GrahamList_INIT(void);
#endif//$XCC__h

void
GrahamList_INIT(void)
{
}

#ifdef $XCC__h
/**
 * Finalize the GrahamList module.
 */
void
GrahamList_FINI(void);
#endif//$XCC__h

void
GrahamList_FINI(void)
{
}

#if XCC__C
static void
GrahamList_TEST(void)
{
#include "SCD/GrahamList_.c"
}

int
main(int argc, char **argv)
{
  SCD_Test(GrahamList_TEST);
  return 0;
}
#endif

