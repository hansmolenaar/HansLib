#include "GraphIsomorphismTaggerSpectrum.h"
#include "Functors.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismUtils.h"
#include "MyAssert.h"
#include "UndirectedGraphSpectrum.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

TaggerSpectrum::TaggerSpectrum(const Graph::IGraphUs &graph)
    : m_graph(graph),
      m_eigenSolution(m_graph.getNumVertices() > 0
                          ? std::make_unique<EigenSolution>(UndirectedGraphSpectrum(m_graph).getEigenSolution())
                          : std::unique_ptr<EigenSolution>())
{
}

const Graph::IGraphUs &TaggerSpectrum::getGraph() const
{
    return m_graph;
}

std::weak_ordering TaggerSpectrum::compareGraph(const IGraphCompare &other) const
{
    const Vertex lhsNumVertices = getGraph().getNumVertices();
    const Vertex rhsNumVertices = other.getGraph().getNumVertices();

    if (lhsNumVertices != rhsNumVertices)
    {
        return lhsNumVertices <=> rhsNumVertices;
    }
    else if (lhsNumVertices == 0)
    {
        return std::weak_ordering::equivalent;
    }

    const auto &lhsEigenSolution = m_eigenSolution;
    const auto &rhsEigenSolution = dynamic_cast<const TaggerSpectrum &>(other).m_eigenSolution;
    Functors::AreClose areClose{.RelTolerance = 1.0e-2, .AbsTolerance = 1.0e-5};
    const double lhsMaxVal = lhsEigenSolution->getAvailableEigenValues().back();
    const double rhsMaxVal = rhsEigenSolution->getAvailableEigenValues().back();

    if (areClose(lhsMaxVal, rhsMaxVal))
    {
        return std::weak_ordering::equivalent;
    }
    else if (lhsMaxVal < rhsMaxVal)
    {
        return std::weak_ordering::less;
    }
    else
    {
        return std::weak_ordering::greater;
    }
}

const EigenSolution *TaggerSpectrum::getEigenSolution() const
{
    return m_eigenSolution.get();
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ICompare> CompareSpectrumFactory::createCompare(const Graph::IGraphUs &graph) const
{
    return std::make_unique<TaggerSpectrum>(graph);
}
