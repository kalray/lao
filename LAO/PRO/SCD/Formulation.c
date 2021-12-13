#ifdef $XCC_h
/*
 * !!!!	Formulation.xcc
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
 * @brief RCMSP time-indexed Formulation(s).
 */
#endif//$XCC_h


#include "SCD/SCD_.h"

#ifdef $XCC__h
#include "SCD/Formulation.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Time-indexed formulation for optimal modulo scheduling.
 */
struct Formulation_ {
  //@args	Memory memory, Scheduler scheduler
  Memory MEMORY;		// This Formulation Memory.
  Scheduler SCHEDULER;		// The instruction scheduler.
  //@access LAMBDA2	Scheduler_LAMBDAS(Formulation_SCHEDULER(this))[2]
  //@access LINKARCS	Scheduler_LINKARCS(Formulation_SCHEDULER(this))
  //@access HEADNAME	Scheduler_HEADNAME(Formulation_SCHEDULER(this))
  //@access ISSUETABLE	Scheduler_ISSUETABLE(Formulation_SCHEDULER(this))
  //@access SOLVING	Scheduler_SOLVING(Formulation_SCHEDULER(this))
  //@access TIMEOUT	Scheduler_TIMEOUT(Formulation_SCHEDULER(this))
  GrahamList GRAHAMLIST;	// The GrahamList for this Formulation.
  //@access INDEX2TASK	GrahamList_INDEX2TASK(Formulation_GRAHAMLIST(this))
  DDGraph MCRLGRAPH;		// The reference DDGraph for this Formulation.
  //@access INDEX2NODE	DDGraph_nodes(Formulation_MCRLGRAPH(this))
  int32_t FIRSTINDEX;		// The first GrahamListTask INDEX.
  int32_t LASTINDEX;		// The last GrahamListTask INDEX.
  int16_t *EARLYDATES;		// Array of the DDGraphNode(s) early dates.
  int16_t *LATEDATES;		// Array of the DDGraphNode(s) late dates.
  int16_t **DELAYMATRIX;	// Matrix of longest path delays.
  DDGraphArc **EDGEMATRIX;	// Matrix of longest path edges.
  PtrSeq_ NODESTACK[1];		// DDGraphNode stack for constraint propagation.
  bool *INSTACK;		// Flag the DDGraphNode(s) that are on NODESTACK;
  //@access isDinechin	((Formulation_SOLVING(this) & OptimizeRCMSSolving_Dinechin) != 0)
  //@access isLNSScan1	((Formulation_SOLVING(this) & OptimizeRCMSSolving_LNSScan1) != 0)
  //@access isLNSScan2	((Formulation_SOLVING(this) & OptimizeRCMSSolving_LNSScan2) != 0)
  //@access isEicDavAb	((Formulation_SOLVING(this) & OptimizeRCMSSolving_EicDavAb) != 0)
  //@access isAggrDeps	((Formulation_SOLVING(this) & OptimizeRCMSSolving_AggrDeps) != 0)
  //@access isRational	((Formulation_SOLVING(this) & OptimizeRCMSSolving_Rational) != 0)
  //@access isPostPass	((Formulation_SOLVING(this) & OptimizeRCMSSolving_PostPass) != 0)
  //@access isAcyclic	((Formulation_SOLVING(this) & OptimizeRCMSSolving_Acyclic) != 0)
  int8_t SCANNING;		// Max number of scanning steps in Scanning heuristic.
  LProgram PROGRAM;		// The LProgram corresponding to this Formulation.
  //@access STATUS	LProgram_STATUS(Formulation_PROGRAM(this))
  //@access VARIABLECOUNT	LProgram_VARIABLECOUNT(Formulation_PROGRAM(this))  
  //@access CONSTRAINTCOUNT	LProgram_CONSTRAINTCOUNT(Formulation_PROGRAM(this))  
  //@access SOLUTION	LProgram_solution(Formulation_PROGRAM(this))  
  int16_t LAMBDA;		// The current LAMBDA.
  int16_t HORIZON;		// The current HORIZON.
  unsigned EXITWEIGHT;		// Weight of exit operations in the schedule.
  double TIMER;			// Timer that accumulates time spent on failure.
};
#endif//$XCC__h

Formulation
Formulation_Ctor(Formulation this,
                 Memory memory, Scheduler scheduler)
{
  DDGraph mcrlGraph = Scheduler_MCRLGRAPH(scheduler);
  GrahamList grahamList = Scheduler_GRAHAMLIST(scheduler);
  GrahamListTask firstTask = GrahamList_FIRSTTASK(grahamList);
  GrahamListTask lastTask = GrahamList_LASTTASK(grahamList);
  int32_t firstIndex = GrahamListTask_INDEX(firstTask);
  int32_t lastIndex = GrahamListTask_INDEX(lastTask), index;
  size_t datesSize = (lastIndex+1)*sizeof(int16_t);
  size_t delayMatrixSize = (lastIndex+1)*sizeof(int16_t*);
  size_t delayVectorSize = (lastIndex+1)*sizeof(int16_t);
  size_t edgeMatrixSize = (lastIndex+1)*sizeof(DDGraphArc*);
  size_t edgeVectorSize = (lastIndex+1)*sizeof(DDGraphArc);
  *Formulation__MEMORY(this) = memory;
  *Formulation__SCHEDULER(this) = scheduler;
  *Formulation__GRAHAMLIST(this) = grahamList;
  *Formulation__MCRLGRAPH(this) = mcrlGraph;
  *Formulation__FIRSTINDEX(this) = firstIndex;
  *Formulation__LASTINDEX(this) = lastIndex;
  *Formulation__EARLYDATES(this) = Memory_alloc(memory, datesSize);
  memset(Formulation_EARLYDATES(this), -1, datesSize);
  *Formulation__LATEDATES(this) = Memory_alloc(memory, datesSize);
  memset(Formulation_LATEDATES(this), -1, datesSize);
  *Formulation__DELAYMATRIX(this) = Memory_alloc(memory, delayMatrixSize);
  for (index = 0; index <= lastIndex; index++) {
    int16_t **delayMatrix = Formulation_DELAYMATRIX(this);
    delayMatrix[index] = Memory_alloc(memory, delayVectorSize);
  }
  *Formulation__EDGEMATRIX(this) = Memory_alloc(memory, edgeMatrixSize);
  for (index = 0; index <= lastIndex; index++) {
    DDGraphArc **edgeMatrix = Formulation_EDGEMATRIX(this);
    edgeMatrix[index] = Memory_alloc(memory, edgeVectorSize);
  }
  PtrSeq_Ctor(Formulation_NODESTACK(this), memory, (lastIndex+1));
  *Formulation__INSTACK(this) = Memory_alloc(memory, sizeof(bool)*(lastIndex+1));
  *Formulation__SCANNING(this) = FORCE_SCANNING < 0?
                                 Formulation_isLNSScan1(this) +
                                 Formulation_isLNSScan2(this)*2:
                                 FORCE_SCANNING;
  *Formulation__PROGRAM(this) = NULL;
  *Formulation__LAMBDA(this) = 0;
  *Formulation__HORIZON(this) = 0;
  *Formulation__EXITWEIGHT(this) = FORCE_EXITWEIGHT >= 0? FORCE_EXITWEIGHT: 0;
  *Formulation__HORIZON(this) = 0;
  *Formulation__TIMER(this) = 0.0;
  return this;
}

void
Formulation_Dtor(Formulation this)
{
  Memory memory = Formulation_MEMORY(this);
  int32_t lastIndex = Formulation_LASTINDEX(this), index;
  Memory_free_(memory, Formulation_EARLYDATES(this));
  Memory_free_(memory, Formulation_LATEDATES(this));
  for (index = 0; index <= lastIndex; index++) {
    Memory_free_(memory, Formulation_DELAYMATRIX(this)[index]);
  }
  Memory_free_(memory, Formulation_DELAYMATRIX(this));
  for (index = 0; index <= lastIndex; index++) {
    Memory_free_(memory, Formulation_EDGEMATRIX(this)[index]);
  }
  Memory_free_(memory, Formulation_EDGEMATRIX(this));
  PtrSeq_Dtor(Formulation_NODESTACK(this));
  Memory_free_(memory, Formulation_INSTACK(this));
  LProgram_delete(Formulation_PROGRAM(this));
}

