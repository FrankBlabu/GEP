/*
 * GEPTree.hpp - Generic tree individual
 *
 * Frank Cieslok, 15.07.2006
 */

#ifndef __GEPTree_hpp__
#define __GEPTree_hpp__

#include "GEPTreeValue.hpp"
#include <GEPCoreIndividual.hpp>

#include <vector>

namespace GEP {
  namespace Tree {

    class Executer;
    class NodeCreator;
    
    /*
     * Tree node base class
     */
    class Node
    {
    public:
      Node (const NodeCreator* creator);
      Node (const Node& toCopy);
      virtual ~Node ();

      inline const NodeCreator* getCreator () const;
      inline const std::vector<Node*>& getSources () const;
      
      virtual Node* clone () const = 0;
      virtual Value execute (const Executer* executer) const = 0;
      virtual QString toString () const = 0;

      void addSource (Node* source);
      void removeSource (Node* source);
      
    private:
      const NodeCreator* _creator;
      std::vector<Node*> _sources;
    };

    /*
     * Tree node creator
     */
    class NodeCreator
    {
    public:
      NodeCreator (const QString& type_name, unsigned int number_of_sources);
      virtual ~NodeCreator ();

      virtual Node* create () const = 0;

      inline const QString& getTypeName () const;
      inline unsigned int getNumberOfSources () const;
      
      inline const std::vector<Value::Type_t>& getSourceTypes () const;
      inline Value::Type_t getTargetType () const;

    protected:
      void setTypes (const std::vector<Value::Type_t>& source_types,
		     Value::Type_t target_type);
      
    private:
      QString _type_name;
      unsigned int _number_of_sources;

      std::vector<Value::Type_t> _source_types;
      Value::Type_t _target_type;
    };
    
    /*
     * Generic tree individual
     */
    class Tree : public GEP::Core::Individual
    {
    public:
      Tree ();
      Tree (const Tree& toCopy);
      virtual ~Tree ();

      inline const Node* getRoot () const;
      void setRoot (Node* node);
      
      virtual Tree* clone () const;
      QString toString () const;
      
    private:
      Node* _root;
    };

    /*
     * Creator for tree objects
     */
    class TreeCreator
    {
    public:
      TreeCreator ();
      virtual ~TreeCreator ();

      void addNodeCreator (NodeCreator* creator);
      void removeNodeCreator (NodeCreator* creator);

    private:
      std::vector<NodeCreator*> _node_creators;
    };
 
    
    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Return the creator which created this node */
    inline const NodeCreator* Node::getCreator () const
    {
      return _creator;
    }

    /* Return sources of this node */
    inline const std::vector<Node*>& Node::getSources () const
    {
      return _sources;
    }
    
    /* Return the type name of nodes creator by this creator */
    inline const QString& NodeCreator::getTypeName () const
    {
      return _type_name;
    }
    
    /* Return the number of source nodes creator by this creator */
    inline unsigned int NodeCreator::getNumberOfSources () const
    {
      return _number_of_sources;
    }

    /* Return the required data types of the source nodes */
    inline const std::vector<Value::Type_t>& NodeCreator::getSourceTypes ()
      const
    {
      return _source_types;
    }

    /* Return the generated data type */
    inline Value::Type_t NodeCreator::getTargetType () const
    {
      return _target_type;
    }

    /* Return the trees root node */
    inline const Node* Tree::getRoot () const
    {
      return _root;
    }
    
  } // namespace Tree
} // namespace GEP

#endif
