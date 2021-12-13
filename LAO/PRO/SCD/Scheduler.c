#ifdef $XCC_h
/*
 * !!!!	Scheduler.xcc
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
 * @ingroup SCD
 * @brief Instruction Scheduler driver.
 */
#endif//$XCC_h


#include "SCD/SCD_.h"

#ifdef $XCC__h
#include "SCD/Scheduler.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Base scheduler class for a BlockTrace.
 */
struct Scheduler_;
  //@args	Memory memory, BlockTrace blockTrace
#endif//$XCC_h

#ifdef $XCC__h
struct Scheduler_ {
  Memory MEMORY;
  BlockTrace BLOCKTRACE;	// The BlockTrace to schedule.
  //@access isInnerLoop	BlockTrace_isInnerLoop(Scheduler_BLOCKTRACE(this))
  //@access isPrePass	BlockTrace_isPrePass(Scheduler_BLOCKTRACE(this))
  //@access isPipeline	BlockTrace_isPipeline(Scheduler_BLOCKTRACE(this))
  //@access OPTIMIZE	BlockTrace_OPTIMIZE(Scheduler_BLOCKTRACE(this))
  //@access REGIONTYPE	BlockTrace_REGIONTYPE(Scheduler_BLOCKTRACE(this))
  //@access HEADBLOCK	BlockTrace_HEADBLOCK(Scheduler_BLOCKTRACE(this))
  //@access ISSUEITEMS	BlockTrace_ISSUEITEMS(Scheduler_BLOCKTRACE(this))
  //@access ITEMCOUNT	BlockTrace_ITEMCOUNT(Scheduler_BLOCKTRACE(this))
  //@access FIRSTITEM	BlockTrace_FIRSTITEM(Scheduler_BLOCKTRACE(this))
  //@access LASTITEM	BlockTrace_LASTITEM(Scheduler_BLOCKTRACE(this))
  //@access ITEMCOUNT	BlockTrace_ITEMCOUNT(Scheduler_BLOCKTRACE(this))
  //@access PROCESSOR	BlockTrace_PROCESSOR(Scheduler_BLOCKTRACE(this))
  //@access LAMBDA	BlockTrace_LAMBDA(Scheduler_BLOCKTRACE(this))
  //@mutate LAMBDA	BlockTrace__LAMBDA(Scheduler_BLOCKTRACE(this))
  //@access LENGTH	BlockTrace_LENGTH(Scheduler_BLOCKTRACE(this))
  //@mutate LENGTH	BlockTrace__LENGTH(Scheduler_BLOCKTRACE(this))
  //@access COMPENSATION	BlockTrace_COMPENSATION(Scheduler_BLOCKTRACE(this))
  //@access SPECULATION	BlockTrace_SPECULATION(Scheduler_BLOCKTRACE(this))
  //@access RELAXATION	BlockTrace_RELAXATION(Scheduler_BLOCKTRACE(this))
  //@access PIPELINING	BlockTrace_PIPELINING(Scheduler_BLOCKTRACE(this))
  //@access RENAMING	BlockTrace_RENAMING(Scheduler_BLOCKTRACE(this))
  //@access BOOSTING	BlockTrace_BOOSTING(Scheduler_BLOCKTRACE(this))
  //@access FACTOR	BlockTrace_FACTOR(Scheduler_BLOCKTRACE(this))
  //@access SOLVING	BlockTrace_SOLVING(Scheduler_BLOCKTRACE(this))
  //@access TIMEOUT	BlockTrace_TIMEOUT(Scheduler_BLOCKTRACE(this))
  //@access LOGTIME	BlockTrace_LOGTIME(Scheduler_BLOCKTRACE(this))
  //@access KUNROLL	BlockTrace_KUNROLL(Scheduler_BLOCKTRACE(this))
  //@access OVERLAP	BlockTrace_OVERLAP(Scheduler_BLOCKTRACE(this))
  String HEADNAME;		// Label name of this BlockTrace HEADBLOCK.
  int32_t IDENTITY;		// Identity of this BlockTrace HEADBLOCK.
  GrahamList GRAHAMLIST;	// The GrahamList of this Scheduler.
  //@access INDEX2TASK	GrahamList_INDEX2TASK(Scheduler_GRAHAMLIST(this))
  //@access ISSUETABLE	GrahamList_ISSUETABLE(Scheduler_GRAHAMLIST(this))
  int32_t FIRSTINDEX;		// The first GrahamListTask INDEX.
  int32_t LASTINDEX;		// The last GrahamListTask INDEX.
  DDGraph DDGRAPHS[DDGraphKind__];	// The scheduling graphs of this Scheduler.
  //@access EARLYGRAPH	 (Scheduler_DDGRAPHS(this)[DDGraphKind_Early])
  //@access LATEGRAPH	 (Scheduler_DDGRAPHS(this)[DDGraphKind_Late])
  //@access MCRLGRAPH	 (Scheduler_DDGRAPHS(this)[DDGraphKind_MCRL])
  DDGraphArc *LINKARCS[RegFile__COUNT];	// Link arcs heading into the lifetime nodes.
  int16_t MAXLIVES[RegFile__COUNT];	// This Scheduler MaxLive per RegFile.
  int16_t *PRESSURES[RegFile__COUNT];	// This Scheduler pressure per RegFile.
  int32_t SERIALIZE;		// Counter of serialization arcs.
  int16_t LAMBDAS[3];		// Lambda resource, lambda recurrence, lambda relaxation.
  //@access DELTAS	(Scheduler_LAMBDA(this) - Scheduler_LAMBDAS(this)[2])
  int8_t BAROMEGA;		// Maximum of the OMEGA in the DDGraph(s).
  int8_t UNWINDING;		// The Relaxation unwinding level.
  int16_t BODYCOUNT;
  int16_t SCHEDULED;
  int16_t DELTALAMBDA;
  int16_t DELTALENGTH;
};
#endif//$XCC__h

