#pragma once

#include "Cell.h"

namespace Sudoku
{
struct House
{
   static const House &getRow(Index);
   static const House &getCol(Index);
   static const House &getBox(Index);

   std::array<Cell, NumIndex> cells;
};

} // namespace Sudoku
