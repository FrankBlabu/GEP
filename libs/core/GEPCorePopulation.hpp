/*
 * GEPCorePopulation.hpp - Population of Individuals
 *
 * Frank Cieslok, 03.06.2006
 */

#ifndef __GEPCorePopulation_hpp__
#define __GEPCorePopulation_hpp__

#include <GEPException.hpp>
#include <vector>

namespace GEP {
  namespace Core {

    class Individual;
    
    /*
     * Population of Individuals
     */
    class Population
    {
    public:
      Population ();
      virtual ~Population ();

      typedef std::vector<Individual*> Individuals;
      typedef Individuals::const_iterator IndividualConstIterator;

      inline IndividualConstIterator getIndividualBegin () const;
      inline IndividualConstIterator getIndividualEnd () const;

      inline unsigned int size () const;
      inline const Individual* operator[] (unsigned int index) const;

      virtual void computeFitness () const = 0;

      void addIndividual (Individual* individual);
      
    private:
      Individuals _individuals;
    };

    /*
     * Creator for a new population object
     */
    class PopulationCreator
    {
    public:
      PopulationCreator ();
      virtual ~PopulationCreator ();

      virtual Population* create (unsigned int size) const = 0;
    };
    

    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Return the begin iterator over this populations individuals */
    inline Population::IndividualConstIterator
    Population::getIndividualBegin () const
    {
      return _individuals.begin ();
    }
    
    /* Return the end iterator over this populations individuals */
    inline Population::IndividualConstIterator
    Population::getIndividualEnd () const
    {
      return _individuals.end ();
    }

    /* Return the size of the population */
    inline unsigned int Population::size () const
    {
      return _individuals.size ();
    }

    /* Return individual at a given index position */
    inline const Individual* Population::operator[] (unsigned int index) const
    {
      if (index >= _individuals.size ())
	throw InternalException ("Population index exceeded");

      return _individuals[index];
    }
    
  } // namespace Core
} // namespace GEP

#endif
