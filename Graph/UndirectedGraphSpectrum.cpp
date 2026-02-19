#include "UndirectedGraphSpectrum.h"

#include "Iota.h"
#include "MatrixDenseSymmetric.h"

using namespace Graph;

namespace
{
MatrixDenseSymmetric createLaplacian(const Graph::IGraphUs &graph)
{
    const auto nVertices = graph.getNumVertices();
    MatrixDenseSymmetric result(nVertices);

    std::vector<Vertex> neighors;
    for (auto v : Iota::GetRange(nVertices))
    {
        graph.setAdjacentVertices(v, neighors);
        result.set(v, v, neighors.size());
        for (auto n : neighors)
        {
            if (n > v)
            {
                result.set(n, v, -1.0);
            }
        }
    }

    return result;
}
} // namespace

UndirectedGraphSpectrum::UndirectedGraphSpectrum(const Graph::IGraphUs &graph) : m_graph(graph)
{
}

const Graph::IGraphUs &UndirectedGraphSpectrum::getGraph() const
{
    return m_graph;
}

EigenSolution UndirectedGraphSpectrum::getEigenSolution() const
{
    auto laplacian = createLaplacian(m_graph);
    return laplacian.getEigenSolution();
}