Scheduler
Scheduler_Ctor(Scheduler this,
               Memory memory, BlockTrace blockTrace)
{
  CFGNode headNode = BlockTrace_HEADNODE(blockTrace);
  BasicBlock headBlock = CFGNode_basicBlock(headNode);
  Label headLabel = BasicBlock_makeLabel(headBlock);
  RegFile regFile = RegFile__UNDEF;
  DDGraphKind kind = DDGraphKind__;
  *Scheduler__MEMORY(this) = memory;
  *Scheduler__BLOCKTRACE(this) = blockTrace;
  *Scheduler__HEADNAME(this) = Label_name(headLabel);
  *Scheduler__IDENTITY(this) = BasicBlock_identity(headBlock);
  *Scheduler__GRAHAMLIST(this) = NULL;
  *Scheduler__FIRSTINDEX(this) = 0;
  *Scheduler__LASTINDEX(this) = 0;
  for (kind = 0; kind < DDGraphKind__; kind++) {
    Scheduler_DDGRAPHS(this)[kind] = NULL;
  }
  for (regFile = 0; regFile < RegFile__Count; regFile++) {
    Scheduler_LINKARCS(this)[regFile] = NULL;
  }
  for (regFile = 0; regFile < RegFile__Count; regFile++) {
    Scheduler_MAXLIVES(this)[regFile] = -1;
  }
  for (regFile = 0; regFile < RegFile__Count; regFile++) {
    Scheduler_PRESSURES(this)[regFile] = NULL;
  }
  *Scheduler__SERIALIZE(this) = 0;
  Scheduler_LAMBDAS(this)[0] = 0;
  Scheduler_LAMBDAS(this)[1] = 0;
  Scheduler_LAMBDAS(this)[2] = 0;
  *Scheduler__BAROMEGA(this) = 0;
  *Scheduler__UNWINDING(this) = 0;
  *Scheduler__BODYCOUNT(this) = 0;
  *Scheduler__SCHEDULED(this) = 0;
  *Scheduler__DELTALAMBDA(this) = 0;
  *Scheduler__DELTALENGTH(this) = 0;
  return this;
}

void
Scheduler_Dtor(Scheduler this)
{
  DDGraphKind kind = DDGraphKind__;
  RegFile regFile = RegFile__UNDEF;
  Memory memory = Scheduler_MEMORY(this);
  GrahamList_delete(Scheduler_GRAHAMLIST(this));
  for (kind = 0; kind < DDGraphKind__; kind++) {
    if (Scheduler_DDGRAPHS(this)[kind] != NULL) {
      DDGraph_delete(Scheduler_DDGRAPHS(this)[kind]);
    }
  }
  for (regFile = 0; regFile < RegFile__Count; regFile++) {
    Memory_free(memory, Scheduler_LINKARCS(this)[regFile]);
  }
  for (regFile = 0; regFile < RegFile__Count; regFile++) {
    Memory_free(memory, Scheduler_PRESSURES(this)[regFile]);
  }
}

size_t
Scheduler_Size(Memory memory, BlockTrace blockTrace)
{
  return sizeof(Scheduler_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Scheduler_)\t%zu\n", sizeof(Scheduler_));
}
#endif//$XCC__c

#ifdef $XCC_h
Scheduler
Scheduler_new(Memory parent, BlockTrace blockTrace);
#endif//$XCC_h

Scheduler
Scheduler_new(Memory parent, BlockTrace blockTrace)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Scheduler_Size(memory, blockTrace);
  Scheduler this = Memory_alloc(memory, size);
  return Scheduler_Ctor(this, memory, blockTrace);
}

#ifdef $XCC_h
Scheduler
Scheduler_delete(Scheduler this);
#endif//$XCC_h

