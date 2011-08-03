/*
 * display_main_window.cpp - Main window for all GEP based applications
 *
 * Frank Cieslok, 01.07.2007
 */

#include "GEPDisplayMainWindow.hpp"
#include <GEPException.hpp>
#include <GEPDebug.hpp>

#include <QtGui/QKeyEvent>

namespace GEP {
  namespace Display {

    //#***********************************************************************
    // CLASS GEP::Display::MainWindow
    //#***********************************************************************

    /*! Constructor */
    MainWindow::MainWindow ()
      : QMainWindow ()
    {
    }

    /*! Destructor */
    MainWindow::~MainWindow ()
    {
    }
    
    /*! Event filter for all incoming application events */
    bool MainWindow::event (QEvent* event)
    {
      bool processed = false;

      //
      // Keys
      //
      if (event->type () == QEvent::KeyPress)
	{
	  QKeyEvent* key_event = dynamic_cast<QKeyEvent*> (event);
	  if (key_event == 0)
	    throw InternalException ("KeyPress event without matching object "
				     "received");

	  if (key_event->key () == Qt::Key_Escape)
	    {
	      emit signalEscapePressed ();
	      processed = true;
	    }
	}

      //
      // Default event dispatching
      //
      if (!processed)
	processed = QMainWindow::event (event);

      return processed;
    }

  } // namespace Display
} // namespace GEP
