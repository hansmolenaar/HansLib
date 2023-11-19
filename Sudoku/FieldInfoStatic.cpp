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
   SubSquareRowColIndex ToSubSquareRowCol(RowColBoxIndex index)
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
      for (RowColBoxIndex row : RowColAll)
      {
         const RowColBoxIndex subSquareRow = ToSubSquareRowCol(row);
         for (RowColBoxIndex col : RowColAll)
         {
            const RowColBoxIndex subSquareCol = ToSubSquareRowCol(col);
            const BoxIndex box = subSquareRow * NumSubSquareRowCol + subSquareCol;
            const auto field = RowColToField(row, col);
            FieldInfoStatic& info = s_instance.at(field);
            info.Row = row;
            info.Col = col;
            info.Field = field;
            info.Box = box;
         }
      }
   }

   return s_instance;
}

void FieldInfoStatic::CheckRowColIndex(RowColBoxIndex index)
{
   if (index < 0) throw MyException("FieldInfoStatic::CheckRowColIndex index should be >= 0, actual " + std::to_string(index));
   if (index >= NumRowColBox) throw MyException("FieldInfoStatic::CheckRowColIndex index should be < 9, actual " + std::to_string(index));
}

void FieldInfoStatic::CheckValue(Value value)
{
   if (value < 1) throw MyException("FieldInfoStatic::CheckValue value should be >= 1, actual " + std::to_string(value));
   if (value > NumValues) throw MyException("FieldInfoStatic::CheckValue value should be <= 9, actual " + std::to_string(value));
}

const FieldSet& FieldInfoStatic::GetRow(RowColBoxIndex row)
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


const FieldSet& FieldInfoStatic::GetCol(RowColBoxIndex col)
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

const FieldSet& FieldInfoStatic::GetBox(RowColBoxIndex box)
{
   static std::vector<FieldSet> s_instance;
   if (s_instance.empty())
   {
      std::multimap< BoxIndex, FieldIndex> s2f;
      for (const auto& info : Instance())
      {
         s2f.emplace(info.Box, info.Field);
      }

      for (auto ssi : BoxAll)
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
   return s_instance.at(box);
}

const FieldSet& FieldInfoStatic::GetFieldSet(RowColBoxType type, RowColBoxIndex subSetIndex)
{
   if (type == RowColBoxType::Row) return GetRow(subSetIndex);
   if (type == RowColBoxType::Col) return GetCol(subSetIndex);
   if (type == RowColBoxType::Box) return GetBox(subSetIndex);
   throw MyException("ieldInfoStatic::GetFieldSet should not come here");
}