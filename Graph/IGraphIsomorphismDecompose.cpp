#include "IGraphIsomorphismDecompose.h"
#include "Defines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "GraphIsomorphismUtils.h"
#include "IGraphIsomorphismTagger.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
enum IDecomposeType : TagEntry
{
    Leaf = 0,
    Disconnected,
    UniversalVertex,
    Known,
    Complement,
    OmittedEdges,
    ComplementKnown,
};

Vertex GetVertexInParent(Vertex vertex, const IGraphUs &graph)
{
    const auto *subGraph = dynamic_cast<const SubGraph *>(&graph);
    if (subGraph != nullptr)
    {
        return subGraph->getVertexInParent(vertex);
    }
    return vertex;
}

Vertex GetVertexInParent(Vertex vertex, const IDecompose &decompose)
{
    const auto *complement = dynamic_cast<const DecomposeComplement *>(&decompose);
    const auto &graph = complement != nullptr ? complement->getOriginal() : decompose.getGraph();
    return GetVertexInParent(vertex, graph);
}

void AddToParentMapRecur(const IDecompose *current, const IDecompose *parent,
                         std::map<const IDecompose *, const IDecompose *> &toParent)
{
    // Add self
    toParent[current] = parent;

    for (const auto &group : current->getGroupingChildren()())
    {
        for (const auto *child : group)
        {
            AddToParentMapRecur(child, current, toParent);
        }
    }
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

std::unique_ptr<IDecompose> IDecompose::Create(const Graph::IGraphUs &graph, bool tryComplement)
{
    auto knownGraph = DecomposeKnown::tryCreate(graph);
    if (knownGraph)
    {
        return knownGraph;
    }

    if (!graph.isConnected())
    {
        return std::make_unique<DecomposeDisconnected>(graph);
    }

    const auto universalVertices = graph.getUniversalVertices();
    if (!universalVertices.empty())
    {
        const std::set<Vertex> universalVerticesSet(universalVertices.begin(), universalVertices.end());
        return std::make_unique<DecomposeUniversalVertex>(graph, universalVerticesSet);
    }

    auto omitEdges = DecomposeOmitEdges::tryCreate(graph);
    if (omitEdges)
    {
        return omitEdges;
    }

    const auto complement = std::make_shared<UndirectedGraph>(UndirectedGraph::CreateComplement(graph));
    auto complementIsKnown = DecomposeComplementKnown::tryCreate(graph, complement);
    if (complementIsKnown)
    {
        return complementIsKnown;
    }

#if true // TODO
    if (tryComplement)
    {
        auto complementGraph = DecomposeComplement::Create(graph);
        if (complementGraph)
        {
            return complementGraph;
        }
    }
#endif
    return std::make_unique<DecomposeLeaf>(graph);
}

bool IDecompose::isLeaf() const
{
    return getGroupingChildren()().empty();
}

std::string IDecompose::getGraphName() const
{
    return getGraph().getName();
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

const Tag &DecomposeLeaf::getTag() const
{
    return m_tag;
}

std::string DecomposeLeaf::getDescription() const
{
    return "Leaf: " + getGraphName();
}

const Grouping<const IDecompose *> &DecomposeLeaf::getGroupingChildren() const
{
    return m_groupingChildren;
}

Graph::Vertex DecomposeLeaf::getVertexInParent(Graph::Vertex vertex) const
{
    return GetVertexInParent(vertex, *this);
}

// !!!!!!!!!!! Known

std::unique_ptr<IDecompose> DecomposeKnown::tryCreate(const Graph::IGraphUs &graph)
{
    const TaggerKnown taggerKnown(graph);
    auto knownTag = taggerKnown.getGraphTag();
    if (knownTag.front() != TaggerKnown::KnownType::Unknown)
    {
        knownTag.insert(knownTag.begin(), IDecomposeType::Known);
        return std::make_unique<DecomposeKnown>(graph, knownTag);
    }

    return {};
}

DecomposeKnown::DecomposeKnown(const Graph::IGraphUs &graph, Tag tag) : IDecompose(graph), m_tag(std::move(tag))
{
}

const Tag &DecomposeKnown::getTag() const
{
    return m_tag;
}

std::string DecomposeKnown::getDescription() const
{
    return "Known: " + getGraphName();
}

const Grouping<const IDecompose *> &DecomposeKnown::getGroupingChildren() const
{
    return m_groupingChildren;
}

Graph::Vertex DecomposeKnown::getVertexInParent(Graph::Vertex vertex) const
{
    return GetVertexInParent(vertex, *this);
}

// !!!!!!!!!!! complement is known

std::unique_ptr<IDecompose> DecomposeComplementKnown::tryCreate(
    const IGraphUs &graph, const std::shared_ptr<Graph::UndirectedGraph> &complement)
{
    const TaggerKnown taggerKnown(*complement);
    if (taggerKnown.getGraphTag().front() == TaggerKnown::KnownType::Unknown)
    {
        return {};
    }
    return std::unique_ptr<IDecompose>(new DecomposeComplementKnown(graph, complement));
}

DecomposeComplementKnown::DecomposeComplementKnown(const IGraphUs &graph,
                                                   std::shared_ptr<Graph::UndirectedGraph> complement)
    : IDecompose(graph), m_complement(std::move(complement)), m_tag{IDecomposeType::ComplementKnown},
      m_child(DecomposeKnown::tryCreate(*m_complement)),
      m_groupingChildren(CreateGrouping(std::vector<const IDecompose *>{m_child.get()}))
{
    MyAssert(static_cast<bool>(m_child));
    MyAssert(m_child->isLeaf());
}

const Tag &DecomposeComplementKnown::getTag() const
{
    return m_tag;
}

std::string DecomposeComplementKnown::getDescription() const
{
    return "Complement is knonw";
}

const Grouping<const IDecompose *> &DecomposeComplementKnown::getGroupingChildren() const
{
    return m_groupingChildren;
}

Graph::Vertex DecomposeComplementKnown::getVertexInParent(Graph::Vertex vertex) const
{
    return GetVertexInParent(vertex, getGraph());
}

// !!!!!!!!!!! complement

std::unique_ptr<IDecompose> DecomposeComplement::Create(const Graph::IGraphUs &graph)
{
    auto complement = std::make_unique<UndirectedGraph>(UndirectedGraph::CreateComplement(graph));
    TaggerKnown taggerKnown(*complement);
    if (!complement->isConnected())
    {
        auto retval = std::make_unique<DecomposeComplement>(std::move(complement), graph);
        if (!retval->isLeaf())
        {
            return retval;
        }
    }

    // No decomposition possible of complement, ignore
    return {};
}

DecomposeComplement::DecomposeComplement(std::unique_ptr<Graph::UndirectedGraph> &&complement,
                                         const Graph::IGraphUs &org)
    : IDecompose(*complement), m_complement(std::move(complement)), m_original(org), m_tag{IDecomposeType::Complement}
{
    // Further decomposition now possible?
    auto offspring = IDecompose::Create(*m_complement, false);
    if (!offspring->isLeaf())
    {
        m_child = std::move(offspring);
        std::vector<const IDecompose *> allChildren{m_child.get()};
        m_groupingChildren = CreateGrouping(allChildren);
    }
    else
    {
        // A leaf is returned
    }
}

std::string DecomposeComplement::getDescription() const
{
    return "Complement of: " + m_original.getName();
}

const Tag &DecomposeComplement::getTag() const
{
    return m_tag;
}

const Grouping<const IDecompose *> &DecomposeComplement::getGroupingChildren() const
{
    return m_groupingChildren;
}

const Graph::IGraphUs &DecomposeComplement::getOriginal() const
{
    return m_original;
}

Graph::Vertex DecomposeComplement::getVertexInParent(Graph::Vertex vertex) const
{
    return GetVertexInParent(vertex, *this);
}

// !!!!!!!!!!! Disconnected
DecomposeDisconnected::DecomposeDisconnected(const Graph::IGraphUs &graph)
    : IDecompose(graph), m_tag({IDecomposeType::Disconnected})
{
    std::map<Vertex, std::set<Vertex>> components;
    Vertex vertex = 0;
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
        m_childDecomposes.emplace_back(IDecompose::Create(child));
        allChildren.push_back(m_childDecomposes.back().get());
    }

    m_groupingChildren = CreateGrouping(allChildren);
}

std::string DecomposeDisconnected::getDescription() const
{
    return "Disconnected: " + getGraphName();
}

const Tag &DecomposeDisconnected::getTag() const
{
    return m_tag;
}

const Grouping<const IDecompose *> &DecomposeDisconnected::getGroupingChildren() const
{
    return m_groupingChildren;
}

Graph::Vertex DecomposeDisconnected::getVertexInParent(Graph::Vertex vertex) const
{
    return GetVertexInParent(vertex, *this);
}

// !!!!!!!!!!! edges in vertex groups that are cliques
DecomposeOmitEdges::DecomposeOmitEdges(const IGraphUs &graph)
    : IDecompose(graph), m_tag{IDecomposeType::OmittedEdges, static_cast<TagEntry>(graph.getNumEdges()), 0}
{
    const Grouping<Graph::Vertex> &vertexGrouping = getTaggedGraph().getVertexGrouping();
    std::vector<std::vector<Vertex>> cliques;

    for (const auto &group : vertexGrouping())
    {
        if (graph.isClique(group))
        {
            cliques.emplace_back(group);
        }
    }

    if (cliques.empty())
    {
        return;
    }

    m_child = std::make_unique<UndirectedGraph>(UndirectedGraph::CreateEdgesOmitted(graph, cliques));
    m_numOmittedEdges = graph.getNumEdges() - m_child->getNumEdges();
    if (m_numOmittedEdges == 0)
    {
        m_child = {};
        return;
    }

    m_tag.back() = m_numOmittedEdges;
    m_childDecomposes = IDecompose::Create(*m_child);
    m_groupingChildren = CreateGrouping(std::vector<const IDecompose *>{m_childDecomposes.get()});
}

std::string DecomposeOmitEdges::getDescription() const
{
    return std::to_string(getNumOmittedEdges()) + " omitted edges from: " + getGraph().getName();
}

Edge DecomposeOmitEdges::getNumOmittedEdges() const
{
    return m_numOmittedEdges;
}

const Tag &DecomposeOmitEdges::getTag() const
{
    return m_tag;
}

std::unique_ptr<IDecompose> DecomposeOmitEdges::tryCreate(const IGraphUs &graph)
{
    std::unique_ptr<DecomposeOmitEdges> retval(new DecomposeOmitEdges(graph));
    if (retval->getNumOmittedEdges() == 0)
    {
        retval = {};
    }
    return retval;
}

const Grouping<const IDecompose *> &DecomposeOmitEdges::getGroupingChildren() const
{
    return m_groupingChildren;
}

Graph::Vertex DecomposeOmitEdges::getVertexInParent(Graph::Vertex vertex) const
{
    return GetVertexInParent(vertex, *this);
}

// !!!!!!!!!!! universal vertices

DecomposeUniversalVertex::DecomposeUniversalVertex(const Graph::IGraphUs &graph,
                                                   const std::set<Vertex> &universalVertices)
    : IDecompose(graph), m_tag{IDecomposeType::UniversalVertex, static_cast<TagEntry>(universalVertices.size())}
{
    MyAssert(!universalVertices.empty());

    // Complete part
    auto completePart = std::make_unique<SubGraphConnected>(graph, universalVertices);
    m_children.emplace_back(std::move(completePart));

    // Remainder
    auto range =
        graph.getVertexRange() | stv::filter([&universalVertices](Vertex v) { return !universalVertices.contains(v); });
    std::set<Vertex> remainder;
    str::copy(range, std::inserter(remainder, remainder.end()));

    auto remainderPart = std::make_unique<SubGraph>(graph, remainder);
    m_children.emplace_back(std::move(remainderPart));

    std::vector<const IDecompose *> allChildren;
    for (const auto &child : m_children)
    {
        m_childDecomposes.emplace_back(IDecompose::Create(*child));
        allChildren.push_back(m_childDecomposes.back().get());
    }

    m_groupingChildren = CreateGrouping(allChildren);
}

const Tag &DecomposeUniversalVertex::getTag() const
{
    return m_tag;
}

std::string DecomposeUniversalVertex::getDescription() const
{
    return "Universal vertices: " + getGraphName();
}

const Grouping<const IDecompose *> &DecomposeUniversalVertex::getGroupingChildren() const
{
    return m_groupingChildren;
}

Graph::Vertex DecomposeUniversalVertex::getVertexInParent(Graph::Vertex vertex) const
{
    return GetVertexInParent(vertex, *this);
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

Vertex ToParentMap::getVertexInRoot(Vertex vertex, const IDecompose *decompose) const
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

std::string ToParentMap::getDescriptions() const
{
    std::string result;
    for (const auto *leaf : getLeaves())
    {
        const auto *id = leaf;
        while (id != nullptr)
        {
            result += id->getDescription() + "\n";
            id = getParent(id);
        }
        result += "\n\n";
    }
    return result;
}
