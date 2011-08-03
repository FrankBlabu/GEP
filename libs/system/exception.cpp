/*
 * exception.cpp - Exception base class
 *
 * Frank Cieslok, 03.06.2006
 */

#include "GEPException.hpp"

using namespace std;

namespace GEP {

  //#*************************************************************************
  // CLASS GEP::Exception
  //#*************************************************************************

  /* Constructor */
  Exception::Exception (const QString& message)
    : _message (message)
  {
  }

  /* Destructor */
  Exception::~Exception ()
  {
  }

  /* Set exception message */
  void Exception::setMessage (const QString& message)
  {
    _message = message;
  }
  
  
  //#*************************************************************************
  // CLASS GEP::InternalException
  //#*************************************************************************

  /* Constructor */
  InternalException::InternalException (const QString& message)
    : Exception (message)
  {
  }

  /* Constructor */
  InternalException::InternalException (const char* function, const char* file,
					unsigned int line,
					const QString& message)
    : Exception ("")
  {
    setMessage (QString ("\n"
			 "Internal exception: %2%\n"
			 "in function %3%\n"
			 "in file %4% line %1%\n\n")
		.arg (line)
		.arg (message, QString (function),QString (file)));
  }
  
  /* Destructor */
  InternalException::~InternalException ()
  {
  }
  
} // namespace GEP
