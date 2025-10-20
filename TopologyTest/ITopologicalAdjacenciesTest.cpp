#include "IReferenceShape.h"
#include "MyException.h"
#include "ReferenceShapeHyperCube.h"
#include "ReferenceShapeNode.h"
#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"
#include <gtest/gtest.h>

using namespace Topology;
TEST(ITopologicalAdjacenciesTest, CountSafe)
{
    const ReferenceShapeNode rs;
    ASSERT_EQ(rs.getAdjacencies().getCountSafe(Topology::Corner), 1);
    ASSERT_ANY_THROW(rs.getAdjacencies().getCountSafe(Topology::Volume));
}

TEST(ITopologicalAdjacenciesTest, CountSafeError)
{
    auto adjacency = TopologicalAdjacency::CreateTrivial(Topology::Face, Topology::Corner, 3);
    std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacencyList;
    adjacencyList.emplace_back(std::move(adjacency));
    std::map<TopologyDimension, int> counts{{Topology::Corner, 3}, {Topology::Face, 1}};
    const auto triangle =
        TopologicalAdjacencies::CreateWithPartialCounts(Topology::Face, std::move(counts), std::move(adjacencyList));
    ASSERT_ANY_THROW(triangle->getCountSafe(Topology::Edge));
}

TEST(ITopologicalAdjacenciesTest, NotComplete)
{
    auto adjacency = TopologicalAdjacency::CreateTrivial(Topology::Face, Topology::Corner, 3);
    std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacencyList;
    adjacencyList.emplace_back(std::move(adjacency));
    std::map<TopologyDimension, int> counts{{Topology::Corner, 3}, {Topology::Face, 1}};
    const auto triangle =
        TopologicalAdjacencies::CreateWithPartialCounts(Topology::Face, std::move(counts), std::move(adjacencyList));
    ASSERT_FALSE(triangle->isComplete());
}

TEST(ITopologicalAdjacenciesTest, Complete)
{
    const auto &edge = ReferenceShapeHyperCube::Get(Topology::Edge);
    ASSERT_TRUE(edge.getAdjacencies().isComplete());
}
