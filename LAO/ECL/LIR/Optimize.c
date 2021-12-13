#ifdef $XCC_h
/*
 * !!!!	Optimize.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu).
 * Duco van Amstel (duco.van-amstel@kalray.eu)
 *
 * Copyright 2010 - 2013 Kalray
 * Copyright 2002 - 2007 STMicroelectronics.
 * Copyright 1995 - 1998 Commissariat a l'Energie Atomique.
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
 * @ingroup LIR
 * @brief Optimization flags.
 */
#endif//$XCC_h


#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/Optimize.h"
#endif//$XCC__h

#ifdef $XCC_h
typedef uint8_t OptimizeFlags;
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Activation of LAO phases.
 */
typedef enum {
  OptimizeActivation_Encode = 0x1,	// Instruction encoding.
  OptimizeActivation_PostPass = 0x2,	// Post-pass scheduling
  OptimizeActivation_RegAlloc = 0x4,	// Register allocation
  OptimizeActivation_PrePass = 0x8,	// Pre-pass scheduling
  OptimizeActivation_SSAForm = 0x10,	// SSA Form optimizations.
} enum_OptimizeActivation;
typedef uint8_t OptimizeActivation;
#define OptimizeActivation_default ( \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeActivation_Table[1];
static inline bool
OptimizeActivation_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeActivation_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeActivation_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeActivation_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeActivation flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeActivation_Entries[] = {
  ETableENTRY(OptimizeActivation, Encode),
  ETableENTRY(OptimizeActivation, PostPass),
  ETableENTRY(OptimizeActivation, RegAlloc),
  ETableENTRY(OptimizeActivation, PrePass),
  ETableENTRY(OptimizeActivation, SSAForm),
  ETableENTRY_NULL
};
ETable_ OptimizeActivation_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration of the code profiling levels.
 */
typedef enum {
  OptimizeProfiling_Static = 0x1,	// Static profiling
  OptimizeProfiling_Edge = 0x2,		// Dynamic edge profiling.
  OptimizeProfiling_Path = 0x4,		// Dynamic path profiling.
} enum_OptimizeProfiling;
typedef uint8_t OptimizeProfiling;
#define OptimizeProfiling_default ( \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeProfiling_Table[1];
static inline bool
OptimizeProfiling_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeProfiling_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeProfiling_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeProfiling_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeProfiling flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeProfiling_Entries[] = {
  ETableENTRY(OptimizeProfiling, Static),
  ETableENTRY(OptimizeProfiling, Edge),
  ETableENTRY(OptimizeProfiling, Path),
  ETableENTRY_NULL
};
ETable_ OptimizeProfiling_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration of the pre- and post-scheduler levels.
 */
typedef enum {
  OptimizeScheduling_Unwinding = 0x1,		// Unwinding modulo scheduling.
  OptimizeScheduling_Insertion = 0x2,	        // Insertion modulo scheduling.
  OptimizeScheduling_Iterative = 0x4,		// Iterative modulo scheduling.
  OptimizeScheduling__
} enum_OptimizeScheduling;
typedef uint8_t OptimizeScheduling;
#define OptimizeScheduling_default ( \
        OptimizeScheduling_Insertion | \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeScheduling_Table[1];
static inline bool
OptimizeScheduling_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeScheduling_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeScheduling_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeScheduling_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeScheduling flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeScheduling_Entries[] = {
  ETableENTRY(OptimizeScheduling, Unwinding),
  ETableENTRY(OptimizeScheduling, Insertion),
  ETableENTRY(OptimizeScheduling, Iterative),
  ETableENTRY_NULL
};
ETable_ OptimizeScheduling_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration of the register allocation types.
 */
