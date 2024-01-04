#include <gtest/gtest.h>

#include "InitialBoundingboxGenerator.h"
#include <array>

TEST(InitialboundingboxGeneratorTest, Scale1D)
{
   constexpr double eps = 1.0e-12;
   const BoundingBox<double, 1> bb = BoundingBox<double, 1>::CreateFrom2Points(Point1{ 2 }, Point1{ 3 });
   ASSERT_NEAR(bb.getInterval(0).getMeasure(), 1.0, eps);

   constexpr double factor = 2;
   const auto generator = InitialBoundingboxGenerator<1> ::Create(factor);
   const auto initialBB = generator->generate(bb);
   ASSERT_NEAR(initialBB.getInterval(0).getMeasure(), factor, eps);
   ASSERT_NEAR(initialBB.getInterval(0).getLower(), 1.5, eps);
   ASSERT_NEAR(initialBB.getInterval(0).getUpper(), 3.5, eps);
}