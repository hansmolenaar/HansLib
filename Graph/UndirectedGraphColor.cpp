#include "UndirectedGraphColor.h"
#include "MyAssert.h"

using namespace Graph;
using namespace Utilities;

namespace
{
constexpr UndirectedGraphColor::Color ColorUndefined = std::numeric_limits<UndirectedGraphColor::Color>::max();

bool ColorRecur(Vertex v, UndirectedGraphColor::Color c, const Graph::IGraphUs &graph,
                std::vector<UndirectedGraphColor::Color> &result)
{
    if (result.at(v) != ColorUndefined)
    {
        return (result[v] == c);
    }

    result[v] = c;

    std::vector<Vertex> ngbs;
    graph.setAdjacentVertices(v, ngbs);
    for (auto ngb : ngbs)
    {
        constexpr UndirectedGraphColor::Color ColorSecond = 1;
        const auto otherColor =
            (c == UndirectedGraphColor::ColorFirst ? ColorSecond : UndirectedGraphColor::ColorFirst);
        if (!ColorRecur(ngb, otherColor, graph, result))
        {
            return false;
        }
    }

    return true;
}

std::vector<UndirectedGraphColor::Color> GetColoring2(const Graph::IGraphUs &graph)
{
    const auto numVertices = graph.getNumVertices();
    std::vector<UndirectedGraphColor::Color> result(numVertices, ColorUndefined);

    for (Vertex current : Iota::GetRange(numVertices))
    {

        if (result.at(current) != ColorUndefined)
        {
            continue;
        }
        if (!ColorRecur(current, UndirectedGraphColor::ColorFirst, graph, result))
        {
            return {};
        }
    }

    return result;
}

std::optional<UndirectedGraphColor::Color> Test3ColoredBrooks(const IGraphUs &graph)
{
    // We know the graph is not 2-colorable, now check if it is 3 colorable using Brooks theorem
    if (graph.isConnected())
    {
        const auto degreeSequence = graph.getSortedDegreeSequence();
        const auto degreeMin = degreeSequence.front();
        const auto degreeMax = degreeSequence.back();
        MyAssert(degreeMin <= degreeMax);
        MyAssert(degreeMax >= 2);
        if (degreeMin + 1 == graph.getNumVertices())
        {
            // Complete graph
            return degreeMin + 1;
        }
        else if (degreeMax == 3)
        {
            return 3;
        }
    }
    return {};
}

} // namespace

UndirectedGraphColor::UndirectedGraphColor(const Graph::IGraphUs &graph) : m_graph(graph)
{
    const auto numVertices = m_graph.getNumVertices();
    if (numVertices == 0)
    {
        m_chromaticNumber = 0;
        return;
    }

    const auto colors = GetColoring2(m_graph);
    if (colors.empty())
    {
        // Maybe it is 3 colored or complete
        m_chromaticNumber = Test3ColoredBrooks(m_graph);
        return;
    }

    m_chromaticNumber = *str::max_element(colors) + 1;

    if (m_chromaticNumber == 2)
    {
        m_colors = colors;
    }
}

std::vector<UndirectedGraphColor::Color> UndirectedGraphColor::getColoring2() const
{
    MyAssert(m_chromaticNumber.has_value());
    MyAssert(m_chromaticNumber == 2);
    return m_colors;
}

std::optional<UndirectedGraphColor::Color> UndirectedGraphColor::getChromaticNumber() const
{
    return m_chromaticNumber;
}
