#ifdef $XCC_h
/*
 * !!!! CodeBuffer.xcc
 *
 * Artur Pietrek (artur.pietrek@kalray.eu)
 * Duco van Amstel (duco.van-amstel@kalray.eu)
 *
 * Copyright 2011-2013 Kalray
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
 * @ingroup JIT
 * @brief A Code buffer
 */
#endif//$XCC_h

#include "JIT/JIT_.h"

#ifdef $XCC__h
#include "JIT/CodeBuffer.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
struct CodeBufferEntry_ {
  Symbol  SYMBOL;          //!< A symbol of this entry's function
  size_t  SIZE;            //!< Size of this entry
  void*   NEXT;            //!< Address of next entry
};
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * A CodeBuffer
 * 
 */
struct CodeBuffer_;
#endif//$XCC_h

#ifdef $XCC__h
struct CodeBuffer_ {
  //@args Memory memory
  Memory  MEMORY;                //!< Where to allocate objects
  uint8_t *PTR;
  uint8_t *END;
  HTable_ CALLSITES[1];          //!< A map of call sites used for triggering compilation/call back
  PlatformUInt  BUFFER[1024];         //!< Where the code is kept
};
#endif//$XCC__h

CodeBuffer
CodeBuffer_Ctor(CodeBuffer this,
             Memory memory)
{
  CodeBufferEntry entry = NULL;
  *CodeBuffer__MEMORY(this) = memory;
  *CodeBuffer__PTR(this) = (uint8_t*)CodeBuffer_BUFFER(this);
  *CodeBuffer__END(this) = (uint8_t*)(CodeBuffer_BUFFER(this) + 1024);
  HTable_Ctor(CodeBuffer_CALLSITES(this), memory, 512);
  entry = (CodeBufferEntry)CodeBuffer_PTR(this);
  CodeBufferEntry_SYMBOL(entry) = NULL;
  CodeBufferEntry_NEXT(entry) = NULL;
  return this;
}

void
CodeBuffer_Dtor(CodeBuffer this)
{
//   HTable_FOREACH(CodeBuffer_ENTRIES(this), key, CodeBufferEntry, value)
//   {       
//     CodeBufferEntry entry = (CodeBufferEntry)value;
//     PtrSet users = CodeBufferEntry_USERS(entry);       
//     PtrSet refList = CodeBufferEntry_REFLIST(entry);
//     PtrSet_Dtor(users);
//     PtrSet_Dtor(refList);
//   } HTable_ENDEACH;
  
//   HTable_Dtor(CodeBuffer_ENTRIES(this));
  HTable_Dtor(CodeBuffer_CALLSITES(this));
}

#ifdef $XCC_h
CodeBuffer
CodeBuffer_new(Memory parent);
#endif//$XCC_h

CodeBuffer
CodeBuffer_new(Memory parent)
{
  Memory memory = Memory_new(parent, true);
  size_t size = CodeBuffer_Size(memory);
  CodeBuffer this = Memory_alloc(memory, size);
  return CodeBuffer_Ctor(this, memory);
}

#ifdef $XCC_h
CodeBuffer
CodeBuffer_delete(CodeBuffer this);
#endif//$XCC_h

