#include <gtest/gtest.h>

#include "FieldInfoStatic.h"
#include "MyException.h"
#include <unordered_set>
using namespace Sudoku;

TEST(FieldInfoStaticTest, InstanceTest)
{
   const std::array<FieldInfoStatic, NumFields>& instance = FieldInfoStatic::Instance();
   std::map<RowColBoxIndex, int> rowCount;
   std::map<RowColBoxIndex, int> colCount;
   std::map<RowColBoxIndex, int> boxCount;

   for (const auto& info : instance)
   {
      ASSERT_GE(info.Row, 0);
      ASSERT_LT(info.Row, NumRowColBox);

      ASSERT_GE(info.Col, 0);
      ASSERT_LT(info.Col, NumRowColBox);

      ASSERT_GE(info.Box, 0);
      ASSERT_LT(info.Box, NumRowColBox);

      rowCount[info.Row] += 1;
      colCount[info.Col] += 1;
      boxCount[info.Box] += 1;
   }

   for (auto indx : RowColBoxAll)
   {
      ASSERT_EQ(rowCount.at(indx), NumRowColBox);
      ASSERT_EQ(colCount.at(indx), NumRowColBox);
   }

   for (auto indx : RowColBoxAll)
   {
      ASSERT_EQ(boxCount.at(indx), NumRowBoxCol);
   }
}


TEST(FieldInfoStaticTest, RowColField)
{
   const std::array<FieldInfoStatic, NumFields>& instance = FieldInfoStatic::Instance();
   for (FieldIndex f = 0; f < NumFields; ++f)
   {
      const auto& info = instance.at(f);
      ASSERT_EQ(FieldInfoStatic::RowColToField(info.Row, info.Col), f);
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
   const auto& info = FieldInfoStatic::Instance();

   for (auto row : RowColBoxAll)
   {
      const auto& cols = FieldInfoStatic::GetRow(row);
      for (auto f : cols)
      {
         used.insert(f);
         ASSERT_EQ(info.at(f).Row, row);
      }
   }
   ASSERT_EQ(used.size(), NumFields);
}


TEST(FieldInfoStaticTest, GetCol)
{
   std::unordered_set<FieldIndex> used;
   const auto& info = FieldInfoStatic::Instance();

   for (auto col : RowColBoxAll)
   {
      const auto& rows = FieldInfoStatic::GetCol(col);
      for (auto f : rows)
      {
         used.insert(f);
         ASSERT_EQ(info.at(f).Col, col);
      }
   }
   ASSERT_EQ(used.size(), NumFields);
}


TEST(FieldInfoStaticTest, GetBox)
{
   std::unordered_set<FieldIndex> used;
   const auto& info = FieldInfoStatic::Instance();

   for (auto b : RowColBoxAll)
   {
      const auto& fields = FieldInfoStatic::GetBox(b);
      for (auto f : fields)
      {
         used.insert(f);
         ASSERT_EQ(info.at(f).Box, b);
      }
   }
   ASSERT_EQ(used.size(), NumFields);
}

