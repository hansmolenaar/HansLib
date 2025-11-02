#include "GraphIsomorphismStatus.h"
#include "MyAssert.h"

using namespace GraphIsomorphism;
using namespace Utilities;

Status::Status(GraphVertex numVertices) : m_numVertices(numVertices), m_flag(Flag::Undecided)
{
}
Flag Status::getFlag() const
{
    return m_flag;
}

void Status::addPair(VertexPair vp)
{
    VertexPairs.push_back(vp);
    MyAssert(getFlag() == Flag::Undecided);
    if (VertexPairs.size() == m_numVertices)
    {
        m_flag = Flag::Isomorphic;
    }
}

const std::vector<VertexPair> &Status::getVertexPairs() const
{
    return VertexPairs;
}
