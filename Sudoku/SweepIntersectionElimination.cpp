#include "SweepIntersectionElimination.h"
#include "CandidatesGrid.h"
#include "Contains.h"
#include "FieldsInfo.h"
#include "MyAssert.h"
#include "SudokuUtils.h"
#include "boost/container/static_vector.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>

namespace str = std::ranges;
using namespace Sudoku;
using namespace Utilities;

bool SweepIntersectionElimination::SweepHouse(CandidatesGrid &grid, const House &house)
{
   bool anyChange = false;
   const auto value2fields = SudokuUtils::getValue2Fields(grid, house.cells);
   for (Value value : ValueAll)
   {
      const auto numFieldsWithValue = value2fields[value].size();
      if (numFieldsWithValue == 2 || numFieldsWithValue == 3)
      {
         boost::container::static_vector<Cell, 3> cells(numFieldsWithValue);
         str::transform(value2fields[value], cells.begin(), Cell::get);
         MyAssert(cells.size() == numFieldsWithValue);
         const auto intersectionHouseId = TryGetIntersectionHouse(house.houseId.houseType, cells);
         if (intersectionHouseId.has_value())
         {
            const auto &intersectionHouse = House::getHouse(intersectionHouseId.value());
            auto activeCells = intersectionHouse.cells | std::views::filter(IsNotContained<Cell>{cells});

// TODO remove me
#if false
            std::cout << "SweepHouse value = " << std::to_string(value) << "     cell ";
            for (const auto c : activeCells)
            {
               std::cout << c << " ";
            }
            std::cout << "\n";
#endif
            anyChange |= grid.RemoveCandidate(value, activeCells);
         }
      }
   }
   return anyChange;
};

std::optional<HouseId> SweepIntersectionElimination::TryGetIntersectionHouse(HouseType houseTypeIn,
                                                                             std::span<const Cell> cells)
{
   const auto size = cells.size();
   MyAssert(size >= 2 && size <= 3);
   const HouseId houseIn = HouseId::getFromCellaAndType(cells.front(), houseTypeIn);
   MyAssert(str::all_of(cells, CellInHouse{houseIn}));

   HouseId houseOut;
   if (houseTypeIn == HouseType::Box)
   {
      // Row or Col
      houseOut = HouseId::getFromCellaAndType(cells.front(), HouseType::Row);
      if (str::all_of(cells, CellInHouse{houseOut}))
      {
         return houseOut;
      }
      houseOut = HouseId::getFromCellaAndType(cells.front(), HouseType::Col);
      if (str::all_of(cells, CellInHouse{houseOut}))
      {
         return houseOut;
      }
   }
   else
   {
      // Only box possible
      houseOut = HouseId::getFromCellaAndType(cells.front(), HouseType::Box);
      if (str::all_of(cells, CellInHouse{houseOut}))
      {
         return houseOut;
      }
   }

   return {};
}

FieldValue::Container SweepIntersectionElimination::collectHouse(const FieldsInfo &finfo, const HouseId &houseId)
{
   FieldValue::Container result;
   const auto house = finfo.getHouseFields(houseId);
   const auto activeFields = finfo.getActiveFields();
   for (Value value : ValueAll)
   {
      const auto activeFieldsWithValue = finfo.getActiveFieldsWithValue(value);
      const auto fieldsWithValueInHouse = activeFieldsWithValue & house;
      const auto numFieldsWithValue = fieldsWithValueInHouse.count();
      if (numFieldsWithValue == 2 || numFieldsWithValue == 3)
      {
         const auto fields = SudokuUtils::FieldBitsToVector(fieldsWithValueInHouse); // TODO clean up
         std::vector<Cell> cells(fields.size());
         std::transform(fields.begin(), fields.end(), cells.begin(), Cell::get);
         MyAssert(cells.size() == numFieldsWithValue);
         const auto intersectionHouseId = TryGetIntersectionHouse(houseId.houseType, cells);
         if (intersectionHouseId.has_value())
         {
            const auto intersectionHouse = finfo.getHouseFields(intersectionHouseId.value()) & activeFields;
            for (Field field : SudokuUtils::FieldBitsToRange(intersectionHouse))
            {
               if (!fieldsWithValueInHouse.test(field) && activeFieldsWithValue.test(field))
               {
                  std::cout << "CcollectHouse   " << Cell::get(field) << " " << std::to_string(value) << "\n";
                  result.insert({field, value});
                  MyAssert(!finfo.getGrid()[field].isSolved(),
                           "collectHouse field is solved: " + std::to_string(field)); // TODO remove me
                  MyAssert(finfo.getGrid()[field].contains(value), "collectHouse field " + std::to_string(field) +
                                                                       " does not contain value " +
                                                                       std::to_string(value)); // TODO remove me
               }
            }
         }
      }
   }
   return result;
}

FieldValue::Container SweepIntersectionElimination::collect(const FieldsInfo &finfo)
{
   FieldValue::Container result;
   for (const auto &house : House::getAllHouses())
   {
      FieldValue::add(collectHouse(finfo, house.houseId), result);
   }

   return result;
}

bool SweepIntersectionElimination::sweep(CandidatesGrid &grid)
{
   bool anyChange = false;
   for (const auto &house : House::getAllHouses())
   {
      if (SweepHouse(grid, house))
      {
         anyChange = true;
         // grid.update();
      }
   }
   return anyChange;
};
