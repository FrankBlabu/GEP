/*
 * GEPDisplayPopulationList.hpp - List widget displaying a population object
 *
 * Frank Cieslok, 10.06.2006
 */

#ifndef __GEPDisplayPopulationList_hpp__
#define __GEPDisplayPopulationList_hpp__

#include <QtCore/QVariant>
#include <QtGui/QTreeView>

class QItemSelection;

namespace GEP {

  namespace Core {
    class Controller;
    class Individual;
    class Population;
  }
  
  namespace Display {

    class PopulationListModel;
    
    /*
     * Interface implemented by the population displayed in the
     * PopulationList widget
     */
    class PopulationListInterface
    {
    public:
      PopulationListInterface ();
      virtual ~PopulationListInterface ();

      virtual std::vector<QString> getAdditionalListColumns () const = 0;
      virtual QVariant getListColumnContent 
      (const Core::Individual* individual, unsigned int column) const = 0;
    };

    /*
     * Widget displaying a list of the populations individuals properties
     */
    class PopulationList : public QTreeView
    {
      Q_OBJECT

    public:
      PopulationList (Core::Controller* controller, QWidget* parent=0);
      virtual ~PopulationList ();

      void setPopulation (const Core::Population* population);

    signals:
      void signalSelectionChanged ();

    private slots:
      void slotStarted (const Core::Controller* controller);
      void slotStopped (const Core::Controller* controller);
      void slotPopulationsChanged (const Core::Controller* controller);
      void slotSelectionChanged (const QItemSelection& selected, 
				 const QItemSelection& deselected);

    private:
      PopulationListModel* _model;
      const Core::Population* _population;
    };
    
  } // namespace Display
} // namespace GEP

#endif
