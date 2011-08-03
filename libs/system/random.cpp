/*
 * random.cpp - Random number generator
 *
 * Frank Cieslok, 03.06.2006
 */

#include "GEPRandom.hpp"

#include <QtCore/QMutex>
#include <sys/time.h>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>

namespace GEP {

  //#*************************************************************************
  // CLASS GEP::Random
  //#*************************************************************************

  /* Constructor */
  Random::Random ()
  {
    struct timeval tv;
    gettimeofday (&tv, 0);
    
    _generator.seed (static_cast<unsigned int> (tv.tv_usec));
  }

  /* Get random integer value */
  int Random::getInt (int min, int max) const
  {
    boost::uniform_int<> range (min, max);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> >
      die (_generator, range);

    return die ();
  }
  
  /* Get random double value [STATIC] */
  double Random::getDouble (double min, double max) const
  {
    boost::uniform_real<> range (min, max);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> >
      die (_generator, range);

    return die ();
  }
  
} // namespace GEP

