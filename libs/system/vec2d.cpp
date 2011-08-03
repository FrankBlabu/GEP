/*
 * vec2d.hpp - 2-dimensional vector class
 *
 * Frank Cieslok, 10.06.2006
 */

#include "GEPVec2d.hpp"

#include <math.h>

namespace GEP {

  //#*************************************************************************
  // CLASS GEP::Vec2d
  //#*************************************************************************
  
  /* Compute the length of this vector */
  double Vec2d::length () const
  {
    return sqrt (_x * _x + _y * _y);
  }

} // namespace GEP
