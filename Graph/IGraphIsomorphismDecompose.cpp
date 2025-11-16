#include "IGraphIsomorphismDecompose.h"
#include "Defines.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "GraphIsomorphismUtils.h"
#include "IGraphIsomorphismTagger.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
enum IDecomposeType : TagEntry
{
    Leaf = 0,
    Disconnected,
    FullyConnected
};

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

void AddToParentMapRecur(const IDecompose *current, const IDecompose *parent,
                         std::map<const IDecompose *, const IDecompose *> &toParent)
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

// TODO remove me
GraphTags IDecompose::GetGraphTags(const Graph::IGraphUs &graph)
{
    GraphTags result;
    for (const auto &tagger : GraphIsomorphism::getAllTaggers(graph))
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
    const TaggerKnown taggerKnown(graph);
    const auto knownTag = taggerKnown.getGraphTag();
    if (knownTag.front() != TaggerKnown::KnownType::Unknown)
    {
        return std::make_unique<DecomposeLeaf>(graph);
    }

    if (!graph.isConnected())
    {
        return std::make_unique<DecomposeDisconnected>(graph);
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
// !!!!!!!!!!! Leaf
DecomposeLeaf::DecomposeLeaf(const Graph::IGraphUs &graph) : m_self(graph), m_tag{IDecomposeType::Leaf}
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
const Tag &DecomposeLeaf::getTag() const
{
    return m_tag;
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
    : m_self(graph), m_tag({IDecomposeType::Disconnected})
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
        m_tag.push_back(cmp.second.size());
    }
    std::sort(m_tag.begin() + 1, m_tag.end());

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

const Tag &DecomposeDisconnected::getTag() const
{
    return m_tag;
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
    : m_self(graph), m_tag{IDecomposeType::FullyConnected, static_cast<TagEntry>(fullyConnected.size())}
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

const Tag &DecomposeVertexFullyConnected::getTag() const
{
    return m_tag;
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

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ToParent

ToParentMap::ToParentMap(const IDecompose *root)
{
    AddToParentMapRecur(root, nullptr, m_toParent);
}

const IDecompose *ToParentMap::getParent(const IDecompose *ptr) const
{
    return m_toParent.at(ptr);
}

bool ToParentMap::isRoot(const IDecompose *ptr) const
{
    return getParent(ptr) == nullptr;
}

std::vector<const IDecompose *> ToParentMap::getLeaves() const
{
    std::vector<const IDecompose *> result;
    for (const auto &itr : m_toParent)
    {
        if (itr.first->isLeaf())
        {
            result.push_back(itr.first);
        }
    }
    return result;
}

size_t ToParentMap::size() const
{
    return m_toParent.size();
}

GraphVertex ToParentMap::getVertexInRoot(GraphVertex vertex, const IDecompose *decompose) const
{
    while (decompose != nullptr)
    {
        vertex = decompose->getVertexInParent(vertex);
        decompose = getParent(decompose);
    }
    return vertex;
}

std::vector<Tag> ToParentMap::collectDecomposeTags(const IDecompose *decompose) const
{
    std::vector<Tag> result;
    while (decompose != nullptr)
    {
        result.push_back(decompose->getTag());
        decompose = getParent(decompose);
    }
    return result;
}

const IDecompose *ToParentMap::getRoot(const IDecompose *ptr) const
{
    MyAssert(ptr != nullptr);
    while (!isRoot(ptr))
    {
        ptr = getParent(ptr);
    }
    return ptr;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! compare

std::weak_ordering IDecompose::CompareRoots(const IDecompose *root1, const IDecompose *root2)
{
    const ToParentMap toParentMap1(root1);
    const ToParentMap toParentMap2(root2);

    MyAssert(toParentMap1.isRoot(root1));
    MyAssert(toParentMap2.isRoot(root2));

    const TaggedGraph tg1(root1->getSelf());
    const TaggedGraph tg2(root2->getSelf());

    std::weak_ordering result = (tg1 <=> tg2);
    if (result != 0)
    {
        return result;
    }
    return result;
}

std::weak_ordering IDecompose::CompareLeaves(const IDecompose *leaf1, const IDecompose *leaf2,
                                             const ToParentMap &toParent)
{
    MyAssert(leaf1->isLeaf());
    MyAssert(leaf2->isLeaf());

    std::weak_ordering result = toParent.collectDecomposeTags(leaf1) <=> toParent.collectDecomposeTags(leaf2);
    if (result != 0)
    {
        return result;
    }

    const auto *d1 = leaf1;
    const auto *d2 = leaf2;
    while (d1 != nullptr)
    {
        const TaggedGraph tg1(d1->getSelf());
        const TaggedGraph tg2(d2->getSelf());
        result = tg1 <=> tg2;
        if (result != 0)
        {
            return result;
        }
        d1 = toParent.getParent(d1);
        d2 = toParent.getParent(d2);
    }
    return result;
}
