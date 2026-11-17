#include "SweepNakedTuples.h"
#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "FieldsInfo.h"
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

void collect(const FieldsInfo &finfo, const FieldBits &house, std::span<const Field> candidateTuple,
             FieldValue::Container &fieldValues)
{
   const CandidatesGrid &grid = finfo.getGrid();
   const auto candidatesToRemove = CandidatesGrid::getValuesInFields(grid, candidateTuple);
   boost::container::static_vector<Value, NumValue> valuesToRemove(candidatesToRemove.count());
   str::copy_if(ValueAll, valuesToRemove.begin(), Bitset{candidatesToRemove});

   const auto fields = house & ~SudokuUtils::getFields(candidateTuple);
   for (Value value : valuesToRemove)
   {
      FieldValue::add(fields, value, fieldValues);
   }
}

bool RemoveCandidates(CandidatesGrid &grid, const FieldBits &house, std::span<const Field> candidateTuple)
{
   FieldValue::Container fieldValues;
   FieldsInfo finfo(grid);
   collect(finfo, house, candidateTuple, fieldValues);
   return grid.apply(grid, fieldValues).numValuesChanged > 0;
}

bool IsNakedTuple(const CandidatesGrid &grid, const std::vector<Field> &candidateTuple)
{
   const auto values = CandidatesGrid::getValuesInFields(grid, candidateTuple);
   return values.count() == candidateTuple.size();
}

} // namespace

bool SweepNakedTuples::sweepHouse(CandidatesGrid &grid, FieldsInfo &finfo, HouseId houseId, Index tupleSize)

{
   bool anyChange = false;

   const FieldBits house = finfo.getHouseFields(houseId);
   const FieldBits fields = house & finfo.getActiveFields();
   std::vector<Field> activeFields;
   for (Field field : FieldAll | std::views::filter(Bitset{fields}))
   {
      if (grid[field].size() <= tupleSize)
      {
         activeFields.push_back(field);
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

   if (anyChange)
   {
      finfo.update();
   }
   return anyChange;
}

bool SweepNakedTuples::sweepHousesOfType(CandidatesGrid &grid, FieldsInfo &finfo, HouseType type, Index tupleSize)
{
   bool anyChange = false;
   HouseId houseId{type, IndexUndefined};
   for (Index idx : IndexAll)
   {
      houseId.houseIndex = idx;
      if (sweepHouse(grid, finfo, houseId, tupleSize))
      {
         anyChange = true;
         grid.update();
      }
   }
   return anyChange;
}

bool SweepNakedTuples::sweep(CandidatesGrid &grid, Index tupleSize)
{
   FieldsInfo finfo(grid);
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= sweepHousesOfType(grid, finfo, type, tupleSize);
   }
   return anyChange;
}

// ***************************************************************

bool SweepNakedPairs::sweep(CandidatesGrid &grid)
{
   FieldsInfo finfo(grid);
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= SweepNakedTuples::sweepHousesOfType(grid, finfo, type, 2);
   }
   return anyChange;
}

// ***************************************************************

bool SweepNakedTriplet::sweep(CandidatesGrid &grid)
{
   FieldsInfo finfo(grid);
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= SweepNakedTuples::sweepHousesOfType(grid, finfo, type, 3);
   }
   return anyChange;
}

// ***************************************************************

bool SweepNakedQuartet::sweep(CandidatesGrid &grid)
{
   FieldsInfo finfo(grid);
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= SweepNakedTuples::sweepHousesOfType(grid, finfo, type, 4);
   }
   return anyChange;
}
