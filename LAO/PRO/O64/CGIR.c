#ifdef $XCC_h
/*
 * !!!!	CGIR.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Christophe Guillon (Christophe.Guillon@st.com).
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
 * @ingroup O64
 * @brief Call-back functions for the CGIR (code generator
 * intermediate representation).
 */
#endif//$XCC_h


#include "O64/O64_.h"

#ifdef $XCC__h
#include "O64/CGIR.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Host compiler CGIR types.
 */
typedef uint32_t CGIRLAB;
typedef uint32_t CGIRSYM;
typedef struct WN *CGIRWN;
typedef struct tn *CGIRTN;
typedef struct op *CGIROP;
typedef struct bb *CGIRBB;
typedef struct LOOP_DESCR *CGIRLD;
typedef struct region_id *CGIRRID;
#endif//$XCC_h

#ifdef $XCC_h
typedef enum {
  CGIRTNType_Virtual,
  CGIRTNType_Assigned,
  CGIRTNType_Dedicated,
  CGIRTNType_Absolute,
  CGIRTNType_Symbol,
  CGIRTNType_Label,
  CGIRTNType_Modifier,
  CGIRTNType__,
} CGIRTNType;
const char *
CGIRTNType_Id(CGIRTNType type);
#endif//$XCC_h

const char *
CGIRTNType_Id(CGIRTNType type)
{
  static const char *_[] = {
    "Virtual",
    "Assigned",
    "Dedicated",
    "Absolute",
    "Symbol",
    "Label",
    "Modifier",
  };
  Except_REQUIRE((unsigned)type < CGIRTNType__);
  return _[type];
}

#ifdef $XCC_h
/**
 * Used to pass OperationFlag to CGIROP.
 */
typedef enum {
  CGIROPFlags_Hoisted = 0x1,
  CGIROPFlags_Volatile = 0x2,
  CGIROPFlags_MemorySafe = 0x4,
  CGIROPFlags_MemoryTouch = 0x8,
  CGIROPFlags_MemoryLevel2 = 0x10,
  CGIROPFlags_MemoryFence = 0x20,
  CGIROPFlags_MemorySpill = 0x40,
  CGIROPFlags_PureCopy = 0x100,
} enum_CGIROPFlags;
typedef uint16_t CGIROPFlags;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Used to pass BasicBlockFlag to CGIROP.
 */
typedef enum {
  CGIRBBFlags_Allocated = 0x1,
  CGIRBBFlags_Scheduled = 0x2,
} CGIRBBFlags;
#endif//$XCC_h

#ifdef $XCC_h
/*
 * intptr_t CGIRLAB_identity(CGIRLAB cgirlab); --	Identity of a CGIRLAB.
 */
#define CGIRLAB_identity(cgirlab) \
  (intptr_t)(cgirlab)
#endif//$XCC_h

#ifdef $XCC_h
/*
 * intptr_t CGIRSYM_identity(CGIRSYM cgirsym); --	Identity of a CGIRSYM.
 */
#define CGIRSYM_identity(cgirsym) \
  (intptr_t)(cgirsym)
#endif//$XCC_h

#ifdef $XCC_h
/*
 * intptr_t CGIRTN_identity(CGIRTN cgirtn); --	Identity of a CGIRTN.
 */
#define CGIRTN_identity(cgirtn) \
  (intptr_t)(cgirtn)
#endif//$XCC_h

#ifdef $XCC_h
/*
 * intptr_t CGIROP_identity(CGIROP cgirop); --	Identity of a CGIROP.
 */
#define CGIROP_identity(cgirop) \
      (intptr_t)(cgirop)
#endif//$XCC_h

#ifdef $XCC_h
/*
 * intptr_t CGIRBB_identity(CGIRBB cgirbb); --	Identity of a CGIRBB.
 */
#define CGIRBB_identity(cgirbb) \
  (intptr_t)(cgirbb)
#endif//$XCC_h

#ifdef $XCC_h
/*
 * intptr_t CGIRLD_identity(CGIRLD cgirld); --	Identity of a CGIRLD.
 */
#define CGIRLD_identity(cgirld) \
  (intptr_t)(cgirld)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Update a CGIRLAB.
 */
static CGIRLAB
CGIRLAB_make(CGIRLAB cgirlab, const char *name);
#endif//$XCC_h

