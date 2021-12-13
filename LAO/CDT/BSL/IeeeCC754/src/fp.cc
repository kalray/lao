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


$Id: fp.cc 78 2005-11-02 20:14:19Z huesken $

                                                                     
*/
/**	\file
 * 	\brief Definition of Class FP
 *
 * 	Class FP for floating point numbers (IEEE754). It uses the bitstring class for storing the exponent and the mantissa.
 */

#include <Fp.h>
#include <Types.h>
#include <string.h>


int FP::Endian = NOT_KNOWN;
Bitstring FP::fpEnv(32);

/***********************************************************************
* Member:  GetFPRound()
* Purpose: gets the FP rounding mode from the evnironment
* Return:  void
***********************************************************************/
int FP::GetFPRound()
{
  Bitstring temp(2);
  fpEnv.SubBitstring(21,temp);
  return (temp[0]>>30);
}


/***********************************************************************
* Function:  ClearFPEnvironment()
* Purpose: clears the environment (fpEnv)
* Return:  void
***********************************************************************/
void FP::ClearFPEnvironment()
{
  fpEnv.Clear(0);
  fpEnv.Clear(1);
  fpEnv.Clear(2);
  fpEnv.Clear(3);
  fpEnv.Clear(4);
}


/***********************************************************************
* Member:  CheckEndian()
* Purpose: needed to correctly convert a FP to a HW floating-point
* Return:  void
***********************************************************************/
void FP::CheckEndian()
{
  char *tmp;
  ulong32 tmplong =1;
  
  if (FP::Endian == NOT_KNOWN)
    {
      tmp= (char*)&tmplong;
      if (tmp[0]== 1)
  Endian= MYLITTLE_ENDIAN;
      else
  Endian= MYBIG_ENDIAN;
    }
}


/***********************************************************************
* Member:  FP () Constructor
* Purpose: Create a floating point number
* Return:  Nothing
***********************************************************************/
FP::FP()
{
  CheckEndian();
  sign= -1;
  hidden =-1;
  sizeMant=0;
  sizeExp=0;
  decimal = NULL;
  //  fpEnv.Resize(32);
}


/***********************************************************************
* Member:  FP (int sizeM, int sizeE, int hiddenbit=0) Constructor
* Purpose: Create a floating point number with the size of the exponent 
*          and the mantissa defined and if the there is a hidden bit
*          0 -> false
*          1 -> true
* Return:  Nothing
***********************************************************************/
FP::FP(int sizeM, int sizeE,int hiddenbit)
{
  CheckEndian();
  sign= -1;
  hidden=hiddenbit;
  sizeMant=sizeM;
  sizeExp=sizeE;
  decimal = NULL;
  mant.Resize(sizeM);
  exp.Resize(sizeE);
  //  fpEnv.Resize(32);
}


/***********************************************************************
* Member:  FP (Bitstring &fp,int sizeM,int sizeE,int hiddenbit=0) Constructor
* Purpose: Create a floating point number with the size of the exponent 
*          and the mantissa defined and if the there is a hidden bit
*          0 -> false, 1 -> true and initialize it with "fp" as a
*          bit representation of a IEEE 754 floating point number
* Return:  Nothing
***********************************************************************/
FP::FP(Bitstring &fp,int sizeM, int sizeE,int hiddenbit)
{
  CheckEndian();
  //  fpEnv.Resize(32);
  sign= -1;
  hidden =hiddenbit;
  sizeMant=sizeM;
  sizeExp=sizeE;
  decimal = NULL;

  mant = Bitstring(sizeMant);
  if (sizeExp > 0)
    exp = Bitstring(sizeExp);
  else
    exp = Bitstring();
  
  sign = fp.GetBit(0);
  if (sizeExp > 0) {
    fp.SubBitstring(1,exp);
    fp.SubBitstring(sizeExp+1,mant);
  }
  else // integer
    fp.SubBitstring(0,mant);
}


/***********************************************************************
* Member:  FP(FP & copy)  Copy constructor
* Purpose: Create a floating point number with initiale value the 
*          floating point "copy"
* Return:  Nothing
***********************************************************************/
FP::FP(const FP & copy)
{
  CheckEndian();
  //  fpEnv.Resize(32);
  sign = copy.sign;
  hidden = copy.hidden;
  sizeExp = copy.sizeExp;
  sizeMant = copy.sizeMant;
  mant = copy.mant;
  exp = copy.exp;
  if (copy.decimal != NULL) {
    decimal = new char[256];
if(strlen(copy.decimal)>256)printf("!!!fp.c:214!!!\n");
    strcpy(decimal,copy.decimal);
  }
  else
    decimal = NULL;
}


