#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

class UndirectedGraph;

namespace GraphIsomorphism
{

class TaggerDegree : public ITagger
{
  public:
    explicit TaggerDegree(const UndirectedGraph &);
    const UndirectedGraph &getGraph() const override;
    Tag getTag(GraphVertex) const override;

  private:
    const UndirectedGraph &m_graph;
};

class TaggerDegreeFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> create(const UndirectedGraph &) override;
};

} // namespace GraphIsomorphism
