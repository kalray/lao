#ifdef $XCC_h
/*
 * !!!!	Liveness.xcc
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
 * @brief Liveness analysis.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/Liveness.h"
#endif//$XCC__h

#ifdef $XCC_h
#ifndef Liveness_SETCOUNT
#define Liveness_SETCOUNT 16
#endif//Liveness_SETCOUNT
#endif//$XCC_h

#ifdef $XCC__h
/**
 * If not zero, use DATAFLOW
 */
#if _OPTIMIZE
//#define Liveness_DATAFLOW 1
#endif//_OPTIMIZE
#ifndef Liveness_DATAFLOW
extern int Liveness_DATAFLOW;
#endif//Liveness_DATAFLOW
#endif//$XCC__h

#ifndef Liveness_DATAFLOW
int Liveness_DATAFLOW = 1;
#endif//Liveness_DATAFLOW

#ifdef $XCC_h
/**
 * Enumerates the Liveness modes.
 *
 * Different modes are possible: http://docs.google.com/Doc?docid=dc6bn3dg_0fvdttj&hl=fr
 * Copy:
 *   a use of a PHI is placed at the end of the predecessor block
 *   the def of a PHI is placed at the beginning of the block
 *   This is the mode used by the Decoalescer.
 * Mixed:  This mode is used when some kind of PHI virtualisation is in place.
 *   At the begining of the out-of SSA phase the mode is Copy, but while PHI
 *   arguments are processed, they are added to the working sets.
 *   This mode is used in case of PHI virtualization.
 * Mplex:  The PHI acts as a multiplexer in the multiplexing region:
 *   a use of a PHI is placed on the incoming edge
 *   the def of a PHI is placed just before the entry of the block.
 *   This is the mode used by Sreedhar SAS'99.
 */
 /* Comment from fab:
    - Mplex is necessary for Sreedhar. Otherwise, by lack of interference test, it may bug.
      But Sreedhar is not robust enough and might not be used in the future.
    - Mplex might also be used for register allocation (tree scan) when considering that edges
      can be split. But this semantic is not very clean.
    - Idealy, SEQCOPIES may not use Mplex. The IR should do the work by updating correctly
      the ordering of the use point of a phi. Mplex is a pessimistic approximation of SEQCOPIES.
    - Copy mode is nothing else than a Mixed mode where no phis are processed.
 */
typedef enum {
  LivenessMode_Copy,		// SSA Live in copy mode.
  LivenessMode_Mixed,		// SSA Live in mixed mode.
  LivenessMode_Mplex,		// SSA Live in mplex mode.
} LivenessMode;
typedef uint8_t short_LivenessMode;
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Liveness state for a BasicBlock.
 */
struct LivenessState_ {
  //@args	Memory memory, int32_t temporaryCount,
  //@args	TemporarySet liveInSet, TemporarySet liveOutSet
  BasicBlock BASICBLOCK;	// BasicBlock of this LivenessState.
  bool ISLISTED;		// This LivenessState is in Liveness worklist.
  struct LivenessState_ *WORKNEXT;
  TemporarySet_ USEINSET[1];	// The use-in TemporarySet.
  TemporarySet_ KILLEDSET[1];	// The killed TemporarySet.
  TemporarySet LIVEINSET;	// The live-in TemporarySet.
  TemporarySet LIVEOUTSET;	// The live-out TemporarySet.
};
#endif//$XCC__h

LivenessState
LivenessState_Ctor(LivenessState this,
                   Memory memory, int32_t temporaryCount,
                   TemporarySet liveInSet, TemporarySet liveOutSet)
{
  *LivenessState__BASICBLOCK(this) = NULL;
  *LivenessState__ISLISTED(this) = false;
  *LivenessState__WORKNEXT(this) = NULL;
  TemporarySet_Ctor(LivenessState_USEINSET(this), memory, Liveness_SETCOUNT);
  TemporarySet_Ctor(LivenessState_KILLEDSET(this), memory, Liveness_SETCOUNT);
  *LivenessState__LIVEINSET(this) = liveInSet;
  *LivenessState__LIVEOUTSET(this) = liveOutSet;
  return this;
}

