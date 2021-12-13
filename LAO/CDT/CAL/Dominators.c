#ifdef $XCC_h
/*
 * !!!!	Dominators.xcc
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
 * @brief Dominators on DGraph (Lengauer Tarjan).
 *
 * @warning These do not seem to be used in LAO ! Obsolete ? (Florent)
 */
#endif//$XCC_h


#include "CAL/CAL_.h"

#ifdef $XCC__h
#include "CAL/Dominators.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Maintain dominance information.
 */
struct Dominators_;
#endif//$XCC_h

#ifdef $XCC__h
struct Dominators_ {
  //@args	Memory memory, DGraph dgraph, DGraphNode entry, DGraphNode exit
  Memory MEMORY;
  DGraph DGRAPH;
  DGraphNode ENTRY;
  DGraphNode EXIT;
  int32_t COUNT;		//!< Number of nodes in the graph.
  DGraphNode *NODEOF;		//!< Maps node number to node.
  IETree_ *BUCKET;		//!< Lengauer-Tarjan temporary.
  int32_t *PARENT;		//!< Lengauer-Tarjan temporary.
  int32_t *ANCESTOR;		//!< Lengauer-Tarjan temporary.
  int32_t *CHILD;		//!< Lengauer-Tarjan temporary.
  int32_t *LABEL;		//!< Lengauer-Tarjan temporary.
  int32_t *SEMI;		//!< Lengauer-Tarjan temporary.
  int32_t *SIZE;		//!< Lengauer-Tarjan temporary.
  int32_t NDFS;			//!< Number of DFS reached nodes.
  int32_t *VERTEX;		//!< Lengauer-Tarjan result: DFS number to node number.
  int32_t *IDOM;		//!< Immediate dominator. 
                                //!  Lengauer-Tarjan result: node number to IDOM node
                		//!  number. Set to 0 for entry and unreachable nodes.
  IETree_ *DOMS;		//!< Successors dominated.
                                //!  Set of nodes whose Dominators_::IDOM is this node.
  int32_t *DOMN;		//!< Dominance DFS Number.
                                //!  DFS number of node in the dominator tree.
  int32_t *DOML;		//!< Max DFS number of the node children.
  IETree_ *DOMF;		//!< The dominance frontier of this node.
};
#endif//$XCC__h

