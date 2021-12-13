#ifdef $XCC_h
/*
 * !!!!	Symbol.xcc
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

#ifdef $XCC_h
/**
 * @file
 * @ingroup AIR
 * @brief Symbols for code and data.
 *
 * Symbols are a way use abstract "names" (i.e., char strings) to refer to 
 * "values", which are usually adresses of objects that are not known yet. 
 * Examples include:
 * - function: e.g. "printf" since the actual code address of the function is 
 *   not known at compile time;
 * - a data structure: since the address is not known before data layout
 * - a stack location: when spilling a register, to keep a note that some space 
 *   in the stack frame need to be reserved for the register, without actually 
 *   having to decide where exactly.
 *
 * Since symbols can represent different objects, they have a variable union 
 * field, SymbolUnion_.
 *
 * Symbols are also usually kept in tables, SymbolTable_, so that it is 
 * possible to find a symbol given a name, and to scan all existing symbols 
 * (for instance, when performing the stack frame layout).
 *
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/Symbol.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * IndexedFactory for Symbol(s).
 */
extern IndexedFactory_ Symbol_Factory[1];
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Symbol Classes.
 *
 * Used to state which kind of object a symbol represents.
 */
typedef enum {
  SymbolClass__UNDEF,	//!< Symbol class is undefined.
  SymbolClass_VAR,	//!< Symbol is a data variable.
  SymbolClass_FUNC,	//!< Symbol is a function (procedure) address.
  SymbolClass_CONST,	//!< Symbol is a constant value.
  SymbolClass_PREG,	//!< Symbol is a pseudo register.
  SymbolClass_BLOCK,	//!< Symbol is a base symbol for a block.
  SymbolClass__COUNT
} enum_SymbolClass;
typedef uint8_t SymbolClass;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ SymbolClass_Table[1];
#define SymbolClass_Id(this)	ETable_name(SymbolClass_Table, this)
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ SymbolClassEntries[] = {
  ETableENTRY(SymbolClass, _UNDEF),
  ETableENTRY(SymbolClass, VAR),
  ETableENTRY(SymbolClass, FUNC),
  ETableENTRY(SymbolClass, CONST),
  ETableENTRY(SymbolClass, PREG),
  ETableENTRY(SymbolClass, BLOCK),
  ETableENTRY_NULL
};
ETable_ SymbolClass_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Enumerates the Symbol Store Classes.
 *
 * Storage indicates where the object refered by the symbol is stored and how 
 * it can be accessed.
 */
typedef enum {
  SymbolStore__UNDEF,		//!< Symbol sclass is undefined.
  SymbolStore_AUTO,		//!< Symbol is automatic (stack variable).
                                //!  Meaning derived from the C "auto" keyword 
                                //!  which states that a variable is "local" to 
                                //!  the function (this is in fact the default).
  SymbolStore_FORMAL,		//!< Symbol is a formal parameter.
                                //!  Function argument passed by value.
  SymbolStore_FORMAL_REF,	//!< Symbol is a formal ref parameter.
                                //!  Function argument passed by reference.
  SymbolStore_PSTATIC,		//!< Symbol is PU static.
                                //!  Corresponds to the usage of the `static' 
                                //!  keyword inside a function in C.
  SymbolStore_FSTATIC,		//!< Symbol is file static.
                                //!  Corresponds to the usage of the `static' 
                                //!  keyword for a global variable in C.
  SymbolStore_COMMON,		//!< Symbol is common.
                                //!  Multiple symbol with the same name may be 
                                //!  defined in different files (though at most 
                                //!  one definition can initialize it). The 
                                //!  resulting object has the size the largest of 
                                //!  all definitions and alignment the strictest 
                                //!  over all definitions.
  SymbolStore_EXTERN,		//!< Symbol is unallocated data or text.
                                //!  Symbol will be found in another file at 
                                //!  link time.
                                //!  Corresponds to the usage of the `extern' 
                                //!  keyword in C.
  SymbolStore_UGLOBAL,		//!< Symbol is uninitialized global data.
                                //!  Symbol will refer to an address inside the 
                                //!  ".bss" section (all bytes initialized to 0 
                                //!  at program startup).
  SymbolStore_DGLOBAL,		//!< Symbol is defined global data.
                                //!  Symbol will refer to an address inside the 
                                //!  ".data" section.
  SymbolStore_TEXT,		//!< Symbol is an executable code.
                                //!  Symbol will refer to an address inside the 
                                //!  ".text" section.
  SymbolStore_REG,		//!< Symbol is a register variable.
                                //!  Corresponds to the usage of the `register' 
                                //!  keyword in C. However, we do not guarantee 
                                //!  that the variable will indeed be placed in a 
                                //!  register.
  SymbolStore__COUNT
} enum_SymbolStore;
typedef uint8_t SymbolStore;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ SymbolStore_Table[1];
#define SymbolStore_Id(this)	ETable_name(SymbolStore_Table, this)
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ SymbolStoreEntries[] = {
  ETableENTRY(SymbolStore, _UNDEF),
  ETableENTRY(SymbolStore, AUTO),
  ETableENTRY(SymbolStore, FORMAL),
  ETableENTRY(SymbolStore, FORMAL_REF),
  ETableENTRY(SymbolStore, PSTATIC),
  ETableENTRY(SymbolStore, FSTATIC),
  ETableENTRY(SymbolStore, COMMON),
  ETableENTRY(SymbolStore, EXTERN),
  ETableENTRY(SymbolStore, UGLOBAL),
  ETableENTRY(SymbolStore, DGLOBAL),
  ETableENTRY(SymbolStore, TEXT),
  ETableENTRY(SymbolStore, REG),
  ETableENTRY_NULL
};
ETable_ SymbolStore_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Enumerates the Symbol Export Classes.
 *
 * Describes the possible scopes that of symbols exported from a file.
 * Provides the linker with information on the symbols.
 */
