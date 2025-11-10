#pragma once

#include "GraphDefines.h"
#include "IGraphUs.h"

namespace Graph
{

class UndirectedGraphDistance
{
  public:
    using AtDistance = std::vector<std::vector<GraphVertex>>;

    explicit UndirectedGraphDistance(const Graph::IGraphUs &);
    const AtDistance &operator()(GraphVertex) const;
    GraphVertex getNumVertices() const;

  private:
    std::vector<AtDistance> m_distances;
};

} // namespace Graph
