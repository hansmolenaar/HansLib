#pragma once

#include "SudokuDefines.h"

namespace Sudoku
{
struct Coordinates
{
   RowColBox Row = RowColBoxUndefined;
   RowColBox Col = RowColBoxUndefined;

   Field toField() const;
   static Coordinates fromField(Field);
};

} // namespace Sudoku
