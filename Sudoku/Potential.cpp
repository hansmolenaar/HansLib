#include "Potential.h"
#include "Defines.h"
#include "FieldInfoStatic.h"
using namespace Sudoku;

Potential::Potential()
{
   setAll();
}

Potential Potential::Create(std::initializer_list<Value> values)
{
   Potential result;
   result.setNone();
   for (auto val : values)
   {
      result.set(val);
   }
   return result;
}

void Potential::setAll()
{
   m_active.set();
}

void Potential::setNone()
{
   m_active.reset();
}

void Potential::setSingle(Value value)
{
   setNone();
   set(value);
}

void Potential::set(Value value)
{
   FieldInfoStatic::CheckValue(value);
   m_active.set(value - 1);
}

bool Potential::unset(Value value)
{
   FieldInfoStatic::CheckValue(value);
   const bool changed = m_active.test(value - 1);
   m_active.reset(value - 1);
   return changed;
}

bool Potential::containsValue(Value value) const
{
   //FieldInfoStatic::CheckValue(value);
   return m_active.test(value - 1);
}

RowColIndex Potential::count() const
{
   return static_cast<RowColIndex>(m_active.count());
}

Value Potential::getSingleValue() const
{
   Value result = ValueUndefined;
   for (auto val : ValueAll)
   {
      if (m_active.test(val - 1))
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
   return count() == 1;
}

bool Potential::IsSingle(const Potential& pot)
{
   return pot.isSingle();
}

PotentialValues Potential::getPotentialValues() const
{
   PotentialValues result;
   for (auto value : ValueAll)
   {
      if (m_active.test(value - 1))
      {
         result.push_back(value);
      }
   }
   return result;
}

PotentialValues Potential::getIntersection(const Potential& pot1, const Potential& pot2)
{
   PotentialValues result;
   for (auto v : ValueAll)
   {
      if (pot1.containsValue(v) && pot2.containsValue(v))
      {
         result.push_back(v);
      }
   }
   return result;
}

PotentialValues Potential::getUnion(const Potential& pot1, const Potential& pot2)
{
   PotentialValues result;
   for (auto v : ValueAll)
   {
      if (pot1.containsValue(v) || pot2.containsValue(v))
      {
         result.push_back(v);
      }
   }
   return result;
}

std::string Potential::toString() const
{
   std::string result = "(";
   bool first = true;
   for (auto value : getPotentialValues())
   {
      if (first)
      {
         first = false;
         result += std::to_string(value);
      }
      else
      {
         result += "," + std::to_string(value);
      }
   }
   result += ")";
   return result;
}