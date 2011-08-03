/*
 * GEPTraceGuard.hpp - Wrapper for backtrace catching
 *
 * Frank Cieslok, 20.06.2006
 */

#ifndef __GEPTraceGuard_hpp__

#include <QtCore/QString>
#include <sys/types.h>

namespace GEP {

  class Exception;
  
  /*
   * Exception processing
   */
  class TraceGuard
  {
  public:
    TraceGuard (const QString& application_name);

    enum ErrorMode_t { NONE, BT, GDB, DDD };
    inline ErrorMode_t getErrorMode () const;
    void setErrorMode (ErrorMode_t mode);
    
    void error (const Exception& exception);

  private:
    void printBacktrace ();
    void printError (const QString& text);
    void controlGDB (pid_t gdb_pid, int gdb_stdin, int gdb_stdout,
		     int gdb_stderr);

  private:
    QString _application_name;
    pid_t _main_pid;
    ErrorMode_t _error_mode;
  };


  //#*************************************************************************
  // Inline functions
  //#*************************************************************************

  /* Return current error mode */
  inline TraceGuard::ErrorMode_t TraceGuard::getErrorMode () const
  {
    return _error_mode;
  }
  
} // namespace GEP

#endif

