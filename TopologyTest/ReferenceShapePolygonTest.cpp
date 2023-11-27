#include <gtest/gtest.h>
#include "ReferenceShapePolygon.h"
#include "MyException.h"
#include "TestIReferenceShape.h"

TEST(ReferenceShapePolygon, Triangle)
{
   const IReferenceShape& triangle = ReferenceShapePolygon::GetTriangle();
   const auto& adjacencies = triangle.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), TopologyDimensionDef::Face);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Corner), 3);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Edge), 3);
   ASSERT_EQ(adjacencies.getCountSafe(TopologyDimensionDef::Face), 1);
   TopologyTest::TestIReferenceShape(triangle);
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