#ifdef $XCC_h
/*
 * kv3-Operator.c
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

int
Operator_require(Operator this, RegFile regFile)
{
  return 1;
}

bool
Operator_isSeqCopy(Operator this)
{
  switch (this) {
  case Operator_COPY:
  case Operator_kv3_COPYD_1singleReg_2singleReg_simple:
  case Operator_kv3_MAKE_1singleReg_2signed16_simple:
  case Operator_kv3_MAKE_1singleReg_2signed43_double:
  case Operator_kv3_MAKE_1singleReg_2wrapped64_triple:
    return true;
  default:
    break;
  }
  return false;
}

bool
Operator_isAddressAdd(Operator this)
{
  switch (this) {
  case Operator__ADD:
  case Operator_kv3_ADDD_1singleReg_2singleReg_3singleReg_simple:
  case Operator_kv3_ADDD_1singleReg_2singleReg_3signed10_simple:
  case Operator_kv3_ADDD_1singleReg_2singleReg_3signed37_double:
    return true;
  default:
    break;
  }
  return false;
}

bool
Operator_isAdjust(Operator this)
{
  return (this == Operator_kv3_ADJUST);
}

bool
Operator_isGoTo(Operator this)
{
  switch (this) {
  case Operator_GOTO:
  case Operator_LOOPTO:
  case Operator_IFTRUE:
  case Operator_IFFALSE:
  case Operator_kv3_GNUASM_CFG:
  case Operator_kv3_GOTO_1pcrel27_simple:
  case Operator_kv3_CB_WNEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_WEQZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_WLTZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_WGEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_WLEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_WGTZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_ODD_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_EVEN_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DNEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DEQZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DLTZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DGEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DLEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DGTZ_2singleReg_3pcrel17_simple:
    return true;
  default:
    break;
  }
  return false;
}

bool
Operator_isJump(Operator this)
{
  switch (this) {
  case Operator_JUMP:
  case Operator_kv3_IGOTO_1singleReg_simple:
    return true;
  default:
    break;
  }
  return false;
}

bool
Operator_isBranch(Operator this)
{
  switch (this) {
  case Operator_LOOPTO:
  case Operator_FALLTO:
  case Operator_GOTO:
  case Operator_IFTRUE:
  case Operator_IFFALSE:
  case Operator_kv3_GOTO_1pcrel27_simple:
  case Operator_kv3_CB_WNEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_WEQZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_WLTZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_WGEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_WLEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_WGTZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_ODD_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_EVEN_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DNEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DEQZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DLTZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DGEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DLEZ_2singleReg_3pcrel17_simple:
  case Operator_kv3_CB_DGTZ_2singleReg_3pcrel17_simple:
  case Operator_JUMP:
  case Operator_kv3_IGOTO_1singleReg_simple:
    return true;
  default:
    break;
  }
  return false;
}

bool
Operator_isCall(Operator this)
{
  switch (this) {
  case Operator_CALL:
  case Operator_kv3_CALL_1pcrel27_simple:
    return true;
  default:
    break;
  }
  return false;
}

bool
Operator_isLink(Operator this)
{
  switch (this) {
  case Operator_LINK:
  case Operator_kv3_ICALL_1singleReg_simple:
    return true;
  default:
    break;
  }
  return false;
}

bool
Operator_isRoutine(Operator this)
{
  switch (this) {
  case Operator_CALL:
  case Operator_kv3_CALL_1pcrel27_simple:
  case Operator_LINK:
  case Operator_kv3_ICALL_1singleReg_simple:
    return true;
  default:
    break;
  }
  return false;
}

bool
Operator_isReturn(Operator this)
{
  switch (this) {
  case Operator_RETURN:
  case Operator_kv3_RET_simple:
  case Operator_kv3_RFE_simple:
    return true;
  default:
    break;
  }
  return false;
}

bool
Operator_isGNUASM(Operator this)
{
  switch (this) {
    case Operator_kv3_GNUASM:
    case Operator_kv3_GNUASM_CFG:
      return true;
    default:
      return false;
  }
}

bool
Operator_isSimulated(Operator this)
{
  switch (this) {
    case Operator_kv3_ADJUST:
    case Operator_kv3_GETPC:
    case Operator_kv3_SET_GOTP:
    case Operator_kv3_GNUASM:
    case Operator_kv3_GNUASM_CFG:
    case Operator_kv3_MOVP:
    case Operator_kv3_COMPOSEP:
    case Operator_kv3_EXTRACTP:
    case Operator_kv3_MOVC:
    case Operator_kv3_MOVPC:
    case Operator_kv3_MOVCF:
    case Operator_kv3_MOVPCF:
    case Operator_kv3_SYSCALL:
      return true;
    default:
      return false;
  }
}

Operator
Operator_speculate(Operator this, Processor processor)
{
  return this;
}

Operator
Operator_predicatePositive(Operator this, Processor processor)
{
  switch (this) {
  case Operator_GOTO: return Operator_IFTRUE;
  // FIXME! Move to Operation.c
  default: break;
  }
  return Operator_;
}

Operator
Operator_predicateNegative(Operator this, Processor processor)
{
  switch (this) {
  case Operator_GOTO: return Operator_IFFALSE;
  // FIXME! Move to Operation.c
  default: break;
  }
  return Operator_;
}

int
Operator_predicateIndex(Operator this)
{
  switch (this) {
  case Operator_IFTRUE: return 0;
  case Operator_IFFALSE: return 0;
  // FIXME! Do something with branches
  default: break;
  }
  return -1;
}

int
Operator_predicatePolarity(Operator this)
{
  switch (this) {
  case Operator_IFTRUE: return 1;
  case Operator_IFFALSE: return -1;
  // FIXME! Do something with branches
  default: break;
  }
  return 0;
}

Operator
Operator_unpredicate(Operator this, Processor processor)
{
  switch (this) {
  case Operator_IFTRUE: return Operator_GOTO;
  case Operator_IFFALSE: return Operator_GOTO;
  // FIXME! Do something with branches
  default: break;
  }
  return Operator_;
}

Operator
Operator_complementWrite(Operator this, Processor processor, int32_t writeIndex)
{
  // FIXME! Do something with compares.
  return Operator_;
}

Operator
Operator_complementRead(Operator this, Processor processor, int32_t readIndex)
{
  if (readIndex == 0) {
    switch (this) {
    case Operator_IFTRUE: return Operator_IFFALSE;
    case Operator_IFFALSE: return Operator_IFTRUE;
    // FIXME! Do something with branches and CMOVEs
    default: break;
    }
  }
  return Operator_;
}

