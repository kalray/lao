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


$Id: UCB.h 77 2005-10-27 12:52:24Z huesken $

                                                                     
*/
/**	\file
 * 	\brief Declaration and definition of Class UCB
 */

#ifndef _UCB_H
#define _UCB_H

// ----
// Includes
// ----
#include <string>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <Fp.h>

// ----
// Defines
// ----
#define BUF_LEN    1024
#define FP_STR      256

#define NO_FLAGS_INEXACT      16
#define NO_FLAGS_OVERFLOW     8
#define NO_FLAGS_UNDERFLOW    4
#define NO_FLAGS_INVALID      2
#define NO_FLAGS_DIV_BY_ZERO  1

/*class definition*/
/**	\brief This class uses templates to specify the floating point class that is derived from FP for library testing.
 *
 * 	Created to easily test a line of UCB-output.
 * 	all according SUN-UCB floating point presentation
 *
 * 	The data members in the UCB-class are all the elements of one UCB-vector;
 * 	the operands and the correct result are stored in template T, as declared in UCB<T>,
 * 	where T must be derived class of FP.
 * 	Similarly, the rounding mode is stored in the static FpEnv control/status word from the FP class.
 * 	The purpose of the UCB class member functions is to read and test a UCB-testvector,
 * 	with regard to check if the calculated result from class T equals the UCB output.
 * 	To make this possible, the result (object from class T) and the exceptions,
 * 	generated from the derived FP class and stored in fpEnv, needs to be called.
 */

template<class T> class UCB
{

protected:

/// test only ieee-vectors?
  int ieee;

/// number of lines read
  int line;

/// size of the exponent
  int sizeE;

/// size of the mantissa, 
  int sizeM;

/// hidden bit (yes/no),
  int hidden;

/// this variable holds the flags that should not be tested
  int noFlags;

  int dsizeE, dsizeM, dhidden, signedZero, ieeeVector;

/// UCB operation to be tested
  char operation[ 10 ];
  
/// UCB precision
  char prec;

/// UCB rounding mode
  char rounding;
  
/// UCB compare result
  char compare[ 5 ];
  
/// UCB exceptions
  char exceptions[ 10 ];

/// first operand
  T operand1;
  
/// second operand
  T operand2;
  
/// expected result of the operation
  T correct_result;
  
/// real result of the operation
  T computed_result;
  
  int pre, post; // to accomodate '?'

  unsigned int errors,warnings,skipped;

/// adds comment to log file
  virtual void PrintError(ostream & logfile);

public:

  int signalu,notsignalu,signalv,signalw,notsignalv,notsignalw,nou,nov,now,allops;

/// constructor
  UCB( );

/// deconstructor
  virtual ~UCB( );

/// reset internal variables
  virtual void Reset ( );
  
/// sets the library/machine rounding mode in FP 'controle/status'-word fpEnv
  void SetFPRound ( );

/// if str==NULL then this function reads a line from the input stream.
  int ReadLine(istream & is,int sZero=1, int nflags=0,int *lines=NULL );
/// if str==NULL then this function reads a line from the input stream.
  int ReadLine(const char* str=NULL,int sZero=1, int nflags=0,int *lines=NULL );

/// returns the UCB operation
  char* GetOperation( );	//TODO: unused?

/// returns the UCB precision
  char  GetPrecision( );

/// returns the UCB rounding mode
  char  GetRounding( );

/// returns the UCB compare
  char* GetCompare( );	//TODO: unused?

/// returns the UCB exceptions
  char* GetExceptions( );

/// returns the first operand
  T &  GetOperand1( );

/// returns the second operand
  T &  GetOperand2( );

/// returns the (correct) result
  T &  GetResult( );

/// set value of "ieee"-var
  void SetIeee(int i) { ieee = i; }

/// does the testing
  void DoLine( int tiny,int inf, int nan);

/// checks the result of the test and the flags
  virtual void Compare (ostream & logfile);

/// checks underflow mechanism
  virtual void CheckUnderflow(int tiny , ostream & logfile);

/// adds summary to logfile
  virtual void Close( int tiny , ostream & logfile );
  
/// Return number of errors
  unsigned int GetErrors(void) { return errors; }
   
/// Return number of warnings
  unsigned int GetWarnings(void) { return warnings; }
  
};	//end of declaration part

//begin of definition

//constructor
template <class T>
UCB<T>::UCB( )
{
  Reset();
}

// reset internal variables to default state
template <class T>
void UCB<T>::Reset(void)
{
  line = 0;
  //  correct_result.SetEnvironment(); change BV
  signalu = 0;
  signalv = 0;
  signalw = 0;
  notsignalu = 0;
  notsignalv = 0;
  notsignalw = 0;
  nou= 1;
  nov = 1;
  now = 1;
  pre = post = 0;
  errors = 0;
  warnings = 0;
  skipped = 0;
  allops = 0;
  
  ieee = 1;
}

