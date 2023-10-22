#include <gtest/gtest.h>

#include "FieldInfoStatic.h"
#include "MyException.h"

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
