/*
 * tree_executer.cpp - Class for executing a function tree
 *
 * Frank Cieslok, 04.08.2006
 */

#include "GEPTreeExecuter.hpp"

namespace GEP {
  namespace Tree {

    //#***********************************************************************
    // CLASS GEP::Tree::Executer
    //#***********************************************************************

    /*! Constructor */
    Executer::Executer ()
    {
    }

    /*! Destructor */
    Executer::~Executer ()
    {
    }
    
    /*! Clear current input values */
    void Executer::clearInputValues ()
    {
      _input_values.clear ();
    }

    /*!
     * Get the value of a named input
     *
     * \param name Name of the input to read
     * \return Current value of the input or Value::NONE if not set
     */
    Value Executer::getInputValue (const QString& name) const
    {
      ValueMap::const_iterator pos = _input_values.find (name);
      return pos != _input_values.end () ? pos->second : Value::NONE;
    }

    /*!
     * Set the value of a named input
     */
    void Executer::setInputValue (const QString& name, const Value& value)
    {
      _input_values[name] = value;
    }

  } // namespace Tree
} // namespace GEP

