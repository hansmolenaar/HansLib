#include "IGraphIsomorphismDecompose.h"
#include "Defines.h"
#include "GraphIsomorphismConstruct.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "IGraphIsomorphismTagger.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
GraphVertex GetVertexInParent(GraphVertex vertex, const IGraphIsomorphismDecompose &decompose)
{
    const auto &self = decompose.getSelf();
    const auto *subGraph = dynamic_cast<const SubGraph *>(&self);
    if (subGraph != nullptr)
    {
        return subGraph->getVertexInParent(vertex);
    }
    return vertex;
}

void AddToParentMapRecur(const IGraphIsomorphismDecompose *current, const IGraphIsomorphismDecompose *parent,
                         IGraphIsomorphismDecompose::ToParentMap &toParent)
{
    // Add self
    toParent[current] = parent;

    for (const auto &tag : current->getChildTags())
    {
        for (const auto *child : current->getChildren(tag))
        {
            AddToParentMapRecur(child, current, toParent);
        }
    }
}

} // namespace

// !!!!!!!!!!!  IGraphIsomorphismDecompose

GraphTags IGraphIsomorphismDecompose::GetGraphTags(const Graph::IGraphUs &graph)
{
    GraphTags result;
    for (auto *factory : Construct::getGraphTaggerFactories())
    {
        result.emplace_back(factory->createGraphTagger(graph)->getGraphTag());
    }
    return result;
}

std::unique_ptr<IGraphIsomorphismDecompose> IGraphIsomorphismDecompose::Create(const Graph::IGraphUs &graph)
{
    if (!graph.isConnected())
    {
        return std::make_unique<GraphIsomorphismDecomposeDisconnected>(graph);
    }

    const TaggerKnown taggerKnown(graph);
    const auto knownTag = taggerKnown.getGraphTag();
    if (knownTag.front() != TaggerKnown::KnownType::Unknown)
    {
        return std::make_unique<GraphIsomorphismDecomposeLeaf>(graph);
    }

    const auto fullyConnectedVertices = graph.getFullyConnectedVertices();
    if (!fullyConnectedVertices.empty())
    {
        const std::set<GraphVertex> fullyConnectedVerticesSet(fullyConnectedVertices.begin(),
                                                              fullyConnectedVertices.end());
        return std::make_unique<GraphIsomorphismDecomposeVertexFullyConnected>(graph, fullyConnectedVerticesSet);
    }

    return std::make_unique<GraphIsomorphismDecomposeLeaf>(graph);
}

bool IGraphIsomorphismDecompose::isLeaf() const
{
    return getChildTags().empty();
}

IGraphIsomorphismDecompose::ToParentMap IGraphIsomorphismDecompose::GetToParentMap(
    const IGraphIsomorphismDecompose *root)
{
    ToParentMap result;
    AddToParentMapRecur(root, nullptr, result);
    return result;
}
// !!!!!!!!!!! Leaf
GraphIsomorphismDecomposeLeaf::GraphIsomorphismDecomposeLeaf(const Graph::IGraphUs &graph)
    : m_self(graph), m_tagSelf(IGraphIsomorphismDecompose::GetGraphTags(m_self))
{
}

GraphVertex GraphIsomorphismDecomposeLeaf::getVertexInParent(GraphVertex v) const
{
    return GetVertexInParent(v, *this);
}

const Graph::IGraphUs &GraphIsomorphismDecomposeLeaf::getSelf() const
{
    return m_self;
}
const GraphTags &GraphIsomorphismDecomposeLeaf::getTagSelf() const
{
    return m_tagSelf;
}

const std::vector<GraphTags> &GraphIsomorphismDecomposeLeaf::getChildTags() const
{
    return m_childTags;
}

std::vector<const IGraphIsomorphismDecompose *> GraphIsomorphismDecomposeLeaf::getChildren(const GraphTags &) const
{
    throw MyException("GraphIsomorphismDecomposeLeaf::getChildren should not come here");
}

