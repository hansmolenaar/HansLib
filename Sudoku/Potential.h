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
      void Set(RowColIndex index);
      void Unset(RowColIndex index);
      bool operator()(RowColIndex index) const;
      RowColIndex Count() const;
   private:
      std::array<bool, NumRowCol> m_active;
   };

}