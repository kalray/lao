#ifdef $XCC_h
/*
 * !!!!	Resource.xcc
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
#include "MDT/Resource.h"
#endif//$XCC__h

#ifdef $XCC__h
typedef enum {
#define Resource(ID, AVAILABILITY) \
  Resource_##ID,
  Resource_,
#include /*MDT*/"Resource.tuple"
  Resource__
#undef Resource
} enum_Resource;
#endif//$XCC__h

const int Resource__Count = Resource__;

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/*
 * Pack a vector of Resource__ - 1 units in a word (exclude Resource_).
 */
union ResourceVector_ {
  uint8_t UNITS[2*sizeof(uint64_t)/sizeof(uint8_t)];
  uint64_t PACKED[2];
};
typedef union ResourceVector_ ResourceVector_, *ResourceVector;
typedef const union ResourceVector_ *const_ResourceVector;
typedef union ResourceVector_ *restrict restrict_ResourceVector;
#define ResourceVector_UNITS(this) ((this)->UNITS)
#define ResourceVector_PACKED(this) ((this)->PACKED)
#endif//$XCC_h

#ifdef $XCC__c
{
  Except_ALWAYS(Resource__ - 1 <= 2*sizeof(uint64_t)/sizeof(uint8_t));
}
#endif//$XCC__c

#ifdef $XCC_h
/**
 * Availability of Resource(s).
 */
Resource
ResourceVector_available(const_ResourceVector this, Resource resource);
#endif//$XCC_h

Resource
ResourceVector_available(const_ResourceVector this, Resource resource)
{
  Except_CHECK((unsigned)(resource - 1) < (unsigned)(Resource__Count - 1));
  return ResourceVector_UNITS(this)[resource - 1];
}

#ifdef $XCC_h
/**
 * Pointer to availability of Resource(s).
 */
uint8_t *
ResourceVector__available(ResourceVector this, Resource resource);
#endif//$XCC_h

uint8_t *
ResourceVector__available(ResourceVector this, Resource resource)
{
  Except_CHECK((unsigned)(resource - 1) < (unsigned)(Resource__Count - 1));
  return ResourceVector_UNITS(this) + resource - 1;
}

#ifdef $XCC__h
/**
 * Clear this ResourceVector.
 */
#ifdef NSIMDIZE
void
ResourceVector_clear(ResourceVector this);
#else
static inline void
ResourceVector_clear(ResourceVector this) {
  ResourceVector_PACKED(this)[0] = 0;
  ResourceVector_PACKED(this)[1] = 0;
}
#endif//NSIMDIZE
#endif//$XCC__h

#ifdef NSIMDIZE
void
ResourceVector_clear(ResourceVector this)
{
  int resource;
  for (resource = 1; resource < Resource__; resource++) {
    *ResourceVector__UNITS(this)[resource - 1] = 0;
  }
}
#endif//NSIMDIZE

#ifdef $XCC__h
/**
 * Accumulate @c that ResourceVector to @c this ResourceVector.
 */
#ifdef NSIMDIZE
void
ResourceVector_accumulate(ResourceVector this, const_ResourceVector that);
#else
static inline void
ResourceVector_accumulate(ResourceVector this, const_ResourceVector that)
{
  ResourceVector_PACKED(this)[0] += ResourceVector_PACKED(that)[0];
  ResourceVector_PACKED(this)[1] += ResourceVector_PACKED(that)[1];
}
#endif//NSIMDIZE
#endif//$XCC__h

#ifdef NSIMDIZE
void
ResourceVector_accumulate(ResourceVector this, const_ResourceVector that)
{
  int resource;
  for (resource = 1; resource < Resource__; resource++) {
    *ResourceVector__UNITS(this)[resource - 1] += ResourceVector_UNITS(that)[resource - 1];
  }
}
#endif//NSIMDIZE

#ifdef $XCC__h
/**
 * True if @c this + @c increase <= @c limit.
 */
#ifdef NSIMDIZE
bool
ResourceVector_accept(const_ResourceVector this,
                      const_ResourceVector increase,
                      const_ResourceVector limit);
