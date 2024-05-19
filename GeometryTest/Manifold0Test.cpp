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
   PointExact<Rational, 1> areEqual;
   ASSERT_TRUE(manifold.contains(point, areEqual));
   ASSERT_TRUE(areEqual.SamePoints(manifold.GetPoint(), point));
   const auto bb = manifold.getBoundingBox();
   ASSERT_TRUE(areEqual.SamePoints(bb.getLower(), point));
   ASSERT_TRUE(areEqual.SamePoints(bb.getUpper(), point));
}
