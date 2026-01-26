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

    auto compare = [](const DecomposeNode *child1, const DecomposeNode *child2) { return *child1 < *child2; };
    m_groupingChildren = Grouping<const DecomposeNode *>(allChildren, compare);
}

std::unique_ptr<DecomposeNode> DecomposeNode::Create(const std::shared_ptr<XGraph> &xgraph)
{
    return std::unique_ptr<DecomposeNode>(new DecomposeNode(std::move(ITransform::Create(xgraph))));
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

std::weak_ordering DecomposeNode::operator<=>(const DecomposeNode &other) const
{
    std::weak_ordering result = getGroupingChildren().getGroupSizes() <=> other.getGroupingChildren().getGroupSizes();
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    result = getTag() <=> other.getTag();
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    result = getXGraph() <=> other.getXGraph();
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    if (isLeaf())
    {
        MyAssert(other.isLeaf());
    }
    else
    {
        const auto &groups0 = getGroupingChildren()();
        const auto &groups1 = other.getGroupingChildren()();
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
