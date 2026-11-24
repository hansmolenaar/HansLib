#include "StrategyNakedTuples.h"
#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "FieldsInfo.h"
#include "Functors.h"
#include "House.h"
#include "Logging.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "Visibility.h"

#include "boost/container/static_vector.hpp"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <sstream>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{

void add(const FieldsInfo &finfo, const House &house, std::span<const Field> candidateTuple, FieldValues &fieldValues)
{
   bool doLog = false;
   const CandidatesGrid &grid = finfo.getGrid();
   const auto candidatesToRemove = CandidatesGrid::getValuesInFields(grid, candidateTuple);
   boost::container::static_vector<Value, NumValue> valuesToRemove(candidatesToRemove.count());
   str::copy_if(ValueAll, valuesToRemove.begin(), Bitset{candidatesToRemove});

   const auto houseFields = finfo.getHouseFields(house);
   const auto fields = houseFields & ~SudokuUtils::getFields(candidateTuple);
   const auto prvSize = fieldValues.size();
   for (Value value : valuesToRemove)
   {
      const auto fieldsWithValue = fields & finfo.getActiveFieldsWithValue(value);
      fieldValues.add(fieldsWithValue, value);
   }
   if (doLog && prvSize != fieldValues.size())
   {
      Logging logMessage;
      logMessage.strategyName = "StrategyNakedTuples";
      logMessage.numChanges = fieldValues.size() - prvSize;
      std::stringstream info;
      info << "tuple with " << candidateTuple.size() << " cells";
      info << " and values " << candidatesToRemove;
      info << " in house " << house;
      logMessage.info = info.str();
      std::cout << logMessage;
   }
}

struct BendedNakedTripleResult
{
   House line;
   ValueBits values;
};

void addBendedTriplets(const FieldsInfo &finfo,
                       const boost::container::static_vector<BendedNakedTripleResult, 3> &xtraLines,
                       std::span<const Field> candidateTuple, House inHouse, FieldValues &fieldValues)
{
   bool doLog = false;
   for (const auto &bnt : xtraLines)
   {

      const auto fieldsInHouse = finfo.getHouseFields(bnt.line) & ~SudokuUtils::getFields(candidateTuple);
      for (Value value : SudokuUtils::ValueBitsToRange(bnt.values))
      {
         const auto fields = fieldsInHouse & finfo.getActiveFieldsWithValue(value);
         const auto prvSize = fieldValues.size();
         const bool anyChange = fieldValues.add(fields, value);

         if (anyChange && doLog)
         {
            const auto valuesInTriplet = CandidatesGrid::getValuesInFields(finfo.getGrid(), candidateTuple);
            Logging logMessage;
            logMessage.strategyName = "StrategyNakedTuples";
            logMessage.numChanges = fieldValues.size() - prvSize;
            std::stringstream info;
            info << "bended triplet ";
            info << "with values " << valuesInTriplet;
            info << " in house " << inHouse;
            info << " removes " << value << " from line " << bnt.line;
            logMessage.info = info.str();
            std::cout << logMessage;
         }
      }
   }
}

bool IsNakedTuple(const CandidatesGrid &grid, const std::vector<Field> &candidateTuple)
{
   const auto values = CandidatesGrid::getValuesInFields(grid, candidateTuple);
   return values.count() == candidateTuple.size();
}

boost::container::static_vector<BendedNakedTripleResult, 3> IsBendedNakedTriplet(
    const CandidatesGrid &grid, const std::vector<Field> &candidateTuple)
{
   constexpr Index numFields = 3;
   boost::container::static_vector<Field, 3> pairFields;
   ValueBits values;
   for (size_t n = 0; n < numFields; ++n)
   {
      const Field field = candidateTuple.at(n);
      const auto fieldValues = grid[field].getValues();
      values |= fieldValues;

      const auto numCandidates = fieldValues.count();
      MyAssert(numCandidates > 1 && numCandidates <= 3);
      if (numCandidates == 2)
      {
         pairFields.push_back(field);
      }
   }

   boost::container::static_vector<BendedNakedTripleResult, numFields> result;
   for (Field pairField : pairFields)
   {
      std::array<Field, numFields - 1> other2;
      str::copy_if(candidateTuple, other2.begin(), Functors::IsNotEqualTo{pairField});
      const auto visibility = Visibility::areVisibleVia(Cell::get(other2[0]), Cell::get(other2[1]));
      for (auto houseType : visibility)
      {
         if (houseType != HouseType::Box)
         {
            const ValueBits removeValues = values & ~grid[pairField].getValues();
            const House house = House::getFromCellaAndType(Cell::get(other2[0]), houseType);
            result.push_back(BendedNakedTripleResult{house, removeValues});
         }
      }
   }

   return result;
}

void CollectHouse(const FieldsInfo &finfo, const House &house, Index tupleSize, FieldValues &result)
{
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
      return;
   }

   Combinations<Field> combinations(activeFields, tupleSize);
   do
   {
      if (IsNakedTuple(grid, combinations()))
      {
         add(finfo, house, combinations(), result);
         if (tupleSize == 3)
         {
            const auto xtra = IsBendedNakedTriplet(grid, combinations());
            addBendedTriplets(finfo, xtra, combinations(), house, result);
         }
      }
   } while (combinations.next());
}
} // namespace

void StrategyNakedTuples::collectHousesOfType(const FieldsInfo &finfo, HouseType type, Index tupleSize,
                                              FieldValues &result)
{
   for (Index idx : IndexAll)
   {
      const House house{type, idx};
      CollectHouse(finfo, house, tupleSize, result);
   }
}

void StrategyNakedTuples::collectOfTupleSize(const FieldsInfo &finfo, Index tupleSize, FieldValues &result)
{
   for (HouseType type : HouseTypeAll)
   {
      collectHousesOfType(finfo, type, tupleSize, result);
   }
}

void StrategyNakedTuples::collect(const FieldsInfo &finfo, FieldValues &result)
{
   collectOfTupleSize(finfo, 2, result);
   collectOfTupleSize(finfo, 3, result);
   collectOfTupleSize(finfo, 4, result);
}
