#ifdef $XCC_h
/*
 * !!!!	Label.xcc
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
 * @brief Labels for code.
 */
#endif//$XCC_h


#include "AIR/AIR_.h"

#ifdef $XCC__h
#include "AIR/Label.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * IndexedFactory for Label(s).
 */
extern IndexedFactory_ Label_Factory[1];
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Counter for the generated label names.
 */
extern uint32_t Label_NameCounter;
#endif//$XCC_h

uint32_t Label_NameCounter = 0;

#ifdef $XCC_h
/**
 * Implement a code label.
 */
struct Label_ {
  //@args	uint32_t index, String name
  Indexed_ __;		//!< A Label_ ISA Indexed_.
  //@access IDENTITY	Indexed_IDENTITY(Label____(this))
  //@access INDEX	Indexed_INDEX(Label____(this))
  //@mutate INDEX	Indexed__INDEX(Label____(this))
  ImmediateValue VALUE;         	//!< The resolved value of this Label.
  String NAME;				//!< This Label name.
  struct BasicBlock_ *BLOCK;		//!< This Label BasicBlock.
};
#endif//$XCC_h

#ifdef $XCC_h
static inline Indexed
Label__(Label this) {
  return Label____(this);
}
static inline const_Indexed
Label__const(const_Label this) {
  return Label____(this);
}
#endif//$XCC_h

Label
Label_Ctor(Label this,
           uint32_t index, String name)
{
  Indexed_Ctor(Label____(this), Label_Factory);
  *Label__INDEX(this) = index;
  *Label__VALUE(this) = 0;
  *Label__NAME(this) = name;
  *Label__BLOCK(this) = NULL;
  return this;
}

