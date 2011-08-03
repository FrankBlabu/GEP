/*
 * test_selection_operators.cpp - Tests for the selection operators
 *
 * Frank Cieslok, 24.06.2007
 */

#define _DEBUG

#include "GEPCoreIndividual.hpp"
#include "GEPCorePopulation.hpp"
#include "GEPCoreSelection.hpp"

#include <GEPDebug.hpp>
#include <GEPException.hpp>
#include <GEPRandom.hpp>

#include <iostream>
#include <set>

//
// Number of individuals per test population
//
static const unsigned int NUMBER_OF_INDIVIDUALS=30;

namespace GEP {
  namespace Core {

    //#***********************************************************************
    // CLASS GEP::Core::TestIndividual
    //#***********************************************************************

    class TestIndividual : public GEP::Core::Individual
    {
    public:
      TestIndividual (unsigned int id, double fitness);
      TestIndividual (const TestIndividual& toCopy);
      virtual ~TestIndividual ();

      virtual Individual* clone () const;
      
      inline unsigned int getId () const { return _id; }

    private:
      unsigned int _id;
      double _fitness;
    };

    TestIndividual::TestIndividual (unsigned int id, double fitness)
      : Individual (),
	_id (id)
    {
      setFitness (fitness);
    }
    
    TestIndividual::TestIndividual (const TestIndividual& toCopy)
      : Individual (toCopy),
	_id (toCopy._id)
    {
    }
    
    TestIndividual::~TestIndividual ()
    {
    }

    Individual* TestIndividual::clone () const
    {
      return new TestIndividual (*this);
    }


    //#***********************************************************************
    // CLASS GEP::Core::TestPopulation
    //#***********************************************************************

    class TestPopulation : public GEP::Core::Population
    {
    public:
      TestPopulation (unsigned int size);
      virtual ~TestPopulation ();

      virtual void computeFitness () const;

    private:
      Random _random;
    };

    TestPopulation::TestPopulation (unsigned int size)
      : Population ()
    {
      for (unsigned int i=0; i < size; ++i)
	addIndividual (new TestIndividual (i, _random.getDouble (0.0, 1.0)));
    }
    
    TestPopulation::~TestPopulation ()
    {
    }

    void TestPopulation::computeFitness () const
    {
    }
    
    
    //#***********************************************************************
    // Test functions
    //#***********************************************************************

    /*
     * Print population
     */
    static void printPopulation (const QString& title,
				 const TestPopulation& population)
    {
      std::cout << qPrintable (title) << std::endl;
      std::cout << qPrintable (QString (title.size (), '-')) << std::endl;
      
      std::vector<const TestIndividual*> individuals;

      for ( Population::IndividualConstIterator i =
	      population.getIndividualBegin ();
	    i != population.getIndividualEnd (); ++i )
	individuals.push_back (dynamic_cast<const TestIndividual*> (*i));;

      std::sort (individuals.begin (), individuals.end (),
		 IndividualFitnessComparator ());

      double average_fitness = 0.0;
 
      for (unsigned int i=0; i < individuals.size (); ++i)
	{
	  const TestIndividual* individual = individuals[i];
	  std::cout << qPrintable (QString::number (individual->getId ()))
		    << " "
		    << qPrintable (QString::number (individual->getFitness (),
						    'f', 2))
		    << std::endl;

	  average_fitness += individual->getFitness ();
	}

      std::cout << std::endl;

      std::cout << "Average fitness: " 
		<< qPrintable (QString::number (average_fitness, 'f', 2))
		<< std::endl;
      std::cout << std::endl;
    }

    /*
     * Test selection operators
     */
    static void testSelection () throw (InternalException)
    {
      std::cout << "*** Testing selection" << std::endl;
      std::cout << std::endl;

      //
      // Test 1: BestSelection
      //
      {
	std::cout << "* Best selection" << std::endl;
	std::cout << std::endl;

	TestPopulation source (NUMBER_OF_INDIVIDUALS);
	TestPopulation target (0);
	
	BestSelectionOperator best;
	best.select (&source, &target);
	
	printPopulation ("Source", source);
	printPopulation ("Target", target);
      }

      //
      // Test2: RouletteWheelSelection
      //
      {
	std::cout << "* Roulette wheel selection" << std::endl;
	std::cout << std::endl;

	TestPopulation source (NUMBER_OF_INDIVIDUALS);
	TestPopulation target (0);
	
	RouletteSelectionOperator roulette;
	roulette.select (&source, &target);
	
	printPopulation ("Source", source);
	printPopulation ("Target", target);
      }
    }
    
  } // namespace Core
} // namespace GEP


int main (int /*argc*/, char** /*argv*/)
{
  bool ok = true;

  try
  {
    GEP::Core::testSelection ();
  }
  catch (const GEP::InternalException& exception)
  {
    std::cerr << "ERROR: " << qPrintable (exception.getMessage ()) 
	      << std::endl;
    ok = false;
  }
  
  return ok ? 0 : 1;
}
