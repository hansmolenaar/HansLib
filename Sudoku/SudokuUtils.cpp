#include "SudokuUtils.h"
#include "CandidatesGrid.h"
#include "UniqueSorted.h"
#include <algorithm>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

Index SudokuUtils::countOccurence(const CandidatesGrid &grid, std::span<const Cell> cells, Value value)
{
   return str::count_if(cells, [&grid, value](Cell cell) { return grid[cell.field].contains(value); });
}

std::set<Value> SudokuUtils::getUniqueActiveValuesInCells(const CandidatesGrid &grid, std::span<const Field> fields)
{
   std::set<Value> result;
   boost::container::static_vector<Value, NumIndex> solvedValues;
   for (Field field : fields)
   {
      const auto &candidates = grid[field];
      // Store the known values
      if (candidates.isSolved())
      {
         solvedValues.push_back(candidates.tryGetSolvedValue().value());
      }
      else
      {
         for (Value value : ValueAll)
         {
            if (candidates.contains(value))
            {
               result.insert(value);
            }
         }
      }
   }
   for (Value value : solvedValues)
   {
      result.erase(value);
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
