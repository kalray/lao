#ifdef $XCC_h
/*
 * !!!!	Scoreboard.xcc
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
 * @brief Scoreboard instruction scheduler.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/Scoreboard.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Access or write action of an Opcode for use by a Scoreboard.
 */
union ScoreboardAction_ {
  struct {
    Opcode OPCODE;
    int16_t LASTDATE;
  } __;
  uint32_t PACKED;
};
typedef union ScoreboardAction_ ScoreboardAction_, *ScoreboardAction;
typedef const union ScoreboardAction_ *const_ScoreboardAction;
#define ScoreboardAction_OPCODE(this) ((this)->__.OPCODE)
#define ScoreboardAction__OPCODE(this) (&(this)->__.OPCODE)
#define ScoreboardAction_LASTDATE(this) ((this)->__.LASTDATE)
#define ScoreboardAction__LASTDATE(this) (&(this)->__.LASTDATE)
#define ScoreboardAction_PACKED(this) ((this)->PACKED)
#define ScoreboardAction__PACKED(this) (&(this)->PACKED)
#endif//$XCC_h

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ScoreboardAction_)\t%zu\n", sizeof(ScoreboardAction_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Emulates the scoreboard scheduler of a superscalar processor.
 *
 * This Scoreboard computes the earliest date an Instruction can be issued within
 * a time window, given the StorageCell uses and the Resource requirements of the
 * Instruction. The Resource requirements must ensure the Instruction(s) issued
 * at the same date by Scoreboard can always be grouped together.
 *
 * Each WRITEACTIONS entry records the last write date to a particular StorageCell.
 * Each ACCESSACTIONS entry records the last access date to a particular StorageCell.
 */
struct Scoreboard_ {
  //@args	Memory memory, Processor processor, int windowSize
  Memory MEMORY;
  ResourceTable_ *TABLE;
  //@access WINDOWSIZE	ResourceTable_WINDOWSIZE(Scoreboard_TABLE(this))
  //@access CLP2HORIZON	ResourceTable_CLP2HORIZON(Scoreboard_TABLE(this))
  //@access WINDOWSTART	ResourceTable_WINDOWSTART(Scoreboard_TABLE(this))
  ScoreboardAction_ *WRITEACTIONS;
  ScoreboardAction_ *ACCESSACTIONS;
};
#endif//$XCC_h

Scoreboard
Scoreboard_Ctor(Scoreboard this,
                Memory memory, Processor processor, int windowSize)
{
  int cellCount = StorageCell__Count, cell;
  size_t tableSize = ResourceTable_Size(processor, windowSize);
  ResourceTable_ *table = Memory_alloc(memory, tableSize);
  ScoreboardAction_ *writeActions = Memory_alloc(memory, sizeof(ScoreboardAction_)*2*cellCount);
  ScoreboardAction_ *accessActions = writeActions + cellCount;
  ScoreboardAction_ actions_min_  = { { Opcode__UNDEF, INT16_MIN } };
  uint32_t packed_min = ScoreboardAction_PACKED(&actions_min_);
  *Scoreboard__MEMORY(this) = memory;
  *Scoreboard__TABLE(this) = ResourceTable_Ctor(table, processor, windowSize);
  for (cell = 0; cell < StorageCell__Count; cell++) {
    *ScoreboardAction__PACKED(writeActions + cell) = packed_min;
  }
  for (cell = 0; cell < StorageCell__Count; cell++) {
    *ScoreboardAction__PACKED(accessActions + cell) = packed_min;
  }
  *Scoreboard__WRITEACTIONS(this) = writeActions;
  *Scoreboard__ACCESSACTIONS(this) = accessActions;
  return this;
}

