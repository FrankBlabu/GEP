/*
 * GEPTravelingPopulation.hpp - Population object for the traveling salesman
 *                              scenario
 *
 * Frank Cieslok, 10.06.2006
 */

#ifndef __GEPTravelingPopulation_hpp__
#define __GEPTravelingPopulation_hpp__

#include <GEPCorePopulation.hpp>
#include <GEPDisplayPopulationList.hpp>
#include <GEPDisplayPopulationWidget.hpp>
#include <GEPRandom.hpp>
#include <GEPVec2d.hpp>

#include <QtCore/QPoint>

namespace GEP {

  namespace Core {
    class IndividualCreator;
    class Population;
  }
  
  namespace Traveling {

    class World;
    
    /*
     * Population object for the traveling salesman scenario
     */
    class RoutePopulation : public Core::Population,
			    public Display::PopulationDisplayInterface,
			    public Display::PopulationListInterface
    {
    public:
      RoutePopulation (const World* world);
      virtual ~RoutePopulation ();
      
      virtual void computeFitness () const;
      
      //
      // Functions fuer the population display interface
      //
      virtual std::vector<const Core::Individual*> getIndividuals () const;
      virtual void drawWorld (QPainter* painter) const;
      virtual void drawIndividual (QPainter* painter,
				   const Core::Individual* individual) const;

      //
      // Functions for the population list interface
      //
      virtual std::vector<QString> getAdditionalListColumns () const;
      virtual QVariant getListColumnContent 
      (const Core::Individual* individual, unsigned int column) const;

    private:
      QPoint computeWidgetCoordinates (const QPainter* painter,
				       const Vec2d& point) const;

    private:
      const World* _world;

      static const int CITY_MARKER_SIZE = 10;
    };

    /*
     * Creator for a new population object
     */
    class RoutePopulationCreator : public Core::PopulationCreator
    {
    public:
      RoutePopulationCreator (const World* world);
      virtual ~RoutePopulationCreator ();

      virtual Core::Population* create (unsigned int size) const;

    private:
      const World* _world;
      Random _random;
    };
    
    
  } // namespace Traveling
} // namespace GEP

#endif