static CGIRLAB
CGIRLAB_make(CGIRLAB cgirlab, const char *name)
{
  O64_OUTPUT && fprintf(O64_OUTPUT, "    CGIRLAB_make(CGIRLAB(%td), \"%s\");\n",
      CGIRLAB_identity(cgirlab), name);
  return cgirlab;
}

#ifdef $XCC_h
/**
 * Update a CGIRSYM.
 */
static CGIRSYM
CGIRSYM_make(CGIRSYM cgirsym, const char *name, bool isSpill, NativeType lai_nativeType);
#endif//$XCC_h

static CGIRSYM
CGIRSYM_make(CGIRSYM cgirsym, const char *name, bool isSpill, NativeType lai_nativeType)
{
  O64_OUTPUT && fprintf(O64_OUTPUT, "    CGIRSYM_make(CGIRSYM(%td), \"%s\", %s, NativeType_%s);\n",
      CGIRSYM_identity(cgirsym), name, isSpill? "true": "false", NativeType_Id(lai_nativeType));
  return cgirsym;
}

#ifdef $XCC_h
/**
 * Update a CGIRTN.
 */
static CGIRTN
CGIRTN_make(CGIRTN cgirtn, CGIRTNType cgirtype, ...);
#endif//$XCC_h

static CGIRTN
CGIRTN_make(CGIRTN cgirtn, CGIRTNType cgirtype, ...)
{
  O64_OUTPUT && fprintf(O64_OUTPUT, "    CGIRTN_make(CGIRTN(%td), CGIRTNType_%s, ...);\n",
      CGIRTN_identity(cgirtn), CGIRTNType_Id(cgirtype));
  return cgirtn;
}

#ifdef $XCC_h
/**
 * Update a CGIROP from a LIR Operation.
 */
static CGIROP
CGIROP_make(CGIROP cgirop, Operator lai_operator, CGIRTN arguments[], CGIRTN results[], CGIROP orig_op);
#endif//$XCC_h

static CGIROP
CGIROP_make(CGIROP cgirop, Operator lai_operator, CGIRTN arguments[], CGIRTN results[], CGIROP orig_op)
{
  O64_OUTPUT &&
    fprintf(O64_OUTPUT, "    CGIROP_make(CGIROP(%td),", CGIROP_identity(cgirop)) &&
    CGIRTN_printVector(arguments, O64_OUTPUT) && CGIRTN_printVector(results, O64_OUTPUT) &&
    fprintf(O64_OUTPUT, ");\n");
  return cgirop;
}

#ifdef $XCC_h
static void
CGIROP_more(CGIROP cgirop, int instance, int startDate, unsigned flags);
#endif//$XCC_h

static void
CGIROP_more(CGIROP cgirop, int instance, int startDate, unsigned flags)
{
}

#ifdef $XCC_h
/*
 * void CGIRBB_make --	Update a CGIRBB.
 */
static CGIRBB
CGIRBB_make(CGIRBB cgirbb, CGIRLAB labels[], CGIROP operations[], CGIRRID cgirrid, float frequency);
#endif//$XCC_h

static CGIRBB
CGIRBB_make(CGIRBB cgirbb, CGIRLAB labels[], CGIROP operations[], CGIRRID cgirrid, float frequency)
{
  O64_OUTPUT &&
    fprintf(O64_OUTPUT, "    CGIRBB_make(CGIRBB(%td)", CGIRBB_identity(cgirbb)) &&
    CGIRLAB_printVector(labels, O64_OUTPUT) && CGIROP_printVector(operations, O64_OUTPUT) &&
    fprintf(O64_OUTPUT, ",\n\t0x%p,%f);\n", cgirrid, frequency);
  return cgirbb;
}

#ifdef $XCC_h
static void
CGIRBB_more(CGIRBB cgirbb, CGIRBB loop_bb, intptr_t traceId, int unrolled, unsigned flags);
#endif//$XCC_h

static void
CGIRBB_more(CGIRBB cgirbb, CGIRBB loop_bb, intptr_t traceId, int unrolled, unsigned flags)
{
}

#ifdef $XCC_h
/**
 * Update a CGIRLD.
 */
static CGIRLD
CGIRLD_make(CGIRLD cgirld, CGIRBB head_bb, CGIRTN trip_count_tn, int unrolled);
#endif//$XCC_h

