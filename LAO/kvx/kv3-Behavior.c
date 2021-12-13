#ifdef $XCC_h
/*
 * kv3-Behavior.c
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

#ifdef $XCC__h
#define THREAD_LOCAL  __thread
#include <math.h>
#include "softfloat.h"
//
#define f16_isNaN(f16)	((f16.v>>10 & ((1<<5)-1)) == ((1<<5)-1) && (f16.v & ((1<<10)-1)) != 0)
#define f32_isNaN(f32)	((f32.v>>23 & ((1<<8)-1)) == ((1<<8)-1) && (f32.v & ((1<<23)-1)) != 0)
#define f64_isNaN(f64)	((f64.v>>52 & ((1<<11)-1)) == ((1<<11)-1) && (f64.v & ((1LL<<52)-1)) != 0)

#define f32_isSigNaN(f32) (f32_isNaN(f32) && (f32.v & (1 << 22)) == 0)
#define f16_isSigNaN(f16) (f16_isNaN(f16) && (f16.v & (1 << 9)) == 0) 

#define f16_isInf(f16)	((f16.v>>10 & ((1<<5)-1)) == ((1<<5)-1) && (f16.v & ((1<<10)-1)) == 0)
#define f32_isInf(f32)	((f32.v>>23 & ((1<<8)-1)) == ((1<<8)-1) && (f32.v & ((1<<23)-1)) == 0)

#define f32_isNaN_flip(f32)	((f32>>23 & ((1<<8)-1)) == ((1<<8)-1) && (f32 & ((1<<23)-1)) != 0)
#define f64_isNaN_flip(f64)	((f64>>52 & ((1<<11)-1)) == ((1<<11)-1) && (f64 & ((1LL<<52)-1)) != 0)
//
#define f16_defaultNaN	(float16_t){(((1<<(5+1))-1) << (10-1))}
#define f32_defaultNaN	(float32_t){(((1<<(8+1))-1) << (23-1))}
#define f64_defaultNaN	(float64_t){(((1LL<<(11+1))-1) << (52-1))}

#define f32_defaultNaN_flip	(flip32){(((1<<(8+1))-1) << (23-1))}
#define f64_defaultNaN_flip	(flip64){(((1LL<<(11+1))-1) << (52-1))}

#define f32_minusZero_flip	(flip32){(1ull << 31)}
//
#define fixupNaN_f16_f16(r, a)	(f16_isNaN(r) || f16_isNaN(a))? f16_defaultNaN: (r)
#define fixupNaN_f16_f32(r, a)	(f16_isNaN(r) || f32_isNaN(a))? f16_defaultNaN: (r)
#define fixupNaN_f16_f16_f16(r, a, b)	(f16_isNaN(r) || f16_isNaN(a) || f16_isNaN(b))? f16_defaultNaN: (r)
#define fixupNaN_f16_f16_f16_f16(r, a, b, c)	(f16_isNaN(r) || f16_isNaN(a) || f16_isNaN(b) || f16_isNaN(c))? f16_defaultNaN: (r)
//
#define fixupNaN_f32_f16(r, a)	(f32_isNaN(r) || f16_isNaN(a))? f32_defaultNaN: (r)
#define fixupNaN_f32_f32(r, a)	(f32_isNaN(r) || f32_isNaN(a))? f32_defaultNaN: (r)
#define fixupNaN_f32_f64(r, a)	(f32_isNaN(r) || f64_isNaN(a))? f32_defaultNaN: (r)
#define fixupNaN_f32_f16_f16(r, a, b)	(f32_isNaN(r) || f16_isNaN(a) || f16_isNaN(b))? f32_defaultNaN: (r)
#define fixupNaN_f32_f32_f32(r, a, b)	(f32_isNaN(r) || f32_isNaN(a) || f32_isNaN(b))? f32_defaultNaN: (r)
#define fixupNaN_f32_f32_f16_f16(r, a, b, c)	(f32_isNaN(r) || f32_isNaN(a) || f16_isNaN(b) || f16_isNaN(c))? f32_defaultNaN: (r)
#define fixupNaN_f32_f32_f32_f32(r, a, b, c)	(f32_isNaN(r) || f32_isNaN(a) || f32_isNaN(b) || f32_isNaN(c))? f32_defaultNaN: (r)
#define fixupNaN_f32_f32_f32_f32_f32(r, a, b, c, d)	(f32_isNaN(r) || f32_isNaN(a) || f32_isNaN(b) || f32_isNaN(c) || f32_isNaN(d))? f32_defaultNaN: (r)
#define fixupNaN_f32_f32_f32_f32_f32_f32(r, a, b, c, d, e)	(f32_isNaN(r) || f32_isNaN(a) || f32_isNaN(b) || f32_isNaN(c) || f32_isNaN(d) || f32_isNaN(e))? f32_defaultNaN: (r)

#define fixupNaN_f32_f32_flip(r, a)	(f32_isNaN_flip(r) || f32_isNaN_flip(a))? f32_defaultNaN_flip: (r)
#define fixupNaN_f32_f32_f32_f32_f32_flip(r, a, b, c, d)	(f32_isNaN_flip(r) || f32_isNaN_flip(a) || f32_isNaN_flip(b) || f32_isNaN_flip(c) || f32_isNaN_flip(d))? f32_defaultNaN_flip: (r)
#define fixupNaN_f64_f32_f32_f32_f32_flip(r, a, b, c, d)	(f64_isNaN_flip(r) || f32_isNaN_flip(a) || f32_isNaN_flip(b) || f32_isNaN_flip(c) || f32_isNaN_flip(d))? f64_defaultNaN_flip: (r)
//
#define fixupNaN_f64_f32(r, a)	(f64_isNaN(r) || f32_isNaN(a))? f64_defaultNaN: (r)
#define fixupNaN_f64_f64(r, a)	(f64_isNaN(r) || f64_isNaN(a))? f64_defaultNaN: (r)
#define fixupNaN_f64_f32_f32(r, a, b)	(f64_isNaN(r) || f32_isNaN(a) || f32_isNaN(b))? f64_defaultNaN: (r)
#define fixupNaN_f64_f64_f64(r, a, b)	(f64_isNaN(r) || f64_isNaN(a) || f64_isNaN(b))? f64_defaultNaN: (r)
#define fixupNaN_f64_f64_f32_f32(r, a, b, c)	(f64_isNaN(r) || f64_isNaN(a) || f32_isNaN(b) || f32_isNaN(c))? f64_defaultNaN: (r)
#define fixupNaN_f64_f64_f64_f64(r, a, b, c)	(f64_isNaN(r) || f64_isNaN(a) || f64_isNaN(b) || f64_isNaN(c))? f64_defaultNaN: (r)
#define fixupNaN_f64_f32_f32_f32_f32(r, a, b, c, d)	(f64_isNaN(r) || f32_isNaN(a) || f32_isNaN(b) || f32_isNaN(c) || f32_isNaN(d))? f64_defaultNaN: (r)
//
static inline float16_t f16_neg(float16_t a) { a.v ^= 0x8000; return a; }
static inline float32_t f32_neg(float32_t a) { a.v ^= 0x80000000; return a; }
static inline float64_t f64_neg(float64_t a) { a.v ^= 0x8000000000000000LL; return a; }
//
#define f16_mulSub(a, b, c)	f16_mulAdd(a, f16_neg(b), c)
#define f32_mulSub(a, b, c)	f32_mulAdd(a, f32_neg(b), c)
#define f64_mulSub(a, b, c)	f64_mulAdd(a, f64_neg(b), c)
#endif//$XCC__h

#ifdef $XCC_h
// Bit-Matrix 8x8 Multiplication
static inline uint64_t
uint64_bmm8(uint64_t a, uint64_t b) __attribute__ ((unused));
static inline uint64_t
uint64_bmm8(uint64_t a, uint64_t b)
{
  int i, j;
  uint64_t c = 0;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (a & 0x1) {
        c ^= ((b>>(j*8))&0xFF) << i*8;
      }
      a >>= 1;
    }
  }
  return c;
}
#endif//$XCC_h

#ifdef $XCC_h
// Bit-Matrix 8x8 Transposition
static inline uint64_t
uint64_bmt8(uint64_t a) __attribute__ ((unused));
static inline uint64_t
uint64_bmt8(uint64_t a)
{
  int i, j;
  uint64_t b = 0;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (a & 0x1) {
        b ^= (uint64_t)0x1 << ((j*8) + i);
      }
      a >>= 1;
    }
  }
  return b;
}
#endif//$XCC_h

#ifdef $XCC__h
static inline Int256_
Behavior__BMM_8(void *this, Int256_ opnd1, Int256_ opnd2)
{
  uint64_t a = Int256_toUInt64(opnd1); 
  uint64_t b = Int256_toUInt64(opnd2); 
  uint64_t c = uint64_bmm8(a, b);
  return Int256_fromUInt64(c);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior__BMT_8(void *this, Int256_ opnd1)
{
  uint64_t a = Int256_toUInt64(opnd1);
  uint64_t b = uint64_bmt8(a);
  return Int256_fromUInt64(b);
}
#endif//$XCC__h

/** bypass undefined behavior when shift amount exceeds
 *  type size */
#ifdef $XCC__h
static inline uint64_t
UNBOUND_U64_RSHIFT(uint64_t x, unsigned s) __attribute__((unused));
static inline uint64_t
UNBOUND_U64_RSHIFT(uint64_t x, unsigned s)
{
    return ((s) >= 64 ? 0 : (uint64_t) (x) >> (s));
}
#endif // $XCC__h

/** 64-bit reflection */
#ifdef $XCC__h
static inline uint64_t
reflect_64(uint64_t op) __attribute__((unused));
static inline uint64_t
reflect_64(uint64_t op)
{
    uint64_t result = 0;
    int i;
    for (i = 0; i < 64; ++i) {
        result = (result << 1) ^ (op & 1);
        op >>= 1;
    }
    return result;
}
#endif // $XCC__h

