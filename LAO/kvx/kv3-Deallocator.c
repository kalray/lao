#ifdef $XCC_h
/*
 * kv3-Deallocator.c
 *
 * Copyright 2014 Kalray.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#include "MDT/MDT_.h"

bool
Deallocator_registerIsStackPointer(Register reg)
{
  switch (reg) {
    case Register_kv3_R12:
      return true;
    default:
      return false;
  }
}


RegFile
Deallocator_getGeneralPurposeRegFile(void)
{
  return RegFile_kv3_GPR;
}
