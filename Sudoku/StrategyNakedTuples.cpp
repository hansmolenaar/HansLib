#include "StrategyNakedTuples.h"
#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "FieldsInfo.h"
#include "Functors.h"
#include "House.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "Visibility.h"

#include "boost/container/static_vector.hpp"

#include <algorithm>
#include <iostream>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{

void add(const FieldsInfo &finfo, const FieldBits &house, std::span<const Field> candidateTuple,
         FieldValue::Container &fieldValues)
{
   bool isLogged = true;
   const CandidatesGrid &grid = finfo.getGrid();
   const auto candidatesToRemove = CandidatesGrid::getValuesInFields(grid, candidateTuple);
   boost::container::static_vector<Value, NumValue> valuesToRemove(candidatesToRemove.count());
   str::copy_if(ValueAll, valuesToRemove.begin(), Bitset{candidatesToRemove});

   const auto fields = house & ~SudokuUtils::getFields(candidateTuple);
   for (Value value : valuesToRemove)
   {
      const auto fieldsWithValue = fields & finfo.getActiveFieldsWithValue(value);
      const bool anyChange = FieldValue::add(fieldsWithValue, value, fieldValues);
      if (anyChange && !isLogged)
      {
         isLogged = true;
         std::cout << "Naked tuple of size " << candidateTuple.size() << " with values (";
         for (Value value : SudokuUtils::ValueBitsToRange(candidatesToRemove))
         {
            std::cout << " " << value;
         }
         std::cout << " ) and cells ";
         for (Field field : candidateTuple)
         {
            std::cout << " " << Cell::get(field);
         }
         std::cout << " )\n";
      }
   }
}

struct BendedNakedTripleResult
{
   House line;
   ValueBits values;
};

void addBendedTriplets(const FieldsInfo &finfo,
                       const boost::container::static_vector<BendedNakedTripleResult, 3> &xtraLines,
                       std::span<const Field> candidateTuple, FieldValue::Container &fieldValues)
{
   bool isLogged = true;
   for (const auto &bnt : xtraLines)
   {

      const auto fieldsInHouse = finfo.getHouseFields(bnt.line) & ~SudokuUtils::getFields(candidateTuple);
      for (Value value : SudokuUtils::ValueBitsToRange(bnt.values))
      {
         const auto fields = fieldsInHouse & finfo.getActiveFieldsWithValue(value);
         const bool anyChange = FieldValue::add(fields, value, fieldValues);

         if (anyChange && !isLogged)
         {
            isLogged = true;
            std::cout << "Bended triplet of the cells  ";
            for (Field field : candidateTuple)
            {
               std::cout << " " << Cell::get(field);
            }
            std::cout << " ) with the line " << bnt.line << " with values (";
            for (Value value : SudokuUtils::ValueBitsToRange(bnt.values))
            {
               std::cout << " " << value;
            }
            std::cout << ")\n";
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

} // namespace

void StrategyNakedTuples::collectHouse(const FieldsInfo &finfo, const House &house, Index tupleSize,
                                       FieldValue::Container &result)
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
         add(finfo, houseFields, combinations(), result);
         if (tupleSize == 3)
         {
            const auto xtra = IsBendedNakedTriplet(grid, combinations());
            addBendedTriplets(finfo, xtra, combinations(), result);
         }
      }
   } while (combinations.next());
}

void StrategyNakedTuples::collectHousesOfType(const FieldsInfo &finfo, HouseType type, Index tupleSize,
                                              FieldValue::Container &result)
{
   for (Index idx : IndexAll)
   {
      const House house{type, idx};
      collectHouse(finfo, house, tupleSize, result);
   }
}

void StrategyNakedTuples::collectOfTupleSize(const FieldsInfo &finfo, Index tupleSize, FieldValue::Container &result)
{
   for (HouseType type : HouseTypeAll)
   {
      collectHousesOfType(finfo, type, tupleSize, result);
   }
}

bool StrategyNakedTuples::collect(const FieldsInfo &finfo, FieldValue::Container &result)
{
   const auto size = result.size();
   collectOfTupleSize(finfo, 2, result);
   collectOfTupleSize(finfo, 3, result);
   collectOfTupleSize(finfo, 4, result);
   return size != result.size();
}
