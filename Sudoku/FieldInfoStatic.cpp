#pragma once

#include "FieldInfoStatic.h"
#include "MyException.h"
#include "Defines.h"
#include "SudokuDefines.h"
#include <algorithm>
#include <vector>
#include <map>
#include <numeric>

using namespace Sudoku;
using namespace FieldInfoStatic;

using SubSquareRowColIndex = int; // 0..2
constexpr int NumBox_RowColPositons = 3;

namespace
{
   struct FieldInfoStatiC
   {
      RowColBoxIndex    Row;
      RowColBoxIndex    Col;
      RowColBoxIndex    Box;
      FieldIndex        Field;
   };

   SubSquareRowColIndex ToSubSquareRowCol(RowColBoxIndex index)
   {
      FieldInfoStatic::CheckRowColIndex(index);
      return index / NumBox_RowColPositons;
   }

   const std::array<FieldInfoStatiC, NumFields>& Instance()
   {
      static std::array<FieldInfoStatiC, NumFields> s_instance;
      static bool s_isinitialized = false;

      if (!s_isinitialized)
      {
         s_isinitialized = true;
         for (RowColBoxIndex row : RowColBoxAll)
         {
            const RowColBoxIndex subSquareRow = ToSubSquareRowCol(row);
            for (RowColBoxIndex col : RowColBoxAll)
            {
               const RowColBoxIndex subSquareCol = ToSubSquareRowCol(col);
               const RowColBoxPosition box = subSquareRow * NumBox_RowColPositons + subSquareCol;
               const auto field = RowColToField(row, col);
               FieldInfoStatiC& info = s_instance.at(field);
               info.Row = row;
               info.Col = col;
               info.Field = field;
               info.Box = box;
            }
         }
      }

      return s_instance;
   }


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
      for (auto r : RowColBoxAll)
      {
         s_instance.push_back(FieldSet{});
         for (auto c : RowColBoxAll)
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
      for (auto c : RowColBoxAll)
      {
         s_instance.push_back(FieldSet{});
         for (auto r : RowColBoxAll)
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
      std::multimap< RowColBoxIndex, FieldIndex> s2f;
      for (const auto& info : Instance())
      {
         s2f.emplace(info.Box, info.Field);
      }

      for (auto ssi : RowColBoxAll)
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

const std::array<FieldIndex, NumFields>& FieldInfoStatic::getAllFields()
{
   static std::array<FieldIndex, NumFields> s_instance;
   static bool s_isinitialized = false;
   if (!s_isinitialized)
   {
      s_isinitialized = true;
      std::iota(s_instance.begin(), s_instance.end(), 0);
   }
   return s_instance;
}

RowColBoxIndex FieldInfoStatic::FieldToBox(FieldIndex field)
{
   return Instance().at(field).Box;
}


RowColBoxIndex FieldInfoStatic::FieldToRow(FieldIndex field)
{
   return Instance().at(field).Row;
}


RowColBoxIndex FieldInfoStatic::FieldToCol(FieldIndex field)
{
   return Instance().at(field).Col;
}