/***********************************************************************
* Operator:  FP & operator = (FP &copy)
* 	assignment operator
***********************************************************************/
/// assignment operator
FP& FP::operator = ( const FP &copy)
{
  sign = copy.sign;
  hidden = copy.hidden;
  sizeExp = copy.sizeExp;
  sizeMant = copy.sizeMant;
  mant = copy.mant;
  exp = copy.exp;
  if (decimal != NULL) delete[] decimal;
  if (copy.decimal != NULL) {
    decimal = new char[maxstr];
    strcpy(decimal,copy.decimal);
  }
  else
    decimal = NULL;
  return *this;
}


/***********************************************************************
* Function:	int Sign(int sgn)
* Purpose:	returns or sets the sign
* Return:	sign
***********************************************************************/
int FP::Sign(int sgn)
{
  int temp = sign;
  if (sgn != -1)
    {
      if ((sgn == 0)||(sgn==1))
  sign = sgn;
    }
  return temp;
}


/***********************************************************************
* Function:	Bitstring & GetMantissa()
* Purpose:	returns the mantissa
* Return:	mantissa (with hidden bit if any)
***********************************************************************/
Bitstring & FP::GetMantissa()
{
  return mant;
}


/***********************************************************************
* Function:	Bitstring & GetExponent()
* Purpose:	returns the exponent
* Return:	exponent
***********************************************************************/
Bitstring & FP::GetExponent()
{
    return exp;
}


/***********************************************************************
* Function:	Bistring PutMantissa(Bitstring & mantissa)
* Purpose:	sets the mantissa
* Return:	bitstring
***********************************************************************/
Bitstring  FP::PutMantissa(Bitstring &mantissa)
{
  Bitstring temp(mant);
  
  mant=mantissa;
  // mant.Resize(sizeMant+hidden);       //in case the mantisa is too big
  return temp;
}


/***********************************************************************
* Function:	Bitstring PutExponent(Bistring & exponent)
* Purpose:	sets the exponent
* Return:	bitstring
***********************************************************************/
Bitstring  FP::PutExponent(Bitstring &exponent)
{
  Bitstring temp(exp);
  
  exp=exponent;
  exp.Resize(sizeExp);       //in case the mantisa is too big
  return temp;
}


/***********************************************************************
* Function:	int isNaN()
* Purpose:	checks if the floating-point is Nan
* Return:	1 if Nan, 0 in the other case
***********************************************************************/
int FP::IsNaN()
{
  unsigned int i;
  Bitstring fullExp(sizeExp);
  Bitstring fullMant(sizeMant+1-hidden);

  if (sizeExp > 0) {
    for (i=0; i < sizeExp ; i++)
      fullExp.PutBit(i,1); 
    if (fullExp != exp)
      return 0;
    for (i=1-hidden; i < sizeMant+1-hidden ; i++)
      fullMant.PutBit(i,0);
    if (!hidden)
      fullMant.PutBit(0,1);
    return(fullMant != mant);
  }
  else
    return 0;
}


/***********************************************************************
* Function:	int istiny()
* Purpose:	checks if the floating-point is tiny
* Return:
***********************************************************************/
int FP::istiny()
{
  unsigned int i;
  Bitstring fullExp(sizeExp);
  Bitstring fullMant(sizeMant);
  if ((sizeMant > 0) && (sizeExp > 0)) {
    for (i=0; i < sizeExp; i++)
      fullExp.PutBit(i,0); 
    for (i=0; i < sizeMant ; i++)
      fullMant.PutBit(i,0);
    return ((fullExp == exp) && (fullMant != mant));
  }
//   else
    return 0;
}


/***********************************************************************
* Function:	int isInf()
* Purpose:	checks if the floating-point is infinite
* Return:
***********************************************************************/
int FP::isInf()
{
  unsigned int i;
  Bitstring fullExp(sizeExp);
  Bitstring fullMant(sizeMant);
  if ((sizeMant > 0) && (sizeExp > 0)) {
    for (i=0; i < sizeExp ; i++)
      fullExp.PutBit(i,1); 
    if (fullExp == exp) {
      i = 0;
      if (!hidden)
  i++;
      for (;i < sizeMant;i++)
  if (mant.GetBit(i) != 0)
    return 0;
      return 1;
    }
    else 
      return 0;
  }
//   else
    return 0;
}


