/*
 * GEPTreeFunction.hpp - Tree object representing a function
 *
 * Frank Cieslok, 04.08.2006
 */

#ifndef __GEPTreeFunction_hpp__
#define __GEPTreeFunction_hpp__

#include "GEPTreeValue.hpp"
#include "GEPTree.hpp"

namespace GEP {
  namespace Tree {

    /*!
     * Node representing a constant
     */
    class ConstantNode : public GEP::Tree::Node
    {
    public:
      ConstantNode (const NodeCreator* creator, const Value& value);
      ConstantNode (const ConstantNode& toCopy);
      virtual ~ConstantNode ();

      inline const Value& getValue () const;
      void setValue (const Value& value);
      
      virtual Node* clone () const;
      virtual Value execute (const Executer* executer) const;
      virtual QString toString () const;

    private:
      Value _value;
    };

    /*!
     * Creator for constant nodes
     */
    class ConstantNodeCreator : public GEP::Tree::NodeCreator
    {
    public:
      ConstantNodeCreator ();
      virtual ~ConstantNodeCreator ();

      virtual Node* create () const;
    };

    /*!
     * Node representing a names input value
     */
    class InputNode : public GEP::Tree::Node
    {
    public:
      InputNode (const NodeCreator* creator, const QString& name);
      InputNode (const InputNode& toCopy);
      virtual ~InputNode ();

      inline const QString& getName () const;
      void setName (const QString& name);
      
      virtual Node* clone () const;
      virtual Value execute (const Executer* executer) const;
      virtual QString toString () const;

    private:
      QString _name;
    };

    /*!
     * Creator for input nodes
     */
    class InputNodeCreator : public GEP::Tree::NodeCreator
    {
    public:
      InputNodeCreator (const QString& name);
      virtual ~InputNodeCreator ();

      virtual Node* create () const;

      inline const QString& getName () const;
      void setName (const QString& name);

    private:
      void checkValidName (const QString& name) const;
      
    private:
      QString _name;
    };

    /*!
     * Base class for arithmetic function representing nodes with two sources
     */
    class DualSourceArithmeticNode : public GEP::Tree::Node
    {
    public:
      DualSourceArithmeticNode (const NodeCreator* creator);
      DualSourceArithmeticNode (const DualSourceArithmeticNode& toCopy);
      virtual ~DualSourceArithmeticNode ();
      
      virtual Node* clone () const = 0;
      virtual Value execute (const Executer* executer) const = 0;
      virtual QString toString () const;

    protected:
      void getValues (const Executer* executer, Value* value1, Value* value2)
	const;
    };
    
    /*!
     * Creator for nodes of type DualSourceArithmeticNode
     */
    class DualSourceArithmeticNodeCreator : public GEP::Tree::NodeCreator
    {
    public:
      DualSourceArithmeticNodeCreator (const QString& type_name,
				       const QString& output_symbol);
      virtual ~DualSourceArithmeticNodeCreator ();

      inline const QString& getOutputSymbol () const;
      
      virtual Node* create () const = 0;

    private:
      QString _output_symbol;
    };

    /*!
     * Node representing an addition
     */
    class AdditionNode : public GEP::Tree::DualSourceArithmeticNode
    {
    public:
      AdditionNode (const NodeCreator* creator);
      AdditionNode (const AdditionNode& toCopy);
      virtual ~AdditionNode ();

      virtual Node* clone () const;
      virtual Value execute (const Executer* executer) const;
    };
      
    /*!
     * Creator for addition nodes
     */
    class AdditionNodeCreator :
      public GEP::Tree::DualSourceArithmeticNodeCreator
    {
    public:
      AdditionNodeCreator ();
      virtual ~AdditionNodeCreator ();

      virtual Node* create () const;
    };

    /*!
     * Node representing a subtraction
     */
    class SubtractionNode : public GEP::Tree::DualSourceArithmeticNode
    {
    public:
      SubtractionNode (const NodeCreator* creator);
      SubtractionNode (const SubtractionNode& toCopy);
      virtual ~SubtractionNode ();

      virtual Node* clone () const;
      virtual Value execute (const Executer* executer) const;
    };
      
    /*!
     * Creator for subtraction nodes
     */
    class SubtractionNodeCreator :
      public GEP::Tree::DualSourceArithmeticNodeCreator
    {
    public:
      SubtractionNodeCreator ();
      virtual ~SubtractionNodeCreator ();

      virtual Node* create () const;
    };

    /*!
     * Node representing a multiplication
     */
    class MultiplicationNode : public GEP::Tree::DualSourceArithmeticNode
    {
    public:
      MultiplicationNode (const NodeCreator* creator);
      MultiplicationNode (const MultiplicationNode& toCopy);
      virtual ~MultiplicationNode ();

      virtual Node* clone () const;
      virtual Value execute (const Executer* executer) const;
    };
      
    /*!
     * Creator for multiplication nodes
     */
    class MultiplicationNodeCreator :
      public GEP::Tree::DualSourceArithmeticNodeCreator
    {
    public:
      MultiplicationNodeCreator ();
      virtual ~MultiplicationNodeCreator ();

      virtual Node* create () const;
    };

    /*!
     * Node representing a division
     */
    class DivisionNode : public GEP::Tree::DualSourceArithmeticNode
    {
    public:
      DivisionNode (const NodeCreator* creator);
      DivisionNode (const DivisionNode& toCopy);
      virtual ~DivisionNode ();

      virtual Node* clone () const;
      virtual Value execute (const Executer* executer) const;
    };
      
    /*!
     * Creator for division nodes
     */
    class DivisionNodeCreator :
      public GEP::Tree::DualSourceArithmeticNodeCreator
    {
    public:
      DivisionNodeCreator ();
      virtual ~DivisionNodeCreator ();

      virtual Node* create () const;
    };
    
    /*!
     * Node representing a power operation
     */
    class PowerNode : public GEP::Tree::DualSourceArithmeticNode
    {
    public:
      PowerNode (const NodeCreator* creator);
      PowerNode (const PowerNode& toCopy);
      virtual ~PowerNode ();

      virtual Node* clone () const;
      virtual Value execute (const Executer* executer) const;
    };
      
    /*!
     * Creator for addition nodes
     */
    class PowerNodeCreator :
      public GEP::Tree::DualSourceArithmeticNodeCreator
    {
    public:
      PowerNodeCreator ();
      virtual ~PowerNodeCreator ();

      virtual Node* create () const;
    };

    /*
     * Creator for function tree objects
     */
    class FunctionTreeCreator : public GEP::Tree::TreeCreator
    {
    public:
      FunctionTreeCreator (unsigned int number_of_variables);
      virtual ~FunctionTreeCreator ();
    };
      
    
    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Return value of this constant */
    inline const Value& ConstantNode::getValue () const
    {
      return _value;
    }

    /* Return name of the input */
    inline const QString& InputNode::getName () const
    {
      return _name;
    }

    /* Return possible input node names */
    inline const QString& InputNodeCreator::getName () const
    {
      return _name;
    }
    
    /* Return the symbol used for printing the node */
    inline const QString& DualSourceArithmeticNodeCreator::getOutputSymbol () 
      const
    {
      return _output_symbol;
    }
    
  } // namespace Tree
} // namespace GEP

#endif
