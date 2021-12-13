#ifdef $XCC_h
/*
 * !!!!	Convention.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 *
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

#include "MDT/MDT_.h"

#ifdef $XCC__h
#include "MDT/Convention.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Convention(ID, RESERVED, ARGUMENT, RESULT, STRUCT, CALLER, CALLEE, PROGRAM, HANDLER, \
                   VENEER, RETURN, STACK, STATIC, FRAME, GLOBAL, LOCAL, WIRED, ZERO, ONE) \
  Convention_##ID,
  Convention_,
#include /*MDT*/"Convention.tuple"
  Convention__
#undef Convention
} enum_Convention;
#endif//$XCC__h

const int Convention__Count = Convention__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerate the Convention Register roles.
 */
typedef enum {
  ConventionRole_Reserved,		//!< Reserved (non allocatable) Register(s).
  ConventionRole_Argument,		//!< Function call argument Register(s).
  ConventionRole_Result,		//!< Function call result Register(s).
  ConventionRole_Struct,		//!< Pointer to result structure Register(s).
  ConventionRole_Caller,		//!< Caller saved (scratch) Register(s).
  ConventionRole_Callee,		//!< Callee saved (preserved) Register(s).
  ConventionRole_Program,		//!< Program counter Register(s).
  ConventionRole_Handler,		//!< Exception Handler return Register(s).
  ConventionRole_Veneer,		//!< Intra-procedure-call temporary Register(s).
  ConventionRole_Return,		//!< Return address (link) Register(s).
  ConventionRole_Stack,			//!< Stack pointer Register(s)
  ConventionRole_Frame,			//!< Frame pointer Register(s).
  ConventionRole_Static,		//!< Static link pointer Register(s).
  ConventionRole_Global,		//!< Global data or GOT pointer Register(s).
  ConventionRole_Local,			//!< Local data or TLS pointer Register(s).
  ConventionRole_Wired,			//!< Wired (constant or bucket) Register(s).
  ConventionRole_Zero,			//!< Constant zero value Register(s).
  ConventionRole_One,			//!< Constant one value Register(s).
  ConventionRole__
} enum_ConventionRole;
typedef uint8_t ConventionRole;
extern const char *
ConventionRole_Id(ConventionRole this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
ConventionRole_Id(ConventionRole this)
{
  static const char *_[] = {
    "Reserved",
    "Argument",
    "Result",
    "Struct",
    "Caller",
    "Callee",
    "Program",
    "Handler",
    "Veneer",
    "Return",
    "Stack",
    "Frame",
    "Static",
    "Global",
    "Local",
    "Wired",
    "Zero",
    "One",
  };
  Except_REQUIRE((unsigned)this < ConventionRole__);
  return _[this];
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Convention enumeration.
 */
extern const char *
Convention_Id(Convention this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Convention_Id(Convention this)
{
  static const char *_[] = {
#define Convention(ID, RESERVED, ARGUMENT, RESULT, STRUCT, CALLER, CALLEE, PROGRAM, HANDLER, \
                   VENEER, RETURN, STACK, STATIC, FRAME, GLOBAL, LOCAL, WIRED, ZERO, ONE) \
    #ID,
    "_UNDEF",
#include "Convention.tuple"
  };
  Except_REQUIRE((unsigned)this < Convention__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int convention;
  for (convention = 0; convention < Convention__; convention++) {
    printf("Convention_%s\t%d\n", Convention_Id(convention), convention);
  }
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Check it is a valid convention
 */
bool
Convention_check(Convention this);
#endif//$XCC_h

bool
Convention_check(Convention this)
{
  return this > Convention_ && this < Convention__;
}



////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
// DEPRECATED! This array entries are oudated when register sets are dynamically adjusted.
extern const RegisterList_
Convention_RoleRegisterLists[Convention__][ConventionRole__];
#define Convention_argumentRegisterList(this) \
  (&Convention_RoleRegisterLists[this][ConventionRole_Argument])
#define Convention_resultRegisterList(this) \
  (&Convention_RoleRegisterLists[this][ConventionRole_Result])
#endif//$XCC__h

const RegisterList_
Convention_RoleRegisterLists[Convention__][ConventionRole__] = {
#define REGISTER(ID) Register_##ID,
#define RESERVED(count, items) { count, { items } }
#define ARGUMENT(count, items) { count, { items } }
#define RESULT(count, items) { count, { items } }
#define STRUCT(count, items) { count, { items } }
#define CALLER(count, items) { count, { items } }
#define CALLEE(count, items) { count, { items } }
#define PROGRAM(count, items) { count, { items } }
#define HANDLER(count, items) { count, { items } }
#define VENEER(count, items) { count, { items } }
#define RETURN(count, items) { count, { items } }
#define STACK(count, items) { count, { items } }
#define FRAME(count, items) { count, { items } }
#define STATIC(count, items) { count, { items } }
#define GLOBAL(count, items) { count, { items } }
#define LOCAL(count, items) { count, { items } }
#define WIRED(count, items) { count, { items } }
#define ZERO(count, items) { count, { items } }
#define ONE(count, items) { count, { items } }
#define Convention(ID, RESERVED, ARGUMENT, RESULT, STRUCT, CALLER, CALLEE, PROGRAM, HANDLER, \
                   VENEER, RETURN, STACK, STATIC, FRAME, GLOBAL, LOCAL, WIRED, ZERO, ONE) \
  { RESERVED, ARGUMENT, RESULT, STRUCT, CALLER, CALLEE, PROGRAM, HANDLER, \
    VENEER, RETURN, STACK, FRAME, STATIC, GLOBAL, LOCAL, WIRED, ZERO, ONE },
  { { 0 } },
#include "Convention.tuple"
};

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Convention_RoleRegisterLists)\t%zu\n", sizeof(Convention_RoleRegisterLists));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * RegisterSet(s) indexed by ConventionRole(s).
 */
const_RegisterSet
Convention_roleRegisterSet(Convention this, ConventionRole role);
#endif//$XCC_h

extern RegisterSet_
Convention_RoleRegisterSets[Convention__][ConventionRole__];
const_RegisterSet
Convention_roleRegisterSet(Convention this, ConventionRole role)
{
  Except_REQUIRE((unsigned)this < Convention__);
  Except_REQUIRE((unsigned)role < ConventionRole__);
  return &Convention_RoleRegisterSets[this][role];
}

RegisterSet_
Convention_RoleRegisterSets[Convention__][ConventionRole__];

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Convention_RoleRegisterSets)\t%zu\n", sizeof(Convention_RoleRegisterSets));
}
#endif//$XCC__c

