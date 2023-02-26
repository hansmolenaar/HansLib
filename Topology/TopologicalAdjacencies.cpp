#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"
#include "BoundsCheck.h"
#include "Topology/TopologyDimension.h"

#include <algorithm>
#include <set>

namespace
{
   static const auto IsPositive = BoundsCheck<int>::CreateIsPositive();

   void CheckEdge2Corner(const ITopologicalAdjacency& e2c)
   {
      if (e2c.getDimensionLow() != TopologyDimensionDef::Corner) throw MyException("CheckEdge2Corner low dime");
      if (e2c.getDimensionHigh() != TopologyDimensionDef::Edge) throw MyException("CheckEdge2Corner high dime");
   }

   void CheckInputAdjacency(TopologyDimension maxDim, const std::map<TopologyDimension, int>& count, const ITopologicalAdjacency& adjacency)
   {
      BoundsCheck<TopologyDimension>::CreateUpperBound(maxDim)(adjacency.getDimensionHigh());
      const int countLo = count.at(adjacency.getDimensionLow());
      const int countHi = count.at(adjacency.getDimensionHigh());
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
   Utilities::MyAssert(!count.empty());
   std::map<TopologyDimension, int> countMap;

   TopologyDimension tdim = TopologyDimensionDef::Corner;
   for (auto c : count)
   {
      IsPositive(c);
      countMap.emplace(tdim, c);
      ++tdim;
   }
   const TopologyDimension maxDimension = static_cast<TopologyDimension>(count.size() - 1);
   return CreateWithPartialCounts(maxDimension, std::move(countMap), std::move(adjacencies));
}

std::unique_ptr<TopologicalAdjacencies> TopologicalAdjacencies::CreateWithPartialCounts(
   const TopologyDimension maxDim,
   std::map<TopologyDimension, int>&& countMap,
   std::vector<std::unique_ptr<ITopologicalAdjacency>>&& adjacencies)
{
   // Want to know something
   Utilities::MyAssert(!countMap.empty());
   Utilities::MyAssert(countMap.begin()->first == TopologyDimensionDef::Corner);
   Utilities::MyAssert(countMap.rbegin()->first == maxDim);
   for (auto& itr : countMap)
   {
      IsPositive(itr.second);
   }

   AdjacencyMap adjacencyMap;
   for (auto& adj : adjacencies)
   {
      const auto pair = std::make_pair(adj->getDimensionLow(), adj->getDimensionHigh());
      Utilities::MyAssert(!adjacencyMap.contains(pair));
      CheckInputAdjacency(maxDim, countMap, *adj);
      adjacencyMap.emplace(pair, std::move(adj));
   }
   return std::unique_ptr<TopologicalAdjacencies>(new TopologicalAdjacencies(std::move(countMap), std::move(adjacencyMap)));
}

TopologyDimension TopologicalAdjacencies::getMaxTopologyDimension() const
{
   return m_count.rbegin()->first;
}

std::pair<bool, int> TopologicalAdjacencies::getCount(TopologyDimension dim) const
{
   if (!m_count.contains(dim)) return { false, -1 };
   return { true, m_count.at(dim) };
}

std::pair<bool, const ITopologicalAdjacency*> TopologicalAdjacencies::getAdjacency(TopologyDimension dim1, TopologyDimension dim2) const
{
   Utilities::MyAssert(dim1 != dim2);
   m_checkDimension.check(std::max(dim1, dim2));
   const auto pair = std::make_pair(std::min(dim1, dim2), std::max(dim1, dim2));
   std::pair<bool, const ITopologicalAdjacency*> result = std::make_pair(m_adjecencies.contains(pair), nullptr);
   if (result.first)
   {
      result.second = m_adjecencies.at(pair).get();
   }
   return result;
}