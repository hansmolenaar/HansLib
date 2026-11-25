#include "StrategyIntersectionElimination.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "Logging.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"

#include <optional>

using namespace Sudoku;
using namespace Utilities;

namespace
{
std::string GetName()
{
   return "Intersection elimination";
}
} // namespace

std::optional<House> StrategyIntersectionElimination::TryGetIntersectionHouse(HouseType houseTypeIn,
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

FieldValueCount StrategyIntersectionElimination::collectHouse(const FieldsInfo &finfo, const House &house,
                                                              FieldValues &fieldValues)
{
   FieldValueCount result = 0;
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
            const auto prvSize = fieldValues.size();
            result += fieldValues.add(fieldsToCancel, value);
            const auto changeSize = fieldValues.size() - prvSize;
            if (Logging::getDoLog() && changeSize > 0)
            {
               Logging logMessage;
               logMessage.numChanges = changeSize;
               logMessage.strategyName = GetName();
               logMessage.info << numFieldsWithValue << " cells in house " << house;
               logMessage.info << " with value " << value;
               logMessage.info << " eliminate this value from house " << intersectionHouse.value();
               std::cout << logMessage;
            }
         }
      }
   }
   return result;
}

void StrategyIntersectionElimination::collect(const FieldsInfo &finfo, FieldValues &result)
{
   for (const auto &house : House::getAllHouses())
   {
      collectHouse(finfo, house, result);
   }
}

std::string StrategyIntersectionElimination::getName() const
{
   return GetName();
}
