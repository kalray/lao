#ifdef $XCC_h
/*
 * !!!!	IssueTable.xcc
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
 * @ingroup XFA
 * @brief Issue, IssueGroup and IssueTable.
 */
#endif//$XCC_h


#include "XFA/XFA_.h"

#ifdef $XCC__h
#include "XFA/Issue.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Item to represents an Operation in an IssueTable.
 */
struct IssueItem_ {
  //@args	Operation operation, Opcode opcode
  //@access NEXT		(IssueItem)(IDListItem_NEXT(this))
  //@access PREV		(IssueItem)(IDListItem_PREV(this))
  InstructionTask_ TASK[1];		// The InstructionTask of this Instruction.
  //@access OPERATION	(Operation)InstructionTask_INDEXED(IssueItem_TASK(this))
  //@mutate OPERATION	(Operation *)InstructionTask__INDEXED(IssueItem_TASK(this))
  //@access INDEX	InstructionTask_INDEX(IssueItem_TASK(this))
  //@mutate INDEX	InstructionTask__INDEX(IssueItem_TASK(this))
  //@access FLAGS	InstructionTask_FLAGS(IssueItem_TASK(this))
  //@mutate FLAGS	InstructionTask__FLAGS(IssueItem_TASK(this))
  //@access isControl	InstructionTask_isControl(IssueItem_TASK(this))
  //@access isVolatile	InstructionTask_isVolatile(IssueItem_TASK(this))
  //@access isMemoryRead	InstructionTask_isMemoryRead(IssueItem_TASK(this))
  //@access isMemoryWrite	InstructionTask_isMemoryWrite(IssueItem_TASK(this))
  //@access isInductive	InstructionTask_isInductive(IssueItem_TASK(this))
  //@access isSPUpdate	InstructionTask_isSPUpdate(IssueItem_TASK(this))
  //@access isInProlog	InstructionTask_isInProlog(IssueItem_TASK(this))
  //@access isKunrolled	InstructionTask_isKunrolled(IssueItem_TASK(this))
  //@access OPCODE	InstructionTask_OPCODE(IssueItem_TASK(this))
  //@access SCHEDULING	InstructionTask_SCHEDULING(IssueItem_TASK(this))
  //@mutate SCHEDULING	InstructionTask__SCHEDULING(IssueItem_TASK(this))
  //@access RESERVATION	InstructionTask_RESERVATION(IssueItem_TASK(this))
  //@access PROCESSING	InstructionTask_PROCESSING(IssueItem_TASK(this))
  //@access INSTANCE	InstructionTask_INSTANCE(IssueItem_TASK(this))
  //@mutate INSTANCE	InstructionTask__INSTANCE(IssueItem_TASK(this))
  //@access ORDERING	InstructionTask_ORDERING(IssueItem_TASK(this))
  //@mutate ORDERING	InstructionTask__ORDERING(IssueItem_TASK(this))
  //@access STARTDATE	InstructionTask_STARTDATE(IssueItem_TASK(this))
  //@mutate STARTDATE	InstructionTask__STARTDATE(IssueItem_TASK(this))
  //@access RELEASE	InstructionTask_RELEASE(IssueItem_TASK(this))
  //@mutate RELEASE	InstructionTask__RELEASE(IssueItem_TASK(this))
  //@access CRITICAL	InstructionTask_CRITICAL(IssueItem_TASK(this))
  //@mutate CRITICAL	InstructionTask__CRITICAL(IssueItem_TASK(this))
  //@access PREDCOUNT	InstructionTask_PREDCOUNT(IssueItem_TASK(this))
  //@mutate PREDCOUNT	InstructionTask__PREDCOUNT(IssueItem_TASK(this))
  const_ReservationTable TABLE;		// This IssueItem ReservationTable.
  //@access COLUMNS	ReservationTable_COLUMNS(IssueItem_TABLE(this))
  //@access REQUIREMENTS	(ResourceVector__available(IssueItem_COLUMNS(this), 1) - 1)
  int16_t CONTROLID;			// This IssueItem controlId number.
  struct DDGraphNode_ *DDGNODES[3];	// The DDGraphNodes for this IssueItem.
  //@access EARLYNODE	(IssueItem_DDGNODES(this)[DDGraphKind_Early])
  //@access LATENODE	(IssueItem_DDGNODES(this)[DDGraphKind_Late])
  //@access MCRLNODE	(IssueItem_DDGNODES(this)[DDGraphKind_MCRL])
  struct CFGArc_ *EXITARC;		// The exit CFGArc in case of exit.
  struct CFGArc_ *FALLARC;		// The fall-through CFGArc.
};
#endif//$XCC_h

