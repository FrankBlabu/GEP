/*
 * core_ring_individual.cpp - Individual represented by a ring sequence
 *
 * Frank Cieslok, 28.07.2006
 */

//#define _DEBUG

#include "GEPCorePopulation.hpp"
#include "GEPCoreRingIndividual.hpp"

#include <GEPDebug.hpp>
#include <GEPException.hpp>

#include <set>

namespace GEP
{
  namespace Core
  {

    //#***********************************************************************
    // Local functions
    //#***********************************************************************

    /*
     * Swap values
     */
    template <class T>
    void swap (T* object_1, T* object_2)
    {
      T temp = *object_1;
      *object_1 = *object_2;
      *object_2 = temp;
    }


    //#***********************************************************************
    // CLASS GEP::Core::RingIndividual
    //#***********************************************************************

    /* Constructor */
    RingIndividual::RingIndividual ()
      : Individual ()
    {}
    
    /* Copy constructor */
    RingIndividual::RingIndividual (const RingIndividual& toCopy)
      : Individual (toCopy),
        _chromosome (toCopy._chromosome)
    {}
    
    /* Destructor */
    RingIndividual::~RingIndividual ()
    {}
    
    /* Set chromosome */
    void RingIndividual::setChromosome (const Chromosome& chromosome)
    {
      _chromosome = chromosome;
    }


    //#***********************************************************************
    // CLASS GEP::Core::RingPartiallyMatchedCrossoverOperator
    //#***********************************************************************

    /* Constructor */
    RingPartiallyMatchedCrossoverOperator::
    RingPartiallyMatchedCrossoverOperator ()
      : CrossoverOperator ("Partially matched crossover")
    {
    }

    /* Destructor */
    RingPartiallyMatchedCrossoverOperator::
    ~RingPartiallyMatchedCrossoverOperator ()
    {}

    /* Execute crossover operation */
    void RingPartiallyMatchedCrossoverOperator::crossover
    (const Individual* individual_1,
     const Individual* individual_2,
     Population* target) const
    {
      DV ("* Crossover");

      const RingIndividual* ring_1 =
        dynamic_cast<const RingIndividual*> (individual_1);
      if (ring_1 == 0)
        throw InternalException ("Individual is not a ring object");

      const RingIndividual* ring_2 =
        dynamic_cast<const RingIndividual*> (individual_2);
      if (ring_2 == 0)
        throw InternalException ("Individual is not a ring object");

      RingIndividual::Chromosome chromosome_1 = ring_1->getChromosome ();
      RingIndividual::Chromosome chromosome_2 = ring_2->getChromosome ();
      
      if (chromosome_1.size () != chromosome_2.size ())
	throw InternalException ("Chromosomes have different sizes");
      
      if (chromosome_1.size () < 3)
	throw InternalException ("Chromosome size must be at least 3 "
				 "for this operator");
      
      //
      // Select gene cutting locations
      //
      unsigned int size = chromosome_1.size ();
      unsigned int index_1 = _random.getInt (0, size - 1);
      unsigned int index_2 = index_1 + _random.getInt (1, size - 2);
      
      DV ("  index_1=" << index_1 << ", index_2=" << (index_2 % size));
      
      //
      // Cut gene sequence and adapt individuals
      //
      std::vector<RingIndividual::Gene> cut_1 (index_2 - index_1 + 1);
      std::vector<RingIndividual::Gene> cut_2 (index_2 - index_1 + 1);
      
      for (unsigned int i=index_1; i <= index_2; ++i)
        {
          cut_1[(i - index_1) % size] = chromosome_1[i % size];
          cut_2[(i - index_1) % size] = chromosome_2[i % size];
        }
      
      adaptChromosome (index_1, index_2, cut_1, chromosome_2);
      adaptChromosome (index_1, index_2, cut_2, chromosome_1);
      
      RingIndividual* target_1 =
	dynamic_cast<RingIndividual*> (ring_1->clone ());
      target_1->setChromosome (chromosome_1);
      target->addIndividual (target_1);
      
      RingIndividual* target_2 =
	dynamic_cast<RingIndividual*> (ring_2->clone ());
      target_2->setChromosome (chromosome_2);
      target->addIndividual (target_2);
    }


