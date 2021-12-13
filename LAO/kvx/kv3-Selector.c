#ifdef $XCC_h
/*
 * kv3-Selector.c
 *
 * Copyright 2009 Kalray.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#include "MDT/MDT_.h"
#include "LIR/LIR_.h"

/*
 * Forward declarations. See implementation for description.
 */
static Operator get_load_operator(Selector this, RegFile class, NativeType type,
                                  Temporary offset);
static Operator get_store_operator(Selector this, RegFile class, NativeType type,
                                  Temporary offset);
static Temporary Generate_Absolute(Selector this, RegFile regFile, NativeType nativeType, int64_t value);
static Temporary Get_DummyReg(Selector this, RegFile regFile);
static Temporary Get_SP(Selector this);
static Temporary Get_FP(Selector this);
static Temporary Get_GP(Selector this);
static Temporary Get_RA(Selector this);
static Symbol Get_Symbol_SP(Selector this);
static Symbol Get_Symbol_FP(Selector this);
static Symbol Get_Symbol_GP(Selector this);
static inline void checkRegSrc1Src2(Selector this,
                                    Temporary* reg_src1, Temporary* new_src2,
                                    Temporary  src1, Temporary src2);
static inline void checkRegFile(Temporary temporary, RegFile regFile);
static inline Temporary Temporary_ofImmediate(Selector this,
                                              RegFile regFile, 
                                              Temporary temporary);

/**
 * Return the general purpose register file.
 *
 * @todo should find a better place for this function.
 */
RegFile
Selector_genPurposeRegFile()
{
  return RegFile_kv3_GPR;
}

/**
 * Selector a load from memory.
 */
void
Selector_selectLOAD(Selector this, NativeType nativeType, Temporary dst, Temporary base,
              Temporary offset)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Operator operator;
  Temporary load_dst;

  checkRegFile(dst, RegFile_kv3_GPR);

  Except_ENSURE(Temporary_hasRegFile(dst));
  Except_ENSURE(Temporary_hasLiteral(offset));
  Except_ENSURE(Temporary_hasRegFile(base));
  Except_ENSURE(Temporary_regFile(base) == RegFile_kv3_GPR);

  load_dst = dst;
  operator = get_load_operator(this, RegFile_kv3_GPR, nativeType, offset);
  
  Except_ENSURE(operator != Operator__UNDEF);

  Selector_makeOperation_1_2(this, operator, load_dst, offset, base);
}

/**
 * Selector a store from memory.
 */
void
Selector_selectSTORE(Selector this, NativeType nativeType, Temporary src, Temporary base,
               Temporary offset)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Operator operator;
  Temporary store_src;

  checkRegFile(src, RegFile_kv3_GPR);

  Except_ENSURE(Temporary_hasRegFile(src));
  Except_ENSURE(Temporary_hasLiteral(offset));
  Except_ENSURE(Temporary_hasRegFile(base));
  Except_ENSURE(Temporary_regFile(base) == RegFile_kv3_GPR);

  store_src = src;
  operator = get_store_operator(this, RegFile_kv3_GPR, nativeType, offset);
  
  Except_ENSURE(operator != Operator__UNDEF);

  Selector_makeOperation_0_3(this, operator, offset, base, store_src);
}

/**
 * Selector a move between different regFile tns.
 */
void
Selector_selectMOVE(Selector this, Temporary dst, Temporary src)
{
#if 0
  Except_ENSURE(Temporary_regFile(dst) != Temporary_regFile(src));
  if (Temporary_regFile(dst) == RegFile_kv3_BR) {
    /* Move GRC -> BRC */
    Selector_makeOperation_1_1(this, Operator_kv3_mtb_1branch_2general_simple, dst, src);
  } else {
    /* Move BRC -> GRC */
    Selector_makeOperation_1_1(this, Operator_kv3_mfb_1general_2branch_simple, dst, src);
  }
#endif
  Except_NEVER(true);
}

/**
 * Selector a copy between same regFile temporaries
 */
void
Selector_selectCOPY(Selector this, Temporary dst, Temporary src)
{
  checkRegFile(dst, RegFile_kv3_GPR);
  checkRegFile(src, RegFile_kv3_GPR);
  Selector_makeOperation_1_1(this, Operator_kv3_COPYD_1singleReg_2singleReg_simple, dst, src);
}

/**
 * See interface description.
 */
