#ifdef $XCC_h
/*
 * !!!!	Register.xcc
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

#include "MDT/MDT_.h"

#ifdef $XCC__h
#include "MDT/Register.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Register(ID, NAMES, REGFILES, WIRED, STORAGECELLS, MINREADSTAGES) \
  Register_##ID,
  Register_,
#include /*MDT*/"Register.tuple"
  Register__,
#undef Register
} enum_Register;
#define Register__BIAS	(Register_ + 1)
#endif//$XCC__h

const int Register__Count = Register__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Register enumeration.
 */
extern const char *
Register_Id(Register this);
extern const char *
Register_name(Register this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Register_Id(Register this)
{
  static const char *_[] = {
#define Register(ID, NAMES, REGFILES, WIRED, STORAGECELLS, MINREADSTAGES) \
    #ID,
    "_UNDEF",
#include "Register.tuple"
  };
  Except_REQUIRE((unsigned)this < Register__);
  return _[this];
}
const char *
Register_name(Register this)
{
  return this? strchr(Register_Id(this) + sizeof(MDT_TARGET) - 1, '_') + 1: "_UNDEF";
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int registre = 0;
  for (; registre < Register__; registre++) {
    printf("Register_%s\t%d\n", Register_Id(registre), registre);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The names of this Register.
 */
const char **
Register_names(Register this);
#endif//$XCC_h

extern const char *
Register_Names[][Register_NAMES_MAX+1];
const char **
Register_names(Register this)
{
  Except_REQUIRE((unsigned)this < Register__);
  return Register_Names[this];
}

/* Registers can have multiple names.
 * The list of names is NULL-terminated.
 */
const char *
Register_Names[][Register_NAMES_MAX+1] = {
#define NAME(ID) #ID,
#define NAMES(count, items) { items NULL }
#define Register(ID, NAMES, REGFILES, WIRED, STORAGECELLS, MINREADSTAGES) \
  NAMES,
  NAMES(1, NAME(_UNDEF)),
#include "Register.tuple"
#undef NAME
#undef NAMES
};

#ifdef $XCC__c
{
  int registre = 0, i;
  for (; registre < Register__; registre++) {
    printf("Names(%s)", Register_Id(registre));
    for (i = 0; i < Register_NAMES_MAX; i++) {
      const char *name = Register_names(registre)[i];
      if (name != NULL) printf("\t%s", name);
    }
    printf("\n");
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Find Register by its name.
 */
Register
Register_fromName(const char* string, Processor processor);
#endif//$XCC_h

/* This know works even if count > 1 */ 
Register
Register_fromName(const char* string, Processor processor)
{
  // When count > 1, multiple NAME macros follow each other. They are 'OR'ed.
#define NAME(ID) !strcmp(#ID, string) ||
#define NAMES(count, items) items
#define PROCESSOR(cpu) Processor_##cpu == processor
#define REGFILE(cpu, value) (cpu && RegFile_##value != RegFile__UNDEF) ||
#define REGFILES(items) items
#define Register(ID, NAMES, REGFILES, WIRED, STORAGECELLS, MINREADSTAGES) \
  else if((NAMES false) && (REGFILES false)) { \
    return Register_ ## ID; \
  }
  if(!strcmp("_UNDEF", string)) {
    return Register__UNDEF;
  }
#include "Register.tuple"
#undef REGFILE
#undef REGFILES
  return Register__UNDEF;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The RegFile of this Register.
 */
RegFile
Register_regFile(Register this, Processor processor);
#endif//$XCC_h

extern const RegFile
Register_RegFiles[][Processor__];
RegFile
Register_regFile(Register this, Processor processor)
{
  Except_REQUIRE((unsigned)this < Register__);
  Except_REQUIRE((unsigned)processor < Processor__);
  return (RegFile)Register_RegFiles[this][processor];
}

const RegFile
Register_RegFiles[][Processor__] = {
#define REGFILE(processor, ID) RegFile_##ID,
#define REGFILES(items) { 0/*Processor_*/, items }
#define Register(ID, NAMES, REGFILES, WIRED, STORAGECELLS, MINREADSTAGES) \
  REGFILES,
  REGFILES(/**/),
#include "Register.tuple"
};

#ifdef $XCC__c
{
  int processor, registre = 0;
  for (; registre < Register__; registre++) {
    printf("Register_regFiles(%s)", Register_Id(registre));
    for (processor = 0; processor < Processor__; processor++) {
      RegFile regFile = Register_regFile(registre, processor);
      printf("\t%s", RegFile_Id(regFile));
    }
    printf("\n");
  }
  fprintf(stderr, "sizeof(Register_RegFiles)\t%zu\n", sizeof(Register_RegFiles));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * True if this Register is wired.
 */
bool
Register_isWired(Register this);
#endif//$XCC_h

extern bool
Register_Wired[];
bool
Register_isWired(Register this)
{
  Except_REQUIRE((unsigned)this < Register__);
  return Register_Wired[this];
}

bool
Register_Wired[] = {
#define WIRED(b) b
#define Register(ID, NAMES, REGFILES, WIRED, STORAGECELLS, MINREADSTAGES) \
  WIRED,
  WIRED(true),	// Ensure Register_isWired(Register_) is true.
#include "Register.tuple"
};

#ifdef $XCC__c
{
  int registre = 0;
  for (; registre < Register__; registre++) {
    if (Register_isWired(registre)) {
      printf("Register_isWired(%s)\n", Register_Id(registre));
    }
  }
  fprintf(stderr, "sizeof(Register_Wired)\t%zu\n", sizeof(Register_Wired));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

const int Register__MaxStorageCells = RegisterStorageCells_MAXCOUNT;

#ifdef $XCC_h
/**
 * Sequence of StorageCell(s).
 */
struct RegisterStorageCells_;
#endif//$XCC_h

#ifdef $XCC__h
struct RegisterStorageCells_ {
  int16_t COUNT;
  StorageCell ITEMS[RegisterStorageCells_MAXCOUNT];
};
#endif//$XCC__h

#ifdef $XCC_h
/**
 * Count of StorageCell(s) in this RegisterStorageCells.
 */
int
RegisterStorageCells_count(const_RegisterStorageCells this);
#endif//$XCC_h

int
RegisterStorageCells_count(const_RegisterStorageCells this)
{
  return RegisterStorageCells_COUNT(this);
}

#ifdef $XCC_h
/**
 * StorageCell(s) in this RegisterStorageCells.
 */
const StorageCell *
RegisterStorageCells_items(const_RegisterStorageCells this);
#endif//$XCC_h

const StorageCell *
RegisterStorageCells_items(const_RegisterStorageCells this)
{
  return RegisterStorageCells_ITEMS(this);
}

#ifdef $XCC_h
/**
 * Access a StorageCell in this RegisterStorageCells.
 */
StorageCell
RegisterStorageCells_access(const_RegisterStorageCells this, int index);
#endif//$XCC_h

StorageCell
RegisterStorageCells_access(const_RegisterStorageCells this, int index)
{
  Except_CHECK((unsigned)index < RegisterStorageCells_COUNT(this));
  return (StorageCell)RegisterStorageCells_ITEMS(this)[index];
}

#ifdef $XCC_h
/**
 * Iterate this RegisterStorageCells.
 */
#define RegisterStorageCells_FOREACH_StorageCell(this, cell) { \
  const StorageCell *RegisterStorageCells_ITEMS = RegisterStorageCells_items(this); \
  int RegisterStorageCells_COUNT = RegisterStorageCells_count(this); \
  int RegisterStorageCells_INDEX = 0; \
  for (; RegisterStorageCells_INDEX < RegisterStorageCells_COUNT; RegisterStorageCells_INDEX++) { \
    int cell = RegisterStorageCells_ITEMS[RegisterStorageCells_INDEX];
#define RegisterStorageCells_ENDEACH_StorageCell \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Register RegisterStorageCells.
 */
const_RegisterStorageCells
Register_storageCells(Register this);
#endif//$XCC_h

extern RegisterStorageCells_
Register_StorageCells[];
const_RegisterStorageCells
Register_storageCells(Register this)
{
  Except_REQUIRE((unsigned)this < Register__);
  return Register_StorageCells + this;
}

RegisterStorageCells_
Register_StorageCells[] = {
#define STORAGECELL(ID) StorageCell_##ID,
#define STORAGECELLS(count, items) count, { items }
#define Register(ID, NAMES, REGFILES, WIRED, STORAGECELLS, MINREADSTAGES) \
  { STORAGECELLS },
  { STORAGECELLS(0, /**/) },
#include "Register.tuple"
};

#ifdef $XCC__c
{
  int registre = 0;
  for (; registre < Register__; registre++) {
    const_RegisterStorageCells storageCells = Register_storageCells(registre);
    printf("Register_storageCells(%s)", Register_Id(registre));
    RegisterStorageCells_FOREACH_StorageCell(storageCells, cell) {
      printf("\t%s", StorageCell_Id(cell));
    } RegisterStorageCells_ENDEACH_StorageCell;
    printf("\n");
  }
  fprintf(stderr, "sizeof(Register_StorageCells)\t%zu\n", sizeof(Register_StorageCells));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Register minimum read stage for each Processor.
 */
int
Register_minReadStage(Register this, Processor processor);
#endif//$XCC_h

extern const uint8_t
Register_MinReadStages[][Processor__];
int
Register_minReadStage(Register this, Processor processor)
{
  Except_REQUIRE((unsigned)this < Register__);
  Except_REQUIRE((unsigned)processor < Processor__);
  return Register_MinReadStages[this][processor];
}

const uint8_t
Register_MinReadStages[][Processor__] = {
#define MINREADSTAGE(processor, stage) (uint8_t)(stage),
#define MINREADSTAGES(items) { 0/*Processor_*/, items }
#define Register(ID, NAMES, REGFILES, WIRED, STORAGECELLS, MINREADSTAGES) \
  MINREADSTAGES,
  MINREADSTAGES(/**/),
#include "Register.tuple"
#undef MINREADSTAGES
};

#ifdef $XCC__c
{
  int registre;
  int processor;
  printf("\t\t\t");
  for (processor = 0; processor < Processor__; processor++) {
    printf("\t%s", Processor_Id(processor));
  }
  printf("\n");
  for (registre = 0; registre < Register__; registre++) {
    printf("MinReadStages(%s)", Register_Id(registre));
    for (processor = 0; processor < Processor__; processor++) {
      int stage = Register_minReadStage(registre, processor);
      printf("\t\t%d", stage);
    }
    printf("\n");
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Non-mutable list of Register(s).
 */
struct RegisterList_ {
  uint8_t COUNT;
  Register ITEMS[RegisterList_MAXCOUNT];
};
#endif//$XCC__h

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(RegisterList_)\t%zu\n", sizeof(RegisterList_));
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Count the Register(s) in this RegisterList.
 */
static inline int
RegisterList_count(const_RegisterList this)
{
  return RegisterList_COUNT(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * First item of the RegisterList.
 * Return:       The first Register.
 */
static inline Register
RegisterList_first(const_RegisterList this)
{
  Except_REQUIRE(RegisterList_COUNT(this) > 0);
  return (Register)RegisterList_ITEMS(this)[0];
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Access the RegisterList by index.
 * index:        The index.
 * Return:       Corresponding Register.
 */
static inline Register
RegisterList_access(const_RegisterList this, int32_t index)
{
  Except_REQUIRE(0 <= index && index < RegisterList_COUNT(this));
  return (Register)RegisterList_ITEMS(this)[index];
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * For use by RegisterList_FOREACH_Register.
 */
static inline const Register *
RegisterList_items(const_RegisterList this)
{
  return RegisterList_ITEMS(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this RegisterList Register(s).
 */
#define RegisterList_FOREACH_Register(this, registre) { \
  const Register *RegisterList_ITEMS = RegisterList_items(this); \
  int RegisterList_COUNT = RegisterList_count(this), RegisterList_INDEX = 0; \
  for (; RegisterList_INDEX < RegisterList_COUNT; RegisterList_INDEX++) { \
    int registre = RegisterList_ITEMS[RegisterList_INDEX];
#define RegisterList_ENDEACH_Register \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * True if this RegisterList contains the given Register.
 */
bool
RegisterList_contains(const_RegisterList this, Register member);
#endif//$XCC__h

bool
RegisterList_contains(const_RegisterList this, Register member)
{
  RegisterList_FOREACH_Register(this, registre) {
    if (registre == member) return true;
  } RegisterList_ENDEACH_Register;
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Test if this Register Lifetime should be minimized.
 */
bool
Register_isLifetime(Register this, Convention convention);
#endif//$XCC_h

bool
Register_isLifetime(Register this, Convention convention)
{
  if (   RegisterSet_contains(Convention_roleRegisterSet(convention,
                                                         ConventionRole_Program), this)
      || RegisterSet_contains(Convention_roleRegisterSet(convention,
                                                         ConventionRole_Stack), this)
      || RegisterSet_contains(Convention_roleRegisterSet(convention,
                                                         ConventionRole_Wired), this)) {
    return false;
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef uint64_t RegisterSetWord;
#define RegisterSet_WORDBITS (sizeof(RegisterSetWord)*8)
#define RegisterSet_MAJOR(member) ((Register)(member)/(RegisterSet_WORDBITS))
#define RegisterSet_MINOR(member) ((Register)(member)&(RegisterSet_WORDBITS - 1))
#define RegisterSet_WORDCOUNT (RegisterSet_MAJOR(Register__COUNT) + 1)
#endif//$XCC_h

#ifdef $XCC__c
{
  printf("RegisterSet_WORDCOUNT\t%d\n", (int)(RegisterSet_WORDCOUNT));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Set of architectural registers.
 *
 * See the CCL/BitSet.xcc code to add RegisterSet methods.
 */
struct RegisterSet_ {
  //@args
  RegisterSetWord WORDS[RegisterSet_WORDCOUNT];
};
#endif//$XCC_h

RegisterSet
RegisterSet_Ctor(RegisterSet this)
{
  int i;
  for (i = 0; i < RegisterSet_WORDCOUNT; i++) {
    RegisterSet_WORDS(this)[i] = 0;
  }
  return this;
}

RegisterSet
RegisterSet_Copy(RegisterSet this, const_RegisterSet that)
{
  *(this) = *(that);
  return this;
}

void
RegisterSet_Dtor(RegisterSet this)
{
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(RegisterSet_)\t%zu\n", sizeof(RegisterSet_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Return the RegisterSet word array.
 */
static inline const RegisterSetWord *
RegisterSet_words(const_RegisterSet this)
{
  return RegisterSet_WORDS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this RegisterSet is empty.
 */
bool
RegisterSet_isEmpty(const_RegisterSet this);
#endif//$XCC_h

bool
RegisterSet_isEmpty(const_RegisterSet this)
{
  const RegisterSetWord *words = RegisterSet_WORDS(this);
  int wordCount = RegisterSet_WORDCOUNT, i;
  for (i = 0; i < wordCount; i++) {
    if (words[i] != 0) return false;
  }
  return true;
}

#ifdef $XCC_h
/**
 * Count members contained in the RegisterSet.
 * @return The count of members in this RegisterSet.
 */
int
RegisterSet_count(const_RegisterSet this);
#endif//$XCC_h

int
RegisterSet_count(const_RegisterSet this)
{
  const RegisterSetWord *words = RegisterSet_WORDS(this);
  int wordCount = RegisterSet_WORDCOUNT, count = 0, i;
  if (RegisterSet_WORDBITS <= 32) {
    for (i = 0; i < wordCount; i++) count += HackerU32_popc(words[i]);
  } else {
    for (i = 0; i < wordCount; i++) count += HackerU64_popc(words[i]);
  }
  return count;
}

#ifdef $XCC_h
/**
 * First Register in this RegisterSet.
 */
Register
RegisterSet_first(const_RegisterSet this);
#endif//$XCC_h

Register
RegisterSet_first(const_RegisterSet this)
{
  int result = 0;
  const RegisterSetWord *words = RegisterSet_WORDS(this);
  int wordCount = RegisterSet_WORDCOUNT, i;
  for (i = 0; i < wordCount; i++) {
    RegisterSetWord word = *words++;
    RegisterSetWord least = word & -word;
    if (least != 0) {
      if (RegisterSet_WORDBITS <= 32) {
#ifndef __st200__
        result += HackerU32_popc(least - 1);
#else// __st200__
        result += 31 - __lzcntw(least);
#endif
      } else {
        result += HackerU64_popc(least - 1);
      }
      return result;
    }
    result += RegisterSet_WORDBITS;
  }
  return Register_;
}

#ifdef $XCC_h
/**
 * Empty this RegisterSet.
 */
void
RegisterSet_empty(RegisterSet this);
#endif//$XCC_h

void
RegisterSet_empty(RegisterSet this)
{
  RegisterSetWord *words = RegisterSet_WORDS(this);
  int wordCount = RegisterSet_WORDCOUNT, i;
  for (i = 0; i < wordCount; i++) words[i] = 0;
}

#ifdef $XCC_h
/**
 * Choose and remove a member of the RegisterSet.
 *
 * @return Valid Register or Register_ if this RegisterSet is empty.
 */
Register
RegisterSet_choose(RegisterSet this);
#endif//$XCC_h

Register
RegisterSet_choose(RegisterSet this)
{
  int wordCount = RegisterSet_WORDCOUNT, index = 0, i, j;
  RegisterSetWord *words = RegisterSet_WORDS(this), word = 0;
  for (i = 0; i < wordCount; i++) {
    if (word == 0) {
      word = words[i];
      index = i;
    }
  }
  if (word != 0) {
    RegisterSetWord least = word & -word;
#if __GNUC__ > 2
    if (sizeof(word) <= sizeof(int)) {
      j = __builtin_ctz(word);
    } else if (sizeof(word) <= sizeof(long long)) {
      j = __builtin_ctzll(word);
    } else Except_NEVER(true);
    Except_CHECK(least == (RegisterSetWord)1<<j);
#else//__GNUC__
    if (RegisterSet_WORDBITS <= 32) {
      j = HackerU32_popc(least - 1);
    } else {
      j = HackerU64_popc(least - 1);
    }
#endif//__GNUC__
    words[index] = word ^ least;
    return index*RegisterSet_WORDBITS + j;
  }
  return Register_;
}

#ifdef $XCC_h
/**
 * Get the next Register of the RegisterSet.
 */
Register
RegisterSet_nextMember(const_RegisterSet this, Register member);
#endif//$XCC_h

Register
RegisterSet_nextMember(const_RegisterSet this, Register member)
{
  const RegisterSetWord *words = RegisterSet_WORDS(this);
  int32_t wordCount = RegisterSet_WORDCOUNT;
  int32_t i = RegisterSet_MAJOR(member + 1);
  int32_t j = RegisterSet_MINOR(member + 1);
  if (i < wordCount) {
    RegisterSetWord word = words[i] >> j;
#if __GNUC__ > 2
    word <<= j;
    if (word != 0) {
      if (sizeof(word) <= sizeof(int)) {
        j = __builtin_ctz(word);
      } else if (sizeof(word) <= sizeof(long long)) {
        j = __builtin_ctzll(word);
      } else Except_NEVER(true);
      return i*RegisterSet_WORDBITS + j;
    }
#else//__GNUC__
    for (; word != 0; j++, word >>= 1) {
      if (word & 1) {
        return i*RegisterSet_WORDBITS + j;
      }
    }
#endif//__GNUC__
  }
  for (i++; i < wordCount; i++) {
    RegisterSetWord word = words[i];
#if __GNUC__ > 2
    if (word != 0) {
      if (sizeof(word) <= sizeof(int)) {
        j = __builtin_ctz(word);
      } else if (sizeof(word) <= sizeof(long long)) {
        j = __builtin_ctzll(word);
      } else Except_NEVER(true);
      return i*RegisterSet_WORDBITS + j;
    }
#else//__GNUC__
    for (j = 0; word != 0; j++, word >>= 1) {
      if (word & 1) {
        return i*RegisterSet_WORDBITS + j;
      }
    }
#endif//__GNUC__
  }
  return Register_;
}

#ifdef $XCC_h
/**
 * Choose and remove a member of the RegisterSet
 * starting from the last_reg, e.g., if rn was
 * last used, rn+1 will be return if it is avaiable.
 *
 * @return Valid Register or Register_ if this RegisterSet is empty.
 */
Register
RegisterSet_chooseFromLast(RegisterSet set, Register last_reg);
#endif//$XCC_h

Register
RegisterSet_chooseFromLast(RegisterSet set, Register last_reg)
{
  int wordCount = RegisterSet_WORDCOUNT;
  int index = 0, i, j;
  RegisterSetWord *words = RegisterSet_WORDS(set);
  RegisterSetWord word = 0;
  int start_idx = last_reg/RegisterSet_WORDBITS;
  int start_pos = last_reg - (start_idx * RegisterSet_WORDBITS);

  index = start_idx;
  if (RegisterSet_WORDBITS <= 32) {
    if (start_pos != 31)
      word = words[index] & (0xFFFFFFFF << (start_pos+1));
  }
  else {
    if (start_pos != 63)
      word = words[index] & (0xFFFFFFFFFFFFFFFFLL << (start_pos+1));
  }

  if (word)
    goto assign_reg;

  for (i = start_idx + 1;  i < wordCount; i++) {
    if (words[i] != 0) {
      word = words[i];
      index = i;
      goto assign_reg;
    }
  }
  for (i = 0; i < start_idx + 1; i++) {
    if (words[i] != 0) {
      word = words[i];
      index = i;
      goto assign_reg;
    }
  }

  return Register_;

assign_reg:
  {
    RegisterSetWord least = word & -word;
    if (RegisterSet_WORDBITS <= 32) {
// TODO: Add rules to configure to check for __builtin_clz and use it if it exists.
// #if defined(HAVE___BUILTIN_CLZ)
//    ERR_ASSERT(sizeof(RegisterSetWord) == SIZEOF_UNSIGNED_INT);
//    j = 31 - __builtin_clz(least);
#if defined(__st200__)
      j = 31 - __lzcntw(least);
#else
      int32_t value = least;
      j  = (-(value & 0x0000FFFF)>>31)<<4;
      j += (-(value & 0x00FF00FF)>>31)<<3;
      j += (-(value & 0x0F0F0F0F)>>31)<<2;
      j += (-(value & 0x33333333)>>31)<<1;
      j += (-(value & 0x55555555)>>31)+31;
#endif // HAVE___BUILTIN_CLZ
      Except_CHECK(j == HackerU32_popc(least - 1));
    }
    else {
      Except_CHECK(RegisterSet_WORDBITS == 64);
      j = HackerU64_popc(least - 1);
    }
    words[index] = word ^ least;
    return index*RegisterSet_WORDBITS + j;
  }
}

#ifdef $XCC_h
/**
 * Test a member for containment.
 * @return True if this RegisterSet contains member.
 */
bool
RegisterSet_contains(const_RegisterSet this, Register member);
#endif//$XCC_h

bool
RegisterSet_contains(const_RegisterSet this, Register member)
{
  RegisterSetWord old_word, new_word;
  int major = RegisterSet_MAJOR(member);
  int minor = RegisterSet_MINOR(member);
  Except_REQUIRE(major < RegisterSet_WORDCOUNT);
  old_word = new_word = RegisterSet_WORDS(this)[major];
  new_word |= ((RegisterSetWord)1 << minor);
  return old_word == new_word;
}

#ifdef $XCC_h
/**
 * Insert a member in this RegisterSet.
 * @return False iff member was already contained in this RegisterSet.
 */
bool
RegisterSet_insert(RegisterSet this, Register member);
#endif//$XCC_h

bool
RegisterSet_insert(RegisterSet this, Register member)
{
  RegisterSetWord old_word, new_word;
  int major = RegisterSet_MAJOR(member);
  int minor = RegisterSet_MINOR(member);
  Except_REQUIRE(major < RegisterSet_WORDCOUNT);
  old_word = new_word = RegisterSet_WORDS(this)[major];
  new_word |= ((RegisterSetWord)1 << minor);
  RegisterSet_WORDS(this)[major] = new_word;
  return old_word != new_word;
}

#ifdef $XCC_h
/**
 * Remove a member from this RegisterSet.
 * @return False iff member was not contained in this RegisterSet.
 */
bool
RegisterSet_remove(RegisterSet this, Register member);
#endif//$XCC_h

bool
RegisterSet_remove(RegisterSet this, Register member)
{
  RegisterSetWord old_word, new_word;
  int major = RegisterSet_MAJOR(member);
  int minor = RegisterSet_MINOR(member);
  Except_REQUIRE(major < RegisterSet_WORDCOUNT);
  old_word = new_word = RegisterSet_WORDS(this)[major];
  new_word &= ~((RegisterSetWord)1 << minor);
  RegisterSet_WORDS(this)[major] = new_word;
  return old_word != new_word;
}

#ifdef $XCC_h
/**
 * Test for RegisterSet equality.
 */
bool
RegisterSet_equals(const_RegisterSet this, const_RegisterSet that);
#endif//$XCC_h

bool
RegisterSet_equals(const_RegisterSet this, const_RegisterSet that)
{
  int i = 0;
  for (; i < RegisterSet_WORDCOUNT; i++) {
    if (RegisterSet_WORDS(this)[i] != RegisterSet_WORDS(that)[i]) return false;
  }
  return true;
}

#ifdef $XCC_h
/**
 * True iff this RegisterSet comprises that RegisterSet.
 */
bool
RegisterSet_comprises(const_RegisterSet this, const_RegisterSet that);
#endif//$XCC_h

bool
RegisterSet_comprises(const_RegisterSet this, const_RegisterSet that)
{
  int32_t i = 0;
  for (i = 0; i < RegisterSet_WORDCOUNT; i++) {
    RegisterSetWord this_word = RegisterSet_WORDS(this)[i];
    RegisterSetWord that_word = RegisterSet_WORDS(that)[i];
    if ((this_word | that_word) != this_word) return false;
  }
  return true;
}

#ifdef $XCC_h
/**
 * Union of this RegisterSet with that RegisterSet.
 */
void
RegisterSet_union(RegisterSet this, const_RegisterSet that);
#endif//$XCC_h

void
RegisterSet_union(RegisterSet this, const_RegisterSet that)
{
  int i = 0;
  for (; i < RegisterSet_WORDCOUNT; i++) {
    RegisterSet_WORDS(this)[i] |= RegisterSet_WORDS(that)[i];
  }
}

#ifdef $XCC_h
/**
 * Intersect this RegisterSet with that RegisterSet.
 */
void
RegisterSet_inter(RegisterSet this, const_RegisterSet that);
#endif//$XCC_h

void
RegisterSet_inter(RegisterSet this, const_RegisterSet that)
{
  int i = 0;
  for (; i < RegisterSet_WORDCOUNT; i++) {
    RegisterSet_WORDS(this)[i] &= RegisterSet_WORDS(that)[i];
  }
}

#ifdef $XCC_h
/**
 * Remove that RegisterSet members from this RegisterSet.
 */
void
RegisterSet_diff(RegisterSet this, const_RegisterSet that);
#endif//$XCC_h

void
RegisterSet_diff(RegisterSet this, const_RegisterSet that)
{
  int i = 0;
  for (; i < RegisterSet_WORDCOUNT; i++) {
    RegisterSet_WORDS(this)[i] &= ~RegisterSet_WORDS(that)[i];
  }
}

#ifdef $XCC_h
/**
 * Iterate over this RegisterSet registers
 */
#define RegisterSet_FOREACH_Register(this, registre) { \
  int registre = Register__UNDEF; \
  const_RegisterSet RegisterSet_THIS = (this); \
  int RegisterSet_COUNT = RegisterSet_WORDCOUNT, RegisterSet_I, RegisterSet_J; \
  for (RegisterSet_I = 0; RegisterSet_I < RegisterSet_COUNT; RegisterSet_I++) { \
    RegisterSetWord RegisterSet_WORD = RegisterSet_WORDS(RegisterSet_THIS)[RegisterSet_I]; \
    for (RegisterSet_J = 0; RegisterSet_J < (int)RegisterSet_WORDBITS;	\
        RegisterSet_J++, RegisterSet_WORD >>= 1, registre++) { \
      if (RegisterSet_WORD & 1) {
#define RegisterSet_ENDEACH_Register \
      } \
    } \
	if (RegisterSet_J != (int)RegisterSet_WORDBITS) break;	\
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pretty-print this RegisterSet.
 */
bool
RegisterSet_pretty(const_RegisterSet this, FILE *file, Processor processor);
#endif//$XCC_h

#ifndef _NTRACE
bool
RegisterSet_pretty(const_RegisterSet this, FILE *file, Processor processor)
{
  bool inRange = false;
  int prevReg = Register_;
  if (file == NULL) file = stderr;
  RegisterSet_FOREACH_Register(this, registre) {
    if (prevReg == Register_) {
      fprintf(file, "%s", Register_name(registre));
    } else if (registre == prevReg + 1 &&
        Register_regFile(registre, processor) == Register_regFile(prevReg, processor)) {
      // Inside Register range.
      inRange = true;
    } else if (inRange) {
      // Stop Register range.
      fprintf(file, "-%s", Register_name(prevReg));
      fprintf(file, " %s", Register_name(registre));
      inRange = false;
    } else {
      fprintf(file, " %s", Register_name(registre));
    }
    prevReg = registre;
  } RegisterSet_ENDEACH_Register;
  if (inRange && prevReg != Register_) {
    fprintf(file, "-%s", Register_name(prevReg));
  }
  return true;
}
#endif//_NTRACE


#ifdef $XCC_h
/**
 * Pretty-print this RegisterSet in MiniR format
 */
bool
RegisterSet_prettyMinir(const_RegisterSet this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
RegisterSet_prettyMinir(const_RegisterSet this, FILE *file)
{
  const char* sep = "";
  int prevReg = Register_;
  if (file == NULL) file = stderr;
  RegisterSet_FOREACH_Register(this, registre) {
    fprintf(file, "%s%s", sep, Register_name(registre));
    sep = ", ";
  } RegisterSet_ENDEACH_Register;
  return true;
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int registre;
  RegisterSet_ registerSet[1];
  RegisterSet_Ctor(registerSet);
  for (registre = 0; registre < Register__; registre++) {
    RegisterSet_insert(registerSet, registre);
  }
  while (!RegisterSet_isEmpty(registerSet)) {
    registre = RegisterSet_choose(registerSet);
    printf("choose: %s\n", Register_Id(registre));
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Add range of registers denoted by a string to this RegisterSet.
 */
bool
RegisterSet_addRange(RegisterSet this, const char *range, Processor processor);
#endif//$XCC_h

bool
RegisterSet_addRange(RegisterSet this, const char *range, Processor processor)
{
  const char *hyphen = strchr(range, '-');
  if (hyphen == NULL) {
    Register registre = Register_fromName(range, processor);
    if (registre != Register__UNDEF) {
      RegisterSet_insert(this, registre);
      return true;
    }
  } else {
    char first[hyphen + 1 - range];
    const char *second = hyphen + 1;
    memcpy(first, range, sizeof(first));
    first[sizeof(first) - 1] = '\0';
    {
      Register registre_1 = Register_fromName(first, processor);
      Register registre_2 = Register_fromName(second, processor);
      if (registre_1 != Register__UNDEF && registre_2 != Register__UNDEF) {
        int registre = registre_1;
        for (; registre < registre_2; registre++) {
          RegisterSet_insert(this, registre);
        }
        return registre_1 < registre_2;
      }
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Action on a Register for scoreboarding and bundling.
 */
struct RegisterAction_ {
  Register REGISTER;	// Implicit register if rank == 0.
  RegClass REGCLASS;	// RegClass if not implicit register.
  int8_t XXXINDEX;	        // Index of argument or result.
  int8_t RANK;	        	// Operand rank in Operands.
  int8_t STAGE;			// Access stage in the pipeline.
};
#endif//$XCC_h

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(RegisterAction_)\t%zu\n", sizeof(RegisterAction_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Register of this RegisterAction.
 */
static inline Register
RegisterAction_register(const_RegisterAction this)
{
  return (Register)RegisterAction_REGISTER(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * RegClass of this RegisterAction.
 */
static inline RegClass
RegisterAction_regClass(const_RegisterAction this)
{
  return (RegClass)RegisterAction_REGCLASS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Index of argument or result OperatorParameter(s).
 */
static inline int
RegisterAction_xxxIndex(const_RegisterAction this)
{
  return RegisterAction_XXXINDEX(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Rank of this RegisterAction in the Opcode Operand list.
 *
 * @return 0 if the RegisterAction has no corresponding Operand.
 */
static inline int
RegisterAction_rank(const_RegisterAction this)
{
  return RegisterAction_RANK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The pipeline stage of this RegisterAction.
 */
static inline int
RegisterAction_stage(const_RegisterAction this)
{
  return RegisterAction_STAGE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the pipeline stage of this RegisterAction.
 */
static inline void
RegisterAction_setStage(RegisterAction this, int stage)
{
  *RegisterAction__STAGE(this) = stage;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Array of RegisterAction(s).
 */
struct RegisterActionArray_ {
  uint8_t READCOUNT;			// Count of Read RegisterAction(s).
  uint8_t READSTART;			// Start index of variant Read RegisterAction(s).
  uint8_t WRITECOUNT;			// Count of Write RegisterAction(s).
  uint8_t WRITESTART;			// Start index of variant Write RegisterAction(s).
  RegisterAction_ *ITEMS;		// This RegisterActionArray items.
};
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of Read RegisterAction(s).
 */
static inline int
RegisterActionArray_readCount(const_RegisterActionArray this)
{
  return RegisterActionArray_READCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Start index of variant Read RegisterAction(s).
 */
static inline int
RegisterActionArray_readStart(const_RegisterActionArray this)
{
  return RegisterActionArray_READSTART(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of Write RegisterAction(s).
 */
static inline int
RegisterActionArray_writeCount(const_RegisterActionArray this)
{
  return RegisterActionArray_WRITECOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Start index of variant Write RegisterAction(s).
 */
static inline int
RegisterActionArray_writeStart(const_RegisterActionArray this)
{
  return RegisterActionArray_WRITESTART(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of Read and Write RegisterAction(s).
 */
static inline int
RegisterActionArray_count(const_RegisterActionArray this)
{
  return RegisterActionArray_READCOUNT(this) + RegisterActionArray_WRITECOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * this RegisterActionArray read RegisterAction(s).
 */
static inline const RegisterAction_ *
RegisterActionArray_readActions(const_RegisterActionArray this)
{
  return RegisterActionArray_ITEMS(this) + 0;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * this RegisterActionArray write RegisterAction(s).
 */
static inline const RegisterAction_ *
RegisterActionArray_writeActions(const_RegisterActionArray this)
{
  return RegisterActionArray_ITEMS(this) + RegisterActionArray_READCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by RegisterActionArray_FOREACH_RegisterAction.
 */
static inline const RegisterAction_ *
RegisterActionArray_items(const_RegisterActionArray this)
{
  return RegisterActionArray_ITEMS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this RegisterActionArray RegisterAction(s).
 */
#define RegisterActionArray_FOREACH_RegisterAction(this, action) { \
  const RegisterAction_ *RegisterActionArray_ITEMS = RegisterActionArray_items(this); \
  int RegisterActionArray_COUNT = RegisterActionArray_count(this), RegisterActionArray_INDEX = 0; \
  for (; RegisterActionArray_INDEX < RegisterActionArray_COUNT; RegisterActionArray_INDEX++) { \
    const_RegisterAction action = RegisterActionArray_ITEMS + RegisterActionArray_INDEX;
#define RegisterActionArray_ENDEACH_RegisterAction \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this RegisterActionArray Read RegisterAction(s).
 */
#define RegisterActionArray_FORREAD_RegisterAction(this, action) { \
  const RegisterAction_ *RegisterActionArray_ITEMS = RegisterActionArray_items(this); \
  int RegisterActionArray_READCOUNT = RegisterActionArray_readCount(this), RegisterActionArray_INDEX = 0; \
  for (; RegisterActionArray_INDEX < RegisterActionArray_READCOUNT; RegisterActionArray_INDEX++) { \
    const_RegisterAction action = RegisterActionArray_ITEMS + RegisterActionArray_INDEX;
#define RegisterActionArray_ENDREAD_RegisterAction \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this RegisterActionArray Write RegisterAction(s).
 */
#define RegisterActionArray_FORWRITE_RegisterAction(this, action) { \
  const RegisterAction_ *RegisterActionArray_ITEMS = RegisterActionArray_items(this); \
  int RegisterActionArray_READCOUNT = RegisterActionArray_readCount(this); \
  int RegisterActionArray_COUNT = RegisterActionArray_count(this); \
  int RegisterActionArray_INDEX = RegisterActionArray_READCOUNT; \
  for (; RegisterActionArray_INDEX < RegisterActionArray_COUNT; RegisterActionArray_INDEX++) { \
    const_RegisterAction action = RegisterActionArray_ITEMS + RegisterActionArray_INDEX;
#define RegisterActionArray_ENDWRITE_RegisterAction \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Register module.
 */
#define Register_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Register module.
 */
#define Register_FINI()
#endif//$XCC__h

#if XCC__C
static void
Register_TEST(void)
{
#include "MDT/Register_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Register_TEST);
  return 0;
}
#endif

