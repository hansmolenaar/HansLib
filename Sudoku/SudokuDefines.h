#pragma once

#include <array>

namespace Sudoku
{
   using Value = int; // 1..9
   constexpr Value ValueUndefined = 0;
   constexpr Value NumValues = 9;
   constexpr std::array<Value, NumValues> ValueAll = { 1,2,3,4,5,6,7,8,9 };
   constexpr Value ValueMax = ValueAll.back();

   enum class SubSetType {Row, Column, SubSquare};
   constexpr std::array<SubSetType, 3> SubSetTypeAll = { SubSetType::Row, SubSetType::Column, SubSetType::SubSquare };

   using SubSetIndex = int;
   using SubSetItem = int;
   constexpr SubSetIndex NumSubSets = 9;
   constexpr SubSetItem SubSetSize = 9;

   constexpr std::array<SubSetIndex, NumSubSets> SubSetsAll = { 0,1,2,3,4,5,6,7,8 };
   constexpr std::array<SubSetItem, SubSetSize> SubSetItemsAll = { 0,1,2,3,4,5,6,7,8 };

   using RowColIndex = SubSetItem; // 0..8
   constexpr RowColIndex NumRowCol = SubSetSize;
   constexpr std::array<RowColIndex, NumRowCol> RowColAll = { 0,1,2,3,4,5,6,7,8 };

   using SubSquareIndex = SubSetItem; // 0..8
   using SubSquareRowColIndex = int; // 0..2
   constexpr int NumSubSquareRowCol = 3;
   constexpr int NumSubSquares = NumSubSets;
   constexpr std::array<SubSquareIndex, NumSubSquares> SubSquareAll = { 0,1,2,3,4,5,6,7,8 };

   using FieldIndex = int; // 0..80
   constexpr int NumFields = NumRowCol * NumRowCol;

   using FieldSet = std::array<FieldIndex, SubSetSize>;
   using ValueSet = std::array<Value, SubSetSize>;

   struct FieldValue
   {
      FieldIndex Field;
      Value  Value;
   };
}