#ifdef $XCC__c
{
  int convention = 0, role = 0;
  for (convention = 0; convention < Convention__; convention++) {
    for (role = 0; role < ConventionRole__; role++) {
      const_RegisterSet registerSet = Convention_roleRegisterSet(convention, role);
      printf("%sSet(%s)", ConventionRole_Id(role), Convention_Id(convention));
      RegisterSet_FOREACH_Register(registerSet, registre) {
        printf("\t%s", Register_Id(registre));
      } RegisterSet_ENDEACH_Register;
      printf("\n");
    }
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
#define Convention_reservedRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Reserved)
#define Convention_argumentRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Argument)
#define Convention_resultRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Result)
#define Convention_structRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Struct)
#define Convention_callerRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Caller)
#define Convention_calleeRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Callee)
#define Convention_programRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Program)
#define Convention_handlerRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Handler)
#define Convention_veneerRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Veneer)
#define Convention_returnRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Return)
#define Convention_stackRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Stack)
#define Convention_frameRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Frame)
#define Convention_staticRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Static)
#define Convention_globalRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Global)
#define Convention_localRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Local)
#define Convention_wiredRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Wired)
#define Convention_zeroRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_Zero)
#define Convention_oneRegisterSet(this) \
  Convention_roleRegisterSet(this, ConventionRole_One)
#endif//$XCC_h

#ifdef $XCC_h
#define Convention_isReservedRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Reserved), registre)
#define Convention_isArgumentRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Argument), registre)
#define Convention_isResultRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Result), registre)
#define Convention_isStructRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Struct), registre)
#define Convention_isCallerRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Caller), registre)
#define Convention_isCalleeRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Callee), registre)
#define Convention_isProgramRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Program), registre)
#define Convention_isHandlerRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Handler), registre)
#define Convention_isVeneerRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Veneer), registre)
#define Convention_isReturnRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Return), registre)
#define Convention_isStackRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Stack), registre)
#define Convention_isFrameRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Frame), registre)
#define Convention_isStaticRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Static), registre)
#define Convention_isGlobalRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Global), registre)
#define Convention_isLocalRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Local), registre)
#define Convention_isWiredRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Wired), registre)
#define Convention_isZeroRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_Zero), registre)
#define Convention_isOneRegister(this, registre) \
  RegisterSet_contains(Convention_roleRegisterSet(this, ConventionRole_One), registre)
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Set a Register ConventionRole for this Convention.
 */
void
Convention_setRole(Convention this, ConventionRole role, Register registre);
#endif//$XCC__h

void
Convention_setRole(Convention this, ConventionRole role, Register registre)
{
  Except_REQUIRE((unsigned)this < Convention__);
  Except_REQUIRE((unsigned)role < ConventionRole__);
  Except_REQUIRE((unsigned)registre < Register__);
  RegisterSet_insert(&Convention_RoleRegisterSets[this][role], registre);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Convention Struct pointer Register.
 */
Register
Convention_structRegister(Convention this);
#endif//$XCC_h

extern Register
Convention_StructRegister[Convention__];
Register
Convention_structRegister(Convention this)
{
  Except_REQUIRE((unsigned)this < Convention__);
  return (Register)Convention_StructRegister[this];
}

Register
Convention_StructRegister[Convention__];

#ifdef $XCC__c
{
  int convention;
  for (convention = 0; convention < Convention__; convention++) {
    Register structRegister = Convention_structRegister(convention);
    printf("StructRegister(%s)\t%s\n", Convention_Id(convention), Register_Id(structRegister));
  }
  fprintf(stderr, "sizeof(Convention_StructRegister)\t%zu\n", sizeof(Convention_StructRegister));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Convention Program pointer Register.
 */
Register
Convention_programRegister(Convention this);
#endif//$XCC_h

extern Register
Convention_ProgramRegister[Convention__];
Register
Convention_programRegister(Convention this)
{
  Except_REQUIRE((unsigned)this < Convention__);
  return (Register)Convention_ProgramRegister[this];
}

Register
Convention_ProgramRegister[Convention__];

#ifdef $XCC__c
{
  int convention;
  for (convention = 0; convention < Convention__; convention++) {
    int programRegister = Convention_programRegister(convention);
    printf("ProgramRegister(%s)\t%s\n", Convention_Id(convention), Register_Id(programRegister));
  }
  fprintf(stderr, "sizeof(Convention_ProgramRegister)\t%zu\n", sizeof(Convention_ProgramRegister));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Convention Return pointer Register.
 */
Register
Convention_returnRegister(Convention this);
#endif//$XCC_h

extern Register
Convention_ReturnRegister[Convention__];
Register
Convention_returnRegister(Convention this)
{
  Except_REQUIRE((unsigned)this < Convention__);
  return (Register)Convention_ReturnRegister[this];
}

Register
Convention_ReturnRegister[Convention__];

#ifdef $XCC__c
{
  int convention;
  for (convention = 0; convention < Convention__; convention++) {
    int returnRegister = Convention_returnRegister(convention);
    printf("ReturnRegister(%s)\t%s\n", Convention_Id(convention), Register_Id(returnRegister));
  }
  fprintf(stderr, "sizeof(Convention_ReturnRegister)\t%zu\n", sizeof(Convention_ReturnRegister));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Convention Stack pointer Register.
 */
Register
Convention_stackRegister(Convention this);
#endif//$XCC_h

extern Register
Convention_StackRegister[Convention__];
Register
Convention_stackRegister(Convention this)
{
  Except_REQUIRE((unsigned)this < Convention__);
  return (Register)Convention_StackRegister[this];
}

Register
Convention_StackRegister[Convention__];

#ifdef $XCC__c
{
  int convention;
  for (convention = 0; convention < Convention__; convention++) {
    int stackRegister = Convention_stackRegister(convention);
    printf("StackRegister(%s)\t%s\n", Convention_Id(convention), Register_Id(stackRegister));
  }
  fprintf(stderr, "sizeof(Convention_StackRegister)\t%zu\n", sizeof(Convention_StackRegister));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Convention Frame pointer Register.
 */
Register
Convention_frameRegister(Convention this);
#endif//$XCC_h

extern Register
Convention_FrameRegister[Convention__];
Register
Convention_frameRegister(Convention this)
{
  Except_REQUIRE((unsigned)this < Convention__);
  return (Register)Convention_FrameRegister[this];
}

Register
Convention_FrameRegister[Convention__];

#ifdef $XCC__c
{
  int convention;
  for (convention = 0; convention < Convention__; convention++) {
    int frameRegister = Convention_frameRegister(convention);
    printf("FrameRegister(%s)\t%s\n", Convention_Id(convention), Register_Id(frameRegister));
  }
  fprintf(stderr, "sizeof(Convention_FrameRegister)\t%zu\n", sizeof(Convention_FrameRegister));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Convention Static pointer Register.
 */
Register
Convention_staticRegister(Convention this);
#endif//$XCC_h

extern Register
Convention_StaticRegister[Convention__];
Register
Convention_staticRegister(Convention this)
{
  Except_REQUIRE((unsigned)this < Convention__);
  return (Register)Convention_StaticRegister[this];
}

Register
Convention_StaticRegister[Convention__];

#ifdef $XCC__c
{
  int convention;
  for (convention = 0; convention < Convention__; convention++) {
    int staticRegister = Convention_staticRegister(convention);
    printf("StaticRegister(%s)\t%s\n", Convention_Id(convention), Register_Id(staticRegister));
  }
  fprintf(stderr, "sizeof(Convention_StaticRegister)\t%zu\n", sizeof(Convention_StaticRegister));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Convention Global pointer Register.
 */
Register
Convention_globalRegister(Convention this);
#endif//$XCC_h

extern Register
Convention_GlobalRegister[Convention__];
Register
Convention_globalRegister(Convention this)
{
  Except_REQUIRE((unsigned)this < Convention__);
  return (Register)Convention_GlobalRegister[this];
}

Register
Convention_GlobalRegister[Convention__];

#ifdef $XCC__c
{
  int convention;
  for (convention = 0; convention < Convention__; convention++) {
    int globalRegister = Convention_globalRegister(convention);
    printf("GlobalRegister(%s)\t%s\n", Convention_Id(convention), Register_Id(globalRegister));
  }
  fprintf(stderr, "sizeof(Convention_GlobalRegister)\t%zu\n", sizeof(Convention_GlobalRegister));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Convention Local pointer Register.
 */
Register
Convention_localRegister(Convention this);
#endif//$XCC_h

extern Register
Convention_LocalRegister[Convention__];
Register
Convention_localRegister(Convention this)
{
  Except_REQUIRE((unsigned)this < Convention__);
  return (Register)Convention_LocalRegister[this];
}

Register
Convention_LocalRegister[Convention__];

#ifdef $XCC__c
{
  int convention;
  for (convention = 0; convention < Convention__; convention++) {
    int localRegister = Convention_localRegister(convention);
    printf("LocalRegister(%s)\t%s\n", Convention_Id(convention), Register_Id(localRegister));
  }
  fprintf(stderr, "sizeof(Convention_LocalRegister)\t%zu\n", sizeof(Convention_LocalRegister));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Returns a Register identifier from a (kind,num) pair.
 * \param kind the char for the register file
 * \param num the register number in the register file starting at 0
 * \return the Register identifier or \e Register_ if (kind,num) is invalid
 */
static Register
getRegister(char kind, int num)
{
  int class = 0;
  kind = tolower(kind);
#ifndef _NTRACE
  for (; class < RegFile__; class++) {
    char id = tolower(*Register_name(RegFile_lowRegister(class)));
    if (id == kind) {
      if (RegFile_lowRegister(class)+num <= RegFile_highRegister(class))
        return RegFile_lowRegister(class)+num;
      else
        return Register_;
    }
  }
#endif//_NTRACE
  return Register_;
}

/**
 * Returns a Register identifier from a register string
 * \param str the string to convert
 * \param pptr a pointer into which the position after match is set
 * \return the Register identifier or \e Register_ if str is invalid
 */
static Register
strRegToRegister(const char *str, const char **pptr)
{
  char kind;
  int num;
  char *ptr2;
  const char *ptr;
  int reg;
  //
  ptr = str;
  if (*ptr == '\0') goto failed;
  kind = *ptr;
  if (!isalpha(kind)) goto failed;
  ptr++;
  num = strtol(ptr, &ptr2, 10);
  if (ptr2 == ptr) goto failed;
  ptr = ptr2;
  //
  reg = getRegister(kind, num);
  if (reg == Register_) goto failed;
  *pptr = ptr;
  return reg;
failed:
  *pptr = str;
  return Register_;
}

/**
 * Add into a RegisterSet the set of registers specified by a register range.
 * \param str the string to convert
 * \param pptr a pointer into which the position after match is set
 * \param registers the RegisterSet to fill
 * \return 0 for success, -1 if the string is an invalid register range
 */
static int
strRangeToRegisters(const char *str, const char **pptr, RegisterSet registers)
{
  char kind;
  int num;
  const char *ptr2;
  const char *ptr;
  int reg1, reg2, reg;
  //
  ptr = str;
  if (*ptr == '\0') goto failed;
  //
  reg1 = strRegToRegister(ptr, &ptr2);
  if (reg1 == Register_) goto failed;
  ptr = ptr2;
  if (*ptr != '-') { reg2 = reg1; }
  else {
    ptr++;
    reg2 = strRegToRegister(ptr, &ptr2);
    if (reg2 == Register_) goto failed;
    ptr = ptr2;
  }
  if (reg2 < reg1) goto failed;
  //
  for (reg = reg1; reg <= reg2; reg++) {
    RegisterSet_insert(registers, reg);
  }
  *pptr = ptr;
  return 0;
failed:
  *pptr = str;
  return -1;
}

/**
 * Add into a BitSet the set of registers specified by a register set.
 * \param str the string to convert
 * \param pptr a pointer into which the position after match is set
 * \param registers the BitSet to fill
 * \return 0 for success, -1 if the string is an invalid register set
 */
static int
strToRegisters(const char *str, const char **pptr, RegisterSet registers)
{
  const char *ptr;
  const char *ptr2;
  int sep;
  //
  ptr = str;
  if (*ptr == '\0') goto end;
  do {
    int status = strRangeToRegisters(ptr, &ptr2, registers);
    if (status == -1) goto failed;
    ptr = ptr2;
    sep = *ptr++;
  } while (sep == ':' || sep == '_');
  ptr--;
end:
  if (pptr != NULL) *pptr = ptr;
  return 0;
failed:
  if (pptr != NULL) *pptr = str;
  return -1;
}

#ifdef $XCC_h
extern RegisterSet_
Convention_allocatableSet(Convention this, RegFile regFile);
#endif//$XCC_h

RegisterSet_
Convention_allocatableSet(Convention this, RegFile regFile)
{
  const_RegisterSet reservedSet = Convention_roleRegisterSet(this, ConventionRole_Reserved);
  const char *allocatable_str = GETENV("GRA_REGS");
  RegisterSet_ allocatableSet[1];
  RegisterSet_Copy(allocatableSet, RegFile_registerSet(regFile));
  RegisterSet_diff(allocatableSet, reservedSet);
  if (allocatable_str != NULL) {
    RegisterSet_ usableSet[1];
    RegisterSet_Ctor(usableSet);
    if (strToRegisters(allocatable_str, NULL, usableSet) == -1) {
      fprintf(stderr, "Warning: error in alocatable_reg set, ignored\n");
    }
    RegisterSet_inter(allocatableSet, usableSet);
    RegisterSet_Dtor(usableSet);
  }
  return *allocatableSet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Convention module.
 */
void
Convention_INIT(void);
#endif//$XCC__h

void
Convention_INIT(void)
{
  int convention = 0, role = 0;
  for (convention = 0; convention < Convention__; convention++) {
    Convention_StructRegister[convention] = Register_;
    Convention_ProgramRegister[convention] = Register_;
    Convention_ReturnRegister[convention] = Register_;
    Convention_StackRegister[convention] = Register_;
    Convention_FrameRegister[convention] = Register_;
    Convention_StaticRegister[convention] = Register_;
    Convention_GlobalRegister[convention] = Register_;
    Convention_LocalRegister[convention] = Register_;
    for (role = 0; role < ConventionRole__; role++) {
      const_RegisterList registerList = &Convention_RoleRegisterLists[convention][role];
      if (RegisterList_count(registerList) > 0) {
        RegisterSet registerSet = &Convention_RoleRegisterSets[convention][role];
        RegisterList_FOREACH_Register(registerList, registre) {
          RegisterSet_insert(registerSet, registre);
        } RegisterList_ENDEACH_Register;
        if (role == ConventionRole_Struct) {
          Convention_StructRegister[convention] = RegisterList_first(registerList);
        }
        if (role == ConventionRole_Program) {
          Convention_ProgramRegister[convention] = RegisterList_first(registerList);
        }
        if (role == ConventionRole_Return) {
          Convention_ReturnRegister[convention] = RegisterList_first(registerList);
        }
        if (role == ConventionRole_Stack) {
          Convention_StackRegister[convention] = RegisterList_first(registerList);
        }
        if (role == ConventionRole_Frame) {
          Convention_FrameRegister[convention] = RegisterList_first(registerList);
        }
        if (role == ConventionRole_Static) {
          Convention_StaticRegister[convention] = RegisterList_first(registerList);
        }
        if (role == ConventionRole_Global) {
          Convention_GlobalRegister[convention] = RegisterList_first(registerList);
        }
        if (role == ConventionRole_Local) {
          Convention_LocalRegister[convention] = RegisterList_first(registerList);
        }
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Finalize the Convention module.
 */
#define Convention_FINI()
#endif//$XCC__h

#if XCC__C
static void
Convention_TEST(void)
{
#include "MDT/Convention_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Convention_TEST);
  return 0;
}
#endif

