#include <gtest/gtest.h>

#include "DirectedEdge.h"
#include "DirectedEdgeIntersections.h" 
#include "PointClose.h"

using namespace Geometry;

TEST(DirectedEdgeIntersections, empty)
{
   const PointClose<double, GeomDim2> areClose;
   const std::vector<DirectedEdgePoint<double, GeomDim2>> intersectionPoints;
   const auto isContained = [](const Point2& p) {return false; };
   const DirectedEdgeIntersections<double, GeomDim2> intersections(intersectionPoints, isContained, areClose);
   ASSERT_TRUE(intersections.empty());
}

TEST(DirectedEdgeIntersections, single)
{
   const PointClose<double, GeomDim2> areClose;
   const Point2 p0{ 0,0 };
   const Point2 p1{ 1,1 };
   const DirectedEdge<double, GeomDim2> edge(p0, p1);
   const DirectedEdgePoint<double, GeomDim2> dep(p1, edge, areClose);

   const std::vector<DirectedEdgePoint<double, GeomDim2>> intersectionPoints{ dep, dep };
   const auto isContained = [](const Point2& p) {return false; };
   const DirectedEdgeIntersections<double, GeomDim2> intersections(intersectionPoints, isContained, areClose);
   ASSERT_FALSE(intersections.empty());
   ASSERT_TRUE(intersections.size() == 1);
   const auto i0 = intersections[0];
   ASSERT_TRUE((std::get<DirectedEdgePoint<double, 2>>(i0)).getPointType() == DirectedEdgePointType::Point1);
}
