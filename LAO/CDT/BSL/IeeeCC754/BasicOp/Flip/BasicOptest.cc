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


$Id: BasicOptest.cc 55 2005-01-26 16:59:10Z huesken $

                                                                     
*/

#include <cmath>
#include <DriverFloatRepr.h>
extern "C" {
#include "Flip32.h"
#include "Flip64.h"
}

using namespace std;

DriverFloatRepr DriverFloatRepr::operator + (DriverFloatRepr& fl) 
{
  DriverFloatRepr r;
  switch (sizeExp) {
    // Flip32
    case 8: {
        flip32 res, op1, op2;
        op1 = toFlip32();
        op2 = fl.toFlip32();

        SetLibRound();
        SetLibEnvironment();

        // cout << "op1 : " << hex << op1 << endl << flush;
        // cout << "op2 : " << op2 << endl << flush;
        GetLibExceptions(); 
        res = Flip32_add(op1, op2);
        // cout << "res: " << res << endl << flush; // debug
        // cout << dec;

        GetLibExceptions();  
        r.fromFlip32(res);
        break;
    }
    // Flip64
    case 11: {
        flip64 res, op1, op2;
        op1 = toFlip64();
        op2 = fl.toFlip64();

        SetLibRound();
        SetLibEnvironment();

// cout << "op1 : " << hex << op1 << endl << flush;
// cout << "op2 : " << op2 << endl << flush;
        GetLibExceptions(); 
        res = Flip64_add(op1, op2);
// cout << "res: " << res << endl << flush; // debug
// cout << dec;

        GetLibExceptions();  
        r.fromFlip64(res);
        break;
    }
    default: {
        throw("Wrong exponent size");
    }

  }

  return r;
}

DriverFloatRepr DriverFloatRepr::operator - (DriverFloatRepr &fl)
{
  DriverFloatRepr r;
  switch (sizeExp) {
    // Flip32
    case 8: {
        flip32 res, op1, op2;
        op1 = toFlip32();
        op2 = fl.toFlip32();

        SetLibRound();
        SetLibEnvironment();

        // cout << "op1 : " << hex << op1 << endl << flush;
        // cout << "op2 : " << op2 << endl << flush;
        GetLibExceptions(); 
        res = Flip32_sub(op1, op2);
        // cout << "res: " << res << endl << flush; // debug
        // cout << dec;

        GetLibExceptions();  
        r.fromFlip32(res);
        break;
    }
    // Flip64
    case 11: {
        flip64 res, op1, op2;
        op1 = toFlip64();
        op2 = fl.toFlip64();

        SetLibRound();
        SetLibEnvironment();

        // cout << "op1 : " << hex << op1 << endl << flush;
        // cout << "op2 : " << op2 << endl << flush;
        GetLibExceptions(); 
        res = Flip64_sub(op1, op2);
        // cout << "res: " << res << endl << flush; // debug
        // cout << dec;

        GetLibExceptions();  
        r.fromFlip64(res);
        break;
    }
    default: {
        throw("Wrong exponent size");
    }

  }

  return r;
}

DriverFloatRepr DriverFloatRepr::operator * (DriverFloatRepr &fl)
{
  DriverFloatRepr r;
  switch (sizeExp) {
    // Flip32
    case 8: {
        flip32 res, op1, op2;
        op1 = toFlip32();
        op2 = fl.toFlip32();

        SetLibRound();
        SetLibEnvironment();

        // cout << "op1 : " << hex << op1 << endl << flush;
        // cout << "op2 : " << op2 << endl << flush;
        GetLibExceptions(); 
        res = Flip32_mul(op1, op2);
        // cout << "res: " << res << endl << flush; // debug
        // cout << dec;

        GetLibExceptions();  
        r.fromFlip32(res);
        break;
    }
    // Flip64
    case 11: {
        flip64 res, op1, op2;
        op1 = toFlip64();
        op2 = fl.toFlip64();

        SetLibRound();
        SetLibEnvironment();

        // cout << "op1 : " << hex << op1 << endl << flush;
        // cout << "op2 : " << op2 << endl << flush;
        GetLibExceptions(); 
        res = Flip64_mul(op1, op2);
        // cout << "res: " << res << endl << flush; // debug
        // cout << dec;

        GetLibExceptions();  
        r.fromFlip64(res);
        break;
    }
    default: {
        throw("Wrong exponent size");
    }
  }

  return r;
}

DriverFloatRepr DriverFloatRepr::operator / (DriverFloatRepr &fl) {
//  <Your datatype> res(sizeMant),op1(sizeMant),op2(sizeMant);
//
//  To(op1);
//  fl.To(op2);
//
//  SetLibRound();
//
//  // cout << "op1 : " << endl << op1 << endl << flush;
//  // cout << "op2 : " << endl << op2 << endl << flush;
//  res = op1 / op2;
//  // cout << "res: " << res << endl << flush; // debug
//
//  GetLibExceptions();  
//  DriverFloatRepr r(res);
//  // DriverFloatRepr r(op1); // debug
  DriverFloatRepr r;
  return r;
}


DriverFloatRepr DriverFloatRepr::operator % (DriverFloatRepr &fl)
{
//  <Your datatype> res(sizeMant),op1(sizeMant),op2(sizeMant);
//
//  To(op1);
//  fl.To(op2);
//
//  SetLibRound();
//  // cout << "op1 : " << endl << op1 << endl << flush;
//  // cout << "op2 : " << endl << op2 << endl << flush;
//  res = op1 % op2;
//  // cout << "res: " << res << endl << flush; // debug
//
//  GetLibExceptions();  
//  DriverFloatRepr r(res);
  DriverFloatRepr r;
  return r;
}

DriverFloatRepr DriverFloatRepr::sqrt()
{
//  <Your datatype> res(sizeMant ),op1(sizeMant );
//
//  // cout << *this;
//  To(op1);
//  // cout << "op1 : " << endl << op1 << endl << flush;
//  // cout << "op1.exp = " << op1.getexp() << endl;
//
//  SetLibRound();
//
//  res = op1.<your square root>;
//  // cout << "res: " << endl << res << endl << flush; // debug
//
//  GetLibExceptions();  
//
//  DriverFloatRepr r(res);
  DriverFloatRepr r;
  return r;
}

