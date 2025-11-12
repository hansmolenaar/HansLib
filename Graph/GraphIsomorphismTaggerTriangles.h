#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerTriangles : public IGraphTagger
{
  public:
    explicit TaggerTriangles(const Graph::IGraphUs &);

    const Tag &getGraphTag() const override;
    const Tag &getVertexTag(GraphVertex) const;
    GraphVertex getNumVertices() const;

    static std::vector<std::array<GraphVertex, 3>> getAllTriangles(const Graph::IGraphUs &);

  private:
    Tag m_graphTag;
    std::vector<size_t> m_countPerVertex;
};

class TaggerTrianglesFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
