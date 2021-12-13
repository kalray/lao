#ifdef $XCC_h
/*
 * !!!!	IDList.xcc
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
 * @brief List (double linked) of inlined items.
 *
 * An Inline Double List is a double-linked list, meaning that there is a prev
 * and a next pointer per list item, and these pointers are stored next to the
 * item. Precisely, the pointers are in a List_Item_ object that is inlined
 * below the item. List pointers point to items, not to the List_Item_ objects.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/IDList.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Pseudo type for IDList items.
 */
typedef void *IDListItem;
typedef const void *const_IDListItem;
typedef void (*IDListItemRelease)(IDListItem);
typedef int (*IDListItemCompare)(const_IDListItem, const_IDListItem);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access to the IDListItemCell under @c this item.
 */
#define IDListItem_CELL(this) ((IDListItemCell)(this) - 1)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access to the next item in the IDList.
 */
#define IDListItem_NEXT(this) IDListItemCell_NEXT(IDListItem_CELL(this))
#endif//$XCC_h

#define IDListItem__NEXT(this) IDListItemCell__NEXT(IDListItem_CELL(this))

#ifdef $XCC_h
/**
 * Access to the previous item in the IDList.
 */
#define IDListItem_PREV(this) IDListItemCell_PREV(IDListItem_CELL(this))
#endif//$XCC_h

#define IDListItem__PREV(this) IDListItemCell__PREV(IDListItem_CELL(this))

#ifdef $XCC_h
/**
 * List item cell inlined below the IDListItem(s).
 *
 * IDListItemCell is exported for use in IDList_FOREACH.
 */
struct IDListItemCell_ {
  //@args	size_t itemSize
  IDListItem NEXT;	// The next IDListItem.
  IDListItem PREV;	// The previous IDListItem.
  //@access ITEM	((IDListItem)((IDListItemCell)(this) + 1))
};
#endif//$XCC_h

IDListItemCell
IDListItemCell_Ctor(IDListItemCell this, size_t itemSize)
{
  *IDListItemCell__NEXT(this) = NULL;
  *IDListItemCell__PREV(this) = NULL;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IDListItemCell_)\t%zu\n", sizeof(IDListItemCell_));
  Except_ALWAYS(_ALIGNED(sizeof(IDListItemCell_)));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
struct IDList_ {
  //@args	Memory memory
  Memory MEMORY;	// Where the IDList items are allocated.
  IDListItem FIRSTITEM;	// First item in the IDList.
  IDListItem LASTITEM;	// Last item in the IDList.
  int32_t COUNT;	// Current count of items in the IDList.
};
#endif//$XCC_h

IDList
IDList_Ctor(IDList this, Memory memory)
{
  *IDList__MEMORY(this) = memory;
  *IDList__FIRSTITEM(this) = NULL;
  *IDList__LASTITEM(this) = NULL;
  *IDList__COUNT(this) = 0;
  return this;
}

void
IDList_Dtor(IDList this)
{
  IDList_empty(this, NULL);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IDList_)\t%zu\n", sizeof(IDList_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Empty this IDList.
 */
void
IDList_empty(IDList this, IDListItemRelease release);
#endif//$XCC_h

void
IDList_empty(IDList this, IDListItemRelease release)
{
  Memory memory = IDList_MEMORY(this);
  IDListItem firstItem = IDList_FIRSTITEM(this);
  IDListItem item = firstItem;
  if (release != NULL) {
    while (item != NULL) {
      IDListItemCell cell = IDListItem_CELL(item);
      IDListItem next = IDListItemCell_NEXT(cell);
      (*release)(item);
      Memory_free_(memory, cell);
      item = next;
    }
  } else {
    while (item != NULL) {
      IDListItemCell cell = IDListItem_CELL(item);
      IDListItem next = IDListItemCell_NEXT(cell);
      Memory_free_(memory, cell);
      item = next;
    }
  }
  *IDList__FIRSTITEM(this) = NULL;
  *IDList__LASTITEM(this) = NULL;
  *IDList__COUNT(this) = 0;
  Except_CHECK(IDList_isEmpty(this));
}

#ifdef $XCC_h
bool
IDList_check(const_IDList this, const_IDListItem item);
#endif//$XCC_h

