#include "GraphIsomorphismTaggerComponents.h"
#include "Defines.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerComponents::TaggerComponents(const Graph::IGraphUS &graph)
{
    const auto components = graph.getConnectedComponents();
    std::map<GraphVertex, TagEntry> componentSize;
    for (auto c : components)
    {
        componentSize[c] += 1;
    }

    for (auto itr : componentSize)
    {
        m_graphTag.push_back(itr.second);
    }

    str::sort(m_graphTag);
    m_isConnected = (m_graphTag.size() == 1);
}

bool TaggerComponents::isConnected() const
{
    return m_isConnected;
}

const Tag &TaggerComponents::getGraphTag() const
{
    return m_graphTag;
}

// !!!!!!!!!!!!!

std::unique_ptr<IGraphTagger> TaggerComponentFactory::createGraphTagger(const Graph::IGraphUS &graph)
{
    return std::make_unique<TaggerComponents>(graph);
}
