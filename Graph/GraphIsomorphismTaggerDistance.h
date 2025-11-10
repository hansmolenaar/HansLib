#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

namespace Graph
{
class UndirectedGraphDistance;
}

namespace GraphIsomorphism
{

class TaggerDistance : public IVertexTagger
{
  public:
    explicit TaggerDistance(std::shared_ptr<Graph::UndirectedGraphDistance>);
    explicit TaggerDistance(const Graph::IGraphUs &);
    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;
    const Tag &getGraphTag() const;

  private:
    Tag getTag(GraphVertex v) const;
    std::shared_ptr<Graph::UndirectedGraphDistance> m_distances;
    std::vector<Tag> m_tags;
    Tag m_graphTag;
};

class TaggerDistanceFactory : public IVertexTaggerFactory
{
  public:
    std::unique_ptr<IVertexTagger> createVertexTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
