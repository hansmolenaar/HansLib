#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

class UndirectedGraphDistance;

namespace GraphIsomorphism
{

class TaggerDistance : public ITagger
{
  public:
    explicit TaggerDistance(const UndirectedGraphDistance &);
    const UndirectedGraph &getGraph() const override;
    VertexTag getTag(GraphVertex) const override;

  private:
    const UndirectedGraphDistance &m_distances;
};

} // namespace GraphIsomorphism
