#include "GraphIsomorphismTaggerColor.h"
#include "MyAssert.h"
#include "UndirectedGraphColor.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

TaggerColor::TaggerColor(const Graph::IGraphUs &graph) : m_graph(graph)
{
    const UndirectedGraphColor coloring(m_graph);
    const auto chromaticNumber = coloring.getChromaticNumber();
    if (!chromaticNumber.has_value())
    {
        return;
    }
    m_graphTag.push_back(chromaticNumber.value());
    if (chromaticNumber == 2 && m_graph.isConnected())
    {
        const auto nVertices = m_graph.getNumVertices();
        const auto coloring2 = coloring.getColoring2();
        const Vertex numColorFirst = str::count(coloring2, UndirectedGraphColor::ColorFirst);
        const Vertex numColorSecond = nVertices - numColorFirst;
        MyAssert(numColorFirst > 0);
        MyAssert(numColorSecond > 0);
        m_graphTag.push_back(std::min(numColorFirst, numColorSecond));
        m_graphTag.push_back(std::max(numColorFirst, numColorSecond));
        if (numColorFirst != numColorSecond)
        {
            str::transform(coloring2, std::back_inserter(m_vertexColorTag),
                           [numColorFirst, numColorSecond](UndirectedGraphColor::Color c) {
                               return c == UndirectedGraphColor::ColorFirst ? numColorFirst : numColorSecond;
                           });
        }
    }
}

const Tag &TaggerColor::getGraphTag() const
{
    return m_graphTag;
}

const Graph::IGraphUs &TaggerColor::getGraph() const
{
    return m_graph;
}

TagEntry TaggerColor::getVertexTag(Graph::Vertex v) const
{
    if (m_vertexColorTag.empty())
    {
        return -1;
    }
    return m_vertexColorTag.at(v);
}

std::weak_ordering TaggerColor::compareVertexOtherGraph(Graph::Vertex vertex1, const IVertexCompare &other,
                                                        Graph::Vertex vertex2) const
{
    const TaggerColor &cmp2 = dynamic_cast<const TaggerColor &>(other);
    return getVertexTag(vertex1) <=> cmp2.getVertexTag(vertex2);
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ICompare> CompareColorFactory::createCompare(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerColor>(graph);
}
