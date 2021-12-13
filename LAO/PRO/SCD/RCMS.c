#ifdef $XCC_h
/*
 * !!!!	RCMS.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Frederic Brault (frederic.brault@inria.fr).
 *
 * Copyright 2008 STMicroelectronics.
 * Copyright 2008 INRIA Orsay
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
 * @brief Interface to the RCMS component.
 */
#endif//$XCC_h


#include "SCD/SCD_.h"

#ifdef $XCC__h
#include "SCD/RCMS.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
typedef int32_t RCMSNodeId;
typedef int32_t RCMSArcId;
typedef int RCMSResourceId;
typedef int RCMSReservationId;
typedef int RCMSRegFileId;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Enumerate the RCMS statuses.
 */
typedef enum {
  RCMSStatus_Success,
  RCMSStatus_Failure,
} RCMSStatus;
#endif//$XCC_h

#ifdef $XCC_h
/**
 *  Enumerate the RCMS node types.
 */
typedef enum {
  RCMSNodeType_Root,		// Root node in case of margins.
  RCMSNodeType_Dummy,		// Dummy node such as START and STOP.
  RCMSNodeType_Actual,		// Actual node to schedule.
  RCMSNodeType_Killer,		// Lifetime killer node.
  RCMSNodeType__
} RCMSNodeType;
extern const char *
RCMSNodeType_Id(RCMSNodeType this);
#endif//$XCC_h

const char *
RCMSNodeType_Id(RCMSNodeType this)
{
  static const char *_[] = {
    "Root",
    "Dummy",
    "Actual",
    "Killer",
  };
  Except_REQUIRE((unsigned)this < RCMSNodeType__);
  return _[this];
}

#ifdef $XCC_h
/**
 * Enumerate the RCMS arc types.
 */
typedef enum {
  RCMSArcType_Relax,		// Relaxed dependence arc.
  RCMSArcType_Flow,		// Flow dependence arc.
  RCMSArcType_Anti,		// Anti dependence arc.
  RCMSArcType_Output,		// Output dependence arc.
  RCMSArcType_Life,		// Consumer to killer arc.
  RCMSArcType_Link,		// Producer to killer arc.
  RCMSArcType_Reuse,		// Register reuse arc.
  RCMSArcType_Some,		// Some dependence arc.
  RCMSArcType_Margin,		// Margin-enforcing arc.
  RCMSArcType__
} RCMSArcType;
extern const char *
RCMSArcType_Id(RCMSArcType this);
#endif//$XCC_h

const char *
RCMSArcType_Id(RCMSArcType this)
{
  static const char *_[] = {
    "Relax",
    "Flow",
    "Anti",
    "Output",
    "Life",
    "Link",
    "Reuse",
    "Some",
    "Margin",
  };
  Except_REQUIRE((unsigned)this < RCMSArcType__);
  return _[this];
}

#ifdef $XCC_h
/**
 * Callback structure for the RCMS.
 */
typedef struct RCMSCallback_ {
  void *Context;
  /* Context to provide as first argument to RCMSCallback methods.
  */
  RCMSStatus (*AddArc)(void *context, RCMSNodeId source_nodeId, RCMSNodeId target_nodeId,
                       RCMSArcType arcType, RCMSRegFileId regFileId, int latency, int distance);
  /* Add an arc to the LAO graph from RCMS.
   * regFileId is 0 if arc type is RCMSArcType_Other.
   */
  RCMSStatus (*FreezeNode)(void *context, RCMSNodeId nodeId, int tau, int phi);
  /* Freeze a node at the date tau + II*phi.
   */
} RCMSCallback_, *RCMSCallback;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Interface structure for the RCMS.
 */
