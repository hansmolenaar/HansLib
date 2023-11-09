#pragma once
#include "SudokuDefines.h"

#include <array>
#include <span>

namespace Sudoku
{
   class Potential
   {
   public:
      Potential();
      static Potential Combine(const Potential& pot1, const Potential& pot2, const Potential& pot3);
      void SetAll();
      void SetNone();
      void SetSingle(Value value);
      void Set(Value value);
      // Returns: value changed
      bool Unset(Value value);
      bool ContainsValue(Value value) const;
      Value getSingleValue() const;
      RowColIndex Count() const;
      bool isSingle() const;
   private:
      std::array<bool, NumValues> m_active;
   };

}