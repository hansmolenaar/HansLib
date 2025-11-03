#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUSC.h"

namespace GraphIsomorphism
{

class TaggerMaxDegree : public IVertexTagger
{
  public:
    explicit TaggerMaxDegree(const Graph::IGraphUSC &);

    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;

  private:
    GraphVertex m_numVertices;
    std::map<GraphVertex, TagEntry> m_fullyConnected2id;
    ;
};

class TaggerMaxDegreeFactory : public IVertexTaggerFactory
{
  public:
    std::unique_ptr<IVertexTagger> createVertexTagger(const Graph::IGraphUSC &) override;
};

} // namespace GraphIsomorphism
