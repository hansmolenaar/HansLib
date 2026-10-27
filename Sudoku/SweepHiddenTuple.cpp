#include "SweepHiddenTuple.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "Contains.h"
#include "House.h"
#include "StreamUtils.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "boost/container/static_vector.hpp"
#include <algorithm>
#include <iterator>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{
struct CellContainsAllValues
{
   bool operator()(const Cell &cell) const
   {
      return grid[cell.field].contains(values);
   }

   const CandidatesGrid &grid;
   std::span<const Value> values;
};

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
   const CellContainsAllValues containsHiddenTuple{grid, candidateValueTuple};
   const Index count = str::count_if(house.cells, containsHiddenTuple);
   return (count == size);
}

bool RemoveCandidates(CandidatesGrid &grid, const House &house, std::span<const Value> candidateTuple)
{
   // Remove all values except the candidate tuple
   boost::container::static_vector<Value, NumValue> activeValues;
   str::copy_if(ValueAll, std::back_inserter(activeValues),
                [&candidateTuple](Value value) { return !Contains(candidateTuple, value); });

   return grid.RemoveCandidates(activeValues, house.cells, CellContainsAllValues{grid, candidateTuple});
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
