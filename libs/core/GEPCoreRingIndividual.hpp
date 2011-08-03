/*
 * GEPCoreRingIndividual.hpp - Individual represented by a ring sequence
 *
 * Frank Cieslok, 28.07.2006
 */

#ifndef __GEPCoreRingIndividual_hpp__
#define __GEPCoreRingIndividual_hpp__

#include "GEPCoreCrossover.hpp"
#include "GEPCoreIndividual.hpp"
#include "GEPCoreMutation.hpp"

#include <GEPRandom.hpp>
#include <vector>

namespace GEP {
  namespace Core {

    /*!
     * Individual represented by a ring sequence
     */
    class RingIndividual : public GEP::Core::Individual
    {
    public:
      RingIndividual ();
      RingIndividual (const RingIndividual& toCopy);
      virtual ~RingIndividual ();

      virtual Individual* clone () const = 0;

      typedef unsigned int Gene;
      typedef std::vector<Gene> Chromosome;

      inline const Chromosome& getChromosome () const;
      void setChromosome (const Chromosome& chromosome);
      
    private:
      Chromosome _chromosome;
    };

    /*
     * Partially matched crossover operator for ring based individuals
     */
    class RingPartiallyMatchedCrossoverOperator : public CrossoverOperator
    {
    public:
      RingPartiallyMatchedCrossoverOperator ();
      virtual ~RingPartiallyMatchedCrossoverOperator ();

      virtual void crossover (const Individual* individual1,
			      const Individual* individual2,
			      Population* target) const;

    private:
      void adaptChromosome (unsigned int index_1, unsigned int index_2,
			    const std::vector<RingIndividual::Gene>& cut,
			    RingIndividual::Chromosome& chromosome) const;

    private:
      Random _random;
    };
    
    /*
     * Partially matched crossover operator for ring based individuals
     */
    class RingResortingCrossoverOperator : public CrossoverOperator
    {
    public:
      RingResortingCrossoverOperator ();
      virtual ~RingResortingCrossoverOperator ();

      virtual void crossover (const Individual* individual1,
			      const Individual* individual2,
			      Population* target) const;

    private:
      void insertSequence (const std::vector<RingIndividual::Gene>& sequence,
			   RingIndividual::Chromosome& chromosome) const;

    private:
      Random _random;
    };

    /*
     * Mutation operator for ring based individuals
     */
    class RingUniformMutationOperator : public MutationOperator
    {
    public:
      RingUniformMutationOperator (double mutation_probability);
      virtual ~RingUniformMutationOperator ();

      inline double getMutationProbability () const;
      void setMutationProbability (double mutation_probability);
      
      virtual Individual* mutate (const Individual* source) const;

    private:
      double _mutation_probability;
      Random _random;
    };
    

    
    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Return the individuals genes */
    inline const RingIndividual::Chromosome& RingIndividual::getChromosome ()
      const
    {
      return _chromosome;
    }

    /* Return the mutation probability */
    inline double RingUniformMutationOperator::getMutationProbability () const
    {
      return _mutation_probability;
    }
    
  } // namespace Core
} // namespace GEP

#endif

