#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include <vector>

namespace GraphIsomorphism
{

class TagCompare
{
  public:
    struct Result
    {
        enum Status
        {
            NotEquivalent,
            Equivalent
        };
        Status CompareStatus;
        std::vector<VertexPair> VertexPairs;
    };
    Result operator()(std::pair<const ITagger &, const ITagger &>) const;
};

} // namespace GraphIsomorphism
