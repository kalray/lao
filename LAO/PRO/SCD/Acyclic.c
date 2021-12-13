#ifdef $XCC_h
/*
 * !!!! Acyclic.xcc
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
 * @brief Acyclic instruction scheduler.
 */
#endif//$XCC_h


#include "SCD/SCD_.h"

#ifdef $XCC__h
#include "SCD/Acyclic.h"
#endif//$XCC__h

#ifdef $XCC__h
/**
 * If zero, do not recompute CRITICAL from scratch on II increase.
 */
//#define Acyclic_CRITICAL 0
#ifndef Acyclic_CRITICAL
extern int Acyclic_CRITICAL;
#endif//Acyclic_CRITICAL
#endif//$XCC__h

#ifndef Acyclic_CRITICAL
int Acyclic_CRITICAL = 0;
#endif//Acyclic_CRITICAL

#ifdef $XCC__h
/**
 * If not zero, try to infer the next II
 */
//#define Acyclic_CONVERGE 0
#ifndef Acyclic_CONVERGE
extern int Acyclic_CONVERGE;
#endif//Acyclic_CONVERGE
#endif//$XCC__h

#ifndef Acyclic_CONVERGE
int Acyclic_CONVERGE = 0;
#endif//Acyclic_CONVERGE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
struct AcyclicTask_ {
  //@args	DDGraphNode node, Opcode opcode, AcyclicTask pred, AcyclicTask succ
  InstructionTask_ __;			// Base class for this AcyclicTask.
  //@access SCHEDULING	InstructionTask_SCHEDULING(AcyclicTask____(this))
  //@access RESERVATION	InstructionTask_RESERVATION(AcyclicTask____(this))
  //@access PROCESSING	InstructionTask_PROCESSING(AcyclicTask____(this))
  //@access INSTANCE	InstructionTask_INSTANCE(AcyclicTask____(this))
  //@mutate INSTANCE	InstructionTask__INSTANCE(AcyclicTask____(this))
  //@access STARTDATE	InstructionTask_STARTDATE(AcyclicTask____(this))
  //@mutate STARTDATE	InstructionTask__STARTDATE(AcyclicTask____(this))
  //@access RELEASE	InstructionTask_RELEASE(AcyclicTask____(this))
  //@mutate RELEASE	InstructionTask__RELEASE(AcyclicTask____(this))
  //@access CRITICAL	InstructionTask_CRITICAL(AcyclicTask____(this))
  //@mutate CRITICAL	InstructionTask__CRITICAL(AcyclicTask____(this))
  //@access PREDCOUNT	InstructionTask_PREDCOUNT(AcyclicTask____(this))
  //@mutate PREDCOUNT	InstructionTask__PREDCOUNT(AcyclicTask____(this))
  DDGraphNode NODE;			// The associated DDGraphNode.
  //@access INDEX	DDGraphNode_INDEX(AcyclicTask_NODE(this))
  struct AcyclicTask_ *PRED;		// Predecessor AcyclicTask in regular chain.
  struct AcyclicTask_ *SUCC;		// Successor AcyclicTask in regular chain.
  struct AcyclicTask_ *NEXT;		// Next AcyclicTask in issue group.
};
#endif//$XCC__h

AcyclicTask
AcyclicTask_Ctor(AcyclicTask this,
                 DDGraphNode node, Opcode opcode, AcyclicTask pred, AcyclicTask succ)
{
  InstructionTask_Ctor(AcyclicTask____(this), opcode);
  *AcyclicTask__NODE(this) = node;
  *AcyclicTask__PRED(this) = pred;
  *AcyclicTask__SUCC(this) = succ;
  *AcyclicTask__NEXT(this) = NULL;
  return this;
}

void
AcyclicTask_Dtor(AcyclicTask this)
{
}

size_t
AcyclicTask_Size(DDGraphNode node, Opcode opcode, AcyclicTask pred, AcyclicTask succ)
{
  return sizeof(AcyclicTask_);
}

#ifdef $XCC__h
bool
AcyclicTask_pretty(const_AcyclicTask this, FILE *file, const AcyclicTask_ *tasksBase);
#endif//$XCC__h

