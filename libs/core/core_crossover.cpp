/*
 * core_crossover.cpp - Base class for crossover operators
 *
 * Frank Cieslok, 03.06.2006
 */

#include "GEPCoreCrossover.hpp"
#include "GEPCoreIndividual.hpp"
#include "GEPCorePopulation.hpp"

#include <GEPException.hpp>
#include <GEPRandom.hpp>

#include <algorithm>


namespace GEP {
  namespace Core {

    
    //#***********************************************************************
    // CLASS GEP::Core::CrossoverOperator
    //#***********************************************************************
    
    /* Constructor */
    CrossoverOperator::CrossoverOperator (const QString& description)
      : Operator (description)
    {
    }

    /* Destructor */
    CrossoverOperator::~CrossoverOperator ()
    {
    }

  } // namespace Core
} // namespace GEP
