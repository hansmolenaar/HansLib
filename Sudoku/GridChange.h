#pragma once

#include "SudokuDefines.h"

namespace Sudoku
{

struct GridChange
{
   size_t numValuesChanged = 0;
   Field numFieldsSolved = 0;

   inline GridChange &operator+=(const GridChange &add)
   {
      numValuesChanged += add.numValuesChanged;
      numFieldsSolved += add.numFieldsSolved;
      return *this;
   }
};

} // namespace Sudoku
