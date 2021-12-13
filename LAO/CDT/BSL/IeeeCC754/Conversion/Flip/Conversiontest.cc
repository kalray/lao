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


$Id: Conversiontest.cc 55 2005-01-26 16:59:10Z huesken $

                                                                     
*/

#include <cmath>
#include <DriverFloatRepr.h>
extern "C" {
#include "Flip32.h"
#include "Flip64.h"
}

DriverFloatRepr DriverFloatRepr::roundto(int dsizeExp,int dsizeMant,int dhidden){
  DriverFloatRepr r;
  SetLibRound();
  SetLibEnvironment();
    switch (sizeExp) {
        // From Flip32...
        case 8: {
            // To Flip64
            if (dsizeExp == 11) {
                flip32 from = toFlip32();
                flip64 to = Flip64_fromFlip32(from);
                r.fromFlip64(to);
            }
            else {
                throw("Wrong exponent size.");
            }
            break;
        }
        // From Flip64
        case 11: {
            // To Fli32p
            if (dsizeExp == 8) {
                flip64 from = toFlip64();
                flip32 to = Flip32_fromFlip64(from);
                r.fromFlip32(to);
            }
            else {
                throw("Wrong exponent size.");
            }
            break;
        }
        default: {
            throw("Wrong exponent size.");
        }
    }
  GetLibExceptions(); 
  return r;
}

DriverFloatRepr DriverFloatRepr::copyto(int dsizeExp,int dsizeMant,int dhidden){
    SetLibRound();
    SetLibEnvironment();
    flip32 from = toFlip32();
    flip64 to = Flip64_fromFlip32(from);
    GetLibExceptions(); 
    DriverFloatRepr r;
    r.fromFlip64(to);
    return r;
}


DriverFloatRepr DriverFloatRepr::rint(){
//  <Your datatype> op(sizeMant );
//  To(op);
//  SetLibRound();
//  op.<round to integral value>;
//  GetLibExceptions();  
//  DriverFloatRepr r(op);
  DriverFloatRepr r;
  std::cerr << "Called wrong function" << std::endl;
  return r;
}
DriverFloatRepr DriverFloatRepr::ri(){
  SetLibRound();
  SetLibEnvironment();
  flip32 from = toFlip32();
  uint32_t res = Flip32_toInt32(from);
  DriverFloatRepr r = (ulong32) res;
  GetLibExceptions(); 
  return r;
}
DriverFloatRepr DriverFloatRepr::ru(){
  SetLibRound();
  SetLibEnvironment();
  flip32 from = toFlip32();
  uint32_t res = Flip32_toUint32(from);
  DriverFloatRepr r = ((ulong32) res);
  GetLibExceptions(); 
  return r;
}

DriverFloatRepr DriverFloatRepr::rI(){
  SetLibRound();
  SetLibEnvironment();
  flip64 from = toFlip64();
  uint64_t res = Flip64_toInt64(from);
  DriverFloatRepr r = ((unsigned long long int) res);
  GetLibExceptions(); 
  return r;
}

DriverFloatRepr DriverFloatRepr::rU(){
  SetLibRound();
  SetLibEnvironment();
  flip64 from = toFlip64();
  uint64_t res = Flip64_toUint64(from);
  DriverFloatRepr r = ((unsigned long long int) res);
  GetLibExceptions(); 
  return r;
}

DriverFloatRepr DriverFloatRepr::ci(int dsizeExp,int dsizeMant,int dhidden){
  SetLibRound();
  SetLibEnvironment();
  flip32 toto = Flip32_fromInt32(toint());
  GetLibExceptions(); 
  DriverFloatRepr r;
  r.fromFlip32(toto);
  return r;
}

DriverFloatRepr DriverFloatRepr::cu(int dsizeExp,int dsizeMant,int dhidden){
  SetLibRound();
  SetLibEnvironment();
  uint32_t u = toint();
  flip32 toto = Flip32_fromUint32(u);
  GetLibExceptions(); 
  DriverFloatRepr r;
  r.fromFlip32(toto);
  return r;
}

DriverFloatRepr DriverFloatRepr:: cI(int dsizeExp,int dsizeMant,int dhidden){
  SetLibRound();
  SetLibEnvironment();
  flip64 toto = Flip64_fromInt64(tolonglong());
  GetLibExceptions(); 
  DriverFloatRepr r;
  r.fromFlip64(toto);
  return r;
}

DriverFloatRepr DriverFloatRepr:: cU(int dsizeExp,int dsizeMant,int dhidden){
  SetLibRound();
  SetLibEnvironment();
  flip64 toto = Flip64_fromUint64(toulonglong());
  GetLibExceptions(); 
  DriverFloatRepr r;
  r.fromFlip64(toto);
  return r;
}

DriverFloatRepr DriverFloatRepr:: b2d(unsigned int prec){
  // convert binary representation to prec decimal digits
//  <Your datatype> op(sizeMant ),res(sizeMant );
  DriverFloatRepr tmp;
//  To(op);
//  SetLibRound();
//  op.<todecimal(tmp.decimal,prec)>;
//  GetLibExceptions(); 
  std::cerr << "Called wrong function" << std::endl;
  return tmp;
}

DriverFloatRepr DriverFloatRepr::d2b() {
//  // convert decimal digit representation to binary
//  <Your datatype> res(sizeMant );
//  // cout << sizeMant ;
//  SetLibRound();
//  res.<fromdecimal(decimal)>;
//  GetLibExceptions();  
//  DriverFloatRepr r(res);
  DriverFloatRepr r;
  std::cerr << "Called wrong function" << std::endl;
  return r;
}



