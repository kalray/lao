#ifdef $XCC_h
/*
 * Data
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com,
 *                            Benoit.Dinechin@kalray.eu).
 * Florent Bouchez (florent.bouchez@kalray.eu).
 * Artur Pietrek (artur.pietrek@kalray.eu).
 * Duco van Amstel (duco.van-amstel@kalray.eu)
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
 * @brief Support for data sections.
 */
#endif//$XCC_h

#include "LIR/LIR_.h"

#ifdef $XCC__h
#include "LIR/Data.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the DataInit types.
 */
typedef enum
{
  DataInitType_ascii,
  DataInitType_string,
  DataInitType_byte,
  DataInitType_sbyte,
  DataInitType_ubyte,
  DataInitType_hword,
  DataInitType_shword,
  DataInitType_uhword,
  DataInitType_word,
  DataInitType_sword,
  DataInitType_uword,
  DataInitType_quad,
  DataInitType_squad,
  DataInitType_uquad,
  DataInitType_float,
  DataInitType_double,
  DataInitType_space,
  DataInitType_symbol,
  DataInitType__
} enum_DataInitType;
typedef uint8_t DataInitType;
extern const char *
DataInitType_Id(DataInitType this);
#endif//$XCC_h


#ifndef _NTRACE
const char *
DataInitType_Id(DataInitType this)
{
  static const char *_[] = {
    "ascii",
    "string",
    "byte",
    "sbyte",
    "ubyte",
    "hword",
    "shword",
    "uhword",
    "word",
    "sword",
    "uword",
    "quad",
    "squad",
    "uquad",
    "float",
    "double",
    "space",
    "word" //for symbol use pointer type
  };
  Except_REQUIRE((unsigned)this < DataInitType__);
  return _[this];
}
#endif//_NTRACE


#ifdef $XCC_h
/**
 * Represents an Initializer of the data.
 */
struct DataInitializer_ {
  //@args  Memory memory, DataInitType type, size_t dsize
  Memory MEMORY;                //!< Where to allocate objects
  DataInitType TYPE;		//!< This Data initializer type
  void* SYM;                    //!< Symbol reference
  size_t  SIZE;		        //!< Size in bytes of the initializer
  uint8_t INIT[];		//!< The actual initializer.
};
#endif//$XCC_h

DataInitializer
DataInitializer_Ctor(DataInitializer this, Memory memory, DataInitType type, size_t dsize)
{
  DataInitializer_MEMORY(this) = memory;
  DataInitializer_TYPE(this) = type;
  DataInitializer_SIZE(this) = dsize;
  DataInitializer_SYM(this) = NULL;
  return this;
}

void
DataInitializer_Dtor(DataInitializer this)
{
}

#ifdef $XCC__h
DataInitializer
DataInitializer_new(Memory parent, DataInitType type, void* init, void* symbol, size_t dsize);
#endif//$XCC__h


DataInitializer
DataInitializer_new(Memory parent, DataInitType type, void* init, void* symbol, size_t dsize)
{
  Except_REQUIRE((init!=NULL && dsize>0 && symbol==NULL) || ((init==NULL && dsize==0 && symbol!=NULL)));
  {
    Memory memory = Memory_new(parent, true);
    void *alloc = Memory_alloc(memory, sizeof(DataInitializer_)+dsize);

    DataInitializer dinit = DataInitializer_Ctor(alloc, memory, type, dsize);

    if(init!= NULL)
      memcpy(DataInitializer_INIT(dinit), init, dsize);
    DataInitializer_SYM(dinit) = symbol;
    return dinit;
  }
}


/**
 * Helper function to convert a number to octal
 */
static inline char toOctal(int X) { return (X&7)+'0'; }

#ifdef $XCC_h
/**
 * Print an ascii initializer
 */
void
DataInitializer_printQuotedString(const_DataInitializer this, FILE *file);
#endif//$XCC_h


/**
 * Print an ascii initializer
 */
