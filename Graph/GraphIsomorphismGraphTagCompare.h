#pragma once

#include "GraphIsomorphismTaggerNumbers.h"
#include "IGraphUS.h"

namespace GraphIsomorphism
{

class GraphTagCompare
{
  public:
    GraphTagCompare(const TaggerNumbersFactory &numbersFactory, const std::vector<const IGraphTaggerFactory *> &others);
    Status compare(const Graph::IGraphUS &, const Graph::IGraphUS &) const;

  private:
    const TaggerNumbersFactory &m_numbersFactory;
    std::vector<const IGraphTaggerFactory *> m_others;
};

} // namespace GraphIsomorphism
