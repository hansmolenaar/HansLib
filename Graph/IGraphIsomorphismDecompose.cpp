#include "IGraphIsomorphismDecompose.h"
#include "Defines.h"
#include "GraphIsomorphismGrouping.h"
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
    const auto &self = decompose.getGraph();
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

GraphTags GetGraphTags(const Graph::IGraphUs &graph)
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

} // namespace

// !!!!!!!!!!!  IDecompose

IDecompose::IDecompose(const Graph::IGraphUs &graph) : m_taggedGraph(std::make_unique<TaggedGraph>(graph))
{
}

const TaggedGraph &IDecompose::getTaggedGraph() const
{
    return *m_taggedGraph;
}

const Graph::IGraphUs &IDecompose::getGraph() const
{
    return m_taggedGraph->getGraph();
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

std::weak_ordering IDecompose::operator<=>(const IDecompose &id1) const
{
    const IDecompose &id0 = *this;
    std::weak_ordering result = id0.getTag() <=> id1.getTag();
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    return id0.getTaggedGraph() <=> id1.getTaggedGraph();
}

Grouping<const IDecompose *> IDecompose::CreateGrouping(const std::vector<const IDecompose *> &children)
{
    auto compare = [](const IDecompose *child1, const IDecompose *child2) { return *child1 < *child2; };
    return Grouping<const IDecompose *>(children, compare);
}

// !!!!!!!!!!! Leaf
DecomposeLeaf::DecomposeLeaf(const Graph::IGraphUs &graph)
    : IDecompose(graph), m_tag{IDecomposeType::Leaf, static_cast<TagEntry>(graph.getNumVertices()),
                               static_cast<TagEntry>(graph.getNumEdges())}
{
}

GraphVertex DecomposeLeaf::getVertexInParent(GraphVertex v) const
{
    return GetVertexInParent(v, *this);
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

const Grouping<const IDecompose *> &DecomposeLeaf::getGroupingChildren() const
{
    throw MyException("DecomposeLeaf::getGrouping should not come here");
}

// !!!!!!!!!!! Disconnected
DecomposeDisconnected::DecomposeDisconnected(const Graph::IGraphUs &graph)
    : IDecompose(graph), m_tag({IDecomposeType::Disconnected})
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
        m_children.emplace_back(graph, cmp.second);
        m_tag.push_back(cmp.second.size());
    }
    std::sort(m_tag.begin() + 1, m_tag.end());

    std::vector<const IDecompose *> allChildren;
    for (const auto &child : m_children)
    {
        const auto tag = GetGraphTags(child);
        if (!m_childDecomposes.contains(tag))
        {
            m_childTags.emplace_back(tag);
        }
        m_childDecomposes[tag].emplace_back(IDecompose::Create(child));
        allChildren.push_back(m_childDecomposes[tag].back().get());
    }

    m_groupingChildren = CreateGrouping(allChildren);
}

