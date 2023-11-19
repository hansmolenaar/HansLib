#include <gtest/gtest.h>

#include "FieldInfoStatic.h"
#include "MyException.h"
#include <unordered_set>
using namespace Sudoku;

TEST(FieldInfoStaticTest, InstanceTest)
{
   std::map<RowColBoxIndex, int> rowCount;
   std::map<RowColBoxIndex, int> colCount;
   std::map<RowColBoxIndex, int> boxCount;

   for (auto f : FieldInfoStatic::getAllFields())
   {
      const auto row = FieldInfoStatic::FieldToRow(f);
      const auto col = FieldInfoStatic::FieldToCol(f);
      const auto box = FieldInfoStatic::FieldToCol(f);

      ASSERT_GE(row, 0);
      ASSERT_LT(row, NumRowColBox);

      ASSERT_GE(col, 0);
      ASSERT_LT(col, NumRowColBox);

      ASSERT_GE(box, 0);
      ASSERT_LT(box, NumRowColBox);

      rowCount[row] += 1;
      colCount[col] += 1;
      boxCount[box] += 1;
   }

   for (auto indx : RowColBoxAll)
   {
      ASSERT_EQ(rowCount.at(indx), NumRowColBox);
      ASSERT_EQ(colCount.at(indx), NumRowColBox);
      ASSERT_EQ(boxCount.at(indx), NumRowBoxCol);
   }
}


TEST(FieldInfoStaticTest, RowColField)
{
   for (auto f : FieldInfoStatic::getAllFields())
   {
      const auto row = FieldInfoStatic::FieldToRow(f);
      const auto col = FieldInfoStatic::FieldToCol(f);
      ASSERT_EQ(FieldInfoStatic::RowColToField(row, col), f);
   }
}


TEST(FieldInfoStaticTest, CheckRowColIndex)
{
   ASSERT_THROW(FieldInfoStatic::CheckRowColIndex(-1), MyException);
   ASSERT_THROW(FieldInfoStatic::CheckRowColIndex(NumRowColBox), MyException);
}


TEST(FieldInfoStaticTest, GetRow)
{
   std::unordered_set<FieldIndex> used;
   for (auto row : RowColBoxAll)
   {
      const auto& cols = FieldInfoStatic::GetRow(row);
      for (auto f : cols)
      {
         used.insert(f);
         ASSERT_EQ(FieldInfoStatic::FieldToRow(f), row);
      }
   }
   ASSERT_EQ(used.size(), NumFields);
}


TEST(FieldInfoStaticTest, GetCol)
{
   std::unordered_set<FieldIndex> used;

   for (auto col : RowColBoxAll)
   {
      const auto& rows = FieldInfoStatic::GetCol(col);
      for (auto f : rows)
      {
         used.insert(f);
         ASSERT_EQ(FieldInfoStatic::FieldToCol(f), col);
      }
   }
   ASSERT_EQ(used.size(), NumFields);
}


TEST(FieldInfoStaticTest, GetBox)
{
   std::unordered_set<FieldIndex> used;
   for (auto b : RowColBoxAll)
   {
      const auto& fields = FieldInfoStatic::GetBox(b);
      for (auto f : fields)
      {
         used.insert(f);
         ASSERT_EQ(FieldInfoStatic::FieldToBox(f), b);
      }
   }
   ASSERT_EQ(used.size(), NumFields);
}

