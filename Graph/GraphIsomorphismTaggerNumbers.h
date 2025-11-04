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

  private:
    const Graph::IGraphUs &m_graph;
    Tag m_numbers;
};

class TaggerNumbersFactory : public IGraphTaggerFactory
{
  public:
    std::unique_ptr<IGraphTagger> createGraphTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
