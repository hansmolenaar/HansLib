#pragma once

#include "GraphIsomorphismDecomposeNode.h"

namespace GraphIsomorphism
{

class DecomposeTree : public IGraphCompare
{
  public:
    explicit DecomposeTree(const Graph::IGraphUs &);
    explicit DecomposeTree(std::unique_ptr<DecomposeNode> &&);

    std::weak_ordering compareGraph(const IGraphCompare &) const override;
    const VertexGrouping &getVertexGrouping() const;
    const Graph::IGraphUs &getGraph() const override;

    const DecomposeNode &getRoot() const;
    std::vector<const DecomposeNode *> getLeaves() const;
    size_t size() const;
    Graph::Vertex getVertexInRoot(Graph::Vertex, const DecomposeNode &) const;
    std::vector<std::string> getDescriptions() const;

    std::weak_ordering compareLeaves(const DecomposeNode *, const DecomposeNode *) const;
    GraphIsomorphism::Grouping<const DecomposeNode *> groupLeaves() const;

    const DecomposeNode *getParent(const DecomposeNode *) const;
    std::vector<Tag> collectDecomposeTagsForLeaf(const DecomposeNode *) const;

  private:
    std::unique_ptr<DecomposeNode> m_root;

    std::map<const DecomposeNode *, const DecomposeNode *> m_toParent;
};

class DecomposeTreeFactory : public ICompareFactory
{
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