typedef enum {
  OptimizeAllocation_ELinearScan = 0x1,		// Linear Scan of Sarkar & Barik.
  OptimizeAllocation_DomTreeScan = 0x2,		// Tree Scan of Rastello et al.
  OptimizeAllocation_GraphColoring = 0x4,	// Some Graph Coloring allocator.
  OptimizeAllocation_FromSSAForm = 0x10,	// Allocator input is from SSA Form.
} enum_OptimizeAllocation;
typedef uint8_t OptimizeAllocation;
#define OptimizeAllocation_default ( \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeAllocation_Table[1];
static inline bool
OptimizeAllocation_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeAllocation_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeAllocation_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeAllocation_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeAllocation flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeAllocation_Entries[] = {
  ETableENTRY(OptimizeAllocation, ELinearScan),
  ETableENTRY(OptimizeAllocation, DomTreeScan),
  ETableENTRY(OptimizeAllocation, GraphColoring),
  ETableENTRY(OptimizeAllocation, FromSSAForm),
  ETableENTRY_NULL
};
ETable_ OptimizeAllocation_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration of the register pressure lowering types.
 */
typedef enum {
  OptimizeSpilling_SebastianPhD = 0x1,		// Sebastian PhD algorithm.
  OptimizeSpilling_SebastianCC09 = 0x2,		// Sebastian CC09 algorithm.
  OptimizeSpilling_FlorentILP = 0x4,		// Florent ILP formulation.
} enum_OptimizeSpilling;
typedef uint8_t OptimizeSpilling;
#define OptimizeSpilling_default ( \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeSpilling_Table[1];
static inline bool
OptimizeSpilling_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeSpilling_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeSpilling_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeSpilling_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeSpilling flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeSpilling_Entries[] = {
  ETableENTRY(OptimizeSpilling, SebastianPhD),
  ETableENTRY(OptimizeSpilling, SebastianCC09),
  ETableENTRY(OptimizeSpilling, FlorentILP),
  ETableENTRY_NULL
};
ETable_ OptimizeSpilling_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumeration of the register coloring types.
 */
typedef enum {
  OptimizeColoring_ELinearScan = 0x1,		// Linear Scan of Sarkar & Barik.
  OptimizeColoring_DomTreeScan = 0x2,		// Tree Scan of Rastello et al.
  OptimizeColoring_GeorgeAppel = 0x4,		// George & Appel coloring.
} enum_OptimizeColoring;
typedef uint8_t OptimizeColoring;
#define OptimizeColoring_default ( \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeColoring_Table[1];
static inline bool
OptimizeColoring_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeColoring_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeColoring_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeColoring_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeColoring flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeColoring_Entries[] = {
  ETableENTRY(OptimizeColoring, ELinearScan),
  ETableENTRY(OptimizeColoring, DomTreeScan),
  ETableENTRY(OptimizeColoring, GeorgeAppel),
  ETableENTRY_NULL
};
ETable_ OptimizeColoring_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * SSA form conversion flags.
 */
typedef enum {
  OptimizeConversion_Folding = 0x1,	//!< Copy Folding during SSA construction.
  OptimizeConversion_Cleaning = 0x2,	//!< Code Cleaning during SSA construction.
  OptimizeConversion_SemiPruned = 0x4,	//!< Semi-pruned improved SSA construction.
  OptimizeConversion_SigmaGoTo = 0x8,	//!< Sigma operations for the GoTo conditions.
  OptimizeConversion_Dedicated = 0x10,	//!< Associate Variable(s) to Dedicated.
} enum_OptimizeConversion;
typedef uint8_t OptimizeConversion;
#define OptimizeConversion_default ( \
        OptimizeConversion_Folding | \
        OptimizeConversion_Cleaning | \
        OptimizeConversion_SemiPruned | \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeConversion_Table[1];
static inline bool
OptimizeConversion_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeConversion_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeConversion_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeConversion_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeConversion flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeConversion_Entries[] = {
  ETableENTRY(OptimizeConversion, Folding),
  ETableENTRY(OptimizeConversion, Cleaning),
  ETableENTRY(OptimizeConversion, SemiPruned),
  ETableENTRY(OptimizeConversion, SigmaGoTo),
  ETableENTRY(OptimizeConversion, Dedicated),
  ETableENTRY_NULL
};
ETable_ OptimizeConversion_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * SSA form coalescing flags.
 */
