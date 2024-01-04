#include <gtest/gtest.h>
#include "ReferenceShapePolygon.h"
#include "MyException.h"
#include "TestIReferenceShape.h"
#include "Defines.h"

TEST(ReferenceShapePolygon, Triangle)
{
   const IReferenceShape& triangle = ReferenceShapePolygon::GetTriangle();
   const auto& adjacencies = triangle.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), TopologyDimensionDef::Face);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Corner), ReferenceShapePolygon::TriangleNumCorners);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Edge), ReferenceShapePolygon::TriangleNumEdges);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Face), 1);
   TopologyTest::TestIReferenceShape(triangle);

   const auto e2c = adjacencies.getAdjacency(TopologyDimensionDef::Corner, TopologyDimensionDef::Edge);
   ASSERT_TRUE(e2c.first);
   ASSERT_TRUE(str::equal(e2c.second->getConnectedLowers(0), std::array<int, 2>{0, 1}));
   ASSERT_TRUE(str::equal(e2c.second->getConnectedLowers(1), std::array<int, 2>{1, 2}));
   ASSERT_TRUE(str::equal(e2c.second->getConnectedLowers(2), std::array<int, 2>{2, 0}));

   ASSERT_TRUE(str::equal(e2c.second->getConnectedHighers(0), std::array<int, 2>{0, 2}));
   ASSERT_TRUE(str::equal(e2c.second->getConnectedHighers(1), std::array<int, 2>{0, 1}));
   ASSERT_TRUE(str::equal(e2c.second->getConnectedHighers(2), std::array<int, 2>{1, 2}));
}

TEST(ReferenceShapePolygon, Square)
{
   const IReferenceShape& square = ReferenceShapePolygon::Get(4);
   const auto& adjacencies = square.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), TopologyDimensionDef::Face);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Corner), 4);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Edge), 4);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Face), 1);
   TopologyTest::TestIReferenceShape(square);
}