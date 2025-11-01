#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUS.h"

namespace GraphIsomorphism
{

class TaggerDegree : public IGraphTagger
{
  public:
    explicit TaggerDegree(const Graph::IGraphUS &);

    const Tag &getTag() const override;

  private:
    const Graph::IGraphUS &m_graph;
    Tag m_degreeSequenceTag;
};

class TaggerDegreeFactory : public IGraphTaggerFactory
{
  public:
    std::unique_ptr<IGraphTagger> create(const Graph::IGraphUS &) override;
};

} // namespace GraphIsomorphism
