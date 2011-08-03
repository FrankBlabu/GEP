/*
 * traveling_population.cpp - Population object for the traveling salesman
 *                            scenario
 *
 * Frank Cieslok, 10.06.2006
 */

//#define _DEBUG

#include "GEPTravelingPopulation.hpp"
#include "GEPTravelingIndividual.hpp"

#include <GEPDebug.hpp>
#include <GEPRandom.hpp>

#include <QtGui/QPainter>
#include <QtGui/QPolygon>

#include <boost/format.hpp>

#include <algorithm>
#include <map>
#include <math.h>

namespace GEP {
  namespace Traveling {

    //#***********************************************************************
    // CLASS GEP::Traveling::RoutePopulation
    //#***********************************************************************
    
    /* Constructor */
    RoutePopulation::RoutePopulation (const World* world)
      : _world (world)
    {
    }

    /* Destructor */
    RoutePopulation::~RoutePopulation ()
    {
    }

    /* Get the populations individuals in sorted order */
    std::vector<const Core::Individual*> RoutePopulation::getIndividuals ()
      const
    {
      std::vector<const Core::Individual*> individuals;

      for ( IndividualConstIterator i = getIndividualBegin ();
	    i != getIndividualEnd (); ++i )
	individuals.push_back (*i);
      
      return individuals;
    }

    /*
     * Compute fitness of all individuals in the population
     */
    void RoutePopulation::computeFitness () const
    {
      //
      // Step 1: Compute route distance plus the minimum and maximum distance
      //         values 
      //
      typedef std::map<Route*, double> DistanceMap;
      DistanceMap distances;

      double min_distance = std::numeric_limits<double>::max ();
      double max_distance = std::numeric_limits<double>::min ();
      
      for ( IndividualConstIterator i = getIndividualBegin ();
	    i != getIndividualEnd (); ++i )
      {
	Route* route = dynamic_cast<Route*> (*i);
	double distance = route->computeDistance ();

	distances.insert (std::make_pair (route, distance));

	min_distance = std::min (min_distance, distance);
	max_distance = std::max (max_distance, distance);
      }

      //
      // Step 2: Set individual fitness to the normalized fitness value in
      //         interval [0.0, 1.0]
      //
      for ( DistanceMap::const_iterator i = distances.begin ();
	    i != distances.end (); ++i )
      {
	Route* route = i->first;
	double distance = i->second;

	double fitness = 0.0;
	if (max_distance != min_distance)
	  fitness = 1.0 - (distance - min_distance) / 
	    (max_distance - min_distance);
	else
	  fitness = 1.0;
	  
	fitness = std::max (0.0, fitness);
	fitness = std::min (1.0, fitness);
	
	route->setFitness (fitness);
      }

#ifdef _DEBUG
      std::cout << boost::format ("%1$-5s %2$-20s %3$-20s") % "Index"
	% "Distance" % "Fitness"
		<< std::endl;
      std::cout << std::string (5+1+20+1+20, '-') << std::endl;

      std::vector<Route*> sorted_routes;
      for ( DistanceMap::const_iterator i = distances.begin ();
	    i != distances.end (); ++i )
	sorted_routes.push_back (i->first);

      std::sort (sorted_routes.begin (), sorted_routes.end (),
		 Core::IndividualFitnessComparator ());

      double fitness_sum = 0.0;
      for (unsigned int i=0; i < sorted_routes.size (); ++i)
      {
	Route* route = sorted_routes[i];
	fitness_sum += route->getFitness ();
	
	DistanceMap::const_iterator pos = distances.find (route);
	if (pos == distances.end ())
	  throw InternalException ("Distance map corrupted");

	std::cout << boost::format ("%1$-5.5d %2$-20.4f %3$-20.4f")
	  % i
	  % pos->second
	  % route->getFitness ()
		  << std::endl;
      }

      std::cout << std::endl;
      std::cout << "Fitness sum: " << fitness_sum << std::endl;
#endif
    }

