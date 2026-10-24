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
                                                  false, false, false, false}; // TODO is this necessary?
   std::array<bool, NumValue + 1> solvedValues = activeValues;

   for (Field field : fields)
   {
      const auto &candidates = grid[field];
      const auto solved = candidates.tryGetSolvedValue();
      // Store the known values
      if (solved.has_value())
      {
         solvedValues[solved.value()] = true;
      }
      else
      {
         for (Value value : ValueAll)
         {
            if (candidates.contains(value))
            {
               activeValues[value] = true;
            }
         }
      }
   }

   std::set<Value> result;
   for (Value value : ValueAll)
   {
      if (activeValues[value] && !solvedValues[value])
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
