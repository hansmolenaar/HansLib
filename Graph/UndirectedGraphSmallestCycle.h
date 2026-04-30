#pragma once

#include "GraphDefines.h"

namespace Graph
{
class IGraphUs;

class UndirectedGraphSmallestCycle
{
  public:
    static constexpr Graph::Vertex noCycleLength = 0;

    explicit UndirectedGraphSmallestCycle(const Graph::IGraphUs &);
    const Graph::IGraphUs &getGraph() const;
    const std::vector<Graph::Vertex> &getSmallestCycleLengths() const;

  private:
    static std::optional<Vertex> getSmallestCycleLength(const Graph::IGraphUs &, Graph::Vertex);

    const Graph::IGraphUs &m_graph;
    std::vector<Graph::Vertex> m_lengths;
};

} // namespace Graph
