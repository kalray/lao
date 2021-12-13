#ifdef $XCC_h
/*
 * !!!!	Procedure.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu)
 * Duco van Amstel (duco.van-amstel@kalray.eu)
 *
 * Copyright 2010 - 2013 Kalray.
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
 * @brief Procedure implementation.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/Procedure.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * IndexedFactory for Procedure(s).
 */
extern IndexedFactory_ Procedure_Factory[1];
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Procedure flags.
 */
typedef enum {
  ProcedureFlag_Built = 0x1,		//!< Procedure CodeRegion(s) are built.
} enum_ProcedureFlag;
typedef uint8_t ProcedureFlags;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ ProcedureFlag_Table[1];
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ ProcedureFlagEntries[] = {
  ETableENTRY(ProcedureFlag, Built),
  ETableENTRY_NULL
};
ETable_ ProcedureFlag_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Represent a Procedure in a Program.
 */
struct Procedure_;
#endif//$XCC_h

#ifdef $XCC__h
struct Procedure_ {
  //@args	Memory memory, Symbol symbol, struct Program_ *program
  Indexed_ __;			//!< A Procedure_ ISA Indexed_.
  //@access IDENTITY	Indexed_IDENTITY(Procedure____(this))
  //@access INDEX	Indexed_INDEX(Procedure____(this))
  //@mutate INDEX	Indexed__INDEX(Procedure____(this))
  struct Program_ *PROGRAM;		//!< This Procedure enclosing Program.
  //@access DEDICATED	Program_DEDICATED(Procedure_PROGRAM(this))
  //@access LITERALS	Program_LITERALS(Procedure_PROGRAM(this))
  //@access PROCESSOR	Program_PROCESSOR(Procedure_PROGRAM(this))
  //@access CONVENTION	Program_CONVENTION(Procedure_PROGRAM(this))
  IBList_ TEMPORARIES[1];		//!< Temporary(ies) for this Procedure.
  HTable_ REMATERMAP[1];		//!< Map Temporary to its rematerialization.
  HTable_ HOMEABLEMAP[1];		//!< Map Temporary to its home Temporary.
  IDList_ BLOCKCHAIN[1];		//!< The BasicBlock chain of this Procedure.
  //@access MEMORY	IDList_memory(Procedure_BLOCKCHAIN(this))
  //@access BLOCKCOUNT	IDList_count(Procedure_BLOCKCHAIN(this))
  IDList_ DISCARDED[1];			//!< The list of discarded BasicBlock(s).
  IDList_ CODEREGIONS[1];		//!< List of CodeRegion(s) in creation order.
  //@access FIRSTREGION	IDList_firstItem(Procedure_CODEREGIONS(this))
  //@access LASTREGION	IDList_lastItem(Procedure_CODEREGIONS(this))
  HTable_ LOOPSCOPES[1];		//!< Map header BasicBlock to LoopScope.
  DataList_ DATALIST[1];		//!< Data section of procedure.
  DataSection SECTION;		        //!< In which section this function is located
  uint8_t ALIGN;			//!< Alignment of this function
  Symbol SYMBOL;			//!< This Procedure Symbol.
  Frame_ FRAME[1];			//!< This Procedure stack Frame.
  //@access isTail      Frame_SCRATCHSIZE(Procedure_FRAME(this)) == 0
  Symbol SYMBOLSP;			//!< This Procedure Stack Pointer Symbol.
  Symbol SYMBOLFP;			//!< This Procedure Frame Pointer Symbol.
  Symbol SYMBOLGP;			//!< This Procedure Global Pointer Symbol.
  ProcedureFlags FLAGS;			//!< This Procedure flags.
  //@access isBuilt		((Procedure_FLAGS(this) & ProcedureFlag_Built) != 0)
  PlatformUInt ADDRESS;			//!< This Procedure native code address
  uint32_t  COUNTER;        //!< This Procedure execution counter
};
#endif//$XCC__h

#ifdef $XCC__h
static inline Indexed
Procedure__(Procedure this)
{
  return Procedure____(this);
}
static inline const_Indexed
Procedure__const(const_Procedure this)
{
  return Procedure____(this);
}
#endif//$XCC__h

Procedure
Procedure_Ctor(Procedure this,
               Memory memory, Symbol symbol, struct Program_ *program)
{
  Indexed_Ctor(Procedure____(this), Procedure_Factory);
  *Procedure__PROGRAM(this) = program;
  IBList_Ctor(Procedure_TEMPORARIES(this), memory, sizeof(Temporary_), 30);
  HTable_Ctor(Procedure_REMATERMAP(this), memory, 16);
  HTable_Ctor(Procedure_HOMEABLEMAP(this), memory, 16);
  IDList_Ctor(Procedure_BLOCKCHAIN(this), memory);
  IDList_Ctor(Procedure_DISCARDED(this), memory);
  IDList_Ctor(Procedure_CODEREGIONS(this), memory);
  HTable_Ctor(Procedure_LOOPSCOPES(this), memory, 16);
  DataList_Ctor(Procedure_DATALIST(this), memory);
  Procedure_SECTION(this) = NULL;
  Procedure_ALIGN(this) = 0;
  *Procedure__SYMBOL(this) = symbol;
  *Symbol__CLASS(symbol) = SymbolClass_FUNC;
  Frame_Ctor(Procedure_FRAME(this), memory);
  {
    SymbolTable symbolTable = Program_SYMBOLTABLE(program);
    Symbol symbolSP = SymbolTable_makeSymbol(symbolTable, ".SP_block");
    Symbol symbolFP = SymbolTable_makeSymbol(symbolTable, ".FP_block");
    Symbol symbolGP = SymbolTable_makeSymbol(symbolTable, ".GP_block");
    Symbol_setClasses(symbolSP, SymbolClass_BLOCK, SymbolStore_AUTO, SymbolExport_LOCAL);
    Symbol_setClasses(symbolFP, SymbolClass_BLOCK, SymbolStore_AUTO, SymbolExport_LOCAL);
    Symbol_setClasses(symbolGP, SymbolClass_BLOCK, SymbolStore_AUTO, SymbolExport_LOCAL);
    *Procedure__SYMBOLSP(this) = symbolSP;
    *Procedure__SYMBOLFP(this) = symbolFP;
    *Procedure__SYMBOLGP(this) = symbolGP;
    *Symbol__PROCEDURE(symbolSP) = this;
    *Symbol__PROCEDURE(symbolFP) = this;
    *Symbol__PROCEDURE(symbolGP) = this;
  }
  *Procedure__FLAGS(this) = 0;
  *Procedure__ADDRESS(this) = 0;
  *Procedure__COUNTER(this) = 0;
  return this;
}

void
Procedure_Dtor(Procedure this)
{
  IBList_Dtor(Procedure_TEMPORARIES(this));
  HTable_empty(Procedure_REMATERMAP(this), NULL);
  HTable_Dtor(Procedure_REMATERMAP(this));
  HTable_empty(Procedure_HOMEABLEMAP(this), NULL);
  HTable_Dtor(Procedure_HOMEABLEMAP(this));
  IDList_empty(Procedure_BLOCKCHAIN(this), (IDListItemRelease)BasicBlock_Dtor);
  IDList_Dtor(Procedure_BLOCKCHAIN(this));
  IDList_empty(Procedure_DISCARDED(this), (IDListItemRelease)BasicBlock_Dtor);
  IDList_Dtor(Procedure_DISCARDED(this));
  IDList_empty(Procedure_CODEREGIONS(this), (IDListItemRelease)CodeRegion_Dtor);
  IDList_Dtor(Procedure_CODEREGIONS(this));
  HTable_empty(Procedure_LOOPSCOPES(this), (HTableValueRelease)LoopScope_Dtor);
  HTable_Dtor(Procedure_LOOPSCOPES(this));
  DataList_Dtor(Procedure_DATALIST(this));
  Frame_Dtor(Procedure_FRAME(this));
}

size_t
Procedure_Size(Memory memory, Symbol symbol, struct Program_ *program)
{
  return sizeof(Procedure_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Procedure_)\t%zu\n", sizeof(Procedure_));
}
#endif//$XCC__c

#ifdef $XCC__h
Procedure
Procedure_new(Memory parent, Symbol symbol, struct Program_ *program);
#endif//$XCC__h

Procedure
Procedure_new(Memory parent, Symbol symbol, struct Program_ *program)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Procedure_Size(memory, symbol, program);
  void *alloc = Memory_alloc(memory, size);
  return Procedure_Ctor(alloc, memory, symbol, program);
}

#ifdef $XCC__h
Procedure
Procedure_delete(Procedure this);
#endif//$XCC__h

