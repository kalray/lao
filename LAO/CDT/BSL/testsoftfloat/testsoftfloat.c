
/*
===============================================================================

This C source file is part of TestFloat, Release 2a, a package of programs
for testing the correctness of floating-point arithmetic complying to the
IEC/IEEE Standard for Floating-Point.

Written by John R. Hauser.  More information is available through the Web
page `http://HTTP.CS.Berkeley.EDU/~jhauser/arithmetic/TestFloat.html'.

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

#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "milieu.h"
#include "fail.h"

#include "slowfloat.h"
#include "testCases.h"
#include "testLoops.h"

#include "FTypes.h"
#include "Flip32.h"
#include "Flip64.h"

#define __HAS_ROUNDINGMODE

extern __thread Flip_rnd_mode_t Flip_rnd_mode;
extern __thread flip_exception_t Flip_status;

static void catchSIGINT( int signalCode )
{

    if ( stop ) exit( EXIT_FAILURE );
    stop = TRUE;

}

static int8_t clearFlags( void )
{
    int8_t flags = 0;

#ifdef __HAS_EXCEPTIONFLAGS
    flags = Flip_status;
    Flip_status = 0;
#endif
    return flags;

}

enum {
    INT32_TO_FLOAT32 = 1,
    INT32_TO_FLOAT64,
#ifdef FLOATX80
    INT32_TO_FLOATX80,
#endif
#ifdef FLOAT128
    INT32_TO_FLOAT128,
#endif
#ifdef BITS64
    INT64_TO_FLOAT32,
    INT64_TO_FLOAT64,
#ifdef FLOATX80
    INT64_TO_FLOATX80,
#endif
#ifdef FLOAT128
    INT64_TO_FLOAT128,
#endif
#endif
    FLOAT32_TO_INT32,
//    FLOAT32_TO_INT32_ROUND_TO_ZERO,
#ifdef BITS64
//    FLOAT32_TO_INT64,
//    FLOAT32_TO_INT64_ROUND_TO_ZERO,
#endif
    FLOAT32_TO_FLOAT64,
#ifdef FLOATX80
    FLOAT32_TO_FLOATX80,
#endif
#ifdef FLOAT128
    FLOAT32_TO_FLOAT128,
#endif
#ifdef __HAS_R2I
    FLOAT32_ROUND_TO_INT,
#endif
    FLOAT32_ADD,
    FLOAT32_SUB,
    FLOAT32_MAX,
    FLOAT32_MIN,
    FLOAT32_MUL,
//    FLOAT32_DIV,
#ifdef __HAS_REM
    FLOAT32_REM,
#endif
//    FLOAT32_SQRT,
#ifdef __HAS_PREDICATES
    FLOAT32_EQ,
    FLOAT32_LE,
    FLOAT32_LT,
#ifdef __HAS_FULL_PREDICATES
    FLOAT32_EQ_SIGNALING,
    FLOAT32_LE_QUIET,
    FLOAT32_LT_QUIET,
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
//    FLOAT64_TO_INT32,
//    FLOAT64_TO_INT32_ROUND_TO_ZERO,
#ifdef BITS64
    FLOAT64_TO_INT64,
//    FLOAT64_TO_INT64_ROUND_TO_ZERO,
#endif
    FLOAT64_TO_FLOAT32,
#ifdef FLOATX80
    FLOAT64_TO_FLOATX80,
#endif
#ifdef FLOAT128
    FLOAT64_TO_FLOAT128,
#endif
#ifdef __HAS_R2I
    FLOAT64_ROUND_TO_INT,
#endif
    FLOAT64_ADD,
    FLOAT64_SUB,
    FLOAT64_MAX,
    FLOAT64_MIN,
    FLOAT64_MUL,
//    FLOAT64_DIV,
#ifdef __HAS_REM
    FLOAT64_REM,
#endif
//    FLOAT64_SQRT,
#ifdef __HAS_PREDICATES
    FLOAT64_EQ,
    FLOAT64_LE,
    FLOAT64_LT,
#ifdef __HAS_FULL_PREDICATES
    FLOAT64_EQ_SIGNALING,
    FLOAT64_LE_QUIET,
    FLOAT64_LT_QUIET,
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
#ifdef FLOATX80
    FLOATX80_TO_INT32,
    FLOATX80_TO_INT32_ROUND_TO_ZERO,
#ifdef BITS64
    FLOATX80_TO_INT64,
    FLOATX80_TO_INT64_ROUND_TO_ZERO,
#endif
    FLOATX80_TO_FLOAT32,
    FLOATX80_TO_FLOAT64,
#ifdef FLOAT128
    FLOATX80_TO_FLOAT128,
#endif
    FLOATX80_ROUND_TO_INT,
    FLOATX80_ADD,
    FLOATX80_SUB,
    FLOATX80_MUL,
    FLOATX80_DIV,
#ifdef __HAS_REM
    FLOATX80_REM,
#endif
    FLOATX80_SQRT,
#ifdef __HAS_PREDICATES
    FLOATX80_EQ,
    FLOATX80_LE,
    FLOATX80_LT,
#ifdef __HAS_FULL_PREDICATES
    FLOATX80_EQ_SIGNALING,
    FLOATX80_LE_QUIET,
    FLOATX80_LT_QUIET,
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
#endif
#ifdef FLOAT128
    FLOAT128_TO_INT32,
    FLOAT128_TO_INT32_ROUND_TO_ZERO,
#ifdef BITS64
    FLOAT128_TO_INT64,
    FLOAT128_TO_INT64_ROUND_TO_ZERO,
#endif
    FLOAT128_TO_FLOAT32,
    FLOAT128_TO_FLOAT64,
#ifdef FLOATX80
    FLOAT128_TO_FLOATX80,
#endif
    FLOAT128_ROUND_TO_INT,
    FLOAT128_ADD,
    FLOAT128_SUB,
    FLOAT128_MUL,
    FLOAT128_DIV,
#ifdef __HAS_REM
    FLOAT128_REM,
#endif
    FLOAT128_SQRT,
#ifdef __HAS_PREDICATES
    FLOAT128_EQ,
    FLOAT128_LE,
    FLOAT128_LT,
#ifdef __HAS_FULL_PREDICATES
    FLOAT128_EQ_SIGNALING,
    FLOAT128_LE_QUIET,
    FLOAT128_LT_QUIET,
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
#endif
    NUM_FUNCTIONS
};
static struct {
    const char *name;
    int8_t numInputs;
    flag roundingPrecision, roundingMode;
    flag tininessMode, tininessModeAtReducedPrecision;
} functions[ NUM_FUNCTIONS ] = {
    { 0, 0, 0, 0, 0, 0 },
    { "int32_to_float32",                1, FALSE, TRUE,  FALSE, FALSE },
    { "int32_to_float64",                1, FALSE, FALSE, FALSE, FALSE },
#ifdef FLOATX80
    { "int32_to_floatx80",               1, FALSE, FALSE, FALSE, FALSE },
#endif
#ifdef FLOAT128
    { "int32_to_float128",               1, FALSE, FALSE, FALSE, FALSE },
#endif
#ifdef BITS64
    { "int64_to_float32",                1, FALSE, TRUE,  FALSE, FALSE },
    { "int64_to_float64",                1, FALSE, TRUE,  FALSE, FALSE },
#ifdef FLOATX80
    { "int64_to_floatx80",               1, FALSE, FALSE, FALSE, FALSE },
#endif
#ifdef FLOAT128
    { "int64_to_float128",               1, FALSE, FALSE, FALSE, FALSE },
#endif
#endif
    { "float32_to_int32",                1, FALSE, TRUE,  FALSE, FALSE },
//    { "float32_to_int32_round_to_zero",  1, FALSE, FALSE, FALSE, FALSE },
#ifdef BITS64
//    { "float32_to_int64",                1, FALSE, TRUE,  FALSE, FALSE },
//    { "float32_to_int64_round_to_zero",  1, FALSE, FALSE, FALSE, FALSE },
#endif
    { "float32_to_float64",              1, FALSE, FALSE, FALSE, FALSE },
#ifdef FLOATX80
    { "float32_to_floatx80",             1, FALSE, FALSE, FALSE, FALSE },
#endif
#ifdef FLOAT128
    { "float32_to_float128",             1, FALSE, FALSE, FALSE, FALSE },
#endif
#ifdef __HAS_R2I
    { "float32_round_to_int",            1, FALSE, TRUE,  FALSE, FALSE },
#endif
    { "float32_add",                     2, FALSE, TRUE,  FALSE, FALSE },
    { "float32_sub",                     2, FALSE, TRUE,  FALSE, FALSE },
    { "float32_max",                     2, FALSE, TRUE,  TRUE,  FALSE },
    { "float32_min",                     2, FALSE, TRUE,  TRUE,  FALSE },
    { "float32_mul",                     2, FALSE, TRUE,  TRUE,  FALSE },
//    { "float32_div",                     2, FALSE, TRUE,  FALSE, FALSE },
#ifdef __HAS_REM
    { "float32_rem",                     2, FALSE, FALSE, FALSE, FALSE },
#endif
//    { "float32_sqrt",                    1, FALSE, TRUE,  FALSE, FALSE },
#ifdef __HAS_PREDICATES
    { "float32_eq",                      2, FALSE, FALSE, FALSE, FALSE },
    { "float32_le",                      2, FALSE, FALSE, FALSE, FALSE },
    { "float32_lt",                      2, FALSE, FALSE, FALSE, FALSE },
#ifdef __HAS_FULL_PREDICATES
    { "float32_eq_signaling",            2, FALSE, FALSE, FALSE, FALSE },
    { "float32_le_quiet",                2, FALSE, FALSE, FALSE, FALSE },
    { "float32_lt_quiet",                2, FALSE, FALSE, FALSE, FALSE },
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
//    { "float64_to_int32",                1, FALSE, TRUE,  FALSE, FALSE },
//    { "float64_to_int32_round_to_zero",  1, FALSE, FALSE, FALSE, FALSE },
#ifdef BITS64
    { "float64_to_int64",                1, FALSE, TRUE,  FALSE, FALSE },
//    { "float64_to_int64_round_to_zero",  1, FALSE, FALSE, FALSE, FALSE },
#endif
    { "float64_to_float32",              1, FALSE, TRUE,  TRUE,  FALSE },
#ifdef FLOATX80
    { "float64_to_floatx80",             1, FALSE, FALSE, FALSE, FALSE },
#endif
#ifdef FLOAT128
    { "float64_to_float128",             1, FALSE, FALSE, FALSE, FALSE },
#endif
#ifdef __HAS_R2I
    { "float64_round_to_int",            1, FALSE, TRUE,  FALSE, FALSE },
#endif
    { "float64_add",                     2, FALSE, TRUE,  FALSE, FALSE },
    { "float64_sub",                     2, FALSE, TRUE,  FALSE, FALSE },
    { "float64_max",                     2, FALSE, TRUE,  TRUE,  FALSE },
    { "float64_min",                     2, FALSE, TRUE,  TRUE,  FALSE },
    { "float64_mul",                     2, FALSE, TRUE,  TRUE,  FALSE },
//    { "float64_div",                     2, FALSE, TRUE,  FALSE, FALSE },
#ifdef __HAS_REM
    { "float64_rem",                     2, FALSE, FALSE, FALSE, FALSE },
#endif
//    { "float64_sqrt",                    1, FALSE, TRUE,  FALSE, FALSE },
#ifdef __HAS_PREDICATES
    { "float64_eq",                      2, FALSE, FALSE, FALSE, FALSE },
    { "float64_le",                      2, FALSE, FALSE, FALSE, FALSE },
    { "float64_lt",                      2, FALSE, FALSE, FALSE, FALSE },
#ifdef __HAS_FULL_PREDICATES
    { "float64_eq_signaling",            2, FALSE, FALSE, FALSE, FALSE },
    { "float64_le_quiet",                2, FALSE, FALSE, FALSE, FALSE },
    { "float64_lt_quiet",                2, FALSE, FALSE, FALSE, FALSE },
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
#ifdef FLOATX80
    { "floatx80_to_int32",               1, FALSE, TRUE,  FALSE, FALSE },
    { "floatx80_to_int32_round_to_zero", 1, FALSE, FALSE, FALSE, FALSE },
#ifdef BITS64
    { "floatx80_to_int64",               1, FALSE, TRUE,  FALSE, FALSE },
    { "floatx80_to_int64_round_to_zero", 1, FALSE, FALSE, FALSE, FALSE },
#endif
    { "floatx80_to_float32",             1, FALSE, TRUE,  TRUE,  FALSE },
    { "floatx80_to_float64",             1, FALSE, TRUE,  TRUE,  FALSE },
#ifdef FLOAT128
    { "floatx80_to_float128",            1, FALSE, FALSE, FALSE, FALSE },
#endif
#ifdef __HAS_R2I
    { "floatx80_round_to_int",           1, FALSE, TRUE,  FALSE, FALSE },
#endif
    { "floatx80_add",                    2, TRUE,  TRUE,  FALSE, TRUE  },
    { "floatx80_sub",                    2, TRUE,  TRUE,  FALSE, TRUE  },
    { "floatx80_mul",                    2, TRUE,  TRUE,  TRUE,  TRUE  },
    { "floatx80_div",                    2, TRUE,  TRUE,  FALSE, TRUE  },
#ifdef __HAS_REM
    { "floatx80_rem",                    2, FALSE, FALSE, FALSE, FALSE },
#endif
    { "floatx80_sqrt",                   1, TRUE,  TRUE,  FALSE, FALSE },
#ifdef __HAS_PREDICATES
    { "floatx80_eq",                     2, FALSE, FALSE, FALSE, FALSE },
    { "floatx80_le",                     2, FALSE, FALSE, FALSE, FALSE },
    { "floatx80_lt",                     2, FALSE, FALSE, FALSE, FALSE },
#ifdef __HAS_FULL_PREDICATES
    { "floatx80_eq_signaling",           2, FALSE, FALSE, FALSE, FALSE },
    { "floatx80_le_quiet",               2, FALSE, FALSE, FALSE, FALSE },
    { "floatx80_lt_quiet",               2, FALSE, FALSE, FALSE, FALSE },
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
#endif
#ifdef FLOAT128
    { "float128_to_int32",               1, FALSE, TRUE,  FALSE, FALSE },
    { "float128_to_int32_round_to_zero", 1, FALSE, FALSE, FALSE, FALSE },
#ifdef BITS64
    { "float128_to_int64",               1, FALSE, TRUE,  FALSE, FALSE },
    { "float128_to_int64_round_to_zero", 1, FALSE, FALSE, FALSE, FALSE },
#endif
    { "float128_to_float32",             1, FALSE, TRUE,  TRUE,  FALSE },
    { "float128_to_float64",             1, FALSE, TRUE,  TRUE,  FALSE },
#ifdef FLOATX80
    { "float128_to_floatx80",            1, FALSE, TRUE,  TRUE,  FALSE },
#endif
#ifdef __HAS_R2I
    { "float128_round_to_int",           1, FALSE, TRUE,  FALSE, FALSE },
#endif
    { "float128_add",                    2, FALSE, TRUE,  FALSE, FALSE },
    { "float128_sub",                    2, FALSE, TRUE,  FALSE, FALSE },
    { "float128_mul",                    2, FALSE, TRUE,  TRUE,  FALSE },
    { "float128_div",                    2, FALSE, TRUE,  FALSE, FALSE },
#ifdef __HAS_REM
    { "float128_rem",                    2, FALSE, FALSE, FALSE, FALSE },
#endif
    { "float128_sqrt",                   1, FALSE, TRUE,  FALSE, FALSE },
#ifdef __HAS_PREDICATES
    { "float128_eq",                     2, FALSE, FALSE, FALSE, FALSE },
    { "float128_le",                     2, FALSE, FALSE, FALSE, FALSE },
    { "float128_lt",                     2, FALSE, FALSE, FALSE, FALSE },
#ifdef __HAS_FULL_PREDICATES
    { "float128_eq_signaling",           2, FALSE, FALSE, FALSE, FALSE },
    { "float128_le_quiet",               2, FALSE, FALSE, FALSE, FALSE },
    { "float128_lt_quiet",               2, FALSE, FALSE, FALSE, FALSE },
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
#endif
};

enum {
    ROUND_NEAREST_EVEN = 1,
    ROUND_UP,
    ROUND_DOWN,
    ROUND_TO_ZERO,
    NUM_ROUNDINGMODES
};
enum {
    TININESS_BEFORE_ROUNDING = 1,
    TININESS_AFTER_ROUNDING,
    NUM_TININESSMODES
};

static void
 testFunctionVariety(
     uint8_t functionCode,
     int8_t roundingPrecision,
     int8_t roundingMode,
     int8_t tininessMode
 )
{
    uint8_t roundingCode = 0;
    int8_t tininessCode = 0;

    functionName = (char *) functions[ functionCode ].name;
    if ( roundingPrecision == 32 ) {
        roundingPrecisionName = "32";
    }
    else if ( roundingPrecision == 64 ) {
        roundingPrecisionName = "64";
    }
    else if ( roundingPrecision == 80 ) {
        roundingPrecisionName = "80";
    }
    else {
        roundingPrecisionName = 0;
    }
#ifdef FLOATX80
    floatx80_rounding_precision = roundingPrecision;
    slow_floatx80_rounding_precision = roundingPrecision;
#endif
    switch ( roundingMode ) {
     case 0:
        roundingModeName = 0;
        roundingCode = float_round_nearest_even;
        Flip_rnd_mode = FLIP_RN;
        break;
     case ROUND_NEAREST_EVEN:
        roundingModeName = "nearest_even";
        roundingCode = float_round_nearest_even;
        Flip_rnd_mode = FLIP_RN;
        break;
     case ROUND_TO_ZERO:
        roundingModeName = "to_zero";
        roundingCode = float_round_to_zero;
        Flip_rnd_mode = FLIP_RZ;
        break;
     case ROUND_DOWN:
        roundingModeName = "down";
        roundingCode = float_round_down;
        Flip_rnd_mode = FLIP_RD;
        break;
     case ROUND_UP:
        roundingModeName = "up";
        roundingCode = float_round_up;
        Flip_rnd_mode = FLIP_RU;
        break;
    }


#ifdef __HAS_ROUNDINGMODE
//    Flip_rnd_mode = roundingCode;
    //float_rounding_mode = roundingCode;
#endif
#if defined(__LIBFP_USE_FLIP) && defined(__FLIP_HAS_ROUNDING_MODES)
    //flip_rnd_mode = roundingCode;
#endif
    slow_float_rounding_mode = roundingCode;

    switch ( tininessMode ) {
     case 0:
        tininessModeName = 0;
        tininessCode = float_tininess_after_rounding;
        break;
     case TININESS_BEFORE_ROUNDING:
        tininessModeName = "before";
        tininessCode = float_tininess_before_rounding;
        break;
     case TININESS_AFTER_ROUNDING:
        tininessModeName = "after";
        tininessCode = float_tininess_after_rounding;
        break;
    }

#ifdef __HAS_TININESSMODE
    flip_detect_tininess = tininessCode;
#endif 
    slow_float_detect_tininess = tininessCode;

    fputs( "Testing ", stdout );
    writeFunctionName( stdout );
    fputs( ".\n", stdout );
    switch ( functionCode ) {
     case INT32_TO_FLOAT32:
        test_a_int32_z_float32( slow_int32_to_float32, int32_to_float32 );
        break;
     case INT32_TO_FLOAT64:
        test_a_int32_z_float64( slow_int32_to_float64, int32_to_float64 );
        break;
#ifdef FLOATX80
     case INT32_TO_FLOATX80:
        test_a_int32_z_floatx80( slow_int32_to_floatx80, int32_to_floatx80 );
        break;
#endif
#ifdef FLOAT128
     case INT32_TO_FLOAT128:
        test_a_int32_z_float128( slow_int32_to_float128, int32_to_float128 );
        break;
#endif
#ifdef BITS64
     case INT64_TO_FLOAT32:
        test_a_int64_z_float32( slow_int64_to_float32, int64_to_float32 );
        break;
     case INT64_TO_FLOAT64:
        test_a_int64_z_float64( slow_int64_to_float64, int64_to_float64 );
        break;
#ifdef FLOATX80
     case INT64_TO_FLOATX80:
        test_a_int64_z_floatx80( slow_int64_to_floatx80, int64_to_floatx80 );
        break;
#endif
#ifdef FLOAT128
     case INT64_TO_FLOAT128:
        test_a_int64_z_float128( slow_int64_to_float128, int64_to_float128 );
        break;
#endif
#endif
     case FLOAT32_TO_INT32:
        test_a_float32_z_int32( slow_float32_to_int32, float32_to_int32 );
        break;
//     case FLOAT32_TO_INT32_ROUND_TO_ZERO:
//        test_a_float32_z_int32(
//            slow_float32_to_int32_round_to_zero,
//            float32_to_int32_round_to_zero
//        );
//        break;
#ifdef BITS64
//     case FLOAT32_TO_INT64:
//        test_a_float32_z_int64( slow_float32_to_int64, float32_to_int64 );
//        break;
//     case FLOAT32_TO_INT64_ROUND_TO_ZERO:
//        test_a_float32_z_int64(
//            slow_float32_to_int64_round_to_zero,
//            float32_to_int64_round_to_zero
//        );
//        break;
#endif
     case FLOAT32_TO_FLOAT64:
        test_a_float32_z_float64(
            slow_float32_to_float64, float32_to_float64 );
        break;
#ifdef FLOATX80
     case FLOAT32_TO_FLOATX80:
        test_a_float32_z_floatx80(
            slow_float32_to_floatx80, float32_to_floatx80 );
        break;
#endif
#ifdef FLOAT128
     case FLOAT32_TO_FLOAT128:
        test_a_float32_z_float128(
            slow_float32_to_float128, float32_to_float128 );
        break;
#endif
#ifdef __HAS_R2I
     case FLOAT32_ROUND_TO_INT:
        test_az_float32( slow_float32_round_to_int, float32_round_to_int );
        break;
#endif
     case FLOAT32_ADD:
        test_abz_float32( slow_float32_add, float32_add );
        break;
     case FLOAT32_SUB:
        test_abz_float32( slow_float32_sub, float32_sub );
        break;
     case FLOAT32_MAX:
        test_abz_float32( slow_float32_max, float32_max );
        break;
     case FLOAT32_MIN:
        test_abz_float32( slow_float32_min, float32_min );
        break;
     case FLOAT32_MUL:
        test_abz_float32( slow_float32_mul, float32_mul );
        break;
//     case FLOAT32_DIV:
//        test_abz_float32( slow_float32_div, float32_div );
//        break;
#ifdef __HAS_REM
     case FLOAT32_REM:
        test_abz_float32( slow_float32_rem, float32_rem );
        break;
#endif
//     case FLOAT32_SQRT:
//        test_az_float32( slow_float32_sqrt, float32_sqrt );
//        break;
#ifdef __HAS_PREDICATES
     case FLOAT32_EQ:
        test_ab_float32_z_flag( slow_float32_eq, float32_eq );
        break;
     case FLOAT32_LE:
        test_ab_float32_z_flag( slow_float32_le, float32_le );
        break;
     case FLOAT32_LT:
        test_ab_float32_z_flag( slow_float32_lt, float32_lt );
        break;
#ifdef __HAS_FULL_PREDICATES
     case FLOAT32_EQ_SIGNALING:
        test_ab_float32_z_flag(
            slow_float32_eq_signaling, float32_eq_signaling );
        break;
     case FLOAT32_LE_QUIET:
        test_ab_float32_z_flag( slow_float32_le_quiet, float32_le_quiet );
        break;
     case FLOAT32_LT_QUIET:
        test_ab_float32_z_flag( slow_float32_lt_quiet, float32_lt_quiet );
        break;
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
//     case FLOAT64_TO_INT32:
//        test_a_float64_z_int32( slow_float64_to_int32, float64_to_int32 );
//        break;
//     case FLOAT64_TO_INT32_ROUND_TO_ZERO:
//        test_a_float64_z_int32(
//            slow_float64_to_int32_round_to_zero,
//            float64_to_int32_round_to_zero
//        );
//        break;
#ifdef BITS64
     case FLOAT64_TO_INT64:
        test_a_float64_z_int64( slow_float64_to_int64, float64_to_int64 );
        break;
//     case FLOAT64_TO_INT64_ROUND_TO_ZERO:
//        test_a_float64_z_int64(
//            slow_float64_to_int64_round_to_zero,
//            float64_to_int64_round_to_zero
//        );
//        break;
#endif
     case FLOAT64_TO_FLOAT32:
        test_a_float64_z_float32(
            slow_float64_to_float32, float64_to_float32 );
        break;
#ifdef FLOATX80
     case FLOAT64_TO_FLOATX80:
        test_a_float64_z_floatx80(
            slow_float64_to_floatx80, float64_to_floatx80 );
        break;
#endif
#ifdef FLOAT128
     case FLOAT64_TO_FLOAT128:
        test_a_float64_z_float128(
            slow_float64_to_float128, float64_to_float128 );
        break;
#endif
#ifdef __HAS_R2I
     case FLOAT64_ROUND_TO_INT:
        test_az_float64( slow_float64_round_to_int, float64_round_to_int );
        break;
#endif
     case FLOAT64_ADD:
        test_abz_float64( slow_float64_add, float64_add );
        break;
     case FLOAT64_SUB:
        test_abz_float64( slow_float64_sub, float64_sub );
        break;
     case FLOAT64_MAX:
        test_abz_float64( slow_float64_max, float64_max );
        break;
     case FLOAT64_MIN:
        test_abz_float64( slow_float64_min, float64_min );
        break;
     case FLOAT64_MUL:
        test_abz_float64( slow_float64_mul, float64_mul );
        break;
//     case FLOAT64_DIV:
//        test_abz_float64( slow_float64_div, float64_div );
//        break;
#ifdef __HAS_REM
     case FLOAT64_REM:
        test_abz_float64( slow_float64_rem, float64_rem );
        break;
#endif
//     case FLOAT64_SQRT:
//        test_az_float64( slow_float64_sqrt, float64_sqrt );
//        break;
#ifdef __HAS_PREDICATES
     case FLOAT64_EQ:
        test_ab_float64_z_flag( slow_float64_eq, float64_eq );
        break;
     case FLOAT64_LE:
        test_ab_float64_z_flag( slow_float64_le, float64_le );
        break;
     case FLOAT64_LT:
        test_ab_float64_z_flag( slow_float64_lt, float64_lt );
        break;
#ifdef __HAS_FULL_PREDICATES
     case FLOAT64_EQ_SIGNALING:
        test_ab_float64_z_flag(
            slow_float64_eq_signaling, float64_eq_signaling );
        break;
     case FLOAT64_LE_QUIET:
        test_ab_float64_z_flag( slow_float64_le_quiet, float64_le_quiet );
        break;
     case FLOAT64_LT_QUIET:
        test_ab_float64_z_flag( slow_float64_lt_quiet, float64_lt_quiet );
        break;
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
#ifdef FLOATX80
     case FLOATX80_TO_INT32:
        test_a_floatx80_z_int32( slow_floatx80_to_int32, floatx80_to_int32 );
        break;
     case FLOATX80_TO_INT32_ROUND_TO_ZERO:
        test_a_floatx80_z_int32(
            slow_floatx80_to_int32_round_to_zero,
            floatx80_to_int32_round_to_zero
        );
        break;
#ifdef BITS64
     case FLOATX80_TO_INT64:
        test_a_floatx80_z_int64( slow_floatx80_to_int64, floatx80_to_int64 );
        break;
     case FLOATX80_TO_INT64_ROUND_TO_ZERO:
        test_a_floatx80_z_int64(
            slow_floatx80_to_int64_round_to_zero,
            floatx80_to_int64_round_to_zero
        );
        break;
#endif
     case FLOATX80_TO_FLOAT32:
        test_a_floatx80_z_float32(
            slow_floatx80_to_float32, floatx80_to_float32 );
        break;
     case FLOATX80_TO_FLOAT64:
        test_a_floatx80_z_float64(
            slow_floatx80_to_float64, floatx80_to_float64 );
        break;
#ifdef FLOAT128
     case FLOATX80_TO_FLOAT128:
        test_a_floatx80_z_float128(
            slow_floatx80_to_float128, floatx80_to_float128 );
        break;
#endif
#ifdef __HAS_R2I
     case FLOATX80_ROUND_TO_INT:
        test_az_floatx80( slow_floatx80_round_to_int, floatx80_round_to_int );
        break;
#endif
     case FLOATX80_ADD:
        test_abz_floatx80( slow_floatx80_add, floatx80_add );
        break;
     case FLOATX80_SUB:
        test_abz_floatx80( slow_floatx80_sub, floatx80_sub );
        break;
     case FLOATX80_MUL:
        test_abz_floatx80( slow_floatx80_mul, floatx80_mul );
        break;
     case FLOATX80_DIV:
        test_abz_floatx80( slow_floatx80_div, floatx80_div );
        break;
#ifdef __HAS_REM
     case FLOATX80_REM:
        test_abz_floatx80( slow_floatx80_rem, floatx80_rem );
        break;
#endif
     case FLOATX80_SQRT:
        test_az_floatx80( slow_floatx80_sqrt, floatx80_sqrt );
        break;
#ifdef __HAS_PREDICATES
     case FLOATX80_EQ:
        test_ab_floatx80_z_flag( slow_floatx80_eq, floatx80_eq );
        break;
     case FLOATX80_LE:
        test_ab_floatx80_z_flag( slow_floatx80_le, floatx80_le );
        break;
     case FLOATX80_LT:
        test_ab_floatx80_z_flag( slow_floatx80_lt, floatx80_lt );
        break;
#ifdef __HAS_FULL_PREDICATES
     case FLOATX80_EQ_SIGNALING:
        test_ab_floatx80_z_flag(
            slow_floatx80_eq_signaling, floatx80_eq_signaling );
        break;
     case FLOATX80_LE_QUIET:
        test_ab_floatx80_z_flag( slow_floatx80_le_quiet, floatx80_le_quiet );
        break;
     case FLOATX80_LT_QUIET:
        test_ab_floatx80_z_flag( slow_floatx80_lt_quiet, floatx80_lt_quiet );
        break;
#endif /* __HAS_FULL_PREDICATES */
#endif /* __HAS_PREDICATES */
#endif
#ifdef FLOAT128
     case FLOAT128_TO_INT32:
        test_a_float128_z_int32( slow_float128_to_int32, float128_to_int32 );
        break;
     case FLOAT128_TO_INT32_ROUND_TO_ZERO:
        test_a_float128_z_int32(
            slow_float128_to_int32_round_to_zero,
            float128_to_int32_round_to_zero
        );
        break;
