#pragma once

#include "GraphIsomorphismGrouping.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

class CharacteristicsComparers : public ICharacteristicsCompare
{
  public:
    explicit CharacteristicsComparers(std::vector<const ICharacteristicsCompare *>);

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareCharacteristics(const ICharacteristicsCompare &) const override;

  private:
    std::vector<const ICharacteristicsCompare *> m_characteristicsComparers;
};

} // namespace GraphIsomorphism
