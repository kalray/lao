#ifdef $XCC_h
/*
 * !!!!	Minir.c
 *
 * Florent Bouchez (florent.bouchez@kalray.eu)
 *
 * Copyright 2010 - 2013 Kalray
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
 * @brief Support for Minimalistic Intermediate Representation.
 *
 * Minir is a Minimalistic Intermediate Representation written in Yaml format.
 *
 * The Minir module parses a program in Minir format to produce the
 * corresponding internal representation of the CFG in LAO.
 *
 * We allow some latitude compared to original Minir:
 * - @c fallthru key can have special value @c .next to designate the following
 *   basic block in the @c bbs sequence;
 * - basic block labels should be consistent in one program, i.e., start with
 *   the same prefix followed by some digits (e.g., @c Block_1, @c L3, or @c
 *   BB12). The prefix can be different in two programs even if they belong to
 *   the same Yaml file (seperated by `---')
 *
 */
#endif//$XCC_h

#include "LIR/LIR_.h"

#ifdef $XCC__h
#include <errno.h>
#include "LIR/Minir.h"
#endif//$XCC__h

#ifdef $XCC_h
#ifdef YAML_ENABLED
#include "yaml.h"
#else
typedef int yaml_document_t;
typedef int yaml_node_t;
typedef int yaml_parser_t;
#endif//YAML_ENABLED
#endif//$XCC_h

/*#ifdef $XCC_h*/
/*|+**/
/* * Verbose parsing of Minir*/
/* +|*/
/*extern bool MINIR_DEBUG;*/
/*#endif//$XCC_h*/


/*#define MINIR_DEBUG*/


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Structure to hold temporary informations when building Intermetiate 
 * Representation from Minir representation.
 *
 * Array for BasicBlock informations are indexed by "Minir Id", i.e., the XX
 * in Minir label "Block_XX" for a BasicBlock.
 */
struct Minir_ {
  //@args	Memory memory, FILE *file
  Memory MEMORY;                //!< Where to allocate objects
  //Yaml stuff
  yaml_parser_t PARSER;         //!< The Yaml parser to retrieve Minir programs
  //BasicBlock stuff
  HTable_ BLOCKS[1];		//!< Retrieve BasicBlock by Minir order (in sequence `bbs')
  HTable_ BNAMES[1];		//!< Minir Name (string) of BasicBlock
  HTable_ BLABELS[1];		//!< LAO Label created for BasicBlock
  //Temporary stuff
  int MIN_TEMP_INDEX;           //!< Minimum index allowed when reading
  HTable_ TEMPS[1];		//!< Remember Temporaries in Minir
  IArray_ DEPNODES[1];		//!< Remember Operations that are a memory dependence node
  IArray_ LOOPS[1];		//!< Remember LoopScopes
  PtrSeq_ TEMPRESULTS[1];		//!< Hold results during operation creation
  PtrSeq_ TEMPARGUMENTS[1];	//!< Hold arguments during operation creation
  bool KEEP_INDEX;              //!< Whether to keep the same index for Temporaries. @warning: only keeps the last two digits (e.g. the 23 of "123") so 145 and 245 will be considered the same!!!
  //Other stuff
  Program PROGRAM;		//!< Current program being parsed.
  int PROGRAMCOUNTER;		//!< Number of program parsed.
  unsigned OPERATIONCOUNTER;	//!< Number of operation parsed in current program.
  Procedure PROCEDURE;		//!< Current procedure being parsed.
  CodeRegion CODEREGION;	//!< Current CodeRegion being parsed.
  bool ISTEST;			//!< Is this a real program or a test?
  bool ISSSA;			//!< Is this program under SSA?
  bool EXPLICITFLOW;		//!< Is the control-flow explicited with "preds" and/or "succs"?
};
#endif//$XCC_h

Minir
Minir_Ctor(Minir this, Memory memory, FILE *file)
{
  *Minir__MEMORY(this) = memory;
  HTable_Ctor(Minir_BLOCKS(this), memory, 512);
  HTable_Ctor(Minir_BNAMES(this), memory, 512);
  HTable_Ctor(Minir_BLABELS(this), memory, 512);
  HTable_Ctor(Minir_TEMPS(this), memory, 512);
  IArray_Ctor(Minir_DEPNODES(this), memory, sizeof(Operation), 0);
  IArray_Ctor(Minir_LOOPS(this), memory, sizeof(LoopScope), 0);
  Minir_MIN_TEMP_INDEX(this) = 0;
  Minir_KEEP_INDEX(this) = false; // Disable by default because requires more indexes.
  PtrSeq_Ctor(Minir_TEMPRESULTS(this), memory, 2);
  PtrSeq_Ctor(Minir_TEMPARGUMENTS(this), memory, 4);
#ifdef YAML_ENABLED
  int i = yaml_parser_initialize(&Minir_PARSER(this));
  Except_THROWIF(i == 0, "Failed to initialize Yaml parser!");
  yaml_parser_set_input_file(&Minir_PARSER(this), file);
#endif//YAML_ENABLED
  Minir_PROGRAM(this) = NULL;
  Minir_PROGRAMCOUNTER(this) = 0;
  Minir_OPERATIONCOUNTER(this) = 0;
  Minir_PROCEDURE(this) = NULL;
  Minir_CODEREGION(this) = NULL;
  Minir_ISTEST(this) = false;
  Minir_ISSSA(this) = false;
  Minir_EXPLICITFLOW(this) = false;
  return this;
}

void
Minir_Dtor(Minir this)
{
#ifdef YAML_ENABLED
  yaml_parser_delete(&Minir_PARSER(this));
#endif//YAML_ENABLED
  HTable_Dtor(Minir_BLOCKS(this));
  HTable_Dtor(Minir_BNAMES(this));
  HTable_Dtor(Minir_BLABELS(this));
  HTable_Dtor(Minir_TEMPS(this));
  IArray_Dtor(Minir_DEPNODES(this));
  IArray_Dtor(Minir_LOOPS(this));
  PtrSeq_Dtor(Minir_TEMPRESULTS(this));
  PtrSeq_Dtor(Minir_TEMPARGUMENTS(this));
}

size_t
Minir_Size(Memory memory, FILE *file)
{
  return sizeof(Minir_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Minir_)\t%zu\n", sizeof(Minir_));
  fprintf(stderr, "sizeof(int)\t%zu\n", sizeof(int));
  fprintf(stderr, "sizeof(long int)\t%zu\n", sizeof(long int));
  fprintf(stderr, "sizeof(void*)\t%zu\n", sizeof(void*));
}
#endif//$XCC__c


#ifdef $XCC_h
Minir
Minir_new(Memory parent, FILE *file);
#endif//$XCC_h

Minir
Minir_new(Memory parent, FILE *file)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Minir_Size(memory, file);
  Minir this = Memory_alloc(memory, size);
  return Minir_Ctor(this, memory, file);
}

#ifdef $XCC_h
Minir
Minir_delete(Minir this);
#endif//$XCC_h

Minir
Minir_delete(Minir this)
{
  if (this != NULL) {
    Memory memory = Minir_MEMORY(this);
#ifndef _NDTOR
    Minir_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/**
 * Reset minir object after function read.
 *
 * Use between parsing of different functions.
 */
Minir
Minir_resetFunction(Minir this);
#endif//$XCC_h

Minir
Minir_resetFunction(Minir this)
{
  HTable_empty(Minir_BLOCKS(this), NULL);
  HTable_empty(Minir_BNAMES(this), NULL);
  HTable_empty(Minir_BLABELS(this), NULL);
  HTable_empty(Minir_TEMPS(this), NULL);
  IArray_empty(Minir_DEPNODES(this), NULL);
  Minir_initTemporaries(this);
  return this;
}


#ifdef $XCC_h
/**
 * Reset minir object after program read.
 *
 * Use between parsing of different programs.
 *
 * @note This does not reset the SSA flag so that Minir_isSSA() works even
 * after a call to this function. Also does not reset the Minir_PROGRAM so that
 * calls to Minir_program() gives the last MinIR program parsed.
 */
Minir
Minir_resetProgram(Minir this);
#endif//$XCC_h

Minir
Minir_resetProgram(Minir this)
{
  Minir_resetFunction(this);
  HTable_empty(Minir_TEMPS(this), NULL);
  PtrSeq_empty(Minir_TEMPRESULTS(this));
  PtrSeq_empty(Minir_TEMPARGUMENTS(this));
  //Minir_PROGRAM(this) = NULL; // do not reset!
  Minir_PROCEDURE(this) = NULL;
  Minir_CODEREGION(this) = NULL;
  return this;
}


#ifdef $XCC_h
/**
 * Check if the current program in Minir is declared as being in SSA form.
 *
 * This also works after last program has been parsed, i.e., after a call to
 * Minir_nextProgram(), but should be before any subsequent Yaml program parsing.
 */
bool
Minir_isSSA(Minir this);
#endif//$XCC_h

bool
Minir_isSSA(Minir this)
{
  return Minir_ISSSA(this);
}

#ifdef $XCC_h
/**
 * This Minir current Program.
 */
Program
Minir_program(Minir this);
#endif//$XCC_h

Program
Minir_program(Minir this)
{
  return Minir_PROGRAM(this);
}

#ifdef $XCC_h
/**
 * This Minir current Procedure.
 */
Procedure
Minir_procedure(Minir this);
#endif//$XCC_h

Procedure
Minir_procedure(Minir this)
{
  return Minir_PROCEDURE(this);
}


////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************
 *  Pretty-print functions for debugging when parsing Minir
 ************************************************************/
#ifdef YAML_ENABLED

#ifdef $XCC__h
/**
 * pretty print Yaml node.
 */
void
node_pretty(yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

/** Pretty print node. */
void
node_pretty(yaml_document_t *document, yaml_node_t *this)
{
  yaml_node_pair_t *pair;
  yaml_node_item_t *item;
  yaml_node_t *node;
  int count;
  switch (this->type) {
    case YAML_NO_NODE:
      printf("An empty node.\n");
      break;
    case YAML_SCALAR_NODE:
#ifdef     MINIR_DEBUG 
      printf("Scalar node of length %zu with value: '%s'\n",
          this->data.scalar.length, this->data.scalar.value);
#endif
      break;
    case YAML_SEQUENCE_NODE:
      count = 0;
      for(  item = this->data.sequence.items.start ; item < this->data.sequence.items.top ; item++ ) {
        count++;
      }
#ifdef     MINIR_DEBUG 
      printf("Sequence node of style %d and length %d\n", this->data.sequence.style, count);
#endif
      break;
    case YAML_MAPPING_NODE:
      count = 0;
      for(pair = this->data.mapping.pairs.start ;
          pair < this->data.mapping.pairs.top ; pair++ ) {
        count++;
      }
#ifdef     MINIR_DEBUG 
      printf("A mapping node of style %d and %d pairs\n", this->data.mapping.style, count);
#endif
      break;
    default:
      fprintf(stderr, "NO SUCH NODE TYPE KNOWN!\n");
  }
}

#ifdef $XCC__h
/**
 * pretty print yaml node recursively.
 */
void 
node_pretty_full(yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

void 
node_pretty_full(yaml_document_t *document, yaml_node_t *this)
{
  yaml_node_pair_t *pair;
  yaml_node_item_t *item;
  yaml_node_t *node;
  int first, last;
  switch (this->type) {
    case YAML_NO_NODE:
      printf("An empty node.\n");
      break;
    case YAML_SCALAR_NODE:
      printf("A scalar node of length %zu with value: '%s'\n",
          this->data.scalar.length, this->data.scalar.value);
      break;
    case YAML_SEQUENCE_NODE:
      printf("A sequence node of style %d.\n", this->data.sequence.style);
      for(  item = this->data.sequence.items.start ; item < this->data.sequence.items.top ; item++ ) {
        fprintf(stderr, "Automatic lookup of node %p -> %d!\n", item, *item);
        node = yaml_document_get_node(document, *item);
        if(!node) {
          fprintf(stderr, "Cannot find node %p in document!", item);
        } else {
          node_pretty(document, node);
        }
      }
      break;
    case YAML_MAPPING_NODE:
      printf("A mapping node of style %d.\n", this->data.mapping.style);
      for(pair = this->data.mapping.pairs.start ;
          pair < this->data.mapping.pairs.top ; pair++ ) {
        fprintf(stderr, "Automatic lookup of node %p -> (%d,%d)!", pair, pair->key, (int)pair->value);
        node = yaml_document_get_node(document, pair->key);
        node_pretty(document, node);
        node = yaml_document_get_node(document, pair->value);
        node_pretty(document, node);
      }
      break;
    default:
      fprintf(stderr, "NO SUCH NODE TYPE KNOWN!\n");
  }
}

#ifdef $XCC__h
/**
 * print yaml document.
 */
void
document_pretty(yaml_document_t *this);
#endif//$XCC__h

void 
document_pretty(yaml_document_t *this)
{
  yaml_node_t *node;
  printf("************************\n");
  if(0) {
    printf("Printing all document nodes in order:\n");
    for(node = this->nodes.start;
        node < this->nodes.top;
        node++) {
      fprintf(stderr, "pretty next node at %p:\n", node);
      node_pretty(this, node);
    }
    printf("Bonus: end node at %p:\n", this->nodes.end);
    node_pretty(this, this->nodes.end);
  } else {
    printf("Printing all document nodes in tree fashion:\n");
    if((node = yaml_document_get_root_node(this))) {
      node_pretty(this, node);
    }
  }
  printf("************************\n");
}

/*************************************
 *  Helper functions
 *************************************/

/* Error function */

#define Minir_THROW(node, exception, args...) \
  if(node->start_mark.line == node->end_mark.line) { \
    Except_THROW("Minir Error line %u, char %u-%u: " exception, \
        (unsigned)node->start_mark.line+1, \
        (unsigned)node->start_mark.column+1, \
        (unsigned)node->end_mark.column+1, \
        ##args \
        ); \
  } else { \
    Except_THROW("Minir Error lines %u-%u: " exception, \
        (unsigned)node->start_mark.line+1, \
        (unsigned)node->end_mark.line+1, \
        ##args \
        ); \
  }

#define Minir_THROWIF(node, expression, exception, args...) \
  if(node->start_mark.line == node->end_mark.line) { \
    Except_THROWIF(expression, "Minir Error line %u, char %u-%u: " exception, \
        (unsigned)node->start_mark.line+1, \
        (unsigned)node->start_mark.column+1, \
        (unsigned)node->end_mark.column+1, \
        ##args \
        ); \
  } else { \
    Except_THROWIF(expression, "Minir Error lines %u-%u: " exception, \
        (unsigned)node->start_mark.line+1, \
        (unsigned)node->end_mark.line+1, \
        ##args \
        ); \
  }

/*        "\tat " __FILE__ ":%d:\n", \*/
#define Minir_WARNING(node, warning, args...) \
  if(node->start_mark.line == node->end_mark.line) { \
    fprintf(stderr, "Minir Warning line %u, char %u-%u: " warning "   at %s:%d\n", \
        (unsigned)node->start_mark.line+1, \
        (unsigned)node->start_mark.column+1, \
        (unsigned)node->end_mark.column+1, \
        ##args, \
        __FILE__, __LINE__ \
        ); \
  } else { \
    fprintf(stderr, "Minir Warning lines %u-%u: " warning "   at %s:%d\n", \
        (unsigned)node->start_mark.line+1, \
        (unsigned)node->end_mark.line+1, \
        ##args, \
        __FILE__, __LINE__ \
        ); \
  }


/** Iterators over Yaml structures (sequences or mappings) */

#define Yaml_IS_SEQUENCE(node) \
  node->type == YAML_SEQUENCE_NODE

#define Yaml_NUMSEQ(document, seqnode) \
  ({ Minir_THROWIF(seqnode, seqnode->type != YAML_SEQUENCE_NODE, "Not a Yaml sequence!"); \
     seqnode->data.sequence.items.top - seqnode->data.sequence.items.start; })

#define Yaml_FORSEQ(document, seqnode, node) { \
  yaml_node_item_t    *item; \
  yaml_node_t         *node; \
  Except_CHECK(seqnode); \
  Minir_THROWIF(seqnode, seqnode->type != YAML_SEQUENCE_NODE, "Not a Yaml sequence!"); \
  for(item = seqnode->data.sequence.items.start ; \
      item < seqnode->data.sequence.items.top ; \
      item++ ) { \
    node = yaml_document_get_node(document, *item) ;
#define Yaml_ENDSEQ \
  } \
}

#define Yaml_SCANSEQ_INIT(document, seqnode, node) { \
  yaml_node_item_t    *item; \
  yaml_node_t         *node; \
  Except_CHECK(seqnode); \
  Minir_THROWIF(seqnode, seqnode->type != YAML_SEQUENCE_NODE, "Not a Yaml sequence!"); \
  item = seqnode->data.sequence.items.start-1 ; \
  Yaml_SCANSEQ_NEXT(document, seqnode, node);

#define Yaml_SCANSEQ_NEXT(document, seqnode, node) \
  item++ ; \
  node = (item < seqnode->data.sequence.items.top) ? \
    yaml_document_get_node(document, *item) : \
    NULL \

#define Yaml_SCANSEQ_END \
  }


#define Yaml_FORSTRINGSEQ(document, seqnode, charnode) { \
  yaml_node_item_t    *Yaml_FORSEQ_item; \
  yaml_node_t         *Yaml_FORSEQ_node; \
  char                *charnode; \
  Except_CHECK(seqnode); \
  Minir_THROWIF(seqnode, seqnode->type != YAML_SEQUENCE_NODE, "Not a Yaml sequence!"); \
  for(Yaml_FORSEQ_item = seqnode->data.sequence.items.start ; \
      Yaml_FORSEQ_item < seqnode->data.sequence.items.top ; \
      Yaml_FORSEQ_item++ ) { \
    Yaml_FORSEQ_node = yaml_document_get_node(document, *Yaml_FORSEQ_item) ; \
    charnode = (char*)Yaml_FORSEQ_node->data.scalar.value ;
#define Yaml_ENDSTRINGSEQ \
  } \
}

#define Yaml_IS_MAPPING(node) \
  node->type == YAML_MAPPING_NODE

#define Yaml_FORMAP(document, mapnode, mapkey, mapvalue) { \
  yaml_node_pair_t    *pair; \
  yaml_node_t         *mapkey; \
  yaml_node_t         *mapvalue; \
  Except_CHECK(mapnode); \
  Minir_THROWIF(mapnode, mapnode->type != YAML_MAPPING_NODE, "Not a Yaml mapping!"); \
  for(pair = mapnode->data.mapping.pairs.start ;\
      pair < mapnode->data.mapping.pairs.top ;\
      pair++ ) {\
    mapkey = yaml_document_get_node(document, pair->key) ; \
    mapvalue = yaml_document_get_node(document, pair->value) ;
#define Yaml_ENDMAP \
  } \
}

