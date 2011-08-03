/*
 * tree_value.cpp - Generic value object passed thru tree structures
 *
 * Frank Cieslok, 04.08.2006
 */

#include "GEPTreeValue.hpp"

namespace GEP {
  namespace Tree {

    //#***********************************************************************
    // CLASS GEP::Tree::Value
    //#***********************************************************************

    /*! Null value constant [STATIC] */
    Value Value::NONE;

    /*! Constructor */
    Value::Value ()
      : _type (INVALID)
    {
    }

    /*! Constructor */
    Value::Value (bool value)
      : _type (BOOL)
    {
      _value._bool_value = value;
    }

    /*! Constructor */
    Value::Value (int value)
      : _type (INT)
    {
      _value._int_value = value;
    }

    /*! Constructor */
    Value::Value (double value)
      : _type (DOUBLE)
    {
      _value._double_value = value;
    }

    /*! Copy constructor */
    Value::Value (const Value& toCopy)
      : _type  (toCopy._type),
	_value (toCopy._value)
    {
    }

    /*! Destructor */
    Value::~Value ()
    {
    }

    /* Comparison operator */
    bool Value::operator== (const Value& value) const
    {
      bool equals = false;

      if (_type == value._type)
      {
	switch (_type)
	{
	  case INVALID:
	    equals = true;
	    break;
	    
	  case BOOL:
	    equals = getBool () == value.getBool ();
	    break;
	    
	  case INT:
	    equals = getInt () == value.getInt ();
	    break;
	    
	  case DOUBLE:
	    equals = getDouble () == value.getDouble ();
	    break;
	}
      }
      
      return equals;
    }

    /* Comparison operator */
    bool Value::operator!= (const Value& value) const
    {
      return !(*this == value);
    }

    /* Assignment operator */
    const Value& Value::operator= (const Value& toCopy)
    {
      _type = toCopy._type;
      _value = toCopy._value;

      return *this;
    }

    /* Return printable string representation of this value */
    QString Value::toString() const
    {
      QString result = "";

      switch (_type)
	{
	  case INVALID:
	    result = "<INVALID>";
	    break;
	    
	  case BOOL:
	    result = getBool () ? "True" : "False";
	    break;
	    
	  case INT:
	    result = QString ("%1").arg (getInt ());
	    break;
	    
	  case DOUBLE:
	    result = QString ("%1").arg (getDouble ());
	    break;
	}

      return result;
    }
    
  } // namespace Tree
} // namespace GEP
