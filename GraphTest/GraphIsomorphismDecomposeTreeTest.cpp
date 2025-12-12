#include <gtest/gtest.h>

#include "GraphIsomorphismDecomposeTree.h"
#include "GraphIsomorphismTaggedGraph.h"
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
        const auto numVertices = graph.getNumVertices();
        for (Vertex v = 0; v < numVertices; ++v)
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
    expectNumLeaves = expectNumLeaves > 0 ? expectNumLeaves : decomposeTree.getLeaves().size();
    CheckVertexConservation(decomposeTree, expectNumLeaves);

#if false
    // Try some permutations
    constexpr size_t numPermutation = 3;
    const Permutation trivial = Permutation::CreateTrivial(graph.getNumVertices());
    for (size_t n = 0; n < numPermutation; ++n)
    {
        const auto permutation = Permutation::CreateRandomShuffle(trivial, n);
        const auto graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const auto decomposedPermuted = IDecompose::Create(graphPermuted);
        const DecomposeTree decomposeTreePermuted(decomposed.get());
        CheckVertexConservation(decomposeTreePermuted, expectNumLeaves);
        const auto cmp = decomposeTree <=> decomposeTreePermuted;
        ASSERT_TRUE(cmp == std::weak_ordering::equivalent);
    }
#endif
}

#if false
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

    std::vector<DecomposeTree> decomposeTreeMaps;
    str::transform(decomposedGraphs, std::back_inserter(decomposeTreeMaps),
                   [](const auto &dg) { return DecomposeTree(dg.get()); });

    std::map<DecomposeTree, size_t> multiplicityMap;
    for (const auto &dg : decomposedGraphs)
    {
        const DecomposeTree tpm(dg.get());
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

    std::vector<DecomposeTree> decomposeTreeMaps;
    str::transform(decomposedGraphs, std::back_inserter(decomposeTreeMaps),
                   [](const auto &dg) { return DecomposeTree(dg.get()); });

    std::map<DecomposeTree, std::vector<DecomposeTree>> multiplicityMap;
    for (const auto &dg : decomposedGraphs)
    {
        const DecomposeTree tpm(dg.get());
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
#endif
} // namespace

TEST(GraphIsomorphismDecomposeTreeTest, Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const auto tgraph = std::make_shared<TaggedGraph>(*graph);
    auto decomposed = DecomposeNode::Create(tgraph);
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
              "Known graph: complete graph of order 2 -> Disconnected graph with components of order: 1 2");
    ASSERT_EQ(descr.at(1),
              "Known graph: complete graph of order 1 -> Disconnected graph with components of order: 1 2");
    CheckDecompose(*graph, 2);
}

#if false
TEST(GraphIsomorphismDecomposeTreeTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(GraphIsomorphismDecomposeTreeTest, Pan3)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const auto decomposed = IDecompose::Create(*graph);
    const DecomposeTree decomposeTree(decomposed.get());
    const auto leaves = decomposeTree.groupLeaves();
    ASSERT_EQ(leaves().size(), 3);
    ASSERT_EQ(leaves().at(0).size(), 1);
    ASSERT_EQ(leaves().at(1).size(), 1);
    ASSERT_EQ(leaves().at(2).size(), 1);
    ASSERT_EQ(leaves().at(0).front()->getTag(), (Tag{3, 1, 1}));
    ASSERT_EQ(leaves().at(1).front()->getTag(), (Tag{3, 1, 1}));
    ASSERT_EQ(leaves().at(2).front()->getTag(), (Tag{3, 1, 2}));
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, leaves().at(0).front()), 3);
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, leaves().at(1).front()), 2);
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, leaves().at(2).front()), 0);
    ASSERT_EQ(decomposeTree.getVertexInRoot(1, leaves().at(2).front()), 1);
}

