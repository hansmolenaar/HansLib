#include <gtest/gtest.h>

#include "Coordinates.h"

using namespace Sudoku;

TEST(CoordinatesTest, DefaultConstructor)
{
   const Coordinates c;
   ASSERT_ANY_THROW(c.toField());
}

TEST(CoordinatesTest, Min)
{
   const Coordinates c{0, 0};
   const auto field = c.toField();
   ASSERT_EQ(field, 0);
}

TEST(CoordinatesTest, Max)
{
   const Coordinates c{8, 8};
   const auto field = c.toField();
   ASSERT_EQ(field, 80);
}

TEST(CoordinatesTest, All)
{
   for (Field f = 0; f < NumField; ++f)
   {
      const Coordinates c = Coordinates::fromField(f);
      const auto retval = c.toField();
      ASSERT_EQ(f, retval);
   }
}

TEST(CoordinatesTest, AllReverse)
{
   for (auto row : IndexAll)
   {
      for (auto col : IndexAll)
      {
         const Coordinates cor(row, col);
         const Field field = cor.toField();
         const auto retval = Coordinates::fromField(field);
         ASSERT_EQ(row, retval.Row);
         ASSERT_EQ(col, retval.Col);
      }
   }
}
