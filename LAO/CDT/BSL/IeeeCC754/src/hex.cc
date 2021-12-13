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


$Id: hex.cc 78 2005-11-02 20:14:19Z huesken $

                                                                     
*/
/**	\file
 *	\brief Definition of class Hex inherited from Bitstring.
 *
 *	Class Hex manipulates the Bitstring class as a hexadecimal string of bits.
 *	The only difference in the hexadecimal representation is the length of the string,
 *	as it is four times smaller then the binary representation.
 */

#include <Types.h>
#include <Hex.h>
#include <string.h>

/*class implementation*/

/***********************************************************************
* Member:  GetBin(char v)
* Purpose: returns a Bitstring with the corresponding binary value of "v"
* Return:  Bitstring with binary value of "v"
***********************************************************************/
void Hex::GetBin(char v)
{
  switch (v)
    {
    case '0':
      bitstr[0] = 0;
      break;
    case '1':
      bitstr[0] = 1;
      break;
    case '2':
      bitstr[0] = 2;
      break;
    case '3':
      bitstr[0] = 3;
      break;
    case '4':
      bitstr[0] = 4;
      break;
    case '5':
      bitstr[0] = 5; 
      break;
    case '6':
      bitstr[0] = 6; 
      break;
    case '7':
      bitstr[0] = 7; 
      break;
    case '8':
      bitstr[0] = 8; 
      break;
    case '9':
      bitstr[0] = 9; 
      break;
    case 'A':
    case 'a':
      bitstr[0] = 10;
      break;
    case 'B':
    case 'b':
      bitstr[0] = 11; 
      break;
    case 'C':
    case 'c':
      bitstr[0] = 12; 
      break;
    case 'D':
    case 'd':
      bitstr[0] = 13; 
      break;
    case 'E':
    case 'e':
      bitstr[0] = 14; 
      break;
    case 'F':
    case 'f':
      bitstr[0] = 15; 
      break;
    }
}


/***********************************************************************
* Member:  GetHex(Bitstring &b)
* Purpose: It gives the character that is represented in "b"
* Return:  returns b as a character
***********************************************************************/
char Hex::GetHex(Bitstring &b) const
{

  Bitstring temp(4);


  temp.StringToBitstr("0000");
  if(temp == b)
    return '0';

  temp.StringToBitstr("0001");
  if(temp == b)
    return '1';

  temp.StringToBitstr("0010");
  if(temp== b)
    return '2';

  temp.StringToBitstr("0011");
  if(temp == b)
    return '3';

  temp.StringToBitstr("0100");
  if(temp == b)
    return '4';

  temp.StringToBitstr("0101");
  if(temp == b)
    return '5';

  temp.StringToBitstr("0110");
  if(temp == b)
    return '6';

  temp.StringToBitstr("0111");
  if(temp == b)
    return '7';

  temp.StringToBitstr("1000");
  if(temp == b)
    return '8';

  temp.StringToBitstr("1001");
  if(temp == b)
    return '9';

  temp.StringToBitstr("1010");
  if(temp == b)
    return 'A';

  temp.StringToBitstr("1011");
  if(temp == b)
    return 'B';
  
  temp.StringToBitstr("1100") ;
  if(temp == b)
    return 'C';

  temp.StringToBitstr("1101");
  if(temp == b)
    return 'D';

  temp.StringToBitstr("1110");
  if(temp == b)
    return 'E';

  temp.StringToBitstr("1111");
  if(temp == b)
    return 'F';

  return '\0';

}


/***********************************************************************
* Member:  Hex()  Constructor
* Purpose: Create empty hexadecimal bitstring
* Return:  Nothing
***********************************************************************/
Hex::Hex(): Bitstring()
{ }


/***********************************************************************
* Member:  Bitstring(ulong32 size)  Constructor
* Purpose: Create a hexadecimal bitstring of size "size"*4
* Return:  Nothing
***********************************************************************/
Hex::Hex(ulong32 size): Bitstring(size)
{ 
  Resize(size);
}


/***********************************************************************
* Member:  Hex(char * str)  Constructor
* Purpose: Create hexadecimal bitstring with "str" as initiale value
* Return:  Nothing
***********************************************************************/
Hex::Hex(char *hstr)
{
  long32 i,j;
  Hex t1(4),t2(4);
 
  length = strlen(hstr)*4;
  lengthT= sizeof(ulong32) * 8;
  lengthBlock = (length/lengthT)+1;

  bitstr = new ulong32 [lengthBlock];

  for (i=0;i<(long32)lengthBlock;i++)
    bitstr[i]=0;

  j=0;
  for (i= (length/4) -1; i >= 0; i--)
    {
      t1.GetBin(hstr[i--]);
      if (i >= 0)
  {
    t2.GetBin(hstr[i]);
          t2.Concat(t1);
    PutByte(j,t2);
  }
      else
    PutByte(j,t1);
      j++;
    }
}