#ifdef BITS64
     case FLOAT128_TO_INT64:
        test_a_float128_z_int64( slow_float128_to_int64, float128_to_int64 );
        break;
     case FLOAT128_TO_INT64_ROUND_TO_ZERO:
        test_a_float128_z_int64(
            slow_float128_to_int64_round_to_zero,
            float128_to_int64_round_to_zero
        );
        break;
#endif
     case FLOAT128_TO_FLOAT32:
        test_a_float128_z_float32(
            slow_float128_to_float32, float128_to_float32 );
        break;
     case FLOAT128_TO_FLOAT64:
        test_a_float128_z_float64(
            slow_float128_to_float64, float128_to_float64 );
        break;
#ifdef FLOATX80
     case FLOAT128_TO_FLOATX80:
        test_a_float128_z_floatx80(
            slow_float128_to_floatx80, float128_to_floatx80 );
        break;
#endif
#ifdef __HAS_R2I
     case FLOAT128_ROUND_TO_INT:
        test_az_float128( slow_float128_round_to_int, float128_round_to_int );
        break;
#endif
     case FLOAT128_ADD:
        test_abz_float128( slow_float128_add, float128_add );
        break;
     case FLOAT128_SUB:
        test_abz_float128( slow_float128_sub, float128_sub );
        break;
     case FLOAT128_MUL:
        test_abz_float128( slow_float128_mul, float128_mul );
        break;
     case FLOAT128_DIV:
        test_abz_float128( slow_float128_div, float128_div );
        break;
