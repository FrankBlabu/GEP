/*
 * display_population_list.cpp - List widget displaying a population object
 *
 * Frank Cieslok, 10.06.2006
 */

#include "GEPDisplayPopulationList.hpp"

#include <GEPCoreController.hpp>
#include <GEPCoreIndividual.hpp>
#include <GEPCorePopulation.hpp>
#include <GEPDebug.hpp>
#include <GEPException.hpp>

#include <QtCore/QAbstractItemModel>
#include <QtGui/QItemSelectionModel>

#include <algorithm>
#include <vector>

namespace GEP {
  namespace Display {

    
    //#***********************************************************************
    // CLASS GEP::Display::PopulationListModel
    //#***********************************************************************

    /* Konstruktor */
    PopulationListInterface::PopulationListInterface ()
    {
    }

    /* Destruktor */
    PopulationListInterface::~PopulationListInterface ()
    {
    }


    //#***********************************************************************
    // CLASS GEP::Display::PopulationListModel
    //#***********************************************************************

    /*
     * Data model for a population
     */
    class PopulationListModel : public QAbstractItemModel
    {
    public:
      PopulationListModel (QObject* parent);
      virtual ~PopulationListModel ();

      void setPopulation (const Core::Population* population);
      Core::Individual* getIndividual (const QModelIndex& index) const;

      virtual int rowCount (const QModelIndex& parent) const;
      virtual int columnCount (const QModelIndex& parent) const;

      virtual Qt::ItemFlags flags(const QModelIndex& index) const;
      virtual QModelIndex index (int row, int column,
				 const QModelIndex& parent) const;
      virtual QModelIndex parent (const QModelIndex& index ) const;
      
      virtual QVariant data (const QModelIndex& index, int role) const;
      virtual QVariant headerData (int section, Qt::Orientation orientation,
				   int role) const;

    private:
      const Core::Population* _population;

      std::vector<Core::Individual*> _individuals;
      
      static const int COLUMN_INDEX = 0;
      static const int COLUMN_FITNESS = 1;
    };

    /* Constructor */
    PopulationListModel::PopulationListModel (QObject* parent)
      : QAbstractItemModel (parent),
	_population (0)
    {
    }

    /* Destructor */
    PopulationListModel::~PopulationListModel ()
    {
    }

    /*
     * Return the individual belonging to the given index
     */
    Core::Individual* PopulationListModel::getIndividual
    (const QModelIndex& index) const
    {
      if ( !index.isValid () ||
	   index.row () >= static_cast<int> (_individuals.size ()) )
	throw InternalException ("Illegal individual index");
      
      return _individuals[index.row ()];
    }
    
    /*
     * Set population to be displayed
     */
    void PopulationListModel::setPopulation
    (const Core::Population* population)
    {
      _population = population;

      _individuals.clear ();
      
      if (_population != 0)
      {
	for ( Core::Population::IndividualConstIterator i =
		_population->getIndividualBegin ();
	      i != _population->getIndividualEnd (); ++i )
	  _individuals.push_back (*i);

	std::sort (_individuals.begin (), _individuals.end (),
		   Core::IndividualFitnessComparator ());
      }

      emit layoutChanged ();
    }

    /*
     * Get number of rows
     */
    int PopulationListModel::rowCount (const QModelIndex& parent) const
    {
      return parent.isValid () ? 0 :_individuals.size ();
    }
    
    /*
     * Get number of columns
     */
    int PopulationListModel::columnCount (const QModelIndex& parent) const
    {
      int columns = 0;

      if (!parent.isValid ())
	{
	  columns = 2;

	  const PopulationListInterface* pi =
	    dynamic_cast<const PopulationListInterface*> (_population);
	  if (pi != 0)
	    columns += pi->getAdditionalListColumns ().size ();
	}

      return columns;
    }

