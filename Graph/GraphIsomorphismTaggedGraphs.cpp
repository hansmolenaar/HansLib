#include "GraphIsomorphismTaggedGraphs.h"
#include "GraphIsomorphismGrouping.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;

TaggedGraphs::TaggedGraphs(const Graph::IGraphUs &graph)
    : m_complement(UndirectedGraph::CreateComplement(graph)), m_taggedGraph(std::make_shared<TaggedGraph>(graph)),
      m_taggedGraphComplement(std::make_shared<TaggedGraph>(m_complement)),
      m_grouping(
          Grouping<Vertex>::Combine(m_taggedGraph->getVertexGrouping(), m_taggedGraphComplement->getVertexGrouping()))
{
}

std::weak_ordering TaggedGraphs::operator<=>(const TaggedGraphs &other) const
{
    std::weak_ordering result = *m_taggedGraph <=> *other.m_taggedGraph;
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    result = *m_taggedGraphComplement <=> *other.m_taggedGraphComplement;
    return result;
}

bool TaggedGraphs::operator==(const TaggedGraphs &other) const
{
    if (!(*m_taggedGraph == *other.m_taggedGraph))
    {
        return false;
    }

    return *m_taggedGraphComplement == *other.m_taggedGraphComplement;
}

const Grouping<Graph::Vertex> &TaggedGraphs::getVertexGrouping() const
{
    return m_grouping;
}
