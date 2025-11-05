#include "GraphIsomorphismStatus.h"
#include "Defines.h"
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

void Status::setFlag(Flag flag)
{
    MyAssert(flag == m_flag || m_flag == Flag::Undecided);
    m_flag = flag;
}

void Status::addPair(VertexPair vp)
{
    MyAssert(!containsFirst(vp.first));
    MyAssert(!containsSecond(vp.second));

    m_vertexFirst.push_back(vp.first);
    m_vertexSecond.push_back(vp.second);

    MyAssert(getFlag() == Flag::Undecided);
    if (m_vertexFirst.size() == m_numVertices)
    {
        m_flag = Flag::Isomorphic;
    }
}

std::vector<VertexPair> Status::getVertexPairs() const
{
    std::vector<VertexPair> result;
    for (size_t n = 0; n < m_vertexFirst.size(); ++n)
    {
        result.push_back({m_vertexFirst.at(n), m_vertexSecond.at(n)});
    }
    return result;
}

bool Status::containsFirst(GraphVertex vertex) const
{
    return str::find(m_vertexFirst, vertex) != m_vertexFirst.end();
}

bool Status::containsSecond(GraphVertex vertex) const
{
    return str::find(m_vertexSecond, vertex) != m_vertexSecond.end();
}
