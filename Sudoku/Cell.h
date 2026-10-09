#pragma once

#include "SudokuDefines.h"

namespace Sudoku
{
struct Cell
{
   Cell(Field);

   Field field = FieldUndefined;
   Index row = IndexUndefined;
   Index col = IndexUndefined;
   Index box = IndexUndefined;
};

} // namespace Sudoku
