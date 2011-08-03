/*
 * test_ring_buffer.cpp - Test for the ring buffer container
 *
 * Frank Cieslok, 04.06.2006
 */

#include "GEPRingBuffer.hpp"
#include "GEPDebug.hpp"

#include <QtCore/QString>
#include <vector>

using namespace std;

namespace GEP {

  static QString toString (const vector<int>& sequence)
  {
    QString result;

    QString separator = "";
    for (unsigned int i=0; i < sequence.size (); ++i)
    {
      result += separator + QString ("%1").arg (sequence[i]);
      separator = ", ";
    }
    
    return result;
  }
			  
  static void testRingBuffer ()
  {
    RingBuffer<int> buffer;

    for (int i=0; i < 10; ++i)
      buffer.push_back (i);

    DV ("Source: " << toString (buffer));

    for (unsigned int i=0; i < buffer.size (); ++i)
      DV (i << ": " << toString (buffer.get (i, 5)));
  }
  
} // namespace GEP

int main (int /*argc*/, char** /*argv*/)
{
  GEP::testRingBuffer ();
  return 0;
}
