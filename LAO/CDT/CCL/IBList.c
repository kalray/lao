#ifdef $XCC_h
/*
 * !!!!	IBList.xcc
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
 * The IBList implements a blocked list of same-sized items.
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup CCL
 * @brief List (blocked) of inlined items.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/IBList.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef void *IBListBlock;
typedef void *IBListItem;
typedef const void *const_IBListItem;
typedef void (*IBListItemRelease)(IBListItem);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * List of inlined items in blocked storage.
 */
struct IBList_ {
  //@args	Memory memory, size_t itemSize, int32_t blocking
  size_t BLOCKING;
  size_t ITEMSIZE;
  IBListItem FIRSTITEM;
  IBListItem PAST;
  IDList_ BLOCKLIST_;
  //@access BLOCKLIST	IBList__BLOCKLIST_(this)
  //@access FIRSTBLOCK  (IBListBlock)IDList_firstItem(IBList_BLOCKLIST(this))
  //@access LASTBLOCK  (IBListBlock)IDList_lastItem(IBList_BLOCKLIST(this))
  //@access MEMORY	IDList_memory(IBList_BLOCKLIST(this))
};
#endif//$XCC_h

IBList
IBList_Ctor(IBList this, Memory memory, size_t itemSize, int32_t blocking)
{
  *IBList__BLOCKING(this) = blocking;
  Except_REQUIRE(blocking > 0);
  *IBList__ITEMSIZE(this) = itemSize;
  Except_REQUIRE(itemSize > 0);
  *IBList__FIRSTITEM(this) = NULL;
  *IBList__PAST(this) = NULL;
  IDList_Ctor(IBList_BLOCKLIST(this), memory);
  return this;
}

void
IBList_Dtor(IBList this)
{
  IBList_empty(this, NULL);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IBList_)\t%zu\n", sizeof(IBList_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Empty this IBList.
 */
void
IBList_empty(IBList this, IBListItemRelease release);
#endif//$XCC_h