typedef struct RCMSInterface_ {
  RCMSStatus (*Open)(const char *procedure_name, const char *label_name, const char *options);
  /* First function to call.
   * options is a string formatted like the RCMS command line.
   * Return:	RCMSStatus.
   */
  RCMSResourceId (*MakeResource)(const char *name, int num_available);
  /* Make a new resource of the given name and the number of available units.
   * Return:	RCMSResourceId.
   */
  RCMSReservationId (*MakeReservation)(const char *name, int num_entries, int entries[]);
  /* Make a new reservation table of the given name and provide the first
   * num_entries of the table, assuming column order. The rows correspond
   * to the resources previously declared by MakeResource.
   * Return:	RCMSReservationId.
   */
  RCMSRegFileId (*MakeRegFile)(const char *name, int num_allocatable);
  /* Make a new RegFile of the given name and the number of allocatable registers.
   * Return:	RCMSRegFileId.
   */
  RCMSNodeId (*MakeNode)(RCMSReservationId reservationId, RCMSNodeType nodeType,
                         int num_write, RCMSRegFileId regFileIds[], const char *operator);
  /* Make a new dependence node with num_write writes to allocatable registers.
   * The variable argument list is an array of num_write RCMSRegFileId(s).
   * Return:	RCMSNodeId.
   */
  RCMSArcId (*MakeArc)(RCMSNodeId source_nodeId, RCMSNodeId target_nodeId, RCMSArcType arcType,
                       RCMSRegFileId regFileId, int latency, int distance, const char *temporary);
  /* Make a new dependence arc with given type and destination Regfile.
   * regFileId is 0 if arc type is RCMSArcType_Other.
   * Return:	RCMSArcId.
   */
  int (*Process)(int minII, int latency, int distance, RCMSCallback callback);
  /* Call RCMS, giving it a minimum II, a latency and distance for the time horizon,
   * and a callback structure.
   * Return:	The new minimum II.
   */
  RCMSStatus (*Close)(void);
  /* Close RCMS after each Process.
   * Return:	RCMSStatus.
   */
  size_t SizeOf;
  /* The current size of struct RCMSInterface_, for compatibility checking.
   */
} RCMSInterface_, *RCMSInterface;
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Stub RCMSInterface_.
 */
RCMSInterface_ RCMS_InterfaceStub_ = {
  RCMS_OpenStub,
  RCMS_MakeResourceStub,
  RCMS_MakeReservationStub,
  RCMS_MakeRegFileStub,
  RCMS_MakeNodeStub,
  RCMS_MakeArcStub,
  RCMS_ProcessStub,
  RCMS_CloseStub,
  sizeof(RCMSInterface_),
};

int32_t RCMS_ResourceCounter;
int32_t RCMS_ReservationCounter;
int32_t RCMS_RegFileCounter;
int32_t RCMS_NodeCounter;
int32_t RCMS_ArcCounter;
HTable_ RCMS_Mnemonics[1];

#ifdef $XCC__h
RCMSStatus
RCMS_OpenStub(const char *procedure_name, const char *label_name, const char *options);
#endif//$XCC__h

RCMSStatus
RCMS_OpenStub(const char *procedure_name, const char *label_name, const char *options)
{
  RCMSStatus status = RCMSStatus_Success;
  SCD_TRACE && fprintf(SCD_TRACE, "RCMS_Open(\"%s\", \"%s\", \"%s\"",
                                  procedure_name, label_name, options);
  SCD_TRACE && fprintf(SCD_TRACE, ")\t=> status=%d\n", status);
  if (RCMS_XML) {
    fputs("<?xml version='1.0' encoding='UTF-8'?>\n", RCMS_XML);
    fputs("<!DOCTYPE Problem SYSTEM 'RCMS.dtd'>\n", RCMS_XML);
    fprintf(RCMS_XML, "<Problem procedure='%s' codeBlock='%s'>\n", procedure_name, label_name);
  }
  RCMS_ResourceCounter = 0;
  RCMS_ReservationCounter = 0;
  RCMS_RegFileCounter = 0;
  RCMS_NodeCounter = 0;
  RCMS_ArcCounter = 0;
  HTable_Ctor(RCMS_Mnemonics, Memory_Root, 100);
  return status;
}

#ifdef $XCC__h
RCMSResourceId
RCMS_MakeResourceStub(const char *name, int num_available);
#endif//$XCC__h

RCMSResourceId
RCMS_MakeResourceStub(const char *name, int num_available)
{
  RCMSResourceId resourceId = RCMS_ResourceCounter++;
  SCD_TRACE && fprintf(SCD_TRACE, "RCMS_MakeResource(\"%s\", available=%d",
                                  name, num_available);
  SCD_TRACE && fprintf(SCD_TRACE, ")\t=> resourceId=%d\n", resourceId);
  if (RCMS_XML && resourceId > 0) {
    fprintf(RCMS_XML, "  <Resource ID='R_%s' available='%d'/>\n", name, num_available);
  }
  return resourceId;
}

