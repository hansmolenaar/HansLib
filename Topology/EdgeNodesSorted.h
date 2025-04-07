#pragma once
#include "EdgeNodesDirected.h"

namespace Topology
{

   class EdgeNodesSorted
   {
   public:
      explicit EdgeNodesSorted(const EdgeNodesDirected& edge);
      EdgeNodesSorted(NodeIndex n0, NodeIndex n1);
      auto begin() const { return m_edge.begin(); }
      auto end() const { return m_edge.end(); }
      NodeIndex operator[](size_t n) const { return m_edge[n]; };
      auto operator<=>(const EdgeNodesSorted& other) const { return m_edge <=> other.m_edge; };
      auto operator==(const EdgeNodesSorted& other) const { return m_edge == other.m_edge; };
      friend std::ostream& operator<<(std::ostream& os, const EdgeNodesSorted& edge)
      {
         return StreamUtils::insertList(os, edge);
      }
   private:
      EdgeNodesDirected m_edge;
   };
}