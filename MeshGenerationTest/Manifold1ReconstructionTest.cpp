#include <gtest/gtest.h>

#include "Manifold1Reconstruction.h"
#include "PointClose.h"
#include "Single.h"
#include "TopologyDefines.h"
#include "TrianglesNodes.h"
#include "UniquePointCollectionBinning.h"

using namespace MeshGeneration;
using namespace Topology;
using namespace Utilities;

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
   const std::array<NodeIndex, 3 > nodes{ 3, 1, 4 };
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(1, 0, 3);
   trianglesNodes.addTriangle(3, 2, 4);
   const auto reconstruction = MeshGeneration::Generate2(nodes, trianglesNodes, points);
   ASSERT_TRUE(reconstruction.Singletons.empty());
   ASSERT_TRUE(reconstruction.Cycles.empty());
   const auto path = Single(reconstruction.Paths);
   const std::vector<NodeIndex> expect{ 1, 3, 4 };
   ASSERT_TRUE(std::equal(path.begin(), path.end(), expect.begin(), expect.end()));
}