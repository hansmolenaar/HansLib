#include "MeshGeneration2.h"
#include "IntervalTreeBalance.h"
#include "Defines.h"   
#include "BoundingBox.h"

using namespace Geometry;
using namespace IntervalTree;

namespace
{
   
}

IndexTreeToSimplices2::Triangles MeshGeneration2::GenerateBaseTriangulation(const IGeometryRegion<double, 2>& region, MeshingStrategy2& strategy)
{
   IndexTree<2> tree;
   tree.refineUntilReady(strategy.getRefinementPredicate());
   IntervalTree::Balance(tree);
   auto triangles = IndexTreeToSimplices2::Create(tree);
#if false
   // Remove triangles that do not overlap with the region
   const BoundingBox<double,2> initialBb = strategy.getInitialBoundingBoxGenerator().generate(region.getBoundingBox());
   const auto [first, last] = str::remove_if(triangles, [&initialBb, &region, &predicate](const std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners>& triangle)
      {
         const auto bbIn01 = BoundingBox<Rational, 2>::CreateFromList(triangle);
         const auto bb = initialBb.scaleFrom01(bbIn01);
         return !region.CouldIntersectWith(bb, predicate);
      });
   triangles.erase(first, last);
#endif
   return triangles;
}