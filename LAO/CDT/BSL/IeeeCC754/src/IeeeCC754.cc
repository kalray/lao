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


$Id: IeeeCC754.cc 78 2005-11-02 20:14:19Z huesken $

                                                                     
*/
/**	\file
 * 	\brief Driver program. contains main()
 *
 * 	This is the Ieee compliance checker for floating point implementation according to IEEE754-854 specifications
 * 	Check out http://www.win.ua.ac.be/~cant/ieeecc754.html for more information.
 */

#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <sstream>
#include <string.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <Types.h>
#include <Bitstring.h>
#include <DriverFloatRepr.h>
#include <UCB.h>
using namespace std;

UCB<DriverFloatRepr> ucb;

#define TD_OVERFLOW             0x08
#define TD_UNDERFLOW            0x10
/// hex 40	-	inexactness, no denormalization loss
#define TD_UNDERFLOWV           0x40  // inexactness, no denormalization loss
/// hex 80	-	tininess before rounding
#define TD_UNDERFLOWW           0x80  // tininess before rounding

#define TD_INVALID              0x01
#define TD_INEXACT              0x20
#define TD_DIVBYZERO            0x04
#define IEEEDEFAULTENV          0

#define TD_TONEAREST    0  // deze numberlen worden gebruikt       // FP_RN
#define TD_UPWARD       2  // ipv de originele roundingnumberlen   // FP_RP
#define TD_DOWNWARD     3  // zoals die voorkomen in "ieeefp.h"   // FP_RM
#define TD_TOWARDZERO   1  // voor compatibiliteit met de UCB     // FP_RZ
// outputfile qua volgorde van
// rounding
#define ALL_FORMATS 3
#define TESTEVEN 1
#define TESTODD 2
#define NO_FLAGS 31
#define NO_FLAGS_INEXACT      16
#define NO_FLAGS_OVERFLOW     8
#define NO_FLAGS_UNDERFLOW    4
#define NO_FLAGS_INVALID      2
#define NO_FLAGS_DIV_BY_ZERO  1
#define ROUND_ALL  31
#define ROUND_NEAREST  8
#define ROUND_UP       4
#define ROUND_DOWN     2
#define ROUND_ZERO     1
#define ALL_MODES ((1<<TD_TONEAREST) | (1<<TD_DOWNWARD) | (1<<TD_UPWARD) | (1<<TD_TOWARDZERO))
#define NO_OF_BITS   32

// Exit codes
#define ERR_SUCCESS           0
#define ERR_WARN              1
#define ERR_ERROR             2
#define ERR_WARN_AND_ERROR    3
#define ERR_PARSE_CMDLINE     4
#define ERR_FILE              5
#define ERR_MEM               6
#define ERR_PARSE_TESTVECTOR  7
#define ERR_USAGE             255

int use_extended_prec=0;  // only used on x86: do (not) use extd. prec.
int cant_infinity=0 , small_norm=0; 
int HexNoIsGiven=0;
int posDec;
int posUpDec;
int incexp=0;
int NaN1,NaN2,NaN3;
int err1,err2,err3;
int isIeeeVector=0;
ulong32 expon;
ulong32 hidden;
ulong32 mant;
ulong32 length;
ulong32 rest;
ulong32 sign_exp_length;
ulong32 kl_biased_exp;
ulong32 sign_exp_rest ;
FILE *input=NULL;
ulong32 readings;
ulong32  sexpon;
ulong32 dexpon;
ulong32  smant=0;
ulong32 dmant=0;
ulong32  shidden;
ulong32 dhidden;
ulong32  slength;
ulong32  srest;
ulong32 dlength;
ulong32 drest;
ulong32 ssign_exp_length; // sign+exponent length(in multiples of NO_OF_BITS)
ulong32 ssign_exp_rest;
ulong32 dsign_exp_length;
ulong32 dsign_exp_rest;
ulong32 skl_biased_exp;
ulong32 dkl_biased_exp;
ulong32 testFormats;
ulong32 testModes;
ulong32 flushVector;
ulong32 killerVector;
ulong32 *operand1 = NULL, *operand2 = NULL, *result = NULL;
long32 xcp, dots, wrong_input, lines_in, err_form;
char dot, opcode, op2code, op3code, op4code;
int comp_sort;
int CmndLinePos=1, def=0, defe = 0,deft=0,falt =0;
int ucbInput=0;
int check =-1;
int noFlags=0;
int Round =ROUND_ALL;
int dectobin=0;
int tiny=1;
int inf=1;
int NaN=1;
int signedZero=1;
int ieee=1;
string intstr = "";
int skipVector=0,jumpOverflow=0,jumpUnderflow=0,jumpInvalid=0,jumpDivZero=0;
int pre = 0, post = 0; // to accomodate '?' before or after a result
int NaNresult = 0;
int nextdig = 0;
int mysignbit = 0;
char decimal[maxstr];
char updecimal[maxstr];
char *logfilename = (char*)"ieee.log";

// Variable that holds the currently used mode (-u, -c, -o)
// The mode can also be concluded from the variables "check" and "ucbInput":
// "-c" is equivalent to check ==  1 && ucbInput == 0;
// "-u" is equivalent to check == -1 && ucbInput == 1;
// "-o" is equivalent to check ==  0 && ucbInput == 0.
char current_mode = 0;

// infile and logfile
fstream UCBinfile, UCBlogfile;

/// show Usage info and program name and exit with status err
void MyUsage( int err, const char * name = NULL );
/// call MyCleanup, show error message, call exit(err)
void MyExit( int err, const char * errmsg = NULL);
/// clean up (close input/output)
void MyCleanup( void );
/// check if warnings and/or errors occurred while testing
template <class T> int MyCheck( T & t );
/// parse the command line arguments to get the precision and rounding modes
void ParseArgs( int, char** );
/// change coonen to ucb format
void ParsecaseO( int argc, char** argv);
/// specify single precision
void ParsecaseS();
/// default is for double precision else it gets precision specifications for the destination (only used in conversions)
void ParsecaseD( int argc, char** argv);
/// long doulbe precision
void ParsecaseL();
/// quadruple precision
void ParsecaseQ();
/// Multi precision
void ParsecaseM();
/// specify no. of bits for exponent
void ParsecaseE( int argc, char** argv);
/// specify <int> bits precision
void ParsecaseT( int argc, char** argv);
/// specify rounding modes
void ParsecaseR( int argc, char** argv);
/// do not test specified exceptions, denormalized nos., NaNs, signed infinty, signed zeros
void ParsecaseN( int argc, char** argv);
/// jump/skip test vectors raising overflow, underflow, invalid or divide by zero exception
void ParsecaseJ( int argc, char** argv);
/// specify if hidden bit
void ParsecaseH();
/// if change to ucb then calls required fns, else check for errors and opens log file.
void PostParseAction( int argc, char** argv );
void readParam( ulong32 & );
/// reads a test vector from the file and checks if the test SW/HW is compatible with IEEE spcifications 
int readAline( );
/// calculates the sizes of arrays to store numbers according to the precision specifications in the command line
void Calc_General_Stuff( );
/// Slam a dot out to show some life
void putDot( );
/// get rounding modes from the test vector
void getModes( );
/// gets exceptions raised for the test vector
void getExceptions( );
/// gets integer value from the string (also checks for hex no.)
ulong32 getinteger();
/// gets integer value of Hex no.
ulong32 getHex();
/// takes the coded number and precsion to get the number 
void getNumber( ulong32 *number);
/// reads the binary coded number form the test set (ONLY FOR CONVERSIONS)
void getbinary( ulong32 *number);
/// get decimal coded number form testset (ONLY FOR CONVERSION)
void getdecimal( );
/// gets bias for the binary number 
void getBbias(ulong32 *number);
/// get significant of the binary number 
void getBsignificant(ulong32 *number);
/// gets exponent for the binary number
long32 getBexpon();
/// get significand of decimal coded number
void getDsignificand();
/// get exponent of decimal coded number
void getDexpon();
/// to find location to stream with proper spaces 
int FindNextSpace(const string & pstring, int beginpos, int occurance );
/// writes the test vector in UCB fromat
void writeUCB();
/// writes opcode of testvector in the UCB format
void writeUCBopcode(ostringstream &stream);
/// writes precision of testvector in the UCB format
void writeUCBprecision(ostringstream &stream);
/// writes rounding mode of the test vector in the UCB format
void writeUCBround(int m,ostringstream &stream);
/// writes exceptions of the testvector in the UCB format
void writeUCBexcep(ostringstream &stream);
/// writes operands or source for the operation in UCB format
void writeUCBoperand(ostringstream &stream);
/// writes result or destination of the operation in UCB format
void writeUCBresult(int m,ostringstream &stream);
/// if + or * then writes test vector to check comutativity and calls functions to check the test vectors generated
void writeUCBcomutative(ostringstream &stream);
/// round and write the destination for binary to decimal conversion
void writeUCBresultBin (int m, ostringstream &stream);
/// writes destination for decimal to binary conversion
void writeUCBresultDec (int m, ostringstream &stream);
/// writes destination for operations other than b2d and d2b
void writeUCBresultNum (ostringstream &stream);
/// get floating ponit integer in IEEE format
void getFPInteger( ulong32 *number);
/// Sets a number to infinty according to precision
void Infinity( ulong32 *number);
/// sets a number to Q
void Quiet_NaN( ulong32 *number);
/// sets number to S
void Signaling_NaN( ulong32 *number);
/// sets number to samllest normal (or tiny T)
void Smallest_Norm( ulong32 *number);
/// Modifies the rootnumber for case 'p'-plus or case 'm'-minus 
void ModifierPorM(ulong32 *number,int McasePorM);
/// Modifies the rootnumber 
void getModifier(ulong32 *number);
/// Modifies the rootnumber for case 'i'-increment or case 'd'-decrement
void ModifierIorD(ulong32 *number,int McaseIorD);
/// extracts the rootnumber from the coded number
void getFPRootNumber(  ulong32 *number);
/// this is called if the coded number is hexadecimal . This stores hexadecimal number in intstr[] and quits getNumber().
void getFPHexNo();
/// checks if the sign of a number is + or negative ... by default it returns + .
int getSign();
/// assigns values to global variables for precision parsed from the command line.
void initialize_values(int source);
/// checks if exponent is zero
ulong32 null_exp( const ulong32 *number);
/// checks if exponent is one
ulong32 one_exp( const ulong32 *number);
/// operates p<digit> operation in the coded number
void Plus( ulong32 *number, ulong32 k);
/// operates m<digits> operation in coded number
void Minus( ulong32 *number, ulong32 k);
/// multiply B by 2^k ... operation pB<digits>
void PlusBk( ulong32 *number, ulong32 k );
/// divide Bias by 2^k ... operation mB<digits>
void MinusBk( ulong32 *number, ulong32 k);
/// operates i(<pos>)<digit>
void Incr_at_pos( ulong32 *number, ulong32 pos, ulong32 k);
/// operates i<digit> on the coded number
void Incr( ulong32 *number, ulong32 k);
/// operates d(<pos>)<digit>  on the coded number
void Decr_at_pos (ulong32 *, ulong32, ulong32);
/// operates d<digit> on the coded number
void Decr( ulong32 *number, ulong32 k);
/// operates u<digit>
void ModifierU( ulong32 *number, ulong32 ulps);
/// Inserts correct number of zeros at most significant position.
void WriteNumber(ostream &, const ulong32 *, ulong32, ulong32,ulong32, ulong32, const int);
/// avoid spaces while reading a vector
void skipSpace( );
/// apply getc() of <stdio.h> to the present location of file in use
int mygetc( FILE* );
/// apply ungetc() of <stdio.h> to the present location of file in use
void myungetc( int, FILE* );
/// check old ostrstream new ostringstream
//bool checkOutStringStreams(ostrstream &old, ostringstream &nw, string calledFrom);

/*-----------------------------------------------------------------------------
name        :IeeeCC754()
description :this is the Ieee compliance checker for floating point 
		implementation according to IEEE754-854 specifications.
called from :/
call fns    :ParseArgs(),Calc_General_Stuff(),readAline(),putdot(),ucb.close()
exceptions  :/
algorithm   :
Global var. used :
-----------------------------------------------------------------------------*/


extern "C" {
    int IeeeCC754(int, char *[]);

    int IeeeCC754( int argc,char * argv[ ] )
    {
    readings=dots=wrong_input=lines_in=err_form=0;
    CmndLinePos=1, def=0, defe = 0,deft=0,falt =0;
    check =-1;
    if ( argc >1 )
        
        ParseArgs( argc, argv ); /* to parse arguments in the executing statement*/

    else 
        
        {
        MyUsage(ERR_USAGE, argv[0]);
        }
    
    if (ucbInput)
    {
        while (UCBinfile.good() && !UCBinfile.eof()) 
        {
            if (ucb.ReadLine(UCBinfile, signedZero, noFlags))
            {
            ucb.DoLine( tiny,inf,NaN );
            ucb.Compare(std::cout);
            } // if Readline()
        } // while
    } // if ucbInput
    else {
    
    Calc_General_Stuff( );
    
    operand1 = new ulong32[ slength ];
    if ( operand1 == NULL ) 
        {
        MyExit(ERR_MEM, "\nNot enough memory!\nProgram aborted.");
        }
    
    operand2 = new ulong32[ slength ];
    if ( operand2 == NULL ) 
        {
        MyExit(ERR_MEM, "\nNot enough memory!\nProgram aborted.");
        }
    
    result = new ulong32[ dlength ];
    
    if ( result == NULL ) 
        {
        MyExit(ERR_MEM, "\nNot enough memory!\nProgram aborted.");
        }
    
    dot='.';
    
    while ( readAline( ) ) 
        {
        skipVector = 0;
        readings += 1;
//        putDot( );
        }
    
    cout << "\n\nCounting " << readings << " lines." << endl;
    
    } // if !ucbInput
    
    // write summary to logfile (if not in conversion mode...)
    if (current_mode != 'o' && UCBlogfile.is_open()) ucb.Close( tiny , UCBlogfile);
    // clean up and return appropiate value
    MyCleanup();
    return MyCheck(ucb);
    }
}

/// show Usage info and program name and exit with status err
void MyUsage( int err, const char * name )
 {
   char prog[] = "IeeeCC754";
   if (name == NULL) name = prog;
   cout << "Error in calling " << name 
        << "\nFor instructions on calling the program, see the readme.usage file." << endl;
   MyExit(err);
 }
 
/// call MyCleanup, show error message, call exit(err)
void MyExit(int err, const char * errmsg)
 {
   // clean up
   MyCleanup();
   // output error message (if given)
   if (errmsg) cout << errmsg << endl;
   // leave program
   exit(err);
 }

/// clean up (close input/output)
void MyCleanup(void)
 {
   // close input-/output-streams and init where necessary
   if (input)
    {
      fclose(input);
      input = NULL;
    }
   if (UCBinfile.is_open()) UCBinfile.close();
   if (UCBlogfile.is_open()) UCBlogfile.close();
   UCBinfile.clear();
   UCBlogfile.clear();
   // delete global int-arrays...
   if (operand1) delete[] operand1;
   if (operand2) delete[] operand2;
   if (result) delete[] result;
   // ...and init values
   operand1 = NULL;
   operand2 = NULL;
   result = NULL;
 }
   
template <class T>
int MyCheck( T & t )
 {
   //(mh) Calculate return value
   int ret = ERR_SUCCESS;
   if (t.GetWarnings()) ret = ERR_WARN;
   if (t.GetErrors())
    {
      if (ret == ERR_WARN) ret = ERR_WARN_AND_ERROR;
      else ret = ERR_ERROR;
    }
   return ret;
 }

