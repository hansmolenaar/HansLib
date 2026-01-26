#include "IGraphIsomorphismTagger.h"

using namespace Graph;
using namespace GraphIsomorphism;

std::weak_ordering IVertexCompare::compare(Vertex lhs, Vertex rhs) const
{
    return compareVertexOtherGraph(lhs, *this, rhs);
}

const ICharacteristicsCompare *ICompare::getCharacteristicsCompare() const
{
    return dynamic_cast<const ICharacteristicsCompare *>(this);
}

const IVertexCompare *ICompare::getVertexCompare() const
{
    return dynamic_cast<const IVertexCompare *>(this);
}

std::weak_ordering IGraphTagger::compareCharacteristics(const ICharacteristicsCompare &gc) const
{
    return getGraphTag() <=> dynamic_cast<const IGraphTagger *>(&gc)->getGraphTag();
};

const IGraphTagger *ICompare::getGraphTagger() const
{
    return dynamic_cast<const IGraphTagger *>(getCharacteristicsCompare());
}
