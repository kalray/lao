#ifdef $XCC_h
/*
 * !!!!	Compiler.xcc
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

#include "JIT/JIT_.h"

#ifdef $XCC__h
#include "JIT/Compiler.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Compile a given Procedure.
 */
size_t
Compiler_compileProcedure(Procedure procedure, CodeBuffer codeBuffer);
#endif//$XCC__h

extern void callback(void);

size_t
Compiler_compileProcedure(Procedure procedure, CodeBuffer codeBuffer)
{
  size_t codeSize = 0;
  Memory memory = Procedure_memory(procedure);
    
  JIT_TRACE && fprintf(JIT_TRACE, "\n********************************\n"
                       "Compiling procedure: %s\n",
                        Procedure_name(procedure));  
  
  Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
    Optimize optimize = CodeRegion_optimize(codeRegion);
    SSAForm ssaForm = CodeRegion_ssaForm(codeRegion);
    *Optimize__COALESCING(optimize) = OptimizeCoalescing_SeqCopies;
//     Spill_allGlobalEverywhereLocalBelady(codeRegion);
//     SSAForm_destruct(ssaForm);
//     CodeRegion_expandGenerics(codeRegion);
  } Procedure_ENDEACH_CodeRegion;
    
  {
    NativeStream nativeStream = NativeStream_new(Memory_Root, 10);    
//     Frame frame = Procedure_frame(procedure);
//     Frame_resolveSpillSymbols(frame);
//     Procedure_updateFrameSize(procedure);

    Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
//       CodeRegion_expandSimulated(codeRegion);
      CodeRegion_addtoNativeStream(codeRegion, nativeStream);
//       JIT_TRACE && CodeRegion_pretty(codeRegion, JIT_TRACE);
    } Procedure_ENDEACH_CodeRegion;
  

    NativeStream_assumeUnscheduled(nativeStream);
    codeSize = NativeStream_getCodeSize(nativeStream, 0);

    Symbol symbol = Procedure_symbol(procedure);
    
//     get the buffer for the code
    void *buffer = CodeBuffer_getPointer(codeBuffer, symbol, codeSize);
    Procedure_setAddress(procedure, (uintptr_t)buffer);
    
//     encode the code
    NativeStream_encode(nativeStream, (uintptr_t)buffer, buffer);

        
    NativeStream_FOREACH_NativeBlock(nativeStream, nativeBlock)
    {
      NativeBlock_FOREACH_InstructionGroup(nativeBlock, group)
      {
        Symbol callSymbol = NULL;
        int count = InstructionGroup_COUNT(group);
        Instruction instruction = InstructionGroup_INSTRUCTION(group);
        
//      find the addres of next bundle
        uintptr_t ra = Instruction_ADDRESS(instruction) + 
                       Template_increment(InstructionGroup_TEMPLATE(group));
        for (; count>0; --count, ++instruction)
        {
          Literal literal = Instruction_LITERAL(instruction);
          if ( literal == NULL || !Instruction_isControl(instruction)) continue;
          if ( Literal_isSymbol(literal) &&
               Symbol_store(Literal_symbol(literal)) != SymbolStore_EXTERN)
          {
            uintptr_t cb = 0;//BD3 (uintptr_t)&callback;
            callSymbol = Literal_symbol(literal);
            JIT_TRACE && fprintf(JIT_TRACE, "== callback: 0x%08zx\n"
                            "== symbol (%s): 0x%08llx\n", 
                    cb, 
                    Symbol_name(callSymbol), (long long)Symbol_value(callSymbol));
            if (Symbol_value(callSymbol) == 0) {
              JIT_TRACE && fprintf(JIT_TRACE, "== setting symbol value to callback\n");
              Symbol_setValue(callSymbol, cb);
              Instruction_encode(instruction, (void*)Instruction_ADDRESS(instruction));
            }
//             CodeBuffer_addUser(codeBuffer, callSymbol, instruction);
//             CodeBuffer_addReference(codeBuffer, symbol, instruction);
            break;
          }
        }
                
        if (callSymbol) {
          CodeBuffer_insertCallSite(codeBuffer, ra, callSymbol);
          JIT_TRACE && fprintf(JIT_TRACE, "== inserting call site(%s): 0x%08zx\n", 
                               Symbol_name(callSymbol), ra);
        }
        
      } NativeBlock_ENDEACH_InstructionGroup;
    } NativeStream_ENDEACH_NativeBlock;
        
 
    
#ifdef __K1__
    __k1_mb();
#else
    JIT_TRACE && fprintf(JIT_TRACE, "Not on K1 platform, don't need memory barrier.\n");
#endif
        
    JIT_TRACE && fprintf(JIT_TRACE,"Code size: %zu bytes\n"
                                   "Address: %p\n", 
                                   codeSize, codeBuffer);
    JIT_TRACE && NativeStream_pretty(nativeStream, JIT_TRACE);
//     NativeStream_delete(nativeStream);
  }
  
//   Compiler_patchSymbol(codeBuffer, Procedure_symbol(procedure));
  return codeSize;
}

// #ifdef $XCC_h
// void
// Compiler_patchSymbol(CodeBuffer codeBuffer, Symbol symbol);
// #endif//$XCC_h
// 
// void
// Compiler_patchSymbol(CodeBuffer codeBuffer, Symbol symbol)
// {
//   const_PtrSet users = CodeBuffer_getUsers(codeBuffer, symbol);
//   if (users == NULL) return;
//   PtrSet_FOREACH(users, Instruction, instruction)
//   {
//     JIT_TRACE && fprintf(JIT_TRACE, "== Patching symbol: %s\n  old: ", Symbol_name(symbol));
//     JIT_TRACE && Instruction_asmPrint(instruction, JIT_TRACE, NULL);
//     JIT_TRACE && fprintf(JIT_TRACE, "\n");               
//    
//     uintptr_t address = Instruction_ADDRESS(instruction);
//     Instruction_encode(instruction, (void*)address);
//     
//     JIT_TRACE && fprintf(JIT_TRACE, "  new: ");
//     JIT_TRACE && Instruction_asmPrint(instruction, JIT_TRACE, NULL);
//     JIT_TRACE && fprintf(JIT_TRACE, "\n");
//   }PtrSet_ENDEACH;
// }

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Compiler module.
 */
#define Compiler_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Compiler module.
 */
#define Compiler_FINI()
#endif//$XCC__h

#if XCC__C
static void
Compiler_TEST(void)
{
#include "JIT/Compiler_.c"
}

int
main(int argc, char **argv)
{
  JIT_Test(Compiler_TEST);
  return 0;
}
#endif

