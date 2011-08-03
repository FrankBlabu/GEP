/*
 * GEPCoreMutation.hpp - Base class for mutation operators
 *
 * Frank Cieslok, 03.06.2006
 */

#ifndef __GEPCoreMutation_hpp__
#define __GEPCoreMutation_hpp__

#include "GEPCoreOperator.hpp"

#include <QtCore/QString>

namespace GEP {
  namespace Core {

    class Individual;
    
    /*
     * Base class for all mutation operators
     */
    class MutationOperator : public Operator
    {
    public:
      MutationOperator (const QString& description);
      virtual ~MutationOperator ();

      virtual Individual* mutate (const Individual* source) const = 0;
    };
    
  } // namespace Core
} // namespace GEP

#endif
