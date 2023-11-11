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
      static Potential Create(std::initializer_list<Value> values);

      void setAll();
      void setNone();
      void setSingle(Value value);

      // Only for testing
      void set(Value value);

      // Returns: value changed
      bool unset(Value value);
      bool containsValue(Value value) const;
      Value getSingleValue() const;
      RowColIndex count() const;
      bool isSingle() const;

      PotentialValues getPotentialValues() const;

      static PotentialValues getIntersection(const Potential& pot1, const Potential& pot2);
      static PotentialValues getUnion(const Potential& pot1, const Potential& pot2);
   private:
      std::bitset<NumValues> m_active;
   };

}