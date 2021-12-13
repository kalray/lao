#ifdef $XCC_h
/*
 * !!!!	IQueue.xcc
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
 * The IQueue implements a double-ended queue of inlined items.
 * storage. This is the traditional double-ended queue ADT.
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup CCL
 * @brief Queue (double ended) of inlined items.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/IQueue.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef void IQueueItem_;
typedef IQueueItem_ *IQueueItem;
typedef void (*IQueueItemRelease)(IQueueItem);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Queue of inlined items in contiguous storage.
 *
 * If @c maxCount is zero, no memory is allocated at construction time.
 */
struct IQueue_ {
  //@args	Memory memory, size_t itemSize, int32_t maxCount
  Memory MEMORY;	// Where the IQueue items are allocated.
  size_t ITEMSIZE;
  int32_t MAXCOUNT;
  //@access WRAPSIZE	((IQueue_MAXCOUNT(this) + 1)*IQueue_ITEMSIZE(this))
  IQueueItem_ *BASE;
  //@access WRAPITEM	((char *)IQueue_BASE(this) + IQueue_WRAPSIZE(this))
  IQueueItem_ *PAST;
  IQueueItem_ *FIRSTITEM;
};
#endif//$XCC_h

IQueue
IQueue_Ctor(IQueue this, Memory memory, size_t itemSize, int32_t maxCount)
{
  *IQueue__MEMORY(this) = memory;
  *IQueue__ITEMSIZE(this) = itemSize;
  Except_REQUIRE(itemSize > 0);
  *IQueue__MAXCOUNT(this) = 0;
  *IQueue__BASE(this) = NULL;
  *IQueue__PAST(this) = NULL;
  *IQueue__FIRSTITEM(this) = NULL;
  if (maxCount > 0) IQueue_resize(this, maxCount);
  return this;
}