void
DataInitializer_printQuotedString(const_DataInitializer this, FILE *file)
{
  size_t len = DataInitializer_SIZE(this);
  uint8_t *ptr = (uint8_t*)DataInitializer_INIT(this);

  fputc('\"', file);
  for (unsigned i = 0; i != len; ++i, ++ptr) {
    unsigned char C = *ptr;
    if (C == '"' || C == '\\') {
      fprintf(file, "\\%c", (char)C);
      continue;
    }

    if (isprint((unsigned char)C)) {
      fputc((char)C, file);
      continue;
    }

    switch (C) {
    case '\b': fprintf(file, "\\b"); break;
    case '\f': fprintf(file, "\\f"); break;
    case '\n': fprintf(file, "\\n"); break;
    case '\r': fprintf(file, "\\r"); break;
    case '\t': fprintf(file, "\\t"); break;
    case '\0': fprintf(file, "\\0"); break;
    default:
      fprintf(file, "\\%c%c%c",
              toOctal(C >> 6),
              toOctal(C >> 3),
              toOctal(C >> 0));
      break;
    }
  }
  fputc('\"', file);
}



////////////////////////////////////////////////////////////////////////////////////////////////////



#ifdef $XCC_h
/**
 * Represent a Initializers of data.
 */
struct DataInitializerList_ {
  //@args	Memory memory
  PtrSeq_ INITIALIZER[1];		//!< The initializer
  //@access MEMORY     	PtrSeq_memory(DataInitializerList_INITIALIZER(this))
};
#endif//$XCC__h


DataInitializerList
DataInitializerList_Ctor(DataInitializerList this, Memory memory)
{
  PtrSeq_Ctor(DataInitializerList_INITIALIZER(this), memory, 0);
  return this;
}

void
DataInitializerList_Dtor(DataInitializerList this)
{
  PtrSeq_Dtor(DataInitializerList_INITIALIZER(this));
}

size_t
DataInitializerList_Size(Memory memory)
{
  return sizeof(DataInitializerList_);
}


#ifdef $XCC_h
/**
 * Check if a DataInitializerList contains some Initializers.
 */
bool
DataInitializerList_isEmpty(DataInitializerList this);
#endif//$XCC_h


bool
DataInitializerList_isEmpty(DataInitializerList this)
{
  PtrSeq ptrSeq = DataInitializerList_INITIALIZER(this);
  return PtrSeq_isEmpty(ptrSeq);
}

#ifdef $XCC_h
/**
 * Add a DataInitializer to a DataInitializerList, resizing it if necessary
 */
void
DataInitializerList_push2(DataInitializerList list, DataInitializer init);
#endif//$XCC_h

void
DataInitializerList_push2(DataInitializerList this, DataInitializer init)
{
  PtrSeq ptrSeq = DataInitializerList_INITIALIZER(this);
  PtrSeq_push2(ptrSeq, init);
}

#ifdef $XCC_h
/**
 * Iterate over the Initializer of this DataInitializerList.
 */
#define DataInitializerList_FOREACH_Initializer(this, init) { \
  PtrSeq_FOREACH(DataInitializerList_INITIALIZER(this), DataInitializer, init) {
#define DataInitializerList_ENDEACH_Initializer \
  } PtrSeq_ENDEACH \
}
#endif//$XCC_h



#ifdef $XCC_h
bool
DataInitializerList_pretty(const_DataInitializerList this, FILE *file);
#endif//$XCC_h



