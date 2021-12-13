#ifdef $XCC_h
/*
 * !!!!	String.xcc
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
/**
 * @file
 * @ingroup BSL
 * @brief @c String and @c StringTable implementations.
 *
 * A @c String is a single-copy, non-mutable object, that can contain arbitrary data.
 * String(s) pointers are equal only when the underlying data is the same. The
 * single copy property of @c String(s) is only relative to a particular @c StringTable (String
 * Table). The @c StringTable itself organizes its String(s) into a balanced
 * binary tree search structure.
 */
#endif//$XCC_h

#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/String.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * A single-copy, non-mutable object, maintained in a StringTable.
 *
 * An String is aligned to @c intptr_t alignment constraints, and its storage is
 * rounded up to @c sizeof(intptr_t). The leftover storage bytes are cleared.
 */
typedef const char *String;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access the underlying StringTableEntry of this String.
 */
#define String_ENTRY(this) (((StringTableEntry)((void*)(this))) - 1)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The memory size needed by this String
 */
#define String_Size(data, length, leaf) ( \
  StringTableEntry_DATA(leaf) != NULL? \
      ((length) + sizeof(intptr_t) - 1) & -sizeof(intptr_t): 0 \
)
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Information to manage a String into a StringTable.
 */
struct StringTableEntry_ {
  //@args	const void *data, int32_t length, StringTableEntry leaf
  struct StringTableEntry_ *LEFT;	// The left kid in the binary search tree.
  struct StringTableEntry_ *RIGHT;	// The right kid in the binary search tree.
  uint32_t HASHED;	// The hashed data.
  //@access PRIORITY	(StringTableEntry_HASHED(this))
  int32_t LENGTH;	// The data length. A zero length is also a flag for leaf nodes in Treap.
  const void *DATA;	// Where the data is located. Set to 0x1 in StringTable_LEAF in case of copies.
  //@access STRING	((String)((StringTableEntry)(this) + 1))
};
#endif//$XCC_h

StringTableEntry
StringTableEntry_Ctor(StringTableEntry this,
                      const void *data, int32_t length, StringTableEntry leaf)
{
  uint32_t hashed = Hacker_hashJenkins(data, length, 0) & 0x7fffffff;
  *StringTableEntry__LEFT(this) = leaf;
  *StringTableEntry__RIGHT(this) = leaf;
  *StringTableEntry__HASHED(this) = hashed;
  *StringTableEntry__LENGTH(this) = length;
  *StringTableEntry__DATA(this) = data;
  return this;
}

