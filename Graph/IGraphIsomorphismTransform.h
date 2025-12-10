#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"

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

    static std::unique_ptr<ITransform> Create(const TaggedGraph &);

  protected:
    explicit ITransform(const TaggedGraph &);

  private:
    const TaggedGraph &m_taggedGraph;
};

class TransformKnown : public ITransform
{
  public:
    static std::unique_ptr<ITransform> tryCreate(const TaggedGraph &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<const TaggedGraph *> &getChildren() const override;

  private:
    TransformKnown(const TaggedGraph &, const TaggerKnown &);

    Tag m_tag;
};

} // namespace GraphIsomorphism
