#ifdef $XCC_h
/*
 * !!!!	XMLGen.xcc
 *
 * Laura Hernandez de la Puerta (lauhp09@gmail.com)
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2008 STMicroelectronics.
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
 * @brief XML Generator.
 */
#endif//$XCC_h


#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/XMLGen.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * UTF-8 strings
 */
typedef char *UTF8;
typedef const char *const_UTF8;
int escape;
#endif//$XCC_h

#ifdef $XCC_h
typedef enum {
  SEQUENCE_NO_DOC,
  SEQUENCE_PRE_DOC,
  SEQUENCE_POST_DOC,
  SEQUENCE_START_TAG,
  SEQUENCE_ATTRIBUTES,
  SEQUENCE_CONTENT,
} writerSequence;
#endif//$XCC_h

#ifdef $XCC_h
typedef enum {
  XMLGenStatus_OK,
  XMLGenStatus_Error,
  XMLGenStatus_BadUTF8,
  XMLGenStatus_NonXML,
  XMLGenStatus_AllocFailed,
  XMLGenStatus_SeqError,
  XMLGenStatus_CommentError,
  XMLGenStatus_PIError,
} XMLGenStatus;
#endif//$XCC_h

#ifdef $XCC_h
/*
 * XMLGenWriter_rec -- XML Generator object.
 */
struct XMLGenWriter_rec_;
#endif//$XCC_h

#ifdef $XCC_h
typedef struct
{
  XMLGenWriter_rec writer;
  int        count;
  int        space;
  void * *   pointers;
} plist;
#endif//$XCC_h

#ifdef $XCC_h
typedef struct
{
  UTF8 buf;
  int  used;
  int  space;
} collector;
#endif//$XCC_h

#ifdef $XCC__h
struct XMLGenWriter_rec_
{
  //@args      XMLGenStatus status,  Buffer_ buffer, plist elements, void * userData, void * (* alloc)(void * userData, int bytes), void(* dealloc)(void * userData, void * data);
  XMLGenStatus status;
  Buffer_ *buffer;
  plist elements;
  plist stackElems;
  plist attributes;
  writerSequence sequence;
  int escape;
  void *          	   userData;
  void *       		(* alloc)(void * userData, int bytes);
  void         		(* dealloc)(void * userData, void * data);
};
#endif//$XCC__h

#ifdef $XCC_h
struct XMLGenElement_rec
{
  //@args const_UTF8 type
   const_UTF8 type;
};
#endif//$XCC_h

#ifdef $XCC_h
struct XMLGenElement_{
  struct XMLGenElement_rec *XMLGENELEMENT;
};
#endif//$XCC_h

#ifdef $XCC_h
struct XMLGenAttribute_rec_ {
  const_UTF8 name;
  int provided;
  collector value;
};
#endif//$XCC_h

#ifdef $XCC_h
struct XMLGenAttribute_ {
  struct XMLGenAttribute_rec_ *XMLGENATTRIBUTE;
};
#endif//$XCC_h

#ifdef $XCC_h
/*
 * XMLGen -- XML Generator object.
 */
struct XMLGen_;
#endif//$XCC_h

#ifdef $XCC__h
struct XMLGen_ {
  //@args	Memory memory, struct XMLGenWriter_rec *XMLGenWriter
   Memory MEMORY;
  struct XMLGenWriter_rec *XMLGENWRITER;
};
#endif//$XCC__h

#ifdef $XCC_h
/*
 * isLetter
 */
