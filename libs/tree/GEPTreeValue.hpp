/*
 * GEPTreeValue.hpp - Generic value object passed thru tree structures
 *
 * Frank Cieslok, 04.08.2006
 */

#ifndef __GEPTreeValue_hpp__
#define __GEPTreeValue_hpp__

#include <GEPException.hpp>

namespace GEP {
  namespace Tree {

    /*!
     * Generic value object passed thru tree structures
     */
    class Value
    {
    public:
      enum Type_t { INVALID, BOOL, INT, DOUBLE };
      
    public:
      Value ();
      explicit Value (bool value);
      explicit Value (int value);
      explicit Value (double value);
      Value (const Value& toCopy);
      virtual ~Value ();

      inline Type_t getType () const;
      inline bool getBool () const;
      inline int getInt () const;
      inline double getDouble () const;

      bool operator== (const Value& value) const;
      bool operator!= (const Value& value) const;

      const Value& operator= (const Value& toCopy);

      QString toString () const;
      
      static Value NONE;
      
    private:
      Type_t _type;
      
      union
      {
	bool _bool_value;
	int _int_value;
	double _double_value;
      } _value;
    };


    //#***********************************************************************
    // Inline functions
    //#***********************************************************************

    /* Return the type of this value */
    inline Value::Type_t Value::getType () const
    {
      return _type;
    }

    /* Return bool value */
    inline bool Value::getBool ()  const
    {
      if (_type != BOOL)
	throw InternalException ("Not a boolean value");
      
      return _value._bool_value;
    }
    
    /* Return integer value */
    inline int Value::getInt ()  const
    {
      if (_type != INT)
	throw InternalException ("Not an integer value");
      
      return _value._int_value;
    }
    
    /* Return double value */
    inline double Value::getDouble ()  const
    {
      if (_type != DOUBLE)
	throw InternalException ("Not a double value");
      
      return _value._double_value;
    }
    
  } // namespace Tree
} // namespace GEP

#endif
