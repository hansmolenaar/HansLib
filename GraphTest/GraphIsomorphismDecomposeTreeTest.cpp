#include <gtest/gtest.h>

#include "GraphIsomorphismDecomposeTree.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "GraphIsomorphismUtils.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{

void CheckVertexConservation(const GraphIsomorphism::DecomposeTree &decomposeTree, int expectNumLeaves)
{
    int numLeaves = 0;
    std::vector<Vertex> vertices;
    for (const auto *leaf : decomposeTree.getLeaves())
    {
        numLeaves += 1;
        const IGraphUs &graph = leaf->getGraph();
        for (Vertex v : graph.getVertexRange())
        {
            vertices.emplace_back(decomposeTree.getVertexInRoot(v, *leaf));
        }
    }

    if (expectNumLeaves > 0)
    {
        ASSERT_EQ(expectNumLeaves, numLeaves);
    }

    // Not vertices lost or added?
    const auto fullNumVertices = decomposeTree.getRoot().getGraph().getNumVertices();
    str::sort(vertices);
    ASSERT_EQ(vertices.size(), fullNumVertices);
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
    ASSERT_EQ(vertices.size(), fullNumVertices);
    ASSERT_EQ(vertices.front(), 0);
    ASSERT_EQ(vertices.back(), fullNumVertices - 1);
}

void CheckDecompose(const DecomposeTree &decomposeTree, int expectNumLeaves = -1)
{
    const auto &graph = decomposeTree.getRoot().getGraph();
    expectNumLeaves = expectNumLeaves > 0 ? expectNumLeaves : decomposeTree.getLeaves().size();
    CheckVertexConservation(decomposeTree, expectNumLeaves);

    // Try some permutations
    constexpr size_t numPermutation = 3;
    const Permutation trivial = Permutation::CreateTrivial(graph.getNumVertices());
    for (size_t n : Iota::GetRange(numPermutation))
    {
        const auto permutation = Permutation::CreateRandomShuffle(trivial, n);
        const auto graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const DecomposeTree decomposeTreePermuted(graphPermuted);
        CheckVertexConservation(decomposeTreePermuted, expectNumLeaves);
        const auto cmp = decomposeTree <=> decomposeTreePermuted;
        ASSERT_TRUE(cmp == std::weak_ordering::equivalent);
    }
}

void CheckDecomposeGraphList(const std::vector<std::unique_ptr<IGraphUs>> &graphs, Tag expectMultiplicities,
                             bool printMultipleDecompositions)
{
    std::vector<std::unique_ptr<DecomposeTree>> decompositions;
    for (const auto &graph : graphs)
    {
        decompositions.emplace_back(std::make_unique<DecomposeTree>(*graph));
    }

    for (const auto &tree : decompositions)
    {
        CheckDecompose(*tree);
    }

    auto decomposeTreeLess = [](const DecomposeTree *lhs, const DecomposeTree *rhs) { return *lhs < *rhs; };
    std::map<const DecomposeTree *, std::vector<const DecomposeTree *>, decltype(decomposeTreeLess)> multiplicityMap(
        decomposeTreeLess);
    for (const auto &dg : decompositions)
    {
        multiplicityMap[dg.get()].push_back(dg.get());
    }

    std::vector<size_t> multiplicities;
    for (const auto &itr : multiplicityMap)
    {
        multiplicities.push_back(itr.second.size());
    }

    if (printMultipleDecompositions)
    {
        for (const auto &itr : multiplicityMap)
        {
            if (itr.second.size() > 1)
            {
                std::cout << "size = " << itr.second.size() << "\n";
                for (const auto &tpm : itr.second)
                {
                    std::cout << tpm->getRoot().getGraph().getName() << "\n";
#if false
                    for (const auto &d : tpm->getDescriptions())
                    {
                        std::cout << d << "\n";
                    }
                    std::cout << "\n";
                    for (const auto *leaf : tpm->getLeaves())
                    {
                        GraphIsomorphism::toEdgeList(leaf->getGraph(), std::cout);
                    }

                    std::cout << "\n";
                    std::cout << "Vertex groups\n";
                    for (const auto *leaf : tpm->getLeaves())
                    {
                        std::cout << leaf->getVertexGrouping();
                    }
                    std::cout << "\n";
#endif
                }
                std::cout << "\n";
                std::cout << "\n";
            }
        }
    }

    const auto tag = CondenseSizeSequence(multiplicities);
    ASSERT_EQ(tag, expectMultiplicities);
}