#else
static inline bool
ResourceVector_accept(const_ResourceVector this,
                      const_ResourceVector increase,
                      const_ResourceVector limit)
{
  int result = true;
  uint64_t mask = 0x8080808080808080ULL;
  {
    uint64_t x_0 = ResourceVector_PACKED(limit)[0];
    uint64_t y_0 = ResourceVector_PACKED(this)[0] + ResourceVector_PACKED(increase)[0];
    x_0 |= mask;			// Set high bit of each unit.
    x_0 -= y_0;			// Carry does not propagate across units.
    x_0 &= mask;			// Grab high bits after substract.
    result &= (x_0 == mask);		// High bit change means this + increase > limit.
  }
  {
    uint64_t x_1 = ResourceVector_PACKED(limit)[1];
    uint64_t y_1 = ResourceVector_PACKED(this)[1] + ResourceVector_PACKED(increase)[1];
    x_1 |= mask;			// Set high bit of each unit.
    x_1 -= y_1;			// Carry does not propagate across units.
    x_1 &= mask;			// Grab high bits after substract.
    result &= (x_1 == mask);		// High bit change means this + increase > limit.
  }
  return result;
}
#endif//NSIMDIZE
#endif//$XCC__h

#ifdef NSIMDIZE
bool
ResourceVector_accept(const_ResourceVector this,
                      const_ResourceVector increase,
                      const_ResourceVector limit)
{
  int resource;
  for (resource = 1; resource < Resource__; resource++) {
    int required = ResourceVector_UNITS(increase)[resource - 1];
    int available = ResourceVector_UNITS(limit)[resource - 1];
    int unit = ResourceVector_UNITS(this)[resource - 1];
    int new_unit = unit + required;
    if (new_unit > available) return false;
  }
  return true;
}
#endif//NSIMDIZE

#ifdef $XCC__h
/**
 * Max-merge @c that ResourceVector into @c this ResourceVector.
 *
 * Return:	true if the merge changed @c this else false.
 */
#ifdef NSIMDIZE
bool
ResourceVector_maxMerge(ResourceVector this, const_ResourceVector that);
#else
static inline bool
ResourceVector_maxMerge(ResourceVector this, const_ResourceVector that)
{
  int result = false;
  uint64_t mask = 0x8080808080808080ULL;
  {
    uint64_t x_0 = ResourceVector_PACKED(that)[0];
    uint64_t y_0 = ResourceVector_PACKED(this)[0];
    uint64_t z_0 = x_0, t_0 = 0;
    z_0 |= mask;			// Set high bit of each unit.
    z_0 -= y_0;			// Carry does not propagate across units.
    z_0 ^= mask;			// z_0 = (that - this).
    t_0 = mask & z_0;			// Grab the sign bits of (that - this).
    t_0 = (t_0 << 1) - (t_0 >> 7);	// All ones if (that < this) else all zeros.
    z_0 &= ~t_0;			// z_0 = _MAX(that - this, 0).
    y_0 += z_0;			// y_0 = _MAX(that, this).
    ResourceVector_PACKED(this)[0] = y_0;
    result |= (z_0 != 0);
  }
  {
    uint64_t x_1 = ResourceVector_PACKED(that)[1];
    uint64_t y_1 = ResourceVector_PACKED(this)[1];
    uint64_t z_1 = x_1, t_1 = 0;
    z_1 |= mask;			// Set high bit of each unit.
    z_1 -= y_1;			// Carry does not propagate across units.
    z_1 ^= mask;			// z_1 = (that - this).
    t_1 = mask & z_1;			// Grab the sign bits of (that - this).
    t_1 = (t_1 << 1) - (t_1 >> 7);	// All ones if (that < this) else all zeros.
    z_1 &= ~t_1;			// z_1 = _MAX(that - this, 0).
    y_1 += z_1;			// y_1 = _MAX(that, this).
    ResourceVector_PACKED(this)[1] = y_1;
    result |= (z_1 != 0);
  }
  return result;
}
#endif//NSIMDIZE
#endif//$XCC__h

