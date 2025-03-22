#pragma once
#include "EdgeNodesDirected.h"
#include "TriangleNodes.h"

namespace Topology
{
   class TriangleNodesSorted
   {
   public:
      TriangleNodesSorted(NodeIndex n0, NodeIndex n1, NodeIndex n2);
      NodeIndex operator[](size_t n) const { return m_triangle[n]; };
      NodeIndex at(size_t n) const { return m_triangle[n]; };
      auto begin() const { return m_triangle.begin(); }
      auto end() const { return m_triangle.end(); }
      auto operator<=>(const TriangleNodesSorted& other) const { return m_triangle <=> other.m_triangle; };
      auto operator==(const TriangleNodesSorted& other) const { return m_triangle == other.m_triangle; };
      friend std::ostream& operator<<(std::ostream& os, const TriangleNodesSorted& triangle)
      {
         os << triangle.m_triangle;
         return os;
      }

      bool contains(NodeIndex node) const { return m_triangle.contains(node); }
      bool contains(const EdgeNodesDirected& edge) const { return m_triangle.contains(edge); };
      NodeIndex oppositeNode(const EdgeNodesDirected& edge) const { return m_triangle.oppositeNode(edge); }
      const TriangleNodes& asTriangleNodes() const { return m_triangle; }
   private:
      TriangleNodes m_triangle;
   };


}