#include <gtest/gtest.h>

#include "DirectedEdge.h"
#include "DirectedEdgePoint.h"
#include "PointClose.h"

using namespace Geometry;

TEST(DirectedEdgePointTest, equals)
{
   const PointClose<double, GeomDim2> areClose;
   const DirectedEdgePointEquals<double, 2> equals{ areClose };
   const Point2 p0{ 0,0 };
   const Point2 p1{ 1,1 };
   const DirectedEdge<double, GeomDim2> edge(p0, p1);
   const DirectedEdgePoint<double, GeomDim2> dep0(p0, edge, areClose);
   const DirectedEdgePoint<double, GeomDim2> dep1(p1, edge, areClose);
   const DirectedEdgePoint<double, GeomDim2> mid1({ 0.1, 0.1 }, edge, areClose);
   const DirectedEdgePoint<double, GeomDim2> mid2({ 0.2, 0.2 }, edge, areClose);

   ASSERT_TRUE(dep0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(dep1.getPointType() == DirectedEdgePointType::Point1);

   ASSERT_TRUE(equals(dep0, dep0));
   ASSERT_TRUE(equals(dep1, dep1));
   ASSERT_TRUE(equals(mid1, mid1));

   ASSERT_FALSE(equals(dep0, mid1));
   ASSERT_FALSE(equals(dep0, dep1));

   ASSERT_FALSE(equals(dep1, dep0));
   ASSERT_FALSE(equals(dep1, mid1));

   ASSERT_FALSE(equals(mid1, dep0));
   ASSERT_FALSE(equals(mid1, dep1));
   ASSERT_FALSE(equals(mid1, mid2));
   ASSERT_FALSE(equals(mid2, mid1));
}


TEST(DirectedEdgePointTest, less)
{
   const PointClose<double, GeomDim2> areClose;
   const DirectedEdgePointLess<double, 2> less{ areClose };
   const Point2 p0{ 0,0 };
   const Point2 p1{ 1,1 };
   const DirectedEdge<double, GeomDim2> edge(p0, p1);
   const DirectedEdgePoint<double, GeomDim2> dep0(p0, edge, areClose);
   const DirectedEdgePoint<double, GeomDim2> dep1(p1, edge, areClose);
   const DirectedEdgePoint<double, GeomDim2> mid1({ 0.1, 0.1 }, edge, areClose);
   const DirectedEdgePoint<double, GeomDim2> mid2({ 0.2, 0.2 }, edge, areClose);

   ASSERT_FALSE(less(dep0, dep0));
   ASSERT_FALSE(less(dep1, dep1));
   ASSERT_FALSE(less(mid1, mid1));

   ASSERT_TRUE(less(dep0, mid1));
   ASSERT_TRUE(less(dep0, dep1));

   ASSERT_FALSE(less(dep1, dep0));
   ASSERT_FALSE(less(dep1, mid1));

   ASSERT_FALSE(less(mid1, dep0));
   ASSERT_TRUE(less(mid1, dep1));
   ASSERT_TRUE(less(mid1, mid2));
   ASSERT_FALSE(less(mid2, mid1));
}
