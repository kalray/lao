#ifdef $XCC_h
/*
 * !!!!	ValueNumber.xcc
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
 * @ingroup PFA
 * @brief Value Number services.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/ValueNumber.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

static int32_t operation_count;

#ifdef $XCC_h
/*
 *
 * This module performs Value Numbering as it is described in the paper
 * P. Briggs, K. Cooper and L. Simpson,  "Value Numbering"
 *
 * We are using dominator based value numberig here.
 */
#endif//$XCC_h

static inline uint32_t
ValueNumber_Hash(uint32_t number)
{
  uint64_t result = number*(uint64_t)2654435761U;
  uint32_t hash = result + (result >> 31);
  return hash + (hash >> 31);
}

#ifdef $XCC__h
typedef uint32_t VNTableKey;
#endif//$XCC__h

/* ValueNumber_hashString() - makes a key from arbitrary string of bytes.
 *
 * This function uses FNV (Fowler, Noll, Vo) hashing method for finding a key. It performs
 * best if hash is initialized with offset like described by the authors. Once
 * initialized, it is called repeatedly to add other parts of the string.
 */
static inline void
ValueNumber_hashString(uint32_t *hash, uint8_t *string, uint32_t len)
{
  uint32_t  index;
  static uint32_t FNV_prime= 16777619;
  for (index = 0; index < len; index++) {
    uint32_t temp;
    *hash = *hash * FNV_prime;
    temp = *hash ^ string[index];
    *hash = (*hash & 0xFFFFFF00) | (temp & 0x000000FF);
  }
}

static void
ValueNumber_hashTemporary(uint32_t *hash, Temporary temporary) {
  if (Temporary_hasRegFile(temporary)) {
    // Dedicated, Virtual and Assigned register temporaries are skipped!!!
    // TODO! Maybe raise an exception??
    return;
  } else if (Temporary_isVariable(temporary)) {
    // Here we use IDENTITY
    uint32_t identity = Temporary_IDENTITY(temporary);
    ValueNumber_hashString(hash, (uint8_t*) &identity, 4);
  } else if (Temporary_hasLiteral(temporary)) {
    Literal literal = Temporary_LITERAL(temporary);
    if (Temporary_isAbsolute(temporary)) {
      // Here we hash the VALUE
      // HACK! uint64_t is assumed for VALUE!!!
      uint64_t value = Literal_VALUE(literal);
      ValueNumber_hashString(hash, (uint8_t*) &value, 8);
    } else if (Temporary_isSymbol(temporary)) {
      // Hash the pointer for now!!!
      uint64_t offset = Literal_OFFSET(literal);
      uint32_t identity = Symbol_IDENTITY(Literal_SYMBOL(literal));
      ValueNumber_hashString(hash, (uint8_t*) &offset, sizeof(offset));
      ValueNumber_hashString(hash, (uint8_t*) &identity, sizeof(identity));
/*      ImmediateValue offset = Literal_OFFSET(literal);
      if (offset == 0)
      else */
    } else if (Temporary_isLabel(temporary)) {
      uint32_t identity = Label_IDENTITY(Literal_LABEL(literal));
      ValueNumber_hashString(hash, (uint8_t*) &identity, 4);
/*      ImmediateValue offset = Literal_OFFSET(literal);
      if (offset == 0)
      else */
    } else Except_NEVER(true);
  } /*else if (Temporary_isModifier(temporary)) {
  } else if (Temporary_isRegMask(temporary)) {
  } else if (Temporary_isVolatile(temporary)) {
  } else if (Temporary_isControl(temporary)) {
  } else if (Temporary_isMemory(temporary)) {
  } else Except_NEVER(true); */
}

// Calculates FNV hash
static inline VNTableKey
ValueNumber_calculateKey(Operation operation)
{
  // Make canonical form, if possible...
  uint32_t hash = 0x811c9dc5;
  uint32_t operator = Operation_operator(operation);
  ValueNumber_hashString(&hash, (uint8_t*) &operator, 4);
  if (Operation_argCount(operation) == 1) {
    ValueNumber_hashTemporary(&hash, Operation_getArgument(operation, 0));
  } else if (Operation_argCount(operation) == 2) {
    Temporary arg0 = Operation_getArgument(operation, 0);
    Temporary arg1 = Operation_getArgument(operation, 1);
    if (Operation_isCommutative(operation, 0, 1)) {
      if (Temporary_IDENTITY(arg1) > Temporary_IDENTITY(arg0)) {
        Temporary temp = arg0;
        arg0 = arg1;
        arg1 = temp;
      }
    }
    ValueNumber_hashTemporary(&hash, arg0);
    ValueNumber_hashTemporary(&hash, arg1);
  }
#if 0
  PFA_TRACE && printf(" == KEY == ") && Temporary_pretty(arg0, stdout)
            && printf("  %s", Operator_mnemonic(Operation_operator(operation)))
            && printf("  ") && Temporary_pretty(arg1, stdout) && printf("  ==  %d\n", hash);
#endif
  return hash;
}

