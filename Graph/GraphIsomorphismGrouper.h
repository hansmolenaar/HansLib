#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "UndirectedGraph.h"

class UndirectedGraph;

namespace GraphIsomorphism
{

class Grouper
{
  public:
    explicit Grouper(const ITagger &tagger);
    const std::vector<Tag> &getTags() const;
    // Returns empty list if not found
    const std::vector<GraphVertex> &getGroupMembers(const Tag &) const;
    int countUnique() const;
    bool isResolved() const;

  private:
    const UndirectedGraph &m_graph;
    std::vector<Tag> m_tags;
    // Same order as m_tags
    std::vector<std::vector<GraphVertex>> m_groups;
};

} // namespace GraphIsomorphism
