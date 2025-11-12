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
    static std::vector<std::array<GraphVertex, 3>> getAllTriangles(const Graph::IGraphUs &);

  private:
    Tag m_graphTag;
};

class TaggerTrianglesFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
