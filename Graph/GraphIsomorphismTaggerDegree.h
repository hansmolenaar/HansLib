#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerDegree : public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerDegree(const Graph::IGraphUs &);

    const Tag &getGraphTag() const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(Graph::GraphVertex, const IVertexCompare &, Graph::GraphVertex) const override;

  private:
    const Graph::IGraphUs &m_graph;
    Tag m_degreeSequenceTag;
};

class TaggerDegreeFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