void
Selector_selectCOPYOnTrue(Selector this, Temporary dst, Temporary cond, Temporary src)
{
#if 0
  Except_ENSURE(Temporary_regFile(dst) == Temporary_regFile(src));
  Except_ENSURE(Temporary_regFile(dst) == RegFile_kv3_GR);
  Except_ENSURE(Temporary_regFile(cond) == RegFile_kv3_BR);
  Selector_makeOperation_1_3(this,
                             Operator_kv3_slct_1general_2branch_3general_4general_simple,
                             dst, cond, src, dst);
#endif
  Except_NEVER(true);
}

/**
 * See interface description.
 */
void
Selector_selectCOPYOnFalse(Selector this, Temporary dst, Temporary cond, Temporary src)
{
#if 0
  Except_ENSURE(Temporary_regFile(dst) == Temporary_regFile(src));
  Except_ENSURE(Temporary_regFile(dst) == RegFile_kv3_GR);
  Except_ENSURE(Temporary_regFile(cond) == RegFile_kv3_BR);
  Selector_makeOperation_1_3(this,
                             Operator_kv3_slctf_1general_2branch_3general_4general_simple,
                             dst, cond, src, dst);
#endif
  Except_NEVER(true);
}

/**
 * See interface description.
 */
void
Selector_selectSWAP(Selector this, Temporary dst1, Temporary dst2)
{
  Operator operator;
  checkRegFile(dst1, RegFile_kv3_GPR);
  checkRegFile(dst2, RegFile_kv3_GPR);

  operator = Operator_kv3_XORD_1singleReg_2singleReg_3singleReg_simple,
  Selector_makeOperation_1_2(this, operator, dst1, dst1, dst2);
  Selector_makeOperation_1_2(this, operator, dst2, dst1, dst2);
  Selector_makeOperation_1_2(this, operator, dst1, dst1, dst2);
}

/**
 * Selector a control transfer to label.
 */
void
Selector_selectGOTO(Selector this, Label label)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Temporary label_temporary = Procedure_makeLabelTemporary(procedure, label, Relocation_kv3_pcrel27);
  Selector_makeOperation_0_1(this, Operator_kv3_GOTO_1pcrel27_simple, label_temporary);
}

/**
 * Generates code to implement a IFTRUE
 */
void
Selector_selectIFTRUE(Selector this, Temporary condition, Label label)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Temporary new_condition = condition;
  Temporary label_temporary;

  Except_REQUIRE(Temporary_hasRegFile(condition));

  label_temporary = Procedure_makeLabelTemporary(procedure, label, Relocation_kv3_pcrel17);

  Selector_makeOperation_0_2(this, Operator_kv3_CB_WNEZ_2singleReg_3pcrel17_simple,
                                   new_condition,
                                   label_temporary);
}


/**
 * Generates code to implement a IFFALSE
 */
void
Selector_selectIFFALSE(Selector this, Temporary condition, Label label)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Temporary new_condition = condition;
  Temporary label_temporary;

  Except_REQUIRE(Temporary_hasRegFile(condition));

  label_temporary = Procedure_makeLabelTemporary(procedure, label, Relocation_kv3_pcrel17);

  Selector_makeOperation_0_2(this, Operator_kv3_CB_WEQZ_2singleReg_3pcrel17_simple,
                                   new_condition,
                                   label_temporary);
}

/**
 * Generates an ADD
 */
void
Selector_selectADD(Selector this, Temporary dst, Temporary src1, Temporary src2)
{
  // TODO: Do we need to support when both src1 and src2 are literal?
  //       Can we common functions with LOr?
  Procedure procedure = Selector_PROCEDURE(this);
  Operator  operator = Operator__UNDEF;
  Temporary reg_temp1 = NULL;
  Temporary temp2 = NULL;

  checkRegFile(dst, RegFile_kv3_GPR);
  checkRegSrc1Src2(this, &reg_temp1, &temp2, src1, src2);

  if (Temporary_hasRegFile(temp2)) {
    operator = Operator_kv3_ADDD_1singleReg_2singleReg_3singleReg_simple;
  }
  else if (Temporary_isAbsolute(temp2)) {
    Literal temp2_literal = Temporary_LITERAL(temp2);
    if (Immediate_canEncode(Immediate_kv3_signed10,
                            Literal_VALUE(temp2_literal)))
    {
      operator = Operator_kv3_ADDD_1singleReg_2singleReg_3signed10_simple;
    }
    else {
      Except_CHECK(Immediate_canEncode(Immediate_kv3_signed37,
                   Literal_VALUE(temp2_literal)));
      operator = Operator_kv3_ADDD_1singleReg_2singleReg_3signed37_double;
    }
  }
  else Except_NEVER(true);
  Selector_makeOperation_1_2(this, operator, dst, reg_temp1, temp2);
}