#ifdef __HAS_REM
     case FLOAT128_REM:
        test_abz_float128( slow_float128_rem, float128_rem );
        break;
#endif
     case FLOAT128_SQRT:
        test_az_float128( slow_float128_sqrt, float128_sqrt );
        break;
#ifdef __HAS_PREDICATES
     case FLOAT128_EQ:
        test_ab_float128_z_flag( slow_float128_eq, float128_eq );
        break;
     case FLOAT128_LE:
        test_ab_float128_z_flag( slow_float128_le, float128_le );
        break;
     case FLOAT128_LT:
        test_ab_float128_z_flag( slow_float128_lt, float128_lt );
        break;
#ifdef __HAS_FULL_PREDICATES
     case FLOAT128_EQ_SIGNALING:
        test_ab_float128_z_flag(
            slow_float128_eq_signaling, float128_eq_signaling );
        break;
     case FLOAT128_LE_QUIET:
        test_ab_float128_z_flag( slow_float128_le_quiet, float128_le_quiet );
        break;
     case FLOAT128_LT_QUIET:
        test_ab_float128_z_flag( slow_float128_lt_quiet, float128_lt_quiet );
        break;
#endif /* __HAS_FULL_PREDICATES */
#endif /*__HAS_PREDICATES */
#endif
    }
    if ( ( errorStop && anyErrors ) || stop ) exitWithStatus();

}

