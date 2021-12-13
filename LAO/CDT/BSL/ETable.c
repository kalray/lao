#ifdef $XCC_h
/*
 * !!!!	ETable.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2007 STMicroelectronics.
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
 * @ingroup BSL
 * @brief Tables of enumerated values.
 */
#endif//$XCC_h


#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/ETable.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * ETable entry defined as { NAME, VALUE }.
 */
struct ETableEntry_ {
  const char *NAME;		// Name as string.
  int32_t VALUE;		// Integer value.
  uint32_t HASH;		// Hash value for NAME.
  struct ETableEntry_ *LINK;	// Link to entry of same HASH.
};
#endif//$XCC_h

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ETableEntry_)\t%zu\n", sizeof(ETableEntry_));
}
#endif//$XCC__c

/**
 * Used by qsort to sort the HTable ENTRIES.
 */
static int
ETableEntry_compare(const void *this, const void *that)
{
  const ETableEntry_ *this_entry = (const ETableEntry_ *)this;
  const ETableEntry_ *that_entry = (const ETableEntry_ *)that;
  return ETableEntry_VALUE(this_entry) - ETableEntry_VALUE(that_entry);
}

#ifdef $XCC_h
/**
 * Count the number of ETableEntry_ in an array.
 */
#define ETableEntry_COUNT(array)	(sizeof(array)/sizeof(ETableEntry_))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Static initializer for a ETableEntry_.
 */
#define ETableENTRY(Enum, name)	{ #name, Enum##_##name, 0, NULL }
#define ETableENTRY_NULL	{ NULL }
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Table to map enum value to name and name to value.
 *
 * The mapping from VALUE to NAME is obtained by sorting the ENTRIES during
 * initialization, then using dichotomy search. The mapping from NAME to VALUE is
 * obtained by hashing.
 */
struct ETable_ {
  //@args	Memory memory, ETableEntry_ *entries
  Memory MEMORY;
  ETableEntry_ *ENTRIES;	// This ETable ETableEntry(s).
  int32_t COUNT;		// Count this ETable ENTRIES.
  uint32_t MASK;		// Count this ETable BUCKETS.
  ETableEntry *BUCKETS;		// This ETable buckets for hashing.
};
#endif//$XCC_h

ETable
ETable_Ctor(ETable this, Memory memory, ETableEntry_ *entries)
{
  uint32_t mask = 0;
  bool sorted = true;
  int32_t count = 0, i;
  ETableEntry *buckets = NULL;
  // Count the ENTRIES and sort them if needed.
  if (entries[count].NAME != NULL) ++count;
  while (entries[count].NAME != NULL) {
    ETableEntry curr_entry = entries + count;
    ETableEntry prev_entry = curr_entry - 1;
    if (ETableEntry_VALUE(prev_entry) > ETableEntry_VALUE(curr_entry)) sorted = false;
    ++count;
  }
  if (!sorted) qsort(entries, count, sizeof(ETableEntry_), ETableEntry_compare);
  //
  mask = HackerU32_clp2(count) - 1;
  buckets = Memory_alloc(memory, (mask + 1)*sizeof(ETableEntry));
  for (i = 0; i < mask + 1; i++) buckets[i] = NULL;
  // Hash all the ENTRIES and fill the BUCKETS.
  for (i = 0; i < count; i++) {
    ETableEntry entry = entries + i;
    const char *name = ETableEntry_NAME(entry);
    uint32_t hash = Hacker_hashFNV32(name);
    int32_t index = hash & mask;
    *ETableEntry__HASH(entry) = hash;
    *ETableEntry__LINK(entry) = buckets[index];
    buckets[index] = entry;
  }
  //
  *ETable__MEMORY(this) = memory;
  *ETable__ENTRIES(this) = entries;
  *ETable__BUCKETS(this) = buckets;
  *ETable__COUNT(this) = count;
  *ETable__MASK(this) = mask;
  return this;
}