TEST(GraphIsomorphismDecomposeTreeTest, Paw)
{
    const auto graph = UndirectedGraphFromG6::Create("Cx");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(GraphIsomorphismDecomposeTreeTest, Dart)
{
    const auto graph = UndirectedGraphFromG6::Create("DvC");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(GraphIsomorphismDecomposeTreeTest, K5MinE)
{
    const auto graph = UndirectedGraphFromG6::Create("D~k");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(GraphIsomorphismDecomposeTreeTest, Gem)
{
    const auto graph = UndirectedGraphFromG6::Create("Dh{");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(GraphIsomorphismDecomposeTreeTest, ComplementKnownHouse)
{
    const auto house = UndirectedGraphFromG6::Create("DUw");
    const auto complement = std::make_shared<UndirectedGraph>(UndirectedGraph::CreateComplement(*house));
    const auto decomposed = DecomposeComplementKnown::tryCreate(*house, complement);
    ASSERT_TRUE(static_cast<bool>(decomposed));
    CheckDecompose(*house, 1);
}

TEST(GraphIsomorphismDecomposeTreeTest, ComplementKnownCycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const auto complement = std::make_shared<UndirectedGraph>(UndirectedGraph::CreateComplement(*graph));
    const auto decomposed = DecomposeComplementKnown::tryCreate(*graph, complement);
    ASSERT_TRUE(static_cast<bool>(decomposed));
    const DecomposeTree decomposeTree(decomposed.get());
    ASSERT_EQ(decomposeTree.size(), 2);
    CheckDecompose(*graph, 1);
}

TEST(GraphIsomorphismDecomposeTreeTest, ComplementKnownSpecialCase1)
{
    const auto graph = UndirectedGraphFromG6::Create("GJOg~{");
    const auto decomposed = IDecompose::Create(*graph);
    const DecomposeTree decomposeTree(decomposed.get());
    const auto descr = decomposeTree.getDescriptions();
    CheckDecompose(*graph, 3);
}

TEST(GraphIsomorphismDecomposeTreeTest, ComplementDisconnectedPath2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const auto complement = std::make_shared<UndirectedGraph>(UndirectedGraph::CreateComplement(*graph));
    const auto decomposed = DecomposeComplementDisconnected::tryCreate(*graph, complement);
    ASSERT_TRUE(static_cast<bool>(decomposed));
    const DecomposeTree decomposeTree(decomposed.get());
    ASSERT_EQ(decomposeTree.size(), 4);
}

TEST(GraphIsomorphismDecomposeTreeTest, ComplementDisconnectedClaw)
{
    const auto graph = UndirectedGraphLibrary::Get_Claw();
    const auto complement = std::make_shared<UndirectedGraph>(UndirectedGraph::CreateComplement(*graph));
    const auto decomposed = DecomposeComplementDisconnected::tryCreate(*graph, complement);
    ASSERT_TRUE(static_cast<bool>(decomposed));
    const DecomposeTree decomposeTree(decomposed.get());
    ASSERT_EQ(decomposeTree.size(), 4);
}

TEST(GraphIsomorphismDecomposeTreeTest, X100)
{
    const auto graph = UndirectedGraphFromG6::Create("FgCNw");
    const auto decomposed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase1)
{
    const auto g0 = UndirectedGraphFromG6::CreateConnected("F@h^w");
    const auto g1 = UndirectedGraphFromG6::CreateConnected("F?lvw");

    const auto decompose0 = IDecompose::Create(*g0);
    const auto decompose1 = IDecompose::Create(*g1);
    const DecomposeTree map0(decompose0.get());
    const DecomposeTree map1(decompose1.get());
    const auto cmp = map0 <=> map1;
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

    const auto decompose0 = IDecompose::Create(*g0);
    const auto decompose1 = IDecompose::Create(*g1);
    const DecomposeTree map0(decompose0.get());
    const DecomposeTree map1(decompose1.get());
    const auto cmp = map0 <=> map1;
    ASSERT_TRUE(cmp != 0);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase3)
{
    const auto graph = UndirectedGraphFromG6::Create("D]w");
    const auto decompose = IDecompose::Create(*graph);
    ASSERT_FALSE(decompose->isLeaf());

    const DecomposeTree decomposeTree(decompose.get());
    const auto leaves = decomposeTree.getLeaves();
    ASSERT_EQ(leaves.size(), 2);
    const auto grouping = decomposeTree.groupLeaves();
    ASSERT_EQ(grouping().size(), 2);
    ASSERT_EQ(grouping().at(0).front()->getTag(), (Tag{3, 1, 2}));
    ASSERT_EQ(grouping().at(1).front()->getTag(), (Tag{3, 1, 3}));
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase4)
{
    // Bipartite (2,3)
    const auto graph = UndirectedGraphFromG6::Create("DFw");
    const auto decompose = IDecompose::Create(*graph);
    ASSERT_FALSE(decompose->isLeaf());

    const DecomposeTree decomposeTree(decompose.get());
    const auto leaves = decomposeTree.getLeaves();
    ASSERT_EQ(leaves.size(), 2);
    const auto grouping = decomposeTree.groupLeaves();
    ASSERT_EQ(grouping().size(), 2);
    const auto *leaf2 = Single(grouping().at(0));
    const auto *leaf3 = Single(grouping().at(1));
    ASSERT_EQ(leaf2->getTag(), (Tag{3, 1, 2}));
    ASSERT_EQ(leaf3->getTag(), (Tag{3, 1, 3}));
    ASSERT_EQ(decomposeTree.collectDecomposeTagsForLeaf(leaf2), (std::vector<Tag>{{3, 1, 2}, {1, 2, 3}, {6}}));
    ASSERT_EQ(decomposeTree.collectDecomposeTagsForLeaf(leaf3), (std::vector<Tag>{{3, 1, 3}, {1, 2, 3}, {6}}));
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, leaf2), 3);
    ASSERT_EQ(decomposeTree.getVertexInRoot(1, leaf2), 4);
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, leaf3), 0);
    ASSERT_EQ(decomposeTree.getVertexInRoot(1, leaf3), 1);
    ASSERT_EQ(decomposeTree.getVertexInRoot(2, leaf3), 2);
    CheckVertexConservation(decomposeTree, 2);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase5)
{
    // Bipartite (2,3) + singletom
    const auto graph = UndirectedGraphFromG6::Create("E?\\o");
    const auto decompose = IDecompose::Create(*graph);
    ASSERT_FALSE(decompose->isLeaf());

    const DecomposeTree decomposeTree(decompose.get());
    const auto leaves = decomposeTree.getLeaves();
    ASSERT_EQ(leaves.size(), 3);
    const auto grouping = decomposeTree.groupLeaves();
    ASSERT_EQ(grouping().size(), 3);
    const auto *leaf1 = Single(grouping().at(0));
    const auto *leaf2 = Single(grouping().at(1));
    const auto *leaf3 = Single(grouping().at(2));
    ASSERT_EQ(decomposeTree.collectDecomposeTagsForLeaf(leaf1), (std::vector<Tag>{{3, 1, 1}, {1, 1, 5}}));
    ASSERT_EQ(decomposeTree.collectDecomposeTagsForLeaf(leaf2), (std::vector<Tag>{{3, 1, 2}, {1, 2, 3}, {6}, {1, 1, 5}}));
    ASSERT_EQ(decomposeTree.collectDecomposeTagsForLeaf(leaf3), (std::vector<Tag>{{3, 1, 3}, {1, 2, 3}, {6}, {1, 1, 5}}));
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, leaf1), 0);
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, leaf2), 4);
    ASSERT_EQ(decomposeTree.getVertexInRoot(1, leaf2), 5);
    ASSERT_EQ(decomposeTree.getVertexInRoot(0, leaf3), 1);
    ASSERT_EQ(decomposeTree.getVertexInRoot(1, leaf3), 2);
    ASSERT_EQ(decomposeTree.getVertexInRoot(2, leaf3), 3);
    CheckVertexConservation(decomposeTree, 3);
}

