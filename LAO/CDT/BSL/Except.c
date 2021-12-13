#ifdef $XCC_h
/*
 * !!!!	Except.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2007 STMicroelectronics.
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
 * @ingroup BSL
 * @brief Macros to perform assert-like checks.
 */
#endif//$XCC_h


#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/Except.h"
#endif//$XCC__h

#ifdef $XCC_h
/**
 * Define to disable checks.
 */
#ifdef _OPTIMIZE
# if (_OPTIMIZE >= 1)
#  define Except_CHECK(expression)
#  define Except_INVARIANT(expression)
# endif
# if (_OPTIMIZE >= 2)
#  define Except_ENSURE(expression)
#  define Except_REQUIRE(expression)
# endif
# if (_OPTIMIZE >= 3)
#  define Except_NEVER(expression)	do { if (expression) abort(); } while (0)
#  define Except_ALWAYS(expression)	do { if (expression); else abort(); } while (0)
# endif
#endif//_OPTIMIZE
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Throw an exception with a string.
 */
#ifndef Except_THROW
# define Except_THROW(exception, args...) \
  (exit(fprintf(stderr, exception "\tat " __FILE__ ":%d:\n", ##args, __LINE__)))
#endif//Except_THROW
/**
 * Throw an exception with a string if condition is satisfied
 */
#ifndef Except_THROWIF
# define Except_THROWIF(expression, exception, args...) \
  do { if (expression) Except_THROW(exception, ##args); } while (0)
#endif//Except_THROWIF

/**
 * Output a warning with a string.
 */
#ifndef Except_WARN
# define Except_WARN(exception, args...) \
  (fprintf(stderr, "Warning: " exception "\tat " __FILE__ ":%d:\n", ##args, __LINE__), 0)
#endif//Except_WARN

/**
 * Output a warning with a string if condition is satisfied.
 */
#ifndef Except_WARNIF
# define Except_WARNIF(expression, exception, args...) \
  do { if (expression) Except_WARN(exception, ##args); } while (0)
#endif//Except_WARNIF

/**
 * Inline assertion that expression is never true.
 */
#ifndef Except_NEVER
# define Except_NEVER(expression)	ASSERT(!(expression));
#endif//Except_NEVER
/**
 * Inline assertion that expression is always true.
 */
#ifndef Except_ALWAYS
# define Except_ALWAYS(expression)	ASSERT(expression);
#endif//Except_ALWAYS
/**
 * Check method pre-condition.
 */
#ifndef Except_REQUIRE
# define Except_REQUIRE(expression)	ASSERT(expression)
#endif//Except_REQUIRE
/**
 * Check method post-condition.
 */
#ifndef Except_ENSURE
# define Except_ENSURE(expression)	ASSERT(expression)
#endif//Except_ENSURE
/**
 * Check loop invariant.
 */
#ifndef Except_INVARIANT
# define Except_INVARIANT(expression)	ASSERT(expression)
#endif//Except_INVARIANT
/**
 * Used for any other inline checks.
 */
#ifndef Except_CHECK
# define Except_CHECK(expression)	ASSERT(expression)
#endif//Except_CHECK
/**
 * Used for heavy debug checks.
 */
#ifdef _DEBUG
# define Except_DEBUG(expression)	ASSERT(expression)
#else//_DEBUG
# define Except_DEBUG(expression)
#endif//_DEBUG
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
if (0) {
  printf("Try Except_THROW:\n");
  if(0) Except_THROW("TestThrow0");
  if(1) Except_THROW("TestThrow1");
}
if (0) {
  Except_THROWIF(0, "TestThrowIf 0");
  Except_THROWIF(1, "TestThrowIf 1");
}
if (0) {
  Except_THROWIF(1, "TestThrowIf with string %s and num %d", "mystring", 42);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Except module.
 */
#define Except_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Except module.
 */
#define Except_FINI()
#endif//$XCC__h

#if XCC__C
static void
Except_TEST(void)
{
#include "BSL/Except_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(Except_TEST);
  return 0;
}
#endif

