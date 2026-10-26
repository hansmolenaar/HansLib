#include "SudokuUtils.h"
#include "CandidatesGrid.h"
#include "SudokuDefines.h"
#include <algorithm>
#include <boost/container/static_vector.hpp>

using namespace Sudoku;

namespace str = std::ranges;

Index SudokuUtils::countOccurence(const CandidatesGrid &grid, std::span<const Cell> cells, Value value)
{
   return str::count_if(cells, [&grid, value](Cell cell) { return grid[cell.field].contains(value); });
}

std::set<Value> SudokuUtils::getUniqueActiveValuesInCells(const CandidatesGrid &grid, std::span<const Field> fields)
{
   std::array<bool, NumValue + 1> activeValues = {false, false, false, false, false,
                                                  false, false, false, false, false}; // TODO is this necessary?

   for (Field field : fields)
   {
      for (Value value : grid[field])
      {
         activeValues[value] = true;
      }
   }

   const auto valueIsSolved = SudokuUtils::getSolvedValues(grid, fields);
   std::set<Value> result;
   for (Value value : ValueAll)
   {
      if (activeValues[value] && !valueIsSolved.test(value))
      {
         result.insert(value);
      }
   }
   return result;
}

std::set<Value> SudokuUtils::getUniqueActiveValuesInCells(const CandidatesGrid &grid, std::span<const Cell> cells)
{
   std::vector<Field> fields(cells.size());
   str::transform(cells, fields.begin(), [](Cell cell) { return cell.field; });
   return getUniqueActiveValuesInCells(grid, fields);
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
