#include <gtest/gtest.h>

#include "Manifold1Reconstruction.h"
#include "UniquePointCollectionBinning.h"
#include "PointClose.h"
#include "TopologyDefines.h"
#include "TrianglesNodes.h"

using namespace MeshGeneration;
using namespace Topology;

TEST(Manifold1ReconstructionTest, singleEdge)
{
   const PointClose<GeomType, GeomDim2> areClose;
   const UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{Point2{ 1,1 }, Point2{ 2,1 }, Point2{ 1,2 }});
   const std::array<NodeIndex, 2 > nodes{ 1, 2 };
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(0, 1, 2);
   const auto reconstruction = Manifold1Reconstruction::Generate2(nodes, trianglesNodes, points);
}
