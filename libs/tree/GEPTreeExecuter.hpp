/*
 * GEPTreeExecuter.hpp - Class for executing a function tree
 *
 * Frank Cieslok, 04.08.2006
 */

#ifndef __GEPTreeExecuter_hpp__
#define __GEPTreeExecuter_hpp__

#include "GEPTreeValue.hpp"

#include <map>

namespace GEP {
  namespace Tree {

    /*!
     * Class for executing a function tree
     */
    class Executer
    {
    public:
      Executer ();
      ~Executer ();

      void clearInputValues ();

      Value getInputValue (const QString& name) const;
      void setInputValue (const QString& name, const Value& value);
      
    private:
      typedef std::map<QString, Value> ValueMap;
      ValueMap _input_values;
    };
    
  } // namespace Tree
} // namespace GEP

#endif

