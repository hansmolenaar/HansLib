#include <gtest/gtest.h>
#include "TopologicalAdjacencies.h"
#include "TestITopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"

namespace
{
   std::vector<std::unique_ptr<ITopologicalAdjacency>> CreateClawAdjacencyList()
   {
      //     0 - 3 - 1
      //         |
      //         2
      const std::map<int, std::vector<int>> hi2lo{ {0, std::vector<int>{0,3}, },  {1, std::vector<int>{1,3}, },  {2, std::vector<int>{2,3}, } };
      auto adjacency = TopologicalAdjacency::Create(Topology::Edge, 3, Topology::Corner, 4, hi2lo);
      std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacencyList;
      adjacencyList.emplace_back(std::move(adjacency));
      return adjacencyList;
   }
}
TEST(TopologicalAdjacenciesTest, Claw)
{
   const std::vector<int> counts{ 4,3 };
   const auto adjacencies = TopologicalAdjacencies::Create(counts, CreateClawAdjacencyList());
   ASSERT_EQ(adjacencies->getMaxTopologyDimension(), Topology::Edge);
   ASSERT_EQ(adjacencies->getCountSafe(Topology::Corner), 4);
   ASSERT_EQ(adjacencies->getCountSafe(Topology::Edge), 3);
   const auto retval = adjacencies->getAdjacency(Topology::Edge, Topology::Corner);
   ASSERT_TRUE(retval);
   TopologyTest::TestITopologicalAdjacencies(*adjacencies);
}


TEST(TopologicalAdjacenciesTest, ErrorCreate1)
{
   ASSERT_ANY_THROW(TopologicalAdjacencies::Create(std::vector<int>{}, std::vector<std::unique_ptr<ITopologicalAdjacency>>{}));
}


TEST(TopologicalAdjacenciesTest, ErrorCreate2)
{
   const std::vector<int> counts{ 1, 2, 3 };
   ASSERT_ANY_THROW(TopologicalAdjacencies::Create(counts, CreateClawAdjacencyList()));
}


TEST(TopologicalAdjacenciesTest, ErrorCreateWithPartialCounts1)
{
   ASSERT_ANY_THROW(TopologicalAdjacencies::CreateWithPartialCounts(Topology::Volume, std::map<TopologyDimension, int>{}, std::vector<std::unique_ptr<ITopologicalAdjacency>>{}));
}

TEST(TopologicalAdjacenciesTest, ErrorCreateWithPartialCounts2)
{
   std::map<TopologyDimension, int > counts{ {Topology::Volume, 1} };
   ASSERT_ANY_THROW(TopologicalAdjacencies::CreateWithPartialCounts(Topology::Edge, std::map<TopologyDimension, int >{ {Topology::Volume, 1} }, CreateClawAdjacencyList()));
   ASSERT_ANY_THROW(TopologicalAdjacencies::CreateWithPartialCounts(Topology::Edge, std::map<TopologyDimension, int >{ {Topology::Edge, 1} }, CreateClawAdjacencyList()));
   ASSERT_ANY_THROW(TopologicalAdjacencies::CreateWithPartialCounts(Topology::Edge, std::map<TopologyDimension, int >{ {Topology::Edge, 1}, { Topology::Corner, 1 } }, CreateClawAdjacencyList()));
   ASSERT_ANY_THROW(TopologicalAdjacencies::CreateWithPartialCounts(Topology::Edge, std::map<TopologyDimension, int >{ {Topology::Edge, 1}, { Topology::Corner, 0 } }, CreateClawAdjacencyList()));
   ASSERT_ANY_THROW(TopologicalAdjacencies::CreateWithPartialCounts(Topology::Edge, std::map<TopologyDimension, int >{ {Topology::Edge, 3}, { Topology::Corner, -4 } }, CreateClawAdjacencyList()));
}