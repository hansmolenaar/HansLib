#pragma once
#include "EdgeNodesDirected.h"
#include "TriangleNodes.h"

namespace Topology
{
   class TetrahedronNodesOriented
   {
   public:
      // Nodes are permuted such that the first node is the smallest, the last the biggest and the permutation is even
      TetrahedronNodesOriented(NodeIndex n0, NodeIndex n1, NodeIndex n2, NodeIndex n3);
      explicit Topology::TetrahedronNodesOriented(std::array<NodeIndex, NumNodesOnTetrehadron> nodes);

      NodeIndex operator[](size_t n) const { return m_nodes[n]; };
      NodeIndex at(size_t n) const { return m_nodes[n]; };
      auto begin() const { return m_nodes.begin(); }
      auto end() const { return m_nodes.end(); }
      auto operator<=>(const TetrahedronNodesOriented& other) const { return m_nodes <=> other.m_nodes; };
      auto operator==(const TetrahedronNodesOriented& other) const { return m_nodes == other.m_nodes; };

      friend std::ostream& operator<<(std::ostream& os, const TetrahedronNodesOriented tet)
      {
         os << "(" << tet[0] << ", " << tet[1] << ", " << tet[2] << ", " << tet[3] << ")";
         return os;
      }

      bool contains(NodeIndex node) const;
      //bool contains(const EdgeNodesDirected& edge) const { return m_triangle.contains(edge); };
      //NodeIndex oppositeNode(const EdgeNodesDirected& edge) const { return m_triangle.oppositeNode(edge); }
      //const TriangleNodes& asTriangleNodes() const { return m_triangle; }

      // returns std::numeric_limtis<size_t>::max() if not found
      size_t find(NodeIndex node) const;

      // If the tetrahedron has positive volume, the normals to the faces are outward pointing.
      std::array<TriangleNodes, NumFacesOnTetrehadron> getFaces() const;
   private:
      std::array<NodeIndex, Topology::NumNodesOnTetrehadron> m_nodes;
   };


}