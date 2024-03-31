#include <gtest/gtest.h>
#include "TopologicalAdjacency.h"
#include "TestITopologicalAdjacency.h"

TEST(TopologicalAdjacencyTest, edge)
{
   const std::map<int, std::vector<int>> adj{ {0, std::vector<int>{1,0}} };
   const auto adjacency = TopologicalAdjacency::Create(Topology::Edge, 1, Topology::Corner, 2, adj);
   ASSERT_EQ(adjacency->getDimensionLow(), Topology::Corner);
   ASSERT_EQ(adjacency->getDimensionHigh(), Topology::Edge);

   auto retval = adjacency->getConnectedLowers(0);
   ASSERT_TRUE(std::ranges::equal(retval, std::vector<int>{ 1, 0 }));
   retval = adjacency->getConnectedHighers(0);
   ASSERT_TRUE(std::ranges::equal(retval, std::vector<int>{ 0 }));
   retval = adjacency->getConnectedHighers(1);
   ASSERT_TRUE(std::ranges::equal(retval, std::vector<int>{ 0 }));
}


TEST(TopologicalAdjacencyTest, test_interface)
{
   const std::map<int, std::vector<int>> adj{ {0, std::vector<int>{1,0}} };
   const auto adjacency = TopologicalAdjacency::Create(Topology::Edge, 1, Topology::Corner, 2, adj);
   TopologyTest::TestITopologicalAdjacency(*adjacency, 2, 1);
}

TEST(TopologicalAdjacencyTest, error1)
{
   ASSERT_ANY_THROW(TopologicalAdjacency::Create(Topology::Edge, 1, Topology::Corner, 1, std::map<int, std::vector<int>> { {0, std::vector<int>{1, 0}} }));
}

TEST(TopologicalAdjacencyTest, error2)
{
   ASSERT_ANY_THROW(TopologicalAdjacency::Create(Topology::Edge, 0, Topology::Corner, 2, std::map<int, std::vector<int>> { {0, std::vector<int>{1, 0}} }));
}


TEST(TopologicalAdjacencyTest, error3)
{
   const std::map<int, std::vector<int>> adj{ {0, std::vector<int>{1,0}} };
   ASSERT_ANY_THROW(TopologicalAdjacency::Create(Topology::Corner, 1, Topology::Corner, 1, std::map<int, std::vector<int>> { {0, std::vector<int>{1, 0}} }));
}


TEST(TopologicalAdjacencyTest, error4)
{
   ASSERT_ANY_THROW(TopologicalAdjacency::Create(Topology::Edge, 1, Topology::Corner, 2, std::map<int, std::vector<int>>{ {0, std::vector<int>{1, 1}} }));
}

TEST(TopologicalAdjacencyTest, error5)
{
   ASSERT_ANY_THROW(TopologicalAdjacency::Create(Topology::Edge, 1, Topology::Corner, 2, std::map<int, std::vector<int>>{ {1, std::vector<int>{0, 1}} }));
}

TEST(TopologicalAdjacencyTest, error6)
{
   ASSERT_ANY_THROW(TopologicalAdjacency::Create(Topology::Edge, 1, Topology::Corner, 2, std::map<int, std::vector<int>>{ {0, std::vector<int>{0, 2}} }));
}