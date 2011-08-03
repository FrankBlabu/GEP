/*
 * tree_function.cpp - Tree object representing a function
 *
 * Frank Cieslok, 04.08.2006
 */

#include "GEPTreeFunction.hpp"
#include "GEPTreeExecuter.hpp"

#include <GEPException.hpp>

#include <math.h>
#include <vector>

namespace GEP {
  namespace Tree {

    //#***********************************************************************
    // CLASS GEP::Tree::ConstantNode
    //#***********************************************************************
    
    /*! Constructor */
    ConstantNode::ConstantNode (const NodeCreator* creator,
				const Value& value)
      : Node (creator),
	_value (value)
    {
      if (value.getType () != Value::DOUBLE && value != Value::NONE)
	throw InternalException ("Constant must be a double");
    }

    /* Copy constructor */
    ConstantNode::ConstantNode (const ConstantNode& toCopy)
      : Node (toCopy),
	_value (toCopy._value)
    {
    }

    /* Destructor */
    ConstantNode::~ConstantNode ()
    {
    }

    /* Create a copy of this node */
    Node* ConstantNode::clone () const
    {
      return new ConstantNode (*this);
    }

    /* Compute current node value */
    Value ConstantNode::execute (const Executer* /*executer*/) const
    {
      return _value;
    }

    /* Set constant value */
    void ConstantNode::setValue (const Value& value)
    {
      if (value.getType () != Value::DOUBLE && value != Value::NONE)
	throw InternalException ("Constant must be a double");

      _value = value;
    }

    /* Return string representation of this node */
    QString ConstantNode::toString () const
    {
      return _value != Value::NONE ?
	QString ("%1").arg (_value.getDouble (), 8, 'f') : QString ("NaN");
    }

    
    //#***********************************************************************
    // CLASS GEP::Tree::ConstantNodeCreator
    //#***********************************************************************

    /* Constructor */
    ConstantNodeCreator::ConstantNodeCreator ()
      : NodeCreator ("constant", 0)
    {
      std::vector<Value::Type_t> source_types;
      setTypes (source_types, Value::DOUBLE);
    }

    /* Destructor */
    ConstantNodeCreator::~ConstantNodeCreator ()
    {
    }

    /* Create a node of this type */
    Node* ConstantNodeCreator::create () const
    {
      return new ConstantNode (this, Value (0.0));
    }

    
    //#***********************************************************************
    // CLASS GEP::Tree::InputNode
    //#***********************************************************************
    
    /*! Constructor */
    InputNode::InputNode (const NodeCreator* creator, const QString& name)
      : Node (creator),
	_name (name)
    {
      if (_name.isEmpty ())
	throw InternalException ("Input node must have a name");
    }

    /* Copy constructor */
    InputNode::InputNode (const InputNode& toCopy)
      : Node (toCopy),
	_name (toCopy._name)
    {
    }

    /* Destructor */
    InputNode::~InputNode ()
    {
    }

    /* Create a copy of this node */
    Node* InputNode::clone () const
    {
      return new InputNode (*this);
    }

    /* Compute current node value */
    Value InputNode::execute (const Executer* executer) const
    {
      return executer->getInputValue (_name);
    }

    /* Set input name */
    void InputNode::setName (const QString& name)
    {
      _name = name;
    }

    /* Return string representation of this node */
    QString InputNode::toString () const
    {
      return _name;
    }
    
    //#***********************************************************************
    // CLASS GEP::Tree::InputNodeCreator
    //#***********************************************************************

    /* Constructor */
    InputNodeCreator::InputNodeCreator (const QString& name)
      : NodeCreator ("input", 0),
	_name (name)
    {
      checkValidName (name);
      
      std::vector<Value::Type_t> source_types;
      setTypes (source_types, Value::DOUBLE);
    }

    /* Destructor */
    InputNodeCreator::~InputNodeCreator ()
    {
    }

    /* Create a node of this type */
    Node* InputNodeCreator::create () const
    {
      return new InputNode (this, _name);
    }

