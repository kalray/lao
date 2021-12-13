#ifdef $XCC_h
/*
 * !!!! ExecutionEngine.xcc
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
 * @brief Execution engine --- controls the interpretation/compilation
 */
#endif//$XCC_h

#include "JIT/JIT_.h"
#include "MDT/MDT_.h"

#ifdef $XCC__h
#include "JIT/ExecutionEngine.h"
#endif//$XCC__h

#ifdef $XCC_h
#define STACK_SIZE  1024
reg_t
setupargs(void *sp, reg_t *regSet, int argc, char **argv, int regCount);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Structure to control the execution
 */
struct ExecutionEngine_;
#endif//$XCC_h

#ifdef $XCC__h
struct ExecutionEngine_ {
  //@args Memory memory, Program program, bool jit, int argc, char **argv
  Memory            MEMORY;               //!< Where to allocate objects
  bool              JIT;                  //!< JIT-only mode
  Interpreter       INTERPRETER;          //!< Our interpreter  
  uint32_t          TRESHOLD;             //!< JIT Treshold
  int               ARGC;                 //!< argc passed to main function
  char**            ARGV;                 //!< argv passed to main function
  Program           PROGRAM;
  //@access PROCESSOR Program_processor(ExecutionEngine_PROGRAM(this))
  uint32_t           *DATAMEM;            //!< data memory
  uint32_t           STACK[STACK_SIZE];   //!< Procedure stack
  reg_t             *REGSET; //!< Register set for interpretation
  reg_t             SP;                   //!< Stack pointer
  CodeBuffer        CODEBUFFER; //!<Where the code is kept
};
#endif//$XCC__h

#ifdef $XCC__h
static CodeBuffer theCodeBuffer = NULL;
extern uintptr_t
getProcedure(uintptr_t ra);
#endif//$XCC__h

uintptr_t
getProcedure(uintptr_t ra)
{
  JIT_TRACE && fprintf(JIT_TRACE, "== CallSite lookup ra: 0x%08zx\n", ra);
  Symbol symbol = CodeBuffer_getCallSite(theCodeBuffer, ra);
// TODO: if symbol not found, it means something terribly wrong happend
  if (!symbol) return 0;
  JIT_TRACE && fprintf(JIT_TRACE, "== found CallSite for symbol: %s\n"
                                  "   old value: 0x%08llx\n",
                       Symbol_name(symbol),
                       (long long)Symbol_value(symbol));
  {
    Procedure procedure = Symbol_PROCEDURE(symbol);
    if (Procedure_address(procedure) == 0)      
    {
      Compiler_compileProcedure(procedure, theCodeBuffer);
      JIT_TRACE && fprintf(JIT_TRACE, "   new value: 0x%08zx\n",
                           (uintptr_t)Procedure_address(procedure));
    }
    return (uintptr_t)Procedure_address(procedure);
  }
  return 0;
}

static inline void ExecutionEngine_addSymbol(Program program, const char *name, void* addr) 
{
  SymbolTable symbolTable = Program_symbolTable(program);
  Symbol symbol = SymbolTable_searchName(symbolTable, name);
  if (symbol == NULL) 
    symbol = SymbolTable_makeSymbol(symbolTable, name);
  
  Except_THROWIF(symbol == NULL, "Cannot find/create symbol for function %s!", name);
  
  Symbol_setClasses(symbol, SymbolClass_FUNC,
                    SymbolStore_EXTERN, SymbolExport__UNDEF);
  
  Symbol_setResolved(symbol);
  Symbol_setValue(symbol, ImmediateValue((uintptr_t)addr));
}


