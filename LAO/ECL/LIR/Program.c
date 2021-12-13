#ifdef $XCC_h
/*
 * !!!!	Program.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu).
 * Duco van Amstel (duco.van-amstel@kalray.eu).
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
 * @brief Program implementation.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/Program.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#define MAX_FILENAME_LEN 62
#endif//$XCC_h


#ifdef $XCC_h
/*
  Program --	Representation of a program.
 */
struct Program_;
#endif//$XCC_h

#ifdef $XCC__h
struct Program_ {
  //@args	Memory memory, Processor processor, Convention convention
  LabelTable_ LABELTABLE[1];		//!< The LabelTable of this Program.
  SymbolTable_ SYMBOLTABLE[1];		//!< The SymbolTable of this Program.
  //@access MEMORY	SymbolTable_memory(Program_SYMBOLTABLE(this))
  //@access STRINGS	SymbolTable_strings(Program_SYMBOLTABLE(this))
  ProcedureTable_ PROCEDURETABLE[1];	//!< IndexedTable of Procedure(s).
  //@access PROCEDURECOUNT	ProcedureTable_count(Program_PROCEDURETABLE(this))
  IBList_ LITERALS[1];			//!< Literal(s) and Temporary(ies) for this Program.
  Temporary_ *DEDICATED;		//!< Dedicated Temporary(ies) for this Program.
  Optimize_ OPTIMIZE[1];
  Configure_ CONFIGURE[1];
  DataList_ DATALIST[1];			//!< Data sections of Program.
  //@access DATASIZE DataList_DATASIZE(Program_DATALIST(this))
  IBList_ SECTIONS[1];			//!< Sections' string table of this Program.
  Processor PROCESSOR;			//!< The target Processor.
  Convention CONVENTION;		//!< The target Convention.
  char FILENAME[MAX_FILENAME_LEN];	//!< The name of the file being compiled
};
#endif//$XCC__h

Program
Program_Ctor(Program this,
             Memory memory, Processor processor, Convention convention)
{
  LabelTable_Ctor(Program_LABELTABLE(this), memory, 512);
  SymbolTable_Ctor(Program_SYMBOLTABLE(this), memory, 512);
  ProcedureTable_Ctor(Program_PROCEDURETABLE(this), memory, 256);
  IBList_Ctor(Program_LITERALS(this), memory, sizeof(Literal_) + sizeof(Temporary_), 30);
  Optimize_Ctor(Program_OPTIMIZE(this));
  Configure_Ctor(Program_CONFIGURE(this));
  DataList_Ctor(Program_DATALIST(this), memory);
  IBList_Ctor(Program_SECTIONS(this), memory, sizeof(DataSection_), 4);
  *Program__PROCESSOR(this) = processor;
  *Program__CONVENTION(this) = convention;
  (*Program__FILENAME(this))[0] = '\0';
  {
    int registre;
    Convention convention = Program_CONVENTION(this);
    Temporary_ *dedicated = Memory_alloc(memory, sizeof(Temporary_)*Register__XCOUNT);
    const_RegisterSet wiredSet = Convention_roleRegisterSet(convention, ConventionRole_Wired);
    memset(dedicated, 0, sizeof(Temporary_));
    for (registre = 1; registre < Register__Count; registre++) {
      TemporaryFlags flags = RegisterSet_contains(wiredSet, registre)?
                             TemporaryFlag_Constant + TemporaryFlag_Register:
                             TemporaryFlag_Register;
      Temporary temporary = Temporary_Ctor(dedicated + registre, TemporaryType_Dedicated, flags);
      *Temporary__REGFILE(temporary) = Register_regFile(registre, processor);
      *Temporary__REGISTER(temporary) = registre;
    }
    Temporary_Ctor(dedicated + Register__Volatile, TemporaryType_Volatile, 0);
    Temporary_Ctor(dedicated + Register__Control, TemporaryType_Control, 0);
    Temporary_Ctor(dedicated + Register__Memory, TemporaryType_Memory, 0);
    *Program__DEDICATED(this) = dedicated;
  }
  return this;
}

