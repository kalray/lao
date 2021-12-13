#ifdef $XCC_h
/*
 * !!!!	Instruction.xcc
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
 * @brief Instructions in the code stream.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/Instruction.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Instruction flags.
 */
typedef enum {
  InstructionFlag_Skip = 0x1,		// Instruction has Control effects.
  InstructionFlag_Control = 0x2,	// Instruction has Control effects.
  InstructionFlag_MemoryRead = 0x4,	// Instruction reads in Memory.
  InstructionFlag_MemoryWrite = 0x8,	// Instruction writes to Memory.
  InstructionFlag_MemorySpill = 0x10,	// Instruction is spill code.
  InstructionFlag_Volatile = 0x20,	// Instruction is Volatile.
  InstructionFlag_Inductive = 0x40,	// Instruction defines a simple induction.
  InstructionFlag_SPUpdate = 0x80,	// Instruction updates stack pointer.
  InstructionFlag_Speculable = 0x100,	// Instruction is control speculable.
  InstructionFlag_Compensable = 0x200,	// Instruction is control compensable.
  InstructionFlag_InProlog = 0x400,	// Instruction is in software pipeline prolog.
  InstructionFlag_Kunrolled = 0x800,	// Instruction is software pipeline kunrolled.
} enum_InstructionFlag;
typedef uint16_t InstructionFlags;
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(InstructionFlag_Control == 1<<OpcodeAttribute_Control);
  Except_ALWAYS(InstructionFlag_MemoryRead == 1<<OpcodeAttribute_MemoryRead);
  Except_ALWAYS(InstructionFlag_MemoryWrite == 1<<OpcodeAttribute_MemoryWrite);
}
#endif//$XCC__c

