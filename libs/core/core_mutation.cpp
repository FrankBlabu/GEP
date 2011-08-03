/*
 * core_mutation.cpp - Base class for mutation operators
 *
 * Frank Cieslok, 03.06.2006
 */

#include "GEPCoreMutation.hpp"

namespace GEP {
  namespace Core {
    
    //#***********************************************************************
    // CLASS GEP::Core::MutationOperator
    //#***********************************************************************
    
    /* Constructor */
    MutationOperator::MutationOperator (const QString& description)
      : Operator (description)
    {
    }

    /* Destructor */
    MutationOperator::~MutationOperator ()
    {
    }

  } // namespace Core
} // namespace GEP