#ifdef NSIMDIZE
bool
ResourceVector_maxMerge(ResourceVector this, const_ResourceVector that)
{
  int resource;
  bool changed = false;
  for (resource = 1; resource < Resource__; resource++) {
    int this_unit = ResourceVector_UNITS(this)[resource - 1];
    int that_unit = ResourceVector_UNITS(that)[resource - 1];
    if (this_unit < that_unit) {
      *ResourceVector__UNITS(this)[resource - 1] = that_unit;
      changed = true;
    }
  }
  return changed;
}
#endif//NSIMDIZE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Maintains the Resource state for Instruction scheduling.
 *
 * Each VECTORS element is the resource usage for a date (modulo ResourceTable_CLP2HORIZON).
 * The dates considered are never earlier than the ResourceTable WINDOWSTART.
 */
struct ResourceTable_;
  //@args	Processor processor, int windowSize
#endif//$XCC_h

#ifdef $XCC__h
struct ResourceTable_ {
  uint8_t WINDOWSIZE;
  uint16_t CLP2HORIZON;
  int16_t WINDOWSTART;
  const_ResourceVector AVAILABILITY;
  //@access VECTORS	(ResourceVector_ *)((ResourceTable)(this) + 1)
};
#endif//$XCC__h

ResourceTable
ResourceTable_Ctor(ResourceTable this,
                   Processor processor, int windowSize)
{
  int horizon = windowSize+Reservation_COLUMNS_MAX;
  int clp2Horizon = HackerU32_clp2(horizon);
  size_t vectorsSize = clp2Horizon*sizeof(ResourceVector_);
  Except_REQUIRE(windowSize >= 0 && clp2Horizon > 0);
  *ResourceTable__WINDOWSIZE(this) = windowSize;
  *ResourceTable__CLP2HORIZON(this) = clp2Horizon;
  *ResourceTable__WINDOWSTART(this) = 0;
  *ResourceTable__AVAILABILITY(this) = Processor_availability(processor);
  memset(ResourceTable_VECTORS(this), 0, vectorsSize);
  return this;
}

size_t
ResourceTable_Size(Processor processor, int windowSize)
{
  int horizon = windowSize+Reservation_COLUMNS_MAX;
  int clp2Horizon = HackerU32_clp2(horizon);
  size_t vectorsSize = clp2Horizon*sizeof(ResourceVector_);
  Except_REQUIRE(windowSize >= 0 && clp2Horizon > 0);
  return sizeof(ResourceTable_) + vectorsSize;
}

#ifdef $XCC_h
void
ResourceTable_reset(ResourceTable this, int windowStart);
#endif//$XCC_h

void
ResourceTable_reset(ResourceTable this, int windowStart)
{
  int clp2Horizon = ResourceTable_CLP2HORIZON(this);
  size_t vectorsSize = clp2Horizon*sizeof(ResourceVector_);
  *ResourceTable__WINDOWSTART(this) = windowStart;
  memset(ResourceTable_VECTORS(this), 0, vectorsSize);
}

#ifdef $XCC_h
bool
ResourceTable_fitReservation(ResourceTable this, Reservation reservation, int startDate);
#endif//$XCC_h

bool
ResourceTable_fitReservation(ResourceTable this, Reservation reservation, int startDate)
{
  int windowStart = ResourceTable_WINDOWSTART(this);
  int clp2Horizon = ResourceTable_CLP2HORIZON(this);
  ResourceVector_ *vectors = ResourceTable_VECTORS(this);
  const_ResourceVector availability = ResourceTable_AVAILABILITY(this);
  const_ReservationTable table = Reservation_table(reservation);
  const_ResourceVector columns = ReservationTable_COLUMNS(table);	// FIXME!
  unsigned moduloDate = startDate & (clp2Horizon - 1);
  ResourceVector vector = vectors + moduloDate;
  Except_CHECK(startDate >= windowStart && startDate < windowStart + clp2Horizon);
  if (ResourceVector_accept(vector, columns, availability)) {
    ResourceVector_accumulate(vector, columns);
    return true;
  }
  return false;
}

#ifdef $XCC_h
bool
ResourceTable_isFull(const_ResourceTable this, const_ResourceVector column, int startDate);
#endif//$XCC_h

bool
ResourceTable_isFull(const_ResourceTable this, const_ResourceVector column, int startDate)
{
  int windowStart = ResourceTable_WINDOWSTART(this);
  int clp2Horizon = ResourceTable_CLP2HORIZON(this);
  ResourceVector_ *vectors = ResourceTable_VECTORS(this);
  const_ResourceVector availability = ResourceTable_AVAILABILITY(this);
  unsigned moduloDate = startDate & (clp2Horizon - 1);
  ResourceVector vector = vectors + moduloDate;
  Except_CHECK(startDate >= windowStart && startDate < windowStart + clp2Horizon);
  return !ResourceVector_accept(vector, column, availability);
}