#ifdef $XCC__h
/*
 * The VNTableEntry_ structure implements entries to Value Number table.
 */
struct VNTableEntry_ {
  //@args	VNTableKey key, Operation operation, struct VNTableEntry_ *link
  VNTableKey KEY;		// The key, which holds a pointer.
  Operation OPERATION;		// Value
  struct VNTableEntry_ *LINK;	// Link to the next VNTableEntry in bucket.
};
#endif//$XCC__h

VNTableEntry
VNTableEntry_Ctor(VNTableEntry this,
                  VNTableKey key, Operation operation, struct VNTableEntry_ *link)
{
  *VNTableEntry__KEY(this) = key;
  *VNTableEntry__OPERATION(this) = operation;
  *VNTableEntry__LINK(this) = link;
  return this;
}

size_t
VNTableEntry_Size(VNTableKey key, Operation operation, struct VNTableEntry_ *link)
{
  return (sizeof(VNTableEntry_));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * The VNTableScope_ structure implements entries to Value Number table.
 *
 * This implementation is borowed from HTable and modified to fit the needs of Value
 * Numbering.
 *
 * FIXME! There is a problem with RootBlock which has no operations, but scope is created
 * for it. Currently, if estimate is 0, table of size 1 is created to avoid crashes.
 */
struct VNTableScope_ {
  //@args	Memory memory, VNTableScope next_scope, uint32_t estimate
  Memory MEMORY;
  uint32_t MASK;		// Mask for modulo clp2.
  int32_t COUNT;		// Count this VNTableScope ENTRIES.
  struct VNTableScope_ *NEXT;		// Pointer to next scope.
  VNTableEntry *BUCKETS;		// This VNTableScope buckets.
};
#endif//$XCC__h

VNTableScope
VNTableScope_Ctor(VNTableScope this,
                  Memory memory, VNTableScope next_scope, uint32_t estimate)
{
  uint32_t clp2 = HackerU32_clp2(estimate), mask = clp2 == 0 ? 0 : clp2 - 1;
  size_t size = (clp2 == 0 ? 2 : clp2)*sizeof(VNTableEntry);
  *VNTableScope__MEMORY(this) = memory;
  *VNTableScope__MASK(this) = mask;
  *VNTableScope__COUNT(this) = 0;
  *VNTableScope__NEXT(this) = next_scope;
  *VNTableScope__BUCKETS(this) = Memory_alloc(memory, size);
  memset(VNTableScope_BUCKETS(this), 0, size);
  return this;
}

void
VNTableScope_Dtor(VNTableScope this)
{
  Memory memory = VNTableScope_MEMORY(this);
  VNTableEntry *buckets = VNTableScope_BUCKETS(this);
  uint32_t mask = VNTableScope_MASK(this);
  if (buckets != NULL) {
    int32_t i;
    for (i = 0; i <= mask; i++) {
      VNTableEntry entry = buckets[i];
      while (entry != NULL) {
        VNTableEntry link = VNTableEntry_LINK(entry);
        Memory_free_(memory, entry);
        entry = link;
      }
      buckets[i] = NULL;
    }
    Memory_free_(memory, buckets);
  }
  *VNTableScope__COUNT(this) = 0;
  *VNTableScope__BUCKETS(this) = NULL;
}

size_t
VNTableScope_Size(Memory memory, VNTableScope next_scope, uint32_t estimate)
{
  return (sizeof(VNTableScope_));
}

#ifdef $XCC__h
VNTableScope
VNTableScope_new(Memory memory, VNTableScope next_scope, int32_t estimate);
#endif//$XCC__h

VNTableScope
VNTableScope_new(Memory memory, VNTableScope next_scope, int32_t estimate)
{
  size_t size = VNTableScope_Size(memory, next_scope, estimate);
  VNTableScope this = Memory_alloc(memory, size);
  return VNTableScope_Ctor(this, memory, next_scope, estimate);
}

#ifdef $XCC__h
void
VNTableScope_delete(VNTableScope this);
#endif//$XCC__h

void
VNTableScope_delete(VNTableScope this)
{
  if (this != NULL) {
    Memory memory = VNTableScope_MEMORY(this);
    VNTableScope_Dtor(this);
    Memory_free_(memory, this);
  }
}

static inline bool
ValueNumber_areTemporariesEquivalent(Temporary t1, Temporary t2)
{
  if (Temporary_TYPE(t1) != Temporary_TYPE(t2))
    return false;
  if (Temporary_isVariable(t1)) {
    return Temporary_IDENTITY(t1) == Temporary_IDENTITY(t2);
  } else if (Temporary_hasLiteral(t1)) {
    Literal t1_literal = Temporary_LITERAL(t1);
    Literal t2_literal = Temporary_LITERAL(t2);
    if (Temporary_isAbsolute(t1)) {
      return    Literal_VALUE(t1_literal)
             == Literal_VALUE(t2_literal);
    } else if (Temporary_isSymbol(t1)) {
      return (      Literal_OFFSET(t1_literal)
                 == Literal_OFFSET(t2_literal)
              &&    Symbol_IDENTITY(Literal_SYMBOL(t1_literal))
                 == Symbol_IDENTITY(Literal_SYMBOL(t2_literal)));
    } else if (Temporary_isLabel(t1)) {
      return    Label_IDENTITY(Literal_LABEL(t1_literal))
             == Label_IDENTITY(Literal_LABEL(t2_literal));
    } else Except_NEVER(true);
  }
  return false;
}

static inline bool
ValueNumber_areOperationsEquivalent(Operation o1, Operation o2)
{
  // FIXME! Need to check for equivalence between the two operands. For example, if two
  // constants are the same but with different names, this will return false...
  if (Operation_operator(o1) != Operation_operator(o2))
    return false;
  if (Operation_argCount(o1) != Operation_argCount(o2))
    return false; // TODO! Can we have two operators with different number of arguments?
  // FIXME! Need to check for commutative operators...
  if (!Operation_isCommutative(o1, 0, 1)) {
    Operation_ARGUMENTS_FOREACH_Temporary(o1, argument) {
      if (!ValueNumber_areTemporariesEquivalent(argument, 
                                                Operation_getArgument(o2, Operation_ARGUMENTS_INDEX)))
        return false;
    } Operation_ARGUMENTS_ENDEACH_Temporary;
  } else {
    Temporary x1, x2, y1, y2;
    // Only two-operand stuff...
    if (Operation_argCount(o1) != 2)
      return false;
    x1 = Operation_getArgument(o1, 0);
    x2 = Operation_getArgument(o1, 1);
    y1 = Operation_getArgument(o2, 0);
    y2 = Operation_getArgument(o2, 1);
    if (   (ValueNumber_areTemporariesEquivalent(x1, y1) && ValueNumber_areTemporariesEquivalent(x2, y2))
        || (ValueNumber_areTemporariesEquivalent(x1, y2) && ValueNumber_areTemporariesEquivalent(x2, y1)))
      return true;
    else
      return false;
  }
  return true;
}

//XCC_.h
Operation
VNTableScope_search(const_VNTableScope this, VNTableKey key, Operation operation);

Operation
VNTableScope_search(const_VNTableScope this, VNTableKey key, Operation operation)
{
  VNTableEntry *buckets = VNTableScope_BUCKETS(this);
  uint32_t hash = ValueNumber_Hash((uint32_t)key);
  uint32_t mask = VNTableScope_MASK(this);
  uint32_t index = hash & mask;
  VNTableEntry entry = buckets[index];
  while (entry != NULL) {
    VNTableEntry link = VNTableEntry_LINK(entry);
    Operation found_operation = VNTableEntry_OPERATION(entry);
    if (   VNTableEntry_KEY(entry) == key 
        && ValueNumber_areOperationsEquivalent(operation, found_operation)) {
      return VNTableEntry_OPERATION(entry);
    }
    entry = link;
  }
  return NULL;
}

#ifdef $XCC__h
bool
VNTableScope_insert(VNTableScope this, VNTableKey key, Operation operation);
#endif//$XCC__h

bool
VNTableScope_insert(VNTableScope this, VNTableKey key, Operation operation)
{
  Memory memory = VNTableScope_MEMORY(this);
  VNTableEntry *buckets = VNTableScope_BUCKETS(this);
  uint32_t hash = ValueNumber_Hash((uint32_t)key);
  uint32_t mask = VNTableScope_MASK(this);
  int32_t count = VNTableScope_COUNT(this);
  uint32_t index = hash & mask;
  VNTableEntry entry = buckets[index];
  while (entry != NULL) {
    VNTableEntry link = VNTableEntry_LINK(entry);
//    TODO! Decide if you need key at all!
//    if (VNTableEntry_KEY(entry) == key) {
//      return false;
//    }
    entry = link;
  }
  entry = Memory_alloc(memory, VNTableEntry_Size(key, operation, buckets[index]));
  VNTableEntry_Ctor(entry, key, operation, buckets[index]);
  buckets[index] = entry;
  *VNTableScope__COUNT(this) = count + 1;
  return true;
}

//XCC_.h
Operation
VNTableScope_remove(const_VNTableScope this, VNTableKey key, Operation operation);

#if 0
// This was used only for statistical purposes to get the density of the hash table. Now
// it's usless, but code is here in case that this information will be needed in the
// future.
static void
VNTableScope_printDensity(VNTableScope this, FILE *file)
{
  VNTableEntry *buckets = VNTableScope_BUCKETS(this);
  int32_t mask = VNTableScope_MASK(this);
  int32_t index = 0;
  int32_t empty_buckets = 0;
  char sep = ' ';
  if (file == NULL)
    file = stderr;
  fprintf(file, "*** VNTableScope : Number of buckets %d : Operation count %d ***\n  ", 
                mask, VNTableScope_COUNT(this));
  for (index = 0; index < mask; index++) {
    int32_t local_count = 0;
    VNTableEntry entry = buckets[index];
    while (entry != NULL) {
      local_count++;
      entry = VNTableEntry_LINK(entry);
    }
    if (local_count == 0)
      empty_buckets++;
    if (local_count != 0 ) {
      fprintf (file, "%c %d", sep, local_count);
      sep = ',';
    }
  }
  fprintf(file, "\n  Empty buckets %d\n", empty_buckets);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Value numbering support.
 *
 * TODO! This strcuture contains all necessary data for both dominator-based and
 * scope-based Value Numbering. Once one of these is selected, the other one will be
 * removed in other to save space.
 *
 * Dominance based Value Numbering approach using unified hash table and dominators 
 * as suggested by Mon-Ping.
 *
 * This approach uses unified hash table, but checks for dominance before deciding if one
 * operation can replace another.
 */
struct ValueNumber_;
#endif//$XCC_h

#ifdef $XCC__h
struct ValueNumber_ {
  //@args	Memory memory, SSAForm ssaForm
  Memory MEMORY;
  SSAForm SSAFORM;			// The SSAForm this ValueNumber applies to.
  CodeRegion CODEREGION;
  struct VNTableScope_ *CURRENT_SCOPE;
  Dominance DOMINANCE;
  bool DOMINANCE_BASED;
};
#endif//$XCC__h

ValueNumber
ValueNumber_Ctor(ValueNumber this,
                 Memory memory, SSAForm ssaForm)
{
  CodeRegion codeRegion = SSAForm_codeRegion(ssaForm);
  *ValueNumber__MEMORY(this) = memory;
  *ValueNumber__SSAFORM(this) = ssaForm;
  *ValueNumber__CODEREGION(this) = codeRegion;
  *ValueNumber__CURRENT_SCOPE(this) = NULL;
  *ValueNumber__DOMINANCE(this) = SSAForm_makeDominance(ssaForm, 0);
  *ValueNumber__DOMINANCE_BASED(this) = getenv("PFA_VN_DOMINANCE") != NULL;
  return this;
}

void
ValueNumber_Dtor(ValueNumber this)
{
}

size_t
ValueNumber_Size(Memory memory, SSAForm ssaForm)
{
  return sizeof(ValueNumber_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(ValueNumber_)\t%zu\n", sizeof(ValueNumber_));
}
#endif//$XCC__c

#ifdef $XCC_h
ValueNumber
ValueNumber_new(Memory parent, struct SSAForm_ *ssaForm);
#endif//$XCC_h

ValueNumber
ValueNumber_new(Memory parent, struct SSAForm_ *ssaForm)
{
  Memory memory = Memory_new(parent, true);
  size_t size = ValueNumber_Size(memory, ssaForm);
  ValueNumber this = Memory_alloc(memory, size);
  ValueNumber_Ctor(this, memory, ssaForm);
  if (ValueNumber_DOMINANCE_BASED(this)) {
    int32_t size = 0;
    CodeRegion_FOREACH_BasicBlock(SSAForm_codeRegion(ssaForm), block) {
      size += BasicBlock_countOperations(block);
    } CodeRegion_ENDEACH_BasicBlock;
    *ValueNumber__CURRENT_SCOPE(this) = VNTableScope_new(memory, NULL, size + size/2);
  }
  return this;
}

#ifdef $XCC_h
ValueNumber
ValueNumber_delete(ValueNumber this);
#endif//$XCC_h

ValueNumber
ValueNumber_delete(ValueNumber this)
{
  if (this != NULL) {
    Memory memory = ValueNumber_MEMORY(this);
    while (ValueNumber_CURRENT_SCOPE(this) != NULL) {
      ValueNumber_removeScope(this, NULL);
    }
#ifndef _NDTOR
    ValueNumber_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * ValueNumber_createNewScope() - creates new scope and puts it on top.
 */
void
ValueNumber_createNewScope(ValueNumber this, BasicBlock block);
#endif//$XCC_h

void
ValueNumber_createNewScope(ValueNumber this, BasicBlock block)
{
  if (!ValueNumber_DOMINANCE_BASED(this)) {
    Memory memory = ValueNumber_MEMORY(this);
    int32_t estimate = BasicBlock_countOperations(block) + BasicBlock_countOperations(block)/2;
    VNTableScope top_scope = ValueNumber_CURRENT_SCOPE(this);
    VNTableScope new_scope;
    if (estimate < 32)
      estimate = 32;
    new_scope = VNTableScope_new(memory, top_scope, estimate);
    *ValueNumber__CURRENT_SCOPE(this) = new_scope;
  }
}

#ifdef $XCC_h
/*
 * ValueNumber_removeScope() - removes current scope from the top.
 */
void
ValueNumber_removeScope(ValueNumber this, BasicBlock block);
#endif//$XCC_h

void
ValueNumber_removeScope(ValueNumber this, BasicBlock block)
{
  if (!ValueNumber_DOMINANCE_BASED(this)) {
    VNTableScope top_scope = ValueNumber_CURRENT_SCOPE(this);
    VNTableScope next_scope;
    // TODO! - Raise an exception if top_scope is not present?!
    if (top_scope == NULL)
      return;
    next_scope = VNTableScope_NEXT(top_scope);
    *ValueNumber__CURRENT_SCOPE(this) = next_scope;
    //VNTableScope_printDensity(top_scope, 0);
    VNTableScope_delete(top_scope);
  }
}

/*
 * ValueNumber_findOperation() - searches hash tables for this operation.
 */
static Operation
ValueNumber_findOperation(ValueNumber this, VNTableKey key, Operation operation)
{
  VNTableScope current_scope = ValueNumber_CURRENT_SCOPE(this);
  while (current_scope != NULL) {
    Operation found = VNTableScope_search(current_scope, key, operation);
    if (found != NULL) {
      return found;
    }
    current_scope = VNTableScope_NEXT(current_scope);
  }
  return NULL;
}

#ifdef $XCC_h
bool
ValueNumber_canValueNumberOperation(ValueNumber this, Operation operation);
#endif//$XCC_h

bool
ValueNumber_canValueNumberOperation(ValueNumber this, Operation operation)
{
  // FIXME! This uses Operation_isCritical (copy/paste - modified)
  // TODO - Replace with call to Operation_isCritical
  // Left very conservative for purpose for initial testing... only operations whose all
  // arguments are variable temporaries are considered for numbering...
  Operator operator = Operation_operator(operation);
  unsigned flags = Operation_FLAGS(operation);
  if (Operation_resCount(operation) != 1)
    return false;
  if (  flags
      & (  OperationFlag_Volatile
         | OperationFlag_MemoryTouch
         | OperationFlag_MemoryFence
         | OperationFlag_EntryCode
         | OperationFlag_ExitCode)) return false;
  if (Operation_resCount(operation) == 0) return false;
  if (Operator_isMemoryEffect(operator)) return false;
  if (Operator_isMemoryRead(operator)) return false;
  if (Operator_isMemoryWrite(operator)) return false;
  if (Operator_isControl(operator)) return false;
  Operation_RESULTS_FOREACH_Temporary(operation, result) {
    if (!Temporary_isRenameOK(result)) return false;
  } Operation_RESULTS_ENDEACH_Temporary;
  Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
    if (!Temporary_isVariable(argument) && !Temporary_hasLiteral(argument) && !Temporary_isSymbol(argument)) 
      return false;
  } Operation_ARGUMENTS_ENDEACH_Temporary;
  return true;
}

static inline Temporary
DominanceValueNumber_getValueNumber(ValueNumber this, Operation operation)
{
  VNTableKey key = ValueNumber_calculateKey(operation);
  Operation existing_operation = NULL;
  VNTableScope scope =  ValueNumber_CURRENT_SCOPE(this);
  uint32_t hash = ValueNumber_Hash((uint32_t)key);
  uint32_t index = hash & VNTableScope_MASK(scope);
  VNTableEntry entry = VNTableScope_BUCKETS(scope)[index];
  while (entry != NULL) {
    VNTableEntry link = VNTableEntry_LINK(entry);
    Operation found_operation = VNTableEntry_OPERATION(entry);
    if (   VNTableEntry_KEY(entry) == key 
        && ValueNumber_areOperationsEquivalent(operation, found_operation)
        && Dominance_dominates(ValueNumber_DOMINANCE(this),
                               Operation_basicBlock(found_operation),
                               Operation_basicBlock(operation))) {
      existing_operation = found_operation;
      break;
    }
    entry = link;
  }
  if (existing_operation != NULL) { 
    LIR_TRACE && printf ("=== Value Numbering ===\n")
              && printf("\t") && Operation_pretty(operation, stdout)
              && printf("\n\t") && Operation_pretty(existing_operation, stdout)
              && printf("\n");
    operation_count++;
    return Operation_getResult(existing_operation, 0);
  }
  VNTableScope_insert(ValueNumber_CURRENT_SCOPE(this), key, operation);
  return NULL;
}

static inline Temporary
ScopedValueNumber_getValueNumber(ValueNumber this, Operation operation)
{
  VNTableKey key = ValueNumber_calculateKey(operation);
  Operation existing_operation = ValueNumber_findOperation(this, key, operation);
  if (existing_operation != NULL) {
    LIR_TRACE && printf ("=== Value Numbering ===\n")
              && printf("\t") && Operation_pretty(operation, stdout)
              && printf("\n\t") && Operation_pretty(existing_operation, stdout)
              && printf("\n");
    operation_count++;
    return Operation_getResult(existing_operation, 0);
  }
  VNTableScope_insert(ValueNumber_CURRENT_SCOPE(this), key, operation);
  return NULL;
}

#ifdef $XCC_h
/*
 *
 * ValueNumber_getValueNumber() - Returns Value Number for this expression. Value Number
 * is the name of the SSA variable, as described in Briggs et. al. "Value Numbering".
 *
 * Returns:
 *   Value number  - if equivalent operation exists in the hash table.
 *   NULL          - if no equivalent operation was found. In this case expression is
 *                   entered into the hash table.
 */
Temporary
ValueNumber_getValueNumber(ValueNumber this, Operation operation);
#endif//$XCC_h

Temporary
ValueNumber_getValueNumber(ValueNumber this, Operation operation)
{
  if (ValueNumber_DOMINANCE_BASED(this)) 
    return DominanceValueNumber_getValueNumber(this, operation);
  else
    return ScopedValueNumber_getValueNumber(this, operation);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This ValueNumber Memory.
 */
Memory
ValueNumber_memory(ValueNumber this);
#endif//$XCC_h

Memory
ValueNumber_memory(ValueNumber this)
{
  return ValueNumber_MEMORY(this);
}

#ifdef $XCC_h
/**
 * This ValueNumber CodeRegion.
 */
CodeRegion
ValueNumber_codeRegion(ValueNumber this);
#endif//$XCC_h

CodeRegion
ValueNumber_codeRegion(ValueNumber this)
{
  return ValueNumber_CODEREGION(this);
}

#ifdef $XCC_h
/**
 * Pretty-print this ValueNumber.
 */
bool
ValueNumber_pretty(ValueNumber this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
ValueNumber_pretty(ValueNumber this, FILE *file)
{
  if (file == NULL) file = stderr;
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  Minir minir = Minir_new(Memory_Root, stdin);
  Program program;
  Minir_KEEP_INDEX(minir) = false;
  program = Minir_nextProgram(minir);
  {
    Procedure procedure = Program_findProcedure(program, "value-numbering");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    /*    CodeRegion_pretty(codeRegion, stdout); // just to check*/
    SSAForm ssaForm = SSAForm_construct(Memory_Root, codeRegion, true);
    printf("\nCode after performing Value Numbering:\n\n");
    CodeRegion_pretty(codeRegion, stdout);
    SSAForm_delete(ssaForm);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0
  // Initialize ValueNumber if needed.
  if (conversion & OptimizeConversion_Numbering) {
    SSAForm ssaForm = SSAConstruct_SSAFORM(this);
    PFA_TRACE && fprintf(PFA_TRACE, "*** Value Numbering:\n");
    *SSAConstruct__VALUENUMBER(this) = ValueNumber_new(memory, ssaForm);
  }
#endif

#if 0
  ValueNumber valueNumber = SSAConstruct_VALUENUMBER(this);
  // Initialize the new scope for Value Numbering.
  if (conversion & OptimizeConversion_Numbering) {
    ValueNumber_createNewScope(valueNumber, basicBlock);
  }
#endif

#if 0
    if (conversion & OptimizeConversion_Numbering) {
      if (Operator_isPhi(operator)) {
        bool is_redundant = true;
        Temporary replacement = Operation_getArgument(operation, 0);
        Operation_ARGUMENTS_FOREACH_Temporary(operation, argument) {
          if (replacement != argument || !Temporary_isVariable(argument)) {
            is_redundant = false;
            break;
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary;
        if (is_redundant) {
          Temporary result = Operation_getResult(operation, 0);
          PFA_TRACE && fprintf(PFA_TRACE, "  Value Numbering : Redundant PHI : ")
                    && Operation_pretty(operation, PFA_TRACE);
          if (replacement != result)
            SSAConstruct_renameVariable(this, result, replacement, &globalPushCount);
          BasicBlock_detachOperation(basicBlock, operation);
          continue;
        }
        replacement = ValueNumber_getValueNumber(valueNumber, operation);
        if (   replacement != NULL 
               && basicBlock == Operation_basicBlock(DefUses_getProducer(Temporary_DEFUSES(replacement)))) {
          // Do the actual replacement...
          Temporary result = Operation_getResult(operation, 0);
          PFA_TRACE && fprintf(PFA_TRACE, "  Value Numbering : Removing PHI : replacing with :")
                    && Temporary_pretty(replacement, PFA_TRACE)
                    && fprintf(PFA_TRACE, ", operation : ")
                    && Operation_pretty(operation, PFA_TRACE);
          SSAConstruct_renameVariable(this, result, replacement, &globalPushCount);
          BasicBlock_detachOperation(basicBlock, operation); 
          continue;
        } 
      } else if (   !Operator_isSeqCopy(operator) 
                 &&  ValueNumber_canValueNumberOperation(valueNumber, operation)) {
        // Search if operation exists in the table, and enter it to the table if not.
        Temporary replacement = ValueNumber_getValueNumber(valueNumber, operation);
        if (replacement != NULL) {
          // Do the actual replacement...
          Temporary result = Operation_getResult(operation, 0);
          PFA_TRACE && fprintf(PFA_TRACE, "  Value Numbering : replacement temporary : ")
                    && Temporary_pretty(replacement, PFA_TRACE)
                    && fprintf(PFA_TRACE, ", operation : ")
                    && Operation_pretty(operation, PFA_TRACE);
          SSAConstruct_renameVariable(this, result, replacement, &globalPushCount);
          BasicBlock_detachOperation(basicBlock, operation);
          continue;
        }
      }
    }
#endif

#if 0
  // Remove the current scope for Value Numbering.
  if (conversion & OptimizeConversion_Numbering) {
    ValueNumber_removeScope(valueNumber, basicBlock);
  }
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the ValueNumber module.
 */
#define ValueNumber_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the ValueNumber module.
 */
#define ValueNumber_FINI()
#endif//$XCC__h

#if XCC__C
static void
ValueNumber_TEST(void)
{
#include "PFA/ValueNumber_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(ValueNumber_TEST);
  return 0;
}
#endif

