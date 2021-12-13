#ifdef $XCC_h
/*
 * !!!!	Literal.xcc
 *
 * Author: Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2001 - 2007 STMicroelectronics
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
 * @brief Literals in the code stream.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/Literal.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Literal types.
 */
typedef enum {
  LiteralType_Absolute,        // Absolute Literal: HANDLE is NULL
  LiteralType_Symbol,          // Symbol Literal: SYMBOL is the Symbol
  LiteralType_Label,           // Label Literal: LABEL is the Label
  LiteralType__
} enum_LiteralType;
typedef uint8_t LiteralType;
#define LiteralType_isAbsolute(type)   ((type) == LiteralType_Absolute)
#define LiteralType_isSymbol(type)     ((type) == LiteralType_Symbol)
#define LiteralType_isLabel(type)      ((type) == LiteralType_Label)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Union for the variant part of Literal.
 */
union LiteralUnion_ {
  void *HANDLE;			//!< Client handle.
  Symbol SYMBOL;		//!< Symbol of this Literal.
  Label LABEL;			//!< Label of this Literal.
};
typedef union LiteralUnion_ LiteralUnion_, *LiteralUnion;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Literal for native code.
 */
struct Literal_ {
  //@args	LiteralType type, void *handle, ImmediateValue value, Immediate immediate
  ImmediateValue VALUE;		//!< The resolved value of this Literal.
  //@access OFFSET	Literal_VALUE(this)
  //@mutate OFFSET	Literal__VALUE(this)
  LiteralUnion_ UNION_;
  //@access HANDLE	(Literal_UNION_(this).HANDLE)
  //@mutate HANDLE	(&Literal_UNION_(this).HANDLE)
  //@access SYMBOL	(Literal_UNION_(this).SYMBOL)
  //@mutate SYMBOL	(&Literal_UNION_(this).SYMBOL)
  //@access LABEL	(Literal_UNION_(this).LABEL)
  //@mutate LABEL	(&Literal_UNION_(this).LABEL)
  LiteralType TYPE;		//!< This Literal type.
  //@access isAbsolute	LiteralType_isAbsolute(Literal_type(this))
  //@access isSymbol	LiteralType_isSymbol(Literal_type(this))
  //@access isLabel	LiteralType_isLabel(Literal_type(this))
  Immediate IMMEDIATE;
  Relocation RELOCATION;
  NativeType NATIVETYPE;
};
#endif//$XCC_h

Literal
Literal_Ctor(Literal this,
             LiteralType type, void *handle, ImmediateValue value, Immediate immediate)
{
  *Literal__TYPE(this) = type;
  *Literal__VALUE(this) = value;
  *Literal__HANDLE(this) = handle;
  *Literal__IMMEDIATE(this) = immediate;
  *Literal__RELOCATION(this) = Relocation__UNDEF;
  *Literal__NATIVETYPE(this) = NativeType__UNDEF;
  return this;
}

void
Literal_Dtor(Literal this)
{
  *Literal__OFFSET(this) = 0;
  *Literal__TYPE(this) = LiteralType__;
}

