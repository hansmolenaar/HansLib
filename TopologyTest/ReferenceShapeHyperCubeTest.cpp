#include <gtest/gtest.h>

#include "Topology/ReferenceShapeHyperCube.h"
#include "TestIReferenceShape.h"

TEST(ReferenceShapeHyperCubeTest, Edge)
{
   const auto& edge = ReferenceShapeHyperCube::Get(TopologyDimensionDef::Edge);
   const auto& adjacencies = edge.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), TopologyDimensionDef::Edge);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Corner), 2);
   TopologyTest::TestIReferenceShape(edge);
}


TEST(ReferenceShapeHyperCubeTest, Square)
{
   const auto& square = ReferenceShapeHyperCube::Get(TopologyDimensionDef::Face);
   const auto& adjacencies = square.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), TopologyDimensionDef::Face);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Corner), 4);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Edge), 4);
   TopologyTest::TestIReferenceShape(square);
}


TEST(ReferenceShapeHyperCubeTest, Cube)
{
   const auto& cube = ReferenceShapeHyperCube::Get(TopologyDimensionDef::Volume);
   const auto& adjacencies = cube.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), TopologyDimensionDef::Volume);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Corner), 8);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Edge), 12);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Face), 6);
   TopologyTest::TestIReferenceShape(cube);
}


TEST(ReferenceShapeHyperCubeTest, Tesseract)
{
   const auto& tesseract = ReferenceShapeHyperCube::Get(4);
   const auto& adjacencies = tesseract.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), 4);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Corner), 16);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Edge), 32);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Face), 24);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Volume), 8);
   TopologyTest::TestIReferenceShape(tesseract);
}


TEST(ReferenceShapeHyperCubeTest, Penteract)
{
   const auto& penteract = ReferenceShapeHyperCube::Get(5);
   const auto& adjacencies = penteract.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), 5);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Corner), 32);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Edge), 80);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Face), 80);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Volume), 40);
   ASSERT_EQ(adjacencies.getCountSafe(4), 10);
   TopologyTest::TestIReferenceShape(penteract);
}