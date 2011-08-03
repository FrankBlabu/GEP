/*
 * diagram_fitness_widget.cpp - Widgets displaying fitness values
 *
 * Frank Cieslok, 15.07.2006
 */

#include "GEPDiagramFitnessWidget.hpp"

#include <GEPCoreController.hpp>
#include <GEPCoreIndividual.hpp>
#include <GEPCorePopulation.hpp>
#include <GEPDebug.hpp>

#include <limits>

namespace GEP {
  namespace Diagram {

    //#***********************************************************************
    // CLASS GEP::Diagram::FitnessDiagramWidget
    //#***********************************************************************

    /*! Constructor */
    FitnessDiagramWidget::FitnessDiagramWidget
    (Core::Controller* controller, QWidget* parent)
      : DataCollectingDiagramWidget (3, parent)
    {
      setLineColor (0, Qt::red);
      setLineColor (1, Qt::green);
      setLineColor (2, Qt::yellow);

      connect (controller,
	       SIGNAL (signalPopulationsChanged (const Core::Controller*)),
	       SLOT (slotPopulationsChanged (const Core::Controller*)));
    }

    /*! Destructor */
    FitnessDiagramWidget::~FitnessDiagramWidget ()
    {
    }

    /* Slot called if the current controller step changed */
    void FitnessDiagramWidget::addStep (const Core::Controller* controller)
    {
      double minimum_fitness = std::numeric_limits<double>::max ();
      double maximum_fitness = std::numeric_limits<double>::min ();
      double average_fitness = 0;

      unsigned int count = 0;
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
	  double fitness = individual->getFitness ();

	  average_fitness += fitness;
	  
	  if (fitness < minimum_fitness)
	    minimum_fitness = fitness;
	  if (fitness > maximum_fitness)
	    maximum_fitness = fitness;
	  
	  ++count;
	}
      }

      if (count == 0)
      {
	minimum_fitness = 0;
	maximum_fitness = 0;
      }
      else
	average_fitness /= count;

      DiagramWidget::Data_t data (3);
      data[0] = minimum_fitness;
      data[1] = average_fitness;
      data[2] = maximum_fitness;

      slotAddPoint (data);
    }

    /*!
     * Called when the controllers populations changed
     */
    void FitnessDiagramWidget::slotPopulationsChanged 
    (const Core::Controller* controller)
    {
      addStep (controller);
    }

  } // namespace Diagram
} // namespace GEP
