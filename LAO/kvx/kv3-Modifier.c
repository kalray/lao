#ifdef $XCC_h
/*
 * kv3-Modifier.c
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

ModifierMember
ModifierMember_complement(ModifierMember this)
{
  switch (this) {
  // simplecond
  case ModifierMember_kv3_simplecond_NEZ: return ModifierMember_kv3_simplecond_EQZ;
  case ModifierMember_kv3_simplecond_EQZ: return ModifierMember_kv3_simplecond_NEZ;
  case ModifierMember_kv3_simplecond_LTZ: return ModifierMember_kv3_simplecond_GEZ;
  case ModifierMember_kv3_simplecond_GEZ: return ModifierMember_kv3_simplecond_LTZ;
  case ModifierMember_kv3_simplecond_LEZ: return ModifierMember_kv3_simplecond_GTZ;
  case ModifierMember_kv3_simplecond_GTZ: return ModifierMember_kv3_simplecond_LEZ;
  case ModifierMember_kv3_simplecond_ODD: return ModifierMember_kv3_simplecond_EVEN;
  case ModifierMember_kv3_simplecond_EVEN: return ModifierMember_kv3_simplecond_ODD;
  // scalarcond
  case ModifierMember_kv3_scalarcond_DNEZ: return ModifierMember_kv3_scalarcond_DEQZ;
  case ModifierMember_kv3_scalarcond_DEQZ: return ModifierMember_kv3_scalarcond_DNEZ;
  case ModifierMember_kv3_scalarcond_DLTZ: return ModifierMember_kv3_scalarcond_DGEZ;
  case ModifierMember_kv3_scalarcond_DGEZ: return ModifierMember_kv3_scalarcond_DLTZ;
  case ModifierMember_kv3_scalarcond_DLEZ: return ModifierMember_kv3_scalarcond_DGTZ;
  case ModifierMember_kv3_scalarcond_DGTZ: return ModifierMember_kv3_scalarcond_DLEZ;
  case ModifierMember_kv3_scalarcond_ODD: return ModifierMember_kv3_scalarcond_EVEN;
  case ModifierMember_kv3_scalarcond_EVEN: return ModifierMember_kv3_scalarcond_ODD;
  case ModifierMember_kv3_scalarcond_WNEZ: return ModifierMember_kv3_scalarcond_WEQZ;
  case ModifierMember_kv3_scalarcond_WEQZ: return ModifierMember_kv3_scalarcond_WNEZ;
  case ModifierMember_kv3_scalarcond_WLTZ: return ModifierMember_kv3_scalarcond_WGEZ;
  case ModifierMember_kv3_scalarcond_WGEZ: return ModifierMember_kv3_scalarcond_WLTZ;
  case ModifierMember_kv3_scalarcond_WLEZ: return ModifierMember_kv3_scalarcond_WGTZ;
  case ModifierMember_kv3_scalarcond_WGTZ: return ModifierMember_kv3_scalarcond_WLEZ;
  // comparison
  case ModifierMember_kv3_comparison_NE: return ModifierMember_kv3_comparison_EQ;
  case ModifierMember_kv3_comparison_EQ: return ModifierMember_kv3_comparison_NE;
  case ModifierMember_kv3_comparison_LT: return ModifierMember_kv3_comparison_GE;
  case ModifierMember_kv3_comparison_GE: return ModifierMember_kv3_comparison_LT;
  case ModifierMember_kv3_comparison_LE: return ModifierMember_kv3_comparison_GT;
  case ModifierMember_kv3_comparison_GT: return ModifierMember_kv3_comparison_LE;
  case ModifierMember_kv3_comparison_LTU: return ModifierMember_kv3_comparison_GEU;
  case ModifierMember_kv3_comparison_GEU: return ModifierMember_kv3_comparison_LTU;
  case ModifierMember_kv3_comparison_LEU: return ModifierMember_kv3_comparison_GTU;
  case ModifierMember_kv3_comparison_GTU: return ModifierMember_kv3_comparison_LEU;
  case ModifierMember_kv3_comparison_ALL: return ModifierMember_kv3_comparison_NALL;
  case ModifierMember_kv3_comparison_NALL: return ModifierMember_kv3_comparison_ALL;
  case ModifierMember_kv3_comparison_ANY: return ModifierMember_kv3_comparison_NONE;
  case ModifierMember_kv3_comparison_NONE: return ModifierMember_kv3_comparison_ANY;
  // floatcomp
  case ModifierMember_kv3_floatcomp_ONE: return ModifierMember_kv3_floatcomp_UEQ;
  case ModifierMember_kv3_floatcomp_UEQ: return ModifierMember_kv3_floatcomp_ONE;
  case ModifierMember_kv3_floatcomp_OEQ: return ModifierMember_kv3_floatcomp_UNE;
  case ModifierMember_kv3_floatcomp_UNE: return ModifierMember_kv3_floatcomp_OEQ;
  case ModifierMember_kv3_floatcomp_OLT: return ModifierMember_kv3_floatcomp_UGE;
  case ModifierMember_kv3_floatcomp_UGE: return ModifierMember_kv3_floatcomp_OLT;
  case ModifierMember_kv3_floatcomp_OGE: return ModifierMember_kv3_floatcomp_ULT;
  case ModifierMember_kv3_floatcomp_ULT: return ModifierMember_kv3_floatcomp_OGE;
  //
  default:
    break;
  }
  return ModifierMember_;
}

