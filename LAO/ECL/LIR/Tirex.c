#ifdef $XCC_h
/*
 * !!!!	Tirex.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dinechin@kalray.eu).
 * Duco van Amstel (duco.van-amstel@kalray.eu).
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
 * @ingroup LIR
 * @brief Tirex parser.
 */
#endif//$XCC_h

#include "MDT/MDT_.h"
#include "LIR/LIR_.h"
#include <ctype.h>
#include <stdlib.h>

#ifdef $XCC__h
#include "LIR/Tirex.h"
#endif//$XCC__h

#ifdef $XCC_h
#ifdef YAML_ENABLED
#include "yaml.h"
#else
typedef int yaml_parser_t;
typedef int yaml_event_t;
typedef int yaml_event_type_t;
typedef int yaml_mark_t;
#endif//YAML_ENABLED
#endif//$XCC_h

#ifdef $XCC_h
/**
 * File handle used to print the Tirex traces.
 */
extern FILE *TIREX_TRACE;
#ifdef _NTRACE
#define TIREX_TRACE NULL
#endif//_NTRACE
#endif//$XCC_h

#undef TIREX_TRACE
FILE *TIREX_TRACE = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Emulate error() and error_at_line() using macros.
 */
#define error(status, errnum, format, ...) \
{ \
  fflush(stdout); \
  if (errnum) perror(NULL); \
  fprintf(stderr, format, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  exit(status); \
}
#define error_at_line(status, errnum, filename, linenum, format, ...) \
{ \
  fflush(stdout); \
  if (errnum) perror(NULL); \
  fprintf(stderr, "%s:%u: ", filename, linenum); \
  fprintf(stderr, format, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  exit(status); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Tirex parser context.
 */
struct Tirex_;
#endif//$XCC_h

#ifdef $XCC__h
#define Tirex_KEYLENGTH 14
#endif//$XCC__h

#ifdef $XCC__h
struct Tirex_ {
  //@args	Memory memory, FILE *inputFile
  Memory MEMORY;			//!< This Tirex Memory.
  const char *INPUTNAME;
  FILE *INPUTFILE;
  yaml_parser_t PARSER[1];
  yaml_event_t EVENT[1];
  //@access EVENT_TYPE	(Tirex_EVENT(this)->type)
  //@access EVENT_ANCHOR	(const char *)(Tirex_EVENT(this)->data.alias.anchor)
  //@access EVENT_VALUE	(const char *)(Tirex_EVENT(this)->data.scalar.value)
  //@access EVENT_LENGTH (size_t)(Tirex_EVENT(this)->data.scalar.length)
  StringTable_ STRINGS[1];
  HTable_ TEMPORARIES[1];
  HTable_ BASICBLOCKS[1];
  HTable_ LOOPSCOPES[1];
  Program PROGRAM;
  //@access PROCESSOR	Program_PROCESSOR(Tirex_PROGRAM(this))
  //@access OPTIMIZE	Program_optimize(Tirex_PROGRAM(this))
  //@access CONVENTION Program_convention(Tirex_PROGRAM(this))
  Procedure PROCEDURE;
  char KEY[Tirex_KEYLENGTH + 4];
  const char *VALUE;
  int STREAMSTATE;
};
#endif//$XCC__h

Tirex
Tirex_Ctor(Tirex this,
           Memory memory, FILE *inputFile)
{
  *Tirex__MEMORY(this) = memory;
  *Tirex__INPUTNAME(this) = "-";
  *Tirex__INPUTFILE(this) = inputFile;
#ifdef YAML_ENABLED
  if (!yaml_parser_initialize(Tirex_PARSER(this))) {
    Tirex_ERROR(this, "yaml_parser_initialize failure");
  }
  yaml_parser_set_input_file(Tirex_PARSER(this), inputFile);
#endif//YAML_ENABLED
  memset(Tirex_EVENT(this), 0, sizeof(yaml_event_t));
  StringTable_Ctor(Tirex_STRINGS(this), memory, true);
  HTable_Ctor(Tirex_TEMPORARIES(this), memory, 512);
  HTable_Ctor(Tirex_BASICBLOCKS(this), memory, 512);
  HTable_Ctor(Tirex_LOOPSCOPES(this), memory, 512);
  *Tirex__PROGRAM(this) = 0;
  *Tirex__PROCEDURE(this) = 0;;
  memset(Tirex_KEY(this), 0, Tirex_KEYLENGTH + 4);
  *Tirex__VALUE(this) = NULL;
  *Tirex__STREAMSTATE(this) = 0;
  return this;
}

void
Tirex_Dtor(Tirex this)
{
  HTable_Dtor(Tirex_LOOPSCOPES(this));
  HTable_Dtor(Tirex_BASICBLOCKS(this));
  HTable_Dtor(Tirex_TEMPORARIES(this));
  StringTable_Dtor(Tirex_STRINGS(this));
#ifdef YAML_ENABLED
  yaml_parser_delete(Tirex_PARSER(this));
#endif//YAML_ENABLED
}


#ifdef $XCC_h
void
Tirex_resetHTables(Tirex this);
/**
 * Called at the start of parsing a new procedure (function)
 */
#endif//$XCC_h

void
Tirex_resetHTables(Tirex this)
{
  Memory memory = Tirex_MEMORY(this);
  /* Destroy the old procedures HTables */
  HTable_Dtor(Tirex_LOOPSCOPES(this));
  HTable_Dtor(Tirex_BASICBLOCKS(this));
  HTable_Dtor(Tirex_TEMPORARIES(this));
  /* Initialize the HTables for the new procedure */
  HTable_Ctor(Tirex_TEMPORARIES(this), memory, 512);
  HTable_Ctor(Tirex_BASICBLOCKS(this), memory, 512);
  HTable_Ctor(Tirex_LOOPSCOPES(this), memory, 512);
}

size_t
Tirex_Size(Memory memory, FILE *inputFile)
{
  return sizeof(Tirex_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Tirex_)\t%zu\n", sizeof(Tirex_));
}
#endif//$XCC__c

#ifdef $XCC_h
Tirex
Tirex_new(Memory parent, FILE *inputFile);
#endif//$XCC_h

Tirex
Tirex_new(Memory parent, FILE *inputFile)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Tirex_Size(memory, inputFile);
  void *alloc = Memory_alloc(memory, size);
  return Tirex_Ctor(alloc, memory, inputFile);
}

#ifdef $XCC_h
Tirex
Tirex_delete(Tirex this);
#endif//$XCC_h

Tirex
Tirex_delete(Tirex this)
{
  if (this != NULL) {
    Memory memory = Tirex_MEMORY(this);
#ifndef _NDTOR
    Tirex_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Return the program associated with this Tirex object
 */
Program
Tirex_program(Tirex this);
#endif//$XCC_h

Program
Tirex_program(Tirex this)
{
  return Tirex_PROGRAM(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static inline Register
Tirex_makeRegister(Tirex this, const char *name)
{
  Processor processor = Procedure_processor(Tirex_PROCEDURE(this));
  Register registre = Register_fromName(name, processor);
  if (registre == Register__UNDEF)
    Tirex_ERROR(this, "unknown register name '%s'");
  return registre;
}

static Label
Tirex_makeLabel(Tirex this, const char *name)
{
  StringTable strings = Tirex_STRINGS(this);
  String key = StringTable_lookup(strings, name, strlen(name) + 1);
  Label *_label = HTable_lookup(Tirex_BASICBLOCKS(this), (HTableKey)key, sizeof(Label));
  if (*_label == NULL) {
    Program program = Tirex_PROGRAM(this);
    LabelTable labelTable = Program_labelTable(program);
    Label label = LabelTable_lookup(labelTable, LabelTable_makeIndex(labelTable), name);
    *_label = label;
  }
  return *_label;
}

static Symbol
Tirex_makeSymbol(Tirex this, const char *name)
{
  StringTable strings = Tirex_STRINGS(this);
  String key = StringTable_lookup(strings, name, strlen(name) + 1);
  Symbol *_symbol = HTable_lookup(Tirex_BASICBLOCKS(this), (HTableKey)key, sizeof(Symbol));
  if (*_symbol == NULL) {
    Program program = Tirex_PROGRAM(this);
    SymbolTable symbolTable = Program_symbolTable(program);
    Symbol symbol = SymbolTable_lookup(symbolTable, SymbolTable_makeIndex(symbolTable), name);
    *_symbol = symbol;
  }
  return *_symbol;
}

static inline Temporary
Tirex_makeDedicatedTemporary(Tirex this, const char *name)
{
  Procedure procedure = Tirex_PROCEDURE(this);
  Temporary_ *dedicated = Procedure_dedicated(procedure);
  Register registre = Tirex_makeRegister(this, name);
  return dedicated + registre;
}

static Temporary
Tirex_makeAssignedTemporary(Tirex this, const char *name, Register registre)
{
  StringTable strings = Tirex_STRINGS(this);
  String key = StringTable_lookup(strings, name, strlen(name) + 1);
  Temporary *_temporary = HTable_lookup(Tirex_TEMPORARIES(this), (HTableKey)key, sizeof(Temporary));
  if (*_temporary == NULL) {
    Procedure procedure = Tirex_PROCEDURE(this);
    Temporary temporary = Procedure_makeAssignedTemporary(procedure, registre);
    *_temporary = temporary;
  }
  if (!Temporary_isAssigned(*_temporary))
    Tirex_ERROR(this, "operand '%s' already exists with a differenty type");
  return *_temporary;
}

static Temporary
Tirex_makeVirtualTemporary(Tirex this, const char *name)
{
  StringTable strings = Tirex_STRINGS(this);
  String key = StringTable_lookup(strings, name, strlen(name) + 1);
  Temporary *_temporary = HTable_lookup(Tirex_TEMPORARIES(this), (HTableKey)key, sizeof(Temporary));
  if (*_temporary == NULL) {
    Procedure procedure = Tirex_PROCEDURE(this);
    Temporary temporary = Procedure_makeVirtualTemporary(procedure, RegFile__UNDEF);
    *_temporary = temporary;
  }
  if (!Temporary_isVirtual(*_temporary))
    Tirex_ERROR(this, "operand '%s' already exists with a differenty type");
  return *_temporary;
}

static Temporary
Tirex_makeAbsoluteTemporary(Tirex this, const char *name)
{
  int64_t value = strtoull(name, NULL, 0);
  Procedure procedure = Tirex_PROCEDURE(this);
  Temporary temporary = Procedure_makeAbsoluteTemporary(procedure, value, Immediate__UNDEF);
  return temporary;
}

static Temporary
Tirex_makeLabelTemporary(Tirex this, const char *name)
{
  Procedure procedure = Tirex_PROCEDURE(this);
  Label label = Tirex_makeLabel(this, name);
  Temporary temporary = Procedure_makeLabelTemporary(procedure, label, Immediate__UNDEF);
  return temporary;
}

static Temporary
Tirex_makeSymbolTemporary(Tirex this, const char *name, int64_t offset)
{
  Procedure procedure = Tirex_PROCEDURE(this);
  Symbol symbol = Tirex_makeSymbol(this, name);
  Temporary temporary = Procedure_makeSymbolTemporary(procedure, symbol, offset, Immediate__UNDEF);
  return temporary;
}

static BasicBlock
Tirex_makeBasicBlock(Tirex this, const char *name, bool chain)
{
  StringTable strings = Tirex_STRINGS(this);
  String key = StringTable_lookup(strings, name, strlen(name) + 1);
  BasicBlock *_basicBlock = HTable_lookup(Tirex_BASICBLOCKS(this), (HTableKey)key, sizeof(BasicBlock) + 1);
  if (*_basicBlock == NULL) {
    Procedure procedure = Tirex_PROCEDURE(this);
    BasicBlock basicBlock = Procedure_makeBasicBlock(procedure, 0, 0.0);

    int blockIndex;
    if (sscanf(name, "B%d", &blockIndex) == 1)
      BasicBlock_INDEX(basicBlock) = blockIndex;
    else
      Tirex_ERROR(this, "basicblock name is not of type B<n>");

    uint32_t identity = strtoul(name, NULL, 0);
    if (!chain) {
      Procedure_unchainBasicBlock(procedure, basicBlock);
    }
    *(char *)(_basicBlock + 1) = chain;
    *_basicBlock = basicBlock;
  }
  if (chain && !*(char *)(_basicBlock + 1)) {
    Procedure procedure = Tirex_PROCEDURE(this);
    Procedure_pushBasicBlock(procedure, *_basicBlock);
    *(char *)(_basicBlock + 1) = chain;
  }
  return *_basicBlock;
}
 
static LoopScope
Tirex_makeLoopScope(Tirex this, const char *name, BasicBlock basicBlock)
{
  // FIXME FRAGILE !!! The first time this function is called for a given LoopScope the
  // basicBlock argument should ALWAYS be correctly given.
  StringTable strings = Tirex_STRINGS(this);
  String key = StringTable_lookup(strings, name, strlen(name) + 1);
  LoopScope *_loopScope = HTable_lookup(Tirex_LOOPSCOPES(this), (HTableKey)key, sizeof(LoopScope) + 1);
  if (*_loopScope == NULL) {
    Procedure procedure = Tirex_PROCEDURE(this);
    Configure configure = Program_CONFIGURE(Procedure_PROGRAM(procedure));
    LoopScope loopScope = Procedure_makeLoopScope(procedure, basicBlock, configure);

    int loopIndex;
    if (sscanf(name, "L%d", &loopIndex) == 1)
      LoopScope_INDEX(loopScope) = loopIndex;
    else
      Tirex_ERROR(this, "loop name is not of type L<n>");

    *_loopScope = loopScope;
  }
  return *_loopScope;
}

#define Tirex_MAX_RESCOUNT 256
#define Tirex_MAX_ARGCOUNT 256
#define Tirex_MAX_REGCOUNT 512

#ifdef $XCC_h
/**
 * When possible / necessary expand an operation from it's standard version to
 * it's multi-version with seperated register-pair components.
 */
void
Tirex_expandToMulti(Tirex this, Operation operation, Selector selector);
#endif//$XCC_h

void
Tirex_expandToMulti(Tirex this, Operation operation, Selector selector)
{
  Processor processor = Program_processor(Tirex_program(this));
  Temporary temporary;
  Temporary results[Tirex_MAX_RESCOUNT];
  Temporary arguments[Tirex_MAX_ARGCOUNT];
  Operator multiOperator = Operator__UNDEF;
  Operator operator = Operation_operator(operation);
  Operation multiOperation = NULL;
  OperatorParameter parameter;
  OperatorParameterArray parameterArray;
  int resCount = 0, resMulti = 0;
  int argCount = 0, argMulti = 0;
  int regNum;
  int index;
  char* ptr = NULL;
  const char * regName = NULL;
  char regNameMulti[8];
  char multiName[64] = "multi_";
  //
  if (strstr(Operator_shortName(operator), "multi_"))
    return;

  strcat(multiName, Operator_shortName(operator));
  multiOperator = Operator_fromShortName(multiName, processor);

  if (multiOperator == Operator__UNDEF)
    return;

  parameterArray = (OperatorParameterArray) Operator_parameterArray(multiOperator);

  parameter = (OperatorParameter) OperatorParameterArray_writeParameters(parameterArray);
  Operation_RESULTS_FOREACH_Temporary(operation, temporary) {
    if (OperatorParameter_kind(&parameter[resCount + resMulti]) == OperatorParameterKind_WriteMulti) {
      Except_CHECK(Temporary_type(temporary) == TemporaryType_Dedicated);
      //Except_CHECK(Temporary_regFile(temporary) == RegFile_k1b_PRF);

      regName = Register_names(Temporary_register(temporary))[0];
      ptr = strchr(regName, (int) 'r');
      regNum = atoi(ptr + 1);
      sprintf(regNameMulti, "$r%d", regNum);
      results[resCount++ + resMulti] = Tirex_makeDedicatedTemporary(this, regNameMulti);
      sprintf(regNameMulti, "$r%d", regNum + 1);
      results[resCount + resMulti++] = Tirex_makeDedicatedTemporary(this, regNameMulti);
    } else {
      results[resCount + resMulti] = Operation_results(operation)[resCount];
      resCount++;
    }
  } Operation_RESULTS_ENDEACH_Temporary;
  resCount += resMulti;
  
  parameter = (OperatorParameter) OperatorParameterArray_readParameters(parameterArray);
  Operation_ARGUMENTS_FOREACH_Temporary_Index(operation, temporary, index) {
    if (index >= Operator_readCount(operator)) { /* Directly copy implicit uses */
      arguments[argCount + argMulti] = Operation_arguments(operation)[argCount];
      argCount++;
      continue;
    }

    if ((OperatorParameter_kind(&parameter[argCount + argMulti]) == OperatorParameterKind_ReadMulti) ||
      (OperatorParameter_kind(&parameter[argCount + argMulti]) == OperatorParameterKind_ReadMultiStored)) {
      Except_CHECK(Temporary_type(temporary) == TemporaryType_Dedicated);
      //Except_CHECK(Temporary_regFile(temporary) == RegFile_k1b_PRF);

      regName = Register_names(Temporary_register(temporary))[0];
      ptr = strchr(regName, (int) 'r');
      regNum = atoi(ptr + 1);
      sprintf(regNameMulti, "$r%d", regNum);
      arguments[argCount + argMulti] = Tirex_makeDedicatedTemporary(this, regNameMulti);
      argCount++;
      sprintf(regNameMulti, "$r%d", regNum + 1);
      arguments[argCount + argMulti] = Tirex_makeDedicatedTemporary(this, regNameMulti);
      argMulti++;
    } else
    if ((OperatorParameter_kind(&parameter[argCount + argMulti]) == OperatorParameterKind_ReadMultiPartial) ||
        (OperatorParameter_kind(&parameter[argCount + argMulti]) == OperatorParameterKind_ReadMultiLeft) ||
        (OperatorParameter_kind(&parameter[argCount + argMulti]) == OperatorParameterKind_ReadMultiRight)) {
      // Nothing to be done
    } else {
      arguments[argCount + argMulti] = Operation_arguments(operation)[argCount];
      argCount++;
    }
  } Operation_ARGUMENTS_ENDEACH_Temporary;
  argCount += argMulti;

  multiOperation = Selector_makeOperation(selector, multiOperator, resCount, argCount, results, arguments, NULL);
  Operation_fixOperandsWithParameters(multiOperation);

  Selector_moveOperationAfter(selector, operation, multiOperation);
  Selector_detachOperation(selector, operation);
}


#ifdef $XCC_h
/**
 * When possible fuse a multi-variant operation into it's standard version.
 */
void
Tirex_fuseMulti(Tirex this, Operation operation, Selector selector);
#endif//$XCC_h

void
Tirex_fuseMulti(Tirex this, Operation operation, Selector selector)
{
  Processor processor = Program_processor(Tirex_program(this));
  Temporary temporary;
  Temporary results[Tirex_MAX_RESCOUNT];
  Temporary arguments[Tirex_MAX_ARGCOUNT];
  Operator singleOperator = Operator__UNDEF;
  Operator operator = Operation_operator(operation);
  Operation singleOperation = NULL;
  OperatorParameter parameter;
  OperatorParameterArray parameterArray;
  int resCount = 0, resMulti = 0;
  int argCount = 0, argMulti = 0;
  int regNum;
  int index;
  char * ptr = NULL;
  const char * regName = NULL;
  char regNameSingle[8];
  char * singleName = NULL;
  bool skipNext = false;
  //
  if (!strstr(Operator_shortName(operator), "multi_"))
    return;

  singleName = strchr(Operator_shortName(operator), (int) '_');
  singleOperator = Operator_fromShortName(&singleName[1], processor);

  if (singleOperator == Operator__UNDEF)
    return;

  parameterArray = (OperatorParameterArray) Operator_parameterArray(operator);

  parameter = (OperatorParameter) OperatorParameterArray_writeParameters(parameterArray);
  Operation_RESULTS_FOREACH_Temporary(operation, temporary) {
    if (skipNext) {
      skipNext = false;
      continue;
    }

    if (OperatorParameter_kind(&parameter[resCount + resMulti]) == OperatorParameterKind_WriteMulti) {
      Except_CHECK(Temporary_type(temporary) == TemporaryType_Dedicated);

      regName = Register_names(Temporary_register(temporary))[0];
      ptr = strchr(regName, (int) 'r');
      regNum = atoi(&ptr[1]);
      sprintf(regNameSingle, "$p%d", regNum);
      results[resCount] = Tirex_makeDedicatedTemporary(this, regNameSingle);
      resCount++;
      resMulti++;
      skipNext = true;
    } else {
      results[resCount] = Operation_results(operation)[resCount + resMulti];
      resCount++;
    }
  } Operation_RESULTS_ENDEACH_Temporary;
  
  parameter = (OperatorParameter) OperatorParameterArray_readParameters(parameterArray);
  Operation_ARGUMENTS_FOREACH_Temporary_Index(operation, temporary, index) {
    if (skipNext) {
      skipNext = false;
      continue;
    }

    if (index >= Operator_readCount(operator)) { /* Directly copy implicit uses */
      arguments[argCount] = Operation_arguments(operation)[argCount + argMulti];
      argCount++;
      continue;
    }

    if ((OperatorParameter_kind(&parameter[argCount + argMulti]) == OperatorParameterKind_ReadMulti) ||
        (OperatorParameter_kind(&parameter[argCount + argMulti]) == OperatorParameterKind_ReadMultiStored)) {
      Except_CHECK(Temporary_type(temporary) == TemporaryType_Dedicated);

      regName = Register_names(Temporary_register(temporary))[0];
      ptr = strchr(regName, (int) 'r');
      regNum = atoi(&ptr[1]);
      sprintf(regNameSingle, "$p%d", regNum);
      arguments[argCount] = Tirex_makeDedicatedTemporary(this, regNameSingle);
      argCount++;
      argMulti++;
      skipNext = true;
    } else
    if ((OperatorParameter_kind(&parameter[argCount + argMulti]) == OperatorParameterKind_ReadMultiPartial) ||
        (OperatorParameter_kind(&parameter[argCount + argMulti]) == OperatorParameterKind_ReadMultiLeft) ||
        (OperatorParameter_kind(&parameter[argCount + argMulti]) == OperatorParameterKind_ReadMultiRight)) {
      // Nothing to be done
    } else {
      arguments[argCount] = Operation_arguments(operation)[argCount + argMulti];
      argCount++;
    }
  } Operation_ARGUMENTS_ENDEACH_Temporary;

  singleOperation = Selector_makeOperation(selector, singleOperator, resCount, argCount, results, arguments, NULL);
  Operation_fixOperandsWithParameters(singleOperation);
  Operation_setStartDate(singleOperation, Operation_startDate(operation));

  Selector_moveOperationAfter(selector, operation, singleOperation);
  Selector_detachOperation(selector, operation);
}



////////////////////////////////////////////////////////////////////////////////////////////////////

static int
Tirex_streamState(Tirex this)
{
  return Tirex_STREAMSTATE(this);
}

static void
Tirex_advanceStreamState(Tirex this)
{
  Tirex_STREAMSTATE(this)++;
}

static bool
Tirex_printEvent(Tirex this, FILE *file, const char *function)
{
#ifdef YAML_ENABLED
  if (file == NULL) file = stderr;
  if (function) {
    fprintf(file, "%s\t", function + 6);
  }
  switch(Tirex_EVENT_TYPE(this)) {
  case YAML_NO_EVENT:
    fprintf(file, "YAML_NO_EVENT\n");
    break;
  case YAML_STREAM_START_EVENT:
    fprintf(file, "YAML_STREAM_START_EVENT\n");
    break;
  case YAML_STREAM_END_EVENT:
    fprintf(file, "YAML_STREAM_END_EVENT\n");
    break;
  case YAML_DOCUMENT_START_EVENT:
    fprintf(file, "YAML_DOCUMENT_START_EVENT\n");
    break;
  case YAML_DOCUMENT_END_EVENT:
    fprintf(file, "YAML_DOCUMENT_END_EVENT\n");
    break;
  case YAML_SEQUENCE_START_EVENT:
    fprintf(file, "YAML_SEQUENCE_START_EVENT\n");
    break;
  case YAML_SEQUENCE_END_EVENT:
    fprintf(file, "YAML_SEQUENCE_END_EVENT\n");
    break;
  case YAML_MAPPING_START_EVENT:
    fprintf(file, "YAML_MAPPING_START_EVENT\n");
    break;
  case YAML_MAPPING_END_EVENT:
    fprintf(file, "YAML_MAPPING_END_EVENT\n");
    break;
  case YAML_ALIAS_EVENT:
    fprintf(file, "YAML_ALIAS_EVENT (%s)\n", Tirex_EVENT_ANCHOR(this));
    break;
  case YAML_SCALAR_EVENT:
    fprintf(file, "YAML_SCALAR_EVENT (%s)\n", Tirex_EVENT_VALUE(this));
    break;
  }
#endif//YAML_ENABLED
  return true;
}

#ifdef $XCC_h
void
Tirex_error(Tirex this, const char *function, const char *message);
#define Tirex_ERROR(this, message) Tirex_error(this, __FUNCTION__, message)
#endif//$XCC_h

void
Tirex_error(Tirex this, const char *function, const char *message)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = Tirex_EVENT_TYPE(this);
  fprintf(stderr, "(from %s)\t", function);
  if (type != YAML_NO_EVENT) {
    size_t line = Tirex_EVENT(this)->start_mark.line;
    error_at_line(EXIT_FAILURE, 0, Tirex_INPUTNAME(this), (unsigned)line,
                  message, Tirex_VALUE(this));
  } else {
    error(EXIT_FAILURE, 0, message, Tirex_VALUE(this));
  }
#endif//YAML_ENABLED
}

#ifdef $XCC__h
static yaml_event_type_t
Tirex_pull(Tirex this, const char *function);
#define Tirex_PULL(this) Tirex_pull(this, __FUNCTION__)
#endif//$XCC__h

static yaml_event_type_t
Tirex_pull(Tirex this, const char *function)
{
#ifdef YAML_ENABLED
  if (Tirex_EVENT_TYPE(this) != YAML_NO_EVENT) {
    yaml_event_delete(Tirex_EVENT(this));
  }
  if (!yaml_parser_parse(Tirex_PARSER(this), Tirex_EVENT(this))) {
    Tirex_error(this, function, "input parse error");
  }
  TIREX_TRACE && Tirex_printEvent(this, TIREX_TRACE, function);
  return Tirex_EVENT_TYPE(this);
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC_h
/**
 * Tirex_makeProgram 
 */
Program
Tirex_makeProgram(Tirex this);
#endif//$XCC_h

Program
Tirex_makeProgram(Tirex this)
{
  int state = Tirex_streamState(this);
  if (state == 0) {
    Tirex_startStream(this);
  }
  if (state == 2) {
    return NULL;
  }
  Tirex_parseDocument(this);
  return Tirex_PROGRAM(this);
}

#ifdef $XCC_h
/**
 * Tirex_startStream
 * Used to initialize a YAML stream containing Tirex
 */
void
Tirex_startStream(Tirex this);
#endif//$XCC_h

void
Tirex_startStream(Tirex this)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  //
  type = Tirex_PULL(this);
  if (type != YAML_STREAM_START_EVENT)
    Tirex_ERROR(this, "expecting STREAM_START");

  type = Tirex_PULL(this);
  if (type == YAML_STREAM_END_EVENT) {
    Tirex_advanceStreamState(this);
    Tirex_advanceStreamState(this);
  } else if (type == YAML_DOCUMENT_START_EVENT) {
    Tirex_advanceStreamState(this);
  } else {
    Tirex_ERROR(this, "expecting DOCUMENT_START or STREAM_END");
  }
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after DOCUMENT_END or STREAM_START
 * Parse (MAPPING_START,SCALAR..MAPPING_END)+
 */
void
Tirex_parseDocument(Tirex this);
#endif//$XCC__h

void
Tirex_parseDocument(Tirex this)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  //
  Except_CHECK(Tirex_EVENT_TYPE(this) == YAML_DOCUMENT_START_EVENT);
  //
  type = Tirex_PULL(this);
  if (type == YAML_MAPPING_START_EVENT) {
  } else
    Tirex_ERROR(this, "expecting MAPPING_START");
  //
  type = Tirex_parseRoot(this);
  if (type == YAML_MAPPING_END_EVENT) {
  } else
    Tirex_ERROR(this, "expecting MAPPING_END");
  //
  type = Tirex_PULL(this);
  if (type == YAML_DOCUMENT_END_EVENT) {
  } else
    Tirex_ERROR(this, "expecting DOCUMENT_END");
  //
  type = Tirex_PULL(this);
  if (type == YAML_STREAM_END_EVENT)
    Tirex_advanceStreamState(this);
  else if (type != YAML_DOCUMENT_START_EVENT)
    Tirex_ERROR(this, "expecting STREAM_END");
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after MAPPING_START
 * Parse (MAPPING_START,SCALAR..MAPPING_END)+
 */
yaml_event_type_t
Tirex_parseRoot(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseRoot(Tirex this)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  //
  /* Initialize the program for this Tirex object */
  *Tirex__PROGRAM(this) = Program_new(Memory_Root, (Processor)1, (Convention)1);
  //
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
    } else
      return type;
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      if (!strcmp(Tirex_KEY(this), "file")) {
        Program_setFilename(Tirex_PROGRAM(this), Tirex_EVENT_VALUE(this));
      } else
        Tirex_ERROR(this, "expecting 'file'");
    } else
    if (type == YAML_SEQUENCE_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "program")) {
        type = Tirex_parseProgram(this);
      } else
        Tirex_ERROR(this, "expecting 'program'");
      if (type != YAML_SEQUENCE_END_EVENT)
        Tirex_ERROR(this, "expecting SEQUENCE_END");
    } else
    if (type == YAML_MAPPING_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "optimize")) {
        type = Tirex_parseOptimize(this);
      } else
        Tirex_ERROR(this, "expecting 'optimize'");
      if (type != YAML_MAPPING_END_EVENT)
        Tirex_ERROR(this, "expecting MAPPING_END");
    } else
      Tirex_ERROR(this, "expecting SCALAR, SEQUENCE_START or MAPPING_START");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(program),SEQUENCE_START
 * Parse (MAPPING_START,SCALAR..MAPPING_END)+
 */
yaml_event_type_t
Tirex_parseProgram(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseProgram(Tirex this)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "program"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_MAPPING_START_EVENT) {
    } else
      return type;
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
      if (!strcmp(Tirex_KEY(this), "function")) {
        type = Tirex_parseFunction(this);
      } else
      if (!strcmp(Tirex_KEY(this), "section")) {
        type = Tirex_PULL(this);
        if (type != YAML_MAPPING_START_EVENT)
          Tirex_ERROR(this, "expected MAPPING_START");
        type = Tirex_parseSection(this, NULL);
        if (type == YAML_MAPPING_END_EVENT)
          type = Tirex_PULL(this);
        else
          Tirex_ERROR(this, "expecting MAPPING_END");
      } else
      if (!strcmp(Tirex_KEY(this), "common")) {
        type = Tirex_parseCommon(this);
      } else
      if (!strcmp(Tirex_KEY(this), "object")) {
        type = Tirex_parseObject(this);
      } else
        Tirex_ERROR(this, "expecting 'function' or 'section' or 'common' or 'object'");
      if (type != YAML_MAPPING_END_EVENT)
        Tirex_ERROR(this, "expecting MAPPING_END");
    } else
      Tirex_ERROR(this, "expecting SCALAR");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after MAPPING_START,SCALAR(optimize)
 * Parse SCALAR,(SCALAR,SCALAR)*
 */
