/*
 * display_population_widget.hpp - Base class for population displaying
 *                                 widgets
 *
 * Frank Cieslok, 05.06.2006
 */

#define _DEBUG

#include "GEPDisplayPlaneWidget.hpp"
#include "GEPDisplayPopulationWidget.hpp"

#include <GEPCoreController.hpp>
#include <GEPCoreIndividual.hpp>
#include <GEPCorePopulation.hpp>
#include <GEPDebug.hpp>
#include <GEPVec2d.hpp>

#include "display_population_widget.hpp"

#include <QtGui/QButtonGroup>
#include <QtGui/QLayout>
#include <QtGui/QPainter>

#include <algorithm>
#include <boost/bind.hpp>

namespace GEP
{
  namespace Display
  {

    //#***********************************************************************
    // CLASS GEP::Display::PopulationWidgetDrawArea
    //#***********************************************************************

    /*
     * Drawing area for the population displaying widget
     */
    class PopulationWidgetDrawArea : public GEP::Display::PlaneWidget
    {
    public:
      PopulationWidgetDrawArea (QWidget* parent);
      virtual ~PopulationWidgetDrawArea ();

      void setPopulation (const PopulationDisplayInterface* population);

      enum DisplayMode_t
      {
        SHOW_ALL, SHOW_FITTEST, SHOW_SELECTED
      };
      void setDisplayMode (DisplayMode_t mode);

      virtual void paintEvent (QPaintEvent* event);

    private:
      const PopulationDisplayInterface* _population;
      DisplayMode_t _display_mode;
    };

    /* Constructor */
    PopulationWidgetDrawArea::PopulationWidgetDrawArea (QWidget* parent)
      : PlaneWidget (parent),
	_population   (0),
	_display_mode (SHOW_ALL)
    {}

    /* Destructor */
    PopulationWidgetDrawArea::~PopulationWidgetDrawArea ()
    {}

    /*
     * Set population to be displayed in the widget
     *
     * \param population Population to be displayed. The population object
     *                   is *not* copied and must remain valid as long as
     *                   the pointer is set
     */
    void PopulationWidgetDrawArea::setPopulation
    (const PopulationDisplayInterface* population)
    {
      _population = population;
    }

    /* Set population display mode */
    void PopulationWidgetDrawArea::setDisplayMode (DisplayMode_t mode)
    {
      _display_mode = mode;
    }

    /* Handle paint event */
    void PopulationWidgetDrawArea::paintEvent (QPaintEvent* event)
    {
      QWidget::paintEvent (event);

      QPainter painter (this);
      painter.fillRect (contentsRect (), Qt::black);

      setupPainter (&painter);

      if (_population != 0)
	{
	  std::vector<const Core::Individual*> individuals =
	    _population->getIndividuals ();
	  
	  std::sort (individuals.begin (), individuals.end (),
		     Core::IndividualFitnessComparator ());
	  
	  _population->drawWorld (&painter);

	  switch (_display_mode)
	    {
	      case SHOW_ALL:
		//
		// Show all individuals at once. The fittest individual is
		// drawn last on top of the other individuals.
		//
		for (unsigned int i=0; i < individuals.size (); ++i)
		  _population->drawIndividual (&painter, individuals[i]);
		break;
		
	      case SHOW_FITTEST:
		//
		// Show only the fittest individual
		//
		if (individuals.size () > 0)
		  _population->drawIndividual
		    (&painter, individuals[individuals.size () - 1]);
		break;

	      case SHOW_SELECTED:
		//
		// Show the selected individuals;
		//
		
		for (unsigned int i=0; i < individuals.size (); ++i)
		  {
		    const Core::Individual* individual = individuals[i];
		    if (individual->getSelected ())
		      _population->drawIndividual (&painter, individual);
		  }
		break;
	    }
	}
    }


    //#***********************************************************************
    // CLASS GEP::Display::PopulationWidgetUI
    //#***********************************************************************

    /*
     * GUI elements for this widget
     */
    class PopulationWidgetUI 
      : public QWidget, 
	public Ui::DisplayPopulationWidgetUI
    {
    public:
      PopulationWidgetUI (const Core::Controller* controller, QWidget* parent);
      virtual ~PopulationWidgetUI ();

      void setPopulation (const PopulationDisplayInterface* population);
      void setControllerRunning (bool state);
      void update ();

    private:
      const Core::Controller* _controller;
      PopulationWidgetDrawArea* _draw_area;

      bool _controller_running;
      bool _update_in_progress;
    };

    /* Constructor */
    PopulationWidgetUI::PopulationWidgetUI (const Core::Controller* controller,
					    QWidget* parent)
      : QWidget (parent),
	_controller         (controller),
	_controller_running (false),
	_update_in_progress (false)
    {
      setupUi (this);

      _draw_area = new PopulationWidgetDrawArea (_display);

      QBoxLayout* layout = new QVBoxLayout (_display);
      layout->setMargin (0);
      layout->addWidget (_draw_area);

      QButtonGroup* button_group = new QButtonGroup (this);
      button_group->addButton (_show_all);
      button_group->addButton (_show_fittest);
      button_group->addButton (_show_selected);
      button_group->setExclusive (true);

      update ();
    }