    /*
     * Insert cut sequence into chromosome)
     */
    void RingPartiallyMatchedCrossoverOperator::adaptChromosome
    (unsigned int index_1, unsigned int index_2,
     const std::vector<RingIndividual::Gene>& cut,
     RingIndividual::Chromosome& chromosome) const
    {
      typedef RingIndividual::Gene Gene;
      unsigned int size = chromosome.size ();

      //
      // Step 1: Keep the removed chromosomes for later insertion
      //
      std::vector<Gene> removed (index_2 - index_1 + 1);
      for (unsigned int i=index_1; i <= index_2; ++i)
        removed[(i - index_1) % size] = chromosome[i % size];

      //
      // Step 2: Replace the gene sequence
      //
      std::set<Gene> inserted;
      for (unsigned int i=0; i < cut.size (); ++i)
      {
        chromosome[(index_1 + i) % size] = cut[i];
        inserted.insert (cut[i]);
      }

      //
      // Step 3: Find the genes which have just been removed and are not part
      //         of the inserted sequence. These genes must be placed
      //         outside of the swapped area in the next step.
      //
      std::vector<Gene> replacements;
      for (unsigned int i=0; i < removed.size (); ++i)
        if (inserted.find (removed[i]) == inserted.end ())
          replacements.push_back (removed[i]);

      //
      // Step 4: Find double genes outside of the just swapped area
      //
      for (unsigned int i=0; i < size; ++i)
      {
        bool index_ok = true;
        if (index_1 < index_2 % size)
          index_ok = i < index_1 || i > index_2;
        else
          index_ok = i > index_2 % size && i < index_1;

        //
        // Check, if we are within the unswapped sequence and the gene at
        // this position is part of the just inserted chromosome snippet. If
        // so, replace it with a randomly selected gene from the replacement
        // vector.
        //
        if (index_ok && inserted.find (chromosome[i]) != inserted.end ())
        {
          unsigned int replacement_index =
            _random.getInt (0, replacements.size () - 1);
          chromosome[i] = replacements[replacement_index];

          for ( unsigned int j=replacement_index + 1;
                j < replacements.size (); ++j )
            replacements[j-1] = replacements[j];

          replacements.pop_back ();
        }
      }
    }


    //#***********************************************************************
    // CLASS GEP::Core::RingResortingCrossoverOperator
    //#***********************************************************************

    /* Constructor */
    RingResortingCrossoverOperator::RingResortingCrossoverOperator ()
      : CrossoverOperator ("Resorting crossover")
    {
    }

    /* Destructor */
    RingResortingCrossoverOperator::~RingResortingCrossoverOperator ()
    {}

    /* Execute crossover operation */
    void RingResortingCrossoverOperator::crossover
    (const Individual* individual_1,
     const Individual* individual_2,
     Population* target) const
    {
      DV ("* Crossover");

      const RingIndividual* ring_1 =
        dynamic_cast<const RingIndividual*> (individual_1);
      if (ring_1 == 0)
        throw InternalException ("Individual is not a ring object");

      const RingIndividual* ring_2 =
        dynamic_cast<const RingIndividual*> (individual_2);
      if (ring_2 == 0)
        throw InternalException ("Individual is not a ring object");
      
      RingIndividual::Chromosome chromosome_1 = ring_1->getChromosome ();
      RingIndividual::Chromosome chromosome_2 = ring_2->getChromosome ();

      if (chromosome_1.size () != chromosome_2.size ())
	throw InternalException ("Chromosomes have different sizes");

      if (chromosome_1.size () < 3)
	throw InternalException ("Chromosome size must be at least 3 "
				 "for this operator");

      //
      // Select gene cutting locations
      //
      unsigned int size = chromosome_1.size ();
      unsigned int index_1 = _random.getInt (0, size - 1);
      unsigned int index_2 = index_1 + _random.getInt (1, size - 2);

      DV ("  index_1=" << index_1 << ", index_2=" << (index_2 % size));

      //
      // Cut gene sequence and adapt individuals
      //
      std::vector<RingIndividual::Gene> cut_1 (index_2 - index_1 + 1);
      std::vector<RingIndividual::Gene> cut_2 (index_2 - index_1 + 1);

      for (unsigned int i=index_1; i <= index_2; ++i)
        {
          cut_1[(i - index_1) % size] = chromosome_1[i % size];
          cut_2[(i - index_1) % size] = chromosome_2[i % size];
        }

      insertSequence (cut_1, chromosome_2);
      insertSequence (cut_2, chromosome_1);

      RingIndividual* target_1 =
	dynamic_cast<RingIndividual*> (ring_1->clone ());
      target_1->setChromosome (chromosome_1);
      target->addIndividual (target_1);

      RingIndividual* target_2 =
	dynamic_cast<RingIndividual*> (ring_2->clone ());
      target_2->setChromosome (chromosome_2);
      target->addIndividual (target_2);
    }


