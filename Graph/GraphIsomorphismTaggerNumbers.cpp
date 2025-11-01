#include "GraphIsomorphismTaggerNumbers.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerNumbers::TaggerNumbers(const Graph::IGraphUS &graph)
    : m_graph(graph)
{
    m_numbers.push_back(graph.getNumVertices());
    m_numbers.push_back(graph.getNumEdges());
}

const Tag &TaggerNumbers::getTag() const
{
    return m_numbers;
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<IGraphTagger> TaggerNumbersFactory::create(const Graph::IGraphUS &graph)
{
    std::unique_ptr<IGraphTagger> retval = std::make_unique<TaggerNumbers>(graph);
    return retval;
}
