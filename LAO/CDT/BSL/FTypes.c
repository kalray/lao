#ifdef $XCC_h
/*
 * !!!!	FTypes.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2007 STMicroelectronics.
 * Copyright 1995 - 1998 Commissariat a l'Energie Atomique.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/FTypes.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include <stdint.h>

typedef int flag;
typedef uint8_t bits8;
typedef int8_t sbits8;
typedef uint16_t bits16;
typedef int16_t sbits16;
typedef int16_t fract16;
typedef uint32_t bits32;
typedef int32_t sbits32;
typedef int32_t fract32;
typedef uint64_t bits64;
typedef int64_t sbits64;
typedef uint16_t flip16;
typedef uint32_t flip32;
typedef uint64_t flip64;
// Testfloat compatibility
typedef flip16 float16;
typedef flip32 float32;
typedef flip64 float64;
/*----------------------------------------------------------------------------
| Internal canonical NaN format.
*----------------------------------------------------------------------------*/
typedef struct {
    flag sign;
    bits64 high, low;
} commonNaNT;
#endif//$XCC_h

#ifdef $XCC_h
typedef enum {
  LesserThan = -1,
  Equal,
  GreaterThan,
  Unordered
} Flip_compare_t;
#endif//$XCC_h

#ifdef $XCC_h
typedef enum {
    FLIP_RN, ///< Rounding to nearest (default mode)
    FLIP_RU, ///< Rounding up
    FLIP_RD, ///< Rounding down
    FLIP_RZ,  ///< Rounding to zero
} Flip_rnd_mode_t;
/// Active rounding mode register.
extern __thread Flip_rnd_mode_t Flip_rnd_mode;
#endif//$XCC_h

__thread Flip_rnd_mode_t Flip_rnd_mode = FLIP_RN;

#ifdef $XCC_h
// EXCEPTIONS
/// Defines the possible exceptions.
typedef enum
{
  FLIP_EXPT_OK = 0, ///< No exception.
  FLIP_INVALID = 1,
  FLIP_DIV_BY_ZERO = 2,
  FLIP_OVERFLOW = 4,
  FLIP_UNDERFLOW = 8,
  FLIP_INEXACT = 16
} flip_exception_t;
/// \brief IEEE exceptions register.
extern __thread flip_exception_t Flip_status;
#endif//$XCC_h

/// Set default value in the execption register.
__thread flip_exception_t Flip_status = FLIP_EXPT_OK;

#ifdef $XCC_h
/*----------------------------------------------------------------------------
| Software IEC/IEEE floating-point underflow tininess-detection mode.
*----------------------------------------------------------------------------*/
// We currently only handle tininess before rounding.
extern __thread int8_t flip_detect_tininess;
enum {
    flip_tininess_after_rounding  = 0,
    flip_tininess_before_rounding = 1
};
#endif//$XCC_h

__thread int8_t flip_detect_tininess = flip_tininess_before_rounding;

#ifdef $XCC_h
// Wrapper for testfloat 
#define float32_add Flip32_add
#define float32_sub Flip32_sub
#define float32_mul Flip32_mul
#define float32_madd Flip32_fma
#define float32_msub Flip32_fms
#define float32_nmadd Flip32_fman
#define float32_nmsub Flip32_fmsn
//#define float32_eq Flip32_eq
//#define float32_ne Flip32_ne
//#define float32_le Flip32_le
//#define float32_gt Flip32_gt
//#define float32_lt Flip32_lt
//#define float32_ge Flip32_ge
//#define float32_div Flip32_div
//#define float32_sqrt __float32_sqrt_float32
//#define float32_recip Flip32_inv
//#define float32_square __float32_square_float32
//#define float32_rsqrt Flip32_invsqrt
#define float32_min Flip32_min
#define float32_max Flip32_max
#define int32_to_float32 Flip32_fromInt32
#define int64_to_float32 Flip32_fromInt64
#define float32_to_int32 Flip32_toInt32
#define float64_to_float32 Flip32_fromFlip64
//#define float32_to_int32_round_to_zero __int32_from_float32_rtz
//#define float32_to_int64 __int64_from_float32
//#define float32_to_int64_round_to_zero __int64_from_float32_rtz
//#define float32_round_to_int __float32_round_to_int_float32
#endif//$XCC_h

#ifdef $XCC_h
// Wrapper for testfloat 
#define float64_add Flip64_add
#define float64_sub Flip64_sub
#define float64_mul Flip64_mul
//#define float64_div Flip64_div
//#define float64_sqrt __float64_sqrt_float64
//#define float64_eq Flip64_eq
//#define float64_ne Flip64_ne
//#define float64_le Flip64_le
//#define float64_gt Flip64_gt
//#define float64_lt Flip64_lt
//#define float64_ge Flip64_ge
#define float64_min Flip64_min
#define float64_max Flip64_max
#define float32_to_float64 Flip64_fromFlip32
#define int32_to_float64 Flip64_fromInt32
#define int64_to_float64 Flip64_fromInt64
//#define float64_to_int32 __int32_from_float64
//#define float64_to_int32_round_to_zero __int32_from_float64_rtz
#define float64_to_int64 Flip64_toInt64
//#define float64_to_int64_round_to_zero __int64_from_float64_rtz
//#define float64_round_to_int __float64_round_to_int_float64
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC__h
/**
 * Initialize the FTypes module.
 */
#define FTypes_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the FTypes module.
 */
#define FTypes_FINI()
#endif//$XCC__h

#if XCC__C
static void
FTypes_TEST(void)
{
#include "BSL/FTypes_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(FTypes_TEST);
  return 0;
}
#endif

