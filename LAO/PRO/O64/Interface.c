#ifdef $XCC_h
/*
 * !!!!	Interface.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Francois de Ferriere (Francois.de-Ferriere@st.com).
 * Christophe Guillon (Christophe.Guillon@st.com).
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
 * @ingroup O64
 * @brief Interface with Open64.
 */
#endif//$XCC_h

// For getpid()
#include <sys/types.h>
#include <unistd.h>

#include "O64/O64_.h"

#ifdef $XCC__h
#include "O64/Interface.h"
#undef _OPTIMIZE
#define _OPTIMIZE 1
#endif//$XCC__h

#ifdef $XCC__h
//#define Interface_DEDICATED 0
#ifndef Interface_DEDICATED
extern int Interface_DEDICATED;
#endif
#endif//$XCC__h

#ifndef Interface_DEDICATED
int Interface_DEDICATED;
#endif

#ifdef $XCC__h
#define GETENVVALUE(str) (getenv(str) != NULL? atoi(getenv(str)): 0)
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef void *InterfaceCGIR;
typedef void *LIR2CGIR;
#endif//$XCC_h

#ifdef $XCC_h
/*
  InterfaceType -- Enumerate the Interface types in reverse mapping.
 */
typedef enum {
  InterfaceType_CGIR,
  InterfaceType_Label,
  InterfaceType_Symbol,
  InterfaceType_Temporary,
  InterfaceType_Operation,
  InterfaceType_BasicBlock,
  InterfaceType_LoopScope,
  InterfaceType_CGIRImport,
  InterfaceType_CGIRValid,
  InterfaceType__
} InterfaceType;
typedef uint8_t short_InterfaceType;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Mapped value in the LIR2CGIR HTables.
 */
struct InterfaceMapped_ {
  //@args	void *cgir, InterfaceType type
  InterfaceCGIR CGIR;		// The CGIR object mapped to.
  short_InterfaceType TYPE;	// The LIR key object type.
};
#endif//$XCC_h

