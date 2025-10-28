#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

class UndirectedGraphDistance;

namespace GraphIsomorphism
{

class TaggerDistance : public ITagger
{
  public:
    explicit TaggerDistance(std::shared_ptr<UndirectedGraphDistance>);
    const UndirectedGraph &getGraph() const override;
    VertexTag getTag(GraphVertex) const override;

  private:
    std::shared_ptr<UndirectedGraphDistance> m_distances;
};

class TaggerDistanceFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> create(const UndirectedGraph &) override;
};

} // namespace GraphIsomorphism