void
IQueue_Dtor(IQueue this)
{
  Memory memory = IQueue_MEMORY(this);
  IQueue_empty(this, NULL);
  Memory_free(memory, IQueue_BASE(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IQueue_)\t%zu\n", sizeof(IQueue_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Resize this IQueue.
 * maxCount:	The new @c maxCount.
 * After @c IQueue_resize, IQueue may have moved in memory.
 */
void
IQueue_resize(IQueue this, int32_t maxCount);
#endif//$XCC_h

void
IQueue_resize(IQueue this, int32_t maxCount)
{
  Memory memory = IQueue_MEMORY(this);
  size_t itemSize = IQueue_ITEMSIZE(this);
  int32_t old_maxCount = IQueue_MAXCOUNT(this);
  if (maxCount > old_maxCount) {
    size_t maxSize = (maxCount + 1)*itemSize;
    size_t alignedSize = _ALIGN_NEXT(maxSize);
    char *new_base = Memory_alloc_(memory, alignedSize);
    char *old_base = (char *)IQueue_BASE(this);
    size_t old_usedSize = IQueue_usedSize(this);
    if (old_usedSize > 0) {
      Except_NEVER(true);	// FIXME!
      Memory_free_(memory, old_base);
    }
    *IQueue__FIRSTITEM(this) = new_base;
    *IQueue__PAST(this) = new_base;
    while ((alignedSize -= itemSize) >= maxSize) ++maxCount;
    *IQueue__BASE(this) = new_base;
    *IQueue__MAXCOUNT(this) = maxCount;
  }
  Except_ENSURE((char *)IQueue_PAST(this) >= (char *)IQueue_BASE(this));
  Except_ENSURE((char *)IQueue_PAST(this) < (char *)IQueue_WRAPITEM(this));
  Except_ENSURE((char *)IQueue_FIRSTITEM(this) >= (char *)IQueue_BASE(this));
  Except_ENSURE((char *)IQueue_FIRSTITEM(this) < (char *)IQueue_WRAPITEM(this));
}

#ifdef $XCC_h
/**
 * Empty this IQueue.
 */
void
IQueue_empty(IQueue this, IQueueItemRelease release);
#endif//$XCC_h

void
IQueue_empty(IQueue this, IQueueItemRelease release)
{
  Memory memory = IQueue_MEMORY(this);
  IQueueItem_ *base = IQueue_BASE(this);
  if (base != NULL && release != NULL) {
    size_t itemSize = IQueue_ITEMSIZE(this);
    char *wrapItem = (char *)IQueue_WRAPITEM(this);
    char *firstItem = (char *)IQueue_FIRSTITEM(this);
    char *past = IQueue_PAST(this);
    if (firstItem < past) {
      char *item = firstItem;
      for (; item < past; item += itemSize) {
        (*release)(item);
      }
    } else if (firstItem > past) {
      char *item = firstItem;
      for (; item < wrapItem; item += itemSize) {
        (*release)(item);
      }
      item = base;
      for (; item < past; item += itemSize) {
        (*release)(item);
      }
    }
  }
  *IQueue__PAST(this) = base;
  *IQueue__FIRSTITEM(this) = base;
  Except_CHECK(IQueue_isEmpty(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This IQueue memory.
 */
static inline Memory
IQueue_memory(const_IQueue this)
{
  return IQueue_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Size in bytes of an IQueue item.
 */
static inline size_t
IQueue_itemSize(const_IQueue this)
{
  return IQueue_ITEMSIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IQueue maximum count.
 */
static inline int32_t
IQueue_maxCount(const_IQueue this)
{
  return IQueue_MAXCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IQueue base item.
 */
static inline IQueueItem_ *
IQueue_base(const_IQueue this)
{
  return IQueue_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Needed by IQueue_FOREACH and friends.
 */
static inline size_t
IQueue_wrapSize(const_IQueue this)
{
  return IQueue_WRAPSIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IQueue past item.
 */
static inline IQueueItem_ *
IQueue_past(const_IQueue this)
{
  return IQueue_PAST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IQueue first item.
 */
static inline IQueueItem_ *
IQueue_firstItem(const_IQueue this)
{
  return IQueue_FIRSTITEM(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Size in bytes used by this IQueue items.
 */
static inline size_t
IQueue_usedSize(const_IQueue this)
{
  size_t wrapSize = IQueue_WRAPSIZE(this);
  char *firstItem = (char *)IQueue_FIRSTITEM(this);
  char *past = (char *)IQueue_PAST(this);
  if (past < firstItem) past += wrapSize;
  return (past - firstItem);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this IQueue is empty.
 */
static inline bool
IQueue_isEmpty(const_IQueue this)
{
  char *firstItem = (char *)IQueue_FIRSTITEM(this);
  char *past = (char *)IQueue_PAST(this);
  return past == firstItem;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this IQueue has a single entry.
 */
static inline bool
IQueue_isSingle(const_IQueue this)
{
  size_t wrapSize = IQueue_WRAPSIZE(this);
  size_t itemSize = IQueue_ITEMSIZE(this);
  char *firstItem = (char *)IQueue_FIRSTITEM(this);
  char *past = (char *)IQueue_PAST(this);
  char *lastItem = past - itemSize;
  return firstItem == lastItem ||
      firstItem == lastItem + wrapSize;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this IQueue is full.
 */
static inline bool
IQueue_isFull(const_IQueue this)
{
  size_t wrapSize = IQueue_WRAPSIZE(this);
  size_t itemSize = IQueue_ITEMSIZE(this);
  char *firstItem = (char *)IQueue_FIRSTITEM(this);
  char *past = (char *)IQueue_PAST(this);
  char *prevItem = firstItem - itemSize;
  return past == prevItem ||
      past == prevItem + wrapSize;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count items in the IQueue.
 * Return:	The count of items in the IQueue.
 */
static inline int32_t
IQueue_count(const_IQueue this)
{
  size_t usedSize = IQueue_usedSize(this);
  size_t itemSize = IQueue_ITEMSIZE(this);
  return usedSize/itemSize;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Last item of the IQueue.
 * Return:	Pointer to the last item.
 */
static inline IQueueItem_ *
IQueue_lastItem(const_IQueue this)
{
  size_t wrapSize = IQueue_WRAPSIZE(this);
  size_t itemSize = IQueue_ITEMSIZE(this);
  char *past = (char *)IQueue_PAST(this);
  char *base = (char *)IQueue_BASE(this);
  char *lastItem = past - itemSize;
  Except_REQUIRE(!IQueue_isEmpty(this));
  if (lastItem < base) lastItem += wrapSize;
  return lastItem;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Dereference bottom (first) item of this IQueue.
 */
#define IQueue_BOT(this, Type) *(Type *)IQueue_firstItem(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Dereference top (last) item of this IQueue.
 */
#define IQueue_TOP(this, Type) *(Type *)IQueue_lastItem(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Needed by IQueue_FOREACH and friends.
 */
static inline IQueueItem_ *
IQueue_wrapItem(const_IQueue this)
{
  return IQueue_WRAPITEM(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from first to last IQueue item.
 *
 * Exiting IQueue_FOREACH with @c break or @c return is allowed.
 * this:	The IQueue.
 * @c Type:	Type of the inlined items.
 * iter:	Type* pointer set to each item.
 */
#define IQueue_FOREACH(this, Type, iter) { \
  if (!IQueue_isEmpty(this)) { \
    Type *IQueue_FIRSTITEM = (Type *)IQueue_firstItem(this); \
    Type *IQueue_LASTITEM = (Type *)IQueue_lastItem(this); \
    Type *IQueue_WRAPITEM = (Type *)IQueue_wrapItem(this), *(iter); \
    size_t IQueue_WRAPSIZE = IQueue_wrapSize(this); \
    Except_CHECK(sizeof(Type) == IQueue_itemSize(this)); \
    if (IQueue_LASTITEM < IQueue_FIRSTITEM) \
      IQueue_LASTITEM = (Type *)((char *)IQueue_LASTITEM + IQueue_WRAPSIZE); \
    for (iter = IQueue_FIRSTITEM; iter <= IQueue_LASTITEM; ++(iter)) { \
      if (iter >= IQueue_WRAPITEM) { \
        iter = (Type *)((char *)(iter) - IQueue_WRAPSIZE); \
        IQueue_LASTITEM = (Type *)((char *)(IQueue_LASTITEM) - IQueue_WRAPSIZE); \
      } \
      {
#define IQueue_ENDEACH \
      } \
    } \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from last to first IQueue item.
 *
 * Exiting IQueue_FORBACK with @c break or @c return is allowed.
 * this:	The IQueue.
 * @c Type:	Type of the inlined items.
 * iter:	Type* pointer set to each item.
 */
#define IQueue_FORBACK(this, Type, iter) { \
  if (!IQueue_isEmpty(this)) { \
    Type *IQueue_FIRSTITEM = (Type *)IQueue_firstItem(this); \
    Type *IQueue_LASTITEM = (Type *)IQueue_lastItem(this); \
    Type *BASE = (Type *)IQueue_base(this), *(iter); \
    size_t IQueue_WRAPSIZE = IQueue_wrapSize(this); \
    Except_CHECK(sizeof(Type) == IQueue_itemSize(this)); \
    if (IQueue_LASTITEM < IQueue_FIRSTITEM) \
      IQueue_FIRSTITEM = (Type *)((char *)IQueue_FIRSTITEM - IQueue_WRAPSIZE); \
    for (iter = IQueue_LASTITEM; iter >= IQueue_FIRSTITEM; --(iter)) { \
      if (iter < BASE) { \
        iter = (Type *)((char *)(iter) + IQueue_WRAPSIZE); \
        IQueue_FIRSTITEM = (Type *)((char *)(IQueue_FIRSTITEM) + IQueue_WRAPSIZE); \
      } \
      {
#define IQueue_ENDBACK \
      } \
    } \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Push as last item on the IQueue.
 * @return The pushed IQueueItem.
 */
static inline IQueueItem_ *
IQueue_push(IQueue this)
{
  size_t wrapSize = IQueue_WRAPSIZE(this);
  size_t itemSize = IQueue_ITEMSIZE(this);
  char *past = (char *)IQueue_PAST(this);
  char *wrapItem = (char *)IQueue_WRAPITEM(this);
  IQueueItem_ *newItem = (IQueueItem_ *)past;
  Except_REQUIRE(!IQueue_isFull(this));
  past += itemSize;
  if (past >= wrapItem) past -= wrapSize;
  *IQueue__PAST(this) = past;
  return newItem;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Macro used to IQueue_push value types.
 */
#define IQueue_PUSH(this, Type, value) \
  (*(Type *)IQueue_push(this) = (value))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Put as first item on the IQueue.
 * @return The put IQueueItem.
 */
static inline IQueueItem_ *
IQueue_put(IQueue this)
{
  size_t wrapSize = IQueue_WRAPSIZE(this);
  size_t itemSize = IQueue_ITEMSIZE(this);
  char *firstItem = (char *)IQueue_FIRSTITEM(this);
  char *base = (char *)IQueue_BASE(this);
  Except_REQUIRE(!IQueue_isFull(this));
  firstItem -= itemSize;
  if (firstItem < base) firstItem += wrapSize;
  *IQueue__FIRSTITEM(this) = firstItem;
  return firstItem;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * IQueue_PUT - Macro used to IQueue_put value types.
 */
#define IQueue_PUT(this, Type, value) \
  (*(Type *)IQueue_put(this) = (value))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pop the last IQueue item.
 * Return:	True iff items remain in this IQueue.
 */
static inline void
IQueue_pop(IQueue this, IQueueItemRelease release)
{
  size_t wrapSize = IQueue_WRAPSIZE(this);
  size_t itemSize = IQueue_ITEMSIZE(this);
  char *past = (char *)IQueue_PAST(this);
  char *base = (char *)IQueue_BASE(this);
  Except_REQUIRE(!IQueue_isEmpty(this));
  past -= itemSize;
  if (past < base) past += wrapSize;
  if (release != NULL) {
    (*release)(past);
  }
  *IQueue__PAST(this) = past;
}
#endif//$XCC_h

#ifdef $XCC_h
#define IQueue_pop_(this) IQueue_pop(this, NULL)
#endif//$XCC_h

#ifdef $XCC_h
/*
 * IQueue_POP - Macro used to IQueue_pop value types.
 */
#define IQueue_POP(this, Type, value) \
  ((value) = *(Type *)IQueue_lastItem(this), IQueue_pop_(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Drop the first IQueue item.
 * Return:	True iff items remain in this IQueue.
 */
static inline void
IQueue_drop(IQueue this, IQueueItemRelease release)
{
  size_t wrapSize = IQueue_WRAPSIZE(this);
  size_t itemSize = IQueue_ITEMSIZE(this);
  char *firstItem = (char *)IQueue_FIRSTITEM(this);
  char *wrapItem = (char *)IQueue_WRAPITEM(this);
  Except_REQUIRE(!IQueue_isEmpty(this));
  if (release != NULL) {
    (*release)(firstItem);
  }
  firstItem += itemSize;
  if (firstItem >= wrapItem) firstItem -= wrapSize;
  *IQueue__FIRSTITEM(this) = firstItem;
}
#endif//$XCC_h

#ifdef $XCC_h
#define IQueue_drop_(this) IQueue_drop(this, NULL)
#endif//$XCC_h

#ifdef $XCC_h
/*
 * IQueue_DROP - Macro used to IQueue_drop value types.
 */
#define IQueue_DROP(this, Type, value) \
  ((value) = *(Type *)IQueue_firstItem(this), IQueue_drop_(this))
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
  IQueue_ iqueue[1];
  IQueue_Ctor(iqueue, Memory_Root, sizeof(Pair_), 16);
  printf("push/pop/FOREACH:\n");
  for (i = 0; i < 16; i++) {
    Pair k = IQueue_push(iqueue);
    k->a = i, k->b = j;
    j = (j + 7) & 15;
  }
  while (!IQueue_isEmpty(iqueue)) {
    printf("[\t");
    IQueue_FOREACH(iqueue, Pair_, k) {
      printf("%d,%d\t", k->a, k->b);
    } IQueue_ENDEACH;
    printf("]\n");
    IQueue_pop_(iqueue);
  }
  printf("put/drop/FORBACK:\n");
  for (i = 0; i < 16; i++) {
    Pair k = IQueue_put(iqueue);
    k->a = i, k->b = j;
    j = (j + 7) & 15;
  }
  while (!IQueue_isEmpty(iqueue)) {
    printf("[\t");
    IQueue_FORBACK(iqueue, Pair_, k) {
      printf("%d,%d\t", k->a, k->b);
    } IQueue_ENDBACK;
    printf("]\n");
    IQueue_drop_(iqueue);
  }
  printf("circular test:\n");
  for (i = 0; i < 16 - 1; i++) {
    Pair k1 = IQueue_push(iqueue);
    Pair k2 = IQueue_push(iqueue);
    k1->a = i, k1->b = 2*i;
    k2->a = -i, k2->b = -2*i;
    IQueue_drop_(iqueue);
  }
  printf("[\t");
  IQueue_FOREACH(iqueue, Pair_, k) {
    printf("%d\t", k->a);
  } IQueue_ENDEACH;
  printf("]\n");
  IQueue_empty(iqueue, (IQueueItemRelease)Pair_Dtor);
  IQueue_Dtor(iqueue);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the IQueue module.
 */
#define IQueue_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the IQueue module.
 */
#define IQueue_FINI()
#endif//$XCC__h

#if XCC__C
static void
IQueue_TEST(void)
{
#include "CCL/IQueue_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(IQueue_TEST);
  return 0;
}
#endif