IssueItem
IssueItem_Ctor(IssueItem this,
               Operation operation, Opcode opcode)
{
  Scheduling scheduling = Opcode_scheduling(opcode);
  Reservation reservation = Scheduling_reservation(scheduling);
  const_ReservationTable table = Reservation_table(reservation);
  Operation_setTask(operation, IssueItem_TASK(this));
  InstructionTask_Ctor(IssueItem_TASK(this), opcode);
  *InstructionTask__INDEXED(IssueItem_TASK(this)) = (Indexed)operation;
  *InstructionTask__STARTDATE(IssueItem_TASK(this)) = INT16_MAX;
  *IssueItem__TABLE(this) = Reservation_table(reservation);
  *IssueItem__SCHEDULING(this) = scheduling;
  *IssueItem__CONTROLID(this) = -1;
  IssueItem_DDGNODES(this)[0] = NULL;
  IssueItem_DDGNODES(this)[1] = NULL;
  IssueItem_DDGNODES(this)[2] = NULL;
  *IssueItem__EXITARC(this) = NULL;
  *IssueItem__FALLARC(this) = NULL;
  return this;
}

void
IssueItem_Dtor(IssueItem this)
{
}

size_t
IssueItem_Size(Operation operation, Opcode opcode)
{
  return sizeof(IssueItem_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IssueItem_)\t%zu\n", sizeof(IssueItem_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Pretty-print this IssueItem.
 */
bool
IssueItem_pretty(IssueItem this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
IssueItem_pretty(IssueItem this, FILE *file)
{
  Operation operation = IssueItem_OPERATION(this);
  struct CFGArc_ *exitArc = IssueItem_EXITARC(this);
  struct CFGArc_ *fallArc = IssueItem_FALLARC(this);
  BasicBlock basicBlock = Operation_basicBlock(operation);
  if (file == NULL) file = stderr;
  if (basicBlock != NULL) {
    DGraphNode node = (DGraphNode)BasicBlock_CFGNODE(basicBlock);
    fprintf(file, "%d:Block_%d",
        BasicBlock_identity(basicBlock), DGraphNode_INDEX(node));
  } else fprintf(file, "\t");
  if (exitArc != NULL) {
    DGraphNode tail_node = DGraphArc_TAILNODE(exitArc);
    DGraphNode head_node = DGraphArc_HEADNODE(exitArc);
    fprintf(file, "\texit(_%d, _%d)",
        tail_node ==  NULL? -1: DGraphNode_INDEX(tail_node),
        head_node ==  NULL? -1: DGraphNode_INDEX(head_node));
  } else fprintf(file, "\t\t");
  if (fallArc != NULL) {
    DGraphNode tail_node = DGraphArc_TAILNODE(fallArc);
    DGraphNode head_node = DGraphArc_HEADNODE(fallArc);
    fprintf(file, "\tfall(_%d, _%d)",
        tail_node ==  NULL? -1: DGraphNode_INDEX(tail_node),
        head_node ==  NULL? -1: DGraphNode_INDEX(head_node));
  } else fprintf(file, "\t\t");
#if 0
  if (IssueItem_isInvariant(this)) {
    fprintf(file, "(invar)");
  }
#endif
  Operation_pretty(operation, file);
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Group of IssueItems that may issue simultaneously.
 */
struct IssueGroup_ {
  //@args	const uint8_t *availability
  IssueItem ITEMS[Bundle__MAXINSTRUCTIONS];
  const uint8_t *AVAILABILITY;
  uint8_t ITEMCOUNT;
  uint8_t COUNTERS[Resource__COUNT];
};
#endif//$XCC_h

IssueGroup
IssueGroup_Ctor(IssueGroup this,
                const uint8_t *availability)
{
  int resource;
  IssueGroup_ITEMS(this)[0] = NULL;
  *IssueGroup__AVAILABILITY(this) = availability;
  *IssueGroup__ITEMCOUNT(this) = 0;
  for (resource = 0; resource < Resource__Count; resource++) {
    IssueGroup_COUNTERS(this)[resource] = 0;
  }
  return this;
}

IssueGroup
IssueGroup_Copy(IssueGroup this, const_IssueGroup that)
{
  *(this) = *(that);
  return this;
}

void
IssueGroup_Dtor(IssueGroup this)
{
}

size_t
IssueGroup_Size(const uint8_t *availability)
{
  return sizeof(IssueGroup_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IssueGroup_)\t%zu\n", sizeof(IssueGroup_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Count this IssueGroup IssueItems.
 */
static inline int
IssueGroup_itemCount(IssueGroup this)
{
  return IssueGroup_ITEMCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Needed by IssueGroup_FOREACH_IssueItem.
 */
static inline IssueItem *
IssueGroup_items(IssueGroup this)
{
  return IssueGroup_ITEMS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this IssueGroup Resources.
 */
#define IssueGroup_FOREACH_Resource(this, resource) { \
  int IssueGroup_RESOURCE = 1; \
  for (; IssueGroup_RESOURCE < Resource__Count; IssueGroup_RESOURCE++) { \
    Resource resource = IssueGroup_RESOURCE;
#define IssueGroup_ENDEACH_Resource \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this IssueGroup ITEMS.
 */
#define IssueGroup_FOREACH_IssueItem(this, item) { \
  IssueItem *IssueGroup_ITEMS = IssueGroup_items(this); \
  int IssueGroup_ITEMCOUNT = IssueGroup_itemCount(this), IssueGroup_I; \
  for (IssueGroup_I = 0; IssueGroup_I < IssueGroup_ITEMCOUNT; IssueGroup_I++) { \
    IssueItem item = IssueGroup_ITEMS[IssueGroup_I];
#define IssueGroup_ENDEACH_IssueItem \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pretty-print this IssueGroup.
 */
bool
IssueGroup_pretty(IssueGroup this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
IssueGroup_pretty(IssueGroup this, FILE *file)
{
  if (file == NULL) file = stderr;
  IssueGroup_FOREACH_IssueItem(this, item) {
    int32_t index = IssueItem_INDEX(item);
    Scheduling scheduling = IssueItem_SCHEDULING(item);
    fprintf(file, "\t%s_%d", Scheduling_name(scheduling), index);
  } IssueGroup_ENDEACH_IssueItem;
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Check this IssueGroup invariants.
 */
bool
IssueGroup_check(IssueGroup this);
#endif//$XCC_h

bool
IssueGroup_check(IssueGroup this)
{
  const uint8_t *availability = IssueGroup_AVAILABILITY(this);
  uint8_t *counters = IssueGroup_COUNTERS(this);
  IssueItem *items = IssueGroup_ITEMS(this);
  int itemCount = IssueGroup_ITEMCOUNT(this), i;
  uint8_t *check_counters = alloca(sizeof(uint8_t)*Resource__Count);
  Except_ALWAYS(itemCount <= Bundle__MaxInstructions);
  IssueGroup_FOREACH_Resource(this, resource) {
    Except_ALWAYS(counters[resource] <= availability[resource]);
    check_counters[resource] = 0;
  } IssueGroup_ENDEACH_Resource;
  for (i = 0; i < itemCount - 1; i++) {
    Scheduling scheduling_i = IssueItem_SCHEDULING(items[i]);
    Scheduling scheduling_j = IssueItem_SCHEDULING(items[i + 1]);
    Except_ALWAYS(scheduling_i <= scheduling_j);
  }
  for (i = 0; i < itemCount; i++) {
    const uint8_t *requirements = IssueItem_REQUIREMENTS(items[i]);
    IssueGroup_FOREACH_Resource(this, resource) {
      check_counters[resource] += requirements[resource];
    } IssueGroup_ENDEACH_Resource;
  }
  IssueGroup_FOREACH_Resource(this, resource) {
    Except_ALWAYS(check_counters[resource] == counters[resource]);
  } IssueGroup_ENDEACH_Resource;
  return true;
}

#ifdef $XCC_h
/**
 * Clear the contents of this IssueGroup.
 */
static inline void
IssueGroup_clear(IssueGroup this)
{
  const uint8_t *availability = IssueGroup_AVAILABILITY(this);
  IssueGroup_Dtor(this), IssueGroup_Ctor(this, availability);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Find a conflict between this IssueGroup and that IssueItem.
 */
IssueItem
IssueGroup_conflict(IssueGroup this, IssueItem item);
#endif//$XCC_h

IssueItem
IssueGroup_conflict(IssueGroup this, IssueItem item)
{
  const uint8_t *requirements = IssueItem_REQUIREMENTS(item);
  const uint8_t *this_availability = IssueGroup_AVAILABILITY(this);
  uint8_t *this_counters = IssueGroup_COUNTERS(this);
  IssueItem *this_items = IssueGroup_ITEMS(this);
  int itemCount = IssueGroup_ITEMCOUNT(this);
  Except_REQUIRE(IssueItem_SCHEDULING(item) != Scheduling__UNDEF);
  // Check this IssueGroup_COUNTERS.
  IssueGroup_FOREACH_Resource(this, resource) {
    int available = this_availability[resource];
    int counter = requirements[resource], i;
    if (this_counters[resource] + counter > available) {
      for (i = 0; i < itemCount; i++) {
        IssueItem this_item = this_items[i];
        const uint8_t *this_requirements = IssueItem_REQUIREMENTS(this_item);
        if (this_requirements[resource] + counter > available) return this_item;
      }
    }
  } IssueGroup_ENDEACH_Resource;
  Except_CHECK(itemCount > 0);
  return this_items[0];
}

#ifdef $XCC_h
/**
 * Check if this IssueGroup may accept that IssueItem.
 */
bool
IssueGroup_accept(IssueGroup this, IssueItem item);
#endif//$XCC_h

bool
IssueGroup_accept(IssueGroup this, IssueItem item)
{
  Scheduling scheduling = IssueItem_SCHEDULING(item);
  const uint8_t *requirements = IssueItem_REQUIREMENTS(item);
  const uint8_t *this_availability = IssueGroup_AVAILABILITY(this);
  uint8_t *this_counters = IssueGroup_COUNTERS(this);
  Except_REQUIRE(scheduling != Scheduling__UNDEF);
  // Check this IssueGroup_COUNTERS.
  IssueGroup_FOREACH_Resource(this, resource) {
    int available = this_availability[resource];
    int counter = requirements[resource];
    if (this_counters[resource] + counter > available) return false;
  } IssueGroup_ENDEACH_Resource;
  if (0) {
    IssueItem *this_items = IssueGroup_ITEMS(this);
    int itemCount = IssueGroup_ITEMCOUNT(this), i, j;
    Scheduling schedulings[Bundle__MAXINSTRUCTIONS + 1];
    // Fill the schedulings array in non-decreasing scheduling order.
    for (i = 0; i < itemCount; i++) {
      if (IssueItem_SCHEDULING(this_items[i]) >= scheduling) break;
      schedulings[i] = IssueItem_SCHEDULING(this_items[i]);
    }
    schedulings[i] = scheduling;
    for (j = itemCount; j > i; j--) {
      schedulings[j] = IssueItem_SCHEDULING(this_items[j - 1]);
    }
    schedulings[itemCount + 1] = Scheduling__UNDEF;
  }
  return true;
}

#ifdef $XCC_h
/**
 * Insert an IssueItem into this IssueGroup.
 */
void
IssueGroup_insert(IssueGroup this, IssueItem item);
#endif//$XCC_h

void
IssueGroup_insert(IssueGroup this, IssueItem item)
{
  Scheduling scheduling = IssueItem_SCHEDULING(item);
  const uint8_t *requirements = IssueItem_REQUIREMENTS(item);
  uint8_t *this_counters = IssueGroup_COUNTERS(this);
  IssueItem *this_items = IssueGroup_ITEMS(this);
  int itemCount = IssueGroup_ITEMCOUNT(this), i, j;
  Except_REQUIRE(scheduling != Scheduling__UNDEF);
  Except_REQUIRE(itemCount < Bundle__MAXINSTRUCTIONS);
  // Update this IssueGroup_COUNTERS.
  IssueGroup_FOREACH_Resource(this, resource) {
    this_counters[resource] += requirements[resource];
  } IssueGroup_ENDEACH_Resource;
  // Insert item in non-decreasing scheduling order.
  for (i = 0; i < itemCount; i++) {
    if (IssueItem_SCHEDULING(this_items[i]) >= scheduling) break;
  }
  for (j = itemCount; j > i; j--) this_items[j] = this_items[j - 1];
  this_items[i] = item;
  ++*IssueGroup__ITEMCOUNT(this);
  Except_CHECK(IssueGroup_check(this));
}

#ifdef $XCC_h
/**
 * Remove that IssueItem from this IssueGroup.
 */
void
IssueGroup_remove(IssueGroup this, IssueItem item);
#endif//$XCC_h

void
IssueGroup_remove(IssueGroup this, IssueItem item)
{
  Scheduling scheduling = IssueItem_SCHEDULING(item);
  const uint8_t *requirements = IssueItem_REQUIREMENTS(item);
  uint8_t *this_counters = IssueGroup_COUNTERS(this);
  IssueItem *this_items = IssueGroup_ITEMS(this);
  int itemCount = IssueGroup_ITEMCOUNT(this), i, j;
  Except_REQUIRE(scheduling != Scheduling__UNDEF);
  // Update this IssueGroup_COUNTERS.
  IssueGroup_FOREACH_Resource(this, resource) {
    this_counters[resource] -= requirements[resource];
  } IssueGroup_ENDEACH_Resource;
  // Remove item in non-decreasing scheduling order.
  for (i = 0; i < itemCount && this_items[i] != item; i++);
  Except_CHECK(i < itemCount);
  for (j = i + 1; j < itemCount; j++) this_items[j - 1] = this_items[j];
  --*IssueGroup__ITEMCOUNT(this);
  Except_CHECK(IssueGroup_check(this));
}

#if XCC__C
static void
IssueGroup_enumerate(IssueGroup this, Processor processor, Scheduling scheduling)
{
  IssueItem_ item[1];
  LabelTable labelTable = LabelTable_new(Memory_Root, 16);
  Label label = LabelTable_makeLabel(labelTable);
  Reservation reservation = Scheduling_reservation(scheduling);
  const_ReservationTable table = Reservation_table(reservation);
  Operation_ operation[1];
  Operation_Ctor(operation, Operator_LABEL, 0, 0, NULL, NULL);
  IssueItem_Ctor(item, operation, Opcode__UNDEF);
  *IssueItem__TABLE(item) = table;
  *IssueItem__SCHEDULING(item) = scheduling;
  printf("IssueGroup:");
  IssueGroup_FOREACH_IssueItem(this, item) {
    Scheduling scheduling = IssueItem_SCHEDULING(item);
    printf("\t%s", Scheduling_name(scheduling));
  } IssueGroup_ENDEACH_IssueItem;
  printf("\n");
  for (; scheduling < Scheduling__Count; scheduling++) {
    Reservation reservation = Scheduling_reservation(scheduling);
    const_ReservationTable table = Reservation_table(reservation);
    const_ResourceVector column = ReservationTable_COLUMNS(table);
    const uint8_t *requirements = ResourceVector__available((ResourceVector)column, 1) - 1;
    int cumulated = 0, resource;
    for (resource = 1; resource < Resource__Count; resource++) {
      cumulated += requirements[resource];
    }
    if (cumulated == 0) continue;
    *IssueItem__TABLE(item) = table;
    *IssueItem__SCHEDULING(item) = scheduling;
    if (IssueGroup_accept(this, item)) {
      IssueGroup_insert(this, item);
      IssueGroup_enumerate(this, processor, scheduling);
      IssueGroup_remove(this, item);
    }
  }
  LabelTable_delete(labelTable);
  IssueItem_Dtor(item);
}

static void
IssueGroup_Enumerate(Processor processor)
{
  const_ResourceVector requirements = Processor_availability(processor);
  const uint8_t *availability = ResourceVector__available((ResourceVector)requirements, 1) - 1;
  IssueGroup_ group[1];
  IssueGroup_Ctor(group, availability);
  IssueGroup_enumerate(group, processor, Scheduling__UNDEF + 1);
  IssueGroup_Dtor(group);
}
#endif

#ifdef $XCC__c
{
#if 0// BD3 2014-08-01: Disable as it creates huge output on k1 cores.
  int processor;
  for (processor = Processor__UNDEF + 1; processor < Processor__Count; processor++) {
    printf("\nProcessor_%s:\n", Processor_Id(processor));
    IssueGroup_Enumerate(processor);
  }
#endif
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Table of IssueGroups.
 */
struct IssueTable_ {
  //@args	Memory memory, const uint8_t *availability
  IStack_ GROUPS_;			// The inlined IssueGroups.
  //@access GROUPS	IssueTable__GROUPS_(this)
  //@access MEMORY	IStack_memory(IssueTable_GROUPS(this))
  //@access LASTGROUP	(IssueGroup_ *)IStack_lastItem(IssueTable_GROUPS(this))
  //@access BASEGROUP	(IssueGroup_ *)IStack_base(IssueTable_GROUPS(this))
  //@access PASTGROUP	(IssueGroup_ *)IStack_past(IssueTable_GROUPS(this))
  //@access LAMBDA	(IStack_usedSize(IssueTable_GROUPS(this))/sizeof(IssueGroup_))
  const uint8_t *AVAILABILITY;		// The Processor_availability.
  int32_t DELTAS;			// Sum of the deltas.
  int32_t COUNTERS[Resource__COUNT];	// Global Resource counters.
};
#endif//$XCC_h

IssueTable
IssueTable_Ctor(IssueTable this,
                Memory memory, const uint8_t *availability)
{
  int resource;
  IStack_Ctor(IssueTable_GROUPS(this), memory, sizeof(IssueGroup_), 128);
  IssueGroup_Ctor(IStack_push2(IssueTable_GROUPS(this)), availability);
  *IssueTable__AVAILABILITY(this) = availability;
  *IssueTable__DELTAS(this) = 0;
  for (resource = 0; resource < Resource__Count; resource++) {
    IssueTable_COUNTERS(this)[resource] = 0;
  }
  return this;
}

void
IssueTable_Dtor(IssueTable this)
{
  IStack_Dtor(IssueTable_GROUPS(this));
}

size_t
IssueTable_Size(Memory memory, const uint8_t *availability)
{
  return sizeof(IssueTable_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IssueTable_)\t%zu\n", sizeof(IssueTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
IssueTable
IssueTable_new(Memory parent, const uint8_t *availability);
#endif//$XCC_h

IssueTable
IssueTable_new(Memory parent, const uint8_t *availability)
{
  Memory memory = Memory_new(parent, true);
  size_t size = IssueTable_Size(memory, availability);
  IssueTable this = Memory_alloc(memory, size);
  return IssueTable_Ctor(this, memory, availability);
}

#ifdef $XCC_h
IssueTable
IssueTable_delete(IssueTable this);
#endif//$XCC_h

IssueTable
IssueTable_delete(IssueTable this)
{
  if (this != NULL) {
    Memory memory = IssueTable_MEMORY(this);
#ifndef _NDTOR
    IssueTable_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * This IssueTable memory.
 */
static inline Memory
IssueTable_memory(IssueTable this)
{
  return IssueTable_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IssueTable lambda.
 */
static inline int
IssueTable_lambda(IssueTable this)
{
  return IssueTable_LAMBDA(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IssueTable availability.
 */
static inline const uint8_t *
IssueTable_availability(IssueTable this)
{
  return IssueTable_AVAILABILITY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IssueTable deltas.
 */
static inline int
IssueTable_deltas(IssueTable this)
{
  return IssueTable_DELTAS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IssueTable Resource counters.
 */
static inline int32_t *
IssueTable_counters(IssueTable this)
{
  return IssueTable_COUNTERS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this IssueTable Resources.
 */
#define IssueTable_FOREACH_Resource(this, resource) { \
  int IssueTable_RESOURCE = 1; \
  for (; IssueTable_RESOURCE < Resource__Count; IssueTable_RESOURCE++) { \
    Resource resource = IssueTable_RESOURCE;
#define IssueTable_ENDEACH_Resource \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this IssueTable IssueGroups.
 */
#define IssueTable_FOREACH_IssueGroup(this, group) \
  IStack_FOREACH(IssueTable_GROUPS(this), IssueGroup_, group) {
#define IssueTable_ENDEACH_IssueGroup \
  } IStack_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pretty-print this IssueTable COUNTERS.
 */
bool
IssueTable_prettyCounters(IssueTable this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
IssueTable_prettyCounters(IssueTable this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "    RESOURCE");
  IssueTable_FOREACH_Resource(this, resource) {
    fprintf(file, "\t%s", Resource_name(resource));
  } IssueTable_ENDEACH_Resource;
  fprintf(file, "\n");
  fprintf(file, "    COUNTERS");
  IssueTable_FOREACH_Resource(this, resource) {
    int available = IssueTable_AVAILABILITY(this)[resource];
    int counter = IssueTable_COUNTERS(this)[resource];
    fprintf(file, "\t%d/%d", counter, available);
  } IssueTable_ENDEACH_Resource;
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Pretty-print this IssueTable.
 */
bool
IssueTable_pretty(IssueTable this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
IssueTable_pretty(IssueTable this, FILE *file)
{
  int cycle = 0;
  if (file == NULL) file = stderr;
  fprintf(file, "*** ISSUE TABLE:\tLAMBDA=%zu\tDELTAS=%d\n",
      IssueTable_LAMBDA(this), IssueTable_DELTAS(this));
  IssueTable_FOREACH_IssueGroup(this, group) {
    fprintf(file, "    [%d] ", cycle);
    IssueGroup_pretty(group, file);
    fprintf(file, "\n");
    cycle++;
  } IssueTable_ENDEACH_IssueGroup;
  IssueTable_prettyCounters(this, file);
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Check this IssueTable invariants.
 */
bool
IssueTable_check(IssueTable this);
#endif//$XCC_h

bool
IssueTable_check(IssueTable this)
{
  bool result = true;
  IssueTable_FOREACH_IssueGroup(this, group) {
    result &= IssueGroup_check(group);
  } IssueTable_ENDEACH_IssueGroup;
  return result;
}

#ifdef $XCC_h
/**
 * Enter that IssueItem in this IssueTable.
 */
void
IssueTable_enter(IssueTable this, IssueItem item);
#endif//$XCC_h

void
IssueTable_enter(IssueTable this, IssueItem item)
{
  Scheduling scheduling = IssueItem_SCHEDULING(item);
  const uint8_t *requirements = IssueItem_REQUIREMENTS(item);
  if (scheduling != Scheduling__UNDEF) {
    PFA_TRACE && fprintf(PFA_TRACE, "    %s_%d",
        Scheduling_name(scheduling), IssueItem_INDEX(item));
    IssueTable_FOREACH_Resource(this, resource) {
      IssueTable_COUNTERS(this)[resource] += requirements[resource];
      PFA_TRACE && fprintf(PFA_TRACE, "\t%d", requirements[resource]);
    } IssueTable_ENDEACH_Resource;
    PFA_TRACE && fprintf(PFA_TRACE, "\n");
  }
}

#ifdef $XCC_h
/**
 * Return an estimate of LAMBDA resources.
 */
int
IssueTable_estimateLambda(IssueTable this);
#endif//$XCC_h

int
IssueTable_estimateLambda(IssueTable this)
{
  const uint8_t *this_availability = IssueTable_AVAILABILITY(this);
  int *this_counters = IssueTable_COUNTERS(this), lambda = 1;
  IssueTable_FOREACH_Resource(this, resource) {
    int available = this_availability[resource], counter = this_counters[resource];
    if (available > 0 && lambda*available < counter + available - 1) {
      lambda = (counter + available - 1)/available;
    }
  } IssueTable_ENDEACH_Resource;
  return lambda;
}

#ifdef $XCC_h
/**
 * Ready the contents of this IssueTable for scheduling.
 */
void
IssueTable_ready(IssueTable this, int lambda);
#endif//$XCC_h

void
IssueTable_ready(IssueTable this, int lambda)
{
  int delta = lambda - IssueTable_LAMBDA(this);
  IssueTable_FOREACH_IssueGroup(this, group) {
    IssueGroup_clear(group);
  } IssueTable_ENDEACH_IssueGroup;
  for (; delta > 0; delta--) {
    IssueGroup group = IStack_push2(IssueTable_GROUPS(this));
    IssueGroup_Ctor(group, IssueTable_AVAILABILITY(this));
  }
  for (; delta < 0; delta++) {
    IStack_pop(IssueTable_GROUPS(this), (IStackItemRelease)IssueGroup_Dtor);
  }
  *IssueTable__DELTAS(this) = 0;
  Except_ENSURE(lambda == IssueTable_LAMBDA(this));
}

#ifdef $XCC_h
/**
 * Access an IssueGroup in this IssueTable.
 */
static inline IssueGroup
IssueTable_access(IssueTable this, int moduloDate)
{
  IssueGroup baseGroup = IssueTable_BASEGROUP(this);
  Except_REQUIRE(moduloDate >= 0 && moduloDate < IssueTable_LAMBDA(this));
  return baseGroup + moduloDate;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Assign an IssueGroup in this IssueTable.
 */
IssueGroup
IssueTable_assign(IssueTable this, int moduloDate, IssueGroup group);
#endif//$XCC_h

IssueGroup
IssueTable_assign(IssueTable this, int moduloDate, IssueGroup group)
{
  IssueGroup baseGroup = IssueTable_BASEGROUP(this);
  Except_REQUIRE(moduloDate >= 0 && moduloDate < IssueTable_LAMBDA(this));
  IssueGroup_Dtor(baseGroup + moduloDate);
  IssueGroup_Copy(baseGroup + moduloDate, group);
  return baseGroup + moduloDate;
}

#ifdef $XCC_h
/**
 * Test acceptance of that IssueItem at @c moduloDate in this IssueTable.
 * Return:	Number of cycles this IssueTable needs to be enlarged to accept the Scheduling.
 */
int
IssueTable_accept(IssueTable this, IssueItem item, int moduloDate);
#endif//$XCC_h

int
IssueTable_accept(IssueTable this, IssueItem item, int moduloDate)
{
  IssueGroup baseGroup = IssueTable_BASEGROUP(this);
  Except_REQUIRE(baseGroup + moduloDate < IssueTable_PASTGROUP(this));
  return IssueGroup_accept(baseGroup + moduloDate, item);
}

#ifdef $XCC_h
/**
 * Insert that IssueItem at @c moduloDate into this IssueTable.
 */
void
IssueTable_insert(IssueTable this, IssueItem item, int moduloDate);
#endif//$XCC_h

void
IssueTable_insert(IssueTable this, IssueItem item, int moduloDate)
{
  if (IssueItem_SCHEDULING(item) != Scheduling__UNDEF) {
    IssueGroup baseGroup = IssueTable_BASEGROUP(this);
    Except_REQUIRE(baseGroup + moduloDate < IssueTable_PASTGROUP(this));
    IssueGroup_insert(baseGroup + moduloDate, item);
  }
}

#ifdef $XCC_h
/**
 * Remove that IssueItem at @c moduloDate from this IssueTable.
 */
void
IssueTable_remove(IssueTable this, IssueItem item, int moduloDate);
#endif//$XCC_h

void
IssueTable_remove(IssueTable this, IssueItem item, int moduloDate)
{
  IssueGroup baseGroup = IssueTable_BASEGROUP(this);
  Except_REQUIRE(baseGroup + moduloDate < IssueTable_PASTGROUP(this));
  IssueGroup_remove(baseGroup + moduloDate, item);
}

#ifdef $XCC_h
/**
 * Grow this IssueTable ap @c moduloDate for @c delta cycles.
 */
void
IssueTable_growAt(IssueTable this, int moduloDate, int delta);
#endif//$XCC_h

void
IssueTable_growAt(IssueTable this, int moduloDate, int delta)
{
  if (delta > 0) {
    int lambda = IssueTable_LAMBDA(this) + delta, i;
    const uint8_t *availability = IssueTable_AVAILABILITY(this);
    IssueGroup baseGroup = NULL, pastGroup = NULL, group = NULL;
    for (i = 0; i < delta; i++) {
      IssueGroup_Ctor(IStack_push2(IssueTable_GROUPS(this)), availability);
    }
    Except_CHECK(lambda == IssueTable_LAMBDA(this));
    baseGroup = IssueTable_BASEGROUP(this);
    pastGroup = IssueTable_PASTGROUP(this);
    for (group = pastGroup - 1; group - delta > baseGroup + moduloDate; --group) {
      IssueGroup_Dtor(group), IssueGroup_Copy(group, group - delta);
    }
    for (group = baseGroup + moduloDate + 1, i = 0; i < delta; i++, group++) {
      IssueGroup_Dtor(group), IssueGroup_Ctor(group, availability);
    }
    *IssueTable__DELTAS(this) += delta;
  }
}

#ifdef $XCC_h
/**
 * Shrink this IssueTable to lambda.
 */
int
IssueTable_shrinkTo(IssueTable this, int lambda);
#endif//$XCC_h

int
IssueTable_shrinkTo(IssueTable this, int lambda)
{
  int delta = IssueTable_LAMBDA(this) - lambda;
  while (delta-- > 0) {
    IssueGroup group = IssueTable_LASTGROUP(this);
    if (IssueGroup_itemCount(group) == 0) {
      IStack_pop(IssueTable_GROUPS(this), (IStackItemRelease)IssueGroup_Dtor);
      --*IssueTable__DELTAS(this);
    } else break;
  }
  return IssueTable_LAMBDA(this);
}

#ifdef $XCC_h
/**
 * Sort the resources by decreasing usage.
 */
int
IssueTable_sortResources(IssueTable this, Resource resources[]);
#endif//$XCC_h

int
IssueTable_sortResources(IssueTable this, Resource resources[])
{
  int resourceUses[Resource__COUNT], i;
  // Initialize the resources and resourceUses arrays.
  resourceUses[Resource__UNDEF] = 0;
  resources[Resource__UNDEF] = Resource__UNDEF;
  IssueTable_FOREACH_Resource(this, resource) {
    int available = IssueTable_AVAILABILITY(this)[resource];
    int counter = IssueTable_COUNTERS(this)[resource];
    resourceUses[resource] = available != 0?
        (counter + available - 1)/available: 0;
    resources[resource] = (Resource)resource;
  } IssueTable_ENDEACH_Resource;
  // Sort the resources array.
  for (i = Resource__UNDEF; i < Resource__Count; i++) {
    int min = i, j;
    Resource tmp_resource;
    for (j = i + 1; j < Resource__Count; j++) {
      Resource resource1 = resources[j];
      Resource resource2 = resources[min];
      if (resourceUses[resource1] > resourceUses[resource2]) min = j;
    }
    tmp_resource = resources[i];
    resources[i] = resources[min];
    resources[min] = tmp_resource;
  }
  // Find index of the lowest zero resourceUses.
  for (i = Resource__Count - 1; i >= Resource__UNDEF; --i) {
    if(resourceUses[resources[i]] != 0) break;
  }
  return i + 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize Issue.
 */
#define Issue_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize Issue.
 */
#define Issue_FINI()
#endif//$XCC__h

#if XCC__C
static void
Issue_TEST(void)
{
#include "XFA/Issue_.c"
}

int
main(int argc, char **argv)
{
  XFA_Test(Issue_TEST);
  return 0;
}
#endif