#ifndef _NTRACE
bool
DataInitializerList_pretty(const_DataInitializerList this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "    init:\n");
  DataInitializerList_asmPrint(this, file);
  return true;

  DataInitializerList_FOREACH_Initializer(this, init) {
    DataInitType dtype = DataInitializer_TYPE(init);

    /*  DataInitializer_asmPrint(init, file);*/
    /*  return true;*/

    if(file==NULL) file = stderr;
    fprintf(file,"      %s:", DataInitType_Id(dtype));
    switch(dtype)
    {
      case DataInitType_ascii:
      case DataInitType_string:
        DataInitializer_printQuotedString(init, file);
        break;
      case DataInitType_byte:
        fprintf(file, "%#hhx", (uint8_t) *DataInitializer_INIT(init));
        break;
      case DataInitType_sbyte:
        fprintf(file, "%hhd", (int8_t) *DataInitializer_INIT(init));
        break;
      case DataInitType_ubyte:
        fprintf(file, "%hhu", (char) *DataInitializer_INIT(init));
        break;
      case DataInitType_hword:
        fprintf(file, "%#hx", (short) *DataInitializer_INIT(init));
        break;
      case DataInitType_shword:
        fprintf(file, "%hd", (short) *DataInitializer_INIT(init));
        break;
      case DataInitType_uhword:
        fprintf(file, "%hu", (short) *DataInitializer_INIT(init));
        break;
      case DataInitType_word:
      case DataInitType_float:
        fprintf(file, "%#x", (signed) *DataInitializer_INIT(init));
        break;
      case DataInitType_sword:
        fprintf(file, "%d", (signed) *DataInitializer_INIT(init));
        break;
      case DataInitType_uword:
        fprintf(file, "%u", (unsigned) *DataInitializer_INIT(init));
        break;
      case DataInitType_quad:
      case DataInitType_double:
        fprintf(file, "%#llx", (long long) *DataInitializer_INIT(init));
        break;
      case DataInitType_squad:
        fprintf(file, "%lld", (long long) *DataInitializer_INIT(init));
        break;
      case DataInitType_uquad:
        fprintf(file, "%llu", (long long) *DataInitializer_INIT(init));
        break;
      case DataInitType_space:
        break;
      case DataInitType_symbol: {
          Temporary temporary = (Temporary)DataInitializer_SYM(init);
          Temporary_pretty(temporary, file);
        }
        break;
      default:
        Except_THROW("Unknown data initializer type!");
    }
    fputc('\n', file);
    return true;
  } DataInitializerList_ENDEACH_Initializer;
  return true;
}
#endif//_NTRACE


