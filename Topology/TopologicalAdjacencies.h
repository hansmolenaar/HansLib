#pragma once

#include "ITopologicalAdjacencies.h"

#include <memory>
#include <map>
#include <array>

class TopologicalAdjacencies : public ITopologicalAdjacencies
{
public:
   static std::unique_ptr<TopologicalAdjacencies> Create(const std::array<int,2>&, std::unique_ptr<ITopologicalAdjacency>&&);

   TopologyDimension getMaxTopologyDimension() const override;
   int getCount(TopologyDimension dim) const override;
   const ITopologicalAdjacency& getAdjacency(TopologyDimension, TopologyDimension) const override;

private:
   TopologicalAdjacencies(const std::array<int, 2>& ,std::unique_ptr<ITopologicalAdjacency>&&);

   std::map < std::pair<TopologyDimension, TopologyDimension>, std::unique_ptr<ITopologicalAdjacency>> m_adjecencies;
   std::map<TopologyDimension, int> m_count;
};
