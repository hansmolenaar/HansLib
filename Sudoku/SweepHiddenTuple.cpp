#include "SweepHiddenTuple.h"
#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "Contains.h"
#include "FieldsInfo.h"
#include "House.h"
#include "StreamUtils.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "boost/container/static_vector.hpp"
#include <algorithm>
#include <iterator>
#include <ranges>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{
struct CellContainsAllValues
{
   bool operator()(Field field) const
   {
      return grid[field].contains(values);
   }
   bool operator()(const Cell &cell) const
   {
      return grid[cell.field].contains(values);
   }

   const CandidatesGrid &grid;
   std::span<const Value> values;
};

bool IsHiddenTuple(const FieldsInfo &finfo, const HouseId &house, std::span<const Value> candidateValueTuple)
{
   const auto size = candidateValueTuple.size();
   auto houseFields = finfo.getHouseFields(house);
   for (Value value : candidateValueTuple)
   {
      if ((houseFields & finfo.getFieldsWithValue(value)).count() != size)
      {
         return false;
      }
   }
   const CellContainsAllValues containsHiddenTuple{finfo.getGrid(), candidateValueTuple};
   const Index count = str::count_if(FieldAll | std::views::filter(Bitset{houseFields}), containsHiddenTuple);
   return (count == size);
}

bool RemoveCandidates(CandidatesGrid &grid, FieldsInfo &finfo, const HouseId &house,
                      std::span<const Value> candidateTuple)
{
   // Remove all values except the candidate tuple
   boost::container::static_vector<Value, NumValue> activeValues;
   str::copy_if(ValueAll, std::back_inserter(activeValues), IsNotContained(candidateTuple));

   auto houseFields = finfo.getHouseFields(house);
   const CellContainsAllValues cellPredicate{grid, candidateTuple};
   auto activeFields = FieldAll | std::views::filter(Bitset{houseFields}) | std::views::filter(cellPredicate);

   const bool anyChange = grid.RemoveCandidatesFromFields(activeValues, activeFields);
   if (anyChange)
   {
      finfo.update();
   }
   return anyChange;
}
} // namespace

bool SweepHiddenTuple::sweepHouse(CandidatesGrid &grid, const House &house, Index size)
{
   FieldsInfo finfo(grid);
   bool anyChange = false;
   const auto activeUniqueCandidates =
       SudokuUtils::getUniqueActiveValuesInCells(grid, finfo.getHouseFields(house.houseId));
   if (activeUniqueCandidates.count() < size)
   {
      return anyChange;
   }

   std::vector<Value> activeCandidates(activeUniqueCandidates.count());
   str::copy_if(ValueAll, activeCandidates.begin(), Bitset{activeUniqueCandidates});

   Combinations<Value> combinations(activeCandidates, size);
   do
   {
      if (IsHiddenTuple(finfo, house.houseId, combinations()))
      {
         anyChange |= RemoveCandidates(grid, finfo, house.houseId, combinations());
      }
   } while (combinations.next());

   return anyChange;
}

bool SweepHiddenTuple::sweep(CandidatesGrid &grid, Index size)
{
   bool anyChange = false;
   for (const House &house : House::getAllHouses())
   {
      if (sweepHouse(grid, house, size))
      {
         anyChange = true;
         grid.update();
      }
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