/*-----------------------------------------------------------------------------
name        :ParseArgs()
description :parse the command line arguments to get the precision and 
		rounding modes.
called from :main()
call fns    :ParsecaseO/S/D/L/Q/M/E/T/R/N/J/H , PostParseAction
Global var. used :check, def,defe,deft,flat,ucbInput
-----------------------------------------------------------------------------*/
void ParseArgs( int argc, char** argv )
{
  dmant = 0;
  dexpon = 0;
  shidden=0;
  
  while ( CmndLinePos < argc ) 
    {
      if ( ( strlen( argv[ CmndLinePos ] )  <= 3 )  && ( argv[CmndLinePos][ 0 ] == '-' ) ) 
	{
	  switch ( argv[CmndLinePos][ 1 ] ) 
	    {
	    case 'c':			//coonen format 
	      if ( check == -1 )
          {
		check = 1;
        current_mode = 'c';
          }
	      else
		falt =1;
	      break;
	      
	    case 'u':			//ucb format (precisely one of 
					//	-c | -u |-o should be there)
	      if ( check !=-1 )
		falt =1;
	      def =1;
	      ucbInput =1;
          current_mode = 'u';

	      break;
	      
	    case 'o':			//change coonen to ucb format 
	      ParsecaseO(argc,argv);
	      break;
	      
	      
	    case 's':			//single precision 
	      ParsecaseS();
	      break;
	    case 'd':			//if -d then double presion
	      ParsecaseD(argc,argv);		// else specifications for destination
	      break;
	    case 'l':			//long double precision
	      ParsecaseL();
	      break;
	    case 'q':			//quadruple precision
	      ParsecaseQ();
	      break;
	    case 'm':			//multi precision
	      ParsecaseM();
	      break;
	    case 'e':			//specify no. of bits for exponent
	      ParsecaseE(argc,argv);	
	      break;
	    case 't':			//specify <int> bits precision
	      ParsecaseT(argc,argv);
	      break;
	      
	    case 'r':			//specify rounding modes
	      ParsecaseR(argc,argv);   
	   break;
	      
	    case 'i':			//test conversions within range specified by ieee 
	      ieee=0;
	      break;
	    
	    case 'n':			//do not test specified exceptions, infinity,...
	      ParsecaseN(argc,argv);
	      break;

	    case 'j':			//jump/skip test vectors raising overflow, ...
		ParsecaseJ(argc,argv);
	      break;
	      
	    case 'h':			//specify if hidden bit
	      ParsecaseH();
	      break;

	    case 'f': 			 // name of log file
	      CmndLinePos++;
	      logfilename = argv[CmndLinePos];
	      break;

            case 'x':                    // use extended prec instead
              use_extended_prec=1;       // of single/double on x86
              break;                     // using fesetprecision
	    
	    default:
	      falt = 1;
	    } // end switch
	}
      else 
	{
	  if ( CmndLinePos == argc-1 ) 		//check if test vector file exists 
	    {
	      input = fopen( argv[CmndLinePos], "r" );
	      if ( input == NULL ) 
		{
		  cout << "ERROR: can't open " << argv[CmndLinePos] << "." << endl;
		  falt =1;
		} else
		  cout << "Taking input from " << argv[CmndLinePos] << "." << endl;
	    } else
	      falt = 1;
	}
      
      if ( falt ) 
	{
	  MyUsage(ERR_USAGE, argv[0]);
	}
    
    CmndLinePos++;
    } // end while
  
  
  if ( ( dmant == 0 )  || ( dexpon == 0 ) )   //if destination precision still has initial
    {					      //values then equate them to that of source.
      dmant = smant;			      //(could be changed only for conversions)
      dexpon = sexpon;
      dhidden = shidden;
    } // if
  
  // only test ieee-compliance?
  ucb.SetIeee(ieee);
  
  PostParseAction(argc,argv);

}


/*-----------------------------------------------------------------------------
name        :ParsecaseO()
description :change coonen to ucb format
called from :ParseArgs()
call fns    :\
Global var. used :check,CmndLinePos,falt
-----------------------------------------------------------------------------*/
void ParsecaseO( int argc, char** argv)
{
  if ( check == -1 )
    {
      if ( ++CmndLinePos != argc )
	{
	  if ( argv[CmndLinePos][ 0 ]  != '-' ) 
	    {
	      UCBlogfile.open(argv[CmndLinePos], ios::out);
	      if (!UCBlogfile.good()) 
		{
		  cout << "ERROR: can't create " << argv[CmndLinePos] << "." << endl;
		  falt =1;
		} else
        {
		  cout << "Output sent to " << argv[CmndLinePos] << "." << endl;
          current_mode = 'o';
          }
	    } else
	      falt = 1;
	} else
	  falt = 1;
      check = 0;
    } else
      falt = 1;
}


/*-----------------------------------------------------------------------------
name        :ParsecaseS()
description :specify single precision
called from :ParseArgs()
call fns    :\
Global var. used :sexpon,smant,shidden,def,falt
-----------------------------------------------------------------------------*/
void ParsecaseS()
{
  if ( !def ) 
    {
      sexpon = 8;
      smant = 23;
      shidden = 1;
      def = 1;
    } else
      falt =1;
}


/*-----------------------------------------------------------------------------
name        :ParsecaseD()
description :default is for double precision else it gets precision 
	specifications for the destination (only used in conversions)
called from :ParseArgs()
call fns    :\
Global var. used :CmndLinePos,falt
-----------------------------------------------------------------------------*/
void ParsecaseD( int argc, char** argv)
{
  switch ( argv[CmndLinePos][ 2 ] )  
    {
    case 'h':
      dhidden= 1;
      dmant = dmant -1;
      break;
    case 'e':
      if ( ++CmndLinePos != argc ) 
	{
	  dexpon = atoi( argv[CmndLinePos] );
	  if ( dexpon == 0 ) 
	    {
	      cout << "Error: not a number or zero" << endl;
	      falt =1;
	    }
	}
      break;
    case 't':
      if ( ++CmndLinePos != argc ) 
	{
	  dmant = dmant + atoi( argv[CmndLinePos] );
	  if ( dmant == 0 )
	    cout << "Error: not a number or zero" << endl;
	}
      break;
    case 's':
      dexpon = 8;
      dmant = 23;
      dhidden = 1;
      break;
    case 'd':
      dexpon = 11;
      dmant = 52;
      dhidden = 1;
      break;
    case 'l':
      dexpon = 15;
      dmant = 64;
      dhidden = 0;
      break;
    case 'q':
      dexpon = 15;
      dmant = 112;
      dhidden = 1;
      break;
    case 'm':
      dexpon = 15;
      dmant = 240;
      dhidden = 0;
      break;
    default:
      if ( !def )  
	{
	  sexpon = 11;
	  smant = 52;
	  shidden = 1;
	  def = 1;
	} else
	  falt =1;
      break;
    } // switch
}


/*-----------------------------------------------------------------------------
name        :ParsecaseL()
description :long doulbe precision
called from :ParseArgs()
call fns    :\
Global var. used :
-----------------------------------------------------------------------------*/
void ParsecaseL()
{
  if ( !def ) 
    {
      sexpon = 15;
      smant = 64;
      shidden = 0;
      def = 1;
    } else
      falt =1;
}


/*-----------------------------------------------------------------------------
name        :ParsecaseQ()
description :quadruple precision
called from :ParseArgs()
call fns    :\
Global var. used :
-----------------------------------------------------------------------------*/
void ParsecaseQ()
{
  if ( !def ) 
    {
      sexpon = 15;
      smant = 112;
      shidden = 1;
      def = 1;
    } else
      falt =1;
}


/*-----------------------------------------------------------------------------
name        :ParsecaseM()
description :Multi precision
called from :ParseArgs()
call fns    :\
Global var. used :
-----------------------------------------------------------------------------*/
void ParsecaseM()
{
  if ( !def )
    {
      sexpon = 15;
      smant = 240;
      shidden = 0;
      def = 1;
    } else
      falt =1;
}


/*-----------------------------------------------------------------------------
name        :ParsecaseE()
description :specify no. of bits for exponent
called from :ParseArgs()
call fns    :\
Global var. used :CmndLinePos,falt
-----------------------------------------------------------------------------*/
void ParsecaseE( int argc, char** argv)
{	  
  if ( !def ) 
    {
      if ( ++CmndLinePos != argc ) 
	{
	  sexpon = atoi( argv[CmndLinePos] );
	  if ( sexpon == 0 ) 
	    {
	      cout << "Error: not a number or zero" << endl;
	      falt =1;
	    }
	  defe =1;
	} else
	  falt = 1;
    } else
      falt =1;
}


/*-----------------------------------------------------------------------------
name        :ParsecaseT()
description :specify <int> bits precision
called from :ParseArgs()
call fns    :\
Global var. used :CmndLinePos,falt
-----------------------------------------------------------------------------*/
void ParsecaseT( int argc, char** argv)
{
  if( !def ) 
    {
      if ( ++CmndLinePos != argc ) 
	{
	  smant = smant + atoi( argv[CmndLinePos] );
	  if ( smant == 0 ) 
	    {
	      cout << "Error: not a number or zero" << endl;
	      falt =1;
	    }
	  deft=1;
	} else
	  falt =1;
    } else
      falt =1;
}


/*-----------------------------------------------------------------------------
name        :ParsecaseR()
description :specify rounding modes
called from :ParseArgs()
call fns    :\
Global var. used :
-----------------------------------------------------------------------------*/
void ParsecaseR( int argc, char** argv)
{	
  int j;
  Round = 0;
  if ( ++CmndLinePos != argc ) 
    {
      if ( ( argv[CmndLinePos][ 0 ]  != '-' )  && ( CmndLinePos != argc -1 ) ) 
	{
	  j=0;
	  while ( argv[CmndLinePos][ j ]  != '\0' ) 
	    {
	      switch( argv[CmndLinePos][ j ] )
		{
		case 'n':
		  Round |= ROUND_NEAREST;
		  break;
		case 'p':
		  Round |= ROUND_UP;
		  break;
		case 'm':
		  Round |= ROUND_DOWN;
		  break;
		case 'z':
		  Round |= ROUND_ZERO;
		  break;
		}
	      j++;
	    } // while
	}
      else 
	{
	  Round = ROUND_ALL;     // Test all
	  CmndLinePos--;
	}
    } else
      falt = 1;
}


/*-----------------------------------------------------------------------------
name        :ParsecaseN()
description :do not test specified exceptions, denormalized nos., NaNs,
		 signed infinty, signed zeros
called from :ParseArgs()
call fns    :\
Global var. used :
-----------------------------------------------------------------------------*/
void ParsecaseN( int argc, char** argv)
{
  int j;
  if ( ++CmndLinePos != argc ) 
    {
      if ( ( argv[CmndLinePos][ 0 ]  != '-' )  && ( CmndLinePos != argc -1 ) ) 
	{
	  j=0;
	  while ( argv[CmndLinePos][ j ]  != '\0' ) 
	    {
	      switch( argv[CmndLinePos][ j ] ) 
		{
		  /*
		    case 'i':
		    noFlags |= NO_FLAGS_INVALID;
		    break;
		  */
		case 'o':
		  noFlags |= NO_FLAGS_OVERFLOW ;
		  break;
		case 'x':
		  noFlags |= NO_FLAGS_INEXACT;
		  break;
		case 'z':
		  noFlags |= NO_FLAGS_DIV_BY_ZERO;
		  break;
		case 'u':
		  noFlags |= NO_FLAGS_UNDERFLOW;
		  break;
		case 't':
		  if ( ( argv[CmndLinePos][ j+1 ]  == 'i' )  &&
		       ( argv[CmndLinePos][ j+2 ]  == 'n' )  &&
		       ( argv[CmndLinePos][ j+3 ]  == 'y' ) )  
		    {
		      tiny = 0;
		      j += 3;
		    } else
		      falt = 1;
		  break;
		case 'i':
		  if ( ( argv[CmndLinePos][ j+1 ]  == 'n' )  &&
		       ( argv[CmndLinePos][ j+2 ]  == 'f' )  )  {
		    inf = 0;
		    j += 2;
		  } else 
		    {
		      noFlags |= NO_FLAGS_INVALID;
		    }
		  break;
		case 'n':
		  if ( ( argv[CmndLinePos][ j+1 ]  == 'a' )  &&
		       ( argv[CmndLinePos][ j+2 ]  == 'n' ) )  
		    {
		      NaN = 0;
		      j += 2;
		    } else
		      falt = 1;
		  break;
		case 's':
		  if ( ( argv[CmndLinePos][ j+1 ]  == 'n' )  &&
		       ( argv[CmndLinePos][ j+2 ]  == 'z' ) )  
		    {
		      signedZero = 0;
		      j += 2;
		    } else
		      falt = 1;
		  break;
		}
	      j++;
	    } // while
	}
      else 
	{
	  noFlags = NO_FLAGS;     //Test no flags (all values selected)
	  CmndLinePos--;
	}
    } else
      falt = 1;
}


/*-----------------------------------------------------------------------------
name        :ParsecaseJ()
description :jump/skip test vectors raising overflow, underflow, invalid or 
		divide by zero exception
called from :ParseArgs()
call fns    :\
Global var. used :
-----------------------------------------------------------------------------*/
void ParsecaseJ( int argc, char** argv)
{
  int j;
  if ( ++CmndLinePos != argc ) 
    {
      if ( ( argv[CmndLinePos][ 0 ]  != '-' )  && ( CmndLinePos != argc -1 ) ) 
	{
	  j=0;
	  while ( argv[CmndLinePos][ j ]  != '\0' ) 
	    {
	      switch( argv[CmndLinePos][ j ] ) {
	      case 'i':
		jumpInvalid= 1;
		break;
	      case 'o':
		jumpOverflow= 1;
		break;
	      case 'u':
		jumpUnderflow= 1;
		break;
	      case 'z':
		jumpDivZero= 1;
		break;
	      default:
		falt = 1;
		break;
	      }
	      j++;
	    } // while
	}
    } else
      falt = 1;
}


/*-----------------------------------------------------------------------------
name        :ParsecaseH()
description :specify if hidden bit
called from :ParseArgs()
call fns    :\
Global var. used :
-----------------------------------------------------------------------------*/
void ParsecaseH()
{
  if( !def )
    {shidden= 1;
    smant = smant -1;}
  else
    falt =1;
}


