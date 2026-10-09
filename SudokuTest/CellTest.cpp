#include <gtest/gtest.h>

#include "Cell.h"

using namespace Sudoku;

TEST(CellTest, Min)
{
   const Cell cell{0};
   ASSERT_EQ(cell.row, 0);
   ASSERT_EQ(cell.col, 0);
   ASSERT_EQ(cell.box, 0);
}

TEST(CellTest, Max)
{
   const Cell cell{80};
   ASSERT_EQ(cell.row, 8);
   ASSERT_EQ(cell.col, 8);
   ASSERT_EQ(cell.box, 8);
}

TEST(CellTest, All)
{
   std::array<std::vector<Field>, NumIndex> row2field;
   std::array<std::vector<Field>, NumIndex> col2field;
   std::array<std::vector<Field>, NumIndex> box2field;

   for (Field f = 0; f < NumField; ++f)
   {
      const Cell cell(f);
      ASSERT_EQ(cell.field, f);
      row2field.at(cell.row).push_back(f);
      col2field.at(cell.col).push_back(f);
      box2field.at(cell.box).push_back(f);
   }

   for (auto idx : IndexAll)
   {
      ASSERT_EQ(row2field.at(idx).size(), NumIndex);
      ASSERT_EQ(col2field.at(idx).size(), NumIndex);
      ASSERT_EQ(box2field.at(idx).size(), NumIndex);
   }
}
