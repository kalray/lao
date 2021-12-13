#ifdef $XCC_h
/*
 * !!!!	XMLCan.xcc
 *
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com)
 *
 * Copyright 2007 STMicroelectronics.
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
 * @ingroup BSL
 * @brief XML Canonical pull scanner.
 */
#endif//$XCC_h

#include "BSL/BSL_.h"

#ifdef $XCC__h
#include "BSL/XMLCan.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Input buffer for XMLCan.
 */
struct XMLCanBuffer_;
#endif//$XCC_h

#ifdef $XCC__h
struct XMLCanBuffer_ {
  //@args	Memory memory, FILE *input, size_t size
  Memory MEMORY;
  FILE *INPUT;
  size_t SIZE;
  char *DATA;
  char *START;
  char *PAST;
};
#endif//$XCC__h

XMLCanBuffer
XMLCanBuffer_Ctor(XMLCanBuffer this,
                  Memory memory, FILE *input, size_t size)
{
  char *data = Memory_alloc(memory, size + 8);
  *XMLCanBuffer__MEMORY(this) = memory;
  *XMLCanBuffer__INPUT(this) = input;
  *XMLCanBuffer__SIZE(this) = size;
  *XMLCanBuffer__DATA(this) = data;
  *XMLCanBuffer__START(this) = data;
  *XMLCanBuffer__PAST(this) = data;
  return this;
}

void
XMLCanBuffer_Dtor(XMLCanBuffer this)
{
  Memory memory = XMLCanBuffer_MEMORY(this);
  Memory_free(memory, XMLCanBuffer_DATA(this));
}

size_t
XMLCanBuffer_Size(Memory memory, FILE *input, size_t size)
{
  return sizeof(XMLCanBuffer_);
}

static bool
XMLCanBuffer_check(XMLCanBuffer this)
{
  Except_ALWAYS(XMLCanBuffer_DATA(this) <= XMLCanBuffer_START(this));
  Except_ALWAYS(XMLCanBuffer_START(this) <= XMLCanBuffer_DATA(this) + XMLCanBuffer_SIZE(this) + 8);
  Except_ALWAYS(XMLCanBuffer_PAST(this) <= XMLCanBuffer_DATA(this) + XMLCanBuffer_SIZE(this) + 8);
  return true;
}

#ifdef $XCC_h
/**
 * Prefill this XMLCanBuffer with a string.
 */
void
XMLCanBuffer_prefill(XMLCanBuffer this, const char *str);
#endif//$XCC_h

void
XMLCanBuffer_prefill(XMLCanBuffer this, const char *str)
{
  size_t length = strlen(str);
  char *data = XMLCanBuffer_DATA(this);
  size_t size = XMLCanBuffer_SIZE(this);
  Except_CHECK(length <= size);
  *XMLCanBuffer__START(this) = memcpy(data, str, length + 1);
  *XMLCanBuffer__PAST(this) = data + length;
}

#ifdef $XCC__h
/**
 * Refill this XMLCanBuffer.
 */
intptr_t
XMLCanBuffer_refill(XMLCanBuffer this);
#endif//$XCC__h

intptr_t
XMLCanBuffer_refill(XMLCanBuffer this)
{
  intptr_t readCount = 0;
  FILE *input = XMLCanBuffer_INPUT(this);
  size_t size = XMLCanBuffer_SIZE(this);
  char *data = XMLCanBuffer_DATA(this);
  char *start = XMLCanBuffer_START(this);
  char *past = XMLCanBuffer_PAST(this);
  intptr_t inUse = past - start;
  Except_DEBUG(XMLCanBuffer_check(this));
  if (inUse <= 0) inUse = 0;
  else memmove(data, start, inUse);
  if (input != NULL) {
    readCount = fread(data + inUse, 1, size - inUse, input);
  }
  if (readCount > 0) {
    *XMLCanBuffer__START(this) = data;
    *XMLCanBuffer__PAST(this) = data + inUse + readCount;
    *XMLCanBuffer_PAST(this) = '\0';
  }
  return readCount;
}

#ifdef $XCC_h
/**
 * XMLCan event types for its pull API.
 */
typedef enum {
  XMLCanEvent_XML,		// XML declaration.
  XMLCanEvent_Comment,		// Comment.
  XMLCanEvent_DOCTYPE,		// DOCTYPE.
  XMLCanEvent_DTD,		// DTD declaration.
  XMLCanEvent_PublicID,		// Public ID.
  XMLCanEvent_SystemID,		// System ID.
  XMLCanEvent_PI,		// Processing Instruction.
  XMLCanEvent_STag,		// Element start.
  XMLCanEvent_AttName,		// Attribute name.
  XMLCanEvent_AttValue,		// Attribute value text.
  XMLCanEvent_CharData,		// Element text content.
  XMLCanEvent_CDATA,		// CDATA section content.
  XMLCanEvent_ETag,		// Element stop.
  XMLCanEvent_Error,		// Parse error.
  XMLCanEvent_End,		// End of input.
  XMLCanEvent__
} XMLCanEvent;
typedef int8_t short_XMLCanEvent;
#endif//$XCC_h

#ifdef $XCC_h
/*
 * XMLCan_event
 */
#define XMLCan_eventXML(this)	(XMLCan_event(this) == XMLCanEvent_XML)
#define XMLCan_eventComment(this)	(XMLCan_event(this) == XMLCanEvent_Comment)
#define XMLCan_eventDOCTYPE(this)	(XMLCan_event(this) == XMLCanEvent_DOCTYPE)
#define XMLCan_eventPublicID(this)	(XMLCan_event(this) == XMLCanEvent_PublicID)
#define XMLCan_eventSystemID(this)	(XMLCan_event(this) == XMLCanEvent_SystemID)
#define XMLCan_eventPI(this)	(XMLCan_event(this) == XMLCanEvent_PI)
#define XMLCan_eventSTag(this)	(XMLCan_event(this) == XMLCanEvent_STag)
#define XMLCan_eventAttName(this)	(XMLCan_event(this) == XMLCanEvent_AttName)
#define XMLCan_eventAttValue(this)	(XMLCan_event(this) == XMLCanEvent_AttValue)
#define XMLCan_eventCharData(this)	(XMLCan_event(this) == XMLCanEvent_CharData)
#define XMLCan_eventCDATA(this)	(XMLCan_event(this) == XMLCanEvent_CDATA)
#define XMLCan_eventETag(this)	(XMLCan_event(this) == XMLCanEvent_ETag)
#define XMLCan_eventError(this)	(XMLCan_event(this) == XMLCanEvent_Error)
#define XMLCan_eventEnd(this)	(XMLCan_event(this) == XMLCanEvent_End)
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Flag for the XMLCan.
 */
