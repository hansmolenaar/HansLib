#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouper.h"
#include "GraphIsomorphismStatus.h"
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
    std::vector<Grouper> m_vertexGroupers;
    std::vector<Tag> m_vertexGroupTags; // Sorted
    std::map<Tag, GraphVertex> m_uniqueVertexAndGroupTag;
};

} // namespace GraphIsomorphism