/***********************************************************************
* Function:	int isNan()
* Purpose:	checks if the floating-point is Nan
* Return:	1 if Nan, 0 in the other case
***********************************************************************/
int FP::isNan()
{
  unsigned int i;
  Bitstring fullExp(sizeExp);
  Bitstring fullMant(sizeMant);
  if ((sizeMant > 0) && (sizeExp > 0)) {
    for (i=0; i < sizeExp ; i++)
      fullExp.PutBit(i,1); 
    if (fullExp == exp) {
      i = 0;
      if (!hidden)
  i++;
      for (;i < sizeMant;i++)
  if (mant.GetBit(i) != 0)
    return 1;
      return 0;
    }
    else 
      return 0;
  }
//   else
    return 0;
}


/***********************************************************************
* Function:	CreateQFPNan()
* Purpose:	creates a quiet Nan
* Return:	void
***********************************************************************/
void  FP::CreateQFPNan()
{

  unsigned int i;
  for (i=0; i < sizeExp ; i++)
    exp.PutBit(i,1);

  mant.PutBit(sizeMant-1,1); //PLAATS
}


/***********************************************************************
* Operator:  FP & operator = (Bitstring &copy)
* 	assignment operator
***********************************************************************/
/// assignment operator
FP& FP::operator = (const Bitstring &copy)
{
  copy.SubBitstring(0,mant);
  copy.SubBitstring(sizeMant,exp);
  sign = copy.GetBit(sizeMant+sizeExp);
  return *this;
}


/***********************************************************************
* Function:	void SetFPRound(int rm)
* Purpose:	sets the rounding mode in the fpEnv
* Return:	void
***********************************************************************/
void FP::SetFPRound (int rm)
{
  fpEnv.Clear(21);
  fpEnv.Clear(22);
  switch (rm)
    {
    case RM_UP:
      fpEnv.Set(21);
      break;
    case RM_DOWN:
      fpEnv.Set(22);
      break;
    case RM_ZERO:
      fpEnv.Set(21);
      fpEnv.Set(22);
      break;
    }
}


void FP::GetFPExceptions(Bitstring E)
{
  fpEnv.SubBitstring(0,E);
}

/***********************************************************************
* Operator:  ostream & operator << (ostream &, FP &)
* 	output operator
***********************************************************************/
/// output operator
ostream& operator << (ostream& outs, FP &strout)
{unsigned int i;

  if (strout.decimal != NULL) // decimal representation
    outs << strout.decimal;
  else {
    int size;
    if (strout.sizeExp > 0)
      size = 1+strout.sizeExp+strout.sizeMant;
    else
      size = strout.sizeMant;
    Bitstring merge(size);
    if (strout.sizeExp > 0) {
      if (strout.sign)
  merge.PutBit(0,1);
      else 
  merge.PutBit(0,0);
      for (i = 0; i < strout.sizeExp;i++)
  merge.PutBit(i+1,strout.exp.GetBit(i));
      for (i = 0; i < strout.sizeMant;i++)
  merge.PutBit(1+strout.sizeExp+i,strout.mant.GetBit(i));
    }
    else { // integer
      for (i = 0; i < strout.sizeMant;i++)
  merge.PutBit(i,strout.mant.GetBit(i));
    }
    outs << merge;
  }
  return outs;
}


/***********************************************************************
* Function:	int isZero()
* Purpose:	checks if the floating-point is a Zero
* Return:	1 if Zero, 0 in the other case
***********************************************************************/
int FP::IsZero()
{
  unsigned int i;
  if ((sizeMant > 0) && (sizeExp > 0)) {
    for (i=0; i < sizeExp ; i++)
      if ( exp.GetBit(i) != 0 ){
        return 0;
      } 
  i=0;
  if (!hidden)
    i++;
  for (;i < sizeMant;i++)
    if (mant.GetBit(i) != 0)
      return 0;  
  }
  return 1;
}


/***********************************************************************
* Function:	int isNegZero()
* Purpose:	checks if the floating-point is a negative Zero
* Return:	1 when a negative Zero, 0 in the other case
***********************************************************************/
int FP::IsNegZero()
{
  return 0;
/*  if ( sign == 0 )
    return 0;
  int i;
  if ((sizeMant > 0) && (sizeExp > 0)) {
    for (i=0; i < sizeExp ; i++)
      if ( exp.GetBit(i) != 0 ){
        return 0;
      } 
  i=0;
  if (!hidden)
    i++;
  for (;i < sizeMant;i++)
    if (mant.GetBit(i) != 0)
      return 0;  
  }
  return 1;
*/
}