/*-----------------------------------------------------------------------------
name        :PostParseAction()
description :if change to ucb then calls required fns, 
		else check for errors and opens log file.
called from :ParseArgs()
call fns    :ucb.OpenLogFile(),ucb.ReadLine(),ucb.DoLine(),ucb.Close
Global var. used :check, def,defe,deft,flat,ucbInput
-----------------------------------------------------------------------------*/
void PostParseAction( int argc, char** argv )
{

  if ( !ucbInput )  
    { // check float formats
      if ( ( sexpon > 32 )  || ( dexpon> 32 ) )  
	{
	  MyExit(ERR_PARSE_CMDLINE, "Exit: exponent size too big (> 32)");
	} else if ( ( smant+shidden <24 )  || ( dmant+dhidden <24 )  || ( sexpon < 8 )  || ( dexpon < 8 ) )  
	  {
	    MyExit(ERR_PARSE_CMDLINE, "Exit: floating-point format too small");
	  } else if ( ( smant + shidden < sexpon )  || ( dmant + dhidden < dexpon ) )  
	    {
	      MyExit(ERR_PARSE_CMDLINE, "Exit: Precision t must at least equal exponent size");
	    } else if ( ( smant + shidden > ((ulong32)1 << (sexpon-1)))  || ( dmant + dhidden > ((ulong32)1 << (dexpon-1)) ) )  
	      {
		MyExit(ERR_PARSE_CMDLINE, "Exit: Precision t > Bias + 1 = 2^(e-1)");
	      }
    } // if
  
  if ( ( !def )  & ( !deft | !defe ) ) 
    {
      MyUsage(ERR_USAGE, argv[0]);
    }

  
  if ( check )    
   {  
     // open log file, connect to ucb-stream and dump command line
     UCBlogfile.open(logfilename, ios::out);
     if (!UCBlogfile.good())
      {
        cerr << "Error opening outfile " << logfilename << ", leaving..."  << endl;
        MyExit(ERR_FILE);
      }
     else
      {
        UCBlogfile << "Testrun: ";
        for (int i=0; i<argc; i++) UCBlogfile << argv[i]  << " ";
        UCBlogfile << endl << endl << flush;
      }
   }

  
  if ( ucbInput ) 
    {
      if ( input ) 
      {
        fclose(input);
        input = NULL;
      }
        
        // open input file & connect to ucb-class
        UCBinfile.open(argv[argc-1], ios::in);
        if (!UCBinfile.good())
         {
           cerr << "Error opening infile " << argv[argc-1] << ", leaving..."  << endl;
           MyExit(ERR_FILE);
         }
    }

 
  else if ( ( input == NULL )  | ( ( !UCBlogfile.good() )  & ( check == -1 ) ) ) 
    {
      if ( input == NULL )
	cout << "Error: No input \n";
      else
	cout << "Error: No output\n";
      
      MyUsage(ERR_FILE, argv[0]);
    }
}


/*-----------------------------------------------------------------------------
name        :Calc_General_Stuff()
description :calculates the sizes of arrays to store numbers according to the 
		precision specifications in the command line 
called from :main()
call fns    :/
exceptions  :/
algorithm   :
Global var. used :slength,srest,sexpon,smant,delength,drest,dexpon,dmant,
			ssign_exp_length,ssign_exp_rest,skl_biased_exp,dkl_biased_exp,
			dsign_exp_length,dsign_exp_rest.
-----------------------------------------------------------------------------*/
void Calc_General_Stuff( )
{
  //to calculate length of arrays used 
  // change BV 17/01/01 

  /*  comments regarding the calculated variables
		s for source , d for destination.
	slength ---- length of FP format in multiples of <no. of bits>
	srest   ---- no. of bits required for FP format in 
		the last of these allocated array elemnt 
	ssign_exp_length ---number of array elements to
		represent all the bits in exponent + sign bit
	ssign_exp_rest --- no. of bits required for sign + exponent in 
		the last of these allocated array elemnt - 1 	
	NO_OF_BITS--- number of bits in array element
  */
 
  slength = ( ulong32 ) ceil( ( sexpon+smant+1 ) /float(NO_OF_BITS));
  srest = ( sexpon+smant+1 )  % NO_OF_BITS;

  dlength = ( ulong32 ) ceil( ( dexpon+dmant+1 ) /float(NO_OF_BITS));
  drest = ( dexpon+dmant+1 )  % NO_OF_BITS;

  ssign_exp_length = ( sexpon/NO_OF_BITS )  + 1;
  ssign_exp_rest = sexpon % NO_OF_BITS;

  dsign_exp_length = ( dexpon/NO_OF_BITS )  + 1;
  dsign_exp_rest = dexpon % NO_OF_BITS;

  skl_biased_exp = 1L << ( NO_OF_BITS-ssign_exp_rest-1 );
  dkl_biased_exp = 1L << ( NO_OF_BITS-dsign_exp_rest-1 );

}


/*-----------------------------------------------------------------------------
name        :readAline()
description :reads a test vector from the file and checks if the test 
		software/hardware is compatible with IEEE spcifications 
called from :main()
call fns    :getbinary(),getNumber(),getdecimal()isspace(),mygetc(),myungetc(),
		getModes(),getException(),skipSpace(),
		initialize_values(),writeUCB().
exceptions  :/
algorithm   :/
Global var. used :lots
-----------------------------------------------------------------------------*/
int readAline( )
{
  char c;
  err1=0,err2=0,err3=0, isIeeeVector=0;
  comp_sort=0;
  err_form=0;
  killerVector=0;
  int NaN1, NaN2, NaN3;
  int source=0;


  pre = post = 0;
  NaN1 = NaN2 = NaN3 = 0;

  while ( ( ( c = mygetc( input ) )  == '!' )  || ( c == '\n' )  || ( c == ' ' ) )  
    {
      if ( c == '!' )
	/* Skip rest of comment line. */
	while ( ( c = mygetc( input ) )  != '\n' )
	  ;
      if ( c == '\n' )
	lines_in++;
    }

  if ( c == EOF )
    return 0;

  opcode = mygetc( input ); // 2nd character is the first character of the opcode
  op2code=op3code = op4code = ' ';
  testFormats=0;
  c = mygetc( input );
  
  if ( !isspace( c ) )  
    {
      op2code = c;
    }
  // seek further opcode bytes 
  while ( !isspace( c ) ) c = mygetc( input ); // and throw them away because we only need first char
  
  
  // Get the precision specifications 
  while ( isspace( c ) ) c= mygetc( input );
  switch ( c ) {
    case 's':
      op3code='s'; 
      break;
    case 'd':
      op3code='d'; 
      break;
    case 'l':
      op3code='l'; 
      break;
    case 'q':
      op3code='q'; 
      break;
    case 'm':
      op3code='m';  
      break;
    case 'e':
      testFormats= TESTEVEN;
      break;
    case 'o':
      testFormats= TESTODD;
      break;
  default:
    op3code='z'; // op3code z-> no precisions specification
    myungetc( c, input); // put the current char back because it does not belong to precision specs
    break;
  }
  // check if test-vector precision specs correspond with command-line provided precision specs 
  if ( op3code !='z' && op3code !='e' && op3code!='o' ) 
    {
      if ( ( (sexpon==8)  && ( (shidden+smant) ==24 )  && (op3code != 's') )  ||
	   ( ( ( (sexpon!=8)  ||  (shidden+smant)!=24 ) )  && (op3code == 's') )  || 
	   ( ( sexpon==11 )  && ( (shidden+smant)==53)  && (op3code != 'd') )   ||
	   ( ( (sexpon!=11)  || ( (shidden+smant)!=53) )  && (op3code == 'd') ) || 
	   ( ( sexpon==15 )  && ((shidden+smant)==64 )  && ( op3code != 'l' ) )  ||
	   ( ( ( sexpon!=15 )  || ((shidden+smant)!=64 ) )  && ( op3code == 'l' ) )  || 
	   ( ( sexpon==15 )  && ((shidden+smant)==113 )  && ( op3code != 'q' ) )  ||
	   ( ( ( sexpon!=15 )  || ((shidden+smant)!=113 ) )  && ( op3code == 'q' ) )  || 
	   ( ( sexpon==15 )  && ((shidden+smant)==240 )  && ( op3code != 'm' ) )  ||
	   ( ( ( sexpon!=15 )  || ((shidden+smant)!=240 ) )   && ( op3code == 'm' ) )  )  {
	while ( ( c = mygetc( input ) )  != '\n' )  /* Skip rest of line. */
	  ;
	return 1; // precision dependent test vectors
      }
    }
  c = mygetc( input );
  if ( op3code !='z' && op3code !='e' && op3code!='o' ) 
    {
      switch ( c ) 
	{
	case 'i':
	  isIeeeVector= 1; 
	  while ( !isspace( c ) ) c= mygetc( input ); // read the eee and throw it away!
	  break;
	}
    }

  myungetc( c, input);
  skipSpace( );
  getModes( );
  skipSpace( );
  
  if ( opcode == 'b' )  
    {
	source=1;					//for source
	initialize_values(source);			
      getbinary(operand1);
      for (ulong32 i=0; i<slength;i++ )  // init operand2
	operand2[ i ] =0;
    } else if ( opcode == 'd' )  {
      getdecimal( );
      for (ulong32 i=0; i<slength;i++ )  // init operand2
	operand2[ i ] =0;
    } else 
      {
	source=1;					//for source
	initialize_values(source);	
	getNumber(operand1);
	
	if ( NaNresult )
	  NaN1 = 1;
    
	if ( wrong_input )
	  err1=1;
	
	skipSpace( );
	source=1;					//for source
	initialize_values(source);
	getNumber(operand2);
    
	if ( NaNresult )
	  NaN2 = 1;
	
	if ( wrong_input )
	  err2=1;
      }
  
  skipSpace( );
  getExceptions( );
  skipSpace( );
  
  if ( opcode == 'b' )  
    {
      getdecimal( );
    }
  else if ( opcode == 'd' )
  {
	source=1;			   		//for source
  	  initialize_values(source);
	getbinary(result);
  }
       else 
	 {
	source=0;					//for destination
	initialize_values(source);
	
	   getNumber(result);
	   if ( NaNresult )
	     NaN3 = 1;
	   if ( wrong_input )
	     err3=1;
	   if ( opcode != 'C' )
	     comp_sort=6;
	 }
  writeUCB();
  if ( err_form )
    cout << "\nWrong input on line " << lines_in+readings+1 << endl;
  while ( c != '\n' )
    c = mygetc( input );
  return 1;
}


