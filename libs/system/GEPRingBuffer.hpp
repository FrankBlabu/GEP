/*
 * GEPRingBuffer.hpp - Ringbuffer container
 *
 * Frank Cieslok, 04.06.2006
 */

#ifndef __GEPRingBuffer_hpp__
#define __GEPRingBuffer_hpp__

#include "GEPException.hpp"
#include <vector>

namespace GEP {

  /*
   * Container for cyclic element access
   */
  template <class T>
  class RingBuffer : public std::vector<T>
  {
  public:
    RingBuffer ();
    RingBuffer (const std::vector<T>& values);

    std::vector<T> get (unsigned int index, unsigned int length) const;
  };

  /* Constructor */
  template <class T>
  RingBuffer<T>::RingBuffer ()
  {
  }

  /* Constructor */
  template <class T>
  RingBuffer<T>::RingBuffer (const std::vector<T>& values)
    : std::vector<T> (values)
  {
  }

  /* Get a sequence from the buffer */
  template <class T>
  std::vector<T> RingBuffer<T>::get (unsigned int index, unsigned int length)
    const
  {
    if (length < 1)
      throw InternalException ("Null length not allowed in ringbuffer "
			       "operation");
    
    std::vector<T> result (length);

    for (unsigned int i=0; i < length; ++i)
      result[i] = (*this)[(index + i) % this->size ()];
    
    return result;
  }

} // namespace GEP

#endif
