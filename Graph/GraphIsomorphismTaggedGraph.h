#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismStatus.h"
#include "GraphIsomorphismVertexComparers.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggedGraph
{
  public:
    explicit TaggedGraph(const Graph::IGraphUs &);
    const Graph::IGraphUs &getGraph() const;

    std::weak_ordering operator<=>(const TaggedGraph &) const;
    bool operator==(const TaggedGraph &) const;

    static Status tryConnect(const TaggedGraph &, const TaggedGraph &);

  private:
    const Graph::IGraphUs &m_graph;
    std::vector<std::unique_ptr<ITagger>> m_taggers;
    GraphTags m_graphTags; // For quick testing
    VertexComparers m_vertexComparers;
    Grouping<GraphVertex> m_grouping;
};

} // namespace GraphIsomorphism