    /*
     * Draw the underlying world
     */
    void RoutePopulation::drawWorld (QPainter* painter) const
    {
      QPen pen (Qt::SolidLine);
      pen.setWidth (1);
      pen.setColor (Qt::red);
      painter->setPen (pen);
      painter->setBrush (QBrush (Qt::red, Qt::SolidPattern));
      
      for (unsigned int i=0; i < _world->size (); ++i)
	{
	  QPoint center = computeWidgetCoordinates
	    (painter, (*_world)[i].getPoint ());
	  painter->drawEllipse (center.x () - CITY_MARKER_SIZE / 2,
				center.y () - CITY_MARKER_SIZE / 2,
				CITY_MARKER_SIZE, CITY_MARKER_SIZE);
	}
    }

    /*
     * Draw a single individual of the population into the display widget
     */
    void RoutePopulation::drawIndividual
    (QPainter* painter, const Core::Individual* individual) const
    {
      QPen pen (Qt::SolidLine);
      pen.setWidth (1);
      
      const Route* route = dynamic_cast<const Route*> (individual);
      if (route != 0)
      {
	const World* world = route->getWorld ();
	const Route::Chromosome& cities = route->getChromosome ();

	//
	// Draw individual
	//
	QPolygon polygon (cities.size ());
	
	for (unsigned int i=0; i < cities.size (); ++i)
	  {
	    const Vec2d& point = (*world)[cities[i]].getPoint ();
	    polygon.setPoint (i, computeWidgetCoordinates (painter, point));
	  }
	
	pen.setColor (Qt::white);
	painter->setPen (pen);
	painter->setBrush (QBrush (Qt::SolidPattern));
	
	painter->drawPolyline (polygon);
      }
    }

    /*
     * Compute widget coordinates of a single point
     */
    QPoint RoutePopulation::computeWidgetCoordinates (const QPainter* painter,
						      const Vec2d& point) const
    {
      return QPoint (static_cast<int>
		     (round (painter->window ().x () +
			     point._x * painter->window ().width () /
			     (World::MAX_COORDINATE - 
			      World::MIN_COORDINATE))),
		     static_cast<int>
		     (round (painter->window ().y () +
			     point._y * painter->window ().height () /
			     (World::MAX_COORDINATE - 
			      World::MIN_COORDINATE))));
    }
    
    /*
     * Get names of the additional display list columns
     */
    std::vector<QString> RoutePopulation::getAdditionalListColumns () const
    {
      std::vector<QString> names;
      names.push_back ("Distance");
      return names;
    }

    /*
     * Get displayed value of the given additional list column
     */
    QVariant RoutePopulation::getListColumnContent 
    (const Core::Individual* individual, unsigned int column) const
    {
      QVariant value;

      const Route* route = dynamic_cast<const Route*> (individual);
      if (route == 0)
	throw InternalException ("Unknown individual object type");

      if (column == 0)
	value = QVariant (route->getDistance ());

      return value;
    }

    
    //#***********************************************************************
    // CLASS GEP::Traveling::RoutePopulationCreator
    //#***********************************************************************

    /* Constructor */
    RoutePopulationCreator::RoutePopulationCreator (const World* world)
      : _world (world)
    {
    }

    /* Destructor */
    RoutePopulationCreator::~RoutePopulationCreator ()
    {
    }

    /* Create a new population */
    Core::Population* RoutePopulationCreator::create (unsigned int size) const
    {
      RoutePopulation* population = new RoutePopulation (_world);

      for (unsigned int i=0; i < size; ++i)
      {
	Route* route = new Route (_world);

	std::vector<unsigned int> indices;
	for (unsigned int i=0; i < _world->size (); ++i)
	  indices.push_back (i);

	Route::Chromosome chromosome;
	while (indices.size () > 0)
	{
	  unsigned int pos = _random.getInt (0, indices.size () - 1);
	  chromosome.push_back (indices[pos]);
	  
	  for (unsigned int i=pos + 1; i < indices.size (); ++i)
	    indices[i - 1] = indices[i];
	  
	  indices.pop_back ();
	}
	
	route->setChromosome (chromosome);
	population->addIndividual (route);
      }

      return population;
    }
    
    
  } // namespace Traveling
} // namespace GEP
