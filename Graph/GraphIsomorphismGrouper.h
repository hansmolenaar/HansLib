#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{

class Grouper
{
  public:
    explicit Grouper(const IVertexTagger &tagger);
    const std::vector<Tag> &getTags() const;
    // Returns empty list if not found
    const std::vector<GraphVertex> &getGroupMembers(const Tag &) const;
    int countUnique() const;
    bool isResolved() const;

    // Equivalent if same tags, and same number of vertices per tag
    static bool areEquivalent(const Grouper &, const Grouper &);

    void updateVertexGroupTags(std::vector<Tag> &) const;

  private:
    std::vector<Tag> m_tags;
    // Same order as m_tags
    std::vector<std::vector<GraphVertex>> m_groups;
};

} // namespace GraphIsomorphism
