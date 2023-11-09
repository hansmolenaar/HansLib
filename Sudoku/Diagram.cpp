#pragma once

#include "Diagram.h"
#include "Defines.h"
#include "Potential.h"
#include "FieldInfoStatic.h"
#include "MyException.h"

#include <algorithm>
#include <sstream>
#include <unordered_set>

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
            if (v < ValueUndefined || v > NumValues)
            {
               throw MyException("Diagram check, unexpected 1 <= value <= 9, actual: " + std::to_string(v));
            }
            const FieldInfoStatic& info = infoAll.at(f);
            if (rowContains.at(info.Row).ContainsValue(v))
            {
               throw MyException("Diagram check, row " + std::to_string(info.Row) + " already contains value " + std::to_string(v));
            }
            if (colContains.at(info.Col).ContainsValue(v))
            {
               throw MyException("Diagram check, column " + std::to_string(info.Col) + " already contains value " + std::to_string(v));
            }
            if (subSquareContains.at(info.SubSquare).ContainsValue(v))
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
      FieldInfoStatic::CheckValue(iv.second);
      values.at(iv.first) = iv.second;
   }

   Check(values);
   return Diagram(values);
}

Diagram Diagram::Create(const Potentials& values)
{
   std::unordered_map<FieldIndex, Value> map;
   const std::array<FieldInfoStatic, NumFields>& infoAll = FieldInfoStatic::Instance();
   for (FieldIndex field = 0; field < NumFields; ++field)
   {
      const auto value = values.GetSingleOrUndefined(field);
      if (value != ValueUndefined)
      {
         map[field] = value;
      }
   }
   return Create(map);
}

Diagram Diagram::Create(const  std::array<Value, NumFields> values)
{
   std::unordered_map<FieldIndex, Value> map;
   const std::array<FieldInfoStatic, NumFields>& infoAll = FieldInfoStatic::Instance();
   FieldIndex field = 0;
   for (Value value : values)
   {
      if (value != ValueUndefined)
      {
         map[field] = value;
      }
      ++field;
   }
   return Create(map);
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

std::array<Potential, NumFields> Diagram::getPotentials() const
{
   const std::array<FieldInfoStatic, NumFields>& infoAll = FieldInfoStatic::Instance();

   std::array<Potential, NumRowCol> availableInRow;
   std::array<Potential, NumRowCol> availableInCol;
   std::array<Potential, NumSubSquares> availableInSubSquare;

   for (auto index : RowColAll)
   {
      availableInRow.at(index).SetAll();
      availableInCol.at(index).SetAll();
   }
   for (auto index : SubSquareAll)
   {
      availableInSubSquare.at(index).SetAll();
   }

   for (FieldIndex f = 0; f < NumFields; ++f)
   {
      const Value v = (*this)(f);
      if (v != ValueUndefined)
      {
         const auto& info = infoAll.at(f);
         availableInRow.at(info.Row).Unset(v);
         availableInCol.at(info.Col).Unset(v);
         availableInSubSquare.at(info.SubSquare).Unset(v);
      }
   }

   std::array<Potential, NumFields> result;
   for (FieldIndex f = 0; f < NumFields; ++f)
   {
      const Value v = (*this)(f);
      if (v == ValueUndefined)
      {
         const auto& info = infoAll.at(f);
         result.at(f) = Potential::Combine(availableInRow.at(info.Row), availableInCol.at(info.Col), availableInSubSquare.at(info.SubSquare));
      }
   }

   return  result;
}

Sudoku::Potentials Diagram::getPotentialS() const
{
   Potentials result;
   for (FieldIndex field = 0; field < NumFields; ++field)
   {
      if (m_state.at(field) != ValueUndefined)
      {
         result.SetSingle(field, m_state.at(field));
      }
   }
   return result;
}

std::string Diagram::toString() const
{
   std::ostringstream stream;
   stream << *this;
   return stream.str();
}

bool Diagram::isSolved() const
{
   const std::array<FieldInfoStatic, NumFields>& infoAll = FieldInfoStatic::Instance();

   if (str::any_of(m_state, [](Value val) {return val == ValueUndefined; }))
   {
      return false;
   }

   for (auto row : RowColAll)
   {
      std::unordered_set<RowColIndex> isSet;
      for (auto field : FieldInfoStatic::GetRow(row))
      {
         isSet.insert(m_state.at(field));
      }
      if (isSet.size() != NumRowCol)
      {
         return false;
      }
   }

   for (auto col : RowColAll)
   {
      std::unordered_set<RowColIndex> isSet;
      for (auto field : FieldInfoStatic::GetCol(col))
      {
         isSet.insert(m_state.at(field));
      }
      if (isSet.size() != NumRowCol)
      {
         return false;
      }
   }

   for (auto ssq : SubSquareAll)
   {
      std::unordered_set<RowColIndex> isSet;
      for (auto field : FieldInfoStatic::GetSubSquare(ssq))
      {
         isSet.insert(m_state.at(field));
      }
      if (isSet.size() != NumRowCol)
      {
         return false;
      }
   }

   return true;
}

const std::array<Value, NumFields>& Diagram::getState() const
{
   return m_state;
}