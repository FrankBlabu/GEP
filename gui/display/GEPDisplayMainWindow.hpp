/*
 * GEPDisplayMainWindow.hpp - Main window for all GEP based applications
 *
 * Frank Cieslok, 01.07.2007
 */

#ifndef __GEPDisplayMainWindow_hpp__
#define __GEPDisplayMainWindow_hpp__

#include <QtGui/QMainWindow>

namespace GEP {
  namespace Display {

    /*!
     * Main window for all GEP based applications
     */
    class MainWindow : public QMainWindow
    {
      Q_OBJECT

    public:
      MainWindow ();
      virtual ~MainWindow ();

      virtual bool event (QEvent* event);

      signals:
        void signalEscapePressed ();
    };

  } // namespace Display
} // namespace GEP

#endif
