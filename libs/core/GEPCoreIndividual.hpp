/*
 * GEPCoreIndividual.hpp - Basic class for individuals
 *
 * Frank Cieslok, 03.06.2006
 */

#ifndef __GEPCoreIndividual_hpp__
#define __GEPCoreIndividual_hpp__

#include <GEPException.hpp>
#include <limits>

namespace GEP {
  namespace Core {

    class Population;
    
    /*!
     * Base class for all individuals
     */
    class Individual
    {
    public:
      Individual ();
      Individual (const Individual& toCopy);
      virtual ~Individual ();

      virtual Individual* clone () const = 0;

      inline bool getSelected () const;
      void setSelected (bool selected);

      inline double getFitness () const;
      void setFitness (double fitness);

    private:
      bool _selected;
      double _fitness;
    };

    
    /*!
     * Comparator class for fitness based individual sorting
     */
    class IndividualFitnessComparator
    {
    public:
      inline IndividualFitnessComparator () {}

      inline bool operator () (const Individual* individual1,
			       const Individual* individual2) const;
    };

    
    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /*
     * Return if this individual is currently selected
     */
    inline bool Individual::getSelected () const
    {
      return _selected;
    }

    /*
     * Get fitness
     *
     * \return Normalized fitness in interval [0.0, 1.0]
     */
    inline double Individual::getFitness () const
    {
      if (_fitness == std::numeric_limits<double>::min ())
	throw InternalException ("Fitness not computed yet");

      return _fitness;
    }

    /*
     * Comparison operator
     */
    inline bool IndividualFitnessComparator::operator ()
      (const Individual* individual1, const Individual* individual2) const
    {
      return individual1->getFitness () > individual2->getFitness ();
    }
    
  } // namespace Core
} // namespace GEP

#endif