#ifdef $XCC_h
int
ResourceTable_tryReservation(ResourceTable this, Reservation reservation, int startDate);
#endif//$XCC_h

int
ResourceTable_tryReservation(ResourceTable this, Reservation reservation, int startDate)
{
  int windowStart = ResourceTable_WINDOWSTART(this);
  int clp2Horizon = ResourceTable_CLP2HORIZON(this);
  ResourceVector_ *vectors = ResourceTable_VECTORS(this);
  const_ResourceVector availability = ResourceTable_AVAILABILITY(this);
  const_ReservationTable table = Reservation_table(reservation);
  const_ResourceVector columns = ReservationTable_COLUMNS(table);	// FIXME!
  while (startDate < windowStart + clp2Horizon) {
    unsigned moduloDate = startDate & (clp2Horizon - 1);
    ResourceVector vector = vectors + moduloDate;
    if (ResourceVector_accept(vector, columns, availability)) break;
    startDate++;
  }
  return startDate;
}

#ifdef $XCC_h
void
ResourceTable_addReservation(ResourceTable this, Reservation reservation, int startDate);
#endif//$XCC_h

void
ResourceTable_addReservation(ResourceTable this, Reservation reservation, int startDate)
{
  int windowSize = ResourceTable_WINDOWSIZE(this);
  int clp2Horizon = ResourceTable_CLP2HORIZON(this);
  ResourceVector_ *vectors = ResourceTable_VECTORS(this);
  const_ResourceVector availability = ResourceTable_AVAILABILITY(this);
  const_ReservationTable table = Reservation_table(reservation);
  const_ResourceVector columns = ReservationTable_COLUMNS(table);	// FIXME!
  unsigned moduloDate = startDate & (clp2Horizon - 1);
  // Advance windowStart to startDate - windowSize.
  ResourceTable_advance(this, startDate - windowSize);
  // Accumulate the resource uses.
  ResourceVector_accumulate(vectors + moduloDate, columns);
}

#ifdef $XCC_h
void
ResourceTable_advance(ResourceTable this, int newDate);
#endif//$XCC_h

void
ResourceTable_advance(ResourceTable this, int newDate)
{
  int clp2Horizon = ResourceTable_CLP2HORIZON(this);
  int windowStart = ResourceTable_WINDOWSTART(this);
  ResourceVector_ *vectors = ResourceTable_VECTORS(this);
  if (newDate > windowStart) {
    // Clear the past entries of VECTORS.
    if (newDate >= windowStart + clp2Horizon) {
      size_t tableSize = clp2Horizon*sizeof(ResourceVector_);
      memset(vectors, 0, tableSize);
    } else {
      for (; windowStart < newDate; windowStart++) {
        unsigned moduloDate = windowStart & (clp2Horizon - 1);
        ResourceVector_clear(vectors + moduloDate);
      }
    }
    *ResourceTable__WINDOWSTART(this) = newDate;
  }
}

#ifdef $XCC_h
bool
ResourceTable_merge(ResourceTable this, const_ResourceTable that, int delay);
#endif//$XCC_h

bool
ResourceTable_merge(ResourceTable this, const_ResourceTable that, int delay)
{
  bool changed = false;
  int clp2Horizon = ResourceTable_CLP2HORIZON(this);
  int this_windowStart = ResourceTable_WINDOWSTART(this), i;
  int that_windowStart = ResourceTable_WINDOWSTART(that) + delay;
  ResourceVector_ *this_vectors = ResourceTable_VECTORS(this);
  const ResourceVector_ *that_vectors = ResourceTable_VECTORS(that);
  Except_CHECK(clp2Horizon == ResourceTable_CLP2HORIZON(that));
  for (i = 0; i < clp2Horizon - delay; i++) {
    int this_date = this_windowStart + i;
    int that_date = that_windowStart + i;
    unsigned this_moduloDate = this_date & (clp2Horizon - 1);
    unsigned that_moduloDate = that_date & (clp2Horizon - 1);
    ResourceVector this_vector = this_vectors + this_moduloDate;
    const_ResourceVector that_vector = that_vectors + that_moduloDate;
    changed |= ResourceVector_maxMerge(this_vector, that_vector);
  }
  return changed;
}

