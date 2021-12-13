#ifdef $XCC_h
/*
 * !!!!	Int256.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@gmail.com)
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

#ifdef $XCC_h
/**
 * @file
 * @ingroup BSL
 * @brief Arithmetic support for 256-bit integers.
 */
#endif//$XCC_h


#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/Int256.h"
#endif//$XCC__h


#ifdef $XCC_h
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#ifdef __SIZEOF_INT128__
#ifndef TYPE_INT128
#define TYPE_INT128
typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;
#endif//TYPE_INT128
#endif//__SIZEOF_INT128__
#endif //$XCC_h

#if 0
#define DEBUG_INT256 1
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
union Int256_ {
#ifdef TYPE_INT128
  uint128_t qwords[2];
#endif//TYPE_INT128
  uint64_t dwords[4];
  uint32_t words[8];
  uint16_t hwords[16];
  uint8_t bytes[32];
  struct {
    Int128_ lower;
    Int128_ upper;
  };
};
typedef union Int256_ Int256_, *Int256;
#endif//$XCC_h

#ifdef $XCC_h
#define Int256_zero	Int256_fromUInt64(0LL)
#define Int256_dwords_0(this)	(this).dwords[0]
#define Int256_dwords_1(this)	(this).dwords[1]
#define Int256_dwords_2(this)	(this).dwords[2]
#define Int256_dwords_3(this)	(this).dwords[3]
#endif//$XCC_h