static CGIRLD
CGIRLD_make(CGIRLD cgirld, CGIRBB head_bb, CGIRTN trip_count_tn, int unrolled)
{
  O64_OUTPUT && fprintf(O64_OUTPUT, "    CGIRLD_make(CGIRLD(%td), CGIRBB(%td), ",
      CGIRLD_identity(cgirld), CGIRBB_identity(head_bb));
  O64_OUTPUT && trip_count_tn != NULL && fprintf(O64_OUTPUT, "CGIRTN(%td));\n", CGIRTN_identity(trip_count_tn));
  O64_OUTPUT && trip_count_tn == NULL && fprintf(O64_OUTPUT, "NULL);\n");
  return cgirld;
}

#ifdef $XCC_h
/**
 * Chain two CGIRBBs in the CGIR.
 */
static void
CGIRBB_chain(CGIRBB cgirbb, CGIRBB succ_cgirbb);
#endif//$XCC_h

static void
CGIRBB_chain(CGIRBB cgirbb, CGIRBB succ_cgirbb)
{
  O64_OUTPUT && fprintf(O64_OUTPUT, "    CGIRBB_chain(CGIRBB(%td), CGIRBB(%td));\n",
      CGIRBB_identity(cgirbb), CGIRBB_identity(succ_cgirbb));
}

#ifdef $XCC_h
/**
 * Unchain a CGIRBB in the CGIR.
 */
static void
CGIRBB_unchain(CGIRBB cgirbb);
#endif//$XCC_h

static void
CGIRBB_unchain(CGIRBB cgirbb)
{
  O64_OUTPUT && fprintf(O64_OUTPUT, "    CGIRBB_unchain(CGIRBB(%td));\n",
      CGIRBB_identity(cgirbb));
}

#ifdef $XCC_h
/**
 * Link two CGIRBBs in the CGIR with the given branch probability.
 */
static void
CGIRBB_link(CGIRBB tail_cgirbb, CGIRBB head_cgirbb, float probability);
#endif//$XCC_h

static void
CGIRBB_link(CGIRBB tail_cgirbb, CGIRBB head_cgirbb, float probability)
{
  O64_OUTPUT && fprintf(O64_OUTPUT, "    CGIRBB_link(CGIRBB(%td), CGIRBB(%td), %f);\n",
      CGIRBB_identity(tail_cgirbb), CGIRBB_identity(head_cgirbb), probability);
}

#ifdef $XCC_h
/*
 * void CGIRBB_unlink(CGIRBB cgirbb, bool preds, bool succs);
 * --	Unlink the predecessors and/or the successors of a CGIRBB in the CGIR.
 */
static void
CGIRBB_unlink(CGIRBB cgirbb, bool preds, bool succs);
#endif//$XCC_h

static void
CGIRBB_unlink(CGIRBB cgirbb, bool preds, bool succs)
{
  O64_OUTPUT && fprintf(O64_OUTPUT, "    CGIRBB_unlink(CGIRBB(%td), %d, %d);\n",
      CGIRBB_identity(cgirbb), preds, succs);
}

#ifdef $XCC_h
/*
 * void CGIRBB_discard(CGIRBB cgirbb);
 * --	Discard a CGIRBB in the CGIR.
 */
static void
CGIRBB_discard(CGIRBB cgirbb);
#endif//$XCC_h

