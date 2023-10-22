#pragma once

#include <array>

namespace Sudoku
{
   using Value = int; // 1..9
   constexpr Value ValueUndefined = 0;
   constexpr Value NumValues = 9;

   using RowColIndex = int; // 0..8
   constexpr RowColIndex NumRowCol = 9;
   constexpr std::array<RowColIndex, NumRowCol> RowColAll = { 0,1,2,3,4,5,6,7,8 };

   using SubSquareIndex = int; // 0..8
   using SubSquareRowColIndex = int; // 0..2
   constexpr int NumSubSquareRowCol = 3;
   constexpr int NumSubSquares = NumSubSquareRowCol* NumSubSquareRowCol;
   constexpr std::array<SubSquareIndex, NumSubSquares> SubSquareAll = { 0,1,2,3,4,5,6,7,8 };

   using FieldIndex = int; // 0..80
   constexpr int NumFields = NumRowCol * NumRowCol;
}