typedef enum {
  OptimizeCoalescing_Sreedhar = 0x1,	//!< Sreedhar CSSA conversion.
  OptimizeCoalescing_Boissinot = 0x2,	//!< Boissinot CSSA conversion.
  OptimizeCoalescing_Decoalesce = 0x4,	//!< Decoalesce the SSA-webs (Budimlic).
  OptimizeCoalescing_EarlyPrune = 0x8,	//!< Early prune phi-interferences (Method II)
  OptimizeCoalescing_Virtualize = 0x10,	//!< Virtualize COPY insertion (Method III).
  OptimizeCoalescing_Congruence = 0x20,	//!< Congruence-based coalescing.
  OptimizeCoalescing_SeqCopies = 0x40,	//!< Use sequential copies (parallel by default).
  OptimizeCoalescing_Florent = 0x80,	//!< Use coalescing by Florent.
} enum_OptimizeCoalescing;
typedef uint8_t OptimizeCoalescing;
#define OptimizeCoalescing_default ( \
        OptimizeCoalescing_Sreedhar | \
        OptimizeCoalescing_Virtualize | \
        OptimizeCoalescing_Congruence | \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeCoalescing_Table[1];
static inline bool
OptimizeCoalescing_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeCoalescing_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeCoalescing_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeCoalescing_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeCoalescing flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeCoalescing_Entries[] = {
  ETableENTRY(OptimizeCoalescing, Sreedhar),
  ETableENTRY(OptimizeCoalescing, Boissinot),
  ETableENTRY(OptimizeCoalescing, Decoalesce),
  ETableENTRY(OptimizeCoalescing, EarlyPrune),
  ETableENTRY(OptimizeCoalescing, Virtualize),
  ETableENTRY(OptimizeCoalescing, Congruence),
  ETableENTRY(OptimizeCoalescing, SeqCopies),
  ETableENTRY(OptimizeCoalescing, Florent),
  ETableENTRY_NULL
};
ETable_ OptimizeCoalescing_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Value Numbering flags.
 */
typedef enum {
  OptimizeNumbering_Basic = 0x1,	// Basic value numbering.
  OptimizeNumbering_SCCVN = 0x2,	// Simpson's SCCVN improved by Cooper.
  OptimizeNumbering_Avail = 0x4,	// AVAIL-based removal (else use Dominance).
} enum_OptimizeNumbering;
typedef uint8_t OptimizeNumbering;
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Instruction Rewriting flags.
 */
typedef enum {
  OptimizeRewriting_Range = 0x1,	// Exploit value ranges
  OptimizeRewriting_BWLU = 0x80,	// Bit-Sliced Logic Unit.
} enum_OptimizeRewriting;
typedef uint8_t OptimizeRewriting;
#define OptimizeRewriting_default ( \
        OptimizeRewriting_BWLU | \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeRewriting_Table[1];
static inline bool
OptimizeRewriting_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeRewriting_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeRewriting_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeRewriting_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeRewriting flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeRewriting_Entries[] = {
  ETableENTRY(OptimizeRewriting, Range),
  ETableENTRY(OptimizeRewriting, BWLU),
  ETableENTRY_NULL
};
ETable_ OptimizeRewriting_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Data-flow facts Propagation flags.
 */
typedef enum {
  OptimizePropagation_Constant = 0x1,	// Sparse conditional constant propagation.
} enum_OptimizePropagation;
typedef uint8_t OptimizePropagation;
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Code Predication level.
 */
typedef enum {
  OptimizePredication_None,		// Disable code predication.
  OptimizePredication_Select,		// Select-only code predication.
  OptimizePredication__
} enum_OptimizePredication;
typedef uint8_t OptimizePredication;
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerate the RCMSSolving flags.
 */
