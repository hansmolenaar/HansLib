#pragma once
#include "StreamUtils.h"
#include "TopologyDefines.h"
#include <ostream>

namespace Topology
{
class EdgeNodesDirected
{
  public:
    EdgeNodesDirected(NodeIndex n0, NodeIndex n1);
    NodeIndex operator[](size_t n) const
    {
        return m_nodes[n];
    };
    auto begin() const
    {
        return m_nodes.begin();
    }
    auto end() const
    {
        return m_nodes.end();
    }
    auto operator<=>(const EdgeNodesDirected &other) const
    {
        return m_nodes <=> other.m_nodes;
    };
    auto operator==(const EdgeNodesDirected &other) const
    {
        return m_nodes == other.m_nodes;
    };
    friend std::ostream &operator<<(std::ostream &os, const EdgeNodesDirected &edge)
    {
        return StreamUtils::insertList(os, edge, "(", " -> ", ")");
    }
    bool contains(NodeIndex node) const;

  private:
    std::array<NodeIndex, NumCornersOnEdge> m_nodes;
};
} // namespace Topology