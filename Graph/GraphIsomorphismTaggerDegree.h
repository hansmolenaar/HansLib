#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUS.h"

namespace GraphIsomorphism
{

class TaggerDegree : public IGraphTagger, public IVertexTagger
{
  public:
    explicit TaggerDegree(const Graph::IGraphUS &);

    const Tag &getGraphTag() const override;
    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;

  private:
    const Graph::IGraphUS &m_graph;
    Tag m_degreeSequenceTag;
};

class TaggerDegreeFactory : public IGraphTaggerFactory
{
  public:
    std::unique_ptr<IGraphTagger> createGraphTagger(const Graph::IGraphUS &) override;
};

} // namespace GraphIsomorphism
