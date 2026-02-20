#include "GraphIsomorphismTaggerComponents.h"
#include "Defines.h"
#include "GraphIsomorphismUtils.h"

using namespace Graph;
using namespace GraphIsomorphism;

GraphIsomorphism::TaggerComponents::TaggerComponents(const Graph::IGraphUs &graph) : m_graph(graph)
{
    const auto components = graph.getConnectedComponents();
    std::map<Vertex, TagEntry> componentSize;
    for (auto c : components)
    {
        componentSize[c] += 1;
    }

    Tag tag;
    for (auto itr : componentSize)
    {
        tag.push_back(itr.second);
    }
    str::sort(tag);
    m_isConnected = (tag.size() == 1);
    m_graphTag = CondenseSizeSequence(tag);
}

bool TaggerComponents::isConnected() const
{
    return m_isConnected;
}

const Tag &TaggerComponents::getGraphTag() const
{
    return m_graphTag;
}

std::weak_ordering TaggerComponents::compareCharacteristics(const ICharacteristicsCompare &otherComparer) const
{
    const auto &other = dynamic_cast<const TaggerComponents &>(otherComparer);
    return getGraphTag() <=> other.getGraphTag();
}

const Graph::IGraphUs &TaggerComponents::getGraph() const
{
    return m_graph;
}

// !!!!!!!!!!!!!

std::unique_ptr<ICompare> CompareComponentsFactory::createCompare(const Graph::IGraphUs &graph) const
{
    return std::make_unique<TaggerComponents>(graph);
}
