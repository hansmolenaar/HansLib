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

void add(const FieldsInfo &finfo, const FieldBits &house, std::span<const Field> candidateTuple,
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

bool IsNakedTuple(const CandidatesGrid &grid, const std::vector<Field> &candidateTuple)
{
   const auto values = CandidatesGrid::getValuesInFields(grid, candidateTuple);
   return values.count() == candidateTuple.size();
}

} // namespace

FieldValue::Container SweepNakedTuples::collectHouse(const FieldsInfo &finfo, HouseId houseId, Index tupleSize)
{
   FieldValue::Container result;
   const CandidatesGrid &grid = finfo.getGrid();

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
      return result;
   }

   Combinations<Field> combinations(activeFields, tupleSize);
   do
   {
      if (IsNakedTuple(grid, combinations()))
      {
         add(finfo, house, combinations(), result);
      }
   } while (combinations.next());

   return result;
}

FieldValue::Container SweepNakedTuples::collectHousesOfType(const FieldsInfo &finfo, HouseType type, Index tupleSize)
{
   FieldValue::Container result;
   HouseId houseId{type, IndexUndefined};
   for (Index idx : IndexAll)
   {
      houseId.houseIndex = idx;
      const auto fieldValuesHouse = collectHouse(finfo, houseId, tupleSize);
      result.insert(fieldValuesHouse.begin(), fieldValuesHouse.end());
   }
   return result;
   ;
}

FieldValue::Container SweepNakedTuples::collect(const FieldsInfo &finfo, Index tupleSize)
{
   FieldValue::Container result;
   for (HouseType type : HouseTypeAll)
   {
      const auto fieldValuesHouseType = collectHousesOfType(finfo, type, tupleSize);
      result.insert(fieldValuesHouseType.begin(), fieldValuesHouseType.end());
   }
   return result;
}

// ***************************************************************

bool SweepNakedPairs::sweep(CandidatesGrid &grid)
{
   const FieldsInfo finfo(grid);
   const auto fieldValues = SweepNakedTuples::collect(finfo, 2);
   const auto changes = grid.apply(fieldValues);
   return changes.numValuesChanged > 0;
}

// ***************************************************************

bool SweepNakedTriplet::sweep(CandidatesGrid &grid)
{
   const FieldsInfo finfo(grid);
   const auto fieldValues = SweepNakedTuples::collect(finfo, 3);
   const auto changes = grid.apply(fieldValues);
   return changes.numValuesChanged > 0;
}

// ***************************************************************

bool SweepNakedQuartet::sweep(CandidatesGrid &grid)
{
   const FieldsInfo finfo(grid);
   const auto fieldValues = SweepNakedTuples::collect(finfo, 4);
   const auto changes = grid.apply(fieldValues);
   return changes.numValuesChanged > 0;
}
