#ifdef $XCC_h
/*
 * !!!!	Temporary.xcc
 *
 * Benoit Dupont de Dinechin (benoit.dinechin@kalray.eu).
 * Christophe Guillon (Christophe.Guillon@st.com).
 * Nikola Puzovic (Nikola.Puzovic@st.com).
 * Duco van Amstel (duco.van-amstel@kalray.eu).
 *
 * 
 * Copyright 2010 - 2013 Kalray.
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
 * Handling of temporaries (virtual locations for holding values).
 *
 * There are different temporary types:
 *
 * @par Dedicated Temporary(ies)
 *  Dedicated Temporary(ies) are immutable and used to represent 
 *  @c Operation arguments or results that must be in specific machine registers. 
 *  This applies to procedure register arguments and results and also to 
 *  implicit or pinned register operands.  In effect, using a Dedicated 
 *  Temporary is an override for the register allocator on what Register to 
 *  assign to a particular Temporary.\n
 * .
 * @c Temporary_hasRegFile is @c true for Dedicated Temporary(ies)
 * @c Temporary_hasRegister is @c true for Dedicated Temporary(ies)
 *
 * @note
 * It is a requirement for correctness to use dedicated temporaries in the 
 * following cases:
 *   - use of procedure incoming parameters in and only in the prologue basic 
 *     block (the procedure entry),
 *   - definition of procedure outgoing results in and only in any epilogue 
 *     basic block containing a exit from the procedure,
 *   - definition of function call parameter in and only in the block where the 
 *     call site appears,
 *   - use of function result in and only in the block immediatly following the 
 *     block where the call site appears, this block must exist and have a single 
 *     predecessor as soon as some return value of the call site is used,
 *   - any use or definition of defined registers such as the stack pointer, 
 *     the frame pointer, the global data pointer and the procedure link register 
 *     must use the corresponding dedicated temporary.
 *   .
 * These requirements must be enforced before register allocation. After register
 * allocation, the same requirements hold but the scope of the dedicated temporaries
 * used for parameter passing can be extended out of the homing basic block.
 *
 *
 * @par Virtual Temporary(ies)
 * Virtual Temporary(ies) that are also known as pseudo-registers. They are used to
 * carry values between Operation(s).\n
 * .
 * @c Temporary_hasRegFile is @c true for Virtual Temporary(ies)
 * @c Temporary_hasRegister is valid for Virtual Temporary(ies)
 *
 * @par	Variable Temporary(ies)
 * Variable Temporary(ies) are base objects of Variable(s). They can be 
 * assigned a Register.\n
 * .
 * @c Temporary_hasRegFile is @c true for Variable Temporary(ies)
 * @c Temporary_hasRegister is valid for Variable Temporary(ies)
 *
 *
 * @par Absolute Temporary(ies)
 * Absolute Temporary(ies) are used to represent resolved constants.\n
 * .
 * @c Temporary_hasLiteral is @c true for Absolute Temporary(ies)
 *
 * @par Label Temporary(ies)
 * Label Temporary(ies) are used for the arguments for intra-procedural branches. For
 * inter-procedural branches, such as those that result from optimizing tail calls,
 * including recursive tail call, Symbol Temporary(ies) must be used instead.\n
 * .
 * @c Temporary_hasLiteral is @c true for Label Temporary(ies)
 *
 * @par Symbol Temporary(ies)
 * Symbol Temporary(ies) are used to represent symbolic values made of a base Symbol
 * plus an offset.\n
 * .
 * @c Temporary_hasLiteral is @c true for Symbol Temporary(ies)
 *
 * @par Modifier Temporary(ies)
 * Modifier Temporary(ies) are used to represent instruction modifiers.
 *
 * @par RegMask Temporary(ies)
 * RegMask Temporary(ies) are placeholders for the immediate values interpreted as
 * register masks.  During encoding, the value of the Instruction's register mask
 * is computed from the Operation's additional arguments or results that are not
 * Parameters.
 *
 *
 * @note
 * - <em>sharing Temporary(ies):</em>
 * The Register Temporary(ies) should be shared between Operation(s).
 * Other Temporary(ies) should be used by only one Operation. This can be relaxed
 * for Temporary(ies) that are immutable like Absolute Temporary(ies), and also in
 * case it is guaranteed all Operation(s) that share a non-Register Temporary do
 * not update it. Updates are for instance changing the Relocation of the offset.
 *
 * @note 
   - <em>Temporary Relocation(s):</em>
 * Literal Temporary(ies) have an associated Relocation. This Relocation is an
 * enumeration that specifies additional processing or checking to be done on a
 * resolved ImmediateValue before being passed to the Immediate encoding. Thus,
 * processing done by a linker relocation combines the processing of Relocation
 * with the Immediate encoding process.
 */
#endif//$XCC_h

#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/Temporary.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * IndexedFactory for Temporary(ies).
 */
extern IndexedFactory_ Temporary_Factory[1];
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Temporary types.
 *
 * Depending on the type, the fields of the Temporary structure can have 
 * different meanings. The TemporaryUnion field is also different.
 */
typedef enum {
  TemporaryType_Dedicated,		//!< Dedicated register.
                                        //!	WIDTH is the bit-width if known.
                                        //!	REGFILE is the RegFile.
                                        //!	REGISTER is the Register.
  TemporaryType_Virtual,		//!< Virtual register.
                                        //!	WIDTH is the bit-width if known.
                                        //!	REGFILE is the RegFile.
                                        //!	REGISTER is Register__UNDEF;
  TemporaryType_Variable,		//!< Base of an (SSA) Variable.
                                        //!	WIDTH is the bit-width if known.
                                        //!	REGFILE is the RegFile.
                                        //!	REGISTER is Register__UNDEF;
  TemporaryType_Absolute,		//!< Absolute type.
                                        //!	LITERAL is the Literal.
  TemporaryType_Symbol,			//!< Symbol type.
                                        //!	LITERAL is the Literal.
  TemporaryType_Label,			//!< Label type.
                                        //!	LITERAL is the Literal.
  TemporaryType_Modifier,               //!< Opcode modifier.
  TemporaryType_RegMask,		//!< Register mask.
  TemporaryType_Volatile,		//!< Volatile resource.
  TemporaryType_Control,		//!< Control resource.
  TemporaryType_Memory,			//!< Memory resource.
  TemporaryType__
} enum_TemporaryType;
typedef uint8_t TemporaryType;
#define TemporaryType_isDedicated(type)	((type) == TemporaryType_Dedicated)
#define TemporaryType_isVirtual(type)	((type) == TemporaryType_Virtual)
#define TemporaryType_isVariable(type)	((type) == TemporaryType_Variable)
#define TemporaryType_isAbsolute(type)	((type) == TemporaryType_Absolute)
#define TemporaryType_isSymbol(type)	((type) == TemporaryType_Symbol)
#define TemporaryType_isLabel(type)	((type) == TemporaryType_Label)
#define TemporaryType_isModifier(type)	((type) == TemporaryType_Modifier)
#define TemporaryType_isRegMask(type)	((type) == TemporaryType_RegMask)
#define TemporaryType_isVolatile(type)	((type) == TemporaryType_Volatile)
#define TemporaryType_isControl(type)	((type) == TemporaryType_Control)
#define TemporaryType_isMemory(type)	((type) == TemporaryType_Memory)
#define TemporaryType_hasRegFile(type)	\
  ((unsigned)(type) <= TemporaryType_Variable)
