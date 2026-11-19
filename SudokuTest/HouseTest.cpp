#include <gtest/gtest.h>

#include "FieldsInfo.h"
#include "House.h"
#include "MyAssert.h"
#include "SudokuUtils.h"

using namespace Sudoku;
using namespace Utilities;

namespace
{
template <typename F> void TestGetHouse(F &&getHouse)
{
   CandidatesGrid grid;
   FieldsInfo finfo(grid);
   std::set<Field> fieldsFound;

   for (auto idx : IndexAll)
   {
      const House &house = getHouse(idx);
      const HouseType houseType = house.houseId.houseType;
      MyAssert(house.houseId.houseIndex == idx);
      const auto houseFields = finfo.getHouseFields(house.houseId);
      for (Field field : SudokuUtils::FieldBitsToRange(houseFields))
      {
         const Cell cell = Cell::get(field);
         MyAssert(cell[houseType] == idx);
         fieldsFound.insert(cell.field);
      }
   }
   MyAssert(fieldsFound.size() == NumField);
   MyAssert(*fieldsFound.rbegin() == 80);
}

} // namespace

TEST(HouseTest, GetFromCellAndType)
{
   const Cell cell = Cell::get(42);
   ASSERT_EQ(HouseId::getFromCellaAndType(cell, HouseType::Row), (HouseId{HouseType::Row, 4}));
   ASSERT_EQ(HouseId::getFromCellaAndType(cell, HouseType::Col), (HouseId{HouseType::Col, 6}));
   ASSERT_EQ(HouseId::getFromCellaAndType(cell, HouseType::Box), (HouseId{HouseType::Box, 5}));
}
