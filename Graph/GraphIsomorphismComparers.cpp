#include "GraphIsomorphismComparers.h"

#include "GraphIsomorphismComparersFactory.h"
#include "GraphIsomorphismUtils.h"
#include "Iota.h"
#include "MyAssert.h"
#include "UniquePointer.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

Comparers::Comparers(std::vector<std::unique_ptr<ICompare>> &&comparers)
    : m_comparers(std::move(comparers)), m_graphComparers(getCastPointers<const IGraphCompare>(m_comparers)),
      m_vertexComparers(selectVertexCompare(m_comparers))
{
}

Comparers::Comparers(const Graph::IGraphUs &graph) : Comparers(ComparersFactory().getAllComparers(graph))
{
}

const VertexGrouping &Comparers::getVertexGrouping() const
{
    return m_vertexComparers.getVertexGrouping();
}

const Graph::IGraphUs &Comparers::getGraph() const
{
    return m_comparers.front()->getGraph();
}

std::weak_ordering Comparers::compareGraph(const IGraphCompare &other) const
{
    const auto &lhs = *this;
    const auto &rhs = dynamic_cast<const Comparers &>(other);

    const size_t nGraphComparers = m_graphComparers.size();
    MyAssert(rhs.m_graphComparers.size() == nGraphComparers);
    for (size_t n : Iota::GetRange(nGraphComparers))
    {
        const auto result = lhs.m_graphComparers.at(n)->compareGraph(*rhs.m_graphComparers.at(n));
        if (result != std::weak_ordering::equivalent)
        {
            return result;
        }
    }

    return lhs.m_vertexComparers.compareGraph(rhs.m_vertexComparers);
}
