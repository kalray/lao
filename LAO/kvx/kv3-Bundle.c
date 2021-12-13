#ifdef $XCC_h
/*
 * kv3-Bundle.c
 *
 * Copyright 2015 Kalray.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#include "MDT/MDT_.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#define BundleMatch_ENABLE 0
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

// Steering values for the kv3 VLIW architecture.
typedef enum {
  Steering_BCU,
  Steering_LSU,
  Steering_MAU,
  Steering_ALU,
  Steering__
} enum_Steering;
typedef uint8_t Steering;

// Extension values a.k.a. sub-steering values.
typedef enum {
  Extension_ALU0,
  Extension_ALU1,
  Extension_MAU,
  Extension_LSU,
  Extension__
} enum_Extension;
typedef uint8_t Extension;

#ifdef $XCC__h
#if Bundle_INSTRUCTIONS_MAX
/*
 * BundleIssue enumeration.
 */
typedef enum {
  BundleIssue_BCU,
  BundleIssue_TCA,
  BundleIssue_ALU0,
  BundleIssue_ALU1,
  BundleIssue_MAU,
  BundleIssue_LSU,
  BundleIssue__,
} enum_BundleIssue;
typedef uint8_t BundleIssue;
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__h

#ifdef $XCC__h
#if Bundle_INSTRUCTIONS_MAX
#define BundleEncoded_WORDCOUNT_MAX  ((Template_INCREMENT_MAX + sizeof(uint32_t) - 1)/sizeof(uint32_t))
struct BundleEncoded_ {
  //@args Processor processor, bool mode64
  BundleSlot_ SLOTS[BundleIssue__];
  uint32_t WORDS[BundleEncoded_WORDCOUNT_MAX];
  uint8_t WORDCOUNT; // Count of valid WORDS.
  Processor PROCESSOR;
  uint8_t MODE64;
  uint8_t OPXD;
};
#endif//Bundle_INSTRUCTIONS_MAX
#endif//$XCC__h

BundleEncoded
BundleEncoded_Ctor(BundleEncoded this,
                   Processor processor, bool mode64)
{
  memset(this, 0, sizeof(BundleEncoded_));
  *BundleEncoded__PROCESSOR(this) = processor;
  *BundleEncoded__MODE64(this) = mode64;
  return this;
}

