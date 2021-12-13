#ifdef $XCC_h
/*
 * !!!! Interpreter.xcc
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

#include "JIT/JIT_.h"

#ifdef $XCC__h
#include "MDT/MDT_.h"
#include "JIT/Interpreter.h"
#endif//$XCC__h

#ifdef $XCC_h
// #include "JIT/SSARegs.h"
// #include "JIT/k1/k1.h"
#endif//$XCC_h

#if 0
#ifdef __K1__
#include "HAL/hal/hal.h"
#endif//__K1__
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

#define HELPER(routine) Behavior_##routine

#define DECLARE(declare) static inline declare
#define BehaviorDeclare(ID, DECLARE) DECLARE
#include "Behavior.tuple"
#undef BehaviorDeclare
#undef DECLARE

#define Behavior_FETCH
#include "Behavior.tuple"
#undef Behavior_FETCH

#ifdef $XCC_h
extern const Behavior Opcode_FetchBehavior[];
#endif//$XCC_h

const Behavior
Opcode_FetchBehavior[] = {
#define FETCH(ID)  ID
#define Behavior(OPCODE,FETCH,EXECUTE,COMMIT) \
        FETCH,
  FETCH(0),
#include "Behavior.tuple"
#undef Behavior
#undef FETCH
};

#define Behavior_EXECUTE
#include "Behavior.tuple"
#undef Behavior_EXECUTE

#ifdef $XCC_h
extern const Behavior Opcode_ExecBehavior[];
#endif//$XCC_h

const Behavior
Opcode_ExecBehavior[] = {
#define EXECUTE(ID)  ID
#define Behavior(OPCODE,FETCH,EXECUTE,COMMIT) \
        EXECUTE,
  EXECUTE(0),
#include "Behavior.tuple"
#undef Behavior
#undef EXECUTE
};

#define Behavior_COMMIT
#include "Behavior.tuple"
#undef Behavior_COMMIT

#ifdef $XCC_h
extern const Behavior Opcode_WbBehavior[];
#endif//$XCC_h

const Behavior
Opcode_WbBehavior[] = {
#define COMMIT(ID)  ID
#define Behavior(OPCODE,FETCH,EXECUTE,COMMIT) \
        COMMIT,
  COMMIT(0),
#include "Behavior.tuple"
#undef Behavior
#undef COMMIT
};


#define COMPILE_TRESHOLD 20

#ifdef $XCC_h
struct Oper_;
#endif//$XCC_h

#ifdef $XCC__h
struct Oper_ {
//   Operation         OPERATION;
  OperandDecoded    DECODED[Operands__MAXOPERANDS];
  uint64_t          OPERANDS[Operands__MAXOPERANDS];
//   bool              isDecoded;
};
#endif//$XCC__h

#ifdef $XCC_h
#define DEBUG_DUMP()  JIT_TRACE && fprintf(JIT_TRACE, \
  "\nPC: 0x%08x\nRA: 0x%08x\nSP: 0x%08x\n\n", \
  (unsigned int)(PC(this)), (unsigned int)(RA(this)), (unsigned int)(SP(this)))
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Structure to keep needed data
 */
struct Interpreter_;
#endif//$XCC_h

#ifdef $XCC__h
struct Interpreter_ {
  //@args Memory memory, void *ee, Program program, Convention convention,
  //@args int argc, char **argv, uint32_t *stack, uint32_t *datamem, reg_t *regset
  Memory            MEMORY;               //!< Where to allocate objects
  void*             EE;                   //!< Pointer to a parent, i.e. the EE
  reg_t             *REGSET;              //!< A set of registers for interpretation
  reg_t             *PC;                  //!< Program counter reg according to Conventions
  reg_t             *RA;                  //!< Return address reg according to Conventions
  reg_t             *SP;                  //!< stack pointer
  reg_t             NPC;              //!< Next PC for behavior use
  Oper_             OPER[1];     //!< Operations buffer
  //@access DECODED (Oper_DECODED(Interpreter_OPER(this)))
  //@access OPERANDS (Oper_OPERANDS(Interpreter_OPER(this)))
  Program           PROGRAM;          //!< Program being interpreted
  uint32_t           *STACK;          //!< Procedure stack
  uint32_t           *DATAMEM;             //!< data memory
//   SSARegs_          VIRTREGS[1];           //!< Virtual SSA registers
  BasicBlock        ORIGIN;                //!< Origin basic block
  PtrSeq_ CONTEXTS[1];  //!< Hold the context for (SSA)function calls
};
#endif//$XCC__h