yaml_event_type_t
Tirex_parseOptimize(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseOptimize(Tirex this)
{
#ifdef YAML_ENABLED
  // FIXME Future 'optimize' fields should allow for the register convention
  // to be set with a flag like 'Embedded' or 'PIC' (in the K1 case)
  Processor processor;
  Program program = Tirex_PROGRAM(this);
  Optimize optimize = Tirex_OPTIMIZE(this);
  OptimizeFlags flags = 0;
  yaml_event_type_t type = YAML_NO_EVENT;
  //
  Except_CHECK(!strcmp(Tirex_KEY(this), "optimize"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
    } else {
      return type;
    }
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char * mapping_value = Tirex_EVENT_VALUE(this);
      if (!strcmp(Tirex_KEY(this), "arch")) {
        processor = Processor_fromName(mapping_value);
        *Program__PROCESSOR(program) = processor;
        *Program__CONVENTION(program) = 1; // FIXME
        if (!processor)
          Tirex_ERROR(this, "unknown processor");
      } else
      if (!strcmp(Tirex_KEY(this), "SSA")) {
        flags = OptimizeActivation_SSAForm;
        if (!strcmp("true", mapping_value)) {
          *Optimize__ACTIVATION(optimize) |= flags;
        } else
        if (!strcmp("false", mapping_value)) {
          *Optimize__ACTIVATION(optimize) &= ~flags;
        } else
          Tirex_ERROR(this, "expecting boolean");
      } else
        Tirex_ERROR(this, "expected 'arch' or 'SSA'");
    } else if (type == YAML_SEQUENCE_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "phases")) {
        type = Tirex_parseOptimizePhases(this);
      } else
        Tirex_ERROR(this, "expecting 'phases'");
      if (type != YAML_SEQUENCE_END_EVENT)
        Tirex_ERROR(this, "expecting SEQUENCE_END");
    } else
      Tirex_ERROR(this, "expecting SCALAR or SEQUENCE_START");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Caleed after SCALAR(function), SEQUENCE_START
 * Parse SCALAR*
 */
