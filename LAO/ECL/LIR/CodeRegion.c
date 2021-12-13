#ifdef $XCC_h
/*
 * !!!!	CodeRegion.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray).
 * Duco van Amstel (duco.van-amstel@kalray.eu)
 *
 * Copyright 2010 - 2013 Kalray
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
 * @ingroup LIR
 * @brief Code Region is the unit of optimization.
 *
 * Procedures are comprised of one or more CodeRegions.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#if _OPTIMIZE
//#define CodeRegion_BRIGGSGLOBALS 1
#endif//_OPTIMIZE
#ifndef CodeRegion_BRIGGSGLOBALS
extern int CodeRegion_BRIGGSGLOBALS;
#endif//CodeRegion_BRIGGSGLOBALS
#endif//$XCC__h

#ifndef CodeRegion_BRIGGSGLOBALS
int CodeRegion_BRIGGSGLOBALS = 0;	//!<FIXME! Should work with CodeRegion_BRIGGSGLOBALS = 1.
#endif//CodeRegion_BRIGGSGLOBALS

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Code region inside a Procedure.
 *
 * A CodeRegion provides a scope for partitioning the compilation of a Procedure.
 * The key requirement of a CodeRegion is that all its BasicBlock(s) have the same Processor.
 * The control-flow of a CodeRegion is converted into a single-entry, single-exit region
 * by introducing a root BasicBlock, whose INDEX is always 0. All the BasicBlock(s) of a
 * CodeRegion are densely indexed thanks to the CodeRegion BLOCKTABLE.
 */
struct CodeRegion_;
#endif//$XCC_h

#ifdef $XCC__h
struct CodeRegion_ {
  //@args	Memory memory, Procedure procedure,
  //@args	intptr_t regionId, Optimize optimize, Configure configure
  IDListItemCell_ ITEMCELL[1];		//!< Dummy IDListItemCell_ for the ROOTBLOCK-.
  BasicBlock_ ROOTBLOCK[1];		//!< Dummy block (INDEX = 0) of this CodeRegion.
  //@access MEMORY	BasicBlock_MEMORY(CodeRegion_ROOTBLOCK(this))
  //@access PROCEDURE	BasicBlock_PROCEDURE(CodeRegion_ROOTBLOCK(this))
  //@access PROCESSOR	BasicBlock_PROCESSOR(CodeRegion_ROOTBLOCK(this))
  //@access CONVENTION	BasicBlock_CONVENTION(CodeRegion_ROOTBLOCK(this))
  //@access SELECTOR	BasicBlock_SELECTOR(CodeRegion_ROOTBLOCK(this))
  //@access LEAVEFIRST	BasicBlock_ENTERFIRST(CodeRegion_ROOTBLOCK(this))
  BasicBlockTable_ BLOCKTABLE[1];	//!< IndexedTable of BasicBlock(s).
  //@access BLOCKTABLEBASE	BasicBlockTable_base(CodeRegion_BLOCKTABLE(this))
  //@access BLOCKCOUNT	BasicBlockTable_count(CodeRegion_BLOCKTABLE(this))
  BasicBlock FIRSTBLOCK;		//!< This CodeRegion first BasicBlock.
  BasicBlock PASTBLOCK;			//!< This CodeRegion past BasicBlock.
  TemporaryTable_ GLOBALTABLE[1];	//!< Table of global Temporary(ies).
  IBList_ VARIABLES[1];			//!< Container for Variable(s) of this CodeRegion.
  Memory CONSUMEM;			//!< Memory for Variable CONSUMEMRS.
  int32_t LIVECOUNT;			//!< BLOCKCOUNT for LIVEINSETS and LIVEOUTSETS;
  TemporarySet_ *LIVEINSETS;		//!< The live-in sets of the blocks of this CodeRegion.
  TemporarySet_ *LIVEOUTSETS;		//!< The live-out sets of the blocks this CodeRegion.
  TemporarySet_ *PHIDEFSETS;		//!< The phi-in sets of this CodeRegion.
  TemporarySet_ *PHIUSESETS;		//!< The phi-out sets of this CodeRegion.
  intptr_t REGIONID;			//!< This CodeRegion ID.
  struct SSAForm_ *SSAFORM;		//!< This CodeRegion SSA Form.
  struct RegionFlow_ *REGIONFLOW;	//!< RegionFlow of this CodeRegion (LAO legacy).
  intptr_t TRACECOUNTER;		//!< TraceId counter (LAO legacy).
  Optimize OPTIMIZE;			//!< This CodeRegion Optimize.
  Configure_ CONFIGURE[1];		//!< This CodeRegion Configure.
};
#endif//$XCC__h

CodeRegion
CodeRegion_Ctor(CodeRegion this,
                Memory memory, Procedure procedure,
                intptr_t regionId, Optimize optimize, Configure configure)
{
  IDListItemCell_Ctor(CodeRegion_ITEMCELL(this), sizeof(BasicBlock_));
  BasicBlock_Ctor(CodeRegion_ROOTBLOCK(this), memory, procedure, this, 0.0);
  BasicBlockTable_Ctor(CodeRegion_BLOCKTABLE(this), memory, 256);
  // Ensure the ROOTBLOCK exists at index 0 in BLOCKTABLE.
  BasicBlockTable_insert(CodeRegion_BLOCKTABLE(this), CodeRegion_ROOTBLOCK(this));
  *CodeRegion__FIRSTBLOCK(this) = NULL;
  *CodeRegion__PASTBLOCK(this) = NULL;
  TemporaryTable_Ctor(CodeRegion_GLOBALTABLE(this), memory, 1024);
  IBList_Ctor(CodeRegion_VARIABLES(this), memory, sizeof(Variable_), 30);
  *CodeRegion__CONSUMEM(this) = NULL;
  *CodeRegion__LIVECOUNT(this) = 0;
  *CodeRegion__LIVEINSETS(this) = NULL;
  *CodeRegion__LIVEOUTSETS(this) = NULL;
  *CodeRegion__PHIDEFSETS(this) = NULL;
  *CodeRegion__PHIUSESETS(this) = NULL;
  *CodeRegion__REGIONID(this) = regionId;
  *CodeRegion__SSAFORM(this) = NULL;
  *CodeRegion__REGIONFLOW(this) = NULL;
  *CodeRegion__TRACECOUNTER(this) = 0;
  *CodeRegion__OPTIMIZE(this) = optimize;
  Configure_Copy(CodeRegion_CONFIGURE(this), configure);
  return this;
}

void
CodeRegion_Dtor(CodeRegion this)
{
  Memory memory = CodeRegion_MEMORY(this);
  BasicBlock_Dtor(CodeRegion_ROOTBLOCK(this));
  BasicBlockTable_Dtor(CodeRegion_BLOCKTABLE(this));
  TemporaryTable_Dtor(CodeRegion_GLOBALTABLE(this));
  IBList_Dtor(CodeRegion_VARIABLES(this));
  if (!FORCE_POOLED && CodeRegion_CONSUMEM(this)) {
    fprintf(stderr, "*** Variable Consumers:\ttotalAlloc=%zu\n",
                    Memory_TOTALSIZE(CodeRegion_CONSUMEM(this)));
  }
  Memory_delete(CodeRegion_CONSUMEM(this));
  Except_REQUIRE(CodeRegion_REGIONFLOW(this) == NULL);
  Configure_Dtor(CodeRegion_CONFIGURE(this));
}

