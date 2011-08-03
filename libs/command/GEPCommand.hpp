/*
 * GEPCommand.hpp - Base classes for all application commands
 *
 * Frank Cieslok, 20.05.2007
 */

#ifndef __GEPCommand_hpp__
#define __GEPCommand_hpp__

#include <QtCore/QString>

namespace GEP {
  namespace Command {

    /*
     * Base class for all application commands
     */
    class Command
    {
    public:
      Command (const QString& name);
      virtual ~Command ();

      inline const QString& getName () const;

      virtual bool execute (QString* error) = 0;

    private:
      QString _name;
    };

    
    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Get command name */
    inline const QString& Command::getName () const
    {
      return _name;
    }
    
  } // namespace Command
} // namespace GEP

#endif