//deconstructor
template <class T>
UCB<T>::~UCB( )
{}


/***********************************************************************
* Function:	void CheckUnderflow(int tiny)
* Purpose:	adds summary to logfile
* Return:	void
***********************************************************************/
template <class T>
void UCB<T>::CheckUnderflow( int tiny , ostream & logfile)
{
  if ( tiny )   { // conclusion underflow
    if ( nou )  {
      if ( nov )  {
        if ( !( now ) )
          std::cout << "Warning: only 'w' underflow cases in the testset" << endl;
      } else
        if ( now )
          logfile << "Warning: only 'v' underflow cases in the testset" << endl;
        else
          logfile << "Warning: only 'v' and 'w' underflow cases in the testset" << endl;
    } // if
    else {
      if ( nov )  {
        if ( now )  {
          logfile << "Warning: no special 'v' or 'w' underflow cases in the testset" << endl;
          if ( signalu && !notsignalu )
            logfile << "Implementation signals underflow in case the result" << endl << "(1) is tiny after rounding and" << endl << "(2) suffers denormalization loss" << endl << "('u' -  underflow)" << endl;
          else if (signalu && notsignalu)
           {
//            logfile << "u-underflow inconsistent (1)" << endl;
            logfile << "Implementation signals underflow in case the result" << endl << "(1) is tiny after rounding and" << endl << "(2) suffers denormalization loss" << endl << "('u' -  underflow)" << endl;
            logfile << "Warning: Underflow mode used inconsistently!" << endl;
           }
        } else
          logfile << "Warning: no special 'v' underflow cases in the testset" << endl;
      } else {
        if ( now )  {
          logfile << "Warning: no special 'w' underflow cases in the testset" << endl;
          if ( ( signalu && !notsignalu )  && ( signalv && !notsignalv ) )
            logfile << "Implementation signals underflow in case the result" << endl << "(1) is tiny after rounding and" << endl << "(2) raises the inexact exception" << endl << "('v' - underflow)" << endl;
          else if ((signalu && signalv) && (notsignalu || notsignalv))
           {
//             logfile << "v-underflow inconsistent (2)" << endl;
             logfile << "Implementation signals underflow in case the result" << endl << "(1) is tiny after rounding and" << endl << "(2) raises the inexact exception" << endl << "('v' - underflow)" << endl;
             logfile << "Warning: Underflow mode used inconsistently!" << endl;
           }
        } else if ( signalu && !notsignalu )  {
          if ( signalw && !notsignalw )
            logfile << "Implementation signals underflow in case the result" << endl << "(1) is tiny before rounding and" << endl << "(2) raises the inexact exception" << endl << "('w' - underflow)" << endl;
          else if ( signalv && !notsignalv )
            logfile << "Implementation signals underflow in case the result" << endl << "(1) is tiny after rounding and" << endl << "(2) raises the inexact exception"<< endl << "('v' - underflow)" << endl;
          else
            logfile << "Implementation signals underflow in case the result" << endl << "(1) is tiny after rounding and" << endl << "(2) suffers denormalization loss" << endl << "('u' -  underflow)" << endl;
        } // else
      else if (notsignalu || notsignalv || notsignalw)
       {
         if (signalu)
          {
            if (signalw)
             {
//              logfile << "w-underflow inconsistent (3)" << endl;
               logfile << "Implementation signals underflow in case the result" << endl << "(1) is tiny before rounding and" << endl << "(2) raises the inexact exception" << endl << "('w' - underflow)" << endl;
               logfile << "Warning: Underflow mode used inconsistently!" << endl;
             }
            else if (signalv)
             {
//              logfile << "v-underflow inconsistent (4)" << endl;
               logfile << "Implementation signals underflow in case the result" << endl << "(1) is tiny after rounding and" << endl << "(2) raises the inexact exception" << endl << "('v' - underflow)" << endl;
               logfile << "Warning: Underflow mode used inconsistently!" << endl;
             }
            else
             {
//               logfile << "u-underflow inconsistent (5)" << endl;
               logfile << "Implementation signals underflow in case the result" << endl << "(1) is tiny after rounding and" << endl << "(2) suffers denormalization loss" << endl << "('u' -  underflow)" << endl;
               logfile << "Warning: Underflow mode used inconsistently!" << endl;
             } 
          } // (mh) if
       } // (mh) else if
      } // else
    } // else
   if (!(signalu || signalv || signalw))
     logfile << "Error: Could not detect underflow mechanism" << endl;
  } // if tiny
}

