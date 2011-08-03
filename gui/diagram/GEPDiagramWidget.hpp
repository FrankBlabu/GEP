/*
 * GEPDiagramWidget.hpp - Widgets displaying data diagrams
 *
 * Frank Cieslok, 09.07.2006
 */

#ifndef __GEPDiagramWidget_hpp__
#define __GEPDiagramWidget_hpp__

#include <GEPDisplayPlaneWidget.hpp>
#include <GEPVec2d.hpp>

#include <QtGui/QColor>
#include <QtGui/QWidget>

#include <vector>

namespace GEP {
  namespace Diagram {

    /*!
     * Abstract base class for all data diagram displaying widgets
     */
    class DiagramWidget : public GEP::Display::PlaneWidget
    {
      Q_OBJECT

      static const double INVALID_POINT;
      
    public:
      DiagramWidget (unsigned int number_of_lines, QWidget* parent);
      virtual ~DiagramWidget ();

      inline unsigned int getNumberOfLines () const;

      const QColor& getLineColor (unsigned int index) const;
      void setLineColor (unsigned int index, const QColor& color);
      
      virtual QSize sizeHint () const;

    public slots:
      virtual void slotClear () = 0;

    protected:
      typedef std::vector<double> Data_t;
      virtual const std::vector<Data_t>& getData () const = 0;

      void invalidateCachedData ();

      virtual void paintEvent (QPaintEvent* event);

    private:
      unsigned int _number_of_lines;

      bool _cached_data_valid;
      double _min_value;
      double _max_value;

      std::vector<QColor> _line_colors;
    };


    /*!
     * Diagram widget for continuously collected data
     */
    class DataCollectingDiagramWidget : public GEP::Diagram::DiagramWidget
    {
      Q_OBJECT

    public:
      DataCollectingDiagramWidget (unsigned int number_of_lines,
				   QWidget* parent);
      virtual ~DataCollectingDiagramWidget ();

    public slots:
      virtual void slotClear ();
      void slotAddPoint (const Data_t& point);

    protected:
      virtual const std::vector<Data_t>& getData () const;

    private:
      std::vector<Data_t> _data;
    };

    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Return the number of lines drawn by this diagram */
    inline unsigned int DiagramWidget::getNumberOfLines () const
    {
      return _number_of_lines;
    }
    
  } // namespace Diagram
} // namespace GEP

#endif