/**
 * Generates code to implement a SUB
 */
void
Selector_selectSUB(Selector this, Temporary dst, Temporary src1, Temporary src2)
{
  // TODO: Do we need to support when both src1 and src2 are literal?
  //       Can we common functions with LOr?
  Procedure procedure = Selector_PROCEDURE(this);
  Operator  operator = Operator__UNDEF;

  checkRegFile(dst, RegFile_kv3_GPR);

  // If $rXX - immediate, generates $rXX + (-immediate) instead
  if (Temporary_hasRegFile(src1)) {
    if (Temporary_hasRegFile(src2)) {
      operator = Operator_kv3_SBFD_1singleReg_2singleReg_3singleReg_simple;
    } else {
      Except_CHECK(Temporary_isAbsolute(src2));
      Literal temp2_literal = Temporary_LITERAL(src2);
      ImmediateValue value = Literal_VALUE(temp2_literal);
      Literal_setValue(temp2_literal, -value);
      Selector_selectADD(this, dst, src1, src2);
      return;
    }
  }
  else if (Temporary_isAbsolute(src1)) {
    Literal temp1_literal = Temporary_LITERAL(src1);
    if (Immediate_canEncode(Immediate_kv3_signed10, Literal_VALUE(temp1_literal)))
    {
      operator = Operator_kv3_SBFD_1singleReg_2singleReg_3signed10_simple;
    }
    else {
      Except_CHECK(Immediate_canEncode(Immediate_kv3_signed37, Literal_VALUE(temp1_literal)));
      operator = Operator_kv3_SBFD_1singleReg_2singleReg_3signed37_double;
    }
  }
  else Except_NEVER(true);
  Selector_makeOperation_1_2(this, operator, dst, src2, src1);
}


/**
 * Generates a logical AND
 */
void
Selector_selectLAND(Selector this, Temporary dst, Temporary src1, Temporary src2)
{
  // TODO: Do we need to support when both src1 and src2 are literal?
  //       Can we common functions with LOr?
  Procedure procedure = Selector_PROCEDURE(this);
  Operator operator = Operator_kv3_LANDW_1singleReg_2singleReg_3singleReg_simple;
  Temporary reg_temp1 = NULL;
  Temporary reg_temp2 = NULL;

  checkRegFile(dst, RegFile_kv3_GPR);

  if (Temporary_hasRegFile(src1)) {
    reg_temp1 = src1;
  }
  else if (Temporary_isAbsolute(src1)) {
    reg_temp1 = Temporary_ofImmediate(this, RegFile_kv3_GPR, src1);
  }
  else Except_NEVER(true);
  if (Temporary_hasRegFile(src2)) {
    reg_temp2 = src2;
  }
  else if (Temporary_isAbsolute(src2)) {
    reg_temp2 = Temporary_ofImmediate(this, RegFile_kv3_GPR, src2);
  }
  else Except_NEVER(true);

  Selector_makeOperation_1_2(this, operator, dst, reg_temp1, reg_temp2);
}

/**
 * Generates a logical NOT
 */
void
Selector_selectLNOT(Selector this, Temporary dst, Temporary src)
{
#if 0
  // TODO: Do we need to support when src is literal?
  Procedure procedure = Selector_PROCEDURE(this);
  Operator  operator = Operator__UNDEF;
  Temporary new_source = src;

  Except_REQUIRE(Temporary_hasRegFile(dst));
  Except_REQUIRE(Temporary_hasRegFile(src));
  Except_ENSURE(  Temporary_regFile(dst) == RegFile_kv3_GR
                || Temporary_regFile(dst) == RegFile_kv3_BR);

  // Select the appropriate operator
  if (Temporary_regFile(dst) == RegFile_kv3_GR)
    operator = Operator_kv3_norl_1general_2general_3general_simple;
  else if (Temporary_regFile(dst) == RegFile_kv3_BR)
    operator = Operator_kv3_norl_1branch_2general_3general_simple;

  // Make copy if necessary
  if (Temporary_regFile(src) != RegFile_kv3_GR) {
    new_source = Procedure_makeVirtualTemporary(procedure, RegFile_kv3_GR);
    Selector_selectMOVE(this, new_source, src);
  }
  // Make the LNot operation.
  Except_ENSURE(operator != Operator__UNDEF);
  Selector_makeOperation_1_2(this, operator, dst, new_source,
                             Procedure_makeDedicatedTemporary(procedure, Register_kv3_R0));
#endif
  Except_NEVER(true);
}

