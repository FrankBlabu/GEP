/*
 * GEPMemoryLogger.hpp - Class logging access to dynamically allocated objects
 *
 * Frank Cieslok, 17.06.2006
 */

#ifndef __GEPMemoryLogger_hpp__
#define __GEPMemoryLogger_hpp__

#include "GEPException.hpp"

#include <QtCore/QString>
#include <map>
#include <set>

#ifdef _DEBUG_MEMORY_LOGGER

#define MemoryLoggerCreated(NAME, MARK, OBJECT) \
    GEP::MemoryLogger::get (NAME)->created ((MARK), (OBJECT))
#define MemoryLoggerDeleted(NAME, MARK, OBJECT) \
    GEP::MemoryLogger::get (NAME)->deleted ((MARK), (OBJECT))
#define MemoryLoggerAccessed(NAME, MARK, OBJECT) \
    GEP::MemoryLogger::get (NAME)->accessed ((MARK), (OBJECT))

#else

#define MemoryLoggerCreated(NAME, MARK, OBJECT)
#define MemoryLoggerDeleted(NAME, MARK, OBJECT)
#define MemoryLoggerAccessed(NAME, MARK, OBJECT)

#endif

namespace GEP {

  /*!
   * Class logging access to dynamically allocated objects
   */
  class MemoryLogger
  {
  public:
    class Exception : public GEP::Exception
    {
    public:
      Exception (const QString& message, const QString& mark,
		 const void* object);
    };

  public:
    MemoryLogger (const QString& name);

    inline const QString& getName () const;

    void created (const QString& mark, const void* object);
    void deleted (const QString& mark, const void* object);
    void accessed (const QString& mark, const void* object);
    
    static void deinitialize ();
    static MemoryLogger* get (const QString& name);
    
  private:
    QString _name;

    typedef std::set<const void*> ObjectMap;
    ObjectMap _objects;
    
    typedef std::map<QString, MemoryLogger*> InstanceMap;
    static InstanceMap _instances;
  };

  
  //#*************************************************************************
  // Inline functions
  //#*************************************************************************

  /*! Return name of the logger object */
  inline const QString& MemoryLogger::getName () const
  {
    return _name;
  }
  
} // namespace GEP


#endif
