#ifdef $XCC_h
/*
 * !!!!	Format.xcc
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
#include "MDT/Format.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Format(ID, OPERANDS) \
  Format_##ID,
  Format_,
#include /*MDT*/"Format.tuple"
  Format__
#undef Format
} enum_Format;
#endif//$XCC__h

const int Format__Count = Format__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Format enumeration.
 */
extern const char *
Format_Id(Format this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Format_Id(Format this)
{
  static const char *_[] = {
#define Format(ID, OPERANDS) \
    #ID,
    "_UNDEF",
#include "Format.tuple"
  };
  Except_REQUIRE((unsigned)this < Format__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int format = 0;
  for (; format < Format__; format++) {
    printf("Format_%s\t%d\n", Format_Id(format), format);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Format module.
 */
#define Format_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Format module.
 */
#define Format_FINI()
#endif//$XCC__h

#if XCC__C
static void
Format_TEST(void)
{
#include "MDT/Format_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Format_TEST);
  return 0;
}
#endif

