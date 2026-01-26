#pragma once

#include "GraphIsomorphismGrouping.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

class GraphComparers : public ICharacteristicsCompare
{
  public:
    explicit GraphComparers(std::vector<const ICharacteristicsCompare *>);

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareCharacteristics(const ICharacteristicsCompare &) const override;

  private:
    std::vector<const ICharacteristicsCompare *> m_graphComparers;
};

} // namespace GraphIsomorphism
