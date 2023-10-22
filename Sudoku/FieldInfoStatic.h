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
      static constexpr FieldIndex RowColToField(RowColIndex row, RowColIndex col) { return row * NumRowCol + col; }
      static void CheckRowColIndex(RowColIndex index);
      static void CheckValue(Value value);
   };

}