void
Scoreboard_Dtor(Scoreboard this)
{
  Memory memory = Scoreboard_MEMORY(this);
  Memory_free_(memory, Scoreboard_TABLE(this));
  Memory_free_(memory, Scoreboard_WRITEACTIONS(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Scoreboard_)\t%zu\n", sizeof(Scoreboard_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Scoreboard Memory allocator.
 */
static inline Memory
Scoreboard_memory(const_Scoreboard this)
{
  return Scoreboard_MEMORY(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Try to schedule an Instruction with this Scoreboard.
 *
 * Return:	The earliest start date of @c instruction.
 *
 * The principle is to check the register dependences, then find the earliest start date
 * that satisfies the dependences and which is also free of resource conflicts. Searching
 * starts at the Instruction_RELEASE.
 *
 * For the RAW dependences, we rely on the write LASTDATE(s) combined with the action STAGE
 * correction form the Instruction actionArray.
 *
 * For the WAR and WAW dependences, we use the access LASTDATE(s) defined as the issue
 * date plus a corrective term defined as follows. For a read issued at date t, access
 * date is t + WAR latency. For a write at date t, access date is t + WAW latency.
 */
int
Scoreboard_tryInstruction(const_Scoreboard this, const_Instruction instruction);
#endif//$XCC__h

int
Scoreboard_tryInstruction(const_Scoreboard this, const_Instruction instruction)
{
  int windowStart = Scoreboard_WINDOWSTART(this);
  const ScoreboardAction_ *writeActions = Scoreboard_WRITEACTIONS(this);
  const ScoreboardAction_ *accessActions = Scoreboard_ACCESSACTIONS(this);
  const InstructionAction_ *actions = Instruction_ACTIONS(instruction);
  int writeCount = Instruction_WRITECOUNT(instruction);
  int readCount = Instruction_READCOUNT(instruction), i;
  Opcode opcode = Instruction_OPCODE(instruction);
  int startDate = Instruction_RELEASE(instruction);
  if (startDate < windowStart)
    startDate = windowStart;
  // The RAW Register latencies.
  for (i = writeCount; i < writeCount + readCount; i++) {
    InstructionAction_ action_ = actions[i];
    StorageCell cell = action_.__.CELL;
    Opcode write_opcode = ScoreboardAction_OPCODE(writeActions + cell);
    int lastWrite = ScoreboardAction_LASTDATE(writeActions + cell);
    int readStage = action_.__.STAGE, biasRAW = action_.__.RAW;
    if (Opcode_fixupRAW(write_opcode, opcode)) {
      biasRAW = Opcode_biasRAW(write_opcode, opcode, cell, biasRAW);
      if (biasRAW < 0) continue;
    }
    int readDate = lastWrite - readStage + biasRAW;
    if (startDate < readDate)
      startDate = readDate;
  }
  // The WAR and WAW register latencies.
  for (i = 0; i < writeCount; i++) {
    InstructionAction_ action_ = actions[i];
    StorageCell cell = action_.__.CELL;
    int lastWrite = ScoreboardAction_LASTDATE(writeActions + cell);
    int lastAccess = ScoreboardAction_LASTDATE(accessActions + cell);
    int writeStage = action_.__.STAGE, biasWAW = action_.__.WAW;
    int writeDate = lastWrite - writeStage + biasWAW;
    if (startDate < writeDate)
      startDate = writeDate;
    if (startDate < lastAccess)
      startDate = lastAccess;
  }
  // Try the Resource(s).
  startDate = ResourceTable_tryReservation(Scoreboard_TABLE(this),
                                           Instruction_RESERVATION(instruction),
                                           startDate);
  // Return the earliest start date found.
  return startDate;
}

#ifdef $XCC__h
/**
 * Define dynamic stall cycles for memory access instructions.
 *
 * Update the WRITEACTIONS and the ACCESSACTIONS of StorageCell__Memory if any.
 */
void
Scoreboard_setInstructionStall(Scoreboard this, Instruction instruction, int date);
#endif//$XCC__h

void
Scoreboard_setInstructionStall(Scoreboard this, Instruction instruction, int date)
{
  ScoreboardAction_ *restrict writeActions = Scoreboard_WRITEACTIONS(this);
  ScoreboardAction_ *restrict accessActions = Scoreboard_ACCESSACTIONS(this);
  const InstructionAction_ *actions = Instruction_ACTIONS(instruction);
  int writeCount = Instruction_WRITECOUNT(instruction);
  int readCount = Instruction_READCOUNT(instruction), i;
  // Read actions: update the ACCESSACTIONS of StorageCell__Memory.
  for (i = writeCount; i < writeCount + readCount; i++) {
    InstructionAction_ action_ = actions[i];
    StorageCell cell = action_.__.CELL;
    int newAccessDate = date + action_.__.WAR;
    if (cell == StorageCell__Memory) {
      *ScoreboardAction__LASTDATE(accessActions + cell) = newAccessDate;
    }
  }
  // Write actions: update the WRITEACTIONS and ACCESSACTIONS of StorageCell__Memory.
  for (i = 0; i < writeCount; i++) {
    InstructionAction_ action_ = actions[i];
    StorageCell cell = action_.__.CELL;
    int newWriteDate = date + action_.__.STAGE;
    int newAccessDate = date + action_.__.WAW;
    if (cell == StorageCell__Memory) {
      *ScoreboardAction__LASTDATE(writeActions  + cell) = newWriteDate;
      *ScoreboardAction__LASTDATE(accessActions + cell) = newAccessDate;
    }
  }
}

#ifdef $XCC__h
/**
 * Add an Instruction to this Scoreboard.
 *
 * Update the TABLE, WRITEACTIONS and ACCESSACTIONS.
 *
 * Return:	the increase of the @c instruction start date.
 */
int
Scoreboard_addInstruction(Scoreboard this, Instruction instruction, int startDate);
#endif//$XCC__h

int
Scoreboard_addInstruction(Scoreboard this, Instruction instruction, int startDate)
{
  int increase = 0;
  int windowSize = Scoreboard_WINDOWSIZE(this);
  int windowStart = Scoreboard_WINDOWSTART(this);
  ScoreboardAction_ *restrict writeActions = Scoreboard_WRITEACTIONS(this);
  ScoreboardAction_ *restrict accessActions = Scoreboard_ACCESSACTIONS(this);
  const InstructionAction_ *actions = Instruction_ACTIONS(instruction);
  int writeCount = Instruction_WRITECOUNT(instruction);
  int readCount = Instruction_READCOUNT(instruction), i;
  Opcode opcode = Instruction_OPCODE(instruction);
  Except_CHECK(startDate >= windowStart);
  // Read actions: update the ACCESSACTIONS of register and its aliases.
  for (i = writeCount; i < writeCount + readCount; i++) {
    InstructionAction_ action_ = actions[i];
    StorageCell cell = action_.__.CELL;
    int newAccessDate = startDate + action_.__.WAR;
    if (newAccessDate > ScoreboardAction_LASTDATE(accessActions + cell)) {
      *ScoreboardAction__OPCODE(accessActions + cell) = opcode;
      *ScoreboardAction__LASTDATE(accessActions + cell) = newAccessDate;
    }
  }
  // Write actions: the WRITEACTIONS and ACCESSACTIONS of register and its aliases.
  for (i = 0; i < writeCount; i++) {
    InstructionAction_ action_ = actions[i];
    StorageCell cell = action_.__.CELL;
    int newWriteDate = startDate + action_.__.STAGE;
    int newAccessDate = startDate + action_.__.WAW;
    Except_CHECK(newWriteDate >= ScoreboardAction_LASTDATE(writeActions + cell));
    Except_CHECK(newAccessDate >= ScoreboardAction_LASTDATE(accessActions + cell));
    *ScoreboardAction__OPCODE(writeActions + cell) = opcode;
    *ScoreboardAction__LASTDATE(writeActions + cell) = newWriteDate;
    *ScoreboardAction__OPCODE(accessActions + cell) = opcode;
    *ScoreboardAction__LASTDATE(accessActions + cell) = newAccessDate;
  }
  // Update the Resource(s).
  ResourceTable_addReservation(Scoreboard_TABLE(this),
                               Instruction_RESERVATION(instruction),
                               startDate);
  // Update instruction STARTDATE and compute increase.
  increase = startDate - Instruction_RELEASE(instruction);
  *Instruction__STARTDATE(instruction) = startDate;
  *Instruction__RELEASE(instruction) = startDate;
#ifndef MDT_USECOUNT
  if (MDT_USECOUNT) Opcode_UseCount[opcode]++;
#endif//MDT_USECOUNT
  return increase;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Schedule Instruction(s) using this Scoreboard.
 */
void
Scoreboard_schedule(Scoreboard this, InstructionStack instructions);
#endif//$XCC_h

void
Scoreboard_schedule(Scoreboard this, InstructionStack instructions)
{
}

#ifdef $XCC_h
/**
 * Advance time so the WINDOWSTART equals the new date.
 *
 * Require:	Given @c newDate is not lower than this Scoreboard_windowStart.
 * Ensure:	This Scoreboard_windowStart equals @c newDate.
 *
 * Used to prevent new Instruction(s) from being scheduled before @c newDate.
 */
static inline void
Scoreboard_advance(Scoreboard this, int newDate)
{
  ResourceTable_advance(Scoreboard_TABLE(this), newDate);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Reset this Scoreboard to empty state and given @c windowStart.
 *
 * The sequence +Scoreboard_merge(Scoreboard_reset(this, windowStart), that, 0)+ has
 * the effect of translating @c that Scoreboard to @c this at @c windowStart.
 */
Scoreboard
Scoreboard_reset(Scoreboard this, int windowStart);
#endif//$XCC_h

Scoreboard
Scoreboard_reset(Scoreboard this, int windowStart)
{
  int cell = 0;
  ScoreboardAction_ actions_min_  = { { Opcode__UNDEF, INT16_MIN } };
  uint32_t packed_min = ScoreboardAction_PACKED(&actions_min_);
  ScoreboardAction_ *writeActions = Scoreboard_WRITEACTIONS(this);
  ScoreboardAction_ *accessActions = Scoreboard_ACCESSACTIONS(this);
  ResourceTable_reset(Scoreboard_TABLE(this), windowStart);
  for (cell = 0; cell < StorageCell__Count; cell++) {
    *ScoreboardAction__PACKED(writeActions + cell) = packed_min;
    *ScoreboardAction__PACKED(accessActions + cell) = packed_min;
  }
  return this;
}

#ifdef $XCC_h
/**
 * Merge this Scoreboard with that delayed Scoreboard.
 *
 * Return:	true iff this Scoreboard has changed as result of merge.
 *
 * Used to merge Scoreboard(s) at control-flow join.
 */
bool
Scoreboard_merge(Scoreboard this, const_Scoreboard that, int delay);
#endif//$XCC_h

bool
Scoreboard_merge(Scoreboard this, const_Scoreboard that, int delay)
{
  bool changed = false;
  int this_windowStart = Scoreboard_WINDOWSTART(this);
  int that_windowStart = Scoreboard_WINDOWSTART(that) + delay;
  ScoreboardAction_ *this_writeActions = Scoreboard_WRITEACTIONS(this);
  ScoreboardAction_ *this_accessActions = Scoreboard_ACCESSACTIONS(this);
  const ScoreboardAction_ *that_writeActions = Scoreboard_WRITEACTIONS(that);
  const ScoreboardAction_ *that_accessActions = Scoreboard_ACCESSACTIONS(that);
  int delta = that_windowStart - this_windowStart, cell;
  for (cell = 0; cell < StorageCell__Count; cell++) {
    ScoreboardAction this_writeAction = this_writeActions + cell;
    ScoreboardAction this_accessAction = this_accessActions + cell;
    int this_lastWrite = ScoreboardAction_LASTDATE(this_writeAction);
    int this_lastAccess = ScoreboardAction_LASTDATE(this_accessAction);
    const_ScoreboardAction that_writeAction = that_writeActions + cell;
    const_ScoreboardAction that_accessAction = that_accessActions + cell;
    int that_lastWrite = ScoreboardAction_LASTDATE(that_writeAction);
    int that_lastAccess = ScoreboardAction_LASTDATE(that_accessAction);
    if (this_lastAccess < that_lastAccess - delta) {
      this_lastAccess = that_lastAccess - delta;
      changed = true;
    }
    if (this_lastWrite < that_lastWrite - delta) {
      this_lastWrite = that_lastWrite - delta;
      changed = true;
    }
    //TODO! *ScoreboardAction__OPCODE(this_writeAction) = write_opcode;
    *ScoreboardAction__LASTDATE(this_writeAction) = this_lastWrite;
    //TODO! *ScoreboardAction__OPCODE(this_accessAction) = read_opcode;
    *ScoreboardAction__LASTDATE(this_accessAction) = this_lastAccess;
  }
  changed |= ResourceTable_merge(Scoreboard_TABLE(this), Scoreboard_TABLE(that), delay);
  return changed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
bool
Scoreboard_pretty(const_Scoreboard this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Scoreboard_pretty(const_Scoreboard this, FILE *file)
{
  int clp2Horizon = Scoreboard_CLP2HORIZON(this), i;
  const ScoreboardAction_ *writeActions = Scoreboard_WRITEACTIONS(this);
  const ScoreboardAction_ *accessActions = Scoreboard_ACCESSACTIONS(this);
  int windowStart = Scoreboard_WINDOWSTART(this), cell;
  if (file == NULL) file = stderr;
  ResourceTable_pretty(Scoreboard_TABLE(this), file);
  fputs("--------|", file);
  for (i = 0; i < clp2Horizon; i++) fputs("---", file);
  fputc('\n', file);
  for (cell = 0; cell < StorageCell__Count; cell++) {
    int lastAccess = ScoreboardAction_LASTDATE(accessActions + cell);
    int lastWrite = ScoreboardAction_LASTDATE(writeActions + cell);
    if (lastAccess >= windowStart || lastWrite >= windowStart) {
      if (cell == StorageCell__Volatile) {
        fprintf(file, "%s|", StorageCell_name(cell));
      } else {
        fprintf(file, "%s\t|", StorageCell_name(cell));
      }
      for (i = 0; i < clp2Horizon; i++) {
        int date = windowStart + i;
        char a = date <= lastAccess? 'a': ' ';
        char w = date <= lastWrite? 'w': ' ';
        fprintf(file, " %c%c", a, w);
      }
      fputs("  (", file);
      if (lastAccess >= windowStart + clp2Horizon) fprintf(file, "%d", lastAccess);
      fputc(',', file);
      if (lastWrite >= windowStart + clp2Horizon) fprintf(file, "%d", lastWrite);
      fputs(")\n", file);
    }
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  Scoreboard_ this_scoreboard[1];
  Scoreboard_ that_scoreboard[1];
  Processor processor = Processor__DEFAULT;
  for (; processor < Processor__; processor++) {
    NativeStream stream = NativeStream_new(Memory_Root, 10);
    NativeBlock block = NativeStream_pushNativeBlock(stream, processor, 0, NULL);
#ifdef MDT_CORE_armv5e
    // Date=0	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv5e_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=3	add r3, r3, r1
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv5e_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR3, Register_arm_GR3, Register_arm_GR1);
    // Date=4	mov r0, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rm(
        block, Opcode_armv5e_moval_reg_rdNoPC_rm,
        Register_arm_GR0, Register_arm_GR3);
    // Date=5	blal #0
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
    // Date=6	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv5e_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=10	add r3, r3, r1 lsl #6
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm_shfimml(
        block, Opcode_armv5e_addal_lsl_imm_rdNoPC_rn_rm_shfimml,
        Register_arm_GR3, Register_arm_GR3, Register_arm_GR1,
        ImmediateValue(6));
    // Date=11	mov r0, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rm(
        block, Opcode_armv5e_moval_reg_rdNoPC_rm,
        Register_arm_GR0, Register_arm_GR3);
    // Date=12	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
    // Date=13	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv5e_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=16	add r3, r1, r3, lsl r4
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC_rmNoPC_rsNoPC(
        block, Opcode_armv5e_addal_lsl_reg_rdNoPC_rnNoPC_rmNoPC_rsNoPC,
        Register_arm_GR3, Register_arm_GR1, Register_arm_GR3, Register_arm_GR4);
    // Date=17	mov r0, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rm(
        block, Opcode_armv5e_moval_reg_rdNoPC_rm,
        Register_arm_GR0, Register_arm_GR3);
    // Date=18	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
    // Date=19	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv5e_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=20	add r0, r5, r4
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv5e_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR6, Register_arm_GR5, Register_arm_GR4);
    // Date=21	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
    // Date=22	add r1, r2, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv5e_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR1, Register_arm_GR2, Register_arm_GR3);
    // Date=23	add r0, r6, r1
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv5e_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR9, Register_arm_GR6, Register_arm_GR1);
    // Date=24	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
    // Date=25	add r2, r5, r6
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv5e_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR2, Register_arm_GR5, Register_arm_GR6);
    // Date=27	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv5e_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=28	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
    // Date=29	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv5e_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=33	ldr r5, [r1]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv5e_ldral_rdNoPC_rn,
        Register_arm_GR5, Register_arm_GR1);
    // Date=34	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
    // Date=35	add r1, r2, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv5e_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR1, Register_arm_GR2, Register_arm_GR3);
    // Date=37	add r4, r5, r1 lsl #1
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm_shfimml(
        block, Opcode_armv5e_addal_lsl_imm_rdNoPC_rn_rm_shfimml,
        Register_arm_GR4, Register_arm_GR5, Register_arm_GR1,
        ImmediateValue(1));
    // Date=38	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
    // Date=39	add r1, r2, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv5e_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR1, Register_arm_GR2, Register_arm_GR3);
    // Date=40	add r4, r1, r9 lsl #1
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm_shfimml(
        block, Opcode_armv5e_addal_lsl_imm_rdNoPC_rn_rm_shfimml,
        Register_arm_GR4, Register_arm_GR1, Register_arm_GR9,
        ImmediateValue(1));
    // Date=41	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
    // Date=42	add r1, r2, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv5e_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR1, Register_arm_GR2, Register_arm_GR3);
    // Date=45	add r4, r2, r4 lsl r1
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC_rmNoPC_rsNoPC(
        block, Opcode_armv5e_addal_lsl_reg_rdNoPC_rnNoPC_rmNoPC_rsNoPC,
        Register_arm_GR4, Register_arm_GR2, Register_arm_GR4, Register_arm_GR1);
    // Date=46	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
    // Date=47	ldr r5, [r2, #4]!
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC_offset12(
        block, Opcode_armv5e_ldral_pre_addi_rdNoPC_rnNoPC_offset13,
        Register_arm_GR5, Register_arm_GR2,
        ImmediateValue(4));
    // Date=48	ldr r6, [r2, #0x10]!
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC_offset12(
        block, Opcode_armv5e_ldral_pre_addi_rdNoPC_rnNoPC_offset13,
        Register_arm_GR6, Register_arm_GR2,
        ImmediateValue(0x10));
    // Date=49	ldr r7, [r2, #0x20]!
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC_offset12(
        block, Opcode_armv5e_ldral_pre_addi_rdNoPC_rnNoPC_offset13,
        Register_arm_GR7, Register_arm_GR2,
        ImmediateValue(0x20));
    // Date=50	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv5e_blal_btarg1,
        ImmediateValue(0));
#endif//MDT_CORE_armv5e
#ifdef MDT_CORE_armv6
    // Date=0	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv6_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=3	add r3, r3, r1
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv6_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR3, Register_arm_GR3, Register_arm_GR1);
    // Date=4	mov r0, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rm(
        block, Opcode_armv6_moval_reg_rdNoPC_rm,
        Register_arm_GR0, Register_arm_GR3);
    // Date=5	blal #0
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
    // Date=6	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv6_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=10	add r3, r3, r1 lsl #6
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm_shfimml(
        block, Opcode_armv6_addal_lsl_imm_rdNoPC_rn_rm_shfimml,
        Register_arm_GR3, Register_arm_GR3, Register_arm_GR1,
        ImmediateValue(6));
    // Date=11	mov r0, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rm(
        block, Opcode_armv6_moval_reg_rdNoPC_rm,
        Register_arm_GR0, Register_arm_GR3);
    // Date=12	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
    // Date=13	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv6_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=16	add r3, r1, r3, lsl r4
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC_rmNoPC_rsNoPC(
        block, Opcode_armv6_addal_lsl_reg_rdNoPC_rnNoPC_rmNoPC_rsNoPC,
        Register_arm_GR3, Register_arm_GR1, Register_arm_GR3, Register_arm_GR4);
    // Date=17	mov r0, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rm(
        block, Opcode_armv6_moval_reg_rdNoPC_rm,
        Register_arm_GR0, Register_arm_GR3);
    // Date=18	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
    // Date=19	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv6_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=20	add r0, r5, r4
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv6_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR6, Register_arm_GR5, Register_arm_GR4);
    // Date=21	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
    // Date=22	add r1, r2, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv6_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR1, Register_arm_GR2, Register_arm_GR3);
    // Date=23	add r0, r6, r1
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv6_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR9, Register_arm_GR6, Register_arm_GR1);
    // Date=24	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
    // Date=25	add r2, r5, r6
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv6_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR2, Register_arm_GR5, Register_arm_GR6);
    // Date=27	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv6_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=28	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
    // Date=29	ldr r1, [r2]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv6_ldral_rdNoPC_rn,
        Register_arm_GR1, Register_arm_GR2);
    // Date=33	ldr r5, [r1]
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC(
        block, Opcode_armv6_ldral_rdNoPC_rn,
        Register_arm_GR5, Register_arm_GR1);
    // Date=34	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
    // Date=35	add r1, r2, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv6_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR1, Register_arm_GR2, Register_arm_GR3);
    // Date=37	add r4, r5, r1 lsl #1
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm_shfimml(
        block, Opcode_armv6_addal_lsl_imm_rdNoPC_rn_rm_shfimml,
        Register_arm_GR4, Register_arm_GR5, Register_arm_GR1,
        ImmediateValue(1));
    // Date=38	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
    // Date=39	add r1, r2, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv6_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR1, Register_arm_GR2, Register_arm_GR3);
    // Date=40	add r4, r1, r9 lsl #1
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm_shfimml(
        block, Opcode_armv6_addal_lsl_imm_rdNoPC_rn_rm_shfimml,
        Register_arm_GR4, Register_arm_GR1, Register_arm_GR9,
        ImmediateValue(1));
    // Date=41	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
    // Date=42	add r1, r2, r3
    NativeBlock_makeInstruction_arm_rdNoPC_rn_rm(
        block, Opcode_armv6_addal_reg_rdNoPC_rn_rm,
        Register_arm_GR1, Register_arm_GR2, Register_arm_GR3);
    // Date=45	add r4, r2, r4 lsl r1
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC_rmNoPC_rsNoPC(
        block, Opcode_armv6_addal_lsl_reg_rdNoPC_rnNoPC_rmNoPC_rsNoPC,
        Register_arm_GR4, Register_arm_GR2, Register_arm_GR4, Register_arm_GR1);
    // Date=46	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
    // Date=47	ldr r5, [r2, #4]!
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC_offset12(
        block, Opcode_armv6_ldral_pre_addi_rdNoPC_rnNoPC_offset13,
        Register_arm_GR5, Register_arm_GR2,
        ImmediateValue(4));
    // Date=48	ldr r6, [r2, #0x10]!
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC_offset12(
        block, Opcode_armv6_ldral_pre_addi_rdNoPC_rnNoPC_offset13,
        Register_arm_GR6, Register_arm_GR2,
        ImmediateValue(0x10));
    // Date=49	ldr r7, [r2, #0x20]!
    NativeBlock_makeInstruction_arm_rdNoPC_rnNoPC_offset12(
        block, Opcode_armv6_ldral_pre_addi_rdNoPC_rnNoPC_offset13,
        Register_arm_GR7, Register_arm_GR2,
        ImmediateValue(0x20));
    // Date=50	blal #0 
    NativeBlock_makeInstruction_arm_btarg1(
        block, Opcode_armv6_blal_btarg1,
        ImmediateValue(0));