static void
 testFunction(
     uint8_t functionCode,
     int8_t roundingPrecisionIn,
     int8_t roundingModeIn,
     int8_t tininessModeIn
 )
{
    int8_t roundingPrecision, roundingMode, tininessMode;

    roundingPrecision = 32;
    for (;;) {
        if ( ! functions[ functionCode ].roundingPrecision ) {
            roundingPrecision = 0;
        }
        else if ( roundingPrecisionIn ) {
            roundingPrecision = roundingPrecisionIn;
        }
        for ( roundingMode = 1;
              roundingMode < NUM_ROUNDINGMODES;
              ++roundingMode
            ) {
            if ( ! functions[ functionCode ].roundingMode ) {
                roundingMode = 0;
            }
            else if ( roundingModeIn ) {
                roundingMode = roundingModeIn;
            }
            for ( tininessMode = 1;
                  tininessMode < NUM_TININESSMODES;
                  ++tininessMode
                ) {
                if (    ( roundingPrecision == 32 )
                     || ( roundingPrecision == 64 ) ) {
                    if ( ! functions[ functionCode ]
                               .tininessModeAtReducedPrecision
                       ) {
                        tininessMode = 0;
                    }
                    else if ( tininessModeIn ) {
                        tininessMode = tininessModeIn;
                    }
                }
                else {
                    if ( ! functions[ functionCode ].tininessMode ) {
                        tininessMode = 0;
                    }
                    else if ( tininessModeIn ) {
                        tininessMode = tininessModeIn;
                    }
                }
                testFunctionVariety(
                    functionCode, roundingPrecision, roundingMode, tininessMode
                );
                if ( tininessModeIn || ! tininessMode ) break;
            }
            if ( roundingModeIn || ! roundingMode ) break;
        }
        if ( roundingPrecisionIn || ! roundingPrecision ) break;
        if ( roundingPrecision == 80 ) {
            break;
        }
        else if ( roundingPrecision == 64 ) {
            roundingPrecision = 80;
        }
        else if ( roundingPrecision == 32 ) {
            roundingPrecision = 64;
        }
    }

}

