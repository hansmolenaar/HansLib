#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismStatus.h"
#include "IGraphIsomorphismTagger.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{

class VertexGrouper
{
  public:
    explicit VertexGrouper(const IVertexTagger &tagger);
    const std::vector<Tag> &getTags() const;
    // Returns empty list if not found
    const std::vector<GraphVertex> &getGroupMembers(const Tag &) const;
    int countUnique() const;
    bool isResolved() const;

    std::weak_ordering operator<=>(const GraphIsomorphism::VertexGrouper &) const;
    bool operator==(const GraphIsomorphism::VertexGrouper &) const;

    GraphVertex getNumVertices() const;

  private:
    GraphVertex m_numVertices;
    GraphIsomorphism::TaggedGrouping<Tag, GraphVertex> m_grouping;
};

} // namespace GraphIsomorphism