static void
CGIRBB_discard(CGIRBB cgirbb)
{
  O64_OUTPUT && fprintf(O64_OUTPUT, "    CGIRBB_discard(CGIRBB(%td));\n",
      CGIRBB_identity(cgirbb));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Call back functions.
 */
struct CGIRCallBack_ {
  CGIRLAB (*LAB_make)(CGIRLAB cgirlab, const char *name);
  CGIRSYM (*SYM_make)(CGIRSYM cgirsym, const char *name, bool isSpill, NativeType lai_nativeType);
  CGIRTN (*TN_make)(CGIRTN cgirtn, CGIRTNType cgirtype, ...);
  CGIROP (*OP_make)(CGIROP cgirop, Operator lai_operator, CGIRTN arguments[], CGIRTN results[], CGIROP orig_op);
  void (*OP_more)(CGIROP cgirop, int instance, int startDate, unsigned flags);
  CGIRBB (*BB_make)(CGIRBB cgirbb, CGIRLAB labels[], CGIROP operations[], CGIRRID cgirrid, float frequency);
  void (*BB_more)(CGIRBB cgirbb, CGIRBB loop_bb, intptr_t traceId, int unrolled, unsigned flags);
  CGIRLD (*LD_make)(CGIRLD cgirld, CGIRBB head_bb, CGIRTN trip_count_tn, int unrolled);
  void (*BB_chain)(CGIRBB cgirbb, CGIRBB succ_cgirbb);
  void (*BB_unchain)(CGIRBB cgirbb);
  void (*BB_link)(CGIRBB tail_cgirbb, CGIRBB head_cgirbb, float probability);
  void (*BB_unlink)(CGIRBB cgirbb, bool preds, bool succs);
  void (*BB_discard)(CGIRBB cgirbb);
};
#endif//$XCC_h

#ifdef $XCC_h
int
CGIRLAB_printVector(CGIRLAB cgirlabs[], FILE *file);
#endif//$XCC_h

int
CGIRLAB_printVector(CGIRLAB cgirlabs[], FILE *file)
{
  int i;
  fprintf(file, "CGIRLAB_Vector(");
  for (i = 0; cgirlabs[i] != 0; i++) {
    fprintf(file, ", CGIRLAB(%td)", CGIRLAB_identity(cgirlabs[i]));
  }
  fprintf(file, ", 0)");
  return 1;
}

#ifdef $XCC_h
int
CGIRTN_printVector(CGIRTN cgirtns[], FILE *file);
#endif//$XCC_h

int
CGIRTN_printVector(CGIRTN cgirtns[], FILE *file)
{
  int i;
  fprintf(file, "CGIRTN_Vector(");
  for (i = 0; cgirtns[i] != 0; i++) {
    fprintf(file, ", CGIRTN(%td)", CGIRTN_identity(cgirtns[i]));
  }
  fprintf(file, ", 0)");
  return 1;
}

#ifdef $XCC_h
int
CGIROP_printVector(CGIROP cgirops[], FILE *file);
#endif//$XCC_h

int
CGIROP_printVector(CGIROP cgirops[], FILE *file)
{
  int i;
  fprintf(file, "CGIROP_Vector(");
  for (i = 0; cgirops[i] != 0; i++) {
    fprintf(file, ", CGIROP(%td)", CGIROP_identity(cgirops[i]));
  }
  fprintf(file, ", 0)");
  return 1;
}

#ifdef $XCC_h
/**
 * O64 Interface object.
 * All functions for the LAO Interface are available from this object.
 */
struct O64_Interface_ {
  int size;
  void (*Interface_Initialize)(void);
  void (*Interface_Finalize)(void);
  struct Interface_* (*Interface_get)(void);
  void (*Interface_setMaxIssue)(struct Interface_* this, Processor processor, int maxIssue);
  void (*Interface_setMinTaken)(struct Interface_* this, Processor processor, int minTaken);
  void (*Interface_setReserved)(struct Interface_* this, Convention convention, Register reserved);
  bool (*Interface_setReadStage)(struct Interface_* this, Processor processor, Operator operator, int index, int stage);
  bool (*Interface_setWriteStage)(struct Interface_* this, Processor processor, Operator operator, int index, int stage);
  Label (*Interface_makeLabel)(struct Interface_* this, CGIRLAB cgirlab, const char *name);
  Label (*Interface_findLabel)(struct Interface_* this, CGIRLAB cgirlab);
  Symbol (*Interface_makeSymbol)(struct Interface_* this, CGIRSYM cgirsym, const char *name);
  void (*Interface_Symbol_setClasses)(struct Interface_* this, Symbol symbol, SymbolClass sclass, SymbolStore sstore, SymbolExport sexport);
  Symbol (*Interface_findSymbol)(struct Interface_* this, CGIRSYM cgirsym);
  Temporary (*Interface_makeDedicatedTemporary)(struct Interface_* this, CGIRTN cgirtn, Register registre);
  Temporary (*Interface_makeAssignedTemporary)(struct Interface_* this, CGIRTN cgirtn, Register registre);
  Temporary (*Interface_makeVirtualTemporary)(struct Interface_* this, CGIRTN cgirtn, RegFile regFile);
  Temporary (*Interface_makeAbsoluteTemporary)(struct Interface_* this, CGIRTN cgirtn, Immediate immediate, int64_t value);
  Temporary (*Interface_makeSymbolTemporary)(struct Interface_* this, CGIRTN cgirtn, Immediate immediate, Symbol symbol, int64_t offset);
  Temporary (*Interface_makeLabelTemporary)(struct Interface_* this, CGIRTN cgirtn, Immediate immediate, Label label);
  Temporary (*Interface_makeModifierTemporary)(struct Interface_* this, CGIRTN cgirtn, ModifierMember member);
  void (*Interface_Temporary_setWidth)(struct Interface_* this, Temporary temporary, unsigned width);
  void (*Interface_Temporary_setRemater)(struct Interface_* this, Temporary temporary, Temporary value);
  void (*Interface_Temporary_setHomeable)(struct Interface_* this, Temporary temporary, Temporary location);
  void (*Interface_Temporary_setDedicated)(struct Interface_* this, Temporary temporary);
  Temporary (*Interface_findTemporary)(struct Interface_* this, CGIRTN cgirtn);
  uint32_t (*Interface_Temporary_identity)(Temporary temporary);
  int (*Interface_Temporary_isAbsolute)(Temporary temporary);
  int64_t (*Interface_Temporary_value)(Temporary temporary);
  int (*Interface_Temporary_isSymbol)(Temporary temporary);
  Symbol (*Interface_Temporary_symbol)(Temporary temporary);
  int64_t (*Interface_Temporary_offset)(Temporary temporary);
  int (*Interface_Temporary_isVirtual)(Temporary temporary);
  int (*Interface_Temporary_isDedicated)(Temporary temporary);
  int (*Interface_Temporary_isAssigned)(Temporary temporary);
  Register (*Interface_Temporary_register)(Temporary temporary);
  RegFile (*Interface_Temporary_regFile)(Temporary temporary);
  uint32_t (*Interface_Symbol_identity)(Symbol symbol);
  const char * (*Interface_Symbol_name)(Symbol symbol);
  int (*Interface_Symbol_isSpill)(Symbol symbol);
  NativeType (*Interface_Symbol_nativeType)(Symbol symbol);
  uint32_t (*Interface_Operation_identity)(Operation operation);
  Operator (*Interface_Operation_operator)(Operation operation);
  int (*Interface_Operation_instance)(Operation operation);
  int (*Interface_Operation_startDate)(Operation operation);
  int (*Interface_Operation_isMemorySpill)(Operation operation);
  int (*Interface_Operation_isVolatile)(Operation operation);
  int (*Interface_Operation_isHoisted)(Operation operation);
  uint32_t (*Interface_Label_identity)(Label label);
  const char * (*Interface_Label_name)(Label label);
  uint32_t (*Interface_BasicBlock_identity)(BasicBlock basicBlock);
  int (*Interface_BasicBlock_instance)(BasicBlock basicBlock);
  intptr_t (*Interface_BasicBlock_traceId)(BasicBlock basicBlock);
  uint32_t (*Interface_LoopScope_identity)(LoopScope loopScope);
  int (*Interface_LoopScope_unrolled)(LoopScope loopScope);
  Operation (*Interface_makeOperation)(struct Interface_* this, CGIROP cgirop, Operator operator, int instance, int argCount, Temporary arguments[], int resCount, Temporary results[], int regCount, int registers[]);
  Operation (*Interface_findOperation)(struct Interface_* this, CGIROP cgirop);
  void (*Interface_Operation_setMemorySafe)(struct Interface_* this, Operation operation);
  void (*Interface_Operation_setHoisted)(struct Interface_* this, Operation operation);
  void (*Interface_Operation_setVolatile)(struct Interface_* this, Operation operation);
  void (*Interface_Operation_setMemoryTouch)(struct Interface_* this, Operation operation);
  void (*Interface_Operation_setMemoryLevel2)(struct Interface_* this, Operation operation);
  void (*Interface_Operation_setMemoryFence)(struct Interface_* this, Operation operation);
  void (*Interface_Operation_setPureCopy)(struct Interface_* this, Operation operation);
  void (*Interface_Operation_setMemorySpill)(struct Interface_* this, Operation operation, Symbol symbol);
  BasicBlock (*Interface_makeBasicBlock)(struct Interface_* this, CGIRBB cgirbb, int unrolled, int labelCount, Label labels[], int operationCount, Operation operations[], intptr_t regionId, float frequency);
  BasicBlock (*Interface_findBasicBlock)(struct Interface_* this, CGIRBB cgirbb);
  void (*Interface_linkBasicBlocks)(struct Interface_* this, BasicBlock tail_block, BasicBlock head_block, float probability);
  LoopScope (*Interface_makeLoopScope)(struct Interface_* this, CGIRLD cgirld, BasicBlock basicBlock, Temporary temporary, ConfigureItem item, ...);
  LoopScope (*Interface_findLoopScope)(struct Interface_* this, CGIRLD cgirld);
  void (*Interface_LoopScope_setDependenceNode)(struct Interface_* this, LoopScope loopScope, Operation operation, DependenceNode node);
  void (*Interface_LoopScope_setDependenceArc)(struct Interface_* this, LoopScope loopScope, Operation tail_operation, Operation head_operation, int latency, int omega, DependenceKind type);
  void (*Interface_setBody)(struct Interface_* this, BasicBlock basicBlock);
  void (*Interface_setEntry)(struct Interface_* this, BasicBlock basicBlock);
  void (*Interface_setExit)(struct Interface_* this, BasicBlock basicBlock);
  void (*Interface_setStart)(struct Interface_* this, BasicBlock basicBlock);
  void (*Interface_updateCGIR)(struct Interface_* this, CGIRCallBack callback);
  void (*Interface_open)(struct Interface_* this, const char *name, Processor processor, Convention convention, ConfigureItem item, ...);
  unsigned (*Interface_optimize)(struct Interface_* this, OptimizeItem item, ...);
  void (*Interface_close)(struct Interface_* this);
};
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The singleton O64_Interface instance.
 */
O64_Interface
O64_getInterface(void);
#endif//$XCC_h

O64_Interface
O64_getInterface(void)
{
  static O64_Interface_ O64_interface_ = {
    sizeof(O64_Interface_),
    &Interface_Initialize,
    &Interface_Finalize,
    &Interface_get,
    &Interface_setMaxIssue,
    &Interface_setMinTaken,
    &Interface_setReserved,
    &Interface_setReadStage,
    &Interface_setWriteStage,
    &Interface_makeLabel,
    &Interface_findLabel,
    &Interface_makeSymbol,
    &Interface_Symbol_setClasses,
    &Interface_findSymbol,
    &Interface_makeDedicatedTemporary,
    &Interface_makeAssignedTemporary,
    &Interface_makeVirtualTemporary,
    &Interface_makeAbsoluteTemporary,
    &Interface_makeSymbolTemporary,
    &Interface_makeLabelTemporary,
    &Interface_makeModifierTemporary,
    &Interface_Temporary_setWidth,
    &Interface_Temporary_setRemater,
    &Interface_Temporary_setHomeable,
    &Interface_Temporary_setDedicated,
    &Interface_findTemporary,
    &Interface_Temporary_identity,
    &Interface_Temporary_isAbsolute,
    &Interface_Temporary_value,
    &Interface_Temporary_isSymbol,
    &Interface_Temporary_symbol,
    &Interface_Temporary_offset,
    &Interface_Temporary_isVirtual,
    &Interface_Temporary_isDedicated,
    &Interface_Temporary_isAssigned,
    &Interface_Temporary_register,
    &Interface_Temporary_regFile,
    &Interface_Symbol_identity,
    &Interface_Symbol_name,
    &Interface_Symbol_isSpill,
    &Interface_Symbol_nativeType,
    &Interface_Operation_identity,
    &Interface_Operation_operator,
    &Interface_Operation_instance,
    &Interface_Operation_startDate,
    &Interface_Operation_isMemorySpill,
    &Interface_Operation_isVolatile,
    &Interface_Operation_isHoisted,
    &Interface_Label_identity,
    &Interface_Label_name,
    &Interface_BasicBlock_identity,
    &Interface_BasicBlock_instance,
    &Interface_BasicBlock_traceId,
    &Interface_LoopScope_identity,
    &Interface_LoopScope_unrolled,
    &Interface_makeOperation,
    &Interface_findOperation,
    &Interface_Operation_setMemorySafe,
    &Interface_Operation_setHoisted,
    &Interface_Operation_setVolatile,
    &Interface_Operation_setMemoryTouch,
    &Interface_Operation_setMemoryLevel2,
    &Interface_Operation_setMemoryFence,
    &Interface_Operation_setPureCopy,
    &Interface_Operation_setMemorySpill,
    &Interface_makeBasicBlock,
    &Interface_findBasicBlock,
    &Interface_linkBasicBlocks,
    &Interface_makeLoopScope,
    &Interface_findLoopScope,
    &Interface_LoopScope_setDependenceNode,
    &Interface_LoopScope_setDependenceArc,
    &Interface_setBody,
    &Interface_setEntry,
    &Interface_setExit,
    &Interface_setStart,
    &Interface_updateCGIR,
    &Interface_open,
    &Interface_optimize,
    &Interface_close
  };
  return &O64_interface_;
}

#ifdef $XCC_h
/*
 * Accessors to the O64_Interface functions.
 * O64_interface must be an O64_Interface object.
 */
#define O64_Interface_Initialize (*O64_interface->Interface_Initialize)
#define O64_Interface_Finalize (*O64_interface->Interface_Finalize)
#define O64_Interface_get (*O64_interface->Interface_get)
#define O64_Interface_setMaxIssue (*O64_interface->Interface_setMaxIssue)
#define O64_Interface_setMinTaken (*O64_interface->Interface_setMinTaken)
#define O64_Interface_setReserved (*O64_interface->Interface_setReserved)
#define O64_Interface_setReadStage (*O64_interface->Interface_setReadStage)
#define O64_Interface_setWriteStage (*O64_interface->Interface_setWriteStage)
#define O64_Interface_makeLabel (*O64_interface->Interface_makeLabel)
#define O64_Interface_findLabel (*O64_interface->Interface_findLabel)
#define O64_Interface_makeSymbol (*O64_interface->Interface_makeSymbol)
#define O64_Interface_Symbol_setClasses (*O64_interface->Interface_Symbol_setClasses)
#define O64_Interface_findSymbol (*O64_interface->Interface_findSymbol)
#define O64_Interface_makeDedicatedTemporary (*O64_interface->Interface_makeDedicatedTemporary)
#define O64_Interface_makeAssignedTemporary (*O64_interface->Interface_makeAssignedTemporary)
#define O64_Interface_makeVirtualTemporary (*O64_interface->Interface_makeVirtualTemporary)
#define O64_Interface_makeAbsoluteTemporary (*O64_interface->Interface_makeAbsoluteTemporary)
#define O64_Interface_makeSymbolTemporary (*O64_interface->Interface_makeSymbolTemporary)
#define O64_Interface_makeLabelTemporary (*O64_interface->Interface_makeLabelTemporary)
#define O64_Interface_makeModifierTemporary (*O64_interface->Interface_makeModifierTemporary)
#define O64_Interface_Temporary_setWidth (*O64_interface->Interface_Temporary_setWidth)
#define O64_Interface_Temporary_setRemater (*O64_interface->Interface_Temporary_setRemater)
#define O64_Interface_Temporary_setHomeable (*O64_interface->Interface_Temporary_setHomeable)
#define O64_Interface_Temporary_setDedicated (*O64_interface->Interface_Temporary_setDedicated)
#define O64_Interface_findTemporary (*O64_interface->Interface_findTemporary)
#define O64_Interface_Temporary_identity (*O64_interface->Interface_Temporary_identity)
#define O64_Interface_Temporary_isAbsolute (*O64_interface->Interface_Temporary_isAbsolute)
#define O64_Interface_Temporary_value (*O64_interface->Interface_Temporary_value)
#define O64_Interface_Temporary_isSymbol (*O64_interface->Interface_Temporary_isSymbol)
#define O64_Interface_Temporary_symbol (*O64_interface->Interface_Temporary_symbol)
#define O64_Interface_Temporary_offset (*O64_interface->Interface_Temporary_offset)
#define O64_Interface_Temporary_isVirtual (*O64_interface->Interface_Temporary_isVirtual)
#define O64_Interface_Temporary_isDedicated (*O64_interface->Interface_Temporary_isDedicated)
#define O64_Interface_Temporary_isAssigned (*O64_interface->Interface_Temporary_isAssigned)
#define O64_Interface_Temporary_register (*O64_interface->Interface_Temporary_register)
#define O64_Interface_Temporary_regFile (*O64_interface->Interface_Temporary_regFile)
#define O64_Interface_Symbol_identity (*O64_interface->Interface_Symbol_identity)
#define O64_Interface_Symbol_name (*O64_interface->Interface_Symbol_name)
#define O64_Interface_Symbol_isSpill (*O64_interface->Interface_Symbol_isSpill)
#define O64_Interface_Symbol_nativeType (*O64_interface->Interface_Symbol_nativeType)
#define O64_Interface_Operation_identity (*O64_interface->Interface_Operation_identity)
#define O64_Interface_Operation_operator (*O64_interface->Interface_Operation_operator)
#define O64_Interface_Operation_instance (*O64_interface->Interface_Operation_instance)
#define O64_Interface_Operation_startDate (*O64_interface->Interface_Operation_startDate)
#define O64_Interface_Operation_isMemorySpill (*O64_interface->Interface_Operation_isMemorySpill)
#define O64_Interface_Operation_isVolatile (*O64_interface->Interface_Operation_isVolatile)
#define O64_Interface_Operation_isHoisted (*O64_interface->Interface_Operation_isHoisted)
#define O64_Interface_Label_identity (*O64_interface->Interface_Label_identity)
#define O64_Interface_Label_name (*O64_interface->Interface_Label_name)
#define O64_Interface_BasicBlock_identity (*O64_interface->Interface_BasicBlock_identity)
#define O64_Interface_BasicBlock_instance (*O64_interface->Interface_BasicBlock_instance)
#define O64_Interface_BasicBlock_traceId (*O64_interface->Interface_BasicBlock_traceId)
#define O64_Interface_LoopScope_identity (*O64_interface->Interface_LoopScope_identity)
#define O64_Interface_LoopScope_unrolled (*O64_interface->Interface_LoopScope_unrolled)
#define O64_Interface_makeOperation (*O64_interface->Interface_makeOperation)
#define O64_Interface_findOperation (*O64_interface->Interface_findOperation)
#define O64_Interface_Operation_setMemorySafe (*O64_interface->Interface_Operation_setMemorySafe)
#define O64_Interface_Operation_setHoisted (*O64_interface->Interface_Operation_setHoisted)
#define O64_Interface_Operation_setVolatile (*O64_interface->Interface_Operation_setVolatile)
#define O64_Interface_Operation_setMemoryTouch (*O64_interface->Interface_Operation_setMemoryTouch)
#define O64_Interface_Operation_setMemoryLevel2 (*O64_interface->Interface_Operation_setMemoryLevel2)
#define O64_Interface_Operation_setMemoryFence (*O64_interface->Interface_Operation_setMemoryFence)
#define O64_Interface_Operation_setPureCopy (*O64_interface->Interface_Operation_setPureCopy)
#define O64_Interface_Operation_setMemorySpill (*O64_interface->Interface_Operation_setMemorySpill)
#define O64_Interface_makeBasicBlock (*O64_interface->Interface_makeBasicBlock)
#define O64_Interface_findBasicBlock (*O64_interface->Interface_findBasicBlock)
#define O64_Interface_linkBasicBlocks (*O64_interface->Interface_linkBasicBlocks)
#define O64_Interface_makeLoopScope (*O64_interface->Interface_makeLoopScope)
#define O64_Interface_findLoopScope (*O64_interface->Interface_findLoopScope)
#define O64_Interface_LoopScope_setDependenceNode (*O64_interface->Interface_LoopScope_setDependenceNode)
#define O64_Interface_LoopScope_setDependenceArc (*O64_interface->Interface_LoopScope_setDependenceArc)
#define O64_Interface_setBody (*O64_interface->Interface_setBody)
#define O64_Interface_setEntry (*O64_interface->Interface_setEntry)
#define O64_Interface_setExit (*O64_interface->Interface_setExit)
#define O64_Interface_setStart (*O64_interface->Interface_setStart)
#define O64_Interface_updateCGIR (*O64_interface->Interface_updateCGIR)
#define O64_Interface_open (*O64_interface->Interface_open)
#define O64_Interface_optimize (*O64_interface->Interface_optimize)
#define O64_Interface_close (*O64_interface->Interface_close)
#endif//$XCC_h

#ifdef $XCC__c
{
  O64_Interface O64_interface = O64_getInterface();
  Interface interface = O64_Interface_get();
  O64_Interface_Initialize();
  fprintf(stderr, "O64_Interface_size(O64_interface) = %d\n", O64_Interface_size(O64_interface));
  O64_Interface_Finalize();
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the CGIR module.
 */
void
CGIR_INIT(void);
#endif//$XCC__h

void
CGIR_INIT(void)
{
}

#ifdef $XCC__h
/**
 * Finalize the CGIR module.
 */
void
CGIR_FINI(void);
#endif//$XCC__h

void
CGIR_FINI(void)
{
}

#if XCC__C
static void
CGIR_TEST(void)
{
#include "O64/CGIR_.c"
}

int
main(int argc, char **argv)
{
  O64_Test(CGIR_TEST);
  return 0;
}
#endif