#ifdef $XCC_h
bool
ResourceTable_pretty(const_ResourceTable this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
ResourceTable_pretty(const_ResourceTable this, FILE *file)
{
  int clp2Horizon = ResourceTable_CLP2HORIZON(this);
  int windowStart = ResourceTable_WINDOWSTART(this), resource, i;
  const_ResourceVector availability = ResourceTable_AVAILABILITY(this);
  ResourceVector_ *vectors = ResourceTable_VECTORS(this);
  if (file == NULL) file = stderr;
  fprintf(file, "[%d]\t|", windowStart);
  for (i = 0; i < clp2Horizon; i++) {
    fprintf(file, "%+3d", i);
  }
  fputc('\n', file);
  fputs("--------|", file);
  for (i = 0; i < clp2Horizon; i++) fputs("---", file);
  fputc('\n', file);
  for (resource = 1; resource < Resource__; resource++) {
    fprintf(file, "%s\t|", Resource_name(resource));
    for (i = 0; i < clp2Horizon; i++) {
      int date = windowStart + i;
      unsigned moduloDate = date & (clp2Horizon - 1);
      unsigned use = ResourceVector_UNITS(vectors + moduloDate)[resource - 1];
      if (use != 0) fprintf(file, "%3d", use);
      else fputs("   ", file);
    }
    fprintf(file,"  (%d)\n", ResourceVector_UNITS(availability)[resource - 1]);
  }
  return true;
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Names for the Resource enumeration.
 */
extern const char *
Resource_Id(Resource this);
extern const char *
Resource_name(Resource this);
#endif//$XCC_h

#ifndef _NTRACE
const char *
Resource_Id(Resource this)
{
  static const char *_[] = {
#define Resource(ID, AVAILABILITY) \
    #ID,
    "_UNDEF",
#include "Resource.tuple"
  };
  Except_REQUIRE((unsigned)this < Resource__);
  return _[this];
}
const char *
Resource_name(Resource this)
{
  return this? strchr(Resource_Id(this) + sizeof(MDT_TARGET) - 1, '_') + 1: "_UNDEF";
}
#endif//_NTRACE

#ifdef $XCC__c
{
  int resource;
  for (resource = 0; resource < Resource__; resource++) {
    printf("Resource_%s\t%d\n", Resource_Id(resource), resource);
  }
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

const uint8_t
Resource_Availability[][Processor__] = {
#define AVAILABILITY(items) { 1/*Processor_*/, items }
#define AVAILABLE(processor,available) /*[processor] =*/ available,
#define Resource(ID, AVAILABILITY) \
  AVAILABILITY,
  AVAILABILITY(0),
#include "Resource.tuple"
};

#ifdef $XCC_h
/**
 * This Resource availability units for a Processor.
 */
int
Resource_availability(Resource this, Processor processor);
#endif//$XCC_h

int
Resource_availability(Resource this, Processor processor)
{
  Except_CHECK((unsigned)(this - 1) < Resource__ - 1);
  Except_CHECK((unsigned)processor < Processor__);
  return Resource_Availability[this][processor];
}

#ifdef $XCC__c
{
  int resource, processor;
  printf("\t\t\t\t");
  for (processor = 0; processor < Processor__; processor++) {
    printf("%s\t", Processor_Id(processor));
  }
  printf("\n");
  for (resource = 1; resource < Resource__; resource++) {
    printf("Resource_%s\t", Resource_Id(resource));
    for (processor = 0; processor < Processor__; processor++) {
      printf("\t\t%d", Resource_availability(resource, processor));
    }
    printf("\n");
  }
  fprintf(stderr, "sizeof(Resource_Availability)\t%zu\n", sizeof(Resource_Availability));
}
#endif//$XCC__c

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Resource module.
 */
#define Resource_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Resource module.
 */
#define Resource_FINI()
#endif//$XCC__h

#if XCC__C
static void
Resource_TEST(void)
{
#include "MDT/Resource_.c"
}

int
main(int argc, char **argv)
{
  MDT_Test(Resource_TEST);
  return 0;
}
#endif

