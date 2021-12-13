#ifdef $XCC_h
/*
 * !!!!	IETree.xcc
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
 * The IETree implements set of integers as interval trees, see Martin Erwing
 * ``Diets for Fat Sets'', J. Functional Programming, 1,
 * (http://cs.oregonstate.edu/~erwig/diet/).
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup CCL
 * @brief Integer set implement at interval encoding tree.
 */
#endif//$XCC_h


#include "CCL/CCL_.h"

#ifdef $XCC__h
#include "CCL/IETree.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef intptr_t IETreeMember;
/**
 * Successor of this IETreeMember.
 */
#define IETreeMember_SUCC(this) ((IETreeMember)(this) + 1)
/**
 * Predecessor of this IETreeMember.
 */
#define IETreeMember_PRED(this) ((IETreeMember)(this) - 1)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Compare two IETreeMembers for less than.
 */
#define IETreeMember_GT(a, b) ((IETreeMember)(a) > (IETreeMember)(b))
/**
 * Compare two IETreeMembers for greater than.
 */
#define IETreeMember_LT(a, b) ((IETreeMember)(a) < (IETreeMember)(b))
/**
 * Compare two IETreeMembers for less than or equal.
 */
#define IETreeMember_GE(a, b) ((IETreeMember)(a) >= (IETreeMember)(b))
/**
 * Compare two IETreeMembers for greater than or equal.
 */
#define IETreeMember_LE(a, b) ((IETreeMember)(a) <= (IETreeMember)(b))
/**
 * Compare two IETreeMembers for equality
 */
#define IETreeMember_EQ(a, b) ((IETreeMember)(a) == (IETreeMember)(b))
/**
 * Compare two IETreeMembers for non-equality
 */
#define IETreeMember_NE(a, b) ((IETreeMember)(a) != (IETreeMember)(b))
#endif//$XCC_h

#ifdef $XCC_h
/*
 * The IETreeNode_ structure implements the IETree tree nodes.
 */
struct IETreeNode_ {
  //@args	IETreeMember low, IETreeMember high
  IETreeMember LOW;
  IETreeMember HIGH;
  struct IETreeNode_ *LEFT;
  struct IETreeNode_ *RIGHT;
};
#endif//$XCC_h

IETreeNode
IETreeNode_Ctor(IETreeNode this, IETreeMember low, IETreeMember high)
{
  Except_REQUIRE(IETreeMember_LE(low, high));
  *IETreeNode__LOW(this) = low;
  *IETreeNode__HIGH(this) = high;
  *IETreeNode__LEFT(this) = NULL;
  *IETreeNode__RIGHT(this) = NULL;
  return this;
}

