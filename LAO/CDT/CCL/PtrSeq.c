#ifdef $XCC_h
/*
 * !!!!	PtrSeq.xcc
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
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup CCL
 * Pointer sequence implemented as stack.
 *
 * The @c PtrSeq implements a LIFO stack of pointers over sequential storage.
 * A @c PtrSeq has an initial maximum number of elements @c maxCount, but can
 * be resized explicitely using @c PtrSeq_resize or implicitely with @c
 * PtrSeq_push2.
 *
 * Typical usage is to push/pop elements using @c PtrSeq_push and @c
 * PtrSet_pop.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/PtrSeq.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * One element of a @c PtrSeq.
 */
typedef void *PtrSeqItem;
/**
 * One constant element of a @c PtrSeq.
 */
typedef const void *const_PtrSeqItem;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Sequence of pointers implemented as stack.
 *
 * If @c maxCount is zero, no memory is allocated at construction time.
 */
struct PtrSeq_ {
  //@args	Memory memory, int32_t maxCount
  Memory MEMORY;	//!< Where the PtrSeq items are allocated.
  //@access ITEMSIZE	sizeof(PtrSeqItem)
  uint32_t STATUS;	//!< 31 bits for MAXCOUNT1; 1 bit for ISORDERED flag;
  //@access ISORDERED	(PtrSeq_STATUS(this)&0x1)
  //@access MAXCOUNT	(int32_t)(PtrSeq_STATUS(this)>>1)
  //@access MAXSIZE	(PtrSeq_MAXCOUNT(this)*sizeof(PtrSeqItem))
  PtrSeqItem *BASE;     //!< Bottom of the @c PtrSeq stack.
  PtrSeqItem *PAST;     //!< Top of the @c PtrSeq stack.
  //@access USEDSIZE	((char*)PtrSeq_PAST(this) - (char*)PtrSeq_BASE(this))
};
#endif//$XCC_h

PtrSeq
PtrSeq_Ctor(PtrSeq this, Memory memory, int32_t maxCount)
{
  *PtrSeq__MEMORY(this) = memory;
  *PtrSeq__STATUS(this) = 0;
  *PtrSeq__BASE(this) = NULL;
  *PtrSeq__PAST(this) = NULL;
  if (maxCount > 0) PtrSeq_resize(this, maxCount);
  return this;
}