/***********************************************************************
* Function:	void Close(int tiny)
* Purpose:	adds summary to logfile
* Return:	void
***********************************************************************/
template <class T>
void UCB<T>::Close( int tiny , ostream & logfile )
{
  logfile << "Summary:  " << endl;
  logfile << "--------  " << endl;

  CheckUnderflow(tiny, logfile);
 
  logfile << "Errors:   " << errors << "/" << allops << endl;
  logfile << "Warnings: " << warnings << "/" << allops << endl;
  logfile << "Skipped:  " << skipped << "/" << allops << endl;
}


// Read lines from infile
template <class T>
int UCB<T>::ReadLine(istream & is, int sZero, int nflags, int *lines )
{
  static char buf[BUF_LEN];
  if (is.good() && !is.eof())
   {
     is.getline(buf, BUF_LEN);
     if (strlen(buf)>0) return ReadLine(buf, sZero, nflags, lines);
   }
  return 0;
}

/**	\brief	If str==NULL then this function reads a line from the input stream.
 *
 * 	The variables str, nflags and lines make it possible to use the UCB-class in the Coonen Driver program.
 */
/***********************************************************************
* Function:	int ReadLine(char *str=NULL, int *nflags=NULL, int *lines=NULL)
* Purpose:	read line from the input stream
* Return:
***********************************************************************/
template <class T>
int UCB<T>::ReadLine(const char* str,int sZero,int nflags,int *lines )
{
  int i,j,k,count;
  Hex tmphex,tmphex2;
  Bitstring tmpbitstring,tmpbitstring2;
  static char buf[ BUF_LEN ];
  static char tmp[ FP_STR ];
  line++;
  signedZero=sZero;
  pre = post = 0;
  
  // check if we have input and progress...
  if (str && strlen(str)>0 && str[0]!='!')
   {
    strcpy(buf, str);
    if (lines) line=*lines;
    noFlags=nflags;

    i=0;
    while ( buf[ i ] != ' ' ) {
      operation[ i ] =buf[ i ];
      i++;
    }
   
    operation[ i ] ='\0';
   
    if ( !isdigit( operation[ strlen( operation ) -1 ] ) ) {
      prec= operation[ strlen( operation ) -1 ];
      operation[ strlen( operation ) -1 ] ='\0';
      switch ( prec ) {
        case 's':
          sizeE=8;
          sizeM=23+1;
          hidden=1;
          ieeeVector=0;
          break;
        case 'S':
          sizeE=8;
          sizeM=23+1;
          hidden=1;
          ieeeVector=1;
          break;
        case 'd':
          sizeE=11;
          sizeM=52+1;
          hidden=1;
          ieeeVector=0;
          break;
        case 'D':
          sizeE=11;
          sizeM=52+1;
          hidden=1;
          ieeeVector=1;
          break; 
        case 'l':
          sizeE=15;
          sizeM=64 + 1;
          hidden=0;
          ieeeVector=0;
          break;
        case 'L':
          sizeE=15;
          sizeM=64 + 1;
          hidden=0;
          ieeeVector=1;
          break;
        case 'q':
          sizeE=15;
          sizeM=112+1;
          hidden=1;
          ieeeVector=0;
          break;
        case 'Q':
          sizeE=15;
          sizeM=112+1;
          hidden=1;
          ieeeVector=1;
          break;
        case 'm':
          sizeE=15;
          sizeM=240 + 1;
          hidden = 0;
          ieeeVector=0; 
          break;
        case 'M':
          sizeE=15;
          sizeM=240 + 1;
          hidden = 0;
          ieeeVector=1;
          break;
      }
    } else {
 
      ieeeVector=0; // Cannot be ieeeVector!
      i--;
      while ( isdigit( buf[ i ] ) )  i--; // rewind
      i++;
      operation[ i ]  = '\0'; // ignore digit after operation
      j = 0;
      while ( buf[ i ] != ' ' ) tmp[ j++ ] =buf[ i++ ];
      tmp[ j ] ='\0';
      sizeE = atoi( tmp );
      while ( buf[ ++i ] == ' ' );
      j = 0;
      while ( buf[ i ] != ' ' ) tmp[ j++ ] =buf[ i++ ];
      tmp[ j ] ='\0';
      hidden = atoi( tmp );
      while ( buf[ ++i ] == ' ' );
      j = 0;
      while ( buf[ i ]  != ' ' ) tmp[ j++ ] =buf[ i++ ];
      tmp[ j ] ='\0';
      sizeM = atoi( tmp );
      sizeM++ ;  // +1 for the sign
    }
    
    while ( buf[ i ]  == ' ' )  i++;

    // read destination format
    if ( ( strncmp( operation,"rt",2 )  == 0 )  || ( strncmp( operation,"ct",2 )  == 0 ) )  {
      if ( !isdigit( buf[ i ] ) )  {
        prec = buf[ i ];
        switch ( prec )  {
          case 's':
            dsizeE=8;
            dsizeM=23+1;
            dhidden=1;
            break;
          case 'd':
            dsizeE=11;
            dsizeM=52+1;
            dhidden=1;
            break;
          case 'l':
            dsizeE=15;
            dsizeM=64 + 1;
            dhidden=0;
            break;
          case 'q':
            dsizeE=15;
            dsizeM=112+1;
            dhidden=1;
            break;
          case 'm':
            dsizeE=15;
            dsizeM=240 + 1;
            dhidden = 0;
            break;
        } // switch
        
        i++;
        while ( buf[ i ]  == ' ' )  i++;
      } // if
      else {
        j = 0;
        while ( buf[ i ]  != ' ' ) tmp[ j++ ] =buf[ i++ ];
        tmp[ j ] ='\0';
        dsizeE = atoi( tmp );
        while ( buf[ ++i ] == ' ' );
        j = 0;
        while ( buf[ i ]  != ' ' ) tmp[ j++ ]  = buf[ i++ ];
        tmp[ j ] ='\0';
        dhidden = atoi( tmp );
        while ( buf[ ++i ] == ' ' );
        j = 0;
        while ( buf[ i ]  != ' ' ) tmp[ j++ ] =buf[ i++ ];
        tmp[ j ] ='\0';
        dsizeM = atoi( tmp );
        dsizeM++;

      } // else

      while ( buf[ i ]  == ' ' )  i++;
    } // if
    else {
      dsizeE = sizeE;
      dhidden = hidden;
      dsizeM = sizeM;
    } // else

    rounding=buf[ i ];
    while ( buf[ ++i ] == ' ' );
    j=0;
    while ( buf[ i ]  != ' ' ) compare[ j++ ] =buf[ i++ ];
    compare[ j ] ='\0';
    while ( buf[ ++i ] == ' ' );
    j=0;
    while ( buf[ i ]  != ' ' )  {
      exceptions[ j++ ] =buf[ i++ ];
      exceptions[ j++ ] =' ';
    }
    exceptions[ j ] ='\0';
    while ( buf[ ++i ] == ' ' );
    if ( ( strncmp( operation,"ci",2 ) ==0 )  ||
         ( strncmp( operation,"cu",2 ) ==0 ) )  {
      count = 32; // 32 bit integer
      i += 2; // avoid 0x
      
      for ( j=0; j<count;j++ )  {
      if ( isdigit( buf[ i ] )  || ( ( buf[ i ]  >= 'a' )  && ( buf[ i ]  <= 'f' ) ) )
          tmp[ j ]  = buf[ i++ ];
        else {
          tmp[ j ]  = 0;
          break;
        }
      } // for
      
    for ( ; j<count;j++ )
        tmp[ j ]  = 0;
        
    } else if ( ( strncmp( operation,"cI",2 ) ==0 )  ||
                ( strncmp( operation,"cU",2 ) ==0 ) )  {
      count = 64; // 64 bit integer
      i += 2; // avoid 0x
      for ( j=0; j<count;j++ )  {
      if ( isdigit( buf[ i ] )  || ( ( buf[ i ]  >= 'a' )  && ( buf[ i ]  <= 'f' ) ) )
          tmp[ j ]  = buf[ i++ ];
        else {
          tmp[ j ]  = 0;
          break;
        }
      }
      for ( ; j<count;j++ )
        tmp[ j ]  = 0;
        
    } else if ( strncmp( operation,"d2b",3 )  == 0 )  {
      j = 0;
      while ( buf[ i ]  != ' ' )  {
        tmp[ j++ ]  = buf[ i++ ];
      }
      
    } else {
      count =( int )  ceil( ( double ) ( sizeM+sizeE ) /32.0 ) *8;
      for ( j=0; j<count ;j++ ) {
        if ( buf[ i ] ==' ' ) i++;
        tmp[ j ]  = buf[ i++ ];
      }
    }
    tmp[ j ] ='\0';

    if ( strncmp( operation,"d2b",3 )  == 0 )  {
      operand1 = T( sizeM-1, sizeE, hidden ); // sets Mantissa and exp right
      // only create new string if there isn't already one
      if (operand1.decimal == NULL)
      operand1.decimal = new char[ maxstr ];
      for ( k = 0; k <= j; k++ )
        operand1.decimal[ k ]  = tmp[ k ];
      operand1.decimal[ k ]  = '\n';
    } else if ( strncmp( operation,"ci",2 ) ==0 )  {
      tmphex.StringToBitstr( tmp );
      operand1 = T( tmphex,32,0,0 );
    } else if ( strncmp( operation,"cu",2 ) ==0 )  {
      tmphex.StringToBitstr( tmp );
      operand1 = T( tmphex,32,0,0 );
    } else if ( strncmp( operation,"cI",2 ) ==0 )  {
      tmphex.StringToBitstr( tmp );
      operand1 = T( tmphex,64,0,0 );
    } else if ( strncmp( operation,"cU",2 ) ==0 )  {
      tmphex.StringToBitstr( tmp );
      operand1 = T( tmphex,64,0,0 );
    } else {
      tmphex.StringToBitstr( tmp );
      operand1 = T( tmphex,sizeM-1,sizeE,hidden );
    }
 
    i++;
    count =( int )  ceil( ( double ) ( sizeM+sizeE ) /32.0 ) *8; // reset count!

    for ( j=0; j<count ;j++ ) {
      if ( buf[ i ] ==' ' )
        i++;
      tmp[ j ]  = buf[ i++ ];
    }

    tmp[ j ] ='\0';
    tmphex.StringToBitstr( tmp );
    operand2 = T( tmphex,sizeM-1,sizeE,hidden );
    
    if ( buf[ i+1 ]  == '?' )  {
      pre = 1;
    } else {
      if ( ( strncmp( operation,"ri",2 ) ==0 )  ||
           ( strncmp( operation,"ru",2 ) ==0 ) )  {
        count = 32; // 32 bit integer
        i += 3; // avoid 0x
        for ( j=0; j<count;j++ )  {
          if ( isdigit( buf[ i ] )  || ( ( buf[ i ]  >= 'a' )  && ( buf[ i ]  <= 'f' ) ) )
            tmp[ j ]  = buf[ i++ ];
          else {
            tmp[ j ]  = 0;
            break;
          }
        } // for
        for ( ; j<count;j++ )
          tmp[ j ]  = 0;
      }
      else if ( ( strncmp( operation,"rI",2 ) ==0 )  ||
                ( strncmp( operation,"rU",2 ) ==0 ) )  {
        count = 64; // 64 bit integer
        i += 3; // avoid 0x
        for ( j=0; j<count;j++ )  {
          if ( isdigit( buf[ i ] )  || ( ( buf[ i ]  >= 'a' )  && ( buf[ i ]  <= 'f' ) ) )
            tmp[ j ]  = buf[ i++ ];
          else {
            tmp[ j ]  = 0;
            break;
          }
        } // for
        for ( ; j<count;j++ )
          tmp[ j ]  = 0;
      } else if ( strncmp( operation,"b2d",3 )  == 0 )  {
        i++;
        j = 0;
        // make sure scanning stops at string end...
        while ( buf[i] != '\0' && buf[ i ]  != '\n' )  {
          tmp[ j++ ]  = buf[ i++ ];
        }
      } else {
        count =( int )  ceil( ( double ) ( dsizeM+dsizeE ) /32.0 ) *8;
        i++;
        for ( j=0; j<count ;j++ ) {
          if ( buf[ i ] ==' ' )
            i++;
          tmp[ j ]  = buf[ i++ ];
        }
      }
      
      tmp[ j ] ='\0';
      
      if ( strncmp( operation,"b2d",3 )  == 0 )  {
        // only create new string if there isn't already one
        if (correct_result.decimal == 0)
          correct_result.decimal = new char[ maxstr ];
        for ( k = 0; k <= j; k++ )
          correct_result.decimal[ k ]  = tmp[ k ];
        correct_result.decimal[ k ]  = '\0';
      }

      else if ( strncmp( operation,"ri",2 ) ==0 )  {
        tmphex.StringToBitstr( tmp );
        correct_result = T( tmphex,32,0,0 );
      } else if ( strncmp( operation,"ru",2 ) ==0 )  {
        tmphex.StringToBitstr( tmp );
        correct_result = T( tmphex,32,0,0 );
      } else if ( strncmp( operation,"rI",2 ) ==0 )  {
        tmphex.StringToBitstr( tmp );
        correct_result = T( tmphex,64,0,0 );
      } else if ( strncmp( operation,"rU",2 ) ==0 )  {
        tmphex.StringToBitstr( tmp );
        correct_result = T( tmphex,64,0,0 );
      } else {
        tmphex.StringToBitstr( tmp );
        correct_result = T( tmphex,dsizeM-1,dsizeE,dhidden );
      }
      
    }
   return 1;
  } // end scanning 
  return 0;
}


