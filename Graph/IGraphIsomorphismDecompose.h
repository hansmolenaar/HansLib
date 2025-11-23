#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "IGraphUs.h"
#include "SubGraphConnected.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{
class ToParentMap;

class IDecompose
{
  public:
    virtual ~IDecompose() = default;

    const Graph::IGraphUs &getGraph() const;
    const TaggedGraph &getTaggedGraph() const;
    virtual const Tag &getTag() const = 0;
    virtual const Grouping<const IDecompose *> &getGroupingChildren() const = 0;

    std::weak_ordering operator<=>(const IDecompose &) const;
    bool isLeaf() const;
    std::string getName() const;

    static std::unique_ptr<IDecompose> Create(const Graph::IGraphUs &, bool = true);

    static Grouping<const IDecompose *> CreateGrouping(const std::vector<const IDecompose *> &);

  protected:
    explicit IDecompose(const Graph::IGraphUs &);

  private:
    std::unique_ptr<TaggedGraph> m_taggedGraph;
};

class DecomposeDisconnected : public IDecompose
{
  public:
    explicit DecomposeDisconnected(const Graph::IGraphUs &);

    const Tag &getTag() const override;

    // Can be empty: leaf of tree
    const Grouping<const IDecompose *> &getGroupingChildren() const override;

  private:
    Tag m_tag;
    std::vector<Graph::SubGraphConnected> m_children;
    std::vector<std::unique_ptr<IDecompose>> m_childDecomposes;
    Grouping<const IDecompose *> m_groupingChildren;
};

class DecomposeVertexFullyConnected : public IDecompose
{
  public:
    explicit DecomposeVertexFullyConnected(const Graph::IGraphUs &, const std::set<GraphVertex> &);

    const Tag &getTag() const override;

    // Can be empty: leaf of tree
    const Grouping<const IDecompose *> &getGroupingChildren() const override;

    const std::vector<GraphVertex> &getFullyConnectedVerticesInParent() const;

  private:
    Tag m_tag;
    std::vector<std::unique_ptr<Graph::IGraphUs>> m_children;
    std::vector<std::unique_ptr<IDecompose>> m_childDecomposes;
    Grouping<const IDecompose *> m_groupingChildren;
};

class DecomposeLeaf : public IDecompose
{
  public:
    explicit DecomposeLeaf(const Graph::IGraphUs &);

    const Tag &getTag() const override;

    // Is empty
    const Grouping<const IDecompose *> &getGroupingChildren() const override;

  private:
    Tag m_tag;
    const Grouping<const IDecompose *> m_groupingChildren;
};

class DecomposeKnown : public IDecompose
{
  public:
    DecomposeKnown(const Graph::IGraphUs &, Tag);
    static std::unique_ptr<IDecompose> tryCreate(const Graph::IGraphUs &);

    const Tag &getTag() const override;

    // Is empty
    const Grouping<const IDecompose *> &getGroupingChildren() const override;

  private:
    Tag m_tag;
    const Grouping<const IDecompose *> m_groupingChildren;
};

class DecomposeComplement : public IDecompose
{
  public:
    DecomposeComplement(std::unique_ptr<Graph::UndirectedGraph> &&graph, const Graph::IGraphUs &org);
    static std::unique_ptr<IDecompose> Create(const Graph::IGraphUs &);

    const Tag &getTag() const override;
    const Graph::IGraphUs &getOriginal() const;

    // Is empty
    const Grouping<const IDecompose *> &getGroupingChildren() const override;

  private:
    const Graph::IGraphUs &m_original;
    std::unique_ptr<Graph::UndirectedGraph> m_complement;
    Tag m_tag;
    std::unique_ptr<IDecompose> m_child; // only child?
    Grouping<const IDecompose *> m_groupingChildren;
};

class ToParentMap
{
  public:
    explicit ToParentMap(const IDecompose *);
    bool isRoot(const IDecompose *) const; // Root has nullptr as parent
    const IDecompose *getRoot() const;
    const IDecompose *getRoot(const IDecompose *) const;
    const IDecompose *getParent(const IDecompose *) const;
    std::vector<const IDecompose *> getLeaves() const;
    size_t size() const;
    GraphVertex getVertexInRoot(GraphVertex vertex, const IDecompose *) const;

    std::vector<Tag> collectDecomposeTagsForLeaf(const IDecompose *) const;

    std::weak_ordering compareLeaves(const IDecompose *, const IDecompose *) const;
    GraphIsomorphism::Grouping<const IDecompose *> groupLeaves() const;

    std::weak_ordering operator<=>(const ToParentMap &) const;

  private:
    std::map<const IDecompose *, const IDecompose *> m_toParent;
};

} // namespace GraphIsomorphism
