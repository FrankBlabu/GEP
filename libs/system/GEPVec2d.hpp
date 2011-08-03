/*
 * GEPVec2d.hpp - 2-dimensional vector class
 *
 * Frank Cieslok, 10.06.2006
 */

#ifndef __GEPVec2d_hpp__
#define __GEPVec2d_hpp__

namespace GEP {

  /*
   * 2-dimensional vector
   */
  class Vec2d
  {
  public:
    inline Vec2d ();
    inline Vec2d (double x, double y);
    inline Vec2d (const Vec2d& toCopy);

    inline const Vec2d& operator= (const Vec2d& toCopy);

    inline Vec2d operator+ (const Vec2d& toAdd);
    inline Vec2d operator- (const Vec2d& toAdd);

    inline const Vec2d& operator+= (const Vec2d& toAdd);
    inline const Vec2d& operator-= (const Vec2d& toAdd);

    double length () const;
    
  public:
    double _x;
    double _y;
  };


  //#*************************************************************************
  // Inline functions
  //#*************************************************************************
  
  /* Constructor */
  inline Vec2d::Vec2d ()
    : _x (0.0),
      _y (0.0)
  {
  }
  
  /* Constructor */
  inline Vec2d::Vec2d (double x, double y)
    : _x (x),
      _y (y)
  {
  }

  /* Copy constructor */
  inline Vec2d::Vec2d (const Vec2d& toCopy)
    : _x (toCopy._x),
      _y (toCopy._y)
  {
  }
  
  /* Assignment operator */
  inline const Vec2d& Vec2d::operator= (const Vec2d& toCopy)
  {
    _x = toCopy._x;
    _y = toCopy._y;

    return *this;
  }

  /* Addition operator */
  inline const Vec2d& Vec2d::operator+= (const Vec2d& toAdd)
  {
    _x += toAdd._x;
    _y += toAdd._y;
    return *this;
  }

  /* Subtraction operator */
  inline const Vec2d& Vec2d::operator-= (const Vec2d& toAdd)
  {
    _x -= toAdd._x;
    _y -= toAdd._y;
    return *this;
  }

  /* Addition operator */
  inline Vec2d Vec2d::operator+ (const Vec2d& toAdd)
  {
    return Vec2d (_x + toAdd._x, _y + toAdd._y);
  }

  /* Subtraction operator */
  inline Vec2d Vec2d::operator- (const Vec2d& toAdd)
  {
    return Vec2d (_x - toAdd._x, _y - toAdd._y);
  }
  
} // namespace GEP

#endif
