#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "SubGraph.h"

namespace Graph
{
class IGraphUs;
}

namespace GraphIsomorphism
{

class TaggedGraph;

class ITransform
{
  public:
    enum Type : TagEntry
    {
        Failure,
        Known,
        Disconnected,
    };

    virtual ~ITransform() = default;

    const Graph::IGraphUs &getGraph() const;
    const TaggedGraph &getTaggedGraph() const;
    const std::shared_ptr<TaggedGraph> getTaggedGraphPtr() const;

    virtual const Tag &getTagOfTransform() const = 0;
    virtual std::string getDescription() const = 0;
    virtual const std::vector<std::shared_ptr<TaggedGraph>> &getChildren() const = 0;

    static std::unique_ptr<ITransform> Create(const std::shared_ptr<TaggedGraph> &);

  protected:
    explicit ITransform(std::shared_ptr<TaggedGraph>);

  private:
    std::shared_ptr<TaggedGraph> m_taggedGraph;
};

class TransformKnown : public ITransform
{
  public:
    static std::unique_ptr<TransformKnown> tryCreate(const std::shared_ptr<TaggedGraph> &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<std::shared_ptr<TaggedGraph>> &getChildren() const override;

  private:
    TransformKnown(const std::shared_ptr<TaggedGraph> &, TaggerKnown);

    TaggerKnown m_taggerKnown;
    Tag m_tag;
};

class TransformFailure : public ITransform
{
  public:
    TransformFailure(const std::shared_ptr<TaggedGraph> &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<std::shared_ptr<TaggedGraph>> &getChildren() const override;

  private:
    Tag m_tag;
};

class TransformDisconnected : public ITransform
{
  public:
    static std::unique_ptr<TransformDisconnected> tryCreate(const std::shared_ptr<TaggedGraph> &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<std::shared_ptr<TaggedGraph>> &getChildren() const override;

    static std::vector<std::vector<Graph::Vertex>> getComponentsJoinSingletons(const Graph::IGraphUs &);

  private:
    TransformDisconnected(const std::shared_ptr<TaggedGraph> &, const std::vector<std::vector<Graph::Vertex>> &);

    Tag m_tag;
    std::vector<std::unique_ptr<Graph::SubGraph>> m_components;
    std::vector<std::shared_ptr<TaggedGraph>> m_taggedGraphs;
};

} // namespace GraphIsomorphism
