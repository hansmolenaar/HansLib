#pragma once

#include "GraphIsomorphismComparers.h"
#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggedGraph : public IGraphCompare, public IVertexCompare
{
  public:
    explicit TaggedGraph(const Graph::IGraphUs &);
    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(const IGraphCompare &) const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;

    std::weak_ordering operator<=>(const TaggedGraph &) const;
    bool operator==(const TaggedGraph &) const;

    const Grouping<Graph::Vertex> &getVertexGrouping() const;

  private:
    std::unique_ptr<Comparers> m_compare;
};

class CompareTaggedGraphFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
