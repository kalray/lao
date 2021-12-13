#ifdef $XCC_h
/*
 * !!!!	CodeTrace.xcc
 *
 * Author: Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2008 STMicroelectronics
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
 * @brief Scope for instruction scheduling.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/CodeTrace.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Implements a trace of native code for use by instruction schedulers and encoders.
 */
struct CodeTrace_ {
  //@args	Memory memory, Processor processor
  InstructionStack_ INSTRUCTIONS[1];	// This CodeTrace Instruction(s).
  //@access MEMORY	InstructionStack_memory(CodeTrace_INSTRUCTIONS(this))
  //@access BASE	InstructionStack_base(CodeTrace_INSTRUCTIONS(this))
  Processor PROCESSOR;		// The Processor this CodeTrace is for.
  int32_t GROUPCOUNT;			// Count of InstructionGroup(s).
  struct InstructionGroup_ *GROUPS;	// Array of InstructionGroup(s).
};
#endif//$XCC__h

CodeTrace
CodeTrace_Ctor(CodeTrace this,
               Memory memory, Processor processor)
{
  InstructionStack_Ctor(CodeTrace_INSTRUCTIONS(this), memory, 128);
  *CodeTrace__PROCESSOR(this) = processor;
  *CodeTrace__GROUPCOUNT(this) = -1;
  *CodeTrace__GROUPS(this) = NULL;
  return this;
}

void
CodeTrace_Dtor(CodeTrace this)
{
  Memory memory = CodeTrace_MEMORY(this);
  InstructionStack_Dtor(CodeTrace_INSTRUCTIONS(this));
  Memory_free(memory, CodeTrace_GROUPS(this));
}

size_t
CodeTrace_Size(Memory memory, Processor processor)
{
  return sizeof(CodeTrace_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(CodeTrace_)\t%zu\n", sizeof(CodeTrace_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * For use by CodeTrace_FOREACH_Instruction.
 */
const_InstructionStack
CodeTrace_instructions(const_CodeTrace this);
#endif//$XCC_h

const_InstructionStack
CodeTrace_instructions(const_CodeTrace this)
{
  return CodeTrace_INSTRUCTIONS(this);
}

#ifdef $XCC_h
/**
 * Iterate this CodeTrace Instruction(s).
 */
#define CodeTrace_FOREACH_Instruction(this, instruction) { \
  const_InstructionStack CodeTrace_INSTRUCTIONS = CodeTrace_instructions(this); \
  InstructionStack_FOREACH_Instruction(CodeTrace_INSTRUCTIONS, instruction)
#define CodeTrace_ENDEACH_Instruction \
  InstructionStack_ENDEACH_Instruction \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this CodeTrace Instruction(s).
 */
#define CodeTrace_FORBACK_Instruction(this, instruction) { \
  const_InstructionStack CodeTrace_INSTRUCTIONS = CodeTrace_instructions(this); \
  InstructionStack_FORBACK_Instruction(CodeTrace_INSTRUCTIONS, instruction)
#define CodeTrace_ENDBACK_Instruction \
  InstructionStack_ENDBACK_Instruction \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the CodeTrace module.
 */
#define CodeTrace_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the CodeTrace module.
 */
#define CodeTrace_FINI()
#endif//$XCC__h

#if XCC__C
static void
CodeTrace_TEST(void)
{
#include "AIR/CodeTrace_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(CodeTrace_TEST);
  return 0;
}
#endif

