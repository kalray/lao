#ifdef $XCC_h
/*
 * !!!!	IArray.xcc
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
 * The IArray implements an array of inlined items. The main use of IArray is to
 * provide sequential storage initialized to zero. IArray items can be
 * reallocated using @c IArray_resize().
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup CCL
 * @brief Array of inlined items.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/IArray.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef void *IArrayItem;
typedef const void *const_IArrayItem;
typedef void (*IArrayItemRelease)(IArrayItem);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Array of inlined items, initialized to zero.
 *
 * If @c maxCount is zero, no memory is allocated at construction time.
 */
struct IArray_ {
  //@args	Memory memory, size_t itemSize, int32_t maxCount
  Memory MEMORY;	// Where the IArray items are allocated.
  size_t ITEMSIZE;
  int32_t MAXCOUNT;
  //@access MAXSIZE	(IArray_MAXCOUNT(this)*IArray_ITEMSIZE(this))
  IArrayItem BASE;
  //@access PAST	(IArrayItem)((char *)IArray_BASE(this)+IArray_MAXSIZE(this))
};
#endif//$XCC_h

IArray
IArray_Ctor(IArray this, Memory memory, size_t itemSize, int32_t maxCount)
{
  *IArray__MEMORY(this) = memory;
  *IArray__ITEMSIZE(this) = itemSize;
  Except_REQUIRE(itemSize > 0);
  *IArray__MAXCOUNT(this) = 0;
  *IArray__BASE(this) = NULL;
  if (maxCount > 0) IArray_resize(this, maxCount);
  return this;
}