    /* Set available input names */
    void InputNodeCreator::setName (const QString& name)
    {
      checkValidName (name);
      _name = name;
    }

    /* Check if the given input name is valid */
    void InputNodeCreator::checkValidName (const QString& name) const
    {
      if (name.isEmpty ())
	throw InternalException ("Empty input name");
      
      for (int i=0; i < name.size (); ++i)
	if (!name[i].isLetterOrNumber () && name[i] != '_')
	  throw InternalException ("Illegal input name character");
    }
    

    
    //#***********************************************************************
    // CLASS GEP::Tree::DualSourceArithmeticNode
    //#***********************************************************************

    /*! Constructor */
    DualSourceArithmeticNode::DualSourceArithmeticNode
    (const NodeCreator* creator)
      : Node (creator)
    {
    }

    /*! Copy constructor */
    DualSourceArithmeticNode::DualSourceArithmeticNode
    (const DualSourceArithmeticNode& toCopy)
      : Node (toCopy)
    {
    }

    /*! Destructor */
    DualSourceArithmeticNode::~DualSourceArithmeticNode ()
    {
    }
      
    /*!
     * Get the two source values of this nodes successors
     *
     * An exception is thrown if the number of sources or their value types
     * does not match.
     */
    void DualSourceArithmeticNode::getValues
    (const Executer* executer, Value* value1, Value* value2) const
    {
      const std::vector<Node*>& sources = Node::getSources ();
      if (sources.size () != 2)
	throw InternalException ("Exactly two sources expected");

      if (sources[0]->getCreator ()->getTargetType () != Value::DOUBLE)
	throw InternalException ("Source 0 must be double");
      if (sources[1]->getCreator ()->getTargetType () != Value::DOUBLE)
	throw InternalException ("Source 1 must be double");

      *value1 = sources[0]->execute (executer);
      *value2 = sources[1]->execute (executer);
    }

    /* Return string representation of this node */
    QString DualSourceArithmeticNode::toString () const
    {
      const std::vector<Node*>& sources = Node::getSources ();
      if (sources.size () != 2)
	throw InternalException ("Exactly two sources expected");

      const DualSourceArithmeticNodeCreator* creator =
	dynamic_cast<const DualSourceArithmeticNodeCreator*> (getCreator ());
      if (creator == 0)
	throw InternalException ("Creator not set or of wrong type");
      
      return QString ("(%1% %2% %3%)")
	.arg (creator->getOutputSymbol ())
	.arg (sources[0]->toString ())
	.arg (sources[1]->toString ());
    }

    
    //#***********************************************************************
    // CLASS GEP::Tree::DualSourceArithmeticNodeCreator
    //#***********************************************************************

    /* Constructor */
    DualSourceArithmeticNodeCreator::DualSourceArithmeticNodeCreator
    (const QString& type_name, const QString& output_symbol)
      : NodeCreator (type_name, 2),
	_output_symbol (output_symbol)
    {
      std::vector<Value::Type_t> source_types;
      source_types.push_back (Value::DOUBLE);
      source_types.push_back (Value::DOUBLE);

      setTypes (source_types, Value::DOUBLE);
    }

    /* Destructor */
    DualSourceArithmeticNodeCreator::~DualSourceArithmeticNodeCreator ()
    {
    }


    //#***********************************************************************
    // CLASS GEP::Tree::AdditionNode
    //#***********************************************************************

    /*! Constructor */
    AdditionNode::AdditionNode (const NodeCreator* creator)
      : DualSourceArithmeticNode (creator)
    {
    }
    
    /*! Copy constructor */
    AdditionNode::AdditionNode (const AdditionNode& toCopy)
      : DualSourceArithmeticNode (toCopy)
    {
    }

    /*! Destructor */
    AdditionNode::~AdditionNode ()
    {
    }

    /*! Create a copy of this node */
    Node* AdditionNode::clone () const
    {
      return new AdditionNode (*this);
    }
    
