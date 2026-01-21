#include "IGraphIsomorphismTagger.h"

using namespace Graph;
using namespace GraphIsomorphism;

const IGraphTagger *ITagger::getGraphTagger() const
{
    return dynamic_cast<const IGraphTagger *>(this);
}

const IVertexCompare *ITagger::getVertexCompare() const
{
    return dynamic_cast<const IVertexCompare *>(this);
}

std::weak_ordering IVertexCompare::compare(Vertex lhs, Vertex rhs) const
{
    return compareOtherGraph(lhs, *this, rhs);
}

const IGraphCompare *ICompare::getGraphCompare() const
{
    return dynamic_cast<const IGraphCompare *>(this);
}

const IVertexCompare *ICompare::getVertexCompare() const
{
    return dynamic_cast<const IVertexCompare *>(this);
}

std::weak_ordering IGraphTagger::compareOtherGraph(const IGraphCompare &gc) const
{
    return getGraphTag() <=> dynamic_cast<const IGraphTagger *>(&gc)->getGraphTag();
};
