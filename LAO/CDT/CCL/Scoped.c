#ifdef $XCC_h
/*
 * !!!!	Scoped.xcc
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
 * @ingroup CCL
 * @brief Scoped symbol table implementation.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/Scoped.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef const void *ScopedKey;
typedef void *ScopedValue;
typedef void (*ScopedValueRelease)(ScopedValue);
#endif//$XCC_h

#ifdef $XCC_h
/*
  ScopedEntry -- Object inlined below ScopedEntry.
 */
struct ScopedEntry_ {
  //@args	void *symbol, int32_t scope
  void *SYMBOL;
  int32_t SCOPE;
};
#endif//$XCC_h

ScopedEntry
ScopedEntry_Ctor(ScopedEntry this, void *symbol, int32_t scope)
{
  *ScopedEntry__SYMBOL(this) = symbol;
  *ScopedEntry__SCOPE(this) = scope;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ScopedEntry_)\t%zu\n", sizeof(ScopedEntry_));
}
#endif//$XCC__c

#ifdef $XCC_h
struct Scoped_ {
  //@args	Memory memory, bool copies, int32_t estimate
  StringTable_ STRINGS_;        // Local StringTable.
  //@access STRINGS     Scoped__STRINGS_(this)
  HTable_ HTABLE_;		//!< Maps symbol key to ScopedEntry.
  //@access HTABLE	Scoped__HTABLE_(this)
  //@access MEMORY	HTable_MEMORY(Scoped_HTABLE(this))
  IDList_ IDLIST_;		//!< Where the symbols are inlined.
  //@access IDLIST	Scoped__IDLIST_(this)
  //@access COUNT	IDList_count(Scoped_IDLIST(this))
};
#endif//$XCC_h

Scoped
Scoped_Ctor(Scoped this, Memory memory, bool copies, int32_t estimate)
{
  StringTable_Ctor(Scoped_STRINGS(this), memory, copies);
  HTable_Ctor(Scoped_HTABLE(this), memory, estimate);
  IDList_Ctor(Scoped_IDLIST(this), memory);
  return this;
}

void
Scoped_Dtor(Scoped this) {
  Scoped_empty(this, NULL);
  HTable_Dtor(Scoped_HTABLE(this));
  IDList_Dtor(Scoped_IDLIST(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Scoped_)\t%zu\n", sizeof(Scoped_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Empty this Scoped.
 */
void
Scoped_empty(Scoped this, ScopedValueRelease release);
#endif//$XCC_h

void
Scoped_empty(Scoped this, ScopedValueRelease release)
{
  IDList_empty(Scoped_IDLIST(this), (IDListItemRelease) release);
  HTable_empty(Scoped_HTABLE(this), NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Scoped memory.
 */
static inline Memory
Scoped_memory(const_Scoped this)
{
  return Scoped_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use in Scoped_FOREACH.
 */
static inline const_IDList
Scoped_idlist(const_Scoped this)
{
  return Scoped_IDLIST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count entries in the Scoped.
 * Return:	The count of entries in the Scoped.
 */
static inline int32_t
Scoped_count(const_Scoped this)
{
  return Scoped_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Search the symbol associated to @c key.
 * Return:	The corresponding ScopedEntry if @c key is in Scoped, else NULL.
 */
void *
Scoped_search(const_Scoped this, ScopedKey key, int32_t *_scope);
#endif//$XCC_h

void *
Scoped_search(const_Scoped this, ScopedKey key, int32_t *_scope)
{
  ScopedEntry entry = HTable_search(Scoped_HTABLE(this), key);
  if (entry != NULL) {
    if (_scope != NULL) *_scope = ScopedEntry_SCOPE(entry);
    return ScopedEntry_SYMBOL(entry);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Lookup the symbol at @p key, insert if not already there.
 */
void *
Scoped_lookup(Scoped this, ScopedKey key, size_t symbolSize, int32_t scope);
#endif//$XCC_h

void *
Scoped_lookup(Scoped this, ScopedKey key, size_t symbolSize, int32_t scope)
{
  HTable htable = Scoped_HTABLE(this);
  int32_t count = HTable_count(htable);
  ScopedEntry entry = HTable_lookup(htable, key, sizeof(ScopedEntry_));
  if (count != HTable_count(htable)) {
    void *symbol = IDList_put(Scoped_IDLIST(this), symbolSize);
    ScopedEntry_Ctor(entry, symbol, scope);
    return symbol;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Insert the symbol at @p key if not already there.
 */
void *
Scoped_insert(Scoped this, ScopedKey key, size_t symbolSize, int32_t scope);
#endif//$XCC_h

void *
Scoped_insert(Scoped this, ScopedKey key, size_t symbolSize, int32_t scope)
{
  HTable htable = Scoped_HTABLE(this);
  ScopedEntry entry = HTable_insert(htable, key, sizeof(ScopedEntry_));
  if (entry != NULL) {
    void *symbol = IDList_put(Scoped_IDLIST(this), symbolSize);
    ScopedEntry_Ctor(entry, symbol, scope);
    return symbol;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Insert a symbol even if another has same key but not same scope.
 */
void *
Scoped_install(Scoped this, ScopedKey key, size_t symbolSize, int32_t scope);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Remove @c key and the corresponding symbol.
 * Return:	@c true if @c key was in Scoped, else @c false.
 */
bool
Scoped_remove(Scoped this, ScopedKey key, int32_t scope);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Purge symbols of higher scopes.
 */
void
Scoped_purgeScope(Scoped this);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from first to last Scoped entry.
 */
#define Scoped_FOREACH(this, Type, iter) \
  IDList_FOREACH(Scoped_idlist(this), Type, iter)
#define Scoped_ENDEACH \
  IDList_ENDEACH;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from last to first Scoped entry.
 */
#define Scoped_FORBACK(this, Type, iter) \
  IDList_FORBACK(Scoped_idlist(this), Type, iter)
#define Scoped_ENDBACK \
  IDList_ENDBACK;
#endif//$XCC_h

#ifdef $XCC__c
{
  int i;
  Scoped_ scoped[1];
  StringTable_ table[1];
  typedef struct Symbol_ { String name; } Symbol_, *Symbol;
  const char *names[] = { "alpha", "beta", "gamma", "zeta", "alpha", "beta", NULL };
  Scoped_Ctor(scoped, Memory_Root, true, 10);
  StringTable_Ctor(table, Memory_Root, true);
  for (i = 0; names[i] != NULL; i++) {
    String name = StringTable_S(table, names[i]);
    Symbol symbol = Scoped_insert(scoped, (ScopedKey)name, sizeof(Symbol_), 0);
    if (symbol != NULL) {
      symbol->name = name;
      printf("insert %s:\tOK\n", name);
    } else {
      printf("insert %s:\tNO\n", name);
    }
  }
  printf("scoped:\n");
  Scoped_FOREACH(scoped, Symbol_, symbol) {
    printf("  %s\n", symbol->name);
  } Scoped_ENDEACH;
  Scoped_Dtor(scoped);
  StringTable_Dtor(table);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Scoped module.
 */
#define Scoped_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Scoped module.
 */
#define Scoped_FINI()
#endif//$XCC__h

#if XCC__C
static void
Scoped_TEST(void)
{
#include "CCL/Scoped_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(Scoped_TEST);
  return 0;
}
#endif

