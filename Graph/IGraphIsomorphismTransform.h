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
class XGraph;

class ITransform
{
  public:
    enum Type : TagEntry
    {
        Failure,
        Known,
        Disconnected,
        ComplementDisconnected,
        ComplementKnown,
        OmitEdges,
    };

    static constexpr size_t numType = 6;

    virtual ~ITransform() = default;

    const Graph::IGraphUs &getGraph() const;
    const XGraph &getTaggedGraphs() const;
    const std::shared_ptr<XGraph> getTaggedGraphsPtr() const;

    virtual const Tag &getTagOfTransform() const = 0;
    virtual std::string getDescription() const = 0;
    virtual const std::vector<std::shared_ptr<XGraph>> &getChildren() const = 0;

    static std::unique_ptr<ITransform> Create(const std::shared_ptr<XGraph> &);

  protected:
    explicit ITransform(std::shared_ptr<XGraph>);

  private:
    std::shared_ptr<XGraph> m_taggedGraphs;
};

class TransformKnown : public ITransform
{
  public:
    static std::unique_ptr<TransformKnown> tryCreate(const std::shared_ptr<XGraph> &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<std::shared_ptr<XGraph>> &getChildren() const override;

  private:
    TransformKnown(const std::shared_ptr<XGraph> &, TaggerKnown);

    TaggerKnown m_taggerKnown;
    Tag m_tag;
};

class TransformFailure : public ITransform
{
  public:
    TransformFailure(const std::shared_ptr<XGraph> &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<std::shared_ptr<XGraph>> &getChildren() const override;

  private:
    Tag m_tag;
};

class TransformDisconnected : public ITransform
{
  public:
    static std::unique_ptr<TransformDisconnected> tryCreate(const std::shared_ptr<XGraph> &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<std::shared_ptr<XGraph>> &getChildren() const override;

    static std::vector<std::vector<Graph::Vertex>> getComponentsJoinSingletons(const Graph::IGraphUs &);

  private:
    TransformDisconnected(const std::shared_ptr<XGraph> &, const std::vector<std::vector<Graph::Vertex>> &);

    Tag m_tag;
    std::vector<std::unique_ptr<Graph::SubGraph>> m_components;
    std::vector<std::shared_ptr<XGraph>> m_taggedGraphs;
};
;

class TransformComplementDisconnected : public ITransform
{
  public:
    static std::unique_ptr<TransformComplementDisconnected> tryCreate(const std::shared_ptr<XGraph> &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<std::shared_ptr<XGraph>> &getChildren() const override;

    static std::vector<std::vector<Graph::Vertex>> getComponentsJoinSingletons(const Graph::IGraphUs &);

  private:
    TransformComplementDisconnected(const std::shared_ptr<XGraph> &,
                                    const std::vector<std::vector<Graph::Vertex>> &);

    Tag m_tag;
    std::vector<std::unique_ptr<Graph::SubGraph>> m_components;
    std::vector<std::shared_ptr<XGraph>> m_taggedGraphs;
};

class TransformComplementKnown : public ITransform
{
  public:
    static std::unique_ptr<TransformComplementKnown> tryCreate(const std::shared_ptr<XGraph> &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<std::shared_ptr<XGraph>> &getChildren() const override;

  private:
    TransformComplementKnown(const std::shared_ptr<XGraph> &, const TaggerKnown &tagger);

    TaggerKnown m_taggerKnown;
    Tag m_tag;
};

class TransformOmitEdges : public ITransform
{
  public:
    static std::unique_ptr<TransformOmitEdges> tryCreate(const std::shared_ptr<XGraph> &);

    const Tag &getTagOfTransform() const override;
    std::string getDescription() const override;
    const std::vector<std::shared_ptr<XGraph>> &getChildren() const override;

  private:
    TransformOmitEdges(const std::shared_ptr<XGraph> &, const std::vector<std::vector<Graph::Vertex>> &);

    Graph::Edge m_numOmittedEdges = 0;
    Tag m_tag;
    std::unique_ptr<Graph::UndirectedGraph> m_child;
    std::vector<std::shared_ptr<XGraph>> m_taggedGraphs;
};

} // namespace GraphIsomorphism
