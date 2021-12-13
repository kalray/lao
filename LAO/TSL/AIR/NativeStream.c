#ifdef $XCC_h
/*
 * !!!!	NativeStream.xcc
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
 * @brief Native code stream.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/NativeStream.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Container for native code.
 */
struct NativeStream_ {
  //@args	Memory memory, int32_t aboutBlocks
  IDList_ LITERALS[1];		// List of relocatable Literal(s).
  IDList_ NATIVEBLOCKS[1];	// List of NativeBlock(s) of this NativeStream.
  //@access ENTRYBLOCK	IDList_firstItem(NativeStream_NATIVEBLOCKS(this))
  //@access MEMORY	IDList_memory(NativeStream_NATIVEBLOCKS(this))
  PlatformUInt NATIVEPC;	// The native PC at which this NativeStream is encoded.
  PlatformUInt CODESIZE;	// The size required to encode this NativeStream.
  void *CODEBUFFER;		// Buffer where this NativeStream is encoded.
  const char *(*MAKELABELNAME)(const void *Label);
  const char *(*MAKESYMBOLNAME)(const void *Symbol);
};
#endif//$XCC_h

NativeStream
NativeStream_Ctor(NativeStream this,
                  Memory memory, int32_t aboutBlocks)
{
  IDList_Ctor(NativeStream_LITERALS(this), memory);
  IDList_Ctor(NativeStream_NATIVEBLOCKS(this), memory);
  *NativeStream__NATIVEPC(this) = 0;
  *NativeStream__CODESIZE(this) = 0;
  *NativeStream__CODEBUFFER(this) = NULL;
  *NativeStream__MAKELABELNAME(this) =
      (const char *(*)(const void *))Label_name;
  *NativeStream__MAKESYMBOLNAME(this) =
      (const char *(*)(const void *))Symbol_name;
  return this;
}

void
NativeStream_Dtor(NativeStream this)
{
  IDList_empty(NativeStream_NATIVEBLOCKS(this),
               (IDListItemRelease)NativeBlock_Dtor);
  IDList_Dtor(NativeStream_NATIVEBLOCKS(this));
  IDList_Dtor(NativeStream_LITERALS(this));
}

