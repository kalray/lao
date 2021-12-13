#ifdef $XCC_h
/*
 * !!!!	Flip64.xcc
 *
 * Benjamin Orgogozo (benjamin.orgogozo@normalesup.org)
 * Benoit Dupont de Dinechin (benoit.dinechin@gmail.com)
 * Nicolas Brunie (nicolas.brunie@kalray.eu)
 *
 * Code adapted from the Arenaire FLIP library.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 *
 * This program is a partially based on SoftFloat library written by
 * JOHN HAUSER (http://www.jhauser.us/arithmetic/SoftFloat-2b/).
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup BSL
 * @brief Arithmetic support for 64-bit floating-point.
 */
#endif//$XCC_h

#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/Flip64.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include "FTypes.h"
#define _FLIP_ABS(x) ( (x) < 0 ? -(x) : (x) )
#endif//$XCC_h

 /* Creates a flip64 from its inputs.
 */
#ifdef $XCC_h
static inline flip64
Flip64_create(int sign, int16_t exp, bits64 mant) {
    return (((bits64) sign) << 63) + (((bits64) exp) << 52) + mant;
}
#endif//$XCC_h

#ifdef $XCC_h
 /* Returns the sign bitt
 */
static inline flag
Flip64_sign(flip64 a) {
    return (a >> 63);
}
#endif//$XCC_h

#ifdef $XCC_h
 /* Returns the exponent part of a
 */
static inline int16_t
Flip64_exp(flip64 a) {
    return (a >> 52) & 0x7FF;
}
#endif//$XCC_h

#ifdef $XCC_h
 /* Returns the fractional part of a
 */
