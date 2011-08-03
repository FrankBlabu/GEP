/*
 * core_individual.cpp - Basic class for individuals
 *
 * Frank Cieslok, 03.06.2006
 */

#include "GEPCoreIndividual.hpp"

namespace GEP {
  namespace Core {

    //#************************************************************************
    // CLASS GEP::Core::Individual
    //#************************************************************************
    
    /* Constructor */
    Individual::Individual ()
      : _selected (false),
	_fitness (std::numeric_limits<double>::min ())
    {
    }
    
    /* Copy constructor */
    Individual::Individual (const Individual& toCopy)
      : _selected (toCopy._selected),
	_fitness (toCopy._fitness)
    {
    }

    /* Destructor */
    Individual::~Individual ()
    {
    }

    /*
     * Select/deselect individual
     */
    void Individual::setSelected (bool selected)
    {
      _selected = selected;
    }

    /* 
     * Set fitness value 
     *
     * \param fitness Individuals normalized fitness in interval [0.0, 1.0]
     */
    void Individual::setFitness (double fitness)
    {
      if (fitness < 0.0)
	throw InternalException ("Fitness is too small, normalized value "
				 "expected");
      if (fitness > 1.0)
	throw InternalException ("Fitness is too large, normalized value "
				 "expected");
      
      _fitness = fitness;
    }
    
  } // namespace Core
} // namespace GEP
