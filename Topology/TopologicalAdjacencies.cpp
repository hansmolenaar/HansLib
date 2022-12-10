#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"
#include "Utilities/BoundsCheck.h"
#include "Topology/TopologyDimension.h"

#include <algorithm>
#include <set>

namespace
{
   const auto isPositive = BoundsCheck<int>::CreateLowerBound(1);

   void CheckEdge2Corner(const ITopologicalAdjacency& e2c)
   {
      if (e2c.getDimensionLow() != TopologyDimensionDef::Corner) throw MyException("CheckEdge2Corner low dime");
      if (e2c.getDimensionHigh() != TopologyDimensionDef::Edge) throw MyException("CheckEdge2Corner high dime");
   }

   void CheckInputAdjacency(TopologyDimension maxDim, const std::vector<int>& count, const ITopologicalAdjacency& adjacency)
   {
      BoundsCheck<TopologyDimension>::CreateUpperBound(maxDim)(adjacency.getDimensionHigh());
      const int countLo = count.at(static_cast<size_t>(adjacency.getDimensionLow()));
      const int countHi = count.at(static_cast<size_t>(adjacency.getDimensionHigh()));
      const auto checkLo = BoundsCheck<int>::Create(0, countLo - 1);
      const auto checkHi = BoundsCheck<int>::Create(0, countHi - 1);
      for (int lo = 0; lo < countLo; ++lo)
      {
         for (auto hi : adjacency.getConnectedHighers(lo))
         {
            checkHi.check(hi);
         }
      }
      for (int hi = 0; hi < countHi; ++hi)
      {
         for (auto lo : adjacency.getConnectedLowers(hi))
         {
            checkLo.check(lo);
         }
      }
   }
}

TopologicalAdjacencies::TopologicalAdjacencies(std::map<TopologyDimension, int>&& count, AdjacencyMap&& adjacencies) :
   m_count(std::move(count)), m_adjecencies(std::move(adjacencies)), m_checkDimension(BoundsCheck<TopologyDimension>::CreateUpperBound(getMaxTopologyDimension()))
{
}

std::unique_ptr<TopologicalAdjacencies> TopologicalAdjacencies::Create(const std::vector<int>& count, std::vector<std::unique_ptr<ITopologicalAdjacency>>&& adjacencies)
{
   std::map<TopologyDimension, int> countMap;

   TopologyDimension tdim = TopologyDimensionDef::Corner;
   for (auto c : count)
   {
      isPositive(c);
      countMap.emplace(tdim, c);
      ++tdim;
   }

   const TopologyDimension maxDimension = static_cast<TopologyDimension>(count.size());
   AdjacencyMap adjacencyMap;
   for (auto& adj : adjacencies)
   {
      const auto pair = std::make_pair(adj->getDimensionLow(), adj->getDimensionHigh());
      Utilities::Assert(!adjacencyMap.contains(pair));
      CheckInputAdjacency(maxDimension, count, *adj);
      adjacencyMap.emplace(pair, std::move(adj));
   }
   return std::unique_ptr<TopologicalAdjacencies>(new TopologicalAdjacencies(std::move(countMap), std::move(adjacencyMap)));
}

TopologyDimension TopologicalAdjacencies::getMaxTopologyDimension() const
{
   return m_count.rbegin()->first;
}

int TopologicalAdjacencies::getCount(TopologyDimension dim) const
{
   return m_count.at(dim);
}

std::pair<bool, const ITopologicalAdjacency*> TopologicalAdjacencies::getAdjacency(TopologyDimension dim1, TopologyDimension dim2) const
{
   Utilities::Assert(dim1 != dim2);
   const auto pair = std::make_pair(std::min(dim1, dim2), std::max(dim1, dim2));
   std::pair<bool, const ITopologicalAdjacency*> result = std::make_pair(m_adjecencies.contains(pair), nullptr);
   if (result.first)
   {
      result.second = m_adjecencies.at(pair).get();
   }
   return result;
}