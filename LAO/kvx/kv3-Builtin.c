/*
 * kv3/Builtin.c
 *  (c) Copyright 2010-2018 Kalray SA.
 * Automatically generated from the Machine Description System (MDS).
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#ifdef $XCC_h
extern void __builtin_kvx_set_ps(int32_t operand1, int32_t operand2);
#endif//$XCC_h

void __builtin_kvx_set_ps(int32_t operand1, int32_t operand2);

void __builtin_kvx_set_ps(int32_t operand1, int32_t operand2)
{
;
}

#ifdef $XCC_h
extern void __builtin_kvx_syncgroup(unsigned operand1);
#endif//$XCC_h

void __builtin_kvx_syncgroup(unsigned operand1);

void __builtin_kvx_syncgroup(unsigned operand1)
{
;
}

typedef struct {
  unsigned ic: 1;
  unsigned io: 1;
  unsigned dz: 1;
  unsigned ov: 1;
  unsigned un: 1;
  unsigned in: 1;
  unsigned u1: 2;
  unsigned rm: 2;
  unsigned fs: 1;
  unsigned ne: 4;
  unsigned wu: 1;
  unsigned cc: 16;
} __builtin_kvx_cs_t;
extern __builtin_kvx_cs_t *__builtin_kvx__cs;

__builtin_kvx_cs_t *__builtin_kvx__cs = 0;

#ifdef $XCC_h
extern uint64_t __builtin_kvx_insf(uint64_t operand2, unsigned operand3, unsigned operand4);
#endif//$XCC_h

uint64_t __builtin_kvx_insf(uint64_t operand2, unsigned operand3, unsigned operand4);

uint64_t __builtin_kvx_insf(uint64_t operand2, unsigned operand3, unsigned operand4)
{
unsigned startbit = operand4, stopbit = operand3, bias = startbit <= stopbit;
uint64_t mask = (2<<stopbit) - (2-bias<<startbit) + bias - 1;
uint64_t operand1 = ((operand2 << startbit) & mask) | (operand1 & ~mask);
return operand1;
}

#ifdef $XCC_h
extern uint64_t __builtin_kvx_extfz(uint64_t operand2, unsigned operand3, unsigned operand4);
#endif//$XCC_h

uint64_t __builtin_kvx_extfz(uint64_t operand2, unsigned operand3, unsigned operand4);

uint64_t __builtin_kvx_extfz(uint64_t operand2, unsigned operand3, unsigned operand4)
{
unsigned startbit = operand4, stopbit = operand3, bias = startbit <= stopbit;
uint64_t mask = (2<<stopbit) - (2-bias<<startbit) + bias - 1;
uint64_t masked = operand2 & mask;
uint64_t operand1 = masked >> startbit;
return operand1;
}

#ifdef $XCC_h
extern int64_t __builtin_kvx_extfs(uint64_t operand2, unsigned operand3, unsigned operand4);
#endif//$XCC_h

int64_t __builtin_kvx_extfs(uint64_t operand2, unsigned operand3, unsigned operand4);

int64_t __builtin_kvx_extfs(uint64_t operand2, unsigned operand3, unsigned operand4)
{
unsigned startbit = operand4 & 0x1F;
unsigned stopbit = operand3 & 0x1F;
unsigned bias = startbit <= stopbit;
uint64_t mask = (2<<stopbit) - (2-bias<<startbit) + bias - 1;
uint64_t masked1 = operand2 & mask;
uint64_t masked2 = operand2 | ~mask;
uint64_t condition = (mask & ~(mask>>1)) & operand2;
uint64_t masked = (condition == 0)? masked1: masked2;
int64_t operand1 = masked >> startbit;
return operand1;
}

#ifdef $XCC_h
extern int32_t __builtin_kvx_stsuw(int32_t operand2, int32_t operand3);
#endif//$XCC_h

int32_t __builtin_kvx_stsuw(int32_t operand2, int32_t operand3);

int32_t __builtin_kvx_stsuw(int32_t operand2, int32_t operand3)
{
int32_t operand1 = operand3 >= operand2?
                  (operand3 - operand2)<<1 | 1:
                  operand3<<1;
return operand1;
}

#ifdef $XCC_h
extern int32_t __builtin_kvx_abdw(int32_t operand2, int32_t operand3);
#endif//$XCC_h

int32_t __builtin_kvx_abdw(int32_t operand2, int32_t operand3);

int32_t __builtin_kvx_abdw(int32_t operand2, int32_t operand3)
{
int64_t diff = operand3 - operand2;
int32_t operand1 = diff < 0? -diff: diff;
return operand1;
}

#ifdef $XCC_h
extern int32_t __builtin_kvx_minw(int32_t operand2, int32_t operand3);
#endif//$XCC_h

int32_t __builtin_kvx_minw(int32_t operand2, int32_t operand3);

int32_t __builtin_kvx_minw(int32_t operand2, int32_t operand3)
{
int32_t operand1 = operand2 < operand3? operand2: operand3;
return operand1;
}

#ifdef $XCC_h
extern uint32_t __builtin_kvx_minuw(uint32_t operand2, uint32_t operand3);
#endif//$XCC_h

uint32_t __builtin_kvx_minuw(uint32_t operand2, uint32_t operand3);

uint32_t __builtin_kvx_minuw(uint32_t operand2, uint32_t operand3)
{
uint32_t operand1 = operand2 < operand3? operand2: operand3;
return operand1;
}

#ifdef $XCC_h
extern int32_t __builtin_kvx_maxw(int32_t operand2, int32_t operand3);
#endif//$XCC_h

int32_t __builtin_kvx_maxw(int32_t operand2, int32_t operand3);

int32_t __builtin_kvx_maxw(int32_t operand2, int32_t operand3)
{
int32_t operand1 = operand2 > operand3? operand2: operand3;
}

#ifdef $XCC_h
extern uint32_t __builtin_kvx_maxuw(uint32_t operand2, uint32_t operand3);
#endif//$XCC_h

uint32_t __builtin_kvx_maxuw(uint32_t operand2, uint32_t operand3);

uint32_t __builtin_kvx_maxuw(uint32_t operand2, uint32_t operand3)
{
uint32_t operand1 = operand2 > operand3? operand2: operand3;
return operand1;
}

#ifdef $XCC_h
extern int32_t __builtin_kvx_absw(int32_t operand2);
#endif//$XCC_h

int32_t __builtin_kvx_absw(int32_t operand2);

int32_t __builtin_kvx_absw(int32_t operand2)
{
int32_t operand1 = operand2 >= 0? operand2: -operand2;
return operand1;
}

#ifdef $XCC_h
extern int32_t __builtin_kvx_sath(int32_t operand2);
#endif//$XCC_h

int32_t __builtin_kvx_sath(int32_t operand2);

int32_t __builtin_kvx_sath(int32_t operand2)
{
int32_t operand1 = operand2;
if (operand1 > (int32_t)0x00007FFF) operand1 = 0x00007FFF;
else if (operand1 < (int32_t)0xFFFF8000) operand1 = 0xFFFF8000;
return operand1;
}

#ifdef $XCC_h
extern int32_t __builtin_kvx_addsw(int32_t operand2, int32_t operand3);
#endif//$XCC_h

int32_t __builtin_kvx_addsw(int32_t operand2, int32_t operand3);

int32_t __builtin_kvx_addsw(int32_t operand2, int32_t operand3)
{
int32_t operand1 = __builtin_kvx_satw((int64_t)operand3 + (int64_t)operand2);
return operand1;
}

#ifdef $XCC_h
extern int32_t __builtin_kvx_sbfsw(int32_t operand2, int32_t operand3);
#endif//$XCC_h

int32_t __builtin_kvx_sbfsw(int32_t operand2, int32_t operand3);

int32_t __builtin_kvx_sbfsw(int32_t operand2, int32_t operand3)
{
int32_t operand1 = __builtin_kvx_satw((int64_t)operand3 - (int64_t)operand2);
return operand1;
}

#ifdef $XCC_h
extern unsigned __builtin_kvx_clzw(uint32_t operand2);
#endif//$XCC_h

unsigned __builtin_kvx_clzw(uint32_t operand2);

unsigned __builtin_kvx_clzw(uint32_t operand2)
{
unsigned operand1 = 32;
if (operand2 != 0) {
  operand1 = __builtin_clz(operand2);
}
return operand1;
}

#ifdef $XCC_h
extern unsigned __builtin_kvx_clsw(uint32_t operand2);
#endif//$XCC_h

unsigned __builtin_kvx_clsw(uint32_t operand2);

unsigned __builtin_kvx_clsw(uint32_t operand2)
{
unsigned operand1 = 31;
if (operand2 != 0 && operand2 != -1) {
  for (operand1 = 0; operand1 < 31; operand1++) {
    unsigned curr_bit = operand2>>(31 - operand1) & 0x1;
    unsigned next_bit = operand2>>(30 - operand1) & 0x1;
    if (curr_bit != next_bit) break;
  }
}
return operand1;
}

#ifdef $XCC_h
extern unsigned __builtin_kvx_cbsw(uint32_t operand2);
#endif//$XCC_h

unsigned __builtin_kvx_cbsw(uint32_t operand2);

unsigned __builtin_kvx_cbsw(uint32_t operand2)
{
unsigned operand1 = __builtin_popcount(operand2);
return operand1;
}

#ifdef $XCC_h
extern unsigned __builtin_kvx_ctzw(uint32_t operand2);
#endif//$XCC_h

unsigned __builtin_kvx_ctzw(uint32_t operand2);

unsigned __builtin_kvx_ctzw(uint32_t operand2)
{
unsigned operand1 = 32;
if (operand2 != 0) {
  operand1 = __builtin_ctz(operand2);
}
return operand1;
}

#ifdef $XCC_h
extern void __builtin_kvx_set_ps(int32_t operand1, int32_t operand2);
#endif//$XCC_h

void __builtin_kvx_set_ps(int32_t operand1, int32_t operand2);

void __builtin_kvx_set_ps(int32_t operand1, int32_t operand2)
{
;
}