/*-----------------------------------------------------------------------------
  name        :writeUCB
  description :writes the test vector in UCB fromat
  called from :readAline()
  calls fns   :writeUCBopcode(),writeUCBprecision(),writeUCBround(),
		writeUCBexcep(),writeUCBoperand,writeUCBresult()
		writeUCBcomutaitive()
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCB()
{
  int m;
  if (  (  ( ( smant + shidden )  % 2 == 0 )  && ( testFormats == TESTEVEN ) )  |
        (  ( ( smant + shidden )  % 2 != 0 )  && ( testFormats == TESTODD ) )   |
        ( testFormats == ALL_FORMATS )  )  
    {
      for ( m=0;m<4;m++ ) 
	{
	ostringstream newstream;
	  
	  if ( ( testModes >> m )  & 0x1 )  
	    {
		writeUCBopcode(newstream);
		writeUCBprecision(newstream);
		writeUCBround(m, newstream);
		writeUCBexcep(newstream);
		writeUCBoperand(newstream);
		writeUCBresult(m, newstream);
	    	newstream<<endl<<ends;
		writeUCBcomutative( newstream);
	     }
	}
	intstr.clear();
    }
  
}


/*-----------------------------------------------------------------------------
  name        :writeUCBopcode()
  description :writes opcode of testvector in the UCB format
  called from :writeUCB
  calls fns   :
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCBopcode(ostringstream &stream)
{
  
  switch ( opcode )  {
  case '+' :
    stream << "add";
    break;
  case '-' :
    stream << "sub";
    break;
  case '*' :
    stream << "mul";
    break;
  case '/' :
    stream << "div";
    break;
  case '%' :
    stream << "rem";
    break;
  case 'V' :
    stream <<"sqrt";
    break;
  case 'S' :
    stream <<"sqrt";
    break;
  case 'r':
    switch ( op2code )  {
    case ' ':
      stream <<"rt";
      break;
    case 'i':
      stream <<"ri";
      break;
    case 'u':
      stream <<"ru";
      break;
    case 'I':
      stream <<"rI";
      break;
    case 'U':
      stream <<"rU";
      break;
    } // switch
    break;
  case 'c':
    switch ( op2code )  {
    case ' ':
      stream <<"ct";
      break;
    case 'i':
      stream <<"ci";
      break;
    case 'u':
      stream <<"cu";
      break;
    case 'I':
      stream <<"cI";
      break;
    case 'U':
      stream <<"cU";
      break;
    } // switch
    break;
  case 'i':
    stream <<"i";
    break;
  case 'b':
    stream <<"b2d";
    break;
  case 'd':
    stream << "d2b";
    break;
  default:
    cout << "\n\nError on line " << lines_in+readings+1 
         << "of onputfile.\nERROR: unrecognizable operation: " << opcode
         << "\n\n\nDone with " << readings << " readings.";
    MyExit(ERR_PARSE_TESTVECTOR);
  }
}


/*-----------------------------------------------------------------------------
  name        :writeUCBprecision()
  description :writes precision of testvector in the UCB format
  called from :writeUCB
  calls fns   :
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCBprecision(ostringstream &stream)
{
  if ( ( shidden )  && ( sexpon==8 )  && ( smant==23 ) )
    if ( !isIeeeVector ) {
      stream <<"s ";
    }
    else {
      stream <<"S ";
    }
  else if ( ( shidden )  && ( sexpon==11 )  && ( smant==52 ) )
    if ( !isIeeeVector ) {
      stream <<"d ";
    }
    else {
      stream <<"D ";
    }
  else if ( !( shidden )  && ( sexpon==15 )  && ( smant==64 ) )
    if ( !isIeeeVector ) {
      stream <<"l ";
    }
    else {
      stream <<"L ";
    }
  else if ( ( shidden )  && ( sexpon==15 )  && ( smant==112 ) )
    if ( !isIeeeVector ) {
      stream <<"q ";
    }
    else {
      stream <<"Q ";
    }
  else if ( !( shidden )  && ( sexpon==15 )  && ( smant==240 ) )
    if ( !isIeeeVector ) {
      stream <<"m ";
    }
    else {
      stream <<"M ";
    }
  else {
    stream << sexpon << " "<< shidden <<" "<< smant <<" " ;
  }
  
  
  if ( ( ( opcode == 'r' )  || ( opcode == 'c' ) )  && ( op2code == ' ' ) ) {
    if ( ( dhidden )  && ( dexpon==8 )  && ( dmant==23 ) ) {
      stream <<"s ";
    }
    else if ( ( dhidden )  && ( dexpon==11 )  && ( dmant==52 ) ) {
      stream <<"d ";
    }
    else if ( !( dhidden )  && ( dexpon==15 )  && ( dmant==64 ) ) {
      stream <<"l ";
    }
    else if ( ( dhidden )  && ( dexpon==15 )  && ( dmant==112 ) ) {
      stream <<"q ";
    }
    else if ( !( dhidden )  && ( dexpon==15 )  && ( dmant==240 ) ) {
      stream <<"m ";
    }
    else {
      stream << dexpon << " "<< dhidden <<" "<< dmant <<" " ;
    }
  }
}


/*-----------------------------------------------------------------------------
  name        :writeUCBround
  description :writes rounding mode of the test vector in the UCB format
  called from :writeUCB
  calls fns   :
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCBround(int m,ostringstream &stream)
{
  switch( m )  {
  case 0 :
    stream <<"n ";
    break;
  case 1 :
    stream <<"z ";
    break;
  case 2 :
    stream <<"p ";
    break;
  case 3 :
    stream <<"m ";
    break;
  }
}


/*-----------------------------------------------------------------------------
  name        :writeUCBexcep()
  description :writes exceptions of the testvector in the UCB format
  called from :
  calls fns   :
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCBexcep(ostringstream &stream)
{
  if ( !killerVector ) {
    stream <<"eq ";
  }
  else {
    stream <<"uo ";
  }
  
  if ( xcp==0 ) {
    stream <<"-";
  }
  if ( xcp & long32( TD_INVALID ) ) {
    stream <<"v";
  }
  if ( xcp & long32( TD_INEXACT ) ) {
    stream <<"x";
  }
  if ( xcp & long32( TD_OVERFLOW ) ) {
    stream <<"o";
  }
  if ( xcp & long32( TD_UNDERFLOW ) ) {
    stream <<"u";
  }
  if ( xcp & long32( TD_UNDERFLOWV ) ) {
    stream <<"a";
  }
  if ( xcp & long32( TD_UNDERFLOWW ) ) {
    stream <<"b";
  }
  if ( xcp & long32( TD_DIVBYZERO ) ) {
    stream <<"d";
  }
}


/*-----------------------------------------------------------------------------
  name        :writeUCBoperand()
  description :writes operands or source for the operation in UCB format
  called from :writeUCB
  calls fns   :WriteNumber()
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCBoperand(ostringstream &stream)
{
  if ( ( !intstr.empty() )  && ( opcode == 'c' ) )  
    {
      stream << " ";
      stream << intstr.c_str();
      if ( NaN2 )
	NaNresult = 1;
      else
	NaNresult = 0;
	WriteNumber(stream, operand2,sexpon,shidden,smant,slength,err1);
    } else 
      {
	
	if ( NaN1 )
	  NaNresult = 1;
	else
	  NaNresult = 0;
	
	if ( opcode == 'd' )  
	  {
	    stream << " ";
	    stream << decimal;
	  } else 
	    {
		WriteNumber(stream, operand1,sexpon,shidden,smant,slength,err1);
	    }
	
	if ( NaN2 )
	  NaNresult = 1;
	else
	  NaNresult = 0;
	  WriteNumber(stream, operand2,sexpon,shidden,smant,slength,err1);
      }
}


/*-----------------------------------------------------------------------------
  name        :writeUCBresult()
  description :writes result or destination of the operation in UCB format
  called from :writeUCB
  calls fns   :writeUCBresultBin(),writeUCBresultDec(),writeUCBresultNum()
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCBresult(int m,ostringstream &stream)
{
  if ( ( !intstr.empty() )  && ( opcode == 'r' ) )  
    {
      stream << " ";
      if ( pre ) {
	stream << "?";
      }
      stream << intstr.c_str();
      if ( post ) {
	stream << "?";
      }
    } else if ( opcode == 'b' )  {
	writeUCBresultBin(m,stream);
    }else if ( opcode == 'd' ) {
	writeUCBresultDec(m,stream);
    }else {
	writeUCBresultNum(stream);
    }
}


/*-----------------------------------------------------------------------------
  name        :writeUCBresultBin()
  description :round and write the destination for binary to decimal conversion 
  called from :writeUCBresult()
  calls fns   :
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCBresultBin(int m,ostringstream &stream)
{
  // check rounding mode
  stream << " ";
  switch( m )  {
  case 0 :
    switch ( nextdig )  {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      stream << decimal;
      break;
    default:
      stream << updecimal;
      break;
    }
    break;
  case 1 :
    stream << decimal;
    break;
  case 2 :
    if ( mysignbit ) {  // negative
      stream << decimal;
    } else {
      stream << updecimal;
    }
    break;
  case 3 :
    if ( mysignbit ) {
      stream << updecimal;
    } else {
      stream << decimal;
    }
    break;
  default:
    MyExit(ERR_PARSE_TESTVECTOR);
  }
}


/*-----------------------------------------------------------------------------
  name        :writeUCBresultDec()
  description :writes destination for decimal to binary conversion
  called from :writeUCBresult()
  calls fns   :WriteNumber()
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCBresultDec(int m,ostringstream &stream)
{
int source;
  // check rounding mode
  if ( pre ) {
    stream << "?";
  }
  
  if ( NaN3 )
    NaNresult = 1;
  else
    NaNresult = 0;
  
  for (ulong32 i = 0; i < dlength; i++ )	//different form Papadopoulo-release:unsigned
    operand1[ i ]  = result[ i ];
  
  if ( nextdig )
    {
      source=0;		  		//for destination
      initialize_values(source);
      Incr( operand1, 1);
    }    
  switch( m )  {
  case 0 :
    switch ( nextdig )  {
    case 0:
    case 1:{
	WriteNumber(stream, result,dexpon,dhidden,dmant,dlength,err3);
      break;
    }
    default:{
	WriteNumber(stream, operand1,dexpon,dhidden,dmant,dlength,err3);
      break;
    }
  }
    break;
  case 1 :{
	WriteNumber(stream, result,dexpon,dhidden,dmant,dlength,err3);
    break;
  }
  case 2 :
    if ( mysignbit )  
      {// negative
	WriteNumber(stream, result,dexpon,dhidden,dmant,dlength,err3);
      } else 
	{
	WriteNumber(stream, operand1,dexpon,dhidden,dmant,dlength,err3);
	}
    break;
  case 3 :
    if ( mysignbit )  
      {
	WriteNumber(stream, operand1,dexpon,dhidden,dmant,dlength,err3);
      } else 
	{
	WriteNumber(stream, result,dexpon,dhidden,dmant,dlength,err3);
	}
    break;
  default:
    MyExit(ERR_PARSE_TESTVECTOR);
  }
  if ( post ) {
    stream << "?";
  }
}


/*-----------------------------------------------------------------------------
  name        :writeUCBresultNum()
  description :writes destination for operations other than b2d and d2b
  called from :writeUCBresult()
  calls fns   :WriteNumber()
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCBresultNum(ostringstream &stream)
{
  if ( pre ) {
    stream << "?";
  }
  if ( NaN3 )
    NaNresult = 1;
  else
    NaNresult = 0;
	WriteNumber(stream, result,dexpon,dhidden,dmant,dlength,err3);
  if ( post ) {
    stream << "?";
  }
}


/*-----------------------------------------------------------------------------
  name        :writeUCBcomutative()
  description :if + or * then writes test vector to check comutativity and 
		calls functions to check the test vectors generated
  called from :writeUCB
  calls fns   :ucb.ReadLine(),ucb.DoLine()
  Global var. used :/
  -----------------------------------------------------------------------------*/
void writeUCBcomutative(ostringstream &stream)
{
  string outputStream2;
  string op1,op2;
  
  if (  opcode=='+' || opcode=='*' )  
    {
      // if the operation is add or mult we have to
      // generate second line with operands switched places
      // in order to check correctness with concern to
      // commutativity law.
      
      int i,j,k;
      outputStream2 = stream.str();
      
      switch (outputStream2[3])
	{
	case 's': 
	  i= FindNextSpace( outputStream2,10,1 );
	  j= FindNextSpace( outputStream2,i+1,1 );
	  op1= outputStream2.substr( i+1,j-i );
	  k= FindNextSpace( outputStream2, j+1,1 );
	  op2= outputStream2.substr( j+1,k-j );
	  outputStream2.replace( i+1,op2.length( ),op2 );
	  outputStream2.replace( i+1+op2.length( ),op1.length( ),op1 );
	  break;
	  
	case 'd':
	  i= FindNextSpace( outputStream2,10,1 );
	  j= FindNextSpace( outputStream2,i+1,2 );
	  op1= outputStream2.substr( i+1,j-i );
	  k= FindNextSpace( outputStream2, j+1,2 );
	  op2= outputStream2.substr( j+1,k-j );
	  outputStream2.replace( i+1,op2.length( ),op2 );
	  outputStream2.replace( i+1+op2.length( ),op1.length( ),op1 );
	  break;  
	  
	case 'q':
	  i= FindNextSpace( outputStream2,10,1 );
	  j= FindNextSpace( outputStream2,i+1,4 );
	  op1= outputStream2.substr( i+1,j-i );
	  k= FindNextSpace( outputStream2, j+1,4 );
	  op2= outputStream2.substr( j+1,k-j );
	  outputStream2.replace( i+1,op2.length( ),op2 );
	  outputStream2.replace( i+1+op2.length( ),op1.length( ),op1 );
	  break;
	  
	case 'l':
	  i= FindNextSpace( outputStream2,10,1 );
	  j= FindNextSpace( outputStream2,i+1,3 );
	  op1= outputStream2.substr( i+1,j-i );
	  k= FindNextSpace( outputStream2, j+1,3 );
	  op2= outputStream2.substr( j+1,k-j );
	  outputStream2.replace( i+1,op2.length( ),op2 );
	  outputStream2.replace( i+1+op2.length( ),op1.length( ),op1 );
	  break;
	  
	case 'm':
	  i= FindNextSpace( outputStream2,10,1 );
	  j= FindNextSpace( outputStream2,i+1,8 );
	  op1= outputStream2.substr( i+1,j-i );
	  k= FindNextSpace( outputStream2, j+1,8 );
	  op2= outputStream2.substr( j+1,k-j );
	  outputStream2.replace( i+1,op2.length( ),op2 );
	  outputStream2.replace( i+1+op2.length( ),op1.length( ),op1 );
	  
	default:      
	  // mantsize and expsize were given explicitly
	  
	  int opLength=smant+sexpon+1; 
	  int blocks;
	  
				// Calculate operand length as multiple of 32
	  opLength= ( opLength/32 ) *32 + ( !( !( opLength%32 ) ) ) * 32;
	  
				// Calculate nr of 32 bit blocks for operand
	  blocks=opLength/32;
	  
				// grep operand 1 out of the outputstream
	  i= FindNextSpace( outputStream2,0,6 ); // find 6th space
	  j= FindNextSpace( outputStream2,i+1,blocks );
	  op1= outputStream2.substr( i+1,j-i );
	  
				// grep operand 2 out of the outputstream
	  k= FindNextSpace( outputStream2, j+1, blocks );
	  op2= outputStream2.substr( j+1,k-j );
	  
				// switch operand1 and operand2
	  outputStream2.replace( i+1,op2.length( ),op2 );
	  outputStream2.replace( i+1+op2.length( ),op1.length( ),op1 );
	  
	  break;
	}
    }
  if ( check == 0 && !skipVector )  
    {
      UCBlogfile << stream.str().c_str();
      if (  opcode=='+' || opcode=='*' )  
	{
	  if (op1!=op2)  
	    {
	      // generate line with operands switched to test commutivity
	      UCBlogfile << outputStream2.c_str();
	    }
	}
     UCBlogfile << flush;
    } else 
      {
	if (!skipVector)  
	  {
	    int tempLines=( int ) lines_in+readings+1;
	    
	    ucb.ReadLine(stream.str().c_str(),signedZero,noFlags,&tempLines );
	    ucb.DoLine( tiny,inf,NaN );
        ucb.Compare(std::cout);
	    
	    if (  opcode=='+' || opcode=='*' )  
	      {
		if (op1!=op2)  
		  {
		    // generate line with operands switched to test commutivity
		    ucb.ReadLine(outputStream2.c_str(), signedZero, noFlags, &tempLines);
		    ucb.DoLine( tiny,inf,NaN);
            //ucb.Compare(UCBlogfile);
            ucb.Compare(std::cout);
		  }
	      }
	  }
      }
}


/*-----------------------------------------------------------------------------
name        :putDot()
description :Slam a dot out to show some life 
called from :readAline()
call fns    :/
exceptions  :/
algorithm   :/
Global var. used :/
-----------------------------------------------------------------------------*/
void putDot( )
{
  cout << '.' << flush;
  ++dots;
  if ( dots > 78 ) 
    {
      cout << endl;
      dots = 0;
    }
}


/*-----------------------------------------------------------------------------
name        :FindNextSpace()
description :to find location to stream with proper spaces 
called from :readAline()
call fns    :/
exceptions  :/
algorithm   :
Global var. used :/
-----------------------------------------------------------------------------*/
int FindNextSpace(const string & pstring, int beginpos, int occurance )
{

  unsigned i=beginpos;
  while (  i<pstring.length( )  )  
    {
      if (  pstring[ i ] ==' ' )  
	{
	  occurance--;
	  if (  occurance==0 )
	    return i;
	}
      i++;
    }
  return i;
}


/*-----------------------------------------------------------------------------
name        :skipSpace()
description :avoid spaces while reading a vector
called from :readAline()
call fns    :mygetc(),myungetc()
exceptions  :/
algorithm   :/
Global var. used :/
-----------------------------------------------------------------------------*/
void skipSpace( )
{
  char c;
  while ( isspace( c = mygetc( input ) ) ) {
    if ( c=='\n' )
      lines_in++;
    ;
  }
  myungetc( c, input );
}


/*-----------------------------------------------------------------------------
name        :getModes()
description :get rounding modes from the test vector
called from :readAline()
call fns    :mygetc(),myungetc()
exceptions  :/
algorithm   :
Global var. used :round,testmodes,flushVector
-----------------------------------------------------------------------------*/
void getModes( )
{
  char c;
  testModes = flushVector = 0;
  while ( !isspace( c = mygetc( input ) ) )
    switch ( c )  {
      case 'A': // all rounding modes
        c = mygetc( input ); /* skip 'LL' */
      case 'U':
        if ( Round & ROUND_ZERO )
          testModes |= ( 1 << TD_TOWARDZERO );
        if ( Round & ROUND_NEAREST )
          testModes |= ( 1 << TD_TONEAREST );
        if ( Round & ROUND_DOWN )
          testModes |= ( 1 << TD_DOWNWARD );
        if ( Round & ROUND_UP )
          testModes |= ( 1 << TD_UPWARD );
        c = mygetc( input );
        break;
      case '0':
        if ( Round & ROUND_ZERO )
          testModes |= ( 1 << TD_TOWARDZERO );
        break;
      case '=':
        if ( Round & ROUND_NEAREST )
          testModes |= ( 1 << TD_TONEAREST );
        break;
      case '<':
        if ( Round & ROUND_DOWN )
          testModes |= ( 1 << TD_DOWNWARD );
        break;
      case '>':
        if ( Round & ROUND_UP )
          testModes |= ( 1 << TD_UPWARD );
        break;
      case 'o':
        testFormats = TESTODD;
        break;
      case 'e':
        testFormats = TESTEVEN;
        break;
      default:
        cout << "\n\nError on line " << lines_in+readings+1 
             << "of inputfile.\nERROR: unrecognizable mode character: " << c
             << "\n\n\nDone with " << readings << " readings.";
        MyExit(ERR_PARSE_TESTVECTOR);
        break;
    }
  myungetc( c, input );
  if ( testFormats == 0 )
    testFormats = ALL_FORMATS;
}


/*-----------------------------------------------------------------------------
name        :initialize_values()
description :assigns values to global variables for precision parsed 
		from the command line.
called from :readAline()
call fns    :/
exceptions  :/
algorithm   :
Global var. used :expon,hidden,mant,length,rest,sign_exp_length,
		kl_biased_exp,exp_rest.
-----------------------------------------------------------------------------*/
void initialize_values( int source)
{	
  if (source ==1)
    {
      expon = sexpon;
      hidden = shidden;
      mant = smant;
      length = slength;
      rest = srest;
      sign_exp_length = ssign_exp_length;
      kl_biased_exp = skl_biased_exp;
      sign_exp_rest = ssign_exp_rest ;
    }
  
  else 				//destnation
    {
      expon = dexpon;
      hidden = dhidden;
      mant = dmant;
      length = dlength;
      rest = drest;
      sign_exp_length = dsign_exp_length;
      kl_biased_exp = dkl_biased_exp;
      sign_exp_rest = dsign_exp_rest ;
    }
}


