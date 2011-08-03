/*
 * core_selection.cpp - General selection operators
 *
 * Frank Cieslok, 27.07.2006
 */

#include "GEPCoreSelection.hpp"
#include "GEPCoreIndividual.hpp"
#include "GEPCorePopulation.hpp"

#include <GEPDebug.hpp>

#include <algorithm>

namespace GEP {
  namespace Core {
    
    //#***********************************************************************
    // CLASS GEP::Core::SelectionOperator
    //#***********************************************************************
    
    /* Constructor */
    SelectionOperator::SelectionOperator (const QString& description)
      : Operator (description)
    {
    }

    /* Destructor */
    SelectionOperator::~SelectionOperator ()
    {
    }

    
    //#***********************************************************************
    // CLASS GEP::Core::BestSelectionOperator
    //#***********************************************************************
    
    /* Constructor */
    BestSelectionOperator::BestSelectionOperator ()
      : SelectionOperator ("Best selection")
    {
    }

    /* Destructor */
    BestSelectionOperator::~BestSelectionOperator ()
    {
    }

    /* Select individuals */
    void BestSelectionOperator::select (const Population* source,
					Population* target) const
    {
      std::vector<const Individual*> individuals;
      for ( Population::IndividualConstIterator i =
	      source->getIndividualBegin ();
	    i != source->getIndividualEnd (); ++i )
	individuals.push_back (*i);

      std::sort (individuals.begin (), individuals.end (),
		 IndividualFitnessComparator ());

      for (unsigned int i=0; i < source->size (); ++i)
	target->addIndividual
	  (individuals[_random.getInt (0, individuals.size () / 2)]->clone ());
    }

    
    //#***********************************************************************
    // CLASS GEP::Core::RouletteSelectionOperator
    //#***********************************************************************
    
    /* Constructor */
    RouletteSelectionOperator::RouletteSelectionOperator ()
      : SelectionOperator ("Roulette selection")
    {
    }

    /* Destructor */
    RouletteSelectionOperator::~RouletteSelectionOperator ()
    {
    }

    /* Select individuals */
    void RouletteSelectionOperator::select (const Population* source,
					    Population* target) const
    {
      //
      // Step 1: Create table with fitness sum
      //
      typedef std::pair<double, const Individual*> Fitness;
      std::vector<Fitness> fitness;

      double sum = 0.0;
      for ( Population::IndividualConstIterator i =
	      source->getIndividualBegin ();
	    i != source->getIndividualEnd (); ++i )
      {
	const Individual* individual = *i;
	sum += individual->getFitness ();
	fitness.push_back (Fitness (sum, individual));
      }

      //
      // Step 2: Select individuals via roulette selection
      //
      for (unsigned i=0; i < source->size (); ++i)
      {
	double n = _random.getDouble (0.0, sum);

	const Individual* selected = 0;
	for (unsigned int j=0; j < fitness.size () && selected == 0; ++j)
	{
	  if (fitness[j].first > n)
	    selected = fitness[j].second;
	}

	if (selected == 0 && source->size () > 0)
	  selected = (*source)[source->size () - 1];

	target->addIndividual (selected->clone ());
      }
    }

  } // namespace Core
} // namespace GEP
