/*
 * test_function_tree.cpp - Test for function tree objects
 *
 * Frank Cieslok, 05.08.2006
 */

#include "GEPTree.hpp"
#include "GEPTreeExecuter.hpp"
#include "GEPTreeFunction.hpp"
#include "GEPTreeValue.hpp"

#include <iostream>
#include <limits>
#include <memory>

namespace GEP {
  namespace Tree {

    static ConstantNodeCreator constant_creator;
    static InputNodeCreator input_creator ("x");
    static AdditionNodeCreator addition_creator;
    static SubtractionNodeCreator subtraction_creator;
    static MultiplicationNodeCreator multiplication_creator;
    static DivisionNodeCreator division_creator;

    /*
     * Create a constant node
     */
    static Node* createConstant (double value)
    {
      ConstantNode* node =
	dynamic_cast<ConstantNode*> (constant_creator.create ());
      node->setValue (Value (value));
      return node;
    }

    /*
     * Create an input node
     */
    static Node* createInput ()
    {
      InputNode* node =
	dynamic_cast<InputNode*> (input_creator.create ());
      return node;
    }
    
    /*
     * Create an addition node
     */
    static Node* createAddition (Node* source1, Node* source2)
    {
      AdditionNode* node =
	dynamic_cast<AdditionNode*> (addition_creator.create ());
      node->addSource (source1);
      node->addSource (source2);
      return node;
    }
    
    /*
     * Create a subtraction node
     */
    static Node* createSubtraction (Node* source1, Node* source2)
    {
      SubtractionNode* node =
	dynamic_cast<SubtractionNode*> (subtraction_creator.create ());
      node->addSource (source1);
      node->addSource (source2);
      return node;
    }
    
    /*
     * Create a multiplication node
     */
    static Node* createMultiplication (Node* source1, Node* source2)
    {
      MultiplicationNode* node =
	dynamic_cast<MultiplicationNode*> (multiplication_creator.create ());
      node->addSource (source1);
      node->addSource (source2);
      return node;
    }
    
    /*
     * Create a division node
     */
    static Node* createDivision (Node* source1, Node* source2)
    {
      DivisionNode* node =
	dynamic_cast<DivisionNode*> (division_creator.create ());
      node->addSource (source1);
      node->addSource (source2);
      return node;
    }
    
    /*
     * Test tree printing routines
     */
    static void testTreeOutput ()
    {
      Tree tree;
      tree.setRoot
	(createSubtraction
	 (createAddition (createInput (), createConstant (1.0)),
	  createAddition (createInput (), createConstant (2.0))));

      std::cout << qPrintable (tree.toString ()) << std::endl;
    }

    /*
     * Test single tree execution
     */
    static void testExecution (Node* root, double x, double expected)
    {
      Tree tree;
      tree.setRoot (root->clone ());

      std::cout << "Executing: " << qPrintable (tree.toString ()) << std::endl;
      std::cout << "  x=" << x << std::endl;
      std::cout << "  expected=" << expected << std::endl;

      Executer executer;
      executer.setInputValue ("x", Value (x));
      
      Value result = tree.getRoot ()->execute (&executer);
      std::cout << "  result=" << qPrintable (result.toString ()) << std::endl;
      
      if ( result == Value::NONE &&
	   expected != std::numeric_limits<double>::max () )
	throw InternalException (QString ("Expected NONE but got %1")
				 .arg (result.toString ()));
      else if (result.getDouble () != expected)
	throw InternalException (QString ("Expected %1 but got %2")
				 .arg (expected)
				 .arg (result.toString ()));
    }
    
    /*
     * Test tree execution
     */
    static void testTreeExecution ()
    {
      std::auto_ptr<Node> node_1
	(createAddition
	 (createMultiplication (createInput (), createConstant (2.0)),
	  createMultiplication (createInput (), createConstant (3.0))));

      testExecution (node_1.get (), 1.0, 5.0);
      testExecution (node_1.get (), 2.0, 10.0);

      std::auto_ptr<Node> node_2
	(createDivision
	 (createAddition (createInput (), createConstant (2.0)),
	  createAddition (createInput (), createConstant (3.0))));

      testExecution (node_2.get (), 1.0, 3.0 / 4.0);
      testExecution (node_2.get (), 2.0, 4.0 / 5.0);
    }
    
  } // namespace Tree
} // namespace GEP


int main (int /*argc*/, char** /*argv*/)
{
  bool ok = true;

  try
  {
    GEP::Tree::testTreeOutput ();
    GEP::Tree::testTreeExecution ();
  }
  catch (GEP::InternalException& exception)
  {
    std::cerr << "ERROR: " << exception.getMessage () << std::endl;
    ok = false;
  }
  
  return ok ? 0 : 1;
}
