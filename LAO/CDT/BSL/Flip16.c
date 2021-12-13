#ifdef $XCC_h
/*
 * !!!!	Flip16.xcc
 *
 * Benjamin Orgogozo (benjamin.orgogozo@normalesup.org)
 * Benoit Dupont de Dinechin (benoit.dinechin@gmail.com)
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
 * @brief Arithmetic support for 16-bit floating-point.
 */
#endif//$XCC_h


#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/Flip16.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include "FTypes.h"
#endif//$XCC_h


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
#include <stdlib.h>
{
  // No test for now.
}
#endif//$XCC__c

/////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS
/////////////////////////////////////////////////////////////////////////
//



static bits16
Float_shift16RightJamming(bits16 a, int16_t count) {
    if ( count == 0 ) {
        return a;
    }
    else if ( count < 16 ) {
        return ( a>>count ) | ( (( a<<( ( - count ) & 15 ) ) & 0xffff) != 0 );
    }
    else {
        return ( a != 0 );
    }
}


#ifdef $XCC_h
/**
 * Returns the sign bit of a half
 */
static inline unsigned
Flip16_sign(flip16 a) {
    return (a >> 15) & 1;
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Returns the exponent part of a half
 */
static inline uint32_t
Flip16_exp(flip16 a) {
    return (a >> 10) & 0x0000001F;
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Returns the fractional part of a half
 */
static inline uint32_t
Flip16_frac(flip16 a) {
    return a & 0x000003FF;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if NaN, 0 if not.
 */
static inline int
Flip16_isZero(flip16 a) {
    return (Flip16_exp(a) == 0) && (Flip16_frac(a) == 0);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if NaN, 0 if not.
 */
static inline int
Flip16_isNaN(flip16 a) {
    return (((a >> 10) & 0x1F) == 0x1F) && (a & 0x3FF);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if signaling NaN, 0 if not.
 */
static inline int
Flip16_issNaN(flip16 a) {
    return Flip16_isNaN(a) && (!(Flip16_frac(a) >> 9));
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Returns quiet NaN corresponding to a 32 bits NaN
 */
flip16
Flip16_quiet_NaN_from_flip32_NaN(flip32 nan);
#endif//$XCC_h

flip16
Flip16_quiet_NaN_from_flip32_NaN(flip32 nan) {
    return Flip16_create(Flip32_sign(nan), 0x1F, 
        (1 << 9) | (0x1FF & (Flip32_frac(nan) >> 13)));
}

#ifdef $XCC_h
/**
 * Creates a flip16 from a sign, an exponent and a mantisse.
 */
static inline flip16
Flip16_create(int sign, int16_t exp, bits16 mant) {
    flip16 ret;
    // argument specificities
    // mantissa of new null number contains always an implicit one
    // and exp is always decremented by 1
    ret = (((bits16) sign) << 15) + (((bits16) exp) << 10) + mant;
    return ret;
}
#endif//$XCC_h


static flip16 Flip_roundAndPackFloat16( flag zSign, int16_t zExp, bits16 zSig, bits16 sticky ) {
    int8_t roundingMode;
    flag roundNearestEven;
    int8_t roundIncrement, roundBits;
    flag isTiny;
    flip16 ret;

    /** round/sticky/trailing length is 4 bits */

    roundingMode = Flip_rnd_mode;
    roundNearestEven = ( roundingMode == FLIP_RN );
    roundIncrement = 0x8;
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLIP_RZ ) {
            roundIncrement = 0;
        }
        else {
            // round up or down
            roundIncrement = 0xF;
            if ( zSign ) {
                if ( roundingMode == FLIP_RU ) roundIncrement = 0;
            }
            else {
                if ( roundingMode == FLIP_RD ) roundIncrement = 0;
            }
        }
    }
    // extra 7 last bits as sticky
    roundBits = (zSig + sticky) & 0xF;
    // if exp >= 253 overflow
    if ( 0x1D <= (bits16) zExp ) {
        if (    ( 0x1D < zExp )
             || (    ( zExp == 0x1D )
                  && ( (sbits16) ( zSig + roundIncrement ) < 0 ) )
           ) {
           //
           // overflow <=> zExp > 253 || (zExp == 253 and zSig + round overflows)
            Flip_raise( FLIP_OVERFLOW | FLIP_INEXACT );
            ret = Flip16_create( zSign, 0x1F, 0 );
            // setting omega by decrementing ret value when roundIncrement is not set
            ret -= ( roundIncrement == 0 );
            return ret;
        }
        if ( zExp < 0 ) {
            isTiny =
                   ( flip_detect_tininess == flip_tininess_before_rounding )
                || ( zExp < -1 )
                || ( zSig + roundIncrement < 0x8000 );
            zSig = Float_shift16RightJamming( zSig, - zExp);
            zExp = 0;
            roundBits = (zSig + sticky) & 0xF;
            if ( isTiny && roundBits ) Flip_raise( FLIP_UNDERFLOW );
        }
    }
    if ( (zSig & 0xF) || sticky ) Flip_raise(FLIP_INEXACT);
    zSig = ( zSig + (sticky && (roundingMode == FLIP_RU)) + roundIncrement )>>4;
//    zSig &= ~ ( ( ( roundBits ^ 0x40 ) == 0 ) & roundNearestEven );
    zSig &= ~ ( ( ( roundBits ^ 0x8 ) == 0 ) & roundNearestEven & !sticky);
//    if ( zSig == 0 ) zExp = 0;
    return Flip16_create( zSign, zExp, zSig );
}



/////////////////////////////////////////////////////////////////////////
// CONVERSION FUNCTIONS
/////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Converts a flip32 to a flip16.
 */
flip16
Flip16_fromFlip32(uint32_t a);
#endif//$XCC_h

// round nearest even
flip16
Flip16_fromFlip32(uint32_t a) {
    flag aSign;
    int16_t aExp;
    bits32 aSig;
    bits16 zSig;

    aSig = Flip32_frac( a );
    aExp = Flip32_exp( a );
    aSign = Flip32_sign( a );
    if (Flip32_issNaN(a)) {
        Flip_raise(FLIP_INVALID);
    }
    if (Flip32_isNaN(a)) {
        // We keep lowest bits of the payload for generating a quiet
        return Flip16_quiet_NaN_from_flip32_NaN(a);
    }
    if ( aExp == 0xFF ) {
        // infinity cases
        return Flip16_create( aSign, 0x1F, 0 );
    }
    aSig = Flip_ushift32RightJamming( aSig, 9);
    zSig = aSig;
    if ( aExp || zSig ) {
        zSig |= 0x4000;
        // fp16_exp = fp32_exp - fp32_bias + fp16_bias - 1 = fp32_exp - (127 - 15 + 1) = fp32_exp - 113 
        aExp -= 0x71;
    }
    // zSig is 01.a_field where 1 is the implicit one inserted by 
    // |= 0x4000
    return Flip_roundAndPackFloat16( aSign, aExp, zSig, 0 );
}


/////////////////////////////////////////////////////////////////////////
// ARITHMETIC FUNCTIONS
/////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Returns the absolute value of the given float
 */
static inline flip16
Flip16_abs(flip16 a) {
    return 0x7FFF & a;
}
#endif//$XCC_h

#ifdef $XCC_h
/** 
 * Returns the min between two half floats.
 */
flip16
Flip16_min(flip16 a, flip16 b);
#endif//$XCC_h

flip16
Flip16_min(flip16 a, flip16 b) {
    if (Flip16_isNaN(a)) {
        // Fixes #1557, we don't raise invalid anymore so we always
        // return the signaling NaN if any.
        if (Flip16_issNaN(a)) {
            return a;
        }
        if (Flip16_issNaN(b)) {
            return b;
        }
        if (Flip16_isNaN(b)) {
            return a;
        }
        else {
            return b;
        }
    }
    if (Flip16_isNaN(b)) {
        if (Flip16_issNaN(b)) {
            return b;
        }
        else {
            return a;
        }
    }
    // Issue #1329 We return -0 if comparison between two opposite 0.
    if (Flip16_isZero(a) && Flip16_isZero(b)) {
        if (Flip16_sign(a)) return a;
        else return b;
    }
    return LesserThan==Flip16_comp(a, b)? a: b;
}

#ifdef $XCC_h
/**
 * Negates the given half float. 
 */
static inline flip16
Flip16_neg(flip16 x) { 
    return 0x8000 ^ x;
}
#endif//$XCC_h

#ifdef $XCC_h
/** 
 * Returns the max between two half floats.
 */
flip16
Flip16_max(flip16 a, flip16 b);
#endif//$XCC_h

flip16
Flip16_max(flip16 a, flip16 b) {
    if (Flip16_isNaN(a)) {
        // Fixes #1557, we don't raise invalid anymore so we always
        // return the signaling NaN if any.
        if (Flip16_issNaN(a)) {
            return a;
        }
        if (Flip16_issNaN(b)) {
            return b;
        }
        if (Flip16_isNaN(b)) {
            return a;
        }
        else {
            return b;
        }
    }
    if (Flip16_isNaN(b)) {
        if (Flip16_issNaN(b)) {
            return b;
        }
        else {
            return a;
        }
    }
    // Issue #1329  We return 0 if comparison between two opposite 0.
    if (Flip16_isZero(a) && Flip16_isZero(b)) {
        if (Flip16_sign(a)) return b;
        else return a;
    }
    return GreaterThan==Flip16_comp(a, b)? a: b;
}

#ifdef $XCC_h
/* Compares 2 half float
 * Returns:
 * unordered, if at least one is NaN
 * lesserThan, if a < b
 * equal, if both or equal (-0 == +0)
 * greaterThan, if a > b
 */
Flip_compare_t
Flip16_comp(flip16 a, flip16 b);
#endif//$XCC_h

Flip_compare_t
Flip16_comp(flip16 a, flip16 b) {
    // If at least one is a NaN
    if (Flip16_isNaN(a) || Flip16_isNaN(b)) {
        return Unordered;
    }
    else {
        // If exactly one is negative
        if (Flip16_sign(a) != Flip16_sign(b)) {
            // Handle -0/+0 comparison
            if ((Flip16_exp(a) == 0) &&
                (Flip16_frac(a) == 0) &&
                (Flip16_exp(b) == 0) &&
                (Flip16_frac(b) == 0)) {
                return Equal;
            }
            else {
                if (Flip16_sign(a) > Flip16_sign(b)) {
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
                if (Flip16_sign(a)) {
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
                    if (Flip16_sign(a)) {
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


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Flip16 module.
 */
#define Flip16_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Flip16 module.
 */
#define Flip16_FINI()
#endif//$XCC__h

#if XCC__C
void testsoftfloat(int argc, const char **argv);
void IeeeCC754(int argc, const char **argv);
static void
Flip16_TEST(void)
{
#include "BSL/Flip16_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(Flip16_TEST);
  return 0;
}
#endif

