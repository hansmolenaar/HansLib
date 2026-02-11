#include "GraphIsomorphismDecomposeNode.h"
#include "SubGraph.h"

#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
} // namespace

DecomposeNode::DecomposeNode(std::unique_ptr<ITransform> &&transform) : m_transform(std::move(transform))
{
    std::vector<const DecomposeNode *> allChildren;
    const auto &children = m_transform->getChildren();
    for (const auto &child : children)
    {
        auto childTransform = Create(child);
        MyAssert(!!childTransform);
        m_childDecomposes.emplace_back(std::move(childTransform));
        allChildren.push_back(m_childDecomposes.back().get());
    }

    auto compare = [](const DecomposeNode *child1, const DecomposeNode *child2) {
        return child1->compareGraph(*child2) == std::weak_ordering::less;
    };
    m_groupingChildren = Grouping<const DecomposeNode *>(allChildren, compare);
}

std::unique_ptr<DecomposeNode> DecomposeNode::Create(const std::shared_ptr<XGraph> &xgraph)
{
    return std::unique_ptr<DecomposeNode>(new DecomposeNode(ITransform::Create(xgraph)));
}

const XGraph &DecomposeNode::getXGraph() const
{
    return m_transform->getXGraph();
}

const Graph::IGraphUs &DecomposeNode::getGraph() const
{
    return m_transform->getGraph();
}

const Tag &DecomposeNode::getTag() const
{
    return m_transform->getTagOfTransform();
}

std::string DecomposeNode::getDescription() const
{
    return m_transform->getDescription();
}

bool DecomposeNode::isLeaf() const
{
    return m_childDecomposes.empty();
}

Graph::Vertex DecomposeNode::getVertexInParent(Graph::Vertex vertex) const
{
    const auto *subGraph = dynamic_cast<const SubGraph *>(&getGraph());
    if (subGraph != nullptr)
    {
        return subGraph->getVertexInParent(vertex);
    }
    return vertex;
}

const Grouping<const DecomposeNode *> &DecomposeNode::getGroupingChildren() const
{
    return m_groupingChildren;
}

std::weak_ordering DecomposeNode::compareGraph(const IGraphCompare &other) const
{
    const DecomposeNode &lhs = *this;
    const DecomposeNode &rhs = dynamic_cast<const DecomposeNode &>(other);
    std::weak_ordering result = lhs.getGroupingChildren().getGroupSizes() <=> rhs.getGroupingChildren().getGroupSizes();
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    result = lhs.getTag() <=> rhs.getTag();
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    result = lhs.getXGraph().compareGraph((rhs.getXGraph()));
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    if (lhs.isLeaf())
    {
        MyAssert(rhs.isLeaf());
    }
    else
    {
        const auto &groups0 = lhs.getGroupingChildren()();
        const auto &groups1 = rhs.getGroupingChildren()();
        MyAssert(groups0.size() == groups1.size());
        for (size_t n : Iota::GetRange(groups0.size()))
        {
            // No recursion here
            result = groups0.at(n).front()->getTag() <=> groups1.at(n).front()->getTag();
            if (result != std::weak_ordering::equivalent)
            {
                return result;
            }
        }
    }
    return result;
}

const VertexGrouping &DecomposeNode::getVertexGrouping() const
{
    return getXGraph().getVertexGrouping();
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Factory

std::unique_ptr<IGraphCompare> DecomposeNodeFactory::createGraphCompare(const Graph::IGraphUs &graph) const
{
    return DecomposeNode::Create(std::make_shared<XGraph>(graph));
}
