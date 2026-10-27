#include "SudokuUtils.h"
#include "CandidatesGrid.h"
#include <algorithm>
#include <bitset>

using namespace Sudoku;

namespace str = std::ranges;

Index SudokuUtils::countOccurence(const CandidatesGrid &grid, std::span<const Cell> cells, Value value)
{
   return str::count_if(cells, [&grid, value](Cell cell) { return grid[cell.field].contains(value); });
}

boost::container::static_vector<Value, NumValue> SudokuUtils::getUniqueActiveValuesInCells(const CandidatesGrid &grid,
                                                                                           std::span<const Cell> cells)
{
   std::bitset<NumValue + 1> activeValues;
   for (Cell cell : cells)
   {
      for (Value value : grid[cell.field])
      {
         activeValues[value] = true;
      }
   }

   const auto valueIsSolved = SudokuUtils::getSolvedValues(grid, cells);
   boost::container::static_vector<Value, NumValue> result;
   for (Value value : ValueAll)
   {
      if (activeValues[value] && !valueIsSolved.test(value))
      {
         result.push_back(value);
      }
   }
   return result;
}

boost::container::static_vector<Cell, NumCellsInHouse> SudokuUtils::getNotSolvedCells(const CandidatesGrid &grid,
                                                                                      const House &house)
{
   boost::container::static_vector<Cell, NumCellsInHouse> result(NumCellsInHouse);
   const auto last = std::copy_if(house.cells.begin(), house.cells.end(), result.begin(),
                                  [&grid](Cell cell) { return !grid[cell.field].isSolved(); });
   result.erase(last, result.end());
   return result;
}

std::bitset<NumValue + 1> SudokuUtils::getSolvedValues(const CandidatesGrid &grid, std::span<const Cell> cells)
{
   std::bitset<NumValue + 1> result;
   for (auto cell : cells)
   {
      const auto &candidates = grid[cell.field];
      const auto solved = candidates.tryGetSolvedValue();
      if (solved.has_value())
      {
         result.set(solved.value());
      }
   }
   return result;
}

std::bitset<NumValue + 1> SudokuUtils::getSolvedValues(const CandidatesGrid &grid, std::span<const Field> fields)
{
   std::bitset<NumValue + 1> result;
   for (auto field : fields)
   {
      const auto &candidates = grid[field];
      const auto solved = candidates.tryGetSolvedValue();
      if (solved.has_value())
      {
         result.set(solved.value());
      }
   }
   return result;
}

SudokuUtils::Value2Fields SudokuUtils::getValue2Fields(const CandidatesGrid &grid, std::span<Field> fields)
{
   Value2Fields result;
   for (auto field : fields)
   {
      for (auto value : grid[field])
      {
         result[value].push_back(field);
      }
   }
   return result;
}
