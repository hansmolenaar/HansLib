#pragma once

#include "EigenSolution.h"
#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"
#include "IGraphIsomorphismTagger.h"

#include <memory>
namespace GraphIsomorphism
{

class TaggerSpectrum : public IGraphCompare
{
  public:
    explicit TaggerSpectrum(const Graph::IGraphUs &);

    std::weak_ordering compareGraph(const IGraphCompare &) const override;
    // const Tag &getVertexTag(Graph::Vertex) const;
    const Graph::IGraphUs &getGraph() const override;
    // std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;
    const VertexGrouping &getVertexGrouping() const override;

  private:
    const Graph::IGraphUs &m_graph;
    std::unique_ptr<EigenSolution> m_eigenSolution;
    VertexGrouping m_vertexGrouping;
};

class CompareSpectrumFactory : public IGraphCompareFactory
{
  public:
    std::unique_ptr<IGraphCompare> createGraphCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
