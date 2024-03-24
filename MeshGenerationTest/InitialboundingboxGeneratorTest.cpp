#include <gtest/gtest.h>

#include "InitialBoundingboxGenerator.h"
#include "Sphere.h"
#include <array>

TEST(InitialboundingboxGeneratorTest, Scale1D)
{
   constexpr double eps = 1.0e-12;
   const Geometry::Sphere sphere(Point1{ 2.5 }, 0.5);
   const BoundingBox<double, 1> bb = sphere.getBoundingBox();
   ASSERT_NEAR(bb.getInterval(0).getMeasure(), 1.0, eps);

   constexpr double factor = 2;
   const auto generator = InitialBoundingboxGenerator<1> ::Create(factor);
   const auto initialBB = generator->generate(sphere);
   ASSERT_NEAR(initialBB.getInterval(0).getMeasure(), factor, eps);
   ASSERT_NEAR(initialBB.getInterval(0).getLower(), 1.5, eps);
   ASSERT_NEAR(initialBB.getInterval(0).getUpper(), 3.5, eps);
}