/**
 * Generates a logical OR
 */
void
Selector_selectLOR(Selector this, Temporary dst, Temporary src1, Temporary src2)
{
#if 0
  // TODO: Do we need to support when both src1 and src2 are literal?
  //       Can we common functions with LAnd?
  Procedure procedure = Selector_PROCEDURE(this);
  Operator  operator = Operator__UNDEF;
  Temporary reg_temp1 = NULL;
  Temporary temp2 = NULL;

  Except_REQUIRE(Temporary_hasRegFile(dst));
  Except_ENSURE(  Temporary_regFile(dst) == RegFile_kv3_GR
                || Temporary_regFile(dst) == RegFile_kv3_BR);

  checkRegSrc1Src2(this, &reg_temp1, &temp2, src1, src2);

  if (Temporary_hasRegFile(temp2)) {
    if (Temporary_regFile(dst) == RegFile_kv3_GR)
      operator = Operator_kv3_orl_1general_2general_3general_simple;
    else
      operator = Operator_kv3_orl_1branch_2general_3general_simple;
  }
  else if (Temporary_isAbsolute(temp2)) {
    Literal temp2_literal = Temporary_LITERAL(temp2);
    if (Immediate_canEncode(Immediate_kv3_isrc2,
                            Literal_VALUE(temp2_literal)))
    {
      if (Temporary_regFile(dst) == RegFile_kv3_GR)
        operator = Operator_kv3_orl_1general_2general_3isrc2_simple;
      else
        operator = Operator_kv3_orl_1branch_2general_3isrc2_simple;
    }
    else {
      if (Temporary_regFile(dst) == RegFile_kv3_GR)
        operator = Operator_kv3_orl_1general_2general_3xsrc2_simple;
      else
        operator = Operator_kv3_orl_1branch_2general_3xsrc2_simple;
    }
  } else Except_NEVER(true);
  Selector_makeOperation_1_2(this, operator, dst, reg_temp1, temp2);
#endif
  Except_NEVER(true);
}

/**
 * Generates a select.
 */
void
Selector_selectSELECT(Selector this, Temporary dst, Temporary cond,
                               Temporary src1, Temporary src2)
{
#if 0
  // TODO: Do we need to support when both src1 and src2 are literal?
  //       Can we common functions with LAnd?
  Procedure procedure = Selector_PROCEDURE(this);
  Operator  operator = Operator__UNDEF;
  Temporary reg_temp1 = NULL;
  Temporary temp2 = NULL;

  Except_REQUIRE(Temporary_hasRegFile(dst));
  Except_REQUIRE(Temporary_hasRegFile(cond));

  checkRegSrc1Src2(this, &reg_temp1, &temp2, src1, src2);

  if (Temporary_regFile(cond) != RegFile_kv3_BR) {
    Temporary new_cond = Procedure_makeVirtualTemporary(procedure, RegFile_kv3_BR);
    Selector_selectMOVE(this, new_cond, cond);
    cond = new_cond;
  }

  if (Temporary_hasRegFile(temp2)) {
    operator = Operator_kv3_slct_1general_2branch_3general_4general_simple;
  }
  else if (Temporary_isAbsolute(temp2)) {
    Literal temp2_literal = Temporary_LITERAL(temp2);
    if (Immediate_canEncode(Immediate_kv3_isrc2,
                            Literal_VALUE(temp2_literal)))
    {
      operator = Operator_kv3_slct_1general_2branch_3general_4isrc2_simple;
    }
    else {
      operator = Operator_kv3_slct_1general_2branch_3general_4xsrc2_simple;
    }
  } else Except_NEVER(true);
  Selector_makeOperation_1_3(this, operator, dst, cond, reg_temp1, temp2);
#endif
  Except_NEVER(true);
}

