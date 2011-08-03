/*
 * core_controller.hpp - Controller for a genetic optimization process
 *
 * Frank Cieslok, 04.06.2006
 */

//#define _DEBUG

#include "GEPCoreController.hpp"
#include "GEPCoreCriterion.hpp"
#include "GEPCoreCrossover.hpp"
#include "GEPCoreIndividual.hpp"
#include "GEPCoreMutation.hpp"
#include "GEPCorePopulation.hpp"
#include "GEPCoreSelection.hpp"

#include <GEPDebug.hpp>
#include <GEPException.hpp>

#include <memory>
#include <set>


namespace GEP
{
  namespace Core
  {
    //#***********************************************************************
    // CLASS GEP::Core::Controller
    //#***********************************************************************

    /* Constructor */
    Controller::Controller ()
      : _number_of_populations (0),
	_number_of_individuals (0),
	_crossover_probability (0.5),
	_population_creator    (0),
	_step                  (0),
	_stopped               (false)
    {
    }

    /* Destructor */
    Controller::~Controller ()
    {
      //
      // Delete populations
      //
      for (unsigned int i=0; i < _populations.size (); ++i)
        delete _populations[i];

      _populations.clear ();
      emit signalPopulationsChanged (this);

      //
      // Delete registered operators and criterions
      //
      for (unsigned int i=0; i < _selection_operators.size (); ++i)
        delete _selection_operators[i];
      _selection_operators.clear ();

      for (unsigned int i=0; i < _crossover_operators.size (); ++i)
        delete _crossover_operators[i];
      _crossover_operators.clear ();

      for (unsigned int i=0; i < _mutation_operators.size (); ++i)
        delete _mutation_operators[i];
      _mutation_operators.clear ();

      for (unsigned int i=0; i < _termination_criterions.size (); ++i)
        delete _termination_criterions[i];
      _termination_criterions.clear ();

      //
      // Delete registered creators
      //
      delete _population_creator;
      _population_creator = 0;
    }

    /* Set the number of parallel existing populations per step */
    void Controller::setNumberOfPopulations (unsigned int n)
    {
      _number_of_populations = n;
    }

    /* Set the number of individuals per population */
    void Controller::setNumberOfIndividuals (unsigned int n)
    {
      _number_of_individuals = n;
    }

    /*
     * Set the crossover probability
     */
    void Controller::setCrossoverProbability (double value)
    {
      if (value < 0.0 || value > 1.0)
        throw InternalException ("Illegal crossover probability value");

      _crossover_probability = value;
    }

    /*
     * Set the population creator
     *
     * \param population_creator Creator to add. The controller owns the
     *                           creator object and will destroy it upon
     *                           deletion.
     */
    void Controller::setPopulationCreator
    (const PopulationCreator* population_creator)
    {
      if (population_creator == 0)
        throw InternalException ("Population creator is NULL");

      if (_population_creator != 0)
        delete _population_creator;

      _population_creator = population_creator;
    }

    /*
     * Register selection operator
     *
     * \param func Operator to add. The controller owns the operator object
     *             afterwards and will destroy it upon deletion.
     */
    void Controller::addSelectionOperator (const SelectionOperator* func)
    {
      DV ("* Adding selection operator, name=" << func->getDescription ());

      for (unsigned int i=0; i < _selection_operators.size (); ++i)
        if (_selection_operators[i] == func)
          throw InternalException ("Selection operator already registered");

      _selection_operators.push_back (func);
    }

    /*
     * Register crossover operator
     *
     * \param func Operator to add. The controller owns the operator object
     *             afterwards and will destroy it upon deletion.
     */
    void Controller::addCrossoverOperator (const CrossoverOperator* func)
    {
      DV ("* Adding crossover operator, name=" << func->getDescription ());

      for (unsigned int i=0; i < _crossover_operators.size (); ++i)
        if (_crossover_operators[i] == func)
          throw InternalException ("Crossover operator already registered");

      _crossover_operators.push_back (func);
    }

    /*
     * Register mutation operator
     *
     * \param func Operator to add. The controller owns the operator object
     *             afterwards and will destroy it upon deletion.
     */
    void Controller::addMutationOperator (const MutationOperator* func)
    {
      DV ("* Adding mutation operator, name=" << func->getDescription ());

      for (unsigned int i=0; i < _mutation_operators.size (); ++i)
        if (_mutation_operators[i] == func)
          throw InternalException ("Mutation operator already registered");

      _mutation_operators.push_back (func);
    }

    /*
     * Register termination criterion
     *
     * \param criterion Criterion to add. The controller owns the registered
     *                  object and will destroy it upon deletion
     */
    void Controller::addTerminationCriterion (TerminationCriterion* criterion)
    {
      for (unsigned int i=0; i < _termination_criterions.size (); ++i)
        if (_termination_criterions[i] == criterion)
          throw InternalException ("Termination criterion already registered");

      _termination_criterions.push_back (criterion);
    }


