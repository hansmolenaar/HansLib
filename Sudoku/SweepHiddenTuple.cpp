#include "SweepHiddenTuple.h"
#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "Contains.h"
#include "FieldsInfo.h"
#include "House.h"
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
struct CellContainsAnyValue
{
   bool operator()(Field field) const
   {
      return (grid[field].getValues() & values).any();
   }

   const CandidatesGrid &grid;
   ValueBits values;
};

bool IsHiddenTuple(const FieldsInfo &finfo, const House &house, std::span<const Value> candidateValueTuple)
{
   const auto size = candidateValueTuple.size();
   ValueBits valueBits;
   for (Value v : candidateValueTuple)
   {
      valueBits.set(v);
   }
   const auto activeHouseFields = finfo.getHouseFields(house) & finfo.getActiveFields();
   const CellContainsAnyValue containsHiddenTuple{finfo.getGrid(), valueBits};
   const Index count = str::count_if(FieldAll | std::views::filter(Bitset{activeHouseFields}), containsHiddenTuple);
   return (count == size);
}

void CollectCandidates(const FieldsInfo &finfo, const House &house, std::span<const Value> candidateTuple,
                       FieldValue::Container &fieldValues)
{
   // Remove all values  except the candidate tuple
   boost::container::static_vector<Value, NumValue> activeValues;
   str::copy_if(ValueAll, std::back_inserter(activeValues), IsNotContained(candidateTuple));

   auto activeFields = finfo.getHouseFields(house);
   for (Value value : candidateTuple)
   {
      activeFields &= finfo.getActiveFieldsWithValue(value);
   }

   // activeFields now contain all of the values in the value tuplie
   for (Value value : activeValues)
   {
      const auto removeValueFromFields = activeFields & finfo.getFieldsWithValue(value);
      FieldValue::add(removeValueFromFields, value, fieldValues);
   }
}

} // namespace

FieldValue::Container SweepHiddenTuple::collectHouse(const FieldsInfo &finfo, const House &house, Index size)
{
   FieldValue::Container result;

   const auto houseFields = finfo.getHouseFields(house);
   std::vector<Value> activeCandidates;
   for (Value value : ValueAll)
   {
      const auto fieldsInHouseWithValue = finfo.getActiveFieldsWithValue(value) & houseFields;
      const auto numFieldsInHouseWithValue = fieldsInHouseWithValue.count();
      if (numFieldsInHouseWithValue > 0 && numFieldsInHouseWithValue <= size)
      {
         activeCandidates.push_back(value);
      }
   }

   if (activeCandidates.size() < size)
   {
      return {};
   }

   Combinations<Value> combinations(activeCandidates, size);
   do
   {
      if (IsHiddenTuple(finfo, house, combinations()))
      {
         CollectCandidates(finfo, house, combinations(), result);
      }
   } while (combinations.next());

   return result;
}

FieldValue::Container SweepHiddenTuple::collectTuplesOfSize(const FieldsInfo &finfo, Index size)
{
   FieldValue::Container result;
   for (House house : House::getAllHouses())
   {
      FieldValue::add(collectHouse(finfo, house, size), result);
   }
   return result;
}

FieldValue::Container SweepHiddenTuple::collect(const FieldsInfo &finfo)
{
   FieldValue::Container result;
   FieldValue::add(collectTuplesOfSize(finfo, 2), result);
   FieldValue::add(collectTuplesOfSize(finfo, 3), result);
   FieldValue::add(collectTuplesOfSize(finfo, 4), result);
   return result;
};

// *******************************************************************************************

FieldValue::Container SweepHiddenSingle::collect(const FieldsInfo &finfo)
{
   return SweepHiddenTuple::collectTuplesOfSize(finfo, 1);
};

// *******************************************************************************************

FieldValue::Container SweepHiddenPair::collect(const FieldsInfo &finfo)
{
   return SweepHiddenTuple::collectTuplesOfSize(finfo, 2);
};

// *******************************************************************************************

FieldValue::Container SweepHiddenTriplet::collect(const FieldsInfo &finfo)
{
   return SweepHiddenTuple::collectTuplesOfSize(finfo, 3);
};

// *******************************************************************************************

FieldValue::Container SweepHiddenQuartet::collect(const FieldsInfo &finfo)
{
   return SweepHiddenTuple::collectTuplesOfSize(finfo, 4);
};
