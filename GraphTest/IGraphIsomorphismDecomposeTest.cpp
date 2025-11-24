#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "GraphIsomorphismUtils.h"
#include "IGraphIsomorphismDecompose.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{

void CheckVertexConservation(const GraphIsomorphism::ToParentMap &toParent, int expectNumLeaves)
{
    int numLeaves = 0;
    std::vector<GraphVertex> vertices;
    for (const auto *leaf : toParent.getLeaves())
    {
        numLeaves += 1;
        const IGraphUs &graph = leaf->getGraph();
        const auto numVertices = graph.getNumVertices();
        for (GraphVertex v = 0; v < numVertices; ++v)
        {
            vertices.emplace_back(toParent.getVertexInRoot(v, leaf));
        }
    }

    if (expectNumLeaves > 0)
    {
        ASSERT_EQ(expectNumLeaves, numLeaves);
    }

    // Not vertices lost or added?
    const auto fullNumVertices = toParent.getRoot()->getGraph().getNumVertices();
    str::sort(vertices);
    ASSERT_EQ(vertices.size(), fullNumVertices);
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
    ASSERT_EQ(vertices.size(), fullNumVertices);
    ASSERT_EQ(vertices.front(), 0);
    ASSERT_EQ(vertices.back(), fullNumVertices - 1);
}

void CheckDecompose(const IGraphUs &graph, int expectNumLeaves = -1)
{
    const auto decomposed = IDecompose::Create(graph);
    const GraphIsomorphism::ToParentMap toParent(decomposed.get());

    expectNumLeaves = expectNumLeaves > 0 ? expectNumLeaves : toParent.getLeaves().size();
    CheckVertexConservation(toParent, expectNumLeaves);

    // Try some permutations
    constexpr size_t numPermutation = 3;
    const Permutation trivial = Permutation::CreateTrivial(graph.getNumVertices());
    for (size_t n = 0; n < numPermutation; ++n)
    {
        const auto permutation = Permutation::CreateRandomShuffle(trivial, n);
        const auto graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const auto decomposedPermuted = IDecompose::Create(graphPermuted);
        const GraphIsomorphism::ToParentMap toParentPermuted(decomposed.get());
        CheckVertexConservation(toParentPermuted, expectNumLeaves);
        const auto cmp = toParent <=> toParentPermuted;
        ASSERT_TRUE(cmp == std::weak_ordering::equivalent);
    }
}

void CheckDecomposeList(const std::vector<std::string> &g6list, Tag expectMultiplicities)
{
    std::vector<std::unique_ptr<Graph::IGraphUs>> graphs = UndirectedGraphFromG6::getGraphs(g6list);
    for (const auto &graph : graphs)
    {
        CheckDecompose(*graph);
    }

    std::vector<std::unique_ptr<GraphIsomorphism::IDecompose>> decomposedGraphs(graphs.size());
    str::transform(graphs, decomposedGraphs.begin(),
                   [](const auto &g) { return GraphIsomorphism::IDecompose::Create(*g); });

    std::vector<GraphIsomorphism::ToParentMap> toParentMaps;
    str::transform(decomposedGraphs, std::back_inserter(toParentMaps),
                   [](const auto &dg) { return GraphIsomorphism::ToParentMap(dg.get()); });

    std::map<GraphIsomorphism::ToParentMap, size_t> multiplicityMap;
    for (const auto &dg : decomposedGraphs)
    {
        const GraphIsomorphism::ToParentMap tpm(dg.get());
        multiplicityMap[tpm] += 1;
    }

    std::vector<size_t> multiplicities;
    for (const auto &itr : multiplicityMap)
    {
        multiplicities.push_back(itr.second);
    }
    const auto tag = CondenseSizeSequence(multiplicities);
    ASSERT_EQ(tag, expectMultiplicities);
}

