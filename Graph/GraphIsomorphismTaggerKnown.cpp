#include "GraphIsomorphismTaggerKnown.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

GraphIsomorphism::TaggerKnown::TaggerKnown(const Graph::IGraphUs &graph) : m_tag{KnownType::Unknown}
{
    const auto nVertices = graph.getNumVertices();

    if (nVertices == 0)
    {
        return;
    }

    if (!graph.isConnected())
    {
        return;
    }

    if (graph.isComplete())
    {
        m_tag = {KnownType::Complete, static_cast<TagEntry>(nVertices)};
        return;
    }

    const std::vector<GraphVertex> degreeSequence = graph.getSortedDegreeSequence();
    if (degreeSequence.back() > 2)
    {
        return;
    }

    if (degreeSequence.front() == 2)
    {
        m_tag = {KnownType::Cycle, static_cast<TagEntry>(nVertices)};
        return;
    }

    MyAssert(degreeSequence.at(0) == 1);
    MyAssert(degreeSequence.at(1) == 1);
    MyAssert(degreeSequence.at(2) == 2);
    m_tag = {KnownType::Path, static_cast<TagEntry>(nVertices)};
}

const Tag &TaggerKnown::getGraphTag() const
{
    return m_tag;
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ITagger> TaggerKnownFactory::createTagger(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerKnown>(graph);
}