    /*! Compute current node value */
    Value AdditionNode::execute (const Executer* executer) const
    {
      Value value1;
      Value value2;
      getValues (executer, &value1, &value2);

      return value1 != Value::NONE && value1 != Value::NONE ?
	Value (value1.getDouble () + value2.getDouble ()) : Value::NONE;
    }

    
    //#***********************************************************************
    // CLASS GEP::Tree::AdditionNodeCreator
    //#***********************************************************************

    /* Constructor */
    AdditionNodeCreator::AdditionNodeCreator ()
      : DualSourceArithmeticNodeCreator ("add", "+")
    {
    }

    /* Destructor */
    AdditionNodeCreator::~AdditionNodeCreator ()
    {
    }

    /* Create a node of this type */
    Node* AdditionNodeCreator::create () const
    {
      return new AdditionNode (this);
    }

    //#***********************************************************************
    // CLASS GEP::Tree::SubtractionNode
    //#***********************************************************************

    /*! Constructor */
    SubtractionNode::SubtractionNode (const NodeCreator* creator)
      : DualSourceArithmeticNode (creator)
    {
    }
    
    /*! Copy constructor */
    SubtractionNode::SubtractionNode (const SubtractionNode& toCopy)
      : DualSourceArithmeticNode (toCopy)
    {
    }

    /*! Destructor */
    SubtractionNode::~SubtractionNode ()
    {
    }

    /*! Create a copy of this node */
    Node* SubtractionNode::clone () const
    {
      return new SubtractionNode (*this);
    }

    /*! Compute current node value */
    Value SubtractionNode::execute (const Executer* executer) const
    {
      Value value1;
      Value value2;
      getValues (executer, &value1, &value2);

      return value1 != Value::NONE && value1 != Value::NONE ?
	Value (value1.getDouble () - value2.getDouble ()) : Value::NONE;
    }

    
    //#***********************************************************************
    // CLASS GEP::Tree::SubtractionNodeCreator
    //#***********************************************************************

    /* Constructor */
    SubtractionNodeCreator::SubtractionNodeCreator ()
      : DualSourceArithmeticNodeCreator ("sub", "-")
    {
    }

    /* Destructor */
    SubtractionNodeCreator::~SubtractionNodeCreator ()
    {
    }

    /* Create a node of this type */
    Node* SubtractionNodeCreator::create () const
    {
      return new SubtractionNode (this);
    }

    //#***********************************************************************
    // CLASS GEP::Tree::MultiplicationNode
    //#***********************************************************************

    /*! Constructor */
    MultiplicationNode::MultiplicationNode (const NodeCreator* creator)
      : DualSourceArithmeticNode (creator)
    {
    }
    
    /*! Copy constructor */
    MultiplicationNode::MultiplicationNode (const MultiplicationNode& toCopy)
      : DualSourceArithmeticNode (toCopy)
    {
    }

    /*! Destructor */
    MultiplicationNode::~MultiplicationNode ()
    {
    }

    /*! Create a copy of this node */
    Node* MultiplicationNode::clone () const
    {
      return new MultiplicationNode (*this);
    }

    /*! Compute current node value */
    Value MultiplicationNode::execute (const Executer* executer) const
    {
      Value value1;
      Value value2;
      getValues (executer, &value1, &value2);

      return value1 != Value::NONE && value1 != Value::NONE ?
	Value (value1.getDouble () * value2.getDouble ()) : Value::NONE;
    }

    
    //#***********************************************************************
    // CLASS GEP::Tree::MultiplicationNodeCreator
    //#***********************************************************************

    /* Constructor */
    MultiplicationNodeCreator::MultiplicationNodeCreator ()
      : DualSourceArithmeticNodeCreator ("mul", "*")
    {
    }

    /* Destructor */
    MultiplicationNodeCreator::~MultiplicationNodeCreator ()
    {
    }

    /* Create a node of this type */
    Node* MultiplicationNodeCreator::create () const
    {
      return new MultiplicationNode (this);
    }

    
    //#***********************************************************************
    // CLASS GEP::Tree::DivisionNode
    //#***********************************************************************

