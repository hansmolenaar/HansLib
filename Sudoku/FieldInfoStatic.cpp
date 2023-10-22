#pragma once

#include "FieldInfoStatic.h"
#include "MyException.h"

using namespace Sudoku;

namespace
{
   SubSquareRowColIndex ToSubSquareRowCol(RowColIndex index)
   {
      FieldInfoStatic::CheckRowColIndex(index);
      return index / NumSubSquareRowCol;
   }
}

const std::array<FieldInfoStatic, NumFields>& FieldInfoStatic::Instance()
{
   static std::array<FieldInfoStatic, NumFields> s_instance;
   static bool s_isinitialized = false;

   if (!s_isinitialized)
   {
      s_isinitialized = true;
      for (RowColIndex row : RowColAll)
      {
         const SubSquareRowColIndex subSquareRow = ToSubSquareRowCol(row);
         for (RowColIndex col : RowColAll)
         {
            const SubSquareRowColIndex subSquareCol = ToSubSquareRowCol(col);
            const SubSquareIndex subSquare = subSquareRow * NumSubSquareRowCol + subSquareCol;
            const int index = RowColToField(row, col);
            FieldInfoStatic& info = s_instance.at(index);
            info.Row = row;
            info.Col = col;
            info.SubSquare = subSquare;
         }
      }
   }

   return s_instance;
}

void FieldInfoStatic::CheckRowColIndex(RowColIndex index)
{
   if (index < 0) throw MyException("FieldInfoStatic::CheckRowColIndex index should be >= 0, actual " + std::to_string(index));
   if (index >= NumRowCol) throw MyException("FieldInfoStatic::CheckRowColIndex index should be < 9, actual " + std::to_string(index));
}

void FieldInfoStatic::CheckValue(Value value)
{
   if (value < 1) throw MyException("FieldInfoStatic::CheckValue value should be >= 1, actual " + std::to_string(value));
   if (value > NumValues) throw MyException("FieldInfoStatic::CheckValue value should be <= 9, actual " + std::to_string(value));
}