void PrintMultipleDecompositions(const std::vector<std::string> &g6list)
{
    std::vector<std::unique_ptr<Graph::IGraphUs>> graphs = UndirectedGraphFromG6::getGraphs(g6list);
    std::vector<std::unique_ptr<GraphIsomorphism::IDecompose>> decomposedGraphs(graphs.size());
    str::transform(graphs, decomposedGraphs.begin(),
                   [](const auto &g) { return GraphIsomorphism::IDecompose::Create(*g); });

    std::vector<GraphIsomorphism::ToParentMap> toParentMaps;
    str::transform(decomposedGraphs, std::back_inserter(toParentMaps),
                   [](const auto &dg) { return GraphIsomorphism::ToParentMap(dg.get()); });

    std::map<GraphIsomorphism::ToParentMap, std::vector<GraphIsomorphism::ToParentMap>> multiplicityMap;
    for (const auto &dg : decomposedGraphs)
    {
        const GraphIsomorphism::ToParentMap tpm(dg.get());
        multiplicityMap[tpm].emplace_back(tpm);
    }

    for (const auto &itr : multiplicityMap)
    {
        if (itr.second.size() > 1)
        {
            std::cout << "size = " << itr.second.size() << "\n";
            for (const auto &tpm : itr.second)
            {
                std::cout << tpm.getRoot()->getGraph().getName() << "\n";
            }
            std::cout << "\n";
            std::cout << "\n";
        }
    }
}

} // namespace

TEST(IGraphIsomorphismDecomposeTest, GetToParentMapSingleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const auto decomposed = IDecompose::Create(*graph);
    ASSERT_EQ(decomposed->getTag(), (Tag{3, 1, 1}));
    const ToParentMap toParent(decomposed.get());
    ASSERT_EQ(toParent.size(), 1);
    ASSERT_TRUE(toParent.isRoot(decomposed.get()));

    CheckDecompose(*graph, 1);
}

TEST(IGraphIsomorphismDecomposeTest, GetToParentMapDisconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto decomposed = IDecompose::Create(*graph);
    ASSERT_EQ(decomposed->getTag(), (Tag{3, 4, 2}));
    const auto *root = decomposed.get();
    const ToParentMap toParent(root);
    ASSERT_EQ(toParent.size(), 1);
    ASSERT_TRUE(toParent.isRoot(decomposed.get()));
    ASSERT_EQ(toParent.getParent(root), nullptr);

    CheckDecompose(*graph, 1);
}

TEST(IGraphIsomorphismDecomposeTest, Disconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto decomposed = IDecompose::Create(*graph);
    ASSERT_EQ(decomposed->getTag(), (Tag{3, 4, 2}));
    CheckDecompose(*graph, 1);
}

TEST(IGraphIsomorphismDecomposeTest, EdgePlusVertex)
{
    const auto graph = UndirectedGraphFromG6::Create("BO");
    const auto decomposed = IDecompose::Create(*graph);
    ASSERT_EQ(decomposed->getTag(), (Tag{1, 1, 2}));
    const ToParentMap toParent(decomposed.get());

    const auto &grouping = decomposed->getGroupingChildren()();
    ASSERT_EQ(grouping.size(), 2);

    const auto *child0 = dynamic_cast<const DecomposeKnown *>(Single(grouping.at(0)));
    ASSERT_EQ(child0->getGraph().getNumVertices(), 1);
    ASSERT_EQ(child0->getTag(), (Tag{3, 1, 1}));
    ASSERT_TRUE(child0->isLeaf());

    const auto *child1 = dynamic_cast<const DecomposeKnown *>(Single(grouping.at(1)));
    ASSERT_EQ(child1->getGraph().getNumVertices(), 2);
    ASSERT_EQ(child1->getTag(), (Tag{3, 1, 2}));
    ASSERT_TRUE(child1->isLeaf());

    const auto tag0 = toParent.collectDecomposeTagsForLeaf(child0);
    const auto tag1 = toParent.collectDecomposeTagsForLeaf(child1);
    ASSERT_EQ(tag0, (std::vector<Tag>{Tag{3, 1, 1}, Tag{1, 1, 2}}));
    ASSERT_EQ(tag1, (std::vector<Tag>{Tag{3, 1, 2}, Tag{1, 1, 2}}));
    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, Pan3)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const auto decomposed = IDecompose::Create(*graph);
    const ToParentMap toParent(decomposed.get());
    const auto leaves = toParent.groupLeaves();
    ASSERT_EQ(leaves().size(), 3);
    ASSERT_EQ(leaves().at(0).size(), 1);
    ASSERT_EQ(leaves().at(1).size(), 1);
    ASSERT_EQ(leaves().at(2).size(), 1);
    ASSERT_EQ(leaves().at(0).front()->getTag(), (Tag{3, 1, 1}));
    ASSERT_EQ(leaves().at(1).front()->getTag(), (Tag{3, 1, 1}));
    ASSERT_EQ(leaves().at(2).front()->getTag(), (Tag{3, 1, 2}));
    ASSERT_EQ(toParent.getVertexInRoot(0, leaves().at(0).front()), 3);
    ASSERT_EQ(toParent.getVertexInRoot(0, leaves().at(1).front()), 2);
    ASSERT_EQ(toParent.getVertexInRoot(0, leaves().at(2).front()), 0);
    ASSERT_EQ(toParent.getVertexInRoot(1, leaves().at(2).front()), 1);
}

