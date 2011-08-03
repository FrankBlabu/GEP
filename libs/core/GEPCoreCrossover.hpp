/*
 * GEPCoreCrossover.hpp - Base class for crossover operators
 *
 * Frank Cieslok, 03.06.2006
 */

#ifndef __GEPCoreCrossover_hpp__
#define __GEPCoreCrossover_hpp__

#include "GEPCoreOperator.hpp"

#include <QtCore/QString>

namespace GEP {
  namespace Core {

    class Individual;
    class Population;
    
    /*
     * Base class for all crossover operators
     */
    class CrossoverOperator : public Operator
    {
    public:
      CrossoverOperator (const QString& description);
      virtual ~CrossoverOperator ();

      virtual void crossover (const Individual* individual1,
			      const Individual* individual2,
			      Population* target) const = 0;
    };

  } // namespace Core
} // namespace GEP

#endif
