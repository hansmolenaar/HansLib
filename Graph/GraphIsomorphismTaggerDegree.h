#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerDegree : public IGraphCompare, public IVertexCompare
{
  public:
    explicit TaggerDegree(const Graph::IGraphUs &);

    const Tag &getGraphTag() const;
    std::weak_ordering compareGraph(const IGraphCompare &) const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;
    const VertexGrouping &getVertexGrouping() const override;

  private:
    const Graph::IGraphUs &m_graph;
    Tag m_degreeSequenceTag;
    VertexGrouping m_vertexGrouping;
};

class CompareDegreeFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
