#ifdef $XCC_h
/*
 * !!!!	Template.xcc
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
#include "MDT/Template.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#if Template_ENABLE
#define Template(ID, ALIGNMENT, INCREMENT, DISTANCES, ENCODE, DECODE) \
  Template_##ID,
#endif//Template_ENABLE
  Template_,
#include "Template.tuple"
  Template__
#undef Template
} enum_Template;
#endif//$XCC__h

const int Template__Count = Template__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Template enumeration.
 */
extern const char *
Template_Id(Template this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Template_Id(Template this)
{
  static const char *_[] = {
#if Template_ENABLE
#define Template(ID, ALIGNMENT, INCREMENT, DISTANCES, ENCODE, DECODE) \
    #ID,
#endif//Template_ENABLE
    "_UNDEF",
#include "Template.tuple"
  };
  Except_REQUIRE((unsigned)this < Template__);
  return _[this];
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int template = 0;
  for (template = 0; template < Template__; template++) {
    printf("Template_%s\t%d\n", Template_Id(template), template);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Template increment of the PC.
 */
int
Template_increment(Template this);
#endif//$XCC_h

extern const int8_t
Template_Increment[];
int
Template_increment(Template this)
{
  Except_REQUIRE((unsigned)this < Template__);
  return Template_Increment[this];
}

const int8_t
Template_Increment[] = {
#define INCREMENT(d) d
#if Template_ENABLE
#define Template(ID, ALIGNMENT, INCREMENT, DISTANCES, ENCODE, DECODE) \
  INCREMENT,
#endif//Template_ENABLE
  INCREMENT(0),
#include "Template.tuple"
};

#ifdef $XCC__c
{
  int template = 0;
  for (template = 0; template < Template__; template++) {
    int increment = Template_increment(template);
    printf("Increment(%s)\t%d\n", Template_Id(template), increment);
  }
  fprintf(stderr, "sizeof(Template_Increment)\t%zu\n", sizeof(Template_Increment));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Template count of Dispersals.
 */
int
Template_count(Template this);
#endif//$XCC_h

extern const uint8_t
Template_Count[];
int
Template_count(Template this)
{
  Except_REQUIRE((unsigned)this < Template__);
  return Template_Count[this];
}

const uint8_t
Template_Count[] = {
#define DISTANCES(count, items) count
#if Template_ENABLE
#define Template(ID, ALIGNMENT, INCREMENT, DISTANCES, ENCODE, DECODE) \
  DISTANCES,
#endif//Template_ENABLE
  DISTANCES(0, /**/),
#include "Template.tuple"
#undef DISTANCES
};

#ifdef $XCC__c
{
  int template = 0;
  for (template = 0; template < Template__; template++) {
    int count = Template_count(template);
    printf("Count(%s)\t%d\n", Template_Id(template), count);
  }
  fprintf(stderr, "sizeof(Template_Count)\t%zu\n", sizeof(Template_Count));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Template Dispersal PC distances from Bundle PC.
 */
const uint8_t *
Template_distances(Template this);
#endif//$XCC_h

extern const uint8_t
Template_Distances[][Template_DISPERSALS_MAX];
const uint8_t *
Template_distances(Template this)
{
  Except_REQUIRE((unsigned)this < Template__);
  return Template_Distances[this];
}

const uint8_t
Template_Distances[][Template_DISPERSALS_MAX] = {
#define DISTANCE(d) d,
#define DISTANCES(count, items) { items }
#if Template_ENABLE
#define Template(ID, ALIGNMENT, INCREMENT, DISTANCES, ENCODE, DECODE) \
  DISTANCES,
#endif//Template_ENABLE
  DISTANCES(0, /**/),
#include "Template.tuple"
};

#ifdef $XCC__c
{
  int template = 0;
  for (template = 0; template < Template__; template++) {
    const uint8_t *distances = Template_distances(template);
    int count = Template_count(template), i;
    printf("Distances(%s)\t", Template_Id(template));
    for (i = 0; i < count; i++) {
      printf("%d\t", distances[i]);
    }
    printf("\n");
  }
  fprintf(stderr, "sizeof(Template_Distances)\t%zu\n", sizeof(Template_Distances));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Template encode function.
 */
typedef void
(*TemplateEncode)(const_OpcodeEncoded *restrict codeWords, void *restrict buffer);
TemplateEncode
Template_encode(Template this);
#endif//$XCC_h

extern TemplateEncode
Template_Encode[];
TemplateEncode
Template_encode(Template this)
{
  Except_REQUIRE((unsigned)this < Template__);
  return Template_Encode[this];
}

#define ENCODE(e) e
#if Template_ENABLE
#define Template(ID, ALIGNMENT, INCREMENT, DISTANCES, ENCODE, DECODE) \
static void \
TemplateEncode_##ID(const_OpcodeEncoded *restrict codeWords, void *restrict buffer) \
{ \
  ENCODE; \
}
#endif//Template_ENABLE
#include "Template.tuple"
#undef ENCODE
static void
TemplateEncode_(const_OpcodeEncoded *restrict codeWords, void *restrict buffer)
{
}

TemplateEncode
Template_Encode[] = {
#if Template_ENABLE
#define Template(ID, ALIGNMENT, INCREMENT, DISTANCES, ENCODE, DECODE) \
  TemplateEncode_##ID,
#endif//Template_ENABLE
  TemplateEncode_,
#include "Template.tuple"
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Template module.
 */
#define Template_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Template module.
 */
#define Template_FINI()
#endif//$XCC__h

#if XCC__C
static void
Template_TEST(void)
{
#include "MDT/Template_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Template_TEST);
  return 0;
}
#endif

