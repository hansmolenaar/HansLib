#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouper.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggedGraph
{
  public:
    explicit TaggedGraph(const Graph::IGraphUs &);

  private:
    const Graph::IGraphUs &m_graph;
    std::vector<std::unique_ptr<ITagger>> m_taggers;
    GraphTags m_graphTags; // For quick testing
    std::vector<Grouper> m_vertexGroupers;
};

} // namespace GraphIsomorphism
