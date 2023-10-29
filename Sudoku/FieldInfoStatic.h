#pragma once
#include "SudokuDefines.h"

namespace Sudoku
{
   struct FieldInfoStatic
   {
      RowColIndex    Row;
      RowColIndex    Col;
      SubSquareIndex SubSquare;
      FieldIndex     Field;

      static const std::array<FieldInfoStatic, NumFields>& Instance();
      static constexpr FieldIndex RowColToField(RowColIndex row, RowColIndex col) { return row * NumRowCol + col; }
      static void CheckRowColIndex(RowColIndex index);
      static void CheckValue(Value value);
      static const std::array<FieldIndex, NumRowCol>& GetRow(RowColIndex row);
      static const std::array<FieldIndex, NumRowCol>& GetCol(RowColIndex col);
      static const std::array<FieldIndex, NumSubSquares>& GetSubSquare(SubSquareIndex subSquare);
   };

}