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
}

const Tag &TaggerTriangles::getGraphTag() const
{
    return m_graphTag;
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

std::unique_ptr<ICompare> CompareTrianglesFactory::createCompare(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerTriangles>(std::make_shared<Graph::UndirectedGraphTriangles>(graph));
}