size_t
Label_Size(uint32_t index, String name)
{
  return sizeof(Label_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Label_)\t%zu\n", sizeof(Label_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Label value.
 */
static inline ImmediateValue
Label_value(const_Label this)
{
  return Label_VALUE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Label value.
 */
static inline void
Label_setValue(Label this, ImmediateValue value)
{
  *Label__VALUE(this) = value;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This label identity.
 */
static inline uint32_t
Label_identity(const_Label this)
{
  return Label_IDENTITY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Label name.
 */
static inline const char *
Label_name(const_Label this)
{
  return Label_NAME(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * BasicBlock to which this Label is attached.
 */
static inline struct BasicBlock_*
Label_block(const_Label this)
{
  return Label_BLOCK(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Table for Label(s).
 */
struct LabelTable_ {
  //@args	Memory memory, int32_t estimate
  StringStore_ STRINGS_;
  //@access STRINGS	LabelTable__STRINGS_(this)
  HTable_ HTABLE_;
  //@access HTABLE	LabelTable__HTABLE_(this)
  //@access MEMORY	HTable_memory(LabelTable_HTABLE(this))
  uint32_t COUNTER;	//!< The counter used for generated Label keys.
};
#endif//$XCC_h

LabelTable
LabelTable_Ctor(LabelTable this,
                Memory memory, int32_t estimate)
{
  StringStore_Ctor(LabelTable_STRINGS(this), memory);
  HTable_Ctor(LabelTable_HTABLE(this), memory, estimate);
  *LabelTable__COUNTER(this) = 0;
  return this;
}

void
LabelTable_Dtor(LabelTable this)
{
  StringStore_Dtor(LabelTable_STRINGS(this));
  HTable_Dtor(LabelTable_HTABLE(this));
}

size_t
LabelTable_Size(Memory memory, int32_t estimate)
{
  return sizeof(LabelTable_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(LabelTable_)\t%zu\n", sizeof(LabelTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
LabelTable
LabelTable_new(Memory parent, int32_t estimate);
#endif//$XCC_h

LabelTable
LabelTable_new(Memory parent, int32_t estimate)
{
  Memory memory = Memory_new(parent, true);
  size_t size = LabelTable_Size(memory, estimate);
  LabelTable this = Memory_alloc(memory, size);
  return LabelTable_Ctor(this, memory, estimate);
}

#ifdef $XCC_h
LabelTable
LabelTable_delete(LabelTable this);
#endif//$XCC_h

LabelTable
LabelTable_delete(LabelTable this)
{
  if (this != NULL) {
    Memory memory = LabelTable_MEMORY(this);
#ifndef _NDTOR
    LabelTable_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * For use by LabelTable_FOREACH_Label.
 */
static inline HTable
LabelTable_htable(LabelTable this)
{
  return LabelTable_HTABLE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this LabelTable.
 */
#define LabelTable_FOREACH_Label(this, label) { \
  HTable LabelTable_HTABLE = LabelTable_htable(this); \
  HTable_FOREACH(LabelTable_HTABLE, LabelTable_KEY, Label_, label) {
#define LabelTable_ENDEACH_Label \
  } HTable_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make an index for a new Label.
 *
 * The index generated for the new label has the 8 low bits cleared, to ensure
 * it will not collide with any cgir_lab passed in.
 */
static inline uint32_t
LabelTable_makeIndex(LabelTable this)
{
  return (*LabelTable__COUNTER(this) -= 256);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Search this LabelTable by index.
 */
Label
LabelTable_search(LabelTable this, uint32_t index);
#endif//$XCC_h

Label
LabelTable_search(LabelTable this, uint32_t index)
{
  long lkey = index;
  HTableKey key = (HTableKey)lkey;
  HTable htable = LabelTable_HTABLE(this);
  return HTable_search(htable, key);
}

#ifdef $XCC_h
/**
 * Lookup a new label into this LabelTable.
 *
 * @param index 
 * @param name the string name of the new label
 */
Label
LabelTable_lookup(LabelTable this, uint32_t index, const char *name);
#endif//$XCC_h

Label
LabelTable_lookup(LabelTable this, uint32_t index, const char *name)
{
  long lkey = index;
  HTableKey key = (HTableKey)lkey;
  HTable htable = LabelTable_HTABLE(this);
  int32_t count = HTable_count(htable);
  size_t size = Label_Size(index, name);
  Label label = HTable_lookup_(htable, key, size);
  if (count != HTable_count(htable)) {
    StringStore strings = LabelTable_STRINGS(this);
    String string = StringStore_S(strings, name);
    Label_Ctor(label, index, string);
  }
  return label;
}

#ifdef $XCC_h
/**
 * Insert a new label into this LabelTable.
 *
 * @return \c NULL if a label with this @c index already exists.
 */
Label
LabelTable_insert(LabelTable this, uint32_t index, const char *name);
#endif//$XCC_h

Label
LabelTable_insert(LabelTable this, uint32_t index, const char *name)
{
  long lkey = index;
  HTableKey key = (HTableKey)lkey;
  size_t size = Label_Size(index, name);
  HTable htable = LabelTable_HTABLE(this);
  Label label = HTable_insert(htable, key, size);
  if (label != NULL) {
    StringStore strings = LabelTable_STRINGS(this);
    String string = StringStore_S(strings, name);
    Label_Ctor(label, index, string);
  }
  return label;
}

#ifdef $XCC_h
/**
 * Make a new anonymous label.
 */
Label
LabelTable_makeLabel(LabelTable this);
#endif//$XCC_h

Label
LabelTable_makeLabel(LabelTable this)
{
  char buffer[32];
  uint32_t number =  ++Label_NameCounter;
  int32_t index = LabelTable_makeIndex(this);
  int length = 0, i = 0, num_digits = 0;
  // Compute the number of digits needed to represent the label number.
  do ++num_digits; while (number /= 8);
  if (++num_digits < 3) num_digits = 3;
  // Create the label name, as .LAO and the octal representation of the label number.
  buffer[0] = '.', buffer[1] = 'L', buffer[2] = 'A', buffer[3] = 'O';
  // Produce the string version of the label number.
  number = Label_NameCounter;
  buffer[(length = 3 + num_digits + 1)] = '\0';
  do buffer[3 + num_digits - i++] = '0' + (number%8); while (number /= 8);
  while (num_digits > i) buffer[3 + num_digits - i++] = '0';
  // Insert the label into the label table
  Except_ENSURE(length < sizeof(buffer));
  return LabelTable_insert(this, index, buffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize Label.
 */
void
Label_INIT(void);
#endif//$XCC__h

IndexedFactory_ Label_Factory[1];

void
Label_INIT(void)
{
  IndexedFactory_Ctor(Label_Factory);
  Label_NameCounter = 0;
}

#ifdef $XCC__h
/**
 * Finalize Label.
 */
#define Label_FINI()
#endif//$XCC__h

#if XCC__C
static void
Label_TEST(void)
{
#include "AIR/Label_.c"
}

int
main(int argc, char **argv)
{
  AIR_Test(Label_TEST);
  return 0;
}
#endif