#ifdef $XCC_h
#define SRF_BASE  64
#define K1_GRF(id) (*(Interpreter_REGSET(inter)+id))
#define K1_SRF(id) (*(Interpreter_REGSET(inter)+id+SRF_BASE))
//#define K1_REG(id) (Interpreter_REGSET(inter)[id])
#define K1_NPC (Interpreter_NPC(inter))
#define K1_PC      (*Interpreter_PC(inter))
#define K1_RA      (*Interpreter_RA(inter))
#define PC(this)   (*Interpreter_PC(this))
#define RA(this)   (*Interpreter_RA(this))
#define SP(this)     (*Interpreter_SP(this))
#define GRF(id)   (Interpreter_REGSET(this)+id)
#define SRF(id)   (Interpreter_REGSET(this)+id+SRF_BASE)
#define NEXTPC(this)   (Interpreter_NPC(this))
#endif//$XCC_h

Interpreter
Interpreter_Ctor(Interpreter this, Memory memory, void *ee, Program program,
                 Convention convention, int argc, char **argv,
                 uint32_t *stack, uint32_t *datamem, reg_t *regset)
{
  // get some control registers from conventions
  const_RegisterSet pc_reg =
    Convention_programRegisterSet(convention);
  const_RegisterSet ra_reg =
    Convention_returnRegisterSet(convention);
  const_RegisterSet sp_reg =
    Convention_stackRegisterSet(convention);
  Register ra = RegisterSet_first(ra_reg);
  Register pc = RegisterSet_first(pc_reg);
  Register sp = RegisterSet_first(sp_reg);
  Processor processor = Program_processor(program);
  unsigned ra_index = ra-RegFile_lowRegister(Register_regFile(ra,processor));
  unsigned pc_index = pc-RegFile_lowRegister(Register_regFile(pc,processor));
  unsigned sp_index = sp-RegFile_lowRegister(Register_regFile(sp,processor));

  *Interpreter__PROGRAM(this) = program;
  *Interpreter__MEMORY(this) = memory;
  *Interpreter__EE(this) = ee;

  Interpreter_REGSET(this) = regset;
//     Memory_alloc(memory, sizeof(reg_t)*Register__Count);

//   SSARegs_Ctor(Interpreter_VIRTREGS(this), memory, 1024);
  PtrSeq_Ctor(Interpreter_CONTEXTS(this), memory, 10);

//   Create a new context and push it on the context stack
  {
    Context context = Context_new(memory);
    PtrSeq_push2(Interpreter_CONTEXTS(this), context);
  }

  Interpreter_STACK(this) = stack;
  Interpreter_DATAMEM(this) = datamem;
  Interpreter_ORIGIN(this) = NULL;

  Interpreter_PC(this) = SRF(pc_index);
  Interpreter_RA(this) = SRF(ra_index);
  Interpreter_SP(this) = GRF(sp_index);

//   for(int i = 0; i<Register__Count; ++i) Interpreter_REGSET(this)[i] = 0;

  SP(this) = ExecutionEngine_getSP(ee);
  JIT_TRACE && fprintf(JIT_TRACE,  "SP: 0x%08llx\n", (long long)SP(this));




  JIT_TRACE && fprintf(JIT_TRACE, "Looking for entry point\n");
  {
    Procedure procedure = Program_findProcedure(program, "main");
    CodeRegion coderegion = Procedure_firstRegion(procedure);
    BasicBlock basicblock = CodeRegion_firstBlock(coderegion);
    Operation operation = BasicBlock_firstOperation(basicblock);
    Except_THROWIF(operation==NULL, "Operation is null!");
#ifdef __K1__
    PC(this) = (reg_t)operation;
#endif//__K1__
    JIT_TRACE && fprintf(JIT_TRACE, "First operation:\n");
    JIT_TRACE && Operation_pretty(operation, JIT_TRACE);
    JIT_TRACE && fprintf(JIT_TRACE, "\n");
  }
// set the stack pointer -- the end of stack - scratch area for main function
//   SP(this) = Interpreter_setupargs(Interpreter_STACK(this)+STACK_SIZE-1,
//                                    Interpreter_REGSET(this), argc, argv);

  DEBUG_DUMP();

  JIT_TRACE && fprintf(JIT_TRACE, "sizeOf(Interpreter_):0x%x (%u) bytes\n",
                       (unsigned int)Interpreter_Size(memory, ee, program, convention,
                                                      argc, argv, stack, datamem, regset),
                       (unsigned int)Interpreter_Size(memory, ee, program, convention,
                                                      argc, argv, stack, datamem, regset));
  JIT_TRACE && fprintf(JIT_TRACE, "data memory size: 0x%x (%u) bytes\n",
                       (unsigned int)DataList_dataSize(Program_dataList(program)),
                       (unsigned int)DataList_dataSize(Program_dataList(program)));

  JIT_TRACE && Interpreter_dumpRegset(this, JIT_TRACE);

  JIT_TRACE && fprintf(JIT_TRACE, "*********************************\n"
                                  "Virtual registers:\n");
  JIT_TRACE && Context_prettySSAValues(PtrSeq_lastItem(Interpreter_CONTEXTS(this)), JIT_TRACE);
//   JIT_TRACE && SSARegs_pretty(Interpreter_VIRTREGS(this), JIT_TRACE);
  JIT_TRACE && fprintf(JIT_TRACE, "\n*********************************\n\n");
  return this;
}

