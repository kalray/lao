#ifdef $XCC_h
/*
 * !!!!	BBTree.xcc
 *
 * Marco Garatti (Marco.Garatti@st.com)
 *
 * Copyright 2007 STMicroelectronics.
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
 * @ingroup BSL
 * @brief Base Binary Tree common code (unused).
 *
 * A BBTree is a very simple data structure that represent binary trees.
 * Different nodes may contain different types.
 * Each node of the tree contains two information:
 *
 * - node metadata
 * - user data
 *
 * Clients only "see" the user data, while the package manipulates the
 * metadata.
 */
#endif//$XCC_h

#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/BBTree.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * The BBTreeNode_ structure implements the BBTree tree nodes.
 */
struct BBTreeNode_ {
  //@args
  void *LTKID;
  void *RTKID;
};
#endif//$XCC_h

BBTreeNode
BBTreeNode_Ctor(BBTreeNode this)
{
  *BBTreeNode__LTKID(this) = NULL;
  *BBTreeNode__RTKID(this) = NULL;
  return this;
}

static void
BBTreeNode_Dtor_(BBTreeNode this, Memory memory)
{
  BBTreeNode this_node = this - 1;
  if (BBTreeNode_LTKID(this_node) != NULL) {
    BBTreeNode_Dtor_(BBTreeNode_LTKID(this_node), memory);
  }
  if (BBTreeNode_RTKID(this_node) != NULL) {
    BBTreeNode_Dtor_(BBTreeNode_RTKID(this_node), memory);
  }
  Memory_free_(memory, this_node);
}

#ifdef $XCC_h
/*
 * The BBTree
 *  memory:	Where the BBTree nodes are allocated.
 */
struct BBTree_ {
  //@args	Memory memory
  Memory MEMORY;
  void* ROOT;		// The root node.
  int32_t COUNT;	// Current count of entries.
};
#endif//$XCC_h

BBTree
BBTree_Ctor(BBTree this, Memory memory)
{
  *BBTree__MEMORY(this) = memory;
  *BBTree__ROOT(this) = NULL;
  *BBTree__COUNT(this) = 0;
  return this;
}

void
BBTree_Dtor(BBTree this)
{
  if (BBTree_ROOT(this) != NULL) {
    BBTreeNode node = BBTree_ROOT(this);
    Memory memory = BBTree_MEMORY(this);
    BBTreeNode_Dtor_(node, memory);
  }
  *BBTree__ROOT(this) = NULL;
  *BBTree__COUNT(this) = 0;
}

#ifdef $XCC_h
/**
 * This BBTree count of entries.
 */
