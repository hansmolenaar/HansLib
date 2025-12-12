#include "GraphIsomorphismDecomposeTree.h"
#include "GraphIsomorphismTaggedGraph.h"
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

std::string DecomposeTree::getDescriptions() const
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

Grouping<const DecomposeNode *> DecomposeTree::groupLeaves() const
{
    auto cmp = [this](const DecomposeNode *leaf1, const DecomposeNode *leaf2) {
        return compareLeaves(leaf1, leaf2) == std::weak_ordering::less;
    };
    const Grouping<const DecomposeNode *> grouping(getLeaves(), cmp);
    return grouping;
}

std::weak_ordering DecomposeTree::operator<=>(const DecomposeTree &map2) const
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