yaml_event_type_t
Tirex_parseOptimizePhases(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseOptimizePhases(Tirex this)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Optimize optimize = Program_optimize(Tirex_PROGRAM(this));
  OptimizeFlags flags = 0;
  //
  Except_CHECK(!strcmp(Tirex_KEY(this), "phases"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      flags |= OptimizeActivation_parseFlags(Tirex_EVENT_VALUE(this));
    } else {
      *Optimize__ACTIVATION(optimize) = flags;
      return type;
    }
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after MAPPING_START,SCALAR(function)
 * Parse SCALAR,(SCALAR,(SCALAR|SEQUENCE_START..SEQUENCE_END))*
 */
yaml_event_type_t
Tirex_parseFunction(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseFunction(Tirex this)
{
#ifdef YAML_ENABLED
  Procedure procedure = NULL;
  DataSection section = NULL;
  Program program = Tirex_PROGRAM(this);
  Tirex_resetHTables(this);
  yaml_event_type_t type = YAML_NO_EVENT;
  //
  Except_CHECK(!strcmp(Tirex_KEY(this), "function"));
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    const char *mapping_value = Tirex_EVENT_VALUE(this);
    Symbol symbol = SymbolTable_makeSymbol(Program_symbolTable(program), mapping_value);
    *Tirex__PROCEDURE(this) = procedure = Program_makeProcedure(program, symbol);
    *Symbol__EXPORT(symbol) = SymbolExport_LOCAL;
  } else
    Tirex_ERROR(this, "expecting SCALAR");
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
    } else {
      return type;
    }
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char * mapping_value = Tirex_EVENT_VALUE(this);
      if (!strcmp(Tirex_KEY(this), "section")) {
        int i;
        
        section = Program_searchSection(program, mapping_value);
        if (section == NULL)
          Tirex_ERROR(this, "function belongs to an undefined section");
        Procedure_setSection(procedure, section);
        *Symbol__STORE(*Procedure__SYMBOL(procedure)) = SymbolStore_TEXT;
      } else
      if (!strcmp(Tirex_KEY(this), "linkage")) {
        if (!strcmp(mapping_value, "weak")) {
          *Symbol__EXPORT(*Procedure__SYMBOL(procedure)) = SymbolExport_WEAK;
        } else
        if (!strcmp(mapping_value, "local")) {
          *Symbol__EXPORT(*Procedure__SYMBOL(procedure)) = SymbolExport_LOCAL;
        } else
        if (!strcmp(mapping_value, "global")) {
          *Symbol__EXPORT(*Procedure__SYMBOL(procedure)) = SymbolExport_GLOBAL_PREEMPTIBLE;
        } else
        if (!strcmp(mapping_value, "global-protected")) {
          *Symbol__EXPORT(*Procedure__SYMBOL(procedure)) = SymbolExport_GLOBAL_PROTECTED;
        } else
        if (!strcmp(mapping_value, "global-hidden")) {
          *Symbol__EXPORT(*Procedure__SYMBOL(procedure)) = SymbolExport_GLOBAL_HIDDEN;
        } else
        if (!strcmp(mapping_value, "global-internal")) {
          *Symbol__EXPORT(*Procedure__SYMBOL(procedure)) = SymbolExport_GLOBAL_INTERNAL;
        } else
          Tirex_ERROR(this, "unknown linkage parameter");
      } else
      if (!strcmp(Tirex_KEY(this), "entry")) {
        if (mapping_value[0] == 'B') {
          BasicBlock basicBlock = Tirex_makeBasicBlock(this, mapping_value, false);
          unsigned flags = BasicBlock_flags(basicBlock) | BasicBlockFlag_Entry;
          BasicBlock_setFlags(basicBlock, flags);
        } else
          Tirex_ERROR(this, "expecting basic-block identifier to start with 'B'");
      } else
      if (!strcmp(Tirex_KEY(this), "align")) {
        Procedure_setAlign(procedure, (uint8_t) atoi(mapping_value));
      } else
        Tirex_ERROR(this, "expecting 'section', 'linkage', 'align' or 'entry'");
    } else
    if (type == YAML_SEQUENCE_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "exits")) {
        type = Tirex_parseFunctionExits(this);
      } else
      if (!strcmp(Tirex_KEY(this), "blocks")) {
        type = Tirex_parseFunctionBlocks(this);
      } else
      if (!strcmp(Tirex_KEY(this), "loops")) {
        type = Tirex_parseFunctionLoops(this);
      } else
        Tirex_ERROR(this, "expecting 'exits', 'blocks' or 'loops'");
      if (type != YAML_SEQUENCE_END_EVENT)
        Tirex_ERROR(this, "expecting SEQUENCE_END");
    } else
    if (type == YAML_MAPPING_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "section")) {
        char * sectionName;
        type = Tirex_parseSection(this, &sectionName);
        if (type != YAML_MAPPING_END_EVENT)
          Tirex_ERROR(this, "expecting MAPPING_END");
        section = Program_searchSection(program, sectionName);
        if (section == NULL)
          Tirex_ERROR(this, "function belongs to an undefined section");
        Procedure_setSection(procedure, section);
        *Symbol__STORE(*Procedure__SYMBOL(procedure)) = SymbolStore_TEXT;
      } else
        Tirex_ERROR(this, "expecting 'section' with the section definition");
    } else
      Tirex_ERROR(this, "expecting SCALAR, SEQUENCE_START or MAPPING_START");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(exits),SEQUENCE_START
 * Parse (SCALAR)+
 */