InterfaceMapped
InterfaceMapped_Ctor(InterfaceMapped this,
                     void *cgir, InterfaceType type)
{
  *InterfaceMapped__CGIR(this) = cgir;
  *InterfaceMapped__TYPE(this) = type;
  return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Interface between the LAO LIR and a host compiler CGIR.
 *
 * The purpose of the interface is to maintain mappings between LIR Labels,
 * Symbols, Temporaries, Operations, BasicBlocks, LoopScopes, and the corresponding
 * host compiler CGIR (Code Generator Intermediate Representation) objects.
 *
 * There must not be more than one CGIR object mapped to the same LIR object
 * to ensure that all CGIR objects are correctly updated. This constraint
 * apply to all CGIR->LIR object tables.
 * However there may be more than one LIR object mapped to the same CGIR
 * object. So factorization of the objects in the CGIR side is allowed.
 * These duplicated LIR objects however should not be mutable as in this
 * case information from the LIR side may not be passed back to the CGIR
 * side. So the LIR->CGIR reversed tables may have several keys pointing
 * to the same CGIR object. In this case only one update is performed on
 * the CGIR object.
 */
struct Interface_;
#endif//$XCC_h

#ifdef $XCC__h
struct Interface_ {
  //@args	Memory memory
  Memory MEMORY;
  Program PROGRAM;
  Configure CONFIGURE;
  Selector_ SELECTOR_;
  //@access SELECTOR	Interface__SELECTOR_(this)
  //@access PROCEDURE	Selector_procedure(Interface_SELECTOR(this))
  HTable_ DIRECT[InterfaceType__];
  //@access LIR2CGIR	(Interface_DIRECT(this) + InterfaceType_CGIR)
  //@access LABELS	(Interface_DIRECT(this) + InterfaceType_Label)
  //@access SYMBOLS	(Interface_DIRECT(this) + InterfaceType_Symbol)
  //@access TEMPORARIES	(Interface_DIRECT(this) + InterfaceType_Temporary)
  //@access OPERATIONS	(Interface_DIRECT(this) + InterfaceType_Operation)
  //@access BASICBLOCKS	(Interface_DIRECT(this) + InterfaceType_BasicBlock)
  //@access LOOPSCOPES	(Interface_DIRECT(this) + InterfaceType_LoopScope)
  //@access CGIRIMPORT	(Interface_DIRECT(this) + InterfaceType_CGIRImport)
  //@access CGIRVALID	(Interface_DIRECT(this) + InterfaceType_CGIRValid)
  int REGISTERCOUNT;
  int *REGISTERARRAY;
  int LABELCOUNT;
  Label *LABELARRAY;
  int RESULTCOUNT;
  Temporary *RESULTARRAY;
  int ARGUMENTCOUNT;
  Temporary *ARGUMENTARRAY;
  int TEMPORARYCOUNT;
  Temporary *TEMPORARYARRAY;
  int OPERATIONCOUNT;
  Operation *OPERATIONARRAY;
};
#endif//$XCC__h

Interface
Interface_Ctor(Interface this,
               Memory memory)
{
  *Interface__MEMORY(this) = memory;
  *Interface__CONFIGURE(this) = NULL;
  memset(Interface_SELECTOR(this), 0, sizeof(Selector_));
  memset(Interface_DIRECT(this), 0, sizeof(HTable_)*InterfaceType__);
  *Interface__REGISTERCOUNT(this) = 0;
  *Interface__REGISTERARRAY(this) = NULL;
  *Interface__LABELCOUNT(this) = 0;
  *Interface__LABELARRAY(this) = NULL;
  *Interface__RESULTCOUNT(this) = 0;
  *Interface__RESULTARRAY(this) = NULL;
  *Interface__ARGUMENTCOUNT(this) = 0;
  *Interface__ARGUMENTARRAY(this) = NULL;
  *Interface__TEMPORARYCOUNT(this) = 0;
  *Interface__TEMPORARYARRAY(this) = NULL;
  *Interface__OPERATIONCOUNT(this) = 0;
  *Interface__OPERATIONARRAY(this) = NULL;
  return this;
}

void
Interface_Dtor(Interface this)
{
  Memory memory = Interface_MEMORY(this);
  Except_ENSURE(Interface_PROCEDURE(this) == NULL);
  Memory_free(memory, Interface_REGISTERARRAY(this));
  Memory_free(memory, Interface_LABELARRAY(this));
  Memory_free(memory, Interface_RESULTARRAY(this));
  Memory_free(memory, Interface_ARGUMENTARRAY(this));
  Memory_free(memory, Interface_TEMPORARYARRAY(this));
  Memory_free(memory, Interface_OPERATIONARRAY(this));
}

#ifdef $XCC_h
/**
 * Returns the singleton Interface, for use by the LAO clients.
 */
Interface
Interface_get(void);
#endif//$XCC_h

Interface
Interface_get(void)
{
  return interface;
}

#ifdef $XCC__h
/*
 * The singleton Interface object.
 */
extern Interface interface;
#endif//$XCC__h

static Interface_ interface_;
Interface interface = &interface_;

#ifdef $XCC__h
/**
 * Iterate over this Interface_LABELS.
 */
#define Interface_FOREACH_Label(this, cgirlab, label) \
  HTable_FOREACH(Interface_LABELS(this), Interface_KEY, Label, Interface_ITER) { \
    CGIRLAB cgirlab = (CGIRLAB)(uintptr_t)Interface_KEY; \
    Label label = *Interface_ITER;
#define Interface_ENDEACH_Label \
  } HTable_ENDEACH;
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Interface_SYMBOLS.
 */
#define Interface_FOREACH_Symbol(this, cgirsym, symbol) \
  HTable_FOREACH(Interface_SYMBOLS(this), Interface_KEY, Symbol, Interface_ITER) { \
    CGIRSYM cgirsym = (CGIRSYM)(uintptr_t)Interface_KEY; \
    Symbol symbol = *Interface_ITER;
#define Interface_ENDEACH_Symbol \
  } HTable_ENDEACH;
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Interface_TEMPORARIES.
 */
#define Interface_FOREACH_Temporary(this, cgirtn, temporary) \
  HTable_FOREACH(Interface_TEMPORARIES(this), Interface_KEY, Temporary, Interface_ITER) { \
    CGIRTN cgirtn = (CGIRTN)(uintptr_t)Interface_KEY; \
    Temporary temporary = *Interface_ITER;
#define Interface_ENDEACH_Temporary \
  } HTable_ENDEACH;
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Interface_OPERATIONS.
 */
#define Interface_FOREACH_Operation(this, cgirop, operation) \
  HTable_FOREACH(Interface_OPERATIONS(this), Interface_KEY, Operation, Interface_ITER) { \
    CGIROP cgirop = (CGIROP)(uintptr_t)Interface_KEY; \
    Operation operation = *Interface_ITER;
#define Interface_ENDEACH_Operation \
  } HTable_ENDEACH;
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Interface_BASICBLOCKS.
 */
#define Interface_FOREACH_BasicBlock(this, cgirbb, basicBlock) \
  HTable_FOREACH(Interface_BASICBLOCKS(this), Interface_KEY, BasicBlock, Interface_ITER) { \
    CGIRBB cgirbb = (CGIRBB)(uintptr_t)Interface_KEY; \
    BasicBlock basicBlock = *Interface_ITER;
#define Interface_ENDEACH_BasicBlock \
  } HTable_ENDEACH;
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Interface_LOOPSCOPES.
 */
#define Interface_FOREACH_LoopScope(this, cgirld, loopScope) \
  HTable_FOREACH(Interface_LOOPSCOPES(this), Interface_KEY, LoopScope, Interface_ITER) { \
    CGIRLD cgirld = (CGIRLD)(uintptr_t)Interface_KEY; \
    LoopScope loopScope = *Interface_ITER;
#define Interface_ENDEACH_LoopScope \
  } HTable_ENDEACH;
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Interface_LIR2CGIR
 */
#define Interface_FOREACH_InterfaceMapped(this, key, mapped) \
  HTable_FOREACH(Interface_LIR2CGIR(this), key, InterfaceMapped_, mapped)
#define Interface_ENDEACH_InterfaceMapped \
  HTable_ENDEACH;
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Interface_setCGIRImport(Interface this, const void *object)
{
  HTable_insert(Interface_CGIRIMPORT(this), (HTableKey)object, 0);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Interface_isCGIRImport(Interface this, const void *object)
{
  return HTable_search(Interface_CGIRIMPORT(this), (HTableKey)object) != NULL;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Interface_setCGIRValid(Interface this, const void *object)
{
  HTable_insert(Interface_CGIRVALID(this), (HTableKey)object, 0);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Interface_isCGIRValid(Interface this, const void *object)
{
  return HTable_search(Interface_CGIRVALID(this), (HTableKey)object) != NULL;
}
#endif//$XCC__h

#ifdef $XCC_h
int *Interface_Registers(int count, ...);
#endif//$XCC_h

int *Interface_Registers(int count, ...)
{
  int i;
  va_list va;
  Interface this = interface;
  int *registers = Interface_REGISTERARRAY(this);
  if (count > Interface_REGISTERCOUNT(this)) {
    Memory memory = Interface_MEMORY(this);
    Memory_free(memory, Interface_REGISTERARRAY(this));
    registers = *Interface__REGISTERARRAY(this) = Memory_alloc(memory, sizeof(int)*count);
    *Interface__REGISTERCOUNT(this) = count;
  }
  va_start(va, count);
  for (i = 0; i < count; i++) {
    registers[i] = va_arg(va, int);
  }
  va_end(va);
  return registers;
}

#ifdef $XCC__h
/*
 * Interface_PrintRegisters - Print a Register array as a call to Registers.
 */
int
Interface_PrintRegisters(FILE *file, int count, int registers[]);
#endif//$XCC__h

#ifndef _NTRACE
int
Interface_PrintRegisters(FILE *file, int count, int registers[])
{
  int i;
  fprintf(file, "Interface_Registers(%d", count);
  for (i = 0; i < count; i++) {
    fprintf(file, ", Register_%s", Register_Id(registers[i]));
  }
  fprintf(file, ")");
  return 1;
}
#endif//_NTRACE

#ifdef $XCC_h
Label *
Interface_Labels(int count, ...);
#endif//$XCC_h

Label *
Interface_Labels(int count, ...)
{
  int i;
  va_list va;
  Interface this = interface;
  Label *labels = Interface_LABELARRAY(this);
  if (count > Interface_LABELCOUNT(this)) {
    Memory memory = Interface_MEMORY(this);
    Memory_free(memory, Interface_LABELARRAY(this));
    labels = *Interface__LABELARRAY(this) = Memory_alloc(memory, sizeof(Label)*count);
    *Interface__LABELCOUNT(this) = count;
  }
  va_start(va, count);
  for (i = 0; i < count; i++) {
    labels[i] = va_arg(va, Label);
  }
  va_end(va);
  return labels;
}

#ifdef $XCC__h
/**
 * Print a Label array as a call to Labels.
 */
int
Interface_PrintLabels(FILE *file, int count, Label labels[]);
#endif//$XCC__h

#ifndef _NTRACE
int
Interface_PrintLabels(FILE *file, int count, Label labels[])
{
  int i;
  fprintf(file, "Interface_Labels(%d", count);
  for (i = 0; i < count; i++) {
    fprintf(file, ", label[%d]", Label_identity(labels[i]));
  }
  fprintf(file, ")");
  return 1;
}
#endif//_NTRACE

#ifdef $XCC_h
Temporary *
Interface_Temporaries(int count, ...);
#endif//$XCC_h

static Temporary *
Interface_Results(int count, va_list va)
{
  int i;
  Interface this = interface;
  Temporary *temporaries = Interface_RESULTARRAY(this);
  if (count > Interface_RESULTCOUNT(this)) {
    Memory memory = Interface_MEMORY(this);
    Memory_free(memory, Interface_RESULTARRAY(this));
    temporaries = *Interface__RESULTARRAY(this) = Memory_alloc(memory, sizeof(Temporary)*count);
    *Interface__RESULTCOUNT(this) = count;
  }
  for (i = 0; i < count; i++) {
    temporaries[i] = va_arg(va, Temporary);
  }
  return temporaries;
}

static Temporary *
Interface_Arguments(int count, va_list va)
{
  int i;
  Interface this = interface;
  Temporary *temporaries = Interface_ARGUMENTARRAY(this);
  if (count > Interface_ARGUMENTCOUNT(this)) {
    Memory memory = Interface_MEMORY(this);
    Memory_free(memory, Interface_ARGUMENTARRAY(this));
    temporaries = *Interface__ARGUMENTARRAY(this) = Memory_alloc(memory, sizeof(Temporary)*count);
    *Interface__ARGUMENTCOUNT(this) = count;
  }
  for (i = 0; i < count; i++) {
    temporaries[i] = va_arg(va, Temporary);
  }
  return temporaries;
}

Temporary *
Interface_Temporaries(int count, ...)
{
  int i;
  va_list va;
  Interface this = interface;
  Temporary *temporaries = NULL;
  static bool results = false;
  va_start(va, count);
  if (results) {
    temporaries = Interface_Arguments(count, va);
    results = false;
  } else {
    temporaries = Interface_Results(count, va);
    results = true;
  }
  va_end(va);
  return temporaries;
}

#ifdef $XCC__h
/**
 * Print a Temporary array as a call to Temporaries.
 */
int
Interface_PrintTemporaries(FILE *file, int count, Temporary temporaries[]);
#endif//$XCC__h

#ifndef _NTRACE
int
Interface_PrintTemporaries(FILE *file, int count, Temporary temporaries[])
{
  int i;
  fprintf(file, "Interface_Temporaries(%d", count);
  for (i = 0; i < count; i++) {
    fprintf(file, ", temporary[%d]", Temporary_identity(temporaries[i]));
  }
  fprintf(file, ")");
  return 1;
}
#endif//_NTRACE

#ifdef $XCC_h
Operation *
Interface_Operations(int count, ...);
#endif//$XCC_h

Operation *
Interface_Operations(int count, ...)
{
  int i;
  va_list va;
  Interface this = interface;
  Operation *operations = Interface_OPERATIONARRAY(this);
  if (count > Interface_OPERATIONCOUNT(this)) {
    Memory memory = Interface_MEMORY(this);
    Memory_free(memory, Interface_OPERATIONARRAY(this));
    operations = *Interface__OPERATIONARRAY(this) = Memory_alloc(memory, sizeof(Operation)*count);
    *Interface__OPERATIONCOUNT(this) = count;
  }
  va_start(va, count);
  for (i = 0; i < count; i++) {
    operations[i] = va_arg(va, Operation);
  }
  va_end(va);
  return operations;
}

#ifdef $XCC__h
/*
 * Interface_PrintOperations - Print a Operation array as a call to Operations.
 */
int
Interface_PrintOperations(FILE *file, int count, Operation operations[]);
#endif//$XCC__h

#ifndef _NTRACE
int
Interface_PrintOperations(FILE *file, int count, Operation operations[])
{
  int i;
  fprintf(file, "Interface_Operations(%d", count);
  for (i = 0; i < count; i++) {
    fprintf(file, ", operation[%d]", Operation_identity(operations[i]));
  }
  fprintf(file, ")");
  return 1;
}
#endif//_NTRACE

#ifdef $XCC_h
/*--------------------- LAO Interface Input Functions ------------------------*/
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the processor max issue (override MDT information).
 */
void
Interface_setMaxIssue(Interface this, Processor processor, int maxIssue);
#endif//$XCC_h

void
Interface_setMaxIssue(Interface this, Processor processor, int maxIssue)
{
  MDT_setMaxIssue(processor, maxIssue);
  O64_INPUT && fprintf(O64_INPUT,
                       "  Interface_setMaxIssue(interface, Processor_%s, %d);\n",
                       Processor_Id(processor), maxIssue);
}

#ifdef $XCC_h
/**
 * Set the processor min taken (override MDT information).
 */
void
Interface_setMinTaken(Interface this, Processor processor, int minTaken);
#endif//$XCC_h

void
Interface_setMinTaken(Interface this, Processor processor, int minTaken)
{
  MDT_setMinTaken(processor, minTaken);
  O64_INPUT && fprintf(O64_INPUT,
                       "  Interface_setMinTaken(interface, Processor_%s, %d);\n",
                       Processor_Id(processor), minTaken);
}

#ifdef $XCC_h
/**
 * Set a register as reserved (override MDT information).
 */
void
Interface_setReserved(Interface this, Convention convention, Register reserved);
#endif//$XCC_h

void
Interface_setReserved(Interface this, Convention convention, Register reserved)
{
  MDT_setReserved(convention, reserved);
  O64_INPUT && fprintf(O64_INPUT,
                       "  Interface_setReserved(interface, Convention_%s, Register_%s);\n",
                       Convention_Id(convention), Register_Id(reserved));
}

#ifdef $XCC_h
/**
 * Set the argument read stage (override MDT information).
 */
bool
Interface_setReadStage(Interface this, Processor processor, Operator operator, int argIndex, int stage);
#endif//$XCC_h

bool
Interface_setReadStage(Interface this, Processor processor, Operator operator, int argIndex, int stage)
{
  Memory memory =  Interface_MEMORY(this);
  Opcode opcode = Operator_opcode(operator, processor);
  O64_INPUT && fprintf(O64_INPUT,
      "Interface_setReadStage(interface, Processor_%s, Operator_%s, %d, %d);\n",
      Processor_Id(processor), Operator_Id(operator), argIndex, stage);
  if (opcode != Opcode__UNDEF) {
    const_RegisterActionArray actionArray = Opcode_actionArray(opcode);
    RegisterAction_ *items = Opcode_makeRegisterActionItems(opcode, memory);
    RegisterActionArray_FORREAD_RegisterAction(actionArray, action) {
      if (RegisterAction_xxxIndex(action) == argIndex) {
        RegisterAction_setStage(items + RegisterActionArray_INDEX, stage);
        return (RegisterAction_stage(action) == stage);
      }
    } RegisterActionArray_ENDREAD_RegisterAction;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Set the result write stage (override MDT information).
 */
bool
Interface_setWriteStage(Interface this, Processor processor, Operator operator, int resIndex, int stage);
#endif//$XCC_h

bool
Interface_setWriteStage(Interface this, Processor processor, Operator operator, int resIndex, int stage)
{
  Memory memory =  Interface_MEMORY(this);
  Opcode opcode = Operator_opcode(operator, processor);
  O64_INPUT && fprintf(O64_INPUT,
      "Interface_setWriteStage(interface, Processor_%s, Operator_%s, %d, %d);\n",
      Processor_Id(processor), Operator_Id(operator), resIndex, stage);
  if (opcode != Opcode__UNDEF) {
    const_RegisterActionArray actionArray = Opcode_actionArray(opcode);
    RegisterAction_ *items = Opcode_makeRegisterActionItems(opcode, memory);
    RegisterActionArray_FORWRITE_RegisterAction(actionArray, action) {
      if (RegisterAction_xxxIndex(action) == resIndex) {
        RegisterAction_setStage(items + RegisterActionArray_INDEX, stage);
        return RegisterAction_stage(action) == stage;
      }
    } RegisterActionArray_ENDWRITE_RegisterAction;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Make a Label.
 */
Label
Interface_makeLabel(Interface this, CGIRLAB cgirlab, const char *name);
#endif//$XCC_h

Label
Interface_makeLabel(Interface this, CGIRLAB cgirlab, const char *name)
{
  void *pointer = HTable_lookup(Interface_LABELS(this), (HTableKey)(uintptr_t)cgirlab, sizeof(Label));
  Label label = *(Label *)pointer;
  if (label == NULL) {
    Program program = Interface_PROGRAM(this);
    LabelTable labelTable = Program_labelTable(program);
    label = *(Label *)pointer = LabelTable_lookup(labelTable, cgirlab, name);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    label[%d] = Interface_makeLabel(interface, CGIRLAB(%td), \"%s\");\n",
        Label_identity(label), CGIRLAB_identity(cgirlab), name);
#endif//_NTRACE
    //KEEP Interface_setCGIRImport(this, label);
  }
  return label;
}

#ifdef $XCC_h
/**
 * Find a Label.
 */
Label
Interface_findLabel(Interface this, CGIRLAB cgirlab);
#endif//$XCC_h

Label
Interface_findLabel(Interface this, CGIRLAB cgirlab)
{
  void *pointer = HTable_search(Interface_LABELS(this), (HTableKey)(uintptr_t)cgirlab);
  if (pointer != NULL) return *(Label *)pointer;
  return NULL;
}

#ifdef $XCC_h
/**
 * Make a Symbol.
 */
Symbol
Interface_makeSymbol(Interface this, CGIRSYM cgirsym, const char *name);
#endif//$XCC_h

Symbol
Interface_makeSymbol(Interface this, CGIRSYM cgirsym, const char *name)
{
  void *pointer = HTable_lookup(Interface_SYMBOLS(this), (HTableKey)(uintptr_t)cgirsym, sizeof(Symbol));
  Symbol symbol = *(Symbol *)pointer;
  if (symbol == NULL) {
    Program program = Interface_PROGRAM(this);
    SymbolTable symbolTable = Program_symbolTable(program);
    symbol = *(Symbol *)pointer = SymbolTable_makeSymbol(symbolTable, name);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    symbol[%d] = Interface_makeSymbol(interface, CGIRSYM(%td), \"%s\");\n",
        Symbol_identity(symbol), CGIRSYM_identity(cgirsym), name);
#endif//_NTRACE
    //KEEP Interface_setCGIRImport(this, symbol);
  }
  return symbol;
}

#ifdef $XCC_h
/**
 * Set the (class,store,export) for a Symbol.
 */
void
Interface_Symbol_setClasses(Interface this, Symbol symbol, SymbolClass sclass, SymbolStore sstore, SymbolExport sexport);
#endif//$XCC_h

void
Interface_Symbol_setClasses(Interface this, Symbol symbol, SymbolClass sclass, SymbolStore sstore, SymbolExport sexport)
{
  SymbolClass lao_sclass = (SymbolClass)sclass;
  SymbolStore lao_sstore = (SymbolStore)sstore;
  SymbolExport lao_sexport = (SymbolExport)sexport;
  Symbol_setClasses(symbol, lao_sclass, lao_sstore, lao_sexport);
#ifndef _NTRACE
  O64_INPUT && 
    fprintf(O64_INPUT,
            "    Interface_Symbol_setClasses(interface, symbol[%d], SymbolClass_%s, SymbolStore_%s, SymbolExport_%s);\n",
            Symbol_identity(symbol), SymbolClass_Id(lao_sclass), SymbolStore_Id(lao_sstore), SymbolExport_Id(lao_sexport));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Find a Symbol.
 */
Symbol
Interface_findSymbol(Interface this, CGIRSYM cgirsym);
#endif//$XCC_h

Symbol
Interface_findSymbol(Interface this, CGIRSYM cgirsym)
{
  void *pointer = HTable_search(Interface_SYMBOLS(this), (HTableKey)(uintptr_t)cgirsym);
  if (pointer != NULL) return *(Symbol *)pointer;
  return NULL;
}

#ifdef $XCC_h
/**
 * Make a Dedicated Temporary.
 */
Temporary
Interface_makeDedicatedTemporary(Interface this, CGIRTN cgirtn, Register registre);
#endif//$XCC_h

Temporary
Interface_makeDedicatedTemporary(Interface this, CGIRTN cgirtn, Register registre)
{
  Procedure procedure = Interface_PROCEDURE(this);
  void *pointer = HTable_lookup(Interface_TEMPORARIES(this), (HTableKey)(uintptr_t)cgirtn, sizeof(Temporary));
  Temporary temporary = *(Temporary *)pointer;
  Except_REQUIRE(registre != Register__UNDEF);
  if (temporary == NULL) {
    temporary = *(Temporary *)pointer = Procedure_makeDedicatedTemporary(procedure, registre);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    temporary[%d] = Interface_makeDedicatedTemporary(interface, CGIRTN(%td), Register_%s);\n",
        Temporary_identity(temporary), CGIRTN_identity(cgirtn), Register_Id(registre));
#endif//_NTRACE
    //KEEP Interface_setCGIRImport(this, temporary);
  }
  Except_ENSURE(Temporary_isDedicated(temporary));
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a Assigned Temporary.
 */
Temporary
Interface_makeAssignedTemporary(Interface this, CGIRTN cgirtn, Register registre);
#endif//$XCC_h

Temporary
Interface_makeAssignedTemporary(Interface this, CGIRTN cgirtn, Register registre)
{
  Procedure procedure = Interface_PROCEDURE(this);
  void *pointer = HTable_lookup(Interface_TEMPORARIES(this), (HTableKey)(uintptr_t)cgirtn, sizeof(Temporary));
  Temporary temporary = *(Temporary *)pointer;
  if (temporary == NULL) {
    temporary = *(Temporary *)pointer = Procedure_makeAssignedTemporary(procedure, registre);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    temporary[%d] = Interface_makeAssignedTemporary(interface, CGIRTN(%td), Register_%s);\n",
        Temporary_identity(temporary), CGIRTN_identity(cgirtn), Register_Id(registre));
#endif//_NTRACE
    //KEEP Interface_setCGIRImport(this, temporary);
  }
  Except_ENSURE(Temporary_isAssigned(temporary));
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a Virtual Temporary.
 */
Temporary
Interface_makeVirtualTemporary(Interface this, CGIRTN cgirtn, RegFile regFile);
#endif//$XCC_h

Temporary
Interface_makeVirtualTemporary(Interface this, CGIRTN cgirtn, RegFile regFile)
{
  Procedure procedure = Interface_PROCEDURE(this);
  void *pointer = HTable_lookup(Interface_TEMPORARIES(this), (HTableKey)(uintptr_t)cgirtn, sizeof(Temporary));
  Temporary temporary = *(Temporary *)pointer;
  if (temporary == NULL) {
    temporary = *(Temporary *)pointer = Procedure_makeVirtualTemporary(procedure, regFile);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    temporary[%d] = Interface_makeVirtualTemporary(interface, CGIRTN(%td), RegFile_%s);\n",
        Temporary_identity(temporary), CGIRTN_identity(cgirtn), RegFile_Id(regFile));
#endif//_NTRACE
    //KEEP Interface_setCGIRImport(this, temporary);
  }
  Except_ENSURE(Temporary_isVirtual(temporary));
  return temporary;
}

#ifdef $XCC_h
/**
 * Make an Absolute Temporary.
 */
Temporary
Interface_makeAbsoluteTemporary(Interface this, CGIRTN cgirtn, Immediate immediate, int64_t value);
#endif//$XCC_h

Temporary
Interface_makeAbsoluteTemporary(Interface this, CGIRTN cgirtn, Immediate immediate, int64_t value)
{
  Procedure procedure = Interface_PROCEDURE(this);
  void *pointer = HTable_lookup(Interface_TEMPORARIES(this), (HTableKey)(uintptr_t)cgirtn, sizeof(Temporary));
  Temporary temporary = *(Temporary *)pointer;
  if (temporary == NULL) {
    temporary = *(Temporary *)pointer = Procedure_makeAbsoluteTemporary(procedure, value, immediate);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    temporary[%d] = Interface_makeAbsoluteTemporary(interface, CGIRTN(%td), Immediate_%s, %lldLL);\n",
        Temporary_identity(temporary), CGIRTN_identity(cgirtn),
        Immediate_Id(immediate), (long long)value);
#endif//_NTRACE
    //KEEP Interface_setCGIRImport(this, temporary);
  }
  Except_ENSURE(Temporary_isAbsolute(temporary));
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a Symbol Temporary.
 */
Temporary
Interface_makeSymbolTemporary(Interface this, CGIRTN cgirtn, Immediate immediate, Symbol symbol, int64_t offset);
#endif//$XCC_h

Temporary
Interface_makeSymbolTemporary(Interface this, CGIRTN cgirtn, Immediate immediate, Symbol symbol, int64_t offset)
{
  Procedure procedure = Interface_PROCEDURE(this);
  void *pointer = HTable_lookup(Interface_TEMPORARIES(this), (HTableKey)(uintptr_t)cgirtn, sizeof(Temporary));
  Temporary temporary = *(Temporary *)pointer;
  if (temporary == NULL) {
    temporary = *(Temporary *)pointer = Procedure_makeSymbolTemporary(procedure, symbol, offset, immediate);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    temporary[%d] = Interface_makeSymbolTemporary(interface, CGIRTN(%td), Immediate_%s, symbol[%d], %lldLL);\n",
        Temporary_identity(temporary), CGIRTN_identity(cgirtn),
        Immediate_Id(immediate), Symbol_identity(symbol), (long long)offset);
#endif//_NTRACE
    //KEEP Interface_setCGIRImport(this, temporary);
  }
  Except_ENSURE(Temporary_isSymbol(temporary));
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a Label Temporary.
 */
Temporary
Interface_makeLabelTemporary(Interface this, CGIRTN cgirtn, Immediate immediate, Label label);
#endif//$XCC_h

Temporary
Interface_makeLabelTemporary(Interface this, CGIRTN cgirtn, Immediate immediate, Label label)
{
  Procedure procedure = Interface_PROCEDURE(this);
  void *pointer = HTable_lookup(Interface_TEMPORARIES(this), (HTableKey)(uintptr_t)cgirtn, sizeof(Temporary));
  Temporary temporary = *(Temporary *)pointer;
  if (temporary == NULL) {
    temporary = *(Temporary *)pointer = Procedure_makeLabelTemporary(procedure, label, immediate);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    temporary[%d] = Interface_makeLabelTemporary(interface, CGIRTN(%td), Immediate_%s, label[%d]);\n",
        Temporary_identity(temporary), CGIRTN_identity(cgirtn), Immediate_Id(immediate), Label_identity(label));
#endif//_NTRACE
    //KEEP Interface_setCGIRImport(this, temporary);
  }
  Except_ENSURE(Temporary_isLabel(temporary));
  return temporary;
}

#ifdef $XCC_h
/**
 * Make a Modifier Temporary.
 */
Temporary
Interface_makeModifierTemporary(Interface this, CGIRTN cgirtn, ModifierMember member);
#endif//$XCC_h

Temporary
Interface_makeModifierTemporary(Interface this, CGIRTN cgirtn, ModifierMember member)
{
  Procedure procedure = Interface_PROCEDURE(this);
  void *pointer = HTable_lookup(Interface_TEMPORARIES(this), (HTableKey)(uintptr_t)cgirtn, sizeof(Temporary));
  Temporary temporary = *(Temporary *)pointer;
  if (temporary == NULL) {
    temporary = *(Temporary *)pointer = Procedure_makeModifierTemporary(procedure, member);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    temporary[%d] = Interface_makeModifierTemporary(interface, CGIRTN(%td), ModifierMember_%s);\n",
        Temporary_identity(temporary), CGIRTN_identity(cgirtn), ModifierMember_Id(member));
#endif//_NTRACE
    //KEEP Interface_setCGIRImport(this, temporary);
  }
  Except_ENSURE(Temporary_isModifier(temporary));
  return temporary;
}

#ifdef $XCC_h
/**
 * Set a Temporary bit-width.
 */
void
Interface_Temporary_setWidth(Interface this, Temporary temporary, unsigned width);
#endif//$XCC_h

void
Interface_Temporary_setWidth(Interface this, Temporary temporary, unsigned width)
{
  
  Temporary_setWidth(temporary, width);
#ifndef _NTRACE
  O64_INPUT && 
    fprintf(O64_INPUT,
            "    Interface_Temporary_setWidth(interface, temporary[%d], %u);\n",
            Temporary_identity(temporary), width);
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set an Temporary as remat.
 */
void
Interface_Temporary_setRemater(Interface this, Temporary temporary, Temporary value);
#endif//$XCC_h

void
Interface_Temporary_setRemater(Interface this, Temporary temporary, Temporary value)
{
  Procedure procedure = Interface_PROCEDURE(this);
  Procedure_setRemater(procedure, temporary, value);
#ifndef _NTRACE
  O64_INPUT && 
    fprintf(O64_INPUT,
            "    Interface_Temporary_setRemater(interface, temporary[%d], temporary[%d]);\n",
            Temporary_identity(temporary), Temporary_identity(value));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set an Temporary as homeable to location.
 */
void
Interface_Temporary_setHomeable(Interface this, Temporary temporary, Temporary location);
#endif//$XCC_h

void
Interface_Temporary_setHomeable(Interface this, Temporary temporary, Temporary location)
{
  Procedure procedure = Interface_PROCEDURE(this);
  Temporary_setFlags(temporary, TemporaryFlag_Homeable);
  Procedure_setHomeable(procedure, temporary, location);
#ifndef _NTRACE
  O64_INPUT && 
    fprintf(O64_INPUT,
            "    Interface_Temporary_setHomeable(interface, temporary[%d]. temporary[%d]);\n",
            Temporary_identity(temporary), Temporary_identity(location));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set an Temporary as dedicated.
 */
void
Interface_Temporary_setDedicated(Interface this, Temporary temporary);
#endif//$XCC_h

void
Interface_Temporary_setDedicated(Interface this, Temporary temporary)
{
  Temporary_setDedicated(temporary);
#ifndef _NTRACE
  O64_INPUT && 
    fprintf(O64_INPUT,
            "    Interface_Temporary_setDedicated(interface, temporary[%d]);\n",
            Temporary_identity(temporary));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Find a Temporary.
 */
Temporary
Interface_findTemporary(Interface this, CGIRTN cgirtn);
#endif//$XCC_h

Temporary
Interface_findTemporary(Interface this, CGIRTN cgirtn)
{
  void *pointer = HTable_search(Interface_TEMPORARIES(this), (HTableKey)(uintptr_t)cgirtn);
  if (pointer != NULL) return *(Temporary *)pointer;
  return NULL;
}

#ifdef $XCC_h
/*--------------------- LAO Interface Accessors Functions ------------------------*/
#endif//$XCC_h

#ifdef $XCC_h
/**
 * O64 Interface for queries on LAO Temporaries.
 */
uint32_t
Interface_Temporary_identity(Temporary temporary);
int
Interface_Temporary_isAbsolute(Temporary temporary);
int64_t
Interface_Temporary_value(Temporary temporary);
int
Interface_Temporary_isSymbol(Temporary temporary);
Symbol
Interface_Temporary_symbol(Temporary temporary);
int64_t
Interface_Temporary_offset(Temporary temporary);
int
Interface_Temporary_isVirtual(Temporary temporary);
int
Interface_Temporary_isDedicated(Temporary temporary);
int
Interface_Temporary_isAssigned(Temporary temporary);
Register
Interface_Temporary_register(Temporary temporary);
RegFile
Interface_Temporary_regFile(Temporary temporary);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * O64Interface for queries on LAO Symbols.
 */
uint32_t
Interface_Symbol_identity(Symbol symbol);
const char *
Interface_Symbol_name(Symbol symbol);
int
Interface_Symbol_isSpill(Symbol symbol);
NativeType
Interface_Symbol_nativeType(Symbol symbol);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * O64 Interface for queries on LAO Operations.
 */
uint32_t
Interface_Operation_identity(Operation operation);
Operator
Interface_Operation_operator(Operation operation);
int
Interface_Operation_instance(Operation operation);
int
Interface_Operation_startDate(Operation operation);
int
Interface_Operation_isMemorySpill(Operation operation);
int
Interface_Operation_isVolatile(Operation operation);
int
Interface_Operation_isHoisted(Operation operation);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * O64 Interface for queries on LAO Labels.
 */
uint32_t
Interface_Label_identity(Label label);
const char *
Interface_Label_name(Label label);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * O64 Interface for queries on LAO BasicBlocks.
 */
uint32_t
Interface_BasicBlock_identity(BasicBlock basicBlock);
int
Interface_BasicBlock_instance(BasicBlock basicBlock);
intptr_t
Interface_BasicBlock_traceId(BasicBlock basicBlock);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * O64 Interface for queries on LAO LoopScopes.
 */
uint32_t
Interface_LoopScope_identity(LoopScope loopScope);
int
Interface_LoopScope_unrolled(LoopScope loopScope);
#endif//$XCC_h

/**
 * Implementation of O64 Interface Accessors Functions.
 */
uint32_t
Interface_Temporary_identity(Temporary temporary)
{
  return Temporary_identity(temporary);
}
int
Interface_Temporary_isAbsolute(Temporary temporary)
{
  return Temporary_isAbsolute(temporary);
}
int64_t
Interface_Temporary_value(Temporary temporary)
{
  Literal literal = Temporary_LITERAL(temporary);
  return Literal_VALUE(literal);
}
int
Interface_Temporary_isSymbol(Temporary temporary)
{
  return Temporary_isSymbol(temporary);
}
Symbol
Interface_Temporary_symbol(Temporary temporary)
{
  Literal literal = Temporary_LITERAL(temporary);
  return Literal_SYMBOL(literal);
}
int64_t
Interface_Temporary_offset(Temporary temporary)
{
  Literal literal = Temporary_LITERAL(temporary);
  return Literal_OFFSET(literal);
}
int
Interface_Temporary_isVirtual(Temporary temporary)
{
  return Temporary_isVirtual(temporary);
}
int
Interface_Temporary_isDedicated(Temporary temporary)
{
  return Temporary_isDedicated(temporary);
}
int
Interface_Temporary_isAssigned(Temporary temporary)
{
  return Temporary_isAssigned(temporary);
}
Register
Interface_Temporary_register(Temporary temporary)
{
  return Temporary_REGISTER(temporary);
}
RegFile
Interface_Temporary_regFile(Temporary temporary)
{
  return Temporary_REGFILE(temporary);
}
uint32_t
Interface_Symbol_identity(Symbol symbol)
{
  return Symbol_identity(symbol);
}
const char *
Interface_Symbol_name(Symbol symbol)
{
  return Symbol_name(symbol);
}
int
Interface_Symbol_isSpill(Symbol symbol)
{
  return Symbol_isSpill(symbol);
}
NativeType
Interface_Symbol_nativeType(Symbol symbol)
{
  return Symbol_nativeType(symbol);
}
uint32_t
Interface_Operation_identity(Operation operation)
{
  return Operation_identity(operation);
}
Operator
Interface_Operation_operator(Operation operation)
{
  return Operation_operator(operation);
}
int
Interface_Operation_instance(Operation operation)
{
  return Operation_instance(operation);
}
int
Interface_Operation_startDate(Operation operation)
{
  return Operation_startDate(operation);
}
int
Interface_Operation_isMemorySpill(Operation operation)
{
  return Operation_isMemorySpill(operation);
}
int
Interface_Operation_isVolatile(Operation operation)
{
  return Operation_isVolatile(operation);
}
int
Interface_Operation_isHoisted(Operation operation)
{
  return Operation_isHoisted(operation);
}
uint32_t
Interface_Label_identity(Label label)
{
  return Label_identity(label);
}
const char *
Interface_Label_name(Label label)
{
  return Label_name(label);
}
uint32_t
Interface_BasicBlock_identity(BasicBlock basicBlock)
{
  return BasicBlock_identity(basicBlock);
}
int
Interface_BasicBlock_instance(BasicBlock basicBlock)
{
  return BasicBlock_instance(basicBlock);
}
intptr_t
Interface_BasicBlock_traceId(BasicBlock basicBlock)
{
  return BasicBlock_traceId(basicBlock);
}
uint32_t
Interface_LoopScope_identity(LoopScope loopScope)
{
  return LoopScope_identity(loopScope);
}
int
Interface_LoopScope_unrolled(LoopScope loopScope)
{
  return LoopScope_unrolled(loopScope);
}

#ifdef $XCC_h
/**
 * Make an Operation.
 */
Operation
Interface_makeOperation(Interface this, CGIROP cgirop,
    Operator operator, int instance, int argCount, Temporary arguments[],
    int resCount, Temporary results[], int regCount, int registers[]);
#endif//$XCC_h

Operation
Interface_makeOperation(Interface this, CGIROP cgirop,
    Operator operator, int instance, int argCount, Temporary arguments[],
    int resCount, Temporary results[], int regCount, int registers[])
{
  Selector selector = Interface_SELECTOR(this);
  void *pointer = HTable_lookup(Interface_OPERATIONS(this), (HTableKey)(uintptr_t)cgirop, sizeof(Operation));
  Operation operation = *(Operation *)pointer;
  if (operation == NULL) {
    int i;
    RegisterSet_ clobberSet_;
    RegisterSet clobberSet = NULL;
    if (regCount != 0) {
      clobberSet = RegisterSet_Ctor(&clobberSet_);
      for (i = 0; i < regCount; i++) {
        RegisterSet_insert(clobberSet, registers[i]);
      }
    }
    operation = *(Operation *)pointer
              = Selector_makeOperation(selector, operator, resCount, argCount, results, arguments, clobberSet);
    Selector_detachOperation(selector, operation);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    operation[%d] = Interface_makeOperation(interface, CGIROP(%td), Operator_%s, %d",
        Operation_identity(operation), CGIROP_identity(cgirop),
        Operator_Id(operator), instance);
    O64_INPUT && fprintf(O64_INPUT, ",\n\t%d, ", argCount);
    O64_INPUT && Interface_PrintTemporaries(O64_INPUT, argCount, arguments);
    O64_INPUT && fprintf(O64_INPUT, ",\n\t%d, ", resCount);
    O64_INPUT && Interface_PrintTemporaries(O64_INPUT, resCount, results);
    O64_INPUT && fprintf(O64_INPUT, ",\n\t%d, ", regCount);
    O64_INPUT && Interface_PrintRegisters(O64_INPUT, regCount, registers);
    O64_INPUT && fprintf(O64_INPUT, ");\n");
#endif//_NTRACE
    Operation_setInstance(operation, instance);
    //KEEP Interface_setCGIRImport(this, operation);
  }
  return operation;
}

#ifdef $XCC_h
/**
 * Find a Operation.
 */
Operation
Interface_findOperation(Interface this, CGIROP cgirop);
#endif//$XCC_h

Operation
Interface_findOperation(Interface this, CGIROP cgirop)
{
  void *pointer = HTable_search(Interface_OPERATIONS(this), (HTableKey)(uintptr_t)cgirop);
  if (pointer != NULL) return *(Operation *)pointer;
  return NULL;
}

#ifdef $XCC_h
/**
 * Set an Operation as safe memory access.
 */
void
Interface_Operation_setMemorySafe(Interface this, Operation operation);
#endif//$XCC_h

void
Interface_Operation_setMemorySafe(Interface this, Operation operation)
{
  Operation_setFlags(operation, OperationFlag_MemorySafe);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_Operation_setMemorySafe(interface, operation[%d]);\n",
      Operation_identity(operation));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set an Operation as a pure copy.
 */
void
Interface_Operation_setPureCopy(Interface this, Operation operation);
#endif//$XCC_h

void
Interface_Operation_setPureCopy(Interface this, Operation operation)
{
  Operation_setFlags(operation, OperationFlag_PureCopy);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_Operation_setPureCopy(interface, operation[%d]);\n",
      Operation_identity(operation));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set an Operation as volatile.
 */
void
Interface_Operation_setHoisted(Interface this, Operation operation);
#endif//$XCC_h

void
Interface_Operation_setHoisted(Interface this, Operation operation)
{
  Operation_setFlags(operation, OperationFlag_Hoisted);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_Operation_setHoisted(interface, operation[%d]);\n",
      Operation_identity(operation));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set an Operation as volatile.
 */
void
Interface_Operation_setVolatile(Interface this, Operation operation);
#endif//$XCC_h

void
Interface_Operation_setVolatile(Interface this, Operation operation)
{
  Operation_setFlags(operation, OperationFlag_Volatile);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_Operation_setVolatile(interface, operation[%d]);\n",
      Operation_identity(operation));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set an Operation as memory prefetch.
 */
void
Interface_Operation_setMemoryTouch(Interface this, Operation operation);
#endif//$XCC_h

void
Interface_Operation_setMemoryTouch(Interface this, Operation operation)
{
  Operation_setFlags(operation, OperationFlag_MemoryTouch);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_Operation_setMemoryTouch(interface, operation[%d]);\n",
      Operation_identity(operation));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set an Operation as memory preload.
 */
void
Interface_Operation_setMemoryLevel2(Interface this, Operation operation);
#endif//$XCC_h

void
Interface_Operation_setMemoryLevel2(Interface this, Operation operation)
{
  Operation_setFlags(operation, OperationFlag_MemoryLevel2);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_Operation_setMemoryLevel2(interface, operation[%d]);\n",
      Operation_identity(operation));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set an Operation as a memory fence.
 */
void
Interface_Operation_setMemoryFence(Interface this, Operation operation);
#endif//$XCC_h

void
Interface_Operation_setMemoryFence(Interface this, Operation operation)
{
  Operation_setFlags(operation, OperationFlag_MemoryFence);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_Operation_setMemoryFence(interface, operation[%d]);\n",
      Operation_identity(operation));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set an Operation as spill code.
 */
void
Interface_Operation_setMemorySpill(Interface this, Operation operation, Symbol symbol);
#endif//$XCC_h

void
Interface_Operation_setMemorySpill(Interface this, Operation operation, Symbol symbol)
{
  Operation_setFlags(operation, OperationFlag_MemorySpill);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_Operation_setMemorySpill(interface, operation[%d], symbol[%d]);\n",
      Operation_identity(operation), Symbol_identity(symbol));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Make a BasicBlock.
 */
BasicBlock
Interface_makeBasicBlock(Interface this, CGIRBB cgirbb,
                         int unrolled, int labelCount, Label labels[], int operationCount,
                         Operation operations[], intptr_t regionId, float frequency);
#endif//$XCC_h

BasicBlock
Interface_makeBasicBlock(Interface this, CGIRBB cgirbb,
                         int instance, int labelCount, Label labels[], int operationCount,
                         Operation operations[], intptr_t regionId, float frequency)
{
  void *pointer = HTable_lookup(Interface_BASICBLOCKS(this), (HTableKey)(uintptr_t)cgirbb, sizeof(BasicBlock));
  BasicBlock basicBlock = *(BasicBlock *)pointer;
  if (basicBlock == NULL) {
    int i;
    Procedure procedure = Interface_PROCEDURE(this);
    if (frequency < 0.0) frequency = 0.0;	// HACK ALERT!
    basicBlock = *(BasicBlock *)pointer = Procedure_makeBasicBlock(procedure, regionId, frequency);
    for (i = 0; i < labelCount; i++) BasicBlock_addLabel(basicBlock, labels[i]);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    basicBlock[%d] = Interface_makeBasicBlock(interface, CGIRBB(%td), %d",
        BasicBlock_identity(basicBlock), CGIRBB_identity(cgirbb), instance);
    O64_INPUT && fprintf(O64_INPUT, ",\n\t%d, ", labelCount);
    O64_INPUT && Interface_PrintLabels(O64_INPUT, labelCount, labels);
    O64_INPUT && fprintf(O64_INPUT, ",\n\t%d, ", operationCount);
    O64_INPUT && Interface_PrintOperations(O64_INPUT, operationCount, operations);
    O64_INPUT && fprintf(O64_INPUT, ", 0x%tx, %f);\n", (ptrdiff_t)regionId, frequency);
#endif//_NTRACE
    for (i = 0; i < operationCount; i++) {
      Selector_attachLast(BasicBlock_selector(basicBlock), operations[i]);
      // FdF 20090421: The GNUASM flag is only set here. It should also
      // be updated whenever a GNUASM operation is added in or removed from a basic block.
      if (Operator_isGNUASM(Operation_operator(operations[i])))
        BasicBlock_setFlags(basicBlock, BasicBlockFlag_GNUASM);
    }
    BasicBlock_setInstance(basicBlock, instance);
    Interface_setCGIRImport(this, basicBlock);
  }
  return basicBlock;
}

#ifdef $XCC_h
/**
 * Find a BasicBlock.
 */
BasicBlock
Interface_findBasicBlock(Interface this, CGIRBB cgirbb);
#endif//$XCC_h

BasicBlock
Interface_findBasicBlock(Interface this, CGIRBB cgirbb)
{
  void *pointer = HTable_search(Interface_BASICBLOCKS(this), (HTableKey)(uintptr_t)cgirbb);
  if (pointer != NULL) return *(BasicBlock *)pointer;
  return NULL;
}

#ifdef $XCC_h
/**
 * Link two BasicBlocks.
 */
void
Interface_linkBasicBlocks(Interface this, BasicBlock tail_block,
                          BasicBlock head_block, float probability);
#endif//$XCC_h

void
Interface_linkBasicBlocks(Interface this, BasicBlock tail_block,
                          BasicBlock head_block, float probability)
{
  Procedure procedure = Interface_PROCEDURE(this);
  if (probability < 0.0) probability = 0.0;	// HACK ALERT!
  BasicBlock_makeEdge(tail_block, head_block, probability);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_linkBasicBlocks(interface, basicBlock[%d], basicBlock[%d], %f);\n",
      BasicBlock_identity(tail_block), BasicBlock_identity(head_block), probability);
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Make a LoopScope.
 */
LoopScope
Interface_makeLoopScope(Interface this, CGIRLD cgirld, BasicBlock basicBlock,
                        Temporary temporary, ConfigureItem item, ...);
#endif//$XCC_h

LoopScope
Interface_makeLoopScope(Interface this, CGIRLD cgirld, BasicBlock basicBlock,
                        Temporary temporary, ConfigureItem item, ...)
{
  void *pointer = HTable_lookup(Interface_LOOPSCOPES(this), (HTableKey)(uintptr_t)cgirld, sizeof(LoopScope));
  LoopScope loopScope = *(LoopScope *)pointer;
  if (loopScope == NULL) {
    va_list va;
    Procedure procedure = Interface_PROCEDURE(this);
    loopScope = *(LoopScope *)pointer = Procedure_makeLoopScope(procedure, basicBlock, Interface_CONFIGURE(this));
    LoopScope_setTripCount(loopScope, temporary);	// FIXME! In case of SSA.
    va_start(va, item);
    Configure_loadVA(LoopScope_configure(loopScope), item, va);
    va_end(va);
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT,
        "    loopScope[%d] = Interface_makeLoopScope(interface, CGIRLD(%td), basicBlock[%d], ",
        LoopScope_identity(loopScope), CGIRLD_identity(cgirld), BasicBlock_identity(basicBlock));
    O64_INPUT && temporary != NULL && fprintf(O64_INPUT, "temporary[%d], ", Temporary_identity(temporary));
    O64_INPUT && temporary == NULL && fprintf(O64_INPUT, "NULL, ");
    O64_INPUT && Configure_print(LoopScope_configure(loopScope), O64_INPUT);
    O64_INPUT && fprintf(O64_INPUT, ");\n");
#endif//_NTRACE
    //KEEP Interface_setCGIRImport(this, loopScope);
  }
  return loopScope;
}

#ifdef $XCC_h
/**
 * Find a LoopScope.
 */
LoopScope
Interface_findLoopScope(Interface this, CGIRLD cgirld);
#endif//$XCC_h

LoopScope
Interface_findLoopScope(Interface this, CGIRLD cgirld)
{
  void *pointer = HTable_search(Interface_LOOPSCOPES(this), (HTableKey)(uintptr_t)cgirld);
  if (pointer != NULL) return *(LoopScope *)pointer;
  return NULL;
}

#ifdef $XCC_h
/**
 * Set Operation with a memory dependence node.
 */
void
Interface_LoopScope_setDependenceNode(Interface this, LoopScope loopScope, Operation operation, DependenceNode node);
#endif//$XCC_h

void
Interface_LoopScope_setDependenceNode(Interface this, LoopScope loopScope, Operation operation, DependenceNode node)
{
  DependenceTable dependenceTable = LoopScope_makeDependenceTable(loopScope);
  DependenceTable_enterNode(dependenceTable, operation);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_LoopScope_setDependenceNode(interface, loopScope[%d], operation[%d], CGIRWN(%td));\n",
      LoopScope_identity(loopScope), Operation_identity(operation), (ptrdiff_t)node);
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set a memory dependence arc between two operations.
 */
void
Interface_LoopScope_setDependenceArc(Interface this, LoopScope loopScope,
    Operation tail_operation, Operation head_operation, int latency, int omega, DependenceKind kind);
#endif//$XCC_h

void
Interface_LoopScope_setDependenceArc(Interface this, LoopScope loopScope,
    Operation tail_operation, Operation head_operation, int latency, int omega, DependenceKind kind) {
  DependenceTable dependenceTable = LoopScope_makeDependenceTable(loopScope);
  DependenceTable_makeArc(dependenceTable, tail_operation, head_operation,
                          latency, omega, DependenceKind(kind));
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_LoopScope_setDependenceArc(interface, loopScope[%d], operation[%d], operation[%d], %d, %d",
      LoopScope_identity(loopScope), Operation_identity(tail_operation), Operation_identity(head_operation), latency, omega);
  O64_INPUT && fprintf(O64_INPUT,
      ", DependenceKind_%s);\n",
      DependenceKind_Id(DependenceKind(kind)));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set a BasicBlock as a body block.
 */
void
Interface_setBody(Interface this, BasicBlock basicBlock);
#endif//$XCC_h

void
Interface_setBody(Interface this, BasicBlock basicBlock)
{
  Procedure procedure = Interface_PROCEDURE(this);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_setBody(interface, basicBlock[%d]);\n",
      BasicBlock_identity(basicBlock));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set a BasicBlock as an entry block.
 */
void
Interface_setEntry(Interface this, BasicBlock basicBlock);
#endif//$XCC_h

void
Interface_setEntry(Interface this, BasicBlock basicBlock)
{
  Procedure procedure = Interface_PROCEDURE(this);
  Procedure_setEntryBlock(procedure, basicBlock);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_setEntry(interface, basicBlock[%d]);\n",
      BasicBlock_identity(basicBlock));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set a BasicBlock as an exit block.
 */
void
Interface_setExit(Interface this, BasicBlock basicBlock);
#endif//$XCC_h

void
Interface_setExit(Interface this, BasicBlock basicBlock)
{
  Procedure procedure = Interface_PROCEDURE(this);
  Procedure_setExitBlock(procedure, basicBlock);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_setExit(interface, basicBlock[%d]);\n",
      BasicBlock_identity(basicBlock));
#endif//_NTRACE
}

#ifdef $XCC_h
/**
 * Set a BasicBlock as trace start block.
 */
void
Interface_setStart(Interface this, BasicBlock basicBlock);
#endif//$XCC_h

void
Interface_setStart(Interface this, BasicBlock basicBlock)
{
  Procedure procedure = Interface_PROCEDURE(this);
  Procedure_setStartBlock(procedure, basicBlock);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT,
      "    Interface_setStart(interface, basicBlock[%d]);\n",
      BasicBlock_identity(basicBlock));
#endif//_NTRACE
}

#ifdef $XCC_h
/*--------------------- LAO Interface Output Functions -----------------------*/
#endif//$XCC_h

/**
 * Make the CGIRLAB associated with a Label.
 */
static CGIRLAB
Interface_makeCGIRLAB(Interface this, const_Label label, CGIRCallBack callback)
{
  InterfaceMapped mapped = HTable_lookup(Interface_LIR2CGIR(this), (HTableKey)label, sizeof(InterfaceMapped_));
  CGIRLAB cgirlab = (CGIRLAB)(uintptr_t)InterfaceMapped_CGIR(mapped);
  //
  if (!Interface_isCGIRValid(this, label)) {
    const char *name = Label_name(label);
    Interface_setCGIRValid(this, label);
    if (cgirlab == (CGIRLAB)0) {
      // Just created by HTable_lookup.
      cgirlab = (*CGIRCallBack_LAB_make(callback))((CGIRLAB)0, name);
      // WARNING: cgirlab is now different from Label_INDEX!
      // Update CGIR -> LAO table.
      *(const_Label *)HTable_lookup(Interface_LABELS(this), (HTableKey)(uintptr_t)cgirlab, sizeof(Label)) = label;
      InterfaceMapped_Ctor(mapped, (InterfaceCGIR)(uintptr_t)cgirlab, InterfaceType_Label);
    } else {
      cgirlab = (*CGIRCallBack_LAB_make(callback))(cgirlab, name);
      Except_CHECK(InterfaceMapped_TYPE(mapped) == InterfaceType_Label);
      *InterfaceMapped__CGIR(mapped) = (InterfaceCGIR)(uintptr_t)cgirlab;
    }
  }
  Except_ENSURE(cgirlab != 0);
  return cgirlab;
}

/**
 * Make the CGIRSYM associated with a Symbol.
 */
static CGIRSYM
Interface_makeCGIRSYM(Interface this, const_Symbol symbol, CGIRCallBack callback)
{
  InterfaceMapped mapped = HTable_lookup(Interface_LIR2CGIR(this), (HTableKey)symbol, sizeof(InterfaceMapped_));
  CGIRSYM cgirsym = (CGIRSYM)(uintptr_t)InterfaceMapped_CGIR(mapped);
  //
  if (!Interface_isCGIRValid(this, symbol)) {
    const char *name = Symbol_name(symbol);
    bool isSpill = Symbol_isSpill(symbol);
    NativeType lai_nativeType = Symbol_nativeType(symbol);
    Interface_setCGIRValid(this, symbol);
    if (cgirsym == (CGIRSYM)0) {
      // Just created by HTable_lookup.
      cgirsym = (*CGIRCallBack_SYM_make(callback))((CGIRSYM)0, name, isSpill, lai_nativeType);
      // WARNING: cgirsym is now different from Symbol_INDEX!
      // Update CGIR -> LAO table.
      *(const_Symbol *)HTable_lookup(Interface_SYMBOLS(this), (HTableKey)(uintptr_t)cgirsym, sizeof(Symbol)) = symbol;
      InterfaceMapped_Ctor(mapped, (InterfaceCGIR)(uintptr_t)cgirsym, InterfaceType_Symbol);
    } else {
      cgirsym = (*CGIRCallBack_SYM_make(callback))(cgirsym, name, isSpill, lai_nativeType);
      Except_CHECK(InterfaceMapped_TYPE(mapped) == InterfaceType_Symbol);
      *InterfaceMapped__CGIR(mapped) = (InterfaceCGIR)(uintptr_t)cgirsym;
    }
  }
  Except_ENSURE(cgirsym != 0);
  return cgirsym;
}

/*
  Interface_makeCGIRTN -- Make the CGIRTN associated with a Temporary.
 */
static CGIRTN
Interface_makeCGIRTN(Interface this, const_Temporary temporary, CGIRCallBack callback)
{
  InterfaceMapped mapped = HTable_lookup(Interface_LIR2CGIR(this), (HTableKey)temporary, sizeof(InterfaceMapped_));
  CGIRTN cgirtn = (CGIRTN)(uintptr_t)InterfaceMapped_CGIR(mapped);
  //
  if (!Interface_isCGIRValid(this, temporary)) {
    Interface_setCGIRValid(this, temporary);
    if (cgirtn == (CGIRTN)0) {
      // Just created by HTable_lookup.
      if (Temporary_hasRegFile(temporary)) {
        RegFile regFile = Temporary_REGFILE(temporary);
        Register registre = Temporary_REGISTER(temporary);
        unsigned width = Temporary_WIDTH(temporary);
        if (Temporary_isAssigned(temporary)) {
          cgirtn = (*CGIRCallBack_TN_make(callback))((CGIRTN)0, CGIRTNType_Assigned, regFile, registre, width);
        } else if (Temporary_isDedicated(temporary)) {
          cgirtn = (*CGIRCallBack_TN_make(callback))((CGIRTN)0, CGIRTNType_Dedicated, regFile, registre, width);
        } else if (Temporary_isVirtual(temporary)) {
          cgirtn = (*CGIRCallBack_TN_make(callback))((CGIRTN)0, CGIRTNType_Virtual, regFile, registre, width);
        } else Except_NEVER(true);
      } else if (Temporary_hasLiteral(temporary)) {
        Literal literal = Temporary_LITERAL(temporary);
        if (Temporary_isAbsolute(temporary)) {
          int64_t value = Literal_VALUE(literal);
          cgirtn = (*CGIRCallBack_TN_make(callback))((CGIRTN)0, CGIRTNType_Absolute, value);
        } else if (Temporary_isSymbol(temporary)) {
          CGIRSYM cgirsym = Interface_makeCGIRSYM(this, Literal_SYMBOL(literal), callback);
          int64_t offset = Literal_OFFSET(literal);
          cgirtn = (*CGIRCallBack_TN_make(callback))((CGIRTN)0, CGIRTNType_Symbol, cgirsym, offset);
        } else if (Temporary_isLabel(temporary)) {
          CGIRLAB cgirlab = Interface_makeCGIRLAB(this, Literal_LABEL(literal), callback);
          cgirtn = (*CGIRCallBack_TN_make(callback))((CGIRTN)0, CGIRTNType_Label, cgirlab);
        } else Except_NEVER(true);
      } else if (Temporary_isModifier(temporary)) {
        ModifierMember member = Temporary_MEMBER(temporary);
        cgirtn = (*CGIRCallBack_TN_make(callback))((CGIRTN)0, CGIRTNType_Modifier, member);
      } else Except_NEVER(true);
      // Update CGIR -> LAO table.
      *(const_Temporary *)HTable_lookup(Interface_TEMPORARIES(this), (HTableKey)(uintptr_t)cgirtn, sizeof(Temporary)) = temporary;
      InterfaceMapped_Ctor(mapped, (InterfaceCGIR)cgirtn, InterfaceType_Temporary);
    } else {
      if (Temporary_isAssigned(temporary)) {
        RegFile regFile = Temporary_REGFILE(temporary);
        Register registre = Temporary_REGISTER(temporary);
        unsigned width = Temporary_WIDTH(temporary);
        cgirtn = (*CGIRCallBack_TN_make(callback))(cgirtn, CGIRTNType_Assigned, regFile, registre, width);
      } else if (Temporary_isVirtual(temporary)) {
        RegFile regFile = Temporary_REGFILE(temporary);
        Register registre = Temporary_REGISTER(temporary);
        unsigned width = Temporary_WIDTH(temporary);
        cgirtn = (*CGIRCallBack_TN_make(callback))(cgirtn, CGIRTNType_Virtual, regFile, registre, width);
      } else if (Temporary_isModifier(temporary)) {
        ModifierMember member = Temporary_MEMBER(temporary);
        cgirtn = (*CGIRCallBack_TN_make(callback))(cgirtn, CGIRTNType_Modifier, member);
      }
      Except_CHECK(InterfaceMapped_TYPE(mapped) == InterfaceType_Temporary);
      *InterfaceMapped__CGIR(mapped) = (InterfaceCGIR)cgirtn;
    }
  }
  Except_ENSURE(cgirtn != (CGIRTN)0);
  return cgirtn;
}

/**
 * Make the CGIROP associated with an Operation.
 */
static CGIROP
Interface_makeCGIROP(Interface this, const_Operation operation, CGIRCallBack callback)
{
  InterfaceMapped mapped = HTable_lookup(Interface_LIR2CGIR(this), (HTableKey)operation, sizeof(InterfaceMapped_));
  CGIROP cgirop = (CGIROP)(uintptr_t)InterfaceMapped_CGIR(mapped);
  //
  if (!Interface_isCGIRValid(this, operation)) {
    Memory memory =  Interface_MEMORY(this);
    Operator operator = Operation_operator(operation);
    int argCount = Operation_argCount(operation), i = 0;
    int resCount = Operation_resCount(operation), j = 0;
    int instance = Operation_instance(operation);
    int startDate = Operation_startDate(operation);
    const_Operation orig_operation = Operation_replicated(operation);
    CGIRTN *arguments = Memory_alloc(memory, sizeof(CGIRTN)*(argCount + 1));
    CGIRTN *results = Memory_alloc(memory, sizeof(CGIRTN)*(resCount + 1));
    CGIROP orig_op = (CGIROP)0;
    CGIRWN cgirwn = NULL;	//TODO!
    unsigned flags = 0;
    if (Operation_isMemorySafe(operation)) flags |= CGIROPFlags_MemorySafe;
    if (Operation_isHoisted(operation)) flags |= CGIROPFlags_Hoisted;
    if (Operation_isVolatile(operation)) flags |= CGIROPFlags_Volatile;
    if (Operation_isMemoryTouch(operation)) flags |= CGIROPFlags_MemoryTouch;
    if (Operation_isMemoryLevel2(operation)) flags |= CGIROPFlags_MemoryLevel2;
    if (Operation_isMemoryFence(operation)) flags |= CGIROPFlags_MemoryFence;
    if (Operation_isMemorySpill(operation)) flags |= CGIROPFlags_MemorySpill;
    if (Operation_isPureCopy(operation)) flags |= CGIROPFlags_PureCopy;
    if (orig_operation != NULL) {
      // orig_op not 0 in case orig_operation exists.
      Except_CHECK(orig_operation != operation);
      orig_op = Interface_makeCGIROP(this, orig_operation, callback);
    }
    //
#ifndef _NTRACE
    if (O64_OUTPUT) {
      fprintf(O64_OUTPUT, "// update OP: ");
      Operation_pretty(operation, O64_OUTPUT);
    }
#endif//_NTRACE
#ifdef MDT_TARGET_k1b
    if (operator == Operator_SKIP) operator = Operator_k1b_NOP_simple;//HACK!
    if (operator == Operator_COPY) operator = Operator_k1b_COPY_1singleReg_2singleReg_simple;//HACK!
#endif//MDT_TARGET_k1b
#ifdef MDT_TARGET_st200
    if (operator == Operator_SKIP) operator = Operator_st200_nop;//HACK!
    if (operator == Operator_COPY) operator = Operator_st200_mov_1general_2general;//HACK!
#endif//MDT_TARGET_st200
#ifdef MDT_TARGET_arm
    if (operator == Operator_SKIP) operator = Operator_arm_nop;//HACK!
    if (operator == Operator_COPY) operator = Operator_arm_moval_reg_1noPC_2general;//HACK!
#endif//MDT_TARGET_arm
    if (!Operator_isWriteList(operator)) resCount = Operator_writeCount(operator);
    if (!Operator_isReadList(operator)) argCount = Operator_readCount(operator);
    Operation_resizeOperands((Operation)operation, resCount, argCount);//HACK! cast.
    Operation_ARGUMENTS_FOREACH_Temporary(operation, temporary) {
      arguments[i++] = Interface_makeCGIRTN(this, temporary, callback);
    } Operation_ARGUMENTS_ENDEACH_Temporary;
    Except_CHECK(i <= argCount);
    arguments[i] = (CGIRTN)0;
    Operation_RESULTS_FOREACH_Temporary(operation, temporary) {
      results[j++] = Interface_makeCGIRTN(this, temporary, callback);
    } Operation_RESULTS_ENDEACH_Temporary;
    Except_CHECK(j <= resCount);
    results[j] = (CGIRTN)0;
    //
    Interface_setCGIRValid(this, operation);
    if (cgirop == (CGIROP)0) {
      // Just created by HTable_lookup.
      cgirop = (*CGIRCallBack_OP_make(callback))(cgirop, (Operator)operator, arguments, results, orig_op);
      // Update CGIR -> LAO table
      *(const_Operation *)HTable_lookup(Interface_OPERATIONS(this), (HTableKey)(uintptr_t)cgirop, sizeof(Operation)) = operation;
      InterfaceMapped_Ctor(mapped, (InterfaceCGIR)cgirop, InterfaceType_Operation);
    } else {
      cgirop = (*CGIRCallBack_OP_make(callback))(cgirop, (Operator)operator, arguments, results, orig_op);
      Except_CHECK(InterfaceMapped_TYPE(mapped) == InterfaceType_Operation);
      *InterfaceMapped__CGIR(mapped) = (InterfaceCGIR)cgirop;
      // Prevent redundant spill code processing.
      flags &= ~CGIROPFlags_MemorySpill;
    }
    (*CGIRCallBack_OP_more(callback))(cgirop, instance, startDate, flags);
    Memory_free_(memory, results);
    Memory_free_(memory, arguments);
  }
  Except_ENSURE(cgirop != (CGIROP)0);
  return cgirop;
}

/**
 * Make the CGIRBB associated with a BasicBlock.
 */
static CGIRBB
Interface_makeCGIRBB(Interface this, const_BasicBlock basicBlock, CGIRCallBack callback)
{
  InterfaceMapped mapped = HTable_lookup(Interface_LIR2CGIR(this), (HTableKey)basicBlock, sizeof(InterfaceMapped_));
  CGIRBB cgirbb = (CGIRBB)(uintptr_t)InterfaceMapped_CGIR(mapped);
  //
  if (!Interface_isCGIRValid(this, basicBlock)) {
    Memory memory =  Interface_MEMORY(this);
    Procedure procedure = Interface_PROCEDURE(this);
    int controlCount = 0;
    int labelCount = BasicBlock_labelCount(basicBlock), i = 0;
    int operationCount = BasicBlock_countOperations(basicBlock), j = 0;
    CGIRLAB *cgirlabels = Memory_alloc(memory, sizeof(CGIRLAB)*(labelCount + 1));
    CGIROP *cgiroperations = Memory_alloc(memory, sizeof(CGIROP)*(operationCount + 1));
    CGIRRID cgirrid = (CGIRRID)BasicBlock_regionId(basicBlock);
    float frequency = BasicBlock_frequency(basicBlock);
    intptr_t traceId = BasicBlock_traceId(basicBlock);
    int instance = BasicBlock_instance(basicBlock);
    LoopScope loopScope = BasicBlock_loopScope(basicBlock);
    BasicBlock headerBlock = NULL;
    CGIRBB loop_bb = (CGIRBB)0;
    unsigned flags = 0;
    if (BasicBlock_isAllocated(basicBlock)) flags |= CGIRBBFlags_Allocated;
    if (BasicBlock_isScheduled(basicBlock)) flags |= CGIRBBFlags_Scheduled;
    if (loopScope != NULL && LoopScope_isSimple(loopScope)) {
      headerBlock = LoopScope_headerBlock(loopScope);
    }
    //
    BasicBlock_FOREACH_Label(basicBlock, label) {
      cgirlabels[i++] = Interface_makeCGIRLAB(this, label, callback);
    } BasicBlock_ENDEACH_Label;
    cgirlabels[labelCount] = (CGIRLAB)0;
    Except_CHECK(i == labelCount);
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      cgiroperations[j++] = Interface_makeCGIROP(this, operation, callback);
      if (Operator_isControl(Operation_operator(operation))) ++controlCount;
    } BasicBlock_ENDEACH_Operation;
    cgiroperations[operationCount] = (CGIROP)0;
    Except_CHECK(j == operationCount);
    Except_CHECK(controlCount <= 1);
    //
    Interface_setCGIRValid(this, basicBlock);
    if (cgirbb == (CGIRBB)0) {
      // Just created by HTable_lookup.
      cgirbb = (*CGIRCallBack_BB_make(callback))((CGIRBB)0, cgirlabels, cgiroperations, cgirrid, frequency);
      // Update CGIR -> LAO table
      *(const_BasicBlock *)HTable_lookup(Interface_BASICBLOCKS(this), (HTableKey)(uintptr_t)cgirbb, sizeof(BasicBlock)) = basicBlock;
      InterfaceMapped_Ctor(mapped, (InterfaceCGIR)cgirbb, InterfaceType_BasicBlock);
    } else {
      cgirbb = (*CGIRCallBack_BB_make(callback))(cgirbb, cgirlabels, cgiroperations, cgirrid, frequency);
      Except_CHECK(InterfaceMapped_TYPE(mapped) == InterfaceType_BasicBlock);
      *InterfaceMapped__CGIR(mapped) = (InterfaceCGIR)cgirbb;
    }
    if (headerBlock == NULL) loop_bb = (CGIRBB)0;
    else if (headerBlock == basicBlock) loop_bb = cgirbb;
    else loop_bb = Interface_makeCGIRBB(this, headerBlock, callback);
    (*CGIRCallBack_BB_more(callback))(cgirbb, loop_bb, traceId, instance, flags);
    Memory_free_(memory, cgiroperations);
    Memory_free_(memory, cgirlabels);
  }
  Except_ENSURE(cgirbb != (CGIRBB)0);
  return cgirbb;
}

/**
 * Make the CGIRLD associated with a LoopScope.
 */
static CGIRLD
Interface_makeCGIRLD(Interface this, const_LoopScope loopScope, CGIRCallBack callback)
{
  InterfaceMapped mapped = HTable_lookup(Interface_LIR2CGIR(this), (HTableKey)loopScope, sizeof(InterfaceMapped_));
  CGIRLD cgirld = (CGIRLD)(uintptr_t)InterfaceMapped_CGIR(mapped);
  //
  if (!Interface_isCGIRValid(this, loopScope)) {
    Temporary tripCount = LoopScope_tripCount(loopScope);
    BasicBlock headerBlock = LoopScope_headerBlock(loopScope);
    CGIRBB header_cgirbb = Interface_makeCGIRBB(this, headerBlock, callback);
    CGIRTN trip_count_cgirtn = tripCount != NULL?  Interface_makeCGIRTN(this, tripCount, callback): NULL;
    int unrolled = LoopScope_unrolled(loopScope);
    Interface_setCGIRValid(this, loopScope);
    if (cgirld == (CGIRLD)0) {
      // Just created by HTable_lookup.
      cgirld = (*CGIRCallBack_LD_make(callback))((CGIRLD)0, header_cgirbb, trip_count_cgirtn, unrolled);
      // Fix for bugs/st200/pro-release-1-9-0-B/4.
      if (cgirld != (CGIRLD)0) {
        // Update CGIR -> LAO table.
        *(const_LoopScope *)HTable_lookup(Interface_LOOPSCOPES(this), (HTableKey)(uintptr_t)cgirld, sizeof(LoopScope)) = loopScope;
        InterfaceMapped_Ctor(mapped, (InterfaceCGIR)cgirld, InterfaceType_LoopScope);
      }
    } else {
      cgirld = (*CGIRCallBack_LD_make(callback))(cgirld, header_cgirbb, trip_count_cgirtn, unrolled);
      Except_CHECK(InterfaceMapped_TYPE(mapped) == InterfaceType_LoopScope);
      *InterfaceMapped__CGIR(mapped) = (InterfaceCGIR)cgirld;
    }
  }
  //Except_ENSURE(cgirld != (CGIRLD)0);
  return cgirld;
}

/**
 * Construct and fill the LIR2CGIR HTable.
 */
static void
Interface_fillLIR2CGIR(Interface this)
{
  Interface_FOREACH_Label(this, cgirlab, label) {
    InterfaceMapped mapped = HTable_insert(Interface_LIR2CGIR(this), (HTableKey)label, sizeof(InterfaceMapped_));
    InterfaceMapped_Ctor(mapped, (InterfaceCGIR)(uintptr_t)cgirlab, InterfaceType_Label);
  } Interface_ENDEACH_Label;
  Interface_FOREACH_Symbol(this, cgirsym, symbol) {
    InterfaceMapped mapped = HTable_insert(Interface_LIR2CGIR(this), (HTableKey)symbol, sizeof(InterfaceMapped_));
    InterfaceMapped_Ctor(mapped, (InterfaceCGIR)(uintptr_t)cgirsym, InterfaceType_Symbol);
  } Interface_ENDEACH_Symbol;
  Interface_FOREACH_Temporary(this, cgirtn, temporary) {
    InterfaceMapped mapped = HTable_insert(Interface_LIR2CGIR(this), (HTableKey)temporary, sizeof(InterfaceMapped_));
    InterfaceMapped_Ctor(mapped, (InterfaceCGIR)(uintptr_t)cgirtn, InterfaceType_Temporary);
  } Interface_ENDEACH_Temporary;
  Interface_FOREACH_Operation(this, cgirop, operation) {
    InterfaceMapped mapped = HTable_insert(Interface_LIR2CGIR(this), (HTableKey)operation, sizeof(InterfaceMapped_));
    InterfaceMapped_Ctor(mapped, (InterfaceCGIR)(uintptr_t)cgirop, InterfaceType_Operation);
  } Interface_ENDEACH_Operation;
  Interface_FOREACH_BasicBlock(this, cgirbb, basicBlock) {
    InterfaceMapped mapped = HTable_insert(Interface_LIR2CGIR(this), (HTableKey)basicBlock, sizeof(InterfaceMapped_));
    InterfaceMapped_Ctor(mapped, (InterfaceCGIR)(uintptr_t)cgirbb, InterfaceType_BasicBlock);
  } Interface_ENDEACH_BasicBlock;
  Interface_FOREACH_LoopScope(this, cgirld, loopScope) {
    InterfaceMapped mapped = HTable_insert(Interface_LIR2CGIR(this), (HTableKey)loopScope, sizeof(InterfaceMapped_));
    InterfaceMapped_Ctor(mapped, (InterfaceCGIR)(uintptr_t)cgirld, InterfaceType_LoopScope);
  } Interface_ENDEACH_LoopScope;
}

/**
 * Find the CGIRBB associated with a BasicBlock.
 */
static inline CGIRBB
Interface_findCGIRBB(Interface this, BasicBlock basicBlock)
{
  InterfaceMapped mapped = HTable_search(Interface_LIR2CGIR(this), (HTableKey)basicBlock);
  CGIRBB cgirbb = (CGIRBB)(uintptr_t)InterfaceMapped_CGIR(mapped);
  Except_CHECK(InterfaceMapped_TYPE(mapped) == InterfaceType_BasicBlock);
  return cgirbb;
}

#ifdef $XCC_h
/**
 * Update the CGIR from the LAO Interface.
 */
void
Interface_updateCGIR(Interface this, CGIRCallBack callback);
#endif//$XCC_h

void
Interface_updateCGIR(Interface this, CGIRCallBack callback)
{
  if (callback != NULL) {
    Procedure procedure = Interface_PROCEDURE(this);
    CGIRBB first_cgirbb = NULL, prev_cgirbb = NULL;
    //
    // Fill the LIR2CGIR table.
    Interface_fillLIR2CGIR(this);
    //
    // For each LIR BasicBlock, update the corresponding BB. This will recursively
    // trigger updates of OPs, TNs, etc. For the BasicBlocks that were created
    // through this Interface, unchain and unlink in the CGIR the corresponding BBs.
#ifndef _NTRACE
    O64_INPUT && fprintf(O64_INPUT, "  Interface_updateCGIR(interface, NULL);\n");
#endif//_NTRACE
    Procedure_FOREACH_BasicBlock(procedure, basicBlock) {
      CGIRBB cgirbb = Interface_makeCGIRBB(this, basicBlock, callback);
      if (Interface_isCGIRImport(this, basicBlock)) {
        (*CGIRCallBack_BB_unlink(callback))(cgirbb,	
            !BasicBlock_isEntry(basicBlock), !BasicBlock_isExit(basicBlock));
        if (first_cgirbb == NULL) first_cgirbb = cgirbb;
        else (*CGIRCallBack_BB_unchain(callback))(cgirbb);
      }
    } Procedure_ENDEACH_BasicBlock;
    //
    // Traverse the LIR BasicBlocks to update the CGIRLD and restore the CGIRBB chains.
    Procedure_FOREACH_BasicBlock(procedure, basicBlock) {
      CGIRBB cgirbb = Interface_findCGIRBB(this, basicBlock);
      LoopScope loopScope = BasicBlock_loopScope(basicBlock);
      if (loopScope != NULL && LoopScope_isSimple(loopScope)) {
        Interface_makeCGIRLD(this, loopScope, callback);
      }
      if (cgirbb != first_cgirbb) {
        (*CGIRCallBack_BB_chain(callback))(prev_cgirbb, cgirbb);
      }
      prev_cgirbb = cgirbb;
    } Procedure_ENDEACH_BasicBlock;
    //
    // Restore the CGIRBB links from the BasicBlockEdge(s).
    Procedure_FOREACH_BasicBlock(procedure, tailBlock) {
      CGIRBB tail_bb = Interface_findCGIRBB(this, tailBlock);
      BasicBlock_LEAVE_FOREACH_BasicBlockEdge(tailBlock, edge) {
        BasicBlock destBlock = BasicBlockEdge_destBlock(edge);
        float probability = BasicBlockEdge_probability(edge);
        if (BasicBlock_index(destBlock) != 0) {
          CGIRBB head_bb = Interface_findCGIRBB(this, destBlock);
          (*CGIRCallBack_BB_link(callback))(tail_bb, head_bb, probability);
        } else {
          // FIXME! There should be no CodeRegion rootBlock left here.
        }
      } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    } Procedure_ENDEACH_BasicBlock;
    //
    // Traverse the discarded BasicBlock(s).
    Procedure_DISCARDED_FOREACH_BasicBlock(procedure, basicBlock) {
      if (Interface_isCGIRImport(this, basicBlock)) {
        CGIRBB cgirbb = Interface_findCGIRBB(this, basicBlock);
        if (cgirbb != NULL) (*CGIRCallBack_BB_discard(callback))(cgirbb);
      }
    } Procedure_DISCARDED_ENDEACH_BasicBlock;
  }
}

#ifdef $XCC_h
/*--------------------- LAO Interface Control Functions ----------------------*/
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Initialize the interface, must be called once per process.
 */
void
Interface_Initialize(void);
#endif//$XCC_h

void
Interface_Initialize(void)
{
  O64_Initialize();
}

#ifdef $XCC__h
extern int Interface_Counter;
#endif//$XCC__h

int Interface_Counter = 0;

#ifdef $XCC_h
/**
 * Open this Interface.
 */
void
Interface_open(Interface this, const char *name, Processor processor, Convention convention, ConfigureItem item, ...);
#endif//$XCC_h

void
Interface_open(Interface this, const char *name, Processor processor, Convention convention, ConfigureItem item, ...)
{
  va_list va;
  int lir2cgir_estimate = 2000;
  int label_estimate = 10;
  int symbol_estimate = 20;
  int temporary_estimate = 1000;
  int operation_estimate = 500;
  int basicBlock_estimate = 20;
  int loopScope_estimate = 10;
  int cgirImport_estimate = 20;
  int cgirValid_estimate = 2000;
  Program program = NULL;
  //
  if (GETENV("LAO_PID")) {
    static int dummy = -1;
    if (dummy <= 0) {
      fprintf(stderr, "LAO_PID=%d\n", getpid());
      scanf("%d", &dummy);
    }
  }
  O64_Open();
  program = Program_new(Interface_MEMORY(this), processor, convention);
  *Interface__CONFIGURE(this) = Program_configure(program);
  *Interface__PROGRAM(this) = program;
  //
  va_start(va, item);
  Configure_loadVA(Interface_CONFIGURE(this), item, va);
  va_end(va);
  //
  //
#ifndef _NTRACE
  O64_INPUT && !Interface_Counter && fprintf(O64_INPUT, "}\n");
  O64_INPUT && fprintf(O64_INPUT, "\nvoid Interface_%d(void) {\n", ++Interface_Counter);
  O64_INPUT && fprintf(O64_INPUT, "  Interface_open(interface, \"%s\", Processor_%s, Convention_%s, ",
                                   name, Processor_Id(processor), Convention_Id(convention));
  O64_INPUT && Configure_print(Interface_CONFIGURE(this), O64_INPUT);
  O64_INPUT && fprintf(O64_INPUT, ");\n");
#endif//_NTRACE
  //
  Except_REQUIRE(Interface_PROCEDURE(this) == NULL);
  {
    Symbol symbol = SymbolTable_makeSymbol(Program_symbolTable(program), name);
    Procedure procedure = Program_makeProcedure(program, symbol);
    Selector_Ctor(Interface_SELECTOR(this), Procedure_memory(procedure), procedure, false);
  }
  //
  {
    Memory memory = Interface_MEMORY(this);
    HTable_Ctor(Interface_LIR2CGIR(this), memory, lir2cgir_estimate);
    HTable_Ctor(Interface_LABELS(this), memory, label_estimate);
    HTable_Ctor(Interface_SYMBOLS(this), memory, symbol_estimate);
    HTable_Ctor(Interface_TEMPORARIES(this), memory, temporary_estimate);
    HTable_Ctor(Interface_OPERATIONS(this), memory, operation_estimate);
    HTable_Ctor(Interface_BASICBLOCKS(this), memory, basicBlock_estimate);
    HTable_Ctor(Interface_LOOPSCOPES(this), memory, loopScope_estimate);
    HTable_Ctor(Interface_CGIRIMPORT(this), memory, cgirImport_estimate);
    HTable_Ctor(Interface_CGIRVALID(this), memory, cgirValid_estimate);
  }
}

#ifdef $XCC_h
/**
 * Apply the LAO optimizations through this Interface.
 */
unsigned
Interface_optimize(Interface this, OptimizeItem item, ...);
#endif//$XCC_h

unsigned
Interface_optimize(Interface this, OptimizeItem item, ...)
{
  va_list va;
  unsigned optimized = 0;
  OptimizeFlags activation = 0;
  OptimizeFlags conversion = 0;
  OptimizeFlags coalescing = 0;
  OptimizeFlags rewriting = 0;
  OptimizeFlags allocation = 0;
  OptimizeFlags scheduling = 0;
  Program program = Interface_PROGRAM(this);
  Procedure procedure = Interface_PROCEDURE(this);
  Memory memory = Procedure_memory(procedure);
  Optimize optimize = Program_optimize(program);
  va_start(va, item);
  Optimize_loadVA(Program_optimize(program), item, va);
  va_end(va);
  if (Interface_DEDICATED > 0) {
    *Optimize__CONVERSION(optimize) |= OptimizeConversion_Dedicated;
  }
  activation = Optimize_ACTIVATION(optimize);
  conversion = Optimize_CONVERSION(optimize);
  coalescing = Optimize_COALESCING(optimize);
  rewriting = Optimize_REWRITING(optimize);
  allocation = Optimize_ALLOCATION(optimize);
  scheduling = Optimize_SCHEDULING(optimize);
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT, "  Interface_optimize(interface, ");
  O64_INPUT && Optimize_print(Program_optimize(program), O64_INPUT);
  O64_INPUT && fprintf(O64_INPUT, ");\n");
#endif//_NTRACE
  Procedure_buildCodeRegions(procedure);
  Procedure_fixupOpen64(procedure, activation);
  if (   (conversion & OptimizeConversion_Dedicated) != 0
      && (activation & (  OptimizeActivation_SSAForm
                        | OptimizeActivation_RegAlloc)) != 0) {
    Procedure_fixupDedicated(procedure, activation);
#if !_OPTIMIZE
    Interface_FOREACH_Temporary(this, cgirtn, temporary) {
      Except_CHECK(!Temporary_isAssigned(temporary));
    } Interface_ENDEACH_Temporary;
#endif//_OPTIMIZE
  }
  Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
    // Create the loop nesting forest.
    LoopForest_delete(LoopForest_new(memory, codeRegion));
    // SSAForm OptimizationFlags.
    if ((activation & OptimizeActivation_SSAForm) != 0) {
      // SSA analyses.
      if ((conversion & ~OptimizeConversion_Dedicated) != 0) {
        uint32_t temporaryCounter = IndexedFactory_COUNTER(Temporary_Factory);
        uint32_t operationCounter = IndexedFactory_COUNTER(Operation_Factory);
        SSAForm ssaForm = SSAForm_construct(memory, codeRegion, false);
        PFA_TRACE && fprintf(PFA_TRACE, "*** SSA Conversion=")
                  && OptimizeConversion_prettyFlags(conversion, PFA_TRACE)
                  && fprintf(PFA_TRACE, "\n");
        PFA_TRACE && fprintf(PFA_TRACE, "*** SSA Coalescing=")
                  && OptimizeCoalescing_prettyFlags(coalescing, PFA_TRACE)
                  && fprintf(PFA_TRACE, "\n");
        if (rewriting != 0) {
          Accelerator accelerator = Accelerator_new(memory, codeRegion);
          Accelerator_optimize(accelerator);
          Accelerator_delete(accelerator);
        }
        SSAForm_destruct(ssaForm);
        *IndexedFactory__COUNTER(Operation_Factory) = operationCounter;
        *IndexedFactory__COUNTER(Temporary_Factory) = temporaryCounter;
      }
    }
    // PrePass OptimizationFlags.
    if ((activation & OptimizeActivation_PrePass) != 0) {
      // Create the CodeLayout.
      //CodeLayout_delete(CodeLayout_new(memory, codeRegion));
      // PrePass schedule.
      if (scheduling != 0) {
        int scheduled = 0, bodyCount = 0;
        TemporaryTable globalTable = CodeRegion_makeGlobalTable(codeRegion, TemporaryFlag_Constant, 0);
        Liveness liveness = Liveness_make(memory, codeRegion, globalTable, false);
        RegionFlow regionFlow = RegionFlow_new(memory, codeRegion);
        BlockTraceFlags flags = BlockTraceFlag_PrePass;
        RegionFlowCover regionCover = RegionFlowCover_new(memory, regionFlow, flags);
        SCD_TRACE && fprintf(SCD_TRACE, "*** BEFORE PRESCHEDULER\n")
                  && CodeRegion_pretty(codeRegion, SCD_TRACE);
        RegionFlowCover_FOREACH_BlockTrace(regionCover, blockTrace) {
          SCD_TRACE && fprintf(SCD_TRACE, "*** SCHEDULING ");
          SCD_TRACE && BlockTrace_pretty(blockTrace, SCD_TRACE);
          if (BlockTrace_isEmpty(blockTrace)) {
            SCD_TRACE && fprintf(SCD_TRACE, "    EMPTY\n");
          } else {
            Scheduler scheduler = Scheduler_new(memory, blockTrace);
            optimized |= Scheduler_schedule(scheduler, regionFlow);
            scheduled += Scheduler_scheduled(scheduler);
            bodyCount += Scheduler_bodyCount(scheduler);
            Scheduler_delete(scheduler);
          }
        } RegionFlowCover_ENDEACH_BlockTrace;
        RegionFlowCover_delete(regionCover);
        RegionFlow_reorderBasicBlocks(regionFlow);
        RegionFlow_finalize(regionFlow);
        RegionFlow_delete(regionFlow);
        Liveness_kill(liveness);
        // There may be some blocks created during scheduling which now also need to be scheduled.
        liveness = Liveness_make(memory, codeRegion, globalTable, false);
        regionFlow = RegionFlow_new(memory, codeRegion);
        CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
          if (!BasicBlock_isEmpty(basicBlock) && !BasicBlock_isScheduled(basicBlock)) {
            // Create a blockTrace containing just this block, and schedule it.
            CFGNode cfgNode = BasicBlock_cfgNode(basicBlock);
            BlockTrace blockTrace;
            Scheduler scheduler;
            LoopScope loopScope = BasicBlock_loopScope(basicBlock);
            if (loopScope == NULL) {
              loopScope = Procedure_findLoopScope (procedure, NULL);
            }
            blockTrace  = BlockTrace_createSingleton(memory, regionFlow,cfgNode, loopScope, flags);
            scheduler = Scheduler_new(memory, blockTrace);
            optimized |= Scheduler_schedule(scheduler, regionFlow);
            scheduled += Scheduler_scheduled(scheduler);
            bodyCount += Scheduler_bodyCount(scheduler);
            Scheduler_delete(scheduler);
          }
        } CodeRegion_ENDEACH_BasicBlock;
        RegionFlow_reorderBasicBlocks(regionFlow);
        RegionFlow_finalize(regionFlow);
        RegionFlow_delete(regionFlow);
        scheduled && bodyCount &&
        SCD_TRACE && fprintf(SCD_TRACE, "*** PRESCHEDULED=%d\tBODYCOUNT=%d\tPERCENT=%2.4g\n",
                             scheduled, bodyCount, (float)scheduled*100/(float)bodyCount);
        SCD_TRACE && fprintf(SCD_TRACE, "*** AFTER PRESCHEDULER\n")
                  && CodeRegion_pretty(codeRegion, SCD_TRACE);
        //SCD_TRACE && CodeRegion_prettyPressures(codeRegion, SCD_TRACE);
        Liveness_kill(liveness);
      }
    }
    // RegAlloc OptimizationFlags.
    if (   (activation & OptimizeActivation_RegAlloc) != 0
        && (conversion & ~OptimizeConversion_Dedicated) != 0) {
      // Register allocation.
      if ((allocation & OptimizeAllocation_ELinearScan) != 0) {
        SSAForm ssaForm = SSAForm_construct(memory, codeRegion, false);
        ELinearScan linearScan = ELinearScan_new(memory, ssaForm);
        ELinearScan_allocate(linearScan);
        ELinearScan_delete(linearScan);
        SSAForm_destruct(ssaForm);
      } else
       if ((allocation & OptimizeAllocation_DomTreeScan) != 0) {
      } else
      if ((allocation & OptimizeAllocation_GraphColoring) != 0) {
      }
    }
    // PostPass OptimizationFlags.
    if ((activation & OptimizeActivation_PostPass) != 0) {
#if !_OPTIMIZE
      Interface_FOREACH_Temporary(this, cgirtn, temporary) {
        if (Temporary_isVariable(temporary)) {
          Except_CHECK(Temporary_hasRegister(temporary));
        }
      } Interface_ENDEACH_Temporary;
#endif//_OPTIMIZE
      // PostPass schedule.
      if (scheduling != 0) {
        TemporaryTable globalTable = CodeRegion_makeGlobalTable(codeRegion, TemporaryFlag_Constant, 0);
        Liveness liveness = Liveness_make(memory, codeRegion, globalTable, false);
        RegionFlow regionFlow = RegionFlow_new(memory, codeRegion);
        RegionFlowCover regionCover = RegionFlowCover_new(memory, regionFlow, 0);
        SCD_TRACE && fprintf(SCD_TRACE, "*** BEFORE POSTSCHEDULER\n")
                  && CodeRegion_pretty(codeRegion, SCD_TRACE);
        RegionFlowCover_FOREACH_BlockTrace(regionCover, blockTrace) {
          SCD_TRACE && fprintf(SCD_TRACE, "*** SCHEDULING ");
          SCD_TRACE && BlockTrace_pretty(blockTrace, SCD_TRACE);
          if (BlockTrace_isEmpty(blockTrace)) {
            SCD_TRACE && fprintf(SCD_TRACE, "    EMPTY\n");
          } else {
            Scheduler scheduler = Scheduler_new(memory, blockTrace);
            optimized |= Scheduler_schedule(scheduler, regionFlow);
            Scheduler_delete(scheduler);
          }
        } RegionFlowCover_ENDEACH_BlockTrace;
        RegionFlowCover_delete(regionCover);
        RegionFlow_reorderBasicBlocks(regionFlow);
        RegionFlow_finalize(regionFlow);
        RegionFlow_delete(regionFlow);
        SCD_TRACE && fprintf(SCD_TRACE, "*** AFTER POSTSCHEDULER\n")
                  && CodeRegion_pretty(codeRegion, SCD_TRACE);
        //SCD_TRACE && CodeRegion_prettyPressures(codeRegion, SCD_TRACE);
        Liveness_kill(liveness);
      }
    }
    // Encode OptimizationFlags.
    if ((activation & OptimizeActivation_Encode) != 0) {
      int32_t blockCount = CodeRegion_blockCount(codeRegion);
      NativeStream nativeStream = NativeStream_new(memory, blockCount);
      CodeRegion_addtoNativeStream(codeRegion, nativeStream);
      // PostPass scheduling.
      if (Optimize_SCOREBOARDING(optimize) != 0) {
        SCD_TRACE && fprintf(SCD_TRACE, "*** BEFORE SCOREBOARDER\n")
                  && CodeRegion_pretty(codeRegion, SCD_TRACE);
        PostPass_delete(PostPass_new(memory, codeRegion));
        SCD_TRACE && fprintf(SCD_TRACE, "*** AFTER SCOREBOARDER\n")
                  && CodeRegion_pretty(codeRegion, SCD_TRACE);
        NativeStream_assumeScheduled(nativeStream);
      } else {
        NativeStream_assumeUnscheduled(nativeStream);
      }
      {
        size_t codeSize = NativeStream_getCodeSize(nativeStream, 0);
        void *codeBuffer = Memory_alloc(memory, codeSize);
        NativeStream_encode(nativeStream, 0, codeBuffer);
        LIR_TRACE && NativeStream_pretty(nativeStream, LIR_TRACE);
        Memory_free_(memory, codeBuffer);
      }
      NativeStream_delete(nativeStream);
    }
  } Procedure_ENDEACH_CodeRegion;
  return optimized;
}

#ifdef $XCC_h
/**
 * Close this Interface.
 */
void
Interface_close(Interface this);
#endif//$XCC_h

void
Interface_close(Interface this)
{
#ifndef _NDTOR
  HTable_Dtor(Interface_CGIRVALID(this));
  HTable_Dtor(Interface_CGIRIMPORT(this));
  HTable_Dtor(Interface_LOOPSCOPES(this));
  HTable_Dtor(Interface_BASICBLOCKS(this));
  HTable_Dtor(Interface_OPERATIONS(this));
  HTable_Dtor(Interface_TEMPORARIES(this));
  HTable_Dtor(Interface_SYMBOLS(this));
  HTable_Dtor(Interface_LABELS(this));
  HTable_Dtor(Interface_LIR2CGIR(this));
#endif//_NDTOR
  //
  Selector_Dtor(Interface_SELECTOR(this));
  memset(Interface_SELECTOR(this), 0, sizeof(Selector_));
  //
#ifndef _NTRACE
  O64_INPUT && fprintf(O64_INPUT, "  Interface_close(interface);\n");
  O64_INPUT && fprintf(O64_INPUT, "}\n\n");
#endif//_NTRACE
  //
  Program_delete(Interface_PROGRAM(this));
  *Interface__CONFIGURE(this) = NULL;
  *Interface__PROGRAM(this) = NULL;
  O64_Close();
}

#ifdef $XCC_h
/**
 * Finalize the interface, must be called once per process.
 */
void
Interface_Finalize(void);
#endif//$XCC_h

void
Interface_Finalize(void)
{
  O64_Finalize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Interface module.
 */
void
Interface_INIT(void);
#endif//$XCC__h

void
Interface_INIT(void)
{
  Memory memory = Memory_new(Memory_Root, 0);
  // Get environment
#ifndef Interface_DEDICATED
  if (GETENV("Interface_DEDICATED")) {
    sscanf(GETENV("Interface_DEDICATED"), "%d", &Interface_DEDICATED);
    fprintf(stderr, "*** Interface_DEDICATED=%d\n", Interface_DEDICATED);
  }
#endif
  Interface_Ctor(interface, memory);
}

#ifdef $XCC__h
/**
 * Finalize the Interface module.
 */
void
Interface_FINI(void);
#endif//$XCC__h

void
Interface_FINI(void)
{
  Memory memory = Interface_MEMORY(interface);
  Interface_Dtor(interface);
  Memory_delete(memory);
}

#if XCC__C
static void
Interface_TEST(void)
{
#include "O64/Interface_.c"
}

int
main(int argc, char **argv)
{
  O64_Test(Interface_TEST);
  return 0;
}
#endif