typedef enum {
  XMLCanFlag_Raw = 0x1,			// Do not normalize AttValue and CharData.
  XMLCanFlag_Trim = 0x2,		// Trim leading and trailing space in AttValue.
  XMLCanFlag_HTML = 0x4,		// Accept HTML syntax that is not XML valid.
  XMLCanFlag__
} XMLCanFlag;
typedef unsigned XMLCanFlags;
typedef uint8_t short_XMLCanFlags;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * XMLCan parser.
 */
struct XMLCan_;
  //@args	XMLCanBuffer buffer,
  //@args	int (*lookup)(struct XMLCan_ *this, const char *name), XMLCanFlags flags
#endif//$XCC_h

#ifdef $XCC__h
struct XMLCan_ {
  XMLCanBuffer BUFFER;
  int (*LOOKUP)(struct XMLCan_ *this, const char *name);
  const char *ERROR;
  int32_t LINE;
  char *TEXT;
  //@access NAME	XMLCan_TEXT(this)
  //@mutate NAME	XMLCan__TEXT(this)
  size_t SIZE;
  short_XMLCanFlags FLAGS;
  short_XMLCanEvent EXPECT;
  short_XMLCanEvent EVENT;
  char SAVED;
  char QUOTE;
  int16_t DEPTH;
};
#endif//$XCC__h

XMLCan
XMLCan_Ctor(XMLCan this,
            XMLCanBuffer buffer,
            int (*lookup)(struct XMLCan_ *this, const char *name), XMLCanFlags flags)
{
  *XMLCan__BUFFER(this) = buffer;
  *XMLCan__LOOKUP(this) = lookup;
  *XMLCan__ERROR(this) = NULL;
  *XMLCan__LINE(this) = 1;
  *XMLCan__TEXT(this) = NULL;
  *XMLCan__SIZE(this) = 0;
  *XMLCan__FLAGS(this) = flags;
  *XMLCan__EXPECT(this) = XMLCanEvent__;
  *XMLCan__EVENT(this) = XMLCanEvent__;
  *XMLCan__SAVED(this) = '\0';
  *XMLCan__QUOTE(this) = '\0';
  *XMLCan__DEPTH(this) = 0;
  XMLCanCharacterClass_Ctor();
  return this;
}

void
XMLCan_Dtor(XMLCan this)
{
  memset(this, -1, sizeof(XMLCan_));
}

size_t
XMLCan_Size(XMLCanBuffer buffer,
            int (*lookup)(struct XMLCan_ *this, const char *name), XMLCanFlags flags)
{
  return sizeof(XMLCan_);
}

typedef enum {
  XMLCanCharacter_WhiteSpace = 0x1,
  XMLCanCharacter_NameStart = 0x2,
  XMLCanCharacter_NameChar = 0x4,
  XMLCanCharacter_DecChar = 0x8,
  XMLCanCharacter_HexChar = 0x10,
  XMLCanCharacter__
} XMLCanCharacter;

static uint8_t XMLCanCharacterClass[256];

#ifdef $XCC__h
void
XMLCanCharacterClass_Ctor(void);
#endif//$XCC__h

void
XMLCanCharacterClass_Ctor(void)
{
  int count = sizeof(XMLCanCharacterClass)/sizeof(XMLCanCharacterClass[0]), c;
  if (XMLCanCharacterClass[0x9] != 0) return;
  for (c = 0; c < count; c++) {
    if ((c) == 0x9 ||
        (c) == 0xA ||
        (c) == 0xD ||
        (c) == 0x20) {
      XMLCanCharacterClass[c] |= XMLCanCharacter_WhiteSpace;
    }
    if ((c) == ':' ||
        ((c) >= 'A' && (c) <= 'Z') ||
        (c) == '_' ||
        ((c) >= 'a' && (c) <= 'z') ||
        ((c) >= 0xC0 && (c) <= 0xD6) ||
        ((c) >= 0xD8 && (c) <= 0xF6) ||
        ((c) >= 0xF8 && (c) <= 0x2FF)) {
      XMLCanCharacterClass[c] |= XMLCanCharacter_NameStart;
      XMLCanCharacterClass[c] |= XMLCanCharacter_NameChar;
    }
    if ((c) == '-' ||
        (c) == '.' ||
        ((c) >= '0' && (c) <= '9') ||
        (c) == 0xB7) {
      XMLCanCharacterClass[c] |= XMLCanCharacter_NameChar;
    }
    if ((c) >= '0' && (c) <= '9') {
      XMLCanCharacterClass[c] |= XMLCanCharacter_DecChar;
      XMLCanCharacterClass[c] |= XMLCanCharacter_HexChar;
    }
    if (((c) >= 'A' && (c) <= 'F') ||
        ((c) >= 'a' && (c) <= 'f')) {
      XMLCanCharacterClass[c] |= XMLCanCharacter_HexChar;
    }
  }
}

#ifdef $XCC__h
/*
 * Macros for the XML character classes.
 */
#define XMLCanIsWhiteSpace(c) \
  ((XMLCanCharacterClass[(unsigned char)(c)] & XMLCanCharacter_WhiteSpace) != 0)
#define XMLCanIsNameStart(c) \
  ((XMLCanCharacterClass[(unsigned char)(c)] & XMLCanCharacter_NameStart) != 0)
