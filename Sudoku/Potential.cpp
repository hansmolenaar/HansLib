#include "Potential.h"
#include "Defines.h"

using namespace Sudoku;

Potential::Potential()
{
   SetNone();
}

void Potential::SetAll()
{
   for (auto i : RowColAll)
   {
      Set(i);
   }
}

void Potential::SetNone()
{
   for (auto i : RowColAll)
   {
      Unset(i);
   }
}

void Potential::Set(RowColIndex index)
{
   m_active.at(index) = true;
}

void Potential::Unset(RowColIndex index)
{
   m_active.at(index) = false;
}

bool Potential::operator()(RowColIndex index) const
{
   return m_active.at(index);
}

RowColIndex Potential::Count() const
{
   return static_cast<RowColIndex>(str::count_if(m_active, [](bool b) {return b; }));
}

Potential Potential::Combine(const Potential& pot1, const Potential& pot2, const Potential& pot3)
{
   Potential result;
   for (auto index : RowColAll)
   {
      if (pot1(index) && pot2(index) && pot3(index))
      {
         result.Set(index);
      }
   }
   return result;
}