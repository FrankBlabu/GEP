/*
 * GEPDiagramDistributionWidget.hpp - Diagram widget displaying the fitness
 *                                    distribution of a population
 *
 * Frank Cieslok, 04.08.2006
 */

#ifndef __GEPDiagramDistributionWidget_hpp__
#define __GEPDiagramDistributionWidget_hpp__

#include "GEPDiagramWidget.hpp"

namespace GEP {

  namespace Core {
    class Controller;
  }
  
  namespace Diagram {

    /*!
     * Diagram widget displaying the fitness distribution of a population
     */
    class DistributionDiagramWidget : public GEP::Diagram::DiagramWidget
    {
      Q_OBJECT
      
    public:
      DistributionDiagramWidget (Core::Controller* controller,
				 QWidget* parent);
      virtual ~DistributionDiagramWidget ();

      inline unsigned int getNumberOfSteps () const;
      void setNumberOfSteps (unsigned int steps);
      
      void update (const Core::Controller* controller);
      
    public slots:
      virtual void slotClear ();

    protected:
      virtual const std::vector<Data_t>& getData () const;

    private slots:
      void slotPopulationsChanged (const Core::Controller* controller);

    private:
      std::vector<Data_t> _data;
      unsigned int _number_of_steps;
    };


    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Return the number of steps in this diagram */
    inline unsigned int DistributionDiagramWidget::getNumberOfSteps () const
    {
      return _number_of_steps;
    }
    
  } // namespace Diagram
} // namespace GEP

#endif
