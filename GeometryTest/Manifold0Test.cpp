#include <gtest/gtest.h>

#include "IManifold0Test.h"
#include "Manifold0.h"
#include "PointClose.h"
#include "PointExact.h"
#include "Rational.h"

using namespace Geometry;

TEST(Manifold0Test, Basics)
{
   PointExact<Rational, 1> predicate;
   const Point<Rational, 1> point{Rational(1, 2)};
   const Manifold0<Rational, 1> manifold(point, "Manifold0Test_Basics");
   ASSERT_EQ(manifold.getTopologyDimension(), Topology::Corner);
   ASSERT_TRUE(manifold.contains(point, predicate));
   ASSERT_TRUE(predicate.samePoints(manifold.getPoint(), point));
   const auto bb = manifold.getBoundingBox();
   ASSERT_TRUE(predicate.samePoints(bb.getLower(), point));
   ASSERT_TRUE(predicate.samePoints(bb.getUpper(), point));
}


TEST(Manifold0Test, Float)
{
   const PointClose<double, GeomDim1> predicate;
   const Point<double, 1> point{ 0.5 };
   const Manifold0<double, 1> manifold(point, "Manifold0Test_Float");
   IManifold0TestInterface(manifold, predicate);
   ASSERT_EQ(manifold.getTopologyDimension(), Topology::Corner);
   ASSERT_TRUE(manifold.contains(point, predicate));
   ASSERT_TRUE(predicate.samePoints(manifold.getPoint(), point));
   const auto bb = manifold.getBoundingBox();
   ASSERT_TRUE(predicate.samePoints(bb.getLower(), point));
   ASSERT_TRUE(predicate.samePoints(bb.getUpper(), point));
}
