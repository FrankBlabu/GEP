/*
 * gep.cpp - General evaluation GUI for the GEP project
 *
 * Frank Cieslok, 05.06.2006
 */

#include <GEPDebug.hpp>

#include <QtGui/QApplication>

using namespace std;

//#***************************************************************************
// Global functions
//#***************************************************************************

/*
 * MAIN
 */
int main (int argc, char** argv)
{
  QApplication app (argc, argv);
  
  return app.exec ();
}
