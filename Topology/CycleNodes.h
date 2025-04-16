#pragma once
#include "EdgeNodesDirected.h"
#include "StreamUtils.h"

namespace Topology
{
   class CycleNodes
   {
   public:
      explicit CycleNodes(std::span<const NodeIndex> nodes);

      NodeIndex operator[](size_t n) const { return m_nodes[n]; };
      NodeIndex at(size_t n) const { return m_nodes[n]; };
      auto begin() const { return m_nodes.begin(); }
      auto end() const { return m_nodes.end(); }
      auto operator<=>(const CycleNodes& other) const { return m_nodes <=> other.m_nodes; };
      auto operator==(const CycleNodes& other) const { return m_nodes == other.m_nodes; };
      friend std::ostream& operator<<(std::ostream& os, const CycleNodes& cyle)
      {
         return StreamUtils::insertList(os, cyle);
      }

      bool contains(NodeIndex node) const;
      std::vector<EdgeNodesDirected> getEdges() const;

   private:
      std::vector<NodeIndex> m_nodes;
   };


}