Dominators
Dominators_Ctor(Dominators this,
                Memory memory, DGraph dgraph, DGraphNode entry, DGraphNode exit)
{
  *Dominators__MEMORY(this) = memory;
  *Dominators__DGRAPH(this) = dgraph;
  *Dominators__ENTRY(this) = entry;
  *Dominators__EXIT(this) = exit;
  *Dominators__COUNT(this) = DGraph_count(dgraph);
  *Dominators__NODEOF(this) = DGraph_nodes(dgraph) - 1;
  *Dominators__BUCKET(this) = NULL;
  *Dominators__PARENT(this) = NULL;
  *Dominators__ANCESTOR(this) = NULL;
  *Dominators__CHILD(this) = NULL;
  *Dominators__LABEL(this) = NULL;
  *Dominators__SEMI(this) = NULL;
  *Dominators__SIZE(this) = NULL;
  *Dominators__NDFS(this) = 0;
  *Dominators__VERTEX(this) = NULL;
  *Dominators__IDOM(this) = NULL;
  *Dominators__DOMS(this) = NULL;
  *Dominators__DOMN(this) = NULL;
  *Dominators__DOML(this) = NULL;
  *Dominators__DOMF(this) = NULL;
  {
    DGraphNode entry = Dominators_ENTRY(this);
    DGraphNode exit = Dominators_EXIT(this);
    IETree_ *BUCKET = Dominators_BUCKET(this);
    int32_t *PARENT = Dominators_PARENT(this);
    int32_t *ANCESTOR = Dominators_ANCESTOR(this);
    int32_t *CHILD = Dominators_CHILD(this);
    int32_t *LABEL = Dominators_LABEL(this);
    int32_t *SEMI = Dominators_SEMI(this);
    int32_t *SIZE = Dominators_SIZE(this);
    int32_t COUNT = Dominators_COUNT(this), r, v;
    if (entry != NULL) r = DGraphNode_NUMBER(entry);
    else if (exit != NULL) r = DGraphNode_NUMBER(exit);
    else return this;
    // Allocate the Lengauer-Tarjan temporary arrays.
    BUCKET = *Dominators__BUCKET(this) = (IETree_ *)malloc(COUNT*sizeof(IETree_)) - 1;
    for (v = 1; v <= COUNT; v++) IETree_Ctor((IETree)(BUCKET + v), Memory_Root);
    PARENT = *Dominators__PARENT(this) = (int32_t *)malloc(COUNT*sizeof(int32_t)) - 1;
    ANCESTOR = *Dominators__ANCESTOR(this) = (int32_t *)malloc(COUNT*sizeof(int32_t)) - 1;
    CHILD = *Dominators__CHILD(this) = (int32_t *)malloc(COUNT*sizeof(int32_t)) - 1;
    LABEL = *Dominators__LABEL(this) = (int32_t *)malloc((COUNT + 1)*sizeof(int32_t));
    SEMI = *Dominators__SEMI(this) = (int32_t *)malloc((COUNT + 1)*sizeof(int32_t));
    SIZE = *Dominators__SIZE(this) = (int32_t *)malloc((COUNT + 1)*sizeof(int32_t));
    // Call the Lengauer-Tarjan algorithm.
    Dominators_DOMINATORS(this, r);
    // Complete the Dominators results.
    Dominators_complete(this, r);
#ifdef XCC__C
    {
      DGraphNode *NODEOF = Dominators_NODEOF(this);
      int32_t *PARENT = Dominators_PARENT(this);
      int32_t *SEMI = Dominators_SEMI(this);
      int32_t *IDOM = Dominators_IDOM(this);
      printf("NODE\tPARENT\tSEMI\tIDOM\n");
      DGraph_FOREACH(Dominators_DGRAPH(this), DGraphNode_, node) {
        int32_t index = DGraphNode_INDEX(node);
        if (index > 0) {
          int32_t number = DGraphNode_NUMBER(node);
          int32_t parent = DGraphNode_INDEX(NODEOF[PARENT[number]]);
          int32_t semi = DGraphNode_INDEX(NODEOF[SEMI[number]]);
          int32_t idom = DGraphNode_INDEX(NODEOF[IDOM[number]]);
          printf("%d\t%d\t%d\t%d\n", index, parent, semi, idom);
        }
      } DGraph_ENDEACH;
    }
#endif
    // Release the Lengauer-Tarjan temporary arrays.
    free(SIZE), *Dominators__SIZE(this) = NULL;
    free(SEMI), *Dominators__SEMI(this) = NULL;
    free(LABEL), *Dominators__LABEL(this) = NULL;
    free(CHILD + 1); *Dominators__CHILD(this) = NULL;
    free(ANCESTOR + 1); *Dominators__ANCESTOR(this) = NULL;
    free(PARENT + 1); *Dominators__PARENT(this) = NULL;
    for (v = 1; v <= COUNT; v++) {
      IETree_Dtor((IETree)(BUCKET + v));
    }
    free(BUCKET + 1); *Dominators__BUCKET(this) = NULL;
  }
  return this;
}

void
Dominators_Dtor(Dominators this)
{
  Memory memory = Dominators_MEMORY(this);
  int32_t count = Dominators_COUNT(this), v;
  if (Dominators_VERTEX(this) != NULL) {
    Memory_free(memory, Dominators_VERTEX(this) + 1), *Dominators__VERTEX(this) = NULL;
  }
  if (Dominators_IDOM(this) != NULL) {
    Memory_free(memory, Dominators_IDOM(this) + 1), *Dominators__IDOM(this) = NULL;
  }
  if (Dominators_DOMS(this) != NULL) {
    for (v = 1; v <= count; v++) {
      IETree_Dtor((IETree)(Dominators_DOMS(this) + v));
    }
    Memory_free(memory, Dominators_DOMS(this) + 1), *Dominators__DOMS(this) = NULL;
  }
  if (Dominators_DOMN(this) != NULL) {
    Memory_free(memory, Dominators_DOMN(this) + 1), *Dominators__DOMN(this) = NULL;
  }
  if (Dominators_DOML(this) != NULL) {
    Memory_free(memory, Dominators_DOML(this) + 1), *Dominators__DOML(this) = NULL;
  }
  if (Dominators_DOMF(this) != NULL) {
    for (v = 1; v <= count; v++) {
      IETree_Dtor((IETree)(Dominators_DOMF(this) + v));
    }
    Memory_free(memory, Dominators_DOMF(this) + 1), *Dominators__DOMF(this) = NULL;
  }
}

size_t
Dominators_Size(Memory memory, DGraph dgraph, DGraphNode entry, DGraphNode exit)
{
  return sizeof(Dominators_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Dominators_)\t%zu\n", sizeof(Dominators_));
}
#endif//$XCC__c

#ifdef $XCC_h
Dominators
Dominators_new(Memory parent, DGraph dgraph, DGraphNode entry, DGraphNode exit);
#endif//$XCC_h