static inline int32_t
BBTree_count(const_BBTree this)
{
  return BBTree_COUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the BBTree root.
 *
 * @c new_root:	New root to be set.
 */
static inline void
BBTree_setRoot(BBTree this, void* new_root)
{
  *BBTree__ROOT(this) = new_root;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set the BBTree root.
 *
 * Return:	The tree root.
 */
static inline void*
BBTree_getRoot(const_BBTree this)
{
  return (BBTree_ROOT(this));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get left kid.
 */
static inline void *
BBTreeNode_getLTKID(void *node)
{
    unsigned char* header = ((unsigned char*)node) - sizeof(BBTreeNode_);
    return BBTreeNode_LTKID((BBTreeNode)((void*)header));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get right kid.
 */
static inline void *
BBTreeNode_getRTKID(void *node)
{
    unsigned char* header = ((unsigned char*)node) - sizeof(BBTreeNode_);
    return BBTreeNode_RTKID((BBTreeNode)((void*)header));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Insert in the BBTree tree.
 *
 * size:	The size of the element to add
 * Return:	@c value.
 */
void *
BBTree_createNode(BBTree this,int32_t size);
#endif//$XCC_h

void *
BBTree_createNode(BBTree this, int32_t size)
{
  Memory memory = BBTree_MEMORY(this);
  // The allocated object contains both the user data and the BBTree control information.
  size_t adjustedSize = size + sizeof(BBTreeNode_);
  size_t align_size = _ALIGN_NEXT(adjustedSize);
  BBTreeNode new_node = Memory_alloc_(memory, align_size);
  *BBTreeNode__LTKID(new_node) = NULL;
  *BBTreeNode__RTKID(new_node) = NULL;
  ++*BBTree__COUNT(this);
  return (void*)(new_node + 1);
}

#ifdef $XCC_h
/**
 * Connect two kids to a node. If kids already existed
 *                          they are just overwritten.
 * left:	  Left kid
 * right:	 Right kid
 */
void
BBTreeNode_attachKids(void* node,void* left,void* right);
#endif//$XCC_h

void
BBTreeNode_attachKids(void* node,void* left,void* right)
{
  /* These pointers point into the user data, go back to get to the underlying node info */
  BBTreeNode father = (BBTreeNode)node - 1;
  assert(left && right);
  *BBTreeNode__LTKID(father) = left;
  *BBTreeNode__RTKID(father) = right;
}

#ifdef $XCC_h
/**
 * Connect a kid to a node as left child. If kid
 *                             already existed it is just overwritten.
 * left:	  Left kid
 */
void
BBTreeNode_attachLeftKid(void* node,void* left);
#endif//$XCC_h

void
BBTreeNode_attachLeftKid(void* node,void* left)
{
  /* These pointers point into the user data, go back to get to the underlying node info */
  BBTreeNode father = (BBTreeNode)node - 1;
  assert(left);
  *BBTreeNode__LTKID(father) = left;
}

#ifdef $XCC_h
/**
 * Connect a kid to a node as right child. If kid
 *                              already existed it is just overwritten.
 * right:	  Right kid
 */
void
BBTreeNode_attachRightKid(void* node,void* right);
#endif//$XCC_h

void
BBTreeNode_attachRightKid(void* node,void* right)
{
  /* These pointers point into the user data, go back to get to the
      underlying node info */
  BBTreeNode father = (BBTreeNode)node - 1;
  assert(right);
  *BBTreeNode__RTKID(father) = right;
}

#ifdef $XCC_h
/**
 * Connect two trees as kids of a node. If kids already
 *                      existed they are just overwritten.
 *                      Please note that the trees to be connected are no longer
 *                      valid after this call and they cannot be used any longer,
 *                      but the object deallocation must be performed by the
 *                      client.
 * left:	  Left tree
 * right:	 Right tree
 */
void
BBTree_attachTrees(BBTree this, void* node,BBTree left,BBTree right);
#endif//$XCC_h

void
BBTree_attachTrees(BBTree this, void* node,BBTree left,BBTree right)
{
  /* These pointers point into the user data, go back to get to the
     underlying node info */
  BBTreeNode father = (BBTreeNode)node - 1;
  assert(left && right);
  *BBTreeNode__LTKID(father) = BBTree_ROOT(left);
  *BBTreeNode__RTKID(father) = BBTree_ROOT(right);
  *BBTree__COUNT(this) += BBTree_COUNT(left) + BBTree_COUNT(right);
}

#ifdef $XCC_h
/**
 * Connect a tree as left kid of a node. If kid already
 *                          existed they are just overwritten.
 *                          Please note that the trees to be connected are no
 *                          longer valid after this call and they cannot be used
 *                          any longer, but the object deallocation must be
 *                          performed by the client.
 * left:	  Left Tree
 */
void
BBTree_attachLeftTree(BBTree this, void* node,BBTree left);
#endif//$XCC_h

void
BBTree_attachLeftTree(BBTree this, void* node,BBTree left)
{
  /* These pointers point into the user data, go back to get to the underlying
     node info */
  BBTreeNode father = (BBTreeNode)node - 1;
  assert(left);
  *BBTreeNode__LTKID(father) = BBTree_ROOT(left);
  *BBTree__COUNT(this) += BBTree_COUNT(left);
}

#ifdef $XCC_h
/**
 * Connect a tree as right kid of a node.If kid already
 *                           existed they are just overwritten.
 *                           Please note that the trees to be connected are no
 *                           longer valid after this call and they cannot be used
 *                           any longer, but the object deallocation must be
 *                           performed by the client.
 * right:	  Right Tree
 */
void
BBTree_attachRightTree(BBTree this, void* node,BBTree right);
#endif//$XCC_h

void
BBTree_attachRightTree(BBTree this, void* node,BBTree right)
{
  /* These pointers point into the user data, go back to get to the underlying
     node info */
  BBTreeNode father = (BBTreeNode)node - 1;
  assert(right);
  *BBTreeNode__RTKID(father) = BBTree_ROOT(right);
  *BBTree__COUNT(this) += BBTree_COUNT(right);
}

#ifdef $XCC_h
/**
 * DFS walk of the BBTree entries using the supplied @c map function.
 */
void
BBTree_dfs(BBTree this, void (*map)(void *, va_list), ...);
#endif//$XCC_h

void
BBTree_dfs(BBTree this, void (*map)(void *, va_list), ...)
{
  va_list va;
  if (BBTree_ROOT(this) != NULL) {
    va_start(va, map);
    BBTreeNode_dfs(BBTree_ROOT(this), map, va);
    va_end(va);
  }
}

#ifdef $XCC__h
void
BBTreeNode_dfs(BBTreeNode this, void (*map)(void *, va_list), va_list va);
#endif//$XCC__h

void
BBTreeNode_dfs(BBTreeNode this, void (*map)(void *, va_list), va_list va)
{
  BBTreeNode this_node = this - 1;
  if (BBTreeNode_LTKID(this_node) != NULL) {
    BBTreeNode_dfs(BBTreeNode_LTKID(this_node), map, va);
  }
  if (BBTreeNode_RTKID(this_node) != NULL) {
    BBTreeNode_dfs(BBTreeNode_RTKID(this_node), map, va);
  }
  (*map)(this, va);
}

#if XCC__C
typedef struct MyStruct {
    int x;
    int y;
} MyStruct;

static void dump(void *value, va_list va)
{
  MyStruct* my_struct = (MyStruct*)value;
  printf("x=%d -- y=%d\n",my_struct->x,my_struct->y);
}
#endif

#ifdef $XCC__c
{
  BBTree_ btree1[1];
  BBTree_ btree2[1];
  MyStruct *root1, *node1, *node2, *node3, *node4;
  MyStruct *root2, *node5, *node6;
  //
  BBTree_Ctor(btree1, Memory_Root);
  /* Populate the BBTree 1 */
  root1 = (MyStruct*)BBTree_createNode(btree1,sizeof(MyStruct));
  BBTree_setRoot(btree1,root1);
  root1->x = 0;
  root1->y = 100;
  //
  node1 = (MyStruct*)BBTree_createNode(btree1,sizeof(MyStruct));
  node1->x = 1;
  node1->y = 101;
  //
  node2 = (MyStruct*)BBTree_createNode(btree1,sizeof(MyStruct));
  node2->x = 2;
  node2->y = 102;
  //
  BBTreeNode_attachKids(root1,node1,node2);
  //
  node3 = (MyStruct*)BBTree_createNode(btree1,sizeof(MyStruct));
  node3->x = 3;
  node3->y = 103;
  //
  node4 = (MyStruct*)BBTree_createNode(btree1,sizeof(MyStruct));
  node4->x = 4;
  node4->y = 104;
  //
  BBTreeNode_attachKids(node2,node3,node4);
  //
  printf("Tree #1: COUNT=%d\n",BBTree_COUNT(btree1));
  BBTree_dfs(btree1, dump, stdout);
  //
  BBTree_Ctor(btree2, Memory_Root);
  /* Populate the BBTree 2 */
  root2 = (MyStruct*)BBTree_createNode(btree2,sizeof(MyStruct));
  BBTree_setRoot(btree2,root2);
  root2->x = 5;
  root2->y = 105;
  //
  node5 = (MyStruct*)BBTree_createNode(btree2,sizeof(MyStruct));
  node5->x = 6;
  node5->y = 106;
  //
  node6 = (MyStruct*)BBTree_createNode(btree2,sizeof(MyStruct));
  node6->x = 7;
  node6->y = 107;
  //
  BBTreeNode_attachKids(root2,node5,node6);
  //
  printf("Tree #2: COUNT=%d\n",BBTree_COUNT(btree2));
  BBTree_dfs(btree2, dump, stdout);
  //
  /* Merge the two trees2 */
  BBTree_attachLeftTree(btree1,node3,btree2);
  //
  printf("Tree #1 (after merging): COUNT=%d\n",BBTree_COUNT(btree1));
  BBTree_dfs(btree1, dump, stdout);
  //
  BBTree_Dtor(btree1);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the BBTree module.
 */
#define BBTree_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the BBTree module.
 */
#define BBTree_FINI()
#endif//$XCC__h

#if XCC__C
static void
BBTree_TEST(void)
{
#include "BSL/BBTree_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(BBTree_TEST);
  return 0;
}
#endif

