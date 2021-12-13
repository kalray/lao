#ifdef $XCC_h
/*
 * !!!!	Reservation.xcc
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
#include "MDT/Reservation.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Reservation(ID, TABLE) \
  Reservation_##ID,
  Reservation_,
#include /*MDT*/"Reservation.tuple"
  Reservation__
#undef Reservation
} enum_Reservation;
#endif//$XCC__h

const int Reservation__Count = Reservation__;
const int Reservation__MaxColumnsCount = Reservation_COLUMNS_MAX;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Reservation table.
 */
struct ReservationTable_ {
  int COLUMNCOUNT;
  ResourceVector_ *COLUMNS;
};
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Count of columns of this ReservationTable.
 */
static inline int
ReservationTable_columnCount(const_ReservationTable this)
{
  return ReservationTable_COLUMNCOUNT(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Array of columns of this ReservationTable.
 */
static inline const ResourceVector_ *
ReservationTable_columns(const_ReservationTable this)
{
  return ReservationTable_COLUMNS(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Reservation enumeration.
 */
extern const char *
Reservation_Id(Reservation this);
extern const char *
Reservation_name(Reservation this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Reservation_Id(Reservation this)
{
  static const char *_[] = {
#define Reservation(ID, TABLE) \
    #ID,
    "_UNDEF",
#include "Reservation.tuple"
  };
  Except_REQUIRE((unsigned)this < Reservation__);
  return _[this];
}
const char *
Reservation_name(Reservation this)
{
  return this? strchr(Reservation_Id(this) + sizeof(MDT_TARGET) - 1, '_') + 1: "_UNDEF";
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int reservation = 0;
  for (; reservation < Reservation__; reservation++) {
    printf("Reservation_%s\t%d\n", Reservation_Id(reservation), reservation);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

/*const*/ ResourceVector_
Reservation_Columns[] = {
#define REQUIRED(resource, units) units,
#define REQUIREMENTS(items) { { items } }
#define ReservationColumn(COLUMNINDEX, REQUIREMENTS) \
  REQUIREMENTS,
  // No dummy entry because only used as Reservation_Columns + columnIndex.
#include "Reservation.tuple"
#undef REQUIREMENTS
};

// Zero ResourceVector for use by ReservationTable of Reservation_.
/*const*/ ResourceVector_
Reservation_Columns__ZERO;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Access to this Reservation class ReservationTable
 */
const_ReservationTable
Reservation_table(Reservation this);
#endif//$XCC_h

#ifdef $XCC__h
extern /*const*/ ReservationTable_
Reservation_Table[];
#endif//$XCC__h

const_ReservationTable
Reservation_table(Reservation this)
{
  Except_REQUIRE((unsigned)this < Reservation__);
  return Reservation_Table + this;
}

/*const*/ ReservationTable_
Reservation_Table[] = {
#define COLUMNCOUNT(d) d
#define COLUMNINDEX(d) d
#define TABLE(columnCount, columnIndex) \
  { columnCount, Reservation_Columns + columnIndex }
#define Reservation(ID, TABLE) \
  TABLE,
  { 0, &Reservation_Columns__ZERO },
#include "Reservation.tuple"
};

#ifdef $XCC_h
/**
 * The Resource requirements of this Reservation.
 */
static inline const_ResourceVector
Reservation_requirements(Reservation this)
{
  const_ReservationTable table = Reservation_table(this);
  return ReservationTable_COLUMNS(table);
}
#endif//$XCC_h

#ifdef $XCC__c
{
  int resource, reservation;
  printf("\nRequirements\t");
  for (resource = 1; resource < Resource__; resource++) {
    printf("\t%s", Resource_Id(resource));
  }
  printf("\n");
  for (reservation = 0; reservation < Reservation__; reservation++) {
    const_ResourceVector requirements = Reservation_requirements(reservation);
    printf("Reservation(%s)", Reservation_Id(reservation));
    for (resource = 1; resource < Resource__; resource++) {
      printf("\t\t%d", ResourceVector_available(requirements, resource));
    }
    printf("\n");
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Reservation_criticality
 */
unsigned
Reservation_criticality(Reservation this);
#endif//$XCC_h

unsigned
Reservation_criticality(Reservation this)
{
#ifdef MDT_TARGET_st200
  switch (this) {
  case Reservation_st200_ALL:	return 8;
  case Reservation_st200_ALU:	return 1;
  case Reservation_st200_ALUX:	return 2;
  case Reservation_st200_CTL:	return 4;
  case Reservation_st200_ODD:	return 3;
  case Reservation_st200_ODDX:	return 4;
  case Reservation_st200_MEM:	return 5;
  case Reservation_st200_MEMX:	return 6;
  case Reservation_st200_PSW:	return 2;
  case Reservation_st200_EVEN:	return 3;
  case Reservation_st240_MEM:	return 5;
  case Reservation_st240_MEMX:	return 6;
  case Reservation_st240_EVEN:	return 3;
  default: return 0;
  }
#endif//MDT_TARGET_st200
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The minimum number of cycles between two Reservation classes.
 */
int
Reservation_odot(Reservation this, Reservation that, Processor processor);
#endif//$XCC_h

extern int8_t
Reservation_ODot[Reservation__][Reservation__][Processor__];
int
Reservation_odot(Reservation this, Reservation that, Processor processor)
{
  Except_REQUIRE((unsigned)this < Reservation__);
  Except_REQUIRE((unsigned)that < Reservation__);
  Except_REQUIRE((unsigned)processor < Processor__);
  return Reservation_ODot[this][that][processor];
}

int8_t Reservation_ODot[Reservation__][Reservation__][Processor__];

#ifdef $XCC__c
{
  int processor, this, that;
  for (processor = 0; processor < Processor__; processor++) {
    printf("\nProcessor_%s:\n", Processor_Id(processor));
    for (that = 0; that < Reservation__; that++) {
      printf("\t%s", Reservation_Id(that));
    }
    for (this = 0; this < Reservation__; this++) {
      printf("\n%s", Reservation_Id(this));
      for (that = 0; that < Reservation__; that++) {
        printf("\t%2d", Reservation_odot(this, that, processor));
      }
    }
    printf("\n");
  }
}
#endif//$XCC__c

#ifdef $XCC__h
/**
 * Initialize the contents of the Reservation_ODot array.
 */
void
Reservation_InitializeODot(void);
#endif//$XCC__h

void
Reservation_InitializeODot(void)
{
  int processor, this, that, resource;
  for (processor = 0; processor < Processor__; processor++) {
    for (this = 0; this < Reservation__; this++) {
      for (that = 0; that < Reservation__; that++) {
        for (resource = 1; resource < Resource__; resource++) {
          const_ResourceVector this_requirements = Reservation_requirements(this);
          const_ResourceVector that_requirements = Reservation_requirements(that);
          const_ResourceVector availability = Processor_availability(processor);
          int this_required = ResourceVector_available(this_requirements, resource);
          int that_required = ResourceVector_available(that_requirements, resource);
          int available = ResourceVector_available(availability, resource);
          if (this_required + that_required > available) {
            Reservation_ODot[this][that][processor] = 1;
          }
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Reservation module.
 */
void
Reservation_INIT(void);
#endif//$XCC__h

void
Reservation_INIT(void)
{
  Reservation_InitializeODot();
}

#ifdef $XCC__h
/**
 * Finalize the Reservation module.
 */
#define Reservation_FINI()
#endif//$XCC__h

#if XCC__C
static void
Reservation_TEST(void)
{
#include "MDT/Reservation_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Reservation_TEST);
  return 0;
}
#endif

