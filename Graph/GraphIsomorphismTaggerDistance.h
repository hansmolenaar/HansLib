#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphIsomorphismVertexCompare.h"

namespace Graph
{
class UndirectedGraphDistance;
}

namespace GraphIsomorphism
{

class TaggerDistance : public IVertexTagger, public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerDistance(std::shared_ptr<Graph::UndirectedGraphDistance>);
    explicit TaggerDistance(const Graph::IGraphUs &);
    const Tag &getVertexTag(GraphVertex) const ;
    const Tag &getGraphTag() const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compare(GraphVertex, GraphVertex) const override;
    std::weak_ordering compareOtherGraph(GraphVertex, const IVertexCompare &, GraphVertex) const override;

  private:
    Tag getTag(GraphVertex v) const;
    std::shared_ptr<Graph::UndirectedGraphDistance> m_distances;
    std::vector<Tag> m_tags;
    Tag m_graphTag;
};

class TaggerDistanceFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
