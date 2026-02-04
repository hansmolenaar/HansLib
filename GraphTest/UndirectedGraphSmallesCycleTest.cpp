#include <gtest/gtest.h>

#include "GraphIsomorphismUtils.h"
#include "Iota.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UndirectedGraphSmallestCycle.h"

using namespace Graph;
using namespace GraphIsomorphism;

TEST(UndirectedGraphSmallestCycleTest, Disconnected)
{
    for (Vertex n : Iota::GetRange(5))
    {
        const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(n);
        const UndirectedGraphSmallestCycle smallesCycles(*graph);
        const std::vector<Vertex> expect(n, UndirectedGraphSmallestCycle::noCycleLength);
        ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), expect);
    }
}

TEST(UndirectedGraphSmallestCycleTest, Cycle)
{
    for (Vertex n : Iota::GetRangeFromTo(3, 10))
    {
        const auto graph = UndirectedGraphLibrary::Get_Cycle(n);
        const UndirectedGraphSmallestCycle smallesCycles(*graph);
        const std::vector<Vertex> expect(n, n);
        ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), expect);
    }
}

TEST(UndirectedGraphSmallestCycleTest, Path)
{
    for (Vertex n : Iota::GetRangeFromTo(2, 10))
    {
        const auto graph = UndirectedGraphLibrary::Get_Path(n);
        const UndirectedGraphSmallestCycle smallesCycles(*graph);
        const std::vector<Vertex> expect(n, UndirectedGraphSmallestCycle::noCycleLength);
        ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), expect);
    }
}

TEST(UndirectedGraphSmallestCycleTest, Diamond)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::diamond);
    const UndirectedGraphSmallestCycle smallesCycles(*graph);
    ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), (std::vector<Vertex>{3, 3, 3, 3}));
}

TEST(UndirectedGraphSmallestCycleTest, X84)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::X84);
    const UndirectedGraphSmallestCycle smallesCycles(*graph);
    ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), (std::vector<Vertex>{4, 4, 4, 4, 0, 0}));

    auto uGraph = UndirectedGraph::CreateDisjointedUnion(*graph, *UndirectedGraphLibrary::Get_Path(11));
    uGraph.addEdge(4, 6);
    uGraph.addEdge(5, 16);
    const UndirectedGraphSmallestCycle smallesCycles2(uGraph);
    const std::vector<Vertex> expect{4, 4, 4, 4, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
    ASSERT_EQ(smallesCycles2.getSmallestCycleLengths(), expect);
}

TEST(UndirectedGraphSmallestCycleTest, Pan3)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const UndirectedGraphSmallestCycle smallesCycles(*graph);
    ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), (std::vector<Vertex>{3, 3, 3, 0}));
}

TEST(UndirectedGraphSmallestCycleTest, Claw)
{
    const auto graph = UndirectedGraphLibrary::Get_Claw();
    const UndirectedGraphSmallestCycle smallesCycles(*graph);
    ASSERT_EQ(smallesCycles.getSmallestCycleLengths(), (std::vector<Vertex>{0, 0, 0, 0}));
}

TEST(UndirectedGraphSmallestCycleTest, Cycles)
{
    const auto cycle3 = UndirectedGraphLibrary::Get_Cycle(3);
    const auto cycle20 = UndirectedGraphLibrary::Get_Cycle(20);
    auto graph = UndirectedGraph::CreateDisjointedUnion(*cycle3, *cycle20);

    auto sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{3, 3, 20, 20}));

    graph.addEdge(0, 3);

    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{3, 3, 20, 20}));

    graph.addEdge(1, 4);

    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{3, 3, 4, 2, 20, 18}));
}

TEST(UndirectedGraphSmallestCycleTest, Stuff)
{
    auto graph = UndirectedGraphFromG6::Create("Ks?GOObDRCI_");
    auto sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{4, 12}));

    graph = UndirectedGraphFromG6::Create("Ks?GOgQPPDL?");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{3, 6, 4, 4, 5, 2}));

    graph = UndirectedGraphFromG6::Create("G?`@pk");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{0, 4, 3, 4}));

    graph = UndirectedGraphFromG6::Create("G??gv_");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{0, 2, 6, 6}));

    graph = UndirectedGraphFromG6::Create("G`?KzW");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{3, 4, 5, 4}));

    graph = UndirectedGraphFromG6::Create("G?DPTC");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{0, 5, 3, 3}));

    graph = UndirectedGraphFromG6::Create("G?DPTC");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{0, 5, 3, 3}));

    graph = UndirectedGraphFromG6::Create("I`?KQKSIG");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{4, 10}));

    graph = UndirectedGraphFromG6::Create("JGECA?_?ww?");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{0, 3, 3, 3, 5, 5}));

    graph = UndirectedGraphFromG6::Create("L??????BP`GW_g");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{0, 13}));

    graph = UndirectedGraphFromG6::Create("L?AAHC@?G@_BE@");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{0, 9, 3, 4}));

    graph = UndirectedGraphFromG6::Create("LQQ@?GGC???R?Y");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{8, 13}));

    graph = UndirectedGraphFromG6::Create("FI_gw");
    sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, (Tag{0, 1, 3, 3, 5, 3}));
}

TEST(UndirectedGraphSmallestCycleTest, Permutations)
{
    auto graph = UndirectedGraphFromG6::Create("Dr[");
    const UndirectedGraphSmallestCycle smallesCycles(*graph);
    const std::vector<TagEntry> expectSizes{3, 4, 4, 1};
    const auto sizes = CondenseSizeSequence(UndirectedGraphSmallestCycle(*graph).getSmallestCycleLengths());
    ASSERT_EQ(sizes, expectSizes);
    for (Permutation::Entry seed : Iota::GetRange(10))
    {
        const auto pgraph = UndirectedGraph::CreateRandomShuffled(*graph, seed);
        const UndirectedGraphSmallestCycle psmallesCycles(pgraph);
        const auto psizes = CondenseSizeSequence(psmallesCycles.getSmallestCycleLengths());
        ASSERT_EQ(psizes, expectSizes);
    }
}
