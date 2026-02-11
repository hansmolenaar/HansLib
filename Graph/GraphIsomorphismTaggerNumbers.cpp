#include "GraphIsomorphismTaggerNumbers.h"
#include "MyAssert.h"

using namespace GraphIsomorphism;
using namespace Utilities;

GraphIsomorphism::TaggerNumbers::TaggerNumbers(const Graph::IGraphUs &graph) : m_graph(graph)
{
    m_numbers.push_back(graph.getNumVertices());
    m_numbers.push_back(graph.getNumEdges());
}

const Tag &TaggerNumbers::getGraphTag() const
{
    return m_numbers;
}

const Graph::IGraphUs &TaggerNumbers::getGraph() const
{
    return m_graph;
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ICompare> CompareNumbersFactory::createCompare(const Graph::IGraphUs &graph) const
{
    return std::make_unique<TaggerNumbers>(graph);
}