typedef enum {
  OptimizeRCMSSolving_Dinechin = 0x1,		// Formulation of Dinechin.
  OptimizeRCMSSolving_LNSScan1 = 0x2,		// LSB of LNS scanning value.
  OptimizeRCMSSolving_LNSScan2 = 0x4,		// MSB of LNS scanning value.
  OptimizeRCMSSolving_EicDavAb = 0x8,		// Formulation of Eichenberger et al.
  OptimizeRCMSSolving_AggrDeps = 0x10,		// Use aggregated dependences.
  OptimizeRCMSSolving_Rational = 0x20,		// Allow rational solution.
  OptimizeRCMSSolving_PostPass = 0x40,		// Apply to postpass problems.
  OptimizeRCMSSolving_Acyclic = 0x80,		// Apply to acyclic problems.
} enum_OptimizeRCMSSolving;
typedef uint8_t OptimizeRCMSSolving;
#define OptimizeRCMSSolving_default ( \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeRCMSSolving_Table[1];
static inline bool
OptimizeRCMSSolving_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeRCMSSolving_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeRCMSSolving_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeRCMSSolving_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeRCMSSolving flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeRCMSSolving_Entries[] = {
  ETableENTRY(OptimizeRCMSSolving, Dinechin),
  ETableENTRY(OptimizeRCMSSolving, LNSScan1),
  ETableENTRY(OptimizeRCMSSolving, LNSScan2),
  ETableENTRY(OptimizeRCMSSolving, EicDavAb),
  ETableENTRY(OptimizeRCMSSolving, AggrDeps),
  ETableENTRY(OptimizeRCMSSolving, Rational),
  ETableENTRY(OptimizeRCMSSolving, PostPass),
  ETableENTRY(OptimizeRCMSSolving, Acyclic),
  ETableENTRY_NULL
};
ETable_ OptimizeRCMSSolving_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * RCMS problem description.
 */
typedef enum {
  OptimizeRCMSProblem_Regular = 0x1,	// Pass regular nodes and arcs.
  OptimizeRCMSProblem_Overlap = 0x2,	// Pass pipeline overlap-limiting arc.
  OptimizeRCMSProblem_Lifetime = 0x4,	// Pass lifetine nodes and arcs.
  OptimizeRCMSProblem_Renaming = 0x8,	// Pass modulo renaming-limiting arcs.
  OptimizeRCMSProblem_Margins = 0x10,	// Pass margin-enforcing arcs.
} enum_OptimizeRCMSProblem;
typedef uint8_t OptimizeRCMSProblem;
#define OptimizeRCMSProblem_default ( \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeRCMSProblem_Table[1];
static inline bool
OptimizeRCMSProblem_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeRCMSProblem_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeRCMSProblem_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeRCMSProblem_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeRCMSProblem flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeRCMSProblem_Entries[] = {
  ETableENTRY(OptimizeRCMSProblem, Regular),
  ETableENTRY(OptimizeRCMSProblem, Overlap),
  ETableENTRY(OptimizeRCMSProblem, Lifetime),
  ETableENTRY(OptimizeRCMSProblem, Renaming),
  ETableENTRY(OptimizeRCMSProblem, Margins),
  ETableENTRY_NULL
};
ETable_ OptimizeRCMSProblem_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerate the LogTimeOut flags.
 */
typedef enum {
  OptimizeLogTimeOut_Times2 = 0x1,		// TimeOut *= 2.
  OptimizeLogTimeOut_Times4 = 0x2,		// TimeOut *= 4.
  OptimizeLogTimeOut_Times16 = 0x4,		// TimeOut *= 16.
  OptimizeLogTimeOut_Times256 = 0x8,		// TimeOut *= 256.
} enum_OptimizeLogTimeOut;
typedef uint8_t OptimizeLogTimeOut;
#define OptimizeLogTimeOut_default ( \
        0 )
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ OptimizeLogTimeOut_Table[1];
static inline bool
OptimizeLogTimeOut_prettyFlags(OptimizeFlags flags, FILE *file)
{
  if (!flags) return fprintf(file, "0");
  return ETable_prettyFlags(OptimizeLogTimeOut_Table, flags, ",", file);
}
static inline OptimizeFlags
OptimizeLogTimeOut_parseFlags(const char *string)
{
  uint32_t flags;
  if (!ETable_parseFlags(OptimizeLogTimeOut_Table, string, ',', &flags)) {
    fprintf(stderr, "Error parsing OptimizeLogTimeOut flags: %s\n", string);
    ASSERT(0);
  }
  return flags;
}
#endif//$XCC_h

