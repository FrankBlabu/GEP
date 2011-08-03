/*
 * memory_logger.cpp - Class logging access to dynamically allocated objects
 *
 * Frank Cieslok, 17.06.2006
 */

#define _DEBUG

#include "GEPMemoryLogger.hpp"
#include "GEPDebug.hpp"

namespace GEP {

  //#*************************************************************************
  // CLASS GEP::MemoryLogger::Exception
  //#*************************************************************************

  /*! Constructor*/
  MemoryLogger::Exception::Exception (const QString& message,
				      const QString& mark,
				      const void* object)
    : GEP::Exception ("")
  {
    setMessage (QString ("%2 at '%3': %1")
		.arg (reinterpret_cast<unsigned long> (object), 16)
		.arg (message, mark));
  }

  
  //#*************************************************************************
  // CLASS GEP::MemoryLogger
  //#*************************************************************************

  /*! Map of existing loggers [STATIC] */
  MemoryLogger::InstanceMap MemoryLogger::_instances;
  
  /*! Constructor */
  MemoryLogger::MemoryLogger (const QString& name)
    : _name (name)
  {
  }

  /*! Log object creation */
  void MemoryLogger::created (const QString& mark, const void* object)
  {
    DV ("Created (" << qPrintable (mark) << "): " << object);
    
    if (_objects.find (object) != _objects.end ())
      throw Exception ("Create: Object already exists", mark, object);

    _objects.insert (object);
  }

  /*! Log object deletion */
  void MemoryLogger::deleted (const QString& mark, const void* object)
  {
    DV ("Deleted (" << qPrintable (mark) << "): " << object);

    ObjectMap::iterator pos = _objects.find (object);
    if (pos == _objects.end ())
      throw Exception ("Delete: Object does not exist", mark, object);

    _objects.erase (pos);
  }

  /*! Log object access */
  void MemoryLogger::accessed (const QString& mark, const void* object)
  {
    DV ("Accessed (" << qPrintable (mark) << "): " << object);
    
    if (_objects.find (object) == _objects.end ())
      throw Exception ("Access: Object does not exist", mark, object);
  }
  
  /*!
   * Get memory logger class. If the object is not exiting yet it will be
   * created [STATIC]
   *
   * \param name Name of the logger class
   */
  MemoryLogger* MemoryLogger::get (const QString& name)
  {
    MemoryLogger* logger = 0;
    
    InstanceMap::const_iterator pos = _instances.find (name);
    if (pos == _instances.end ())
    {
      logger = new MemoryLogger (name);
      _instances.insert (std::make_pair (name, logger));
    }
    else
      logger = pos->second;

    return logger;
  }

  /*!
   * Destroy all existing logger objects [STATIC]
   */
  void MemoryLogger::deinitialize ()
  {
    for ( InstanceMap::iterator i = _instances.begin ();
	  i != _instances.end (); ++i )
      delete i->second;

    _instances.clear ();
  }
  
} // namespace GEP
