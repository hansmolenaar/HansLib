#include "IGraphIsomorphismDecompose.h"
#include "Defines.h"
#include "GraphIsomorphismConstruct.h" // TODO remove me
#include "GraphIsomorphismTaggerKnown.h"
#include "IGraphIsomorphismTagger.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
GraphVertex GetVertexInParent(GraphVertex vertex, const IDecompose &decompose)
{
    const auto &self = decompose.getSelf();
    const auto *subGraph = dynamic_cast<const SubGraph *>(&self);
    if (subGraph != nullptr)
    {
        return subGraph->getVertexInParent(vertex);
    }
    return vertex;
}

void AddToParentMapRecur(const IDecompose *current, const IDecompose *parent, IDecompose::ToParentMap &toParent)
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

// !!!!!!!!!!!  IDecompose

GraphTags IDecompose::GetGraphTags(const Graph::IGraphUs &graph)
{
    GraphTags result;
    for (const auto& tagger : Construct::getAllTaggers(graph))
    {
        const auto *graphTagger = tagger->getGraphTagger();
        if (graphTagger != nullptr)
        {
            result.emplace_back(graphTagger->getGraphTag());
        }
    }
    return result;
}

std::unique_ptr<IDecompose> IDecompose::Create(const Graph::IGraphUs &graph)
{
    if (!graph.isConnected())
    {
        return std::make_unique<DecomposeDisconnected>(graph);
    }

    const TaggerKnown taggerKnown(graph);
    const auto knownTag = taggerKnown.getGraphTag();
    if (knownTag.front() != TaggerKnown::KnownType::Unknown)
    {
        return std::make_unique<DecomposeLeaf>(graph);
    }

    const auto fullyConnectedVertices = graph.getFullyConnectedVertices();
    if (!fullyConnectedVertices.empty())
    {
        const std::set<GraphVertex> fullyConnectedVerticesSet(fullyConnectedVertices.begin(),
                                                              fullyConnectedVertices.end());
        return std::make_unique<DecomposeVertexFullyConnected>(graph, fullyConnectedVerticesSet);
    }

    return std::make_unique<DecomposeLeaf>(graph);
}

bool IDecompose::isLeaf() const
{
    return getChildTags().empty();
}

IDecompose::ToParentMap IDecompose::GetToParentMap(const IDecompose *root)
{
    ToParentMap result;
    AddToParentMapRecur(root, nullptr, result);
    return result;
}
// !!!!!!!!!!! Leaf
DecomposeLeaf::DecomposeLeaf(const Graph::IGraphUs &graph) : m_self(graph), m_tagSelf(IDecompose::GetGraphTags(m_self))
{
}

GraphVertex DecomposeLeaf::getVertexInParent(GraphVertex v) const
{
    return GetVertexInParent(v, *this);
}

const Graph::IGraphUs &DecomposeLeaf::getSelf() const
{
    return m_self;
}
const GraphTags &DecomposeLeaf::getTagSelf() const
{
    return m_tagSelf;
}

const std::vector<GraphTags> &DecomposeLeaf::getChildTags() const
{
    return m_childTags;
}

std::vector<const IDecompose *> DecomposeLeaf::getChildren(const GraphTags &) const
{
    throw MyException("DecomposeLeaf::getChildren should not come here");
}

// !!!!!!!!!!! Disconnected
DecomposeDisconnected::DecomposeDisconnected(const Graph::IGraphUs &graph)
    : m_self(graph), m_tagSelf(IDecompose::GetGraphTags(m_self))
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
        const auto tag = IDecompose::GetGraphTags(child);
        if (!m_childDecomposes.contains(tag))
        {
            m_childTags.emplace_back(tag);
        }
        m_childDecomposes[tag].emplace_back(IDecompose::Create(child));
    }
}

GraphVertex DecomposeDisconnected::getVertexInParent(GraphVertex v) const
{
    return GetVertexInParent(v, *this);
}

const Graph::IGraphUs &DecomposeDisconnected::getSelf() const
{
    return m_self;
}

const GraphTags &DecomposeDisconnected::getTagSelf() const
{
    return m_tagSelf;
}

const std::vector<GraphTags> &DecomposeDisconnected::getChildTags() const
{
    return m_childTags;
}
std::vector<const IDecompose *> DecomposeDisconnected::getChildren(const GraphTags &tag) const
{
    const auto &childrenWithTag = m_childDecomposes.at(tag);
    std::vector<const IDecompose *> result(childrenWithTag.size());
    str::transform(childrenWithTag, result.begin(), [](const auto &kid) { return kid.get(); });
    return result;
}

// !!!!!!!!!!! fully connected vertices

DecomposeVertexFullyConnected::DecomposeVertexFullyConnected(const Graph::IGraphUs &graph,
                                                             const std::set<GraphVertex> &fullyConnected)
    : m_self(graph), m_tagSelf(IDecompose::GetGraphTags(m_self))
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
        const auto tag = IDecompose::GetGraphTags(*child);
        if (!m_childDecomposes.contains(tag))
        {
            m_childTags.emplace_back(tag);
        }
        m_childDecomposes[tag].emplace_back(IDecompose::Create(*child));
    }
}

GraphVertex DecomposeVertexFullyConnected::getVertexInParent(GraphVertex v) const
{
    return GetVertexInParent(v, *this);
}

const Graph::IGraphUs &DecomposeVertexFullyConnected::getSelf() const
{
    return m_self;
}

const GraphTags &DecomposeVertexFullyConnected::getTagSelf() const
{
    return m_tagSelf;
}

const std::vector<GraphTags> &DecomposeVertexFullyConnected::getChildTags() const
{
    return m_childTags;
}

std::vector<const IDecompose *> DecomposeVertexFullyConnected::getChildren(const GraphTags &tag) const
{
    const auto &childrenWithTag = m_childDecomposes.at(tag);
    std::vector<const IDecompose *> result(childrenWithTag.size());
    str::transform(childrenWithTag, result.begin(), [](const auto &kid) { return kid.get(); });
    return result;
}
