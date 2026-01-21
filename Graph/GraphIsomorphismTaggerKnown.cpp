#include "GraphIsomorphismTaggerKnown.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

GraphIsomorphism::TaggerKnown::TaggerKnown(const Graph::IGraphUs &graph)
    : m_graph(graph), m_tag{KnownType::Unknown}, m_description{"Unknown"}
{
    const auto nVertices = graph.getNumVertices();

    if (nVertices == 0)
    {
        return;
    }

    const auto nComponents = graph.getNumberOfComponents();
    if (nComponents > 1 && nVertices > 1)
    {
        if (graph.getNumberOfComponents() == nVertices)
        {
            m_tag = {KnownType::Singletons, static_cast<TagEntry>(nVertices)};
            m_description = "completely disconnected graph of order " + std::to_string(nVertices);
        }
        return;
    }

    if (graph.isComplete())
    {
        m_tag = {KnownType::Complete, static_cast<TagEntry>(nVertices)};
        m_description = "complete graph of order " + std::to_string(nVertices);
        return;
    }

    const std::vector<Vertex> degreeSequence = graph.getSortedDegreeSequence();
    if (degreeSequence.back() > 2)
    {
        return;
    }

    if (degreeSequence.front() == 2)
    {
        m_tag = {KnownType::Cycle, static_cast<TagEntry>(nVertices)};
        m_description = "cycle of order " + std::to_string(nVertices);
        return;
    }

    MyAssert(degreeSequence.at(0) == 1);
    MyAssert(degreeSequence.at(1) == 1);
    MyAssert(degreeSequence.at(2) == 2);
    m_tag = {KnownType::Path, static_cast<TagEntry>(nVertices)};
    m_description = "path of order " + std::to_string(nVertices);
}

const Graph::IGraphUs &TaggerKnown::getGraph() const 
{
 return m_graph;
}

const Tag &TaggerKnown::getGraphTag() const
{
    return m_tag;
}

std::string TaggerKnown::getDescription() const
{
    return m_description;
}
// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ITagger> TaggerKnownFactory::createTagger(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerKnown>(graph);
}
