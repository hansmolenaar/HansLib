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

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<IGraphTagger> TaggerNumbersFactory::createGraphTagger(const Graph::IGraphUs &graph)
{
    std::unique_ptr<IGraphTagger> retval = std::make_unique<TaggerNumbers>(graph);
    return retval;
}