yaml_event_type_t
Tirex_parseFunctionExits(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseFunctionExits(Tirex this)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "exits"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
      if (!strncmp(Tirex_KEY(this), "B", 1)) {
        BasicBlock basicBlock = Tirex_makeBasicBlock(this, Tirex_KEY(this), false);
        unsigned flags = BasicBlock_flags(basicBlock) | BasicBlockFlag_Exit;
        BasicBlock_setFlags(basicBlock, flags);
      } else
        Tirex_ERROR(this, "expecting 'B<n>'");
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(blocks),SEQUENCE_START
 * Parse (MAPPING_START,SCALAR,..MAPPING_END)*
 */
yaml_event_type_t
Tirex_parseFunctionBlocks(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseFunctionBlocks(Tirex this)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "blocks"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_MAPPING_START_EVENT) {
    } else
      return type;
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
      if (!strcmp(Tirex_KEY(this), "block")) {
        type = Tirex_parseBodyBlock(this);
      } else
        Tirex_ERROR(this, "expecting 'block'");
      if (type != YAML_MAPPING_END_EVENT)
        Tirex_ERROR(this, "expecting MAPPING_END");
    } else
      Tirex_ERROR(this, "expecting SCALAR");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after MAPPING_START,SCALAR(block)
 * Parse SCALAR,(SCALAR,(SCALAR|SEQUENCE_START..SEQUENCE_END))*
 */
yaml_event_type_t
Tirex_parseBodyBlock(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseBodyBlock(Tirex this)
{
#ifdef YAML_ENABLED
  BasicBlock basicBlock = NULL;
  //
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "block"));
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    const char *mapping_value = Tirex_EVENT_VALUE(this);
    basicBlock = Tirex_makeBasicBlock(this, mapping_value, true);
  } else
    Tirex_ERROR(this, "expecting SCALAR");
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
    } else
      return type;
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char *mapping_value = Tirex_EVENT_VALUE(this);
      if (!strcmp(Tirex_KEY(this), "frequency")) {
        float frequency = atof(mapping_value);
        BasicBlock_setFrequency(basicBlock, frequency);
      } else
        Tirex_ERROR(this, "expecting 'frequency'");
    } else
    if (type == YAML_SEQUENCE_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "flags")) {
        type = Tirex_parseBlockFlags(this, basicBlock);
      } else
      if (!strcmp(Tirex_KEY(this), "labels")) {
        type = Tirex_parseBlockLabels(this, basicBlock);
      } else
      if (!strcmp(Tirex_KEY(this), "successors")) {
        type = Tirex_parseBlockSuccessors(this, basicBlock);
      } else
      if (!strcmp(Tirex_KEY(this), "predecessors")) {
        type = Tirex_parseBlockPredecessors(this, basicBlock);
      } else
      if (!strcmp(Tirex_KEY(this), "operations")) {
        type = Tirex_parseBlockOperations(this, basicBlock);
      } else
        Tirex_ERROR(this, "expecting 'flags', 'labels', 'successors', 'predecessors' or 'operations'");
      if (type != YAML_SEQUENCE_END_EVENT)
        Tirex_ERROR(this, "expecting SEQUENCE_END");
    } else
      Tirex_ERROR(this, "expecting SCALAR or SEQUENCE_START");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(flags),SEQUENCE_START
 * Parse (SCALAR)*
 */
yaml_event_type_t
Tirex_parseBlockFlags(Tirex this, BasicBlock basicBlock);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseBlockFlags(Tirex this, BasicBlock basicBlock)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "flags"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      uint32_t flags = 0;
      *Tirex__VALUE(this) = Tirex_EVENT_VALUE(this);
      if (ETable_parseFlags(BasicBlockFlag_Table, Tirex_VALUE(this), ',', &flags)) {
        flags |= BasicBlock_flags(basicBlock);
        BasicBlock_setFlags(basicBlock, flags);
      } else
        Tirex_ERROR(this, "Unknown BasicBlock flag '%s'");
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(labels),SEQUENCE_START
 * Parse (SCALAR)+
 */
yaml_event_type_t
Tirex_parseBlockLabels(Tirex this, BasicBlock basicBlock);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseBlockLabels(Tirex this, BasicBlock basicBlock)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "labels"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char *mapping_value = Tirex_EVENT_VALUE(this);
      Label label = Tirex_makeLabel(this, mapping_value);
      BasicBlock_addLabel(basicBlock, label);
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(successors),SEQUENCE_START
 * Parse (MAPPING_START,SCALAR,SCALAR,MAPPING_END)+
 */
yaml_event_type_t
Tirex_parseBlockSuccessors(Tirex this, BasicBlock basicBlock);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseBlockSuccessors(Tirex this, BasicBlock basicBlock)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  //
  Except_CHECK(!strcmp(Tirex_KEY(this), "successors"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_MAPPING_START_EVENT) {
      type = Tirex_parseBlockSuccessorsElement(this, basicBlock);
      if (type != YAML_MAPPING_END_EVENT)
        Tirex_ERROR(this, "expecting MAPPING_END");
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(successors), (SCALAR)*
 * Parse (SCALAR,SCALAR) */
yaml_event_type_t
Tirex_parseBlockSuccessorsElement(Tirex this, BasicBlock basicBlock);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseBlockSuccessorsElement(Tirex this, BasicBlock basicBlock)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  BasicBlock succ_block = NULL;
  BasicBlockEdge edge = NULL;
  //
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    if (!strcmp(Tirex_EVENT_VALUE(this), "block")) {
      type = Tirex_PULL(this);
      const char * mapping_value = Tirex_EVENT_VALUE(this);
      if (mapping_value[0] == 'B') {
        succ_block = Tirex_makeBasicBlock(this, mapping_value, false);
      } else
        Tirex_ERROR(this, "expecting 'B<integer>'");
      edge = BasicBlock_hasSuccessor(basicBlock, succ_block);
      if (!edge)
        edge = BasicBlock_makeEdge(basicBlock, succ_block, 0);
    } else
      Tirex_ERROR(this, "expecting 'block'");
  } else
    Tirex_ERROR(this, "expecting 'block'");
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    if (!strcmp(Tirex_EVENT_VALUE(this), "probability")) {
      type = Tirex_PULL(this);
      const char * mapping_value = Tirex_EVENT_VALUE(this);
      if (isdigit(mapping_value[0]))
        edge->PROBABILITY = atof(mapping_value);
      else
        Tirex_ERROR(this, "probability is not given as a float");
    } else
      Tirex_ERROR(this, "expecting 'probability: <float>'");
  } else
  if (type == YAML_MAPPING_END_EVENT) {
    return type;
  } else
    Tirex_ERROR(this, "expecting 'probability: <float>' or MAPPING_END");
  type = Tirex_PULL(this);
  return type;
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(predecessors),SEQUENCE_START
 * Parse (SCALAR)+
 */
yaml_event_type_t
Tirex_parseBlockPredecessors(Tirex this, BasicBlock basicBlock);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseBlockPredecessors(Tirex this, BasicBlock basicBlock)
{
#ifdef YAML_ENABLED
  BasicBlock pred_block = NULL;
  //
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "predecessors"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
      if (!strncmp(Tirex_KEY(this), "B", 1)) {
        pred_block = Tirex_makeBasicBlock(this, Tirex_KEY(this), false);
        if (!BasicBlock_hasSuccessor(pred_block, basicBlock)) {
          BasicBlock_makeEdge(pred_block, basicBlock, 0);
        }
      } else
        Tirex_ERROR(this, "expecting 'B<n>'");
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(operations),SEQUENCE_START
 * Parse (MAPPING_START,SCALAR..MAPPING_END)*
 */
yaml_event_type_t
Tirex_parseBlockOperations(Tirex this, BasicBlock basicBlock);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseBlockOperations(Tirex this, BasicBlock basicBlock)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "operations"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_MAPPING_START_EVENT) {
    } else
      return type;
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
      if (!strcmp(Tirex_KEY(this), "op")) {
        Selector selector = BasicBlock_selector(basicBlock);
        type = Tirex_parseOperation(this, selector);
      } else
        Tirex_ERROR(this, "expecting 'op'");/* or 'labels'");*/
      if (type != YAML_MAPPING_END_EVENT)
        Tirex_ERROR(this, "expecting MAPPING_END");
    } else
      Tirex_ERROR(this, "expecting SCALAR or SEQUENCE_START");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}


