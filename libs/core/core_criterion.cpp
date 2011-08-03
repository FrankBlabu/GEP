/*
 * core_criterion.hpp - Termination criterions
 *
 * Frank Cieslok, 04.06.2006
 */

#include "GEPCoreCriterion.hpp"
#include "GEPCoreController.hpp"
#include "GEPCoreIndividual.hpp"
#include "GEPCorePopulation.hpp"

#include <limits>
#include <math.h>

namespace GEP {
  namespace Core {

    //#***********************************************************************
    // CLASS GEP::Core::TerminationCriterion
    //#***********************************************************************
    
    /* Constructor */
    TerminationCriterion::TerminationCriterion ()
    {
    }

    /* Destructor */
    TerminationCriterion::~TerminationCriterion ()
    {
    }

    /* Reset criterion class */
    void TerminationCriterion::reset ()
    {
    }
    

    //#***********************************************************************
    // CLASS GEP::Core::FixedStepTerminationCriterion
    //#***********************************************************************
    
    /* Constructor */
    FixedStepTerminationCriterion::FixedStepTerminationCriterion
    (unsigned int steps, unsigned int max_unchanged_steps)
      : _steps                (steps),
	_max_unchanged_steps  (max_unchanged_steps),
	_last_average_fitness (std::numeric_limits<double>::min ()),
	_unchanged_steps      (0)
    {
    }

    /* Destructor */
    FixedStepTerminationCriterion::~FixedStepTerminationCriterion ()
    {
    }

    /* Test if the algorithm should terminate */
    bool FixedStepTerminationCriterion::finished (const Controller* controller)
    {
      //
      // We are finished if the maximum number of steps is exceeded...
      //
      bool finished = controller->getStep () >= _steps;
      
      //
      // ...or the average fitness value did not changed in the last few steps
      //
      if (!finished && _max_unchanged_steps > 0)
      {
	double average_fitness = 0.0;
	unsigned int count = 0;

	for ( Controller::PopulationConstIterator i =
		controller->getPopulationBegin ();
	      i != controller->getPopulationEnd (); ++i )
	{
	  const Population* population = *i;

	  double average_population_fitness = 0.0;
	  unsigned int population_count = 0;
	  
	  for ( Population::IndividualConstIterator j =
		  population->getIndividualBegin ();
		j != population->getIndividualEnd (); ++j )
	  {
	    const Individual* individual = *j;
	    average_population_fitness += individual->getFitness ();
	    ++population_count;
	  }

	  average_fitness += average_population_fitness;
	  ++count;
	}

	average_fitness /= count;
	
	if ( fabs (_last_average_fitness - average_fitness) <
	     std::numeric_limits<double>::epsilon () )
	  ++_unchanged_steps;
	else
	  _unchanged_steps = 0;

	if (_unchanged_steps > _max_unchanged_steps)
	  finished = true;

	_last_average_fitness = average_fitness;
      }

      return finished;
    }
    
  } // namespace Core
} // namespace GEP