void
Interpreter_Dtor(Interpreter this)
{
//   SSARegs_Dtor(Interpreter_VIRTREGS(this));
  PtrSeq_Dtor(Interpreter_CONTEXTS(this));
}

size_t
Interpreter_Size(Memory memory, void *ee, Program program,
                 Convention convention, int argc, char **argv,
                 uint32_t *stack, uint32_t *datamem, reg_t *regset)
{
  return sizeof(Interpreter_);
//     +sizeof(reg_t)*Register__Count;
}

#ifdef $XCC_h
Interpreter
Interpreter_new(Memory parent, void *ee, Program program,
                Convention convention, int argc, char **argv,
                uint32_t *stack, uint32_t *datamem, reg_t *regset);
#endif//$XCC_h

Interpreter
Interpreter_new(Memory parent, void *ee, Program program,
                Convention convention, int argc, char **argv,
                uint32_t *stack, uint32_t *datamem, reg_t *regset)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Interpreter_Size(memory, ee, program, convention, argc, argv, stack, datamem, regset);
  Interpreter this = Memory_alloc(memory, size);
  return Interpreter_Ctor(this, memory, ee, program, convention, argc, argv, stack, datamem, regset);
}

#ifdef $XCC_h
Interpreter
Interpreter_delete(Interpreter this);
#endif//$XCC_h

