#pragma once
#include "SudokuDefines.h"

#include <bitset>
#include<boost/container/static_vector.hpp>

namespace Sudoku
{
   using PotentialValues = boost::container::static_vector<Value, SubSetSize>;

   class Potential
   {
   public:
      Potential();

      void setAll();
      void setNone();
      void setSingle(Value value);
      void set(Value value);
      // Returns: value changed
      bool unset(Value value);
      bool containsValue(Value value) const;
      Value getSingleValue() const;
      RowColIndex count() const;
      bool isSingle() const;

      PotentialValues getPotentialValues() const;
   private:
      std::bitset<NumValues> m_active;
   };

}