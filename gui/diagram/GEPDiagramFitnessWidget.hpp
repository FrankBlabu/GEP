/*
 * GEPDiagramFitnessWidget.hpp - Widgets displaying fitness values
 *
 * Frank Cieslok, 15.07.2006
 */

#ifndef __GEPDiagramFitnessWidget_hpp__
#define __GEPDiagramFitnessWidget_hpp__

#include "GEPDiagramWidget.hpp"

namespace GEP {

  namespace Core {
    class Controller;
  }
  
  namespace Diagram {

    /*!
     * Diagram widget for displaying a controllers fitness status
     */
    class FitnessDiagramWidget
      : public GEP::Diagram::DataCollectingDiagramWidget
    {
      Q_OBJECT

    public:
      FitnessDiagramWidget (Core::Controller* controller, QWidget* parent);
      virtual ~FitnessDiagramWidget ();

    public:
      void addStep (const Core::Controller* controller);

    private slots:
      void slotPopulationsChanged (const Core::Controller* controller);
    };
    
  } // namespace Diagram
} // namespace GEP

#endif