TEST(IGraphIsomorphismDecomposeTest, Paw)
{
    const auto graph = UndirectedGraphFromG6::Create("Cx");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, Dart)
{
    const auto graph = UndirectedGraphFromG6::Create("DvC");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, K5MinE)
{
    const auto graph = UndirectedGraphFromG6::Create("D~k");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, Gem)
{
    const auto graph = UndirectedGraphFromG6::Create("Dh{");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, X100)
{
    const auto graph = UndirectedGraphFromG6::Create("FgCNw");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, SpecialCase1)
{
    const auto g0 = UndirectedGraphFromG6::CreateConnected("F@h^w");
    const auto g1 = UndirectedGraphFromG6::CreateConnected("F?lvw");

    const auto decompose0 = IDecompose::Create(*g0);
    const auto decompose1 = IDecompose::Create(*g1);
    const ToParentMap map0(decompose0.get());
    const ToParentMap map1(decompose1.get());
    const auto cmp = map0 <=> map1;
    ASSERT_TRUE(cmp != 0);
}

TEST(IGraphIsomorphismDecomposeTest, SpecialCase2)
{
    const auto g0 = UndirectedGraphFromG6::Create("DR{");
    const auto g1 = UndirectedGraphFromG6::Create("DAK");
    const TaggedGraph tg0(*g0);
    const TaggedGraph tg1(*g1);
    const auto tgCompare = tg0 <=> tg1;
    ASSERT_TRUE(tgCompare != 0);

    const auto decompose0 = IDecompose::Create(*g0);
    const auto decompose1 = IDecompose::Create(*g1);
    const ToParentMap map0(decompose0.get());
    const ToParentMap map1(decompose1.get());
    const auto cmp = map0 <=> map1;
    ASSERT_TRUE(cmp != 0);
}

TEST(IGraphIsomorphismDecomposeTest, SpecialCase3)
{
    const auto graph = UndirectedGraphFromG6::Create("D]w");
    const auto decompose = IDecompose::Create(*graph, true);
    ASSERT_FALSE(decompose->isLeaf());

    const ToParentMap toParent(decompose.get());
    const auto leaves = toParent.getLeaves();
    ASSERT_EQ(leaves.size(), 2);
    const auto grouping = toParent.groupLeaves();
    ASSERT_EQ(grouping().size(), 2);
    ASSERT_EQ(grouping().at(0).front()->getTag(), (Tag{3, 1, 2}));
    ASSERT_EQ(grouping().at(1).front()->getTag(), (Tag{3, 3, 3}));
}

