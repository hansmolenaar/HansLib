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
   static std::unique_ptr<TopologicalAdjacencies> CreateWithPartialCounts(TopologyDimension, std::map<TopologyDimension, int>&&, std::vector<std::unique_ptr<ITopologicalAdjacency>>&&);

   TopologyDimension getMaxTopologyDimension() const override;
   std::optional<int> getCount(TopologyDimension dim) const override;
   std::optional<const ITopologicalAdjacency*> getAdjacency(TopologyDimension hi, TopologyDimension lo) const override;

private:
   using AdjacencyMap = std::map <std::pair<TopologyDimension, TopologyDimension>, std::unique_ptr<ITopologicalAdjacency>>;
   TopologicalAdjacencies(const std::array<int, 2>& ,std::unique_ptr<ITopologicalAdjacency>&&);
   TopologicalAdjacencies(std::map<TopologyDimension, int>&& count, AdjacencyMap&& adjacencies);

   AdjacencyMap m_adjecencies;
   std::map<TopologyDimension, int> m_count;
   BoundsCheck<TopologyDimension> m_checkDimension;
};
