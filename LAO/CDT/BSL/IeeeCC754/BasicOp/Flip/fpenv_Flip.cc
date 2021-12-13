 /*
##########################################################################
#                                                                        #
# Program: IeeeCC754                                                     #
#                                                                        #
# Description:                                                           #
#   IeeeCC754 or IEEE 754 Compliance Checker is a precision and range    #
#   independent tool to test whether an implementation of                #
#   floating-point arithmetic (in hardware or software) is compliant     #
#   with the principles of the IEEE 754-854 floating-point standards.    #
#   You can find out more about the testing tool IeeeCC754 at            #
#                                                                        #
#         http://www.win.ua.ac.be/~cant/ieeecc754.html                   #
#                                                                        #
#   This tool is in parts based on and greatly benefited from the        #
#   the program FPTEST developed by Jerome Coonen. For a full            #
#   description of the extensions to FPTEST and a reference to           #
#   the original Coonen program, please refer to the URL given above.    #
#   For the options available with the program IeeeCC754 and its         #
#   compatibility with David Hough's hexadecimal UCB format, we          #
#   also refer to the file readme.usage.                                 #
#                                                                        #
#  Usage: see readme.usage                                               #
#                                                                        #
#  Responsible authors:                                                  #
#         Brigitte Verdonk                                               #
#         Annie Cuyt                                                     #
#                                                                        #
#  Contributors:                                                         #
#         Tarun Agarwal (05-07/2002)                                     #
#         Stefan Becuwe (07/2000- )                                      #
#         Johan Bogo (10/1998-09/1999)                                   #
#         Wim Dausy (10/2002-09/2003)                                    #
#         Tim Gevers (10-12/2000)                                        #
#         Matthias Huesken (02/2004- )                                   #
#         Debby Ooms (10/1996-09/1997)                                   #
#         Geert Vermuyten (10/1996-09/1997)                              #
#         Dennis Verschaeren (09/1996-06/2000)                           #
#                                                                        #
#  Copyright (C) 2000,2001,2002,2003,2004,2005                           #
#                Universiteit Antwerpen                                  #
#                                                                        #
#  This program can be obtained from the authors, free, but WITHOUT ANY  #
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or     #
#  FITNESS FOR A PARTICULAR PURPOSE.                                     #
#                                                                        #
#  Contact:                                                              #
#       Brigitte.Verdonk@ua.ac.be                                        #
#       Departement Wiskunde en Informatica                              #
#       Universiteit Antwerpen                                           #
#       Middelheimlaan 1                                                 #
#       B-2020 Antwerpen, Belgium                                        #
#                                                                        #
##########################################################################


$Id: fpenv_Platform.cc 78 2005-11-02 20:14:19Z huesken $

                                                                     
*/

#include <cmath>
#include <Types.h>
#include <DriverFloatRepr.h>
extern "C" {
#include "FTypes.h"
}

/* 
By default, all functions in this file are declared in the header file

  DriverFloatRepr.h

except for two functions 

DriverFloatRepr::DriverFloatRepr(<MyDatatype &D>)
<MyDatatype> DriverFloatRepr::to()

These two functions, for conversion between the floating-point
data type of your target implementation and DriverFloatRepr, the
floating-point data type of the driver program, should be declared
explicitly in the header file DriverFloatRepr.h, with MyDatatype
replaced by the appropriate identifier.  */

void DriverFloatRepr::SetLibRound()
{
  switch (GetFPRound()) { 
      case RM_NEAR:
        Flip_rnd_mode = FLIP_RN;
        break;
      case RM_ZERO:
        Flip_rnd_mode = FLIP_RZ;
        break;
      case RM_UP:
        Flip_rnd_mode = FLIP_RU;
        break;
      case RM_DOWN:
        Flip_rnd_mode = FLIP_RD;
        break;
    } 
}

void DriverFloatRepr::SetLibEnvironment()
{
    Flip_status = FLIP_EXPT_OK;
}

void DriverFloatRepr::GetLibExceptions()
{
    if (Flip_status & FLIP_DIV_BY_ZERO) SetFPDivByZero();
    if (Flip_status & FLIP_INVALID) SetFPInvalid();
    if (Flip_status & FLIP_UNDERFLOW) SetFPUnderflow();
    if (Flip_status & FLIP_OVERFLOW) SetFPOverflow();
    if (Flip_status & FLIP_INEXACT) SetFPInexact();
}

