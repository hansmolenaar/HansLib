#pragma once

#include "GraphIsomorphismXGraph.h"
#include "IGraphIsomorphismTransform.h"

namespace GraphIsomorphism
{

class DecomposeNode : public IGraphCompare
{
  public:
    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareGraph(const IGraphCompare &) const override;
    const VertexGrouping &getVertexGrouping() const;

    const Tag &getTag() const;
    std::string getDescription() const;
    const Grouping<const DecomposeNode *> &getGroupingChildren() const;
    Graph::Vertex getVertexInParent(Graph::Vertex vertex) const;
    bool isLeaf() const;

    static std::unique_ptr<DecomposeNode> Create(const std::shared_ptr<XGraph> &);

  private:
    DecomposeNode(std::unique_ptr<ITransform> &&);
    const XGraph &getXGraph() const;

    std::unique_ptr<ITransform> m_transform;
    std::vector<std::unique_ptr<DecomposeNode>> m_childDecomposes;
    Grouping<const DecomposeNode *> m_groupingChildren;
    Tag m_tag;
    std::string m_description;
};

class DecomposeNodeFactory : public IGraphCompareFactory
{
    std::unique_ptr<IGraphCompare> createGraphCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