static int isLetter(int c) __attribute__ ((unused));
static int isLetter(int c)
{
  if (c < 0 || c > 0xffff)
    return 0;
  else
    return 1;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * isNameChar
 */
static int isNameChar(int c) __attribute__ ((unused));
static int isNameChar(int c)
{
  if (c < 0 || c > 0xffff)
    return 0;
  else
    return 1;
}
#endif//$XCC_h

#ifdef $XCC_h
/*
 * isXMLChar
 */
int isXMLChar(int c);
#endif//$XCC_h

int isXMLChar(int c)
{
  if (c < 0)
    return 0;
  else
    return (c <= 0x10ffff);
}


#ifdef $XCC_h
/*
 * NextUnicodeChar
 */
int NextUnicodeChar(const_UTF8 *sp);
#endif//$XCC_h

int NextUnicodeChar(const_UTF8 *sp)
{
  const unsigned char *s = (const unsigned char *) *sp;
  int c;
  if (*s == 0)
    return -1;
  if (*s < 0x80)
    c = *s++;
  else if (*s < 0xc2)
    goto malformed;
  else if (*s < 0xe0)
  {
    c = (*s++ & 0x1f) << 6;
      if (*s < 0x80 || *s > 0xbf)
      goto malformed;
    c |= *s++ & 0x3f;
  }
  else if (*s < 0xf0)
  {
    int b0 = *s;
    c = (*s++ & 0x0f) << 12;
    if ((b0 == 0xe0 && (*s < 0xa0 || *s > 0xbf)) ||(b0 <  0xed && (*s < 0x80 || *s > 0xbf)) ||(b0 == 0xed && (*s < 0x80 || *s > 0x9f)) ||(b0  > 0xed && (*s < 0x80 || *s > 0xbf)))
      goto malformed;
    c |= (*s++ & 0x3f) << 6;
    if (*s < 0x80 || *s > 0xbf)
      goto malformed;
    c |= *s++ & 0x3f;
  }
  else if (*s < 0xf5)
  {
    int b0 = *s;
    c = (*s++ & 0x07) << 18;
    if ((b0 == 0xf0 && (*s < 0x90 || *s > 0xbf)) ||(b0 <  0xf4 && (*s < 0x80 || *s > 0xbf)) ||(b0 >= 0xf4 && (*s < 0x80 || *s > 0x8f)))
      goto malformed;
    c |= (*s++ & 0x3f) << 12;
    if (*s < 0x80 || *s > 0xbf)
      goto malformed;
    c |= (*s++ & 0x3f) << 6;
    if (*s < 0x80 || *s > 0xbf)
      goto malformed;
    c |= *s++ & 0x3f;
  }
  else
    goto malformed;
  *sp = (const_UTF8)s;
  return c;
malformed:
  if (*s) ++s;
  *sp = (const_UTF8)s;
  return -1;
}

#ifdef $XCC_h
/*
 * checkNCName
 */
XMLGenStatus checkNCName(const_UTF8 name);
#endif//$XCC_h

XMLGenStatus checkNCName(const_UTF8 name)
{
  int c;

  if (name == NULL || *name == 0)
    return XMLGenStatus_Error;

  c = NextUnicodeChar(&name);
  if (!isLetter(c) && c != ':' && c != '_')
    return XMLGenStatus_Error;

  while (*name)
  {
    c = NextUnicodeChar(&name);
    if (c == -1)
      return XMLGenStatus_BadUTF8;
    if (!isNameChar(c))
      return XMLGenStatus_NonXML;
  }
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/*
 * checkText
 */
XMLGenStatus checkText( const_UTF8 s);
#endif//$XCC_h

XMLGenStatus checkText( const_UTF8 s)
{

  while (*s)
  {
    int c = NextUnicodeChar(&s);
    if (c == -1)
      return XMLGenStatus_BadUTF8;
    if (!isXMLChar(c))
      return XMLGenStatus_NonXML;
   }
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/*
 * initCollector
 */
XMLGenStatus initCollector(XMLGenWriter_rec w, collector * c);
#endif//$XCC_h

XMLGenStatus initCollector(XMLGenWriter_rec w, collector * c)
{
  c->space = 20;
  if ((c->buf = allocate(w, c->space)) == NULL)
    return XMLGenStatus_Error;
  c->used = 0;
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/*
 * growCollector
 */
XMLGenStatus growCollector(XMLGenWriter_rec w, collector * c, int size);
#endif//$XCC_h

XMLGenStatus growCollector(XMLGenWriter_rec w, collector * c, int size)
{
  UTF8 newSpace;
  c->space = size * 2;
  if ((newSpace = allocate(w, c->space)) == NULL)
    return XMLGenStatus_AllocFailed;
  strncpy((char *) newSpace, (const char *) c->buf, c->used);
  newSpace[c->used] = 0;
  if (c->buf!=NULL){
    deallocate(w, c->buf);
    c->buf = newSpace;
  }
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/*
 * startCollect
 */
void startCollect(collector * c);
#endif//$XCC_h

void startCollect(collector * c)
{
  c->used = 0;
}

#ifdef $XCC_h
/*
 * endCollect
 */
void endCollect(collector * c);
#endif//$XCC_h

void endCollect(collector * c)
{
  c->buf[c->used] = 0;
}

#ifdef $XCC_h
/*
 * collectString
 */
XMLGenStatus collectString(XMLGenWriter_rec w, collector * c, const_UTF8 string);
#endif//$XCC_h

XMLGenStatus collectString(XMLGenWriter_rec w, collector * c, const_UTF8 string)
{
  int sl = strlen((const char *) string);
  if (sl >= c->space)
    if ((w->status = growCollector(w, c, sl)) != XMLGenStatus_OK)
      return XMLGenStatus_AllocFailed;
  strcpy((char *) c->buf, (const char *) string);
  return XMLGenStatus_OK;
}

#define collectPiece(w,c,d,size) {if (((c)->used+(size))>=(c)->space){if (((w)->status=growCollector(w,c,(c)->used+(size)))!=XMLGenStatus_OK) return (w)->status;}strncpy((char *)(c)->buf+(c)->used,d,size);(c)->used+=size;}

#ifdef $XCC_h
/*
 * initPlist
 */
XMLGenStatus initPlist(XMLGenWriter_rec w, plist * pl);
#endif//$XCC_h

XMLGenStatus initPlist(XMLGenWriter_rec w, plist * pl)
{
  pl->writer = w;
  pl->count = 0;
  pl->space = 20;
  pl->pointers = (void * *) allocate(w, pl->space * sizeof(void *));
  if (pl->pointers == NULL)
    return XMLGenStatus_AllocFailed;
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/*
 * initElemList
 */
XMLGenStatus initElemList(XMLGenWriter_rec w, plist * pl);
#endif//$XCC_h

XMLGenStatus initElemList(XMLGenWriter_rec w, plist * pl)
{
  pl->count = 0;
  pl->space = 20;
  pl->pointers = NULL;
  return XMLGenStatus_OK;
}

XMLGen
XMLGen_Ctor(XMLGen this, Memory memory, struct XMLGenWriter_rec *XMLGenWriter)
{
  *XMLGen__MEMORY(this) = memory;
  *XMLGen__XMLGENWRITER(this) = XMLGenWriter;
   return this;
}

void
XMLGen_Dtor(XMLGen this)
{
  memset(this, -1, sizeof(XMLGen_));
}

size_t
XMLGen_Size(Memory memory, struct XMLGenWriter_rec *XMLGenWriter)
{
  return sizeof(XMLGen_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(XMLGen_)\t%zu\n", sizeof(XMLGen_));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Make a new XMLGen.
 */
XMLGen
XMLGen_new(Memory parent);
#endif//$XCC_h

XMLGen
XMLGen_new(Memory parent)
{
  struct XMLGenWriter_rec *writer = NULL;
  void *userData;
  int bytes;
  void *data;
  plist * liste;
  Memory memory = Memory_new(parent, true);

  void *(*alloc)(void *userData,int bytes ) = (void *(*)(void *, int))Memory_alloc;
  void (*dealloc)(void *userData,void *data ) = (void (*)(void *, void *))Memory_free;
    
  writer = malloc(sizeof(struct XMLGenWriter_rec_)); 
  if (writer == NULL) return NULL;
  else {
    size_t size = XMLGen_Size(memory, writer);
    void *new = Memory_alloc(memory, size);
    return XMLGen_Ctor(new, memory, writer);
  }
}

#ifdef $XCC_h
/*
 * XMLGen_dispose
 */
void XMLGen_dispose(XMLGenWriter_rec_ *writer);
#endif//$XCC_h

void XMLGen_dispose(XMLGenWriter_rec_ *writer){

  int i;
  XMLGenElement * ee = (XMLGenElement *) writer->elements.pointers;
  XMLGenAttribute * aa = (XMLGenAttribute *) writer->attributes.pointers;
  XMLGenElement * ss = (XMLGenElement *) writer->stackElems.pointers;

  for (i = 1; i <= writer->elements.count; i++)
  {
    deallocate(writer, (void *)((struct XMLGenElement_rec *)ee[i])->type);
    deallocate(writer, ee[i]);
  }
  for (i = 1; i <= writer->attributes.count; i++)
  {
    deallocate(writer, (void *)((struct XMLGenAttribute_rec_ *)aa[i])->name);
    deallocate(writer, (void *)((struct XMLGenAttribute_rec_ *)aa[i])->value.buf);
    deallocate(writer, aa[i]);
  }
  for (i = 1; i <= writer->stackElems.count; i++)
  {
    deallocate(writer, (void *)((struct XMLGenElement_rec *)ss[i])->type);
    deallocate(writer, ss[i]);
  }
  deallocate(writer, writer->elements.pointers);
  deallocate(writer, writer->attributes.pointers);
  deallocate(writer, writer->stackElems.pointers);
  deallocate(writer, writer);
  return;
}

#ifdef $XCC_h
/**
 * Delete this XMLGen.
 */
XMLGen
XMLGen_delete(XMLGen this);
#endif//$XCC_h

XMLGen
XMLGen_delete(XMLGen this)
{
  if (this != NULL) {
    Memory memory = XMLGen_MEMORY(this);
    XMLGenWriter_rec_ *writer = (XMLGenWriter_rec_ *)XMLGen_XMLGENWRITER(this);
    XMLGen_dispose(writer);
#ifndef _NDTOR
    XMLGen_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

#ifdef $XCC_h
/*
 *  allocate
 */
void * allocate(XMLGenWriter_rec w, int bytes);
#endif//$XCC_h

void * allocate(XMLGenWriter_rec w, int bytes)
{
  if (w->alloc)
    return (void *) (*w->alloc)(w->userData, bytes);
  else
    return (void *) malloc(bytes);
}

#ifdef $XCC_h
/*
 *  deallocate
 */
void deallocate(XMLGenWriter_rec w, void * data);
#endif//$XCC_h

void deallocate(XMLGenWriter_rec w, void * data)
{
  if (w->dealloc)
    (*w->dealloc)(w->userData, data);
  else if (w->alloc == NULL)
    free(data);
}

#ifdef $XCC_h
/*
 *  copy
 */
UTF8 copy(XMLGenWriter_rec w, const_UTF8 from);
#endif//$XCC_h

UTF8 copy(XMLGenWriter_rec w, const_UTF8 from)
{
  UTF8 temp;

  if ((temp = allocate(w, strlen((const char *) from) + 1)) == NULL)
    return NULL;
  strcpy((char *) temp, (const char *) from);
  return temp;
}

#ifdef $XCC_h
/*
 *  checkExpand -- make room in a plist
 */
int checkExpand(plist * pl);
#endif//$XCC_h

int checkExpand(plist * pl)
{
  void * * newlist;
  int i;

  if (pl->count < pl->space)
    return 0;

  pl->space *= 2;
  newlist = (void * *) allocate(pl->writer, pl->space * sizeof(void *));
  if (newlist == NULL)
    return -1;
  for (i = 1; i <= pl->count; i++)
    newlist[i] = pl->pointers[i];
  deallocate(pl->writer, pl->pointers);
  pl->pointers = newlist;

  return 0;
}

#ifdef $XCC_h
/*
 *  listAppend
 */
XMLGenStatus listAppend(plist * pl, void * pointer);
#endif//$XCC_h

XMLGenStatus listAppend(plist * pl, void * pointer)
{
  if (checkExpand(pl)!=0)
    return XMLGenStatus_AllocFailed;
  pl->count++;
  pl->pointers[pl->count] = pointer;

  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/*
 *  findElement
 */
int findElement(plist * pl, const_UTF8 type);
#endif//$XCC_h

int findElement(plist * pl, const_UTF8 type)
{
  int i;
  for (i = 1; i <= pl->count; i++)
    {
      if (strcmp((const char *) type,
                 (const char *) ((struct XMLGenElement_rec *)pl->pointers[i])->type) == 0)
        {
          return i;
        }
    }
  return -1;
}

#ifdef $XCC_h
/*
 *  findAttribute
 */
int findAttribute(plist * pl, const_UTF8 name);
#endif//$XCC_h

int findAttribute(plist * pl, const_UTF8 name)
{
  int i;
  for (i = 1; i <= pl->count; i++)
    {
      if ((const char *) ((struct XMLGenAttribute_rec_ *)pl->pointers[i])->name!=NULL)
        if (strcmp((const char *) name,
                   (const char *) ((struct XMLGenAttribute_rec_ *)pl->pointers[i])->name) == 0)
          {
            return i;
          }
    }
  return -1;
}

#ifdef $XCC_h
/**
 * Make a XMLGenElement.
 */
XMLGenElement
XMLGen_declareElement(XMLGen this, const_UTF8 type);
#endif//$XCC_h

XMLGenElement
XMLGen_declareElement(XMLGen this, const_UTF8 type)
{
  struct XMLGenElement_rec *elem= malloc(sizeof(struct XMLGenElement_rec));
  //memcpy(&elem->type,&type,sizeof(const_UTF8));

  XMLGenElement ee = malloc(sizeof(struct XMLGenElement_));
  int old=0;
  XMLGenStatus status = 0;
  XMLGenWriter_rec_ * writer = (XMLGenWriter_rec_ *)XMLGen_XMLGENWRITER(this);
  
   if ((writer->status = checkNCName(type)) != XMLGenStatus_OK)
  {
    return NULL;
  }

   
  /* already declared? */
  old = findElement(&writer->elements, type);
  if (old != -1){
    ee = ((&writer->elements)->pointers[old]);
    return ee;
  }

  if (((XMLGenElement) allocate(writer, sizeof(struct XMLGenElement_))) == NULL)
  {
    writer->status = XMLGenStatus_Error;
    return NULL;
  }

  if ((elem->type = copy(writer, type)) == NULL)
  {
    writer->status = XMLGenStatus_Error;
    return NULL;
  }
  {
  plist *list = &writer->elements;
  if (list->pointers == NULL){
    initPlist(writer,list);
  }
  }
  if ((writer->status = listAppend(&writer->elements, elem)) == XMLGenStatus_AllocFailed)
    {
      return NULL;
    }
  return ee;
}

#ifdef $XCC_h
/**
 * Make a XMLGenAttribute.
 */
XMLGenAttribute
XMLGen_declareAttribute(XMLGen this, const_UTF8 name);
#endif//$XCC_h

XMLGenAttribute
XMLGen_declareAttribute(XMLGen this, const_UTF8 name)
{

  Memory memory = XMLGen_MEMORY(this);
  //  XMLGenAttribute attribute = Memory_alloc(memory, sizeof(XMLGenAttribute_));
  int old=0;
  struct XMLGenAttribute_rec_ *attr = malloc(sizeof(struct XMLGenAttribute_rec_));
  XMLGenAttribute aa = malloc(sizeof(struct XMLGenAttribute_));
  XMLGenWriter_rec_ * writer;
  memcpy(&attr->name,&name,sizeof(const_UTF8));
  attr->provided=0;
  aa = (XMLGenAttribute)attr;
  
  writer = (XMLGenWriter_rec_ *)XMLGen_XMLGENWRITER(this);

  if ((writer->status = checkNCName(name)) != XMLGenStatus_OK)
  {
    return NULL;
  }

  /* already declared? */
  old = findAttribute(&writer->attributes, name);

  if (old != -1){
    aa = ((&writer->attributes)->pointers[old]);
    return aa;
  }

  if (((XMLGenAttribute) allocate(writer, sizeof(struct XMLGenAttribute_))) == NULL)
  {
    writer->status = XMLGenStatus_AllocFailed;
    return NULL;
  }

  if ((attr->name = copy(writer, name)) == NULL)
  {
    writer->status = XMLGenStatus_AllocFailed;
    return NULL;
  }
  {
  plist *list = &writer->attributes;
  if (list->pointers == NULL){
    initPlist(writer,list);
  }
  }
  if ((writer->status = listAppend(&writer->attributes, attr)) == XMLGenStatus_AllocFailed)
    {
      return NULL;
    }
  return aa;
}

#ifdef $XCC_h
/**
 * Add an attribute to the current element.
 */
XMLGenStatus
XMLGen_addAttribute(XMLGen this, XMLGenAttribute attribute, const_UTF8 value);
#endif//$XCC_h

XMLGenStatus
XMLGen_addAttribute(XMLGen this, XMLGenAttribute attribute, const_UTF8 value)
{

  UTF8 lastv = (UTF8) value;
  XMLGenWriter_rec_ * writer = (XMLGenWriter_rec_ *)XMLGen_XMLGENWRITER(this);
  struct XMLGenAttribute_rec_ *a = malloc (sizeof(struct XMLGenAttribute_rec_ *));
  //a = (struct XMLGenAttribute_rec_ *)attribute;
  struct XMLGenElement_rec *elem = malloc (sizeof(struct XMLGenElement_rec_ *));
  UTF8 comp = malloc(5*sizeof(char));
  int num,i,j= 1;
  char cmp[4];
  char *trad =malloc (10*sizeof(char));
  char *tmp =malloc (10*sizeof(char));
  UTF8 breaker, next;
  
  initCollector(writer, &a->value);
  if (writer->sequence != SEQUENCE_START_TAG &&
      writer->sequence != SEQUENCE_ATTRIBUTES)
    return writer->status = XMLGenStatus_SeqError;
  writer->sequence = SEQUENCE_ATTRIBUTES;

  if (value)
    {
      startCollect(&a->value);
      while (*value)
        {
          int c = NextUnicodeChar(&value);
          if (c == -1)
            return writer->status = XMLGenStatus_BadUTF8;
          if (!isXMLChar(c))
            return writer->status = XMLGenStatus_NonXML;
         
          switch(c)
            {
            case 9:
              collectPiece(writer, &a->value, "&#x9;", 5);
              break;
            case 0xa:
              collectPiece(writer, &a->value, "&#xA;", 5);
              break;
            case 0xd:
              collectPiece(writer, &a->value, "&#xD;", 5);
              break;
            case '"':
              collectPiece(writer, &a->value, "&quot;", 6);
              break;
            case '<':
              collectPiece(writer, &a->value, "&lt;", 4);
              break;
            case '&':
              collectPiece(writer, &a->value, "&amp;", 5);
              break;
            case '\'':
              collectPiece(writer, &a->value, "&apos;", 6);
              break;
            case '>':
              collectPiece(writer, &a->value, "&gt;", 4);
              break;
            case '0':
              /*if Escape option, we have to escape the Unicode characters */
              strncpy(comp,(UTF8)value,5*sizeof(char));
/* 	      sprintf(trad,""); */
/* 	      sprintf(tmp,""); */
              for (i=0;i<4;i++)
                cmp[i]='\0';
                // cmp[i]=(char)""; // <- Florent: was previously that (????)

              //////////////// ESCAPE ENTITY XML //////////////////
              if (comp[0]=='x'){
                comp++;
                for (i=0;i<4;i++){
                  if (!(((comp[i]=='0' && i!=3) && i!=2) || (comp[i]=='1')
                        || (comp[i]=='2') || (comp[i]=='3')
                        || (comp[i]=='4') || (comp[i]=='5')
                        || (comp[i]=='6') || (comp[i]=='7')
                        || (comp[i]=='8') || (comp[i]=='9')
                        || (comp[i]=='A') || (comp[i]=='B')
                        || (comp[i]=='C') || (comp[i]=='D')
                        || (comp[i]=='E') || (comp[i]=='F')
                        || (comp[i]=='a') || (comp[i]=='b')
                        || (comp[i]=='c') || (comp[i]=='d')
                        || (comp[i]=='e') || (comp[i]=='f')))
                    {
                      strncpy(cmp,comp,(i)*sizeof(char));
                      break;
                    }
                  else{
                    strncpy(cmp,comp,(i+1)*sizeof(char));
                  }
                }
              }
              else{
                collectPiece(writer, &a->value, (const char *) lastv, value - lastv);
                break;
              }
              for (j=0;j<=i;j++){
                value++;
              }
              num = atoi((const char*)cmp); 
              sprintf(trad,"0x%s",cmp);
              	      
              if (num>0 || trad!=NULL){
        
                if (escape){
                  tmp = Escape_putXMLChar(num);
                  
                  if (tmp!=NULL){
                    sprintf(trad,"&%s;",tmp);
                    collectPiece(writer, &a->value, (const char *) trad, strlen(trad));
                    break;
                  }
                }
                //////////////// ESCAPE ENTITY HTML //////////////////
                /* make UTF8 representation of character */
                breaker=next=trad;
                
                sscanf(trad, "%X", &num);
                if (num < 0x80)
                  *next++ = num;
                else if (num < 0x800)
                  {
                    *next++ = 0xc0 | (num >> 6);
                    *next++ = 0x80 | (num & 0x3f);
                  }
                else if (num < 0x10000)
                  {
                    *next++ = 0xe0 | (num >> 12);
                    *next++ = 0x80 | ((num & 0xfc0) >> 6);
                    *next++ = 0x80 | (num & 0x3f);
                  }
                else
                  {
                    *next++ = 0xf0 | (num >> 18);
                    *next++ = 0x80 | ((num & 0x3f000) >> 12);
                    *next++ = 0x80 | ((num & 0xfc0) >> 6);
                    *next++ = 0x80 | (num & 0x3f);
                  }
                *next = 0;
                
                memcpy(trad,breaker,(next-breaker)*(sizeof(const_UTF8)));
                collectPiece(writer, &a->value, (const char *) trad, strlen(trad));
                break;
              }
              else{
                collectPiece(writer, &a->value, (const char *) lastv, value - lastv);
              }
              break;
            default:
              collectPiece(writer, &a->value, (const char *) lastv, value - lastv);
              break;
            }
          lastv = (UTF8) value;
        }
      endCollect(&a->value);
    }
  a->provided = 1;
  free(trad);
  free(tmp);
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/*
 * XMLGen_PI
 */
XMLGenStatus XMLGen_PI(XMLGen this, const_UTF8 target, const_UTF8 text);
#endif//$XCC_h

XMLGenStatus XMLGen_PI(XMLGen this, const_UTF8 target, const_UTF8 text)
{

  XMLGenWriter_rec_ * writer = (XMLGenWriter_rec_ *)XMLGen_XMLGENWRITER(this);
  int i;

  if (writer->sequence == SEQUENCE_NO_DOC)
    return writer->status = XMLGenStatus_SeqError;

  if ((writer->status = checkText(target)) != XMLGenStatus_OK)
    return writer->status;
  
  if ((strlen((const char *) target) >= 3) &&
      (target[0] == 'x' || target[0] == 'X') &&
      (target[1] == 'm' || target[1] == 'M') &&
      (target[2] == 'l' || target[2] == 'L') ){
    return writer->status = XMLGenStatus_PIError;
  }
  if ((writer->status = checkText(text)) != XMLGenStatus_OK)
    return writer->status;

  /* no ?> within */
  for (i = 1; text[i]; i++)
    if (text[i] == '>' && text[i - 1] == '?')
      return writer->status = XMLGenStatus_PIError;

  if (writer->sequence == SEQUENCE_START_TAG ||
      writer->sequence == SEQUENCE_ATTRIBUTES)
  {
    if ((writer->status = writeTag((this),1)) !=  XMLGenStatus_OK)
      return writer->status;
    writer->sequence = SEQUENCE_CONTENT;
  }

  else if (writer->sequence == SEQUENCE_POST_DOC)
  {
    if ((Buffer_appendS(writer->buffer, "\n")) == 0)
      return XMLGenStatus_Error;
    if ((Buffer_appendS(writer->buffer, "<?")) == 0)
      return XMLGenStatus_Error;
    if ((Buffer_appendS(writer->buffer, target)) == 0)
      return XMLGenStatus_Error;
  }

  if (text[0])
  {
    if ((Buffer_appendS(writer->buffer, " ")) == 0)
      return XMLGenStatus_Error;
    if ((Buffer_appendS(writer->buffer, text)) == 0)
      return XMLGenStatus_Error;
    if ((Buffer_appendS(writer->buffer, "?>")) == 0)
      return XMLGenStatus_Error;

    if (writer->sequence == SEQUENCE_PRE_DOC)
      if ((Buffer_appendS(writer->buffer, "\n")) == 0)
        return XMLGenStatus_Error;
  }
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/**
 * Start a new document.
 */
XMLGenStatus
XMLGen_startDocFile(XMLGen this, int escapeOpt);
#endif//$XCC_h

XMLGenStatus
XMLGen_startDocFile(XMLGen this, int escapeOpt)
{
  XMLGenWriter_rec_ * writer = (XMLGenWriter_rec_ *)XMLGen_XMLGENWRITER(this);
  if (escapeOpt)
    escape = 1;
  else
    escape = 0;
  // write on the buffer
  {
  Buffer_ buffer[1];
  Buffer_Ctor(buffer, Memory_Root, 4);
  writer->buffer = malloc(sizeof(Buffer_));
  memcpy(writer->buffer,buffer,sizeof(Buffer_));
  initElemList(writer, &writer->elements);
  initElemList(writer, &writer->attributes);
  initElemList(writer, &writer->stackElems);
  writer->alloc=NULL;
  writer->dealloc=NULL;
  {
  XMLGenStatus status = XMLGenStatus_OK;
  if (writer->sequence != SEQUENCE_NO_DOC)
    return writer->status = XMLGenStatus_SeqError;
  writer->sequence = SEQUENCE_PRE_DOC;
  return status;
  }
  }
}

#ifdef $XCC_h
/**
 * End a document.
 */
XMLGenStatus
XMLGen_endDocument(XMLGen this, FILE *file);
#endif//$XCC_h

XMLGenStatus
XMLGen_endDocument(XMLGen this, FILE *file)
{
  XMLGenWriter_rec_ *writer = (XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);
  writer->attributes.count = 0;
  writer->elements.count = 0;
  if (writer->sequence != SEQUENCE_POST_DOC)
    return writer->status = XMLGenStatus_SeqError;
  writer->sequence = SEQUENCE_NO_DOC;
  if (writer->status == XMLGenStatus_OK){
    Buffer_fwrite(writer->buffer, file);
    fclose(file);
  }
  Buffer_Dtor(writer->buffer);
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/**
 * Start an element.
 */
XMLGenStatus
XMLGen_startElement(XMLGen this, XMLGenElement element);
#endif//$XCC_h

XMLGenStatus
XMLGen_startElement(XMLGen this, XMLGenElement element)
{
  int i=0;
  XMLGenElement e = malloc (sizeof(struct XMLGenElement_));
  XMLGenWriter_rec_ *writer = (XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);
  struct XMLGenElement_rec *elem = malloc(sizeof(struct XMLGenElement_rec_ *));

  elem = (struct XMLGenElement_rec *)element;
  e = XMLGen_declareElement(this, elem->type);
  if (e == NULL || writer->status != XMLGenStatus_OK)
    return writer->status;
  elem = (struct XMLGenElement_rec *)e;

  switch (writer->sequence)
  {
  case SEQUENCE_NO_DOC:
  case SEQUENCE_POST_DOC:
    return writer->status = XMLGenStatus_SeqError;
  case SEQUENCE_START_TAG:
    if ((writer->status = writeTag((this),0)) != XMLGenStatus_OK)
      return writer->status;
    Buffer_appendS(writer->buffer, "\n");
    break;
  case SEQUENCE_ATTRIBUTES:
    if ((writer->status = writeTag((this),0)) != XMLGenStatus_OK)
      return writer->status;
    Buffer_appendS(writer->buffer, "\n");
    break;
  case SEQUENCE_PRE_DOC:
    break;
  case SEQUENCE_CONTENT:
    break;
  }

  writer->sequence = SEQUENCE_START_TAG;
  {
  plist *list = &writer->stackElems;
  if (list->pointers == NULL){
    initPlist(writer,list);
  }
  }
  if ((writer->status = listAppend(&writer->stackElems, elem)) == XMLGenStatus_AllocFailed)
    {
      return XMLGenStatus_AllocFailed;
    }

  /* clear provided attributes */
  for (i = 1; i <=  writer->attributes.count; i++)
    ((struct XMLGenAttribute_rec_ *) writer->attributes.pointers[i])->provided = 0;

  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/**
 * Start an element.
 */
XMLGenStatus
XMLGen_startElementLiteral(XMLGen this, const_UTF8 type);
#endif//$XCC_h

XMLGenStatus
XMLGen_startElementLiteral(XMLGen this, const_UTF8 type)
{
  XMLGenElement e;
  XMLGenWriter_rec_ *writer = (XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);
  struct XMLGenElement_rec *elem = malloc(sizeof(struct XMLGenElement_rec_ *));

  e = XMLGen_declareElement(this, type);
  if (e == NULL || writer->status != XMLGenStatus_OK)
    return writer->status;
  elem = (struct XMLGenElement_rec *)e;

  switch (writer->sequence)
  {
  case SEQUENCE_NO_DOC:
  case SEQUENCE_POST_DOC:
    return writer->status = XMLGenStatus_SeqError;
  case SEQUENCE_START_TAG:
    break;
  case SEQUENCE_ATTRIBUTES:
    if ((writer->status = writeTag((this),0)) != XMLGenStatus_OK)
      return writer->status;
    break;
  case SEQUENCE_PRE_DOC:
  case SEQUENCE_CONTENT:
    break;
  }

  writer->sequence = SEQUENCE_START_TAG;

  ////////////////////////
  {
  plist *list = &writer->stackElems;
  if (list->pointers == NULL){
    initPlist(writer,list);
  }
  }
  if ((writer->status = listAppend(&writer->stackElems, elem)) == XMLGenStatus_AllocFailed)
    {
      return XMLGenStatus_AllocFailed;
    }
  //////////////////////

  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/**
 * Add an attribute to the current element.
 */
XMLGenStatus
XMLGen_addAttributeLiteral(XMLGen this, const_UTF8 name, const_UTF8 value);
#endif//$XCC_h

XMLGenStatus
XMLGen_addAttributeLiteral(XMLGen this, const_UTF8 name, const_UTF8 value)
{
  XMLGenWriter_rec_ *writer =(XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);
  XMLGenAttribute a;

  a = XMLGen_declareAttribute(this, name);
  if (a == NULL || writer->status != XMLGenStatus_OK)
    return writer->status;
  return XMLGen_addAttribute(this, a, value);
}

#ifdef $XCC_h
/**
 * Add text to the current element.
 */
XMLGenStatus
XMLGen_addText(XMLGen this, const_UTF8 start);
#endif//$XCC_h

XMLGenStatus
XMLGen_addText(XMLGen this, const_UTF8 start)
{
  XMLGenWriter_rec_ *writer =(XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);

  if (writer->sequence == SEQUENCE_START_TAG ||
      writer->sequence == SEQUENCE_ATTRIBUTES)
  {
    if ((writer->status = writeTag((this),1)) != XMLGenStatus_OK)
      return writer->status;
    writer->sequence = SEQUENCE_CONTENT;
  }

  if (writer->sequence != SEQUENCE_CONTENT)
    return writer->status = XMLGenStatus_SeqError;

  // check character entities...
    while (*start)
      {
        int c = NextUnicodeChar(&start);
        if (c == -1)
          writer->status = XMLGenStatus_BadUTF8;
        if (!isXMLChar(c))
          writer->status = XMLGenStatus_NonXML;
        if (!addEscape(this,c)){
          Buffer_appendC(writer->buffer, c);
        }
      }
    writer->status = XMLGenStatus_OK;
    return writer->status;
}

#ifdef $XCC_h
/*
 * XMLGen_comment --  Inserts a comment with the text provided
 */
XMLGenStatus
XMLGen_comment(XMLGen this, const_UTF8 text);
#endif//$XCC_h

XMLGenStatus
XMLGen_comment(XMLGen this, const_UTF8 text)
{
  XMLGenWriter_rec_ *writer =(XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);
  int i;

  if (writer->sequence == SEQUENCE_NO_DOC)
    return writer->status = XMLGenStatus_SeqError;

  if ((writer->status = checkText(text)) != XMLGenStatus_OK)
    return writer->status;
  /* no leading '-', no trailing '-', no '--' */
  if (text[0] == '-')
    return writer->status = XMLGenStatus_CommentError;
  for (i = 0; text[i]; i++)
    if (text[i] == '-' && (text[i + 1] == '-' || text[i + 1] == 0))
      return writer->status = XMLGenStatus_CommentError;

  if (writer->sequence == SEQUENCE_START_TAG ||
      writer->sequence == SEQUENCE_ATTRIBUTES)
  {
    if ((writer->status = writeTag((this),1)) != XMLGenStatus_OK)
      return writer->status;
    writer->sequence = SEQUENCE_CONTENT;
  }

  else if (writer->sequence == SEQUENCE_POST_DOC)
    if ((Buffer_appendS(writer->buffer, "\n")) == 0)
      return XMLGenStatus_Error;
  if ((Buffer_appendS(writer->buffer, "<!--")) == 0)
    return XMLGenStatus_Error;
  if ((Buffer_appendS(writer->buffer, text)) == 0)
    return XMLGenStatus_Error;
  if ((Buffer_appendS(writer->buffer, "-->")) == 0)
    return XMLGenStatus_Error;
  if (writer->sequence == SEQUENCE_PRE_DOC)
    if ((Buffer_appendS(writer->buffer, "\n")) == 0)
      return XMLGenStatus_Error;

  return XMLGenStatus_OK;

}

#ifdef $XCC_h
/*
 * addEscape
 */
int addEscape(XMLGen this, int c);
#endif//$XCC_h

int addEscape(XMLGen this, int c)
{
  XMLGenWriter_rec_ *writer =(XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);

  if (c == -1)
    return XMLGenStatus_Error;

  if (!isXMLChar(c))
    return XMLGenStatus_NonXML;
  switch(c)
  {
  case 0xd:
    Buffer_appendS(writer->buffer, "&#xD;");
    break;
  case '<':
    Buffer_appendS(writer->buffer, "&lt;");
  break;
  case '&':
    Buffer_appendS(writer->buffer, "&amp;");
    break;
  case '>':
    Buffer_appendS(writer->buffer, "&gt;");
    break;
  case '\'':
    Buffer_appendS(writer->buffer, "&apos;");
    break;
  case '"':
    Buffer_appendS(writer->buffer, "&quot;");
    break;
  default:
    return 0;
    break;
  }
  return 1;
}

#ifdef $XCC_h
/**
 * Add unicode character to the current element.
 */
XMLGenStatus
XMLGen_addCharacter(XMLGen this, int c);
#endif//$XCC_h

XMLGenStatus
XMLGen_addCharacter(XMLGen this, int c)
{
  XMLGenWriter_rec_ *writer =(XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);
  unsigned char cUTF8[10];
  UTF8 breaker, next;
  int char_escape = 0;

  if (writer->sequence == SEQUENCE_START_TAG ||
      writer->sequence == SEQUENCE_ATTRIBUTES)
  {
    if ((writer->status = writeTag((this),1)) != XMLGenStatus_OK)
      return writer->status;
    writer->sequence = SEQUENCE_CONTENT;
  }

  if (writer->sequence != SEQUENCE_CONTENT)
    return writer->status = XMLGenStatus_SeqError;
  if (!isXMLChar(c)){
    
    return writer->status = XMLGenStatus_NonXML;
  }
  
  /* make UTF8 representation of character */
  breaker = next = (UTF8)cUTF8;
  
  if (c < 0x80)
    *next++ = c;
  else if (c < 0x800)
    {
      *next++ = 0xc0 | (c >> 6);
      *next++ = 0x80 | (c & 0x3f);
    }
  else if (c < 0x10000)
    {
      *next++ = 0xe0 | (c >> 12);
      *next++ = 0x80 | ((c & 0xfc0) >> 6);
      *next++ = 0x80 | (c & 0x3f);
    }
  else
    {
      *next++ = 0xf0 | (c >> 18);
      *next++ = 0x80 | ((c & 0x3f000) >> 12);
      *next++ = 0x80 | ((c & 0xfc0) >> 6);
      *next++ = 0x80 | (c & 0x3f);
    }
  *next = 0;

  if (escape){
    char *trad=malloc (20*sizeof(char));
    if ((trad =(char *) Escape_putXMLChar(c)) != NULL){
       Buffer_appendS(writer->buffer, "&");
      Buffer_appendS(writer->buffer, trad);
      Buffer_appendS(writer->buffer, ";");
      char_escape = 1;
    }
  }
  if (!char_escape){
      if (!addEscape(this, c)){
        memcpy(cUTF8,breaker,(next-breaker)*(sizeof(const_UTF8)));
        Buffer_appendS(writer->buffer, (UTF8)cUTF8);
      }
  }
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/*
 * writeTag
 */
XMLGenStatus
writeTag (XMLGen this, int empty);
#endif//$XCC_h

XMLGenStatus writeTag (XMLGen this, int empty){

  int i=0;
  XMLGenWriter_rec_ *writer =(XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);
  struct XMLGenElement_rec *elem;
  XMLGenAttribute * aa = (XMLGenAttribute *) (&writer->attributes)->pointers;
  elem = (struct XMLGenElement_rec *)(&writer->stackElems)->pointers[(&writer->stackElems)->count];
  for (i=writer->stackElems.count;i>1;i--){
    Buffer_appendS(writer->buffer,"\t");
  }
  Buffer_appendS(writer->buffer, "<");
  Buffer_appendS(writer->buffer, elem->type);

  for (i = 1; i <= writer->attributes.count; i++)
  {
    if (((struct XMLGenAttribute_rec_ *)aa[i])->provided)
    {
      Buffer_appendS(writer->buffer, " ");
      Buffer_appendS(writer->buffer, ((struct XMLGenAttribute_rec_ *)aa[i])->name);
      Buffer_appendS(writer->buffer, "=\"");
      Buffer_appendS(writer->buffer, ((struct XMLGenAttribute_rec_ *)aa[i])->value.buf);
      Buffer_appendS(writer->buffer, "\"");
    }
  }
  /* clear provided attributes */
  for (i = 1; i <= writer->attributes.count; i++)
    ((struct XMLGenAttribute_rec_ *) writer->attributes.pointers[i])->provided = 0;
  if (empty)
    Buffer_appendS(writer->buffer, "/>");
  else
    Buffer_appendS(writer->buffer, ">");
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/**
 * End the current element.
 */
XMLGenStatus
XMLGen_endEmptyElement(XMLGen this);
#endif//$XCC_h

XMLGenStatus
XMLGen_endEmptyElement(XMLGen this)
{
  int i=0;
  XMLGenWriter_rec_ *writer =(XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);
  
  switch (writer->sequence)
  {
  case SEQUENCE_NO_DOC:
  case SEQUENCE_PRE_DOC:
  case SEQUENCE_POST_DOC:
    return writer->status = XMLGenStatus_SeqError;
  case SEQUENCE_START_TAG:
  case SEQUENCE_ATTRIBUTES:
    if ((writer->status = writeTag((this),1)) != XMLGenStatus_OK)
      return writer->status;
    break;
  case SEQUENCE_CONTENT:
    break;
  }
  Buffer_appendS(writer->buffer,"\n");
  writer->stackElems.count--;
  
  /* pop the NULL */
  if (writer->stackElems.count < 0)
    return writer->status = XMLGenStatus_Error;
  if (writer->stackElems.count == 0)
    writer->sequence = SEQUENCE_POST_DOC;
  else
    writer->sequence = SEQUENCE_CONTENT;
  return XMLGenStatus_OK;
}

#ifdef $XCC_h
/**
 * End the current element.
 */
XMLGenStatus
XMLGen_endElement(XMLGen this);
#endif//$XCC_h

XMLGenStatus
XMLGen_endElement(XMLGen this)
{
  int i=0;
  XMLGenWriter_rec_ *writer =(XMLGenWriter_rec_ *) XMLGen_XMLGENWRITER(this);

  switch (writer->sequence)
  {
  case SEQUENCE_NO_DOC:
  case SEQUENCE_PRE_DOC:
  case SEQUENCE_POST_DOC:
    return writer->status = XMLGenStatus_SeqError;
  case SEQUENCE_START_TAG:
  case SEQUENCE_ATTRIBUTES:
    if ((writer->status = writeTag((this),1)) != XMLGenStatus_OK)
      return writer->status;
    break;
  case SEQUENCE_CONTENT:
    break;
  }
  Buffer_appendS(writer->buffer,"\n");
  for (i=writer->stackElems.count;i>1;i--){
    Buffer_appendS(writer->buffer,"\t");
  }
  Buffer_appendS(writer->buffer, "</");
  Buffer_appendS(writer->buffer, ((struct XMLGenElement_rec *)(&writer->stackElems)->pointers[(&writer->stackElems)->count])->type);
  Buffer_appendS(writer->buffer, ">\n");

  writer->stackElems.count--;

  /* pop the NULL */
  if (writer->stackElems.count < 0)
    return writer->status = XMLGenStatus_Error;
  if (writer->stackElems.count == 0)
    writer->sequence = SEQUENCE_POST_DOC;
  else
    writer->sequence = SEQUENCE_CONTENT;

  return XMLGenStatus_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__c
/* { */
#endif//$XCC__c

/*   char year_value[100], month_value[100]; */
/*   XMLGen gen = XMLGen_new(Memory_Root); */
/*   XMLGenStatus status;  */
/*   status = XMLGen_startDocFile(gen);  */
/*   XMLGenElement dates = XMLGen_declareElement(gen, "dates"); */
/*   XMLGenElement date = XMLGen_declareElement(gen, "date"); */
/*   XMLGenAttribute yyyy = XMLGen_declareAttribute(gen, "yyyy"); */
/*   XMLGenAttribute atr2 = XMLGen_declareAttribute(gen, "id"); */
/*   /\*  status = XMLGen_startElement(gen, dates); *\/ */
/*   /\*   status = XMLGen_startElementLiteral(gen, "lazy");  *\/ */
/*     sprintf(year_value, "%d", 1900 + 3); */
/*     sprintf(month_value, "%02d", 1 + 5); */
/*   /\*   XMLGen_addText(gen, "\n  "); *\/ */
/*   status = XMLGen_startElement(gen, date);     */
/*   XMLGen_addAttribute(gen, yyyy, year_value); */
/*   XMLGen_addAttribute(gen, atr2, month_value); */
/*   XMLGen_addText(gen, "aaabbbccccdddeee");  */
/*   status = XMLGen_startElement(gen, dates);     */
/*   XMLGen_addAttribute(gen, yyyy, year_value); */
/*   /\*   XMLGen_endElement(gen); *\/ */
/*   /\*   XMLGen_endElement(gen); *\/ */
/*   XMLGen_endElement(gen); */
/*   XMLGen_endElement(gen); */
/*   XMLGen_endDocument(gen); */
/*   XMLGen_delete(gen); */
/*   printf("\n");  */
/* } */

#ifdef $XCC__c
if (0) {
  int escapeExt=1;
  XMLGen gen = XMLGen_new(Memory_Root);
  XMLGenElement dates = XMLGen_declareElement(gen, "dates");
  XMLGenElement date = XMLGen_declareElement(gen, "date");
  XMLGenAttribute yyyy = XMLGen_declareAttribute(gen, "yyyy");
  XMLGenAttribute mm = XMLGen_declareAttribute(gen, "mm");
  int i;
  static char year[10] = { 1, 3, 2, 7, 9, 4, 8, 19, 29, 72 };
  static char month[10] = { 9, 3, 5, 2, 6, 6, 1, 8, 4, 7 };
  XMLGen_startDocFile(gen,escapeExt);
  XMLGen_startElement(gen, dates);
  XMLGen_addCharacter(gen, 0x5A);
  XMLGen_comment(gen,"This is a commentaire");
  for (i = 0; i < 10; i++) {
    char year_value[100], month_value[100];
    sprintf(year_value, "%d", 1900 + year[i]);
    sprintf(month_value, "%02d", 1 + month[i]);
    XMLGen_addText(gen, "\n  ");
    XMLGen_startElement(gen, date);
    XMLGen_addAttribute(gen, yyyy, year_value);
    XMLGen_addAttribute(gen, mm, month_value);
    XMLGen_addAttributeLiteral(gen, "day", "0");
    XMLGen_PI(gen, "xml:pi2", "2");
    XMLGen_addCharacter(gen, 0x208);
    XMLGen_addCharacter(gen, 0x3042);
    //XMLGen_addText(gen, "text in date & \"23/07/08\"....");
    XMLGen_startElementLiteral(gen, "lazy");
    XMLGen_comment(gen,"This is a comment");
    XMLGen_addText(gen, "Text of lazy...");
    XMLGen_endElement(gen);
    XMLGen_endElement(gen);
  }
  XMLGen_addText(gen, "\n");
  XMLGen_endElement(gen);
  XMLGen_endDocument(gen, stdout);
  //
  XMLGen_delete(gen);
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the XMLGen module.
 */
#define XMLGen_INIT()
/*@XCC_.h
 * XMLGen_FINI --	Finalize the XMLGen module.
 */
#define XMLGen_FINI()
#endif//$XCC__h

#if XCC__C
static void
XMLGen_TEST(void)
{
#include "BSL/XMLGen_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(XMLGen_TEST);
  return 0;
}
#endif