/*-----------------------------------------------------------------------------
name        :getExceptions()
description :gets exceptions raised for the test vector
called from :readAline()
call fns    :
exceptions  :/
algorithm   :
Global var. used :skipVector,jumpOverflow,jumpUnderflow,jumpInvalid,jumpDivZero
-----------------------------------------------------------------------------*/
void getExceptions( )
{
  char c;

  xcp = 0;
  while ( !isspace( c = mygetc( input ) ) ) {
    switch ( c ) {
      case 'x':
        xcp |= ( long32 )  TD_INEXACT;
        break;
        /*
         * In the Metro case, checking tininess before rounding to detect
         * underflow means that all possible cases of underflow apply.
         * (By contrast, checking AFTER rounding lets some boundary cases
         * slip by, and checking for EXTRAORDINARY error due to subnormalization
         * lets some tiny cases through that happen to round the same in
         * spite of subnormalization.
         */
        
      case 'u':
        xcp |= ( long32 )  TD_UNDERFLOW;
        if (jumpUnderflow) 
            skipVector=1;
        break;
      case 'v':
        xcp |= ( long32 )  TD_UNDERFLOWV; 
        if (jumpUnderflow) 
            skipVector=1;
        break;
      case 'w':
        xcp |= ( long32 )  TD_UNDERFLOWW; 
        if (jumpUnderflow) 
            skipVector=1;
        break;
      case 'o':
        xcp |= ( long32 )  TD_OVERFLOW; 
        if (jumpOverflow) 
            skipVector=1;
        break;
      case 'i':
        xcp |= ( long32 )  TD_INVALID; 
        if (jumpInvalid) 
            skipVector=1;
        break;
      case 'z':
        xcp |= ( long32 )  TD_DIVBYZERO; 
        if (jumpDivZero) 
            skipVector=1;
        break;
      case 'O':
        c = mygetc( input );
        /* skip the 'OK' */
        break;
      default:
        cout << "\n\nError on line " << lines_in+readings+1 
             << "of inputfile.\nERROR: unrecognizable exception character: " << c
             << "\n\n\nDone with " << readings << " readings.";
        MyExit(ERR_PARSE_TESTVECTOR);
        break;
    }
  }
  myungetc( c, input );
}


/*-----------------------------------------------------------------------------
name        :getbinary()
description :reads the binary coded number form the test set //ONLY FOR CONVERSIONS
called from :readAline()
call fns    :getSign()
exceptions  :/
algorithm   :
Global var. used :length,rest,sign_exp_length,exp_rest,pre,binary,upbinary,nextdig
-----------------------------------------------------------------------------*/
void getbinary( ulong32 *number)
{
  ulong32 i= 0;	//difference with Papadopoulo-release:unsigned
  NaNresult = 0; 
  
  for ( i=0; i<length;i++ )  // init number
    number[ i ] =0;
  
  mysignbit=getSign();
  
  getBsignificant(number);
 
  getBbias(number);
  
/* Stuff the sign nonarithmetically, to protect signaling NaNs. */
  if ( mysignbit )
    number[ 0 ]  |= 0x80000000L;
}


/*-----------------------------------------------------------------------------
  name        :getBsignificant()
  description :get significant of the binary number 
  called from :getbinary()
  calls fns   :
  exceptions  :/
  algorithm   :
Global var. used :/
  -----------------------------------------------------------------------------*/
void getBsignificant(ulong32 *number)
{
  int i,j,k=0,val=0;
  char c=mygetc(input);
  static char binary[maxstr];
  
  i = 0;
  while ( !isspace( c )  && ( c != '_' ) )  
    {
      binary[ i++ ]  = c;
      c = mygetc( input );
    }
  nextdig = 0; // exact
  if ( c == '_' )  
    {
      if ( mygetc( input )  == '1' ) 
	{
	  nextdig = 2;
	  if ( mygetc( input )  == '1' )
	    nextdig++;
	} else
	  nextdig = 1;
    }
  while ( !isspace( c ) )  c = mygetc( input ); // skip tail
  skipSpace( );
  
  // copy binary per 4 bits for each hex input character
  i--; //  binary length (times 4 bits)
  j = length - 1;
  
  while ( i - 7 >= 0 )  
    {
      if ( isdigit( binary[ i-7 ] ) )
	val = binary[ i-7 ]  - '0';
      else
	val = 0xa + ( binary[ i-7 ]  - 'a' );
      number[ j ]  += val;
      for ( k = 6;k >= 0;k-- )  
	{
	  number[ j ]  *= 16;
	  if ( isdigit( binary[ i-k ] ) )
	    val = binary[ i-k ]  - '0';
	  else
	    val = 0xa + ( binary[ i-k ]  - 'a' );
	  number[ j ]  += val;
	}
      
      i -= 8;
      j--;
    }
  if ( i >= 0 )  
    {
      for ( k = 0;k < i;k++ )  
	{
	  if ( isdigit( binary[ k ] ) )
	    val = binary[ k ]  - '0';
	  else
	    val = 0xa + ( binary[ k ]  - 'a' );
	  number[ j ]  += val;
	  number[ j ]  *= 16;
	}
      
      if ( isdigit( binary[ k ] ) )
	val = binary[ k ]  - '0';
      else
	val = 0xa + ( binary[ k ]  - 'a' );
      number[ j ]  += val;
    }
  if ( rest != 0 )  
    {
      unsigned i = sign_exp_length - 1;
      for ( ; i<length-1;i++ )
	for (unsigned j = 0;j < NO_OF_BITS - rest;j++ )  
	  {
	    number[ i ]  *= 2;
	    if ( number[ i+1 ]  & ( 1L << ( (NO_OF_BITS-1) - j ) ) )
	      number[ i ] ++;
	  }
      for (unsigned j = 0;j < NO_OF_BITS - rest;j++ )
	number[ i ]  *= 2;
    }
  
  if ( ( hidden )  && ( number[ 0 ]  != 0 ) )  // remove leading bit, if not zero!
    {
      if ( sign_exp_rest==(NO_OF_BITS-1) )
	number[ sign_exp_length ]  -= ( 1L<<(NO_OF_BITS-1) );
      else
	number[ sign_exp_length-1 ]  -= ( 1L<<( NO_OF_BITS-sign_exp_rest-1 ) );
    }
}


/*-----------------------------------------------------------------------------
  name        :getBbias()
  description :gets bias for the binary number 
  called from :getbinary()
  calls fns   :getBexpon()
  exceptions  :/
  algorithm   :
Global var. used :/
  -----------------------------------------------------------------------------*/
void getBbias(ulong32 *number)
{
  ulong32 i,k;	//different from Papadopoulo-release:i declared when needed just unisgned, and k just an int
  long32 u=0;
  
  u=getBexpon();
  
  ulong32 * bias = new ulong32[ sign_exp_length ];
  if ( bias == NULL )
   {
     cout << "\nNot enough memory\nProgram aborted.\n\n\nError on line " 
          << lines_in+readings+1 << " of inputfile.\n\nDone with " 
          << readings << " readings." << endl;
     MyExit(ERR_MEM);
   }
  for ( i=1; i<sign_exp_length;i++ )
    bias[ i ] =0xFFFFFFFFL;
  bias[ 0 ] =0x3FFFFFFFL;
  if ( sign_exp_length > 1 )
    bias[ sign_exp_length-1 ]  <<= ( NO_OF_BITS-sign_exp_rest-1 );
  else
    bias[ 0 ] =( ulong32 ) ( ((ulong32)1 << (expon-1)) -1 )  << ( NO_OF_BITS-sign_exp_rest-1 );
  // add exp to bias
  while ( u > 0 )  
    { // positive exp
      bias[ sign_exp_length-1 ]  += ( 1L << ( NO_OF_BITS-sign_exp_rest-1 ) );
      k=sign_exp_length-1;
      while ( bias[ k ] ==0 ) 
	{
	  k--;
	  bias[ k ]  += 1;
	}
      k=sign_exp_length-1;
      u--;
    }
  while ( u < 0 )  
    { // negative exp
      k=sign_exp_length-1;
      while ( bias[ k ] ==0 ) 
	{
	  k--;
	  bias[ k ]  -= 1;
	}
      bias[ sign_exp_length-1 ]  -= ( 1L << ( NO_OF_BITS-sign_exp_rest-1 ) );
      u++;
    }
  
  // copy bias into number
  for ( i=0;i<sign_exp_length-1;i++ )
    number[ i ]  = bias[ i ];
  /*
    if (kl_biased_exp > 0) // change kl_biased_exp
    number[sign_exp_length-1] &= kl_biased_exp-1;
  */
  number[ sign_exp_length-1 ]  |= bias[ sign_exp_length-1 ];
  delete[ ]  bias;

}


/*-----------------------------------------------------------------------------
  name        :getBexpon
  description :gets exponent for the binary number
  called from :getBbias()
  calls fns   :
  exceptions  :/
  algorithm   :
  Global var. used :/
  -----------------------------------------------------------------------------*/
long32 getBexpon()
{
  char c;
  ulong32 signexp = 0;
  long32 u=0;
  c=mygetc( input ); // skip 'E' character
  c=mygetc( input );
  if ( c == '-' ) 
    {
      signexp = 1;
      c = mygetc( input );
    } else if ( c == '+' ) 
      {
	c = mygetc( input );
      }
  
  while ( isdigit( c ) )  
    {
      u = ( u * 10 )  + ( c - '0' );
      c = mygetc( input );
    } // while
  myungetc( c, input ); // for EOL
  if ( signexp )
    u = -u;
  return u;
}


/*-----------------------------------------------------------------------------
name        :getdecimal()
description :get decimal coded number form testset  //  ONLY FOR CONVERSION
called from :readAline()
call fns    :getDsignificand(),getDexpon
exceptions  :/
algorithm   :
Global var. used :decimal,updecimal,nextdig
-----------------------------------------------------------------------------*/
void getdecimal( )
{
  getDsignificand(); 		//i is the position till which 
				//significant has occupied decimal[]
  getDexpon();
}


/*-----------------------------------------------------------------------------
name        :getDsignificand()
description :get significand of decimal coded number
called from :getdecimal()
call fns    :mygetc(),myungetc()
exceptions  :/
algorithm   :
Global var. used :decimal,updecimal,nextdig,incexp
-----------------------------------------------------------------------------*/
void getDsignificand()
{					// get significand
  posDec = 0,posUpDec = 0;
  incexp = 0;
  char c;
  c = mygetc( input );
  
  while ( !isspace( c )  && ( c != '_' ) )  
    {
      decimal[ posDec ]  = c;
      updecimal[ posDec++ ]  = c;
      c = mygetc( input );
    }
  if ( c == '_' )  	 	//find next number for case of 9's in end 
    {
      posUpDec = posDec-1;			// or all 9's .
      while ( ( posUpDec > 1 )   && ( updecimal[ posUpDec ]  == '9' ) )
	updecimal[ posUpDec-- ]  = '0';
      if ( updecimal[ posUpDec ]  == '9' )  
	{
	  updecimal[ 1 ]  = '1';
	  for ( posUpDec = 2; posUpDec <= posDec; posUpDec++ )
	    updecimal[ posUpDec ]  = '0';
	  incexp = 1;
	} else
	  updecimal[ posUpDec ]  = ( int )  updecimal[ posUpDec ]  + 1;
      nextdig = mygetc( input )  - '0';
      c = mygetc( input );
      while ( !isspace( c ) )  c = mygetc( input ); // skip tail
    }  else 
      {
	nextdig = 0;
      }	
}


/*-----------------------------------------------------------------------------
name        :getDexpon()
description :get exponent of decimal coded number
called from :getdecimal()
call fns    :mygetc(),myungetc(),exceptions  :/
algorithm   :
Global var. used :decimal,updecimal,nextdig,incexp
-----------------------------------------------------------------------------*/
void getDexpon()
{
  char signexp,c;
  c = mygetc( input );

  if ( c == 'E' )   			// get exponent
    {
      decimal[ posDec ]  = c;
      updecimal[ posDec++ ]  = c;
      c = mygetc( input );
      signexp = c;
      while ( !isspace( c ) )  
	{
	  decimal[ posDec ]  = c;
	  updecimal[ posDec++ ]  = c;
	  c = mygetc( input );
	}
      decimal[ posDec ]  = '\0';
      posUpDec = posDec;

      if ( incexp )  
	{			
	  if ( signexp != '-' )   		// inc exponent
	    {
	      posDec--;
	      while ( updecimal[ posDec ]  == '9' )  
		{
		  updecimal[ posDec ]  = '0';
		  posDec--;
		}
	      if ( ( updecimal[ posDec ]  == 'E' ) || ( updecimal[ posDec ]  == '+' ) )
		{
		  updecimal[ posDec+1 ]  = '1';
		  updecimal[ posUpDec++ ]  = '0';
		} else 
		  {
		    updecimal[ posDec ]  = ( int )  updecimal[ posDec ]  + 1;
		  }
	    } else 				 // dec exponent
	      {
		posDec--;
		while ( updecimal[ posDec ]  == '0' ) 
		  {
		    updecimal[ posDec ]  = '9';
		    posDec--;
		  }
		updecimal[ posDec ]  = ( int )  updecimal[ posDec ]  - 1;

		// Convert E-0xyz into E-xyz
		if ((updecimal[posDec - 1] == '-') && (updecimal[posDec]
== '0') && (updecimal[posDec + 1] != '\0'))
		  {
		    while(updecimal[posDec+1] != '\0')
                    {
                      updecimal[posDec] = updecimal[posDec+1];
                      posDec++;
                    }
		    posUpDec = posDec;
		  }
              }
	} // if
    }else 
      {
	myungetc( c,input );
      }
  updecimal[ posUpDec ]  = '\0';
  myungetc( c, input );
}


/*-----------------------------------------------------------------------------
name        :getNumber
description :takes the coded number and precsion to get the number 
called from :readAline()
call fns    :getSign(), getFPRootNumber(),getModifier()
exceptions  :/
algorithm   :
Global var. used :HexNoIsGiven,cant_infinity,small_norm,NaNresult
-----------------------------------------------------------------------------*/
void getNumber( ulong32 *number)
{	
 
  ulong32 i;             
  int signbit=0;
  HexNoIsGiven=0; 		//initialize global variable
  cant_infinity=0, small_norm=0;	//initialize global variable
  NaNresult = 0; 			// change DV
  
  for ( i=0; i<length;i++ )               // number initialized to 0
    number[ i ] =0;
  
  signbit=getSign();
  
  getFPRootNumber(number);
  
  if (HexNoIsGiven) return ;	
  /* if hexadecimal number is given as operand then save 
     the number to the global string intstr and exit this function */
  
  getModifier(number);
  
  /* Stuff the sign nonarithmetically, to protect signaling NaNs. */
  if (signbit)		   // checksign() is to get the sign of sign of number
    number[ 0 ]  |= 0x80000000L;
}


/*-----------------------------------------------------------------------------
name        :WriteNumber()
description :Inserts correct number of zeros at most significant position.
called from :readAline()
call fns    :/
Global var. used :length,NaNresult
-----------------------------------------------------------------------------*/
void WriteNumber(ostream & stream,  const ulong32 *number, ulong32 expon,
                    ulong32 hidden, ulong32 mant, ulong32 length, const int err)
{
  ulong32 i;

  if ( !err ) {
    for ( i=0; i<length;i++ )
      // stream << " "<< setw(8) << setfill('0') << hex << number[i];
      if ( number[ i ]  >= 0x10000000 ) {
        stream << " " << hex << number[ i ];
      }
      else if ( number[ i ]  >= 0x01000000 )  {
        stream << " 0" << hex << number[ i ];
      } else if ( number[ i ]  >= 0x00100000 )  {
        stream << " 00" << hex << number[ i ];
      } else if ( number[ i ]  >= 0x00010000 )  {
        stream << " 000" << hex << number[ i ];
      } else if ( number[ i ]  >= 0x00001000 )  {
        stream << " 0000" << hex << number[ i ];
      } else if ( number[ i ]  >= 0x00000100 )  {
        stream << " 00000" << hex << number[ i ];
      } else if ( number[ i ]  >= 0x00000010 )  {
        stream << " 000000" << hex << number[ i ];
      } else {
        stream << " 0000000" << hex << number[ i ];
      }
  } else {
    stream << " Wrong inputnumber";
    err_form=1;
  }
  if ( NaNresult ) {
    stream << '?';
  }
}


