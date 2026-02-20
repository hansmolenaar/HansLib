#include "GraphIsomorphismTaggerTriangles.h"
#include "Defines.h"
#include "GraphIsomorphismTagFlyweight.h"
#include "GraphIsomorphismUtils.h"

using namespace Graph;
using namespace GraphIsomorphism;

TaggerTriangles::TaggerTriangles(std::shared_ptr<Graph::UndirectedGraphTriangles> triangles)
    : m_triangles(std::move(triangles)), m_graphTag(CondenseSizeSequence(m_triangles->getSequence()))

{
    m_graphTag = CondenseSizeSequence(m_triangles->getSequence());
    m_vertexGrouping = VertexGrouping(m_triangles->getGraph().getVertexRange(), VertexLess{*this});
}

const Tag &TaggerTriangles::getGraphTag() const
{
    return m_graphTag;
}

std::weak_ordering TaggerTriangles::compareCharacteristics(const ICharacteristicsCompare &otherComparer) const
{
    const auto &other = dynamic_cast<const TaggerTriangles &>(otherComparer);
    return getGraphTag() <=> other.getGraphTag();
}

const Graph::IGraphUs &TaggerTriangles::getGraph() const
{
    return m_triangles->getGraph();
}

std::weak_ordering TaggerTriangles::compareVertexOtherGraph(Vertex vertex0, const IVertexCompare &other,
                                                            Vertex vertex1) const
{
    return m_triangles->numTrianglesAt(vertex0) <=>
           dynamic_cast<const TaggerTriangles &>(other).m_triangles->numTrianglesAt(vertex1);
}

const VertexGrouping &TaggerTriangles::getVertexGrouping() const
{
    return m_vertexGrouping;
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ICompare> CompareTrianglesFactory::createCompare(const Graph::IGraphUs &graph) const
{
    return std::make_unique<TaggerTriangles>(std::make_shared<Graph::UndirectedGraphTriangles>(graph));
}