size_t
NativeStream_Size(Memory memory, int32_t aboutBlocks)
{
  return sizeof(NativeStream_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(NativeStream_)\t%zu\n", sizeof(NativeStream_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Create a new NativeStream.
 *
 * parent:	The parent memory allocator for this NativeStream.
 * aboutBlocks:	About the number of NativeBlocks(s) in this NativeStream.
 */
NativeStream
NativeStream_new(Memory parent, int32_t aboutBlocks);
#endif//$XCC_h

NativeStream
NativeStream_new(Memory parent, int32_t aboutBlocks)
{
  Memory memory = Memory_new(parent, true);
  size_t size = NativeStream_Size(memory, aboutBlocks);
  NativeStream this = Memory_alloc(memory, size);
  return NativeStream_Ctor(this, memory, aboutBlocks);
}

#ifdef $XCC_h
/**
 * Delete a NativeStream.
 */
NativeStream
NativeStream_delete(NativeStream this);
#endif//$XCC_h

NativeStream
NativeStream_delete(NativeStream this)
{
  if (this != NULL) {
    Memory memory = NativeStream_MEMORY(this);
#ifndef _NDTOR
    NativeStream_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * For use by NativeStream_FOREACH_Literal.
 */
static inline const_IDList
NativeStream_literals(const_NativeStream this)
{
  return NativeStream_LITERALS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate first to last Literal.
 */
#define NativeStream_FOREACH_Literal(this, literal) \
  IDList_FOREACH(NativeStream_literals(this), Literal_, literal)
#define NativeStream_ENDEACH_Literal \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Push a new Literal last on this NativeStream.
 */
Literal
NativeStream_makeLabelLiteral(NativeStream this, Label label, ImmediateValue offset, Immediate immediate);
#endif//$XCC_h

Literal
NativeStream_makeLabelLiteral(NativeStream this, Label label, ImmediateValue offset, Immediate immediate)
{
  IDList literals = NativeStream_LITERALS(this);
  size_t size = Literal_Size(LiteralType_Label, label, offset, immediate);
  Literal literal = IDList_push(literals, size);
  Literal_Ctor(literal, LiteralType_Label, label, offset, immediate);
  return literal;
}

#ifdef $XCC_h
/**
 * Make a Literal in this NativeStream symbols.
 */
Literal
NativeStream_makeSymbolLiteral(NativeStream this, Symbol symbol, ImmediateValue offset, Immediate immediate);
#endif//$XCC_h

Literal
NativeStream_makeSymbolLiteral(NativeStream this, Symbol symbol, ImmediateValue offset, Immediate immediate)
{
  IDList literals = NativeStream_LITERALS(this);
  size_t size = Literal_Size(LiteralType_Symbol, symbol, offset, immediate);
  Literal literal = IDList_push(literals, size);
  Literal_Ctor(literal, LiteralType_Symbol, symbol, offset, immediate);
  return literal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * For use by NativeStream_FOREACH_NativeBlock.
 */
static inline const_IDList
NativeStream_nativeBlocks(const_NativeStream this)
{
  return NativeStream_NATIVEBLOCKS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate first to last NativeBlock.
 */
#define NativeStream_FOREACH_NativeBlock(this, nativeBlock) \
  IDList_FOREACH(NativeStream_nativeBlocks(this), NativeBlock_, nativeBlock)
#define NativeStream_ENDEACH_NativeBlock \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Push a new NativeBlock last on this NativeStream.
 *
 * processor:	The Processor for the new NativeBlock.
 * flags:	The NativeBlockFlags for the new NativeBlock.
 * label:	label (Label Literal key) for the new NativeBlock.
 */
struct NativeBlock_ *
NativeStream_pushNativeBlock(NativeStream this, Processor processor,
                             NativeBlockFlags flags, Label label);
#endif//$XCC_h

struct NativeBlock_ *
NativeStream_pushNativeBlock(NativeStream this, Processor processor,
                             NativeBlockFlags flags, Label label)
{
  Memory memory = NativeStream_MEMORY(this);
  size_t size = NativeBlock_Size(memory, processor, flags);
  IDList nativeBlocks = NativeStream_NATIVEBLOCKS(this);
  NativeBlock nativeBlock = IDList_push(nativeBlocks, size);
  NativeBlock_Ctor(nativeBlock, memory, processor, flags);
  *NativeBlock__NATIVESTREAM(nativeBlock) = this;
  if (label != NULL) {
    *NativeBlock__LABEL(nativeBlock) = label;
  }
  return nativeBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The native PC of this NativeStream.
 */
static inline PlatformUInt
NativeStream_nativePC(const_NativeStream this)
{
  return NativeStream_NATIVEPC(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The code size of this NativeStream.
 */
static inline PlatformUInt
NativeStream_codeSize(const_NativeStream this)
{
  return NativeStream_CODESIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the NativeStream_MakeLabelName call-back.
 */
void
NativeStream_setMakeLabelName(NativeStream this,
                              const char *(*makeLabelName)(const void *Label));
#endif//$XCC_h

void
NativeStream_setMakeLabelName(NativeStream this,
                              const char *(*makeLabelName)(const void *Label))
{
  *NativeStream__MAKELABELNAME(this) = makeLabelName;
}

#ifdef $XCC_h
/**
 * Set the NativeStream_MakeSymbolName call-back.
 */
void
NativeStream_setMakeSymbolName(NativeStream this,
                               const char *(*makeSymbolName)(const void *Symbol));
#endif//$XCC_h

void
NativeStream_setMakeSymbolName(NativeStream this,
                               const char *(*makeSymbolName)(const void *Symbol))
{
  *NativeStream__MAKESYMBOLNAME(this) = makeSymbolName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Scoreboard this NativeStream Instruction(s).
 *
 * Simple Instruction scheduling using a Scoreboard on the linear order of Instruction(s)
 * in the NativeStream. Only useful for non-optimizing JIT since it is flow-insensitive.
 */
void
NativeStream_scoreboardInstructions(NativeStream this, int windowSize);
#endif//$XCC_h

void
NativeStream_scoreboardInstructions(NativeStream this, int windowSize)
{
  Processor processor = Processor__UNDEF;
  Memory memory = NativeStream_MEMORY(this);
  Scoreboard_ scoreboard_, *scoreboard = NULL;
  NativeStream_FOREACH_NativeBlock(this, nativeBlock) {
    if (processor != NativeBlock_PROCESSOR(nativeBlock)) {
      processor = NativeBlock_PROCESSOR(nativeBlock);
      if (scoreboard != NULL) Scoreboard_Dtor(scoreboard);
      scoreboard = Scoreboard_Ctor(&scoreboard_, memory, processor, windowSize);
    }
    Scoreboard_reset(scoreboard, 0);
    NativeBlock_scoreboardInstructions(nativeBlock, scoreboard);
  } NativeStream_ENDEACH_NativeBlock;
  if (scoreboard != NULL) Scoreboard_Dtor(scoreboard);
}

#ifdef $XCC_h
/**
 * Assume this NativeStream Instruction(s) are scheduled.
 *
 * Should be called before @c NativeStream_encode when the Instruction(s) already have
 * valid issue dates in their NativeBlock. Else call @c NativeStream_assumeUnscheduled.
 */
void
NativeStream_assumeScheduled(NativeStream this);
#endif//$XCC_h

void
NativeStream_assumeScheduled(NativeStream this)
{
  NativeStream_FOREACH_NativeBlock(this, nativeBlock) {
    NativeBlock_assumeScheduled(nativeBlock);
  } NativeStream_ENDEACH_NativeBlock;
}

#ifdef $XCC_h
/**
 * Assume this NativeStream Instruction(s) are not scheduled.
 *
 * Must be called if @c NativeStream_assumeScheduled is not called before
 * @c NativeStream_encode.
 */
void
NativeStream_assumeUnscheduled(NativeStream this);
#endif//$XCC_h

void
NativeStream_assumeUnscheduled(NativeStream this)
{
  NativeStream_FOREACH_NativeBlock(this, nativeBlock) {
    NativeBlock_assumeUnscheduled(nativeBlock);
  } NativeStream_ENDEACH_NativeBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Get the size required to encode this NativeStream.
 *
 * @c nativePC:	A nativePC with same alignment as that will be used for encoding.
 * Return:	The size needed to encode this NativeStream.
 */
PlatformUInt
NativeStream_getCodeSize(NativeStream this, PlatformUInt nativePC);
#endif//$XCC_h

PlatformUInt
NativeStream_getCodeSize(NativeStream this, PlatformUInt nativePC)
{
  PlatformUInt codeSize = 0;
  *NativeStream__NATIVEPC(this) = nativePC;
  NativeStream_FOREACH_NativeBlock(this, nativeBlock) {
    // Compute the code size for the nativeBlock, implying bundling.
    nativePC += NativeBlock_getCodeSize(nativeBlock, nativePC);
  } NativeStream_ENDEACH_NativeBlock;
  codeSize = nativePC - NativeStream_NATIVEPC(this);
  *NativeStream__CODESIZE(this) = codeSize;
  return codeSize;
}

#ifdef $XCC_h
/**
 * Encode this NativeStream in the given code buffer.
 *
 * Require:	NativeStream_getCodeSize has been called before.
 */
void
NativeStream_encode(NativeStream this, PlatformUInt nativePC, void *codeBuffer);
#endif//$XCC_h

void
NativeStream_encode(NativeStream this, PlatformUInt nativePC, void *codeBuffer)
{
  *NativeStream__NATIVEPC(this) = nativePC;
  *NativeStream__CODEBUFFER(this) = codeBuffer;
  Except_REQUIRE(NativeStream_CODESIZE(this) != 0);
  // Resolve this NativeStream Symbol Literal(s).
  NativeStream_FOREACH_Literal(this, literal) {
    ImmediateValue value = 0;
    if (Literal_isLabel(literal)) {
      Label label = Literal_label(literal);
      value = Label_value(label);
    } else
    if (Literal_isSymbol(literal)) {
      Symbol symbol = Literal_symbol(literal);
      value = Symbol_value(symbol);
    } else Except_NEVER(true);
    Literal_setValue(literal, value + Literal_offset(literal));
  } NativeStream_ENDEACH_Literal;
  // Resolve this NativeStream Label Literal(s).
  NativeStream_FOREACH_NativeBlock(this, nativeBlock) {
    Label label = NativeBlock_LABEL(nativeBlock);
    if (label != NULL) Label_setValue(label, nativePC);
    nativePC += NativeBlock_CODESIZE(nativeBlock);
  } NativeStream_ENDEACH_NativeBlock;
  // Encode this NativeStream NativeBlock Instruction(s).
  nativePC = NativeStream_NATIVEPC(this);
  NativeStream_FOREACH_NativeBlock(this, nativeBlock) {
    PlatformUInt codeSize = NativeBlock_CODESIZE(nativeBlock);
    NativeBlock_encode(nativeBlock, nativePC, codeBuffer);
    codeBuffer = (char *)codeBuffer + codeSize;
    nativePC += codeSize;
  } NativeStream_ENDEACH_NativeBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Print this NativeStream in assembler format.
 */
bool
NativeStream_asmPrint(const_NativeStream this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
NativeStream_asmPrint(const_NativeStream this, FILE *file)
{
  if (file == NULL)
    file = stderr;

  NativeStream_FOREACH_NativeBlock(this, nativeBlock) {
    NativeBlock_asmPrint(nativeBlock, file);
  } NativeStream_ENDEACH_NativeBlock;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty-print this NativeStream.
 */
bool
NativeStream_pretty(const_NativeStream this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
NativeStream_pretty(const_NativeStream this, FILE *file)
{
  PlatformUInt offset = 0;
  if (file == NULL) file = stderr;
  NativeStream_FOREACH_NativeBlock(this, nativeBlock) {
    NativeBlock_pretty(nativeBlock, file, offset);
    offset += NativeBlock_CODESIZE(nativeBlock);
    fprintf(file, "\n");
  } NativeStream_ENDEACH_NativeBlock;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the NativeStream module.
 */
#define NativeStream_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the NativeStream module.
 */
#define NativeStream_FINI()
#endif//$XCC__h

#if XCC__C
static void
NativeStream_TEST(void)
{
#include "AIR/NativeStream_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(NativeStream_TEST);
  return 0;
}
#endif