void
Program_Dtor(Program this)
{
  Memory memory = Program_MEMORY(this);
  LabelTable_Dtor(Program_LABELTABLE(this));
  SymbolTable_Dtor(Program_SYMBOLTABLE(this));
  Program_FOREACH_Procedure(this, procedure) {
    Procedure_delete(procedure);
  } Program_ENDEACH_Procedure;
  ProcedureTable_Dtor(Program_PROCEDURETABLE(this));
  Memory_free_(memory, Program_DEDICATED(this));
  IBList_Dtor(Program_LITERALS(this));
  Optimize_Dtor(Program_OPTIMIZE(this));
  Configure_Dtor(Program_CONFIGURE(this));
  DataList_Dtor(Program_DATALIST(this));
  IBList_Dtor(Program_SECTIONS(this));
}

size_t
Program_Size(Memory memory, Processor processor, Convention convention)
{
  return sizeof(Program_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Program_)\t%zu\n", sizeof(Program_));
}
#endif//$XCC__c

#ifdef $XCC_h
Program
Program_new(Memory parent, Processor processor, Convention convention);
#endif//$XCC_h

Program
Program_new(Memory parent, Processor processor, Convention convention)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Program_Size(memory, processor, convention);
  void *alloc = Memory_alloc(memory, size);
  return Program_Ctor(alloc, memory, processor, convention);
}

#ifdef $XCC_h
Program
Program_delete(Program this);
#endif//$XCC_h