size_t
CodeRegion_Size(Memory memory, Procedure procedure,
                intptr_t regionId, Optimize optimize, Configure configure)
{
  return sizeof(CodeRegion_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(CodeRegion_)\t%zu\n", sizeof(CodeRegion_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * True iff this CodeRegion is empty.
 */
static inline bool
CodeRegion_isEmpty(const_CodeRegion this)
{
  return CodeRegion_FIRSTBLOCK(this) == NULL;
}
#endif//$XCC__h

#ifdef $XCC_h
/*
 * CodeRegion_memory --        	This CodeRegion Procedure.
 */
Memory
CodeRegion_memory(CodeRegion this);
#endif//$XCC_h

Memory
CodeRegion_memory(CodeRegion this)
{
  return CodeRegion_MEMORY(this);
}


#ifdef $XCC_h
/**
 * This CodeRegion Procedure.
 */
struct Procedure_ *
CodeRegion_procedure(CodeRegion this);
#endif//$XCC_h

struct Procedure_ *
CodeRegion_procedure(CodeRegion this)
{
  return CodeRegion_PROCEDURE(this);
}

#ifdef $XCC_h
/**
 * This CodeRegion Selector.
 */
Selector
CodeRegion_selector(CodeRegion this);
#endif//$XCC_h

Selector
CodeRegion_selector(CodeRegion this)
{
  return CodeRegion_SELECTOR(this);
}

#ifdef $XCC_h
/**
 * This CodeRegion Convention.
 */
Convention
CodeRegion_convention(CodeRegion this);
#endif//$XCC_h

Convention
CodeRegion_convention(CodeRegion this)
{
  return CodeRegion_CONVENTION(this);
}

#ifdef $XCC_h
/**
 * This CodeRegion Processor.
 */
Processor
CodeRegion_processor(CodeRegion this);
#endif//$XCC_h

Processor
CodeRegion_processor(CodeRegion this)
{
  return CodeRegion_PROCESSOR(this);
}

#ifdef $XCC_h
/**
 * Root BasicBlock used to attach BasicBlock(s) to this CodeRegion.
 *
 * The root BasicBlock is not linked to others BasicBlock(s) by BasicBlock_next and BasicBlock_prev.
 * The root BasicBlock always has zero as BasicBlock INDEX.
 */
BasicBlock
CodeRegion_rootBlock(CodeRegion this);
#endif//$XCC_h

BasicBlock
CodeRegion_rootBlock(CodeRegion this)
{
  BasicBlock rootBlock = CodeRegion_ROOTBLOCK(this);
  Except_CHECK(BasicBlock_INDEX(rootBlock) == 0);
  return rootBlock;
}

#ifdef $XCC_h
/**
 * Enumerate the entering blocks of this CodeRegion.
 */
#define CodeRegion_FORENTER_BasicBlock(this, basicBlock) { \
  BasicBlock CodeRegion_ROOTBLOCK = CodeRegion_rootBlock(this); \
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(CodeRegion_ROOTBLOCK, CodeRegion_SUCCESSOR) { \
    BasicBlock basicBlock = BasicBlockEdge_destBlock(CodeRegion_SUCCESSOR);
#define CodeRegion_ENDENTER_BasicBlock \
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge; \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by CodeRegion_FORLEAVE_BasicBlock.
 */
BasicBlockEdge
CodeRegion_leaveFirst(CodeRegion this);
#endif//$XCC_h

BasicBlockEdge
CodeRegion_leaveFirst(CodeRegion this)
{
  return CodeRegion_LEAVEFIRST(this);
}

#ifdef $XCC_h
/**
 * Enumerate the leaving blocks of this CodeRegion.
 */
#define CodeRegion_FORLEAVE_BasicBlock(this, basicBlock) { \
  BasicBlockEdge CodeRegion_LEAVENEXT = NULL; \
  BasicBlockEdge CodeRegion_LEAVEEDGE = CodeRegion_leaveFirst(this); \
  for (; CodeRegion_LEAVEEDGE != NULL; CodeRegion_LEAVEEDGE = CodeRegion_LEAVENEXT) { \
    BasicBlock basicBlock = BasicBlockEdge_ORIGBLOCK(CodeRegion_LEAVEEDGE); \
    CodeRegion_LEAVENEXT = BasicBlockEdge_ENTERNEXT(CodeRegion_LEAVEEDGE);
#define CodeRegion_ENDLEAVE_BasicBlock \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The identifier of this CodeRegion.
 */
intptr_t
CodeRegion_regionId(const_CodeRegion this);
#endif//$XCC_h

intptr_t
CodeRegion_regionId(const_CodeRegion this)
{
  return CodeRegion_REGIONID(this);
}

#ifdef $XCC_h
/**
 * Base of BasicBlock(s) mapping array.
 */
const BasicBlock *
CodeRegion_blockTableBase(const_CodeRegion this);
#endif//$XCC_h

const BasicBlock *
CodeRegion_blockTableBase(const_CodeRegion this)
{
  return CodeRegion_BLOCKTABLEBASE(this);
}

#ifdef $XCC_h
/**
 * Number of BasicBlock(s) in this CodeRegion.
 *
 * The count includes the CodeRegion_rootBlock.
 */
int32_t
CodeRegion_blockCount(const_CodeRegion this);
#endif//$XCC_h

int32_t
CodeRegion_blockCount(const_CodeRegion this)
{
  return CodeRegion_BLOCKCOUNT(this);
}

#ifdef $XCC_h
/**
 * For use by CodeRegion_FOREACH_BasicBlock.
 */
BasicBlock
CodeRegion_firstBlock(const_CodeRegion this);
#endif//$XCC_h

BasicBlock
CodeRegion_firstBlock(const_CodeRegion this)
{
  return CodeRegion_FIRSTBLOCK(this);
}

#ifdef $XCC_h
/**
 * For use by CodeRegion_FOREACH_BasicBlock.
 */
BasicBlock
CodeRegion_pastBlock(const_CodeRegion this);
#endif//$XCC_h

BasicBlock
CodeRegion_pastBlock(const_CodeRegion this)
{
  return CodeRegion_PASTBLOCK(this);
}

#ifdef $XCC_h
/**
 * Iterate this CodeRegion BasicBlock(s).
 *
 * Does not include the CodeRegion_rootBlock in iteration.
 */
#define CodeRegion_FOREACH_BasicBlock(this, basicBlock) { \
  BasicBlock CodeRegion_PASTBLOCK = CodeRegion_pastBlock(this); \
  BasicBlock basicBlock = CodeRegion_firstBlock(this); \
  for (; basicBlock != CodeRegion_PASTBLOCK; \
       basicBlock = BasicBlock_next(basicBlock)) { \
    Except_CHECK(BasicBlock_index(basicBlock) > 0); \
    Except_CHECK(BasicBlock_index(basicBlock) < CodeRegion_blockCount(this)); \
    {
#define CodeRegion_ENDEACH_BasicBlock \
    } \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This CodeRegion GLOBALTABLE.
 */
TemporaryTable
CodeRegion_globalTable(CodeRegion this);
#endif//$XCC_h

TemporaryTable
CodeRegion_globalTable(CodeRegion this)
{
  return CodeRegion_GLOBALTABLE(this);
}

#ifdef $XCC_h
/**
 * This CodeRegion VARIABLES.
 */
IBList
CodeRegion_variables(CodeRegion this);
#endif//$XCC_h

IBList
CodeRegion_variables(CodeRegion this)
{
  return CodeRegion_VARIABLES(this);
}

#ifdef $XCC_h
/**
 * Iterate this CodeRegion Variable(s).
 *
 * Caution: also includes unused Variable(s) in iteration.
 */
#define CodeRegion_FOREACH_Variable(this, variable) \
  IBList_FOREACH(CodeRegion_variables(this), Variable_, variable)
#define CodeRegion_ENDEACH_Variable \
  IBList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this CodeRegion live-out sets.
 */
void
CodeRegion_setLiveSets(CodeRegion this,
                       int32_t liveCount,
                       TemporarySet_ *liveInSets,
                       TemporarySet_ *liveOutSets,
                       TemporarySet_ *phiDefSets,
                       TemporarySet_ *phiUseSets);
#define CodeRegion_resetLiveSets(this) \
  CodeRegion_setLiveSets(this, 0, NULL, NULL, NULL, NULL)
#endif//$XCC_h

void
CodeRegion_setLiveSets(CodeRegion this,
                       int32_t liveCount,
                       TemporarySet_ *liveInSets,
                       TemporarySet_ *liveOutSets,
                       TemporarySet_ *phiDefSets,
                       TemporarySet_ *phiUseSets)
{
  Except_REQUIRE(liveCount <= CodeRegion_BLOCKCOUNT(this));
  *CodeRegion__LIVECOUNT(this) = liveCount;
  *CodeRegion__LIVEINSETS(this) = liveInSets;
  *CodeRegion__LIVEOUTSETS(this) = liveOutSets;
  *CodeRegion__PHIDEFSETS(this) = phiDefSets;
  *CodeRegion__PHIUSESETS(this) = phiUseSets;
}

#ifdef $XCC_h
int32_t
CodeRegion_liveCount(CodeRegion this);
TemporarySet_ *
CodeRegion_liveInSets(CodeRegion this);
TemporarySet_ *
CodeRegion_liveOutSets(CodeRegion this);
TemporarySet_ *
CodeRegion_phiDefSets(CodeRegion this);
TemporarySet_ *
CodeRegion_phiUseSets(CodeRegion this);
#endif//$XCC_h

int32_t
CodeRegion_liveCount(CodeRegion this)
{
  return CodeRegion_LIVECOUNT(this);
}
TemporarySet_ *
CodeRegion_liveInSets(CodeRegion this)
{
  return CodeRegion_LIVEINSETS(this);
}
TemporarySet_ *
CodeRegion_liveOutSets(CodeRegion this)
{
  return CodeRegion_LIVEOUTSETS(this);
}
TemporarySet_ *
CodeRegion_phiDefSets(CodeRegion this)
{
  return CodeRegion_PHIDEFSETS(this);
}
TemporarySet_ *
CodeRegion_phiUseSets(CodeRegion this)
{
  return CodeRegion_PHIUSESETS(this);
}

#ifdef $XCC_h
/**
 * This CodeRegion SSAForm.
 */
struct SSAForm_ *
CodeRegion_ssaForm(const_CodeRegion this);
#endif//$XCC_h

struct SSAForm_ *
CodeRegion_ssaForm(const_CodeRegion this)
{
  return CodeRegion_SSAFORM(this);
}

#ifdef $XCC_h
/**
 * Set this CodeRegion SSAForm.
 */
void
CodeRegion_setSSAForm(CodeRegion this, struct SSAForm_ *ssaForm);
#define CodeRegion_resetSSAForm(this) \
  CodeRegion_setSSAForm(this, NULL);
#endif//$XCC_h

void
CodeRegion_setSSAForm(CodeRegion this, struct SSAForm_ *ssaForm)
{
  *CodeRegion__SSAFORM(this) = ssaForm;
}

#ifdef $XCC_h
/**
 * This CodeRegion RegionFlow.
 */
struct RegionFlow_ *
CodeRegion_regionFlow(CodeRegion this);
#endif//$XCC_h

struct RegionFlow_ *
CodeRegion_regionFlow(CodeRegion this)
{
  return CodeRegion_REGIONFLOW(this);
}

#ifdef $XCC_h
/**
 * Set this CodeRegion RegionFlow.
 */
void
CodeRegion_setRegionFlow(CodeRegion this, struct RegionFlow_ *regionFlow);
#endif//$XCC_h

void
CodeRegion_setRegionFlow(CodeRegion this, struct RegionFlow_ *regionFlow)
{
  *CodeRegion__REGIONFLOW(this) = regionFlow;
}

#ifdef $XCC_h
/**
 * Make a new traceId from this CodeRegion TRACECOUNTER.
 */
intptr_t
CodeRegion_makeTraceId(CodeRegion this);
#endif//$XCC_h

intptr_t
CodeRegion_makeTraceId(CodeRegion this)
{
  return ++*CodeRegion__TRACECOUNTER(this);
}

#ifdef $XCC_h
/**
 * Reset this CodeRegion TRACECOUNTER.
 */
void
CodeRegion_resetTraceCounter(CodeRegion this);
#endif//$XCC_h

void
CodeRegion_resetTraceCounter(CodeRegion this)
{
  *CodeRegion__TRACECOUNTER(this) = 0;
}

#ifdef $XCC_h
/**
 * This CodeRegion Optimize.
 */
Optimize
CodeRegion_optimize(CodeRegion this);
#endif//$XCC_h

Optimize
CodeRegion_optimize(CodeRegion this)
{
  return CodeRegion_OPTIMIZE(this);
}

#ifdef $XCC_h
/**
 * This CodeRegion Configure.
 */
Configure
CodeRegion_configure(CodeRegion this);
#endif//$XCC_h

Configure
CodeRegion_configure(CodeRegion this)
{
  return CodeRegion_CONFIGURE(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Push a BasicBlock on this CodeRegion.
 *
 * Low-level call, prefer higher-level functions:
 * @sa Procedure_makeBasicBlock or CodeRegion_makeBasicBlock.
 */
void
CodeRegion_pushBasicBlock(CodeRegion this, BasicBlock basicBlock);
#endif//$XCC__h

void
CodeRegion_pushBasicBlock(CodeRegion this, BasicBlock basicBlock)
{
  if (CodeRegion_FIRSTBLOCK(this) == NULL) {
    *CodeRegion__FIRSTBLOCK(this) = basicBlock;
  }
  *CodeRegion__PASTBLOCK(this) = BasicBlock_next(basicBlock);
  BasicBlockTable_insert(CodeRegion_BLOCKTABLE(this), basicBlock);
}

#ifdef $XCC_h
/**
 * Make a BasicBlock inside this CodeRegion.
 *
 * @pre The CodeRegions must have been built (using
 * Procedure_buildCodeRegions() ).
 *
 * @warning The created ::BasicBlock cannot be an entry or exit of this CodeRegion.
 *
 * @sa Procedure_buildCodeRegions, Procedure_makeBasicBlock.
 */
BasicBlock
CodeRegion_makeBasicBlock(CodeRegion this, float frequency);
#endif//$XCC_h

BasicBlock
CodeRegion_makeBasicBlock(CodeRegion this, float frequency)
{
  Procedure procedure = CodeRegion_PROCEDURE(this);
  intptr_t regionId = CodeRegion_REGIONID(this);
  Memory memory = Procedure_MEMORY(procedure);
  BasicBlock pastBlock = CodeRegion_PASTBLOCK(this);
  size_t size = BasicBlock_Size(memory, procedure, this, frequency);
  IDList blockChain = Procedure_BLOCKCHAIN(procedure);
  BasicBlock new_block = IDList_makeItem(blockChain, size);
  if (pastBlock != NULL) {
    IDList_insert_(blockChain, pastBlock, new_block);
  } else {
    IDList_push_(blockChain, new_block);
  }
  BasicBlock_Ctor(new_block, memory, procedure, this, frequency);
  CodeRegion_pushBasicBlock(this, new_block);
  return new_block;
}

#ifdef $XCC_h
/**
 * Split a BasicBlock and return the second half.
 */
BasicBlock
CodeRegion_splitBasicBlock(CodeRegion this, BasicBlock basicBlock, int32_t count);
#endif//$XCC_h

BasicBlock
CodeRegion_splitBasicBlock(CodeRegion this, BasicBlock basicBlock, int32_t count)
{
  Procedure procedure = CodeRegion_PROCEDURE(this);
  intptr_t regionId = CodeRegion_REGIONID(this);
  BasicBlock pastBlock = CodeRegion_PASTBLOCK(this);
  // Make a new BasicBlock just after basicBlock.
  Memory memory = Procedure_MEMORY(procedure);
  float frequency = BasicBlock_FREQUENCY(basicBlock);
  size_t size = BasicBlock_Size(memory, procedure, this, frequency);
  IDList blockChain = Procedure_BLOCKCHAIN(procedure);
  BasicBlock new_block = IDList_makeItem(blockChain, size);
  BasicBlock_Ctor(new_block, memory, procedure, this, frequency);
  IDList_append_(blockChain, basicBlock, new_block);
  // Transfer basicBlock LEAVEEDGES to new_block LEAVEEDGES.
  IDList_transfer(BasicBlock_LEAVEEDGES(basicBlock), BasicBlock_LEAVEEDGES(new_block));
  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(new_block, edge) {
    *BasicBlockEdge__ORIGBLOCK(edge) = new_block;
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  // Make a fall through edge from basicBlock to basicBlock.
  BasicBlock_makeEdge(basicBlock, new_block, 1.0);
  while (count-- > 0) {
    Operation operation = Selector_detachLast(BasicBlock_SELECTOR(basicBlock));
    if (Operator_isPhi(Operation_operator(operation))) {
      // Cannot transfer a PHI Operation, so put it back and break.
      Selector_attachLast(BasicBlock_SELECTOR(basicBlock), operation);
      break;
    }
    Selector_attachFirst(BasicBlock_SELECTOR(new_block), operation);
  }
  // Update this CodeRegion.
  if (pastBlock == basicBlock) {
    *CodeRegion__PASTBLOCK(this) = new_block;
  }
  BasicBlockTable_insert(CodeRegion_BLOCKTABLE(this), basicBlock);
  return new_block;
}

#ifdef $XCC_h
/**
 * Split the BasicBlock(s) of this CodeRegion.
 */
int32_t
CodeRegion_splitBasicBlocks(CodeRegion this, int32_t count);
#endif//$XCC_h

int32_t
CodeRegion_splitBasicBlocks(CodeRegion this, int32_t count)
{
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    // Split the large BasicBlock(s).
    while (BasicBlock_countOperations(basicBlock) > count) {
      CodeRegion_splitBasicBlock(this, basicBlock, count);
    }
  } CodeRegion_ENDEACH_BasicBlock;
  return CodeRegion_BLOCKCOUNT(this);
}

#ifdef $XCC_h
/**
 * Split the edge (A,B) with new block C.
 */
BasicBlock
CodeRegion_splitBasicBlockEdge(CodeRegion this, BasicBlockEdge edgeAB);
#endif//$XCC_h

BasicBlock
CodeRegion_splitBasicBlockEdge(CodeRegion this, BasicBlockEdge edgeAB)
{
  Procedure procedure = CodeRegion_PROCEDURE(this);
#if 0
  float probAB = BasicBlockEdge_PROBABILITY(edgeAB);
  BasicBlock bbA = BasicBlockEdge_ORIGBLOCK(edgeAB);
  BasicBlock bbB = BasicBlockEdge_DESTBLOCK(edgeAB);
  float freqC = probAB * BasicBlock_FREQUENCY(bbA);
  int32_t leaveCountA = BasicBlock_LEAVECOUNT(bbA);
  int32_t enterCountB = BasicBlock_ENTERCOUNT(bbB);
  // Make C, reroute (A,B) to (A,C) and add (C, B).
  BasicBlock bbC = CodeRegion_makeBasicBlock(this, freqC);
  BasicBlockEdge edgeAC = BasicBlockEdge_changeDestBlock(edgeAB, bbC);
  BasicBlockEdge edgeCB = BasicBlock_makeEdge(bbC, bbB, 1.0);
  Operation branchA = BasicBlock_getControlOperation(bbA);
  Label labelB = BasicBlock_makeLabel(bbB);
  Label labelC = BasicBlock_makeLabel(bbC);
  Except_CHECK(branchA != NULL);
  if (BasicBlock_next(bbA) == bbB) {
    // B was next to A, insert C between A and B.
    Procedure_chainBasicBlocks(procedure, bbA, bbC);
    if (Operator_isFall(Operation_OPERATOR(branchA))) {
      // Update Fall operation of A.
      Procedure_updateTargetedOperation(procedure, branchA, labelC);
    }
  } else if (Operator_isBranch(Operation_OPERATOR(branchA))) {
    // Must update branch operation of A and insert a GOTO from C to B.
    Procedure_updateTargetedOperation(procedure, branchA, labelC);
    Selector_selectGOTO(BasicBlock_SELECTOR(bbC), labelB);
  }
  if (enterCountB == 1) {
    // Non-critical edge.
    // No PHI or ENTEREDGES in bbB.
    Except_CHECK(BasicBlock_ENTEREDGES(bbB) == NULL);
  } else {
    // Must take care of PHI and ENTEREDGES in bbB.
    if (leaveCountA == 1) {
      // Non-critical edge.
      // Insert C between A and B.
      Procedure_chainBasicBlocks(procedure, bbA, bbC);
    } else {
      // Critical edge.
      BasicBlockEdge edgeCB = BasicBlock_makeEdge(bbC, bbB, 1.0);
      // Reroute (A,B) to (A,C).
      BasicBlockEdge edgeAC = BasicBlockEdge_changeDestBlock(edgeAB, bbC);
      // Update branch operation of A.
    }
  }
  return bbC;
#endif
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Set a BasicBlock as entering this CodeRegion.
 */
void
CodeRegion_setEnterBlock(CodeRegion this, BasicBlock basicBlock);
#endif//$XCC_h

void
CodeRegion_setEnterBlock(CodeRegion this, BasicBlock basicBlock)
{
  BasicBlock rootBlock = CodeRegion_ROOTBLOCK(this);
  Except_REQUIRE(basicBlock != rootBlock);
  BasicBlock_makeEdge(rootBlock, basicBlock, 0.0);
}

#ifdef $XCC_h
/**
 * Set a BasicBlock as leaving this CodeRegion.
 */
void
CodeRegion_setLeaveBlock(CodeRegion this, BasicBlock basicBlock);
#endif//$XCC_h

void
CodeRegion_setLeaveBlock(CodeRegion this, BasicBlock basicBlock)
{
  BasicBlock rootBlock = CodeRegion_ROOTBLOCK(this);
  Except_REQUIRE(basicBlock != rootBlock);
  BasicBlock_makeEdge(basicBlock, rootBlock, 0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC_h
/**
 * Removes any superfluous "dead" copy operations that may have been
 * induced by previous passes and register allocation.
 */
void
CodeRegion_removeDeadCopies(CodeRegion this);
#endif//$XCC_h

void
CodeRegion_removeDeadCopies(CodeRegion this)
{
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    Selector selector = BasicBlock_selector(basicBlock);

    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      Temporary * results = Operation_results(operation);
      Temporary * arguments = Operation_arguments(operation);

      if (Operator_isSeqCopy(operator)
          && Temporary_hasRegister(results[0])
          && Temporary_hasRegister(arguments[0])
          && (Temporary_register(results[0]) == Temporary_register(arguments[0]))) {
        Selector_detachOperation(selector, operation);
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

#ifdef $XCC_h
/**
 * Walks every instruction in the region and expands all
 * generics and simulated to their associated code sequences.
 * It also removes some of the Pseudo operators (only KILL for now).
 */
void
CodeRegion_expandGenericsSimulated(CodeRegion this);
#endif//$XCC_h

void
CodeRegion_expandGenericsSimulated(CodeRegion this)
{
  Selector selector = CodeRegion_SELECTOR(this);
  Processor processor = Selector_PROCESSOR(selector);
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    BasicBlock_FORBACK_Operation(basicBlock, operation) {
      // If the operation is a generic, try to expand it
      Operator operator = Operation_operator(operation);
      Temporary *results = Operation_results(operation);
      Temporary *arguments = Operation_arguments(operation);
      Literal literal = NULL;
#if 0
      fprintf(stderr, "%s:%d, operation is:\n", __FILE__, __LINE__);
      Operation_pretty(operation, stderr);
      if ( Operator_opcode(operator, processor) == Opcode__UNDEF)
        fprintf(stderr, "\tno opcode for op\n");
      if ( Operator_isPseudo(operator))
        fprintf(stderr, "\top is pseudo\n");
#endif

      if(Operator_isAdjust(operator)) {
        Temporary size;
        Except_CHECK(Operation_resCount(operation) == 1);
        Except_CHECK(Operation_argCount(operation) == 2);
        size = Operation_arguments(operation)[1];
        if(Temporary_hasLiteral(size)) {
          Literal literal = Temporary_LITERAL(size);
          if(Literal_value(literal) == 0 &&
             Operation_results(operation)[0] ==
             Operation_arguments(operation)[0]
              ) {
            size = NULL; // ADJUST can just be removed
          }
        }
        if(size != NULL) {
          Selector_selectSUB(selector, 
              Operation_results(operation)[0],
              Operation_arguments(operation)[0],
              Operation_arguments(operation)[1]
              );
          BasicBlock_insertSelector(basicBlock, operation, selector);
        }
        BasicBlock_detachOperation(basicBlock, operation);
      }
      if (Operator_opcode(operator, processor) == Opcode__UNDEF) {
        if(Operator_isPseudo(operator)) {
          switch (operator) {
            case Operator_KILL:
              BasicBlock_detachOperation(basicBlock, operation);
              break;
            default:
              Except_THROW("Pseudo operator not mapped to actual operator: `%s'!",
                  Operator_shortName(operator));
          }
        } else {
          switch (operator) {
            case Operator_GOTO:
              Except_REQUIRE(Operation_resCount(operation) == 0);
              Except_REQUIRE(Operation_argCount(operation) == 1);
              Except_REQUIRE(Temporary_isLabel(arguments[0]));
              literal = Temporary_LITERAL(arguments[0]);
              Selector_selectGOTO(selector, Literal_LABEL(literal));
              break;
            case Operator_IFTRUE:
              Except_REQUIRE(Operation_resCount(operation) == 0);
              Except_REQUIRE(Operation_argCount(operation) == 2);
              literal = Temporary_LITERAL(arguments[1]);
              Selector_selectIFTRUE(selector, arguments[0],
                  Literal_LABEL(literal));
              break;
            case Operator_IFFALSE:
              Except_REQUIRE(Operation_resCount(operation) == 0);
              Except_REQUIRE(Operation_argCount(operation) == 2);
              literal = Temporary_LITERAL(arguments[1]);
              Selector_selectIFTRUE(selector, arguments[0],
                  Literal_LABEL(literal));
              break;
            case Operator_CALL:
              Except_REQUIRE(Operation_resCount(operation) == 0);
              Except_REQUIRE(Operation_argCount(operation) == 1);
              Selector_selectCALL(selector, arguments[0]);
              break;
            case Operator_LINK:
              Except_THROW("Generic operator not mapped to actual operator: `%s'!",
                  Operator_shortName(operator));
              break;
            case Operator_RETURN:
              Except_REQUIRE(Operation_resCount(operation) == 0);
              Except_REQUIRE(Operation_argCount(operation) == 0);
              Selector_selectRETURN(selector);
              break;
            case Operator_SKIP:
              Except_THROW("Generic operator not mapped to actual operator: `%s'!",
                  Operator_shortName(operator));
              break;
            case Operator_COPY:
              Except_REQUIRE(Operation_resCount(operation) == 1);
              Except_REQUIRE(Operation_argCount(operation) == 1);
              Selector_selectCOPY(selector, results[0], arguments[0]);
              break;
            case Operator_LNOT:
              Except_REQUIRE(Operation_resCount(operation) == 1);
              Except_REQUIRE(Operation_argCount(operation) == 1);
              Selector_selectLNOT(selector, results[0], arguments[0]);
              break;
            case Operator_BNOT:
            case Operator_NEG:
              Except_THROW("Generic operator not mapped to actual operator: `%s'!",
                  Operator_shortName(operator));
              break;
            case Operator_ADD:
              Except_REQUIRE(Operation_resCount(operation) == 1);
              Except_REQUIRE(Operation_argCount(operation) == 2);
              Selector_selectADD(selector, results[0], arguments[0], arguments[1]);
              break;
            case Operator_SUB:
            case Operator_MUL:
            case Operator_DIV:
            case Operator_REM:
            case Operator_SHL:
            case Operator_ASHR:
            case Operator_LSHR:
              Except_THROW("Generic operator not mapped to actual operator: `%s'!",
                  Operator_shortName(operator));
              break;
            case Operator_LAND:
              Except_REQUIRE(Operation_resCount(operation) == 1);
              Except_REQUIRE(Operation_argCount(operation) == 2);
              Selector_selectLAND(selector, results[0], arguments[0], arguments[1]);
              break;
            case Operator_LIOR:
              Except_REQUIRE(Operation_resCount(operation) == 1);
              Except_REQUIRE(Operation_argCount(operation) == 2);
              Selector_selectLOR(selector, results[0], arguments[0], arguments[1]);
              break;
            case Operator_LXOR:
            case Operator_BAND:
            case Operator_BIOR:
            case Operator_BXOR:
            case Operator_MIN:
            case Operator_MAX:
              Except_THROW("Generic operator not mapped to actual operator: `%s'!",
                  Operator_shortName(operator));
              break;
            case Operator_SELECT:
              Except_REQUIRE(Operation_resCount(operation) == 1);
              Except_REQUIRE(Operation_argCount(operation) == 3);
              Selector_selectSELECT(selector, results[0], arguments[0],
                  arguments[1], arguments[2]);
              break;
            case Operator_APPLY:
              Except_REQUIRE(Operation_resCount(operation) == 1);
              Except_REQUIRE(Operation_argCount(operation) == 1);
              Selector_selectMAKE(selector, results[0], arguments[0]);
              break;
            default:
              Except_THROW("Generic operator not mapped to actual operator: `%s'!",
                  Operator_shortName(operator));
              break;
          }
          BasicBlock_insertSelector(basicBlock, operation, selector);
          BasicBlock_detachOperation(basicBlock, operation);
        }
      }
    } BasicBlock_ENDBACK_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  *Selector__PROCESSOR(selector) = Processor__UNDEF;
  Except_CHECK(Selector_isEmpty(selector));
}

#ifdef $XCC_h
/**
 * Walks every instruction in the region and expands all
 * simulated to their actual operations.
 *
 * @note currently, only expands ADJUST operation, and badly !
 * Use instead @c CodeRegion_expandGenericsSimulated !
 */
void
CodeRegion_expandSimulated(CodeRegion this);
#endif//$XCC_h

void
CodeRegion_expandSimulated(CodeRegion this)
{
  Selector selector = CodeRegion_selector(this);
  Processor processor = Selector_processor(selector);
  Except_CHECK(false); // This function should not be used anymore!
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    int numAdjust = 0;
    bool reverse = true;

    if(BasicBlock_isEntry(basicBlock)) { numAdjust++; reverse = false; }
    if(BasicBlock_isExit(basicBlock)) numAdjust++;

    for(int i=0; i<numAdjust; i++) {
      
      Operation adjust = BasicBlock_findStackAdjust(basicBlock, reverse);
      if (adjust)
      {
        Except_CHECK(Operator_isAdjust(Operation_operator(adjust)));
        Except_CHECK(Operation_resCount(adjust) == 1);
        Except_CHECK(Operation_argCount(adjust) == 2);

      { Temporary size = Operation_arguments(adjust)[1];
        Literal literal = Temporary_LITERAL(size);
        Except_CHECK(Temporary_hasLiteral(size));
        Literal_setValue(literal, -Literal_value(literal));
      }

        Selector_selectADD(selector, 
            Operation_results(adjust)[0],
            Operation_arguments(adjust)[0],
            Operation_arguments(adjust)[1]
            );

        BasicBlock_insertSelector(basicBlock, adjust, selector);
        BasicBlock_detachOperation(basicBlock, adjust);
      }
      reverse = true;
    }
  } CodeRegion_ENDEACH_BasicBlock;
  *Selector__PROCESSOR(selector) = Processor__UNDEF;
  Except_CHECK(Selector_isEmpty(selector));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make the @c GLOBALTABLE of this @c CodeRegion.
 * 
 * The @c GLOBALTABLE is a @c TemporaryTable that provides dense INDEXing of
 * global Temporary(ies) and is also useful as a ground set for @c
 * TemporarySparse sets. In particular, the @c GLOBALTABLE is the ground set
 * used by @c Liveness and @c SSAConstruct. In case a @c Temporary has a @c
 * Register assigned, the representative dedicated @c Temporary is used instead.
 * This means that @c Temporary(ies) with an assigned @c Register are never
 * entered into the @c GLOBALTABLE. In addition, we only enter in the @c
 * GLOBALTABLE whose flags in @p flagsMask match the value in @p flagsValue. As
 * a side-effect of this function, the @c TemporaryFlag_Global is cleared for
 * each @c Temporary.
 *
 * The @c GLOBALTABLE is filled using Briggs, Cooper, Simpson algorithm to
 * compute non-locals given in "Practical Improvements to the Construction and
 * Destruction of Static Single Assignment Form". The killed set of this
 * algorithm is represented by combining the @c TemporaryStack @c killed and the
 * @c TemporaryFlag_Killed. Using this flag prevents multiple pushes of the same
 * @c Temporary and we check this flag by taking advantage of the existing test
 * for matching @c flagsMask and @c flagsValue in @c Temporary_FLAGS.
 *
 * The returned table may only include register temporaries 
 * (@c Temporary_hasRegFile()).
 *
 * @return a @c TemporaryTable
 */
TemporaryTable
CodeRegion_makeGlobalTable(CodeRegion this,
                           TemporaryFlags flagsMask,
                           TemporaryFlags flagsValue);
#endif//$XCC_h

TemporaryTable
CodeRegion_makeGlobalTable(CodeRegion this,
                           TemporaryFlags flagsMask,
                           TemporaryFlags flagsValue)
{
  Procedure procedure = CodeRegion_procedure(this);
  Temporary_ *dedicated = Procedure_dedicated(procedure);
  TemporaryTable globalTable = CodeRegion_GLOBALTABLE(this);
  TemporaryTable_empty(globalTable);
  flagsValue &= flagsMask;
  if (CodeRegion_BRIGGSGLOBALS) {
    TemporaryStack_ killed[1];
    TemporaryStack_Ctor(killed, CodeRegion_MEMORY(this), 1024);
    // We only enter non-killed Temporary(ies) in the GLOBALTABLE.
    Except_CHECK(!(flagsMask & TemporaryFlag_Killed));
    flagsMask |= TemporaryFlag_Killed;
    CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
      BasicBlock_FOREACH_Operation(basicBlock, operation) {
        RegisterSet clobberSet = Operation_clobberSet(operation);
        // Operation arguments.
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (Temporary_hasRegFile(argument)) {
            if ((Temporary_FLAGS(argument) & flagsMask) == flagsValue) {
              if (Temporary_hasRegister(argument)) {
                argument = dedicated + Temporary_REGISTER(argument);
              }
              TemporaryTable_insert(globalTable, argument);
//              } else
//                TemporaryTable_insert(globalTable, argument);
            }
            // Reset the Global flag (will be recomputed later).
            *Temporary__FLAGS(argument) &= ~TemporaryFlag_Global;
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
        // Operation clobbers.
        if (clobberSet != NULL) {
          RegisterSet_FOREACH_Register(clobberSet, registre) {
            Temporary clobbered = dedicated + registre;
            if ((Temporary_FLAGS(clobbered) & flagsMask) == flagsValue) {
              // Add clobbered to the killed set.
              *Temporary__FLAGS(clobbered) |= TemporaryFlag_Killed;
              TemporaryStack_push(killed, clobbered);
            }
            // Reset the Global flag (will be recomputed later).
            *Temporary__FLAGS(clobbered) &= ~TemporaryFlag_Global;
          } RegisterSet_ENDEACH_Register;
        }
        // Operation results.
        Operation_RESULTS_FOREACH_Temporary(operation, result) {
          if (Temporary_hasRegFile(result)) {
            // Partial defs are a problem in SSA until we implement PSI-SSA.
            if ((Temporary_FLAGS(result) & flagsMask) == flagsValue) {
              if (Temporary_hasRegister(result)) {
                result = dedicated + Temporary_REGISTER(result);
              }
              if (Operator_isPhi(Operation_OPERATOR(operation))) {
                // Don't kill PHI-function results so they are added to the globalTable.
              } else {
                // Add result to the killed set.
                *Temporary__FLAGS(result) |= TemporaryFlag_Killed;
                TemporaryStack_push(killed, result);
              }
            }
            // Reset the Global flag (will be recomputed later).
            *Temporary__FLAGS(result) &= ~TemporaryFlag_Global;
          }
        } Operation_RESULTS_ENDEACH_Temporary;
      } BasicBlock_ENDEACH_Operation;
      // Reset the Killed flag and empty the killed TemporaryStack.
      TemporaryStack_FOREACH_Temporary(killed, temporary) {
        *Temporary__FLAGS(temporary) &= ~TemporaryFlag_Killed;
      } TemporaryStack_ENDEACH_Temporary;
      TemporaryStack_empty(killed);
    } CodeRegion_ENDEACH_BasicBlock;
    TemporaryStack_Dtor(killed);
  } else /*!CodeRegion_BRIGGSGLOBALS*/ {
    CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
      int32_t prev_count = TemporaryTable_count(globalTable);
      RegisterSet_ seenColorsInPhis[1];
      RegisterSet_Ctor(seenColorsInPhis);
      BasicBlock_FORBACK_Operation(basicBlock, operation) {
        RegisterSet clobberSet = Operation_clobberSet(operation);
        // Operation results.
        Operation_RESULTS_FOREACH_Temporary(operation, result) {
          if (Temporary_hasRegFile(result)) {
            if ((Temporary_FLAGS(result) & flagsMask) == flagsValue) {
              if (Temporary_hasRegister(result)) {
                result = dedicated + Temporary_REGISTER(result);
              }
              if (Operator_isPhi(Operation_OPERATOR(operation))) {
              } else if (   !Temporary_hasRegister(result)
                         || !RegisterSet_contains(seenColorsInPhis, Temporary_REGISTER(result))) {
                // Kill definition, remove from globalTable if inserted in basicBlock.
                TemporaryTable_removeHigh(globalTable, result, prev_count);
              }
            }
            // Reset the Global flag (will be recomputed later).
            *Temporary__FLAGS(result) &= ~TemporaryFlag_Global;
          }
        } Operation_RESULTS_ENDEACH_Temporary;
        // Operation clobbers.
        if (clobberSet != NULL) {
          RegisterSet_FOREACH_Register(clobberSet, registre) {
            Temporary clobbered = dedicated + registre;
            if ((Temporary_FLAGS(clobbered) & flagsMask) == flagsValue) {
              TemporaryTable_removeHigh(globalTable, clobbered, prev_count);
            }
            // Reset the Global flag (will be recomputed later).
            *Temporary__FLAGS(clobbered) &= ~TemporaryFlag_Global;
          } RegisterSet_ENDEACH_Register;
        }
        // Operation arguments.
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (Temporary_hasRegFile(argument)) {
            if ((Temporary_FLAGS(argument) & flagsMask) == flagsValue) {
              if (Temporary_hasRegister(argument)) {
                argument = dedicated + Temporary_REGISTER(argument);
                if (Operator_isPhi(Operation_operator(operation))) {
                  RegisterSet_insert(seenColorsInPhis, Temporary_REGISTER(argument));
                }
              }
              TemporaryTable_insert(globalTable, argument);
//              } else
//                TemporaryTable_insert(globalTable, argument);
            }
            // Reset the Global flag (will be recomputed later).
            *Temporary__FLAGS(argument) &= ~TemporaryFlag_Global;
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
      } BasicBlock_ENDBACK_Operation;
    } CodeRegion_ENDEACH_BasicBlock;
  }
  // Set the Global flag for each Temporary in the globalTable.
  TemporaryTable_FOREACH_Temporary(globalTable, temporary) {
    *Temporary__FLAGS(temporary) |= TemporaryFlag_Global;
  } TemporaryTable_ENDEACH_Temporary;
  LIR_TRACE && fprintf(LIR_TRACE, "*** CodeRegion_makeGlobalTable\tcount=%d\t",
                       TemporaryTable_count(globalTable))
            && TemporaryTable_pretty(globalTable, LIR_TRACE) && fprintf(LIR_TRACE, "\n");
  return globalTable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make a new SSA variable Temporary.
 *
 * @param origin is the temporary from which the variable is created.
 *
 * The created Variable @c Temporary has same @c Temporary_INDEX as @p origin.
 */
Temporary
CodeRegion_makeVariableTemporary(CodeRegion this, Temporary origin);
#endif//$XCC_h

Temporary
CodeRegion_makeVariableTemporary(CodeRegion this, Temporary origin)
{
  IBList variables = CodeRegion_VARIABLES(this);
  Variable variable = IBList_push(variables);
  Variable_Ctor(variable, origin);
  if (CodeRegion_CONSUMEM(this) != NULL) {
    Variable_makeConsumers(variable, CodeRegion_CONSUMEM(this));
  }
  return Variable____(variable);
}

#ifdef $XCC_h
/**
 * Make the CONSUMERS for this CodeRegion Variable(s).
 *
 * Get the Variable(s) from the TemporaryTable. If @p table is @c NULL, 
 * all variables from the CodeRegion are considered.
 */
void
CodeRegion_makeConsumers(CodeRegion this, TemporaryTable table);
#endif//$XCC_h

void
CodeRegion_makeConsumers(CodeRegion this, TemporaryTable table)
{
  Memory consuMemory = CodeRegion_CONSUMEM(this);
  if (consuMemory == NULL) {
    consuMemory = Memory_new(CodeRegion_MEMORY(this), true);
    *CodeRegion__CONSUMEM(this) = consuMemory;
  }
  if(table == NULL) {
    CodeRegion_FOREACH_Variable(this, variable) {
      Variable_makeConsumers(variable, consuMemory);
    } CodeRegion_ENDEACH_Variable;
  } else {
    TemporaryTable_FOREACH_Temporary(table, temporary) {
      if (Temporary_isVariable(temporary)) {
        Variable variable = Temporary_VARIABLE(temporary);
        Variable_makeConsumers(variable, consuMemory);
      }
    } TemporaryTable_ENDEACH_Temporary;
  }
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
        if (   Temporary_isVariable(argument)
            && (table == NULL || TemporaryTable_contains(table, argument))) {
          Variable variable = Temporary_VARIABLE(argument);
          Variable_addConsumer(variable, operation, Operation_ARGUMENTS_INDEX);
        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Add this CodeRegion to a NativeStream.
 */
void
CodeRegion_addtoNativeStream(CodeRegion this, NativeStream stream);
#endif//$XCC_h

void
CodeRegion_addtoNativeStream(CodeRegion this, NativeStream stream)
{
  Processor processor = CodeRegion_PROCESSOR(this);
  Optimize optimize = CodeRegion_OPTIMIZE(this);
  int maxBBOC = 1<<Optimize_LOGMAXBBOC(optimize);
  CodeRegion_splitBasicBlocks(this, maxBBOC);
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    // Create the nativeBlock.
    NativeBlockFlags flags = 0;
    NativeBlock nativeBlock = NULL;
    void *label = BasicBlock_label(basicBlock);
    if (BasicBlock_isEntry(basicBlock)) flags |= NativeBlockFlag_Entry;
    if (BasicBlock_isExit(basicBlock)) flags |= NativeBlockFlag_Exit;
    nativeBlock = NativeStream_pushNativeBlock(stream, processor, flags, label);
    *BasicBlock__NATIVEBLOCK(basicBlock) = nativeBlock;
    // Fill the nativeBlock with Instruction(s).
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Instruction instruction = Operation_makeInstruction(operation, processor, nativeBlock);
      Except_CHECK(Operation_TASK(operation) == NULL);
      *Operation__TASK(operation) = Instruction_TASK(instruction);
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Compute register pressures.
 */
void
CodeRegion_computePressures(const_CodeRegion this, int *actual, int *virtual);
#endif//$XCC_h

static void
TemporarySet_fillPressures(TemporarySet this, int *actualPressure, int *virtualPressure)
{
  int regFile = RegFile__UNDEF;
  for (; regFile < RegFile__Count; regFile++) {
    actualPressure[regFile] = virtualPressure[regFile] = 0;
  }
  TemporarySet_FOREACH_Temporary(this, temporary) {
    if (Temporary_hasRegister(temporary)) {
      actualPressure[Temporary_REGFILE(temporary)]++;
    } else if (Temporary_hasRegFile(temporary)) {
      virtualPressure[Temporary_REGFILE(temporary)]++;
    }
  } TemporarySet_ENDEACH_Temporary;
}

void
CodeRegion_computePressures(const_CodeRegion this, int *actual, int *virtual)
{
  const_Procedure procedure = CodeRegion_PROCEDURE(this);
  TemporarySet liveSet = CodeRegion_LIVEINSETS(this) + 0;
  TemporarySet_ *liveOutSets = CodeRegion_LIVEOUTSETS(this);
  TemporarySet_ *phiUseSets = CodeRegion_PHIUSESETS(this);
  Temporary_ *dedicated = Procedure_DEDICATED(procedure);
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    int32_t index = BasicBlock_index(basicBlock);
    int *actualPressure = alloca(sizeof(int)*RegFile__Count);
    int *virtualPressure = alloca(sizeof(int)*RegFile__Count);
    int regFile;
    // let liveSet = liveOutSet(b)
    TemporarySet_assign(liveSet, liveOutSets + index);
    if (phiUseSets != NULL) {
      TemporarySet_union(liveSet, phiUseSets + index);
    }
    //
    TemporarySet_fillPressures(liveSet, actualPressure, virtualPressure);
    for (regFile = RegFile__UNDEF; regFile < RegFile__Count; regFile++) {
      actual[index*RegFile__Count + regFile] = actualPressure[regFile];
      virtual[index*RegFile__Count + regFile] = virtualPressure[regFile];
    }
    // forall I in instructions(b) in reverse order
    BasicBlock_FORBACK_Operation(basicBlock, operation) {
      // liveSet <- liveSet union def(I)
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        if (Temporary_hasRegister(result)) {
          result = dedicated + Temporary_REGISTER(result);
        }
        if (Temporary_hasRegFile(result)) {
          TemporarySet_insert(liveSet, result);
        }
      } Operation_RESULTS_ENDEACH_Temporary;
      //
      TemporarySet_fillPressures(liveSet, actualPressure, virtualPressure);
      for (regFile = RegFile__UNDEF; regFile < RegFile__Count; regFile++) {
        actual[index*RegFile__Count + regFile] = actualPressure[regFile];
        virtual[index*RegFile__Count + regFile] = virtualPressure[regFile];
      }
      // liveSet <- Use(I) union (liveSet\def(I))
      if (!Operator_isPhi(Operation_operator(operation))) {
        Operation_RESULTS_FOREACH_Temporary(operation, result) {
          if (Temporary_hasRegister(result)) {
            result = dedicated + Temporary_REGISTER(result);
          }
          if (Temporary_hasRegFile(result)) {
            TemporarySet_remove(liveSet, result);
          }
        } Operation_RESULTS_ENDEACH_Temporary;
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (Temporary_hasRegister(argument)) {
            argument = dedicated + Temporary_REGISTER(argument);
          }
          if (Temporary_hasRegFile(argument)) {
            TemporarySet_insert(liveSet, argument);
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
      }
      //
      TemporarySet_fillPressures(liveSet, actualPressure, virtualPressure);
      for (regFile = RegFile__UNDEF; regFile < RegFile__Count; regFile++) {
        if (actual[index*RegFile__Count + regFile] < actualPressure[regFile]) {
          actual[index*RegFile__Count + regFile] = actualPressure[regFile];
        }
        if (virtual[index*RegFile__Count + regFile] < virtualPressure[regFile]) {
          virtual[index*RegFile__Count + regFile] = virtualPressure[regFile];
        }
      }
    } BasicBlock_ENDBACK_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  TemporarySet_empty(liveSet);
}

#ifdef $XCC_h
bool
CodeRegion_prettyPressures(const_CodeRegion this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
CodeRegion_prettyPressures(const_CodeRegion this, FILE *file)
{
  int32_t blockCount = CodeRegion_blockCount(this), index, regFile;
  int *actual = alloca(sizeof(int)*RegFile__Count*blockCount);
  int *virtual = alloca(sizeof(int)*RegFile__Count*blockCount);
  CodeRegion_computePressures(this, actual, virtual);
  for (index = 1; index < blockCount; index++) {
    const char *sep = "actual={";
    fprintf(file, "*** Pressure Block_%d\t", index);
    for (regFile = RegFile__UNDEF + 1; regFile < RegFile__Count; regFile++) {
      fprintf(file, "%s%s=>%d", sep, RegFile_name(regFile), actual[index*RegFile__Count + regFile]);
      sep = ",";
    }
    sep = "}\tvirtual={";
    for (regFile = RegFile__UNDEF + 1; regFile < RegFile__Count; regFile++) {
      fprintf(file, "%s%s=>%d", sep, RegFile_name(regFile), virtual[index*RegFile__Count + regFile]);
      sep = ",";
    }
    fprintf(file, "}\n");
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty-print this CodeRegion.
 */
bool
CodeRegion_pretty(CodeRegion this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
CodeRegion_pretty(CodeRegion this, FILE *file)
{
  float contributed = 0.0;
  Procedure procedure = CodeRegion_PROCEDURE(this);
  const char *procedureName = Procedure_name(procedure);
  if (file == NULL) file = stderr;
  fprintf(file, "------- Region %zu (%s) -----\n", CodeRegion_REGIONID(this), procedureName);
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    LoopScope loopScope = BasicBlock_LOOPSCOPE(basicBlock);
    BasicBlock headerBlock = loopScope != NULL? LoopScope_HEADERBLOCK(loopScope): NULL;
    if (loopScope != NULL && headerBlock == basicBlock) {
      LoopScope_pretty(loopScope, file);
      fprintf(file, "\n");
    }
    BasicBlock_pretty(basicBlock, file);
    contributed += BasicBlock_contributed(basicBlock);
  } CodeRegion_ENDEACH_BasicBlock;
  fprintf(file, "------- contributed=%g -----\n", contributed);
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Pretty-print this in the Minir format.
 */
bool
CodeRegion_prettyMinir(CodeRegion this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
CodeRegion_prettyMinir(CodeRegion this, FILE *file)
{
  float contributed = 0.0;
  const char *sep = "";
  Procedure procedure = CodeRegion_procedure(this);
  const char *procedureName = Procedure_name(procedure);
  if (file == NULL) file = stderr;
  /* Print Minir header for this function */
  fprintf(file, "  - label: %s_%zu\n", procedureName, CodeRegion_REGIONID(this));
  /* Entry basic blocks */
  fprintf(file, "    entries: [");
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    if (BasicBlock_isEntry(basicBlock)) {
      fprintf(file, "%sBlock_%d", sep, BasicBlock_identity(basicBlock));
      sep = ", ";
    }
  } CodeRegion_ENDEACH_BasicBlock;
  fprintf(file, "]\n");
  sep = "";
  /* Exit basic blocks */
  fprintf(file, "    exit: [");
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    if (BasicBlock_isExit(basicBlock)) {
      fprintf(file, "%sBlock_%d", sep, BasicBlock_identity(basicBlock));
      sep = ", ";
    }
  } CodeRegion_ENDEACH_BasicBlock;
  fprintf(file, "]\n");

  /* Print Minir header for this function */
  fprintf(file, "    bbs:\n");
  CodeRegion_FOREACH_BasicBlock(this, basicBlock) {
    /* TODO: handle loopscope in Minir */
    BasicBlock_prettyMinir(basicBlock, file);
    contributed += BasicBlock_contributed(basicBlock);
  } CodeRegion_ENDEACH_BasicBlock;
  /* TODO: contributed? */
  return true;
}
#endif//_NTRACE


#ifdef $XCC_h
/**
 * Print VCG graph of this code region.
 */
bool
CodeRegion_prettyVCG(CodeRegion this, FILE *file, const char *name);
#endif//$XCC_h

#ifndef _NTRACE
bool
CodeRegion_prettyVCG(CodeRegion this, FILE *file, const char *name)
{
  if (file == NULL) file = stderr;
  //
  fprintf(file, "graph: {\n");
  fprintf(file, "title: \"%s\"\n", name);
  fprintf(file, "shape: box\n");
  fprintf(file, "color: white\n");
  // Dump all the nodes
  CodeRegion_FOREACH_BasicBlock(this, block) {
    fprintf(file, "node: { title: \"%d\" label: \"BB%d\n ",
            BasicBlock_INDEX(block),BasicBlock_INDEX(block));
      BasicBlock_pretty(block, file);
    fprintf(file, "\"}\n");
    // Now dump all the outgoing edges
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(block, edge) {
        fprintf(file, "edge: { sourcename: \"%d\" targetname: \"%d\" color: blue}\n",
                BasicBlock_INDEX(BasicBlockEdge_origBlock(edge)),
                BasicBlock_INDEX(BasicBlockEdge_destBlock(edge)));
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  } CodeRegion_ENDEACH_BasicBlock;
  fprintf(file, "}\n");
  //
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the CodeRegion module.
 */
#if defined(CodeRegion_BRIGGSGLOBALS)
#define CodeRegion_INIT()
#else
void
CodeRegion_INIT(void);
#endif
#endif//$XCC__h

#ifndef CodeRegion_INIT
void
CodeRegion_INIT(void)
{
#ifndef CodeRegion_BRIGGSGLOBALS
  if (GETENV("CodeRegion_BRIGGSGLOBALS")) {
    sscanf(GETENV("CodeRegion_BRIGGSGLOBALS"), "%d", &CodeRegion_BRIGGSGLOBALS);
    fprintf(stderr, "*** CodeRegion_BRIGGSGLOBALS=%d\n", CodeRegion_BRIGGSGLOBALS);
  }
#endif//CodeRegion_BRIGGSGLOBALS
}
#endif//CodeRegion_INIT

#ifdef $XCC__h
/**
 * Finalize the CodeRegion module.
 */
#define CodeRegion_FINI()
#endif//$XCC__h

#if XCC__C
static void
CodeRegion_TEST(void)
{
#include "LIR/CodeRegion_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(CodeRegion_TEST);
  return 0;
}
#endif

