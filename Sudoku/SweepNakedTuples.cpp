#include "SweepNakedTuples.h"
#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "Contains.h"
#include "House.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include <algorithm>
#include <ranges>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{

bool RemoveCandidates(CandidatesGrid &grid, const House &house, std::span<const Field> candidateTuple)
{
   const auto candidatesToRemove = CandidatesGrid::getValuesInFields(grid, candidateTuple);
   boost::container::static_vector<Value, NumValue> valuesToRemove(candidatesToRemove.count());
   str::copy_if(ValueAll, valuesToRemove.begin(), Bitset{candidatesToRemove});

   auto cellNotInCandidateTuple = [&candidateTuple](const Cell &cell) { return !Contains(candidateTuple, cell.field); };
   auto cells = house.cells | std::views::filter(cellNotInCandidateTuple);
   return grid.RemoveCandidates(valuesToRemove, cells);
}

bool IsNakedTuple(const CandidatesGrid &grid, const std::vector<Field> &candidateTuple)
{
   const auto values = CandidatesGrid::getValuesInFields(grid, candidateTuple);
   return values.count() == candidateTuple.size();
}

} // namespace

bool SweepNakedTuples::sweepHouse(CandidatesGrid &grid, const House &house, Index tupleSize)

{
   bool anyChange = false;

   std::vector<Field> activeFields;
   for (Cell cell : house.cells)
   {
      if (!grid[cell.field].isSolved() && grid[cell.field].size() <= tupleSize)
      {
         activeFields.push_back(cell.field);
      }
   }

   if (activeFields.size() < tupleSize)
   {
      return anyChange;
   }

   Combinations<Field> combinations(activeFields, tupleSize);
   do
   {
      if (IsNakedTuple(grid, combinations()))
      {
         anyChange |= RemoveCandidates(grid, house, combinations());
      }
   } while (combinations.next());

   return anyChange;
}

bool SweepNakedTuples::sweepHousesOfType(CandidatesGrid &grid, HouseType type, Index tupleSize)
{
   bool anyChange = false;
   HouseId houseId{type, IndexUndefined};
   for (Index idx : IndexAll)
   {
      houseId.houseIndex = idx;
      anyChange |= sweepHouse(grid, House::getHouse(houseId), tupleSize);
   }
   return anyChange;
}

bool SweepNakedTuples::sweep(CandidatesGrid &cg, Index tupleSize)
{
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= sweepHousesOfType(cg, type, tupleSize);
   };
   return anyChange;
}

// ***************************************************************

bool SweepNakedPairs::sweep(CandidatesGrid &cg)
{
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= SweepNakedTuples::sweepHousesOfType(cg, type, 2);
   };
   return anyChange;
}

// ***************************************************************

bool SweepNakedTriplet::sweep(CandidatesGrid &cg)
{
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= SweepNakedTuples::sweepHousesOfType(cg, type, 3);
   };
   return anyChange;
}

// ***************************************************************

bool SweepNakedQuartet::sweep(CandidatesGrid &cg)
{
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= SweepNakedTuples::sweepHousesOfType(cg, type, 4);
   };
   return anyChange;
}