    /*
     * Get item flags
     */
    Qt::ItemFlags PopulationListModel::flags (const QModelIndex& index) const
    {
      return !index.isValid() ?
	Qt::ItemIsEnabled : Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    /*
     * Get model index of a <row,column,parent>-Triple
     */
    QModelIndex PopulationListModel::index
    (int row, int column, const QModelIndex& parent) const
    {
      QModelIndex result;

      if (!parent.isValid())
	result = createIndex (row, column, 0);

      return result;
    }
    
    /*
     * Return the parent of a model index
     */
    QModelIndex PopulationListModel::parent (const QModelIndex& /*index*/)
      const
    {
      return QModelIndex ();
    }

    /*
     * Get displayed data
     */
    QVariant PopulationListModel::data (const QModelIndex& index, int role)
      const
    {
      QVariant result;

      if ( index.isValid () &&
	   index.row () < static_cast<int> (_individuals.size ()) )
      {
	if (role == Qt::DisplayRole)
	{
	  switch (index.column ())
	    {
	      case COLUMN_INDEX:
		result = index.row () + 1;
		break;

	      case COLUMN_FITNESS:
		result = _individuals[index.row ()]->getFitness ();
		break;

	      default:
		{
		  const PopulationListInterface* pi =
		    dynamic_cast<const PopulationListInterface*>
		    (_population);
		  if ( pi != 0 &&
		       index.column () > COLUMN_FITNESS &&
		       index.column () <= COLUMN_FITNESS +
		       static_cast<int>
		       (pi->getAdditionalListColumns ().size ()) )
		    result = pi->getListColumnContent 
		      (_individuals[index.row ()],
		       index.column () - COLUMN_FITNESS - 1);
		  else
		    throw InternalException ("Unhandled model column");
		}
		break;
	    }
	}
      }
      
      return result;
    }

    /*
     * Get header text
     */
    QVariant PopulationListModel::headerData
    (int section, Qt::Orientation /*orientation*/, int role) const
    {
      QVariant result;

      if (role == Qt::DisplayRole)
      {
	switch (section)
	{
	  case COLUMN_INDEX:
	    result = QString ("Index");
	    break;

	  case COLUMN_FITNESS:
	    result = QString ("Fitness");
	    break;

	  default:
	    {
	      const PopulationListInterface* pi =
		dynamic_cast<const PopulationListInterface*>
		(_population);
	      if (pi != 0)
		{
		  std::vector<QString> names =
		    pi->getAdditionalListColumns ();
		  if ( section > COLUMN_FITNESS &&
		       section <= COLUMN_FITNESS + 
		       static_cast<int> (names.size ()) )
		    result = QVariant (names[section - COLUMN_FITNESS - 1]);
		  else
		    throw InternalException ("Unhandled model column");
		}
	      else
		throw InternalException ("Unhandled model column");
	    }
	    break;
	}
      }
      
      return result;
    }
    
    
    //#***********************************************************************
    // CLASS GEP::Display::PopulationList
    //#***********************************************************************

    /* Constructor */
    PopulationList::PopulationList (Core::Controller* controller,
				    QWidget* parent)
      : QTreeView (parent),
	_model      (new PopulationListModel (this)),
	_population (0)
    {
      setModel (_model);

      connect (controller,
	       SIGNAL (signalStarted (const Core::Controller*)),
	       SLOT (slotStarted (const Core::Controller*)));
      connect (controller,
	       SIGNAL (signalStopped (const Core::Controller*)),
	       SLOT (slotStopped (const Core::Controller*)));
      connect (controller,
	       SIGNAL (signalPopulationsChanged (const Core::Controller*)),
	       SLOT (slotPopulationsChanged (const Core::Controller*)));
      connect (selectionModel (), 
	       SIGNAL (selectionChanged (const QItemSelection&, 
					 const QItemSelection&)),
	       SLOT (slotSelectionChanged (const QItemSelection&, 
					   const QItemSelection&)));
    }

    /* Destructor */
    PopulationList::~PopulationList ()
    {
    }

    /*
     * Set population to be displayed
     */
    void PopulationList::setPopulation (const Core::Population* population)
    {
      _population = population;
      _model->setPopulation (population);
      
      update ();
    }

    /*!
     * Called when the controller has been started
     */
    void PopulationList::slotStarted (const Core::Controller* /*controller*/)
    {
      selectionModel ()->clearSelection ();
    }

    /*!
     * Called when the controller has been stopped
     */
    void PopulationList::slotStopped (const Core::Controller* /*controller*/)
    {
    }

    /*!
     * Called when a controllers population changed
     */
    void PopulationList::slotPopulationsChanged 
    (const Core::Controller* controller)
    {
      if ( controller->getPopulationBegin () !=
	   controller->getPopulationEnd () )
	setPopulation (*controller->getPopulationBegin ());
      else
	setPopulation (0);
    }

    /*!
     * Called when the list item selection has been changed
     */
    void PopulationList::slotSelectionChanged 
    (const QItemSelection& selected_items,
     const QItemSelection& deselected_items)
    {
      if (_population == 0)
	throw InternalException ("Population not set, but selection changed");

      QModelIndexList selected = selected_items.indexes ();
      for (int i=0; i < selected.count (); ++i)
	_model->getIndividual (selected[i])->setSelected (true);
      
      QModelIndexList deselected = deselected_items.indexes ();
      for (int i=0; i < deselected.count (); ++i)
	_model->getIndividual (deselected[i])->setSelected (false);

      emit signalSelectionChanged ();
    }
    
  } // namespace Display
} // namespace GEP