size_t
Formulation_Size(Memory memory, Scheduler scheduler)
{
  return sizeof(Formulation_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Formulation_)\t%zu\n", sizeof(Formulation_));
}
#endif//$XCC__c

#ifdef $XCC__h
Formulation
Formulation_new(Memory parent, Scheduler scheduler);
#endif//$XCC__h

Formulation
Formulation_new(Memory parent, Scheduler scheduler)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Formulation_Size(memory, scheduler);
  Formulation this = Memory_alloc(memory, size);
  return Formulation_Ctor(this, memory, scheduler);
}

#ifdef $XCC__h
Formulation
Formulation_delete(Formulation this);
#endif//$XCC__h

Formulation
Formulation_delete(Formulation this)
{
  if (this != NULL) {
    Memory memory = Formulation_MEMORY(this);
#ifndef _NDTOR
    Formulation_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * For use by Formulation_FOREACH_DDGraphNode.
 */
static inline int32_t
Formulation_firstIndex(Formulation this)
{
  return Formulation_FIRSTINDEX(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * For use by Formulation_FOREACH_DDGraphNode.
 */
static inline int32_t
Formulation_lastIndex(Formulation this)
{
  return Formulation_LASTINDEX(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * For use by Formulation_FOREACH_DDGraphNode.
 */
static inline DDGraphNode *
Formulation_index2node(Formulation this)
{
  return Formulation_INDEX2NODE(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Formulation DDGraphNode(s).
 */
#define Formulation_FOREACH_DDGraphNode(this, node) { \
  DDGraphNode *Formulation_INDEX2NODE = Formulation_index2node(this); \
  int32_t Formulation_FIRSTINDEX = Formulation_firstIndex(this); \
  int32_t Formulation_LASTINDEX = Formulation_lastIndex(this); \
  int32_t Formulation_NODEINDEX = Formulation_FIRSTINDEX; \
  for (; Formulation_NODEINDEX <= Formulation_LASTINDEX; Formulation_NODEINDEX++) { \
    DDGraphNode node = Formulation_INDEX2NODE[Formulation_NODEINDEX];
#define Formulation_ENDEACH_DDGraphNode \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * For use by Formulation_FOREACH_GrahamListTask.
 */
static inline struct GrahamListTask_ **
Formulation_index2task(Formulation this)
{
  return Formulation_INDEX2TASK(this);
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Formulation GrahamListTask(s).
 */
#define Formulation_FOREACH_GrahamListTask(this, task) { \
  GrahamListTask_ **Formulation_INDEX2TASK = Formulation_index2task(this); \
  int32_t Formulation_FIRSTINDEX = Formulation_firstIndex(this); \
  int32_t Formulation_LASTINDEX = Formulation_lastIndex(this); \
  int32_t Formulation_TASKINDEX = Formulation_FIRSTINDEX; \
  for (; Formulation_TASKINDEX <= Formulation_LASTINDEX; Formulation_TASKINDEX++) { \
    GrahamListTask task = Formulation_INDEX2TASK[Formulation_TASKINDEX];
#define Formulation_ENDEACH_GrahamListTask \
  } \
}
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Iterate over this Formulation LINKARCS.
 */
#define Formulation_LINKARCS_FOREACH_DDGraphArc(this, regFile, arc) \
  Scheduler_LINKARCS_FOREACH_DDGraphArc(Formulation_SCHEDULER(this), regFile, arc)
#define Formulation_LINKARCS_ENDEACH_DDGraphArc \
  Scheduler_LINKARCS_ENDEACH_DDGraphArc;
#endif//$XCC__h

#ifdef $XCC__h
const char *
Formulation_saveName(Formulation this, const char *name);
#endif//$XCC__h

const char *
Formulation_saveName(Formulation this, const char *name)
{
  if (name != NULL) {
    size_t length = strlen(name);
    Memory memory = Formulation_MEMORY(this);
    char *saveName = Memory_alloc(memory, length + 1);
    return strcpy(saveName, name);
  }
  return NULL;
}

#ifdef $XCC__h
#define Formulation_KEY1(regFile)	(void *)(regFile - RegFile__)
#define Formulation_KEY2(index, date)	(void *)(((index)<<16) | ((date)&0xffff))
#endif//$XCC__h

#ifdef $XCC__h
bool
Formulation_pretty(Formulation this, FILE *file);
#endif//$XCC__h

#ifndef _NTRACE
bool
Formulation_pretty(Formulation this, FILE *file)
{
  int32_t lastIndex = Formulation_LASTINDEX(this);
  int16_t *earlyDates = Formulation_EARLYDATES(this);
  int16_t *lateDates = Formulation_LATEDATES(this);
  if (file == NULL) file = stderr;
  Formulation_FOREACH_DDGraphNode(this, node) {
    int32_t index = Formulation_NODEINDEX;
    RegFile regFile = 0;
    fprintf(file, "\\ %s \tearly=%d\tlate=%d\t",
        DDGraphNode_name(node), earlyDates[index], lateDates[index]);
    fprintf(file, "\n");
  } Formulation_ENDEACH_DDGraphNode;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Schedule by solving a Formulation.
 *
 * We use a dichotomy search to find the minimum delta such that the relaxation
 * succeeds a initiation interval lambda + delta.
 *
 * Require:	The problem is feasible at lambda = lambda_max.
 * Return:	A feasible lambda if found else INT_MAX.
 */
int
Formulation_Schedule(Scheduler scheduler, int lambda_min, int lambda_max, int length);
#endif//$XCC__h

int
Formulation_Schedule(Scheduler scheduler, int lambda_min, int lambda_max, int length)
{
  int lambda = INT_MAX;
  unsigned solving = Scheduler_SOLVING(scheduler);
  Except_REQUIRE(lambda_min <= lambda_max && lambda_max < INT_MAX);
  if (   (Scheduler_isPrePass(scheduler) || (solving & OptimizeRCMSSolving_PostPass))
      && (Scheduler_isInnerLoop(scheduler) || (solving & OptimizeRCMSSolving_Acyclic))
      && (Scheduler_ITEMCOUNT(scheduler) <= LAO_MAXITEMCOUNT)) {
    Formulation this = Formulation_new(Scheduler_MEMORY(scheduler), scheduler);
    int timeOut = Formulation_TIMEOUT(this);
    int scanning = Formulation_SCANNING(this);
    SCD_TRACE && fprintf(SCD_TRACE, "*** FORMULATION(%s)\tSCHEDULE\tMIN=%d\tMAX=%d\tSCANNING=%d\t",
                                    Formulation_HEADNAME(this), lambda_min, lambda_max, scanning);
#ifndef _NTRACE
    SCD_TRACE && ETable_prettyFlags(OptimizeRCMSSolving_Table,
                                    Formulation_SOLVING(this), ",", SCD_TRACE);
#endif//_NTRACE
    SCD_TRACE && fprintf(SCD_TRACE, "\tTIMEOUT=%ds\n", timeOut);
    if (scanning > 0) {
      // Use the Large Neighborhood Search.
      while (lambda_max >= lambda_min) {
        int length_max = length;
        int scanStep = 0, scanSuccess = 0;
        // First scan: check feasibility at lambda_max.
        while (scanStep < scanning && Formulation_TIMER(this) <= timeOut) {
          if (Formulation_trySchedule(this, lambda_max, scanStep++, true)) {
            length = Formulation_commitSchedule(this);
            scanSuccess = scanStep;
            lambda = lambda_max;
            break;
          }
        }
        if (lambda > lambda_max) break;
        // Second scan: try to improve the incumbent solution.
        while (scanSuccess < scanning && Formulation_TIMER(this) <= timeOut) {
          LProgramValue scanSolution = Formulation_SOLUTION(this);
          // Cut search at this LAMBDA if quality is good enough.
          if (length + length/lambda < length_max) break;
          if (   Formulation_trySchedule(this, lambda_max, scanSuccess++, false)
              && Formulation_SOLUTION(this) < scanSolution) {
            length = Formulation_commitSchedule(this);
          }
        }
        // Move to next LAMBDA.
        *Formulation__TIMER(this) = 0.0;
        lambda_max--;
      }
    } else {
      // Classic search for the minimum feasible lambda, try lambda_min first.
      if (Formulation_trySchedule(this, lambda_min, 0, true)) {
        Formulation_commitSchedule(this);
        lambda = lambda_min;
      } else
      if (   lambda_max > lambda_min
          && Formulation_trySchedule(this, lambda_max, 0, true)) {
        // Dichotomy search between lambda_min and lambda_max
        int delta = lambda_max - lambda_min, prev_delta = 0;
        // Here lambda_min + delta is feasible and lambda_min + prev_delta is infeasible.
        Formulation_commitSchedule(this);
        if (prev_delta + 1 <= delta - 1) {
          int l = prev_delta + 1, r = delta - 1;
          while (r >= l) {
            int i = (l + r) >> 1;
            if (Formulation_trySchedule(this, lambda_min + i, 0, true)) {
              Formulation_commitSchedule(this);
              delta = i;
              r = i - 1;
            } else {
              l = i + 1;
            }
          }
        }
        lambda = lambda_min + delta;
      }
      // Either lambda <= lambda_max and lambda is feasible or lambda is infeasible.
    }
    Formulation_delete(this);
  }
  return lambda;
}

#ifdef $XCC__h
/**
 * Try to schedule with Formulation at the given lambda.
 */
bool
Formulation_trySchedule(Formulation this, int lambda, int scanStep, bool feasibility);
#endif//$XCC__h

bool
Formulation_trySchedule(Formulation this, int lambda, int scanStep, bool feasibility)
{
  bool solved = false;
  LProgramStatus status = LProgramStatus__;
  LProgram program = Formulation_buildProgram(this, lambda, scanStep, feasibility);
  LProgram_open(program, "formulation");
  LProgram_optimize(program, true);
  LProgram_close(program);
  solved = LProgram_status(program) == LProgramStatus_Solved;
  *Formulation__TIMER(this) += LProgram_timeSpent(program);
  SCD_TRACE && fprintf(SCD_TRACE, "*** FORMULATION(%s)\tTRY=%c\tLAMBDA=%d\t",
                       Formulation_HEADNAME(this), feasibility? 'F': 'O', lambda);
  if (solved) {
    SCD_TRACE && fprintf(SCD_TRACE, "SOLUTION=%g\t", Formulation_SOLUTION(this));
    if (Formulation_isRational(this)) solved = false;
  } else {
    SCD_TRACE && LProgramStatus_pretty(LProgram_status(program), SCD_TRACE)
              && fprintf(SCD_TRACE, "\t");
  } 
  SCD_TRACE && fprintf(SCD_TRACE, "SCANSTEP=%d\tVARIABLES=%d\tCONSTRAINTS=%d\t",
                       scanStep, LProgram_variableCount(program),
                       LProgram_constraintCount(program))
            && fprintf(SCD_TRACE, "\tSPENT=%g\tTIMER=%g\n",
                       LProgram_timeSpent(program),
                       Formulation_TIMER(this));
  return solved;
}

#ifdef $XCC__h
/**
 * Build the LProgram for this Formulation.
 */
LProgram
Formulation_buildProgram(Formulation this, int lambda, int scanStep, bool feasibility);
#endif//$XCC__h

LProgram
Formulation_buildProgram(Formulation this, int lambda, int scanStep, bool feasibility)
{
  Memory memory = Formulation_MEMORY(this);
  double timeOut = Formulation_TIMEOUT(this);
  LProgram program = Formulation_PROGRAM(this);
  LProgram_delete(Formulation_PROGRAM(this));
  program = LProgram_new(memory, LProgramType_Minimize, timeOut);
  *Formulation__PROGRAM(this) = program;
  SCD_TRACE && fprintf(SCD_TRACE,
                       "*** FORMULATION(%s)\tDATES\tLAMBDA=%d\tSCANSTEP=%d\n",
                       Formulation_HEADNAME(this), lambda, scanStep);
  if (Formulation_LAMBDA(this) != lambda) {
    *Formulation__LAMBDA(this) = lambda;
    Formulation_makeAllPairsLongestPaths(this);
  }
  Formulation_adjustEarlyLateDates(this, scanStep, feasibility);
  IPF_TRACE && fprintf(IPF_TRACE, "\\ FORMULATION(%s)\t", Formulation_HEADNAME(this));
#ifndef _NTRACE
  IPF_TRACE && ETable_prettyFlags(OptimizeRCMSSolving_Table,
                                  Formulation_SOLVING(this), "+", IPF_TRACE);
#endif//_NTRACE
  IPF_TRACE && fprintf(IPF_TRACE, "\tLAMBDA=%d\tHORIZON=%d\n",
                       lambda, Formulation_HORIZON(this));
  IPF_TRACE && Formulation_pretty(this, IPF_TRACE);
  if (Formulation_isDinechin(this)) {
    Formulation_buildVariablesDinechin(this);
    Formulation_buildObjectiveDinechin(this);
    Formulation_buildAssignmentsDinechin(this);
    Formulation_buildDependencesDinechin(this);
    Formulation_buildResourcesDinechin(this);
    Formulation_buildBoundsDinechin(this);
  } else
  if (Formulation_isEicDavAb(this)) {
    Formulation_buildVariablesEicDavAb(this);
    Formulation_buildObjectiveEicDavAb(this);
    Formulation_buildAssignmentsEicDavAb(this);
    Formulation_buildDependencesEicDavAb(this);
    Formulation_buildResourcesEicDavAb(this);
    Formulation_buildBoundsEicDavAb(this);
  } else Except_NEVER(true);
  return program;
}

#ifdef $XCC__h
/**
 * All-pairs longest-paths.
 */
void
Formulation_makeAllPairsLongestPaths(Formulation this);
#endif//$XCC__h

void
Formulation_makeAllPairsLongestPaths(Formulation this)
{
  int lambda = Formulation_LAMBDA(this);
  int32_t firstIndex = Formulation_FIRSTINDEX(this);
  int32_t lastIndex = Formulation_LASTINDEX(this), i, j, k;
  int16_t **delayMatrix = Formulation_DELAYMATRIX(this);
  DDGraphArc **edgeMatrix = Formulation_EDGEMATRIX(this);
  int edgeCount = 0, carriedCount = 0;
  // Initialize matrices.
  for (i = firstIndex; i <= lastIndex; i++) {
    for (j = firstIndex; j <= lastIndex; j++) {
      delayMatrix[i][j] = INT16_MIN;
      edgeMatrix[i][j] = NULL;
    }
    delayMatrix[i][i] = 0;
  }
  // Initialize delays and edges.
  Formulation_FOREACH_DDGraphNode(this, i_node) {
    int32_t i_index = Formulation_NODEINDEX;
    DDGraphNode_FORLEAVE_DDGraphArc(i_node, arc) {
      DDGraphNode j_node = DDGraphArc_HEAD(arc);
      int32_t j_index = DDGraphNode_INDEX(j_node);
      if (j_index != i_index && j_index >= firstIndex && j_index <= lastIndex) {
        int latency = DDGraphArc_THETA(arc) - lambda*DDGraphArc_OMEGA(arc);
        if (delayMatrix[i_index][j_index] < latency) {
          delayMatrix[i_index][j_index] = latency;
          edgeMatrix[i_index][j_index] = arc;
        }
      }
      *DDGraphArc__FLAGS(arc) &= ~NetArcFlag_Longest;
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  } Formulation_ENDEACH_DDGraphNode;
  // Main Floyd-Warshall loop.
  for (k = firstIndex; k <= lastIndex; k++) {
    for (i = firstIndex; i <= lastIndex; i++) {
      for (j = firstIndex; j <= lastIndex; j++) {
        if (delayMatrix[i][j] < delayMatrix[i][k] + delayMatrix[k][j]) {
          delayMatrix[i][j] = delayMatrix[i][k] + delayMatrix[k][j];
          edgeMatrix[i][j] = edgeMatrix[k][j];
        }
      }
    }
  }
  // Collect the longest path arcs.
  for (i = firstIndex; i <= lastIndex; i++) {
    for (j = firstIndex; j <= lastIndex; j++) {
      DDGraphArc edge = edgeMatrix[i][j];
      if (   edge != NULL
          && !(DDGraphArc_FLAGS(edge) & NetArcFlag_Longest)) {
        int slack = -(delayMatrix[i][j] + delayMatrix[j][i]);
        *DDGraphArc__FLAGS(edge) |= NetArcFlag_Longest;
        SCD_TRACE && fprintf(SCD_TRACE, "\t(_%d, _%d)\tslack=%-3d\t\%s\n",
                             DDGraphNode_INDEX(DDGraphArc_TAIL(edge)),
                             DDGraphNode_INDEX(DDGraphArc_HEAD(edge)),
                             slack, DependenceKind_Id(DDGraphArc_KIND(edge)));
        if (DDGraphArc_OMEGA(edge)) carriedCount++;
        edgeCount++;
      }
    }
  }
  SCD_TRACE && fprintf(SCD_TRACE, "\tNODECOUNT=%d\tEDGECOUNT=%d\tCARRIED=%d\n",
                       lastIndex - firstIndex + 1, edgeCount, carriedCount);
}

#ifdef $XCC__h
/**
 * Adjust EARLYDATES, LATEDATES, HORIZON.
 */
void
Formulation_adjustEarlyLateDates(Formulation this, int scanStep, bool feasibility);
#endif//$XCC__h

void
Formulation_adjustEarlyLateDates(Formulation this, int scanStep, bool feasibility)
{
  int lambda = Formulation_LAMBDA(this);
  int scanning = Formulation_SCANNING(this);
  int32_t lastIndex = Formulation_LASTINDEX(this);
  int16_t *earlyDates = Formulation_EARLYDATES(this);
  int16_t *lateDates = Formulation_LATEDATES(this);
  GrahamListTask *index2task = Formulation_INDEX2TASK(this);
  GrahamList grahamList = Formulation_GRAHAMLIST(this);
  int prevLambda = GrahamList_LAMBDA(grahamList);
  PtrSeq nodeStack = Formulation_NODESTACK(this);
  bool *inStack = Formulation_INSTACK(this);
  Formulation_FOREACH_DDGraphNode(this, node) {
    int32_t index = Formulation_NODEINDEX;
    // Processing an operation node.
    GrahamListTask task = index2task[index];
    int prevTau = GrahamListTask_TAU(task);
    int prevPhi = GrahamListTask_PHI(task);
    int prevDate = prevTau + prevLambda*prevPhi;
    int scaledDate = prevTau + lambda*prevPhi;
    Except_CHECK(scaledDate <= prevDate);
    if (scanning > 0) {
      int fudge = (prevPhi + 1)<<scanStep;
      int earlyDate = fudge <= scaledDate? scaledDate - fudge: 0;
      int lateDate = prevDate + fudge/*/2*/;
      earlyDates[index] = earlyDate;
      lateDates[index] = lateDate;
    } else {
      int lambda2 = Formulation_LAMBDA2(this);	// LAMBDA Relaxation.
      earlyDates[index] = GrahamListTask_EARLYDATE(task);
      lateDates[index] = (GrahamListTask_LATEDATE(task)/lambda2 + 1)*lambda;
    }
    Except_CHECK(earlyDates[index] >= 0 && earlyDates[index] <= lateDates[index]);
    SCD_TRACE && fprintf(SCD_TRACE, "    %s\tPREV=%-3d\tEARLY=%-3d\tLATE=%-3d\n",
                                    DDGraphNode_name(node), prevDate,
                                    earlyDates[index], lateDates[index]);
  } Formulation_ENDEACH_DDGraphNode;
  // Dependence-consistent early dates.
  Formulation_FOREACH_DDGraphNode(this, node) {
    PtrSeq_push(nodeStack, node);
    inStack[Formulation_NODEINDEX] = true;
  } Formulation_ENDEACH_DDGraphNode;
  while (!PtrSeq_isEmpty(nodeStack)) {
    DDGraphNode node = PtrSeq_top(nodeStack);
    int32_t index = DDGraphNode_INDEX(node);
    inStack[index] = false;
    PtrSeq_pop(nodeStack);
    DDGraphNode_FORLEAVE_DDGraphArc(node, arc) {
      if ((DDGraphArc_FLAGS(arc) & NetArcFlag_Longest)) {
        DDGraphNode succ_node = DDGraphArc_HEAD(arc);
        int32_t succ_index = DDGraphNode_INDEX(succ_node);
        int latency = DDGraphArc_THETA(arc) - lambda*DDGraphArc_OMEGA(arc);
        if (earlyDates[succ_index] < earlyDates[index] + latency) {
          earlyDates[succ_index] = earlyDates[index] + latency;
          if (!inStack[succ_index]) {
            PtrSeq_push(nodeStack, succ_node);
            inStack[succ_index] = true;
          }
        }
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  }
  // Dependence-consistent late dates.
  Formulation_FOREACH_DDGraphNode(this, node) {
    PtrSeq_push(nodeStack, node);
    inStack[Formulation_NODEINDEX] = true;
  } Formulation_ENDEACH_DDGraphNode;
  while (!PtrSeq_isEmpty(nodeStack)) {
    DDGraphNode node = PtrSeq_top(nodeStack);
    int32_t index = DDGraphNode_INDEX(node);
    inStack[index] = false;
    PtrSeq_pop(nodeStack);
    DDGraphNode_FORENTER_DDGraphArc(node, arc) {
      if ((DDGraphArc_FLAGS(arc) & NetArcFlag_Longest)) {
        DDGraphNode pred_node = DDGraphArc_TAIL(arc);
        int32_t pred_index = DDGraphNode_INDEX(pred_node);
        int latency = DDGraphArc_THETA(arc) - lambda*DDGraphArc_OMEGA(arc);
        if (lateDates[pred_index] > lateDates[index] - latency) {
          lateDates[pred_index] = lateDates[index] - latency;
          if (!inStack[pred_index]) {
            PtrSeq_push(nodeStack, pred_node);
            inStack[pred_index] = true;
          }
        }
      }
    } DDGraphNode_ENDENTER_DDGraphArc;
  }
  // Compute this Formulation HORIZON.
  {
    int horizon = 0;
    Formulation_FOREACH_DDGraphNode(this, node) {
      int32_t index = Formulation_NODEINDEX;
      if (horizon < lateDates[index] + 1) {
        horizon = lateDates[index] + 1;
      }
      Except_CHECK(earlyDates[index] <= lateDates[index]);
    } Formulation_ENDEACH_DDGraphNode;
    *Formulation__HORIZON(this) = horizon;
  }
}

#ifdef $XCC__h
/**
 * Commit the schedule of this Formulation LProgram.
 */
int
Formulation_commitSchedule(Formulation this);
#endif//$XCC__h

int
Formulation_commitSchedule(Formulation this)
{
  int lambda = Formulation_LAMBDA(this);
  int firstDate = INT_MAX, lastDate = -1, t;
  LProgram program = Formulation_PROGRAM(this);
  GrahamList grahamList = Formulation_GRAHAMLIST(this);
  IssueTable issueTable = Formulation_ISSUETABLE(this);
  *GrahamList__LAMBDA(grahamList) = lambda;
  IssueTable_ready(issueTable, lambda);
  if (Formulation_isDinechin(this)) {
    int16_t *earlyDates = Formulation_EARLYDATES(this);
    int16_t *lateDates = Formulation_LATEDATES(this);
    Formulation_FOREACH_GrahamListTask(this, task) {
      IssueItem item = GrahamListTask_ITEM(task);
      long index = Formulation_TASKINDEX;
      int earlyDate = earlyDates[index];
      int lateDate = lateDates[index];
      for (t = earlyDate; t <= lateDate; t++) {
        void *key2 = Formulation_KEY2(index, t);
        LProgramVariable variable = LProgram_accessVariable(program, key2);
        LProgramValue value = LProgramVariable_value(variable);
        const char *name = LProgramVariable_name(variable);
        if (value > 0.5) {
          int tau = t%lambda, phi = t/lambda;
          IssueTable_insert(issueTable, item, tau);
          *GrahamListTask__TAU(task) = tau;
          *GrahamListTask__PHI(task) = phi;
          if (firstDate > t) firstDate = t;
          if (lastDate < t) lastDate = t;
        }
      }
    } Formulation_ENDEACH_GrahamListTask;
  } else
  if (Formulation_isEicDavAb(this)) {
    Formulation_FOREACH_GrahamListTask(this, task) {
      IssueItem item = GrahamListTask_ITEM(task);
      long index = Formulation_TASKINDEX;
      for (t = 0; t < lambda; t++) {
        void *key2 = Formulation_KEY2(index, t);
        LProgramVariable variable = LProgram_accessVariable(program, key2);
        LProgramValue value = LProgramVariable_value(variable);
        const char *name = LProgramVariable_name(variable);
        if (value > 0.5) {
          int tau = t;
          IssueTable_insert(issueTable, item, tau);
          *GrahamListTask__TAU(task) = tau;
        }
      }
      {
        void *key2 = Formulation_KEY2(index, lambda);
        LProgramVariable variable = LProgram_accessVariable(program, key2);
        LProgramValue value = LProgramVariable_value(variable);
        const char *name = LProgramVariable_name(variable);
        int phi = (int)(value + 0.5);
        *GrahamListTask__PHI(task) = phi;
      }
    } Formulation_ENDEACH_GrahamListTask;
  } else Except_NEVER(true);
  // Compute schedule length.
  Formulation_FOREACH_GrahamListTask(this, task) {
    int startDate = GrahamListTask_TAU(task) + lambda*GrahamListTask_PHI(task);
    if (firstDate > startDate) firstDate = startDate;
    if (lastDate < startDate) lastDate = startDate;
  } Formulation_ENDEACH_GrahamListTask;
  //Formulation_verifyDependences(this);
  SCD_TRACE && fprintf(SCD_TRACE, "*** FORMULATION(%s)\tCOMMIT\tLAMBDA=%d\tLENGTH=%d\n",
                       Formulation_HEADNAME(this), lambda, lastDate - firstDate);
  return lastDate - firstDate;
}

#ifdef $XCC__h
void
Formulation_verifyDependences(Formulation this);
#endif//$XCC__h

void
Formulation_verifyDependences(Formulation this)
{
  int lambda = Formulation_LAMBDA(this);
  int32_t firstIndex = Formulation_FIRSTINDEX(this);
  int32_t lastIndex = Formulation_LASTINDEX(this);
  GrahamListTask *index2task = Formulation_INDEX2TASK(this);
  Formulation_FOREACH_DDGraphNode(this, i_node) {
    int32_t i_index = Formulation_NODEINDEX;
    GrahamListTask i_task = index2task[i_index];
    int i_date = GrahamListTask_TAU(i_task) + lambda*GrahamListTask_PHI(i_task);
    DDGraphNode_FORLEAVE_DDGraphArc(i_node, arc) {
      DDGraphNode j_node = DDGraphArc_HEAD(arc);
      int32_t j_index = DDGraphNode_INDEX(j_node);
      if (j_index != i_index && j_index >= firstIndex && j_index <= lastIndex) {
        GrahamListTask j_task = index2task[j_index];
        int j_date = GrahamListTask_TAU(j_task) + lambda*GrahamListTask_PHI(j_task);
        int latency = DDGraphArc_THETA(arc) - lambda*DDGraphArc_OMEGA(arc);
        Except_ALWAYS(i_date + latency <= j_date);
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  } Formulation_ENDEACH_DDGraphNode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Build this Formulation ProgramVariables.
 */
void
Formulation_buildVariablesDinechin(Formulation this);
#endif//$XCC__h

void
Formulation_buildVariablesDinechin(Formulation this)
{
  char buffer[128];
  int16_t *earlyDates = Formulation_EARLYDATES(this);
  int16_t *lateDates = Formulation_LATEDATES(this);
  LProgram program = Formulation_PROGRAM(this);
  // Scheduling variables.
  Formulation_FOREACH_DDGraphNode(this, node) {
    long index = Formulation_NODEINDEX;
    int earlyDate = earlyDates[index];
    int lateDate = lateDates[index], t;
    for (t = earlyDate; t <= lateDate; t++) {
      int length = sprintf(buffer, "%s_%d", DDGraphNode_name(node), t);
      const char *name = Formulation_saveName(this, buffer);
      LProgramVariable variable = Formulation_isRational(this)?
                                  LProgram_makeContinuousVariable(program, name):
                                  LProgram_makeBinaryVariable(program, name);
      if (LProgram_enterVariable(program, variable, Formulation_KEY2(index, t))) {
        Except_CHECK(length < sizeof(buffer));
      } else Except_NEVER(true);
    }
  } Formulation_ENDEACH_DDGraphNode;
  // Register file variables
}

#ifdef $XCC__h
/**
 * 
 *
 * We list all the problem variables, with a zero coefficient when not used in
 * the objective function. This is because CPLEX orders its variables by the
 * occurrence of first reference, and we want CPLEX to keep our order when
 * displaying solutions.
 */
void
Formulation_buildObjectiveDinechin(Formulation this);
#endif//$XCC__h

void
Formulation_buildObjectiveDinechin(Formulation this)
{
  int32_t lastIndex = Formulation_LASTINDEX(this);
  int16_t *earlyDates = Formulation_EARLYDATES(this);
  int16_t *lateDates = Formulation_LATEDATES(this);
  unsigned exitWeight = Formulation_EXITWEIGHT(this);
  LProgram program = Formulation_PROGRAM(this);
  // Minimize \sum_{t=1}^{T-1} t x_{n+1}^t
  Formulation_FOREACH_DDGraphNode(this, node) {
    Operation operation = DDGraphNode_OPERATION(node);
    Operator operator = Operation_operator(operation);
    bool isGoTo = Operator_isGoTo(operator);
    long index = Formulation_NODEINDEX;
    int earlyDate = earlyDates[index];
    int lateDate = lateDates[index], t;
    SCD_TRACE && isGoTo && fprintf(SCD_TRACE, "*** FORMULATION(%s)\tWEIGHT(%d)",
                                              Formulation_HEADNAME(this), exitWeight)
              && Operation_pretty(operation, SCD_TRACE);
    for (t = earlyDate; t <= lateDate; t++) {
      void *key2 = Formulation_KEY2(index, t);
      LProgramVariable variable = LProgram_accessVariable(program, key2);
      LProgramValue weight = (index != lastIndex)?
                             (LProgramValue)t*isGoTo*exitWeight:
                             (LProgramValue)t;
      LProgramVariable_setWeight(variable, weight);
    }
    //exitWeight -= isGoTo;
  } Formulation_ENDEACH_DDGraphNode;
  IPF_TRACE && LProgram_prettyObjective(program, IPF_TRACE);
  IPF_TRACE && fprintf(IPF_TRACE, "Subject To\n");
}

#ifdef $XCC__h
/**
 * 
 */
void
Formulation_buildAssignmentsDinechin(Formulation this);
#endif//$XCC__h

void
Formulation_buildAssignmentsDinechin(Formulation this)
{
  char buffer[128];
  int16_t *earlyDates = Formulation_EARLYDATES(this);
  int16_t *lateDates = Formulation_LATEDATES(this);
  LProgram program = Formulation_PROGRAM(this);
  // \sum_{t=0}^{T-1} x_i^t = 1 \forall i
  Formulation_FOREACH_DDGraphNode(this, node) {
    long index = Formulation_NODEINDEX;
    int length = sprintf(buffer, "assign_%ld", index), t;
    const char *name = Formulation_saveName(this, buffer);
    LProgramConstraint constraint =
        LProgram_makeEQConstraint(program, (LProgramValue)1, name);
    Except_ENSURE(length < sizeof(buffer));
    for (t = earlyDates[index]; t <= lateDates[index]; t++) {
      void *key2 = Formulation_KEY2(index, t);
      LProgramVariable variable = LProgram_accessVariable(program, key2);
      LProgramConstraint_addTerm(constraint, variable, (LProgramValue)1);
    }
    IPF_TRACE && LProgramConstraint_pretty(constraint, IPF_TRACE);
  } Formulation_ENDEACH_DDGraphNode;
}

#ifdef $XCC__h
/**
 * 
 */
void
Formulation_buildDependencesDinechin(Formulation this);
#endif//$XCC__h

void
Formulation_buildDependencesDinechin(Formulation this)
{
  char buffer[128];
  int32_t firstIndex = Formulation_FIRSTINDEX(this);
  int32_t lastIndex = Formulation_LASTINDEX(this);
  int16_t *earlyDates = Formulation_EARLYDATES(this);
  int16_t *lateDates = Formulation_LATEDATES(this);
  LProgram program = Formulation_PROGRAM(this);
  int lambda = Formulation_LAMBDA(this), s, t;
  Formulation_FOREACH_DDGraphNode(this, i_node) {
    long i_index = Formulation_NODEINDEX;
    int i_earlyDate = earlyDates[i_index], i_lateDate = lateDates[i_index];
    DDGraphNode_FORLEAVE_DDGraphArc(i_node, arc) {
      DDGraphNode j_node = DDGraphArc_HEAD(arc);
      long j_index = DDGraphNode_INDEX(j_node);
      if (   j_index >= firstIndex && j_index <= lastIndex
          && (DDGraphArc_FLAGS(arc) & NetArcFlag_Longest)) {
        int j_earlyDate = earlyDates[j_index], j_lateDate = lateDates[j_index];
        int latency = DDGraphArc_THETA(arc) - lambda*DDGraphArc_OMEGA(arc);
        IPF_TRACE && fprintf(IPF_TRACE, "\\ Arc(_%ld, _%ld)\tlatency=%d\t\%s\n",
            i_index, j_index, latency, DependenceKind_Id(DDGraphArc_KIND(arc)));
        if (Formulation_isAggrDeps(this)) {
          // \sum_{t=0}^{D_i-p_i} t x_i^t - \sum_{t=0}^{D_j-p_j} t x_j^t \le latency
          int length = sprintf(buffer, "dep_%ld_%ld", i_index, j_index);
          const char *name = Formulation_saveName(this, buffer);
          LProgramConstraint constraint =
              LProgram_makeLEConstraint(program, (LProgramValue)-latency, name);
          Except_ENSURE(length < sizeof(buffer));
          for (t = i_earlyDate; t <= j_lateDate; t++) {
            void *i_key2 = Formulation_KEY2(i_index, t);
            LProgramVariable i_variable = 
                  LProgram_accessVariable(program, i_key2);
            LProgramConstraint_addTerm(constraint, i_variable,
                                            (LProgramValue)t);
          }
          for (t = j_earlyDate; t <= j_lateDate; t++) {
            void *j_key2 = Formulation_KEY2(j_index, t);
            LProgramVariable j_variable = 
                  LProgram_accessVariable(program, j_key2);
            LProgramConstraint_addTerm(constraint, j_variable,
                                            (LProgramValue)-t);
          }
        } else {
          // \sum_{s=t}^{D_i-p_i} x_i^s +
          // \sum_{s=0}^{\min(t+latency-1,D_j-p_j)} x_j^s \le 1
          // Loop \forall t \in [0, horizon-1]. However, we can skip the  equations if:
          //   * t > i_lateDate => \sum_{s=t}^{T-1} x_i^s = 0
          //   * t+latency-1 < j_earlyDate => \sum_{s=0}^{t+latency-1} x_j^s = 0
          // So t \in [0, horizon-1] \inter [ j_earlyDate-latency+1, i_lateDate ]
          int t_first = _MAX(0, j_earlyDate - latency + 1);
          int t_last = i_lateDate;
          for (t = t_first; t <= t_last; t++) {
            int i_firstDate = _MAX(t, i_earlyDate);
            int i_lastDate = i_lateDate;
            int j_firstDate = j_earlyDate;
            int j_lastDate = _MIN(t + latency - 1, j_lateDate);
            int length = sprintf(buffer, "dep_%ld_%ld_%d", i_index, j_index, t);
            const char *name = Formulation_saveName(this, buffer);
            LProgramConstraint constraint =
                LProgram_makeLEConstraint(program, (LProgramValue)1, name);
            Except_ENSURE(length < sizeof(buffer));
            for (s = i_firstDate; s <= i_lastDate; s++) {
              void *i_key2 = Formulation_KEY2(i_index, s);
              LProgramVariable i_variable =
                  LProgram_accessVariable(program, i_key2);
              LProgramConstraint_addTerm(constraint, i_variable,
                                              (LProgramValue)1);
            }
            for (s = j_firstDate; s <= j_lastDate; s++) {
              void *j_key2 = Formulation_KEY2(j_index, s);
              LProgramVariable j_variable =
                  LProgram_accessVariable(program, j_key2);
              LProgramConstraint_addTerm(constraint, j_variable,
                                              (LProgramValue)1);
            }
            IPF_TRACE && LProgramConstraint_pretty(constraint, IPF_TRACE);
          }
        }
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  } Formulation_ENDEACH_DDGraphNode;
}

#ifdef $XCC__h
/**
 * 
 *
 * \sum_{i=1}^{n} \sum{k=0}^{\div{D_i-p_i}{\lambda}} x_i^{t+k\lambda} b_i^r \le B_i^r
 *
 * The \sum{s=t+k\lambda-p_i+1}^{t+k\lambda} x_i^s b_i^r has been replaced by
 * x_i^{t+k\lambda} b_i^r because p_i is assumed to be 1 (UET operations).
 */
void
Formulation_buildResourcesDinechin(Formulation this);
#endif//$XCC__h

void
Formulation_buildResourcesDinechin(Formulation this)
{
  char buffer[128];
  DDGraphNode *index2node = Formulation_INDEX2NODE(this);
  int16_t *earlyDates = Formulation_EARLYDATES(this);
  int16_t *lateDates = Formulation_LATEDATES(this);
  LProgram program = Formulation_PROGRAM(this);
  IssueTable issueTable = Formulation_ISSUETABLE(this);
  const uint8_t *availability = IssueTable_availability(issueTable);
  int32_t *counters = IssueTable_counters(issueTable);
  int horizon = Formulation_HORIZON(this);
  int lambda = Formulation_LAMBDA(this), k, s, t;
  // Iterate over the used resources.
  IssueTable_FOREACH_Resource(issueTable, resource) {
    if (counters[resource] != 0) {
      int64_t available = availability[resource];
      // Here we need a constraint per resource unit per t.
      IPF_TRACE && fprintf(IPF_TRACE, "\\ %s\tlimit=%d\tuses=%d\n",
          Resource_name(resource), (int)available, counters[resource]);
      for (t = 0; t < lambda; t++) {
        int64_t maxRequired = 0;
#ifndef _NTRACE
        int length = sprintf(buffer, "%s_%d", Resource_name(resource), t);
        const char *name = Formulation_saveName(this, buffer);
#else//_NTRACE
        int length = 0;
        const char *name = NULL;
#endif//_NTRACE
        LProgramConstraint constraint =
            LProgram_makeLEConstraint(program, (LProgramValue)available, name);
        Except_ENSURE(length < sizeof(buffer));
        // Iterate over the GrahamListTask(s) to access Scheduling(s).
        Formulation_FOREACH_GrahamListTask(this, task) {
          IssueItem item = GrahamListTask_ITEM(task);
          long index = Formulation_TASKINDEX;
          DDGraphNode node = index2node[index];
          int required = IssueItem_REQUIREMENTS(item)[resource];
          if (required != 0) {
            bool hasConstraints = false;
            // Requirements of operation i for resource unit.
            for (k = 0; k*lambda < horizon; k++) {
              s = t + k*lambda;
              if (s >= earlyDates[index] && s <= lateDates[index]) {
                void *key2 = Formulation_KEY2(index, s);
                LProgramVariable variable =
                    LProgram_accessVariable(program, key2);
                LProgramConstraint_addTerm(constraint, variable,
                                                (LProgramValue)required);
                hasConstraints = true;
              }
            }
            maxRequired += required*hasConstraints;
          }
        } Formulation_ENDEACH_GrahamListTask;
        if (maxRequired <= available) {
          LProgram_killConstraint(program, constraint);
        } else
        IPF_TRACE && LProgramConstraint_pretty(constraint, IPF_TRACE);
      }
    }
  } IssueTable_ENDEACH_Resource;
}

#ifdef $XCC__h
/**
 * 
 */
void
Formulation_buildBoundsDinechin(Formulation this);
#endif//$XCC__h

void
Formulation_buildBoundsDinechin(Formulation this)
{
  int16_t *earlyDates = Formulation_EARLYDATES(this);
  int16_t *lateDates = Formulation_LATEDATES(this);
  LProgram program = Formulation_PROGRAM(this);
  IPF_TRACE && fprintf(IPF_TRACE, "Bounds\n");
  if (!Formulation_isRational(this)) {
    IPF_TRACE && fprintf(IPF_TRACE, "Binaries\n");
  }
  // x_i^t \in \{ 0, 1 \} \forall i, \forall t
  Formulation_FOREACH_DDGraphNode(this, node) {
    long index = Formulation_NODEINDEX, t;
    for (t = earlyDates[index]; t <= lateDates[index]; t++) {
      void *key2 = Formulation_KEY2(index, t);
      LProgramVariable variable = LProgram_accessVariable(program, key2);
      const char *name = LProgramVariable_name(variable);
      if (Formulation_isRational(this)) {
        LProgramVariable_setLower(variable, (LProgramValue)0);
        LProgramVariable_setUpper(variable, (LProgramValue)1);
        IPF_TRACE && fprintf(IPF_TRACE, "  0 <= %s <= 1\n", name);
      } else {
        IPF_TRACE && fprintf(IPF_TRACE, "  %s\n", name);
      }
    }
  } Formulation_ENDEACH_DDGraphNode;
  IPF_TRACE && fprintf(IPF_TRACE, "End\n\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Build this Formulation ProgramVariables.
 */
void
Formulation_buildVariablesEicDavAb(Formulation this);
#endif//$XCC__h

void
Formulation_buildVariablesEicDavAb(Formulation this)
{
  char buffer[128];
  int lambda = Formulation_LAMBDA(this), t;
  int16_t *lateDates = Formulation_LATEDATES(this);
  LProgram program = Formulation_PROGRAM(this);
  // Scheduling variables.
  Formulation_FOREACH_DDGraphNode(this, node) {
    long index = Formulation_NODEINDEX;
    for (t = 0; t <= lambda; t++) {
      int length = sprintf(buffer, "%s_%d", DDGraphNode_name(node), t);
      const char *name = Formulation_saveName(this, buffer);
      LProgramVariable variable = NULL;
      if (Formulation_isRational(this)) {
        variable = LProgram_makeContinuousVariable(program, name);
      } else {
        variable = t < lambda?
                   LProgram_makeBinaryVariable(program, name):
                   LProgram_makeIntegerVariable(program, name);
      }
      if (LProgram_enterVariable(program, variable, Formulation_KEY2(index, t))) {
        Except_CHECK(length < sizeof(buffer));
      } else Except_NEVER(true);
    }
  } Formulation_ENDEACH_DDGraphNode;
}

#ifdef $XCC__h
/**
 * 
 *
 * We list all the problem variables, with a zero coefficient when not used in
 * the objective function. This is because CPLEX orders its variables by the
 * occurrence of first reference, and we want CPLEX to keep our order when
 * displaying solutions.
 */
void
Formulation_buildObjectiveEicDavAb(Formulation this);
#endif//$XCC__h

void
Formulation_buildObjectiveEicDavAb(Formulation this)
{
  int lambda = Formulation_LAMBDA(this), t;
  int32_t lastIndex = Formulation_LASTINDEX(this);
  unsigned exitWeight = Formulation_EXITWEIGHT(this);
  LProgram program = Formulation_PROGRAM(this);
  // Minimize \sum_{t=0}^{\lambda-1} t y_{n+1}^t + k_{n+1} \lambda
  Formulation_FOREACH_DDGraphNode(this, node) {
    Operation operation = DDGraphNode_OPERATION(node);
    Operator operator = Operation_operator(operation);
    bool isGoTo = Operator_isGoTo(operator);
    long index = Formulation_NODEINDEX;
    SCD_TRACE && isGoTo && fprintf(SCD_TRACE, "*** FORMULATION(%s)\tWEIGHT(%d)",
                                              Formulation_HEADNAME(this), exitWeight)
              && Operation_pretty(operation, SCD_TRACE);
    for (t = 0; t <= lambda; t++) {
      void *key2 = Formulation_KEY2(index, t);
      LProgramVariable variable = LProgram_accessVariable(program, key2);
      LProgramValue weight = (index != lastIndex)?
                             (LProgramValue)t*isGoTo*exitWeight:
                             (LProgramValue)t;
      LProgramVariable_setWeight(variable, weight);
    }
  } Formulation_ENDEACH_DDGraphNode;
  IPF_TRACE && LProgram_prettyObjective(program, IPF_TRACE);
  IPF_TRACE && fprintf(IPF_TRACE, "Subject To\n");
}

#ifdef $XCC__h
/**
 * 
 */
void
Formulation_buildAssignmentsEicDavAb(Formulation this);
#endif//$XCC__h

void
Formulation_buildAssignmentsEicDavAb(Formulation this)
{
  char buffer[128];
  int lambda = Formulation_LAMBDA(this), t;
  LProgram program = Formulation_PROGRAM(this);
  // \sum_{t=0}^{lambda} y_i^t = 1
  Formulation_FOREACH_DDGraphNode(this, node) {
    long index = Formulation_NODEINDEX;
    int length = sprintf(buffer, "assign_%ld", index), t;
    const char *name = Formulation_saveName(this, buffer);
    LProgramConstraint constraint =
        LProgram_makeEQConstraint(program, (LProgramValue)1, name);
    Except_ENSURE(length < sizeof(buffer));
    for (t = 0; t < lambda; t++) {
      void *key2 = Formulation_KEY2(index, t);
      LProgramVariable variable = LProgram_accessVariable(program, key2);
      LProgramConstraint_addTerm(constraint, variable, (LProgramValue)1);
    }
    IPF_TRACE && LProgramConstraint_pretty(constraint, IPF_TRACE);
  } Formulation_ENDEACH_DDGraphNode;
}

#ifdef $XCC__h
/**
 * 
 */
void
Formulation_buildDependencesEicDavAb(Formulation this);
#endif//$XCC__h

void
Formulation_buildDependencesEicDavAb(Formulation this)
{
  char buffer[128];
  int32_t firstIndex = Formulation_FIRSTINDEX(this);
  int32_t lastIndex = Formulation_LASTINDEX(this);
  LProgram program = Formulation_PROGRAM(this);
  int lambda = Formulation_LAMBDA(this), s, t;
  Formulation_FOREACH_DDGraphNode(this, i_node) {
    long i_index = Formulation_NODEINDEX;
    DDGraphNode_FORLEAVE_DDGraphArc(i_node, arc) {
      DDGraphNode j_node = DDGraphArc_HEAD(arc);
      long j_index = DDGraphNode_INDEX(j_node);
      if (   j_index >= firstIndex && j_index <= lastIndex
          && (DDGraphArc_FLAGS(arc) & NetArcFlag_Longest)) {
        int theta = DDGraphArc_THETA(arc), omega = DDGraphArc_OMEGA(arc);
        int latency = theta - lambda*omega;
        IPF_TRACE && fprintf(IPF_TRACE, "\\ Arc(_%ld, _%ld)\tlatency=%d\t\%s\n",
            i_index, j_index, latency, DependenceKind_Id(DDGraphArc_KIND(arc)));
        if (Formulation_isAggrDeps(this)) {
          // \sum_{t=0}^{\lambda-1} t y_i^t + k_i \lambda +
          // \sum_{t=0}^{\lambda-1} -t y_j^t - k_j \lambda \le -latency
          int length = sprintf(buffer, "dep_%ld_%ld", i_index, j_index);
          const char *name = Formulation_saveName(this, buffer);
          LProgramConstraint constraint =
              LProgram_makeLEConstraint(program, (LProgramValue)-latency, name);
          Except_ENSURE(length < sizeof(buffer));
          for (t = 0; t <= lambda; t++) {
            void *i_key2 = Formulation_KEY2(i_index, t);
            LProgramVariable i_variable = 
                  LProgram_accessVariable(program, i_key2);
            LProgramConstraint_addTerm(constraint, i_variable,
                                            (LProgramValue)t);
          }
          for (t = 0; t <= lambda; t++) {
            void *j_key2 = Formulation_KEY2(j_index, t);
            LProgramVariable j_variable = 
                  LProgram_accessVariable(program, j_key2);
            LProgramConstraint_addTerm(constraint, j_variable,
                                            (LProgramValue)-t);
          }
        } else {
          // \sum_{s=t}^{\lambda-1} y_i^s +
          // \sum_{s=0}^{\mod{t+\theta_i^j-1}{\lambda}} y_j^s + k_i - k_j
          // \le \omega_i^j - \div{t + \theta_i^j - 1}{\lambda} + 1
          for (t = 0; t < lambda; t++) {
            int term = t + theta - 1;
            int mod = term >= 0? term%lambda: (term + lambda)%lambda;
            int div = term >= 0? term/lambda: (term - lambda)/lambda;
            int bound = omega - div + 1;
            int length = sprintf(buffer, "dep_%ld_%ld_%d", i_index, j_index, t);
            const char *name = Formulation_saveName(this, buffer);
            LProgramConstraint constraint =
                LProgram_makeLEConstraint(program, (LProgramValue)bound, name);
            Except_ENSURE(length < sizeof(buffer));
            for (s = t; s <= lambda - 1; s++) {
              void *i_key2 = Formulation_KEY2(i_index, s);
              LProgramVariable i_variable =
                  LProgram_accessVariable(program, i_key2);
              LProgramConstraint_addTerm(constraint, i_variable,
                                              (LProgramValue)1);
            }
            {
              void *i_key2 = Formulation_KEY2(i_index, lambda);
              LProgramVariable i_variable =
                  LProgram_accessVariable(program, i_key2);
              LProgramConstraint_addTerm(constraint, i_variable,
                                              (LProgramValue)1);
            }
            for (s = 0; s <= mod; s++) {
              void *j_key2 = Formulation_KEY2(j_index, s);
              LProgramVariable j_variable =
                  LProgram_accessVariable(program, j_key2);
              LProgramConstraint_addTerm(constraint, j_variable,
                                              (LProgramValue)1);
            }
            {
              void *j_key2 = Formulation_KEY2(j_index, lambda);
              LProgramVariable j_variable =
                  LProgram_accessVariable(program, j_key2);
              LProgramConstraint_addTerm(constraint, j_variable,
                                              (LProgramValue)-1);
            }
            IPF_TRACE && LProgramConstraint_pretty(constraint, IPF_TRACE);
          }
        }
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  } Formulation_ENDEACH_DDGraphNode;
}

#ifdef $XCC__h
/**
 * \sum_{i=1}^{n}
 *   \sum{s=0}^{p_i-1} y_i^{\mod{t-s}{\lambda}} b_i^r \le B_i^r \forall t, \forall r
 *
 * The \sum{s=0}^{p_i-1} y_i^{\mod{t-s}{\lambda}} b_i^r has been replaced by
 * y_i^{t} b_i^r because p_i is assumed to be 1 (UET operations).
 */
void
Formulation_buildResourcesEicDavAb(Formulation this);
#endif//$XCC__h

void
Formulation_buildResourcesEicDavAb(Formulation this)
{
  char buffer[128];
  DDGraphNode *index2node = Formulation_INDEX2NODE(this);
  LProgram program = Formulation_PROGRAM(this);
  IssueTable issueTable = Formulation_ISSUETABLE(this);
  const uint8_t *availability = IssueTable_availability(issueTable);
  int32_t *counters = IssueTable_counters(issueTable);
  int lambda = Formulation_LAMBDA(this), t;
  // Iterate over the used resources.
  IssueTable_FOREACH_Resource(issueTable, resource) {
    if (counters[resource] != 0) {
      int64_t available = availability[resource];
      // Here we need a constraint per resource unit per t.
      IPF_TRACE && fprintf(IPF_TRACE, "\\ %s\tlimit=%d\tuses=%d\n",
          Resource_name(resource), (int)available, counters[resource]);
      for (t = 0; t < lambda; t++) {
        int64_t maxRequired = 0;
#ifndef _NTRACE
        int length = sprintf(buffer, "%s_%d", Resource_name(resource), t);
        const char *name = Formulation_saveName(this, buffer);
#else//_NTRACE
        int length = 0;
        const char *name = NULL;
#endif//_NTRACE
        LProgramConstraint constraint =
            LProgram_makeLEConstraint(program, (LProgramValue)available, name);
        Except_ENSURE(length < sizeof(buffer));
        // Iterate over the GrahamListTask(s) to access Scheduling(s).
        Formulation_FOREACH_GrahamListTask(this, task) {
          IssueItem item = GrahamListTask_ITEM(task);
          long index = Formulation_TASKINDEX;
          DDGraphNode node = index2node[index];
          int required = IssueItem_REQUIREMENTS(item)[resource];
          if (required != 0) {
            // Requirements of operation i for resource unit.
            void *key2 = Formulation_KEY2(index, t);
            LProgramVariable variable =
                LProgram_accessVariable(program, key2);
            LProgramConstraint_addTerm(constraint, variable,
                                            (LProgramValue)required);
            maxRequired += required;
          }
        } Formulation_ENDEACH_GrahamListTask;
        if (maxRequired <= available) {
          LProgram_killConstraint(program, constraint);
        } else
        IPF_TRACE && LProgramConstraint_pretty(constraint, IPF_TRACE);
      }
    }
  } IssueTable_ENDEACH_Resource;
}

#ifdef $XCC__h
/**
 * y_i^t \in \{ 0, 1 \} \forall i, \forall t
 */
void
Formulation_buildBoundsEicDavAb(Formulation this);
#endif//$XCC__h

void
Formulation_buildBoundsEicDavAb(Formulation this)
{
  int lambda = Formulation_LAMBDA(this), t;
  int16_t *earlyDates = Formulation_EARLYDATES(this);
  int16_t *lateDates = Formulation_LATEDATES(this);
  LProgram program = Formulation_PROGRAM(this);
  IPF_TRACE && fprintf(IPF_TRACE, "Bounds\n");
  Formulation_FOREACH_DDGraphNode(this, node) {
    long index = Formulation_NODEINDEX;
    void *key2 = Formulation_KEY2(index, lambda);
    LProgramVariable variable = LProgram_accessVariable(program, key2);
    const char *name = LProgramVariable_name(variable);
    int lower = earlyDates[index]/lambda, upper = lateDates[index]/lambda;
    LProgramVariable_setLower(variable, (LProgramValue)lower);
    LProgramVariable_setUpper(variable, (LProgramValue)upper);
    IPF_TRACE && fprintf(IPF_TRACE, "  %d <= %s <= %d\n", lower, name, upper);
  } Formulation_ENDEACH_DDGraphNode;
  if (!Formulation_isRational(this)) {
    IPF_TRACE && fprintf(IPF_TRACE, "Binaries\n");
  }
  Formulation_FOREACH_DDGraphNode(this, node) {
    long index = Formulation_NODEINDEX;
    for (t = 0; t < lambda; t++) {
      void *key2 = Formulation_KEY2(index, t);
      LProgramVariable variable = LProgram_accessVariable(program, key2);
      const char *name = LProgramVariable_name(variable);
      if (Formulation_isRational(this)) {
        LProgramVariable_setLower(variable, (LProgramValue)0);
        LProgramVariable_setUpper(variable, (LProgramValue)1);
        IPF_TRACE && fprintf(IPF_TRACE, "  0 <= %s <= 1\n", name);
      } else {
        IPF_TRACE && fprintf(IPF_TRACE, "  %s\n", name);
      }
    }
  } Formulation_ENDEACH_DDGraphNode;
  if (!Formulation_isRational(this)) {
    IPF_TRACE && fprintf(IPF_TRACE, "Generals\n");
    Formulation_FOREACH_DDGraphNode(this, node) {
      long index = Formulation_NODEINDEX;
      void *key2 = Formulation_KEY2(index, lambda);
      LProgramVariable variable = LProgram_accessVariable(program, key2);
      const char *name = LProgramVariable_name(variable);
      IPF_TRACE && fprintf(IPF_TRACE, "  %s\n", name);
    } Formulation_ENDEACH_DDGraphNode;
  }
  IPF_TRACE && fprintf(IPF_TRACE, "End\n\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize Formulation.
 */
#define Formulation_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize Formulation.
 */
#define Formulation_FINI()
#endif//$XCC__h

#if XCC__C
static void
Formulation_TEST(void)
{
#include "SCD/Formulation_.c"
}

int
main(int argc, char **argv)
{
  SCD_Test(Formulation_TEST);
  return 0;
}
#endif

