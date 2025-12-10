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
    virtual std::string getDescription() const = 0;
    virtual const Grouping<const IDecompose *> &getGroupingChildren() const = 0;
    virtual Graph::Vertex getVertexInParent(Graph::Vertex vertex) const = 0;

    std::weak_ordering operator<=>(const IDecompose &) const;
    bool isLeaf() const;
    std::string getGraphName() const;

    static std::unique_ptr<IDecompose> Create(const Graph::IGraphUs &);

    static Grouping<const IDecompose *> CreateGrouping(const std::vector<const IDecompose *> &);

  protected:
    explicit IDecompose(const Graph::IGraphUs &);
    explicit IDecompose(std::shared_ptr<TaggedGraph>);

  private:
    std::shared_ptr<TaggedGraph> m_taggedGraph;
};

class DecomposeDisconnected : public IDecompose
{
  public:
    explicit DecomposeDisconnected(const Graph::IGraphUs &);

    const Tag &getTag() const override;
    std::string getDescription() const override;
    Graph::Vertex getVertexInParent(Graph::Vertex vertex) const override;

    // Can be empty: leaf of tree
    const Grouping<const IDecompose *> &getGroupingChildren() const override;

  private:
    Tag m_tag;
    std::vector<Graph::SubGraphConnected> m_children;
    std::vector<std::unique_ptr<IDecompose>> m_childDecomposes;
    Grouping<const IDecompose *> m_groupingChildren;
};

class DecomposeUniversalVertex : public IDecompose
{
  public:
    explicit DecomposeUniversalVertex(const Graph::IGraphUs &, const std::set<Graph::Vertex> &);

    const Tag &getTag() const override;
    std::string getDescription() const override;
    Graph::Vertex getVertexInParent(Graph::Vertex vertex) const override;

    // Can be empty: leaf of tree
    const Grouping<const IDecompose *> &getGroupingChildren() const override;

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
    std::string getDescription() const override;
    Graph::Vertex getVertexInParent(Graph::Vertex vertex) const override;

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
    std::string getDescription() const override;
    Graph::Vertex getVertexInParent(Graph::Vertex vertex) const override;

    // Is empty
    const Grouping<const IDecompose *> &getGroupingChildren() const override;

  private:
    Tag m_tag;
    const Grouping<const IDecompose *> m_groupingChildren;
};

class DecomposeComplementKnown : public IDecompose
{
  public:
    static std::unique_ptr<IDecompose> tryCreate(const Graph::IGraphUs &,
                                                 const std::shared_ptr<Graph::UndirectedGraph> &);

    const Tag &getTag() const override;
    std::string getDescription() const override;
    const Grouping<const IDecompose *> &getGroupingChildren() const override;
    Graph::Vertex getVertexInParent(Graph::Vertex vertex) const override;

  private:
    DecomposeComplementKnown(const Graph::IGraphUs &, std::shared_ptr<Graph::UndirectedGraph>);

    std::shared_ptr<Graph::UndirectedGraph> m_complement;
    Tag m_tag;
    std::unique_ptr<IDecompose> m_child;
    const Grouping<const IDecompose *> m_groupingChildren;
};

class DecomposeComplementDisconnected : public IDecompose
{
  public:
    static std::unique_ptr<IDecompose> tryCreate(const Graph::IGraphUs &,
                                                 const std::shared_ptr<Graph::UndirectedGraph> &);

    const Tag &getTag() const override;
    std::string getDescription() const override;
    const Grouping<const IDecompose *> &getGroupingChildren() const override;
    Graph::Vertex getVertexInParent(Graph::Vertex vertex) const override;

  private:
    DecomposeComplementDisconnected(const Graph::IGraphUs &, std::shared_ptr<Graph::UndirectedGraph>);

    std::unique_ptr<Graph::UndirectedGraph> m_edgesReduced;
    Tag m_tag;
    std::unique_ptr<DecomposeDisconnected> m_child;
    const Grouping<const IDecompose *> m_groupingChildren;
};

class DecomposeOmitEdges : public IDecompose
{
  public:
    static std::unique_ptr<IDecompose> tryCreate(const Graph::IGraphUs &);

    const Tag &getTag() const override;
    std::string getDescription() const override;
    const Grouping<const IDecompose *> &getGroupingChildren() const override;
    Graph::Vertex getVertexInParent(Graph::Vertex vertex) const override;
    Graph::Edge getNumOmittedEdges() const;

  private:
    DecomposeOmitEdges(const Graph::IGraphUs &);

    Graph::Edge m_numOmittedEdges = 0;
    Tag m_tag;
    std::unique_ptr<Graph::UndirectedGraph> m_child;
    std::unique_ptr<IDecompose> m_childDecomposes;
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
    Graph::Vertex getVertexInRoot(Graph::Vertex vertex, const IDecompose *) const;

    std::vector<Tag> collectDecomposeTagsForLeaf(const IDecompose *) const;
    std::string getDescriptions() const;

    std::weak_ordering compareLeaves(const IDecompose *, const IDecompose *) const;
    GraphIsomorphism::Grouping<const IDecompose *> groupLeaves() const;

    std::weak_ordering operator<=>(const ToParentMap &) const;

  private:
    std::map<const IDecompose *, const IDecompose *> m_toParent;
};

} // namespace GraphIsomorphism
