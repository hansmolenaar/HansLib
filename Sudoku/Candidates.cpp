#include "Candidates.h"
#include "MyAssert.h"

#include <algorithm>

using namespace Sudoku;

namespace str = std::ranges;

bool Candidates::contains(Value v) const
{
   return str::find(m_candidates, v) != m_candidates.end();
}

void Candidates::setGiven(Value v)
{
   Utilities::MyAssert(size() == NumIndex);
   setOnly({v});
}

bool Candidates::unsetValue(Value v)
{
   const auto found = str::find(m_candidates, v);
   const bool succes = found != m_candidates.end();
   if (succes)
   {
      m_candidates.erase(found);
   }

   return succes;
}

bool Candidates::isSolved() const
{
   return size() == 1;
}

Index Candidates::size() const
{
   return m_candidates.size();
}

std::optional<Value> Candidates::tryGetSolvedValue() const
{
   if (m_candidates.size() != 1)
   {
      return {};
   }
   return m_candidates.front();
}

void Candidates::setOnly(std::initializer_list<Value> values)
{
   for (auto v : values)
   {
      Utilities::MyAssert(contains(v));
   }
   m_candidates = values;
   Utilities::MyAssert(!m_candidates.empty());
   str::sort(m_candidates);
   m_candidates.erase(std::unique(m_candidates.begin(), m_candidates.end()), m_candidates.end());
}