/***********************************************************************
* Function:	char *GetOperation()
* Purpose:	returns the UCB operation
* Return:	operation
***********************************************************************/
template <class T>
char* UCB<T>::GetOperation( )
{
  return operation;
}


/***********************************************************************
* Function:	char GetPrecision()
* Purpose:	returns the UCB precision
* Return:	prec
***********************************************************************/
template <class T>
char  UCB<T>::GetPrecision( )
{
  return prec;
}


/***********************************************************************
* Function:	char GetRounding()
* Purpose:	returns the UCB rounding mode
* Return:	rounding
***********************************************************************/
template <class T>
char  UCB<T>::GetRounding( )
{
  return rounding;
}


/***********************************************************************
* Function:	char *GetCompare()
* Purpose:	returns the UCB compare
* Return:	compare
***********************************************************************/
template <class T>
char* UCB<T>::GetCompare( )
{
  return compare;
}


/***********************************************************************
* Function:	char *GetExceptions()
* Purpose:	returns the UCB exceptions
* Return:	exceptions
***********************************************************************/
template <class T>
char* UCB<T>::GetExceptions( )
{
  return exceptions;
}


/***********************************************************************
* Function:	T & GetOperand1()
* Purpose:	returns the first operand
* Return:	first operand
***********************************************************************/
template <class T>
T &  UCB<T>::GetOperand1( )
{
  return operand1;
}