/*-----------------------------------------------------------------------------
name        :getSign()
description :checks if the sign of a number is + or negative ... by default it returns + .  
called from :getNumber()
call fns    :/                                //mygetc()
exceptions  :/
algorithm   :
Global var. used :pre
-----------------------------------------------------------------------------*/ 
int getSign(){
  char c; 
  int signBit=0;		//default takes positive
  c = mygetc( input );
  switch(c){
  case '-' : 
    signBit=1;
    break;
  case '+' : 
    signBit=0;
    break;
  case '?' :
    pre=1;
    break;
  default : 
    myungetc(c,input);
    break;
  }
  return signBit;
}


/*-----------------------------------------------------------------------------
name        :getFPRootNumber
description :extracts the rootnumber from the coded number  
called from :getNumber()
calls fns   :Infinity(),Quiet_NaN(),Signaling_NaN(),smallest_Norm(),getFPHexNo(),getFPInteger()
exceptions  :/
algorithm   :
Global var. used :cant_infinity,killerVector,small_norm,NaNresult,HexNoIsGiven
-----------------------------------------------------------------------------*/  
void getFPRootNumber(  ulong32 *number)
{
  char c,d;
  c = mygetc( input );
  switch (c)
    {
    case 'H':
      Infinity( number);
      cant_infinity=1;
      break;
    case 'Q':
      Quiet_NaN( number);
      killerVector = 1;   /* change BV */
      NaNresult = 1; // change DV
      break;
    case 'S':
      Signaling_NaN( number);
      killerVector = 1;  /* change BV */
      NaNresult = 1;
      break;
    case 'E':              // this 'E' is given for compatibility with Coonen vectors 
    case 'T':
      Smallest_Norm( number);
      small_norm=1;       // indien er een number volgt op T, dan kan via deze
      // variabele gezien worden of er een plus (voor T)
      // of een minus (voor H, zie aldaar) moet gebeuren
      break;
    case '0':
      d = mygetc( input );
      if ( d == 'x' ) 		//hexadecimal number
	{
	  getFPHexNo();		//save Hexadecimal no. in intstr[]
	  HexNoIsGiven=1;
	} 
      else 
	{
	  myungetc(d,input);
	  myungetc(c,input);
	  getFPInteger( number);
	}
      break;
    default:
      if ( isdigit( c ) )
	{
	  myungetc(c,input);
	  getFPInteger( number);
	} else 
	  {
        cout << "\n\nError on line " << lines_in+readings+1 
             << "of inputfile.\ngetNumber() ERROR: " << c 
             << "is an unknown number specifier in getNumber\n\n\nDone with "
             << readings << " readings.";
        MyExit(ERR_PARSE_TESTVECTOR);
	  }
    }
} 


/*-----------------------------------------------------------------------------
  name        :getModifier
  description :Modifies the rootnumber 
  called from :getNumber()
  calls fns   :ModifierPorM(),ModifierIorD(),ModifierU()
  exceptions  :/
  algorithm   :
Global var. used :/
  -----------------------------------------------------------------------------*/
void getModifier(ulong32 *number)
{
  ulong32 k; 
  int McasePorM,McaseIorD;
  char c;

  while ( !isspace( c = mygetc( input ) ) )  
    {
      switch ( c )  {
      case 'p':	
	McasePorM=0;
	ModifierPorM(number,McasePorM);
	break;
      case 'm' :
	McasePorM=1;
	ModifierPorM(number,McasePorM);
	break;
      case 'i' :
	McaseIorD=0;
	ModifierIorD(number,McaseIorD);
	break;
      case 'd' :
	McaseIorD=1;
	ModifierIorD(number,McaseIorD);
	break;
      case 'u' :
	k = getinteger( );
	ModifierU( number,k);
	break;

	
      case '0':
      case '1':
      case '2':
      case '3':             // indien er hier een cijfer wordt ingelezen, dan
      case '4':             // kan dat enkel voorafgegaan zijn door een
      case '5':             // H (infinity) of een T (smallest normalized)
      case '6':             // if here a figure is read in,

			    // then that
			    // can only be preceded by H(infinty ) 
			    // or T (smallest normalised)  
      case '7':
	    case '8':
      case '9':
      case 't':
      case 'B':
      case 'C':
	if ( ( c != 't' )  && ( c != 'u' )  && ( c!='B' )  && ( c != 'C' ) )
	  k=( long32 ) atoi( &c );
	else if ( c == 't' )
	  k = mant + hidden;
	else if ( c == 'B' )  
	  {
	    // k = (ulong32) (((ulong32)1 << (expon-1)) - 1);
		  k = (ulong32) ULONG_MAX;
		  cout << "ULONG_MAX :" << ULONG_MAX << endl;
	  } else if ( c == 'u' )
	    k = dmant + dhidden;
	else if ( c == 'C' )
	  // k = (ulong32) (((ulong32)1 << (dexpon-1)) - 1);
	  k = (ulong32) ULONG_MAX;
	if ( cant_infinity )          // if H
	Minus(number,k);
		//	  PlusBk( number,k);
	if ( small_norm )        // if T
	  Plus( number,k);
	break;
	

      default:
        cout << "\n\nError on line " << lines_in+readings+1 
             << "of inputfile.\ngetNumber() ERROR: unrecognizable modifier character: " << c 
             << "\n\n\nDone with " << readings << " readings.";
        MyExit(ERR_PARSE_TESTVECTOR);
      }
    }
  myungetc( c, input );
}


/*-----------------------------------------------------------------------------
name        :getFPHexNo()
description :this is called if the coded number is hexadecimal . This stores hexadecimal number in intstr[] and quits getNumber(). 
called from :getFPRootNumber()
calls fns   :/
exceptions  :/
algorithm   :
Global var. used : intstr[]  
-----------------------------------------------------------------------------*/
void getFPHexNo(){
  long32 i;
  char c, capc ;
  intstr+="0x";
  i = 2;
  for ( capc = toupper( c = mygetc( input ) );
	( '0' <= capc && capc <= '9' )  || ( 'A' <= capc && capc <= 'F' );
	capc = toupper( c = mygetc( input ) ) )  
    {
      intstr+=c;
      i++;
    }
 myungetc(c,input);
}


/*-----------------------------------------------------------------------------
  name        :ModifierIorD
  description :Modifies the rootnumber for case 'i'-increment or case 'd'-decrement 
  called from :getModifier()
  calls fns   :Incr_at_pos(), Incr(),Decr_at_pos(),Decr(),getInteger()
  exceptions  :/
  algorithm   :
  Global var. used :/
  -----------------------------------------------------------------------------*/
void ModifierIorD(ulong32 *number,int McaseIorD) {
  char c;
  
  ulong32 pos,k;
  c = mygetc( input );
  switch ( c )  {
    
  case '(':
    pos = getinteger( );
    c = mygetc( input );
    switch ( c )  {
      
    case '+':
      pos += getinteger( );
      c = mygetc( input );
      if ( c == ')' )  
	{
	  k = getinteger( );
	  if (McaseIorD)
	    Decr_at_pos( number,pos,k);
	  else
	    Incr_at_pos( number,pos,k);
	} else 
	  {
        cout << "\n\nError on line " << lines_in+readings+1 
             << "of inputfile.\ngetNumber() ERROR: unexpected character: " << c 
             << "\n\n\nDone with " << readings << " readings.";
        MyExit(ERR_PARSE_TESTVECTOR);
		      } // end if
		  break;
		  
    case '-':
      pos -= getinteger( );
      c = mygetc( input );
      if ( c == ')' )  
	{
	  k = getinteger( );
	  if (McaseIorD)
	    Decr_at_pos( number,pos,k);
	  else
	    Incr_at_pos( number,pos,k);
	} else 
	  {
        cout << "\n\nError on line " << lines_in+readings+1 
             << "of inputfile.\ngetNumber() ERROR: unexpected character: " << c 
             << "\n\n\nDone with " << readings << " readings.";
        MyExit(ERR_PARSE_TESTVECTOR);
	  } // end if
      break;
      
    case ')':
      k = getinteger( );
      if (McaseIorD)
	Decr_at_pos( number,pos,k);
      else
	Incr_at_pos( number,pos,k);
      break;
		  
    default:
        cout << "\n\nError on line " << lines_in+readings+1 
             << "of inputfile.\ngetNumber() ERROR: unexpected character: " << c 
             << "\n\n\nDone with " << readings << " readings.";
        MyExit(ERR_PARSE_TESTVECTOR);
    } // end if
    break;
    
  default:
    myungetc( c, input );
    k = getinteger( );
    if (McaseIorD)
      Decr( number,k);
    else
      Incr( number,k);
    break;
  } // switch case 'i'
  
}


/*-----------------------------------------------------------------------------
  name        :ModifierPorM
  description :Modifies the rootnumber for case 'p'-plus or case 'm'-minus 
  called from :getModifier()
  calls fns   :Plus(),PLusBk(),Minus(),MinusBk()
  exceptions  :/
  algorithm   :
  Global var. used :/
  -----------------------------------------------------------------------------*/
void ModifierPorM(ulong32 *number,int McasePorM)
{
char c,ch;
ulong32 k=0;

c = mygetc( input );
  if (c== 'B' || c== 'C' ) 		//Bias (C is only for conversions)
    {
      ch=c;
      c= mygetc(input);
      if ((c - '0')>0 && (c - '0')<10)
	{
	  k=c-'0' ;
	  if (McasePorM) 
	    MinusBk( number,k);
	  else 
	    PlusBk( number,k);
	  return;
	} else  if ((c - '0') != 0 ) 	//makes B0 as B
	  myungetc(c,input) ;
	c=ch;
    }
  myungetc( c,input );
  k = getinteger( );
  if (McasePorM) 
    Minus(number,k);
  else 
    Plus(number,k);
}


/*-----------------------------------------------------------------------------
name        :ModifierU()
description :operates u<digit> 
called from :getModifier()
call fns    :null_exp(),one_exp()
exceptions  :/
algorithm   :
Global var. used :sign_exp_length,exp_rest,length,rest,hidden,kl_biased_exp,mant
-----------------------------------------------------------------------------*/
void ModifierU( ulong32 *number, ulong32 ulps)
{
  ulong32 *e, i, bits_ulps, k, shift_part;
  
  e = new ulong32[ sign_exp_length ];
  
  if (  e == NULL ) {
    cout << "\nNot enough memory in procedure ModifierU!!!\nProgram aborted.\n"
         << "\n\nError on line " << lines_in+readings+1 << " of inputfile."
         << "\n\nDone with " << readings << " readings." << endl;
    MyExit(ERR_MEM);
  }
  
  for ( i=0;i<sign_exp_length-1;i++ )
    e[ i ] =number[ i ];
  e[ sign_exp_length-1 ]  = ( number[ sign_exp_length-1 ]  >> ( (NO_OF_BITS-1)-sign_exp_rest ) );
  e[ sign_exp_length-1 ]  <<= ( (NO_OF_BITS-1)-sign_exp_rest );
  
    
  if (  null_exp( e)  ||
        one_exp( e)  )  
    {
      for ( i=0;i<length;i++ )
	number[ i ] =0;
      if ( ulps != 0 ) 
	{
	  bits_ulps = ( ulong32 )  floor( ::log10( ulps ) / ::log10( 2 ) ) +1;
	  if  ( bits_ulps > mant ) 
	    {
	      ulps=0;
	      wrong_input=1;
	    }
	}
      number[ length-1 ] =ulps;
      
      if ( rest!=0 ) 
	{
	  if ( ( ( bits_ulps > rest )  && hidden )  || ( ( bits_ulps+1 > rest )  && ( !hidden ) ) )
	    number[ length-2 ]  = ( ulps >> rest );
	  number[ length-1 ]  <<= ( NO_OF_BITS-rest );
	}
    }
  else 
    {   
      
      for ( i=0;i<length;i++ )
	number[ i ] =0;
      if ( ulps != 0 ) 
	{
	  bits_ulps = ( ulong32 )  floor( ::log10( ulps ) / ::log10( 2 ) ) +1;
	if  ( bits_ulps > mant ) 
	  {
	    ulps=0;
	    wrong_input=1;
	  }
	}
      number[ length-1 ] =ulps;
      
      if ( rest!=0 ) 
	{
	  if ( ( ( bits_ulps > rest )  && hidden )  || ( ( bits_ulps+1 > rest )  && ( !hidden ) ) )
	    number[ length-2 ]  = ( ulps >> rest );
	  number[ length-1 ]  <<= ( NO_OF_BITS-rest );
	}
      
      k=sign_exp_length-1;
      shift_part=length-1;
      
      while ( ( hidden && !( number[ sign_exp_length-1 ]  & kl_biased_exp ) )  ||
	      ( !hidden &&  (  ( ( !( number[ sign_exp_length ]  & ( 1L<<(NO_OF_BITS-1) ) ) )  &&
				 ( sign_exp_rest == (NO_OF_BITS-1) ) )  ||
			       ( !( number[ sign_exp_length-1 ]  & ( kl_biased_exp>>1 ) )  &&
				 ( sign_exp_rest != (NO_OF_BITS-1) ) )  ) )
	      )  {
	e[ sign_exp_length-1 ]  -= ( 1L << ( (NO_OF_BITS-1)-sign_exp_rest ) );
	k=sign_exp_length-1;
	while ( k==sign_exp_length-1 ? e[ k ] ==( 0xFFFFFFFFUL << ( NO_OF_BITS-sign_exp_rest-1 ) )  : e[ k ] ==0xFFFFFFFFUL )  
	  {
	    k--;
	    e[ k ]  -= 1;
	  }
	// shift left
	if ( shift_part!=0 )
	  number[ shift_part-1 ]  = ( number[ shift_part-1 ]  << 1 )  | ( ( number[ shift_part ]  >> (NO_OF_BITS-1) )  & 0x01L );
	number[ shift_part ]  <<= 1;
	if ( number[ shift_part ] ==0 )
	  shift_part--;
	i++;
      } // while
      
      for ( i=0; i<sign_exp_length-1; i++ )
	number[ i ]  = e[ i ];
      number[ i ]  &= kl_biased_exp-1;
      number[ i ]  |= e[ sign_exp_length-1 ];
    }
  
  delete[ ]  e;
}


/*-----------------------------------------------------------------------------
name        :getinteger()
description :gets integer value from the string (also checks for hex no.)
called from :Modifier(),ModifierIorD(),ModifierPorM()
call fns    :mygetc(),myungetc()
exceptions  :/
algorithm   :
Global var. used :
-----------------------------------------------------------------------------*/
/* Peruse the input stream for an unsigned integer.
 * Allow the 0xddddd syntax of C integers, and also decimals.
 */