TEST(IGraphIsomorphismDecomposeTest, SpecialCase4)
{
    // Bipartite (2,3)
    const auto graph = UndirectedGraphFromG6::Create("DFw");
    const auto decompose = IDecompose::Create(*graph, true);
    ASSERT_FALSE(decompose->isLeaf());

    const ToParentMap toParent(decompose.get());
    const auto leaves = toParent.getLeaves();
    ASSERT_EQ(leaves.size(), 2);
    const auto grouping = toParent.groupLeaves();
    ASSERT_EQ(grouping().size(), 2);
    const auto *leaf2 = Single(grouping().at(0));
    const auto *leaf3 = Single(grouping().at(1));
    ASSERT_EQ(leaf2->getTag(), (Tag{3, 1, 2}));
    ASSERT_EQ(leaf3->getTag(), (Tag{3, 1, 3}));
    ASSERT_EQ(toParent.collectDecomposeTagsForLeaf(leaf2), (std::vector<Tag>{{3, 1, 2}, {1, 2, 3}, {4}}));
    ASSERT_EQ(toParent.collectDecomposeTagsForLeaf(leaf3), (std::vector<Tag>{{3, 1, 3}, {1, 2, 3}, {4}}));
    ASSERT_EQ(toParent.getVertexInRoot(0, leaf2), 3);
    ASSERT_EQ(toParent.getVertexInRoot(1, leaf2), 4);
    ASSERT_EQ(toParent.getVertexInRoot(0, leaf3), 0);
    ASSERT_EQ(toParent.getVertexInRoot(1, leaf3), 1);
    ASSERT_EQ(toParent.getVertexInRoot(2, leaf3), 2);
    CheckVertexConservation(toParent, 2);
}

TEST(IGraphIsomorphismDecomposeTest, SpecialCase5)
{
    // Bipartite (2,3) + singletom
    const auto graph = UndirectedGraphFromG6::Create("E?\\o");
    const auto decompose = IDecompose::Create(*graph, true);
    ASSERT_FALSE(decompose->isLeaf());

    const ToParentMap toParent(decompose.get());
    const auto leaves = toParent.getLeaves();
    ASSERT_EQ(leaves.size(), 3);
    const auto grouping = toParent.groupLeaves();
    ASSERT_EQ(grouping().size(), 3);
    const auto *leaf1 = Single(grouping().at(0));
    const auto *leaf2 = Single(grouping().at(1));
    const auto *leaf3 = Single(grouping().at(2));
    ASSERT_EQ(toParent.collectDecomposeTagsForLeaf(leaf1), (std::vector<Tag>{{3, 1, 1}, {1, 1, 5}}));
    ASSERT_EQ(toParent.collectDecomposeTagsForLeaf(leaf2), (std::vector<Tag>{{3, 1, 2}, {1, 2, 3}, {4}, {1, 1, 5}}));
    ASSERT_EQ(toParent.collectDecomposeTagsForLeaf(leaf3), (std::vector<Tag>{{3, 1, 3}, {1, 2, 3}, {4}, {1, 1, 5}}));
    ASSERT_EQ(toParent.getVertexInRoot(0, leaf1), 0);
    ASSERT_EQ(toParent.getVertexInRoot(0, leaf2), 4);
    ASSERT_EQ(toParent.getVertexInRoot(1, leaf2), 5);
    ASSERT_EQ(toParent.getVertexInRoot(0, leaf3), 1);
    ASSERT_EQ(toParent.getVertexInRoot(1, leaf3), 2);
    ASSERT_EQ(toParent.getVertexInRoot(2, leaf3), 3);
    CheckVertexConservation(toParent, 3);
}
TEST(IGraphIsomorphismDecomposeTest, BullIsSelfComplement)
{
    const auto graph = UndirectedGraphLibrary::Get_Bull();
    const auto decompose = IDecompose::Create(*graph, true);
    ASSERT_TRUE(decompose->isLeaf());
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList3)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_3(), Tag{1, 4});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList4)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_4(), Tag{1, 11});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList5)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_5(), {1, 34});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList6)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_6(), {1, 155});
}
TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList7)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_7(), {1, 302});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList8)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_8(), {1, 744, 2, 1});
    // PrintMultipleDecompositions(UndirectedGraphFromG6::getListNumVertices_8());
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList9)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_9(), {1, 469, 2, 6});
}
TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList10)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_10(), {1, 704, 2, 1, 3, 1, 6, 1});
}
