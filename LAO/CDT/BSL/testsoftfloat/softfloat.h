#ifndef __SOFTFLOAT_H__
#define __SOFTFLOAT_H__

/*
===============================================================================

This C header file is part of the SoftFloat IEC/IEEE Floating-point
Arithmetic Package, Release 2a.

Written by John R. Hauser.  This work was made possible in part by the
International Computer Science Institute, located at Suite 600, 1947 Center
Street, Berkeley, California 94704.  Funding was partially provided by the
National Science Foundation under grant MIP-9311980.  The original version
of this code was written as part of a project to build a fixed-point vector
processor in collaboration with the University of California at Berkeley,
overseen by Profs. Nelson Morgan and John Wawrzynek.  More information
is available through the Web page `http://HTTP.CS.Berkeley.EDU/~jhauser/
arithmetic/SoftFloat.html'.

THIS SOFTWARE IS DISTRIBUTED AS IS, FOR FREE.  Although reasonable effort
has been made to avoid it, THIS SOFTWARE MAY CONTAIN FAULTS THAT WILL AT
TIMES RESULT IN INCORRECT BEHAVIOR.  USE OF THIS SOFTWARE IS RESTRICTED TO
PERSONS AND ORGANIZATIONS WHO CAN AND WILL TAKE FULL RESPONSIBILITY FOR ANY
AND ALL LOSSES, COSTS, OR OTHER PROBLEMS ARISING FROM ITS USE.

Derivative works are acceptable, even for commercial purposes, so long as
(1) they include prominent notice that the work is derivative, and (2) they
include prominent notice akin to these four paragraphs for those parts of
this code that are retained.

===============================================================================
*/

/*------------------------------------------------------------------------------
Type definitions mandatory to use operators.
This file is derived from fpmodels.src 
($Revision: 1.35 $ $Date: 2007/04/19 08:16:33 $).
STMicroelectronics Confidential.
------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/


#include "FTypes.h"

/*
-------------------------------------------------------------------------------
Software IEC/IEEE floating-point underflow tininess-detection mode.
-------------------------------------------------------------------------------
*/
#ifdef __HAS_TININESSMODE
extern __thread int8_t flip_detect_tininess;
#endif
enum {
    float_tininess_after_rounding  = 0,
    float_tininess_before_rounding = 1
};

/*
-------------------------------------------------------------------------------
Software IEC/IEEE floating-point rounding mode.
-------------------------------------------------------------------------------
*/
#ifdef __HAS_ROUNDINGMODE
extern int float_rounding_mode;
#endif
enum {
    float_round_nearest_even = 0,
    float_round_to_zero      = 1,
    float_round_up           = 2,
    float_round_down         = 3
};

#ifdef __HAS_ROUNDINGMODE
// We must be able to set rounding mode into Flip library
extern __thread int Flip_rnd_mode;
#endif

/*
-------------------------------------------------------------------------------
Software IEC/IEEE floating-point exception flags.
-------------------------------------------------------------------------------
*/
enum {
    float_flag_invalid   = 1,
    float_flag_divbyzero =  2,
    float_flag_overflow  =  4,
    float_flag_underflow =  8,
    float_flag_inexact   =  16
};

/*
-------------------------------------------------------------------------------
Software IEC/IEEE integer-to-floating-point conversion routines.
-------------------------------------------------------------------------------
*/
// float32 int32_to_float32( int32_t );
// float64 int32_to_float64( int32_t );
float32 int64_to_float32( int64_t );
float64 int64_to_float64( int64_t );

/*
-------------------------------------------------------------------------------
Software IEC/IEEE single-precision conversion routines.
-------------------------------------------------------------------------------
*/
// int32_t float32_to_int32( float32 );
int32_t float32_to_int32_round_to_zero( float32 );
int64_t float32_to_int64( float32 );
int64_t float32_to_int64_round_to_zero( float32 );
float64 float32_to_float64( float32 );
/*
-------------------------------------------------------------------------------
Software IEC/IEEE single-precision operations.
-------------------------------------------------------------------------------
*/
float32 float32_round_to_int( float32 );
float32 float32_add( float32, float32 );
float32 float32_sub( float32, float32 );
float32 float32_max( float32, float32 );
float32 float32_min( float32, float32 );
float32 float32_mul( float32, float32 );
float32 float32_madd( float32, float32, float32 );
float32 float32_msub( float32, float32, float32 );
float32 float32_nmadd( float32, float32, float32 );
float32 float32_nmsub( float32, float32, float32 );
float32 float32_div( float32, float32 );
float32 float32_rem( float32, float32 );
float32 float32_sqrt( float32 );
float32 float32_rsqrt( float32 );
float32 float32_recip( float32 );
float32 float32_square( float32 );
float32 float32_rsqrt( float32 );
int float32_eq( float32, float32 );
int float32_le( float32, float32 );
int float32_lt( float32, float32 );
int float32_eq_signaling( float32, float32 );
int float32_le_quiet( float32, float32 );
int float32_lt_quiet( float32, float32 );
int float32_is_signaling_nan( float32 );

