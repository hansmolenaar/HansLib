#include "IGraphIsomorphismTagger.h"

using namespace GraphIsomorphism;

const IGraphTagger *ITagger::getGraphTagger() const
{
    return dynamic_cast<const IGraphTagger *>(this);
}

const IVertexTagger *ITagger::getVertexTagger() const
{
    return dynamic_cast<const IVertexTagger *>(this);
}
