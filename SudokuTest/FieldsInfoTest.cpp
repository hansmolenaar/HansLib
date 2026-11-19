#include <gtest/gtest.h>

#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "House.h"
#include "SudokuDefines.h"
#include "SweepNakedSingle.h"
#include "TestModels.h"
#include "Visibility.h"

using namespace Sudoku;
using namespace Utilities;

TEST(FieldsInfoTest, GetHouseFields)
{
   CandidatesGrid grid;
   FieldsInfo finfo(grid);

   for (const auto &house : House::getAllHouses())
   {
      const FieldBits &fb = finfo.getHouseFields(house.houseId);
      ASSERT_EQ(fb.count(), 9);
   }
}

TEST(FieldsInfoTest, GetActiveFieldsWithValue)
{
   CandidatesGrid grid;
   FieldsInfo finfo(grid);
   FieldBits fieldsValue = finfo.getActiveFieldsWithValue(3);
   ASSERT_EQ(fieldsValue.count(), NumField);

   grid = TestModels::getSolvedGrid();
   finfo = FieldsInfo(grid);
   fieldsValue = finfo.getActiveFieldsWithValue(3);
   ASSERT_EQ(fieldsValue.count(), 0);
}

TEST(FieldsInfoTest, GetNotSolvedCells)
{
   CandidatesGrid grid;
   grid.setGiven(42, 4);
   auto finfo = FieldsInfo(grid);
   ASSERT_EQ(finfo.getActiveFields().count(), NumField - 1);

   grid.setGiven(0, 5);
   finfo = FieldsInfo(grid);
   ASSERT_EQ(finfo.getActiveFields().count(), NumField - 2);
   ASSERT_EQ(Bitset{finfo.getActiveFields()}.first(), 1);

   grid = TestModels::getSolvedGrid();
   finfo = FieldsInfo(grid);
   const auto activeCells = finfo.getActiveFields();
   ASSERT_TRUE(activeCells.none());
}

TEST(FieldsInfoTest, GetSolvedCellsBitSet)
{
   CandidatesGrid grid;
   grid.setGiven(42, 4);
   FieldsInfo finfo(grid);
   const auto solvedCells = finfo.getSolvedFields();
   ASSERT_EQ(solvedCells.count(), 1);
   ASSERT_TRUE(solvedCells.test(42));
}

TEST(FieldsInfoTest, GetSolvedCells)
{
   CandidatesGrid grid;
   FieldsInfo finfo(grid);
   const HouseId house{HouseType::Box, 8};
   const auto houseFields = finfo.getHouseFields(house);
   FieldBits activeFields = finfo.getActiveFields() & houseFields;
   ASSERT_EQ(activeFields.count(), NumCellsInHouse);

   grid.setGiven(6, 6, 1);
   finfo.update();
   activeFields = finfo.getActiveFields() & houseFields;
   ASSERT_EQ(activeFields.count(), NumCellsInHouse - 1);

   grid.setGiven(7, 7, 5);
   finfo.update();
   activeFields = finfo.getActiveFields() & houseFields;
   ASSERT_EQ(activeFields.count(), NumCellsInHouse - 2);

   const std::vector<Field> expect{61, 62, 69, 71, 78, 79, 80};
   auto range = FieldAll | std::views::filter(Bitset{activeFields});
   ASSERT_TRUE(std::equal(range.begin(), range.end(), expect.begin(), expect.end()));
}

TEST(FieldsInfoTest, GetVisibleFields)
{
   const CandidatesGrid grid = TestModels::getSolvedGrid();
   FieldsInfo finfo(grid);

   for (Field field : FieldAll)
   {
      const auto visible = finfo.getVisibleFields(field);
      ASSERT_EQ(visible.count(), 20);
      ASSERT_FALSE(visible.test(field));
   }
}

TEST(FieldsInfoTest, GetAllVisibleFields)
{
   const CandidatesGrid grid = TestModels::getSolvedGrid();
   FieldsInfo finfo(grid);
   FieldBits cells;
   cells.set(0);

   FieldBits visibleFields = finfo.getAllVisibleFields(cells);
   ASSERT_EQ(visibleFields.count(), 20);

   cells.set(1);
   visibleFields = finfo.getAllVisibleFields(cells);
   ASSERT_EQ(visibleFields.count(), 27); // 6 new cells in column + cell(0)
}

TEST(VisibilityTest, VisibleFields)
{
   const CandidatesGrid grid;
   const FieldsInfo finfo(grid);

   for (Field field : FieldAll)
   {
      const FieldBits visibleFields = finfo.getVisibleFields(field);
      ASSERT_FALSE(visibleFields.test(field));
      ASSERT_EQ(visibleFields.count(), NumVisibleCellls);
      for (Field visibleField : FieldAll | std::views::filter(Bitset{visibleFields}))
      {
         ASSERT_TRUE(Visiblity::areVisible(field, visibleField));
      }
   }
}

TEST(VisibilityTest, Update)
{
   CandidatesGrid grid;
   grid.setGiven(6, 6, 1);
   grid.setGiven(7, 7, 5);

   FieldsInfo finfo(grid);
   const auto fieldValues = SweepNakedSingle().collect(finfo);
   grid.apply(fieldValues, finfo);

   const auto finfoOld = finfo;
   finfo = FieldsInfo(grid);

   ASSERT_EQ(finfoOld.getActiveFields(), finfo.getActiveFields());
   ASSERT_EQ(finfoOld.getSolvedFields(), finfo.getSolvedFields());
   for (House h : House::getAllHouses())
   {
      ASSERT_EQ(finfoOld.getHouseFields(h.houseId), finfo.getHouseFields(h.houseId));
   }
   for (Value v : ValueAll)
   {
      ASSERT_EQ(finfoOld.getFieldsWithValue(v), finfo.getFieldsWithValue(v));
      ASSERT_EQ(finfoOld.getActiveFieldsWithValue(v), finfo.getActiveFieldsWithValue(v));
   }
}
