#include "CandidatesGrid.h"
#include "Cell.h"
#include "House.h"
#include "MyAssert.h"
#include "StreamUtils.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "SweepNakedSingle.h"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <numeric>
#include <string>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{

typedef std::array<std::string, NumBoxIndex> (*candidatesToStringsPtr)(const Candidates &);

std::vector<std::string> ToStrings(const CandidatesGrid &grid, candidatesToStringsPtr fun)
{
   std::vector<std::string> result;
   const std::string hsep = "*-----------------*-----------------*-----------------*";
   const std::string box_hsep_top_bot = "*******************************************************";
   const std::string box_hsep_x = box_hsep_top_bot; //"*             *             *             *";
   const std::string vsep = " | ";
   const std::string box_vsep = "*";

   for (const auto row : IndexAll)
   {
      if (row == 0)
      {
         result.push_back(box_hsep_top_bot);
      }
      else if (row % 3 == 0)
      {
         result.push_back(box_hsep_x);
      }
      else
      {
         result.push_back(hsep);
      }
      const size_t pos = result.size();
      result.push_back({}); // Open 3 new lines
      result.push_back({});
      result.push_back({});

      const House &house = House::getRow(row);
      for (const auto &cell : house.cells)
      {
         const Field field = cell.field;
         const auto cstrings = fun(grid[field]);

         for (auto p : BoxIndexAll)
         {
            if (cell.col == 0)
            {
               result.at(pos + p).append(box_vsep + " ");
            }
            else if ((cell.col) % 3 == 0)
            {
               result.at(pos + p).append(" " + box_vsep + " ");
            }
            else
            {
               result.at(pos + p).append(vsep);
            }
            result.at(pos + p).append(cstrings.at(p));
         }
      }
      for (size_t p = pos; p < pos + NumBoxIndex; ++p)
      {
         result.at(p).append(" " + box_vsep);
      }
   }

   result.push_back(box_hsep_top_bot);
   return result;
}

} // namespace

Index CandidatesGrid::getNumCandidates() const
{
   return std::accumulate(m_candidates.begin(), m_candidates.end(), static_cast<Index>(0),
                          [](Index acc, const Candidates &c) { return acc + c.size(); });
}

bool CandidatesGrid::setCandidates(Field field, std::initializer_list<Value> values)
{
   return m_candidates.at(field).setOnly(values);
}

bool CandidatesGrid::removeCandidate(Field field, Value value)
{
   return m_candidates.at(field).removeCandidate(value);
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

   // Check rule of 1
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
   result.checkValidSudoku();
   return result;
}

CandidatesGrid CandidatesGrid::FromValues(const std::array<Value, NumField> &values)

{
   CandidatesGrid result;
   for (Field f : FieldAll)
   {
      if (values[f] != 0)
      {
         result.setGiven(f, values[f]);
      }
   }
   result.checkValidSudoku();
   return result;
}

bool CandidatesGrid::isSolved() const
{
   return str::all_of(FieldAll, [this](Field f) { return m_candidates[f].isSolved(); });
}

std::ostream &operator<<(std::ostream &os, const Sudoku::CandidatesGrid &cg)
{
   os << ToStrings(cg, &Candidates::ToStrings);
   return os;
}

std::vector<std::string> CandidatesGrid::DifferenceToStrings(const CandidatesGrid &prvGrid,
                                                             const CandidatesGrid &nxtGrid)
{
   CandidatesGrid result;
   Field numChanges = 0;
   for (Field field : FieldAll)
   {
      const Candidates &prv = prvGrid[field];
      const Candidates &nxt = nxtGrid[field];
      if (prv != nxt)
      {
         for (Value value : ValueAll)
         {
            const bool prvContains = prv.contains(value);
            const bool nxtContains = nxt.contains(value);
            MyAssert(!(!prvContains && nxtContains));
            if (prvContains == nxtContains)
            {
               result.removeCandidate(field, value);
            }
         }
         numChanges += result[field].size();
      }
   }

   auto retval = ToStrings(result, &Candidates::ToStringsDifferences);
   retval.push_back("Number of changes = " + std::to_string(numChanges));
   return retval;
}
