#include "SweepHiddenTuple.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "Contains.h"
#include "House.h"
#include "StreamUtils.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include <algorithm>

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

bool RemoveCandidates(CandidatesGrid &grid, const House &house, std::span<const Value> candidateTuple)
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
   const auto activeUniqueCandidates = SudokuUtils::getUniqueActiveValuesInCells(grid, house.cells);
   if (activeUniqueCandidates.size() < size)
   {
      return anyChange;
   }
   const std::vector<Value> activeCandidates(activeUniqueCandidates.begin(), activeUniqueCandidates.end());

   Combinations<Value> combinations(activeCandidates, size);
   do
   {
      if (IsHiddenTuple(grid, house, combinations()))
      {
         anyChange |= RemoveCandidates(grid, house, combinations());
      }
   } while (combinations.next());

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

// *******************************************************************************************

bool SweepHiddenSingle::sweep(CandidatesGrid &grid)
{
   return SweepHiddenTuple::sweep(grid, 1);
};

// *******************************************************************************************

bool SweepHiddenPair::sweep(CandidatesGrid &grid)
{
   return SweepHiddenTuple::sweep(grid, 2);
};

// *******************************************************************************************

bool SweepHiddenTriplet::sweep(CandidatesGrid &grid)
{
   return SweepHiddenTuple::sweep(grid, 3);
};

// *******************************************************************************************

bool SweepHiddenQuartet::sweep(CandidatesGrid &grid)
{
   return SweepHiddenTuple::sweep(grid, 4);
};