ulong32 getinteger()
{
  char c;
  ulong32 u = 0;
  
  u=getHex();			//returns integer value if Hex 
  if (u != 0) return u;		// hex no. is given

	  //Now the input character c is either a decimal
	  //digit or a terminal.  A null number returns as zero.
	   
  c=mygetc(input);
  switch(c){
  case 't':
    u = smant + shidden;
    break;
    
    
  case 'B' :
    u = ( ulong32 )  ( ((ulong32)1 << (sexpon-1))  - 1 );
    break;
    
  case 'u' :
    u = dmant + dhidden;
    break;
    
    
  case 'C' :
    u = ( ulong32 )  ( ((ulong32)1 << (dexpon-1))  - 1 );
    // u = ULONG_MAX; 
    break;
    
  case 'h' :
    u = ( mant + hidden + 1 )  / 2 - 1;
    break;

  default:
    while ( isdigit( c ) ) {
      u = ( u * 10 )  + ( c - '0' );
      c = mygetc( input );
    }
    myungetc( c, input );
  }
    return u;
}


/*-----------------------------------------------------------------------------
name        :getHex()
description :gets integer value of Hex no.
called from :getinteger()
call fns    :mygetc(),myungetc()
Global var. used :
-----------------------------------------------------------------------------*/
ulong32 getHex()
{
char c,capc;
ulong32 u=0;
c = mygetc( input );
  if ( c == '0' ) {
    c = mygetc( input );
    if ( c == 'x' ) {
      for ( capc = toupper( c = mygetc( input ) );
            ( '0' <= capc && capc <= '9' )  || ( 'A' <= capc && capc <= 'F' ); capc = toupper( c = mygetc( input ) ) ) {
        u <<= 4;
        if ( isdigit( capc ) )
          u |= capc - '0';
        else
          u |= 0xa + ( capc - 'A' );
      }
      myungetc( c, input );
      return u;
    }
  }
myungetc(c,input);
return u;
  
}


/*-----------------------------------------------------------------------------
name        :getFPInteger()
description :get floating ponit integer in IEEE format
called from :getFPRootNumber()
call fns    :mygetc(),myungetc()
exceptions  :/
algorithm   :
Global var. used :wrong_input
-----------------------------------------------------------------------------*/
void getFPInteger( ulong32 *number)
{
  char c;
  ulong32 bits_u, ex, *bias, u=0;
  ulong32 k, i;	//different from Papadopoulo-release:k just long, and i declared when needed just unsigned
  
  wrong_input=0;
  c=mygetc( input );
  while ( isdigit( c ) )  
    {
      u = ( u * 10 )  + ( c - '0' );
      c = mygetc( input );
    } // while

  myungetc( c, input );
  
  if ( u != 0 ) 
    {  // determine no. of bits for u
      bits_u = ( ulong32 ) floor( ::log10( u ) / ::log10( 2 ) )  + 1;
      if ( bits_u> mant+hidden ) 
	{
	  u=0;
	  wrong_input=1;  	    // u greater than maximal value of significand
	}
      
    }
  u &= 0x00FFFFFFL;    				// limited to 24-bit integer
  if ( u != 0 ) 
    { 
      //determine no. of bits for u (now less than 24)
      bits_u = ( ulong32 ) floor( ::log10( u ) / ::log10( 2 ) )  + 1;
      
      if ( !hidden )
	u |= ( 1L << bits_u );  		 // put hidden bit in front
      
      number[ length-1 ] =u;
      
      if ( rest != 0 ) 
	{
	  if ( ( ( bits_u > rest )  && hidden )  || ( ( bits_u+1 > rest )  && ( !hidden ) ) )
	    number[ length-2 ] =( u >> rest );  	// if the 2 parts overlap
							// then the (bits_u-rest) bits that 
							//follow will be added to the previous part
	  number[ length-1 ]  <<= ( NO_OF_BITS-rest );
	}
      
      if ( hidden )
	ex=mant;
      else
	ex=mant-1;
      
      k=length-1;
      
      while ( ( number[ sign_exp_length-1 ]  & kl_biased_exp ) ==0 ) 
	{
	  if ( k!=0 )
	    number[ k-1 ]  = ( number[ k-1 ]  << 1 )  | ( ( number[ k ]  >> (NO_OF_BITS-1) )  & 0x01L );
	  
	  number[ k ]  <<= 1;
	  ex--;
	  
	  if ( number[ k ] ==0 )
	    k--;
	}
      
      bias = new ulong32[ sign_exp_length ];
      
      if ( bias == NULL ) 
	{
      cout << "\nNot enough memory in procedure getFPInteger!!!\nProgram aborted.\n"
           << "\n\nError on line " << lines_in+readings+1 << " of inputfile."
           << "\n\nDone with " << readings << " readings." << endl;
      MyExit(ERR_MEM);
	}
      
      for ( i=0; i<sign_exp_length;i++ )
	bias[ i ] =0;
      
      for ( i=1; i<sign_exp_length;i++ )
	bias[ i ] =0xFFFFFFFFL;
      bias[ 0 ] =0x3FFFFFFFL;
      
      if ( sign_exp_length > 1 )
	bias[ sign_exp_length-1 ]  <<= ( NO_OF_BITS-sign_exp_rest-1 );
      else
	bias[ 0 ] =( ulong32 ) ( ((ulong32)1 << (expon-1)) -1 )  << ( NO_OF_BITS-sign_exp_rest-1 );
      
				      //add rest of exponent to bias
      k=sign_exp_length-1;
      
      for ( i=0;i<ex;i++ ) 
	{
	  bias[ sign_exp_length-1 ]  += ( 1L << ( NO_OF_BITS-sign_exp_rest-1 ) );
	  while ( bias[ k ] ==0 ) 
	    {
	      k--;
	      bias[ k ]  += 1;
	    }
	  k=sign_exp_length-1;
	}
      
      // determine Exponent of number[] by assigning number[] =bias[]
      for ( i=0;i<sign_exp_length-1;i++ )
	number[ i ] =bias[ i ];
      
      if ( kl_biased_exp > 0 )  // change kl_biased_exp
	number[ sign_exp_length-1 ]  &= kl_biased_exp-1;
      
      number[ sign_exp_length-1 ]  |= bias[ sign_exp_length-1 ];
      
      delete[ ]  bias;
    }
}


/*-----------------------------------------------------------------------------
name        :Infinity()
description :Sets a number to infinty according to precision
called from :getFPRootNumber()
call fns    :
exceptions  :/
algorithm   :
Global var. used :sign_exp_length,exp_rest,hidden
-----------------------------------------------------------------------------*/
void Infinity( ulong32 *number)
{
  long32 j;

  if ( sign_exp_length==1 )
    number[ 0 ] =0x7FFFFFFFL<<( NO_OF_BITS-sign_exp_rest-1 ) ;
  else
    number[ 0 ] =0x7FFFFFFFL;

  if ( ( sign_exp_length==1 )  && ( sign_exp_rest!=(NO_OF_BITS-1) ) )
    number[ 0 ]  -= ( 1L << (NO_OF_BITS-1) );

  for ( j=sign_exp_length-2;j>0;j-- )
    number[ j ]  += 0xFFFFFFFFL;

  if ( sign_exp_length != 1 )
    number[ sign_exp_length-1 ]  = ( 0xFFFFFFFFL<<( NO_OF_BITS-sign_exp_rest-1 ) );

  if ( !hidden ) {
    if ( sign_exp_rest==(NO_OF_BITS-1) )
      number[ sign_exp_length ]  += ( 1L<<(NO_OF_BITS-1) );
    else
      number[ sign_exp_length-1 ]  += ( 1L<<( NO_OF_BITS-sign_exp_rest-2 ) );
  }
}


/*-----------------------------------------------------------------------------
name        :Quiet_NaN()
description :sets a number to Q 
called from :getFPRootNumber()
call fns    :/
exceptions  :/
algorithm   :
Global var. used :sign_exp_length,exp_rest,hidden
-----------------------------------------------------------------------------*/
void Quiet_NaN( ulong32 *number)
{
  long32 j;
  if ( sign_exp_length==1 )
    number[ 0 ] =0x7FFFFFFFL<<( NO_OF_BITS-sign_exp_rest-1 );
  else
    number[ 0 ] =0x7FFFFFFFL;

  if ( ( sign_exp_length==1 )  && ( sign_exp_rest!=(NO_OF_BITS-1) ) )
    number[ 0 ]  -= ( 1L << (NO_OF_BITS-1) );

  for ( j=sign_exp_length-2;j>0;j-- )
    number[ j ]  += 0xFFFFFFFFL;
  if ( sign_exp_length != 1 )
    number[ sign_exp_length-1 ]  = ( 0xFFFFFFFFL<<( NO_OF_BITS-sign_exp_rest-1 ) );

  if ( !hidden ) {
    if ( sign_exp_rest==(NO_OF_BITS-1) )
      number[ sign_exp_length ]  += ( 1L<<30 );
    else if ( sign_exp_rest==30 )
      number[ sign_exp_length ]  += ( 1L<<(NO_OF_BITS-1) );
    else
      number[ sign_exp_length-1 ]  += ( 1L<<( NO_OF_BITS-sign_exp_rest-3 ) );
  } else {
    if ( sign_exp_rest==(NO_OF_BITS-1) )
      number[ sign_exp_length ]  += ( 1L<<(NO_OF_BITS-1) );
    else
      number[ sign_exp_length-1 ]  += ( 1L<<( NO_OF_BITS-sign_exp_rest-2 ) );
  }
  if ( !hidden ) {
    if ( sign_exp_rest==(NO_OF_BITS-1) )
      number[ sign_exp_length ]  += ( 1L<<(NO_OF_BITS-1) );
    else
      number[ sign_exp_length-1 ]  += ( 1L<<( NO_OF_BITS-sign_exp_rest-2 ) );
  }
}


/*-----------------------------------------------------------------------------
name        :Signaling_NaN()
description :sets number to S
called from :getFPRootNumber()
call fns    :/
exceptions  :/
algorithm   :
Global var. used :sign_exp_length,exp_rest,hidden
-----------------------------------------------------------------------------*/
void Signaling_NaN( ulong32 *number)
{
  long32 j;
  if (  sign_exp_length==1 )
    number[ 0 ] =0x7FFFFFFFL<<( NO_OF_BITS-sign_exp_rest-1 );
  else
    number[ 0 ] =0x7FFFFFFFL;

  if ( ( sign_exp_length==1 )  && ( sign_exp_rest!=(NO_OF_BITS-1) ) )
    number[ 0 ]  -= ( 1L << (NO_OF_BITS-1) );
  for ( j=sign_exp_length-2;j>0;j-- )
    number[ j ]  += 0xFFFFFFFFL;
  if ( sign_exp_length != 1 )
    number[ sign_exp_length-1 ]  = ( 0xFFFFFFFFL<<( NO_OF_BITS-sign_exp_rest-1 ) );

  if ( rest )
    number[ length-1 ]  += ( 1L<<( NO_OF_BITS-rest ) );
  else
    number[ length-1 ]  += 1;

  if ( !hidden ) {
    if ( sign_exp_rest==(NO_OF_BITS-1) )
      number[ sign_exp_length ]  += ( 1L<<(NO_OF_BITS-1) );
    else
      number[ sign_exp_length-1 ]  += ( 1L<<( NO_OF_BITS-sign_exp_rest-2 ) );
  }
}


/*-----------------------------------------------------------------------------
name        :Smallest_Norm()
description :sets number to samllest normal (or tiny T) 
called from :getFPRootNumber()
call fns    :/
exceptions  :/
algorithm   :
Global var. used :sign_exp_length,exp_rest,hidden
-----------------------------------------------------------------------------*/
void Smallest_Norm( ulong32 *number)
{
  // if (hidden) // change DV
  number[ sign_exp_length-1 ]  += ( 1L<<( NO_OF_BITS-sign_exp_rest-1 ) );
  if ( ( !hidden )  && ( sign_exp_rest==(NO_OF_BITS-1) ) )
    number[ sign_exp_length ]  += ( 1L<<(NO_OF_BITS-1) );
  else if ( !hidden )
    number[ sign_exp_length-1 ]  += ( 1L<<( NO_OF_BITS-sign_exp_rest-2 ) );
}


/*-----------------------------------------------------------------------------
name        :Plus()
description :operates p<digit> operation in the coded number
called from :ModifierPorM()
call fns    :null_exp()
exceptions  :/
algorithm   :
Global var. used :sign_exp_length,exp_rest
-----------------------------------------------------------------------------*/
void Plus( ulong32 *number, ulong32 k)
{
  ulong32 j, i;

  //add 1 to exponent k times
  for ( i=0;i<k;i++ ) {
    j = sign_exp_length-1;
    number[ j ]  += ( 1L<<( (NO_OF_BITS-1)-sign_exp_rest ) );
    if ( ( ( number[ j ] >>( NO_OF_BITS-sign_exp_rest-1 ) )  == 0 )  && ( j!=0 ) )      //only exponent part
    {
      j--;
      number[ j ]  += 1;
    }
    if ( j>0 )
      if ( number[ j ] ==0 )  {
        j--;
        number[ j ]  += 1;
      } // if
  }
}


/*-----------------------------------------------------------------------------
name        :Minus()
description :operates m<digits> operation in coded number
called from :ModifierPorM
call fns    :null_exp()
exceptions  :/
algorithm   :
Global var. used :sign_exp_length,exp_rest
-----------------------------------------------------------------------------*/
void Minus( ulong32 *number, ulong32 k)
{
  ulong32 j, i;

  //decrement exponent with 1, k times
  // Only works for e<=31
  for ( i=0;i<k;i++ ) {
    j = sign_exp_length-1;
    number[ j ]  -= ( 1L<<( NO_OF_BITS-sign_exp_rest-1 ) );
    if ( ( number[ j ] >>( NO_OF_BITS-sign_exp_rest-1 ) )  == ( 0xFFFFFFFFUL>>( NO_OF_BITS-sign_exp_rest-1 ) )  && ( j!=0 ) ) {
      j--;
      number[ j ]  -= 1;
    }
    while ( ( j>0 )  && ( number[ j ]  == 0xFFFFFFFFL ) ) {
      j--;
      number[ j ]  -= 1;
    }
  }
}


/*-----------------------------------------------------------------------------
name        :MinusBk()
description :divide Bias by 2^k ... operation mB<digits>
called from :ModifierPorM
call fns    :/
exceptions  :/
algorithm   :
Global var. used :
-----------------------------------------------------------------------------*/
void MinusBk( ulong32 *number, ulong32 k)
{
  // incorrect, only for e <= 31    // for exp <= 2^31
  ulong32 Bk = 0;
  ulong32 i;	//different from Papadopoulo: i declared when needed, just unsigned

  // Calc Bk
  Bk =  0x40000000L;
  /*   Divide (Bias + 1) by k */
  for ( i=1;i<=k;i++ )  // shift exponent k places to the right
    Bk = Bk >> 1;

  // Sub Bk
  number[ 0 ]  -= Bk;
}


/*-----------------------------------------------------------------------------
name        :PlusBk()
description :multiply B by 2^k ... operation pB<digits>
called from :ModifierPorM()
call fns    :/
exceptions  :/
algorithm   :
Global var. used :
-----------------------------------------------------------------------------*/
void PlusBk( ulong32 *number, ulong32 k)
{
  // incorrect only works for e<=31   // for exp <= 2^31
  ulong32 Bk = 0;
  ulong32 i;	//different from Papadopoulo-release: i declared when needed and just unsigned

  // Calc Bk
  Bk =  0x40000000L;
  /*   Divide (Bias + 1) by k */
  for ( i=1;i<=k;i++ )  // shift exponent k places to the right
    Bk = Bk >> 1;

  // Add Bk
  number[ 0 ]  += Bk;

}