size_t
StringTableEntry_Size(const void *data, int32_t length, StringTableEntry leaf)
{
  return sizeof(StringTableEntry_) + String_Size(data, length, leaf);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * return the hashed value of @c this String.
 */
static inline uint32_t
String_hashed(String this)
{
  StringTableEntry entry = String_ENTRY(this);
  return StringTableEntry_HASHED(entry);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * return the length of @c this String.
 */
static inline int32_t
String_length(String this)
{
  StringTableEntry entry = String_ENTRY(this);
  return StringTableEntry_LENGTH(entry);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * return the data of @c this String.
 */
static inline const void *
String_data(String this)
{
  StringTableEntry entry = String_ENTRY(this);
  return StringTableEntry_DATA(entry);
}
#endif//$XCC_h

#ifdef $XCC__h
/**
 * Given @c this pointer, construct the String.
 */
void
String_Ctor(String this, const void *data, int32_t length, const_StringTableEntry leaf);
#endif//$XCC__h

void
String_Ctor(String this, const void *data, int32_t length, const_StringTableEntry leaf)
{
  intptr_t *first = (intptr_t *)this;
  size_t size = String_Size(data, length, leaf);
  intptr_t *past = (intptr_t *)((char *)this + size);
  StringTableEntry entry = String_ENTRY(this);
  if (first < past) {
    *StringTableEntry__DATA(entry) = this;
    past[-1] = 0;	// Clear the padding bytes implied by String_Size.
    memcpy(first, data, length);
  } else {
    // No copies of data: nothing to do.
  }
}

static void
StringTableEntry_Dtor_(StringTableEntry this, const_StringTableEntry leaf, Memory memory)
{
  StringTableEntry left = StringTableEntry_LEFT(this);
  StringTableEntry right = StringTableEntry_RIGHT(this);
  if (left != leaf) StringTableEntry_Dtor_(left, leaf, memory);
  if (right != leaf) StringTableEntry_Dtor_(right, leaf, memory);
  Memory_free(memory, this);
}

#ifdef $XCC__h
static inline int32_t
StringTableEntry_cmp_(const_StringTableEntry this, const void *that_data, int32_t that_length)
{
  int32_t this_length = StringTableEntry_LENGTH(this);
  const char *this_data = StringTableEntry_DATA(this);
  int32_t cmp = this_length - that_length;
  if (cmp == 0) {
    cmp = memcmp(this_data, that_data, that_length);
  }
  return cmp;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline int32_t
StringTableEntry_cmp(const_StringTableEntry this, const_StringTableEntry that)
{
  const char *that_data = StringTableEntry_DATA(that);
  int32_t that_length = StringTableEntry_LENGTH(that);
  return StringTableEntry_cmp_(this, that_data, that_length);
}
#endif//$XCC__h

/**
 * Simple top down splay, not requiring n to be in the tree t.
 *
 * Adapted from ftp://ftp.cs.cmu.edu/user/sleator/splaying/top-down-splay.c
 * "An implementation of top-down splaying", D. Sleator <sleator@cs.cmu.edu>
 */
static StringTableEntry
StringTableEntry_Splay(StringTableEntry t, StringTableEntry n, StringTableEntry leaf)
{
  StringTableEntry_ N, *l, *r, *y;
  if (t == leaf) return t;
  N.LEFT = N.RIGHT = leaf;
  l = r = &N;
  for (;;) {
    int32_t cmp = StringTableEntry_cmp(n, t);
    if (cmp < 0) {
      if (StringTableEntry_LEFT(t) == leaf) break;
      cmp = StringTableEntry_cmp(n, StringTableEntry_LEFT(t));
      if (cmp < 0) {
        y = StringTableEntry_LEFT(t);			/* rotate right */
        *StringTableEntry__LEFT(t) = StringTableEntry_RIGHT(y);
        *StringTableEntry__RIGHT(y) = t;
        t = y;
        if (StringTableEntry_LEFT(t) == leaf) break;
      }
      *StringTableEntry__LEFT(r) = t;			/* link right */
      r = t;
      t = StringTableEntry_LEFT(t);
    } else if (cmp > 0) {
      if (StringTableEntry_RIGHT(t) == leaf) break;
      cmp = StringTableEntry_cmp(n, StringTableEntry_RIGHT(t));
      if (cmp > 0) {
        y = StringTableEntry_RIGHT(t);			/* rotate left */
        *StringTableEntry__RIGHT(t) = StringTableEntry_LEFT(y);
        *StringTableEntry__LEFT(y) = t;
        t = y;
        if (StringTableEntry_RIGHT(t) == leaf) break;
      }
      *StringTableEntry__RIGHT(l) = t;			/* link left */
      l = t;
      t = StringTableEntry_RIGHT(t);
    } else {
      break;
    }
  }
  *StringTableEntry__RIGHT(l) = StringTableEntry_LEFT(t);		/* assemble */
  *StringTableEntry__LEFT(r) = StringTableEntry_RIGHT(t);
  *StringTableEntry__LEFT(t) = N.RIGHT;
  *StringTableEntry__RIGHT(t) = N.LEFT;
  return t;
}

/*
 * This function can be called only if K2 has a left child.
 * Perform a rotate between a node (K2) and its left child.
 * Update heights, then return new root.
 */
static inline StringTableEntry
StringTableEntry_SingleRotateWithLeft(StringTableEntry K2)
{
  StringTableEntry K1;
  K1 = StringTableEntry_LEFT(K2);
  *StringTableEntry__LEFT(K2) = StringTableEntry_RIGHT(K1);
  *StringTableEntry__RIGHT(K1) = K2;
  return K1;			/* New root */
}

/*
 * This function can be called only if K1 has a right child.
 * Perform a rotate between a node (K1) and its right child.
 * Update heights, then return new root.
 */
static inline StringTableEntry
StringTableEntry_SingleRotateWithRight(StringTableEntry K1)
{
  StringTableEntry K2;
  K2 = StringTableEntry_RIGHT(K1);
  *StringTableEntry__RIGHT(K1) = StringTableEntry_LEFT(K2);
  *StringTableEntry__LEFT(K2) = K1;
  return K2;			/* New root */
}

static StringTableEntry
StringTableEntry_insertTreap(StringTableEntry t, StringTableEntry n, StringTableEntry leaf)
{
  if (t == leaf) {
    // Flag that n is inserted.
    *StringTableEntry__LENGTH(leaf) = -1;
    t = n;
  } else {
    int32_t cmp = StringTableEntry_cmp(n, t);
    if (cmp < 0) {
      *StringTableEntry__LEFT(t) = StringTableEntry_insertTreap(StringTableEntry_LEFT(t), n, leaf);
      if (StringTableEntry_PRIORITY(StringTableEntry_LEFT(t)) < StringTableEntry_PRIORITY(t)) {
        t = StringTableEntry_SingleRotateWithLeft(t);
      }
    } else if (cmp > 0) {
      *StringTableEntry__RIGHT(t) = StringTableEntry_insertTreap(StringTableEntry_RIGHT(t), n, leaf);
      if (StringTableEntry_PRIORITY(StringTableEntry_RIGHT(t)) < StringTableEntry_PRIORITY(t)) {
        t = StringTableEntry_SingleRotateWithRight(t);
      }
    }
  }
  // Otherwise it's a duplicate; do nothing.
  return t;
}

static StringTableEntry
StringTableEntry_removeTreap(StringTableEntry t, StringTableEntry n, StringTableEntry leaf)
{
  int32_t cmp = StringTableEntry_cmp(n, t);
  if (cmp < 0) {
    *StringTableEntry__LEFT(t) = StringTableEntry_removeTreap(StringTableEntry_LEFT(t), n, leaf);
  } else if (cmp > 0) {
    *StringTableEntry__RIGHT(t) = StringTableEntry_removeTreap(StringTableEntry_RIGHT(t), n, leaf);
  } else {
    // Match found.
    if (StringTableEntry_PRIORITY(StringTableEntry_LEFT(t)) <
        StringTableEntry_PRIORITY(StringTableEntry_RIGHT(t))) {
      t = StringTableEntry_SingleRotateWithLeft(t);
    } else {
      t = StringTableEntry_SingleRotateWithRight(t);
    }
    if (t != leaf) {
      // Continue on down.
      t = StringTableEntry_removeTreap(t, n, leaf);
    } else {
      // At a leaf.
      *StringTableEntry__LEFT(t) = leaf;
    }
  }
  return t;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Implements a table of String(s) without duplicates.
 *
 * @c copies:	If set, the string will be copied when entered in the StringTable.
 */
struct StringTable_ {
  //@args	Memory memory, bool copies
  Memory MEMORY;	// Where the String(s) are allocated.
  struct StringTableEntry_ *ROOT;
  StringTableEntry_ LEAF_;
  //@access LEAF	StringTable__LEAF_(this)
  //@access COPIES	(StringTableEntryDATA(StringTable_LEAF(this)) == (const void *)1)
  uint32_t COUNT;
};
#endif//$XCC_h

StringTable
StringTable_Ctor(StringTable this, Memory memory, bool copies)
{
  StringTableEntry leaf = StringTable_LEAF(this);
  *StringTable__MEMORY(this) = memory;
  *StringTable__ROOT(this) = StringTable_LEAF(this);
  StringTableEntry_Ctor(leaf, NULL, 0, leaf);
  *StringTableEntry__HASHED(leaf) = (uint32_t)(int32_t)-1; // For Treap.
  if (copies) *StringTableEntry__DATA(leaf) = (const void *)0x1;
  *StringTable__COUNT(this) = 0;
  return this;
}

void
StringTable_Dtor(StringTable this)
{
  const_StringTableEntry leaf = StringTable_LEAF(this);
  Memory memory = StringTable_MEMORY(this);
  if (StringTable_ROOT(this) != leaf) {
    StringTableEntry_Dtor_(StringTable_ROOT(this), leaf, memory);
  }
  *StringTable__ROOT(this) = StringTable_LEAF(this);
  *StringTable__COUNT(this) = 0;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(StringTable_)\t%zu\n", sizeof(StringTable_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This StringTable Memory.
 */
static inline Memory
StringTable_memory(const_StringTable this)
{
  return StringTable_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Check if this StringTable copies elements added to it.
 */
static inline bool
StringTable_copies(const_StringTable this)
{
  const_StringTableEntry leaf = StringTable_LEAF(this);
  return StringTableEntry_DATA(leaf) != NULL;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Search the String corresponding to data.
 *
 * Return:	The String corresponding to data, or NULL if not found.
 */
String
StringTable_search(const_StringTable this, const void *data, int32_t length);
#endif//$XCC_h

/*
 * Standard binary tree search.
 */
String
StringTable_search(const_StringTable this, const void *data, int32_t length)
{
  const_StringTableEntry t = StringTable_ROOT(this);
  const_StringTableEntry leaf = StringTable_LEAF(this);
  while (t != leaf) {
    int32_t cmp = StringTableEntry_cmp_(t, data, length);
    if (cmp > 0) t = StringTableEntry_LEFT(t);
    else if (cmp < 0) t = StringTableEntry_RIGHT(t);
    else return StringTableEntry_STRING(t);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Lookup the String corresponding to data in this StringTable.
 *
 * Return:	The String corresponding to data
 */
String
StringTable_lookup(StringTable this, const void *data, int32_t length);
#endif//$XCC_h

String
StringTable_lookup(StringTable this, const void *data, int32_t length)
{
  const_StringTableEntry t = StringTable_ROOT(this);
  const_StringTableEntry leaf = StringTable_LEAF(this);
  while (t != leaf) {
    int32_t cmp = StringTableEntry_cmp_(t, data, length);
    if (cmp > 0) t = StringTableEntry_LEFT(t);
    else if (cmp < 0) t = StringTableEntry_RIGHT(t);
    else return StringTableEntry_STRING(t);
  }
  return StringTable_insert(this, data, length);
}

#ifdef $XCC_h
/**
 * Insert data into this StringTable, unless it's already there.
 */
String
StringTable_insert(StringTable this, const void *data, int32_t length);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Remove data from this StringTable if it's there.
 */
bool
StringTable_remove(StringTable this, const void *data, int32_t length);
#endif//$XCC_h

#ifdef $XCC__h
#ifndef StringTable_SPLAY
# define StringTable_insertTreap(this, data, length) \
    StringTable_insert(this, data, length)
# define StringTable_removeTreap(this, data, length) \
        StringTable_remove(this, data, length)
#else
# define StringTable_insertSplay(this, data, length) \
            StringTable_insert(this, data, length)
# define StringTable_removeSplay(this, data, length) \
                StringTable_remove(this, data, length)
#endif//StringTable_SPLAY
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Insert data into this table, unless it's already there.
 *
 * Adapted from ftp://ftp.cs.cmu.edu/user/sleator/splaying/top-down-splay.c
 * "An implementation of top-down splaying", D. Sleator <sleator@cs.cmu.edu>
 */
String
StringTable_insertSplay(StringTable this, const void *data, int32_t length);
#endif//$XCC__h

String
StringTable_insertSplay(StringTable this, const void *data, int32_t length)
{
  StringTableEntry t = StringTable_ROOT(this);
  StringTableEntry leaf = StringTable_LEAF(this);
  Memory memory = StringTable_MEMORY(this);
  size_t size = StringTableEntry_Size(data, length, leaf);
  StringTableEntry n = Memory_alloc(memory, size);
  StringTableEntry_Ctor(n, data, length, leaf);
  String_Ctor(StringTableEntry_STRING(n), data, length, leaf);
  if (t == leaf) {
    *StringTableEntry__LEFT(n) = *StringTableEntry__RIGHT(n) = leaf;
    *StringTable__COUNT(this) = 1;
    *StringTable__ROOT(this) = n;
    return StringTableEntry_STRING(n);
  }
  t = StringTableEntry_Splay(t, n, leaf);
  {
    int32_t cmp = StringTableEntry_cmp(n, t);
    if (cmp < 0) {
      *StringTableEntry__LEFT(n) = StringTableEntry_LEFT(t);
      *StringTableEntry__RIGHT(n) = t;
      *StringTableEntry__LEFT(t) = leaf;
      ++*StringTable__COUNT(this);
      *StringTable__ROOT(this) = n;
      return StringTableEntry_STRING(n);
    } else if (cmp > 0) {
      *StringTableEntry__RIGHT(n) = StringTableEntry_RIGHT(t);
      *StringTableEntry__LEFT(n) = t;
      *StringTableEntry__RIGHT(t) = leaf;
      ++*StringTable__COUNT(this);
      *StringTable__ROOT(this) = n;
      return StringTableEntry_STRING(n);
    } else {
      // We get here if it's already in the tree. Don't add it again.
      Memory_free(memory, n);
      *StringTable__ROOT(this) = t;
      return NULL;
    }
  }
}

/**
 * Remove data from the tree if it's there.
 *
 * Adapted from ftp://ftp.cs.cmu.edu/user/sleator/splaying/top-down-splay.c
 * "An implementation of top-down splaying", D. Sleator <sleator@cs.cmu.edu>
 */
static bool
StringTable_removeSplay(StringTable this, const void *data, int32_t length)
{
  Memory memory = StringTable_MEMORY(this);
  StringTableEntry t = StringTable_ROOT(this);
  StringTableEntry leaf = StringTable_LEAF(this);
  String a = StringTable_search(this, data, length);
  if (a) {
    StringTableEntry n = String_ENTRY(a), x = 0;
    t = StringTableEntry_Splay(t, n, leaf);
    if (!StringTableEntry_cmp(n, t)) {		/* found it */
      if (StringTableEntry_LEFT(t) == leaf) {
        x = StringTableEntry_RIGHT(t);
      } else {
        x = StringTableEntry_Splay(StringTableEntry_LEFT(t), n, leaf);
        *StringTableEntry__RIGHT(x) = StringTableEntry_RIGHT(t);
      }
      --*StringTable__COUNT(this);
      Memory_free(memory, t);
      *StringTable__ROOT(this) = x;
      return true;
    }
    *StringTable__ROOT(this) = t;		/* wasn't there */
  }
  return false;
}

#ifdef $XCC__h
/**
 * Insert data into the tree t, unless it's already there.
 *
 * Adapted from http://www.cs.fiu.edu/~weiss/dsaa_c2e/files.html
 * "Data Structures and Algorithm Analysis in C", Mark Allen Weiss <weiss@fiu.edu>
 */
String
StringTable_insertTreap(StringTable this, const void *data, int32_t length);
#endif//$XCC__h

String
StringTable_insertTreap(StringTable this, const void *data, int32_t length)
{
  StringTableEntry t = StringTable_ROOT(this);
  StringTableEntry leaf = StringTable_LEAF(this);
  size_t size = StringTableEntry_Size(data, length, leaf);
  Memory memory = StringTable_MEMORY(this);
  StringTableEntry n = Memory_alloc(memory, size);
  StringTableEntry_Ctor(n, data, length, leaf);
  String_Ctor(StringTableEntry_STRING(n), data, length, leaf);
  t = StringTableEntry_insertTreap(t, n, leaf);
  if (StringTableEntry_LENGTH(leaf) != 0) {
    *StringTableEntry__LENGTH(leaf) = 0;
    ++*StringTable__COUNT(this);
    *StringTable__ROOT(this) = t;
    return StringTableEntry_STRING(n);
  } else {
    Memory_free(memory, n);
    return NULL;
  }
}

/**
 * Remove data from the tree t.
 *
 * Adapted from http://www.cs.fiu.edu/~weiss/dsaa_c2e/files.html
 * "Data Structures and Algorithm Analysis in C", Mark Allen Weiss <weiss@fiu.edu>
 */
bool
StringTable_removeTreap(StringTable this, const void *data, int32_t length)
{
  Memory memory = StringTable_MEMORY(this);
  StringTableEntry t = StringTable_ROOT(this);
  StringTableEntry leaf = StringTable_LEAF(this);
  String a = StringTable_search(this, data, length);
  if (a != NULL) {
    StringTableEntry n = String_ENTRY(a);
    t = StringTableEntry_removeTreap(t, n, leaf);
    --*StringTable__COUNT(this);
    Memory_free(memory, n);
    *StringTable__ROOT(this) = t;
    return true;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Map the StringTable String(s) using the supplied @c map function.
 * map:	The function that will be called for each String in the StringTable.
 *               First argument of =(*map)= is the current String. Second argument
 *               of =(*map)= is the @c va_list corresponding to the =...=
 *               arguments of StringTable_map.
 *  =...=:	The other arguments if any to be passed to =(*map)= as @c va_list.
 */
void
StringTable_map(StringTable this, void (*map)(String, va_list), ...);
#endif//$XCC_h

#ifdef XCC__C
static int32_t StringTable_indent;
#endif//XCC__C

void
StringTable_map(StringTable this, void (*map)(String, va_list), ...)
{
  va_list va;
  const_StringTableEntry leaf = StringTable_LEAF(this);
#ifdef XCC__C
  StringTable_indent = -1;
#endif//XCC__C
  if (StringTable_ROOT(this) != leaf) {
    va_start(va, map);
    StringTableEntry_map(StringTable_ROOT(this), leaf, map, va);
    va_end(va);
  }
}

#ifdef $XCC__h
void
StringTableEntry_map(StringTableEntry this, const_StringTableEntry leaf,
                void (*map)(String, va_list), va_list va);
#endif//$XCC__h

void
StringTableEntry_map(StringTableEntry this, const_StringTableEntry leaf,
                void (*map)(String, va_list), va_list va)
{
#ifdef XCC__C
  StringTable_indent++;
#endif//XCC__C
  (*map)(StringTableEntry_STRING(this), va);
  if (StringTableEntry_LEFT(this) != leaf) {
    StringTableEntry_map(StringTableEntry_LEFT(this), leaf, map, va);
  }
  if (StringTableEntry_RIGHT(this) != leaf) {
    StringTableEntry_map(StringTableEntry_RIGHT(this), leaf, map, va);
  }
#ifdef XCC__C
  StringTable_indent--;
#endif//XCC__C
}

#ifdef $XCC__c
{
  int32_t i, j, *k;
  StringTable_ stringTable[1];
  StringTable_Ctor(stringTable, Memory_Root, true);
#define NumItems 500
  printf("Insert:\n");
  for (i = 0, j = 0; i < NumItems; i++, j = (j + 7)%NumItems) {
    StringTable_insert(stringTable, &j, 4);
    k = (int32_t *)StringTable_search(stringTable, &j, 4);
    if (!k || *k != j) printf("Error0 at %d\n", j);
  }
  printf("String count = %d\n", StringTable_COUNT(stringTable));
  for (j = 0; j < 2; j++) {
    for (i = 0; i < NumItems; i++) {
      k = (int32_t *)StringTable_search(stringTable, &i, 4);
      if (!k || *k != i) printf("Error1 at %d\n", i);
    }
  }
  printf("Remove:\n");
  for (i = 0; i < NumItems; i += 2) {
    if (StringTable_remove(stringTable, &i, 4));
    else printf("Error3 at %d\n", i);
  }
  for (i = 1; i < NumItems; i += 2) {
    k = (int32_t *)StringTable_search(stringTable, &i, 4);
    if (!k || *k != i) printf("Error4 at %d\n", i);
  }
  for (i = 0; i < NumItems; i += 2) {
    k = (int32_t *)StringTable_search(stringTable, &i, 4);
    if (k) printf("Error5 at %d\n", i);
  }
  printf("String count = %d\n", StringTable_COUNT(stringTable));
  for (i = 1; i < NumItems; i += 2) {
    if (StringTable_remove(stringTable, &i, 4));
    else printf("Error6 at %d\n", i);
  }
  printf("String count = %d\n", StringTable_COUNT(stringTable));
  StringTable_Dtor(stringTable);
}
#endif//$XCC__c

#if XCC__C
static void
dump(String string, va_list va)
{
  va_list va2;       // copy required for 64bit because va is not pass-by-value
  FILE *file;
  va_copy(va2, va);  // see http://www.bailopan.net/blog/?p=30
  file = (FILE *)va_arg(va2, FILE *);
  fprintf(file, "    %s (%d)\n", (const char *)String_data(string), StringTable_indent);
}

const char *
strings[] = {
  "aim", "your", "message", "at", "the", "audience", "you", "want", "to", "influence",
  "create", "specific", "messages", "for", "each", "segment", "of", "your", "audience",
  0
};

static void
StringTable_test(bool copies)
{
  int32_t i;
  String string = 0;
  StringTable_ stringTable[1];
  StringTable_Ctor(stringTable, Memory_Root, copies);
  for (i = 0; strings[i]; i++) {
    const char *data = strings[i];
    int32_t length = strlen(data) + 1;
    printf("access(%s)\t", data);
    string = StringTable_search(stringTable, data, length);
    if (string != NULL) {
      printf("-> '%s'\n", (const char *)String_data(string));
    } else {
      printf("\n");
    }
    printf("insert(%s)\t", data);
    string = StringTable_insert(stringTable, data, length);
    if (string != NULL) {
      printf("-> '%s'\n", (const char *)String_data(string));
    } else {
      printf("*\n");
    }
  }
  StringTable_map(stringTable, dump, stdout, 0);
  StringTable_Dtor(stringTable);
}
#endif

#ifdef $XCC__c
{
  StringTable_test(true);
  StringTable_test(false);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Make a string from %s.
 */
String
StringTable_S(StringTable this, const char *name);
#endif//$XCC_h

String
StringTable_S(StringTable this, const char *name)
{
  int32_t length = strlen(name) + 1;
  String string = StringTable_insert(this, name, length);
  Except_REQUIRE(StringTable_copies(this));
  if (string == NULL) {
    return StringTable_search(this, name, length);
  }
  return string;
}

#ifdef $XCC_h
/**
 * Make a string from %lld.
 */
String
StringTable_D(StringTable this, int64_t lld);
#endif//$XCC_h

String
StringTable_D(StringTable this, int64_t lld)
{
  char data[64];
  int32_t length = sprintf(data, "%0" PRId64, lld) + 1;
  String string = StringTable_insert(this, data, length);
  Except_REQUIRE(StringTable_copies(this));
  if (string == NULL) {
    return StringTable_search(this, data, length);
  }
  return string;
}

#ifdef $XCC__c
{
  const char *string = "BSL Library";
  StringTable_ table[1];
  StringTable_Ctor(table, Memory_Root, true);
  {
    String name = StringTable_S(table, string);
    String number = StringTable_D(table, 1+0);
    printf("name\t%s\n", name);
    printf("number\t%s\n", number);
  }
  StringTable_Dtor(table);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Information attached to a String in a StringStore.
 */
struct StringStoreItem_ {
  //@args	int32_t length, struct StringStoreItem_ *next
  int32_t LENGTH;
  struct StringStoreItem_ *NEXT;
};
#endif//$XCC_h

StringStoreItem
StringStoreItem_Ctor(StringStoreItem this,
                     int32_t length, struct StringStoreItem_ *next)
{
  *StringStoreItem__LENGTH(this) = length;
  *StringStoreItem__NEXT(this) = next;
  return this;
}

#ifdef $XCC_h
/**
 * Simple store to save strings.
 */
struct StringStore_ {
  //@args	Memory memory
  Memory MEMORY;
  StringStoreItem FIRST;
};
#endif//$XCC_h

StringStore
StringStore_Ctor(StringStore this, Memory memory)
{
  *StringStore__MEMORY(this) = memory;
  *StringStore__FIRST(this) = NULL;
  return this;
}

void
StringStore_Dtor(StringStore this)
{
  Memory memory = StringStore_MEMORY(this);
  StringStoreItem item = StringStore_FIRST(this);
  while (item != NULL) {
    StringStoreItem next = StringStoreItem_NEXT(item);
    Memory_free(memory, item);
    item = next;
  }
  *StringStore__FIRST(this) = NULL;
}

#ifdef $XCC_h
/**
 * This StringStore Memory.
 */
static inline Memory
StringStore_memory(const_StringStore this)
{
  return StringStore_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use in StringStore_FOREACH_String.
 */
static inline StringStoreItem
StringStore_first(const_StringStore this)
{
  return StringStore_FIRST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this StringStore String(s).
 */
#define StringStore_FOREACH_String(this, string) { \
  StringStoreItem StringStore_ITEM = StringStore_first(this); \
  while (StringStore_ITEM != NULL) { \
    StringStoreItem StringStore_NEXT = StringStoreItem_NEXT(StringStore_ITEM); \
    String string = (String)(StringStore_ITEM + 1);
#define StringStore_ENDEACH_String \
    StringStore_ITEM = StringStore_NEXT; \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Push a String onto this StringStore.
 */
String
StringStore_push(StringStore this, const void *data, int32_t length);
#endif//$XCC_h

String
StringStore_push(StringStore this, const void *data, int32_t length)
{
  Memory memory = StringStore_MEMORY(this);
  size_t size = sizeof(StringStoreItem_) + length;
  StringStoreItem first = StringStore_FIRST(this);
  StringStoreItem item = Memory_alloc(memory, size);
  StringStoreItem_Ctor(item, length, first);
  memcpy(item + 1, data, length);
  *StringStore__FIRST(this) = item;
  return (String)(item + 1);
}

#ifdef $XCC_h
/**
 * Push a null-teminated C string onto this StringStore.
 */
static inline String
StringStore_S(StringStore this, const char *name)
{
  int32_t length = strlen(name) + 1;
  return StringStore_push(this, name, length);
}
#endif//$XCC_h

#ifdef $XCC__c
{
  StringStore_ stringStore[1];
  StringStore_Ctor(stringStore, Memory_Root);
  StringStore_S(stringStore, "this is the end...");
  StringStore_S(stringStore, "when the music's over");
  StringStore_FOREACH_String(stringStore, string) {
    printf("String:\t%s\n", string);
  } StringStore_ENDEACH_String;
  StringStore_Dtor(stringStore);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the StringTable module.
 */
#define String_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the StringTable module.
 */
#define String_FINI()
#endif//$XCC__h

#if XCC__C
static void
String_TEST(void)
{
#include "BSL/String_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(String_TEST);
  return 0;
}
#endif

