#ifdef $XCC_h
/*
 * kv3-Operation.c
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
 * Operation_isAdditive --	Test if this Operation is additive.
 *
 * An operator OP is additive if OP(a, b) = OP(a+c, b-c).
 */
bool
Operation_isAdditive(const_Operation this, int aIndex, int bIndex)
{
  Operator operator = Operation_OPERATOR(this);
  Except_REQUIRE(aIndex >= 0 && bIndex >= 0);
  switch (operator) {
  // Add Operator(s).
  case Operator__ADD:
  case Operator_kv3_ADDD_1singleReg_2singleReg_3signed10_simple:
  case Operator_kv3_ADDD_1singleReg_2singleReg_3signed37_double:
  case Operator_kv3_ADDD_1singleReg_2singleReg_3singleReg_simple:
  case Operator_kv3_ADDW_1singleReg_2singleReg_3signed10_simple:
  case Operator_kv3_ADDW_1singleReg_2singleReg_3signed37_double:
  case Operator_kv3_ADDW_1singleReg_2singleReg_3singleReg_simple:
  // Load Operator(s).
  case Operator_kv3_LBS__2singleReg_3signed10_4singleReg_simple:
  case Operator_kv3_LBS__2singleReg_3signed37_4singleReg_double:
  case Operator_kv3_LBZ__2singleReg_3signed10_4singleReg_simple:
  case Operator_kv3_LBZ__2singleReg_3signed37_4singleReg_double:
  case Operator_kv3_LV__2vectorReg_3signed10_4singleReg_simple:
  case Operator_kv3_LV__2vectorReg_3signed37_4singleReg_double:
  case Operator_kv3_LD__2singleReg_3signed10_4singleReg_simple:
  case Operator_kv3_LD__2singleReg_3signed37_4singleReg_double:
  case Operator_kv3_LHS__2singleReg_3signed10_4singleReg_simple:
  case Operator_kv3_LHS__2singleReg_3signed37_4singleReg_double:
  case Operator_kv3_LHZ__2singleReg_3signed10_4singleReg_simple:
  case Operator_kv3_LHZ__2singleReg_3signed37_4singleReg_double:
  case Operator_kv3_LO__2quadReg_3signed10_4singleReg_simple:
  case Operator_kv3_LO__2quadReg_3signed37_4singleReg_double:
  case Operator_kv3_LQ__2pairedReg_3signed10_4singleReg_simple:
  case Operator_kv3_LQ__2pairedReg_3signed37_4singleReg_double:
  case Operator_kv3_LWS__2singleReg_3signed10_4singleReg_simple:
  case Operator_kv3_LWS__2singleReg_3signed37_4singleReg_double:
  case Operator_kv3_LWZ__2singleReg_3signed10_4singleReg_simple:
  case Operator_kv3_LWZ__2singleReg_3signed37_4singleReg_double:
  // Store Operator(s).
  case Operator_kv3_SB_1signed10_2singleReg_3singleReg_simple:
  case Operator_kv3_SB_1signed37_2singleReg_3singleReg_double:
  case Operator_kv3_SV_1signed10_2singleReg_3vectorReg_simple:
  case Operator_kv3_SV_1signed37_2singleReg_3vectorReg_double:
  case Operator_kv3_SD_1signed10_2singleReg_3singleReg_simple:
  case Operator_kv3_SD_1signed37_2singleReg_3singleReg_double:
  case Operator_kv3_SH_1signed10_2singleReg_3singleReg_simple:
  case Operator_kv3_SH_1signed37_2singleReg_3singleReg_double:
  case Operator_kv3_SO_1signed10_2singleReg_3quadReg_simple:
  case Operator_kv3_SO_1signed37_2singleReg_3quadReg_double:
  case Operator_kv3_SQ_1signed10_2singleReg_3pairedReg_simple:
  case Operator_kv3_SQ_1signed37_2singleReg_3pairedReg_double:
  case Operator_kv3_SW_1signed10_2singleReg_3singleReg_simple:
  case Operator_kv3_SW_1signed37_2singleReg_3singleReg_double:
  // Memory Operator(s).
  case Operator_kv3_DTOUCHL_1signed10_2singleReg_simple:
  case Operator_kv3_DTOUCHL_1signed37_2singleReg_double:
  case Operator_kv3_DINVALL_1signed10_2singleReg_simple:
  case Operator_kv3_DINVALL_1signed37_2singleReg_double:
  case Operator_kv3_DZEROL_1signed10_2singleReg_simple:
  case Operator_kv3_DZEROL_1signed37_2singleReg_double:
  //case Operator_kv3_ITOUCHL_1signed10_2singleReg_simple:
  //case Operator_kv3_ITOUCHL_1signed37_2singleReg_double:
  case Operator_kv3_IINVALS_1signed10_2singleReg_simple:
  case Operator_kv3_IINVALS_1signed37_2singleReg_double:
    return aIndex + bIndex == 1;
  default:
    break;
  }
  return false;
}

/*
 * Operation_isSubtractive --	Test if this Operation is subtractive.
 *
 * An operator OP is subtractive if OP(a, b) = OP(a+c, b+c).
 */
