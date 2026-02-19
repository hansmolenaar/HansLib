#pragma once

#include "EigenSolution.h"
#include "GraphDefines.h"
#include "IGraphUs.h"

namespace Graph
{

class UndirectedGraphSpectrum
{
  public:
    explicit UndirectedGraphSpectrum(const Graph::IGraphUs &);
    const Graph::IGraphUs &getGraph() const;
    EigenSolution getEigenSolution() const;

  private:
    const Graph::IGraphUs &m_graph;
};

} // namespace Graph
