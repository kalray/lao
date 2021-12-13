#ifdef $XCC_h
/*
 * !!!!	Frame
 *
 * Florent Bouchez <florent.bouchez@kalray.eu>
 *
 * Copyright 2011 - ... Kalray
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
 * @brief Handle the stack frames of procedures.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/Frame.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Implement a stack Frame.
 */
struct Frame_ {
  //@args	Memory memory
/*  Procedure PROCEDURE;		//!< The procedure it belongs to.*/
  uint8_t SPILLSIZE;		//!< Size of the spill area (contains spilled 
                                //!  variables and local variables).
  uint8_t FRAMEMARKERSIZE;	//!< Size of the frame marker (allows stack unwinding,
                                //!  can contain copy of previous FP for instance.
  uint8_t ARGSSIZE;		//!< Size of the "arguments" area if call to function
                                //!  with more args than dedicated registers.
  uint8_t SCRATCHSIZE;		//!< Size of the "scratch" area.
                                //!  16 bytes if contains function calls,
                                //!  not necessary if tail function.
  bool FIXED;			//!< True if frame has a fixed size.
  //@access SIZE   Frame_SPILLSIZE(this) + Frame_FRAMEMARKERSIZE(this) + Frame_ARGSSIZE(this) + Frame_SCRATCHSIZE(this)
  PtrSeq_ BLOCKS[1];		//!< "Blocks" inside the frame. For instance, 
                                //!  for spilled variables, callee saved 
                                //!  registers, incoming parameters, local 
                                //!  variables.
  PtrSeq_ FPBLOCK[1];		//!< Symbols referenced by FP
  PtrSeq_ SPBLOCK[1];		//!< Symbols referenced by SP
  PtrSeq_ FORMALS[1];		//!< Formal parameters passed on stack to the procedure
  PtrSeq_ SYMBOLS[1];		//!< All symbols that are allocated to this frame.
};
#endif//$XCC_h

Frame
Frame_Ctor(Frame this, Memory memory) //, Procedure procedure)
{
/*  Indexed_Ctor(Frame____(this), Frame_Factory);*/
/*  *Frame__PROCEDURE(this) = procedure;*/
  *Frame__SPILLSIZE(this) = 0;
  *Frame__FRAMEMARKERSIZE(this) = 0;
  *Frame__ARGSSIZE(this) = 0;
  *Frame__SCRATCHSIZE(this) = 0;
  *Frame__FIXED(this) = true;
  PtrSeq_Ctor(Frame_BLOCKS(this), memory, 0);
  PtrSeq_Ctor(Frame_FPBLOCK(this), memory, 0);
  PtrSeq_Ctor(Frame_SPBLOCK(this), memory, 0);
  PtrSeq_Ctor(Frame_FORMALS(this), memory, 0);
  PtrSeq_Ctor(Frame_SYMBOLS(this), memory, 0);
  return this;
}

void
Frame_Dtor(Frame this)
{
  PtrSeq_Dtor(Frame_BLOCKS(this));
  PtrSeq_Dtor(Frame_FPBLOCK(this));
  PtrSeq_Dtor(Frame_SPBLOCK(this));
  PtrSeq_Dtor(Frame_SYMBOLS(this));
}


size_t
Frame_Size(Memory memory) //, Procedure procedure)
{
  return sizeof(Frame_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Frame_)\t%zu\n", sizeof(Frame_));
}
#endif//$XCC__c


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Get the size of the Frame.
 */
uint32_t
Frame_size(Frame this);
#endif//$XCC_h

uint32_t
Frame_size(Frame this)
{
  return Frame_SIZE(this);
}

#ifdef $XCC_h
/**
 * Get the size of the spill area.
 */
uint8_t
Frame_spillSize(Frame this);
#endif//$XCC_h

uint8_t
Frame_spillSize(Frame this)
{
  return Frame_SPILLSIZE(this);
}

#ifdef $XCC_h
/**
 * Set the size of the spill area.
 */
void
Frame_setSpillSize(Frame this, uint8_t size);
#endif//$XCC_h

void
Frame_setSpillSize(Frame this, uint8_t size)
{
  *Frame__SPILLSIZE(this) = size;
}

