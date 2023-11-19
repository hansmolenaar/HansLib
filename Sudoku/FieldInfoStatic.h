#pragma once
#include "SudokuDefines.h"

namespace Sudoku
{
   namespace FieldInfoStatic
   {
      constexpr FieldIndex RowColToField(RowColBoxIndex row, RowColBoxIndex col) { return row * NumRowColBox + col; }
      void CheckRowColIndex(RowColBoxIndex index);
      void CheckValue(Value value);
      const FieldSet& GetRow(RowColBoxIndex row);
      const FieldSet& GetCol(RowColBoxIndex col);
      const FieldSet& GetBox(RowColBoxIndex box);
      const FieldSet& GetFieldSet(RowColBoxType type, RowColBoxIndex subSetIndex);
      const std::array<FieldIndex, NumFields>& getAllFields();

      RowColBoxIndex FieldToRow(FieldIndex field);
      RowColBoxIndex FieldToCol(FieldIndex field);
      RowColBoxIndex FieldToBox(FieldIndex field);
   };

}