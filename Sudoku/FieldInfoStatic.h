#pragma once
#include "SudokuDefines.h"

namespace Sudoku
{
   struct FieldInfoStatic
   {
      RowColIndex    Row;
      RowColIndex    Col;
      SubSquareIndex SubSquare;

      static const std::array<FieldInfoStatic, NumFields>& Instance();
      static FieldIndex RowColToField(RowColIndex row, RowColIndex col);
      static void CheckRowColIndex(RowColIndex index);
   };

}