void
LivenessState_Dtor(LivenessState this)
{
  TemporarySet_Dtor(LivenessState_USEINSET(this));
  TemporarySet_Dtor(LivenessState_KILLEDSET(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LivenessState_)\t%zu\n", sizeof(LivenessState_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Local analysis for this LivenessState.
 *
 * Analyze this LivenessState BasicBlock in order to fill USEINSET and KILLEDSET.
 *
 * Require:	useInSparse is empty except perhaps for PHI arguments of successors.
 */
void
LivenessState_analyzeOperations(LivenessState this,
                                TemporarySparse useInSparse,
                                struct Temporary_ dedicated[]);
#endif//$XCC__h

void
LivenessState_analyzeOperations(LivenessState this,
                                TemporarySparse useInSparse,
                                struct Temporary_ dedicated[])
{
  TemporarySet useInSet = LivenessState_USEINSET(this);
  TemporarySet killedSet = LivenessState_KILLEDSET(this);
  BasicBlock basicBlock = LivenessState_BASICBLOCK(this);
  TemporaryTable temporaryTable = TemporarySparse_table(useInSparse);
  Except_CHECK(TemporarySet_isEmpty(useInSet));
  Except_CHECK(TemporarySet_isEmpty(killedSet));
  BasicBlock_FORBACK_Operation(basicBlock, operation) {
    RegisterSet clobberSet = Operation_clobberSet(operation);
    // Operation results.
    Operation_RESULTS_FOREACH_Temporary(operation, result) {
      if (Temporary_hasRegister(result)) {
        result = dedicated + Temporary_REGISTER(result);
      }
      if (TemporaryTable_contains(temporaryTable, result)) {
        TemporarySet_pushMember(killedSet, result);
        TemporarySparse_remove(useInSparse, result);
      }
    } Operation_RESULTS_ENDEACH_Temporary;
    // Operation clobbers.
    if (clobberSet != NULL) {
      RegisterSet_FOREACH_Register(clobberSet, registre) {
        Temporary clobbered = dedicated + registre;
        if (TemporaryTable_contains(temporaryTable, clobbered)) {
          TemporarySet_pushMember(killedSet, clobbered);
          TemporarySparse_remove(useInSparse, clobbered);
        }
      } RegisterSet_ENDEACH_Register;
    }
    if (!Operator_isPhi(Operation_operator(operation))) {
      // Operation arguments for non-PHI operations.
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        if (Temporary_hasRegister(argument)) {
          argument = dedicated + Temporary_REGISTER(argument);
        }
        if (TemporaryTable_contains(temporaryTable, argument)) {
          TemporarySparse_insert(useInSparse, argument);
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
    }
  } BasicBlock_ENDBACK_Operation;
  // Transfer the contents of useInSparse to LivenessState_USEINSET.
  TemporarySparse_FOREACH_Temporary(useInSparse, temporary) {
    TemporarySet_pushMember(LivenessState_USEINSET(this), temporary);
  } TemporarySparse_ENDEACH_Temporary;
  // Reorder KILLEDSET and USEINSET for normal TemporarySet operation.
  TemporarySet_makeOrder(killedSet);
  TemporarySet_makeOrder(useInSet);
0 &&
  PFA_TRACE && fprintf(PFA_TRACE, "*** LOCAL Block_%d", BasicBlock_index(basicBlock))
            && fprintf(PFA_TRACE, "\tuseInSet={ ")
            && TemporarySet_pretty(useInSet, PFA_TRACE)
            && fprintf(PFA_TRACE, "}\tkilledSet={ ")
            && TemporarySet_pretty(killedSet, PFA_TRACE)
            && fprintf(PFA_TRACE, "}\n");
}

/**
 * The count of the LIVEOUTSET Set.
 *
 * This is used to see if more iterations of the data-flow are required.
 */
static inline int32_t
LivenessState_countLiveOutSet(LivenessState this)
{
  return TemporarySet_count(LivenessState_LIVEOUTSET(this));
}

#ifdef $XCC__h
/**
 * Pretty-print this LivenessState.
 */
bool
LivenessState_pretty(LivenessState this, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
bool
LivenessState_pretty(LivenessState this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "\tliveInSet:\t");
  TemporarySet_pretty(LivenessState_LIVEINSET(this), file);
  fprintf(file, "\n\tliveOutSet:\t");
  TemporarySet_pretty(LivenessState_LIVEOUTSET(this), file);
  fprintf(file, "\n\tkilledSet:\t");
  TemporarySet_pretty(LivenessState_KILLEDSET(this), file);
  fprintf(file, "\n\tuseInSet:\t");
  TemporarySet_pretty(LivenessState_USEINSET(this), file);
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Liveness computation.
 *
 * The @c phiSets argument enables construction of Liveness PHIDEFSETS and LIVEOUTSETS.
 * These sets can be merged with the Liveness LIVEINSETS and LIVEOUTSETS by calling
 * Liveness_multiplex(), for use by legacy code that does not distinguish PHI semantics.
 */
struct Liveness_;
#endif//$XCC_h

#ifdef $XCC__h
struct Liveness_ {
  //@args	Memory memory, CodeRegion codeRegion,
  //@args	TemporaryTable temporaryTable, bool phiSets
  Memory MEMORY;
  CodeRegion CODEREGION;
  Temporary_ *DEDICATED;
  int32_t ITERCOUNT;
  int32_t BLOCKCOUNT;
  int32_t TEMPORARYCOUNT;
  LivenessState_ *BASESTATE;		// Base of LivenessState array.
  TemporarySet_ *LIVEINSETS;		// LiveInSets array.
  TemporarySet_ *LIVEOUTSETS;		// LiveOutSets array.
  TemporarySet_ *PHIDEFSETS;		// PhiInSets array.
  TemporarySet_ *PHIUSESETS;		// PhiOutSets array.
  LivenessState_ *WORKTAIL;		// First LivenessState in worklist.
  LivenessState_ *WORKHEAD;		// Last LivenessState in worklist.
  TemporarySparse_ SPARSESET[1];	// Sparse set for computing BasicBlock use-in.
  //@access TEMPORARYTABLE	TemporarySparse_table(Liveness_SPARSESET(this))
  BasicBlockStack_ *KILLINGBLOCKS;
  BasicBlockStack_ *CONSUMERBLOCKS;
  BasicBlock *KILLEDFLAG;
};
#endif//$XCC__h

Liveness
Liveness_Ctor(Liveness this,
              Memory memory, CodeRegion codeRegion,
              TemporaryTable temporaryTable, bool phiSets)
{
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  Procedure procedure = CodeRegion_procedure(codeRegion);
  Temporary_ *dedicated = Procedure_dedicated(procedure);
  size_t StatesSize = blockCount*sizeof(LivenessState_);
  size_t liveSetsSize = blockCount*sizeof(TemporarySet_);
  TemporarySet_ *liveInSets = Memory_alloc(memory, liveSetsSize);
  TemporarySet_ *liveOutSets = Memory_alloc(memory, liveSetsSize);
  int32_t temporaryCount = TemporaryTable_count(temporaryTable), index;
  *Liveness__MEMORY(this) = memory;
  *Liveness__CODEREGION(this) = codeRegion;
  *Liveness__DEDICATED(this) = dedicated;
  *Liveness__ITERCOUNT(this) = 0;
  *Liveness__BLOCKCOUNT(this) = blockCount;
  *Liveness__TEMPORARYCOUNT(this) = temporaryCount;
  *Liveness__BASESTATE(this) = NULL;
  for (index = 0; index < blockCount; index++) {
    TemporarySet liveInSet = liveInSets + index;
    TemporarySet liveOutSet = liveOutSets + index;
    TemporarySet_Ctor(liveInSet, memory, Liveness_SETCOUNT);
    TemporarySet_Ctor(liveOutSet, memory, Liveness_SETCOUNT);
  }
  *Liveness__LIVEINSETS(this) = liveInSets;
  *Liveness__LIVEOUTSETS(this) = liveOutSets;
  *Liveness__PHIDEFSETS(this) = NULL;
  *Liveness__PHIUSESETS(this) = NULL;
  *Liveness__WORKTAIL(this) = NULL;
  *Liveness__WORKHEAD(this) = NULL;
  TemporarySparse_Ctor(Liveness_SPARSESET(this), memory, temporaryTable);
  *Liveness__KILLINGBLOCKS(this) = NULL;
  *Liveness__CONSUMERBLOCKS(this) = NULL;
  *Liveness__KILLEDFLAG(this) = NULL;
  if (phiSets) {
    // Allocate and initialize the PHIDEFSETS and PHIUSESETS.
    size_t liveSetsSize = blockCount*sizeof(TemporarySet_);
    TemporarySet_ *phiDefSets = Memory_alloc(memory, liveSetsSize);
    TemporarySet_ *phiUseSets = Memory_alloc(memory, liveSetsSize);
    for (index = 0; index < blockCount; index++) {
      TemporarySet phiDefSet = phiDefSets + index;
      TemporarySet phiUseSet = phiUseSets + index;
      TemporarySet_Ctor(phiDefSet, memory, Liveness_SETCOUNT);
      TemporarySet_Ctor(phiUseSet, memory, Liveness_SETCOUNT);
    }
    *Liveness__PHIDEFSETS(this) = phiDefSets;
    *Liveness__PHIUSESETS(this) = phiUseSets;
  }
  // Nullify dedicated if must skip local analysis.
  if (temporaryCount == 0) {
    *Liveness__DEDICATED(this) = NULL;
  }
  if (Liveness_DATAFLOW) {
    LivenessState_ *baseState = Memory_alloc(memory, StatesSize);
    for (index = 0; index < blockCount; index++) {
      TemporarySet liveInSet = liveInSets + index;
      TemporarySet liveOutSet = liveOutSets + index;
      LivenessState_Ctor(baseState + index, memory, temporaryCount, liveInSet, liveOutSet);
    }
    *Liveness__BASESTATE(this) = baseState;
    Liveness_activateState(this, CodeRegion_rootBlock(codeRegion), phiSets);
    if (phiSets) {
      // Fill the PHIDEFSETS with the Phi results.
      TemporarySet_ *phiDefSets = Liveness_PHIDEFSETS(this);
      Liveness_FOREACH_LivenessState(this, state) {
        BasicBlock basicBlock = LivenessState_BASICBLOCK(state);
        TemporarySet phiDefSet = phiDefSets + BasicBlock_index(basicBlock);
        if (BasicBlock_phiCount(basicBlock) != 0) {
          BasicBlock_FOREACH_Operation(basicBlock, operation) {
            if (Operator_isPhi(Operation_operator(operation))) {
              Temporary result = Operation_getResult(operation, 0);
              if (Temporary_hasRegister(result)) {
                result = dedicated + Temporary_REGISTER(result);
              }
              if (TemporaryTable_contains(temporaryTable, result)) {
                TemporarySet_pushMember(phiDefSet, result);
              }
            } else break;
          } BasicBlock_ENDEACH_Operation;
        }
        TemporarySet_makeOrder(phiDefSet);
      } Liveness_ENDEACH_LivenessState;
    }
    //PFA_TRACE && fprintf(PFA_TRACE, "\n");
    Liveness_doIterate(this);
  } else {
    BasicBlockStack_ *killingBlocks = Memory_alloc(memory, temporaryCount*sizeof(BasicBlockStack_));
    BasicBlockStack_ *consumerBlocks = Memory_alloc(memory, temporaryCount*sizeof(BasicBlockStack_));
    BasicBlock *killedFlag = Memory_alloc(memory, temporaryCount*sizeof(BasicBlock));
    for (index = 0; index < temporaryCount; index++) {
      BasicBlockStack_Ctor(killingBlocks + index, memory, 8);
      BasicBlockStack_Ctor(consumerBlocks + index, memory, 8);
      killedFlag[index] = NULL;
    }
    *Liveness__KILLINGBLOCKS(this) = killingBlocks;
    *Liveness__CONSUMERBLOCKS(this) = consumerBlocks;
    *Liveness__KILLEDFLAG(this) = killedFlag;
    if (phiSets) Liveness_computePhiSets(this);
    Liveness_makeKillingConsumerBlocks(this, phiSets);
    Liveness_doUpAndMark(this);
    Liveness_orderSets(this, phiSets);
  }
0&&
  PFA_TRACE && fprintf(PFA_TRACE, "*** Liveness\tBLOCKCOUNT=%d\tITERCOUNT=%d\n",
                       Liveness_BLOCKCOUNT(this), Liveness_ITERCOUNT(this));
  return this;
}

void
Liveness_Dtor(Liveness this)
{
  Memory memory = Liveness_MEMORY(this);
  CodeRegion codeRegion = Liveness_CODEREGION(this);
  int32_t blockCount = Liveness_BLOCKCOUNT(this), index;
  int32_t temporaryCount = Liveness_TEMPORARYCOUNT(this);
  LivenessState_ *baseState = Liveness_BASESTATE(this);
  TemporarySet_ *liveInSets = Liveness_LIVEINSETS(this);
  TemporarySet_ *liveOutSets = Liveness_LIVEOUTSETS(this);
  TemporarySet_ *phiDefSets = Liveness_PHIDEFSETS(this);
  TemporarySet_ *phiUseSets = Liveness_PHIUSESETS(this);
  BasicBlockStack_ *killingBlocks = Liveness_KILLINGBLOCKS(this);
  BasicBlockStack_ *consumerBlocks = Liveness_CONSUMERBLOCKS(this);
  BasicBlock *killedFlag = Liveness_KILLEDFLAG(this);
  for (index = 0; index < blockCount; index++) {
    if (liveInSets) TemporarySet_Dtor(liveInSets + index);
    if (liveOutSets) TemporarySet_Dtor(liveOutSets + index);
    if (baseState) LivenessState_Dtor(baseState + index);
    if (phiDefSets) TemporarySet_Dtor(phiDefSets + index);
    if (phiUseSets) TemporarySet_Dtor(phiUseSets + index);
  }
  for (index = 0; index < temporaryCount; index++) {
    if (killingBlocks) BasicBlockStack_Dtor(killingBlocks + index);
    if (consumerBlocks) BasicBlockStack_Dtor(consumerBlocks + index);
  }
  Memory_free_(memory, baseState);
  Memory_free(memory, liveInSets);
  Memory_free(memory, liveOutSets);
  Memory_free(memory, phiDefSets);
  Memory_free(memory, phiUseSets);
  Memory_free(memory, killingBlocks);
  Memory_free(memory, consumerBlocks);
  Memory_free(memory, killedFlag);
  TemporarySparse_Dtor(Liveness_SPARSESET(this));
}

size_t
Liveness_Size(Memory memory, CodeRegion codeRegion,
              TemporaryTable temporaryTable, bool phiSets)
{
  return sizeof(Liveness_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Liveness_)\t%zu\n", sizeof(Liveness_));
}
#endif//$XCC__c

#ifdef $XCC_h
Liveness
Liveness_make(Memory parent, CodeRegion codeRegion, TemporaryTable temporaryTable, bool phiSets);
#endif//$XCC_h

Liveness
Liveness_make(Memory parent, CodeRegion codeRegion, TemporaryTable temporaryTable, bool phiSets)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Liveness_Size(memory, codeRegion, temporaryTable, phiSets);
  Liveness this = Memory_alloc(memory, size);
  Liveness_Ctor(this, memory, codeRegion, temporaryTable, phiSets);
  CodeRegion_setLiveSets(codeRegion,
                         Liveness_BLOCKCOUNT(this),
                         Liveness_LIVEINSETS(this),
                         Liveness_LIVEOUTSETS(this),
                         Liveness_PHIDEFSETS(this),
                         Liveness_PHIUSESETS(this));
  return this;
}

#ifdef $XCC_h
Liveness
Liveness_kill(Liveness this);
#endif//$XCC_h

Liveness
Liveness_kill(Liveness this)
{
  if (this != NULL) {
    Memory memory = Liveness_MEMORY(this);
    CodeRegion codeRegion = Liveness_CODEREGION(this);
    CodeRegion_resetLiveSets(codeRegion);
#ifndef _NDTOR
    Liveness_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Liveness Memory.
 */
Memory
Liveness_memory(const_Liveness this);
#endif//$XCC_h

Memory
Liveness_memory(const_Liveness this)
{
  return Liveness_MEMORY(this);
}

#ifdef $XCC_h
/**
 * For use in Liveness_FOREACH.
 */
int32_t
Liveness_blockCount(const_Liveness this);
#endif//$XCC_h

int32_t
Liveness_blockCount(const_Liveness this)
{
  return Liveness_BLOCKCOUNT(this);
}

#ifdef $XCC_h
/**
 * Array of live-in TemporarySet(s) indexed by BasicBlock.
 */
TemporarySet_ *
Liveness_liveInSets(const_Liveness this);
#endif//$XCC_h

TemporarySet_ *
Liveness_liveInSets(const_Liveness this)
{
  return Liveness_LIVEINSETS(this);
}

#ifdef $XCC_h
/**
 * Array of live-out TemporarySet(s) indexed by BasicBlock.
 */
TemporarySet_ *
Liveness_liveOutSets(const_Liveness this);
#endif//$XCC_h

TemporarySet_ *
Liveness_liveOutSets(const_Liveness this)
{
  return Liveness_LIVEOUTSETS(this);
}

#ifdef $XCC_h
/**
 * Array of phi-in TemporarySet(s) indexed by BasicBlock.
 */
TemporarySet_ *
Liveness_phiDefSets(const_Liveness this);
#endif//$XCC_h

TemporarySet_ *
Liveness_phiDefSets(const_Liveness this)
{
  return Liveness_PHIDEFSETS(this);
}

#ifdef $XCC_h
/**
 * Array of phi-out TemporarySet(s) indexed by BasicBlock.
 */
TemporarySet_ *
Liveness_phiUseSets(const_Liveness this);
#endif//$XCC_h

TemporarySet_ *
Liveness_phiUseSets(const_Liveness this)
{
  return Liveness_PHIUSESETS(this);
}

#ifdef $XCC__h
/**
 * For use in Liveness_FOREACH.
 */
LivenessState_ *
Liveness_baseState(const_Liveness this);
#endif//$XCC__h

LivenessState_ *
Liveness_baseState(const_Liveness this)
{
  return Liveness_BASESTATE(this);
}

#ifdef $XCC__h
/**
 * Iterate over this Liveness LivenessState.
 */
#define Liveness_FOREACH_LivenessState(this, state) { \
  int32_t Liveness_BLOCKCOUNT = Liveness_blockCount(this); \
  LivenessState Liveness_BASESTATE = Liveness_baseState(this), state; \
  LivenessState Liveness_PASTSTATE = Liveness_BASESTATE + Liveness_BLOCKCOUNT; \
  for (state = Liveness_BASESTATE; state < Liveness_PASTSTATE; ++state) { \
    if (LivenessState_BASICBLOCK(state) != NULL) {
#define Liveness_ENDEACH_LivenessState \
    } \
  } \
}
#endif//$XCC__h

#ifdef $XCC_h
/*
 Liveness_pretty -- Pretty-print this Liveness.
 */
bool
Liveness_pretty(Liveness this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Liveness_pretty(Liveness this, FILE *file)
{
  if (file == NULL) file = stderr;
  Liveness_FOREACH_LivenessState(this, state) {
    BasicBlock basicBlock = LivenessState_BASICBLOCK(state);
    fprintf(file, "    block_%d:", BasicBlock_index(basicBlock));
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(basicBlock, edge) {
      BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
      fprintf(file, "\tblock_%d", BasicBlock_index(succ_block));
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    LivenessState_pretty(state, file);
    fprintf(file, "\n");
  } Liveness_ENDEACH_LivenessState;
  return true;
}
#endif//_NTRACE

static inline LivenessState
Liveness_workTail(Liveness this)
{
  return Liveness_WORKTAIL(this);
}

static inline LivenessState
Liveness_workHead(Liveness this)
{
  return Liveness_WORKHEAD(this);
}

static inline bool
Liveness_workIsEmpty(Liveness this)
{
  return Liveness_WORKTAIL(this) == NULL;
}

// Push state at head of worklist.
static inline void
Liveness_workPush(Liveness this, LivenessState state)
{
  LivenessState workHead = Liveness_WORKHEAD(this);
  Except_CHECK(LivenessState_WORKNEXT(state) == NULL);
  if (workHead == NULL) {
    *Liveness__WORKTAIL(this) = state;
  } else {
    *LivenessState__WORKNEXT(state) = workHead;
  }
  *Liveness__WORKHEAD(this) = state;
}

// Put state at tail of worklist.
static inline void
Liveness_workPut(Liveness this, LivenessState state)
{
  LivenessState workTail = Liveness_WORKTAIL(this);
  Except_CHECK(LivenessState_WORKNEXT(state) == NULL);
  *Liveness__WORKTAIL(this) = state;
  if (workTail == NULL) {
    *Liveness__WORKHEAD(this) = state;
  } else {
    *LivenessState__WORKNEXT(workTail) = state;
  }
}

// Pop state at head of worklist.
static inline void
Liveness_workPop(Liveness this)
{
  LivenessState workTail = Liveness_WORKTAIL(this);
  LivenessState workHead = Liveness_WORKHEAD(this);
  if (workTail == workHead) {
    *Liveness__WORKTAIL(this) = *Liveness__WORKHEAD(this) = NULL;
  } else {
    *Liveness__WORKHEAD(this) = LivenessState_WORKNEXT(workHead);
  }
  *LivenessState__WORKNEXT(workHead) = NULL;
}

#ifdef $XCC__h
#define Liveness_FORWORK_LivenessState(this, state) { \
  LivenessState Liveness_WORKNEXT = NULL; \
  LivenessState state = Liveness_WORKHEAD(this); \
  for (; state != NULL; state = Liveness_WORKNEXT) { \
    Liveness_WORKNEXT = LivenessState_WORKNEXT(state);
#define Liveness_ENDWORK_LivenessState \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Activate Liveness for a BasicBlock.
 *
 * We put first on the worklist the block in post-order, so they are pop(ed) in
 * post-order. This is a recommended ordering for backward data-flow problems.
 */
void
Liveness_activateState(Liveness this, BasicBlock curr_block, bool phiSets);
#endif//$XCC__h

void
Liveness_activateState(Liveness this, BasicBlock curr_block, bool phiSets)
{
  Temporary_ *dedicated = Liveness_DEDICATED(this);
  LivenessState_ *baseState = Liveness_BASESTATE(this);
  int32_t curr_index = BasicBlock_index(curr_block);
  LivenessState curr_state = baseState + curr_index;
  Except_CHECK(BasicBlock_index(curr_block) >= 0);
  // Forward DFS inside the CodeRegion.
  *LivenessState__BASICBLOCK(curr_state) = curr_block;
  *LivenessState__ISLISTED(curr_state) = true;
  // Local liveness analysis.
  if (dedicated != NULL) {
    TemporarySparse useInSparse = Liveness_SPARSESET(this);
    TemporaryTable temporaryTable = Liveness_TEMPORARYTABLE(this);
    // Collect the PHI arguments of successors in useInSparse.
    TemporarySparse_empty(useInSparse);
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(curr_block, edge) {
      int32_t argIndex = BasicBlockEdge_ARGINDEX(edge);
      BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
      if (BasicBlock_phiCount(succ_block) != 0) {
        BasicBlock_FOREACH_Operation(succ_block, operation) {
          if (Operator_isPhi(Operation_operator(operation))) {
            Temporary argument = Operation_getArgument(operation, argIndex);
            if (Temporary_hasRegister(argument)) {
              argument = dedicated + Temporary_REGISTER(argument);
            }
            if (TemporaryTable_contains(temporaryTable, argument)) {
              TemporarySparse_insert(useInSparse, argument);
            }
          } else break;
        } BasicBlock_ENDEACH_Operation;
      }
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    if (phiSets) {
      // Fill the PHIUSESETS with the PHI arguments of successors.
      TemporarySet phiUseSet = Liveness_PHIUSESETS(this) + curr_index;
      Except_CHECK(TemporarySet_isEmpty(phiUseSet));
      TemporarySparse_FOREACH_Temporary(useInSparse, temporary) {
        TemporarySet_pushMember(phiUseSet, temporary);
      } TemporarySparse_ENDEACH_Temporary;
      TemporarySet_makeOrder(phiUseSet);
    }
    // Local liveness analysis.
    LivenessState_analyzeOperations(curr_state, useInSparse, dedicated);
  }
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(curr_block, edge) {
    BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
    LivenessState succ_state = baseState + BasicBlock_index(succ_block);
    Except_CHECK(BasicBlock_index(succ_block) >= 0);
    if (!LivenessState_ISLISTED(succ_state)) {
      Liveness_activateState(this, succ_block, phiSets);
    }
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // Postorder put first on worklist including ROOTBLOCK.
  Liveness_workPut(this, curr_state);
  //PFA_TRACE && fprintf(PFA_TRACE, "\tPUT Block_%d", BasicBlock_index(curr_block));
}

#ifdef $XCC__h
/**
 * Iterate this Liveness problem until fixpoint.
 *
 * This is the worklist iterative algorithm to solve backward data-flow problems.
 * The activated blocks are pushed on top of the worklist, and the worklist is
 * pop(ed) to get the current block. This enables the data-flow information to
 * stabilize in loops before proceeding further in the flow graph.
 */
void
Liveness_doIterate(Liveness this);
#endif//$XCC__h

void
Liveness_doIterate(Liveness this)
{
  LivenessState_ *baseState = Liveness_BASESTATE(this);
  //PFA_TRACE && fprintf(PFA_TRACE, "Liveness_doIterate\n");
  //PFA_TRACE && Liveness_pretty(this, PFA_TRACE);
  while (!Liveness_workIsEmpty(this)) {
    LivenessState curr_state = Liveness_workHead(this);
    int32_t countLiveOutSet = LivenessState_countLiveOutSet(curr_state);
    BasicBlock curr_block = LivenessState_BASICBLOCK(curr_state);
    // Pop curr_state from the worklist.
    *LivenessState__ISLISTED(curr_state) = false;
    Liveness_workPop(this);
//PFA_TRACE && fprintf(PFA_TRACE, "popped block_%d:\n", BasicBlock_index(curr_block));
//PFA_TRACE && LivenessState_pretty(curr_state, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(curr_block, edge) {
      BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
      LivenessState succ_state = baseState + BasicBlock_index(succ_block);
      Except_CHECK(BasicBlock_index(succ_block) >= 0);
      if (BasicBlock_index(succ_block) == 0) continue;
      // LIVEINSET[succ] = USEINSET[succ] U (LIVEOUTSET[succ] - KILLEDSET[succ]).
      TemporarySet_assign(LivenessState_LIVEINSET(succ_state),
                          LivenessState_LIVEOUTSET(succ_state));
      TemporarySet_diff(LivenessState_LIVEINSET(succ_state),
                        LivenessState_KILLEDSET(succ_state));
      TemporarySet_union(LivenessState_LIVEINSET(succ_state),
                         LivenessState_USEINSET(succ_state));
//PFA_TRACE && fprintf(PFA_TRACE, "\tprocess\tBlock_%d\n", BasicBlock_index(succ_block));
//PFA_TRACE && LivenessState_pretty(succ_state, PFA_TRACE);
      // LIVEOUTSET[curr] U= LIVEINSET[succ], for succ a successor of curr.
      TemporarySet_union(LivenessState_LIVEOUTSET(curr_state),
                         LivenessState_LIVEINSET(succ_state));
      //PFA_TRACE && Liveness_pretty(this, PFA_TRACE);
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    ++*Liveness__ITERCOUNT(this);
//PFA_TRACE && fprintf(PFA_TRACE, "result block_%d:\n", BasicBlock_index(curr_block));
//PFA_TRACE && LivenessState_pretty(curr_state, PFA_TRACE) && fprintf(PFA_TRACE, "\n");
    // In case of change, push the predecessors on the worklist.
    if (LivenessState_countLiveOutSet(curr_state) != countLiveOutSet) {
      BasicBlock_ENTER_FOREACH_BasicBlockEdge(curr_block, edge) {
        BasicBlock pred_block = BasicBlockEdge_origBlock(edge);
        LivenessState pred_state = baseState + BasicBlock_index(pred_block);
        Except_CHECK(BasicBlock_index(pred_block) >= 0);
        if (BasicBlock_index(pred_block) == 0) continue;
        // pred_state must be initialized by Liveness_activateState
        Except_CHECK(LivenessState_BASICBLOCK(pred_state) != NULL);
        if (!LivenessState_ISLISTED(pred_state)) {
          Liveness_workPush(this, pred_state);
          *LivenessState__ISLISTED(pred_state) = true;
          //PFA_TRACE && fprintf(PFA_TRACE, "\tPUSH\tBlock_%d\n",
                               //BasicBlock_index(pred_block));
        }
      } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
    }
  }
}

/**
 * Initialize the live-in and live-out sets as upward exposed uses.
 */
static void
Liveness_doUpward(Liveness this)
{
  LivenessState_ *baseState = Liveness_BASESTATE(this);
  int32_t blockCount = Liveness_blockCount(this), index;
  TemporaryTable temporaryTable = Liveness_TEMPORARYTABLE(this);
  // Fill the base state LIVEINSET and LIVEOUTSET with the Global Temporary(ies).
  TemporarySet liveInSet = LivenessState_LIVEINSET(baseState);
  TemporarySet liveOutSet = LivenessState_LIVEOUTSET(baseState);
  TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
    if (Temporary_isGlobal(temporary)) {
      TemporarySet_pushMember(liveInSet, temporary);
    }
  } TemporaryTable_ENDEACH_Temporary;
  TemporarySet_makeOrder(liveInSet);
  TemporarySet_assign(liveOutSet, liveInSet);
  // Assign the LIVEINSET and LIVEOUTSET of non-base states.
  for (index = 1; index < blockCount; index++) {
    LivenessState state = baseState + index;
    TemporarySet_assign(LivenessState_LIVEINSET(state), liveInSet);
    TemporarySet_assign(LivenessState_LIVEOUTSET(state), liveOutSet);
  }
}

/**
 * Linear pass to compute approximate Liveness.
 *
 * We initialize the live-out and live-out sets with the upward exposed uses then
 * refine the computation with one linear pass in post-order.
 */
static void
Liveness_doLinear(Liveness this)
{
  LivenessState_ *baseState = Liveness_BASESTATE(this);
  Liveness_doUpward(this);
  Liveness_FORWORK_LivenessState(this, curr_state) {
    BasicBlock curr_block = LivenessState_BASICBLOCK(curr_state);
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(curr_block, edge) {
      BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
      LivenessState succ_state = baseState + BasicBlock_index(succ_block);
      Except_CHECK(BasicBlock_index(succ_block) >= 0);
      if (BasicBlock_index(succ_block) == 0) continue;
      // LIVEINSET[succ] = USEINSET[succ] U (LIVEOUTSET[succ] - KILLEDSET[succ]).
      TemporarySet_assign(LivenessState_LIVEINSET(succ_state),
                          LivenessState_LIVEOUTSET(succ_state));
      TemporarySet_diff(LivenessState_LIVEINSET(succ_state),
                        LivenessState_KILLEDSET(succ_state));
      TemporarySet_union(LivenessState_LIVEINSET(succ_state),
                         LivenessState_USEINSET(succ_state));
      // LIVEOUTSET[curr] U= LIVEINSET[succ], for succ a successor of curr.
      TemporarySet_union(LivenessState_LIVEOUTSET(curr_state),
                         LivenessState_LIVEINSET(succ_state));
      ++*Liveness__ITERCOUNT(this);
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  } Liveness_ENDWORK_LivenessState;
}

#ifdef $XCC_h
/**
 * Convert Liveness to multiplex form (Sreedhar Phi effects).
 */
void
Liveness_multiplex(Liveness this);
#endif//$XCC_h

void
Liveness_multiplex(Liveness this)
{
  Memory memory = Liveness_MEMORY(this);
  TemporarySet_ *liveInSets = Liveness_LIVEINSETS(this);
  TemporarySet_ *liveOutSets = Liveness_LIVEOUTSETS(this);
  TemporarySet_ *phiDefSets = Liveness_PHIDEFSETS(this);
  TemporarySet_ *phiUseSets = Liveness_PHIUSESETS(this);
  int32_t blockCount = Liveness_blockCount(this), index;
  Except_REQUIRE(phiDefSets != NULL && phiUseSets != NULL);
  for (index = 0; index < blockCount; index++) {
    TemporarySet_union(liveInSets + index, phiDefSets + index);
    TemporarySet_union(liveOutSets + index, phiUseSets + index);
#ifndef _NDTOR
    TemporarySet_Dtor(phiDefSets + index);
    TemporarySet_Dtor(phiUseSets + index);
#endif//_NDTOR
  }
#ifndef _NDTOR
  Memory_free_(memory, phiDefSets);
  Memory_free_(memory, phiUseSets);
#endif//_NDTOR
  *Liveness__PHIDEFSETS(this) = NULL;
  *Liveness__PHIUSESETS(this) = NULL;
  CodeRegion_setLiveSets(Liveness_CODEREGION(this),
                         Liveness_BLOCKCOUNT(this),
                         Liveness_LIVEINSETS(this),
                         Liveness_LIVEOUTSETS(this),
                         Liveness_PHIDEFSETS(this),
                         Liveness_PHIUSESETS(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Compute the phiDefSets and the phiUseSets.
 */
void
Liveness_computePhiSets(Liveness this);
#endif//$XCC__h

void
Liveness_computePhiSets(Liveness this)
{
  CodeRegion codeRegion = Liveness_CODEREGION(this);
  TemporaryTable temporaryTable = Liveness_TEMPORARYTABLE(this);
  TemporarySet_ *phiDefSets = Liveness_PHIDEFSETS(this);
  TemporarySet_ *phiUseSets = Liveness_PHIUSESETS(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, block_n) {
    int32_t index_n = BasicBlock_index(block_n);
    BasicBlock_FOREACH_Operation(block_n, operation) {
      if (Operator_isPhi(Operation_operator(operation))) {
        int32_t argCount = Operation_argCount(operation), i;
        Temporary *arguments = Operation_arguments(operation);
        Temporary result = Operation_results(operation)[0];
        if (Temporary_hasRegister(result)) {
          Except_CHECK(0);
          //result = dedicated + Temporary_REGISTER(result);
        }
        BasicBlock_ENTER_FOREACH_BasicBlockEdge(block_n, edge) {
          int32_t argIndex = BasicBlockEdge_ARGINDEX(edge);
          BasicBlock block_p = BasicBlockEdge_origBlock(edge);
          int32_t index_p = BasicBlock_index(block_p);
          Temporary argument = arguments[argIndex];
          if (Temporary_hasRegister(argument)) {
            Except_CHECK(0);
            //argument = dedicated + Temporary_REGISTER(result);
          }
          if (TemporaryTable_contains(temporaryTable, argument)) {
            TemporarySet_pushMember(phiUseSets + index_p, argument);
          }
        } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
        if (TemporaryTable_contains(temporaryTable, result)) {
          TemporarySet_pushMember(phiDefSets + index_n, result);
        }
      } else break;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC_h
/*
 * Liveness_orderSets  -- in case livesets are not ordered, order them.
 */
void
Liveness_orderSets(Liveness this, bool phiSets);
#endif//$XCC_h

void
Liveness_orderSets(Liveness this, bool phiSets)
{
  CodeRegion codeRegion = Liveness_CODEREGION(this);
  TemporarySet_ *phiDefSets = Liveness_PHIDEFSETS(this);
  TemporarySet_ *phiUseSets = Liveness_PHIUSESETS(this);
  TemporarySet_ *liveInSets = Liveness_LIVEINSETS(this);
  TemporarySet_ *liveOutSets = Liveness_LIVEOUTSETS(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    int32_t index = BasicBlock_index(basicBlock);
//fprintf(stderr, "livesets: %d %d %d %d =>  ", TemporarySet_count(phiDefSets + index), TemporarySet_count(phiUseSets + index), TemporarySet_count(liveInSets + index), TemporarySet_count(liveOutSets + index));
    if (phiSets) {
      TemporarySet_makeOrder(phiDefSets + index);
      TemporarySet_makeOrder(phiUseSets + index);
    }
    TemporarySet_makeOrder(liveInSets + index);
    TemporarySet_makeOrder(liveOutSets + index);
//fprintf(stderr, "%d %d %d %d\n", TemporarySet_count(phiDefSets + index), TemporarySet_count(phiUseSets + index), TemporarySet_count(liveInSets + index), TemporarySet_count(liveOutSets + index));
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC_h
/*
 * Liveness_doUpAndMark  --
 *     Variable per variable technique. Once killing and consumers sets have been
 *     computed, walk up and mark on paths from uses.
 */
void
Liveness_doUpAndMark(Liveness this);
#endif//$XCC_h

void
Liveness_doUpAndMark(Liveness this)
{
  int32_t blockCount = Liveness_BLOCKCOUNT(this);
  TemporaryTable temporaryTable = Liveness_TEMPORARYTABLE(this);
  Temporary *liveInFlag = memset(alloca(sizeof(Temporary)*blockCount), 0, sizeof(Temporary)*blockCount);
  Temporary *liveOutFlag = memset(alloca(sizeof(Temporary)*blockCount), 0, sizeof(Temporary)*blockCount);
  Temporary *killedFlag = memset(alloca(sizeof(Temporary)*blockCount), 0, sizeof(Temporary)*blockCount);
  BasicBlockStack_ *killingBlocks = Liveness_KILLINGBLOCKS(this);
  BasicBlockStack_ *consumerBlocks = Liveness_CONSUMERBLOCKS(this);
  TemporarySet_ *liveInSets = Liveness_LIVEINSETS(this);
  // foreach T
  TemporaryTable_FOREACH_Temporary(temporaryTable, temporary) {
    // foreach BB in T.killingBlocks
    BasicBlockStack_FOREACH_BasicBlock(killingBlocks + Temporary_INDEX(temporary), basicBlock) {
      int32_t index = BasicBlock_index(basicBlock);
      // BB.killedFlag := T
      killedFlag[index] = temporary;
    } BasicBlockStack_ENDEACH_BasicBlock;
    // foreach BB in T.consumerBlocks
    BasicBlockStack_FOREACH_BasicBlock(consumerBlocks + Temporary_INDEX(temporary), basicBlock) {
      int32_t index = BasicBlock_index(basicBlock);
      // if (BB.liveinFlag <> T)
      if (liveInFlag[index] != temporary) {
        // BB.livein U= T && BB.liveinFlag = T
        TemporarySet_pushMember(liveInSets + index, temporary);
        liveInFlag[index] = temporary;
        // foreach P in CFG_preds(BB) upandmark(P, T)
        BasicBlock_ENTER_FOREACH_BasicBlockEdge(basicBlock, edge) {
          BasicBlock basicBlock_p = BasicBlockEdge_origBlock(edge);
          Liveness_walkUpAndMark(this, basicBlock_p, temporary, liveInFlag, liveOutFlag, killedFlag);
        } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
      }
    } BasicBlockStack_ENDEACH_BasicBlock;
  } TemporaryTable_ENDEACH_Temporary;
}

#ifdef $XCC__h
/*
 * Liveness_walkUpAndMark --
 */
void
Liveness_walkUpAndMark(Liveness this, BasicBlock basicBlock, Temporary temporary,
                       Temporary liveInFlag[], Temporary liveOutFlag[], Temporary killedFlag[]);
#endif//$XCC__h

void
Liveness_walkUpAndMark(Liveness this, BasicBlock basicBlock, Temporary temporary,
                       Temporary liveInFlag[], Temporary liveOutFlag[], Temporary killedFlag[])
{
  int32_t index = BasicBlock_index(basicBlock);
  TemporarySet liveOutSet = Liveness_LIVEOUTSETS(this) + index;
  TemporarySet liveInSet = Liveness_LIVEINSETS(this) + index;
  // if (BB.liveoutFlag == T) return
  if (liveOutFlag[index] == temporary) return;
  // BB.liveout U= T && BB.livoutFlag = T
  TemporarySet_pushMember(liveOutSet, temporary);
  liveOutFlag[index] = temporary;
  // if (BB.killedFlag == T || BB.liveinFlag == T) return
  if ( killedFlag[index] == temporary ||
       liveInFlag[index] == temporary) return;
  // BB.livein U= T
  TemporarySet_pushMember(liveInSet, temporary);
  // foreach P in CFG_preds(BB)
  BasicBlock_ENTER_FOREACH_BasicBlockEdge(basicBlock, edge) {
    BasicBlock basicBlock_p = BasicBlockEdge_origBlock(edge);
    // upandmark(P, T)
    Liveness_walkUpAndMark(this, basicBlock_p, temporary, liveInFlag, liveOutFlag, killedFlag);
  } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
}

#ifdef $XCC_h
/**
 * Make the KILLINGBLOCKS and CONSUMERBLOCKS for this CodeRegion Variable(s).
 *
 * Get the Variable(s) from the TemporaryTable else from the CodeRegion.
 */
void
Liveness_makeKillingConsumerBlocks(Liveness this, bool phiSets);
#endif//$XCC_h

void
Liveness_makeKillingConsumerBlocks(Liveness this, bool phiSets)
{
  Memory memory = Liveness_MEMORY(this);
  CodeRegion codeRegion = Liveness_CODEREGION(this);
  Temporary_ *dedicated = Liveness_DEDICATED(this);
  TemporaryTable temporaryTable = Liveness_TEMPORARYTABLE(this);
  TemporarySet_ *phiDefSets = Liveness_PHIDEFSETS(this);
  TemporarySet_ *phiUseSets = Liveness_PHIUSESETS(this);
  BasicBlockStack_ *killingBlocks = Liveness_KILLINGBLOCKS(this);
  BasicBlockStack_ *consumerBlocks = Liveness_CONSUMERBLOCKS(this);
  BasicBlock *killedFlag = Liveness_KILLEDFLAG(this);
  /* /!\ The order in which operands are treated might be incorrect  */
  /* eg. both a partial def and a def. The def treated before, the partial */
  /*     def is not considered as livein ... /!\ */
  // foreach BB
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    // foreach T in BB.phidefs
    if (phiSets) {
      TemporarySet phiDefSet = phiDefSets + BasicBlock_index(basicBlock);
      TemporarySet_FOREACH_Temporary(phiDefSet, temporary) {
        // T.killedFlag := BB
        // T.killingBlocks U= BB
        if (TemporaryTable_contains(temporaryTable,temporary)) {
          //*Temporary__KILLEDFLAG(temporary) = basicBlock;
          killedFlag[Temporary_INDEX(temporary)] = basicBlock;
          //Temporary_addKillingBlock(temporary, basicBlock);
          BasicBlockStack_push2(killingBlocks + Temporary_INDEX(temporary), basicBlock);
        }
      } TemporarySet_ENDEACH_Temporary;
    }
    if (dedicated != NULL) {
      // foreach non phi operation
      BasicBlock_FOREACH_Operation(basicBlock, operation) {
        RegisterSet clobberSet = Operation_clobberSet(operation);
        if (Operator_isPhi(Operation_operator(operation))) continue;
        // foreach T used
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (Temporary_hasRegister(argument)) {
            argument = dedicated + Temporary_REGISTER(argument);
          }
          if (TemporaryTable_contains(temporaryTable, argument)) {
            // if (T.killedFlag <> BB) T.consumerBlocks U= BB
            //if (Temporary_KILLEDFLAG(argument) != basicBlock)
            if (killedFlag[Temporary_INDEX(argument)] != basicBlock)
              //Temporary_addConsumerBlock(argument, basicBlock);
              BasicBlockStack_push2(consumerBlocks + Temporary_INDEX(argument), basicBlock);
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
        // foreach T defined
        Operation_RESULTS_FOREACH_Temporary(operation, result) {
          if (Temporary_hasRegister(result)) {
            result = dedicated + Temporary_REGISTER(result);
          }
          if (TemporaryTable_contains(temporaryTable, result)) {
            // T.killedFlag := BB
            // T.killingBlocks U= BB
            //*Temporary__KILLEDFLAG(result) = basicBlock;
            killedFlag[Temporary_INDEX(result)] = basicBlock;
            //Temporary_addKillingBlock(result, basicBlock);
            BasicBlockStack_push2(killingBlocks + Temporary_INDEX(result), basicBlock);
          }
        } Operation_RESULTS_ENDEACH_Temporary;
        if (clobberSet != NULL) {
          RegisterSet_FOREACH_Register(clobberSet, registre) {
            Temporary clobbered = dedicated + registre;
            // Clobber treated as a "def" (kill):
            // T.killedFlag := BB
            // T.killingBlocks U= BB
            if (TemporaryTable_contains(temporaryTable, clobbered)) {
              //*Temporary__KILLEDFLAG(clobbered) = basicBlock;
              killedFlag[Temporary_INDEX(clobbered)] = basicBlock;
              //Temporary_addKillingBlock(clobbered, basicBlock);
              BasicBlockStack_push2(killingBlocks + Temporary_INDEX(clobbered), basicBlock);
            }
          } RegisterSet_ENDEACH_Register;
        }
      } BasicBlock_ENDEACH_Operation;
    }
    // foreach T in BB.phiuses
    if (phiSets) {
      TemporarySet phiUseSet = phiUseSets + BasicBlock_index(basicBlock);
      TemporarySet_FOREACH_Temporary(phiUseSet, temporary) {
        // if (T.killedFlag <> BB) T.consumerBlocks U= BB
        if (TemporaryTable_contains(temporaryTable,temporary) &&
            //Temporary_KILLEDFLAG(temporary) != basicBlock)
            killedFlag[Temporary_INDEX(temporary)] != basicBlock)
          //Temporary_addConsumerBlock(temporary, basicBlock);
          BasicBlockStack_push2(consumerBlocks + Temporary_INDEX(temporary), basicBlock);
      } TemporarySet_ENDEACH_Temporary;
    }
  } CodeRegion_ENDEACH_BasicBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Liveness module.
 */
#if defined(Liveness_DATAFLOW)
#define Liveness_INIT()
#else
void
Liveness_INIT(void);
#endif
#endif//$XCC__h

#ifndef Liveness_INIT
void
Liveness_INIT(void)
{
#ifndef Liveness_DATAFLOW
  if (GETENV("Liveness_DATAFLOW")) {
    sscanf(GETENV("Liveness_DATAFLOW"), "%d", &Liveness_DATAFLOW);
    fprintf(stderr, "*** Liveness_DATAFLOW=%d\n", Liveness_DATAFLOW);
  }
}
#endif//Liveness_DATAFLOW
#endif//Liveness_INIT

#ifdef $XCC__h
/**
 * Finalize the Liveness module.
 */
#define Liveness_FINI()
#endif//$XCC__h

#if XCC__C
static void
Liveness_TEST(void)
{
#include "PFA/Liveness_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(Liveness_TEST);
  return 0;
}
#endif