void
IArray_Dtor(IArray this)
{
  Memory memory = IArray_MEMORY(this);
  Memory_free(memory, IArray_BASE(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IArray_)\t%zu\n", sizeof(IArray_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Resize the IArray.
 *
 * After @c IArray_resize, IArray may have moved in memory. in this case any
 * pointer obtained though @c IArray_access() will be dangling. The extra storage
 * obtained by resizing is initialized to zero.
 * maxCount:	The new @c maxCount.
 */
void
IArray_resize(IArray this, int32_t maxCount);
#endif//$XCC_h

void
IArray_resize(IArray this, int32_t maxCount)
{
  Memory memory = IArray_MEMORY(this);
  size_t itemSize = IArray_ITEMSIZE(this);
  int32_t old_maxCount = IArray_MAXCOUNT(this);
  if (maxCount > old_maxCount) {
    size_t maxSize = maxCount*itemSize;
    size_t alignedSize = _ALIGN_NEXT(maxSize);
    char *new_base = Memory_alloc_(memory, alignedSize);
    char *old_base = IArray_BASE(this);
    size_t old_maxSize = IArray_MAXSIZE(this);
    if (old_maxSize > 0) {
      memcpy(new_base, old_base, old_maxSize);
      Memory_free_(memory, old_base);
    }
    memset(new_base + old_maxSize, 0, alignedSize - old_maxSize);
    while ((alignedSize -= itemSize) >= maxSize) ++maxCount;
    *IArray__BASE(this) = new_base;
    *IArray__MAXCOUNT(this) = maxCount;
  }
}

#ifdef $XCC_h
/**
 * Empty this IArray
 */
void
IArray_empty(IArray this, IArrayItemRelease release);
#endif//$XCC_h

void
IArray_empty(IArray this, IArrayItemRelease release)
{
  char *base = IArray_BASE(this);
  size_t itemSize = IArray_ITEMSIZE(this);
  int32_t maxCount = IArray_MAXCOUNT(this), i;
  if (base != NULL) {
    if (release != NULL) {
      for (i = 0; i < maxCount; i++) {
        (*release)(base + i*itemSize);
      }
    }
    memset(base, 0, IArray_MAXSIZE(this));
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This IArray memory.
 */
static inline Memory
IArray_memory(const_IArray this)
{
  return IArray_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IArray item size.
 */
static inline size_t
IArray_itemSize(const_IArray this)
{
  return IArray_ITEMSIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IArray maximum count.
 */
static inline int32_t
IArray_maxCount(const_IArray this)
{
  return IArray_MAXCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IArray base item.
 */
static inline IArrayItem
IArray_base(const_IArray this)
{
  return IArray_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IArray past item.
 */
static inline IArrayItem
IArray_past(const_IArray this)
{
  return IArray_PAST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Maximum size in bytes used by this IArray items.
 */
static inline size_t
IArray_maxSize(const_IArray this)
{
  return IArray_MAXSIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This IArray maximum count.
 */
static inline int32_t
IArray_count(const_IArray this)
{
  return IArray_MAXCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * First item of the IArray.
 * Return:	Pointer to the first item.
 */
static inline IArrayItem
IArray_firstItem(const_IArray this)
{
  return IArray_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Last item of the IArray.
 * Return:	Pointer to the last item.
 */
static inline IArrayItem
IArray_lastItem(const_IArray this)
{
  char *past = (char *)IArray_PAST(this);
  size_t itemSize = IArray_ITEMSIZE(this);
  return (IArrayItem)(past - itemSize);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from first to last IArray item.
 *
 * Exiting IArray_FOREACH with @c break or @c return is allowed.
 * this:	The IArray.
 * @c Type:	Type of the inlined items.
 * iter:	Type* pointer set to each item.
 */
#define IArray_FOREACH(this, Type, iter) { \
  Type *IArray_BASE = (Type *)IArray_base(this); \
  Type *IArray_PAST = (Type *)IArray_past(this), *(iter); \
  Except_CHECK(IArray_BASE == NULL || sizeof(Type) == IArray_itemSize(this)); \
  for (iter = IArray_BASE; iter < IArray_PAST; ++(iter)) {
#define IArray_ENDEACH \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from last to first IArray item.
 *
 * Exiting IArray_FORBACK with @c break or @c return is allowed.
 * this:	The IArray.
 * @c Type:	Type of the inlined items.
 * iter:	Type* pointer set to each item.
 */
#define IArray_FORBACK(this, Type, iter) { \
  Type *IArray_BASE = (Type *)IArray_base(this); \
  Type *IArray_PAST = (Type *)IArray_past(this), *(iter); \
  Except_CHECK(IArray_BASE == NULL || sizeof(Type) == IArray_itemSize(this)); \
  for (iter = IArray_PAST - 1; iter >= IArray_BASE; --(iter)) {
#define IArray_ENDBACK \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access the IArray by index.
 * index:	The index.
 * Return:	Pointer to the corresponding item.
 */
static inline IArrayItem
IArray_access(const_IArray this, int32_t index)
{
  char *base = (char *)IArray_BASE(this);
  size_t itemSize = IArray_ITEMSIZE(this);
  Except_REQUIRE(index >= 0 && index*itemSize < IArray_maxSize(this));
  return base + itemSize*index;
}
#endif//$XCC_h

#ifdef $XCC__c
if(0){ // 32/64 bits dependent
  int count, i;
  int16_t *array = 0;
  IArray_ iarray[1];
  IArray_Ctor(iarray, Memory_Root, sizeof(int16_t), 5);
  array = IArray_access(iarray, 0);
  count = IArray_maxCount(iarray);
  printf("Array count = %d\n", count);
  for (i = 0 ; i < count; i++) {
    if (array[i] != 0) printf("non-zero at %d\n", i);
    array[i] = 'a' + i;
  }
  IArray_resize(iarray, 20);
  array = IArray_access(iarray, 0);
  count = IArray_maxCount(iarray);
  for (; i < count; i++) {
    if (array[i] != 0) printf("non-zero at %d\n", i);
    array[i] = 'A' + i;
  }
  printf("Array count = %d\n", count);
  i = 0;
  array = IArray_access(iarray, 0);
  IArray_FOREACH(iarray, int16_t, iter) {
    if (iter != IArray_access(iarray, i)) printf("error1 at %d\n", i);
    if (array + i != iter) printf("error2 at %d\n", i);
    printf("%c ", *iter);
    ++i;
  } IArray_ENDEACH;
  printf("\n");
  i = IArray_maxCount(iarray);
  array = IArray_access(iarray, 0);
  IArray_FORBACK(iarray, int16_t, iter) {
    --i;
    if (iter != IArray_access(iarray, i)) printf("error1 at %d\n", i);
    if (array + i != iter) printf("error2 at %d\n", i);
    printf("%c ", *iter);
  } IArray_ENDBACK;
  printf("\n");
  IArray_Dtor(iarray);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the IArray module.
 */
#define IArray_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the IArray module.
 */
#define IArray_FINI()
#endif//$XCC__h

#if XCC__C
static void
IArray_TEST(void)
{
#include "CCL/IArray_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(IArray_TEST);
  return 0;
}
#endif

