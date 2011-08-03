/*
 * GEPCoreSelection.hpp - General selection operators
 *
 * Frank Cieslok, 27.07.2006
 */

#ifndef __GEPCoreSelection_hpp__
#define __GEPCoreSelection_hpp__

#include "GEPCoreOperator.hpp"
#include <GEPRandom.hpp>

namespace GEP {
  namespace Core {

    class Population;

    /*
     * Base class for all individual selection operators
     */
    class SelectionOperator : public Operator
    {
    public:
      SelectionOperator (const QString& description);
      virtual ~SelectionOperator ();

      virtual void select (const Population* source,
			   Population* target) const = 0;
    };
    
    /*
     * Best individual selection operator
     */
    class BestSelectionOperator : public SelectionOperator
    {
    public:
      BestSelectionOperator ();
      virtual ~BestSelectionOperator ();

      virtual void select (const Population* source,
			   Population* target) const;

    private:
      Random _random;
    };
    
    /*
     * Roulette based selection operator
     */
    class RouletteSelectionOperator : public SelectionOperator
    {
    public:
      RouletteSelectionOperator ();
      virtual ~RouletteSelectionOperator ();

      virtual void select (const Population* source,
			   Population* target) const;

    private:
      Random _random;
    };
    
  } // namespace Core
} // namespace GEP

#endif
