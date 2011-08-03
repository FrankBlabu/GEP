/*
 * test_display.cpp - Test for the display classes
 *
 * Frank Cieslok, 05.06.2006
 */

#include "GEPDisplayPopulationWidget.hpp"

#include <QApplication>
#include <string>

using namespace std;

namespace GEP {

  //#*************************************************************************
  // Test functions
  //#*************************************************************************
  
} // namespace GEP

int main (int argc, char** argv)
{
  QApplication app (argc, argv);
  
  GEP::Display::PopulationWidget widget (0);
  widget.show ();

  return app.exec ();
}