#ifdef $XCC__h
/**
 * Called after MAPPING_START,SCALAR(op)
 * Parse SCALAR,(SCALAR,(SEQUENCE_START..SEQUENCE_END|SCALAR))*
 */
yaml_event_type_t
Tirex_parseOperation(Tirex this, Selector selector);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseOperation(Tirex this, Selector selector)
{
#ifdef YAML_ENABLED
  Operator operator = Operator__UNDEF;
  Temporary results[Tirex_MAX_RESCOUNT];
  Temporary arguments[Tirex_MAX_ARGCOUNT];
  RegisterSet_ clobberSet_, *clobberSet = NULL;
  int resCount = 0, argCount = 0;
  bool hasCallArguments = false;
  OperationFlags flags = 0;
  //
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "op"));
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    const char *mapping_value = Tirex_EVENT_VALUE(this);
    Processor processor = Selector_processor(selector);
    operator = Operator_fromShortName(mapping_value, processor);
    if (operator == Operator__UNDEF) {
      *Tirex__VALUE(this) = Tirex_EVENT_VALUE(this);
      Tirex_ERROR(this, "unknown operator '%s'");
    }
  } else
    Tirex_ERROR(this, "expecting SCALAR");
  //
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
    } else {
      // When necessary insert a dummy operands for a GNUASM operation
      if (Operator_isGNUASM(operator)) {
        while (resCount < Operator_writeCount(operator)) {
          Temporary dummyTemp = (Temporary) malloc(sizeof(struct Temporary_));
          Temporary_Ctor(dummyTemp, TemporaryType_Virtual, 0x0);
          results[resCount++] = dummyTemp;
        }
        while (argCount < Operator_readCount(operator)) {
          Temporary dummyTemp = (Temporary) malloc(sizeof(struct Temporary_));
          Temporary_Ctor(dummyTemp, TemporaryType_Virtual, 0x0);
          results[argCount++] = dummyTemp;
        }
      }
      // FIXME this is a temporary way of introducting a clobberSet for calls
      if (Operator_isCall(operator))
        clobberSet = Convention_callerRegisterSet(Tirex_CONVENTION(this));
      Operation operation = Selector_makeOperation(selector, operator, resCount, argCount, results, arguments, clobberSet);
      Operation_fixOperandsWithParameters(operation);
      if ((Operator_isCall(operator) || Operator_isGoTo(operator)) && hasCallArguments) {
        // TODO Process Call arguments
      } else if (hasCallArguments)
        Tirex_ERROR(this, "A non-call operation has call arguments");
      *Operation__FLAGS(operation) |= flags;
      return type;
    }
    type = Tirex_PULL(this);
    if (type == YAML_SEQUENCE_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "defs")) {
        type = Tirex_parseOperationDefs(this, operator, &resCount, results);
      } else
      if (!strcmp(Tirex_KEY(this), "uses")) {
        type = Tirex_parseOperationUses(this, operator, &argCount, arguments);
      } else
      if (!strcmp(Tirex_KEY(this), "clobbers")) {
        clobberSet = RegisterSet_Ctor(&clobberSet_);
        type = Tirex_parseOperationClobbers(this, clobberSet);
      } else
      /* Operation flags and call_arguments are not yet part of the Tirex specification */
      /* (!strcmp(Tirex_KEY(this), "flags")) {
        type = Tirex_parseOperationFlags(this, &flags);
      } else
      if (!strcmp(Tirex_KEY(this), "call_arguments")) {
        hasCallArguments = true;
        while (type != YAML_SEQUENCE_END_EVENT)
          type = Tirex_PULL(this);
        continue;
        // TODO Parse call arguments
      } else */
        Tirex_ERROR(this, "expecting 'defs' or 'uses' or 'clobbers'");
      if (type != YAML_SEQUENCE_END_EVENT)
        Tirex_ERROR(this, "expecting SEQUENCE_END");
    } else
    if (type == YAML_SCALAR_EVENT) {
      const char *mapping_value = Tirex_EVENT_VALUE(this);
      if (!strcmp(Tirex_KEY(this), "node")) {
      } else
      if (!strcmp(Tirex_KEY(this), "asm")) {
        arguments[argCount++] = Tirex_makeSymbolTemporary(this, mapping_value, 0);
      } else
        Tirex_ERROR(this, "expecting 'node' or 'asm'");
    } else
      Tirex_ERROR(this, "expecting SCALAR or SEQUENCE_START");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(defs),SEQUENCE_START
 * Parse (SCALAR)+
 */
yaml_event_type_t
Tirex_parseOperationDefs(Tirex this, Operator operator, int *_resCount, Temporary results[]);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseOperationDefs(Tirex this, Operator operator, int *_resCount, Temporary results[])
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  //
  Except_CHECK(!strcmp(Tirex_KEY(this), "defs"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char * name = Tirex_EVENT_VALUE(this);
      Temporary temporary = Tirex_parseDefTemporary(this, name);
      if (*_resCount >= Tirex_MAX_RESCOUNT)
        Tirex_ERROR(this, "more than Tirex_MAX_RESCOUNT defs");
      results[(*_resCount)++] = temporary;
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
Temporary
Tirex_parseDefTemporary(Tirex this, const char *name);
#endif//$XCC__h

Temporary
Tirex_parseDefTemporary(Tirex this, const char *name)
{
#ifdef YAML_ENABLED
  Temporary temporary = NULL;
  *Tirex__VALUE(this) = name;
  if (*name == '$') {
    // Dedicated Temporary
    return Tirex_makeDedicatedTemporary(this, name);
  } else
  if (*name == 'V') {
    const char *constraint = strchr(name + 1, '.');
    if (constraint != NULL) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
      Tirex_KEY(this)[constraint - name] = '\0';
      *Tirex__VALUE(this) = ++constraint;
      if (*constraint == '$') {
        // Assigned Temporary
        Register registre = Tirex_makeRegister(this, constraint);
        temporary = Tirex_makeAssignedTemporary(this, Tirex_KEY(this), registre);
      } else
        Tirex_ERROR(this, "unrecognized operand constraint '%s'");
    } else {
      // Virtual Temporary
      temporary = Tirex_makeVirtualTemporary(this, name);
    }
  } else
  if (*name == '+' || *name == '-' || isdigit(*name)) {
    // Immediates may be encountered in a 'Def' in case of a memory write
    temporary = Tirex_makeAbsoluteTemporary(this, name);
  } else
    Tirex_ERROR(this, "expecting def operand starting with '$' or 'T'");
  return temporary;
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(uses),SEQUENCE_START
 * Parse (SCALAR|(SEQUENCE_START..SEQUENCE_END)*)
 */
yaml_event_type_t
Tirex_parseOperationUses(Tirex this, Operator operator, int *_argCount, Temporary arguments[]);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseOperationUses(Tirex this, Operator operator, int *_argCount, Temporary arguments[])
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  //
  Except_CHECK(!strcmp(Tirex_KEY(this), "uses"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char * name = Tirex_EVENT_VALUE(this);
      Temporary temporary = Tirex_parseUseTemporary(this, name);
      if (*_argCount >= Tirex_MAX_RESCOUNT)
        Tirex_ERROR(this, "more than Tirex_MAX_RESCOUNT defs");
      arguments[(*_argCount)++] = temporary;
    } else
    if (type == YAML_SEQUENCE_START_EVENT) {
      type = Tirex_parseUseSymbol(this, _argCount, arguments);
      if (type != YAML_SEQUENCE_END_EVENT)
        Tirex_ERROR(this, "expecting SEQUENCE_END");
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
Temporary
Tirex_parseUseTemporary(Tirex this, const char *name);
#endif//$XCC__h

Temporary
Tirex_parseUseTemporary(Tirex this, const char *name)
{
#ifdef YAML_ENABLED
  Temporary temporary = NULL;
  *Tirex__VALUE(this) = name;
  if (*name == '$') {
    // Dedicated Temporary
    return Tirex_makeDedicatedTemporary(this, name);
  } else
  if (*name == 'V') {
    const char *constraint = strchr(name + 1, '.');
    if (constraint != NULL) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
      Tirex_KEY(this)[constraint - name] = '\0';
      *Tirex__VALUE(this) = ++constraint;
      if (*constraint == '$') {
        // Assigned Temporary
        Register registre = Tirex_makeRegister(this, constraint);
        temporary = Tirex_makeAssignedTemporary(this, Tirex_KEY(this), registre);
      } else
        Tirex_ERROR(this, "unrecognized operand constraint '%s'");
    } else {
      // Virtual Temporary
      temporary = Tirex_makeVirtualTemporary(this, name);
    }
  } else
  if (*name == '.') {
    // Label Temporary
    temporary = Tirex_makeLabelTemporary(this, name);
  } else
  if (*name == '+' || *name == '-' || isdigit(*name)) {
    // Absolute Temporary
    temporary = Tirex_makeAbsoluteTemporary(this, name);
  } else
    Tirex_ERROR(this, "expecting use operand starting with '$' or 'T' or '.' or a number");
  return temporary;
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SEQUENCE_START
 * Parse (SCALAR){1,3}
 */
yaml_event_type_t
Tirex_parseUseSymbol(Tirex this, int *_argCount, Temporary arguments[]);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseUseSymbol(Tirex this, int *_argCount, Temporary arguments[])
{
#ifdef YAML_ENABLED
  int i;
  int64_t offset = 0;
  Symbol symbol = NULL;
  Relocation relocation = Relocation__UNDEF;
  Procedure procedure = Tirex_PROCEDURE(this);
  Temporary temporary = NULL;
  //
  yaml_event_type_t type = YAML_NO_EVENT;
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    // Symbol
    const char *name = Tirex_EVENT_VALUE(this);
    symbol = Tirex_makeSymbol(this, name);
    for (i = 0; i < 3; i++) { // Maximum 2 arguments and SEQUENCE_END (relocation and offset)
      type = Tirex_PULL(this);
      if (type == YAML_SCALAR_EVENT) {
        const char * mapping_value = Tirex_EVENT_VALUE(this);
        if (isdigit(mapping_value[1])) {
          // Offset
          offset = strtoll(Tirex_EVENT_VALUE(this), NULL, 0);
        } else if (mapping_value[0] == '@') {
          // Relocation
#ifdef MDT_TARGET_k1b
          char relocationId[32] = "k1b_";
#endif//MDT_TARGET_k1b
#ifdef MDT_TARGET_kv3
          char relocationId[32] = "kv3_";
#endif//MDT_TARGET_kv3
          const char *name = Tirex_EVENT_VALUE(this);
          strcat(relocationId, name + 1);
          relocation = Relocation_ofId(relocationId);
        } else
          Tirex_ERROR(this, "expected offset value or relocation symbol starting with '@'");
      } else
      if (type == YAML_SEQUENCE_END_EVENT) {
        temporary = Procedure_makeSymbolTemporary(procedure, symbol, offset, Immediate__UNDEF);
        if (relocation != Relocation__UNDEF)
          Literal_setRelocation(Temporary_LITERAL(temporary), relocation);
        arguments[(*_argCount)++] = temporary;
        return type;
      } else
        Tirex_ERROR(this, "expected relocation symbol, offset value or SEQUENCE_END");
    }
  } else
    Tirex_ERROR(this, "expecting SCALAR");
  return YAML_NO_EVENT;
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(uses),SEQUENCE_START
 * Parse (SCALAR|(SEQUENCE_START..SEQUENCE_END)*)
 */
yaml_event_type_t
Tirex_parseOperationClobbers(Tirex this, RegisterSet clobberSet);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseOperationClobbers(Tirex this,  RegisterSet clobberSet)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "clobbers"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      Processor processor = Tirex_PROCESSOR(this);
      if (RegisterSet_addRange(clobberSet, Tirex_EVENT_VALUE(this), processor));
      else {
        *Tirex__VALUE(this) = Tirex_EVENT_VALUE(this);
        Tirex_ERROR(this, "invalid register range '%s'");
      }
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(flags),SEQUENCE_START
 * Parse (SCALAR)*
 */
yaml_event_type_t
Tirex_parseOperationFlags(Tirex this, OperationFlags *_flags);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseOperationFlags(Tirex this, OperationFlags *_flags)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "flags"));
  OperationFlags flag = 0;
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      if (!ETable_parseFlags(OperationFlag_Table, Tirex_EVENT_VALUE(this), ',', &flag)) {
        *Tirex__VALUE(this) = Tirex_EVENT_VALUE(this);
        Tirex_ERROR(this, "invalid operation flag '%s'");
      }
      *_flags |= flag;
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(loops),SEQUENCE_START
 * Parse (MAPPING_START,SCALAR,..MAPPING_END)*
 */
yaml_event_type_t
Tirex_parseFunctionLoops(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseFunctionLoops(Tirex this)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "loops"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_MAPPING_START_EVENT) {
    } else
      return type;
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
      if (!strcmp(Tirex_KEY(this), "loop")) {
        type = Tirex_parseBodyLoop(this);
      } else
        Tirex_ERROR(this, "expecting 'loop'");
      if (type != YAML_MAPPING_END_EVENT)
        Tirex_ERROR(this, "expecting MAPPING_END");
    } else
      Tirex_ERROR(this, "expecting SCALAR");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after MAPPING_START,SCALAR(loop)
 * Parse SCALAR,(SCALAR,SCALAR|MAPPING_START..MAPPING_END|SEQUENCE_START..SEQUENCE_END)+
 */
yaml_event_type_t
Tirex_parseBodyLoop(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseBodyLoop(Tirex this)
{
#ifdef YAML_ENABLED
  int blockIdx = 0;
  char loopName[32];
  bool loopHasHeader = false;
  BasicBlock blocks[1024], headerBlock;
  LoopScope parentScope = NULL;
  LoopScopeFlags loopFlags = 0x0;
  //
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "loop"));
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    const char *mapping_value = Tirex_EVENT_VALUE(this);
    if (mapping_value[0] == 'L')
      strcpy(loopName, mapping_value);
    else
      Tirex_ERROR(this, "loop name is not of form 'L<n>'");
  } else
    Tirex_ERROR(this, "expecting loop name 'L<n>'");
  //
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
    } else {
      if (loopHasHeader) {
        // Construct the LoopScope object from the gathered information */
        LoopScope loopScope = Tirex_makeLoopScope(this, loopName, headerBlock);
        LoopScope_setFlags(loopScope, (LoopScopeFlags) loopFlags);
        LoopScope_PARENT(loopScope) = parentScope;
        LoopScope_BLOCKCOUNT(loopScope) = blockIdx;
        for (int i = 0; i < blockIdx; i++)
          BasicBlock_setLoopScope(blocks[i], loopScope);
        return type;
      } else
        Tirex_ERROR(this, "loop declaration does not indicate loop header block");
    }
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char *mapping_value = Tirex_EVENT_VALUE(this);
      if (!strcmp(Tirex_KEY(this), "parent")) {
        // This assumes parent loops are always declared before their children
        parentScope = Tirex_makeLoopScope(this, mapping_value, NULL);
      } else
      if (!strcmp(Tirex_KEY(this), "header")) {
        headerBlock = Tirex_makeBasicBlock(this, mapping_value, false);
        loopHasHeader = true;
      } else
        Tirex_ERROR(this, "expecting 'parent', 'header'");
    } else
    if (type == YAML_SEQUENCE_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "flags")) {
        type = Tirex_parseLoopFlags(this, &loopFlags);
      } else
      if (!strcmp(Tirex_KEY(this), "body")) {
        type = Tirex_parseLoopBody(this, blocks, &blockIdx);
      } else
      if (!strcmp(Tirex_KEY(this), "nodes")) {
        // FIXME fix argument parsing cannot pass loopScope as it does not exist yet
        type = Tirex_parseLoopNodes(this, NULL);
      } else
      if (!strcmp(Tirex_KEY(this), "arcs")) {
        // FIXME fix argument parsing cannot pass loopScope as it does not exist yet
        type = Tirex_parseLoopArcs(this, NULL);
      } else
        Tirex_ERROR(this, "expecting 'flags', 'body', 'nodes' or 'arcs'");
      if (type != YAML_SEQUENCE_END_EVENT)
        Tirex_ERROR(this, "expecting SEQUENCE_END");
    } else
      Tirex_ERROR(this, "expecting SCALAR or MAPPING_START or SEQUENCE_START");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(flags),SEQUENCE_START
 * Parse (SCALAR)*
 */
