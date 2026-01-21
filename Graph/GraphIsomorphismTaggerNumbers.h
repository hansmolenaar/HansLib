#pragma once

#include "GraphIsomorphismStatus.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerNumbers : public IGraphTagger
{
  public:
    explicit TaggerNumbers(const Graph::IGraphUs &);

    const Tag &getGraphTag() const override;
    const Graph::IGraphUs &getGraph() const override;

  private:
    const Graph::IGraphUs &m_graph;
    Tag m_numbers;
};

class TaggerNumbersFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

class CompareNumbersFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