void CheckDecomposeList(const std::vector<std::string> &g6list, Tag expectMultiplicities,
                        bool printMultipleDecompositions = false)
{
    std::vector<std::unique_ptr<IGraphUs>> graphs = UndirectedGraphFromG6::getGraphs(g6list);
    CheckDecomposeGraphList(graphs, expectMultiplicities, printMultipleDecompositions);
}

} // namespace

TEST(GraphIsomorphismDecomposeTreeTest, Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const auto xgraph = std::make_shared<XGraph>(*graph);
    auto decomposed = DecomposeNode::Create(xgraph);
    ASSERT_EQ(decomposed->getTag(), (Tag{1, 1, 1}));
    const DecomposeTree decomposeTree(std::move(decomposed));
    const auto descr = decomposeTree.getDescriptions();
    ASSERT_EQ(Single(descr), "Known graph: complete graph of order 1");
    ASSERT_EQ(decomposeTree.size(), 1);

    CheckDecompose(decomposeTree, 1);
}

TEST(GraphIsomorphismDecomposeTreeTest, Disconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const DecomposeTree decomposeTree(*graph);

    ASSERT_EQ(decomposeTree.size(), 1);
    const auto *leaf = Single(decomposeTree.getLeaves());
    ASSERT_EQ(Single(decomposeTree.collectDecomposeTagsForLeaf(leaf)), (Tag{1, 4, 2}));
    const auto descr = decomposeTree.getDescriptions();
    ASSERT_EQ(Single(descr), "Known graph: completely disconnected graph of order 2");

    CheckDecompose(decomposeTree, 1);
}

TEST(GraphIsomorphismDecomposeTreeTest, EdgePlusVertex)
{
    const auto graph = UndirectedGraphFromG6::Create("BO");
    const DecomposeTree decomposeTree(*graph);

    const auto leaves = decomposeTree.getLeaves();
    ASSERT_EQ(leaves.size(), 2);
    ASSERT_TRUE(leaves.at(0)->isLeaf());
    ASSERT_TRUE(leaves.at(1)->isLeaf());

    const auto grouping = decomposeTree.groupLeaves()();
    ASSERT_EQ(grouping.size(), 2);

    const auto *child0 = Single(grouping.at(0));
    ASSERT_EQ(child0->getGraph().getNumVertices(), 1);
    ASSERT_EQ(child0->getTag(), (Tag{1, 1, 1}));
    ASSERT_TRUE(child0->isLeaf());

    const auto *child1 = Single(grouping.at(1));
    ASSERT_EQ(child1->getGraph().getNumVertices(), 2);
    ASSERT_EQ(child1->getTag(), (Tag{1, 1, 2}));
    ASSERT_TRUE(child1->isLeaf());

    const auto tag0 = decomposeTree.collectDecomposeTagsForLeaf(child0);
    const auto tag1 = decomposeTree.collectDecomposeTagsForLeaf(child1);
    ASSERT_EQ(tag0, (std::vector<Tag>{Tag{1, 1, 1}, Tag{2, 1, 2}}));
    ASSERT_EQ(tag1, (std::vector<Tag>{Tag{1, 1, 2}, Tag{2, 1, 2}}));

    const auto descr = decomposeTree.getDescriptions();
    ASSERT_EQ(descr.size(), 2);
    ASSERT_EQ(descr.at(0),
              "Known graph: complete graph of order 1 -> Disconnected graph with components of order: 1 2");
    ASSERT_EQ(descr.at(1),
              "Known graph: complete graph of order 2 -> Disconnected graph with components of order: 1 2");
    CheckDecompose(decomposeTree, 2);
}