/**
 * Selector an tempory int dst.
 */
void
Selector_selectMAKE(Selector this, Temporary dst, Temporary src)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Operator operator;
  Literal literal;

  checkRegFile(dst, RegFile_kv3_GPR);
  Except_CHECK(Temporary_isAbsolute(src));

  literal = Temporary_LITERAL(src);
  if (Immediate_canEncode(Immediate_kv3_signed16, Literal_VALUE(literal)))
  {
    Selector_makeOperation_1_1(this, Operator_kv3_MAKE_1singleReg_2signed16_simple, dst, src);
  }
  else {
    Except_CHECK(Immediate_canEncode(Immediate_kv3_signed37, Literal_VALUE(literal)));
    Selector_makeOperation_1_1(this, Operator_kv3_MAKE_1singleReg_2signed43_double, dst, src);
  }
}


/**
 * Selector an absolute value into dst.
 */
void
Selector_selectMAKEAbsolute(Selector this, NativeType nativeType, Temporary dst, int64_t value)
{
  Procedure procedure = Selector_PROCEDURE(this);
  RegFile regFile = Temporary_regFile(dst);
  Temporary absolute;

  Except_CHECK(regFile == RegFile_kv3_GPR);

  /* Build value into a GRC. */
  absolute = Procedure_makeAbsoluteTemporary(procedure, (ImmediateValue)value, Relocation__UNDEF);
  Selector_selectMAKE(this, dst, absolute);
}

/**
 * Get the temporary pair (base, offset) for building a symbol+offset address.
 *
 * Currently handle:
 * - SP based
 * - FP based
 * - GP based
 * Does not handle:
 * - relocations
 */
static void
Get_Address_Base_and_Offset(Selector this, Symbol symbol, int64_t offset, Temporary *t_base, Temporary *t_offset)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Program program = Procedure_PROGRAM(procedure);
  SymbolTable symbolTable = Program_SYMBOLTABLE(program);
  // Ensure symbol is allocated
  //Allocate_Symbol(this, symbol);
  if (Symbol_store(symbol) == SymbolStore_AUTO) {
    *t_offset = Procedure_makeSymbolTemporary(procedure, symbol, offset, Relocation__UNDEF);
    /* If an FP based symbol, base register is FP else SP. */
    if (SymbolTable_getBaseSymbol(symbolTable, symbol) == Get_Symbol_FP(this)) {
      *t_base = Get_FP(this);
    } else {
      //maybe one day: Except_CHECK(SymbolTable_getBaseSymbol(symbolTable, symbol) == Get_Symbol_SP(this));
      *t_base = Get_SP(this);
    }
  } else {
    *t_offset = Procedure_makeSymbolTemporary(procedure, symbol, offset, Relocation__UNDEF);
    /* If an GP based symbol, base register is GP, else no base. */
    if (SymbolTable_getBaseSymbol(symbolTable, symbol) == Get_Symbol_GP(this)) {
      *t_base = Get_GP(this);
    } else {
      *t_base = NULL;
    }
  }
}

/**
 * Selector a symbolic address + offset into dst.
 */
void
Selector_selectMAKEAddress(Selector this, NativeType nativeType, Temporary dst, Symbol symbol, int64_t offset)
{
#if 0
  Temporary t_base, t_offset;
  Except_ENSURE(Temporary_regFile(dst) == RegFile_kv3_GR);

  Get_Address_Base_and_Offset(this, symbol, offset, &t_base, &t_offset);
  if (t_base != NULL) {
    Selector_makeOperation_1_2(this, Operator_kv3_add_1general_2general_3isrc2_simple,
                      dst, t_base, t_offset);
  } else {
    Selector_makeOperation_1_1(this, Operator_kv3_mov_1general_2xsrc2_simple, dst, t_offset);
  }
#endif
  Except_NEVER(true);
}

/**
 * Selector a direct load at address symbol+offset.
 */
void
Selector_selectDirectLOAD(Selector this, NativeType nativeType, Temporary dst, Symbol symbol, int64_t offset)
{
  Temporary t_base, t_offset;
  Except_ENSURE(Temporary_hasRegFile(dst));

  Get_Address_Base_and_Offset(this, symbol, offset, &t_base, &t_offset);
  if (t_base != NULL) {
    Selector_selectLOAD(this, nativeType, dst, t_base, t_offset);
  } else {
    Temporary t_zero = Generate_Absolute(this, RegFile_kv3_GPR, NativeType_kv3_Int8, 0);
    Selector_selectLOAD(this, nativeType, dst, t_zero, t_offset);
  }
}

