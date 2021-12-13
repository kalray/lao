#ifdef $XCC_h
/*
 * !!!!	Flip32.xcc
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
 * @brief Arithmetic support for 32-bit floating-point.
 */
#endif//$XCC_h


#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/Flip32.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#include "FTypes.h"
#endif//$XCC_h


/////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS
/////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Set the rounding mode of the library. 
 */
static inline void flip_set_rnd_mode(Flip_rnd_mode_t rnd) {
  // FIXME strict value checking (ifdef condition testing)
  Flip_rnd_mode = rnd;
}
static inline flip_exception_t flip_get_exceptions() {
    return Flip_status;
}

static inline void flip_reset_exceptions() {
    Flip_status = FLIP_EXPT_OK;
}


#endif//$XCC_h

#ifdef $XCC_h
/*----------------------------------------------------------------------------
| Raises the exceptions specified by `flags'.  Floating-point traps can be
| defined here if desired.  It is currently not possible for such a trap
| to substitute a result value.  If traps are not implemented, this routine
| should be simply `float_exception_flags |= flags;'.
*----------------------------------------------------------------------------*/
void Flip_raise( int8_t flags );
#endif//$XCC_h

void Flip_raise( int8_t flags ) {
    Flip_status |= flags;
}


static bits32
Float_shift32RightJamming(bits32 a, int16_t count) {
    if ( count == 0 ) {
        return a;
    }
    else if ( count < 32 ) {
        return ( a>>count ) | ( ( a<<( ( - count ) & 31 ) ) != 0 );
    }
    else {
        return ( a != 0 );
    }
}


#ifdef $XCC_h
/**
 * Returns the sign bit
 */