#define Yaml_FORSTRINGMAP(document, mapnode, charkey, mapvalue) { \
  yaml_node_pair_t    *pair; \
  char                *charkey; \
  yaml_node_t         *mapvalue; \
  Except_CHECK(mapnode); \
  Minir_THROWIF(mapnode, mapnode->type != YAML_MAPPING_NODE, "Not a Yaml mapping!"); \
  for(pair = mapnode->data.mapping.pairs.start ;\
      pair < mapnode->data.mapping.pairs.top ;\
      pair++ ) {\
    mapvalue = yaml_document_get_node(document, pair->key) ; \
    Minir_THROWIF(mapvalue, mapvalue->type != YAML_SCALAR_NODE, "Not a Yaml scalar!"); \
    charkey = (char*)mapvalue->data.scalar.value ;\
    mapvalue = yaml_document_get_node(document, pair->value) ;
#define Yaml_ENDSTRINGMAP \
  } \
}

#define Yaml_FIRSTMAPKEY(document, mapnode) \
  ({yaml_node_pair_t   *pair; \
   yaml_node_t         *key_node; \
   Except_CHECK(mapnode); \
   Minir_THROWIF(mapnode, mapnode->type != YAML_MAPPING_NODE, "Not a Yaml mapping!"); \
   pair = mapnode->data.mapping.pairs.start ;\
   key_node = yaml_document_get_node(document, pair->key) ; \
   Minir_THROWIF(key_node, key_node->type != YAML_SCALAR_NODE, "Not a Yaml scalar!"); \
   (char*)key_node->data.scalar.value ; })


/** Return the string associated to a node. */
#define Yaml_GET_SCALAR_STRING(document, node) \
  ({ Minir_THROWIF(node, node->type != YAML_SCALAR_NODE, "Not a Yaml scalar!"); \
   (char*)node->data.scalar.value; })

/** Return the length of the string associated to a node. */
#define Yaml_GET_SCALAR_STRING_LENGTH(document, node) \
  ({ Minir_THROWIF(node, node->type != YAML_SCALAR_NODE, "Not a Yaml scalar!"); \
   node->data.scalar.length; })


/** Return the integer associated to a node. */

#define Yaml_GET_INTEGER(document, node) \
  ({ int64_t i ; char *c ; \
   Minir_THROWIF(node, node->type != YAML_SCALAR_NODE, "Not a Yaml scalar!"); \
   c = (char*)node->data.scalar.value; \
   i = int_of_string(c); \
   Minir_THROWIF(node, errno != 0, "Failure: get Yaml integer on `%s'!", c); \
   i ; })

/** Return the float associated to a node. */

#define Yaml_GET_FLOAT(document, node) \
  ({ float f ; \
   Minir_THROWIF(node, node->type != YAML_SCALAR_NODE, "Not a Yaml scalar!"); \
   f = float_of_string((char*)node->data.scalar.value); \
   Minir_THROWIF(node, errno != 0, "Failure: get Yaml float!"); \
   f ; })

/** Return the double associated to a node. */

#define Yaml_GET_DOUBLE(document, node) \
  ({ double f ; \
   Minir_THROWIF(node, node->type != YAML_SCALAR_NODE, "Not a Yaml scalar!"); \
   f = double_of_string((char*)node->data.scalar.value); \
   Minir_THROWIF(node, errno != 0, "Failure: get Yaml double!"); \
   f ; })


/** Return the bool associated to a node. */

#define Yaml_GET_BOOL(document, node) \
  ({ bool b ; \
   char *val; \
   Minir_THROWIF(node, node->type != YAML_SCALAR_NODE, "Not a Yaml scalar!"); \
   val = (char*)node->data.scalar.value; \
   if(!strcmp(val, "true")) { \
     b = true; \
   } else if(!strcmp(val, "false")) {\
     b = false; \
   } else { \
     Except_THROW("Failure: get Yaml bool, `%s' found (allowed are `true' and `false')!", val); \
   } ; b ; })


/** Return the number of element in a Yaml sequence. */
 
#define Yaml_SEQ_COUNT(document, node) \
  ({ Minir_THROWIF(node, node->type != YAML_SEQUENCE_NODE, "Not a Yaml sequence!"); \
   node->data.sequence.items.top - node->data.sequence.items.start; })

#ifdef $XCC__h
/**
 * lookup a key in a yaml mapping and return associated .
 * value node
 */
yaml_node_t* 
Yaml_mapping_lookup_key(yaml_document_t *document, yaml_node_t *node, const char *map_key);
#endif//$XCC__h

yaml_node_t* 
Yaml_mapping_lookup_key(yaml_document_t *document, yaml_node_t *node, const char *map_key)
{
  Minir_THROWIF(node, node->type != YAML_MAPPING_NODE, "Yaml node is not a mapping, cannot look for `%s' in it!", map_key);
  Yaml_FORSTRINGMAP(document, node, key, value) {
    /* printf("key in mapping is '%s'\n", key);*/
    if (!strcmp(key, map_key)){
      return value;
    }
  } Yaml_ENDSTRINGMAP;
  return NULL;
}

#ifdef $XCC__h
/**
 * Lookup a sequence whose first key matches a string.
 * @return the node of the sequence that matches if any, NULL otherwise.
 */
yaml_node_t* 
Yaml_sequence_lookup_first_key(yaml_document_t *document, yaml_node_t *node, const char *first_key);
#endif//$XCC__h

yaml_node_t* 
Yaml_sequence_lookup_first_key(yaml_document_t *document, yaml_node_t *node, const char *first_key)
{
  char* key;
  Minir_THROWIF(node, node->type != YAML_SEQUENCE_NODE, "Yaml node is not a sequence!");
  Yaml_FORSEQ(document, node, seqnode) {
    key = Yaml_FIRSTMAPKEY(document, seqnode);
    if(!strcmp(key, first_key))
      return seqnode;
  } Yaml_ENDSEQ;
  return NULL;
}


#ifdef $XCC__h
/**
 * Return the float described by a string.
 *
 * Sets @c errno to 1 if the string is not a float.
 */
float
float_of_string(const char* string);
#endif//$XCC__h

float
float_of_string(const char* string)
{
  char* p = (char*)string;
  float val;
  errno = 0;
  val = strtof(string, &p);
  if (errno != 0
      || string == p
      || *p != 0) {
    errno = 1;
  }
  return val;
}

#ifdef $XCC__h
/**
 * Return the double described by a string.
 *
 * Sets @c errno to 1 if the string is not a double.
 */
double
double_of_string(const char* string);
#endif//$XCC__h

double
double_of_string(const char* string)
{
  char* p = (char*)string;
  double val;
  errno = 0;
  val = strtod(string, &p);
  if (errno != 0
      || string == p
      || *p != 0) {
    errno = 1;
  }
  return val;
}


#ifdef $XCC__h
/**
 * Return the integer described by a string.
 *
 * Sets @c errno to 1 if the string is not an integer. Integer must be in
 * base 10.
 */
int64_t
int_of_string(const char* string);
#endif//$XCC__h

int64_t
int_of_string(const char* string)
{
  char* p = (char*)string;
  int64_t val;
  errno = 0;
  val = strtoull(string, &p, 0); // 0 base means 0x -> hex, 0 -> octal, else dec.
  if (errno != 0
      || string == p
      || *p != '\0') {
    errno = 1;
  }
  return val;
}

#ifdef $XCC__h
/**
 * Return the first integer described in a string.
 *
 * Sets @c errno to 1 if the string is not an integer. Integer must be in
 * base 10.
 */
int64_t
int_in_string(const char* string);
#endif//$XCC__h

int64_t
int_in_string(const char* string)
{
  int64_t val;
  char* str = (char*)string;
  char* endptr;
  errno = 0;
  while(*str != 0) {
    if(*str < '0' || *str > '9') { str++; continue; }
    val = strtoll(str, &endptr, 10); // 10 is the base
    if (errno != 0
        || str == endptr
        || *endptr != '\0') {
      errno = 1;
    }
    return val;
  }
  errno = 1;
  return 0;
}




#ifdef $XCC__h
/**
 * Check if a string corresponds to a BasicBlock Label.
 *
 * @return the label associated with the name, or @c NULL.
 */
Label
Minir_isBasicBlockName(Minir this, const char *name);
#endif//$XCC__h

Label
Minir_isBasicBlockName(Minir this, const char *name)
{
  HTable_FOREACH(Minir_BNAMES(this), key, char*, iter) {
    if(!strcmp(*iter, name)) {
      return Minir_findBasicBlockLabel(this, (long int)key);
    }
  } HTable_ENDEACH;
  return NULL;
}

#ifdef $XCC__h
/**
 * Check if a string corresponds to a Procedure Symbol.
 *
 * @return the symbol associated with the name, or @c NULL.
 */
Symbol
Minir_isProcedureName(Minir this, const char *name);
#endif//$XCC__h

Symbol
Minir_isProcedureName(Minir this, const char *name)
{
  Program program = Minir_PROGRAM(this);
  SymbolTable symbolTable = Program_symbolTable(program);
  SymbolTable_FOREACH_Symbol(symbolTable, symbol) {
    if(!strcmp(Symbol_name(symbol), name)) {
      return symbol;
    }
  } SymbolTable_ENDEACH_Symbol;
  return NULL;
}
 

#ifdef $XCC_h
/**
 * Get BasicBlock number by its Minir Label.
 *
 * @param this the yaml node to point to in case of an error
 * @param name the name of the block to look for
 */
long int
Minir_bbIndexOfName(Minir minir, yaml_node_t *this, const char *name);
#endif//$XCC_h

long int
Minir_bbIndexOfName(Minir minir, yaml_node_t *this, const char *name)
{
  static char prefix[20];
  static size_t plength = 0;
  static int programCounter = 0;
  Program program = NULL;
  if(programCounter != Minir_PROGRAMCOUNTER(minir)) {
    // New program, determining basic block prefix
    // look for first occurence of a digit
    char *ptr = strpbrk(name, "0123456789");
    programCounter = Minir_PROGRAMCOUNTER(minir);
    Except_THROWIF(!ptr, "Cannot find a digit in basic block name `%s'!", name);
    plength = ptr - name;
    Except_THROWIF(!plength, "Cannot find prefix before digit in basic block name `%s'!", name);
    strncpy(prefix, name, plength);
    prefix[plength] = '\0';
#ifdef   MINIR_DEBUG 
    printf("%s:%d\tBasic block prefix of length %zu found: `%s' (in %s)\n", __FUNCTION__, __LINE__, plength, prefix, name);
#endif
    Except_THROWIF(!strcmp(prefix, "Block_"),
        "Cannot use `Block_' as prefix for basic block label! (would be confusing with LAO internal representation)"
        );
  }
  if(!strncmp(name, prefix, plength)) {
    int i = int_of_string(name+plength);
    if(errno == 0 && i >= 0)
      return (long int)i;
  }
  Minir_THROW(this, "Cannot find basic block number in `%s' (prefix checked: `%s' of length %zu)!",
      name, prefix, plength);
}

#ifdef $XCC_h
/**
 * Find a BasicBlock by its Minir index.
 */
BasicBlock
Minir_findBasicBlock(Minir this, long int bbIndex);
#endif//$XCC_h

BasicBlock
Minir_findBasicBlock(Minir this, long int bbIndex)
{
  long int key = bbIndex;
  void *pointer = HTable_search(Minir_BLOCKS(this), (HTableKey)key);
  if (pointer != NULL) return *(BasicBlock *)pointer;
  return NULL;
}

#ifdef $XCC_h
/**
 * Find a BasicBlock Name by its Minir index.
 */
char*
Minir_findBasicBlockName(Minir this, long int bbIndex);
#endif//$XCC_h

char*
Minir_findBasicBlockName(Minir this, long int bbIndex)
{
  long int key = bbIndex;
  void *pointer = HTable_search(Minir_BNAMES(this), (HTableKey)key);
  if (pointer != NULL) return *(char **)pointer;
  return NULL;
}


#ifdef $XCC_h
/**
 * Find a BasicBlock Label by its Minir index.
 */
Label
Minir_findBasicBlockLabel(Minir this, long int bbIndex);
#endif//$XCC_h

Label
Minir_findBasicBlockLabel(Minir this, long int bbIndex)
{
  long int key = bbIndex;
  void *pointer = HTable_search(Minir_BLABELS(this), (HTableKey)key);
  if (pointer != NULL) return *(Label *)pointer;
  return NULL;
}

#ifdef $XCC_h
/**
 * Find a BasicBlock by its label name.
 *
 * @param this is used to throw an exception if required.
 */
BasicBlock
Minir_findBasicBlockByName(Minir minir, yaml_node_t *this, const char *name);
#endif//$XCC_h

BasicBlock
Minir_findBasicBlockByName(Minir minir, yaml_node_t *this, const char *name)
{
  long int key = Minir_bbIndexOfName(minir, this, name);
  void *pointer = HTable_search(Minir_BLOCKS(minir), (HTableKey)key);
  if (pointer != NULL) return *(BasicBlock *)pointer;
  return NULL;
}

#ifdef $XCC_h
/**
 * Make a BasicBlock.
 */
BasicBlock
Minir_makeBasicBlock(Minir this, long int bbIndex, char* name, float frequency);
#endif//$XCC_h

BasicBlock
Minir_makeBasicBlock(Minir this, long int bbIndex, char* name, float frequency)
{
  BasicBlock block;
  Program program = Minir_program(this);
  Procedure procedure =  Minir_procedure(this);
  LabelTable labelTable = Program_labelTable(program);
  CodeRegion codeRegion;
  int regionId = 0;
  Label label;
  long key = bbIndex;
  void *pointer = HTable_insert(Minir_BLOCKS(this),
      (HTableKey)key,
      sizeof(BasicBlock));
  Except_THROWIF(!pointer,
      "There is already a basic block with index %ld!",
      bbIndex);
  codeRegion = Procedure_findCodeRegion(procedure, regionId);
  // Code taken from Procedure.c
  // Executed here to have control over the index of the basic block
  if (codeRegion == NULL) {
    Memory memory = Procedure_MEMORY(procedure);
    IDList codeRegions = Procedure_CODEREGIONS(procedure);
    Optimize optimize = Program_OPTIMIZE(program);
    Configure configure = Program_CONFIGURE(program);
    size_t size = CodeRegion_Size(memory, procedure, regionId, optimize, configure);
    codeRegion = CodeRegion_Ctor(IDList_push(codeRegions, size),
                                 memory, procedure, regionId, optimize, configure);
  }
/*  IndexedFactory_COUNTER(BasicBlock_Factory) = bbIndex;*/
  block = *(BasicBlock*)pointer = Procedure_makeBasicBlock(
      Minir_PROCEDURE(this), regionId, frequency);
/*  IndexedFactory_COUNTER(BasicBlock_Factory) =*/
/*    IndexedFactory_MAXCOUNT(BasicBlock_Factory);*/
  {
    char labelName[16];
    // snprintf(labelName, 63, ".LAO_%d_%s", BasicBlock_identity(block), name);
    snprintf(labelName, 15, ".LAO_%d_%d",
        Procedure_IDENTITY(procedure),
        BasicBlock_identity(block));
  // Add basic block label
    label = Minir_makeLabel(labelTable, labelName);
    BasicBlock_addLabel(block, label);
  }
  // Store name in Minir_BNAMES
  pointer = HTable_insert(Minir_BNAMES(this),
      (HTableKey)key,
      sizeof(char*));
  *(char**)pointer = name;
  // Store label in Minir_BLABELS
  pointer = HTable_insert(Minir_BLABELS(this),
      (HTableKey)key,
      sizeof(Label));
  *(Label*)pointer = label;
  return block;
}


#ifdef $XCC_h
/**
 * Attach a new label to a BasicBlock.
 */
void
Minir_addBasicBlockLabel(Minir this, BasicBlock block, char* name);
#endif//$XCC_h

void
Minir_addBasicBlockLabel(Minir this, BasicBlock block, char* name)
{
  static long dummy_key = 0;
  Program program = Minir_PROGRAM(this);
  LabelTable labelTable = Program_labelTable(program);
  Label label = Minir_makeLabel(labelTable, name);
  void *pointer;
  // Add basic block label
  BasicBlock_addLabel(block, label);
  dummy_key--;
  // Store name in Minir_BNAMES
  pointer = HTable_insert(Minir_BNAMES(this),
      (HTableKey)dummy_key,
      sizeof(char*));
  *(char**)pointer = name;
  // Store label in Minir_BLABELS
  pointer = HTable_insert(Minir_BLABELS(this),
      (HTableKey)dummy_key,
      sizeof(Label));
  *(Label*)pointer = label;
}


#ifdef $XCC__h
/**
 * Create a new label in LabelTable.
 *
 * Takes care of index creation.
 */
Label
Minir_makeLabel(LabelTable this, const char *name);
#endif//$XCC__h

Label
Minir_makeLabel(LabelTable this, const char *name)
{
  static long int key = 0;
  Label label;
#ifdef MINIR_DEBUG 
  printf("The table key for labels is now %ld\n", key);
#endif
  label = LabelTable_lookup(this, key, name);
  key++;
  return label;
}

