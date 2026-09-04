#pragma once
#include "SudokuDefines.h"

namespace Sudoku
{
class ValueSetUtils
{
 public:
   static bool IsSolved(const ValueSet &valueSet);
};

} // namespace Sudoku