ExecutionEngine
ExecutionEngine_Ctor(ExecutionEngine this, 
                     Memory memory, 
                     Program program,
                     bool jit,
                     int argc, char **argv)
{   
  JIT_TRACE && fprintf(JIT_TRACE, 
                       "*********************************\n"
                       "Creating the Execution Engine\n");  
  *ExecutionEngine__MEMORY(this) = memory;
  *ExecutionEngine__ARGC(this) = argc;
  *ExecutionEngine__ARGV(this) = argv;  
  *ExecutionEngine__PROGRAM(this) = program;
  *ExecutionEngine__JIT(this) = jit;
  *ExecutionEngine__TRESHOLD(this) = 20;
  ExecutionEngine_addSymbol(program, "printf", (void*)&printf);
  ExecutionEngine_addSymbol(program, "puts", (void*)&puts);
  
  *ExecutionEngine__REGSET(this) = 
    Memory_alloc(memory, sizeof(reg_t)*Register__Count);
  
  *ExecutionEngine__SP(this) = setupargs(ExecutionEngine_STACK(this)+STACK_SIZE-1,
                                         ExecutionEngine_REGSET(this), argc, argv, Register__Count);
  JIT_TRACE && fprintf(JIT_TRACE,  "SP: 0x%08llx\n", (long long)ExecutionEngine_SP(this));
  
  if ( Program_processor(program) == Processor__UNDEF ) {
    JIT_TRACE && fprintf(JIT_TRACE, "No processor given, falling to: %s\n",
                         Processor_Id(1));
  }
  JIT_TRACE && fprintf(JIT_TRACE, "Processor: %s\n",
                       Processor_Id(Program_processor(program)));
  

  
//***********************************************************
// Expand generics if needed
//***********************************************************
//   JIT_TRACE && fprintf(JIT_TRACE, "Expanding generics\n");
//   Program_FOREACH_Procedure(program, procedure) {
//     Procedure_FOREACH_CodeRegion(procedure, coderegion) {
//       CodeRegion_expandGenerics(coderegion);
//     } Procedure_ENDEACH_CodeRegion;
//   } Program_ENDEACH_Procedure;   
  
  
//***********************************************************
// Initialize the data in a memory buffer
// set the symbol value to point to a beginning of each entry
//***********************************************************  
  {
    DataList dlist = Program_dataList(program);
    uint8_t *ptr = NULL;
    size_t i = 0;
    ExecutionEngine_DATAMEM(this) = 
      Memory_alloc(memory, sizeof(uint8_t)*DataList_dataSize(dlist));
      
    ptr = (uint8_t*)ExecutionEngine_DATAMEM(this);
    
    JIT_TRACE && fprintf(JIT_TRACE, 
                         "*********************************\n"
                         "Initializing data:\n");
    
    JIT_TRACE && DataList_pretty(Program_dataList(program), JIT_TRACE);
    
    DataList_FOREACH_Data(dlist, data) {
      Symbol sym = Data_SYMBOL(data);
      Symbol_setValue(sym, (uintptr_t)(ptr+i));
      JIT_TRACE && fprintf(JIT_TRACE, "setting symbol %s->0x%08x\n", 
                           Symbol_name(sym), (unsigned)Symbol_value(sym));
      if (Data_hasInitializer(data)) {
        DataInitializerList_FOREACH_Initializer(Data_DATAINIT(data), init) {
          size_t size = DataInitializer_SIZE(init);
          JIT_TRACE && fprintf(JIT_TRACE, "offset: 0x%04x, size: %d bytes\n",
                               (int)i, (int)size);
          memcpy(ptr+i, DataInitializer_INIT(init), size);
          i += size;
        } DataInitializerList_ENDEACH_Initializer;
      } else {
        size_t size = Data_DATASIZE(data);
        JIT_TRACE && fprintf(JIT_TRACE, 
                             "offset: 0x%04x, size: %d bytes zeroinit\n",
                             (int)i, (int)size);
        memset(ptr+i, 0, size);
        i+=size;
      }
    } DataList_ENDEACH_Data;
    JIT_TRACE && fprintf(JIT_TRACE, "Total size: %d bytes\n\n", (int)i);
  }
  


  ExecutionEngine_INTERPRETER(this) = Interpreter_new(memory, this, program, 
                                      argc, argv, (Convention)1, /*FIXME*/
                                      ExecutionEngine_STACK(this),
                                      ExecutionEngine_DATAMEM(this),
                                      ExecutionEngine_REGSET(this));    

  //   Create the code buffer
  theCodeBuffer = ExecutionEngine_CODEBUFFER(this) = CodeBuffer_new(memory);
  
  if (jit) {
    Procedure procedure = Program_findProcedure(program, "main");      
    Except_THROWIF(procedure == NULL, "Cannot find entry");  
    Compiler_compileProcedure(procedure, ExecutionEngine_CODEBUFFER(this));    
  }    
  return this;
}

void
ExecutionEngine_Dtor(ExecutionEngine this)
{
  if (ExecutionEngine_INTERPRETER(this) != NULL)
    Interpreter_delete(ExecutionEngine_INTERPRETER(this));
  CodeBuffer_delete(ExecutionEngine_CODEBUFFER(this));
}

size_t
ExecutionEngine_Size(Memory memory, Program program, bool jit,
                     int argc, char **argv)
{
  return sizeof(ExecutionEngine_);
        +sizeof(reg_t)*Register__Count
        +sizeof(uint8_t)*DataList_dataSize(Program_dataList(program));
}

#ifdef $XCC_h
ExecutionEngine
ExecutionEngine_new(Memory parent, Program program, bool jit,
                    int argc, char **argv);
#endif//$XCC_h

ExecutionEngine
ExecutionEngine_new(Memory parent, Program program, bool jit,
                    int argc, char **argv)
{  
  Memory memory = Memory_new(parent, true);
  {
  size_t size = ExecutionEngine_Size(memory, program, jit, argc, argv);  
  ExecutionEngine this = Memory_alloc(memory, size);
  return ExecutionEngine_Ctor(this, memory, program, jit, argc, argv);
  }
}

#ifdef $XCC_h
ExecutionEngine
ExecutionEngine_delete(ExecutionEngine this);
#endif//$XCC_h