CodeBuffer
CodeBuffer_delete(CodeBuffer this)
{
  if (this != NULL) {
    Memory memory = CodeBuffer_MEMORY(this);
#ifndef _NDTOR
    CodeBuffer_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

size_t
CodeBuffer_Size(Memory memory)
{
  return sizeof(CodeBuffer_);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// #ifdef $XCC__h
// static inline CodeBufferEntry
// CodeBuffer_addCodeBufferEntry(CodeBuffer this, Symbol symbol)
// {
//   HTableKey key = (HTableKey)symbol;
//   CodeBufferEntry entry = (CodeBufferEntry)HTable_search(CodeBuffer_ENTRIES(this), key);
//   if (!entry)
//   {    
//     entry = (CodeBufferEntry)HTable_insert(CodeBuffer_ENTRIES(this), key, sizeof(CodeBufferEntry_));
//     Except_CHECK(entry != NULL);
// //     PtrSet userList = CodeBufferEntry_USERS(entry);
// //     PtrSet refList = CodeBufferEntry_REFLIST(entry);
// //     PtrSet_Ctor(userList, CodeBuffer_MEMORY(this), 20);
// //     PtrSet_Ctor(refList, CodeBuffer_MEMORY(this), 20);
//   }
//   return entry;
// }
// #endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
void*
CodeBuffer_getPointer(CodeBuffer this, Symbol symbol, PlatformUInt codeSize);
#endif//$XCC_h

void*
CodeBuffer_getPointer(CodeBuffer this, Symbol symbol, PlatformUInt codeSize) 
{
  uint8_t *ptr = CodeBuffer_PTR(this);
  uint8_t *res = ptr;
  size_t size = 0;
  size_t cs = (codeSize+3)&~3;
  size_t es = (sizeof(CodeBufferEntry_)+3)&~3;
  JIT_TRACE && fprintf(JIT_TRACE, "== looking for slot\n"
                       "   symbol: %s\n"
                       "   code size: %u\n"
                       "   code size aligned: %zu\n"
                       "   entry size: %zu\n"
                       "   entry size aligned: %zu\n",
                       Symbol_name(symbol), codeSize, cs, 
                       sizeof(CodeBufferEntry_),
                       es);
  do
  {
    CodeBufferEntry entry = (CodeBufferEntry)ptr;    
    //there's no space at the end of the buffer to keep this function
    //start from beginning
    if (res+cs+es>=CodeBuffer_END(this))
    {      
      size = 0;
      if (res == ptr)
      {
        JIT_TRACE && fprintf(JIT_TRACE, "-- codeSize too big\n");
        return NULL;
      }
      
      JIT_TRACE && fprintf(JIT_TRACE, "-- codeSize won't fit, starting from the beginning\n");
      res = ptr = (uint8_t*)CodeBuffer_BUFFER(this);      
      continue;
    }
    
    //empty entry -- means that rest of the buffer is free
    if (CodeBufferEntry_SYMBOL(entry) == NULL
      || CodeBufferEntry_NEXT(entry) == NULL)
    {      
      JIT_TRACE && fprintf(JIT_TRACE, "-- found null entry, rest of buffer is free\n");
      *CodeBufferEntry__SYMBOL(entry) = symbol;
      *CodeBufferEntry__SIZE(entry) = codeSize;
      
      ptr+=codeSize+es;           
      
      if (ptr+es>=CodeBuffer_END(this)) 
      {
        JIT_TRACE && fprintf(JIT_TRACE, "-- ptr at the end, setting to the begining\n");
        CodeBuffer_PTR(this) = (uint8_t*)CodeBuffer_BUFFER(this);
      } else {
        JIT_TRACE && fprintf(JIT_TRACE, "-- appending null entry \n");
        CodeBufferEntry_NEXT(entry) = ptr;
        entry = (CodeBufferEntry)ptr;
        CodeBufferEntry_SYMBOL(entry) = NULL;
        CodeBufferEntry_SIZE(entry) = 0;
        CodeBuffer_PTR(this) = ptr;
      }
      return res+es;
    } else {
      size += (uint8_t*)CodeBufferEntry_NEXT(entry) - ptr;
      ptr = CodeBufferEntry_NEXT(entry);
      JIT_TRACE && fprintf(JIT_TRACE, "-- found entry\n"
                           "   res: %p\n"
                           "   ptr: %p\n"
                           "   size: %zu\n", res, ptr, size);
       if (size >= codeSize+es)
       {
         JIT_TRACE && fprintf(JIT_TRACE, "-- overwriting entries\n");
         entry = (CodeBufferEntry)res;
         while ((uint8_t*)entry != ptr)
         {
           Symbol symbol = CodeBufferEntry_SYMBOL(entry);
           Procedure procedure = Symbol_PROCEDURE(symbol);
           JIT_TRACE && fprintf(JIT_TRACE, "-- removing function %s\n", Symbol_name(symbol));
           if (procedure!=NULL)
             Procedure_setAddress(procedure, 0);
           entry = CodeBufferEntry_NEXT(entry);
         }
         entry = (CodeBufferEntry)res;
         *CodeBufferEntry__SYMBOL(entry) = symbol;
         *CodeBufferEntry__SIZE(entry) = codeSize;
         *CodeBufferEntry__NEXT(entry) = ptr;
         return res+es;
       }
    }    
  } while (ptr!=CodeBuffer_PTR(this));
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
void
CodeBuffer_flushFunction(CodeBuffer this, Symbol symbol);
#endif//$XCC_h

void
CodeBuffer_flushFunction(CodeBuffer this, Symbol symbol)
{
//   HTableKey key = (HTableKey)symbol;
//   CodeBufferEntry entry = (CodeBufferEntry)HTable_search(CodeBuffer_ENTRIES(this), key);
//   if (!entry) return;
//   PtrSet userList = CodeBufferEntry_USERS(entry);
//   PtrSet refList = CodeBufferEntry_REFLIST(entry);
  
  // checkout all references and remove entries from it's user lists
  // create a stub, update symbol addres and patch all users
  // free space in the code buffer
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// #ifdef $XCC_h
// void
// CodeBuffer_addUser(CodeBuffer this, Symbol symbol, Instruction instruction);
// #endif//$XCC_h
// 
// void
// CodeBuffer_addUser(CodeBuffer this, Symbol symbol, Instruction instruction)
// {
//   CodeBufferEntry entry = CodeBuffer_addCodeBufferEntry(this, symbol);
//   // push the instruction on the list
//   PtrSet_insert(CodeBufferEntry_USERS(entry), (PtrSetMember)instruction);  
// }

// #ifdef $XCC_h
// void
// CodeBuffer_addReference(CodeBuffer this, Symbol symbol, Instruction instruction);
// #endif//$XCC_h
// 
// void
// CodeBuffer_addReference(CodeBuffer this, Symbol symbol, Instruction instruction)
// {
//   CodeBufferEntry entry = CodeBuffer_addCodeBufferEntry(this, symbol);
//   // push the instruction on the list
//   PtrSet_insert(CodeBufferEntry_REFLIST(entry), (PtrSetMember)instruction);
// }

// #ifdef $XCC_h
// static inline const_PtrSet
// CodeBuffer_getUsers(CodeBuffer this, Symbol symbol)
// {
//   CodeBufferEntry entry = (CodeBufferEntry)HTable_search(CodeBuffer_ENTRIES(this), (HTableKey)symbol);
//   if (entry == NULL) return NULL;
//   return (PtrSet)CodeBufferEntry_USERS(entry);
// }
// #endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
void
CodeBuffer_insertCallSite(CodeBuffer this, uintptr_t address, Symbol symbol);
#endif//$XCC__h

void
CodeBuffer_insertCallSite(CodeBuffer this, uintptr_t address, Symbol symbol)
{
  *(Symbol *)HTable_lookup(CodeBuffer_CALLSITES(this), 
                           (HTableKey)address, sizeof(symbol)) = symbol;
}

#ifdef $XCC__h
void
CodeBuffer_removeCallSite(CodeBuffer this, uintptr_t address);
#endif//$XCC__h

void
CodeBuffer_removeCallSite(CodeBuffer this, uintptr_t address)
{
  HTable_remove(CodeBuffer_CALLSITES(this), (HTableKey)address, NULL);
}

#ifdef $XCC__h
static inline Symbol
CodeBuffer_getCallSite(CodeBuffer this, uintptr_t address)
{
  HTableValue value = HTable_search(CodeBuffer_CALLSITES(this), (HTableKey)address);
  if (value!=NULL) return *(Symbol *)value;
  
  return NULL;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC__c
{
  fprintf(stderr, "Testing CodeBuffer...\n");
  fprintf(stderr, "sizeof(CodeBuffer_)\t%zu\n", sizeof(CodeBuffer_));
}
#endif//$XCC__c

#ifdef $XCC__c
{
  SymbolTable symbolTable = SymbolTable_new(Memory_Root, 16);  
  int i;
  int sizes[] = 
  {
    128, 100, 256, 128, 96, 346, 720, 720, 1024, 2048,
    8192, 64, 256, 512, 16384, 32, 144, 32768, 4096, 128
  };
  
  CodeBuffer codeBuffer = CodeBuffer_new(Memory_Root);
  
  srand((unsigned)(time(0)));
  
  for ( i = 0; i<20; ++i )
  {
    void *ptr;
    Symbol symbol;
    char name[128];
    
    sprintf(name, "f%d", i);
    symbol = SymbolTable_makeSymbol(symbolTable, name);
    fprintf(stderr, "Get pointer for function: %s\n", Symbol_name(symbol));
    ptr = CodeBuffer_getPointer(codeBuffer, symbol, sizes[i]);
    fprintf(stderr, "   address: %p\n   size: %u\n", ptr, sizes[i]);
  }

  CodeBuffer_delete(codeBuffer);
  SymbolTable_delete(symbolTable);
  
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#if XCC__C
static void
CodeBuffer_TEST(void)
{
#include "JIT/CodeBuffer_.c"
}

int
main(int argc, char **argv)
{
  JIT_Test(CodeBuffer_TEST);
  return 0;
}
#endif
