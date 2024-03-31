#pragma once

#include "ITopologicalAdjacencies.h"
#include "BoundsCheck.h"

#include <memory>
#include <map>
#include <array>
#include <vector>

class TopologicalAdjacencies : public ITopologicalAdjacencies
{
public:
   static std::unique_ptr<TopologicalAdjacencies> Create(const std::vector<int>&, std::vector<std::unique_ptr<ITopologicalAdjacency>>&&);
   static std::unique_ptr<TopologicalAdjacencies> CreateWithPartialCounts(Topology::TopologyDimension, std::map<Topology::TopologyDimension, int>&&, std::vector<std::unique_ptr<ITopologicalAdjacency>>&&);

   Topology::TopologyDimension getMaxTopologyDimension() const override;
   std::optional<int> getCount(Topology::TopologyDimension dim) const override;
   std::optional<const ITopologicalAdjacency*> getAdjacency(Topology::TopologyDimension hi, Topology::TopologyDimension lo) const override;

private:
   using AdjacencyMap = std::map <std::pair<Topology::TopologyDimension, Topology::TopologyDimension>, std::unique_ptr<ITopologicalAdjacency>>;
   TopologicalAdjacencies(const std::array<int, 2>& ,std::unique_ptr<ITopologicalAdjacency>&&);
   TopologicalAdjacencies(std::map<Topology::TopologyDimension, int>&& count, AdjacencyMap&& adjacencies);

   AdjacencyMap m_adjecencies;
   std::map<Topology::TopologyDimension, int> m_count;
   BoundsCheck<Topology::TopologyDimension> m_checkDimension;
};
