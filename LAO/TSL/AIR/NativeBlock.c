#ifdef $XCC_h
/*
 * !!!!	NativeBlock.xcc
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
 * @brief Native code block.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/NativeBlock.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef enum {
  NativeBlockFlag_Entry = 0x2,
  NativeBlockFlag_Exit = 0x4,
  NativeBlockFlag_Protected = 0x8,
  NativeBlockFlag_Catch = 0x10,
  NativeBlockFlag_Finally = 0x20,
  NativeBlockFlag_Filter = 0x40,
  NativeBlockFlag_Fault = 0x80,
  NativeBlockFlag_Bundled = 0x100,
} enum_NativeBlockFlag;
typedef uint16_t NativeBlockFlags;
// TODO: Replace NativeBlockFlags by NativeBlockType in JITn.
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ NativeBlockFlag_Table[1];
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ NativeBlockFlagEntries[] = {
  ETableENTRY(NativeBlockFlag, Entry),
  ETableENTRY(NativeBlockFlag, Exit),
  ETableENTRY(NativeBlockFlag, Protected),
  ETableENTRY(NativeBlockFlag, Catch),
  ETableENTRY(NativeBlockFlag, Finally),
  ETableENTRY(NativeBlockFlag, Fault),
  ETableENTRY(NativeBlockFlag, Filter),
  ETableENTRY(NativeBlockFlag, Bundled),
  ETableENTRY_NULL
};
ETable_ NativeBlockFlag_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Implements a block of native code or data.
 */
struct NativeBlock_ {
  //@args	Memory memory, Processor processor, NativeBlockFlags flags
  InstructionStack_ INSTRUCTIONS[1];	// This NativeBlock Instruction(s).
  //@access MEMORY	InstructionStack_memory(NativeBlock_INSTRUCTIONS(this))
  struct NativeStream_ *NATIVESTREAM;	// The NativeStream where this NativeBlock lives.
  Processor PROCESSOR;		// The Processor this NativeBlock is for.
  bool FILLEMPTY;			// True if must skip empty InstructionGroup(s).
  NativeBlockFlags FLAGS;		// This NativeBlock NativeBlockFlags.
  //@access isEntry	((NativeBlock_flags(this) & NativeBlockFlag_Entry) != 0)
  //@access isExit	((NativeBlock_flags(this) & NativeBlockFlag_Exit) != 0)
  //@access isProtected	((NativeBlock_flags(this) & NativeBlockFlag_Protected) != 0)
  //@access isCatch	((NativeBlock_flags(this) & NativeBlockFlag_Catch) != 0)
  //@access isFinally	((NativeBlock_flags(this) & NativeBlockFlag_Finally) != 0)
  //@access isFilter	((NativeBlock_flags(this) & NativeBlockFlag_Filter) != 0)
  //@access isFault	((NativeBlock_flags(this) & NativeBlockFlag_Fault) != 0)
  //@access isBundled	((NativeBlock_flags(this) & NativeBlockFlag_Bundled) != 0)
  Label LABEL;				// This NativeBlock Label.
  int32_t LASTDATE;			// Last start date after scheduling.
  //@access isScheduled	(NativeBlock_lastDate(this) >= 0)
  int32_t GROUPCOUNT;			// Count of InstructionGroup(s).
  struct InstructionGroup_ *GROUPS;	// Array of InstructionGroup(s).
  PlatformUInt CODESIZE;		// The size required to encode this NativeBlock.
};
#endif//$XCC_h

NativeBlock
NativeBlock_Ctor(NativeBlock this,
                 Memory memory, Processor processor, NativeBlockFlags flags)
{
  InstructionStack_Ctor(NativeBlock_INSTRUCTIONS(this), memory, 64);
  *NativeBlock__NATIVESTREAM(this) = NULL;
  *NativeBlock__PROCESSOR(this) = processor;
  *NativeBlock__FILLEMPTY(this) = !Processor_interlocks(processor);
  *NativeBlock__FLAGS(this) = flags;
  *NativeBlock__LABEL(this) = NULL;
  *NativeBlock__LASTDATE(this) = -1;
  *NativeBlock__GROUPCOUNT(this) = -1;
  *NativeBlock__GROUPS(this) = NULL;
  *NativeBlock__CODESIZE(this) = 0;
  return this;
}

