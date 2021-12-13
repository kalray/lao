#ifdef $XCC_h
/*
 * !!!!	PostPass.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 *
 * Copyright 2002 - 2007 STMicroelectronics.
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
 * @ingroup CGO
 * @brief Scoreboard scheduler.
 */
#endif//$XCC_h


#include "CGO/CGO_.h"

#ifdef $XCC__h
#include "CGO/PostPass.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * PostPass state for a BasicBlock.
 */
struct PostPassState_ {
  //@args	Memory memory, Processor processor, int windowSize
  BasicBlock BASICBLOCK;
  //@access FREQUENCY	BasicBlock_frequency(PostPassState_BASICBLOCK(this))
  bool ISLISTED;		// This PostPassState is in PostPass worklist.
  Processor PROCESSOR;
  struct PostPassState_ *WORKNEXT;
  uint32_t STAMP;		// Time stamp for this PostPassState.
  Scoreboard_ SCOREBOARD[1];	// Scoreboard at the end of BASICBLOCK.
  //@access MEMORY	Scoreboard_memory(PostPassState_SCOREBOARD(this))
};
#endif//$XCC__h

PostPassState
PostPassState_Ctor(PostPassState this,
                       Memory memory, Processor processor, int windowSize)
{
  *PostPassState__BASICBLOCK(this) = NULL;
  *PostPassState__ISLISTED(this) = false;
  *PostPassState__PROCESSOR(this) = processor;
  *PostPassState__WORKNEXT(this) = NULL;
  *PostPassState__STAMP(this) = 0;
  Scoreboard_Ctor(PostPassState_SCOREBOARD(this),
                  memory, processor, windowSize);
  return this;
}

