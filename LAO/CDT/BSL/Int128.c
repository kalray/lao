#ifdef $XCC_h
/*
 * !!!!	Int128.xcc
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
 * @brief Arithmetic support for 128-bit integers.
 */
#endif//$XCC_h


#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/Int128.h"
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
#define DEBUG_INT128 1
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
union Int128_ {
#ifdef TYPE_INT128
  uint128_t qword;
#endif//TYPE_INT128
  uint64_t dwords[2];
  uint32_t words[4];
  uint16_t hwords[8];
  uint8_t bytes[16];
};
typedef union Int128_ Int128_, *Int128;
#endif//$XCC_h

#ifdef $XCC_h
#define Int128_zero	Int128_fromUInt64(0LL)
#define Int128_dwords_0(this)	(this).dwords[0]
#define Int128_dwords_1(this)	(this).dwords[1]
#endif//$XCC_h

#ifdef $XCC_h
static inline void
Int128_Ctor(Int128_ *output, uint64_t dword_0, uint64_t dword_1)
{
  output->dwords[0] = dword_0;
  output->dwords[1] = dword_1;
}
#endif//$XCC_h

#ifdef $XCC_h
static inline Int128_
Int128_make(uint64_t dword_0, uint64_t dword_1)
{
  Int128_ output;
  output.dwords[0] = dword_0;
  output.dwords[1] = dword_1;
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
void
Int128_print(Int128_ this);
#endif//$XCC_h

#if !defined(_NTRACE) || defined(XCC__C)
void
Int128_print(Int128_ this)
{
  for (int j = 1; j >= 0; --j) {
    printf("%016" PRIx64, this.dwords[j]);
    printf("|");
  }
  printf("\n");
}
#endif//_NTRACE

#ifdef $XCC_h
static inline int
Int128_equals(Int128_ this, Int128_ that)
{
#ifdef TYPE_INT128
  return this.qword == that.qword;
#else//TYPE_INT128
  return (this.dwords[0] == that.dwords[0]) &&
         (this.dwords[1] == that.dwords[1]);
#endif//TYPE_INT128
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Signed saturate.
 */
Int128_
Int128_sat(Int128_ this, unsigned width);
#endif//$XCC_h

Int128_
Int128_sat(Int128_ this, unsigned width)
{
  Int128_ output = this;
  if (width > 128);
  else if (width > 64) {
    // width in [64+1..2*64]
    int64_t smin = -(1LL<<(width-64-1));
    int64_t smax = (1LL<<(width-64-1)) - 1;
    if ((int64_t)this.dwords[1] < smin) {
      output.dwords[0] = 0ULL;
      output.dwords[1] = smin;
    } else if ((int64_t)this.dwords[1] > smax) {
      output.dwords[0] = -1ULL;
      output.dwords[1] = smax;
    }
  } else if (width > 0) {
    // width in [1..64]
    int64_t smin = -(1LL<<(width-1));
    int64_t smax = (1LL<<(width-1)) - 1;
    if ((int64_t)this.dwords[1] < 0) {
      if (   (int64_t)this.dwords[1] != -1LL
          || (uint64_t)this.dwords[0] < (uint64_t)smin) {
        output.dwords[0] = smin;
        output.dwords[1] = -1ULL;
      }
    } else {
      if (   (int64_t)this.dwords[1] != 0ULL
          || (uint64_t)this.dwords[0] > (uint64_t)smax) {
        output.dwords[0] = smax;
        output.dwords[1] = 0ULL;
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
Int128_
Int128_satu(Int128_ this, unsigned width);
#endif//$XCC_h

Int128_
Int128_satu(Int128_ this, unsigned width)
{
  Int128_ output = this;
  if ((uint64_t)this.dwords[1] >> 64-1) {
    // Negative, saturate to zero.
    output.dwords[0] = 0ULL;
    output.dwords[1] = 0ULL;
    return output;
  }
  if (width >= 128);
  else if (width >= 64) {
    // width in [64..2*64-1]
    uint64_t umax = (1ULL<<(width-64)) - 1;
    if ((uint64_t)this.dwords[1] > umax) {
      output.dwords[0] = -1ULL;
      output.dwords[1] = umax;
    }
  } else if (width > 0) {
    // width in [1..64-1]
    uint64_t umax = (1ULL<<width) - 1;
    if (   (uint64_t)this.dwords[1] != 0ULL
        || (uint64_t)this.dwords[0] > umax) {
      output.dwords[0] = umax;
      output.dwords[1] = 0ULL;
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
#ifdef TYPE_INT128
static inline int
Int128_cmpu(Int128_ this, Int128_ that)
{
  return ((uint128_t)this.qword > (uint128_t)that.qword) -
         ((uint128_t)this.qword < (uint128_t)that.qword);
}
#else//TYPE_INT128
int
Int128_cmpu(Int128_ this, Int128_ that);
#endif//TYPE_INT128
#endif//$XCC_h

#ifndef TYPE_INT128
int
Int128_cmpu(Int128_ this, Int128_ that)
{
  int result = 0;
#ifdef DEBUG_INT128
  printf("Input cmpu:\n");
  Int128_print(this);
  Int128_print(that);
#endif
  for (int i = 1; (i >= 0) && (result == 0); --i) {
    if (this.dwords[i] > that.dwords[i]) {
      result = 1;
    }
    if (this.dwords[i] < that.dwords[i]) {
      result = -1;
    }
  }
#ifdef DEBUG_INT128
  printf("Output: %i\n", result);
#endif
  return result;
}
#endif//TYPE_INT128

#ifdef $XCC_h
/*
 * Signed comparison
 * if this == that -> return 0
 * if this > that -> return 1
 * if this < that -> return -1
 */
#ifdef TYPE_INT128
static inline int
Int128_cmp(Int128_ this, Int128_ that)
{
  return ((int128_t)this.qword > (int128_t)that.qword) - 
         ((int128_t)this.qword < (int128_t)that.qword);
}
#else//TYPE_INT128
int
Int128_cmp(Int128_ this, Int128_ that);
#endif//TYPE_INT128
#endif//$XCC_h

#ifndef TYPE_INT128
int
Int128_cmp(Int128_ this, Int128_ that)
{
  int output = 0;
#ifdef DEBUG_INT128
  printf("Input cmp:\n");
  Int128_print(this);
  Int128_print(that);
#endif
  // We retrieve the sign
  unsigned thisFirstBit = this.dwords[1] >> 63;
  unsigned thatFirstBit = that.dwords[1] >> 63;
  if (thisFirstBit != thatFirstBit) {
    // Here this is negative and that positive
    if (thisFirstBit) {
      output = -1;
    }
    // Here this is positive and that negative
    else {
      output = 1;
    }
  }
  else {
    output = Int128_cmpu(this, that);
  }
#ifdef DEBUG_INT128
  printf("Output: %i\n", output);
#endif
  return output;
}
#endif//TYPE_INT128

#ifdef $XCC_h
/*
 * Bitwise OR.
 */
#ifdef TYPE_INT128
static inline Int128_
Int128_or(Int128_ this, Int128_ that)
{
  this.qword |= that.qword;
  return this;
}
#else//TYPE_INT128
Int128_
Int128_or(Int128_ this, Int128_ that);
#endif//TYPE_INT128
#endif//$XCC_h

#ifndef TYPE_INT128
Int128_
Int128_or(Int128_ this, Int128_ that)
{
#ifdef DEBUG_INT128
  printf("Input or:\n");
  Int128_print(this);
  Int128_print(that);
#endif
  for (int i = 0; i < 2; ++i) {
    this.dwords[i] |= that.dwords[i];
  }
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(this); 
#endif
  return this;
}
#endif//TYPE_INT128

#ifdef $XCC_h
/*
 * Bitwise XOR.
 */
#ifdef TYPE_INT128
static inline Int128_
Int128_xor(Int128_ this, Int128_ that)
{
  this.qword ^= that.qword;
  return this;
}
#else//TYPE_INT128
Int128_
Int128_xor(Int128_ this, Int128_ that);
#endif//TYPE_INT128
#endif//$XCC_h

#ifndef TYPE_INT128
Int128_
Int128_xor(Int128_ this, Int128_ that)
{
#ifdef DEBUG_INT128 
  printf("Input xor:\n");
  Int128_print(this);
  Int128_print(that);
#endif
  for (int i = 0; i < 2; ++i) {
    this.dwords[i] ^= that.dwords[i];
  }
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(this); 
#endif
  return this;
}
#endif//TYPE_INT128

#ifdef $XCC_h
/*
 * Bitwise AND.
 */
#ifdef TYPE_INT128
static inline Int128_
Int128_and(Int128_ this, Int128_ that)
{
  this.qword &= that.qword;
  return this;
}
#else//TYPE_INT128
Int128_
Int128_and(Int128_ this, Int128_ that);
#endif//TYPE_INT128
#endif//$XCC_h

#ifndef TYPE_INT128
Int128_
Int128_and(Int128_ this, Int128_ that)
{
#ifdef DEBUG_INT128
  printf("Input add:\n");
  Int128_print(this);
  Int128_print(that);
#endif
  for (int i = 0; i < 2; ++i) {
    this.dwords[i] &= that.dwords[i];
  }
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(this); 
#endif
  return this;
}
#endif//TYPE_INT128

#ifdef $XCC_h
/*
 * Bitwise NOT.
 */
#ifdef TYPE_INT128
static inline Int128_
Int128_not(Int128_ this)
{
  this.qword = ~this.qword;
  return this;
}
#else//TYPE_INT128
Int128_
Int128_not(Int128_ this);
#endif//TYPE_INT128
#endif//$XCC_h

#ifndef TYPE_INT128
Int128_
Int128_not(Int128_ this)
{
#ifdef DEBUG_INT128
  printf("Input not:\n");
  Int128_print(this);
#endif
  for (int i = 0; i < 2; ++i) {
    this.dwords[i] = ~this.dwords[i];
  }
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(this); 
#endif
  return this;
}
#endif//TYPE_INT128

#ifdef $XCC_h
/*
 * Count Leading Zeros (CLZ(0) = width).
 */
unsigned
Int128_clz(Int128_ this, unsigned width);
#endif//$XCC_h

unsigned
Int128_clz(Int128_ this, unsigned width)
{
  assert(width <= 128);
  unsigned clz = width;
#ifdef DEBUG_INT128
  printf("Input clz.%d:\n", width);
  Int128_print(this);
#endif
  Int128_ temp = Int128_shl_(this, 128 - width);
  for (int i = 1; i >= 0; --i) {
    if (temp.dwords[i]) {
      clz = 64*(1-i) + __builtin_clzll(temp.dwords[i]);
      break;
    }
  }
#ifdef DEBUG_INT128
  printf("Output: %i\n", clz);
#endif
  return clz;
}

#ifdef $XCC_h
/*
 * Count Trailing Zeros (CTZ(0) = width).
 */
unsigned
Int128_ctz(Int128_ this, unsigned width);
#endif//$XCC_h

unsigned
Int128_ctz(Int128_ this, unsigned width)
{
  assert(width <= 128);
  unsigned ctz = width;
#ifdef DEBUG_INT128
  printf("Input ctz.%d:\n", width);
  Int128_print(this);
#endif
  for (int i = 0; i <= 1; ++i) {
    if (this.dwords[i]) {
      ctz = 64*i + __builtin_ctzll(this.dwords[i]);
      if (ctz > width) ctz = width;
      break;
    }
  }
#ifdef DEBUG_INT128
  printf("Output: %i\n", ctz);
#endif
  return ctz;
}

#ifdef $XCC_h
/*
 * Count Leading Signs (CLS(0) = CLS(-1) = width - 1).
 */
unsigned
Int128_cls(Int128_ this, unsigned width);
#endif//$XCC_h

unsigned
Int128_cls(Int128_ this, unsigned width)
{
  assert(width <= 128);
  Int128_ temp = Int128_shl_(this, 128 - width);
  if (temp.dwords[1] >> 63) {
    temp = Int128_not(temp);
  }
  temp = Int128_shr_(temp, 128 - width);
  return Int128_clz(temp, width) - 1;
}

#ifdef $XCC_h
/*
 * Count Bit Set.
 * Return the number of bits set to one in a word.
 */
static inline unsigned
Int128_cbs(Int128_ this, unsigned width)
{
  assert(width <= 128);
  if (width > 64) {
    return __builtin_popcountll(this.dwords[0]) +
           __builtin_popcountll(this.dwords[1] << (128 - width));
  } else {
    return __builtin_popcountll(this.dwords[0] << (64 - width));
  }
}
#endif//$XCC_h

#ifdef $XCC_h
Int128_
Int128_swap_1(Int128_ this);
#endif//$XCC_h

Int128_
Int128_swap_1(Int128_ this)
{
  uint64_t mask = 0x5555555555555555LLU;
  for (int i = 0; i < 2; ++i) {
    this.dwords[i] = ((this.dwords[i] & mask) << 1) |
                     ((this.dwords[i] & ~mask) >> 1);
  }
  return this;
}

#ifdef $XCC_h
Int128_
Int128_swap_2(Int128_ this);
#endif//$XCC_h

Int128_
Int128_swap_2(Int128_ this)
{
  uint64_t mask = 0x3333333333333333LLU;
  for (int i = 0; i < 2; ++i) {
    this.dwords[i] = ((this.dwords[i] & mask) << 2) |
                     ((this.dwords[i] & ~mask) >> 2);
  }
  return this;
}

#ifdef $XCC_h
Int128_
Int128_swap_4(Int128_ this);
#endif//$XCC_h

Int128_
Int128_swap_4(Int128_ this)
{
  uint64_t mask = 0x0f0f0f0f0f0f0f0f;
  for (int i = 0; i < 2; ++i) {
    this.dwords[i] = ((this.dwords[i] & mask) << 4) |
                     ((this.dwords[i] & ~mask) >> 4);
  }
  return this;
}

#ifdef $XCC_h
Int128_
Int128_swap_8(Int128_ this);
#endif//$XCC_h

Int128_
Int128_swap_8(Int128_ this)
{
  uint64_t mask = 0x00ff00ff00ff00ff;
  for (int i = 0; i < 2; ++i) {
    this.dwords[i] = ((this.dwords[i] & mask) << 8) |
                     ((this.dwords[i] & ~mask) >> 8);
  }
  return this;
}

#ifdef $XCC_h
Int128_
Int128_swap_16(Int128_ this);
#endif//$XCC_h

Int128_
Int128_swap_16(Int128_ this)
{
  uint64_t mask = 0x0000ffff0000ffff;
  for (int i = 0; i < 2; ++i) {
    this.dwords[i] = ((this.dwords[i] & mask) << 16) |
                     ((this.dwords[i] & ~mask) >> 16);
  }
  return this;
}

#ifdef $XCC_h
Int128_
Int128_swap_32(Int128_ this);
#endif//$XCC_h

Int128_
Int128_swap_32(Int128_ this)
{
  uint64_t mask = 0x00000000ffffffff;
  for (int i = 0; i < 2; ++i) {
    this.dwords[i] = ((this.dwords[i] & mask) << 32) |
                     ((this.dwords[i] & ~mask) >> 32);
  }
  return this;
}

#ifdef $XCC_h
Int128_
Int128_swap_64(Int128_ this);
#endif//$XCC_h

Int128_
Int128_swap_64(Int128_ this)
{
  uint64_t temp = this.dwords[0];
  this.dwords[0] = this.dwords[1];
  this.dwords[1] = temp;
  return this;
}

#ifdef $XCC_h
/*
 * Swap adjacent bits.
 */
Int128_
Int128_swap(Int128_ this, unsigned slice);
#endif//$XCC_h

Int128_
Int128_swap(Int128_ this, unsigned slice)
{
  Int128_ output;
#ifdef DEBUG_INT128
  printf("Input swap:\n");
  Int128_print(this);
  printf("slice size: %u\n", slice);
#endif
  switch(slice) {
  case 1:
    output = Int128_swap_1(this);
    break;
  case 2:
    output = Int128_swap_2(this);
    break;
  case 4:
    output = Int128_swap_4(this);
    break;
  case 8:
    output = Int128_swap_8(this);
    break;
  case 16:
    output = Int128_swap_16(this);
    break;
  case 32:
    output = Int128_swap_32(this);
    break;
  case 64:
    output = Int128_swap_64(this);
    break;
  default:
    output = this;
    //assert(0);
  }
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(output); 
#endif
  return output;
}

#ifdef $XCC_h
/*
 * Zero Extension.
 */
static inline Int128_
Int128_zx(Int128_ this, unsigned width)
{
  assert(width <= 128);
#ifdef DEBUG_INT128
  printf("Input zx.%d:\n", width);
  Int128_print(this);
#endif
  if (width >= 128) return this;
#ifdef TYPE_INT128
    this.qword <<= (128 - width);
    this.qword >>= (128 - width);
#else//TYPE_INT128
  if (width >= 64) {
    width -= 64;
    this.dwords[1] &= ((1LLU << width ) - 1);
  } else {
    this.dwords[0] &= ((1LLU << width ) - 1);
    this.dwords[1] = 0;
  }
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(this); 
#endif
  return this;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Sign Extension.
 */
static inline Int128_
Int128_sx(Int128_ this, unsigned width)
{
  assert(width <= 128);
#ifdef DEBUG_INT128
  printf("Input sx.%d:\n", width);
  Int128_print(this);
#endif
  if (width >= 128) return this;
#ifdef TYPE_INT128
    int128_t qword = this.qword;
    qword <<= (128 - width);
    qword >>= (128 - width);
    this.qword = qword;
#else//TYPE_INT128
  if (width) {
    // index will be the index of the word in which we have to start
    // the sign extension. Below this index we just let the values
    // of this. Above this index we just have to set to 1 or 0,
    // depending on the sign.
    unsigned index = (width - 1) / 64;
    width = (width - 1) % 64;
    // Use arithmetic shift to extend sign.
    // (workaround for gcc-4.3.2)
    this.dwords[index] <<= 63 - width;
    this.dwords[index] = (int64_t)(this.dwords[index]) >> (63 - width);
    if (index == 0) {
      this.dwords[1] = (int64_t)this.dwords[0] >> 63;
    }
  }
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(this); 
#endif
  return this;
}
#endif//$XCC_h


////////////////////////////////////////////////////////////////////////////////
// Conversion operations
////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Convert from Boolean.
 */
static inline Int128_
Int128_fromBool(bool input)
{
  Int128_ output;
#ifdef DEBUG_INT128
  printf("Input fromBool: %d\n", input);
#endif
#ifdef TYPE_INT128
  output.qword = (input != 0);
#else//TYPE_INT128
  output.dwords[0] = (input != 0);
  output.dwords[1] = 0;
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(output); 
#endif
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert to Boolean
 */
static inline bool
Int128_toBool(Int128_ this)
{
#ifdef TYPE_INT128
  return this.qword != 0;
#else//TYPE_INT128
  return (this.dwords[0] | this.dwords[1]) != 0;
#endif//TYPE_INT128
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert from Int32.
 */
static inline Int128_
Int128_fromInt32(int32_t i)
{
  Int128_ output;
#ifdef DEBUG_INT128
  printf("Input fromInt32: 0x%" PRIx32 "\n", i);
#endif
#ifdef TYPE_INT128
  output.qword = (int32_t)i;
#else//TYPE_INT128
  output.dwords[0] = (int32_t)i;
  output.dwords[1] = (int64_t)i >> 63;
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(output); 
#endif
  return output;
}
#endif // $XCC_h

#ifdef $XCC_h
/*
 * Convert to Int32.
 */
static inline int32_t
Int128_toInt32(Int128_ this)
{
#ifdef TYPE_INT128
  int32_t output = this.qword;
#else//TYPE_INT128
  int32_t output = this.dwords[0];
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Input toInt32:\n");
  Int128_print(this);
#endif
#ifdef DEBUG_INT128
  printf("Output: %d\n", output);
#endif
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert from UInt32.
 */
static inline Int128_
Int128_fromUInt32(uint32_t i)
{
  Int128_ output;
#ifdef DEBUG_INT128
  printf("Input fromUInt32: 0x%" PRIx32 "\n", i);
#endif
#ifdef TYPE_INT128
  output.qword = (uint32_t)i;
#else//TYPE_INT128
  output.dwords[0] = (uint32_t)i;
  output.dwords[1] = 0;
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(output); 
#endif
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert to UInt32.
 */
static inline uint32_t
Int128_toUInt32(Int128_ this)
{
#ifdef TYPE_INT128
  uint32_t output = this.qword;
#else//TYPE_INT128
  uint32_t output = this.dwords[0];
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Input toUInt32:\n");
  Int128_print(this);
#endif
#ifdef DEBUG_INT128
  printf("Output: %x\n", output);
#endif
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert from Int64.
 */
static inline Int128_
Int128_fromInt64(int64_t i)
{
  Int128_ output;
#ifdef DEBUG_INT128
  printf("Input fromInt64: 0x%" PRIx64 "\n", i);
#endif
#ifdef TYPE_INT128
  output.qword = (int64_t)i;
#else//TYPE_INT128
  output.dwords[0] = (int64_t)i;
  output.dwords[1] = (int64_t)i >> 63;
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(output); 
#endif
  return output;
}
#endif // $XCC_h

#ifdef $XCC_h
/*
 * Convert to Int64.
 */
static inline int64_t
Int128_toInt64(Int128_ this)
{
#ifdef TYPE_INT128
  int64_t output = this.qword;
#else//TYPE_INT128
  int64_t output = this.dwords[0];
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Input toInt64:\n");
  Int128_print(this);
#endif
#ifdef DEBUG_INT128
  printf("Output: %" PRId64 "\n", output);
#endif
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert from UInt64.
 */
static inline Int128_
Int128_fromUInt64(uint64_t i)
{
  Int128_ output;
#ifdef DEBUG_INT128
  printf("Input fromUInt64: 0x%" PRIx64 "\n", i);
#endif
#ifdef TYPE_INT128
  output.qword = (uint64_t)i;
#else//TYPE_INT128
  output.dwords[0] = (uint64_t)i;
  output.dwords[1] = 0;
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(output); 
#endif
  return output;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Convert to UInt64.
 */
static inline uint64_t
Int128_toUInt64(Int128_ this)
{
#ifdef TYPE_INT128
  uint64_t output = this.qword;
#else//TYPE_INT128
  uint64_t output = this.dwords[0];
#endif//TYPE_INT128
#ifdef DEBUG_INT128
  printf("Input toUInt64:\n");
  Int128_print(this);
#endif
#ifdef DEBUG_INT128
  printf("Output: 0x%" PRIx64 "\n", output);
#endif
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
#ifdef TYPE_INT128
static inline Int128_
Int128_add(Int128_ this, Int128_ that)
{
  this.qword += that.qword;
  return this;
}
#else//TYPE_INT128
Int128_
Int128_add(Int128_ this, Int128_ that);
#endif//TYPE_INT128
#endif//$XCC_h

#ifndef TYPE_INT128
Int128_
Int128_add(Int128_ this, Int128_ that)
{
#ifdef DEBUG_INT128
  printf("Input add:\n");
  Int128_print(this);
  Int128_print(that);
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
    }
    else {
      carry = 0;
    }
    this.dwords[i] = result;
    ++i;
  } while (i < 2);
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(this); 
#endif
  return this;
}
#endif//TYPE_INT128

#ifdef $XCC_h
/*
 * Negate.
 */
#ifdef TYPE_INT128
static inline Int128_
Int128_neg(Int128_ this)
{
  this.qword = -this.qword;
  return this;
}
#else//TYPE_INT128
Int128_
Int128_neg(Int128_ this);
#endif//TYPE_INT128
#endif//$XCC_h

#ifndef TYPE_INT128
Int128_
Int128_neg(Int128_ this)
{
  Int128_ one = Int128_fromUInt64(1);
  Int128_ output = Int128_add(Int128_not(this), one);
#ifdef DEBUG_INT128
  printf("Input neg:\n");
  Int128_print(this);
#endif
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(output); 
#endif
  return output;
}
#endif//TYPE_INT128

#ifdef $XCC_h
/*
 * Return the absolute value of the input
 */
Int128_
Int128_abs(Int128_ this);
#endif//$XCC_h

Int128_
Int128_abs(Int128_ this)
{
    Int128_ output;
#ifdef DEBUG_INT128
    printf("Input abs:\n");
    Int128_print(this);
#endif
    if (this.dwords[1] >> 63) {
        output = Int128_neg(this);
#ifdef DEBUG_INT128
        printf("Output:\n");
        Int128_print(output);
#endif
        return output;
    } else {
#ifdef DEBUG_INT128
        printf("Output:\n");
        Int128_print(this);
#endif
        return this;
    }
}

#ifdef $XCC_h
/*
 * Subtract.
 */
#ifdef TYPE_INT128
static inline Int128_
Int128_sub(Int128_ this, Int128_ that)
{
  this.qword -= that.qword;
  return this;
}
#else//TYPE_INT128
Int128_
Int128_sub(Int128_ this, Int128_ that);
#endif//TYPE_INT128
#endif//$XCC_h

#ifndef TYPE_INT128
Int128_
Int128_sub(Int128_ this, Int128_ that)
{
  Int128_ output = Int128_add(this, Int128_neg(that));
#ifdef DEBUG_INT128
  printf("Input sub:\n");
  Int128_print(this);
  Int128_print(that);
#endif
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(output); 
#endif
  return output;
}
#endif//TYPE_INT128


#ifdef $XCC_h
/*
 *  Multiply signed 64 bits with signed 64 bits.
 */
#ifdef TYPE_INT128
static inline Int128_
Int128_mul64(Int128_ this, Int128_ that)
{
  Int128_ result;
  result.qword = (int128_t)(int64_t)this.dwords[0] *
                 (int128_t)(int64_t)that.dwords[0];
  return result;
}
#else//TYPE_INT128
Int128_
Int128_mul64(Int128_ this, Int128_ that);
#endif//TYPE_INT128
#endif//$XCC_h

#ifndef TYPE_INT128
Int128_
Int128_mul64(Int128_ this, Int128_ that) {
  Int128_ result = Int128_fromUInt64(0);
  int64_t temp;
  uint32_t thisL, thatL;
  int32_t sthisH, sthatH;
  //
  thisL= this.dwords[0];
  sthisH = this.dwords[0] >> 32;
  //
  thatL = that.dwords[0];
  sthatH = that.dwords[0] >> 32;
  //
  temp = ((uint64_t) thisL) * ((uint64_t) thatL);
  result.dwords[0] = temp;
  //
  temp = ((int64_t) sthisH) * ((uint64_t) thatL);
  result = Int128_add(result, Int128_shl_(Int128_fromInt64(temp), 32));
  //
  temp = ((int64_t) sthatH) * ((uint64_t) thisL);
  result = Int128_add(result, Int128_shl_(Int128_fromInt64(temp), 32));
  //
  temp = ((int64_t) sthatH) * ((int64_t) sthisH);
  result = Int128_add(result, Int128_shl_(Int128_fromInt64(temp), 64));
  return result;
}
#endif//TYPE_INT128

#ifdef $XCC_h
/*
 * Multiply.
 */
#ifdef TYPE_INT128
static inline Int128_
Int128_mul(Int128_ this, Int128_ that)
{
  this.qword *= that.qword;
  return this;
}
#else//TYPE_INT128
#define Int128_mul(this, that) Int256_mulu128(this, that).lower
#endif//TYPE_INT128
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Do a right shift of 'shift' bits, filling the first bits set to
 * 'bit' (used for signed or unsigned right shift)
 */
Int128_
Int128_shrx_(Int128_ this, unsigned shift, unsigned fill);
#endif//$XCC_h

Int128_
Int128_shrx_(Int128_ this, unsigned shift, unsigned fill)
{
  Int128_ output;
  uint64_t previous = 0ULL - (fill != 0);
  int i = 1, j = 1;
  assert(shift < 128);
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
#ifdef TYPE_INT128
static inline Int128_
Int128_shr_(Int128_ this, unsigned shift)
{
  Int128_ output;
  assert(shift < 128);
  output.qword = (int128_t)this.qword >> shift;
  return output;
}
#else//TYPE_INT128
static inline Int128_
Int128_shr_(Int128_ this, unsigned shift)
{
  return Int128_shrx_(this, shift, this.dwords[1] >> 63);
}
#endif//TYPE_INT128
static inline Int128_
Int128_shr(Int128_ this, Int128_ that)
{
  return Int128_shr_(this, Int128_toUInt64(that));
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Shift Right Unsigned.
 */
#ifdef TYPE_INT128
static inline Int128_
Int128_shru_(Int128_ this, unsigned shift)
{
  Int128_ output;
  assert(shift < 128);
  output.qword = (uint128_t)this.qword >> shift;
  return output;
}
#else//TYPE_INT128
static inline Int128_
Int128_shru_(Int128_ this, unsigned shift)
{
  return Int128_shrx_(this, shift, 0);
}
#endif//TYPE_INT128
static inline Int128_
Int128_shru(Int128_ this, Int128_ that)
{
  return Int128_shru_(this, Int128_toUInt64(that));
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * Shift Left.
 */
#ifdef TYPE_INT128
static inline Int128_
Int128_shl_(Int128_ this, unsigned shift)
{
  Int128_ output;
  assert(shift < 128);
  output.qword = this.qword << shift;
  return output;
}
#else//TYPE_INT128
Int128_
Int128_shl_(Int128_ this, unsigned shift);
#endif//TYPE_INT128
static inline Int128_
Int128_shl(Int128_ this, Int128_ that)
{
  return Int128_shl_(this, Int128_toUInt64(that));
}
#endif//$XCC_h

#ifndef TYPE_INT128
Int128_
Int128_shl_(Int128_ this, unsigned shift)
{
  Int128_ output;
  uint64_t previous = 0ULL;
  int i = 0, j = 0;
  assert(shift < 128);
  while (shift >= 64) {
    output.dwords[i] = previous;
    shift -= 64;
    i++;
  }
  while (i < 2) {
    output.dwords[i] = this.dwords[j] << shift;
    if (shift) {
      output.dwords[i] |= previous >> (64 - shift);
      previous = this.dwords[j];
    }
    i++, j++;
  }
  return output;
}
#endif//TYPE_INT128


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * Divide / Modulo Unsigned.
 */
Int256_
Int128_divmodu(Int128_ this, Int128_ that);
#endif//$XCC__h

Int256_
Int128_divmodu(Int128_ this, Int128_ that)
{
    Int256_ output;
    //
    Int128_ one = Int128_fromUInt64(1);
    //
#ifdef DEBUG_INT128
    printf("Input divmodu:\n");
    Int128_print(this);
    Int128_print(that);
#endif
    if (Int128_cmpu(that, this) > 0) {
        output.upper = Int128_fromUInt64(0);
        output.lower = this;
#ifdef DEBUG_INT128
        printf("Output:\n");
        Int256_print(output); 
#endif
        return output;
    }
    int k = 0;
    // While that <= (this / 2)
    while (Int128_cmpu(that, Int128_shru(this, one)) <= 0) {
        // that = that * 2
        that = Int128_shl(that, one);
        ++k;
    }
    // The upper part will be the quotient, the lower the remainder.
    output.upper = Int128_fromUInt64(0);
    output.lower = this;
    while (k + 1) {
        output.upper = Int128_shl(output.upper, one);
        if (Int128_cmpu(output.lower, that) >= 0) {
            output.upper = Int128_add(output.upper, one);
            output.lower = Int128_sub(output.lower, that);
        }
        that = Int128_shru(that, one);
        --k;
    }
#ifdef DEBUG_INT128
    printf("Output:\n");
    Int256_print(output); 
#endif
    return output;
}

#ifdef $XCC__h
/*
 * Divide / Modulo Signed.
 */
Int256_
Int128_divmod(Int128_ this, Int128_ that);
#endif//$XCC__h

Int256_
Int128_divmod(Int128_ this, Int128_ that)
{
    // We retrieve the sign
    int sign = (this.dwords[1] >> 63) ^ (that.dwords[1] >> 63)?-1:1;
#ifdef DEBUG_INT128
    printf("Input divmod:\n");
    Int128_print(this);
    Int128_print(that);
#endif
    // We convert inputs to positive values if they are negative.
    if (this.dwords[1] >> 63) {
        this = Int128_neg(this);
    }
    if (that.dwords[1] >> 63) {
        that = Int128_neg(that);
    }
    if (sign == -1) {
        Int256_ output = Int128_divmodu(this, that);
        output.lower = Int128_neg(output.lower);
        output.upper = Int128_neg(output.upper);
#ifdef DEBUG_INT128
        printf("Output:\n");
        Int256_print(output); 
#endif
        return output;
    } else {
        Int256_ output = Int128_divmodu(this, that);
#ifdef DEBUG_INT128
        printf("Output:\n");
        Int256_print(output); 
#endif
        return output;
    }
}

#ifdef $XCC_h
/*
 * Quotient
 */
Int128_
Int128_quo(Int128_ this, Int128_ that);
#endif//$XCC_h

Int128_
Int128_quo(Int128_ this, Int128_ that)
{
    Int256_ result = Int128_divmod(this, that);
#ifdef DEBUG_INT128
    printf("Input quo:\n");
    Int128_print(this);
    Int128_print(that);
#endif
#ifdef DEBUG_INT128
    printf("Output:\n");
    Int128_print(result.upper); 
#endif
    return result.upper;
}

#ifdef $XCC_h
/*
 * Remainder
 */
Int128_
Int128_rem(Int128_ this, Int128_ that);
#endif//$XCC_h

Int128_
Int128_rem(Int128_ this, Int128_ that)
{
    Int256_ result = Int128_divmod(this, that);
#ifdef DEBUG_INT128
    printf("Input rem:\n");
    Int128_print(this);
    Int128_print(that);
#endif
#ifdef DEBUG_INT128
    printf("Output:\n");
    Int128_print(result.lower); 
#endif
    return result.lower;
}

#ifdef $XCC_h
/*
 * Rotate Right
 * width: we work modulo bit-width
 * ror(4, 2, 11100110) = 00001010
 */
Int128_
Int128_ror(unsigned width, Int128_ this, Int128_ rotate);
#endif//$XCC_h

Int128_
Int128_ror(unsigned width, Int128_ this, Int128_ rotate)
{
  assert(width <= 128);
  // If we want to rotate more than width bits, we take the rest modulo width.
  unsigned shift = Int128_toUInt64(rotate) % width;
  Int128_ rshift = Int128_fromUInt32(shift);
  Int128_ lshift = Int128_fromUInt32(width - shift);
  Int128_ output = 
    Int128_or(
      Int128_shru(Int128_zx(this, width), rshift),
      Int128_zx(Int128_shl(this, lshift), width));
#ifdef DEBUG_INT128
  printf("Input ror.%d:\n", width);
  Int128_print(rotate);
  Int128_print(this);
#endif
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(output); 
#endif
  return output;
}

#ifdef $XCC_h
/*
 * Rotate on Left
 * width: we work modulo bit-width
 * rol(4, 2, 11100110) = 00001001
 */
Int128_
Int128_rol(unsigned width, Int128_ this, Int128_ rotate);
#endif//$XCC_h

Int128_
Int128_rol(unsigned width, Int128_ this, Int128_ rotate)
{
  assert(width <= 128);
  // If we want to rotate more than width bits, we take the rest modulo width.
  unsigned shift = Int128_toUInt64(rotate) % width;
  Int128_ rshift = Int128_fromUInt32(width - shift);
  Int128_ lshift = Int128_fromUInt32(shift);
  Int128_ output = 
    Int128_or(
      Int128_shru(Int128_zx(this, width), rshift),
      Int128_zx(Int128_shl(this, lshift), width));
#ifdef DEBUG_INT128
  printf("Input rol.d:\n", width);
  Int128_print(rotate);
  Int128_print(this);
#endif
#ifdef DEBUG_INT128
  printf("Output:\n");
  Int128_print(output); 
#endif
  return output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  ////////////////////////////
  // Test Int128_print 
  ////////////////////////////
  int i;
  // first flit, second flit
  uint64_t values[][2] = { {0xFFFFFFFF00000000LLU, 0xFFFFFFFF10000004LLU},
        {0, 7, },
        {0x0126456789ABCDEFLLU, 0x01234467896BCDEFLLU},
        {0x0123453782ABCDEFLLU, 0x0123436779ABCDEFLLU},
        {0xAAA8AAAAAAAAAAAALLU, 0xAA2AA2AAAA4AAAAALLU}
    };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input;
    Int128_Ctor(&input, values[i][1], values[i][0]);
    Int128_print(input);
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test or
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][6] = {
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0, 0, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU },
    { 0xAAAAAAAAAAAAAAAALLU, 0xAAAAAAAAAAAAAAAALLU, 0x5555555555555555LLU, 0x5555555555555555LLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU},
    { 0xCCCCCCCCCCCCCCCCLLU, 0xCCCCCCCCCCCCCCCCLLU, 0x3333333333333333LLU, 0x3333333333333333LLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ correct;
    Int128_ input_1, input_2;
    Int128_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int128_or(input_1, input_2);
    Int128_Ctor(&correct, values[i][5], values[i][4]);
    printf("OR test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Inputs:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result: ");
      Int128_print(output);
      printf("Expected: ");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test xor
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][6] = {
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0, 0, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU },
    { 0x00000000FFFFFFFFLLU, 0xAAAAAAAA00000000LLU, 0x00000000FFFFFFFFLLU, 0x4444444455555555LLU, 0, 0xEEEEEEEE55555555LLU},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ correct;
    Int128_ input_1, input_2;
    Int128_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int128_xor(input_1, input_2);
    Int128_Ctor(&correct, values[i][5], values[i][4]);
    printf("XOR test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Inputs:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result: ");
      Int128_print(output);
      printf("Expected: ");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test and
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][6] = {
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0, 0, 0, 0},
    { 0x00000000FFFFFFFFLLU, 0xAAAAAAAA33333333LLU, 0x00000000FFFFFFFFLLU, 0x7777777755555555LLU, 0x00000000FFFFFFFFLLU, 0x2222222211111111LLU},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ correct;
    Int128_ input_1, input_2;
    Int128_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int128_and(input_1, input_2);
    Int128_Ctor(&correct, values[i][5], values[i][4]);
    if (!Int128_equals(output, correct)) {
      printf("AND%d failed\n", i+1);
      printf("Inputs:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result: ");
      Int128_print(output);
      printf("Expected: ");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test not
  ////////////////////////////
  int i;
  // operand1, result
  uint64_t values[][4] = {
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0, 0},
    { 0xCCCCCCCC55555555LLU, 0x3333333311111111LLU, 0x33333333AAAAAAAALLU, 0xCCCCCCCCEEEEEEEELLU},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ correct;
    Int128_ input;
    Int128_ output;
    Int128_Ctor(&input, values[i][1], values[i][0]);
    output = Int128_not(input);
    Int128_Ctor(&correct, values[i][3], values[i][2]);
    printf("NOT test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Inputs:\n");
      Int128_print(input);
      printf("Result: ");
      Int128_print(output);
      printf("Expected: ");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test clz
  ////////////////////////////
  int i;
  // operand1, result
  uint64_t values[][3] = {
    { 0, 0x000000000000FFFF, 112},
    { 0x000537EA00000000, 0, 13},
    { 0xFFFFFFFF00000000, 0, 0},
    { 0, 0, 128},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input;
    uint64_t correct = values[i][2];
    unsigned output;
    Int128_Ctor(&input, values[i][1], values[i][0]);
    output = Int128_clz(input, 128);
    printf("CLZ test %d\n", i+1);
    if (output != correct) {
      printf("test %d failed\n", i+1);
      printf("Inputs:\n");
      Int128_print(input);
      printf("Result: %u\n", output);
      printf("Expected: %0"PRId64"\n", correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test ctz
  ////////////////////////////
  int i;
  // operand1, result
  uint64_t values[][3] = {
    { 0, 0x00000000FFFF0000, 16},
    { 0x537EA00000000000, 0, 109},
    { 0, 0, 128},
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input;
    uint64_t correct = values[i][2];
    unsigned output;
    Int128_Ctor(&input, values[i][1], values[i][0]);
    output = Int128_ctz(input, 128);
    printf("CTZ test %d\n", i+1);
    if (output != correct) {
      printf("test %d failed\n", i+1);
      printf("Inputs:\n");
      Int128_print(input);
      printf("Result: %u\n", output);
      printf("Expected: %0"PRId64"\n", correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test cls
  ////////////////////////////
  int i;
  // operand1, result
  uint64_t values[][3] = {
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 127},
    { 0, 0, 127},
    { 0x0000000000FFFFFFLLU, 0, 39},
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFF574638LLU, 103},
    { 0, 1, 126},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input;
    uint64_t correct = values[i][2];
    unsigned output;
    Int128_Ctor(&input, values[i][1], values[i][0]);
    output = Int128_cls(input, 128);
    printf("CLS test %d\n", i+1);
    if (output != correct) {
      printf("test %d failed\n", i+1);
      printf("Inputs:\n");
      Int128_print(input);
      printf("Result: %u\n", output);
      printf("Expected: %0"PRId64"\n", correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test cbs
  ////////////////////////////
  int i;
  // operand1, result
  uint64_t values[][3] = {
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 128},
    { 0, 0, 0},
    { 0xACAE034F00000000LLU, 0xACAE034F00000000LU, 32},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input;
    uint64_t correct = values[i][2];
    unsigned output;
    Int128_Ctor(&input, values[i][1], values[i][0]);
    output = Int128_cbs(input, 128);
    printf("CBS test %d\n", i+1);
    if (output != correct) {
      printf("test %d failed\n", i+1);
      printf("Inputs:\n");
      Int128_print(input);
      printf("Result: %u\n", output);
      printf("Expected: %0"PRId64"\n", correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test swap
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][5] = {
    { 0x0123456789ABCDEFLLU, 0x76543210FEDCBA98LLU, 1, 0x02138A9B4657CEDFLLU, 0xB9A83120FDEC7564LLU},
    { 0x0123456789ABCDEFLLU, 0x76543210FEDCBA98LLU, 2, 0x048C159D26AE37BFLLU, 0xD951C840FB73EA62LLU},
    { 0x0123456789ABCDEFLLU, 0x76543210FEDCBA98LLU, 4, 0x1032547698BADCFELLU, 0x67452301EFCDAB89LLU},
    { 0x0123456789ABCDEFLLU, 0x76543210FEDCBA98LLU, 8, 0x23016745AB89EFCDLLU, 0x54761032DCFE98BALLU},
    { 0x0123456789ABCDEFLLU, 0x76543210FEDCBA98LLU, 16, 0x45670123CDEF89ABLLU, 0x32107654BA98FEDCLLU},
    { 0x0123456789ABCDEFLLU, 0x76543210FEDCBA98LLU, 32, 0x89ABCDEF01234567LLU, 0xFEDCBA9876543210LLU},
    { 0x0123456789ABCDEFLLU, 0x76543210FEDCBA98LLU, 64, 0x76543210FEDCBA98LLU, 0x0123456789ABCDEFLLU},
    { 0x0123456789ABCDEFLLU, 0x76543210FEDCBA98LLU, 43, 0x0123456789ABCDEFLLU, 0x76543210FEDCBA98LLU},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1;
    Int128_ output;
    Int128_ correct;
    unsigned input_2 = values[i][2];
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    output = Int128_swap(input_1, input_2);
    Int128_Ctor(&correct, values[i][4], values[i][3]);
    printf("SWAP test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Inputs:\n");
      Int128_print(input_1);
      printf("%d\n", input_2);
      printf("Result:\n");
      Int128_print(output);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test zx
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][5] = {
    { 0xFFFFFFFFFFFFFFFFLLU, 0x0000CDCD12345678LLU, 124, 0x0FFFFFFFFFFFFFFFLLU, 0x0000CDCD12345678LLU},
    { 0xFFFFFFFFFFFFFFFFLLU, 0x0000CDCD12345678LLU, 64, 0, 0x0000CDCD12345678LLU},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1;
    Int128_ output;
    Int128_ correct;
    uint64_t input_2 = values[i][2];
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    output = Int128_zx(input_1, input_2);
    Int128_Ctor(&correct, values[i][4], values[i][3]);
    printf("ZX test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Inputs:\n");
      Int128_print(input_1);
      printf("%0"PRId64"\n", input_2);
      printf("Result:\n");
      Int128_print(output);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test sx
  ////////////////////////////
  int i;
  /* operand, bit number, expected result */
  uint64_t values[][5] = {
    {0x08000000FFFFFFFFLLU, 0xFF00CDCD12345678LLU, 124, 0xF8000000FFFFFFFFLLU, 0xFF00CDCD12345678LLU},
    {0x08000000FFFFFFFFLLU, 0xFF00CDCD12345678LLU, 53, 0, 0x0000CDCD12345678LLU},
    {0x08000000FFFFFFFFLLU, 0xFF00CDCD12345678LLU, 36, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFD12345678LLU},
    {0x0800000000000000LLU, 0x0F00CDCD12345678LLU, 64, 0, 0x0F00CDCD12345678LLU},
    {0x0800000000000000LLU, 0x8F00CDCD12345678LLU, 64, 0xFFFFFFFFFFFFFFFFLLU, 0x8F00CDCD12345678LLU},
    {0, 0, 16, 0, 0},
    {0, 0, 32, 0, 0},
  };
  for(i=0; i < sizeof(values)/sizeof(*values); i++) {
    Int128_ correct;
    Int128_ input;
    Int128_ output;
    Int128_Ctor(&input, values[i][1], values[i][0]);
    output = Int128_sx(input, values[i][2]);
    Int128_Ctor(&correct, values[i][4], values[i][3]);
    printf("SX test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("Failure: SX.%0"PRId64" of:",values[i][4]);
      Int128_print(input);
      printf("Result:  ");
      Int128_print(output);
      printf("Expected: ");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test sat
  ////////////////////////////
  int i;
  /* operand, bit number, expected result */
  uint64_t values[][5] = {
    {0x08000000FFFFFFFFLLU, 0xFF00CDCD12345678LLU, 128, 0x08000000FFFFFFFFLLU, 0xFF00CDCD12345678LLU},
    {0x08000000FFFFFFFFLLU, 0x0F00CDCD12345678LLU, 64, 0, 0x7FFFFFFFFFFFFFFFLLU},
    {0x88000000FFFFFFFFLLU, 0x0F00CDCD12345678LLU, 64, 0xFFFFFFFFFFFFFFFFLLU, 0x8000000000000000LLU},
    {0x08000000FFFFFFFFLLU, 0xFF00CDCD12345678LLU, 31, 0, 0x000000003FFFFFFFLLU},
    {0x88000000FFFFFFFFLLU, 0xFF00CDCDF2345678LLU, 31, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFc0000000LLU},
    {0x8000000000000000LLU, 0, 64, 0xFFFFFFFFFFFFFFFFLLU, 0x8000000000000000LLU},
  };
  for(i=0; i < sizeof(values)/sizeof(*values); i++) {
    Int128_ correct;
    Int128_ input;
    Int128_ output;
    Int128_Ctor(&input, values[i][1], values[i][0]);
    output = Int128_sat(input, values[i][2]);
    Int128_Ctor(&correct, values[i][4], values[i][3]);
    printf("SAT test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("Failure: %d-th SAT.%0"PRId64" of:", i+1, values[i][2]);
      Int128_print(input);
      printf("Result:  ");
      Int128_print(output);
      printf("Expected: ");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test toBool
  ////////////////////////////
  int i;
  // operand, result
  uint64_t toBool_values[][3] = {
    { 0, 0, 0},
    { 1, 0, 1},
    { 0x0000000100000000LLU, 0x0000000100000000LLU, 1},
  };
  for (i = 0; i < sizeof(toBool_values) / (8*3); ++i) {
    Int128_ input;
    uint64_t correct = toBool_values[i][2];
    unsigned outputBool;
    Int128_Ctor(&input, toBool_values[i][1], toBool_values[i][0]);
    outputBool = Int128_toBool(input);
    printf("TOBOOL test %d\n", i+1);
    if (outputBool != correct) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input);
      printf("Result: %u\n", outputBool);
      printf("Expected: %0"PRId64"\n", correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test fromBool
  ////////////////////////////
  int i;
  // operand, result
  uint64_t fromBool_values[][3] = {
    { 1, 0, 1},
    { 0, 0, 0},
  };
  for (i = 0; i < sizeof(fromBool_values) / (8*3); ++i) {
    uint64_t input = fromBool_values[i][2];
    Int128_ outputBool = Int128_fromBool(input);
    Int128_ correct;
    Int128_Ctor(&correct, fromBool_values[i][2], fromBool_values[i][1]);
    outputBool = Int128_fromBool(input);
    printf("FROMBOOL test %d\n", i+1);
    if (Int128_cmp(outputBool, correct) != 0) {
      printf("test %d failed\n", i+1);
      printf("Input: %0"PRId64"\n", input);
      printf("Result:\n");
      Int128_print(outputBool);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test cmp
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][5] = {
    { 0, 4, 0, 6, -1},
    { 0, 4, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 1},
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFCLLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, -1},
    { 0, 4, 0, 4, 0},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    int output, correct;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int128_cmp(input_1, input_2);
    correct = values[i][4];
    printf("CMP test %d\n", i+1);
    if (output != correct) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result: %d\n", output);
      printf("Expected: %d\n", correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test neg 
  ////////////////////////////
  int i;
  // operand, result
  uint64_t values[][4] = {
    { 0, 0, 0, 0},
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0, 1},
    { 0, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 1},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input;
    Int128_ correct;
    Int128_ output;
    Int128_Ctor(&input, values[i][1], values[i][0]);
    output = Int128_neg(input);
    Int128_Ctor(&correct, values[i][3], values[i][2]);
    printf("NEG test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input);
      printf("Result:\n");
      Int128_print(output);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test add 
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][6] = {
    { 0x0123456789ABCDEFLLU, 0x3333333325789349LLU, 0x7654321076543210LLU, 0x5218F32918264893LLU, 0x77777777FFFFFFFFLLU, 0x854C265C3D9EDBDCLLU},
    { 0, 0xFFFFFFFFFFFFFFFFLLU, 0, 0xFFFFFFFFFFFFFFFFLLU, 1, 0xFFFFFFFFFFFFFFFELLU},
    { 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0, 1, 0, 0},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    Int128_ correct;
    Int128_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int128_add(input_1, input_2);
    Int128_Ctor(&correct, values[i][5], values[i][4]);
    printf("ADD test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result:\n");
      Int128_print(output);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test shr 
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][6] = {
    {0xFFFFFFFFFFFF0000LLU, 0x0000000010000000LLU, 0, 4, 0xFFFFFFFFFFFFF000LLU, 0x0000000001000000LLU},
    {0x0123456700000000LLU, 0, 0, 37, 0x0000000000091A2BLLU, 0x3800000000000000LLU},
    {0xFFFFFFFFFFFF0000LLU, 0x0000000010000000LLU, 0, 0, 0xFFFFFFFFFFFF0000LLU, 0x0000000010000000LLU},
    {0xDEADBEEF88888888LLU, 0x8888888888888888LLU, 0, 124, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFD},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    Int128_ correct;
    Int128_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int128_shr(input_1, input_2);
    Int128_Ctor(&correct, values[i][5], values[i][4]);
    printf("SHR test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result:\n");
      Int128_print(output);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test shru 
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][6] = {
    {0x0123456789ABCDEFLLU, 0x0123456789ABCDEFLLU, 0, 4, 0x00123456789ABCDELLU, 0xF0123456789ABCDELLU},
    {0x0123456789ABCDEFLLU, 0x0123456789ABCDEFLLU, 0, 36, 0x0000000000123456LLU, 0x789ABCDEF0123456LLU},
    {0x8123456789ABCDEFLLU, 0x0123456789ABCDEFLLU, 0, 64, 0x0000000000000000LLU, 0x8123456789ABCDEFLLU},
    {0x0123456789ABCDEFLLU, 0x0123456789ABCDEFLLU, 0, 0, 0x0123456789ABCDEFLLU, 0x0123456789ABCDEFLLU},
    {0xAAAAAAAAAAAAAAAALLU, 0xAAAAAAAAAAAAAAAALLU, 0, 7, 0x0155555555555555LLU, 0x5555555555555555LLU},
    {0xAAAAAAAAAAAAAAAALLU, 0xAAAAAAAAAAAAAAAALLU, 0, 127, 0, 0x1},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    Int128_ correct;
    Int128_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int128_shru(input_1, input_2);
    Int128_Ctor(&correct, values[i][5], values[i][4]);
    printf("SHRU test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result:\n");
      Int128_print(output);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
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
  uint64_t values[][6] = {
    {0x8888888888888888LLU, 0x8888888888888888LLU, 0, 49, 0x1111111111111111LLU, 0x1110000000000000LLU},
    {1, 0x0000000200000003LLU, 0, 64, 0x0000000200000003LLU, 0},
    {1, 0x00000002F0000003LLU, 0, 68, 0x0000002F00000030LLU, 0},
    {0xDEADBEEF88888888LLU, 0x88888888DEADBEEFLLU, 0, 120, 0xEF00000000000000, 0x0000000000000000},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    Int128_ correct;
    Int128_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int128_shl(input_1, input_2);
    Int128_Ctor(&correct, values[i][5], values[i][4]);
    printf("SHL test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result:\n");
      Int128_print(output);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test ror 
  ////////////////////////////
  int i;
  // operand1, operand2, operand3 result
  uint64_t values[][7] = {
    {0xFFFFFFFF00000000LLU, 0xFFFFFFFF10000004LLU, 0xFFFFFFFFFFFF0000LLU, 0xAAAAAAAA12345678LLU, 64, 0, 0x8AAAAAAAA1234567LLU},
    {0xFFFFFFFF00000000LLU, 0xFFFFFFFF10000028LLU, 0xFFFFFFFFFFFF0000LLU, 0xAAAAAAAA12345678LLU, 64, 0, 0xAA12345678AAAAAALLU},
    {0, 5, 0xFFFFFFFFFFFF0000LLU, 0xAAAAAAAA66666666LLU, 37, 0, 0x0000000653333333LLU},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    int input_3;
    Int128_ correct;
    Int128_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    input_3 = values[i][4];
    output = Int128_ror(input_3, input_2, input_1);
    Int128_Ctor(&correct, values[i][6], values[i][5]);
    printf("ROR test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n%d\n", input_3);
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result:\n");
      Int128_print(output);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test rol 
  ////////////////////////////
  int i;
  // operand1, operand2, operand3 result
  uint64_t values[][7] = {
    {0xFFFFFFFF00000000LLU, 0xFFFFFFFF10000004LLU, 0xFFFFFFFFFFFF0000LLU, 0xAAAAAAAA12345678LLU, 64, 0, 0xAAAAAAA12345678ALLU},
    {0, 7, 0xFFFFFFFFFFFF0000LLU, 0xAAAAAAAA11111111LLU, 97, 0x00000001FF800055LLU, 0x55555508888888FFLLU},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    int input_3;
    Int128_ correct;
    Int128_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    input_3 = values[i][4];
    output = Int128_rol(input_3, input_2, input_1);
    Int128_Ctor(&correct, values[i][6], values[i][5]);
    printf("ROL test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n%d\n", input_3);
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result:\n");
      Int128_print(output);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test mulu 
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][8] = {
    {0, 1, 0, 1, 0, 0, 0, 1},
    {0, 1, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFELLU, 0, 0, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFELLU},
    {0x0123456789ABCDEFLLU, 0x3333333325789349LLU, 0x7654321076543210LLU, 0x5218F32918264893LLU, 0x0086A1C975B78E71LLU, 0x729A529FE5BDDCB7LLU, 0x1460469F37BB45F4LLU, 0xD6DD884E227F1AEBLLU},
    {0xFFFFFFFFFCABF915LLU, 0xFFFFFFFFFCABF915LLU, 0xDE8E4B7FBD1C9800LLU, 0xDE8E4B7FBD1C9800LLU, 0xDE8E4B7FBA37E06ALLU, 0xD596E7244FA038D3LLU, 0x0F82EBC97098D069LLU, 0x187A5024DB307800LLU},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    Int256_ correct;
    Int256_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int256_mulu128(input_1, input_2);
    Int256_Ctor(&correct, values[i][7], values[i][6], values[i][5], values[i][4]);
    printf("MULU test %d\n", i+1);
    if (!Int256_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result:\n");
      Int256_print(output);
      printf("Expected:\n");
      Int256_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test mul 
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][8] = {
    {0, 1, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFELLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFELLU},
    {0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFELLU, 0, 7, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFF2LLU},
    {0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFELLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFF9LLU, 0, 0, 0, 14},
    {0, 0xFFFFFFFFFCABF915LLU, 0, 0XDE8E4B7FBD1C9800LLU, 0, 0, 0XDE8E4B7FBA37E069LLU, 0X187A5024DB307800LLU },
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    Int256_ correct;
    Int256_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int256_mul128(input_1, input_2);
    Int256_Ctor(&correct, values[i][7], values[i][6], values[i][5], values[i][4]);
    printf("MUL test %d\n", i+1);
    if (!Int256_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result:\n");
      Int256_print(output);
      printf("Expected:\n");
      Int256_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test mul64 
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t mul64_values[][6] = {
    {0, 1, 0, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU},
    {0, 0xFFFFFFFFFFFFFFFFLLU, 0, 0xFFFFFFFFFFFFFFFFLLU, 0, 1},
    {0, 0xFFFFFFFFFFFFFFFELLU, 0, 7, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFF2LLU},
    {0, 0xFFFFFFFFFFFFFFFELLU, 0, 0xFFFFFFFFFFFFFFF9LLU, 0, 14},
    {0, 0x12bb8fb82feLLU, 0, 0x12c28b29fa2LLU,  0x15F6C, 0x6CC652900A20A6BCLLU},
    {0, 0x2BBE12E2CD52B5LLU, 0, 0xFFFFFFFFFFFFFFFELLU,  0xFFFFFFFFFFFFFFFFLLU, 0xFFA883DA3A655A96LLU},
  };
  for (i = 0; i < sizeof(mul64_values) / (8*6); ++i) {
    Int128_ input_1, input_2;
    Int128_ correct;
    Int128_ output;
    Int128_Ctor(&input_1, mul64_values[i][1], mul64_values[i][0]);
    Int128_Ctor(&input_2, mul64_values[i][3], mul64_values[i][2]);
    output = Int128_mul64(input_1, input_2);
    Int128_Ctor(&correct, mul64_values[i][5], mul64_values[i][4]);
    printf("MUL64 test %d\n", i+1);
    if (!Int128_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result:\n");
      Int128_print(output);
      printf("Expected:\n");
      Int128_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test divmodu 
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][8] = {
    {0, 3, 0, 22, 0, 0, 0, 3},
    {0, 22, 0, 3, 0, 7, 0, 1},
    {0xFA56728301234567LLU, 0x1111111119283945LLU, 0x0000000018273948LLU, 0x000000009478FBACLLU, 0, 0x0000000A5D545F99LLU, 0x000000000FD05159LLU, 0x0E3659C76704FB79LLU},
    {0x0192837D1827B0DELLU, 0xBA98054600000000LLU, 0, 1, 0x0192837D1827B0DELLU, 0xBA98054600000000LLU, 0, 0},
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    Int256_ correct;
    Int256_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int128_divmodu(input_1, input_2);
    Int256_Ctor(&correct, values[i][7], values[i][6], values[i][5], values[i][4]);
    printf("DIVMODU test %d\n", i+1);
    if (!Int256_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input_1);
      Int128_print(input_2);
      printf("Result:\n");
      Int256_print(output);
      printf("Expected:\n");
      Int256_print(correct);
      exit(1);
    }
  }
}
#endif//$XCC__c

#ifdef $XCC__c
{
  ////////////////////////////
  // Test divmod 
  ////////////////////////////
  int i;
  // operand1, operand2, result
  uint64_t values[][8] = {
    {0, 3, 0, 22, 0, 0, 0, 3},
    {0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFEALLU, 0, 3, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFF9LLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU},
    {0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFEALLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFDLLU, 0, 7, 0, 1},
    {0, 22, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFDLLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFF9LLU, 0xFFFFFFFFFFFFFFFFLLU, 0xFFFFFFFFFFFFFFFFLLU}
  };
  for (i = 0; i < sizeof(values)/sizeof(*values); ++i) {
    Int128_ input_1, input_2;
    Int256_ correct;
    Int256_ output;
    Int128_Ctor(&input_1, values[i][1], values[i][0]);
    Int128_Ctor(&input_2, values[i][3], values[i][2]);
    output = Int128_divmod(input_1, input_2);
    Int256_Ctor(&correct, values[i][7], values[i][6], values[i][5], values[i][4]);
    printf("DIVMOD test %d\n", i+1);
    if (!Int256_equals(output, correct)) {
      printf("test %d failed\n", i+1);
      printf("Input:\n");
      Int128_print(input_1);
      Int128_print(input_2);
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
 * Initialize the Int128 module.
 */
#define Int128_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Int128 module.
 */
#define Int128_FINI()
#endif//$XCC__h

#if XCC__C
static void
Int128_TEST(void)
{
#include "BSL/Int128_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(Int128_TEST);
  return 0;
}
#endif

