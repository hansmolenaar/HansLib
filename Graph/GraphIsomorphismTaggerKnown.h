#pragma once

#include "GraphIsomorphismStatus.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerKnown : public IGraphCompare
{
  public:
    enum KnownType : TagEntry
    {
        Unknown,
        Complete,
        Cycle,
        Path,
        Singletons, // More than 1
    };

    explicit TaggerKnown(const Graph::IGraphUs &);

    std::weak_ordering compareGraph(const IGraphCompare &) const override;
    const Graph::IGraphUs &getGraph() const override;
    const Tag &getGraphTag() const;
    std::string getDescription() const;

  private:
    const Graph::IGraphUs &m_graph;
    Tag m_tag;
    std::string m_description;
};

class CompareKnownFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
