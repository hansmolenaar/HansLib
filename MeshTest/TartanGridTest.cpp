#include <gtest/gtest.h>

#include "Mesh/TartanGrid.h"


TEST(TartanGridTest, 1D)
{
   auto coordinates = std::vector<std::vector<double>>{ std::vector<double > {2, 3, 4 } };
   const TartanGrid<double, 1> grid(std::move(coordinates));
   ASSERT_EQ(grid.getNumPoints(), 3);
   ASSERT_EQ(grid.getPointIndexer().getFlatSize(), 3);
   ASSERT_EQ(grid.getTopology().getMaxTopologyDimension(), TopologyDimensionDef::Edge);
   ASSERT_EQ(grid.getTopology().getCountSafe(TopologyDimensionDef::Edge), 2);
   ASSERT_EQ(grid.getTopology().getCountSafe(TopologyDimensionDef::Corner), 3);
   ASSERT_EQ(grid.getCellIndexer().getFlatSize(), 2);
}


TEST(TartanGridTest, 2D)
{
   auto coordinates = std::vector<std::vector<double>>{ std::vector<double> {2, 3, 4, 5 }, std::vector<double> {3, 4, 5 } };
   const TartanGrid<double, 2> grid(std::move(coordinates));
   ASSERT_EQ(grid.getPointIndexer().getFlatSize(), 12);
   ASSERT_EQ(grid.getTopology().getMaxTopologyDimension(), TopologyDimensionDef::Face);
   ASSERT_EQ(grid.getTopology().getCountSafe(TopologyDimensionDef::Face), 6);
   ASSERT_EQ(grid.getTopology().getCountSafe(TopologyDimensionDef::Corner), 12);
   ASSERT_EQ(grid.getCellIndexer().getFlatSize(), 6);
   ASSERT_EQ(grid.getPointIndexer().getFlatSize(), 12);
}


TEST(TartanGridTest, locate1D)
{
   auto coordinates = std::vector<std::vector<int>>{ std::vector<int> {0, 1, 3, 5 } };
   const TartanGrid<int, 1> grid(std::move(coordinates));
   ASSERT_EQ(grid.locatePointInCell(IntPoint1{ 0 }), 0);
   ASSERT_EQ(grid.locatePointInCell(IntPoint1{ 1 }), 0);
   ASSERT_EQ(grid.locatePointInCell(IntPoint1{ 2 }), 1);
   ASSERT_EQ(grid.locatePointInCell(IntPoint1{ 3 }), 1);
   ASSERT_EQ(grid.locatePointInCell(IntPoint1{ 4 }), 2);
   ASSERT_EQ(grid.locatePointInCell(IntPoint1{ 5 }), 2);

   ASSERT_EQ(grid.locatePointInCell(IntPoint1{ -1 }), PointIndexInvalid);
   ASSERT_EQ(grid.locatePointInCell(IntPoint1{ 6 }), PointIndexInvalid);
}


TEST(TartanGridTest, locate2D)
{
   auto coordinates = std::vector<std::vector<int>>{ std::vector<int> {0, 1, 3, 5 }, std::vector<int>{1,3,9} };
   const TartanGrid<int, 2> grid(std::move(coordinates));
   ASSERT_EQ(grid.locatePointInCell(IntPoint2{ 0,1 }), 0);
   ASSERT_EQ(grid.locatePointInCell(IntPoint2{ 0,3 }), 0);
   ASSERT_EQ(grid.locatePointInCell(IntPoint2{ 4, 8 }), 5);
   ASSERT_EQ(grid.locatePointInCell(IntPoint2{ 5, 9 }), 5);
   ASSERT_EQ(grid.locatePointInCell(IntPoint2{ 2, 2 }), 1);
   ASSERT_EQ(grid.locatePointInCell(IntPoint2{ 0, 5 }), 3);
   ASSERT_EQ(grid.locatePointInCell(IntPoint2{ 2, 5 }), 4);
}
