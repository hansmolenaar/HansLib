#pragma once

#include "FieldInfoStatic.h"
#include "MyException.h"
#include "Defines.h"
#include <algorithm>
#include <vector>
#include <map>

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
            const auto field = RowColToField(row, col);
            FieldInfoStatic& info = s_instance.at(field);
            info.Row = row;
            info.Col = col;
            info.Field = field;
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

const FieldSet& FieldInfoStatic::GetRow(SubSetIndex row)
{
   static std::vector<FieldSet> s_instance;
   if (s_instance.empty())
   {
      for (auto r : RowColAll)
      {
         s_instance.push_back(FieldSet{});
         for (auto c : RowColAll)
         {
            s_instance.at(r).at(c) = RowColToField(r, c);
         }
      }
   }
   return s_instance.at(row);
}


const FieldSet& FieldInfoStatic::GetCol(SubSetIndex col)
{
   static std::vector<FieldSet> s_instance;
   if (s_instance.empty())
   {
      for (auto c : RowColAll)
      {
         s_instance.push_back(FieldSet{});
         for (auto r : RowColAll)
         {
            s_instance.at(c).at(r) = RowColToField(r, c);
         }
      }
   }
   return s_instance.at(col);
}

const FieldSet& FieldInfoStatic::GetSubSquare(SubSetIndex subSquare)
{
   static std::vector<FieldSet> s_instance;
   if (s_instance.empty())
   {
      std::multimap< SubSquareIndex, FieldIndex> s2f;
      for (const auto& info : Instance())
      {
         s2f.emplace(info.SubSquare, info.Field);
      }

      for (SubSquareIndex ssi = 0; ssi < NumSubSquares; ++ssi)
      {
         s_instance.push_back(FieldSet{});
         const auto range = s2f.equal_range(ssi);
         size_t pos = 0;
         for (auto itr = range.first; itr != range.second; ++itr)
         {
            s_instance.at(ssi).at(pos) = itr->second;
            ++pos;
         }
         str::sort(s_instance.back());
      }
   }
   return s_instance.at(subSquare);
}

const FieldSet& FieldInfoStatic::GetFieldSet(SubSetType type, SubSetIndex subSetIndex)
{
   if (type == SubSetType::Row) return GetRow(subSetIndex);
   if (type == SubSetType::Column) return GetCol(subSetIndex);
   if (type == SubSetType::SubSquare) return GetSubSquare(subSetIndex);
   throw MyException("ieldInfoStatic::GetFieldSet should not come here");
}