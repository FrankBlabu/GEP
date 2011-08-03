/*
 * GEPCoreOperator.hpp - Base classes for operators on individuals
 *
 * Frank Cieslok, 03.06.2006
 */

#ifndef __GEPCoreOperator_hpp__
#define __GEPCoreOperator_hpp__

#include <QtCore/QString>

namespace GEP {
  namespace Core {

    /*
     * Base class for all individual based operator classes
     */
    class Operator
    {
    public:
      Operator (const QString& description);
      virtual ~Operator ();

      inline const QString& getDescription () const;
      
    private:
      QString _description;
    };

    
    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Get operators description */
    inline const QString& Operator::getDescription () const
    {
      return _description;
    }
    
  } // namespace Core
} // namespace GEP

#endif
