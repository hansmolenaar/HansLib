#include <gtest/gtest.h>

#include "ReferenceShapeHyperCube.h"
#include "TestIReferenceShape.h"

TEST(ReferenceShapeHyperCubeTest, Edge)
{
    const auto &edge = ReferenceShapeHyperCube::Get(Topology::Edge);
    const auto &adjacencies = edge.getAdjacencies();
    ASSERT_EQ(adjacencies.getMaxTopologyDimension(), Topology::Edge);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), 2);
    ASSERT_TRUE(adjacencies.isComplete());
    TopologyTest::TestIReferenceShape(edge);
}

TEST(ReferenceShapeHyperCubeTest, Square)
{
    const auto &square = ReferenceShapeHyperCube::Get(Topology::Face);
    const auto &adjacencies = square.getAdjacencies();
    ASSERT_EQ(adjacencies.getMaxTopologyDimension(), Topology::Face);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), 4);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Edge), 4);
    ASSERT_TRUE(adjacencies.isComplete());
    TopologyTest::TestIReferenceShape(square);
}

TEST(ReferenceShapeHyperCubeTest, Cube)
{
    const auto &cube = ReferenceShapeHyperCube::Get(Topology::Volume);
    const auto &adjacencies = cube.getAdjacencies();
    ASSERT_EQ(adjacencies.getMaxTopologyDimension(), Topology::Volume);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), 8);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Edge), 12);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Face), 6);
    ASSERT_TRUE(adjacencies.isComplete());
    TopologyTest::TestIReferenceShape(cube);
}

TEST(ReferenceShapeHyperCubeTest, Tesseract)
{
    const auto &tesseract = ReferenceShapeHyperCube::Get(4);
    const auto &adjacencies = tesseract.getAdjacencies();
    ASSERT_EQ(adjacencies.getMaxTopologyDimension(), 4);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), 16);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Edge), 32);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Face), 24);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Volume), 8);
    ASSERT_FALSE(adjacencies.isComplete());
    TopologyTest::TestIReferenceShape(tesseract);
}

TEST(ReferenceShapeHyperCubeTest, Penteract)
{
    const auto &penteract = ReferenceShapeHyperCube::Get(5);
    const auto &adjacencies = penteract.getAdjacencies();
    ASSERT_EQ(adjacencies.getMaxTopologyDimension(), 5);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), 32);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Edge), 80);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Face), 80);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Volume), 40);
    ASSERT_EQ(adjacencies.getCountSafe(4), 10);
    TopologyTest::TestIReferenceShape(penteract);
}