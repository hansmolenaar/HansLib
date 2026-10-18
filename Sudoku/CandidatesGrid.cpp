#include "CandidatesGrid.h"
#include "Cell.h"
#include "House.h"
#include "MyAssert.h"
#include "SweepNakedSingle.h"

#include <algorithm>
#include <bitset>
#include <numeric>
#include <string>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{

std::array<std::string, NumBoxIndex> SolvedCellToString(Value value)
{
   std::array<std::string, NumBoxIndex> result;
   result[0] = "   ";
   result[1] = " " + std::to_string(value) + " ";
   result[2] = "   ";
   return result;
}

std::array<std::string, NumBoxIndex> UnSolvedCellToString(const Candidates &candidates)
{
   std::array<std::string, NumBoxIndex> result;
   for (auto row : BoxIndexAll)
   {
      for (auto col : BoxIndexAll)
      {
         const Value val = col + row * NumBoxIndex + 1;
         if (candidates.contains(val))
         {
            result.at(row).append(std::to_string(val));
         }
         else
         {
            result.at(row).append(std::string{ValueUndefinedChar});
         }
      }
   }
   return result;
}

std::array<std::string, NumBoxIndex> CandidatesToStrings(const Candidates &candidates)
{
   const auto solvedValue = candidates.tryGetSolvedValue();
   if (solvedValue.has_value())
   {
      return SolvedCellToString(solvedValue.value());
   }
   else
   {
      return UnSolvedCellToString(candidates);
   }
}

std::vector<std::string> ToStrings(const CandidatesGrid &grid)
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
         const auto cstrings = CandidatesToStrings(grid[field]);

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

void CandidatesGrid::setCandidates(Field field, std::initializer_list<Value> values)
{
   m_candidates.at(field).setOnly(values);
}

void CandidatesGrid::removeCandidate(Field field, Value value)
{
   m_candidates.at(field).removeCandidate(value);
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
   const auto strings = ToStrings(cg);
   for (const auto &s : strings)
   {
      os << s + "\n";
   }
   return os;
}
CandidatesGrid CandidatesGrid::Difference(const CandidatesGrid &prvGrid, const CandidatesGrid &nxtGrid)
{
   CandidatesGrid result;
   for (Field field : FieldAll)
   {
      const Candidates &prv = prvGrid[field];
      const Candidates &nxt = nxtGrid[field];
      for (Value value : ValueAll)
      {
         const bool prvContains = prv.contains(value);
         const bool nxtContains = nxt.contains(value);
         MyAssert(!(!prvContains && nxtContains));
         if (prvContains && !nxtContains)
         {
            result.removeCandidate(field, value);
         }
      }
   }

   return result;
}
