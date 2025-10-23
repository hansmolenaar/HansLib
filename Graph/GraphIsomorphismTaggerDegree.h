#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

class UndirectedGraph;

namespace GraphIsomorphism
{

class TaggerDegree : public IGraphIsomorphismTagger
{
  public:
    explicit TaggerDegree(const UndirectedGraph &);
    const UndirectedGraph &getGraph() const override;
    VertexTag getTag(GraphVertex) const override;

  private:
    const UndirectedGraph &m_graph;
};

} // namespace GraphIsomorphism
