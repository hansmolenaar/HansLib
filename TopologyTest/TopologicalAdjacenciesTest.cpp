#include <gtest/gtest.h>
#include "Topology/TopologicalAdjacencies.h"
#include "TestITopologicalAdjacencies.h"
#include "Topology/TopologicalAdjacency.h"

TEST(TopologicalAdjacenciesTest, Claw)
{
   //     0 - 3 - 1
   //         |
   //         2
   const std::vector<int> counts{ 4,3 };
   const std::map<int, std::vector<int>> hi2lo{ {0, std::vector<int>{0,3}, },  {1, std::vector<int>{1,3}, },  {2, std::vector<int>{2,3}, } };
   auto adjacency = TopologicalAdjacency::Create(TopologyDimensionDef::Edge, 3, TopologyDimensionDef::Corner, 4, hi2lo);
   std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacencyList;
   adjacencyList.emplace_back(std::move(adjacency));
   const auto adjacencies = TopologicalAdjacencies::Create(counts, std::move(adjacencyList));
   ASSERT_EQ(adjacencies->getMaxTopologyDimension(), TopologyDimensionDef::Edge);
   ASSERT_EQ(adjacencies->getCountSafe(TopologyDimensionDef::Corner), 4);
   ASSERT_EQ(adjacencies->getCountSafe(TopologyDimensionDef::Edge), 3);
   const auto retval = adjacencies->getAdjacency(TopologyDimensionDef::Edge, TopologyDimensionDef::Corner);
   ASSERT_TRUE(retval.first);

   TopologyTest::TestITopologicalAdjacencies(*adjacencies);
}