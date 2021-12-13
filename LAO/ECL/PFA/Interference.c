#ifdef $XCC_h
/*
 * !!!!	Interference.xcc
 *
 * Nikola Puzovic (nikola.puzovic@st.com).
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 * Fabrice Rastello (Fabrice.Rastello@ens-lyon.fr).
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
 * @ingroup PFA
 * @brief Interference analysis.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/Interference.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Interference graph over a CodeRegion given a TemporaryTable.
 */
struct Interference_;
#endif//$XCC_h

#ifdef $XCC__h
struct Interference_ {
  //@args	Memory memory, CodeRegion codeRegion, TemporaryTable temporaryTable, bool chaintin
  CodeRegion CODEREGION;		// The CodeRegion for this Interference graph.
  TemporaryTable TEMPORARYTABLE;	// The TemporaryTable for this Interference.
  TemporarySparse_ SPARSESET[1];	// Sparse set for computing Live in BasicBlock.
  IStack_ ISTACK[1];			// IStack of BitSet(s) for the bit half-matrix.
  //@access MEMORY	IStack_memory(Interference_ISTACK(this))
  //@access COUNT	(IStack_usedSize(Interference_ISTACK(this))/sizeof(BitSet_))
  //@access BASE	IStack_base(Interference_ISTACK(this))
};
#endif//$XCC__h

Interference
Interference_Ctor(Interference this,
                  Memory memory, CodeRegion codeRegion, TemporaryTable temporaryTable, bool chaintin)
{
  IStack istack = Interference_ISTACK(this);
  TemporarySparse liveSet = Interference_SPARSESET(this);
  TemporarySet_ *phiUseSets = CodeRegion_phiUseSets(codeRegion);
  TemporarySet_ *liveOutSets = CodeRegion_liveOutSets(codeRegion);
  int32_t temporaryCount = TemporaryTable_count(temporaryTable), count = 0;
  *Interference__CODEREGION(this) = codeRegion;
  *Interference__TEMPORARYTABLE(this) = temporaryTable;
  TemporarySparse_Ctor(liveSet, memory, temporaryTable);
  IStack_Ctor(istack, memory, sizeof(BitSet_), temporaryCount);
  // Initialize the ISTACK.
  while (count < temporaryCount) {
    BitSet bitSet = IStack_push(istack);
    BitSet_Ctor(bitSet, memory, ++count);
    Except_CHECK(count == IStack_count(istack));
  }
  /*
   * Adapted from L. George, A. Appel "Iterated Register Coalescing".
   *
   * procedure Build()
   *   forall b in 'blocks in program'
   *     let liveSet = liveOutSet(b)
   *     forall I in instructions(b) in reverse order
   *       if isMove(I) then
   *         liveSet <- liveSet\use(I)
   *       liveSet <- liveSet union def(I)
   *       forall d in def(I)
   *         forall l in liveSet
   *           AddEdge(l, d)
   *       liveSet <- Use(I) union (liveSet\def(I))
   */
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    int32_t index = BasicBlock_index(basicBlock);
    // let liveSet = liveOutSet(b)
    TemporarySparse_empty(liveSet);
    TemporarySet_FOREACH_Temporary(liveOutSets + index, temporary) {
      if (TemporaryTable_contains(temporaryTable, temporary)) {
        TemporarySparse_insert(liveSet, temporary);
      }
    } TemporarySet_ENDEACH_Temporary;
    /* /!\ This corresponds to the mplex model!
     * For the copy model, the phiUseSet should be unioned only
     * after having traversed the jump. Indeed, the parallel copy can be
     * placed only before the jump.
     */
    if (phiUseSets != NULL) {
      TemporarySet_FOREACH_Temporary(phiUseSets + index, temporary) {
        if (TemporaryTable_contains(temporaryTable, temporary)) {
          TemporarySparse_insert(liveSet, temporary);
        }
      } TemporarySet_ENDEACH_Temporary;
    }
    // forall I in instructions(b) in reverse order
    BasicBlock_FORBACK_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      // if isMove(I) then (Chaitin's trick)
      if (chaintin && Operator_isSeqCopy(operator)) {
        // liveSet <- liveSet\use(I)
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (TemporaryTable_contains(temporaryTable, argument)) {
            TemporarySparse_remove(liveSet, argument);
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
      }
      // liveSet <- liveSet union def(I)
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        if (TemporaryTable_contains(temporaryTable, result)) {
          TemporarySparse_insert(liveSet, result);
        }
      } Operation_RESULTS_ENDEACH_Temporary;
      // forall d in def(I)
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        if (TemporaryTable_contains(temporaryTable, result)) {
          // forall l in liveSet
          TemporarySparse_FOREACH_Temporary(liveSet, temporary) {
            if (temporary != result) {
              Interference_add(this, result, temporary);
            }
          } TemporarySparse_ENDEACH_Temporary;
        }
      } Operation_RESULTS_ENDEACH_Temporary;
      // liveSet <- Use(I) union (liveSet\def(I))
      if (!Operator_isPhi(Operation_operator(operation))) {
        // Rastello: do not remove PHI targets from liveSet, as PHI are parallel
        // and their results should interfere.
        Operation_RESULTS_FOREACH_Temporary(operation, result) {
          if (TemporaryTable_contains(temporaryTable, result)) {
            TemporarySparse_remove(liveSet, result);
          }
        } Operation_RESULTS_ENDEACH_Temporary;
        // From: cliffc <cliff.click@Eng.Sun.COM> 6 Jan 1998 22:42:51 -0500
        // (2) Build the InterFerence Graph (IFG) based on this SSA-LIVE info.
        // Same basic technique as with normal LIVE: roll backwards through
        // the block DEFd things interference with what is currently LIVE.
        // Normal USEs are added to the current LIVE set; Phi uses are not.
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (TemporaryTable_contains(temporaryTable, argument)) {
            TemporarySparse_insert(liveSet, argument);
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
      }
    } BasicBlock_ENDBACK_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  return this;
}