#ifdef $XCC__c
{
#if Bundle_INSTRUCTIONS_MAX
  fprintf(stderr, "sizeof(BundleEncoded_)\t%zu\n", sizeof(BundleEncoded_));
#endif//Bundle_INSTRUCTIONS_MAX
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * An IMMX syllable is associated with the BundleIssue Extension_BundleIssue[extension].
 */
static const BundleIssue
Extension_BundleIssue[] = { BundleIssue_ALU0, BundleIssue_ALU1, BundleIssue_MAU, BundleIssue_LSU };

static inline int
kv3_steering(uint32_t x)
{
  return (((x) & 0x60000000) >> 29);
}

static inline int
kv3_extension(uint32_t x)
{
  return  (((x) & 0x18000000) >> 27);
}

static inline int
kv3_has_parallel_bit(uint32_t x)
{
  return (((x) & 0x80000000) == 0x80000000);
}

static inline int
kv3_is_tca_opcode(uint32_t x)
{
  unsigned major = ((x)>>24) & 0x1F;
  return (major > 1) && (major < 8);
}

static inline int
kv3_is_nop_opcode(uint32_t x)
{
  return ((x)<<1) == 0xFFFFFFFE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int
BundleEncoded_gather_kv3_v1(BundleEncoded this);
int
BundleEncoded_gather_kv3_v2(BundleEncoded this);

/*
 * BundleEncoded_gather -- Gather the BundleEncoded syllables.
 *
 * |this| BundleEncoded where to gather.
 * |words| Array of code words to read from.
 * |count| Count of available code words.
 * |return| The number of |words| consumed or <= 0 if error.
 */
int
BundleEncoded_gather(BundleEncoded this, const uint32_t *words, int count)
{
  int index;
  if (count > BundleEncoded_WORDCOUNT_MAX) {
    count = BundleEncoded_WORDCOUNT_MAX;
  }
  for (index = 0; index < count; index++) {
    uint32_t codeWord = words[index];
    BundleEncoded_WORDS(this)[index] = codeWord;
    if (!kv3_has_parallel_bit(codeWord)) break;
  }
  if (index++ < count) {
    // If index < count then we correctly exited from loop by break.
    *BundleEncoded__WORDCOUNT(this) = index;
    Processor processor = BundleEncoded_PROCESSOR(this);
    if (processor == Processor_v1_cpu) {
      return BundleEncoded_gather_kv3_v1(this);
    } 
    if (processor == Processor_v2_cpu) {
      return BundleEncoded_gather_kv3_v2(this);
    } 
    return 0;
  }
  return 0;
}

/*
 * BundleEncoded_transfer -- Transfer the BundleEncoded to OpcodeBuffer(s).
 *
 * |this| This BundleEncoded, after BundleEncoded_gather().
 * |opcodeBuffers| Fill one OpcodeBuffer_ per opcode.
 * |return| The total number of issued slots or <= 0 if error.
 *
 */
int
BundleEncoded_transfer(BundleEncoded this, OpcodeBuffer_ opcodeBuffers[])
{
  BundleSlot bundleSlots = BundleEncoded_SLOTS(this);
  int issued = 0;
  for (int issue = BundleIssue_BCU; issue < BundleIssue__; issue++) {
    if (bundleSlots[issue].DECODING) {
      opcodeBuffers[issued].WORDS[0] = bundleSlots[issue].MAINWORD;
      opcodeBuffers[issued].DECODING = bundleSlots[issue].DECODING;
      if (bundleSlots[issue].IMMXCOUNT > 0) {
        opcodeBuffers[issued].WORDS[1] = bundleSlots[issue].IMMXWORDS[0];
      }
      if (bundleSlots[issue].IMMXCOUNT > 1) {
        opcodeBuffers[issued].WORDS[2] = bundleSlots[issue].IMMXWORDS[1];
      }
      issued++;
    }
  }
  //
  return issued;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Function derived from BundleEncoded_gather_kv3_v1.
 */
int
BundleEncoded_gather_kv3_v1(BundleEncoded this)
{
  int bundleWordCount = BundleEncoded_WORDCOUNT(this);
  uint32_t *bundleWords = BundleEncoded_WORDS(this);
  BundleSlot bundleSlots = BundleEncoded_SLOTS(this);
  //
  int bcu_taken = 0;
  int tca_taken = 0;
  int alu0_taken = 0;
  int alu1_taken = 0;
  int mau_taken = 0;
  int lsu_taken = 0;
  //
  for (int index = 0; index < bundleWordCount ; index++) {
    uint32_t bundle_word = bundleWords[index];
    int steering = kv3_steering(bundle_word);
    if (steering == Steering_BCU) {
      if (index == 0) {
        // First syllable in bundle, is BCU or TCA.
        if (kv3_is_tca_opcode(bundle_word)) {
          bundleSlots[BundleIssue_TCA].MAINWORD = bundle_word;
          bundleSlots[BundleIssue_TCA].DECODING = Decoding_v1_simple;
          tca_taken = 1;
        } else {
          bundleSlots[BundleIssue_BCU].MAINWORD = bundle_word;
          bundleSlots[BundleIssue_BCU].DECODING = Decoding_v1_simple;
          bcu_taken = 1;
        }
      } else {
        // Not first syllable in bundle, is IMMX or TCA.
        if (index == 1 && bcu_taken && kv3_is_tca_opcode(bundle_word)) {
          // Error, TCA already in use.
          if (tca_taken) {
            return -BundleIssue_TCA;
          }
          bundleSlots[BundleIssue_TCA].MAINWORD = bundle_word;
          bundleSlots[BundleIssue_TCA].DECODING = Decoding_v1_simple;
          tca_taken = 1;
        } else {
          int issue = Extension_BundleIssue[kv3_extension(bundle_word)];
          int decoding = bundleSlots[issue].DECODING;
          // Error, issue main syllable not seen yet.
          if (decoding == 0) {
            return -BundleIssue_BCU;
          }
          int immxCount = bundleSlots[issue].IMMXCOUNT;
          bundleSlots[issue].IMMXWORDS[immxCount] = bundle_word;
          bundleSlots[issue].DECODING = immxCount < 1?
                                        Decoding_v1_double:
                                        Decoding_v1_triple;
          bundleSlots[issue].IMMXCOUNT = immxCount + 1;
        }
      }
    } else
    if (steering == Steering_ALU) {
      if (alu0_taken == 0) {
        bundleSlots[BundleIssue_ALU0].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_ALU0].DECODING = Decoding_v1_simple;
        alu0_taken = 1;
      } else if (alu1_taken == 0) {
        bundleSlots[BundleIssue_ALU1].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_ALU1].DECODING = Decoding_v1_simple;
        alu1_taken = 1;
      } else if (mau_taken == 0) {
        bundleSlots[BundleIssue_MAU].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_MAU].DECODING = Decoding_v1_simple;
        mau_taken = 1;
      } else if (lsu_taken == 0) {
        bundleSlots[BundleIssue_LSU].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_LSU].DECODING = Decoding_v1_simple;
        lsu_taken = 1;
      } else if (kv3_is_nop_opcode(bundle_word)) {
        // Discard the ALU NOPs at the end of bundle.
      } else {
        // Error, all ALUs already in use.
        return -BundleIssue_ALU0;
      }
    } else
    if (steering == Steering_MAU) {
      if (mau_taken == 1) {
        // Error, MAU already in use.
        return -BundleIssue_MAU;
      } else {
        bundleSlots[BundleIssue_MAU].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_MAU].DECODING = Decoding_v1_simple;
        mau_taken = 1;
      }
    } else
    if (steering == Steering_LSU) {
      if (lsu_taken == 1) {
        // Error, LSU already in use.
        return -BundleIssue_LSU;
      } else {
        bundleSlots[BundleIssue_LSU].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_LSU].DECODING = Decoding_v1_simple;
        lsu_taken = 1;
      }
    } else {
      // Error, unknown issue slot.
      return -BundleIssue__;
    }
  }
  //
  return (bcu_taken + tca_taken + alu0_taken) + (alu1_taken + mau_taken + lsu_taken);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Function derived from BundleEncoded_gather_kv3_v2.
 */