/**
 * Selector a direct store at address symbol+offset.
 */
void
Selector_selectDirectSTORE(Selector this, NativeType nativeType, Temporary src, Symbol symbol, int64_t offset)
{
  Temporary t_base, t_offset;
  Except_ENSURE(Temporary_hasRegFile(src));

  Get_Address_Base_and_Offset(this, symbol, offset, &t_base, &t_offset);
  if (t_base != NULL) {
    Selector_selectSTORE(this, nativeType, src, t_base, t_offset);
  } else {
    Temporary t_zero = Generate_Absolute(this, RegFile_kv3_GPR, NativeType_kv3_Int8, 0);
    Selector_selectSTORE(this, nativeType, src, t_zero, t_offset);
  }
}

/**
 * Selector a function call.
 */
void
Selector_selectCALL(Selector this, Temporary dst)
{
/*  Selector_makeOperation_0_1(this, Operator__CALL, dst);*/
  Selector_makeOperation_1_1(this,
      Operator_kv3_CALL_1pcrel27_simple,
      Get_RA(this),
      dst);
}


/**
 * Selector a function return.
 */
void
Selector_selectRETURN(Selector this)
{
  Operation operation = Selector_makeOperation_0_1(this, Operator_kv3_RET_simple, Get_RA(this));
  *Operation__FLAGS(operation) |= OperationFlag_ExitCode;
}


/**
 * Generates code to implement a stack frame ADJUST.
 * Use a positive @c size to allocate memory on the stack, and a negative 
 * @c size to return the stack to a previous state.
 */
void
Selector_selectADJUST(Selector this, int32_t size)
{
  Temporary t_size = Procedure_makeAbsoluteTemporary(
      Selector_procedure(this),
      (ImmediateValue)size,
      Relocation__UNDEF);

  Selector_makeOperation_1_2(this,
      Operator_kv3_ADJUST,
      Get_SP(this),
      Get_SP(this),
      t_size);
}



/**
 * Selector an absolute value into a new register or get a register with this
 * value.
 */
static Temporary
Generate_Absolute(Selector this, RegFile regFile, NativeType nativeType, int64_t value)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Temporary dst;

  // No bit bucket in kvx
  // Hardwired zero reg. */
  /*  if (value == 0 && regFile == RegFile_kv3_GR)*/
  /*    return Procedure_makeDedicatedTemporary(procedure, Register_kv3_R0);*/

  Except_CHECK(regFile == RegFile_kv3_GPR);
  {
    Temporary absolute = Procedure_makeAbsoluteTemporary(procedure, (ImmediateValue)value, Relocation__UNDEF);
    Operator operator;

    if(Immediate_canEncode(Immediate_kv3_signed16, value)) {
      operator = Operator_kv3_MAKE_1singleReg_2signed16_simple;
    } else {
      Except_CHECK(Immediate_canEncode(Immediate_kv3_signed37, value));
      operator = Operator_kv3_MAKE_1singleReg_2signed43_double;
    }
    dst = Procedure_makeVirtualTemporary(procedure, RegFile_kv3_GPR);
    Selector_makeOperation_1_1(this, operator, dst, absolute);
  }
  return dst;
}


/**
 * Get a register that can be read/write
 * without side effect or generate a temporary if no such register
 * is available.
 */
static Temporary
Get_DummyReg(Selector this, RegFile regFile)
{
#if 0
  Procedure procedure = Selector_PROCEDURE(this);
  if (regFile == RegFile_kv3_GR) {
    return Procedure_makeDedicatedTemporary(procedure, Register_kv3_R0);
  }
  return Procedure_makeVirtualTemporary(procedure, regFile);
#endif
  Except_NEVER(true);
  return NULL;
}

/**
 * Get the stack pointer.
 */
static Temporary
Get_SP(Selector this)
{
  Procedure procedure = Selector_PROCEDURE(this);
  return Procedure_makeDedicatedTemporary(procedure, Register_kv3_R12);
}

/**
 * Get the frame pointer.
 */
static Temporary
Get_FP(Selector this)
{
  Procedure procedure = Selector_PROCEDURE(this);
  return Procedure_makeDedicatedTemporary(procedure, Register_kv3_R10);
}

