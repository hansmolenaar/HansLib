#pragma once

#include "SudokuDefines.h"

namespace Sudoku
{
struct Coordinates
{
   Index Row = IndexUndefined;
   Index Col = IndexUndefined;

   Field toField() const;
   static Coordinates fromField(Field);
};

} // namespace Sudoku