GraphVertex DecomposeDisconnected::getVertexInParent(GraphVertex v) const
{
    return GetVertexInParent(v, *this);
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

const Grouping<const IDecompose *> &DecomposeDisconnected::getGroupingChildren() const
{
    return m_groupingChildren;
}

// !!!!!!!!!!! fully connected vertices

DecomposeVertexFullyConnected::DecomposeVertexFullyConnected(const Graph::IGraphUs &graph,
                                                             const std::set<GraphVertex> &fullyConnected)
    : IDecompose(graph), m_tag{IDecomposeType::FullyConnected, static_cast<TagEntry>(fullyConnected.size())}
{
    MyAssert(!fullyConnected.empty());

    // Complete part
    auto completePart = std::make_unique<SubGraphConnected>(graph, fullyConnected);
    m_children.emplace_back(std::move(completePart));

    // Remainder
    auto range =
        graph.getVertexRange() | stv::filter([&fullyConnected](GraphVertex v) { return !fullyConnected.contains(v); });
    std::set<GraphVertex> remainder;
    str::copy(range, std::inserter(remainder, remainder.end()));

    auto remainderPart = std::make_unique<SubGraph>(graph, remainder);
    m_children.emplace_back(std::move(remainderPart));

    std::vector<const IDecompose *> allChildren;
    for (const auto &child : m_children)
    {
        const auto tag = GetGraphTags(*child);
        if (!m_childDecomposes.contains(tag))
        {
            m_childTags.emplace_back(tag);
        }
        m_childDecomposes[tag].emplace_back(IDecompose::Create(*child));
        allChildren.push_back(m_childDecomposes[tag].back().get());
    }

    m_groupingChildren = CreateGrouping(allChildren);
}

GraphVertex DecomposeVertexFullyConnected::getVertexInParent(GraphVertex v) const
{
    return GetVertexInParent(v, *this);
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

const Grouping<const IDecompose *> &DecomposeVertexFullyConnected::getGroupingChildren() const
{
    return m_groupingChildren;
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

std::vector<Tag> ToParentMap::collectDecomposeTagsForLeaf(const IDecompose *decompose) const
{
    MyAssert(decompose != nullptr && decompose->isLeaf());
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

const IDecompose *ToParentMap::getRoot() const
{
    MyAssert(!m_toParent.empty());
    return getRoot(m_toParent.begin()->first);
}

std::weak_ordering ToParentMap::compareLeaves(const IDecompose *leaf1, const IDecompose *leaf2) const
{
    MyAssert(leaf1->isLeaf());
    MyAssert(leaf2->isLeaf());
    std::weak_ordering result = collectDecomposeTagsForLeaf(leaf1) <=> collectDecomposeTagsForLeaf(leaf2);
    if (result != 0)
    {
        return result;
    }

    const IDecompose *d1 = leaf1;
    const IDecompose *d2 = leaf2;
    while (d1 != d2)
    {
        const TaggedGraph &tg1(d1->getTaggedGraph());
        const TaggedGraph &tg2(d2->getTaggedGraph());
        result = tg1 <=> tg2;
        if (result != 0)
        {
            return result;
        }
        d1 = getParent(d1);
        d2 = getParent(d2);
    }
    return result;
}

Grouping<const IDecompose *> ToParentMap::groupLeaves() const
{
    auto cmp = [this](const IDecompose *leaf1, const IDecompose *leaf2) {
        return compareLeaves(leaf1, leaf2) == std::weak_ordering::less;
    };
    const Grouping<const IDecompose *> grouping(getLeaves(), cmp);
    return grouping;
}

std::weak_ordering ToParentMap::operator<=>(const ToParentMap &map2) const
{
    const auto &map1 = *this;
    const auto groups1 = map1.groupLeaves();
    const auto groups2 = map2.groupLeaves();
    std::weak_ordering result = groups1.getGroupSizes() <=> groups2.getGroupSizes();
    if (result != 0)
    {
        return result;
    }

    std::set<std::pair<const IGraphUs *, const IGraphUs *>> done;

    const auto nGroups = groups1().size();
    for (size_t n = 0; n < nGroups; ++n)
    {
        const auto &group1 = groups1().at(n);
        const auto &group2 = groups2().at(n);
        const auto *leaf1 = group1.front();
        const auto *leaf2 = group2.front();

        const auto pathTags1 = map1.collectDecomposeTagsForLeaf(leaf1);
        const auto pathTags2 = map2.collectDecomposeTagsForLeaf(leaf2);
        result = pathTags1 <=> pathTags2;
        if (result != 0)
        {
            return result;
        }

        // Test back to root
        const IDecompose *decomp1 = leaf1;
        const IDecompose *decomp2 = leaf2;
        while (decomp1 != nullptr)
        {
            MyAssert(decomp2 != nullptr);

            // Test only once, this is an expensive operation
            const auto &self1 = decomp1->getGraph();
            const auto &self2 = decomp2->getGraph();
            const auto graphPair = std::make_pair(&self1, &self2);
            if (done.contains(graphPair))
            {
                break;
            }
            done.insert(graphPair);
            const TaggedGraph &tg1(decomp1->getTaggedGraph());
            const TaggedGraph &tg2(decomp2->getTaggedGraph());
            result = tg1 <=> tg2;
            if (result != 0)
            {
                return result;
            }
            decomp1 = map1.getParent(decomp1);
            decomp2 = map2.getParent(decomp2);
        }
    }
    return result;
}
