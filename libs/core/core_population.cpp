/*
 * core_population.cpp - Population of Individuals
 *
 * Frank Cieslok, 03.06.2006
 */

#include "GEPCorePopulation.hpp"
#include "GEPCoreIndividual.hpp"

namespace GEP {
  namespace Core {

    //#***********************************************************************
    // CLASS GEP::Core::Population
    //#***********************************************************************
    
    /* Constructor */
    Population::Population ()
    {
    }

    /* Destructor */
    Population::~Population ()
    {
      for (unsigned int i=0; i < _individuals.size (); ++i)
	delete _individuals[i];

      _individuals.clear ();
    }

    /*
     * Add single individual to this population
     *
     * \param individual Individual to add. The individual must be unique and
     *                   is then owned by the population. It will be destroyed
     *                   upon the populations deletion.
     */
    void Population::addIndividual (Individual* individual)
    {
      for (unsigned int i=0; i < _individuals.size (); ++i)
	if (_individuals[i] == individual)
	  throw InternalException ("Duplicate individual added");

      _individuals.push_back (individual);
    }
    

    //#***********************************************************************
    // CLASS GEP::Core::PopulationCreator
    //#***********************************************************************

    /* Constructor */
    PopulationCreator::PopulationCreator ()
    {
    }
    
    /* Destructor */
    PopulationCreator::~PopulationCreator ()
    {
    }
    

  } // namespace Core
} // namespace GEP