#endif//MDT_CORE_armv6
#ifdef MDT_CORE_st231
    // Date=0	ldb $r23 = 9[$r16]
    NativeBlock_makeInstruction_st200_nlidest_isrc2_src1(
        block, Opcode_st231_ldb_nlidest_isrc2_src1,
        Register_st200_GR23,
        ImmediateValue(9),
        Register_st200_GR16);
    // Date=0	add $r18 = $r18, -12
    NativeBlock_makeInstruction_st200_idest_src1_isrc2(
        block, Opcode_st231_add_idest_src1_isrc2,
        Register_st200_GR18, Register_st200_GR18,
        ImmediateValue(-12));
    // Date=0	add $r19 = $r19, -1
    NativeBlock_makeInstruction_st200_idest_src1_isrc2(
        block, Opcode_st231_add_idest_src1_isrc2,
        Register_st200_GR19, Register_st200_GR19,
        ImmediateValue(-1));
    // Date=1	ldb $r24 = 11[$r16]
    NativeBlock_makeInstruction_st200_nlidest_isrc2_src1(
        block, Opcode_st231_ldb_nlidest_isrc2_src1,
        Register_st200_GR24,
        ImmediateValue(11),
        Register_st200_GR16);
    // Date=1	mtb $b3 = $r19
    NativeBlock_makeInstruction_st200_bdest_src1(
        block, Opcode_st231_mtb_bdest_src1,
        Register_st200_BR3, Register_st200_GR19);
    // Date=2	ldb $r20 = 8[$r16]
    NativeBlock_makeInstruction_st200_nlidest_isrc2_src1(
        block, Opcode_st231_ldb_nlidest_isrc2_src1,
        Register_st200_GR20,
        ImmediateValue(8),
        Register_st200_GR16);
    // Date=3	shl $r23 = $r23, 8
    NativeBlock_makeInstruction_st200_idest_src1_isrc2(
        block, Opcode_st231_shl_idest_src1_isrc2,
        Register_st200_GR23, Register_st200_GR23,
        ImmediateValue(8));
    // Date=3	ldb $r21 = 10[$r16]
    NativeBlock_makeInstruction_st200_nlidest_isrc2_src1(
        block, Opcode_st231_ldb_nlidest_isrc2_src1,
        Register_st200_GR21,
        ImmediateValue(10),
        Register_st200_GR16);
    // Date=3	add $r16 = $r16, 12
    NativeBlock_makeInstruction_st200_idest_src1_isrc2(
        block, Opcode_st231_add_idest_src1_isrc2,
        Register_st200_GR16, Register_st200_GR16,
        ImmediateValue(12));
    // Date=4	shl $r24 = $r24, 24
    NativeBlock_makeInstruction_st200_idest_src1_isrc2(
        block, Opcode_st231_shl_idest_src1_isrc2,
        Register_st200_GR24, Register_st200_GR24,
        ImmediateValue(24));
    // Date=5	add $r15 = $r15, $r24
    NativeBlock_makeInstruction_st200_dest_src1_src2(
        block, Opcode_st231_add_dest_src1_src2,
        Register_st200_GR15, Register_st200_GR15,
        Register_st200_GR24);
    // Date=5	add $r20 = $r20, $r23
    NativeBlock_makeInstruction_st200_dest_src1_src2(
        block, Opcode_st231_add_dest_src1_src2,
        Register_st200_GR20, Register_st200_GR20,
        Register_st200_GR23);
    // Date=6	shl $r21 = $r21, 16
    NativeBlock_makeInstruction_st200_idest_src1_isrc2(
        block, Opcode_st231_shl_idest_src1_isrc2,
        Register_st200_GR21, Register_st200_GR21,
        ImmediateValue(16));
    // Date=7	add $r20 = $r20, $r21
    NativeBlock_makeInstruction_st200_dest_src1_src2(
        block, Opcode_st231_add_dest_src1_src2,
        Register_st200_GR20, Register_st200_GR20,
        Register_st200_GR21);
    // Date=8	add $r15 = $r15, $r20
    NativeBlock_makeInstruction_st200_dest_src1_src2(
        block, Opcode_st231_add_dest_src1_src2,
        Register_st200_GR15, Register_st200_GR15,
        Register_st200_GR20);
    // Date=8	br $b3, L?__0_7
    NativeBlock_makeInstruction_st200_bcond_btarg(
        block, Opcode_st231_br_bcond_btarg,
        Register_st200_BR3,
        ImmediateValue(-64));