int
BundleEncoded_gather_kv3_v2(BundleEncoded this)
{
  int bundleWordCount = BundleEncoded_WORDCOUNT(this);
  uint32_t *bundleWords = BundleEncoded_WORDS(this);
  BundleSlot bundleSlots = BundleEncoded_SLOTS(this);
  //
  int bcu_taken = 0;
  int tca_taken = 0;
  int alu0_taken = 0;
  int alu1_taken = 0;
  int mau_taken = 0;
  int lsu_taken = 0;
  //
  for (int index = 0; index < bundleWordCount ; index++) {
    uint32_t bundle_word = bundleWords[index];
    int steering = kv3_steering(bundle_word);
    if (steering == Steering_BCU) {
      if (index == 0) {
        // First syllable in bundle, is BCU or TCA.
        if (kv3_is_tca_opcode(bundle_word)) {
          bundleSlots[BundleIssue_TCA].MAINWORD = bundle_word;
          bundleSlots[BundleIssue_TCA].DECODING = Decoding_v2_simple;
          tca_taken = 1;
        } else {
          bundleSlots[BundleIssue_BCU].MAINWORD = bundle_word;
          bundleSlots[BundleIssue_BCU].DECODING = Decoding_v2_simple;
          bcu_taken = 1;
        }
      } else {
        // Not first syllable in bundle, is IMMX or TCA.
        if (index == 1 && bcu_taken && kv3_is_tca_opcode(bundle_word)) {
          // Error, TCA already in use.
          if (tca_taken) {
            return -BundleIssue_TCA;
          }
          bundleSlots[BundleIssue_TCA].MAINWORD = bundle_word;
          bundleSlots[BundleIssue_TCA].DECODING = Decoding_v2_simple;
          tca_taken = 1;
        } else {
          int issue = Extension_BundleIssue[kv3_extension(bundle_word)];
          int decoding = bundleSlots[issue].DECODING;
          // Error, issue main syllable not seen yet.
          if (decoding == 0) {
            return -BundleIssue_BCU;
          }
          int immxCount = bundleSlots[issue].IMMXCOUNT;
          bundleSlots[issue].IMMXWORDS[immxCount] = bundle_word;
          bundleSlots[issue].DECODING = immxCount < 1?
                                        Decoding_v2_double:
                                        Decoding_v2_triple;
          bundleSlots[issue].IMMXCOUNT = immxCount + 1;
        }
      }
    } else
    if (steering == Steering_ALU) {
      if (alu0_taken == 0) {
        bundleSlots[BundleIssue_ALU0].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_ALU0].DECODING = Decoding_v2_simple;
        alu0_taken = 1;
      } else if (alu1_taken == 0) {
        bundleSlots[BundleIssue_ALU1].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_ALU1].DECODING = Decoding_v2_simple;
        alu1_taken = 1;
      } else if (mau_taken == 0) {
        bundleSlots[BundleIssue_MAU].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_MAU].DECODING = Decoding_v2_simple;
        mau_taken = 1;
      } else if (lsu_taken == 0) {
        bundleSlots[BundleIssue_LSU].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_LSU].DECODING = Decoding_v2_simple;
        lsu_taken = 1;
      } else if (kv3_is_nop_opcode(bundle_word)) {
        // Discard the ALU NOPs at the end of bundle.
      } else {
        // Error, all ALUs already in use.
        return -BundleIssue_ALU0;
      }
    } else
    if (steering == Steering_MAU) {
      if (mau_taken == 1) {
        // Error, MAU already in use.
        return -BundleIssue_MAU;
      } else {
        bundleSlots[BundleIssue_MAU].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_MAU].DECODING = Decoding_v2_simple;
        mau_taken = 1;
      }
    } else
    if (steering == Steering_LSU) {
      if (lsu_taken == 1) {
        // Error, LSU already in use.
        return -BundleIssue_LSU;
      } else {
        bundleSlots[BundleIssue_LSU].MAINWORD = bundle_word;
        bundleSlots[BundleIssue_LSU].DECODING = Decoding_v2_simple;
        lsu_taken = 1;
      }
    } else {
      // Error, unknown issue slot.
      return -BundleIssue__;
    }
  }
  //
  return (bcu_taken + tca_taken + alu0_taken) + (alu1_taken + mau_taken + lsu_taken);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

