#ifdef $XCC_h
/*
 * !!!!	Hierarchy.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 *
 * Copyright 2002 - 2007 STMicroelectronics.
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
 * @ingroup CAL
 * @brief Hierarchy of SCCs in a DGraph (Bourdoncle).
 */
#endif//$XCC_h


#include "CAL/CAL_.h"

#ifdef $XCC__h
#include "CAL/Hierarchy.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerate the HierarchyComponent types.
 */
typedef enum {
  HierarchyComponentType_Node,
  HierarchyComponentType_Root,
  HierarchyComponentType_SCC,
  HierarchyComponentType_Loop,
  HierarchyComponentType__
} HierarchyComponentType;
typedef uint8_t short_HierarchyComponentType;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Region in a Hierarchy of a DGraph.
 */
struct HierarchyComponent_ {
  //@args	HierarchyComponentType type, DGraphNode node
  short_HierarchyComponentType TYPE;	// This HierarchyComponent type.
  int16_t DEPTH;			// Nesting level of this HierarchyComponent.
  DGraphNode NODE;			// DGraphNode.
  struct HierarchyComponent_ *CHILD;	// First child of this HierarchyComponent.
  struct HierarchyComponent_ *NEXT;	// Next child in parent HierarchyComponent.
};
#endif//$XCC_h

HierarchyComponent
HierarchyComponent_Ctor(HierarchyComponent this,
                        HierarchyComponentType type, DGraphNode node)
{
  *HierarchyComponent__TYPE(this) = type;
  *HierarchyComponent__DEPTH(this) = -1;
  *HierarchyComponent__NODE(this) = node;
  *HierarchyComponent__CHILD(this) = NULL;
  *HierarchyComponent__NEXT(this) = NULL;
  return this;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(HierarchyComponent_) = %zu\n", sizeof(HierarchyComponent_));
}
#endif//$XCC__c