/***********************************************************************
* Function:	T & GetOperand2()
* Purpose:	returns the second operand
* Return:	second operand
***********************************************************************/
template <class T>
T &  UCB<T>::GetOperand2( )
{
  return operand2;
}


/***********************************************************************
* Function:	T & GetResult()
* Purpose:	returns the correct result
* Return:	correct_result
***********************************************************************/
template <class T>
T &  UCB<T>::GetResult( )
{
  return correct_result;
}


/***********************************************************************
* Function:	char *Doline(int tiny, int inf, int nan)
* Purpose:
* Return:
***********************************************************************/
template <class T>
void UCB<T>::DoLine( int tiny,int inf, int nan)
{
  if ( !( tiny )  && ( operand1.istiny( )  || operand2.istiny( )  || correct_result.istiny( ) ) )
    return; // do not test tiny denormalized numbers
  else if ( !( inf )  && ( operand1.isInf( )  || operand2.isInf( )  || correct_result.isInf( ) ) )
    return; // do not test infinities
  else if ( !( nan )  && ( operand1.isNan( )  || operand2.isNan( )  || correct_result.isNan( ) ) )
    return; // do not test NaNs

  allops++;
  SetFPRound( );
  correct_result.ClearFPEnvironment( );

#ifdef BasicOperations
  if ( strncmp( operation,"add",3 ) ==0 )
    computed_result = operand1 + operand2;
  else if( strncmp( operation,"sub",3 ) ==0 )
    computed_result = operand1 - operand2;
  else if( strncmp( operation,"mul",3 ) ==0 )
    computed_result = operand1 * operand2;
  else if( strncmp( operation,"div",3 ) ==0 )
    computed_result = operand1 / operand2; // debug
  else if( strncmp( operation,"rem",3 ) ==0 )
    computed_result = operand1 % operand2;
  else if( strncmp( operation,"sqrt",4 ) ==0 )
    computed_result = operand1.sqrt( );
#endif

#ifdef Conversions
  if ( strncmp( operation,"rt",2 ) ==0 )
    computed_result = operand1.roundto( dsizeE,dsizeM-1,dhidden );
  else if ( strncmp( operation,"ct",2 ) ==0 )
    computed_result = operand1.copyto( dsizeE,dsizeM-1,dhidden );
  else if ( strncmp( operation,"i",1 ) ==0 )
    computed_result = operand1.rint( );
  else if ( strncmp( operation,"ri",2 ) ==0 )
    computed_result = operand1.ri( );
  else if ( strncmp( operation,"ru",2 ) ==0 )
    computed_result = operand1.ru( );
  else if ( strncmp( operation,"rI",2 ) ==0 )
    computed_result = operand1.rI( );
  else if ( strncmp( operation,"rU",2 ) ==0 )
    computed_result = operand1.rU( );
  else if ( strncmp( operation,"ci",2 ) ==0 )
    computed_result = operand1.ci( dsizeE,dsizeM-1,dhidden );
  else if ( strncmp( operation,"cu",2 ) ==0 )
    computed_result = operand1.cu( dsizeE,dsizeM-1,dhidden );
  else if ( strncmp( operation,"cI",2 ) ==0 )
    computed_result = operand1.cI( dsizeE,dsizeM-1,dhidden );
  else if ( strncmp( operation,"cU",2 ) ==0 )
    computed_result = operand1.cU( dsizeE,dsizeM-1,dhidden );
  else if ( strncmp( operation,"b2d",3 ) ==0 )  {
    unsigned i = 0;
    while ( correct_result.decimal[ i ]  != 'E' )  {
      i++;
    }
    if ( correct_result.decimal[ 0 ]  == '+' || correct_result.decimal[ 0 ]  == '-' )
      i--;
    
    computed_result = operand1.b2d( i );
  }
  else if ( strncmp( operation,"d2b",3 ) ==0 )  {
    computed_result = operand1.d2b( );
  }
#if defined(IntelPentium) && defined(NONE_TEST) && defined(Conversions)
  else if( strncmp( operation,"rem",3 ) ==0 )
    computed_result = operand1 % operand2;
  else if( strncmp( operation,"sqrt",4 ) ==0 )
    computed_result = operand1.sqrt( );
#endif

#endif

}


