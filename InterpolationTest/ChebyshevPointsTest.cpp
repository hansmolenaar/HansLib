#include <gtest/gtest.h>

#include "ChebyshevPoints.h"

#include <cmath>

TEST(ChebyshevPointsTest, Case1)
{
   const auto& points = ChebyshevPoints::Generate(0, 10, 1);
   ASSERT_EQ(points.size(), 1);
   ASSERT_NEAR(points.at(0), 5.0, 1.0e-10);
}


TEST(ChebyshevPointsTest, Case2)
{
   const auto& points = ChebyshevPoints::Generate(-2, 2, 2);
   ASSERT_EQ(points.size(), 2);
   ASSERT_NEAR(points.at(0), std::sqrt(2), 1.0e-10);
   ASSERT_NEAR(points.at(1), -std::sqrt(2), 1.0e-10);
}



