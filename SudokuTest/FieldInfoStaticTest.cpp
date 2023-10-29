#include <gtest/gtest.h>

#include "FieldInfoStatic.h"
#include "MyException.h"
#include <unordered_set>
using namespace Sudoku;

TEST(FieldInfoStaticTest, InstanceTest)
{
   const std::array<FieldInfoStatic, NumFields>& instance = FieldInfoStatic::Instance();
   std::map<RowColIndex, int> rowCount;
   std::map<RowColIndex, int> colCount;
   std::map<RowColIndex, int> subSquareCount;

   for (const auto& info : instance)
   {
      ASSERT_GE(info.Row, 0);
      ASSERT_LT(info.Row, NumRowCol);

      ASSERT_GE(info.Col, 0);
      ASSERT_LT(info.Col, NumRowCol);

      ASSERT_GE(info.SubSquare, 0);
      ASSERT_LT(info.SubSquare, NumSubSquares);

      rowCount[info.Row] += 1;
      colCount[info.Col] += 1;
      subSquareCount[info.SubSquare] += 1;
   }

   for (auto indx : RowColAll)
   {
      ASSERT_EQ(rowCount.at(indx), NumRowCol);
      ASSERT_EQ(colCount.at(indx), NumRowCol);
   }

   for (auto indx : SubSquareAll)
   {
      ASSERT_EQ(subSquareCount.at(indx), NumSubSquares);
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
   ASSERT_THROW(FieldInfoStatic::CheckRowColIndex(NumRowCol), MyException);
}


TEST(FieldInfoStaticTest, GetRow)
{
   std::unordered_set<FieldIndex> used;
   const auto& info = FieldInfoStatic::Instance();

   for (auto row : RowColAll)
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

   for (auto col : RowColAll)
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


TEST(FieldInfoStaticTest, GetSubSquare)
{
   std::unordered_set<FieldIndex> used;
   const auto& info = FieldInfoStatic::Instance();

   for (auto ss : SubSquareAll)
   {
      const auto& fields = FieldInfoStatic::GetSubSquare(ss);
      for (auto f : fields)
      {
         used.insert(f);
         ASSERT_EQ(info.at(f).SubSquare, ss);
      }
   }
   ASSERT_EQ(used.size(), NumFields);
}

