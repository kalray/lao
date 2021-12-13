#ifdef $XCC_h
/*
 * !!!!	Spill.xcc
 *
 * Christophe Guillon (Christophe.Guillon@st.com).
 *
 * Copyright 2002 - 2007 STMicroelectronics.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

/**
 * @file
 * @ingroup CGO
 * @brief Spill generation support.
 *
 * This module provide support function for the generation
 * of spill symbol and the generation of spill code.
 */

#include "CGO/CGO_.h"

#ifdef $XCC__h
#include "CGO/Spill.h"
#endif//$XCC__h


/*
 * Helper macros
 */
/* Defined here, but should be available in Operator.xcc. */
#define Operator_loadedIndex(operator) 0      // Assume loaded value is at result index 0.

/**
 * Code region currently processed.
 */
static CodeRegion spill_coderegion;

/**
 * spill symbol (Symbol type) mapping
 * And access functions
 */
static HTable_ spill_table_;
static HTable spill_table = (HTable)&spill_table_;

static Symbol 
Spill_getSymbol(Temporary temporary)
{
  void *pointer = HTable_search(spill_table, temporary);
  if (pointer != NULL) return *(Symbol *)pointer;
  return NULL;
}

static void
Spill_setSymbol(Temporary temporary, Symbol symbol)
{
  Symbol *_symbol = HTable_lookup(spill_table, temporary, sizeof(Symbol));
  *_symbol = symbol;
}

/**
 * Find first "actual" operation of a BasicBlock.
 *
 * Phi operation and function entry code are dismissed since spill code should 
 * be added after them.
 * @return NULL if BasicBlock has no "actual" operation
 */
static Operation
Spill_startOperation(BasicBlock bb)
{
  Operation last_phi = NULL, tmp;
  Operation last_entry;
  Operation spill_start;
  bool phi_check = false, start_check = false;

  // Find last phi operation
  tmp = BasicBlock_firstOperation(bb);
  while (tmp != NULL && Operation_operator(tmp) == Operator_PHI) {
    last_phi = tmp;
    tmp = Operation_next(tmp);
  }

  last_entry = NULL;
  if (BasicBlock_isEntry(bb)) {
    BasicBlock_FOREACH_Operation(bb, operation) {
      if (Operation_isEntryCode(operation)) {
        last_entry = operation;
      }
      else break;
    } BasicBlock_ENDEACH_Operation;
  }

  spill_start = BasicBlock_firstOperation(bb);
  if (last_phi == NULL) phi_check = true;
  if (last_entry == NULL) start_check = true;
  if (phi_check && start_check) return spill_start;
  
  while (spill_start != NULL) {
    if (last_phi == spill_start) phi_check = true;
    if (last_entry == spill_start) start_check = true;
    spill_start = Operation_next(spill_start);
    if (phi_check && start_check) break;
  }
  return spill_start;
}

/**
 * Find last "actual" operation of a BasicBlock.
 *
 * Control flow operation and function exit code are dismissed since spill code should 
 * be added before them.
 * @return NULL if there is no "actual" operation.
 */
