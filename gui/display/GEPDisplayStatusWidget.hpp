/*
 * GEPDisplayStatusWidget.hpp - Status displaying widget
 *
 * Frank Cieslok, 05.07.2006
 */

#ifndef __GEPDisplayStatusWidget_hpp__
#define __GEPDisplayStatusWidget_hpp__

#include <QtGui/QWidget>

namespace GEP {

  namespace Core {
    class Controller;
  }
  
  namespace Display {

    class StatusWidgetUI;
    
    /*
     * Process status displaying widget
     */
    class StatusWidget : public QWidget
    {
      Q_OBJECT

    public:
      StatusWidget (Core::Controller* controller, QWidget* parent);
      virtual ~StatusWidget ();

    public slots:
      void slotUpdate (const Core::Controller* controller);
      
    private:
      StatusWidgetUI* _widget;
    };
    
  } // namespace Display
} // namespace GEP

#endif

