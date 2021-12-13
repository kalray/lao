#ifdef $XCC_h
/*
 * !!!! Context.xcc
 *
 * Artur Pietrek (artur.pietrek@kalray.eu)
 *
 * Copyright 2011 Kalray
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
 * @brief Context for a function scope
 */
#endif//$XCC_h

#include "JIT/JIT_.h"
#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "JIT/Context.h"
#endif//$XCC__h


#ifdef $XCC_h
/**
 * A Context
 * 
 */
struct Context_;
#endif//$XCC_h

#ifdef $XCC__h
struct Context_ {
  //@args Memory memory
  Memory  MEMORY;               //!< Where to allocate objects
  HTable_ SSAVARIABLES[1];      //!< Values of the SSA variables
};
#endif//$XCC__h

Context
Context_Ctor(Context this,
             Memory memory)
{
  *Context__MEMORY(this) = memory;
  HTable_Ctor(Context_SSAVARIABLES(this), memory, 512);
  return this;
}

void
Context_Dtor(Context this)
{
  HTable_Dtor(Context_SSAVARIABLES(this));
}

#ifdef $XCC_h
Context
Context_new(Memory parent);
#endif//$XCC_h

Context
Context_new(Memory parent)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Context_Size(memory);
  Context this = Memory_alloc(memory, size);
  return Context_Ctor(this, memory);
}

#ifdef $XCC_h
Context
Context_delete(Context this);
#endif//$XCC_h

Context
Context_delete(Context this)
{
  if (this != NULL) {
    Memory memory = Context_MEMORY(this);
#ifndef _NDTOR
    Context_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

size_t
Context_Size(Memory memory)
{
  return sizeof(Context_);
}

#ifdef $XCC__h
static inline uintptr_t
Context_insertSSA(Context this, Temporary temporary)
{
  HTableKey key = (HTableKey)temporary;
  HTable_insert(Context_SSAVARIABLES(this),
                key,
                sizeof(reg_t));
  return (uintptr_t)key;
}
#endif//$XCC__h


#ifdef $XCC__h
static inline reg_t
Context_getSSAValue(Context this, Temporary temporary)
{
  HTableKey key = (HTableKey)temporary;
  void* pointer = HTable_search(Context_SSAVARIABLES(this), key);
  if (pointer != NULL) return *(reg_t*)pointer;
  Except_THROW("Attempt to use a SSA Variable before it's construction");
  return 0;
}
#endif//$XCC__h


#ifdef $XCC__h
static inline void
Context_setSSAValue(Context this, Temporary temporary, reg_t value)
{  
  HTableKey key = (HTableKey)temporary;
  void* pointer = HTable_search(Context_SSAVARIABLES(this), key);
  if (pointer != NULL) {
    *(reg_t *)pointer = value;
    return;
  }
  Except_THROW("Attempt to use a SSA Variable before it's construction");
//   Context_setSSAValueByID(this, Temporary_identity(temporary), value);
}
#endif//$XCC__h

#ifdef $XCC_h
bool
Context_prettySSAValues(const_Context this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Context_prettySSAValues(const_Context this, FILE *file)
{
  int i = 0;
  if (file == NULL) file = stderr;
  fprintf(file, "SSA regs {\n");  
  HTable_FOREACH(Context_SSAVARIABLES(this), key, reg_t*, iter) {
    Temporary temporary = (Temporary)key;
    fprintf(file, " V[%d]", (int)Temporary_identity(temporary));
    fprintf(file, "=0x%08llx\n",(long long)(*(reg_t*)iter));
  } HTable_ENDEACH;
  fprintf(file, " }\n");
  return true;
}
#endif//_NTRACE

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Context_)\t%zu\n", sizeof(Context_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#if XCC__C
static void
Context_TEST(void)
{
#include "JIT/Context_.c"
}

int
main(int argc, char **argv)
{
  JIT_Test(Context_TEST);
  return 0;
}
#endif

