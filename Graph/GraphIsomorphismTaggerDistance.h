#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

class UndirectedGraphDistance;

namespace GraphIsomorphism
{

class TaggerDistance : public IVertexTagger
{
  public:
    explicit TaggerDistance(std::shared_ptr<UndirectedGraphDistance>);
    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;

  private:
    Tag getTag(GraphVertex v) const;
    std::shared_ptr<UndirectedGraphDistance> m_distances;
    std::vector<Tag> m_tags;
};

class TaggerDistanceFactory : public IVertexTaggerFactory
{
  public:
    std::unique_ptr<IVertexTagger> createVertexTagger(const Graph::IGraphUSC &) override;
};

} // namespace GraphIsomorphism
