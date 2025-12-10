#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismTaggerKnown.h"

namespace Graph
{
class IGraphUs;
}

namespace GraphIsomorphism
{

class TaggedGraph;
class IGraphUs;
class TaggerKnown;

class ITransform
{
  public:
    enum Type : TagEntry
    {
        Known,
    };

    virtual ~ITransform() = default;

    const Graph::IGraphUs &getGraph() const;
    const TaggedGraph &getTaggedGraph() const;

    virtual const Tag &getTagOfTransform() const = 0;
    virtual std::string getDescription() const = 0;
    virtual const std::vector<const TaggedGraph *> &getChildren() const = 0;

    static std::unique_ptr<ITransform> Create(const std::shared_ptr<TaggedGraph> &);

  protected:
    explicit ITransform(std::shared_ptr<TaggedGraph>);

  private:
    std::shared_ptr<TaggedGraph> m_taggedGraph;
};

class TransformKnown : public ITransform
{
  public:
    static std::unique_ptr<ITransform> tryCreate(const std::shared_ptr<TaggedGraph> &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<const TaggedGraph *> &getChildren() const override;

  private:
    TransformKnown(const std::shared_ptr<TaggedGraph> &, TaggerKnown);

    TaggerKnown m_taggerKnown;
    Tag m_tag;
};

} // namespace GraphIsomorphism
