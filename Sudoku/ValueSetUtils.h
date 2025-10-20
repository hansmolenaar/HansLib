#pragma once
#include "SudokuDefines.h"

#include <bitset>

namespace Sudoku
{
class ValueSetUtils
{
  public:
    static bool IsSolved(const ValueSet &valueSet);
};

} // namespace Sudoku