#pragma once

#include "GraphDefines.h"

class UndirectedGraph;

class UndirectedGraphDistance
{
  public:
    using AtDistance = std::vector<std::vector<GraphVertex>>;

    explicit UndirectedGraphDistance(const UndirectedGraph &);
    const AtDistance &operator()(GraphVertex) const;
    const UndirectedGraph &getGraph() const;

  private:
    const UndirectedGraph &m_graph;
    std::vector<AtDistance> m_distances;
};
