#include "CandidatesGrid.h"
#include "Cell.h"
#include "House.h"
#include "MyAssert.h"
#include "SweepNakedSingle.h"

#include <algorithm>
#include <bitset>
#include <numeric>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

Index CandidatesGrid::getNumCandidates() const
{
   return std::accumulate(m_candidates.begin(), m_candidates.end(), static_cast<Index>(0),
                          [](Index acc, const Candidates &c) { return acc + c.size(); });
}

void CandidatesGrid::setCandidates(Field field, std::initializer_list<Value> values)
{
   m_candidates.at(field).setOnly(values);
}

void CandidatesGrid::removeCandidate(Field field, Value value)
{
   m_candidates.at(field).unsetValue(value);
}

void CandidatesGrid::setGiven(Field field, Value value)
{
   MyAssert((*this)[field].size() == NumValue);
   setCandidates(field, {value});
}

void CandidatesGrid::setGiven(Index row, Index col, Value value)
{
   const Field field = Cell::getFromRowCol(row, col).field;
   setGiven(field, value);
}

void CandidatesGrid::update()
{
   SweepNakedSingle sweeper;
   ISweep::iterate(sweeper, *this);
}

void CandidatesGrid::checkValidSudoku() const
{

   // Check if value within bounds
   constexpr Value lwr = ValueAll.front();
   constexpr Value upr = ValueAll.back();
   for (const auto f : FieldAll)
   {
      const auto value = (*this)[f].tryGetSolvedValue();
      if (value.has_value())
      {
         MyAssert(value >= lwr && value <= upr);
      }
   }

   for (const auto &house : House::getAllHouses())
   {
      std::bitset<NumValue + 1> isUsed; // all 0; 1 extra because Value is 1-based
      for (const auto &cell : house.cells)
      {
         const auto value = (*this)[cell.field].tryGetSolvedValue();
         if (value.has_value())
         {
            MyAssert(!isUsed.test(value.value()));
            isUsed.set(value.value());
         }
      }
   }
}

CandidatesGrid CandidatesGrid::FromString(const std::string &str)
{
   MyAssert(str.length() == NumField);
   CandidatesGrid result;
   for (Field f : FieldAll)
   {
      const char c = str[f];
      if (c != ValueUndefinedChar)
      {
         const Value value = c - '0';
         result.setGiven(f, value);
      }
   }
   return result;
}

bool CandidatesGrid::isSolved() const
{
   return str::all_of(FieldAll, [this](Field f) { return m_candidates[f].isSolved(); });
}
