#include <gtest/gtest.h>

#include "InitialBoundingboxGenerator.h"
#include "Ball2AsRegion.h"
#include <array>

using namespace Geometry;
TEST(InitialboundingboxGeneratorTest, Scale1D)
{
   constexpr double eps = 1.0e-12;
   const Ball<double, 2> ball(Point2{ 2.5, 0 }, 0.5);
   const Ball2AsRegion<double> ballAsRegion(ball, "InitialboundingboxGeneratorTest_Scale1D");
   const BoundingBox<double, 2> bb = ball.getBoundingBox();
   ASSERT_NEAR(bb.getInterval(0).getMeasure(), 1.0, eps);

   constexpr double factor = 2;
   const auto generator = InitialBoundingboxGenerator<2> ::Create(factor);
   const auto initialBB = generator->generate(ballAsRegion);
   ASSERT_NEAR(initialBB.getInterval(0).getMeasure(), factor, eps);
   ASSERT_NEAR(initialBB.getInterval(0).getLower(), 1.5, eps);
   ASSERT_NEAR(initialBB.getInterval(0).getUpper(), 3.5, eps);
}