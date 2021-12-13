#ifdef $XCC_h
/*
 * !!!!	Processor.xcc
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
#include "MDT/Processor.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Processor(ID, MINTAKEN, INTERLOCKS, AVAILABILITY) \
  Processor_##ID,
  Processor_,
#include /*MDT*/"Processor.tuple"
  Processor__
#undef Processor
} enum_Processor;
#endif//$XCC__h

const int Processor__Count = Processor__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Processor enumeration.
 */
extern const char *
Processor_Id(Processor this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Processor_Id(Processor this)
{
  static const char *_[] = {
#define Processor(ID, MINTAKEN, INTERLOCKS, AVAILABILITY) \
    #ID,
    "_UNDEF",
#include "Processor.tuple"
  };
  Except_REQUIRE((unsigned)this < Processor__);
  return _[this];
}
#endif//_NTRACE


#ifdef $XCC_h
/**
 * Find a processor number by name, return 0 if none found
 */
Processor
Processor_fromName(const char *cpu);
#endif//$XCC_h

Processor
Processor_fromName(const char *cpu)
{
  size_t length = strlen(cpu);
  // start from 1 because 0 is UNDEF
  for (int processor = 1; processor < Processor__; ++processor)
  {
    const char *id = Processor_Id(processor);
    if (strlen(id) < length) continue;
    if (strncmp(id, cpu, length) == 0) {
      return processor;
    }
  }
  return 0;
}

#ifdef $XCC_h
/**
 * Get a processor name
 */
const char*
Processor_getName(Processor this);
#endif//$XCC_h

const char*
Processor_getName(Processor this)
{
  Except_REQUIRE((unsigned)this < Processor__);
  return Processor_Id(this);
}

#ifdef $XCC_h
void
Processor_printName(Processor this, FILE *file);
#endif//$XCC_h

void
Processor_printName(Processor this, FILE *file)
{
  char *ptr;
  const char *name;
  Except_REQUIRE((unsigned)this < Processor__);
  name = Processor_Id(this);
  if ((ptr = strstr(name, "_cpu")) == NULL)
  {
    fprintf(file, "%s", name);
  }
  else
  {
    unsigned length = (unsigned)(ptr-name);
    for (unsigned i=0; i<length; ++i)
      fputc(*(name+i), file);
  }
}

#ifdef $XCC__c
{
  int processor = 0;
  for (; processor < Processor__; processor++) {
    printf("Processor_%s\t%d\n", Processor_Id(processor), processor);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Processor minTaken.
 */
int
Processor_minTaken(Processor this);
#endif//$XCC_h

extern /*const*/ int8_t
Processor_MinTaken[];
int
Processor_minTaken(Processor this)
{
  Except_REQUIRE((unsigned)this < Processor__);
  return Processor_MinTaken[this];
}

/*const*/ int8_t
Processor_MinTaken[] = {
#define MINTAKEN(d) d
#define Processor(ID, MINTAKEN, INTERLOCKS, AVAILABILITY) \
  MINTAKEN,
  MINTAKEN(0),
#include "Processor.tuple"
};

#ifdef $XCC__h
/**
 * Change the minimum branch taken penalty.
 */
void
Processor_setMinTaken(Processor this, int minTaken);
#endif//$XCC__h

void
Processor_setMinTaken(Processor this, int minTaken)
{
  Except_REQUIRE((unsigned)this < Processor__);
  Processor_MinTaken[this] = minTaken;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This processor maximum branch taken cost.
 */
int
Processor_maxTakenCost(Processor this);
#endif//$XCC_h

int
Processor_maxTakenCost(Processor this)
{
  int maxTaken = 1;
  int maxIssue = Resource_availability(Resource__ISSUE, this);
  return maxIssue*maxTaken;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Whether this Processor interlocks registers.
 */
bool
Processor_interlocks(Processor this);
#endif//$XCC_h

extern const bool
Processor_Interlocks[];
bool
Processor_interlocks(Processor this)
{
  Except_REQUIRE((unsigned)this < Processor__);
  return Processor_Interlocks[this];
}

const bool
Processor_Interlocks[] = {
#define INTERLOCKS(b) b
#define Processor(ID, MINTAKEN, INTERLOCKS, AVAILABILITY) \
  INTERLOCKS,
  INTERLOCKS(0),
#include "Processor.tuple"
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The minimum RAW latency on this Processor.
 */
int
Processor_minTheta(Processor this);
#endif//$XCC_h

int
Processor_minTheta(Processor this)
{
  int minTheta = -1;
  Except_REQUIRE((unsigned)this < Processor__);
#ifdef MDT_TARGET_st200
  minTheta = 0;
#endif//MDT_TARGET_st200
#ifdef MDT_TARGET_k1b
  minTheta = 1;
#endif//MDT_TARGET_k1b
  Except_REQUIRE(minTheta >= 0);
  return minTheta;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Processor availability of Resource(s).
 */
const_ResourceVector
Processor_availability(Processor this);
#endif//$XCC_h

extern /*const*/ ResourceVector_
Processor_Availability[];
const_ResourceVector
Processor_availability(Processor this)
{
  Except_REQUIRE((unsigned)this < Processor__);
  return Processor_Availability + this;
}

/*const*/ ResourceVector_
Processor_Availability[] = {
#define AVAILABLE(resource, units) units,
#define AVAILABILITY(items) { { items } }
#define Processor(ID, MINTAKEN, INTERLOCKS, AVAILABILITY) \
  AVAILABILITY,
  AVAILABILITY(/**/),
#include "Processor.tuple"
#undef AVAILABILITY
};

#ifdef $XCC__c
{
  int processor, resource;
  printf("Resource\t");
  for (resource = 1; resource < Resource__; resource++) {
    printf("\t%s", Resource_Id(resource));
  }
  printf("\n");
  for (processor = 0; processor < Processor__; processor++) {
    printf("Availability(%s)", Processor_Id(processor));
    for (resource = 1; resource < Resource__; resource++) {
      int available = ResourceVector_available(Processor_Availability + processor, resource);
      printf("\t\t%d", available);
    }
    printf("\n");
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Change the maxIssue.
 */
void
Processor_setMaxIssue(Processor this, int maxIssue);
#endif//$XCC__h

void
Processor_setMaxIssue(Processor this, int maxIssue)
{
  int resource, scheduling, issue_resource = Resource__ISSUE;
  int issueWidth = Resource_availability(issue_resource, this);
  if (maxIssue == issueWidth) return;
  for (resource = 1; resource < Resource__; resource++) {
    int available = Resource_availability(resource, this);
    int newAvailable = _MAX((available*maxIssue)/issueWidth, 1);
    *ResourceVector__available(Processor_Availability + this, resource) = newAvailable;
    for (scheduling = 0; scheduling < Scheduling__; scheduling++) {
      if (Scheduling_processor(scheduling) == this) {
        int reservation = Scheduling_reservation(scheduling);
        ReservationTable table = Reservation_Table + reservation;
        ResourceVector_ *columns = ReservationTable_COLUMNS(table);
        int columnCount = ReservationTable_COLUMNCOUNT(table), i;
        for (i = 0; i < columnCount; i++) {
          ResourceVector column = columns + i;
          int required = ResourceVector_available(column, resource);
          if (required > newAvailable) {
            *ResourceVector__available(column, resource) = newAvailable;
          }
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Processor module.
 */
#define Processor_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Processor module.
 */
#define Processor_FINI()
#endif//$XCC__h

#if XCC__C
static void
Processor_TEST(void)
{
#include "MDT/Processor_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Processor_TEST);
  return 0;
}
#endif

