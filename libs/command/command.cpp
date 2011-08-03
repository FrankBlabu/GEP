/*
 * cmmand.cpp - Base classes for all application commands
 *
 * Frank Cieslok, 20.05.2007
 */

#include "GEPCommand.hpp"

namespace GEP {
  namespace Command {

    //#***********************************************************************
    // CLASS GEP::Command::Command
    //#***********************************************************************

    /*! Constructor */
    Command::Command (const QString& name)
      : _name (name)
    {
    }

    /*! Destructor */
    Command::~Command ()
    {
    }
    
  } // namespace Command
} // namespace GEP

