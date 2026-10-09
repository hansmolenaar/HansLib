#include "Grid.h"
#include "House.h"

#include <algorithm>
#include <numeric>
#include <ostream>
#include <string>

using namespace Sudoku;
namespace str = std::ranges;

void Grid::setGiven(Field field, Value value)
{
   m_candidates.at(field).setGiven(value);
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
   for (Index row = 0; row < NumIndex; ++row)
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