// !!!!!!!!!!! Disconnected
GraphIsomorphismDecomposeDisconnected::GraphIsomorphismDecomposeDisconnected(const Graph::IGraphUs &graph)
    : m_self(graph), m_tagSelf(IGraphIsomorphismDecompose::GetGraphTags(m_self))
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
    }

    for (const auto &child : m_children)
    {
        const auto tag = IGraphIsomorphismDecompose::GetGraphTags(child);
        if (!m_childDecomposes.contains(tag))
        {
            m_childTags.emplace_back(tag);
        }
        m_childDecomposes[tag].emplace_back(IGraphIsomorphismDecompose::Create(child));
    }
}

GraphVertex GraphIsomorphismDecomposeDisconnected::getVertexInParent(GraphVertex v) const
{
    return GetVertexInParent(v, *this);
}

const Graph::IGraphUs &GraphIsomorphismDecomposeDisconnected::getSelf() const
{
    return m_self;
}

const GraphTags &GraphIsomorphismDecomposeDisconnected::getTagSelf() const
{
    return m_tagSelf;
}

const std::vector<GraphTags> &GraphIsomorphismDecomposeDisconnected::getChildTags() const
{
    return m_childTags;
}
std::vector<const IGraphIsomorphismDecompose *> GraphIsomorphismDecomposeDisconnected::getChildren(
    const GraphTags &tag) const
{
    const auto &childrenWithTag = m_childDecomposes.at(tag);
    std::vector<const IGraphIsomorphismDecompose *> result(childrenWithTag.size());
    str::transform(childrenWithTag, result.begin(), [](const auto &kid) { return kid.get(); });
    return result;
}

// !!!!!!!!!!! fully connected vertices

GraphIsomorphismDecomposeVertexFullyConnected::GraphIsomorphismDecomposeVertexFullyConnected(
    const Graph::IGraphUs &graph, const std::set<GraphVertex> &fullyConnected)
    : m_self(graph), m_tagSelf(IGraphIsomorphismDecompose::GetGraphTags(m_self))
{
    MyAssert(!fullyConnected.empty());

    // Complete part
    auto completePart = std::make_unique<SubGraphConnected>(graph, fullyConnected);
    m_children.emplace_back(std::move(completePart));

    // Remainder
    std::set<GraphVertex> remainder;
    const auto nVertices = graph.getNumVertices();
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        if (!fullyConnected.contains(v))
        {
            remainder.insert(v);
        }
    }

    auto remainderPart = std::make_unique<SubGraph>(graph, remainder);
    m_children.emplace_back(std::move(remainderPart));

    for (const auto &child : m_children)
    {
        const auto tag = IGraphIsomorphismDecompose::GetGraphTags(*child);
        if (!m_childDecomposes.contains(tag))
        {
            m_childTags.emplace_back(tag);
        }
        m_childDecomposes[tag].emplace_back(IGraphIsomorphismDecompose::Create(*child));
    }
}

GraphVertex GraphIsomorphismDecomposeVertexFullyConnected::getVertexInParent(GraphVertex v) const
{
    return GetVertexInParent(v, *this);
}

const Graph::IGraphUs &GraphIsomorphismDecomposeVertexFullyConnected::getSelf() const
{
    return m_self;
}

const GraphTags &GraphIsomorphismDecomposeVertexFullyConnected::getTagSelf() const
{
    return m_tagSelf;
}

const std::vector<GraphTags> &GraphIsomorphismDecomposeVertexFullyConnected::getChildTags() const
{
    return m_childTags;
}

std::vector<const IGraphIsomorphismDecompose *> GraphIsomorphismDecomposeVertexFullyConnected::getChildren(
    const GraphTags &tag) const
{
    const auto &childrenWithTag = m_childDecomposes.at(tag);
    std::vector<const IGraphIsomorphismDecompose *> result(childrenWithTag.size());
    str::transform(childrenWithTag, result.begin(), [](const auto &kid) { return kid.get(); });
    return result;
}
