/*
 * tree.cpp - Generic tree individual
 *
 * Frank Cieslok, 15.07.2006
 */

#include "GEPTree.hpp"

namespace GEP {
  namespace Tree {

    //#***********************************************************************
    // CLASS GEP::Tree::Node
    //#***********************************************************************

    /* Constructor */
    Node::Node (const NodeCreator* creator)
      : _creator (creator)
    {
    }

    /* Copy constructor */
    Node::Node (const Node& toCopy)
      : _creator (toCopy._creator)
    {
      for (unsigned int i=0; i < toCopy._sources.size (); ++i)
	_sources.push_back (toCopy._sources[i]->clone ());
    }

    /* Destructor */
    Node::~Node ()
    {
      for (unsigned int i=0; i < _sources.size (); ++i)
	delete _sources[i];

      _sources.clear ();
    }

    /*
     * Add a new source node to this node
     *
     * \param source Node to be added. The added node is owned by this node
     *               afterwards and will be deleted by it later.
     */
    void Node::addSource (Node* source)
    {
      for (unsigned int i=0; i < _sources.size (); ++i)
	if (_sources[i] == source)
	  throw InternalException ("The same node has been added twice");

      _sources.push_back (source);
    }

    /*
     * Remove a source node
     */
    void Node::removeSource (Node* source)
    {
      std::vector<Node*>::iterator pos = _sources.begin ();

      while (pos != _sources.end () && *pos != source)
	++pos;

      if (pos == _sources.end ())
	throw InternalException ("Node to remove can not be found");

      delete *pos;
      _sources.erase (pos);
    }

    
    
    //#***********************************************************************
    // CLASS GEP::Tree::NodeCreator
    //#***********************************************************************

    /* Constructor */
    NodeCreator::NodeCreator (const QString& type_name,
			      unsigned int number_of_sources)
      : _type_name         (type_name),
	_number_of_sources (number_of_sources)
    {
    }

    /* Destructor */
    NodeCreator::~NodeCreator ()
    {
    }

    /*
     * Set source and target data types required by nodes created by this
     * creator object.
     *
     * This function must be called once in the creators constructor.
     */
    void NodeCreator::setTypes (const std::vector<Value::Type_t>& source_types,
				Value::Type_t target_type)
    {
      _source_types = source_types;
      _target_type = target_type;
    }

    
    //#***********************************************************************
    // CLASS GEP::Function::Tree
    //#***********************************************************************

    /* Constructor */
    Tree::Tree ()
      : Core::Individual (),
	_root (0)
    {
    }

    /* Copy constructor */
    Tree::Tree (const Tree& toCopy)
      : Core::Individual (toCopy),
	_root (toCopy.getRoot () != 0 ? toCopy.getRoot ()->clone () : 0)
    {
    }

    /* Destructor */
    Tree::~Tree ()
    {
      if (_root != 0)
	delete _root;
      _root = 0;
    }

    /* Clone this tree */
    Tree* Tree::clone () const
    {
      return new Tree (*this);
    }

    /*
     * Set the trees root node
     *
     * \param node Node acting as root node and entry point for the tree. The
     *             node is owned by the tree afterwards and will be deleted
     *             by it automatically later.
     */
    void Tree::setRoot (Node* node)
    {
      if (_root != 0)
	delete _root;

      _root = node;
    }
    
    /*
     * Generate a string representation of the whole tree
     */
    QString Tree::toString () const
    {
      return _root != 0 ? _root->toString () : QString ("empty");
    }
    

    //#***********************************************************************
    // CLASS GEP::Function::TreeCreator
    //#***********************************************************************

    /*! Constructor */
    TreeCreator::TreeCreator ()
    {
    }

    /*! Destructor */
    TreeCreator::~TreeCreator ()
    {
      for (unsigned int i=0; i < _node_creators.size (); ++i)
	delete _node_creators[i];
      _node_creators.clear ();
    }

    /*!
     * Add node creator instance
     *
     * \param creator Creator instance to be added. The object is owned
     *                by the tree creator class afterwards and will be
     *                destroyed by it later.
     */
    void TreeCreator::addNodeCreator (NodeCreator* creator)
    {
      for (unsigned int i=0; i < _node_creators.size (); ++i)
	if (_node_creators[i] == creator)
	  throw InternalException ("Node creator object added twice");

      _node_creators.push_back (creator);
    }
    
    /*!
     * Remove previously registered node creator instance
     */
    void TreeCreator::removeNodeCreator (NodeCreator* creator)
    {
      std::vector<NodeCreator*>::iterator pos = _node_creators.begin ();

      while (pos != _node_creators.end () && *pos != creator)
	++pos;

      if (pos == _node_creators.end ())
	throw InternalException ("Creator object to be removes was never "
				 "registered");

      delete *pos;
      _node_creators.erase (pos);
    }


    
  } // namespace Tree
} // namespace GEP