void
IETreeNode_Dtor(IETreeNode this)
{
  *IETreeNode__LEFT(this) = NULL;
  *IETreeNode__RIGHT(this) = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Interval Decoding Tree.
 */
struct IETree_ {
  //@args	Memory memory
  //  memory:	The Memory where the IETreeMembers are allocated.
  Memory MEMORY;
  IETreeNode ROOT;
  IETreeMember LOW;
  IETreeMember HIGH;
  int32_t COUNT;
  int32_t ORDER;
};
#endif//$XCC_h

IETree
IETree_Ctor(IETree this, Memory memory)
{
  *IETree__MEMORY(this) = memory;
  *IETree__ROOT(this) = NULL;
  *IETree__LOW(this) = (IETreeMember)0;
  *IETree__HIGH(this) = (IETreeMember)0;
  *IETree__COUNT(this) = 0;
  *IETree__ORDER(this) = 0;
  return this;
}

static void
IETree_killNode(IETree this, IETreeNode node)
{
  Memory memory = IETree_MEMORY(this);
  IETreeNode left = IETreeNode_LEFT(node);
  IETreeNode right = IETreeNode_RIGHT(node);
  if (left != NULL) IETree_killNode(this, left);
  if (right != NULL) IETree_killNode(this, right);
  IETreeNode_Dtor(node);
  Memory_free_(memory, node);
}

void
IETree_Dtor(IETree this)
{
  IETreeNode root = IETree_ROOT(this);
  if (root != NULL) IETree_killNode(this, root);
  *IETree__ROOT(this) = NULL;
  *IETree__LOW(this) = (IETreeMember)0;
  *IETree__HIGH(this) = (IETreeMember)0;
  *IETree__COUNT(this) = 0;
  *IETree__ORDER(this) = 0;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(IETree_)\t%zu\n", sizeof(IETree_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * True iff this IETree is empty.
 */
static inline bool
IETree_isEmpty(const_IETree this)
{
  return IETree_ROOT(this) == NULL;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * True iff this IETree contains a single member.
 */
static inline bool
IETree_isSingle(const_IETree this)
{
  IETreeNode root = IETree_ROOT(this);
  return root != NULL &&
      IETreeNode_LEFT(root) == NULL &&
      IETreeNode_RIGHT(root) == NULL &&
      IETreeNode_LOW(root) == IETreeNode_HIGH(root);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by IETree_FOREACH.
 */
static inline IETreeNode
IETree_root(const_IETree this)
{
  return IETree_ROOT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by IETree_FOREACH.
 */
static inline int32_t
IETree_order(const_IETree this)
{
  return IETree_ORDER(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count members contained in the IETree.
 * Return:	The count of members in this IETree.
 */
int32_t
IETree_count(const_IETree this);
#endif//$XCC_h

static inline IETreeNode
IETree_makeNode(IETree this, IETreeMember low, IETreeMember high)
{
  IETreeNode new_node = Memory_alloc_(IETree_MEMORY(this), sizeof(IETreeNode_));
  IETreeNode_Ctor(new_node, low, high);
  ++*IETree__ORDER(this);
  return new_node;
}

#ifdef $XCC__c
{
  // Check Memory_alloc_ is safe in IETree_makeNode.
  Except_ALWAYS(_ALIGNED(sizeof(IETreeNode_)));
}
#endif//$XCC__c

static inline void
IETree_delNode(IETree this, IETreeNode node)
{
  --*IETree__ORDER(this);
  IETreeNode_Dtor(node);
  Memory_free(IETree_MEMORY(this), node);
}

#ifdef $XCC__h
/**
 * Remove the minimum interval under node.
 */
IETreeNode
IETree_splitMin(IETree this, IETreeNode node);
#endif//$XCC__h

IETreeNode
IETree_splitMin(IETree this, IETreeNode node)
{
  IETreeNode prev_node = NULL;
  IETreeNode min_node = node;
  IETreeNode split_node = node;
  while (IETreeNode_LEFT(min_node) != NULL) {
    prev_node = min_node;
    min_node = IETreeNode_LEFT(min_node);
  }
  if (prev_node != NULL) {
    *IETreeNode__LEFT(prev_node) = IETreeNode_RIGHT(min_node);
  } else {
    split_node = IETreeNode_RIGHT(min_node);
  }
  *IETree__LOW(this) = IETreeNode_LOW(min_node);
  *IETree__HIGH(this) = IETreeNode_HIGH(min_node);
  IETree_delNode(this, min_node);
  return split_node;
}

#ifdef $XCC__h
/**
 * Remove the maximum interval under node.
 */
IETreeNode
IETree_splitMax(IETree this, IETreeNode node);
#endif//$XCC__h

IETreeNode
IETree_splitMax(IETree this, IETreeNode node)
{
  IETreeNode prev_node = NULL;
  IETreeNode max_node = node;
  IETreeNode split_node = node;
  while (IETreeNode_RIGHT(max_node) != NULL) {
    prev_node = max_node;
    max_node = IETreeNode_RIGHT(max_node);
  }
  if (prev_node != NULL) {
    *IETreeNode__RIGHT(prev_node) = IETreeNode_LEFT(max_node);
  } else {
    split_node = IETreeNode_LEFT(max_node);
  }
  *IETree__LOW(this) = IETreeNode_LOW(max_node);
  *IETree__HIGH(this) = IETreeNode_HIGH(max_node);
  IETree_delNode(this, max_node);
  return split_node;
}

#ifdef $XCC__h
/**
 * Merge the LEFT and RIGHT subtrees of node.
 */
IETreeNode
IETree_mergeNode(IETree this, IETreeNode node);
#endif//$XCC__h

IETreeNode
IETree_mergeNode(IETree this, IETreeNode node)
{
  IETreeNode left_node = IETreeNode_LEFT(node);
  IETreeNode right_node = IETreeNode_RIGHT(node);
  if (left_node == NULL) {
    IETree_delNode(this, node);
    return right_node;
  } else if (right_node == NULL) {
    IETree_delNode(this, node);
    return left_node;
  } else {
    IETreeNode split_node = IETree_splitMax(this, left_node);
    *IETreeNode__LOW(node) = IETree_LOW(this);
    *IETreeNode__HIGH(node) = IETree_HIGH(this);
    *IETreeNode__LEFT(node) = split_node;
    return node;
  }
}

#ifdef $XCC_h
/**
 * Choose and remove a member of the IETree.
 */
IETreeMember
IETree_choose(IETree this);
#endif//$XCC_h

IETreeMember
IETree_choose(IETree this)
{
  IETreeMember low, high;
  IETreeNode prev_node = NULL;
  IETreeNode node = IETree_ROOT(this);
  Except_REQUIRE(node != NULL);
  while (IETreeNode_LEFT(node) != NULL) {
    prev_node = node;
    node = IETreeNode_LEFT(node);
  }
  low = IETreeNode_LOW(node);
  high = IETreeNode_HIGH(node);
  if (IETreeMember_EQ(low, high)) {
    if (prev_node == NULL) {
      *IETree__ROOT(this) = IETree_mergeNode(this, node);
    } else {
      *IETreeNode__LEFT(prev_node) = IETree_mergeNode(this, node);
    }
  } else {
    *IETreeNode__LOW(node) = IETreeMember_SUCC(low);
  }
  return low;
}

#ifdef $XCC_h
/**
 * Test a member for containment.
 * Return:	True if this IETree contains member.
 */
bool
IETree_contains(const_IETree this, IETreeMember member);
#endif//$XCC_h

bool
IETree_contains(const_IETree this, IETreeMember member)
{
  IETreeNode node = IETree_ROOT(this);
  while (node != NULL) {
    IETreeMember low = IETreeNode_LOW(node);
    IETreeMember high = IETreeNode_HIGH(node);
    if (IETreeMember_LT(member, low)) node = IETreeNode_LEFT(node);
    else if (IETreeMember_GT(member, high)) node = IETreeNode_RIGHT(node);
    return true;
  }
  return false;
}

static void
IETree_joinLeft(IETree this, IETreeNode node)
{
  IETreeNode left_node = IETreeNode_LEFT(node);
  if (left_node != NULL) {
    IETreeNode max_node = left_node;
    while (IETreeNode_RIGHT(max_node) != NULL) {
      max_node = IETreeNode_RIGHT(max_node);
    }
    if (IETreeMember_EQ(IETreeNode_HIGH(max_node),
        IETreeMember_PRED(IETreeNode_LOW(node)))) {
      *IETreeNode__LOW(node) = IETreeNode_LOW(max_node);
      *IETreeNode__LEFT(node) = IETree_splitMax(this, left_node);
    }
  }
}

static void
IETree_joinRight(IETree this, IETreeNode node)
{
  IETreeNode right_node = IETreeNode_RIGHT(node);
  if (right_node != NULL) {
    IETreeNode min_node = right_node;
    while (IETreeNode_LEFT(min_node) != NULL) {
      min_node = IETreeNode_LEFT(min_node);
    }
    if (IETreeMember_EQ(IETreeNode_LOW(min_node),
        IETreeMember_SUCC(IETreeNode_HIGH(node)))) {
      *IETreeNode__HIGH(node) = IETreeNode_HIGH(min_node);
      *IETreeNode__RIGHT(node) = IETree_splitMin(this, right_node);
    }
  }
}

#ifdef $XCC_h
/**
 * Insert a member in this IETree.
 * Return:	False iff member was already contained in this IETree.
 */
bool
IETree_insert(IETree this, IETreeMember member);
#endif//$XCC_h

bool
IETree_insert(IETree this, IETreeMember member)
{
  IETreeNode node = IETree_ROOT(this);
  if (node == NULL) {
    *IETree__ROOT(this) = IETree_makeNode(this, member, member);
  } else do {
    IETreeMember low = IETreeNode_LOW(node);
    IETreeMember high = IETreeNode_HIGH(node);
    if (IETreeMember_LT(member, low)) {
      IETreeMember succ = IETreeMember_SUCC(member);
      if (IETreeMember_EQ(succ, low)) {
        *IETreeNode__LOW(node) = member;
        IETree_joinLeft(this, node);
        break;
      } else {
        if (IETreeNode_LEFT(node) == NULL) {
          *IETreeNode__LEFT(node) = IETree_makeNode(this, member, member);
          break;
        }
        node = IETreeNode_LEFT(node);
      }
    } else if (IETreeMember_GT(member, high)) {
      IETreeMember pred = IETreeMember_PRED(member);
      if (IETreeMember_EQ(pred, high)) {
        *IETreeNode__HIGH(node) = member;
        IETree_joinRight(this, node);
        break;
      } else {
        if (IETreeNode_RIGHT(node) == NULL) {
          *IETreeNode__RIGHT(node) = IETree_makeNode(this, member, member);
          break;
        }
        node = IETreeNode_RIGHT(node);
      }
    } else return false;
  } while (1);
  return true;
}

#ifdef $XCC_h
/**
 * Remove a member from this IETree.
 * Return:	False iff member was not contained in this IETree.
 */
bool
IETree_remove(IETree this, IETreeMember member);
#endif//$XCC_h

bool
IETree_remove(IETree this, IETreeMember member)
{
  IETreeNode prev_node = NULL;
  IETreeNode node = IETree_ROOT(this);
  while (node != NULL) {
    IETreeMember low = IETreeNode_LOW(node);
    IETreeMember high = IETreeNode_HIGH(node);
    if (IETreeMember_LT(member, low)) {
      prev_node = node;
      node = IETreeNode_LEFT(node);
    } else if (IETreeMember_GT(member, high)) {
      prev_node = node;
      node = IETreeNode_RIGHT(node);
    } else if (IETreeMember_EQ(low, high)) {
      if (prev_node == NULL) {
        *IETree__ROOT(this) = IETree_mergeNode(this, node);
      } else if (node == IETreeNode_LEFT(prev_node)) {
        *IETreeNode__LEFT(prev_node) = IETree_mergeNode(this, node);
      } else if (node == IETreeNode_RIGHT(prev_node)) {
        *IETreeNode__RIGHT(prev_node) = IETree_mergeNode(this, node);
      } else Except_NEVER(true);
      break;
    } else if (IETreeMember_EQ(member, low)) {
      *IETreeNode__LOW(node) = IETreeMember_SUCC(low);
      break;
    } else if (IETreeMember_EQ(member, high)) {
      *IETreeNode__HIGH(node) = IETreeMember_PRED(high);
      break;
    } else {
      IETreeNode new_node = IETree_makeNode(this, IETreeMember_SUCC(member), high);
      *IETreeNode__RIGHT(new_node) = IETreeNode_RIGHT(node);
      *IETreeNode__HIGH(node) = IETreeMember_PRED(member);
      *IETreeNode__RIGHT(node) = new_node;
      break;
    }
  }
  return node != NULL;
}

#if XCC__C
static void
IETreeNode_pretty(IETreeNode this, FILE *file)
{
  IETreeNode left = IETreeNode_LEFT(this);
  IETreeNode right = IETreeNode_RIGHT(this);
  IETreeMember low = IETreeNode_LOW(this);
  IETreeMember high = IETreeNode_HIGH(this);
  if (left != NULL) IETreeNode_pretty(left, file);
  fprintf(file, "[%lld,%lld] ", (long long)low, (long long)high);
  if (right != NULL) IETreeNode_pretty(right, file);
}

static bool
IETree_pretty(IETree this, FILE *file)
{
  IETreeNode root = IETree_ROOT(this);
  if (file == NULL) file = stderr;
  if (root != NULL) IETreeNode_pretty(root, file);
  fprintf(file, "\n");
  return true;
}
#endif//XCC__C

#ifdef $XCC__c
{
  int i = 0, j = 0, k = 5, l = 9;
  IETree_ ietree[1];
  IETree_Ctor(ietree, Memory_Root);
  IETree_insert(ietree, 5);
  IETree_insert(ietree, 2);
  IETree_insert(ietree, 8);
  IETree_remove(ietree, 5);
  IETree_remove(ietree, 2);
  IETree_remove(ietree, 8);
  printf("ietree:  \t");
  IETree_pretty(ietree, stdout);
  for (i = 0; i < 16; i++) {
    printf("insert %d:\t", (i*k)&15);
    IETree_insert(ietree, (i*k)&15);
    IETree_pretty(ietree, stdout);
  }
  do {
    j = (j + l)&15;
    printf("remove %d:\t", j);
    IETree_remove(ietree, j);
    IETree_pretty(ietree, stdout);
  } while (!IETree_isEmpty(ietree));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Test for IETree equality.
 */
bool
IETree_equals(const_IETree this, const_IETree that);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Union of this IETree with that IETree.
 */
bool
IETree_union(IETree this, IETree that);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Intersect this IETree with that IETree.
 */
bool
IETree_inter(IETree this, IETree that);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Remove that IETree members from this IETree.
 */
bool
IETree_diff(IETree this, IETree that);
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterates over the IETree members.
 */
#define IETree_FOREACH(this, iter) { \
  int32_t IETree_ORDER = IETree_order(this); \
  IETreeNode IETree_NODEARRAY[128]; \
  IETreeNode *IETree_NODES = IETree_ORDER > 128? \
                             Memory_alloc(Memory_Root, sizeof(IETreeNode)*IETree_ORDER): \
                             IETree_NODEARRAY; \
  IETreeNode *IETree_LAST = IETree_NODES; \
  IETreeNode IETree_NODE = IETree_root(this); \
  for (;;) { \
    while (IETree_NODE != NULL) { \
      *IETree_LAST++ = IETree_NODE; \
      IETree_NODE = IETreeNode_LEFT(IETree_NODE); \
    } \
    if (IETree_LAST-- != IETree_NODES) { \
      IETreeMember IETree_ITER = IETreeNode_LOW(*IETree_LAST); \
      IETreeMember IETree_HIGH = IETreeNode_HIGH(*IETree_LAST); \
      IETree_NODE = IETreeNode_RIGHT(*IETree_LAST); \
      for (; IETreeMember_LE(IETree_ITER, IETree_HIGH); \
          IETree_ITER = IETreeMember_SUCC(IETree_ITER)) { \
        IETreeMember iter = IETree_ITER;
#define IETree_ENDEACH \
      } \
      if (IETreeMember_LE(IETree_ITER, IETree_HIGH)) break; \
    } else break; \
  } \
  if (IETree_ORDER > 128) Memory_free_(Memory_Root, IETree_NODES); \
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int max = 100, i;
  IETree_ sieve[1];
  IETree_ primes[1];
  IETree_Ctor(sieve, Memory_Root);
  IETree_Ctor(primes, Memory_Root);
  for (i = 2; i < max/2; i++) IETree_insert(sieve, (IETreeMember)i);
  for (i = max - 1; i >= max/2; i--) IETree_insert(sieve, (IETreeMember)i);
  while (!IETree_isEmpty(sieve)) {
    int j = (int)IETree_choose(sieve);
    printf("prime %d\n", j);
    IETree_insert(primes, (IETreeMember)j);
    for (i = j + j; i < max; i += j) IETree_remove(sieve, (IETreeMember)i);
    printf("sieve: ");
    IETree_FOREACH(sieve, iter) {
      printf("%d ", (int)iter);
    } IETree_ENDEACH;
    printf("\n");
  }
  printf("primes: ");
  IETree_FOREACH(primes, iter) {
    printf("%d ", (int)iter);
  } IETree_ENDEACH;
  printf("\n");
  IETree_Dtor(primes);
  IETree_Dtor(sieve);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the IETree module.
 */
#define IETree_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the IETree module.
 */
#define IETree_FINI()
#endif//$XCC__h

#if XCC__C
static void
IETree_TEST(void)
{
#include "CCL/IETree_.c"
}

int
main(int argc, char **argv)
{
  CCL_Test(IETree_TEST);
  return 0;
}
#endif