void
PtrSeq_Dtor(PtrSeq this)
{
  Memory memory = PtrSeq_MEMORY(this);
  PtrSeq_empty(this);
  Memory_free(memory, PtrSeq_BASE(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(PtrSeq_)\t%zu\n", sizeof(PtrSeq_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Resize this PtrSeq.
 *
 * maxCount:	The new @c maxCount.
 * After @c PtrSeq_resize, PtrSeq may have moved in memory. in this case any
 * pointer obtained though @c PtrSeq_access() will be dangling.
 */
void
PtrSeq_resize(PtrSeq this, int32_t maxCount);
#endif//$XCC_h

void
PtrSeq_resize(PtrSeq this, int32_t maxCount)
{
  Memory memory = PtrSeq_MEMORY(this);
  size_t itemSize = sizeof(PtrSeqItem);
  bool old_isOrdered = PtrSeq_ISORDERED(this);
  int32_t old_maxCount = PtrSeq_MAXCOUNT(this);
  if (maxCount > old_maxCount) {
    size_t maxSize = maxCount*itemSize;
    size_t alignedSize = _ALIGN_NEXT(maxSize);
    char *new_base = Memory_alloc_(memory, alignedSize);
    char *old_base = (char *)PtrSeq_BASE(this);
    size_t old_usedSize = PtrSeq_USEDSIZE(this);
    if (old_usedSize != 0) {
      memcpy(new_base, old_base, old_usedSize);
      Memory_free_(memory, old_base);
    } else {
      Memory_free(memory, old_base);
    }
    *PtrSeq__PAST(this) = (PtrSeqItem *)(new_base + old_usedSize);
    *PtrSeq__BASE(this) = (PtrSeqItem *)(new_base);
    while ((alignedSize -= itemSize) >= maxSize) ++maxCount;
    *PtrSeq__STATUS(this) = ((maxCount<<1)|old_isOrdered);
  }
  Except_ENSURE(PtrSeq_PAST(this) >= PtrSeq_BASE(this));
  Except_ENSURE(PtrSeq_USEDSIZE(this) <= PtrSeq_MAXSIZE(this));
}

#ifdef $XCC_h
/**
 * Empty this PtrSeq.
 */
void
PtrSeq_empty(PtrSeq this);
#endif//$XCC_h

void
PtrSeq_empty(PtrSeq this)
{
  PtrSeqItem base = PtrSeq_BASE(this);
  *PtrSeq__PAST(this) = base;
  Except_CHECK(PtrSeq_isEmpty(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This PtrSeq memory.
 */
static inline Memory
PtrSeq_memory(const_PtrSeq this)
{
  return PtrSeq_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This PtrSeq maximum count.
 */
static inline int32_t
PtrSeq_maxCount(const_PtrSeq this)
{
  return PtrSeq_MAXCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This PtrSeq base item.
 */
static inline const PtrSeqItem *
PtrSeq_base(const_PtrSeq this)
{
  return PtrSeq_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This PtrSeq past item.
 */
static inline const PtrSeqItem *
PtrSeq_past(const_PtrSeq this)
{
  return PtrSeq_PAST(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from first to last PtrSeq item.
 *
 * Exiting PtrSeq_FOREACH with @c break or @c return is allowed.
 * this:	The PtrSeq.
 * item:	Type item.
 */
#define PtrSeq_FOREACH(this, Type, item) { \
  const PtrSeqItem *PtrSeq_BASE = PtrSeq_base(this); \
  const PtrSeqItem *PtrSeq_PAST = PtrSeq_past(this); \
  const PtrSeqItem *PtrSeq_ITER = PtrSeq_BASE; \
  for (; PtrSeq_ITER < PtrSeq_PAST; PtrSeq_ITER++) { \
    Type (item) = (Type)*PtrSeq_ITER;
#define PtrSeq_ENDEACH \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates from last to first PtrSeq item.
 *
 * Exiting PtrSeq_FORBACK with @c break or @c return is allowed.
 * this:	The PtrSeq.
 * item:	Type item.
 */
#define PtrSeq_FORBACK(this, Type, item) { \
  const PtrSeqItem *PtrSeq_BASE = PtrSeq_base(this); \
  const PtrSeqItem *PtrSeq_PAST = PtrSeq_past(this); \
  const PtrSeqItem *PtrSeq_ITER = PtrSeq_PAST - 1; \
  for (; PtrSeq_ITER >= PtrSeq_BASE; --PtrSeq_ITER) { \
    Type (item) = (Type)*PtrSeq_ITER;
#define PtrSeq_ENDBACK \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Maximum size in bytes used by this PtrSeq items.
 */
static inline size_t
PtrSeq_maxSize(const_PtrSeq this)
{
  return PtrSeq_MAXSIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Size in bytes used by this PtrSeq items.
 */
static inline size_t
PtrSeq_usedSize(const_PtrSeq this)
{
  return PtrSeq_USEDSIZE(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * True iff this PtrSeq is empty.
 */
static inline bool
PtrSeq_isEmpty(const_PtrSeq this)
{
  PtrSeqItem *base = PtrSeq_BASE(this);
  PtrSeqItem *past = PtrSeq_PAST(this);
  return past == base;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this PtrSeq has a single entry.
 */
static inline bool
PtrSeq_isSingle(const_PtrSeq this)
{
  PtrSeqItem *base = PtrSeq_BASE(this);
  PtrSeqItem *past = PtrSeq_PAST(this);
  return past == base + 1;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this PtrSeq is full.
 */
static inline bool
PtrSeq_isFull(const_PtrSeq this)
{
  PtrSeqItem *base = PtrSeq_BASE(this);
  PtrSeqItem *past = PtrSeq_PAST(this);
  int32_t maxCount = PtrSeq_MAXCOUNT(this);
  return past == base + maxCount;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count items in the PtrSeq.
 * @return The count of items in the PtrSeq.
 */
static inline int32_t
PtrSeq_count(const_PtrSeq this)
{
  PtrSeqItem *base = PtrSeq_BASE(this);
  PtrSeqItem *past = PtrSeq_PAST(this);
  return past - base;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Look for an element in the PtrSeq
 * @return The index of the element in the PtrSeq if found. -1 else.
 */
static inline int32_t
PtrSeq_contains(const_PtrSeq this, PtrSeqItem item)
{
  int32_t index=0;
  PtrSeq_FOREACH(this, PtrSeqItem, iter) {
    if(item == iter) return index;
    index++;
  } PtrSeq_ENDEACH;
  return -1;
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * This PtrSeq ordered status.
 */
static inline bool
PtrSeq_isOrdered(const_PtrSeq this)
{
  return PtrSeq_ISORDERED(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this PtrSeq status as unordered.
 */
static inline void
PtrSeq_clearOrdered(PtrSeq this)
{
  *PtrSeq__STATUS(this) &= ~0x1;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Base of this PtrSeq for use by external sort.
 */
static inline PtrSeqItem *
PtrSeq_base_(PtrSeq this)
{
  *PtrSeq__STATUS(this) &= ~0x1;
  return PtrSeq_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * First item of the PtrSeq.
 * Return:	Pointer to the first item.
 */
static inline PtrSeqItem
PtrSeq_firstItem(const_PtrSeq this)
{
  PtrSeqItem *base = PtrSeq_BASE(this);
  Except_REQUIRE(!PtrSeq_isEmpty(this));
  return base[0];
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Last item of the PtrSeq.
 * Return:	Pointer to the last item.
 */
static inline PtrSeqItem
PtrSeq_lastItem(const_PtrSeq this)
{
  PtrSeqItem *past = PtrSeq_PAST(this);
  Except_REQUIRE(!PtrSeq_isEmpty(this));
  return past[-1];
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Access the PtrSeq by index.
 * @param index	The index.
 * @return 	The corresponding item.
 */
static inline PtrSeqItem
PtrSeq_access(const_PtrSeq this, int32_t index)
{
  PtrSeqItem *base = PtrSeq_BASE(this);
  int32_t maxCount __attribute__ ((unused));
  maxCount = PtrSeq_MAXCOUNT(this);
  Except_REQUIRE(index >= 0 && index < maxCount);
  return base[index];
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access the bottom PtrSeq.
 */
static inline PtrSeqItem
PtrSeq_bottom(const_PtrSeq this)
{
  PtrSeqItem *base = PtrSeq_BASE(this);
  return base[0];
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access the top PtrSeqItem
 */
static inline PtrSeqItem
PtrSeq_top(const_PtrSeq this)
{
  PtrSeqItem *past = PtrSeq_PAST(this);
  Except_REQUIRE(!PtrSeq_isEmpty(this));
  return past[-1];
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Push as last item on the PtrSeq.
 */
static inline void
PtrSeq_push(PtrSeq this, PtrSeqItem item)
{
  PtrSeqItem *past = PtrSeq_PAST(this);
  Except_REQUIRE(!PtrSeq_isFull(this));
  *PtrSeq__PAST(this) = past + 1;
  *past = item;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Push as last item on the PtrSeq, resizing the PtrSeq if necessary.
 *
 * @return The pushed PtrSeqItem.
 *
 * @warning As the PtrSeq may be resized, it is an error to maintain pointers
 * to existing items.
 */
void
PtrSeq_push2(PtrSeq this, PtrSeqItem item);
#endif//$XCC_h

#ifndef PtrSeq_MAGICCOUNT
#define PtrSeq_MAGICCOUNT 16
#endif//PtrSeq_MAGICCOUNT

void
PtrSeq_push2(PtrSeq this, PtrSeqItem item)
{
  PtrSeqItem *past = PtrSeq_PAST(this);
  if (PtrSeq_isFull(this)) {
    int32_t newCount = PtrSeq_count(this)*2;
    if (newCount < PtrSeq_MAGICCOUNT) {
      newCount = PtrSeq_MAGICCOUNT;
    }
    PtrSeq_resize(this, newCount);
    past = PtrSeq_PAST(this);
    Except_CHECK(!PtrSeq_isFull(this));
  }
  *PtrSeq__PAST(this) = past + 1;
  *past = item;
}

#ifdef $XCC_h
/**
 * Pop the last PtrSeq item.
 */
static inline void
PtrSeq_pop(PtrSeq this)
{
  PtrSeqItem *past = PtrSeq_PAST(this);
  Except_REQUIRE(!PtrSeq_isEmpty(this));
  *PtrSeq__PAST(this) = --past;
}
#endif//$XCC_h

#ifdef $XCC__c
{
  const PtrSeqItem *array = 0;
  int count = 20, i;
  PtrSeq_ ptrseq[1];
  PtrSeq_Ctor(ptrseq, Memory_Root, count/5);
  for (i = 0 ; i < count; i++) {
    if (PtrSeq_isFull(ptrseq)) {
      PtrSeq_resize(ptrseq, i+3);
    }
    {
      intptr_t value = 'a' + i;
      PtrSeq_push(ptrseq, (PtrSeqItem)value);
    }
  }
  printf("Stack count = %d\n", count);
  i = 0;
  array = PtrSeq_base(ptrseq);
  PtrSeq_FOREACH(ptrseq, intptr_t, item) {
    if (item != (intptr_t)PtrSeq_access(ptrseq, i)) printf("error1 at %d\n", i);
    if (item != (intptr_t)array[i]) printf("error2 at %d\n", i);
    printf("%c ", (int)item);
    ++i;
  } PtrSeq_ENDEACH;
  printf("\n");
  i = PtrSeq_count(ptrseq);
  array = PtrSeq_base(ptrseq);
  PtrSeq_FORBACK(ptrseq, intptr_t, item) {
    --i;
    if (item != (intptr_t)PtrSeq_access(ptrseq, i)) printf("error1 at %d\n", i);
    if (item != (intptr_t)array[i]) printf("error2 at %d\n", i);
    printf("%c ", (int)item);
  } PtrSeq_ENDBACK;
  printf("\n");
  PtrSeq_Dtor(ptrseq);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the PtrSeq module.
 */
#define PtrSeq_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the PtrSeq module.
 */
#define PtrSeq_FINI()
#endif//$XCC__h

#if XCC__C
static void
PtrSeq_TEST(void)
{
#include "CCL/PtrSeq_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(PtrSeq_TEST);
  return 0;
}
#endif