Interpreter
Interpreter_delete(Interpreter this)
{
  if (this != NULL) {
    Memory memory = Interpreter_MEMORY(this);
#ifndef _NDTOR
    Interpreter_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}


#ifdef $XCC_h
/**
 * Get decoded operands
 */
OperandDecoded*
Interpreter_getOperandDecoded(Interpreter this, Operation operation, Processor processor);
#endif//$XCC_h

OperandDecoded*
Interpreter_getOperandDecoded(Interpreter this, Operation operation, Processor processor)
{
  OperandDecoded *decoded = Interpreter_DECODED(this);
  Operator operator = Operation_OPERATOR(operation);
  const_OperatorParameterArray parameterArray = Operator_parameterArray(operator);
  const OperatorParameter_ *writeParameters =
      OperatorParameterArray_writeParameters(parameterArray);
  const OperatorParameter_ *readParameters =
      OperatorParameterArray_readParameters(parameterArray);
  int readCount = OperatorParameterArray_readCount(parameterArray);
  int writeCount = OperatorParameterArray_writeCount(parameterArray);
  Temporary *arguments = Operation_ARGUMENTS(operation);
  Temporary *results = Operation_RESULTS(operation);
  int argCount = Operation_argCount(operation);
  int resCount = Operation_resCount(operation);


  Operation_RESULTS_FOREACH_Temporary(operation, temporary) {
    const_OperatorParameter parameter = writeParameters;
    int rank = OperatorParameter_rank(parameter), index = rank - 1;
    if (--writeCount < 0) break;
    if (rank > 0) {
      if (Temporary_hasRegister(temporary)) {
        decoded[index] = Temporary_REGISTER(temporary);
      } else if (Temporary_isVariable(temporary)){
        Context context = PtrSeq_lastItem(Interpreter_CONTEXTS(this));
        decoded[index] = (OperandDecoded)Context_insertSSA(context, temporary);
      } else Except_NEVER(true);
    }
    ++writeParameters;
    ++results, --resCount;
  } Operation_RESULTS_ENDEACH_Temporary;
  Operation_ARGUMENTS_FOREACH_Temporary(operation, temporary) {
    const_OperatorParameter parameter = readParameters;
    int rank = OperatorParameter_rank(parameter), index = rank - 1;
    if (--readCount < 0) break;
    if (rank > 0) {
      OperandType operandType = OperatorParameter_type(parameter);
      if (Temporary_hasRegister(temporary)) {
        decoded[index] = Temporary_REGISTER(temporary);
      } else if (operandType == OperandType_Immediate) {
        Literal literal = Temporary_LITERAL(temporary);
        if (Temporary_isAbsolute(temporary)) {
          ImmediateValue value = Literal_value(literal);
          decoded[index] = (OperandDecoded)value;
        } else if (Temporary_isSymbol(temporary)) {
          Symbol symbol = Literal_symbol(literal);
          ImmediateValue value = Symbol_value(symbol);
          if (Symbol_class(symbol) == SymbolClass_FUNC)
          {
            Procedure procedure = Symbol_PROCEDURE(symbol);
            CodeRegion coderegion = Procedure_firstRegion(procedure);
            BasicBlock basicblock = CodeRegion_firstBlock(coderegion);
#ifdef __K1__
            value = (uint32_t)BasicBlock_firstOperation(basicblock) - PC(this);
#endif//__K1__
          }
          decoded[index] = (OperandDecoded)value;
        } else if (Temporary_isLabel(temporary)) {
          BasicBlock basicblock = Label_block(Literal_label(literal));
          Operation operation = BasicBlock_firstOperation(basicblock);
#ifdef __K1__
          ImmediateValue value = (uint32_t)operation-PC(this);
          decoded[index] = (OperandDecoded)value;
#endif//__K1__
        } else Except_NEVER(true);
      } else if (Temporary_isVariable(temporary)) {
        Context context = PtrSeq_lastItem(Interpreter_CONTEXTS(this));
        decoded[index] = (OperandDecoded)Context_insertSSA(context, temporary);
      } else Except_NEVER(true);
    }
    ++readParameters;
    ++arguments, --argCount;
  } Operation_ARGUMENTS_ENDEACH_Temporary;

  {
    int index = 0;
    OperatorModify_ modify_ = Operator_modify_(operator, index++);
    if (modify_.RANK) {
      do {
         decoded[modify_.RANK-1] = (OperandDecoded)modify_.MEMBER;
         modify_ = Operator_modify_(operator, index++);
      } while (modify_.RANK);
    }
  }
  return decoded;
}


/**
 * Find first operation of a fallthru basic block
 * in case a basic block does not end with a terminator
 */
static inline Operation
Interpreter_doFallThru(Interpreter this, Operation operation)
{
  BasicBlock origin = Operation_basicBlock(operation);
  BasicBlock dest = BasicBlock_next(origin);
  Operation nextop = BasicBlock_firstOperation(dest);
  if (nextop == 0) dest = BasicBlock_next(dest);
  Except_THROWIF(nextop == 0, "Invalid program, cannot find operations in basic block");
  return nextop;
}

static inline Operation
Interpreter_processPHI(Interpreter this, Operation operation, Operation nextop)
{
  BasicBlock origin = Operation_basicBlock(operation);
  BasicBlock dest = Operation_basicBlock(nextop);
  reg_t *temps = NULL;
  int32_t phiCount = BasicBlock_phiCount(dest);
  int32_t index = BasicBlock_getPHIArgIndex(dest, origin)+1;

  JIT_TRACE && fprintf(JIT_TRACE, "EnterCount: %u\n", (unsigned)BasicBlock_enterCount(dest));
  if (phiCount > 0)
  {
    Context context = PtrSeq_lastItem(Interpreter_CONTEXTS(this));
    JIT_TRACE && fprintf(JIT_TRACE, "*********************************\n"
                                    "Processing %u PHI functions\n", (unsigned)phiCount);
//     ssaregs = Interpreter_VIRTREGS(this);
    temps = (reg_t*)alloca(phiCount*sizeof(reg_t));
    for (int i = 0; i < phiCount; ++i)
    {
      Except_THROWIF( Operation_operator(nextop) != Operator_PHI, "Expected PHI-function, invalid Tirex program!!");
      {
        Temporary temporary = Operation_OPERANDS(nextop)[index];
        Except_THROWIF( temporary == NULL, "Invalid edge number, verify the Tirex program!!");
        JIT_TRACE && fprintf(JIT_TRACE, "PHI function: ");
        JIT_TRACE && Operation_pretty(nextop, JIT_TRACE);
        JIT_TRACE && fprintf(JIT_TRACE, "selecting [%u] ", (unsigned)index);
        JIT_TRACE && Temporary_pretty(temporary, JIT_TRACE);
        JIT_TRACE && fprintf(JIT_TRACE, "\n");
        temps[i] = Context_getSSAValue(context, temporary);
      }
      nextop = Operation_next(nextop);
    }
    nextop = BasicBlock_firstOperation(dest);
    for (int i = 0; i< phiCount; ++i)
    {
      Temporary temporary = Operation_RESULTS(nextop)[0];
      JIT_TRACE && fprintf(JIT_TRACE, "copying value: 0x%08x to: ", (unsigned)temps[i]);
      JIT_TRACE && Temporary_pretty(temporary, JIT_TRACE);
      JIT_TRACE && fprintf(JIT_TRACE, "\n");
      Context_setSSAValue(context, temporary, temps[i]);
      nextop = Operation_next(nextop);
    }
  }
  return nextop;
}

#ifdef $XCC_h
/**
 * Interpret a given operation
 */
void
Interpreter_execute(Interpreter this, Processor processor);
#endif//$XCC_h

void
Interpreter_execute(Interpreter this, Processor processor)
{
#ifdef __K1__

  while( PC(this) != 0 )
  {
    Operation operation = (Operation)PC(this);
    OperandDecoded *decoded = NULL;
    Operator operator = Operation_OPERATOR(operation);
    Opcode opcode = Operator_opcode(operator, processor);

    JIT_TRACE && fprintf(JIT_TRACE, "%08x: ", (unsigned int)PC(this));
    JIT_TRACE && Operation_pretty(operation, JIT_TRACE);
    JIT_TRACE && fprintf(JIT_TRACE, "Operator: %s\n", Operator_Id(operator));
    JIT_TRACE && fprintf(JIT_TRACE, "Opcode: %s\n", Opcode_Id(opcode));

    NEXTPC(this) = (reg_t)Operation_next(operation);

    if (Operation_isRoutine(operation))
    {
      const Temporary *arguments = Operation_ARGUMENTS(operation);
      Temporary call_temporary = arguments[0];
      Literal literal = Temporary_LITERAL(call_temporary);
      Symbol symbol = Literal_SYMBOL(literal);
      Procedure procedure = Symbol_PROCEDURE(symbol);

      if (procedure && Procedure_address(procedure) == 0 &&
        Procedure_getCounter(procedure) > ExecutionEngine_getTreshold(Interpreter_EE(this)))
      {
        JIT_TRACE && fprintf(JIT_TRACE, "   reached treshold, compiling procedure: %s\n",
                             Symbol_name(symbol));
        ExecutionEngine_jit(Interpreter_EE(this), procedure);
      }


      JIT_TRACE && fprintf(JIT_TRACE, "Operation is a routine...");
      if (Symbol_store(symbol) == SymbolStore_EXTERN ||
          (procedure && Procedure_address(procedure) != 0))
      {
        uintptr_t val = (uintptr_t)Symbol_value(symbol);
        JIT_TRACE && fprintf(JIT_TRACE, "== calling native code via trampoline\n");

//       calling JITed code
        if (Symbol_store(symbol) != SymbolStore_EXTERN)
          val = Procedure_address(procedure);

        trampoline(Interpreter_REGSET(this), (void*)SP(this), (void*)val);
//    PC==0 means there's no more operations in current basic block
//    so we fallthru to next one if exists
        if ((PC(this) = NEXTPC(this)) == 0)
        {
          Operation nextop = Interpreter_doFallThru(this, operation);
          PC(this) = (reg_t)Interpreter_processPHI(this, operation, nextop);
        }
        DEBUG_DUMP();
        JIT_TRACE && Context_prettySSAValues(PtrSeq_lastItem(Interpreter_CONTEXTS(this)), JIT_TRACE);
        JIT_TRACE && Interpreter_dumpRegset(this, JIT_TRACE);
        continue;
      }

      if (procedure)
        Procedure_increaseCounter(procedure);
      JIT_TRACE && fprintf(JIT_TRACE, "Tirex, creating new context and interpreting\n");
      // a call is last op in a basic block -- do a fallthru
      NEXTPC(this) = (reg_t)Interpreter_processPHI(this, operation,
                                                  Interpreter_doFallThru(this, operation));
      {
        Context context = Context_new(Interpreter_MEMORY(this));
        PtrSeq_push2(Interpreter_CONTEXTS(this), context);
        JIT_TRACE && fprintf(JIT_TRACE, "Context count: %u\n",
                            (unsigned)PtrSeq_count(Interpreter_CONTEXTS(this)));
      }
    }

    if (Operation_isReturn(operation)) {
      Context context = PtrSeq_lastItem(Interpreter_CONTEXTS(this));
      Except_REQUIRE(context != NULL);
      Context_delete(context);
      PtrSeq_pop(Interpreter_CONTEXTS(this));
      NEXTPC(this) = RA(this);
      JIT_TRACE && fprintf(JIT_TRACE, "Context count: %u\n",
                          (unsigned)PtrSeq_count(Interpreter_CONTEXTS(this)));
    } else {
      decoded = Interpreter_getOperandDecoded(this, operation, processor);
      Opcode_behavior(opcode, BehaviorPhase_Fetch)(this, decoded, processor);
      Opcode_behavior(opcode, BehaviorPhase_Execute)(this, decoded, processor);
      Opcode_behavior(opcode, BehaviorPhase_Commit)(this, decoded, processor);
    }

//   NEXTPC==0 means there's no more instructions in this basic block
    if (!Operation_isReturn(operation)) {
      if (NEXTPC(this) == 0)
        NEXTPC(this) = (reg_t)Interpreter_doFallThru(this, operation);
      if (Operation_operator((Operation)NEXTPC(this)) == Operator_PHI)
        NEXTPC(this) = (reg_t)Interpreter_processPHI(this, operation, (Operation)NEXTPC(this));
    }

    Except_THROWIF( PC(this) == NEXTPC(this), "NPC equals PC!\n");
    PC(this) = NEXTPC(this);
    DEBUG_DUMP();
    if (!PtrSeq_isEmpty(Interpreter_CONTEXTS(this)))
      JIT_TRACE && Context_prettySSAValues(PtrSeq_lastItem(Interpreter_CONTEXTS(this)), JIT_TRACE);
  //   JIT_TRACE && SSARegs_pretty(Interpreter_VIRTREGS(this), JIT_TRACE);
    JIT_TRACE && Interpreter_dumpRegset(this, JIT_TRACE);
  }
#else
  fprintf(stderr, "Interpreter cannot be run on this platform\n");
#endif//__K1__
}

#ifdef $XCC_h
/**
 * Dump the regset
 */
bool
Interpreter_dumpRegset(Interpreter this, FILE *file);
#endif//$XCC_h

bool
Interpreter_dumpRegset(Interpreter this, FILE *file)
{
//   uint8_t *ptr = (uint8_t*)SP(this);
  if (file==NULL) file = stderr;
  fprintf(file,"Regset:\n");
  for (int i=0; i < 64; ++i)
  {
    if (i%8 == 0)
      fprintf(file,"\n");
    fprintf(file, "%02d:%08x ", i, (unsigned int)Interpreter_REGSET(this)[i]);
  }
  fprintf(file,"\n\n");
//   fprintf(file, "Stack frame:\n");
//   for (int i=0; i<32; ++i)
//   {
//     if (i%8 == 0)
//       fprintf(file, "\n%08x:%02x(%02d): ", (unsigned int)(ptr+i), i, i);
//     fprintf(file, "%02x ", (unsigned int)(*(ptr+i)));
//   }
//   fprintf(file,"\n\n");
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  fprintf(stderr, "Testing Interpreter...\n");
}
#endif//$XCC__c

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Interpreter_)\t%zu\n", sizeof(Interpreter_));
  fprintf(stderr, "sizeof(int)\t%zu\n", sizeof(int));
  fprintf(stderr, "sizeof(long int)\t%zu\n", sizeof(long int));
  fprintf(stderr, "sizeof(void*)\t%zu\n", sizeof(void*));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#if XCC__C
static void
Interpreter_TEST(void)
{
#include "JIT/Interpreter_.c"
}

int
main(int argc, char **argv)
{
  JIT_Test(Interpreter_TEST);
  return 0;
}
#endif

