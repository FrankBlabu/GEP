/*
 * GEPTavelingIndividual.hpp - Individual objects for the traveling salesman
 *                             test application 
 *
 * Frank Cieslok, 10.06.2006
 */

#ifndef __GEPTravelingIndividual_hpp__
#define __GEPTravelingIndividual_hpp__

#include <GEPCoreRingIndividual.hpp>
#include <GEPRandom.hpp>
#include <GEPVec2d.hpp>

#include <vector>

namespace GEP {
  namespace Traveling {

    /*
     * Class describing a single city
     */
    class City
    {
    public:
      City (double x, double y);
      City (const City& toCopy);

      inline const Vec2d& getPoint () const;
      
      double distance (const City& city) const;
      
    private:
      Vec2d _point;
    };

    
    /*
     * Class describing the traveling salesmans world
     */
    class World
    {
    public:
      static const double MIN_COORDINATE;
      static const double MAX_COORDINATE;

    public:
      World (unsigned int number_of_cities);

      inline unsigned int size () const { return _cities.size (); }
      const City& operator[] (unsigned int index) const;
      
    private:
      typedef std::vector<City> Cities;
      Cities _cities;

      Random _random;
    };


    /*
     * Individual representing a single route
     */
    class Route : public GEP::Core::RingIndividual
    {
    public:
      Route (const World* world);
      Route (const Route& toCopy);
      virtual ~Route ();

      virtual GEP::Core::Individual* clone () const;
      
      inline const World* getWorld () const;
      inline double getDistance () const;

      double computeDistance ();
      
    private:
      const World* _world;
      double _distance;
    };

    
    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Return the position of the city */
    inline const Vec2d& City::getPoint () const
    {
      return _point;
    }

    /* Return the world belonging to this route */
    inline const World* Route::getWorld () const
    {
      return _world;
    }

    /* Return the last computed distance of this route */
    inline double Route::getDistance () const
    {
      return _distance;
    }
    
  } // namespace Traveling
} // namespace GEP

#endif
