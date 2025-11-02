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
        enum TagStatus
        {
            NotEquivalent,
            Equivalent
        };
        TagStatus TagCompareStatus;
        std::vector<VertexPair> VertexPairs;
    };

    Result compare(std::pair<const IVertexTagger &, const IVertexTagger &>) const;
};

} // namespace GraphIsomorphism