void
ETable_Dtor(ETable this)
{
  Memory memory = ETable_MEMORY(this);
  Memory_free(memory, ETable_BUCKETS(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ETable_)\t%zu\n", sizeof(ETable_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * For use by ETable_FOREACH_ETableEntry.
 */
static inline ETableEntry_ *
ETable_entries(const_ETable this)
{
  return ETable_ENTRIES(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by ETable_FOREACH_ETableEntry.
 */
static inline int32_t
ETable_count(const_ETable this)
{
  return ETable_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this ETable entries.
 */
#define ETable_FOREACH_ETableEntry(this, entry) { \
  ETableEntry_ *ETable_ENTRIES = ETable_entries(this); \
  int32_t ETable_COUNT = ETable_count(this), ETable_I = 0; \
  for (; ETable_I < ETable_COUNT; ETable_I++) { \
    ETableEntry entry = ETable_ENTRIES + ETable_I;
#define ETable_ENDEACH_ETableEntry \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this ETable entries.
 */
#define ETable_FORBACK_ETableEntry(this, entry) { \
  ETableEntry_ *ETable_ENTRIES = ETable_entries(this); \
  int32_t ETable_COUNT = ETable_count(this), ETable_I = ETable_COUNT - 1; \
  for (; ETable_I >= 0; ETable_I--) { \
    ETableEntry entry = ETable_ENTRIES + ETable_I;
#define ETable_ENDBACK_ETableEntry \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get a name from a value.
 */
const char *
ETable_name(const_ETable this, int32_t value);
#endif//$XCC_h

const char *
ETable_name(const_ETable this, int32_t value)
{
  ETableEntry_ *entries = ETable_ENTRIES(this);
  int32_t count = ETable_COUNT(this);
  int32_t l = 0, r = count - 1;
  while (r >= l) {
    int32_t i = (l + r) >> 1;
    int32_t i_value = ETableEntry_VALUE(entries + i);
    if (value < i_value) r = i - 1;
    else if (value > i_value) l = i + 1;
    else return ETableEntry_NAME(entries + i);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Get a value from a name.
 */
int32_t
ETable_value(const_ETable this, const char *name);
#endif//$XCC_h

int32_t
ETable_value(const_ETable this, const char *name)
{
  ETableEntry *buckets = ETable_BUCKETS(this);
  uint32_t hash = Hacker_hashFNV32(name);
  uint32_t mask = ETable_MASK(this);
  int32_t index = hash & mask;
  ETableEntry entry = buckets[index];
  for (; entry != NULL; entry = ETableEntry_LINK(entry)) {
    if (ETableEntry_HASH(entry) == hash &&
        !strcmp(ETableEntry_NAME(entry), name)) {
      return ETableEntry_VALUE(entry);
    }
  }
  return 0;
}

#ifdef $XCC_h
/**
 * Pretty print the full ETable
 */
bool
ETable_pretty(ETable this, const char *prefix, FILE *file);
#endif//$XCC_h

bool
ETable_pretty(ETable this, const char *prefix, FILE *file)
{
  fprintf(file, "%s[\n", prefix);
  ETable_FOREACH_ETableEntry(this, entry) {
    uint32_t value = ETableEntry_VALUE(entry);
    const char *name = ETableEntry_NAME(entry);
    fprintf(file, "%s%s = %d\n", prefix, name, value);
  } ETable_ENDEACH_ETableEntry;
  fprintf(file, "%s]\n", prefix);
  return true;
}

#ifdef $XCC_h
/**
 * Extract and pretty values out of flags.
 */
bool
ETable_prettyFlags(ETable this, uint32_t flags, const char *sep, FILE *file);
#endif//$XCC_h

bool
ETable_prettyFlags(ETable this, uint32_t flags, const char *sep, FILE *file)
{
  const char *string = "";
  ETable_FOREACH_ETableEntry(this, entry) {
    uint32_t value = ETableEntry_VALUE(entry);
    if ((value & flags) == value) {
      const char *name = ETableEntry_NAME(entry);
      fprintf(file, "%s%s", string, name);
      flags ^= value;
      string = sep;
    }
    if (flags == 0) break;
  } ETable_ENDEACH_ETableEntry;
  return true;
}

#ifdef $XCC_h
/**
 * Parse a set of flags separated by @c sep from a @c string.
 * @result \c false if the string contains an undetermined flag. 
 */
bool
ETable_parseFlags(ETable this, const char *string, char sep, uint32_t *flags);
#endif//$XCC_h

bool
ETable_parseFlags(ETable this, const char *string, char sep, uint32_t *flags)
{
  bool result = true;
  int tokens = 0;
  *flags = 0;
  while(string != NULL && *string != '\0') {
    size_t length = 0;
    const char *ptr = strchr(string, sep);
    if (ptr == NULL) length = strlen(string);
    else length = ptr - string;
    if (length > 0) {
      char *endptr = NULL;
      uint32_t parsed_flag = strtoul(string, &endptr, 0);
      if (endptr - string == length) {
        // Parsed a valid number, verify if it is a valid flag value
        uint32_t valid_flag = parsed_flag;
        ETable_FOREACH_ETableEntry(this, entry) {
          uint32_t value = ETableEntry_VALUE(entry);
          if (value & valid_flag) {
            valid_flag &= ~value;
          }
        } ETable_ENDEACH_ETableEntry;
        if (valid_flag != 0) result = false;
        *flags |= parsed_flag;
        tokens++;
      } else if (endptr == string) {
        parsed_flag = 0;
        ETable_FOREACH_ETableEntry(this, entry) {
          uint32_t value = ETableEntry_VALUE(entry);
          const char *name = ETableEntry_NAME(entry);
          if (strncmp(name, string, length) == 0 &&
              strlen(name) == length) {
            parsed_flag = value;
          }
        } ETable_ENDEACH_ETableEntry;
        if (parsed_flag == 0) result = false;
        *flags |= parsed_flag;
        tokens++;
      } else {
        result = false;
      }
    }
    if (ptr != NULL) ptr++;
    string = ptr;
  }
  return result && tokens > 0;
}

#ifdef $XCC__c
{
  typedef enum {
    Enum_Foo = 3,
    Enum_Bar = 8,
    Enum_Baz = 1,
    Enum_Bah = 0,
    Enum_A = 65,
    Enum_B,
    Enum_C,
    Enum_D,
    Enum_E,
    Enum_F,
  } Emum;
  ETableEntry_ Enum_Entries[] = {
    ETableENTRY(Enum, Foo),
    ETableENTRY(Enum, Bar),
    ETableENTRY(Enum, Baz),
    ETableENTRY(Enum, Bah),
    ETableENTRY(Enum, A),
    ETableENTRY(Enum, B),
    ETableENTRY(Enum, C),
    ETableENTRY(Enum, D),
    ETableENTRY(Enum, E),
    ETableENTRY(Enum, F),
    ETableENTRY_NULL,
  };
  ETable_ etable[1];
  ETable_Ctor(etable, Memory_Root, Enum_Entries);
  Except_ALWAYS(ETable_COUNT(etable) + 1 == ETableEntry_COUNT(Enum_Entries));
  ETable_FOREACH_ETableEntry(etable, entry) {
    ETableEntry link = ETableEntry_LINK(entry);
    printf("NAME=%s\t", ETableEntry_NAME(entry));
    printf("VALUE=%d\t", ETableEntry_VALUE(entry));
    printf("HASH=%d\t", ETableEntry_HASH(entry));
    printf("LINK=(%s)\n", link ? ETableEntry_NAME(link) : "null");
  } ETable_ENDEACH_ETableEntry;
  ETable_FOREACH_ETableEntry(etable, entry) {
    const char *NAME = ETableEntry_NAME(entry);
    int32_t VALUE = ETableEntry_VALUE(entry);
    const char *name = ETable_name(etable, VALUE);
    int32_t value = ETable_value(etable, NAME);
    printf("NAME=%s\tvalue=%d\t", NAME, value);
    printf("VALUE=%d\tname=%s\n", VALUE, name);
  } ETable_ENDEACH_ETableEntry;
  ETable_Dtor(etable);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the ETable module.
 */
#define ETable_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the ETable module.
 */
#define ETable_FINI()
#endif//$XCC__h

#if XCC__C
static void
ETable_TEST(void)
{
#include "BSL/ETable_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(ETable_TEST);
  return 0;
}
#endif

