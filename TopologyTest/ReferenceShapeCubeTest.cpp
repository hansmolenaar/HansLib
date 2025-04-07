#include "ReferenceShapeCube.h"
#include "TestIReferenceShape.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(ReferenceShapeCubeTest, ReferenceShapeCubeBase)
{
   const ReferenceShapeCube& rs = ReferenceShapeCube::getInstance();
   const auto& adjacencies = rs.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), Topology::Volume);
   ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), NumNodesOnCube);
   ASSERT_EQ(adjacencies.getCountSafe(Topology::Edge), NumEdgesOnCube);
   ASSERT_EQ(adjacencies.getCountSafe(Topology::Face), NumFacesOnCube);
   ASSERT_TRUE(adjacencies.isComplete());

   // Every node connected to 3 edges and faces?
   const auto* adjacencyC2E = *adjacencies.getAdjacency(Topology::Corner, Topology::Edge);
   const auto* adjacencyC2F = *adjacencies.getAdjacency(Topology::Corner, Topology::Face);
   for (int c = 0; c < NumNodesOnCube; ++c)
   {
      ASSERT_EQ(adjacencyC2E->getConnectedHighers(c).size(), 3);
      ASSERT_EQ(adjacencyC2F->getConnectedHighers(c).size(), 3);
   }

   // Every edge connected to 2 faces?
   const auto* adjacencyE2F = *adjacencies.getAdjacency(Topology::Edge, Topology::Face);
   for (int e = 0; e < NumEdgesOnCube; ++e)
   {
      const auto tmp = adjacencyE2F->getConnectedHighers(e);
      ASSERT_EQ(adjacencyE2F->getConnectedHighers(e).size(), 2);
   }
}


TEST(ReferenceShapeCubeTest, TestInterface)
{
   TopologyTest::TestIReferenceShape(ReferenceShapeCube::getInstance());
}
