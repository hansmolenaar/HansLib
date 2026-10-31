#include <gtest/gtest.h>

#include "House.h"
#include "MyAssert.h"

using namespace Sudoku;
using namespace Utilities;

namespace
{
template <typename F> void TestGetHouse(F &&getHouse)
{
   std::set<Field> fieldsFound;

   for (auto idx : IndexAll)
   {
      const House &house = getHouse(idx);
      const HouseType houseType = house.houseId.houseType;
      MyAssert(house.houseId.houseIndex == idx);
      for (const Cell &cell : house.cells)
      {
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

TEST(HouseTest, CellInHouse)
{
   const HouseId houseId = HouseId::getFromCellaAndType(Cell::get(42), HouseType::Row);
   const CellInHouse cellInHouse{houseId};

   ASSERT_TRUE(cellInHouse(Cell::get(41)));
   ASSERT_TRUE(cellInHouse(Cell::get(42)));
   ASSERT_TRUE(cellInHouse(Cell::get(43)));
   ASSERT_FALSE(cellInHouse(Cell::getFromRowCol(5, 6)));
   ASSERT_FALSE(cellInHouse(Cell::get(0)));
}
