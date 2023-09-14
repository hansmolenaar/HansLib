#include <gtest/gtest.h>

#include "DirectedEdge.h"
#include "PointClose.h"

using namespace Geometry;
TEST(DirectedEdgeTest, UnhappyPath)
{
   const PointClose<double, 1> areClose;
   const std::array<double, 1> point{ 1.0 };
   ASSERT_TRUE(areClose.SamePoints(point, point));
   ASSERT_ANY_THROW((DirectedEdge<double, 1>::Create(point, point, areClose)));
}

TEST(DirectedEdgeTest, LengthSquared)
{
   const PointClose<double, 2> areClose;
   const Point2 from{ 4.0, 2.0 };
   const Point2 to{ -1, 2 };
   auto edge = DirectedEdge<double, 2>::Create(from, to, areClose);
   ASSERT_NEAR(edge.lengthSquared(), 25.0, 1.0e-10);
}
