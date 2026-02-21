#pragma once

#include "GraphIsomorphismStatus.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerNumbers : public IGraphCompare
{
  public:
    explicit TaggerNumbers(const Graph::IGraphUs &);

    const Tag &getGraphTag() const;
    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareGraph(const IGraphCompare &) const override;

  private:
    const Graph::IGraphUs &m_graph;
    Tag m_numbers;
};

class CompareNumbersFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
