#ifdef $XCC_h
/*
 * !!!!	CodeLayout.xcc
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
 * @ingroup PFA
 * @brief Code Layout in memory.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/CodeLayout.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Build a layout of the BasicBlock(s) of the CodeRegion.
 *
 * We first form traces using the classic downward grow from a BasicBlock seed using the mutual
 * most-likely heuristic, see "Better Global Scheduling using Path Profiles" of Young et al.
 * Traces are then converted to CodeLayout(s) by breaking them and/or by tail duplication.
 * Finaly we perform basic block alignment between superblocks, based on the bottom-up greedy
 * algorithm of Pettis & Hansen as described in ``Reducing Branch Costs via Branch Alignment''
 * by Calder & Grunwald in ASPLOS 1994.
 */
struct CodeLayout_;
#endif//$XCC_h

#ifdef $XCC__h
struct CodeLayout_ {
  //@args	Memory memory, CodeRegion codeRegion
  Memory MEMORY;
  CodeRegion CODEREGION;		// The CodeRegion for this CodeLayout.
  PQueue_ SEEDBLOCKS[1];		// Where the seed BasicBlock(s) are managed.
};
#endif//$XCC__h

CodeLayout
CodeLayout_Ctor(CodeLayout this,
                 Memory memory, CodeRegion codeRegion)
{
  int32_t traceID = 0;
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  PQueue seedBlocks = CodeLayout_SEEDBLOCKS(this);
  *CodeLayout__MEMORY(this) = memory;
  *CodeLayout__CODEREGION(this) = codeRegion;
  PQueue_Ctor(seedBlocks, memory, blockCount);
  PFA_TRACE && fprintf(PFA_TRACE, "*** CodeLayout\n");
  //
  // Fill the SEEDBLOCKS priority queue.
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    union { PQueuePriority priority; double frequency; } u;
    Except_CHECK(sizeof(PQueuePriority) == sizeof(double));
    u.frequency = (double)BasicBlock_frequency(basicBlock);
    PQueue_addEntry(seedBlocks, basicBlock, -u.priority);
    //BasicBlock_setTraceId(basicBlock, 0);
  } CodeRegion_ENDEACH_BasicBlock;
  PQueue_makeHeap(seedBlocks);
  //
  // Extract the seed BasicBlock(s).
  while (!PQueue_isEmpty(seedBlocks)) {
    BasicBlock basicBlock = PQueue_extractMin(seedBlocks);
    if (BasicBlock_traceId(basicBlock) == 0) {
      //BasicBlock_setTraceId(basicBlock, ++traceID);
      PFA_TRACE && fprintf(PFA_TRACE, "%4d:Block_%d\ttraceID=%d\n",
                           BasicBlock_identity(basicBlock), BasicBlock_index(basicBlock), ++traceID);
    }
  }
  //
  return this;
}

void
CodeLayout_Dtor(CodeLayout this)
{
  PQueue_Dtor(CodeLayout_SEEDBLOCKS(this));
}

size_t
CodeLayout_Size(Memory memory, CodeRegion codeRegion)
{
  return sizeof(CodeLayout_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(CodeLayout_)\t%zu\n", sizeof(CodeLayout_));
}
#endif//$XCC__c

#ifdef $XCC_h
CodeLayout
CodeLayout_new(Memory parent, CodeRegion codeRegion);
#endif//$XCC_h

CodeLayout
CodeLayout_new(Memory parent, CodeRegion codeRegion)
{
  Memory memory = Memory_new(parent, true);
  size_t size = CodeLayout_Size(memory, codeRegion);
  CodeLayout this = Memory_alloc(memory, size);
  return CodeLayout_Ctor(this, memory, codeRegion);
}

#ifdef $XCC_h
CodeLayout
CodeLayout_delete(CodeLayout this);
#endif//$XCC_h

CodeLayout
CodeLayout_delete(CodeLayout this)
{
  if (this != NULL) {
    Memory memory = CodeLayout_MEMORY(this);
#ifndef _NDTOR
    CodeLayout_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the CodeLayout module.
 */
#define CodeLayout_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the CodeLayout module.
 */
#define CodeLayout_FINI()
#endif//$XCC__h

#if XCC__C
static void
CodeLayout_TEST(void)
{
#include "PFA/CodeLayout_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(CodeLayout_TEST);
  return 0;
}
#endif

