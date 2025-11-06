#include "IGraphIsomorphismTransform.h"
#include "GraphIsomorphismConstruct.h"
#include "IGraphIsomorphismTagger.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

// !!!!!!!!!!!  IGraphIsomorphismTransform

GraphTags IGraphIsomorphismTransform::GetGraphTags(const Graph::IGraphUs &graph)
{
    GraphTags result;
    for (auto *factory : Construct::getGraphTaggerFactories())
    {
        result.emplace_back(factory->createGraphTagger(graph)->getGraphTag());
    }
    return result;
}

std::unique_ptr<IGraphIsomorphismTransform> IGraphIsomorphismTransform::Create(const Graph::IGraphUs &graph)
{
    if (!graph.isConnected())
    {
        return std::make_unique<GraphIsomorphismTransformDisconnected>(graph);
    }
    return std::make_unique<GraphIsomorphismTransformLeaf>(graph);
}

// !!!!!!!!!!! Leaf
GraphIsomorphismTransformLeaf::GraphIsomorphismTransformLeaf(const Graph::IGraphUs &graph)
    : m_self(graph), m_tagSelf(IGraphIsomorphismTransform::GetGraphTags(m_self))
{
}

GraphVertex GraphIsomorphismTransformLeaf::getVertexInParent(GraphVertex v) const
{
    return v;
}

const Graph::IGraphUs &GraphIsomorphismTransformLeaf::getSelf() const
{
    return m_self;
}
const GraphTags &GraphIsomorphismTransformLeaf::getTagSelf() const
{
    return m_tagSelf;
}

const std::vector<GraphTags> &GraphIsomorphismTransformLeaf::getChildTags() const
{
    return m_childTags;
}

const std::vector<const IGraphIsomorphismTransform *> &GraphIsomorphismTransformLeaf::getChildren(
    const GraphTags &) const
{
    throw MyException("GraphIsomorphismTransformLeaf::getChildren should not come here");
}

// !!!!!!!!!!! Disconnected
GraphIsomorphismTransformDisconnected::GraphIsomorphismTransformDisconnected(const Graph::IGraphUs &graph)
    : m_self(graph), m_tagSelf(IGraphIsomorphismTransform::GetGraphTags(m_self))
{
    std::map<GraphVertex, std::set<GraphVertex>> components;
    GraphVertex vertex = 0;
    for (auto c : graph.getConnectedComponents())
    {
        components[c].insert(vertex);
        ++vertex;
    }
    MyAssert(components.size() > 1);

    for (const auto &cmp : components)
    {
        m_children.emplace_back(m_self, cmp.second);
        const auto tag = IGraphIsomorphismTransform::GetGraphTags(m_children.back());
        if (!m_childTransformsUP.contains(tag))
        {
            m_childTags.emplace_back(tag);
        }
        m_childTransformsUP[tag].emplace_back(IGraphIsomorphismTransform::Create(m_children.back()));
        m_childTransforms.emplace_back(m_childTransformsUP.at(tag).back().get());
    }
}

GraphVertex GraphIsomorphismTransformDisconnected::getVertexInParent(GraphVertex v) const
{
    return v;
}

const Graph::IGraphUs &GraphIsomorphismTransformDisconnected::getSelf() const
{
    return m_self;
}

const GraphTags &GraphIsomorphismTransformDisconnected::getTagSelf() const
{
    return m_tagSelf;
}

const std::vector<GraphTags> &GraphIsomorphismTransformDisconnected::getChildTags() const
{
    return m_childTags;
}
const std::vector<const IGraphIsomorphismTransform *> &GraphIsomorphismTransformDisconnected::getChildren(
    const GraphTags &) const
{
    return m_childTransforms;
}
