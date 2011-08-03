/*
 * GEPDebug.hpp - Useful debugging functions
 *
 * Frank Cieslok, 03.06.2006
 */

#ifndef __GEPDebug_hpp__
#define __GEPDebug_hpp__

#include <QtCore/QString>

#include <iostream>
#include <vector>

namespace GEP {

#ifdef _DEBUG
#define DV(expression) (std::cout << expression << std::endl)
#else
#define DV(expression)
#endif

#define DVX(expression) (std::cout << expression << std::endl)
  
  namespace Debug {

    template <class T>
    static QString toString (const T& value)
    {
      return QString ("%1").arg (value);
    }
    
    template <class T>
    static QString toString (const std::vector<T>& sequence)
    {
      QString result;
      
      QString separator = "";
      for (unsigned int i=0; i < sequence.size (); ++i)
      {
	result += separator + QString ("%1").arg (sequence[i]);
	separator = ", ";
      }
      
      return result;
    }
    
  } // namespace Debug
} // namespace GEP

#endif
