#include <gtest/gtest.h>

#include "Manifold0.h"
#include "Rational.h"
#include "PointExact.h"

using namespace Geometry;

TEST(Manifold0Test, Basics)
{
   const Point<Rational, 1> point(Rational(1, 2));
   const Manifold0<Rational, 1> manifold(point);
   ASSERT_EQ(manifold.GetTopologyDimension(), Topology::Corner);
   ASSERT_TRUE(manifold.contains(point, PointExact<Rational, 1>()));
}