/**
 * Get the global pointer.
 */
static Temporary
Get_GP(Selector this)
{
  Procedure procedure = Selector_PROCEDURE(this);
  return Procedure_makeDedicatedTemporary(procedure, Register_kv3_R14);
}

/**
 * Get the return adress.
 */
static Temporary
Get_RA(Selector this)
{
  Procedure procedure = Selector_PROCEDURE(this);
  return Procedure_makeDedicatedTemporary(procedure, Register_kv3_RA);
}


/**
 * Get an unique symbol for SP based block
 */
static Symbol
Get_Symbol_SP(Selector this)
{
  Procedure procedure = Selector_PROCEDURE(this);
  return Procedure_SYMBOLSP(procedure);
}

/**
 * Get an unique symbol for FP based block.
 */
static Symbol
Get_Symbol_FP(Selector this)
{
  Procedure procedure = Selector_PROCEDURE(this);
  return Procedure_SYMBOLFP(procedure);
}

/**
 * Get an unique symbol for GP based block.
 */
static Symbol
Get_Symbol_GP(Selector this)
{
  Procedure procedure = Selector_PROCEDURE(this);
  return Procedure_SYMBOLGP(procedure);
}

/**
 * Get machine operator for load of NativeType (type) into register of
 * RegFile (regFile).
 */
static Operator
get_load_operator(Selector this, RegFile regFile, NativeType nativeType,
                  Temporary offset)
{
  Operator operator;
  Literal offset_literal;

  Except_ENSURE(regFile == RegFile_kv3_GPR);

  Except_THROWIF(!Temporary_hasLiteral(offset),
      "Offset is not a literal, TODO: code for register offset!");
  offset_literal = Temporary_LITERAL(offset);

  if(regFile == RegFile_kv3_GPR) {
    if (   Temporary_isAbsolute(offset)
        && Immediate_canEncode(Immediate_kv3_signed10,
          Literal_VALUE(offset_literal))) {
      switch(nativeType) {
        case NativeType_kv3_Int8:
          operator = Operator_kv3_LBS__2singleReg_3signed10_4singleReg_simple; break;
        case NativeType_kv3_UInt8:
          operator = Operator_kv3_LBZ__2singleReg_3signed10_4singleReg_simple; break;
        case NativeType_kv3_Int16:
          operator = Operator_kv3_LHS__2singleReg_3signed10_4singleReg_simple; break;
        case NativeType_kv3_UInt16:
          operator = Operator_kv3_LHZ__2singleReg_3signed10_4singleReg_simple; break;
        case NativeType_kv3_Int32:
          operator = Operator_kv3_LWS__2singleReg_3signed10_4singleReg_simple; break;
        case NativeType_kv3_UInt32:
          operator = Operator_kv3_LWZ__2singleReg_3signed10_4singleReg_simple; break;
        default:
          operator = Operator__UNDEF;
      }
    } else {
      switch(nativeType) {
        case NativeType_kv3_Int8:
          operator = Operator_kv3_LBS__2singleReg_3signed37_4singleReg_double; break;
        case NativeType_kv3_UInt8:
          operator = Operator_kv3_LBZ__2singleReg_3signed37_4singleReg_double; break;
        case NativeType_kv3_Int16:
          operator = Operator_kv3_LHS__2singleReg_3signed37_4singleReg_double; break;
        case NativeType_kv3_UInt16:
          operator = Operator_kv3_LHZ__2singleReg_3signed37_4singleReg_double; break;
        case NativeType_kv3_Int32:
          operator = Operator_kv3_LWS__2singleReg_3signed37_4singleReg_double; break;
        case NativeType_kv3_UInt32:
          operator = Operator_kv3_LWZ__2singleReg_3signed37_4singleReg_double; break;
        default:
          operator = Operator__UNDEF;
      }
    }
  } else {
    Except_NEVER(true);  // Flo: don't know how to do
  }
  return operator;
}

/**
 * Get machine operator for store of NativeType (type)
 * from register of RegFile (regFile).
 */
