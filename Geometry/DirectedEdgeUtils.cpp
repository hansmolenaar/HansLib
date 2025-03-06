#include "DirectedEdgeUtils.h"
#include "Interval.h"

using namespace Geometry;

std::optional<DirectedEdgeIntersection<double, 2>> DirectedEdgeUtils::intersectWith2(const DirectedEdge2& edge, const DirectedEdge2& target, const IGeometryPredicate<double, GeomDim2>& predicate)
{
   if (edge.isDegenerate(predicate)) throw MyException("DirectedEdgeUtils::IntersectWith2 edge degenerate");

   const double lambda0 = edge.project(target.point0());
   const double lambda1 = edge.project(target.point1());
   const Point2 p0 = edge.interpolate(lambda0);
   const Point2 p1 = edge.interpolate(lambda1);
   const bool isContained0 = predicate.SamePoints(p0, target.point0());
   const bool isContained1 = predicate.SamePoints(p1, target.point1());
   if (isContained0 && !isContained1)
   {
      if (edge.contains(p0, predicate))
      {
         return { DirectedEdgePoint<double, GeomDim2>(target.point0(), edge, predicate) };
      }
      return {};
   }
   if (!isContained0 && isContained1)
   {
      if (edge.contains(p1, predicate))
      {
         return { DirectedEdgePoint<double, GeomDim2>(target.point1(), edge, predicate) };
      }
      return {};
   }
   if (isContained0 && isContained1)
   {
      // colinear
      const Interval<double> intvProjection(std::min(lambda0, lambda1), std::max(lambda0, lambda1));
      const Interval<double> intvEdge(0, 1);
      double lambdaIp0 = std::numeric_limits<double>::quiet_NaN();
      double lambdaIp1 = std::numeric_limits<double>::quiet_NaN();
      const bool succes = Interval<double>::TryIntersect(intvProjection, intvEdge, lambdaIp0, lambdaIp1);
      if (!succes) return {};
      const Point2 ipoint0 = edge.interpolate(lambdaIp0);
      const Point2 ipoint1 = edge.interpolate(lambdaIp1);
      const DirectedEdgePoint<double, GeomDim2> dep0(ipoint0, edge, predicate);
      if (predicate.SamePoints(ipoint0, ipoint1))
      {
         return  { dep0 };
      }
      else
      {
         const DirectedEdgePoint<double, GeomDim2> dep1(ipoint1, edge, predicate);
         return { DirectedEdgeInterval<double, GeomDim2>(dep0, dep1) };
      }
   }

   return {};
}