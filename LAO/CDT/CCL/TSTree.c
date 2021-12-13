/*XCC.h
 * !!!!	TSTree.xcc
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
 *
 * A Ternary Search Tree is a structure discoved by Bentley and Sedgewick
 * used to build a dictionary of sequences. In the initial presentation,
 * sequences were C strings (null-terminated char[]).
 *
 * In the present implementation, the sequence elements are of type intptr_t,
 * and the terminating value is given at TSTree construction time. The TSTree
 * structure can thus hold sequence of integers or pointers.
 */

#ifdef $XCC_h
/**
 * @file
 * @ingroup CCL
 * @brief Ternary search tree of Sedgewick.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/TSTree.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * The TSTreeNode_ structure implements the TSTree tree nodes.
 */
struct TSTreeNode_ {
  //@args	intptr_t split
  intptr_t SPLIT;
  void *LTKID;
  void *EQKID;
  void *GTKID;
};
#endif//$XCC__h

TSTreeNode
TSTreeNode_Ctor(TSTreeNode this, intptr_t split)
{
  *TSTreeNode__SPLIT(this) = split;
  *TSTreeNode__LTKID(this) = NULL;
  *TSTreeNode__EQKID(this) = NULL;
  *TSTreeNode__GTKID(this) = NULL;
  return this;
}

static void
TSTreeNode_Dtor_(TSTreeNode this, intptr_t stop, Memory memory)
{
  if (TSTreeNode_LTKID(this) != NULL)
      TSTreeNode_Dtor_(TSTreeNode_LTKID(this), stop, memory);
  if (TSTreeNode_EQKID(this) != NULL && TSTreeNode_SPLIT(this) != stop)
      TSTreeNode_Dtor_(TSTreeNode_EQKID(this), stop, memory);
  if (TSTreeNode_GTKID(this) != NULL)
      TSTreeNode_Dtor_(TSTreeNode_GTKID(this), stop, memory);
  Memory_free_(memory, this);
}

#ifdef $XCC_h
/**
 * Ternary search tree as described by Bentley and Sedgewick.
 *
 * The TSTree structure enables to map a key, an array of sequence of @c intptr_t
 * values terminated by the designated @c stop value, to a @c intptr_t value.
 */
struct TSTree_;
  //@args	Memory memory, intptr_t stop
#endif//$XCC_h

#ifdef $XCC__h
struct TSTree_ {
  //@args	Memory memory, intptr_t stop
  //memory:	Where the TSTree nodes are allocated.
  //stop:	The special value that stops the search key.
  Memory MEMORY;
  intptr_t STOP;	// The stop value.
  TSTreeNode ROOT;	// The root node.
  TSTreeNode LAST;	// The last node inserted.
  int32_t COUNT;	// Current count of entries.
  int32_t LENGTH;	//<! Maximum key length.
};
#endif//$XCC__h

TSTree
TSTree_Ctor(TSTree this, Memory memory, intptr_t stop)
{
  *TSTree__MEMORY(this) = memory;
  *TSTree__STOP(this) = stop;
  *TSTree__ROOT(this) = NULL;
  *TSTree__LAST(this) = NULL;
  *TSTree__COUNT(this) = 0;
  *TSTree__LENGTH(this) = 0;
  return this;
}

void
TSTree_Dtor(TSTree this)
{
  if (TSTree_ROOT(this) != NULL) {
    TSTreeNode node = TSTree_ROOT(this);
    intptr_t stop = TSTree_STOP(this);
    Memory memory = TSTree_MEMORY(this);
    TSTreeNode_Dtor_(node, stop, memory);
  }
  *TSTree__ROOT(this) = NULL;
  *TSTree__LAST(this) = NULL;
  *TSTree__COUNT(this) = 0;
  *TSTree__LENGTH(this) = 0;
}

#ifdef $XCC_h
/**
 * This TSTree count of entries.
 */
int32_t
TSTree_count(const_TSTree this);
#endif//$XCC_h

int32_t
TSTree_count(const_TSTree this)
{
  return TSTree_COUNT(this);
}

#ifdef $XCC_h
/**
 * Search the TSTree tree.
 * key:	The array of @c intptr_t used as key.
 * Return:	Mapped value if there, else NULL.
 */
void *
TSTree_search(const_TSTree this, const intptr_t key[]);
#endif//$XCC_h