#define TemporaryType_hasLiteral(type)	\
  (   (unsigned)((type) - TemporaryType_Absolute) \
   <= (unsigned)(TemporaryType_Label - TemporaryType_Absolute))
#define TemporaryType_isRelaxable(type)	\
  (   (unsigned)((type) - TemporaryType_Absolute) \
   <= (unsigned)(TemporaryType_Symbol - TemporaryType_Absolute))
extern const char *
TemporaryType_Id(TemporaryType this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
TemporaryType_Id(TemporaryType this)
{
  static const char *_[] = {
    "Dedicated",
    "Virtual",
    "Variable",
    "Absolute",
    "Symbol",
    "Label",
    "Modifier",
    "RegMask",
    "Volatile",
    "Control",
    "Memory",
  };
  Except_REQUIRE((unsigned)this < TemporaryType__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the Temporary flags.
 */
typedef enum {
  TemporaryFlag_Constant = 0x1,		//!< Temporary is constant.
  TemporaryFlag_Register = 0x2,		//!< Temporary has a Register.
  TemporaryFlag_Global = 0x4,		//!< Temporary is global in Liveness.
  TemporaryFlag_Killed = 0x8,		//!< Liveness flag, must be cleared after use.
  TemporaryFlag_Remater = 0x10,		//!< Register Temporary is rematerializable.
                                        //!  Get Literal Temporary in REMATERMAP.
  TemporaryFlag_Homeable = 0x20,	//!< Register Temporary is homeable.
                                        //!  The temporary has already a "home" 
                                        //!  assigned in the stack. Happen for 
                                        //!  instance when the address of a local 
                                        //!  variable is taken.
                                        //!  Get home Symbol Temporary in HOMEABLEMAP.
  TemporaryFlag_RenameOK = 0x40,	//!< Virtual Temporary can be SSA renamed.
  TemporaryFlag_Recycled = 0x80,	//!< Virtual Temporary is recycled by SSADestruct.
} enum_TemporaryFlag;
typedef uint8_t TemporaryFlags;
#endif//$XCC_h

#ifdef $XCC_h
extern struct ETable_ TemporaryFlag_Table[1];
#endif//$XCC_h

#ifndef _NTRACE
ETableEntry_ TemporaryFlagEntries[] = {
  ETableENTRY(TemporaryFlag, Constant),
  ETableENTRY(TemporaryFlag, Register),
  ETableENTRY(TemporaryFlag, Global),
  ETableENTRY(TemporaryFlag, Killed),
  ETableENTRY(TemporaryFlag, Remater),
  ETableENTRY(TemporaryFlag, Homeable),
  ETableENTRY(TemporaryFlag, RenameOK),
  ETableENTRY(TemporaryFlag, Recycled),
  ETableENTRY_NULL
};
ETable_ TemporaryFlag_Table[1];
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Union for the variant part of Temporary.
 *
 * This depends on the TemporaryType of the Temporary.
 */
union TemporaryUnion_ {
  struct Variable_ *WEBROOT;		//!< TemporaryType_isVirtual.
  struct Variable_ *SPILLED;		//!< TemporaryType_isVariable.
  struct Literal_  *LITERAL;		//!< TemporaryType_hasLiteral.
  ModifierMember MEMBER;		//!< TemporaryType_isModifier.
};
typedef union TemporaryUnion_ TemporaryUnion_, *TemporaryUnion;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Argument and result type for Operations.
 */
struct Temporary_ {
  //@args	TemporaryType type, unsigned flags
  Indexed_ __;				//!< A Temporary_ ISA Indexed_.
  //@access IDENTITY	Indexed_IDENTITY(Temporary____(this))
  //@access INDEX	Indexed_INDEX(Temporary____(this))
  //@mutate INDEX	Indexed__INDEX(Temporary____(this))
  TemporaryUnion_ UNION_;		//!< See TemporaryUnion.
  //@access WEBROOT	(Temporary_UNION_(this).WEBROOT)
  //@mutate WEBROOT	(&Temporary_UNION_(this).WEBROOT)
  //@access SPILLED	(Temporary_UNION_(this).SPILLED)
  //@mutate SPILLED	(&Temporary_UNION_(this).SPILLED)
  //@access LITERAL	(Temporary_UNION_(this).LITERAL)
  //@mutate LITERAL	(&Temporary_UNION_(this).LITERAL)
  //@access MEMBER	(Temporary_UNION_(this).MEMBER)
  //@mutate MEMBER	(&Temporary_UNION_(this).MEMBER)
  TemporaryType TYPE;			//!< This Temporary type.
  //@access isDedicated	TemporaryType_isDedicated(Temporary_type(this))
  //@access isVirtual	TemporaryType_isVirtual(Temporary_type(this))
  //@access isVariable	TemporaryType_isVariable(Temporary_type(this))
  //@access isAbsolute	TemporaryType_isAbsolute(Temporary_type(this))
  //@access isSymbol	TemporaryType_isSymbol(Temporary_type(this))
  //@access isLabel	TemporaryType_isLabel(Temporary_type(this))
  //@access isModifier	TemporaryType_isModifier(Temporary_type(this))
  //@access isRegMask	TemporaryType_isRegMask(Temporary_type(this))
  //@access isVolatile	TemporaryType_isVolatile(Temporary_type(this))
  //@access isControl	TemporaryType_isControl(Temporary_type(this))
  //@access isMemory	TemporaryType_isMemory(Temporary_type(this))
  //@access hasRegFile	TemporaryType_hasRegFile(Temporary_type(this))
  //@access hasLiteral	TemporaryType_hasLiteral(Temporary_type(this))
  //@access isRelaxable	TemporaryType_isRelaxable(Temporary_type(this))
  TemporaryFlags FLAGS;		//!< This Temporary flags.
  //@access isConstant	((Temporary_flags(this) & TemporaryFlag_Constant) != 0)
  //@access hasRegister	((Temporary_flags(this) & TemporaryFlag_Register) != 0)
  //@access isGlobal	((Temporary_flags(this) & TemporaryFlag_Global) != 0)
  //@access isKilled	((Temporary_flags(this) & TemporaryFlag_Killed) != 0)
  //@access isRemater	((Temporary_flags(this) & TemporaryFlag_Remater) != 0)
  //@access isHomeable	((Temporary_flags(this) & TemporaryFlag_Homeable) != 0)
  //@access isRenameOK	((Temporary_flags(this) & TemporaryFlag_RenameOK) != 0)
  //@access isRecycled	((Temporary_flags(this) & TemporaryFlag_Recycled) != 0)
  //@access isAssigned	(Temporary_isVirtual(this) && Temporary_hasRegister(this))
  uint8_t WIDTH;			//!< Width in bits of this Temporary.
  RegFile REGFILE;			//!< The RegFile of this Temporary.
  //@access isRenameReg	(Temporary_hasRegFile(this) && !Temporary_isDedicated(this))
  Register REGISTER;			//!< The Register of this Temporary.
  //@access VARIABLE	(Variable)(this)
};
#endif//$XCC_h

#ifdef $XCC_h
static inline Indexed
Temporary__(Temporary this)
{
  return Temporary____(this);
}
static inline const_Indexed
Temporary__const(const_Temporary this)
{
  return Temporary____(this);
}
#endif//$XCC_h

Temporary
Temporary_Ctor(Temporary this,
               TemporaryType type, unsigned flags)
{
  Indexed_Ctor(Temporary____(this), Temporary_Factory);
  *Temporary__WEBROOT(this) = NULL;
  *Temporary__TYPE(this) = type;
  *Temporary__FLAGS(this) = flags;
  *Temporary__WIDTH(this) = 0;
  *Temporary__REGFILE(this) = RegFile__UNDEF;
  *Temporary__REGISTER(this) = Register__UNDEF;
  return this;
}

void
Temporary_Dtor(Temporary this)
{
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Temporary_)\t%zu\n", sizeof(Temporary_));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Temporary identity.
 */
static inline uint32_t
Temporary_identity(const_Temporary this)
{
  return Temporary_IDENTITY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Temporary @c index.
 */
static inline uint32_t
Temporary_index(const_Temporary this)
{
  return Temporary_INDEX(this);
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Set this @p Temporary INDEX.
 */
static inline void
Temporary_setIndex(Temporary this, uint32_t index)
{
  *Temporary__INDEX(this) = index;
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * This Temporary TemporaryType.
 */
static inline TemporaryType
Temporary_type(const_Temporary this)
{
  return Temporary_TYPE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Temporary TemporaryFlags.
 */
static inline TemporaryFlags
Temporary_flags(const_Temporary this)
{
  return Temporary_FLAGS(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Temporary flags.
 */
static inline void
Temporary_setFlags(Temporary this, TemporaryFlags flags)
{
  *Temporary__FLAGS(this) |= flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Clear this Temporary flags.
 */
static inline void
Temporary_clearFlags(Temporary this, TemporaryFlags flags)
{
  *Temporary__FLAGS(this) &= ~flags;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The register associated with a Register Temporary.
 *
* @return the Register, or Register__UNDEF for non-Register Temporary(ies).
 */
static inline Register
Temporary_register(const_Temporary this)
{
  Except_CHECK(Temporary_hasRegister(this) || Temporary_REGISTER(this) == Register__UNDEF);
  return Temporary_REGISTER(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The variable associated to a temporary.
 */
static inline struct Variable_*
Temporary_variable(const_Temporary this)
{
  Except_CHECK(Temporary_isVariable(this));
/*  return Temporary_VARIABLE(this);*/
  return (struct Variable_*)(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * The register file associated with a Register Temporary.
 *
 * @return the Register, or RegFile__UNDEF for non-Register Temporary(ies).
 */
static inline RegFile
Temporary_regFile(const_Temporary this)
{
  Except_CHECK(Temporary_hasRegFile(this) || Temporary_REGFILE(this) == RegFile__UNDEF);
  return Temporary_REGFILE(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Low-level function to make a new Temporary.
 */
static inline Temporary
Temporary_make(IBList idlist, TemporaryType type, TemporaryFlags flags)
{
  Temporary temporary = IBList_push(idlist);
  Temporary_Ctor(temporary, type, flags);
  return temporary;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Force a Virtual Temporary to be dedicated.
 *
 * Note that this function only exists because copy folding may have discarded a
 * dedicated Temporary and replaced it by an Assigned Temporary. On the Open64
 * such copy folding breaks the invariant that procedure register arguments should be
 * in Dedicated Temporary(ies). Calling Temporary_setDedicated cures the problem.
 */
static inline void
Temporary_setDedicated(Temporary this)
{
  Except_CHECK(Temporary_isAssigned(this));
  *Temporary__TYPE(this) = TemporaryType_Dedicated;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Compare two Temporary(ies) for total ordering.
 */
int
Temporary_compare(const_Temporary this, const_Temporary that);
#endif//$XCC_h

int
Temporary_compare(const_Temporary this, const_Temporary that)
{
  int this_type = Temporary_TYPE(this);
  int that_type = Temporary_TYPE(that);
  int this_register = Temporary_REGISTER(this);
  int that_register = Temporary_REGISTER(that);
  uint32_t this_identity = Temporary_IDENTITY(this);
  uint32_t that_identity = Temporary_IDENTITY(that);
  int compare_type = that_type - this_type;
  int compare_register = this_register - that_register;
  int compare_identity = (this_identity > that_identity) -
                         (this_identity < that_identity);
  if (compare_type) return compare_type;
  if (compare_register &&
      Temporary_hasRegister(this)) return compare_register;
  return compare_identity;
}

#ifdef $XCC_h
/**
 * Fix this Temporary to match an OperatorParameter.
 */
void
Temporary_fixWithParameter(Temporary this, const_OperatorParameter parameter);
#endif//$XCC_h

void
Temporary_fixWithParameter(Temporary this, const_OperatorParameter parameter)
{
  OperandType type = OperatorParameter_type(parameter);
  if (type == OperandType_RegClass && Temporary_hasRegFile(this)) {
    RegClass regClass = (RegFile)OperatorParameter_enum(parameter);
    *Temporary__REGFILE(this) = RegClass_regFile(regClass);
  } else
  if (type == OperandType_Immediate && Temporary_hasLiteral(this)) {
    Immediate immediate = (Immediate)OperatorParameter_enum(parameter);
    Literal literal = Temporary_LITERAL(this);
    *Literal__IMMEDIATE(literal) = immediate;
  }
}

#ifdef $XCC_h
/**
 * Check if this Temporary matches a parameter of an operator.
 */
bool
Temporary_matchParameter(const_Temporary this, const_OperatorParameter parameter);
#endif//$XCC_h

bool
Temporary_matchParameter(const_Temporary this, const_OperatorParameter parameter)
{
  OperandType type = OperatorParameter_type(parameter);
  switch(type) {
    case OperandType_Register:
      if (!Temporary_hasRegFile(this)) return false;
      if (!Temporary_hasRegister(this)) return false;
      return false;
      break;
    case OperandType_RegClass:
      if (!Temporary_hasRegFile(this)) return false;
      break;
    case OperandType_Immediate:
      if (!Temporary_hasLiteral(this)) return false;
      /* Check immediate size */
      if (Temporary_isAbsolute(this)) {
        Immediate immediate = OperatorParameter_enum(parameter);
        Literal literal = Temporary_LITERAL(this);
        if (!Immediate_canEncode(immediate, Literal_VALUE(literal))) return false;
      }
      break;
    case OperandType_Modifier:
      if (!Temporary_isModifier(this)) return false;
      break;
    case OperandType_RegMask:
      if (!Temporary_isRegMask(this)) return false;
      break;
    default: return false;
  }
  return true;
}

#ifdef $XCC_h
/**
 * Pretty print of this Temporary.
 */
bool
Temporary_pretty(const_Temporary this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
#include <ctype.h>
bool
Temporary_pretty(const_Temporary this, FILE *file)
{
  if (this == NULL) return true;
  if (file == NULL) file = stderr;
  if (Temporary_hasRegFile(this)) {
    RegFile regFile = Temporary_REGFILE(this);
    Register registre = Temporary_REGISTER(this);
    const char *name = RegFile_name(regFile);
    char c = Temporary_isGlobal(this)? toupper(*name): tolower(*name);
    if (Temporary_isDedicated(this)) {
      Except_CHECK(Temporary_hasRegister(this));
      fprintf(file, "%s", Register_names(registre)[0]);
    } else if (Temporary_isAssigned(this)) {
      Except_CHECK(Temporary_hasRegister(this));
      fprintf(file, "%c%d", c, Temporary_IDENTITY(this));
      fprintf(file, "(%s)", Register_names(registre)[0]);
    } else if (Temporary_isVirtual(this)) {
      fprintf(file, "%c%d", c, Temporary_IDENTITY(this));
    } else if (Temporary_isVariable(this)) {
      Variable_pretty(Temporary_VARIABLE(this), file, 0);
    } else {
      fprintf(file, "ERROR(Register)");
    }
  } else if (Temporary_hasLiteral(this)) {
    Literal literal = Temporary_LITERAL(this);
    if (Temporary_isAbsolute(this)) {
      fprintf(file, "%lld", (long long)Literal_VALUE(literal));
    } else if (Temporary_isSymbol(this)) {
      Symbol symbol = Literal_SYMBOL(literal);
      ImmediateValue offset = Literal_OFFSET(literal);
      if (Symbol_isResolved(symbol))
          fprintf(file, "%lld", (long long)Symbol_value(symbol)+offset);
      else {
        if (offset == 0)
          fprintf(file, "%s", Symbol_name(symbol));
        else
          fprintf(file, "%s%+lld", Symbol_name(symbol), (long long)offset);
      }
    } else if (Temporary_isLabel(this)) {
      Label label = Literal_LABEL(literal);
      ImmediateValue offset = Literal_OFFSET(literal);
      if (offset == 0)
        fprintf(file, "%s", Label_name(label));
      else
        fprintf(file, "%s%+lld", Label_name(label), (long long)offset);
    } else {
      fprintf(file, "ERROR(Literal)");
    }
  } else if (Temporary_isModifier(this)) {
    ModifierMember member = Temporary_MEMBER(this);
    fprintf(file, "%s", ModifierMember_name(member));
  } else if (Temporary_isRegMask(this)) {
    fprintf(file, "RegMask)");
  } else if (Temporary_isVolatile(this)) {
    fprintf(file, "Volatile");
  } else if (Temporary_isControl(this)) {
    fprintf(file, "Control");
  } else if (Temporary_isMemory(this)) {
    fprintf(file, "Memory");
  } else {
    fprintf(file, "ERROR(?)");
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Asm print of this Temporary.
 */
bool
Temporary_asmPrint(const_Temporary this, FILE *file, bool printHex, int modifier);
#endif//$XCC_h

#include <ctype.h>
bool
Temporary_asmPrint(const_Temporary this, FILE *file, bool printHex, int modifier)
{
  if (this == NULL) return true;
  if (file == NULL) file = stderr;
  if (Temporary_hasRegFile(this)) {
    RegFile regFile = Temporary_REGFILE(this);
    Register registre = Temporary_REGISTER(this);
    const char *name = RegFile_name(regFile);
    char c = Temporary_isGlobal(this)? toupper(*name): tolower(*name);
    if (Temporary_isDedicated(this) || Temporary_isAssigned(this)) {
      char * ptr;
      char registerName[16];

      Except_CHECK(Temporary_hasRegister(this));
      strcpy(registerName, Register_names(registre)[0]);

      if (RegFile_width(regFile) == 32) {
        fprintf(file, "%s", registerName);
      } else {
        switch (modifier) {
          case 's':
            ptr = strrchr(registerName, 'r');
            *ptr = '\0';
            fprintf(file, "%s", registerName);
            break;
          case 'S':
            ptr = strrchr(registerName, 'r');
            fprintf(file, "$%s", ptr);
            break;
          default:
            fprintf(file, "%s", registerName);
        }
      }
    } else if (Temporary_isVirtual(this)) {
      fprintf(file, "%c%d", c, Temporary_IDENTITY(this));
    } else if (Temporary_isVariable(this)) {
      registre = Variable_register(Temporary_variable(this));
      if (registre != Register__UNDEF)
        fprintf(file, "%s", Register_names(registre)[0]);
      else
        fprintf(file, "ERROR(Register)");
    } else {
      fprintf(file, "ERROR(Register)");
    }
  } else if (Temporary_hasLiteral(this)) {
    Literal literal = Temporary_LITERAL(this);
    if (Temporary_isAbsolute(this)) {
      if (printHex)
        fprintf(file, "0x%lx", (long unsigned int) Literal_VALUE(literal));
      else
        fprintf(file, "%ld", (long int) Literal_VALUE(literal));
    } else if (Temporary_isSymbol(this)) {
      Symbol symbol = Literal_SYMBOL(literal);
      Relocation relocation = Literal_relocation(literal);
      ImmediateValue offset = Literal_offset(literal);
      if (Symbol_isResolved(symbol))
          fprintf(file, "%lld", (long long int) Symbol_value(symbol)+offset);
      else {
        char origin[32];
        char * relocationToken = NULL;

        if (relocation != Relocation__UNDEF) {
          relocationToken = strchr(Relocation_Id(relocation), (int) '_') + 1;
          switch (Relocation_relative(relocation)) {
            case RelocationRelative_GOT:
              {
                if (strstr(relocationToken, "funcdesc") != NULL) {
                  strcpy(origin, strchr(relocationToken, (int) '_') + 1);
                  strcat(origin, "_funcdesc");
                  relocationToken = origin;
                }
              }
              break;
            case RelocationRelative_TP:
            case RelocationRelative_GP:
              if (relocationToken[0] == 's')
                relocationToken = strrchr(Relocation_Id(relocation), (int) '_') + 1;
              break;
            default:
              /* Do nothing, no relocation token necessary */
              break;
          }
        }

        if (relocationToken) {
          if (offset == 0)
            fprintf(file, "@%s(%s)", relocationToken, Symbol_name(symbol));
          else
            fprintf(file, "@%s(%s)%+lld", relocationToken, Symbol_name(symbol), (long long)offset);
        } else {
          if (offset == 0)
            fprintf(file, "%s", Symbol_name(symbol));
          else
            fprintf(file, "%s%+lld", Symbol_name(symbol), (long long)offset);
        }
      }
    } else if (Temporary_isLabel(this)) {
      Label label = Literal_LABEL(literal);
      ImmediateValue offset = Literal_OFFSET(literal);
      if (offset == 0)
        fprintf(file, "%s", Label_name(label));
      else
        fprintf(file, "%s%+lld", Label_name(label), (long long)offset);
    } else {
      fprintf(file, "ERROR(Literal)");
    }
  } else if (Temporary_isModifier(this)) {
    ModifierMember member = Temporary_MEMBER(this);
    fprintf(file, "%s", ModifierMember_name(member));
  } else if (Temporary_isRegMask(this)) {
    fprintf(file, "RegMask)");
  } else if (Temporary_isVolatile(this)) {
    fprintf(file, "Volatile");
  } else if (Temporary_isControl(this)) {
    fprintf(file, "Control");
  } else if (Temporary_isMemory(this)) {
    fprintf(file, "Memory");
  } else {
    fprintf(file, "ERROR(?)");
  }
  return true;
}

#ifdef $XCC_h
/**
 * Pretty print of this Temporary in Minir format.
 */
bool
Temporary_prettyMinir(const_Temporary this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
#include <ctype.h>
bool
Temporary_prettyMinir(const_Temporary this, FILE *file)
{
  if (this == NULL) return true;
  if (file == NULL) file = stderr;
  if (Temporary_hasRegFile(this)) {
    RegFile regFile = Temporary_REGFILE(this);
    Register registre = Temporary_REGISTER(this);
    const char *name = RegFile_name(regFile);
    char c = Temporary_isGlobal(this)? toupper(*name): tolower(*name);
    if (Temporary_isDedicated(this)) {
      Except_CHECK(Temporary_hasRegister(this));
      fprintf(file, "%s", Register_names(registre)[0]);
    } else if (Temporary_isAssigned(this)) {
      Except_CHECK(Temporary_hasRegister(this));
      fprintf(file, "%c%d", c, Temporary_IDENTITY(this));
      /* fprintf(file, "(%s)", Register_names(registre)[0]);*/
    } else if (Temporary_isVirtual(this)) {
      fprintf(file, "%c%d", c, Temporary_IDENTITY(this));
    } else if (Temporary_isVariable(this)) {
      Variable_prettyMinir(Temporary_VARIABLE(this), file, 0);
    } else {
      fprintf(file, "ERROR(Register)");
    }
  } else if (Temporary_hasLiteral(this)) {
    Literal literal = Temporary_LITERAL(this);
    if (Temporary_isAbsolute(this)) {
      fprintf(file, "'%lld'", (long long)Literal_VALUE(literal));
    } else if (Temporary_isSymbol(this)) {
      Symbol symbol = Literal_SYMBOL(literal);
      ImmediateValue offset = Literal_OFFSET(literal);
      if (offset == 0)
        fprintf(file, "'(%s)'", Symbol_name(symbol));
      else
        fprintf(file, "'(%s%+lld)'", Symbol_name(symbol), (long long)offset);
    } else if (Temporary_isLabel(this)) {
      Label label = Literal_LABEL(literal);
      ImmediateValue offset = Literal_OFFSET(literal);
      if (offset == 0)
        fprintf(file, "'(%s)'", Label_name(label));
      else
        fprintf(file, "'(%s%+lld)'", Label_name(label), (long long)offset);
    } else {
      fprintf(file, "ERROR(Literal)");
    }
  } else if (Temporary_isModifier(this)) {
    /* handled in Operation */
    return false;
  } else if (Temporary_isRegMask(this)) {
    fprintf(file, "RegMask)");
  } else if (Temporary_isVolatile(this)) {
    fprintf(file, "Volatile");
  } else if (Temporary_isControl(this)) {
    fprintf(file, "Control");
  } else if (Temporary_isMemory(this)) {
    fprintf(file, "Memory");
  } else {
    fprintf(file, "ERROR(?)");
  }
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * True if the lifetime of this Temporary must be considered.
 */
static inline bool
Temporary_isLifetime(Temporary this)
{
  Convention convention = Convention__DEFAULT;	//!< FIXME!
  return    Temporary_isVirtual(this)
         || (   Temporary_hasRegFile(this)
             && Register_isLifetime(Temporary_REGISTER(this), convention));
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Assign a Register to this Temporary.
 */
void
Temporary_assignRegister(Temporary this, Register registre);
#endif//$XCC_h

void
Temporary_assignRegister(Temporary this, Register registre)
{
  Except_REQUIRE(Temporary_hasRegFile(this));
  *Temporary__FLAGS(this) |= TemporaryFlag_Register;
  /* FIXME! Check that registre belongs to Temporary_REGFILE. */
  *Temporary__REGISTER(this) = registre;
}

#ifdef $XCC_h
/**
 * Un-assign this Temporary.
 */
void
Temporary_unassignRegister(Temporary this);
#endif//$XCC_h

void
Temporary_unassignRegister(Temporary this)
{
  Register registre = Temporary_REGISTER(this);
  Except_REQUIRE(Temporary_hasRegFile(this));
  *Temporary__FLAGS(this) &= ~TemporaryFlag_Register;
  *Temporary__REGISTER(this) = Register__UNDEF;
}

#ifdef $XCC_h
/**
 * Set the bit-width of this Register Temporary.
 */
static inline void
Temporary_setWidth(Temporary this, unsigned width)
{
  *Temporary__WIDTH(this) = width;
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Stack of Temporary(ies).
 */
struct TemporaryStack_ {
  //@args	Memory memory, int maxCount
  PtrSeq_ __;		//!< Underlying PtrSeq.
};
#endif//$XCC_h

#ifdef $XCC_h
static inline PtrSeq
TemporaryStack__(TemporaryStack this)
{
  return TemporaryStack____(this);
}
static inline const_PtrSeq
TemporaryStack__const(const_TemporaryStack this)
{
  return TemporaryStack____(this);
}
#endif//$XCC_h

TemporaryStack
TemporaryStack_Ctor(TemporaryStack this,
                    Memory memory, int maxCount)
{
  PtrSeq_Ctor(TemporaryStack____(this), memory, maxCount);
  return this;
}

void
TemporaryStack_Dtor(TemporaryStack this)
{
  PtrSeq_Dtor(TemporaryStack____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(TemporaryStack_)\t%zu\n", sizeof(TemporaryStack_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Iterate over this TemporaryStack Temporary(ies).
 */
#define TemporaryStack_FOREACH_Temporary(this, temporary) { \
  const_PtrSeq TemporaryStack____ = TemporaryStack__const(this); \
  PtrSeq_FOREACH(TemporaryStack____, Temporary, temporary) {
#define TemporaryStack_ENDEACH_Temporary \
  } PtrSeq_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this TemporaryStack Temporary(ies).
 */
#define TemporaryStack_FORBACK_Temporary(this, temporary) { \
  const_PtrSeq TemporaryStack____ = TemporaryStack__const(this); \
  PtrSeq_FORBACK(TemporaryStack____, Temporary, temporary) {
#define TemporaryStack_ENDBACK_Temporary \
  } PtrSeq_ENDBACK \
}
#endif//$XCC_h

#ifdef $XCC_h
#define TemporaryStack_resize(this, maxCount) \
  PtrSeq_resize(TemporaryStack__(this), maxCount)
#define TemporaryStack_empty(this) \
  PtrSeq_empty(TemporaryStack__(this))
#define TemporaryStack_memory(this) \
  PtrSeq_memory(TemporaryStack__const(this))
#define TemporaryStack_base_(this) (Temporary *) \
  PtrSeq_base_(TemporaryStack__(this))
#define TemporaryStack_base(this) (const Temporary *) \
  PtrSeq_base(TemporaryStack__const(this))
#define TemporaryStack_past(this) (const Temporary *) \
  PtrSeq_past(TemporaryStack__const(this))
#define TemporaryStack_isEmpty(this) \
  PtrSeq_isEmpty(TemporaryStack__const(this))
#define TemporaryStack_isSingle(this) \
  PtrSeq_isSingle(TemporaryStack__const(this))
#define TemporaryStack_isFull(this) \
  PtrSeq_isFull(TemporaryStack__const(this))
#define TemporaryStack_count(this) \
  PtrSeq_count(TemporaryStack__const(this))
#define TemporaryStack_access(this, index) (Temporary) \
  PtrSeq_access(TemporaryStack__const(this), index)
#define TemporaryStack_bottom(this) (Temporary) \
  PtrSeq_bottom(TemporaryStack__const(this))
#define TemporaryStack_top(this) (Temporary) \
  PtrSeq_top(TemporaryStack__const(this))
#define TemporaryStack_pop(this) \
  PtrSeq_pop(TemporaryStack__(this))
#define TemporaryStack_push(this, temporary) \
  PtrSeq_push(TemporaryStack__(this), temporary)
#define TemporaryStack_push2(this, temporary) \
  PtrSeq_push2(TemporaryStack__(this), temporary)
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Set of Temporary(ies).
 */
struct TemporarySet_ {
  //@args	Memory memory, int32_t estimate
  IndexedSet_ __;		//!< The IndexedSet_ that implements the set.
  //@access MEMORY	IndexedSet_memory(TemporarySet____(this))
};
#endif//$XCC_h

#ifdef $XCC_h
static inline IndexedSet
TemporarySet__(TemporarySet this)
{
  return TemporarySet____(this);
}
static inline const_IndexedSet
TemporarySet__const(const_TemporarySet this)
{
  return TemporarySet____(this);
}
#endif//$XCC_h

TemporarySet
TemporarySet_Ctor(TemporarySet this,
                  Memory memory, int32_t estimate)
{
  IndexedSet_Ctor(TemporarySet____(this), memory, estimate);
  return this;
}

void
TemporarySet_Dtor(TemporarySet this)
{
  IndexedSet_Dtor(TemporarySet____(this));
}

size_t
TemporarySet_Size(Memory memory, int32_t estimate)
{
        return sizeof(TemporarySet_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(TemporarySet_)\t%zu\n", sizeof(TemporarySet_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Iterate over this TemporarySet Temporary(ies).
 */
#define TemporarySet_FOREACH_Temporary(this, temporary) { \
  const_IndexedSet TemporarySet____ = TemporarySet__const(this); \
  IndexedSet_FOREACH_Indexed(TemporarySet____, indexed) { \
    Temporary temporary = (Temporary)indexed;
#define TemporarySet_ENDEACH_Temporary \
  } IndexedSet_ENDEACH_Indexed \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this TemporarySet sorted Temporary(ies).
 */
#define TemporarySet_FORSORT_Temporary(this, compare, temporary) { \
  const_IndexedSet TemporarySet____ = TemporarySet__const(this); \
  IndexedSet_FORSORT_Indexed(TemporarySet____, compare, indexed) { \
    Temporary temporary = (Temporary)indexed;
#define TemporarySet_ENDSORT_Temporary \
  } IndexedSet_ENDSORT_Indexed \
}
#endif//$XCC_h

#ifdef $XCC_h
#define TemporarySet_memory(this) \
  IndexedSet_memory(TemporarySet__const(this))
#define TemporarySet_base_(this) (Temporary *) \
  IndexedSet_base_(TemporarySet__(this))
#define TemporarySet_base(this) \
  ((const Temporary *)IndexedSet_base(TemporarySet__const(this)))
#define TemporarySet_past(this) \
  ((const Temporary *)IndexedSet_past(TemporarySet__const(this)))
#define TemporarySet_isOrdered(this) \
  IndexedSet_isOrdered(TemporarySet__const(this))
#define TemporarySet_count(this) \
  IndexedSet_count(TemporarySet__const(this))
#define TemporarySet_isEmpty(this) \
  IndexedSet_isEmpty(TemporarySet__const(this))
#define TemporarySet_isSingle(this) \
  IndexedSet_isSingle(TemporarySet__const(this))
#define TemporarySet_empty(this) \
  IndexedSet_empty(TemporarySet__(this))
#define TemporarySet_clearOrdered(this) \
  IndexedSet_clearOrdered(TemporarySet__(this))
#define TemporarySet_raiseOrdered(this) \
  IndexedSet_raiseOrdered(TemporarySet__(this))
#define TemporarySet_pushMember(this, temporary)	\
  IndexedSet_pushMember(TemporarySet__(this), (Indexed)temporary)
#define TemporarySet_makeOrder(this) \
  IndexedSet_makeOrder(TemporarySet__(this))
#define TemporarySet_contains(this, temporary)	\
  IndexedSet_contains(TemporarySet__const(this), (Indexed)temporary)
#define TemporarySet_insert(this, temporary)	\
  IndexedSet_insert(TemporarySet__(this), (Indexed)temporary)
#define TemporarySet_remove(this, temporary)	\
  IndexedSet_remove(TemporarySet__(this), (Indexed)temporary)
#define TemporarySet_equals(this, that)	\
  IndexedSet_equals(TemporarySet__const(this), TemporarySet__const(that))
#define TemporarySet_assign(this, that)	\
  IndexedSet_assign(TemporarySet__(this), TemporarySet__const(that))
#define TemporarySet_union(this, that)	\
  IndexedSet_union(TemporarySet__(this), TemporarySet__const(that))
#define TemporarySet_inter(this, that)	\
  IndexedSet_inter(TemporarySet__(this), TemporarySet__const(that))
#define TemporarySet_diff(this, that)	\
  IndexedSet_diff(TemporarySet__(this), TemporarySet__const(that))
#define TemporarySet_comprises(this, that)	\
  IndexedSet_comprises(TemporarySet__const(this), TemporarySet__const(that))
#define TemporarySet_overlaps(this, that)	\
  IndexedSet_overlaps(TemporarySet__const(this), TemporarySet__const(that))
#define TemporarySet_choose(this) \
  ((Temporary)IndexedSet_choose(TemporarySet__(this)))
#endif//$XCC_h

#ifdef $XCC_h
TemporarySet
TemporarySet_make(Memory memory, int32_t estimate);
#endif//$XCC_h

TemporarySet
TemporarySet_make(Memory memory, int32_t estimate)
{
  size_t size = TemporarySet_Size(memory, estimate);
  TemporarySet this = Memory_alloc(memory, size);
  return TemporarySet_Ctor(this, memory, estimate);
}

#ifdef $XCC_h
TemporarySet
TemporarySet_kill(TemporarySet this);
#endif//$XCC_h

TemporarySet
TemporarySet_kill(TemporarySet this)
{
  if (this != NULL) {
    Memory memory = TemporarySet_MEMORY(this);
    TemporarySet_Dtor(this);
    Memory_free_(memory, this);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Pretty-print this TemporarySet.
 */
bool
TemporarySet_pretty(TemporarySet this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
TemporarySet_pretty(TemporarySet this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "{");
  TemporarySet_FORSORT_Temporary(this, Temporary_compare, temporary) {
    fprintf(file, " ");
    Temporary_pretty(temporary, file);
  } TemporarySet_ENDSORT_Temporary;
  fprintf(file, " }");
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Display this TemporarySet in VCG format.
 */
bool
TemporarySet_display(TemporarySet this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
TemporarySet_display(TemporarySet this, FILE *file)
{
  if (file == NULL) file = stderr;
  if (this == NULL || TemporarySet_isEmpty(this)) {
    fprintf(file, "--");
  } else {
    int i = 1;
    int count = TemporarySet_count(this);
    TemporarySet_FORSORT_Temporary(this, Temporary_compare, temporary) {
      Temporary_pretty(temporary, file);
      if (i % 4 == 0 && i != count)
        fprintf(file, "\\n");
      else
        fprintf(file, "\\ ");
      i++;
    } TemporarySet_ENDSORT_Temporary;
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Specialization of IndexedTable for Temporary(ies).
 */
struct TemporaryTable_ {
  //@args	Memory memory, int32_t estimate
  IndexedTable_ __;
  //@access MEMORY	IndexedTable_memory(TemporaryTable____(this))
  //@access COUNT	IndexedTable_count(TemporaryTable____(this))
  //@access BASE	(const Temporary *)IndexedTable_base(TemporaryTable____(this))
};
#endif//$XCC_h

#ifdef $XCC_h
static inline IndexedTable
TemporaryTable__(TemporaryTable this)
{
  return TemporaryTable____(this);
}
static inline const_IndexedTable
TemporaryTable__const(const_TemporaryTable this)
{
  return TemporaryTable____(this);
}
#endif//$XCC_h

TemporaryTable
TemporaryTable_Ctor(TemporaryTable this,
                    Memory memory, int32_t estimate)
{
  IndexedTable_Ctor(TemporaryTable____(this), memory, estimate);
  return this;
}

void
TemporaryTable_Dtor(TemporaryTable this)
{
  IndexedTable_Dtor(TemporaryTable____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(TemporaryTable_)\t%zu\n", sizeof(TemporaryTable_));
}
#endif//$XCC__c

#ifdef $XCC_h
#define TemporaryTable_memory(this) \
  IndexedTable_memory(TemporaryTable__const(this))
#define TemporaryTable_isEmpty(this) \
  IndexedTable_isEmpty(TemporaryTable__const(this))
#define TemporaryTable_isSingle(this) \
  IndexedTable_isSingle(TemporaryTable__const(this))
#define TemporaryTable_count(this) \
  IndexedTable_count(TemporaryTable__const(this))
#define TemporaryTable_empty(this) \
  IndexedTable_empty(TemporaryTable__(this))
#define TemporaryTable_contains(this, temporary) \
  IndexedTable_contains(TemporaryTable__const(this), Temporary__const(temporary))
#define TemporaryTable_insert(this, temporary) \
  IndexedTable_insert(TemporaryTable__(this), Temporary__(temporary))
#define TemporaryTable_remove(this, temporary) \
  IndexedTable_remove(TemporaryTable__(this), Temporary__const(temporary))
#define TemporaryTable_removeHigh(this, temporary, high) \
  IndexedTable_removeHigh(TemporaryTable__(this), Temporary__const(temporary), high)
#define TemporaryTable_makeOrder(this) \
  IndexedTable_makeIndexedSetOrder(TemporaryTable__(this))
#endif//$XCC_h

#ifdef $XCC_h
static inline const Temporary *
TemporaryTable_base(const_TemporaryTable this)
{
  return TemporaryTable_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this TemporaryTable Temporary(ies).
 */
#define TemporaryTable_FOREACH_Temporary(this, temporary) { \
  IndexedTable_FOREACH_Indexed(TemporaryTable__const(this), \
                                   Temporary, temporary) { \
    int32_t TemporaryTable_Temporary_INDEX = IndexedTable_INDEX;
#define TemporaryTable_ENDEACH_Temporary \
  } IndexedTable_ENDEACH_Indexed; \
}
#endif//$XCC_h

#ifdef $XCC_h
bool
TemporaryTable_pretty(const_TemporaryTable this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
TemporaryTable_pretty(const_TemporaryTable this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "{");
  TemporaryTable_FOREACH_Temporary(this, temporary) {
    fprintf(file, " [%d]", Temporary_INDEX(temporary));
    Temporary_pretty(temporary, file);
  } TemporaryTable_ENDEACH_Temporary;
  fprintf(file, " }");
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Sparse set of Temporary(ies) based on INDEX Sparse set.
 */
struct TemporarySparse_ {
  //@args	Memory memory, TemporaryTable table
  IndexedSparse_ __;
  //@access MEMORY	IndexedSparse_memory(TemporarySparse____(this))
  //@access BASE	(const Temporary *)IndexedSparse_base(TemporarySparse____(this))
};
#endif//$XCC_h

#ifdef $XCC_h
static inline IndexedSparse
TemporarySparse__(TemporarySparse this)
{
  return TemporarySparse____(this);
}
static inline const_IndexedSparse
TemporarySparse__const(const_TemporarySparse this)
{
  return TemporarySparse____(this);
}
#endif//$XCC_h

TemporarySparse
TemporarySparse_Ctor(TemporarySparse this,
                     Memory memory, TemporaryTable table)
{
  IndexedTable indexedTable = TemporaryTable__(table);
  IndexedSparse_Ctor(TemporarySparse____(this), memory, indexedTable);
  return this;
}

void
TemporarySparse_Dtor(TemporarySparse this)
{
  IndexedSparse_Dtor(TemporarySparse____(this));
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(TemporarySparse_)\t%zu\n", sizeof(TemporarySparse_));
}
#endif//$XCC__c

#ifdef $XCC_h
#define TemporarySparse_empty(this) \
  IndexedSparse_empty(TemporarySparse__(this))
#define TemporarySparse_memory(this) \
  IndexedSparse_memory(TemporarySparse__const(this))
#define TemporarySparse_isEmpty(this) \
  IndexedSparse_isEmpty(TemporarySparse__const(this))
#define TemporarySparse_isSingle(this) \
  IndexedSparse_isSingle(TemporarySparse__const(this))
#define TemporarySparse_count(this) \
  IndexedSparse_count(TemporarySparse__const(this))
#define TemporarySparse_choose(this) \
  IndexedSparse_choose(TemporarySparse__(this))
#define TemporarySparse_contains(this, temporary) \
  IndexedSparse_contains(TemporarySparse__const(this), Temporary__const(temporary))
#define TemporarySparse_insert(this, temporary) \
  IndexedSparse_insert(TemporarySparse__(this), Temporary__const(temporary))
#define TemporarySparse_remove(this, temporary) \
  IndexedSparse_remove(TemporarySparse__(this), Temporary__const(temporary))
#define TemporarySparse_equals(this, that) \
  IndexedSparse_equals(TemporarySparse__const(this), TemporarySparse__const(that))
#define TemporarySparse_assign(this, that) \
  IndexedSparse_assign(TemporarySparse__(this), TemporarySparse__const(that))
#define TemporarySparse_union(this, that) \
  IndexedSparse_union(TemporarySparse__(this), TemporarySparse__const(that))
#define TemporarySparse_inter(this, that) \
  IndexedSparse_inter(TemporarySparse__(this), TemporarySparse__const(that))
#define TemporarySparse_diff(this, that) \
  IndexedSparse_diff(TemporarySparse__(this), TemporarySparse__const(that))
#define TemporarySparse_position(this, temporary) \
  IndexedSparse_position(TemporarySparse__const(this), Temporary__const(temporary))
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this TemporarySparse Temporary(ies).
 */
#define TemporarySparse_FOREACH_Temporary(this, temporary) { \
  IndexedSparse_FOREACH_Indexed(TemporarySparse__const(this), Temporary, temporary) { \
    int32_t TemporarySparse_POSITION = IndexedSparse_POSITION; \
    int32_t TemporarySparse_INDEX = IndexedSparse_INDEX;
#define TemporarySparse_ENDEACH_Temporary \
  } IndexedSparse_ENDEACH_Indexed; \
}
#endif//$XCC_h

#ifdef $XCC_h
static inline TemporaryTable
TemporarySparse_table(TemporarySparse this)
{
  return (TemporaryTable)IndexedSparse_table(TemporarySparse____(this));
}
static inline const_TemporaryTable
TemporarySparse_table_const(const_TemporarySparse this)
{
  return (const_TemporaryTable)IndexedSparse_table_const(TemporarySparse____(this));
}
#endif//$XCC_h

#ifdef $XCC_h
bool
TemporarySparse_pretty(const_TemporarySparse this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
TemporarySparse_pretty(const_TemporarySparse this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "{");
  TemporarySparse_FOREACH_Temporary(this, temporary) {
    fprintf(file, " [%d]", Temporary_INDEX(temporary));
    Temporary_pretty(temporary, file);
  } TemporarySparse_ENDEACH_Temporary;
  fprintf(file, " }");
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Map Temporary to user-defined object.
 *
 * This map is based on the Temporary_INDEX fields, which must be properly
 * initialized for instance by entering the Temporary(ies) in a TemporaryTable
 * before the TemporaryMap is constructed.
 */
struct TemporaryMap_ {
  //@args	Memory memory, int32_t maxCount, size_t itemSize
  Memory MEMORY;
  int32_t MAXCOUNT;
  size_t ITEMSIZE;
  void *BASE;
};
#endif//$XCC_h

TemporaryMap
TemporaryMap_Ctor(TemporaryMap this,
                  Memory memory, int32_t maxCount, size_t itemSize)
{
  size_t totalSize = maxCount*itemSize;
  *TemporaryMap__MEMORY(this) = memory;
  *TemporaryMap__MAXCOUNT(this) = 0;
  *TemporaryMap__ITEMSIZE(this) = itemSize;
  *TemporaryMap__BASE(this) = NULL;
  if (maxCount > 0) TemporaryMap_resize(this, maxCount);
  return this;
}

void
TemporaryMap_Dtor(TemporaryMap this)
{
  TemporaryMap_release(this, NULL);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(TemporaryMap_)\t%zu\n", sizeof(TemporaryMap_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Resize this TemporaryMap with a new maxCount.
 */
void
TemporaryMap_resize(TemporaryMap this, int32_t maxCount);
#endif//$XCC_h

void
TemporaryMap_resize(TemporaryMap this, int32_t maxCount)
{
  Memory memory = TemporaryMap_MEMORY(this);
  size_t itemSize = TemporaryMap_ITEMSIZE(this);
  size_t totalSize = maxCount*itemSize;
  Except_REQUIRE(TemporaryMap_MAXCOUNT(this) == 0);
  if (maxCount > 0) {
    *TemporaryMap__MAXCOUNT(this) = maxCount;
    *TemporaryMap__BASE(this) = Memory_alloc(memory, totalSize);
  }
}

#ifdef $XCC_h
/**
 * Resize this TemporaryMap memory.
 */
void
TemporaryMap_release(TemporaryMap this, void (*release)(void * item));
#endif//$XCC_h

void
TemporaryMap_release(TemporaryMap this, void (*release)(void * item))
{
  Memory memory = TemporaryMap_MEMORY(this);
  void *base = TemporaryMap_BASE(this);
  if (base != NULL && release != NULL) {
    char *item = base;
    size_t itemSize = TemporaryMap_ITEMSIZE(this);
    int32_t maxCount = TemporaryMap_MAXCOUNT(this), i;
    for (i = 0; i < maxCount; i++) {
      (*release)((void *)item);
      item += itemSize;
    }
  }
  Memory_free(memory, base);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * For use in TemporaryMap_FOREACH.
 */
static inline void *
TemporaryMap_base(TemporaryMap this)
{
  return TemporaryMap_BASE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use in TemporaryMap_FOREACH.
 */
static inline void *
TemporaryMap_past(TemporaryMap this)
{
  char *base = TemporaryMap_BASE(this);
  int32_t maxCount = TemporaryMap_MAXCOUNT(this);
  size_t itemSize = TemporaryMap_ITEMSIZE(this);
  return base + maxCount*itemSize;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * For use in TemporaryMap_FOREACH.
 */
static inline size_t
TemporaryMap_itemSize(TemporaryMap this)
{
  return TemporaryMap_ITEMSIZE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate over this TemporaryMap objects.
 */
#define TemporaryMap_FOREACH(this, Type, iter) { \
  size_t TemporaryMap_ITEMSIZE = TemporaryMap_itemSize(this); \
  Type *TemporaryMap_BASE = (Type *)TemporaryMap_base(this); \
  Type *TemporaryMap_PAST = (Type *)TemporaryMap_past(this), *(iter); \
  for (iter = TemporaryMap_BASE; iter < TemporaryMap_PAST; ++(iter)) { \
    int32_t TemporaryMap_INDEX = iter - TemporaryMap_BASE;
#define TemporaryMap_ENDEACH \
  } \
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Access the object mapped by the temporary.
 */
void *
TemporaryMap_access(TemporaryMap this, Temporary temporary);
#endif//$XCC_h

void *
TemporaryMap_access(TemporaryMap this, Temporary temporary)
{
  int32_t index = Temporary_INDEX(temporary);
  char *base = TemporaryMap_BASE(this);
  int32_t maxCount = TemporaryMap_MAXCOUNT(this);
  size_t itemSize = TemporaryMap_ITEMSIZE(this);
  Except_REQUIRE(index >= 0 && index < maxCount);
  return base + itemSize*index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize Temporary.
 */
void
Temporary_INIT(void);
#endif//$XCC__h

IndexedFactory_ Temporary_Factory[1];

void
Temporary_INIT(void)
{
  IndexedFactory_Ctor(Temporary_Factory);
#ifndef _NTRACE
  ETable_Ctor(TemporaryFlag_Table, Memory_Root, TemporaryFlagEntries);
#endif//_NTRACE
}

#ifdef $XCC__h
/**
 * Finalize Temporary.
 */
#ifndef _NTRACE
void
Temporary_FINI(void);
#else//_NTRACE
#define Temporary_FINI()
#endif//_NTRACE
#endif//$XCC__h

#ifndef _NTRACE
void
Temporary_FINI(void)
{
  ETable_Dtor(TemporaryFlag_Table);
}
#endif//_NTRACE

#if XCC__C
static void
Temporary_TEST(void)
{
#include "LIR/Temporary_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Temporary_TEST);
  return 0;
}
#endif

