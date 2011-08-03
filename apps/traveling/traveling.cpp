/*
 * traveling.cpp - Traveling salesman application
 *
 * Frank Cieslok, 03.06.2006
 */

#include "GEPTravelingMainWindow.hpp"
#include <GEPDebug.hpp>
#include <GEPException.hpp>

#include <iostream>

#include <QtGui/QApplication>


//#***************************************************************************
// Global functions
//#***************************************************************************

/*
 * MAIN
 */
int main (int argc, char** argv)
{
  int result = 0;

  QApplication application (argc, argv);
  
  try
  {
    GEP::Traveling::TravelingMainWindow main_window;
    main_window.show ();

    result = application.exec ();
  }
  catch (const GEP::Exception& exception)
  {
    std::cerr << "ERROR: " << qPrintable (exception.getMessage ()) 
	      << std::endl;
    std::cerr.flush ();

    result = -1;
  }

  return result;
}