Dominators
Dominators_new(Memory parent, DGraph dgraph, DGraphNode entry, DGraphNode exit)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Dominators_Size(memory, dgraph, entry, exit);
  Dominators this = Memory_alloc(memory, size);
  return Dominators_Ctor(this, memory, dgraph, entry, exit);
}

#ifdef $XCC_h
Dominators
Dominators_delete(Dominators this);
#endif//$XCC_h

Dominators
Dominators_delete(Dominators this)
{
  if (this != NULL) {
    Memory memory = Dominators_MEMORY(this);
#ifndef _NDTOR
    Dominators_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * This Dominators NODEOF array.
 */
DGraphNode *
Dominators_nodeOf(Dominators this);
#endif//$XCC_h

DGraphNode *
Dominators_nodeOf(Dominators this)
{
  return DGraph_nodeOf(Dominators_DGRAPH(this));
}

/*
 * See ``A Fast Algorithm for Finding Dominators in a Flowgraph'' by Lengauer and
 * Tarjan, ACM TOPLAS Vol.1 , No. 1, July 1979. The implementation below is
 * verbatim from the paper, which is needed to understand the algorithm. --	BD3
 */
static void
Dominators_DFS(Dominators this, int32_t v, int32_t *ndfs)
{
  DGraphNode entry = Dominators_ENTRY(this);
  DGraphNode exit = Dominators_EXIT(this);
  DGraphNode *NODEOF = Dominators_NODEOF(this);
  int32_t *PARENT = Dominators_PARENT(this);
  int32_t *ANCESTOR = Dominators_ANCESTOR(this);
  int32_t *CHILD = Dominators_CHILD(this);
  int32_t *LABEL = Dominators_LABEL(this);
  int32_t *SEMI = Dominators_SEMI(this);
  int32_t *SIZE = Dominators_SIZE(this);
  int32_t *VERTEX = Dominators_VERTEX(this);
  //
  SEMI[v] = ++*ndfs;
  VERTEX[*ndfs] = LABEL[v] = v;
  ANCESTOR[v] = CHILD[v] = 0;
  SIZE[v] = 1;
  if (entry != NULL) {
    DGraphNode_FORLEAVE(NODEOF[v], DGraphArc_, arc) {
      DGraphNode w_node = DGraphArc_HEADNODE(arc);
      int32_t w = DGraphNode_NUMBER(w_node);
      if (SEMI[w] == 0) {
        PARENT[w] = v;
        Dominators_DFS(this, w, ndfs);
      }
    } DGraphNode_ENDLEAVE;
  } else if (exit != NULL) {
    DGraphNode_FORENTER(NODEOF[v], DGraphArc_, arc) {
      DGraphNode w_node = DGraphArc_TAILNODE(arc);
      int32_t w = DGraphNode_NUMBER(w_node);
      if (SEMI[w] == 0) {
        PARENT[w] = v;
        Dominators_DFS(this, w, ndfs);
      }
    } DGraphNode_ENDENTER;
  }
}

/*
 * See ``A Fast Algorithm for Finding Dominators in a Flowgraph'' by Lengauer and
 * Tarjan, ACM TOPLAS Vol.1 , No. 1, July 1979. The implementation below is
 * verbatim from the paper, which is needed to understand the algorithm. --	BD3
 */
static inline void
Dominators_COMPRESS(Dominators this, int32_t v)
{
  int32_t *ANCESTOR = Dominators_ANCESTOR(this);
  int32_t *LABEL = Dominators_LABEL(this);
  int32_t *SEMI = Dominators_SEMI(this);
  //
  if (ANCESTOR[ANCESTOR[v]] != 0) {
    Dominators_COMPRESS(this, ANCESTOR[v]);
    if (SEMI[LABEL[ANCESTOR[v]]] < SEMI[LABEL[v]]) {
      LABEL[v] = LABEL[ANCESTOR[v]];
    }
    ANCESTOR[v] = ANCESTOR[ANCESTOR[v]];
  }
}

/*
 * See ``A Fast Algorithm for Finding Dominators in a Flowgraph'' by Lengauer and
 * Tarjan, ACM TOPLAS Vol.1 , No. 1, July 1979. The implementation below is
 * verbatim from the paper, which is needed to understand the algorithm. --	BD3
 */
static inline int32_t
Dominators_EVAL(Dominators this, int32_t v)
{
  int32_t *ANCESTOR = Dominators_ANCESTOR(this);
  int32_t *LABEL = Dominators_LABEL(this);
  int32_t *SEMI = Dominators_SEMI(this);
  //
#ifdef Dominators_SIMPLE
  if (ANCESTOR[v] == 0) return v;
  Dominators_COMPRESS(this, v);
  return LABEL[v];
#else
  if (ANCESTOR[v] == 0) return LABEL[v];
  Dominators_COMPRESS(this, v);
  if (SEMI[LABEL[ANCESTOR[v]]] >= SEMI[LABEL[v]]) return LABEL[v];
  return LABEL[ANCESTOR[v]];
#endif
}

/*
 * See ``A Fast Algorithm for Finding Dominators in a Flowgraph'' by Lengauer and
 * Tarjan, ACM TOPLAS Vol.1 , No. 1, July 1979. The implementation below is
 * verbatim from the paper, which is needed to understand the algorithm. --	BD3
 */
static inline void
Dominators_LINK(Dominators this, int32_t v, int32_t w)
{
  int32_t *ANCESTOR = Dominators_ANCESTOR(this);
  int32_t *CHILD = Dominators_CHILD(this);
  int32_t *LABEL = Dominators_LABEL(this);
  int32_t *SEMI = Dominators_SEMI(this);
  int32_t *SIZE = Dominators_SIZE(this);
  int32_t s = w;
#ifdef Dominators_SIMPLE
  ANCESTOR[w] = v;
#else
  while (SEMI[LABEL[w]] < SEMI[LABEL[CHILD[s]]]) {
    if (SIZE[s] + SIZE[CHILD[CHILD[s]]] >= 2*SIZE[CHILD[s]]) {
      ANCESTOR[CHILD[s]] = s;
      CHILD[s] = CHILD[CHILD[s]];
    } else {
      SIZE[CHILD[s]] = SIZE[s];
      s = ANCESTOR[s] = CHILD[s];
    }
  }
  LABEL[s] = LABEL[w];
  SIZE[v] += SIZE[w];
  if (SIZE[v] < 2*SIZE[w]) {
    int32_t t = s;
    s = CHILD[v];
    CHILD[v] = t;
  }
  while (s != 0) {
    ANCESTOR[s] = v;
    s = CHILD[s];
  }
#endif
}

#ifdef $XCC__h
/**
 * The core Lengauer-Tarjan algorithm.
 *
 * See ``A Fast Algorithm for Finding Dominators in a Flowgraph'' by Lengauer and
 * Tarjan, ACM TOPLAS Vol.1 , No. 1, July 1979. The implementation below is
 * verbatim from the paper, which is needed to understand the algorithm. --	BD3
 */
void
Dominators_DOMINATORS(Dominators this, int32_t r);
#endif//$XCC__h

void
Dominators_DOMINATORS(Dominators this, int32_t r)
{
  Memory memory = Dominators_MEMORY(this);
  DGraphNode entry = Dominators_ENTRY(this);
  DGraphNode exit = Dominators_EXIT(this);
  DGraphNode *NODEOF = Dominators_NODEOF(this);
  IETree_ *BUCKET = Dominators_BUCKET(this);
  int32_t *PARENT = Dominators_PARENT(this);
  int32_t *LABEL = Dominators_LABEL(this);
  int32_t *SEMI = Dominators_SEMI(this);
  int32_t *SIZE = Dominators_SIZE(this);
  int32_t NDFS = Dominators_NDFS(this);
  int32_t *VERTEX = Dominators_VERTEX(this);
  int32_t *IDOM = Dominators_IDOM(this);
  int32_t COUNT = Dominators_COUNT(this), v, i;
  // Allocate the Lengauer-Tarjan result arrays.
  VERTEX = *Dominators__VERTEX(this)
         = (int32_t *)Memory_alloc(memory, COUNT*sizeof(int32_t)) - 1;
  memset(VERTEX + 1, 0, COUNT*sizeof(int32_t));
  IDOM = *Dominators__IDOM(this)
       = (int32_t *)Memory_alloc(memory, COUNT*sizeof(int32_t)) - 1;
  memset(IDOM + 1, 0, COUNT*sizeof(int32_t));
  // Step 1 of the Lengauer-Tarjan algorithm.
  for (v = 1; v <= COUNT; v++) SEMI[v] = 0;
  *Dominators__NDFS(this) = 0;
  Dominators_DFS(this, r, Dominators__NDFS(this));
  NDFS = Dominators_NDFS(this);
  SIZE[0] = LABEL[0] = SEMI[0] = 0;
  for (i = NDFS; i >= 2; --i) {
    int32_t w = VERTEX[i];
    // Step 2 of the Lengauer-Tarjan algorithm.
    DGraphNode w_node = NODEOF[w];
    if (entry != NULL) {
      DGraphNode_FORENTER(w_node, DGraphArc_, arc) {
        DGraphNode v_node = DGraphArc_TAILNODE(arc);
        int32_t v = DGraphNode_NUMBER(v_node);
        int32_t u;
        if (SEMI[v] == 0) continue;	/* Skip unvisited. */
        u = Dominators_EVAL(this, v);
        if (SEMI[u] < SEMI[w]) SEMI[w] = SEMI[u];
      } DGraphNode_ENDENTER;
    } else if (exit != NULL) {
      DGraphNode_FORLEAVE(w_node, DGraphArc_, arc) {
        DGraphNode v_node = DGraphArc_HEADNODE(arc);
        int32_t v = DGraphNode_NUMBER(v_node);
        int32_t u;
        if (SEMI[v] == 0) continue;	/* Skip unvisited. */
        u = Dominators_EVAL(this, v);
        if (SEMI[u] < SEMI[w]) SEMI[w] = SEMI[u];
      } DGraphNode_ENDLEAVE;
    }
    IETree_insert((IETree)(BUCKET + VERTEX[SEMI[w]]), (IETreeMember)w);
    Dominators_LINK(this, PARENT[w], w);
    // Step 3 of the Lengauer-Tarjan algorithm.
    while (!IETree_isEmpty((IETree)(BUCKET + PARENT[w]))) {
      int32_t v = (int32_t)IETree_choose((IETree)(BUCKET + PARENT[w]));
      int32_t u = Dominators_EVAL(this, v);
      IDOM[v] = SEMI[u] < SEMI[v]? u: PARENT[w];
    }
  }
  // Step 4 of the Lengauer-Tarjan algorithm.
  for (i = 2; i <= NDFS; i++) {
    int32_t w = VERTEX[i];
    if (IDOM[w] != VERTEX[SEMI[w]]) {
      IDOM[w] = IDOM[IDOM[w]];
    }
  }
  IDOM[r] = 0;
}

#ifdef $XCC__h
/**
 * Make the Dominators_::DOMN and the Dominators_::DOML indices from the Dominators_::DOMS sets.
 */
void
Dominators_DOMDFS(Dominators this, int32_t v, int32_t *ndfs);
#endif//$XCC__h

void
Dominators_DOMDFS(Dominators this, int32_t v, int32_t *ndfs)
{
  IETree_ *DOMS = Dominators_DOMS(this);
  int32_t *DOMN = Dominators_DOMN(this);
  int32_t *DOML = Dominators_DOML(this);
  //
  DOMN[v] = ++*ndfs;
  IETree_FOREACH((IETree)(DOMS + v), iter) {
    int32_t w = (int32_t)iter;
    Dominators_DOMDFS(this, w, ndfs);
  } IETree_ENDEACH;
  DOML[v] = *ndfs;
}

#ifdef $XCC__h
/**
 * Complete this Dominators results.
 */
void
Dominators_complete(Dominators this, int32_t r);
#endif//$XCC__h

void
Dominators_complete(Dominators this, int32_t r)
{
  Memory memory = Dominators_MEMORY(this);
  int32_t COUNT = Dominators_COUNT(this);
  int32_t NDFS = Dominators_NDFS(this);
  int32_t *VERTEX = Dominators_VERTEX(this);
  int32_t *IDOM = Dominators_IDOM(this);
  IETree_ *DOMS = Dominators_DOMS(this);
  int32_t *DOMN = Dominators_DOMN(this);
  int32_t *DOML = Dominators_DOML(this);
  int32_t ndfs = 0, v, i;
  // Allocate the DOMS, DOMN, DOML arrays.
  DOMS = *Dominators__DOMS(this)
       = (IETree_ *)Memory_alloc(memory, COUNT*sizeof(IETree_)) - 1;
  for (v = 1; v <= COUNT; v++) IETree_Ctor((IETree)(Dominators_DOMS(this) + v), memory);
  DOMN = *Dominators__DOMN(this)
       = (int32_t *)Memory_alloc(memory, COUNT*sizeof(int32_t)) - 1;
  memset(DOMN + 1, 0, COUNT*sizeof(int32_t));
  DOML = *Dominators__DOML(this)
       = (int32_t *)Memory_alloc(memory, COUNT*sizeof(int32_t)) - 1;
  memset(DOML + 1, 0, COUNT*sizeof(int32_t));
  // Make the DOMSets from the IDOM.
  for (i = 2; i <= NDFS; i++) {
    int32_t w = VERTEX[i];
    IETree_insert((IETree)(DOMS + IDOM[w]), (IETreeMember)w);
  }
  // Call the Dominators_DOMDFS.
  Dominators_DOMDFS(this, r, &ndfs);
}

#ifdef $XCC_h
/**
 * Return the IDOM of a DGraphNode.
 */
int32_t
Dominators_nodeIDOM(Dominators this, DGraphNode node);
#endif//$XCC_h

int32_t
Dominators_nodeIDOM(Dominators this, DGraphNode node)
{
  int32_t number = DGraphNode_NUMBER(node);
  Except_CHECK(DGraphNode_NUMBER(Dominators_NODEOF(this)[number]) == number);
  return Dominators_IDOM(this)[number];
}

#ifdef $XCC_h
/**
 * Return the DOMS IETree of a DGraphNode.
 */
IETree
Dominators_nodeDOMS(Dominators this, DGraphNode node);
#endif//$XCC_h

IETree
Dominators_nodeDOMS(Dominators this, DGraphNode node)
{
  int32_t number = DGraphNode_NUMBER(node);
  Except_CHECK(DGraphNode_NUMBER(Dominators_NODEOF(this)[number]) == number);
  return (IETree)(Dominators_DOMS(this) + number);
}

#ifdef $XCC_h
/**
 * Iterate over a DGraphNode DOMS DGraphNodes.
 */
#define Dominators_DOMS_FOREACH(this, node, Type, succ) { \
  DGraphNode *Dominators_NODEOF = Dominators_nodeOf(this); \
  IETree Dominators_NODEDOMS = Dominators_nodeDOMS(this, node); \
  IETree_FOREACH(Dominators_NODEDOMS, Dominators_ITER) { \
    Type *(succ) = (Type *)Dominators_NODEOF[(int32_t)Dominators_ITER];
#define Dominators_DOMS_ENDEACH \
  } IETree_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Return the DOMN of a DGraphNode.
 */
int32_t
Dominators_nodeDOMN(Dominators this, DGraphNode node);
#endif//$XCC_h

int32_t
Dominators_nodeDOMN(Dominators this, DGraphNode node)
{
  int32_t number = DGraphNode_NUMBER(node);
  Except_CHECK(DGraphNode_NUMBER(Dominators_NODEOF(this)[number]) == number);
  return Dominators_DOMN(this)[number];
}

#ifdef $XCC_h
/**
 * True is this Dominators has DOMF.
 */
bool
Dominators_hasDOMF(Dominators this);
#endif//$XCC_h

bool
Dominators_hasDOMF(Dominators this)
{
  return Dominators_DOMF(this) != NULL;
}

#ifdef $XCC_h
/**
 * Return the DOMF IETree of a DGraphNode.
 */
IETree
Dominators_nodeDOMF(Dominators this, DGraphNode node);
#endif//$XCC_h

IETree
Dominators_nodeDOMF(Dominators this, DGraphNode node)
{
  int32_t number = DGraphNode_NUMBER(node);
  Except_CHECK(DGraphNode_NUMBER(Dominators_NODEOF(this)[number]) == number);
  return (IETree)(Dominators_DOMF(this) + number);
}

#ifdef $XCC_h
/**
 * Iterate over a DGraphNode DOMF DGraphNodes.
 */
#define Dominators_DOMF_FOREACH(this, node, Type, succ) { \
  DGraphNode *Dominators_NODEOF = Dominators_nodeOf(this); \
  IETree Dominators_NODEDOMF = Dominators_nodeDOMF(this, node); \
  IETree_FOREACH(Dominators_NODEDOMF, Dominators_ITER) { \
    Type *(succ) = (Type *)Dominators_NODEOF[(int32_t)Dominators_ITER];
#define Dominators_DOMF_ENDEACH \
  } IETree_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Test dominance relation between two DGraphNodes.
 *
 * Application of the (not so) well-known properties of DFS numbering.
 */
bool
Dominators_dominates(Dominators this, DGraphNode this_node, DGraphNode that_node);
#endif//$XCC_h

bool
Dominators_dominates(Dominators this, DGraphNode this_node, DGraphNode that_node)
{
  int32_t *DOMN = Dominators_DOMN(this);
  int32_t *DOML = Dominators_DOML(this);
  int32_t this_number = DGraphNode_NUMBER(this_node);
  int32_t that_number = DGraphNode_NUMBER(that_node);
  Except_CHECK(DOMN[this_number] != 0 && DOMN[that_number] != 0);
  return DOMN[this_number] <= DOMN[that_number] &&
      DOMN[that_number] <= DOML[this_number];
}

#ifdef $XCC_h
/**
 * Make the dominance frontiers.
 *
 * Implementation from "A Simple, Fast Dominators Algorithm", Software Practice
 * and Experience, Vol 4, 2001, by K. D. Cooper, T. J. Harvey, K. Kennedy.
 */
void
Dominators_makeFrontiers(Dominators this);
#endif//$XCC_h

void
Dominators_makeFrontiers(Dominators this)
{
  Memory memory = Dominators_MEMORY(this);
  DGraphNode entry = Dominators_ENTRY(this);
  DGraphNode exit = Dominators_EXIT(this);
  DGraphNode *NODEOF = Dominators_NODEOF(this);
  int32_t NDFS = Dominators_NDFS(this);
  int32_t *VERTEX = Dominators_VERTEX(this);
  int32_t *IDOM = Dominators_IDOM(this);
  IETree_ *DOMF = Dominators_DOMF(this);
  int32_t COUNT = Dominators_COUNT(this), i, v;
  if (Dominators_DOMF(this) != NULL) return;
  // Allocate and initialize the DOMF array.
  DOMF = *Dominators__DOMF(this)
       = (IETree_ *)Memory_alloc(memory, COUNT*sizeof(IETree_)) - 1;
  for (v = 1; v <= COUNT; v++) IETree_Ctor((IETree)(Dominators_DOMF(this) + v), memory);
  if (entry != NULL) {
    // for all nodes b
    for (i = 1; i <= NDFS; i++) {
      int32_t b = VERTEX[i];
      DGraphNode b_node = NODEOF[b];
      DGraphArc arc = DGraphNode_ENTERARC(b_node);
      // if number of predecessors of b >= 2
      if (arc != NULL && DGraphArc_ENTERARC(arc) != NULL) {
        // for all predecessors p of b
        DGraphNode_FORENTER(b_node, DGraphArc_, arc) {
          DGraphNode p_node = DGraphArc_TAILNODE(arc);
          // runner <- p
          int32_t p = DGraphNode_NUMBER(p_node), r = p;
          if (IDOM[r] == 0) continue;	/* Skip unvisited. */
          // while runner != doms[b]
          while (r != IDOM[b]) {
            // add b to runner's dominance frontier set
            IETree_insert((IETree)(DOMF + r), (IETreeMember)b);
            // runner <- doms[runner]
            r = IDOM[r];
          }
        } DGraphNode_ENDENTER;
      }
    }
  } else if (exit != NULL) {
    // for all nodes b
    for (i = 1; i <= NDFS; i++) {
      int32_t b = VERTEX[i];
      DGraphNode b_node = NODEOF[b];
      DGraphArc arc = DGraphNode_LEAVEARC(b_node);
      // if number of successors of b >= 2
      if (arc != NULL && DGraphArc_LEAVEARC(arc) != NULL) {
        // for all successors p of b
        DGraphNode_FORLEAVE(b_node, DGraphArc_, arc) {
          DGraphNode p_node = DGraphArc_HEADNODE(arc);
          // runner <- p
          int32_t p = DGraphNode_NUMBER(p_node), r = p;
          if (IDOM[r] == 0) continue;	/* Skip unvisited. */
          // while runner != doms[b]
          while (r != IDOM[b]) {
            // add b to runner's dominance frontier set
            IETree_insert((IETree)(DOMF + r), (IETreeMember)b);
            // runner <- doms[runner]
            r = IDOM[r];
          }
        } DGraphNode_ENDLEAVE;
      }
    }
  }
}

#ifdef $XCC_h
/**
 * Pretty-print this Dominators.
 */
bool
Dominators_pretty(Dominators this, FILE *file);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
bool
Dominators_pretty(Dominators this, FILE *file)
{
  DGraph dgraph = Dominators_DGRAPH(this);
  if (file == NULL) file = stderr;
  DGraph_FOREACH(dgraph, DGraphNode_, node) {
    int32_t number = DGraphNode_NUMBER(node);
    printf("node_%d:", DGraphNode_INDEX(node));
    printf("\tIDOM=%d", Dominators_IDOM(this)[number]);
    printf("\tDOMN=%d", Dominators_DOMN(this)[number]);
    printf("\tDOML=%d", Dominators_DOML(this)[number]);
    printf("\tDOMS={");
    Dominators_DOMS_FOREACH(this, node, DGraphNode_, dom_node) {
      printf(" node_%d", DGraphNode_INDEX(dom_node));
    } Dominators_DOMS_ENDEACH;
    if (Dominators_DOMF(this) != NULL) {
      printf(" }");
      printf("\tDOMF={");
      Dominators_DOMF_FOREACH(this, node, DGraphNode_, dom_node) {
        printf(" node_%d", DGraphNode_INDEX(dom_node));
      } Dominators_DOMF_ENDEACH;
    }
    printf(" }\n");
  } DGraph_ENDEACH;
  return true;
}
#endif//_NTRACE

#ifdef $XCC__c
// Test forward Dominators_makeFrontiers.
{
  DGraph_ dgraph_;
  DGraph dgraph = DGraph_Ctor(&dgraph_, Memory_Root, 20);
  // Example taken from "Computing Dominators and Dominators Frontiers" by
  // Preston Briggs and Tim Harvey, Technical Report, June 1994.
  DGraphNode node_0 = DGraph_makeNode(dgraph, 0);
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
  DGraph_makeArc(dgraph, node_0, node_1, 0);
  DGraph_makeArc(dgraph, node_1, node_11, 0);
  DGraph_makeArc(dgraph, node_1, node_8, 0);
  DGraph_makeArc(dgraph, node_1, node_2, 0);
  DGraph_makeArc(dgraph, node_2, node_6, 0);
  DGraph_makeArc(dgraph, node_2, node_3, 0);
  DGraph_makeArc(dgraph, node_3, node_4, 0);
  DGraph_makeArc(dgraph, node_4, node_5, 0);
  DGraph_makeArc(dgraph, node_5, node_4, 0);
  DGraph_makeArc(dgraph, node_5, node_1, 0);
  DGraph_makeArc(dgraph, node_6, node_7, 0);
  DGraph_makeArc(dgraph, node_6, node_4, 0);
  DGraph_makeArc(dgraph, node_7, node_4, 0);
  DGraph_makeArc(dgraph, node_8, node_12, 0);
  DGraph_makeArc(dgraph, node_8, node_11, 0);
  DGraph_makeArc(dgraph, node_8, node_9, 0);
  DGraph_makeArc(dgraph, node_9, node_10, 0);
  DGraph_makeArc(dgraph, node_10, node_9, 0);
  DGraph_makeArc(dgraph, node_10, node_5, 0);
  DGraph_makeArc(dgraph, node_11, node_12, 0);
  DGraph_makeArc(dgraph, node_12, node_13, 0);
  DGraph_makeArc(dgraph, node_13, node_10, 0);
  printf("Dominators Forward:\n");
  {
    Dominators_ dominance[1];
    Dominators_Ctor(dominance, Memory_Root, dgraph, node_0, NULL);
    Dominators_makeFrontiers(dominance);
    Dominators_pretty(dominance, stdout);
    Dominators_Dtor(dominance);
  }
  DGraph_Dtor(dgraph);
}
#endif//$XCC__c

#ifdef $XCC__c
// Test backward Dominators_makeFrontiers.
{
  DGraph_ dgraph_;
  DGraph dgraph = DGraph_Ctor(&dgraph_, Memory_Root, 20);
  // Example taken from "Computing Dominators and Dominators Frontiers" by
  // Preston Briggs and Tim Harvey, Technical Report, June 1994.
  DGraphNode node_0 = DGraph_makeNode(dgraph, 0);
  DGraphNode node_1 = DGraph_makeNode(dgraph, 0);
  DGraphNode node_2 = DGraph_makeNode(dgraph, 0);
  DGraphNode node_3 = DGraph_makeNode(dgraph, 0);
  DGraphNode node_4 = DGraph_makeNode(dgraph, 0);
  DGraphNode node_5 = DGraph_makeNode(dgraph, 0);
  DGraphNode node_6 = DGraph_makeNode(dgraph, 0);
  DGraph_makeArc(dgraph, node_1, node_6, 0);
  DGraph_makeArc(dgraph, node_1, node_2, 0);
  DGraph_makeArc(dgraph, node_2, node_4, 0);
  DGraph_makeArc(dgraph, node_2, node_3, 0);
  DGraph_makeArc(dgraph, node_3, node_5, 0);
  DGraph_makeArc(dgraph, node_4, node_5, 0);
  DGraph_makeArc(dgraph, node_5, node_6, 0);
  DGraph_makeArc(dgraph, node_6, node_0, 0);
  printf("Dominators Backward:\n");
  {
    Dominators_ dominance[1];
    Dominators_Ctor(dominance, Memory_Root, dgraph, NULL, node_0);
    Dominators_makeFrontiers(dominance);
    Dominators_pretty(dominance, stdout);
    Dominators_Dtor(dominance);
  }
  DGraph_Dtor(dgraph);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Dominators module.
 */
#define Dominators_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Dominators module.
 */
#define Dominators_FINI()
#endif//$XCC__h

#if XCC__C
static void
Dominators_TEST(void)
{
#include "CAL/Dominators_.c"
}

int
main(int argc, char **argv)
{
  CAL_Test(Dominators_TEST);
  return 0;
}
#endif