typedef enum {
  SymbolExport__UNDEF,			//!< Symbol export is undefined.
  SymbolExport_WEAK,       //!< Exported but only chosen when no other symbol
                                        //! with the same name is available.
  SymbolExport_LOCAL,			//!< Default for static (not exported).
                                        //!  May have its address taken.
  SymbolExport_LOCAL_INTERNAL,		//!< Static that do not have address
                                        //!  passed out of .out/.so.
  SymbolExport_GLOBAL_INTERNAL,		//!< Exported, but only visible and used
                                        //!  within .out/.so.
  SymbolExport_GLOBAL_HIDDEN,		//!< Exported, but only visible
                                        //!  within .out/.so. However, may be
                                        //!  used outside if address is taken.
  SymbolExport_GLOBAL_PROTECTED,	//!< Exported, not preemptible.
  SymbolExport_GLOBAL_PREEMPTIBLE,	//!< Exported and preemptible (replaced 
                                        //!  if dynamic load of the same symbol 
                                        //!  in a different file).
  SymbolExport__COUNT
} enum_SymbolExport;
typedef uint8_t SymbolExport;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ SymbolExport_Table[1];
#define SymbolExport_Id(this)	ETable_name(SymbolExport_Table, this)
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ SymbolExportEntries[] = {
  ETableENTRY(SymbolExport, _UNDEF),
  ETableENTRY(SymbolExport, WEAK),
  ETableENTRY(SymbolExport, LOCAL),
  ETableENTRY(SymbolExport, LOCAL_INTERNAL),
  ETableENTRY(SymbolExport, GLOBAL_INTERNAL),
  ETableENTRY(SymbolExport, GLOBAL_HIDDEN),
  ETableENTRY(SymbolExport, GLOBAL_PROTECTED),
  ETableENTRY(SymbolExport, GLOBAL_PREEMPTIBLE),
  ETableENTRY_NULL
};
ETable_ SymbolExport_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Enumerates the Symbol flags.
 */
typedef enum {
  SymbolFlag_Resolved  = 0x01,	//!< Symbol is resolved, i.e., its VALUE is known.
} enum_SymbolFlag;
typedef uint8_t SymbolFlags;
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Union for the variant part of Symbol.
 */