ExecutionEngine
ExecutionEngine_delete(ExecutionEngine this)
{
  if (this != NULL) {
    Memory memory = ExecutionEngine_MEMORY(this);
#ifndef _NDTOR
    ExecutionEngine_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Execute the program
 */
int
ExecutionEngine_run(ExecutionEngine this);
#endif//$XCC_h


int
ExecutionEngine_run(ExecutionEngine this)
{
  int res = 0; // result      
  //If JIT-only mode
  if (ExecutionEngine_JIT(this))  
  {
    // Find entry point and compile
    Procedure procedure = Program_findProcedure(ExecutionEngine_PROGRAM(this), "main");
    uintptr_t address = Procedure_address(procedure);
    if (address != 0)
    {
      JIT_TRACE && fprintf(JIT_TRACE, "== found main function: 0x%08zx\n", address);
      JIT_TRACE && fprintf(JIT_TRACE, "== executing native code\n");
      trampoline(ExecutionEngine_REGSET(this), (void*)ExecutionEngine_SP(this),
                    (void*)address);
      JIT_TRACE && fprintf(JIT_TRACE, "== finished executing native code\n");
      JIT_TRACE && fprintf(JIT_TRACE, "\nResult: %d\n", res);
      return (int)ExecutionEngine_REGSET(this)[0];
    }
  }
  
  //otherwise interpret
  {
    Processor processor = ExecutionEngine_PROCESSOR(this);  
    Interpreter inter = ExecutionEngine_INTERPRETER(this);    
    Interpreter_execute(inter, processor);    
    res = (int)ExecutionEngine_REGSET(this)[0];
  }
  JIT_TRACE && fprintf(JIT_TRACE, "\nResult: %d\n", res);  
  return res;
}

#ifdef $XCC_h
/**
 * Execute the program in step mode
 */
int
ExecutionEngine_stepRun(ExecutionEngine this);
#endif//$XCC_h


int
ExecutionEngine_stepRun(ExecutionEngine this)
{
  return 0;
//   int res = 0; // result
//   Processor processor = ExecutionEngine_PROCESSOR(this);
//   Interpreter inter = ExecutionEngine_INTERPRETER(this);
// //   while ( PC(inter) < opcount ) {
//   while( PC(inter) != 0) {
// //     Interpreter_execute(inter, Interpreter_OPERATION(inter), 
// //                         processor);  
//     Interpreter_execute(inter, (Operation)PC(inter), processor);
//     fprintf(stderr, "Press enter to continue...");
//     fflush(stdout);
//     getchar();
//   }
//   res = (uint32_t)Interpreter_REGSET(inter)[0];
//   
// //   JIT_TRACE && SSARegs_pretty(Interpreter_VIRTREGS(inter), JIT_TRACE);
//   JIT_TRACE && fprintf(JIT_TRACE, "Result: %d\n", res);  
//   return res;
}


#ifdef $XCC_h
void
ExecutionEngine_setTreshold(ExecutionEngine this, uint32_t treshold);
#endif//$XCC_h

void
ExecutionEngine_setTreshold(ExecutionEngine this, uint32_t treshold)
{
  *ExecutionEngine__TRESHOLD(this) = treshold;
}


#ifdef $XCC_h
uint32_t
ExecutionEngine_getTreshold(ExecutionEngine this);
#endif//$XCC_h

uint32_t
ExecutionEngine_getTreshold(ExecutionEngine this)
{
  return *ExecutionEngine__TRESHOLD(this);
}



#ifdef $XCC_h
reg_t
ExecutionEngine_getSP(ExecutionEngine this);
#endif//$XCC_h

reg_t
ExecutionEngine_getSP(ExecutionEngine this)
{
  return ExecutionEngine_SP(this);
}


#ifdef $XCC_h
/**
 * Dump the symbol table
 */
void
ExecutionEngine_dumpSymbolTable(ExecutionEngine this, FILE *file);
#endif//$XCC_h

void
ExecutionEngine_dumpSymbolTable(ExecutionEngine this, FILE *file) {  
  SymbolTable symbolTable = Program_symbolTable(ExecutionEngine_PROGRAM(this));
  fprintf(file, "*********************************\n"
                "Symbol Table:\n");
  SymbolTable_FOREACH_Symbol(symbolTable, symbol) {
    Symbol_pretty(symbol, file);
  } SymbolTable_ENDEACH_Symbol;
  fprintf(file, "\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * JIT a procedure
 */
void
ExecutionEngine_jit(ExecutionEngine this, Procedure procedure);
#endif//$XCC_h

void
ExecutionEngine_jit(ExecutionEngine this, Procedure procedure)
{
  Compiler_compileProcedure(procedure, ExecutionEngine_CODEBUFFER(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  fprintf(stderr, "Testing ExecutionEngine...\n");
}
#endif//$XCC__c

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ExecutionEngine_)\t%zu\n", sizeof(ExecutionEngine_));
  fprintf(stderr, "sizeof(int)\t%zu\n", sizeof(int));
  fprintf(stderr, "sizeof(long int)\t%zu\n", sizeof(long int));
  fprintf(stderr, "sizeof(void*)\t%zu\n", sizeof(void*));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#if XCC__C
static void
ExecutionEngine_TEST(void)
{
#include "JIT/ExecutionEngine_.c"
}

int
main(int argc, char **argv)
{
  JIT_Test(ExecutionEngine_TEST);
  return 0;
}
#endif
