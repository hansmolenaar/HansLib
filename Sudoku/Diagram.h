#pragma once

#include "SudokuDefines.h"
#include <unordered_map>

namespace Sudoku
{
   class Diagram
   {
   public:
      static Diagram Create(const std::unordered_map<FieldIndex, Value>& input);
   private:
      Diagram(std::array<Value, NumFields> values);
      std::array<Value, NumFields> m_state;
   };

}