#define XMLCanIsNameChar(c) \
  ((XMLCanCharacterClass[(unsigned char)(c)] & XMLCanCharacter_NameChar) != 0)
#define XMLCanIsDecChar(c) \
  ((XMLCanCharacterClass[(unsigned char)(c)] & XMLCanCharacter_DecChar) != 0)
#define XMLCanIsHexChar(c) \
  ((XMLCanCharacterClass[(unsigned char)(c)] & XMLCanCharacter_HexChar) != 0)
#define XMLCanIsNewLineChar(c) \
  ((c) == '\n')
#endif//$XCC__h

#ifdef $XCC_h
/**
 * Normalize white space in text.
 */
size_t
XMLCan_normalizeWhiteSpace(const_XMLCan this, char *text, size_t size);
#endif//$XCC_h

size_t
XMLCan_normalizeWhiteSpace(const_XMLCan this, char *text, size_t size)
{
  XMLCanFlags flags = XMLCan_FLAGS(this);
  bool trim = (flags & XMLCanFlag_Trim) != 0;
  bool skipStore = trim;
  char *dest = text;
  int i = 0;
  for (; i < size; i++) {
    char current = text[i];
    if (XMLCanIsWhiteSpace(current)) {
      if (!skipStore) {
        *dest++ = ' ';
      }
      skipStore = true;
    } else {
      *dest++ = current;
      skipStore = false;
    }
  }
  if (trim) {
    if (dest[-1] == ' ') --dest;
  }
  return dest - text;
}

