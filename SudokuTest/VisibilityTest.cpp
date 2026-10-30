#include <gtest/gtest.h>

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