Program
Program_delete(Program this)
{
  if (this != NULL) {
    Memory memory = Program_MEMORY(this);
#ifndef _NDTOR
    Program_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC__c
if (0) {
  int registre;
  Program program = Program_new(Memory_Root, (Processor)1, (Convention)1);
  Temporary_ *dedicated = Program_DEDICATED(program);
  for (registre = 1; registre < Register__Count; registre++) {
    Temporary temporary = dedicated + registre;
    Temporary_pretty(temporary, stdout);
    printf("\n");
  }
  Program_delete(program);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Program Memory.
 */
Memory
Program_memory(Program this);
#endif//$XCC_h

Memory
Program_memory(Program this)
{
  return Program_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This Program LabelTable.
 */
LabelTable
Program_labelTable(Program this);
#endif//$XCC_h

LabelTable
Program_labelTable(Program this)
{
  return Program_LABELTABLE(this);
}

#ifdef $XCC_h
/**
 * This Program SymbolTable.
 */
SymbolTable
Program_symbolTable(Program this);
#endif//$XCC_h

SymbolTable
Program_symbolTable(Program this)
{
  return Program_SYMBOLTABLE(this);
}

#ifdef $XCC_h
/**
 * For use by Program_FOREACH_Procedure.
 */
ProcedureTable
Program_procedureTable(Program this);
#endif//$XCC_h

ProcedureTable
Program_procedureTable(Program this)
{
  return Program_PROCEDURETABLE(this);
}

#ifdef $XCC_h
/**
 * Enumerate this Program Procedure(s).
 */
#define Program_FOREACH_Procedure(this, procedure) \
  ProcedureTable_FOREACH_Procedure(Program_procedureTable(this), procedure)
#define Program_ENDEACH_Procedure \
  ProcedureTable_ENDEACH_Procedure
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Find a procedure in program by label name.
 */
Procedure
Program_findProcedure(Program this, const char *name);
#endif//$XCC_h

Procedure
Program_findProcedure(Program this, const char *name)
{
  Program_FOREACH_Procedure(this, procedure) {
    Symbol symbol = Procedure_symbol(procedure);
    if(!strcmp(name, Symbol_name(symbol)))
      return procedure;
  } Program_ENDEACH_Procedure;
  Except_THROW("Could not find procedure %s in program!", name);
}

#ifdef $XCC_h
/**
 * This Program Optimize.
 */
Optimize
Program_optimize(Program this);
#endif//$XCC_h

Optimize
Program_optimize(Program this)
{
  return Program_OPTIMIZE(this);
}

#ifdef $XCC_h
/**
 * This Program Convention.
 */
Convention
Program_convention(Program this);
#endif//$XCC_h

Convention
Program_convention(Program this)
{
  return Program_CONVENTION(this);
}

#ifdef $XCC_h
/**
 * This Program Configure.
 */
Configure
Program_configure(Program this);
#endif//$XCC_h

Configure
Program_configure(Program this)
{
  return Program_CONFIGURE(this);
}

#ifdef $XCC_h
/**
 * This Program Data.
 */
DataList
Program_dataList(Program this);
#endif//$XCC_h

DataList
Program_dataList(Program this)
{
  return Program_DATALIST(this);
}

#ifdef $XCC_h
/**
 * This Program Processor.
 */
Processor
Program_processor(Program this);
#endif//$XCC_h

Processor
Program_processor(Program this)
{
  return Program_PROCESSOR(this);
}

#ifdef $XCC_h
/**
 * This Program Sections Table
 */
IBList
Program_sections(Program this);
#endif//$XCC_h

IBList
Program_sections(Program this)
{
  return Program_SECTIONS(this);
}

#ifdef $XCC_h
/**
 * The name of the file from which this program originates
 */
const char *
Program_filename(Program this);
#endif//$XCC_h

const char *
Program_filename(Program this)
{
  return Program_FILENAME(this);
}

#ifdef $XCC_h
/**
 * Set this program filename (the file it comes from).
 */
void
Program_setFilename(Program this, const char *name);
#endif//$XCC_h

void
Program_setFilename(Program this, const char *name)
{
  strncpy (*Program__FILENAME(this), name, MAX_FILENAME_LEN);
  (*Program__FILENAME(this))[MAX_FILENAME_LEN-1] = '\0';
}

#ifdef $XCC_h
/**
 * This Program array of dedicated Temporary(ies).
 */
struct Temporary_ *
Program_dedicated(Program this);
#endif//$XCC_h

struct Temporary_ *
Program_dedicated(Program this)
{
  return Program_DEDICATED(this);
}

#ifdef $XCC_h
/**
 * For use by Program_FOREACH_Literal.
 */
IBList
Program_literals(Program this);
#endif//$XCC_h

IBList
Program_literals(Program this)
{
  return Program_LITERALS(this);
}

#ifdef $XCC_h
/**
 * Iterate this Program Literal(s).
 */
#define Program_FOREACH_Literal(this, literal) \
  IBList_FOREACH(Program_literals(this), Literal_, literal)
#define Program_ENDEACH_Literal \
  IBList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this DEDICATED Temporary(ies).
 */
#define Program_DEDICATED_FOREACH_Temporary(this, temporary) { \
  Temporary_ *Program_DEDICATED = Program_dedicated(this); \
  Register Program_REGISTER = Register__UNDEF; \
  for (; Program_REGISTER < Register__Count; Program_REGISTER++) { \
    Temporary temporary = Program_DEDICATED + Program_REGISTER;
#define Program_DEDICATED_ENDEACH_Temporary \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Make a new Procedure for this Program.
 */
Procedure
Program_makeProcedure(Program this, Symbol symbol);
#endif//$XCC_h

Procedure
Program_makeProcedure(Program this, Symbol symbol)
{
  Memory memory = Program_MEMORY(this);
  Procedure procedure = Procedure_new(memory, symbol, this);
  ProcedureTable_insert(Program_PROCEDURETABLE(this), procedure);
  *Procedure__PROGRAM(procedure) = this;
  *Symbol__PROCEDURE(symbol) = procedure;
  return procedure;
}

#ifdef $XCC_h
/*
* Program_killProcedure --	Kill a Procedure of this Program.
*
* Only the global Symbol of the Procedure is left in this Program.
*/
void
Program_killProcedure(Program this, Procedure procedure);
#endif//$XCC_h

void
Program_killProcedure(Program this, Procedure procedure)
{
  Symbol symbol = Procedure_SYMBOL(procedure);
  *Symbol__PROCEDURE(symbol) = NULL;
  Procedure_delete(procedure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Print program in assembly language.
 */
void
Program_asmPrint(Program this, FILE *file);
#endif//$XCC_h

void
Program_asmPrint(Program this, FILE *file)
{
  DataList dataList = Program_dataList(this);

  /* Filename compiled to get this program */
  fprintf(file, "\t.file \"%s\"\n", Program_FILENAME(this));

  /* Processor core */
  // fprintf(file, "\t.assume\t");
  // Processor_printName(Program_PROCESSOR(this), file);
  // putc ('\n', file);

  /* nonop insertion */
  fprintf(file, "\t.nonopinsertion\n\n");

  if(!DataList_isEmpty(dataList))
    DataList_asmPrint(dataList, file);

  Program_FOREACH_Procedure(this, procedure) {
    Procedure_asmPrint(procedure, file);
  } Program_ENDEACH_Procedure;

  fprintf(file, "\n");
  fprintf(file, "\t.ident \"GCC+Tirex+LAO (Kalray compiler chain)\"\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC_h
/**
 * Make a new symbol Temporary.
 */
Temporary
Program_makeSymbolTemporary(Program this, Symbol symbol, ImmediateValue offset, Immediate immediate);
#endif//$XCC_h

Temporary
Program_makeSymbolTemporary(Program this, Symbol symbol, ImmediateValue offset, Immediate immediate)
{
  IBList literals = Program_LITERALS(this);
  Literal literal = Literal_Ctor(IBList_push(literals),
                                 LiteralType_Symbol, symbol, offset, immediate);
  Temporary temporary = Temporary_Ctor((Temporary)(literal + 1),
                                       TemporaryType_Symbol, TemporaryFlag_Constant);
  *Temporary__LITERAL(temporary) = literal;
  return temporary;
}

#ifdef $XCC_h
/**
 * Look for a DataSection in the program.
 */
DataSection
Program_makeSection(Program this, const char* name, uint8_t align, DataSectionFlags flags,
                    DataSectionType type, uint8_t entsize);
#endif//$XCC_h

DataSection
Program_makeSection(Program this, const char* name, uint8_t align, DataSectionFlags flags,
                    DataSectionType type, uint8_t entsize)
{
  IBList sections = Program_SECTIONS(this);
  DataSection section = DataSection_make(sections, name, align, flags, type, entsize);
  return section;
}

#ifdef $XCC_h
/**
 * Look for a DataSection in the program.
 */
DataSection
Program_searchSection(Program this, const char* name);
#endif//$XCC_h

DataSection
Program_searchSection(Program this, const char* name)
{
  IBList sections = Program_SECTIONS(this);
  IBList_FOREACH(sections, DataSection_, section) {
    if(!strcmp(name, DataSection_name(section)))
      return section;
  } IBList_ENDEACH;
  return NULL;
}

#ifdef $XCC_h
/**
 * Create a Data object in the program
 */
Data
Program_makeData(Program this, Symbol symbol, size_t size, uint8_t align, DataSection section);
#endif//$XCC_h

Data
Program_makeData(Program this, Symbol symbol, size_t size, uint8_t align, DataSection section)
{
  Data data = Data_new(Program_MEMORY(this), symbol, size, align, section);
  DataList_push2(Program_DATALIST(this), data);
  return data;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Pretty print the program
 */
bool
Program_pretty(Program this, FILE *file);
#endif//$XCC__h

bool
Program_pretty(Program this, FILE *file)
{
  if (this == NULL) return true;
  if (file == NULL) file = stderr;

  Program_FOREACH_Procedure(this, procedure) {
    Procedure_pretty(procedure, file);
    fprintf(file, "\n");
  } Program_ENDEACH_Procedure;
  return true;
}

#ifdef $XCC__h
/**
 * Initialize the Program module.
 */
#define Program_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Program module.
 */
#define Program_FINI()
#endif//$XCC__h

#if XCC__C
static void
Program_TEST(void)
{
#include "LIR/Program_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Program_TEST);
  return 0;
}
#endif

