/*
 * GEPException.hpp - Exception base class
 *
 * Frank Cieslok, 03.06.2006
 */

#ifndef __GEPException_hpp__
#define __GEPException_hpp__

#include <QtCore/QString>

namespace GEP {

  /*
   * Base class for all GEP exception classes
   */
  class Exception
  {
  public:
    Exception (const QString& message);
    virtual ~Exception ();

    inline const QString& getMessage () const;
    void setMessage (const QString& message);
    
  private:
    QString _message;
  };

  
#define CALLER_INFO __func__, __FILE__, __LINE__
  
  /*
   * Exception class for internal errors
   */
  class InternalException : public Exception
  {
  public:
    InternalException (const QString& message);
    InternalException (const char* function, const char* file,
		       unsigned int line, const QString& message);
    virtual ~InternalException ();
  };
  

  //#*************************************************************************
  // Inline functions
  //#*************************************************************************
  
  /*
   * Return the exceptions error message
   */
  inline const QString& Exception::getMessage () const
  {
    return _message;
  }
  
} // namespace GEP

#endif
