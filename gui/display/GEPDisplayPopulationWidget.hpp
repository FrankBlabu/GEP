/*
 * GEPDisplayPopulationWidget.hpp - Population displaying widget
 *
 * Frank Cieslok, 05.06.2006
 */

#ifndef __GEPDisplayPopulationWidget_hpp__
#define __GEPDisplayPopulationWidget_hpp__

#include <GEPVec2d.hpp>
#include <QtGui/QWidget>

#include <vector>

class QPainter;

namespace GEP {

  namespace Core {
    class Controller;
    class Individual;
    class Population;
  }
  
  namespace Display {

    class PopulationWidgetUI;
    
    /*
     * This interface is must be implemented by all population classes which
     * should be displayed in a PopulationWidget object
     */
    class PopulationDisplayInterface
    {
    public:
      PopulationDisplayInterface ();
      virtual ~PopulationDisplayInterface ();

      virtual std::vector<const Core::Individual*> getIndividuals () const = 0;

      virtual void drawWorld (QPainter* painter) const = 0;
      virtual void drawIndividual 
      (QPainter* painter, const Core::Individual* individual) const = 0;
    };
    
    /*
     * Population displaying widget
     */
    class PopulationWidget : public QWidget
    {
      Q_OBJECT
      
    public:
      PopulationWidget (const Core::Controller* controller, QWidget* parent);
      virtual ~PopulationWidget ();

      void setPopulation (const Core::Population* population);

    public slots:
      void slotUpdate ();
      
    private slots:
      void slotStarted ();
      void slotStopped ();
      void slotPopulationsChanged (const Core::Controller* controller);

    private:
      PopulationWidgetUI* _widget;
    };
    
  } // namespace Display
} // namespace GEP

#endif