#ifdef $XCC__h
RCMSReservationId
RCMS_MakeReservationStub(const char *name, int num_entries, int entries[]);
#endif//$XCC__h

RCMSReservationId
RCMS_MakeReservationStub(const char *name, int num_entries, int entries[])
{
  int i;
  RCMSReservationId reservationId = RCMS_ReservationCounter++;
  SCD_TRACE && fprintf(SCD_TRACE, "RCMS_MakeReservation(\"%s\", num_entries=%d, entries={ ",
                                  name, num_entries);
  for (i = 0; i < num_entries; i++) {
    SCD_TRACE && fprintf(SCD_TRACE, "%d, ", entries[i]);
  }
  SCD_TRACE && fprintf(SCD_TRACE, "})\t=> reservationId=%d\n", reservationId);
  if (RCMS_XML && reservationId > 0) {
    fprintf(RCMS_XML, "  <Reservation ID='%s'", name);
    if (num_entries > 0) {
      char sep = '\'';
      fprintf(RCMS_XML, " entries=");
      for (i = 0; i < num_entries; i++) {
        if (i%RCMS_ResourceCounter == 0) continue;
        fprintf(RCMS_XML, "%c%d", sep, entries[i]);
        sep = ' ';
      }
      fprintf(RCMS_XML, "'");
    }
    fprintf(RCMS_XML, "/>\n");
  }
  return reservationId;
}

#ifdef $XCC__h
RCMSRegFileId
RCMS_MakeRegFileStub(const char *name, int num_allocatable);
#endif//$XCC__h

RCMSRegFileId
RCMS_MakeRegFileStub(const char *name, int num_allocatable)
{
  RCMSRegFileId regFileId = RCMS_RegFileCounter++;
  SCD_TRACE && fprintf(SCD_TRACE, "RCMS_MakeRegFile(\"%s\", num_allocatable=%d",
                                  name, num_allocatable);
  SCD_TRACE && fprintf(SCD_TRACE, ")\t=> regFileId=%d\n", regFileId);
  if (RCMS_XML && regFileId > 0) {
    fprintf(RCMS_XML, "  <RegFile ID='%s' capacity='%d'/>\n", name, num_allocatable);
  }
  return regFileId;
}

#ifdef $XCC__h
RCMSNodeId
RCMS_MakeNodeStub(RCMSReservationId reservationId, RCMSNodeType nodeType,
                  int num_write, RCMSRegFileId regFileIds[], const char *mnemonic);
#endif//$XCC__h

RCMSNodeId
RCMS_MakeNodeStub(RCMSReservationId reservationId, RCMSNodeType nodeType,
                  int num_write, RCMSRegFileId regFileIds[], const char *mnemonic)
{
  RCMSNodeId nodeId = RCMS_NodeCounter++;
  SCD_TRACE && fprintf(SCD_TRACE, "RCMS_MakeNode(reservationId=%d, nodeType=%s, num_write=%d",
                                  reservationId, RCMSNodeType_Id(nodeType), num_write);
  SCD_TRACE && mnemonic && fprintf(SCD_TRACE, ", %s", mnemonic);
  SCD_TRACE && fprintf(SCD_TRACE, ")\t=> nodeId=%d\n", nodeId);
  if (RCMS_XML && nodeId > 0) {
    int i;
    if (mnemonic == NULL) mnemonic = "O";
    *(const char **)HTable_insert(RCMS_Mnemonics, (HTableKey)(uintptr_t)nodeId, sizeof(void *)) = mnemonic;
    fprintf(RCMS_XML, "  <Operation ID='%s_%d' type='%s'",
                      mnemonic, nodeId, RCMSNodeType_Id(nodeType));
    if (reservationId > 0) {
      fprintf(RCMS_XML, " reservation='%s'", Reservation_name(reservationId));
    }
    if (num_write > 0) {
      char sep = '\'';
      fprintf(RCMS_XML, " written=");
      for (i = 0; i < num_write; i++) {
        fprintf(RCMS_XML, "%c%s", sep, RegFile_name(regFileIds[i]));
        sep = ' ';
      }
      fprintf(RCMS_XML, "'");
    }
    fprintf(RCMS_XML, "/>\n");
  }
  return nodeId;
}

#ifdef $XCC__h
RCMSArcId
RCMS_MakeArcStub(RCMSNodeId source_nodeId, RCMSNodeId target_nodeId, RCMSArcType arcType,
                 RCMSRegFileId regFileId, int latency, int distance, const char *temporary_name);