TEST(GraphIsomorphismDecomposeTreeTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    const DecomposeTree decomposeTree(*graph);

    const auto grouping = decomposeTree.groupLeaves()();
    ASSERT_EQ(grouping.size(), 2);

    const auto *child0 = Single(grouping.at(0));
    ASSERT_EQ(child0->getGraph().getNumVertices(), 2);
    ASSERT_EQ(child0->getTag(), (Tag{1, 1, 2}));
    ASSERT_TRUE(child0->isLeaf());

    const auto *child1 = Single(grouping.at(1));
    ASSERT_EQ(child1->getGraph().getNumVertices(), 2);
    ASSERT_EQ(child1->getTag(), (Tag{1, 4, 2}));
    ASSERT_TRUE(child1->isLeaf());

    const auto tag0 = decomposeTree.collectDecomposeTagsForLeaf(child0);
    const auto tag1 = decomposeTree.collectDecomposeTagsForLeaf(child1);
    ASSERT_EQ(tag0, (std::vector<Tag>{Tag{1, 1, 2}, Tag{3, 2, 2}}));
    ASSERT_EQ(tag1, (std::vector<Tag>{Tag{1, 4, 2}, Tag{3, 2, 2}}));

    const auto descr = decomposeTree.getDescriptions();
    ASSERT_EQ(descr.size(), 2);
    ASSERT_EQ(
        descr.at(0),
        "Known graph: complete graph of order 2 -> Complement is disconnected graph with components of order: 2 2");
    ASSERT_EQ(descr.at(1), "Known graph: completely disconnected graph of order 2 -> Complement is disconnected graph "
                           "with components of order: 2 2");

    CheckDecompose(decomposeTree, 2);
}

TEST(GraphIsomorphismDecomposeTreeTest, Pan3)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const DecomposeTree decomposeTree(*graph);
    const auto leaves = decomposeTree.groupLeaves();
    ASSERT_EQ(leaves().size(), 3);

    ASSERT_EQ(leaves().at(0).size(), 1);
    ASSERT_EQ(leaves().at(1).size(), 1);
    ASSERT_EQ(leaves().at(2).size(), 1);
    ASSERT_EQ(leaves().at(0).front()->getTag(), (Tag{1, 1, 1}));
    ASSERT_EQ(leaves().at(1).front()->getTag(), (Tag{1, 1, 1}));
    ASSERT_EQ(leaves().at(2).front()->getTag(), (Tag{1, 1, 2}));
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, *leaves().at(0).front()), 3);
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, *leaves().at(1).front()), 2);
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, *leaves().at(2).front()), 0);
    ASSERT_EQ(decomposeTree.getVertexInRoot(1, *leaves().at(2).front()), 1);
    CheckDecompose(decomposeTree, 3);
}

TEST(GraphIsomorphismDecomposeTreeTest, Dart)
{
    const auto graph = UndirectedGraphFromG6::Create("DvC");
    const DecomposeTree decomposeTree(*graph);
    CheckDecompose(decomposeTree, 3);
}

TEST(GraphIsomorphismDecomposeTreeTest, K5MinE)
{
    const auto graph = UndirectedGraphFromG6::Create("D~k");
    const DecomposeTree decomposeTree(*graph);
    CheckDecompose(decomposeTree, 2);
}

TEST(GraphIsomorphismDecomposeTreeTest, Gem)
{
    const auto graph = UndirectedGraphFromG6::Create("Dh{");
    const DecomposeTree decomposeTree(*graph);
    CheckDecompose(decomposeTree, 2);
}

TEST(GraphIsomorphismDecomposeTreeTest, ComplementKnownCycle6)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(6);
    const auto complement = std::make_shared<UndirectedGraph>(UndirectedGraph::CreateComplement(*graph));
    const DecomposeTree decomposeTree(*complement);
    CheckDecompose(decomposeTree, 1);
    const auto descr = decomposeTree.getDescriptions();
    ASSERT_EQ(Single(descr), "Complement is known graph: cycle of order 6");
    const auto tag = decomposeTree.collectDecomposeTagsForLeaf(&decomposeTree.getRoot());
    ASSERT_EQ(Single(tag), (Tag{4, 2, 6}));
}

TEST(GraphIsomorphismDecomposeTreeTest, ComplementKnownHouse)
{
    const auto house = UndirectedGraphFromG6::Create("DUw");
    const auto complement = std::make_shared<UndirectedGraph>(UndirectedGraph::CreateComplement(*house));
    const DecomposeTree decomposeTree(*complement);
    CheckDecompose(decomposeTree, 1);
}

