#include "GridTopology.h"
#include "MyException.h"
#include "TestITopologicalAdjacencies.h"
#include <gtest/gtest.h>

TEST(GridTopologyTest, OneD2cells)
{
    const GridTopology grid(std::vector<int>{2});
    ASSERT_EQ(grid.getShape().getAdjacencies().getMaxTopologyDimension(), Topology::Edge);
    TopologyTest::TestITopologicalAdjacencies(grid.getAdjacencies());
    const auto &adjacencies = grid.getAdjacencies();
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), 3);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Edge), 2);
    const auto found = adjacencies.getAdjacency(Topology::Edge, Topology::Corner);
    const auto &adjacency = **found;
    auto corners = adjacency.getConnectedLowers(0);
    ASSERT_TRUE(std::ranges::equal(corners, std::vector<int>{0, 1}));
    corners = adjacency.getConnectedLowers(1);
    ASSERT_TRUE(std::ranges::equal(corners, std::vector<int>{1, 2}));
}

TEST(GridTopologyTest, TwoD6cells)
{
    const GridTopology grid(std::vector<int>{2, 3});
    ASSERT_EQ(grid.getShape().getAdjacencies().getMaxTopologyDimension(), Topology::Face);
    TopologyTest::TestITopologicalAdjacencies(grid.getAdjacencies());
    const auto &adjacencies = grid.getAdjacencies();
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), 12);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Face), 6);
    const auto found = adjacencies.getAdjacency(Topology::Face, Topology::Corner);
    const auto &adjacency = **found;
    auto corners = adjacency.getConnectedLowers(0);
    ASSERT_TRUE(std::ranges::equal(corners, std::vector<int>{0, 1, 3, 4}));
    corners = adjacency.getConnectedLowers(1);
    ASSERT_TRUE(std::ranges::equal(corners, std::vector<int>{1, 2, 4, 5}));
}

TEST(GridTopologyTest, ThreeD2cells)
{
    const GridTopology grid(std::vector<int>{1, 1, 2});
    ASSERT_EQ(grid.getShape().getAdjacencies().getMaxTopologyDimension(), Topology::Volume);
    TopologyTest::TestITopologicalAdjacencies(grid.getAdjacencies());
    const auto &adjacencies = grid.getAdjacencies();
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), 12);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Volume), 2);
    const auto found = adjacencies.getAdjacency(Topology::Volume, Topology::Corner);
    const auto &adjacency = **found;
    auto corners = adjacency.getConnectedLowers(0);
    ASSERT_TRUE(std::ranges::equal(corners, std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7}));
    corners = adjacency.getConnectedLowers(1);
    ASSERT_TRUE(std::ranges::equal(corners, std::vector<int>{4, 5, 6, 7, 8, 9, 10, 11}));
}