#endif//$XCC__h

RCMSArcId
RCMS_MakeArcStub(RCMSNodeId source_nodeId, RCMSNodeId target_nodeId, RCMSArcType arcType,
                 RCMSRegFileId regFileId, int latency, int distance, const char *temporary_name)
{
  RCMSArcId arcId = RCMS_ArcCounter++;
  SCD_TRACE && fprintf(SCD_TRACE, "RCMS_MakeArc(source_nodeId=%d, target_nodeId=%d, arcType=%s",
                                  source_nodeId, target_nodeId, RCMSArcType_Id(arcType));
  SCD_TRACE && fprintf(SCD_TRACE, ", regFileId=%d, latency=%d, distance=%d",
                                   regFileId, latency, distance);
  SCD_TRACE && temporary_name && fprintf(SCD_TRACE, ", %s", temporary_name);
  SCD_TRACE && fprintf(SCD_TRACE, ")\t=> arcId=%d\n", arcId);
  if (RCMS_XML && arcId > 0) {
    const char **_source_mnemonic = HTable_search(RCMS_Mnemonics, (HTableKey)(uintptr_t)source_nodeId);
    const char **_target_mnemonic = HTable_search(RCMS_Mnemonics, (HTableKey)(uintptr_t)target_nodeId);
    fprintf(RCMS_XML, "  <Dependence ID='D_%d' source='%s_%d' target='%s_%d'",
                       arcId, *_source_mnemonic, source_nodeId, *_target_mnemonic, target_nodeId);
    fprintf(RCMS_XML, " type='%s' latency='%d' distance='%d'",
                      RCMSArcType_Id(arcType), latency, distance);
    if (regFileId > 0) {
      fprintf(RCMS_XML, " regFile='%s'", RegFile_name(regFileId));
    }
    fprintf(RCMS_XML, "/>\n");
  }
  return arcId;
}

#ifdef $XCC__h
int
RCMS_ProcessStub(int minII, int latency, int distance, RCMSCallback callback);
#endif//$XCC__h

int
RCMS_ProcessStub(int minII, int latency, int distance, RCMSCallback callback)
{
  SCD_TRACE && fprintf(SCD_TRACE, "RCMS_Process(minII=%d, latency=%d, distance=%d",
                                  minII, latency, distance);
  SCD_TRACE && fprintf(SCD_TRACE, ")\t=> minII=%d\n", minII);
  return minII;
}

#ifdef $XCC__h
RCMSStatus
RCMS_CloseStub(void);
#endif//$XCC__h