/** 32-bit reflection */
#ifdef $XCC__h
static inline uint32_t
reflect_32(uint32_t op) __attribute__((unused));
static inline uint32_t
reflect_32(uint32_t op)
{
    uint32_t result = 0;
    int i;
    for (i = 0; i < 32; ++i) {
        result = (result << 1) ^ (op & 1);
        op >>= 1;
    }
    return result;
}
#endif // $XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_clm_64_128(void *this, Int256_ opnd1, Int256_ opnd2)
{
  uint64_t lhs = Int256_toUInt64(opnd1);
  uint64_t rhs = Int256_toUInt64(opnd2);
  uint64_t acc_lo = 0, acc_hi = 0;
  int i;

  for (i = 0; i < 64; ++i) {
    if ((lhs >> i) & 0x1ull) {
        acc_lo ^= (rhs << i);
        acc_hi ^= UNBOUND_U64_RSHIFT(rhs, (64 - i));
    }
  }

  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = acc_lo;
  Int256_dwords_1(result) = acc_hi;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_gcm_bb_64_128(void *this, Int256_ opnd1, Int256_ opnd2)
{
  uint64_t lhs = Int256_toUInt64(opnd1);
  uint64_t rhs = Int256_toUInt64(opnd2);

  // reflecting inputs
  lhs = reflect_64(lhs);
  rhs = reflect_64(rhs);

  uint64_t acc_lo = 0, acc_hi = 0;
  uint64_t mul_lo = 0, mul_hi = 0;
  int i;

  /** Carry Less Multiplication lhs x rhs */
  for (i = 0; i < 64; ++i) {
    if ((lhs >> i) & 0x1ull) {
        mul_lo ^= (rhs << i);
        mul_hi ^= UNBOUND_U64_RSHIFT(rhs, (64 -i));
    }
  }

  // X^7 + X^2 + X + 1
  uint64_t gcm_mod_lo = (1 << 7) ^ (1 << 2) ^ (1 << 1) ^ (1 << 0);

  // adding low part in accumulator MSB
  acc_hi ^= mul_lo;
  // xoring (LHS x RHS) . X^0 to the part to be reduced
  acc_hi ^= mul_hi;
  acc_lo ^= mul_lo;

  // the following implementation only works because MOD's HI [127:64]
  // is equal to zero
  for (int i = 63; i >= 0; i--) {
      // extracting i-th bit from hi
      if ((mul_hi >> i) & 0x1) {
          // if set xor modulo's MSB
          acc_lo ^= gcm_mod_lo << i ;
          acc_hi ^= UNBOUND_U64_RSHIFT(gcm_mod_lo, (64 - i));
      }
  }

  Int256_ result = Int256_zero;
  // reflecting outputs (including hi-lo swap)
  Int256_dwords_0(result) = reflect_64(acc_hi);
  Int256_dwords_1(result) = reflect_64(acc_lo);
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_gcm_bt_64_128(void *this, Int256_ opnd1, Int256_ opnd2)
{
  uint64_t lhs = Int256_toUInt64(opnd1);
  uint64_t rhs = Int256_toUInt64(opnd2);

  // reflecting inputs
  lhs = reflect_64(lhs);
  rhs = reflect_64(rhs);

  uint64_t acc_lo = 0, acc_hi = 0;
  uint64_t mul_lo = 0, mul_hi = 0;
  int i;

  /** Carry Less Multiplication lhs x rhs */
  for (i = 0; i < 64; ++i) {
    if ((lhs >> i) & 0x1ull) {
        mul_lo ^= (rhs << i);
        mul_hi ^= UNBOUND_U64_RSHIFT(rhs, (64 -i));
    }
  }

  // X^7 + X^2 + X + 1
  uint64_t gcm_mod_lo = (1 << 7) ^ (1 << 2) ^ (1 << 1) ^ (1 << 0);

  // adding low part in accumulator MSB
  acc_hi ^= mul_lo;

  // the following implementation only works because MOD's HI [127:64]
  // is equal to zero
  for (int i = 63; i >= 0; i--) {
      // extracting i-th bit from hi
      if ((mul_hi >> i) & 0x1) {
          // if set xor modulo's MSB
          acc_lo ^= gcm_mod_lo << i ;
          acc_hi ^= UNBOUND_U64_RSHIFT(gcm_mod_lo, (64 - i));
      }
  }

  Int256_ result = Int256_zero;
  // reflecting outputs (including hi-lo swap)
  Int256_dwords_0(result) = reflect_64(acc_hi);
  Int256_dwords_1(result) = reflect_64(acc_lo);
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_gcm_tt_64_128(void *this, Int256_ opnd1, Int256_ opnd2)
{
  uint64_t lhs = Int256_toUInt64(opnd1);
  uint64_t rhs = Int256_toUInt64(opnd2);
  // reflecting inputs
  lhs = reflect_64(lhs);
  rhs = reflect_64(rhs);
  uint64_t acc_lo = 0, acc_hi = 0;
  uint64_t mul_lo = 0, mul_hi = 0;
  int i;
  /** Carry Less Multiplication lhs x rhs */
  for (i = 0; i < 64; ++i) {
    if ((lhs >> i) & 0x1ull) {
        mul_lo ^= (rhs << i);
        mul_hi ^= UNBOUND_U64_RSHIFT(rhs, (64 -i));
    }
  }
  // X^7 + X^2 + X + 1
  uint64_t gcm_mod_lo = (1 << 7) ^ (1 << 2) ^ (1 << 1) ^ (1 << 0);
  // adding low part in accumulator MSB
  acc_hi = mul_lo;
  acc_hi ^= mul_hi; // xoring (LHS x RHS) . X^64 high part
  acc_lo = mul_lo;  // xoring (LHS x RHS) . X^64 low part
  // we reduced the bit [255:192] stored in mul_hi into the bits [191: 64]
  // stored in [acc_hi:acc_lo], initially acc_hi = mul_lo
  // the following implementation only works because MOD's HI [127:64]
  // is equal to zero
  for (int i = 63; i >= 0; i--) {
      // extracting i-th bit from hi
      if ((mul_hi >> i) & 0x1) {
          // if set xor modulo's MSB
          acc_lo ^= gcm_mod_lo << i ;
          acc_hi ^= UNBOUND_U64_RSHIFT(gcm_mod_lo, (64 - i));
      }
  }
  // we reduced the bit [191: 64] [acc_hi:acc_lo] into the bits
  // [127:0], initially set to [acc_lo:0]
  uint64_t extra_hi = acc_hi;
  acc_hi = acc_lo;
  acc_lo = 0;
  for (int i = 63; i >= 0; i--) {
      // extracting i-th bit from hi
      if ((extra_hi >> i) & 0x1) {
          // if set xor modulo's MSB
          acc_lo ^= gcm_mod_lo << i ;
          acc_hi ^= UNBOUND_U64_RSHIFT(gcm_mod_lo, (64 - i));
      }
  }
  Int256_ result = Int256_zero;
  // reflecting outputs (including hi-lo swap)
  Int256_dwords_0(result) = reflect_64(acc_hi);
  Int256_dwords_1(result) = reflect_64(acc_lo);
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_reflect_32(void *this, Int256_ opnd1)
{
  uint32_t op = Int256_toUInt32(opnd1);
  // reflecting inputs
  op = reflect_32(op);
  Int256_ result = Int256_fromUInt32(op);
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_crc32_be_u32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  uint32_t data = Int256_toUInt32(opnd1);
  uint32_t poly = Int256_toUInt32(opnd2);
  // reflecting inputs
  uint32_t crc = data;
  int i;
  for (i = 0; i < 32; ++i)
  {
    crc = (crc << 1) ^ ((crc & 0x80000000u) ? poly : 0);
  }
  Int256_ result = Int256_fromUInt32(crc);
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_comparison_(int value, int64_t sopnd2, int64_t sopnd3, uint64_t uopnd2, uint64_t uopnd3)
{
  switch(value) {
    case ModifierMemberValue_kv3_comparison_NE:
      return (sopnd2 != sopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_EQ:
      return (sopnd2 == sopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_LT:
      return (sopnd2 < sopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_GE:
      return (sopnd2 >= sopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_LE:
      return (sopnd2 <= sopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_GT:
      return (sopnd2 > sopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_LTU:
      return (uopnd2 < uopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_GEU:
      return (uopnd2 >= uopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_LEU:
      return (uopnd2 <= uopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_GTU:
      return (uopnd2 > uopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_ALL:
      return ((uopnd2 & uopnd3) == uopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_NALL:
      return !((uopnd2 & uopnd3) == uopnd3);
      break;
    case ModifierMemberValue_kv3_comparison_ANY:
      return ((uopnd2 & uopnd3) != 0);
      break;
    case ModifierMemberValue_kv3_comparison_NONE:
      return !((uopnd2 & uopnd3) != 0);
      break;
    default:
      Except_THROW("Modifier value %d not managed\n", value);
  }
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_comparison_8(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  int value = (int)Int256_toInt64(opnd1);
  int64_t sopnd2 = (int8_t)Int256_toInt64(opnd2);
  int64_t sopnd3 = (int8_t)Int256_toInt64(opnd3);
  uint64_t uopnd2 = (uint8_t)Int256_toUInt64(opnd2);
  uint64_t uopnd3 = (uint8_t)Int256_toUInt64(opnd3);
  return Behavior_comparison_(value, sopnd2, sopnd3, uopnd2, uopnd3);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_comparison_16(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  int value = (int)Int256_toInt64(opnd1);
  int64_t sopnd2 = (int16_t)Int256_toInt64(opnd2);
  int64_t sopnd3 = (int16_t)Int256_toInt64(opnd3);
  uint64_t uopnd2 = (uint16_t)Int256_toUInt64(opnd2);
  uint64_t uopnd3 = (uint16_t)Int256_toUInt64(opnd3);
  return Behavior_comparison_(value, sopnd2, sopnd3, uopnd2, uopnd3);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_comparison_32(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  int value = (int)Int256_toInt64(opnd1);
  int64_t sopnd2 = (int32_t)Int256_toInt64(opnd2);
  int64_t sopnd3 = (int32_t)Int256_toInt64(opnd3);
  uint64_t uopnd2 = (uint32_t)Int256_toUInt64(opnd2);
  uint64_t uopnd3 = (uint32_t)Int256_toUInt64(opnd3);
  return Behavior_comparison_(value, sopnd2, sopnd3, uopnd2, uopnd3);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_comparison_64(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  int value = (int)Int256_toInt64(opnd1);
  int64_t sopnd2 = (int64_t)Int256_toInt64(opnd2);
  int64_t sopnd3 = (int64_t)Int256_toInt64(opnd3);
  uint64_t uopnd2 = (uint64_t)Int256_toUInt64(opnd2);
  uint64_t uopnd3 = (uint64_t)Int256_toUInt64(opnd3);
  return Behavior_comparison_(value, sopnd2, sopnd3, uopnd2, uopnd3);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_scalarcond(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int condition = Int256_toInt64(opnd1);
  int64_t operand = Int256_toInt64(opnd2);
  switch(condition) {
    case ModifierMemberValue_kv3_scalarcond_DNEZ:
      return (int64_t)operand != 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_DEQZ:
      return (int64_t)operand == 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_DLTZ:
      return (int64_t)operand < 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_DGEZ:
      return (int64_t)operand >= 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_DLEZ:
      return (int64_t)operand <= 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_DGTZ:
      return (int64_t)operand > 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_EVEN:
      return !(operand & 1);
      break;
    case ModifierMemberValue_kv3_scalarcond_ODD:
      return operand & 1;
      break;
    case ModifierMemberValue_kv3_scalarcond_WNEZ:
      return (int32_t)operand != 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_WEQZ:
      return (int32_t)operand == 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_WLTZ:
      return (int32_t)operand < 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_WGEZ:
      return (int32_t)operand >= 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_WLEZ:
      return (int32_t)operand <= 0;
      break;
    case ModifierMemberValue_kv3_scalarcond_WGTZ:
      return (int32_t)operand > 0;
      break;
    default:
      Except_THROW("Modifier value %d not managed\n", condition);
  }
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_simplecond_(void *this, int condition, int64_t operand)
{
  switch(condition) {
    case ModifierMemberValue_kv3_simplecond_NEZ:
      return operand != 0;
      break;
    case ModifierMemberValue_kv3_simplecond_EQZ:
      return operand == 0;
      break;
    case ModifierMemberValue_kv3_simplecond_LTZ:
      return operand < 0;
      break;
    case ModifierMemberValue_kv3_simplecond_GEZ:
      return operand >= 0;
      break;
    case ModifierMemberValue_kv3_simplecond_LEZ:
      return operand <= 0;
      break;
    case ModifierMemberValue_kv3_simplecond_GTZ:
      return operand > 0;
      break;
    case ModifierMemberValue_kv3_simplecond_EVEN:
      return !(operand & 1);
      break;
    case ModifierMemberValue_kv3_simplecond_ODD:
      return operand & 1;
      break;
    default:
      Except_THROW("Modifier value %d not managed\n", condition);
  }
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_simplecond_8(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_simplecond_(this, (int)Int256_toInt64(opnd1), (int8_t)Int256_toInt64(opnd2));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_simplecond_16(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_simplecond_(this, (int)Int256_toInt64(opnd1), (int16_t)Int256_toInt64(opnd2));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_simplecond_32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_simplecond_(this, (int)Int256_toInt64(opnd1), (int32_t)Int256_toInt64(opnd2));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_simplecond_64(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_simplecond_(this, (int)Int256_toInt64(opnd1), (int64_t)Int256_toInt64(opnd2));
}
#endif//$XCC__h



#ifdef $XCC__h
static inline void
flip_set_rounding_mode(unsigned value)
{
  Flip_rnd_mode_t roundingMode = FLIP_RN;
  //
  switch(value) {
    case ModifierMemberValue_kv3_rounding_RZ:
      roundingMode = FLIP_RZ;
      break;
    case ModifierMemberValue_kv3_rounding_RU:
      roundingMode = FLIP_RU;
      break;
    case ModifierMemberValue_kv3_rounding_RD:
      roundingMode = FLIP_RD;
      break;
    case ModifierMemberValue_kv3_rounding_RN:
      roundingMode = FLIP_RN;
      break;
    default:
      Except_THROW("Modifier value %d not managed\n", (int)value);
  }
  //
  flip_set_rnd_mode(roundingMode);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
softfloat_set_rounding_mode(unsigned value)
{
  switch(value) {
    case ModifierMemberValue_kv3_rounding_RZ:
      softfloat_roundingMode = softfloat_round_minMag;
      break;
    case ModifierMemberValue_kv3_rounding_RU:
      softfloat_roundingMode = softfloat_round_max;
      break;
    case ModifierMemberValue_kv3_rounding_RD:
      softfloat_roundingMode = softfloat_round_min;
      break;
    case ModifierMemberValue_kv3_rounding_RN:
      softfloat_roundingMode = softfloat_round_near_even;
      break;
    default:
      Except_THROW("Modifier value %d not managed\n", (int)value);
  }
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline void
reset_exceptions(void *this)
{
  flip_reset_exceptions();
  softfloat_exceptionFlags = 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
set_rounding_mode(void *this, Int256_ rounding_mode)
{
  uint32_t mode = Int256_toUInt32(rounding_mode);
  flip_set_rounding_mode(mode);
  softfloat_set_rounding_mode(mode);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fixed_32(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip32 fopnd2 = Int256_toUInt64(opnd2);
  uint32_t scaling = Int256_toUInt32(opnd3);
  uint32_t result = Flip32_fixed(fopnd2, scaling);
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fixed_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip32 fopnd2_l = Int256_toUInt32(opnd2);
  flip32 fopnd2_m = Int256_toUInt64(opnd2) >> 32;
  uint32_t scaling = Int256_toUInt32(opnd3);
  uint32_t result_l = Flip32_fixed(fopnd2_l, scaling);
  uint32_t result_m = Flip32_fixed(fopnd2_m, scaling);
  return Int256_fromUInt64(((uint64_t)result_m << 32) | result_l);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fixed_16_x4(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  return Int256_zero; // FIXME!
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fixed_64(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip64 fopnd2 = Int256_toUInt64(opnd2);
  uint32_t scaling = Int256_toUInt32(opnd3);
  int64_t result = Flip64_fixed(fopnd2, scaling);
  return Int256_fromInt64(result);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fixedu_32(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip32 fopnd2 = Int256_toUInt64(opnd2);
  uint32_t scaling = Int256_toUInt32(opnd3);
  uint32_t result = Flip32_fixedu(fopnd2, scaling);
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fixedu_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip32 fopnd2_l = Int256_toUInt32(opnd2);
  flip32 fopnd2_m = Int256_toUInt64(opnd2) >> 32;
  uint32_t scaling = Int256_toUInt32(opnd3);
  uint32_t result_l = Flip32_fixedu(fopnd2_l, scaling);
  uint32_t result_m = Flip32_fixedu(fopnd2_m, scaling);
  return Int256_fromUInt64(((uint64_t)result_m << 32) | result_l);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fixedu_16_x4(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  return Int256_zero; // FIXME!
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fixedu_64(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  set_rounding_mode(this, rounding_mode);
  reset_exceptions(this);
  flip64 fopnd2 = Int256_toUInt64(opnd2);
  uint32_t scaling = Int256_toUInt32(opnd3);
  uint64_t result = Flip64_fixedu(fopnd2, scaling);
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_float_32(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t scaling = Int256_toUInt32(opnd3);
  flip32 result = Flip32_float(Int256_toInt32(opnd2), scaling);
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_float_32_(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words2 = opnd2.words;
  uint32_t scaling = opnd3.words[0];
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = Flip32_float(words2[i], scaling);
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_float_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_float_32_(this, rounding_mode, opnd2, opnd3, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_float_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_float_32_(this, rounding_mode, opnd2, opnd3, 8);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_float_64(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t scaling = Int256_toUInt32(opnd3);
  flip64 result = Flip64_float(Int256_toInt64(opnd2), scaling);
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_floatu_32(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t scaling = Int256_toUInt32(opnd3);
  flip32 result = Flip32_floatu(Int256_toUInt32(opnd2), scaling);
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_floatu_32_(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words2 = opnd2.words;
  uint32_t scaling = opnd3.words[0];
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = Flip32_floatu(words2[i], scaling);
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_floatu_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_floatu_32_(this, rounding_mode, opnd2, opnd3, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_floatu_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_floatu_32_(this, rounding_mode, opnd2, opnd3, 8);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_floatu_64(void *this, Int256_ rounding_mode, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t scaling = Int256_toUInt32(opnd3);
  flip64 result = Flip64_floatu(Int256_toInt64(opnd2), scaling);
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline bool
flip_floatcomp_helper(void *this, int64_t value, Flip_compare_t comp)
{
  bool result = false;
  //
  switch(value) {
    case ModifierMemberValue_kv3_floatcomp_ONE:
      result = ((comp != Unordered) && (comp != Equal));
      break;
    case ModifierMemberValue_kv3_floatcomp_UEQ:
      result = ((comp == Unordered) || (comp == Equal));
      break;
    case ModifierMemberValue_kv3_floatcomp_OEQ:
      result = (comp == Equal);
      break;
    case ModifierMemberValue_kv3_floatcomp_UNE:
      result = ((comp == Unordered) || (comp != Equal));
      break;
    case ModifierMemberValue_kv3_floatcomp_OLT:
      result = (comp == LesserThan);
      break;
    case ModifierMemberValue_kv3_floatcomp_UGE:
      result = ((comp == Unordered) || (comp == GreaterThan) || (comp == Equal));
      break;
    case ModifierMemberValue_kv3_floatcomp_OGE:
      result = ((comp == GreaterThan) || (comp == Equal));
      break;
    case ModifierMemberValue_kv3_floatcomp_ULT:
      result = ((comp == Unordered) || (comp == LesserThan));
      break;
    default:
      Except_THROW("Modifier value %d not managed\n", (int)value);
  }
  //
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_floatcomp_16(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  flip16 fopnd2;
  flip16 fopnd3;
  fopnd2 = Int256_toUInt64(opnd2);
  fopnd3 = Int256_toUInt64(opnd3);
  Flip_compare_t comp = Flip16_comp(fopnd2, fopnd3);
  return flip_floatcomp_helper(this, Int256_toUInt64(opnd1), comp);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_floatcomp_32(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  flip32 fopnd2;
  flip32 fopnd3;
  fopnd2 = Int256_toUInt64(opnd2);
  fopnd3 = Int256_toUInt64(opnd3);
  Flip_compare_t comp = Flip32_comp(fopnd2, fopnd3);
  return flip_floatcomp_helper(this, Int256_toUInt64(opnd1), comp);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_floatcomp_64(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  flip64 fopnd2 = Int256_toUInt64(opnd2);
  flip64 fopnd3 = Int256_toUInt64(opnd3);
  Flip_compare_t comp = Flip64_comp(fopnd2, fopnd3);
  return flip_floatcomp_helper(this, Int256_toUInt64(opnd1), comp);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_16_16(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float16_t fopnd1 = { Int256_toUInt32(opnd1) };
  float16_t fopnd2 = { Int256_toUInt32(opnd2) };
  float16_t fopnd3 = { Int256_toUInt32(opnd3) };
  float16_t result = f16_mulAdd(fopnd3, fopnd2, fopnd1);
  result = fixupNaN_f16_f16_f16_f16(result, fopnd1, fopnd2, fopnd3);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1 = { Int256_toUInt32(opnd1) };
  float32_t fopnd2 = { Int256_toUInt32(opnd2) };
  float32_t fopnd3 = { Int256_toUInt32(opnd3) };
  float32_t result = f32_mulAdd(fopnd3, fopnd2, fopnd1);
  result = fixupNaN_f32_f32_f32_f32(result, fopnd1, fopnd2, fopnd3);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_32_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1 = { Int256_toUInt64(opnd1) };
  float32_t fopnd2 = { Int256_toUInt32(opnd2) };
  float32_t fopnd3 = { Int256_toUInt32(opnd3) };
  float64_t result = f64_mulAdd(f32_to_f64(fopnd3), f32_to_f64(fopnd2), fopnd1);
  result = fixupNaN_f64_f64_f32_f32(result, fopnd1, fopnd2, fopnd3);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_64_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1 = { Int256_toUInt64(opnd1) };
  float64_t fopnd2 = { Int256_toUInt64(opnd2) };
  float64_t fopnd3 = { Int256_toUInt64(opnd3) };
  float64_t result = f64_mulAdd(fopnd3, fopnd2, fopnd1);
  result = fixupNaN_f64_f64_f64_f64(result, fopnd1, fopnd2, fopnd3);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words1 = opnd1.words;
  uint32_t *words2 = opnd2.words;
  uint32_t *words3 = opnd3.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float32_t fopnd2 = { words2[i] };
    float32_t fopnd3 = { words3[i] };
    float32_t fresult = f32_mulAdd(fopnd3, fopnd2, fopnd1);
    fresult = fixupNaN_f32_f32_f32_f32(fresult, fopnd1, fopnd2, fopnd3);
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffma_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffma_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_32_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffma_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 8);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_32_64_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1_l = { Int256_dwords_0(opnd1) };
  float64_t fopnd1_m = { Int256_dwords_1(opnd1) };
  float32_t fopnd2_l = { Int256_toUInt64(opnd2) };
  float32_t fopnd2_m = { Int256_toUInt64(opnd2)>>32 };
  float32_t fopnd3_l = { Int256_toUInt64(opnd3) };
  float32_t fopnd3_m = { Int256_toUInt64(opnd3)>>32 };
  float64_t result_l = f64_mulAdd(f32_to_f64(fopnd3_l), f32_to_f64(fopnd2_l), fopnd1_l);
  float64_t result_m = f64_mulAdd(f32_to_f64(fopnd3_m), f32_to_f64(fopnd2_m), fopnd1_m);
  result_l = fixupNaN_f64_f64_f32_f32(result_l, fopnd1_l, fopnd2_l, fopnd3_l);
  result_m = fixupNaN_f64_f64_f32_f32(result_m, fopnd1_m, fopnd2_m, fopnd3_m);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_l.v;
  Int256_dwords_1(result) = result_m.v;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_16_16_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint16_t *hwords1 = opnd1.hwords;
  uint16_t *hwords2 = opnd2.hwords;
  uint16_t *hwords3 = opnd3.hwords;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float16_t fopnd1 = { hwords1[i] };
    float16_t fopnd2 = { hwords2[i] };
    float16_t fopnd3 = { hwords3[i] };
    float16_t fresult = f16_mulAdd(fopnd3, fopnd2, fopnd1);
    fresult = fixupNaN_f16_f16_f16_f16(fresult, fopnd1, fopnd2, fopnd3);
    result.hwords[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_16_16_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffma_16_16_(this, rounding_mode, opnd1, opnd2, opnd3, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_16_16_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffma_16_16_(this, rounding_mode, opnd1, opnd2, opnd3, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_16_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words1 = opnd1.words;
  uint16_t *hwords2 = opnd2.hwords;
  uint16_t *hwords3 = opnd3.hwords;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float16_t fopnd2 = { hwords2[i] };
    float16_t fopnd3 = { hwords3[i] };
    float32_t fresult = f32_mulAdd(f16_to_f32(fopnd3), f16_to_f32(fopnd2), fopnd1);
    fresult = fixupNaN_f32_f32_f16_f16(fresult, fopnd1, fopnd2, fopnd3);
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_16_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffma_16_32_(this, rounding_mode, opnd1, opnd2, opnd3, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_16_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffma_16_32_(this, rounding_mode, opnd1, opnd2, opnd3, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffma_16_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffma_16_32_(this, rounding_mode, opnd1, opnd2, opnd3, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_16_16(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float16_t fopnd1 = { Int256_toUInt32(opnd1) };
  float16_t fopnd2 = { Int256_toUInt32(opnd2) };
  float16_t fopnd3 = { Int256_toUInt32(opnd3) };
  float16_t result = f16_mulSub(fopnd3, fopnd2, fopnd1);
  result = fixupNaN_f16_f16_f16_f16(result, fopnd1, fopnd2, fopnd3);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1 = { Int256_toUInt32(opnd1) };
  float32_t fopnd2 = { Int256_toUInt32(opnd2) };
  float32_t fopnd3 = { Int256_toUInt32(opnd3) };
  float32_t result = f32_mulSub(fopnd3, fopnd2, fopnd1);
  result = fixupNaN_f32_f32_f32_f32(result, fopnd1, fopnd2, fopnd3);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_32_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1 = { Int256_toUInt64(opnd1) };
  float32_t fopnd2 = { Int256_toUInt32(opnd2) };
  float32_t fopnd3 = { Int256_toUInt32(opnd3) };
  float64_t result = f64_mulSub(f32_to_f64(fopnd3), f32_to_f64(fopnd2), fopnd1);
  result = fixupNaN_f64_f64_f32_f32(result, fopnd1, fopnd2, fopnd3);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_64_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1 = { Int256_toUInt64(opnd1) };
  float64_t fopnd2 = { Int256_toUInt64(opnd2) };
  float64_t fopnd3 = { Int256_toUInt64(opnd3) };
  float64_t result = f64_mulSub(fopnd3, fopnd2, fopnd1);
  result = fixupNaN_f64_f64_f64_f64(result, fopnd1, fopnd2, fopnd3);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words1 = opnd1.words;
  uint32_t *words2 = opnd2.words;
  uint32_t *words3 = opnd3.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float32_t fopnd2 = { words2[i] };
    float32_t fopnd3 = { words3[i] };
    float32_t fresult = f32_mulSub(fopnd3, fopnd2, fopnd1);
    fresult = fixupNaN_f32_f32_f32_f32(fresult, fopnd1, fopnd2, fopnd3);
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffms_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffms_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_32_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffms_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 8);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_32_64_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1_l = { Int256_dwords_0(opnd1) };
  float64_t fopnd1_m = { Int256_dwords_1(opnd1) };
  float32_t fopnd2_l = { Int256_toUInt64(opnd2) };
  float32_t fopnd2_m = { Int256_toUInt64(opnd2)>>32 };
  float32_t fopnd3_l = { Int256_toUInt64(opnd3) };
  float32_t fopnd3_m = { Int256_toUInt64(opnd3)>>32 };
  float64_t result_l = f64_mulSub(f32_to_f64(fopnd3_l), f32_to_f64(fopnd2_l), fopnd1_l);
  float64_t result_m = f64_mulSub(f32_to_f64(fopnd3_m), f32_to_f64(fopnd2_m), fopnd1_m);
  result_l = fixupNaN_f64_f64_f32_f32(result_l, fopnd1_l, fopnd2_l, fopnd3_l);
  result_m = fixupNaN_f64_f64_f32_f32(result_m, fopnd1_m, fopnd2_m, fopnd3_m);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_l.v;
  Int256_dwords_1(result) = result_m.v;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_16_16_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint16_t *hwords1 = opnd1.hwords;
  uint16_t *hwords2 = opnd2.hwords;
  uint16_t *hwords3 = opnd3.hwords;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float16_t fopnd1 = { hwords1[i] };
    float16_t fopnd2 = { hwords2[i] };
    float16_t fopnd3 = { hwords3[i] };
    float16_t fresult = f16_mulSub(fopnd3, fopnd2, fopnd1);
    fresult = fixupNaN_f16_f16_f16_f16(fresult, fopnd1, fopnd2, fopnd3);
    result.hwords[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_16_16_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffms_16_16_(this, rounding_mode, opnd1, opnd2, opnd3, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_16_16_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffms_16_16_(this, rounding_mode, opnd1, opnd2, opnd3, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_16_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words1 = opnd1.words;
  uint16_t *hwords2 = opnd2.hwords;
  uint16_t *hwords3 = opnd3.hwords;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float16_t fopnd2 = { hwords2[i] };
    float16_t fopnd3 = { hwords3[i] };
    float32_t fresult = f32_mulSub(f16_to_f32(fopnd3), f16_to_f32(fopnd2), fopnd1);
    fresult = fixupNaN_f32_f32_f16_f16(fresult, fopnd1, fopnd2, fopnd3);
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_16_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffms_16_32_(this, rounding_mode, opnd1, opnd2, opnd3, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_16_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffms_16_32_(this, rounding_mode, opnd1, opnd2, opnd3, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffms_16_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffms_16_32_(this, rounding_mode, opnd1, opnd2, opnd3, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_16_16(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float16_t fopnd1 = { Int256_toUInt32(opnd1) };
  float16_t fopnd2 = { Int256_toUInt32(opnd2) };
  float16_t result = f16_mul(fopnd1, fopnd2);
  result = fixupNaN_f16_f16_f16(result, fopnd1, fopnd2);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_16_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float16_t fopnd1 = { Int256_toUInt32(opnd1) };
  float16_t fopnd2 = { Int256_toUInt32(opnd2) };
  float32_t result = f32_mul(f16_to_f32(fopnd1), f16_to_f32(fopnd2));
  result = fixupNaN_f32_f16_f16(result, fopnd1, fopnd2);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1 = { Int256_toUInt32(opnd1) };
  float32_t fopnd2 = { Int256_toUInt32(opnd2) };
  float32_t result = f32_mul(fopnd1, fopnd2);
  result = fixupNaN_f32_f32_f32(result, fopnd1, fopnd2);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_32_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1 = { Int256_toUInt32(opnd1) };
  float32_t fopnd2 = { Int256_toUInt32(opnd2) };
  float64_t result = f64_mul(f32_to_f64(fopnd1), f32_to_f64(fopnd2));
  result = fixupNaN_f64_f32_f32(result, fopnd1, fopnd2);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_64_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1 = { Int256_toUInt64(opnd1) };
  float64_t fopnd2 = { Int256_toUInt64(opnd2) };
  float64_t result = f64_mul(fopnd1, fopnd2);
  result = fixupNaN_f64_f64_f64(result, fopnd1, fopnd2);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words1 = opnd1.words;
  uint32_t *words2 = opnd2.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float32_t fopnd2 = { words2[i] };
    float32_t fresult = f32_mul(fopnd1, fopnd2);
    fresult = fixupNaN_f32_f32_f32(fresult, fopnd1, fopnd2);
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmul_32_32_(this, rounding_mode, opnd1, opnd2, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmul_32_32_(this, rounding_mode, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_32_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmul_32_32_(this, rounding_mode, opnd1, opnd2, 8);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_32_64_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1_l = { Int256_toUInt64(opnd1) };
  float32_t fopnd1_m = { Int256_toUInt64(opnd1)>>32 };
  float32_t fopnd2_l = { Int256_toUInt64(opnd2) };
  float32_t fopnd2_m = { Int256_toUInt64(opnd2)>>32 };
  float64_t result_l = f64_mul(f32_to_f64(fopnd1_l), f32_to_f64(fopnd2_l));
  float64_t result_m = f64_mul(f32_to_f64(fopnd1_m), f32_to_f64(fopnd2_m));
  result_l = fixupNaN_f64_f32_f32(result_l, fopnd1_l, fopnd2_l);
  result_m = fixupNaN_f64_f32_f32(result_m, fopnd1_m, fopnd2_m);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_l.v;
  Int256_dwords_1(result) = result_m.v;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_64_64_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1_l = { Int256_dwords_0(opnd1) };
  float64_t fopnd1_m = { Int256_dwords_1(opnd1) };
  float64_t fopnd2_l = { Int256_dwords_0(opnd2) };
  float64_t fopnd2_m = { Int256_dwords_1(opnd2) };
  float64_t result_l = f64_mul(fopnd1_l, fopnd2_l);
  float64_t result_m = f64_mul(fopnd1_m, fopnd2_m);
  result_l = fixupNaN_f64_f64_f64(result_l, fopnd1_l, fopnd2_l);
  result_m = fixupNaN_f64_f64_f64(result_m, fopnd1_m, fopnd2_m);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_l.v;
  Int256_dwords_1(result) = result_m.v;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_16_16_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint16_t *hwords1 = opnd1.hwords;
  uint16_t *hwords2 = opnd2.hwords;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float16_t fopnd1 = { hwords1[i] };
    float16_t fopnd2 = { hwords2[i] };
    float16_t fresult = f16_mul(fopnd1, fopnd2);
    fresult = fixupNaN_f16_f16_f16(fresult, fopnd1, fopnd2);
    result.hwords[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_16_16_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmul_16_16_(this, rounding_mode, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_16_16_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmul_16_16_(this, rounding_mode, opnd1, opnd2, 8);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmul_16_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float16_t fopnd1_x = { Int256_toUInt64(opnd1) };
  float16_t fopnd1_y = { Int256_toUInt64(opnd1)>>16 };
  float16_t fopnd1_z = { Int256_toUInt64(opnd1)>>32 };
  float16_t fopnd1_t = { Int256_toUInt64(opnd1)>>48 };
  float16_t fopnd2_x = { Int256_toUInt64(opnd2) };
  float16_t fopnd2_y = { Int256_toUInt64(opnd2)>>16 };
  float16_t fopnd2_z = { Int256_toUInt64(opnd2)>>32 };
  float16_t fopnd2_t = { Int256_toUInt64(opnd2)>>48 };
  float32_t result_x = f32_mul(f16_to_f32(fopnd1_x), f16_to_f32(fopnd2_x));
  float32_t result_y = f32_mul(f16_to_f32(fopnd1_y), f16_to_f32(fopnd2_y));
  float32_t result_z = f32_mul(f16_to_f32(fopnd1_z), f16_to_f32(fopnd2_z));
  float32_t result_t = f32_mul(f16_to_f32(fopnd1_t), f16_to_f32(fopnd2_t));
  result_x = fixupNaN_f32_f16_f16(result_x, fopnd1_x, fopnd2_x);
  result_y = fixupNaN_f32_f16_f16(result_y, fopnd1_y, fopnd2_y);
  result_z = fixupNaN_f32_f16_f16(result_z, fopnd1_z, fopnd2_z);
  result_t = fixupNaN_f32_f16_f16(result_t, fopnd1_t, fopnd2_t);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = (uint32_t)result_x.v | (uint64_t)result_y.v<<32;
  Int256_dwords_1(result) = (uint32_t)result_z.v | (uint64_t)result_t.v<<32;
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fmulc_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    flip32 fopnd1_r = opnd1.words[2*i+0];
    flip32 fopnd1_i = opnd1.words[2*i+1];
    flip32 fopnd2_r = opnd2.words[2*i+0];
    flip32 fopnd2_i = opnd2.words[2*i+1];
    flip32 result_r = Flip32_fdms(fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
    flip32 result_i = Flip32_fdma(fopnd1_r, fopnd2_i, fopnd1_i, fopnd2_r);
    result_r = fixupNaN_f32_f32_f32_f32_f32_flip(result_r, fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
    result_i = fixupNaN_f32_f32_f32_f32_f32_flip(result_i, fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
    result.words[2*i+0] = result_r;
    result.words[2*i+1] = result_i;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmulc_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmulc_32_32_(this, rounding_mode, opnd1, opnd2, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmulc_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmulc_32_32_(this, rounding_mode, opnd1, opnd2, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmulc_32_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip32 fopnd1_r, fopnd2_r, fopnd2_i, fopnd1_i;
  fopnd1_r = Int256_toUInt64(opnd1) & 0xFFFFFFFF;
  fopnd1_i = Int256_toUInt64(opnd1) >> 32;
  fopnd2_r = Int256_toUInt64(opnd2) & 0xFFFFFFFF;
  fopnd2_i = Int256_toUInt64(opnd2) >> 32;
  flip64 result_r = Flip64_fdms(fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
  flip64 result_i = Flip64_fdma(fopnd1_r, fopnd2_i, fopnd1_i, fopnd2_r);
  result_r = fixupNaN_f64_f32_f32_f32_f32_flip(result_r, fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
  result_i = fixupNaN_f64_f32_f32_f32_f32_flip(result_i, fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_r;
  Int256_dwords_1(result) = result_i;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmulcc_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    flip32 fopnd1_r = opnd1.words[2*i+0];
    flip32 fopnd1_i = opnd1.words[2*i+1];
    flip32 fopnd2_r = opnd2.words[2*i+0];
    flip32 fopnd2_i = opnd2.words[2*i+1];
    flip32 result_r = Flip32_fdma(fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
    flip32 result_i = Flip32_fdms(fopnd1_r, fopnd2_i, fopnd1_i, fopnd2_r);
    result_r = fixupNaN_f32_f32_f32_f32_f32_flip(result_r, fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
    result_i = fixupNaN_f32_f32_f32_f32_f32_flip(result_i, fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
    result.words[2*i+0] = result_r;
    result.words[2*i+1] = result_i;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmulcc_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmulcc_32_32_(this, rounding_mode, opnd1, opnd2, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmulcc_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmulcc_32_32_(this, rounding_mode, opnd1, opnd2, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmulcc_32_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip32 fopnd1_r, fopnd2_r, fopnd2_i, fopnd1_i;
  fopnd1_r = Int256_toUInt64(opnd1) & 0xFFFFFFFF;
  fopnd1_i = Int256_toUInt64(opnd1) >> 32;
  fopnd2_r = Int256_toUInt64(opnd2) & 0xFFFFFFFF;
  fopnd2_i = Int256_toUInt64(opnd2) >> 32;
  flip64 result_r = Flip64_fdma(fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
  flip64 result_i = Flip64_fdms(fopnd1_r, fopnd2_i, fopnd1_i, fopnd2_r);
  result_r = fixupNaN_f64_f32_f32_f32_f32_flip(result_r, fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
  result_i = fixupNaN_f64_f32_f32_f32_f32_flip(result_i, fopnd1_r, fopnd2_r, fopnd1_i, fopnd2_i);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_r;
  Int256_dwords_1(result) = result_i;
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fdot2_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip32 fopnd1, fopnd2, fopnd3, fopnd4;
  fopnd1 = Int256_toUInt64(opnd1) & 0xFFFFFFFF;
  fopnd2 = Int256_toUInt64(opnd1) >> 32;
  fopnd3 = Int256_toUInt64(opnd2) & 0xFFFFFFFF;
  fopnd4 = Int256_toUInt64(opnd2) >> 32;
  flip32 result = Flip32_fdma(fopnd1, fopnd3, fopnd2, fopnd4);
  result = fixupNaN_f32_f32_f32_f32_f32_flip(result, fopnd1, fopnd3, fopnd2, fopnd4);
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fdot2_32_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip32 fopnd1, fopnd2, fopnd3, fopnd4;
  fopnd1 = Int256_toUInt64(opnd1) & 0xFFFFFFFF;
  fopnd2 = Int256_toUInt64(opnd1) >> 32;
  fopnd3 = Int256_toUInt64(opnd2) & 0xFFFFFFFF;
  fopnd4 = Int256_toUInt64(opnd2) >> 32;
  flip64 result = Flip64_fdma(fopnd1, fopnd3, fopnd2, fopnd4);
  result = fixupNaN_f64_f32_f32_f32_f32_flip(result, fopnd1, fopnd3, fopnd2, fopnd4);
  return Int256_fromUInt64(result);
}
#endif//$XCC__h


#ifdef $XCC__h
static inline Int256_
Behavior_fdot2_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip32 fopnd1_x = { Int256_dwords_0(opnd1) };
  flip32 fopnd1_y = { Int256_dwords_0(opnd1)>>32 };
  flip32 fopnd1_z = { Int256_dwords_1(opnd1) };
  flip32 fopnd1_t = { Int256_dwords_1(opnd1)>>32 };
  flip32 fopnd2_x = { Int256_dwords_0(opnd2) };
  flip32 fopnd2_y = { Int256_dwords_0(opnd2)>>32 };
  flip32 fopnd2_z = { Int256_dwords_1(opnd2) };
  flip32 fopnd2_t = { Int256_dwords_1(opnd2)>>32 };
  flip32 result_l = Flip32_fdma(fopnd1_x, fopnd2_x, fopnd1_y, fopnd2_y);
  flip32 result_m = Flip32_fdma(fopnd1_z, fopnd2_z, fopnd1_t, fopnd2_t);
  result_l = fixupNaN_f32_f32_f32_f32_f32_flip(result_l, fopnd1_x, fopnd2_x, fopnd1_y, fopnd2_y);
  result_m = fixupNaN_f32_f32_f32_f32_f32_flip(result_m, fopnd1_z, fopnd2_z, fopnd1_t, fopnd2_t);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_l;
  Int256_dwords_1(result) = result_m;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fdot2_32_64_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  flip32 fopnd1_x = { Int256_dwords_0(opnd1) };
  flip32 fopnd1_y = { Int256_dwords_0(opnd1)>>32 };
  flip32 fopnd1_z = { Int256_dwords_1(opnd1) };
  flip32 fopnd1_t = { Int256_dwords_1(opnd1)>>32 };
  flip32 fopnd2_x = { Int256_dwords_0(opnd2) };
  flip32 fopnd2_y = { Int256_dwords_0(opnd2)>>32 };
  flip32 fopnd2_z = { Int256_dwords_1(opnd2) };
  flip32 fopnd2_t = { Int256_dwords_1(opnd2)>>32 };
  flip64 result_l = Flip64_fdma(fopnd1_x, fopnd2_x, fopnd1_y, fopnd2_y);
  flip64 result_m = Flip64_fdma(fopnd1_z, fopnd2_z, fopnd1_t, fopnd2_t);
  result_l = fixupNaN_f64_f32_f32_f32_f32_flip(result_l, fopnd1_x, fopnd2_x, fopnd1_y, fopnd2_y);
  result_m = fixupNaN_f64_f32_f32_f32_f32_flip(result_m, fopnd1_z, fopnd2_z, fopnd1_t, fopnd2_t);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_l;
  Int256_dwords_1(result) = result_m;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmm2wq(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1_x = { Int256_dwords_0(opnd1) };
  float32_t fopnd1_y = { Int256_dwords_0(opnd1)>>32 };
  float32_t fopnd2_x = { Int256_dwords_0(opnd2) };
  float32_t fopnd2_y = { Int256_dwords_0(opnd2)>>32 };
  float32_t result_x = f32_mul(fopnd1_x, fopnd2_x);
  float32_t result_y = f32_mul(fopnd1_x, fopnd2_y);
  float32_t result_z = f32_mul(fopnd1_y, fopnd2_x);
  float32_t result_t = f32_mul(fopnd1_y, fopnd2_y);
  result_x = fixupNaN_f32_f32_f32(result_x, fopnd1_x, fopnd2_x);
  result_y = fixupNaN_f32_f32_f32(result_y, fopnd1_x, fopnd2_y);
  result_z = fixupNaN_f32_f32_f32(result_z, fopnd1_y, fopnd2_x);
  result_t = fixupNaN_f32_f32_f32(result_t, fopnd1_y, fopnd2_y);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = (uint32_t)result_x.v | (uint64_t)result_y.v<<32;
  Int256_dwords_1(result) = (uint32_t)result_z.v | (uint64_t)result_t.v<<32;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmm2awq(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1_x = { Int256_dwords_0(opnd1) };
  float32_t fopnd1_y = { Int256_dwords_0(opnd1)>>32 };
  float32_t fopnd1_z = { Int256_dwords_1(opnd1) };
  float32_t fopnd1_t = { Int256_dwords_1(opnd1)>>32 };
  float32_t fopnd2_x = { Int256_dwords_0(opnd2) };
  float32_t fopnd2_y = { Int256_dwords_0(opnd2)>>32 };
  float32_t fopnd3_x = { Int256_dwords_0(opnd3) };
  float32_t fopnd3_y = { Int256_dwords_0(opnd3)>>32 };
  float32_t result_x = f32_mulAdd(fopnd3_x, fopnd2_x, fopnd1_x);
  float32_t result_y = f32_mulAdd(fopnd3_y, fopnd2_x, fopnd1_y);
  float32_t result_z = f32_mulAdd(fopnd3_x, fopnd2_y, fopnd1_z);
  float32_t result_t = f32_mulAdd(fopnd3_y, fopnd2_y, fopnd1_t);
  result_x = fixupNaN_f32_f32_f32_f32(result_x, fopnd3_x, fopnd2_x, fopnd1_x);
  result_y = fixupNaN_f32_f32_f32_f32(result_y, fopnd3_y, fopnd2_x, fopnd1_y); 
  result_z = fixupNaN_f32_f32_f32_f32(result_z, fopnd3_x, fopnd2_y, fopnd1_z);
  result_t = fixupNaN_f32_f32_f32_f32(result_t, fopnd3_y, fopnd2_y, fopnd1_t);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = (uint32_t)result_x.v | (uint64_t)result_y.v<<32;
  Int256_dwords_1(result) = (uint32_t)result_z.v | (uint64_t)result_t.v<<32;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmm2swq(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1_x = { Int256_dwords_0(opnd1) };
  float32_t fopnd1_y = { Int256_dwords_0(opnd1)>>32 };
  float32_t fopnd1_z = { Int256_dwords_1(opnd1) };
  float32_t fopnd1_t = { Int256_dwords_1(opnd1)>>32 };
  float32_t fopnd2_x = { Int256_dwords_0(opnd2) };
  float32_t fopnd2_y = { Int256_dwords_0(opnd2)>>32 };
  float32_t fopnd3_x = { Int256_dwords_0(opnd3) };
  float32_t fopnd3_y = { Int256_dwords_0(opnd3)>>32 };
  float32_t result_x = f32_mulSub(fopnd3_x, fopnd2_x, fopnd1_x);
  float32_t result_y = f32_mulSub(fopnd3_y, fopnd2_x, fopnd1_y);
  float32_t result_z = f32_mulSub(fopnd3_x, fopnd2_y, fopnd1_z);
  float32_t result_t = f32_mulSub(fopnd3_y, fopnd2_y, fopnd1_t);
  result_x = fixupNaN_f32_f32_f32_f32(result_x, fopnd3_x, fopnd2_x, fopnd1_x);
  result_y = fixupNaN_f32_f32_f32_f32(result_y, fopnd3_y, fopnd2_x, fopnd1_y); 
  result_z = fixupNaN_f32_f32_f32_f32(result_z, fopnd3_x, fopnd2_y, fopnd1_z);
  result_t = fixupNaN_f32_f32_f32_f32(result_t, fopnd3_y, fopnd2_y, fopnd1_t);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = (uint32_t)result_x.v | (uint64_t)result_y.v<<32;
  Int256_dwords_1(result) = (uint32_t)result_z.v | (uint64_t)result_t.v<<32;
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_16_16(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float16_t fopnd1 = { Int256_toUInt32(opnd1) };
  float16_t fopnd2 = { Int256_toUInt32(opnd2) };
  float16_t result = f16_add(fopnd1, fopnd2);
  result = fixupNaN_f16_f16_f16(result, fopnd1, fopnd2);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1 = { Int256_toUInt32(opnd1) };
  float32_t fopnd2 = { Int256_toUInt32(opnd2) };
  float32_t result = f32_add(fopnd1, fopnd2);
  result = fixupNaN_f32_f32_f32(result, fopnd1, fopnd2);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_64_64(void *this, Int256_ rounding_mode,Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1 = { Int256_toUInt64(opnd1) };
  float64_t fopnd2 = { Int256_toUInt64(opnd2) };
  float64_t result = f64_add(fopnd1, fopnd2);
  result = fixupNaN_f64_f64_f64(result, fopnd1, fopnd2);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words1 = opnd1.words;
  uint32_t *words2 = opnd2.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float32_t fopnd2 = { words2[i] };
    float32_t fresult = f32_add(fopnd2, fopnd1);
    fresult = fixupNaN_f32_f32_f32(fresult, fopnd1, fopnd2);
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fadd_32_32_(this, rounding_mode, opnd1, opnd2, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fadd_32_32_(this, rounding_mode, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_32_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fadd_32_32_(this, rounding_mode, opnd1, opnd2, 8);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_64_64_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1_l = { Int256_dwords_0(opnd1) };
  float64_t fopnd1_m = { Int256_dwords_1(opnd1) };
  float64_t fopnd2_l = { Int256_dwords_0(opnd2) };
  float64_t fopnd2_m = { Int256_dwords_1(opnd2) };
  float64_t result_l = f64_add(fopnd1_l, fopnd2_l);
  float64_t result_m = f64_add(fopnd1_m, fopnd2_m);
  result_l = fixupNaN_f64_f64_f64(result_l, fopnd1_l, fopnd2_l);
  result_m = fixupNaN_f64_f64_f64(result_m, fopnd1_m, fopnd2_m);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_l.v;
  Int256_dwords_1(result) = result_m.v;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_16_16_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint16_t *hwords1 = opnd1.hwords;
  uint16_t *hwords2 = opnd2.hwords;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float16_t fopnd1 = { hwords1[i] };
    float16_t fopnd2 = { hwords2[i] };
    float16_t fresult = f16_add(fopnd1, fopnd2);
    fresult = fixupNaN_f16_f16_f16(fresult, fopnd1, fopnd2);
    result.hwords[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_16_16_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fadd_16_16_(this, rounding_mode, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fadd_16_16_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fadd_16_16_(this, rounding_mode, opnd1, opnd2, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_faddcc_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1_r = { Int256_toUInt32(opnd1) };
  float32_t fopnd1_i = { Int256_toUInt64(opnd1) >> 32 };
  float32_t fopnd2_r = { Int256_toUInt32(opnd2) };
  float32_t fopnd2_i = { Int256_toUInt64(opnd2) >> 32 };
  float32_t result_r = f32_add(fopnd1_r, fopnd2_r);
  float32_t result_i = f32_sub(fopnd2_i, fopnd1_i);
  result_r = fixupNaN_f32_f32_f32(result_r, fopnd1_r, fopnd2_r);
  result_i = fixupNaN_f32_f32_f32(result_i, fopnd1_i, fopnd2_i);
  return Int256_fromUInt64(((uint64_t)result_i.v << 32) | result_r.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_faddcc_64_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1_r = { Int256_dwords_0(opnd1) };
  float64_t fopnd1_i = { Int256_dwords_1(opnd1) };
  float64_t fopnd2_r = { Int256_dwords_0(opnd2) };
  float64_t fopnd2_i = { Int256_dwords_1(opnd2) };
  float64_t result_r = f64_add(fopnd1_r, fopnd2_r);
  float64_t result_i = f64_sub(fopnd2_i, fopnd1_i);
  result_r = fixupNaN_f64_f64_f64(result_r, fopnd1_r, fopnd2_r);
  result_i = fixupNaN_f64_f64_f64(result_i, fopnd1_i, fopnd2_i);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_r.v;
  Int256_dwords_1(result) = result_i.v;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_faddcc_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1_l_r = { Int256_dwords_0(opnd1) };
  float32_t fopnd1_l_i = { Int256_dwords_0(opnd1)>>32 };
  float32_t fopnd1_m_r = { Int256_dwords_1(opnd1) };
  float32_t fopnd1_m_i = { Int256_dwords_1(opnd1)>>32 };
  float32_t fopnd2_l_r = { Int256_dwords_0(opnd2) };
  float32_t fopnd2_l_i = { Int256_dwords_0(opnd2)>>32 };
  float32_t fopnd2_m_r = { Int256_dwords_1(opnd2) };
  float32_t fopnd2_m_i = { Int256_dwords_1(opnd2)>>32 };
  float32_t result_l_r = f32_add(fopnd1_l_r, fopnd2_l_r);
  float32_t result_l_i = f32_sub(fopnd2_l_i, fopnd1_l_i);
  float32_t result_m_r = f32_add(fopnd1_m_r, fopnd2_m_r);
  float32_t result_m_i = f32_sub(fopnd2_m_i, fopnd1_m_i);
  result_l_r = fixupNaN_f32_f32_f32(result_l_r, fopnd1_l_r, fopnd2_l_r);
  result_l_i = fixupNaN_f32_f32_f32(result_l_i, fopnd1_l_i, fopnd2_l_i);
  result_m_r = fixupNaN_f32_f32_f32(result_m_r, fopnd1_m_r, fopnd2_m_r);
  result_m_i = fixupNaN_f32_f32_f32(result_m_i, fopnd1_m_i, fopnd2_m_i);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = (uint32_t)result_l_r.v | (uint64_t)result_l_i.v<<32;
  Int256_dwords_1(result) = (uint32_t)result_m_r.v | (uint64_t)result_m_i.v<<32;
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_16_16(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float16_t fopnd1 = { Int256_toUInt32(opnd1) };
  float16_t fopnd2 = { Int256_toUInt32(opnd2) };
  float16_t result = f16_sub(fopnd2, fopnd1);
  result = fixupNaN_f16_f16_f16(result, fopnd1, fopnd2);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1 = { Int256_toUInt32(opnd1) };
  float32_t fopnd2 = { Int256_toUInt32(opnd2) };
  float32_t result = f32_sub(fopnd2, fopnd1);
  result = fixupNaN_f32_f32_f32(result, fopnd1, fopnd2);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_64_64(void *this, Int256_ rounding_mode,Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1 = { Int256_toUInt64(opnd1) };
  float64_t fopnd2 = { Int256_toUInt64(opnd2) };
  float64_t result = f64_sub(fopnd2, fopnd1);
  result = fixupNaN_f64_f64_f64(result, fopnd1, fopnd2);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words1 = opnd1.words;
  uint32_t *words2 = opnd2.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float32_t fopnd2 = { words2[i] };
    float32_t fresult = f32_sub(fopnd2, fopnd1);
    fresult = fixupNaN_f32_f32_f32(fresult, fopnd1, fopnd2);
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fsbf_32_32_(this, rounding_mode, opnd1, opnd2, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fsbf_32_32_(this, rounding_mode, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_32_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fsbf_32_32_(this, rounding_mode, opnd1, opnd2, 8);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_64_64_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1_l = { Int256_dwords_0(opnd1) };
  float64_t fopnd1_m = { Int256_dwords_1(opnd1) };
  float64_t fopnd2_l = { Int256_dwords_0(opnd2) };
  float64_t fopnd2_m = { Int256_dwords_1(opnd2) };
  float64_t result_l = f64_sub(fopnd2_l, fopnd1_l);
  float64_t result_m = f64_sub(fopnd2_m, fopnd1_m);
  result_l = fixupNaN_f64_f64_f64(result_l, fopnd1_l, fopnd2_l);
  result_m = fixupNaN_f64_f64_f64(result_m, fopnd1_m, fopnd2_m);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_l.v;
  Int256_dwords_1(result) = result_m.v;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_16_16_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint16_t *hwords1 = opnd1.hwords;
  uint16_t *hwords2 = opnd2.hwords;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float16_t fopnd1 = { hwords1[i] };
    float16_t fopnd2 = { hwords2[i] };
    float16_t fresult = f16_sub(fopnd2, fopnd1);
    fresult = fixupNaN_f16_f16_f16(fresult, fopnd1, fopnd2);
    result.hwords[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_16_16_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fsbf_16_16_(this, rounding_mode, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbf_16_16_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fsbf_16_16_(this, rounding_mode, opnd1, opnd2, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fsbfcc_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1_r = { Int256_toUInt32(opnd1) };
  float32_t fopnd1_i = { Int256_toUInt64(opnd1) >> 32 };
  float32_t fopnd2_r = { Int256_toUInt32(opnd2) };
  float32_t fopnd2_i = { Int256_toUInt64(opnd2) >> 32 };
  float32_t result_r = f32_sub(fopnd2_r, fopnd1_r);
  float32_t result_i = f32_add(fopnd1_i, fopnd2_i);
  result_r = fixupNaN_f32_f32_f32(result_r, fopnd1_r, fopnd2_r);
  result_i = fixupNaN_f32_f32_f32(result_i, fopnd1_i, fopnd2_i);
  return Int256_fromUInt64(((uint64_t)result_i.v << 32) | result_r.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbfcc_64_64(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1_r = { Int256_dwords_0(opnd1) };
  float64_t fopnd1_i = { Int256_dwords_1(opnd1) };
  float64_t fopnd2_r = { Int256_dwords_0(opnd2) };
  float64_t fopnd2_i = { Int256_dwords_1(opnd2) };
  float64_t result_r = f64_sub(fopnd2_r, fopnd1_r);
  float64_t result_i = f64_add(fopnd1_i, fopnd2_i);
  result_r = fixupNaN_f64_f64_f64(result_r, fopnd1_r, fopnd2_r);
  result_i = fixupNaN_f64_f64_f64(result_i, fopnd1_i, fopnd2_i);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = result_r.v;
  Int256_dwords_1(result) = result_i.v;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsbfcc_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1_l_r = { Int256_dwords_0(opnd1) };
  float32_t fopnd1_l_i = { Int256_dwords_0(opnd1)>>32 };
  float32_t fopnd1_m_r = { Int256_dwords_1(opnd1) };
  float32_t fopnd1_m_i = { Int256_dwords_1(opnd1)>>32 };
  float32_t fopnd2_l_r = { Int256_dwords_0(opnd2) };
  float32_t fopnd2_l_i = { Int256_dwords_0(opnd2)>>32 };
  float32_t fopnd2_m_r = { Int256_dwords_1(opnd2) };
  float32_t fopnd2_m_i = { Int256_dwords_1(opnd2)>>32 };
  float32_t result_l_r = f32_sub(fopnd2_l_r, fopnd1_l_r);
  float32_t result_l_i = f32_add(fopnd1_l_i, fopnd2_l_i);
  float32_t result_m_r = f32_sub(fopnd2_m_r, fopnd1_m_r);
  float32_t result_m_i = f32_add(fopnd1_m_i, fopnd2_m_i);
  result_l_r = fixupNaN_f32_f32_f32(result_l_r, fopnd1_l_r, fopnd2_l_r);
  result_l_i = fixupNaN_f32_f32_f32(result_l_i, fopnd1_l_i, fopnd2_l_i);
  result_m_r = fixupNaN_f32_f32_f32(result_m_r, fopnd1_m_r, fopnd2_m_r);
  result_m_i = fixupNaN_f32_f32_f32(result_m_i, fopnd1_m_i, fopnd2_m_i);
  Int256_ result = Int256_zero;
  Int256_dwords_0(result) = (uint32_t)result_l_r.v | (uint64_t)result_l_i.v<<32;
  Int256_dwords_1(result) = (uint32_t)result_m_r.v | (uint64_t)result_m_i.v<<32;
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fmax_32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  flip32 fopnd1 = Int256_toUInt64(opnd1);
  flip32 fopnd2 = Int256_toUInt64(opnd2);
  flip32 result = Flip32_max(fopnd1, fopnd2);
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmax_64(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  flip64 fopnd1 = Int256_toUInt64(opnd1);
  flip64 fopnd2 = Int256_toUInt64(opnd2);
  flip64 result = Flip64_max(fopnd1, fopnd2);
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmax_16_x4(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  uint64_t xopnd1 = Int256_toUInt64(opnd1);
  uint64_t xopnd2 = Int256_toUInt64(opnd2);
  flip16 fopnd1_l_b = (uint16_t)(xopnd1);
  flip16 fopnd1_l_t = (uint16_t)(xopnd1 >> 16);
  flip16 fopnd1_m_b = (uint16_t)(xopnd1 >> 32);
  flip16 fopnd1_m_t = (uint16_t)(xopnd1 >> 48);
  flip16 fopnd2_l_b = (uint16_t)(xopnd2);
  flip16 fopnd2_l_t = (uint16_t)(xopnd2 >> 16);
  flip16 fopnd2_m_b = (uint16_t)(xopnd2 >> 32);
  flip16 fopnd2_m_t = (uint16_t)(xopnd2 >> 48);
  flip16 result_l_b = Flip16_max(fopnd1_l_b, fopnd2_l_b);
  flip16 result_l_t = Flip16_max(fopnd1_l_t, fopnd2_l_t);
  flip16 result_m_b = Flip16_max(fopnd1_m_b, fopnd2_m_b);
  flip16 result_m_t = Flip16_max(fopnd1_m_t, fopnd2_m_t);
  uint64_t result = ((uint64_t)result_l_b) | ((uint64_t)result_l_t << 16)
                  | ((uint64_t)result_m_b << 32) | ((uint64_t)result_m_t << 48);
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmax_32_(void *this, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  uint32_t *words1 = opnd1.words;
  uint32_t *words2 = opnd2.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float32_t fopnd2 = { words2[i] };
    float32_t fresult = { Flip32_max(fopnd1.v, fopnd2.v) };
    //fresult = fixupNaN_f32_f32_f32(fresult, fopnd1, fopnd2); // FIXME
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmax_32_x2(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmax_32_(this, opnd1, opnd2, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmax_32_x4(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmax_32_(this, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmax_32_x8(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmax_32_(this, opnd1, opnd2, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fmin_32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  flip32 fopnd1 = Int256_toUInt64(opnd1);
  flip32 fopnd2 = Int256_toUInt64(opnd2);
  flip32 result = Flip32_min(fopnd1, fopnd2);
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmin_64(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  flip64 fopnd1 = Int256_toUInt64(opnd1);
  flip64 fopnd2 = Int256_toUInt64(opnd2);
  flip64 result = Flip64_min(fopnd1, fopnd2);
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmin_16_x4(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  uint64_t xopnd1 = Int256_toUInt64(opnd1);
  uint64_t xopnd2 = Int256_toUInt64(opnd2);
  flip16 fopnd1_l_b = (uint16_t)(xopnd1);
  flip16 fopnd1_l_t = (uint16_t)(xopnd1 >> 16);
  flip16 fopnd1_m_b = (uint16_t)(xopnd1 >> 32);
  flip16 fopnd1_m_t = (uint16_t)(xopnd1 >> 48);
  flip16 fopnd2_l_b = (uint16_t)(xopnd2);
  flip16 fopnd2_l_t = (uint16_t)(xopnd2 >> 16);
  flip16 fopnd2_m_b = (uint16_t)(xopnd2 >> 32);
  flip16 fopnd2_m_t = (uint16_t)(xopnd2 >> 48);
  flip16 result_l_b = Flip16_min(fopnd1_l_b, fopnd2_l_b);
  flip16 result_l_t = Flip16_min(fopnd1_l_t, fopnd2_l_t);
  flip16 result_m_b = Flip16_min(fopnd1_m_b, fopnd2_m_b);
  flip16 result_m_t = Flip16_min(fopnd1_m_t, fopnd2_m_t);
  uint64_t result = ((uint64_t)result_l_b) | ((uint64_t)result_l_t << 16)
                  | ((uint64_t)result_m_b << 32) | ((uint64_t)result_m_t << 48);
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmin_32_(void *this, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  uint32_t *words1 = opnd1.words;
  uint32_t *words2 = opnd2.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float32_t fopnd2 = { words2[i] };
    float32_t fresult = { Flip32_min(fopnd1.v, fopnd2.v) };
    //fresult = fixupNaN_f32_f32_f32(fresult, fopnd1, fopnd2); // FIXME
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmin_32_x2(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmin_32_(this, opnd1, opnd2, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmin_32_x4(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmin_32_(this, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmin_32_x8(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmin_32_(this, opnd1, opnd2, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fneg_32_(void *this, Int256_ opnd1, int n)
{
  reset_exceptions(this);
  uint32_t *words1 = opnd1.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float32_t fresult = { fopnd1.v ^ 0x80000000 };
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fneg_32_x2(void *this, Int256_ opnd1)
{
  return Behavior_fneg_32_(this, opnd1, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fneg_32_x4(void *this, Int256_ opnd1)
{
  return Behavior_fneg_32_(this, opnd1, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fneg_32_x8(void *this, Int256_ opnd1)
{
  return Behavior_fneg_32_(this, opnd1, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fabs_32_(void *this, Int256_ opnd1, int n)
{
  reset_exceptions(this);
  uint32_t *words1 = opnd1.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float32_t fopnd1 = { words1[i] };
    float32_t fresult = { fopnd1.v & 0x7FFFFFFF };
    result.words[i] = fresult.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fabs_32_x2(void *this, Int256_ opnd1)
{
  return Behavior_fabs_32_(this, opnd1, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fabs_32_x4(void *this, Int256_ opnd1)
{
  return Behavior_fabs_32_(this, opnd1, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fabs_32_x8(void *this, Int256_ opnd1)
{
  return Behavior_fabs_32_(this, opnd1, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fsdiv_32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  flip32 fopnd1 = Int256_toUInt64(opnd1);
  flip32 fopnd2 = Int256_toUInt64(opnd2);
  // 1 means: can raise exceptions.
  return Int256_fromUInt32(Flip32_inv(fopnd1, fopnd2, 1));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsdiv_64(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  flip64 fopnd1 = Int256_toUInt64(opnd1);
  flip64 fopnd2 = Int256_toUInt64(opnd2);
  // 1 means: can raise exceptions.
  return Int256_fromUInt64(Flip64_inv(fopnd1, fopnd2, 1));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsdiv_32_x2(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  flip32 fopnd1_l = Int256_toUInt32(opnd1);
  flip32 fopnd1_m = Int256_toUInt64(opnd1) >> 32;
  flip32 fopnd2_l = Int256_toUInt32(opnd2);
  flip32 fopnd2_m = Int256_toUInt64(opnd2) >> 32;
  // 1 means: can raise exceptions.
  flip32 result_l = Flip32_inv(fopnd1_l, fopnd2_l, 1);
  flip32 result_m = Flip32_inv(fopnd1_m, fopnd2_m, 1);
  return Int256_fromUInt64(((uint64_t)result_m << 32) | result_l);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fcdiv_32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  flip32 fopnd1 = Int256_toUInt64(opnd1);
  flip32 fopnd2 = Int256_toUInt64(opnd2);
  // 1 means: can raise exceptions.
  return Int256_fromUInt32(Flip32_inv(fopnd1, fopnd2, 2));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fcdiv_64(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  flip64 fopnd1 = Int256_toUInt64(opnd1);
  flip64 fopnd2 = Int256_toUInt64(opnd2);
  // 1 means: can raise exceptions.
  return Int256_fromUInt64(Flip64_inv(fopnd1, fopnd2, 2));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fcdiv_32_x2(void *this, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  flip32 fopnd1_l = Int256_toUInt32(opnd1);
  flip32 fopnd1_m = Int256_toUInt64(opnd1) >> 32;
  flip32 fopnd2_l = Int256_toUInt32(opnd2);
  flip32 fopnd2_m = Int256_toUInt64(opnd2) >> 32;
  // 1 means: can raise exceptions.
  flip32 result_l = Flip32_inv(fopnd1_l, fopnd2_l, 2);
  flip32 result_m = Flip32_inv(fopnd1_m, fopnd2_m, 2);
  return Int256_fromUInt64(((uint64_t)result_m << 32) | result_l);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fsrec_32(void *this, Int256_ opnd1)
{
  reset_exceptions(this);
  flip32 fopnd1 = Int256_toUInt64(opnd1);
  // 1 means: can raise exceptions.
  return Int256_fromUInt32(Flip32_inv(0x3f800000, fopnd1, 1));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsrec_64(void *this, Int256_ opnd1)
{
  reset_exceptions(this);
  flip64 fopnd1 = Int256_toUInt64(opnd1);
  // 1 means: can raise exceptions.
  return Int256_fromUInt64(Flip64_inv(0x3ff0000000000000, fopnd1, 1));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsrec_32_x2(void *this, Int256_ opnd1)
{
  reset_exceptions(this);
  flip32 fopnd1_l = Int256_toUInt32(opnd1);
  flip32 fopnd1_m = Int256_toUInt64(opnd1) >> 32;
  // 1 means: can raise exceptions.
  flip32 result_l = Flip32_inv(0x3f800000, fopnd1_l, 1);
  flip32 result_m = Flip32_inv(0x3f800000, fopnd1_m, 1);
  return Int256_fromUInt64(((uint64_t)result_m << 32) | result_l);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_frec_32(void *this, Int256_ rounding_mode, Int256_ opnd1)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1 = {Int256_toUInt32(opnd1) };
  // 1.0f
  float32_t fone = {0x3f800000};
  float32_t result = f32_div(fone, fopnd1);
  result = fixupNaN_f32_f32(result, fopnd1);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fsrsr_32(void *this, Int256_ opnd1)
{
  reset_exceptions(this);
  flip32 fopnd1 = Int256_toUInt64(opnd1);
  return Int256_fromUInt32(Flip32_invsqrt(fopnd1));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsrsr_64(void *this, Int256_ opnd1)
{
  reset_exceptions(this);
  flip64 fopnd1 = Int256_toUInt64(opnd1);
  return Int256_fromUInt64(Flip64_invsqrt(fopnd1));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fsrsr_32_x2(void *this, Int256_ opnd1)
{
  reset_exceptions(this);
  flip32 fopnd1_l = Int256_toUInt32(opnd1);
  flip32 fopnd1_m = Int256_toUInt64(opnd1) >> 32;
  flip32 result_l = Flip32_invsqrt(fopnd1_l);
  flip32 result_m = Flip32_invsqrt(fopnd1_m);
  return Int256_fromUInt64(((uint64_t)result_m << 32) | result_l);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_frsq_32(void *this, Int256_ rounding_mode, Int256_ opnd1)
{
  reset_exceptions(this);
  float32_t fopnd1 = {Int256_toUInt32(opnd1) };
  float64_t fone = {0x3ff0000000000000};

  // forcing rounding mode to Round to nearest
  // for internal operations
  softfloat_set_rounding_mode(ModifierMemberValue_kv3_rounding_RN);

  float64_t large_result = f64_div(fone, f64_sqrt(f32_to_f64(fopnd1)));

  // setting rounding mode to env rounding mode for final conversion
  set_rounding_mode(this, rounding_mode);

  float32_t result = f64_to_f32(large_result);
  result = fixupNaN_f32_f32(result, fopnd1);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fwiden_16_32(void *this, Int256_ opnd1)
{
  reset_exceptions(this);
  float16_t fopnd1 = { Int256_toUInt32(opnd1) };
  float32_t result = f16_to_f32(fopnd1);
  result = fixupNaN_f32_f16(result, fopnd1);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fwiden_32_64(void *this, Int256_ opnd1)
{
  reset_exceptions(this);
  float32_t fopnd1 = { Int256_toUInt32(opnd1) };
  float64_t result = f32_to_f64(fopnd1);
  result = fixupNaN_f64_f32(result, fopnd1);
  return Int256_fromUInt64(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fwiden_16_32_(void *this, Int256_ opnd1, int n, int reset)
{
  if (reset)
    reset_exceptions(this);
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    float16_t fopnd1 = { opnd1.hwords[i] };
    float32_t fwiden = f16_to_f32(fopnd1);
    fwiden = fixupNaN_f32_f16(fwiden, fopnd1);
    result.words[i] = fwiden.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fwiden_16_32_x2(void *this, Int256_ opnd1)
{
  return Behavior_fwiden_16_32_(this, opnd1, 2, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fwiden_16_32_x8_reset(void *this, Int256_ opnd1)
{
  return Behavior_fwiden_16_32_(this, opnd1, 8, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fwiden_16_32_x8_noreset(void *this, Int256_ opnd1)
{
  return Behavior_fwiden_16_32_(this, opnd1, 8, 0);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fnarrow_32_16(void *this, Int256_ rounding_mode, Int256_ opnd1)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1 = { Int256_toUInt32(opnd1) };
  float16_t result = f32_to_f16(fopnd1);
  result = fixupNaN_f16_f32(result, fopnd1);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fnarrow_64_32(void *this, Int256_ rounding_mode, Int256_ opnd1)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1 = { Int256_toUInt64(opnd1) };
  float32_t result = f64_to_f32(fopnd1);
  result = fixupNaN_f32_f64(result, fopnd1);
  return Int256_fromUInt32(result.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fnarrow_64_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float64_t fopnd1_l = { Int256_dwords_0(opnd1) };
  float64_t fopnd1_m = { Int256_dwords_1(opnd1) };
  float32_t result_l = f64_to_f32(fopnd1_l);
  float32_t result_m = f64_to_f32(fopnd1_m);
  result_l = fixupNaN_f32_f64(result_l, fopnd1_l);
  result_m = fixupNaN_f32_f64(result_m, fopnd1_m);
  return Int256_fromUInt64((uint64_t)result_m.v<<32 |
                           (uint32_t)result_l.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fnarrow_32_16_x4(void *this, Int256_ rounding_mode, Int256_ opnd1)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1_x = { Int256_dwords_0(opnd1) };
  float32_t fopnd1_y = { Int256_dwords_0(opnd1)>>32 };
  float32_t fopnd1_z = { Int256_dwords_1(opnd1) };
  float32_t fopnd1_t = { Int256_dwords_1(opnd1)>>32 };
  float16_t result_x = f32_to_f16(fopnd1_x);
  float16_t result_y = f32_to_f16(fopnd1_y);
  float16_t result_z = f32_to_f16(fopnd1_z);
  float16_t result_t = f32_to_f16(fopnd1_t);
  result_x = fixupNaN_f16_f32(result_x, fopnd1_x);
  result_y = fixupNaN_f16_f32(result_y, fopnd1_y);
  result_z = fixupNaN_f16_f32(result_z, fopnd1_z);
  result_t = fixupNaN_f16_f32(result_t, fopnd1_t);
  return Int256_fromUInt64((uint64_t)result_t.v<<48 |
                           (uint64_t)result_z.v<<32 |
                           (uint32_t)result_y.v<<16 |
                           (uint16_t)result_x.v);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fnarrow_32_16_x8_helper(void *this, Int256_ rounding_mode, Int256_ opnd1)
{
  set_rounding_mode(this, rounding_mode);
  float32_t fopnd1_0 = { Int256_dwords_0(opnd1) };
  float32_t fopnd1_1 = { Int256_dwords_0(opnd1)>>32 };
  float32_t fopnd1_2 = { Int256_dwords_1(opnd1) };
  float32_t fopnd1_3 = { Int256_dwords_1(opnd1)>>32 };
  float32_t fopnd1_4 = { Int256_dwords_2(opnd1) };
  float32_t fopnd1_5 = { Int256_dwords_2(opnd1)>>32 };
  float32_t fopnd1_6 = { Int256_dwords_3(opnd1) };
  float32_t fopnd1_7 = { Int256_dwords_3(opnd1)>>32 };
  float16_t result_0 = f32_to_f16(fopnd1_0);
  float16_t result_1 = f32_to_f16(fopnd1_1);
  float16_t result_2 = f32_to_f16(fopnd1_2);
  float16_t result_3 = f32_to_f16(fopnd1_3);
  float16_t result_4 = f32_to_f16(fopnd1_4);
  float16_t result_5 = f32_to_f16(fopnd1_5);
  float16_t result_6 = f32_to_f16(fopnd1_6);
  float16_t result_7 = f32_to_f16(fopnd1_7);
  result_0 = fixupNaN_f16_f32(result_0, fopnd1_0);
  result_1 = fixupNaN_f16_f32(result_1, fopnd1_1);
  result_2 = fixupNaN_f16_f32(result_2, fopnd1_2);
  result_3 = fixupNaN_f16_f32(result_3, fopnd1_3);
  result_4 = fixupNaN_f16_f32(result_4, fopnd1_4);
  result_5 = fixupNaN_f16_f32(result_5, fopnd1_5);
  result_6 = fixupNaN_f16_f32(result_6, fopnd1_6);
  result_7 = fixupNaN_f16_f32(result_7, fopnd1_7);
  return Int256_make((uint64_t)result_0.v     |
                     (uint64_t)result_1.v<<16 |
                     (uint64_t)result_2.v<<32 |
                     (uint64_t)result_3.v<<48,
                     (uint64_t)result_4.v     |
                     (uint64_t)result_5.v<<16 |
                     (uint64_t)result_6.v<<32 |
                     (uint64_t)result_7.v<<48,
                     0, 0);
}

/** First stage of multiple fnarrow: reset exception before computation */
static inline Int256_
Behavior_fnarrow_32_16_x8_reset(void *this, Int256_ rounding_mode, Int256_ opnd1)
{
  reset_exceptions(this);
  return Behavior_fnarrow_32_16_x8_helper(this, rounding_mode, opnd1);
}

/** Second stage of multiple fnarrow: do not reset exception before computation */
static inline Int256_
Behavior_fnarrow_32_16_x8_noreset(void *this, Int256_ rounding_mode, Int256_ opnd1)
{
  return Behavior_fnarrow_32_16_x8_helper(this, rounding_mode, opnd1);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline int32_t
hybrid_mac(int32_t input, float scale, int32_t offset)
{
    double input_d = input; // exact
    double scale_d = scale; // exact
    // this multiplication is not exact
    double result_hi = input_d * scale_d;
    // result_hi + resul_lo is exactly equal to (input_d * scale_d)
    double result_lo = fma(-input_d, scale_d, result_hi);
    if (result_hi >= 0x1p32) {
         // no value of offset can avoid positive overflow
         return INT32_MAX;
    } else if (result_hi < -0x1p32) {
       // no value of offset can avoid negative overflow
       return INT32_MIN;
    } else {
       // the difficulty is that input * scale may need 24+31 = 55 bits
       // to be represented exactly, which does not fit a double
       // dirty trick we use a 80-bit long double representation (which has 64-bit
       // significand
       long double result = (long double) result_hi + (long double) result_lo;
       // FIXME: in at least round-towards-zero
       // round(input * scale + offset) != round(input * scale) + offset
       // For example: round(1 * -0.5 + 1) = 0 != round(1 * -0.5) + 1 = 1
       switch(softfloat_roundingMode) {
       case softfloat_round_near_even: // RN
           return offset + (int32_t) roundl(result);
       case softfloat_round_max: // RU
           return offset + (int32_t) ceill(result);
       case softfloat_round_min: // RD
           return offset + (int32_t) floorl(result);
       case softfloat_round_minMag: // RZ
           return offset + (int32_t) (result > 0 ? floorl(result) : ceill(result));
       case softfloat_round_near_maxMag: // RNA
           return offset + (int32_t) nearbyintl(result + 0.5);
       case softfloat_round_odd: // RO
       default:
           abort(); // error
       }
    }
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_maddif_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words1 = opnd1.words;
  uint32_t *words2 = opnd2.words;
  float *words3 = (float *)opnd3.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    uint32_t opnd1 = words1[i];
    uint32_t opnd2 = words2[i];
    float fopnd3 = words3[i];
    result.words[i] = hybrid_mac(opnd2, fopnd3, opnd1);
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_maddif_32_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_maddif_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 8);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_msbfif_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  uint32_t *words1 = opnd1.words;
  float *words2 = (float *)opnd2.words;
  uint32_t *words3 = opnd3.words;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    int32_t opnd1 = words1[i];
    float fopnd2 = -words2[i];
    int32_t opnd3 = words3[i];
    result.words[i] = hybrid_mac(opnd3, fopnd2, opnd1);
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_msbfif_32_32_x8(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_msbfif_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_mul_8_32_(void *this, Int256_ opnd1, Int256_ opnd2, int n)
{
  uint8_t *bytes1 = opnd1.bytes;
  uint8_t *bytes2 = opnd2.bytes;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = (int8_t)bytes1[i] * (int8_t)bytes2[i];
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_mul_8_32_x4(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_mul_8_32_(this, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_mul_8_32_x8(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_mul_8_32_(this, opnd1, opnd2, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_mulu_8_32_(void *this, Int256_ opnd1, Int256_ opnd2, int n)
{
  uint8_t *bytes1 = opnd1.bytes;
  uint8_t *bytes2 = opnd2.bytes;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = (uint8_t)bytes1[i] * (uint8_t)bytes2[i];
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_mulu_8_32_x4(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_mulu_8_32_(this, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_mulu_8_32_x8(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_mulu_8_32_(this, opnd1, opnd2, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_mulsu_8_32_(void *this, Int256_ opnd1, Int256_ opnd2, int n)
{
  uint8_t *bytes1 = opnd1.bytes;
  uint8_t *bytes2 = opnd2.bytes;
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = (int8_t)bytes1[i] * (uint8_t)bytes2[i];
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_mulsu_8_32_x4(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_mulsu_8_32_(this, opnd1, opnd2, 4);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_mulsu_8_32_x8(void *this, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_mulsu_8_32_(this, opnd1, opnd2, 8);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_clamp_32_32_x8(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  Int256_ result = Int256_zero;
  for (int i = 0; i < 8; i++) {
    int32_t value = opnd1.words[i];
    int32_t min = opnd2.words[i];
    int32_t max = opnd3.words[i];
    value = value < min? min: value;
    value = value > max? max: value;
    result.words[i] = value;
  }
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_trunc_32_8_x8(void *this, Int256_ opnd1)
{
  Int256_ result = Int256_zero;
  for (int i = 0; i < 8; i++) {
    result.bytes[i] = opnd1.words[i];
  }
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_sx_8_32_x8(void *this, Int256_ opnd1)
{
  Int256_ result = Int256_zero;
  for (int i = 0; i < 8; i++) {
    result.words[i] = (int8_t)opnd1.bytes[i];
  }
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_zx_8_32_x8(void *this, Int256_ opnd1)
{
  Int256_ result = Int256_zero;
  for (int i = 0; i < 8; i++) {
    result.words[i] = (uint8_t)opnd1.bytes[i];
  }
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fpow2scale_32_32_x8(void *this, Int256_ rounding_mode, Int256_ pow2scale, Int256_ opnd1)
{
  int i;
  Int256_ result;
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  union {
    uint32_t u;
    struct {
      unsigned mant:23;
      unsigned exp:8;
      unsigned sign:1;
    } s;
    float32_t f;
  } fscale = { 0x3f800000 };
  fscale.s.exp += pow2scale.words[0];
  for (i = 0; i < 8; i++) {
    float32_t fopnd1_i = { opnd1.words[i] };
    float32_t result_i = f32_mul(fopnd1_i, fscale.f);
    result_i = fixupNaN_f32_f32(result_i, fopnd1_i);
    result.words[i] = result_i.v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_frelu_32_32_x8_helper(void *this, Int256_ opnd1)
{
  int i;
  Int256_ result;
  for (i = 0; i < 8; i++) {
    flip32 fopnd1_i = opnd1.words[i];
    // the min value is set to -0, so that every negative value
    // is rounded to minus zero
    flip32 result_i = Flip32_max(fopnd1_i, f32_minusZero_flip);
    result_i = fixupNaN_f32_f32_flip(result_i, fopnd1_i);
    result.words[i] = result_i;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_frelu_32_32_x8_reset(void *this, Int256_ opnd1)
{
  reset_exceptions(this);
  return Behavior_frelu_32_32_x8_helper(this, opnd1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_frelu_32_32_x8_noreset(void *this, Int256_ opnd1)
{
  return Behavior_frelu_32_32_x8_helper(this, opnd1);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_finvalid(void *this)
{
  bool flag = ((softfloat_exceptionFlags & softfloat_flag_invalid) | (flip_get_exceptions() & FLIP_INVALID)) != 0;
  return Int256_fromUInt32(flag);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_foverflow(void *this)
{
  bool flag = ((softfloat_exceptionFlags & softfloat_flag_overflow) | (flip_get_exceptions() & FLIP_OVERFLOW)) != 0;
  return Int256_fromUInt32(flag);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_funderflow(void *this)
{
  bool flag = ((softfloat_exceptionFlags & softfloat_flag_underflow) | (flip_get_exceptions() & FLIP_UNDERFLOW)) != 0;
  return Int256_fromUInt32(flag);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_finexact(void *this)
{
  bool flag = ((softfloat_exceptionFlags & softfloat_flag_inexact) | (flip_get_exceptions() & FLIP_INEXACT)) != 0;
  return Int256_fromUInt32(flag);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fdivbyzero(void *this)
{
  bool flag = ((softfloat_exceptionFlags & softfloat_flag_infinite) | (flip_get_exceptions() & FLIP_DIV_BY_ZERO)) != 0;
  return Int256_fromUInt32(flag);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#ifdef $XCC__h
static inline Int256_
Behavior_transpose_32_2x2(void *this, Int256_ opnd1)
{
  Int256_ result = Int256_zero;
  result.words[0] = opnd1.words[0];
  result.words[1] = opnd1.words[2];
  result.words[2] = opnd1.words[1];
  result.words[3] = opnd1.words[3];
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_transpose_16_4x4(void *this, Int256_ opnd1)
{
  Int256_ result = Int256_zero;
  short (*in)[4] = (short (*)[4])&opnd1;
  short (*out)[4] = (short (*)[4])&result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      out[j][i] = in[i][j];
    }
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_transpose_32_4x4_0(void *this, Int256_ opnd1, Int256_ opnd2)
{
  Int256_ result = Int256_zero;
  int (*in1)[2] = (int (*)[2])&opnd1;
  int (*in2)[2] = (int (*)[2])&opnd2;
  int (*out)[2] = (int (*)[2])&result;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++) {
      out[j][i] = j < 2? in1[i][j]: in2[i][j-2];
    }
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_transpose_32_4x4_1(void *this, Int256_ opnd1, Int256_ opnd2)
{
  Int256_ result = Int256_zero;
  int (*in1)[2] = (int (*)[2])&opnd1;
  int (*in2)[2] = (int (*)[2])&opnd2;
  int (*out)[2] = (int (*)[2])&result;
  for (int i = 2; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      out[j][i-2] = j < 2? in1[i][j]: in2[i][j-2];
    }
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_transpose_64_4x4_0(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, Int256_ opnd4)
{
  Int256_ result = Int256_zero;
  result.dwords[0] = opnd1.dwords[0];
  result.dwords[1] = opnd2.dwords[0];
  result.dwords[2] = opnd3.dwords[0];
  result.dwords[3] = opnd4.dwords[0];
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_transpose_64_4x4_1(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, Int256_ opnd4)
{
  Int256_ result = Int256_zero;
  result.dwords[0] = opnd1.dwords[1];
  result.dwords[1] = opnd2.dwords[1];
  result.dwords[2] = opnd3.dwords[1];
  result.dwords[3] = opnd4.dwords[1];
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_transpose_64_4x4_2(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, Int256_ opnd4)
{
  Int256_ result = Int256_zero;
  result.dwords[0] = opnd1.dwords[2];
  result.dwords[1] = opnd2.dwords[2];
  result.dwords[2] = opnd3.dwords[2];
  result.dwords[3] = opnd4.dwords[2];
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_transpose_64_4x4_3(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, Int256_ opnd4)
{
  Int256_ result = Int256_zero;
  result.dwords[0] = opnd1.dwords[3];
  result.dwords[1] = opnd2.dwords[3];
  result.dwords[2] = opnd3.dwords[3];
  result.dwords[3] = opnd4.dwords[3];
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_join_8_x8(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, Int256_ opnd4,
                               Int256_ opnd5, Int256_ opnd6, Int256_ opnd7, Int256_ opnd8)
{
  Int256_ result = Int256_zero;
  result.bytes[0] = opnd1.bytes[0];
  result.bytes[1] = opnd2.bytes[0];
  result.bytes[2] = opnd3.bytes[0];
  result.bytes[3] = opnd4.bytes[0];
  result.bytes[4] = opnd5.bytes[0];
  result.bytes[5] = opnd6.bytes[0];
  result.bytes[6] = opnd7.bytes[0];
  result.bytes[7] = opnd8.bytes[0];
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_join_32_x4(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, Int256_ opnd4)
{
  Int256_ result = Int256_zero;
  result.words[0] = opnd1.words[0];
  result.words[1] = opnd2.words[0];
  result.words[2] = opnd3.words[0];
  result.words[3] = opnd4.words[0];
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_join_32_x8(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, Int256_ opnd4,
                                Int256_ opnd5, Int256_ opnd6, Int256_ opnd7, Int256_ opnd8)
{
  Int256_ result = Int256_zero;
  result.words[0] = opnd1.words[0];
  result.words[1] = opnd2.words[0];
  result.words[2] = opnd3.words[0];
  result.words[3] = opnd4.words[0];
  result.words[4] = opnd5.words[0];
  result.words[5] = opnd6.words[0];
  result.words[6] = opnd7.words[0];
  result.words[7] = opnd8.words[0];
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_join_64_x2(void *this, Int256_ opnd1, Int256_ opnd2)
{
  Int256_ result = Int256_zero;
  result.dwords[0] = opnd1.dwords[0];
  result.dwords[1] = opnd2.dwords[0];
  result.dwords[2] = 0;
  result.dwords[3] = 0;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_join_64_x4(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, Int256_ opnd4)
{
  Int256_ result = Int256_zero;
  result.dwords[0] = opnd1.dwords[0];
  result.dwords[1] = opnd2.dwords[0];
  result.dwords[2] = opnd3.dwords[0];
  result.dwords[3] = opnd4.dwords[0];
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_insert_64(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  Int256_ result = opnd1;
  uint32_t qindex = opnd3.words[0];
  if (qindex < 4) {
    result.dwords[qindex] = opnd2.dwords[0];
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_insert_128(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  Int256_ result = opnd1;
  uint32_t hindex = opnd3.words[0];
  if (hindex < 2) {
    result.dwords[2*hindex + 0] = opnd2.dwords[0];
    result.dwords[2*hindex + 1] = opnd2.dwords[1];
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_extract_32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  Int256_ result = Int256_zero;
  uint32_t index = opnd2.words[0];
  if (index < 8) {
    result.words[0] = opnd1.words[index];
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_extract_64(void *this, Int256_ opnd1, Int256_ opnd2)
{
  Int256_ result = Int256_zero;
  uint32_t index = opnd2.words[0];
  if (index < 4) {
    result.dwords[0] = opnd1.dwords[index];
  }
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_group_8_x32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int j = 0;
  Int256_ result = Int256_zero;
  unsigned bits = opnd2.words[0];
  for (int i = 0; i < 32; i++) {
    if ((bits >> i & 0x1) == 0) {
      result.bytes[j++] = opnd1.bytes[i];
    }
  }
  for (int i = 0; i < 32; i++) {
    if ((bits >> i & 0x1) == 1) {
      result.bytes[j++] = opnd1.bytes[i];
    }
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_weave_8_2_x32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  Int256_ result = Int256_zero;
  int indices[4] = { 0, 0, 0, 0 };
  unsigned long long nibbles = opnd2.dwords[0];
  for (int i = 0; i < 32; i++) {
    unsigned nibble = (nibbles >> i*2) & 3;
    int index = indices[nibble]++;
    unsigned char *bytes = (void *)&result.dwords[nibble];
    if (index < 8) {
      bytes[index] = opnd1.bytes[i];
    }
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_valid_8_x32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  Int256_ result = Int256_zero;
  unsigned bits = opnd2.words[0];
  for (int i = 0; i < 32; i++) {
    if ((bits >> i & 0x1) == 1) {
      result.bytes[i] = opnd1.bytes[i];
    }
  }
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_add_32_32_x8(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int i;
  Int256_ result = Int256_zero;
  for (i = 0; i < 8; i++) {
    result.words[i] = opnd1.words[i] + opnd2.words[i];
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_add_64_64_x4(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int i;
  Int256_ result = Int256_zero;
  for (i = 0; i < 4; i++) {
    result.dwords[i] = opnd1.dwords[i] + opnd2.dwords[i];
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot8_8_32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int i;
  uint32_t result = 0;
  for (i = 0; i < 8; i++) {
    result += (int32_t)(int8_t)opnd1.bytes[i] * (int32_t)(int8_t)opnd2.bytes[i];
  }
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot8u_8_32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int i;
  uint32_t result = 0;
  for (i = 0; i < 8; i++) {
    result += (uint32_t)(uint8_t)opnd1.bytes[i] * (uint32_t)(uint8_t)opnd2.bytes[i];
  }
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot8su_8_32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int i;
  uint32_t result = 0;
  for (i = 0; i < 8; i++) {
    result += (int32_t)(int8_t)opnd1.bytes[i] * (uint32_t)(uint8_t)opnd2.bytes[i];
  }
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot8us_8_32(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int i;
  uint32_t result = 0;
  for (i = 0; i < 8; i++) {
    result += (uint32_t)(uint8_t)opnd1.bytes[i] * (int32_t)(int8_t)opnd2.bytes[i];
  }
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot16add_8_32(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  int i;
  uint32_t result = opnd3.words[0];
  for (i = 0; i < 16; i++) {
    result += (int32_t)(int8_t)opnd1.bytes[i] * (int32_t)(int8_t)opnd2.bytes[i];
  }
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot16addu_8_32(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  int i;
  uint32_t result = opnd3.words[0];
  for (i = 0; i < 16; i++) {
    result += (uint32_t)(uint8_t)opnd1.bytes[i] * (uint32_t)(uint8_t)opnd2.bytes[i];
  }
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot16addsu_8_32(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  int i;
  uint32_t result = opnd3.words[0];
  for (i = 0; i < 16; i++) {
    result += (int32_t)(int8_t)opnd1.bytes[i] * (uint32_t)(uint8_t)opnd2.bytes[i];
  }
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot16addus_8_32(void *this, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  int i;
  uint32_t result = opnd3.words[0];
  for (i = 0; i < 16; i++) {
    result += (uint32_t)(uint8_t)opnd1.bytes[i] * (int32_t)(int8_t)opnd2.bytes[i];
  }
  return Int256_fromUInt32(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot4_16_64(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int i;
  uint64_t result = 0;
  for (i = 0; i < 4; i++) {
    result += (int64_t)(int16_t)opnd1.hwords[i] * (int64_t)(int16_t)opnd2.hwords[i];
  }
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot4u_16_64(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int i;
  uint64_t result = 0;
  for (i = 0; i < 4; i++) {
    result += (uint64_t)(uint16_t)opnd1.hwords[i] * (uint64_t)(uint16_t)opnd2.hwords[i];
  }
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot4su_16_64(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int i;
  uint64_t result = 0;
  for (i = 0; i < 4; i++) {
    result += (int64_t)(int16_t)opnd1.hwords[i] * (uint64_t)(uint16_t)opnd2.hwords[i];
  }
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_dot4us_16_64(void *this, Int256_ opnd1, Int256_ opnd2)
{
  int i;
  uint64_t result = 0;
  for (i = 0; i < 4; i++) {
    result += (uint64_t)(uint16_t)opnd1.hwords[i] * (int64_t)(int16_t)opnd2.hwords[i];
  }
  return Int256_fromUInt64(result);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_zxb_x4(void *this, Int256_ opnd1)
{
  int i;
  Int256_ result = Int256_zero;
  uint64_t result_0 = 0;
  for (i = 3; i >= 0; i--) {
    result_0 = (result_0 << 16) | ((uint8_t)opnd1.bytes[i] & 0xFFFF);
  }
  result.dwords[0] = result_0;
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_sxb_x4(void *this, Int256_ opnd1)
{
  int i;
  Int256_ result = Int256_zero;
  uint64_t result_0 = 0;
  for (i = 3; i >= 0; i--) {
    result_0 = (result_0 << 16) | ((int8_t)opnd1.bytes[i] & 0xFFFF);
  }
  result.dwords[0] = result_0;
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
#define Two_Sum(a, b, xh, xl) do { \
  xh = f32_add(a, b); \
  Two_Sum_Tail(a, b, xh, xl); \
} while(0)
//
#define Two_Sum_Tail(a, b, xh, xl) do { \
  float32_t bvirt = f32_sub(xh, a); \
  float32_t avirt = f32_sub(xh, bvirt); \
  float32_t bround = f32_sub(b, bvirt); \
  float32_t around = f32_sub(a, avirt); \
  xl = f32_add(around, bround); \
} while (0)
//
#define Two_One_Sum(a1, a0, b, x2, x1, x0) do { \
  float32_t _i; \
  Two_Sum(a0, b , _i, x0); \
  Two_Sum(a1, _i, x2, x1); \
} while (0)
//
#define Two_Two_Sum(a1, a0, b1, b0, x3, x2, x1, x0) do { \
  float32_t _j, _0; \
  Two_One_Sum(a1, a0, b0, _j, _0, x0); \
  Two_One_Sum(_j, _0, b1, x3, x2, x1); \
} while (0)
//
#define Four_One_Sum(a3, a2, a1, a0, b, x4, x3, x2, x1, x0) do { \
  float32_t _k; \
  Two_One_Sum(a1, a0, b , _k, x1, x0); \
  Two_One_Sum(a3, a2, _k, x4, x3, x2); \
} while (0)
//
#define Two_Sum_f64(a, b, xh, xl) do { \
  xh = f64_add(a, b); \
  float64_t bvirt = f64_sub(xh, a); \
  float64_t avirt = f64_sub(xh, bvirt); \
  float64_t bround = f64_sub(b, bvirt); \
  float64_t around = f64_sub(a, avirt); \
  xl = f64_add(around, bround); \
} while (0)
//
#define Two_One_Sum_f64(a1, a0, b, x2, x1, x0) do { \
  float64_t _i; \
  Two_Sum_f64(a0, b , _i, x0); \
  Two_Sum_f64(a1, _i, x2, x1); \
} while (0)
//
#define Two_Two_Sum_f64(a1, a0, b1, b0, x3, x2, x1, x0) do { \
  float64_t _j, _0; \
  Two_One_Sum_f64(a1, a0, b0, _j, _0, x0); \
  Two_One_Sum_f64(_j, _0, b1, x3, x2, x1); \
} while (0)
//
#endif//$XCC__h

#ifdef $XCC__h

/** Set all mantissa bit of @p x to zero (except the implicit one) */
#define red(x) ((x).v & ~0x7fffffu)
#define mantissa_is_one(x) (red(x) == (x).v)
/** Quick & Dirty computation of @p Unit int last place  */
#define ulp(x) f32_sub((float32_t) {red(x) | 0x1}, (float32_t) {red(x)})

/** 0.5 in binary32 format */
static const __attribute__((unused)) float32_t half = { 0x3f000000 };
/** -2^(-p-1) = -2**-25 in binary32 format */
static const __attribute__((unused)) float32_t minus2powminusmpm1 = { 0xb3000000 };
/** Quick & Dirty computation of Half of @p Unit int last place  */
#define half_ulp(x) f32_mul(half, ulp(x))
/** compute a quarter (half an half of a unit in the last place for @p x */
#define half_half_ulp(x) f32_mul(half, (half_ulp(x)))
/** Quick & Dirty computation of the absolute value of @p x */
#define f32_abs(x) ((float32_t) {(x).v & ~0x80000000u})

static inline int is_round_to_nearest(Int256_ rounding_mode)
{
    return Int256_toUInt32(rounding_mode) == ModifierMemberValue_kv3_rounding_RN;
}

static inline Int256_
Behavior_fdot4add_16_32_helper(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  // numerical constant
  const float32_t zero = { 0 };

  // saving input flags and rounding mode
  int save_flags = softfloat_exceptionFlags;
  Int256_ overall_rounding_mode = rounding_mode;
  // forcing rounding mode to round-to-nearest
  set_rounding_mode(this, Int256_fromUInt32(ModifierMemberValue_kv3_rounding_RN));

  Int256_ result = Int256_zero;
  // operand extraction
  float16_t opnd1_0 = { opnd1.hwords[0] }, opnd2_0 = { opnd2.hwords[0] };
  float16_t opnd1_1 = { opnd1.hwords[1] }, opnd2_1 = { opnd2.hwords[1] };
  float16_t opnd1_2 = { opnd1.hwords[2] }, opnd2_2 = { opnd2.hwords[2] };
  float16_t opnd1_3 = { opnd1.hwords[3] }, opnd2_3 = { opnd2.hwords[3] };
  // partial products, operations are exact
  float32_t t0 = f32_mul(f16_to_f32(opnd1_0), f16_to_f32(opnd2_0));
  float32_t t1 = f32_mul(f16_to_f32(opnd1_1), f16_to_f32(opnd2_1));
  float32_t t2 = f32_mul(f16_to_f32(opnd1_2), f16_to_f32(opnd2_2));
  float32_t t3 = f32_mul(f16_to_f32(opnd1_3), f16_to_f32(opnd2_3));
  // input accumulator
  float32_t t4 = { opnd3.words[0] };

  // if any operands is infty, Shewchuk and twoSum do not work
  // so the result is the sum of inputs (multiple infinities will accumulate
  // or generate NaN if they are of opposite signs)
  float32_t result_0 = {0};
  // is there at least one infinity or one NaN among the products and accumulator
  int any_inf = f32_isInf(t0) || f32_isInf(t1) || f32_isInf(t2) || f32_isInf(t3) || f32_isInf(t4);
  int any_nan =  f32_isNaN(t0) || f32_isNaN(t1) || f32_isNaN(t2) || f32_isNaN(t3) || f32_isNaN(t4);
  // is there any NaNs (quiet or signaling) among the inputs
  int any_inputs_nan =  f16_isNaN(opnd1_0) || f16_isNaN(opnd2_0) ||
                         f16_isNaN(opnd1_1) || f16_isNaN(opnd2_1) ||
                         f16_isNaN(opnd1_2) || f16_isNaN(opnd2_2) ||
                         f16_isNaN(opnd1_3) || f16_isNaN(opnd2_3) ||
                         f32_isNaN(t4);
  // is there any signaling NaNs among the inputs
  int any_inputs_snan =  f16_isSigNaN(opnd1_0) || f16_isSigNaN(opnd2_0) ||
                         f16_isSigNaN(opnd1_1) || f16_isSigNaN(opnd2_1) ||
                         f16_isSigNaN(opnd1_2) || f16_isSigNaN(opnd2_2) ||
                         f16_isSigNaN(opnd1_3) || f16_isSigNaN(opnd2_3) ||
                         f32_isSigNaN(t4);
  if (any_inf || any_nan) {
    float32_t r0, r1, r2, r3, r4;
    r0 = t0;
    r1 = t1;
    r2 = t2;
    r3 = t3;
    r4 = t4;
    result_0 = f32_add(f32_add(f32_add(f32_add(r0, r1), r2), r3), r4);
    // operation with Inf or NaN does not raise inexact flags
    // so if raised arbitrarily by the above sum (which can add non special
    // values before adding special values) we must clear it
    if (!(save_flags & softfloat_flag_inexact)) softfloat_exceptionFlags &= ~softfloat_flag_inexact;
    // if there is a NaN within the number, and no sNaN we should not raise an invalid
    // exception (quiet NaN overcome possible invalid operations)
    if (!(save_flags & softfloat_flag_invalid) && (any_inputs_nan && !any_inputs_snan)) softfloat_exceptionFlags &= ~softfloat_flag_invalid;
    result_0 = fixupNaN_f32_f32_f32_f32_f32_f32(result_0, r0, r1, r2, r3, r4);

  } else if (f32_eq(zero, t0) && f32_eq(zero, t1) && f32_eq(zero, t2) && f32_eq(zero, t3) && f32_eq(zero, t4)) {
    // when all sum operands are zero, the sign is determined by addition rules
    set_rounding_mode(this, rounding_mode);
    result_0 = f32_add(f32_add(f32_add(f32_add(t0, t1), t2), t3), t4);

  } else {
    float32_t r0, r1, r2, r3, r4;
    // Shewchuk code for non-overlapping components.
    float32_t a0, a1, b0, b1;
    Two_Sum(t0, t1, a1, a0);
    Two_Sum(t2, t3, b1, b0);
    float32_t x0, x1, x2, x3;
    Two_Two_Sum(a1, a0, b1, b0, x3, x2, x1, x0);
    Four_One_Sum(x3, x2, x1, x0, t4, r4, r3, r2, r1, r0);

    // Sum the result sequence from small to large component.
    // r_v[0] is the largest component
    float32_t r_v[5] = {r4, r3, r2, r1, r0};

    // pushing zeros at the end of the result list
    // pre-Normalize the terms of results, ensuring that zeros elements
    //  are pushed back to high index cell of r_v
    int i = 0;
    for (i = 0; i < 5; ++i) {
        if (f32_eq(r_v[i], zero)) {
            int j = 0;
            for (j = i + 1; j < 5; ++j) r_v[j-1] = r_v[j];
            r_v[4] = zero;
        }
    }
    // normalization: to make sure the 2 most significant terms do not overlap
    for (i = 4; i > 0; i--) {
        // low part
        float32_t r_v0 = r_v[i-1];
        // high part
        float32_t r_v1 = r_v[i];
        Two_Sum(r_v0, r_v1, r_v[i-1], r_v[i]);
    };
    // test if the result is an exact 0 coming from a full cancellation
    int is_cancellation_zero = f32_eq(r_v[4], zero) && f32_eq(r_v[3], zero)
                               && f32_eq(r_v[2], zero) && f32_eq(r_v[1], zero)
                               && f32_eq(r_v[0], zero);

    // extract the two most significant limbs of the result
    float32_t r_hi = r_v[0];
    float32_t r_lo = r_v[1];


    // MUST avoid spurious exceptions here
    float32_t half_ulp_abs_msb = f32_abs(half_ulp(r_hi));

    /** Coarse evaluation of the error of r_v[0] + r_v[1] compared
     *  to the exact sum */
    float32_t eval_error = f32_add(r_v[2], f32_add(r_v[3], r_v[4]));


    int half_ulp_flag = f32_eq(f32_abs(r_lo), half_ulp_abs_msb);
    /* flag to fix T10167 case by detecting mid-point comming from
     * operation similar to 1.0 - 2^(-p-1) */
    int half_half_ulp_flag = mantissa_is_one(r_hi) && f32_eq(r_lo, f32_mul(minus2powminusmpm1, r_hi));

    // spurious exception during pre-computation must be discarded
    // reseting flag state
    softfloat_exceptionFlags = save_flags;

    /** Setting proper inexact flags */
    if (!f32_eq(r_lo, zero) || !f32_eq(eval_error, zero)) {
        softfloat_exceptionFlags |= softfloat_flag_inexact;
    }

    /* detection of rounding to nearest mode */
    int rne_flag = is_round_to_nearest(overall_rounding_mode);

    /** if the term before last is equal to the last half ulp
     *  and rounding mode is round-to-nearest (tie-to-even) we
     *  must compute the result in such a way that double rounding
     *  errors are avoided */
    if ((half_ulp_flag || half_half_ulp_flag) && rne_flag) {
        if (f32_lt(zero, eval_error)) {
            // if the error is greater than zero, then round up
            uint_fast8_t save_rnd_mode = softfloat_roundingMode;
            softfloat_roundingMode = softfloat_round_max;
            result_0 = f32_add(r_hi, r_lo);
            softfloat_roundingMode = save_rnd_mode;
        } else if (f32_lt(eval_error, zero)) {
            // if the error is less than zero, then round down
            uint_fast8_t save_rnd_mode = softfloat_roundingMode;
            softfloat_roundingMode = softfloat_round_min;
            result_0 = f32_add(r_hi, r_lo);
            softfloat_roundingMode = save_rnd_mode;
        } else {
            // if the error is zero, then rounding to nearest (at this point,
            // because we could use final_r4 and final_r3)
            result_0 = f32_add(r_hi, r_lo);
        }
    } else if (is_cancellation_zero) {
        set_rounding_mode(this, rounding_mode);
        // +0 - +0 = +0 + (-0) = 0 with correct sign for full cancellation cases
        result_0 = f32_sub(zero, zero);
    } else {
        set_rounding_mode(this, rounding_mode);
        result_0 = f32_add(f32_add(f32_add(f32_add(r_v[4],
                                                   r_v[3]),
                                           r_v[2]),
                                   r_v[1]),
                           r_v[0]);
    }

    // Cleaning NaN value of the result
    result_0 = fixupNaN_f32_f32_f32_f32_f32_f32(result_0, r0, r1, r2, r3, r4);

  }
  result.words[0] = result_0.v;
  return result;
}

/** fdot4 version with reset of IEEE flags */
static inline Int256_
Behavior_fdot4add_16_32_reset(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  return Behavior_fdot4add_16_32_helper(this, rounding_mode, opnd1, opnd2, opnd3);
}

/** fdot4 version without reset of IEEE flags */
static inline Int256_
Behavior_fdot4add_16_32_noreset(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_fdot4add_16_32_helper(this, rounding_mode, opnd1, opnd2, opnd3);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fdot8add_16_32_helper(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  // FIXME! Implement without the intermediate roundings.
  int i;
  float32_t result = { opnd3.words[0] };
  float16_t *fopnd1 = (float16_t *)&opnd1;
  float16_t *fopnd2 = (float16_t *)&opnd2;
  int any_inf = f32_isInf(result);
  int any_nan = f32_isNaN(result);
  int any_inputs_nan = f32_isNaN(result);
  int any_inputs_snan = f32_isSigNaN(result);
  int save_flags = softfloat_exceptionFlags;
  // numerical constant
  const float32_t zero = { 0 };
  const float64_t zero64 = { 0 };
  int all_zero = f32_eq(zero, result);
  // forcing rounding mode to round-to-nearest
  set_rounding_mode(this, Int256_fromUInt32(ModifierMemberValue_kv3_rounding_RN));

  // double-double is enough to encode exactly the sum of 8 products
  // of float16_t. To perform the correct addition we need multiple add3
  float32_t products[8];
  for (i = 0; i < 8; i++) {
      // conversions from f16 to f32 for each product operands is exact
      // multiplication between two f32 encodings f16 values is also exact
      products[i] = f32_mul(f16_to_f32(fopnd1[i]), f16_to_f32(fopnd2[i]));
      if (f32_isInf(products[i])) {
          any_inf = 1;
      }
      if (f32_isNaN(products[i])) {
          any_nan = 1;
      }
      if (f16_isNaN(fopnd1[i]) || f16_isNaN(fopnd2[i])) {
          any_inputs_nan = 1;
      }
      if (f16_isSigNaN(fopnd1[i]) || f16_isSigNaN(fopnd2[i])) {
          any_inputs_snan = 1;
      }
      all_zero = all_zero && f32_eq(zero, products[i]);
  }
  if (any_inf || any_nan) {
      // Result is already accumulator value
      for (i = 0; i < 8; i++) {
          result = f32_add(products[i], result);
      }
      // operation with Inf or NaN does not raise inexact flags
      // so if raised arbitrarily by the above sum (which can add non special
      // values before adding special values) we must clear it
      if (!(save_flags & softfloat_flag_inexact)) softfloat_exceptionFlags &= ~softfloat_flag_inexact;
      // if there is a NaN within the number, and no sNaN we should not raise an invalid
      // exception (quiet NaN overcome possible invalid operations)
      if (!(save_flags & softfloat_flag_invalid) && (any_inputs_nan && !any_inputs_snan)) softfloat_exceptionFlags &= ~softfloat_flag_invalid;

  } else if (all_zero) {
    // when all sum operands are zero, the sign is determined by addition rules
    set_rounding_mode(this, rounding_mode);
    for (i = 0; i < 8; i++) {
        result = f32_add(products[i], result);
    }
  } else {
      // c_2p40 is used to anchor the high part of the double-double
      // accumulator
      const float64_t c_2p40 = {0x4270000000000000}; // 2^^40 >> max fp16 x fp16 product exponent
      float64_t hi = c_2p40;
      float64_t lo = {0};
      for (i = 0; i < 8; i++) {
          float64_t pre_lo;
          float64_t pre_hi = hi;
          Two_Sum_f64(pre_hi, f32_to_f64(products[i]), hi, pre_lo);
          // because of hi anchoring, pre_lo/lo <= 2^(40-53) = 2^-13
          // because fp16 products cannot have any digit with weigth less than 2^-48
          // they are higher than pre_lo/lo minimal digit weigth of -13-52 = -65
          lo = f64_add(pre_lo, lo);
          //Clean inexact flag as this addition should be infinite precision
          if (!(save_flags & softfloat_flag_inexact)) softfloat_exceptionFlags &= ~softfloat_flag_inexact;
      }
      // removing the anchor constant
      hi = f64_sub(hi, c_2p40);

      // final 3-add with accumulator result
      // Implementing Boldo and Melquiond's 3-Sum algorithm based
      // on rounding-to-odd
      // (Handbook of Floating-point arithmetic, 2nd edition, p192)
      float64_t uh, ul;
      Two_Sum_f64(hi, lo, uh, ul);
      float64_t th, tl;
      Two_Sum_f64(f32_to_f64(result), uh, th, tl);
      int rne_flag = is_round_to_nearest(rounding_mode);
      if (rne_flag) softfloat_roundingMode = softfloat_round_odd;
      if (!rne_flag) set_rounding_mode(this, rounding_mode);
      float64_t v = f64_add(tl, ul);
      float64_t f64_result = f64_add(th, v);
      // restoring rounding-mode
      set_rounding_mode(this, rounding_mode);
      // +0 - +0 = +0 + (-0) = 0 with correct sign for full cancellation cases
      // see T7609
      if (f64_eq(f64_result,zero64)) {
          f64_result = f64_sub(zero64, zero64);
      }
      result = f64_to_f32(f64_result);
  }
  result = f32_isNaN(result) ? f32_defaultNaN : result;

  return Int256_fromUInt32(result.v);
}

/** fdot8 version with reset of IEEE flags */
static inline Int256_
Behavior_fdot8add_16_32_reset(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  return Behavior_fdot8add_16_32_helper(this, rounding_mode, opnd1, opnd2, opnd3);
}

/** fdot8 version without reset of IEEE flags */
static inline Int256_
Behavior_fdot8add_16_32_noreset(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_fdot8add_16_32_helper(this, rounding_mode, opnd1, opnd2, opnd3);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline float32_t
f32_dualMulDualAdd(float32_t fopnd1_0, float32_t fopnd2_0,
                   float32_t fopnd1_1, float32_t fopnd2_1, float32_t *_fopnd3)
{
  unsigned saved_roundingMode = softfloat_roundingMode;
  // exact conversions
  float64_t f64opnd1_0 = f32_to_f64(fopnd1_0);
  float64_t f64opnd1_1 = f32_to_f64(fopnd1_1);
  float64_t f64opnd2_0 = f32_to_f64(fopnd2_0);
  float64_t f64opnd2_1 = f32_to_f64(fopnd2_1);
  float64_t f64opnd3 = {0};
  if (_fopnd3)
    f64opnd3= f32_to_f64(*_fopnd3);
#if 1
  // TODO/FIXME: exception and special cases are not managed yet
  // exact product
  float64_t fprod0 = f64_mul(f64opnd1_0, f64opnd2_0);
  float64_t fprod1 = f64_mul(f64opnd1_1, f64opnd2_1);
  // Boldo and Melquiond's 3-Sum algorithm based on round-to-odd
  // (Handbook of Floating-point arithmetic, 2nd edition, p192)
  // Force rounding mode to round-to-nearest-even for the Two_Sum
  softfloat_roundingMode = softfloat_round_near_even;
  float64_t uh, ul;
  Two_Sum_f64(fprod0, fprod1, uh, ul);
  float64_t th, tl;
  Two_Sum_f64(f64opnd3, uh, th, tl);
  // If round-to-nearest-even, force round-to-odd for the two adds
  if (saved_roundingMode == softfloat_round_near_even)
    softfloat_roundingMode = softfloat_round_odd;
  // Else use the requested rounding mode for the two adds
  else
    softfloat_roundingMode = saved_roundingMode;
  float64_t v = f64_add(tl, ul);
  float64_t f64result = f64_add(th, v);
#else
  float64_t f64result = f64_mulAdd(f64opnd1_1, f64opnd2_1, f64opnd3);
  f64result = f64_mulAdd(f64opnd1_0, f64opnd2_0, f64result);
  fprintf(stderr, "FDMDA((%g*%g)+(%g*%g)+(%g)) = %g\n", *(double*)&f64opnd1_0, *(double*)&f64opnd2_0,
          *(double*)&f64opnd1_1, *(double*)&f64opnd2_1, *(double*)&f64opnd3, *(double*)&f64result);
#endif
  // Always use the requested rounding mode for the last conversion
  softfloat_roundingMode = saved_roundingMode;
  float32_t f32result = f64_to_f32(f64result);
  return f32result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline float32_t
f32_dualMulAdd(float32_t fopnd1_0, float32_t fopnd2_0,
               float32_t fopnd1_1, float32_t fopnd2_1)
{
  return f32_dualMulDualAdd(fopnd1_0, fopnd2_0, fopnd1_1, fopnd2_1, 0);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffdma_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t *fopnd1 = (float32_t *)&opnd1.words[0];
  float32_t *fopnd2 = (float32_t *)&opnd2.words[0];
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = f32_dualMulAdd(fopnd1[i], fopnd2[i], fopnd1[i+n], fopnd2[i+n]).v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdma_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_ffdma_32_32_(this, rounding_mode, opnd1, opnd2, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdma_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_ffdma_32_32_(this, rounding_mode, opnd1, opnd2, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdma_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_ffdma_32_32_(this, rounding_mode, opnd1, opnd2, 4);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffdms_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t *fopnd1 = (float32_t *)&opnd1.words[0];
  float32_t *fopnd2 = (float32_t *)&opnd2.words[0];
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = f32_dualMulAdd(fopnd1[i], fopnd2[i], f32_neg(fopnd1[i+n]), fopnd2[i+n]).v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdms_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_ffdms_32_32_(this, rounding_mode, opnd1, opnd2, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdms_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_ffdms_32_32_(this, rounding_mode, opnd1, opnd2, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdms_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_ffdms_32_32_(this, rounding_mode, opnd1, opnd2, 4);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmda_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t *fopnd1 = (float32_t *)&opnd1.words[0];
  float32_t *fopnd2 = (float32_t *)&opnd2.words[0];
  float32_t *fopnd3 = (float32_t *)&opnd3.words[0];
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = f32_dualMulDualAdd(fopnd1[i], fopnd2[i], fopnd1[i+n], fopnd2[i+n], fopnd3+i).v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmda_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmda_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmda_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmda_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmda_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmda_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 4);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmsa_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t *fopnd1 = (float32_t *)&opnd1.words[0];
  float32_t *fopnd2 = (float32_t *)&opnd2.words[0];
  float32_t *fopnd3 = (float32_t *)&opnd3.words[0];
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = f32_dualMulDualAdd(f32_neg(fopnd1[i]), fopnd2[i], fopnd1[i+n], fopnd2[i+n], fopnd3+i).v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmsa_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmsa_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmsa_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmsa_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmsa_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmsa_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 4);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmds_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t *fopnd1 = (float32_t *)&opnd1.words[0];
  float32_t *fopnd2 = (float32_t *)&opnd2.words[0];
  float32_t *fopnd3 = (float32_t *)&opnd3.words[0];
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = f32_dualMulDualAdd(f32_neg(fopnd1[i]), fopnd2[i], f32_neg(fopnd1[i+n]), fopnd2[i+n], fopnd3+i).v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmds_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmds_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmds_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmds_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmds_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmds_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 4);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmas_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  float32_t *fopnd1 = (float32_t *)&opnd1.words[0];
  float32_t *fopnd2 = (float32_t *)&opnd2.words[0];
  float32_t *fopnd3 = (float32_t *)&opnd3.words[0];
  Int256_ result = Int256_zero;
  for (int i = 0; i < n; i++) {
    result.words[i] = f32_dualMulDualAdd(fopnd1[i], fopnd2[i], f32_neg(fopnd1[i+n]), fopnd2[i+n], fopnd3+i).v;
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmas_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmas_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmas_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmas_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 2);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffdmas_32_32_x4(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffdmas_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 4);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fmm212_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  int t = 0; // Transpose if one.
  float32_t *fopnd1 = (float32_t *)opnd1.words;
  float32_t *fopnd2 = (float32_t *)opnd2.words;
  float32_t *fresult = (float32_t *)result.words;
  fresult[0] = f32_mul(fopnd1[0], fopnd2[0]);
  fresult[1+t] = f32_mul(fopnd1[0], fopnd2[1]);
  fresult[2-t] = f32_mul(fopnd1[1], fopnd2[0]);
  fresult[3] = f32_mul(fopnd1[1], fopnd2[1]);
  fresult[0] = fixupNaN_f32_f32_f32(fresult[0], fopnd1[0], fopnd2[0]);
  fresult[1+t] = fixupNaN_f32_f32_f32(fresult[1+t], fopnd1[0], fopnd2[1]);
  fresult[2-t] = fixupNaN_f32_f32_f32(fresult[2-t], fopnd1[1], fopnd2[0]);
  fresult[3] = fixupNaN_f32_f32_f32(fresult[3], fopnd1[1], fopnd2[1]);
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmm222_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  int t1 = !(opnd3.words[0] & 0x1);
  int t2 = (opnd3.words[0]>>1) & 0x1;
  if (t1) opnd1 = Behavior_transpose_32_2x2(this, opnd1);
  if (t2) opnd2 = Behavior_transpose_32_2x2(this, opnd2);
  float32_t *fopnd1 = (float32_t *)opnd1.words;
  float32_t *fopnd2 = (float32_t *)opnd2.words;
  float32_t *fresult = (float32_t *)result.words;
  fresult[0] = f32_dualMulAdd(fopnd1[0], fopnd2[0], fopnd1[2], fopnd2[2]);
  fresult[1] = f32_dualMulAdd(fopnd1[0], fopnd2[1], fopnd1[2], fopnd2[3]);
  fresult[2] = f32_dualMulAdd(fopnd1[1], fopnd2[0], fopnd1[3], fopnd2[2]);
  fresult[3] = f32_dualMulAdd(fopnd1[1], fopnd2[1], fopnd1[3], fopnd2[3]);
  fresult[0] = fixupNaN_f32_f32_f32_f32_f32(fresult[0], fopnd1[0], fopnd2[0], fopnd1[2], fopnd2[2]);
  fresult[1] = fixupNaN_f32_f32_f32_f32_f32(fresult[1], fopnd1[0], fopnd2[1], fopnd1[2], fopnd2[3]);
  fresult[2] = fixupNaN_f32_f32_f32_f32_f32(fresult[2], fopnd1[1], fopnd2[0], fopnd1[3], fopnd2[2]);
  fresult[3] = fixupNaN_f32_f32_f32_f32_f32(fresult[3], fopnd1[1], fopnd2[1], fopnd1[3], fopnd2[3]);
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fmma212_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  int t = 0; // Transpose if one.
  float32_t *fopnd1 = (float32_t *)opnd1.words;
  float32_t *fopnd2 = (float32_t *)opnd2.words;
  float32_t *fopnd3 = (float32_t *)opnd3.words;
  float32_t *fresult = (float32_t *)result.words;
  fresult[0] = f32_mulAdd(fopnd1[0], fopnd2[0], fopnd3[0]);
  fresult[1+t] = f32_mulAdd(fopnd1[0], fopnd2[1], fopnd3[1+t]);
  fresult[2-t] = f32_mulAdd(fopnd1[1], fopnd2[0], fopnd3[2-t]);
  fresult[3] = f32_mulAdd(fopnd1[1], fopnd2[1], fopnd3[3]);
  fresult[0] = fixupNaN_f32_f32_f32_f32(fresult[0], fopnd1[0], fopnd2[0], fopnd3[0]);
  fresult[1+t] = fixupNaN_f32_f32_f32_f32(fresult[1+t], fopnd1[0], fopnd2[1], fopnd3[1+t]);
  fresult[2-t] = fixupNaN_f32_f32_f32_f32(fresult[2-t], fopnd1[1], fopnd2[0], fopnd3[2-t]);
  fresult[3] = fixupNaN_f32_f32_f32_f32(fresult[3], fopnd1[1], fopnd2[1], fopnd3[3]);
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmma222_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, Int256_ opnd4)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  int t1 = !(opnd4.words[0] & 0x1);
  int t2 = (opnd4.words[0]>>1) & 0x1;
  if (t1) opnd1 = Behavior_transpose_32_2x2(this, opnd1);
  if (t2) opnd2 = Behavior_transpose_32_2x2(this, opnd2);
  float32_t *fopnd1 = (float32_t *)opnd1.words;
  float32_t *fopnd2 = (float32_t *)opnd2.words;
  float32_t *fopnd3 = (float32_t *)opnd3.words;
  float32_t *fresult = (float32_t *)result.words;
  fresult[0] = f32_dualMulDualAdd(fopnd1[0], fopnd2[0], fopnd1[2], fopnd2[2], &fopnd3[0]);
  fresult[1] = f32_dualMulDualAdd(fopnd1[0], fopnd2[1], fopnd1[2], fopnd2[3], &fopnd3[1]);
  fresult[2] = f32_dualMulDualAdd(fopnd1[1], fopnd2[0], fopnd1[3], fopnd2[2], &fopnd3[2]);
  fresult[3] = f32_dualMulDualAdd(fopnd1[1], fopnd2[1], fopnd1[3], fopnd2[3], &fopnd3[3]);
  fresult[0] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[0], fopnd1[0], fopnd2[0], fopnd1[2], fopnd2[2], fopnd3[0]);
  fresult[1] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[1], fopnd1[0], fopnd2[1], fopnd1[2], fopnd2[3], fopnd3[1]);
  fresult[2] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[2], fopnd1[1], fopnd2[0], fopnd1[3], fopnd2[2], fopnd3[2]);
  fresult[3] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[3], fopnd1[1], fopnd2[1], fopnd1[3], fopnd2[3], fopnd3[3]);
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fmms212_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  int t = 0; // Transpose if one.
  float32_t *fopnd1 = (float32_t *)opnd1.words;
  float32_t *fopnd2 = (float32_t *)opnd2.words;
  float32_t *fopnd3 = (float32_t *)opnd3.words;
  float32_t *fresult = (float32_t *)result.words;
  fresult[0] = f32_mulSub(fopnd1[0], fopnd2[0], fopnd3[0]);
  fresult[1+t] = f32_mulSub(fopnd1[0], fopnd2[1], fopnd3[1+t]);
  fresult[2-t] = f32_mulSub(fopnd1[1], fopnd2[0], fopnd3[2-t]);
  fresult[3] = f32_mulSub(fopnd1[1], fopnd2[1], fopnd3[3]);
  fresult[0] = fixupNaN_f32_f32_f32_f32(fresult[0], fopnd1[0], fopnd2[0], fopnd3[0]);
  fresult[1+t] = fixupNaN_f32_f32_f32_f32(fresult[1+t], fopnd1[0], fopnd2[1], fopnd3[1+t]);
  fresult[2-t] = fixupNaN_f32_f32_f32_f32(fresult[2-t], fopnd1[1], fopnd2[0], fopnd3[2-t]);
  fresult[3] = fixupNaN_f32_f32_f32_f32(fresult[3], fopnd1[1], fopnd2[1], fopnd3[3]);
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fmms222_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, Int256_ opnd4)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  int t1 = !(opnd4.words[0] & 0x1);
  int t2 = (opnd4.words[0]>>1) & 0x1;
  if (t1) opnd1 = Behavior_transpose_32_2x2(this, opnd1);
  if (t2) opnd2 = Behavior_transpose_32_2x2(this, opnd2);
  float32_t *fopnd1 = (float32_t *)opnd1.words;
  float32_t *fopnd2 = (float32_t *)opnd2.words;
  float32_t *fopnd3 = (float32_t *)opnd3.words;
  float32_t *fresult = (float32_t *)result.words;
  fresult[0] = f32_dualMulDualAdd(f32_neg(fopnd1[0]), fopnd2[0], f32_neg(fopnd1[2]), fopnd2[2], &fopnd3[0]);
  fresult[1] = f32_dualMulDualAdd(f32_neg(fopnd1[0]), fopnd2[1], f32_neg(fopnd1[2]), fopnd2[3], &fopnd3[1]);
  fresult[2] = f32_dualMulDualAdd(f32_neg(fopnd1[1]), fopnd2[0], f32_neg(fopnd1[3]), fopnd2[2], &fopnd3[2]);
  fresult[3] = f32_dualMulDualAdd(f32_neg(fopnd1[1]), fopnd2[1], f32_neg(fopnd1[3]), fopnd2[3], &fopnd3[3]);
  fresult[0] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[0], fopnd1[0], fopnd2[0], fopnd1[2], fopnd2[2], fopnd3[0]);
  fresult[1] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[1], fopnd1[0], fopnd2[1], fopnd1[2], fopnd2[3], fopnd3[1]);
  fresult[2] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[2], fopnd1[1], fopnd2[0], fopnd1[3], fopnd2[2], fopnd3[2]);
  fresult[3] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[3], fopnd1[1], fopnd2[1], fopnd1[3], fopnd2[3], fopnd3[3]);
  return result;
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_fconj_32_32_(void *this, Int256_ opnd1, int n)
{
  Int256_ result = opnd1;
  float32_t *fopnd1 = (float32_t *)opnd1.words;
  float32_t *fresult = (float32_t *)result.words;
  for (int i = 0; i < n; i++) {
    fresult[2*i+1] = f32_neg(fopnd1[2*i+1]);
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fconj_32_32(void *this, Int256_ opnd1)
{
  return Behavior_fconj_32_32_(this, opnd1, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_fconj_32_32_x2(void *this, Int256_ opnd1)
{
  return Behavior_fconj_32_32_(this, opnd1, 2);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef TBD// $XCC__h
static inline Int256_
Behavior_fmulc_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  float32_t *fopnd1 = (float32_t *)opnd1.words;
  float32_t *fopnd2 = (float32_t *)opnd2.words;
  float32_t *fresult = (float32_t *)result.words;
  for (int i = 0; i < n; i++) {
    fresult[2*i+0] = f32_dualMulAdd(fopnd1[2*i+0], fopnd2[2*i+0], f32_neg(fopnd1[2*i+1]), fopnd2[2*i+1]);
    fresult[2*i+1] = f32_dualMulAdd(fopnd1[2*i+0], fopnd2[2*i+1], fopnd1[2*i+1], fopnd2[2*i+0]);
    fresult[2*i+0] = fixupNaN_f32_f32_f32_f32_f32(fresult[2*i+0], fopnd1[2*i+0], fopnd2[2*i+0], fopnd1[2*i+1], fopnd2[2*i+1]);
    fresult[2*i+1] = fixupNaN_f32_f32_f32_f32_f32(fresult[2*i+1], fopnd1[2*i+0], fopnd2[2*i+1], fopnd1[2*i+1], fopnd2[2*i+0]);
  }
  return result;
}
#endif//$XCC__h

#ifdef TBD// $XCC__h
static inline Int256_
Behavior_fmulc_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmulc_32_32_(this, rounding_mode, opnd1, opnd2, 1);
}
#endif//$XCC__h

#ifdef TBD// $XCC__h
static inline Int256_
Behavior_fmulc_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2)
{
  return Behavior_fmulc_32_32_(this, rounding_mode, opnd1, opnd2, 2);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffmac_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  float32_t *fopnd1 = (float32_t *)opnd1.words;
  float32_t *fopnd2 = (float32_t *)opnd2.words;
  float32_t *fopnd3 = (float32_t *)opnd3.words;
  float32_t *fresult = (float32_t *)result.words;
  for (int i = 0; i < n; i++) {
    fresult[2*i+0] = f32_dualMulDualAdd(fopnd1[2*i+0], fopnd2[2*i+0], f32_neg(fopnd1[2*i+1]), fopnd2[2*i+1], &fopnd3[2*i+0]);
    fresult[2*i+1] = f32_dualMulDualAdd(fopnd1[2*i+0], fopnd2[2*i+1], fopnd1[2*i+1], fopnd2[2*i+0], &fopnd3[2*i+1]);
    fresult[2*i+0] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[2*i+0], fopnd1[2*i+0], fopnd2[2*i+0], fopnd1[2*i+1], fopnd2[2*i+1], fopnd3[2*i+0]);
    fresult[2*i+1] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[2*i+1], fopnd1[2*i+0], fopnd2[2*i+1], fopnd1[2*i+1], fopnd2[2*i+0], fopnd3[2*i+1]);
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffmac_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffmac_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffmac_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffmac_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 2);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static inline Int256_
Behavior_ffmsc_32_32_(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3, int n)
{
  reset_exceptions(this);
  set_rounding_mode(this, rounding_mode);
  Int256_ result = Int256_zero;
  float32_t *fopnd1 = (float32_t *)opnd1.words;
  float32_t *fopnd2 = (float32_t *)opnd2.words;
  float32_t *fopnd3 = (float32_t *)opnd3.words;
  float32_t *fresult = (float32_t *)result.words;
  for (int i = 0; i < n; i++) {
    fresult[2*i+0] = f32_dualMulDualAdd(fopnd1[2*i+0], f32_neg(fopnd2[2*i+0]), f32_neg(fopnd1[2*i+1]), f32_neg(fopnd2[2*i+1]), &fopnd3[2*i+0]);
    fresult[2*i+1] = f32_dualMulDualAdd(fopnd1[2*i+0], f32_neg(fopnd2[2*i+1]), fopnd1[2*i+1], f32_neg(fopnd2[2*i+0]), &fopnd3[2*i+1]);
    fresult[2*i+0] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[2*i+0], fopnd1[2*i+0], fopnd2[2*i+0], fopnd1[2*i+1], fopnd2[2*i+1], fopnd3[2*i+0]);
    fresult[2*i+1] = fixupNaN_f32_f32_f32_f32_f32_f32(fresult[2*i+1], fopnd1[2*i+0], fopnd2[2*i+1], fopnd1[2*i+1], fopnd2[2*i+0], fopnd3[2*i+1]);
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffmsc_32_32(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffmsc_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_ffmsc_32_32_x2(void *this, Int256_ rounding_mode, Int256_ opnd1, Int256_ opnd2, Int256_ opnd3)
{
  return Behavior_ffmsc_32_32_(this, rounding_mode, opnd1, opnd2, opnd3, 2);
}
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/** Macro to manage shift larger than 32 */
#define LARGE_BOUNDED_USRL(value, shift, bound, sat) \
    ((shift) >= (bound) ? (sat) : ((value) >> shift))

#define STICKY_32B(value, shift) ((shift) > 1 ? ((value) << (32 + 1 - (shift))) : 0)
#define FRAC_32B(value, shift) ((shift) ? ((value) << (32  - (shift))) : 0)

static inline Int256_
Behavior_round_32_x8(void *this, Int256_ roundint, Int256_ dropbits, Int256_ opnd1)
{
  int i;
  uint32_t uroundint = Int256_toUInt32(roundint);
  Int256_ result = Int256_zero;
  unsigned shift = Int256_toUInt32(dropbits) & 0x1f;
  int32_t offset = (shift? (1 << (shift - 1)): 0);
  for (i = 0; i < 8; i++) {
    int32_t value = opnd1.words[i];
    // Compute (value + offset) >> shift while avoiding overflow.
    switch (uroundint) {
    case ModifierMemberValue_kv3_roundint_RHU:
        result.words[i] = ((value & ~offset) >> shift) + ((value & offset) != 0);
        break;
    case ModifierMemberValue_kv3_roundint_RZ: {
        if (value >= 0) result.words[i] = ((value & ~offset) >> shift);
        else result.words[i] = (value >> shift) + (FRAC_32B(value, shift) != 0);
        break;
    };
    case ModifierMemberValue_kv3_roundint_RD:
        result.words[i] = ((value & ~offset) >> shift);
        break;
    case ModifierMemberValue_kv3_roundint_RU:
        result.words[i] = (value >> shift) + (FRAC_32B(value, shift) != 0);
        break;
    case ModifierMemberValue_kv3_roundint_RN:
        result.words[i] = ((value & ~offset) >> shift) + \
                           ((value & offset) != 0 && (STICKY_32B(value, shift) != 0 || ((value >> shift) & 1) != 0));
        break;
    default:
        // error case
        // TODO/FIXME: handle error properly
        result.words[i] = -1;
        break;
    }
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h

/** +1 is added to shift because we must discard both significant and round bits */
#define STICKY_64B(value, shift) ((shift) > 1 ? ((value) << (64 + 1 - (shift))) : 0)
#define FRAC_64B(value, shift) (shift ? value << (64 - shift): 0)

static inline Int256_
Behavior_round_64_x4(void *this, Int256_ roundint, Int256_ dropbits, Int256_ opnd1)
{
  int i;
  Int256_ result = Int256_zero;
  uint32_t uroundint = Int256_toUInt32(roundint);
  // limiting shift amount to 6 bits
  unsigned shift = Int256_toUInt32(dropbits) & 0x3f;
  // if rounding mode is RHU then add a half ulp to round to nearest UP
  int64_t offset = (shift? (1LL << (shift - 1)): 0);
  for (i = 0; i < 4; i++) {
    int64_t value = opnd1.dwords[i];
    // Compute (value + offset) >> shift while avoiding overflow.
    switch (uroundint) {
    case ModifierMemberValue_kv3_roundint_RHU:
        result.dwords[i] = ((value & ~offset) >> shift) + ((value & offset) != 0);
        break;
    case ModifierMemberValue_kv3_roundint_RZ: {
        if (value > 0) result.dwords[i] = (value >> shift);
        else result.dwords[i] = (value >> shift) + (FRAC_64B(value, shift) != 0 );
        break;
    }
    case ModifierMemberValue_kv3_roundint_RD:
        result.dwords[i] = (value >> shift);
        break;
    case ModifierMemberValue_kv3_roundint_RU:
        result.dwords[i] = ((value & ~offset) >> shift) + (FRAC_64B(value, shift) != 0);
        break;
    case ModifierMemberValue_kv3_roundint_RN:
        result.dwords[i] = (value  >> shift) + \
                           ((value & offset) != 0 && (STICKY_64B(value, shift) != 0 || ((value >> shift) & 1) != 0));
        break;
    default:
        // error case
        // TODO/FIXME: handle error properly
        result.dwords[i] = -1;
        break;
    }
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_satu_32_8_x8(void *this, Int256_ opnd1)
{
  int i;
  Int256_ result = Int256_zero;
  for (i = 0; i < 8; i++) {
    int32_t value = opnd1.words[i];
    result.bytes[i] = value>0xFF? 0xFF: (value < 0? 0: value);
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_satu_64_16_x4(void *this, Int256_ opnd1)
{
  int i;
  Int256_ result = Int256_zero;
  for (i = 0; i < 4; i++) {
    int64_t value = opnd1.dwords[i];
    result.hwords[i] = value>0xFFFFLL? 0xFFFFLL: (value < 0? 0: value);
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_sat_32_8_x8(void *this, Int256_ opnd1)
{
  int i;
  Int256_ result = Int256_zero;
  for (i = 0; i < 8; i++) {
    int32_t value = opnd1.words[i];
    result.bytes[i] = value>0x7F? 0x7F: (value < -0x80? -0x80: value);
  }
  return result;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline Int256_
Behavior_sat_64_16_x4(void *this, Int256_ opnd1)
{
  int i;
  Int256_ result = Int256_zero;
  for (i = 0; i < 4; i++) {
    int64_t value = opnd1.dwords[i];
    result.hwords[i] = value>0x7FFFLL? 0x7FFFLL: (value < -0x8000? -0x8000: value);
  }
  return result;
}
#endif//$XCC__h

