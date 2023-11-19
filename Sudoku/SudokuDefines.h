#pragma once

#include <array>

namespace Sudoku
{
   using Value = int; // 1..9
   constexpr Value ValueUndefined = 0;
   constexpr Value NumValues = 9;
   constexpr std::array<Value, NumValues> ValueAll = { 1,2,3,4,5,6,7,8,9 };
   constexpr Value ValueMax = ValueAll.back();

   enum class RowColBoxType {Row, Col, Box};
   constexpr std::array<RowColBoxType, 3> RowColBoxTypeAll = { RowColBoxType::Row, RowColBoxType::Col, RowColBoxType::Box };

   using RowColBoxPosition = int; // Position in row/col/box
   using RowColBoxIndex = int; // 0..8, numbering of row/col/box
   constexpr RowColBoxIndex NumRowBoxCol = 9;
   constexpr RowColBoxPosition NumRowColBoxPositions = 9; // Number of RowColBoxPosition in row/col/box

   constexpr std::array<RowColBoxIndex, NumRowBoxCol> SubSetsAll = { 0,1,2,3,4,5,6,7,8 };
   constexpr std::array<RowColBoxPosition, NumRowColBoxPositions> SubSetItemsAll = { 0,1,2,3,4,5,6,7,8 };

   constexpr RowColBoxIndex NumRowColBox = 9;
   constexpr std::array<RowColBoxIndex, NumRowColBox> RowColAll = { 0,1,2,3,4,5,6,7,8 };

   using BoxIndex = RowColBoxPosition; // 0..8
   using SubSquareRowColIndex = int; // 0..2
   constexpr int NumSubSquareRowCol = 3;
   constexpr int NumBox = NumRowBoxCol;
   constexpr std::array<BoxIndex, NumBox> BoxAll = { 0,1,2,3,4,5,6,7,8 };

   using FieldIndex = int; // 0..80
   constexpr int NumFields = NumRowColBox * NumRowColBox;

   using FieldSet = std::array<FieldIndex, NumRowColBoxPositions>;
   using ValueSet = std::array<Value, NumRowColBoxPositions>;

   struct FieldValue
   {
      FieldIndex Field;
      Value  Value;
   };
}