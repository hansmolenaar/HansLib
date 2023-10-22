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
      void Set(Value value);
      void Unset(Value value);
      bool ContainsValue(Value value) const;
      RowColIndex Count() const;
   private:
      std::array<bool, NumValues> m_active;
   };

}