/***********************************************************************
* Function:	void PrintError(T &computed_result)
* Purpose:	adds comment to logfile
* Return:	void
***********************************************************************/
template <class T>
void UCB<T>::PrintError(ostream & logfile)
{
  unsigned int i;
  logfile<<"Operation: " << operation << endl;
  switch ( rounding )  {
    case 'n':
      logfile<<"Round to nearest" << endl;
      break;
    case 'z':
      logfile<<"Round to zero" << endl;
      break;
    case 'p':
      logfile<<"Round up" << endl;
      break;
    case 'm':
      logfile<<"Round down" << endl;
      break;
  } // switch
  logfile<<"Operand 1: " << operand1 << endl;
  logfile<<"Operand 2: " << operand2 << endl;
  logfile<< "Flags expected: ";
  for( i=0 ; i < strlen( exceptions );i++ )
    switch ( exceptions[ i ] ) {
      case 'x':
        logfile<<"x ";
        break;
      case 'o':
        logfile<<"o ";
        break;
      case 'u':
        logfile<<"u ";
        break;
      case 'a':
        logfile<<"v ";
        break;
      case 'b':
        logfile<<"w ";
        break;
      case 'v':
        logfile<<"i ";
        break;
      case 'd':
        logfile<<"z ";
        break;
    } // switch
  logfile << endl;
  logfile <<"Flags returned: ";
  if ( computed_result.GetFPDivByZero( ) )
    logfile << "z " ;
  if ( computed_result.GetFPInvalid( ) )
    logfile << "i " ;
  if ( computed_result.GetFPInexact( ) )
    logfile << "x " ;
  if ( computed_result.GetFPOverflow( ) )
    logfile << "o " ;
  if ( computed_result.GetFPUnderflow( ) )  {
    logfile << "u " ;
  }
  logfile << endl;
  logfile << "Correct result:  " << correct_result << endl;
  logfile << "Returned result: " << computed_result << endl<< endl;
  return;
}