#ifdef $XCC_h
static inline InstructionFlags
InstructionFlags_make(Opcode opcode)
{
  return Opcode_attributes(opcode) &
         (
           InstructionFlag_Skip |
           InstructionFlag_Control |
           InstructionFlag_MemoryRead |
           InstructionFlag_MemoryWrite
         );
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Read or write action of an Instruction on a StorageCell.
 */
union InstructionAction_ {
  struct {
    int32_t CELL;
    int8_t STAGE;
    int8_t RAW;
    int8_t WAR;
    int8_t WAW;
  } __;
  uint64_t PACKED;
};
typedef union InstructionAction_ InstructionAction_, *InstructionAction;
typedef const union InstructionAction_ *restrict const_InstructionAction;
typedef union InstructionAction_ *restrict restrict_InstructionAction;
#define InstructionAction_CELL(this) ((this)->__.CELL)
#define InstructionAction__CELL(this) (&(this)->__.CELL)
#define InstructionAction_STAGE(this) ((this)->__.STAGE)
#define InstructionAction__STAGE(this) (&(this)->__.STAGE)
#define InstructionAction_RAW(this) ((this)->__.RAW)
#define InstructionAction__RAW(this) (&(this)->__.RAW)
#define InstructionAction_WAR(this) ((this)->__.WAR)
#define InstructionAction__WAR(this) (&(this)->__.WAR)
#define InstructionAction_WAW(this) ((this)->__.WAW)
#define InstructionAction__WAW(this) (&(this)->__.WAW)
#define InstructionAction_PACKED(this) ((this)->PACKED)
#define InstructionAction__PACKED(this) (&(this)->PACKED)
#endif//$XCC_h

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(InstructionAction_)\t%zu\n", sizeof(InstructionAction_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Instruction fields needed to schedule Instruction(s).
 */
struct InstructionTask_ {
  //@args	Opcode opcode
  Opcode OPCODE;			// This InstructionTask Opcode.
  InstructionFlags FLAGS;		// This InstructionTask flags.
  //@access isControl	((InstructionTask_FLAGS(this)&InstructionFlag_Control) != 0)
  //@access isMemoryRead	((InstructionTask_FLAGS(this)&InstructionFlag_MemoryRead) != 0)
  //@access isMemoryWrite	((InstructionTask_FLAGS(this)&InstructionFlag_MemoryWrite) != 0)
  //@access isVolatile	((InstructionTask_FLAGS(this)&InstructionFlag_Volatile) != 0)
  //@access isInductive	((InstructionTask_FLAGS(this)&InstructionFlag_Inductive) != 0)
  //@access isSPUpdate	((InstructionTask_FLAGS(this)&InstructionFlag_SPUpdate) != 0)
  //@access isSpeculable	((InstructionTask_FLAGS(this)&InstructionFlag_Speculable) != 0)
  //@access isCompensable	((InstructionTask_FLAGS(this)&InstructionFlag_Compensable) != 0)
  //@access isInProlog	((InstructionTask_FLAGS(this)&InstructionFlag_InProlog) != 0)
  //@access isKunrolled	((InstructionTask_FLAGS(this)&InstructionFlag_Kunrolled) != 0)
  Indexed INDEXED;			// This InstructionTask reference to Operation.
  //@access INDEX	Indexed_INDEX(InstructionTask_INDEXED(this))
  Scheduling SCHEDULING;		// The Scheduling class of this InstructionTask.
  Reservation RESERVATION;		// The Reservation of this InstructionTask.
  uint8_t PROCESSING;			// This InstructionTask processing time.
  int8_t INSTANCE;			// The instance during kernel unrolling.
  int32_t ORDERING;			// For use by Instruction reordering.
  int16_t STARTDATE;			// This InstructionTask start date.
  int16_t RELEASE;			// This InstructionTask release date.
  int16_t CRITICAL;			// Critical path length for TopDown scheduling.
  int16_t PREDCOUNT;			// Count of predecessors TopDownEffect(s).
  int16_t WRITECOUNT;			// Cound of write InstructionAction(s).
  int16_t READCOUNT;			// Cound of read InstructionAction(s).
  struct TopDownEffect_ *EFFECTS;	// This InstructionTask effects (writes then reads).
};
#endif//$XCC_h

InstructionTask
InstructionTask_Ctor(InstructionTask this,
                     Opcode opcode)
{
  Scheduling scheduling = Opcode_scheduling(opcode);
  Reservation reservation = Scheduling_reservation(scheduling);
  const_ReservationTable table = Reservation_table(reservation);
  int columnCount = ReservationTable_columnCount(table);
  *InstructionTask__OPCODE(this) = opcode;
  *InstructionTask__FLAGS(this) = InstructionFlags_make(opcode);
  *InstructionTask__INDEXED(this) = NULL;
  *InstructionTask__SCHEDULING(this) = scheduling;
  *InstructionTask__RESERVATION(this) = reservation;
  *InstructionTask__PROCESSING(this) = columnCount;
  *InstructionTask__INSTANCE(this) = 0;
  *InstructionTask__ORDERING(this) = 0;
  *InstructionTask__STARTDATE(this) = -1;
  *InstructionTask__RELEASE(this) = 0;
  *InstructionTask__CRITICAL(this) = columnCount;
  *InstructionTask__PREDCOUNT(this) = 0;
  *InstructionTask__WRITECOUNT(this) = 0;
  *InstructionTask__READCOUNT(this) = 0;
  *InstructionTask__EFFECTS(this) = NULL;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(InstructionTask_)\t%zu\n", sizeof(InstructionTask_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Compare function for sorting IssueItems.
 */
int
InstructionTask_compare(const void *this_pointer, const void *that_pointer);
#endif//$XCC_h

int
InstructionTask_compare(const void *this_pointer, const void *that_pointer)
{
  const_InstructionTask this = this_pointer;
  const_InstructionTask that = that_pointer;
  int result = InstructionTask_STARTDATE(this) - InstructionTask_STARTDATE(that);
  if (result == 0) {
    int this_position = InstructionTask_isInductive(this);
    int that_position = InstructionTask_isInductive(that);
    if (InstructionTask_isControl(this)) this_position += 255;
    if (InstructionTask_isControl(that)) that_position += 255;
    result = this_position - that_position;
    if (result == 0) {
      result = InstructionTask_INSTANCE(this) - InstructionTask_INSTANCE(that);
      if (result == 0) {
        result = InstructionTask_INDEX(this) - InstructionTask_INDEX(that);
      }
    }
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef $XCC_h
/**
 * Literal and index in the operand list
 */
struct LiteralOperand_ {
  Literal LITERAL; // The Literal if any.  
  int8_t  INDEX;   // The Literal index in the OperandsBuffer
};
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Representation of a native instruction.
 */
struct Instruction_ {
  //@args	Opcode opcode, Literal literal
  InstructionTask_ TASK[1];			// The InstructionTask of this Instruction.
  //@access INDEX	InstructionTask_INDEX(Instruction_TASK(this))
  //@access OPCODE	InstructionTask_OPCODE(Instruction_TASK(this))
  //@access INDEXED	InstructionTask_INDEXED(Instruction_TASK(this))
  //@mutate INDEXED	InstructionTask__INDEXED(Instruction_TASK(this))
  //@access FLAGS	InstructionTask_FLAGS(Instruction_TASK(this))
  //@mutate FLAGS	InstructionTask__FLAGS(Instruction_TASK(this))
  //@access isControl	InstructionTask_isControl(Instruction_TASK(this))
  //@access isVolatile	InstructionTask_isVolatile(Instruction_TASK(this))
  //@access isMemoryRead	InstructionTask_isMemoryRead(Instruction_TASK(this))
  //@access isMemoryWrite	InstructionTask_isMemoryWrite(Instruction_TASK(this))
  //@access isSPUpdate	InstructionTask_isSPUpdate(Instruction_TASK(this))
  //@access RESERVATION	InstructionTask_RESERVATION(Instruction_TASK(this))
  //@access PROCESSING	InstructionTask_PROCESSING(Instruction_TASK(this))
  //@access INSTANCE	InstructionTask_INSTANCE(Instruction_TASK(this))
  //@mutate INSTANCE	InstructionTask__INSTANCE(Instruction_TASK(this))
  //@access STARTDATE	InstructionTask_STARTDATE(Instruction_TASK(this))
  //@mutate STARTDATE	InstructionTask__STARTDATE(Instruction_TASK(this))
  //@access RELEASE	InstructionTask_RELEASE(Instruction_TASK(this))
  //@mutate RELEASE	InstructionTask__RELEASE(Instruction_TASK(this))
  //@access CRITICAL	InstructionTask_CRITICAL(Instruction_TASK(this))
  //@mutate CRITICAL	InstructionTask__CRITICAL(Instruction_TASK(this))
  //@access PREDCOUNT	InstructionTask_PREDCOUNT(Instruction_TASK(this))
  //@mutate PREDCOUNT	InstructionTask__PREDCOUNT(Instruction_TASK(this))
  //@access WRITECOUNT	InstructionTask_WRITECOUNT(Instruction_TASK(this))
  //@mutate WRITECOUNT	InstructionTask__WRITECOUNT(Instruction_TASK(this))
  //@access READCOUNT	InstructionTask_READCOUNT(Instruction_TASK(this))
  //@mutate READCOUNT	InstructionTask__READCOUNT(Instruction_TASK(this))
  //@access EFFECTS	InstructionTask_EFFECTS(Instruction_TASK(this))
  //@mutate EFFECTS	InstructionTask__EFFECTS(Instruction_TASK(this))
  OperandsBuffer_ BUFFER[1];			// Operands in decoded form.
  LiteralOperand_ LITERALOP[1];   // Literal operand if any
  //@access LITERAL LiteralOperand_LITERAL(Instruction_LITERALOP(this))
  //@access LITERALINDEX LiteralOperand_INDEX(Instruction_LITERALOP(this))
  //@access hasLiteral  (Instruction_literal(this) != NULL)
  PlatformUInt ADDRESS;				// Native code address.
  InstructionAction_ *ACTIONS;			// This InstructionTask actions (writes then reads).
};
#endif//$XCC_h

Instruction
Instruction_Ctor(Instruction this,
                 Opcode opcode, Literal literal)
{
  Scheduling scheduling = Opcode_scheduling(opcode);
  Reservation reservation = Scheduling_reservation(scheduling);
  InstructionTask_Ctor(Instruction_TASK(this), opcode);
  Instruction_LITERAL(this) = literal;
  Instruction_LITERALINDEX(this) = -1;
  *Instruction__ADDRESS(this) = (PlatformInt)-1;
  *Instruction__ACTIONS(this) = NULL;  
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Instruction_)\t%zu\n", sizeof(Instruction_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Decode an Instruction from Decoding and codeWords.
 */
Instruction
Instruction_Decode(Instruction this, Decoding decoding, const void *codeWords);
#endif//$XCC_h

#ifndef NDECODE
Instruction
Instruction_Decode(Instruction this, Decoding decoding, const void *codeWords)
{
  OperandsBuffer buffer = Instruction_BUFFER(this);
  Opcode opcode = (*Decoding_decode(decoding))(codeWords);
  Operands operands = Opcode_operands(opcode);
  Instruction_Ctor(this, opcode, NULL);
  (*Operands_decode(operands))(codeWords, buffer);
  return this;
}
#endif//NDECODE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Instruction Opcode.
 */
static inline Opcode
Instruction_opcode(const_Instruction this)
{
  return Instruction_OPCODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Instruction start date.
 */
static inline int
Instruction_startDate(const_Instruction this)
{
  return Instruction_STARTDATE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Instruction start date.
 */
static inline void
Instruction_setStartDate(Instruction this, int startDate)
{
  *Instruction__STARTDATE(this) = startDate;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Instruction flags.
 */
static inline InstructionFlags
Instruction_flags(const_Instruction this)
{
  return Instruction_FLAGS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set Instruction flags.
 */
static inline void
Instruction_setFlags(Instruction this, InstructionFlags flags)
{
  *Instruction__FLAGS(this) |= flags;
}
#endif//$XCC_h

#ifdef $XCC_h
#define Instruction_setVolatile(this) \
  Instruction_setFlags((this), InstructionFlag_Volatile)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Clear Instruction flags.
 */
static inline void
Instruction_clearFlags(Instruction this, InstructionFlags flags)
{
  *Instruction__FLAGS(this) &= ~flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Instruction Indexed.
 */
static inline Indexed
Instruction_indexed(const_Instruction this)
{
  return Instruction_INDEXED(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Instruction Indexed.
 */
static inline void
Instruction_setIndexed(Instruction this, Indexed indexed)
{
  *Instruction__INDEXED(this) = indexed;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Instruction InstructionAction(s).
 */
static inline const InstructionAction_ *
Instruction_actions(const_Instruction this)
{
  return Instruction_ACTIONS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Instruction Literal.
 */
static inline Literal
Instruction_literal(const_Instruction this)
{
  return Instruction_LITERAL(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Instruction Literal.
 */
static inline void
Instruction_setLiteral(Instruction this, Literal literal)
{
  Instruction_LITERAL(this) = literal;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Instruction Literal's index.
 */
static inline int8_t
Instruction_literalIndex(const_Instruction this)
{
  return Instruction_LITERALINDEX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Instruction Literal's index.
 */
static inline void
Instruction_setLiteralIndex(Instruction this, int8_t index)
{
  Instruction_LITERALINDEX(this) = index;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Instruction Bundling class.
 */
static inline Bundling
Instruction_bundling(const_Instruction this)
{
  Opcode opcode = Instruction_OPCODE(this);
  Scheduling scheduling = Opcode_scheduling(opcode);
  return Scheduling_bundling(scheduling);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Compute the size of the Instruction_ACTIONS.
 */
size_t
Instruction_sizeActions(const_Instruction instruction);
#endif//$XCC_h

#define InstructionTask_ACTIONS_MAXCOUNT ( \
  (Register__MaxStorageCells)*Opcode__MaxActions + 4 \
)

size_t
Instruction_sizeActions(const_Instruction instruction)
{
  return sizeof(InstructionAction_)*InstructionTask_ACTIONS_MAXCOUNT;
}

#ifdef $XCC_h
/**
 * Make this Instruction ACTIONS array.
 *
 * For control Instruction(s), the WAR latency is modified to ensure it is non-zero.
 * This trick is motivated by the following sample code, where register $b1 is WAR:
 *
 * 	BR $b1, target	# unscheduled
 * 	CMP $b1 = ...	# unscheduled
 *
 * Without this trick, instruction scheduling could produce code like:
 *
 * 	BR $b1, target	# cycle 0
 * 	CMP $b1 = ...	# cycle 0
 *
 * However, the compiler needs the Instruction(s) in semantic order and in particular
 * the control Instruction appear last in bundles. The code would be normalized to:
 *
 * 	CMP $b1 = ...	# cycle 0
 * 	BR $b1, target	# cycle 0
 *
 * This is incorrect reordering. Forcing a non-zero WAR latency will ensure:
 *
 * 	BR $b1, target	# cycle 0
 * 	CMP $b1 = ...	# cycle 1
 */
const InstructionAction_ *
Instruction_makeActions(Instruction this, InstructionAction_ *actions);
#endif//$XCC_h

const InstructionAction_ *
Instruction_makeActions(Instruction this, InstructionAction_ *actions)
{
  int writeCount = 0, readCount = 0;
  Opcode opcode = Instruction_OPCODE(this);
  const_RegisterActionArray actionArray = Opcode_actionArray(opcode);
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(this));
  InstructionAction_ *write_actions = actions, *read_actions = NULL;
  InstructionFlags flags = Instruction_FLAGS(this);
  bool war = (flags & InstructionFlag_Control) != 0;
  InstructionAction_ register_action_, volatile_action_;
  InstructionAction_ control_action_, memory_action_;
  register_action_.__.RAW = 0; register_action_.__.WAR = war; register_action_.__.WAW = 1;
  volatile_action_.__.RAW = 1; volatile_action_.__.WAR = 1; volatile_action_.__.WAW = 1;
  control_action_.__.RAW = 1; control_action_.__.WAR = 0; control_action_.__.WAW = 1;
  memory_action_.__.RAW = 1; memory_action_.__.WAR = 1; memory_action_.__.WAW = 1;
  volatile_action_.__.CELL = StorageCell__Volatile; volatile_action_.__.STAGE = 0;
  control_action_.__.CELL = StorageCell__Control; control_action_.__.STAGE = 0;
  memory_action_.__.CELL = StorageCell__Memory; memory_action_.__.STAGE = 0;
  // Write RegisterActions.
  RegisterActionArray_FORWRITE_RegisterAction(actionArray, action) {
    int rank = RegisterAction_rank(action);
    Register registre = rank > 0?
                        (Register)decoded[rank - 1]:
                        RegisterAction_register(action);
    const_RegisterStorageCells storageCells = Register_storageCells(registre);
    if (Register_isWired(registre)) continue;
    register_action_.__.STAGE = RegisterAction_stage(action);
    RegisterStorageCells_FOREACH_StorageCell(storageCells, cell) {
      register_action_.__.CELL = cell;
      if (cell != StorageCell__Control) {
        write_actions[writeCount++] = register_action_;
      }
    } RegisterStorageCells_ENDEACH_StorageCell;
  } RegisterActionArray_ENDWRITE_RegisterAction;
  if (flags & InstructionFlag_Volatile) {
    write_actions[writeCount++] = volatile_action_;
  }
  if (flags & InstructionFlag_Control) {
    write_actions[writeCount++] = control_action_;
  }
  if (flags & InstructionFlag_MemoryWrite) {
    write_actions[writeCount++] = memory_action_;
  }
  read_actions = actions + writeCount;
  // Read RegisterActions.
  RegisterActionArray_FORREAD_RegisterAction(actionArray, action) {
    int rank = RegisterAction_rank(action);
    Register registre = rank > 0?
                        (Register)decoded[rank - 1]:
                        RegisterAction_register(action);
    const_RegisterStorageCells storageCells = Register_storageCells(registre);
    if (Register_isWired(registre)) continue;
    register_action_.__.STAGE = RegisterAction_stage(action);
    RegisterStorageCells_FOREACH_StorageCell(storageCells, cell) {
      register_action_.__.CELL = cell;
      read_actions[readCount++] = register_action_;
    } RegisterStorageCells_ENDEACH_StorageCell;
  } RegisterActionArray_ENDREAD_RegisterAction;
  if (!(flags & InstructionFlag_Control)) {
    read_actions[readCount++] = control_action_;
  }
  if (flags & InstructionFlag_MemoryRead) {
    read_actions[readCount++] = memory_action_;
  }
  Except_CHECK(writeCount + readCount <= InstructionTask_ACTIONS_MAXCOUNT);
  *Instruction__WRITECOUNT(this) = writeCount;
  *Instruction__READCOUNT(this) = readCount;
  *Instruction__ACTIONS(this) = actions;
  return actions;
}

#ifdef $XCC_h
/**
 * Kill this Instruction ACTIONS array.
 */
static inline void
Instruction_killActions(Instruction this, Memory memory)
{
  if (memory != NULL) {
    Memory_free(memory, Instruction_ACTIONS(this));
  }
  *Instruction__WRITECOUNT(this) = 0;
  *Instruction__READCOUNT(this) = 0;
  *Instruction__ACTIONS(this) = NULL;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Encode this Instruction in the supplied code buffer.
 */
void
Instruction_encode(Instruction this, void *codeBuffer);
#endif//$XCC_h

void
Instruction_encode(Instruction this, void *codeBuffer)
{
  Opcode opcode = Instruction_OPCODE(this);
  Operands operands = Opcode_operands(opcode);
  OperandsBuffer buffer = Instruction_BUFFER(this);
  OperandDecoded *decoded = OperandsBuffer_DECODED(buffer);

  // A literal of an instruction is set by Operation_getOperandDecoded
  // while adding this instruction to a native stream.
  Literal literal = Instruction_LITERAL(this);
  if (literal != NULL) {
    int8_t index = Instruction_LITERALINDEX(this);
    
    // These are basic block identities
    if (Literal_isLabel(literal))
    {
      Label label = Literal_label(literal);      
      decoded[index] = Label_value(label) - Instruction_ADDRESS(this);
    }
    // Symbols are used for functions and global data
    else if (Literal_isSymbol(literal))
    {
      Symbol symbol = Literal_symbol(literal);
      uint64_t value = Symbol_value(symbol);
      
      // if this instruction is control, the encoded value is PC-relative
      // so this instruction's address should be substracted
      if (Instruction_isControl(this))
        decoded[index] = value==0 ? 0 : value - Instruction_ADDRESS(this);      
      // otherwise it is materialization of an immediate, i.e. setting a register
      // with an address of a global data, e.g. make $r0 = L.str
      else
        decoded[index] = value;
    }
    
  }
  (*Operands_encode(operands))(buffer, codeBuffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
#ifdef MDT_CORE_st231
  OpcodeEncoded_ codeWords[] = {
    { { 0x2502444c } },	//  stw 36[$r12] = $r17
    { { 0x2502040c } },	//  stw 32[$r12] = $r16
    { { 0x2001824c } },	//  ldw $r9 = 24[$r12]
    { { 0x00010240 } },	//  mov $r16 = $r9
    { { 0x30000000 } },	//  call $r63 = <Module>:fclose
    { { 0x2001424c } },	//  ldw $r9 = 20[$r12]
    { { 0x00010240 } },	//  mov $r16 = $r9
    { { 0x30000000 } },	//  call $r63 = <Module>:fclose
    { { 0x08001400 } },	//  mov $r16 = 1
    { { 0x30000000 } },	//  call $r63 = <Module>:__io_ftable_get_stdentry
  };
  printf("Instruction_decode:\n");
  {
    int count = sizeof(codeWords)/sizeof(*codeWords), i;
    for (i = 0; i < count; i++) {
      Instruction_ instruction[1];
      OpcodeEncoded opcodeCodeWords = codeWords + i;
      Decoding decoding = OpcodeEncoded_WORDS(opcodeCodeWords)[1]?
                          Decoding_st231_ANYX: Decoding_st231_ANY;
      Instruction_Decode(instruction, decoding, opcodeCodeWords);
      printf("\t"), Instruction_asmPrint(instruction, stdout, NULL), printf("\n");
    }
  }
#endif//MDT_CORE_st231
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty-print this Instruction.
 */
bool
Instruction_asmPrint(const_Instruction this, FILE *file, void *stream);
#endif//$XCC_h

#ifndef _NTRACE
bool
Instruction_asmPrint(const_Instruction this, FILE *file, void *stream)
{
  NativeStream nativeStream = (NativeStream)stream;
  Opcode opcode = Instruction_OPCODE(this);
  Operands operands = Opcode_operands(opcode);
  const_OperandsBuffer buffer = Instruction_BUFFER(this);
  const OperandDecoded *decoded = OperandsBuffer_DECODED(buffer);
  const char *syntax = Opcode_syntax(opcode), *q;
  if (file == NULL) file = stderr;
  for (q = syntax; *q != 0; q++) {
    if (q[0] == '%') {
      if (q[1] >= '0' && q[1] <= '9') {
        int rank = q[1] - '0';
        if (rank == 0) {
          fputs(Opcode_mnemonic(opcode), file);
          // TODO: Display other InstructionFlags
          if (Instruction_isVolatile(this))
            fputs(".vol", file);
        } else {
          int operandsIndex = rank - 1;
          Operand operand = Operands_access(operands, operandsIndex);
          Immediate immediate = Operand_immediate(operand);
          OperandType type = Operand_type(operand);
          if (type == OperandType_RegClass) {
            Register registre = (Register)decoded[operandsIndex];
            fputs(Register_names(registre)[0], file);
          } else if (type == OperandType_Immediate) {
            Literal literal = Instruction_literal(this);
            if (nativeStream != NULL && literal != NULL &&
                Literal_handle(literal) != NULL &&
                rank == Operands_relocatableRank(operands)) {
              void *handle = Literal_handle(literal);
              long long offset = (long long)Literal_offset(literal);
              const char *format = NULL, *name = NULL;
              char print[16], origin[32], *relocate = NULL;
              Relocation relocation = Literal_relocation(literal);

              if ((offset) && (relocation)) format = "@%s(%s)%+lld";
              else if (offset) format = "%s%+lld";
              else if (relocation) format = "@%s(%s)";
              else format = "%s";

              if (relocation) {
                relocate = strchr(Relocation_Id(relocation), (int) '_') + 1;
                switch (Relocation_relative(relocation)) {
                  case RelocationRelative_GOT:
                    if (strstr(relocate, "funcdesc") != NULL) {
                      strcpy(origin, strchr(relocate, (int) '_') + 1);
                      strcat(origin, "_funcdesc");
                      relocate = origin;
                    }
                    break;
                  case RelocationRelative_TP:
                  case RelocationRelative_GP:
                    if (relocate[0] == 's')
                      relocate = strrchr(Relocation_Id(relocation), (int) '_') + 1;
                    break;
                  default:
                    fprintf(stderr, "Unknown Relocation type encountered in instruction\n");
                    break;
                }
              }

              if (Literal_isLabel(literal)) {
                const char *(*makeLabelName)(const void *Label) =
                    NativeStream_MAKELABELNAME(nativeStream);
                name = makeLabelName != NULL ? (*makeLabelName)(handle) : print;
                if (name == print) sprintf(print, "L?%p", handle);
              } else {
                const char *(*makeSymbolName)(const void *Symbol) =
                    NativeStream_MAKESYMBOLNAME(nativeStream);
                name = makeSymbolName != NULL ? (*makeSymbolName)(handle) : print;
                if (name == print) sprintf(print, "S?%p", handle);
              }

              if (relocation)
                fprintf(file, format, relocate, name, offset);
              else
                fprintf(file, format, name, offset);
            } else {
              long long value = (long long)decoded[operandsIndex];
              const_ImmediateRelocations relocations = Immediate_relocations(immediate);
              Relocation relocation = ImmediateRelocations_count(relocations)?
                                      ImmediateRelocations_access(relocations, 0):
                                      Relocation__UNDEF;
              const char *format = Relocation_isRelativePC(relocation)? ".%+lld": "%lld";
              fprintf(file, format, value);
            }
          } else if (type == OperandType_Modifier) {
            Modifier modifier = Operand_modifier(operand);
            ModifierMember member = (ModifierMember)decoded[operandsIndex];
            for (int i = 0; i < strlen(ModifierMember_name(member)); i++)
              fputc(tolower(ModifierMember_name(member)[i]), file);
          } else Except_NEVER(true);
        }
        q++;
      } else if (q[1] == '%') {
        fputc(q[1], file);
        q++;
      }
    } else {
      fputc(q[0], file);
    }
  }
  if (Instruction_STARTDATE(this) >= 0) {
    fprintf(file, "\t## date %d", Instruction_STARTDATE(this));
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * InstructionGroup -- Group of Instruction(s) that issue in parallel.
 */
struct InstructionGroup_ {
  //@args
  Bundle BUNDLE;		// Bundle for this InstructionGroup.
  Template TEMPLATE;		// Template for this InstructionGroup.
  uint8_t COUNT;		// Count of Instruction(s) in this InstructionGroup.
  Instruction INSTRUCTIONS[Bundle__MAXINSTRUCTIONS];
  //@access INSTRUCTION	(InstructionGroup_INSTRUCTIONS(this)[0])
  //@mutate INSTRUCTION	(InstructionGroup_INSTRUCTIONS(this) + 0)
  BundlingBuffer_ BUFFER_;
  //@access BUFFER	InstructionGroup__BUFFER_(this)
  //@access BUNDLINGS	BundlingBuffer_BUNDLINGS(InstructionGroup_BUFFER(this))
  //@access PERMUTE	BundlingBuffer_PERMUTE(InstructionGroup_BUFFER(this))
};
#endif//$XCC_h

InstructionGroup
InstructionGroup_Ctor(InstructionGroup this)
{
  *InstructionGroup__BUNDLE(this) = Bundle__UNDEF;
  *InstructionGroup__TEMPLATE(this) = Template__UNDEF;
  *InstructionGroup__COUNT(this) = 0;
  *InstructionGroup__INSTRUCTION(this) = NULL;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(InstructionGroup_)\t%zu\n", sizeof(InstructionGroup_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Add an Instruction to this InstructionGroup.
 */
void
InstructionGroup_addInstruction(InstructionGroup this, Instruction instruction);
#endif//$XCC__h

void
InstructionGroup_addInstruction(InstructionGroup this, Instruction instruction)
{
  if (Bundle__MaxInstructions) {
    int count = InstructionGroup_COUNT(this);
    Bundling bundling = Instruction_bundling(instruction);
    InstructionGroup_INSTRUCTIONS(this)[count] = instruction;
    InstructionGroup_BUNDLINGS(this)[count] = bundling;
    *InstructionGroup__COUNT(this) = count + 1;
    Except_REQUIRE(count < Bundle__MaxInstructions);
  } else {
    Except_CHECK(InstructionGroup_INSTRUCTION(this) == NULL);
    *InstructionGroup__INSTRUCTION(this) = instruction;
  }
}

#ifdef $XCC__h
/**
 * Find a Template for this InstructionGroup.
 */
Template
InstructionGroup_findTemplate(InstructionGroup this, PlatformUInt nativePC);
#endif//$XCC__h

Template
InstructionGroup_findTemplate(InstructionGroup this, PlatformUInt nativePC)
{
  int count = InstructionGroup_COUNT(this);
  BundlingBuffer buffer = InstructionGroup_BUFFER(this);
  uint32_t hash = BundlingBuffer_sort(buffer, count);
#if BundleMatch_ENABLE
  Bundle bundle = BundleMatch_FindBundle(hash, nativePC);
  Template template = Bundle_template(bundle);
  Except_ENSURE(bundle != Bundle__UNDEF);
#else//!BundleMatch_ENABLE
  Bundle bundle = Bundle__UNDEF;
  Template template = Template__UNDEF;
#endif//BundleMatch_ENABLE
  *InstructionGroup__BUNDLE(this) = bundle;
  *InstructionGroup__TEMPLATE(this) = template;
  return template;
}

#ifdef $XCC__h
/**
 * Encode this InstructionGroup in the given code buffer.
 */
void
InstructionGroup_encode(InstructionGroup this, PlatformUInt nativePC, void *codeBuffer);
#endif//$XCC__h

void
InstructionGroup_encode(InstructionGroup this, PlatformUInt nativePC, void *codeBuffer)
{
  if (Bundle__MaxInstructions) {
    int count = InstructionGroup_COUNT(this), i;
    Template template = InstructionGroup_TEMPLATE(this);
    const uint8_t *restrict distances = Template_distances(template);
    Instruction *restrict instructions = InstructionGroup_INSTRUCTIONS(this);
    OpcodeEncoded_ templateCodeWords_[Bundle__MaxInstructions];
    const_OpcodeEncoded templateCodeWords[Bundle__MaxInstructions];
    if (count == 1) {
      int distance = distances[0];
      Instruction instruction = instructions[0];
      Opcode opcode = Instruction_OPCODE(instruction);
      *Instruction__ADDRESS(instruction) = nativePC + distance;
      templateCodeWords_[0] = *Opcode_encoded(opcode);
      Instruction_encode(instruction, templateCodeWords_ + 0);
      templateCodeWords[0] = templateCodeWords_ + 0;
    } else {
      Bundle bundle = InstructionGroup_BUNDLE(this);
      const uint8_t *restrict inverse = Bundle_inverse(bundle);
      BundlingBuffer bundlingBuffer = InstructionGroup_BUFFER(this);
      uint8_t *restrict permute = BundlingBuffer_PERMUTE(bundlingBuffer);
      for (i = 0; i < count; i++) {
        int distance = distances[i];
        int index = permute[inverse[i]];
        Instruction instruction = instructions[index];
        Opcode opcode = Instruction_OPCODE(instruction);
        *Instruction__ADDRESS(instruction) = nativePC + distance;
        templateCodeWords_[i] = *Opcode_encoded(opcode);
        Instruction_encode(instruction, templateCodeWords_ + i);
        templateCodeWords[i] = templateCodeWords_ + i;
      }
    }
    (*Template_encode(template))(templateCodeWords, codeBuffer);
  } else {
    Instruction instruction = InstructionGroup_INSTRUCTION(this);
    if (instruction != NULL) {
      Instruction_encode(instruction, codeBuffer);
      *Instruction__ADDRESS(instruction) = nativePC;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Print this InstructionGroup in assembler format.
 */
bool
InstructionGroup_asmPrint(const_InstructionGroup this, FILE *file, void *stream);
#endif//$XCC_h

#ifndef _NTRACE
bool
InstructionGroup_asmPrint(const_InstructionGroup this, FILE *file, void *stream)
{
  NativeStream nativeStream = stream;
  if (Bundle__MaxInstructions) {
    int count = InstructionGroup_COUNT(this), i, j;
    Template template = InstructionGroup_TEMPLATE(this);
    const Instruction *instructions = InstructionGroup_INSTRUCTIONS(this);
    const_BundlingBuffer bundlingBuffer = InstructionGroup_BUFFER(this);
    const uint8_t *permute = BundlingBuffer_PERMUTE(bundlingBuffer);
    Bundle bundle = InstructionGroup_BUNDLE(this);
    const uint8_t *inverse = Bundle_inverse(bundle);

    if (file == NULL)
      file = stderr;

    for (i = 0; i < count; i++) {
      int index = permute[inverse[i]];
      Instruction instruction = instructions[index];
      fputc('\t', file);
      Instruction_asmPrint(instruction, file, nativeStream);
      fputc('\n', file);
    }
  } else {
    Instruction instruction = InstructionGroup_INSTRUCTION(this);
    if (instruction != NULL) {
      Opcode opcode = Instruction_OPCODE(instruction);
      fputc('\t', file);
      Instruction_asmPrint(instruction, file, nativeStream);
      fputc('\n', file);
    }
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty-print this InstructionGroup.
 */
bool
InstructionGroup_pretty(const_InstructionGroup this, FILE *file, void *stream, int offset);
#endif//$XCC_h

#ifndef _NTRACE
bool
InstructionGroup_pretty(const_InstructionGroup this, FILE *file, void *stream, int offset)
{
  NativeStream nativeStream = stream;
  const uint8_t *codeBuffer = NativeStream_CODEBUFFER(nativeStream);
  const uint8_t *encoded = codeBuffer + offset;
  if (Bundle__MaxInstructions) {
    int count = InstructionGroup_COUNT(this), i, j;
    Template template = InstructionGroup_TEMPLATE(this);
    const uint8_t *distances = Template_distances(template);
    const Instruction *instructions = InstructionGroup_INSTRUCTIONS(this);
    const_BundlingBuffer bundlingBuffer = InstructionGroup_BUFFER(this);
    const uint8_t *permute = BundlingBuffer_PERMUTE(bundlingBuffer);
    Bundle bundle = InstructionGroup_BUNDLE(this);
    const uint8_t *inverse = Bundle_inverse(bundle);
    int codeSizes[Bundle__MAXINSTRUCTIONS];
    if (file == NULL) file = stderr;
    codeSizes[0] = distances[1];
    for (i = 1; i < count - 1; i++) {
      codeSizes[i] = distances[i + 1] - distances[i];
    }
    codeSizes[count - 1] = Template_increment(template);
    if (count > 1) codeSizes[count - 1] -= distances[count - 1];
    for (i = 0; i < count; i++) {
      int index = permute[inverse[i]];
      Instruction instruction = instructions[index];
      fprintf(file, "%08zx\t", encoded - codeBuffer);
      for (j = Decoding_MAXSIZE - 1; j >= codeSizes[i]; j--) {
        fprintf(file, "   ");
      }
      for (j = codeSizes[i] - 1; j >= 0; j--) {
        fprintf(file, "%02x ", encoded[j]);
      }
      fputc('\t', file);
      Instruction_asmPrint(instruction, file, nativeStream);
      fputc('\n', file);
      encoded += codeSizes[i];
    }
  } else {
    Instruction instruction = InstructionGroup_INSTRUCTION(this);
    if (instruction != NULL) {
      Opcode opcode = Instruction_OPCODE(instruction);
      int codeSize = Opcode_increment(opcode), j;
      fprintf(file, "%08zx\t", encoded - codeBuffer);
      for (j = Decoding_MAXSIZE - 1; j >= codeSize; j--) {
        fprintf(file, "   ");
      }
      for (j = codeSize - 1; j >= 0; j--) {
        fprintf(file, "%02x ", encoded[j]);
      }
      fputc('\t', file);
      Instruction_asmPrint(instruction, file, nativeStream);
      fputc('\n', file);
    }
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  Processor processor = Processor__DEFAULT;
  NativeStream stream = NativeStream_new(Memory_Root, 10);
  NativeBlock block_1 = NativeStream_pushNativeBlock(stream, processor, 0, NULL);
  NativeBlock block_2 = NativeStream_pushNativeBlock(stream, processor, 0, NULL);
  static uint32_t codeBuffer[64] = { 0xdeadbeef, 0xdeadbeef };
#ifdef MDT_TARGET_arm
  RegisterSet_ regmask[1];
  RegisterSet_insert(regmask, Register_arm_GR0);
  RegisterSet_insert(regmask, Register_arm_GR1);
  RegisterSet_insert(regmask, Register_arm_GR2);
  RegisterSet_insert(regmask, Register_arm_LR);
  NativeBlock_makeInstruction_arm_cond_amode4L_rnNoPC_reglistNoPC(
      block_1, Operator_opcode(Operator_arm_stm_w_3noPC_1cond_2amode4S_3noPC_4reglist, processor),
      ModifierMember_arm_cond_none, ModifierMember_arm_amode4L_fd, Register_arm_SP, regmask);
  NativeBlock_makeInstruction_arm_cond_rdNoPC_rnNoPC(
      block_1, Operator_opcode(
          Operator_arm_ldr_2noPC_1cond_3general,
          processor),
      ModifierMember_arm_cond_none, Register_arm_IP, Register_arm_SP);
  NativeBlock_makeInstruction_arm_cond_rn_immed8(
      block_1, Operator_opcode(Operator_arm_cmp_imm_1cond_2general_3immed8, processor),
      ModifierMember_arm_cond_none, Register_arm_IP,
      ImmediateValue(32));
#endif//MDT_TARGET_arm
#ifdef MDT_CORE_st231
  NativeBlock_makeInstruction_st200_idest_src1_isrc2(
      block_1, Opcode_st231_add_idest_src1_isrc2,
      Register_st200_GR12, Register_st200_GR12,
      ImmediateValue(-32));
  NativeBlock_makeInstruction_st200_isrc2_src1_src2(
      block_1, Opcode_st231_stw_isrc2_src1_src2,
      ImmediateValue(24),
      Register_st200_GR12, Register_st200_GR63);
  NativeBlock_makeInstruction_st200_isrc2_src1_src2(
      block_1, Opcode_st231_stw_isrc2_src1_src2,
      ImmediateValue(32),
      Register_st200_GR12, Register_st200_GR16);
  NativeBlock_makeInstruction_st200_isrc2_src1_src2(
      block_1, Opcode_st231_stw_isrc2_src1_src2,
      ImmediateValue(36),
      Register_st200_GR12, Register_st200_GR17);
  NativeBlock_makeInstruction_st200_isrc2_src1_src2(
      block_1, Opcode_st231_stw_isrc2_src1_src2,
      ImmediateValue(40),
      Register_st200_GR12, Register_st200_GR18);
  NativeBlock_makeInstruction_st200_idest_xsrc2_src1(
      block_2, Opcode_st231_ldw_idest_xsrc2_src1,
      Register_st200_GR9,
      ImmediateValue(32),
      Register_st200_GR12);
  NativeBlock_makeInstruction_st200_ibdest_src1_isrc2(
      block_2, Opcode_st231_cmpgt_ibdest_src1_isrc2,
      Register_st200_BR0, Register_st200_GR9,
      ImmediateValue(32));
  NativeBlock_makeInstruction_st200_btarg(
      block_2, Opcode_st231_call_btarg,
      ImmediateValue(4));
  NativeBlock_makeInstruction_st200_idest_isrc2_src1(
      block_2, Opcode_st231_ldw_idest_isrc2_src1,
      Register_st200_GR26,
      ImmediateValue(80),
      Register_st200_GR12);
  NativeBlock_makeInstruction_st200_dest_src2(
      block_2, Opcode_st231_mov_dest_src2,
      Register_st200_GR27, Register_st200_GR16);
  NativeBlock_makeInstruction_st200_dest_src1_src2(
      block_2, Opcode_st231_add_dest_src1_src2,
      Register_st200_GR27, Register_st200_GR26,
      Register_st200_GR10);
#endif//MDT_CORE_st231
#ifdef MDT_TARGET_st200
  {
    size_t codeSize = NativeStream_getCodeSize(stream, 0);
    void *codeBuffer = calloc(1, codeSize);
    NativeStream_encode(stream, 0, codeBuffer);
    printf("Processor %s:\n", Processor_Id(processor));
    NativeStream_pretty(stream, stdout);
    NativeStream_FOREACH_NativeBlock(stream, nativeBlock) {
      NativeBlock_FOREACH_Instruction(nativeBlock, instruction) {
        int writeCount = 0, readCount = 0, i;
        Opcode opcode = Instruction_OPCODE(instruction);
        InstructionAction_ *actions = alloca(Instruction_sizeActions(instruction));
        Instruction_makeActions(instruction, actions);
        writeCount = Instruction_WRITECOUNT(instruction);
        readCount = Instruction_READCOUNT(instruction);
        fputc('\t', stdout);
        Instruction_asmPrint(instruction, stdout, stream);
        for (i = 0; i < writeCount; i++) {
          StorageCell cell = InstructionAction_CELL(actions + i);
          printf(" \tWrite(%s)", StorageCell_name(cell));
        }
        for (i = 0; i < readCount; i++) {
          StorageCell cell = InstructionAction_CELL(actions + writeCount + i);
          printf(" \tRead(%s)", StorageCell_name(cell));
        }
        fputc('\n', stdout);
        Instruction_killActions(instruction, NULL);
      } NativeBlock_ENDEACH_Instruction;
    } NativeStream_ENDEACH_NativeBlock;
    free(codeBuffer);
  }
  NativeStream_delete(stream);
#endif//MDT_TARGET_st200
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Stack of Instruction(s).
 */
struct InstructionStack_ {
  //@args	Memory memory, int maxCount
  PtrSeq_ __;		// Underlying PtrSeq.
};
#endif//$XCC_h

#ifdef $XCC_h
static inline PtrSeq
InstructionStack__(InstructionStack this) {
  return InstructionStack____(this);
}
static inline const_PtrSeq
InstructionStack__const(const_InstructionStack this) {
  return InstructionStack____(this);
}
#endif//$XCC_h

InstructionStack
InstructionStack_Ctor(InstructionStack this,
                      Memory memory, int maxCount)
{
  PtrSeq_Ctor(InstructionStack____(this), memory, maxCount);
  return this;
}

void
InstructionStack_Dtor(InstructionStack this)
{
  PtrSeq_Dtor(InstructionStack____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(InstructionStack_)\t%zu\n", sizeof(InstructionStack_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Iterate over this InstructionStack Instruction(s).
 */
#define InstructionStack_FOREACH_Instruction(this, instruction) { \
  const_PtrSeq InstructionStack____ = InstructionStack__const(this); \
  PtrSeq_FOREACH(InstructionStack____, Instruction, instruction) {
#define InstructionStack_ENDEACH_Instruction \
  } PtrSeq_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this InstructionStack Instruction(s).
 */
#define InstructionStack_FORBACK_Instruction(this, instruction) { \
  const_PtrSeq InstructionStack____ = InstructionStack__const(this); \
  PtrSeq_FORBACK(InstructionStack____, Instruction, instruction) {
#define InstructionStack_ENDBACK_Instruction \
  } PtrSeq_ENDBACK \
}
#endif//$XCC_h

#ifdef $XCC_h
#define InstructionStack_resize(this, maxCount) \
  PtrSeq_resize(InstructionStack__(this), maxCount)
#define InstructionStack_empty(this) \
  PtrSeq_empty(InstructionStack__(this))
#define InstructionStack_memory(this) \
  PtrSeq_memory(InstructionStack__const(this))
#define InstructionStack_base_(this) (Instruction *) \
  PtrSeq_base_(InstructionStack__(this))
#define InstructionStack_base(this) (const Instruction *) \
  PtrSeq_base(InstructionStack__const(this))
#define InstructionStack_past(this) (const Instruction *) \
  PtrSeq_past(InstructionStack__const(this))
#define InstructionStack_isEmpty(this) \
  PtrSeq_isEmpty(InstructionStack__const(this))
#define InstructionStack_isSingle(this) \
  PtrSeq_isSingle(InstructionStack__const(this))
#define InstructionStack_isFull(this) \
  PtrSeq_isFull(InstructionStack__const(this))
#define InstructionStack_count(this) \
  PtrSeq_count(InstructionStack__const(this))
#define InstructionStack_access(this, index) (Instruction) \
  PtrSeq_access(InstructionStack__const(this), index)
#define InstructionStack_bottom(this) (Instruction) \
  PtrSeq_bottom(InstructionStack__const(this))
#define InstructionStack_top(this) (Instruction) \
  PtrSeq_top(InstructionStack__const(this))
#define InstructionStack_pop(this) \
  PtrSeq_pop(InstructionStack__(this))
#define InstructionStack_push(this, instruction) \
  PtrSeq_push(InstructionStack__(this), instruction)
#define InstructionStack_push2(this, instruction) \
  PtrSeq_push2(InstructionStack__(this), instruction)
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Instruction module.
 */
#define Instruction_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Instruction module.
 */
#define Instruction_FINI()
#endif//$XCC__h

#if XCC__C
static void
Instruction_TEST(void)
{
#include "AIR/Instruction_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(Instruction_TEST);
  return 0;
}
#endif

