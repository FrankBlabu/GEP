/*
 * GEPDisplayPlaneWidget.hpp - Widget for scrollable and zoomable plane
 *                             display widgets 
 *
 * Frank Cieslok, 26.07.2006
 */

#ifndef __GEPDisplayPlaneWidget_hpp__
#define __GEPDisplayPlaneWidget_hpp__

#include <GEPVec2d.hpp>
#include <QtGui/QWidget>

class QPainter;

namespace GEP {
  namespace Display {
    
    /*
     * Widget for scrollable and zoomable plane display widgets
     */
    class PlaneWidget : public QWidget
    {
    public:
      PlaneWidget (QWidget* parent);
      virtual ~PlaneWidget ();

      inline const Vec2d& getOffset () const;
      inline double getZoom () const;
      
      virtual void mousePressEvent (QMouseEvent* event);
      virtual void mouseMoveEvent (QMouseEvent* event);
      virtual void wheelEvent (QWheelEvent* event);

    protected:
      void setupPainter (QPainter* painter);
      
    private:
      Vec2d _offset;
      double _zoom;

      Vec2d _last_mouse_position;
    };


    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /*! Get current drawing offset */
    inline const Vec2d& PlaneWidget::getOffset () const
    {
      return _offset;
    }

    /*! Get current drawing zoom factor */
    inline double PlaneWidget::getZoom () const
    {
      return _zoom;
    }

  } // namespace Display
} // namespace GEP

#endif
