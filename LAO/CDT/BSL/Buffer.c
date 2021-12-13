#ifdef $XCC_h
/*
 * !!!!	Buffer.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2007 STMicroelectronics.
 * Copyright 1995 - 1998 Commissariat a l'Energie Atomique.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup BSL
 * @brief Character buffer for preparing output.
 */
#endif//$XCC_h


#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/Buffer.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Block storage for the Buffer.
 */
struct BufferBlock_ {
  //@args	size_t blockSize
  size_t AVAIL;			// Available space in this BufferBlock.
  //@access BASE	((char *)((BufferBlock)(this) + 1))
  struct BufferBlock_ *NEXT;	// Next BufferBlock in the Buffer.
};
#endif//$XCC__h

BufferBlock
BufferBlock_Ctor(BufferBlock this, size_t blockSize)
{
  *BufferBlock__AVAIL(this) = blockSize;
  *BufferBlock__NEXT(this) = NULL;
  return this;
}

void
BufferBlock_Dtor(BufferBlock this)
{
  *BufferBlock__NEXT(this) = NULL;
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(BufferBlock_)\t%zu\n", sizeof(BufferBlock_));
  Except_ALWAYS(_ALIGNED(sizeof(BufferBlock_)));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Buffer for character data.
 */
struct Buffer_ {
  //@args	Memory memory, size_t blockSize
  Memory MEMORY;
  size_t BLOCKSIZE;		// BufferBlock size.
  size_t LENGTH;		// Total length in bytes.
  struct BufferBlock_ *FIRST;	// First BufferBlock.
  struct BufferBlock_ *LAST;	// Last BufferBlock.
  char *START;			// Start data pointer in the FIRST BufferBlock.
  char *PAST;			// Past data pointer in the LAST BufferBlock.
  const char *INDENT;		// The indentation string.
  int16_t ILEVEL;		// The indentation level.
  int16_t ISIZE;		// The cached strlen(INDENT).
};
#endif//$XCC_h

Buffer
Buffer_Ctor(Buffer this, Memory memory, size_t blockSize)
{
  *Buffer__MEMORY(this) = memory;
  *Buffer__BLOCKSIZE(this) = blockSize;
  *Buffer__LENGTH(this) = 0;
  *Buffer__FIRST(this) = NULL;
  *Buffer__LAST(this) = NULL;
  *Buffer__START(this) = NULL;
  *Buffer__PAST(this) = NULL;
  *Buffer__INDENT(this) = "\t";
  *Buffer__ILEVEL(this) = 0;
  *Buffer__ISIZE(this) = strlen(Buffer_INDENT(this));
  return this;
}

void
Buffer_Dtor(Buffer this)
{
  Buffer_empty(this);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Buffer_)\t%zu\n", sizeof(Buffer_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Empty this Buffer.
 */
void
Buffer_empty(Buffer this);
#endif//$XCC_h

void
Buffer_empty(Buffer this)
{
  Memory memory = Buffer_MEMORY(this);
  BufferBlock block = Buffer_FIRST(this);
  while (block != NULL) {
    BufferBlock next = BufferBlock_NEXT(block);
    BufferBlock_Dtor(block);
    Memory_free_(memory, block);
    block = next;
  }
  *Buffer__LENGTH(this) = 0;
  *Buffer__FIRST(this) = NULL;
  *Buffer__LAST(this) = NULL;
  *Buffer__START(this) = NULL;
  *Buffer__PAST(this) = NULL;
  *Buffer__ILEVEL(this) = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Buffer memory.
 */
static inline Memory
Buffer_memory(const_Buffer this)
{
  return Buffer_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Buffer length.
 */
static inline int32_t
Buffer_length(const_Buffer this)
{
  return Buffer_LENGTH(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This Buffer indent string.
 */
static inline const char *
Buffer_indent(const_Buffer this)
{
  return Buffer_INDENT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Buffer indent string.
 */
static inline void
Buffer_setIndent(Buffer this, const char *indent)
{
  *Buffer__INDENT(this) = indent;
  *Buffer__ISIZE(this) = strlen(indent);
  *Buffer__ILEVEL(this) = 0;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Increment this Buffer indentation level.
 */
static inline int
Buffer_moreIndent(Buffer this)
{
  return ++*Buffer__ILEVEL(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Decrement this Buffer indentation level.
 */
static inline int
Buffer_lessIndent(Buffer this)
{
  return --*Buffer__ILEVEL(this);
}
#endif//$XCC_h

#ifdef $XCC_h
int32_t
Buffer_append(Buffer this, size_t length, const char *data);
#endif//$XCC_h

int32_t
Buffer_append(Buffer this, size_t length, const char *data)
{
  int remains = length;
  BufferBlock last = Buffer_LAST(this);
  if (last == NULL) {
    Memory memory = Buffer_MEMORY(this);
    size_t blockSize = Buffer_BLOCKSIZE(this);
    void *alloc = Memory_alloc(memory, sizeof(BufferBlock_) + blockSize);
    BufferBlock block = BufferBlock_Ctor(alloc, blockSize);
    char *base = BufferBlock_BASE(block);
    *Buffer__FIRST(this) = block;
    *Buffer__START(this) = base;
    *Buffer__LAST(this) = block;
    *Buffer__PAST(this) = base;
    last = block;
  }
  for (;;) {
    char *past = Buffer_PAST(this);
    size_t avail = BufferBlock_AVAIL(last);
    size_t size = _MIN(avail, remains);
    if (size > 0) {
      if (data != NULL) memcpy(past, data, size);
      else memset(past, 0, size);
      *Buffer__PAST(this) += size;
      *BufferBlock__AVAIL(last) -= size;
      *Buffer__LENGTH(this) += size;
      remains -= size;
      data += size;
    }
    if (remains > 0) {
      Memory memory = Buffer_MEMORY(this);
      size_t blockSize = Buffer_BLOCKSIZE(this);
      void *alloc = Memory_alloc(memory, sizeof(BufferBlock_) + blockSize);
      BufferBlock block = BufferBlock_Ctor(alloc, blockSize);
      char *base = BufferBlock_BASE(block);
      *BufferBlock__NEXT(last) = block;
      *Buffer__LAST(this) = block;
      *Buffer__PAST(this) = base;
      last = block;
    } else break;
  }
  return length;
}

#ifdef $XCC_h
/**
 * Append the current indentation to this Buffer.
 */
int32_t
Buffer_appendIndent(Buffer this);
#endif//$XCC_h

int32_t
Buffer_appendIndent(Buffer this)
{
  const char *indent = Buffer_INDENT(this);
  int iLevel = Buffer_ILEVEL(this);
  int iSize = Buffer_ISIZE(this);
  int length = iLevel*iSize, i;
  char buffer[128], *p = buffer;
  if (length <= sizeof(buffer)) {
    for (i = 0; i < iLevel; i++) {
      strncpy(p, indent, iSize);
      p += iSize;
    }
    Buffer_append(this, length, buffer);
  } else {
    for (i = 0; i < iLevel; i++) {
      Buffer_append(this, iSize, indent);
    }
  }
  return length;
}

#ifdef $XCC_h
/**
 * Append a char to this Buffer.
 */
int32_t
Buffer_appendC(Buffer this, int c);
#endif//$XCC_h

int32_t
Buffer_appendC(Buffer this, int c)
{
  char character = c;
  Buffer_append(this, 1, &character);
  return 1;
}

#ifdef $XCC_h
/**
 * Append a %lld to this Buffer.
 */
int32_t
Buffer_appendD(Buffer this, int64_t lld);
#endif//$XCC_h

int32_t
Buffer_appendD(Buffer this, int64_t lld)
{
  char data[64];
  int lenght = sprintf(data, "%0" PRId64, lld);
  return Buffer_append(this, lenght, data);
}

#ifdef $XCC_h
/**
 * Append a %llu to this Buffer.
 */
int32_t
Buffer_appendU(Buffer this, int64_t llu);
#endif//$XCC_h

int32_t
Buffer_appendU(Buffer this, int64_t llu)
{
  char data[64];
  int lenght = sprintf(data, "%0" PRIu64, llu);
  return Buffer_append(this, lenght, data);
}

#ifdef $XCC_h
/**
 * Append a %llx to this Buffer.
 */
int32_t
Buffer_appendX(Buffer this, int64_t llx);
#endif//$XCC_h

int32_t
Buffer_appendX(Buffer this, int64_t llx)
{
  char data[64];
  int lenght = sprintf(data, "%0" PRIx64, llx);
  return Buffer_append(this, lenght, data);
}

#ifdef $XCC_h
/**
 * Append a literal string to this Buffer.
 */
#define Buffer_appendL(this, literal) \
  Buffer_append(this, sizeof(literal) - 1, literal)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Append a string to this Buffer.
 */
#define Buffer_appendS(this, string) \
  Buffer_append(this, strlen(string), string)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Append XML data to this Buffer. (TODO)
 */
#define Buffer_appendXML(this, string) \
  Buffer_append(this, strlen(string), string)
#endif//$XCC_h

#ifdef $XCC_h
void
Buffer_fwrite(const_Buffer this, FILE *file);
#endif//$XCC_h

void
Buffer_fwrite(const_Buffer this, FILE *file)
{
  size_t blockSize = Buffer_BLOCKSIZE(this);
  BufferBlock first = Buffer_FIRST(this);
  BufferBlock last = Buffer_LAST(this);
  if (first != NULL) {
    if (first == last) {
      char *start = Buffer_START(this);
      char *past = Buffer_PAST(this);
      fwrite(start, 1, past - start, file);
    } else {
      BufferBlock block = BufferBlock_NEXT(first);
      char *start = Buffer_START(this);
      char *base = BufferBlock_BASE(first);
      char *past = base + blockSize;
      fwrite(start, 1, past - start, file);
      while (block != last) {
        char *base = BufferBlock_BASE(block);
        BufferBlock next = BufferBlock_NEXT(block);
        fwrite(base, 1, blockSize, file);
        block = next;
      }
      base = BufferBlock_BASE(last);
      past = Buffer_PAST(this);
      fwrite(base, 1, past - base, file);
    }
  }
}

#ifdef $XCC__c
{
  Buffer_ buffer[1];
  Buffer_Ctor(buffer, Memory_Root, 4);
  Buffer_appendS(buffer, "The");
  Buffer_appendS(buffer, " quick");
  Buffer_appendS(buffer, " brown");
  Buffer_appendS(buffer, " fox");
  Buffer_appendS(buffer, " jumps");
  Buffer_appendS(buffer, " over");
  Buffer_appendS(buffer, " the");
  Buffer_appendS(buffer, " lazy");
  Buffer_appendS(buffer, " dog");
  Buffer_appendS(buffer, ".\n");
  Buffer_appendX(buffer, 0xdeadbeef);
  Buffer_appendC(buffer, '\n');
  Buffer_fwrite(buffer, stdout);
  Buffer_Dtor(buffer);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Buffer module.
 */
#define Buffer_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Buffer module.
 */
#define Buffer_FINI()
#endif//$XCC__h

#if XCC__C
static void
Buffer_TEST(void)
{
#include "BSL/Buffer_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(Buffer_TEST);
  return 0;
}
#endif

