#include <gtest/gtest.h>

#include "CellQuality2.h"
#include "EdgeFlip.h"
#include "Manifold1Reconstruction.h"
#include "ManifoldId.h"
#include "PointClose.h"
#include "TopologyDefines.h"
#include "TrianglesNodes.h"
#include "UniquePointCollectionBinning.h"

using namespace MeshGeneration;
using namespace Topology;
using namespace Utilities;
using namespace Geometry;

TEST(EdgeFlipTest, base)
{
   const PointClose<GeomType, GeomDim2> areClose;
   UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{Point2{ -1, -1 }, Point2{ 1,1 }});
   const auto node0 = points.addIfNew(Point2{ -1,0 });
   const auto node1 = points.addIfNew(Point2{ 1,0 });
   const auto node2 = points.addIfNew(Point2{ 0,0.1 });
   const auto node3 = points.addIfNew(Point2{ 0, -0.1 });
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(node0, node1, node2);
   trianglesNodes.addTriangle(node0, node1, node3);

   Reconstruction1 reconstruction;
   reconstruction.Cycles.push_back(std::vector<NodeIndex>{ node0, node2, node1, node3 });

   const Geometry::ManifoldId manifoldId(Topology::Edge, "Loop");
   std::vector<std::unique_ptr<IManifoldReconstruction>> reconstructions;
   reconstructions.emplace_back(std::make_unique<Manifold1Reconstruction>(manifoldId, reconstruction));

   EdgeFlip edgeFlip(trianglesNodes, CellQuality2::MinimumAngle, points, reconstructions);
   ASSERT_TRUE(edgeFlip.isFlippable(EdgeNodesSorted(node0, node1)));
   ASSERT_TRUE(edgeFlip.isFlippable(EdgeNodesSorted(node2, node3))); // does not exist
   ASSERT_FALSE(edgeFlip.isFlippable(EdgeNodesSorted(node0, node3)));

   const EdgeFlipStrategy strategy{ 0.9, 2 };
   edgeFlip.execute(strategy);
}