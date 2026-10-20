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
   ASSERT_EQ(House::getRow(8).type, HouseType::Row);
}

TEST(HouseTest, ByCol)
{
   TestGetHouse([](Index idx) { return House::getCol(idx); }, [](const Cell &cell) { return cell.col; });
   ASSERT_EQ(House::getCol(8).type, HouseType::Col);
}

TEST(HouseTest, ByBox)
{
   TestGetHouse([](Index idx) { return House::getBox(idx); }, [](const Cell &cell) { return cell.box; });
   ASSERT_EQ(House::getBox(8).type, HouseType::Box);
}
