#pragma once

#include "EdgeNodesSorted.h"
#include "TriangleNodes.h"

namespace Topology
{
   class TriangleNodesOriented
   {
   public:
      // Ordering defines 'positive' are
      // Nodes are rotated such that the first node is the smallest
      TriangleNodesOriented(NodeIndex n0, NodeIndex n1, NodeIndex n2);
      NodeIndex operator[](size_t n) const { return m_triangle[n]; };
      NodeIndex at(size_t n) const { return m_triangle[n]; };
      auto begin() const { return m_triangle.begin(); }
      auto end() const { return m_triangle.end(); }
      auto operator<=>(const TriangleNodesOriented& other) const { return m_triangle <=> other.m_triangle; };
      auto operator==(const TriangleNodesOriented& other) const { return m_triangle == other.m_triangle; };
      friend std::ostream& operator<<(std::ostream& os, const TriangleNodesOriented& triangle)
      {
         os << triangle.m_triangle;
         return os;
      }

      bool contains(NodeIndex node) const { return m_triangle.contains(node); }
      bool containsDirectedEdge(const EdgeNodesDirected& edge) const;
      NodeIndex oppositeNode(const EdgeNodesDirected& edge) const { return m_triangle.oppositeNode(edge); }
      NodeIndex oppositeNode(const EdgeNodesSorted& edge) const { return m_triangle.oppositeNode(edge); }
      const TriangleNodes& asTriangleNodes() const { return m_triangle; }
      std::array<EdgeNodesDirected, NumEdgesOnTriangle> getEdges() const;

      // returns std::numeric_limtis<size_t>::max() if not found
      size_t find(NodeIndex node) const;
   private:
      TriangleNodes m_triangle;
   };


}