#include "Potential.h"
#include "Defines.h"
#include "FieldInfoStatic.h"
using namespace Sudoku;

Potential::Potential()
{
   SetNone();
}

void Potential::SetAll()
{
   for (auto i : ValueAll)
   {
      Set(i);
   }
}

void Potential::SetNone()
{
   for (auto i : ValueAll)
   {
      Unset(i);
   }
}

void Potential::SetSingle(Value value)
{
   SetNone();
   Set(value);
}

void Potential::Set(Value value)
{
   FieldInfoStatic::CheckValue(value);
   m_active.at(value-1) = true;
}

bool Potential::Unset(Value value)
{
   FieldInfoStatic::CheckValue(value);
   const bool changed = m_active.at(value - 1);
   m_active.at(value-1) = false;
   return changed;
}

bool Potential::ContainsValue(Value value) const
{
   FieldInfoStatic::CheckValue(value);
   return m_active.at(value-1);
}

RowColIndex Potential::Count() const
{
   return static_cast<RowColIndex>(str::count_if(m_active, [](bool b) {return b; }));
}

Potential Potential::Combine(const Potential& pot1, const Potential& pot2, const Potential& pot3)
{
   Potential result;
   for (auto value : ValueAll)
   {
      if (pot1.ContainsValue(value) && pot2.ContainsValue(value) && pot3.ContainsValue(value))
      {
         result.Set(value);
      }
   }
   return result;
}

Value Potential::getSingleValue() const
{
   Value result = ValueUndefined;
   for (auto val : ValueAll)
   {
      if (m_active.at(val - 1))
      {
         if (result == ValueUndefined)
         {
            result = val;
         }
         else
         {
            throw MyException("Potential::getSingleValue() contains " + std::to_string(result) + " and " + std::to_string(val));
         }
      }
   }

   if (result == ValueUndefined)
   {
      throw MyException("Potential::getSingleValue no value found");
   }

   return result;
}

bool Potential::isSingle() const
{
   return Count() == 1;
}