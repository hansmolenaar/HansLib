#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerComponents : public ICharacteristicsCompare
{
  public:
    explicit TaggerComponents(const Graph::IGraphUs &);

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareCharacteristics(const ICharacteristicsCompare &) const override;
    const Tag &getGraphTag() const;
    bool isConnected() const;

  private:
    const Graph::IGraphUs &m_graph;
    bool m_isConnected;
    Tag m_graphTag;
};

class CompareComponentsFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
