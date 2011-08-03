/*
 * display_plane_widget.cpp - Widget for scrollable and zoomable plane
 *                            display widgets 
 *
 * Frank Cieslok, 26.07.2006
 */

#include "GEPDisplayPlaneWidget.hpp"

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QWheelEvent>

namespace GEP {
  namespace Display {

    //#***********************************************************************
    // CLASS GEP::Display::PlaneWidget
    //#***********************************************************************

    /* Constructor */
    PlaneWidget::PlaneWidget (QWidget* parent)
      : QWidget (parent),
	_offset              (0.0, 0.0),
	_zoom                (1.0),
	_last_mouse_position (0.0, 0.0)
    {
    }

    /* Destructor */
    PlaneWidget::~PlaneWidget ()
    {
    }

    /* Setup painter with the current translation and scaling */
    void PlaneWidget::setupPainter (QPainter* painter)
    {
      painter->scale (_zoom, _zoom);
      painter->translate (_offset._x, _offset._y);
    }

    /* Handle mouse press events */
    void PlaneWidget::mousePressEvent (QMouseEvent* event)
    {
      _last_mouse_position = Vec2d (event->x (), event->y());
    }

    /* Handle mouse move events */
    void PlaneWidget::mouseMoveEvent (QMouseEvent* event)
    {
      Vec2d delta (event->x () - _last_mouse_position._x,
		   event->y () - _last_mouse_position._y);

      _offset += Vec2d (delta._x / _zoom, delta._y / _zoom);
      _offset._x = std::min (_offset._x, 0.0);
      _offset._y = std::min (_offset._y, 0.0);
      _offset._x = std::max (_offset._x, -width () + width () / _zoom);
      _offset._y = std::max (_offset._y, -height () + height () / _zoom);
      
      _last_mouse_position = Vec2d (event->x (), event->y ());

      update ();
    }

    /* Handle mouse wheel events */
    void PlaneWidget::wheelEvent (QWheelEvent* event)
    {
      double current_zoom = _zoom;
      Vec2d size (contentsRect ().width (), contentsRect ().height ());
      
      _zoom *= 1.0 - 0.1 * (event->delta () / (8 * 15));
      _zoom = std::max (_zoom, 1.0);
      _zoom = std::min (_zoom, 100.0);

      _offset._x +=
	(size._x / 2.0) / _zoom - (size._x / 2.0 ) / current_zoom;
      _offset._y +=
	(size._y / 2.0) / _zoom - (size._y / 2.0) / current_zoom;
      
      _offset._x = std::min (_offset._x, 0.0);
      _offset._y = std::min (_offset._y, 0.0);
      _offset._x = std::max (_offset._x, -size._x + size._x / _zoom);
      _offset._y = std::max (_offset._y, -size._y + size._y / _zoom);

      update ();
    }

  } // namespace Display
} // namespace GEP