#ifdef $XCC_h
static inline void
Int256_Ctor(Int256_ *output, uint64_t dword_0, uint64_t dword_1, uint64_t dword_2, uint64_t dword_3)
{
  output->dwords[0] = dword_0;
  output->dwords[1] = dword_1;
  output->dwords[2] = dword_2;
  output->dwords[3] = dword_3;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Int256_
Int256_make(uint64_t dword_0, uint64_t dword_1, uint64_t dword_2, uint64_t dword_3)
{
  Int256_ output;
  output.dwords[0] = dword_0;
  output.dwords[1] = dword_1;
  output.dwords[2] = dword_2;
  output.dwords[3] = dword_3;
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
void
Int256_print(Int256_ this);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
void
Int256_print(Int256_ this)
{
  for (int j = 3; j >= 0; --j) {
    printf("%016" PRIx64, this.dwords[j]);
    printf("|");
  }
  printf("\n");
}
#endif//_NTRACE

#ifdef $XCC_h
static inline int
Int256_equals(Int256_ this, Int256_ that)
{
#ifdef TYPE_INT128
  return (this.qwords[0] == that.qwords[0]) &&
         (this.qwords[1] == that.qwords[1]);
#else//TYPE_INT128
  return (this.dwords[0] == that.dwords[0]) &&
         (this.dwords[1] == that.dwords[1]) &&
         (this.dwords[2] == that.dwords[2]) &&
         (this.dwords[3] == that.dwords[3]);
#endif//TYPE_INT128
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Signed saturate.
 */
Int256_
Int256_sat(Int256_ this, unsigned width);
#endif//$XCC_h

Int256_
Int256_sat(Int256_ this, unsigned width)
{
  Int256_ output = this;
  uint64_t sign = -(this.upper.dwords[1] >> (64-1));
  if (width > 4*64);
  else if (width > 3*64) {
    // width in [3*64+1..4*64]
    int64_t smin = -(1LL<<(width-3*64-1));
    int64_t smax = (1LL<<(width-3*64-1)) - 1;
    if ((int64_t)this.dwords[3] < smin) {
      output.dwords[0] = 0ULL;
      output.dwords[1] = 0ULL;
      output.dwords[2] = 0ULL;
      output.dwords[3] = smin;
    } else if ((int64_t)this.dwords[3] > smax) {
      output.dwords[0] = -1ULL;
      output.dwords[1] = -1ULL;
      output.dwords[2] = -1ULL;
      output.dwords[3] = smax;
    }
  } else if (width > 2*64) {
    // width in [2*64+1..3*64]
    int64_t smin = -(1LL<<(width-2*64-1));
    int64_t smax = (1LL<<(width-2*64-1)) - 1;
    if ((int64_t)this.dwords[3] < 0) {
      if (   (int64_t)this.dwords[3] != -1LL
          || (uint64_t)this.dwords[2] < (uint64_t)smin) {
        output.dwords[0] = 0ULL;
        output.dwords[1] = 0ULL;
        output.dwords[2] = smin;
        output.dwords[3] = -1ULL;
      }
    } else {
      if (   (int64_t)this.dwords[3] != 0ULL
          || (uint64_t)this.dwords[2] > (uint64_t)smax) {
        output.dwords[0] = -1ULL;
        output.dwords[1] = -1ULL;
        output.dwords[2] = smax;
        output.dwords[3] = 0ULL;
      }
    }
  } else if (width > 64) {
    // width in [64+1..2*64]
    int64_t smin = -(1LL<<(width-64-1));
    int64_t smax = (1LL<<(width-64-1)) - 1;
    if ((int64_t)this.dwords[3] < 0) {
      if (   (int64_t)this.dwords[3] != -1LL
          || (int64_t)this.dwords[2] != -1LL
          || (uint64_t)this.dwords[1] < (uint64_t)smin) {
        output.dwords[0] = 0ULL;
        output.dwords[1] = smin;
        output.dwords[2] = -1ULL;
        output.dwords[3] = -1ULL;
      }
    } else {
      if (   (int64_t)this.dwords[3] != 0ULL
          || (int64_t)this.dwords[2] != 0ULL
          || (uint64_t)this.dwords[1] > (uint64_t)smax) {
        output.dwords[0] = -1ULL;
        output.dwords[1] = smax;
        output.dwords[2] = 0ULL;
        output.dwords[3] = 0ULL;
      }
    }
  } else if (width > 0) {
    // width in [1..64]
    int64_t smin = -(1LL<<(width-1));
    int64_t smax = (1LL<<(width-1)) - 1;
    if ((int64_t)this.dwords[3] < 0) {
      if (   (int64_t)this.dwords[3] != -1LL
          || (int64_t)this.dwords[2] != -1LL
          || (int64_t)this.dwords[1] != -1LL
          || (uint64_t)this.dwords[0] < (uint64_t)smin) {
        output.dwords[0] = smin;
        output.dwords[1] = -1ULL;
        output.dwords[2] = -1ULL;
        output.dwords[3] = -1ULL;
      }
    } else {
      if (   (int64_t)this.dwords[3] != 0ULL
          || (int64_t)this.dwords[2] != 0ULL
          || (int64_t)this.dwords[1] != 0ULL
          || (uint64_t)this.dwords[0] > (uint64_t)smax) {
        output.dwords[0] = smax;
        output.dwords[1] = 0ULL;
        output.dwords[2] = 0ULL;
        output.dwords[3] = 0ULL;
      }
    }
  } else
    __builtin_trap();
  return output;
}

#ifdef $XCC_h
/*
 * Unsigned saturate.
 */
Int256_
Int256_satu(Int256_ this, unsigned width);
#endif//$XCC_h

Int256_
Int256_satu(Int256_ this, unsigned width)
{
  Int256_ output = this;
  if ((uint64_t)this.dwords[3] >> 64-1) {
    // Negative, saturate to zero.
    output.dwords[0] = 0ULL;
    output.dwords[1] = 0ULL;
    output.dwords[2] = 0ULL;
    output.dwords[3] = 0ULL;
    return output;
  }
  if (width >= 4*64);
  else if (width >= 3*64) {
    // width in [3*64..4*64-1]
    uint64_t umax = (1ULL<<(width-3*64)) - 1;
    if ((uint64_t)this.dwords[3] > umax) {
      output.dwords[0] = -1ULL;
      output.dwords[1] = -1ULL;
      output.dwords[2] = -1ULL;
      output.dwords[3] = umax;
    }
  } else if (width >= 2*64) {
    // width in [2*64..3*64-1]
    uint64_t umax = (1ULL<<(width-2*64)) - 1;
    if (   (uint64_t)this.dwords[3] != 0ULL
        || (uint64_t)this.dwords[2] > umax) {
      output.dwords[0] = -1ULL;
      output.dwords[1] = -1ULL;
      output.dwords[2] = umax;
      output.dwords[3] = 0ULL;
    }
  } else if (width >= 64) {
    // width in [64..2*64-1]
    uint64_t umax = (1ULL<<(width-64)) - 1;
    if (   (uint64_t)this.dwords[3] != 0ULL
        || (uint64_t)this.dwords[2] != 0ULL
        || (uint64_t)this.dwords[1] > umax) {
      output.dwords[0] = -1ULL;
      output.dwords[1] = umax;
      output.dwords[2] = 0ULL;
      output.dwords[3] = 0ULL;
    }
  } else if (width > 0) {
    // width in [1..64-1]
    uint64_t umax = (1ULL<<width) - 1;
    if (   (uint64_t)this.dwords[3] != 0ULL
        || (uint64_t)this.dwords[2] != 0ULL
        || (uint64_t)this.dwords[1] != 0ULL
        || (uint64_t)this.dwords[0] > umax) {
      output.dwords[0] = umax;
      output.dwords[1] = 0ULL;
      output.dwords[2] = 0ULL;
      output.dwords[3] = 0ULL;
    }
  } else
    __builtin_trap();
  return output;
}

#ifdef $XCC_h
/*
 * Unsigned comparison.
 * if this == that -> return 0
 * if this > that -> return 1
 * if this < that -> return -1
 */
static inline int
Int256_cmpu(Int256_ this, Int256_ that)
{
  int result = Int128_cmpu(this.upper, that.upper);
  if (!result) {
    result = Int128_cmpu(this.lower, that.lower);
  }
  return result;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Signed comparison.
 * if this == that -> return 0
 * if this > that -> return 1
 * if this < that -> return -1
 */
static inline int
Int256_cmp(Int256_ this, Int256_ that)
{
  int result = Int128_cmp(this.upper, that.upper);
  if (!result) {
    result = Int128_cmp(this.lower, that.lower);
  }
  return result;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Bitwise OR.
 */
static inline Int256_
Int256_or(Int256_ this, Int256_ that)
{
  Int256_ output;
  output.lower = Int128_or(this.lower, that.lower);
  output.upper = Int128_or(this.upper, that.upper);
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Bitwise XOR.
 */
static inline Int256_
Int256_xor(Int256_ this, Int256_ that)
{
  Int256_ output;
  output.lower = Int128_xor(this.lower, that.lower);
  output.upper = Int128_xor(this.upper, that.upper);
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Bitwise AND.
 */
static inline Int256_
Int256_and(Int256_ this, Int256_ that)
{
  Int256_ output;
  output.lower = Int128_and(this.lower, that.lower);
  output.upper = Int128_and(this.upper, that.upper);
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Bitwise NOT.
 */
static inline Int256_
Int256_not(Int256_ this)
{
  Int256_ output;
  output.lower = Int128_not(this.lower);
  output.upper = Int128_not(this.upper);
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Count Leading Zeros (CLZ(0) = width).
 */
unsigned
Int256_clz(Int256_ this, unsigned width);
#endif//$XCC_h

unsigned
Int256_clz(Int256_ this, unsigned width)
{
  unsigned clz = width;
  assert(width <= 256);
#ifdef DEBUG_INT256
  printf("Input clz.%d:\n", width);
  Int256_print(this);
#endif
  Int256_ temp = Int256_shl_(this, 256 - width);
  for (int i = 3; i >= 0; --i) {
    if (temp.dwords[i]) {
      clz = 64*(3-i) + __builtin_clzll(temp.dwords[i]);
      break;
    }
  }
#ifdef DEBUG_INT256
  printf("Output: %i\n", clz);
#endif
  return clz;
}

#ifdef $XCC_h
/*
 * Count Trailing Zeros (CTZ(0) = width).
 */
unsigned
Int256_ctz(Int256_ this, unsigned width);
#endif//$XCC_h

unsigned
Int256_ctz(Int256_ this, unsigned width)
{
  unsigned ctz = width;
  assert(width <= 256);
#ifdef DEBUG_INT256
  printf("Input ctz.%d:\n", width);
  Int256_print(this);
#endif
  for (int i = 0; i <= 3; ++i) {
    if (this.dwords[i]) {
      ctz = 64*i + __builtin_ctzll(this.dwords[i]);
      if (ctz > width) ctz = width;
      break;
    }
  }
#ifdef DEBUG_INT256
  printf("Output: %i\n", ctz);
#endif
  return ctz;
}

#ifdef $XCC_h
/*
 * Count Leading Signs (CLS(0) = CLS(-1) = width - 1).
 */
unsigned
Int256_cls(Int256_ this, unsigned width);
#endif//$XCC_h

unsigned
Int256_cls(Int256_ this, unsigned width)
{
  assert(width <= 256);
  Int256_ temp = Int256_shl_(this, 256 - width);
  if (temp.dwords[3] >> 63) {
    temp = Int256_not(temp);
  }
  temp = Int256_shr_(temp, 256 - width);
  return Int256_clz(temp, width) - 1;
}

#ifdef $XCC_h
/*
 * Count Bit Set.
 * Return the number of bits set to one in a word.
 */
static inline unsigned
Int256_cbs(Int256_ this, unsigned width)
{
  assert(width <= 256);
  if (width > 128) {
     return Int128_cbs(this.lower, 128) + Int128_cbs(this.upper, width - 128);
  } else {
     return Int128_cbs(this.lower, width);
  }
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Swap adjacent bits.
 */
static inline Int256_
Int256_swap(Int256_ this, unsigned slice)
{
  Int256_ output;
  output.lower = Int128_swap(this.lower, slice);
  output.upper = Int128_swap(this.upper, slice);
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Splat adjacent bits.
 */
Int256_
Int256_splat(Int256_ this, unsigned slice);
#endif//$XCC_h

Int256_
Int256_splat(Int256_ this, unsigned slice)
{
  Int256_ output;
  switch(slice) {
  int bits, byte, i;
  case 1:
    bits = this.bytes[0] & 0x1;
    bits = bits | (bits<<1) | (bits<<2) | (bits<<3);
    byte = bits | (bits<<4);
    memset(&output, byte, sizeof(output));
    break;
  case 2:
    bits = this.bytes[0] & 0x3;
    bits = bits | (bits<<2);
    byte = bits | (bits<<4);
    memset(&output, byte, sizeof(output));
    break;
  case 4:
    bits = this.bytes[0] & 0xF;
    byte = bits | (bits<<4);
    memset(&output, byte, sizeof(output));
    break;
  case 8:
    byte = this.bytes[0] & 0xFF;
    memset(&output, byte, sizeof(output));
    break;
  case 16:
    for (i = 0; i < sizeof(output)/sizeof(output.hwords[0]); i++)
      output.hwords[i] = this.hwords[0];
    break;
  case 32:
    for (i = 0; i < sizeof(output)/sizeof(output.words[0]); i++)
      output.words[i] = this.words[0];
    break;
  case 64:
    for (i = 0; i < sizeof(output)/sizeof(output.dwords[0]); i++)
      output.dwords[i] = this.dwords[0];
    break;
  default:
    output = this;
    assert(0);
  }
  return output;
}

#ifdef $XCC_h
/*
 * Zero Extension.
 */
static inline Int256_
Int256_zx(Int256_ this, unsigned width)
{
  Int256_ output;
  assert(width <= 256);
  if (width > 128) {
    output.lower = this.lower;
    output.upper = Int128_zx(this.upper, width - 128);
  } else {
    output.lower = Int128_zx(this.lower, width);
    output.upper = Int128_fromInt32(0);
  }
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Sign Extension.
 */
static inline Int256_
Int256_sx(Int256_ this, unsigned width)
{
  Int256_ output;
  assert(width <= 256);
  if (width > 128) {
    output.lower = this.lower;
    output.upper = Int128_sx(this.upper, width - 128);
  } else {
    output.lower = Int128_sx(this.lower, width);
    int32_t sign = -((output.lower.dwords[1] >> 63) & 1);
    output.upper = Int128_fromInt32(sign);
  }
  return output;
}
#endif//$XCC_h


////////////////////////////////////////////////////////////////////////////////
// Conversion operations
////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Convert from Boolean.
 */
static inline Int256_
Int256_fromBool(bool input)
{
  Int256_ output;
  output.lower = Int128_fromBool(input);
  output.upper = Int128_fromUInt64(0LL);
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert to Boolean
 */
static inline bool
Int256_toBool(Int256_ this)
{
  return Int128_toBool(this.lower) |
         Int128_toBool(this.upper);
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert from Int32.
 */
static inline Int256_
Int256_fromInt32(int32_t i)
{
  Int256_ output;
  output.lower = Int128_fromInt32(i);
  int32_t sign = -((i >> 31) & 1);
  output.upper = Int128_fromInt32(sign);
  return output;
}
#endif // $XCC_h

#ifdef $XCC_h
/*
 * Convert to Int32.
 */
static inline int32_t
Int256_toInt32(Int256_ this)
{
  return Int128_toInt32(this.lower);
}
#endif // $XCC_h

#ifdef $XCC_h
/*
 * Convert from UInt32.
 */
static inline Int256_
Int256_fromUInt32(int32_t i)
{
  Int256_ output;
  output.lower = Int128_fromUInt32(i);
  output.upper = Int128_fromUInt32(0);
  return output;
}
#endif // $XCC_h

#ifdef $XCC_h
/*
 * Convert to UInt32.
 */
static inline uint32_t
Int256_toUInt32(Int256_ this)
{
  return Int128_toUInt32(this.lower);
}
#endif // $XCC_h

#ifdef $XCC_h
/*
 * Convert from Int64.
 */
static inline Int256_
Int256_fromInt64(int64_t i)
{
  Int256_ output;
  output.lower = Int128_fromInt64(i);
  int32_t sign = -((i >> 63) & 1);
  output.upper = Int128_fromInt64(sign);
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert to Int64.
 */
static inline int64_t
Int256_toInt64(Int256_ this)
{
  return Int128_toInt64(this.lower);
}
#endif // $XCC_h

#ifdef $XCC_h
/*
 * Convert from UInt64.
 */
static inline Int256_
Int256_fromUInt64(uint64_t i)
{
  Int256_ output;
  output.lower = Int128_fromUInt64(i);
  output.upper = Int128_fromUInt64(0LL);
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert to UInt64.
 */
static inline uint64_t
Int256_toUInt64(Int256_ this)
{
  return Int128_toUInt64(this.lower);
}
#endif // $XCC_h

#ifdef $XCC_h
/*
 * Convert from Int128.
 */
static inline Int256_
Int256_fromInt128(Int128_ this)
{
  Int256_ output;
  uint64_t ext = (int64_t)this.dwords[1] >> 63;
  output.lower = this;
  output.dwords[2] = ext;
  output.dwords[3] = ext;
  return output;
}
#endif//$XCC_h


////////////////////////////////////////////////////////////////////////////////
// Arithmetic operations
////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Add.
 */
Int256_
Int256_add(Int256_ this, Int256_ that);
#endif//$XCC_h

Int256_
Int256_add(Int256_ this, Int256_ that)
{
#ifdef DEBUG_INT256
  printf("Input add:\n");
  Int256_print(this);
  Int256_print(that);
#endif
  int i = 0;
  unsigned carry = 0;
  do {
    uint64_t result = this.dwords[i] + that.dwords[i] + carry;
    // There is an overflow iff the result is strictly lower than one of its the operands.
    // If there is one equality and there was a carry, the other operand must be 0xFFFFFFFFFFFFFFFF.
    if (   (result < this.dwords[i])
        || (result < that.dwords[i])
        || (((result == this.dwords[i]) || (result == that.dwords[i])) && carry)) {
      carry = 1;
    } else {
      carry = 0;
    }
    this.dwords[i] = result;
    ++i;
  } while (i < 4);
#ifdef DEBUG_INT256
  printf("Output:\n");
  Int256_print(this); 
#endif
  return this;
}

#ifdef $XCC_h
/*
 * Negate.
 */
static inline Int256_
Int256_neg(Int256_ this)
{
  return Int256_add(Int256_not(this),
                    Int256_fromUInt64(1));
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Return the absolute value of the input
 */
Int256_
Int256_abs(Int256_ this);
#endif//$XCC_h

Int256_
Int256_abs(Int256_ this)
{
    Int256_ output;
#ifdef DEBUG_INT256
    printf("Input abs:\n");
    Int256_print(this);
#endif
    if (this.dwords[3] >> 63) {
        output = Int256_neg(this);
#ifdef DEBUG_INT256
        printf("Output:\n");
        Int256_print(output);
#endif
        return output;
    }
    else {
#ifdef DEBUG_INT256
        printf("Output:\n");
        Int256_print(this);
#endif
        return this;
    }
}

#ifdef $XCC_h
/*
 * Subtract.
 */
static inline Int256_
Int256_sub(Int256_ this, Int256_ that)
{
  return Int256_add(this, Int256_neg(that));
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Multiply Unsigned 128x128.
 */
Int256_
Int256_mulu128(Int128_ this, Int128_ that);
#endif//$XCC_h

Int256_
Int256_mulu128(Int128_ this, Int128_ that)
{
    Int256_ output;
    // We can't multiply integer bigger than 32 bits since result must
    // fit into the biggest type which is currently 64 bits, so we cut
    // inputs into 32 bits blocks.
    uint64_t this_0 = this.dwords[0] & 0x00000000FFFFFFFFLLU;
    uint64_t this_1 = this.dwords[0] >> 32;
    uint64_t this_2 = this.dwords[1] & 0x00000000FFFFFFFFLLU;
    uint64_t this_3 = this.dwords[1] >> 32;
    //
    uint64_t that_0 = that.dwords[0] & 0x00000000FFFFFFFFLLU;
    uint64_t that_1 = that.dwords[0] >> 32;
    uint64_t that_2 = that.dwords[1] & 0x00000000FFFFFFFFLLU;
    uint64_t that_3 = that.dwords[1] >> 32;

    uint64_t t00    = this_0 * that_0;
    uint64_t t01    = this_0 * that_1;
    uint64_t t02    = this_0 * that_2;
    uint64_t t03    = this_0 * that_3;

    uint64_t t10    = this_1 * that_0;
    uint64_t t11    = this_1 * that_1;
    uint64_t t12    = this_1 * that_2;
    uint64_t t13    = this_1 * that_3;

    uint64_t t20    = this_2 * that_0;
    uint64_t t21    = this_2 * that_1;
    uint64_t t22    = this_2 * that_2;
    uint64_t t23    = this_2 * that_3;

    uint64_t t30    = this_3 * that_0;
    uint64_t t31    = this_3 * that_1;
    uint64_t t32    = this_3 * that_2;
    uint64_t t33    = this_3 * that_3;

    // We compute [0-32[ bits.
    uint64_t product = t00;
    // What we need to propagate for the next 32 bits.
    uint64_t toPropagate = product >> 32;
#ifdef DEBUG_INT128
    printf("Input mulu:\n");
    Int128_print(this);
    Int128_print(that);
#endif
    output.lower.dwords[0] = product & 0x00000000FFFFFFFFLLU;
    // We compute [32-64[ bits = t01 + t10 + toPropagate
    product = t01 + toPropagate;
    toPropagate = product >> 32;
    product = (product & 0xffffffff) + t10;
    toPropagate = toPropagate + (product >> 32);

    output.lower.dwords[0] |= product << 32;
    // What we need to propagate for the next 32 bits.
    // We compute [64-96[ bits = t02 + t11 + t20 + toPropagate
    product = t02 + toPropagate;
    toPropagate = product >> 32;
    product = (product & 0xffffffff) + t11;
    toPropagate = toPropagate + (product >> 32);
    product = (product & 0xffffffff) + t20;
    toPropagate = toPropagate + (product >> 32);

    output.lower.dwords[1] = product & 0x00000000FFFFFFFFLLU;
    // What we need to propagate for the next 32 bits.
    // We compute [96-128[ bits = t03 + t12 + t21 + t30 + toPropagate
    product = t03 + toPropagate;
    toPropagate = product >> 32;
    product = (product & 0xffffffff) + t12;
    toPropagate = toPropagate + (product >> 32);
    product = (product & 0xffffffff) + t21;
    toPropagate = toPropagate + (product >> 32);
    product = (product & 0xffffffff) + t30;
    toPropagate = toPropagate + (product >> 32);

    output.lower.dwords[1] |= product << 32;
    // What we need to propagate for the next 32 bits.
    // We compute [128-160[ bits = t13 + t22 + t31 + toPropagate
    product = t13 + toPropagate;
    toPropagate = product >> 32;
    product = (product & 0xffffffff) + t22;
    toPropagate = toPropagate + (product >> 32);
    product = (product & 0xffffffff) + t31;
    toPropagate = toPropagate + (product >> 32);

    output.upper.dwords[0] = product & 0x00000000FFFFFFFFLLU;
    // What we need to propagate for the next 32 bits.
    // We compute [160-192[ bits = t23 + t32 + toPropagate
    product = t23 + toPropagate;
    toPropagate = product >> 32;
    product = (product & 0xffffffff) + t32;
    toPropagate = toPropagate + (product >> 32);

    output.upper.dwords[0] |= product << 32;
    // What we need to propagate for the next 32 bits.
    // We compute [192-256[ bits.
    product = t33 + toPropagate;
    output.upper.dwords[1] = product;
#ifdef DEBUG_INT128
    printf("Output:\n");
    Int256_print(output); 
#endif
    return output;
}

#ifdef $XCC_h
/*
 * Multiply Signed 128x128.
 */
Int256_
Int256_mul128(Int128_ this, Int128_ that);
#endif//$XCC_h

Int256_
Int256_mul128(Int128_ this, Int128_ that)
{
    // We retrieve the sign
    unsigned thisFirstBit = this.dwords[1] >> 63;
    unsigned thatFirstBit = that.dwords[1] >> 63;
    // Sign of the product
    int sign = (thisFirstBit != thatFirstBit)?-1:1;
    // We convert inputs to positive values if they are negative.
    Int256_ output;
#ifdef DEBUG_INT128
    printf("Input mul:\n");
    Int128_print(this);
    Int128_print(that);
#endif
    if (thisFirstBit)
        this = Int128_neg(this);
    if (thatFirstBit)
        that = Int128_neg(that);
    if (sign == -1) {
        output = Int256_neg(Int256_mulu128(this, that));
    }
    else {
        output = Int256_mulu128(this, that);
    }
#ifdef DEBUG_INT128
    printf("Output:\n");
    Int256_print(output); 
#endif
    return output;
}

#ifdef $XCC_h
/*
 * Multiply.
 */
Int256_
Int256_mul(Int256_ this, Int256_ that);
#endif//$XCC_h

Int256_
Int256_mul(Int256_ this, Int256_ that)
{
  uint64_t this_dword_0 = this.dwords[0];
  uint64_t that_dword_0 = that.dwords[0];
  if ((that_dword_0 & (that_dword_0 - 1)) == 0) {
    if (!that.dwords[1] && !that.dwords[2] && !that.dwords[3]) {
      if (that_dword_0) {
        return Int256_shl_(this, __builtin_ctzll(that_dword_0));
      } else {
        return Int256_zero;
      }
    }
  }
  if ((this_dword_0 & (this_dword_0 - 1)) == 0) {
    if (!this.dwords[1] && !this.dwords[2] && !this.dwords[3]) {
      if (this_dword_0) {
        return Int256_shl_(that, __builtin_ctzll(this_dword_0));
      } else {
        return Int256_zero;
      }
    }
  }
  return Int256_mul128(this.lower, that.lower);
}

#ifdef $XCC_h
/*
 * Do a right shift of 'shift' bits, filling the first bits set to
 * 'bit' (used for signed or unsigned right shift)
 */
Int256_
Int256_shrx_(Int256_ this, unsigned shift, unsigned fill);
#endif//$XCC_h

Int256_
Int256_shrx_(Int256_ this, unsigned shift, unsigned fill)
{
  Int256_ output;
  uint64_t previous = 0LL - (fill != 0);
  int i = 3, j = 3;
  assert(shift < 256);
  while (shift >= 64) {
    output.dwords[i] = previous;
    shift -= 64;
    i--;
  }
  while (i >= 0) {
    output.dwords[i] = this.dwords[j] >> shift;
    if (shift) {
      output.dwords[i] |= previous << (64 - shift);
      previous = this.dwords[j];
    }
    i--, j--;
  }
  return output;
}

#ifdef $XCC_h
/*
 * Shift Right Signed.
 */
static inline Int256_
Int256_shr_(Int256_ this, unsigned shift)
{
  return Int256_shrx_(this, shift, this.dwords[3] >> 63);
}
static inline Int256_
Int256_shr(Int256_ this, Int256_ that)
{
  return Int256_shr_(this, Int256_toUInt64(that));
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Shift Right Unsigned.
 */
static inline Int256_
Int256_shru_(Int256_ this, unsigned shift)
{
  return Int256_shrx_(this, shift, 0);
}
static inline Int256_
Int256_shru(Int256_ this, Int256_ that)
{
  return Int256_shru_(this, Int256_toUInt64(that));
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Shift Left.
 */
Int256_
Int256_shl_(Int256_ this, unsigned shift);
static inline Int256_
Int256_shl(Int256_ this, Int256_ that)
{
  return Int256_shl_(this, Int256_toUInt64(that));
}
#endif//$XCC_h

Int256_
Int256_shl_(Int256_ this, unsigned shift)
{
  Int256_ output;
  uint64_t previous = 0LL;
  int i = 0, j = 0;
  assert(shift < 256);
  while (shift >= 64) {
    output.dwords[i] = previous;
    shift -= 64;
    i++;
  }
  while (i < 4) {
    output.dwords[i] = this.dwords[j] << shift;
    if (shift) {
      output.dwords[i] |= previous >> (64 - shift);
      previous = this.dwords[j];
    }
    i++, j++;
  }
  return output;
}


////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Rotate Right
 * width: we work modulo bit-width
 * ror(4, 2, 11100110) = 00001010
 */
Int256_
Int256_ror(unsigned width, Int256_ this, Int256_ rotate);
#endif//$XCC_h

Int256_
Int256_ror(unsigned width, Int256_ this, Int256_ rotate)
{
  assert(width <= 256);
  // If we want to rotate more than width bits, we take the rest modulo width.
  unsigned shift = Int256_toUInt64(rotate) % width;
  Int256_ rshift = Int256_fromUInt32(shift);
  Int256_ lshift = Int256_fromUInt32(width - shift);
  Int256_ output = 
    Int256_or(
      Int256_shru(Int256_zx(this, width), rshift),
      Int256_zx(Int256_shl(this, lshift), width));
#ifdef DEBUG_INT128
  printf("Input ror.%d:\n", width);
  Int256_print(rotate);
  Int256_print(this);
#endif
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int256_print(output); 
#endif
  return output;
}

#ifdef $XCC_h
/*
 * Rotate on Left
 * width: we work modulo bit-width
 * rol(4, 2, 11100110) = 00001001
 */
Int256_
Int256_rol(unsigned width, Int256_ this, Int256_ rotate);
#endif//$XCC_h

Int256_
Int256_rol(unsigned width, Int256_ this, Int256_ rotate)
{
  assert(width <= 256);
  // If we want to rotate more than width bits, we take the rest modulo width.
  unsigned shift = Int256_toUInt64(rotate) % width;
  Int256_ rshift = Int256_fromUInt32(width - shift);
  Int256_ lshift = Int256_fromUInt32(shift);
  Int256_ output = 
    Int256_or(
      Int256_shru(Int256_zx(this, width), rshift),
      Int256_zx(Int256_shl(this, lshift), width));
#ifdef DEBUG_INT128
  printf("Input rol%d:\n", width);
  Int256_print(rotate);
  Int256_print(this);
#endif
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int256_print(output); 
#endif
  return output;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  ////////////////////////////
  // Test Int256_print 
  ////////////////////////////
  int i;
  // first flit, second flit, thrid flit, forth flit
  uint64_t print_values[][4] = { 
        {0xFFFFFFFF00000000LLU, 0xFFFFFFFF10000004LLU, 0, 7},
        {0x0123456789ABCDEFLLU, 0x0123456789ABCDEFLLU, 0x0123456789ABCDEFLLU, 0x0123456789ABCDEFLLU},
        {0x0123456789ABCDEFLLU, 0x0123456789ABCDEFLLU, 0xAAAAAAAAAAAAAAAALLU, 0xAAAAAAAAAAAAAAAALLU},
        {0xAAAAAAAAAAAAAAAALLU, 0xAAAAAAAAAAAAAAAALLU, 0, 0}
    };
  for (i = 0; i < sizeof(print_values) / (8*4); ++i) {
    Int256_ input;
    Int256_Ctor(&input, print_values[i][3], print_values[i][2], print_values[i][1], print_values[i][0]);
    Int256_print(input);
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test shl 
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][12] = {
    {0, 0, 0x8888888888888888LLU, 0x8888888888888888LLU, 0, 0, 0, 49, 0, 0x0001111111111111, 0x1111111111111111LLU, 0x1110000000000000LLU},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int256_ input_1, input_2;
    Int256_ correct;
    Int256_ output;
    Int256_Ctor(&input_1, values[i][3], values[i][2], values[i][1], values[i][0]);
    Int256_Ctor(&input_2, values[i][7], values[i][6], values[i][5], values[i][4]);
    output = Int256_shl(input_1, input_2);
    Int256_Ctor(&correct, values[i][11], values[i][10], values[i][9], values[i][8]);
    printf("SHL test %d\n", i+1);
    if (!Int256_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int256_print(input_1);
      Int256_print(input_2);
      printf("Result:\n");
      Int256_print(output);
      printf("Expected:\n");
      Int256_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Int256 module.
 */
#define Int256_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Int256 module.
 */
#define Int256_FINI()
#endif//$XCC__h

#if XCC__C
static void
Int256_TEST(void)
{
#include "BSL/Int256_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(Int256_TEST);
  return 0;
}
#endif

