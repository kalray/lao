
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
#include <stdio.h>
#include <math.h>
#include "milieu.h"
#include "softfloat.h"
#include "testCases.h"
#include "writeHex.h"
#include "testLoops.h"

volatile flag stop = FALSE;

const char *trueName, *testName;
flag forever, errorStop;
uint32_t maxErrorCount = 0;
flag checkNaNs = FALSE;
// [HK] ask checks for denormals in case of -D__HAS_SUBNORMAL
#ifdef __HAS_SUBNORMAL
flag checkSubNs = TRUE ;
#else 
flag checkSubNs = FALSE ;
#endif // __HAS_SUBNORMAL
flag checkFlags = TRUE ;
int8_t *trueFlagsPtr;
int8_t ( *testFlagsFunctionPtr )( void );
const char *functionName;
const char *roundingPrecisionName, *roundingModeName, *tininessModeName;
flag anyErrors = FALSE;

void writeFunctionName( FILE *stream )
{

    fputs( functionName, stream );
    if ( roundingModeName ) {
        if ( roundingPrecisionName ) {
            fputs( ", precision ", stream );
            fputs( roundingPrecisionName, stream );
        }
        fputs( ", rounding ", stream );
        fputs( roundingModeName, stream );
        if ( tininessModeName ) {
            fputs( ", tininess ", stream );
            fputs( tininessModeName, stream );
            fputs( " rounding", stream );
        }
    }

}

void exitWithStatus( void )
{

    exit( anyErrors ? EXIT_FAILURE : EXIT_SUCCESS );

}

static uint32_t tenthousandsCount, errorCount = 0;

static void writeTestsTotal( void )
{

    if ( forever ) {
        fputs( "Unbounded tests.\n", stdout );
    }
    else {
        fprintf( stdout, "%d tests total.\n", testCases_total );
    }

}

static void writeTestsPerformed( int16_t count )
{
// Commented that to ease comparison during make check.

    if ( tenthousandsCount ) {
//        fprintf(
//            stdout, "\r%d%04d tests performed", tenthousandsCount, count );
    }
    else {
//        fprintf( stdout, "\r%d tests performed", count );
    }
    if ( errorCount ) {
//        fprintf(
//            stdout,
//            "; %d error%s found.\n",
//            errorCount,
//            ( errorCount == 1 ) ? "" : "s"
//        );
    }
    else {
//        fputs( ".\n", stdout );
        fputs( "No errors found in ", stdout );
        writeFunctionName( stdout );
        fputs( ".\n", stdout );
        fflush( stdout );
    }

}

static void checkEarlyExit( void )
{

    ++tenthousandsCount;
    if ( stop ) {
        writeTestsPerformed( 0 );
        exitWithStatus();
    }
//    fprintf( stdout, "%3d0000", tenthousandsCount );

}

static void writeErrorFound( int16_t count )
{

    fputc( '\r', stdout );
    if ( errorCount == 1 ) {
        fputs( "Errors found in ", stdout );
        writeFunctionName( stdout );
        fputs( ":\n", stdout );
    }
    if ( stop ) {
        writeTestsPerformed( count );
        exitWithStatus();
    }
    anyErrors = TRUE;

}

INLINE void writeInput_a_int32( void )
{

    writeHex_bits32( testCases_a_int32, stdout );

}

#ifdef BITS64

INLINE void writeInput_a_int64( void )
{

    writeHex_bits64( testCases_a_int64, stdout );

}

#endif

INLINE void writeInput_a_float32( void )
{

    writeHex_float32( testCases_a_float32, stdout );

}

static void writeInputs_ab_float32( void )
{

    writeHex_float32( testCases_a_float32, stdout );
    fputs( "  ", stdout );
    writeHex_float32( testCases_b_float32, stdout );

}

INLINE void writeInput_a_float64( void )
{

    writeHex_float64( testCases_a_float64, stdout );

}

static void writeInputs_ab_float64( void )
{

    writeHex_float64( testCases_a_float64, stdout );
    fputs( "  ", stdout );
    writeHex_float64( testCases_b_float64, stdout );

}