ETableEntry_ OptimizeLogTimeOut_Entries[] = {
  ETableENTRY(OptimizeLogTimeOut, Times2),
  ETableENTRY(OptimizeLogTimeOut, Times4),
  ETableENTRY(OptimizeLogTimeOut, Times16),
  ETableENTRY(OptimizeLogTimeOut, Times256),
  ETableENTRY_NULL
};
ETable_ OptimizeLogTimeOut_Table[1];

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * PostPass scheduling levels.
 */
typedef enum {
  OptimizeScoreboarding_Fixup = 0x1,	// Fixup pass over the BasicBlock(s).
  OptimizeScoreboarding_Iterate = 0x2,	// Iterate forward data-flow problem.
  OptimizeScoreboarding_Priority = 0x4,	// Pre-order Operation(s) by priority.
} enum_OptimizeScoreboarding;
typedef uint8_t OptimizeScoreboarding;
#define OptimizeScoreboarding_default ( \
        OptimizeScoreboarding_Iterate | \
        0 )
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerate the Optimize items.
 *
 * Each OptimizeItem maps to a set of flags.
 */
typedef enum {
  OptimizeItem_Activation,		// See OptimizeActivation.
  OptimizeItem_LogMaxBBOC,		// Log2 of max BasicBlock Operation count.
  OptimizeItem_Profiling,		// See OptimizeProfiling.
  OptimizeItem_Conversion,		// See OptimizeConversion.
  OptimizeItem_Coalescing,		// See OptimizeCoalescing.
  OptimizeItem_Rewriting,		// See OptimizeRewriting.
  OptimizeItem_Numbering,		// See OptimizeNumbering.
  OptimizeItem_Propagation,		// See OptimizePropagation.
  OptimizeItem_Predication,		// See OptimizePredication.
  OptimizeItem_Scheduling,		// See OptimizeScheduling.
  OptimizeItem_Allocation,		// See OptimizeAllocation.
  OptimizeItem_Spilling,		// See OptimizeSpilling.
  OptimizeItem_Coloring,		// See OptimizeColoring.
  OptimizeItem_RCMSSolving,		// See OptimizeRCMSSolving.
  OptimizeItem_RCMSProblem,		// See OptimizeRCMSProblem.
  OptimizeItem_LogTimeOut,		// See OptimizeLogTimeOut.
  OptimizeItem_Scoreboarding,		// See OptimizeScoreboarding.
  OptimizeItem_WindowSize,		// Scoreboard window size
  OptimizeItem_PrePadding,		// See OptimizePrePadding.
  OptimizeItem_PostPadding,		// See OptimizePostPadding.
  OptimizeItem__
} enum_OptimizeItem;
typedef uint8_t OptimizeItem;
extern const char *
OptimizeItem_Id(OptimizeItem this);
#endif//$XCC_h

