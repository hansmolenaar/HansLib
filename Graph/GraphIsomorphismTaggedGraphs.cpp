#include "GraphIsomorphismTaggedGraphs.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;

TaggedGraphs::TaggedGraphs(const Graph::IGraphUs &graph)
    : m_complement(UndirectedGraph::CreateComplement(graph)), m_taggedGraph(std::make_shared<TaggedGraph>(graph)),
      m_taggedGraphComplement(std::make_shared<TaggedGraph>(m_complement))
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

namespace
{
std::map<Graph::Vertex, size_t> Invert(const Grouping<Graph::Vertex> &grouping)
{
    const auto size = grouping.size();
    std::map<Graph::Vertex, size_t> result;
    for (size_t g = 0UZ; g < grouping().size(); ++g)
    {
        for (Vertex vertex : grouping().at(g))
        {
            result[vertex] = g;
        }
    }

    Utilities::MyAssert(result.begin()->first == 0);
    Utilities::MyAssert(result.rbegin()->first + 1 == size);
    return result;
}
} // namespace

Grouping<Graph::Vertex> TaggedGraphs::Combine(const Grouping<Graph::Vertex> &grouping0,
                                              const Grouping<Graph::Vertex> &grouping1)
{
    const auto size = grouping0.size();
    Utilities::MyAssert(size == grouping1.size());

    const std::map<Graph::Vertex, size_t> map0 = Invert(grouping0);
    const std::map<Graph::Vertex, size_t> map1 = Invert(grouping1);
    std::vector<Vertex> vertices(size);
    str::iota(vertices, static_cast<Vertex>(0));
    auto cmp = [&map0, &map1](Vertex v0, Vertex v1) {
        if (map0.at(v0) < map0.at(v1))
            return true;
        return map1.at(v0) < map1.at(v1);
    };
    return Grouping<Graph::Vertex>(vertices, cmp);
}
