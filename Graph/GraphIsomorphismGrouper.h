#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismStatus.h"
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

    std::weak_ordering operator<=>(const GraphIsomorphism::Grouper &) const;
    bool operator==(const GraphIsomorphism::Grouper &) const;

    GraphVertex getNumVertices() const;

  private:
    GraphVertex m_numVertices;
    std::vector<Tag> m_tags;
    // Same order as m_tags
    std::vector<std::vector<GraphVertex>> m_groups;
};

} // namespace GraphIsomorphism
