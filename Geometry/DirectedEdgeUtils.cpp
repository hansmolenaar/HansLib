#include "DirectedEdgeUtils.h"


using namespace Geometry;

std::optional<DirectedEdgeIntersection<double, 2>> DirectedEdgeUtils::IntersectWith2(const DirectedEdge2& edge, const DirectedEdge2& target, const IGeometryPredicate<double, GeomDim2>& predicate)
{
   if (edge.isDegenerate(predicate)) throw MyException("DirectedEdgeUtils::IntersectWith2 edge degenerate");

   //const Point2 p1 = edge.pr
   return {};
}