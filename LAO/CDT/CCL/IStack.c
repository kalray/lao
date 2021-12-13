#ifdef $XCC_h
/*
 * !!!!	IStack.xcc
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
 * The IStack implements a FIFO stack of inlined items over sequential storage.
 * This is the traditional push-down stack ADT.
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup CCL
 * @brief Stack of inlined items.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/IStack.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef void IStackItem_;
typedef IStackItem_ *IStackItem;
typedef void (*IStackItemRelease)(IStackItem);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Stack of inlined items in contiguous storage.
 *
 * If @c maxCount is zero, no memory is allocated at construction time.
 */
struct IStack_ {
  //@args	Memory memory, size_t itemSize, int32_t maxCount
  Memory MEMORY;	// Where the IStack items are allocated.
  size_t ITEMSIZE;
  int32_t MAXCOUNT;
  //@access MAXSIZE	(IStack_MAXCOUNT(this)*IStack_ITEMSIZE(this))
  IStackItem_ *BASE;
  IStackItem_ *PAST;
  //@access USEDSIZE	(size_t)((char*)IStack_PAST(this) - (char*)IStack_BASE(this))
  void *POINTER;
};
#endif//$XCC_h

IStack
IStack_Ctor(IStack this, Memory memory, size_t itemSize, int32_t maxCount)
{
  *IStack__MEMORY(this) = memory;
  *IStack__ITEMSIZE(this) = itemSize;
  Except_REQUIRE(itemSize > 0);
  *IStack__MAXCOUNT(this) = 0;
  *IStack__BASE(this) = NULL;
  *IStack__PAST(this) = NULL;
  *IStack__POINTER(this) = NULL;
  if (maxCount > 0) IStack_resize(this, maxCount);
  return this;
}

