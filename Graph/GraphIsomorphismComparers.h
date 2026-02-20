#pragma once

#include "GraphIsomorphismCharacteristicsComparers.h"
#include "GraphIsomorphismVertexComparers.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

class Comparers : public IGraphCompare
{
  public:
    explicit Comparers(std::vector<std::unique_ptr<ICompare>> &&);
    explicit Comparers(const Graph::IGraphUs &);

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareGraph(const IGraphCompare &) const override;
    const VertexGrouping &getVertexGrouping() const;

  private:
    std::vector<std::unique_ptr<ICompare>> m_comparers;
    CharacteristicsComparers m_characteristicsComparers;
    VertexComparers m_vertexComparers;
};

} // namespace GraphIsomorphism
