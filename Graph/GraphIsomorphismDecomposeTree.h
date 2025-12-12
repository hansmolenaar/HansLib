#pragma once

#include "GraphIsomorphismDecomposeNode.h"

namespace GraphIsomorphism
{

class DecomposeTree
{
  public:
    DecomposeTree(const Graph::IGraphUs &);
    DecomposeTree(std::unique_ptr<DecomposeNode> &&);
    const DecomposeNode &getRoot() const;
    std::vector<const DecomposeNode *> getLeaves() const;
    size_t size() const;
    Graph::Vertex getVertexInRoot(Graph::Vertex, const DecomposeNode &) const;
    std::vector<std::string> getDescriptions() const;

    std::weak_ordering compareLeaves(const DecomposeNode *, const DecomposeNode *) const;
    GraphIsomorphism::Grouping<const DecomposeNode *> groupLeaves() const;
    std::weak_ordering operator<=>(const DecomposeTree &) const;

    const DecomposeNode *getParent(const DecomposeNode *) const;
    std::vector<Tag> collectDecomposeTagsForLeaf(const DecomposeNode *) const;

  private:
    std::unique_ptr<DecomposeNode> m_root;

    std::map<const DecomposeNode *, const DecomposeNode *> m_toParent;
};

} // namespace GraphIsomorphism