#ifdef $XCC_h
/**
 * Find a Temporary by its Minir index.
 */
Temporary
Minir_findTemporary(Minir this, long int tempIndex);
#endif//$XCC_h

Temporary
Minir_findTemporary(Minir this, long int origIndex)
{
  void *pointer = HTable_search(Minir_TEMPS(this), (HTableKey)origIndex);
  if (pointer != NULL) return *(Temporary *)pointer;
  return NULL;
}

#ifdef $XCC_h
/**
 * Get a new index for a Temporary, trying if possible to keep the original 
 * index.
 */
static inline long int
Minir_newTemporaryIndex(Minir this, long int origIndex)
{
  long int newIndex;
  static int offset = 0;
  Temporary temporary;
  while(origIndex+offset < Minir_MIN_TEMP_INDEX(this)) {
    offset += 100;
#ifdef     MINIR_DEBUG 
      fprintf(stderr, "%s:%d	OrigIndex to small (%ld<%d), offset is now %d\n",
        __FILE__, __LINE__,
        origIndex,
        Minir_MIN_TEMP_INDEX(this),
        offset
        );
#endif
  }
  while(Minir_findTemporary(this, origIndex+offset) != NULL)
    offset += 100;
  return (offset+origIndex);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Get a new index for an SSA Variable based on newIndex.
 * index.
 */
static inline long int
Minir_newSSAfromTemporaryIndex(Minir this, long int newIndex)
{
  long int ssaTempIndex;
  int offset = 100;
  Temporary temporary;
  while(Minir_findTemporary(this, newIndex+offset) != NULL)
    offset += 100;
  return (offset+newIndex);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Init temporaries.
 */
static inline void
Minir_initTemporaries(Minir minir)
{
  *Minir__MIN_TEMP_INDEX(minir) = IndexedFactory_COUNTER(Temporary_Factory);
}
#endif//$XCC_h



#ifdef $XCC_h
/**
 * Make a Temporary
 */
Temporary
Minir_makeTemporary(Minir this, const char* name, long int origIndex, RegFile regFile);
#endif//$XCC_h

Temporary
Minir_makeTemporary(Minir this, const char* name, long int origIndex, RegFile regFile)
{
  Temporary temporary = NULL;
  static int programCounter = 0;
  static int temp100prefix = -1;
  static int base100 = 0;
  long int newIndex = Minir_newTemporaryIndex(this, origIndex);
  void *pointer;
  Procedure procedure = Minir_procedure(this);
  pointer = HTable_insert(
      Minir_TEMPS(this),
      (HTableKey)origIndex,
      sizeof(Temporary));
  Except_THROWIF(!pointer,
      "There is already a temporary with index %ld!",
      origIndex);
/*  switch (name[0]) {*/
/*    case 'T' :*/
/*    case 'V' : regFile = 2 ; break ; //FIXME: RegFile_**_GRF*/
/*    case '_' :*/
/*    default : regFile = RegFile__UNDEF;*/
/*  }*/
  // TODO check modifying Temporary_Factory counter this is the right way
  if(!Minir_KEEP_INDEX(this)) {
    if(Minir_isSSA(this)) {
      temporary = Procedure_makeVirtualTemporary(procedure, regFile);
      temporary = *(Temporary*)pointer =
        CodeRegion_makeVariableTemporary(Minir_CODEREGION(this), temporary);
      Variable_makeConsumers(
          Temporary_VARIABLE(temporary),
          Procedure_memory(procedure));
    } else {
      temporary = *(Temporary*)pointer =
        Procedure_makeVirtualTemporary(procedure, regFile);
    }
  }
  else { // Try to keep lowest digits
    if(Minir_isSSA(this)) {
      long int ssaTempIndex = Minir_newSSAfromTemporaryIndex(this, newIndex);
      IndexedFactory_COUNTER(Temporary_Factory) = ssaTempIndex;
      temporary = Procedure_makeVirtualTemporary(procedure, regFile);
    }
    // save current counter
    IndexedFactory_COUNTER(Temporary_Factory) = newIndex;
    if(Minir_isSSA(this)) {
      temporary = *(Temporary*)pointer =
        CodeRegion_makeVariableTemporary(Minir_CODEREGION(this), temporary);
      Variable_makeConsumers(
          Temporary_VARIABLE(temporary),
          Procedure_memory(procedure));
    } else {
      temporary = *(Temporary*)pointer =
        Procedure_makeVirtualTemporary(procedure, regFile);
    }
    // Restore old counter
    IndexedFactory_COUNTER(Temporary_Factory) = 
      IndexedFactory_MAXCOUNT(Temporary_Factory);
  }
  return temporary;
}

#ifdef $XCC__h
/**
 * Record an operation has future dependence node.
 */
void
Minir_recordDependenceNode(Minir minir, yaml_node_t *this, const char *name, Operation op);
#endif//$XCC__h

void
Minir_recordDependenceNode(Minir minir, yaml_node_t *this, const char *name, Operation op)
{
  IArray depNodes = Minir_DEPNODES(minir);
  int32_t nodeNum;
  Minir_THROWIF(this, name[0] != 'N',
      "`node' key should start by `N' or `N_' but is `%s'!", name);
  nodeNum = int_of_string(name + (name[1] == '_' ? 2 : 1));
  Minir_THROWIF(this, errno != 0,
      "Cannot get node number of `%s'!", name);
  if(nodeNum >= IArray_maxCount(depNodes)) {
    IArray_resize(depNodes, HackerU32_clp2(nodeNum+1)); // nearest power of 2
  }
  Minir_THROWIF(this, *(Operation*)IArray_access(depNodes, nodeNum) != NULL,
      "There is already a dependence node with number %d!", nodeNum);
  *(Operation*)IArray_access(depNodes, nodeNum) = op;
}

#ifdef $XCC__h
/**
 * Get an operation that is dependence node based on its name.
 */
Operation
Minir_getDependenceNode(Minir minir, yaml_node_t *this, const char *name);
#endif//$XCC__h

Operation
Minir_getDependenceNode(Minir minir, yaml_node_t *this, const char *name)
{
  IArray depNodes = Minir_DEPNODES(minir);
  int32_t nodeNum;
  Minir_THROWIF(this, name[0] != 'N' || name[1] != '_',
      "`node' key should start by `N_' but is `%s'!", name);
  nodeNum = int_of_string(name+2);
  return *(Operation*)IArray_access(depNodes, nodeNum);
}

#ifdef $XCC__h
/**
 * Record a loopScope using its index.
 */
void
Minir_recordLoopScope(Minir minir, yaml_node_t *this, int32_t index, LoopScope loopScope);
#endif//$XCC__h

void
Minir_recordLoopScope(Minir minir, yaml_node_t *this, int32_t index, LoopScope loopScope)
{
  IArray loops = Minir_LOOPS(minir);
  if(index >= IArray_maxCount(loops)) {
    IArray_resize(loops, HackerU32_clp2(index+1)); // nearest power of 2
  }
  Minir_THROWIF(this, *(LoopScope*)IArray_access(loops, index) != NULL,
      "There is already a dependence node with number %d!", index);
  *(LoopScope*)IArray_access(loops, index) = loopScope;
}

#ifdef $XCC__h
/**
 * Get an LoopScope based on its name.
 */
LoopScope
Minir_getLoopScope(Minir minir, yaml_node_t *this, const char *name);
#endif//$XCC__h

LoopScope
Minir_getLoopScope(Minir minir, yaml_node_t *this, const char *name)
{
  IArray loops = Minir_LOOPS(minir);
  int32_t index;
  Minir_THROWIF(this, name[0] != 'L' || name[1] != '_',
      "Loop should start by `L_' but is `%s'!", name);
  index = int_of_string(name+2);
  if(index >= IArray_maxCount(loops)) 
    return NULL;
  return *(LoopScope*)IArray_access(loops, index);
}



#ifdef $XCC__h
/**
 * Transform to SSA.
 * @warning not working
 */
void
Minir_transformSSAVariables(Procedure procedure);
#endif//$XCC__h

void
Minir_transformSSAVariables(Procedure procedure)
{
  Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
    CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
      BasicBlock_FOREACH_Operation(basicBlock, operation) {
        Operation_OPERANDS_FOREACH_Temporary(operation, temporary) {
          if(Temporary_isVirtual(temporary)) {
            Temporary SSAVariable = CodeRegion_makeVariableTemporary(codeRegion, temporary);
            Operation_OPERANDS[Operation_OPERANDS_INDEX] = SSAVariable;
          }
        } Operation_OPERANDS_ENDEACH_Temporary ;
      } BasicBlock_ENDEACH_Operation;
    } CodeRegion_ENDEACH_BasicBlock;
  } Procedure_ENDEACH_CodeRegion;
}



/*************************************
 *  Parsing functions
 *************************************/

#ifdef $XCC__h
/**
 * Parse a node in Minir format (debug function).
 */