void
Interference_Dtor(Interference this)
{
  IStack_empty(Interference_ISTACK(this), (IStackItemRelease)BitSet_Dtor);
  IStack_Dtor(Interference_ISTACK(this));
}

size_t
Interference_Size(Memory memory, CodeRegion codeRegion, TemporaryTable temporaryTable, bool chaintin)
{
  return sizeof(Interference_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Interference_)\t%zu\n", sizeof(Interference_));
}
#endif//$XCC__c

#ifdef $XCC_h
Interference
Interference_make(Memory parent, CodeRegion codeRegion, TemporaryTable temporaryTable, bool chaintin);
#endif//$XCC_h

Interference
Interference_make(Memory parent, CodeRegion codeRegion, TemporaryTable temporaryTable, bool chaintin)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Interference_Size(memory, codeRegion, temporaryTable, chaintin);
  Interference this = Memory_alloc(memory, size);
  return Interference_Ctor(this, memory, codeRegion, temporaryTable, chaintin);
}

#ifdef $XCC_h
Interference
Interference_kill(Interference this);
#endif//$XCC_h

Interference
Interference_kill(Interference this)
{
  if (this != NULL) {
    Memory memory = Interference_MEMORY(this);
#ifndef _NDTOR
    Interference_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Interference Memory.
 */
Memory
Interference_memory(const_Interference this);
#endif//$XCC_h

Memory
Interference_memory(const_Interference this)
{
  return Interference_MEMORY(this);
}

#ifdef $XCC_h
/**
 * Pretty-print this Interference.
 */
bool
Interference_pretty(const_Interference this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Interference_pretty(const_Interference this, FILE *file)
{
  const_TemporaryTable temporaryTable = Interference_TEMPORARYTABLE(this);
  if (file == NULL) file = stderr;
  TemporaryTable_FOREACH_Temporary(temporaryTable, t_u) {
    fprintf(file, "\t");
    Temporary_pretty(t_u, file);
  } TemporaryTable_ENDEACH_Temporary;
  fprintf(file, "\n");
  TemporaryTable_FOREACH_Temporary(temporaryTable, t_u) {
    Temporary_pretty(t_u, file);
    TemporaryTable_FOREACH_Temporary(temporaryTable, t_v) {
      fprintf(file, "\t");
      if (Interference_exists(this, t_u, t_v)) {
        fprintf(file, "1");
      }
    } TemporaryTable_ENDEACH_Temporary;
    fprintf(file, "\n");
  } TemporaryTable_ENDEACH_Temporary;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Add interference between two variables.
 */
void
Interference_add(Interference this, const_Temporary u, const_Temporary v);
#endif//$XCC_h

void
Interference_add(Interference this, const_Temporary u, const_Temporary v)
{
  int32_t count = Interference_COUNT(this);
  int32_t index_u = Temporary_INDEX(u);
  int32_t index_v = Temporary_INDEX(v);
  int32_t row = _MAX(index_u, index_v);
  int32_t col = _MIN(index_u, index_v);
  int32_t increase = row - count + 1;
  struct BitSet_ *baseBitSet_ = Interference_BASE(this);
  Except_CHECK(TemporaryTable_contains(Interference_TEMPORARYTABLE(this), u));
  Except_CHECK(TemporaryTable_contains(Interference_TEMPORARYTABLE(this), v));
  Except_CHECK(index_u != index_v);
  if (increase > 0) {
    Memory memory = Interference_MEMORY(this);
    IStack istack = Interference_ISTACK(this);
    while (increase--) {
      // WARNING! the BitSet_(s) may be moved in memory by IStack_push2.
      BitSet bitSet = IStack_push2(istack);
      BitSet_Ctor(bitSet, memory, ++count);
      Except_CHECK(count == IStack_count(istack));
    }
    baseBitSet_ = IStack_base(istack);
  }
  BitSet_insert(baseBitSet_ + row, col);
0&&
  PFA_TRACE && fprintf(PFA_TRACE, "\tInterference_add([%d]", index_u)
            && Temporary_pretty(u, PFA_TRACE) && fprintf(PFA_TRACE, ", [%d]", index_v)
            && Temporary_pretty(v, PFA_TRACE) && fprintf(PFA_TRACE, ")\n");
}

#ifdef $XCC_h
/**
 * Check interference between two variables.
 */
bool
Interference_exists(const_Interference this, const_Temporary u, const_Temporary v);
#endif//$XCC_h

bool
Interference_exists(const_Interference this, const_Temporary u, const_Temporary v)
{
  int32_t count = Interference_COUNT(this);
  int32_t index_u = Temporary_INDEX(u);
  int32_t index_v = Temporary_INDEX(v);
  int32_t row = _MAX(index_u, index_v);
  int32_t col = _MIN(index_u, index_v);
  struct BitSet_ *baseBitSet_ = Interference_BASE(this);
  Except_CHECK(TemporaryTable_contains(Interference_TEMPORARYTABLE(this), u));
  Except_CHECK(TemporaryTable_contains(Interference_TEMPORARYTABLE(this), v));
  if (row < count) {
    return BitSet_contains(baseBitSet_ + row, col);
  }
  return false;
}

#ifdef $XCC__c
{
  Symbol_ symbol[1];
  Program program = Program_new(Memory_Root, (Processor)1, (Convention)1);
  Procedure procedure = Program_makeProcedure(program, symbol);
  Memory memory = Procedure_memory(procedure);
  BasicBlock block_1 = Procedure_makeBasicBlock(procedure, 0, 0.0);
  Procedure_buildCodeRegions(procedure);
  {
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    TemporaryTable globalTable = CodeRegion_globalTable(codeRegion);
    // For testing purposes 8 variables will be created and few interferences will be
    // added among them. Output should give symetrical matrix that will contain these
    // interferencies.
    Temporary temporary_1 = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_2 = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_3 = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_4 = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_5 = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_6 = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_7 = Procedure_makeVirtualTemporary(procedure, 0);
    Temporary temporary_8 = Procedure_makeVirtualTemporary(procedure, 0);
    TemporaryTable_empty(globalTable);
    TemporaryTable_insert(globalTable, temporary_1);
    TemporaryTable_insert(globalTable, temporary_2);
    TemporaryTable_insert(globalTable, temporary_3);
    TemporaryTable_insert(globalTable, temporary_4);
    TemporaryTable_insert(globalTable, temporary_5);
    TemporaryTable_insert(globalTable, temporary_6);
    TemporaryTable_insert(globalTable, temporary_7);
    TemporaryTable_insert(globalTable, temporary_8);
    {
      Interference_ interference[1];
      Liveness liveness = Liveness_make(memory, codeRegion, globalTable, true);
      Interference_Ctor(interference, memory, codeRegion, globalTable, true);
      Interference_add(interference, temporary_1, temporary_4);
      Interference_add(interference, temporary_2, temporary_8);
      Interference_add(interference, temporary_3, temporary_6);
      Interference_add(interference, temporary_4, temporary_7);
      Interference_add(interference, temporary_5, temporary_2);
      Interference_add(interference, temporary_6, temporary_5);
      printf("Interference test:\n");
      Interference_pretty(interference, stdout);
      Interference_Dtor(interference);
      Liveness_kill(liveness);
    }
  }
  Program_delete(program);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Interference module.
 */
#define Interference_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Interference module.
 */
#define Interference_FINI()
#endif//$XCC__h

#if XCC__C
static void
Interference_TEST(void)
{
#include "PFA/Interference_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(Interference_TEST);
  return 0;
}
#endif