Scheduler
Scheduler_delete(Scheduler this)
{
  if (this != NULL) {
    Memory memory = Scheduler_MEMORY(this);
#ifndef _NDTOR
    Scheduler_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
int
Scheduler_bodyCount(const_Scheduler this);
#endif//$XCC_h

int
Scheduler_bodyCount(const_Scheduler this)
{
  return Scheduler_BODYCOUNT(this);
}

#ifdef $XCC_h
int
Scheduler_scheduled(const_Scheduler this);
#endif//$XCC_h

int
Scheduler_scheduled(const_Scheduler this)
{
  return Scheduler_SCHEDULED(this);
}

#ifdef $XCC_h
/**
 * For use by Scheduler_FOREACH_IssueItem.
 */
IDList
Scheduler_issueItems(Scheduler this);
#endif//$XCC_h

IDList
Scheduler_issueItems(Scheduler this)
{
  return Scheduler_ISSUEITEMS(this);
}

#ifdef $XCC_h
/**
 * Iterate over this Scheduler issueItems.
 */
#define Scheduler_FOREACH_IssueItem(this, issueItem) { \
  IDList Scheduler_ISSUEITEMS = Scheduler_issueItems(this); \
  IDList_FOREACH(Scheduler_ISSUEITEMS, IssueItem_, issueItem)
#define Scheduler_ENDEACH_IssueItem \
  IDList_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use by Scheduler_LINKARCS_FOREACH_DDGraphArc.
 */
DDGraphArc **
Scheduler_linkArcs(Scheduler this);
#endif//$XCC_h

DDGraphArc **
Scheduler_linkArcs(Scheduler this)
{
  return Scheduler_LINKARCS(this);
}

#ifdef $XCC_h
/**
 * Iterate over this Scheduler LINKARCS.
 */
#define Scheduler_LINKARCS_FOREACH_DDGraphArc(this, regFile, arc) { \
  DDGraphArc *Scheduler_LINKARCS = Scheduler_linkArcs(this)[regFile]; \
  int32_t Scheduler_ARCINDEX = 0; \
  for (; Scheduler_LINKARCS[Scheduler_ARCINDEX] != NULL; Scheduler_ARCINDEX++) { \
    DDGraphArc arc = Scheduler_LINKARCS[Scheduler_ARCINDEX];
#define Scheduler_LINKARCS_ENDEACH_DDGraphArc \
  } \
}
#endif//$XCC_h

#ifdef $XCC__h
/**
 * Pretty-print this Scheduler.
 */
bool
Scheduler_pretty(Scheduler this, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
bool
Scheduler_pretty(Scheduler this, FILE *file)
{
  int16_t lambda = Scheduler_LAMBDA(this);
  if (file == NULL) file = stderr;
  Scheduler_FOREACH_IssueItem(this, item) {
    fprintf(file, "[%d]\t", IssueItem_STARTDATE(item));
    IssueItem_pretty(item, file);
  } Scheduler_ENDEACH_IssueItem;
  return true;
}
#endif//_NTRACE

#ifdef $XCC__h
/**
 * Build a DDGraph of the specified DDGraphKind.
 */
DDGraph
Scheduler_buildDDGraph(Scheduler this, DDGraph thatGraph, DDGraphKind kind, DDGraphFlags flags);
#endif//$XCC__h

DDGraph
Scheduler_buildDDGraph(Scheduler this, DDGraph thatGraph, DDGraphKind kind, DDGraphFlags flags)
{
  Memory memory = Scheduler_MEMORY(this);
  int32_t serialize = Scheduler_SERIALIZE(this);
  int32_t itemCount = Scheduler_ITEMCOUNT(this);
  BlockTrace blockTrace = Scheduler_BLOCKTRACE(this);
  CodeRegion codeRegion = BlockTrace_CODEREGION(blockTrace);
  Processor processor = BlockTrace_PROCESSOR(blockTrace);
  int32_t nodeCount = kind == DDGraphKind_MCRL? 2*itemCount + 4: itemCount + 4;
  DDGraph ddgraph = DDGraph_new(memory, kind, flags, processor, nodeCount, Scheduler__LAMBDA(this));
  Scheduler_DDGRAPHS(this)[kind] = ddgraph;
  DDGraph_enterNodes(ddgraph, blockTrace);
  if (thatGraph == NULL) {
    int overlap = BlockTrace_OVERLAP(blockTrace);
    DDGraph_enterArcs(ddgraph, blockTrace);
    *Scheduler__BAROMEGA(this) = DDGraph_BAROMEGA(ddgraph);
    *Scheduler__UNWINDING(this) = Scheduler_BAROMEGA(this)
                                + (FORCE_UNWINDING >= 0? FORCE_UNWINDING: overlap);
    SCD_TRACE && fprintf(SCD_TRACE, "*** BAROMEGA=%d\tUNWINDING=%d\n",
                                    Scheduler_BAROMEGA(this), Scheduler_UNWINDING(this));
  } else {
    DDGraph_copyArcs(ddgraph, thatGraph);
  }
  DDGraph_complete(ddgraph, Scheduler_OVERLAP(this), serialize, processor);
  SCD_TRACE && fprintf(SCD_TRACE, "*** DDGRAPH %s\n", DDGraphKind_Id(kind));
  SCD_TRACE && GETENV("SCD_DDGRAPH") && DDGraph_pretty(ddgraph, SCD_TRACE);
  return ddgraph;
}

#ifdef $XCC__h
/**
 * Build this Scheduler LINKARCS.
 *
 * Each LINKARCS entry is a NULL-terminated array of DDGraphArcs. There is one such
 * entry per RegFile.
 */
void
Scheduler_buildLinkArcs(Scheduler this, DDGraph mcrlGraph);
#endif//$XCC__h

void
Scheduler_buildLinkArcs(Scheduler this, DDGraph mcrlGraph)
{
  Memory memory = Scheduler_MEMORY(this);
  DDGraphNode *ddgNodes = DDGraph_nodes(mcrlGraph);
  IssueItem firstItem = Scheduler_FIRSTITEM(this);
  IssueItem lastItem = Scheduler_LASTITEM(this);
  int32_t lastIndex = IssueItem_INDEX(lastItem);
  int32_t nodeCount = DDGraph_nodeCount(mcrlGraph);
  int32_t linkCount = nodeCount - lastIndex;
  size_t linkArcsSize = linkCount*sizeof(DDGraphArc);
  int32_t linkCounts[RegFile__COUNT];
  int32_t life_index = 0;
  RegFile regFile = 0;
  for (regFile = 0; regFile < RegFile__Count; regFile++) {
    linkCounts[regFile] = 0;
    Scheduler_LINKARCS(this)[regFile] = Memory_alloc(memory, linkArcsSize);
    memset(Scheduler_LINKARCS(this)[regFile], 0, linkArcsSize);
  }
  // Loop over the lifetime nodes.
  for (life_index = lastIndex + 1; life_index < nodeCount; life_index++) {
    DDGraphNode life_node = ddgNodes[life_index];
    DDGraphNode_FORENTER_DDGraphArc(life_node, ARC) {
      if (DDGraphArc_KIND(ARC) == DependenceKind_Link) {
        // Arc between producer node and lifetime node.
        Temporary temporary = DDGraphArc_TEMPORARY(ARC);
        RegFile regFile = Temporary_REGFILE(temporary);
        // Set the LINKARCS[regFile][] entry.
        DDGraphArc *linkArcs = Scheduler_LINKARCS(this)[regFile];
        Except_CHECK(regFile != RegFile__UNDEF);
        linkArcs[linkCounts[regFile]++] = ARC;
      }
    } DDGraphNode_ENDENTER_DDGraphArc;
  }
}

#ifdef $XCC__h
/**
 * Compute the register pressures of the schedule.
 *
 * This computation is based on the scheduling dates of the lifetime nodes,
 * identified by the LINKARCS. Lifetimes that have no such nodes are not accounted for.
 */
void
Scheduler_computePressures(Scheduler this);
#endif//$XCC__h

void
Scheduler_computePressures(Scheduler this)
{
  Memory memory = Scheduler_MEMORY(this);
  int16_t lambda = Scheduler_LAMBDA(this);
  int16_t *maxLives = Scheduler_MAXLIVES(this);
  RegFile regFile = RegFile__UNDEF; int date = 0;
  for (regFile = RegFile__UNDEF + 1; regFile < RegFile__Count; regFile++) {
    int16_t *pressure = Memory_alloc(memory, sizeof(int16_t)*lambda);
    memset(pressure, 0, sizeof(int16_t)*lambda);
    SCD_TRACE && fprintf(SCD_TRACE, "*** PRESSURE %s:", RegFile_name(regFile));
    Scheduler_LINKARCS_FOREACH_DDGraphArc(this, regFile, ARC) {
      DDGraphNode prod_node = DDGraphArc_TAIL(ARC);
      DDGraphNode life_node = DDGraphArc_HEAD(ARC);
      int prod_date = DDGraphNode_DATE(prod_node);
      int life_date = DDGraphNode_DATE(life_node);
      for (date = prod_date; date < life_date; date++) ++pressure[date%lambda];
    } Scheduler_LINKARCS_ENDEACH_DDGraphArc;
    for (date = 0; date < lambda; date++) {
      if (maxLives[regFile] < pressure[date]) maxLives[regFile] = pressure[date];
      SCD_TRACE && fprintf(SCD_TRACE, "%3d", pressure[date]);
    }
    Scheduler_PRESSURES(this)[regFile] = pressure;
    SCD_TRACE && fprintf(SCD_TRACE, "\tMAXLIVE=%d\n", maxLives[regFile]);
  }
}

#ifdef $XCC__h
/**
 * Try the Forward and the Backward Relaxations.
 * Return:	true if the relaxations are feasible, else false.
 */
bool
Scheduler_tryRelaxations(Scheduler this, int lambda,
                         int issueCount, const Resource *resources);
#endif//$XCC__h

bool
Scheduler_tryRelaxations(Scheduler this, int lambda,
                         int issueCount, const Resource *resources)
{
  bool feasible = true;
  Memory memory = Scheduler_MEMORY(this);
  int unwinding = 0;//Scheduler_UNWINDING(this);
  GrahamList grahamList = Scheduler_GRAHAMLIST(this);
  BlockTrace blockTrace = Scheduler_BLOCKTRACE(this);
  Relaxation forward_relaxation = Relaxation_new(memory, RelaxationKind_Forward,
                                                 grahamList, unwinding);
  Relaxation backward_relaxation = Relaxation_new(memory, RelaxationKind_Backward,
                                                  grahamList, unwinding);
  Except_CHECK(GrahamList_COUNT(grahamList) == BlockTrace_ITEMCOUNT(blockTrace));
  if (Scheduler_LAMBDA(this) != lambda) {
    *Scheduler__LAMBDA(this) = lambda;
    Scheduler_optimizeFlows(this, false, true);
  }
  if (FORCE_LLPALEVEL == 1) {
    Relaxation_enterGrahamList(forward_relaxation, grahamList);
    Relaxation_readDates(forward_relaxation);
    SCD_TRACE && fprintf(SCD_TRACE, "*** CANDIDATE LAMBDA=%d\n", lambda);
    feasible = Relaxation_testFeasibility(forward_relaxation, issueCount, resources);
    if (feasible) {
      Relaxation_writeDates(forward_relaxation);
      //SCD_TRACE && fprintf(SCD_TRACE, "*** ")
                //&& Relaxation_pretty(forward_relaxation, SCD_TRACE);
    }
  } else if (FORCE_LLPALEVEL == 2) {
    Relaxation_enterGrahamList(backward_relaxation, grahamList);
    Relaxation_readDates(backward_relaxation);
    SCD_TRACE && fprintf(SCD_TRACE, "*** CANDIDATE LAMBDA=%d\n", lambda);
    feasible = Relaxation_testFeasibility(backward_relaxation, issueCount, resources);
    if (feasible) {
      Relaxation_writeDates(backward_relaxation);
      //SCD_TRACE && fprintf(SCD_TRACE, "*** ")
                //&& Relaxation_pretty(backward_relaxation, SCD_TRACE);
    }
  } else /* FORCE_LLPALEVEL == 3 */ {
    Relaxation_enterGrahamList(forward_relaxation, grahamList);
    Relaxation_enterGrahamList(backward_relaxation, grahamList);
    Relaxation_readDates(forward_relaxation);
    Relaxation_readDates(backward_relaxation);
    SCD_TRACE && fprintf(SCD_TRACE, "*** CANDIDATE LAMBDA=%d\n", lambda);
    feasible = Relaxation_testFeasibility(forward_relaxation, issueCount, resources);
    if (feasible) {
      Relaxation_writeDates(forward_relaxation);
      Relaxation_readDates(backward_relaxation);
      feasible = Relaxation_testFeasibility(backward_relaxation, issueCount, resources);
      if (feasible) {
        Relaxation_writeDates(backward_relaxation);
        Relaxation_readDates(forward_relaxation);
        //SCD_TRACE && fprintf(SCD_TRACE, "*** ")
                  //&& Relaxation_pretty(forward_relaxation, SCD_TRACE);
        //SCD_TRACE && fprintf(SCD_TRACE, "*** ")
                  //&& Relaxation_pretty(backward_relaxation, SCD_TRACE);
      }
    }
  }
  Relaxation_delete(backward_relaxation);
  Relaxation_delete(forward_relaxation);
  return feasible;
}

#ifdef $XCC__h
/**
 * Run the Forward and the Backward Relaxations.
 *
 * We use a dichotomy search to find the minimum delta such that the relaxation
 * succeeds a initiation interval lambda + delta.
 */
void
Scheduler_runRelaxations(Scheduler this, int issueCount, const Resource *resources);
#endif//$XCC__h

void
Scheduler_runRelaxations(Scheduler this, int issueCount, const Resource *resources)
{
  int lambda_min = Scheduler_LAMBDA(this);
  if (!Scheduler_tryRelaxations(this, lambda_min, issueCount, resources)) {
    bool feasible = true;
    int  prev_delta = 0, delta = 1;
    // Increase delta until Relaxation is feasible at LAMBDA = lambda_min + delta.
    while (!Scheduler_tryRelaxations(this, lambda_min + delta, issueCount, resources)) {
      prev_delta = delta, delta <<= 1;
    }
    // Here lambda_min + delta is feasible, and lambda_min + prev_delta is infeasible.
    // Dichotomy search between prev_delta + 1 and delta - 1.
    if (prev_delta + 1 <= delta - 1) {
      int l = prev_delta + 1, r = delta - 1;
      while (r >= l) {
        int i = (l + r) >> 1;
        feasible = Scheduler_tryRelaxations(this, lambda_min + i, issueCount, resources);
        if (!feasible) {
          l = i + 1;
        } else {
          delta = i;
          r = i - 1;
        }
      }
      if (!feasible) {
        feasible = Scheduler_tryRelaxations(this, lambda_min + delta, issueCount, resources);
      }
    }
    // Because Scheduler_tryRelaxations calls Relaxation_writeDates, the last call must be feasible.
    Except_ENSURE(feasible);
  }
}

#ifdef $XCC__h
/**
 * Prepare the scheduling problem.
 */
void
Scheduler_prepareScheduling(Scheduler this);
#endif//$XCC__h

void
Scheduler_prepareScheduling(Scheduler this)
{
  int unwinding = 0;//Scheduler_UNWINDING(this);
  DDGraph mcrlGraph = Scheduler_MCRLGRAPH(this);
  Processor processor = Scheduler_PROCESSOR(this);
  const_ResourceVector vector = Processor_availability(processor);
  const uint8_t *availability = ResourceVector__available((ResourceVector)vector, 1) - 1;
  GrahamList grahamList = GrahamList_new(Scheduler_MEMORY(this), mcrlGraph, availability);
  IssueTable issueTable = GrahamList_ISSUETABLE(grahamList);
  *Scheduler__GRAHAMLIST(this) = grahamList;
  // Enter this IssueItem in the grahamList (and the issueTable).
  Scheduler_FOREACH_IssueItem(this, ITEM) {
    GrahamList_enterIssueItem(grahamList, ITEM, unwinding);
  } Scheduler_ENDEACH_IssueItem;
  *Scheduler__FIRSTINDEX(this) = GrahamListTask_INDEX(GrahamList_FIRSTTASK(grahamList));
  *Scheduler__LASTINDEX(this) = GrahamListTask_INDEX(GrahamList_LASTTASK(grahamList));
  SCD_TRACE && fprintf(SCD_TRACE, "*** FIRSTINDEX=%d LASTINDEX=%d\n",
                       Scheduler_FIRSTINDEX(this), Scheduler_LASTINDEX(this));
  SCD_TRACE && IssueTable_prettyCounters(issueTable, SCD_TRACE);
  // Estimate LAMBDA resources from the IssueTable.
  *Scheduler__LAMBDA(this) = IssueTable_estimateLambda(issueTable);
  Scheduler_LAMBDAS(this)[0] = Scheduler_LAMBDA(this);
  SCD_TRACE && fprintf(SCD_TRACE, "*** LAMBDA RESOURCE=%d\n", Scheduler_LAMBDA(this));
  // Solve the initial flow problems to get LAMBDA recurrences.
  DDGraph_initializeFlows(Scheduler_EARLYGRAPH(this), false);
  DDGraph_initializeFlows(Scheduler_LATEGRAPH(this), false);
  Scheduler_LAMBDAS(this)[1] = Scheduler_LAMBDA(this);
  SCD_TRACE && fprintf(SCD_TRACE, "*** LAMBDA RECURRENCE=%d\n", Scheduler_LAMBDA(this));
  // Run the Leung relaxations to find a lower bound on LAMBDA.
  if (FORCE_LLPALEVEL > 0) {
    Resource resources[Resource__COUNT];
    int issueCount = IssueTable_sortResources(issueTable, resources);
    Scheduler_runRelaxations(this, issueCount, resources);
  }
  Scheduler_LAMBDAS(this)[2] = Scheduler_LAMBDA(this);
  SCD_TRACE && fprintf(SCD_TRACE, "*** LAMBDA RELAXATION=%d\n", Scheduler_LAMBDA(this));
  Scheduler_buildLinkArcs(this, mcrlGraph);
  //DDGraph_initializeFlows(Scheduler_MCRLGRAPH(this), false);
  SCD_TRACE && GETENV("SCD_MCRLGRAPH") && DDGraph_pretty(mcrlGraph, SCD_TRACE);
}

#ifdef $XCC__h
/**
 * Iterate scheduling until it is successful.
 */
bool
Scheduler_iterateScheduling(Scheduler this, int scheduling);
#endif//$XCC__h

bool
Scheduler_iterateScheduling(Scheduler this, int scheduling)
{
  int pipelining = Scheduler_PIPELINING(this);
  Optimize optimize = Scheduler_OPTIMIZE(this);
  OptimizeFlags rcmsProblem = Optimize_RCMSPROBLEM(optimize);
  unsigned solving = Scheduler_SOLVING(this);
  int lambda_min = Scheduler_LAMBDA(this);
  int lambda_max = INT_MAX, length = INT_MAX;
  // First run the RCMS plugin for software pipelines.
  if (rcmsProblem > 0 && pipelining > 0) {
    lambda_min = RCMSInterface_Schedule(this, lambda_min);
    Except_CHECK(lambda_min >= Scheduler_LAMBDA(this));
    Scheduler_makeOrderings(this);
  }
  // Then try the various scheduling heuristics.
  for (--scheduling; scheduling > 0; scheduling-- > 0) {
    if (Scheduler_Schedule[scheduling] != NULL) {
      int lambda = lambda_max;
      Scheduler_unfreeze(this);
      lambda = (*Scheduler_Schedule[scheduling])(this, lambda_min, lambda_max, length);
      if (lambda <= lambda_max && lambda < INT_MAX) {
        length = Scheduler_commitSchedule(this, lambda);
        Except_CHECK(Scheduler_verifyDependences(this) == NULL);
        lambda_max = lambda;
        break;
      }
    }
  }
  // Finally run the RCMSSolving if earlier scheduling succeeded.
  if (scheduling > 0 && solving > 0) {
    int lambda = lambda_max;
    Scheduler_unfreeze(this);
    lambda = Formulation_Schedule(this, lambda_min, lambda_max, length);
    if (lambda <= lambda_max && lambda < INT_MAX) {
      int new_length = Scheduler_commitSchedule(this, lambda);
      Except_CHECK(Scheduler_verifyDependences(this) == NULL);
      *Scheduler__DELTALAMBDA(this) = lambda_max - lambda;
      *Scheduler__DELTALENGTH(this) = length - new_length;
      lambda_max = lambda;
    }
  }
  return lambda_max < INT_MAX;
}

#ifdef $XCC__h
/**
 * Recompute the IssueItem ORDERING(s) in topological order.
 */
void
Scheduler_makeOrderings(Scheduler this);
#endif//$XCC__h

static void
Scheduler_topologicalVisit(Scheduler this, bool *visited, DDGraphNode node, int32_t *_ordering)
{
  Operation operation = DDGraphNode_OPERATION(node);
  IssueItem issueItem = Operation_issueItem(operation);
  Scheduling scheduling = IssueItem_SCHEDULING(issueItem);
  visited[DDGraphNode_INDEX(node)] = true;
  DDGraphNode_FORLEAVE_DDGraphArc(node, arc) {
    DDGraphNode head_node = DDGraphArc_HEAD(arc);
    if (   DDGraphArc_OMEGA(arc) <= 0
        && !visited[DDGraphNode_INDEX(head_node)]) {
      Scheduler_topologicalVisit(this, visited, head_node, _ordering);
    }
  } DDGraphNode_ENDLEAVE_DDGraphArc;
  SCD_TRACE && fprintf(SCD_TRACE, "*** Reorder %s_%d:\t%d => %d\n",
                                  Scheduling_name(scheduling), DDGraphNode_INDEX(node),
                                  IssueItem_ORDERING(issueItem), *_ordering - 1);
  *IssueItem__ORDERING(issueItem) = --*_ordering;
}

void
Scheduler_makeOrderings(Scheduler this)
{
  Memory memory = Scheduler_MEMORY(this);
  DDGraph mcrlGraph = Scheduler_MCRLGRAPH(this);
  int32_t nodeCount = DDGraph_nodeCount(mcrlGraph);
  int32_t firstIndex = Scheduler_FIRSTINDEX(this);
  int32_t lastIndex = Scheduler_LASTINDEX(this), index;
  DDGraphNode *index2node = DDGraph_nodes(mcrlGraph);
  DDGraphNode firstNode = index2node[firstIndex];
  DDGraphNode lastNode = index2node[lastIndex];
  Operation last_operation = DDGraphNode_OPERATION(lastNode);
  IssueItem last_issueItem = Operation_issueItem(last_operation);
  int32_t last_ordering = IssueItem_ORDERING(last_issueItem);
  bool *visited = Memory_alloc(memory, nodeCount*sizeof(bool));
  memset(visited, 0, nodeCount*sizeof(bool));
  for (index = 0; index < firstIndex; index++) visited[index] = true;
  for (index = lastIndex; index < nodeCount; index++) visited[index] = true;
  Scheduler_topologicalVisit(this, visited, firstNode, &last_ordering);
  Memory_free(memory, visited);
}

#ifdef $XCC__h
/**
 * Commit schedule.
 */
int
Scheduler_commitSchedule(Scheduler this, int lambda);
#endif//$XCC__h

int
Scheduler_commitSchedule(Scheduler this, int lambda)
{
  int firstDate = INT_MAX, endDate = -1;
  GrahamList grahamList = Scheduler_GRAHAMLIST(this);
  IssueTable issueTable = GrahamList_ISSUETABLE(grahamList);
  GrahamList_FOREACH_GrahamListTask(grahamList, TASK) {
    IssueItem issueItem = GrahamListTask_ITEM(TASK);
    int processing = IssueItem_PROCESSING(issueItem);
    int startDate = GrahamListTask_TAU(TASK) + lambda*GrahamListTask_PHI(TASK);
    *IssueItem__STARTDATE(GrahamListTask_ITEM(TASK)) = startDate;
    // Compute the firstDate and the endDate.
    Except_CHECK(startDate >= 0);
    if (firstDate > startDate) firstDate = startDate;
if (DDGraph_PROCESSING) {
    if (endDate < startDate + processing) endDate = startDate + processing;
} else {
    if (endDate < startDate) endDate = startDate;
}
  } GrahamList_ENDEACH_GrahamListTask;
  Except_CHECK(firstDate >= 0 && endDate >= firstDate);
  *Scheduler__LAMBDA(this) = lambda;
if (DDGraph_PROCESSING) {
  *Scheduler__LENGTH(this) = endDate - firstDate;
} else {
  *Scheduler__LENGTH(this) = endDate - firstDate + 1;
}
  SCD_TRACE && IssueTable_pretty(issueTable, SCD_TRACE);
  return Scheduler_LENGTH(this);
}

#ifdef $XCC__h
DDGraphArc
Scheduler_verifyDependences(Scheduler this);
#endif//$XCC__h

DDGraphArc
Scheduler_verifyDependences(Scheduler this)
{
  int lambda = Scheduler_LAMBDA(this);
  GrahamList grahamList = Scheduler_GRAHAMLIST(this);
  GrahamListTask *index2task = Scheduler_INDEX2TASK(this);
  int32_t firstIndex = Scheduler_FIRSTINDEX(this);
  int32_t lastIndex = Scheduler_LASTINDEX(this), nodeIndex;
  DDGraphNode *index2node = DDGraph_nodes(Scheduler_MCRLGRAPH(this));
  for (nodeIndex = firstIndex; nodeIndex <= lastIndex; nodeIndex++) {
    int32_t i_index = nodeIndex;
    DDGraphNode i_node = index2node[i_index];
    GrahamListTask i_task = index2task[i_index];
    int i_date = GrahamListTask_TAU(i_task) + lambda*GrahamListTask_PHI(i_task);
    DDGraphNode_FORLEAVE_DDGraphArc(i_node, arc) {
      DDGraphNode j_node = DDGraphArc_HEAD(arc);
      int32_t j_index = DDGraphNode_INDEX(j_node);
      if (j_index != i_index && j_index >= firstIndex && j_index <= lastIndex) {
        GrahamListTask j_task = index2task[j_index];
        int j_date = GrahamListTask_TAU(j_task) + lambda*GrahamListTask_PHI(j_task);
        int latency = DDGraphArc_THETA(arc) - lambda*DDGraphArc_OMEGA(arc);
        if (i_date + latency > j_date) {
          SCD_TRACE && fprintf(SCD_TRACE, "*** FAILED: %d + %d <= %d\n\t", i_date, latency, j_date)
                    && DDGraphArc_pretty(arc, SCD_TRACE) && fprintf(SCD_TRACE, "\n");
          return arc;
        }
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Main processing function.
 */
bool
Scheduler_schedule(Scheduler this, RegionFlow regionFlow);
#endif//$XCC_h

bool
Scheduler_schedule(Scheduler this, RegionFlow regionFlow)
{
  const char *SCD_SCHEDULING = GETENV("SCD_SCHEDULING");
  const char *SCD_POSTPASS = GETENV("SCD_POSTPASS");
  const char *headName = Scheduler_HEADNAME(this);
  uint32_t identity = Scheduler_IDENTITY(this);
  int32_t serialize = (int)identity == LAO_IDENTITY? LAO_SERIALIZE: 0;
  BlockTrace blockTrace = Scheduler_BLOCKTRACE(this);
  CodeRegion codeRegion = BlockTrace_CODEREGION(blockTrace);
  Optimize optimize = CodeRegion_optimize(codeRegion);
  bool disable_scd = (int)identity < LAO_STARTSCD;
  bool disable_swp = (int)identity < LAO_STARTSWP;
  int scheduling = Optimize_SCHEDULING(optimize);
  if (disable_scd) serialize = INT32_MAX;
  if (disable_swp) BlockTrace_disablePipelining(blockTrace);
  SCD_TRACE && fprintf(SCD_TRACE,
                       "    %s\tcompensation=%d\tspeculation=%d\trelaxation=%d",
                       Scheduler_isPrePass(this)? "prepass": "postpass",
                       Scheduler_COMPENSATION(this), Scheduler_SPECULATION(this),
                       Scheduler_RELAXATION(this))
            && fprintf(SCD_TRACE,
                       "\tpipelining=%d\trenaming=%d\tboosting=%d\toverlap=%d\n",
                       Scheduler_PIPELINING(this), Scheduler_RENAMING(this),
                       Scheduler_BOOSTING(this), Scheduler_OVERLAP(this));
  if (Scheduler_isPrePass(this)) {
    if (   (SCD_SCHEDULING != NULL && strcmp(SCD_SCHEDULING, headName))
        || SCD_POSTPASS != NULL) {
      SCD_TRACE && fprintf(SCD_TRACE, "    SKIPPED\n");
      return false;
    }
    if (FORCE_PRESERIAL > serialize) serialize = FORCE_PRESERIAL;
  } else {
    if (   (SCD_POSTPASS != NULL && strcmp(SCD_POSTPASS, headName))
        || SCD_SCHEDULING != NULL) {
      SCD_TRACE && fprintf(SCD_TRACE, "    SKIPPED\n");
      return false;
    }
    if (FORCE_POSTSERIAL > serialize) serialize = FORCE_POSTSERIAL;
  }
  if (scheduling == 0) return false;
  *Scheduler__SERIALIZE(this) = serialize;
  // Normalize the control-flow and enter the operations.
  BlockTrace_normalizeRegionFlow(blockTrace);
  BlockTrace_prepareScheduling(blockTrace);
  SCD_TRACE && fprintf(SCD_TRACE, "*** ENTER OPERATIONS:\n");
  BlockTrace_enterOperations(blockTrace);
  SCD_TRACE && Scheduler_pretty(this, SCD_TRACE);
  // Build the Effects and the Lifetimes.
  BlockTrace_relaxConstraints(blockTrace);
  BlockTrace_buildEffects(blockTrace);
  BlockTrace_buildLifetimes(blockTrace);
  //if (BlockTrace_isInnerLoop(blockTrace)) BlockTrace_findInvariants(blockTrace);
  // Build the MCRL and Early and Late DDGraphs.
  Scheduler_buildDDGraph(this, NULL,
      DDGraphKind_MCRL, DDGraphFlag_Forward|DDGraphFlag_Lifetime);
  Scheduler_buildDDGraph(this, Scheduler_MCRLGRAPH(this),
      DDGraphKind_Early, DDGraphFlag_Forward);
  Scheduler_buildDDGraph(this, Scheduler_EARLYGRAPH(this),
      DDGraphKind_Late, DDGraphFlag_Backward);
  // Prepare the scheduling problem.
  Scheduler_prepareScheduling(this);
  if (Scheduler_iterateScheduling(this, scheduling)) {
    int deltaLambda = Scheduler_DELTALAMBDA(this);
    int deltaLength = Scheduler_DELTALENGTH(this);
    bool improved = deltaLambda > 0 || (deltaLambda == 0 && deltaLength > 0);
    // Scheduling succeeded, so construct the schedule.
    int bodyCount = Scheduler_BODYCOUNT(this);
    int scheduled = Scheduler_SCHEDULED(this);
    if (Scheduler_isPrePass(this)) {
      Scheduler_computePressures(this);
      if (Scheduler_isPipeline(this)) {
        SCD_TRACE && fprintf(SCD_TRACE, "PIPELINE:%d\t", identity);
      } else {
        SCD_TRACE && fprintf(SCD_TRACE, "PREPASS:%d\t", identity);
      }
    } else {
      SCD_TRACE && fprintf(SCD_TRACE, "POSTPASS:%d\t", identity);
    }
    SCD_TRACE && fprintf(SCD_TRACE, "LABEL=%s\t", Scheduler_HEADNAME(this));
    SCD_TRACE && fprintf(SCD_TRACE, "LENGTH=%d\tLAMBDA=%d\tDELTAS=%d\t",
                         Scheduler_LENGTH(this),
                         Scheduler_LAMBDA(this),
                         Scheduler_DELTAS(this));
    SCD_TRACE && fprintf(SCD_TRACE, "LAMBDAS={Res=>%d,Rec=>%d,Rel=>%d}\t",
                         Scheduler_LAMBDAS(this)[0],
                         Scheduler_LAMBDAS(this)[1],
                         Scheduler_LAMBDAS(this)[2]);
    SCD_TRACE && bodyCount && scheduled && fprintf(SCD_TRACE, "BODYCOUNT=%d\tSCHEDULED=%d\t",
                                                              bodyCount, scheduled);

    SCD_TRACE && improved && fprintf(SCD_TRACE, "IMPROVED(%d,%d)\t", deltaLambda, deltaLength);
    if (Scheduler_isPrePass(this)) {
      const char *sep = "{";
      RegFile regFile = RegFile__UNDEF;
      SCD_TRACE && fprintf(SCD_TRACE, "\tMAXLIVES=");
      for (regFile = RegFile__UNDEF + 1; regFile < RegFile__Count; regFile++) {
        SCD_TRACE && fprintf(SCD_TRACE, "%s%s=>%d", sep, RegFile_name(regFile),
                             Scheduler_MAXLIVES(this)[regFile]);
        sep = ",";
      }
      SCD_TRACE && fprintf(SCD_TRACE, "}\t");
    }
    SCD_TRACE && fprintf(SCD_TRACE, "\n");
    {
      Memory memory = Scheduler_MEMORY(this);
      Construction construction = Construction_make(memory, regionFlow, blockTrace);
      Construction_construct(construction, false);
      Construction_kill(construction);
    }
    return true;
  }
  // Scheduling failed for whatever reason.
  return false;
}

#ifdef $XCC__h
/**
 * Solve the initial network flow problems.
 */
void
Scheduler_initializeFlows(Scheduler this, bool lifetime);
#endif//$XCC__h

void
Scheduler_initializeFlows(Scheduler this, bool lifetime)
{
  DDGraphKind kind = 0;
  for (; kind < DDGraphKind__; kind++) {
    DDGraph ddgraph = Scheduler_DDGRAPHS(this)[kind];
    if (ddgraph != NULL) {
      if (lifetime || !DDGraph_isLifetime(ddgraph)) {
        DDGraph_initializeFlows(ddgraph, lifetime);
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Increase LAMBDA just enough to satisfy dependences.
 */
int
Scheduler_increaseLambda(Scheduler this, int maxDelta);
#endif//$XCC__h

int
Scheduler_increaseLambda(Scheduler this, int maxDelta)
{
  int16_t lambda = Scheduler_LAMBDA(this);
  DDGraphKind kind = DDGraphKind_Early;
  DDGraph ddgraph = Scheduler_DDGRAPHS(this)[kind];
  if (ddgraph != NULL) DDGraph_increaseLambda(ddgraph);
  else *Scheduler__LAMBDA(this) += maxDelta;
  return Scheduler_LAMBDA(this) - lambda;
}

#ifdef $XCC__h
/**
 * Optimize the network flow problems.
 */
void
Scheduler_optimizeFlows(Scheduler this, bool lifetime, bool refill);
#endif//$XCC__h

void
Scheduler_optimizeFlows(Scheduler this, bool lifetime, bool refill)
{
  DDGraphKind kind = 0;
  // Solve the Network problems.
  for (; kind < DDGraphKind__; kind++) {
    DDGraph ddgraph = Scheduler_DDGRAPHS(this)[kind];
    if (ddgraph != NULL) {
      if (lifetime || !DDGraph_isLifetime(ddgraph)) {
        DDGraph_optimizeFlows(ddgraph, refill);
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Freeze the GrahamListTask with TAU biased by delta.
 */
void
Scheduler_freezeTask(Scheduler this, GrahamListTask task, int delta);
#endif//$XCC__h

void
Scheduler_freezeTask(Scheduler this, GrahamListTask task, int delta)
{
  IssueItem item = GrahamListTask_ITEM(task);
  int16_t tau = GrahamListTask_TAU(task) + delta;
  int16_t phi = GrahamListTask_PHI(task);
  DDGraphKind kind = 0;
  for (; kind < DDGraphKind__; kind++) {
    DDGraph ddgraph = Scheduler_DDGRAPHS(this)[kind];
    if (ddgraph != NULL) {
      DDGraphNode node = IssueItem_DDGNODES(item)[kind];
      if (DDGraph_isForward(ddgraph)) {
        DDGraph_freezeNode(ddgraph, node, tau, -phi);
      } else {
        DDGraph_freezeNode(ddgraph, node, DDGraph_EXTENT - tau, phi);
      }
    }
  }
  *GrahamListTask__TAU(task) = tau;
}

#ifdef $XCC__h
/**
 * Unfreeze the dependence graphs.
 */
void
Scheduler_unfreeze(Scheduler this);
#endif//$XCC__h

void
Scheduler_unfreeze(Scheduler this)
{
  DDGraphKind kind = 0;
  for (; kind < DDGraphKind__; kind++) {
    DDGraph ddgraph = Scheduler_DDGRAPHS(this)[kind];
    if (ddgraph != NULL) DDGraph_unfreeze(ddgraph);
  }
}

#ifdef $XCC__h
/**
 * Make an new dependence arc for this Scheduler.
 */
DDGraphArc
Scheduler_makeArc(Scheduler this, int32_t tail_index, int32_t head_index,
                  int theta, int omega, DependenceKind kind, Temporary temporary);
#endif//$XCC__h

DDGraphArc
Scheduler_makeArc(Scheduler this, int32_t tail_index, int32_t head_index,
                  int theta, int omega, DependenceKind depKind, Temporary temporary)
{
  DDGraphKind kind = 0;
  DDGraphArc arcs[DDGraphKind__];
  for (; kind < DDGraphKind__; kind++) {
    DDGraph ddgraph = Scheduler_DDGRAPHS(this)[kind];
    if (ddgraph != NULL) {
      DDGraphNode tail = DDGraph_access(ddgraph, tail_index);
      DDGraphNode head = DDGraph_access(ddgraph, head_index);
      arcs[kind] = DDGraph_isForward(ddgraph)?
          DDGraph_makeArc(ddgraph, tail, head, theta, omega, depKind, temporary):
          DDGraph_makeArc(ddgraph, head, tail, theta, omega, depKind, temporary);
    } else {
      arcs[kind] = NULL;
    }
  }
  return arcs[DDGraphKind_MCRL];
}

#ifdef $XCC__h
/**
 * Freeze a node at the given tau and phi.
 */
DDGraphNode
Scheduler_freezeNode(Scheduler this, int32_t node_index, int tau, int phi);
#endif//$XCC__h

DDGraphNode
Scheduler_freezeNode(Scheduler this, int32_t node_index, int tau, int phi)
{
  DDGraphKind kind = 0;
  DDGraphNode nodes[DDGraphKind__];
  for (; kind < DDGraphKind__; kind++) {
    DDGraph ddgraph = Scheduler_DDGRAPHS(this)[kind];
    if (ddgraph != NULL) {
      DDGraphNode node = DDGraph_access(ddgraph, node_index);
      if (DDGraph_isForward(ddgraph)) {
        DDGraph_freezeNode(ddgraph, node, tau, -phi);
      } else {
        DDGraph_freezeNode(ddgraph, node, DDGraph_EXTENT - tau, phi);
      }
      nodes[kind] = node;
    } else {
      nodes[kind] = NULL;
    }
  }
  return nodes[DDGraphKind_MCRL];
}

#ifdef $XCC__h
/**
 * Array of Schedule functions.
 */
extern
int (*Scheduler_Schedule[OptimizeScheduling__])(struct Scheduler_ *, int, int, int);
#endif//$XCC__h

int (*Scheduler_Schedule[OptimizeScheduling__])(struct Scheduler_ *, int, int, int);

#ifdef $XCC_h
/**
 * Set a Schedule function for @c scheduling.
 */
void
Scheduler_SetSchedule(int scheduling, int (*schedule)(struct Scheduler_ *, int, int, int));
#endif//$XCC_h

void
Scheduler_SetSchedule(int scheduling, int (*schedule)(struct Scheduler_ *, int, int, int))
{
  Except_REQUIRE(scheduling >= 0);
  Except_REQUIRE(scheduling*sizeof(*Scheduler_Schedule) < sizeof(Scheduler_Schedule));
  Scheduler_Schedule[scheduling] = schedule;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Scheduler module.
 */
void
Scheduler_INIT(void);
#endif//$XCC__h

void
Scheduler_INIT(void)
{
}

#ifdef $XCC__h
/**
 * Finalize the Scheduler module.
 */
void
Scheduler_FINI(void);
#endif//$XCC__h

void
Scheduler_FINI(void)
{
}

#if XCC__C
static void
Scheduler_TEST(void)
{
#include "SCD/Scheduler_.c"
}

int
main(int argc, char **argv)
{
  SCD_Test(Scheduler_TEST);
  return 0;
}
#endif

