#ifdef $XCC_h
/*
 * !!!!	TopDown.xcc
 *
 * Author: Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2001 - 2007 STMicroelectronics
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
 * @ingroup AIR
 * @brief Top-down instruction scheduler.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/TopDown.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 *
 */
typedef enum {
  TopDownEffectFlag_Write = 0x1,
} enum_TopDownEffectFlag;
typedef uint8_t TopDownEffectFlags;
#endif//$XCC__h

#ifdef $XCC__h
/*
 * Read or Write effect for the TopDown Array of Dependency Lists.
 */
struct TopDownEffect_ {
  //@args	InstructionTask task, InstructionAction action, TopDownEffectFlags flags
  TopDownEffectFlags FLAGS;		// This TopDownEffect flags.
  //@access isRead	((TopDownEffect_FLAGS(this) & TopDownEffectFlag_Write) == 0)
  //@access isWrite	((TopDownEffect_FLAGS(this) & TopDownEffectFlag_Write) != 0)
  Opcode OPCODE;			// Cached Opcode of the TASK.
  InstructionTask TASK;			// Owning InstructionTask of this TopDownEffect.
  InstructionAction ACTION;		// InstructionAction of this TopDownEffect.
  struct TopDownEffect_ *NEXT;		// Next TopDownEffect in the TopDownEffectList.
  struct TopDownEffect_ *PRED;		// Dependence predecessor of this TopDownEffect.
  struct TopDownEffect_ *SUCC;		// Dependence successor of this TopDownEffect.
};
#endif//$XCC__h

TopDownEffect
TopDownEffect_Ctor(TopDownEffect this,
                    InstructionTask task, InstructionAction action, TopDownEffectFlags flags)
{
  *TopDownEffect__FLAGS(this) = flags;
  *TopDownEffect__OPCODE(this) = InstructionTask_OPCODE(task);
  *TopDownEffect__TASK(this) = task;
  *TopDownEffect__ACTION(this) = action;
  *TopDownEffect__NEXT(this) = NULL;
  *TopDownEffect__PRED(this) = NULL;
  *TopDownEffect__SUCC(this) = NULL;
  return this;
}