/*
-------------------------------------------------------------------------------
Software IEC/IEEE double-precision conversion routines.
-------------------------------------------------------------------------------
*/
int32_t float64_to_int32( float64 );
int32_t float64_to_int32_round_to_zero( float64 );
int64_t float64_to_int64( float64 );
int64_t float64_to_int64_round_to_zero( float64 );
float32 float64_to_float32( float64 );

/*
-------------------------------------------------------------------------------
Software IEC/IEEE double-precision operations.
-------------------------------------------------------------------------------
*/
float64 float64_round_to_int( float64 );
float64 float64_add( float64, float64 );
float64 float64_sub( float64, float64 );
float64 float64_max( float64, float64 );
float64 float64_min( float64, float64 );
float64 float64_mul( float64, float64 );
float64 float64_div( float64, float64 );
float64 float64_rem( float64, float64 );
float64 float64_sqrt( float64 );
int float64_eq( float64, float64 );
int float64_le( float64, float64 );
int float64_lt( float64, float64 );
int float64_eq_signaling( float64, float64 );
int float64_le_quiet( float64, float64 );
int float64_lt_quiet( float64, float64 );
int float64_is_signaling_nan( float64 );

#ifdef __LIBFP_USE_FLIP



#ifdef __FLIP_HAS_ROUNDING_MODES
/* ---------------------------------------------------------------- */
// ROUDING MODES

/// Defines the 4 possible IEEE rounding modes.
typedef enum 
{
  FLIP_RN, ///< Rounding to nearest (default mode)
  FLIP_RZ,  ///< Rounding to zero
  FLIP_RU, ///< Rounding up
  FLIP_RD ///< Rounding down
} flip_rnd_mode_t;

/// Active rounding mode register.
extern flip_rnd_mode_t flip_rnd_mode;

/// Set the active rounding mode.
inline void flip_set_rnd_mode(flip_rnd_mode_t rnd);

/* ---------------------------------------------------------------- */
// EXCEPTIONS

/// Defines the possible execptions.
typedef enum 
{
  FLIP_EXPT_OK ///< No execption.
} flip_execption_t;

/// \brief IEEE exceptions register.
extern flip_execption_t flip_status;

#endif // __FLIP_HAS_ROUNDING_MODES




/* ---------------------------------------------------------------- */
// SPECIFIC VALUES

#define FLIP_NaN   0x7FFFFFFF ///< IEEE coding of Not a Number (one the possible codings).
#define FLIP_ZERO  0x00000000 ///< IEEE coding of +0.
#define FLIP_MZERO 0x80000000 ///< IEEE coding of -0.
#define FLIP_INF   0x7F800000 ///< IEEE coding of +Infinity.
#define FLIP_MINF  0xFF800000 ///< IEEE coding of -Infinity.

/* ---------------------------------------------------------------- */
// MACROS DEFINITION

/// Returns the \b maximum of the two operands \p x and \p y.
#define _FLIP_MAX(x,y) ( (x)>(y) ? (x) : (y) )

/// Returns the \b minimum of the two operands \p x and \p y.
#define _FLIP_MIN(x,y) ( (x)<(y) ? (x) : (y) )

/// Returns the \b absolute \b value of the operand \p x.
#define _FLIP_ABS(x) ( (x) < 0 ? -(x) : (x) )


/* ---------------------------------------------------------------- */
// CONSTANTS DEFINTION

#define _FLIP_EXP_SIZE 8   ///< \b Size of \b exponent in single precision numbers.
#define _FLIP_MANT_SIZE 23 ///< \b Size of \b mantissa in single precision numbers.
#define _FLIP_BIAS 127     ///< \b Bias of \b exponent in single precision numbers.


#endif
    
#ifdef __cplusplus
}
#endif /*__cplusplus*/

/*
-------------------------------------------------------------------------------
The `LIT64' macro takes as its argument a textual integer literal and
if necessary ``marks'' the literal as having a 64-bit integer type.
For example, the GNU C Compiler (`gcc') requires that 64-bit literals be
appended with the letters `LL' standing for `long long', which is `gcc's
name for the 64-bit integer type.  Some compilers may allow `LIT64' to be
defined as the identity macro:  `#define LIT64( a ) a'.
-------------------------------------------------------------------------------
*/
#define LIT64( a ) a##LL


#endif /*__SOFTFLOAT_H__*/
