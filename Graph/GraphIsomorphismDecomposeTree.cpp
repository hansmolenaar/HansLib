#include "GraphIsomorphismDecomposeTree.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{

void AddToParentMapRecur(const DecomposeNode *current, const DecomposeNode *parent,
                         std::map<const DecomposeNode *, const DecomposeNode *> &toParent)
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

DecomposeTree::DecomposeTree(const Graph::IGraphUs &graph)
    : DecomposeTree(DecomposeNode::Create(std::make_shared<XGraph>(graph)))
{
    // Empty
}

DecomposeTree::DecomposeTree(std::unique_ptr<DecomposeNode> &&root) : m_root(std::move(root))
{
    AddToParentMapRecur(m_root.get(), nullptr, m_toParent);
}

const DecomposeNode &DecomposeTree::getRoot() const
{
    return *m_root;
}

size_t DecomposeTree::size() const
{
    return m_toParent.size();
}

const DecomposeNode *DecomposeTree::getParent(const DecomposeNode *ptr) const
{
    return m_toParent.at(ptr);
}

std::vector<const DecomposeNode *> DecomposeTree::getLeaves() const
{
    std::vector<const DecomposeNode *> result;
    for (const auto &itr : m_toParent)
    {
        if (itr.first->isLeaf())
        {
            result.push_back(itr.first);
        }
    }
    auto compare = [](const DecomposeNode *child1, const DecomposeNode *child2) {
        return child1->compareGraph(*child2) == std::weak_ordering::less;
    };
    str::sort(result, compare);

    return result;
}

Vertex DecomposeTree::getVertexInRoot(Vertex vertex, const DecomposeNode &decomposeRef) const
{
    const DecomposeNode *decompose = &decomposeRef;
    while (decompose != nullptr)
    {
        vertex = decompose->getVertexInParent(vertex);
        decompose = getParent(decompose);
    }
    return vertex;
}

std::vector<std::string> DecomposeTree::getDescriptions() const
{
    static const std::string sep = " -> ";
    std::vector<std::string> result;
    for (const auto *leaf : getLeaves())
    {
        const auto *id = leaf;
        std::string descr;
        while (id != nullptr)
        {
            if (!descr.empty())
            {
                descr += sep;
            }
            descr += id->getDescription();
            id = getParent(id);
        }
        result.push_back(descr);
    }
    return result;
}

std::vector<Tag> DecomposeTree::collectDecomposeTagsForLeaf(const DecomposeNode *decompose) const
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

std::weak_ordering DecomposeTree::compareLeaves(const DecomposeNode *leaf1, const DecomposeNode *leaf2) const
{
    MyAssert(leaf1->isLeaf());
    MyAssert(leaf2->isLeaf());
    std::weak_ordering result = collectDecomposeTagsForLeaf(leaf1) <=> collectDecomposeTagsForLeaf(leaf2);
    if (result != 0)
    {
        return result;
    }

    const DecomposeNode *d1 = leaf1;
    const DecomposeNode *d2 = leaf2;
    while (d1 != d2)
    {
        result = d1->compareGraph(*d2);
        if (result != 0)
        {
            return result;
        }
        d1 = getParent(d1);
        d2 = getParent(d2);
    }
    return result;
}

Grouping<const DecomposeNode *> DecomposeTree::groupLeaves() const
{
    auto cmp = [this](const DecomposeNode *leaf1, const DecomposeNode *leaf2) {
        return compareLeaves(leaf1, leaf2) == std::weak_ordering::less;
    };
    const Grouping<const DecomposeNode *> grouping(getLeaves(), cmp);
    return grouping;
}

std::weak_ordering DecomposeTree::compareGraph(const IGraphCompare &other) const
{
    const auto &lhs = *this;
    const auto &rhs = dynamic_cast<const DecomposeTree &>(other);

    const auto groups1 = lhs.groupLeaves();
    const auto groups2 = rhs.groupLeaves();
    std::weak_ordering result = groups1.getGroupSizes() <=> groups2.getGroupSizes();
    if (result != 0)
    {
        return result;
    }

    std::set<std::pair<const IGraphUs *, const IGraphUs *>> done;

    const auto nGroups = groups1().size();
    for (size_t n : Iota::GetRange(nGroups))
    {
        const auto &group1 = groups1().at(n);
        const auto &group2 = groups2().at(n);
        const auto *leaf1 = group1.front();
        const auto *leaf2 = group2.front();

        const auto pathTags1 = lhs.collectDecomposeTagsForLeaf(leaf1);
        const auto pathTags2 = rhs.collectDecomposeTagsForLeaf(leaf2);
        result = pathTags1 <=> pathTags2;
        if (result != 0)
        {
            return result;
        }

        // Test back to root
        const DecomposeNode *decomp1 = leaf1;
        const DecomposeNode *decomp2 = leaf2;
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
            result = decomp1->compareGraph(*decomp2);
            if (result != 0)
            {
                return result;
            }
            decomp1 = lhs.getParent(decomp1);
            decomp2 = rhs.getParent(decomp2);
        }
    }
    return result;
}

const VertexGrouping &DecomposeTree::getVertexGrouping() const
{
    return getRoot().getVertexGrouping();
}

const Graph::IGraphUs &DecomposeTree::getGraph() const
{
    return getRoot().getGraph();
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Factory

std::unique_ptr<IGraphCompare> DecomposeTreeFactory::createGraphCompare(const Graph::IGraphUs &graph) const
{
    return std::make_unique<DecomposeTree>(graph);
}
