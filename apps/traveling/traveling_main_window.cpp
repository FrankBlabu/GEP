/*
 * traveling_main_window.cpp - Main window for the traveling salesman
 *                             application
 *
 * Frank Cieslok, 11.06.2006
 */

#include "GEPTravelingMainWindow.hpp"
#include "GEPTravelingIndividual.hpp"
#include "GEPTravelingPopulation.hpp"

#include <GEPCoreController.hpp>
#include <GEPCoreCriterion.hpp>
#include <GEPCorePopulation.hpp>
#include <GEPCoreRingIndividual.hpp>
#include <GEPCoreSelection.hpp>
#include <GEPDebug.hpp>
#include <GEPDiagramDistributionWidget.hpp>
#include <GEPDiagramFitnessWidget.hpp>
#include <GEPDisplayPopulationList.hpp>
#include <GEPDisplayStatusWidget.hpp>

#include <QtGui/QApplication>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>
#include <QtGui/QSplitter>
#include <QtGui/QToolBar>

#include "traveling_main_window.hpp"

//
// Number of cities to visit
//
#define NUMBER_OF_CITIES 50

namespace GEP {
  namespace Traveling {

    //#***********************************************************************
    // CLASS GEP::Traveling::TravelingMainWindow
    //#***********************************************************************

    /* Constructor */
    TravelingMainWindow::TravelingMainWindow ()
      : Display::MainWindow (),
	_widget            (new Ui::TravelingMainWindowUI ()),
	_population_list   (0),
	_population_widget (0),
	_controller        (new Core::Controller ()),
	_world             (new World (NUMBER_OF_CITIES)),
	_started           (false),
	_running           (false)
    {
      _widget->setupUi (this);

      //
      // Setup sub widget layouts
      //
      QWidget* left_frame = new QWidget ();
      QSplitter* left_splitter = new QSplitter (Qt::Vertical, left_frame);

      QGroupBox* status_group = new QGroupBox ("Status", left_frame);
      QGroupBox* population_group =
	new QGroupBox ("Population", left_splitter);
      QGroupBox* fitness_group = new QGroupBox ("Fitness", left_splitter);

      left_splitter->addWidget (population_group);
      left_splitter->addWidget (fitness_group);
      
      QBoxLayout* left_frame_layout = new QVBoxLayout (left_frame);
      left_frame_layout->setMargin (0);
      left_frame_layout->addWidget (status_group);
      left_frame_layout->addWidget (left_splitter);
      
      QSplitter* main_splitter =
	new QSplitter (Qt::Horizontal, _widget->_content_frame);

      QGroupBox* individual_group =
	new QGroupBox ("Individuals", main_splitter);

      _population_list =
	new Display::PopulationList (_controller, population_group);
      _population_widget =
	new Display::PopulationWidget (_controller, individual_group);
      _status_widget = new Display::StatusWidget (_controller, status_group);
      
      QBoxLayout* status_group_layout = new QVBoxLayout (status_group);
      status_group_layout->setMargin (9);
      status_group_layout->addWidget (_status_widget);

      QSplitter* diagram_splitter =
	new QSplitter (Qt::Horizontal, fitness_group);
      
      _fitness_widget = new Diagram::FitnessDiagramWidget
	(_controller, diagram_splitter);
      _distribution_widget = new Diagram::DistributionDiagramWidget
	(_controller, diagram_splitter);
      
      diagram_splitter->addWidget (_fitness_widget);
      diagram_splitter->addWidget (_distribution_widget);
      
      QBoxLayout* fitness_group_layout = new QHBoxLayout (fitness_group);
      fitness_group_layout->setMargin (9);
      fitness_group_layout->addWidget (diagram_splitter);
      
      QBoxLayout* population_layout = new QVBoxLayout (population_group);
      population_layout->setMargin (9);
      population_layout->addWidget (_population_list);
      
      QBoxLayout* individuals_layout = new QVBoxLayout (individual_group);
      individuals_layout->setMargin (9);
      individuals_layout->addWidget (_population_widget);
      
      QBoxLayout* main_splitter_layout =
	new QVBoxLayout (_widget->_content_frame);
      main_splitter_layout->addWidget (main_splitter);
      main_splitter->addWidget (left_frame);
      main_splitter->addWidget (individual_group);
      
      //
      // Setup toolbar
      //
      QToolBar* tool_bar = new QToolBar (this);
      addToolBar (Qt::TopToolBarArea, tool_bar);

      _start_action = tool_bar->addAction
	("Start", this, SLOT (slotStart ()));
      _continue_action = tool_bar->addAction 
	("Continue", this, SLOT (slotContinue ()));
      _step_action = tool_bar->addAction ("Step", this, SLOT (slotStep ()));

      //
      // Setup controller object
      //
      _controller->setPopulationCreator (new RoutePopulationCreator (_world));
      _controller->setNumberOfPopulations (1);
      _controller->setNumberOfIndividuals (100);
      _controller->setCrossoverProbability (0.3);

#if 1
      _controller->addSelectionOperator
	(new Core::RouletteSelectionOperator ());
#else
      _controller->addSelectionOperator
	(new Core::BestSelectionOperator ());
#endif
#if 1
      _controller->addCrossoverOperator
	(new Core::RingPartiallyMatchedCrossoverOperator ());
#else
      _controller->addCrossoverOperator
	(new Core::RingResortingCrossoverOperator ());
#endif
      _controller->addMutationOperator
	(new Core::RingUniformMutationOperator (0.2 / NUMBER_OF_CITIES));
      _controller->addTerminationCriterion
	(new Core::FixedStepTerminationCriterion (500, 50));

      connect (_controller, SIGNAL (signalStep (const Core::Controller*)),
	       SLOT (slotUpdate ()));
      connect (_population_list, SIGNAL (signalSelectionChanged ()),
	       _population_widget, SLOT (slotUpdate ()));
     connect (this, SIGNAL (signalEscapePressed ()),
	       _controller, SLOT (slotStop ()));

      updateEnabledState ();
    }

    /* Destructor */
    TravelingMainWindow::~TravelingMainWindow ()
    {
      delete _controller;
      _controller = 0;

      delete _world;
      _world = 0;
    }

    /* Start new computation */
    void TravelingMainWindow::slotStart ()
    {
      if (_controller->initialize ())
	{
	  _fitness_widget->slotClear ();
	  _started = true;

	  slotContinue ();
	}    
    }
    
    /* Continue computation */
    void TravelingMainWindow::slotContinue ()
    {
      if (!_started)
	throw InternalException ("Controller is not yet started");

      _running = true;
      updateEnabledState ();

      if (!_controller->execute ())
	_started = false;
      
      _running = false;
      updateEnabledState ();
    }

    /* Execute single step */
    void TravelingMainWindow::slotStep ()
    {
      if (!_started)
	throw InternalException ("Controller is not yet started");

      _running = true;
      updateEnabledState ();

      if (!_controller->executeStep ())
	_started = false;
      
      _running = false;
      updateEnabledState ();
    }
    
    /* Update display */
    void TravelingMainWindow::slotUpdate ()
    {
      update ();
      QApplication::processEvents ();
    }

    /* Update UI enabled state */
    void TravelingMainWindow::updateEnabledState ()
    {
      _start_action->setEnabled (!_running);
      _continue_action->setEnabled (_started && !_running);
      _step_action->setEnabled (_started && !_running);
    }
    
  } // namespace Traveling
} // namespace GEP
