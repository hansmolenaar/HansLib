#include "Potentials.h"
#include "Defines.h"
#include "FieldInfoStatic.h"

using namespace Sudoku;

Potentials::Potentials()
{
   for (auto& p : m_potentials)
   {
      p.SetAll();
   }
}

Value Potentials::GetSingleOrUndefined(FieldIndex field) const
{
   if (m_potentials.at(field).Count() == 1)
   {
      return m_potentials.at(field).getSingleValue();
   }
   return ValueUndefined;
}

void Potentials::SetSingle(FieldIndex field, Value value)
{
   m_potentials.at(field).SetSingle(value);
}

void Potentials::Set(FieldIndex field, Potential potential)
{
   m_potentials.at(field) = potential;
}

bool Potentials::isSingle(FieldIndex field) const
{
   return m_potentials.at(field).isSingle();
}

bool Potentials::unset(FieldIndex field, Value value) 
{
   return m_potentials.at(field).Unset(value);
}