void
Minir_node_parse(yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

void 
Minir_node_parse(yaml_document_t *document, yaml_node_t *this)
{
  yaml_node_pair_t *pair;
  yaml_node_item_t *item;
  yaml_node_t *node;
  switch (this->type) {
    case YAML_NO_NODE:
      fprintf(stderr, "Error: empty node!\n");
      break;
    case YAML_SCALAR_NODE:
      printf("A scalar node of length %zu with value: '%s'\n",
          this->data.scalar.length, this->data.scalar.value);
      break;
    case YAML_SEQUENCE_NODE:
      printf("A sequence node of style %d.\n", this->data.sequence.style);
      for(  item = this->data.sequence.items.start ; item < this->data.sequence.items.top ; item++ ) {
        fprintf(stderr, "Automatic lookup of node %p -> %d!\n", item, *item);
        node = yaml_document_get_node(document, *item);
        if(!node) {
          fprintf(stderr, "Cannot find node %p in document!", item);
        } else {
          node_pretty(document, node);
        }
      }
      break;
    case YAML_MAPPING_NODE:
      printf("A mapping node of style %d.\n", this->data.mapping.style);
      for(pair = this->data.mapping.pairs.start ;
          pair < this->data.mapping.pairs.top ; pair++ ) {
        fprintf(stderr, "Automatic lookup of node %p -> (%d,%d)!", pair, pair->key, pair->value);
        node = yaml_document_get_node(document, pair->key);
        node_pretty(document, node);
        node = yaml_document_get_node(document, pair->value);
        node_pretty(document, node);
      }
      break;
    default:
      fprintf(stderr, "NO SUCH NODE TYPE KNOWN!\n");
  }
}

#ifdef $XCC__h
/**
 * Parse a symbolic operand of a Minir operation.
 *
 * @return a Temporary.
 */
Temporary
Minir_parseSymbolic(Minir minir, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

Temporary
Minir_parseSymbolic(Minir minir, yaml_document_t *document, yaml_node_t *this)
{
  Temporary temporary = NULL;
//  Procedure procedure = BasicBlock_procedure(block);
  char* string = NULL;
  Symbol symbol = NULL;
  ImmediateValue offset = 0;
  Relocation relocation = Relocation__UNDEF;
  Label label;
  // No "sym" mapping key anymore
  //yaml_node_t *node = Yaml_mapping_lookup_key(document, this, "sym");
  //Except_THROWIF(!node, "Key `sym' required in operand mapping!");
  Yaml_FORSEQ(document, this, symargs) {
    string = Yaml_GET_SCALAR_STRING(document, symargs);
    /* First element is the symbol */
    if(!symbol) {
/*      if(Minir_isBasicBlockName(minir, string)) {*/
/*        Minir_THROW(symargs, "No basic block name allowed here.");*/
/*      } else {*/
        Program program = Minir_PROGRAM(minir);
        SymbolTable symbolTable = Program_symbolTable(program);
        symbol = SymbolTable_searchName(symbolTable, string);
        if(symbol == NULL) { // Assume external symbol
          symbol = SymbolTable_makeSymbol(symbolTable, string);
          Symbol_setClasses(symbol,
              SymbolClass__UNDEF,
              SymbolStore__UNDEF,
              SymbolExport__UNDEF
              );
        }
/*      }*/
    } else {
      switch(string[0]) {
        case '@': relocation = Relocation__UNDEF;
                  Minir_WARNING(symargs, "Warning: relocation not correctly handled for now (%s)!\n", string);
/*                  Relocation_ofId(string+1); |+ TODO relocation +|*/
                  break;
        case '+': offset = int_of_string(string+1);
                  break;
        case '-': offset = - int_of_string(string+1);
                  break;
        default: Except_THROW("Don't know what to do with `%s' in symbol array!", string);
      }
    }
  } Yaml_ENDSEQ;
  temporary = Program_makeSymbolTemporary(
      Minir_PROGRAM(minir),
      symbol,
      offset,
      Immediate__UNDEF);
#ifdef MINIR_DEBUG
  fprintf(stderr, "%s:%d temporary for symbol: (%d) ", __FILE__, __LINE__, Temporary_identity(temporary));
  Temporary_pretty(temporary, stderr);
  fprintf(stderr, "\n");
#endif
  return temporary;
}

#ifdef $XCC__h
/**
 * Parse and set attributes for a given symbol.
 *
 * Modifies directly the symbol attributes.
 */
void
Minir_parseSymAttributes(yaml_document_t *document, yaml_node_t *this, Symbol symbol);
#endif//$XCC__h

void
Minir_parseSymAttributes(yaml_document_t *document, yaml_node_t *this, Symbol symbol)
{
  Except_THROWIF(!symbol, "Cannot set attributes for null symbol");
  Yaml_FORSEQ(document, this, symattr) {
    char *string = Yaml_GET_SCALAR_STRING(document, symattr);
    if(!strcmp(string, "comm")) {
      Symbol_setStore(symbol, SymbolStore_COMMON);
    }
    else if(!strcmp(string, "local")) {
      Symbol_setExport(symbol, SymbolExport_LOCAL);
    }
    else if(!strcmp(string, "internal")) {
      Symbol_setExport(symbol, SymbolExport_LOCAL_INTERNAL);
    }
    else if(!strcmp(string, "protected")) {
      Symbol_setExport(symbol, SymbolExport_GLOBAL_PROTECTED);
    }
    else if(!strcmp(string, "hidden")) {
      Symbol_setExport(symbol, SymbolExport_GLOBAL_HIDDEN);
    }
    // fprintf(stderr, "Found attribute `%s'\n", string);
  } Yaml_ENDSEQ;
}


#define Minir_checkTemporaryType(where, type, parameter) \
  if(parameter) { \
    OperandType expected = OperatorParameter_type(parameter) ; \
    if( expected == type || \
        (expected == OperandType_RegClass && \
         OperatorParameter_enum(parameter) == RegClass__UNDEF)) { \
    } else { \
      Minir_WARNING(where, \
          "Operand has type %s but type %s was expected!", \
          OperandType_Id(type), OperandType_Id(expected)); \
    } \
  }
 
#ifdef $XCC__h
/**
 * Check if a parameter expects a fixed register.
 *
 * If it expects a fixed reg, i.e., parameter has class Register, or a RegClass 
 * with only one register, check if the yaml node is this fixed register.
 * @return the fixed register if requires a fixed register AND the yaml node is 
 * NOT the fixed register (or the yaml node is NULL).
 * @return @c NULL otherwise.
 * 
 */
Temporary
Minir_checkTemporaryFixedRegister(Minir minir, BasicBlock block, yaml_document_t *document, yaml_node_t *this, const_OperatorParameter parameter);
#endif//$XCC__h

Temporary
Minir_checkTemporaryFixedRegister(Minir minir, BasicBlock block, yaml_document_t *document, yaml_node_t *this, const_OperatorParameter parameter)
{
  Procedure procedure = Minir_PROCEDURE(minir);
  OperandType type = parameter ? OperatorParameter_type(parameter) : OperandType_RegClass;
  Register expected_reg = Register__UNDEF;
  char *string;
  Register reg;
  Temporary temp;
  // Not a fixed register operand
  if (type == OperandType_Register) {
    expected_reg = OperatorParameter_enum(parameter);
  } else {
    int regClass;
    if (!parameter || type != OperandType_RegClass)
      return NULL ;
    regClass = OperatorParameter_enum (parameter);
    RegisterSet_FOREACH_Register (RegClass_registerSet (regClass), reg) {
      if (expected_reg != Register__UNDEF)
        return NULL ; /* More than one register in this regClass */
      expected_reg = reg;
    } RegisterSet_ENDEACH_Register ;
  }
  if (expected_reg == Register__UNDEF)
    return NULL;
  if (this) {
    string = Yaml_GET_SCALAR_STRING(document, this);
    if(string[0] == '$') {
      reg = Register_fromName(string, Procedure_processor(procedure));
      if(reg == expected_reg) return NULL; // fixed register expected and fixed reg given, ok
    }
  }
  // In any other case, fixed register not given. Construct it.
  temp = Procedure_makeDedicatedTemporary(procedure, expected_reg);
  return temp;
}

#ifdef $XCC__h
/**
 * Parse an operand of a Minir operation.
 *
 * @param parameter is the OperatorParameter expected by the operator for this 
 * temporary. A @c NULL value means no operator associated, and considers the 
 * Temporary should have type RegClass, and is in the general purpose class.
 * @return a Temporary.
 * @retval NULL if the operand is a BasicBlock label.
 *
 * @todo should return an SSA @c Variable.
 */
Temporary
Minir_parseTemporary(Minir minir, BasicBlock block, yaml_document_t *document, yaml_node_t *this, const_OperatorParameter parameter);
#endif//$XCC__h

Temporary
Minir_parseTemporary(Minir minir, BasicBlock block, yaml_document_t *document, yaml_node_t *this, const_OperatorParameter parameter)
{
  Temporary temporary;
  Procedure procedure = Minir_PROCEDURE(minir);
  char* string;
  char* ptr;
  ImmediateValue val;
  Label label;
  Symbol symbol;
  OperandType type = parameter ? OperatorParameter_type(parameter) : OperandType_RegClass;
  // Mapping possible for symbolics
  if(Yaml_IS_SEQUENCE(this)) {
    temporary = Minir_parseSymbolic(minir, document, this);
    return temporary;
  }
  // Otherwise, parse the string
  string = Yaml_GET_SCALAR_STRING(document, this);
  if(string[0] == '$') { /* Operand is a register */
    Register reg = Register_fromName(string, Procedure_processor(procedure));
    if(type == OperandType_Register) {
      int expected_reg = OperatorParameter_enum(parameter);
      Minir_THROWIF(this, reg != expected_reg,
          "Expected register %s but given %s!",
          Register_name(expected_reg), 
          Register_name(reg));
    } else if (type == OperandType_RegClass) {
      int regClass = parameter ? OperatorParameter_enum(parameter) : -1;
      Except_CHECK(parameter);
      Minir_THROWIF(this,
          regClass != RegClass__UNDEF && !RegClass_contains(regClass, reg),
          "Operator needs an operand in RegClass %s but %s is not in this RegClass!",
          RegClass_Id(regClass), string);
    } else {
/*      Minir_THROW(this, "Operand has type %s but type Register or RegClass was expected!", OperandType_Id(type));*/
      Minir_WARNING(this, "Operand has type %s but type Register or RegClass was expected!", OperandType_Id(type));
    }
    temporary = Procedure_makeDedicatedTemporary(procedure, reg);
  }
  else if(string[0] == 'V' || string[0] == 'T' || string[0] == '_') {
    int tempIndex = int_of_string(string+ (string[1] == '_' ? 2 : 1));
    Minir_THROWIF(this, errno != 0, "Cannot find number for register %s!", string);
    Minir_checkTemporaryType(this, OperandType_RegClass, parameter);
    temporary = Minir_findTemporary(minir, tempIndex);
    if(!temporary) {
      int regFile = parameter ? 
        RegClass_regFile(OperatorParameter_enum(parameter)) :
        RegFile__UNDEF;
      if(regFile == RegFile__UNDEF) 
        regFile = Selector_genPurposeRegFile();
      Except_CHECK(parameter);
      temporary = Minir_makeTemporary(minir, string, tempIndex, regFile);
    }
  }
  else if(string[0] == '(') { /* Maybe a function label for call */
    Minir_THROW(this, "Do not handle operands with parenthesis!");
  }
  else if(({val = int_of_string(string); errno == 0;})) {
    Minir_checkTemporaryType(this, OperandType_Immediate, parameter);
    temporary = Procedure_makeAbsoluteTemporary(procedure, val, Immediate__UNDEF);
  }
  else if((label = Minir_isBasicBlockName(minir, string))) {
    // fprintf(stderr, "Label of bb name %s (%s)\n", string, Label_name(label));
    Minir_checkTemporaryType(this, OperandType_Immediate, parameter);
    temporary = Procedure_makeLabelTemporary(procedure, label, Immediate__UNDEF);
  }
  // Maybe this is a simple computation
  else if ((ptr = strchr (string, '+')) || (ptr = strchr (string, '-'))) {
    ImmediateValue sum = 0;
    char* prev_ptr = string;
    bool prev_op_plus_p = true;
    bool curr_op_plus_p;
    bool end_of_string_p = false;
    ptr = string;
    while (! end_of_string_p) {
      ptr = strchr (prev_ptr, '+');
      if (!ptr) ptr = strchr (prev_ptr, '-');
      if (!ptr) ptr = prev_ptr + strlen (prev_ptr); /* No access to strchrnul function */
      switch (*ptr) {
        case '+' : curr_op_plus_p = true;  break;
        case '-' : curr_op_plus_p = false; break;
        case '\0': end_of_string_p = true; break;
        default : Minir_THROW(this, "Cannot perform computation!");
      }
      *ptr = '\0';
      val = int_of_string (prev_ptr);
      if (prev_op_plus_p) 
        sum += val;
      else
        sum -= val;
      *ptr = curr_op_plus_p ? '+' : '-'; // Restore the char
      prev_op_plus_p = curr_op_plus_p;
      prev_ptr = ptr+1;
    }
    Minir_checkTemporaryType(this, OperandType_Immediate, parameter);
    temporary = Procedure_makeAbsoluteTemporary(procedure, sum, Immediate__UNDEF);
  }
  else if((symbol = Minir_isProcedureName(minir, string))) {
    Minir_checkTemporaryType(this, OperandType_Immediate, parameter);
    temporary = Procedure_makeSymbolTemporary(procedure, symbol,
        (ImmediateValue)0, Immediate__UNDEF);
  }
  else {
    /* External symbol? */
    //Program program = Minir_PROGRAM(minir);
    //SymbolTable symbolTable = Program_symbolTable(program);
    Minir_THROW(this, "Don't know what to do with `%s'! (mistyped label?)", string);
    //temporary = Procedure_makeVirtualTemporary(procedure, 0);
    //printf("Warning: should be symbol or label temporary probably \"%s\"\n", string);
  }
  return temporary;
}

#ifdef $XCC__h
/**
 * Parse operation node in Minir format.
 */
Operation
Minir_parseOperation(Minir minir, long int bbIndex, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

Operation
Minir_parseOperation(Minir minir, long int bbIndex, yaml_document_t *document, yaml_node_t *this)
{
  char *string;
  int resCount=0; // number of results
  int argCount=0; // number of arguments (non-modifiers)
  int paramCount=0; // Total number of parameters (res + arg)
  int count;      // foo counter
  int identity=-1;   // Operation identity if given as O_xx
  char* depNode=NULL;// Node identifier of this operation if present
  Label target = NULL;
  BasicBlock bbTarget = NULL;
  BasicBlock bbFallthrough = NULL;
  BasicBlock block = Minir_findBasicBlock(minir, bbIndex);
  Operator operator = Operator__UNDEF;
  Selector selector = BasicBlock_selector(block);
  Procedure procedure = BasicBlock_procedure(block);
  Operation operation;
  RegisterSet_ clobberSet_;
  RegisterSet clobberSet = NULL;
  Temporary *arguments;
  Temporary *results;
  Temporary temp;
  yaml_node_t *node, *nodeDefs = NULL, *nodeUses = NULL;
  yaml_node_t *flags = NULL;
  float frequency = -1.0; // for control-flow edges
  Except_CHECK(block);
  // operators don't have modifiers anymore
  // simply cb.eqz and cb.nez are separate operators now
  // First, find the operator
  // Finding operator & modifiers
  Yaml_FORSTRINGMAP(document, this, key, value) {
    if(!strcmp(key, "op") ||
       (key[0] == 'O' && key[1] == '_')
        ) {
      if(key[0] == 'O') { // if O_xx, we use xx as operation identity
        identity = int_of_string(key+2);
      }
      string = Yaml_GET_SCALAR_STRING(document, value);
      operator = Operator_fromShortName_or_Mnemonic(string, Procedure_processor(procedure));
      Minir_THROWIF(value, (operator == Operator__UNDEF),
          "Cannot find an operator corresponding to ShortName (or Mnemonic) `%s'!", string);
      // Setup arg and res count
      resCount = Operator_isWriteList(operator) ? -1 : Operator_writeCount(operator);
      argCount = Operator_isReadList(operator) ? -1 : Operator_readCount(operator);
    }
    else if(!strcmp(key, "defs")) {
      nodeDefs = value;
    }
    else if(!strcmp(key, "uses")) {
      nodeUses = value;
    }
    /* Additional definitions */
    else if(!strcmp(key, "outs")) {
      Minir_WARNING(this, "Additional definitions provided. Not used for now!\n");
    }
    /* Additional parameters as inputs */
    else if(!strcmp(key, "ins")) {
      Minir_WARNING(this, "Additional inputs provided. Not used for now!\n");
    }
    else if(!strcmp(key, "target")) {
      long int targNum;
      string = Yaml_GET_SCALAR_STRING(document, value);
#ifdef     MINIR_DEBUG 
      printf("Operation has a BB target: %s\n", string);
#endif
      targNum = Minir_bbIndexOfName(minir, value, string);
#ifdef     MINIR_DEBUG 
      printf("                BB target: targNum %ld\n", targNum);
#endif
      Except_CHECK(!strcmp(string, Minir_findBasicBlockName(minir, targNum)));
      target = Minir_findBasicBlockLabel(minir, targNum);
      bbTarget = Minir_findBasicBlock(minir, targNum);
    }
    else if(!strcmp(key, "freq") ||
            !strcmp(key, "frequency")) {
      // Target edge frequency.
      frequency = Yaml_GET_FLOAT(document, value);
#ifdef     MINIR_DEBUG 
      printf("Frequency for target is %f\n", frequency);
#endif
    }
    else if(!strcmp(key, "fallthru")) {
      string = Yaml_GET_SCALAR_STRING(document, value);
      bbFallthrough = BasicBlock_next(block);
      if(strcmp(string, ".next")) {
        bool matches = false;
        // Check "string" is a valid label of following basic block
        Label label = Minir_isBasicBlockName(minir, string);
        BasicBlock_FOREACH_Label(bbFallthrough, check_label) {
          if(label == check_label) {
            matches = true;
            break;
          }
        } BasicBlock_ENDEACH_Label;
        Minir_THROWIF(this, !label || !matches,
            "fallthru label `%s' is not a valid label of the following basic block (%s)!",
            string, Label_name(BasicBlock_label(bbFallthrough)));
      }
      Minir_THROWIF(this, bbFallthrough == NULL, "Cannot find fall-through basic block %s of current block (%s)!", string, Label_name(BasicBlock_label(block)));
#ifdef     MINIR_DEBUG 
      printf("    key fallthru: linking to bb %s\n",
          Label_name(BasicBlock_label(block)));
#endif
    }
    else if(!strcmp(key, "implicit_defs") ||
            !strcmp(key, "clobbers")) {
      char *c;
      Register reg;
      clobberSet = RegisterSet_Ctor(&clobberSet_);
      Yaml_FORSTRINGSEQ(document, value, regstring) {
        // Can be stated as a sequence, like $r0-$r9
        if((c = strchr(regstring, '-'))) {
          int rstart, rend;
          Register reg_start, reg_end;
          *c++ = '\0'; // Separate the string by a NULL
          rstart = int_in_string(regstring);
          Minir_THROWIF(this, errno != 0, "Cannot find start number for register in string %s!", regstring);
          rend = int_in_string(c);
          Minir_THROWIF(this, errno != 0, "Cannot find end number for register in string %s!", c);
          reg_start = Register_fromName(regstring, Procedure_processor(procedure));
          reg_end = Register_fromName(c, Procedure_processor(procedure));
          for(reg = reg_start; reg < reg_start + (rend - rstart) + 1; reg++) {
            RegisterSet_insert(clobberSet, reg);
          }
          Except_THROWIF(reg != reg_end+1,
              "End register for clobbers is now `%s' but should be `%s'!",
              Register_name(reg), Register_name(reg_end+1));
        } else {
          // Regular clobber, one by one
          reg = Register_fromName(regstring, Procedure_processor(procedure));
          RegisterSet_insert(clobberSet, reg);
        }
      } Yaml_ENDSEQ;
    }
    else if(!strcmp(key, "node")) {
      depNode = Yaml_GET_SCALAR_STRING(document, value);
    }
    else if(!strcmp(key, "flags")) {
      flags = value;
    }
    else {
      Minir_WARNING(value, "Unknown key in map: '%s'\n", key);
    }
  } Yaml_ENDSTRINGMAP;
  //
  // Need now to check consistency of operation
  //
  Minir_THROWIF(this, !operator, "No operator given for operation!");
  /* Now Parsing definitions */
  if (nodeDefs) {
    int res = Yaml_SEQ_COUNT(document, nodeDefs);
    int resIndex;
    const_OperatorParameter parameter;
    Temporary temp;
    unsigned int seqRemaining = Yaml_NUMSEQ (document, nodeDefs);
    bool var_length_p = resCount == -1;
    Minir_THROWIF(nodeDefs, !operator,
        "Results of operation must appear after the operator!");
    // Variable list length
    if (var_length_p) resCount = seqRemaining;
    // Scanning the write parameters
    Yaml_SCANSEQ_INIT(document, nodeDefs, def) {
      for (resIndex=0; resIndex < resCount; resIndex++) {
        parameter = Operator_writeParameter(operator, 
            var_length_p ? 0 : resIndex);
        // if it returns non-null, a fixed reg was expected, but the node is 
        // not a fixed reg and the temp returned is the fixed reg constructed.
        temp = Minir_checkTemporaryFixedRegister(minir, block, document, def, parameter);
        if (temp) {
          PtrSeq_push2(Minir_TEMPRESULTS(minir), temp);
          // current node was not used not scanning next sequence item
        } else {
          // Checking if parameter is constrained to be same as other
          // And there is not enough remaining items sequence item
          if (Operator_isConstrained (operator) &&
              OperatorParameter_same (parameter) != -1 &&
              seqRemaining < resCount - resIndex) {
            // In that case, just insert NULL and fix it later
            PtrSeq_push2(Minir_TEMPRESULTS(minir), NULL);
          } else {
            Minir_THROWIF(nodeDefs, !def,
                "Trying to get a definition but no more item!");
            temp = Minir_parseTemporary(minir, block, document, def, parameter);
            PtrSeq_push2(Minir_TEMPRESULTS(minir), temp);
            Yaml_SCANSEQ_NEXT(document, nodeDefs, def);
            seqRemaining--;
          }
        }
      }
      Minir_THROWIF(nodeDefs, seqRemaining != 0,
          "There remains %d parameters unused in def!", seqRemaining);
    } Yaml_SCANSEQ_END;
  }
  else if (resCount > 0) { /* Maybe only fixed regs results or SAME */
    int resIndex;
    for (resIndex=0; resIndex < resCount; resIndex++) {
      const_OperatorParameter parameter = Operator_writeParameter(operator, resIndex);
      temp = Minir_checkTemporaryFixedRegister(minir, block, document, NULL, parameter);
      if (temp) {
        PtrSeq_push2(Minir_TEMPRESULTS(minir), temp);
      } else {
        if (Operator_isConstrained (operator) &&
            OperatorParameter_same (parameter) != -1) {
          // In that case, just insert NULL and fix it later
          PtrSeq_push2(Minir_TEMPRESULTS(minir), NULL);
        }
        else {
          Minir_THROWIF(this, !temp,
              "Some non fixed-reg use required but not found!");
        }
      }
    }
  }
  /* Now Parsing uses */
  if (nodeUses) {
    int arg = Yaml_SEQ_COUNT(document, nodeUses);
    int argIndex;
    const_OperatorParameter parameter;
    Temporary temp;
    unsigned int seqRemaining = Yaml_NUMSEQ (document, nodeUses);
    bool var_length_p = argCount == -1;
    Minir_THROWIF(nodeUses, !operator,
        "arguments of operation must appear after the operator!");
    // Variable list length
    if (var_length_p) argCount = seqRemaining;
    // Scanning the read parameters
    Yaml_SCANSEQ_INIT(document, nodeUses, def) {
      for (argIndex=0; argIndex < argCount; argIndex++) {
        parameter = Operator_readParameter(operator, 
            var_length_p ? 0 : argIndex);
        // if it returns non-null, a fixed reg was expected, but the node is 
        // not a fixed reg and the temp returned is the fixed reg constructed.
        temp = Minir_checkTemporaryFixedRegister(minir, block, document, def, parameter);
        if (temp) {
          PtrSeq_push2(Minir_TEMPARGUMENTS(minir), temp);
          // current node was not used not scanning next sequence item
        } else {
          // Checking if parameter is target of a constrained parameter
          // And there is not enough remaining items sequence item
          if (Operator_isConstrained (operator)   &&
              seqRemaining < argCount - argIndex  &&
              ({bool constr_p = false;
               uint8_t resIndex = 0;
               PtrSeq_FOREACH(Minir_TEMPRESULTS(minir), Temporary, tempDef) {
               if (resCount + argIndex == 
                 OperatorParameter_same (
                   Operator_writeParameter(operator, resIndex ))) {
               constr_p = true;
               break;
               }
               resIndex ++;
               } PtrSeq_ENDEACH ;
               constr_p ; // Return the predicate on constraint
               }))
          {
            // In that case, just insert NULL and fix it later
            PtrSeq_push2(Minir_TEMPARGUMENTS(minir), NULL);
          } else {
            Minir_THROWIF(nodeUses, !def,
                "Trying to get a definition but no more item!");
            temp = Minir_parseTemporary(minir, block, document, def, parameter);
            PtrSeq_push2(Minir_TEMPARGUMENTS(minir), temp);
            Yaml_SCANSEQ_NEXT(document, nodeUses, def);
            seqRemaining--;
          }
        }
      }
      Minir_THROWIF(nodeUses, seqRemaining != 0,
          "There remains %d parameters unused in uses!", seqRemaining);
    } Yaml_SCANSEQ_END;
  }
  else if (argCount > 0) { /* Maybe only fixed regs results */
    int argIndex;
    for (argIndex=0; argIndex < argCount; argIndex++) {
      const_OperatorParameter parameter = Operator_readParameter(operator, argIndex);
      temp = Minir_checkTemporaryFixedRegister(minir, block, document, NULL, parameter);
      Minir_THROWIF(this, !temp,
          "Some non fixed-reg use required but not found!");
      PtrSeq_push2(Minir_TEMPARGUMENTS(minir), temp);
    }
  }
  // If O_xx type operation, set counter for operation for easier tracking 
  // between compilers.
  if(identity >= 0)
    IndexedFactory_COUNTER(Operation_Factory) = identity;
  // Fixing SAME parameters if any
  if (Operator_isConstrained (operator)) {
    uint8_t index = 0;
    const_PtrSeq to_update = Minir_TEMPRESULTS(minir);
    const_PtrSeq reference = Minir_TEMPARGUMENTS(minir);
    const_OperatorParameter (* Operator_getUpParameter)(Operator this, int writeIndex)
      = Operator_writeParameter;
    const_OperatorParameter (* Operator_getRefParameter)(Operator this, int writeIndex)
      = Operator_readParameter;
    uint8_t index_offset = resCount;
    do {
      PtrSeq_FOREACH(to_update, Temporary, temp) {
        if (! temp) {
          int same = OperatorParameter_same (
              Operator_getUpParameter(operator, index));
/*              index_offset == 0 ?*/
/*              Operator_readParameter(operator, index) :*/
/*              Operator_writeParameter(operator, index)*/
/*              );*/
          if (same != -1) {
            // Get the temporay pointed by SAME 
            temp = PtrSeq_access(reference, same - index_offset);
          } else {
            // Need to find other whose SAME point to current temp
            uint8_t refIndex = 0;
            PtrSeq_FOREACH(reference, Temporary, refTemp) {
              if ((resCount - index_offset) + index == 
                  OperatorParameter_same (
                    Operator_getRefParameter(operator, refIndex ))) {
                temp = refTemp;
                break;
              }
              refIndex ++;
            } PtrSeq_ENDEACH ;
          }
          Minir_THROWIF(this, !temp,
              "Cannot find reference for SAME parameter!");
          // Replace in results by the SAME temporary 
          {
            PtrSeqItem *base = *PtrSeq__BASE(to_update);
            base[index] = temp;
          }
        }
        index ++;
      } PtrSeq_ENDEACH ;
      //Change ref and update for next round 
      to_update = Minir_TEMPARGUMENTS(minir);
      reference = Minir_TEMPRESULTS(minir);
      Operator_getUpParameter = Operator_readParameter;
      Operator_getRefParameter = Operator_writeParameter;
      index = 0;
    } while ( index_offset != 0 && ! (index_offset = 0) );
  }
  PtrSeq_FOREACH(Minir_TEMPRESULTS(minir), Temporary, temp) {
    Except_THROWIF (!temp, "NULL value remains here !"); 
  } PtrSeq_ENDEACH ;
  Minir_THROWIF (this, resCount != PtrSeq_count (Minir_TEMPRESULTS (minir)),
      "%d definitions found but %d expected!",
      PtrSeq_count (Minir_TEMPRESULTS (minir)), resCount);
  PtrSeq_FOREACH(Minir_TEMPARGUMENTS(minir), Temporary, temp) {
    Except_THROWIF (!temp, "NULL value remains here !"); 
  } PtrSeq_ENDEACH ;
  Minir_THROWIF (this, argCount != PtrSeq_count (Minir_TEMPARGUMENTS (minir)),
      "%d uses found but %d expected!",
      PtrSeq_count (Minir_TEMPARGUMENTS (minir)), argCount);
  // Now, setup operation arguments (incl. modifiers) and results
  operation = Selector_makeOperation(selector,
      operator,
      resCount,
      /*modCount+*/argCount,
      (Temporary*)PtrSeq_base(Minir_TEMPRESULTS(minir)),
      (Temporary*)PtrSeq_base(Minir_TEMPARGUMENTS(minir)),
      clobberSet);
  // Restore factory counter for operations
  if(identity >= 0)
    IndexedFactory_COUNTER(Operation_Factory) =
      IndexedFactory_MAXCOUNT(Operation_Factory);
  if(flags) {
    OperationFlags f = 0;
    Yaml_FORSTRINGSEQ(document, flags, flag) {
      if(!strcmp(flag, "PureCopy")) {
        f |= OperationFlag_PureCopy;
      }
      else if(!strcmp(flag, "MemorySafe")) {
        f |= OperationFlag_MemorySafe;
      }
      else {
        Minir_WARNING(flags, "Unknown operation flag `%s'\n", flag);
      }
    } Yaml_ENDSTRINGMAP;
    Operation_setFlags(operation, f);
  }
  // Handle specials cases
  if(Operator_isPhi(operator)) {
    Selector_incrementPhiCount(selector);
  } else if(Operator_isCall(operator)) {
    Procedure_setTail(procedure, false);
  }
  // Under SSA, update the producer and/or list of consumers
  if(Minir_isSSA(minir)) {
    for(count=0; count<resCount; count++) {
      Temporary temp = PtrSeq_access(Minir_TEMPRESULTS(minir), count);
      if(Temporary_isVariable(temp)) {
        Variable_setOperationResult(
            Temporary_VARIABLE(temp),
            operation,
            count
            );
      }
    }
    for(count=0; count<argCount; count++) {
      Temporary temp = PtrSeq_access(Minir_TEMPARGUMENTS(minir), count);
      if(Temporary_isVariable(temp)) {
        Variable_setOperationArgument(
            Temporary_VARIABLE(temp),
            operation,
            count
            );
      }
    }
  }
  // Check target is correct
  if(target) {
    PtrSeq_FOREACH(Minir_TEMPARGUMENTS(minir), Temporary, arg) {
      if(Temporary_isLabel(arg)) {
        Literal literal = Temporary_LITERAL(arg);
        Label label = Literal_LABEL(literal);
        Minir_THROWIF(this, Label_block(target) != Label_block(label),
            "Labels `%s' and `%s' do not target the same BasicBlock!",
            Label_name(target), Label_name(label)
            );
        break;
      }
    } PtrSeq_ENDEACH;
  }
  // If adjust involving FP and SP, the frame has not a fixed size
  if(Operator_isAdjust(operator)) {
    Program program = Minir_program(minir);
    Convention convention = Program_convention(program);
    Temporary def = PtrSeq_access(Minir_TEMPRESULTS(minir), 0);
    Temporary use = PtrSeq_access(Minir_TEMPARGUMENTS(minir), 0);
    if(Temporary_hasRegister(def) && Temporary_hasRegister(use) &&
        Convention_isFrameRegister(convention, Temporary_register(def)) &&
        Convention_isStackRegister(convention, Temporary_register(use))) {
      Frame frame = Procedure_frame(procedure);
      Frame_setFixed(frame, false);
    }
  }
  
  // Mark symbols as stack-based if involved in SP computations
  {
    bool has_SP = false;
    bool has_FP = false;
    Symbol withSymbol = NULL;
    Program program = Minir_program(minir);
    Convention convention = Program_convention(program);
    PtrSeq_FOREACH(Minir_TEMPARGUMENTS(minir), Temporary, temp) {
      if(Temporary_isSymbol(temp)) {
        Except_CHECK(withSymbol == NULL); // two symbols is not allowed
        withSymbol = Literal_SYMBOL(Temporary_LITERAL(temp));
      }
      else if(Temporary_isDedicated(temp)) {
        if(Convention_isStackRegister(convention, Temporary_register(temp))) {
          has_SP = true;
        }
        else if(Convention_isFrameRegister(convention, Temporary_register(temp))) {
          has_FP = true;
        }
      }
    } PtrSeq_ENDEACH;
    if(withSymbol) {
      Frame frame = Procedure_frame(procedure);
      if(has_SP || (has_FP && !Frame_isFixed(frame))) {
        if(Symbol_class(withSymbol) == SymbolClass__UNDEF)
          Symbol_setClass(withSymbol, SymbolClass_VAR);
        if(Symbol_store(withSymbol) == SymbolStore__UNDEF)
          Symbol_setStore(withSymbol, SymbolStore_AUTO);
        if(Symbol_export(withSymbol) == SymbolExport__UNDEF)
          Symbol_setExport(withSymbol, SymbolExport_LOCAL);
        if(Symbol_size(withSymbol) == 0)
          Symbol_setSize(withSymbol, 4); //TODO: not hard-coded (fix open 64 !!)
        Except_CHECK(
            (
             Symbol_class(withSymbol) == SymbolClass_VAR &&
             Symbol_store(withSymbol) == SymbolStore_AUTO &&
             Symbol_export(withSymbol) == SymbolExport_LOCAL
            ) ||
            (
             Symbol_class(withSymbol) == SymbolClass_VAR &&
             Symbol_store(withSymbol) == SymbolStore_FORMAL &&
             Symbol_export(withSymbol) == SymbolExport_GLOBAL_PROTECTED
            ));
        if(!Frame_hasSymbol(frame, withSymbol)) {
          Frame_addSymbol(frame, withSymbol);
#ifdef FLO_DEBUG
          fprintf(stderr, "%s:%d added symbol %s to frame\n", __FILE__, __LINE__, Symbol_name(withSymbol));
#endif//FLO_DEBUG
        }
#ifdef FLO_DEBUG
        else {
          fprintf(stderr, "%s:%d symbol %s already in frame\n", __FILE__, __LINE__, Symbol_name(withSymbol));
        }
#endif//FLO_DEBUG
        // TODO: check is store or load?
        /*
           if(!Operator_isAddressAdd(operator)) {
             Minir_WARNING(this, "Symbol used in something else than an addresse add!");
        }*/
      }
      // Can happen with function calls for instance
      /*
      else {
        Minir_WARNING(this, "Symbol used in operation not involving SP");
      } */
    }
  }
        
  // Create edges of BasicBlock if operator is control
  if(!Minir_EXPLICITFLOW(minir)) {
    if(Operator_isControl(operator)) {
      if(target != NULL) {
        // Explicit control with target
        // Except_CHECK(!BasicBlock_hasSuccessor(block, bbTarget));
        Minir_THROWIF(this, BasicBlock_hasSuccessor(block, bbTarget),
            "Basic block %s is already a target of %s!",
            Label_name(BasicBlock_label(bbTarget)),
            Label_name(BasicBlock_label(block))
            );
/*        if(!BasicBlock_hasSuccessor(block, bbTarget))*/
          BasicBlock_makeEdge(block, bbTarget,
              frequency != -1.0 ?
              frequency :
              (bbFallthrough == NULL ?  // freq: 100% if unconditionnal jump
               1.0 :
               0.0)
              );
        if(bbFallthrough != NULL) {
          if(BasicBlock_hasSuccessor(block, bbFallthrough)) {
            // Should be the only case
            Except_CHECK(bbFallthrough == bbTarget);
            // Branch operation has same target and fallthrough. Need to create 
            // two edges (edge taken may be important, e.g., if target has phi)
          }
          BasicBlock_makeEdge(block, bbFallthrough,
              (frequency != -1.0) ?
              (1.0 - frequency) :
              0.0
              );
        }
      } else {
        // Implicit control like "call"
        if(!Operator_isReturn(operator)) {
          bbFallthrough = BasicBlock_next(block);
          // Except_CHECK(bbFallthrough != NULL);
          // Only if block is not the last, 
          if(bbFallthrough && !BasicBlock_hasSuccessor(block, bbFallthrough))
            BasicBlock_makeEdge(block, bbFallthrough,
                (frequency != -1.0) ?
                (1.0 - frequency) :
                0.0
                );
        }
      }
    }
  }
  //TODO: finish loopscope
  // create a new dep table or return existing one
  if(depNode != NULL) { 
    Minir_recordDependenceNode(minir, this, depNode, operation);
/*    LoopScope loopScope = BasicBlock_loopScope(block);*/
/*    DependenceTable dependenceTable;*/
/*    Minir_THROWIF(this, loopScope == NULL,*/
/*        "Operation is node but basic block does not have a loop scope!");*/
/*    dependenceTable = LoopScope_makeDependenceTable(loopScope);*/
/*    DependenceTable_enterNode(dependenceTable, operation);*/
  }
/*  Except_THROWIF(operator != Operation_operator(operation),*/
/*      "GOOD news: operator has changed from %d to %d!",*/
/*      operator, Operation_operator(operation));*/
  // Clean-up
  PtrSeq_empty(*Minir__TEMPRESULTS(minir));
  PtrSeq_empty(*Minir__TEMPARGUMENTS(minir));
//   Increment operation counter
  ++*Minir__OPERATIONCOUNTER(minir);
  return operation;
}


#ifdef $XCC__h
/**
 *   Check if a Yaml mapping corresponds to a loop.
 *   In practice, check if first element of mapping is the key "loop"
 */
bool
Minir_mappingIsLoop(yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

bool
Minir_mappingIsLoop(yaml_document_t *document, yaml_node_t *this)
{
  Yaml_FORSTRINGMAP(document, this, key, value) {
    return (!strcmp(key, "loop"));
  } Yaml_ENDSTRINGMAP;
  Minir_THROW(this, "No key in the mapping!");
}

#ifdef $XCC__h
/**
 *   Check if a Yaml mapping corresponds to a block.
 *   In practice, check if first element of mapping is the key "block"
 *   or "label"
 */
bool
Minir_mappingIsBlock(yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

bool
Minir_mappingIsBlock(yaml_document_t *document, yaml_node_t *this)
{
  Yaml_FORSTRINGMAP(document, this, key, value) {
    return (!strcmp(key, "block") || !strcmp(key, "label"));
  } Yaml_ENDSTRINGMAP;
  Minir_THROW(this, "No key in the mapping!");
}

#ifdef $XCC__h
/**
 *   Check if a Yaml mapping corresponds to a jump table.
 *   In practice, check if first element of mapping is the key "jump_table"
 *   or "label"
 */
bool
Minir_mappingIsJumpTable(yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

bool
Minir_mappingIsJumpTable(yaml_document_t *document, yaml_node_t *this)
{
  Yaml_FORSTRINGMAP(document, this, key, value) {
    return (!strcmp(key, "jump-table"));
  } Yaml_ENDSTRINGMAP;
  Minir_THROW(this, "No key in the mapping!");
}



#ifdef $XCC__h
/**
 *   Check if a Yaml mapping corresponds to an object.
 *   In practice, check if first element of mapping is the key "object"
 */
bool
Minir_mappingIsObject(yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

bool
Minir_mappingIsObject(yaml_document_t *document, yaml_node_t *this)
{
  Yaml_FORSTRINGMAP(document, this, key, value) {
    return (!strcmp(key, "object"));
  } Yaml_ENDSTRINGMAP;
  Minir_THROW(this, "No key in the mapping!");
}




#ifdef $XCC__h
/**
 *   Create a BasicBlock and setup label for it
 */
BasicBlock
Minir_newBasicBlock(Minir minir, Procedure procedure, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

BasicBlock
Minir_newBasicBlock(Minir minir, Procedure procedure, yaml_document_t *document, yaml_node_t *this)
{
  Program program = Procedure_program(procedure);
  LabelTable labelTable = Program_labelTable(program);
  BasicBlock block;
  //HTableValue htableValue;
  yaml_node_t *labelsNode = NULL;
  yaml_node_t *flagsNode = NULL;
  int32_t bbIndex;
  float freq = 0.0;
  char *name = NULL;
  void *pointer;
  Yaml_FORSTRINGMAP(document, this, key, value) {
    if(!strcmp(key, "label") || // Minir
        !strcmp(key, "block")) { // Tirex
      name = Yaml_GET_SCALAR_STRING(document, value);
      bbIndex = Minir_bbIndexOfName(minir, value, name);
      Minir_THROWIF(this, bbIndex < 0, "Basic block number < 0! (%d)", bbIndex);
    }
    else if(!strcmp(key, "ops") || // Minir
        !strcmp(key, "operations")) { // Tirex
      // Nothing to do in this function
      // Will be handled by Minir_parseBasicBlockOperations
    }
    else if(!strcmp(key, "succs") ||
        !strcmp(key, "successors") ||
        !strcmp(key, "preds") ||
        !strcmp(key, "predecessors")) {
      // Handled by Minir_connectBasicBlocks
    }
    else if(!strcmp(key, "freq") ||
            !strcmp(key, "frequency")) {
      freq = Yaml_GET_FLOAT(document, value);
    }
    else if(!strcmp(key, "labels")) {
      labelsNode = value;
    }
    else if(!strcmp(key, "loop")) {
      // Some loop information
      Minir_parseLoop(minir, procedure, document, value);
    }
    else if(!strcmp(key, "flags")) {
      flagsNode = value;
    } else {
      Minir_WARNING(value, "Unused key `%s' in basic block!\n", key);
    }
  } Yaml_ENDSTRINGMAP;
  Minir_THROWIF(this, name == NULL, "BasicBlock has no `label' or `block'!");
  // New BasicBlock
  block = Minir_makeBasicBlock(minir, bbIndex, name, freq);
#ifdef MINIR_DEBUG 
  fprintf(stderr, "New basic block labeled '%s'\n", name);
#endif
  // Other labels
  if(labelsNode) {
    Yaml_FORSEQ(document, labelsNode, node_label) {
      name = Yaml_GET_SCALAR_STRING(document, node_label);
#ifdef MINIR_DEBUG 
      printf("New label for BasicBlock: %s\n", name);
#endif
      Minir_addBasicBlockLabel(minir, block, name);
    } Yaml_ENDSEQ;
  }
  if(flagsNode) {
    BasicBlockFlags flags = 0;
    Yaml_FORSTRINGSEQ(document, flagsNode, flag) {
      if(!strcmp(flag, "Entry")) {
        flags |= BasicBlockFlag_Entry;
      }
      else if(!strcmp(flag, "Exit")) {
        flags |= BasicBlockFlag_Exit;
      }
      else {
        Minir_WARNING(flagsNode, "Unused flag `%s' in basic block!\n", flag);
      }
    } Yaml_ENDSTRINGSEQ;
    BasicBlock_setFlags(block, flags);
  }
  return block;
}

#ifdef $XCC__h
/**
 *   Create a Loop scope
 */
LoopScope
Minir_parseLoop(Minir minir, Procedure procedure, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

LoopScope
Minir_parseLoop(Minir minir, Procedure procedure, yaml_document_t *document, yaml_node_t *this)
{
  Configure configure;
  LoopScope loopScope = NULL;
  DependenceTable depTable = NULL;
  BasicBlock header;
  char *name;
  int32_t loopIndex;
  yaml_node_t *node = Yaml_mapping_lookup_key(document, this, "loop");
  Minir_THROWIF(this, !node, "`loop' mapping key required for loop information");
  name = Yaml_GET_SCALAR_STRING(document, node);
  Minir_THROWIF(this, name[0] != 'L' || name[1] != '_',
      "`loop' value should start with `L_' but is `%s'", name);
  loopIndex = int_of_string(name+2);
  node = Yaml_mapping_lookup_key(document, this, "header");
  Minir_THROWIF(this, !node, "`header' mapping key required for loop information");
  header = Minir_findBasicBlockByName(minir, 
      node,
      Yaml_GET_SCALAR_STRING(document, node));
  Minir_THROWIF(node, !header, "No such basic block!");
  loopScope = BasicBlock_loopScope(header);
  Minir_THROWIF(this, loopScope, "There is already a loopScope for this block!");
  // Create now loopScope for basic block
  configure = Program_configure(Procedure_program(procedure)); //FIXME: really?
  {
    IndexedFactory_COUNTER(LoopScope_Factory) = loopIndex;
    loopScope = Procedure_makeLoopScope(procedure, header, configure);
    IndexedFactory_COUNTER(LoopScope_Factory) =
      IndexedFactory_MAXCOUNT(LoopScope_Factory);
  }
  BasicBlock_setLoopScope(header, loopScope);
  depTable = LoopScope_makeDependenceTable(loopScope);
  Yaml_FORSTRINGMAP(document, this, key, value) {
    if(!strcmp(key, "header") || !strcmp(key, "loop")) {
      // Nothing left to do
    }
    else if(!strcmp(key, "trip count")) {
      Temporary tripCount = Minir_parseTemporary(minir, header, document, value, NULL);
      LoopScope_setTripCount(loopScope, tripCount);
      // FIXME (from Interface_makeLoopScope)! In case of SSA.
    }
    else if(!strcmp(key, "body")) {
      BasicBlock block;
      Yaml_FORSTRINGSEQ(document, value, name) {
        if(name[0] == 'L') { // Loop
          LoopScope inLoopScope = Minir_getLoopScope(minir, value, name);
          if(inLoopScope) {
            LoopScope_update(inLoopScope, loopScope,
                LoopScope_blockCount(inLoopScope),
                LoopScope_isSimple(inLoopScope),
                LoopScope_isInner(inLoopScope)
                );
          }
        } else {
          block = Minir_findBasicBlockByName(minir, value, name);
          BasicBlock_setLoopScope(block, loopScope);
          //TODO: also change the BlockCount of the LoopScope?
        }
      } Yaml_ENDSTRINGSEQ;
    }
    else if(!strcmp(key, "parent")) {
      LoopScope parentLoopScope = Minir_getLoopScope(minir, value, name);
      if(parentLoopScope) {
        LoopScope_update(loopScope, parentLoopScope,
            LoopScope_blockCount(loopScope),
            LoopScope_isSimple(loopScope),
            LoopScope_isInner(loopScope)
            );
      }
    }
    else if(!strcmp(key, "nodes")) {
      Operation op;
      DependenceTable table = LoopScope_dependenceTable(loopScope);
      Yaml_FORSTRINGSEQ(document, value, name) {
        op = Minir_getDependenceNode(minir, value, name);
        Except_CHECK(op);
        DependenceTable_enterNode(table, op);
      } Yaml_ENDSTRINGSEQ;
    }
    else if(!strcmp(key, "arcs")) {
      DependenceTable table = LoopScope_dependenceTable(loopScope);
      Operation source, target;
      DependenceKind kind;
      int latency=0; // TODO: get from MDS
      int omega;
      Yaml_FORSEQ(document, value, arc) {
        source = NULL;
        target = NULL;
        kind = -1;
        latency= 1; // TODO: get from MDS
        omega = -1;
        Yaml_FORSTRINGSEQ(document, arc, name) { // implicit def of 'node'
          if(name[0] == 'N' && name[1] == '_') {
            if(!source)
              source = Minir_getDependenceNode(minir, arc, name);
            else if(!target)
              target = Minir_getDependenceNode(minir, value, name);
            else {
              Minir_WARNING(node, "Does not know what to do with `%s'! (%s:%d)", name, __FILE__, __LINE__);
            }
          }
          else {
            if(omega == -1) {
              omega = int_of_string(name);
              if(errno != 0)
                omega = -1;
            } else {
              errno = 1;
            }
            if(errno != 0) {
              // Try to get dep kind
              kind = DependenceKind_ofName(name);
              if(kind == DependenceKind__) {
                kind = -1;
                Minir_WARNING(node, "Does not know what to do with `%s'!", name);
              }
            }
          }
        } Yaml_ENDSTRINGSEQ;
        {
          if(omega == -1) {
            Minir_WARNING(arc, "Dependence omega must be supplied");
          }
          if(latency == -1) {
            Minir_WARNING(arc, "Dependence latency must be supplied");
          }
          if(kind == -1) {
            Minir_WARNING(arc, "Dependence kind must be supplied");
          }
          Minir_THROWIF(arc, source == NULL, "Source of dependence arc is not provided!");
          Minir_THROWIF(arc, target == NULL, "Target of dependence arc is not provided!");
        }
        DependenceTable_makeArc(table, source, target, latency, omega, kind);
      } Yaml_ENDSEQ;
    }
    else if(!strcmp(key, "flags")) {
      Yaml_FORSTRINGSEQ(document, value, flag) {
        if(!strcmp(flag, "Inner")) {
          LoopScope_setFlags(loopScope, LoopScopeFlag_Inner);
        }
        else if(!strcmp(flag, "Simple")) {
          LoopScope_setFlags(loopScope, LoopScopeFlag_Simple);
        }
        else if(!strcmp(flag, "Hardware")) {
          LoopScope_setFlags(loopScope, LoopScopeFlag_Hardware);
        }
        else if(!strcmp(flag, "Remainder")) {
          LoopScope_setFlags(loopScope, LoopScopeFlag_Remainder);
        }
        else {
          Minir_WARNING(value, "Unused loop flag `%s'!", key);
        }
      } Yaml_ENDSTRINGSEQ;
    }
    else {
      Minir_WARNING(value, "Unused key `%s'!", key);
    }
  } Yaml_ENDSTRINGMAP;
  return loopScope;
}

#ifdef $XCC__h
/**
 *   Create a Jump Table
 */
void
Minir_parseJumpTable(Minir minir, Procedure procedure, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

void
Minir_parseJumpTable(Minir minir, Procedure procedure, yaml_document_t *document, yaml_node_t *this)
{
  DataList dataList = Procedure_dataList(procedure);
  Data data = Minir_parseData(minir, document, this);
  DataList_push2(dataList, data);
}




#ifdef $XCC__h
/**
 *   Parse a basic block in Minir format.
 */
void
Minir_parseBasicBlockOperations(Minir minir, Procedure procedure, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

void 
Minir_parseBasicBlockOperations(Minir minir, Procedure procedure, yaml_document_t *document, yaml_node_t *this)
{
  yaml_node_t *node;
  char *name;
  long int bbIndex;
  BasicBlock block;
  node = Yaml_mapping_lookup_key(document, this, "label");
  if(!node) node = Yaml_mapping_lookup_key(document, this, "block");
  name = Yaml_GET_SCALAR_STRING(document, node);
  bbIndex = Minir_bbIndexOfName(minir, node, name);
  block = Minir_findBasicBlock(minir, bbIndex);
#ifdef MINIR_DEBUG 
  printf("Parsing operations for BasicBlock %s:\n", name);
#endif
  node = Yaml_mapping_lookup_key(document, this, "ops");
  if(!node) node = Yaml_mapping_lookup_key(document, this, "operations");
  if(node) {
    bool must_be_last = false;
    Operation operation;
    // Check that if operation is control, it is the last of the basic block
    Yaml_FORSEQ(document, node, op) {
      Minir_THROWIF(op, must_be_last,
          "Previous operator `%s' is Control but is not the last of the basic block!",
          Operator_shortName(Operation_operator(operation))
          );
      operation = Minir_parseOperation(minir, bbIndex, document, op);
      must_be_last = Operation_isControl(operation);
    } Yaml_ENDSEQ ;
    // Connect to fall-through if necessary.
    // If last operation is a branch, then the fall-through edge has already been 
    // added if necessary. So we add a fall-through edge whenever the last 
    // operation is not a changing flow operation.
    if(!Minir_EXPLICITFLOW(minir)) {
      Operation last_operation = BasicBlock_lastOperation(block);
      if (last_operation == NULL ||
          !Operation_isControl(last_operation)
         ) {
        BasicBlock fallthru = BasicBlock_next(block);
        if(fallthru == NULL) {
          if(!Minir_ISTEST(minir)) {
            Minir_THROWIF(this, fallthru == NULL,
                "No fall-through basic-block available for %s! ('return' forgotten ?)", name);
          }
        } else {
          // printf("Last operation has a fall-through to next BasicBlock\n");
          BasicBlock_makeEdge(block, fallthru, 1.0); //Frequency is always 100%
        }
      }
    }
    // One more check if under SSA
    if(BasicBlock_phiCount(block) > 0) {
      int enterCount, numSeq;
      BasicBlockEdge edge;
      yaml_node_t *preds = Yaml_mapping_lookup_key(document, this, "preds");
      if(!preds) preds = Yaml_mapping_lookup_key(document, this, "predecessors");
      Minir_THROWIF(this, !preds,
          "Basic block `%s' contains phi-functions but has no `preds' field!", name);
      enterCount = BasicBlock_enterCount(block);
      numSeq = Yaml_NUMSEQ(document, preds);
      Minir_THROWIF(preds, enterCount != numSeq,
          "Basic block `%s' has %d enter edges but `preds' field contains %d items!",
          name, enterCount, numSeq);
      edge = BasicBlock_enterFirst(block);
      Yaml_FORSTRINGSEQ(document, preds, bname) {
        BasicBlock predBlock = Minir_findBasicBlockByName(minir, preds, bname);
        BasicBlock origBlock = BasicBlockEdge_origBlock(edge);
        Except_CHECK(BasicBlockEdge_destBlock(edge) == block);
        Minir_THROWIF(preds, predBlock != origBlock,
            "Error in preds block order! Was expecting `%s' but found instead `%s' (%s)! phi-functions will be broken!",
            Label_name(BasicBlock_label(origBlock)),
            Label_name(BasicBlock_label(predBlock)),
            bname);
        edge = BasicBlockEdge_ENTERNEXT(edge);
      } Yaml_ENDSTRINGSEQ;
      Except_CHECK(edge == NULL);
    }
  }
}

#ifdef $XCC__h
/**
 * Connects the basic blocks together by control-flow edges using the @c preds 
 * or @c succs Yaml keys.
 */
void
Minir_connectBasicBlocks(Minir minir, void *procedure, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

void 
Minir_connectBasicBlocks(Minir minir, void *procedure, yaml_document_t *document, yaml_node_t *this)
{
  yaml_node_t *node;
  char *name;
  long int bbIndex;
  float frequency;
  bool useConnect = false;
  bool hasPredsOrSuccs = false;
  BasicBlock block;
  node = Yaml_mapping_lookup_key(document, this, "label");
  if(!node) node = Yaml_mapping_lookup_key(document, this, "block");
  name = Yaml_GET_SCALAR_STRING(document, node);
  bbIndex = Minir_bbIndexOfName(minir, node, name);
  block = Minir_findBasicBlock(minir, bbIndex);
#ifdef MINIR_DEBUG 
  printf("Connecting BasicBlock %s.\n", name);
#endif
  node = Yaml_mapping_lookup_key(document, this, "succs");
  if(!node) node = Yaml_mapping_lookup_key(document, this, "successors");
  if(node) {
    BasicBlock target;
    char *tname;
    useConnect = true;
    Minir_EXPLICITFLOW(minir) = true;
    hasPredsOrSuccs = true;
    Yaml_FORSEQ(document, node, bb) {
      if(Yaml_IS_MAPPING(bb)) {
        Yaml_FORSTRINGMAP(document, bb, the_name, freq_node) {
          tname = the_name;
          frequency = Yaml_GET_FLOAT(document, freq_node);
        } Yaml_ENDSTRINGMAP;
      } else {
        tname = Yaml_GET_SCALAR_STRING(document, bb);
        frequency = 0.0;
      }
      bbIndex = Minir_bbIndexOfName(minir, bb, tname);
      target = Minir_findBasicBlock(minir, bbIndex);
      Minir_THROWIF(this, !target,
          "Non-existent basic block `%s' in succs of `%s'!", tname, name);
      if(!BasicBlock_hasSuccessor(block, target)) {
        BasicBlock_makeEdge(block, target, frequency);
      }
      else {
        if(Yaml_IS_MAPPING(bb)) {
          BasicBlock_setFrequency(block, frequency);
        }
      }
    } Yaml_ENDSEQ ;
  }
  node = Yaml_mapping_lookup_key(document, this, "preds");
  if(!node) node = Yaml_mapping_lookup_key(document, this, "predecessors");
  if(node) {
    BasicBlock source;
    char *sname;
    useConnect = true;
    Minir_EXPLICITFLOW(minir) = true;
    hasPredsOrSuccs = true;
    Yaml_FORSEQ(document, node, bb) {
      sname = Yaml_GET_SCALAR_STRING(document, bb);
      bbIndex = Minir_bbIndexOfName(minir, bb, sname);
      source = Minir_findBasicBlock(minir, bbIndex);
      Minir_THROWIF(this, !source,
          "Non-existent basic block `%s' in preds of `%s'!", sname, name);
      if(!BasicBlock_hasSuccessor(source, block))
        BasicBlock_makeEdge(source, block, 0.0);
    } Yaml_ENDSEQ ;
  }
 
  Minir_THROWIF(this, !hasPredsOrSuccs && Minir_isSSA(minir),
    "`succs' or `pred' keys for BasicBlock %s required under SSA!\n", name);
}

/**
 * Parse one data initialization with integers.
 */
#define Minir_parseOneDataInitInteger(minir, document, data, node, Type, dataType, initSize) { \
  Type value; \
  if(Yaml_IS_SEQUENCE(node)) { \
    Yaml_FORSEQ(document, node, elt) { \
      if(Yaml_IS_SEQUENCE(elt)) {  /* symbol */  \
        Minir_THROWIF(elt, dataType != DataInitType_word && dataType != DataInitType_quad, \
            "Symbols must be given in `word' or `quad' list!"); \
        Temporary temporary = Minir_parseSymbolic(minir, document, elt); \
        Data_addInitializer(data, DataInitType_symbol, NULL, temporary, 0); \
      } else { \
        value = Yaml_GET_INTEGER(document, elt); \
                Data_addInitializer(data, dataType, &value, NULL, sizeof(value)); \
      } \
      initSize += sizeof(value); \
    } Yaml_ENDSEQ; \
  } else { \
    value = Yaml_GET_INTEGER(document, node); \
    Data_addInitializer(data, dataType, &value, NULL, sizeof(value)); \
    initSize += sizeof(value); \
  } \
}


/**
 * Parse one data initialization with floats.
 */
#define Minir_parseOneDataInitFloat(minir, document, data, node, Type, dataType, initSize) { \
  Type value; \
  if(Yaml_IS_SEQUENCE(node)) { \
    Yaml_FORSEQ(document, node, elt) { \
      if(Yaml_IS_SEQUENCE(elt)) {  /* symbol */  \
        Minir_THROWIF(elt, dataType != DataInitType_word && dataType != DataInitType_quad, \
            "Symbols must be given in `word' or `quad' list!"); \
        Temporary temporary = Minir_parseSymbolic(minir, document, elt); \
        Data_addInitializer(data, DataInitType_symbol, NULL, temporary, 0); \
      } else { \
        value = Yaml_GET_FLOAT(document, elt); \
                Data_addInitializer(data, dataType, &value, NULL, sizeof(value)); \
      } \
      initSize += sizeof(value); \
    } Yaml_ENDSEQ; \
  } else { \
    value = Yaml_GET_FLOAT(document, node); \
    Data_addInitializer(data, dataType, &value, NULL, sizeof(value)); \
    initSize += sizeof(value); \
  } \
}
 
 
#ifdef $XCC__h
/**
 * Parse data initialization.
 */
void
Minir_parseDataInit(Minir minir, yaml_document_t *document, Data data, yaml_node_t *initNode);
#endif//$XCC__h

void
Minir_parseDataInit(Minir minir, yaml_document_t *document, Data data, yaml_node_t *initNode)
{
  size_t initSize = 0;
  Yaml_FORSEQ(document, initNode, seqnode) {
    Yaml_FORSTRINGMAP(document, seqnode, key, node) {
      // Sequences now used for multiple init of same type.
      // Nested sequences are used for symbols
/*      if(Yaml_IS_SEQUENCE(node)) {*/
/*        if(!strcmp(key, "word")) {*/
/*          Temporary temporary = Minir_parseSymbolic(minir, document, node);*/
/*          Data_addInitializer(data, DataInitType_symbol, NULL, temporary, 0);*/
/*          initSize += 4;*/
/*          continue;*/
/*        }*/
/*        else if(!strcmp(key, "quad")) {*/
/*          Temporary temporary = Minir_parseSymbolic(minir, document, node);*/
/*          Data_addInitializer(data, DataInitType_symbol, NULL, temporary, 0);*/
/*          initSize += 8;*/
/*        }*/
/*        // Else: array is a list of values*/
/*      }*/
      if(!strcmp(key, "ascii") ||
         !strcmp(key, "asciz") ||
         !strcmp(key, "string")) {
        char *value = Yaml_GET_SCALAR_STRING(document, node);
        size_t len  = Yaml_GET_SCALAR_STRING_LENGTH(document, node);
        // No need do add trailing zero
        if(!strcmp(key, "ascii") ||
            (!strcmp(key, "string") &&
             value[len-1] == '\0')) {
          Data_addInitializer(data, DataInitType_ascii, value, NULL, len);
        } else {
          // asciz or string non null-terminated
          len += 1; // string + NULL
          DataInitializer dinit = Data_addInitializer(data, DataInitType_ascii, value, NULL, len);
          (*DataInitializer__INIT(dinit))[len-1] = 0; // NULL terminator
        }
        initSize += len;
      } else if(!strcmp(key, "byte") ||
          !strcmp(key, "s8")   ||
          !strcmp(key, "u8")) {
        Minir_parseOneDataInitInteger(minir, document, data, node, int8_t, DataInitType_byte, initSize);
      } else if(!strcmp(key, "half") ||
          !strcmp(key, "hword")||
          !strcmp(key, "short")||
          !strcmp(key, "s16")  ||
          !strcmp(key, "u16")) {
        Minir_parseOneDataInitInteger(minir, document, data, node, int16_t, DataInitType_hword, initSize);
      } else if(!strcmp(key, "word")||
          !strcmp(key, "int") ||
          !strcmp(key, "long")||
          !strcmp(key, "s32") ||
          !strcmp(key, "u32")) {
        Minir_parseOneDataInitInteger(minir, document, data, node, int32_t, DataInitType_word, initSize);
/*        int32_t value = Yaml_GET_INTEGER(document, node);*/
/*        Data_addInitializer(data, DataInitType_word, &value, NULL, sizeof(value));*/
/*        initSize += sizeof(value);*/
      } else if(!strcmp(key, "xword")||
          !strcmp(key, "quad")  ||
          !strcmp(key, "dword") ||  /* Tirex */
          !strcmp(key, "s64")  ||
          !strcmp(key, "u64") ) {
        Minir_parseOneDataInitInteger(minir, document, data, node, int64_t, DataInitType_quad, initSize);
      } else if(!strcmp(key, "s128")||
          !strcmp(key, "u128")) {
        Minir_THROW(node, "Not supporting 128 bit long integers!");
      } else if(!strcmp(key, "float")||
          !strcmp(key, "f32")) {
        Minir_parseOneDataInitFloat(minir, document, data, node, float, DataInitType_word, initSize);
/*#ifdef MINIR_DEBUG*/
/*        fprintf(stderr, "Read float %f (%a)\n", value, value);*/
/*        fprintf(stderr, "Hexa float %x (%u)\n", *(int*)&value, *(int*)&value);*/
/*#endif*/
      } else if(!strcmp(key, "double")||
          !strcmp(key, "f64")) {
        Minir_parseOneDataInitFloat(minir, document, data, node, double, DataInitType_quad, initSize);
/*#ifdef MINIR_DEBUG*/
/*        fprintf(stderr, "Read double %f (%a)\n", value, value);*/
/*        fprintf(stderr, "Hexa double %llx (%llu)\n", *(long long int*)&value, *(long long int*)&value);*/
/*#endif*/
      } else if(key[0] == 'f') {
        Minir_THROW(node, "Not supporting this kind of floats!");
      } else if(key[0] == 'c') {
        Minir_THROW(node, "Not supporting complex numbers!");
      } else if(!strcmp(key, "space")) {
        int32_t space = Yaml_GET_INTEGER(document, node);
        Data_addInitializer(data, DataInitType_space, &space, NULL, sizeof(space));
        initSize += space;
      } else {
        Minir_THROW(node, "Don't know how to use `%s' for initialization of `%s'!", key, Symbol_name(Data_symbol(data)));
      }
    } Yaml_ENDSTRINGMAP;
  } Yaml_ENDSEQ;
  Minir_THROWIF(initNode, Data_dataSize(data)!= initSize,
      "Size of object is %zu bytes but init provides %zu bytes!",
      Data_dataSize(data), initSize
      );
}


#ifdef $XCC__h
/**
 * Parse some data in a data section.
 */
void*
Minir_parseData(Minir minir, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

void*
Minir_parseData(Minir minir, yaml_document_t *document, yaml_node_t *this)
{
  yaml_node_t *init = NULL;
  Data data;
  Symbol symbol = NULL;
  size_t dataSize = -1;
  uint8_t align = 0;
  int32_t orig = -1; // offset of data in section
  DataSection section = NULL;
  Program program = Minir_PROGRAM(minir);
  DataList dlist = Program_dataList(program);
  SymbolStore store = SymbolStore_FSTATIC; // default store
  SymbolExport export = SymbolExport_GLOBAL_PROTECTED; // default export
  Yaml_FORSTRINGMAP(document, this, key, node) {
    // Reading symbol name
    if(!strcmp(key, "label") ||  // Minir
       (!strcmp(key, "common") && (store = SymbolStore_COMMON)) ||  // Tirex
       !strcmp(key, "object")||  // Tirex
       (!strcmp(key, "formal") && (store = SymbolStore_FORMAL)) ||  // Tirex
       (!strcmp(key, "auto") &&
          (store = SymbolStore_AUTO) &&
          (export = SymbolExport_LOCAL)) ||  // Tirex
       !strcmp(key, "global") ||             // Tirex
       !strcmp(key, "jump-table"))
    {
      char *label = Yaml_GET_SCALAR_STRING(document, node);
      Program program = Minir_PROGRAM(minir);
      SymbolTable symbolTable = Program_symbolTable(program);
      // check first if the symbol wasn't already added during program parsing
      if ( (symbol = SymbolTable_searchName(symbolTable, label)) == NULL)
        symbol = SymbolTable_makeSymbol(symbolTable, label);
      // By default set class symbol as variable
      Symbol_setClass(symbol, SymbolClass_VAR);
      Symbol_setExport(symbol, export);
      Symbol_setStore(symbol, store);
    }
    else if(!strcmp(key, "object")) { // Tirex: juste states this is an object
    }
    else if(!strcmp(key, "align")) {
      align = Yaml_GET_INTEGER(document, node);
    }
    else if(!strcmp(key, "origin")) {
      orig = Yaml_GET_INTEGER(document, node);
    }
    else if(!strcmp(key, "size")) {
      dataSize = Yaml_GET_INTEGER(document, node);
    }
    else if(!strcmp(key, "section")) {
      const char *secName;
      if (Yaml_IS_MAPPING (node)) { /* create a section */
        secName = Minir_parseSection (minir, document, node);
      } else {
        secName = Yaml_GET_SCALAR_STRING(document, node);
      }
      if(!strncmp(secName, ".bss", 4)) /* section name starts with ".bss" */
        Symbol_setStore(symbol, SymbolStore_UGLOBAL);
      section = Program_searchSection(program, secName);
      Minir_THROWIF(node, section == NULL,
          "Cannot find section `%s' in program!", secName);
    }
    else if(!strcmp(key, "attr")) {
      Minir_parseSymAttributes(document, node, symbol);
    }
    else if(!strcmp(key, "linkage")) {
      char* linkType = Yaml_GET_SCALAR_STRING(document, node);
      if(!strcmp(linkType, "global")) {
        Symbol_setStore(symbol, SymbolStore_DGLOBAL);
      }
      else {
        Symbol_setStore(symbol, SymbolStore_FSTATIC);
        Minir_WARNING(node, "Unsupported linkage type `%s'!\n", linkType);
      }
    }
    else if(!strcmp(key, "export")) {
      char* expType = Yaml_GET_SCALAR_STRING(document, node);
      if(!strcmp(expType, "local")) {
        Symbol_setExport(symbol, SymbolExport_LOCAL);
      }
      else {
        Symbol_setExport(symbol, SymbolExport__UNDEF);
        Minir_WARNING(node, "Unsupported export type `%s'!\n", expType);
      }
    }
    else if(!strcmp(key, "init") ||
            !strcmp(key, "table")) {
      init = node;
    }
    else {
      Minir_WARNING(node, "Warning: unused key `%s' in object for data %s!\n",
          key, symbol != NULL ? Symbol_name(symbol) : "");
    }
  } Yaml_ENDSTRINGMAP;
  Minir_THROWIF(this, symbol == NULL, "`label' or `common' required for data entry!");
  Minir_THROWIF(this, dataSize == -1, "Data `size' required for data entry!");
  //Minir_THROWIF(this, align == -1,    "Alignment required for data entry!");
  Minir_THROWIF(this,
      section == NULL &&
      Symbol_store(symbol) != SymbolStore_AUTO &&
      Symbol_store(symbol) != SymbolStore_FORMAL &&
      Symbol_store(symbol) != SymbolStore_COMMON,
      "`section' required for this data entry!");
  if(Symbol_store(symbol) == SymbolStore_AUTO ||
     Symbol_store(symbol) == SymbolStore_FORMAL) {
    // No need to create actual data, only the symbol was important
    Minir_THROWIF(this, init != NULL,
        "Data initialization not allowed for formal parameters!");
    Symbol_setSize(symbol, dataSize);
    return symbol;
  }
  data = Data_new(Program_MEMORY(program), symbol, dataSize, align, section);
  if(orig != -1) Data_setOrigin(data, orig);
  // Initialize data if required
  if(init) {
    Minir_parseDataInit(minir, document, data, init);
  }
  return data;
}


#ifdef $XCC__h
/**
 * Parse a section for a program.
 * @return the name of the section.
 */
const char *
Minir_parseSection(Minir minir, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

const char *
Minir_parseSection(Minir minir, yaml_document_t *document, yaml_node_t *this)
{
  char* name=NULL;
  uint8_t align = 0;
  DataSectionFlags flags = 0;
  DataSectionType sectionType = DataSectionType_UNDEF;
  uint8_t entsize = 0;
  Yaml_FORSTRINGMAP(document, this, key, value) {
    if(!strcmp(key, "section")) {
      name = Yaml_GET_SCALAR_STRING(document, value);
      Minir_THROWIF(value, strlen(name) >= MAX_SECTION_NAME_LEN,
          "Section names are restricted to %d characters!", MAX_SECTION_NAME_LEN-1);
    }
    else if(!strcmp(key, "align")) {
      align = Yaml_GET_INTEGER(document, value);
      Minir_THROWIF(value, align < 0, "Alignment must be positive!");
    }
    else if(!strcmp(key, "flags")) {
      flags = 0;
      Yaml_FORSTRINGSEQ(document, value, flag) {
        if(!strcmp(flag, "Alloc")) {
          flags |= DataSectionFlag_Alloc;
        }
        else if(!strcmp(flag, "Exec")) {
          flags |= DataSectionFlag_Exec;
        }
        else if(!strcmp(flag, "Write")) {
          flags |= DataSectionFlag_Write;
        }
        else if(!strcmp(flag, "Small")) {
          flags |= DataSectionFlag_Small;
        }
        else if(!strcmp(flag, "Merge")) {
          flags |= DataSectionFlag_Merge;
        }
        else if(!strcmp(flag, "Strings")) {
          flags |= DataSectionFlag_Strings;
        }
        else if(!strcmp(flag, "TLS")) {
          flags |= DataSectionFlag_TLS;
        }
        else if(!strcmp(flag, "Group")) {
          flags |= DataSectionFlag_Group;
          Minir_THROW(value, "Not able to handle group sections for now!");
        }
        else {
          Minir_WARNING(value, "unused section flag `%s'", flag);
        }
      } Yaml_ENDSTRINGSEQ;
    }
    else if(!strcmp(key, "type")) {
      const char *type = Yaml_GET_SCALAR_STRING (document, value);
      if (!strcmp (type, "progbits"))
        sectionType = DataSectionType_progbits;
      else if (!strcmp (type, "nobits"))
        sectionType = DataSectionType_nobits;
      else if (!strcmp (type, "note"))
        sectionType = DataSectionType_note;
      else if (!strcmp (type, "init_array"))
        sectionType = DataSectionType_init_array;
      else if (!strcmp (type, "fini_array"))
        sectionType = DataSectionType_fini_array;
      else if (!strcmp (type, "preinit_array"))
        sectionType = DataSectionType_preinit_array;
    }
    else if(!strcmp(key, "entsize")) {
      entsize = Yaml_GET_INTEGER (document, value);
    }
    else {
      Minir_WARNING(this, "unused key `%s' in %s", key, __FUNCTION__);
      node_pretty(document, value);
    }
  } Yaml_ENDSTRINGMAP;
  Minir_THROWIF(this, name == NULL, "`section' name required for section!");
  // Minir_THROWIF(this, align == 0, "`align' != 0 required for section!");
  Minir_THROWIF(this, 
      flags & DataSectionFlag_Merge && entsize == 0,
      "`entsize' required for mergeable sections");
  Program_makeSection (Minir_program(minir), name, align, flags, sectionType, entsize);
  return name;
}


#ifdef $XCC__h
/**
 * Create a symbol for a function.
 */
Symbol
Minir_makeFunctionSymbol(Minir minir, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

Symbol
Minir_makeFunctionSymbol(Minir minir, yaml_document_t *document, yaml_node_t *this)
{
  Symbol symbol;
  char *string;
  Program program = Minir_PROGRAM(minir);
  SymbolTable symbolTable = Program_symbolTable(program);
  yaml_node_t *node = Yaml_mapping_lookup_key(document, this, "label");
  if(!node) node = Yaml_mapping_lookup_key(document, this, "function");
  Minir_THROWIF(this, !node, "`label' or `function' key not found in function mapping!");
  string = Yaml_GET_SCALAR_STRING(document, node);
#ifdef MINIR_DEBUG 
  printf("Function label: %s\n", string);
#endif
  symbol = SymbolTable_makeSymbol(symbolTable, string);
  Symbol_setClasses(symbol,
      SymbolClass_FUNC,
      SymbolStore_TEXT,
      SymbolExport_LOCAL /* TODO: check default export, may depend on arch (e.g., systemV maybe preemptible (i.e., can be replaced by dynamic library), on embedded, maybe better not preemptible) */
      );
  return symbol;
}

#ifdef $XCC__h
/**
 * Parse a function (procedure) in Minir format.
 */
Procedure
Minir_parseFunction(Minir minir, Symbol symbol, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

Procedure
Minir_parseFunction(Minir minir, Symbol symbol, yaml_document_t *document, yaml_node_t *this)
{
  Program program = Minir_PROGRAM(minir);
  Procedure procedure = Program_makeProcedure(program, symbol);
  BasicBlock pred = NULL;
  yaml_node_t *basic_blocks = NULL;
  yaml_node_t *entries_node = NULL;
  yaml_node_t *exits_node = NULL;
#ifdef MINIR_DEBUG 
  printf("Parsing next function\n"); // node_pretty(document, this);
#endif
  Minir_PROCEDURE(minir) = procedure;
  Yaml_FORSTRINGMAP(document, this, key, value) {
    if(!strcmp(key, "label") || !strcmp(key, "function")) {
      char* label = Yaml_GET_SCALAR_STRING(document, value);
      Except_CHECK(!strcmp(label, Symbol_name(symbol)));
    }
    else if(!strcmp(key, "test")) {
      // Input for a self test, print the string
      printf("\n%s\n", Yaml_GET_SCALAR_STRING(document, value));
      Minir_ISTEST(minir) = true;
    }
    else if(!strcmp(key, "bbs") || !strcmp(key, "body") ||
            !strcmp(key, "blocks")) {
      basic_blocks = value;
    }
    else if(!strcmp(key, "entries")) {
      entries_node = value;
    }
    else if(!strcmp(key, "exits")) {
      exits_node = value;
    }
    else if(!strcmp(key, "frame size")) {
      Minir_THROW(value, "Cannot use `frame size' anymore!");
/*      Frame frame = Procedure_frame(procedure);*/
/*      frame_size = Yaml_GET_INTEGER(document, value);*/
/*      Frame_setSize(frame, frame_size);*/
    }
    else if(!strcmp(key, "fixed frame")) {
      bool fixed = Yaml_GET_BOOL(document, value);
      Frame frame = Procedure_frame(procedure);
      Frame_setFixed(frame, fixed);
    }
    // Formal parameters when they do not fit in registers
    else if(!strcmp(key, "frame")) {
      char* key;
      Symbol symbol;
      Frame frame = Procedure_frame(procedure);
      Yaml_FORSEQ(document, value, node) {
        symbol = Minir_parseData(minir, document, node);
        switch(Symbol_store(symbol)) {
          case SymbolStore_AUTO: Frame_addSymbol(frame, symbol); break;
          case SymbolStore_FORMAL: Frame_addFormal(frame, symbol); break;
          default: Minir_THROW(node,
            "This symbol store is not allowed in `frame' (%s)!",
            SymbolStore_Id(Symbol_store(symbol)));
        }
      } Yaml_ENDSEQ;
    }
    else if(!strcmp(key, "objects") || !strcmp(key, "data")) {
      DataList dataList = Procedure_dataList(procedure);
      Data data;
      char* key;
      Yaml_FORSEQ(document, value, node) {
        data = Minir_parseData(minir, document, node);
        DataList_push2(dataList, data);
      } Yaml_ENDSEQ;
    }
    else if(!strcmp(key, "section")) {
      const char* secName;
      DataSection section;
      if (Yaml_IS_MAPPING (value)) { /* create a section */
        secName = Minir_parseSection (minir, document, value);
      } else {
        secName = Yaml_GET_SCALAR_STRING(document, value);
      }
      section = Program_searchSection (program, secName);
      Procedure_setSection (procedure, section);
    }
    else if(!strcmp(key, "align")) {
      uint8_t align = Yaml_GET_INTEGER (document, value);
      Procedure_setAlign(procedure, align);
    }
    else if(!strcmp(key, "linkage")) {
      char* link = Yaml_GET_SCALAR_STRING(document, value);
      if(!strcmp(link, "global")) {
        Symbol_setExport(symbol, SymbolExport_GLOBAL_PROTECTED);
      }
      else {
        Minir_WARNING(value, "unknown linkage `%s'\n", link);
      }
    }
    else if(!strcmp(key, "args size")) {
      int8_t argsSize = Yaml_GET_INTEGER(document, value);
      Frame frame = Procedure_frame(procedure);
      Frame_setArgsSize(frame, argsSize);
    }
    else if(!strcmp(key, "loopscope")) { /* Loop information: data dependences */
      Yaml_FORSTRINGMAP(document, value, key, loopvalue) {
        if(!strcmp(key, "dependences")) {
/*          Minir_parseLoop*/
        }
        else {
          Minir_WARNING(loopvalue, "unused key `%s'\n", key);
        }
      } Yaml_ENDSTRINGMAP;
    }
    else {
      Minir_WARNING(this, "unused key `%s' when parsing function", key);
      node_pretty(document, value);
    }
  } Yaml_ENDSTRINGMAP;
  Minir_THROWIF(this, !procedure, "Label of procedure missing!");
  // Parse blocks
  if(basic_blocks) {
    // Create Jump Tables
    Yaml_FORSEQ(document, basic_blocks, node) {
      if(Minir_mappingIsJumpTable(document, node)) {
        Minir_parseJumpTable(minir, procedure, document, node);
      }
    } Yaml_ENDSEQ;
    // Create BasicBlocks with label
    Yaml_FORSEQ(document, basic_blocks, node) {
      if(Minir_mappingIsBlock(document, node)) {
        Minir_newBasicBlock(minir, procedure, document, node);
      }
    } Yaml_ENDSEQ;
    // Connect BasicBlocks with preds & succs fields if any.
    // Default is to link all basic blocks with fall-through edges
    Minir_EXPLICITFLOW(minir) = false; // will be change to "true" if any "preds" or "succs" field are encountered
    Yaml_FORSEQ(document, basic_blocks, node) {
      if(Minir_mappingIsBlock(document, node)) {
        Minir_connectBasicBlocks(minir, procedure, document, node);
      }
    } Yaml_ENDSEQ;
    /*      // Check that explicit flow was encountered if under SSA*/
    /*      Minir_EXPLICITFLOW(minir) = false; // will be change to "true" if any "preds" or "succs" field are encountered*/
    // If Minir is under SSA, BasicBlocks should be connected using preds & 
    // succs fields
    if(Minir_isSSA(minir)) {
      IDList list;
      CodeRegion codeRegion;
      // Build the code region (required for SSA variables)
      Procedure_buildCodeRegions(procedure);
      list = Procedure_codeRegions(procedure);
      Minir_THROWIF(this, 
          (IDList_count(list) != 1),
          "More than one CodeRegion in program not currently supported in Minir!");
      codeRegion = IDList_firstItem(list);
      Minir_CODEREGION(minir) = codeRegion;
      // Sort enter edges of basic blocks
      Procedure_FOREACH_BasicBlock(procedure, block) {
        BasicBlock_makeEnterEdges(block, Memory_Root, true);
      } Procedure_ENDEACH_BasicBlock;
    }//end if SSA
    // Populate BasicBlocks
    Yaml_FORSEQ(document, basic_blocks, node) {
      if(Minir_mappingIsBlock(document, node)) {
        Minir_parseBasicBlockOperations(minir, procedure, document, node);
      }
    } Yaml_ENDSEQ;
    // Create Loop Scopes
    Yaml_FORSEQ(document, basic_blocks, node) {
      if(Minir_mappingIsLoop(document, node)) {
        Minir_parseLoop(minir, procedure, document, node);
      }
    } Yaml_ENDSEQ;
  }
  
  // Setup entry blocks
  if(entries_node) {
    int numEntries = 0;
    Convention convention = Program_CONVENTION(program);
    Yaml_FORSEQ(document, entries_node, entry) {
      char* name = Yaml_GET_SCALAR_STRING(document, entry);
      long int bbIndex = Minir_bbIndexOfName(minir, entry, name);
      BasicBlock block = Minir_findBasicBlock(minir, bbIndex);
      numEntries++;
      if(!Minir_ISTEST(minir)) {
        Minir_THROWIF(entry, numEntries > 1,
            "There can only by one entry basic block!");
        Minir_THROWIF(entry, block == NULL,
            "Basic block `%s' does not exist and cannot be an `entry'!",
            name
            );
        Procedure_FOREACH_BasicBlock(procedure, basicBlock) {
          Minir_THROWIF(entry, basicBlock != block,
              "The entry basic block should be the first in the `bbs' list!");
          break;
        } Procedure_ENDEACH_BasicBlock;
      }
      Procedure_setEntryBlock(procedure, block);
      BasicBlock_flagEntryCode(block, convention); // mark entry code in the block
    } Yaml_ENDSEQ;
  }
  // Setup exit blocks
  if(exits_node) {
    Convention convention = Program_CONVENTION(program);
    Yaml_FORSEQ(document, exits_node, nexit) {
      char* name = Yaml_GET_SCALAR_STRING(document, nexit);
      long int bbIndex = Minir_bbIndexOfName(minir, nexit, name);
      BasicBlock block = Minir_findBasicBlock(minir, bbIndex);
      Minir_THROWIF(nexit, block == NULL,
          "Basic block `%s' does not exist and cannot be an `exit'!",
          name
          );
      Procedure_setExitBlock(procedure, block);
      BasicBlock_flagExitCode(block, convention);
    } Yaml_ENDSEQ;
  }
  // Check frame size is consistent  // Not used anymore
  /*
  Procedure_FOREACH_BasicBlock(procedure, block) {
    if(BasicBlock_isEntry(block)) {
      Operation adj = BasicBlock_findAdjust(block);
      if(adj) {
        Operation_OPERANDS_FOREACH_Temporary(adj, temp) {
          if(Temporary_hasLiteral(temp)) {
            Frame frame = Procedure_frame(procedure);
            if(frame_size == -1) {
              frame_size = (int)Literal_value(Temporary_LITERAL(temp));
              Frame_setSize(frame, frame_size);
            } else {
              Minir_THROWIF(this, 
                  frame_size != Literal_value(Temporary_LITERAL(temp)),
                  "Frame size set to %d but adjust operation uses %d!",
                  (int)frame_size,
                  (int)Literal_value(Temporary_LITERAL(temp))
                  );
            }
            break;
          }
        } Operation_OPERANDS_ENDEACH_Temporary;
      }
      break; //Check only one entry basic block
    }
  } Procedure_ENDEACH_BasicBlock;
  */
  // Build the CodeRegions to allow SSA variables
  // Already done if Minir is under SSA
  if(!Minir_isSSA(minir)) Procedure_buildCodeRegions(procedure);
  // Convert all temporaries to SSA variables
  // BUGGY!
  // Minir_transformSSAVariables(procedure);
  return procedure;
}

#ifdef $XCC__h
/**
 * Parse the global data section.
 */
void
Minir_parseObjects(Minir minir, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

void
Minir_parseObjects(Minir minir, yaml_document_t *document, yaml_node_t *this)
{
  Program program = Minir_PROGRAM(minir);
  DataList dataList = Program_dataList(program);
  Data data;
  char* key;
  Yaml_FORSEQ(document, this, node) {
    key = Yaml_FIRSTMAPKEY(document, node);
    if(!strcmp(key, "section")) {
      Minir_parseSection(minir, document, node);
    } else {
      data = Minir_parseData(minir, document, node);
      DataList_push2(dataList, data);
    }
  } Yaml_ENDSEQ;
}

#ifdef $XCC__h
/**
 * Parse the one object.
 */
void
Minir_parseObject(Minir minir, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

void
Minir_parseObject(Minir minir, yaml_document_t *document, yaml_node_t *this)
{
  Program program = Minir_PROGRAM(minir);
  DataList dataList = Program_dataList(program);
  Data data;
  data = Minir_parseData(minir, document, this);
  DataList_push2(dataList, data);
}


#ifdef $XCC__h
/**
 * Parse the Tirex mapping
 */
void
Minir_parseTirex(Minir minir, yaml_document_t *document, yaml_node_t *this);
#endif//$XCC__h

void
Minir_parseTirex(Minir minir, yaml_document_t *document, yaml_node_t *this)
{
  char* key;
  int funCount;
  Program program = Minir_PROGRAM(minir);
  DataList dataList = Program_dataList(program);
  Data data;
  PtrSeq_ procSymbols_;
  PtrSeq procSymbols = &procSymbols_;
  PtrSeq_Ctor(procSymbols, Program_memory(program), 0);
  // Creating symbols first
  Yaml_FORSEQ(document, this, node) {
    // Determine type of information in current node
    key = Yaml_FIRSTMAPKEY(document, node);
    if(!strcmp(key, "common")) {
      data = Minir_parseData(minir, document, node);
      DataList_push2(dataList, data);
    }
    else if(!strcmp(key, "object")) {
      data = Minir_parseData(minir, document, node);
      DataList_push2(dataList, data);
    }
    else if(!strcmp(key, "section")) {
      Minir_parseSection(minir, document, node);
    }
    else if(!strcmp(key, "function")) {
      Symbol symbol = Minir_makeFunctionSymbol(minir, document, node);
      PtrSeq_push2(procSymbols, symbol);
    }
    else if(!strcmp(key, "optimize")) {
          }
    else {
      Minir_WARNING(node, "Warning: unused key `%s'! (%s:%d) \n", key, __FILE__, __LINE__);
    }
  } Yaml_ENDSEQ;
  // Parsing functions
  funCount = 0;
  Yaml_FORSEQ(document, this, node) {
    key = Yaml_FIRSTMAPKEY(document, node);
    if(!strcmp(key, "function")) {
      Symbol symbol = PtrSeq_access(procSymbols, funCount);
      Minir_parseFunction(minir, symbol, document, node);
      Minir_resetFunction(minir);
      funCount++;
    }
  } Yaml_ENDSEQ;
  PtrSeq_Dtor(procSymbols);
}


#ifdef $XCC__h
/**
 * Parse a program in Minir format.
 */
Program
Minir_parseProgram(Minir minir, yaml_document_t *document);
#endif//$XCC__h

Program
Minir_parseProgram(Minir minir, yaml_document_t *document)
{
  yaml_node_t *node, *root;
  Program program = NULL ;
  /* Root node is a mapping with "functions" key */
  root = yaml_document_get_root_node(document);
  Except_THROWIF(!root, "Cannot find yaml root node!");
  // If Tirex, program properties can be found in program->optimize
  node = Yaml_mapping_lookup_key(document, root, "program");
  if(node) {
    node = Yaml_sequence_lookup_first_key(document, node, "optimize");
    if(node) {
      Processor processor = 0;
      Convention convention = 0;
      Yaml_FORSTRINGMAP(document, node, optstring, optvalue) {
        if(!strcmp(optstring, "optimize")) { // Do nothing
        }  
        else if(!strcmp(optstring, "processor")) {
          char *arch = Yaml_GET_SCALAR_STRING(document, optvalue);
          processor = Processor_fromName(arch);
          Minir_THROWIF(optvalue, !processor, "Unknown Processor `%s'!", arch);
        }
        else if(!strcmp(optstring, "convention")) {
          convention = Yaml_GET_INTEGER(document, optvalue);
          Minir_THROWIF(optvalue, !Convention_check(convention),
              "Bad convention `%d'!", convention);
        }
        else {
          Minir_WARNING(node, "Unused key `%s' in optimize field!", optstring);
        }
      } Yaml_ENDSTRINGMAP;
      Minir_THROWIF(node, processor == 0, "`processor' not given!");
      Minir_THROWIF(node, convention == 0, "`convention' not given!");
      program = Program_new(Memory_Root, processor, convention);
    }
  }
  if(!program)
    program = Program_new(Memory_Root, (Processor)1, (Convention)1);
  /* Get ready for reading some variables/temporaries */
  *Minir__PROGRAM(minir) = program;
  *Minir__PROGRAMCOUNTER(minir) += 1;
  Minir_initTemporaries(minir);
  node = Yaml_mapping_lookup_key(document, root, "label");
  if(node) {
    char* name = Yaml_GET_SCALAR_STRING(document, node);
#ifdef   MINIR_DEBUG 
    printf("Parsing program `%s'...\n", name);
  } else {
    printf("Parsing un-labeled program...\n");
#endif
  }
  *Minir__ISSSA(minir) = false;
  Yaml_FORSTRINGMAP(document, root, key, node) {
    if(!strcmp(key, "label") ||
       !strcmp(key, "file")) {
      const char* filename = Yaml_GET_SCALAR_STRING (document, node);
      Program_setFilename (program, filename);
    }
    else if(!strcmp(key, "keep index")) {
      bool keep = Yaml_GET_BOOL(document, node);
      *Minir__KEEP_INDEX(minir) = keep;
    }
    else if(!strcmp(key, "SSA")) {
      bool ssa = Yaml_GET_BOOL(document, node);
      *Minir__ISSSA(minir) = ssa;
#ifdef     MINIR_DEBUG 
      printf("The program is %sunder SSA Form.\n", ssa ? "" : "not ");
#endif
    }
    else if(!strcmp(key, "arch")) {
      char *arch = Yaml_GET_SCALAR_STRING(document, node);
      Program_PROCESSOR(program) = Processor_fromName(arch);
#ifdef     MINIR_DEBUG 
      printf("The target cpu is %s.\n", arch);
#endif
      Minir_THROWIF(node, !Program_PROCESSOR(program), "Unknown Processor `%s'!", arch);
    }
    else if(!strcmp(key, "functions")){ // Minir
      int count;
      PtrSeq_ procSymbols_;
      PtrSeq procSymbols = &procSymbols_;
      PtrSeq_Ctor(procSymbols, Program_memory(program), 0);
#ifdef     MINIR_DEBUG 
      printf("Parsing all Minir functions\n");
#endif
      /* Reading function names */
      Yaml_FORSEQ(document, node, in_node) {
        Symbol symbol = Minir_makeFunctionSymbol(minir, document, in_node);
        PtrSeq_push2(procSymbols, symbol);
      } Yaml_ENDSEQ;
      /* Parsing functions */
      count=0;
      Yaml_FORSEQ(document, node, in_node) {
        Symbol symbol = PtrSeq_access(procSymbols, count);
        Minir_parseFunction(minir, symbol, document, in_node);
        Minir_resetFunction(minir);
        count++;
      } Yaml_ENDSEQ;
      PtrSeq_Dtor(procSymbols);
    }
    else if(!strcmp(key, "objects")) {
      Minir_parseObjects(minir, document, node);
    }
    else if(!strcmp(key, "tirex") ||
            !strcmp(key, "program")) { // Tirex way
      Minir_parseTirex(minir, document, node);
    }
    else {
      Minir_WARNING(node, "Warning: unused key `%s'! (%s:%d) \n", key, __FILE__, __LINE__);
    }
  } Yaml_ENDSTRINGMAP;
  /* Set Temporary counter to the maximum encountered yet. */
  if(Minir_KEEP_INDEX(minir))
    IndexedFactory_COUNTER(Temporary_Factory) =
      IndexedFactory_MAXCOUNT(Temporary_Factory);
  return program;
}

/*************************************
 *  Main function
 *************************************/

#ifdef $XCC_h
/**
 * Read a new program from the yaml parser.
 *
 * @return @c NULL if there is no more program to be read.
 * @sa Minir_parseFile
 */
Program
Minir_nextProgram(Minir minir);
#endif//$XCC_h

Program
Minir_nextProgram(Minir minir)
{
  yaml_document_t document;
  Program program;
  int ret;
#ifdef MINIR_DEBUG 
  printf("\n******************\nReading next document...\n");
#endif
  ret = yaml_parser_load(&Minir_PARSER(minir), &document);
  Except_THROWIF(ret == 0, "Cannot load document from parser! Check file is correct Yaml syntax.");
  if(!yaml_document_get_root_node(&document)) {
    return NULL;
  }
#ifdef MINIR_DEBUG 
  printf("**\nParsing Minir program...\n");
#endif
  program = Minir_parseProgram(minir, &document);
#ifdef MINIR_DEBUG 
    printf("\n*****************\nPrinting program\n");
    Program_FOREACH_Procedure(program, procedure) {
      Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
        printf("*** Printing Code Region...\n");
        CodeRegion_pretty(codeRegion, stdout);
      } Procedure_ENDEACH_CodeRegion;
    } Program_ENDEACH_Procedure;
#endif
  yaml_document_delete(&document);
  Minir_resetProgram(minir);
  return program;
}


#if 0
#ifdef $XCC_h
/**
 * Read the first Minir program from a file descriptor.
 */
Program
Minir_parseFile(Minir minir, FILE *file);
#endif//$XCC_h

Program
Minir_parseFile(Minir minir, FILE *file)
{
  yaml_parser_t parser = Minir_init_parser(minir, file);
  yaml_document_t document;
  Program program;
  MINIR_DEBUG && printf("Reading next document.\n");
  Except_CHECK(yaml_parser_load(&parser, &document));
  Except_ALWAYS(!yaml_document_get_root_node(&document));
  printf("\n*****************\nRead one document\n");
  program = Minir_parseProgram(minir, &document);
  if (MINIR_DEBUG) {
    Program_FOREACH_Procedure(program, procedure) {
      Procedure_FOREACH_CodeRegion(procedure, codeRegion) {
        printf("*** Printing Code Region...\n");
        CodeRegion_pretty(codeRegion, stdout);
      } Procedure_ENDEACH_CodeRegion;
    } Program_ENDEACH_Procedure;
  }
  yaml_document_delete(&document);
  Minir_close_parser(minir, parser);
  return program;
}
#endif//0

#endif//YAML_ENABLED

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  DataList dataList;
  Tirex tirex = Tirex_new(Memory_Root, stdin);
  Program program;
  fprintf(stderr, "TESTING MINIR...\n");
  while( (program = Tirex_makeProgram(tirex)) ) {
    Program_FOREACH_Procedure(program, procedure) {
      CodeRegion codeRegion = Procedure_findCodeRegion(procedure, 0);
      dataList = Procedure_dataList(procedure);
      CodeRegion_pretty(codeRegion, stdout);
      Procedure_pretty(procedure, stdout);
    } Program_ENDEACH_Procedure;
    dataList = Program_dataList(program);
    if(!DataList_isEmpty(dataList)) 
      DataList_pretty(dataList, stdout);
    Program_delete(program);
  }
  Tirex(minir);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Minir module.
 */
#define Minir_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Minir module.
 */
#define Minir_FINI()
#endif//$XCC__h

#if XCC__C
static void
Minir_TEST(void)
{
#include "LIR/Minir_.c"
}

int
main(int argc, char **argv)
{
  LIR_Test(Minir_TEST);
  return 0;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
{
  // No self-test.
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////
