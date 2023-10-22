#pragma once

#include "SudokuDefines.h"
#include <unordered_map>
#include <ostream>

namespace Sudoku
{
   class Diagram
   {
   public:
      static Diagram Create(const std::unordered_map<FieldIndex, Value>& input);
      Value operator()(FieldIndex field) const;
   private:
      Diagram(std::array<Value, NumFields> values);
      std::array<Value, NumFields> m_state;
   };

};

std::ostream& operator<<(std::ostream& os, const Sudoku::Diagram& diagram);