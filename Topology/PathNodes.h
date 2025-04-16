#pragma once
#include "EdgeNodesDirected.h"
#include "StreamUtils.h"

namespace Topology
{
   class PathNodes
   {
   public:
      explicit PathNodes(std::span<const NodeIndex> nodes);

      NodeIndex operator[](size_t n) const { return m_nodes[n]; };
      NodeIndex at(size_t n) const { return m_nodes[n]; };
      auto begin() const { return m_nodes.begin(); }
      auto end() const { return m_nodes.end(); }
      auto operator<=>(const PathNodes& other) const { return m_nodes <=> other.m_nodes; };
      auto operator==(const PathNodes& other) const { return m_nodes == other.m_nodes; };
      friend std::ostream& operator<<(std::ostream& os, const PathNodes& path)
      {
         return StreamUtils::insertList(os, path);
      }

      bool contains(NodeIndex node) const;
      std::vector<EdgeNodesDirected> getEdges() const;

   private:
      std::vector<NodeIndex> m_nodes;
   };


}