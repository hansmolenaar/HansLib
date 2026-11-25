#include "StrategyHiddenTuple.h"
#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "Contains.h"
#include "FieldsInfo.h"
#include "House.h"
#include "Logging.h"
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

std::string GetNameSingle()
{
   return "Hidden single";
}

std::string GetNameTuple()
{
   return "Hidden tuple";
}

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

FieldValueCount CollectCandidates(const FieldsInfo &finfo, const House &house, std::span<const Value> candidateTuple,
                                  FieldValues &fieldValues)
{
   FieldValueCount count = 0;
   // Remove all values  except the candidate tuple
   boost::container::static_vector<Value, NumValue> activeValues;
   str::copy_if(ValueAll, std::back_inserter(activeValues), IsNotContained(candidateTuple));

   const auto houseFields = finfo.getHouseFields(house);
   FieldBits activeFields;
   for (Value value : candidateTuple)
   {
      activeFields |= finfo.getActiveFieldsWithValue(value) & houseFields;
   }
   MyAssert(activeFields.count() == candidateTuple.size());

   const auto prvSize = fieldValues.size();
   for (Value value : activeValues)
   {
      const auto removeValueFromFields = activeFields & finfo.getFieldsWithValue(value);
      count += fieldValues.add(removeValueFromFields, value);
   }
   const auto changeSize = fieldValues.size() - prvSize;
   if (Logging::getDoLog() && changeSize > 0)
   {
      Logging logMessage;
      logMessage.numChanges = changeSize;
      std::stringstream info;
      if (candidateTuple.size() == 1)
      {
         logMessage.strategyName = GetNameSingle();
         info << "single cell";
         info << " with value " << candidateTuple.front();
      }
      else
      {
         logMessage.strategyName = GetNameTuple();
         info << "tuple with " << candidateTuple.size() << " cells";
         info << " and values " << Bitset<ValueBits().size()>::create(candidateTuple);
      }
      info << " in house " << house;
      logMessage.info = info.str();
      logMessage.log();
   }
   return count;
}

FieldValueCount CollectHouse(const FieldsInfo &finfo, const House &house, Index size, FieldValues &fvs)
{
   FieldValueCount count = 0;
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
      return count;
   }

   Combinations<Value> combinations(activeCandidates, size);
   do
   {
      if (IsHiddenTuple(finfo, house, combinations()))
      {
         count += CollectCandidates(finfo, house, combinations(), fvs);
      }
   } while (combinations.next());
   return count;
}

FieldValueCount CollectTuplesOfSize(const FieldsInfo &finfo, Index size, FieldValues &result)
{
   FieldValueCount count = 0;
   for (House house : House::getAllHouses())
   {
      count += CollectHouse(finfo, house, size, result);
   }
   return count;
}

} // namespace

void StrategyHiddenTuple::collect(const FieldsInfo &finfo, FieldValues &result)
{
   CollectTuplesOfSize(finfo, 2, result);
   CollectTuplesOfSize(finfo, 3, result);
   CollectTuplesOfSize(finfo, 4, result);
};

std::string StrategyHiddenTuple::getName() const
{
   return GetNameTuple();
}
// *******************************************************************************************

void StrategyHiddenSingle::collect(const FieldsInfo &finfo, FieldValues &result)
{
   CollectTuplesOfSize(finfo, 1, result);
};

std::string StrategyHiddenSingle::getName() const
{
   return GetNameSingle();
}
