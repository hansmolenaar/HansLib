#include "TopologicalAdjacency.h"

#include <algorithm>

TopologicalAdjacency::TopologicalAdjacency(TopologyDimension dimLow, std::map<int, std::vector<int>>&& low2high, TopologyDimension dimHigh, const std::map<int, std::vector<int>>&& high2low) :
   m_dimLow(dimLow),
   m_dimHigh(dimHigh),
   m_low_2_high(low2high),
   m_high_2_low(high2low)
{
}

TopologyDimension TopologicalAdjacency::getDimensionLow() const
{
   return m_dimLow;
}

TopologyDimension TopologicalAdjacency::getDimensionHigh() const
{
   return m_dimHigh;
}

const std::vector<int> TopologicalAdjacency::getConnectedLowers(int posHi) const
{
   return m_high_2_low.at(posHi);
}

const std::vector<int> TopologicalAdjacency::getConnectedHighers(int posLo) const
{
   return m_low_2_high.at(posLo);
}

std::unique_ptr<TopologicalAdjacency> TopologicalAdjacency::Create(TopologyDimension dimHi, int countHigh, TopologyDimension dimLo, int countLow, const std::map<int, std::vector<int>>& hi2lo)
{
   std::map<int, std::vector<int>> low_2_high;
   std::map<int, std::vector<int>> high_2_low;

   for (int n = 0; n < countLow; ++n)
   {
      low_2_high.emplace(n, std::vector<int>{});
   }
   for (int n = 0; n < countHigh; ++n)
   {
      high_2_low.emplace(n, std::vector<int>{});
   }
   for (const auto& itr : hi2lo)
   {
      const int high = itr.first;
      for (auto low : itr.second)
      {
         low_2_high.at(low).push_back(high);
         high_2_low.at(high).push_back(low);
      }
   }

   for (auto& itr : low_2_high) std::sort(itr.second.begin(), itr.second.end());
   for (auto& itr : high_2_low) std::sort(itr.second.begin(), itr.second.end());

   // Checks
   // min/max
   for ( const auto itr)

   return std::unique_ptr<TopologicalAdjacency>(new TopologicalAdjacency(dimLo, std::move(low_2_high), dimHi, std::move(high_2_low)));
}