#ifdef $XCC_h
bool
DataInitializerList_asmPrint(const_DataInitializerList this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
DataInitializerList_asmPrint(const_DataInitializerList this, FILE *file)
{
  DataInitType prevType = DataInitType__;
  const char* sep;
  int approxLineWidth = 0;

  if (file == NULL)
    file = stderr;

  DataInitializerList_FOREACH_Initializer(this, init) {
    DataInitType dtype = DataInitializer_TYPE(init);
    if(file==NULL)
      file = stderr;

    if(prevType != dtype) {
      approxLineWidth = 20;

      if(prevType != DataInitType__)
        fputc('\n', file);

      switch (dtype) {
        case DataInitType_ascii:
          fprintf(file, "\t.ascii\t");
          break;
        case DataInitType_string:
          fprintf(file, "\t.string\t");
          break;
        case DataInitType_byte:
        case DataInitType_sbyte:
        case DataInitType_ubyte:
          fprintf(file, "\t.byte\t");
          break;
        case DataInitType_hword:
        case DataInitType_shword:
        case DataInitType_uhword:
          fprintf(file, "\t.hword\t");
          break;
        case DataInitType_word:
        case DataInitType_sword:
        case DataInitType_uword:
        case DataInitType_float:
        case DataInitType_symbol:
          fprintf(file, "\t.word\t");
          break;
        case DataInitType_quad:
        case DataInitType_squad:
        case DataInitType_uquad:
        case DataInitType_double:
          fprintf(file, "\t.quad\t");
          break;
        case DataInitType_space:
          fprintf(file, "\t.space\t");
          break;
        default:
          Except_THROW("Unknown data initializer type!");
      }
      sep = "";
    }

    switch(dtype) {
      case DataInitType_ascii:
      case DataInitType_string:
        DataInitializer_printQuotedString(init, file);
        approxLineWidth = 100; // force line return
        break;
      case DataInitType_byte:
      case DataInitType_ubyte:
      case DataInitType_sbyte:
        fprintf(file, "%s0x%02hhx", sep, *((unsigned char *) DataInitializer_INIT(init)));
        approxLineWidth += 4;
        break;
      case DataInitType_hword:
      case DataInitType_uhword:
      case DataInitType_shword:
        fprintf(file, "%s0x%04hx", sep, *((unsigned short int *) DataInitializer_INIT(init)));
        approxLineWidth += 6;
        break;
      case DataInitType_word:
      case DataInitType_uword:
      case DataInitType_sword:
      case DataInitType_float:
        fprintf(file, "%s0x%08x", sep, *((unsigned int *) DataInitializer_INIT(init)));
        approxLineWidth += 10;
        break;
      case DataInitType_quad:
      case DataInitType_uquad:
      case DataInitType_squad:
      case DataInitType_double:
        fprintf(file, "%s0x%08x%08x", sep, ((unsigned int *) DataInitializer_INIT(init))[1],
            ((unsigned int *) DataInitializer_INIT(init))[0]);
        approxLineWidth += 18;
        break;
      case DataInitType_space:
        approxLineWidth = 100;
        fprintf(file, "%u", (unsigned int) DataInitializer_SIZE(init));
        break;
      case DataInitType_symbol: {
          Temporary temporary = (Temporary)DataInitializer_SYM(init);
          Temporary_pretty(temporary, file);
          approxLineWidth = 100; // new line
          break;
        }
      default:
        Except_THROW("Unknown data initializer type!");
    }
    sep = ", ";
    prevType = dtype;
    if (approxLineWidth >= 80) {
      prevType = DataInitType__; // force line return
      fputc('\n', file);
    }
  } DataInitializerList_ENDEACH_Initializer;
  if(prevType != DataInitType__)
    fputc('\n', file);
  return true;
}
#endif//_NTRACE


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Enumerates the DataSection flags.
 *
 * @see http://sourceware.org/binutils/docs-2.20/as/Section.html for info on 
 * elf section flags.
 */
typedef enum {
  DataSectionFlag_Alloc = 0x1,		//!< DataSection is allocatable
  DataSectionFlag_Exec = 0x2,		//!< DataSection contains executable code
  DataSectionFlag_Write = 0x4,		//!< DataSection is writable
  DataSectionFlag_Small = 0x8,		//!< DataSection is small (not found in elf doc??)
  DataSectionFlag_Merge = 0x10,		//!< DataSection is mergeable
  DataSectionFlag_Strings = 0x20,	//!< DataSection contains zero terminated strings
  DataSectionFlag_TLS = 0x40,		//!< DataSection is used for thread-local-storage
  DataSectionFlag_Group = 0x80,		//!< DataSection is a member of a section group
} enum_DataSectionFlag;
typedef uint8_t DataSectionFlags;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Enumerates the DataSection types.
 *
 * @see http://sourceware.org/binutils/docs-2.20/as/Section.html for info on 
 * elf section type.
 */
typedef enum {
  DataSectionType_UNDEF,	//!< DataSection type is not specified
  DataSectionType_progbits,	//!< DataSection contains data
  DataSectionType_nobits,	//!< DataSection does not contain data (i.e., only space)
  DataSectionType_note,		//!< Data used by things other than the program
  DataSectionType_init_array,	//!< array of pointers to init functions
  DataSectionType_fini_array,	//!< array of pointers to finish functions
  DataSectionType_preinit_array,	//!< array of pointers to pre-init functions
} enum_DataSectionType;
typedef uint8_t DataSectionType;
#endif//$XCC_h



#ifdef $XCC_h
#define MAX_SECTION_NAME_LEN 62
#endif//$XCC_h


#ifdef $XCC_h
/**
 * Represent a Data Section of a program.
 */
struct DataSection_ {
  //@args	const char* name, uint8_t align, DataSectionFlags flags, DataSectionType type, uint8_t entsize
  char NAME[MAX_SECTION_NAME_LEN];			//!< In wich section the data is located
  uint8_t ALIGN;		//!< Alignment of the data:
  				//!  - ELF format: expressed in bytes
  				//!  - a.out format: number of low-order zero bits
  DataSectionFlags FLAGS;	//!< Flags of the section
  //@access isAlloc  ((DataSection_flags(this) & DataSectionFlag_Alloc != 0)
  //@access isExec  ((DataSection_flags(this) & DataSectionFlag_Exec != 0)
  //@access isWrite  ((DataSection_flags(this) & DataSectionFlag_Write != 0)
  //@access isReadOnly  ((DataSection_flags(this) & DataSectionFlag_ReadOnly == 0)
  DataSectionType TYPE;		//!< Type of the section (nobits/progbits)
  uint8_t ENTSIZE;		//!< Size of entities in the section (used for mergeable sections)
};
#endif//$XCC_h

DataSection
DataSection_Ctor(DataSection this, const char* name, uint8_t align, DataSectionFlags flags, DataSectionType type, uint8_t entsize)
{
  Except_ALWAYS(strlen(name) < MAX_SECTION_NAME_LEN);
  strncpy(DataSection_NAME(this), name, MAX_SECTION_NAME_LEN);
  (*DataSection__NAME(this))[MAX_SECTION_NAME_LEN-1] = '\0';
  DataSection_ALIGN(this)   = align;
  DataSection_FLAGS(this)   = flags;
  DataSection_TYPE(this)    = type;
  DataSection_ENTSIZE(this) = entsize;
  return this;
}

void
DataSection_Dtor(DataSection this)
{
}

#ifdef $XCC__h
/**
 * Low-level function to make a new DataSection.
 */
static inline DataSection
DataSection_make(IBList list, const char* name, uint8_t align, DataSectionFlags flags, DataSectionType type, uint8_t entsize)
{
  DataSection section = IBList_push(list);
  DataSection_Ctor(section, name, align, flags, type, entsize);
  return section;
}
#endif//$XCC__h


#ifdef $XCC_h
/**
 * This DataSection name.
 */
const char*
DataSection_name(DataSection this);
#endif//$XCC_h

const char*
DataSection_name(DataSection this)
{
  return DataSection_NAME(this);
}

#ifdef $XCC_h
/**
 * This DataSection align.
 */
uint8_t
DataSection_align(DataSection this);
#endif//$XCC_h

uint8_t
DataSection_align(DataSection this)
{
  return DataSection_ALIGN(this);
}


#ifdef $XCC_h
/**
 * Print this section name with flags in asm.
 */
void
DataSection_asmPrint(DataSection this, FILE *file);
#endif//$XCC_h

void
DataSection_asmPrint(DataSection this, FILE *file)
{
  char flagchars[10], *f = flagchars;
  DataSectionFlags flags = DataSection_FLAGS(this);

  /* Special case for .text section */
  if (!strcmp(DataSection_name(this), ".text")) {
    fprintf(file, "\t.text\n");
    return;
  }

  if (flags & DataSectionFlag_Alloc)
    *f++ = 'a';
  if (flags & DataSectionFlag_Write)
    *f++ = 'w';
  if (flags & DataSectionFlag_Exec)
    *f++ = 'x';
  if (flags & DataSectionFlag_Small)
    *f++ = 's';
  if (flags & DataSectionFlag_Merge)
    *f++ = 'M';
  if (flags & DataSectionFlag_Strings)
    *f++ = 'S';
  if (flags & DataSectionFlag_TLS)
    *f++ = 'T';
  if (flags & DataSectionFlag_Group)
    *f++ = 'G';
  *f = '\0';

  fprintf(file, "\t.section\t%s", DataSection_name(this));
  if (flagchars[0] != '\0') {
    fprintf(file, ",\"%s\"", flagchars);
  } else {
    if (DataSection_TYPE(this) != DataSectionType_UNDEF)
      fprintf(file, ",");
    else
      fprintf(file, "\n");
  }

  switch (DataSection_TYPE(this)) {
    case DataSectionType_UNDEF:
      return;
    case DataSectionType_progbits:
      fprintf(file, ",@progbits");
      break;
    case DataSectionType_nobits:
      fprintf(file, ",@nobits");
      break;
    case DataSectionType_note:
      fprintf(file, ",@note");
      break;
    case DataSectionType_init_array:
      fprintf(file, ",@init_array");
      break;
    case DataSectionType_fini_array:
      fprintf(file, ",@fini_array");
      break;
    case DataSectionType_preinit_array:
      fprintf(file, ",@preinit_array");
      break;
  }
  
  if (DataSection_ENTSIZE(this) != 0) {
    fprintf(file, ",%d", DataSection_ENTSIZE(this));
  }
  /* TODO: add "comdat", linkonce, ... */

  fprintf(file, "\n");
}






////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC_h
/**
 * Represent a Data of a Program or Procedure.
 */
struct Data_ {
  //@args	Memory memory, Symbol symbol, size_t dataSize, uint8_t align, DataSection section
  Memory MEMORY;                //!< Where to allocate objects
  Symbol SYMBOL;		//!< This Data global Symbol.
  size_t DATASIZE;		//!< Size in bytes of the data
  uint8_t ALIGN;		//!< Alignment of the data:
  				//!  - ELF format: expressed in bytes
  				//!  - a.out format: number of low-order zero bits
  int32_t ORIGIN;		//!< Offset from the origin of the section if any. (-1) if not present
  //@access hasOrigin  (Data_ORIGIN(this) != -1)
  DataSection SECTION;		//!< In wich section the data is located
  DataInitializerList_ DATAINIT[1];		//!< The actual data.
};
#endif//$XCC_h

Data
Data_Ctor(Data this, Memory memory, Symbol symbol, size_t dataSize, uint8_t align, DataSection section)
{
  Data_MEMORY(this) = memory;
  Data_SYMBOL(this) = symbol;
  Data_DATASIZE(this) = dataSize;
  Data_ORIGIN(this) = -1;
  Data_SECTION(this) = section;
  Data_ALIGN(this) = align;
  DataInitializerList_Ctor(Data_DATAINIT(this), memory);
  return this;
}

void
Data_Dtor(Data this)
{
  DataInitializerList_Dtor(Data_DATAINIT(this));
}

//size_t
//Data_Size(Memory memory, Symbol symbol, size_t dataSize, uint8_t align, String dstype)
//{
//  return sizeof(Data_) + dataSize - 1;
//}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Data_)\t%zu\n", sizeof(Data_));
}
#endif//$XCC__c

