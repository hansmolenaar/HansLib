#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphUs.h"
#include "SubGraphConnected.h"

namespace GraphIsomorphism
{

using GraphTags = std::vector<Tag>;

class IDecompose
{
  public:
    virtual ~IDecompose() = default;

    virtual GraphVertex getVertexInParent(GraphVertex) const = 0;

    virtual const Graph::IGraphUs &getSelf() const = 0;
    virtual const GraphTags &getTagSelf() const = 0;

    // Can be empty: leaf of tree
    virtual const std::vector<GraphTags> &getChildTags() const = 0;
    virtual std::vector<const IDecompose *> getChildren(const GraphTags &) const = 0;

    bool isLeaf() const;

    static GraphTags GetGraphTags(const Graph::IGraphUs &);
    static std::unique_ptr<IDecompose> Create(const Graph::IGraphUs &);

    // Root has nullptr as parent
    using ToParentMap = std::map<const IDecompose *, const IDecompose *>;
    static ToParentMap GetToParentMap(const IDecompose *);
};

class DecomposeDisconnected : public IDecompose
{
  public:
    explicit DecomposeDisconnected(const Graph::IGraphUs &);
    GraphVertex getVertexInParent(GraphVertex) const override;

    const Graph::IGraphUs &getSelf() const override;
    const GraphTags &getTagSelf() const override;

    // Can be empty: leaf of tree
    const std::vector<GraphTags> &getChildTags() const override;
    std::vector<const IDecompose *> getChildren(const GraphTags &) const override;

  private:
    const Graph::IGraphUs &m_self;
    GraphTags m_tagSelf;
    std::vector<Graph::SubGraphConnected> m_children;
    std::vector<GraphTags> m_childTags;
    std::map<GraphTags, std::vector<std::unique_ptr<IDecompose>>> m_childDecomposes;
};

class DecomposeVertexFullyConnected : public IDecompose
{
  public:
    explicit DecomposeVertexFullyConnected(const Graph::IGraphUs &, const std::set<GraphVertex> &);
    GraphVertex getVertexInParent(GraphVertex) const override;

    const Graph::IGraphUs &getSelf() const override;
    const GraphTags &getTagSelf() const override;

    // Can be empty: leaf of tree
    const std::vector<GraphTags> &getChildTags() const override;
    std::vector<const IDecompose *> getChildren(const GraphTags &) const override;

    const std::vector<GraphVertex> &getFullyConnectedVerticesInParent() const;

  private:
    const Graph::IGraphUs &m_self;
    GraphTags m_tagSelf;
    std::vector<std::unique_ptr<Graph::IGraphUs>> m_children;
    std::vector<GraphTags> m_childTags;
    std::map<GraphTags, std::vector<std::unique_ptr<IDecompose>>> m_childDecomposes;
};

class DecomposeLeaf : public IDecompose
{
  public:
    explicit DecomposeLeaf(const Graph::IGraphUs &);
    GraphVertex getVertexInParent(GraphVertex) const override;

    const Graph::IGraphUs &getSelf() const override;
    const GraphTags &getTagSelf() const override;

    // Can be empty: leaf of tree
    const std::vector<GraphTags> &getChildTags() const override;
    std::vector<const IDecompose *> getChildren(const GraphTags &) const override;

  private:
    const Graph::IGraphUs &m_self;
    GraphTags m_tagSelf;
    std::vector<GraphTags> m_childTags;
};

} // namespace GraphIsomorphism