void *
TSTree_search(const_TSTree this, const intptr_t key[])
{
  intptr_t stop = TSTree_STOP(this);
  TSTreeNode node = TSTree_ROOT(this);
  while (node) {
    intptr_t split = key[0];
    if (split < TSTreeNode_SPLIT(node)) node = TSTreeNode_LTKID(node);
    else if (split > TSTreeNode_SPLIT(node)) node = TSTreeNode_GTKID(node);
    else {
      if (split == stop) {
        return TSTreeNode_EQKID(node);
      }
      node = TSTreeNode_EQKID(node);
      ++key;
    }
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Insert in the TSTree tree.
 * key:	The array of @c intptr_t used as key.
 * value:	The value to insert for @c key.
 * Return:	@c NULL if @c key already there, else @c value.
 */
void *
TSTree_insert(TSTree this, intptr_t key[], void *value);
#endif//$XCC_h

void *
TSTree_insert(TSTree this, intptr_t key[], void *value)
{
  TSTreeNode root = TSTree_ROOT(this);
  *TSTree__LAST(this) = NULL;
  *TSTree__ROOT(this) = TSTreeNode_insert(root, key, this);
  if (TSTree_LAST(this) != NULL) {
    int32_t length = 0;
    intptr_t stop = TSTree_STOP(this);
    TSTreeNode last = TSTree_LAST(this);
    while (key[length++] != stop);
    if (length > TSTree_LENGTH(this)) {
      *TSTree__LENGTH(this) = length;
    }
    ++*TSTree__COUNT(this);
    *TSTreeNode__EQKID(last) = value;
    return value;
  }
  return NULL;
}

#ifdef $XCC__h
TSTreeNode
TSTreeNode_insert(TSTreeNode this, intptr_t key[], TSTree tstree);
#endif//$XCC__h

TSTreeNode
TSTreeNode_insert(TSTreeNode this, intptr_t key[], TSTree tstree)
{
  intptr_t split = key[0];
  intptr_t stop = TSTree_STOP(tstree);
  if (this == NULL) {
    Memory memory = TSTree_MEMORY(tstree);
    this = Memory_alloc_(memory, sizeof(TSTreeNode_));
    TSTreeNode_Ctor(this, split);
    *TSTree__LAST(tstree) = this;
  }
  if (split < TSTreeNode_SPLIT(this)) {
    *TSTreeNode__LTKID(this) = TSTreeNode_insert(TSTreeNode_LTKID(this), key, tstree);
  } else if (split > TSTreeNode_SPLIT(this)) {
    *TSTreeNode__GTKID(this) = TSTreeNode_insert(TSTreeNode_GTKID(this), key, tstree);
  } else {
    if (split != stop) {
      *TSTreeNode__EQKID(this) = TSTreeNode_insert(TSTreeNode_EQKID(this), ++key, tstree);
    }
  }
  return this;
}

#ifdef $XCC__c
{
  // Check Memory_alloc_ is OK in TSTreeNode_insert.
  Except_ALWAYS(_ALIGNED(sizeof(TSTreeNode_)));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Map the TSTree entries using the supplied @c map function.
 */
void
TSTree_map(TSTree this, void (*map)(intptr_t [], void *, va_list), ...);
#endif//$XCC_h

void
TSTree_map(TSTree this, void (*map)(intptr_t [], void *, va_list), ...)
{
  va_list va;
  Memory memory = TSTree_MEMORY(this);
  intptr_t *key = Memory_alloc(memory, sizeof(intptr_t)*TSTree_LENGTH(this));
  if (TSTree_ROOT(this) != NULL) {
    va_start(va, map);
    TSTreeNode_map(TSTree_ROOT(this), TSTree_STOP(this), key, 0, map, va);
    va_end(va);
  }
  Memory_free_(memory, key);
}

#ifdef $XCC__h
void
TSTreeNode_map(TSTreeNode this, intptr_t stop, intptr_t key[], int i,
               void (*map)(intptr_t [], void *, va_list), va_list va);
#endif//$XCC__h

void
TSTreeNode_map(TSTreeNode this, intptr_t stop, intptr_t key[], int i,
               void (*map)(intptr_t [], void *, va_list), va_list va) {
  intptr_t split = TSTreeNode_SPLIT(this);
  if (split == stop) {
    if (key != NULL) key[i] = split;
    (*map)(key, TSTreeNode_EQKID(this), va);
  }
  if (TSTreeNode_LTKID(this) != NULL) {
    TSTreeNode_map(TSTreeNode_LTKID(this), stop, key, i, map, va);
  }
  if (TSTreeNode_EQKID(this) != NULL) {
    if (key != NULL) key[i] = split;
    if (split != stop) {
      TSTreeNode_map(TSTreeNode_EQKID(this), stop, key, i + 1, map, va);
    }
  }
  if (TSTreeNode_GTKID(this) != NULL) {
    TSTreeNode_map(TSTreeNode_GTKID(this), stop, key, i, map, va);
  }
}

#if XCC__C
static void dump(intptr_t key[], void *value, va_list va)
{
  int i;
  va_list va2;       // copy required for 64bit because va is not pass-by-value
  FILE *file;
  va_copy(va2, va);  // see http://www.bailopan.net/blog/?p=30
  file = (FILE *)va_arg(va2, FILE *);
  fprintf(file, "    ");
  for (i = 0; key[i] != 0; i++) {
    fprintf(file, "%c", (int)key[i]);
  }
  fprintf(file, "\t-> %s\n", (char *)value);
}

const char *
strings[] = {
  "aim", "your", "message", "at", "the", "audience", "you", "want", "to", "influence",
  "create", "specific", "messages", "for", "each", "segment", "of", "your", "audience",
  NULL
};
#endif

#ifdef $XCC__c
{
  int i, j;
  intptr_t key[100];
  TSTree_ tstree[1];
  TSTree_Ctor(tstree, Memory_Root, 0);
  for (i = 0; strings[i]; i++) {
    char *value = NULL;
    const char *p = strings[i];
    for (j = 0; p[j]; j++) {
      key[j] = p[j];
    }
    key[j] = p[j];
    printf("access(%s)\t", p);
    value = TSTree_search(tstree, key);
    if (value != NULL) {
      printf("-> '%s'\n", value);
    } else {
      printf("\n");
    }
    printf("insert(%s)\t", p);
    if (TSTree_insert(tstree, key, (void *)p)) {
      value = TSTree_search(tstree, key);
      printf("-> '%s'\n", value);
    } else {
      printf("*\n");
    }
  }
  printf("COUNT=%d\tLENGTH=%d\n",
      TSTree_COUNT(tstree),
      TSTree_LENGTH(tstree));
  TSTree_map(tstree, dump, stdout);
  TSTree_Dtor(tstree);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the TSTree module.
 */
#define TSTree_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the TSTree module.
 */
#define TSTree_FINI()
#endif//$XCC__h

#if XCC__C
static void
TSTree_TEST(void)
{
#include "CCL/TSTree_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(TSTree_TEST);
  return 0;
}
#endif

