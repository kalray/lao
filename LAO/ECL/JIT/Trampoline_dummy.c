/*
 * !!!! Trampoline_dummy.xcc
 *
 * Artur Pietrek (artur.pietrek@kalray.eu)
 *
 * Copyright 2011 Kalray
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 * 
 * Just a dummy trampoline if not crosscompiling for host machine
 * 
 */

#include <stdio.h>

int 
trampoline(void *regset, void *stack, void *func);

void 
callback(void);


int
trampoline(void *regset, void *stack, void *func)
{
  return puts("Trampoline not implemented for this processor!\n");  
}

void
callback(void)
{
  puts("Callback not implemented for this processor!\n");
}
