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


$Id: Bitstring.h 78 2005-11-02 20:14:19Z huesken $

                                                                     
*/
/**	\file
 * 	\brief	Declaration of class Bitstring
 *
 * 	see Bistring class for more information
 *
 */

/***************************************************************************
* This is a class definition for bitstrings. The bitstring is a array of type 
* (class T). The use of templates is to specify the block size. Only use  
* unsigned types to define the class T, because else the shift operations
* (especially right shift) won't work correct.
************************************************************************** */

#ifndef _BITSTRING_H
#define _BITSTRING_H


#include <Types.h>
#include <cmath>
#include <iostream>
using namespace std;


#define NO_ERR               500
#define ERR_OUT_OF_BOUND    501



/*class definition*/
/**	\brief	The Bitstring is an array of type(class T).
 *
 * 	The use of templates is to specify the block size.
 * 	Only use unsigned types to define the class T,
 * 	else the shift operations won't work correctly.
 *
 * 	This class has been created to simplify the manipulation of bits
 * 	over some array of integers (unsigned longs).
 * 	The internal structure of the bitstring resembles a little endian structure,
 * 	because this concept simplifies the resizing of the bitstring.
 *
 * 	@author Bogo Johan
 */



class Bitstring
{
protected:
  ///the bitstring
  ulong32 * bitstr;             
  
  ///length of bitstring,         
  ulong32 length;

  /// number of array elments
  ulong32 lengthBlock;

  ///number of bits in class T
  ulong32 lengthT;  
        
  /// returns a unsigned long  with the first res bits set.
  ulong32 GetPattern(ulong32 rest);

  /// returns the first block of bitstr
  /** It's only defined
    to be used in PutByte so the xor-operator would work
    properly.**/
  ulong32 Convert();

public:

  /// Constructor, creates an empty bitstring
  Bitstring();

  /// Constructor, creates a bitstring of size size.
  /** @param size the default size*/
  Bitstring(ulong32 size);

  /// Constructor,initiate the bitstring with the str as input
  /** @param str the default value for the bitstring  */
  Bitstring(const string& str);

  /// Copy constructor.
  Bitstring(const Bitstring& copy);

  ///Destructor
  virtual ~Bitstring();

  /// Get the length of the bitstring.
  /** @return length of the bitstring*/
  virtual ulong32 Length() const;

  /// Change the length of the bitstring to "len"
  /** If "len" is larger then the length,
      the bitstring is appended with 0,
      else the bitstring is truncated to the new length "len"
    @param len the new length of the bitstring
    @return  previous length */
  virtual ulong32 Resize(ulong32 len);

  /// Get the bit value at position "bit"
  /** @param bit      position of the bit
   @return the bit value 0 <= "bit" < length -> The bit value
            else -> 0 */
  int GetBit(ulong32 bit) const;

  /// Replace the bit value at position "bit" with the new "bitvalue"
  /** @param  bit         posisition of the bit
    @param  bitvalue    new value 
    @return  Previous bit value*/
  int PutBit(ulong32 bit,unsigned int bitvalue);

  /// Replace the bit value at position "bit" with the 1
  /** @param bit position of the bit
     @return Previous bit value*/
  int Set(long32 bit);

  /// Replace the bit value at position "bit" with the 0
  /** @param   bit     position of the bit
    @return  Previous bit value   */
  int Clear(long32 bit);

  /// Clears the bitstring
  /** @return  Cleared bitstring  */
  Bitstring ClearBitString();

  /// Get the byte value at position "byte"
  /** @param  byte    position of the byte
    @return  The byte value */
    void GetByte(ulong32 byte,Bitstring sub) const;

  /// Replace the byte value at position "byte" with the new "bytevalue"
  /** @param  byte    position of the byte
    @param  bytevalue   new value 
    @return:  void 
    Previous byte value */
  void PutByte(ulong32 byte,Bitstring bytevalue);


  /// Increase the bitstring value by 1
  /** @return  Carry after most significant bit*/
  int Inc();