size_t
Literal_Size(LiteralType type, void *handle, ImmediateValue value, Immediate immediate)
{
  return sizeof(Literal_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Literal_)\t%zu\n", sizeof(Literal_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Literal type.
 */
static inline LiteralType
Literal_type(const_Literal this)
{
  return Literal_TYPE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Literal value.
 */
static inline ImmediateValue
Literal_value(const_Literal this)
{
  return Literal_VALUE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Literal value.
 */
static inline void
Literal_setValue(Literal this, ImmediateValue value)
{
  *Literal__VALUE(this) = value;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Literal offset in case of Literal symbol.
 */
static inline ImmediateValue
Literal_offset(const_Literal this)
{
  Except_REQUIRE(Literal_isSymbol(this) || Literal_isLabel(this));
  return Literal_OFFSET(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Literal offset in case of Literal symbol.
 */
static inline void
Literal_setOffset(Literal this, ImmediateValue offset)
{
  Except_REQUIRE(Literal_isSymbol(this) || Literal_isLabel(this));
  *Literal__OFFSET(this) = offset;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Literal handle.
 */
static inline void *
Literal_handle(const_Literal this)
{
  return Literal_HANDLE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Literal symbol.
 */
static inline Symbol
Literal_symbol(const_Literal this)
{
  Except_REQUIRE(Literal_isSymbol(this));
  return Literal_SYMBOL(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Literal label.
 */
static inline Label
Literal_label(const_Literal this)
{
  Except_REQUIRE(Literal_isLabel(this));
  return Literal_LABEL(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Literal Immediate.
 */
static inline Immediate
Literal_immediate(const_Literal this)
{
  return Literal_IMMEDIATE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Literal Immediate.
 */
static inline void
Literal_setImmediate(Literal this, Immediate immediate)
{
  *Literal__IMMEDIATE(this) = immediate;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Literal Relocation.
 */
static inline Relocation
Literal_relocation(const_Literal this)
{
  return Literal_RELOCATION(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Literal Relocation.
 */
static inline void
Literal_setRelocation(Literal this, Relocation relocation)
{
  *Literal__RELOCATION(this) = relocation;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Implement a table of Literal(s) indexed by HANDLE.
 */
struct LiteralTable_ {
  //@args	Memory memory, int32_t estimate
  HTable_ HTABLE[1];		// The base HTable for this LiteralTable.
  //@access MEMORY	HTable_memory(LiteralTable_HTABLE(this))
};
#endif//$XCC_h

LiteralTable
LiteralTable_Ctor(LiteralTable this,
                   Memory memory, int32_t estimate)
{
  HTable_Ctor(LiteralTable_HTABLE(this), memory, estimate);
  return this;
}

void
LiteralTable_Dtor(LiteralTable this)
{
  HTable_Dtor(LiteralTable_HTABLE(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LiteralTable_)\t%zu\n", sizeof(LiteralTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * This LiteralTable memory.
 */
static inline Memory
LiteralTable_memory(const_LiteralTable this)
{
  return LiteralTable_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For buse by LiteralTable_FOREACH_Literal.
 */
static inline const_HTable
LiteralTable_htable(const_LiteralTable this)
{
  return LiteralTable_HTABLE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this LiteralTable Literal(s).
 */
#define LiteralTable_FOREACH_Literal(this, literal) { \
  const_HTable LiteralTable_HTABLE = LiteralTable_htable(this); \
  HTable_FOREACH(LiteralTable_HTABLE, LiteralTable__, Literal_, literal) {
#define LiteralTable_ENDEACH_Literal \
  } HTable_ENDEACH; \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Search a Literal in this LiteralTable.
 */
static inline Literal
LiteralTable_search(const_LiteralTable this, HTableKey key)
{
  const_HTable htable = LiteralTable_HTABLE(this);
  Literal literal = (Literal)HTable_search(htable, key);
  return literal;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Lookup a Literal into this LiteralTable.
 */
static inline Literal
LiteralTable_lookup(LiteralTable this, HTableKey key, LiteralType type,
                    void *handle, ImmediateValue value, Immediate immediate)
{
  HTable htable = LiteralTable_HTABLE(this);
  int32_t count = HTable_count(htable);
  size_t size = Literal_Size(type, handle, value, immediate);
  Literal literal = (Literal)HTable_lookup_(htable, key, size);
  if (count != HTable_count(htable)) {
    Literal_Ctor(literal, type, handle, value, immediate);
  }
  return literal;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Literal module.
 */
#define Literal_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Literal module.
 */
#define Literal_FINI()
#endif//$XCC__h

#if XCC__C
static void
Literal_TEST(void)
{
#include "AIR/Literal_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(Literal_TEST);
  return 0;
}
#endif

