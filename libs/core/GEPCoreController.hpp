/*
 * GEPCoreController.hpp - Controller for a genetic optimization process
 *
 * Frank Cieslok, 04.06.2006
 */

#ifndef __GEPCoreController_hpp__
#define __GEPCoreController_hpp__

#include <GEPRandom.hpp>

#include <QtCore/QObject>
#include <vector>

namespace GEP
{
  namespace Core
  {

    class CrossoverOperator;
    class Individual;
    class MutationOperator;
    class Population;
    class PopulationCreator;
    class SelectionOperator;
    class TerminationCriterion;

    /*
     * Class for controlling a genetic optimization process
     */
    class Controller : public QObject
    {
      Q_OBJECT

    public:
      Controller ();
      ~Controller ();

      inline unsigned int getStep () const;
      inline bool getStopped () const;

      inline unsigned int getNumberOfPopulations () const;
      void setNumberOfPopulations (unsigned int n);

      inline unsigned int getNumberOfIndividuals () const;
      void setNumberOfIndividuals (unsigned int n);

      inline double getCrossoverProbability () const;
      void setCrossoverProbability (double value);

      inline const PopulationCreator* getPopulationCreator () const;
      void setPopulationCreator (const PopulationCreator* population_creator);

      typedef std::vector<const Population*>::const_iterator
      PopulationConstIterator;
      inline PopulationConstIterator getPopulationBegin () const;
      inline PopulationConstIterator getPopulationEnd () const;

      void addSelectionOperator (const SelectionOperator* func);
      void addCrossoverOperator (const CrossoverOperator* func);
      void addMutationOperator (const MutationOperator* func);
      void addTerminationCriterion (TerminationCriterion* criterion);

      bool initialize ();
      bool execute ();
      bool executeStep ();


    public slots:
      void slotStop ();

    signals:
      void signalStarted (const Core::Controller* controller);
      void signalStopped (const Core::Controller* controller);
      void signalStep (const Core::Controller* controller);
      void signalPopulationsChanged (const Core::Controller* controller);
      
    private:
      void executeNextStep ();
      bool checkTermination () const;
      void computeFitness ();

    private:
      unsigned int _number_of_populations;
      unsigned int _number_of_individuals;
      double _crossover_probability;

      //
      // Registered objects
      //
      const PopulationCreator* _population_creator;

      std::vector<const SelectionOperator*> _selection_operators;
      std::vector<const CrossoverOperator*> _crossover_operators;
      std::vector<const MutationOperator*> _mutation_operators;
      std::vector<TerminationCriterion*> _termination_criterions;

      //
      // State informations
      //
      std::vector<const Population*> _populations;

      unsigned int _step;
      bool _stopped;

      Random _random;
    };


    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Return the current optimization iteration */
    inline unsigned int Controller::getStep () const
    {
      return _step;
    }

    /* Return if the current controller execution has just been stopped */
    inline bool Controller::getStopped () const
    {
      return _stopped;
    }

    /* Return the number of parallel existing populations */
    inline unsigned int Controller::getNumberOfPopulations () const
    {
      return _number_of_populations;
    }

    /* Return the number of individuals per population */
    inline unsigned int Controller::getNumberOfIndividuals () const
    {
      return _number_of_individuals;
    }

    /* Return the crossover probability */
    inline double Controller::getCrossoverProbability () const
    {
      return _crossover_probability;
    }

    /* Return the population start iterator */
    inline Controller::PopulationConstIterator
    Controller::getPopulationBegin () const
    {
      return _populations.begin ();
    }

    /* Return the population end iterator */
    inline Controller::PopulationConstIterator
    Controller::getPopulationEnd () const
    {
      return _populations.end ();
    }

    /* Return the current population creator */
    inline const PopulationCreator* Controller::getPopulationCreator () const
    {
      return _population_creator;
    }

  } // namespace Core
} // namespace GEP

#endif
