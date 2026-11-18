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

TEST(HouseTest, ByRow)
{
   TestGetHouse([](Index idx) { return House::getRow(idx); });
   const HouseId &houseId = House::getRow(1).houseId;
   ASSERT_EQ(houseId.houseType, HouseType::Row);
   ASSERT_EQ(houseId.houseIndex, 1);
}

TEST(HouseTest, ByCol)
{
   TestGetHouse([](Index idx) { return House::getCol(idx); });
   const HouseId &houseId = House::getCol(8).houseId;
   ASSERT_EQ(houseId.houseType, HouseType::Col);
   ASSERT_EQ(houseId.houseIndex, 8);
}

TEST(HouseTest, ByBox)
{
   const HouseId &houseId = House::getBox(7).houseId;
   TestGetHouse([](Index idx) { return House::getBox(idx); });
   ASSERT_EQ(houseId.houseType, HouseType::Box);
   ASSERT_EQ(houseId.houseIndex, 7);
}

TEST(HouseTest, GetFromCellAndType)
{
   const Cell cell = Cell::get(42);
   ASSERT_EQ(HouseId::getFromCellaAndType(cell, HouseType::Row), (HouseId{HouseType::Row, 4}));
   ASSERT_EQ(HouseId::getFromCellaAndType(cell, HouseType::Col), (HouseId{HouseType::Col, 6}));
   ASSERT_EQ(HouseId::getFromCellaAndType(cell, HouseType::Box), (HouseId{HouseType::Box, 5}));
}
