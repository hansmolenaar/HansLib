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
    const Graph::IGraphUs &getGraph() const override;
    const EigenSolution *getEigenSolution() const;

  private:
    const Graph::IGraphUs &m_graph;
    std::unique_ptr<EigenSolution> m_eigenSolution;
    VertexGrouping m_vertexGrouping;
};

class CompareSpectrumFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
