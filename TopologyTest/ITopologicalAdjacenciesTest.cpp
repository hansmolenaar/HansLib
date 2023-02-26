#include <gtest/gtest.h>
#include "Topology/ReferenceShapeNode.h"
#include "MyException.h"
#include "Topology/TopologicalAdjacencies.h"
#include "Topology/TopologicalAdjacency.h"
#include "Topology/ReferenceShapeHyperCube.h"
#include "Topology/IReferenceShape.h"

TEST(ITopologicalAdjacenciesTest, CountSafe)
{
   const ReferenceShapeNode rs;
   ASSERT_EQ(rs.getAdjacencies().getCountSafe(TopologyDimensionDef::Corner), 1);
   ASSERT_ANY_THROW(rs.getAdjacencies().getCountSafe(TopologyDimensionDef::Volume));
}

TEST(ITopologicalAdjacenciesTest, CountSafeError)
{
   auto adjacency = TopologicalAdjacency::CreateTrivial(TopologyDimensionDef::Face, TopologyDimensionDef::Corner, 3);
   std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacencyList;
   adjacencyList.emplace_back(std::move(adjacency));
   std::map<TopologyDimension, int> counts{ {TopologyDimensionDef::Corner, 3}, {TopologyDimensionDef::Face, 1 } };
   const auto triangle = TopologicalAdjacencies::CreateWithPartialCounts(TopologyDimensionDef::Face, std::move(counts), std::move(adjacencyList));
   ASSERT_ANY_THROW(triangle->getCountSafe(TopologyDimensionDef::Edge));
}

TEST(ITopologicalAdjacenciesTest, NotComplete)
{
   auto adjacency = TopologicalAdjacency::CreateTrivial(TopologyDimensionDef::Face, TopologyDimensionDef::Corner, 3);
   std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacencyList;
   adjacencyList.emplace_back(std::move(adjacency));
   std::map<TopologyDimension, int> counts{ {TopologyDimensionDef::Corner, 3}, {TopologyDimensionDef::Face, 1 } };
   const auto triangle = TopologicalAdjacencies::CreateWithPartialCounts(TopologyDimensionDef::Face, std::move(counts), std::move(adjacencyList));
   ASSERT_FALSE(triangle->isComplete());
}

TEST(ITopologicalAdjacenciesTest, Complete)
{
   const auto& edge = ReferenceShapeHyperCube::Get(TopologyDimensionDef::Edge);
   ASSERT_TRUE(edge.getAdjacencies().isComplete());
}

