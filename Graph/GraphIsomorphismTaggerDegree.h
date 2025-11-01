#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUS.h"

namespace GraphIsomorphism
{

class TaggerDegree : public ITaggeR
{
  public:
    explicit TaggerDegree(const Graph::IGraphUS &);

    bool isGraphTagger() const override;
    const Tag &getGraphTag() const override;

    bool isVertexTagger() const override;
    const Tag &getVertexTag(GraphVertex) const override;

  private:
    const Graph::IGraphUS &m_graph;
    Tag m_degreeSequenceTag;
};

class TaggerDegreeFactory : public ITaggerUSFactory
{
  public:
    std::unique_ptr<ITaggeR> create(const Graph::IGraphUS &) override;
};

} // namespace GraphIsomorphism