void testsoftfloat( int argc, const char **argv );

void testsoftfloat( int argc, const char **argv )
{
    const char *argPtr;
    flag functionArgument;

    uint8_t functionCode;
    int8_t operands, roundingPrecision, roundingMode, tininessMode;//, flagsMode ;
//    int i;

    fail_programName = "testsoftfloat";
    // To remove warning...
    functionArgument = 0;
    // To remove warning...
    functionCode = 0;
    // To remove warning...
    operands = 0;
    // To remove warning...
    roundingMode = 0;
    // To remove warning...
    tininessMode = 0;

    if ( argc <= 1 ) goto writeHelpMessage;
    testCases_setLevel( 1 );
    trueName = "true";
    testName = "soft";
    errorStop = FALSE;
    forever = FALSE;
    maxErrorCount = 20;
    trueFlagsPtr = &slow_float_exception_flags;
    testFlagsFunctionPtr = clearFlags;
    functionArgument = FALSE;
    functionCode = 0;
    operands = 0;
    roundingPrecision = 0;
    roundingMode = 0;
    tininessMode = 0;
    --argc;
    ++argv;
    while ( argc && ( argPtr = argv[ 0 ] ) ) {
        if ( argPtr[ 0 ] == '-' ) ++argPtr;
        if ( strcmp( argPtr, "help" ) == 0 ) {
 writeHelpMessage:
            fputs(
"testsoftfloat [<option>...] <function>\n"
"  <option>:  (* is default)\n"
"    -help            --Write this message and exit.\n"
"    -level <num>     --Testing level <num> (1 or 2).\n"
" *  -level 1\n"
"    -errors <num>    --Stop each function test after <num> errors.\n"
" *  -errors 20\n"
"    -errorstop       --Exit after first function with any error.\n"
"    -forever         --Test one function repeatedly (implies `-level 2').\n"
#ifdef FLOATX80
"    -precision32     --Only test rounding precision equivalent to float32.\n"
"    -precision64     --Only test rounding precision equivalent to float64.\n"
"    -precision80     --Only test maximum rounding precision.\n"
#endif
"    -nearesteven     --Only test rounding to nearest/even.\n"
"    -tozero          --Only test rounding to zero.\n"
"    -down            --Only test rounding down.\n"
"    -up              --Only test rounding up.\n"
"    -tininessbefore  --Only test underflow tininess before rounding.\n"
"    -tininessafter   --Only test underflow tininess after rounding.\n"
"  <function>:\n"
"    int32_to_<float>                 <float>_add   <float>_eq\n"
"    <float>_to_int32                 <float>_sub   <float>_le\n"
"    <float>_to_int32_round_to_zero   <float>_mul   <float>_lt\n"
#ifdef BITS64
"    int64_to_<float>                 <float>_div   <float>_eq_signaling\n"
"    <float>_to_int64                 <float>_rem   <float>_le_quiet\n"
"    <float>_to_int64_round_to_zero                 <float>_lt_quiet\n"
"    <float>_to_<float>\n"
"    <float>_round_to_int\n"
"    <float>_sqrt\n"
#else
"    <float>_to_<float>               <float>_div   <float>_eq_signaling\n"
"    <float>_round_to_int             <float>_rem   <float>_le_quiet\n"
"    <float>_sqrt                                   <float>_lt_quiet\n"
#endif
"    -all1            --All 1-operand functions.\n"
"    -all2            --All 2-operand functions.\n"
"    -all             --All functions.\n"
"  <float>:\n"
"    float32          --Single precision.\n"
"    float64          --Double precision.\n"
#ifdef FLOATX80
"    floatx80         --Extended double precision.\n"
#endif
#ifdef FLOAT128
"    float128         --Quadruple precision.\n"
#endif
                ,
                stdout
            );
        }
        else if ( strcmp( argPtr, "level" ) == 0 ) {
            if ( argc < 2 ) goto optionError;
            testCases_setLevel( atoi( argv[ 1 ] ) );
            --argc;
            ++argv;
        }
        else if ( strcmp( argPtr, "level1" ) == 0 ) {
            testCases_setLevel( 1 );
        }
        else if ( strcmp( argPtr, "level2" ) == 0 ) {
            testCases_setLevel( 2 );
        }
        else if ( strcmp( argPtr, "errors" ) == 0 ) {
            if ( argc < 2 ) {
     optionError:
                fail( "`%s' option requires numeric argument", argv[ 0 ] );
            }
            maxErrorCount = atoi( argv[ 1 ] );
            --argc;
            ++argv;
        }
        else if ( strcmp( argPtr, "errorstop" ) == 0 ) {
            errorStop = TRUE;
        }
        else if ( strcmp( argPtr, "forever" ) == 0 ) {
            testCases_setLevel( 2 );
            forever = TRUE;
        }
#ifdef FLOATX80
        else if ( strcmp( argPtr, "precision32" ) == 0 ) {
            roundingPrecision = 32;
        }
        else if ( strcmp( argPtr, "precision64" ) == 0 ) {
            roundingPrecision = 64;
        }
        else if ( strcmp( argPtr, "precision80" ) == 0 ) {
            roundingPrecision = 80;
        }
#endif
        else if (    ( strcmp( argPtr, "nearesteven" ) == 0 )
                  || ( strcmp( argPtr, "nearest_even" ) == 0 ) ) {
            roundingMode = ROUND_NEAREST_EVEN;
        }
        else if (    ( strcmp( argPtr, "tozero" ) == 0 )
                  || ( strcmp( argPtr, "to_zero" ) == 0 ) ) {
            roundingMode = ROUND_TO_ZERO;
        }
        else if ( strcmp( argPtr, "down" ) == 0 ) {
            roundingMode = ROUND_DOWN;
        }
        else if ( strcmp( argPtr, "up" ) == 0 ) {
            roundingMode = ROUND_UP;
        }
        else if ( strcmp( argPtr, "tininessbefore" ) == 0 ) {
            tininessMode = TININESS_BEFORE_ROUNDING;
        }
        else if ( strcmp( argPtr, "tininessafter" ) == 0 ) {
            tininessMode = TININESS_AFTER_ROUNDING;
        }
        else if ( strcmp( argPtr, "all1" ) == 0 ) {
            functionArgument = TRUE;
            functionCode = 0;
            operands = 1;
        }
        else if ( strcmp( argPtr, "all2" ) == 0 ) {
            functionArgument = TRUE;
            functionCode = 0;
            operands = 2;
        }
        else if ( strcmp( argPtr, "all" ) == 0 ) {
            functionArgument = TRUE;
            functionCode = 0;
            operands = 0;
        }
        else {
            for ( functionCode = 1;
                  functionCode < NUM_FUNCTIONS;
                  ++functionCode
                ) {
                if ( strcmp( argPtr, functions[ functionCode ].name ) == 0 ) {
                    break;
                }
            }
            if ( functionCode == NUM_FUNCTIONS ) {
                fail( "Invalid option or function `%s'", argv[ 0 ] );
            }
            functionArgument = TRUE;
        }
        --argc;
        ++argv;
    }
    if ( ! functionArgument ) fail( "Function argument required" );
    (void) signal( SIGINT, catchSIGINT );
    (void) signal( SIGTERM, catchSIGINT );
    if ( functionCode ) {
        if ( forever ) {
            if ( ! roundingPrecision ) roundingPrecision = 80;
            if ( ! roundingMode ) roundingMode = ROUND_NEAREST_EVEN;
        }
        testFunction(
            functionCode, roundingPrecision, roundingMode, tininessMode );
    }
    else {
        if ( operands == 1 ) {
            for ( functionCode = 1;
                  functionCode < NUM_FUNCTIONS;
                  ++functionCode
                ) {
                if ( functions[ functionCode ].numInputs == 1 ) {
                    testFunction(
                        functionCode,
                        roundingPrecision,
                        roundingMode,
                        tininessMode
                    );
                }
            }
        }
        else if ( operands == 2 ) {
            for ( functionCode = 1;
                  functionCode < NUM_FUNCTIONS;
                  ++functionCode
                ) {
                if ( functions[ functionCode ].numInputs == 2 ) {
                    testFunction(
                        functionCode,
                        roundingPrecision,
                        roundingMode,
                        tininessMode
                    );
                }
            }
        }
        else {
            for ( functionCode = 1;
                  functionCode < NUM_FUNCTIONS;
                  ++functionCode
                ) {
                testFunction(
                    functionCode, roundingPrecision, roundingMode, tininessMode
                );
            }
        }
    }
// We don't want 'make check' to exit now...
//    exitWithStatus();

}

