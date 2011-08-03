/*
 * test_trace_guard.cpp - Test for the TraceGuard class
 *
 * Frank Cieslok, 24.06.2006
 */

#include "GEPTraceGuard.hpp"
#include "GEPDebug.hpp"

#include <string>

using namespace std;

namespace GEP {

  //#*************************************************************************
  // Test functions
  //#*************************************************************************

  /*
   * Test TraceGuard class
   */
  static void testTraceGuard ()
  {
    TraceGuard guard;
    
    TraceGuard::callDebugger ("Test", TraceGuard::BT);
  }
  
} // namespace GEP

int main (int /*argc*/, char** /*argv*/)
{
  GEP::testTraceGuard ();
  return 0;
}
