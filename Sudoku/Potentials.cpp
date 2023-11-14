#include "Potentials.h"
#include "Defines.h"
#include "FieldInfoStatic.h"

using namespace Sudoku;

namespace
{
   std::string ToStringPerType(Potentials& potentials, SubSetType type)
   {
      std::string result = "\n\nType " + std::to_string(static_cast<int>(type)) + "\n\n";
      for (auto index : SubSetsAll)
      {
         bool first = true;
         for (const auto* pot : potentials.getSubSetPotentials(type, index))
         {
            if (first)
            {
               first = false;
            }
            else
            {
               result += " ";
            }
            result += pot->toString();
         }
         result += "\n";
      }
      return result;
   }

   std::array<std::string, NumFields> GetPotentialsPerField(Potentials& potentials)
   {
      std::array<std::string, NumFields> result;
      for (FieldIndex field = 0; field < NumFields; ++field)
      {
         result.at(field) = potentials.get(field).toString();
      }
      return result;
   }

   std::string FormatPotentialsPerField(const std::array<std::string, NumFields>& perField)
   {
      std::string result;
      std::array<size_t, NumFields> fieldSizes;
      str::transform(perField, fieldSizes.begin(), [](const std::string& str) {return str.size(); });
      const auto maxFieldSize = str::max(fieldSizes);

      for (auto row : RowColAll)
      {
         std::string line;
         for (auto field : FieldInfoStatic::GetRow(row))
         {
             auto fieldAsString = perField.at(field);
             while (fieldAsString.size() < maxFieldSize + 1)
             {
                fieldAsString += " ";
             }
             line += fieldAsString;
         }
         result += line + "\n";
      }
      return result;
   }

}  // namespace {}


Potentials::Potentials()
{
   for (auto& p : m_potentials)
   {
      p.setAll();
   }
}

Value Potentials::getSingleOrUndefined(FieldIndex field) const
{
   if (m_potentials.at(field).count() == 1)
   {
      return m_potentials.at(field).getSingleValue();
   }
   return ValueUndefined;
}

void Potentials::setSingle(FieldIndex field, Value value)
{
   m_potentials.at(field).setSingle(value);
}

bool Potentials::isSingle(FieldIndex field) const
{
   return m_potentials.at(field).isSingle();
}

bool Potentials::unset(FieldIndex field, Value value)
{
   return m_potentials.at(field).unset(value);
}

SubSetPotentials Potentials::getSubSetPotentials(SubSetType type, SubSetIndex index)
{
   const auto& fields = FieldInfoStatic::GetFieldSet(type, index);
   SubSetPotentials result;
   str::transform(fields, result.begin(), [this](FieldIndex f) { return &(m_potentials.at(f)); });
   return result;
}

const Potential& Potentials::get(FieldIndex field) const
{
   return m_potentials.at(field);
}

std::string Potentials::toString(const SubSetPotentials potentials)
{
   std::string result;
   bool first = true;
   for (const auto* pot : potentials)
   {
      if (first)
      {
         first = false;
      }
      else
      {
         result += " ";
      }
      result += pot->toString();
   }
   return result;
}

std::string Potentials::toString()
{
   std::string result;
   for (auto type : SubSetTypeAll)
   {
      result += ToStringPerType(*this, type);
   }

   const auto fields = GetPotentialsPerField(*this);
   const auto fullFieldPotentials = FormatPotentialsPerField(fields);
   result += "\n\n" + fullFieldPotentials;
   return result;
}

bool Potentials::isSolved() const
{
   return str::all_of(m_potentials, [](const auto& pot) {return pot.isSingle(); });
}