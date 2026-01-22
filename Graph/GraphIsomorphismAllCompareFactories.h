#pragma once

#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

class AllCompareFactories
{
  public:
    std::vector<std::unique_ptr<ICompare>> getAllComparers(const Graph::IGraphUs &) const;
};

} // namespace GraphIsomorphism