union SymbolUnion_ {
  void *POINTER;	//!< Dummy pointer to allow easy initialization to NULL if SymbolUnion_ is not used
  struct Procedure_ *PROCEDURE;	//!< The Procedure can be used in the following cases:
    //!  - SymbolClass_FUNC: the procedure it is the symbol off
    //!  - SymbolClass_VAR & (SymbolStore_AUTO || SymbolStore_PSTATIC):
    //!    the procedure to which the symbol belong
  /* can be completed with other if required, see SymbolClass for possible uses. */
};
typedef union SymbolUnion_ SymbolUnion_, *SymbolUnion;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Implement a symbol.
 */
struct Symbol_ {
  //@args	uint32_t index, String name
  Indexed_ __;		//!< A Symbol ISA Indexed.
  //@access IDENTITY	Indexed_IDENTITY(Symbol____(this))
  //@access INDEX	Indexed_INDEX(Symbol____(this))
  //@mutate INDEX	Indexed__INDEX(Symbol____(this))
  ImmediateValue VALUE;		//!< The resolved value of this Symbol. Valid only when the symbol is resolved.
  String NAME;			//!< The Symbol name.
  SymbolUnion_ UNION_;
  //@access POINTER	(Symbol_UNION_(this).POINTER)
  //@mutate POINTER	(&Symbol_UNION_(this).POINTER)
  //@access PROCEDURE	(Symbol_UNION_(this).PROCEDURE)
  //@mutate PROCEDURE	(&Symbol_UNION_(this).PROCEDURE)
  SymbolClass CLASS;		//!< The symbol class
  SymbolStore STORE;		//!< The symbol store class.
  SymbolExport EXPORT;		//!< The symbol export class
  SymbolFlags FLAGS;		//!< This Symbol flags.
  //@access isResolved  ((Symbol_flags(this) & SymbolFlag_Resolved) != 0)
  uint64_t SIZE;		//!< The symbol size in byte
  uint64_t BASEOFFSET;		//!< Positive offset from start of base symbol.
  int32_t  BASEINDEX;		//!< The identity of the base symbol if any, else, equals INDEX.
  NativeType NATIVETYPE;	//!< The symbol machine type if scalar.
};
#endif//$XCC_h

#ifdef $XCC_h
static inline Indexed
Symbol__(Symbol this) {
  return Symbol____(this);
}
static inline const_Indexed
Symbol__const(const_Symbol this) {
  return Symbol____(this);
}
#endif//$XCC_h

Symbol
Symbol_Ctor(Symbol this,
            uint32_t index, String name)
{
  Indexed_Ctor(Symbol____(this), Symbol_Factory);
  *Symbol__INDEX(this) = index;
  *Symbol__VALUE(this) = 0;
  *Symbol__NAME(this) = name;
  *Symbol__POINTER(this) = NULL;
  *Symbol__CLASS(this) = SymbolClass__UNDEF;
  *Symbol__STORE(this) = SymbolStore__UNDEF;
  *Symbol__EXPORT(this) = SymbolExport__UNDEF;
  *Symbol__FLAGS(this) = 0;
  *Symbol__SIZE(this) = 0;
  *Symbol__BASEOFFSET(this) = 0;
  *Symbol__BASEINDEX(this) = Symbol_INDEX(this);
  *Symbol__NATIVETYPE(this) = NativeType__UNDEF;
  return this;
}

