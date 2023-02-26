#include <gtest/gtest.h>

#include "EquidistantPoints.h"
#include "MathConstants.h"

TEST(EquidistantPointsTest, Steps1)
{
   const auto& points = EquidistantPoints::Generate(-1, 1, 1);
   ASSERT_EQ(points.size(), 2);
   ASSERT_NEAR(points.at(0), -1.0, 1.0e-10);
   ASSERT_NEAR(points.at(1), 1.0, 1.0e-10);
}


TEST(EquidistantPointsTest, Steps2)
{
   const auto& points = EquidistantPoints::Generate(-2, 4, 2);
   ASSERT_EQ(points.size(), 3);
   ASSERT_NEAR(points.at(0), -2.0, 1.0e-10);
   ASSERT_NEAR(points.at(1), 1.0, 1.0e-10);
   ASSERT_NEAR(points.at(2), 4.0, 1.0e-10);
}

TEST(EquidistantPointsTest, Steps13)
{
   const auto& points = EquidistantPoints::Generate(MathConstants::SQRT1_2, MathConstants::PI, 13);
   ASSERT_EQ(points.size(), 14);
   const double del = points.at(1) - points.at(0);
   for (int n = 1; n < 14; ++n)
   {
      ASSERT_NEAR(points.at(n) - points.at(n - 1), del, 1.0e-10);
   }
}