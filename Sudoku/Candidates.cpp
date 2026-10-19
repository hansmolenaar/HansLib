#include "Candidates.h"
#include "MyAssert.h"

#include <algorithm>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

bool Candidates::contains(Value v) const
{
   return str::find(m_candidates, v) != m_candidates.end();
}

void Candidates::removeCandidate(Value v)
{
   const auto found = str::find(m_candidates, v);
   const bool succes = found != m_candidates.end();
   if (succes)
   {
      MyAssert(size() > 1);
      m_candidates.erase(found);
   }
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

std::array<std::string, NumBoxIndex> Candidates::ToStringsDifferences(const Candidates &candidates)
{
   std::array<std::string, NumBoxIndex> result;
   result[0] = "...";
   result[1] = "...";
   result[2] = "...";
   if (candidates.size() != NumValue)
   {
      for (Index idx : IndexAll)
      {
         const Value value = idx + 1;
         if (candidates.contains(value))
         {
            auto row = idx / NumBoxIndex;
            auto col = idx - row * NumBoxIndex;
            result[row][col] = char('0' + value);
         }
      }
   }
   return result;
}

std::array<std::string, NumBoxIndex> Candidates::ToStrings(const Candidates &candidates)
{
   std::array<std::string, NumBoxIndex> result;

   const auto solvedValue = candidates.tryGetSolvedValue();
   if (solvedValue)
   {
      result[0] = "   ";
      result[1] = "   ";
      result[2] = "   ";
      result[1][1] = char('0' + solvedValue.value());
   }
   else
   {
      result[0] = "...";
      result[1] = "...";
      result[2] = "...";
      for (Index idx : IndexAll)
      {
         const Value value = idx + 1;
         if (candidates.contains(value))
         {
            auto row = idx / NumBoxIndex;
            auto col = idx - row * NumBoxIndex;
            result[row][col] = char('0' + value);
         }
      }
   }
   return result;
}
