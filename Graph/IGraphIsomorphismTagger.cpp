#include "IGraphIsomorphismTagger.h"

using namespace GraphIsomorphism;

const IGraphTagger *ITagger::getGraphTagger() const
{
    return dynamic_cast<const IGraphTagger *>(this);
}

const IVertexCompare *ITagger::getVertexCompare() const
{
    return dynamic_cast<const IVertexCompare *>(this);
}
