#pragma once

#include "Diagram.h"
#include "Defines.h"
#include "Potential.h"
#include "FieldInfoStatic.h"
#include "MyException.h"

#include <algorithm>

using namespace Sudoku;

namespace
{
   void Check(const std::array<Value, NumFields>& values)
   {
      const std::array<FieldInfoStatic, NumFields>& infoAll = FieldInfoStatic::Instance();
      std::array<Potential, NumRowCol> rowContains;
      std::array<Potential, NumRowCol> colContains;
      std::array<Potential, NumSubSquares> subSquareContains;
      for (FieldIndex f = 0; f < NumFields; ++f)
      {
         const Value v = values.at(f);
         if (v != ValueUndefined)
         {
            if (v < ValueUndefined || v >= NumValues)
            {
               throw MyException("Diagram check, unexpected 1 << value << 9, actual: " + std::to_string(v));
            }
            const FieldInfoStatic& info = infoAll.at(f);
            if (rowContains.at(info.Row)(v))
            {
               throw MyException("Diagram check, row " + std::to_string(info.Row) + " already contains value " + std::to_string(v));
            }
            if (colContains.at(info.Col)(v))
            {
               throw MyException("Diagram check, column " + std::to_string(info.Col) + " already contains value " + std::to_string(v));
            }
            if (subSquareContains.at(info.SubSquare)(v))
            {
               throw MyException("Diagram check, SubSquare " + std::to_string(info.SubSquare) + " already contains value " + std::to_string(v));
            }
            rowContains.at(info.Row).Set(v);
            colContains.at(info.Col).Set(v);
            subSquareContains.at(info.SubSquare).Set(v);
         }
      }
   }
}

Diagram::Diagram(std::array<Value, NumFields> values) : m_state(std::move(values))
{
}

Diagram Diagram::Create(const std::unordered_map<FieldIndex, Value>& input)
{
   std::array<Value, NumFields> values;
   str::fill(values, ValueUndefined);
   for (auto iv : input)
   {
      values.at(iv.first) = iv.second;
   }

   Check(values);
   return Diagram(values);
}

Value Diagram::operator()(FieldIndex field) const
{
   return m_state.at(field);
}

std::ostream& operator<<(std::ostream& os, const Sudoku::Diagram& diagram)
{
   const std::array<FieldInfoStatic, NumFields>& infoAll = FieldInfoStatic::Instance();
   for (RowColIndex row = 0; row < NumRowCol; ++row)
   {
      os << "|";
      for (RowColIndex col = 0; col < NumRowCol; ++col)
      {
         const FieldIndex field = FieldInfoStatic::RowColToField(row, col);
         const Value value = diagram(field);
         os << " " << (value == ValueUndefined ? "-" : std::to_string(value)) + " ";
      }
      os << "|\n";
   }
   return os;
}