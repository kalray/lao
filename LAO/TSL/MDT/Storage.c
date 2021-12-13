#ifdef $XCC_h
/*
 * !!!!	Storage.xcc
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
#include "MDT/Storage.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define StorageCell(ID, KIND, WIDTH) \
  StorageCell_##ID,
  StorageCell_,
#include /*MDT*/"Storage.tuple"
  StorageCell__
#undef StorageCell
} enum_StorageCell;
#endif//$XCC__h

const int StorageCell__Count = StorageCell__;
const int StorageCell__Volatile = StorageCell_Volatile;
const int StorageCell__Control = StorageCell_Control;
const int StorageCell__Memory = StorageCell_Memory;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the StorageCell enumeration.
 */
extern const char *
StorageCell_Id(StorageCell this);
extern const char *
StorageCell_name(StorageCell this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
StorageCell_Id(StorageCell this)
{
  static const char *_[] = {
#define StorageCell(ID, KIND, WIDTH) \
    #ID,
    "_UNDEF",
#include "Storage.tuple"
  };
  Except_REQUIRE((unsigned)this < StorageCell__);
  return _[this];
}
const char *
StorageCell_name(StorageCell this)
{
  return this? strchr(StorageCell_Id(this) + sizeof(MDT_TARGET) - 1, '_') + 1: "_UNDEF";
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int storageCell = 0;
  for (; storageCell < StorageCell__; storageCell++) {
    printf("StorageCell_%s\t%d\n", StorageCell_Id(storageCell), storageCell);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Storage module.
 */
#define Storage_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Storage module.
 */
#define Storage_FINI()
#endif//$XCC__h

#if XCC__C
static void
Storage_TEST(void)
{
#include "MDT/Storage_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Storage_TEST);
  return 0;
}
#endif

