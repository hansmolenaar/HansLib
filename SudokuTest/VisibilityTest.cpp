#include <gtest/gtest.h>

#include "SudokuDefines.h"
#include "Visibility.h"

using namespace Sudoku;

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
