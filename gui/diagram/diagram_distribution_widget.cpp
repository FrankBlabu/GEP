/*
 * diagram_distribution_widget.cpp - Diagram widget displaying the fitness
 *                                   distribution of a population
 *
 * Frank Cieslok, 04.08.2006
 */

#include "GEPDiagramDistributionWidget.hpp"

#include <GEPCoreController.hpp>
#include <GEPCoreIndividual.hpp>
#include <GEPCorePopulation.hpp>
#include <GEPException.hpp>
#include <GEPDebug.hpp>

#include <math.h>

//
// Default number of steps in the diagram
//
#define DEFAULT_NUMBER_OF_STEPS 50

namespace GEP {
  namespace Diagram {

    //#***********************************************************************
    // CLASS GEP::Display::DistributionDiagramWidget
    //#***********************************************************************

    /*! Constructor */
    DistributionDiagramWidget::DistributionDiagramWidget
    (Core::Controller* controller, QWidget* parent)
      : DiagramWidget (1, parent),
	_number_of_steps (DEFAULT_NUMBER_OF_STEPS)
    {
      setLineColor (0, Qt::red);

      connect (controller,
	       SIGNAL (signalPopulationsChanged (const Core::Controller*)),
	       SLOT (slotPopulationsChanged (const Core::Controller*)));
    }

    /*! Destructor */
    DistributionDiagramWidget::~DistributionDiagramWidget ()
    {
    }

    /*! Set the number of steps in this diagram */
    void DistributionDiagramWidget::setNumberOfSteps (unsigned int steps)
    {
      _number_of_steps = steps;
    }

    /*! Clear collected data */
    void DistributionDiagramWidget::slotClear ()
    {
      _data.clear ();
    }

    /*! Fetch data to display */
    const std::vector<DiagramWidget::Data_t>&
    DistributionDiagramWidget::getData () const
    {
      return _data;
    }

    /* Update widget data */
    void DistributionDiagramWidget::update (const Core::Controller* controller)
    {
      _data.clear ();
      
      for (unsigned int i=0; i < _number_of_steps; ++i)
      {
	DiagramWidget::Data_t value (1);
	value[0] = 0.0;
	_data.push_back (value);
      }
      
      for ( Core::Controller::PopulationConstIterator i =
	      controller->getPopulationBegin ();
	    i != controller->getPopulationEnd (); ++i ) 
      {
	const Core::Population* population = *i;
	
	for ( Core::Population::IndividualConstIterator j =
		population->getIndividualBegin ();
	      j != population->getIndividualEnd (); ++j )
	{
	  const Core::Individual* individual = *j;
	  
	  unsigned int index = static_cast<unsigned int>
	    (round (individual->getFitness () * _number_of_steps));
	  
	  index = std::min (index, _number_of_steps - 1);
	  _data[index][0] += 1.0;
	}
      }

      invalidateCachedData ();
      DiagramWidget::update ();
    }

    /* Called if the controllers populations changed */
    void DistributionDiagramWidget::slotPopulationsChanged 
    (const Core::Controller* controller)
    {
      update (controller);
    }

  } // namespace Diagram
} // namespace GEP
