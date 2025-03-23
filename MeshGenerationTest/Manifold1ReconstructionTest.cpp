#include <gtest/gtest.h>

#include "Manifold1Reconstruction.h"
#include "ManifoldId.h"
#include "PointClose.h"
#include "Single.h"
#include "TopologyDefines.h"
#include "TrianglesNodes.h"
#include "UniquePointCollectionBinning.h"

using namespace MeshGeneration;
using namespace Topology;
using namespace Utilities;
using namespace Geometry;

TEST(Manifold1ReconstructionTest, singleEdge)
{
   const PointClose<GeomType, GeomDim2> areClose;
   const UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{Point2{ 1,1 }, Point2{ 2,1 }, Point2{ 1,2 }});
   const std::array<NodeIndex, 2 > nodes{ 1, 2 };
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(0, 1, 2);
   const auto reconstruction = MeshGeneration::Generate2(nodes, trianglesNodes, points);
}


TEST(Manifold1ReconstructionTest, twoEdges)
{
   const PointClose<GeomType, GeomDim2> areClose;
   UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{Point2{ -1, -1 }, Point2{ 5,2 }});
   const auto node0 = points.addIfNew(Point2{ 1,0 });
   const auto node1 = points.addIfNew(Point2{ 0,0 });
   const auto node2 = points.addIfNew(Point2{ 3,1 });
   const auto node3 = points.addIfNew(Point2{ 2,0 });
   const auto node4 = points.addIfNew(Point2{ 4,0 });
   const std::array<NodeIndex, 3 > nodes{ node3, node1, node4 };
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(node1, node0, node3);
   trianglesNodes.addTriangle(node3, node2, node4);
   const auto reconstruction = MeshGeneration::Generate2(nodes, trianglesNodes, points);
   ASSERT_TRUE(reconstruction.Singletons.empty());
   ASSERT_TRUE(reconstruction.Cycles.empty());
   const auto path = Single(reconstruction.Paths);
   const std::vector<NodeIndex> expect{ node1, node3, node4 };
   ASSERT_TRUE(str::equal(path, expect));

   const Geometry::ManifoldId manifoldId(Topology::Edge, "Hello");
   const Manifold1Reconstruction mr(manifoldId, reconstruction);
   ASSERT_TRUE(mr.contains(EdgeNodesSorted(node3, node1)));
   ASSERT_FALSE(mr.contains(EdgeNodesSorted(node1, node2)));
}