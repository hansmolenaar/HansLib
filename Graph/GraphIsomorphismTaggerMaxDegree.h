#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class TaggerMaxDegree : public IVertexTagger
{
  public:
    explicit TaggerMaxDegree(const Graph::IGraphUsc &);

    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;

  private:
    GraphVertex m_numVertices;
    std::map<GraphVertex, TagEntry> m_fullyConnected2id;
};

class TaggerMaxDegreeFactory : public IVertexTaggerFactory
{
  public:
    std::unique_ptr<IVertexTagger> createVertexTagger(const Graph::IGraphUsc &) override;
};

} // namespace GraphIsomorphism
