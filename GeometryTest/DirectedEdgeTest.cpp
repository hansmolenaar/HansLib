#include <gtest/gtest.h>

#include "DirectedEdge.h"
#include "PointClose.h"

using namespace Geometry;

TEST(DirectedEdgeTest, UnhappyPath)
{
   const PointClose<double, 1> areClose;
   const std::array<double, 1> point{ 1.0 };
   ASSERT_TRUE(areClose.SamePoints(point, point));
   const DirectedEdge<double, 1> edge(point, point);
   ASSERT_TRUE(edge.isDegenerate(areClose));
   ASSERT_THROW(edge.project(point), MyException);
}

TEST(DirectedEdgeTest, LengthSquared)
{
   const PointClose<double, 2> areClose;
   const Point2 from{ 4.0, 2.0 };
   const Point2 to{ -1, 2 };
   const DirectedEdge<double, 2> edge(from, to);
   ASSERT_NEAR(edge.lengthSquared(), 25.0, 1.0e-10);
}


TEST(DirectedEdgeTest, Project)
{
   const PointClose<double, 2> areClose;
   const Point2 from{ 1, 0 };
   const Point2 to{ 3, 0 };
   const DirectedEdge<double, 2> edge(from, to);
   ASSERT_NEAR(edge.project({ 0,0 }), -0.5, 1.0e-10);
   ASSERT_NEAR(edge.project({ 0,10 }), -0.5, 1.0e-10);
   ASSERT_NEAR(edge.project({ 1,0 }), 0.0, 1.0e-10);
   ASSERT_NEAR(edge.project({ 1,-10 }), 0.0, 1.0e-10);
   ASSERT_NEAR(edge.project({ 2,0 }), 0.5, 1.0e-10);
   ASSERT_NEAR(edge.project({ 3,10 }), 1.0, 1.0e-10);
   ASSERT_NEAR(edge.project({ 5,5 }), 2.0, 1.0e-10);
}

TEST(DirectedEdgeTest, Contains)
{
   const PointClose<double, 2> areClose;
   const Point2 from{ 1, 0 };
   const Point2 to{ 3, 0 };
   const DirectedEdge<double, 2> edge(from, to);
   ASSERT_FALSE(edge.contains({ 0,0 }, areClose));
   ASSERT_FALSE(edge.contains({ 0, 10 }, areClose));
   ASSERT_TRUE(edge.contains({ 1,0 }, areClose));
   ASSERT_FALSE(edge.contains({ 1,-10 }, areClose));
   ASSERT_TRUE(edge.contains({ 2,0 }, areClose));
   ASSERT_FALSE(edge.contains({ 2,1 }, areClose));
   ASSERT_TRUE(edge.contains({ 3,0 }, areClose));
   ASSERT_FALSE(edge.contains({ 3,1 }, areClose));
   ASSERT_FALSE(edge.contains({ 4,0 }, areClose));

}
