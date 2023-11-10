#pragma once
#include "SudokuDefines.h"

#include <bitset>

namespace Sudoku
{
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
   private:
      std::bitset<NumValues> m_active;
   };

}