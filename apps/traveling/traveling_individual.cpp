/*
 * traveling_individual.cpp - Individual objects for the traveling salesman
 *                            test application
 *
 * Frank Cieslok, 03.06.2006
 */

#include "GEPTravelingIndividual.hpp"

#include <GEPDebug.hpp>
#include <GEPException.hpp>
#include <GEPRandom.hpp>

#include <math.h>

namespace GEP {
  namespace Traveling {

    //#***********************************************************************
    // CLASS GEP::Traveling::City
    //#***********************************************************************

    /* Constructor */
    City::City (double x, double y)
      : _point (x, y)
    {
    }

    /* Copy-Constructor */
    City::City (const City& toCopy)
      : _point (toCopy._point)
    {
    }

    /* Compute distance to another city */
    double City::distance (const City& city) const
    {
      const Vec2d& p1 = getPoint ();
      const Vec2d& p2 = city.getPoint ();
      return sqrt ((p1._x - p2._x) * (p1._x - p2._x) +
		   (p1._y - p2._y) * (p1._y - p2._y));
    }


    //#***********************************************************************
    // CLASS GEP::Traveling::World
    //#***********************************************************************

    /* Minimum/maximum coordinates [STATIC] */
    const double World::MIN_COORDINATE = 0.0;
    const double World::MAX_COORDINATE = 1000.0;

    /* Constructor */
    World::World (unsigned int number_of_cities)
      : _random ()
    {
      for (unsigned int i=0; i < number_of_cities; ++i)
	_cities.push_back 
	  (City (_random.getDouble (MIN_COORDINATE, MAX_COORDINATE),
		 _random.getDouble (MIN_COORDINATE, MAX_COORDINATE)));
    }

    /* Return a single city */
    const City& World::operator[] (unsigned int index) const
    {
      if (index >= _cities.size ())
	throw InternalException ("Index exceeds world size");

      return _cities[index];
    }


    //#***********************************************************************
    // CLASS GEP::Traveling::Route
    //#***********************************************************************

    /*
     * Constructor
     *
     * Constructs a valid random route
     */
    Route::Route (const World* world)
      : Core::RingIndividual (),
	_world (world)
    {
    }

    /*
     * Copy constructor
     */
    Route::Route (const Route& toCopy)
      : Core::RingIndividual (toCopy),
	_world  (toCopy._world)
    {
    }

    /* Destructor */
    Route::~Route ()
    {
    }

    /* Clone this individual */
    GEP::Core::Individual* Route::clone () const
    {
      return new Route (*this);
    }

    /* Compute the individuals fitness in form of the overall distance*/
    double Route::computeDistance ()
    {
      _distance = 0.0;

      const Chromosome& cities = getChromosome ();

      for (unsigned int i=1; i < cities.size (); ++i)
	_distance += (*_world)[cities[i - 1]].distance ((*_world)[cities[i]]);

      return _distance;
    }

  } // namespace Traveling
} // namespace GEP