    /* Destructor */
    PopulationWidgetUI::~PopulationWidgetUI ()
    {
    }

    /*
     * Set population to be displayed in the widget
     *
     * \param population Population to be displayed. The population object
     *                   is *not* copied and must remain valid as long as
     *                   the pointer is set
     */
    void PopulationWidgetUI::setPopulation
    (const PopulationDisplayInterface* population)
    {
      _draw_area->setPopulation (population);
    }

    /*!
     * Set the running state of the controller
     */
    void PopulationWidgetUI::setControllerRunning (bool state)
    {
      _controller_running = state;
    }

    /* Update display parameters */
    void PopulationWidgetUI::update ()
    {
      if (!_update_in_progress)
	{
	  _update_in_progress = true;

	  //
	  // When the controller is running, selected individuals cannot
	  // be displayed
	  //
	  if (_controller_running)
	    {
	      if (_show_selected->isChecked ())
		_show_all->setChecked (true);
	      
	      _show_selected->setEnabled (false);
	    }
	  else
	    _show_selected->setEnabled (true);
	  
	  //
	  // Propagate display mode to the drawing area
	  //
	  if (_show_all->isChecked ())
	    _draw_area->setDisplayMode 
	      (PopulationWidgetDrawArea::SHOW_ALL);
	  else if (_show_fittest->isChecked ())
	    _draw_area->setDisplayMode 
	      (PopulationWidgetDrawArea::SHOW_FITTEST);
	  else if (_show_selected->isChecked ())
	    _draw_area->setDisplayMode 
	      (PopulationWidgetDrawArea::SHOW_SELECTED);
	  else
	    throw InternalException ("Illegal GUI state");
	  
	  _draw_area->update ();

	  _update_in_progress = false;
	}
    }


    //#***********************************************************************
    // CLASS GEP::Display::PopulationDisplayInterface
    //#***********************************************************************

    /* Constructor */
    PopulationDisplayInterface::PopulationDisplayInterface ()
    {}

    /* Destructor */
    PopulationDisplayInterface::~PopulationDisplayInterface ()
    {}


    //#***********************************************************************
    // CLASS GEP::Display::PopulationWidget
    //#***********************************************************************

    /* Constructor */
    PopulationWidget::PopulationWidget (const Core::Controller* controller,
                                        QWidget* parent)
      : QWidget (parent),
	_widget (new PopulationWidgetUI (controller, this))
    {
      QBoxLayout* layout = new QVBoxLayout (this);
      layout->setMargin (0);
      layout->addWidget (_widget);

      connect (_widget->_show_all, SIGNAL (clicked ()),
               SLOT (slotUpdate ()));
      connect (_widget->_show_fittest, SIGNAL (clicked ()),
               SLOT (slotUpdate ()));
      connect (_widget->_show_selected, SIGNAL (clicked ()),
               SLOT (slotUpdate ()));
      connect (controller,
	       SIGNAL (signalStarted (const Core::Controller*)),
	       SLOT (slotStarted ()));
      connect (controller,
	       SIGNAL (signalStopped (const Core::Controller*)),
	       SLOT (slotStopped ()));
      connect (controller,
	       SIGNAL (signalPopulationsChanged (const Core::Controller*)),
	       SLOT (slotPopulationsChanged (const Core::Controller*)));
    }

    /* Destructor */
    PopulationWidget::~PopulationWidget ()
    {
    }

    /*
     * Set population to be displayed in the widget
     *
     * \param population Population to be displayed. The population object
     *                   is *not* copied and must remain valid as long as
     *                   the pointer is set
     */
    void PopulationWidget::setPopulation(const Core::Population* population)
    {
      _widget->setPopulation
      (dynamic_cast<const PopulationDisplayInterface*> (population));
    }

    /*
     * Called when the controller started an execution cycle
     */
    void PopulationWidget::slotStarted ()
    {
      _widget->setControllerRunning (true);
      _widget->update ();
    }

    /*
     * Called when the controller started an execution cycle 
     */
    void PopulationWidget::slotStopped ()
    {
      _widget->setControllerRunning (false);
      _widget->update ();
    }

    /*
     * Forward update initiated by a display parameter change
     */
    void PopulationWidget::slotUpdate ()
    {
      _widget->update ();
    }

    /*!
     * Called if a controllers population changed
     */
    void PopulationWidget::slotPopulationsChanged (const Core::Controller* 
						  controller)
    {
      if ( controller->getPopulationBegin () !=
           controller->getPopulationEnd () )
        setPopulation (*controller->getPopulationBegin ());
      else
	setPopulation (0);
    }

  } // namespace Display
} // namespace GEP

