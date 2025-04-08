#pragma once

#include "ITopologicalAdjacency.h"

#include <map>
#include <memory>
#include <vector>

namespace Topology
{
   class TopologicalAdjacency : public ITopologicalAdjacency
   {
   public:
      static std::unique_ptr<TopologicalAdjacency> Create(TopologyDimension, int, TopologyDimension, int, const std::map<int, std::vector<int>>&);
      static std::unique_ptr<TopologicalAdjacency> CreateTrivial(TopologyDimension, TopologyDimension, int);

      TopologyDimension getDimensionLow() const override;
      TopologyDimension getDimensionHigh() const override;
      const std::vector<int>& getConnectedLowers(int) const override;
      const std::vector<int>& getConnectedHighers(int) const override;

   private:
      TopologicalAdjacency(TopologyDimension, std::map<int, std::vector<int>>&&, TopologyDimension, const std::map<int, std::vector<int>>&&);
      TopologyDimension m_dimLow;
      TopologyDimension m_dimHigh;
      std::map<int, std::vector<int>> m_low_2_high;
      std::map<int, std::vector<int>> m_high_2_low;
   };
}