/***********************************************************************
* Member:  Hex(Bitstring & copy) 
* Purpose: Create a hexadecimal bitstring with initiale value the bitstring
*          "copy"
* Return:  Nothing
***********************************************************************/
Hex::Hex(const Bitstring &copy)
{
  unsigned int i=0;
  
  length = copy.Length();
  lengthT = sizeof(ulong32)*8;
  lengthBlock = (length /lengthT) +1;

  bitstr = new ulong32 [lengthBlock];

  for (i=0 ; i< lengthBlock; i++)
    bitstr[i]= copy[i];
  
  if ((length%4)!= 0)
    Resize(length/4 +1);

}


/***********************************************************************
* Member:  Resize (ulong32 len)
* Purpose: Change the length of the bitstring to "len"*4. If the new length is
*          larger then the length, the bitstring is appended with 0, else
*          the bitstring is truncated to the new length "len"*4
* Return:  Previous length
***********************************************************************/
ulong32 Hex::Resize(ulong32 len)
{
  return Bitstring::Resize(len*4);
}

/***********************************************************************
* Member:  BitstrToString()
* Purpose: Converts a hexadecimal bitstring to a C-string 
* Return:  length >0   ->  The C-string 
*          else        ->  "Bitstring is empty"
***********************************************************************/
void Hex::BitstrToString(char* out) const
{
  Bitstring temp(4);
  unsigned int i;

  out= new char[(length/4)+2];
  for (i = 0 ;i< length/4; i++)
    {
      // cout << "i = " << i << endl << flush;
      SubBitstring(i*4,temp);
      // cout << "temp = " << temp << endl << flush;
      out[length/4 -i -1]=GetHex(temp);
    }
  out[(length/4)]='\0';
}

/***********************************************************************
* Member:  StringToBitstr(char *str)
* Purpose: Converts a C-string to a hexadecimal bitstring
* Return:  converted bitstring 
***********************************************************************/
void Hex::StringToBitstr(char *hstr)
{
  ulong32 i,j,k;
  Bitstring t1(4),t2(4);
  ulong32 tmp;

  length = strlen(hstr)*4;
  lengthT= sizeof(ulong32) * 8;
  if (length % lengthT == 0)
     lengthBlock = length/lengthT;
  else 
     lengthBlock = (length/lengthT)+1;
  if (bitstr)
    delete [] bitstr;
  bitstr = new ulong32 [lengthBlock];
  for (i=0;i<lengthBlock;i++)
    bitstr[i]=0;
  j=0;
  for (i= 0; i < length/4;)  {
    tmp = 0; 
    for (k = 0; k < 7;k++) {
        if (hstr[i] <= '9') 
      tmp += hstr[i++] - '0'; 
        else
            tmp += hstr[i++] - 'a' + 10;
        tmp *= 16;
    }
    if (hstr[i] <= '9')
        tmp += hstr[i++] - '0'; 
    else
        tmp += hstr[i++] - 'a' + 10;
    bitstr[j] = tmp;
    j++;
  } 
}

/***********************************************************************
* Member:  StringToBitstr(string str)
* Purpose: Converts string to a hexadecimal bitstring
* Return:  converted bitstring 
***********************************************************************/
void Hex::StringToBitstr(string hstr)
{
  ulong32 i,j,k;
  Bitstring t1(4),t2(4);
  ulong32 tmp;

  length = hstr.length()*4;
  lengthT= sizeof(ulong32) * 8;
  if (length % lengthT == 0)
     lengthBlock = length/lengthT;
  else 
     lengthBlock = (length/lengthT)+1;
  if (bitstr)
    delete [] bitstr;
  bitstr = new ulong32 [lengthBlock];
  for (i=0;i<lengthBlock;i++)
    bitstr[i]=0;
  j=0;
  for (i= 0; i < length/4;)  {
    tmp = 0; 
    for (k = 0; k < 7;k++) {
        if (hstr[i] <= '9') 
      tmp += hstr[i++] - '0'; 
        else
            tmp += hstr[i++] - 'a' + 10;
        tmp *= 16;
    }
    if (hstr[i] <= '9')
        tmp += hstr[i++] - '0'; 
    else
        tmp += hstr[i++] - 'a' + 10;
    bitstr[j] = tmp;
    j++;
  }  
}