TEST(GraphIsomorphismDecomposeTreeTest, X100)
{
    const auto graph = UndirectedGraphFromG6::Create("FgCNw");
    const DecomposeTree decomposeTree(*graph);
    CheckDecompose(decomposeTree, 3);
    const auto descr = decomposeTree.getDescriptions();
    ASSERT_EQ(descr.size(), 3);
    ASSERT_EQ(
        descr.at(0),
        "Known graph: complete graph of order 1 -> Complement is disconnected graph with components of order: 1 6");
    ASSERT_EQ(descr.at(1), "Known graph: path of order 3 -> Disconnected graph with components of order: 3 3 -> "
                           "Complement is disconnected graph with components of order: 1 6");
    ASSERT_EQ(descr.at(2), "Known graph: path of order 3 -> Disconnected graph with components of order: 3 3 -> "
                           "Complement is disconnected graph with components of order: 1 6");
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase1)
{
    const auto g0 = UndirectedGraphFromG6::CreateConnected("F@h^w");
    const auto g1 = UndirectedGraphFromG6::CreateConnected("F?lvw");

    const DecomposeTree decomposeTree0(*g0);
    const DecomposeTree decomposeTree1(*g1);
    const auto cmp = decomposeTree0 <=> decomposeTree1;
    ASSERT_TRUE(cmp != 0);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase2)
{
    const auto g0 = UndirectedGraphFromG6::Create("DR{");
    const auto g1 = UndirectedGraphFromG6::Create("DAK");
    const TaggedGraph tg0(*g0);
    const TaggedGraph tg1(*g1);
    const auto tgCompare = tg0 <=> tg1;
    ASSERT_TRUE(tgCompare != 0);

    const DecomposeTree decomposeTree0(*g0);
    const DecomposeTree decomposeTree1(*g1);
    const auto cmp = decomposeTree0 <=> decomposeTree1;
    ASSERT_TRUE(cmp != 0);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase3)
{
    const auto graph = UndirectedGraphFromG6::Create("D]w");
    const DecomposeTree decomposeTree(*graph);
    CheckDecompose(decomposeTree, 3);

    const auto leaves = decomposeTree.getLeaves();
    ASSERT_EQ(leaves.size(), 3);
    const auto grouping = decomposeTree.groupLeaves();
    ASSERT_EQ(grouping().size(), 3);
    ASSERT_EQ(grouping().at(0).front()->getTag(), (Tag{1, 1, 1}));
    ASSERT_EQ(grouping().at(1).front()->getTag(), (Tag{1, 1, 2}));
    ASSERT_EQ(grouping().at(2).front()->getTag(), (Tag{1, 4, 2}));

    const auto descr = decomposeTree.getDescriptions();
    ASSERT_EQ(descr.size(), 3);
    ASSERT_EQ(descr.at(0), "Known graph: complete graph of order 1 -> Disconnected graph with components of order: 1 2 "
                           "-> Complement is disconnected graph with components of order: 2 3");
    ASSERT_EQ(descr.at(1), "Known graph: complete graph of order 2 -> Disconnected graph with components of order: 1 2 "
                           "-> Complement is disconnected graph with components of order: 2 3");
    ASSERT_EQ(descr.at(2), "Known graph: completely disconnected graph of order 2 -> Complement is disconnected graph "
                           "with components of order: 2 3");
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase4)
{
    // Bipartite (2,3)
    const auto graph = UndirectedGraphFromG6::Create("DFw");
    const DecomposeTree decomposeTree(*graph);
    CheckDecompose(decomposeTree, 2);

    const auto leaves = decomposeTree.getLeaves();
    ASSERT_EQ(leaves.size(), 2);
    const auto grouping = decomposeTree.groupLeaves();
    ASSERT_EQ(grouping().size(), 2);
    const auto *leaf2 = Single(grouping().at(0));
    const auto *leaf3 = Single(grouping().at(1));
    ASSERT_EQ(leaf2->getTag(), (Tag{1, 4, 2}));
    ASSERT_EQ(leaf3->getTag(), (Tag{1, 4, 3}));
    ASSERT_EQ(decomposeTree.collectDecomposeTagsForLeaf(leaf2), (std::vector<Tag>{{1, 4, 2}, {3, 2, 3}}));
    ASSERT_EQ(decomposeTree.collectDecomposeTagsForLeaf(leaf3), (std::vector<Tag>{{1, 4, 3}, {3, 2, 3}}));
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, *leaf2), 3);
    ASSERT_EQ(decomposeTree.getVertexInRoot(1, *leaf2), 4);
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, *leaf3), 0);
    ASSERT_EQ(decomposeTree.getVertexInRoot(1, *leaf3), 1);
    ASSERT_EQ(decomposeTree.getVertexInRoot(2, *leaf3), 2);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase5)
{
    // Bipartite (2,3) + singleton
    const auto graph = UndirectedGraphFromG6::Create("E?\\o");
    const DecomposeTree decomposeTree(*graph);
    CheckDecompose(decomposeTree, 3);

    const auto leaves = decomposeTree.getLeaves();
    ASSERT_EQ(leaves.size(), 3);
    const auto grouping = decomposeTree.groupLeaves();
    ASSERT_EQ(grouping().size(), 3);
    const auto *leaf1 = Single(grouping().at(0));
    const auto *leaf2 = Single(grouping().at(1));
    const auto *leaf3 = Single(grouping().at(2));
    ASSERT_EQ(decomposeTree.collectDecomposeTagsForLeaf(leaf1), (std::vector<Tag>{{1, 1, 1}, {2, 1, 5}}));
    ASSERT_EQ(decomposeTree.collectDecomposeTagsForLeaf(leaf2), (std::vector<Tag>{{1, 4, 2}, {3, 2, 3}, {2, 1, 5}}));
    ASSERT_EQ(decomposeTree.collectDecomposeTagsForLeaf(leaf3), (std::vector<Tag>{{1, 4, 3}, {3, 2, 3}, {2, 1, 5}}));
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, *leaf1), 0);
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, *leaf2), 4);
    ASSERT_EQ(decomposeTree.getVertexInRoot(1, *leaf2), 5);
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, *leaf3), 1);
    ASSERT_EQ(decomposeTree.getVertexInRoot(1, *leaf3), 2);
    ASSERT_EQ(decomposeTree.getVertexInRoot(2, *leaf3), 3);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase6)
{
    const auto g0 = UndirectedGraphFromG6::Create("HR]~Mv~");
    const auto g1 = UndirectedGraphFromG6::Create("HRl~Mv~");

    const DecomposeTree decompose0(*g0);
    const DecomposeTree decompose1(*g1);

    auto descr0 = decompose0.getDescriptions();
    EXPECT_EQ(descr0.size(), 2);
    EXPECT_EQ(descr0.at(0), "Graph of order 8 cannot be transformed -> Omit 3 edges -> Complement is disconnected "
                            "graph with components of order: 1 8");
    EXPECT_EQ(
        descr0.at(1),
        "Known graph: complete graph of order 1 -> Complement is disconnected graph with components of order: 1 8");

    auto descr1 = decompose1.getDescriptions();
    EXPECT_EQ(descr1.size(), 2);
    EXPECT_EQ(descr1.at(0), "Graph of order 8 cannot be transformed -> Omit 1 edges -> Omit 2 edges -> Complement is "
                            "disconnected graph with components of order: 1 8");
    EXPECT_EQ(
        descr1.at(1),
        "Known graph: complete graph of order 1 -> Complement is disconnected graph with components of order: 1 8");

    const auto cmp = decompose0 <=> decompose1;
    ASSERT_TRUE(cmp != 0);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase7)
{
    const auto g0 = UndirectedGraphFromG6::Create("F?~vg");
    const auto g1 = UndirectedGraphFromG6::Create("F@~v_");
    const TaggedGraph tg0(*g0);
    const TaggedGraph tg1(*g1);
    const auto tgCompare = tg0 <=> tg1;
    ASSERT_TRUE(tgCompare != 0);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase8_411)
{
    UndirectedGraph cycle4(6);
    cycle4.addEdge(0, 1);
    cycle4.addEdge(1, 2);
    cycle4.addEdge(2, 3);
    cycle4.addEdge(3, 0);

    UndirectedGraph g0 = cycle4;
    UndirectedGraph g1 = cycle4;

    // g0
    g0.addEdge(4, 0);
    g0.addEdge(4, 3);
    g0.addEdge(5, 1);
    g0.addEdge(5, 2);

    // g1
    g1.addEdge(4, 1);
    g1.addEdge(4, 3);
    g1.addEdge(5, 0);
    g1.addEdge(5, 2);

    const TaggedGraph tg0(g0);
    const TaggedGraph tg1(g1);
    const auto tgCompare = tg0 <=> tg1;
    ASSERT_TRUE(tgCompare != 0);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase8_422)
{
    UndirectedGraph base(8);
    // cyle 4
    base.addWalk({0, 1, 2, 3, 0});

    // 2 edges
    base.addEdge(4, 5);
    base.addEdge(6, 7);

    base.addEdge(0, 4);
    base.addEdge(0, 5);
    base.addEdge(0, 7);

    UndirectedGraph g0 = base;
    UndirectedGraph g1 = base;

    // g0
    g0.addEdge(1, 4);
    g0.addEdge(1, 5);
    g0.addEdge(1, 6);

    g0.addEdge(2, 6);
    g0.addEdge(2, 7);
    g0.addEdge(2, 4);

    g0.addEdge(3, 6);
    g0.addEdge(3, 7);
    g0.addEdge(3, 5);

    // g1
    g1.addEdge(2, 4);
    g1.addEdge(2, 5);
    g1.addEdge(2, 6);

    g1.addEdge(1, 6);
    g1.addEdge(1, 7);
    g1.addEdge(1, 4);

    g1.addEdge(3, 6);
    g1.addEdge(3, 7);
    g1.addEdge(3, 5);

    const TaggedGraph tg0(g0);
    const TaggedGraph tg1(g1);
    auto tgCompare = tg0 <=> tg1;
    ASSERT_EQ(tgCompare, std::weak_ordering::equivalent);

    std::vector<std::unique_ptr<IGraphUs>> graphs;
    graphs.emplace_back(std::make_unique<UndirectedGraph>(g0));
    graphs.emplace_back(std::make_unique<UndirectedGraph>(g1));
    CheckDecomposeGraphList(graphs, Tag{1, 2}, false);

    const auto g0c = UndirectedGraph::CreateComplement(g0);
    const auto g1c = UndirectedGraph::CreateComplement(g1);
    const TaggedGraph tg0c(g0c);
    const TaggedGraph tg1c(g1c);
    tgCompare = tg0c <=> tg1c;
    ASSERT_NE(tgCompare, std::weak_ordering::equivalent);
    graphs.clear();
    graphs.emplace_back(std::make_unique<UndirectedGraph>(g0c));
    graphs.emplace_back(std::make_unique<UndirectedGraph>(g1c));
    CheckDecomposeGraphList(graphs, Tag{1, 2}, false);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase9)
{
    UndirectedGraph g0(8);
    g0.addWalk({0, 1, 2, 3, 0});
    auto g1 = g0;

    g0.addWalk({0, 4, 5, 1});
    g0.addWalk({2, 6, 7, 3});

    g1.addWalk({0, 4, 5, 2});
    g1.addWalk({1, 6, 7, 3});

    const TaggedGraph tg0(g0);
    const TaggedGraph tg1(g1);
    const auto tgCompare = tg0 <=> tg1;
    ASSERT_NE(tgCompare, std::weak_ordering::equivalent);
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList3)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_3(), Tag{1, 4});
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList4)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_4(), Tag{1, 11});
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList5)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_5(), {1, 34});
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList6)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_6(), {1, 155});
}
TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList7)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_7(), {1, 302});
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList8)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_8(), {1, 746});
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList9)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_9(), {1, 481});
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList10)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_10(), {1, 715});
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList11)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_11_connected(), {1, 325});
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList12)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_12_connected(), {1, 566, 2, 11, 3, 1, 4, 1}, true);
}