bool
Operation_isSubtractive(const_Operation this, int aIndex, int bIndex)
{
  Operator operator = Operation_OPERATOR(this);
  Except_REQUIRE(aIndex >= 0 && bIndex >= 0);
  switch (operator) {
  // Sub Operator(s).
  case Operator__SUB:
  case Operator_kv3_SBFD_1singleReg_2singleReg_3signed10_simple:
  case Operator_kv3_SBFD_1singleReg_2singleReg_3signed37_double:
  case Operator_kv3_SBFD_1singleReg_2singleReg_3singleReg_simple:
  case Operator_kv3_SBFW_1singleReg_2singleReg_3signed10_simple:
  case Operator_kv3_SBFW_1singleReg_2singleReg_3signed37_double:
  case Operator_kv3_SBFW_1singleReg_2singleReg_3singleReg_simple:
    return aIndex + bIndex == 1;
  // Compare Operator(s).
  case Operator_kv3_COMPD_NE_2singleReg_3singleReg_4signed10_simple:
  case Operator_kv3_COMPD_EQ_2singleReg_3singleReg_4signed10_simple:
  case Operator_kv3_COMPD_NE_2singleReg_3singleReg_4signed37_double:
  case Operator_kv3_COMPD_EQ_2singleReg_3singleReg_4signed37_double:
  case Operator_kv3_COMPD_NE_2singleReg_3singleReg_4singleReg_simple:
  case Operator_kv3_COMPD_EQ_2singleReg_3singleReg_4singleReg_simple:
    return aIndex + bIndex == 1;
  default:
    break;
  }
  return false;
}

/*
 * Operation_isTranslative --	Test if this Operation is translative.
 *
 * An operator OP is translative if OP(a+c, b+c) = OP(a, b) + c.
 */
bool
Operation_isTranslative(const_Operation this, int aIndex, int bIndex)
{
  Operator operator = Operation_OPERATOR(this);
  Except_REQUIRE(aIndex >= 0 && bIndex >= 0);
  switch (operator) {
    // FIXME!
  default:
    break;
  }
  return false;
}

/*
 * Operation_isAssociative --	Test if this Operation is associative.
 *
 * An operator OP is associative if OP(OP(a, b), c) = OP(a, OP(b, c)).
 */
bool
Operation_isAssociative(const_Operation this, int aIndex, int bIndex)
{
  Operator operator = Operation_OPERATOR(this);
  Except_REQUIRE(aIndex >= 0 && bIndex >= 0);
  if (Operation_isAdditive(this, aIndex, bIndex)) return true;
  switch (operator) {
    // FIXME!
  default:
    break;
  }
  return false;
}

/*
 * Operation_isCommutative --	Test if this Operation is commutative.
 *
 * An operator OP is commutative if OP(a, b) = OP(b, a).
 */
bool
Operation_isCommutative(const_Operation this, int aIndex, int bIndex)
{
  Operator operator = Operation_OPERATOR(this);
  Except_REQUIRE(aIndex >= 0 && bIndex >= 0);
  if (Operation_isAssociative(this, aIndex, bIndex)) return true;
  switch (operator) {
    // FIXME!
  default:
    break;
  }
  return false;
}

/*
 * Operation_mayComplementResult --	Check this Operation may be complemented on result.
 */
bool
Operation_mayComplementResult(const_Operation this, Processor processor, int32_t resIndex)
{
  Operator operator = Operation_OPERATOR(this);
  Operator new_operator = Operator_complementWrite(operator, processor, resIndex);
  return (new_operator != Operator__UNDEF);
}

/*
 * Operation_complementResult --	Complement this operation on the designated result.
 */
void
Operation_complementResult(Operation this, Processor processor, int32_t resIndex)
{
  Operator operator = Operation_OPERATOR(this);
  Operator new_operator = Operator_complementWrite(operator, processor, resIndex);
  *Operation__OPERATOR(this) = new_operator;
  Except_ENSURE(new_operator != Operator__UNDEF);
}

/*
 * Operation_mayComplementArgument --	Check this Operation may be complemented on argument.
 */
bool
Operation_mayComplementArgument(const_Operation this, Processor processor, int32_t argIndex)
{
  Operator operator = Operation_OPERATOR(this);
  Operator new_operator = Operator_complementRead(operator, processor, argIndex);
  return (new_operator != Operator__UNDEF);
}

/*
 * Operation_complementArgument --	Complement this operation on the designated argument.
 */
void
Operation_complementArgument(Operation this, Processor processor, int32_t argIndex)
{
  Operator operator = Operation_OPERATOR(this);
  Operator new_operator = Operator_complementRead(operator, processor, argIndex);
  *Operation__OPERATOR(this) = new_operator;
  Except_ENSURE(new_operator != Operator__UNDEF);
}

/*
 * Operation_tryInvertGoTo --	Try to invert a GoTo Operation.
 */
bool
Operation_tryInvertGoTo(Operation this, Processor processor)
{
  Operator operator = Operation_OPERATOR(this);
  Operator new_operator = Operator__UNDEF;
  switch (operator) {
  case Operator_kv3_CB_WNEZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_WEQZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_WEQZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_WNEZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_WLTZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_WGEZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_WGEZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_WLTZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_WLEZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_WGTZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_WGTZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_WLEZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_ODD_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_EVEN_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_EVEN_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_ODD_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_DNEZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_DEQZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_DEQZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_DNEZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_DLTZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_DGEZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_DGEZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_DLTZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_DLEZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_DGTZ_2singleReg_3pcrel17_simple; break;
  case Operator_kv3_CB_DGTZ_2singleReg_3pcrel17_simple: new_operator = Operator_kv3_CB_DLEZ_2singleReg_3pcrel17_simple; break;
  default:
    break;
  }
  if (new_operator != Operator__UNDEF) {
    *Operation__OPERATOR(this) = new_operator;
    return true;
  }
  return false;
}

