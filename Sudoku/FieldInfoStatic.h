#pragma once
#include "SudokuDefines.h"

namespace Sudoku
{
   struct FieldInfoStatic
   {
      RowColBoxIndex    Row;
      RowColBoxIndex    Col;
      RowColBoxIndex    Box;
      FieldIndex        Field;

      static const std::array<FieldInfoStatic, NumFields>& Instance();
      static constexpr FieldIndex RowColToField(RowColBoxIndex row, RowColBoxIndex col) { return row * NumRowColBox + col; }
      static void CheckRowColIndex(RowColBoxIndex index);
      static void CheckValue(Value value);
      static const FieldSet& GetRow(RowColBoxIndex row);
      static const FieldSet& GetCol(RowColBoxIndex col);
      static const FieldSet& GetBox(RowColBoxIndex box);
      static const FieldSet& GetFieldSet(RowColBoxType type, RowColBoxIndex subSetIndex);
   };

}