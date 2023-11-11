#pragma once

#include "Diagram.h"
#include "Defines.h"
#include "Potential.h"
#include "FieldInfoStatic.h"
#include "MyException.h"
#include "Functors.h"
#include <algorithm>
#include <sstream>
#include <unordered_set>

using namespace Sudoku;

namespace
{
   void CheckSubSet(const ValueSet& valueSet)
   {
      const Functors::IsInIncludeBounds<Value> inBounds{ ValueUndefined, ValueMax };
      std::bitset< SubSetSize> inSet;
      for (auto value : valueSet)
      {
         if (!inBounds(value))
         {
            throw MyException("Diagram check, unexpected 1 <= value <= 9, actual: " + std::to_string(value));
         }
         if (value == ValueUndefined) continue;
         if (inSet.test(value - 1))
         {
            throw MyException("Diagram check, subset already contains value " + std::to_string(value));
         }
         inSet.set(value - 1);
      }
   }

   void Check(const std::array<Value, NumFields>& values)
   {
      for (auto sst : SubSetTypeAll)
      {
         for (auto subSetIndex : SubSetsAll)
         {
            const auto& fields = FieldInfoStatic::GetFieldSet(sst, subSetIndex);
            ValueSet valueSet;
            str::transform(fields, valueSet.begin(), [&values](FieldIndex field) {return values.at(field); });
            CheckSubSet(valueSet);
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
      const auto value = values.getSingleOrUndefined(field);
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

Sudoku::Potentials Diagram::getPotentials() const
{
   Potentials result;
   for (FieldIndex field = 0; field < NumFields; ++field)
   {
      if (m_state.at(field) != ValueUndefined)
      {
         result.setSingle(field, m_state.at(field));
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
   const Functors::IsEqualTo<Value> isUndefined{ ValueUndefined };
   if (str::any_of(m_state, isUndefined))
   {
      return false;
   }

   for (auto type : SubSetTypeAll)
   {
      for (auto subSetIndex : SubSetsAll)
      {
         const auto& fields = FieldInfoStatic::GetFieldSet(type, subSetIndex);
         ValueSet valueSet;
         str::transform(fields, valueSet.begin(), [this](FieldIndex field) {return m_state.at(field); });
         std::bitset<SubSetSize> isSet;
         for (Value value : valueSet)
         {
            isSet.set(value - 1);
         }
         if (!isSet.all()) return false;
      }
   }

   return true;
}