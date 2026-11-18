#include "SweepIntersectionElimination.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "MyAssert.h"
#include "SudokuUtils.h"

#include <optional>

using namespace Sudoku;
using namespace Utilities;

std::optional<HouseId> SweepIntersectionElimination::TryGetIntersectionHouse(HouseType houseTypeIn,
                                                                             const FieldBits &fields,
                                                                             const FieldsInfo &finfo)
{
   const auto size = fields.count();
   MyAssert(size >= 2 && size <= 3);
   const Cell cell1 = Cell::get(SudokuUtils::FirstField(fields));
   const HouseId houseIn = HouseId::getFromCellaAndType(cell1, houseTypeIn);
   MyAssert((fields & finfo.getHouseFields(houseIn)).count() == size);

   HouseId houseOut;
   if (houseTypeIn == HouseType::Box)
   {
      // Row or Col
      houseOut = HouseId::getFromCellaAndType(cell1, HouseType::Row);
      if ((fields & finfo.getHouseFields(houseOut)).count() == size)
      {
         return houseOut;
      }
      houseOut = HouseId::getFromCellaAndType(cell1, HouseType::Col);
      if ((fields & finfo.getHouseFields(houseOut)).count() == size)
      {
         return houseOut;
      }
   }
   else
   {
      // Only box possible
      houseOut = HouseId::getFromCellaAndType(cell1, HouseType::Box);
      if ((fields & finfo.getHouseFields(houseOut)).count() == size)
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
         const auto intersectionHouseId = TryGetIntersectionHouse(houseId.houseType, fieldsWithValueInHouse, finfo);
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
