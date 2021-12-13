#ifdef $XCC_h
/*
 * !!!!	Construction.xcc
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
 * @brief Software pipeline Construction.
 */
#endif//$XCC_h


#include "SCD/SCD_.h"

#ifdef $XCC__h
#include "SCD/Construction.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Implementation of software pipeline construction.
 */
struct Construction_;
  //@args	Memory memory, RegionFlow regionFlow, BlockTrace blockTrace
#endif//$XCC_h

#ifdef $XCC__h
struct Construction_ {
  RegionFlow REGIONFLOW;		// This Construction enclosing Procedure.
  BlockTrace BLOCKTRACE;	// This Construction BlockTrace.
  //@access PROCESSOR	BlockTrace_PROCESSOR(Construction_BLOCKTRACE(this))
  //@access LAMBDA	BlockTrace_LAMBDA(Construction_BLOCKTRACE(this))
  //@access LENGTH	BlockTrace_LENGTH(Construction_BLOCKTRACE(this))
  //@access KUNROLL	BlockTrace_KUNROLL(Construction_BLOCKTRACE(this))
  IDList_ ISSUEITEMS_;		// This Construction IssueItem list.
  //@access ISSUEITEMS	Construction__ISSUEITEMS_(this)
  //@access FIRSTITEM	(IssueItem)IDList_firstItem(Construction_ISSUEITEMS(this))
  //@access LASTITEM	(IssueItem)IDList_lastItem(Construction_ISSUEITEMS(this))
  //@access ITEMCOUNT	IDList_count(Construction_ISSUEITEMS(this))
  //@access MEMORY	IDList_memory(Construction_ISSUEITEMS(this))
  IssueItem_ ENDITEM_;		// The end IssueItem of this Construction.
  //@access ENDITEM	Construction__ENDITEM_(this)
  IDList_ MASTERS_;		// The list of ConstructionMasters of this Construction.
  //@access MASTERS	Construction__MASTERS_(this)
  //@access FIRSTMASTER	(ConstructionMaster)IDList_firstItem(Construction_MASTERS(this))
  //@access LASTMASTER	(ConstructionMaster)IDList_lastItem(Construction_MASTERS(this))
  //@access MASTERCOUNT	IDList_count(Construction_MASTERS(this))
  IDList_ EPILOGS_;		// The list of ConstructionEpilogs of this Construction.
  //@access EPILOGS	Construction__EPILOGS_(this)
  BasicBlock PREDBLOCK;		// Predecessor BasicBlock of the BlockTrace.
  BasicBlock HEADBLOCK;		// Head BasicBlock of the BlockTrace.
  intptr_t TRACEID;		// BasicBlock TRACEID of this Construction.
  int32_t REGIONID;		// REGIONID of the BlockTrace.
  int16_t INSTANCE;		// The instance of the constructed BasicBlock(s).
  float FREQUENCY;		// Frequency the BlockTrace is executed.
};
#endif//$XCC__h

Construction
Construction_Ctor(Construction this,
                  Memory memory, RegionFlow regionFlow, BlockTrace blockTrace)
{
  CFGNode headNode = BlockTrace_HEADNODE(blockTrace);
  CFGNode tailNode = BlockTrace_TAILNODE(blockTrace);
  BasicBlock headBlock = CFGNode_basicBlock(headNode);
  BasicBlock tailBlock = CFGNode_basicBlock(tailNode);
  intptr_t traceId = BasicBlock_traceId(headBlock);
  int32_t regionId = BasicBlock_regionId(headBlock);
  int instance = BasicBlock_instance(headBlock);
  *Construction__REGIONFLOW(this) = regionFlow;
  *Construction__BLOCKTRACE(this) = blockTrace;
  IDList_Ctor(Construction_ISSUEITEMS(this), memory);
  memset(Construction_ENDITEM(this), 0, sizeof(IssueItem_));
  IDList_Ctor(Construction_MASTERS(this), memory);
  IDList_Ctor(Construction_EPILOGS(this), memory);
  *Construction__PREDBLOCK(this) = BasicBlock_prev(headBlock);
  *Construction__HEADBLOCK(this) = headBlock;
  *Construction__TRACEID(this) = traceId;
  *Construction__REGIONID(this) = regionId;
  *Construction__INSTANCE(this) = instance;
  *Construction__FREQUENCY(this) = BlockTrace_getEnterFrequency(blockTrace);
  return this;
}

void
Construction_Dtor(Construction this)
{
  IDList_Dtor(Construction_ISSUEITEMS(this));
  IDList_Dtor(Construction_MASTERS(this));
  IDList_Dtor(Construction_EPILOGS(this));
}

size_t
Construction_Size(Memory memory, RegionFlow regionFlow, BlockTrace blockTrace)
{
        return sizeof(Construction_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Construction_)\t%zu\n", sizeof(Construction_));
}
#endif//$XCC__c

#ifdef $XCC_h
Construction
Construction_make(Memory memory, RegionFlow regionFlow, BlockTrace blockTrace);
#endif//$XCC_h

Construction
Construction_make(Memory memory, RegionFlow regionFlow, BlockTrace blockTrace)
{
  size_t size = Construction_Size(memory, regionFlow, blockTrace);
  Construction this = Memory_alloc(memory, size);
  return Construction_Ctor(this, memory, regionFlow, blockTrace);
}

#ifdef $XCC_h
Construction
Construction_kill(Construction this);
#endif//$XCC_h