#endif//MDT_CORE_st231
#ifdef MDT_CORE_k1bdp
    NativeBlock_makeInstruction_k1b_registerW_signed16(
        block, Opcode_k1bdp_MAKE_registerW_signed16_simple,
        Register_k1b_R0, ImmediateValue(0));
    NativeBlock_makeInstruction_k1b_registerW_signed16(
        block, Opcode_k1bdp_MAKE_registerW_signed16_simple,
        Register_k1b_R1, ImmediateValue(1));
    NativeBlock_makeInstruction_k1b_registerW_signed16(
        block, Opcode_k1bdp_MAKE_registerW_signed16_simple,
        Register_k1b_R2, ImmediateValue(2));
    NativeBlock_makeInstruction_k1b_registerW_signed16(
        block, Opcode_k1bdp_MAKE_registerW_signed16_simple,
        Register_k1b_R3, ImmediateValue(3));
    NativeBlock_makeInstruction_k1b_registerW_registerX_registerZ_registerY(
        block, Opcode_k1bdp_FFMA_registerW_registerX_registerZ_registerY_simple,
        Register_k1b_R0, Register_k1b_R1, Register_k1b_R2, Register_k1b_R3);
    NativeBlock_makeInstruction_k1b_registerW_registerX_registerZ_registerY(
        block, Opcode_k1bdp_FFMA_registerW_registerX_registerZ_registerY_simple,
        Register_k1b_R0, Register_k1b_R0, Register_k1b_R2, Register_k1b_R3);
    NativeBlock_makeInstruction_k1b_registerW_registerX_registerZ_registerY(
        block, Opcode_k1bdp_FFMA_registerW_registerX_registerZ_registerY_simple,
        Register_k1b_R1, Register_k1b_R1, Register_k1b_R2, Register_k1b_R3);
    NativeBlock_makeInstruction_k1b_registerW_signed16(
        block, Opcode_k1bdp_MAKE_registerW_signed16_simple,
        Register_k1b_R4, ImmediateValue(4));
    NativeBlock_makeInstruction_k1b_registerW_signed16(
        block, Opcode_k1bdp_MAKE_registerW_signed16_simple,
        Register_k1b_R5, ImmediateValue(5));
    NativeBlock_makeInstruction_k1b_registerW_signed16(
        block, Opcode_k1bdp_MAKE_registerW_signed16_simple,
        Register_k1b_R6, ImmediateValue(6));
    NativeBlock_makeInstruction_k1b_registerW_signed16(
        block, Opcode_k1bdp_MAKE_registerW_signed16_simple,
        Register_k1b_R7, ImmediateValue(7));