void
NativeBlock_Dtor(NativeBlock this)
{
  Memory memory = NativeBlock_MEMORY(this);
  InstructionStack_Dtor(NativeBlock_INSTRUCTIONS(this));
  Memory_free(memory, NativeBlock_GROUPS(this));
}

size_t
NativeBlock_Size(Memory memory, Processor processor, NativeBlockFlags flags)
{
  return sizeof(NativeBlock_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(NativeBlock_)\t%zu\n", sizeof(NativeBlock_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Create a new NativeBlock.
 *
 * parent:       The parent memory allocator for this NativeBlock.
 * aboutBlocks:  About the number of NativeBlocks(s) in this NativeBlock.
 */
NativeBlock
NativeBlock_new(Memory parent, Processor processor, NativeBlockFlags flags);
#endif//$XCC_h

NativeBlock
NativeBlock_new(Memory parent, Processor processor, NativeBlockFlags flags)
{
  Memory memory = Memory_new(parent, true);
  size_t size = NativeBlock_Size(memory, processor, flags);
  NativeBlock this = Memory_alloc(memory, size);
  return NativeBlock_Ctor(this, memory, processor, flags);
}

#ifdef $XCC_h
/**
 * Delete a NativeBlock.
 */
NativeBlock
NativeBlock_delete(NativeBlock this);
#endif//$XCC_h

NativeBlock
NativeBlock_delete(NativeBlock this)
{
  if (this != NULL) {
    Memory memory = NativeBlock_MEMORY(this);
#ifndef _NDTOR
    NativeBlock_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This NativeBlock flags.
 */
static inline NativeBlockFlags
NativeBlock_flags(const_NativeBlock this)
{
  return NativeBlock_FLAGS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this NativeBlock flags.
 */
static inline void
NativeBlock_setFlags(NativeBlock this, NativeBlockFlags flags)
{
  *NativeBlock__FLAGS(this) |= flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Clear this NativeBlock flags.
 */
static inline void
NativeBlock_clearFlags(NativeBlock this, NativeBlockFlags flags)
{
  *NativeBlock__FLAGS(this) &= ~flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This NativeBlock Label.
 */
static inline Label
NativeBlock_label(const_NativeBlock this)
{
  return NativeBlock_LABEL(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This NativeBlock NativeStream.
 */
static inline struct NativeStream_ *
NativeBlock_nativeStream(const_NativeBlock this)
{
  return NativeBlock_NATIVESTREAM(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by NativeBlock_FOREACH_Instruction.
 */
static inline const_InstructionStack
NativeBlock_instructions(const_NativeBlock this)
{
  return NativeBlock_INSTRUCTIONS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this NativeBlock Instruction(s).
 */
#define NativeBlock_FOREACH_Instruction(this, instruction) { \
  const_InstructionStack NativeBlock_INSTRUCTIONS = NativeBlock_instructions(this); \
  InstructionStack_FOREACH_Instruction(NativeBlock_INSTRUCTIONS, instruction)
#define NativeBlock_ENDEACH_Instruction \
  InstructionStack_ENDEACH_Instruction \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This NativeBlock last start date.
 */
static inline int
NativeBlock_lastDate(const_NativeBlock this)
{
  return NativeBlock_LASTDATE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Scoreboard this NativeBlock Instruction(s).
 */
bool
NativeBlock_scoreboardInstructions(NativeBlock this, Scoreboard scoreboard);
#endif//$XCC_h

bool
NativeBlock_scoreboardInstructions(NativeBlock this, Scoreboard scoreboard)
{
  int lastDate = -1;
  unsigned modified = 0;
  if (FORCE_PIPELINE > 1) {
    TopDown_ scheduler[1];
    Memory memory = NativeBlock_MEMORY(this);
    Processor processor = NativeBlock_PROCESSOR(this);
    InstructionStack instructions = NativeBlock_INSTRUCTIONS(this);
    TopDown_Ctor(scheduler, memory, processor, instructions);
    TopDown_makeCritical(scheduler);
    TopDown_schedule(scheduler);
#if 0
    TopDown_Dtor(scheduler);
#endif
  }
  NativeBlock_FOREACH_Instruction(this, instruction) {
    int startDate = Scoreboard_tryInstruction(scoreboard, instruction);
    modified |= Scoreboard_addInstruction(scoreboard, instruction, startDate);
    if (lastDate < startDate) lastDate = startDate;
  } NativeBlock_ENDEACH_Instruction;
  *NativeBlock__LASTDATE(this) = lastDate;
  return modified != 0;
}

#ifdef $XCC_h
/**
 * Sort NativeBlock INSTRUCTIONS in execution order.
 *
 * FIXME! Slow code that should be replaced by reordering in the scheduler itself.
 */
void
NativeBlock_reorderInstructions(NativeBlock this);
#endif//$XCC_h

void
NativeBlock_reorderInstructions(NativeBlock this)
{
  InstructionStack instructions = NativeBlock_INSTRUCTIONS(this);
  int32_t count = InstructionStack_count(instructions);
  void *base = InstructionStack_base_(instructions);
  HackerPtr_SortArray(base, 0, count - 1, InstructionTask_compare);
  if (count != 0) {
    Instruction *instructions = (Instruction *)base;
    int span = Instruction_STARTDATE(instructions[count - 1])
             - Instruction_STARTDATE(instructions[0]) + 1;
    AIR_TRACE && fprintf(AIR_TRACE, "*** NativeBlock count=%d span=%d IPC=%g\n",
                                    count, span, (count*1.0)/span);
  }
}

#ifdef $XCC__h
/**
 * Assume this NativeBlock is scheduled.
 *
 * Require:	This NativeBlock has valid Instruction_startDate(s).
 * Return:	This NativeBlock updated NativeBlock_lastDate.
 */
void
NativeBlock_assumeScheduled(NativeBlock this);
#endif//$XCC__h

void
NativeBlock_assumeScheduled(NativeBlock this)
{
  int lastDate = -1;
  NativeBlock_FOREACH_Instruction(this, instruction) {
    int startDate = Instruction_STARTDATE(instruction);
    if (lastDate < startDate) lastDate = startDate;
    Except_CHECK(startDate >= 0);
  } NativeBlock_ENDEACH_Instruction;
  *NativeBlock__LASTDATE(this) = lastDate;
}

#ifdef $XCC__h
/**
 * Assume this NativeBlock is not scheduled.
 */
void
NativeBlock_assumeUnscheduled(NativeBlock this);
#endif//$XCC__h

void
NativeBlock_assumeUnscheduled(NativeBlock this)
{
  NativeBlock_FOREACH_Instruction(this, instruction) {
    *Instruction__STARTDATE(instruction) = -1;
  } NativeBlock_ENDEACH_Instruction;
  *NativeBlock__LASTDATE(this) = -1;
}

#ifdef $XCC_h
/**
 * For use by NativeBlock_FOREACH_InstructionGroup.
 */
static inline int32_t
NativeBlock_groupCount(const_NativeBlock this)
{
  return NativeBlock_GROUPCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by NativeBlock_FOREACH_InstructionGroup.
 */
static inline struct InstructionGroup_ *
NativeBlock_groups(const_NativeBlock this)
{
  return NativeBlock_GROUPS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this NativeBlock InstructionGroup(s).
 */
#define NativeBlock_FOREACH_InstructionGroup(this, group) { \
  InstructionGroup_ *NativeBlock_GROUPS = NativeBlock_groups(this); \
  int32_t NativeBlock_GROUPCOUNT = NativeBlock_groupCount(this); \
  int32_t NativeBlock_BUNDLEINDEX = 0; \
  for (; NativeBlock_BUNDLEINDEX < NativeBlock_GROUPCOUNT; NativeBlock_BUNDLEINDEX++) { \
    InstructionGroup group = NativeBlock_GROUPS + NativeBlock_BUNDLEINDEX;
#define NativeBlock_ENDEACH_InstructionGroup \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
* NativeBlock_isEmpty --	True iff this NativeBlock is empty.
*/
static inline bool
NativeBlock_isEmpty(const_NativeBlock this)
{
  return InstructionStack_isEmpty(NativeBlock_INSTRUCTIONS(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/*
* NativeBlock_firstInstruction --	This NativeBlock first Instruction.
*/
static inline Instruction
NativeBlock_firstInstruction(const_NativeBlock this)
{
  return InstructionStack_bottom(NativeBlock_INSTRUCTIONS(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/*
* NativeBlock_lastInstruction --	This NativeBlock last Instruction.
*/
static inline Instruction
NativeBlock_lastInstruction(const_NativeBlock this)
{
  return InstructionStack_top(NativeBlock_INSTRUCTIONS(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a new Instruction last on this NativeBlock.
 */
Instruction
NativeBlock_makeInstruction(NativeBlock this, Opcode opcode, Literal literal);
#endif//$XCC_h

Instruction
NativeBlock_makeInstruction(NativeBlock this, Opcode opcode, Literal literal)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  Instruction_Ctor(instruction, opcode, literal);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Bundle Instructions in this NativeBlock.
 *
 * Bundles contain Instruction(s) with identical non-negative start dates.
 */
void
NativeBlock_bundleInstructions(NativeBlock this, PlatformUInt nativePC);
#endif//$XCC__h

void
NativeBlock_bundleInstructions(NativeBlock this, PlatformUInt nativePC)
{
  PlatformUInt codeSize = 0;
  Memory memory = NativeBlock_MEMORY(this);
  int lastDate = NativeBlock_LASTDATE(this);
  bool fillEmpty = NativeBlock_FILLEMPTY(this);
  int groupCount = lastDate + 1, i = 0;
  InstructionGroup_ *groups = NULL;
  Except_REQUIRE(NativeBlock_GROUPS(this) == NULL);
  // Initialize NativeBlock_GROUPS.
  if (NativeBlock_isScheduled(this)) {
    // NativeBlock is scheduled.
    Except_CHECK(groupCount > 0);
    groups = Memory_alloc(memory, groupCount*sizeof(InstructionGroup_));
    for (i = 0; i < groupCount; i++) {
      InstructionGroup_Ctor(groups + i);
    }
    // Bucket sort the Instruction(s) into InstructionGroup(s).
    NativeBlock_FOREACH_Instruction(this, instruction) {
      int startDate = Instruction_startDate(instruction);
      InstructionGroup group = groups + startDate;
      InstructionGroup_addInstruction(group, instruction);
    } NativeBlock_ENDEACH_Instruction;
  } else {
    // NativeBlock is not scheduled.
    groupCount = InstructionStack_count(NativeBlock_INSTRUCTIONS(this));
    groups = Memory_alloc(memory, groupCount*sizeof(InstructionGroup_));
    NativeBlock_FOREACH_Instruction(this, instruction) {
      InstructionGroup group = InstructionGroup_Ctor(groups + i++);
      InstructionGroup_addInstruction(group, instruction);
    } NativeBlock_ENDEACH_Instruction;
  }
  if (Bundle__MaxInstructions) {
    // Find a Template for each InstructionGroup of NativeBlock_GROUPS.
    for (i = 0; i < groupCount; i++) {
      PlatformUInt currentPC = nativePC + codeSize;
      InstructionGroup group = groups + i;
      if (InstructionGroup_COUNT(group) || fillEmpty) {
        Template template = InstructionGroup_findTemplate(group, currentPC);
        codeSize += Template_increment(template);
      }
    }
  }
  *NativeBlock__FLAGS(this) |= NativeBlockFlag_Bundled;
  *NativeBlock__GROUPCOUNT(this) = groupCount;
  *NativeBlock__GROUPS(this) = groups;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Get the size required to encode this NativeBlock.
 *
 * @c nativePC:	A nativePC with same alignment as that will be used for encoding.
 * Return:	The size needed to encode this NativeBlock.
 */
PlatformUInt
NativeBlock_getCodeSize(NativeBlock this, PlatformUInt nativePC);
#endif//$XCC__h

PlatformUInt
NativeBlock_getCodeSize(NativeBlock this, PlatformUInt nativePC)
{
  PlatformUInt codeSize = nativePC;
  if (NativeBlock_GROUPS(this) == NULL) {
    NativeBlock_bundleInstructions(this, nativePC);
  }
  NativeBlock_FOREACH_InstructionGroup(this, group) {
    if (Bundle__MaxInstructions) {
      Template template = InstructionGroup_TEMPLATE(group);
      codeSize += Template_increment(template);
    } else {
      Instruction instruction = InstructionGroup_INSTRUCTION(group);
      if (instruction != NULL) {
        Opcode opcode = Instruction_OPCODE(instruction);
        codeSize += Opcode_increment(opcode);
        Except_CHECK(Opcode_increment(opcode) > 0);
      }
    }
  } NativeBlock_ENDEACH_InstructionGroup;
  codeSize -= nativePC;
  *NativeBlock__CODESIZE(this) = codeSize;
  return codeSize;
}

#ifdef $XCC__h
/**
 * Encode this NativeBlock in the given code buffer.
 */
void
NativeBlock_encode(NativeBlock this, PlatformUInt nativePC, void *codeBuffer);
#endif//$XCC__h

void
NativeBlock_encode(NativeBlock this, PlatformUInt nativePC, void *codeBuffer)
{
  PlatformUInt codeSize = 0;
  bool fillEmpty = NativeBlock_FILLEMPTY(this);
  NativeStream stream = NativeBlock_NATIVESTREAM(this);
  Except_REQUIRE(NativeBlock_isBundled(this));
  NativeBlock_FOREACH_InstructionGroup(this, group) {
    if (InstructionGroup_COUNT(group) || fillEmpty) {
      void *address = (char *)codeBuffer + codeSize;
      PlatformUInt currentPC = nativePC + codeSize;
      if (Bundle__MaxInstructions) {
        Template template = InstructionGroup_TEMPLATE(group);
        PlatformUInt increment = Template_increment(template);
        codeSize += increment;
      } else {
        Instruction instruction = InstructionGroup_INSTRUCTION(group);
        Opcode opcode = Instruction_OPCODE(instruction);
        PlatformUInt increment = Opcode_increment(opcode);
        codeSize += increment;
      }
      InstructionGroup_encode(group, currentPC, address);
    }
  } NativeBlock_ENDEACH_InstructionGroup;
  Except_ENSURE(codeSize == NativeBlock_CODESIZE(this));
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Print this NativeBlock in assembler format.
 */
bool
NativeBlock_asmPrint(const_NativeBlock this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
NativeBlock_asmPrint(const_NativeBlock this, FILE *file)
{
  NativeStream stream = NativeBlock_NATIVESTREAM(this);
  Label label = NativeBlock_LABEL(this);
  
  if (stream == NULL) {
    fprintf(stderr, "NativeBlock_asmPrint was called with undefined NativeStream\n");
    exit(EXIT_FAILURE);
  }
  
  if (file == NULL)
    file = stderr;

  if (label != NULL) {
    const char *(*makeLabelName)(const void *Label) = NativeStream_MAKELABELNAME(stream);
    const char *name = (*makeLabelName)(label);
    if (name) {
      fprintf(file, "%s:\n", name);
    } else {
      fprintf(stderr, "Could not construct label name\n");
      exit(EXIT_FAILURE);
    }
  }

  if (NativeBlock_GROUPS(this) != NULL) {
    NativeBlock_FOREACH_InstructionGroup(this, group) {
      InstructionGroup_asmPrint(group, file, stream);
      fprintf(file, "\t;;\n");
    } NativeBlock_ENDEACH_InstructionGroup;
  } else {
    NativeBlock_FOREACH_Instruction(this, instruction) {
      fputc('\t', file);
      Instruction_asmPrint(instruction, file, stream);
      fputs("\n\t;;\n", file);
    } NativeBlock_ENDEACH_Instruction;
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty-print this NativeBlock.
 */
bool
NativeBlock_pretty(const_NativeBlock this, FILE *file, int offset);
#endif//$XCC_h

#ifndef _NTRACE
bool
NativeBlock_pretty(const_NativeBlock this, FILE *file, int offset)
{
  char print[16];
  NativeStream stream = NativeBlock_NATIVESTREAM(this);
  uint8_t *codeBuffer = stream? NativeStream_CODEBUFFER(stream): NULL;
  uint8_t *encoded = codeBuffer + offset;
  NativeBlockFlags flags = NativeBlock_FLAGS(this);
  Label label = NativeBlock_LABEL(this);
  if (file == NULL) file = stderr;
  if (label != NULL) {
    const char *(*makeLabelName)(const void *Label) =
        stream?  NativeStream_MAKELABELNAME(stream): NULL;
    const char *name = makeLabelName != NULL? (*makeLabelName)(label): print;
    if (name == print) sprintf(print, "L?%p", label);
    fprintf(file, "    %s:\n", name);
  }
  if (flags != 0) {
    fputs("\t//\t", file);
    ETable_prettyFlags(NativeBlockFlag_Table, flags, "\t", file);
    fputc('\n', file);
  }
  if (NativeBlock_GROUPS(this) != NULL) {
    NativeBlock_FOREACH_InstructionGroup(this, group) {
      if (Bundle__MaxInstructions) {
        Template template = InstructionGroup_TEMPLATE(group);
        InstructionGroup_pretty(group, file, stream, offset);
        offset += Template_increment(template);
      } else {
        Instruction instruction = InstructionGroup_INSTRUCTION(group);
        if (instruction != NULL) {
          Opcode opcode = Instruction_OPCODE(instruction);
          InstructionGroup_pretty(group, file, stream, offset);
          offset += Opcode_increment(opcode);
        }
      }
      fprintf(file, ";;\n");
    } NativeBlock_ENDEACH_InstructionGroup;
  } else {
    NativeBlock_FOREACH_Instruction(this, instruction) {
      Opcode opcode = Instruction_OPCODE(instruction);
      Decoding decoding = Opcode_decoding(opcode);
      int codeSize = Decoding_wordSize(decoding)*Decoding_wordCount(decoding), i;
      OpcodeEncoded_ encoded[1]; *encoded = *Opcode_encoded(opcode);
      Instruction_encode(instruction, encoded);
      fputc('\t', file);
      for (i = Decoding_MAXSIZE - 1; i >= codeSize; i--) {
        fprintf(file, "   ");
      }
      for (i = codeSize - 1; i >= 0; i--) {
        const uint8_t *encoded = (const uint8_t *)encoded;
        fprintf(file, "%02x ", encoded[i]);
      }
      fputc('\t', file);
      Instruction_asmPrint(instruction, file, stream);
      fputc('\n', file);
    } NativeBlock_ENDEACH_Instruction;
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the NativeBlock module.
 */
#ifndef _NTRACE
void
NativeBlock_INIT(void);
#else//_NTRACE
#define NativeBlock_INIT()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
NativeBlock_INIT(void)
{
  ETable_Ctor(NativeBlockFlag_Table, Memory_Root, NativeBlockFlagEntries);
}
#endif//_NTRACE

#ifdef $XCC__h
/**
 * Finalize the NativeBlock module.
 */
#ifndef _NTRACE
void
NativeBlock_FINI(void);
#else//_NTRACE
#define NativeBlock_FINI()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
NativeBlock_FINI(void)
{
  ETable_Dtor(NativeBlockFlag_Table);
}
#endif//_NTRACE

#if XCC__C
static void
NativeBlock_TEST(void)
{
#include "AIR/NativeBlock_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(NativeBlock_TEST);
  return 0;
}
#endif

