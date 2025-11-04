#pragma once

#include "GraphIsomorphismStatus.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUS.h"

namespace GraphIsomorphism
{

class TaggerNumbers : public IGraphTagger
{
  public:
    explicit TaggerNumbers(const Graph::IGraphUS &);

    const Tag &getGraphTag() const override;

  private:
    const Graph::IGraphUS &m_graph;
    Tag m_numbers;
};

class TaggerNumbersFactory : public IGraphTaggerFactory
{
  public:
    std::unique_ptr<IGraphTagger> createGraphTagger(const Graph::IGraphUS &) override;
};

} // namespace GraphIsomorphism
