#include "IGraphIsomorphismTagger.h"

using namespace GraphIsomorphism;

const IGraphTagger *ITagger::getGraphTagger() const
{
    return dynamic_cast<const IGraphTagger *>(this);
}