static inline unsigned
Flip32_sign(flip32 a) {
    return (a >> 31) & 1;
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Returns the exponent part of a float
 */
static inline uint32_t
Flip32_exp(flip32 a) {
    return (a >> 23) & 0x000000FF;
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Returns the fractional part of a float
 */
static inline uint32_t
Flip32_frac(flip32 a) {
    return a & 0x007FFFFF;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if NaN, 0 if not.
 */
static inline int
Flip32_isZero(flip32 a) {
    return (Flip32_exp(a) == 0) && (Flip32_frac(a) == 0);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if NaN, 0 if not.
 */
static inline int
Flip32_isNaN(flip32 a) {
    return (((a << 1) >> 24) == 0xFF) && (a << 9);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if infinity, 0 if not.
 */
static inline int
Flip32_isInf(flip32 a) {
    return (((a << 1) >> 24) == 0xFF) && (!(a << 9));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if signaling NaN, 0 if not.
 */
static inline int
Flip32_issNaN(flip32 a) {
    return Flip32_isNaN(a) && (!(Flip32_frac(a) >> 22));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns 1 if quiet NaN, 0 if not.
 */
static inline int
Flip32_isqNaN(flip32 a) {
    return Flip32_isNaN(a) && (Flip32_frac(a) >> 22);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Creates a flip32 from a sign, an exponent and a mantisse.
 */
static inline flip32
Flip32_create(int sign, int16_t exp, bits32 mant) {
    flip32 ret;
    ret = (((bits32) sign) << 31) + (((bits32) exp) << 23) + mant;
    return ret;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns a quiet NaN with a given payload.
 */
static inline flip32
Flip32_quiet_NaN() {
    return Flip32_create(1, 0xFF, 0x7FFFFF);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns the quiet NaN corresponding to the given signaling NaN
 */
static inline flip32
Flip32_quiet_from_sNaN(flip32 nan) {
    return nan | (1 << 22);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Returns quiet NaN corresponding to a 64 bits NaN
 */
flip32
Flip32_quiet_NaN_from_flip64_NaN(flip64 nan);
#endif//$XCC_h

flip32
Flip32_quiet_NaN_from_flip64_NaN(flip64 nan) {
    return Flip32_create(Flip64_sign(nan), 0xFF, 
        (1 << 22) | (0x7FFFFF & Flip64_frac(nan)));
}


/** 
 * Returns quiet NaN corresponding to a 16 bits NaN
 */
#ifdef $XCC_h
/**
 * Returns quiet NaN corresponding to a 32 bits NaN
 */
flip32
Flip32_quiet_NaN_from_flip16_NaN(flip16 nan);
#endif//$XCC_h

flip32
Flip32_quiet_NaN_from_flip16_NaN(flip16 nan) {
    return Flip32_create(Flip16_sign(nan), 0xFF, 
        (1ULL << 22) | ((uint32_t) Flip16_frac(nan) << 13));
}


static bits32
Flip32_round64AndNormMant32(bits64 a) {
    bits64 R, C, P;

    R = a & 0x0000004000000000LL;
    if (R) {
        P = a & 0x0000008000000000LL;
        C = a & 0x0000003fffffffffLL;
        if (P||C) return  (bits32) ( a >> 39 ) + 1;
    }
    return (bits32) (a >> 39);
}


#ifdef $XCC_h
/**
 * Negates the given float. 
 */
static inline flip32
Flip32_neg(flip32 x) { 
    return 0x80000000 ^ x;
}
#endif//$XCC_h


#ifdef $XCC_h
/* Compares 2 float
 * Returns:
 * unordered, if at least one is NaN
 * lesserThan, if a < b
 * equal, if both or equal (-0 == +0)
 * greaterThan, if a > b
 */
Flip_compare_t
Flip32_comp(flip32 a, flip32 b);
#endif//$XCC_h

Flip_compare_t
Flip32_comp(flip32 a, flip32 b) {
    // If at least one is a NaN
    if (Flip32_isNaN(a) || Flip32_isNaN(b)) {
        return Unordered;
    }
    else {
        // If exactly one is negative
        if (Flip32_sign(a) != Flip32_sign(b)) {
            // Handle -0/+0 comparison
            if ((Flip32_exp(a) == 0) &&
                (Flip32_frac(a) == 0) &&
                (Flip32_exp(b) == 0) &&
                (Flip32_frac(b) == 0)) {
                return Equal;
            }
            else {
                if (Flip32_sign(a) > Flip32_sign(b)) {
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
                if (Flip32_sign(a)) {
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
                    if (Flip32_sign(a)) {
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

/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and significand `zSig', and returns the proper single-precision floating-
| point value corresponding to the abstract input.  Ordinarily, the abstract
| value is simply rounded and packed into the single-precision format, with
| the inexact exception raised if the abstract input cannot be represented
| exactly.  However, if the abstract value is too large, the overflow and
| inexact exceptions are raised and an infinity or maximal finite value is
| returned.  If the abstract value is too small, the input value is rounded to
| a subnormal number, and the underflow and inexact exceptions are raised if
| the abstract input cannot be represented exactly as a subnormal single-
| precision floating-point number.
|     The input significand `zSig' has its binary point between bits 30
| and 29, which is 7 bits to the left of the usual location.  This shifted
| significand must be normalized or smaller.  If `zSig' is not normalized,
| `zExp' must be 0; in that case, the result returned is a subnormal number,
| and it must not require rounding.  In the usual case that `zSig' is
| normalized, `zExp' must be 1 less than the ``true'' floating-point exponent.
| The handling of underflow and overflow follows the IEC/IEEE Standard for
| Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
static flip32 Flip_roundAndPackFloat32( flag zSign, int16_t zExp, bits32 zSig, bits32 sticky ) {
    int8_t roundingMode;
    flag roundNearestEven;
    int8_t roundIncrement, roundBits;
    flag isTiny;
    flip32 ret;

    roundingMode = Flip_rnd_mode;
    roundNearestEven = ( roundingMode == FLIP_RN );
    roundIncrement = 0x40;
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLIP_RZ ) {
            roundIncrement = 0;
        }
        else {
            roundIncrement = 0x7F;
            if ( zSign ) {
                if ( roundingMode == FLIP_RU ) roundIncrement = 0;
            }
            else {
                if ( roundingMode == FLIP_RD ) roundIncrement = 0;
            }
        }
    }
    roundBits = (zSig + sticky) & 0x7F;
    if ( 0xFD <= (bits16) zExp ) {
        if (    ( 0xFD < zExp )
             || (    ( zExp == 0xFD )
                  && ( (sbits32) ( zSig + roundIncrement ) < 0 ) )
           ) {
            Flip_raise( FLIP_OVERFLOW | FLIP_INEXACT );
            ret = Flip32_create( zSign, 0xFF, 0 );
            ret -= ( roundIncrement == 0 );
            return ret;
        }
        if ( zExp < 0 ) {
            isTiny =
                   ( flip_detect_tininess == flip_tininess_before_rounding )
                || ( zExp < -1 )
                || ( zSig + roundIncrement < 0x80000000 );
            zSig = Float_shift32RightJamming( zSig, - zExp);
            zExp = 0;
            roundBits = (zSig + sticky) & 0x7F;
            if ( isTiny && roundBits ) Flip_raise( FLIP_UNDERFLOW );
        }
    }
    if ( (zSig & 0x7F) || sticky ) Flip_raise(FLIP_INEXACT);
    zSig = ( zSig + (sticky && (roundingMode == FLIP_RU)) + roundIncrement )>>7;
//    zSig &= ~ ( ( ( roundBits ^ 0x40 ) == 0 ) & roundNearestEven );
    zSig &= ~ ( ( ( roundBits ^ 0x40 ) == 0 ) & roundNearestEven & !sticky);
//    if ( zSig == 0 ) zExp = 0;
    return Flip32_create( zSign, zExp, zSig );
}

static float32 Flip32_normalizeRoundAndPack(flag zSign, 
                                            int16_t zExp,
                                            bits32 zSig) {
    int8_t shiftCount;
    shiftCount = __builtin_clz(zSig) - 1;
    return Flip_roundAndPackFloat32( zSign, zExp - shiftCount, zSig<<shiftCount, 0);
}


/*----------------------------------------------------------------------------
| Right shift a by count bits and set the LSB as a sticky flag
| of bits discarded during the shift
*----------------------------------------------------------------------------*/
#ifdef $XCC_h
bits32
Flip_ushift32RightJamming(bits32 a, int16_t count);
#endif//$XCC_h

bits32
Flip_ushift32RightJamming(bits32 a, int16_t count)
{
    if ( count == 0 ) {
        return a;
    }
    else if ( count < 32 ) {
        return ( a>>count ) | ( ( a<<( ( - count ) & 31 ) ) != 0 );
    }
    else {
        return ( a != 0 );
    }
}

#ifdef $XCC_h
// Used by Arenaire FPFMA_c implementation.
/** use for unpacking operands */
void unpack32(uint32_t op, uint32_t* sgn, uint32_t* mant, uint32_t* exp, uint32_t* isNormal, uint32_t* isNaN, uint32_t* isInf, uint32_t* isZero);
#endif//$XCC_h

// Used by Arenaire FPFMA_c implementation.
/** use for unpacking operands */
void unpack32(uint32_t op, uint32_t* sgn, uint32_t* mant, uint32_t* exp, uint32_t* isNormal, uint32_t* isNaN, uint32_t* isInf, uint32_t* isZero) {
    *sgn = op >> 31;
    *exp = (op & (0x7f800000)) >> 23;
    *mant = (op << 9) >> 9;
    *isInf = (*exp == 0xff) && (*mant == 0);
    *isNaN = (*exp == 0xff) && !(*isInf);
    *isNormal = (*exp != 0xff) && (*exp != 0x00);
    *isZero = (*exp == 0x00) && (*mant == 0);
}

#ifdef $XCC_h
// Used by Arenaire FPFMA_c implementation.
/* leading zero counter */
int LZC32(uint32_t op, int count, int* correction);
#endif//$XCC_h


// Used by Arenaire FPFMA_c implementation.
/* leading zero counter */
int LZC32(uint32_t op, int count, int* correction) {
    // we made a leading zero count on op[count -- 1], with correction
    for (int i = 1; i <= count;i++) {
        uint32_t tmp = op >> (count - i);
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
flip32 FPFMA_c(uint32_t A, uint32_t B, uint32_t C, Flip_rnd_mode_t roundMode, 
                uint32_t negateAB, uint32_t negateC) {
	uint32_t sgnA, sgnB, sgnC;
	uint32_t mantA, mantB, mantC;
	uint32_t expA, expB, expC;
	// flags
	uint32_t AisNormal, AisNaN, AisInf, AisZero;
	uint32_t BisNormal, BisNaN, BisInf, BisZero;
	uint32_t CisNormal, CisNaN, CisInf, CisZero;

    uint32_t effectiveOp;
    uint32_t infSgn;

    int correctExpA, correctExpB, correctExpC;
    int ea, eb, ec;

    uint64_t mult;
    int eMult;

	uint32_t resultSgn; // TODO : to be defined
	int diff, expOrigin = 0;

    Int128_ tmpC;
	Int128_ sum;

    uint64_t sgnMask, sumSgn, sumPreCarry;
	int lzcSUM;
	Int128_ normResult;
	int newExp;
	uint32_t prodSgn;

	Int128_ preResult_128, preRoundBit, preStickyBit;
	uint32_t preResult, roundBit, stickyBit;

	// TODO : determine if result is subnormal or not and delete implicit one
	uint32_t uNewExp;
	bool infByOverflow;

	Int128_ alignedAB;

	int biasedResultExp;
	Int128_ delCarryMask;
	Int128_ shiftedC;

	uint32_t preRoundedResult, roundedResult;
    uint32_t lastBitOdd;

    uNewExp = 0;
    roundedResult = 0;

	// unpacking and setting flags
	unpack32(A, &sgnA, &mantA, &expA, &AisNormal, &AisNaN, &AisInf, &AisZero);
	unpack32(B, &sgnB, &mantB, &expB, &BisNormal, &BisNaN, &BisInf, &BisZero);
	unpack32(C, &sgnC, &mantC, &expC, &CisNormal, &CisNaN, &CisInf, &CisZero);

	// effective operation : addition (0) or substraction (1)
	effectiveOp = (((negateAB ^ sgnA) ^ sgnB) ^ (sgnC ^ negateC)); 


    /////////////////////////////////
    //  NAN HANDLING
    /////////////////////////////////
    // If one of the operand is a signaling NaN, we generate an
    // exception.
    if (Flip32_issNaN(A) || Flip32_issNaN(B) || Flip32_issNaN(C)) {
        Flip_raise(FLIP_INVALID);
    }
    // If one of the operand is already a NaN, we return a quiet NaN
    // (same sign, same payload).
    if (AisNaN) return Flip32_quiet_from_sNaN(A);
    if (BisNaN) return Flip32_quiet_from_sNaN(B);
    if (CisNaN) return Flip32_quiet_from_sNaN(C);

    // We check if we *generate* NaN
	if ((((AisInf || BisInf) && CisInf && effectiveOp)) || 
        (AisZero && BisInf) || (BisZero && AisInf)) {
        Flip_raise(FLIP_INVALID);
        return Flip32_quiet_NaN();
	}
    /////////////////////////////////

    /////////////////////////////////
    // INFINITY
    /////////////////////////////////
    // In NaN handling above we checked the case when C is also infinity
    // (which could generate NaN) or the case 0 * infinity (which also
    // generates a NaN).
    if (AisInf || BisInf) {
		// inf in mult
		infSgn = sgnA ^ sgnB ^ negateAB;	
        return Flip32_create(infSgn, 0xFF, 0);
	} 
    else if (CisInf) {
		// inf in addend
		infSgn = sgnC ^  negateC;	
        return Flip32_create(infSgn, 0xFF, 0);
	}
    /////////////////////////////////

	correctExpA = 0;
	correctExpB = 0;
	correctExpC = 0;
	// computing real mantissa by adding a leading one if operand was normal
	// and shift left (with exponent correction) if operand was subnormal
	mantA = AisNormal ? (1 << 23) | mantA : (mantA << LZC32(mantA,23,&correctExpA));
	mantB = BisNormal ? (1 << 23) | mantB : (mantB << LZC32(mantB,23,&correctExpB));
	mantC = CisNormal ? (1 << 23) | mantC : (mantC << LZC32(mantC,23,&correctExpC));
	// exponent correction
	ea =  AisNormal ? expA - 127 : - 126 - correctExpA;
	eb =  BisNormal ? expB - 127 : - 126 - correctExpB;
	ec =  CisNormal ? expC - 127 : - 126 - correctExpC;


	mult = ((uint64_t) mantA) * ((uint64_t) mantB); 
	eMult = ea + eb;
	// correction of Emult if A or B is zero
	if (AisZero || BisZero) eMult = -256;


	// SIGN COMPUTATION
	resultSgn = sgnC ^ negateC;

	// exponent diff computation
	// WORK IN PROGRESS
    diff = ec-eMult;
	alignedAB = Int128_fromUInt64(mult);
	// 27 
	alignedAB = Int128_shl(alignedAB, Int128_fromUInt64(27));
	expOrigin = 0;
	if (diff >= 0) {
		int normShift;
		if (diff > 30) {
			normShift = 30;
			expOrigin = ec + 24;
		} else {
			normShift = diff;
			expOrigin = eMult + 54;
		}
		tmpC = Int128_fromUInt64(mantC);
		shiftedC = Int128_shl(tmpC, Int128_fromUInt64(50 + normShift));
	} else {
		int normShift;
		if (diff < -50) normShift = 50;
		else normShift = -diff;
		tmpC = Int128_fromUInt64(mantC);
		shiftedC = Int128_shl(tmpC, Int128_fromUInt64(50 - normShift));
		expOrigin = eMult + 54;
	};	
		
	if (effectiveOp == 0) sum = Int128_add(shiftedC, alignedAB);
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
		newExp = -127;
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
		if (newExp + 127 <= 0) {
			// subnormal case
            // We don't shift too much: we don't want to looze sticky
            // bits for rouding. 30 bits is sufficient here.
            int shift = -(newExp + 127)+1>30?30:-(newExp+127)+1;
			normResult = Int128_shru(normResult, Int128_fromUInt64(shift));
		};
	}
	preResult_128 = Int128_shru(normResult, Int128_fromUInt64(128-24));
	preResult = Int128_toUInt32(preResult_128);


	preRoundBit = Int128_shru(Int128_shl(normResult, Int128_fromUInt64(24)), 
                    Int128_fromUInt64(127));
	roundBit = Int128_toUInt32(preRoundBit);


	preStickyBit = Int128_shl(normResult, Int128_fromUInt64(25));
	stickyBit = !((preStickyBit.dwords[0] == 0) & (preStickyBit.dwords[1] == 0));

    if (stickyBit || roundBit) Flip_raise(FLIP_INEXACT);

	// TODO : determine if result is subnormal or not and delete implicit one
	infByOverflow = false;

	biasedResultExp = newExp + 127;
    if (flip_detect_tininess ==  flip_tininess_before_rounding) {
        if (biasedResultExp <= 0) {
            // We raise flags only if the result is not exactly 0 and is not exact.
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
	} else if (biasedResultExp >= 255) {
		// overflow
		if ((roundMode == FLIP_RD && !resultSgn) || (roundMode == FLIP_RU && resultSgn) || (roundMode == FLIP_RZ)) {
            Flip_raise(FLIP_OVERFLOW | FLIP_INEXACT);
			uNewExp = 0xfe;
			preResult = 0x7fffff;
		} else {
			uNewExp = 0xFF;
			preResult = 0;
            infByOverflow = true;
        }
	} else {
		// delete first one
		preResult = preResult & 0x007fffff;
		uNewExp = biasedResultExp;
	}

	preRoundedResult = (resultSgn << 31) | (uNewExp << 23) | (preResult);
	lastBitOdd = (preRoundedResult << 31);

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
                    if (Flip32_isInf(roundedResult)) Flip_raise(FLIP_OVERFLOW);
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

 	if (Flip32_isInf(roundedResult)) Flip_raise(FLIP_OVERFLOW | FLIP_INEXACT);
    return Flip32_create(roundedResult >> 31, 
                            (roundedResult & 0x7F800000) >> 23,
                            roundedResult & 0x007FFFFF);
}


/**
 * Implementation of FDMA
 * We don't use it directly outside LAO but through wrappers (fdma, fdms)
 */
static
flip32 FPFDMA_c(uint32_t A, uint32_t B, uint32_t C, uint32_t D, 
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

	uint32_t resultSgn; // TODO : to be defined
	int diff, expOrigin = 0;

    Int128_ tmpCD;
	Int128_ sum;

	int lzcSUM;
	Int128_ normResult;
	int newExp;
	uint32_t prodSgnAB;
	uint32_t prodSgnCD;

	Int128_ preResult_128, preRoundBit, preStickyBit;
	uint32_t preResult, roundBit, stickyBit;

	// TODO : determine if result is subnormal or not and delete implicit one
	uint32_t uNewExp;
	bool infByOverflow;

	Int128_ alignedAB;
	Int128_ alignedCD;
    int normShiftCD, normShiftAB;

	int biasedResultExp;

	uint32_t preRoundedResult, roundedResult;
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
    if (AisNaN) return Flip32_quiet_from_sNaN(A);
    if (BisNaN) return Flip32_quiet_from_sNaN(B);
    if (CisNaN) return Flip32_quiet_from_sNaN(C);
    if (DisNaN) return Flip32_quiet_from_sNaN(D);

    // We check if we *generate* NaN
	if (((AisInf||BisInf) && (CisInf||DisInf) && (effectiveOp^sgnA^sgnB)) ||
        (AisZero && BisInf) || (BisZero && AisInf) ||
        (CisZero && DisInf) || (DisZero && CisInf)) {
        Flip_raise(FLIP_INVALID);
        return Flip32_quiet_NaN();
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
        return Flip32_create(infSgn, 0xFF, 0);
	} 
    else if (CisInf || DisInf) {
		// inf in addend
		infSgn = sgnC ^ sgnD ^ negateCD;	
        return Flip32_create(infSgn, 0xFF, 0);
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
		newExp = -127;
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
		if (newExp + 127 <= 0) {
			// subnormal case
            // We don't shift too much: we don't want to looze sticky
            // bits for rouding. 30 bits is sufficient here.
            int shift = -(newExp + 127)+1>30?30:-(newExp+127)+1;
			normResult = Int128_shru(normResult, Int128_fromUInt64(shift));
		};
	}
	preResult_128 = Int128_shru(normResult, Int128_fromUInt64(128-24));
	preResult = Int128_toUInt32(preResult_128);


	preRoundBit = Int128_shru(Int128_shl(normResult, Int128_fromUInt64(24)), 
                    Int128_fromUInt64(127));
	roundBit = Int128_toUInt32(preRoundBit);


	preStickyBit = Int128_shl(normResult, Int128_fromUInt64(25));
	stickyBit = !((preStickyBit.dwords[0] == 0) & (preStickyBit.dwords[1] == 0));

    if (stickyBit || roundBit) Flip_raise(FLIP_INEXACT);

	// TODO : determine if result is subnormal or not and delete implicit one
	infByOverflow = false;

	biasedResultExp = newExp + 127;

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
	} else if (biasedResultExp >= 255) {
		// overflow
		if ((roundMode == FLIP_RD && !resultSgn) || (roundMode == FLIP_RU && resultSgn) || (roundMode == FLIP_RZ)) {
            Flip_raise(FLIP_OVERFLOW | FLIP_INEXACT);
			uNewExp = 0xfe;
			preResult = 0x7fffff;
		} else {
			uNewExp = 0xFF;
			preResult = 0;
            infByOverflow = true;
        }
	} else {
		// delete first one
		preResult = preResult & 0x007fffff;
		uNewExp = biasedResultExp;
	}

	preRoundedResult = (resultSgn << 31) | (uNewExp << 23) | (preResult);
	lastBitOdd = (preRoundedResult << 31);
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
                    if (Flip32_isInf(roundedResult)) Flip_raise(FLIP_OVERFLOW);
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

    if (Flip32_isInf(roundedResult)) Flip_raise(FLIP_OVERFLOW | FLIP_INEXACT);

    return Flip32_create(roundedResult >> 31, 
                            (roundedResult & 0x7F800000) >> 23,
                            roundedResult & 0x007FFFFF);
}



/*----------------------------------------------------------------------------
| Takes a 64-bit fixed-point value `absZ' with binary point between bits 6
| and 7, and returns the properly rounded 32-bit integer corresponding to the
| input.  If `zSign' is 1, the input is negated before being converted to an
| integer.  Bit 63 of `absZ' must be zero.  Ordinarily, the fixed-point input
| is simply rounded to an integer, with the inexact exception raised if the
| input cannot be represented exactly as an integer.  However, if the fixed-
| point input is too large, the invalid exception is raised and the largest
| positive or negative integer is returned.
*----------------------------------------------------------------------------*/
static int32_t Flip32_roundAndPackInt( flag zSign, bits64 absZ )
{
    int8_t roundingMode;
    flag roundNearestEven;
    int8_t roundIncrement, roundBits;
    int32_t z;

    roundingMode = Flip_rnd_mode;
    roundNearestEven = ( roundingMode == FLIP_RN );
    roundIncrement = 0x40;
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLIP_RZ ) {
            roundIncrement = 0;
        }
        else {
            roundIncrement = 0x7F;
            if ( zSign ) {
                if ( roundingMode == FLIP_RU ) roundIncrement = 0;
            }
            else {
                if ( roundingMode == FLIP_RD ) roundIncrement = 0;
            }
        }
    }
    roundBits = absZ & 0x7F;
    absZ = ( absZ + roundIncrement )>>7;
    absZ &= ~ ( ( ( roundBits ^ 0x40 ) == 0 ) & roundNearestEven );
    z = absZ;
    if ( zSign ) z = - z;
    if ( ( absZ>>32 ) || ( z && ( ( z < 0 ) ^ zSign ) ) ) {
        Flip_raise( FLIP_INVALID );
        return zSign ? (sbits32) 0x80000000 : 0x7FFFFFFF;
    }
    if ( roundBits ) Flip_status |= FLIP_INEXACT;
    return z;
}

/*----------------------------------------------------------------------------
| Takes a 64-bit fixed-point value `absZ' with binary point between bits 6
| and 7, and returns the properly rounded 32-bit unsigned integer corresponding 
| to the input.  
| If `zSign' is 1, the input is negated before being converted to an
| integer.  Bit 63 of `absZ' must be zero.  Ordinarily, the fixed-point input
| is simply rounded to an integer, with the inexact exception raised if the
| input cannot be represented exactly as an integer.  However, if the fixed-
| point input is too large, the invalid exception is raised and the largest
| positive or negative integer is returned.
*----------------------------------------------------------------------------*/
static uint32_t Flip32_roundAndPackUint(bits64 absZ )
{
    int8_t roundingMode;
    flag roundNearestEven;
    int8_t roundIncrement, roundBits;
    uint32_t z;

    roundingMode = Flip_rnd_mode;
    roundNearestEven = ( roundingMode == FLIP_RN );
    roundIncrement = 0x40;
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLIP_RZ ) {
            roundIncrement = 0;
        }
        else {
            roundIncrement = 0x7F;
            if ( roundingMode == FLIP_RD ) roundIncrement = 0;
        }
    }
    roundBits = absZ & 0x7F;
    absZ = ( absZ + roundIncrement )>>7;
    absZ &= ~ ( ( ( roundBits ^ 0x40 ) == 0 ) & roundNearestEven );
    z = absZ;
    if ( absZ>>32 ) {
        Flip_raise( FLIP_INVALID );
        return 0xFFFFFFFF;
    }
    if ( roundBits ) Flip_status |= FLIP_INEXACT;
    return z;
}




/////////////////////////////////////////////////////////////////////////
// CONVERSION FUNCTIONS
/////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Converts a flip64 to a flip32.
 */
flip32
Flip32_fromFlip64(uint64_t a);
#endif//$XCC_h

// round nearest even
flip32
Flip32_fromFlip64(uint64_t a) {
    flag aSign;
    int16_t aExp;
    bits64 aSig;
    bits32 zSig;

    aSig = Flip64_frac( a );
    aExp = Flip64_exp( a );
    aSign = Flip64_sign( a );
    if (Flip64_issNaN(a)) {
        Flip_raise(FLIP_INVALID);
    }
    if (Flip64_isNaN(a)) {
        // We keep lowest bits of the payload for generating a quiet
        return Flip32_quiet_NaN_from_flip64_NaN(a);
    }
    if ( aExp == 0x7FF ) {
        return Flip32_create( aSign, 0xFF, 0 );
    }
    // aSig >> 22 
    aSig = Flip_ushift64RightJamming( aSig, 22);
    zSig = aSig;
    if ( aExp || zSig ) {
        zSig |= 0x40000000;
        aExp -= 0x381;
    }
    return Flip_roundAndPackFloat32( aSign, aExp, zSig, 0 );
}


static void Flip16_normalizeSubnormal(bits16 aSig, int16_t *zExpPtr, bits16 *zSigPtr)
{
    int8_t shiftCount;
    shiftCount = __builtin_clz(aSig) - 5;
    *zSigPtr = aSig << (shiftCount - 16);
    *zExpPtr = 1 - (shiftCount - 16);
}


#ifdef $XCC_h
// Converts a flip16 to a flip32.
flip32
Flip32_fromFlip16(flip16 r0);
#endif//$XCC_h

flip32
Flip32_fromFlip16(flip16 r0) {
    flag r0Sign;
    int16_t r0Exp;
    bits16 r0Mant;
    r0Mant = Flip16_frac(r0);
    r0Exp = Flip16_exp(r0);
    r0Sign = Flip16_sign(r0);
    if (Flip16_issNaN(r0)) {
        Flip_raise(FLIP_INVALID);
    }
    if (Flip16_isNaN(r0)) {
        return Flip32_quiet_NaN_from_flip16_NaN(r0);
    }
    if (r0Exp == 0x1F) {
      return Flip32_create(r0Sign, 0xFF, 0);
    }
    if (r0Exp == 0) {
      if (r0Mant == 0) return Flip32_create( r0Sign, 0, 0);
      Flip16_normalizeSubnormal(r0Mant, &r0Exp, &r0Mant);
      --r0Exp;
    }
    return Flip32_create(r0Sign, r0Exp + 112, ((bits32) r0Mant) << 13);
}


#ifdef $XCC_h
/**
 * Converts an int32_t to a flip32.
 */
flip32
Flip32_fromInt32(int32_t a);
#endif//$XCC_h

flip32
Flip32_fromInt32(int32_t a) {
  flip32 ret;
    flag zSign;
    if ( a == 0 ) { return 0; }
    if ( a == (sbits32) 0x80000000 ) return Flip32_create( 1, 0x9E, 0 );
    zSign = ( a < 0 );
    return Flip32_normalizeRoundAndPack( zSign, 0x9C, zSign ? - a : a );
}



#ifdef $XCC_h
/**
 * Converts an uint32_t to a flip32.
 */
flip32
Flip32_fromUint32(uint32_t r0);
#endif//$XCC_h

flip32 Flip32_fromUint32(uint32_t r0) {
    int8_t shiftCount;
    if (r0 == 0) {return Flip32_create(0, 0, 0);}
    if (r0 == 0x80000000) {return Flip32_create(0, 0x9E, 0);}
    shiftCount = __builtin_clz(r0);
    if (shiftCount == 0) {
        return Flip_roundAndPackFloat32(0,  0x9E, (r0 & 0x7FFFFFFF) >> 1, r0 & 1);
    }
    else {
        return Flip_roundAndPackFloat32(0,  0x9C - (shiftCount - 1), r0 << (shiftCount - 1), 0);
    }
}


#ifdef $XCC_h
/**
 * Converts a flip64 to a flip32.
 */
flip32
Flip32_fromInt64(int64_t r0);
#endif//$XCC_h

// round nearest even
flip32
Flip32_fromInt64(int64_t r0) {
  flag roSign;
  int8_t shift;
  bits64 r0Mant;
  bits32 roMant;
  int16_t roExp;
  flip32 ret;
  roExp = 189;
  if ( r0 == 0 ) { return 0; }
  if ( r0 == 0x8000000000000000LL ) return Flip32_create( 1, 190, 0 );
  roSign = ( r0 < 0 ) ;
  r0Mant = roSign ? -r0 : r0 ;
  shift = Flip64_countLeadingZeros(r0Mant) - 1;
  r0Mant <<= shift;
  roMant = Flip32_round64AndNormMant32( r0Mant );
  if ( roMant & 0xff000000 ) roExp++ ;
  roMant &= 0x007fffff;
  return Flip32_create( roSign, roExp - shift, roMant );
}


#ifdef $XCC_h
/**
 * Converts a flip32 to an uint32_t.
 */
uint32_t
Flip32_toUint32(flip32 a);
#endif//$XCC_h

uint32_t
Flip32_toUint32(flip32 a)
{
    flag aSign;
    int16_t aExp, shiftCount;
    bits32 aSig;
    bits64 aSig64;

    aSig = Flip32_frac( a );
    aExp = Flip32_exp( a );
    if ( aExp ) aSig |= 0x00800000;
    shiftCount = 0xAF - aExp;
    aSig64 = aSig;
    aSig64 <<= 32;
    if ( 0 < shiftCount ) aSig64 = Flip_shift64RightJamming(aSig64, shiftCount);
    return  Flip32_roundAndPackUint(aSig64 );
}



#ifdef $XCC_h
/**
 * Converts a flip32 to an int32_t.
 */
int32_t
Flip32_toInt32(flip32 r0);
#endif//$XCC_h

int32_t
Flip32_toInt32(flip32 a)
{
    flag aSign;
    int16_t aExp, shiftCount;
    bits32 aSig;
    bits64 aSig64;

    aSig = Flip32_frac( a );
    aExp = Flip32_exp( a );
    aSign = Flip32_sign( a );
    if ( ( aExp == 0xFF ) && aSig ) aSign = 0;
    if ( aExp ) aSig |= 0x00800000;
    shiftCount = 0xAF - aExp;
    aSig64 = aSig;
    aSig64 <<= 32;
    if ( 0 < shiftCount ) aSig64 = Flip_shift64RightJamming(aSig64, shiftCount);
    return  Flip32_roundAndPackInt( aSign, aSig64 );
}

#ifdef $XCC_h
/**
 * \fn flip32 Flip32_fromFrac(flip32 x, uint32_t y);
 * \brief Takes a float and an integer and returns the rounded result of
 * x divided by 1 << y.
 * @param[in] x The float to divide
 * @param[in] y The amount of shift by which x must be divided.
 * @return flip32 The float representing the correct rounded value of x/(1<<y).
 */
flip32
Flip32_fromFrac(flip32 x, uint32_t y);
#endif//$XCC_h

flip32
Flip32_fromFrac(flip32 x, uint32_t y)
{ 
    bits32 aSig = Flip32_frac( x );
    bits32 aExp = Flip32_exp( x );
    bits32 aSign = Flip32_sign( x );
    if (y <= aExp) {
        return Flip32_create(aSign, aExp - y, aSig);
    }
    else {
        if (aSig & ((1 << (y - aExp)) - 1)) {
            // If we loose digits when shifting to the right, it means
            // that the result will be inexact and an underflow.
            Flip_raise(FLIP_INEXACT | FLIP_UNDERFLOW);
        }
        return Flip_roundAndPackFloat32(aSign, 0, aSig >> (y - aExp), 0);
    }
}

#ifdef $XCC_h
flip32
Flip32_float(int32_t opnd2, uint32_t y);
#endif//$XCC_h

flip32
Flip32_float(int32_t opnd2, uint32_t y) {
    return  Flip32_fromFrac(Flip32_fromInt32(opnd2), y);
}

#ifdef $XCC_h
flip32
Flip32_floatu(uint32_t opnd2, uint32_t y);
#endif//$XCC_h

flip32
Flip32_floatu(uint32_t opnd2, uint32_t y) {
    return Flip32_fromFrac(Flip32_fromUint32(opnd2), y); 
}


#ifdef $XCC_h
/**
 * \fn flip32 Flip32_fromMul(flip32 x, uint32_t y);
 * \brief Takes a float and an integer and returns the rounded result of
 * x multiplied by 1 << y.
 * @param[in] x The float to divide
 * @param[in] y The amount of shift by which x must be multiplied.
 * @return flip32 The float representing the correct rounded value of x*(1<<y).
 */
flip32
Flip32_fromMul(flip32 x, uint32_t y);
#endif//$XCC_h

flip32
Flip32_fromMul(flip32 x, uint32_t y)
{ 
    bits32 aSig = Flip32_frac( x );
    bits32 aExp = Flip32_exp( x );
    bits32 aSign = Flip32_sign( x );
    if (Flip32_isNaN(x)) {
        Flip_raise(FLIP_INVALID);
        return Flip32_create(aSign, 0, 0);
    }

    if ((aExp || aSig) == 0) {return x;}

    if (y + aExp > 0xFE) {
        if ((0xFE - aExp) > 23) {
            // In this case we set value to infinity
            return Flip32_create(aSign, 0xFF, 0);
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
            flip32 tmp = Flip_roundAndPackFloat32(aSign, 0xFE, aSig << (y - (0xFE - aExp)), 0);
            Flip_status = exception;
            return tmp;
        }
    }
    else {
        return Flip32_create(aSign, aExp + y, aSig);
    }
}

#ifdef $XCC_h
/**
 * \fn flip32 Flip32_fromUMul(flip32 x, uint32_t y);
 * \brief Takes a float and an integer and returns the unsigned rounded result of
 * x multiplied by 1 << y.
 * @param[in] x The float to divide
 * @param[in] y The amount of shift by which x must be multiplied.
 * @return flip32 The float representing the correct unsigned rounded value of x*(1<<y).
 */
flip32
Flip32_fromUMul(flip32 x, uint32_t y);
#endif//$XCC_h

flip32
Flip32_fromUMul(flip32 x, uint32_t y)
{ 
    bits32 aSign = Flip32_sign( x );
    if (aSign) {
        // We don't generate an exception if float is -0.
        if (Flip32_frac(x) || Flip32_exp(x)) Flip_raise(FLIP_INVALID);
        return Flip32_create(0, 0, 0);
    }
    else { 
        return Flip32_fromMul(x, y);
    }
}

#ifdef $XCC_h
int32_t
Flip32_fixed(flip32 fopnd2, uint32_t y);
#endif//$XCC_h

int32_t
Flip32_fixed(flip32 fopnd2, uint32_t y) {
    return Flip32_toInt32(Flip32_fromMul(fopnd2, y));
}

#ifdef $XCC_h
uint32_t
Flip32_fixedu(flip32 fopnd2, uint32_t y);
#endif//$XCC_h

uint32_t
Flip32_fixedu(flip32 fopnd2, uint32_t y) {
    return Flip32_toUint32(Flip32_fromUMul(fopnd2, y));
}


/////////////////////////////////////////////////////////////////////////
// ARITHMETIC FUNCTIONS
/////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Returns the absolute value of the given float
 */
static inline flip32
Flip32_abs(flip32 a) {
    return 0x7FFFFFFF & a;
}
#endif//$XCC_h

#ifdef $XCC_h
/** 
 * Returns the min between two floats.
 */
flip32
Flip32_min(flip32 a, flip32 b);
#endif//$XCC_h

flip32
Flip32_min(flip32 a, flip32 b) {
    if (Flip32_isNaN(a)) {
        // Fixes #1557, we don't raise invalid anymore so we always
        // return the signaling NaN if any.
        if (Flip32_issNaN(a)) {
            return a;
        }
        if (Flip32_issNaN(b)) {
            return b;
        }
        if (Flip32_isNaN(b)) {
            return a;
        }
        else {
            return b;
        }
    }
    if (Flip32_isNaN(b)) {
        if (Flip32_issNaN(b)) {
            return b;
        }
        else {
            return a;
        }
    }
    // Issue #1329 We return -0 if comparison between two opposite 0.
    if (Flip32_isZero(a) && Flip32_isZero(b)) {
        if (Flip32_sign(a)) return a;
        else return b;
    }
    return LesserThan==Flip32_comp(a, b)? a: b;
}

#ifdef $XCC_h
/** 
 * Returns the max between two floats.
 */
flip32
Flip32_max(flip32 a, flip32 b);
#endif//$XCC_h

flip32
Flip32_max(flip32 a, flip32 b) {
    if (Flip32_isNaN(a)) {
        // Fixes #1557, we don't raise invalid anymore so we always
        // return the signaling NaN if any.
        if (Flip32_issNaN(a)) {
            return a;
        }
        if (Flip32_issNaN(b)) {
            return b;
        }
        if (Flip32_isNaN(b)) {
            return a;
        }
        else {
            return b;
        }
    }
    if (Flip32_isNaN(b)) {
        if (Flip32_issNaN(b)) {
            return b;
        }
        else {
            return a;
        }
    }
    // Issue #1329  We return 0 if comparison between two opposite 0.
    if (Flip32_isZero(a) && Flip32_isZero(b)) {
        if (Flip32_sign(a)) return b;
        else return a;
    }
    return GreaterThan==Flip32_comp(a, b)? a: b;
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
flip32
Flip32_fdma(flip32 a, flip32 b, flip32 c, flip32 d);
#endif//$XCC_h

flip32
Flip32_fdma(flip32 a, flip32 b, flip32 c, flip32 d) {
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
flip32
Flip32_fdms(flip32 a, flip32 b, flip32 c, flip32 d);
#endif//$XCC_h

flip32
Flip32_fdms(flip32 a, flip32 b, flip32 c, flip32 d) {
    return FPFDMA_c(a, b, c, d, Flip_rnd_mode, 1);
}


#ifdef $XCC_h
/** 
\brief FP \b fused multiply and add operation: returns \f$ x \times y + z \f$.
\param x is the multiplicand (32-bit FPN)
\param y is the multiplier (32-bit FPN)
\param z is the addend (32-bit FPN)
\addindex FPN
*/
flip32
Flip32_fma(flip32 z, flip32 x, flip32 y);
#endif//$XCC_h

flip32
Flip32_fma(flip32 z, flip32 x, flip32 y) {
    return FPFMA_c(x, y, z, Flip_rnd_mode, 0, 0);
}


#ifdef $XCC_h
flip32
Flip32_fman(flip32 z, flip32 x, flip32 y);
#endif//$XCC_h

flip32
Flip32_fman(flip32 z, flip32 x, flip32 y) {
  return FPFMA_c(x, y, z, Flip_rnd_mode, 1, 1);
}


#ifdef $XCC_h
flip32
Flip32_fms(flip32 z, flip32 x, flip32 y);
#endif//$XCC_h

flip32
Flip32_fms(flip32 z, flip32 x, flip32 y) {
  return FPFMA_c(x, y, z, Flip_rnd_mode, 1, 0);
}


#ifdef $XCC_h
flip32
Flip32_fmsn(flip32 z, flip32 x, flip32 y);
#endif//$XCC_h

flip32
Flip32_fmsn(flip32 z, flip32 x, flip32 y) {
  return FPFMA_c(x, y, z, Flip_rnd_mode, 0, 1);
}


#ifdef $XCC_h
flip32
Flip32_add( flip32 a, flip32 b );
#endif//$XCC_h

flip32
Flip32_add( flip32 a, flip32 b ) {
    // If +infty -infty we generate an invalid exception)
    if (Flip32_isInf(a) && Flip32_isInf(b) && 
        (Flip32_sign(a) ^ Flip32_sign(b))) {
        Flip_raise(FLIP_INVALID);
    }
    return FPFMA_c(Flip32_fromInt32(1), b, a, Flip_rnd_mode, 0, 0);
}


#ifdef $XCC_h
flip32
Flip32_sub(flip32 x, flip32 y);
#endif//$XCC_h

flip32
Flip32_sub(flip32 x, flip32 y) { 
    /////////////////////////////////
    //  NAN HANDLING
    /////////////////////////////////
    // If one of the operand is a signaling NaN, we generate an
    // exception.
    if (Flip32_issNaN(x) || Flip32_issNaN(y)) {
        Flip_raise(FLIP_INVALID);
    }
    // If one of the operand is already a NaN, we return a quiet NaN
    // (same sign, same payload).
    if (Flip32_isNaN(x)) return Flip32_quiet_from_sNaN(x);
    if (Flip32_isNaN(y)) return Flip32_quiet_from_sNaN(y);

    return Flip32_add(x, Flip32_neg(y));
}


#ifdef $XCC_h
flip32
Flip32_mul(flip32 x, flip32 y);
#endif//$XCC_h

flip32
Flip32_mul(flip32 x, flip32 y) {
    flag sgnA = Flip32_sign(x);
    flag sgnB = Flip32_sign(y);
    flip32 c = Flip32_create(sgnA ^ sgnB, 0, 0);
    return FPFMA_c(x, y, c, Flip_rnd_mode, 0, 0);
}


#ifdef $XCC_h
flip32
Flip32_muln(flip32 x, flip32 y);
#endif//$XCC_h

flip32
Flip32_muln(flip32 x, flip32 y) { 
    flag sgnA = Flip32_sign(x);
    flag sgnB = Flip32_sign(y);
    flip32 c = Flip32_create(sgnA ^ sgnB, 0, 0);
    return FPFMA_c(x, y, c, Flip_rnd_mode, 1, 1);
}



#ifdef $XCC_h
/** \brief returns the inverse of the square root of \a X
\a X should be a 32-bit floating-point number.
\addindex FPN
*/
/*---------------------------------------------------------------------------
  This implementation of computing the inverse of square root is based on
  the same approach as used for computing the square root in "fsqrt_fast.c". 
  Moreover, same implementation applies for two different processor cores,
  ST220 and ST231. The approach adopted and the optimizations done are
  explained later. The fully IEEE implementation is not supported, that is
  no "subnormals" and rounding mode is fixed to "round-to-nearest-even". 
---------------------------------------------------------------------------*/
flip32
Flip32_invsqrt(flip32 X);
#endif//$XCC_h

flip32
Flip32_invsqrt(flip32 X) {
  flip32 ret;
	uint32_t ApprRecipSqrtTable[128] ={209, 204, 198, 193, 188, 183, 178, 173, 168, 164, 159, 155, 150, 146, 142, 138, 134, 130, 126, 122, 118, 114, 111, 107, 104, 100, 97, 94, 90, 87, 84, 81, 78, 75, 72, 69, 66, 63, 60, 57, 55, 52, 49, 47, 44, 42, 39, 37, 34, 32, 29, 27, 25, 22, 20, 18, 16, 14, 11, 9, 7, 5, 3, 1, 508, 500, 493, 485, 478, 471, 464, 457, 450, 444, 437, 431, 425, 419, 413, 407, 401, 395, 390, 384, 379, 374, 369, 364, 359, 354, 349, 344, 340, 335, 331, 326, 322, 318, 313, 309, 305, 301, 297, 293, 289, 286, 282, 278, 274, 271, 267, 264, 260, 257, 254, 250, 247, 244, 241, 237, 234, 231, 228, 225, 222, 219, 216, 213};

	uint32_t absX, sgn, expField, sigField, XisNormal, XisInfOrNaN, 
		XhasNullSig,XisZero, XisNaN, XisInf, index, InvSqrt, RexpField, 
		bias, qNaN, Inf;
	uint32_t bit0, bit1, bit2, bit3, bit4;
	sgn        = X& (1<<31);
	expField   = (X<<1) >> 24;
	sigField   = (X& 0x007FFFFF);
	absX       = X& 0x7FFFFFFF;
	XisNormal  = absX > 0x007FFFFF;
	XisInfOrNaN= expField==0xFF;
	XhasNullSig= sigField==0;
	XisZero = (!XisNormal) && XhasNullSig;
	XisNaN = XisInfOrNaN && !XhasNullSig;
	XisInf = XisInfOrNaN && XhasNullSig;
	index = (X>>(24-7)) & 127;
	InvSqrt   = ApprRecipSqrtTable[index];
	bias = 127; 
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
	bit4 = (expField <= 26); 

	bit1 = XisZero;
	bit2 = XisInf && (!sgn);
	bit3 = XisNaN || (sgn && (! XisZero));

	bit0 = ! (bit1 || bit2 || bit3 || bit4 || (!XisNormal));

    //if (XisZero) Flip_raise(FLIP_DIV_BY_ZERO);

	ret= sgn | (RexpField << 23) | (InvSqrt << 14) | (bit4 << 4) | (bit3 << 3) | (bit2 << 2) | (bit1 << 1) | bit0;
  return ret;
}

#ifdef $XCC_h
/** \brief returns the reciprocal \a 1 / \a B
\a B should be a 32-bit floating-point number.
\a A should be a 32-bit floating-point number.
\a behavior shoud be a 32-bit integer number 
\addindex FPN
*/
flip32
Flip32_inv(flip32 A, flip32 B, int behavior);
#endif//$XCC_h

flip32 Flip32_inv(flip32 A, flip32 B, int behavior) { 
  flip32 ret;
    uint32_t ApprRecipTable[128] ={508, 500, 492, 485, 477, 470, 463, 455, 448, 441, 434, 428, 421, 414, 408, 401, 395, 389, 383, 377, 371, 365, 359, 353, 347, 342, 336, 331, 326, 320, 315, 310, 305, 300, 295, 290, 285, 280, 275, 271, 266, 261, 257, 252, 248, 243, 239, 235, 231, 226, 222, 218, 214, 210, 206, 202, 198, 195, 191, 187, 183, 180, 176, 172, 169, 165, 162, 158, 155, 152, 148, 145, 142, 138, 135, 132, 129, 126, 123, 120, 117, 114, 111, 108, 105, 102, 99, 96, 93, 91, 88, 85, 82, 80, 77, 74, 72, 69, 67, 64, 62, 59, 57, 54, 52, 49, 47, 45, 42, 40, 38, 35, 33, 31, 29, 26, 24, 22, 20, 18, 15, 13, 11, 9, 7, 5, 3, 0};
    //uint32_t absX, sgn, expField, sigField, XisNormal, XisInfOrNaN, 
    //  XhasNullSig,XisZero, XisNaN, XisInf, index, Inv, RexpField;
    uint32_t sgnB, absB, expFieldA, expFieldB, sigFieldB, BisNormal, BisInf;
    uint32_t AisInfOrNaN, BisInfOrNaN, BisNaN, BhasNullSig, BisZero;
    uint32_t index, Inv, RexpField;
    uint32_t bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7, bit8, bit9, bit10, bit11, bit12;
    int expFieldA_int, expFieldB_int;
    uint32_t bitVector;
    uint32_t AisZero;
    uint32_t sgnA, sigFieldA, AisNaN;
    uint32_t direct_result_mask, AhasNullSig;
    // unpacking operands A and B
    expFieldA = (A<< 1) >> 24; expFieldB = (B<< 1) >> 24;
    BisInfOrNaN = expFieldB == 0xFF;
    AisInfOrNaN = expFieldA == 0xFF;
    sgnB        = B & (1<<31);
    sgnA = A & 0x80000000;
    //expField   = (X<<1) >> 24;
  
    AisZero     = (A & 0x7FFFFFFF) == 0; 
    sigFieldB   = (B & 0x007FFFFF);
    sigFieldA   = (A & 0x007FFFFF);
    absB        = B & 0x7FFFFFFF;
    BisNormal   = absB > 0x007FFFFF;
    //BisInfOrNaN= expFieldB==0xFF;
    BhasNullSig = (sigFieldB == 0);
    AhasNullSig = (sigFieldA == 0);

    BisZero = (!BisNormal) && BhasNullSig;

    BisNaN = BisInfOrNaN && !BhasNullSig;
    AisNaN = AisInfOrNaN && !AhasNullSig;

    BisInf = BisInfOrNaN && BhasNullSig;
    index = sigFieldB>>(23-7);
    Inv   = ApprRecipTable[index];
    RexpField = 0xFD - expFieldB;
    RexpField = (RexpField << 24) >> 24;
    // bit for special cases
    bit1 = expFieldB == 0x00;
    bit2 = expFieldA == 0x00; 
    bit3 = BisInfOrNaN;
    // if expB = 253 or 254
    bit4 = (expFieldB == 0xFD) | (expFieldB == 0xFE);
    bit5 = AisInfOrNaN;
    expFieldA_int = expFieldA;
    bit6 = (expFieldA != 0x00) && (expFieldA_int < 26);
    expFieldB_int = expFieldB;
    bit7 = (expFieldA_int - expFieldB_int <= -124);
    bit8 = (expFieldA_int - expFieldB_int >= 127);
    bit11 = (behavior == 2) ? (expFieldB_int >= 251) : 0;
    bit12 = (behavior == 2) ? (bit1 || bit3 || bit4) : 0 ;
    bit0 = !(bit1 || bit2 || bit3 || bit4 || bit5 || bit6 || bit7 || bit8 || bit11 || bit12);

    if (bit0 == 1) bit10 = bit9 = 0;// non special cases
    else bit9 = 1; // non direct result

    bitVector = bit0 | (bit1 << 1) | (bit2 << 2) | (bit3 << 3) | (bit4 << 4) | (bit5 << 5) | (bit6 << 6) | (bit7 << 7) | (bit8 << 8) | (bit9 << 9) | (bit11 << 11) | (bit12 << 12);// | bit10 << 10;
    //if (BisZero && !BisNaN)   return sgnB | 0x7F800000 | bitVector;
    //else if(BisNaN)            return B;
    //else if(BisInf)            return sgnB | bitVector;
    //else     
    direct_result_mask = 0xfffffdfe; // mask of bit to be ANDed with direct result
    if (AisNaN) {
        if (!(sigFieldA & 0x400000) || (BisNaN && !(sigFieldB & 0x400000))) Flip_raise(FLIP_INVALID); // sNaN
        return (A | 0x400000) & direct_result_mask;
    };
    if (BisNaN) {
        if (! (sigFieldB & 0x400000)) Flip_raise(FLIP_INVALID);
        return (B | 0x400000) & direct_result_mask;
    }
    if (AisInfOrNaN) { 
        // since A is not NaN (due to previous test
        // A is Inf
        if (BisZero) return B ^ A;
        if (BisInf) {
            Flip_raise(FLIP_INVALID);
            return 0xffffffff & direct_result_mask;
        }
        return (B & 0x80000000) ^ A;
    }
    if (BisInfOrNaN) {
        // due to previous tests this implies B is Inf
        // A is no Inf nor NaN
        return sgnA ^ sgnB;
    }
    if (BisZero && !AisZero) {
        Flip_raise(FLIP_DIV_BY_ZERO);
        return (sgnA ^ sgnB) | 0x7f800000;
    }

    if (expFieldA_int - expFieldB_int > 128) {
        Flip_raise(FLIP_OVERFLOW);
        Flip_raise(FLIP_INEXACT);
        
        // TODO : this output can not contain direct_output_mask
        return (sgnA ^ sgnB) | 0x00000600;
    };

    if (BisZero) {
        if (AisZero) {
            Flip_raise(FLIP_INVALID);
            return 0xffffffff & direct_result_mask;
        }
        return (sgnA ^ sgnB) | 0x7f800000;
    }
    if (AisZero) {
        return (sgnA ^ sgnB) & direct_result_mask;
    };


    if (BisZero && !(AisInfOrNaN || AisZero)) Flip_raise(FLIP_DIV_BY_ZERO);
     ret = sgnB | (RexpField<<23) | (Inv<<14) | bitVector;
   return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
#include <stdlib.h>
{
#ifdef TESTSOFTFLOAT
    // Level 2 is a bit too long...
    const char *param[5] = { "testsoftfloat", "-level1", "-tininessbefore", "int32_to_float32" };
    testsoftfloat(4, param);
    //
    param[3] = "float64_to_float32";
    testsoftfloat(4, param);
    //
// Conversion not yet in the RTL so not yet tested.
//    param[2] = "-errorstop";
//    param[3] = "-nearesteven";
//    param[4] = "int64_to_float32";
//    testsoftfloat(5, param);
    //
    param[3] = "float32_to_int32";
    testsoftfloat(4, param);
    //
    param[3] = "float32_to_float64";
    testsoftfloat(4, param);
    //
    param[3] = "float32_add";
    testsoftfloat(4, param);
    //
    param[3] = "float32_sub";
    testsoftfloat(4, param);
    //
    param[3] = "float32_mul";
    testsoftfloat(4, param);
    //
    param[3] = "float32_min";
    testsoftfloat(4, param);
    //
    param[3] = "float32_max";
    testsoftfloat(4, param);
    //
    param[0] = "IeeeCC754";
    param[1] = "-c";
    param[2] = "-s";
    param[3] = "IeeeCC754/testsets/add";
    IeeeCC754(4, param);
    //
    param[3] = "IeeeCC754/testsets/multiply";
    IeeeCC754(4, param);
    //
    param[3] = "IeeeCC754/testsets/rint32";
    IeeeCC754(4, param);
    //
    param[3] = "IeeeCC754/testsets/ruint32";
    IeeeCC754(4, param);
    //
    param[3] = "IeeeCC754/testsets/cint32";
    IeeeCC754(4, param);
    //
    param[3] = "IeeeCC754/testsets/cuint32";
    IeeeCC754(4, param);
    //
    param[2] = "-d";
    param[3] = "-ds";
    param[4] = "IeeeCC754/testsets/roundto";
    IeeeCC754(5, param);
#endif//TESTSOFTFLOAT
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Flip32 module.
 */
#define Flip32_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Flip32 module.
 */
#define Flip32_FINI()
#endif//$XCC__h

#if XCC__C
void testsoftfloat(int argc, const char **argv);
void IeeeCC754(int argc, const char **argv);
static void
Flip32_TEST(void)
{
#include "BSL/Flip32_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(Flip32_TEST);
  return 0;
}
#endif