RCMSStatus
RCMS_CloseStub(void)
{
  RCMSStatus status = RCMSStatus_Success;
  SCD_TRACE && fprintf(SCD_TRACE, "RCMS_Close()\t=> status=%d\n", status);
  if (RCMS_XML) {
    fprintf(RCMS_XML, "</Problem>\n\n");
  }
  HTable_Dtor(RCMS_Mnemonics);
  return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
typedef struct RCMSInterfaceContext_ {
  OptimizeFlags RCMSProblem;
  Processor Processor;
  const_RegisterSet Reserved;
  int32_t FirstIndex;
  int32_t LastIndex;
  Scheduler Scheduler;
  DDGraph DDGraph;
  RCMSResourceId ResourceMap[Resource__COUNT];
  RCMSReservationId ReservationMap[Reservation__COUNT];
  RCMSRegFileId RegFileMap[RegFile__COUNT];
  int32_t NodeCount;
  RCMSNodeId *NodeMap;
} RCMSInterfaceContext_, *RCMSInterfaceContext;
#endif//$XCC__h

static RCMSStatus
RCMSInterfaceContext_AddArc(void *context, RCMSNodeId source_nodeId, RCMSNodeId target_nodeId,
                            RCMSArcType arcType, RCMSRegFileId regFileId,
                            int latency, int distance)
{
  RCMSStatus status = RCMSStatus_Success;
  RCMSInterfaceContext this = context;
  Scheduler scheduler = this->Scheduler;
  DependenceKind dependenceKind = DependenceKind_Some;
  int32_t tail_index = -1, head_index = -1, index;
  for (index = 0; index < this->NodeCount; index++) {
    RCMSNodeId nodeId = this->NodeMap[index];
    if (nodeId == source_nodeId) tail_index = index;
    if (nodeId == target_nodeId) head_index = index;
  }
  if (arcType == RCMSArcType_Reuse) dependenceKind = DependenceKind_Reuse;
  SCD_TRACE && fprintf(SCD_TRACE, "RCMS_AddArc(source_nodeId=%d, target_nodeId=%d, arcType=%s",
                                  source_nodeId, target_nodeId, RCMSArcType_Id(arcType));
  SCD_TRACE && fprintf(SCD_TRACE, ", regFileId=%d, latency=%d, distance=%d",
                                  regFileId, latency, distance);
  if (tail_index >= 0 && head_index >= 0) {
    Scheduler_makeArc(scheduler, tail_index, head_index,
                      latency, distance, dependenceKind, NULL);
  } else {
    status = RCMSStatus_Failure;
  }
  SCD_TRACE && fprintf(SCD_TRACE, ")\t=> status=%d\n", status);
  return status;
}

static RCMSStatus
RCMSInterfaceContext_FreezeNode(void *context, RCMSNodeId nodeId, int tau, int phi)
{
  RCMSStatus status = RCMSStatus_Success;
  RCMSInterfaceContext this = context;
  Scheduler scheduler = this->Scheduler;
  int32_t node_index = -1, index;
  for (index = 0; index < this->NodeCount; index++) {
    if (this->NodeMap[index] == nodeId) node_index = index;
  }
  SCD_TRACE && fprintf(SCD_TRACE, "RCMS_FreezeNode(nodeId=%d, tau=%d, phi=%d", nodeId, tau, phi);
  if (node_index >= 0) {
    Scheduler_freezeNode(scheduler, node_index, tau, phi);
  } else {
    status = RCMSStatus_Failure;
  }
  SCD_TRACE && fprintf(SCD_TRACE, ")\t=> status=%d\n", status);
  return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static void
RCMSInterface_callMakeResources(RCMSInterface this, RCMSInterfaceContext context)
{
  int resource;
  Processor processor = context->Processor;
  RCMSResourceId *resourceMap = context->ResourceMap;
  for (resource = 0; resource < Resource__Count; resource++) {
#ifndef _NTRACE
    const char *name = Resource_name(resource);
#else//_NTRACE
    const char *name = NULL;
#endif//_NTRACE
    int availability = resource? Resource_availability(resource, processor): 0;
    resourceMap[resource] = (this->MakeResource)(name, availability);
  }
}

static void
RCMSInterface_callMakeReservations(RCMSInterface this, RCMSInterfaceContext context)
{
  int scheduling, resource;
  Processor processor = context->Processor;
  RCMSReservationId *reservationMap = context->ReservationMap;
  int *entries = alloca(sizeof(int)*Resource__Count*Reservation__MaxColumnsCount);
  for (scheduling = 0; scheduling < Scheduling__Count; scheduling++) {
    if (   Scheduling_processor(scheduling) == processor
        || scheduling == Scheduling__UNDEF) {
      int reservation = Scheduling_reservation(scheduling);
      if (reservationMap[reservation] < 0) {
#ifndef _NTRACE
        const char *name = Reservation_name(reservation);
#else//_NTRACE
        const char *name = NULL;
#endif//_NTRACE
        const_ReservationTable reservationTable = Reservation_table(reservation);
        const ResourceVector_ *columns = ReservationTable_columns(reservationTable);
        int columnCount = ReservationTable_columnCount(reservationTable), i;
        int *entryp = entries, num_entries = Resource__Count*columnCount;
        for (i = 0; i < columnCount; i++) {
          for (resource = 0; resource < Resource__Count; resource++) {
            *entryp++ = resource? ResourceVector_available(columns + i, resource): 0;
          }
        }
        Except_CHECK(columnCount <= Reservation__MaxColumnsCount);
        Except_CHECK(entryp - entries <= num_entries);
        reservationMap[reservation] = (this->MakeReservation)(name, num_entries, entries);
      }
    }
  }
}

static void
RCMSInterface_callMakeRegFiles(RCMSInterface this, RCMSInterfaceContext context)
{
  int regFile, registre;
  const_RegisterSet reserved = context->Reserved;
  RCMSRegFileId *regFileMap = context->RegFileMap;
  for (regFile = 0; regFile < RegFile__Count; regFile++) {
#ifndef _NTRACE
    const char *name = RegFile_name(regFile);
#else//_NTRACE
    const char *name = NULL;
#endif//_NTRACE
    int lowReg = RegFile_lowRegister(regFile);
    int highReg = RegFile_highRegister(regFile);
    int num_allocatable = 0;
    for (registre = lowReg; registre <= highReg; registre++) {
      if (registre == Register__UNDEF) continue;
      if (!RegisterSet_contains(reserved, registre)) num_allocatable++;
    }
    regFileMap[regFile] = (this->MakeRegFile)(name, num_allocatable);
  }
}

static void
RCMSInterface_callMakeNodes(RCMSInterface this, RCMSInterfaceContext context)
{
  RCMSNodeId nodeId;
  DDGraph graph = context->DDGraph;
  RCMSNodeId *nodeMap = context->NodeMap;
  const_RegisterSet reserved = context->Reserved;
  RCMSReservationId *reservationMap = context->ReservationMap;
  RCMSRegFileId *regFileMap = context->RegFileMap;
  RCMSRegFileId *regFileIds = alloca(sizeof(RCMSRegFileId)*Operator__MaxParameters);
  OptimizeFlags rcmsProblem = context->RCMSProblem;
  bool margins = (rcmsProblem & OptimizeRCMSProblem_Margins) != 0;
  bool lifetime = (rcmsProblem & OptimizeRCMSProblem_Lifetime) != 0;
  DDGraph_FOREACH_DDGraphNode(graph, node) {
    int32_t index = DDGraphNode_INDEX(node);
    Operation operation = DDGraphNode_OPERATION(node);
    if (index == 0) {
      nodeMap[index] = (this->MakeNode)(0, RCMSNodeType_Root, 0, NULL, NULL);
    } else
    if (index < context->FirstIndex) {
    } else
    if (lifetime || index <= context->LastIndex) {
      IssueItem issueItem = Operation_issueItem(operation);
      Reservation reservation = IssueItem_RESERVATION(issueItem);
      RCMSNodeType nodeType = RCMSNodeType_Actual;
      int num_write = 0;
      Operation_RESULTS_FOREACH_Temporary(operation, temporary) {
        if (   !Temporary_hasRegister(temporary)
            || !RegisterSet_contains(reserved, Temporary_REGISTER(temporary))) {
          regFileIds[num_write++] = regFileMap[Temporary_REGFILE(temporary)];
        }
      } Operation_RESULTS_ENDEACH_Temporary;
      if (reservation == Reservation__UNDEF) {
        nodeType = RCMSNodeType_Dummy;
      }
      if (index > context->LastIndex) {
        nodeType = RCMSNodeType_Killer;
        reservation = Reservation__UNDEF;
      }
      if (nodeType == RCMSNodeType_Actual) {
        Operator operator = Operation_operator(operation);
#ifndef _NTRACE
        const char *mnemonic = Operator_mnemonic(operator);
#else//_NTRACE
        const char *mnemonic = NULL;
#endif//_NTRACE
        nodeMap[index] = (this->MakeNode)(reservationMap[reservation],
                                          nodeType, num_write, regFileIds, mnemonic);
      } else {
        nodeMap[index] = (this->MakeNode)(reservation == Reservation__UNDEF?
                                          0: reservationMap[reservation],
                                          nodeType, num_write, regFileIds, NULL);
      }
    }
  } DDGraph_ENDEACH_DDGraphNode;
}

static void
RCMSInterface_callMakeArcs(RCMSInterface this, RCMSInterfaceContext context, int overlap)
{
  const_RegisterSet reserved = context->Reserved;
  RCMSRegFileId *regFileMap = context->RegFileMap;
  DDGraph graph = context->DDGraph;
  RCMSNodeId *nodeMap = context->NodeMap;
  OptimizeFlags rcmsProblem = context->RCMSProblem;
  bool margins = (rcmsProblem & OptimizeRCMSProblem_Margins) != 0;
  bool lifetime = (rcmsProblem & OptimizeRCMSProblem_Lifetime) != 0;
  if (!(rcmsProblem & OptimizeRCMSProblem_Overlap)) overlap = INT_MAX;
  (this->MakeArc)(nodeMap[0], nodeMap[context->FirstIndex], RCMSArcType_Some, 0, 0, 0, NULL);
  DDGraph_FOREACH_DDGraphNode(graph, node) {
    DDGraphNode_FORLEAVE_DDGraphArc(node, arc) {
      int32_t tail_index = DDGraphNode_INDEX(DDGraphArc_TAIL(arc));
      int32_t head_index = DDGraphNode_INDEX(DDGraphArc_HEAD(arc));
      int latency = DDGraphArc_THETA(arc);
      int distance = DDGraphArc_OMEGA(arc);
      RCMSNodeId source_nodeId = nodeMap[tail_index];
      RCMSNodeId target_nodeId = nodeMap[head_index];
      if (distance > overlap) continue;
      if (tail_index < context->FirstIndex) {
        if (margins && head_index <= context->LastIndex) {
          RCMSArcType arcType = RCMSArcType_Margin;
          (this->MakeArc)(source_nodeId, target_nodeId, arcType, 0, latency, distance, NULL);
        }
      } else
      if (head_index < context->FirstIndex) {
        if (margins && tail_index <= context->LastIndex) {
          RCMSArcType arcType = RCMSArcType_Margin;
          (this->MakeArc)(source_nodeId, target_nodeId, arcType, 0, latency, distance, NULL);
        }
      } else
      if (lifetime || (   tail_index <= context->LastIndex
                       && head_index <= context->LastIndex)) {
        DependenceKind kind = DDGraphArc_KIND(arc);
        RCMSArcType arcType = RCMSArcType_Some;
        RCMSRegFileId regFileId = 0;
        const char *temporary_name = NULL;
        if (   kind >= DependenceKind_Relax
            && kind <= DependenceKind_Link) {
          Temporary temporary = DDGraphArc_TEMPORARY(arc);
          if (   !Temporary_hasRegister(temporary)
              || !RegisterSet_contains(reserved, Temporary_REGISTER(temporary))) {
            RegFile regFile = Temporary_REGFILE(temporary);
#ifndef _NTRACE
            char temporary_name_buffer[128];
            const char *regFile_name = RegFile_name(regFile);
            sprintf(temporary_name_buffer, "%s_%u", regFile_name, Temporary_IDENTITY(temporary));
            temporary_name = temporary_name_buffer;
#endif//_NTRACE
            arcType = (RCMSArcType)(kind - DependenceKind_Relax);
            regFileId = regFileMap[regFile];
          }
        }
        (this->MakeArc)(source_nodeId, target_nodeId, arcType, regFileId, latency, distance,
                        temporary_name);
      }
    } DDGraphNode_ENDLEAVE_DDGraphArc;
  } DDGraph_ENDEACH_DDGraphNode;
}

static int
RCMSInterface_invoke(RCMSInterface this, Scheduler scheduler, int lambda_min)
{
  BasicBlock headBlock = Scheduler_HEADBLOCK(scheduler);
  Procedure procedure = BasicBlock_procedure(headBlock);
  const char *procedure_name = Procedure_name(procedure);
  const char *label_name = Scheduler_HEADNAME(scheduler);
  Except_REQUIRE(this->SizeOf == sizeof(RCMSInterface_));
  // Call to Open.
  if ((this->Open)(procedure_name, label_name, GETENV("RCMS_OPTIONS")) == RCMSStatus_Success) {
    int overlap = Scheduler_OVERLAP(scheduler);
    IssueItem firstItem = Scheduler_FIRSTITEM(scheduler);
    IssueItem lastItem = Scheduler_LASTITEM(scheduler);
    DDGraph mcrlGraph = Scheduler_MCRLGRAPH(scheduler);
    int32_t nodeCount = DDGraph_nodeCount(mcrlGraph);
    BlockTrace blockTrace = Scheduler_BLOCKTRACE(scheduler);
    Optimize optimize = Scheduler_OPTIMIZE(scheduler);
    Convention convention = BlockTrace_CONVENTION(blockTrace);
    const_RegisterSet reserved = Convention_roleRegisterSet(convention, ConventionRole_Reserved);
    RCMSCallback_ callback[1];
    RCMSInterfaceContext_ context[1];
    callback->Context = memset(context, -1, sizeof(context));
    callback->AddArc = RCMSInterfaceContext_AddArc;
    callback->FreezeNode = RCMSInterfaceContext_FreezeNode;
    context->RCMSProblem = Optimize_RCMSPROBLEM(optimize);
    context->Processor = BlockTrace_PROCESSOR(blockTrace);
    context->Reserved = reserved;
    context->FirstIndex = IssueItem_INDEX(firstItem);
    context->LastIndex = IssueItem_INDEX(lastItem);
    context->Scheduler = scheduler;
    context->DDGraph = mcrlGraph;
    context->NodeCount = nodeCount;
    context->NodeMap = alloca(nodeCount*sizeof(RCMSNodeId));
    memset(context->NodeMap, -1, nodeCount*sizeof(RCMSNodeId));
    // Calls to MakeResource.
    RCMSInterface_callMakeResources(this, context);
    // Calls to MakeReservation.
    RCMSInterface_callMakeReservations(this, context);
    // Calls to MakeRegFile.
    RCMSInterface_callMakeRegFiles(this, context);
    // Calls to MakeNode.
    RCMSInterface_callMakeNodes(this, context);
    // Calls to MakeArc.
    RCMSInterface_callMakeArcs(this, context, overlap);
    // Call to Process.
    lambda_min = (this->Process)(lambda_min, 1, overlap + 1, callback);
    // Call to Close.
    if ((this->Close)() != RCMSStatus_Success) {
      //TODO.
    }
  }
  return lambda_min;
}

#ifdef $XCC__h
/**
 * Pre-Schedule using RCMS through a RCMSInterface.
 */
int
RCMSInterface_Schedule(Scheduler scheduler, int lambda_min);
#endif//$XCC__h

#if defined (linux) || defined (__linux__) || defined (__linux)
#include <dlfcn.h>
#endif//linux

int
RCMSInterface_Schedule(Scheduler scheduler, int lambda_min)
{
  if (RCMS_LIBRARIES != NULL) {
    char *p = RCMS_LIBRARIES;
    while (isspace(*p) && *p != '\0') p++;
    while (*p != '\0') {
      char *q = p + 1, c;
      while (!isspace(*q) && *q != '\0') q++;
      c = *q; *q = '\0';
      fprintf(stderr, "*** RCMS library %s\n", p);
#if defined (linux) || defined (__linux__) || defined (__linux)
      {
        void *handle = dlopen(p, RTLD_NOW);
        if (handle != NULL) {
          RCMSInterface interface = dlsym(handle, "RCMS_Interface_");
          if (interface != NULL) {
            lambda_min = RCMSInterface_invoke(interface, scheduler, lambda_min);
          } else {
            fprintf(stderr, "*** RCMS error %s\n", dlerror());
          }
          dlclose(handle);
        } else {
          fprintf(stderr, "*** RCMS error %s\n", dlerror());
        }
      }
#endif//linux
      *q = c; p = q;
      while (isspace(*p) && *p != '\0') p++;
    }
  } else {
    lambda_min = RCMSInterface_invoke(&RCMS_InterfaceStub_, scheduler, lambda_min);
  }
  SCD_TRACE && fprintf(SCD_TRACE, "**** RCMS DDGraph:\n")
            && DDGraph_pretty(Scheduler_MCRLGRAPH(scheduler), SCD_TRACE);
  return lambda_min;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * File handle used to dump XML.
 */
extern FILE *RCMS_XML;
#ifdef _NTRACE
#define RCMS_XML NULL
#endif//_NTRACE
#endif//$XCC__h

#undef RCMS_XML 
FILE *RCMS_XML = NULL;

#ifdef $XCC__h
extern char *
RCMS_LIBRARIES;
#endif//$XCC__h

char *
RCMS_LIBRARIES;

#ifdef $XCC__h
/**
 * Initialize the RCMS module.
 */
void
RCMS_INIT(void);
#endif//$XCC__h

void
RCMS_INIT(void)
{
  if (GETENV("RCMS_XML")) {
    int file = 0;
    sscanf(GETENV("RCMS_XML"), "%d", &file);
    if (file == 1) RCMS_XML = stdout;
    else if (file == 2) RCMS_XML = stderr;
  }
  RCMS_LIBRARIES = GETENV("RCMS_LIBRARIES");
}

#ifdef $XCC__h
/**
 * Finalize the RCMS module.
 */
#define RCMS_FINI()
#endif//$XCC__h

#if XCC__C
static void
RCMS_TEST(void)
{
#include "SCD/RCMS_.c"
}

int
main(int argc, char **argv)
{
  SCD_Test(RCMS_TEST);
  return 0;
}
#endif

