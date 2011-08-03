/*
 * test_random.cpp - Test for the random number generator
 *
 * Frank Cieslok, 24.06.2007
 */

#include "GEPRandom.hpp"
#include "GEPDebug.hpp"

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QThread>

#include <iostream>

using namespace std;

namespace GEP {

  //
  // Anzahl der parallelen Threads
  //
  static const unsigned int NUMBER_OF_THREADS = 20;

  //
  // Gewartete Zeit in Sekunden
  //
  static const unsigned int TEST_TIME = 10;

  //#***********************************************************************
  // KLASSE GEP::RandomThread
  //#***********************************************************************

  /*
   * Thread zum Generieren der Zufallszahlen
   */
  class RandomThread : public QThread
  {
  public:
    RandomThread ();

    inline unsigned int getNumberOfRuns () const { return _number_of_runs; }
    void stop ();

  protected:
    virtual void run ();

  private:
    Random _random;
    bool _stopped;

    unsigned int _number_of_runs;
  };

  /*! Konstruktor */
  RandomThread::RandomThread ()
    : _random  (),
      _stopped (false),
      _number_of_runs (0)
  {
  }

  /*! Thread sicher beenden */
  void RandomThread::stop ()
  {
    _stopped = true;
  }

  /*! Thread ausfuehren */
  void RandomThread::run ()
  {
    while (!_stopped)
      {
	_random.getInt (std::numeric_limits<int>::min (),
			std::numeric_limits<int>::max ());
	_random.getDouble (std::numeric_limits<double>::min (),
			   std::numeric_limits<double>::max ());
	++_number_of_runs;
      }
  }

  //#***********************************************************************
  // Globale Funktionen
  //#***********************************************************************

  /* Test ausfuehren */
  void testRandom ()
  {
    RandomThread threads[NUMBER_OF_THREADS];

    for (unsigned int i=0; i < NUMBER_OF_THREADS; ++i)
      threads[i].start ();

    QMutex mutex;
    QWaitCondition condition;

    mutex.lock ();
    condition.wait (&mutex, TEST_TIME * 1000);

    for (unsigned int i=0; i < NUMBER_OF_THREADS; ++i)
      threads[i].stop ();

    for (unsigned int i=0; i < NUMBER_OF_THREADS; ++i)
      {
	threads[i].wait ();
	std::cout << "Thread " << i << ": " <<
	  threads[i].getNumberOfRuns () << " runs" << std::endl;
      }
  }
  
} // namespace GEP

int main (int /*argc*/, char** /*argv*/)
{
  GEP::testRandom ();
  return 0;
}