/*-----------------------------------------------------------------------------
name        :Incr_at_pos()
description :operates i(<pos>)<digit>
called from :ModifierIorD()
call fns    :null_exp()
exceptions  :/
algorithm   :
Global var. used :sign_exp_length,exp_rest,hidden,expon
-----------------------------------------------------------------------------*/
void Incr_at_pos( ulong32 *number, ulong32 pos, ulong32 k)
{
  ulong32 leading, j, i,position,pos_part,pos_rest;

  // determine leading bit if not hidden
  if ( ( sign_exp_rest!=(NO_OF_BITS-1) )  && ( !hidden ) )
    leading = ( number[ sign_exp_length-1 ] >>( NO_OF_BITS-sign_exp_rest-2 ) )  & 1L;
  else  if ( !hidden )
    leading = ( number[ sign_exp_length ] >>(NO_OF_BITS-1) )  & 1L;

  // calculate position
  position = expon + pos + 1 - hidden;
  pos_part = position / NO_OF_BITS + 1;
  pos_rest = position % NO_OF_BITS + 1;

  //Increment significand by 1, k times
  for ( i=0;i<k;i++ )  {
    if ( pos_rest==0 )  {
      number[ pos_part-2 ]  += 1;
      j = pos_part-3;
      while ( ( j>0 )  && ( number[ j ] ==0 ) ) {
        j--;
        number[ j ]  += 1;
      }
    } // if
    else {
      number[ pos_part-1 ]  += ( 1L<<( NO_OF_BITS-pos_rest ) );
      j = pos_part-1;
      if ( ( number[ j ] >>( NO_OF_BITS-pos_rest ) )  == 0 )  {
        j--;
        number[ j ]  += 1;
        while ( ( j>0 )  && ( number[ j ] ==0 ) ) {
          j--;
          number[ j ]  += 1;
        }
      } // if
    } // else
  } //for

  // replace hidden bit

  if ( ( !hidden )  && ( j<sign_exp_length ) ) {
    if ( ( sign_exp_rest==(NO_OF_BITS-1) )  && ( ( number[ sign_exp_length ] ) ==0 ) ) {
      if ( leading==0 )
        number[ sign_exp_length-1 ]  += 1;          //exp+1
      else if ( !null_exp( number) )
        number[ sign_exp_length ]  |= ( 1L<<(NO_OF_BITS-1) );     //leading 1
    }
    else if ( ( number[ sign_exp_length-1 ]  << ( sign_exp_rest+2 ) ) ==0 ) {
      if ( leading==0 )
        number[ sign_exp_length-1 ]  += 1L<<( NO_OF_BITS-sign_exp_rest-1 );      //exp+1
      else if ( !null_exp( number) )
        number[ sign_exp_length-1 ]  |= ( 1L<<( NO_OF_BITS-sign_exp_rest-2 ) );    //leading 1
    }

  } else if ( ( !hidden )  && ( j==sign_exp_length ) ) {
    if ( ( sign_exp_rest==(NO_OF_BITS-1) )  && ( ( number[ sign_exp_length ] ) ==0 ) ) {
      if ( leading==0 )
        number[ sign_exp_length-1 ]  += 1;
      else if ( !null_exp( number) )
        number[ sign_exp_length-1 ]  |= ( 1L << (NO_OF_BITS-1) );
    }
  }
}


/*-----------------------------------------------------------------------------
name        :Incr()
description :operates i<digit> on the coded number
called from :ModifierIorD()
call fns    :null_exp()
exceptions  :/
algorithm   :
Global var. used :length,hidden,sign_exp_length,exp_rest,rest
-----------------------------------------------------------------------------*/
void Incr( ulong32 *number, ulong32 k)
{
  ulong32 leidende, j, i;

  //bepalen van leidende bit als deze niet hidden is
  if ( ( sign_exp_rest!=(NO_OF_BITS-1) )  && ( !hidden ) )
    leidende = ( number[ sign_exp_length-1 ] >>( NO_OF_BITS-sign_exp_rest-2 ) )  & 1L;
  else  if ( !hidden )
    leidende = ( number[ sign_exp_length ] >>(NO_OF_BITS-1) )  & 1L;

  //1 optellen bij de significand en dit k keer
  for ( i=0;i<k;i++ ) {
    if ( rest==0 )
      number[ length-1 ]  += 1;
    else
      number[ length-1 ]  += ( 1L<<( NO_OF_BITS-rest ) );
    j = length-1;
    while ( ( j>0 )  && ( number[ j ] ==0 ) )  // carry propagation
    {
      j--;
      number[ j ]  += 1;
    }
  }

  if ( ( !hidden )  && ( j<sign_exp_length ) ) {
    if ( ( sign_exp_rest==(NO_OF_BITS-1) )  && ( ( number[ sign_exp_length ] ) ==0 ) ) {
      if ( leidende==0 )
        number[ sign_exp_length-1 ]  += 1;          //exp+1
      else if ( !null_exp( number) )
        number[ sign_exp_length ]  |= ( 1L<<(NO_OF_BITS-1) );     //leidende 1
    }
    else if ( ( number[ sign_exp_length-1 ]  << ( sign_exp_rest+2 ) ) ==0 ) {
      if ( leidende==0 )
        number[ sign_exp_length-1 ]  += 1L<<( NO_OF_BITS-sign_exp_rest-1 );      //exp+1
      else if ( !null_exp( number ) )
        number[ sign_exp_length-1 ]  |= ( 1L<<( NO_OF_BITS-sign_exp_rest-2 ) );    //leidende 1
    }

  } else if ( ( !hidden )  && ( j==sign_exp_length ) ) {
    if ( ( sign_exp_rest==(NO_OF_BITS-1) )  && ( ( number[ sign_exp_length ] ) ==0 ) ) {
      if ( leidende==0 )
        number[ sign_exp_length-1 ]  += 1;
      else if ( !null_exp( number) )
        number[ sign_exp_length-1 ]  |= ( 1L << (NO_OF_BITS-1) );
    }
  }
}


/*-----------------------------------------------------------------------------
name        :Decr_at_pos()
description :operates d(<pos>)<digit>  on the coded number
called from :ModifierIorD()
call fns    :null_exp()
exceptions  :/
algorithm   :
Global var. used :hidden,sign_exp_length,exp_rest,expon
-----------------------------------------------------------------------------*/
void Decr_at_pos( ulong32 *number, ulong32 pos, ulong32 k)
{
  ulong32 leading, j, i,position,pos_part,pos_rest;
  
  // determine leading bit if not hidden
  if ( ( sign_exp_rest!=(NO_OF_BITS-1) )  && ( !hidden ) )
	leading = ( number[ sign_exp_length-1 ] >>( NO_OF_BITS-sign_exp_rest-2 ) )  & 1L;
  else  if ( !hidden )
	leading = ( number[ sign_exp_length ] >>(NO_OF_BITS-1) )  & 1L;
  
  // calculate position
  position = expon + pos + 1 - hidden;
  pos_part = position / NO_OF_BITS + 1;
  pos_rest = position % NO_OF_BITS + 1;
  
  for ( i=0;i<k;i++ )  {
    if ( pos_rest==0 )  {
      number[ pos_part-2 ]  -= 1;
      j = pos_part-3;
      while ( ( j>0 )  && ( number[ j ]  == 0xFFFFFFFFL ) )  {
        j--;
        number[ j ]  -= 1;
      } // while
    }
    else {
      number[ pos_part-1 ]  -= ( 1L<<( NO_OF_BITS-pos_rest ) );
      j = pos_part-1;
      if ( ( number[ j ] >>( NO_OF_BITS-pos_rest ) )  == ( 0xFFFFFFFFUL>>( NO_OF_BITS-pos_rest ) ) )  {
        j--;
        number[ j ]  -= 1;
        while ( ( j>0 )  && ( number[ j ]  == 0xFFFFFFFFL ) )  {
          j--;
          number[ j ]  -= 1;
        } // while
      } // if
    } // else
  } // for

  // bepalen van leidende bit als deze niet hidden is
  // change DV

  if ( ( !hidden )  && ( !null_exp( number) ) )  {
    j = sign_exp_length-1;

    if ( ( number[ j ]  >> ( 30-sign_exp_rest ) )  % 2 == 0 )  {
      j = sign_exp_length-1;
      number[ j ]  -= ( 1L<<( (NO_OF_BITS-1) - sign_exp_rest ) );
      if ( number[ j ] >>( (NO_OF_BITS-1)-sign_exp_rest )  == 0xFFFFFFFFUL>>( (NO_OF_BITS-1)-sign_exp_rest ) ) {
        j--;
        number[ j ]  -= 1;
      }
      while ( ( j>0 )  && ( number[ j ]  == 0xFFFFFFFFL ) ) {
        j--;
        number[ j ]  -= 1;
      }
      // replace leading bit if necessary
      if ( sign_exp_rest==(NO_OF_BITS-1) )
        number[ sign_exp_length ]  |= 1L<<(NO_OF_BITS-1);
      else
        number[ sign_exp_length-1 ]  |= 1L<<( 30-sign_exp_rest );
    }
  } // if
}


/*-----------------------------------------------------------------------------
name        :Decr()
description :operates d<digit> on the coded number
called from :ModifierIorD()
call fns    :null_exp()
exceptions  :/
algorithm   :
Global var. used :length,hidden,sign_exp_length,exp_rest,rest
-----------------------------------------------------------------------------*/
void Decr( ulong32 *number, ulong32 k)
{
  unsigned j, i;

  //decrement significand k times
  for ( i=0;i<k;i++ ) {
    j = length-1;
    if ( rest==0 )
      number[ j ]  -= 1;
    else {
      number[ j ]  -= ( 1L<<( NO_OF_BITS-rest ) );
      if ( ( number[ j ] >>( NO_OF_BITS-rest ) )  == ( 0xFFFFFFFFUL>>( NO_OF_BITS-rest ) )  && ( j!=0 ) ) {
        j--;
        number[ j ]  -= 1;
      }
    }
    while ( ( j>0 )  && ( number[ j ]  == 0xFFFFFFFFL ) ) {
      j--;
      number[ j ]  -= 1;
    }
  }

  // change DV
  if ( ( !hidden )  &&
       ( !null_exp( number) ) )  {
    // leading bit = zero ?
    j = sign_exp_length-1;
    
    if (  ( ( sign_exp_rest < (NO_OF_BITS-1) )  && !( number[ j ]  & ( 1L << (30 - sign_exp_rest) ) ) )  ||
          ( ( sign_exp_rest == (NO_OF_BITS-1) )  && !( number[ j ]  & ( 1L << (NO_OF_BITS-1) ) ) )  )  {
      // leading bit == 0
    
      if ( sign_exp_rest == (NO_OF_BITS-1) )  {
        j++;
        number[ j ]  -=  1L;
        if ( ( j > 0 )  && ( number[ j ]  == 0xFFFFFFFFL ) )  {
          j--;
          number[ j ]  -= 1;
        }
      } else {
        /// change DV 28/01/2000
    
        if ( hidden && ( !( ( number[ j ]  >> ( (NO_OF_BITS-1) - sign_exp_rest ) )  & 1L ) ) )  {
          // trailing exp bit == 0
          number[ j ]  += 1L << ( (NO_OF_BITS-1) - sign_exp_rest );
          j--;
          number [ j ]  -= 1;
    
          while ( ( j>0 )  && ( number[ j ]  == 0xFFFFFFFFL ) ) {
            j--;
            number[ j ]  -= 1;
          }
        } else if ( sign_exp_rest == 0 )  { // change DV 8/3/2000
          if ( !( number[ j ]  & ( 1L << ( (NO_OF_BITS-1) - sign_exp_rest ) ) ) )  { // leading exp. bit = 0
            number[ j-1 ]  -= 1; // borrow carry
            number[ j ]  += ( 1L << ( (NO_OF_BITS-1) - sign_exp_rest ) );
          } else
            number[ j ]  -= ( 1L << ( (NO_OF_BITS-1) - sign_exp_rest ) );
    
        }
        else {
          number[ j ]  -= ( 1L << ( (NO_OF_BITS-1) - sign_exp_rest ) );
    
        }

      }
    }
  }
  // replace leading bit if necessary
  if ( ( !hidden )  && ( !null_exp( number) ) )  {
    if ( sign_exp_rest==(NO_OF_BITS-1) )
      number[ sign_exp_length ]  |= 1L<<(NO_OF_BITS-1);
    else
      number[ sign_exp_length-1 ]  |= 1L<<( 30-sign_exp_rest );
  }
}


/*-----------------------------------------------------------------------------
name        :null_exp()
description :checks if exponent is zero
called from :Plus(),Minus(),Incr_at_pos(),Incr(),Decr_at_pos(),
		Decr(),ModifierU().
call fns    :/
exceptions  :/
algorithm   :
Global var. used :sign_exp_length,exp_rest,length
-----------------------------------------------------------------------------*/
ulong32 null_exp( const ulong32 *number)
{
  ulong32 m, null=1;

  for ( m=1;m<sign_exp_length-1;m++ ) {
    if ( number[ m ] !=0 )
      null = 0;
  }
  if ( ( sign_exp_rest==0 )  && ( ( ( number[ 0 ]  & 0x7FFFFFFFL )  << 1 )  != 0 ) )
    null=0;
  if ( ( ( number[ 0 ]  & 0x7FFFFFFFL )  >> ( NO_OF_BITS-sign_exp_rest-1 ) )  != 0 )
    null=0;
  if ( ( length != 1 )  && ( sign_exp_length !=1 )  && ( ( number[ sign_exp_length-1 ]  >> ( NO_OF_BITS-sign_exp_rest-1 ) )  != 0 ) )
    null=0;

  return null;
}

/*-----------------------------------------------------------------------------
name        :one_exp()
description :checks if exponent is one
called from :ModifierU()
call fns    :/
exceptions  :/
algorithm   :
Global var. used :sign_exp_length,exp_rest
-----------------------------------------------------------------------------*/
ulong32 one_exp( const ulong32 *number)
{
  ulong32 m, one=0;

  if ( ( ( number[ sign_exp_length-1 ]  >> ( NO_OF_BITS - sign_exp_rest - 1 ) )  | 1L )  == 1L )
    one = 1;

  for ( m=0;m<sign_exp_length-1;m++ ) {
    if ( number[ m ]  != 0 )
      one = 0;
  }
  if ( ( sign_exp_length > 1 )  && ( ( number[ 0 ]  & 0x7FFFFFFFL )  != 0 ) )
    one = 0;

  return one;
}


/*-----------------------------------------------------------------------------
name        :mygetc()
description :apply getc() of <stdio.h> to the present location of file in use
called from :whole program 
call fns    :/
exceptions  :/
algorithm   :/
Global var. used :/
-----------------------------------------------------------------------------*/
int mygetc( FILE *f )
{
  int c;
  c = getc( f );
  return c;
}


/*-----------------------------------------------------------------------------
name        :myungetc()
description :apply ungetc() of <stdio.h> to the present location of file in use 
called from :whole program
call fns    :/
exceptions  :/
algorithm   :/
Global var. used :/
-----------------------------------------------------------------------------*/
void myungetc( int c, FILE *f )
{
  ungetc( c, f );
}

/* ============================ END OF FILE ============================*/