yaml_event_type_t
Tirex_parseLoopFlags(Tirex this, LoopScopeFlags * flags);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseLoopFlags(Tirex this, LoopScopeFlags * flags)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "flags"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      if (ETable_parseFlags(LoopScopeFlag_Table, Tirex_EVENT_VALUE(this), ',', (uint32_t *) flags)) {
      } else
        Tirex_ERROR(this, "Unkown LoopScope flag '%s'");
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(body),SEQUENCE_START
 * Parse (SCALAR)*
 */
yaml_event_type_t
Tirex_parseLoopBody(Tirex this, BasicBlock * blocks, int * blockIdx);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseLoopBody(Tirex this, BasicBlock * blocks, int * blockIdx)
{
#ifdef YAML_ENABLED
  int block_number = 0;
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "body"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char *mapping_value = Tirex_EVENT_VALUE(this);
      if (mapping_value[0] == 'B') {
        blocks[(*blockIdx)++] = Tirex_makeBasicBlock(this, mapping_value, false);
      } else
      if (mapping_value[0] == 'L') {
        // Do nothing specific as child loops are not declared yet
      } else
        Tirex_ERROR(this, "expecting 'B<n>' or 'L<n>'");
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(nodes),SEQUENCE_START
 * Parse (SCALAR)*
 */
yaml_event_type_t
Tirex_parseLoopNodes(Tirex this, LoopScope loopScope);
// TODO Has to be written. Waiting for GCC's dependency output first.
#endif//$XCC__h

yaml_event_type_t
Tirex_parseLoopNodes(Tirex this, LoopScope loopScope)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "nodes"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(arcs),SEQUENCE_START
 * Parse (SEQUENCE_START..SEQUENCE_END)*
 */
yaml_event_type_t
Tirex_parseLoopArcs(Tirex this, LoopScope loopScope);
// TODO Has to be written. Waiting for GCC's dependency output first.
#endif//$XCC__h

yaml_event_type_t
Tirex_parseLoopArcs(Tirex this, LoopScope loopScope)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "arcs"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SEQUENCE_START_EVENT) {
      type = Tirex_parseLoopArc(this, loopScope);
      if (type != YAML_SEQUENCE_END_EVENT)
        Tirex_ERROR(this, "expecting SEQUENCE_END");
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SEQUENCE_START
 * Parse (SCALAR)*
 */
yaml_event_type_t
Tirex_parseLoopArc(Tirex this, LoopScope loopScope);
// TODO Has to be written. Waiting for GCC's dependency output first.
#endif//$XCC__h

yaml_event_type_t
Tirex_parseLoopArc(Tirex this, LoopScope loopScope)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after MAPPING_START,SCALAR(section)
 * Parse (MAPPING_START...MAPPING_END)
 */
yaml_event_type_t
Tirex_parseSection(Tirex this, char ** nameCallback);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseSection(Tirex this, char ** nameCallback)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  int sectionAlign = 0;
  uint8_t entSize = 0;
  char sectionName[128];
  DataSection section = NULL;
  DataSectionType sectionType = DataSectionType_UNDEF;
  DataSectionFlags sectionFlags = 0;
  //
  Except_CHECK(!strcmp(Tirex_KEY(this), "section"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
    } else {
      if (sectionName[0] == '\0')
        Tirex_ERROR(this, "section definition without compulsory 'name' field");
      Program program = Tirex_PROGRAM(this);
      section = Program_makeSection(program, sectionName, sectionAlign, sectionFlags, sectionType, entSize);
      if (nameCallback)
        *nameCallback = (char *) DataSection_name(section);
      return type;
    }
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char * mapping_value = Tirex_EVENT_VALUE(this);
      if (!strcmp(Tirex_KEY(this), "name")) {
        strcpy(sectionName, Tirex_EVENT_VALUE(this));
      } else
      if (!strcmp(Tirex_KEY(this), "align")) {
        if (isdigit(mapping_value[0])) {
          sectionAlign = atoi(mapping_value);
        } else
          Tirex_ERROR(this, "expecting alignment parameter as an integer");
      } else
      if (!strcmp(Tirex_KEY(this), "type")) {
        if (!strcmp(mapping_value, "progbits")) {
          sectionType = DataSectionType_progbits;
        } else
        if (!strcmp(mapping_value, "nobits")) {
          sectionType = DataSectionType_nobits;
        } else
        if (!strcmp(mapping_value, "note")) {
          sectionType = DataSectionType_note;
        } else
          Tirex_ERROR(this, "unknown section type specified");
      } else
      if (!strcmp(Tirex_KEY(this), "element-size")) {
        entSize = atoi(mapping_value);
      } else
        Tirex_ERROR(this, "expecting 'align', 'type' or 'element-size'");
    } else
    if (type == YAML_SEQUENCE_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "flags")) {
        type = Tirex_parseSectionFlags(this, &sectionFlags);
        if (type != YAML_SEQUENCE_END_EVENT)
          Tirex_ERROR(this, "expecting SEQUENCE_END");
      } else
        Tirex_ERROR(this, "expecting 'flags'");
    } else
      Tirex_ERROR(this, "expecting SCALAR o SEQUENCE_START");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(flags),SEQUENCE_START
 * Parse (SCALAR)*
 */
yaml_event_type_t
Tirex_parseSectionFlags(Tirex this, DataSectionFlags *flags);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseSectionFlags(Tirex this, DataSectionFlags *flags)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "flags"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      if (!strcmp(Tirex_EVENT_VALUE(this), "Alloc")) {
        *flags |= DataSectionFlag_Alloc;
      } else
      if (!strcmp(Tirex_EVENT_VALUE(this), "Write")) {
        *flags |= DataSectionFlag_Write;
      } else
      if (!strcmp(Tirex_EVENT_VALUE(this), "Exec")) {
        *flags |= DataSectionFlag_Exec;
      } else
      if (!strcmp(Tirex_EVENT_VALUE(this), "Small")) {
        *flags |= DataSectionFlag_Small;
      } else
      if (!strcmp(Tirex_EVENT_VALUE(this), "Merge")) {
        *flags |= DataSectionFlag_Merge;
      } else
      if (!strcmp(Tirex_EVENT_VALUE(this), "Strings")) {
        *flags |= DataSectionFlag_Strings;
      } else
      if (!strcmp(Tirex_EVENT_VALUE(this), "TLS")) {
        *flags |= DataSectionFlag_TLS;
      } else
      if (!strcmp(Tirex_EVENT_VALUE(this), "Group")) {
        *flags |= DataSectionFlag_Group;
      } else
        Tirex_ERROR(this, "encountered unknown flag in section definition");
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after MAPPING_START,SCALAR(common)
 * Parse SCALAR,(SCALAR,SCALAR)*
 */