/***********************************************************************
* Function:	void Compare(FP &reslt)
* Purpose:	checks the correct_result of the test and the flags
* Return:	void
***********************************************************************/
template <class T>
void UCB<T>::Compare (ostream & logfile)
{
  unsigned int i,check=1;
  if ( ( noFlags &  NO_FLAGS_DIV_BY_ZERO )  == 0 ) {
    if( computed_result.GetFPDivByZero( ) )
      if( !strchr( exceptions,'d' ) ) {
        logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") << "Line "<<line<< ": divide by zero not expected"<<endl;
        check = 0;
      }
  }
  
  if ( ( noFlags &  NO_FLAGS_INVALID )  == 0 ) {
    if( computed_result.GetFPInvalid( ) )
      if( !strchr( exceptions,'v' ) ) {
        logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": invalid not expected "<<endl;
        check = 0;
      }
  }

  if ( ( noFlags &  NO_FLAGS_INEXACT )  == 0 ) {
    if( computed_result.GetFPInexact( ) )
      if( !strchr( exceptions,'x' ) ) {
        logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": inexact not expected"<<endl;
        check = 0;
      }
  }

  if ( ( noFlags &  NO_FLAGS_OVERFLOW )  == 0 ) {
    if( computed_result.GetFPOverflow( ) )
      if( !strchr( exceptions,'o' ) ) {
        logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": overflow not expected"<<endl;
        check = 0;
      }
  }

  if ( ( noFlags &  NO_FLAGS_UNDERFLOW )  == 0 ) {
    if( computed_result.GetFPUnderflow( ) ) {
      if( !strchr( exceptions,'u' ) ) {
        if( strchr( exceptions,'a' ) )  {
          signalv = 1;
          if ( notsignalv )  {
            logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": underflow without denormalization loss previously not detected"<< endl;
            check = 0;
          } // end if
        }
        else if ( strchr( exceptions,'b' ) )  {
          signalw = 1;
          if ( notsignalw )  {
            logfile <<((ieeeVector) && !(ieee) ? "Warning " : "Error ")<<"Line "<<line<< ": underflow before rounding previously not detected"<< endl;
            check = 0;
          } // end if
        }
        else {
          logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": underflow not expected"<<endl;
          check = 0;
        } // end if
      }
      else {
        signalu = 1;
      }
    }
  } // end if

  for(i=0 ; i < strlen( exceptions );i++ ) {
    switch ( exceptions[ i ] ) {
      case 'x':
        if ( ( noFlags &  NO_FLAGS_INEXACT )  == 0 ) {
          if( !computed_result.GetFPInexact( ) ) {
            logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": inexact flag not returned"<<endl;
            check = 0;
          }
        }
        break;
      case 'o':
        if ( ( noFlags &  NO_FLAGS_OVERFLOW )  == 0 ) {
          if( !computed_result.GetFPOverflow( ) ) {
            logfile <<((ieeeVector) && !(ieee) ? "Warning " : "Error ")<<"Line "<<line<< ": overflow flag not returned"<<endl;
            check = 0;
          }
        }
        break;
      case 'u':
        nou = 0;
        if ( ( noFlags &  NO_FLAGS_UNDERFLOW )  == 0 ) {
          if( !computed_result.GetFPUnderflow( ) ) {
            logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": underflow not returned"<<endl;
            notsignalu = 1;
            check = 0;
          }
        }
        break;
      case 'a':
        nov = 0;
        if ( ( noFlags &  NO_FLAGS_UNDERFLOW )  == 0 ) {
          if( !computed_result.GetFPUnderflow( ) )  {
            notsignalv = 1;
            if ( signalv )  {
              logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": underflow without denormalization loss previously detected"<< endl;
              check = 0;
            } // end if
          } // end if
        } // end if

        break;
      case 'b':
        now = 0;
        if ( ( noFlags &  NO_FLAGS_UNDERFLOW )  == 0 ) {
          if( !computed_result.GetFPUnderflow( ) ) {
            notsignalw = 1;
            if ( signalw )  {
              logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": underflow before rounding previously detected"<< endl;
              check = 0;
            } // end if
          } // end if
        } // end if

        break;
      case 'v':
        if ( ( noFlags &  NO_FLAGS_INVALID )  == 0 ) {
          if( !computed_result.GetFPInvalid( ) ) {
            logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": invalid flag not returned"<<endl;
            check = 0;
          }
        }
        break;
      case 'd':
        if ( ( noFlags &  NO_FLAGS_DIV_BY_ZERO )  == 0 ) {
          if( !computed_result.GetFPDivByZero( ) ) {
            logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ")  <<"Line "<<line<< ": divide flag not returned"<<endl;
            check = 0;
          }
        }
        break;
    }
  }
  if ( !pre )  { // no '?'
    if ( correct_result.decimal != NULL )  {
      if ( strcmp( correct_result.decimal,computed_result.decimal )  != 0 )  {
              int rd=0, rdd=0, epos=-100;
              char *resultdummy=new char[maxstr];
              while (correct_result.decimal[rd] != '\0')
              {
                      if ((rd==epos+1) && correct_result.decimal[rd]!='-')
                         resultdummy[rdd++]='+';

                 resultdummy[rdd] = correct_result.decimal[rd];

                 if (correct_result.decimal[rd] == 'E')
                    epos=rd;
                 rd++;
                 rdd++;
              }
              resultdummy[rdd] = '\0';

              if (strcmp(resultdummy, computed_result.decimal) != 0)
                 {
		   logfile <<((ieeeVector) && !(ieee) ? "Warning " : "Error ") << "Line "<<line<< ": different decimal representation"<< endl;
		   check =0;
		 }
	      delete[] resultdummy;
      }
    } else if ( correct_result.IsNaN( ) ) {
      if( !computed_result.IsNaN( ) ) {
        logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<< line << ": result is not a NaN"<<endl;
        check=0;
      }
    } else {
      if ( computed_result.sizeExp > 0 )  {
        if  ( correct_result.Sign( )  != computed_result.Sign( ) ) {
          if (!correct_result.IsZero( )  ) {
            logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": Different sign"<< endl;
            check =0;
          }
          else if ( signedZero ) { 
            // In this case result is a zero and there is signedzero
            logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") <<"Line "<<line<< ": Different sign"<< endl;
            check =0;
          }
        }
        if ( correct_result.GetExponent( )  != computed_result.GetExponent( ) ) {
          logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") << "Line "<<line<< ": exponent different" << endl;
          check =0;
        }
      }
      if( correct_result.GetMantissa( )  != computed_result.GetMantissa( ) ) {
        logfile<<((ieeeVector) && !(ieee) ? "Warning " : "Error ") << "Line "<<line<< ": mantissa different" << endl;
        check =0;
      }
    }
  } // if !pre
  if ( !check )
   {
     // count errors and warnings
     if ( (!ieeeVector) || (ieee) ) 
       errors++; // total number of errors encountered
     else 
       warnings++;
     PrintError(logfile );
   }
}


/***********************************************************************
* Function:	void SetFPRound()
* Purpose:	sets the library/machine rounding mode
* 		in FP 'control/status' word.
* Return:	void
***********************************************************************/
template <class T>
void UCB<T>::SetFPRound( )
{
  switch ( rounding ) {
    case 'n':
      correct_result.SetFPRound( RM_NEAR );
      break;
    case 'z':
      correct_result.SetFPRound( RM_ZERO );
      break;
    case 'p':
      correct_result.SetFPRound( RM_UP );
      break;
    case 'm':
      correct_result.SetFPRound( RM_DOWN );
      break;
  }
}


#endif