    /*! Constructor */
    DivisionNode::DivisionNode (const NodeCreator* creator)
      : DualSourceArithmeticNode (creator)
    {
    }
    
    /*! Copy constructor */
    DivisionNode::DivisionNode (const DivisionNode& toCopy)
      : DualSourceArithmeticNode (toCopy)
    {
    }

    /*! Destructor */
    DivisionNode::~DivisionNode ()
    {
    }

    /*! Create a copy of this node */
    Node* DivisionNode::clone () const
    {
      return new DivisionNode (*this);
    }

    /*! Compute current node value */
    Value DivisionNode::execute (const Executer* executer) const
    {
      Value result = Value::NONE;
      
      Value value1;
      Value value2;
      getValues (executer, &value1, &value2);

      if ( value1 != Value::NONE && value1 != Value::NONE &&
	   value2.getDouble () != 0.0 )
	result = Value (value1.getDouble () / value2.getDouble ());

      return result;
    }

    
    //#***********************************************************************
    // CLASS GEP::Tree::DivisionNodeCreator
    //#***********************************************************************

    /* Constructor */
    DivisionNodeCreator::DivisionNodeCreator ()
      : DualSourceArithmeticNodeCreator ("div", "/")
    {
    }

    /* Destructor */
    DivisionNodeCreator::~DivisionNodeCreator ()
    {
    }

    /* Create a node of this type */
    Node* DivisionNodeCreator::create () const
    {
      return new DivisionNode (this);
    }

    //#***********************************************************************
    // CLASS GEP::Tree::PowerNode
    //#***********************************************************************

    /*! Constructor */
    PowerNode::PowerNode (const NodeCreator* creator)
      : DualSourceArithmeticNode (creator)
    {
    }
    
    /*! Copy constructor */
    PowerNode::PowerNode (const PowerNode& toCopy)
      : DualSourceArithmeticNode (toCopy)
    {
    }

    /*! Destructor */
    PowerNode::~PowerNode ()
    {
    }

    /*! Create a copy of this node */
    Node* PowerNode::clone () const
    {
      return new PowerNode (*this);
    }

    /*! Compute current node value */
    Value PowerNode::execute (const Executer* executer) const
    {
      Value value1;
      Value value2;
      getValues (executer, &value1, &value2);

      return value1 != Value::NONE && value1 != Value::NONE ?
	Value (pow (value1.getDouble (), value2.getDouble ())) : Value::NONE;
    }

    
    //#***********************************************************************
    // CLASS GEP::Tree::PowerNodeCreator
    //#***********************************************************************

    /* Constructor */
    PowerNodeCreator::PowerNodeCreator ()
      : DualSourceArithmeticNodeCreator ("pow", "^")
    {
    }

    /* Destructor */
    PowerNodeCreator::~PowerNodeCreator ()
    {
    }

    /* Create a node of this type */
    Node* PowerNodeCreator::create () const
    {
      return new PowerNode (this);
    }


    //#***********************************************************************
    // CLASS GEP::Tree::FunctionTreeCreator
    //#***********************************************************************

    /*! Constructor */
    FunctionTreeCreator::FunctionTreeCreator (unsigned int number_of_variables)
    {
      addNodeCreator (new ConstantNodeCreator ());
      addNodeCreator (new AdditionNodeCreator ());
      addNodeCreator (new SubtractionNodeCreator ());
      addNodeCreator (new MultiplicationNodeCreator ());
      addNodeCreator (new DivisionNodeCreator ());
      addNodeCreator (new PowerNodeCreator ());

      for (unsigned int i=0; i < number_of_variables; ++i)
	addNodeCreator (new InputNodeCreator (QString ("x%1").arg (i)));
    }

    /*! Destructor */
    FunctionTreeCreator::~FunctionTreeCreator ()
    {
    }
    

  } // namespace Tree
} // namespace GEP