#ifdef FLOATX80

INLINE void writeInput_a_floatx80( void )
{

    writeHex_floatx80( testCases_a_floatx80, stdout );

}

static void writeInputs_ab_floatx80( void )
{

    writeHex_floatx80( testCases_a_floatx80, stdout );
    fputs( "  ", stdout );
    writeHex_floatx80( testCases_b_floatx80, stdout );

}

#endif

#ifdef FLOAT128

INLINE void writeInput_a_float128( void )
{

    writeHex_float128( testCases_a_float128, stdout );

}

static void writeInputs_ab_float128( void )
{

    writeHex_float128( testCases_a_float128, stdout );
    fputs( "  ", stdout );
    writeHex_float128( testCases_b_float128, stdout );

}

#endif

static void
 writeOutputs_z_flag(
     flag trueZ, uint8_t trueFlags, flag testZ, uint8_t testFlags )
{

    fputs( trueName, stdout );
    fputs( ": ", stdout );
    writeHex_flag( trueZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( trueFlags, stdout );
    fputs( "  ", stdout );
    fputs( testName, stdout );
    fputs( ": ", stdout );
    writeHex_flag( testZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( testFlags, stdout );
    fputc( '\n', stdout );

}

static void
 writeOutputs_z_int32(
     int32_t trueZ, uint8_t trueFlags, int32_t testZ, uint8_t testFlags )
{

    fputs( trueName, stdout );
    fputs( ": ", stdout );
    writeHex_bits32( trueZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( trueFlags, stdout );
    fputs( "  ", stdout );
    fputs( testName, stdout );
    fputs( ": ", stdout );
    writeHex_bits32( testZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( testFlags, stdout );
    fputc( '\n', stdout );

}

#ifdef BITS64

static void
 writeOutputs_z_int64(
     int64_t trueZ, uint8_t trueFlags, int64_t testZ, uint8_t testFlags )
{

    fputs( trueName, stdout );
    fputs( ": ", stdout );
    writeHex_bits64( trueZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( trueFlags, stdout );
    fputs( "  ", stdout );
    fputs( testName, stdout );
    fputs( ": ", stdout );
    writeHex_bits64( testZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( testFlags, stdout );
    fputc( '\n', stdout );

}

#endif

static void
 writeOutputs_z_float32(
     float32 trueZ, uint8_t trueFlags, float32 testZ, uint8_t testFlags )
{

    fputs( trueName, stdout );
    fputs( ": ", stdout );
    writeHex_float32( trueZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( trueFlags, stdout );
    fputs( "  ", stdout );
    fputs( testName, stdout );
    fputs( ": ", stdout );
    writeHex_float32( testZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( testFlags, stdout );
    fputc( '\n', stdout );

}

static void
 writeOutputs_z_float64(
     float64 trueZ, uint8_t trueFlags, float64 testZ, uint8_t testFlags )
{

    fputs( trueName, stdout );
    fputs( ": ", stdout );
    writeHex_float64( trueZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( trueFlags, stdout );
    fputs( "  ", stdout );
    fputs( testName, stdout );
    fputs( ": ", stdout );
    writeHex_float64( testZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( testFlags, stdout );
    fputc( '\n', stdout );

}

#ifdef FLOATX80

static void
 writeOutputs_z_floatx80(
     floatx80 trueZ, uint8_t trueFlags, floatx80 testZ, uint8_t testFlags )
{

    fputs( trueName, stdout );
    fputs( ": ", stdout );
    writeHex_floatx80( trueZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( trueFlags, stdout );
    fputs( "  ", stdout );
    fputs( testName, stdout );
    fputs( ": ", stdout );
    writeHex_floatx80( testZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( testFlags, stdout );
    fputc( '\n', stdout );

}

#endif

#ifdef FLOAT128

static void
 writeOutputs_z_float128(
     float128 trueZ, uint8_t trueFlags, float128 testZ, uint8_t testFlags )
{

    fputs( trueName, stdout );
    fputs( ": ", stdout );
    writeHex_float128( trueZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( trueFlags, stdout );
    fputs( "\n\t", stdout );
    fputs( testName, stdout );
    fputs( ": ", stdout );
    writeHex_float128( testZ, stdout );
    fputc( ' ', stdout );
    writeHex_float_flags( testFlags, stdout );
    fputc( '\n', stdout );

}

#endif

INLINE flag float32_isNaN( float32 a )
{

    return isnan(*(float*)&a) ;
    /*    return 0x7F800000 < ( a & 0x7FFFFFFF );*/

}

INLINE flag float32_isSubN( float32 a )
{

    /*return ( ( ( a>>23 ) & 0xFF ) == 0x00 ) && ( a & 0x003FFFFF );*/
    return fpclassify(*(float*)&(a)) == FP_SUBNORMAL;

}

/* Imported from sofloat, specialization identical for SPARC and 386 */
/*
-------------------------------------------------------------------------------
Returns 1 if the single-precision floating-point value `a' is a signaling
NaN; otherwise returns 0.
-------------------------------------------------------------------------------
*/
flag float32_is_signaling_nan( float32 a )
{
#ifdef __HAS_SIGNALINGNANS
    return ( ( ( a>>22 ) & 0x1FF ) == 0x1FE ) && ( a & 0x003FFFFF ); 
#else
    return 0 ;
#endif    

}

#ifdef BITS64

INLINE flag float64_same( float64 a, float64 b )
{

    return a == b;

}

INLINE flag float64_isNaN( float64 a )
{
    return isnan(*(double*)&a) ;
    /* return LIT64( 0x7FF0000000000000 ) < ( a & LIT64( 0x7FFFFFFFFFFFFFFF ) ); */

}

INLINE flag float64_isSubN( float64 a )
{
    return fpclassify(*(double*)&a) == FP_SUBNORMAL;
    /*    return ( ( ( a>>52 ) & 0x7FF ) == 0x000 ) && (  a & LIT64( 0x0007FFFFFFFFFFFF ) );*/

}

/* Imported from sofloat, specialization identical for SPARC and 386 */
/*
-------------------------------------------------------------------------------
Returns 1 if the double-precision floating-point value `a' is a signaling
NaN; otherwise returns 0.
-------------------------------------------------------------------------------
*/
flag float64_is_signaling_nan( float64 a )
{
#ifdef __HAS_SIGNALINGNANS
    return
           ( ( ( a>>51 ) & 0xFFF ) == 0xFFE )
	   && ( a & LIT64( 0x0007FFFFFFFFFFFF ) ); 
#else
    return 0 ;
#endif

}


#else

INLINE flag float64_same( float64 a, float64 b )
{

    return ( a.high == b.high ) && ( a.low == b.low );

}

INLINE flag float64_isNaN( float64 a )
{
    bits32 absAHigh;

    absAHigh = a.high & 0x7FFFFFFF;
    return
        ( 0x7FF00000 < absAHigh ) || ( ( absAHigh == 0x7FF00000 ) && a.low );

}

#endif

#ifdef FLOATX80

INLINE flag floatx80_same( floatx80 a, floatx80 b )
{

    return ( a.high == b.high ) && ( a.low == b.low );

}

INLINE flag floatx80_isNaN( floatx80 a )
{

    return ( ( a.high & 0x7FFF ) == 0x7FFF ) && a.low;

}

#endif

#ifdef FLOAT128

INLINE flag float128_same( float128 a, float128 b )
{

    return ( a.high == b.high ) && ( a.low == b.low );

}

INLINE flag float128_isNaN( float128 a )
{
    bits64 absAHigh;

    absAHigh = a.high & LIT64( 0x7FFFFFFFFFFFFFFF );
    return
           ( LIT64( 0x7FFF000000000000 ) < absAHigh )
        || ( ( absAHigh == LIT64( 0x7FFF000000000000 ) ) && a.low );

}

#endif

void
 test_a_int32_z_float32(
     float32 trueFunction( int32_t ), float32 testFunction( int32_t ) )
{
    int16_t count;
    float32 trueZ, testZ;
    uint8_t trueFlags, testFlags;
    float32 testCases_a_int32_to_float;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_int32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            testCases_a_int32_to_float = testCases_a_int32;
            if ((    ! checkNaNs
                 && float32_isNaN( trueZ )
                 && float32_isNaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float32_isSubN( trueZ ) || float32_isSubN( testZ ) || float32_isSubN(testCases_a_int32_to_float)) 
	      )) {
                /* no problem */  
	    }         
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int32();
                fputs( "  ", stdout );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_a_int32_z_float64(
     float64 trueFunction( int32_t ), float64 testFunction( int32_t ) )
{
    int16_t count;
    float64 trueZ, testZ;
    uint8_t trueFlags, testFlags;
    float32 testCases_a_int32_to_float;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_int32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            testCases_a_int32_to_float = testCases_a_int32;
            if ((    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float64_isSubN( trueZ ) || float64_isSubN( testZ )  || float32_isSubN( testCases_a_int32_to_float ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int32();
                fputs( "  ", stdout );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOATX80

void
 test_a_int32_z_floatx80(
     floatx80 trueFunction( int32_t ), floatx80 testFunction( int32_t ) )
{
    int16_t count;
    floatx80 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_int32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if ((    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float80_isSubN( trueZ ) || float80_isSubN( testZ )  || float32_isSubN( testCases_a_int32 ) )
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int32();
                fputs( "  ", stdout );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#ifdef FLOAT128

void
 test_a_int32_z_float128(
     float128 trueFunction( int32_t ), float128 testFunction( int32_t ) )
{
    int16_t count;
    float128 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_int32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if ((    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && ( float128_isSubN( trueZ ) || float128_isSubN( testZ ) || float32_isSubN( testCases_a_int32 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int32();
                fputs( "\n\t", stdout );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#ifdef BITS64

void
 test_a_int64_z_float32(
     float32 trueFunction( int64_t ), float32 testFunction( int64_t ) )
{
    int16_t count;
    float32 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_int64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if ((    ! checkNaNs
                 && float32_isNaN( trueZ )
                 && float32_isNaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float32_isSubN( trueZ ) || float32_isSubN( testZ ) || float64_isSubN( testCases_a_int64 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int64();
                fputs( "  ", stdout );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_a_int64_z_float64(
     float64 trueFunction( int64_t ), float64 testFunction( int64_t ) )
{
    int16_t count;
    float64 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_int64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if ((    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float64_isSubN( trueZ ) || float64_isSubN( testZ ) || float64_isSubN( testCases_a_int64 ) )
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int64();
                fputs( "  ", stdout );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOATX80

void
 test_a_int64_z_floatx80(
     floatx80 trueFunction( int64_t ), floatx80 testFunction( int64_t ) )
{
    int16_t count;
    floatx80 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_int64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if ((    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (floatx80_isSubN( trueZ ) || floatx80_isSubN( testZ ) || float64_isSubN( testCases_a_int64 ) )
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int64();
                fputs( "  ", stdout );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#ifdef FLOAT128

void
 test_a_int64_z_float128(
     float128 trueFunction( int64_t ), float128 testFunction( int64_t ) )
{
    int16_t count;
    float128 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_int64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_int64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_int64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if ((    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float128_isSubN( trueZ ) || float128_isSubN( testZ ) || float64_isSubN(testCases_a_int64))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_int64();
                fputs( "\n\t", stdout );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#endif

void
 test_a_float32_z_int32(
     int32_t trueFunction( float32 ), int32_t testFunction( float32 ) )
{
    int16_t count;
    int32_t trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if (    ( trueZ == 0x7FFFFFFF )
                 && (    ( testZ == 0x7FFFFFFF )
                      || ( testZ == (sbits32) 0x80000000 ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                fputs( "  ", stdout );
                writeOutputs_z_int32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef BITS64

void
 test_a_float32_z_int64(
     int64_t trueFunction( float32 ), int64_t testFunction( float32 ) )
{
    int16_t count;
    int64_t trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if (    ( trueZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                 && (    ( testZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                      || ( testZ == (sbits64) LIT64( 0x8000000000000000 ) ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                fputs( "  ", stdout );
                writeOutputs_z_int64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_a_float32_z_float64(
     float64 trueFunction( float32 ), float64 testFunction( float32 ) )
{
    int16_t count;
    float64 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float64_isSubN( trueZ )
		     || float64_isSubN( testZ ) || float32_isSubN( testCases_a_float32 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                fputs( "  ", stdout );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOATX80

void
 test_a_float32_z_floatx80(
     floatx80 trueFunction( float32 ), floatx80 testFunction( float32 ) )
{
    int16_t count;
    floatx80 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (floatx80_isSubN( trueZ ) || floatx80_isSubN( testZ ) || float32_isSubN( testCases_a_float32 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                fputs( "\n\t", stdout );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#ifdef FLOAT128

void
 test_a_float32_z_float128(
     float128 trueFunction( float32 ), float128 testFunction( float32 ) )
{
    int16_t count;
    float128 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float128_isSubN( trueZ ) || float128_isSubN( testZ ) || float32_isSubN( testCases_a_float32 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                fputs( "\n\t", stdout );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_az_float32(
     float32 trueFunction( float32 ), float32 testFunction( float32 ) )
{
    int16_t count;
    float32 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float32_is_signaling_nan( testCases_a_float32 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float32_isNaN( trueZ )
                 && float32_isNaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float32_isSubN( trueZ ) || float32_isSubN( testZ ) || float32_isSubN( testCases_a_float32 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float32();
                fputs( "  ", stdout );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_ab_float32_z_flag(
     flag trueFunction( float32, float32 ),
     flag testFunction( float32, float32 )
 )
{
    int16_t count;
    flag trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32, testCases_b_float32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float32, testCases_b_float32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    float32_is_signaling_nan( testCases_a_float32 )
                      || float32_is_signaling_nan( testCases_b_float32 ) )
               ) {
                trueFlags |= float_flag_invalid;
            }
            if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_float32();
                fputs( "  ", stdout );
                writeOutputs_z_flag( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );
    return;

}

void
 test_abz_float32(
     float32 trueFunction( float32, float32 ),
     float32 testFunction( float32, float32 )
 )
{
    int16_t count;
    float32 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_float32 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float32, testCases_b_float32 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float32, testCases_b_float32 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    float32_is_signaling_nan( testCases_a_float32 )
                      || float32_is_signaling_nan( testCases_b_float32 ) )
               ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float32_isNaN( trueZ )
                 && float32_isNaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float32_isSubN( trueZ ) || float32_isSubN( testZ ) || float32_isSubN( testCases_a_float32 ) || float32_isSubN( testCases_b_float32 ))
	      )){
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_float32();
                fputs( "  ", stdout );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );
    return;

}

void
 test_a_float64_z_int32(
     int32_t trueFunction( float64 ), int32_t testFunction( float64 ) )
{
    int16_t count;
    int32_t trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if (    ( trueZ == 0x7FFFFFFF )
                 && (    ( testZ == 0x7FFFFFFF )
                      || ( testZ == (sbits32) 0x80000000 ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                fputs( "  ", stdout );
                writeOutputs_z_int32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef BITS64

void
 test_a_float64_z_int64(
     int64_t trueFunction( float64 ), int64_t testFunction( float64 ) )
{
    int16_t count;
    int64_t trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if (    ( trueZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                 && (    ( testZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                      || ( testZ == (sbits64) LIT64( 0x8000000000000000 ) ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                fputs( "  ", stdout );
                writeOutputs_z_int64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_a_float64_z_float32(
     float32 trueFunction( float64 ), float32 testFunction( float64 ) )
{
    int16_t count;
    float32 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float32_isNaN( trueZ )
                 && float32_isNaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float32_isSubN( trueZ ) || float32_isSubN( testZ ) || float64_isSubN( testCases_a_float64 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                fputs( "  ", stdout );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOATX80

void
 test_a_float64_z_floatx80(
     floatx80 trueFunction( float64 ), floatx80 testFunction( float64 ) )
{
    int16_t count;
    floatx80 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		       && (floatx80_isSubN( trueZ )|| floatx80_isSubN( testZ ) || float64_isSubN( testCases_a_float64 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                fputs( "\n\t", stdout );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

#ifdef FLOAT128

void
 test_a_float64_z_float128(
     float128 trueFunction( float64 ), float128 testFunction( float64 ) )
{
    int16_t count;
    float128 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float128_isSubN( trueZ )
		     || float128_isSubN( testZ ) || float64_isSubN( testCases_a_float64 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                fputs( "\n\t", stdout );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_az_float64(
     float64 trueFunction( float64 ), float64 testFunction( float64 ) )
{
    int16_t count;
    float64 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float64_is_signaling_nan( testCases_a_float64 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float64_isSubN( trueZ ) || float64_isSubN( testZ ) || float64_isSubN( testCases_a_float64 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float64();
                fputs( "  ", stdout );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_ab_float64_z_flag(
     flag trueFunction( float64, float64 ),
     flag testFunction( float64, float64 )
 )
{
    int16_t count;
    flag trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64, testCases_b_float64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float64, testCases_b_float64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    float64_is_signaling_nan( testCases_a_float64 )
                      || float64_is_signaling_nan( testCases_b_float64 ) )
               ) {
                trueFlags |= float_flag_invalid;
            } 
            if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_float64();
                fputs( "  ", stdout );
                writeOutputs_z_flag( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );
    return;

}

void
 test_abz_float64(
     float64 trueFunction( float64, float64 ),
     float64 testFunction( float64, float64 )
 )
{
    int16_t count;
    float64 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_float64 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float64, testCases_b_float64 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float64, testCases_b_float64 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    float64_is_signaling_nan( testCases_a_float64 )
                      || float64_is_signaling_nan( testCases_b_float64 ) )
               ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            } 
            if ((    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float64_isSubN( trueZ )
		     || float64_isSubN( testZ ) || float64_isSubN( testCases_a_float64 ) || float64_isSubN( testCases_b_float64 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_float64();
                fputs( "\n\t", stdout );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );
    return;

}

#ifdef FLOATX80

void
 test_a_floatx80_z_int32(
     int32_t trueFunction( floatx80 ), int32_t testFunction( floatx80 ) )
{
    int16_t count;
    int32_t trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            } 
            if (    ( trueZ == 0x7FFFFFFF )
                 && (    ( testZ == 0x7FFFFFFF )
                      || ( testZ == (sbits32) 0x80000000 ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                fputs( "  ", stdout );
                writeOutputs_z_int32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef BITS64

void
 test_a_floatx80_z_int64(
     int64_t trueFunction( floatx80 ), int64_t testFunction( floatx80 ) )
{
    int16_t count;
    int64_t trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if (    ( trueZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                 && (    ( testZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                      || ( testZ == (sbits64) LIT64( 0x8000000000000000 ) ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                fputs( "  ", stdout );
                writeOutputs_z_int64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_a_floatx80_z_float32(
     float32 trueFunction( floatx80 ), float32 testFunction( floatx80 ) )
{
    int16_t count;
    float32 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float32_isNaN( trueZ )
                 && float32_isNaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float32_isSubN( trueZ )
		     || float32_isSubN( testZ ) || floatx80_isSubN( testCases_a_floatx80 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                fputs( "  ", stdout );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_a_floatx80_z_float64(
     float64 trueFunction( floatx80 ), float64 testFunction( floatx80 ) )
{
    int16_t count;
    float64 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float64_isSubN( trueZ ) || float64_isSubN( testZ ) || floatx80_isSubN( testCases_a_floatx80 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                fputs( "\n\t", stdout );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOAT128

void
 test_a_floatx80_z_float128(
     float128 trueFunction( floatx80 ), float128 testFunction( floatx80 ) )
{
    int16_t count;
    float128 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float128_isSubN( trueZ ) || float128_isSubN( testZ ) || floatx80_isSubN( testCases_a_floatx80 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                fputs( "\n\t", stdout );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_az_floatx80(
     floatx80 trueFunction( floatx80 ), floatx80 testFunction( floatx80 ) )
{
    int16_t count;
    floatx80 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_floatx80 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && floatx80_is_signaling_nan( testCases_a_floatx80 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (floatx80_isSubN( trueZ ) || floatx80_isSubN( testZ ) || floatx80_isSubN( testCases_a_floatx80 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_floatx80();
                fputs( "\n\t", stdout );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_ab_floatx80_z_flag(
     flag trueFunction( floatx80, floatx80 ),
     flag testFunction( floatx80, floatx80 )
 )
{
    int16_t count;
    flag trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80, testCases_b_floatx80 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_floatx80, testCases_b_floatx80 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    floatx80_is_signaling_nan( testCases_a_floatx80 )
                      || floatx80_is_signaling_nan( testCases_b_floatx80 ) )
               ) {
                trueFlags |= float_flag_invalid;
            }
            if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_floatx80();
                fputs( "  ", stdout );
                writeOutputs_z_flag( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );
    return;

}

void
 test_abz_floatx80(
     floatx80 trueFunction( floatx80, floatx80 ),
     floatx80 testFunction( floatx80, floatx80 )
 )
{
    int16_t count;
    floatx80 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_floatx80 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_floatx80, testCases_b_floatx80 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_floatx80, testCases_b_floatx80 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    floatx80_is_signaling_nan( testCases_a_floatx80 )
                      || floatx80_is_signaling_nan( testCases_b_floatx80 ) )
               ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (floatx80_isSubN( trueZ ) || floatx80_isSubN( testZ ) || floatx80_isSubN( testCases_a_floatx80 ) || floatx80_isSubN( testCases_b_floatx80 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_floatx80();
                fputs( "\n\t", stdout );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );
    return;

}

#endif

#ifdef FLOAT128

void
 test_a_float128_z_int32(
     int32_t trueFunction( float128 ), int32_t testFunction( float128 ) )
{
    int16_t count;
    int32_t trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if (    ( trueZ == 0x7FFFFFFF )
                 && (    ( testZ == 0x7FFFFFFF )
                      || ( testZ == (sbits32) 0x80000000 ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                fputs( "  ", stdout );
                writeOutputs_z_int32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef BITS64

void
 test_a_float128_z_int64(
     int64_t trueFunction( float128 ), int64_t testFunction( float128 ) )
{
    int16_t count;
    int64_t trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if (    ( trueZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                 && (    ( testZ == LIT64( 0x7FFFFFFFFFFFFFFF ) )
                      || ( testZ == (sbits64) LIT64( 0x8000000000000000 ) ) )
                 && ( trueFlags == float_flag_invalid )
                 && ( testFlags == float_flag_invalid )
               ) {
                /* no problem */
            }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                fputs( "\n\t", stdout );
                writeOutputs_z_int64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_a_float128_z_float32(
     float32 trueFunction( float128 ), float32 testFunction( float128 ) )
{
    int16_t count;
    float32 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float32_isNaN( trueZ )
                 && float32_isNaN( testZ )
                 && ! float32_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float32_isSubN( trueZ ) || float32_isSubN( testZ ) || float128_isSubN( testCases_a_float128 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                fputs( "  ", stdout );
                writeOutputs_z_float32( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_a_float128_z_float64(
     float64 trueFunction( float128 ), float64 testFunction( float128 ) )
{
    int16_t count;
    float64 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float64_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float64_isNaN( trueZ )
                 && float64_isNaN( testZ )
                 && ! float64_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float64_isSubN( trueZ ) || float64_isSubN( testZ ) || float128_isSubN( testCases_a_float128 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                fputs( "\n\t", stdout );
                writeOutputs_z_float64( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#ifdef FLOATX80

void
 test_a_float128_z_floatx80(
     floatx80 trueFunction( float128 ), floatx80 testFunction( float128 ) )
{
    int16_t count;
    floatx80 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! floatx80_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && floatx80_isNaN( trueZ )
                 && floatx80_isNaN( testZ )
                 && ! floatx80_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (floatx80_isSubN( trueZ ) || floatx80_isSubN( testZ ) || float128_isSubN( testCases_a_float128 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                fputs( "\n\t", stdout );
                writeOutputs_z_floatx80( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

#endif

void
 test_az_float128(
     float128 trueFunction( float128 ), float128 testFunction( float128 ) )
{
    int16_t count;
    float128 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_a_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float128 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && float128_is_signaling_nan( testCases_a_float128 ) ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float128_isSubN( trueZ ) || float128_isSubN( testZ ) || float128_isSubN( testCases_a_float128 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInput_a_float128();
                fputs( "\n\t", stdout );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );

}

void
 test_ab_float128_z_flag(
     flag trueFunction( float128, float128 ),
     flag testFunction( float128, float128 )
 )
{
    int16_t count;
    flag trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128, testCases_b_float128 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float128, testCases_b_float128 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    float128_is_signaling_nan( testCases_a_float128 )
                      || float128_is_signaling_nan( testCases_b_float128 ) )
               ) {
                trueFlags |= float_flag_invalid;
            }
            if ( ( trueZ != testZ ) || ( trueFlags != testFlags ) ) {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_float128();
                fputs( "\n\t", stdout );
                writeOutputs_z_flag( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );
    return;

}

void
 test_abz_float128(
     float128 trueFunction( float128, float128 ),
     float128 testFunction( float128, float128 )
 )
{
    int16_t count;
    float128 trueZ, testZ;
    uint8_t trueFlags, testFlags;

    errorCount = 0;
    tenthousandsCount = 0;
    count = 10000;
    testCases_initSequence( testCases_sequence_ab_float128 );
    writeTestsTotal();
    while ( ! testCases_done || forever ) {
        testCases_next();
        *trueFlagsPtr = 0;
        trueZ = trueFunction( testCases_a_float128, testCases_b_float128 );
        trueFlags = *trueFlagsPtr;
        (void) testFlagsFunctionPtr();
        testZ = testFunction( testCases_a_float128, testCases_b_float128 );
        testFlags = testFlagsFunctionPtr();
	if (!checkFlags) testFlags = trueFlags ;
        --count;
        if ( count == 0 ) {
            checkEarlyExit();
            count = 10000;
        }
        if ( ! float128_same( trueZ, testZ ) || ( trueFlags != testFlags ) ) {
            if (    ! checkNaNs
                 && (    float128_is_signaling_nan( testCases_a_float128 )
                      || float128_is_signaling_nan( testCases_b_float128 ) )
               ) {
                trueFlags |= float_flag_invalid;
		if (!checkFlags) testFlags = trueFlags ;
            }
            if ((    ! checkNaNs
                 && float128_isNaN( trueZ )
                 && float128_isNaN( testZ )
                 && ! float128_is_signaling_nan( testZ )
                 && ( trueFlags == testFlags )
               ) || (  ! checkSubNs 
		 && (float128_isSubN( trueZ )
		     || float128_isSubN( testZ ) || float128_isSubN( testCases_a_float128 ) || float128_isSubN( testCases_b_float128 ))
	      )) {
		/* no problem */
	    }
            else {
                ++errorCount;
                writeErrorFound( 10000 - count );
                writeInputs_ab_float128();
                fputs( "\n\t", stdout );
                writeOutputs_z_float128( trueZ, trueFlags, testZ, testFlags );
                fflush( stdout );
                if ( errorCount == maxErrorCount ) goto exit;
            }
        }
    }
 exit:
    writeTestsPerformed( 10000 - count );
    return;

}

#endif

