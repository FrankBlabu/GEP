/*
 * test_ring_operators.cpp - Tests for the ring individual operators
 *
 * Frank Cieslok, 29.07.2006
 */

#define _DEBUG

#include "GEPCoreRingIndividual.hpp"
#include "GEPCorePopulation.hpp"

#include <GEPDebug.hpp>
#include <GEPException.hpp>
#include <GEPRandom.hpp>

#include <iostream>
#include <set>

//
// Size of the test chromosome
//
#define CHROMOSOME_SIZE 10

namespace GEP {
  namespace Core {

    //#***********************************************************************
    // CLASS GEP::Core::TestIndividual
    //#***********************************************************************

    class TestIndividual : public GEP::Core::RingIndividual
    {
    public:
      TestIndividual ();
      TestIndividual (const TestIndividual& toCopy);
      virtual ~TestIndividual ();

      virtual Individual* clone () const;
    };

    TestIndividual::TestIndividual ()
      : RingIndividual ()
    {
    }
    
    TestIndividual::TestIndividual (const TestIndividual& toCopy)
      : RingIndividual (toCopy)
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
      TestPopulation ();
      virtual ~TestPopulation ();

      virtual void computeFitness () const;
    };

    TestPopulation::TestPopulation ()
      : Population ()
    {
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
     * Execute crossover
     */
    typedef std::pair<TestIndividual*, TestIndividual*>
    CrossoverResult;
    
    static CrossoverResult executeCrossover
    (const CrossoverOperator& crossover_operator,
     const TestIndividual* source_1,
     const TestIndividual* source_2)
      throw (InternalException)
    {
      CrossoverResult result;
      
      TestPopulation target;
      crossover_operator.crossover (source_1, source_2, &target);

      Population::IndividualConstIterator pos = target.getIndividualBegin ();
      if (pos != target.getIndividualEnd ())
	result.first = dynamic_cast<TestIndividual*> ((*pos++)->clone ());
      else
	throw InternalException ("No individuals generated, first missing");

      if (pos != target.getIndividualEnd ())
	result.second = dynamic_cast<TestIndividual*> ((*pos++)->clone ());
      else
	throw InternalException ("No individuals generated, second missing");

      if (pos != target.getIndividualEnd ())
	throw InternalException ("More than two individuals generated");

      return result;
    }

    /*
     * Set chromosomes of an individual to random values representing a
     * correct TSP path
     */
    static void setRandomChromosome (RingIndividual* individual)
    {
      std::vector<RingIndividual::Gene> genes;
      for (unsigned int i=0; i < CHROMOSOME_SIZE; ++i)
	genes.push_back (i);

      RingIndividual::Chromosome chromosome (CHROMOSOME_SIZE);
      for (unsigned int i=0; i < CHROMOSOME_SIZE; ++i)
      {
	unsigned int index = Random::getInt (0, genes.size () - 1);
	chromosome[i] = genes[index];

	for (unsigned int j=index + 1; j < genes.size (); ++j)
	  genes[j - 1] = genes[j];
	genes.pop_back ();
      }

      individual->setChromosome (chromosome);
    }

    /*
     * Check if a chromosome corresponds to a correct TSP path
     */
    static void checkChromosome (const RingIndividual& ring)
    {
      std::set<RingIndividual::Gene> genes;
      for (unsigned int i=0; i < CHROMOSOME_SIZE; ++i)
	genes.insert (i);

      const RingIndividual::Chromosome& chromosome = ring.getChromosome ();
      for (unsigned int i=0; i < chromosome.size (); ++i)
      {
	RingIndividual::Gene gene = chromosome[i];
	std::set<RingIndividual::Gene>::const_iterator pos = genes.find (gene);
	if (pos == genes.end ())
	  throw InternalException ("Gene not found or doubled, individual=" +
				   Debug::toString (chromosome));

	genes.erase (pos);
      }
    }
    
    /*
     * Test crossover operator
     */
    static void testCrossoverOperator
    (const CrossoverOperator& crossover_operator) throw (InternalException)
    {
      DV ("* Testing crossover: " << crossover_operator.getDescription ());
      
      for (unsigned int i=0; i < 100; ++i)
      {
	DV ("- Run " << i);
	
	TestIndividual ring1;
	TestIndividual ring2;

	setRandomChromosome (&ring1);
	setRandomChromosome (&ring2);

	DV ("  ring_1=" << Debug::toString (ring1.getChromosome ()));
	DV ("  ring_2=" << Debug::toString (ring2.getChromosome ()));
	
	for (unsigned int j=0; j < 100; ++j)
	{
	  DV ("  - Step " << j);
	  
	  CrossoverResult result =
	    executeCrossover (crossover_operator, &ring1, &ring2);
	  std::auto_ptr<TestIndividual> result_1 (result.first);
	  std::auto_ptr<TestIndividual> result_2 (result.second);
	  
	  checkChromosome (*result_1.get ());
	  checkChromosome (*result_2.get ());

	  ring1.setChromosome (result_1->getChromosome ());
	  ring2.setChromosome (result_2->getChromosome ());

	  DV ("  ring_1=" << Debug::toString (ring1.getChromosome ()));
	  DV ("  ring_2=" << Debug::toString (ring2.getChromosome ()));
	}
      }
    }

    /*
     * Test crossover operators
     */
    static void testCrossover () throw (InternalException)
    {
      DV ("* Testing crossover");
      
      testCrossoverOperator (RingPartiallyMatchedCrossoverOperator (1.0));
      testCrossoverOperator (RingResortingCrossoverOperator (1.0));
    }
    

    /*
     * Test mutation operator
     */
    static void testMutation () throw (InternalException)
    {
      DV ("* Testing mutation");

      RingUniformMutationOperator mutation_operator (1.0 / CHROMOSOME_SIZE);
      
      for (unsigned int i=0; i < 100; ++i)
      {
	DV ("- Run " << i);
	
	TestIndividual ring;
	setRandomChromosome (&ring);

	DV ("  ring=" << Debug::toString (ring.getChromosome ()));
	
	for (unsigned int j=0; j < 100; ++j)
	{
	  DV ("  - Step " << j);
	  
	  std::auto_ptr<TestIndividual> result
	    (dynamic_cast<TestIndividual*> (mutation_operator.mutate (&ring)));
	  
	  checkChromosome (*result.get ());
	  ring.setChromosome (result->getChromosome ());

	  DV ("  ring=" << Debug::toString (ring.getChromosome ()));
	}
      }
    }
    
  } // namespace Core
} // namespace GEP


int main (int /*argc*/, char** /*argv*/)
{
  bool ok = true;

  try
  {
    GEP::Core::testCrossover ();
    GEP::Core::testMutation ();
  }
  catch (const GEP::InternalException& exception)
  {
    std::cerr << "ERROR: " << exception.getMessage () << std::endl;
    ok = false;
  }
  
  return ok ? 0 : 1;
}