#ifdef $XCC__c
{
  char s0[] = "a b c";
  char s1[] = " a b   c\n";
  char s2[] = " \t\na b   c\n";
  XMLCan_ can[1];
  XMLCan_Ctor(can, NULL, XMLCan_lookupLatin1, XMLCanFlag_Trim);
  s0[XMLCan_normalizeWhiteSpace(can, s0, strlen(s0))] = '\0';
  s1[XMLCan_normalizeWhiteSpace(can, s1, strlen(s1))] = '\0';
  s2[XMLCan_normalizeWhiteSpace(can, s2, strlen(s2))] = '\0';
  printf("s0=\"%s\"\n", s0);
  printf("s1=\"%s\"\n", s1);
  printf("s2=\"%s\"\n", s2);
  XMLCan_Dtor(can);
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Lookup a character entity by name.
 */
int
XMLCan_lookupEntity(XMLCan this, const char *name, size_t size);
#endif//$XCC_h

int
XMLCan_lookupEntity(XMLCan this, const char *name, size_t size)
{
  int value = -1, i;
  if (size == 2 && name[0] == 'l' && name[1] == 't') return 60;
  if (size == 2 && name[0] == 'g' && name[1] == 't') return 62;
  if (size == 3 && name[0] == 'a' && name[1] == 'm' && name[2] == 'p') return 38;
  if (size == 4 && name[0] == 'a' && name[1] == 'p' && name[2] == 'o' && name[3] == 's') return 39;
  if (size == 4 && name[0] == 'q' && name[1] == 'u' && name[2] == 'o' && name[3] == 't') return 34;
  if (name[0] == '#') {
    if (name[1] == 'x') {
      for (i = 2; i < size; i++) {
        if (!XMLCanIsHexChar(name[i])) return -1;
      }
      sscanf(name + 2, "%x", &value);
    } else {
      for (i = 1; i < size; i++) {
        if (!XMLCanIsDecChar(name[i])) return -1;
      }
      sscanf(name + 1, "%d", &value);
    }
  } else {
    if (XMLCanIsNameStart(name[0])) {
      int (*lookup)(struct XMLCan_ *this, const char *name) = XMLCan_LOOKUP(this);
      for (i = 1; i < size; i++) {
        if (!XMLCanIsNameChar(name[i])) return -1;
      }
      if (lookup != NULL) {
        value = (*lookup)(this, name);
      }
    }
  }
  return value;
}

#ifdef $XCC_h
/**
 * Expand a character entity in place.
 */
bool
XMLCan_expandEntity(XMLCan this, char *start, char *stop);
#endif//$XCC_h

bool
XMLCan_expandEntity(XMLCan this, char *start, char *stop)
{
  int value = -1;
  char *name = start + 1;
  size_t size = stop - name;
  char saved = *stop;
  *stop = '\0';
  value = XMLCan_lookupEntity(this, name, size);
  *stop = saved;
  if (value > 0) {
    // Expand the value using UTF-8 encoding:
    //	bytes | bits | encoding
    //	    1 |    7 | 0vvvvvvv
    //	    2 |   11 | 110vvvvv 10vvvvvv
    //	    3 |   16 | 1110vvvv 10vvvvvv 10vvvvvv
    //	    4 |   21 | 11110vvv 10vvvvvv 10vvvvvv 10vvvvvv
    if (value < 0x80) {
      *start++ = (value);
    } else if (value < 0x800) {
      *start++ = 0xC0 | (value>>6);
      *start++ = 0x80 | (value & 0x3F);
    } else if (value < 0x10000) {
      *start++ = 0xE0 | (value>>12);
      *start++ = 0x80 | (value>>6 & 0x3F);
      *start++ = 0x80 | (value & 0x3F);
    } else if (value < 0x200000) {
      *start++ = 0xF0 | (value>>18);
      *start++ = 0x80 | (value>>12 & 0x3F);
      *start++ = 0x80 | (value>>6 & 0x3F);
      *start++ = 0x80 | (value & 0x3F);
    }
    Except_CHECK(start <= stop + 1);
    // Pad with '\0', that will be later squashed.
    while (start <= stop) *start++ = '\0';
    return true;
  }
  return false;
}

#ifdef $XCC_h
/**
 * Expand the character entities in text.
 */
size_t
XMLCan_expandEntities(XMLCan this, char *text, size_t size);
#endif//$XCC_h

size_t
XMLCan_expandEntities(XMLCan this, char *text, size_t size)
{
  char *start = NULL;
  int replaced = 0, i = 0;
  for (i = 0; i < size; i++) {
    char current = text[i];
    if (current == '&') {
      start = text + i;
    }
    if (current == ';') {
      if (start != NULL) {
        replaced += XMLCan_expandEntity(this, start, text + i);
      }
      start = NULL;
    }
  }
  if (replaced != 0) {
    char *dest = text;
    for (i = 0; i < size; i++) {
      char current = text[i];
      if (current != '\0') {
        *dest++ = current;
      }
    }
    return dest - text;
  }
  return size;
}

#ifdef $XCC__c
{
  char s0[] = "&#xc9;ditions Gallimard";
  char s1[] = "&lt; &apos; &quot; &gt;";
  char s2[] = "&unknown; &amp;amp; &deg;";
  XMLCan_ can[1];
  XMLCan_Ctor(can, NULL, XMLCan_lookupLatin1, XMLCanFlag_Trim);
  s0[XMLCan_expandEntities(can, s0, strlen(s0))] = '\0';
  s1[XMLCan_expandEntities(can, s1, strlen(s1))] = '\0';
  s2[XMLCan_expandEntities(can, s2, strlen(s2))] = '\0';
  printf("s0=\"%s\"\n", s0);
  printf("s1=\"%s\"\n", s1);
  printf("s2=\"%s\"\n", s2);
  XMLCan_Dtor(can);
}
#endif//$XCC__c

static void
XMLCan_normalizeCharData(XMLCan this)
{
  XMLCanFlags flags = XMLCan_FLAGS(this);
  if (flags & XMLCanFlag_Raw);
  else {
    char *text = XMLCan_TEXT(this);
    size_t size = XMLCan_SIZE(this);
    size = XMLCan_expandEntities(this, text, size);
    *XMLCan__SIZE(this) = size;
    text[size] = '\0';
  }
}

static void
XMLCan_normalizeAttValue(XMLCan this)
{
  XMLCanFlags flags = XMLCan_FLAGS(this);
  if (flags & XMLCanFlag_Raw);
  else {
    char *text = XMLCan_TEXT(this);
    size_t size = XMLCan_SIZE(this);
    size = XMLCan_normalizeWhiteSpace(this, text, size);
    size = XMLCan_expandEntities(this, text, size);
    *XMLCan__SIZE(this) = size;
    text[size] = '\0';
  }
}

static XMLCanEvent
XMLCan_getEvent_expect_STag(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  char *name = pointer;
  size_t size = 0;
  if (XMLCanIsNameStart(current)) {
    current = *++pointer;
    size++;
  }
  while (XMLCanIsNameChar(current)) {
    current = *++pointer;
    size++;
  }
  while (XMLCanIsWhiteSpace(current)) {
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current == '>') {
    name[size] = '\0';
    *XMLCan__NAME(this) = name;
    *XMLCan__SIZE(this) = size;
    *XMLCanBuffer__START(buffer) = pointer + 1;
    *XMLCan__EXPECT(this) = XMLCanEvent__;
    *XMLCan__EVENT(this) = XMLCanEvent_STag;
    *XMLCan__DEPTH(this) += 1;
    *XMLCan__LINE(this) = line;
    return XMLCan_EVENT(this);
  } else if (current != '\0') {
    name[size] = '\0';
    *XMLCan__SAVED(this) = current;
    *XMLCan__NAME(this) = name;
    *XMLCan__SIZE(this) = size;
    *XMLCanBuffer__START(buffer) = pointer;
    *XMLCan__EXPECT(this) = XMLCanEvent_AttName;
    *XMLCan__EVENT(this) = XMLCanEvent_STag;
    *XMLCan__LINE(this) = line;
    return XMLCan_EVENT(this);
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "invalid start tag";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

static XMLCanEvent
XMLCan_getEvent_expect_ETag(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  char *name = pointer;
  size_t size = 0;
  if (XMLCanIsNameStart(current)) {
    current = *++pointer;
    size++;
  }
  while (XMLCanIsNameChar(current)) {
    current = *++pointer;
    size++;
  }
  while (XMLCanIsWhiteSpace(current)) {
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current == '>') {
    name[size] = '\0';
    *XMLCan__NAME(this) = name;
    *XMLCan__SIZE(this) = size;
    *XMLCanBuffer__START(buffer) = pointer + 1;
    *XMLCan__EXPECT(this) = XMLCanEvent__;
    *XMLCan__EVENT(this) = XMLCanEvent_ETag;
    *XMLCan__DEPTH(this) -= 1;
    *XMLCan__LINE(this) = line;
    return XMLCan_EVENT(this);
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "invalid end tag";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

static XMLCanEvent
XMLCan_getEvent_expect_PI(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  char *text = pointer;
  while (current != '?' || pointer[1] != '>') {
    if (current == '\0') break;
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current != '\0') {
    *pointer = '\0';
    *XMLCan__TEXT(this) = text;
    *XMLCan__SIZE(this) = pointer - text;
    *XMLCanBuffer__START(buffer) = pointer + 2;
    *XMLCan__EXPECT(this) = XMLCanEvent__;
    *XMLCan__EVENT(this) = XMLCanEvent_PI;
    *XMLCan__LINE(this) = line;
    return XMLCan_EVENT(this);
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "invalid processing instruction";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

static XMLCanEvent
XMLCan_getEvent_expect_Comment(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  char *text = pointer;
  while (current != '-' || pointer[1] != '-' || pointer[2] != '>') {
    if (current == '\0') break;
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current != '\0') {
    *pointer = '\0';
    *XMLCan__TEXT(this) = text;
    *XMLCan__SIZE(this) = pointer - text;
    *XMLCanBuffer__START(buffer) = pointer + 3;
    *XMLCan__EXPECT(this) = XMLCanEvent__;
    *XMLCan__EVENT(this) = XMLCanEvent_Comment;
    *XMLCan__LINE(this) = line;
    return XMLCan_EVENT(this);
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "runaway comment";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

static XMLCanEvent
XMLCan_getEvent_expect_CDATA(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  char *text = pointer;
  while (current != ']' || pointer[1] != ']' || pointer[2] != '>') {
    if (current == '\0') break;
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current != '\0') {
    *pointer = '\0';
    *XMLCan__TEXT(this) = text;
    *XMLCan__SIZE(this) = pointer - text;
    *XMLCanBuffer__START(buffer) = pointer + 3;
    *XMLCan__EXPECT(this) = XMLCanEvent__;
    *XMLCan__EVENT(this) = XMLCanEvent_CDATA;
    *XMLCan__LINE(this) = line;
    return XMLCan_EVENT(this);
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "runaway CDATA section";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

static XMLCanEvent
XMLCan_getEvent_expect_DOCTYPE(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  while (XMLCanIsWhiteSpace(current)) {
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (XMLCanIsNameStart(current)) {
    // Parse the DOCTYPE name.
    char *name = pointer;
    size_t size = 1;
    current = *++pointer;
    while (XMLCanIsNameChar(current)) {
      current = *++pointer;
      size++;
    }
    if (current != '\0') {
      *pointer = '\0';
      *XMLCan__NAME(this) = name;
      *XMLCan__SIZE(this) = size;
      *XMLCanBuffer__START(buffer) = pointer + 1;
      *XMLCan__EXPECT(this) = XMLCanEvent_DTD;
      *XMLCan__EVENT(this) = XMLCanEvent_DOCTYPE;
      *XMLCan__LINE(this) = line;
      return XMLCan_EVENT(this);
    }
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "invalid DOCTYPE declaration";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

static XMLCanEvent
XMLCan_getEvent_expect_CharData(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  char *text = pointer;
  while (current != '<') {
    if (current == '\0') break;
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current == '<' || !XMLCanBuffer_refill(buffer)) {
    *pointer = '\0';
    *XMLCan__SAVED(this) = current;
    *XMLCan__TEXT(this) = text;
    *XMLCan__SIZE(this) = pointer - text;
    *XMLCanBuffer__START(buffer) = pointer;
    *XMLCan__EXPECT(this) = XMLCanEvent__;
    *XMLCan__EVENT(this) = XMLCanEvent_CharData;
    XMLCan_normalizeCharData(this);
    *XMLCan__LINE(this) = line;
    return XMLCan_EVENT(this);
  }
  return XMLCan_getEvent(this);
}

static XMLCanEvent
XMLCan_getEvent_expectDTD(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  while (XMLCanIsWhiteSpace(current)) {
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current == '>') {
    // End of DTD declaration.
    *XMLCanBuffer__START(buffer) = pointer + 1;
    *XMLCan__EXPECT(this) = XMLCanEvent__;
    *XMLCan__LINE(this) = line;
    return XMLCan_getEvent(this);
  } else {
    int i = 0;
    for (i = 0; i < 6; i++) {
      if (current == '\0') break;
      current = *++pointer;
    }
    if (!strncmp(pointer - 6, "PUBLIC", 6)) {
      // Fetch the public ID.
      *XMLCanBuffer__START(buffer) = pointer + 1;
      *XMLCan__EXPECT(this) = XMLCanEvent_PublicID;
      *XMLCan__LINE(this) = line;
      return XMLCan_getEvent(this);
    }
    if (!strncmp(pointer - 6, "SYSTEM", 6)) {
      // Fetch the system ID.
      *XMLCanBuffer__START(buffer) = pointer + 1;
      *XMLCan__EXPECT(this) = XMLCanEvent_SystemID;
      *XMLCan__LINE(this) = line;
      return XMLCan_getEvent(this);
    }
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "invalid DTD declaration";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

static XMLCanEvent
XMLCan_getEvent_expectPublicID(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  while (XMLCanIsWhiteSpace(current)) {
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current == '\'' || current == '"') {
    char quote = current;
    char *text = pointer + 1;
    current = *++pointer;
    while (current != quote) {
      if (current == '\0') break;
      line += XMLCanIsNewLineChar(current);
      current = *++pointer;
    }
    if (current == quote) {
      *pointer = '\0';
      *XMLCan__QUOTE(this) = quote;
      *XMLCan__TEXT(this) = text;
      *XMLCan__SIZE(this) = pointer - text;
      *XMLCanBuffer__START(buffer) = pointer + 1;
      *XMLCan__EXPECT(this) = XMLCanEvent_SystemID;
      *XMLCan__EVENT(this) = XMLCanEvent_PublicID;
      XMLCan_normalizeAttValue(this);
      *XMLCan__LINE(this) = line;
      return XMLCan_EVENT(this);
    }
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "invalid public ID";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

static XMLCanEvent
XMLCan_getEvent_expectSystemID(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  while (XMLCanIsWhiteSpace(current)) {
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current == '>') {
    if (XMLCan_FLAGS(this) & XMLCanFlag_HTML) {
      // HTML with missing SystemID.
      *XMLCanBuffer__START(buffer) = pointer + 1;
      *XMLCan__EXPECT(this) = XMLCanEvent__;
      *XMLCan__LINE(this) = line;
      return XMLCan_getEvent(this);
    }
  } else if (current == '\'' || current == '"') {
    char quote = current;
    char *text = pointer + 1;
    current = *++pointer;
    while (current != quote) {
      if (current == '\0') break;
      line += XMLCanIsNewLineChar(current);
      current = *++pointer;
    }
    if (current == quote) {
      *pointer = '\0';
      *XMLCan__QUOTE(this) = quote;
      *XMLCan__TEXT(this) = text;
      *XMLCan__SIZE(this) = pointer - text;
      *XMLCanBuffer__START(buffer) = pointer + 1;
      *XMLCan__EXPECT(this) = XMLCanEvent_DTD;
      *XMLCan__EVENT(this) = XMLCanEvent_SystemID;
      XMLCan_normalizeAttValue(this);
      *XMLCan__LINE(this) = line;
      return XMLCan_EVENT(this);
    }
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "invalid system ID";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

static XMLCanEvent
XMLCan_getEvent_expectAttName(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  while (XMLCanIsWhiteSpace(current)) {
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current == '>') {
    // End of start tag: parse next input.
    *XMLCanBuffer__START(buffer) = pointer + 1;
    *XMLCan__EXPECT(this) = XMLCanEvent__;
    *XMLCan__DEPTH(this) += 1;
    *XMLCan__LINE(this) = line;
    return XMLCan_getEvent(this);
  } else if (current == '/') {
    current = *++pointer;
    if (current == '>') {
      // Empty tag: post ETag event.
      *XMLCanBuffer__START(buffer) = pointer + 1;
      *XMLCan__EXPECT(this) = XMLCanEvent__;
      *XMLCan__EVENT(this) = XMLCanEvent_ETag;
      *XMLCan__LINE(this) = line;
      return XMLCan_EVENT(this);
    }
  } else if (XMLCanIsNameStart(current)) {
    // Attribute name.
    char *name = pointer;
    size_t size = 1;
    current = *++pointer;
    while (XMLCanIsNameChar(current)) {
      current = *++pointer;
      size++;
    }
    while (XMLCanIsWhiteSpace(current)) {
      line += XMLCanIsNewLineChar(current);
      current = *++pointer;
    }
    if (current == '=') {
      name[size] = '\0';
      *XMLCan__NAME(this) = name;
      *XMLCan__SIZE(this) = size;
      *XMLCanBuffer__START(buffer) = pointer + 1;
      *XMLCan__EXPECT(this) = XMLCanEvent_AttValue;
      *XMLCan__EVENT(this) = XMLCanEvent_AttName;
      *XMLCan__LINE(this) = line;
      return XMLCan_EVENT(this);
    } else if (current != '\0') {
      if (XMLCan_FLAGS(this) & XMLCanFlag_HTML) {
        // HTML missing attribute value.
        name[size] = '\0';
        *XMLCan__SAVED(this) = current;
        *XMLCan__NAME(this) = name;
        *XMLCan__SIZE(this) = size;
        *XMLCanBuffer__START(buffer) = pointer;
        *XMLCan__EXPECT(this) = XMLCanEvent_AttName;
        *XMLCan__EVENT(this) = XMLCanEvent_AttName;
        *XMLCan__LINE(this) = line;
        return XMLCan_EVENT(this);
      }
    }
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "expecting attribute name";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

static XMLCanEvent
XMLCan_getEvent_expectAttValue(XMLCan this, char *pointer, char current)
{
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  int32_t line = XMLCan_LINE(this);
  while (XMLCanIsWhiteSpace(current)) {
    line += XMLCanIsNewLineChar(current);
    current = *++pointer;
  }
  if (current == '\'' || current == '"') {
    char quote = current;
    char *text = pointer + 1;
    current = *++pointer;
    while (current != quote) {
      if (current == '\0') break;
      line += XMLCanIsNewLineChar(current);
      current = *++pointer;
    }
    if (current == quote) {
      *pointer = '\0';
      *XMLCan__QUOTE(this) = quote;
      *XMLCan__TEXT(this) = text;
      *XMLCan__SIZE(this) = pointer - text;
      *XMLCanBuffer__START(buffer) = pointer + 1;
      *XMLCan__EXPECT(this) = XMLCanEvent_AttName;
      *XMLCan__EVENT(this) = XMLCanEvent_AttValue;
      XMLCan_normalizeAttValue(this);
      *XMLCan__LINE(this) = line;
      return XMLCan_EVENT(this);
    }
  } else if (current != '\0') {
    if (XMLCan_FLAGS(this) & XMLCanFlag_HTML) {
      // HTML unquoted attribute value.
      char *text = pointer;
      while (XMLCanIsNameChar(current)) {
        if (current == '\0') break;
        current = *++pointer;
      }
      if (current != '\0') {
        *pointer = '\0';
        *XMLCan__QUOTE(this) = '"';
        *XMLCan__SAVED(this) = current;
        *XMLCan__TEXT(this) = text;
        *XMLCan__SIZE(this) = pointer - text;
        *XMLCanBuffer__START(buffer) = pointer;
        *XMLCan__EXPECT(this) = XMLCanEvent_AttName;
        *XMLCan__EVENT(this) = XMLCanEvent_AttValue;
        *XMLCan__LINE(this) = line;
        return XMLCan_EVENT(this);
      }
    }
  }
  if (current == '\0') {
    // Refill buffer and try again.
    if (XMLCanBuffer_refill(buffer)) {
      return XMLCan_getEvent(this);
    }
  }
  *XMLCan__ERROR(this) = "expecting attribute value";
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__LINE(this) = line;
  return XMLCan_EVENT(this);
}

#ifdef $XCC_h
/**
 * Get the next XMLCanEvent.
 */
XMLCanEvent
XMLCan_getEvent(XMLCan this);
#endif//$XCC_h

XMLCanEvent
XMLCan_getEvent(XMLCan this)
{
  XMLCanEvent expect = XMLCan_EXPECT(this);
  XMLCanBuffer buffer = XMLCan_BUFFER(this);
  char *pointer = XMLCanBuffer_START(buffer);
  char saved = XMLCan_SAVED(this);
  char current = *pointer;
  if (saved != '\0') {
    *pointer = current = saved;
  }
  Except_DEBUG(XMLCanBuffer_check(buffer));
  *XMLCan__SAVED(this) = '\0';
  *XMLCan__QUOTE(this) = '\0';
  *XMLCan__ERROR(this) = NULL;
  *XMLCan__NAME(this) = NULL;
  *XMLCan__TEXT(this) = NULL;
  *XMLCan__SIZE(this) = 0;
  if (expect == XMLCanEvent__) {
    if (current == '\0') {
      // Refill buffer and try again.
      if (XMLCanBuffer_refill(buffer)) {
        return XMLCan_getEvent(this);
      } else {
        *XMLCan__EVENT(this) = XMLCanEvent_End;
        return XMLCan_EVENT(this);
      }
    } else if (current == '<') {
      // Start of markup of some kind.
      current = *++pointer;
      if (XMLCanIsNameStart(current)) {
        // Start tag: parse the element name.
        return XMLCan_getEvent_expect_STag(this, pointer, current);
      } else if (current == '/') {
        // End tag: parse the element name.
        current = *++pointer;
        return XMLCan_getEvent_expect_ETag(this, pointer, current);
      } else if (current == '?') {
        // XML declaration or processing instruction.
        current = *++pointer;
        return XMLCan_getEvent_expect_PI(this, pointer, current);
      } else if (current == '!') {
        current = *++pointer;
        if (current == '-') {
          current = *++pointer;
          if (current == '-') {
            current = *++pointer;
            return XMLCan_getEvent_expect_Comment(this, pointer, current);
          }
        } else {
          int i = 0;
          for (i = 0; i < 7; i++) {
            if (current == '\0') break;
            current = *++pointer;
          }
          if (!strncmp(pointer - 7, "[CDATA[", 7)) {
            // CDATA Section.
            return XMLCan_getEvent_expect_CDATA(this, pointer, current);
          }
          if (!strncmp(pointer - 7, "DOCTYPE", 7)) {
            // DOCTYPE declaration.
            return XMLCan_getEvent_expect_DOCTYPE(this, pointer, current);
          }
        }
      }
      if (current == '\0') {
        // Refill buffer and try again.
        if (XMLCanBuffer_refill(buffer)) {
          return XMLCan_getEvent(this);
        }
      }
      *XMLCan__EXPECT(this) = XMLCanEvent_Error;
      *XMLCan__EVENT(this) = XMLCanEvent_Error;
      *XMLCan__ERROR(this) = "invalid markup";
      return XMLCan_EVENT(this);
    } else {
      // Character Data.
      return XMLCan_getEvent_expect_CharData(this, pointer, current);
    }
  } else if (expect == XMLCanEvent_DTD) {
    return XMLCan_getEvent_expectDTD(this, pointer, current);
  } else if (expect == XMLCanEvent_PublicID) {
    return XMLCan_getEvent_expectPublicID(this, pointer, current);
  } else if (expect == XMLCanEvent_SystemID) {
    return XMLCan_getEvent_expectSystemID(this, pointer, current);
  } else if (expect == XMLCanEvent_AttName) {
    return XMLCan_getEvent_expectAttName(this, pointer, current);
  } else if (expect == XMLCanEvent_AttValue) {
    return XMLCan_getEvent_expectAttValue(this, pointer, current);
  }
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__ERROR(this) = "invalid state";
  return XMLCan_EVENT(this);
}

#ifdef $XCC_h
/**
 * The current XMLCanEvent.
 */
XMLCanEvent
XMLCan_event(XMLCan this);
#endif//$XCC_h

XMLCanEvent
XMLCan_event(XMLCan this)
{
  return XMLCan_EVENT(this);
}

#ifdef $XCC_h
/**
 * Current input line number.
 */
int32_t
XMLCan_line(XMLCan this);
#endif//$XCC_h

int32_t
XMLCan_line(XMLCan this)
{
  return XMLCan_LINE(this);
}

#ifdef $XCC_h
/**
 * Error message in case of parsing error.
 */
const char *
XMLCan_error(XMLCan this);
#endif//$XCC_h

const char *
XMLCan_error(XMLCan this)
{
  return XMLCan_ERROR(this);
}

#ifdef $XCC_h
/**
 * Set the error state and message.
 */
void
XMLCan_setError(XMLCan this, const char *error);
#endif//$XCC_h

void
XMLCan_setError(XMLCan this, const char *error)
{
  *XMLCan__EXPECT(this) = XMLCanEvent_Error;
  *XMLCan__EVENT(this) = XMLCanEvent_Error;
  *XMLCan__ERROR(this) = error;
}

#ifdef $XCC_h
/**
 * Quote character used for quoted strings.
 */
char
XMLCan_quote(XMLCan this);
#endif//$XCC_h

char
XMLCan_quote(XMLCan this)
{
  return XMLCan_QUOTE(this);
}

#ifdef $XCC_h
/**
 * Name of an element or an attribute.
 *
 * This is defined for the STag, AttName, and ETag events.
 * In case of parsing an empty element, the ETag name is zero-length.
 */
const char *
XMLCan_name(XMLCan this);
#endif//$XCC_h

const char *
XMLCan_name(XMLCan this)
{
  return XMLCan_NAME(this);
}

#ifdef $XCC_h
/**
 * Text of element content or attribute value.
 *
 * This is defined for the CharData, and AttValue events.
 */
const char *
XMLCan_text(XMLCan this);
#endif//$XCC_h

const char *
XMLCan_text(XMLCan this)
{
  return XMLCan_TEXT(this);
}

#ifdef $XCC_h
/**
 * Size of the XMLCan_name or XMLCan_text.
 *
 * This is defined for the STag, AttName, AttValue, and
 * ETag events.
 */
size_t
XMLCan_size(XMLCan this);
#endif//$XCC_h

size_t
XMLCan_size(XMLCan this)
{
  return XMLCan_SIZE(this);
}

#ifdef $XCC_h
/**
 * Depth of the current XMLCanEvent.
 *
 * The depth is incremented by STag, and decremented by ETag.
 */
int32_t
XMLCan_depth(XMLCan this);
#endif//$XCC_h

int32_t
XMLCan_depth(XMLCan this)
{
  return XMLCan_DEPTH(this);
}

#ifdef $XCC_h
/**
 * Expand a Latin1 character entity.
 */
int
XMLCanLatin1(const char *name);
#endif//$XCC_h

int
XMLCanLatin1(const char *name)
{
  static struct {
    const char *NAME;
    intptr_t VALUE;
  } XMLCan_Latin1[] = {
    { "AElig",	198 },
    { "Aacute",	193 },
    { "Acirc",	194 },
    { "Agrave",	192 },
    { "Aring",	197 },
    { "Atilde",	195 },
    { "Auml",	196 },
    { "Ccedil",	199 },
    { "ETH",	208 },
    { "Eacute",	201 },
    { "Ecirc",	202 },
    { "Egrave",	200 },
    { "Euml",	203 },
    { "Iacute",	205 },
    { "Icirc",	206 },
    { "Igrave",	204 },
    { "Iuml",	207 },
    { "Ntilde",	209 },
    { "Oacute",	211 },
    { "Ocirc",	212 },
    { "Ograve",	210 },
    { "Oslash",	216 },
    { "Otilde",	213 },
    { "Ouml",	214 },
    { "THORN",	222 },
    { "Uacute",	218 },
    { "Ucirc",	219 },
    { "Ugrave",	217 },
    { "Uuml",	220 },
    { "Yacute",	221 },
    { "aacute",	225 },
    { "acirc",	226 },
    { "acute",	180 },
    { "aelig",	230 },
    { "agrave",	224 },
    { "aring",	229 },
    { "atilde",	227 },
    { "auml",	228 },
    { "brvbar",	166 },
    { "ccedil",	231 },
    { "cedil",	184 },
    { "cent",	162 },
    { "copy",	169 },
    { "curren",	164 },
    { "deg",	176 },
    { "divide",	247 },
    { "eacute",	233 },
    { "ecirc",	234 },
    { "egrave",	232 },
    { "eth",	240 },
    { "euml",	235 },
    { "frac12",	189 },
    { "frac14",	188 },
    { "frac34",	190 },
    { "iacute",	237 },
    { "icirc",	238 },
    { "iexcl",	161 },
    { "igrave",	236 },
    { "iquest",	191 },
    { "iuml",	239 },
    { "laquo",	171 },
    { "macr",	175 },
    { "micro",	181 },
    { "middot",	183 },
    { "nbsp",	160 },
    { "not",	172 },
    { "ntilde",	241 },
    { "oacute",	243 },
    { "ocirc",	244 },
    { "ograve",	242 },
    { "ordf",	170 },
    { "ordm",	186 },
    { "oslash",	248 },
    { "otilde",	245 },
    { "ouml",	246 },
    { "para",	182 },
    { "plusmn",	177 },
    { "pound",	163 },
    { "raquo",	187 },
    { "reg",	174 },
    { "sect",	167 },
    { "shy",	173 },
    { "sup1",	185 },
    { "sup2",	178 },
    { "sup3",	179 },
    { "szlig",	223 },
    { "thorn",	254 },
    { "times",	215 },
    { "uacute",	250 },
    { "ucirc",	251 },
    { "ugrave",	249 },
    { "uml",	168 },
    { "uuml",	252 },
    { "yacute",	253 },
    { "yen",	165 },
    { "yuml",	255 },
  };
  int32_t count = sizeof(XMLCan_Latin1)/sizeof(XMLCan_Latin1[0]);
  int32_t l = 0, r = count - 1;
  while (r >= l) {
    int32_t i = (l + r) >> 1;
    int cmp = strcmp(name, XMLCan_Latin1[i].NAME);
    if (cmp < 0) r = i - 1;
    else if (cmp > 0) l = i + 1;
    else return XMLCan_Latin1[i].VALUE;
  }
  return -1;
}

#ifdef $XCC_h
/**
 * Lookup a Latin1 character entity.
 */
int
XMLCan_lookupLatin1(XMLCan this, const char *name);
#endif//$XCC_h

int
XMLCan_lookupLatin1(XMLCan this, const char *name)
{
  int value = XMLCanLatin1(name);
  if (value <= 0) {
    XMLCan_setError(this, "unknown Latin1 entity");
  }
  return value;
}

#ifdef $XCC__c
{
  int32_t depth;
  XMLCan_ can[1];
  XMLCanBuffer_ buffer[1];
  XMLCanBuffer_Ctor(buffer, Memory_Root, stdin, 1000);
  XMLCanBuffer_prefill(buffer, "<prefill where='here' />\n");
  XMLCan_Ctor(can, buffer, XMLCan_lookupLatin1, XMLCanFlag_HTML);
  depth = XMLCan_depth(can);
  while (XMLCan_getEvent(can) != XMLCanEvent_End) {
    char quote = XMLCan_quote(can);
    Except_DEBUG(XMLCanBuffer_check(buffer));
    if (XMLCan_eventComment(can)) {
      printf("<!--%s-->", XMLCan_text(can));
    }
    if (XMLCan_eventDOCTYPE(can)) {
      printf("<!DOCTYPE %s", XMLCan_name(can));
    }
    if (XMLCan_eventPublicID(can)) {
      printf(" PUBLIC %c%s%c", quote, XMLCan_text(can), quote);
    }
    if (XMLCan_eventSystemID(can)) {
      printf(" SYSTEM %c%s%c>", quote, XMLCan_text(can), quote);
    }
    if (XMLCan_eventSTag(can)) {
      printf("<%s", XMLCan_name(can));
    }
    if (XMLCan_eventAttName(can)) {
      printf(" %s=", XMLCan_name(can));
    }
    if (XMLCan_eventAttValue(can)) {
      printf("%c%s%c", quote, XMLCan_text(can), quote);
    }
    if (depth < XMLCan_depth(can)) {
      printf(">");
    }
    if (XMLCan_eventCharData(can)) {
      printf("%s", XMLCan_text(can));
    }
    if (XMLCan_eventCDATA(can)) {
      printf("<![CDATA[%s]]>", XMLCan_text(can));
    }
    if (XMLCan_eventETag(can)) {
      if (XMLCan_name(can)) {
        printf("</%s>", XMLCan_name(can));
      } else {
        printf("/>");
      }
    }
    if (XMLCan_eventError(can)) {
      fprintf(stderr, "\nError line %d: %s\n",
          XMLCan_line(can), XMLCan_error(can));
      exit(1);
    }
    depth = XMLCan_depth(can);
  }
  XMLCan_Dtor(can);
  XMLCanBuffer_Dtor(buffer);
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the XMLCan module.
 */
#define XMLCan_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the XMLCan module.
 */
#define XMLCan_FINI()
#endif//$XCC__h

#if XCC__C
static void
XMLCan_TEST(void)
{
#include "BSL/XMLCan_.c"
}

int
main(int argc, char **argv)
{
  BSL_Test(XMLCan_TEST);
  return 0;
}
#endif

