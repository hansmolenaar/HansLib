#include "StrategyNakedTuples.h"
#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "FieldsInfo.h"
#include "House.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include <algorithm>

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
      const auto fieldsWithValue = fields & finfo.getActiveFieldsWithValue(value);
      FieldValue::add(fieldsWithValue, value, fieldValues);
   }
}

bool IsNakedTuple(const CandidatesGrid &grid, const std::vector<Field> &candidateTuple)
{
   const auto values = CandidatesGrid::getValuesInFields(grid, candidateTuple);
   return values.count() == candidateTuple.size();
}

} // namespace

FieldValue::Container SweepNakedTuples::collectHouse(const FieldsInfo &finfo, const House &house, Index tupleSize)
{
   FieldValue::Container result;
   const CandidatesGrid &grid = finfo.getGrid();

   const FieldBits houseFields = finfo.getHouseFields(house);
   const FieldBits fields = houseFields & finfo.getActiveFields();
   std::vector<Field> activeFields;
   for (Field field : SudokuUtils::FieldBitsToRange(fields))
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
         add(finfo, houseFields, combinations(), result);
      }
   } while (combinations.next());

   return result;
}

FieldValue::Container SweepNakedTuples::collectHousesOfType(const FieldsInfo &finfo, HouseType type, Index tupleSize)
{
   FieldValue::Container result;
   for (Index idx : IndexAll)
   {
      const House house{type, idx};
      const auto fieldValuesHouse = collectHouse(finfo, house, tupleSize);
      result.insert(fieldValuesHouse.begin(), fieldValuesHouse.end());
   }
   return result;
   ;
}

FieldValue::Container SweepNakedTuples::collectOfTupleSize(const FieldsInfo &finfo, Index tupleSize)
{
   FieldValue::Container result;
   for (HouseType type : HouseTypeAll)
   {
      const auto fieldValuesHouseType = collectHousesOfType(finfo, type, tupleSize);
      result.insert(fieldValuesHouseType.begin(), fieldValuesHouseType.end());
   }
   return result;
}

FieldValue::Container SweepNakedTuples::collect(const FieldsInfo &finfo)
{
   FieldValue::Container result;
   FieldValue::add(collectOfTupleSize(finfo, 2), result);
   FieldValue::add(collectOfTupleSize(finfo, 3), result);
   FieldValue::add(collectOfTupleSize(finfo, 4), result);
   return result;
}
