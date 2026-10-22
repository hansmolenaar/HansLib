#include "SweepHiddenTuple.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "Contains.h"
#include "House.h"
#include "StreamUtils.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include <algorithm>
#include <iostream>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{
bool CellContainsHiddenTuple(const CandidatesGrid &grid, Field field, std::span<const Value> values)
{
   return grid[field].contains(values);
}

bool IsHiddenTuple(const CandidatesGrid &grid, const House &house, std::span<const Value> candidateValueTuple)
{
   const auto size = candidateValueTuple.size();
   for (Value value : candidateValueTuple)
   {
      if (SudokuUtils::countOccurence(grid, house.cells, value) != size)
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
            if (grid[cell.field].contains(value) && !Contains(candidateTuple, value))
            {
               // std::cout << "Remove candidate " << value << " from cell " << cell.row << " " << cell.col
               //         << " in house of type " << static_cast<int>(house.type) << " with hidden single "
               //       << candidateTuple[0] << "\n";
               anyChange |= grid.removeCandidate(cell.field, value);
            }
         }
      }
   }
   return anyChange;
}
} // namespace

bool SweepHiddenTuple::sweepHouse(CandidatesGrid &grid, const House &house, Index size)
{
   bool anyChange = false;
   const auto activeCandidates = SudokuUtils::getUniqueActiveValuesInCells(grid, house.cells);
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

bool SweepHiddenTuple::sweep(CandidatesGrid &grid, Index size)
{
   bool anyChange = false;
   for (const House &house : House::getAllHouses())
   {
      anyChange |= sweepHouse(grid, house, size);
   }
   return anyChange;
}

bool SweepHiddenTuple::sweep(CandidatesGrid &grid)
{
   bool anyChange = false;
   anyChange |= sweep(grid, 2);
   return anyChange;
}