#ifdef $XCC_h
/**
 * Set the size of the stacked arguments to callee functions.
 */
void
Frame_setArgsSize(Frame this, uint8_t size);
#endif//$XCC_h

void
Frame_setArgsSize(Frame this, uint8_t size)
{
  *Frame__ARGSSIZE(this) = size;
}



#ifdef $XCC_h
/**
 * Return whether this frame has a fixed or variable size.
 */
bool
Frame_isFixed(Frame this);
#endif//$XCC_h

bool
Frame_isFixed(Frame this)
{
  return Frame_FIXED(this);
}

#ifdef $XCC_h
/**
 * Set the fixed/variable size status of the frame.
 */
void
Frame_setFixed(Frame this, bool fixed);
#endif//$XCC_h

void
Frame_setFixed(Frame this, bool fixed)
{
  *Frame__FIXED(this) = fixed;
}

#ifdef $XCC_h
/**
 * Return whether this frame has a scratch area or not.
 */
bool
Frame_hasScratch(Frame this);
#endif//$XCC_h

bool
Frame_hasScratch(Frame this)
{
  return Frame_SCRATCHSIZE(this) != 0;
}

#ifdef $XCC_h
/**
 * Return the size of the scratch area.
 */
uint8_t
Frame_scratchSize(Frame this);
#endif//$XCC_h

uint8_t
Frame_scratchSize(Frame this)
{
  return Frame_SCRATCHSIZE(this);
}


#ifdef $XCC_h
/**
 * Set the size of the scratch area.
 */
void
Frame_setScratch(Frame this, int8_t size);
#endif//$XCC_h

void
Frame_setScratch(Frame this, int8_t size)
{
  *Frame__SCRATCHSIZE(this) = size;
}

#ifdef $XCC__h
/**
 * Add an symbol (of unknown base) to the frame.
 */
static int32_t
Frame_spillAreaOffset(Frame this);
#endif//$XCC__h

static int32_t
Frame_spillAreaOffset(Frame this)
{
  int32_t offset = 0;
  if(Frame_isFixed(this)) {
    offset += Frame_SCRATCHSIZE(this);
    offset += Frame_ARGSSIZE(this);
    offset += Frame_FRAMEMARKERSIZE(this);
  }
  return offset;
}
 


#ifdef $XCC_h
/**
 * Add an Formal symbol to the frame.
 */
void
Frame_addFormal(Frame this, Symbol symbol);
#endif//$XCC_h

void
Frame_addFormal(Frame this, Symbol symbol)
{
  Except_CHECK(Symbol_store(symbol) == SymbolStore_FORMAL);
  PtrSeq_push2(Frame_FORMALS(this), symbol);
}
 


#ifdef $XCC_h
/**
 * Add an symbol (of unknown base) to the frame.
 */
bool
Frame_hasSymbol(Frame this, Symbol symbol);
#endif//$XCC_h

bool
Frame_hasSymbol(Frame this, Symbol symbol)
{
  return PtrSeq_contains(Frame_SYMBOLS(this), symbol) != -1;
}



#ifdef $XCC_h
/**
 * Add an symbol (of unknown base) to the frame.
 */
void
Frame_addSymbol(Frame this, Symbol symbol);
#endif//$XCC_h

void
Frame_addSymbol(Frame this, Symbol symbol)
{
  PtrSeq_push2(Frame_SYMBOLS(this), symbol);
}
 

#ifdef $XCC_h
/**
 * Add an SP-based symbol to the frame.
 */
void
Frame_addSPSymbol(Frame this, Symbol symbol);
#endif//$XCC_h

void
Frame_addSPSymbol(Frame this, Symbol symbol)
{
  PtrSeq_push2(Frame_SPBLOCK(this), symbol);
}

#ifdef $XCC_h
/**
 * Add an FP-based symbol to the frame.
 */
void
Frame_addFPSymbol(Frame this, Symbol symbol);
#endif//$XCC_h

void
Frame_addFPSymbol(Frame this, Symbol symbol)
{
  PtrSeq_push2(Frame_FPBLOCK(this), symbol);
}

