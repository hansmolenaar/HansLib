#include <gtest/gtest.h>

#include "Mesh/TartanGrid.h"


TEST(TartanGridTest, 1D)
{
   auto coordinates = std::vector<std::vector<double>>{ std::vector<double > {2, 3, 4 } };
   const TartanGrid<double,1> grid(std::move(coordinates));
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
