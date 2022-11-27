#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"
#include "Utilities/BoundsCheck.h"

#include <algorithm>

namespace
{
   const auto isPositive = BoundsCheck<int>::CreateLowerBound(1);

   void CheckEdge2Corner(const ITopologicalAdjacency& e2c)
   {
      if (e2c.getDimensionLow() != TopologyDimension::Corner) throw MyException("CheckEdge2Corner low dime");
      if (e2c.getDimensionHigh() != TopologyDimension::Edge) throw MyException("CheckEdge2Corner high dime");
   }
}

TopologicalAdjacencies::TopologicalAdjacencies(const std::array<int, 2>& count, std::unique_ptr<ITopologicalAdjacency>&& edge_2_corner) :
   m_count{ {TopologyDimension::Corner, std::get<0>(count)} , {TopologyDimension::Edge, std::get<1>(count)} }
{
   m_adjecencies.emplace(std::make_pair(TopologyDimension::Corner, TopologyDimension::Edge), edge_2_corner);
}

std::unique_ptr<TopologicalAdjacencies> TopologicalAdjacencies::Create(const std::array<int, 2>& count, std::unique_ptr<ITopologicalAdjacency>&& adj)
{
   for (auto dim : count) isPositive(dim);
   CheckEdge2Corner(*adj);
   return std::unique_ptr<TopologicalAdjacencies>(new TopologicalAdjacencies(count, std::move(adj)));
}

TopologyDimension TopologicalAdjacencies::getMaxTopologyDimension() const
{
   return m_count.rbegin()->first;
}

int TopologicalAdjacencies::getCount(TopologyDimension dim) const
{
   return m_count.at(dim);
}

const ITopologicalAdjacency& TopologicalAdjacencies::getAdjacency(TopologyDimension dim1, TopologyDimension dim2) const
{
   const auto pair = std::make_pair(std::min(dim1, dim2), std::max(dim1, dim2));
   return *m_adjecencies.at(pair);
}