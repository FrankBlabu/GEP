/*
 * core_operator.cpp - Base classes for perators on individuals
 *
 * Frank Cieslok, 03.06.2006
 */

#include "GEPCoreOperator.hpp"

namespace GEP {
  namespace Core {
    
    
    //#***********************************************************************
    // CLASS GEP::Core::Operator
    //#***********************************************************************
    
    /* Constructor */
    Operator::Operator (const QString& description)
      : _description (description)
    {
    }

    /* Destructor */
    Operator::~Operator ()
    {
    }
    
  } // namespace Core
} // namespace GEP
