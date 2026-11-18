#include "SweepIntersectionElimination.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "MyAssert.h"
#include "SudokuUtils.h"

#include <algorithm>
#include <optional>

namespace str = std::ranges;
using namespace Sudoku;
using namespace Utilities;

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
   for (Value value : ValueAll)
   {
      const auto activeFieldsWithValue = finfo.getActiveFieldsWithValue(value);
      const auto fieldsWithValueInHouse = activeFieldsWithValue & house;
      const auto numFieldsWithValue = fieldsWithValueInHouse.count();
      if (numFieldsWithValue == 2 || numFieldsWithValue == 3)
      {
         const auto fields = SudokuUtils::FieldBitsToVector(fieldsWithValueInHouse);
         std::vector<Cell> cells(fields.size());
         std::transform(fields.begin(), fields.end(), cells.begin(), Cell::get);
         const auto intersectionHouseId = TryGetIntersectionHouse(houseId.houseType, cells);
         if (intersectionHouseId.has_value())
         {
            const auto intersectionHouse = finfo.getHouseFields(intersectionHouseId.value());
            const auto fieldsToCancel = intersectionHouse & activeFieldsWithValue & ~fieldsWithValueInHouse;
            FieldValue::add(fieldsToCancel, value, result);
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