#ifndef _NTRACE
bool
AcyclicTask_pretty(const_AcyclicTask this, FILE *file, const AcyclicTask_ *tasksBase)
{
  DDGraphNode node = AcyclicTask_NODE(this);
  AcyclicTask pred = AcyclicTask_PRED(this);
  AcyclicTask succ = AcyclicTask_SUCC(this);
  if (file == NULL) file = stderr;
  fprintf(file, "task=[%zu]#%d\tcritical=%d", tasksBase? this - tasksBase: -1,
                AcyclicTask_INSTANCE(this), AcyclicTask_CRITICAL(this));
  if (AcyclicTask_STARTDATE(this) >= 0) {
    fprintf(file, "\tstartDate=%d", AcyclicTask_STARTDATE(this));
  }
  if (AcyclicTask_PREDCOUNT(this) != 0) {
    fprintf(file, "\tpredcount=%d", AcyclicTask_PREDCOUNT(this));
  }
  if (tasksBase != NULL) {
    if (pred != NULL) {
      fprintf(file, "\tpred=[%zu]", pred - tasksBase);
    }
    if (succ != NULL) {
      fprintf(file, "\tsucc=[%zu]", succ - tasksBase);
    }
  }
  fprintf(file, "\n") && Operation_pretty(DDGraphNode_OPERATION(node), file);
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
struct AcyclicState_ {
  //@args	Memory memory, int bodyCount, int lambda, int barOmega
  PtrSeq_ GROUPS[1];		// Schedule groups as linked lists of AcyclicTask(s).
  //@access MEMORY	PtrSeq_memory(AcyclicState_GROUPS(this))
  //@access TIMESLOT	(PtrSeq_count(AcyclicState_GROUPS(this)) - 1)
  int16_t LAMBDA;		// Current initiation interval.
  int16_t BAROMEGA;		// Maximum omega in the dependence graph.
  int16_t BODYCOUNT;		// Count of Operation(s) in loop body.
  int16_t ERRORCOUNT;		// Count of AcyclicTask(s) inside the window minus bodyCount.
  int16_t BEGINDATE;		// Start date of the time window.
  int16_t ERRORDATE;		// Latest date with convergence error (ERRORCOUNT != 0).
  int16_t PREVSPAN;		// Previous makeSpan.
  int16_t DELTACOUNT;		// Count of makeSpan deltas.
  int32_t DELTATOTAL;		// Total of makeSpan deltas.
};
#endif//$XCC__h

AcyclicState
AcyclicState_Ctor(AcyclicState this,
                  Memory memory, int bodyCount, int lambda, int barOmega)
{
  int32_t horizon = bodyCount*(barOmega + 1)*3;
  PtrSeq_Ctor(AcyclicState_GROUPS(this), memory, horizon);
  *AcyclicState__LAMBDA(this) = lambda;
  *AcyclicState__BAROMEGA(this) = barOmega;
  *AcyclicState__BODYCOUNT(this) = bodyCount;
  *AcyclicState__ERRORCOUNT(this) = -bodyCount;
  *AcyclicState__BEGINDATE(this) = -1 - lambda;
  *AcyclicState__ERRORDATE(this) = INT16_MAX;
  *AcyclicState__PREVSPAN(this) = INT16_MAX;
  *AcyclicState__DELTACOUNT(this) = 0;
  *AcyclicState__DELTATOTAL(this) = 0;
  Except_CHECK(   AcyclicState_BEGINDATE(this) + AcyclicState_LAMBDA(this)
               == AcyclicState_TIMESLOT(this));
  return this;
}

void
AcyclicState_Dtor(AcyclicState this)
{
  Memory memory = AcyclicState_MEMORY(this);
  PtrSeq_Dtor(AcyclicState_GROUPS(this));
}

static inline AcyclicTask
AcyclicState_lastTask(AcyclicState this)
{
  PtrSeq groups = AcyclicState_GROUPS(this);
  if (!PtrSeq_isEmpty(groups)) {
    return PtrSeq_lastItem(groups);
  }
  return NULL;
}

#ifdef $XCC__h
AcyclicTask *
AcyclicState_advance(AcyclicState this, int timeSlot);
#endif//$XCC__h

AcyclicTask *
AcyclicState_advance(AcyclicState this, int timeSlot)
{
  AcyclicTask *baseGroup, *lastGroup;
  int beginDate = AcyclicState_BEGINDATE(this);
  int inserted = 0, removed = 0, elapsed = 0, date;
  PtrSeq groups = AcyclicState_GROUPS(this);
  // Compute elapsed, the increase of TIMESLOT.
  while (PtrSeq_count(groups) <= timeSlot) {
    PtrSeq_push2(groups, NULL);
    elapsed++;
  }
  baseGroup = (AcyclicTask *)PtrSeq_base(groups);
  lastGroup = (AcyclicTask *)PtrSeq_past(groups) - 1;
  // Compute how many AcyclicTask(s) are removed from the window.
  date = beginDate > 0? beginDate: 0;
  for (; date < beginDate + elapsed; date++) {
    AcyclicTask task = baseGroup[date];
    for (; task != NULL; task = AcyclicTask_NEXT(task)) removed++;
  }
  // Compute how many AcyclicTask(s) are inserted into the window.
  date = timeSlot - elapsed > 0? timeSlot - elapsed: 0;
  for (; date < timeSlot; date++) {
    AcyclicTask task = baseGroup[date];
    for (; task != NULL; task = AcyclicTask_NEXT(task)) inserted++;
  }
  *AcyclicState__BEGINDATE(this) = beginDate + elapsed;
  *AcyclicState__ERRORCOUNT(this) += inserted - removed;
  if (AcyclicState_ERRORCOUNT(this) != 0) {
    *AcyclicState__ERRORDATE(this) = timeSlot - 1;
  }
  Except_CHECK(AcyclicState_TIMESLOT(this) == timeSlot);
  Except_CHECK(   AcyclicState_BEGINDATE(this) + AcyclicState_LAMBDA(this)
               == AcyclicState_TIMESLOT(this));
  Except_CHECK(elapsed == 0 || *lastGroup == NULL);
  return lastGroup;
}

#ifdef $XCC__h
static inline bool
AcyclicState_converged(AcyclicState this, const_AcyclicTask firstTask)
{
  int lambda = AcyclicState_LAMBDA(this);
  int barOmega = AcyclicState_BAROMEGA(this);
  int bodyCount = AcyclicState_BODYCOUNT(this);
  int errorDate = AcyclicState_ERRORDATE(this);
  int instance = AcyclicTask_INSTANCE(firstTask);
  int firstDate = AcyclicTask_STARTDATE(firstTask);
  const_AcyclicTask lastTask = firstTask + bodyCount - 1;
  int processing = AcyclicTask_PROCESSING(lastTask);
  int lastDate = AcyclicTask_STARTDATE(lastTask);
  int makeSpan = lastDate + processing - firstDate;
  int stableDate = errorDate + 2 - lambda;
  int stabilized = lastDate - stableDate;
  // Strict convergence checks.
  if (stabilized >= makeSpan) {
    int successive = stabilized/lambda;
    if (successive >= barOmega) {
      int hatOmega = (makeSpan - 1)/lambda;
      if (successive >= hatOmega) {
        SCD_TRACE && fprintf(SCD_TRACE, "stableDate=%d\tmakeSpan=%d\tstabilized=%d\tbarOmega=%d\n",
                                        stableDate, makeSpan, stabilized, barOmega);
        return true;
      }
    }
  }
  return false;
}
#endif//$XCC__h

#ifdef $XCC__h
bool
AcyclicState_continue(AcyclicState this, const_AcyclicTask firstTask);
#endif//$XCC__h

bool
AcyclicState_continue(AcyclicState this, const_AcyclicTask firstTask)
{
  int lambda = AcyclicState_LAMBDA(this);
  int bodyCount = AcyclicState_BODYCOUNT(this);
  int instance = AcyclicTask_INSTANCE(firstTask);
  int firstDate = AcyclicTask_STARTDATE(firstTask);
  const_AcyclicTask lastTask = firstTask + bodyCount - 1;
  int processing = AcyclicTask_PROCESSING(lastTask);
  int lastDate = AcyclicTask_STARTDATE(lastTask);
  int makeSpan = lastDate + processing - firstDate;
  if (AcyclicState_converged(this, firstTask)) {
    int maxInstance = (makeSpan + lambda - 1)/lambda + instance;
    SCD_TRACE && fprintf(SCD_TRACE, "Bingo#%d!\tLAMBDA=%d\tmakeSpan=%d\tmaxInstance=%d\n",
                                    instance, lambda, makeSpan, maxInstance);
    return false;
  } else {
    int prevSpan = AcyclicState_PREVSPAN(this);
    int deltaCount = AcyclicState_DELTACOUNT(this);
    int32_t deltaTotal = AcyclicState_DELTATOTAL(this);
    int diffSpan = makeSpan - prevSpan;
    if (diffSpan >= 0) {
      deltaTotal += diffSpan;
      deltaCount++;
    }
    SCD_TRACE && fprintf(SCD_TRACE, "Failed#%d!\tLAMBDA=%d\tmakeSpan=%d\t",
                                    instance, lambda, makeSpan);
    SCD_TRACE && deltaCount && fprintf(SCD_TRACE, "\tdiffSpan=%d\tdelta=%d",
                                                  diffSpan, deltaTotal/deltaCount);
    SCD_TRACE && fprintf(SCD_TRACE, "\n");
    *AcyclicState__PREVSPAN(this) = makeSpan;
    *AcyclicState__DELTACOUNT(this) = deltaCount; 
    *AcyclicState__DELTATOTAL(this) = deltaTotal; 
    if (Acyclic_CONVERGE & 0x1) {
      int mayUnwind = (2*makeSpan + lambda - 1)/lambda;
      if (instance > mayUnwind) return false;
    }
  }
  return true;
}

#ifdef $XCC__h
bool
AcyclicState_pretty(const_AcyclicState this, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
bool
AcyclicState_pretty(const_AcyclicState this, FILE *file)
{
  int startDate = 0;
  if (file == NULL) file = stderr;
  fprintf(file, "*** AcyclicState:\n");
  PtrSeq_FOREACH(AcyclicState_GROUPS(this), AcyclicTask, task) {
    fprintf(file, "[%d]", startDate++);
    while (task != NULL) {
      fprintf(file, "\t%s_%d#%d", Scheduling_name(AcyclicTask_SCHEDULING(task)),
                                  AcyclicTask_INDEX(task), AcyclicTask_INSTANCE(task));
      task = AcyclicTask_NEXT(task);
    }
    fprintf(SCD_TRACE, "\n");
  } PtrSeq_ENDEACH;
  return true;
}
#endif//_NTRACE

#ifdef $XCC__h
bool
AcyclicState_latex(const_AcyclicState this, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
bool
AcyclicState_latex(const_AcyclicState this, FILE *file)
{
  int startDate = 0;
  AcyclicTask tasks[Bundle__MAXINSTRUCTIONS+2];
  if (file == NULL) file = stderr;
  fprintf(file, "*** AcyclicState:\n");
  PtrSeq_FOREACH(AcyclicState_GROUPS(this), AcyclicTask, task) {
    int count = 0, i;
    fprintf(file, "%d &\t", startDate++);
    while (task != NULL) {
      tasks[count++] = task;
      task = AcyclicTask_NEXT(task);
    }
    for (i = count - 1; i >= 0; i--) {
      AcyclicTask task = tasks[i];
      char code = 'A' + AcyclicTask_INDEX(task) - 1;
      fprintf(file, "%c^%d ", code, AcyclicTask_INSTANCE(task) + 1);
    }
    fprintf(SCD_TRACE, "\\\\\n");
  } PtrSeq_ENDEACH;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
struct Acyclic_ {
  //@args Memory memory, Scheduler scheduler
  Scheduler SCHEDULER;
  ResourceTable_ *TABLE;
  //@access isInnerLoop	Scheduler_isInnerLoop(Acyclic_SCHEDULER(this))
  //@access PROCESSOR	Scheduler_PROCESSOR(Acyclic_SCHEDULER(this))
  //@access PIPELINING	Scheduler_PIPELINING(Acyclic_SCHEDULER(this))
  //@access MCRLGRAPH	Scheduler_MCRLGRAPH(Acyclic_SCHEDULER(this))
  //@access FIRSTINDEX	Scheduler_FIRSTINDEX(Acyclic_SCHEDULER(this))
  //@access LASTINDEX	Scheduler_LASTINDEX(Acyclic_SCHEDULER(this))
  //@access BAROMEGA	Scheduler_BAROMEGA(Acyclic_SCHEDULER(this))
  //@access UNWINDING	Scheduler_UNWINDING(Acyclic_SCHEDULER(this))
  PQueue_ WAITING[1];		// All predecessors are scheduled.
  PQueue_ READY[1];		// Data-ready tasks for scheduling.
  IStack_ TASKS[1];		// The set of unwinded tasks.
  //@access MEMORY	IStack_memory(Acyclic_TASKS(this))
  PtrSeq_ NODESTACK[1];		// DDGraphNode stack for priority computation.
  bool *INSTACK;		// Flag the DDGraphNode(s) that are on NODESTACK;
  int32_t BODYCOUNT;
  int32_t SCHEDULED;
};
#endif//$XCC__h

Acyclic
Acyclic_Ctor(Acyclic this,
             Memory memory, Scheduler scheduler)
{
  Processor processor = Scheduler_PROCESSOR(scheduler);
  int32_t firstIndex = Scheduler_FIRSTINDEX(scheduler);
  int32_t lastIndex = Scheduler_LASTINDEX(scheduler);
  int unwinding = Scheduler_UNWINDING(scheduler);
  int32_t bodyCount = lastIndex - firstIndex + 1;
  int32_t taskCount = bodyCount*(unwinding + 1);
  size_t tableSize = ResourceTable_Size(processor, 0);
  ResourceTable_ *table = Memory_alloc(memory, tableSize);
  *Acyclic__SCHEDULER(this) = scheduler;
  *Acyclic__TABLE(this) = ResourceTable_Ctor(table, processor, 0);
  PQueue_Ctor(Acyclic_WAITING(this), memory, taskCount);
  PQueue_Ctor(Acyclic_READY(this), memory, taskCount);
  IStack_Ctor(Acyclic_TASKS(this), memory, sizeof(AcyclicTask_), taskCount);
  PtrSeq_Ctor(Acyclic_NODESTACK(this), memory, (lastIndex + 1));
  *Acyclic__INSTACK(this) = Memory_alloc(memory, sizeof(bool)*(lastIndex + 1));
  memset(Acyclic_INSTACK(this), 0, sizeof(bool)*(lastIndex + 1));
  *Acyclic__BODYCOUNT(this) = bodyCount;
  *Acyclic__SCHEDULED(this) = 0;
  return this;
}

void
Acyclic_Dtor(Acyclic this)
{
  Memory memory = Acyclic_MEMORY(this);
  Memory_free_(memory, Acyclic_TABLE(this));
  PQueue_Dtor(Acyclic_WAITING(this));
  PQueue_Dtor(Acyclic_READY(this));
  IStack_Dtor(Acyclic_TASKS(this));
  PtrSeq_Dtor(Acyclic_NODESTACK(this));
  Memory_free_(memory, Acyclic_INSTACK(this));
}

size_t
Acyclic_Size(Memory memory, Scheduler scheduler)
{
  return sizeof(Acyclic_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Acyclic_)\t%zu\n", sizeof(Acyclic_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * For use by Acyclic_FOREACH_AcyclicTask.
 */
static inline const_IStack
Acyclic_tasks(const_Acyclic this)
{
  return Acyclic_TASKS(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Acyclic AcyclicTask(s).
 */
#define Acyclic_FOREACH_AcyclicTask(this, task) { \
  const_IStack Acyclic_TASKS = Acyclic_tasks(this); \
  IStack_FOREACH(Acyclic_TASKS, AcyclicTask_, task) {
#define Acyclic_ENDEACH_AcyclicTask \
  } IStack_ENDEACH; \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate backward over this Acyclic AcyclicTask(s).
 */
#define Acyclic_FORBACK_AcyclicTask(this, task) { \
  const_IStack Acyclic_TASKS = Acyclic_tasks(this); \
  IStack_FORBACK(Acyclic_TASKS, AcyclicTask_, task) {
#define Acyclic_ENDBACK_AcyclicTask \
  } IStack_ENDBACK; \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Enter the AcyclicTask(s) in this Acyclic.
 */
void
Acyclic_enterTasks(Acyclic this);
#endif//$XCC__h

void
Acyclic_enterTasks(Acyclic this)
{
  IStack tasks = Acyclic_TASKS(this);
  int barOmega = Acyclic_BAROMEGA(this);
  int unwinding = Acyclic_UNWINDING(this), instance;
  DDGraph mcrlGraph = Acyclic_MCRLGRAPH(this);
  DDGraphNode *nodes = DDGraph_nodes(mcrlGraph);
  int32_t firstIndex = Acyclic_FIRSTINDEX(this);
  int32_t lastIndex = Acyclic_LASTINDEX(this), index;
  int32_t bodyCount = lastIndex - firstIndex + 1;
  Processor processor = Acyclic_PROCESSOR(this);
  AcyclicTask_ *tasksBase = IStack_base(tasks);
  Except_CHECK(unwinding >= barOmega);
  SCD_TRACE && fprintf(SCD_TRACE, "*** Acyclic_enterTasks\n");
  for (instance = 0; instance <= unwinding; instance++) {
    for (index = firstIndex; index <= lastIndex; index++) {
      DDGraphNode node = nodes[index];
      Operation operation = DDGraphNode_OPERATION(node);
      Operator operator = Operation_operator(operation);
      Opcode opcode = Operator_opcode(operator, processor);
      int32_t pred_offset = index - firstIndex + bodyCount*(instance - 1);
      int32_t succ_offset = index - firstIndex + bodyCount*(instance + 1);
      AcyclicTask pred = instance > 0? tasksBase + pred_offset: NULL;
      AcyclicTask succ = instance < unwinding? tasksBase + succ_offset: NULL;
      AcyclicTask task = AcyclicTask_Ctor(IStack_push(tasks), node, opcode, pred, succ);
      *AcyclicTask__INSTANCE(task) = instance;
    }
  }
  Acyclic_FOREACH_AcyclicTask(this, task) {
    AcyclicTask_initPredCount(task, firstIndex, lastIndex, tasks);
  } Acyclic_ENDEACH_AcyclicTask;
}

#ifdef $XCC__h
void
AcyclicTask_initPredCount(AcyclicTask this, int32_t firstIndex, int32_t lastIndex, IStack tasks);
#endif//$XCC__h

void
AcyclicTask_initPredCount(AcyclicTask this, int32_t firstIndex, int32_t lastIndex, IStack tasks)
{
  int predCount = 0;
  AcyclicTask pred = AcyclicTask_PRED(this);
  DDGraphNode node = AcyclicTask_NODE(this);
  AcyclicTask_ *tasksBase = IStack_base(tasks);
  int32_t taskCount = IStack_count(tasks);
  int32_t bodyCount = lastIndex - firstIndex + 1;
  int32_t index = DDGraphNode_INDEX(node);
  int32_t offset = this - tasksBase;
  0&&SCD_TRACE && fprintf(SCD_TRACE, "*** AcyclicTask_initPredCount\n");
  DDGraphNode_FORENTER_DDGraphArc(node, arc) {
    DDGraphNode tail = DDGraphArc_TAIL(arc);
    int32_t tail_index = DDGraphNode_INDEX(tail);
    if (tail_index >= firstIndex && tail_index <= lastIndex) {
      int omega = DDGraphArc_OMEGA(arc);
      int32_t tail_offset = offset + tail_index - index - omega*bodyCount;
      0&&SCD_TRACE && fprintf(SCD_TRACE, "    (_%d, _%d)\tomega=%d", tail_index, index, omega);
      //Except_CHECK(tail_offset <= offset);
      if (tail_offset >= 0 && tail_offset < taskCount) {
        predCount++;
        0&&SCD_TRACE && fprintf(SCD_TRACE, "\tpredCount=%d", predCount);
      }
      0&&SCD_TRACE && fprintf(SCD_TRACE, "\n");
    }
  } DDGraphNode_ENDENTER_DDGraphArc;
  if (pred != NULL) predCount++;
  *AcyclicTask__PREDCOUNT(this) = predCount;
}

#ifdef $XCC__h
void
Acyclic_reset(Acyclic this);
#endif//$XCC__h

void
Acyclic_reset(Acyclic this)
{
  int32_t firstIndex = Acyclic_FIRSTINDEX(this);
  int32_t lastIndex = Acyclic_LASTINDEX(this);
  ResourceTable table = Acyclic_TABLE(this);
  IStack tasks = Acyclic_TASKS(this);
  ResourceTable_reset(table, 0);
  Acyclic_FOREACH_AcyclicTask(this, task) {
    AcyclicTask_initPredCount(task, firstIndex, lastIndex, tasks);
    *AcyclicTask__STARTDATE(task) = -1;
    *AcyclicTask__RELEASE(task) = 0;
    *AcyclicTask__NEXT(task) = NULL;
  } Acyclic_ENDEACH_AcyclicTask;
}

#ifdef $XCC__h
/**
 * Compute AcyclicTask critical path length
 *
 * Take advantage from the fact that AcyclicTask(s) are entered in topological order.
 */
int
Acyclic_makeCritical1(Acyclic this, int lambda);
#endif//$XCC__h

// First pass: process loop-independent arcs and enqueue loop-carried arcs.
int
Acyclic_makeCritical1(Acyclic this, int lambda)
{
  IStack tasks = Acyclic_TASKS(this);
  int barOmega = Acyclic_BAROMEGA(this);
  AcyclicTask_ *tasksBase = IStack_base(tasks);
  int32_t firstIndex = Acyclic_FIRSTINDEX(this);
  int32_t lastIndex = Acyclic_LASTINDEX(this);
  int32_t bodyCount = lastIndex - firstIndex + 1;
  AcyclicTask lastTask = tasksBase + bodyCount - 1;
  AcyclicTask firstTask = tasksBase + 0, task;
  PtrSeq nodeStack = Acyclic_NODESTACK(this);
  bool *inStack = Acyclic_INSTACK(this);
  SCD_TRACE && fprintf(SCD_TRACE, "*** Acyclic_makeCritical1\tlambda=%d\n", lambda);
  for (task = lastTask; task >= firstTask; task--) {
    int32_t offset = task - tasksBase;
    int critical = AcyclicTask_PROCESSING(task);
    DDGraphNode node = AcyclicTask_NODE(task);
    int32_t index = DDGraphNode_INDEX(node);
    DDGraphNode_FORLEAVE_DDGraphArc(node, arc) {
      DDGraphNode head = DDGraphArc_HEAD(arc);
      int32_t head_index = DDGraphNode_INDEX(head);
      if (head_index >= firstIndex && head_index <= lastIndex) {
        int32_t head_offset = offset + head_index - index;
        AcyclicTask head_task = tasksBase + head_offset;
        int head_critical = AcyclicTask_CRITICAL(head_task);
        int theta = DDGraphArc_THETA(arc);
        int omega = DDGraphArc_OMEGA(arc);
        if (omega == 0 && head_offset >= offset) {
          Except_CHECK(head_offset < bodyCount);
          Except_CHECK(AcyclicTask_NODE(head_task) == head);
          if (critical < head_critical + theta) {
            critical = head_critical + theta;
          }
          *AcyclicTask__CRITICAL(task) = critical;
        } else
        if (omega <= barOmega) {
          if (!inStack[head_index]) {
            PtrSeq_push(nodeStack, head);
            inStack[head_index] = true;
          }
        }
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  }
  Except_CHECK(AcyclicTask_CRITICAL(lastTask) == AcyclicTask_PROCESSING(lastTask));
  return AcyclicTask_CRITICAL(firstTask) - AcyclicTask_CRITICAL(lastTask)
                                         + AcyclicTask_PROCESSING(lastTask);
}

#ifdef $XCC__h
void
Acyclic_makeCritical2(Acyclic this, int lambda);
#endif//$XCC__h

// Second pass: Bellman-Ford initialized with loop-carried arcs.
void
Acyclic_makeCritical2(Acyclic this, int lambda)
{
  IStack tasks = Acyclic_TASKS(this);
  int unwinding = Acyclic_UNWINDING(this), i = 0;
  AcyclicTask_ *tasksBase = IStack_base(tasks);
  int32_t taskCount = IStack_count(tasks);
  int32_t firstIndex = Acyclic_FIRSTINDEX(this);
  int32_t lastIndex = Acyclic_LASTINDEX(this);
  int32_t bodyCount = lastIndex - firstIndex + 1;
  PtrSeq nodeStack = Acyclic_NODESTACK(this);
  bool *inStack = Acyclic_INSTACK(this);
  Except_CHECK(unwinding > 0);
  SCD_TRACE && fprintf(SCD_TRACE, "*** Acyclic_makeCritical2\tLAMBDA=%d\tnodeStack=%d/%d\n",
                                  lambda, PtrSeq_count(nodeStack), bodyCount);
  while (!PtrSeq_isEmpty(nodeStack)) {
    DDGraphNode node = PtrSeq_top(nodeStack);
    int32_t index = DDGraphNode_INDEX(node);
    int32_t offset = index - firstIndex;
    AcyclicTask task = tasksBase + offset;
    int critical = AcyclicTask_CRITICAL(task);
    inStack[index] = false;
    PtrSeq_pop(nodeStack);
    DDGraphNode_FORENTER_DDGraphArc(node, arc) {
      DDGraphNode tail = DDGraphArc_TAIL(arc);
      int32_t tail_index = DDGraphNode_INDEX(tail);
      if (tail_index >= firstIndex && tail_index <= lastIndex) {
        int32_t tail_offset = offset + tail_index - index;
        AcyclicTask tail_task = tasksBase + tail_offset;
        int tail_critical = AcyclicTask_CRITICAL(tail_task);
        int theta = DDGraphArc_THETA(arc);
        int omega = DDGraphArc_OMEGA(arc);
        int latency = theta - lambda*omega;
        Except_CHECK(tail_offset >= 0 && tail_offset < bodyCount);
        Except_CHECK(AcyclicTask_NODE(tail_task) == tail);
        if (tail_critical < critical + latency) {
          tail_critical = critical + latency;
          if (!inStack[tail_index]) {
            PtrSeq_push(nodeStack, tail);
            inStack[tail_index] = true;
          }
          *AcyclicTask__CRITICAL(tail_task) = tail_critical;
        }
      }
    } DDGraphNode_ENDENTER_DDGraphArc;
    i++;
  }
  SCD_TRACE && fprintf(SCD_TRACE, "*** Acyclic_BellmanFord\titerated=%d/%d\n",
                                  i, bodyCount);
}

#ifdef $XCC__h
void
Acyclic_makeCritical3(Acyclic this, int lambda);
#endif//$XCC__h

// Third pass: propagate the CRITICAL values to higher instances.
void
Acyclic_makeCritical3(Acyclic this, int lambda)
{
  IStack tasks = Acyclic_TASKS(this);
  int unwinding = Acyclic_UNWINDING(this), i, j;
  AcyclicTask_ *tasksBase = IStack_base(tasks);
  int32_t firstIndex = Acyclic_FIRSTINDEX(this);
  int32_t lastIndex = Acyclic_LASTINDEX(this);
  int32_t bodyCount = lastIndex - firstIndex + 1;
  Except_CHECK(unwinding > 0);
  for (i = 0; i < bodyCount; i++) {
    for (j = 0; j < unwinding; j++) {
      AcyclicTask task_a = tasksBase + i + bodyCount*j;
      AcyclicTask task_b = tasksBase + i + bodyCount*(j + 1);
      *AcyclicTask__CRITICAL(task_b) = AcyclicTask_CRITICAL(task_a) - lambda;
    }
  }
}

#ifdef $XCC__h
int32_t
Acyclic_refillNodeStack(Acyclic this, int lambda);
#endif//$XCC__h

int32_t
Acyclic_refillNodeStack(Acyclic this, int lambda)
{
  IStack tasks = Acyclic_TASKS(this);
  int barOmega = Acyclic_BAROMEGA(this);
  int unwinding = Acyclic_UNWINDING(this), i = 0;
  AcyclicTask_ *tasksBase = IStack_base(tasks);
  int32_t taskCount = IStack_count(tasks);
  int32_t firstIndex = Acyclic_FIRSTINDEX(this);
  int32_t lastIndex = Acyclic_LASTINDEX(this);
  int32_t bodyCount = lastIndex - firstIndex + 1;
  AcyclicTask lastTask = tasksBase + bodyCount - 1;
  AcyclicTask firstTask = tasksBase + 0, task;
  PtrSeq nodeStack = Acyclic_NODESTACK(this);
  bool *inStack = Acyclic_INSTACK(this);
  Except_CHECK(unwinding > 0);
  for (task = firstTask; task <= lastTask; task++) {
    int32_t offset = task - tasksBase;
    int critical = AcyclicTask_CRITICAL(task);
    DDGraphNode node = AcyclicTask_NODE(task);
    int32_t index = DDGraphNode_INDEX(node);
    DDGraphNode_FORLEAVE_DDGraphArc(node, arc) {
      DDGraphNode head = DDGraphArc_HEAD(arc);
      int32_t head_index = DDGraphNode_INDEX(head);
      if (head_index >= firstIndex && head_index <= lastIndex) {
        int32_t head_offset = offset + head_index - index;
        AcyclicTask head_task = tasksBase + head_offset;
        int head_critical = AcyclicTask_CRITICAL(head_task);
        int theta = DDGraphArc_THETA(arc);
        int omega = DDGraphArc_OMEGA(arc);
        int latency = theta - lambda*omega;
        Except_CHECK(head_offset >= 0 && head_offset < bodyCount);
        Except_CHECK(AcyclicTask_NODE(head_task) == head);
        if (omega <= barOmega) {
          if (head_critical > critical - latency) {
            if (!inStack[head_index]) {
              PtrSeq_push(nodeStack, head);
              inStack[head_index] = true;
            }
          }
        }
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  }
  SCD_TRACE && fprintf(SCD_TRACE, "*** Acyclic_refillNodeStack\tLAMBDA=%d\tnodeStack=%d/%d\n",
                                  lambda, PtrSeq_count(nodeStack), bodyCount);
  return PtrSeq_count(nodeStack);
}

#ifdef $XCC__h
bool
Acyclic_checkCritical(Acyclic this, int lambda);
#endif//$XCC__h

bool
Acyclic_checkCritical(Acyclic this, int lambda)
{
  IStack tasks = Acyclic_TASKS(this);
  int barOmega = Acyclic_BAROMEGA(this);
  int unwinding = Acyclic_UNWINDING(this), i, j;
  AcyclicTask_ *tasksBase = IStack_base(tasks);
  int32_t taskCount = IStack_count(tasks);
  int32_t firstIndex = Acyclic_FIRSTINDEX(this);
  int32_t lastIndex = Acyclic_LASTINDEX(this);
  int32_t bodyCount = lastIndex - firstIndex + 1;
  AcyclicTask lastTask = tasksBase + bodyCount - 1;
  AcyclicTask firstTask = tasksBase + 0, task;
  // First: check CRITICAL values consistency in instance 0.
  for (task = lastTask; task >= firstTask; task--) {
    int32_t offset = task - tasksBase;
    int critical = AcyclicTask_CRITICAL(task);
    DDGraphNode node = AcyclicTask_NODE(task);
    int32_t index = DDGraphNode_INDEX(node);
    DDGraphNode_FORLEAVE_DDGraphArc(node, arc) {
      DDGraphNode head = DDGraphArc_HEAD(arc);
      int32_t head_index = DDGraphNode_INDEX(head);
      if (head_index >= firstIndex && head_index <= lastIndex) {
        int theta = DDGraphArc_THETA(arc);
        int omega = DDGraphArc_OMEGA(arc);
        int latency = theta - lambda*omega;
        int32_t head_offset = offset + head_index - index;
        AcyclicTask head_task = tasksBase + head_offset;
        int head_critical = AcyclicTask_CRITICAL(head_task);
        if (omega > barOmega) continue;
        Except_CHECK(AcyclicTask_NODE(head_task) == head);
        Except_CHECK(critical >= head_critical + latency);
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  }
  // Second: check CRITICAL values consistency across instances.
  for (i = 0; i < bodyCount; i++) {
    for (j = 0; j <= unwinding; j++) {
      int32_t offset = i + bodyCount*j;
      AcyclicTask task = tasksBase + offset;
      int critical = AcyclicTask_CRITICAL(task);
      AcyclicTask pred = AcyclicTask_PRED(task);
      AcyclicTask succ = AcyclicTask_SUCC(task);
      DDGraphNode node = AcyclicTask_NODE(task);
      int32_t index = DDGraphNode_INDEX(node);
      DDGraphNode_FORLEAVE_DDGraphArc(node, arc) {
        DDGraphNode head = DDGraphArc_HEAD(arc);
        int32_t head_index = DDGraphNode_INDEX(head);
        if (head_index >= firstIndex && head_index <= lastIndex) {
          int theta = DDGraphArc_THETA(arc);
          int omega = DDGraphArc_OMEGA(arc);
          int32_t head_offset = offset + head_index - index + omega*bodyCount;
          //Except_CHECK(head_offset >= offset);
          if (head_offset >= 0 && head_offset < taskCount) {
            AcyclicTask head_task = tasksBase + head_offset;
            int head_critical = AcyclicTask_CRITICAL(head_task);
            Except_CHECK(AcyclicTask_NODE(head_task) == head);
            if (omega > barOmega) continue;
            Except_CHECK(critical >= head_critical + theta);
          }
        }
      } DDGraphNode_ENDLEAVE_DDGraphArc;
      if (pred != NULL) {
        int pred_critical = AcyclicTask_CRITICAL(pred);
        Except_CHECK(critical == pred_critical - lambda);
      }
      if (succ != NULL) {
        int succ_critical = AcyclicTask_CRITICAL(succ);
        Except_CHECK(critical == succ_critical + lambda);
      }
    }
  }
  return true;
}

#ifdef $XCC__h
/**
 * Schedule this Acyclic tasks based on their CRITICAL(ity).
 *
 * When all predecessors of an AcyclicTask are scheduled, it goes into a WAITING
 * queue in order to become data-ready. Once data-ready, it is transferred to a
 * READY queue so the highest CRITICAL(ity) are tried for scheduling first.
 */
AcyclicTask
Acyclic_trySchedule(Acyclic this, AcyclicState state);
#endif//$XCC__h

AcyclicTask
Acyclic_trySchedule(Acyclic this, AcyclicState state)
{
  int timeSlot = -1;
  IStack tasks = Acyclic_TASKS(this);
  int unwinding = Acyclic_UNWINDING(this);
  int32_t firstIndex = Acyclic_FIRSTINDEX(this);
  int32_t lastIndex = Acyclic_LASTINDEX(this);
  int32_t bodyCount = lastIndex - firstIndex + 1;
  PQueue waiting = Acyclic_WAITING(this);
  PQueue ready = Acyclic_READY(this);
  ResourceTable table = Acyclic_TABLE(this);
  int lambda = AcyclicState_LAMBDA(state);
  AcyclicTask_ *tasksBase = IStack_base(tasks);
  AcyclicTask *lastGroup = NULL, firstTask = NULL;
  ResourceVector_ issue_vector[1] = { { { 1, 0, 0, 0, 0, 0, 0, 0 } } };
  SCD_TRACE && fprintf(SCD_TRACE, "*** Acyclic_trySchedule %d tasks\n", IStack_count(tasks));
  // Enter AcyclicTask(s) with zero PREDCOUNT into the WAITING queue.
  Acyclic_FOREACH_AcyclicTask(this, task) {
    if (AcyclicTask_PREDCOUNT(task) == 0) {
      int release = AcyclicTask_RELEASE(task);
      PQueue_addEntry(waiting, task, release);
      Except_CHECK(release >= 0);
    }
    if (AcyclicTask_INSTANCE(task) > 0) break;
  } Acyclic_ENDEACH_AcyclicTask;
  PQueue_makeHeap(waiting);
  // Main scheduling loop.
  while (1) {
    // Fill the READY queue with the AcyclicTask(s) that are data-ready at timeSlot.
    while (!PQueue_isEmpty(waiting)) {
      AcyclicTask task = PQueue_accessMin(waiting);
      int release = AcyclicTask_RELEASE(task);
      int critical = AcyclicTask_CRITICAL(task);
      int32_t index = AcyclicTask_INDEX(task);
      Except_CHECK(release >= timeSlot);
      if (PQueue_isEmpty(ready) && release > timeSlot) {
        // Move timeSlot to release.
        lastGroup = AcyclicState_advance(state, release);
        ResourceTable_advance(table, release);
        SCD_TRACE && fprintf(SCD_TRACE, "STARTDATE=%d\tLAMBDA=%d\tERRORDATE=%d\tERRORCOUNT=%d\t",
                                        release, lambda,
                                        AcyclicState_ERRORDATE(state),
                                        AcyclicState_ERRORCOUNT(state));
        if (firstTask != NULL) {
          if (!AcyclicState_continue(state, firstTask)) return firstTask;
          firstTask = NULL;
        }
        SCD_TRACE && fprintf(SCD_TRACE, "\n");
        timeSlot = release;
      }
      if (release == timeSlot) {
        // AcyclicTask(s) relased at timeSlot are put into the READY queue.
        Reservation reservation = AcyclicTask_RESERVATION(task);
        //int criticality = Reservation_criticality(reservation);
        PQueuePriority priority = ((INT16_MAX - critical)<<16)
                                //| (-criticality&0xF)<<12
                                | (index&0x0FFF);
        Except_CHECK(INT16_MAX - critical >= 0);
        PQueue_insert(ready, task, priority);
        PQueue_extractMin(waiting);
      } else {
        // Next time slot, finished filling the READY queue.
        Except_CHECK(release > timeSlot);
        break;
      }
    }
    // If the READY queue is empty, we are done.
    if (PQueue_isEmpty(ready)) return NULL;
    // Try to schedule AcyclicTask(s) in the READY queue.
    do {
      AcyclicTask task = PQueue_extractMin(ready);
      Reservation reservation = AcyclicTask_RESERVATION(task);
      if (ResourceTable_fitReservation(table, reservation, timeSlot)) {
        int instance = AcyclicTask_INSTANCE(task);
        // Schedule task at timeSlot.
        *AcyclicTask__STARTDATE(task) = timeSlot;
        *AcyclicTask__NEXT(task) = *lastGroup;
        *lastGroup = task;
        // Prepare for convergence check on lastIndex tasks.
        if (task - tasksBase == bodyCount*(instance + 1) - 1) {
          // Here task is a lastTask, get the corresponding firstTask.
          firstTask = tasksBase + bodyCount*instance;
        }
        // Notify the task successors for the WAITING queue.
        Acyclic_notifySuccessors(this, task, lambda);
        SCD_TRACE && fprintf(SCD_TRACE, "scheduled\t");
        ++*Acyclic__SCHEDULED(this);
      } else {
        // Put back the task in WAITING queue at timeSlot + 1.
        *AcyclicTask__RELEASE(task) = timeSlot + 1;
        PQueue_insert(waiting, task, timeSlot + 1);
        SCD_TRACE && fprintf(SCD_TRACE, "delayed \t");
        if (0 && ResourceTable_isFull(table, issue_vector, timeSlot)) {
          PQueue_FOREACH(ready, AcyclicTask, task) {
            *AcyclicTask__RELEASE(task) = timeSlot + 1;
            PQueue_addEntry(waiting, task, timeSlot + 1);
          } PQueue_ENDEACH;
          if (!PQueue_isEmpty(ready)) {
            PQueue_makeHeap(waiting);
            PQueue_empty(ready);
          }
        }
      }
      SCD_TRACE && AcyclicTask_pretty(task, SCD_TRACE, tasksBase) && fprintf(SCD_TRACE, "\n");
    } while (!PQueue_isEmpty(ready));
  }
}

#ifdef $XCC__h
void
Acyclic_notifySuccessors(Acyclic this, AcyclicTask task, int lambda);
#endif//$XCC__h

void
Acyclic_notifySuccessors(Acyclic this, AcyclicTask task, int lambda)
{
  PQueue waiting = Acyclic_WAITING(this);
  IStack tasks = Acyclic_TASKS(this);
  AcyclicTask_ *tasksBase = IStack_base(tasks);
  int32_t taskCount = IStack_count(tasks);
  int32_t firstIndex = Acyclic_FIRSTINDEX(this);
  int32_t lastIndex = Acyclic_LASTINDEX(this);
  int32_t bodyCount = lastIndex - firstIndex + 1;
  int32_t offset = task - tasksBase;
  int startDate = AcyclicTask_STARTDATE(task);
  AcyclicTask succ = AcyclicTask_SUCC(task);
  DDGraphNode node = AcyclicTask_NODE(task);
  int32_t index = DDGraphNode_INDEX(node);
  DDGraphNode_FORLEAVE_DDGraphArc(node, arc) {
    DDGraphNode head = DDGraphArc_HEAD(arc);
    int theta = DDGraphArc_THETA(arc);
    int omega = DDGraphArc_OMEGA(arc);
    int32_t head_index = DDGraphNode_INDEX(head);
    if (head_index >= firstIndex && head_index <= lastIndex) {
      int32_t head_offset = offset + head_index - index + omega*bodyCount;
      //Except_CHECK(head_offset >= offset);
      if (head_offset >= 0 && head_offset < taskCount) {
        AcyclicTask head_task = tasksBase + head_offset;
        int head_release = AcyclicTask_RELEASE(head_task);
        if (head_release < startDate + theta) {
          head_release = startDate + theta;
        }
0&&
        SCD_TRACE && fprintf(SCD_TRACE, "notify\t") && DDGraphArc_pretty(arc, SCD_TRACE)
                  && fprintf(SCD_TRACE, "\trelease_%d#%d=%d\n", head_index,
                                        AcyclicTask_INSTANCE(head_task), head_release);
        *AcyclicTask__RELEASE(head_task) = head_release;
        if (--*AcyclicTask__PREDCOUNT(head_task) == 0) {
          PQueue_insert(waiting, head_task, head_release);
        }
      }
    }
  } DDGraphNode_ENDLEAVE_DDGraphArc;
  if (succ != NULL) {
    int succ_release = AcyclicTask_RELEASE(succ);
    if (succ_release < startDate + lambda) {
      succ_release = startDate + lambda;
    }
    *AcyclicTask__RELEASE(succ) = succ_release;
    if (--*AcyclicTask__PREDCOUNT(succ) == 0) {
      PQueue_insert(waiting, succ, succ_release);
    }
  }
}

#ifdef $XCC__h
bool
Acyclic_pretty(const_Acyclic this, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
bool
Acyclic_pretty(const_Acyclic this, FILE *file)
{
  const_IStack tasks = Acyclic_TASKS(this);
  const AcyclicTask_ *tasksBase = IStack_base(tasks);
  if (file == NULL) file = stderr;
  fprintf(file, "*** Acyclic:\n");
  Acyclic_FOREACH_AcyclicTask(this, task) {
    fprintf(file, "    ");
    AcyclicTask_pretty(task, file, tasksBase);
  } Acyclic_ENDEACH_AcyclicTask;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

static int
Acyclic_Schedule(Scheduler scheduler, int lambda_min, int lambda_max, int length)
{
  GrahamListTask *index2task = Scheduler_INDEX2TASK(scheduler);
  IssueTable issueTable = Scheduler_ISSUETABLE(scheduler);
  int32_t firstIndex = Scheduler_FIRSTINDEX(scheduler);
  int32_t lastIndex = Scheduler_LASTINDEX(scheduler);
  int unwinding = Scheduler_UNWINDING(scheduler);
  int lambda = lambda_min, minSpan, mayUnwind;
  Acyclic_ acyclic[1];
  Acyclic_Ctor(acyclic, Memory_Root, scheduler);
  Acyclic_enterTasks(acyclic);
  minSpan = Acyclic_makeCritical1(acyclic, lambda);
  SCD_TRACE && Acyclic_pretty(acyclic, SCD_TRACE);
  mayUnwind = (2*minSpan + lambda - 1)/lambda;
  if (mayUnwind > unwinding) mayUnwind = unwinding;
  SCD_TRACE && fprintf(SCD_TRACE, "*** MinSpan=%d\tLAMBDA=%d\tmayUnwind=%d\tunwinding=%d\n",
                                  minSpan, lambda, mayUnwind, Scheduler_UNWINDING(scheduler));
  do {
    AcyclicState_ state[1];
    AcyclicTask firstTask = NULL;
    Memory memory = Acyclic_MEMORY(acyclic);
    int barOmega = Acyclic_BAROMEGA(acyclic);
    int unwinding = Acyclic_UNWINDING(acyclic);
    int bodyCount = Acyclic_BODYCOUNT(acyclic);
    AcyclicState_Ctor(state, memory, bodyCount, lambda, barOmega);
    if (unwinding > 0) {
      Acyclic_makeCritical2(acyclic, lambda);
      Acyclic_makeCritical3(acyclic, lambda);
    }
    Except_DEBUG(Acyclic_checkCritical(acyclic, lambda));
    firstTask = Acyclic_trySchedule(acyclic, state);
    SCD_TRACE && AcyclicState_pretty(state, SCD_TRACE);
    if (firstTask != NULL) {
      int instance = AcyclicTask_INSTANCE(firstTask);
      AcyclicTask lastTask = firstTask + (bodyCount - 1);
      int firstDate = AcyclicTask_STARTDATE(firstTask);
      int processing = AcyclicTask_PROCESSING(lastTask);
      int lastDate = AcyclicTask_STARTDATE(lastTask);
      int makeSpan = lastDate + processing - firstDate;
      if (AcyclicState_converged(state, firstTask)) {
        // Done with scheduling.
        Acyclic_commitSchedule(acyclic, firstTask, lambda);
        SCD_TRACE && fprintf(SCD_TRACE, "\n*** Scheduled\tLAMBDA=%d\t", lambda)
                  && fprintf(SCD_TRACE, "makeSpan=%d\tinstance=\%d\tmayUnwind=%d\tunwinding=%d\n",
                                        makeSpan, instance, mayUnwind, unwinding);
        break;
      }
    } else {
      // Reached end of unwinded problem schedule.
      AcyclicTask lastTask = AcyclicState_lastTask(state);
      int instance = AcyclicTask_INSTANCE(lastTask);
      int processing = AcyclicTask_PROCESSING(lastTask);
      int lastDate = AcyclicTask_STARTDATE(lastTask);
      AcyclicState_advance(state, lastDate + 1);
      SCD_TRACE && fprintf(SCD_TRACE, "EXITDATE=%d\tLAMBDA=%d\tERRORDATE=%d\tERRORCOUNT=%d\n",
                                      lastDate + 1, lambda,
                                      AcyclicState_ERRORDATE(state),
                                      AcyclicState_ERRORCOUNT(state));
      firstTask = lastTask - (bodyCount - 1);
      Except_CHECK(instance == unwinding);
      if (unwinding <= 0) {
        int firstDate = AcyclicTask_STARTDATE(firstTask);
        int makeSpan = lastDate + processing - firstDate;
        if (lambda < makeSpan) lambda = makeSpan;
        Acyclic_commitSchedule(acyclic, firstTask, lambda);
        SCD_TRACE && fprintf(SCD_TRACE, "\n*** Scheduled\tLAMBDA=%d\tmakeSpan=%d\n",
                                        lambda, makeSpan);
        break;
      }
      if (AcyclicState_converged(state, firstTask)) {
        int delta = Acyclic_violatedDependencies(acyclic, firstTask, lambda);
        while (delta != 0) {
          lambda += delta;
          delta = Acyclic_violatedDependencies(acyclic, firstTask, lambda);
        }
        Acyclic_commitSchedule(acyclic, firstTask, lambda);
        SCD_TRACE && fprintf(SCD_TRACE, "\n*** Scheduled\tLAMBDA=%d\tinstance=\%d\n",
                                        lambda, instance);
        break;
      }
    }
    // Increment LAMBDA.
    if (Acyclic_CONVERGE & 0x2) {
      int deltaCount = AcyclicState_DELTACOUNT(state);
      int32_t deltaTotal = AcyclicState_DELTATOTAL(state);
      if (deltaCount > 0) {
        int delta = deltaTotal/deltaCount;
        if (delta > 0) lambda += delta;
        else lambda++;
      } else lambda++;
    } else lambda++;
    Except_CHECK(firstTask != NULL && unwinding > 0);
    //Except_DEBUG(!Acyclic_refillNodeStack(acyclic, lambda));
    Acyclic_refillNodeStack(acyclic, lambda);
    if (Acyclic_CRITICAL) Acyclic_makeCritical1(acyclic, lambda);
    AcyclicState_Dtor(state);
    Acyclic_reset(acyclic);
  } while (lambda <= lambda_max);
  Acyclic_Dtor(acyclic);
  return lambda;
}

#ifdef $XCC__h
int
Acyclic_violatedDependencies(Acyclic this, AcyclicTask firstTask, int lambda);
#endif//$XCC__h

int
Acyclic_violatedDependencies(Acyclic this, AcyclicTask firstTask, int lambda)
{
  int delta = 0;
  int32_t firstIndex = Acyclic_FIRSTINDEX(this);
  int32_t lastIndex = Acyclic_LASTINDEX(this);
  int32_t bodyCount = Acyclic_BODYCOUNT(this);
  AcyclicTask lastTask = firstTask + bodyCount - 1, task;
  for (task = firstTask; task <= lastTask; task++) {
    DDGraphNode node = AcyclicTask_NODE(task);
    int32_t index = DDGraphNode_INDEX(node);
    int startDate = AcyclicTask_STARTDATE(task);
    DDGraphNode_FORLEAVE_DDGraphArc(node, arc) {
      DDGraphNode head = DDGraphArc_HEAD(arc);
      int32_t head_index = DDGraphNode_INDEX(head);
      if (head_index >= firstIndex && head_index <= lastIndex) {
        int theta = DDGraphArc_THETA(arc);
        int omega = DDGraphArc_OMEGA(arc);
        AcyclicTask head_task = task + head_index - index;
        Except_CHECK(head_task >= firstTask && head_task <= lastTask);
        if (omega > 0) {
          int slack = startDate + theta - AcyclicTask_STARTDATE(head_task);
          if (lambda*omega < slack) {
            int new_lambda = (slack + omega - 1)/omega;
            SCD_TRACE && fprintf(SCD_TRACE, "*** Violated (_%d, _%d)", index, head_index);
            if (delta < new_lambda - lambda) {
              delta = new_lambda - lambda;
              SCD_TRACE && fprintf(SCD_TRACE, "\tdelta=%d", delta);
            }
            SCD_TRACE && fprintf(SCD_TRACE, "\n");
          }
        }
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  }
  return delta;
}

#ifdef $XCC__h
void
Acyclic_commitSchedule(Acyclic this, AcyclicTask firstTask, int lambda);
#endif//$XCC__h

void
Acyclic_commitSchedule(Acyclic this, AcyclicTask firstTask, int lambda)
{
  int32_t bodyCount = Acyclic_BODYCOUNT(this);
  int32_t scheduled = Acyclic_SCHEDULED(this);
  Scheduler scheduler = Acyclic_SCHEDULER(this);
  IssueTable issueTable = Scheduler_ISSUETABLE(scheduler);
  GrahamListTask *index2task = Scheduler_INDEX2TASK(scheduler);
  AcyclicTask lastTask = firstTask + bodyCount - 1, task;
  IssueTable_ready(issueTable, lambda);
  for (task = firstTask; task <= lastTask; task++) {
    DDGraphNode node = AcyclicTask_NODE(task);
    int startDate = AcyclicTask_STARTDATE(task);
    int32_t nodeIndex = DDGraphNode_INDEX(node);
    int tau = startDate%lambda, phi = startDate/lambda;
    GrahamListTask listTask = index2task[nodeIndex];
    IssueItem item = GrahamListTask_ITEM(listTask);
    *GrahamListTask__TAU(listTask) = tau;
    *GrahamListTask__PHI(listTask) = phi;
    IssueTable_insert(issueTable, item, tau);
    SCD_TRACE && AcyclicTask_pretty(task, SCD_TRACE, NULL);
  }
  *Scheduler__BODYCOUNT(scheduler) = bodyCount;
  *Scheduler__SCHEDULED(scheduler) = scheduled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Acyclic module.
 */
void
Acyclic_INIT(void);
#endif//$XCC__h

void
Acyclic_INIT(void)
{
#ifndef Acyclic_CRITICAL
  if (GETENV("Acyclic_CRITICAL")) {
    sscanf(GETENV("Acyclic_CRITICAL"), "%d", &Acyclic_CRITICAL);
    fprintf(stderr, "*** Acyclic_CRITICAL=%d\n", Acyclic_CRITICAL);
  }
#endif//Acyclic_CRITICAL
#ifndef Acyclic_CONVERGE
  if (GETENV("Acyclic_CONVERGE")) {
    sscanf(GETENV("Acyclic_CONVERGE"), "%d", &Acyclic_CONVERGE);
    fprintf(stderr, "*** Acyclic_CONVERGE=%d\n", Acyclic_CONVERGE);
  }
#endif//Acyclic_CONVERGE
  Scheduler_SetSchedule(0, Acyclic_Schedule);
}

#ifdef $XCC__h
/*
 * Acyclic_FINI -- Finalize the Acyclic module.
 */
#define Acyclic_FINI()
#endif//$XCC__h

#if XCC__C
static void
Acyclic_TEST(void)
{
#include "SCD/Acyclic_.c"
}

int
main(int argc, char **argv)
{
  SCD_Test(Acyclic_TEST);
  return 0;
}
#endif

