#ifdef $XCC_h
/*
 * !!!!	Scheduling.xcc
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
#include "MDT/Scheduling.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Scheduling(ID, PROCESSOR, BUNDLING, RESERVATION) \
  Scheduling_##ID,
  Scheduling_,
#include /*MDT*/"Scheduling.tuple"
  Scheduling__
#undef Scheduling
} enum_Scheduling;
#endif//$XCC__h

const int Scheduling__Count = Scheduling__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Scheduling enumeration.
 */
extern const char *
Scheduling_Id(Scheduling this);
extern const char *
Scheduling_name(Scheduling this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Scheduling_Id(Scheduling this)
{
  static const char *_[] = {
#define Scheduling(ID, PROCESSOR, BUNDLING, RESERVATION) \
    #ID,
    "_UNDEF",
#include "Scheduling.tuple"
  };
  Except_REQUIRE((unsigned)this < Scheduling__);
  return _[this];
}
const char *
Scheduling_name(Scheduling this)
{
  return this? strchr(Scheduling_Id(this) + sizeof(MDT_TARGET) - 1, '_') + 1: "_UNDEF";
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int scheduling = 0;
  for (; scheduling < Scheduling__; scheduling++) {
    printf("Scheduling_%s\t%d\n", Scheduling_Id(scheduling), scheduling);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Scheduling class processor.
 */
Processor
Scheduling_processor(Scheduling this);
#endif//$XCC_h

extern const Processor
Scheduling_Processor[];
Processor
Scheduling_processor(Scheduling this)
{
  Except_REQUIRE((unsigned)this < Scheduling__);
  return (Processor)Scheduling_Processor[this];
}

const Processor
Scheduling_Processor[] = {
#define PROCESSOR(ID) Processor_##ID
#define Scheduling(ID, PROCESSOR, BUNDLING, RESERVATION) \
  PROCESSOR,
  PROCESSOR(_UNDEF),
#include "Scheduling.tuple"
};

#ifdef $XCC__c
{
  int scheduling = 0;
  for (; scheduling < Scheduling__; scheduling++) {
    int processor = Scheduling_processor(scheduling);
    printf("Scheduling_processor(%s)\t%s\n", Scheduling_Id(scheduling), Processor_Id(processor));
  }
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Scheduling class Bundling class.
 */
Bundling
Scheduling_bundling(Scheduling this);
#endif//$XCC_h

extern const Bundling
Scheduling_Bundling[];
Bundling
Scheduling_bundling(Scheduling this)
{
  Except_REQUIRE((unsigned)this < Scheduling__);
  return (Bundling)Scheduling_Bundling[this];
}

const Bundling
Scheduling_Bundling[] = {
#define BUNDLING(ID) Bundling_##ID
#define Scheduling(ID, PROCESSOR, BUNDLING, RESERVATION) \
  BUNDLING,
  BUNDLING(_UNDEF),
#include "Scheduling.tuple"
};

#ifdef $XCC__c
{
  int scheduling = 0;
  for (; scheduling < Scheduling__; scheduling++) {
    int bundling = Scheduling_bundling(scheduling);
    printf("Bundling(%s)\t%s\n", Scheduling_Id(scheduling), Bundling_Id(bundling));
  }
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This Scheduling class Reservation.
 */
Reservation
Scheduling_reservation(Scheduling this);
#endif//$XCC_h

extern const Reservation
Scheduling_Reservation[];
Reservation
Scheduling_reservation(Scheduling this)
{
  Except_REQUIRE((unsigned)this < Scheduling__);
  return (Reservation)Scheduling_Reservation[this];
}

const Reservation
Scheduling_Reservation[] = {
#define RESERVATION(ID) Reservation_##ID
#define Scheduling(ID, PROCESSOR, BUNDLING, RESERVATION) \
  RESERVATION,
  RESERVATION(_UNDEF),
#include "Scheduling.tuple"
};

#ifdef $XCC__c
{
  int scheduling = 0;
  for (; scheduling < Scheduling__; scheduling++) {
    int reservation = Scheduling_reservation(scheduling);
    printf("Reservation(%s)\t%s\n",
           Scheduling_Id(scheduling), Reservation_Id(reservation));
  }
  printf("\n");
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Scheduling module.
 */
#define Scheduling_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Scheduling module.
 */
#define Scheduling_FINI()
#endif//$XCC__h

#if XCC__C
static void
Scheduling_TEST(void)
{
#include "MDT/Scheduling_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Scheduling_TEST);
  return 0;
}
#endif

