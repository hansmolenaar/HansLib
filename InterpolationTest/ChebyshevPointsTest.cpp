#include <gtest/gtest.h>

#include "Interpolation/ChebyshevPoints.h"
//#include "HLUtils/MathConstants.h"

TEST(ChebyshevPointsTest, Case1)
{
   const auto& points = ChebyshevPoints::Generate(-1, 1, 1);
   ASSERT_EQ(points.size(), 1);
   ASSERT_NEAR(points.at(0), 0.0, 1.0e-10);
}


TEST(ChebyshevPointsTest, Case2)

{
   const auto& points = ChebyshevPoints::Generate(-1, 1, 2);
   ASSERT_EQ(points.size(), 2);
   ASSERT_NEAR(points.at(0), std::sqrt(0.5), 1.0e-10);
   ASSERT_NEAR(points.at(1), -std::sqrt(0.5), 1.0e-10);
}


