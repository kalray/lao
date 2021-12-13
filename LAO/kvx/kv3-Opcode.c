#ifdef $XCC_h
/*
 * k1-Opcode.c
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

#include "MDT/MDT_.h"

int
Opcode_biasRAW(Opcode this_write, Opcode that_read, StorageCell cell, int biasRAW)
{
  return biasRAW;
}

