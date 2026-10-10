#include "Grid.h"
#include "House.h"
#include "MyAssert.h"

#include <algorithm>
#include <bitset>
#include <numeric>
#include <ostream>
#include <string>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

void Grid::setGiven(Field field, Value value)
{
   m_candidates.at(field).setGiven(value);
}

void Grid::setGiven(Index row, Index col, Value value)
{
   setGiven(Cell::rowCol2Field(row, col), value);
}

Index Grid::getNumCandidates() const
{
   return std::accumulate(m_candidates.begin(), m_candidates.end(), static_cast<Index>(0),
                          [](Index acc, const Candidates &c) { return acc + c.size(); });
}

std::optional<Value> Grid::tryGetSolvedValue(Field field) const
{
   return m_candidates.at(field).tryGetSolvedValue();
}

bool Grid::isSolved() const
{
   return str::all_of(m_candidates, [](const auto &c) { return c.isSolved(); });
}

std::ostream &operator<<(std::ostream &os, const Sudoku::Grid &grid)
{
   for (const auto row : IndexAll)
   {
      const House &house = House::getRow(row);
      os << "|";
      for (const auto &cell : house.cells)
      {
         const Field field = cell.field;
         const auto value = grid.tryGetSolvedValue(field);
         os << " " << (!value.has_value() ? std::string{ValueUndefinedChar} : std::to_string(*value)) + " ";
      }
      os << "|\n";
   }
   return os;
}

void Grid ::unsetCandidate(Field field, Value value)
{
   m_candidates.at(field).unsetValue(value);
}

void Grid::checkValidSudoku() const
{

   // Check if value within bounds
   constexpr Value lwr = ValueAll.front();
   constexpr Value upr = ValueAll.back();
   for (const auto f : FieldAll)
   {
      const auto value = tryGetSolvedValue(f);
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
         const auto value = tryGetSolvedValue(cell.field);
         if (value.has_value())
         {
            MyAssert(!isUsed.test(value.value()));
            isUsed.set(value.value());
         }
      }
   }
}
