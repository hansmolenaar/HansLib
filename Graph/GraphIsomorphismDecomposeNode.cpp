#include "GraphIsomorphismDecomposeNode.h"

#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
} // namespace

DecomposeNode::DecomposeNode(std::unique_ptr<ITransform> &&transform) : m_transform(std::move(transform))
{
    const auto &children = m_transform->getChildren();
    for (const auto &child : children)
    {
        auto childTransform = Create(child);
        MyAssert(!!childTransform);
        m_childDecomposes.emplace_back(std::move(childTransform));
    }
    MyAssert(m_childDecomposes.empty());
}

std::unique_ptr<DecomposeNode> DecomposeNode::Create(const std::shared_ptr<TaggedGraph> &tgraph)
{
    return std::unique_ptr<DecomposeNode>(new DecomposeNode(std::move(ITransform::Create(tgraph))));
}

const TaggedGraph &DecomposeNode::getTaggedGraph() const
{
    return m_transform->getTaggedGraph();
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
