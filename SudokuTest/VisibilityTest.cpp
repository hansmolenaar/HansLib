#include <gtest/gtest.h>

#include "Single.h"
#include "SudokuDefines.h"
#include "Visibility.h"

using namespace Sudoku;
using namespace Utilities;

TEST(VisibilityTest, AreVisible)
{
   const Cell cell{42};
   ASSERT_TRUE(Visiblity::areVisible(cell, Cell::getFromRowCol(4, 0)));
   ASSERT_TRUE(Visiblity::areVisible(cell, Cell::getFromRowCol(0, 6)));
   ASSERT_TRUE(Visiblity::areVisible(cell, Cell::getFromRowCol(3, 8)));
   ASSERT_FALSE(Visiblity::areVisible(cell, Cell::getFromRowCol(8, 8)));
}

TEST(VisibilityTest, AreVisibleField)
{
   const Field field{0};
   ASSERT_TRUE(Visiblity::areVisible(field, Cell::rowCol2Field(0, 3)));
   ASSERT_TRUE(Visiblity::areVisible(field, Cell::rowCol2Field(8, 0)));
   ASSERT_TRUE(Visiblity::areVisible(field, Cell::rowCol2Field(2, 2)));

   ASSERT_FALSE(Visiblity::areVisible(field, Cell::rowCol2Field(3, 3)));
}

TEST(VisibilityTest, VisibleFields)
{
   for (Field field : FieldAll)
   {
      std::span<const Cell> cells = Visiblity::getVisibleCells(Cell::get(field));
      std::span<const Cell> cellsFromField = Visiblity::getVisibleCells(field);

      ASSERT_EQ(cells.size(), NumVisibleCellls);
      ASSERT_EQ(cells.size(), cellsFromField.size());
      for (size_t n = 0; n < NumVisibleCellls; ++n)
      {
         ASSERT_NE(cells[n].field, field);
         ASSERT_EQ(cells[n].field, cellsFromField[n].field);
         ASSERT_TRUE(Visiblity::areVisible(field, cells[n].field));
         if (n > 0)
         {
            // String increasing
            ASSERT_LT(cells[n - 1].field, cells[n].field);
         }
      }
   }
}

TEST(VisibilityTest, AreVisibleVia)
{
   const Cell cell{0};
   auto retval = Visiblity::areVisibleVia(cell, cell);
   ASSERT_TRUE(retval.empty());

   retval = Visiblity::areVisibleVia(cell, Cell::getFromRowCol(3, 3));
   ASSERT_TRUE(retval.empty());

   retval = Visiblity::areVisibleVia(cell, Cell::getFromRowCol(0, 8));
   ASSERT_EQ(Single(retval), HouseType::Row);

   retval = Visiblity::areVisibleVia(cell, Cell::getFromRowCol(8, 0));
   ASSERT_EQ(Single(retval), HouseType::Col);

   retval = Visiblity::areVisibleVia(cell, Cell::getFromRowCol(2, 2));
   ASSERT_EQ(Single(retval), HouseType::Box);

   retval = Visiblity::areVisibleVia(cell, Cell::getFromRowCol(0, 2));
   ASSERT_EQ(retval.size(), 2);
   ASSERT_EQ(retval[0], HouseType::Row);
   ASSERT_EQ(retval[1], HouseType::Box);

   retval = Visiblity::areVisibleVia(cell, Cell::getFromRowCol(2, 0));
   ASSERT_EQ(retval.size(), 2);
   ASSERT_EQ(retval[0], HouseType::Col);
   ASSERT_EQ(retval[1], HouseType::Box);
}
