#pragma once

#include "IGraphIsomorphismTransform.h"

namespace GraphIsomorphism
{

class DecomposeNode
{
  public:
    const Graph::IGraphUs &getGraph() const;
    const TaggedGraph &getTaggedGraph() const;
    const Tag &getTag() const;
    std::string getDescription() const;
    const Grouping<const DecomposeNode *> &getGroupingChildren() const;
    Graph::Vertex getVertexInParent(Graph::Vertex vertex) const;

    std::weak_ordering operator<=>(const DecomposeNode &) const;
    bool isLeaf() const;

    static std::unique_ptr<DecomposeNode> Create(const std::shared_ptr<TaggedGraph> &);

  private:
    DecomposeNode(std::unique_ptr<ITransform> &&);

    std::unique_ptr<ITransform> m_transform;
    std::vector<std::unique_ptr<DecomposeNode>> m_childDecomposes;
    Grouping<const DecomposeNode *> m_groupingChildren;
    Tag m_tag;
    std::string m_description;
};

} // namespace GraphIsomorphism
