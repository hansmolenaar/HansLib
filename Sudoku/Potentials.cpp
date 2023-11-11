#include "Potentials.h"
#include "Defines.h"
#include "FieldInfoStatic.h"

using namespace Sudoku;

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