void
PostPassState_Dtor(PostPassState this)
{
  Memory memory = PostPassState_MEMORY(this);
  Scoreboard_Dtor(PostPassState_SCOREBOARD(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(PostPassState_)\t%zu\n", sizeof(PostPassState_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Local scheduling for this PostPassState.
 */
bool
PostPassState_schedule(PostPassState this, Scoreboard temp_scoreboard);
#endif//$XCC__h

bool
PostPassState_schedule(PostPassState this, Scoreboard temp_scoreboard)
{
  BasicBlock basicBlock = PostPassState_BASICBLOCK(this);
  Scoreboard scoreboard = PostPassState_SCOREBOARD(this);
  NativeBlock nativeBlock = BasicBlock_NATIVEBLOCK(basicBlock);
  // Propagate temp_scoreboard along the basicBlock.
if (   FORCE_ONLYCOUNT <= 0
    || FORCE_ONLYCOUNT == InstructionStack_count(NativeBlock_instructions(nativeBlock))) {
  bool modified = NativeBlock_scoreboardInstructions(nativeBlock, temp_scoreboard);
  int lastDate = NativeBlock_lastDate(nativeBlock);
  int padding = BasicBlock_padding(basicBlock);
  //CGO_TRACE && fprintf(CGO_TRACE, "\tBEFORE SORT\n");
  //CGO_TRACE && BasicBlock_pretty(basicBlock, CGO_TRACE);
  NativeBlock_reorderInstructions(nativeBlock);
  //CGO_TRACE && fprintf(CGO_TRACE, "\tAFTER SORT\n");
  //CGO_TRACE && BasicBlock_pretty(basicBlock, CGO_TRACE);
  Scoreboard_advance(temp_scoreboard, lastDate + padding);
  // Assign temp_scoreboard to scoreboard.
  // FIXME! Should compute modified here.
  Scoreboard_reset(scoreboard, 0);
  Scoreboard_merge(scoreboard, temp_scoreboard, 0);
  if (FORCE_PIPELINE > 0) modified = false;
  return modified;
} else return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Scoreboard scheduling in a CodeRegion.
 *
 * Scheduling is done by Scoreboard(ing) windowSize operation(s) along all paths until
 * fixed-point dates. This is similar to a forward data-flow fixed-point computation.
 */
struct PostPass_;
#endif//$XCC_h

#ifdef $XCC__h
struct PostPass_ {
  //@args	Memory memory, CodeRegion codeRegion
  Memory MEMORY;
  CodeRegion CODEREGION;
  Scoreboard_ SCOREBOARD[1];		// Used to merge entering PostPassState.
  uint32_t STAMP;
  int32_t ITERCOUNT;
  int32_t BLOCKCOUNT;
  PostPassState_ *BASESTATE;	// Base of PostPassState array.
  PostPassState_ *WORKTAIL;		// First PostPassState in worklist.
  PostPassState_ *WORKHEAD;		// Last PostPassState in worklist.
};
#endif//$XCC__h

PostPass
PostPass_Ctor(PostPass this,
                  Memory memory, CodeRegion codeRegion)
{
  Optimize optimize = CodeRegion_optimize(codeRegion);
  int scoreboarding = Optimize_SCOREBOARDING(optimize);
  int windowSize = Optimize_WINDOWSIZE(optimize);
  Processor processor = CodeRegion_processor(codeRegion);
  *PostPass__MEMORY(this) = memory;
  *PostPass__CODEREGION(this) = codeRegion;
  Scoreboard_Ctor(PostPass_SCOREBOARD(this),
                  memory, processor, windowSize);
  *PostPass__STAMP(this) = 0;
  *PostPass__ITERCOUNT(this) = 0;
  *PostPass__BLOCKCOUNT(this) = 0;
  *PostPass__BASESTATE(this) = NULL;
  *PostPass__WORKTAIL(this) = NULL;
  *PostPass__WORKHEAD(this) = NULL;
  if (scoreboarding != 0) {
    int32_t blockCount = CodeRegion_blockCount(codeRegion), i;
    size_t totalSize = blockCount*sizeof(PostPassState_);
    PostPassState_ *baseState = Memory_alloc(memory, totalSize);
    for (i = 0; i < blockCount; i++) {
      PostPassState_Ctor(baseState + i, memory, processor, windowSize);
    }
    *PostPass__BLOCKCOUNT(this) = blockCount;
    *PostPass__BASESTATE(this) = baseState;
    Except_REQUIRE(windowSize >= 0);
    CGO_TRACE && fprintf(CGO_TRACE, "*** Scoreboarder(%d,%d)\n",
                         scoreboarding, windowSize);
    PostPass_activateState(this, CodeRegion_rootBlock(codeRegion));
    CGO_TRACE && fprintf(CGO_TRACE, "\n");
    if (scoreboarding & OptimizeScoreboarding_Iterate) PostPass_doIterate(this);
    else if (scoreboarding & OptimizeScoreboarding_Fixup) PostPass_doFixup(this);
    else Except_NEVER(true);
    PostPass_writeBack(this);
  }
  return this;
}

void
PostPass_Dtor(PostPass this)
{
  Memory memory = PostPass_MEMORY(this);
  int32_t blockCount = PostPass_BLOCKCOUNT(this), i;
  PostPassState_ *baseState = PostPass_BASESTATE(this);
  if (baseState != NULL) {
    for (i = 0; i < blockCount; i++) PostPassState_Dtor(baseState + i);
    Memory_free_(memory, baseState);
  }
  Scoreboard_Dtor(PostPass_SCOREBOARD(this));
}

size_t
PostPass_Size(Memory memory, CodeRegion codeRegion)
{
        return sizeof(PostPass_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(PostPass_)\t%zu\n", sizeof(PostPass_));
}
#endif//$XCC__c

#ifdef $XCC_h
PostPass
PostPass_new(Memory parent, CodeRegion codeRegion);
#endif//$XCC_h

PostPass
PostPass_new(Memory parent, CodeRegion codeRegion)
{
  Memory memory = Memory_new(parent, true);
  size_t size = PostPass_Size(memory, codeRegion);
  PostPass this = Memory_alloc(memory, size);
  return PostPass_Ctor(this, memory, codeRegion);
}

#ifdef $XCC_h
PostPass
PostPass_delete(PostPass this);
#endif//$XCC_h

PostPass
PostPass_delete(PostPass this)
{
  if (this != NULL) {
    Memory memory = PostPass_MEMORY(this);
#ifndef _NDTOR
    PostPass_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * For use in PostPass_FOREACH.
 */
static inline int32_t
PostPass_blockCount(PostPass this)
{
  return PostPass_BLOCKCOUNT(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * For use in PostPass_FOREACH.
 */
static inline PostPassState_ *
PostPass_baseState(PostPass this)
{
  return PostPass_BASESTATE(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this PostPass PostPassState.
 */
#define PostPass_FOREACH_PostPassState(this, state) { \
  int32_t PostPass_BLOCKCOUNT = PostPass_blockCount(this); \
  PostPassState PostPass_BASESTATE = PostPass_baseState(this), state; \
  PostPassState PostPass_PASTSTATE = PostPass_BASESTATE + PostPass_BLOCKCOUNT; \
  for (state = PostPass_BASESTATE; state < PostPass_PASTSTATE; ++state) { \
    if (PostPassState_BASICBLOCK(state) != NULL) {
#define PostPass_ENDEACH_PostPassState \
    } \
  } \
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

static inline PostPassState
PostPass_workTail(PostPass this)
{
  return PostPass_WORKTAIL(this);
}

static inline PostPassState
PostPass_workHead(PostPass this)
{
  return PostPass_WORKHEAD(this);
}

static inline bool
PostPass_workIsEmpty(PostPass this)
{
  return PostPass_WORKTAIL(this) == NULL;
}

#ifndef PostPass_WORKINSERT
// Push state at head of worklist.
static inline void
PostPass_workPush(PostPass this, PostPassState state)
{
  PostPassState workHead = PostPass_WORKHEAD(this);
  Except_CHECK(PostPassState_WORKNEXT(state) == NULL);
  if (workHead == NULL) {
    *PostPass__WORKTAIL(this) = state;
  } else {
    *PostPassState__WORKNEXT(state) = workHead;
  }
  *PostPass__WORKHEAD(this) = state;
}
#else//PostPass_WORKINSERT
#define PostPass_workPush PostPass_workInsert
#endif//PostPass_WORKINSERT

// Insert state in FREQUENCY order.
static void
PostPass_workInsert(PostPass this, PostPassState state);

static void
PostPass_workInsert(PostPass this, PostPassState state)
{
  PostPassState workHead = PostPass_WORKHEAD(this);
  Except_CHECK(PostPassState_WORKNEXT(state) == NULL);
  if (workHead == NULL) {
    *PostPass__WORKTAIL(this) = state;
    *PostPass__WORKHEAD(this) = state;
  } else {
    PostPassState curr_state = workHead, next_state = NULL;
    do {
      if (PostPassState_FREQUENCY(state) >=
          PostPassState_FREQUENCY(curr_state)) {
        *PostPassState__WORKNEXT(state) = curr_state;
        if (next_state != NULL) {
          *PostPassState__WORKNEXT(next_state) = state;
        } else {
          *PostPass__WORKHEAD(this) = state;
        }
        return;
      }
      next_state = curr_state;
      curr_state = PostPassState_WORKNEXT(curr_state);
    } while (curr_state != NULL);
    *PostPass__WORKTAIL(this) = state;
    *PostPassState__WORKNEXT(next_state) = state;
  }
}

// Put state at tail of worklist.
static inline void
PostPass_workPut(PostPass this, PostPassState state)
{
  PostPassState workTail = PostPass_WORKTAIL(this);
  Except_CHECK(PostPassState_WORKNEXT(state) == NULL);
  *PostPass__WORKTAIL(this) = state;
  if (workTail == NULL) {
    *PostPass__WORKHEAD(this) = state;
  } else {
    *PostPassState__WORKNEXT(workTail) = state;
  }
}

// Pop state at head of worklist.
static inline void
PostPass_workPop(PostPass this)
{
  PostPassState workTail = PostPass_WORKTAIL(this);
  PostPassState workHead = PostPass_WORKHEAD(this);
  if (workTail == workHead) {
    *PostPass__WORKTAIL(this) = *PostPass__WORKHEAD(this) = NULL;
  } else {
    *PostPass__WORKHEAD(this) = PostPassState_WORKNEXT(workHead);
  }
  *PostPassState__WORKNEXT(workHead) = NULL;
}

#ifdef $XCC__h
#define PostPass_FORWORK_PostPassState(this, state) { \
  PostPassState PostPass_WORKNEXT = NULL; \
  PostPassState state = PostPass_WORKHEAD(this); \
  for (; state != NULL; state = PostPass_WORKNEXT) { \
    PostPass_WORKNEXT = PostPassState_WORKNEXT(state);
#define PostPass_ENDWORK_PostPassState \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Activate PostPassState for a BasicBlock.
 *
 * We push last on the worklist the blocks in post-order, so they are pop(ed) in
 * reverse post-order. This is a recommended ordering for forward data-flow problems.
 */
void
PostPass_activateState(PostPass this, BasicBlock curr_block);
#endif//$XCC__h

void
PostPass_activateState(PostPass this, BasicBlock curr_block)
{
  int32_t index = 1;
  Memory memory = PostPass_MEMORY(this);
  Scoreboard this_scoreboard = PostPass_SCOREBOARD(this);
  PostPassState_ *baseState = PostPass_BASESTATE(this);
  PostPassState curr_state = baseState + BasicBlock_index(curr_block);
  Operation last_operation = BasicBlock_lastOperation(curr_block);
  int minTaken = Processor_minTaken(BasicBlock_processor(curr_block));
  Except_CHECK(BasicBlock_index(curr_block) >= 0);
  // Forward DFS on CodeRegion.
  *PostPassState__BASICBLOCK(curr_state) = curr_block;
  *PostPassState__ISLISTED(curr_state) = true;
  BasicBlock_FOREACH_Operation(curr_block, operation) {
    Instruction instruction = Operation_instruction(operation);
    // FIXME: uncomment this and comment in Operation_makeInstruction.
    //Opcode opcode = Instruction_OPCODE(instruction);
    //OperandsBuffer_ buffer_, *buffer = &buffer_;
    //Operands operands = Opcode_operands(Instruction_OPCODE(instruction));
    //InstructionAction_ *actions = Memory_alloc(memory, Instruction_sizeActions(instruction));
    //(*Operands_decode(operands))(Instruction_CODEWORDS(instruction), buffer);
    //Instruction_makeActions(instruction, OperandsBuffer_DECODED(buffer), actions);
    Instruction_setStartDate(instruction, -1);
    Operation_setIndex(operation, index++);
  } BasicBlock_ENDEACH_Operation;
  if (BasicBlock_enterCount(curr_block) > 1) {
    float curr_frequency = BasicBlock_frequency(curr_block);
    BasicBlock_ENTER_FOREACH_BasicBlockEdge(curr_block, edge) {
      BasicBlock pred_block = BasicBlockEdge_origBlock(edge);
      float pred_frequency = BasicBlock_frequency(pred_block);
      if (pred_frequency*3 < curr_frequency) {
        BasicBlock_setPadding(pred_block, 2);
      }
    } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
  }
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(curr_block, edge) {
    int elapsed = minTaken + 1;
    BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
    PostPassState succ_state = baseState + BasicBlock_index(succ_block);
    Except_CHECK(BasicBlock_index(succ_block) >= 0);
    if (!PostPassState_ISLISTED(succ_state)) {
      PostPass_activateState(this, succ_block);
    }
    // Update the BasicBlockEdge fields.
    if (BasicBlockEdge_destBlock(edge) == BasicBlock_next(curr_block)) {
      // Fall-through edge.
      if (last_operation == NULL) {
        elapsed = 0;
      } else if (Operation_isRoutine(last_operation)) {
        elapsed = 2*minTaken + 1;
      } else {
        elapsed = 1;
      }
    }
    *BasicBlockEdge__ELAPSED(edge) = elapsed;
    Except_CHECK(BasicBlockEdge_ELAPSED(edge) == elapsed);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // Postorder push last on worklist excluding ROOTBLOCK.
  if (BasicBlock_index(curr_block) != 0) {
    PostPass_workPush(this, curr_state);
    CGO_TRACE && fprintf(CGO_TRACE, "\tPUSH Block_%d", BasicBlock_index(curr_block));
  } else {
    *PostPassState__STAMP(curr_state) = ++*PostPass__STAMP(this);
  }
}

#ifdef $XCC__h
/**
 * Iterate this PostPass problem until fixpoint.
 *
 * This is the worklist iterative algorithm to solve forward data-flow problems.
 */
void
PostPass_doIterate(PostPass this);
#endif//$XCC__h

void
PostPass_doIterate(PostPass this)
{
  PostPassState_ *baseState = PostPass_BASESTATE(this);
  Scoreboard this_scoreboard = PostPass_SCOREBOARD(this);
  while (!PostPass_workIsEmpty(this)) {
    int assigned = 0, modified = 0;
    PostPassState curr_state = PostPass_workHead(this);
    BasicBlock curr_block = PostPassState_BASICBLOCK(curr_state);
    uint32_t curr_stamp = PostPassState_STAMP(curr_state);
    // Pop curr_state from the worklist.
    *PostPassState__ISLISTED(curr_state) = false;
    PostPass_workPop(this);
    CGO_TRACE && fprintf(CGO_TRACE, "\tPOP\tBlock_%d\tfrequency=%g\n",
                         BasicBlock_index(curr_block),
                         BasicBlock_frequency(curr_block));
    // Merge the incoming Scoreboard(s) after elapsed cycles.
    BasicBlock_ENTER_FOREACH_BasicBlockEdge(curr_block, edge) {
      int elapsed = BasicBlockEdge_ELAPSED(edge);
      BasicBlock pred_block = BasicBlockEdge_origBlock(edge);
      PostPassState pred_state = baseState + BasicBlock_index(pred_block);
      Scoreboard pred_scoreboard = PostPassState_SCOREBOARD(pred_state);
      uint32_t pred_stamp = PostPassState_STAMP(pred_state);
      Except_CHECK(BasicBlock_index(pred_block) >= 0);
      // pred_state must be initialized by PostPass_activateState
      Except_CHECK(PostPassState_BASICBLOCK(pred_state) != NULL);
      if (pred_stamp >= curr_stamp) {
        CGO_TRACE && fprintf(CGO_TRACE, "\tMERGE\tBlock_%d\telapsed=%d\n",
                             BasicBlock_index(pred_block), elapsed);
        //CGO_TRACE && Scoreboard_pretty(pred_scoreboard, CGO_TRACE);
        if (!assigned++) {
          Scoreboard_reset(this_scoreboard, 0);
          Scoreboard_merge(this_scoreboard, pred_scoreboard, elapsed);
        } else {
          Scoreboard_merge(this_scoreboard, pred_scoreboard, elapsed);
        }
      }
      Except_CHECK(elapsed >= 0);
    } BasicBlock_ENTER_ENDEACH_BasicBlockEdge;
    Except_CHECK(assigned != 0);
    ++*PostPass__ITERCOUNT(this);
    // Schedule the current ScoreboardState.
    CGO_TRACE && fprintf(CGO_TRACE, "\tSTART\tBlock_%d\n", BasicBlock_index(curr_block));
    //CGO_TRACE && Scoreboard_pretty(this_scoreboard, CGO_TRACE);
    modified = PostPassState_schedule(curr_state, this_scoreboard);
    // If changes, push successors on worklist.
    if (modified) {
      *PostPassState__STAMP(curr_state) = ++*PostPass__STAMP(this);
      BasicBlock_LEAVE_FOREACH_BasicBlockEdge(curr_block, edge) {
        BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
        PostPassState succ_state = baseState + BasicBlock_index(succ_block);
        Except_CHECK(BasicBlock_index(succ_block) >= 0);
        if (BasicBlock_index(succ_block) == 0) continue;
        if (!PostPassState_ISLISTED(succ_state)) {
          PostPass_workPush(this, succ_state);
          *PostPassState__ISLISTED(succ_state) = true;
          CGO_TRACE && fprintf(CGO_TRACE, "\tPUSH\tBlock_%d\tfrequency=%g\n",
                               BasicBlock_index(succ_block),
                               BasicBlock_frequency(succ_block));
        }
      } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    }
  }
}

#ifdef $XCC__h
/**
 * Fixup pending latencies at BasicBlock boundaries.
 */
void
PostPass_doFixup(PostPass this);
#endif//$XCC__h

void
PostPass_doFixup(PostPass this)
{
  Except_NEVER(true);	// TODO.
}

#ifdef $XCC__h
/**
 * Write-back results into the CodeRegion.
 */
void
PostPass_writeBack(PostPass this);
#endif//$XCC__h

void
PostPass_writeBack(PostPass this)
{
  intptr_t prev_traceId = -1;
  int firstDate = 0, lastDate = -1;
  CodeRegion codeRegion = PostPass_CODEREGION(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    Selector selector = BasicBlock_selector(basicBlock);
    NativeBlock nativeBlock = BasicBlock_nativeBlock(basicBlock);
    intptr_t traceId = BasicBlock_traceId(basicBlock);
    if (traceId == 0 || traceId != prev_traceId) {
      firstDate = 0, lastDate = -1;
    } else {
      // Assume fall-through in trace.
      firstDate = lastDate + 1;
    }
    BasicBlock_setFlags(basicBlock, BasicBlockFlag_Scheduled);
    NativeBlock_FOREACH_Instruction(nativeBlock, instruction) {
      Operation operation = (Operation)Instruction_INDEXED(instruction);
      Selector_moveOperationLast(selector, operation);
      lastDate = firstDate + Instruction_startDate(instruction);
      Operation_setStartDate(operation, lastDate);
      Operation_setIndex(operation, -1);
    } NativeBlock_ENDEACH_Instruction;
    if (traceId != prev_traceId) {
      Selector selector = BasicBlock_selector(basicBlock);
      Operation first_operation = BasicBlock_firstOperation(basicBlock);
      if (first_operation != NULL) {
        Instruction first_instruction = Operation_instruction(first_operation);
        int first_date = Instruction_startDate(first_instruction), date;
        for (date = first_date - 1; date >= 0; date--) {
          Operation skip_operation = Selector_makeOperationSKIP(selector);
          Selector_moveOperationFirst(selector, skip_operation);
          Operation_setStartDate(skip_operation, date);
        }
      }
    }
    prev_traceId = traceId;
  } CodeRegion_ENDEACH_BasicBlock;
  CGO_TRACE && fprintf(CGO_TRACE, "*** Scoreboarder\tBLOCKCOUNT=%d\tITERCOUNT=%d\n",
                       PostPass_BLOCKCOUNT(this), PostPass_ITERCOUNT(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the PostPass module.
 */
#define PostPass_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the PostPass module.
 */
#define PostPass_FINI()
#endif//$XCC__h

#if XCC__C
static void
PostPass_TEST(void)
{
#include "CGO/PostPass_.c"
}

int
main(int argc, char **argv)
{
  CGO_Test(PostPass_TEST);
  return 0;
}
#endif