  /// Decrease the bitstring value by 1
  /** @return 1 -> wrap around (negative)
              0 -> OK */
  int Dec();

  /// Converts a bitstring to a C-string 
  /** @return the converted bitstring
   	length > 0 -> The C-string
      	else -> "Bitstring is empty" */
  void BitstrToString(string str) const;

  ///  Converts a C-string to a bitstring
  /** @param  str    the C-string to be converted
    @return  converted bitstring  */
  virtual void StringToBitstr(string str);

  /// Puts bitstring *this in front of b2
  /** bitstring *this in front of b2 -> so the first bit of b2 
          becomes the first bit of *thisb2
    @param   b2   second bitstring
    @return  *this -> *thisb2 */
  void Concat(const Bitstring &b2); 

  /// Returns the substring defined by position "begin" to "count"
  /** @param  begin  the begining of the substring
     @param  count  the length of the substring
     @return  substring from bitstring */
  // Bitstring SubBitstring (unsigned long begin, unsigned long count) const;
     void SubBitstring(ulong32 begin, Bitstring &sub) const;

  /// Overloads the array operator
  /** Returns/change block "n"
    @param n   block number 
    @return  block "n" */
  ulong32& operator [](ulong32 n) const;

  /// Overloads the assign operator
  /** Makes the bistring equal to "copy"
    @param  copy        the new bitstring
    @return  The changed bitstring */
  Bitstring& operator = (const Bitstring &copy);
  
  /// Overloads the equal operator.
  /** @param	b	second bitstring
     @return	equal -> 1
                not equal -> 0 */
  int operator == (const Bitstring & b) const;

  /// Overloads the not equal operator.
  /** @return	not equal -> 1
       		equal -> 0 */
  int operator != (const Bitstring &b) const;

  /// Overloads the greater than operator.
  /** @return	greater -> 1
            	not greater -> 0 */
  int operator > (const Bitstring &b) const;  

  /// Overloads the smaller than operator.
  /** @return	smaller -> 1
         	not smaller -> 0 */
  int operator < (const Bitstring &b) const;


  /// Overloads the shift left operator
  /** Shifts the bitstring "count" bits to the left
    @param count      number of shifts
    @return  shifted bitstring  */
  void operator <<(ulong32 count);

  /// Overloads the shift right operator
  /** Shifts the bitstring "count" bits to the righ
    @param  count   number of shifts
    @return  shifted bitstring */
  void operator >>(ulong32 count );


  /// Overloads the bitwise and operator
  /** Does a bitwise and with "bitstr" and "bitst".
      Makes the size of bitst equal to the bitstring. 
    @param bitst  second bitstring
    @return  bitwised and -> *this \& bitst */
  Bitstring operator &( const Bitstring &bitst);

  /// Overloads the bitwise or operator
  /** Does a bitwise or with "bitstr" and "bitst".
      Makes the size of bitst equal to the bitstring. 
    @param  bitst   second bitstring
    @return  bitwised or -> *this $|$ bitst */
  Bitstring operator |( const Bitstring &bitst);

  /// Overloads the bitwise xor operator
  /** Does a bitwise xor with "bitstr" and "bitst".
      Makes the size of bitst equal to the bitstring.
    @param   bitst    second bitstring
    @return  bitwised xor -> *this \^\ bitst */
  Bitstring operator ^( const Bitstring &bitst);

  /// Overloads the bitwise not operator
  /** Does a bitwise not with "bitstr".
    @return  bitwised not -> \cxxtilde(*this) */
  Bitstring operator ~();

  /// Overloads the stream output operator
  /** Converts the bitstring to a C-string and returns it to "ostream". 
    @param  outs   the ouput stream
    @param  outstr   the bitstring
    @return  outs -> converted bitstring */
  friend ostream& operator << (ostream& outs,const Bitstring &outstr);

  /// Overloads the stream input operator
  /** Converts the C-string to a bitstring.
    @param  ins   the input stream
    @param  instr  the bitstring 
    @return  ins */
  friend istream& operator >> (istream& ins, Bitstring &instr);
}; 

//@Include: Hex.h

#endif

