#include "SweepHiddenTuple.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "House.h"
#include "StreamUtils.h"
#include "SudokuDefines.h"

#include <algorithm>
#include <iostream>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{
// Forget solved values
std::vector<Value> GetActiveValuesInHouse(const CandidatesGrid &grid, const House &house)
{
   std::vector<Value> result(ValueAll.begin(), ValueAll.end());
   for (const Cell &cell : house.cells)
   {
      const auto solvedValue = grid[cell.field].tryGetSolvedValue();
      if (solvedValue)
      {
         result[solvedValue.value() - 1] = ValueUndefined;
      }
   }
   result.erase(std::remove(result.begin(), result.end(), ValueUndefined), result.end());
   return result;
}

bool CellContainsHiddenTuple(const CandidatesGrid &grid, Field field, std::span<const Value> values)
{
   return grid[field].contains(values);
}

bool IsHiddenTuple(const CandidatesGrid &grid, const House &house, std::span<const Value> candidateValueTuple)
{
   if (candidateValueTuple.size() == 2)
   {
      // std::cout << candidateValueTuple[0] << " " << candidateValueTuple[1] << "\n";
      if (candidateValueTuple[0] == 2 && candidateValueTuple[1] == 6 &&
          str::all_of(house.cells, [](Cell cell) { return cell.box == 3; }))
      {
         // std::cout << candidateValueTuple[0] << " " << candidateValueTuple[1] << "\n";
      }
   }

   for (Value value : candidateValueTuple)
   {
      auto tmp = grid.countOccurence(house.cells, value);
      if (tmp > 100)
         return false;
      Index count = 0;
      for (Cell cell : house.cells)
      {
         // std::cout << Candidates::ToStrings(grid[cell.field]);
         if (grid[cell.field].contains(value))
            count += 1;
      }
      MyAssert(count == tmp);
   }
   const auto size = candidateValueTuple.size();
   for (Value value : candidateValueTuple)
   {
      if (grid.countOccurence(house.cells, value) != size)
      {
         return false;
      }
   }
   const Index count = str::count_if(house.cells, [&grid, candidateValueTuple](Cell cell) {
      return CellContainsHiddenTuple(grid, cell.field, candidateValueTuple);
   });
   return (count == size);
}

bool RemoveCandidates(CandidatesGrid &grid, const House &house, std::span<Value> candidateTuple)
{
   bool anyChange = false;
   for (const Cell &cell : house.cells)
   {
      if (CellContainsHiddenTuple(grid, cell.field, candidateTuple))
      {
         for (Value value : ValueAll)
         {
            if (grid[cell.field].contains(value) && str::find(candidateTuple, value) == candidateTuple.end())
            {
               anyChange |= grid.removeCandidate(cell.field, value);
            }
         }
      }
   }
   return anyChange;
}

bool sweepHouse(CandidatesGrid &grid, const House &house, std::span<const Value> activeCandidates, Index size)
{
   bool anyChange = false;
   if (activeCandidates.size() < size)
   {
      return anyChange;
   }

   std::vector<Value> candidateTuple(size);
   auto generator = make_combination_generator(activeCandidates.begin(), activeCandidates.end(), candidateTuple.begin(),

                                               candidateTuple.end());
   do
   {
      if (IsHiddenTuple(grid, house, candidateTuple))
      {
         anyChange |= RemoveCandidates(grid, house, candidateTuple);
      }
   } while (generator());

   return anyChange;
}

} // namespace

bool SweepHiddenTuple::sweep(CandidatesGrid &grid, Index size)
{
   bool anyChange = false;
   for (const House &house : House::getAllHouses())
   {
      const auto activeValues = GetActiveValuesInHouse(grid, house);
      anyChange |= sweepHouse(grid, house, activeValues, size);
   }
   return anyChange;
}

bool SweepHiddenTuple::sweep(CandidatesGrid &grid)
{
   bool anyChange = false;
   anyChange |= sweep(grid, 2);
   return anyChange;
}