    /*
     * Initialize execution
     *
     * \return 'true', if there are more steps to process
     */
    bool Controller::initialize ()
    {
      _step = 0;
      _stopped = false;

      //
      // Check if all necessary parameters and operators have been set
      //
      if (_population_creator == 0)
        throw InternalException ("Population creator object not set");

      if (_number_of_populations == 0)
        throw InternalException ("Population size not set");

      if (_number_of_individuals == 0)
        throw InternalException ("Number of individuals not set");

      if (_selection_operators.size () < 1)
        throw InternalException ("No selection operators have been set");

      if (_crossover_operators.size () < 1)
        throw InternalException ("No crossover operators have been set");

      if (_mutation_operators.size () < 1)
        throw InternalException ("No mutation operators have been set");

      if (_termination_criterions.size () < 1)
        throw InternalException ("No termination criterions have been set");

      //
      // Clear old data
      //
      for (unsigned int i=0; i < _populations.size (); ++i)
        delete _populations[i];

      _populations.clear ();

      //
      // Initialize all populations
      //
      for (unsigned int i=0; i < _number_of_populations; ++i)
        _populations.push_back (_population_creator->create
                                (_number_of_individuals));

      computeFitness ();
      emit signalPopulationsChanged (this);

      DV ("* Initialized, " << _populations.size () << " populations");

      return !checkTermination ();
    }

    /*
     * Execute genetic optimization process
     *
     * \return 'true', if all necessary steps have been processed. Otherwise
     *         another execution function has to be called to continue
     *         the computation.
     */
    bool Controller::execute ()
    {
      bool finished = false;

      _stopped = false;
      emit signalStarted (this);

      while (!finished && !_stopped)
	{
	  executeNextStep ();
	  emit signalStep (this);
	  finished = checkTermination ();
	}
      
      emit signalStopped (this);
      return !finished;
    }

    /*
     * Execute single step in the optimization process
     *
     * \return 'true', if all necessary steps have been processed. Otherwise
     *         another execution function has to be called to continue
     *         the computation.
     */
    bool Controller::executeStep ()
    {
      emit signalStarted (this);
      executeNextStep ();
      emit signalStopped (this);

      return !checkTermination ();
    }

    /*
     * Execute the next step of the optimization process
     *
     * \return 'true', if there are more steps to process
     */
    void Controller::executeNextStep ()
    {
      _step++;

      DV ("* Step " << _step);

      std::vector<Population*> target_populations;

      for (unsigned int i=0; i < _populations.size (); ++i)
	{
	  const Population* source_population = _populations[i];

	  //
	  // Step 1: Select individuals from the last step
	  //
	  std::auto_ptr<Population> selected_population
	    (_population_creator->create (0));

	  const SelectionOperator* selection_operator =
	    _random.selectElement (_selection_operators);
	  selection_operator->select (source_population,
				      selected_population.get ());

	  DV ("  Selection, selected population size=" <<
	      selected_population->size ());

	  //
	  // Step 2: Crossover
	  //
	  std::auto_ptr<Population> crossover_population
	    (_population_creator->create (0));

	  for (unsigned int j=0; j < selected_population->size () / 2; ++j)
	    {
	      const Individual* individual_1 = (*selected_population)[j * 2];
	      const Individual* individual_2 = 
		(*selected_population)[j * 2 + 1];

	      if (_random.getDouble (0.0, 1.0) < _crossover_probability)
		{
		  const CrossoverOperator* crossover_operator =
		    _random.selectElement (_crossover_operators);
		  crossover_operator->crossover (individual_1, individual_2,
						 crossover_population.get ());
		}
	      else
		{
		  crossover_population->addIndividual (individual_1->clone ());
		  crossover_population->addIndividual (individual_2->clone ());
		}
	    }

	  DV ("  Crossover, crossover population size=" <<
	      crossover_population->size ());

	  //
	  // Step 3: Mutation
	  //
	  std::auto_ptr<Population> target_population
	    (_population_creator->create (0));

	  for ( Population::IndividualConstIterator j =
		  crossover_population->getIndividualBegin ();
		j != crossover_population->getIndividualEnd (); ++j )
	    {
	      const Individual* source = *j;
	      const MutationOperator* mutation_operator =
		_random.selectElement (_mutation_operators);

	      target_population->addIndividual
		(mutation_operator->mutate (source));
	    }

	  DV ("  Mutation, target population size=" <<
	      target_population->size ());

	  //
	  // Step 4: Replace the old population by the new one
	  //
	  _populations[i] = target_population.release ();
	  delete source_population;
	}

      computeFitness ();
      emit signalPopulationsChanged (this);
    }

    /*
     * Compute state information
     */
    void Controller::computeFitness ()
    {
      //
      // Compute the population fitness for all existing populations
      //
      for (unsigned int i=0; i < _populations.size (); ++i)
	{
	  const Population* population = _populations[i];
	  population->computeFitness ();
	}
    }

    /*
     * Check if a termination criterion has been satisfied
     */
    bool Controller::checkTermination () const
    {
      bool finished = false;

      for ( unsigned int i=0; i < _termination_criterions.size () && !finished;
            ++i )
        finished = _termination_criterions[i]->finished (this);

      return finished;
    }

    /*
     * Called if the current controller execution should be stopped at the
     * next possible occasion
     */
    void Controller::slotStop ()
    {
      _stopped = true;
    }

  } // namespace Core
} // namespace GEP