bool
IDList_check(const_IDList this, const_IDListItem item)
{
  int count = IDList_COUNT(this);
  const_IDListItem firstItem = IDList_FIRSTITEM(this);
  const_IDListItem lastItem = IDList_LASTITEM(this);
  if (firstItem ==  NULL) {
    Except_ALWAYS(lastItem == NULL);
    Except_ALWAYS(item == NULL);
    Except_ALWAYS(count == 0);
  } else {
    int found = 0;
    const_IDListItem curr = firstItem, prev = NULL;
    while (curr != NULL) {
      if (curr == item) found++;
      Except_ALWAYS(IDListItem_PREV(curr) == prev);
      --count, prev = curr;
      curr = IDListItem_NEXT(curr);
    }
    if (item != NULL) Except_ALWAYS(found == 1);
    Except_ALWAYS(prev == lastItem);
    Except_ALWAYS(count == 0);
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This IDList memory.
 */
static inline Memory
IDList_memory(const_IDList this)
{
  return IDList_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/*
  IDList_isEmpty -- True iff this IDList is empty.
 */
#define IDList_isEmpty(this)	(IDList_count(this) == 0)
/*
  IDList_isSingle -- True iff this IDList has a single entry.
 */
#define IDList_isSingle(this)	(IDList_count(this) == 1)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count items in the IDList.
 * Return:	The count of items in the IDList.
 */
static inline int32_t
IDList_count(const_IDList this)
{
  return IDList_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * First item of the IDList.
 * Return:	Pointer to the firstItem.
 */
static inline IDListItem
IDList_firstItem(const_IDList this)
{
  return IDList_FIRSTITEM(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Dereference bottom (firstItem) item of this IDList.
 */
#define IDList_BOT(this, Type) *(Type *)IDList_firstItem(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Last item of the IDList.
 * Return:	Pointer to the lastItem.
 */
static inline IDListItem
IDList_lastItem(const_IDList this)
{
  return IDList_LASTITEM(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Dereference top (lastItem) item of this IDList.
 */
#define IDList_TOP(this, Type) *(Type *)IDList_lastItem(this)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate from firstItem to lastItem IDList item.
 *
 * Exiting IDList_FOREACH with @c break or @c return is allowed.
 * @c Type:	Type of the inlined items.
 * @c iter:	Type* pointer set to each item.
 */
#define IDList_FOREACH(this, Type, iter) { \
  IDListItem IDList_NEXT = NULL; \
  Type *(iter) = (Type *)IDList_firstItem(this); \
  for (; iter != NULL; iter = IDList_NEXT) { \
    IDListItemCell IDList_ITER = IDListItem_CELL(iter); \
    IDList_FOREACH_SYNC;
#define IDList_FOREACH_SYNC \
    IDList_NEXT = IDListItemCell_NEXT(IDList_ITER)
#define IDList_ENDEACH \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate from lastItem to firstItem IDList item.
 *
 * Exiting IDList_FORBACK with @c break or @c return is allowed.
 * @c Type:	Type of the inlined items.
 * @c iter:	Type* pointer set to each item.
 */
#define IDList_FORBACK(this, Type, iter) { \
  IDListItem IDList_PREV = NULL; \
  Type *(iter) = (Type *)IDList_lastItem(this); \
  for (; iter != NULL; iter = IDList_PREV) { \
    IDListItemCell IDList_ITER = IDListItem_CELL(iter); \
    IDList_FORBACK_SYNC;
#define IDList_FORBACK_SYNC \
    IDList_PREV = IDListItemCell_PREV(IDList_ITER)
#define IDList_ENDBACK \
  } \
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#define IDList_NEWCELL(this, itemSize, cell) \
  Memory memory = IDList_MEMORY(this); \
  size_t adjustedSize = sizeof(IDListItemCell_) + (itemSize); \
  size_t alignedSize = _ALIGN_NEXT(adjustedSize); \
  IDListItemCell cell = (IDListItemCell)Memory_alloc_(memory, alignedSize); \
  *IDListItemCell__NEXT(cell) = NULL; \
  *IDListItemCell__PREV(cell) = NULL;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make a new IDListItem.
 */
static inline IDListItem
IDList_makeItem(IDList this, size_t itemSize)
{
  IDList_NEWCELL(this, itemSize, cell);
  return IDListItemCell_ITEM(cell);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Push an allocated item as lastItem on the IDList.
 * @c item:	The IDListItem to push.
 * Return:	The pushed IDListItem.
 */
static inline IDListItem
IDList_push_(IDList this, IDListItem item)
{
  IDListItemCell cell = IDListItem_CELL(item);
  IDListItem firstItem = IDList_FIRSTITEM(this);
  IDListItem lastItem = IDList_LASTITEM(this);
  IDListItemCell last_cell = IDListItem_CELL(lastItem);
  Except_REQUIRE(IDListItemCell_PREV(cell) == NULL);
  Except_REQUIRE(IDListItemCell_NEXT(cell) == NULL);
  if (lastItem != NULL) *IDListItemCell__NEXT(last_cell) = item;
  if (firstItem == NULL) *IDList__FIRSTITEM(this) = item;
  *IDListItemCell__PREV(cell) = lastItem;
  *IDListItemCell__NEXT(cell) = NULL;
  *IDList__LASTITEM(this) = item;
  ++*IDList__COUNT(this);
  //Except_DEBUG(IDList_check(this, item));
  return item;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make and push an item as lastItem on the IDList.
 * @c itemSize:	The item size in bytes.
 * Return:	The pushed IDListItem.
 */
IDListItem
IDList_push(IDList this, size_t itemSize);
#endif//$XCC_h

IDListItem
IDList_push(IDList this, size_t itemSize)
{
  IDList_NEWCELL(this, itemSize, cell);
  return IDList_push_(this, IDListItemCell_ITEM(cell));
}

#ifdef $XCC_h
/**
 * Macro used to IDList_push value types.
 */
#define IDList_PUSH(this, Type, value) \
  (*(Type *)IDList_push(this, sizeof(Type)) = (value))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Put an allocated item as firstItem on the IDList.
 * @c item:	The IDListItem to put.
 * Return:	The put IDListItem.
 */
static inline IDListItem
IDList_put_(IDList this, IDListItem item)
{
  IDListItemCell cell = IDListItem_CELL(item);
  IDListItem firstItem = IDList_FIRSTITEM(this);
  IDListItem lastItem = IDList_LASTITEM(this);
  IDListItemCell first_cell = IDListItem_CELL(firstItem);
  Except_REQUIRE(IDListItemCell_PREV(cell) == NULL);
  Except_REQUIRE(IDListItemCell_NEXT(cell) == NULL);
  if (firstItem != NULL) *IDListItemCell__PREV(first_cell) = item;
  if (lastItem == NULL) *IDList__LASTITEM(this) = item;
  *IDListItemCell__NEXT(cell) = firstItem;
  *IDListItemCell__PREV(cell) = NULL;
  *IDList__FIRSTITEM(this) = item;
  ++*IDList__COUNT(this);
  //Except_DEBUG(IDList_check(this, item));
  return item;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make and put an item as firstItem on the IDList.
 * @param itemSize the item size in bytes.
 * @return the put IDListItem.
 */
IDListItem
IDList_put(IDList this, size_t itemSize);
#endif//$XCC_h

IDListItem
IDList_put(IDList this, size_t itemSize)
{
  IDList_NEWCELL(this, itemSize, cell);
  return IDList_put_(this, IDListItemCell_ITEM(cell));
}

#ifdef $XCC_h
/**
 * Macro used to IDList_put value types.
 */
#define IDList_PUT(this, Type, value) \
  (*(Type *)IDList_put(this, sizeof(Type)) = (value))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Insert an allocated item before item on the IDList.
 * @c before:	The item to insert before.
 * @c item:	The IDListItem to insert.
 * Return:	The inserted IDListItem.
 */
static inline IDListItem
IDList_insert_(IDList this, IDListItem before, IDListItem item)
{
  Except_REQUIRE(before != item);
  if (before == IDList_FIRSTITEM(this)) {
    return IDList_put_(this, item);
  } else {
    IDListItemCell cell = IDListItem_CELL(item);
    IDListItemCell before_cell = IDListItem_CELL(before);
    IDListItem prev = IDListItemCell_PREV(before_cell);
    IDListItemCell prev_cell = IDListItem_CELL(prev);
    Except_REQUIRE(IDListItemCell_PREV(cell) == NULL);
    Except_REQUIRE(IDListItemCell_NEXT(cell) == NULL);
    *IDListItemCell__NEXT(cell) = before;
    *IDListItemCell__PREV(cell) = prev;
    *IDListItemCell__PREV(before_cell) = item;
    *IDListItemCell__NEXT(prev_cell) = item;
    ++*IDList__COUNT(this);
    //Except_DEBUG(IDList_check(this, item));
    return item;
  }
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make and insert an item before a given item on the IDList.
 * @c before:	The item to insert before.
 * @c itemSize:	The item size in bytes.
 * Return:	The inserted IDListItem.
 */
IDListItem
IDList_insert(IDList this, IDListItem before, size_t itemSize);
#endif//$XCC_h

IDListItem
IDList_insert(IDList this, IDListItem before, size_t itemSize)
{
  IDList_NEWCELL(this, itemSize, cell);
  return IDList_insert_(this, before, IDListItemCell_ITEM(cell));
}

#ifdef $XCC_h
/**
 * Append an allocated item after a given item on the IDList.
 * @c after:	The item to append after.
 * @c item:	The IDListItem to append.
 * Return:	The inserted IDListItem.
 */
static inline IDListItem
IDList_append_(IDList this, IDListItem after, IDListItem item)
{
  Except_REQUIRE(after != item);
  if (after == IDList_LASTITEM(this)) {
    return IDList_push_(this, item);
  } else {
    IDListItemCell cell = IDListItem_CELL(item);
    IDListItemCell after_cell = IDListItem_CELL(after);
    IDListItem next = IDListItemCell_NEXT(after_cell);
    IDListItemCell next_cell = IDListItem_CELL(next);
    Except_REQUIRE(IDListItemCell_PREV(cell) == NULL);
    Except_REQUIRE(IDListItemCell_NEXT(cell) == NULL);
    *IDListItemCell__PREV(cell) = after;
    *IDListItemCell__NEXT(cell) = next;
    *IDListItemCell__NEXT(after_cell) = item;
    *IDListItemCell__PREV(next_cell) = item;
    ++*IDList__COUNT(this);
    //Except_DEBUG(IDList_check(this, item));
    return item;
  }
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make and append an item after a given item on the IDList.
 * @c after:	The item to append after.
 * @c itemSize:	The item size in bytes.
 * Return:	The inserted IDListItem.
 */
IDListItem
IDList_append(IDList this, IDListItem after, size_t itemSize);
#endif//$XCC_h

IDListItem
IDList_append(IDList this, IDListItem after, size_t itemSize)
{
  IDList_NEWCELL(this, itemSize, cell);
  return IDList_append_(this, after, IDListItemCell_ITEM(cell));
}

#ifdef $XCC_h
/**
 * Pop without releasing the lastItem IDList item.
 * Return:	True iff items remain in this IDList.
 */
static inline void
IDList_pop_(IDList this)
{
  IDListItem lastItem = IDList_LASTITEM(this);
  IDListItemCell last_cell = IDListItem_CELL(lastItem);
  IDListItem prev = IDListItemCell_PREV(last_cell);
  Except_CHECK(lastItem != NULL);
  *IDList__LASTITEM(this) = prev;
  if (prev == NULL) {
    *IDList__FIRSTITEM(this) = NULL;
  } else {
    IDListItemCell prev_cell = IDListItem_CELL(prev);
    *IDListItemCell__NEXT(prev_cell) = NULL;
  }
  *IDListItemCell__PREV(last_cell) = NULL;
  *IDListItemCell__NEXT(last_cell) = NULL;
  --*IDList__COUNT(this);
  //Except_DEBUG(IDList_check(this, NULL));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Pop and release the lastItem IDList item.
 * Return:	True iff items remain in this IDList.
 */
void
IDList_pop(IDList this, IDListItemRelease release);
#endif//$XCC_h

void
IDList_pop(IDList this, IDListItemRelease release)
{
  Memory memory = IDList_MEMORY(this);
  IDListItem lastItem = IDList_LASTITEM(this);
  IDListItemCell last_cell = IDListItem_CELL(lastItem);
  IDList_pop_(this);
  if (release != NULL) {
    (*release)(lastItem);
  }
  Memory_free_(memory, last_cell);
}

#ifdef $XCC_h
/**
 * Drop without releasing the firstItem IDList item.
 * Return:	True iff items remain in this IDList.
 */
static inline void
IDList_drop_(IDList this)
{
  IDListItem firstItem = IDList_FIRSTITEM(this);
  IDListItemCell first_cell = IDListItem_CELL(firstItem);
  IDListItem next = IDListItemCell_NEXT(first_cell);
  Except_CHECK(firstItem != NULL);
  *IDList__FIRSTITEM(this) = next;
  if (next == NULL) {
    *IDList__LASTITEM(this) = NULL;
  } else {
    IDListItemCell next_cell = IDListItem_CELL(next);
    *IDListItemCell__PREV(next_cell) = NULL;
  }
  *IDListItemCell__PREV(first_cell) = NULL;
  *IDListItemCell__NEXT(first_cell) = NULL;
  --*IDList__COUNT(this);
  //Except_DEBUG(IDList_check(this, NULL));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Drop and release the firstItem IDList item.
 * Return:	True iff items remain in this IDList.
 */
void
IDList_drop(IDList this, IDListItemRelease release);
#endif//$XCC_h

void
IDList_drop(IDList this, IDListItemRelease release)
{
  Memory memory = IDList_MEMORY(this);
  IDListItem firstItem = IDList_FIRSTITEM(this);
  IDListItemCell first_cell = IDListItem_CELL(firstItem);
  IDList_drop_(this);
  if (release != NULL) {
    (*release)(firstItem);
  }
  Memory_free_(memory, first_cell);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Remove without releasing an item from this IDList.
 */
static inline void
IDList_remove_(IDList this, IDListItem item)
{
  if (item == IDList_FIRSTITEM(this)) IDList_drop_(this);
  else if (item == IDList_LASTITEM(this)) IDList_pop_(this);
  else {
    IDListItemCell cell = IDListItem_CELL(item);
    IDListItem prev = IDListItemCell_PREV(cell);
    IDListItem next = IDListItemCell_NEXT(cell);
    IDListItemCell prev_cell = IDListItem_CELL(prev);
    IDListItemCell next_cell = IDListItem_CELL(next);
    //Except_DEBUG(IDList_check(this, item));
    *IDListItemCell__PREV(next_cell) = prev;
    *IDListItemCell__NEXT(prev_cell) = next;
    *IDListItemCell__PREV(cell) = NULL;
    *IDListItemCell__NEXT(cell) = NULL;
    --*IDList__COUNT(this);
  }
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Remove and release an item from this IDList.
 */
static inline void
IDList_remove(IDList this, IDListItem item, IDListItemRelease release)
{
  Memory memory = IDList_MEMORY(this);
  IDListItemCell cell = IDListItem_CELL(item);
  IDList_remove_(this, item);
  if (release != NULL) {
    (*release)(item);
  }
  Memory_free_(memory, cell);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Rotate the list so that item at @c index becomes firstItem (index 0).
 */
void
IDList_rotate(IDList this, int32_t index);
#endif//$XCC_h

void
IDList_rotate(IDList this, int32_t index)
{
  if (IDList_count(this) <= 1) return;
  while (index > 0) {
    IDListItem item = IDList_FIRSTITEM(this);
    IDListItemCell cell = IDListItem_CELL(item);
    IDListItem lastItem = IDList_LASTITEM(this);
    IDListItemCell last_cell = IDListItem_CELL(lastItem);
    IDListItem next = IDListItemCell_NEXT(cell);
    IDListItemCell next_cell = IDListItem_CELL(next);
    *IDList__FIRSTITEM(this) = next;
    *IDListItemCell__PREV(next_cell) = NULL;
    *IDListItemCell__NEXT(last_cell) = item;
    *IDListItemCell__PREV(cell) = lastItem;
    *IDListItemCell__NEXT(cell) = NULL;
    *IDList__LASTITEM(this) = item;
    //Except_DEBUG(IDList_check(this, NULL));
    --index;
  }
  while (index < 0) {
    IDListItem item = IDList_LASTITEM(this);
    IDListItemCell cell = IDListItem_CELL(item);
    IDListItem firstItem = IDList_FIRSTITEM(this);
    IDListItemCell first_cell = IDListItem_CELL(firstItem);
    IDListItem prev = IDListItemCell_PREV(cell);
    IDListItemCell prev_cell = IDListItem_CELL(prev);
    *IDList__LASTITEM(this) = prev;
    *IDListItemCell__NEXT(prev_cell) = NULL;
    *IDListItemCell__PREV(first_cell) = item;
    *IDListItemCell__NEXT(cell) = firstItem;
    *IDListItemCell__PREV(cell) = NULL;
    *IDList__FIRSTITEM(this) = item;
    //Except_DEBUG(IDList_check(this, NULL));
    ++index;
  }
  Except_DEBUG(IDList_check(this, NULL));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Test if this IDList contains the given IDListItem.
 */
bool
IDList_contains(const_IDList this, IDListItem item);
#endif//$XCC_h

bool
IDList_contains(const_IDList this, IDListItem item)
{
  bool result = false;
  IDList_FOREACH(this, void, iter) {
    if (iter == item) {
      result = true;
      break;
    }
  } IDList_ENDEACH;
  return result;
}

#ifdef $XCC_h
/**
 * Test if this IDList contains the item same as the given object.
 * @c compare:	The compare function, returns zero if its two arguments compare the same.
 * Return:	The firstItem IDList item that was found the same as the given object.
 */
IDListItem
IDList_containsSame(const_IDList this, int (*compare)(IDListItem, IDListItem),
                    IDListItem object);
#endif//$XCC_h

IDListItem
IDList_containsSame(const_IDList this, int (*compare)(IDListItem, IDListItem),
                    IDListItem object)
{
  IDList_FOREACH(this, void, iter) {
    if (!(*compare)(iter, object)) return iter;
  } IDList_ENDEACH;
  return NULL;
}

#ifdef $XCC_h
/**
 * Grep this IDList according to a given keep function.
 * @c keep:	Pointer to the item keep function, returns true for the items to keep.
 * @c that:	The filtered out items are pushed on that IDList.
 * Return:	The number of items filtered out.
 */
int
IDList_grep(IDList this, IDList that, bool (*keep)(IDListItem, va_list), ...);
#endif//$XCC_h

int
IDList_grep(IDList this, IDList that, bool (*keep)(IDListItem, va_list), ...)
{
  int result = 0;
  va_list va;
  Except_REQUIRE(IDList_MEMORY(this) == IDList_MEMORY(that));
  IDList_FOREACH(this, void, iter) {
    va_start(va, keep);
    if (!(*keep)(iter, va)) {
      IDList_remove_(this, iter);
      IDList_push_(that, iter);
      ++result;
    }
    va_end(va);
  } IDList_ENDEACH;
  Except_DEBUG(IDList_check(this, NULL));
  return result;
}

#ifdef $XCC_h
/**
 * Transfer all items from this IDList at the end of that IDList.
 */
void
IDList_transfer(IDList this, IDList that);
#endif//$XCC_h

void
IDList_transfer(IDList this, IDList that)
{
  Except_REQUIRE(IDList_MEMORY(this) == IDList_MEMORY(that));
  if (IDList_COUNT(that) == 0) {
    *IDList__FIRSTITEM(that) = IDList_FIRSTITEM(this);
    *IDList__LASTITEM(that) = IDList_LASTITEM(this);
    *IDList__COUNT(that) = IDList_COUNT(this);
  } else if (IDList_COUNT(this) != 0) {
    IDListItem this_firstItem = IDList_FIRSTITEM(this);
    IDListItem this_lastItem = IDList_LASTITEM(this);
    IDListItem that_lastItem = IDList_LASTITEM(that);
    *IDListItemCell__PREV(IDListItem_CELL(this_firstItem)) = that_lastItem;
    *IDListItemCell__NEXT(IDListItem_CELL(that_lastItem)) = this_firstItem;
    *IDList__LASTITEM(that) = this_lastItem;
    *IDList__COUNT(that) += IDList_COUNT(this);
  }
  *IDList__FIRSTITEM(this) = NULL;
  *IDList__LASTITEM(this) = NULL;
  *IDList__COUNT(this) = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * IDList_Meld -- Auxiliary merge-sort function used by IDList_sort.
 */
static IDListItem
IDList_Meld(IDListItemCompare compare, IDListItem item1, IDListItem item2)
{
#ifdef IDList_RECURSIVE_MELD
  if (item1 == NULL) return item2;
  if (item2 == NULL) return item1;
  if ((*compare)(item1, item2) <= 0) {
    *IDListItem__NEXT(item1) = IDList_Meld(compare, IDListItem_NEXT(item1), item2);
    return item1;
  } else {
    *IDListItem__NEXT(item2) = IDList_Meld(compare, item1, IDListItem_NEXT(item2));
    return item2;
  }
#else
  IDListItemCell_ meld_cell[1];
  IDListItem meld = IDListItemCell_ITEM(meld_cell);
  while (item1 != NULL && item2 != NULL) {
    if ((*compare)(item1, item2) <= 0) {
      *IDListItem__NEXT(meld) = item1;
      item1 = IDListItem_NEXT((meld = item1));
    } else {
      *IDListItem__NEXT(meld) = item2;
      item2 = IDListItem_NEXT((meld = item2));
    }
  }
  if (item1 != NULL) {
    *IDListItem__NEXT(meld) = item1;
  }
  if (item2 != NULL) {
    *IDListItem__NEXT(meld) = item2;
  }
  return IDListItemCell_NEXT(meld_cell);
#endif
}

#ifdef $XCC_h
/**
 * Sort this IDList according to the @c compare compare function.
 * Return:	False iff this IDList was already sorted.
 */
bool
IDList_sort(IDList this, IDListItemCompare compare);
#endif//$XCC_h

bool
IDList_sort(IDList this, IDListItemCompare compare)
{
  int32_t count = IDList_count(this);
  if (count > 1) {
    Memory memory = IDList_MEMORY(this);
    IDListItem prev = IDList_FIRSTITEM(this);
    IDListItem item = IDListItem_NEXT(prev);
    IQueue_ iqueue[1];
    IQueue_Ctor(iqueue, memory, sizeof(IDListItem), count);
    // Split this IDList into sorted chains.
    IQueue_PUT(iqueue, IDListItem, prev);
    while (item != NULL) {
      if ((*compare)(prev, item) > 0) {
        IQueue_PUT(iqueue, IDListItem, item);
        *IDListItem__PREV(item) = NULL;
        *IDListItem__NEXT(prev) = NULL;
      }
      item = IDListItem_NEXT((prev = item));
    }
    if (IQueue_isSingle(iqueue)) {
      IQueue_Dtor(iqueue);
      return false;
    }
    // Merge the sorted chains together.
    do {
      IDListItem item1 = NULL, item2 = NULL;
      IQueue_POP(iqueue, IDListItem, item1);
      IQueue_POP(iqueue, IDListItem, item2);
      IQueue_PUT(iqueue, IDListItem, IDList_Meld(compare, item1, item2));
    } while (!IQueue_isSingle(iqueue));
    // Restore this IDList invariants.
    *IDList__FIRSTITEM(this) = IQueue_TOP(iqueue, IDListItem);
    item = IDList_firstItem(this), prev = NULL;
    while (item != NULL) {
      *IDListItem__PREV(item) = prev;
      item = IDListItem_NEXT((prev = item));
    }
    *IDList__LASTITEM(this) = prev;
    Except_DEBUG(IDList_check(this, NULL));
    IQueue_Dtor(iqueue);
    return true;
  }
  return false;
}

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
  IDList_ idlist[1];
  IDList_Ctor(idlist, Memory_Root);
  printf("push/pop/FOREACH:\n");
  for (i = 0; i < 16; i++) {
    Pair k = IDList_push(idlist, sizeof(Pair_));
    k->a = i, k->b = j;
    j = (j + 7) & 15;
  }
  while (!IDList_isEmpty(idlist)) {
    printf("[\t");
    IDList_FOREACH(idlist, Pair_, k) {
      printf("%d,%d\t", k->a, k->b);
    } IDList_ENDEACH;
    printf("]\n");
    IDList_pop(idlist, NULL);
  }
  printf("put/drop/FORBACK:\n");
  for (i = 0; i < 16; i++) {
    Pair k = IDList_put(idlist, sizeof(Pair_));
    k->a = i, k->b = j;
    j = (j + 7) & 15;
  }
  while (!IDList_isEmpty(idlist)) {
    printf("[\t");
    IDList_FORBACK(idlist, Pair_, k) {
      printf("%d,%d\t", k->a, k->b);
    } IDList_ENDBACK;
    printf("]\n");
    IDList_drop(idlist, NULL);
  }
  printf("circular test:\n");
  for (i = 0; i < 16 - 1; i++) {
    Pair k1 = IDList_push(idlist, sizeof(Pair_));
    Pair k2 = IDList_push(idlist, sizeof(Pair_));
    k1->a = i, k1->b = 2*i;
    k2->a = -i, k2->b = -2*i;
    IDList_drop(idlist, NULL);
  }
  printf("[\t");
  IDList_FOREACH(idlist, Pair_, k) {
    printf("%d\t", k->a);
  } IDList_ENDEACH;
  printf("]\n");
  printf("remove test:\n");
  i = 0;
  IDList_FOREACH(idlist, Pair_, k) {
    if (++i & 1) {
      printf("remove %d\n", k->a);
      IDList_remove(idlist, k, NULL);
    }
  } IDList_ENDEACH;
  printf("[\t");
  IDList_FOREACH(idlist, Pair_, k) {
    printf("%d\t", k->a);
  } IDList_ENDEACH;
  printf("]\n");
  printf("rotate 1:\n");
  IDList_rotate(idlist, 1);
  printf("[\t");
  IDList_FOREACH(idlist, Pair_, k) {
    printf("%d\t", k->a);
  } IDList_ENDEACH;
  printf("]\n");
  printf("rotate -2:\n");
  IDList_rotate(idlist, -2);
  printf("[\t");
  IDList_FOREACH(idlist, Pair_, k) {
    printf("%d\t", k->a);
  } IDList_ENDEACH;
  printf("]\n");
  IDList_Dtor(idlist);
}
#endif//$XCC__c

#ifdef $XCC__c
{
  IDList_ idlist[1];
  int maxCount = 50, i;
  IDList_Ctor(idlist, Memory_Root);
  for (i = 0; i < maxCount; i++) {
    IDList_PUSH(idlist, int, (47*i)%maxCount);
  }
  printf("list:\n  [\t");
  IDList_FOREACH(idlist, int, p) {
    printf("%d,\t", *p);
  } IDList_ENDEACH;
  printf("]\n");
  IDList_sort(idlist, int_compare);
  printf("sorted:\n  [\t");
  IDList_FOREACH(idlist, int, p) {
    printf("%d,\t", *p);
  } IDList_ENDEACH;
  printf("]\n");
  for (i = 1; i < maxCount; i++) {
    int factor = (47*i)%maxCount;
    IDList_ removed[1];
    IDList_Ctor(removed, Memory_Root);
    printf("remove multiples of %d:\t", factor);
    if (IDList_grep(idlist, removed, not_multiple, factor)) {
      IDList_FOREACH(removed, int, p) {
        printf("%d\t", *p);
      } IDList_ENDEACH;
    } else {
      Except_CHECK(IDList_isEmpty(removed));
      printf("(none)");
    }
    printf("\n");
    IDList_Dtor(removed);
  }
  IDList_Dtor(idlist);
}
#endif//$XCC__c

#if XCC__C
static int
int_compare(const_IDListItem a, const_IDListItem b)
{
  int inta = *(int *)a;
  int intb = *(int *)b;
  int compare = (inta > intb) - (inta < intb);
  return compare;
}

static bool
not_multiple(IDListItem a, va_list va)
{
  int inta = *(int *)a;
  int factor = (int)va_arg(va, int);
  if (inta == 0 || factor == 0) return true;
  return (inta/factor)*factor != inta;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the IDList module.
 */
#define IDList_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the IDList module.
 */
#define IDList_FINI()
#endif//$XCC__h

#if XCC__C
static void
IDList_TEST(void)
{
#include "CCL/IDList_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(IDList_TEST);
  return 0;
}
#endif

