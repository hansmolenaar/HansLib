#include <gtest/gtest.h>

#include "Mesh/TartanGrid.h"


TEST(TartanGridTest, 1D)
{
   auto coordinates = std::vector<std::vector<double>>{ std::vector<double > {2, 3, 4 } };
   const TartanGrid<double,1> grid(std::move(coordinates));
   ASSERT_EQ(grid.getNumPoints(), 3);
}