static inline bits64
Flip64_frac(flip64 a) {
    return a & 0x000FFFFFFFFFFFFFLLU;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if NaN, 0 if not.
 */
static inline int
Flip64_isZero(flip64 a) {
    return (Flip64_exp(a) == 0) && (Flip64_frac(a) == 0);
}
#endif//$XCC_h


#ifdef $XCC_h
static inline int64_t
Flip64_isNaN(flip64 a) {
    return (((a << 1) >> 53) == 0x7FF) && (a << 12);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if infinity, 0 if not.
 */
static inline int
Flip64_isInf(flip64 a) {
    return (Flip64_exp(a) == 0x7FF) && (!(a << 12));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if signaling NaN, 0 if not.
 */
static inline int
Flip64_issNaN(flip64 a) {
    return Flip64_isNaN(a) && (!(Flip64_frac(a) >> 51));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if quiet NaN, 0 if not.
 */
static inline int
Flip64_isqNaN(flip64 a) {
    return Flip64_isNaN(a) && (Flip64_frac(a) >> 51);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns a quiet NaN with a given payload.
 */
static inline flip64
Flip64_quiet_NaN() {
    return Flip64_create(1, 0x7FF, 0xFFFFFFFFFFFFFULL);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns the quiet NaN corresponding to the given signaling NaN
 */
static inline flip64
Flip64_quiet_from_sNaN(flip64 nan) {
    return nan | (1ULL << 51);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns quiet NaN corresponding to a 32 bits NaN
 */
flip64
Flip64_quiet_NaN_from_flip32_NaN(flip32 nan);
#endif//$XCC_h

flip64
Flip64_quiet_NaN_from_flip32_NaN(flip32 nan) {
    return Flip64_create(Flip32_sign(nan), 0x7FF, 
        (1ULL << 51) | (1ULL << 22) | ((uint64_t) Flip32_frac(nan)));
}



/*
-------------------------------------------------------------------------------
Normalizes the subnormal single-precision floating-point value represented
by the denormalized significand `aSig'.  The normalized exponent and
significand are stored at the locations pointed to by `zExpPtr' and
`zSigPtr', respectively.
-------------------------------------------------------------------------------
*/
static void Flip32_normalizeSubnormal(bits32 aSig, int16_t *zExpPtr, bits32 *zSigPtr)
{
    int8_t shiftCount;
    shiftCount = __builtin_clz(aSig) - 8;
    *zSigPtr = aSig << shiftCount;
    *zExpPtr = 1 - shiftCount;
}

/*----------------------------------------------------------------------------
| Normalizes the subnormal double-precision floating-point value represented
| by the denormalized significand `aSig'.  The normalized exponent and
| significand are stored at the locations pointed to by `zExpPtr' and
| `zSigPtr', respectively.
*----------------------------------------------------------------------------*/
static void Flip64_normalizeSubnormal(bits64 aSig, int16_t *zExpPtr, 
                                        bits64 *zSigPtr) {
    int8_t shiftCount;

    shiftCount = Flip64_countLeadingZeros(aSig) - 11;
    *zSigPtr = aSig<<shiftCount;
    *zExpPtr = 1 - shiftCount;
}

/*----------------------------------------------------------------------------
| Multiplies `a' by `b' to obtain a 128-bit product.  The product is broken
| into two 64-bit pieces which are stored at the locations pointed to by
| `z0Ptr' and `z1Ptr'.
*----------------------------------------------------------------------------*/
static void Flip_mul64To128(bits64 a, bits64 b, bits64 *z0Ptr, bits64 *z1Ptr )
{
    bits32 aHigh, aLow, bHigh, bLow;
    bits64 z0, zMiddleA, zMiddleB, z1;

    aLow = a;
    aHigh = a>>32;
    bLow = b;
    bHigh = b>>32;
    z1 = ( (bits64) aLow ) * bLow;
    zMiddleA = ( (bits64) aLow ) * bHigh;
    zMiddleB = ( (bits64) aHigh ) * bLow;
    z0 = ( (bits64) aHigh ) * bHigh;
    zMiddleA += zMiddleB;
    z0 += ( ( (bits64) ( zMiddleA < zMiddleB ) )<<32 ) + ( zMiddleA>>32 );
    zMiddleA <<= 32;
    z1 += zMiddleA;
    z0 += ( z1 < zMiddleA );
    *z1Ptr = z1;
    *z0Ptr = z0;
}

/*----------------------------------------------------------------------------
| Takes the 128-bit fixed-point value formed by concatenating `absZ0' and
| `absZ1', with binary point between bits 63 and 64 (between the input words),
| and returns the properly rounded 64-bit integer corresponding to the input.
| If `zSign' is 1, the input is negated before being converted to an integer.
| Ordinarily, the fixed-point input is simply rounded to an integer, with
| the inexact exception raised if the input cannot be represented exactly as
| an integer.  However, if the fixed-point input is too large, the invalid
| exception is raised and the largest positive or negative integer is
| returned.
*----------------------------------------------------------------------------*/
static int64_t Flip64_roundAndPackUInt(bits64 absZ0, bits64 absZ1) {
    int8_t roundingMode;
    flag roundNearestEven, increment;
    int64_t z;

    roundingMode = Flip_rnd_mode;
    roundNearestEven = ( roundingMode == FLIP_RN );
    increment = ( (sbits64) absZ1 < 0 );
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLIP_RZ ) {
            increment = 0;
        }
        else {
            increment = ( roundingMode == FLIP_RU ) && absZ1;
        }
    }
    if ( increment ) {
        ++absZ0;
        if ( absZ0 == 0 ) goto overflow;
        absZ0 &= ~ ( ( (bits64) ( absZ1<<1 ) == 0 ) & roundNearestEven );
    }
    z = absZ0;
//    if ( z && ( ( z < 0 )) ) {
//      goto overflow;
//    }
    if ( absZ1 ) Flip_raise(FLIP_INEXACT);
    return z;

 overflow:
        Flip_raise(FLIP_INVALID);
        return 0xFFFFFFFFFFFFFFFFLL;
}



/*----------------------------------------------------------------------------
| Takes the 128-bit fixed-point value formed by concatenating `absZ0' and
| `absZ1', with binary point between bits 63 and 64 (between the input words),
| and returns the properly rounded 64-bit integer corresponding to the input.
| If `zSign' is 1, the input is negated before being converted to an integer.
| Ordinarily, the fixed-point input is simply rounded to an integer, with
| the inexact exception raised if the input cannot be represented exactly as
| an integer.  However, if the fixed-point input is too large, the invalid
| exception is raised and the largest positive or negative integer is
| returned.
*----------------------------------------------------------------------------*/
static int64_t Flip64_roundAndPackInt(flag zSign, bits64 absZ0, bits64 absZ1) {
    int8_t roundingMode;
    flag roundNearestEven, increment;
    int64_t z;

    roundingMode = Flip_rnd_mode;
    roundNearestEven = ( roundingMode == FLIP_RN );
    increment = ( (sbits64) absZ1 < 0 );
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLIP_RZ ) {
            increment = 0;
        }
        else {
            if ( zSign ) {
                increment = ( roundingMode == FLIP_RD ) && absZ1;
            }
            else {
                increment = ( roundingMode == FLIP_RU ) && absZ1;
            }
        }
    }
    if ( increment ) {
        ++absZ0;
        if ( absZ0 == 0 ) goto overflow;
        absZ0 &= ~ ( ( (bits64) ( absZ1<<1 ) == 0 ) & roundNearestEven );
    }
    z = absZ0;
    if ( zSign ) z = - z;
    if ( z && ( ( z < 0 ) ^ zSign ) ) {
 overflow:
        Flip_raise(FLIP_INVALID);
        return
              zSign ? (sbits64) 0x8000000000000000LL:0x7FFFFFFFFFFFFFFFLL;
    }
    if ( absZ1 ) Flip_raise(FLIP_INEXACT);
    return z;
}



/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and significand `zSig', and returns the proper double-precision floating-
| point value corresponding to the abstract input.  Ordinarily, the abstract
| value is simply rounded and packed into the double-precision format, with
| the inexact exception raised if the abstract input cannot be represented
| exactly.  However, if the abstract value is too large, the overflow and
| inexact exceptions are raised and an infinity or maximal finite value is
| returned.  If the abstract value is too small, the input value is rounded
| to a subnormal number, and the underflow and inexact exceptions are raised
| if the abstract input cannot be represented exactly as a subnormal double-
| precision floating-point number.
|     The input significand `zSig' has its binary point between bits 62
| and 61, which is 10 bits to the left of the usual location.  This shifted
| significand must be normalized or smaller.  If `zSig' is not normalized,
| `zExp' must be 0; in that case, the result returned is a subnormal number,
| and it must not require rounding.  In the usual case that `zSig' is
| normalized, `zExp' must be 1 less than the ``true'' floating-point exponent.
| The handling of underflow and overflow follows the IEC/IEEE Standard for
| Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
static flip64 Flip64_roundAndPackFloat( flag zSign, int16_t zExp, bits64 zSig, int16_t sticky ) {
    int8_t roundingMode;
    flag roundNearestEven;
    int16_t roundIncrement, roundBits;
    flag isTiny;

    roundingMode = Flip_rnd_mode;
    roundNearestEven = ( roundingMode == FLIP_RN );
    roundIncrement = 0x200;
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLIP_RZ ) {
            roundIncrement = 0;
        }
        else {
            roundIncrement = 0x3FF;
            if ( zSign ) {
                if ( roundingMode == FLIP_RU ) roundIncrement = 0;
            }
            else {
                if ( roundingMode == FLIP_RD ) roundIncrement = 0;
            }
        }
    }
    roundBits = (zSig + sticky) & 0x3FF;
    if ( 0x7FD <= (bits16) zExp ) {
        if (    ( 0x7FD < zExp )
             || (    ( zExp == 0x7FD )
                  && ( (sbits64) ( zSig + roundIncrement ) < 0 ) )
           ) {
            Flip_raise( FLIP_OVERFLOW | FLIP_INEXACT );
            return Flip64_create( zSign, 0x7FF, 0 ) - ( roundIncrement == 0 );
        }
        if ( zExp < 0 ) {
            isTiny =
                   ( flip_detect_tininess == flip_tininess_before_rounding )
                || ( zExp < -1 )
                || ( zSig + roundIncrement < 0x8000000000000000LL );
            zSig = Flip_shift64RightJamming( zSig, - zExp);
            zExp = 0;
            roundBits = zSig & 0x3FF;
            if ( isTiny && roundBits ) Flip_raise( FLIP_UNDERFLOW );
        }
    }
    if ( (zSig & 0x3FF) || sticky ) Flip_raise(FLIP_INEXACT);
    zSig = ( zSig + (sticky && (roundingMode == FLIP_RU)) + roundIncrement )>>10;
    zSig &= ~ ( ( ( roundBits ^ 0x200 ) == 0 ) & roundNearestEven & !sticky);
//    if ( zSig == 0 ) zExp = 0;
    return Flip64_create( zSign, zExp, zSig );
}

static float64 Flip64_normalizeRoundAndPack(flag zSign, 
                                            int16_t zExp,
                                            bits64 zSig) {
    int8_t shiftCount;
    shiftCount = Flip64_countLeadingZeros(zSig) - 1;
    return Flip64_roundAndPackFloat(zSign, zExp - shiftCount, zSig<<shiftCount, 0);
}


/*----------------------------------------------------------------------------
| Returns 1 if the double-precision floating-point value `a' is a signaling
| NaN; otherwise returns 0.
*----------------------------------------------------------------------------*/
static flag flip64_is_signaling_nan( flip64 a ) {
    return
           ( ( ( a>>51 ) & 0xFFF ) == 0xFFE )
        && ( a & 0x0007FFFFFFFFFFFFLL );
}


/*----------------------------------------------------------------------------
| Right shift a by count bits and set the LSB as a sticky flag
| of bits discarded during the shift
*----------------------------------------------------------------------------*/

#ifdef $XCC_h
bits64
Flip_ushift64RightJamming(bits64 a, int16_t count);
#endif//$XCC_h

bits64
Flip_ushift64RightJamming(bits64 a, int16_t count)
{
    if ( count == 0 ) {
        return a;
    }
    else if ( count < 64 ) {
        return ( a>>count ) | ( ( a<<( ( - count ) & 63 ) ) != 0 );
    }
    else {
        return ( a != 0 );
    }
}

#ifdef $XCC_h
sbits64
Flip_shift64RightJamming(sbits64 a, int16_t count);
#endif//$XCC_h

sbits64
Flip_shift64RightJamming(sbits64 a, int16_t count)
{
    sbits64 z;
    if ( count == 0 ) {
        z = a;
    }
    else if ( count < 64 ) {
        z = ( a>>count ) | ( ( a<<( ( - count ) & 63 ) ) != 0 );
    }
    else {
        z = ( a != 0 );
    }
    return z;
}

/*----------------------------------------------------------------------------
| Shifts the 128-bit value formed by concatenating `a0' and `a1' right by 64
| _plus_ the number of bits given in `count'.  The shifted result is at most
| 64 nonzero bits; this is stored at the location pointed to by `z0Ptr'.  The
| bits shifted off form a second 64-bit result as follows:  The _last_ bit
| shifted off is the most-significant bit of the extra result, and the other
| 63 bits of the extra result are all zero if and only if _all_but_the_last_
| bits shifted off were all zero.  This extra result is stored in the location
| pointed to by `z1Ptr'.  The value of `count' can be arbitrarily large.
|     (This routine makes more sense if `a0' and `a1' are considered to form
| a fixed-point value with binary point between `a0' and `a1'.  This fixed-
| point value is shifted right by the number of bits given in `count', and
| the integer part of the result is returned at the location pointed to by
| `z0Ptr'.  The fractional part of the result may be slightly corrupted as
| described above, and is returned at the location pointed to by `z1Ptr'.)
*----------------------------------------------------------------------------*/
static void Flip64_shift64ExtraRightJamming(
     bits64 a0, bits64 a1, int16_t count, bits64 *z0Ptr, bits64 *z1Ptr )
{
    bits64 z0, z1;
    int8_t negCount = ( - count ) & 63;

    if ( count == 0 ) {
        z1 = a1;
        z0 = a0;
    }
    else if ( count < 64 ) {
        z1 = ( a0<<negCount ) | ( a1 != 0 );
        z0 = a0>>count;
    }
    else {
        if ( count == 64 ) {
            z1 = a0 | ( a1 != 0 );
        }
        else {
            z1 = ( ( a0 | a1 ) != 0 );
        }
        z0 = 0;
    }
    *z1Ptr = z1;
    *z0Ptr = z0;
}



#ifdef $XCC_h
int8_t
Flip64_countLeadingZeros(bits64 a);
#endif//$XCC_h

int8_t
Flip64_countLeadingZeros(bits64 a) {
    return __builtin_clzll(a);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
static inline flip64
Flip64_neg(flip64 x) { 
    return 0x8000000000000000LLU ^ x;
}
#endif//$XCC_h


#ifdef $XCC_h
 /* Compares 2 float
  * Returns:
  * unorderd, if at least one is NaN
  * lesserThan, if a < b
  * equal, if both or equal (-0 == +0)
  * greaterThan, if a > b
 */
Flip_compare_t
Flip64_comp(flip64 a, flip64 b);
#endif//$XCC_h

Flip_compare_t
Flip64_comp(flip64 a, flip64 b) {
    // If at least one is a NaN
    if (Flip64_isNaN(a) || Flip64_isNaN(b)) {
        return Unordered;
    }
    else {
        // If exactly one is negative
        if (Flip64_sign(a) != Flip64_sign(b)) {
            // Handle -0/+0 comparison
            if ((Flip64_exp(a) == 0) &&
                (Flip64_frac(a) == 0) &&
                (Flip64_exp(b) == 0) &&
                (Flip64_frac(b) == 0)) {
                return Equal;
            }
            else {
                if (Flip64_sign(a) > Flip64_sign(b)) {
                    return LesserThan;
                }
                else {
                    return GreaterThan;
                }
            }
        }
        else {
            if (a > b) {
                // If both (here either both are positive or negative)
                // are negative, we must reverse the result.
                if (Flip64_sign(a)) {
                    return LesserThan;
                }
                else {
                    return GreaterThan;
                }
            }
            else {
                if (a < b) {
                    // If both (here either both are positive or
                    // negative) are negative, we must reverse the
                    // result.
                    if (Flip64_sign(a)) {
                        return GreaterThan;
                    }
                    else {
                        return LesserThan;
                    }
                }
                else {
                    return Equal;
                }
            }
        }
    }
}


static bits64
Flip64_roundAndNormMant( bits64 a) {
    flag R, C, P,I;

    R = a & 0x200LL;
    if (R) {
      P = a & 0x400LL;
      C = a & 0x1ffLL;
      if (P||C) return  ( a >> 10 ) + 0x1LL;
    }
    return a >> 10;
}

static bits64
Flip64_roundAndNormMant32( bits64 a) {
    flag R, C, P,I;

    R = a & 0x40;
    if (R) {
      P = a & 0x80;
      C = a & 0x3f;
      if (P||C) return  ( a >> 7 ) + 1;
    }
    return a >> 7;
}


#ifdef $XCC_h
// Returns the absolute value of the input
static inline sbits64
Flip64_abs(sbits64 a) {
    return 0x7FFFFFFFFFFFFFFFLLU & a;
}
#endif//$XCC_h

#ifdef $XCC_h
/** 
 * Returns the min between two floats.
 */
flip64
Flip64_min(flip64 a, flip64 b);
#endif//$XCC_h

flip64
Flip64_min(flip64 a, flip64 b) {
    if (Flip64_isNaN(a)) {
        // Fixes #1557, we don't raise invalid anymore so we always
        // return the signaling NaN if any.
        if (Flip64_issNaN(a)) {
            return a;
        }
        if (Flip64_issNaN(b)) {
            return b;
        }
        if (Flip64_isNaN(b)) {
            return a;
        }
        else {
            return b;
        }
    }
    if (Flip64_isNaN(b)) {
        if (Flip64_issNaN(b)) {
            return b;
        }
        else {
            return a;
        }
    }
    // Issue #1329
    // We return -0 if comparison between two opposite 0.
    if (Flip64_isZero(a) && Flip64_isZero(b)) {
        if (Flip64_sign(a)) return a;
        else return b;
    }

    return LesserThan==Flip64_comp(a, b)? a: b;
}



#ifdef $XCC_h
/** 
 * Returns the max between two floats.
 */
flip64
Flip64_max(flip64 a, flip64 b);
#endif//$XCC_h

flip64
Flip64_max(flip64 a, flip64 b) {
    if (Flip64_isNaN(a)) {
        // Fixes #1557, we don't raise invalid anymore so we always
        // return the signaling NaN if any.
        if (Flip64_issNaN(a)) {
            return a;
        }
        if (Flip64_issNaN(b)) {
            return b;
        }
        if (Flip64_isNaN(b)) {
            return a;
        }
        else {
            return b;
        }
    }
    if (Flip64_isNaN(b)) {
        if (Flip64_issNaN(b)) {
            return b;
        }
        else {
            return a;
        }
    }

    // Issue #1329
    // We return 0 if comparison between two opposite 0.
    if (Flip64_isZero(a) && Flip64_isZero(b)) {
        if (Flip64_sign(a)) return b;
        else return a;
    }

    return GreaterThan==Flip64_comp(a, b)? a: b;
}


/*----------------------------------------------------------------------------
| Returns the result of adding the absolute values of the double-precision
| floating-point values `a' and `b'.  If `zSign' is 1, the sum is negated
| before being returned.  `zSign' is ignored if the result is a NaN.
| The addition is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
| This function does not handle the case when a or b are NaN.
*----------------------------------------------------------------------------*/
static flip64 Flip64_addFloatSigs(flip64 a, flip64 b, flag zSign )
{
    int16_t aExp, bExp, zExp;
    bits64 aSig, bSig, zSig;
    int16_t expDiff;

    aSig = Flip64_frac( a );
    aExp = Flip64_exp( a );
    bSig = Flip64_frac( b );
    bExp = Flip64_exp( b );
    expDiff = aExp - bExp;
    aSig <<= 9;
    bSig <<= 9;




    if ( 0 < expDiff ) {
        if ( aExp == 0x7FF ) {
            // Don't check case when a is NaN: done is the calling
            // function.
            return a;
        }
        if ( bExp == 0 ) {
            --expDiff;
        }
        else {
            bSig |= 0x2000000000000000LL;
        }
        bSig = Flip_shift64RightJamming(bSig, expDiff);
        zExp = aExp;
    }
    else if ( expDiff < 0 ) {
        if ( bExp == 0x7FF ) {
            // Don't check case when b is NaN: done is the calling
            // function.
            return Flip64_create( zSign, 0x7FF, 0 );
        }
        if ( aExp == 0 ) {
            ++expDiff;
        }
        else {
            aSig |= 0x2000000000000000LL;
        }
        aSig = Flip_shift64RightJamming(aSig, -expDiff);
        zExp = bExp;
    }
    else {
        if ( aExp == 0x7FF ) {
            if ( aSig | bSig ) return Flip64_quiet_NaN();
            return a;
        }
        if ( aExp == 0 ) return Flip64_create(zSign, 0, (aSig + bSig) >> 9);
        zSig = 0x4000000000000000LL + aSig + bSig;
        zExp = aExp;
        goto roundAndPack;
    }
    aSig |= 0x2000000000000000LL;
    zSig = (aSig + bSig) << 1;
    --zExp;
    if ( (sbits64) zSig < 0 ) {
        zSig = aSig + bSig;
        ++zExp;
    }
 roundAndPack:
    return Flip64_roundAndPackFloat(zSign, zExp, zSig, 0);
}

/*----------------------------------------------------------------------------
| Returns the result of subtracting the absolute values of the double-
| precision floating-point values `a' and `b'.  If `zSign' is 1, the
| difference is negated before being returned.  `zSign' is ignored if the
| result is a NaN.  The subtraction is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic.
| This function does not handle the case when a or b are NaN.
*----------------------------------------------------------------------------*/
static flip64 Flip64_subFloatSigs(flip64 a, flip64 b, flag zSign )
{
    int16_t aExp, bExp, zExp;
    bits64 aSig, bSig, zSig;
    int16_t expDiff;

    aSig = Flip64_frac( a );
    aExp = Flip64_exp( a );
    bSig = Flip64_frac( b );
    bExp = Flip64_exp( b );
    expDiff = aExp - bExp;
    aSig <<= 10;
    bSig <<= 10;
    if ( 0 < expDiff ) goto aExpBigger;
    if ( expDiff < 0 ) goto bExpBigger;
    if ( aExp == 0x7FF ) {
        // Don't check case when a or b are NaN: done is the calling
        // function.
        Flip_raise(FLIP_INVALID);
        return Flip64_quiet_NaN();
    }
    if ( aExp == 0 ) {
        aExp = 1;
        bExp = 1;
    }
    if ( bSig < aSig ) goto aBigger;
    if ( aSig < bSig ) goto bBigger;
    return Flip64_create(Flip_rnd_mode == FLIP_RD, 0, 0);
 bExpBigger:
    if ( bExp == 0x7FF ) {
        // Don't check case when b is NaN: done is the calling
        // function.
        return Flip64_create(zSign ^ 1, 0x7FF, 0);
    }
    if ( aExp == 0 ) {
        ++expDiff;
    }
    else {
        aSig |= 0x4000000000000000LL;
    }
    aSig = Flip_shift64RightJamming(aSig, -expDiff);
    bSig |= 0x4000000000000000LL;
 bBigger:
    zSig = bSig - aSig;
    zExp = bExp;
    zSign ^= 1;
    goto normalizeRoundAndPack;
 aExpBigger:
    if ( aExp == 0x7FF ) {
        // Don't check case when a is NaN: done is the calling
        // function.
        return a;
    }
    if ( bExp == 0 ) {
        --expDiff;
    }
    else {
        bSig |= 0x4000000000000000LL;
    }
    bSig = Flip_shift64RightJamming(bSig, expDiff);
    aSig |= 0x4000000000000000LL;
 aBigger:
    zSig = aSig - bSig;
    zExp = aExp;
 normalizeRoundAndPack:
    --zExp;
    return Flip64_normalizeRoundAndPack(zSign, zExp, zSig);
}



#ifdef $XCC_h
flip64
Flip64_add(flip64 a, flip64 b);
#endif//$XCC_h

flip64
Flip64_add(flip64 a, flip64 b) { 
    flag aSign, bSign;

    aSign = Flip64_sign( a );
    bSign = Flip64_sign( b );

    // If +infty -infty we generate an invalid exception)
    if (Flip64_isInf(a) && Flip64_isInf(b) && 
        (Flip64_sign(a) ^ Flip64_sign(b))) {
        Flip_raise(FLIP_INVALID);
    }

    if (Flip64_issNaN(a) || Flip64_issNaN(b)) Flip_raise(FLIP_INVALID);
    if (Flip64_isNaN(a)) return Flip64_quiet_from_sNaN(a);
    if (Flip64_isNaN(b)) return Flip64_quiet_from_sNaN(b);

    if ( aSign == bSign ) {
        return Flip64_addFloatSigs( a, b, aSign );
    }
    else {
        return Flip64_subFloatSigs(a, b, aSign);
    }
}


#ifdef $XCC_h
// Converts a flip32 to a flip64.
flip64
Flip64_fromFlip32(flip32 r0);
#endif//$XCC_h

flip64
Flip64_fromFlip32(flip32 r0) {
    flag r0Sign;
    int16_t r0Exp;
    bits32 r0Mant;
    r0Mant = Flip32_frac(r0);
    r0Exp = Flip32_exp(r0);
    r0Sign = Flip32_sign(r0);
    if (Flip32_issNaN(r0)) {
        Flip_raise(FLIP_INVALID);
    }
    if (Flip32_isNaN(r0)) {
        return Flip64_quiet_NaN_from_flip32_NaN(r0);
    }
    if (r0Exp == 0xFF) {
      return Flip64_create(r0Sign, 0x7FF, 0);
    }
    if (r0Exp == 0) {
      if (r0Mant == 0) return Flip64_create( r0Sign, 0, 0);
      Flip32_normalizeSubnormal(r0Mant, &r0Exp, &r0Mant);
      --r0Exp;
    }
    return Flip64_create(r0Sign, r0Exp + 896, ((bits64) r0Mant) << 29);
}

#ifdef $XCC_h
// Converts an int32_t to a flip64.
flip64
Flip64_fromInt32(int32_t r0);
#endif//$XCC_h

flip64
Flip64_fromInt32(int32_t r0) {
  int roSign;
  bits32 r0Mant;
  int8_t shift;
  bits64 roMant;
  int16_t roExp;
  roExp = 1053;
  if ( r0 == 0 ) return 0 ;
  if ( r0 == 0x80000000 ) return Flip64_create( 1, 1054, 0 );
  roSign = ( r0 < 0 ) ;
  r0Mant = roSign ? -r0 : r0 ;
  shift = __builtin_clz( r0Mant ) - 1; 
  roMant = (bits64) r0Mant;
  roMant <<= 22+shift;
  roMant &= 0x000fffffffffffffLL;
  return Flip64_create(roSign, roExp - shift, roMant);
}

#ifdef $XCC_h
// Converts an uint64_t to a flip64.
flip64
Flip64_fromUint64(uint64_t r0);
#endif//$XCC_h

flip64 Flip64_fromUint64(uint64_t r0) {
    int8_t shiftCount;
    if (r0 == 0) {return Flip64_create(0, 0, 0);}
    if (r0 == 0x8000000000000000LL) {return Flip64_create(0, 0x43E, 0);}
    shiftCount = Flip64_countLeadingZeros(r0);
    if (shiftCount == 0) {
        return Flip64_roundAndPackFloat(0,  0x43E, (r0 & 0x7FFFFFFFFFFFFFFFLL) >> 1, r0 & 1);
    }
    else {
        return Flip64_roundAndPackFloat(0,  0x43C - (shiftCount - 1), r0 << (shiftCount - 1), 0);
    }
}



#ifdef $XCC_h
// Converts an int64_t to a flip64.
flip64
Flip64_fromInt64(int64_t r0);
#endif//$XCC_h

flip64
Flip64_fromInt64(int64_t r0) {
   flag zSign;

    if ( r0 == 0 ) return 0;
    if ( r0 == (sbits64) 0x8000000000000000LL ) {
        return Flip64_create( 1, 0x43E, 0 );
    }
    zSign = ( r0 < 0 );
    return Flip64_normalizeRoundAndPack(zSign, 0x43C, zSign ?-r0:r0);

}

#ifdef $XCC_h
// Converts an int64_t to a flip64.
uint64_t
Flip64_toUint64(flip64 a);
#endif//$XCC_h

uint64_t
Flip64_toUint64(flip64 a) {
    flag aSign;
    int16_t aExp, shiftCount;
    bits64 aSig, aSigExtra;

    aSig = Flip64_frac( a );
    aExp = Flip64_exp( a );
    aSign = Flip64_sign( a );
    if ( aExp ) aSig |= 0x0010000000000000LL;
    shiftCount = 0x433 - aExp;
    if ( shiftCount <= 0 ) {
        if ( 0x43E < aExp ) {
            Flip_raise(FLIP_INVALID);
//            if ((    ( aExp == 0x7FF )
//                      && ( aSig != 0x0010000000000000LL ) )
//               ) {
                return 0xFFFFFFFFFFFFFFFFLL;
//            }
        }
        aSigExtra = 0;
        aSig <<= - shiftCount;
    }
    else {
        Flip64_shift64ExtraRightJamming(aSig, 0, shiftCount, &aSig, &aSigExtra);
    }
    return Flip64_roundAndPackUInt(aSig, aSigExtra);
}

#ifdef $XCC_h
// Converts an int64_t to a flip64.
int64_t
Flip64_toInt64(flip64 a);
#endif//$XCC_h

int64_t
Flip64_toInt64(flip64 a) {
    flag aSign;
    int16_t aExp, shiftCount;
    bits64 aSig, aSigExtra;

    aSig = Flip64_frac( a );
    aExp = Flip64_exp( a );
    aSign = Flip64_sign( a );
    if ( aExp ) aSig |= 0x0010000000000000LL;
    shiftCount = 0x433 - aExp;
    if ( shiftCount <= 0 ) {
        if ( 0x43E < aExp ) {
            Flip_raise(FLIP_INVALID);
            if (    ! aSign
                 || (    ( aExp == 0x7FF )
                      && ( aSig != 0x0010000000000000LL ) )
               ) {
                return 0x7FFFFFFFFFFFFFFFLL;
            }
            return (sbits64) 0x8000000000000000LL;
        }
        aSigExtra = 0;
        aSig <<= - shiftCount;
    }
    else {
        Flip64_shift64ExtraRightJamming(aSig, 0, shiftCount, &aSig, &aSigExtra);
    }
    return Flip64_roundAndPackInt(aSign, aSig, aSigExtra);
}

// Used by Arenaire FPFMA_c implementation.
/** use for unpacking operands */
static void unpack64(uint64_t op, uint32_t* sgn, uint64_t* mant, uint32_t* exp, uint32_t* isNormal, uint32_t* isNaN, uint32_t* isInf, uint32_t* isZero) {
    *sgn = op >> 63;
    *exp = (op & 0x7FF0000000000000LL) >> 52;
    *mant = (op << 12) >> 12;
    *isInf = (*exp == 0x7ff) && (*mant == 0);
    *isNaN = (*exp == 0x7ff) && !(*isInf);
    *isNormal = (*exp != 0x7ff) && (*exp != 0x00);
    *isZero = (*exp == 0x00) && (*mant == 0);
}

// Used by Arenaire FPFMA_c implementation.
/* leading zero counter */
static int LZC64(uint64_t op, int count, int* correction) {
    // we made a leading zero count on op[count -- 1], with correction
    for (int i = 1; i <= count;i++) {
        uint64_t tmp = op >> (count - i);
        if (tmp != 0) {
            return (*correction) = i-1;
        }
    }
    return -1;
}

/**
 * Implementation of FMA provided by Arenaire.
 * We don't use it directly outside LAO but through wrappers (fma, fmn,
 * ..)
 */
static
flip64 FPFMA_c(uint32_t A, uint32_t B, uint64_t C, Flip_rnd_mode_t roundMode, 
                uint32_t negateAB, uint32_t negateC) {
	uint32_t sgnA, sgnB, sgnC;
	uint32_t mantA, mantB;
  uint64_t mantC;
	uint32_t expA, expB, expC;
	// flags
	uint32_t AisNormal, AisNaN, AisInf, AisZero;
	uint32_t BisNormal, BisNaN, BisInf, BisZero;
	uint32_t CisNormal, CisNaN, CisInf, CisZero;

    uint32_t effectiveOp;
    uint32_t infSgn;

    int correctExpA, correctExpB, correctExpC;
    int ea, eb, ec;

    uint64_t ma, mb, mult;
    int eMult;

	uint64_t resultSgn; // TODO : to be defined
	int diff, expOrigin = 0;

    Int128_ tmpC;
	Int128_ sum;

    uint64_t sgnMask, sumSgn, sumPreCarry;
	int lzcSUM;
	Int128_ normResult;
	int newExp;
	uint32_t prodSgn;

	Int128_ preResult_128, preRoundBit, preStickyBit;
	uint64_t preResult;
    uint32_t roundBit, stickyBit;

	// TODO : determine if result is subnormal or not and delete implicit one
	uint64_t uNewExp;
	bool infByOverflow;

	Int128_ alignedAB;

	int biasedResultExp;
	Int128_ delCarryMask;
	Int128_ shiftedC;

	uint64_t preRoundedResult, roundedResult;
    uint32_t lastBitOdd;
    int normShiftC, normShiftAB;

    uNewExp = 0;
    roundedResult = 0;

	// unpacking and setting flags
	unpack32(A, &sgnA, &mantA, &expA, &AisNormal, &AisNaN, &AisInf, &AisZero);
	unpack32(B, &sgnB, &mantB, &expB, &BisNormal, &BisNaN, &BisInf, &BisZero);
	unpack64(C, &sgnC, &mantC, &expC, &CisNormal, &CisNaN, &CisInf, &CisZero);

	// effective operation : addition (0) or substraction (1)
	effectiveOp = (((negateAB ^ sgnA) ^ sgnB) ^ (sgnC ^ negateC)); 

    /////////////////////////////////
    //  NAN HANDLING
    /////////////////////////////////
    // If one of the operand is a signaling NaN, we generate an
    // exception.
    if (Flip32_issNaN(A) || Flip32_issNaN(B) || Flip64_issNaN(C)) {
        Flip_raise(FLIP_INVALID);
    }
    // If one of the operand is already a NaN, we return a quiet NaN
    // (same sign, same payload).
    if (AisNaN) return Flip64_quiet_NaN_from_flip32_NaN(A);
    if (BisNaN) return Flip64_quiet_NaN_from_flip32_NaN(B);
    if (CisNaN) return Flip64_quiet_from_sNaN(C);

    // We check if we *generate* NaN
	if ((((AisInf || BisInf) && CisInf && effectiveOp)) || 
        (AisZero && BisInf) || (BisZero && AisInf)) {
        Flip_raise(FLIP_INVALID);
        return Flip64_quiet_NaN();
	}
    /////////////////////////////////

    /////////////////////////////////
    // INFINITY
    /////////////////////////////////
    // In NaN handling above we checked the case when C is also infinity
    // (which could generate NaN).
    if (AisInf || BisInf) {
		// inf in mult
		infSgn = sgnA ^ sgnB ^ negateAB;
        return Flip64_create(infSgn, 0x7FF, 0);
	} 
    else if (CisInf) {
		// inf in addend
		infSgn = sgnC ^  negateC;
        return Flip64_create(infSgn, 0x7FF, 0);
	}
    /////////////////////////////////

	correctExpA = 0;
	correctExpB = 0;
	correctExpC = 0;
	// computing real mantissa by adding a leading one if operand was normal
	// and shift left (with exponent correction) if operand was subnormal
	mantA = AisNormal?(1<<23)|mantA:(mantA<<LZC32(mantA, 23, &correctExpA));
	mantB = BisNormal?(1<<23)|mantB:(mantB<<LZC32(mantB, 23, &correctExpB));
	mantC = CisNormal?(1LL<<52)|mantC:(mantC<<LZC64(mantC, 52, &correctExpC));
	// exponent correction
	ea =  AisNormal ? expA - 127 : - 126 - correctExpA;
	eb =  BisNormal ? expB - 127 : - 126 - correctExpB;
	ec =  CisNormal ? expC - 1023 : - 1022 - correctExpC;

	ma = mantA;
	mb = mantB;
	mult = ma * mb; 
	eMult = ea + eb;

	// correction of Emult if A or B is zero
	if (AisZero || BisZero) eMult = -2048;

	// SIGN COMPUTATION
	resultSgn = sgnC ^ negateC;

	// exponent diff computation
	// WORK IN PROGRESS
    diff = ec-eMult;
	expOrigin = 0;

    // We completely shift C to the left with a margin of 10 bits.
	if (diff >= 0) {
        expOrigin = ec + 10;
        normShiftC = 65;
        normShiftAB = (diff > 70)?0:(71-diff);
	}
    // We completely shift AB to the left with a margin of 10 bits.
    else {
        expOrigin = eMult + 11;
        normShiftAB = 70;
        normShiftC = (diff <= -65)?0:64+diff;
	};
    tmpC = Int128_fromUInt64(mantC);
    shiftedC = Int128_shl(tmpC, Int128_fromUInt64(normShiftC));
	alignedAB = Int128_shl(Int128_fromUInt64(mult), 
                    Int128_fromUInt64(normShiftAB));

	if (effectiveOp == 0) sum = Int128_add(shiftedC,alignedAB);
	else sum = Int128_sub(shiftedC, alignedAB);

	// absolute value

	if (effectiveOp != 0) {
		if (sum.dwords[1] >> 63) {
			resultSgn = !resultSgn;
        }
        sum = Int128_abs(sum);
	}

	lzcSUM = Int128_clz(sum, 128);

	prodSgn = sgnA ^ sgnB ^ negateAB;
	// Managing zero cases
	if (lzcSUM == 128) {
		// result is zero
		newExp = -1023;
		normResult = Int128_fromUInt64(0);
		// rounding toward minus infinity
		if (roundMode == FLIP_RD) {
			// addition case 
			resultSgn = (!prodSgn) && (!(sgnC ^ negateC)) ? 0 : 1;
			// substraction case
			//else resultSgn = (!prodSgn) && (sgnC ^ negateC) ? 0 : 1;
		} else {
			// other rounding mode
			resultSgn = (prodSgn && (sgnC ^ negateC)) ? 1 : 0;
		}
		// setting sign for different rounding mode
	} else {
		normResult = Int128_shl(sum, Int128_fromUInt64(lzcSUM));
		//-- old -- newExp = eMult + (54 - lzcSUM);
		newExp = expOrigin - lzcSUM;
		// we have to treat subnormal here, in order to manage correctly round and sticky bit
		if (newExp + 1023 <= 0) {
			// subnormal case
            // We don't shift too much: we don't want to looze sticky
            // bits for rouding. 60 bits is sufficient here.
            int shift = -(newExp + 1023)+1>60?60:-(newExp+1023)+1;
			normResult = Int128_shru(normResult, Int128_fromUInt64(shift));
		}
	}
	preResult_128 = Int128_shru(normResult, Int128_fromUInt64(128-53));
	preResult = Int128_toUInt64(preResult_128);


	preRoundBit = Int128_shru(Int128_shl(normResult, Int128_fromUInt64(53)), 
                    Int128_fromUInt64(127));
	roundBit = Int128_toUInt32(preRoundBit);


	preStickyBit = Int128_shl(normResult, Int128_fromUInt64(54));
	stickyBit = !((preStickyBit.dwords[0] == 0) & (preStickyBit.dwords[1] == 0));

    if (stickyBit || roundBit) Flip_raise(FLIP_INEXACT);
	// TODO : determine if result is subnormal or not and delete implicit one
	infByOverflow = false;

	biasedResultExp = newExp + 1023;

    if (flip_detect_tininess ==  flip_tininess_before_rounding) {
        if (biasedResultExp <= 0) {
            // We raise flags only if the result is not exactly 0 and
            // is not exact.
            if ((((preResult_128.dwords[0] | preResult_128.dwords[1]) || 
                (biasedResultExp < -1)) && (roundBit || stickyBit)) ||
                // Case when preResult_128 0 but normResult != 0
                (!(preResult_128.dwords[0] | preResult_128.dwords[1]) && 
                  (normResult.dwords[0] | normResult.dwords[1])))
                {
                Flip_raise(FLIP_UNDERFLOW | FLIP_INEXACT);
            }
        }
    }

	if (biasedResultExp <= 0) {
		uNewExp = 0;
		// exp for subnormal is 126 not 127 so +1
		//preResult = preResult >> (-biasedResultExp + 1);
	} else if (biasedResultExp >= 2047) {
		// overflow
		if ((roundMode == FLIP_RD && !resultSgn) || (roundMode == FLIP_RU && resultSgn) || (roundMode == FLIP_RZ)) {
            Flip_raise(FLIP_OVERFLOW | FLIP_INEXACT);
			uNewExp = 0x7fe;
			preResult = 0xfffffffffffffLL;
		} else {
            infByOverflow = true;
            uNewExp = 0x7FF;
            preResult = 0LL;
        }
	} else {
		// delete first one
		preResult = preResult & 0xfffffffffffffLL;
		uNewExp = biasedResultExp;
	}

	preRoundedResult = (resultSgn << 63) | (uNewExp << 52) | (preResult);
	lastBitOdd = preRoundedResult & 1;
    if (!infByOverflow) {
        // ROUNDING 
        switch(roundMode) {
            case FLIP_RN:
                if (roundBit && stickyBit) roundedResult = preRoundedResult+1;
                else if (roundBit && lastBitOdd) roundedResult = preRoundedResult+1;
                else roundedResult = preRoundedResult;
                break;
            case FLIP_RU:
                if (!resultSgn && (roundBit || stickyBit)) {
                    roundedResult = preRoundedResult+1;   
                    if (Flip64_isInf(roundedResult)) Flip_raise(FLIP_OVERFLOW);
                }
                // negative
                else roundedResult = preRoundedResult; 
                break;
            case FLIP_RD:
                if (resultSgn && (roundBit || stickyBit)) roundedResult = preRoundedResult+1;
                // negative
                else roundedResult = preRoundedResult; 
                break;
            case FLIP_RZ:
                roundedResult = preRoundedResult;
                break;
            default:
                break;
        };
    }
    else {
        roundedResult = preRoundedResult;
    }

    if (Flip64_isInf(roundedResult)) Flip_raise(FLIP_OVERFLOW | FLIP_INEXACT);

    return Flip64_create(roundedResult >> 63, 
                        (roundedResult & 0x7FF0000000000000LL) >> 52,
                        roundedResult & 0xFFFFFFFFFFFFFLL);
}

/**
 * Implementation of FDMA
 * We don't use it directly outside LAO but through wrappers (fdma, fdms)
 */
static
flip64 FPFDMA_c(uint32_t A, uint32_t B, uint32_t C, uint32_t D, 
                Flip_rnd_mode_t roundMode, uint32_t negateCD) {
	uint32_t sgnA, sgnB, sgnC, sgnD;
	uint32_t mantA, mantB, mantC, mantD;
	uint32_t expA, expB, expC, expD;
	// flags
	uint32_t AisNormal, AisNaN, AisInf, AisZero;
	uint32_t BisNormal, BisNaN, BisInf, BisZero;
	uint32_t CisNormal, CisNaN, CisInf, CisZero;
	uint32_t DisNormal, DisNaN, DisInf, DisZero;

    uint32_t effectiveOp;
    uint32_t infSgn;

    int correctExpA, correctExpB, correctExpC, correctExpD;
    int ea, eb, ec, ed;

    uint64_t multAB, multCD;
    int eMultAB, eMultCD;

	uint64_t resultSgn; // TODO : to be defined
	int diff, expOrigin = 0;

    Int128_ tmpCD;
	Int128_ sum;

	int lzcSUM;
	Int128_ normResult;
	int newExp;
	uint32_t prodSgnAB;
	uint32_t prodSgnCD;

	Int128_ preResult_128, preRoundBit, preStickyBit;
	uint64_t preResult;
	uint32_t roundBit, stickyBit;

	// TODO : determine if result is subnormal or not and delete implicit one
	uint64_t uNewExp;
	bool infByOverflow;

	Int128_ alignedAB;
	Int128_ alignedCD;
    int normShiftCD, normShiftAB;

	int biasedResultExp;

	uint64_t preRoundedResult, roundedResult;
    uint32_t lastBitOdd;

    uNewExp = 0;
    roundedResult = 0;

	// unpacking and setting flags
	unpack32(A, &sgnA, &mantA, &expA, &AisNormal, &AisNaN, &AisInf, &AisZero);
	unpack32(B, &sgnB, &mantB, &expB, &BisNormal, &BisNaN, &BisInf, &BisZero);
	unpack32(C, &sgnC, &mantC, &expC, &CisNormal, &CisNaN, &CisInf, &CisZero);
	unpack32(D, &sgnD, &mantD, &expD, &DisNormal, &DisNaN, &DisInf, &DisZero);

	// effective operation : addition (0) or substraction (1)
	effectiveOp = ((sgnC ^ sgnD) ^ negateCD);

    /////////////////////////////////
    //  NAN HANDLING
    /////////////////////////////////
    // If one of the operand is a signaling NaN, we generate an
    // exception.
    if (Flip32_issNaN(A) || Flip32_issNaN(B) || 
        Flip32_issNaN(C) || Flip32_issNaN(D)) {
        Flip_raise(FLIP_INVALID);
    }
    // If one of the operand is already a NaN, we return a quiet NaN
    // (same sign, same payload).
    if (AisNaN) return Flip64_quiet_NaN_from_flip32_NaN(A);
    if (BisNaN) return Flip64_quiet_NaN_from_flip32_NaN(B);
    if (CisNaN) return Flip64_quiet_NaN_from_flip32_NaN(C);
    if (DisNaN) return Flip64_quiet_NaN_from_flip32_NaN(D);

    // We check if we *generate* NaN
	if (((AisInf||BisInf) && (CisInf||DisInf) && (effectiveOp^sgnA^sgnB)) ||
        (AisZero && BisInf) || (BisZero && AisInf) ||
        (CisZero && DisInf) || (DisZero && CisInf)) {
        Flip_raise(FLIP_INVALID);
        return Flip64_quiet_NaN();
    }
    /////////////////////////////////

    /////////////////////////////////
    // INFINITY
    /////////////////////////////////
    // In NaN handling above we checked the case when C or D is also infinity
    // (which could generate NaN).
    if (AisInf || BisInf) {
		// inf in mult
		infSgn = sgnA ^ sgnB;
        return Flip64_create(infSgn, 0x7FF, 0);
	} 
    else if (CisInf || DisInf) {
		// inf in addend
		infSgn = sgnC ^ sgnD ^ negateCD;
        return Flip64_create(infSgn, 0x7FF, 0);
	}
    /////////////////////////////////

	correctExpA = 0;
	correctExpB = 0;
	correctExpC = 0;
	correctExpD = 0;
	// computing real mantissa by adding a leading one if operand was normal
	// and shift left (with exponent correction) if operand was subnormal
	mantA = AisNormal?(1<<23)|mantA:(mantA << LZC32(mantA, 23, &correctExpA));
	mantB = BisNormal?(1<<23)|mantB:(mantB << LZC32(mantB, 23, &correctExpB));
	mantC = CisNormal?(1<<23)|mantC:(mantC << LZC32(mantC, 23, &correctExpC));
	mantD = DisNormal?(1<<23)|mantD:(mantD << LZC32(mantD, 23, &correctExpD));

	// exponent correction
	ea =  AisNormal ? expA - 127 : - 126 - correctExpA;
	eb =  BisNormal ? expB - 127 : - 126 - correctExpB;
	ec =  CisNormal ? expC - 127 : - 126 - correctExpC;
	ed =  DisNormal ? expD - 127 : - 126 - correctExpD;


	multAB = ((uint64_t) mantA) * ((uint64_t) mantB); 
	eMultAB = ea + eb;

	multCD = ((uint64_t) mantC) * ((uint64_t) mantD); 
	eMultCD = ec + ed;

	// correction of EmultAB if A or B is zero
	if (AisZero || BisZero) eMultAB = -256;

	// correction of EmultCD if C or D is zero
	if (CisZero || DisZero) eMultCD = -256;


	// SIGN COMPUTATION
    prodSgnAB = sgnA ^ sgnB;
    prodSgnCD = sgnC ^ sgnD ^ negateCD;
    effectiveOp = prodSgnAB ^ prodSgnCD;

    if (prodSgnAB == prodSgnCD) {
        resultSgn = prodSgnCD==1?1:0;
    }
    else {
        resultSgn = prodSgnCD;
    }


	// exponent diff computation
	// WORK IN PROGRESS
    diff = eMultCD-eMultAB;
	alignedAB = Int128_fromUInt64(multAB);
	alignedCD = Int128_fromUInt64(multCD);

    // We put the greatest product to the left (with a margin of ten
    // bits) and shift the other one accordingly to the difference of
    // the exponent. As soon as difference is greater than the size of
    // the mantissa + 2, all the bits of the smallest one will only be
    // used for sticky bits computation. Nevertheless here we
    // potentially shift the other product the right even if it's not
    // necessary.
	if (diff >= 0) {
        expOrigin = eMultCD + 11;
        normShiftCD = 70;
        normShiftAB = (diff > 70)?0:(70-diff);
    }
    else {
        expOrigin = eMultAB + 11;
        normShiftAB = 70;
        normShiftCD = (-diff > 70)?0:(70+diff);
    }

	alignedAB = Int128_shl(alignedAB, Int128_fromUInt64(normShiftAB));
    alignedCD = Int128_shl(alignedCD, Int128_fromUInt64(normShiftCD));

	if (effectiveOp == 0) sum = Int128_add(alignedCD, alignedAB);
	else sum = Int128_sub(alignedCD, alignedAB);

	// absolute value
	if (effectiveOp != 0) {
        if (sum.dwords[1] >> 63) {
            resultSgn = !resultSgn;
        }
        sum = Int128_abs(sum);
	}

	lzcSUM = Int128_clz(sum, 128);


	// Managing zero cases
	if (lzcSUM == 128) {
		// result is zero
		newExp = -1023;
		normResult = Int128_fromUInt64(0);
		// rounding toward minus infinity
		if (roundMode == FLIP_RD) {
			// addition case 
			resultSgn = (!prodSgnAB) && (!prodSgnCD) ? 0 : 1;
			// substraction case
			//else resultSgn = (!prodSgn) && (sgnC ^ negateC) ? 0 : 1;
		} else {
			// other rounding mode
			resultSgn = (prodSgnAB && prodSgnCD) ? 1 : 0;
		}
		// setting sign for different rounding mode
	} else {
		normResult = Int128_shl(sum, Int128_fromUInt64(lzcSUM));
		//-- old -- newExp = eMult + (54 - lzcSUM);
		newExp = expOrigin - lzcSUM;
		// we have to treat subnormal here, in order to manage correctly round and sticky bit
		if (newExp + 1023 <= 0) {
			// subnormal case
            // We don't shift too much: we don't want to looze sticky
            // bits for rouding. 30 bits is sufficient here.
            int shift = -(newExp + 1023)+1>60?60:-(newExp+1023)+1;
			normResult = Int128_shru(normResult, Int128_fromUInt64(shift));
		};
	}
	preResult_128 = Int128_shru(normResult, Int128_fromUInt64(128-53));
	preResult = Int128_toUInt64(preResult_128);


	preRoundBit = Int128_shru(Int128_shl(normResult, Int128_fromUInt64(53)), 
                    Int128_fromUInt64(127));
	roundBit = Int128_toUInt32(preRoundBit);


	preStickyBit = Int128_shl(normResult, Int128_fromUInt64(54));
	stickyBit = !((preStickyBit.dwords[0] == 0) & (preStickyBit.dwords[1] == 0));

    if (stickyBit || roundBit) Flip_raise(FLIP_INEXACT);

	// TODO : determine if result is subnormal or not and delete implicit one
	infByOverflow = false;

	biasedResultExp = newExp + 1023;

    if (flip_detect_tininess ==  flip_tininess_before_rounding) {
        if (biasedResultExp <= 0) {
            // We raise flags only if the result is not exactly 0 and
            // is not exact.
            if ((((preResult_128.dwords[0] | preResult_128.dwords[1]) || 
                (biasedResultExp < -1)) && (roundBit || stickyBit)) ||
                // Case when preResult_128 0 but normResult != 0
                (!(preResult_128.dwords[0] | preResult_128.dwords[1]) && 
                  (normResult.dwords[0] | normResult.dwords[1])))
                {
                Flip_raise(FLIP_UNDERFLOW | FLIP_INEXACT);
            }
        }   
    }   

	if (biasedResultExp <= 0) {
		uNewExp = 0;
		// exp for subnormal is 126 not 127 so +1
		//preResult = preResult >> (-biasedResultExp + 1);
	} else if (biasedResultExp >= 2047) {
		// overflow
		if ((roundMode == FLIP_RD && !resultSgn) || (roundMode == FLIP_RU && resultSgn) || (roundMode == FLIP_RZ)) {
            Flip_raise(FLIP_OVERFLOW | FLIP_INEXACT);
			uNewExp = 0x7fe;
			preResult = 0xfffffffffffffLL;
		} else {
            infByOverflow = true;
            uNewExp = 0xFFF;
            preResult = 0LL;
        }
	} else {
		// delete first one
		preResult = preResult & 0xfffffffffffffLL;
		uNewExp = biasedResultExp;
	}

	preRoundedResult = (resultSgn << 63) | (uNewExp << 52) | (preResult);
	lastBitOdd = preRoundedResult & 1;
	// ROUNDING 
    if (!infByOverflow) {
        switch(roundMode) {
            case FLIP_RN:
                if (roundBit && stickyBit) roundedResult = preRoundedResult+1;
                else if (roundBit && lastBitOdd) roundedResult = preRoundedResult+1;
                else roundedResult = preRoundedResult;
                break;
            case FLIP_RU:
                if (!resultSgn && (roundBit || stickyBit)) {
                    roundedResult = preRoundedResult+1;
                    if (Flip64_isInf(roundedResult)) Flip_raise(FLIP_OVERFLOW);
                }
                // negative
                else roundedResult = preRoundedResult; 
                break;
            case FLIP_RD:
                if (resultSgn && (roundBit || stickyBit)) roundedResult = preRoundedResult+1;
                // negative
                else roundedResult = preRoundedResult; 
                break;
            case FLIP_RZ:
                roundedResult = preRoundedResult;
                break;
            default:
                break;
        };
    }
    else {
        roundedResult = preRoundedResult;
    }

    if (Flip64_isInf(roundedResult)) Flip_raise(FLIP_OVERFLOW | FLIP_INEXACT);

    return Flip64_create(roundedResult >> 63, 
                            (roundedResult & 0x7FF0000000000000LL) >> 52,
                            roundedResult & 0xFFFFFFFFFFFFFLL);
}


/**
 * Implementation of FMA provided by Arenaire.
 * We don't use it directly outside LAO but through wrappers (fma, fmn,
 * ..)
 */
static
flip64 lao_ffma_64_64_c(uint64_t A, uint64_t B, uint64_t C, Flip_rnd_mode_t roundMode, 
                uint32_t negateAB, uint32_t negateC) {
	uint32_t sgnA, sgnB, sgnC;
	uint64_t mantA, mantB, mantC;
	uint32_t expA, expB, expC;
	// flags
	uint32_t AisNormal, AisNaN, AisInf, AisZero;
	uint32_t BisNormal, BisNaN, BisInf, BisZero;
	uint32_t CisNormal, CisNaN, CisInf, CisZero;

#ifdef DEBUG_FFMAD
    printf("\n\n\ncomputing FFMAD RND=%d, negateAB=%d, negateC=%d \n", roundMode, negateAB, negateC);
    printf("   A=%"PRIx64"\n", A);
    printf("   B=%"PRIx64"\n", B);
    printf("   C=%"PRIx64"\n", C);
#endif

    uint32_t effectiveOp;
    uint32_t infSgn;

    int correctExpA, correctExpB, correctExpC;
    int ea, eb, ec;

    uint64_t ma, mb;
	Int128_ mult;
	
    int eMult;

	uint64_t resultSgn; // TODO : to be defined
	int diff, expOrigin = 0;

    Int256_ tmpC;
	Int256_ sum;

    uint64_t sgnMask, sumSgn, sumPreCarry;
	int lzcSUM;
	Int256_ normResult;
	int newExp;
	uint32_t prodSgn;

	Int256_ preResult_256, preRoundBit, preStickyBit;
	uint64_t preResult;
    uint32_t roundBit, stickyBit;

	// TODO : determine if result is subnormal or not and delete implicit one
	uint64_t uNewExp;
	bool infByOverflow;

	Int256_ alignedAB;

	int biasedResultExp;
	Int256_ delCarryMask;
	Int256_ shiftedC;

	uint64_t preRoundedResult, roundedResult;
    uint32_t lastBitOdd;
    int normShiftC, normShiftAB;

    uNewExp = 0;
    roundedResult = 0;

	// unpacking and setting flags
	unpack64(A, &sgnA, &mantA, &expA, &AisNormal, &AisNaN, &AisInf, &AisZero);
	unpack64(B, &sgnB, &mantB, &expB, &BisNormal, &BisNaN, &BisInf, &BisZero);
	unpack64(C, &sgnC, &mantC, &expC, &CisNormal, &CisNaN, &CisInf, &CisZero);

	// effective operation : addition (0) or substraction (1)
	effectiveOp = (((negateAB ^ sgnA) ^ sgnB) ^ (sgnC ^ negateC)); 

    /////////////////////////////////
    //  NAN HANDLING
    /////////////////////////////////
    // If one of the operand is a signaling NaN, we generate an
    // exception.
    if (Flip64_issNaN(A) || Flip64_issNaN(B) || Flip64_issNaN(C)) {
        Flip_raise(FLIP_INVALID);
    }
    // If one of the operand is already a NaN, we return a quiet NaN
    // (same sign, same payload).
    if (AisNaN) return Flip64_quiet_from_sNaN(A);
    if (BisNaN) return Flip64_quiet_from_sNaN(B);
    if (CisNaN) return Flip64_quiet_from_sNaN(C);

    // We check if we *generate* NaN
	if ((((AisInf || BisInf) && CisInf && effectiveOp)) || 
        (AisZero && BisInf) || (BisZero && AisInf)) {
        Flip_raise(FLIP_INVALID);
        return Flip64_quiet_NaN();
	}
    /////////////////////////////////

    /////////////////////////////////
    // INFINITY
    /////////////////////////////////
    // In NaN handling above we checked the case when C is also infinity
    // (which could generate NaN).
    if (AisInf || BisInf) {
		// inf in mult
		infSgn = sgnA ^ sgnB ^ negateAB;
        return Flip64_create(infSgn, 0x7FF, 0);
	} 
    else if (CisInf) {
		// inf in addend
		infSgn = sgnC ^  negateC;
        return Flip64_create(infSgn, 0x7FF, 0);
	}
    /////////////////////////////////

	correctExpA = 0;
	correctExpB = 0;
	correctExpC = 0;
	// computing real mantissa by adding a leading one if operand was normal
	// and shift left (with exponent correction) if operand was subnormal
	mantA = AisNormal?(1LL<<52)|mantA:(mantA<<LZC64(mantA, 52, &correctExpA));
	mantB = BisNormal?(1LL<<52)|mantB:(mantB<<LZC64(mantB, 52, &correctExpB));
	mantC = CisNormal?(1LL<<52)|mantC:(mantC<<LZC64(mantC, 52, &correctExpC));
	// exponent correction
	ea =  AisNormal ? expA - 1023 : - 1022 - correctExpA;
	eb =  BisNormal ? expB - 1023 : - 1022 - correctExpB;
	ec =  CisNormal ? expC - 1023 : - 1022 - correctExpC;

	ma = mantA;
	mb = mantB;
	mult = Int128_mul(Int128_fromUInt64(ma), Int128_fromUInt64(mb)); 
	eMult = ea + eb;

#ifdef DEBUG_FFMAD
    printf("MULTIPLICATION %"PRIx64" x %"PRIx64"\n", ma, mb);
    Int128_print(mult);
#endif

	// correction of Emult if A or B is zero
	if (AisZero || BisZero) eMult = -2048;

	// SIGN COMPUTATION
	resultSgn = sgnC ^ negateC;

	// exponent diff computation
	// WORK IN PROGRESS
    diff = ec-eMult;
	expOrigin = 0;

    /* Datapath is 256-bit wide 
     * if diff = (ec - eMult) >= 0
     *        C is left shifted with 10 bit margin (shift = 256 - 53 - 10 = 133)
     *        AB (length: 106 bit) 
     * else
     *
     */

    // We completely shift C to the left with a margin of 10 bits.
	if (diff >= 0) {
        expOrigin = ec + 10;
        normShiftC = 193;
        normShiftAB = (diff > 140)?0:(141-diff);
	}
    // We completely shift AB to the left with a margin of 10 bits.
    else {
        expOrigin = eMult + 11;
        normShiftAB = 140;
        normShiftC = (diff <= -193)?0:192+diff;
	};
    tmpC = Int256_fromUInt64(mantC);
    shiftedC = Int256_shl(tmpC, Int256_fromUInt64(normShiftC));
	alignedAB = Int256_shl(Int256_fromInt128(mult), 
                    Int256_fromUInt64(normShiftAB));

#ifdef DEBUG_FFMAD
    printf("aligned operands:\n");
    printf("   shiftedC  ="); 
    Int256_print(shiftedC); 
    printf("   shiftedAB ="); 
    Int256_print(alignedAB);
#endif

	if (effectiveOp == 0) sum = Int256_add(shiftedC,alignedAB);
	else sum = Int256_sub(shiftedC, alignedAB);

#ifdef DEBUG_FFMAD
    printf("sum = ");
    Int256_print(sum);
#endif

	// absolute value

	if (effectiveOp != 0) {
		if (sum.dwords[3] >> 63) {
#ifdef DEBUG_FFMAD
            printf("negative sum\n");
#endif
			resultSgn = !resultSgn;
        }
        sum = Int256_abs(sum);
	}

	lzcSUM = Int256_clz(sum, 256);

#ifdef DEBUG_FFMAD
    printf("resultSgn: %d\n", resultSgn);
    printf("lzcSUM: %d\n", lzcSUM);
#endif

	prodSgn = sgnA ^ sgnB ^ negateAB;
	// Managing zero cases
	if (lzcSUM == 256) {
		// result is zero
		newExp = -1023;
		normResult = Int256_fromUInt64(0);
		// rounding toward minus infinity
		if (roundMode == FLIP_RD) {
			// addition case 
			resultSgn = (!prodSgn) && (!(sgnC ^ negateC)) ? 0 : 1;
			// substraction case
			//else resultSgn = (!prodSgn) && (sgnC ^ negateC) ? 0 : 1;
		} else {
			// other rounding mode
			resultSgn = (prodSgn && (sgnC ^ negateC)) ? 1 : 0;
		}
		// setting sign for different rounding mode
	} else {
		normResult = Int256_shl(sum, Int256_fromUInt64(lzcSUM));
		//-- old -- newExp = eMult + (54 - lzcSUM);
		newExp = expOrigin - lzcSUM;
		// we have to treat subnormal here, in order to manage correctly round and sticky bit
		if (newExp + 1023 <= 0) {
			// subnormal case
            // We don't shift too much: we don't want to looze sticky
            // bits for rouding. 60 bits is sufficient here.
            int shift = -(newExp + 1023)+1>60?60:-(newExp+1023)+1;
			normResult = Int256_shru(normResult, Int256_fromUInt64(shift));
            #ifdef DEBUG_FFMAD
            printf("shift=%d\n", shift);
            #endif
		}
	}

	preResult_256 = Int256_shru(normResult, Int256_fromUInt64(256-53));
	preResult = Int256_toUInt64(preResult_256);

    Int256_ tmp_sh = Int256_shl(normResult, Int256_fromUInt64(53));

#ifdef DEBUG_FFMAD
    printf("normResult: ");
    Int256_print(normResult);
    printf("preResult: %016"PRIx64"\n", preResult);
    printf("tmp_sh: ");
    Int256_print(tmp_sh);
#endif
	//preRoundBit = Int256_shru(Int256_shl(normResult, Int256_fromUInt64(53)), 
    //                Int256_fromUInt64(255));
    preRoundBit = Int256_shru(tmp_sh, Int256_fromUInt64(255));                
	roundBit = Int256_toUInt32(preRoundBit);

#ifdef DEBUG_FFMAD
    printf("preRoundBit: ");
    Int256_print(preRoundBit);
    printf("roundBit: %d\n", roundBit);
#endif


	preStickyBit = Int256_shl(normResult, Int256_fromUInt64(54));
	stickyBit = !((preStickyBit.dwords[0] == 0) & (preStickyBit.dwords[1] == 0) & (preStickyBit.dwords[2] == 0) & (preStickyBit.dwords[3] == 0));


#ifdef DEBUG_FFMAD
    printf("preStickyBit: ");
    Int256_print(preStickyBit);
    printf("stickyBit: %d\n", stickyBit);
#endif

    if (stickyBit || roundBit) Flip_raise(FLIP_INEXACT);
	// TODO : determine if result is subnormal or not and delete implicit one
	infByOverflow = false;

	biasedResultExp = newExp + 1023;

    if (flip_detect_tininess ==  flip_tininess_before_rounding) {
        if (biasedResultExp <= 0) {
            // We raise flags only if the result is not exactly 0 and
            // is not exact.
            if ((((preResult_256.dwords[0] | preResult_256.dwords[1] | preResult_256.dwords[2] | preResult_256.dwords[3]) || 
                (biasedResultExp < -1)) && (roundBit || stickyBit)) ||
                // Case when preResult_128 0 but normResult != 0
                (!(preResult_256.dwords[0] | preResult_256.dwords[1] | preResult_256.dwords[2] | preResult_256.dwords[3]) && 
                  (normResult.dwords[0] | normResult.dwords[1] | normResult.dwords[2] | normResult.dwords[3])))
                {
                Flip_raise(FLIP_UNDERFLOW | FLIP_INEXACT);
            }
        }
    }

	if (biasedResultExp <= 0) {
		uNewExp = 0;
		// exp for subnormal is 126 not 127 so +1
		//preResult = preResult >> (-biasedResultExp + 1);
	} else if (biasedResultExp >= 2047) {
		// overflow
		if ((roundMode == FLIP_RD && !resultSgn) || (roundMode == FLIP_RU && resultSgn) || (roundMode == FLIP_RZ)) {
            Flip_raise(FLIP_OVERFLOW | FLIP_INEXACT);
			uNewExp = 0x7fe;
			preResult = 0xfffffffffffffLL;
		} else {
            infByOverflow = true;
            uNewExp = 0x7FF;
            preResult = 0LL;
        }
	} else {
		// delete first one
		preResult = preResult & 0xfffffffffffffLL;
		uNewExp = biasedResultExp;
	}

	preRoundedResult = (resultSgn << 63) | (uNewExp << 52) | (preResult);

#ifdef DEBUG_FFMAD
    printf("preRoundedResult=%016"PRIx64"\n", preRoundedResult);
#endif

	lastBitOdd = preRoundedResult & 1;
    if (!infByOverflow) {
        // ROUNDING 
        switch(roundMode) {
            case FLIP_RN:
                if (roundBit && stickyBit) roundedResult = preRoundedResult+1;
                else if (roundBit && lastBitOdd) roundedResult = preRoundedResult+1;
                else roundedResult = preRoundedResult;
                break;
            case FLIP_RU:
                if (!resultSgn && (roundBit || stickyBit)) {
                    roundedResult = preRoundedResult+1;   
                    if (Flip64_isInf(roundedResult)) Flip_raise(FLIP_OVERFLOW);
                }
                // negative
                else roundedResult = preRoundedResult; 
                break;
            case FLIP_RD:
                if (resultSgn && (roundBit || stickyBit)) roundedResult = preRoundedResult+1;
                // negative
                else roundedResult = preRoundedResult; 
                break;
            case FLIP_RZ:
                roundedResult = preRoundedResult;
                break;
            default:
                break;
        };
    }
    else {
        roundedResult = preRoundedResult;
    }

    if (Flip64_isInf(roundedResult)) Flip_raise(FLIP_OVERFLOW | FLIP_INEXACT);

    return Flip64_create(roundedResult >> 63, 
                        (roundedResult & 0x7FF0000000000000LL) >> 52,
                        roundedResult & 0xFFFFFFFFFFFFFLL);
}


/* Floating-Point multiplication fp32 * fp32 -> f⁶4 */

#ifdef $XCC_h
flip64
Flip64_fmulwd(flip32 x, flip32 y);
#endif//$XCC_h

flip64
Flip64_fmulwd(flip32 x, flip32 y) {
    flag sgnA = Flip32_sign(x);
    flag sgnB = Flip32_sign(y);
    flip64 c = Flip64_create(sgnA ^ sgnB, 0, 0);
    return FPFMA_c(x, y, c, Flip_rnd_mode, 0, 0);
}

#ifdef $XCC_h
flip64
Flip64_fmulnwd(flip32 x, flip32 y);
#endif//$XCC_h

flip64
Flip64_fmulnwd(flip32 x, flip32 y) {
    flag sgnA = Flip32_sign(x);
    flag sgnB = Flip32_sign(y);
    flip64 c = Flip64_create(sgnA ^ sgnB, 0, 0);
    return FPFMA_c(x, y, c, Flip_rnd_mode, 1, 1);
}


#ifdef $XCC_h
/** 
\brief FP \b fused multiply and add operation: returns \f$ x \times y + z \f$.
\param x is the multiplicand (32-bit FPN)
\param y is the multiplier (32-bit FPN)
\param z is the addend (64-bit FPN)
\addindex FPN
*/
flip64
Flip64_fma(flip64 z, flip32 x, flip32 y);
#endif//$XCC_h

flip64
Flip64_fma(flip64 z, flip32 x, flip32 y) {
    return FPFMA_c(x, y, z, Flip_rnd_mode, 0, 0);
}


#ifdef $XCC_h
flip64
Flip64_fman(flip64 z, flip32 x, flip32 y);
#endif//$XCC_h

flip64
Flip64_fman(flip64 z, flip32 x, flip32 y) {
  return FPFMA_c(x, y, z, Flip_rnd_mode, 1, 1);
}


#ifdef $XCC_h
flip64
Flip64_fms(flip64 z, flip32 x, flip32 y);
#endif//$XCC_h

flip64
Flip64_fms(flip64 z, flip32 x, flip32 y) {
  return FPFMA_c(x, y, z, Flip_rnd_mode, 1, 0);
}


#ifdef $XCC_h
flip64
Flip64_fmsn(flip64 z, flip32 x, flip32 y);
#endif//$XCC_h

flip64
Flip64_fmsn(flip64 z, flip32 x, flip32 y) {
  return FPFMA_c(x, y, z, Flip_rnd_mode, 0, 1);
}

#ifdef $XCC_h
/** 
\brief FP \b fused multiply and add operation: returns \f$ x \times y + z \f$.
\param x is the multiplicand (64-bit FPN)
\param y is the multiplier (64-bit FPN)
\param z is the addend (64-bit FPN)
\addindex FPN
*/
flip64
Flip64_fmad(flip64 z, flip64 x, flip64 y);
#endif//$XCC_h

flip64
Flip64_fmad(flip64 z, flip64 x, flip64 y) {
    return lao_ffma_64_64_c(x, y, z, Flip_rnd_mode, 0, 0);
}

#ifdef $XCC_h
flip64
Flip64_fmand(flip64 z, flip64 x, flip64 y);
#endif//$XCC_h

flip64
Flip64_fmand(flip64 z, flip64 x, flip64 y) {
  return lao_ffma_64_64_c(x, y, z, Flip_rnd_mode, 1, 1);
}


#ifdef $XCC_h
flip64
Flip64_fmsd(flip64 z, flip64 x, flip64 y);
#endif//$XCC_h

flip64
Flip64_fmsd(flip64 z, flip64 x, flip64 y) {
  return lao_ffma_64_64_c(x, y, z, Flip_rnd_mode, 1, 0);
}

#ifdef $XCC_h
flip64
Flip64_fmsnd(flip64 z, flip64 x, flip64 y);
#endif//$XCC_h

flip64
Flip64_fmsnd(flip64 z, flip64 x, flip64 y) {
  return lao_ffma_64_64_c(x, y, z, Flip_rnd_mode, 0, 1);
}



#ifdef $XCC_h
/** 
\brief FP \b dual multiply and add operation: returns \f$ a \times b + c \times d \f$.
\param a is the multiplicand (32-bit FPN)
\param b is the multiplier (32-bit FPN)
\param c is the multiplicand (32-bit FPN)
\param d is the multiplier (32-bit FPN)
\addindex FPN
*/
flip64
Flip64_fdma(flip32 a, flip32 b, flip32 c, flip32 d);
#endif//$XCC_h

flip64
Flip64_fdma(flip32 a, flip32 b, flip32 c, flip32 d) {
    return FPFDMA_c(a, b, c, d, Flip_rnd_mode, 0);
}

#ifdef $XCC_h
/** 
\brief FP \b dual multiply and substract operation: returns \f$ a \times b - c \times d \f$.
\param a is the multiplicand (32-bit FPN)
\param b is the multiplier (32-bit FPN)
\param c is the multiplicand (32-bit FPN)
\param d is the multiplier (32-bit FPN)
\addindex FPN
*/
flip64
Flip64_fdms(flip32 a, flip32 b, flip32 c, flip32 d);
#endif//$XCC_h

flip64
Flip64_fdms(flip32 a, flip32 b, flip32 c, flip32 d) {
    return FPFDMA_c(a, b, c, d, Flip_rnd_mode, 1);
}



#ifdef $XCC_h
flip64
Flip64_mul(flip64 a, flip64 b);
#endif//$XCC_h

/*----------------------------------------------------------------------------
| Returns the result of multiplying the double-precision floating-point values
| `a' and `b'.  The operation is performed according to the IEC/IEEE Standard
| for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
flip64 Flip64_mul(flip64 a, flip64 b )
{
    flag aSign, bSign, zSign;
    int16_t aExp, bExp, zExp;
    bits64 aSig, bSig, zSig0, zSig1;

    aSig = Flip64_frac( a );
    aExp = Flip64_exp( a );
    aSign = Flip64_sign( a );
    bSig = Flip64_frac( b );
    bExp = Flip64_exp( b );
    bSign = Flip64_sign( b );
    zSign = aSign ^ bSign;

    /////////////////////////////////
    //  NAN HANDLING
    /////////////////////////////////
    // If one of the operand is a signaling NaN, we generate an
    // exception.
    if (Flip64_issNaN(a) || Flip64_issNaN(b)) {
        Flip_raise(FLIP_INVALID);
    }
    // If one of the operand is already a NaN, we return a quiet NaN
    // (same sign, same payload).
    if (Flip64_isNaN(a)) return Flip64_quiet_from_sNaN(a);
    if (Flip64_isNaN(b)) return Flip64_quiet_from_sNaN(b);

    // We check if we *generate* NaN
    if ((((aExp == 0x7FF) && (aSig == 0)) && ((bExp | bSig) == 0)) ||
        (((bExp == 0x7FF) && (bSig == 0)) && ((aExp | aSig) == 0))) {
        Flip_raise(FLIP_INVALID);
        return Flip64_quiet_NaN();
	}
    /////////////////////////////////

    /////////////////////////////////
    // INFINITY
    /////////////////////////////////
    // In NaN handling above we checked the case when C is also infinity
    // (which could generate NaN).
    if ((aExp == 0x7FF) && (aSig == 0)) {
        return Flip64_create( zSign, 0x7FF, 0 );
    }
    if ((bExp == 0x7FF) && (bSig == 0)) {
        return Flip64_create( zSign, 0x7FF, 0 );
    }
    /////////////////////////////////


    if (aExp == 0) {
        if (aSig == 0) return Flip64_create( zSign, 0, 0 );
        Flip64_normalizeSubnormal(aSig, &aExp, &aSig);
    }
    if ( bExp == 0 ) {
        if ( bSig == 0 ) return Flip64_create(zSign, 0, 0);
        Flip64_normalizeSubnormal(bSig, &bExp, &bSig);
    }
    zExp = aExp + bExp - 0x3FF;
    aSig = ( aSig | 0x0010000000000000LL)<<10;
    bSig = ( bSig | 0x0010000000000000LL)<<11;
    Flip_mul64To128(aSig, bSig, &zSig0, &zSig1);
    zSig0 |= ( zSig1 != 0 );
    if ( 0 <= (sbits64) ( zSig0<<1 ) ) {
        zSig0 <<= 1;
        --zExp;
    }
    return Flip64_roundAndPackFloat(zSign, zExp, zSig0, 0);
}


#ifdef $XCC_h
static inline flip64
Flip64_muln(flip64 x, flip64 y)
{ 
    /////////////////////////////////
    //  NAN HANDLING
    /////////////////////////////////
    // If one of the operand is a signaling NaN, we generate an
    // exception.
    if (Flip64_issNaN(x) || Flip64_issNaN(y)) {
        Flip_raise(FLIP_INVALID);
    }
    // If one of the operand is already a NaN, we return a quiet NaN
    // (same sign, same payload).
    if (Flip64_isNaN(x)) return Flip64_quiet_from_sNaN(x);
    if (Flip64_isNaN(y)) return Flip64_quiet_from_sNaN(y);

    return Flip64_mul(Flip64_neg(x), y);
}
#endif//$XCC_h

#ifdef $XCC_h
flip64
Flip64_sub(flip64 x, flip64 y);
#endif//$XCC_h

flip64
Flip64_sub(flip64 x, flip64 y)
{ 
    /////////////////////////////////
    //  NAN HANDLING
    /////////////////////////////////
    // If one of the operand is a signaling NaN, we generate an
    // exception.
    if (Flip64_issNaN(x) || Flip64_issNaN(y)) {
        Flip_raise(FLIP_INVALID);
    }
    // If one of the operand is already a NaN, we return a quiet NaN
    // (same sign, same payload).
    if (Flip64_isNaN(y)) return Flip64_quiet_from_sNaN(y);
    if (Flip64_isNaN(x)) return Flip64_quiet_from_sNaN(x);

    return Flip64_add(x, Flip64_neg(y));
}

#ifdef $XCC_h
/**
 * \fn flip64 Flip64_fromMul(flip64 x, uint64_t y);
 * \brief Takes a float and an integer and returns the rounded result of
 * x multiplied by 1 << y.
 * @param[in] x The float to divide
 * @param[in] y The amount of shift by which x must be multiplied.
 * @return flip64 The float representing the correct rounded value of x*(1<<y).
 */
flip64
Flip64_fromMul(flip64 x, uint64_t y);
#endif//$XCC_h

flip64
Flip64_fromMul(flip64 x, uint64_t y)
{ 
    bits64 aSig = Flip64_frac( x );
    bits64 aExp = Flip64_exp( x );
    bits64 aSign = Flip64_sign( x );

    if (Flip64_isNaN(x)) {
        Flip_raise(FLIP_INVALID);
        return Flip64_create(aSign, 0, 0);
    }

    if ((aExp || aSig) == 0) {return x;}

    if (y + aExp > 0x7FE) {
        if ((0x7FE - aExp) > 52) {
            // In this case we set value to infinity
            return Flip64_create(aSign, 0x7FF, 0);
        }
        else {
            // We don't want in that case to generate exception when multiplying
            // the floating point value.
            // We know that the correct int will be returned even though there
            // is double rounding because either the rounding is done when the
            // floating point value is too big: in that case we always return
            // int_max; or either the floating point is subnormal and in that
            // case the range of the multiplication is too small to reach 0 so
            // we will return 0.
            flip_exception_t exception = Flip_status;
            flip64 tmp = Flip64_roundAndPackFloat(aSign, 0x7FE, aSig << (y - (0x7FE - aExp)), 0);
            Flip_status = exception;
            return tmp;
        }
    }
    else {
        return Flip64_create(aSign, aExp + y, aSig);
    }
}


#ifdef $XCC_h
/**
 * \fn flip64 Flip64_fromUMul(flip64 x, uint64_t y);
 * \brief Takes a float and an integer and returns the unsigned rounded result of
 * x multiplied by 1 << y.
 * @param[in] x The float to divide
 * @param[in] y The amount of shift by which x must be multiplied.
 * @return flip64 The float representing the correct unsigned rounded value of x*(1<<y).
 */
flip64
Flip64_fromUMul(flip64 x, uint64_t y);
#endif//$XCC_h

flip64
Flip64_fromUMul(flip64 x, uint64_t y)
{ 
    bits64 aSign = Flip64_sign( x );
    if (aSign) {
        // We don't generate an exception if float is -0.
        if (Flip64_frac(x) || Flip64_exp(x)) Flip_raise(FLIP_INVALID);
        return Flip64_create(0, 0, 0);
    }
    else { 
        return Flip64_fromMul(x, y);
    }
}

#ifdef $XCC_h
int64_t
Flip64_fixed(flip64 fopnd2, uint32_t y);
#endif//$XCC_h

int64_t
Flip64_fixed(flip64 fopnd2, uint32_t y) {
    return Flip64_toInt64(Flip64_fromMul(fopnd2, y));
}

#ifdef $XCC_h
uint64_t
Flip64_fixedu(flip64 fopnd2, uint32_t y);
#endif//$XCC_h

uint64_t
Flip64_fixedu(flip64 fopnd2, uint32_t y) {
    return Flip64_toUint64(Flip64_fromUMul(fopnd2, y));
}


#ifdef $XCC_h
/**
 * \fn flip64 Flip64_fromFrac(flip64 x, uint32_t y);
 * \brief Takes a float and an integer and returns the rounded result of
 * x divided by 1 << y.
 * @param[in] x The float to divide
 * @param[in] y The amount of shift by which x must be divided.
 * @return flip64 The float representing the correct rounded value of x / 1 << y.
 */
flip64
Flip64_fromFrac(flip64 x, uint32_t y);
#endif//$XCC_h

flip64
Flip64_fromFrac(flip64 x, uint32_t y)
{ 
    bits64 aSig = Flip64_frac( x );
    bits64 aExp = Flip64_exp( x );
    bits64 aSign = Flip64_sign( x );
    if (y <= aExp) {
        return Flip64_create(aSign, aExp - y, aSig);
    }
    else {
        if (aSig & ((1 << (y -aExp)) - 1)) {
            Flip_raise(FLIP_INEXACT | FLIP_UNDERFLOW);
        }
        return Flip64_roundAndPackFloat(aSign, 0, aSig >> (y - aExp), 0);
    }
}

#ifdef $XCC_h
flip64
Flip64_float(int64_t opnd2, int32_t y);
#endif//$XCC_h

flip64
Flip64_float(int64_t opnd2, int32_t y) {
    return Flip64_fromFrac(Flip64_fromInt64(opnd2), y);
}

#ifdef $XCC_h
flip64
Flip64_floatu(uint64_t opnd2, uint32_t y);
#endif//$XCC_h

flip64
Flip64_floatu(uint64_t opnd2, uint32_t y) {
    return Flip64_fromFrac(Flip64_fromUint64(opnd2), y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//

#ifdef $XCC_h
/** \brief returns the inverse of the square root of \a X
\a X should be a 64-bit floating-point number.
*/
flip64
Flip64_invsqrt(flip64 X);
#endif//$XCC_h

flip64
Flip64_invsqrt(flip64 X) {
  flip64 ret;
	uint32_t ApprRecipSqrtTable[128] ={209, 204, 198, 193, 188, 183, 178, 173, 168, 164, 159, 155, 150, 146, 142, 138, 134, 130, 126, 122, 118, 114, 111, 107, 104, 100, 97, 94, 90, 87, 84, 81, 78, 75, 72, 69, 66, 63, 60, 57, 55, 52, 49, 47, 44, 42, 39, 37, 34, 32, 29, 27, 25, 22, 20, 18, 16, 14, 11, 9, 7, 5, 3, 1, 508, 500, 493, 485, 478, 471, 464, 457, 450, 444, 437, 431, 425, 419, 413, 407, 401, 395, 390, 384, 379, 374, 369, 364, 359, 354, 349, 344, 340, 335, 331, 326, 322, 318, 313, 309, 305, 301, 297, 293, 289, 286, 282, 278, 274, 271, 267, 264, 260, 257, 254, 250, 247, 244, 241, 237, 234, 231, 228, 225, 222, 219, 216, 213};

    uint64_t sigField;
	uint64_t absX, sgn, expField, XisNormal, XisInfOrNaN, 
		XhasNullSig,XisZero, XisNaN, XisInf, index, InvSqrt, RexpField, 
		bias, qNaN, Inf;
	uint64_t bit0, bit1, bit2, bit3, bit4;
	sgn        = X & ((uint64_t) 1 << 63);
	expField   = (X<<1) >> 53;
	sigField   = (X&    0x000FFFFFFFFFFFFFull);
	absX       = X&     0x7FFFFFFFFFFFFFFFull;
	XisNormal  = absX > 0x000FFFFFFFFFFFFFull;
	XisInfOrNaN= (expField == 0x7FF);
	XhasNullSig= (sigField ==0);
	XisZero = (!XisNormal) && XhasNullSig;
	XisNaN = XisInfOrNaN && !XhasNullSig;
	XisInf = XisInfOrNaN && XhasNullSig;
	index = (X>>(53-7)) & 127;
	InvSqrt   = ApprRecipSqrtTable[index];
	bias = 1023; 
	if(expField&1) 
		RexpField = (3*bias-2 - expField)>>1;
	else
		RexpField = (3*bias-1 - expField)>>1;

	// Markstein advice to use e <= emin + N  - 1 
	// for special case detection
	// where N is the precision
	// e = expField - 127 + flag <= - 126 + 24 - 1
	// expField <= -1 - flag -1 + 24 = 22
	// we choose 26 as a security measure
	bit4 = (expField <= 55); 

	bit1 = XisZero;
	bit2 = XisInf && (!sgn);
	bit3 = XisNaN || (sgn && (! XisZero));

	bit0 = ! (bit1 || bit2 || bit3 || bit4 || (!XisNormal));

    //if (XisZero) Flip_raise(FLIP_DIV_BY_ZERO);

	ret= sgn | (RexpField << 52) | (InvSqrt << 43) | (bit4 << 4) | (bit3 << 3) | (bit2 << 2) | (bit1 << 1) | bit0;
  return ret;
}

#ifdef $XCC_h
/** \brief returns the reciprocal \a 1 / \a B
\a B should be a 64-bit floating-point number.
\a A should be a 64-bit floating-point number.
\a behavior shoud be a 64-bit integer number 
\addindex FPN
*/
flip64
Flip64_inv(flip64 A, flip64 B, int behavior);
#endif//$XCC_h

flip64 Flip64_inv(flip64 A, flip64 B, int behavior) { 
  flip64 ret;
    uint32_t ApprRecipTable[128] ={508, 500, 492, 485, 477, 470, 463, 455, 448, 441, 434, 428, 421, 414, 408, 401, 395, 389, 383, 377, 371, 365, 359, 353, 347, 342, 336, 331, 326, 320, 315, 310, 305, 300, 295, 290, 285, 280, 275, 271, 266, 261, 257, 252, 248, 243, 239, 235, 231, 226, 222, 218, 214, 210, 206, 202, 198, 195, 191, 187, 183, 180, 176, 172, 169, 165, 162, 158, 155, 152, 148, 145, 142, 138, 135, 132, 129, 126, 123, 120, 117, 114, 111, 108, 105, 102, 99, 96, 93, 91, 88, 85, 82, 80, 77, 74, 72, 69, 67, 64, 62, 59, 57, 54, 52, 49, 47, 45, 42, 40, 38, 35, 33, 31, 29, 26, 24, 22, 20, 18, 15, 13, 11, 9, 7, 5, 3, 0};
    //uint32_t absX, sgn, expField, sigField, XisNormal, XisInfOrNaN, 
    //  XhasNullSig,XisZero, XisNaN, XisInf, index, Inv, RexpField;
    uint64_t sgnB, absB, expFieldA, expFieldB, sigFieldB, BisNormal, BisInf;
    uint64_t AisInfOrNaN, BisInfOrNaN, BisNaN, BhasNullSig, BisZero;
    uint64_t index, Inv, RexpField;
    uint64_t bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9, bit10, bit11, bit12;
    int expFieldA_int, expFieldB_int;
    uint64_t bitVector;
    uint64_t AisZero;
    uint64_t sgnA, sigFieldA, AisNaN;
    uint64_t direct_result_mask, AhasNullSig;
    // unpacking operands A and B
    expFieldA = (A<< 1) >> 53; 
    expFieldB = (B<< 1) >> 53;
    BisInfOrNaN = expFieldB == 0x7FF;
    AisInfOrNaN = expFieldA == 0x7FF;
    sgnB        = B & ((uint64_t) 1<<63);
    sgnA        = A & 0x8000000000000000ull;
  
    AisZero     = (A   & 0x7FFFFFFFFFFFFFFFull) == 0; 
    sigFieldB   = (B   & 0x000FFFFFFFFFFFFFull);
    sigFieldA   = (A   & 0x000FFFFFFFFFFFFFull);
    absB        =  B   & 0x7FFFFFFFFFFFFFFFull;
    BisNormal   = absB > 0x000FFFFFFFFFFFFFull;
    BhasNullSig = (sigFieldB == 0);
    AhasNullSig = (sigFieldA == 0);

    BisZero = (!BisNormal) && BhasNullSig;

    BisNaN = BisInfOrNaN && !BhasNullSig;
    AisNaN = AisInfOrNaN && !AhasNullSig;

    BisInf = BisInfOrNaN && BhasNullSig;
    index = sigFieldB>>(52-7);
    Inv   = ApprRecipTable[index];
    RexpField = 0x7FD - expFieldB;
    RexpField = (RexpField << 53) >> 53;
    // bit for special cases
    bit1 = expFieldB == 0x000;
    bit2 = expFieldA == 0x000; 
    bit3 = BisInfOrNaN;
    // if expB = 253 or 254
    bit4 = (expFieldB == 0x7FD) | (expFieldB == 0x7FE);
    bit5 = AisInfOrNaN;
    expFieldA_int = expFieldA;
    bit6 = (expFieldA != 0x000) && (expFieldA_int < 55);
    expFieldB_int = expFieldB;
    bit7 = (expFieldA_int - expFieldB_int <= -1020);
    bit8 = (expFieldA_int - expFieldB_int >= 1023);
	bit11 = behavior == 2 ? (expFieldB_int >= 2043) : 0;
	bit12 = (behavior == 2) ? (bit1 || bit3 || bit4) : 0 ;
    bit0 = !(bit1 || bit2 || bit3 || bit4 || bit5 || bit6 || bit7 || bit8 || bit11 || bit12);

    if (bit0 == 1) bit10 = bit9 = 0;// non special cases
    else bit9 = 1; // non direct result

    bitVector = bit0 | (bit1 << 1) | (bit2 << 2) | (bit3 << 3) | (bit4 << 4) | (bit5 << 5) | (bit6 << 6) | (bit7 << 7) | (bit8 << 8) | (bit9 << 9) | (bit11 << 11) | (bit12 << 12);// | bit10 << 10;
    //if (BisZero && !BisNaN)   return sgnB | 0x7F800000 | bitVector;
    //else if(BisNaN)            return B;
    //else if(BisInf)            return sgnB | bitVector;
    //else     
    direct_result_mask  = 0xfffffffffffffdfeull; // mask of bit to be ANDed with direct result
    uint64_t q_bit_mask = 0x0008000000000000ull;
    if (AisNaN) {
        if (!(sigFieldA & q_bit_mask) || (BisNaN && !(sigFieldB & q_bit_mask))) Flip_raise(FLIP_INVALID); // sNaN
        return (A | q_bit_mask) & direct_result_mask;
    };
    if (BisNaN) {
        if (! (sigFieldB & q_bit_mask)) Flip_raise(FLIP_INVALID);
        return (B | q_bit_mask) & direct_result_mask;
    }
    if (AisInfOrNaN) { 
        // since A is not NaN (due to previous test
        // A is Inf
        if (BisZero) return B ^ A;
        if (BisInf) {
            Flip_raise(FLIP_INVALID);
            return 0xffffffffffffffffull & direct_result_mask;
        }
        return (B & 0x8000000000000000) ^ A;
    }
    if (BisInfOrNaN) {
        // due to previous tests this implies B is Inf
        // A is no Inf nor NaN
        return sgnA ^ sgnB;
    }
    if (BisZero && !AisZero) {
        Flip_raise(FLIP_DIV_BY_ZERO);
        return (sgnA ^ sgnB) | 0x7ff0000000000000ull;
    }

    if (expFieldA_int - expFieldB_int > 1024) {
        Flip_raise(FLIP_OVERFLOW);
        Flip_raise(FLIP_INEXACT);
        
        // TODO : this output can not contain direct_output_mask
        return (sgnA ^ sgnB) | 0x0000000000000600ull;
    };

    if (BisZero) {
        if (AisZero) {
            Flip_raise(FLIP_INVALID);
            return 0xffffffffffffffffull & direct_result_mask;
        }
        return (sgnA ^ sgnB) | 0x7ff0000000000000ull;
    }
    if (AisZero) {
        return (sgnA ^ sgnB) & direct_result_mask;
    };


    if (BisZero && !(AisInfOrNaN || AisZero)) Flip_raise(FLIP_DIV_BY_ZERO);
     ret = sgnB | (RexpField<<52) | (Inv << 43) | bitVector;
   return ret;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
#include <stdlib.h>
{
#ifdef TESTSOFTFLOAT
    // Level 2 is a bit too long...
    const char *param[5] = { "testsoftfloat", "-level1", "-tininessbefore", "int64_to_float64" };
    //
    testsoftfloat(4, param);
    //
    param[3] = "float64_to_int64";
    testsoftfloat(4, param);
    //
    param[3] = "float64_add";
    testsoftfloat(4, param);
    //
    param[3] = "float64_sub";
    testsoftfloat(4, param);
    //
    param[3] = "float64_mul";
    testsoftfloat(4, param);
    //
    param[3] = "float64_min";
    testsoftfloat(4, param);
    //
    param[3] = "float64_max";
    testsoftfloat(4, param);
    //
    param[0] = "IeeeCC754";
    param[1] = "-c";
    param[2] = "-d";
    param[3] = "IeeeCC754/testsets/add";
    IeeeCC754(4, param);
    //
    param[3] = "IeeeCC754/testsets/multiply";
    IeeeCC754(4, param);
    //
    param[3] = "IeeeCC754/testsets/rint64";
    IeeeCC754(4, param);
    //
    param[3] = "IeeeCC754/testsets/ruint64";
    IeeeCC754(4, param);
    //
    param[3] = "IeeeCC754/testsets/cint64";
    IeeeCC754(4, param);
    //
    param[3] = "IeeeCC754/testsets/cuint64";
    IeeeCC754(4, param);
#endif//TESTSOFTFLOAT
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Flip64 module.
 */
#define Flip64_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Flip64 module.
 */
#define Flip64_FINI()
#endif//$XCC__h

#if XCC__C
void testsoftfloat(int argc, const char **argv);
void IeeeCC754(int argc, const char **argv);
static void
Flip64_TEST(void)
{
#include "BSL/Flip64_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(Flip64_TEST);
  return 0;
}
#endif

