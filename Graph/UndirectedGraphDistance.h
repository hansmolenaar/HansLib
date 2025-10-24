#pragma once

#include "GraphDefines.h"

class UndirectedGraph;

class UndirectedGraphDistance
{
  public:
    using AtDistance = std::vector<std::vector<GraphVertex>>;

    explicit UndirectedGraphDistance(const UndirectedGraph &);
    AtDistance operator()(GraphVertex) const;

  private:
    const UndirectedGraph &m_graph;
};