void
IStack_Dtor(IStack this)
{
  Memory memory = IStack_MEMORY(this);
  IStack_empty(this, NULL);
  Memory_free(memory, IStack_BASE(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IStack_)\t%zu\n", sizeof(IStack_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Resize this IStack.
 * maxCount:	The new @c maxCount.
 * After @c IStack_resize, IStack may have moved in memory so any
 * pointer obtained though @c IStack_access() will be dangling.
 */
void
IStack_resize(IStack this, int32_t maxCount);
#endif//$XCC_h

void
IStack_resize(IStack this, int32_t maxCount)
{
  Memory memory = IStack_MEMORY(this);
  size_t itemSize = IStack_ITEMSIZE(this);
  int32_t old_maxCount = IStack_MAXCOUNT(this);
  if (maxCount > old_maxCount) {
    size_t size = maxCount*itemSize;
    size_t alignedSize = _ALIGN_NEXT(maxCount*itemSize);
    char *new_base = Memory_alloc_(memory, alignedSize);
    size_t old_usedSize = IStack_USEDSIZE(this);
    if (old_usedSize > 0) {
      char *old_base = (char *)IStack_BASE(this);
      memcpy(new_base, old_base, old_usedSize);
      Memory_free_(memory, old_base);
    }
    *IStack__PAST(this) = new_base + old_usedSize;
    *IStack__BASE(this) = new_base;
    while ((alignedSize -= itemSize) >= size) ++maxCount;
    *IStack__MAXCOUNT(this) = maxCount;
  }
  Except_ENSURE((char *)IStack_PAST(this) >= (char *)IStack_BASE(this));
  Except_ENSURE(IStack_USEDSIZE(this) <= IStack_MAXSIZE(this));
}

#ifdef $XCC_h
/**
 * Empty this IStack.
 */
void
IStack_empty(IStack this, IStackItemRelease release);
#endif//$XCC_h

void
IStack_empty(IStack this, IStackItemRelease release)
{
  IStackItem_ *base = IStack_BASE(this);
  if (base != NULL && release != NULL) {
    size_t itemSize = IStack_ITEMSIZE(this);
    char *past = (char *)IStack_PAST(this);
    char *item = base;
    for (; item < past; item += itemSize) {
      (*release)(item);
    }
  }
  *IStack__PAST(this) = base;
  Except_CHECK(IStack_isEmpty(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This IStack memory.
 */
static inline Memory
IStack_memory(const_IStack this)
{
  return IStack_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IStack item size.
 */
static inline size_t
IStack_itemSize(const_IStack this)
{
  return IStack_ITEMSIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IStack maximum count.
 */
static inline int32_t
IStack_maxCount(const_IStack this)
{
  return IStack_MAXCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IStack base item.
 */
static inline IStackItem_ *
IStack_base(const_IStack this)
{
  return IStack_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IStack past item.
 */
static inline IStackItem_ *
IStack_past(const_IStack this)
{
  return IStack_PAST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Maximum size in bytes used by this IStack items.
 */
static inline size_t
IStack_maxSize(const_IStack this)
{
  return IStack_MAXSIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Size in bytes used by this IStack items.
 */
static inline size_t
IStack_usedSize(const_IStack this)
{
  return IStack_USEDSIZE(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * True iff this IStack is empty.
 */
static inline bool
IStack_isEmpty(const_IStack this)
{
  char *base = (char *)IStack_BASE(this);
  char *past = (char *)IStack_PAST(this);
  return past == base;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this IStack has a single entry.
 */
static inline bool
IStack_isSingle(const_IStack this)
{
  char *base = (char *)IStack_BASE(this);
  char *past = (char *)IStack_PAST(this);
  size_t itemSize = IStack_ITEMSIZE(this);
  return past - base == (int)itemSize;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this IStack is full.
 */
static inline bool
IStack_isFull(const_IStack this)
{
  size_t usedSize = IStack_USEDSIZE(this);
  size_t maxSize = IStack_MAXSIZE(this);
  return usedSize == maxSize;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count items in the IStack.
 * Return:	The count of items in the IStack.
 */
static inline int32_t
IStack_count(const_IStack this)
{
  size_t usedSize = IStack_USEDSIZE(this);
  size_t itemSize = IStack_ITEMSIZE(this);
  return usedSize/itemSize;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * First item of the IStack.
 * Return:	Pointer to the first item.
 */
static inline IStackItem_ *
IStack_firstItem(const_IStack this)
{
  Except_REQUIRE(!IStack_isEmpty(this));
  return (IStackItem)IStack_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Dereference bottom (first) item of this IStack.
 */
#define IStack_BOT(this, Type) *(Type *)IStack_firstItem(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Last item of the IStack.
 * Return:	Pointer to the last item.
 */
static inline IStackItem_ *
IStack_lastItem(const_IStack this)
{
  char *past = (char *)IStack_PAST(this);
  size_t itemSize = IStack_ITEMSIZE(this);
  Except_REQUIRE(!IStack_isEmpty(this));
  return (IStackItem_ *)(past - itemSize);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Dereference top (last) item of this IStack.
 */
#define IStack_TOP(this, Type) *(Type *)IStack_lastItem(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from first to last IStack item.
 *
 * Exiting IStack_FOREACH with @c break or @c return is allowed.
 * this:	The IStack.
 * @c Type:	Type of the inlined items.
 * iter:	Type* pointer set to each item.
 */
#define IStack_FOREACH(this, Type, iter) { \
  Type *IStack_BASE = (Type *)IStack_base(this); \
  Type *IStack_PAST = (Type *)IStack_past(this), *(iter); \
  Except_CHECK(IStack_BASE == NULL || sizeof(Type) == IStack_itemSize(this)); \
  for (iter = IStack_BASE; iter < IStack_PAST; ++(iter)) {
#define IStack_ENDEACH \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from last to first IStack item.
 *
 * Exiting IStack_FORBACK with @c break or @c return is allowed.
 * this:	The IStack.
 * @c Type:	Type of the inlined items.
 * iter:	Type* pointer set to each item.
 */
#define IStack_FORBACK(this, Type, iter) { \
  Type *IStack_BASE = (Type *)IStack_base(this); \
  Type *IStack_PAST = (Type *)IStack_past(this), *(iter); \
  Except_CHECK(IStack_BASE == NULL || sizeof(Type) == IStack_itemSize(this)); \
  for (iter = IStack_PAST - 1; iter >= IStack_BASE; --(iter)) {
#define IStack_ENDBACK \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Access the IStack by index.
 * index:	The index.
 * Return:	Pointer to the corresponding item.
 */
static inline IStackItem
IStack_access(const_IStack this, int32_t index)
{
  char *base = (char *)IStack_BASE(this);
  size_t itemSize = IStack_ITEMSIZE(this);
  size_t offset = itemSize*index;
  Except_REQUIRE(index >= 0 && offset < IStack_USEDSIZE(this));
  return base + offset;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Push as last item on the IStack.
 * Return:	The pushed IStackItem, or NULL if stack storage is exhausted.
 */
static inline IStackItem
IStack_push(IStack this)
{
  size_t itemSize = IStack_ITEMSIZE(this);
  char *past = (char *)IStack_PAST(this);
  if (IStack_isFull(this)) return NULL;
  *IStack__PAST(this) = past + itemSize;
  return past;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * IStack_PUSH - Macro used to IStack_push value types.
 */
#define IStack_PUSH(this, Type, value) \
  (*(Type *)IStack_push(this) = (value))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Push as last item on the IStack, resizing the IStack if necessary.
 * Return:	The pushed IStackItem.
 * As the IStack may be resized, it is an error to maintain pointers to existing items.
 */
IStackItem
IStack_push2(IStack this);
#endif//$XCC_h

#ifndef IStack_MAGICCOUNT
#define IStack_MAGICCOUNT 16
#endif//IStack_MAGICCOUNT

IStackItem
IStack_push2(IStack this)
{
  size_t itemSize = IStack_ITEMSIZE(this);
  char *past = (char *)IStack_PAST(this);
  if (IStack_isFull(this)) {
    int32_t newCount = IStack_count(this)*2;
    if (newCount < IStack_MAGICCOUNT) {
      newCount = IStack_MAGICCOUNT;
    }
    IStack_resize(this, newCount);
    past = IStack_PAST(this);
    Except_CHECK(!IStack_isFull(this));
  }
  *IStack__PAST(this) = past + itemSize;
  return past;
}

#ifdef $XCC_h
/*
 * IStack_PUSH2 - Macro used to IStack_push2 value types.
 */
#define IStack_PUSH2(this, Type, value) \
  (*(Type *)IStack_push2(this) = (value))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Specialized @c IStack_pop
 */
static inline void
IStack_pop_(IStack this)
{
  size_t itemSize = IStack_ITEMSIZE(this);
  char *past = (char *)IStack_PAST(this);
  Except_CHECK(!IStack_isEmpty(this));
  *IStack__PAST(this) = (past -= itemSize);
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * IStack_POP - Macro used to IStack_pop value types.
 */
#define IStack_POP(this, Type, value) \
  ((value) = *(Type *)IStack_lastItem(this), IStack_pop_(this))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pop the last IStack item.
 * Return:	True iff items remain in this IStack.
 */
static inline void
IStack_pop(IStack this, IStackItemRelease release)
{
  IStackItem lastItem = IStack_lastItem(this);
  Except_REQUIRE(!IStack_isEmpty(this));
  if (release != NULL) {
    (*release)(lastItem);
  }
  *IStack__PAST(this) = lastItem;
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int16_t *array = 0;
  int count = 20, resized = 0, i;
  IStack_ istack[1];
  IStack_Ctor(istack, Memory_Root, sizeof(int16_t), count/5);
  for (i = 0 ; i < count; i++) {
    int16_t *item = IStack_push(istack);
    if (item) *item = 'a' + resized*('A' - 'a') + i;
    else {
      IStack_resize(istack, i+3);
      resized = resized ^ 0x1;
      --i;
    }
  }
  printf("Stack count = %d\n", count);
  i = 0;
  array = IStack_base(istack);
  IStack_FOREACH(istack, int16_t, iter) {
    if (iter != IStack_access(istack, i)) printf("error1 at %d\n", i);
    if (array + i != iter) printf("error2 at %d\n", i);
    printf("%c ", *iter);
    ++i;
  } IStack_ENDEACH;
  printf("\n");
  i = IStack_count(istack);
  array = IStack_base(istack);
  IStack_FORBACK(istack, int16_t, iter) {
    --i;
    if (iter != IStack_access(istack, i)) printf("error1 at %d\n", i);
    if (array + i != iter) printf("error2 at %d\n", i);
    printf("%c ", *iter);
  } IStack_ENDBACK;
  printf("\n");
  IStack_Dtor(istack);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the IStack module.
 */
#define IStack_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the IStack module.
 */
#define IStack_FINI()
#endif//$XCC__h

#if XCC__C
static void
IStack_TEST(void)
{
#include "CCL/IStack_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(IStack_TEST);
  return 0;
}
#endif

