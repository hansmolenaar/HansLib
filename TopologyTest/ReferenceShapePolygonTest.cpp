#include "Defines.h"
#include "MyException.h"
#include "ReferenceShapePolygon.h"
#include "TestIReferenceShape.h"
#include <gtest/gtest.h>

TEST(ReferenceShapePolygon, Triangle)
{
   const IReferenceShape& triangle = ReferenceShapePolygon::GetTriangle();
   const auto& adjacencies = triangle.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), Topology::Face);
   ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), ReferenceShapePolygon::TriangleNumCorners);
   ASSERT_EQ(adjacencies.getCountSafe(Topology::Edge), ReferenceShapePolygon::TriangleNumEdges);
   ASSERT_EQ(adjacencies.getCountSafe(Topology::Face), 1);
   TopologyTest::TestIReferenceShape(triangle);

   const auto e2c = adjacencies.getAdjacency(Topology::Corner, Topology::Edge);
   ASSERT_TRUE(str::equal(e2c.value()->getConnectedLowers(0), std::array<int, 2>{0, 1}));
   ASSERT_TRUE(str::equal(e2c.value()->getConnectedLowers(1), std::array<int, 2>{1, 2}));
   ASSERT_TRUE(str::equal(e2c.value()->getConnectedLowers(2), std::array<int, 2>{2, 0}));

   ASSERT_TRUE(str::equal(e2c.value()->getConnectedHighers(0), std::array<int, 2>{0, 2}));
   ASSERT_TRUE(str::equal(e2c.value()->getConnectedHighers(1), std::array<int, 2>{0, 1}));
   ASSERT_TRUE(str::equal(e2c.value()->getConnectedHighers(2), std::array<int, 2>{1, 2}));
}

TEST(ReferenceShapePolygon, Square)
{
   const IReferenceShape& square = ReferenceShapePolygon::Get(4);
   const auto& adjacencies = square.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), Topology::Face);
   ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), 4);
   ASSERT_EQ(adjacencies.getCountSafe(Topology::Edge), 4);
   ASSERT_EQ(adjacencies.getCountSafe(Topology::Face), 1);
   TopologyTest::TestIReferenceShape(square);
}