/*
 * display_status_widget.cpp - Status displaying widget
 *
 * Frank Cieslok, 05.07.2006
 */

#include "display_status_widget.hpp"

#include "GEPDisplayStatusWidget.hpp"
#include <GEPCoreController.hpp>
#include <GEPCoreIndividual.hpp>
#include <GEPCorePopulation.hpp>

#include <QtGui/QLayout>

namespace GEP
{
  namespace Display
  {

    //#***********************************************************************
    // CLASS GEP::Display::StatusWidgetUI
    //#***********************************************************************

    /*
     * GUI elements for this widget
     */
    class StatusWidgetUI : public QWidget, public Ui::DisplayStatusWidgetUI
    {
    public:
      StatusWidgetUI (QWidget* parent);
      virtual ~StatusWidgetUI ();

      void update (const Core::Controller* controller);
    };

    /* Constructor */
    StatusWidgetUI::StatusWidgetUI (QWidget* parent)
        : QWidget (parent)
    {
      setupUi (this);
    }

    /* Destructor */
    StatusWidgetUI::~StatusWidgetUI ()
    {}

    /* Update status display */
    void StatusWidgetUI::update (const Core::Controller* controller)
    {
      double max_fitness = std::numeric_limits<double>::min ();
      double average_fitness = std::numeric_limits<double>::min ();

      unsigned int count = 0;
      for ( Core::Controller::PopulationConstIterator i =
              controller->getPopulationBegin ();
            i != controller->getPopulationEnd (); ++i)
      {
        const Core::Population* population = *i;

        for ( Core::Population::IndividualConstIterator j =
                population->getIndividualBegin ();
              j != population->getIndividualEnd (); ++j, ++count )
        {
          const Core::Individual* individual = *j;
	  max_fitness = std::max (max_fitness, individual->getFitness ());
          average_fitness += individual->getFitness ();
	}
      }

      if (count > 0)
	average_fitness /= count;

      _step->setText (QString::number (controller->getStep ()));

      if (average_fitness != std::numeric_limits<double>::min ())
	_average_fitness->setText (QString::number (average_fitness, 'f', 4));
      else
	_average_fitness->setText (QString ("(unknown)"));

      if (max_fitness != std::numeric_limits<double>::min ())
	_max_fitness->setText (QString::number (max_fitness, 'f', 4));
      else
	_max_fitness->setText (QString ("(unknown)"));
    }


    //#***********************************************************************
    // CLASS GEP::Display::StatusWidget
    //#***********************************************************************

    /* Constructor */
    StatusWidget::StatusWidget (Core::Controller* controller, QWidget* parent)
        : QWidget (parent),
          _widget   (new StatusWidgetUI (this))
    {
      QBoxLayout* layout = new QHBoxLayout (this);
      layout->setMargin (0);
      layout->addWidget (_widget);

      connect (controller,
	       SIGNAL (signalPopulationsChanged (const Core::Controller*)),
	       SLOT (slotUpdate (const Core::Controller*)));
    }

    /* Destructor */
    StatusWidget::~StatusWidget ()
    {}

    /* Update status display */
    void StatusWidget::slotUpdate (const Core::Controller* controller)
    {
      _widget->update (controller);
    }

  } // namespace Display
} // namespace GEP
