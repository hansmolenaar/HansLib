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
   const std::array<NodeIndex, 2 > nodes{ 1, 2 };
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(TriangleNodesOriented(0, 1, 2));
   const auto reconstruction = MeshGeneration::Generate2(nodes, trianglesNodes);
}


TEST(Manifold1ReconstructionTest, twoEdges)
{
   const NodeIndex node0 = 1;
   const NodeIndex node1 = 10;
   const NodeIndex node2 = 100;
   const NodeIndex node3 = 2;
   const NodeIndex node4 = 12;
   const std::array<NodeIndex, 3 > nodes{ node3, node1, node4 };
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(TriangleNodesOriented(node1, node0, node3));
   trianglesNodes.addTriangle(TriangleNodesOriented(node3, node2, node4));
   const auto reconstruction = MeshGeneration::Generate2(nodes, trianglesNodes);
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