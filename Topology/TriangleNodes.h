#pragma once
#include "EdgeNodesDirected.h"
#include "TopologyDefines.h"

namespace Topology
{
   class TriangleNodes
   {
   public:
      TriangleNodes(NodeIndex n0, NodeIndex n1, NodeIndex n2);
      NodeIndex operator[](size_t n) const { return m_nodes[n]; };
      NodeIndex at(size_t n) const { return m_nodes[n]; };
      auto begin() const { return m_nodes.begin(); }
      auto end() const { return m_nodes.end(); }
      auto operator<=>(const TriangleNodes& other) const { return m_nodes <=> other.m_nodes; };
      auto operator==(const TriangleNodes& other) const { return m_nodes == other.m_nodes; };
      friend std::ostream& operator<<(std::ostream& os, const TriangleNodes& triangle)
      {
         os << "(" << triangle[0] << ", " << triangle[1] << ", " << triangle[2] << ")";
         return os;
      }
      static TriangleNodes createSorted(NodeIndex n0, NodeIndex n1, NodeIndex n2);

      bool contains(NodeIndex node) const;
      bool contains(const EdgeNodesDirected& edge) const;
      NodeIndex oppositeNode(const EdgeNodesDirected& edge) const;
   private:
      std::array<NodeIndex, NumNodesOnTriangle> m_nodes;
   };


}