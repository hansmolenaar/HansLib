#pragma once
#include "DirectedEdgeNodes.h"

namespace Topology
{

   class SortedEdgeNodes
   {
   public:
      SortedEdgeNodes(NodeIndex n0, NodeIndex n1);
      auto begin() const { return m_edge.begin(); }
      auto end() const { return m_edge.end(); }
      NodeIndex operator[](size_t n) const { return m_edge[n]; };
      auto operator<=>(const SortedEdgeNodes& other) const { return m_edge <=> other.m_edge; };
      auto operator==(const SortedEdgeNodes& other) const { return m_edge == other.m_edge; };
      friend std::ostream& operator<<(std::ostream& os, const SortedEdgeNodes& edge)
      {
         os << "(" << edge[0] << ", " << edge[1] << ")";
         return os;
      }
   private:
      DirectedEdgeNodes m_edge;
   };
}