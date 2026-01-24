#pragma once

#include "GraphIsomorphismGrouping.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

class GraphComparers : public IGraphCompare
{
  public:
    explicit GraphComparers(std::vector<const IGraphCompare *>);

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(const IGraphCompare &) const override;

  private:
    std::vector<const IGraphCompare *> m_graphComparers;
};

} // namespace GraphIsomorphism
