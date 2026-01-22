#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"
#include "UndirectedGraphTriangles.h"

namespace GraphIsomorphism
{

class TaggerTriangles : public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerTriangles(std::shared_ptr<Graph::UndirectedGraphTriangles>);

    const Tag &getGraphTag() const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;

  private:
    std::shared_ptr<Graph::UndirectedGraphTriangles> m_triangles;
    Tag m_graphTag;
};

class CompareTrianglesFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
