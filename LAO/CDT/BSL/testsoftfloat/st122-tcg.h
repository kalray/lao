/*
-------------------------------------------------------------------------------
One of the macros `BIGENDIAN' or `LITTLEENDIAN' must be defined.
-------------------------------------------------------------------------------
*/
#define LITTLEENDIAN

/*
-------------------------------------------------------------------------------
The macro `BITS64' can be defined to indicate that 64-bit integer types are
supported by the compiler.
-------------------------------------------------------------------------------
*/
#define BITS64


/*
-------------------------------------------------------------------------------
The macro `INLINE' can be used before functions that should be inlined.  If
a compiler does not support explicit inlining, this macro should be defined
to be `static'.
-------------------------------------------------------------------------------
*/
#define INLINE static

#ifdef BITS64
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
#endif