void DriverFloatRepr::fromFlip32(const flip32 &D) { 
  sizeExp =  8;
  sizeMant = 23;
  hidden = 1;
  mant = Bitstring(sizeMant);
  exp = Bitstring(sizeExp);

  Bitstring temp(32);
  temp[0] = (ulong32 &) D;
  sign = temp.GetBit(0);
  
  // convert exponent
  for (size_t i = 0; i < sizeExp; ++i) {
    exp.PutBit(i, temp.GetBit(i+1));
  }
  // convert mantissa
  for (size_t i = 0;i < sizeMant; i++) {
    mant.PutBit(i, temp.GetBit(i+sizeExp+1));
  }
}

void DriverFloatRepr::fromFlip64(const flip64 &D) { 
  sizeExp = 11;
  sizeMant = 52;
  hidden = 1;
  mant = Bitstring(sizeMant);
  exp = Bitstring(sizeExp);

  Bitstring temp(64);
  temp[0] = D >> 32;
  temp[1] = D;
  sign = temp.GetBit(0);
  
  // convert exponent
  for (size_t i = 0; i < sizeExp; ++i) {
    exp.PutBit(i, temp.GetBit(i+1));
  }
  // convert mantissa
  for (size_t i = 0;i < sizeMant; i++) {
    mant.PutBit(i, temp.GetBit(i+sizeExp+1));
  }
}


flip32 DriverFloatRepr::toFlip32() {
  Bitstring temp(32);

  if(sign)
    temp.PutBit(0,1);

  for (size_t i = 1; i <= sizeExp;i++)
    temp.PutBit(i,exp.GetBit(i-1));

  for (size_t i = sizeExp + 1;i < sizeExp + 1 + sizeMant;i++)
    temp.PutBit(i,mant.GetBit(i-sizeExp-1));

  return *((flip32 *)(&temp[0]));
}

flip64 DriverFloatRepr::toFlip64() {
  Bitstring temp(64);
  unsigned int i;

  if(sign)
    temp.PutBit(0,1);

  for (i = 1; i <= sizeExp;i++)
    temp.PutBit(i,exp.GetBit(i-1));

  for (i = sizeExp + 1;i < sizeExp + 1 + sizeMant;i++)
    temp.PutBit(i,mant.GetBit(i-sizeExp-1));

  return ((uint64_t) temp[0] << 32) | temp[1];
}


// conversions between DriverFloatRepr and hardware integer
// data types; implementation should not be modified !
// --

DriverFloatRepr::DriverFloatRepr(long32 i) {
  Bitstring temp(32);
  hidden = 0;
  sizeMant = 32;
  // Benjamin Orgogozo: add initialization of mantissa to prevent core
  // dump in SubBitstring.
  Bitstring temporary(sizeMant);
  mant = temporary;
  sizeExp = 0;
  temp[0] = (ulong32) i;
  temp.SubBitstring(0,mant);
}

DriverFloatRepr::DriverFloatRepr(ulong32 i) {
  Bitstring temp(32);
  hidden = 0;
  sizeMant = 32;
  // Benjamin Orgogozo: add initialization of mantissa to prevent core
  // dump in SubBitstring.
  Bitstring temporary(sizeMant);
  mant = temporary;
  sizeExp = 0;
  temp[0] = (ulong32) i;
  temp.SubBitstring(0,mant);
}

DriverFloatRepr::DriverFloatRepr(long long i) {
  Bitstring temp(64);
  temp[0] = i & 0xFFFFFFFF;
  temp[1] = i >> 32;
  hidden = 0;
  sizeMant = 64;
  sizeExp = 0;
  temp.SubBitstring(0,mant);
}

DriverFloatRepr::DriverFloatRepr(unsigned long long i) {
  hidden = 0;
  sizeMant = 64;
  sizeExp = 0;
  Bitstring temp(64);
  // Benjamin Orgogozo: add initialization of mantissa to prevent core
  // dump in SubBitstring.
  Bitstring temporary(sizeMant);
  mant = temporary;
  temp[1] = i & 0xFFFFFFFF;
  temp[0] = i >> 32;
  temp.SubBitstring(0,mant);
}

int DriverFloatRepr::toint() {
  return *((int *)(&mant[0]));
}

unsigned int DriverFloatRepr::touint() {
  return *((unsigned int *)(&mant[0]));
}

long long int DriverFloatRepr::tolonglong() {
  long long int r = (((long long int) mant[0]) << 32) | mant[1];
  return r;
}

unsigned long long int DriverFloatRepr::toulonglong() {
  unsigned long long int r = (((long long int) mant[0]) << 32) | mant[1];
  return r;
} 

