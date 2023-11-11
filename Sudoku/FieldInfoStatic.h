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
      static const FieldSet& GetRow(SubSetIndex row);
      static const FieldSet& GetCol(SubSetIndex col);
      static const FieldSet& GetSubSquare(SubSetIndex subSquare);
      static const FieldSet& GetFieldSet(SubSetType type, SubSetIndex subSetIndex);
   };

}