/*
 * GEPRandom.hpp - Random number generator
 *
 * Frank Cieslok, 03.06.2006
 */

#ifndef __GEPRandom_hpp__
#define __GEPRandom_hpp__

#include "GEPException.hpp"
#include "GEPRingBuffer.hpp"

#include <vector>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

namespace GEP {

  /* Thread safe random number generator */
  class Random
  {
  public:
    Random ();

    int getInt (int min, int max) const;
    double getDouble (double min, double max) const;

    template <class T>
    T selectElement (const std::vector<T>& values);

    template <class T>
    std::vector<T> selectSequence (const std::vector<T>& values,
				   unsigned int length=0);

  private:
    mutable boost::mt19937 _generator;
  };

  /*
   * Randomly select a single element from a vector of elements
   */
  template <class T>
  T Random::selectElement (const std::vector<T>& values)
  {
    if (values.size () == 0)
      throw InternalException ("Selection vector is empty");

    return values[getInt (0, values.size () - 1)];
  }

  /*
   * Randomly select an element sequence from a vector of elements
   */
  template <class T>
  std::vector<T> Random::selectSequence (const std::vector<T>& values,
					 unsigned int length)
  {
    if (values.size () == 0)
      throw InternalException ("Selection vector is empty");

    if (length == 0 || length > values.size ())
      length = values.size ();

    RingBuffer<T> buffer (values);
    return buffer.get (getInt (0, values.size () - 1), getInt (1, length));
  }
  
} // namespace GEP

#endif