yaml_event_type_t
Tirex_parseCommon(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseCommon(Tirex this)
{
#ifdef YAML_ENABLED
  Symbol objectSymbol = NULL;
  int objectAlign = 0;
  int objectSize = 0;
  const char * mapping_value;
  //
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "common"));
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    mapping_value = Tirex_EVENT_VALUE(this);
    objectSymbol = SymbolTable_makeSymbol(Program_symbolTable(Tirex_PROGRAM(this)), mapping_value);
    Symbol_setClass(objectSymbol, SymbolClass_VAR);
    Symbol_setExport(objectSymbol, SymbolExport_GLOBAL_PROTECTED);
    Symbol_setStore(objectSymbol, SymbolStore_COMMON);
  } else
    Tirex_ERROR(this, "expecting SCALAR");
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
    } else {
        Program_makeData(Tirex_PROGRAM(this), objectSymbol, objectSize, objectAlign, NULL);
        return type;
    }
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      mapping_value = Tirex_EVENT_VALUE(this);
      if (!strcmp(Tirex_KEY(this), "size")) {
        objectSize = atoi(mapping_value);
        Symbol_setSize(objectSymbol, objectSize);
      } else
      if (!strcmp(Tirex_KEY(this), "align")) {
        objectAlign = atoi(mapping_value);
      } else
      if (!strcmp(Tirex_KEY(this), "export")) {
        // FIXME : code influence of field
      } else
        Tirex_ERROR(this, "expecting 'size' or 'align'");
    } else
      Tirex_ERROR(this, "expecting SCALAR");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after MAPPING_START,SCALAR(object)
 * Parse SCALAR,(SCALAR,(SCALAR|SEQUENCE_START..SEQUENCE_END))*
 */
yaml_event_type_t
Tirex_parseObject(Tirex this);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseObject(Tirex this)
{
#ifdef YAML_ENABLED
  bool instantiated = false;
  size_t objectSize = 0;
  uint8_t objectAlign = 0;
  Data object = NULL;
  Symbol objectSymbol = NULL;
  Program program = Tirex_PROGRAM(this);
  DataSection objectSection = NULL;
  //
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "object"));
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    const char *mapping_value = Tirex_EVENT_VALUE(this);
    objectSymbol = SymbolTable_makeSymbol(Program_symbolTable(Tirex_PROGRAM(this)), mapping_value);
    *Symbol__CLASS(objectSymbol) = SymbolClass_VAR;
    *Symbol__EXPORT(objectSymbol) = SymbolExport_LOCAL;
    *Symbol__STORE(objectSymbol) = SymbolStore_UGLOBAL;
  } else
    Tirex_ERROR(this, "expecting SCALAR");
  //
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
    } else {
      if (!instantiated)
        object = Program_makeData(Tirex_PROGRAM(this), objectSymbol, objectSize, objectAlign, objectSection);
      return type;
    }
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char * mapping_value = Tirex_EVENT_VALUE(this);
      if (!strcmp(Tirex_KEY(this), "section")) {
        int i;
        objectSection = Program_searchSection(program, mapping_value);
        if (objectSection == NULL)
          Tirex_ERROR(this, "object belongs to an undefined section");
      } else
      if (!strcmp(Tirex_KEY(this), "linkage")) {
        if (!strcmp(mapping_value, "weak")) {
          *Symbol__EXPORT(objectSymbol) = SymbolExport_WEAK;
        } else
        if (!strcmp(mapping_value, "local")) {
          *Symbol__EXPORT(objectSymbol) = SymbolExport_LOCAL;
        } else
        if (!strcmp(mapping_value, "global")) {
          *Symbol__EXPORT(objectSymbol) = SymbolExport_GLOBAL_PREEMPTIBLE;
        } else
        if (!strcmp(mapping_value, "global-protected")) {
          *Symbol__EXPORT(objectSymbol) = SymbolExport_GLOBAL_PROTECTED;
        } else
        if (!strcmp(mapping_value, "global-hidden")) {
          *Symbol__EXPORT(objectSymbol) = SymbolExport_GLOBAL_HIDDEN;
        } else
        if (!strcmp(mapping_value, "global-internal")) {
          *Symbol__EXPORT(objectSymbol) = SymbolExport_GLOBAL_INTERNAL;
        } else
          Tirex_ERROR(this, "unknown linkage type");
      } else
      if (!strcmp(Tirex_KEY(this), "size")) {
        objectSize = (size_t) strtol(mapping_value, NULL, 10);
      } else
      if (!strcmp(Tirex_KEY(this), "align")) {
        objectAlign = (uint8_t) strtoul(mapping_value, NULL, 10);
      } else
      if (!strcmp(Tirex_KEY(this), "attr")) {
        /* No attributed managed for the moment */
      } else
        Tirex_ERROR(this, "expecting 'section', 'storage', 'size', 'align' or 'attr'");
    } else
    if (type == YAML_SEQUENCE_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "init")) {
        *Symbol__STORE(objectSymbol) = SymbolStore_DGLOBAL;
        /* Create the object before initializing it */
        object = Program_makeData(Tirex_PROGRAM(this), objectSymbol, objectSize, objectAlign, objectSection);
        instantiated = true;
        type = Tirex_parseObjectInit(this, object);
        if (type != YAML_SEQUENCE_END_EVENT)
          Tirex_ERROR(this, "expecting SEQUENCE_END");
      } else
        Tirex_ERROR(this, "expecting 'init'");
    } else
    if (type == YAML_MAPPING_START_EVENT) {
      if (!strcmp(Tirex_KEY(this), "section")) {
        char * sectionName;
        type = Tirex_parseSection(this, &sectionName);
        if (type != YAML_MAPPING_END_EVENT)
          Tirex_ERROR(this, "expecting MAPPING_END");
        objectSection = Program_searchSection(program, sectionName);
        if (objectSection == NULL)
          Tirex_ERROR(this, "object belongs to an undefined section");
      } else
        Tirex_ERROR(this, "expecting 'section' with the section definition");
    } else
    Tirex_ERROR(this, "expecting SCALAR, SEQUENCE_START or MAPPING_START");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR(init),SEQUENCE_START
 * Parse (MAPPING_START,SCALAR,..MAPPING_END)*
 */
yaml_event_type_t
Tirex_parseObjectInit(Tirex this, Data data);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseObjectInit(Tirex this, Data data)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  Except_CHECK(!strcmp(Tirex_KEY(this), "init"));
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_MAPPING_START_EVENT) {
    } else
      return type;
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      strncpy(Tirex_KEY(this), Tirex_EVENT_VALUE(this), Tirex_KEYLENGTH);
      if (!strcmp(Tirex_KEY(this), "byte")) {
        type = Tirex_parseInitValues(this, data, DataInitType_byte);
      } else
      if (!strcmp(Tirex_KEY(this), "s8")) {
        type = Tirex_parseInitValues(this, data, DataInitType_sbyte);
      } else
      if (!strcmp(Tirex_KEY(this), "u8")) {
        type = Tirex_parseInitValues(this, data, DataInitType_ubyte);
      } else
      if (!strcmp(Tirex_KEY(this), "hword")) {
        type = Tirex_parseInitValues(this, data, DataInitType_hword);
      } else
      if (!strcmp(Tirex_KEY(this), "s16")) {
        type = Tirex_parseInitValues(this, data, DataInitType_shword);
      } else
      if (!strcmp(Tirex_KEY(this), "u16")) {
        type = Tirex_parseInitValues(this, data, DataInitType_uhword);
      } else
      if (!strcmp(Tirex_KEY(this), "word")) {
        type = Tirex_parseInitValues(this, data, DataInitType_word);
      } else
      if (!strcmp(Tirex_KEY(this), "s32")) {
        type = Tirex_parseInitValues(this, data, DataInitType_sword);
      } else
      if (!strcmp(Tirex_KEY(this), "u32")) {
        type = Tirex_parseInitValues(this, data, DataInitType_uword);
      } else
      if (!strcmp(Tirex_KEY(this), "quad")) {
        type = Tirex_parseInitValues(this, data, DataInitType_quad);
      } else
      if (!strcmp(Tirex_KEY(this), "s64")) {
        type = Tirex_parseInitValues(this, data, DataInitType_squad);
      } else
      if (!strcmp(Tirex_KEY(this), "u64")) {
        type = Tirex_parseInitValues(this, data, DataInitType_uquad);
      } else
      if (!strcmp(Tirex_KEY(this), "f32")) {
        type = Tirex_parseInitValues(this, data, DataInitType_float);
      } else
      if (!strcmp(Tirex_KEY(this), "f64")) {
        type = Tirex_parseInitValues(this, data, DataInitType_double);
      } else
      if (!strcmp(Tirex_KEY(this), "ascii")) {
        type = Tirex_parseInitString(this, data, DataInitType_ascii);
      } else
      if (!strcmp(Tirex_KEY(this), "string")) {
        type = Tirex_parseInitString(this, data, DataInitType_string);
      } else
      if (!strcmp(Tirex_KEY(this), "space")) {
        type = Tirex_parseInitSpace(this, data);
      } else
        Tirex_ERROR(this, "expecting an initializer key ('byte', 'u32', 'f64', 'string', etc...)");
      if (type != YAML_MAPPING_END_EVENT)
        Tirex_ERROR(this, "expecting MAPPING_END");
    } else
      Tirex_ERROR(this, "expecting SCALAR");
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after MAPPING_START,SCALAR(byte|hword|word|dword)
 * Parse (SCALAR|SEQUENCE_START,...,SEQUENCE_END)*
 */
