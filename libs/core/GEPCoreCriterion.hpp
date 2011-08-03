/*
 * GEPCoreCriterion.hpp - Termination criterions
 *
 * Frank Cieslok, 04.06.2006
 */

#ifndef __GEPCoreCriterion_hpp__
#define __GEPCoreCriterion_hpp__

namespace GEP {
  namespace Core {

    class Controller;
    
    /*
     * Base class for all termination criterions
     */
    class TerminationCriterion
    {
    public:
      TerminationCriterion ();
      virtual ~TerminationCriterion ();

      virtual void reset ();
      virtual bool finished (const Controller* controller) = 0;
    };
    
    /*
     * Termination after a fixed amount of steps
     */
    class FixedStepTerminationCriterion : public TerminationCriterion
    {
    public:
      FixedStepTerminationCriterion (unsigned int steps,
				     unsigned int max_unchanged_steps);
      virtual ~FixedStepTerminationCriterion ();

      virtual bool finished (const Controller* controller);

    private:
      unsigned int _steps;
      unsigned int _max_unchanged_steps;
      
      double _last_average_fitness;
      unsigned int _unchanged_steps;
    };
    
  } // namespace Core
} // namespace GEP

#endif
