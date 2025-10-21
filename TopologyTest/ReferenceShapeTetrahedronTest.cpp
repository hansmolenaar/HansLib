#include "ReferenceShapeTetrahedron.h"
#include "TestIReferenceShape.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(ReferenceShapeTetrahedronTest, ReferenceShapeTetrahedronBase)
{
    const ReferenceShapeTetrahedron &rs = ReferenceShapeTetrahedron::getInstance();
    const auto &adjacencies = rs.getAdjacencies();
    ASSERT_EQ(adjacencies.getMaxTopologyDimension(), Topology::Volume);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), NumCornersOnTetrahedron);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Edge), NumEdgesOnTetrahedron);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Face), NumFacesOnTetrahedron);
    ASSERT_TRUE(adjacencies.isComplete());

    // Every node connected to 3 edges and faces?
    const auto *adjacencyC2E = *adjacencies.getAdjacency(Topology::Corner, Topology::Edge);
    const auto *adjacencyC2F = *adjacencies.getAdjacency(Topology::Corner, Topology::Face);
    for (int c = 0; c < NumCornersOnTetrahedron; ++c)
    {
        ASSERT_EQ(adjacencyC2E->getConnectedHighers(c).size(), 3);
        ASSERT_EQ(adjacencyC2F->getConnectedHighers(c).size(), 3);
    }

    // Every edge connected to 2 faces?
    const auto *adjacencyE2F = *adjacencies.getAdjacency(Topology::Edge, Topology::Face);
    for (int e = 0; e < NumEdgesOnTetrahedron; ++e)
    {
        const auto tmp = adjacencyE2F->getConnectedHighers(e);
        ASSERT_EQ(adjacencyE2F->getConnectedHighers(e).size(), 2);
    }
}

TEST(ReferenceShapeTetrahedronTest, TestInterface)
{
    TopologyTest::TestIReferenceShape(ReferenceShapeTetrahedron::getInstance());
}
