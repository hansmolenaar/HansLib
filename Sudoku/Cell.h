#pragma once

#include "SudokuDefines.h"

namespace Sudoku
{
struct Cell
{
   Cell() = default;
   Cell(Field);

   Field field = FieldUndefined;
   Index row = IndexUndefined;
   Index col = IndexUndefined;
   Index box = IndexUndefined;

   static const Cell &get(Field);
   static const Cell &getFromRowCol(Index, Index);
   static Field rowCol2Field(Index, Index);
};

} // namespace Sudoku
