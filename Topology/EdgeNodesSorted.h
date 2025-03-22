#pragma once
#include "EdgeNodesDirected.h"

namespace Topology
{

   class EdgeNodesSorted
   {
   public:
      EdgeNodesSorted(NodeIndex n0, NodeIndex n1);
      auto begin() const { return m_edge.begin(); }
      auto end() const { return m_edge.end(); }
      NodeIndex operator[](size_t n) const { return m_edge[n]; };
      auto operator<=>(const EdgeNodesSorted& other) const { return m_edge <=> other.m_edge; };
      auto operator==(const EdgeNodesSorted& other) const { return m_edge == other.m_edge; };
      friend std::ostream& operator<<(std::ostream& os, const EdgeNodesSorted& edge)
      {
         os << "(" << edge[0] << ", " << edge[1] << ")";
         return os;
      }
   private:
      EdgeNodesDirected m_edge;
   };
}