/*
 * GEPTravelingMainWindow.hpp - Main window for the traveling salesman
 *                              application
 *
 * Frank Cieslok, 11.06.2006
 */

#ifndef __GEPTravelingMainWindow_hpp__
#define __GEPTravelingMainWindow_hpp__

#include <GEPDisplayMainWindow.hpp>

class QAction;

namespace Ui {
  class TravelingMainWindowUI;
}

namespace GEP {

  namespace Core {
    class Controller;
  }
  
  namespace Display {
    class PopulationList;
    class PopulationWidget;
    class StatusWidget;
  }

  namespace Diagram {
    class DistributionDiagramWidget;
    class FitnessDiagramWidget;
  }
    
  namespace Traveling {

    class World;
    
    /*
     * Main window for the traveling salesman application
     */
    class TravelingMainWindow : public Display::MainWindow
    {
      Q_OBJECT
      
    public:
      TravelingMainWindow ();
      virtual ~TravelingMainWindow ();

    private slots:
      void slotStart ();
      void slotContinue ();
      void slotStep ();
      void slotUpdate ();

    private:
      void updateEnabledState ();
      
    private:
      Ui::TravelingMainWindowUI* _widget;

      QAction* _start_action;
      QAction* _continue_action;
      QAction* _step_action;
      
      Display::PopulationList* _population_list;
      Display::PopulationWidget* _population_widget;
      Display::StatusWidget* _status_widget;
      
      Diagram::FitnessDiagramWidget* _fitness_widget;
      Diagram::DistributionDiagramWidget* _distribution_widget;
      
      Core::Controller* _controller;

      World* _world;
      bool _started;
      bool _running;
    };
    
  } // namespace Traveling
} // namespace GEP

#endif
