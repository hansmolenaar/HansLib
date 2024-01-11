#include "MeshGeneration2.h"
#include "IntervalTreeBalance.h"
#include "Defines.h"   
#include "BoundingBox.h"
#include "UniqueHashedPointCollection.h"
#include "UniquePointCollectionBinning.h"

using namespace Geometry;
using namespace IntervalTree;
using namespace MeshGeneration;

IndexTreeToSimplices2::Triangles MeshGeneration2::GenerateBaseTriangulation(const IGeometryRegion<double, 2>& region, MeshingStrategy2& strategy, Logger& logger)
{
   std::string msg = "MeshGeneration2::GenerateBaseTriangulation() region Bb " + region.getBoundingBox().toString();
   logger.logLine(msg);

   IndexTree<2> tree;
   tree.refineUntilReady(strategy.getRefinementPredicate());
   msg = "MeshGeneration2::GenerateBaseTriangulation() after refinement: " + tree.toString();

   IntervalTree::Balance(tree);
   return IndexTreeToSimplices2::Create(tree);
}

void MeshGeneration2::BaseTriangulationToWorld(
   const IndexTreeToSimplices2::Triangles& baseTriangles,
   const IGeometryPredicate<double, 2>& predicate,
   const BoundingBox<double, 2>& worldBB,
   std::unique_ptr<IDynamicUniquePointCollection<double, 2>>& pointGeometry,
   std::unique_ptr<MeshGeneration::TriangleNodes>& triangleNodes,
   Logger& logger)
{
   // Clear output
   pointGeometry = nullptr;
   triangleNodes = nullptr;

   // Collect unique points
   UniqueHashedPointCollection<Rational, 2> collectionRational;
   for (auto& triangle : baseTriangles)
   {
      for (auto& p : triangle)
      {
         collectionRational.addIfNew(p);
      }
   }

   const PointIndex numUniquePoints = collectionRational.getNumPoints();
   std::vector<Point2> uniqueWorldPoints;
   uniqueWorldPoints.reserve(numUniquePoints);
   for (PointIndex n = 0; n < numUniquePoints; ++n)
   {
      const RatPoint2 rpoint = collectionRational.getPoint(n);
      uniqueWorldPoints.emplace_back(worldBB.scaleFromPoint01(rpoint));
   }

   pointGeometry = std::make_unique<UniquePointCollectionBinning<2>>(predicate, uniqueWorldPoints);
   std::unordered_map<RatPoint2, PointIndex> toWorld;

   for (PointIndex n = 0; n < numUniquePoints; ++n)
   {
      const PointIndex worldId = *pointGeometry->tryGetClosePoint(uniqueWorldPoints[n]);
      toWorld.emplace(collectionRational.getPoint(n), worldId);
   }

   triangleNodes = std::make_unique<MeshGeneration::TriangleNodes>();
   for (auto& triangle : baseTriangles)
   {
      const auto n0 = static_cast<TriangleNodes::NodeId>(toWorld.at(triangle.at(0)));
      const auto n1 = static_cast<TriangleNodes::NodeId>(toWorld.at(triangle.at(1)));
      const auto n2 = static_cast<TriangleNodes::NodeId>(toWorld.at(triangle.at(2)));
      triangleNodes->addTriangle(n0, n1, n2);
   }
}