size_t
Symbol_Size(uint32_t index, String name)
{
  return sizeof(Symbol_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Symbol_)\t%zu\n", sizeof(Symbol_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Symbol value.
 */
static inline ImmediateValue
Symbol_value(const_Symbol this)
{
  return Symbol_VALUE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Symbol value.
 */
static inline void
Symbol_setValue(Symbol this, ImmediateValue value)
{
  *Symbol__VALUE(this) = value;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol identity.
 */
static inline uint32_t
Symbol_identity(const_Symbol this)
{
  return Symbol_IDENTITY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol index.
 */
static inline uint32_t
Symbol_index(const_Symbol this)
{
  return Symbol_INDEX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol name.
 */
static inline const char *
Symbol_name(const_Symbol this)
{
  return Symbol_NAME(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol SymbolClass.
 */
static inline SymbolClass
Symbol_class(const_Symbol this)
{
  return Symbol_CLASS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Symbol SymbolClass.
 */
static inline void
Symbol_setClass(Symbol this, SymbolClass sclass)
{
  *Symbol__CLASS(this) = sclass;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol SymbolStore.
 */
static inline SymbolStore
Symbol_store(const_Symbol this)
{
  return Symbol_STORE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Symbol SymbolStore.
 */
static inline void
Symbol_setStore(Symbol this, SymbolStore sstore)
{
  *Symbol__STORE(this) = sstore;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol SymbolExport.
 */
static inline SymbolExport
Symbol_export(const_Symbol this)
{
  return Symbol_EXPORT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Symbol SymbolExport.
 */
static inline void
Symbol_setExport(Symbol this, SymbolExport sexport)
{
  *Symbol__EXPORT(this) = sexport;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the classes (CLASS, STORE, EXPORT) for the symbol.
 */
static inline void
Symbol_setClasses(Symbol this, SymbolClass sclass,
                  SymbolStore sstore, SymbolExport sexport)
{
  *Symbol__CLASS(this) = sclass;
  *Symbol__STORE(this) = sstore;
  *Symbol__EXPORT(this) = sexport;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol flags.
 */
static inline SymbolFlags
Symbol_flags(const_Symbol this)
{
  return Symbol_FLAGS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Symbol flags.
 */
static inline void
Symbol_setFlags(Symbol this, SymbolFlags flags)
{
  *Symbol__FLAGS(this) |= flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Reset this Symbol flags.
 */
static inline void
Symbol_clearFlags(Symbol this, SymbolFlags flags)
{
  *Symbol__FLAGS(this) &= ~flags;
}
#endif//$XCC_h

#ifdef $XCC_h
#define Symbol_setResolved(this) Symbol_setFlags(this, SymbolFlag_Resolved)
#define Symbol_clearResolved(this) Symbol_clearFlags(this, SymbolFlag_Resolved)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test if this Symbol is for spilling.
 *
 * Currently no distinction between a spill symbol and an auto variable.
 */
static inline bool
Symbol_isSpill(const_Symbol symbol)
{
  return Symbol_CLASS(symbol) == SymbolClass_VAR &&
      Symbol_STORE(symbol) == SymbolStore_AUTO &&
      Symbol_EXPORT(symbol) == SymbolExport_LOCAL;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns true if the symbol is a root symbol.
 *
 * A symbol which is not root is a leaf symbol.
 * A root symbol does not need to be allocated.
 */
static inline bool
Symbol_isRoot(const_Symbol this)
{
  SymbolClass sclass = Symbol_class(this);
  // Only VAR and CONST are leaf symbol.
  return !(sclass == SymbolClass_VAR || sclass == SymbolClass_CONST);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns true if the symbol is allocated.
 *
 * Allocated symbols are,
 * - root symbols
 * - leaf symbols with a base root symbol.
 */
static inline bool
Symbol_isAllocated(const_Symbol this)
{
  return Symbol_isRoot(this) ||
      Symbol_BASEINDEX(this) != Symbol_INDEX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns true if the symbol should be typed.
 *
 * Typed symbols are var, const or preg.
 */
static inline bool
Symbol_isTyped(const_Symbol this)
{
  return
      Symbol_class(this) == SymbolClass_VAR ||
      Symbol_class(this) == SymbolClass_CONST ||
      Symbol_class(this) == SymbolClass_PREG;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns true if the symbol has a size.
 *
 * True if the symbol is block, var, const or preg.
 */
static inline bool
Symbol_hasSize(const_Symbol this)
{
  return
      Symbol_class(this) == SymbolClass_BLOCK ||
      Symbol_class(this) == SymbolClass_VAR ||
      Symbol_class(this) == SymbolClass_CONST ||
      Symbol_class(this) == SymbolClass_PREG;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol machine type.
 *
 * Only used for symbol with scalar types.
 * If nativeType == NATIVETYPE_UNKNOWN the symbol is of unknown or non scalar type.
 */
static inline NativeType
Symbol_nativeType(const_Symbol this)
{
  return Symbol_NATIVETYPE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Symbol basic type.
 */
static inline void
Symbol_setNativeType(Symbol this, NativeType nativeType)
{
  Except_REQUIRE(Symbol_isTyped(this));
  Except_REQUIRE(nativeType != NativeType__UNDEF);
  *Symbol__NATIVETYPE(this) = nativeType;
  *Symbol__SIZE(this) = NativeType_sizeOf(nativeType);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol size.
 */
static inline uint64_t
Symbol_size(const_Symbol this)
{
  return Symbol_SIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Symbol size.
 */
static inline void
Symbol_setSize(Symbol this, uint64_t size)
{
  Except_ENSURE(Symbol_hasSize(this));
  *Symbol__SIZE(this) = size;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol base index.
 */
static inline uint32_t
Symbol_baseIndex(const_Symbol this)
{
  return Symbol_BASEINDEX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Symbol base index.
 */
static inline void
Symbol_setBaseIndex(Symbol this, uint32_t baseIndex)
{
  *Symbol__BASEINDEX(this) = baseIndex;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Symbol base offset.
 *
 * The offset is a positive value that locates the symbol into its
 * base symbol container. If the symbol does not have a container base,
 * then the offset is always 0.
 */
static inline uint64_t
Symbol_baseOffset(const_Symbol this)
{
  return Symbol_BASEOFFSET(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Symbol base offset.
 */
static inline void
Symbol_setBaseOffset(Symbol this, uint64_t offset)
{
  *Symbol__BASEOFFSET(this) = offset;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Symbol base offset.
 */
void
Symbol_pretty(Symbol this, FILE* file);
#endif//$XCC_h

void
Symbol_pretty(Symbol this, FILE* file)
{
  fprintf(file, "%s:\n", Symbol_name(this));
  fprintf(file, "\tval: %"PRId64"\n", Symbol_value(this));
  fprintf(file, "\tclass: %s\n", SymbolClass_Id(Symbol_class(this)));
  fprintf(file, "\tstore: %s\n", SymbolStore_Id(Symbol_store(this)));
  fprintf(file, "\texport: %s\n",SymbolExport_Id(Symbol_export(this)));
  fprintf(file, "\tflags: %x\n", Symbol_flags(this));
  if(Symbol_isSpill(this)) fprintf(file, "\tis Spill\n");
  if(Symbol_isRoot(this))  fprintf(file, "\tis Root\n");
  if(Symbol_isAllocated(this))  fprintf(file, "\tis Allocated\n");
  if(Symbol_isTyped(this))  fprintf(file, "\tis Typed\n");
  if(Symbol_hasSize(this))  fprintf(file, "\tsize: %"PRIu64"\n", Symbol_size(this));
  fprintf(file, "\tnative type: %u\n", Symbol_nativeType(this));
  if(Symbol_baseIndex(this) != Symbol_index(this)) { 
    if(Symbol_PROCEDURE(this) == NULL) {
      fprintf(file, "\tbase symbol index: %u\n", Symbol_baseIndex(this));
    } else {
/*      struct Procedure_ *proc = Symbol_PROCEDURE(this);*/
/*      struct Program_ *program = Procedure_program(proc);*/
/*      SymbolTable table = Program_symbolTable(program);*/
/*      Symbol base = SymbolTable_search(table, Symbol_baseIndex(this));*/
/*      fprintf(file, "\tbase symbol: %s\n", Symbol_name(base));*/
      fprintf(file, "\tbase symbol index: %u\n", Symbol_baseIndex(this));
    }
  }
  fprintf(file, "\tbase offset: %"PRIu64"\n", Symbol_baseOffset(this));
  if(Symbol_PROCEDURE(this) != NULL) 
/*    fprintf(file, "\tfrom or is proc: %s\n",*/
/*        Symbol_name(Procedure_symbol( Symbol_PROCEDURE(this))));*/
    fprintf(file, "\tfrom some proc\n");
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Table for Symbol(s).
 */
struct SymbolTable_ {
  //@args	Memory memory, int32_t estimate
  StringStore_ STRINGS_;
  //@access STRINGS	SymbolTable__STRINGS_(this)
  HTable_ HTABLE_;
  //@access HTABLE	SymbolTable__HTABLE_(this)
  //@access MEMORY	HTable_memory(SymbolTable_HTABLE(this))
  uint32_t COUNTER;	//!< The counter used for generated Symbol keys.
  const char *PREFIX;	//!< The prefix used for generated Symbol names.
};
#endif//$XCC_h

SymbolTable
SymbolTable_Ctor(SymbolTable this,
                 Memory memory, int32_t estimate)
{
  StringStore_Ctor(SymbolTable_STRINGS(this), memory);
  HTable_Ctor(SymbolTable_HTABLE(this), memory, estimate);
  *SymbolTable__COUNTER(this) = 0;
  *SymbolTable__PREFIX(this) = "";
  return this;
}

void
SymbolTable_Dtor(SymbolTable this)
{
  StringStore_Dtor(SymbolTable_STRINGS(this));
  HTable_Dtor(SymbolTable_HTABLE(this));
}

size_t
SymbolTable_Size(Memory memory, int32_t estimate)
{
  return sizeof(SymbolTable_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(SymbolTable_)\t%zu\n", sizeof(SymbolTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
SymbolTable
SymbolTable_new(Memory parent, int32_t estimate);
#endif//$XCC_h

SymbolTable
SymbolTable_new(Memory parent, int32_t estimate)
{
  Memory memory = Memory_new(parent, true);
  size_t size = SymbolTable_Size(memory, estimate);
  SymbolTable this = Memory_alloc(memory, size);
  return SymbolTable_Ctor(this, memory, estimate);
}

#ifdef $XCC_h
SymbolTable
SymbolTable_delete(SymbolTable this);
#endif//$XCC_h

SymbolTable
SymbolTable_delete(SymbolTable this)
{
  if (this != NULL) {
    Memory memory = SymbolTable_MEMORY(this);
#ifndef _NDTOR
    SymbolTable_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * This SymbolTable Memory
 */
static inline Memory
SymbolTable_memory(SymbolTable this)
{
  return SymbolTable_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This SymbolTable StringStore
 */
static inline StringStore
SymbolTable_strings(SymbolTable this)
{
  return SymbolTable_STRINGS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This SymbolTable name prefix.
 */
static inline const char *
SymbolTable_prefix(SymbolTable this)
{
  return SymbolTable_PREFIX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this SymbolTable name prefix.
 */
static inline void
SymbolTable_setPrefix(SymbolTable this, const char *prefix)
{
  StringStore strings = SymbolTable_STRINGS(this);
  *SymbolTable__PREFIX(this) = StringStore_S(strings, prefix);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by SymbolTable_FOREACH_Symbol.
 */
static inline HTable
SymbolTable_htable(SymbolTable this)
{
  return SymbolTable_HTABLE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this SymbolTable.
 */
#define SymbolTable_FOREACH_Symbol(this, symbol) { \
  HTable SymbolTable_HTABLE = SymbolTable_htable(this); \
  HTable_FOREACH(SymbolTable_HTABLE, HTable_KEY, Symbol_, symbol) {
#define SymbolTable_ENDEACH_Symbol \
  } HTable_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make an index for a new Symbol.
 *
 * The index generated for the new label has the 8 low bits cleared, to ensure
 * it will not collide with any cgir_sym passed in.
 */
static inline uint32_t
SymbolTable_makeIndex(SymbolTable this)
{
  return (*SymbolTable__COUNTER(this) -= 256);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Search this SymbolTable by index.
 */
static inline Symbol
SymbolTable_search(SymbolTable this, uint32_t index)
{
  HTableKey key = (HTableKey)(uintptr_t)index;
  HTable htable = SymbolTable_htable(this);
  return (Symbol)HTable_search(htable, key);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Search this SymbolTable by name.
 *
 * @return @c NULL if not found.
 * @warning unefficient function: scans all the SymbolTable!
 */
static inline Symbol
SymbolTable_searchName(SymbolTable this, const char* name)
{
  SymbolTable_FOREACH_Symbol(this, symbol) {
    if(!strcmp(name, Symbol_name(symbol))) return symbol;
  } SymbolTable_ENDEACH_Symbol;
  return NULL;
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Insert a new symbol into this SymbolTable.
 * 
 * @return \c NULL if a symbol with the same \p index already exists.
 */
Symbol
SymbolTable_insert(SymbolTable this, uint32_t index, const char *name);
#endif//$XCC_h

Symbol
SymbolTable_insert(SymbolTable this, uint32_t index, const char *name)
{
  HTableKey key = (HTableKey)(uintptr_t)index;
  size_t size = Symbol_Size(index, name);
  HTable htable = SymbolTable_HTABLE(this);
  Symbol symbol = HTable_insert(htable, key, size);
  if (symbol != NULL) {
    StringStore strings = SymbolTable_STRINGS(this);
    String nameString = StringStore_S(strings, name);
    Symbol_Ctor(symbol, index, nameString);
  }
  return symbol;
}

#ifdef $XCC_h
/**
 * Lookup a new symbol into this SymbolTable.
 */
Symbol
SymbolTable_lookup(SymbolTable this, uint32_t index, const char *name);
#endif//$XCC_h

Symbol
SymbolTable_lookup(SymbolTable this, uint32_t index, const char *name)
{
  HTableKey key = (HTableKey)(uintptr_t)index;
  HTable htable = SymbolTable_HTABLE(this);
  int32_t count = HTable_count(htable);
  size_t size = Symbol_Size(index, name);
  Symbol symbol = HTable_lookup_(htable, key, size);
  if (count != HTable_count(htable)) {
    StringStore strings = SymbolTable_STRINGS(this);
    String nameString = StringStore_S(strings, name);
    Symbol_Ctor(symbol, index, nameString);
  }
  return symbol;
}

#ifdef $XCC_h
/**
 * Make a new anonymous symbol.
 */
Symbol
SymbolTable_makeSymbol(SymbolTable this, const char *name);
#endif//$XCC_h

Symbol
SymbolTable_makeSymbol(SymbolTable this, const char *name)
{
  int32_t index = SymbolTable_makeIndex(this);
  return SymbolTable_insert(this, index, name);
}

#ifdef $XCC_h
/**
 * Get the Symbol base of a symbol.
 *
 * Always return a valid symbol.
 * A symbol base may be the symbol itself if the symbol is self contained,
 * in this case the symbol baseOffset is always 0.
 * Otherwise the symbol base is a symbol of class SymbolClass_BLOCK that is a
 * container for this symbol, in this case the symbol baseOffset locates
 * this symbol into the container block symbol.
 * Only one level of symbol container is allowed. Thus a symbol of
 * class SymbolClass_BLOCK has no base other than itself.
 */
static inline Symbol
SymbolTable_getBaseSymbol(SymbolTable this, Symbol symbol)
{
  Symbol base = NULL;
  if (Symbol_BASEINDEX(symbol) == Symbol_INDEX(symbol)) {
    Except_CHECK(Symbol_BASEOFFSET(symbol) == 0);
    base = symbol;
  }
  base = SymbolTable_search(this, Symbol_BASEINDEX(symbol));
  Except_ENSURE(base != NULL);
  return base;
}
#endif//$XCC_h

#ifdef $XCC__c
{
  SymbolTable symbolTable = SymbolTable_new(Memory_Root, 16);
  SymbolTable_makeSymbol(symbolTable, "a");
  SymbolTable_makeSymbol(symbolTable, "b");
  SymbolTable_delete(symbolTable);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize Symbol.
 */
void
Symbol_INIT(void);
#endif//$XCC__h

IndexedFactory_ Symbol_Factory[1];

void
Symbol_INIT(void)
{
  IndexedFactory_Ctor(Symbol_Factory);
#ifndef _NTRACE
  ETable_Ctor(SymbolClass_Table, Memory_Root, SymbolClassEntries);
  ETable_Ctor(SymbolStore_Table, Memory_Root, SymbolStoreEntries);
  ETable_Ctor(SymbolExport_Table, Memory_Root, SymbolExportEntries);
#endif//_NTRACE
}

#ifdef $XCC__h
/**
 * Finalize Symbol.
 */
#ifndef _NTRACE
void
Symbol_FINI(void);
#else//_NTRACE
#define Symbol_FINI()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
Symbol_FINI(void)
{
  ETable_Dtor(SymbolExport_Table);
  ETable_Dtor(SymbolStore_Table);
  ETable_Dtor(SymbolClass_Table);
}
#endif//_NTRACE

#if XCC__C
static void
Symbol_TEST(void)
{
#include "AIR/Symbol_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(Symbol_TEST);
  return 0;
}
#endif

