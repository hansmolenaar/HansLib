#pragma once

#include "GraphIsomorphismStatus.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerKnown : public IGraphTagger
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

    const Tag &getGraphTag() const override;
    std::string getDescription() const;

  private:
    Tag m_tag;
    std::string m_description;
};

class TaggerKnownFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
