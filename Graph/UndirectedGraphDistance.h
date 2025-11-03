#pragma once

#include "GraphDefines.h"
#include "IGraphUSC.h"

namespace Graph
{

class UndirectedGraphDistance
{
  public:
    using AtDistance = std::vector<std::vector<GraphVertex>>;

    explicit UndirectedGraphDistance(const Graph::IGraphUSC &);
    const AtDistance &operator()(GraphVertex) const;
    GraphVertex getNumVertices() const;

  private:
    std::vector<AtDistance> m_distances;
};

}
