#pragma once

#include "GraphDefines.h"
#include "IGraphUs.h"

namespace Graph
{

class UndirectedGraphTwins
{
  public:
    explicit UndirectedGraphTwins(const Graph::IGraphUs &);
    const Graph::IGraphUs &getGraph() const;
    const std::vector<std::vector<Vertex>> &getTrueTwins() const;
    const std::vector<std::vector<Vertex>> &getFalseTwins() const;

  private:
    const Graph::IGraphUs &m_graph;
    std::vector<std::vector<Vertex>> m_trueTwins;
    std::vector<std::vector<Vertex>> m_falseTwins;
};

} // namespace Graph
