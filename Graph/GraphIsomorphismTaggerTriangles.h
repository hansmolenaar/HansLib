#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"
#include "UndirectedGraphTriangles.h"

namespace GraphIsomorphism
{

class TaggerTriangles : public IGraphCompare, public IVertexCompare
{
  public:
    explicit TaggerTriangles(std::shared_ptr<Graph::UndirectedGraphTriangles>);

    const Tag &getGraphTag() const;
    std::weak_ordering compareGraph(const IGraphCompare &) const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;
    const VertexGrouping &getVertexGrouping() const override;

  private:
    std::shared_ptr<Graph::UndirectedGraphTriangles> m_triangles;
    Tag m_graphTag;
    VertexGrouping m_vertexGrouping;
};

class CompareTrianglesFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