#endif//MDT_CORE_k1bdp
    // Scoreboard Instruction(s).
    printf("Processor %s:\n", Processor_Id(processor));
    Scoreboard_Ctor(this_scoreboard, Memory_Root, processor, 0);
    Scoreboard_Ctor(that_scoreboard, Memory_Root, processor, 0);
    NativeBlock_FOREACH_Instruction(block, instruction) {
      int startDate = -1;
      Operands operands = Opcode_operands(Instruction_OPCODE(instruction));
      InstructionAction_ *actions = alloca(Instruction_sizeActions(instruction));
      Instruction_makeActions(instruction, actions);
      startDate = Scoreboard_tryInstruction(this_scoreboard, instruction);
      Scoreboard_addInstruction(this_scoreboard, instruction, startDate);
      printf("\nDate=%d\t", startDate);
      Instruction_asmPrint(instruction, stdout, stream);
      Instruction_killActions(instruction, NULL);
      fputc('\n', stdout);
      Scoreboard_pretty(this_scoreboard, stdout);
    } NativeBlock_ENDEACH_Instruction;
    printf("\nTranslate this Scoreboard:\n");
    Scoreboard_reset(that_scoreboard, 108);
    Scoreboard_merge(that_scoreboard, this_scoreboard, 0);
    Scoreboard_pretty(that_scoreboard, stdout);
    printf("\nMerge(1) this Scoreboard:\n");
    Scoreboard_reset(that_scoreboard, 108);
    Scoreboard_merge(that_scoreboard, this_scoreboard, 1);
    Scoreboard_pretty(that_scoreboard, stdout);
    NativeStream_delete(stream);
  }
  printf("\n");
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
    { { 0x2300028a, 0x00000000 } },	// ldb$r10= 0[$r10]
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
    { { 0x0803830c, 0x00000000 } },	// add$r12= $r12, 56    ## date 40
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
  printf("*** Scoreboard:\n");
  {
    Scoreboard_ scoreboard[1];
    int count = sizeof(codeWords)/sizeof(*codeWords), i;
    Instruction_ instructions_[sizeof(codeWords)/sizeof(*codeWords)];
    Scoreboard_Ctor(scoreboard, Memory_Root, processor, 7);
    for (i = 0; i < count; i++) {
      int startDate = -1;
      OpcodeEncoded opcodeCodeWords = codeWords + i;
      Decoding decoding = OpcodeEncoded_WORDS(opcodeCodeWords)[1]?
                          Decoding_st231_ANYX: Decoding_st231_ANY;
      Instruction instruction = Instruction_Decode(instructions_+i, decoding, opcodeCodeWords);
      Operands operands = Opcode_operands(Instruction_OPCODE(instruction));
      InstructionAction_ *actions = alloca(Instruction_sizeActions(instruction));
      Instruction_makeActions(instruction, actions);
      startDate = Scoreboard_tryInstruction(scoreboard, instruction);
      Scoreboard_addInstruction(scoreboard, instruction, startDate);
      printf("\t"), Instruction_asmPrint(instruction, stdout, NULL), printf("\n");
      Instruction_killActions(instruction, NULL);
      Scoreboard_pretty(scoreboard, stdout);
    }
  }
#endif//MDT_CORE_st231
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Scoreboard module.
 */
#define Scoreboard_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Scoreboard module.
 */
#define Scoreboard_FINI()
#endif//$XCC__h

#if XCC__C
static void
Scoreboard_TEST(void)
{
#include "AIR/Scoreboard_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(Scoreboard_TEST);
  return 0;
}
#endif

