#ifdef $XCC_h
/*
 * kv3-JIT.c
 *
 * Copyright 2009-2011 Kalray
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#include "JIT/JIT_.h"
#include <string.h>

reg_t
setupargs(void *sp, reg_t *regSet, int argc, char **argv, int regCount)
{
  /*  Arguments in stack:
      @ high:  argc
      argv[argc] = NULL
      argv[n]
      .
      .
      .
      argv[0]
      env
   *argv[0]
   .
   .
   .
   @ low:   *argv[n]
   */  
  char**   sp_argv;
  char**   sp_env;
  char*   addr;
  reg_t*   sp_argc = (reg_t*)(sp);
  int         i, len;
  int size = sizeof(argc);
  
// store argc
  *sp_argc = argc;
  
// reserve space for argv, SP points on argv[0]
  sp_argv = (char**)(sp_argc - ((argc+1)*sizeof(void*)));
  size += (argc+1)*sizeof(void*);
  
// reserve space for env
  sp_env = sp_argv - sizeof(void*);
  size += sizeof(void*);
  
// store env
  *sp_env = 0;
  
// copy all strings to correct places and fill the table of pointers  
  addr = (char*)sp_env; 
  for(i=0; i<argc; ++i)
  {
    len = strlen(argv[i])+1;
    size += len;
    addr -= len;
    
    strncpy(addr, argv[i], len);
    sp_argv[i] = addr;
  }
  sp_argv[i] = 0;
  
  if(size%8) size+= 8-size%8;
  
  for(int i = 2; i<regCount; ++i) regSet[i] = 0;
// set R0 and R1 with argc and argv
  regSet[0] = argc;
  regSet[1] = (int)(intptr_t)argv;
  
// return new SP value
  return (reg_t)(intptr_t)(sp_argc-16-size);
}
