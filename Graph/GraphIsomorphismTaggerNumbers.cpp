#include "GraphIsomorphismTaggerNumbers.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerNumbers::TaggerNumbers(const Graph::IGraphUS &graph) : m_graph(graph)
{
    m_numbers.push_back(graph.getNumVertices());
    m_numbers.push_back(graph.getNumEdges());
}

const Tag &TaggerNumbers::getGraphTag() const
{
    return m_numbers;
}

Status TaggerNumbers::compare(const Graph::IGraphUS &graph0, const Graph::IGraphUS &graph1)
{
    const TaggerNumbers tagger0(graph0);
    const TaggerNumbers tagger1(graph1);

    if (tagger0.getGraphTag() != tagger1.getGraphTag())
    {
        return {};
    }

    // Some special cases
    const auto nVertices = graph0.getNumVertices();
    const auto nEdges = graph0.getNumEdges();

    if (nEdges == 0 || nEdges == (nVertices * (nVertices - 1) / 2))
    {
        Status result(nVertices);
        for (GraphVertex v = 0; v < nVertices; ++v)
        {
            result.VertexPairs.push_back({v, v});
        }
        result.StatusFlag = Isomorphic;
        return result;
    }

    return {};
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<IGraphTagger> TaggerNumbersFactory::createGraphTagger(const Graph::IGraphUS &graph)
{
    std::unique_ptr<IGraphTagger> retval = std::make_unique<TaggerNumbers>(graph);
    return retval;
}