size_t
TopDownEffect_Size(InstructionTask task, InstructionAction action, TopDownEffectFlags flags)
{
  return sizeof(TopDownEffect_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(TopDownEffect_)\t%zu\n", sizeof(TopDownEffect_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Compute the RAW latency between two TopDownEffect(s).
 */
int
TopDownEffect_latencyRAW(const_TopDownEffect this, const_TopDownEffect that);
#endif//$XCC__h

int
TopDownEffect_latencyRAW(const_TopDownEffect this, const_TopDownEffect that)
{
  Opcode this_opcode = TopDownEffect_OPCODE(this);
  Opcode that_opcode = TopDownEffect_OPCODE(that);
  InstructionAction this_action = TopDownEffect_ACTION(this);
  InstructionAction that_action = TopDownEffect_ACTION(that);
  StorageCell cell = InstructionAction_CELL(this_action);
  int biasRAW = InstructionAction_RAW(this_action);
  int latency = InstructionAction_STAGE(this_action)
              - InstructionAction_STAGE(that_action);
  if (Opcode_fixupRAW(this_opcode, that_opcode)) {
    biasRAW = Opcode_biasRAW(this_opcode, that_opcode, cell, biasRAW);
    Except_CHECK(biasRAW >= 0);
  }
  latency += biasRAW;
  Except_CHECK(TopDownEffect_isWrite(this));
  Except_CHECK(TopDownEffect_isRead(that));
  Except_CHECK(latency >= 0);
  return latency;
}

#ifdef $XCC__h
/**
 * Compute the WAR latency between two TopDownEffect(s).
 */
int
TopDownEffect_latencyWAR(const_TopDownEffect this, const_TopDownEffect that);
#endif//$XCC__h

int
TopDownEffect_latencyWAR(const_TopDownEffect this, const_TopDownEffect that)
{
  InstructionAction this_action = TopDownEffect_ACTION(this);
  int latency = InstructionAction_WAR(this_action);
  Except_CHECK(TopDownEffect_isRead(this));
  Except_CHECK(TopDownEffect_isWrite(that));
  Except_CHECK(latency >= 0);
  return latency;
}

#ifdef $XCC__h
/**
 * Compute the WAW latency between two TopDownEffect(s).
 */
int
TopDownEffect_latencyWAW(const_TopDownEffect this, const_TopDownEffect that);
#endif//$XCC__h

int
TopDownEffect_latencyWAW(const_TopDownEffect this, const_TopDownEffect that)
{
  InstructionAction this_action = TopDownEffect_ACTION(this);
  InstructionAction that_action = TopDownEffect_ACTION(that);
  int action_WAW = InstructionAction_WAW(this_action);
  int latency = InstructionAction_STAGE(this_action)
              - InstructionAction_STAGE(that_action);
  if (latency < action_WAW) latency = action_WAW;
  Except_CHECK(TopDownEffect_isWrite(this));
  Except_CHECK(TopDownEffect_isWrite(that));
  Except_CHECK(latency >= 0);
  return latency;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * List of TopDownEffect(s).
 */
struct TopDownEffectList_ {
  //@args	
  struct TopDownEffect_ *FIRST;
  struct TopDownEffect_ *LAST;
};
#endif//$XCC__h

TopDownEffectList
TopDownEffectList_Ctor(TopDownEffectList this)
{
  *TopDownEffectList__FIRST(this) = NULL;
  *TopDownEffectList__LAST(this) = NULL;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(TopDownEffectList_)\t%zu\n", sizeof(TopDownEffectList_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * True iff this TopDownEffectList is empty.
 */
bool
TopDownEffectList_isEmpty(const_TopDownEffectList this);
#endif//$XCC__h

bool
TopDownEffectList_isEmpty(const_TopDownEffectList this)
{
  return TopDownEffectList_FIRST(this) == NULL;
}

#ifdef $XCC__h
/**
 * For use by TopDownEffectList_FOREACH_TopDownEffect.
 */
TopDownEffect
TopDownEffectList_first(const_TopDownEffectList this);
#endif//$XCC__h

TopDownEffect
TopDownEffectList_first(const_TopDownEffectList this)
{
  return TopDownEffectList_FIRST(this);
}

#ifdef $XCC__h
/**
 * Iterator
 */
#define TopDownEffectList_FOREACH_TopDownEffect(this, effect) { \
  TopDownEffect TopDownEffectList_NEXT = NULL; \
  TopDownEffect effect = TopDownEffectList_first(this); \
  for (; effect != NULL; effect = TopDownEffectList_NEXT) { \
    TopDownEffectList_NEXT = TopDownEffect_NEXT(effect);
#define TopDownEffectList_ENDEACH_TopDownEffect \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Push an already allocated TopDownEffect.
 */
TopDownEffect
TopDownEffectList_push_(TopDownEffectList this, TopDownEffect effect);
#endif//$XCC__h

TopDownEffect
TopDownEffectList_push_(TopDownEffectList this, TopDownEffect effect)
{
  TopDownEffect first = TopDownEffectList_FIRST(this);
  TopDownEffect last = TopDownEffectList_LAST(this);
  if (first == NULL) *TopDownEffectList__FIRST(this) = effect;
  if (last != NULL) *TopDownEffect__NEXT(last) = effect;
  *TopDownEffectList__LAST(this) = effect;
  return effect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Array of Dependency Lists (ADL) as defined by Verbrugge (Sable TR No. 2002-5).
 */
struct TopDownADL_;
#endif//$XCC_h

#ifdef $XCC__h
struct TopDownADL_ {
  //@args	Memory memory, InstructionStack instructions
  Memory MEMORY;				// Where to allocate ADLs
  InstructionStack INSTRUCTIONS;		// Instruction(s) to schedule.
  TopDownEffectList_ *LISTS;			// Array of Dependency Lists (ADL).
};
#endif//$XCC__h

TopDownADL
TopDownADL_Ctor(TopDownADL this,
                 Memory memory, InstructionStack instructions)
{
  int cellCount = StorageCell__Count, cell;
  TopDownEffectList_ *lists = Memory_alloc(memory, sizeof(TopDownEffectList_)*cellCount);
  for (cell = 0; cell < StorageCell__Count; cell++) {
    TopDownEffectList_Ctor(lists + cell);
  }
  *TopDownADL__MEMORY(this) = memory;
  *TopDownADL__INSTRUCTIONS(this) = instructions;
  *TopDownADL__LISTS(this) = lists;
  //
  InstructionStack_FOREACH_Instruction(instructions, instruction) {
    InstructionTask task = Instruction_TASK(instruction);
    InstructionAction_ *actions = Instruction_ACTIONS(instruction);
    int writeCount = InstructionTask_WRITECOUNT(task);
    int readCount = InstructionTask_READCOUNT(task), i;
    size_t effectsSize = (writeCount + readCount)*sizeof(TopDownEffect_);
    TopDownEffect_ *effects = Memory_alloc(memory, effectsSize);
    // Enter the Read effects.
    for (i = writeCount; i < writeCount + readCount; i++) {
      StorageCell cell = InstructionAction_CELL(actions + i);
      TopDownEffect effect = TopDownEffect_Ctor(effects + i, task,
                                                  actions + i, 0);
      TopDownEffectList_push_(lists + cell, effect);
    }
    // Enter the Write effects.
    for (i = 0; i < writeCount; i++) {
      StorageCell cell = InstructionAction_CELL(actions + i);
      TopDownEffect effect = TopDownEffect_Ctor(effects + i, task,
                                                  actions + i, TopDownEffectFlag_Write);
      TopDownEffectList_push_(lists + cell, effect);
    }
    *InstructionTask__EFFECTS(task) = effects;
  } InstructionStack_ENDEACH_Instruction;
  //
  TopDownADL_linkEffects(this);
  //
  return this;
}

void
TopDownADL_Dtor(TopDownADL this)
{
  Memory memory = TopDownADL_MEMORY(this);
  Memory_free_(memory, TopDownADL_LISTS(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(TopDownADL_)\t%zu\n", sizeof(TopDownADL_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * For use by TopDownADL_FOREACH_TopDownEffectList.
 */
TopDownEffectList_ *
TopDownADL_lists(TopDownADL this);
#endif//$XCC__h

TopDownEffectList
TopDownADL_lists(TopDownADL this)
{
  return TopDownADL_LISTS(this);
}

#ifdef $XCC__h
/**
 * Iterator
 */
#define TopDownADL_FOREACH_TopDownEffectList(this, list) { \
  TopDownEffectList TopDownADL_LISTS = TopDownADL_lists(this); \
  int32_t TopDownADL_CELL = 0; \
  for (; TopDownADL_CELL < StorageCell__Count; TopDownADL_CELL++) { \
    TopDownEffectList list = TopDownADL_LISTS + TopDownADL_CELL;
#define TopDownADL_ENDEACH_TopDownEffectList \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Link the TopDownEffect(s) using PRED and SUCC.
 *
 * The linking of TopDownEffect(s) creates the equivalent of a DAG for each StorageCell.
 * Ensure:	The PRED and SUCC point to the predecessor Write and successor Write.
 */
void
TopDownADL_linkEffects(TopDownADL this);
#endif//$XCC__h

void
TopDownADL_linkEffects(TopDownADL this)
{
  TopDownADL_FOREACH_TopDownEffectList(this, list) {
    TopDownEffect write_effect = NULL;
    TopDownEffectList_FOREACH_TopDownEffect(list, effect) {
      // Set PRED to write_effect the Write predecessor.
      *TopDownEffect__PRED(effect) = write_effect;
      if (TopDownEffect_isWrite(effect)) {
        if (write_effect != NULL) {
          // Set SUCC of each predecessor to this effect.
          TopDownEffect pred_effect = write_effect;
          do {
            *TopDownEffect__SUCC(pred_effect) = effect;
            pred_effect = TopDownEffect_NEXT(pred_effect);
          } while (pred_effect != effect);
        }
        write_effect = effect;
      }
    } TopDownEffectList_ENDEACH_TopDownEffect;
  } TopDownADL_ENDEACH_TopDownEffectList;
}

#ifdef $XCC_h
/**
 * Pretty-print this TopDownADL.
 */
bool
TopDownADL_pretty(TopDownADL this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
TopDownADL_pretty(TopDownADL this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "*** TopDownADL:\n");
  TopDownADL_FOREACH_TopDownEffectList(this, list) {
    if (!TopDownEffectList_isEmpty(list)) {
      fprintf(file, "    %s:\n", StorageCell_name(TopDownADL_CELL));
      TopDownEffectList_FOREACH_TopDownEffect(list, effect) {
        TopDownEffect succ_effect = TopDownEffect_SUCC(effect);
        TopDownEffect next_effect = TopDownEffect_NEXT(effect);
        InstructionTask task = TopDownEffect_TASK(effect);
        if (TopDownEffect_isWrite(effect)) {
          fprintf(file, "\tWrite\t");
        } else {
          fprintf(file, "\tRead\t");
        }
        Instruction_asmPrint((Instruction)task, file, NULL);
        if (TopDownEffect_isWrite(effect)) {
          while (next_effect != NULL) {
            if (TopDownEffect_isWrite(next_effect)) {
              fprintf(file, "\tWAW(%d)", TopDownEffect_latencyWAW(effect, next_effect));
              Except_CHECK(TopDownEffect_SUCC(effect) == next_effect);
              Except_CHECK(TopDownEffect_PRED(next_effect) == effect);
            } else {
              fprintf(file, "\tRAW(%d)", TopDownEffect_latencyRAW(effect, next_effect));
            }
            if (next_effect == succ_effect) break;
            next_effect = TopDownEffect_NEXT(next_effect);
          }
        } else /*TopDownEffect_isRead(effect)*/ {
          while (next_effect != NULL) {
            if (TopDownEffect_isWrite(next_effect)) {
              fprintf(file, "\tWAR(%d)", TopDownEffect_latencyWAR(effect, next_effect));
            }
            if (next_effect == succ_effect) break;
            next_effect = TopDownEffect_NEXT(next_effect);
          }
        }
        fprintf(file, "\n");
      } TopDownEffectList_ENDEACH_TopDownEffect;
    }
  } TopDownADL_ENDEACH_TopDownEffectList;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * TopDown scheduler (cycle-based)
 *
 * When all predecessors of an Instruction are scheduled, the Instruction goes into a
 * WAITING queue in order to become data-ready. Once data-ready, it is transferred to a
 * RELEASED queue so the highest priority Instruction(s) are tried for scheduling first.
 */
struct TopDown_;
#endif//$XCC_h

#ifdef $XCC__h
struct TopDown_ {
  //@args	Memory memory, Processor processor, InstructionStack instructions
  TopDownADL_ ADL[1];		// This TopDown Array of Dependency Lists.
  //@access INSTRUCTIONS	TopDownADL_INSTRUCTIONS(TopDown_ADL(this))
  //@access MEMORY	TopDownADL_MEMORY(TopDown_ADL(this))
  PQueue_ WAITING[1];		// Ordered by lower release date first.
  PQueue_ RELEASED[1];		// Ordered by higher priority first.
  ResourceTable_ *TABLE;	// ResourceTable.
};
#endif//$XCC__h

#define TopDown_DUALQUEUE 0

TopDown
TopDown_Ctor(TopDown this,
              Memory memory, Processor processor, InstructionStack instructions)
{
  size_t tableSize = ResourceTable_Size(processor, 0);
  ResourceTable_ *table = Memory_alloc(memory, tableSize);
  int32_t instructionCount = InstructionStack_count(instructions);
  TopDownADL_Ctor(TopDown_ADL(this), memory, instructions);
#if TopDown_DUALQUEUE
  PQueue_Ctor(TopDown_WAITING(this), memory, instructionCount);
#endif
  PQueue_Ctor(TopDown_RELEASED(this), memory, instructionCount);
  *TopDown__TABLE(this) = ResourceTable_Ctor(table, processor, 0);
  return this;
}

void
TopDown_Dtor(TopDown this)
{
  Memory memory = TopDown_MEMORY(this);
  TopDownADL_Dtor(TopDown_ADL(this));
#if TopDown_DUALQUEUE
  PQueue_Dtor(TopDown_WAITING(this));
#endif
  PQueue_Dtor(TopDown_RELEASED(this));
  Memory_free_(memory, TopDown_TABLE(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(TopDown_)\t%zu\n", sizeof(TopDown_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * For use by TopDown_FOREACH_Instruction.
 */
const_InstructionStack
TopDown_instructions(const_TopDown this);
#endif//$XCC_h

const_InstructionStack
TopDown_instructions(const_TopDown this)
{
  return TopDown_INSTRUCTIONS(this);
}

#ifdef $XCC_h
/**
 * Iterate this TopDown Instruction(s).
 */
#define TopDown_FOREACH_Instruction(this, instruction) { \
  const_InstructionStack TopDown_INSTRUCTIONS = TopDown_instructions(this); \
  InstructionStack_FOREACH_Instruction(TopDown_INSTRUCTIONS, instruction)
#define TopDown_ENDEACH_Instruction \
  InstructionStack_ENDEACH_Instruction \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this TopDown Instruction(s).
 */
#define TopDown_FORBACK_Instruction(this, instruction) { \
  const_InstructionStack TopDown_INSTRUCTIONS = TopDown_instructions(this); \
  InstructionStack_FORBACK_Instruction(TopDown_INSTRUCTIONS, instruction)
#define TopDown_ENDBACK_Instruction \
  InstructionStack_ENDBACK_Instruction \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Compute Instruction critical path length.
 *
 * Take advantage from the fact that Instruction(s) are entered in topological order.
 */
void
TopDown_makeCritical(TopDown this);
#endif//$XCC_h

void
TopDown_makeCritical(TopDown this)
{
  TopDown_FORBACK_Instruction(this, instruction) {
    InstructionTask task = Instruction_TASK(instruction);
    int critical = InstructionTask_CRITICAL(task);
    TopDownEffect_ *effects = InstructionTask_EFFECTS(task);
    int writeCount = InstructionTask_WRITECOUNT(task);
    int readCount = InstructionTask_READCOUNT(task), i;
    // Process the Write effects.
    for (i = 0; i < writeCount; i++) {
      TopDownEffect effect = effects + i;
      TopDownEffect succ_effect = TopDownEffect_SUCC(effect);
      TopDownEffect next_effect = TopDownEffect_NEXT(effect);
      while (next_effect != NULL) {
        InstructionTask next_task = TopDownEffect_TASK(next_effect);
        if (next_task != task) {
          int latency = TopDownEffect_isWrite(next_effect)?
                        TopDownEffect_latencyWAW(effect, next_effect):
                        TopDownEffect_latencyRAW(effect, next_effect);
          int new_critical = InstructionTask_CRITICAL(next_task) + latency;
          if (critical < new_critical) critical = new_critical;
          ++*InstructionTask__PREDCOUNT(next_task);
        }
        if (next_effect == succ_effect) break;
        next_effect = TopDownEffect_NEXT(next_effect);
      }
    }
    // Process the Read effects.
    for (i = writeCount; i < writeCount + readCount; i++) {
      TopDownEffect effect = effects + i;
      TopDownEffect succ_effect = TopDownEffect_SUCC(effect);
      TopDownEffect next_effect = TopDownEffect_NEXT(effect);
      while (next_effect != NULL) {
        InstructionTask next_task = TopDownEffect_TASK(next_effect);
        if (next_task != task && TopDownEffect_isWrite(next_effect)) {
          int latency = TopDownEffect_latencyWAR(effect, next_effect);
          int new_critical = InstructionTask_CRITICAL(next_task) + latency;
          if (critical < new_critical) critical = new_critical;
          ++*InstructionTask__PREDCOUNT(next_task);
        }
        if (next_effect == succ_effect) break;
        next_effect = TopDownEffect_NEXT(next_effect);
      }
    }
    // Update the Instruction CRITICAL.
    *InstructionTask__CRITICAL(task) = critical;
  } TopDown_ENDBACK_Instruction;
}

#ifdef $XCC_h
/**
 * Schedule this TopDown Instruction(s) based on priorities.
 */
void
TopDown_schedule(TopDown this);
#endif//$XCC_h

#if TopDown_DUALQUEUE
void
TopDown_schedule(TopDown this)
{
  int timeSlot = -1;
  PQueue waiting = TopDown_WAITING(this);
  PQueue released = TopDown_RELEASED(this);
  ResourceTable table = TopDown_TABLE(this);
  AIR_TRACE && fprintf(AIR_TRACE, "*** TopDown schedule %d instructions\n",
                       InstructionStack_count(TopDown_INSTRUCTIONS(this)));
  // Enter Instruction(s) with zero PREDCOUNT into the waiting queue.
  TopDown_FOREACH_Instruction(this, instruction) {
    InstructionTask task = Instruction_TASK(instruction);
    if (InstructionTask_PREDCOUNT(task) == 0) {
      int release = InstructionTask_RELEASE(task);
      PQueue_addEntry(waiting, task, release);
      Except_CHECK(release >= 0);
    }
  } TopDown_ENDEACH_Instruction;
  PQueue_makeHeap(waiting);
  // Main scheduling loop.
  while (1) {
    // Fill the released queue with the Instruction(s) data-ready at timeSlot.
    while (!PQueue_isEmpty(waiting)) {
      InstructionTask task = PQueue_extractMin(waiting);
      int release = InstructionTask_RELEASE(task);
      if (PQueue_isEmpty(released)) {
        // Move to next timeSlot.
        timeSlot = release;
        ResourceTable_advance(table, timeSlot);
      }
      if (release == timeSlot) {
        // Instruction(s) relased at timeSlot are put into the released queue.
        int critical = InstructionTask_CRITICAL(task);
        PQueue_insert(released, task, -critical&0xFFFF);
      } else {
        // Put back task into the waiting queue.
        PQueue_insert(waiting, task, release);
        break;
      }
    }
    // Try to schedule Instruction(s) in the released queue.
    if (PQueue_isEmpty(released)) return;
    do {
      InstructionTask task = PQueue_extractMin(released);
      Reservation reservation = InstructionTask_RESERVATION(task);
      AIR_TRACE && fprintf(AIR_TRACE, "    time=%d\t", timeSlot);
      if (ResourceTable_fitReservation(table, reservation, timeSlot)) {
        // Enter the task successors into the waiting queue.
        TopDownEffect_ *effects = InstructionTask_EFFECTS(task);
        int writeCount = InstructionTask_WRITECOUNT(task);
        int readCount = InstructionTask_READCOUNT(task), i;
        // Process the Read effects.
        for (i = writeCount; i < writeCount + readCount; i++) {
          TopDownEffect effect = effects + i;
          TopDownEffect succ_effect = TopDownEffect_SUCC(effect);
          TopDownEffect next_effect = TopDownEffect_NEXT(effect);
          while (next_effect != NULL) {
            InstructionTask next_task = TopDownEffect_TASK(next_effect);
            if (next_task != task && TopDownEffect_isWrite(next_effect)) {
              int latency = TopDownEffect_latencyWAR(effect, next_effect);
              int next_release = InstructionTask_RELEASE(next_task);
              if (next_release < timeSlot + latency) {
                next_release = timeSlot + latency;
              }
              *InstructionTask__RELEASE(next_task) = next_release;
              if (--*InstructionTask__PREDCOUNT(next_task) == 0) {
                PQueue_insert(waiting, next_task, next_release);
              }
            }
            if (next_effect == succ_effect) break;
            next_effect = TopDownEffect_NEXT(next_effect);
          }
        }
        // Process the Write effects.
        for (i = 0; i < writeCount; i++) {
          TopDownEffect effect = effects + i;
          TopDownEffect succ_effect = TopDownEffect_SUCC(effect);
          TopDownEffect next_effect = TopDownEffect_NEXT(effect);
          while (next_effect != NULL) {
            InstructionTask next_task = TopDownEffect_TASK(next_effect);
            if (next_task != task) {
              int latency = TopDownEffect_isWrite(next_effect)?
                            TopDownEffect_latencyWAW(effect, next_effect):
                            TopDownEffect_latencyRAW(effect, next_effect);
              int next_release = InstructionTask_RELEASE(next_task);
              if (next_release < timeSlot + latency) {
                next_release = timeSlot + latency;
              }
              *InstructionTask__RELEASE(next_task) = next_release;
              if (--*InstructionTask__PREDCOUNT(next_task) == 0) {
                PQueue_insert(waiting, next_task, next_release);
              }
            }
            if (next_effect == succ_effect) break;
            next_effect = TopDownEffect_NEXT(next_effect);
          }
        }
        // Issue task at release date.
        *InstructionTask__STARTDATE(task) = timeSlot;
        AIR_TRACE && fprintf(AIR_TRACE, "scheduled\t");
      } else {
        // Put back the task in waiting queue at timeSlot + 1.
        *InstructionTask__RELEASE(task) = timeSlot + 1;
        PQueue_insert(waiting, task, timeSlot + 1);
        AIR_TRACE && fprintf(AIR_TRACE, "delayed \t");
      }
      AIR_TRACE && Instruction_asmPrint((Instruction)task, AIR_TRACE, 0) && fprintf(AIR_TRACE, "\n");
    } while (!PQueue_isEmpty(released));
  }
}
#else//!TopDown_DUALQUEUE
void
TopDown_schedule(TopDown this)
{
  PQueue released = TopDown_RELEASED(this);
  ResourceTable table = TopDown_TABLE(this);
  // Enter Instruction(s) with zero PREDCOUNT into the released queue.
  TopDown_FOREACH_Instruction(this, instruction) {
    InstructionTask task = Instruction_TASK(instruction);
    if (InstructionTask_PREDCOUNT(task) == 0) {
      int release = InstructionTask_RELEASE(task);
      int critical = InstructionTask_CRITICAL(task);
      PQueuePriority priority = (-critical&0xFFFF) | (release<<16);
      PQueue_addEntry(released, task, priority);
    }
  } TopDown_ENDEACH_Instruction;
  PQueue_makeHeap(released);
  AIR_TRACE && fprintf(AIR_TRACE, "*** TopDown schedule %d Instructions\n",
                       InstructionStack_count(TopDown_INSTRUCTIONS(this)));
  // Instruction scheduling loop.
  while (!PQueue_isEmpty(released)) {
    InstructionTask task = PQueue_extractMin(released);
    Reservation reservation = InstructionTask_RESERVATION(task);
    int timeSlot = InstructionTask_RELEASE(task);
    AIR_TRACE && fprintf(AIR_TRACE, "    time=%d\t", timeSlot);
    ResourceTable_advance(table, timeSlot);
    if (ResourceTable_fitReservation(table, reservation, timeSlot)) {
      // Enter the task successors into the released queue.
      TopDownEffect_ *effects = InstructionTask_EFFECTS(task);
      int writeCount = InstructionTask_WRITECOUNT(task);
      int readCount = InstructionTask_READCOUNT(task), i;
      // Process the Read effects.
      for (i = writeCount; i < writeCount + readCount; i++) {
        TopDownEffect effect = effects + i;
        TopDownEffect succ_effect = TopDownEffect_SUCC(effect);
        TopDownEffect next_effect = TopDownEffect_NEXT(effect);
        while (next_effect != NULL) {
          InstructionTask next_task = TopDownEffect_TASK(next_effect);
          if (next_task != task && TopDownEffect_isWrite(next_effect)) {
            int latency = TopDownEffect_latencyWAR(effect, next_effect);
            int next_release = InstructionTask_RELEASE(next_task);
            if (next_release < timeSlot + latency) {
              next_release = timeSlot + latency;
            }
            *InstructionTask__RELEASE(next_task) = next_release;
            if (--*InstructionTask__PREDCOUNT(next_task) == 0) {
              int next_critical = InstructionTask_CRITICAL(next_task);
              PQueuePriority next_priority = (-next_critical&0xFFFF) | (next_release<<16);
              PQueue_insert(released, next_task, next_priority);
            }
          }
          if (next_effect == succ_effect) break;
          next_effect = TopDownEffect_NEXT(next_effect);
        }
      }
      // Process the Write effects.
      for (i = 0; i < writeCount; i++) {
        TopDownEffect effect = effects + i;
        TopDownEffect succ_effect = TopDownEffect_SUCC(effect);
        TopDownEffect next_effect = TopDownEffect_NEXT(effect);
        while (next_effect != NULL) {
          InstructionTask next_task = TopDownEffect_TASK(next_effect);
          if (next_task != task) {
            int latency = TopDownEffect_isWrite(next_effect)?
                          TopDownEffect_latencyWAW(effect, next_effect):
                          TopDownEffect_latencyRAW(effect, next_effect);
            int next_release = InstructionTask_RELEASE(next_task);
            if (next_release < timeSlot + latency) {
              next_release = timeSlot + latency;
            }
            *InstructionTask__RELEASE(next_task) = next_release;
            if (--*InstructionTask__PREDCOUNT(next_task) == 0) {
              int next_critical = InstructionTask_CRITICAL(next_task);
              PQueuePriority next_priority = (-next_critical&0xFFFF) | (next_release<<16);
              PQueue_insert(released, next_task, next_priority);
            }
          }
          if (next_effect == succ_effect) break;
          next_effect = TopDownEffect_NEXT(next_effect);
        }
      }
      // Issue task at timeSlot.
      *InstructionTask__STARTDATE(task) = timeSlot;
      AIR_TRACE && fprintf(AIR_TRACE, "scheduled\t");
    } else {
      // Put back the task in the released queue at timeSlot + 1.
      int critical = InstructionTask_CRITICAL(task);
      PQueuePriority priority = (-critical&0xFFFF) | (++timeSlot<<16);
      PQueue_insert(released, task, priority);
      *InstructionTask__RELEASE(task) = timeSlot;
      AIR_TRACE && fprintf(AIR_TRACE, "delayed \t");
    }
    AIR_TRACE && Instruction_asmPrint((Instruction)task, AIR_TRACE, 0) && fprintf(AIR_TRACE, "\n");
  }
}
#endif//TopDown_DUALQUEUES

#ifdef $XCC_h
/*
 * TopDown_pretty --        Pretty-print this TopDown.
 */
bool
TopDown_pretty(TopDown this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
TopDown_pretty(TopDown this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "TopDown:\n");
  TopDown_FOREACH_Instruction(this, instruction) {
    InstructionTask task = Instruction_TASK(instruction);
    fprintf(file, "    critical=%d\tpredcount=%d\t", InstructionTask_CRITICAL(task),
                                                     InstructionTask_PREDCOUNT(task));
    Instruction_asmPrint(instruction, file, 0);
    fprintf(file, "\n");
  } TopDown_ENDEACH_Instruction;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  AIR_TRACE = stdout;
}
#endif//$XCC__c

#ifdef $XCC__c
{
#ifdef MDT_CORE_st231
  Processor processor = Processor_st231_cpu;
  OpcodeEncoded_ codeWords[] = {
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:BM_TimingEnd
    { { 0x2502444c, 0x00000000 } },	// stw 36[$r12] = $r17  ## date 1
    { { 0x2502040c, 0x00000000 } },	// stw 32[$r12] = $r16  ## date 2
    { { 0x2001824c, 0x00000000 } },	// ldw $r9 = 24[$r12]
    { { 0x00010240, 0x00000000 } },	// mov $r16= $r9
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:fclose
    { { 0x2001424c, 0x00000000 } },	// ldw $r9 = 20[$r12]
    { { 0x00010240, 0x00000000 } },	// mov $r16= $r9
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:fclose
    { { 0x08001400, 0x00000000 } },	// mov $r16= 1
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:__io_ftable_get_stdentry
    { { 0x08088240, 0x0004290a } },	// mov $r9 = 139596936  ## date 12
    { { 0x2002424c, 0x00000000 } },	// ldw $r9 = 36[$r12]
    { { 0x0808a280, 0x0004290a } },	// mov $r10= 139596938  ## date 13
    { { 0x2300028a, 0x00000000 } },	// ldb $r10= 0[$r10]
    { { 0x00013280, 0x00000000 } },	// mov $r19= $r10       ## date 17
    { { 0x00012240, 0x00000000 } },	// mov $r18= $r9
    { { 0x08060440, 0x00042905 } },	// mov $r17= 139594336  ## date 17
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:fprintf
    { { 0x08001400, 0x00000000 } },	// mov $r16= 1
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:__io_ftable_get_stdentry
    { { 0x2002424c, 0x00000000 } },	// ldw $r9 = 36[$r12]
    { { 0x2002028c, 0x00000000 } },	// ldw $r10= 32[$r12]
    { { 0x00013240, 0x00000000 } },	// mov $r19= $r9
    { { 0x00012280, 0x00000000 } },	// mov $r18= $r10       ## date 23
    { { 0x08080440, 0x00042905 } },	// mov $r17= 139594368  ## date 23
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:fprintf
    { { 0x08001400, 0x00000000 } },	// mov $r16= 1
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:__io_ftable_get_stdentry
    { { 0x2001c24c, 0x00000000 } },	// ldw $r9 = 28[$r12]
    { { 0x2502840c, 0x00000000 } },	// stw 40[$r12] = $r16  ## date 26
    { { 0x00010240, 0x00000000 } },	// mov $r16= $r9
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:__wtod
    { { 0x2002424c, 0x00000000 } },	// ldw $r9 = 36[$r12]
    { { 0x2002028c, 0x00000000 } },	// ldw $r10= 32[$r12]
    { { 0x00010400, 0x00000000 } },	// mov $r16= $r16       ## date 30
    { { 0x00011440, 0x00000000 } },	// mov $r17= $r17       ## date 30
    { { 0x00012280, 0x00000000 } },	// mov $r18= $r10       ## date 33
    { { 0x00013240, 0x00000000 } },	// mov $r19= $r9
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:__divd
    { { 0x00013440, 0x00000000 } },	// mov $r19= $r17       ## date 34
    { { 0x00012400, 0x00000000 } },	// mov $r18= $r16       ## date 34
    { { 0x080a0440, 0x00042905 } },	// mov $r17= 139594400  ## date 34
    { { 0x2002824c, 0x00000000 } },	// ldw $r9 = 40[$r12]
    { { 0x00010240, 0x00000000 } },	// mov $r16= $r9
    { { 0x30000000, 0x00000000 } },	// call $r63= <Module>:fprintf
    { { 0x08000400, 0x00000000 } },	// mov $r16= 0
    { { 0x2002c04c, 0x00000000 } },	// ldw $r1 = 44[$r12]
    { { 0x20030fcc, 0x00000000 } },	// ldw $r63= 48[$r12]
    { { 0x0803830c, 0x00000000 } },	// add $r12= $r12, 56    ## date 40
    { { 0x31800000, 0x00000000 } },	// return $r63		## date 44
//
    { { 0x302065ff, 0x00000000 } },	// call $r63 = printf	## date 34
    { { 0xa504c20c, 0x00000000 } },	// stw 76[$r12] = $r8	## date 34
    { { 0xa004c20c, 0x00000000 } },	// ldw $r8 = 76[$r12]	## date 35
    { { 0xa004824c, 0x00000000 } },	// ldw $r9 = 72[$r12]	## date 36
    { { 0xa004428c, 0x00000000 } },	// ldw $r10 = 68[$r12]	## date 37
    { { 0xa00402cc, 0x00000000 } },	// ldw $r11 = 64[$r12]	## date 38
    { { 0xa003c3cc, 0x00000000 } },	// ldw $r15 = 60[$r12]	## date 39
    { { 0xa003860c, 0x00000000 } },	// ldw $r24 = 56[$r12]	## date 40
    { { 0xa003464c, 0x00000000 } },	// ldw $r25 = 52[$r12]	## date 41
    { { 0x2005068c, 0x00000000 } },	// ldw $r26 = 80[$r12]	## date 42
    { { 0x0dc046cb, 0x00000000 } },	// mul32 $r27 = $r11, 4 ## date 42
    { { 0x8001b400, 0x00000000 } },	// mov $r27 = $r16	## date 42
    { { 0x8001b29b, 0x00000000 } },	// add $r27 = $r27, $r10## date 45
    { { 0xa00006db, 0x00000000 } },	// ldw $r27 = 0[$r27]	## date 46
//
    { { 0xb880001a, 0x00000000 } },	// br $b1, L?__2_31
    { { 0x8e672049, 0x00000000 } },	// cmpgeu $b1 = $r9, 114
  };
  {
    int count = sizeof(codeWords)/sizeof(*codeWords), i;
    Instruction_ instructions_[sizeof(codeWords)/sizeof(*codeWords)];
    InstructionStack_ instructions[1];
    InstructionStack_Ctor(instructions, Memory_Root, count);
    for (i = 0; i < count; i++) {
      int startDate = -1;
      OperandsBuffer_ buffer[1];
      OpcodeEncoded opcodeCodeWords = codeWords + i;
      Decoding decoding = OpcodeEncoded_WORDS(opcodeCodeWords)[1]?
                          Decoding_st231_ANYX: Decoding_st231_ANY;
      Instruction instruction = Instruction_Decode(instructions_+i, decoding, opcodeCodeWords);
      InstructionAction_ *actions = Memory_alloc(Memory_Root, Instruction_sizeActions(instruction));
      Instruction_makeActions(instruction, actions);
      InstructionStack_push(instructions, instruction);
    }
    {
      TopDown_ scheduler[1];
      TopDownADL adl = TopDown_ADL(scheduler);
      TopDown_Ctor(scheduler, Memory_Root, processor, instructions);
      TopDownADL_pretty(adl, stdout);
      TopDown_makeCritical(scheduler);
      TopDown_pretty(scheduler, stdout);
      TopDown_schedule(scheduler);
      TopDown_Dtor(scheduler);
    }
    InstructionStack_Dtor(instructions);
  }
#endif//MDT_CORE_st231
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the TopDown module.
 */
#define TopDown_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the TopDown module.
 */
#define TopDown_FINI()
#endif//$XCC__h

#if XCC__C
static void
TopDown_TEST(void)
{
#include "AIR/TopDown_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(TopDown_TEST);
  return 0;
}
#endif