Procedure
Procedure_delete(Procedure this)
{
  if (this != NULL) {
    Memory memory = Procedure_MEMORY(this);
#ifndef _NDTOR
    Procedure_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Procedure Memory.
 */
Memory
Procedure_memory(Procedure this);
#endif//$XCC_h

Memory
Procedure_memory(Procedure this)
{
  return Procedure_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This Procedure enclosing Program.
 */
struct Program_ *
Procedure_program(Procedure this);
#endif//$XCC_h

struct Program_ *
Procedure_program(Procedure this)
{
  return Procedure_PROGRAM(this);
}

#ifdef $XCC_h
/**
 * This Procedure array of dedicated Temporary(ies).
 */
struct Temporary_ *
Procedure_dedicated(Procedure this);
#endif//$XCC_h

struct Temporary_ *
Procedure_dedicated(Procedure this)
{
  return Procedure_DEDICATED(this);
}

#ifdef $XCC_h
/**
 * This Procedure symbol.
 */
Symbol
Procedure_symbol(const_Procedure this);
#endif//$XCC_h

Symbol
Procedure_symbol(const_Procedure this)
{
  return Procedure_SYMBOL(this);
}

#ifdef $XCC_h
/**
 * This Procedure address.
 */
PlatformUInt
Procedure_address(const_Procedure this);
#endif//$XCC_h

PlatformUInt
Procedure_address(const_Procedure this)
{
  return Procedure_ADDRESS(this);
}

#ifdef $XCC_h
/**
 * Set this Procedure address.
 */
void
Procedure_setAddress(Procedure this, PlatformUInt address);
#endif//$XCC_h

void
Procedure_setAddress(Procedure this, PlatformUInt address)
{
  *Procedure__ADDRESS(this) = address;
}

#ifdef $XCC_h
/**
 * This Procedure align.
 */
uint8_t
Procedure_align(Procedure this);
#endif//$XCC_h

uint8_t
Procedure_align(Procedure this)
{
  return Procedure_ALIGN(this);
}

#ifdef $XCC_h
/**
 * This Procedure frame.
 */
Frame
Procedure_frame(Procedure this);
#endif//$XCC_h

Frame
Procedure_frame(Procedure this)
{
  return Procedure_FRAME(this);
}

#ifdef $XCC_h
/**
 * Has this procedure a fixed or variable frame size?
 */
bool
Procedure_hasFixedFrame(Procedure this);
#endif//$XCC_h

bool
Procedure_hasFixedFrame(Procedure this)
{
  return Frame_isFixed(Procedure_FRAME(this));
}



#ifdef $XCC_h
/**
 * Set this procedure tail / non-tail status.
 * This updates the "scratch" area of the frame.
 *
 * @param tail true if the procedure does not make calls, false otherwise.
 */
void
Procedure_setTail(Procedure this, bool tail);
#endif//$XCC_h

void
Procedure_setTail(Procedure this, bool tail)
{
  Frame frame = Procedure_frame(this);
  Frame_setScratch(frame, tail ? 0 : 16); //TODO: should to be hard-encoded
}





#ifdef $XCC_h
/**
 * Update the procedure frame size and modify code accordingly.
 * Call this function after the size of one of the area has been changed.
 */
void
Procedure_updateFrameSize(Procedure this);
#endif//$XCC_h

void
Procedure_updateFrameSize(Procedure this)
{
  Frame frame = Procedure_frame(this);
  ImmediateValue size = Frame_size(frame);
  Operation operation;
  Literal literal;
  Temporary temporary;
  Selector_ selector_;
  Selector selector = NULL;

  Procedure_FOREACH_BasicBlock(this, block) {
    if(BasicBlock_isEntry(block)) {
      // Adjust stack pointer at entry to allocate frame
      operation = BasicBlock_findStackAdjust(block, false);
      if(operation) {
        temporary = Operation_getArgument(operation, 1);
        Except_CHECK(Temporary_isAbsolute(temporary));
        literal = Temporary_LITERAL(temporary);
        Literal_setValue(literal, size);
      } else {
        // No ADJUST, need to create one at top of block
        if(!selector) {
          selector = &selector_;
          Selector_Ctor(selector,
              Procedure_memory(this),
              this, false);
        }
        Except_CHECK(Selector_isEmpty(selector));
        Selector_selectADJUST(selector, size);
        operation = Selector_firstOperation(selector);
        Operation_setFlags(operation, OperationFlag_EntryCode);
        BasicBlock_putSelector(block, selector);
      }
      if(!Frame_isFixed(frame)) {
        Program program = Procedure_program(this);
        Convention convention = Program_convention(program);
        operation = NULL;
        // We must look for the ADJUST that defines FP from SP
        BasicBlock_FOREACH_Operation(block, op) {
          if(!Operation_isEntryCode(op)) break;
          if(Operator_isAdjust(Operation_operator(op))) {
            Temporary arg = Operation_getArgument(op, 0);
            Temporary res = Operation_getResult(op, 0);
            Except_CHECK(Temporary_hasRegister(arg));
            Except_CHECK(Temporary_hasRegister(res));
            if(Convention_isStackRegister(convention, Temporary_register(arg)) &&
               Convention_isFrameRegister(convention, Temporary_register(res))) {
              operation = op;
              break;
            }
          }
        } BasicBlock_ENDEACH_Operation;
        Except_CHECK(operation != NULL);
        temporary = Operation_getArgument(operation, 1);
        Except_CHECK(Temporary_isAbsolute(temporary));
        literal = Temporary_LITERAL(temporary);
        Literal_setValue(literal, -size); // Add back the size to get the orig SP
      }
    }
    if(BasicBlock_isExit(block)) {
      // Adjust stack pointer at exit to allocate frame
      // No adjust at exit for frames with non-fixed size. (SP is stored in 
      // FP).
      operation = BasicBlock_findStackAdjust(block, true);
      if(operation) {
        Except_CHECK(Frame_isFixed(frame)); // There should be no adjust for fixed frames
        temporary = Operation_getArgument(operation, 1);
        Except_CHECK(Temporary_isAbsolute(temporary));
        literal = Temporary_LITERAL(temporary);
        Literal_setValue(literal, -size);
      } else if(Frame_isFixed(frame)) {
        // No ADJUST, need to create one near end of block
        if(!selector) {
          selector = &selector_;
          Selector_Ctor(selector,
              Procedure_memory(this),
              this, false);
        }
        Except_CHECK(Selector_isEmpty(selector));
        Selector_selectADJUST(selector, -size);
        operation = Selector_firstOperation(selector);
        Operation_setFlags(operation, OperationFlag_ExitCode);

        // append after the first non-return operation
        BasicBlock_FORBACK_Operation(block, operation) {
          if(!Operation_isReturn(operation)) { // Return operation is control
            BasicBlock_appendSelector(block, operation, selector);
            break;
          }
        } BasicBlock_ENDBACK_Operation;
      }
    }
  } Procedure_ENDEACH_BasicBlock;
  if(selector) {
    Selector_Dtor(selector);
  }
}

#ifdef $XCC_h
/**
 * This Procedure name.
 */
#define Procedure_name(this)	Symbol_name(Procedure_symbol(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by Procedure_FOREACH_Temporary.
 */
IBList
Procedure_temporaries(Procedure this);
#endif//$XCC_h

IBList
Procedure_temporaries(Procedure this)
{
  return Procedure_TEMPORARIES(this);
}

#ifdef $XCC_h
/**
 * Iterate this Procedure Temporary(ies).
 */
#define Procedure_FOREACH_Temporary(this, temporary) \
  IBList_FOREACH(Procedure_temporaries(this), Temporary_, temporary)
#define Procedure_ENDEACH_Temporary \
  IBList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Procedure Processor.
 */
Processor
Procedure_processor(const_Procedure this);
#endif//$XCC_h

Processor
Procedure_processor(const_Procedure this)
{
  return Procedure_PROCESSOR(this);
}

#ifdef $XCC_h
/**
 * This Procedure Convention.
 */
Convention
Procedure_convention(const_Procedure this);
#endif//$XCC_h

Convention
Procedure_convention(const_Procedure this)
{
  return Procedure_PROCESSOR(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make a new virtual register Temporary.
 */
Temporary
Procedure_makeVirtualTemporary(Procedure this, RegFile regFile);
#endif//$XCC_h

Temporary
Procedure_makeVirtualTemporary(Procedure this, RegFile regFile)
{
  IBList temporaries = Procedure_TEMPORARIES(this);
  Temporary temporary = Temporary_make(temporaries, TemporaryType_Virtual, TemporaryFlag_RenameOK);
  *Temporary__REGFILE(temporary) = regFile;
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a virtual register Temporary a copy of another.
 */
Temporary
Procedure_copyVirtualTemporary(Procedure this, const_Temporary register_temporary);
#endif//$XCC_h

Temporary
Procedure_copyVirtualTemporary(Procedure this, const_Temporary register_temporary)
{
  IBList temporaries = Procedure_TEMPORARIES(this);
  Temporary temporary = Temporary_make(temporaries, TemporaryType_Virtual, TemporaryFlag_RenameOK);
  Except_CHECK(Temporary_hasRegFile(register_temporary));
  *Temporary__WIDTH(temporary) = Temporary_WIDTH(register_temporary);
  *Temporary__REGFILE(temporary) = Temporary_REGFILE(register_temporary);
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a new assigned register Temporary.
 */
Temporary
Procedure_makeAssignedTemporary(Procedure this, Register registre);
#endif//$XCC_h

Temporary
Procedure_makeAssignedTemporary(Procedure this, Register registre)
{
  IBList temporaries = Procedure_TEMPORARIES(this);
  Temporary temporary = Temporary_make(temporaries, TemporaryType_Virtual, TemporaryFlag_Register);
  *Temporary__REGFILE(temporary) = Register_regFile(registre, Procedure_processor(this));
  *Temporary__REGISTER(temporary) = registre;
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a dedicated register Temporary.
 *
 * This function commonalizes the dedicated register Temporary(ies).
 */
Temporary
Procedure_makeDedicatedTemporary(Procedure this, Register registre);
#endif//$XCC_h

Temporary
Procedure_makeDedicatedTemporary(Procedure this, Register registre)
{
  Temporary dedicated = Procedure_DEDICATED(this);
  Temporary temporary = dedicated + registre;
  Except_CHECK(Temporary_isDedicated(temporary));
  Except_CHECK(Temporary_hasRegister(temporary));
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a new absolute Temporary.
 */
Temporary
Procedure_makeAbsoluteTemporary(Procedure this,
                                ImmediateValue value, Immediate immediate);
#endif//$XCC_h

Temporary
Procedure_makeAbsoluteTemporary(Procedure this,
                                ImmediateValue value, Immediate immediate)
{
  IBList literals = Procedure_LITERALS(this);
  Literal literal = Literal_Ctor(IBList_push(literals),
                                 LiteralType_Absolute, NULL, value, immediate);
  Temporary temporary = Temporary_Ctor((Temporary)(literal + 1),
                                       TemporaryType_Absolute, TemporaryFlag_Constant);
  *Temporary__LITERAL(temporary) = literal;
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a new symbol Temporary.
 */
Temporary
Procedure_makeSymbolTemporary(Procedure this, Symbol symbol,
                              ImmediateValue offset, Immediate immediate);
#endif//$XCC_h

Temporary
Procedure_makeSymbolTemporary(Procedure this, Symbol symbol,
                              ImmediateValue offset, Immediate immediate)
{
  IBList literals = Procedure_LITERALS(this);
  Literal literal = Literal_Ctor(IBList_push(literals),
                                 LiteralType_Symbol, symbol, offset, immediate);
  Temporary temporary = Temporary_Ctor((Temporary)(literal + 1),
                                       TemporaryType_Symbol, TemporaryFlag_Constant);
  *Temporary__LITERAL(temporary) = literal;
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a new label Temporary.
 *
 * @param label the label inside the temporary
 */
Temporary
Procedure_makeLabelTemporary(Procedure this, Label label, Immediate immediate);
#endif//$XCC_h

Temporary
Procedure_makeLabelTemporary(Procedure this, Label label, Immediate immediate)
{
  IBList literals = Procedure_LITERALS(this);
  Literal literal = Literal_Ctor(IBList_push(literals),
                                 LiteralType_Label, label, 0, immediate);
  Temporary temporary = Temporary_Ctor((Temporary)(literal + 1),
                                       TemporaryType_Label, TemporaryFlag_Constant);
  *Temporary__LITERAL(temporary) = literal;
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a new Modifier Temporary.
 */
Temporary
Procedure_makeModifierTemporary(Procedure this, ModifierMember member);
#endif//$XCC_h

Temporary
Procedure_makeModifierTemporary(Procedure this, ModifierMember member)
{
  IBList temporaries = Procedure_TEMPORARIES(this);
  Temporary temporary = Temporary_make(temporaries, TemporaryType_Modifier, TemporaryFlag_Constant);
  *Temporary__MEMBER(temporary) = member;
  return temporary;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Get the rematerialization value of a Temporary.
 */
Temporary
Procedure_getRemater(const_Procedure this, Temporary temporary);
#endif//$XCC_h

Temporary
Procedure_getRemater(const_Procedure this, Temporary temporary)
{
  const_HTable rematerMap = Procedure_REMATERMAP(this);
  Except_REQUIRE(Temporary_isRemater(temporary));
  return *(Temporary *)HTable_search(rematerMap, (HTableKey)temporary);
}

#ifdef $XCC_h
/**
 * Associate a Temporary to its rematerialization value.
 */
void
Procedure_setRemater(Procedure this, Temporary temporary, Temporary value);
#endif//$XCC_h

void
Procedure_setRemater(Procedure this, Temporary temporary, Temporary value)
{
  HTable rematerMap = Procedure_REMATERMAP(this);
  Except_REQUIRE(Temporary_hasLiteral(value) || Temporary_isConstant(value));
  *(Temporary *)HTable_lookup_(rematerMap,
                               (HTableKey)temporary,
                               sizeof(Temporary)) = value;
  Temporary_setFlags(temporary, TemporaryFlag_Remater);
}

#ifdef $XCC_h
/**
 * Get the home location of a Temporary.
 */
Temporary
Procedure_getHomeable(const_Procedure this, Temporary temporary);
#endif//$XCC_h

Temporary
Procedure_getHomeable(const_Procedure this, Temporary temporary)
{
  const_HTable homeableMap = Procedure_HOMEABLEMAP(this);
  Except_REQUIRE(Temporary_isHomeable(temporary));
  return *(Temporary *)HTable_search(homeableMap, (HTableKey)temporary);
}

#ifdef $XCC_h
/**
 * Associate a Temporary to its home location.
 */
void
Procedure_setHomeable(Procedure this, Temporary temporary, Temporary location);
#endif//$XCC_h

void
Procedure_setHomeable(Procedure this, Temporary temporary, Temporary location)
{
  HTable homeableMap = Procedure_HOMEABLEMAP(this);
  Except_REQUIRE(Temporary_isSymbol(location));
  *(Temporary *)HTable_lookup_(homeableMap,
                               (HTableKey)temporary,
                               sizeof(Temporary)) = location;
  Temporary_setFlags(temporary, TemporaryFlag_Homeable);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Set this procedure section.
 */
void
Procedure_setSection(Procedure this, DataSection section);
#endif//$XCC_h

void
Procedure_setSection(Procedure this, DataSection section)
{
  Procedure_SECTION(this) = section;
}

#ifdef $XCC_h
/**
 * This Procedure get Section
 */
DataSection
Procedure_getSection(const_Procedure this);
#endif//$XCC_h

DataSection
Procedure_getSection(const_Procedure this)
{
  return Procedure_SECTION(this);
}

#ifdef $XCC_h
/**
 * Set this procedure alignment.
 */
void
Procedure_setAlign(Procedure this, uint8_t align);
#endif//$XCC_h

void
Procedure_setAlign(Procedure this, uint8_t align)
{
  Procedure_ALIGN(this) = align;
}

#ifdef $XCC_h
/**
 * Get this procedure alignment.
 */
uint8_t
Procedure_getAlign(const_Procedure this);
#endif//$XCC_h

uint8_t
Procedure_getAlign(const_Procedure this)
{
  return Procedure_ALIGN(this);
}



////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * For use by Procedure_FOREACH_BasicBlock.
 */
IDList
Procedure_blockChain(Procedure this);
#endif//$XCC_h

IDList
Procedure_blockChain(Procedure this)
{
  return Procedure_BLOCKCHAIN(this);
}

#ifdef $XCC_h
/**
 * Iterate this Procedure BasicBlocks.
 */
#define Procedure_FOREACH_BasicBlock(this, basicBlock) \
  IDList_FOREACH(Procedure_blockChain(this), BasicBlock_, basicBlock)
#define Procedure_FOREACH_BasicBlock_SYNC IDList_FOREACH_SYNC
#define Procedure_ENDEACH_BasicBlock \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this Procedure BasicBlocks.
 */
#define Procedure_FORBACK_BasicBlock(this, basicBlock) \
  IDList_FORBACK(Procedure_blockChain(this), BasicBlock_, basicBlock)
#define Procedure_ENDBACK_BasicBlock \
  IDList_ENDBACK;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of this Procedure BasicBlock(s).
 */
int32_t
Procedure_blockCount(Procedure this);
#endif//$XCC_h

int32_t
Procedure_blockCount(Procedure this)
{
  return Procedure_BLOCKCOUNT(this);
}

#ifdef $XCC_h
/**
 * For use by Procedure_DISCARDED_FOREACH_BasicBlock.
 */
IDList
Procedure_discarded(Procedure this);
#endif//$XCC_h

IDList
Procedure_discarded(Procedure this)
{
  return Procedure_DISCARDED(this);
}

#ifdef $XCC_h
/**
 * Iterate this Procedure discarded BasicBlocks.
 */
#define Procedure_DISCARDED_FOREACH_BasicBlock(this, basicBlock) \
  IDList_FOREACH(Procedure_discarded(this), BasicBlock_, basicBlock)
#define Procedure_DISCARDED_ENDEACH_BasicBlock \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a new BasicBlock for this Procedure.
 *
 * @param this the procedure to which the BasicBlock will belong.
 * @param regionId the identifier for the region.
 * @param frequency the execution frequency of this BasicBlock.
 *
 * @warning This is the BasicBlock creation interface to use before
 * Procedure_buildCodeRegions(). After Procedure_buildCodeRegions(), one should
 * call CodeRegion_makeBasicBlock().
 *
 * @sa CodeRegion_makeBasicBlock.
 */
BasicBlock
Procedure_makeBasicBlock(Procedure this, intptr_t regionId, float frequency);
#endif//$XCC_h

BasicBlock
Procedure_makeBasicBlock(Procedure this, intptr_t regionId, float frequency)
{
  Memory memory = Procedure_MEMORY(this);
  IDList codeRegions = Procedure_CODEREGIONS(this);
  IDList blockChain = Procedure_BLOCKCHAIN(this);
  CodeRegion codeRegion = Procedure_findCodeRegion(this, regionId);
  size_t size = BasicBlock_Size(memory, this, codeRegion, frequency);
  BasicBlock basicBlock = IDList_push(blockChain, size);
  Except_CHECK(!Procedure_isBuilt(this));
  if (codeRegion == NULL) {
    Program program = Procedure_PROGRAM(this);
    Optimize optimize = Program_OPTIMIZE(program);
    Configure configure = Program_CONFIGURE(program);
    size_t size = CodeRegion_Size(memory, this, regionId, optimize, configure);
    codeRegion = CodeRegion_Ctor(IDList_push(codeRegions, size),
                                 memory, this, regionId, optimize, configure);
  }
  Except_CHECK(CodeRegion_isEmpty(codeRegion));
  BasicBlock_Ctor(basicBlock, memory, this, codeRegion, frequency);
  return basicBlock;
}

#ifdef $XCC_h
/**
 * Chain succ_block to pred_block in the BasicBlock chain.
 */
void
Procedure_chainBasicBlocks(Procedure this, BasicBlock pred_block, BasicBlock succ_block);
#endif//$XCC_h

void
Procedure_chainBasicBlocks(Procedure this, BasicBlock pred_block, BasicBlock succ_block)
{
  IDList discarded = Procedure_DISCARDED(this);
  IDList blockChain = Procedure_BLOCKCHAIN(this);
  Except_REQUIRE(pred_block != succ_block);
  Except_REQUIRE(BasicBlock_INDEX(pred_block) != 0);
  Except_REQUIRE(BasicBlock_INDEX(succ_block) != 0);
  Except_CHECK(IDList_contains(blockChain, pred_block));
  if (IDList_contains(discarded, succ_block)) {
    IDList_remove_(discarded, succ_block);
  } else {
    Except_CHECK(IDList_contains(blockChain, succ_block));
    IDList_remove_(blockChain, succ_block);
  }
  IDList_append_(blockChain, pred_block, succ_block);
  BasicBlock_setTraceId(succ_block, BasicBlock_traceId(pred_block));
}

#ifdef $XCC_h
/**
 * Push a BasicBlock on this Procedure_blockChain.
 */
void
Procedure_pushBasicBlock(Procedure this, BasicBlock basicBlock);
#endif//$XCC_h

void
Procedure_pushBasicBlock(Procedure this, BasicBlock basicBlock)
{
  IDList discarded = Procedure_DISCARDED(this);
  IDList blockChain = Procedure_BLOCKCHAIN(this);
  Except_REQUIRE(BasicBlock_INDEX(basicBlock) != 0);
  Except_CHECK(IDList_contains(discarded, basicBlock));
  IDList_remove_(discarded, basicBlock);
  IDList_push_(blockChain, basicBlock);
}

#ifdef $XCC_h
/**
 * Detach basicBlock from this Procedure_blockChain.
 */
void
Procedure_unchainBasicBlock(Procedure this, BasicBlock basicBlock);
#endif//$XCC_h

void
Procedure_unchainBasicBlock(Procedure this, BasicBlock basicBlock)
{
  IDList discarded = Procedure_DISCARDED(this);
  IDList blockChain = Procedure_BLOCKCHAIN(this);
  Except_REQUIRE(BasicBlock_INDEX(basicBlock) != 0);
  Except_CHECK(IDList_contains(blockChain, basicBlock));
  IDList_remove_(blockChain, basicBlock);
  IDList_push_(discarded, basicBlock);
  BasicBlock_setTraceId(basicBlock, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * For use by Procedure_FOREACH_CodeRegion.
 */
IDList
Procedure_codeRegions(Procedure this);
#endif//$XCC_h

IDList
Procedure_codeRegions(Procedure this)
{
  return Procedure_CODEREGIONS(this);
}

#ifdef $XCC_h
/**
 * Iterate this Procedure CodeRegion(s).
 */
#define Procedure_FOREACH_CodeRegion(this, codeRegion) { \
  IDList_FOREACH(Procedure_codeRegions(this), CodeRegion_, codeRegion)
#define Procedure_ENDEACH_CodeRegion \
  IDList_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this Procedure CodeRegion(s).
 */
#define Procedure_FORBACK_CodeRegion(this, codeRegion) { \
  IDList_FORBACK(Procedure_codeRegions(this), CodeRegion_, codeRegion)
#define Procedure_ENDBACK_CodeRegion \
  IDList_ENDBACK \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The first CodeRegion of this Procedure.
 */
CodeRegion
Procedure_firstRegion(Procedure this);
#endif//$XCC_h

CodeRegion
Procedure_firstRegion(Procedure this)
{
  return Procedure_FIRSTREGION(this);
}

#ifdef $XCC_h
/**
 * The last CodeRegion of this Procedure.
 */
CodeRegion
Procedure_lastRegion(Procedure this);
#endif//$XCC_h

CodeRegion
Procedure_lastRegion(Procedure this)
{
  return Procedure_LASTREGION(this);
}

#ifdef $XCC_h
/**
 * Find a CodeRegion from a region identifier.
 */
CodeRegion
Procedure_findCodeRegion(Procedure this, intptr_t regionId);
#endif//$XCC_h

CodeRegion
Procedure_findCodeRegion(Procedure this, intptr_t regionId)
{
  Procedure_FORBACK_CodeRegion(this, codeRegion) {
    if (CodeRegion_REGIONID(codeRegion) == regionId) return codeRegion;
  } Procedure_ENDBACK_CodeRegion;
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Increment this Procedure execution counter
 */
void
Procedure_increaseCounter(Procedure this);
#endif//$XCC_h

void
Procedure_increaseCounter(Procedure this)
{
  ++(*Procedure__COUNTER(this));
}

#ifdef $XCC_h
/**
 * get this Procedure execution counter
 */
uint32_t
Procedure_getCounter(Procedure this);
#endif//$XCC_h
uint32_t
Procedure_getCounter(Procedure this)
{
  return *Procedure__COUNTER(this);
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * For use by Procedure_FOREACH_LoopScope.
 */
HTable
Procedure_loopScopes(Procedure this);
#endif//$XCC_h

HTable
Procedure_loopScopes(Procedure this)
{
  return Procedure_LOOPSCOPES(this);
}

#ifdef $XCC_h
/**
 * This Procedure DataList.
 */
DataList
Procedure_dataList(Procedure this);
#endif//$XCC_h

DataList
Procedure_dataList(Procedure this)
{
  return Procedure_DATALIST(this);
}

#ifdef $XCC_h
/**
 * This Procedure symbol for SP block.
 */
Symbol
Procedure_symbolSP(Procedure this);
#endif//$XCC_h

Symbol
Procedure_symbolSP(Procedure this)
{
  return Procedure_SYMBOLSP(this);
}

#ifdef $XCC_h
/**
 * This Procedure symbol for FP block.
 */
Symbol
Procedure_symbolFP(Procedure this);
#endif//$XCC_h

Symbol
Procedure_symbolFP(Procedure this)
{
  return Procedure_SYMBOLFP(this);
}



#ifdef $XCC_h
/**
 * Iterate this Procedure LoopScope(s).
 */
#define Procedure_FOREACH_LoopScope(this, loopScope) { \
  HTable Procedure_LOOPSCOPES = Procedure_loopScopes(this); \
  HTable_FOREACH(Procedure_LOOPSCOPES, Procedure_KEY, LoopScope_, loopScope) {
#define Procedure_ENDEACH_LoopScope \
  } HTable_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Find a LoopScope from a loop head BasicBlock.
 */
LoopScope
Procedure_findLoopScope(Procedure this, BasicBlock headerBlock);
#endif//$XCC_h

LoopScope
Procedure_findLoopScope(Procedure this, BasicBlock headerBlock)
{
  HTableKey key = (void *)headerBlock;
  HTable loopScopes = Procedure_LOOPSCOPES(this);
  return HTable_search(loopScopes, key);
}

#ifdef $XCC_h
/**
 * Make a LoopScope for a header BasicBlock.
 */
LoopScope
Procedure_makeLoopScope(Procedure this, BasicBlock headerBlock, Configure configure);
#endif//$XCC_h

LoopScope
Procedure_makeLoopScope(Procedure this, BasicBlock headerBlock, Configure configure)
{
  HTableKey key = (void *)headerBlock;
  Memory memory = Procedure_MEMORY(this);
  HTable loopScopes = Procedure_LOOPSCOPES(this);
  int32_t count = HTable_count(loopScopes);
  size_t size = LoopScope_Size(memory, headerBlock, configure);
  LoopScope loopScope = HTable_lookup_(loopScopes, key, size);
  if (HTable_count(loopScopes) != count) {
    LoopScope_Ctor(loopScope, memory, headerBlock, configure);
  }
  return loopScope;
}

#ifdef $XCC_h
/**
 * Remove a LoopScope.
 */
bool
Procedure_removeLoopScope(Procedure this, BasicBlock headerBlock);
#endif//$XCC_h

bool
Procedure_removeLoopScope(Procedure this, BasicBlock headerBlock)
{
  HTable loopScopes = Procedure_LOOPSCOPES(this);
#ifndef _NDTOR
  return HTable_remove(loopScopes, headerBlock, (HTableValueRelease)LoopScope_Dtor);
#else//!_NDTOR
  return HTable_remove_(loopScopes, headerBlock) != NULL;
#endif//_NDTOR
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Set a BasicBlock as a Procedure entry block.
 */
void
Procedure_setEntryBlock(Procedure this, BasicBlock basicBlock);
#endif//$XCC_h

void
Procedure_setEntryBlock(Procedure this, BasicBlock basicBlock)
{
  //Except_CHECK(IDList_isEmpty(Procedure_CODEREGIONS(this)));
  BasicBlock_setFlags(basicBlock, BasicBlockFlag_Entry);
}

#ifdef $XCC_h
/**
 * Set a BasicBlock as a Procedure exit block.
 */
void
Procedure_setExitBlock(Procedure this, BasicBlock basicBlock);
#endif//$XCC_h

void
Procedure_setExitBlock(Procedure this, BasicBlock basicBlock)
{
  //Except_CHECK(IDList_isEmpty(Procedure_CODEREGIONS(this)));
  BasicBlock_setFlags(basicBlock,  BasicBlockFlag_Exit);
}

#ifdef $XCC_h
/**
 * Set a BasicBlock as trace start block.
 */
void
Procedure_setStartBlock(Procedure this, BasicBlock basicBlock);
#endif//$XCC_h

void
Procedure_setStartBlock(Procedure this, BasicBlock basicBlock)
{
  BasicBlock_setFlags(basicBlock, BasicBlockFlag_Start);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Fixup this Procedure when built from Open64.
 *
 * The Open64 does not have the extra arguments for predicated operations.
 * We add these last arguments by copying the Register and RegClass results.
 *
 * The Open64 does not link the function call/return parameters to the actual
 * routine call or return or tail call operation. We restore these links by
 * making these parameters explicit operation arguments or results.
 *
 */
void
Procedure_fixupOpen64(Procedure this, OptimizeFlags activate);
#endif//$XCC_h

void
Procedure_fixupOpen64(Procedure this, OptimizeFlags activate)
{
  Program program = Procedure_PROGRAM(this);
  Convention convention = Program_CONVENTION(program);
  // Fill the missing routine call arguments and results.
  const_RegisterSet reservedSet = Convention_roleRegisterSet(convention, ConventionRole_Reserved);
  const_RegisterSet argumentSet = Convention_roleRegisterSet(convention, ConventionRole_Argument);
  const_RegisterSet resultSet = Convention_roleRegisterSet(convention, ConventionRole_Result);
  const_RegisterSet structSet = Convention_roleRegisterSet(convention, ConventionRole_Struct);
  const_RegisterSet calleeSet = Convention_roleRegisterSet(convention, ConventionRole_Callee);
  const_RegisterSet returnSet = Convention_roleRegisterSet(convention, ConventionRole_Return);
  const_RegisterSet staticSet = Convention_roleRegisterSet(convention, ConventionRole_Static);
  const_RegisterSet wiredSet = Convention_roleRegisterSet(convention, ConventionRole_Wired);
  RegisterSet_ filterArguments[1], filterResults[1], definedSet[1];
  RegisterSet_Copy(filterArguments, argumentSet);
  RegisterSet_union(filterArguments, structSet);
  RegisterSet_union(filterArguments, staticSet);
  RegisterSet_Copy(filterResults, resultSet);
  RegisterSet_Copy(definedSet, reservedSet);
  RegisterSet_diff(definedSet, wiredSet);
  Except_REQUIRE(convention != Convention__UNDEF);
  Procedure_FOREACH_BasicBlock(this, basicBlock) {
    Operation last_operation = BasicBlock_lastOperation(basicBlock);
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_OPERATOR(operation);
      int32_t argCount = Operation_argCount(operation);
      int32_t resCount = Operation_resCount(operation);
      if (Operator_isPredicated(operator)) {
        int32_t readCount = Operator_readCount(operator);
        int32_t writeCount = Operator_writeCount(operator), index;
        if (readCount == argCount + resCount && writeCount == resCount) {
          Operation_resizeOperands(operation, writeCount, readCount);
          for (index = 0; index < resCount; index++) {
            Operation_setArgument(operation, argCount + index, Operation_getResult(operation, index));
          }
        } else Except_NEVER(true);
      }
    } BasicBlock_ENDEACH_Operation;
    if (last_operation != NULL) {
      Operator last_operator = Operation_OPERATOR(last_operation);
      int32_t argCount = Operation_argCount(last_operation);
      int32_t resCount = Operation_resCount(last_operation);
      if (Operator_isRoutine(last_operator)) {
        Operation routine_operation = last_operation;
        int32_t routineArgCount = 0, routineResCount = 0;
        BasicBlock next_block = BasicBlock_next(basicBlock);
        RegisterSet_ routineArguments[1], routineResults[1];
        RegisterSet_Ctor(routineArguments);
        RegisterSet_Ctor(routineResults);
        if (activate & (OptimizeActivation_PrePass | OptimizeActivation_RegAlloc)) {
          // Collect the dedicated results of other Operation(s) in block.
          BasicBlock_FOREACH_Operation(basicBlock, operation) {
            if (operation != last_operation) {
              Operation_getDedicatedResults(operation, routineArguments);
            }
          } BasicBlock_ENDEACH_Operation;
 	  // FdF 20090608: The fall-through successor of the routine
 	  // may be empty or may just contain an unconditional branch.
 	  // In these cases the uses of dedicated must be searched in
 	  // the next successor.
 	  if ((next_block != NULL)
 	      && !BasicBlock_isExit(next_block)
 	      && BasicBlock_leaveCount(next_block) == 1
 	      && (BasicBlock_firstOperation(next_block) == NULL
 		  || Operator_isGoTo(Operation_operator(BasicBlock_firstOperation(next_block))))) {
 	    next_block = BasicBlockEdge_destBlock(BasicBlock_leaveFirst(next_block));
 	  }
 	  if (next_block != NULL) {
            // Collect the dedicated arguments of Operation(s) in next block.
            RegisterSet_ localArg[1];
            RegisterSet_ localRes[1];
            RegisterSet_Ctor(localArg);
            RegisterSet_Ctor(localRes);
            BasicBlock_FOREACH_Operation(next_block, operation) {
              Operation_getDedicatedArguments(operation, localArg);
              RegisterSet_diff(localArg, localRes);
              RegisterSet_union(routineResults,  localArg);
              Operation_getDedicatedResults(operation, localRes);
            } BasicBlock_ENDEACH_Operation;
          }
          RegisterSet_inter(routineArguments, filterArguments);
          RegisterSet_inter(routineResults, filterResults);
        } else
        if (activate & OptimizeActivation_PostPass) {
          RegisterSet_union(routineArguments, filterArguments);
          RegisterSet_union(routineResults, filterResults);
        }
        RegisterSet_union(routineArguments, definedSet);
        routineArgCount = RegisterSet_count(routineArguments);
        routineResCount = RegisterSet_count(routineResults);
        if (routineArgCount > 0 || routineResCount > 0) {
          Operation_resizeOperands(routine_operation,
                                   resCount + routineResCount,
                                   argCount + routineArgCount);
          RegisterSet_FOREACH_Register(routineArguments, registre) {
            Temporary temporary = Procedure_makeDedicatedTemporary(this, registre);
            Operation_appendArgument(routine_operation, temporary);
          } RegisterSet_ENDEACH_Register;
          RegisterSet_FOREACH_Register(routineResults, registre) {
            Temporary temporary = Procedure_makeDedicatedTemporary(this, registre);
            Operation_appendResult(routine_operation, temporary);
          } RegisterSet_ENDEACH_Register;
        }
        RegisterSet_Dtor(routineArguments);
        RegisterSet_Dtor(routineResults);
      } else
      if (Operator_isReturn(last_operator)) {
        Operation return_operation = last_operation;
        int32_t returnArgCount = 0;
        RegisterSet_ returnArguments[1];
        RegisterSet_Ctor(returnArguments);
        if (activate & (OptimizeActivation_PrePass | OptimizeActivation_RegAlloc)) {
          BasicBlock_FOREACH_Operation(basicBlock, operation) {
            Operation_getDedicatedResults(operation, returnArguments);
          } BasicBlock_ENDEACH_Operation;
          RegisterSet_inter(returnArguments, filterResults);
        } else
        if (activate & OptimizeActivation_PostPass) {
          RegisterSet_union(returnArguments, filterResults);
        }
        RegisterSet_union(returnArguments, definedSet);
        RegisterSet_union(returnArguments, calleeSet);
        returnArgCount = RegisterSet_count(returnArguments);
        if (returnArgCount > 0) {
          Operation_resizeOperands(return_operation, resCount, argCount + returnArgCount);
          RegisterSet_FOREACH_Register(returnArguments, registre) {
            Temporary temporary = Procedure_makeDedicatedTemporary(this, registre);
            Operation_appendArgument(return_operation, temporary);
          } RegisterSet_ENDEACH_Register;
        }
      } else
      if (Operator_isBranch(last_operator) &&
          BasicBlock_isExit(basicBlock)) {
        // Non-local branch, assume it results from tail-call optimization.
        // In case of tail-call, the function arguments are prepared and the
        // callee-saved registers are restored before the branch.
        Operation branch_operation = last_operation;
        int32_t branchArgCount = 0;
        RegisterSet_ branchArguments[1];
        RegisterSet_ filterOperands[1];
        RegisterSet_Ctor(branchArguments);
        RegisterSet_Copy(filterOperands, returnSet);
        RegisterSet_union(filterOperands, filterArguments);
        RegisterSet_union(filterOperands, filterResults);
        if (activate & (OptimizeActivation_PrePass | OptimizeActivation_RegAlloc)) {
          BasicBlock_FOREACH_Operation(basicBlock, operation) {
            Operation_getDedicatedResults(operation, branchArguments);
          } BasicBlock_ENDEACH_Operation;
          RegisterSet_inter(branchArguments, filterOperands);
        } else
        if (activate & OptimizeActivation_PostPass) {
          RegisterSet_union(branchArguments, filterOperands);
        }
        RegisterSet_union(branchArguments, calleeSet);
        branchArgCount = RegisterSet_count(branchArguments);
        if (branchArgCount > 0) {
          Operation_resizeOperands(branch_operation, resCount, argCount + branchArgCount);
          RegisterSet_FOREACH_Register(branchArguments, registre) {
            Temporary temporary = Procedure_makeDedicatedTemporary(this, registre);
            Operation_appendArgument(branch_operation, temporary);
          } RegisterSet_ENDEACH_Register;
        }
        RegisterSet_Dtor(filterOperands);
      }
    }
    // We cannot set this flag, because since entry and exit code are
    // linear, it add a constraint on graph coloring that cannot be satisfied
    // when we reduce the number of available colors. (See insertion of spill
    // operation in ECL/CGO/Spill.xcc)
    // With parallel copy (and with suppression of duplication in that copy),
    // this should be ok.
    if (activate & (OptimizeActivation_PrePass/* | OptimizeActivation_RegAlloc*/)) {
      if (BasicBlock_isEntry(basicBlock)) {
        BasicBlock_flagEntryCode(basicBlock, convention);
      }
      if (BasicBlock_isExit(basicBlock)) {
        BasicBlock_flagExitCode(basicBlock, convention);
      }
    }
  } Procedure_ENDEACH_BasicBlock;
  RegisterSet_Dtor(filterArguments);
  RegisterSet_Dtor(filterResults);
}

#ifdef $XCC_h
/**
 * Fixup this Procedure to enable renaming
 * of dedicated registers
 *
 */
void
Procedure_fixupDedicated(Procedure this, OptimizeFlags activate);
#endif//$XCC_h

/*
 * Temporary_dedicatedButRenameOK --  Returns true if it can be renamed.
 * Any temporary can be renamed except the wired registers and the
 * temporaries that must be allocated between actual operations (exceptions)
 * to well defined register. Hence we exclude:
 * - reserved registers : we don't rename them as they have an unknown meaning,
 * - stack, local thread pointer, program counter : they must well defined in
 * case of exception,
 * - wired : we preserved wired use/def as they hold a defined value.
 */
static bool
Temporary_dedicatedButRenameOK(Procedure this, Temporary temporary)
{
  Register reg;
  Convention convention = Program_CONVENTION(Procedure_PROGRAM(this));

  bool assigned =  Temporary_isDedicated(temporary)
                || Temporary_isAssigned(temporary);
  if (!assigned) return false;
  reg = Temporary_register(temporary);
  return    !RegisterSet_contains(Convention_reservedRegisterSet(convention), reg)
         && !RegisterSet_contains(Convention_programRegisterSet(convention), reg)
         && !RegisterSet_contains(Convention_stackRegisterSet(convention), reg)
         && !RegisterSet_contains(Convention_localRegisterSet(convention), reg)
         && !RegisterSet_contains(Convention_wiredRegisterSet(convention), reg);
}

/**
 * Return true if the operation must
 * hold some register constraint.
 */
static bool
Operation_mustHaveRegisters(Operation this)
{
  Operator operator = Operation_operator(this);
  return Operator_isControl(operator) || Operator_isEntry(operator);
}

static void
Procedure_fixupDedicatedPostOrder(Procedure this, BasicBlock basicBlock, HTable dedicatedMap, RegisterSet dedicatedRefs)
{
  // Do a post order traversal of the CFG and rename in the the backward
  // walk. The renaming map is simple because dedicated liveranges span only
  // linear regions. This algorithm will not work for instance in:
  // r <-
  // if ... goto B
  // A: ... <- r; exit
  // B: ... <- r; exit
  // Though it works for the case of local live range, or the case of
  // a live range spanning two consecutive BB with no side exit.
  if (BasicBlock_MARK(basicBlock) != 0)
    return;
  // Remove back edges
  *BasicBlock__MARK(basicBlock) = 1;

  BasicBlock_LEAVE_FOREACH_BasicBlockEdge(basicBlock, edge) {
    BasicBlock succ_block = BasicBlockEdge_destBlock(edge);
    Procedure_fixupDedicatedPostOrder(this, succ_block, dedicatedMap, dedicatedRefs);
  } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  //
  0 && LIR_TRACE && fprintf(LIR_TRACE, "%s: BB:%d\n", __FUNCTION__, BasicBlock_INDEX(basicBlock));
  //
  BasicBlock_FORBACK_Operation(basicBlock, operation) {
    if (Operation_hasClobber(operation)) {
      RegisterSet clobberSet = Operation_clobberSet(operation);
      RegisterSet_ additionalRegisters[1];
      RegisterSet_Copy(additionalRegisters, clobberSet);
      if (Operation_mustHaveRegisters(operation)) {
        Operation_RESULTS_FOREACH_Temporary(operation, result) {
          if (Temporary_hasRegister(result)) {
            if (Temporary_dedicatedButRenameOK(this, result)) {
              RegisterSet_remove(additionalRegisters, Temporary_REGISTER(result));
            }
          }
        } Operation_RESULTS_ENDEACH_Temporary;
      }
      RegisterSet_inter(additionalRegisters, dedicatedRefs);
      Operation_resizeOperands(operation,
                               Operation_resCount(operation) + RegisterSet_count(additionalRegisters),
                               Operation_argCount(operation));
      RegisterSet_FOREACH_Register(additionalRegisters, registre) {
        Temporary temporary = Procedure_makeDedicatedTemporary(this, registre);
        Operation_appendResult(operation, temporary);
      } RegisterSet_ENDEACH_Register;
    }
    Operation_RESULTS_FOREACH_Temporary(operation, result) {
      if (Temporary_hasRegister(result)) {
        // Lazily store dedicated and assigned constraints for the operation.
        if (Operation_mustHaveRegisters(operation) && !Operation_hasRegisters(operation))
          Operation_makeRegisters(operation);
        if (Temporary_dedicatedButRenameOK(this, result)) {
          Temporary new_result, *new_result_ptr =
            (Temporary *)HTable_search(dedicatedMap, (HTableKey)(long int)Temporary_REGISTER(result));
          if (new_result_ptr == NULL) {
            // A dead dedicated definition
            new_result =
              Procedure_makeVirtualTemporary(this, Temporary_regFile(result));
            0 && LIR_TRACE && fprintf(LIR_TRACE, "%s: Dead definition:", __FUNCTION__) &&
              Temporary_pretty(result, LIR_TRACE) && fprintf(LIR_TRACE, "\n");
          } else {
            new_result = *new_result_ptr;
          }
          Operation_setResult(operation, Operation_RESULTS_INDEX, new_result);
          0 && LIR_TRACE && fprintf(LIR_TRACE, "%s: Rename result:", __FUNCTION__) &&
            Temporary_pretty(result, LIR_TRACE) && fprintf(LIR_TRACE, " into: ") &&
            Temporary_pretty(new_result, LIR_TRACE) && fprintf(LIR_TRACE, "\n");
          HTable_remove(dedicatedMap, (HTableKey)(long int)Temporary_REGISTER(result), NULL);
        }
      }
    } Operation_RESULTS_ENDEACH_Temporary;
    Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
      if (Temporary_hasRegister(argument)) {
        // Lazily store dedicated and assigned constraints for the operation.
        if (Operation_mustHaveRegisters(operation) && !Operation_hasRegisters(operation))
          Operation_makeRegisters(operation);
        if (Temporary_dedicatedButRenameOK(this, argument)) {
          Temporary new_argument, *new_argument_ptr =
            (Temporary *)HTable_search(dedicatedMap, (HTableKey)(long int)Temporary_REGISTER(argument));
          if (new_argument_ptr == NULL) {
            // An exposed use
            new_argument =
              Procedure_makeVirtualTemporary(this, Temporary_regFile(argument));
            *(Temporary *)HTable_lookup_(dedicatedMap, (HTableKey)(long int)Temporary_REGISTER(argument),
                                         sizeof(Temporary)) = new_argument;
            0 && LIR_TRACE && fprintf(LIR_TRACE, "%s: Exposed use:", __FUNCTION__) &&
              Temporary_pretty(argument, LIR_TRACE) && fprintf(LIR_TRACE, "\n");
          } else {
            new_argument = *new_argument_ptr;
          }
          Operation_setArgument(operation, Operation_ARGUMENTS_INDEX, new_argument);
          0 && LIR_TRACE && fprintf(LIR_TRACE, "%s: Rename argument:", __FUNCTION__) &&
            Temporary_pretty(argument, LIR_TRACE) && fprintf(LIR_TRACE, " into: ") &&
            Temporary_pretty(new_argument, LIR_TRACE) && fprintf(LIR_TRACE, "\n");
        }
      }
    } Operation_ARGUMENTS_ENDEACH_Temporary;
  } BasicBlock_ENDBACK_Operation;
  if (BasicBlock_isEntry(basicBlock)) {
    // Insert entry operator for all input dedicated arguments
    Selector selector = BasicBlock_SELECTOR(basicBlock);
    Operation entryOperation = Selector_makeOperationENTRY(selector, HTable_count(dedicatedMap));
    Operation firstOperation = BasicBlock_firstOperation(basicBlock);
    if (firstOperation != entryOperation) {
      Selector_moveOperationBefore(selector, firstOperation, entryOperation);
    }
    HTable_FOREACH(dedicatedMap, dedicated, void, _) {
      Register registre = (intptr_t)dedicated;
      OperationENTRY_appendResult(entryOperation, Procedure_makeDedicatedTemporary(this, registre));
    } HTable_ENDEACH;
    Operation_makeRegisters(entryOperation);
    Operation_RESULTS_FOREACH_Temporary(entryOperation, result) {
      Temporary new_result =
        *(Temporary *)HTable_search(dedicatedMap, (HTableKey)(long int)Temporary_REGISTER(result));
        Operation_setResult(entryOperation, Operation_RESULTS_INDEX, new_result);
        0 && LIR_TRACE && fprintf(LIR_TRACE, "%s: Rename result:", __FUNCTION__) &&
          Temporary_pretty(result, LIR_TRACE) && fprintf(LIR_TRACE, " into: ") &&
          Temporary_pretty(new_result, LIR_TRACE) && fprintf(LIR_TRACE, "\n");
    } Operation_RESULTS_ENDEACH_Temporary;
  }
}

void
Procedure_fixupDedicated(Procedure this, OptimizeFlags activate)
{
  int index = 0;
  HTable_ dedicatedMap[1];
  RegisterSet_ dedicatedRefs[1];
  HTable_Ctor(dedicatedMap, Procedure_MEMORY(this), 16);
  RegisterSet_Ctor(dedicatedRefs);
  Procedure_FOREACH_BasicBlock(this, basicBlock) {
    *BasicBlock__MARK(basicBlock) = 0;
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operation_OPERANDS_FOREACH_Temporary(operation, operand) {
        if (Temporary_hasRegister(operand)) {
          if (Temporary_dedicatedButRenameOK(this, operand)) {
            RegisterSet_insert(dedicatedRefs, Temporary_REGISTER(operand));
          }
        }
      } Operation_OPERANDS_ENDEACH_Temporary;
    } BasicBlock_ENDEACH_Operation;
  } Procedure_ENDEACH_BasicBlock;
  Procedure_FOREACH_BasicBlock(this, basicBlock) {
    if (BasicBlock_isEntry(basicBlock)) {
      Procedure_fixupDedicatedPostOrder(this, basicBlock, dedicatedMap, dedicatedRefs);
      HTable_empty(dedicatedMap, NULL);
    }
  } Procedure_ENDEACH_BasicBlock;
  HTable_Dtor(dedicatedMap);
  RegisterSet_Dtor(dedicatedRefs);
}

#ifdef $XCC_h
/**
 * Build this Procedure CodeRegion(s) from the BasicBlock(s).
 *
 * After the call, the procedure contains one or more CodeRegions which are a 
 * partition of its BasicBlocks.
 */
void
Procedure_buildCodeRegions(Procedure this);
#endif//$XCC_h

void
Procedure_buildCodeRegions(Procedure this)
{
  Except_CHECK(!Procedure_isBuilt(this));
  // Push the BasicBlock(s) on their respective CodeRegion.
  Procedure_FOREACH_BasicBlock(this, basicBlock) {
    CodeRegion codeRegion = BasicBlock_CODEREGION(basicBlock);
    CodeRegion_pushBasicBlock(codeRegion, basicBlock);
  } Procedure_ENDEACH_BasicBlock;
  // Mark the BasicBlock(s) that have a predecessor or successor inside their CodeRegion.
  Procedure_FOREACH_BasicBlock(this, basicBlock) {
    CodeRegion codeRegion = BasicBlock_CODEREGION(basicBlock);
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(basicBlock, edge) {
      BasicBlock destBlock = BasicBlockEdge_DESTBLOCK(edge);
      if (BasicBlock_CODEREGION(destBlock) == codeRegion) {
        *BasicBlock__MARK(destBlock) |= 0x01;
        *BasicBlock__MARK(basicBlock) |= 0x10;
      }
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
  } Procedure_ENDEACH_BasicBlock;
  // Collect the CodeRegion entry and exit BasicBlock(s).
  Procedure_FOREACH_BasicBlock(this, basicBlock) {
    CodeRegion codeRegion = BasicBlock_CODEREGION(basicBlock);
    // The BasicBlock(s) without predecessors inside their CodeRegion are entering blocks.
    if (!(BasicBlock_MARK(basicBlock) & 0x01) || BasicBlock_isEntry(basicBlock)) {
      CodeRegion_setEnterBlock(codeRegion, basicBlock);
    }
    // The BasicBlock(s) without successors inside their CodeRegion are leaving blocks.
    if (!(BasicBlock_MARK(basicBlock) & 0x10) || BasicBlock_isExit(basicBlock)) {
      CodeRegion_setLeaveBlock(codeRegion, basicBlock);
    }
    *BasicBlock__MARK(basicBlock) = 0;
  } Procedure_ENDEACH_BasicBlock;
  *Procedure__FLAGS(this) |= ProcedureFlag_Built;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Update the target label of an Operation.
 */
void
Procedure_updateTargetedOperation(Procedure this, Operation operation, Label label);
#endif//$XCC_h

void
Procedure_updateTargetedOperation(Procedure this, Operation operation, Label label)
{
  int32_t targetIndex = Operator_targetIndex(Operation_operator(operation));
  if (targetIndex >= 0) {
    Temporary *arguments = Operation_arguments(operation);
    Temporary target_temporary = arguments[targetIndex];
    Literal target_literal = Temporary_LITERAL(target_temporary);
    Immediate immediate = Literal_immediate(target_literal);
    Except_REQUIRE(Temporary_isLabel(target_temporary));
    target_temporary = Procedure_makeLabelTemporary(this, label, immediate);
    arguments[targetIndex] = target_temporary;
  } else Except_NEVER(true);
}

#ifdef $XCC_h
/**
 * Searches through all BasicBlocks of the procedure and looks for ones with multiple
 * labels. Replaces all occurences of the additional labels in the entire procedure
 * by the first label.
 */
void
Procedure_pruneAliasLabels(Procedure this);
#endif//$XCC_h

void
Procedure_pruneAliasLabels(Procedure this)
{
  Procedure_FOREACH_BasicBlock(this, block) {
  if (BasicBlock_labelCount(block) < 2)
      continue;

    Label mainLabel = BasicBlock_label(block);
    Label * aliases = (Label *) BasicBlock_labels(block);
    for (int i=1; i < BasicBlock_labelCount(block); i++) {
      Procedure_FOREACH_BasicBlock(this, iterBlock) {
        BasicBlock_FOREACH_Operation(iterBlock, op) {
          int32_t targetIndex = Operator_targetIndex(Operation_operator(op));
          if (targetIndex < 0)
            continue;

          Temporary *arguments = Operation_arguments(op);
          Temporary target_temporary = arguments[targetIndex];
          Literal target_literal = Temporary_LITERAL(target_temporary);

          if (!Literal_isLabel(target_literal))
            continue;

          Label currLabel = Literal_label(target_literal);
          if (!strcmp(Label_name(currLabel), Label_name(aliases[i]))) {
            Immediate immediate = Literal_immediate(target_literal);
            target_temporary =
              Procedure_makeLabelTemporary(this, mainLabel, immediate);
            arguments[targetIndex] = target_temporary;
          }
        } BasicBlock_ENDEACH_Operation;
      } Procedure_ENDEACH_BasicBlock;
    }
  } Procedure_ENDEACH_BasicBlock;
}

#ifdef $XCC_h
/**
 * Update the immediate after inductive relaxation.
 */
void
Procedure_updateRelaxedOperation(Procedure this, Operation operation,
                                 int32_t regIndex, int64_t step, int instance);
#endif//$XCC_h

void
Procedure_updateRelaxedOperation(Procedure this, Operation operation,
                                 int32_t regIndex, int64_t step, int instance)
{
  ImmediateValue bias = 0;
  Operator operator = Operation_operator(operation);
  int32_t immediateIndex = Operator_immediateIndex(operator);
  Temporary temporary = Operation_getArgument(operation, immediateIndex);
  Literal literal = Temporary_LITERAL(temporary);
  Immediate immediate = Literal_immediate(literal);
  Except_REQUIRE(regIndex >= 0 && immediateIndex >= 0);
  if (Operation_isAdditive(operation, regIndex, immediateIndex)) {
    // Cases like LOAD(ADD(x, step), m) -> LOAD(x, m+step)
    bias += step*instance;
  } else if (Operation_isSubtractive(operation, regIndex, immediateIndex)) {
    // Cases like CMPEQ(ADD(x, step), m) -> CMPEQ(x, m-step)
    bias -= step*instance;
  } else Except_NEVER(true);
  if (Temporary_isAbsolute(temporary)) {
    ImmediateValue value = Literal_value(literal) + bias;
    temporary = Procedure_makeAbsoluteTemporary(this, value, immediate);
  } else if (Temporary_isSymbol(temporary)) {
    Symbol symbol = Literal_symbol(literal);
    ImmediateValue offset = Literal_offset(literal) + bias;
    temporary = Procedure_makeSymbolTemporary(this, symbol, offset, immediate);
  } else Except_NEVER(true);
  Operation_setArgument(operation, immediateIndex, temporary);
  // In case this was a PureCopy, it is no longer one.
  *Operation__FLAGS(operation) &= ~OperationFlag_PureCopy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pretty print this Procedure.
 */
bool
Procedure_pretty(Procedure this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Procedure_pretty(Procedure this, FILE* file)
{
  if (this == NULL) return true;
  if (file == NULL) file = stderr;
  fprintf(file, "Procedure %s\n", Procedure_name(this));
  Procedure_FOREACH_BasicBlock(this, basicBlock) {
    LoopScope loopScope = BasicBlock_LOOPSCOPE(basicBlock);
    BasicBlock headerBlock = loopScope != NULL? LoopScope_HEADERBLOCK(loopScope): NULL;
    if (loopScope != NULL && headerBlock == basicBlock) {
      LoopScope_pretty(loopScope, file);
      fprintf(file, "\n");
    }
    BasicBlock_pretty(basicBlock, file);
  } Procedure_ENDEACH_BasicBlock;
  {
    DataList dataList = Procedure_dataList(this);
    if(!DataList_isEmpty(dataList)) {
      fprintf(file, "  Data of procedure %s\n", Procedure_name(this));
      DataList_pretty(dataList, file);
    }
  }
  return true;
}
#endif // _NTRACE

#ifdef $XCC_h
/**
 * Print this Procedure in assembly language.
 */
bool
Procedure_asmPrint(Procedure this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Procedure_asmPrint(Procedure this, FILE* file)
{
  Symbol symbol = Procedure_symbol(this);
  if (this == NULL) return true;
  if (file == NULL) file = stderr;

  Except_CHECK(Symbol_class(symbol) == SymbolClass_FUNC);
  Except_CHECK(Symbol_store(symbol) == SymbolStore_TEXT);
  // Could be SymbolStore_EXTERN if function is undefined

  if (Procedure_getSection(this)) 
    DataSection_asmPrint(Procedure_getSection(this), file);
  else 
    fprintf(file, "\t.text\n");

  fprintf(file, "\t.align %d\n", 
      Procedure_ALIGN(this) != 0 ?
      Procedure_ALIGN(this) :
      8);
  //TODO: not hard-coded alignment! Convention?

  switch(Symbol_export(symbol)) {
    case SymbolExport_WEAK:
      fprintf(file, "\t.weak %s\n", Procedure_name(this));
      break;
    case SymbolExport_LOCAL:
      // Nothing
      break;
    case SymbolExport_LOCAL_INTERNAL:
      fprintf(file, "\t.internal %s\n", Procedure_name(this));
      break;
    case SymbolExport_GLOBAL_INTERNAL:
      fprintf(file, "\t.globl %s\n", Procedure_name(this));
      fprintf(file, "\t.internal %s\n", Procedure_name(this));
      break;
    case SymbolExport_GLOBAL_HIDDEN:
      fprintf(file, "\t.globl %s\n", Procedure_name(this));
      fprintf(file, "\t.hidden %s\n", Procedure_name(this));
      break;
    case SymbolExport_GLOBAL_PROTECTED:
      fprintf(file, "\t.globl %s\n", Procedure_name(this));
      fprintf(file, "\t.protected %s\n", Procedure_name(this));
      break;
    case SymbolExport_GLOBAL_PREEMPTIBLE:
      fprintf(file, "\t.globl %s\n", Procedure_name(this));
      break;
    default:
      Except_CHECK(false);
  }
  fprintf(file, "\t.type\t%s, @function\n", Procedure_name(this));


//  print function label
  fprintf(file, "%s:\n", Procedure_name(this));

  Procedure_FOREACH_BasicBlock(this, basicBlock) {
    LoopScope loopScope = BasicBlock_LOOPSCOPE(basicBlock);
    BasicBlock headerBlock = loopScope != NULL? LoopScope_HEADERBLOCK(loopScope): NULL;
    BasicBlock_asmPrint(basicBlock, file);
  } Procedure_ENDEACH_BasicBlock;

  /*  fprintf(file, "\t.endp %s\n", Procedure_name(this));*/
  fprintf(file, "\t.size\t%s, .-%s\n", Procedure_name(this), Procedure_name(this));

//  fprintf(file, "Procedure %s\n", Procedure_name(this));
//  Procedure_FOREACH_BasicBlock(this, basicBlock) {
//    LoopScope loopScope = BasicBlock_LOOPSCOPE(basicBlock);
//    BasicBlock headerBlock = loopScope != NULL? LoopScope_HEADERBLOCK(loopScope): NULL;
//    if (loopScope != NULL && headerBlock == basicBlock) {
//      LoopScope_pretty(loopScope, file);
//      fprintf(file, "\n");
//    }
//    BasicBlock_pretty(basicBlock, file);
//  } Procedure_ENDEACH_BasicBlock;
//  {
//    DataList dataList = Procedure_dataList(this);
//    if(!DataList_isEmpty(dataList)) {
//      fprintf(file, "  Data of procedure %s\n", Procedure_name(this));
//      DataList_pretty(dataList, file);
//    }
//  }

  // Print data for this function if any
  DataList_asmPrint(Procedure_dataList(this), file);
  
  return true;
}
#endif // _NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Specialization of IndexedTable for Procedure(s).
 */
struct ProcedureTable_ {
  //@args	Memory memory, int32_t estimate
  IndexedTable_ __;
  //@access MEMORY	IndexedTable_memory(ProcedureTable____(this))
  //@access COUNT	IndexedTable_count(ProcedureTable____(this))
  //@access BASE	(const Procedure *)IndexedTable_base(ProcedureTable____(this))
};
#endif//$XCC_h

#ifdef $XCC_h
static inline IndexedTable
ProcedureTable__(ProcedureTable this)
{
  return ProcedureTable____(this);
}
static inline const_IndexedTable
ProcedureTable__const(const_ProcedureTable this)
{
  return ProcedureTable____(this);
}
#endif//$XCC_h

ProcedureTable
ProcedureTable_Ctor(ProcedureTable this,
                    Memory memory, int32_t estimate)
{
  IndexedTable_Ctor(ProcedureTable____(this), memory, estimate);
  return this;
}

void
ProcedureTable_Dtor(ProcedureTable this)
{
  IndexedTable_Dtor(ProcedureTable____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ProcedureTable_)\t%zu\n", sizeof(ProcedureTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
#define ProcedureTable_memory(this) \
  IndexedTable_memory(ProcedureTable__const(this))
#define ProcedureTable_count(this) \
  IndexedTable_count(ProcedureTable__const(this))
#define ProcedureTable_empty(this) \
  IndexedTable_empty(ProcedureTable__(this))
#define ProcedureTable_contains(this, procedure) \
  IndexedTable_contains(ProcedureTable__const(this), Procedure__const(procedure))
#define ProcedureTable_insert(this, procedure) \
  IndexedTable_insert(ProcedureTable__(this), Procedure__(procedure))
#define ProcedureTable_remove(this, procedure) \
  IndexedTable_remove(ProcedureTable__(this), Procedure__const(procedure))
#define ProcedureTable_removeHigh(this, procedure, high) \
  IndexedTable_removeHigh(ProcedureTable__(this), Procedure__const(procedure), high)
#endif//$XCC_h

#ifdef $XCC_h
static inline const Procedure *
ProcedureTable_base(const_ProcedureTable this)
{
  return ProcedureTable_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this ProcedureTable Procedure(ies).
 */
#define ProcedureTable_FOREACH_Procedure(this, procedure) { \
  IndexedTable_FOREACH_Indexed(ProcedureTable__const(this), \
                                   Procedure, procedure) { \
    int32_t ProcedureTable_Procedure_INDEX = IndexedTable_INDEX;
#define ProcedureTable_ENDEACH_Procedure \
  } IndexedTable_ENDEACH_Indexed; \
}
#endif//$XCC_h

#ifdef $XCC_h
bool
ProcedureTable_pretty(const_ProcedureTable this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
ProcedureTable_pretty(const_ProcedureTable this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "{");
  ProcedureTable_FOREACH_Procedure(this, procedure) {
    fprintf(file, " [%d]", Procedure_INDEX(procedure));
  } ProcedureTable_ENDEACH_Procedure;
  fprintf(file, " }");
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Procedure module.
 */
void
Procedure_INIT(void);
#endif//$XCC__h

IndexedFactory_ Procedure_Factory[1];

void
Procedure_INIT(void)
{
  IndexedFactory_Ctor(Procedure_Factory);
#ifndef _NTRACE
  ETable_Ctor(ProcedureFlag_Table, Memory_Root, ProcedureFlagEntries);
#endif//_NTRACE
}

#ifdef $XCC__h
/**
 * Finalize Procedure module.
 */
#ifndef _NTRACE
void
Procedure_FINI(void);
#else//_NTRACE
#define Procedure_FINI()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
Procedure_FINI(void)
{
  ETable_Dtor(ProcedureFlag_Table);
}
#endif//_NTRACE

#if XCC__C
static void
Procedure_TEST(void)
{
#include "LIR/Procedure_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Procedure_TEST);
  return 0;
}
#endif

