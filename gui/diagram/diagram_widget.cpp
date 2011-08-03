/*
 * diagram_widget.cpp - Widgets displaying data diagrams
 *
 * Frank Cieslok, 09.07.2006
 */

#include "GEPDiagramDistributionWidget.hpp"

#include <GEPException.hpp>
#include <GEPDebug.hpp>

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPolygon>
#include <QtGui/QWheelEvent>

#include <math.h>
#include <limits>

namespace GEP {
  namespace Diagram {

    //#***********************************************************************
    // CLASS GEP::Diagram::Diagram
    //#***********************************************************************

    const double DiagramWidget::INVALID_POINT =
    std::numeric_limits<double>::max ();

    /*! Constructor */
    DiagramWidget::DiagramWidget (unsigned int number_of_lines,
				  QWidget* parent)
      : GEP::Display::PlaneWidget (parent),
	_number_of_lines   (number_of_lines),
	_cached_data_valid (false)
    {
      for (unsigned int i=0; i < number_of_lines; ++i)
	_line_colors.push_back (Qt::red);
    }

    /*! Destructor */
    DiagramWidget::~DiagramWidget ()
    {
    }

    /*! Get the color used for line 'index' */
    const QColor& DiagramWidget::getLineColor (unsigned int index) const
    {
      if (index >= _number_of_lines)
	throw InternalException ("Index exceeds number of diagram lines");

      return _line_colors[index];
    }

    /*! Set the color used for line 'index' */
    void DiagramWidget::setLineColor (unsigned int index, const QColor& color)
    {
      if (index >= _number_of_lines)
	throw InternalException ("Index exceeds number of diagram lines");

      _line_colors[index] = color;
    }

    /* Invalidate cached data */
    void DiagramWidget::invalidateCachedData ()
    {
      _cached_data_valid = false;
    }
    
    /* Return the desired minimum size */
    QSize DiagramWidget::sizeHint () const
    {
      QSize size = QWidget::sizeHint ();
      size.setHeight (100);
      return size;
    }
    
    /* Handle paint event */
    void DiagramWidget::paintEvent (QPaintEvent* event)
    {
      //
      // Draw widget background
      //
      QPainter painter (this);
      painter.fillRect (contentsRect (), Qt::black);
      
      //
      // Compute data range
      //
      const std::vector<Data_t>& data = getData ();
      if (!_cached_data_valid)
      {
	_min_value = std::numeric_limits<double>::max ();
	_max_value = std::numeric_limits<double>::min ();
	
	for (unsigned int i=0; i < data.size (); ++i)
	{
	  const Data_t& values = data[i];
	  if (values.size () != _number_of_lines)
	    throw InternalException ("Data entry sizes to not match");
	  
	  for (unsigned j=0; j < values.size (); ++j)
	  {
	    _min_value = std::min (_min_value, values[j]);
	    _max_value = std::max (_max_value, values[j]);
	  }
	}
      }
      
      //
      // Draw the lines
      //
      QPen line_pen (Qt::SolidLine);
      line_pen.setWidth (1);

      Vec2d offset (contentsRect ().x (), contentsRect ().y ());
      Vec2d size (contentsRect ().width (), contentsRect ().height ());
      Vec2d data_scale (data.size (), _max_value - _min_value);
      
      setupPainter (&painter);
      
      for (unsigned int i=0; i < _number_of_lines; ++i)
      {
	const QColor& color = _line_colors[i];

	QPolygonF line (data.size ());
	for (unsigned int j=0; j < data.size (); ++j)
	  line[j] = QPointF
	    (offset._x + j * size._x / data_scale._x,
	     offset._y + (_max_value - data[j][i]) * size._y / data_scale._y);
	
	line_pen.setColor (color);
	painter.setPen (line_pen);
	
	painter.drawPolyline (line);
      }
    }


    //#***********************************************************************
    // CLASS GEP::Diagram::DataCollectingDiagramWidget
    //#***********************************************************************

    /*! Constructor */
    DataCollectingDiagramWidget::DataCollectingDiagramWidget
    (unsigned int number_of_lines, QWidget* parent)
      : DiagramWidget (number_of_lines, parent)
    {
    }

    /*! Destructor */
    DataCollectingDiagramWidget::~DataCollectingDiagramWidget ()
    {
    }

    /*! Clear collected data */
    void DataCollectingDiagramWidget::slotClear ()
    {
      _data.clear ();
    }

    /*! Add diagram point */
    void DataCollectingDiagramWidget::slotAddPoint (const Data_t& point)
    {
      if (point.size () != getNumberOfLines ())
	throw InternalException ("Wrong diagram data set size");
      
      _data.push_back (point);
      invalidateCachedData ();
      update ();
    }

    /*! Fetch data to display */
    const std::vector<DiagramWidget::Data_t>&
    DataCollectingDiagramWidget::getData () const
    {
      return _data;
    }

  } // namespace Diagram
} // namespace GEP
