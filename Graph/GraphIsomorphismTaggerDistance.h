#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

class UndirectedGraphDistance;

namespace GraphIsomorphism
{

class TaggerDistance : public ITagger, public IVertexTagger
{
  public:
    explicit TaggerDistance(std::shared_ptr<UndirectedGraphDistance>);
    const UndirectedGraph &getGraph() const override;
    Tag getTag(GraphVertex) const override;
    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;

  private:
    std::shared_ptr<UndirectedGraphDistance> m_distances;
    std::vector<Tag> m_tags;
};

class TaggerDistanceFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> create(const UndirectedGraph &) override;
};

} // namespace GraphIsomorphism