yaml_event_type_t
Tirex_parseInitValues(Tirex this, Data data, DataInitType dataType);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseInitValues(Tirex this, Data data, DataInitType dataType)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      char *mapping_value = (char *) Tirex_EVENT_VALUE(this);
      char real_ieee[64];
      char * ptr;
      unsigned int * encoded_ieee;
      char parseValue[16] = {'\0'};
      
      switch (dataType) {
        case DataInitType_byte:
          sscanf(mapping_value, "%hhx", (unsigned char *) parseValue);
          Data_addInitializer(data, dataType, (void *) parseValue, NULL, 1);
          break;
        case DataInitType_sbyte:
          sscanf(mapping_value, "%hhd", (signed char *) parseValue);
          Data_addInitializer(data, dataType, (void *) parseValue, NULL, 1);
          break;
        case DataInitType_ubyte:
          sscanf(mapping_value, "%hhu", (unsigned char *) parseValue);
          Data_addInitializer(data, dataType, (void *) parseValue, NULL, 1);
          break;
        case DataInitType_hword:
          sscanf(mapping_value, "%hx", (unsigned short int *) parseValue);
          Data_addInitializer(data, dataType, (void *) parseValue, NULL, 2);
          break;
        case DataInitType_shword:
          sscanf(mapping_value, "%hd", (short int *) parseValue);
          Data_addInitializer(data, dataType, (void *) parseValue, NULL, 2);
          break;
        case DataInitType_uhword:
          sscanf(mapping_value, "%hu", (unsigned short int *) parseValue);
          Data_addInitializer(data, dataType, (void *) parseValue, NULL, 2);
          break;
        case DataInitType_word:
          if ((*mapping_value == '+') || (*mapping_value == '-') || (isdigit(*mapping_value))) {
            sscanf(mapping_value, "%lx", (unsigned long int *) parseValue);
            Data_addInitializer(data, dataType, (void *) parseValue, NULL, 4);
          } else if (*mapping_value == '.') {
            Temporary temporary = Tirex_makeLabelTemporary(this, mapping_value);
            Data_addInitializer(data, DataInitType_symbol, NULL, temporary, 0);
          } else
            Tirex_ERROR(this, "unrecognized 'word' initializer value");
          break;
        case DataInitType_sword:
          sscanf(mapping_value, "%d", (int *) parseValue);
          Data_addInitializer(data, dataType, (void *) parseValue, NULL, 4);
          break;
        case DataInitType_uword:
          sscanf(mapping_value, "%u", (unsigned int *) parseValue);
          Data_addInitializer(data, dataType, (void *) parseValue, NULL, 4);
          break;
        case DataInitType_quad:
          if ((*mapping_value == '+') || (*mapping_value == '-') || (isdigit(*mapping_value))) {
            sscanf(mapping_value, "%llx", (unsigned long long int *) parseValue);
            Data_addInitializer(data, dataType, (void *) parseValue, NULL, 8);
          } else if (*mapping_value == '.') {
            Temporary temporary = Tirex_makeLabelTemporary(this, mapping_value);
            Data_addInitializer(data, DataInitType_symbol, NULL, temporary, 0);
          } else
            Tirex_ERROR(this, "unrecognized 'word' initializer value");
          break;
        case DataInitType_squad:
          sscanf(mapping_value, "%ld", (long int *) parseValue);
          Data_addInitializer(data, dataType, (void *) parseValue, NULL, 8);
          break;
        case DataInitType_uquad:
          sscanf(mapping_value, "%lu", (unsigned long int *) parseValue);
          Data_addInitializer(data, dataType, (void *) parseValue, NULL, 8);
          break;
        case DataInitType_float:
        case DataInitType_double:
          {
            int i, exp = 0, maxExp, maskSize;
            char temp[9] = {'\0'};
            char * dot = NULL;
            unsigned int mask;
            unsigned int sig = 0, sig2 = 0, sig3 = 0, sig4 = 0;
            unsigned int encoded_ieee[2] = {0};
            strcpy(real_ieee, mapping_value);
            for (i = 0; i < strlen(real_ieee); i++)
              real_ieee[i] = tolower(real_ieee[i]);
            if (!strncmp(real_ieee, "inf", 3)) {
              encoded_ieee[1] = 0x7f800000;
            } else if (!strncmp(real_ieee, "-inf", 4)) {
              encoded_ieee[1] = 0xff800000;
            } else if (!strncmp(real_ieee, "nan", 3)) {
              encoded_ieee[1] = 0x7f800001;
            } else if (!strncmp(real_ieee, "-nan", 4)) {
              encoded_ieee[1] = 0xff800001;
            } else {
              ptr = real_ieee;
              while (isblank(ptr[0]))
                ptr++;
              if (!strncmp(ptr, "-0x", 3)) {
                encoded_ieee[1] = 0x1 << 31;
                ptr += 3;
              } else if (!strncmp(ptr, "0x", 2)) {
                ptr += 2;
              } else {
                Tirex_ERROR(this, "unrecognized floating-point in initializer");
              }
              if (strchr(ptr, (int) '.')) { /* Significand has an integer part */
                /* Integer part */
                ptr = strtok(ptr, ".");
                if (strlen(ptr) > 8) {
                  strncpy(temp, ptr, strlen(ptr) - 8);
                  temp[strlen(ptr) - 8] = '\0';
                  sscanf(temp, "%x", &sig4);
                  strncpy(temp, ptr + strlen(ptr) - 8, 8);
                  sscanf(temp, "%x", &sig3);
                } else if (strlen(ptr)) { /* Do not count empty values */
                  sscanf(ptr, "%x", &sig3);
                }
                /* Fractional part */
                ptr = strtok(NULL, "p");
                if (strlen(ptr) > 8) {
                  strncpy(temp, ptr, 8);
                  sscanf(temp, "%x", &sig2);
                  strncpy(temp, ptr + 8, strlen(ptr) - 8);
                  temp[strlen(ptr) - 8] = '\0';
                  sscanf(temp, "%x", &sig);
                  sig <<= 4 * (16 - (strlen(ptr)));
                } else if (strlen(ptr)) { /* Do not count empty values */
                  sscanf(ptr, "%x", &sig2);
                  sig2 <<= 4 * (8 - (strlen(ptr)));
                }
              } else { /* No integer part */
                /* Fractional part */
                ptr = strtok(ptr, "p");
                if (strlen(ptr) > 8) {
                  strncpy(temp, ptr, 8);
                  sscanf(temp, "%x", &sig2);
                  strncpy(temp, ptr + 8, strlen(ptr) - 8);
                  temp[strlen(ptr) - 8] = '\0';
                  sscanf(temp, "%x", &sig);
                  sig <<= 4 * (16 - (strlen(ptr)));
                } else if (strlen(ptr)) { /* Do not count empty values */
                  sscanf(ptr, "%x", &sig2);
                  sig2 <<= 4 * (8 - (strlen(ptr)));
                }
              }
              /* Exponent */
              ptr = strtok(NULL, " \n");
              sscanf(ptr, "%d", &exp);
              maxExp = (dataType == DataInitType_float) ? 128 : 1024;
              if ((sig3 != 0) || (sig4 != 0)) {
                while (((sig4 != 0x0) || (sig3 != 0x1)) && (exp < maxExp)) {
                  sig = (sig >> 1) | (sig2 << 31);
                  sig2 = (sig2 >> 1) | (sig3 << 31);
                  sig3 = (sig3 >> 1) | (sig4 << 31);
                  sig4 >>= 1;
                  exp++;
                }
              } else {
                while ((sig3 != 0x1) && (exp > (1 - maxExp))) {
                  sig3 = sig2 >> 31;
                  sig2 = (sig2 << 1) | (sig >> 31);
                  sig <<= 1;
                  exp--;
                }
              }
              mask = (dataType == DataInitType_float) ? 0xff : 0x7ff;
              maskSize = (dataType == DataInitType_float) ? 8 : 11;
              if (exp > (maxExp - 1)) { /* Case of an unrecognized INFINITY */
                encoded_ieee[1] |= 0x7f800000;
                encoded_ieee[0] |= 0x0;
              } else if (exp > (0 - maxExp)) { /* Standard case */
                encoded_ieee[1] |= ((exp + (maxExp - 1)) & mask) << (31 - maskSize);
                encoded_ieee[1] |= (sig2 >> (maskSize + 1));
                encoded_ieee[0] |= (sig2 << (31 - maskSize)) | (sig >> (maskSize + 1));
              } else { /* Case of an unrecognized ZERO */
                /* Nothing to do */
              }
            }
            if (dataType == DataInitType_float)
              Data_addInitializer(data, dataType, (void *) &encoded_ieee[1], NULL, 4);
            else
              Data_addInitializer(data, dataType, (void *) encoded_ieee, NULL, 8);
            break;
          }
        default:
          Tirex_ERROR(this, "got unexpected initializer type");
      }
    } else
    if (type == YAML_SEQUENCE_START_EVENT) {
      switch (dataType) {
        case DataInitType_word:
        case DataInitType_sword:
        case DataInitType_uword:
        case DataInitType_quad:
        case DataInitType_squad:
        case DataInitType_uquad:
          type = Tirex_parseInitSymbol(this, data);
          break;
        default:
          Tirex_ERROR(this, "got a symbol as initializer of an uncompatible key");
      }
      if (type != YAML_SEQUENCE_END_EVENT)
        Tirex_ERROR(this, "expecting SEQUENCE_END");
    } else
      return type;
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SEQUENCE_START
 * Parse (SCALAR)+
 */
yaml_event_type_t
Tirex_parseInitSymbol(Tirex this, Data data);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseInitSymbol(Tirex this, Data data)
{
#ifdef YAML_ENABLED
  Symbol symbol = NULL;
  Relocation relocation = Relocation__UNDEF;
  int offset = 0;
  //
  yaml_event_type_t type = YAML_NO_EVENT;
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    SymbolTable symbolTable = Program_symbolTable(Tirex_PROGRAM(this));
    symbol = SymbolTable_search(symbolTable, (uint32_t)(uintptr_t)Tirex_EVENT_VALUE(this));
    if (symbol == NULL) { // Probably an external symbol
      symbol = SymbolTable_makeSymbol(symbolTable, (char *) Tirex_EVENT_VALUE(this));
      Symbol_setClasses(symbol, SymbolClass_VAR, SymbolStore__UNDEF, SymbolExport__UNDEF);
    }
  } else
    Tirex_ERROR(this, "expecting SCALAR");
  while (1) {
    type = Tirex_PULL(this);
    if (type == YAML_SCALAR_EVENT) {
      const char * mapping_value = Tirex_EVENT_VALUE(this);
      switch (mapping_value[0]) {
        case '@':
          {
#ifdef MDT_TARGET_k1b
            char relocationId[32] = "k1b_";
#endif//MDT_TARGET_k1b
#ifdef MDT_TARGET_kv3
            char relocationId[32] = "kv3_";
#endif//MDT_TARGET_kv3
            strcat(relocationId, mapping_value+1);
            relocation = Relocation_ofId(relocationId);
            Tirex_ERROR(this, "relocation not handled yet");
          }
          break;
        case '+':
        case '-':
          offset = atoi(mapping_value);
          break;
        default:
          Tirex_ERROR(this, "unrecognized element");
      }
    } else {
      Temporary temporary = Program_makeSymbolTemporary(Tirex_PROGRAM(this), symbol, offset, Immediate__UNDEF);
      if (relocation != Relocation__UNDEF)
        Literal_setRelocation(Temporary_LITERAL(temporary), relocation);
      Data_addInitializer(data, DataInitType_symbol, NULL, temporary, 0);
      return type;
    }
  }
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR_EVENT
 * Parse (SCALAR)
 */
yaml_event_type_t
Tirex_parseInitString(Tirex this, Data data, DataInitType dataType);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseInitString(Tirex this, Data data, DataInitType dataType)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    const char *mapping_value = Tirex_EVENT_VALUE(this);
    size_t dataSize = Tirex_EVENT_LENGTH(this);
    Data_addInitializer(data, dataType, (char *) mapping_value, NULL, dataSize);
  } else
    Tirex_ERROR(this, "expecting SCALAR");
  type = Tirex_PULL(this);
  return type;
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

#ifdef $XCC__h
/**
 * Called after SCALAR_EVENT
 * Parse (SCALAR)
 */
yaml_event_type_t
Tirex_parseInitSpace(Tirex this, Data data);
#endif//$XCC__h

yaml_event_type_t
Tirex_parseInitSpace(Tirex this, Data data)
{
#ifdef YAML_ENABLED
  yaml_event_type_t type = YAML_NO_EVENT;
  type = Tirex_PULL(this);
  if (type == YAML_SCALAR_EVENT) {
    const char *mapping_value = Tirex_EVENT_VALUE(this);
    size_t dataSize = atoi(mapping_value);
    void *space = malloc(dataSize * sizeof(char));
    memset(space, '0', dataSize);
    Data_addInitializer(data, DataInitType_space, space, NULL, dataSize);
  } else
    Tirex_ERROR(this, "expecting SCALAR");
  type = Tirex_PULL(this);
  return type;
#else//!YAML_ENABLED
  return 0;
#endif//YAML_ENABLED
}

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC__h
/**
 * Initialize Tirex module.
 */
#ifndef _NTRACE
void
Tirex_INIT(void);
#else//_NTRACE
#define Tirex_INIT()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
Tirex_INIT(void)
{
  if (GETENV("TIREX_TRACE")) {
    int file = 0;
    sscanf(GETENV("TIREX_TRACE"), "%d", &file);
    if (file == 1) TIREX_TRACE = stdout;
    else if (file == 2) TIREX_TRACE = stderr;
  }
}
#endif//_NTRACE

#ifdef $XCC__h
/**
 * Finalize Tirex module.
 */
#ifndef _NTRACE
void
Tirex_FINI(void);
#else//_NTRACE
#define Tirex_FINI()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
Tirex_FINI(void)
{
}
#endif//_NTRACE

#if XCC__C
static void
Tirex_TEST(void)
{
#include "LIR/Tirex_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Tirex_TEST);
  return 0;
}
#endif