const char *
OptimizeItem_Id(OptimizeItem this)
{
  static const char *_[] = {
    "Activation",
    "LogMaxBBOC",
    "Profiling",
    "Conversion",
    "Coalescing",
    "Rewriting",
    "Numbering",
    "Propagation",
    "Predication",
    "Scheduling",
    "Allocation",
    "Spilling",
    "Coloring",
    "RCMSSolving",
    "RCMSProblem",
    "LogTimeOut",
    "Scoreboarding",
    "WindowSize",
    "PrePadding",
    "PostPadding",
  };
  Except_REQUIRE((unsigned)this < OptimizeItem__);
  return _[this];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Array of OptimizeItem values.
 */
struct Optimize_ {
  //@args
  OptimizeFlags ITEMS[OptimizeItem__];
  //@access ACTIVATION	(*Optimize__ACTIVATION(this) + 0)
  //@mutate ACTIVATION	(Optimize_ITEMS(this) + OptimizeItem_Activation)
  //@access LOGMAXBBOC	(*Optimize__LOGMAXBBOC(this) + 0)
  //@mutate LOGMAXBBOC	(Optimize_ITEMS(this) + OptimizeItem_LogMaxBBOC)
  //@access PROFILING	(*Optimize__PROFILING(this) + 0)
  //@mutate PROFILING	(Optimize_ITEMS(this) + OptimizeItem_Profiling)
  //@access CONVERSION	(*Optimize__CONVERSION(this) + 0)
  //@mutate CONVERSION	(Optimize_ITEMS(this) + OptimizeItem_Conversion)
  //@access COALESCING	(*Optimize__COALESCING(this) + 0)
  //@mutate COALESCING	(Optimize_ITEMS(this) + OptimizeItem_Coalescing)
  //@access REWRITING	(*Optimize__REWRITING(this) + 0)
  //@mutate REWRITING	(Optimize_ITEMS(this) + OptimizeItem_Rewriting)
  //@access NUMBERING	(*Optimize__NUMBERING(this) + 0)
  //@mutate NUMBERING	(Optimize_ITEMS(this) + OptimizeItem_Numbering)
  //@access PROPAGATION	(*Optimize__PROPAGATION(this) + 0)
  //@mutate PROPAGATION	(Optimize_ITEMS(this) + OptimizeItem_Propagation)
  //@access PREDICATION	(*Optimize__PREDICATION(this) + 0)
  //@mutate PREDICATION	(Optimize_ITEMS(this) + OptimizeItem_Predication)
  //@access SCHEDULING	(*Optimize__SCHEDULING(this) + 0)
  //@mutate SCHEDULING	(Optimize_ITEMS(this) + OptimizeItem_Scheduling)
  //@access ALLOCATION	(*Optimize__ALLOCATION(this) + 0)
  //@mutate ALLOCATION	(Optimize_ITEMS(this) + OptimizeItem_Allocation)
  //@access SPILLING	(*Optimize__SPILLING(this) + 0)
  //@mutate SPILLING	(Optimize_ITEMS(this) + OptimizeItem_Spilling)
  //@access COLORING	(*Optimize__COLORING(this) + 0)
  //@mutate COLORING	(Optimize_ITEMS(this) + OptimizeItem_Coloring)
  //@access RCMSSOLVING	(*Optimize__RCMSSOLVING(this) + 0)
  //@mutate RCMSSOLVING	(Optimize_ITEMS(this) + OptimizeItem_RCMSSolving)
  //@access RCMSPROBLEM	(*Optimize__RCMSPROBLEM(this) + 0)
  //@mutate RCMSPROBLEM	(Optimize_ITEMS(this) + OptimizeItem_RCMSProblem)
  //@access LOGTIMEOUT	(*Optimize__LOGTIMEOUT(this) + 0)
  //@mutate LOGTIMEOUT	(Optimize_ITEMS(this) + OptimizeItem_LogTimeOut)
  //@access SCOREBOARDING	(*Optimize__SCOREBOARDING(this) + 0)
  //@mutate SCOREBOARDING	(Optimize_ITEMS(this) + OptimizeItem_Scoreboarding)
  //@access WINDOWSIZE	(*Optimize__WINDOWSIZE(this) + 0)
  //@mutate WINDOWSIZE	(Optimize_ITEMS(this) + OptimizeItem_WindowSize)
  //@access PREPADDING	(*Optimize__PREPADDING(this) + 0)
  //@mutate PREPADDING	(Optimize_ITEMS(this) + OptimizeItem_PrePadding)
  //@access POSTPADDING	(*Optimize__POSTPADDING(this) + 0)
  //@mutate POSTPADDING	(Optimize_ITEMS(this) + OptimizeItem_PostPadding)
};
#endif//$XCC_h

Optimize
Optimize_Ctor(Optimize this)
{
  memset(this, 0, sizeof(Optimize_));
  Optimize_load(this,
                OptimizeItem_Activation, OptimizeActivation_default,
                OptimizeItem_LogMaxBBOC, 12,
                OptimizeItem_Conversion, OptimizeConversion_default,
                OptimizeItem_Coalescing, OptimizeCoalescing_default,
                OptimizeItem_Rewriting, OptimizeRewriting_default,
                OptimizeItem_Profiling, OptimizeProfiling_default,
                OptimizeItem_Scheduling, OptimizeScheduling_default,
                OptimizeItem_Allocation, OptimizeAllocation_default,
                OptimizeItem_Spilling, OptimizeSpilling_default,
                OptimizeItem_Coloring, OptimizeColoring_default,
                OptimizeItem_RCMSSolving, OptimizeRCMSSolving_default,
                OptimizeItem_RCMSProblem, OptimizeRCMSProblem_default,
                OptimizeItem_LogTimeOut, OptimizeLogTimeOut_default,
                OptimizeItem_Scoreboarding, OptimizeScoreboarding_default,
                OptimizeItem_WindowSize, 8,
                OptimizeItem__);
  return this;
}

void
Optimize_Dtor(Optimize this)
{
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Optimize_)\t%zu\n", sizeof(Optimize_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Print this Optimize item values.
 */
bool
Optimize_pretty(Optimize this, FILE *file);
#endif//$XCC_h

bool
Optimize_pretty(Optimize this, FILE *file)
{
  int i = 0;
  if (file == NULL) file = stderr;
  for (; i < OptimizeItem__; i++) {
    const char *name = OptimizeItem_Id(i);
    int value = Optimize_ITEMS(this)[i];
    fprintf(file, "\t%s=%d", name, value);
  }
  return true;
}

#ifdef $XCC_h
/**
 * Force item values in this Optimize.
 */
void
Optimize_force(Optimize this);
#endif//$XCC_h

void
Optimize_force(Optimize this)
{
  if (FORCE_ACTIVATION >= 0) *Optimize__ACTIVATION(this) &= FORCE_ACTIVATION;
  if (FORCE_CONVERSION >= 0) *Optimize__CONVERSION(this) = FORCE_CONVERSION;
  if (FORCE_COALESCING >= 0) *Optimize__COALESCING(this) = FORCE_COALESCING;
  if (FORCE_REWRITING >= 0) *Optimize__REWRITING(this) = FORCE_REWRITING;
  if (FORCE_PREDICATION >= 0) *Optimize__PREDICATION(this) = FORCE_PREDICATION;
  if (FORCE_SCHEDULING >= 0) *Optimize__SCHEDULING(this) = FORCE_SCHEDULING;
  if (FORCE_ALLOCATION >= 0) *Optimize__ALLOCATION(this) = FORCE_ALLOCATION;
  if (FORCE_RCMSSOLVING >= 0) *Optimize__RCMSSOLVING(this) = FORCE_RCMSSOLVING;
  if (FORCE_RCMSPROBLEM >= 0) *Optimize__RCMSPROBLEM(this) = FORCE_RCMSPROBLEM;
  if (FORCE_LOGTIMEOUT >= 0) *Optimize__LOGTIMEOUT(this) = FORCE_LOGTIMEOUT;
  if (FORCE_SCOREBOARDING >= 0) *Optimize__SCOREBOARDING(this) = FORCE_SCOREBOARDING;
  if (FORCE_WINDOWSIZE >= 0) *Optimize__WINDOWSIZE(this) = FORCE_WINDOWSIZE;
  if (FORCE_PREPADDING >= 0) *Optimize__PREPADDING(this) = FORCE_PREPADDING;
  if (FORCE_POSTPADDING >= 0) *Optimize__POSTPADDING(this) = FORCE_POSTPADDING;
}

#ifdef $XCC_h
/**
 * Load this Optimize from a va_list.
 */
void
Optimize_loadVA(Optimize this, int item, va_list va);
#endif//$XCC_h

void
Optimize_loadVA(Optimize this, int item, va_list va)
{
  while (item != OptimizeItem__) {
    uint32_t value = (uint32_t)va_arg(va, uint32_t);
    Except_REQUIRE(item >= 0 && item < OptimizeItem__);
    Optimize_ITEMS(this)[item] = value;
    item = (int)va_arg(va, int);
  }
  Optimize_force(this);
}

#ifdef $XCC_h
/**
 * Load this Optimize from variable arguments.
 */
void
Optimize_load(Optimize this, OptimizeItem item, ...);
#endif//$XCC_h

void
Optimize_load(Optimize this, OptimizeItem item, ...)
{
  va_list va;
  va_start(va, item);
  Optimize_loadVA(this, item, va);
  va_end(va);
}

#ifdef $XCC_h
/**
 * Print this Optimize item values.
 */
bool
Optimize_print(Optimize this, FILE *file);
#endif//$XCC_h

bool
Optimize_print(Optimize this, FILE *file)
{
  int i;
  for (i = 0; i < OptimizeItem__; i++) {
    const char *name = OptimizeItem_Id(i);
    uint32_t value = Optimize_ITEMS(this)[i];
    fprintf(file, "OptimizeItem_%s, %u, ", name, value);
  }
  fprintf(file, "OptimizeItem__");
  return true;
}

#ifdef $XCC__c
{
  Optimize_ optimize[1];
  Optimize_Ctor(optimize);
  printf("Optimize(");
  Optimize_print(optimize, stdout);
  printf(")\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Optimize module.
 */
void
Optimize_INIT(void);
#endif//$XCC__h

void
Optimize_INIT(void)
{
  ETable_Ctor(OptimizeActivation_Table, Memory_Root, OptimizeActivation_Entries);
  ETable_Ctor(OptimizeConversion_Table, Memory_Root, OptimizeConversion_Entries);
  ETable_Ctor(OptimizeCoalescing_Table, Memory_Root, OptimizeCoalescing_Entries);
  ETable_Ctor(OptimizeRewriting_Table, Memory_Root, OptimizeRewriting_Entries);
  ETable_Ctor(OptimizeProfiling_Table, Memory_Root, OptimizeProfiling_Entries);
  ETable_Ctor(OptimizeScheduling_Table, Memory_Root, OptimizeScheduling_Entries);
  ETable_Ctor(OptimizeAllocation_Table, Memory_Root, OptimizeAllocation_Entries);
  ETable_Ctor(OptimizeSpilling_Table, Memory_Root, OptimizeSpilling_Entries);
  ETable_Ctor(OptimizeColoring_Table, Memory_Root, OptimizeColoring_Entries);
  ETable_Ctor(OptimizeRCMSSolving_Table, Memory_Root, OptimizeRCMSSolving_Entries);
  ETable_Ctor(OptimizeRCMSProblem_Table, Memory_Root, OptimizeRCMSProblem_Entries);
  ETable_Ctor(OptimizeLogTimeOut_Table, Memory_Root, OptimizeLogTimeOut_Entries);
}

#ifdef $XCC__h
/**
 * Finalize the Optimize module.
 */
void
Optimize_FINI(void);
#endif//$XCC__h

void
Optimize_FINI(void)
{
  ETable_Dtor(OptimizeLogTimeOut_Table);
  ETable_Dtor(OptimizeRCMSProblem_Table);
  ETable_Dtor(OptimizeRCMSSolving_Table);
  ETable_Dtor(OptimizeColoring_Table);
  ETable_Dtor(OptimizeSpilling_Table);
  ETable_Dtor(OptimizeAllocation_Table);
  ETable_Dtor(OptimizeScheduling_Table);
  ETable_Dtor(OptimizeProfiling_Table);
  ETable_Dtor(OptimizeRewriting_Table);
  ETable_Dtor(OptimizeCoalescing_Table);
  ETable_Dtor(OptimizeConversion_Table);
  ETable_Dtor(OptimizeActivation_Table);
}

#if XCC__C
static void
Optimize_TEST(void)
{
#include "LIR/Optimize_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Optimize_TEST);
  return 0;
}
#endif