void
IBList_empty(IBList this, IBListItemRelease release)
{
  Memory memory = IBList_MEMORY(this);
  if (release != NULL) {
    IBListBlock IBList_FIRSTBLOCK = IBList_firstBlock(this);
    IBListBlock IBList_PAST = IBList_past(this);
    int32_t IBList_BLOCKING = IBList_blocking(this);
    size_t IBList_ITEMSIZE = IBList_itemSize(this);
    while (IBList_FIRSTBLOCK != NULL) {
      IBListBlock IBList_NEXTBLOCK = IDListItem_NEXT(IBList_FIRSTBLOCK);
      char *IBList_BASE = (char *)IBList_FIRSTBLOCK;
      char *IBList_OVERITEM = IBList_NEXTBLOCK != NULL?
                              IBList_BASE + IBList_BLOCKING*IBList_ITEMSIZE:
                              (char *)IBList_PAST;
      char *IBList_ITERITEM = IBList_BASE;
      for (; IBList_ITERITEM < IBList_OVERITEM; IBList_ITERITEM += IBList_ITEMSIZE) {
        (*release)((IBListItem)IBList_ITERITEM);
      }
      if (IBList_ITERITEM < IBList_OVERITEM) break;
      IBList_FIRSTBLOCK = IBList_NEXTBLOCK;
    }
  }
  IDList_empty(IBList_BLOCKLIST(this), NULL);
  *IBList__FIRSTITEM(this) = NULL;
  *IBList__PAST(this) = NULL;
  Except_CHECK(IBList_isEmpty(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This IBList memory.
 */
static inline Memory
IBList_memory(const_IBList this)
{
  return IBList_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IBList block factor.
 */
static inline size_t
IBList_blocking(const_IBList this)
{
  return IBList_BLOCKING(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Size in bytes of an IBList item.
 */
static inline size_t
IBList_itemSize(const_IBList this)
{
  return IBList_ITEMSIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IBList first item.
 */
static inline IBListItem
IBList_firstItem(const_IBList this)
{
  return IBList_FIRSTITEM(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IBList past item.
 */
static inline IBListItem
IBList_past(const_IBList this)
{
  return IBList_PAST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IBList first IBListBlock.
 */
static inline IBListBlock
IBList_firstBlock(const_IBList this)
{
  return IBList_FIRSTBLOCK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IBList last IBListBlock.
 */
static inline IBListBlock
IBList_lastBlock(const_IBList this)
{
  return IBList_LASTBLOCK(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this IBList is empty.
 */
static inline bool
IBList_isEmpty(const_IBList this)
{
  char *firstItem = (char *)IBList_FIRSTITEM(this);
  char *past = (char *)IBList_PAST(this);
  return past == firstItem;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this IBList has a single entry.
 */
static inline bool
IBList_isSingle(const_IBList this)
{
  size_t itemSize = IBList_ITEMSIZE(this);
  char *firstItem = (char *)IBList_FIRSTITEM(this);
  char *past = (char *)IBList_PAST(this);
  char *lastItem = past - itemSize;
  return firstItem == lastItem;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count items in the IBList.
 * Return:	The count of items in the IBList.
 */
static inline int32_t
IBList_count(const_IBList this)
{
  size_t itemSize = IBList_ITEMSIZE(this);
  int32_t blocking = IBList_BLOCKING(this);
  const_IDList blockList = IBList_BLOCKLIST(this);
  int32_t count = (IDList_count(blockList) - 1)*blocking;
  char *lastBlock = (char *)IBList_LASTBLOCK(this);
  char *firstItem = (char *)IBList_FIRSTITEM(this);
  char *past = (char *)IBList_PAST(this);
  if (past == firstItem) return 0;
  return count + (past - lastBlock)/itemSize;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Last item of the IBList.
 * Return:	Pointer to the last item.
 */
static inline IBListItem
IBList_lastItem(const_IBList this)
{
  size_t itemSize = IBList_ITEMSIZE(this);
  char *past = (char *)IBList_PAST(this);
  char *lastItem = past - itemSize;
  Except_REQUIRE(!IBList_isEmpty(this));
  return lastItem;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Dereference bottom (first) item of this IBList.
 */
#define IBList_BOT(this, Type) *(Type *)IBList_firstItem(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Dereference top (last) item of this IBList.
 */
#define IBList_TOP(this, Type) *(Type *)IBList_lastItem(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from first to last IBList item.
 *
 * Exiting IBList_FOREACH with \c break or \c return is allowed.
 * @param this is the \c IBList to iterate on.
 * @param Type is the type of the inlined items.
 * @param iter is a \p Type* pointer that is set to each item of the list.
 */
#define IBList_FOREACH(this, Type, iter) { \
  IBListBlock IBList_FIRSTBLOCK = IBList_firstBlock(this); \
  IBListBlock IBList_PAST = IBList_past(this); \
  int32_t IBList_BLOCKING = IBList_blocking(this); \
  Except_CHECK(sizeof(Type) == IBList_itemSize(this)); \
  while (IBList_FIRSTBLOCK != NULL) { \
    IBListBlock IBList_NEXTBLOCK = IDListItem_NEXT(IBList_FIRSTBLOCK); \
    Type *IBList_BASE = (Type *)IBList_FIRSTBLOCK; \
    Type *IBList_OVERITEM = IBList_NEXTBLOCK != NULL? \
                            IBList_BASE + IBList_BLOCKING: \
                            (Type *)IBList_PAST; \
    Type *IBList_ITERITEM = IBList_BASE; \
    for (; IBList_ITERITEM < IBList_OVERITEM; IBList_ITERITEM++) { \
      Type *(iter) = IBList_ITERITEM;
#define IBList_ENDEACH \
    } \
    if (IBList_ITERITEM < IBList_OVERITEM) break; \
    IBList_FIRSTBLOCK = IBList_NEXTBLOCK; \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
bool
IBList_debug(IBList this);
#endif//$XCC_h

bool
IBList_debug(IBList this)
{
  int blockCount = 0, itemCount = 0;
  int blocking = IBList_blocking(this);
  size_t itemSize = IBList_itemSize(this);
  IDList blockList = IBList_BLOCKLIST(this);
  IDList_FOREACH(blockList, void, block) {
    ++blockCount;
    if (IDList_NEXT == NULL) {
      itemCount += ((char *)IBList_past(this) - (char *)block)/itemSize;
    } else {
      itemCount += blocking;
    }
  } IDList_ENDEACH;
  Except_ALWAYS(blockCount == IDList_count(blockList));
  Except_ALWAYS(itemCount == IBList_count(this));
  return true;
}

#ifdef $XCC_h
/**
 * Push as last item on the IBList.
 * Return:	The pushed IBListItem.
 */
static inline IBListItem
IBList_push(IBList this)
{
  size_t itemSize = IBList_ITEMSIZE(this);
  int32_t blocking = IBList_BLOCKING(this);
  size_t blockSize = blocking*itemSize;
  char *past = (char *)IBList_PAST(this);
  IDList blockList = IBList_BLOCKLIST(this);
  char *lastBlock = (char *)IBList_LASTBLOCK(this);
  if (lastBlock == NULL) {
    past = (char *)IDList_push(blockList, blockSize);
    *IBList__FIRSTITEM(this) = past;
  } else if (past >= lastBlock + blockSize) {
    past = (char *)IDList_push(blockList, blockSize);
  }
  *IBList__PAST(this) = past + itemSize;
  return past;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * IBList_PUSH - Macro used to IBList_push value types.
 */
#define IBList_PUSH(this, Type, value) \
  (*(Type *)IBList_push(this) = (value))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Put as first item on the IBList.
 * Return:	The put IBListItem.
 */
IBListItem
IBList_put(IBList this);
#endif//$XCC_h

#ifdef $XCC_h
/*
 * IBList_PUT - Macro used to IBList_put value types.
 */
#define IBList_PUT(this, Type, value) \
  (*(Type *)IBList_put(this) = (value))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pop the last IBList item.
 * Return:	True iff items remain in this IBList.
 */
bool
IBList_pop(IBList this, IBListItemRelease release);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Drop the first IBList item.
 * Return:	True iff items remain in this IBList.
 */
bool
IBList_drop(IBList this, IBListItemRelease release);
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#if XCC__C
typedef struct Pair_ { int a, b; int dummy; } Pair_, *Pair;
static void Pair_Dtor(Pair this)
{
  printf("Pair_Dtor(%d,%d)\n", this->a, this->b);
}
#endif//XCC__C

#ifdef $XCC__c
{
  int i, j = 0;
  IBList_ iblist[1];
  IBList_Ctor(iblist, Memory_Root, sizeof(Pair_), 6);
  printf("push/pop/FOREACH:");
  printf("\t(%d)", IBList_count(iblist));
  for (i = 0; i < 16; i++) {
    Pair k = IBList_push(iblist);
    k->a = i, k->b = j;
    j = (j + 7) & 15;
    printf("\t(%d)", IBList_count(iblist));
    IBList_debug(iblist);
  }
  printf("\n");
#if 0
  while (!IBList_isEmpty(iblist)) {
    printf("[\t");
    IBList_FOREACH(iblist, Pair_, k) {
      printf("%d,%d\t", k->a, k->b);
    } IBList_ENDEACH;
    printf("]\n");
    IBList_pop(iblist);
  }
  printf("put/drop/FORBACK:\n");
  for (i = 0; i < 16; i++) {
    Pair k = IBList_put(iblist);
    k->a = i, k->b = j;
    j = (j + 7) & 15;
  }
  while (!IBList_isEmpty(iblist)) {
    printf("[\t");
    IBList_FORBACK(iblist, Pair_, k) {
      printf("%d,%d\t", k->a, k->b);
    } IBList_ENDBACK;
    printf("]\n");
    IBList_drop(iblist);
  }
  printf("circular test:\n");
  for (i = 0; i < 16 - 1; i++) {
    Pair k1 = IBList_push(iblist);
    Pair k2 = IBList_push(iblist);
    k1->a = i, k1->b = 2*i;
    k2->a = -i, k2->b = -2*i;
    IBList_drop(iblist);
  }
#endif
  printf("[\t");
  IBList_FOREACH(iblist, Pair_, k) {
    printf("%d\t", k->a);
  } IBList_ENDEACH;
  printf("]\n");
  IBList_empty(iblist, (IBListItemRelease)Pair_Dtor);
  IBList_Dtor(iblist);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the IBList module.
 */
#define IBList_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the IBList module.
 */
#define IBList_FINI()
#endif//$XCC__h

#if XCC__C
static void
IBList_TEST(void)
{
#include "CCL/IBList_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(IBList_TEST);
  return 0;
}
#endif