static Operator
get_store_operator(Selector this, RegFile regFile, NativeType nativeType,
                   Temporary offset)
{
  Operator operator;
  Literal offset_literal;

  Except_ENSURE(regFile == RegFile_kv3_GPR);

  Except_THROWIF(!Temporary_hasLiteral(offset),
      "Offset is not a literal, TODO: code for register offset!");
  offset_literal = Temporary_LITERAL(offset);

  if(regFile == RegFile_kv3_GPR) {
    if (Temporary_isAbsolute(offset) &&
        Immediate_canEncode(Immediate_kv3_signed10,
          Literal_VALUE(offset_literal))) {
      switch(nativeType) {
        case NativeType_kv3_Int8:
        case NativeType_kv3_UInt8:
          operator = Operator_kv3_SB_1signed10_2singleReg_3singleReg_simple; break;
        case NativeType_kv3_Int16:
        case NativeType_kv3_UInt16:
          operator = Operator_kv3_SH_1signed10_2singleReg_3singleReg_simple; break;
        case NativeType_kv3_Int32:
        case NativeType_kv3_UInt32:
          operator = Operator_kv3_SW_1signed10_2singleReg_3singleReg_simple; break;
        default:
          operator = Operator__UNDEF;
      }
    }
    else {
      switch(nativeType) {
        case NativeType_kv3_Int8:
        case NativeType_kv3_UInt8:
          operator = Operator_kv3_SB_1signed37_2singleReg_3singleReg_double; break;
        case NativeType_kv3_Int16:
        case NativeType_kv3_UInt16:
          operator = Operator_kv3_SH_1signed37_2singleReg_3singleReg_double; break;
        case NativeType_kv3_Int32:
        case NativeType_kv3_UInt32:
          operator = Operator_kv3_SW_1signed37_2singleReg_3singleReg_double; break;
        default:
          operator = Operator__UNDEF;
      }
    }
  } else {
    Except_NEVER(true);  // Flo: don't know how to do
  }
  return operator;
}

/**
 * Function to generate the appropriate moves or
 * swaps such that a GR register temporary is returned in reg_src1
 * and a GR register or literal temporary is returned in new_src2
 * given a src1 and src2.
 */
static inline void
checkRegSrc1Src2(Selector this, Temporary* reg_src1, Temporary* new_src2,
                                Temporary  src1, Temporary src2)
{
  // This code relies on a decent inliner to avoid the redundant checks
  Procedure procedure = Selector_PROCEDURE(this);
  if (Temporary_hasRegFile(src1)) {
    *reg_src1 = src1;
    *new_src2 = src2;
  }
  else if (Temporary_hasRegFile(src2)) {
    *reg_src1 = src2;
    *new_src2 = src1;
  }
  else if (Temporary_isAbsolute(src1)) {
    Temporary new_temp = Temporary_ofImmediate(this, RegFile_kv3_GPR, src1);
    *reg_src1 = new_temp;
    *new_src2 = src2;
  } else Except_NEVER(true);
}

/**
 * Function that checks temporary has correct @c RegFile, or @c RegFile__UNDEF.
 *
 * In the latter case, sets the @p temporary to @p regFile.
 */
static inline void checkRegFile(Temporary temporary, RegFile regFile)
{
  // Note: Temporary_regFile Except_CHECKs that temporary has a regFile.
  if(Temporary_regFile(temporary) == RegFile__UNDEF)
    *Temporary__REGFILE(temporary) = regFile;
}


/**
 * Function to move an Immediate to a register.
 * Useful when an operator accepts only register as argument.
 */
static inline Temporary
Temporary_ofImmediate(Selector this, RegFile regFile, Temporary temporary)
{
  Procedure procedure = Selector_PROCEDURE(this);
  Operator operator;
  Literal literal;
  Temporary dst;

  Except_CHECK(regFile == RegFile_kv3_GPR);
  Except_CHECK(Temporary_isAbsolute(temporary));

  dst = Procedure_makeVirtualTemporary(procedure, RegFile_kv3_GPR);
  literal = Temporary_LITERAL(temporary);
  if (Immediate_canEncode(Immediate_kv3_signed16, Literal_VALUE(literal)))
  {
    Selector_makeOperation_1_1(this, Operator_kv3_MAKE_1singleReg_2signed16_simple, dst, temporary);
  }
  else {
    Except_CHECK(Immediate_canEncode(Immediate_kv3_signed37, Literal_VALUE(literal)));
    Selector_makeOperation_1_1(this, Operator_kv3_MAKE_1singleReg_2signed43_double, dst, temporary);
  }

  return dst;
}

