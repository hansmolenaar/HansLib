#include "Grid.h"
#include "House.h"
#include "SweepNakedSingle.h"

#include <ostream>
#include <string>

using namespace Sudoku;

namespace
{
std::string CellValueToString(const std::optional<Value> &value)
{
   return !value.has_value() ? std::string{ValueUndefinedChar} : std::to_string(*value);
}

std::vector<std::string> ToStrings(const Grid &grid)
{
   std::vector<std::string> result;
   const std::string hline = "-------------------------------";
   const std::string vsep = "|";
   for (const auto row : IndexAll)
   {
      result.push_back({}); // Open new line
      if (row % 3 == 0)
      {
         result.back().append(hline);
         result.push_back({}); // Open new line
      }

      const House &house = House::getRow(row);
      result.back().append(vsep);
      for (const auto &cell : house.cells)
      {
         const Field field = cell.field;
         const auto value = grid.tryGetSolvedValue(field);
         result.back().append(" " + CellValueToString(value) + " ");
         if (cell.col % 3 == 2)
         {
            result.back().append(vsep);
         }
      }
   }
   result.push_back(hline);

   return result;
}

} // namespace

void Grid::setGiven(Index row, Index col, Value value)
{
   setGiven(Cell::rowCol2Field(row, col), value);
}

void Grid::setGiven(Field field, Value value)
{
   m_candidates.setCandidates(field, {value});
}

Index Grid::getNumCandidates() const
{
   return m_candidates.getNumCandidates();
}

std::optional<Value> Grid::tryGetSolvedValue(Field field) const
{
   return m_candidates[field].tryGetSolvedValue();
}

std::optional<Value> Grid::tryGetSolvedValue(Index row, Index col) const

{
   return tryGetSolvedValue(Cell::rowCol2Field(row, col));
}

bool Grid::isSolved() const
{
   return m_candidates.isSolved();
}

std::ostream &operator<<(std::ostream &os, const Sudoku::Grid &grid)
{
   const auto strings = ToStrings(grid);
   for (const auto &s : strings)
   {
      os << s << "\n";
   }
   return os;
}

void Grid::checkValidSudoku() const
{
   m_candidates.checkValidSudoku();
}

void Grid::update()
{
   SweepNakedSingle sweeper;
   ISweep::iterate(sweeper, m_candidates);
}