#ifdef $XCC_h
static inline HierarchyComponentType
HierarchyComponent_type(const_HierarchyComponent this)
{
  return HierarchyComponent_TYPE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline int
HierarchyComponent_depth(const_HierarchyComponent this)
{
  return HierarchyComponent_DEPTH(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline DGraphNode
HierarchyComponent_node(const_HierarchyComponent this)
{
  return HierarchyComponent_NODE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline HierarchyComponent
HierarchyComponent_child(const_HierarchyComponent this)
{
  return HierarchyComponent_CHILD(this);
}
#endif//$XCC_h

#ifdef $XCC_h
static inline HierarchyComponent
HierarchyComponent_next(const_HierarchyComponent this)
{
  return HierarchyComponent_NEXT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
#define HierarchyComponent_CHILDREN_FOREACH_HierarchyComponent(this, component) { \
  HierarchyComponent component = HierarchyComponent_child(this); \
  for (; component != NULL; component = HierarchyComponent_next(component)) {
#define HierarchyComponent_CHILDREN_ENDEACH_HierarchyComponent \
  } \
}
#endif//$XCC_h

static inline void
HierarchyComponent_putContents(HierarchyComponent this, HierarchyComponent child)
{
  *HierarchyComponent__DEPTH(child) = HierarchyComponent_DEPTH(this) + 1;
  *HierarchyComponent__NEXT(child) = HierarchyComponent_CHILD(this);
  *HierarchyComponent__CHILD(this) = child;
}

#ifdef $XCC_h
bool
HierarchyComponent_pretty(const_HierarchyComponent this, FILE *file,
                          bool (*node_pretty)(DGraphNode, FILE*));
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
HierarchyComponent_pretty(const_HierarchyComponent this, FILE *file,
                          bool (*node_pretty)(DGraphNode, FILE*))
{
  HierarchyComponentType type = HierarchyComponent_TYPE(this);
  if (file == NULL) file = stderr;
  if (type == HierarchyComponentType_Node) {
    DGraphNode node = HierarchyComponent_NODE(this);
    if (node_pretty != NULL) (*node_pretty)(node, file);
    else fprintf(file, "%d", DGraphNode_NUMBER(node));
  } else {
    const char *sep = "";
    if (type == HierarchyComponentType_SCC) fprintf(file, "(");
    if (type == HierarchyComponentType_Loop) fprintf(file, "[");
    HierarchyComponent_CHILDREN_FOREACH_HierarchyComponent(this, component) {
      fputs(sep, file);
      HierarchyComponent_pretty(component, file, node_pretty);
      sep = " ";
    } HierarchyComponent_CHILDREN_ENDEACH_HierarchyComponent;
    if (type == HierarchyComponentType_Loop) fprintf(file, "]");
    if (type == HierarchyComponentType_SCC) fprintf(file, ")");
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
void
HierarchyComponent_free(HierarchyComponent this, Memory memory);
#endif//$XCC_h

void
HierarchyComponent_free(HierarchyComponent this, Memory memory)
{
  HierarchyComponent next = HierarchyComponent_NEXT(this);
  HierarchyComponent child = HierarchyComponent_CHILD(this);
  if (child != NULL) HierarchyComponent_free(child, memory);
  if (next != NULL) HierarchyComponent_free(next, memory);
  Memory_free_(memory, this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Hierarchy of Strongly Connected Regions.
 */
struct Hierarchy_ {
  //@args	Memory memory, DGraph dgraph, DGraphNode entry
  Memory MEMORY;
  int32_t COUNT;
  DGraphNode ENTRY;
  int32_t *DFN;			// Array of DFN numbers indexed by DGraphNode_NUMBER.
  DGraphNode *PAST;		// Past the stack pointer.
  HierarchyComponent_ ROOT_;
  //@access ROOTNODE	Hierarchy__ROOT_(this)
};
#endif//$XCC_h

Hierarchy
Hierarchy_Ctor(Hierarchy this, Memory memory, DGraph dgraph, DGraphNode entry)
{
  int32_t count = DGraph_count(dgraph);
  HierarchyComponent root = Hierarchy_ROOTNODE(this);
  *Hierarchy__MEMORY(this) = memory;
  *Hierarchy__COUNT(this) = count;
  *Hierarchy__ENTRY(this) = entry;
  *Hierarchy__DFN(this) = Memory_alloc(memory, sizeof(int32_t*)*(count+1));
  *Hierarchy__PAST(this) = Memory_alloc(memory, sizeof(int32_t*)*count);
  HierarchyComponent_Ctor(root, HierarchyComponentType_Root, NULL);
  return this;
}

void
Hierarchy_Dtor(Hierarchy this)
{
  Memory memory = Hierarchy_MEMORY(this);
  HierarchyComponent root = Hierarchy_ROOTNODE(this);
  HierarchyComponent child = HierarchyComponent_CHILD(root);
  Memory_free_(memory, Hierarchy_DFN(this));
  Memory_free_(memory, Hierarchy_PAST(this));
  *HierarchyComponent__CHILD(root) = NULL;
  if (child != NULL) HierarchyComponent_free(child, memory);
}

size_t
Hierarchy_Size(Memory memory, DGraph dgraph, DGraphNode entry)
{
  return sizeof(Hierarchy_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Hierarchy_) = %zu\n", sizeof(Hierarchy_));
}
#endif//$XCC__c

#ifdef $XCC_h
Hierarchy
Hierarchy_new(Memory parent, DGraph dgraph, DGraphNode entry);
#endif//$XCC_h

Hierarchy
Hierarchy_new(Memory parent, DGraph dgraph, DGraphNode entry)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Hierarchy_Size(memory, dgraph, entry);
  Hierarchy this = Memory_alloc(memory, size);
  return Hierarchy_Ctor(this, memory, dgraph, entry);
}

#ifdef $XCC_h
Hierarchy
Hierarchy_delete(Hierarchy this);
#endif//$XCC_h

Hierarchy
Hierarchy_delete(Hierarchy this)
{
  if (this != NULL) {
    Memory memory = Hierarchy_MEMORY(this);
#ifndef _NDTOR
    Hierarchy_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
static inline HierarchyComponent
Hierarchy_root(Hierarchy this)
{
  return Hierarchy_ROOTNODE(this);
}
#endif//$XCC_h

static inline HierarchyComponent
Hierarchy_makeComponent(Hierarchy this, HierarchyComponentType type, DGraphNode node)
{
  Memory memory = Hierarchy_MEMORY(this);
  HierarchyComponent component = Memory_alloc(memory, sizeof(HierarchyComponent_));
  HierarchyComponent_Ctor(component, type, node);
  return component;
}

#ifdef $XCC_h
bool
Hierarchy_pretty(const_Hierarchy this, FILE *file,
                 bool (*node_pretty)(DGraphNode, FILE*));
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
Hierarchy_pretty(const_Hierarchy this, FILE *file,
                 bool (*node_pretty)(DGraphNode, FILE*))
{
  const_HierarchyComponent root = Hierarchy_ROOTNODE(this);
  if (file == NULL) file = stderr;
  HierarchyComponent_pretty(root, file, node_pretty);
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

/*
 * This is the classic SCC algorithm from Tarjan adapted from F. Bourdoncle's
 * PhD thesis ``Semantique des langages imperatifs d'ordre superieur et interpretation
 * abstraite'', 1992, Figure 3.8 p. 41.
 */
static int32_t
Hierarchy_visitTarjan(Hierarchy this, DGraphNode vertex, HierarchyComponent parent)
{
  int32_t vertex_number = DGraphNode_NUMBER(vertex);
  int32_t *dfn = Hierarchy_DFN(this);
  int32_t head = dfn[vertex_number] = ++dfn[0];
  bool loop = false;
  *Hierarchy_PAST(this) = vertex;
  ++*Hierarchy__PAST(this);
  DGraphNode_FORLEAVE(vertex, DGraphArc_, arc) {
    DGraphNode succ = DGraphArc_HEADNODE(arc);
    int32_t succ_number = DGraphNode_NUMBER(succ), min = 0;
    if (dfn[succ_number] <= 0) {
      min = Hierarchy_visitTarjan(this, succ, parent);
    } else {
      min = dfn[succ_number];
    }
    if (min <= head) {
      head = min;
      loop = true;
    }
  } DGraphNode_ENDLEAVE;
  if (head == dfn[vertex_number]) {
    DGraphNode element = NULL;
    HierarchyComponent component = NULL;
    if (loop) {
      component = Hierarchy_makeComponent(this, HierarchyComponentType_SCC,
                                          vertex);
      HierarchyComponent_putContents(parent, component);
      parent = component;
    }
    do {
      --*Hierarchy__PAST(this);
      element = *Hierarchy_PAST(this);
      dfn[DGraphNode_NUMBER(element)] = INT32_MAX;
      component = Hierarchy_makeComponent(this, HierarchyComponentType_Node,
                                          element);
      HierarchyComponent_putContents(parent, component);
    } while (element != vertex);
  }
  return head;
}

/*
 * This implementation is adapted from F. Bourdoncle's paper, ``Efficient Chaotic
 * Iteration Strategies with Widenings'', Proc. of the International Conference
 * on Formal Methods in Programming and their Applications, Lecture Notes in
 * Computer Science 735, Springer-Verlag 1993. The Tarjan-Bourdoncle algorithm
 * runs in O(de), where d is the maximum loop nesting depth.
 *
 * The algorithm is extended to detect the extra loop entry nodes as proposed by
 * Steensgaard in ``Sequentializing Program Dependence Graphs for Irreducible
 * Programs''. The resulting loop forest is not Steengaard's though, because
 * the SCC root nodes are removed one at a time. In Steensgaard's forest, all
 * the heads of looping arcs are removed before reprocessing the current SCC.
 */
static int32_t
Hierarchy_visitBourdoncle(Hierarchy this, DGraphNode vertex, HierarchyComponent parent)
{
  int32_t vertex_number = DGraphNode_NUMBER(vertex);
  int32_t *dfn = Hierarchy_DFN(this);
  int32_t head = dfn[vertex_number] = ++dfn[0];
  bool loop = false;
  *Hierarchy_PAST(this) = vertex;
  ++*Hierarchy__PAST(this);
  DGraphNode_FORLEAVE(vertex, DGraphArc_, arc) {
    DGraphNode succ = DGraphArc_HEADNODE(arc);
    int32_t succ_number = DGraphNode_NUMBER(succ), min = 0;
    if (dfn[succ_number] <= 0) {
      min = Hierarchy_visitBourdoncle(this, succ, parent);
    } else {
      min = dfn[succ_number];
    }
    if (min <= head) {
      head = min;
      loop = true;
    }
  } DGraphNode_ENDLEAVE;
  if (head == dfn[vertex_number]) {
    DGraphNode element = NULL, *start = NULL;
    DGraphNode *last = Hierarchy_PAST(this) - 1;
    HierarchyComponent component = NULL;
    dfn[vertex_number] = INT32_MAX;
    --*Hierarchy__PAST(this);
    element = *Hierarchy_PAST(this);
    if (loop) {
      int32_t stamp = -1;
      int32_t extraCount = 0;
      HierarchyComponentType type = HierarchyComponentType_Loop;
      while (element != vertex) {
        dfn[DGraphNode_NUMBER(element)] = stamp;
        --*Hierarchy__PAST(this);
        element = *Hierarchy_PAST(this);
      }
      // Traverse the SCC excluding its root to identify other loop entry nodes.
      for (start = Hierarchy_PAST(this) + 1; start <= last; start++) {
        DGraphNode_FORENTER(*start, DGraphArc_, arc) {
          DGraphNode pred = DGraphArc_TAILNODE(arc);
          int32_t pred_number = DGraphNode_NUMBER(pred);
          if (pred != vertex && dfn[pred_number] != stamp) {
            ++extraCount;
            break;
          }
        } DGraphNode_ENDENTER;
      }
      if (extraCount > 0) type = HierarchyComponentType_SCC;
      component = Hierarchy_makeComponent(this, type, vertex);
      HierarchyComponent_putContents(parent, component);
      parent = component;
      DGraphNode_FORLEAVE(vertex, DGraphArc_, arc) {
        DGraphNode succ = DGraphArc_HEADNODE(arc);
        int32_t succ_number = DGraphNode_NUMBER(succ);
        if (dfn[succ_number] <= 0) {
          Hierarchy_visitBourdoncle(this, succ, parent);
        }
      } DGraphNode_ENDLEAVE;
    }
    component = Hierarchy_makeComponent(this, HierarchyComponentType_Node,
                                        element);
    HierarchyComponent_putContents(parent, component);
  }
  return head;
}

#ifdef $XCC_h
/**
 * Make a Hierarchy of Strongly Connected Components.
 */
void
Hierarchy_makeSCC(Hierarchy this);
#endif//$XCC_h

void
Hierarchy_makeSCC(Hierarchy this)
{
  int32_t count = Hierarchy_COUNT(this);
  DGraphNode entry = Hierarchy_ENTRY(this);
  HierarchyComponent root = Hierarchy_ROOTNODE(this);
  Memory memory = Hierarchy_MEMORY(this);
  HierarchyComponent child = HierarchyComponent_CHILD(root);
  if (child != NULL) HierarchyComponent_free(child, memory);
  HierarchyComponent_Ctor(root, HierarchyComponentType_Root, NULL);
  memset(Hierarchy_DFN(this), 0, sizeof(int32_t)*(count+1));
  Hierarchy_visitTarjan(this, entry, Hierarchy_ROOTNODE(this));
}

#ifdef $XCC_h
/**
 * Make a Hierarchy of Nested Strongly Connected Components.
 *
 * The Nested Strongly Connected Components is a Loop Forest as defined by
 * G. Ramalingam, ``Identifying loops in almost linear time'', Transactions on
 * Programming Languages and Systems, Volume 21, Number 2, March 1999. F.
 * Bourdoncle introduced this decomposition in his PhD thesis in order to
 * minimize the number of widening points in a control-flow graph.
 */
void
Hierarchy_makeNSCC(Hierarchy this);
#endif//$XCC_h

void
Hierarchy_makeNSCC(Hierarchy this)
{
  int32_t count = Hierarchy_COUNT(this);
  DGraphNode entry = Hierarchy_ENTRY(this);
  HierarchyComponent root = Hierarchy_ROOTNODE(this);
  Memory memory = Hierarchy_MEMORY(this);
  HierarchyComponent child = HierarchyComponent_CHILD(root);
  if (child != NULL) HierarchyComponent_free(child, memory);
  HierarchyComponent_Ctor(root, HierarchyComponentType_Root, NULL);
  memset(Hierarchy_DFN(this), 0, sizeof(int32_t)*(count+1));
  Hierarchy_visitBourdoncle(this, entry, Hierarchy_ROOTNODE(this));
}

#ifdef $XCC__c
// Test DGraph_makeNSCC.
{
  DGraph_ dgraph[1];
  DGraph_Ctor(dgraph, Memory_Root, 100);
  {
    // Example taken from F. Bourdoncle's PhD thesis ``Semantique des langages
    // imperatifs d'ordre superieur et interpretation abstraite'', 1992, Figure
    // 3.6 at page 39.
    Hierarchy_ hierarchy[1];
    DGraphNode node_1 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_2 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_3 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_4 = DGraph_makeNode(dgraph, 0);
    DGraph_makeArc(dgraph, node_1, node_3, 0);
    DGraph_makeArc(dgraph, node_1, node_2, 0);
    DGraph_makeArc(dgraph, node_2, node_4, 0);
    DGraph_makeArc(dgraph, node_2, node_3, 0);
    DGraph_makeArc(dgraph, node_3, node_4, 0);
    DGraph_makeArc(dgraph, node_3, node_2, 0);
    Hierarchy_Ctor(hierarchy, Memory_Root, dgraph, node_1);
    printf("Irreducible:\n");
    printf("SCC:\t");
    Hierarchy_makeSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    printf("NSCC:\t");
    Hierarchy_makeNSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    Hierarchy_Dtor(hierarchy);
  }
  DGraph_Dtor(dgraph);
}
#endif//$XCC__c

#ifdef $XCC__c
// Test DGraph_makeNSCC.
{
  DGraph_ dgraph[1];
  DGraph_Ctor(dgraph, Memory_Root, 100);
  {
    // Example taken from F. Bourdoncle's PhD thesis ``Semantique des langages
    // imperatifs d'ordre superieur et interpretation abstraite'', 1992, Figure
    // 3.5 at page 39.
    Hierarchy_ hierarchy[1];
    DGraphNode node_1 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_2 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_3 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_4 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_5 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_6 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_7 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_8 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_9 = DGraph_makeNode(dgraph, 0);
    DGraph_makeArc(dgraph, node_1, node_5, 0);
    DGraph_makeArc(dgraph, node_1, node_4, 0);
    DGraph_makeArc(dgraph, node_1, node_2, 0);
    DGraph_makeArc(dgraph, node_2, node_4, 0);
    DGraph_makeArc(dgraph, node_2, node_3, 0);
    DGraph_makeArc(dgraph, node_4, node_4, 0);
    DGraph_makeArc(dgraph, node_5, node_6, 0);
    DGraph_makeArc(dgraph, node_6, node_7, 0);
    DGraph_makeArc(dgraph, node_7, node_9, 0);
    DGraph_makeArc(dgraph, node_7, node_8, 0);
    DGraph_makeArc(dgraph, node_7, node_6, 0);
    DGraph_makeArc(dgraph, node_8, node_8, 0);
    DGraph_makeArc(dgraph, node_9, node_7, 0);
    DGraph_makeArc(dgraph, node_9, node_5, 0);
    Hierarchy_Ctor(hierarchy, Memory_Root, dgraph, node_1);
    printf("Bourdoncle figure 3.5:\n");
    printf("SCC:\t");
    Hierarchy_makeSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    printf("NSCC:\t");
    Hierarchy_makeNSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    Hierarchy_Dtor(hierarchy);
  }
  DGraph_Dtor(dgraph);
}
#endif//$XCC__c

#ifdef $XCC__c
// Test DGraph_makeNSCC.
{
  DGraph_ dgraph[1];
  DGraph_Ctor(dgraph, Memory_Root, 100);
  {
    // Example taken from F. Bourdoncle's paper ``Efficient Chaotic Iteration
    // Strategies with Widenings'', Figure 2.
    Hierarchy_ hierarchy[1];
    DGraphNode node_1 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_2 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_3 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_4 = DGraph_makeNode(dgraph, 0);
    DGraph_makeArc(dgraph, node_1, node_2, 0);
    DGraph_makeArc(dgraph, node_1, node_4, 0);
    DGraph_makeArc(dgraph, node_2, node_3, 0);
    DGraph_makeArc(dgraph, node_3, node_1, 0);
    DGraph_makeArc(dgraph, node_4, node_3, 0);
    Hierarchy_Ctor(hierarchy, Memory_Root, dgraph, node_1);
    printf("Bourdoncle figure 2:\n");
    printf("SCC:\t");
    Hierarchy_makeSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    printf("NSCC:\t");
    Hierarchy_makeNSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    Hierarchy_Dtor(hierarchy);
  }
  DGraph_Dtor(dgraph);
}
#endif//$XCC__c

#ifdef $XCC__c
// Test DGraph_makeNSCC.
{
  DGraph_ dgraph[1];
  DGraph_Ctor(dgraph, Memory_Root, 100);
  {
    Hierarchy_ hierarchy[1];
    // Example taken from F. Bourdoncle's paper ``Efficient Chaotic Iteration
    // Strategies with Widenings'', Figure 5.
    DGraphNode node_1 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_2 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_3 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_4 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_5 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_6 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_7 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_8 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_9 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_10 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_11 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_12 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_13 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_14 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_15 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_16 = DGraph_makeNode(dgraph, 0);
    DGraph_makeArc(dgraph, node_1, node_4, 0);
    DGraph_makeArc(dgraph, node_1, node_2, 0);
    DGraph_makeArc(dgraph, node_2, node_3, 0);
    DGraph_makeArc(dgraph, node_3, node_6, 0);
    DGraph_makeArc(dgraph, node_4, node_11, 0);
    DGraph_makeArc(dgraph, node_5, node_6, 0);
    DGraph_makeArc(dgraph, node_6, node_15, 0);
    DGraph_makeArc(dgraph, node_11, node_14, 0);
    DGraph_makeArc(dgraph, node_14, node_1, 0);
    DGraph_makeArc(dgraph, node_11, node_12, 0);
    DGraph_makeArc(dgraph, node_12, node_13, 0);
    DGraph_makeArc(dgraph, node_13, node_16, 0);
    DGraph_makeArc(dgraph, node_16, node_5, 0);
    DGraph_makeArc(dgraph, node_15, node_16, 0);
    Hierarchy_Ctor(hierarchy, Memory_Root, dgraph, node_1);
    printf("Bourdoncle figure 5:\n");
    printf("SCC:\t");
    Hierarchy_makeSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    printf("NSCC:\t");
    Hierarchy_makeNSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    Hierarchy_Dtor(hierarchy);
  }
  DGraph_Dtor(dgraph);
}
#endif//$XCC__c

#ifdef $XCC__c
// Test DGraph_makeNSCC.
{
  DGraph_ dgraph[1];
  DGraph_Ctor(dgraph, Memory_Root, 100);
  {
    Hierarchy_ hierarchy[1];
    // Example taken from G. Ramalingam's report ``On Loops, Dominators, and
    // Dominance Frontiers'', figure 1.
    DGraphNode node_1 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_2 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_3 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_4 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_5 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_6 = DGraph_makeNode(dgraph, 0);
    DGraph_makeArc(dgraph, node_1, node_3, 0);
    DGraph_makeArc(dgraph, node_1, node_2, 0);
    DGraph_makeArc(dgraph, node_2, node_4, 0);
    DGraph_makeArc(dgraph, node_3, node_5, 0);
    DGraph_makeArc(dgraph, node_4, node_6, 0);
    DGraph_makeArc(dgraph, node_4, node_5, 0);
    DGraph_makeArc(dgraph, node_4, node_2, 0);
    DGraph_makeArc(dgraph, node_5, node_6, 0);
    DGraph_makeArc(dgraph, node_5, node_4, 0);
    DGraph_makeArc(dgraph, node_5, node_3, 0);
    Hierarchy_Ctor(hierarchy, Memory_Root, dgraph, node_1);
    printf("Ramalingam figure 1:\n");
    printf("SCC:\t");
    Hierarchy_makeSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    printf("NSCC:\t");
    Hierarchy_makeNSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    Hierarchy_Dtor(hierarchy);
  }
  DGraph_Dtor(dgraph);
}
#endif//$XCC__c

#ifdef $XCC__c
// Test DGraph_makeNSCC.
{
  DGraph_ dgraph[1];
  DGraph_Ctor(dgraph, Memory_Root, 100);
  {
    Hierarchy_ hierarchy[1];
    DGraphNode node_1 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_2 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_3 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_4 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_5 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_6 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_7 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_8 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_9 = DGraph_makeNode(dgraph, 0);
    DGraphNode node_10 = DGraph_makeNode(dgraph, 0);
    DGraph_makeArc(dgraph, node_1, node_2, 0);
    DGraph_makeArc(dgraph, node_2, node_6, 0);
    DGraph_makeArc(dgraph, node_2, node_5, 0);
    DGraph_makeArc(dgraph, node_2, node_3, 0);
    DGraph_makeArc(dgraph, node_3, node_5, 0);
    DGraph_makeArc(dgraph, node_3, node_4, 0);
    DGraph_makeArc(dgraph, node_5, node_5, 0);
    DGraph_makeArc(dgraph, node_6, node_7, 0);
    DGraph_makeArc(dgraph, node_7, node_8, 0);
    DGraph_makeArc(dgraph, node_8, node_10, 0);
    DGraph_makeArc(dgraph, node_8, node_9, 0);
    DGraph_makeArc(dgraph, node_8, node_7, 0);
    DGraph_makeArc(dgraph, node_9, node_9, 0);
    DGraph_makeArc(dgraph, node_10, node_8, 0);
    DGraph_makeArc(dgraph, node_10, node_6, 0);
    Hierarchy_Ctor(hierarchy, Memory_Root, dgraph, node_1);
    printf("Quicksort:\n");
    printf("SCC:\t");
    Hierarchy_makeSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    printf("NSCC:\t");
    Hierarchy_makeNSCC(hierarchy);
    Hierarchy_pretty(hierarchy, stdout, NULL);
    Hierarchy_Dtor(hierarchy);
  }
  DGraph_Dtor(dgraph);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
  Hierarchy_INIT -- Initialize the Hierarchy module.
 */
#define Hierarchy_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/*
  Hierarchy_FINI -- Finalize the Hierarchy module.
 */
#define Hierarchy_FINI()
#endif//$XCC__h

#if XCC__C
static void
Hierarchy_TEST(void)
{
#include "CAL/Hierarchy_.c"
}

int
main(int argc, char **argv)
{
  CAL_Test(Hierarchy_TEST);
  return 0;
}
#endif