#ifdef $XCC__h
Data
Data_new(Memory parent, Symbol symbol, size_t dataSize, uint8_t align, DataSection section);
#endif//$XCC__h

Data
Data_new(Memory parent, Symbol symbol, size_t dataSize, uint8_t align, DataSection section)
{
  Memory memory = Memory_new(parent, true);
//  size_t size = Data_Size(memory, symbol, dataSize, align, dstype);
  void *alloc = Memory_alloc(memory, sizeof(Data_));
  return Data_Ctor(alloc, memory, symbol, dataSize, align, section);
}

#ifdef $XCC__h
Data
Data_delete(Data this);
#endif//$XCC__h

Data
Data_delete(Data this)
{
  if (this != NULL) {
    Memory memory = Data_MEMORY(this);
#ifndef _NDTOR
    Data_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}


#ifdef $XCC_h
/**
 * This Data hasInitializer?.
 */
static inline bool
Data_hasInitializer(Data this)
{
  return !DataInitializerList_isEmpty(Data_DATAINIT(this));
}
#endif//$XCC_h


#ifdef $XCC_h
/**
 * This Data DataSection.
 */
static inline DataSection
Data_section(const_Data this)
{
  return Data_SECTION(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Data SectionType.
 */
static inline void
Data_setSectionType(Data this, DataSection section)
{
  fprintf(stderr, "Setting section type to %s\n", DataSection_NAME(section));
  *Data__SECTION(this) = section;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get this Data ORIGIN.
 */
static inline int32_t
Data_origin(Data this)
{
  return Data_ORIGIN(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Set this Data ORIGIN.
 */
static inline void
Data_setOrigin(Data this, int32_t orig)
{
  *Data__ORIGIN(this) = orig;
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Add an initializer to the list of this Data
 */
DataInitializer
Data_addInitializer(Data this, DataInitType dtype, void* init, void* symbol, size_t dsize);
#endif//$XCC_h

/**
 * Add an initializer to the list of this Data
 */
DataInitializer
Data_addInitializer(Data this, DataInitType dtype, void* init, void* symbol, size_t dsize)
{
  DataInitializerList init_list = Data_DATAINIT(this);
  DataInitializer dinit = DataInitializer_new(Data_MEMORY(this), dtype, init, symbol, dsize);
  DataInitializerList_push2(init_list, dinit);
  return dinit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC_h
/**
 * Represent a Data Section of a Program or Procedure.
 */
struct DataList_ {
  //@args	Memory memory
  PtrSeq_ DATA[1];			//!< The data of this section
  //@access MEMORY	PtrSeq_memory(DataList_DATA(this))
  size_t DATASIZE;  //!< All data size in bytes
};
#endif//$XCC_h

DataList
DataList_Ctor(DataList this, Memory memory)
{
  PtrSeq_Ctor(DataList_DATA(this), memory, 0);
  DataList_DATASIZE(this) = 0;
  return this;
}

void
DataList_Dtor(DataList this)
{
  PtrSeq_Dtor(DataList_DATA(this));
}

size_t
DataList_Size(Memory memory)
{
  return sizeof(DataList_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(DataList_)\t%zu\n", sizeof(DataList_));
}
#endif//$XCC__c

/*#ifdef $XCC__h*/
/*DataList*/
/*DataList_new(Memory parent);*/
/*#endif//$XCC__h*/

/*DataList*/
/*DataList_new(Memory parent)*/
/*{*/
/*  Memory memory = Memory_new(parent, true);*/
/*  size_t size = DataList_Size(memory);*/
/*  void *alloc = Memory_alloc(memory, size);*/
/*  return DataList_Ctor(alloc, memory);*/
/*}*/

#ifdef $XCC__h
DataList
DataList_delete(DataList this);
#endif//$XCC__h

DataList
DataList_delete(DataList this)
{
  if (this != NULL) {
    Memory memory = DataList_MEMORY(this);
#ifndef _NDTOR
    DataList_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
size_t
DataList_dataSize(const_DataList this);
#endif//$XCC_h

size_t
DataList_dataSize(const_DataList this)
{
  return DataList_DATASIZE(this);
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Data Symbol.
 */
Symbol
Data_symbol(const_Data this);
#endif//$XCC_h

Symbol
Data_symbol(const_Data this)
{
  return Data_SYMBOL(this);
}

#ifdef $XCC_h
/**
 * This Data dataSize (the actual size of the data hold by the structure).
 */
size_t
Data_dataSize(Data this);
#endif//$XCC_h

size_t
Data_dataSize(Data this)
{
  return Data_DATASIZE(this);
}

#ifdef $XCC_h
/**
 * This Data alignment.
 */
uint8_t
Data_align(Data this);
#endif//$XCC_h

uint8_t
Data_align(Data this)
{
  return Data_ALIGN(this);
}

#ifdef $XCC_h
/**
 * Check if a DataList contains some Data.
 */
bool
DataList_isEmpty(DataList this);
#endif//$XCC_h

bool
DataList_isEmpty(DataList this)
{
  PtrSeq ptrSeq = DataList_DATA(this);
  return PtrSeq_isEmpty(ptrSeq);
}

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef $XCC_h
/**
 * Add a Data to a DataList, resizing it if necessary
 */
void
DataList_push2(DataList dataSection, Data data);
#endif//$XCC_h

void
DataList_push2(DataList dataSection, Data data)
{
  PtrSeq ptrSeq = DataList_DATA(dataSection);
  PtrSeq_push2(ptrSeq, data);
  //DataList_DATASIZE(dataSection) += Data_DATASIZE(data);
}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
bool
Data_pretty(Data this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Data_pretty(Data this, FILE *file)
{
  uint8_t *ptr;
  if (file == NULL) file = stderr;
  fprintf(file, "  - %s\n", Symbol_name(Data_symbol(this)));
  fprintf(file, "    %zu bytes\n", Data_dataSize(this));
  fprintf(file, "    %d alignment\n", Data_align(this));
  fprintf(file, "    %s section\n", DataSection_name(Data_section(this)));
  if (Data_hasInitializer(this))
    DataInitializerList_pretty(Data_DATAINIT(this), file);
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Iterate over the Data of this DataList.
 */
#define DataList_FOREACH_Data(this, data) { \
  PtrSeq_FOREACH(DataList_DATA(this), Data, data) {
#define DataList_ENDEACH_Data \
  } PtrSeq_ENDEACH \
}
#endif//$XCC_h

#ifdef $XCC_h
bool
DataList_pretty(const_DataList this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
DataList_pretty(const_DataList this, FILE *file)
{
  if (file == NULL) file = stderr;
  fprintf(file, "Data Section:\n");
  DataList_FOREACH_Data(this, data) {
    Data_pretty(data, file);
  } DataList_ENDEACH_Data;
  fprintf(file, "Data Section end.\n");
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Asm print data.
 */
bool
Data_asmPrint(Data this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
Data_asmPrint(Data this, FILE *file)
{
  uint8_t *ptr;
  Symbol symbol = NULL;
  const char *name = NULL;
  if (file == NULL) return false;

  symbol = Data_symbol(this);
  name = Symbol_name(symbol);
  // Symbol_pretty(symbol, file);

  if (Symbol_store(symbol) != SymbolStore_COMMON) {
    switch (Symbol_export(symbol)) {
      case SymbolExport_WEAK:
        // Global symbol that is only linked by other files when no other symbol with the
        // same name is available.
        fprintf(file, "\t.weak\t%s\n", name);
        break;
      case SymbolExport_LOCAL:
        // fprintf(file, "\t.local\t%s\n", name); /* Is not used */
        break;
      case SymbolExport_LOCAL_INTERNAL:
        // Static that does not have its address passed out of .out/.so
        fprintf(file, "\t.internal\t%s\n", name);
        break;
      case SymbolExport_GLOBAL_PROTECTED:
        // Exported, but only visible within .out/.so
        fprintf(file, "\t.globl\t%s\n", name);
        fprintf(file, "\t.protected\t%s\n", name);
        break;
      case SymbolExport_GLOBAL_HIDDEN:
        // Exported, but only visible within .out/.so, address may however be used outside
        fprintf(file, "\t.globl\t%s\n", name);
        fprintf(file, "\t.hidden\t%s\n", name);
        break;
      case SymbolExport_GLOBAL_INTERNAL:
        // Exported, but only visible within .out/.so, address may however be used outside
        fprintf(file, "\t.globl\t%s\n", name);
        fprintf(file, "\t.internal\t%s\n", name);
        break;
      case SymbolExport_GLOBAL_PREEMPTIBLE:
        fprintf(file, "\t.globl\t%s\n", name);
        break;
      default:
        Except_WARN("Do not handle this kind of SymbolExport yet!");
    }
  }

  switch (Symbol_store(symbol)) {
    case SymbolStore_COMMON:
      fprintf(file, "\t.comm\t%s,%zd,%d\n", name, Data_dataSize(this), Data_align(this));
      return true;
    case SymbolStore_DGLOBAL:
      fprintf(file, "\t.align %d\n", Data_align(this));
      fprintf(file, "%s:\n", name);
      break;
    case SymbolStore_FSTATIC:
      DataSection_asmPrint (Data_section(this), file);
      fprintf(file, "\t.align %d\n", Data_align(this));
      fprintf(file, "%s:\n", name);
      break;
    case SymbolStore_UGLOBAL:
      DataSection_asmPrint (Data_section(this), file);
      fprintf(file, "\t.align %d\n", Data_align(this));
      fprintf(file, "%s:\n", name);
      fprintf(file, "\t.space %zd\n", Data_dataSize(this));
      break;
    default:
      Except_WARN("Do not handle this kind of SymbolStore yet!");
      break;
  }
  if(Data_hasOrigin(this)) {
    fprintf(file, "\t.org %d\n", Data_origin(this));
  }

  if (Data_hasInitializer(this))
    DataInitializerList_asmPrint(Data_DATAINIT(this), file);
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Asm print data list.
 */
bool
DataList_asmPrint(const_DataList this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
DataList_asmPrint(const_DataList this, FILE *file)
{
  DataSection dataSection = NULL;
  if (file == NULL) return false;

  fprintf(file, "\n");
  DataList_FOREACH_Data(this, data) {
    if (Data_section(data) != NULL
        && dataSection != Data_section(data)) {
      dataSection = Data_section(data);
      DataSection_asmPrint(dataSection, file);
      fprintf(file, "\n");
    }
    Data_asmPrint(data, file);
  } DataList_ENDEACH_Data;
  fprintf(file, "\n");
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  Tirex tirex = Tirex_new(Memory_Root, stdin);
  Program program = Tirex_makeProgram(tirex);

  fprintf(stderr, "TESTING DATA... \n");
  Program_asmPrint(program, stdout);

  Program_delete(program);
  Tirex_delete(tirex);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Data module.
 */
void
Data_INIT(void);
#endif//$XCC__h

void
Data_INIT(void)
{
}

#ifdef $XCC__h
/**
 * Finalize the Data module.
 */
#define Data_FINI()
#endif//$XCC__h

#if XCC__C
static void
Data_TEST(void)
{
#include "LIR/Data_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Data_TEST);
  return 0;
}
#endif
