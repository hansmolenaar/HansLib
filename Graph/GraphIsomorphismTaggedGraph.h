#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismStatus.h"
#include "GraphIsomorphismVertexComparers.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggedGraph : public IGraphCompare
{
  public:
    explicit TaggedGraph(const Graph::IGraphUs &);
    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(const IGraphCompare &) const override;

    std::weak_ordering operator<=>(const TaggedGraph &) const;
    bool operator==(const TaggedGraph &) const;

    const Grouping<Graph::Vertex> &getVertexGrouping() const;

  private:
    const Graph::IGraphUs &m_graph;
    std::vector<std::unique_ptr<ICompare>> m_comparers;
    VertexComparers m_vertexComparers;
    Grouping<Graph::Vertex> m_grouping;
};

class CompareTaggedGraphFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