Construction
Construction_kill(Construction this)
{
  if (this != NULL) {
    Memory memory = Construction_MEMORY(this);
    Construction_Dtor(this);
    Memory_free_(memory, this);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * For use by Construction_FOREACH_IssueItem.
 */
IDList
Construction_issueItems(Construction this);
#endif//$XCC_h

IDList
Construction_issueItems(Construction this)
{
  return Construction_ISSUEITEMS(this);
}

#ifdef $XCC_h
/**
 * Iterate over this Construction IssueItems.
 */
#define Construction_FOREACH_IssueItem(this, issueItem) \
  IDList_FOREACH(Construction_issueItems(this), IssueItem_, issueItem) {
#define Construction_ENDEACH_IssueItem \
  } IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Construction IssueItems.
 */
#define Construction_FORBACK_IssueItem(this, issueItem) \
  IDList_FORBACK(Construction_issueItems(this), IssueItem_, issueItem) {
#define Construction_ENDBACK_IssueItem \
  } IDList_ENDBACK;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pretty-print this Construction IssueItems.
 */
bool
Construction_prettyIssueItems(Construction this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Construction_prettyIssueItems(Construction this, FILE *file)
{
  int16_t lambda = Construction_LAMBDA(this);
  if (file == NULL) file = stderr;
  Construction_FOREACH_IssueItem(this, item) {
    int instance = IssueItem_INSTANCE(item);
    int controlId = IssueItem_CONTROLID(item);
    int startDate = IssueItem_STARTDATE(item);
    fprintf(file, "    [%d] @%d.%d\t", startDate, instance, controlId);
    IssueItem_pretty(item, file);
  } Construction_ENDEACH_IssueItem;
  return true;
}
#endif//_NTRACE

#ifdef $XCC__h
/**
 * Make a new IssueItem for this Construction.
 *
 * The Operation_issueItem is set to the new IssueItem.
 */
IssueItem
Construction_makeIssueItem(Construction this, IssueItem issueItem);
#endif//$XCC__h

IssueItem
Construction_makeIssueItem(Construction this, IssueItem issueItem)
{
  Operation operation = IssueItem_OPERATION(issueItem);
  Opcode opcode = IssueItem_OPCODE(issueItem);
  size_t size = IssueItem_Size(operation, opcode);
  IssueItem newItem = IDList_push(Construction_ISSUEITEMS(this), size);
  IssueItem_Ctor(newItem, operation, opcode);
  // Ensure issueItem and newItem compare the same.
  *IssueItem__FLAGS(newItem) = IssueItem_FLAGS(issueItem);
  *IssueItem__INSTANCE(newItem) = IssueItem_INSTANCE(issueItem);
  *IssueItem__ORDERING(newItem) = IssueItem_ORDERING(issueItem);
  *IssueItem__STARTDATE(newItem) = IssueItem_STARTDATE(issueItem);
  Except_ENSURE(InstructionTask_compare(issueItem, newItem) == 0);
  // Copy some other issueItem fields too.
  *IssueItem__EXITARC(newItem) = IssueItem_EXITARC(issueItem);
  *IssueItem__FALLARC(newItem) = IssueItem_FALLARC(issueItem);
  *IssueItem__CONTROLID(newItem) = IssueItem_CONTROLID(issueItem);
  if (Operator_isControl(Operation_operator(operation))) {
    *IssueItem__FLAGS(newItem) |= InstructionFlag_Control;
  }
  return newItem;
}   

static IssueItem
Construction_appendIssueItem(Construction this, IssueItem issueItem, Operation operation)
{
  size_t size = IssueItem_Size(operation, Opcode__UNDEF);
  IDList issueItems = Construction_ISSUEITEMS(this);
  IssueItem newItem = IDList_makeItem(issueItems, size);
  IssueItem_Ctor(newItem, operation, Opcode__UNDEF);
  IDList_append_(issueItems, issueItem, newItem);
  if (Operator_isControl(Operation_operator(operation))) {
    *IssueItem__FLAGS(newItem) |= InstructionFlag_Control;
  }
  return newItem;
}

static IssueItem
Construction_putIssueItem(Construction this, Operation operation)
{
  size_t size = IssueItem_Size(operation, Opcode__UNDEF);
  IDList issueItems = Construction_ISSUEITEMS(this);
  IssueItem newItem = IDList_makeItem(issueItems, size);
  IssueItem_Ctor(newItem, operation, Opcode__UNDEF);
  IDList_put_(issueItems, newItem);
  if (Operator_isControl(Operation_operator(operation))) {
    *IssueItem__FLAGS(newItem) |= InstructionFlag_Control;
  }
  return newItem;
}

static inline void
Construction_sortIssueItems(Construction this)
{
  IDList_sort(Construction_ISSUEITEMS(this), InstructionTask_compare);
}

#ifdef $XCC_h
/**
 * For use by Construction_FOREACH_ConstructionEpilog.
 */
IDList
Construction_epilogs(Construction this);
#endif//$XCC_h

IDList
Construction_epilogs(Construction this)
{
  return Construction_EPILOGS(this);
}

static inline int32_t
Construction_countEpilogs(Construction this)
{
  return IDList_count(Construction_EPILOGS(this));
}

#ifdef $XCC_h
/**
 * Iterate over this Construction ConstructionEpilogs.
 */
#define Construction_FOREACH_ConstructionEpilog(this, epilog) \
  IDList_FOREACH(Construction_epilogs(this), ConstructionEpilog_, epilog) {
#define Construction_ENDEACH_ConstructionEpilog \
  } IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Construction ConstructionEpilogs.
 */
#define Construction_FORBACK_ConstructionEpilog(this, epilog) \
  IDList_FORBACK(Construction_epilogs(this), ConstructionEpilog_, epilog) {
#define Construction_ENDBACK_ConstructionEpilog \
  } IDList_ENDBACK;
#endif//$XCC_h

#ifdef $XCC__h
/**
 * Make a new ConstructionEpilog for this Construction.
 */
ConstructionEpilog
Construction_makeEpilog(Construction this, IssueItem issueItem);
#endif//$XCC__h

ConstructionEpilog
Construction_makeEpilog(Construction this, IssueItem issueItem)
{
  size_t size = ConstructionEpilog_Size(this, issueItem);
  ConstructionEpilog epilog = IDList_push(Construction_EPILOGS(this), size);
  ConstructionEpilog_Ctor(epilog, this, issueItem);
  return epilog;
}   

#ifdef $XCC__h
void
Construction_sortEpilogs(Construction this);
#endif//$XCC__h

void
Construction_sortEpilogs(Construction this)
{
  IDList_sort(Construction_EPILOGS(this), ConstructionEpilog_compare);
}

#ifdef $XCC_h
/*
 * ConstructionMasterFlag --      Enumerates the ConstructionMaster flags.
 */
typedef enum {
  ConstructionMasterFlag_Unscheduled = 0x1,    // ConstructionMaster contents are not scheduled.
} ConstructionMasterFlag;
typedef unsigned ConstructionMasterFlags;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by Construction_FOREACH_ConstructionMaster.
 */
IDList
Construction_masters(Construction this);
#endif//$XCC_h

IDList
Construction_masters(Construction this)
{
  return Construction_MASTERS(this);
}

static inline int32_t
Construction_countMasters(Construction this)
{
  return IDList_count(Construction_MASTERS(this));
}

/*
 * Sort this Construction MASTERS in execution order.
 */
static inline void
Construction_sortMasters(Construction this)
{
  IDList_sort(Construction_MASTERS(this), ConstructionMaster_compare);
}

#ifdef $XCC_h
/**
 * Iterate over this Construction ConstructionMasters.
 */
#define Construction_FOREACH_ConstructionMaster(this, master) \
  IDList_FOREACH(Construction_masters(this), ConstructionMaster_, master) {
#define Construction_ENDEACH_ConstructionMaster \
  } IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this Construction ConstructionMasters.
 */
#define Construction_FORBACK_ConstructionMaster(this, master) \
  IDList_FORBACK(Construction_masters(this), ConstructionMaster_, master) {
#define Construction_ENDBACK_ConstructionMaster \
  } IDList_ENDBACK;
#endif//$XCC_h

#ifdef $XCC__h
/**
 * Make a new ConstructionMaster for this Construction.
 */
void
Construction_makeMaster(Construction this, IssueItem issueItem, BasicBlock basicBlock);
#endif//$XCC__h

void
Construction_makeMaster(Construction this, IssueItem issueItem, BasicBlock basicBlock)
{
  CFGArc exitArc = IssueItem_EXITARC(issueItem);
  size_t size = ConstructionMaster_Size(issueItem, basicBlock);
  ConstructionMaster master = IDList_push(Construction_MASTERS(this), size);
  ConstructionMaster_Ctor(master, issueItem, basicBlock);
  if (exitArc != NULL) {
    ConstructionEpilog epilog = Construction_makeEpilog(this, issueItem);
    *ConstructionMaster__EPILOG(master) = epilog;
  }
}

static Operation
Construction_replicateOperation(Construction this, const_Operation this_operation)
{
  BlockTrace blockTrace = Construction_BLOCKTRACE(this);
  EffectTable effectTable = BlockTrace_EFFECTTABLE(blockTrace);
  RegionFlow regionFlow = Construction_REGIONFLOW(this);
  CodeRegion codeRegion = RegionFlow_codeRegion(regionFlow);
  Selector selector = CodeRegion_selector(codeRegion);
  Operation that_operation = Selector_replicateOperation(selector, this_operation);
  Operation_setInstance(that_operation, Operation_instance(this_operation));
  Operation_setBoosting(that_operation, Operation_boosting(this_operation));
  Selector_detachOperation(selector, that_operation);
  EffectTable_enterOperation(effectTable, that_operation, false);
  {
    Effect_ *this_argEffects = Operation_argEffects(this_operation);
    Effect_ *this_resEffects = Operation_resEffects(this_operation);
    Effect_ *that_argEffects = Operation_argEffects(that_operation);
    Effect_ *that_resEffects = Operation_resEffects(that_operation);
    Operation_ARGUMENTS_FOREACH_Temporary(this_operation, temporary) {
      *Effect__LIFETIME(that_argEffects + Operation_ARGUMENTS_INDEX) =
          Effect_LIFETIME(this_argEffects + Operation_ARGUMENTS_INDEX);
    } Operation_ARGUMENTS_ENDEACH_Temporary;
    Operation_RESULTS_FOREACH_Temporary(this_operation, temporary) {
      *Effect__LIFETIME(that_resEffects + Operation_RESULTS_INDEX) =
          Effect_LIFETIME(this_resEffects + Operation_RESULTS_INDEX);
    } Operation_RESULTS_ENDEACH_Temporary;
  }
  return that_operation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Main entry point.
 */
void
Construction_construct(Construction this, bool anyExit);
#endif//$XCC_h

void
Construction_construct(Construction this, bool anyExit)
{
  int lambda = Construction_LAMBDA(this);
  BlockTrace blockTrace = Construction_BLOCKTRACE(this);
  // Sort the BlockTrace IssueItems after scheduling.
  BlockTrace_sortIssueItems(blockTrace);
  SCD_TRACE && fprintf(SCD_TRACE, "*** LOCAL SCHEDULE:\tLAMBDA=%d\n",
      BlockTrace_LAMBDA(blockTrace));
  SCD_TRACE && BlockTrace_prettyIssueItems(blockTrace, SCD_TRACE);
  // Construct the instruction schedule.
  BlockTrace_computeKunroll(blockTrace, lambda);
  Construction_makeEndItem(this, anyExit);
  Construction_buildMaster(this);
  if (Construction_KUNROLL(this) > 0) {
    LoopScope loopScope = BlockTrace_LOOPSCOPE(blockTrace);
    LoopScope_updateUnrolled(loopScope, Construction_KUNROLL(this));
    Construction_splitMaster(this);
    Construction_sortMasters(this);
    Construction_sortEpilogs(this);
    Construction_fillEpilogs(this);
    Construction_completeEpilogs(this);
    Construction_patchOperations(this);
    Construction_moveRegisters(this);
    Construction_fixCFGraph(this);
    Construction_fixFrequencies(this);
  } else {
    Construction_sortMasters(this);
    Construction_patchOperations(this);
  }
  Construction_fillBasicBlocks(this);
  Construction_fixBasicBlocks(this);
  Construction_fixSpeculated(this);
}

#ifdef $XCC__h
/**
 * Step 1 of "A Unified Software Pipeline
 * Construction Scheme for Modulo Scheduler Loops", PaCT'97.
 *
 * The purpose of Step 1 is to compute endItem, the IssueItem at the end of the
 * master schedule. This is the earliest shifted copy of an exit branch that is
 * not before the last in the local schedule. In case of software pipelined
 * hardware loops, the kernel boundaries can be any operation  (+anyExit+ true).
 */
void
Construction_makeEndItem(Construction this, bool anyExit);
#endif//$XCC__h

void
Construction_makeEndItem(Construction this, bool anyExit)
{
  int lambda = Construction_LAMBDA(this);
  BlockTrace blockTrace = Construction_BLOCKTRACE(this);
  CFGNode headNode = BlockTrace_HEADNODE(blockTrace);
  CFGNode tailNode = BlockTrace_TAILNODE(blockTrace);
  IssueItem lastItem = BlockTrace_LASTITEM(blockTrace);
  int kunroll = Construction_KUNROLL(this);
  IssueItem endItem = NULL;
  if (kunroll > 0 && !anyExit) {
    // Compute this Construction_ENDITEM.
    bool noEndItem = true;
    IssueItem cutItem = IssueItem_PREV(lastItem);
    // Loop over the EXITITEMS.
    BlockTrace_FOREACH_IssueItem(blockTrace, ITEM) {
      if (IssueItem_EXITARC(ITEM) != NULL) {
        IssueItem_ exit_item_ = *ITEM, *exit_item = &exit_item_;
        while (InstructionTask_compare(exit_item, cutItem) < 0) {
          *IssueItem__STARTDATE(exit_item) += lambda;
          ++*IssueItem__INSTANCE(exit_item);
        }
        // Found the earliest shifted exit item not lower than cutItem.
        if (endItem == NULL) {
          endItem = Construction_ENDITEM(this);
          *endItem = *exit_item;
        } else if (InstructionTask_compare(exit_item, endItem) < 0) {
          // Keep the earliest of all the shifted exit items as the endItem.
          *endItem = *exit_item;
        }
      }
    } BlockTrace_ENDEACH_IssueItem;
    // Adjust for kernel unrolling.
    *IssueItem__STARTDATE(endItem) += (kunroll - 1)*lambda;
    *IssueItem__INSTANCE(endItem) += kunroll - 1;
    *IssueItem__EXITARC(endItem) = NULL;
  } else {
    // In this case, just pick the last of the local schedule.
    endItem = Construction_ENDITEM(this);
    *endItem = *lastItem;
  }
  SCD_TRACE && fprintf(SCD_TRACE, "*** ENDITEM:\tdate=%d\tinstance=%d",
      IssueItem_STARTDATE(endItem), IssueItem_INSTANCE(endItem));
  SCD_TRACE && Operation_pretty(IssueItem_OPERATION(endItem), SCD_TRACE);
}

#ifdef $XCC__h
/**
 * Step 2 of "A Unified Software Pipeline
 * Construction Scheme for Modulo Scheduler Loops", PaCT'97.
 *
 * The purpose of Step 2 is to create the master schedule. We omit the first and
 * the last operations from the replication process, as these operations are
 * scheduling anchors that should exist only once in the master schedule. Also
 * after Step2, no operation remains inside a BasicBlock, and the BasicBlock(s)
 * to be reused for the schedule kernel are into Construction_MASTERS.
 */
void
Construction_buildMaster(Construction this);
#endif//$XCC__h

void
Construction_buildMaster(Construction this)
{
  int lambda = Construction_LAMBDA(this);
  int kunroll = Construction_KUNROLL(this);
  Processor processor = Construction_PROCESSOR(this);
  RegionFlow regionFlow = Construction_REGIONFLOW(this);
  BlockTrace blockTrace = Construction_BLOCKTRACE(this);
  EffectTable effectTable = BlockTrace_EFFECTTABLE(blockTrace);
  IssueItem firstItem = BlockTrace_FIRSTITEM(blockTrace);
  IssueItem lastItem = BlockTrace_LASTITEM(blockTrace);
  IssueItem endItem = Construction_ENDITEM(this);
  BasicBlock basicBlock = NULL;
  // Main replication loop.
  BlockTrace_FOREACH_IssueItem(blockTrace, ITEM) {
    IssueItem_ temp_item_ = *ITEM, *temp_item = &temp_item_;
    Operation operation = IssueItem_OPERATION(ITEM);
    basicBlock = Operation_basicBlock(operation);
    if (basicBlock != NULL) {
      Selector selector = BasicBlock_selector(basicBlock);
      Selector_detachOperation(selector, operation);
    }
    if (ITEM == firstItem) {
      IssueItem first_item = Construction_makeIssueItem(this, firstItem);
    } else if (ITEM != lastItem) {
      do {
        IssueItem new_item = NULL;
        Operation master_operation = operation;
        if (IssueItem_INSTANCE(temp_item) != 0) {
          // Replicate the operation.
          master_operation = Construction_replicateOperation(this, operation);
          Except_CHECK(Operation_index(master_operation) == Operation_index(operation));
          Except_CHECK(kunroll > 0);
        }
        // Fill the master schedule with master_operation.
        *IssueItem__OPERATION(temp_item) = master_operation;
        new_item = Construction_makeIssueItem(this, temp_item);
        if (Operator_isControl(Operation_operator(operation))) {
          Construction_makeMaster(this, new_item, basicBlock);
        }
        // Replication for the next software pipeline stage.
        if (kunroll > 0) {
          *IssueItem__STARTDATE(temp_item) += lambda;
          ++*IssueItem__INSTANCE(temp_item);
        } else break;
      } while (InstructionTask_compare(temp_item, endItem) <= 0);
    }
  } BlockTrace_ENDEACH_IssueItem;
  // Sort the master schedule.
  Construction_sortIssueItems(this);
  // Add the last operation to the master schedule if required.
  if (kunroll <= 0) {
    IssueItem last_item = Construction_makeIssueItem(this, lastItem);
    Construction_makeMaster(this, last_item, basicBlock);
  } else {
    // Software pipelining.
    Procedure procedure = RegionFlow_procedure(regionFlow);
    const_Operation last_operation = IssueItem_OPERATION(lastItem);
    if (Operator_isLoop(Operation_operator(last_operation))) {
      // Do not replicate the LOOP and invert the previous branch.
      IssueItem goto_item = Construction_LASTITEM(this);
      Operation goto_operation = IssueItem_OPERATION(goto_item);
      const_Operation label_operation = IssueItem_OPERATION(firstItem);
      Label label = Operation_getTargetLabel(label_operation);
      Except_CHECK(Operator_isGoTo(Operation_operator(goto_operation)));
      // EffectTable_releaseOperation(effectTable, goto_operation);
      Operation_setMainEffects(goto_operation, NULL);
      // FIXME! Hack in case inverting the GoTo is made by changing a Modifier Temporary.
      // FIXME! Assume this is the only place where Modifier Temporary must no be shared.
      {
        Temporary argument_0 = Operation_getArgument(goto_operation, 0);
        if (Temporary_isModifier(argument_0)) {
          ModifierMember member = Temporary_MEMBER(argument_0);
          Temporary modifier_temporary = Procedure_makeModifierTemporary(procedure, member);
          Operation_setArgument(goto_operation, 0, modifier_temporary);
        }
      }
      if (Operation_tryInvertGoTo(goto_operation, processor)) {
        Procedure_updateTargetedOperation(procedure, goto_operation, label);
      } else Except_NEVER(true);
    } else {
      IssueItem last_item = Construction_makeIssueItem(this, lastItem);
      *IssueItem__STARTDATE(last_item) = IssueItem_STARTDATE(endItem) + kunroll*lambda;
      Construction_makeMaster(this, last_item, basicBlock);
    }
  }
}

#ifdef $XCC__h
/**
 * Split this Construction MASTERS.
 *
 * The limit item between the prolog and the kernel is the down-shifted copy by
 * kunroll of the ENDITEM.
 */
void
Construction_splitMaster(Construction this);
#endif//$XCC__h

void
Construction_splitMaster(Construction this)
{
  int lambda = Construction_LAMBDA(this);
  int kunroll = Construction_KUNROLL(this), controlId = 0;
  RegionFlow regionFlow = Construction_REGIONFLOW(this);
  IssueItem label_item = Construction_FIRSTITEM(this);
  const_Operation label_operation = IssueItem_OPERATION(label_item);
  BlockTrace blockTrace = Construction_BLOCKTRACE(this);
  IssueItem endItem = Construction_ENDITEM(this);
  IssueItem ITEM = label_item, prev_item = NULL;
  // Compute the limit item between the prolog and the kernel.
  IssueItem_ limit_item_ = *endItem, *limit_item = &limit_item_;
  *IssueItem__STARTDATE(limit_item) -= kunroll*lambda;
  *IssueItem__INSTANCE(limit_item) -= kunroll;
  SCD_TRACE && fprintf(SCD_TRACE, "*** CUTITEM:\tdate=%d\tinstance=%d",
      IssueItem_STARTDATE(limit_item), IssueItem_INSTANCE(limit_item));
  SCD_TRACE && Operation_pretty(IssueItem_OPERATION(limit_item), SCD_TRACE);
  Except_CHECK(Operator_isLabel(Operation_operator(label_operation)));
  // Loop to find the first IssueItem in the kernel.
  for (; ITEM != NULL; ITEM = IssueItem_NEXT((prev_item = ITEM))) {
    if (IssueItem_isControl(ITEM)) controlId = IssueItem_CONTROLID(ITEM);
    if (InstructionTask_compare(ITEM, limit_item) > 0) break;
    *IssueItem__FLAGS(ITEM) |= InstructionFlag_InProlog;
  }
  // Here prev_item is the last in the prolog.
  if (prev_item != NULL && !IssueItem_isControl(prev_item)) {
    // Insert a FALL operation to separate the prolog from the kernel.
    ConstructionMaster firstMaster = Construction_FIRSTMASTER(this);
    BasicBlock first_block = ConstructionMaster_BASICBLOCK(firstMaster);
    Selector first_selector = BasicBlock_selector(first_block);
    Label label = Operation_getTargetLabel(label_operation);
    Operation fall_operation = Selector_makeOperationFALL(first_selector, label);
    IssueItem fall_item = Construction_appendIssueItem(this, prev_item, fall_operation);
    Selector_detachOperation(first_selector, fall_operation);
    *IssueItem__STARTDATE(fall_item) = IssueItem_STARTDATE(prev_item);
    *IssueItem__FLAGS(fall_item) |= InstructionFlag_InProlog;
    *IssueItem__CONTROLID(fall_item) = controlId;
    Construction_makeMaster(this, fall_item, first_block);
  }
  if (kunroll > 1) {
    // Set the limit item between the first and the second kernel stage.
    *IssueItem__STARTDATE(limit_item) += lambda;
    ++*IssueItem__INSTANCE(limit_item);
    // Loop to find the first kernel unrolled IssueItem.
    for (; ITEM != NULL; ITEM = IssueItem_NEXT(ITEM)) {
      if (InstructionTask_compare(ITEM, limit_item) > 0) break;
    }
    // Loop to flag the kernel unrolled IssueItems.
    for (; ITEM != NULL; ITEM = IssueItem_NEXT(ITEM)) {
      *IssueItem__FLAGS(ITEM) |= InstructionFlag_Kunrolled;
    }
  }
}

#ifdef $XCC__h
/**
 * Step 3 of "A Unified Software Pipeline
 * Construction Scheme for Modulo Scheduler Loops", PaCT'97.
 *
 * The purpose of Step 3 is to fill the epilogs with operations.
 * The first difference with the paper is that we iterate over the master
 * schedule items, instead of bumping the INSTANCE and PHI of a temporary
 * IssueItem. The second difference is that the two loops are inverted.
 */
void
Construction_fillEpilogs(Construction this);
#endif//$XCC__h

void
Construction_fillEpilogs(Construction this)
{
  RegionFlow regionFlow = Construction_REGIONFLOW(this);
  BlockTrace blockTrace = Construction_BLOCKTRACE(this);
  EffectTable effectTable = BlockTrace_EFFECTTABLE(blockTrace);
  Construction_FOREACH_IssueItem(this, ITEM) {
    const_Operation operation = IssueItem_OPERATION(ITEM);
    Construction_FOREACH_ConstructionEpilog(this, epilog) {
      IssueItem epilog_item = ConstructionEpilog_ISSUEITEM(epilog);
      if (IssueItem_INSTANCE(ITEM) < IssueItem_INSTANCE(epilog_item) ||
          (IssueItem_INSTANCE(ITEM) == IssueItem_INSTANCE(epilog_item) &&
            IssueItem_ORDERING(ITEM) < IssueItem_ORDERING(epilog_item))) {
        // ITEM is executable in the current epilog.
        if (InstructionTask_compare(ITEM, epilog_item) > 0) {
          // ITEM is after epilog_item in the master schedule.
          // Push a replicate of the operation onto the epilog.
          Operation epilog_operation = Construction_replicateOperation(this, operation);
          ConstructionEpilog_pushOperation(epilog, epilog_operation);
        }
      }
    } Construction_ENDEACH_ConstructionEpilog;
  } Construction_ENDEACH_IssueItem;
}

#ifdef $XCC__h
/**
 * Complete this Construction EPILOGS.
 */
void
Construction_completeEpilogs(Construction this);
#endif//$XCC__h

void
Construction_completeEpilogs(Construction this)
{
  int kunroll = Construction_KUNROLL(this);
  ConstructionMaster firstMaster = Construction_FIRSTMASTER(this);
  ConstructionMaster lastMaster = Construction_LASTMASTER(this);
  CFGNode firstNode = ConstructionMaster_CFGNODE(firstMaster);
  CFGNode lastNode = ConstructionMaster_CFGNODE(lastMaster);
  Construction_FOREACH_ConstructionEpilog(this, epilog) {
    CFGNode cfgNode = ConstructionEpilog_CFGNODE(epilog);
    if (cfgNode == firstNode) {
      //TODO
    }
  } Construction_ENDEACH_ConstructionEpilog;
}

#ifdef $XCC__h
/**
 * Patch the operation arguments and results using
 * modulo renaming and inductive relaxation, as per "A Unified Software Pipeline
 * Construction Scheme for Modulo Scheduler Loops", PaCT'97.
 */
void
Construction_patchOperations(Construction this);
#endif//$XCC__h

void
Construction_patchOperations(Construction this)
{
  RegionFlow regionFlow = Construction_REGIONFLOW(this);
  Procedure procedure = RegionFlow_procedure(regionFlow);
  Construction_FOREACH_IssueItem(this, ITEM) {
    int instance = IssueItem_INSTANCE(ITEM);
    const_Operation operation = IssueItem_OPERATION(ITEM);
    if (Operation_mainEffects(operation) != NULL) {
      Operation_ARGUMENTS_FOREACH_Effect(operation, argEffect) {
        Lifetime lifetime = Effect_LIFETIME(argEffect);
        if (lifetime != NULL) {
          Lifetime_fixArgument(lifetime, procedure, argEffect, instance);
        }
        *Effect__LIFETIME(argEffect) = NULL;
      } Operation_ARGUMENTS_ENDEACH_Effect;
      Operation_RESULTS_FOREACH_Effect(operation, resEffect) {
        Lifetime lifetime = Effect_LIFETIME(resEffect);
        if (lifetime != NULL) {
          Lifetime_fixResult(lifetime, procedure, resEffect, instance);
        }
        *Effect__LIFETIME(resEffect) = NULL;
      } Operation_RESULTS_ENDEACH_Effect;
    }
  } Construction_ENDEACH_IssueItem;
}

#ifdef $XCC__h
/**
 * Move the registers after modulo renaming, as per
 * "A Unified Software Pipeline Construction Scheme for Modulo Scheduler Loops", PaCT'97.
 *
 * Compared to the paper, we extended algorithm to work with counted hardware
 * loops. We also fixed two bugs:
 * * The renaming  register used by the MOVE operation into the register live
 * on exit is indexed using the instance number of the exit branch minus its
 * Carried value.
 * * In case of a live on exit renamed register, only the lifetime that
 * contains the indirect use needs to generate the MOVE.
 */
void
Construction_moveRegisters(Construction this);
#endif//$XCC__h

void
Construction_moveRegisters(Construction this)
{
  RegionFlow regionFlow = Construction_REGIONFLOW(this);
  CodeRegion codeRegion = RegionFlow_codeRegion(regionFlow);
  Selector selector = CodeRegion_selector(codeRegion);
  int kunroll = Construction_KUNROLL(this);
  if (kunroll > 0) {
    BlockTrace blockTrace = Construction_BLOCKTRACE(this);
    CFGNode headNode = BlockTrace_headNode(blockTrace);
    ConstructionMaster firstMaster = Construction_FIRSTMASTER(this);
    BasicBlock firstBlock = ConstructionMaster_BASICBLOCK(firstMaster);
    IssueItem prologItem = IssueItem_PREV(ConstructionMaster_ISSUEITEM(firstMaster));
    LifetimeTable lifetimeTable = BlockTrace_LIFETIMETABLE(blockTrace);
    LifetimeTable_FOREACH_Lifetime(lifetimeTable, LIFETIME) {
      Temporary old_temporary = Lifetime_TEMPORARY(LIFETIME);
      if (Lifetime_hasRenaming(LIFETIME)) {
        // If this lifetime contains an exposed use that could be reached from both
        // outside and inside the loop:
        // - If it is reached from outside the loop, then it is reached by the original
        // temporary,
        // - If it is reached from inside the loop then it is reached by the
        // temporary for instance -1.
        // If these are not the same temporary, then insert a copy from the original
        // temporary to the new temporary.
        // the copy must be in the prolog before any possible use of the new_temporary.
        // Sufficient conditions for an exposed use are:
        // - the register is live on entry to the loop, and
        // - the lifetime does not have a def that dominates all uses in the loop.
        if (CFGNode_isLiveInSet(headNode, old_temporary)
            && !Lifetime_isReadAfter(LIFETIME)) {
          Temporary new_temporary = Lifetime_getRename(LIFETIME, -1);
          if (new_temporary != old_temporary) {
            // Put a MOVE new_temporary <- old_temporary in the prolog code block.
            IssueItem copyItem;
            Selector_selectCOPY(selector, new_temporary, old_temporary);
            // Care, because a copy may be more than one operation.
            while (!Selector_isEmpty(selector)) {
              Operation copyOperation = Selector_detachFirst(selector);
              IssueItem copyItem = Construction_putIssueItem(this, copyOperation);
              // [SC] Worry here that we cannot issue an instruction at this date,
              // because we are inserting instructions in already scheduled code;
              // so mark the prolog as unscheduled.
              *IssueItem__STARTDATE(copyItem) = 0;
              *IssueItem__FLAGS(copyItem) |= InstructionFlag_InProlog;
              *IssueItem__CONTROLID(copyItem) = IssueItem_CONTROLID(prologItem);
              prologItem = copyItem;
              SCD_TRACE && fprintf(SCD_TRACE, "*** PROLOG move inserted ");
              SCD_TRACE && Operation_pretty(copyOperation, SCD_TRACE);
            }
            *ConstructionMaster__FLAGS(firstMaster) |= ConstructionMasterFlag_Unscheduled;
          }
        }
        Lifetime_FOREACH_Effect(LIFETIME, EFFECT) {
          if (Effect_isIndirect(EFFECT)) {
            int index = Operation_index(Effect_OPERATION(EFFECT));
            Construction_FOREACH_ConstructionEpilog(this, epilog) {
              IssueItem epilogItem = ConstructionEpilog_ISSUEITEM(epilog);
              if (Operation_index(IssueItem_OPERATION(epilogItem)) == index) {
                int instance = IssueItem_INSTANCE(epilogItem) - Effect_isCarried(EFFECT);
                Temporary new_temporary = Lifetime_getRename(LIFETIME, instance);
                if (new_temporary != old_temporary) {
                  // Add a MOVE old_temporary <- new_temporary to the epilog code block.
                  Selector epilog_selector = ConstructionEpilog_SELECTOR(epilog);
                  Selector_selectCOPY(selector, old_temporary, new_temporary);
                  while (!Selector_isEmpty(selector)) {
                    Operation copyOperation = Selector_detachFirst(selector);
                    Selector_attachLast(epilog_selector, copyOperation);
                  }
                }
              }
            } Construction_ENDEACH_ConstructionEpilog;
          }
        } Lifetime_ENDEACH_Effect;
      }
    } LifetimeTable_ENDEACH_Lifetime;
  }
}

#ifdef $XCC__h
/**
 * Fix the CFGraph for this Construction.
 *
 * Let headNode and tailNode be the head and the tail of the BlockTrace. Let
 * firstNode and lastNode be the first and the last nodes of the master schedule.
 * When not software pipelining: headNode == firstNode. When software pipelining:
 * firstNode is either the first prolog node, if firstNode != headNode, or is the
 * first kernel node in case there is no prolog; headNode is the first kernel
 * node; lastNode is the last kernel node. Note that, unlike headNode, tailNode
 * is not guaranteed to be used in the master schedule.
 */
void
Construction_fixCFGraph(Construction this);
#endif//$XCC__h

void
Construction_fixCFGraph(Construction this)
{
  Memory memory = Construction_MEMORY(this);
  int32_t regionId = Construction_REGIONID(this);
  Processor processor = Construction_PROCESSOR(this);
  RegionFlow regionFlow = Construction_REGIONFLOW(this);
  CodeRegion codeRegion = RegionFlow_codeRegion(regionFlow);
  Procedure procedure = RegionFlow_procedure(regionFlow);
  CFGraph cfgraph = RegionFlow_cfgraph(regionFlow);
  BlockTrace blockTrace = Construction_BLOCKTRACE(this);
  CFGNode headNode = BlockTrace_HEADNODE(blockTrace), next_node = headNode;
  ConstructionMaster firstMaster = Construction_FIRSTMASTER(this);
  BasicBlock firstBlock = ConstructionMaster_BASICBLOCK(firstMaster);
  Selector firstSelector = BasicBlock_selector(firstBlock);
  CFGNodeSet_ cfgNodes_, *cfgNodes = &cfgNodes_;
  CFGNodeSet_Ctor(cfgNodes, memory, Construction_MASTERCOUNT(this));
  // Create and attach the CFGNodes and CFGArcs.
  {
    SCD_TRACE && fprintf(SCD_TRACE, "*** EPILOG BLOCKS:\n");
    Construction_FOREACH_ConstructionEpilog(this, epilog) {
      IssueItem epilog_item = ConstructionEpilog_ISSUEITEM(epilog);
      // If this epilog is reached from the last item of the master schedule,
      // then we can fall through to it.  Otherwise, we must explicitly branch
      // to the epilog.
      bool fall_through = (epilog_item == Construction_LASTITEM(this));
      Selector selector = ConstructionEpilog_SELECTOR(epilog);
      if (!Selector_isEmpty(selector)) {
        CFGNode node = ConstructionEpilog_CFGNODE(epilog);
        BasicBlock block = CFGNode_BASICBLOCK(node);
        Label label = BasicBlock_label(block);
        float frequency = BasicBlock_FREQUENCY(block);
        BasicBlock new_block = CodeRegion_makeBasicBlock(codeRegion, frequency);
        Label new_label = BasicBlock_makeLabel(new_block);
        CFGNode new_node = CFGraph_makeNode(cfgraph, new_block);
        CFGArcFlags new_flags = 0;
        CFGArc new_arc = CFGraph_makeArc(cfgraph, new_node, node, 1.0, new_flags);
        LoopScope loop_scope = LoopScope_PARENT(BlockTrace_LOOPSCOPE(blockTrace));
        Operation gotoOperation;
        BasicBlock_setLoopScope(new_block, loop_scope);
        *ConstructionEpilog__CFGNODE(epilog) = new_node;
        Selector_selectGOTO(firstSelector, label);
        gotoOperation = Selector_detachLast(firstSelector);
        Selector_attachLast(selector, gotoOperation);
        if (fall_through) {
          next_node = new_node;
        } else {
          Operation branch_operation = IssueItem_OPERATION(epilog_item);
          Procedure_updateTargetedOperation(procedure, branch_operation, new_label);
        }
        SCD_TRACE && ConstructionEpilog_pretty(epilog, SCD_TRACE);
      }
    } Construction_ENDEACH_ConstructionEpilog;
  }
  SCD_TRACE && fprintf(SCD_TRACE, "*** MASTER BLOCKS:\n");
  Construction_FORBACK_ConstructionMaster(this, master) {
    IssueItem issueItem = ConstructionMaster_ISSUEITEM(master);
    CFGArc exitArc = ConstructionMaster_EXITARC(master);
    CFGArc fallArc = ConstructionMaster_FALLARC(master);
    CFGNode cfgNode = ConstructionMaster_CFGNODE(master);
    if (IssueItem_isInProlog(issueItem) || IssueItem_isKunrolled(issueItem)) {
      // Must create the ConstructionMaster BASICBLOCK, CFGNODE, EXITARC, FALLARC.
      BasicBlock basicBlock = CodeRegion_makeBasicBlock(codeRegion, 0.0);
      cfgNode = CFGraph_makeNode(cfgraph, basicBlock);
      *ConstructionMaster__BASICBLOCK(master) = basicBlock;
      *ConstructionMaster__CFGNODE(master) = cfgNode;
      // Update the BlockTrace CFGNODES for later use during Construction.
      if (exitArc != NULL) {
        CFGArcFlags flags = 0;
        ConstructionEpilog epilog = ConstructionMaster_EPILOG(master);
        CFGNode exit_node = ConstructionEpilog_CFGNODE(epilog);
        float probability = CFGArc_probability(exitArc);
        exitArc = CFGraph_makeArc(cfgraph, cfgNode, exit_node, probability, flags);
      }
      if (next_node != NULL) {
        CFGArcFlags flags = CFGArcFlag_Fall;
        float probability = fallArc != NULL? CFGArc_probability(fallArc): 1.0;
        fallArc = CFGraph_makeArc(cfgraph, cfgNode, next_node, probability, flags);
      }
      *ConstructionMaster__EXITARC(master) = exitArc;
      *ConstructionMaster__FALLARC(master) = fallArc;
    } else {
      // In the first kernel stage: attach the heads of the EXITARCs.
      if (exitArc != NULL) {
        ConstructionEpilog epilog = ConstructionMaster_EPILOG(master);
        CFGNode exit_node = ConstructionEpilog_CFGNODE(epilog);
        Except_CHECK(CFGArc_TAIL(exitArc) == cfgNode);
        if (CFGArc_HEAD(exitArc) != exit_node) {
          CFGArc_detachHeadNode(exitArc);
          CFGArc_attachHeadNode(exitArc, exit_node);
        }
      }
      // Attach the heads of the FALLARCs.
      if (next_node != NULL) {
        if (CFGArc_HEAD(fallArc) != next_node) {
          CFGArc_detachHeadNode(fallArc);
          CFGArc_attachHeadNode(fallArc, next_node);
        }
      }
    }
    CFGNodeSet_insert(cfgNodes, cfgNode);
    next_node = cfgNode;
    SCD_TRACE && ConstructionMaster_pretty(master, SCD_TRACE);
  } Construction_ENDBACK_ConstructionMaster;
  // Reroute the CFGArcs entering from outside into the HEADNODE.
  if (headNode != next_node) {
    CFGNode_FORENTER_CFGArc(headNode, arc) {
      CFGNode tail_node = CFGArc_TAIL(arc);
      if (!CFGNodeSet_contains(cfgNodes, tail_node)) {
        Operation goto_operation;
        CFGArc_detachHeadNode(arc);
        CFGArc_attachHeadNode(arc, next_node);
        goto_operation = BasicBlock_getControlOperation(CFGNode_basicBlock(tail_node));
        if (goto_operation != NULL) {
          // FdF 20070523: Must check that target(goto_operation) ==
          // headNode
          Label goto_label = Operation_getTargetLabel(goto_operation);
          if (BasicBlock_hasLabel(CFGNode_basicBlock(headNode), goto_label)) {
            Label label = BasicBlock_makeLabel(CFGNode_basicBlock(next_node));
            Procedure procedure = RegionFlow_procedure(regionFlow);
            Procedure_updateTargetedOperation(procedure, goto_operation, label);
          }
        }
      }
    } CFGNode_ENDENTER_CFGArc;
  }
  // Detach the BlockTrace CFGNode(s) that are left over.
  BlockTrace_FOREACH_CFGNode(blockTrace, cfgNode) {
    if (!CFGNodeSet_contains(cfgNodes, cfgNode)) {
      SCD_TRACE && fprintf(SCD_TRACE, "*** LEFTOVER node %d:", CFGNode_INDEX(cfgNode));
      CFGNode_FORENTER_CFGArc(cfgNode, arc) {
        SCD_TRACE && fprintf(SCD_TRACE, "\t<-%d:", CFGNode_INDEX(CFGArc_TAIL(arc)));
        CFGArc_detachTailNode(arc);
      } CFGNode_ENDENTER_CFGArc;
      CFGNode_FORLEAVE_CFGArc(cfgNode, arc) {
        SCD_TRACE && fprintf(SCD_TRACE, "\t->%d:", CFGNode_INDEX(CFGArc_HEAD(arc)));
        CFGArc_detachHeadNode(arc);
      } CFGNode_ENDLEAVE_CFGArc;
      SCD_TRACE && fprintf(SCD_TRACE, "\n");
    }
  } BlockTrace_ENDEACH_CFGNode;
  //
  CFGNodeSet_Dtor(cfgNodes);
}

#ifdef $XCC__h
/**
 * Fix the CFGNode frequencies for this Construction.
 */
void
Construction_fixFrequencies(Construction this);
#endif//$XCC__h

void
Construction_fixFrequencies(Construction this)
{
  float frequency = Construction_FREQUENCY(this);
  Construction_FOREACH_ConstructionMaster(this, master) {
    IssueItem issueItem = ConstructionMaster_ISSUEITEM(master);
    CFGNode cfgNode = ConstructionMaster_CFGNODE(master);
    CFGArc fallArc = ConstructionMaster_FALLARC(master);
    BasicBlock basicBlock = CFGNode_basicBlock(cfgNode);
    if (IssueItem_isInProlog(issueItem) || IssueItem_isKunrolled(issueItem)) {
      BasicBlock_setFrequency(basicBlock, frequency);
    } else {
      frequency = BasicBlock_frequency(basicBlock);
    }
    frequency *= CFGArc_probability(fallArc);
  } Construction_ENDEACH_ConstructionMaster;
}

#ifdef $XCC__h
/**
 * Fill the BasicBlock(s) of the master schedule.
 */
void
Construction_fillBasicBlocks(Construction this);
#endif//$XCC__h

void
Construction_fillBasicBlocks(Construction this)
{
  int firstDate = -1;
  int kunroll = Construction_KUNROLL(this);
  int16_t lambda = Construction_LAMBDA(this);
  RegionFlow regionFlow = Construction_REGIONFLOW(this);
  Procedure procedure = RegionFlow_procedure(regionFlow);
  BlockTrace blockTrace = Construction_BLOCKTRACE(this);
  ConstructionMaster master = Construction_FIRSTMASTER(this);
  BasicBlock headBlock = Construction_HEADBLOCK(this);
  BasicBlock predBlock = Construction_PREDBLOCK(this);
  Processor processor = Construction_PROCESSOR(this);
  BasicBlock previous_block = predBlock;
  // Unchain all the former BasicBlock(s).
  BlockTrace_FOREACH_CFGNode(blockTrace, cfgNode) {
    BasicBlock basicBlock = CFGNode_basicBlock(cfgNode);
    if (predBlock == NULL && basicBlock == headBlock) {
      previous_block = headBlock;
    } else {
      Procedure_unchainBasicBlock(procedure, basicBlock);
    }
  } BlockTrace_ENDEACH_CFGNode;
  // Chain all the current BasicBlock(s).
  Construction_FOREACH_ConstructionMaster(this, master) {
    BasicBlock basicBlock = ConstructionMaster_BASICBLOCK(master);
    if (basicBlock != previous_block) {
      Procedure_chainBasicBlocks(procedure, previous_block, basicBlock);
    }
    ASSERT(BasicBlock_isEmpty(basicBlock));
    previous_block = basicBlock;
  } Construction_ENDEACH_ConstructionMaster;
  Construction_FORBACK_ConstructionEpilog(this, epilog) {
    Selector selector = ConstructionEpilog_SELECTOR(epilog);
    if (!Selector_isEmpty(selector)) {
      CFGNode node = ConstructionEpilog_CFGNODE(epilog);
      BasicBlock basicBlock = CFGNode_BASICBLOCK(node);
      Procedure_chainBasicBlocks(procedure, previous_block, basicBlock);
      previous_block = basicBlock;
    }
  } Construction_ENDBACK_ConstructionEpilog;
  // Scan the master schedule and fill its BasicBlock(s).
  Construction_FOREACH_IssueItem(this, ITEM) {
    Operation operation = IssueItem_OPERATION(ITEM);
    BasicBlock basicBlock = ConstructionMaster_BASICBLOCK(master);
    int startDate = IssueItem_STARTDATE(ITEM);
    Operator operator = Operation_operator(operation);
    if (MDT_isFamily("k1")) {
      // FIXME DUCO
      if (Operator_isPseudo(operator)) {
        switch (operator) {
          case Operator_LABEL:
          case Operator_LOOPTO:
          case Operator_KILL:
            {
              Selector selector = BasicBlock_selector(basicBlock);
              Selector_attachLast(selector, operation);
              break;
            }
          default:
            // Do nothing
            break;
        }
      }
    } else // This way for st200
    if (!Operator_isPseudo(operator) || Operator_isKill(operator)
        || Operator_isHWLBranch(operator)) {
      Selector selector = BasicBlock_selector(basicBlock);
      Selector_attachLast(selector, operation);
    }
    if (IssueItem_isControl(ITEM)) {
      // Iterate to the next ConstructionMaster.
      master = ConstructionMaster_NEXT(master);
    }
    // Compute the firstDate.
    Except_CHECK(startDate >= 0);
    if (firstDate < 0) firstDate = startDate;
  } Construction_ENDEACH_IssueItem;
  Except_CHECK(firstDate >= 0);
  Construction_FORBACK_IssueItem(this, ITEM) {
    Operation operation = IssueItem_OPERATION(ITEM);
    int startDate = IssueItem_STARTDATE(ITEM) - firstDate;
    Operation_setStartDate(operation, startDate);
  } Construction_ENDBACK_IssueItem;
  SCD_TRACE && fprintf(SCD_TRACE, "*** MASTER SCHEDULE:\tLAMBDA=%d\tKUNROLL=%d\n",
                       Construction_LAMBDA(this), Construction_KUNROLL(this));
  SCD_TRACE && Construction_prettyIssueItems(this, SCD_TRACE);
  // Scan the epilogs and fill their BasicBlocks
  Construction_FOREACH_ConstructionEpilog(this, epilog) {
    Selector selector = ConstructionEpilog_SELECTOR(epilog);
    if (!Selector_isEmpty(selector)) {
      CFGNode node = ConstructionEpilog_CFGNODE(epilog);
      BasicBlock basicBlock = CFGNode_BASICBLOCK(node);
      Selector block_selector = BasicBlock_SELECTOR(basicBlock);
      BasicBlock_putSelector(basicBlock, selector);
    }
  } Construction_ENDEACH_ConstructionEpilog;
}

#ifdef $XCC__h
/**
 * Fix the BasicBlock attributes.
 */
void
Construction_fixBasicBlocks(Construction this);
#endif//$XCC__h

void
Construction_fixBasicBlocks(Construction this)
{
  int kunroll = Construction_KUNROLL(this);
  intptr_t traceId = Construction_TRACEID(this);
  BlockTrace blockTrace = Construction_BLOCKTRACE(this);
  int instance = Construction_INSTANCE(this);
  if (instance <= 0) instance = 1;
  Construction_FOREACH_ConstructionMaster(this, master) {
    BasicBlock basicBlock = ConstructionMaster_BASICBLOCK(master);
    Operation first_operation = BasicBlock_firstOperation(basicBlock);
    BasicBlock_setTraceId(basicBlock, traceId);
    if (!ConstructionMaster_isUnscheduled(master)) {
      BasicBlock_setFlags(basicBlock, BasicBlockFlag_Scheduled);
    }
    if (kunroll > 0) {
      BasicBlock basicBlock = ConstructionMaster_BASICBLOCK(master);
      if (IssueItem_isInProlog(ConstructionMaster_ISSUEITEM(master))) {
        Except_CHECK(BasicBlock_instance(basicBlock) == 0);
        BasicBlock_FOREACH_Operation(basicBlock, operation) {
          Operation_setFlags(operation, OperationFlag_Hoisted);
        } BasicBlock_ENDEACH_Operation;
      } else {
        BasicBlock headBlock = Construction_HEADBLOCK(this);
        LoopScope loopScope = BasicBlock_loopScope(headBlock);
        Except_CHECK(LoopScope_headerBlock(loopScope) == headBlock);
        BasicBlock_setLoopScope(basicBlock, loopScope);
        BasicBlock_setInstance(basicBlock, instance*kunroll);
        BasicBlock_FOREACH_Operation(basicBlock, operation) {
          IssueItem ITEM = Operation_issueItem(operation);
          int offset = instance*IssueItem_INSTANCE(ITEM);
          int instance = Operation_instance(operation) + offset;
          Operation_setInstance(operation, instance);
        } BasicBlock_ENDEACH_Operation;
      }
    }
  } Construction_ENDEACH_ConstructionMaster;
}

#ifdef $XCC_h
/**
 * Fix the speculated Operations.
 */
void
Construction_fixSpeculated(Construction this);
#endif//$XCC_h

void
Construction_fixSpeculated(Construction this)
{
  Processor processor = Construction_PROCESSOR(this);
  BlockTrace blockTrace = Construction_BLOCKTRACE(this);
  EffectTable effectTable = BlockTrace_EFFECTTABLE(blockTrace);
  int current_controlId = -1, current_instance = -1; 
  // Fix the speculated Operations.
  PFA_TRACE && fprintf(PFA_TRACE, "*** SPECULATION\n");
  Construction_FORBACK_IssueItem(this, ITEM) {
    int controlId = IssueItem_CONTROLID(ITEM);
    int instance = IssueItem_INSTANCE(ITEM);
    Operation operation = IssueItem_OPERATION(ITEM);
    // EffectTable_releaseOperation(effectTable, operation);
    Operation_setMainEffects(operation, NULL);
    if (IssueItem_isControl(ITEM)) {
      current_controlId = controlId;
      current_instance = instance;
    } else if (instance != current_instance || controlId != current_controlId) {
      // Operation has been control speculated.
      if (Operation_isSpeculable(operation)) {
        Operation_speculate(operation, processor);
      }
      PFA_TRACE && fprintf(PFA_TRACE, "    @%d.%d -> @%d.%d",
          instance, controlId, current_instance, current_controlId);
      PFA_TRACE && Operation_pretty(operation, PFA_TRACE);
    } else if (Operation_boosting(operation) != 0) {
      // Operation has not been control speculated so reverse boosting.
      Operation_unpredicate(operation, processor);
      Operation_setBoosting(operation, 0);
      PFA_TRACE && fprintf(PFA_TRACE, "    reverse boosting");
      PFA_TRACE && Operation_pretty(operation, PFA_TRACE);
    }
    Operation_setTask(operation, NULL);
    Operation_setIndex(operation, -1);
  } Construction_ENDBACK_IssueItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Epilog block for the software pipeline Construction.
 */
struct ConstructionEpilog_;
  //@access NEXT		(ConstructionEpilog)(IDListItem_NEXT(this))
  //@access PREV		(ConstructionEpilog)(IDListItem_PREV(this))
#endif//$XCC_h

#ifdef $XCC__h
struct ConstructionEpilog_ {
  //@args	Construction construction, IssueItem issueItem
  Construction CONSTRUCTION;
  IssueItem ISSUEITEM;
  CFGNode CFGNODE;		// The CFGNode of this ConstructionEpilog.
  Selector_ SELECTOR_;
  //@access SELECTOR	ConstructionEpilog__SELECTOR_(this)
};
#endif//$XCC__h

ConstructionEpilog
ConstructionEpilog_Ctor(ConstructionEpilog this,
                        Construction construction, IssueItem issueItem)
{
  RegionFlow regionFlow = Construction_REGIONFLOW(construction);
  Procedure procedure = RegionFlow_procedure(regionFlow);
  CFGArc exitArc = IssueItem_EXITARC(issueItem);
  *ConstructionEpilog__CONSTRUCTION(this) = construction;
  *ConstructionEpilog__ISSUEITEM(this) = issueItem;
  *ConstructionEpilog__CFGNODE(this) = CFGArc_HEAD(exitArc);
  Selector_Ctor(ConstructionEpilog_SELECTOR(this), Procedure_memory(procedure), procedure, false);
  return this;
}

void
ConstructionEpilog_Dtor(ConstructionEpilog this)
{
  Selector_Dtor(ConstructionEpilog_SELECTOR(this));
}

size_t
ConstructionEpilog_Size(Construction construction, IssueItem issueItem)
{
  return sizeof(ConstructionEpilog_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ConstructionEpilog_)\t%zu\n", sizeof(ConstructionEpilog_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Iterate over this ConstructionEpilog Operations.
 */
#define ConstructionEpilog_FOREACH_Operation(this, operation) { \
  Selector ConstructionEpilog_SELECTOR = ConstructionEpilog_SELECTOR(this); \
  Selector_FOREACH_Operation(ConstructionEpilog_SELECTOR, operation)
#define ConstructionEpilog_ENDEACH_Operation \
  Selector_ENDEACH_Operation \
}
#endif//$XCC__h

#ifdef $XCC_h
/**
 * Pretty-print this ConstructionEpilog.
 */
bool
ConstructionEpilog_pretty(ConstructionEpilog this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
ConstructionEpilog_pretty(ConstructionEpilog this, FILE *file)
{
  IssueItem issueItem = ConstructionEpilog_ISSUEITEM(this);
  if (file == NULL) file = stderr;
  fprintf(file, "*** EPILOG:\tdate=%d\tinstance=%d",
      IssueItem_STARTDATE(issueItem), IssueItem_INSTANCE(issueItem));
  Operation_pretty(IssueItem_OPERATION(issueItem), file);
  ConstructionEpilog_FOREACH_Operation(this, operation) {
    fprintf(file, "\t") && Operation_pretty(operation, file);
  } ConstructionEpilog_ENDEACH_Operation;
  return true;
}
#endif//_NTRACE

#ifdef $XCC__h
static inline void
ConstructionEpilog_pushOperation(ConstructionEpilog this, Operation operation)
{
  Selector selector = ConstructionEpilog_SELECTOR(this);
  Selector_attachLast(selector, operation);
}
#endif//$XCC__h

#ifdef $XCC__h
int
ConstructionEpilog_compare(const void *this_pointer, const void *that_pointer);
#endif//$XCC__h

int
ConstructionEpilog_compare(const void *this_pointer, const void *that_pointer)
{
  const_ConstructionEpilog this = this_pointer;
  const_ConstructionEpilog that = that_pointer;
  return InstructionTask_compare(ConstructionEpilog_ISSUEITEM(this),
                           ConstructionEpilog_ISSUEITEM(that));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Master block for the software pipeline Construction.
 */
struct ConstructionMaster_;
  //@access NEXT		(ConstructionMaster)(IDListItem_NEXT(this))
  //@access PREV		(ConstructionMaster)(IDListItem_PREV(this))
#endif//$XCC_h

#ifdef $XCC__h
struct ConstructionMaster_ {
  //@args	IssueItem issueItem, BasicBlock basicBlock
  IssueItem ISSUEITEM;		// The IssueItem that ends this Master.
  //@access EXITARC	IssueItem_EXITARC(ConstructionMaster_ISSUEITEM(this))
  //@mutate EXITARC	IssueItem__EXITARC(ConstructionMaster_ISSUEITEM(this))
  //@access FALLARC	IssueItem_FALLARC(ConstructionMaster_ISSUEITEM(this))
  //@mutate FALLARC	IssueItem__FALLARC(ConstructionMaster_ISSUEITEM(this))
  BasicBlock BASICBLOCK;	// The BasicBlock of this Master.
  CFGNode CFGNODE;		// The CFGNode of this Master.
  ConstructionEpilog EPILOG;
  ConstructionMasterFlags FLAGS; // This ConstructionMaster flags.
  //@access isUnscheduled ((ConstructionMaster_FLAGS(this)&ConstructionMasterFlag_Unscheduled) != 0)
};
#endif//$XCC__h

ConstructionMaster
ConstructionMaster_Ctor(ConstructionMaster this,
                        IssueItem issueItem, BasicBlock basicBlock)
{
  CFGNode cfgNode = BasicBlock_cfgNode(basicBlock);
  *ConstructionMaster__ISSUEITEM(this) = issueItem;
  *ConstructionMaster__BASICBLOCK(this) = basicBlock;
  *ConstructionMaster__CFGNODE(this) = cfgNode;
  *ConstructionMaster__EPILOG(this) = NULL;
  *ConstructionMaster__FLAGS(this) = 0;
  return this;
}

size_t
ConstructionMaster_Size(IssueItem issueItem, BasicBlock basicBlock) {
  return sizeof(ConstructionMaster_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ConstructionMaster_)\t%zu\n", sizeof(ConstructionMaster_));
}
#endif//$XCC__c

#ifdef $XCC__h
int
ConstructionMaster_compare(const void *this_pointer, const void *that_pointer);
#endif//$XCC__h

int
ConstructionMaster_compare(const void *this_pointer, const void *that_pointer)
{
  const_ConstructionMaster this = this_pointer;
  const_ConstructionMaster that = that_pointer;
  IssueItem this_item = ConstructionMaster_ISSUEITEM(this);
  IssueItem that_item = ConstructionMaster_ISSUEITEM(that);
  return InstructionTask_compare(this_item, that_item);
}

#ifdef $XCC_h
/**
 * Pretty-print this ConstructionMaster.
 */
bool
ConstructionMaster_pretty(ConstructionMaster this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
ConstructionMaster_pretty(ConstructionMaster this, FILE *file)
{
  IssueItem issueItem = ConstructionMaster_ISSUEITEM(this);
  BasicBlock basicBlock = ConstructionMaster_BASICBLOCK(this);
  CFGNode cfgNode = ConstructionMaster_CFGNODE(this);
  CFGArc exitArc = ConstructionMaster_EXITARC(this);
  CFGArc fallArc = ConstructionMaster_FALLARC(this);
  if (file == NULL) file = stderr;
  fprintf(file, "\t%d:Block_%d",
      BasicBlock_identity(basicBlock), CFGNode_INDEX(cfgNode));
  if (exitArc != NULL) {
    CFGNode tail_node = CFGArc_TAIL(exitArc);
    CFGNode head_node = CFGArc_HEAD(exitArc);
    fprintf(file, "\texit(_%d, _%d)",
        tail_node == NULL? -1: CFGNode_INDEX(tail_node),
        head_node == NULL? -1: CFGNode_INDEX(head_node));
  } else fprintf(file, "\t\t");
  if (fallArc != NULL) {
    CFGNode tail_node = CFGArc_TAIL(fallArc);
    CFGNode head_node = CFGArc_HEAD(fallArc);
    fprintf(file, "\tfall(_%d, _%d)",
        tail_node == NULL? -1: CFGNode_INDEX(tail_node),
        head_node == NULL? -1: CFGNode_INDEX(head_node));
  } else fprintf(file, "\t\t");
  Operation_pretty(IssueItem_OPERATION(issueItem), file);
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Construction module.
 */
void
Construction_INIT(void);
#endif//$XCC__h

void
Construction_INIT(void)
{
}

#ifdef $XCC__h
/**
 * Finalize the Construction module.
 */
void
Construction_FINI(void);
#endif//$XCC__h

void
Construction_FINI(void)
{
}

#if XCC__C
static void
Construction_TEST(void)
{
#include "SCD/Construction_.c"
}

int
main(int argc, char **argv)
{
  SCD_Test(Construction_TEST);
  return 0;
}
#endif