TEST(GraphIsomorphismDecomposeTreeTest, SpecialCase6)
{
    const auto g0 = UndirectedGraphFromG6::Create("HR]~Mv~");
    const auto g1 = UndirectedGraphFromG6::Create("HRl~Mv~");
    const TaggedGraph tg0(*g0);
    const TaggedGraph tg1(*g1);
    const auto tgCompare = tg0 <=> tg1;
    ASSERT_TRUE(tgCompare == 0); // TODO :

    const auto decompose0 = IDecompose::Create(*g0);
    const auto decompose1 = IDecompose::Create(*g1);
    const DecomposeTree map0(decompose0.get());
    const DecomposeTree map1(decompose1.get());

    auto tmp0 = map0.getDescriptions();
    auto tmp1 = map1.getDescriptions();
    for (const auto *leaf : map0.getLeaves())
    {
        if (leaf->getGraph().getNumVertices() > 2)
        {
            const auto grouping = leaf->getTaggedGraph().getVertexGrouping();
            // GraphIsomorphism::toEdgeList(leaf->getGraph(), std::cout);
        }
    }
    // std::cout << "\n\n\n";

    for (const auto *leaf : map1.getLeaves())
    {
        if (leaf->getGraph().getNumVertices() > 2)
        {
            const auto grouping = leaf->getTaggedGraph().getVertexGrouping();
            // GraphIsomorphism::toEdgeList(leaf->getGraph(), std::cout);
        }
    }

    const auto cmp = map0 <=> map1;
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

TEST(GraphIsomorphismDecomposeTreeTest, BullIsSelfComplement)
{
    const auto graph = UndirectedGraphLibrary::Get_Bull();
    const auto decompose = IDecompose::Create(*graph);
    ASSERT_FALSE(decompose->isLeaf());
}

TEST(GraphIsomorphismDecomposeTreeTest, OmitEdgesDiamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Bull();
    const auto decomposed = DecomposeOmitEdges::tryCreate(*graph);
    const DecomposeTree decomposeTree(decomposed.get());
    const auto descr = decomposeTree.getDescriptions();
    ASSERT_EQ(decomposeTree.size(), 2);
    // Leaf: path of length 5
    const auto *leaf = Single(decomposeTree.getLeaves());
    const auto tag = leaf->getTag();
    ASSERT_EQ(tag, (std::vector<TagEntry>{3, 3, 5}));
}

TEST(GraphIsomorphismDecomposeTreeTest, OmitEdgesH)
{
    const auto graph = UndirectedGraphFromG6::Create("EgSG");
    const auto decomposed = DecomposeOmitEdges::tryCreate(*graph);
    const DecomposeTree decomposeTree(decomposed.get());
    const auto descr = decomposeTree.getDescriptions();
    ASSERT_EQ(decomposeTree.size(), 4);
    // Leaves: 2 paths of length 3
    const auto leaves = decomposeTree.getLeaves();
    ASSERT_EQ(leaves.size(), 2);
    ASSERT_EQ(leaves[0]->getTag(), (std::vector<TagEntry>{3, 3, 3}));
    ASSERT_EQ(leaves[1]->getTag(), (std::vector<TagEntry>{3, 3, 3}));
    const auto leafRoot = decomposeTree.getRoot()->getTag();
    ASSERT_EQ(leafRoot, (std::vector<TagEntry>{4, 5, 1}));
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
    // PrintMultipleDecompositions(UndirectedGraphFromG6::getListNumVertices_7());
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList8)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_8(), {1, 746});
}

TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList9)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_9(), {1, 479, 2, 1});
    // PrintMultipleDecompositions(UndirectedGraphFromG6::getListNumVertices_9());
}
TEST(GraphIsomorphismDecomposeTreeTest, CheckDecomposeList10)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_10(), {1, 706, 3, 1, 6, 1});
}
#endif