    /*
     * Insert cut sequence into chromosome)
     */
    void RingResortingCrossoverOperator::insertSequence
    (const std::vector<RingIndividual::Gene>& sequence,
     RingIndividual::Chromosome& chromosome) const
    {
      typedef RingIndividual::Gene Gene;

      if (sequence.size () == 0)
        throw InternalException ("Attempt to insert zero sequence");

      //
      // Step 1: Remove all genes from the chromosome which are present in
      //         the sequence to insert, too
      //
      std::set<Gene> inserted_genes (sequence.begin (), sequence.end ());

      RingIndividual::Chromosome target;

      unsigned int index = std::numeric_limits<unsigned int>::max ();
      for (unsigned int i=0; i < chromosome.size (); ++i)
      {
        const Gene& gene = chromosome[i];

        //
        // If the gene is part of the inserted sequence, it is skipped
        //
        if (inserted_genes.find (gene) == inserted_genes.end ())
          target.push_back (gene);

        //
        // The original position of the sequence starting gene is the
        // insertion point
        //
        else if (gene == sequence[0])
          index = target.size ();
      }

      std::vector<Gene>::iterator insertion_pos = target.begin ();
      insertion_pos += index;

      target.insert (insertion_pos, sequence.begin (), sequence.end ());

      chromosome = target;
    }


    //#***********************************************************************
    // CLASS GEP::Core::RingUniformMutationOperator
    //#***********************************************************************

    /* Constructor */
    RingUniformMutationOperator::RingUniformMutationOperator
    (double mutation_probability)
        : MutationOperator ("Uniform mutation"),
        _mutation_probability (mutation_probability)
    {
      if (_mutation_probability < 0.0 || _mutation_probability > 1.0)
        throw InternalException ("Mutation probability out of valid range");
    }

    /* Destructor */
    RingUniformMutationOperator::~RingUniformMutationOperator ()
    {}

    /* Set per gene mutation probability */
    void RingUniformMutationOperator::setMutationProbability
    (double mutation_probability)
    {
      if (mutation_probability < 0.0 || mutation_probability > 1.0)
        throw InternalException ("Mutation probability out of valid range");

      _mutation_probability = mutation_probability;
    }

    /* Mutate ring based individual */
    Individual* RingUniformMutationOperator::mutate (const Individual* source)
    const
    {
      const RingIndividual* ring =
        dynamic_cast<const RingIndividual*> (source);
      if (ring == 0)
        throw InternalException ("Individual is not a ring object");

      RingIndividual::Chromosome chromosome = ring->getChromosome ();

      if (chromosome.size () < 2)
        throw InternalException ("Individual size must be at least 2 for "
                                 "this operator");

      //
      // All genes get the chance for mutation...
      //
      unsigned int size = chromosome.size ();
      for (unsigned int index_1=0; index_1 < size; ++index_1)
      {
        //
        // ...with the given mutation probability (usually pm = 1 / size)
        //
	if (_random.getDouble (0.0, 1.0) < _mutation_probability)
        {
          unsigned int index_2 = _random.getInt (0, size - 1);

          while (index_1 == index_2)
	    index_2 = _random.getInt (0, size - 1);

          swap (&chromosome[index_1], &chromosome[index_2]);
        }
      }

      RingIndividual* target =
        dynamic_cast<RingIndividual*> (ring->clone ());
      target->setChromosome (chromosome);
      return target;
    }


  } // namespace Core
} // namespace GEP