static Operation
Spill_stopOperation(BasicBlock bb)
{
  Operation control, stop;
  bool control_check = false, stop_check = false;
  Operation spill_stop;
  
  control = BasicBlock_getControlOperation(bb);
  stop = NULL;

  if (BasicBlock_isExit(bb)) {
    BasicBlock_FORBACK_Operation(bb, operation) {
      if (Operation_isExitCode(operation)) {
        stop = operation;
      }
      else break;
    } BasicBlock_ENDBACK_Operation;
  }

  spill_stop = BasicBlock_lastOperation(bb);
  if (control == NULL) control_check = true;
  if (stop == NULL) stop_check = true;
  if (control_check && stop_check) return spill_stop;

  while (spill_stop != NULL) {
    if (control == spill_stop) control_check = true;
    if (stop == spill_stop) stop_check = true;
    spill_stop = Operation_prev(spill_stop);
    if (control_check && stop_check) break;
  }
  return spill_stop;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Perform init for a coderegion.
 */
void Spill_initCodeRegion(CodeRegion this);
#endif//$XCC_h

void
Spill_initCodeRegion(CodeRegion codeRegion)
{
  spill_coderegion = codeRegion;
  HTable_Ctor(spill_table, Procedure_memory(CodeRegion_procedure(codeRegion)), 1024);
}

#ifdef $XCC_h
/**
 * Perform fini for a coderegion.
 */
void Spill_finiCodeRegion(CodeRegion this);
#endif//$XCC_h

void
Spill_finiCodeRegion(CodeRegion codeRegion)
{
}

#ifdef $XCC_h
/**
 * Check if a temporary can be rematerialized.
 */
bool
Spill_isRematerialisable(Temporary temporary);
#endif//$XCC_h

bool
Spill_isRematerialisable(Temporary temporary) {
  bool activateRemat = getenv("ACTIVATE_REMAT") != NULL?
    atoi(getenv("ACTIVATE_REMAT")) != 0: 0;
  return Temporary_isRemater(temporary)
    || (activateRemat && Temporary_isVariable(temporary)
        && Temporary_isRemater(Variable_ORIGIN(Temporary_variable(temporary))));
}



#ifdef $XCC_h
/**
 * Generate store code for a spilled temporary.
 *
 * A store operation is not necessarily added. Cases are when:
 * - the value can be rematerialized
 * - a load of the same temporary is found at the insertion point
 * - a store of the same temporary is found at the insertion point
 *
 * @param bb the BasicBlock in which to add the store.
 * @param where the operation after which the store must be added. If @c NULL, 
 * the store is inserted as first operation of the BasicBlock.
 *
 * @param src the temporary containing the value that needs to be stored.
 * @param spilled_temporary the spilled temporary that causes generation of the 
 * spill code. A unique stacked location is generated for this temporary.
 * @param force add a store even if rematerialisation is possible.
 *
 * @note The store operation is marked as OperationFlag_MemorySpill.
 */
void
Spill_appendStore(BasicBlock bb, Operation where, Temporary src,
                  Temporary spilled_temporary, bool force);
#endif//$XCC_h

void
Spill_appendStore(BasicBlock bb, Operation where, Temporary src,
                  Temporary spilled_temporary, bool force)
{
  Selector_ selector_;
  Selector selector = &selector_;
  Temporary locate = NULL;
  Literal locate_literal = NULL;
  Operation store_op, point, next;
  
  if (Spill_isRematerialisable(spilled_temporary) && !force)
    /* No need to store a rematerializable value. */
    return;

  point =  Spill_findFirstAppendPoint(bb, where);
  
  if (Temporary_isHomeable(spilled_temporary)) {
    locate = Spill_getHomeLocation(spilled_temporary);
  } else {
    locate = Spill_genStackedLocation(spilled_temporary);
  }
  Except_ENSURE(Temporary_isSymbol(locate));
  locate_literal = Temporary_LITERAL(locate);

  // If the insertion point is a load at the same location,
  // we remove the sequence: a = load @L, store @L = a.
  if (point != NULL &&
      Operator_isMemoryRead(Operation_operator(point)) &&
      (Operation_flags(point) & OperationFlag_MemorySpill)) {
    Operator point_operator = Operation_operator(point);
    int offsetIndex = Operator_offsetIndex(point_operator);
    int loadedIndex = Operator_loadedIndex(point_operator);
    Temporary offset = Operation_getArgument(point, offsetIndex);
    Temporary loaded = Operation_getResult(point, loadedIndex);
    if (loaded == src && Temporary_isSymbol(offset)) {
      Literal offset_literal = Temporary_LITERAL(offset);
      if (      Literal_SYMBOL(offset_literal)
             == Literal_SYMBOL(locate_literal)
          &&    Literal_OFFSET(offset_literal)
             == Literal_OFFSET(locate_literal)) {
        CGO_TRACE && fprintf(CGO_TRACE, "Remove load/spill sequence: ")
                  && Operation_pretty(point, CGO_TRACE);
        BasicBlock_detachOperation(Operation_basicBlock(point), point);
        return;
      }
    }
  } 
  // If the operation following the insertion point is already a spill
  // of the same variable, we skip the spill
  if (point != NULL &&
      (next = Operation_next(point)) != NULL &&
      Operator_isMemoryWrite(Operation_operator(next)) &&
      (Operation_flags(next) & OperationFlag_MemorySpill)) {
    Operator next_operator = Operation_operator(next);
    int offsetIndex = Operator_offsetIndex(next_operator);
    int storedIndex = Operator_storedIndex(next_operator);
    Temporary offset = Operation_getArgument(next, offsetIndex);
    Temporary stored = Operation_getArgument(next, storedIndex);
    if (stored == src && Temporary_isSymbol(offset)) {
      Literal offset_literal = Temporary_LITERAL(offset);
      if (      Literal_SYMBOL(offset_literal)
             == Literal_SYMBOL(locate_literal)
          &&    Literal_OFFSET(offset_literal)
             == Literal_OFFSET(locate_literal)) {
        CGO_TRACE && fprintf(CGO_TRACE, "Skip spill redundant with store: ")
                  && Operation_pretty(next, CGO_TRACE);
        return;
      }
    }
  } 

  Selector_Ctor(selector, Selector_memory(BasicBlock_selector(bb)),
                Selector_procedure(BasicBlock_selector(bb)), false);
  
  Selector_selectDirectSTORE(selector,
                             RegFile_getNativeType(Temporary_regFile(src)),
                             src,
                             Literal_SYMBOL(locate_literal),
                             Literal_OFFSET(locate_literal));
  // Mark store operation as spill operation.
  store_op = Selector_firstOperation(selector);

  while(store_op != NULL) {
    if (Operator_isMemoryWrite(Operation_operator(store_op))) {
      Operation_setFlags(store_op, OperationFlag_MemorySpill);
      break;
    }
    store_op = Operation_next(store_op);
  }

  // Add store operation to the list of consumers if src is an SSA variable
  if(Temporary_isVariable(src)) {
    store_op = Selector_firstOperation(selector);
    while(store_op != NULL) {
      uint8_t index = 0;
      Operation_ARGUMENTS_FOREACH_Temporary(store_op, temp) {
        if(temp == src) {
          Variable_addConsumer(Temporary_variable(src), store_op, index);
        }
        index++;
      } Operation_ARGUMENTS_ENDEACH_Temporary;
      store_op = Operation_next(store_op);
    }
  }
  
  if (CGO_TRACE) {
    fprintf(CGO_TRACE, "Insert spill: ");
    Selector_FOREACH_Operation(selector, operation) {
      Operation_pretty(operation, CGO_TRACE);
    } Selector_ENDEACH_Operation;
  }

  if (point == NULL) BasicBlock_putSelector(bb, selector);
  else BasicBlock_appendSelector(bb, point, selector);
  
  Selector_Dtor(selector);
}

#ifdef $XCC_h
/**
 * Generate load code for a spilled temporary.
 *
 * A load operation is not necessarily added. Cases are when:
 * - the value can be rematerialized
 * - a store of the same temporary is found at the insertion point
 *
 * @param bb the BasicBlock in which to add the load.
 * @param where the operation before which the store must be added. If @c NULL, 
 * the load is appended as last operation of the BasicBlock (but before control 
 * operations).
 * @param dst the temporary where to put the value loaded from memory.
 * @param spilled_temporary the spilled temporary that causes generation of the 
 * spill code. A unique stacked location is generated for this temporary.
 * @note The load operation is marked as OperationFlag_MemorySpill.
 *
 */
void
Spill_insertLoad(BasicBlock bb, Operation where, Temporary dst,
                   Temporary spilled_temporary);
#endif//$XCC_h

void
Spill_insertLoad(BasicBlock bb, Operation where, Temporary dst,
                   Temporary spilled_temporary)
{
  Operation point;
  Selector_ selector_;
  Selector selector = &selector_;
  
  Selector_Ctor(selector, Selector_memory(BasicBlock_selector(bb)),
                Selector_procedure(BasicBlock_selector(bb)), false);

  point = Spill_findLastInsertPoint(bb, where);
  
  if (Spill_isRematerialisable(spilled_temporary)) {
    Temporary value;

    value = Spill_getRematerializeValue(spilled_temporary);

    if (Temporary_isAbsolute(value)) {
      Literal literal = Temporary_LITERAL(value);
      Selector_selectMAKEAbsolute(selector,
                                  RegFile_getNativeType(Temporary_regFile(dst)), dst,
                                  Literal_VALUE(literal));
    } else if (Temporary_isSymbol(value)) {
      Literal literal = Temporary_LITERAL(value);
      Selector_selectMAKEAddress(selector,
                                 RegFile_getNativeType(Temporary_regFile(dst)), dst,
                                 Literal_SYMBOL(literal),
                                 Literal_OFFSET(literal));
    } else if(Temporary_isConstant(value)) {
      Selector_selectCOPY(selector, dst, value);
    } else Except_NEVER(true);
    
    if (CGO_TRACE) {
      fprintf(CGO_TRACE, "Insert rematerialization: ");
      Selector_FOREACH_Operation(selector, operation) {
        Operation_pretty(operation, CGO_TRACE);
      } Selector_ENDEACH_Operation;
    }
    
    if (point == NULL) BasicBlock_pushSelector(bb, selector);
    else BasicBlock_insertSelector(bb, point, selector);

  } else {
    Temporary locate = NULL;
    Literal locate_literal = NULL;
    Operation load_op = NULL;
    if (Temporary_isHomeable(spilled_temporary)) {
      locate = Spill_getHomeLocation(spilled_temporary);
    } else {
      locate = Spill_genStackedLocation(spilled_temporary);
    }
    Except_ENSURE(Temporary_isSymbol(locate));
    locate_literal = Temporary_LITERAL(locate);

    // If the insertion point is a store at the same location,
    // we remove the sequence: a = load @L, store @L = a.
    if (point != NULL &&
        Operator_isMemoryWrite(Operation_operator(point)) &&
        (Operation_flags(point) & OperationFlag_MemorySpill)) {
      Operator point_operator = Operation_operator(point);
      int offsetIndex = Operator_offsetIndex(point_operator);
      int storedIndex = Operator_storedIndex(point_operator);
      Temporary offset = Operation_getArgument(point, offsetIndex);
      Temporary stored = Operation_getArgument(point, storedIndex);
      if (stored == dst && Temporary_isSymbol(offset)) {
        Literal offset_literal = Temporary_LITERAL(offset);
        if (      Literal_SYMBOL(offset_literal)
               == Literal_SYMBOL(locate_literal)
            &&    Literal_OFFSET(offset_literal)
               == Literal_OFFSET(locate_literal)) {
          CGO_TRACE && fprintf(CGO_TRACE, "Remove reload/store sequence: ")
                    && Operation_pretty(point, CGO_TRACE);
          BasicBlock_detachOperation(Operation_basicBlock(point), point);
          return;
        }
      }
    }
    
    Selector_selectDirectLOAD(selector,
                              RegFile_getNativeType(Temporary_regFile(dst)), dst,
                              Literal_SYMBOL(locate_literal),
                              Literal_OFFSET(locate_literal));
    /* Mark load operation as spill operation. */
    load_op = Selector_firstOperation(selector);

#ifdef FLO_DEBUG
    fprintf(stderr, "%s:%d load operation:\n", __FILE__, __LINE__);
    Operation_pretty(load_op, stderr);
#endif

    while(load_op != NULL) {
      if (Operator_isMemoryRead(Operation_operator(load_op))) {
        Operation_setFlags(load_op, OperationFlag_MemorySpill);
        break;
      }
      load_op = Operation_next(load_op);
    }
    
    // Add store operation to the list of consumers if src is an SSA variable
    if(Temporary_isVariable(dst)) {
      load_op = Selector_firstOperation(selector);
      while(load_op != NULL) {
        uint8_t index = 0;
        Operation_RESULTS_FOREACH_Temporary(load_op, temp) {
          if(temp == dst) {
            Variable_setOperationResult(Temporary_variable(dst), load_op, index);
          }
          index++;
        } Operation_RESULTS_ENDEACH_Temporary;
        load_op = Operation_next(load_op);
      }
    }

    if (CGO_TRACE) {
      fprintf(CGO_TRACE, "Insert reload: ");
      Selector_FOREACH_Operation(selector, operation) {
        Operation_pretty(operation, CGO_TRACE);
      } Selector_ENDEACH_Operation;
    }
    
    if (point == NULL) BasicBlock_pushSelector(bb, selector);
    else BasicBlock_insertSelector(bb, point, selector);
    
  }

  Selector_Dtor(selector);
}


#ifdef $XCC_h
/**
 * Generate copy from variable in memory for a spilled temporary.
 * Insert the copy just after operation @c where.
 *
 * @note The store operation is NOT marked as OperationFlag_MemorySpill.
 */
void
Spill_appendRegToMemCopy(BasicBlock bb, Operation where, Temporary src,
                        Temporary spilled_temporary);
#endif//$XCC_h

void
Spill_appendRegToMemCopy(BasicBlock bb, Operation where, Temporary src,
                        Temporary spilled_temporary)
{
  Temporary locate = NULL;
  Temporary result = spilled_temporary;
  Literal locate_literal = NULL;
  Operation pcopy, point, next;
  point =  Spill_findFirstAppendPoint(bb, where);
  
  if (Temporary_isHomeable(spilled_temporary)) {
    locate = Spill_getHomeLocation(spilled_temporary);
  } else {
    locate = Spill_genStackedLocation(spilled_temporary);
  }
  Except_ENSURE(Temporary_isSymbol(locate));
  locate_literal = Temporary_LITERAL(locate);

  pcopy = Operation_next(point);
  if(!pcopy || !Operator_isParCopy(Operation_operator(pcopy))) {
    pcopy = BasicBlock_makeOperationPCOPY(bb,
        point,
        false,// Make the copy after the point
        4,    // By default, 2 copies allowed
        0);   // No increment (no BB ordering)
  } else {
    uint8_t index = 0;
    // Check that existing operation is not using our spilled_temporary
    Operation_ARGUMENTS_FOREACH_Temporary(pcopy, arg) {
      if(arg == spilled_temporary) {
        // replace spilled_temporary by src
        Operation_setArgument(pcopy, index, src);
        break;
      }
      index++;
    } Operation_ARGUMENTS_ENDEACH_Temporary;
  }

  Operation_addtoParCopy(pcopy,
      spilled_temporary,
      src);
}



#ifdef $XCC_h
/**
 * Generate copy from variable in memory to a temporary for a spilled temporary.
 * Insert the copy just before operation @c where.
 *
 * @note The "load" operation is NOT marked as OperationFlag_MemorySpill.
 */
void
Spill_insertMemToRegCopy(BasicBlock bb, Operation where, Temporary dst,
                          Temporary spilled_temporary);
#endif//$XCC_h

void
Spill_insertMemToRegCopy(BasicBlock bb, Operation where, Temporary dst,
                          Temporary spilled_temporary)
{
  Temporary locate = NULL;
  Temporary origin = spilled_temporary;
  Literal locate_literal = NULL;
  Operation pcopy, point, next;
  int32_t index;
  
  point = Spill_findLastInsertPoint(bb, where);
  
  if (Temporary_isHomeable(spilled_temporary)) {
    locate = Spill_getHomeLocation(spilled_temporary);
  } else {
    locate = Spill_genStackedLocation(spilled_temporary);
  }
  Except_ENSURE(Temporary_isSymbol(locate));
  locate_literal = Temporary_LITERAL(locate);

  pcopy = Operation_prev(point);
  if(!pcopy || !Operator_isParCopy(Operation_operator(pcopy))) {
    pcopy = BasicBlock_makeOperationPCOPY(bb, point, true, 2, 0); // By default, 2 copies allowed
  } else {
    // Check that existing operation is not defining spilled_temporary
    Operation_RESULTS_FOREACH_Temporary(pcopy, res) {
      if(res == spilled_temporary) {
        origin = Operation_getArgument(pcopy, Operation_RESULTS_INDEX);
        break;
      }
    } Operation_RESULTS_ENDEACH_Temporary;
  }
  index = Operation_addtoParCopy(pcopy, dst, origin);

  if(Temporary_isVariable(dst)) {
    Variable_setOperationResult(Temporary_variable(dst), pcopy, index);
  }
}

#ifdef $XCC_h
/**
 *  Generates an unique stacked location for the given temporary.
 *
 * @return a temporary symbol representing the memory address of the stacked 
 * location.
 *
 * Returns the same temporary symbol for all invocations with the same temporary.
 */
Temporary
Spill_genStackedLocation(Temporary spilled_temporary);
#endif//$XCC_h

Temporary
Spill_genStackedLocation(Temporary spilled_temporary)
{
  static uint32_t spill_temp;
  static char buffer[16+10+1]; // prefix_len + maxuint len + NULL
  Procedure procedure = CodeRegion_procedure(spill_coderegion);
  Symbol symbol = Spill_getSymbol(spilled_temporary);
  if (symbol == NULL) {
    Program program = Procedure_program(procedure);
    int lenght = sprintf(buffer, ".lao_spill_temp_%u", spill_temp++);
    Except_ENSURE(lenght < sizeof(buffer));
    symbol = SymbolTable_makeSymbol(Program_symbolTable(program), buffer);  
    Symbol_setClasses(symbol, SymbolClass_VAR, SymbolStore_AUTO, SymbolExport_LOCAL);
    if(Procedure_hasFixedFrame(procedure)) {
      Symbol_setBaseIndex(symbol, Symbol_index(Procedure_symbolSP(procedure)));
    } else {
      Symbol_setBaseIndex(symbol, Symbol_index(Procedure_symbolFP(procedure)));
    }
    Symbol_setNativeType(symbol, RegFile_getNativeType(Temporary_regFile(spilled_temporary)));
    Spill_setSymbol(spilled_temporary, symbol);
    *Symbol__PROCEDURE(symbol) = procedure;
    {
      Frame frame = Procedure_frame(procedure);
      Frame_addSymbol(frame, symbol);
      if(Temporary_isVariable(spilled_temporary)) {
        Variable variable = Temporary_variable(spilled_temporary);
        Variable_setMemory(variable, symbol);
      }
    }
  }
  return Procedure_makeSymbolTemporary(procedure, symbol, 0, Immediate__UNDEF);
}

#ifdef $XCC_h
/**
 * Returns the absolute or symbol.
 *
 * Temporary for the rematerialize value of the Temporary (spilled_temporary).
 * Only valid if the Temporary has the isRemater property.
 */
Temporary
Spill_getRematerializeValue(Temporary spilled_temporary);
#endif//$XCC_h

Temporary
Spill_getRematerializeValue(Temporary spilled_temporary)
{
  Procedure procedure = CodeRegion_procedure(spill_coderegion);
  Temporary value;
  if(!Temporary_isRemater(spilled_temporary)) {
    if(Temporary_isVariable(spilled_temporary)) {
      spilled_temporary = Variable_ORIGIN(Temporary_variable(spilled_temporary));
    } else {
      Except_CHECK(0);
    }
  }
  value = Procedure_getRemater(procedure, spilled_temporary);
  Except_REQUIRE(Temporary_isRemater(spilled_temporary));
  Except_ENSURE(Temporary_hasLiteral(value) || Temporary_isConstant(value));
  return value;
}

#ifdef $XCC_h
/**
 *  Returns the symbol temporary for the homing location of the Temporary (spilled_temporary).
 *
 *  Only valid if the Temporary has the isHomeable property.
 */
Temporary
Spill_getHomeLocation(Temporary spilled_temporary);
#endif//$XCC_h

Temporary
Spill_getHomeLocation(Temporary spilled_temporary)
{
  Procedure procedure = CodeRegion_procedure(spill_coderegion);
  Temporary home = Procedure_getHomeable(procedure, spilled_temporary);
  Except_REQUIRE(Temporary_isHomeable(spilled_temporary));
  Except_ENSURE(Temporary_isSymbol(home));
  return home;
}


/**
 * Default spill cost values. 
 *
 * @todo Should be defined by architecture.
 *
 * The cost is compute as:
 * cost = latency+issue/max_issue.
 * The default costs are for a machine with:
 * issue of remat: 1/4
 * issue of load or store: 1/4
 * latency of load: 3
 * latency of store: 1
 *
 * @note This is the ST200 model.
 */
#define SPILL_COST_SPILL_REMAT 0
#define SPILL_COST_RELOAD_REMAT 1.25
#define SPILL_COST_SPILL 1.25
#define SPILL_COST_RELOAD 3.25


#ifdef $XCC_h
/**
 * Returns the cost for a spill (store) of the temporary.
 */
float
Spill_getSpillCost(Temporary spilled_temporary, bool force);
#endif//$XCC_h

float
Spill_getSpillCost(Temporary spilled_temporary, bool force)
{
  if (Spill_isRematerialisable(spilled_temporary) && !force) {
    return SPILL_COST_SPILL_REMAT;
  } else {
    return SPILL_COST_SPILL;
  }
}


#ifdef $XCC_h
/**
 * Returns the cost for a reload (load) of the temporary.
 */
float
Spill_getReloadCost(Temporary spilled_temporary);
#endif//$XCC_h

float
Spill_getReloadCost(Temporary spilled_temporary)
{
  if (Spill_isRematerialisable(spilled_temporary)) {
    return SPILL_COST_RELOAD_REMAT;
  } else {
    return SPILL_COST_RELOAD;
  }
}


#ifdef $XCC_h
/**
 * Find forward the first safe operation in the entry basic block for
 * insertion of spill code.
 *
 * This point is after any phi or entry code, and after Operation @p where.
 */
extern Operation
Spill_findFirstAppendPoint(BasicBlock bb, Operation where);
#endif//$XCC_h

Operation
Spill_findFirstAppendPoint(BasicBlock bb, Operation where)
{
  Operation start;
  Operation safe;
  bool where_found = false;

  start = Spill_startOperation(bb);
  start = start? Operation_prev(start): NULL;
      // start is now the first non-regular operation
      // or NULL if we were at beginning of BB
  if (start == NULL) return where; // Only regular operations at start of BB.

  if (where == NULL) where_found = true;
  safe = BasicBlock_firstOperation(bb);
  while (safe != NULL) {
    if (safe == where) where_found = true;
    if (safe == start) {
      break;
    }
    safe = Operation_next(safe);
  }

  return where_found? safe: where;
}

#ifdef $XCC_h
/**
 * Find backward the last safe operation in the basic block where spill code can be prepended.
 *
 * If @p where is @c NULL, consider that the insertion point is at the end of the
 * basic block.
 * @retval where if it is a safe insertion point.
 * @retval NULL if the insertion point is at the end of the basic block.
 */
extern Operation
Spill_findLastInsertPoint(BasicBlock bb, Operation where);
#endif//$XCC_h

Operation
Spill_findLastInsertPoint(BasicBlock bb, Operation where)
{
  Operation stop;
  Operation safe;
  bool where_found = false;

  stop = Spill_stopOperation(bb);
  stop = stop? Operation_next(stop): NULL;
                // stop is now the first non-regular operation
                // or NULL if we were at end of BB
  if (stop == NULL) return where; // Only regular operations at end of BB.
  if (where == NULL) where_found = true;
  safe = BasicBlock_lastOperation(bb);
  while (safe != NULL) {
    if (safe == where) where_found = true;
    if (safe == stop) {
      break;
    }
    safe = Operation_prev(safe);
  }

  return where_found? safe: where;
}

#ifdef $XCC_h
/**
 * Insert a load before a variable use for a spilled variable.
 */
void
Spill_variableInsertCopyLoad(Variable variable, Operation operation, int32_t argIndex);
#endif//$XCC_h

void
Spill_variableInsertCopyLoad(Variable variable, Operation operation, int32_t argIndex)
{
  BasicBlock block = Operation_basicBlock(operation);
  Temporary new_temp;
  Variable new_var;

  new_temp = Procedure_makeVirtualTemporary(
      BasicBlock_procedure(block),
      Temporary_regFile(Variable_TEMPORARY(variable)));
  new_var = (Variable)CodeRegion_makeVariableTemporary(
      BasicBlock_codeRegion(block),
      new_temp);
  Variable_raiseNoSpill(new_var);
  Variable_makeConsumers(
      new_var,
      Procedure_memory(BasicBlock_procedure(block)));

  /*      fprintf(stderr, "%s: insert pcopy (load) before:\n", __FUNCTION__);*/
  /*      Operation_pretty(op, stderr);*/

  Variable_setOperationArgument(new_var, operation, argIndex);

  Spill_insertMemToRegCopy(block, operation,
      Variable_TEMPORARY(new_var),
      Variable_TEMPORARY(variable));

  /*    Spill_insertLoad(block, op,*/
  /*        Variable_TEMPORARY(new_var),*/
  /*        Variable_TEMPORARY(variable));*/

}

/**
 * Determine if an operator is a "copy" operation that allows operands to be in 
 * memory.
 *
 * The corresponding operation only "moves" values between locations (registers 
 * or memory).
 */
static inline bool
Spill_operatorIsCopyWithMem(Operator operator)
{
  return (
/*      Operator_isSeqCopy(operator) ||*/ //FIXME: includes "make" functions
                                          //removed but then the actual arch_COPY
                                          //instruction is not taken.
      Operator_isSsa(operator) ||
      Operator_isParCopy(operator)
      );
}


#ifdef $XCC_h
/**
 * Spill one (SSA) Variable in an everywhere fashion.
 *
 * A store is inserted after its definition and a load before each use. New 
 * temporaries are created at each of these points.
 */
bool
Spill_variableEverywhere(Variable variable);
#endif//$XCC_h

bool
Spill_variableEverywhere(Variable variable)
{
  const_OperationResult result = Variable_producer(variable);
  Operation operation = OperationResult_OPERATION(result);
  Operator operator;
  BasicBlock bb;
  Temporary new_temp;
  Variable new_var;
  uint8_t index;

  Except_CHECK(operation);
  operator = Operation_operator(operation);
  bb = Operation_basicBlock(operation);

#ifdef FLO_DEBUG
  fprintf(stderr, "Spill everywhere on var V%d\n", Temporary_IDENTITY(Variable____(variable)));
#endif

  if(Operator_isKill(Operation_operator(operation))) {
#ifdef FLO_DEBUG
    Except_WARN("Producer is KILL, will not spill variable!\n");
#endif
    return false;
  }

  if(Variable_isNoSpill(variable)) {
#ifdef FLO_DEBUG
    Except_WARN("Variable is NOSPILL, will not spill it!\n");
#endif
    return false;
  }

  if(!result) {
#ifdef FLO_DEBUG
    Except_WARN("Producer of variable V%d is not set! will not perform spill on this variable...", Temporary_IDENTITY(Variable____(variable)));
#endif
    return false;
  }

  if(Operation_isDeadCode(operation)) {
#ifdef FLO_DEBUG
    Except_WARN("Producer of variable V%d is DEAD code! will not perform spill on this variable...", Temporary_IDENTITY(Variable____(variable)));
#endif
    return false;
  }

  Except_CHECK(bb);

  // Variable will be spilled
  // Force generation of stack location (required if variable only used in
  // copies or phi-functions
  Spill_genStackedLocation(Variable_TEMPORARY(variable));

  if(!Spill_operatorIsCopyWithMem(operator)) {
#ifdef FLO_DEBUG
    fprintf(stderr, "%s: append pcopy (store) after:\n", __FUNCTION__);
    Operation_pretty(operation, stderr);
#endif

    // Creating new temporary to hold the value before storage
    new_temp = Procedure_makeVirtualTemporary(
        BasicBlock_procedure(bb),
        Temporary_regFile(Variable_TEMPORARY(variable)));
    // New SSA variable on this temporary
    new_var = (Variable)CodeRegion_makeVariableTemporary(
        BasicBlock_codeRegion(bb),
        new_temp);
    Variable_raiseNoSpill(new_var); // mark variable as NoSpill to avoid looping
    // Initialize SSA variable consumers list
    Variable_makeConsumers(
        new_var,
        Procedure_memory(BasicBlock_procedure(bb)));

    // Producer of orig variable is now producer of new var
    Variable_setOperationResult(
        new_var,
        operation,
        OperationResult_RESINDEX(result));

    // Make a parallel copy from register to memory.
    Spill_appendRegToMemCopy(bb, operation,
        Variable_TEMPORARY(new_var),
        Variable_TEMPORARY(variable));
  }

  {
    const_IDList consumers = Variable_consumers(variable);
    if(IDList_isEmpty(consumers)) {
      // Maybe variable is just local ?
      // Scan basic block starting from current operation
      //Except_CHECK(false); // Should not be local
      while((operation = Operation_next(operation))) {
        Operation_ARGUMENTS_FOREACH_Temporary_Index(operation, temporary, index) {
          if(temporary == Variable____(variable)) {
            operator = Operation_operator(operation);
            if(!Spill_operatorIsCopyWithMem(Operation_operator(operation))) {
              Spill_variableInsertCopyLoad(variable, operation, index);
#ifdef FLO_DEBUG
              fprintf(stderr, "%s: insert pcopy (load) for local use before:\n", __FUNCTION__);
              Operation_pretty(operation, stderr);
#endif
            }
          }
        } Operation_ARGUMENTS_ENDEACH_Temporary_Index;
      }
    } else {
      OperationArgument lastArgument = IDList_lastItem(consumers);
      Variable_CONSUMERS_FOREACH_OperationArgument(variable, argument) {
        operation = OperationArgument_OPERATION(argument);
        operator = Operation_operator(operation);
        if(!Spill_operatorIsCopyWithMem(Operation_operator(operation))) {
          Spill_variableInsertCopyLoad(variable,
              operation,
              OperationArgument_ARGINDEX(argument));
#ifdef FLO_DEBUG
          fprintf(stderr, "%s: insert pcopy (load) before:\n", __FUNCTION__);
          Operation_pretty(operation, stderr);
#endif
        }
        if(argument == lastArgument) break;
      } Variable_CONSUMERS_ENDEACH_OperationArgument;
    }
  }

  return true;
}


#define Spill_temporaryIsRegFileVariableWithoutMemory(temporary, regFile) \
 (Temporary_isVariable(temporary) && \
   !Variable_hasMemory(Temporary_variable(temporary)) && \
   Temporary_regFile(temporary) == regFile)


/**
 * Ctor and fill an IQueue with the allocatable registers of a given class.
 * @param use_FP tells whether to include the Frame Pointer or not.
 *
 */
static inline void
Spill_allocatableRegisters(IQueue allocatable, Memory memory, Convention convention, RegFile regFile, bool use_FP)
{
  Register reg;
  int registerCount = 0;
  RegisterSet_ registerSet_ = Convention_allocatableSet(convention, regFile);
  RegisterSet registerSet = &registerSet_;

  for (reg = RegFile_lowRegister(regFile);
       reg <= RegFile_highRegister(regFile); reg++) {
    if(RegisterSet_contains(registerSet, reg) &&
       (use_FP ||
        !Convention_isFrameRegister(convention, reg)))
      registerCount++;
  }
  IQueue_Ctor(allocatable, memory, sizeof(Register), registerCount);
  for (reg = RegFile_lowRegister(regFile);
       reg <= RegFile_highRegister(regFile); reg++) {
    if(RegisterSet_contains(registerSet, reg) &&
       (use_FP ||
        !Convention_isFrameRegister(convention, reg)))
      *((Register *)IQueue_push(allocatable)) = reg;
  }
  RegisterSet_Dtor(registerSet);
}


#ifdef $XCC_h
/**
 * Perform assignment on a basic block with Belady-like algorithm.
 *
 * @param regFile is the register file we consider
 * @param allocatable is an IQueue containing the allocatable registers.
 */
void
Spill_basicBlockBelady(BasicBlock this, RegFile regFile, IQueue allocatable);
#endif//$XCC_h

void
Spill_basicBlockBelady(BasicBlock this, RegFile regFile, IQueue allocatable)
{
  Memory memory = BasicBlock_MEMORY(this);
  TemporaryTable_ temporaryTable_;
  TemporaryTable temporaryTable = &temporaryTable_;
  TemporarySet liveIn = BasicBlock_liveInSet(this);
  TemporarySet liveOut = BasicBlock_liveOutSet(this);
  TemporarySparse_ live_;
  TemporarySparse live = &live_;
  int allocatableCount = IQueue_count(allocatable);
  RegisterSet_ okColors_;
  RegisterSet okColors = &okColors_;


#ifdef FLO_DEBUG
  if(liveIn) {
    fprintf(stderr, "%s:%d Live in of BB %d: ", __FILE__, __LINE__, BasicBlock_identity(this));
    TemporarySet_pretty(liveIn, stderr);
    fprintf(stderr, "\n");
  }
  if(liveOut) {
    fprintf(stderr, "%s:%d Live out of BB %d: ", __FILE__, __LINE__, BasicBlock_identity(this));
    TemporarySet_pretty(liveOut, stderr);
    fprintf(stderr, "\n");
  }
#endif//FLO_DEBUG

  // Check all operands of phis are in memory
  if(BasicBlock_phiCount(this) > 0) {
    TemporarySet tempSet = BasicBlock_phiUseSet(this);
    TemporarySet_FOREACH_Temporary(tempSet, temporary) {
      Except_CHECK( Temporary_regFile(temporary) != regFile ||
          Variable_hasMemory(Temporary_variable(temporary)));
    } TemporarySet_ENDEACH_Temporary;
    tempSet = BasicBlock_phiDefSet(this);
    TemporarySet_FOREACH_Temporary(tempSet, temporary) {
      Except_CHECK( Temporary_regFile(temporary) != regFile ||
          Variable_hasMemory(Temporary_variable(temporary)));
    } TemporarySet_ENDEACH_Temporary;
  }

/*#if !_OPTIMIZE*/
  if(liveIn) {
  TemporarySet_FOREACH_Temporary(liveIn, temporary) {
    Except_CHECK( Temporary_regFile(temporary) != regFile ||
        Variable_hasMemory(Temporary_variable(temporary)));
  } TemporarySet_ENDEACH_Temporary;
/*#endif*/
  }


  // Populate table with all temporaries
  TemporaryTable_Ctor(temporaryTable, memory, 64);
  BasicBlock_FOREACH_Operation(this, operation) {
    Operation_OPERANDS_FOREACH_Temporary(operation, temporary) {
      if(Spill_temporaryIsRegFileVariableWithoutMemory(temporary, regFile))
        TemporaryTable_insert(temporaryTable, temporary);
    } Operation_OPERANDS_ENDEACH_Temporary;
  } BasicBlock_ENDEACH_Operation;

#ifdef FLO_DEBUG
  fprintf(stderr, "%s:%d Temporary Table for basic block %s:\n", __FILE__, __LINE__, Label_name(BasicBlock_label(this)));
  TemporaryTable_pretty(temporaryTable, stderr);
  fprintf(stderr, "\n");
#endif//FLO_DEBUG

  // There should be no actual live in, but for variables that have Memory
  {
    bool spillRequired = false;
    int operationCounter = 0;
    int numToAssign = 0;
    bool check;  // To check RegisterSet insertion / removal

    TemporarySparse_Ctor(live, memory, temporaryTable);
    RegisterSet_Ctor(okColors);
    Except_CHECK(RegisterSet_isEmpty(okColors));

    // Fill RegisterSet with 
    IQueue_FOREACH(allocatable, Register, _reg) {
      check = RegisterSet_insert(okColors, *_reg);
      Except_CHECK(check);
    } IQueue_ENDEACH;

    BasicBlock_FORBACK_Operation(this, operation) {
      spillRequired = false;
      operationCounter++;   // Count operation from last op in basic block

      Operation_RESULTS_FOREACH_Temporary(operation, result){
        // remove results from set of live variables
        if(Spill_temporaryIsRegFileVariableWithoutMemory(result, regFile)) {
#ifdef FLO_DEBUG
          fprintf(stderr, "         removing variable to live: ");
          Variable_pretty(Temporary_variable(result), stderr, 0);
          fprintf(stderr, "\n");
#endif//FLO_DEBUG
          TemporarySparse_remove(live, result);
          // Mark register as available again
/*          Except_CHECK(Temporary_hasRegister(result));*/
          if(Temporary_hasRegister(result)) {
            check = RegisterSet_insert(okColors, Temporary_register(result));
            Except_CHECK(check);
          } else {
            // If no register, this is deadcode (result is not used)
            BasicBlock_detachOperation(this, operation);
          }
        }
      } Operation_RESULTS_ENDEACH_Temporary;

      numToAssign = 0;

      Operation_ARGUMENTS_FOREACH_Temporary(operation, argument){
        if(Spill_temporaryIsRegFileVariableWithoutMemory(argument, regFile)) {
          Variable varArgument = Temporary_variable(argument);
          // add argument to set of live variables
#ifdef FLO_DEBUG
          fprintf(stderr, "         adding variable to live: ");
          Variable_pretty(Temporary_variable(argument), stderr, 0);
          fprintf(stderr, "\n");
#endif//FLO_DEBUG
          if(TemporarySparse_insert(live, argument)) {
            numToAssign++;
          }

          // Set "next use" of argument
          Variable_ORDERING(varArgument) = operationCounter;

        }
      } Operation_ARGUMENTS_ENDEACH_Temporary;

#ifdef FLO_DEBUG
      fprintf(stderr, "      Temporary sparse live (count %d):\n", TemporarySparse_count(live));
      TemporarySparse_pretty(live, stderr);
      fprintf(stderr, "\n");
#endif//FLO_DEBUG


      // Need to spill if not enough registers
      if(numToAssign > RegisterSet_count(okColors)) {
        fprintf(stderr, "%s:%d more to assign that okColors!\n", __FILE__, __LINE__);
        spillRequired = true;
      }

      while(spillRequired) {
        // Scan all live variables, and spill the one(s) with furthest use
        Variable toSpill = NULL;
        int furthest = operationCounter+1;
        TemporarySparse_FOREACH_Temporary(live, temporary){
          Variable variable = Temporary_variable(temporary);
          if(Variable_ORDERING(variable) < furthest) {
            toSpill = variable;
            furthest = Variable_ORDERING(variable);
          }
        } TemporarySparse_ENDEACH_Temporary;
#ifdef FLO_DEBUG
        fprintf(stderr, "      will spill variable: ");
        Variable_pretty(toSpill, stderr, 0);
        fprintf(stderr, "\n");
#endif//FLO_DEBUG
        Except_THROW("Did not check spill belady like in basic block yet!");
        Spill_variableEverywhere(toSpill);
        spillRequired = false;
      }

#ifdef FLO_DEBUG
      fprintf(stderr, "%s:%d Available registers: ", __FILE__, __LINE__);
      RegisterSet_pretty(okColors, stderr, 
          Program_processor(
            Procedure_program(
              BasicBlock_procedure(this))));
      fprintf(stderr, "\n");
#endif//FLO_DEBUG

      // assign arguments to registers if not in register
      TemporarySparse_FOREACH_Temporary(live, temporary) {
        Variable variable = Temporary_variable(temporary);
        Except_CHECK(Temporary_isVariable(temporary));
        if(!Variable_isAssigned(variable)) {
          Register reg = Register__UNDEF;
          Operator operator = Operation_operator(operation);
          // Choose a register in the set of available colors
          if(Operator_isParCopy(operator) ||
             Operator_isSeqCopy(operator)) {
            // Try to choose the same register if use is a copy
            int32_t argIdx = -1;
            Operation_ARGUMENTS_FOREACH_Temporary_Index(operation, temp, index) {
              if(temp == temporary) {
                argIdx = index;
                break;
              }
            } Operation_ARGUMENTS_ENDEACH_Temporary_Index;
            if(argIdx != -1) {
              Temporary res = Operation_getResult(operation, argIdx);
              if(Temporary_hasRegister(res)) {
                reg = Temporary_register(res);
                if(! RegisterSet_remove(okColors, reg)) {
                  reg = Register__UNDEF;
                }
              }
            }
          }
          if(reg == Register__UNDEF) {
            // Try to choose the same register if variable defined in a copy
            const_OperationResult res = Variable_producer(variable);
            Operation operation = OperationResult_OPERATION(res);
            Operator operator = Operation_operator(operation);

            if(Operator_isParCopy(operator) ||
                Operator_isSeqCopy(operator)) {
              int32_t idx = OperationResult_RESINDEX(res);
              Temporary arg = Operation_getArgument(operation, idx);
              if(Temporary_hasRegister(arg)) {
                reg = Temporary_register(arg);
                if(! RegisterSet_remove(okColors, reg)) {
                  reg = Register__UNDEF;
                }
              }
            }
          }
          if(reg == Register__UNDEF) {
            reg = RegisterSet_choose(okColors);
          }
          Except_CHECK(reg != Register__UNDEF);
          // Assign to next available register
          Variable_assignRegister(variable, reg);
        }
      } TemporarySparse_ENDEACH_Temporary;

    } BasicBlock_ENDBACK_Operation;
    
    TemporarySparse_Dtor(live);
    RegisterSet_Dtor(okColors);
  }

  TemporaryTable_Dtor(temporaryTable);
}


#ifdef $XCC_h
/**
 * Spill all Variable%s in a codeRegion in an everywhere fashion.
 *
 * @todo: use a different list of variable. Risk of looping.
 */
void
Spill_allVariableEverywhere(CodeRegion codeRegion);
#endif//$XCC_h

void
Spill_allVariableEverywhere(CodeRegion codeRegion)
{
  Spill_initCodeRegion(codeRegion);
  CodeRegion_FOREACH_Variable(codeRegion, variable) {
    Spill_variableEverywhere(variable);
  } CodeRegion_ENDEACH_Variable;
  Spill_finiCodeRegion(codeRegion);
}


#ifdef $XCC_h
/**
 * Spill global variables in an everywhere fashion, then perform a Belady-like 
 * algorithm on all basic blocks to assign all variables to registers (or 
 * memory).
 *
 * @note: the global table of the code region must be computed.
 */
void
Spill_allGlobalEverywhereLocalBelady(CodeRegion codeRegion);
#endif//$XCC_h

void
Spill_allGlobalEverywhereLocalBelady(CodeRegion codeRegion)
{
  Memory memory = CodeRegion_memory(codeRegion);
  LoopForest loopForest = LoopForest_new(memory, codeRegion);
  TemporaryTable globalTable = CodeRegion_globalTable(codeRegion);
  TemporaryTable_ temporaryTable_;
  TemporaryTable temporaryTable = &temporaryTable_;
  LiveSets liveSets;
  IQueue_ allocatable_;
  IQueue allocatable = &allocatable_;
  Procedure procedure = CodeRegion_procedure(codeRegion);
  Program program = Procedure_program(procedure);
  Convention convention = Program_convention(program);
  RegFile regFile; // FIXME: RegFile_**_GRF

  Spill_initCodeRegion(codeRegion);

  for (regFile = 0; regFile < RegFile__Count; regFile++) {

    if(regFile == RegFile__UNDEF) {
      continue;
    }
#ifdef FLO_DEBUG
    fprintf(stderr, "%s:%d SPILL EVERYWHERE on regfile %s\n", __FILE__, __LINE__,
        RegFile_name(regFile));
#endif//FLO_DEBUG

    // Spill all global variables
    TemporaryTable_FOREACH_Temporary(globalTable, temporary) {
      if(Temporary_regFile(temporary) == regFile) {
        if (Temporary_isVariable(temporary))
          Spill_variableEverywhere(Temporary_variable(temporary));
        else
          Except_THROW("not a variable");
      }
    } TemporaryTable_ENDEACH_Temporary;

    // Create liveness for all variables
    TemporaryTable_Ctor(temporaryTable, memory, 1024);
    CodeRegion_FOREACH_Variable(codeRegion, variable) {
      TemporaryTable_insert(temporaryTable, Variable____(variable));
    } CodeRegion_ENDEACH_Variable;

    liveSets = LiveSets_make(memory, loopForest, temporaryTable);

#ifdef FLO_DEBUG
    fprintf(stderr, "%s:%d After spilling globals, and before Belady:\n", __FILE__, __LINE__);
    CodeRegion_pretty(codeRegion, stderr);
#endif//FLO_DEBUG

    // Ctor and initialize the set of allocatable registers
    Spill_allocatableRegisters(allocatable,
        memory,
        convention,
        regFile,
        Frame_isFixed(Procedure_frame(procedure)));
    // Remove FP from set of allocatable register if proc has variable-length frame

#ifdef FLO_DEBUG
    fprintf(stderr, "%s:%d List of allocatable registers: ", __FILE__, __LINE__);
    IQueue_FOREACH(allocatable, Register, _reg){
      fprintf(stderr, " %s", Register_name(*_reg));
    } IQueue_ENDEACH;
    fprintf(stderr, "\n");
#endif//FLO_DEBUG

    // Belady spill / assignment on each basic block
    CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
      Spill_basicBlockBelady(basicBlock, regFile, allocatable);
    } CodeRegion_ENDEACH_BasicBlock;
    IQueue_Dtor(allocatable);
  }

  Spill_finiCodeRegion(codeRegion);
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
// Minir initialization for tests
{
  Minir minir = Minir_new(Memory_Root, stdin);
  Program program;
  Minir_KEEP_INDEX(minir) = true;
#endif//$XCC__c

#ifdef $XCC__c
  // Test the spill
  program = Minir_nextProgram(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "spill-by-hand");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    CodeRegion_pretty(codeRegion, stdout);
    // Spill variable V100
    Spill_initCodeRegion(codeRegion);
    {
      BasicBlock block = CodeRegion_firstBlock(codeRegion);
      // Spill variable named V100
      Operation def = BasicBlock_firstOperation(block);
      Operation use = BasicBlock_lastOperation(block);
      Temporary to_spill = Operation_getResult(def, 0);
      Spill_appendStore(block, def, to_spill, to_spill, false);
      Spill_insertLoad(block, use, to_spill, to_spill);

      // Spill variable named V101
      def = Operation_next(Operation_next(def)); // skip store of V100
      use = Operation_prev(Operation_prev(use)); // skip load of V100
      to_spill = Operation_getResult(def, 0);
      Spill_appendStore(block, def, to_spill, to_spill, false);
      Spill_insertLoad(block, use, to_spill, to_spill);
    }
    Spill_finiCodeRegion(codeRegion);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
/*  SSAForm_constructAfterMinir(Memory_Root, program, true);*/
  SSAForm_constructAfterMinir(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "spill-everywhere one");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    CodeRegion_pretty(codeRegion, stdout);
    Spill_initCodeRegion(codeRegion);
    CodeRegion_FOREACH_Variable(codeRegion, variable) {
      Spill_variableEverywhere(variable);
      break;
    } CodeRegion_ENDEACH_Variable;
    Spill_finiCodeRegion(codeRegion);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
  SSAForm_constructAfterMinir(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "spill-everywhere all");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    CodeRegion_pretty(codeRegion, stdout);
    Spill_allVariableEverywhere(codeRegion);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
  program = Minir_nextProgram(minir);
  SSAForm_constructAfterMinir(minir);
  {
    Optimize optimize = Program_optimize(program);
    Procedure procedure = Program_findProcedure(program, "spill-everywhere complete");
    CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
    CodeRegion_pretty(codeRegion, stdout);
    Spill_allVariableEverywhere(codeRegion);
    CodeRegion_pretty(codeRegion, stdout);
  }
  Program_delete(program);
#endif//$XCC__c

#ifdef $XCC__c
// Delete Minir constructs
  Minir_delete(minir);
}
#endif//$XCC__c



////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Initialize the Spill module.
 */
#define Spill_INIT()
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Finalize the Spill module.
 */
#define Spill_FINI()
#endif//$XCC_h

#if XCC__C
static void
Spill_TEST(void)
{
#include "CGO/Spill_.c"
}

int
main(int argc, char **argv)
{
  CGO_Test(Spill_TEST);
  return 0;
}
#endif