#ifdef $XCC_h
/**
 * Resolve the spill symbols in this frame.
 * This will change the spill area size. A call to @c 
 * Procedure_updateFrameSize() must be made afterwards to update stack frame
 * adjusts.
 */
void
Frame_resolveSpillSymbols(Frame this);
#endif//$XCC_h

void
Frame_resolveSpillSymbols(Frame this)
{
  int32_t spillAreaSize = 0;
  bool fixedFrame = Frame_isFixed(this);
  int32_t spillAreaOffset = Frame_spillAreaOffset(this);


  //fprintf(stderr, "%s:%d all symbols in frame:\n", __FILE__, __LINE__);
  //PtrSeq_FOREACH(Frame_SYMBOLS(this), Symbol, symbol) {
  //  fprintf(stderr, "\tSymbol: %d, %s\n", Symbol_identity(symbol), Symbol_name(symbol));
  //} PtrSeq_ENDEACH;

  PtrSeq_FOREACH(Frame_SYMBOLS(this), Symbol, symbol) {
    if(Symbol_isSpill(symbol)) {
      Except_CHECK(!Symbol_isResolved(symbol));

      if(fixedFrame) { // Growing upwards
        Frame_addSPSymbol(this, symbol);
        // Assign current value
        Symbol_setValue(symbol, spillAreaOffset + spillAreaSize);
        // Then increase spill area size
        spillAreaSize += Symbol_size(symbol);
      }
      else { // Growing downwards
        Frame_addFPSymbol(this, symbol);
        // First update spill area size
        spillAreaSize += Symbol_size(symbol);
        // Assign current value
        Symbol_setValue(symbol, spillAreaOffset - spillAreaSize);
      }
      Symbol_setResolved(symbol); 
      /*    PtrSeq_empty(Frame_SYMBOLS(this));*/ //TODO: should we remove items from this list?
    }
    //fprintf(stderr, "%s:%d Symbol: %s\n", __FILE__, __LINE__, Symbol_name(symbol));
    Symbol_pretty(symbol, stderr);
  } PtrSeq_ENDEACH;
  Except_CHECK(spillAreaSize >= 0);
  Frame_setSpillSize(this, spillAreaSize);

  //fprintf(stderr, "%s:%d spill size is %d\n", __FILE__, __LINE__, spillAreaSize);
}

#ifdef $XCC_h
/**
 * Resolve the formal parameters symbols in this frame.
 * This will scan the formal parameters, in the order they were added to the
 * frame, to determine their offset from SP/FP.
 * This must be called once the frame size is known in the case of a fixed
 * frame, i.e., after @c Frame_resolveSpillSymbols().
 */
void
Frame_resolveFormalParameters(Frame this);
#endif//$XCC_h

void
Frame_resolveFormalParameters(Frame this)
{
  uint32_t spillAreaSize = 0;
  bool fixedFrame = Frame_isFixed(this);
  int32_t formalOffset = 16; // TODO: hard-coded scratch-size left by caller function

  if(fixedFrame) formalOffset += Frame_size(this);

  //fprintf(stderr, "%s:%d formal params, starting at %d\n", __FILE__, __LINE__,
  //    formalOffset);

  PtrSeq_FOREACH(Frame_FORMALS(this), Symbol, symbol) {
    if(Symbol_store(symbol) == SymbolStore_FORMAL) {

      if(fixedFrame) {
        Frame_addSPSymbol(this, symbol);
      } else {
        Frame_addFPSymbol(this, symbol);
      }
      // Assign current value
      Symbol_setValue(symbol, formalOffset);
      fprintf(stderr, "%s:%d formal %s assigned to %d\n", __FILE__, __LINE__,
          Symbol_name(symbol), formalOffset);
      // Then increase spill area size
      formalOffset += Symbol_size(symbol);
      Symbol_setResolved(symbol); 

    }
  } PtrSeq_ENDEACH;
}



////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize Frame.
 */
void
Frame_INIT(void);
#endif//$XCC__h

void
Frame_INIT(void)
{
}

#ifdef $XCC__h
/**
 * Finalize Frame.
 */
#define Frame_FINI()
#endif//$XCC__h

#if XCC__C
static void
Frame_TEST(void)
{
#include "AIR/Frame_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(Frame_TEST);
  return 0;
}
#endif

