#pragma once

#include "GraphDefines.h"
#include "IGraphUs.h"

namespace Graph
{

class UndirectedGraphTriangles
{
  public:
    explicit UndirectedGraphTriangles(const Graph::IGraphUs &);
    const Graph::IGraphUs &getGraph() const;
    size_t numTrianglesAt(GraphVertex) const;
    size_t numNeighborsInTrianglesAt(GraphVertex) const;
    std::vector<size_t> getSequence() const;

  private:
    const Graph::IGraphUs &m_graph;
    std::vector<size_t> m_countPerVertex;
    std::vector<size_t> m_numNeigborsInTriangles;
};

} // namespace Graph
