#pragma once

#include "ITopologicalAdjacency.h"

#include <map>
#include <vector>
#include <memory>

class TopologicalAdjacency : public ITopologicalAdjacency
{
public:
   static std::unique_ptr<TopologicalAdjacency> Create(Topology::TopologyDimension, int, Topology::TopologyDimension, int, const std::map<int, std::vector<int>>&);
   static std::unique_ptr<TopologicalAdjacency> CreateTrivial(Topology::TopologyDimension, Topology::TopologyDimension, int);

   Topology::TopologyDimension getDimensionLow() const override;
   Topology::TopologyDimension getDimensionHigh() const override;
   const std::vector<int>& getConnectedLowers(int) const override;
   const std::vector<int>& getConnectedHighers(int) const override;

private:
   TopologicalAdjacency(Topology::TopologyDimension, std::map<int, std::vector<int>>&&, Topology::TopologyDimension, const std::map<int, std::vector<int>>&&);
   Topology::TopologyDimension m_dimLow;
   Topology::TopologyDimension m_dimHigh;
   std::map<int, std::vector<int>> m_low_2_high;
   std::map<int, std::vector<int>> m_high_2_low;
};