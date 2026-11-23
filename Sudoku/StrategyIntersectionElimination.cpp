#include "StrategyIntersectionElimination.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "MyAssert.h"
#include "SudokuUtils.h"

#include <optional>

using namespace Sudoku;
using namespace Utilities;

std::optional<House> SweepIntersectionElimination::TryGetIntersectionHouse(HouseType houseTypeIn,
                                                                           const FieldBits &fields,
                                                                           const FieldsInfo &finfo)
{
   const auto size = fields.count();
   MyAssert(size >= 2 && size <= 3);
   const Cell cell1 = Cell::get(SudokuUtils::FirstField(fields));
   const House houseIn = House::getFromCellaAndType(cell1, houseTypeIn);
   MyAssert((fields & finfo.getHouseFields(houseIn)).count() == size);

   House houseOut;
   if (houseTypeIn == HouseType::Box)
   {
      // Row or Col
      houseOut = House::getFromCellaAndType(cell1, HouseType::Row);
      if ((fields & finfo.getHouseFields(houseOut)).count() == size)
      {
         return houseOut;
      }
      houseOut = House::getFromCellaAndType(cell1, HouseType::Col);
      if ((fields & finfo.getHouseFields(houseOut)).count() == size)
      {
         return houseOut;
      }
   }
   else
   {
      // Only box possible
      houseOut = House::getFromCellaAndType(cell1, HouseType::Box);
      if ((fields & finfo.getHouseFields(houseOut)).count() == size)
      {
         return houseOut;
      }
   }

   return {};
}

FieldValue::Container SweepIntersectionElimination::collectHouse(const FieldsInfo &finfo, const House &house)
{
   FieldValue::Container result;
   const auto houseFields = finfo.getHouseFields(house);
   for (Value value : ValueAll)
   {
      const auto activeFieldsWithValue = finfo.getActiveFieldsWithValue(value);
      const auto fieldsWithValueInHouse = activeFieldsWithValue & houseFields;
      const auto numFieldsWithValue = fieldsWithValueInHouse.count();
      if (numFieldsWithValue == 2 || numFieldsWithValue == 3)
      {
         const std::optional<House> intersectionHouse =
             TryGetIntersectionHouse(house.houseType, fieldsWithValueInHouse, finfo);
         if (intersectionHouse.has_value())
         {
            const auto intersectionHouseFields = finfo.getHouseFields(intersectionHouse.value());
            const auto fieldsToCancel = intersectionHouseFields & activeFieldsWithValue & ~fieldsWithValueInHouse;
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
      FieldValue::add(collectHouse(finfo, house), result);
   }

   return result;
}
