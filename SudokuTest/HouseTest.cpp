#include <gtest/gtest.h>

#include "House.h"
#include "MyAssert.h"

using namespace Sudoku;
using namespace Utilities;

namespace
{
template <typename F, typename G> void TestGetHouse(F &&getHouse, G &&getIndexFromCell)
{
   std::set<Field> fieldsFound;

   for (auto idx : IndexAll)
   {
      const House &house = getHouse(idx);
      MyAssert(house.houseId.houseIndex == idx);
      for (const Cell &cell : house.cells)
      {
         MyAssert(getIndexFromCell(cell) == idx);
         fieldsFound.insert(cell.field);
      }
   }
   MyAssert(fieldsFound.size() == NumField);
   MyAssert(*fieldsFound.rbegin() == 80);
}

} // namespace

TEST(HouseTest, ByRow)
{
   TestGetHouse([](Index idx) { return House::getRow(idx); }, [](const Cell &cell) { return cell.row; });
   const HouseId &houseId = House::getRow(1).houseId;
   ASSERT_EQ(houseId.houseType, HouseType::Row);
   ASSERT_EQ(houseId.houseIndex, 1);
}

TEST(HouseTest, ByCol)
{
   TestGetHouse([](Index idx) { return House::getCol(idx); }, [](const Cell &cell) { return cell.col; });
   const HouseId &houseId = House::getCol(8).houseId;
   ASSERT_EQ(houseId.houseType, HouseType::Col);
   ASSERT_EQ(houseId.houseIndex, 8);
}

TEST(HouseTest, ByBox)
{
   const HouseId &houseId = House::getBox(7).houseId;
   TestGetHouse([](Index idx) { return House::getBox(idx); }, [](const Cell &cell) { return cell.box; });
   ASSERT_EQ(houseId.houseType, HouseType::Box);
   ASSERT_EQ(houseId.houseIndex, 7);
}
