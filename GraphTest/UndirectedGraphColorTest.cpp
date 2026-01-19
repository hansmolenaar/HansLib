#include <gtest/gtest.h>

#include "UndirectedGraphColor.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;

namespace
{
void TestColoringSingle(const IGraphUs &graph, const std::optional<UndirectedGraphColor::Color> &expectChromaticNumber)
{
    const UndirectedGraphColor coloring(graph);
    if (!expectChromaticNumber.has_value())
    {
        ASSERT_FALSE(coloring.getChromaticNumber().has_value());
        return;
    }

    const auto chromaticNumber = coloring.getChromaticNumber();
    ASSERT_TRUE(chromaticNumber.has_value());
    ASSERT_EQ(chromaticNumber, expectChromaticNumber);
    if (chromaticNumber == 0)
    {
        ASSERT_EQ(graph.getNumVertices(), 0);
        ASSERT_ANY_THROW(coloring.getColoring2());
    }
    else if (chromaticNumber == 1)
    {
        ASSERT_GT(graph.getNumVertices(), 0);
        ASSERT_EQ(graph.getNumEdges(), 0);
        ASSERT_ANY_THROW(coloring.getColoring2());
    }
    else if (chromaticNumber == 2)
    {
        ASSERT_GT(graph.getNumVertices(), 1);
        ASSERT_GT(graph.getNumEdges(), 0);
        const auto coloring2 = coloring.getColoring2();
        std::vector<Vertex> ngbs;
        for (Vertex v = 0; v < graph.getNumVertices(); ++v)
        {
            graph.setAdjacentVertices(v, ngbs);
            for (auto ngb : ngbs)
            {
                ASSERT_NE(coloring2.at(v), coloring2.at(ngb));
            }
        }
    }
    else
    {
        // Some special cases
        ASSERT_ANY_THROW(coloring.getColoring2());
    }
}

void TestColoring(const IGraphUs &graph, const std::optional<UndirectedGraphColor::Color> &expectChromaticNumber)
{
    TestColoringSingle(graph, expectChromaticNumber);
    for (Permutation::Entry seed = 0; seed < 5; ++seed)
    {
        TestColoringSingle(UndirectedGraph::CreateRandomShuffled(graph, seed), expectChromaticNumber);
    }
}

} // namespace

TEST(UndirectedGraphColorTest, Empty)
{
    const auto graph = UndirectedGraphLibrary::Get_Null();
    TestColoring(*graph, 0);
}

TEST(UndirectedGraphColorTest, Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    TestColoring(*graph, 1);
}

TEST(UndirectedGraphColorTest, Disconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    TestColoring(*graph, 1);
}

TEST(UndirectedGraphColorTest, Edge)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    TestColoring(*graph, 2);
    const UndirectedGraphColor coloring(*graph);
    ASSERT_EQ(coloring.getChromaticNumber(), 2);
    const auto color2 = coloring.getColoring2();
    ASSERT_EQ(color2.size(), 2);
    ASSERT_EQ(color2.at(0), 0);
    ASSERT_EQ(color2.at(1), 1);
}

TEST(UndirectedGraphColorTest, Cycles)
{
    for (int size = 3; size < 10; ++size)
    {
        const auto graph = UndirectedGraphLibrary::Get_Cycle(size);
        std::optional<UndirectedGraphColor::Color> expect;
        if (size % 2 == 0)
        {
            expect = 2;
        }
        else if (size == 3)
        {
            expect = 3;
        }
        TestColoring(*graph, expect);
    }
}

TEST(UndirectedGraphColorTest, InterestingCases)
{
    TestColoring(*UndirectedGraphFromG6::Create("H???XjA"), 2);
    TestColoring(*UndirectedGraphFromG6::Create("H????F|"), 2);
    TestColoring(*UndirectedGraphFromG6::Create("Is?AXW[[?"), 2);
    TestColoring(*UndirectedGraphFromG6::Create("Is?HGtcU?"), 3);
    TestColoring(*UndirectedGraphFromG6::Create("Is?@WxcU?"), 2);
    TestColoring(*UndirectedGraphFromG6::Create("I?AAHGICW"), 3);
    TestColoring(*UndirectedGraphFromG6::Create("I@`??CBNG"), {}); // 5
    TestColoring(*UndirectedGraphFromG6::Create("I?COPHAw?"), 3);
}

TEST(UndirectedGraphColorTest, Complete)
{
    for (int size = 2; size < 10; ++size)
    {
        const auto graph = UndirectedGraphLibrary::Get_CompleteGraph(size);
        TestColoring(*graph, size);
    }
}

TEST(UndirectedGraphColorTest, BiPartite)
{
    constexpr Vertex maxSize = 5;
    for (Vertex v1 = 1; v1 < maxSize; ++v1)
    {
        for (Vertex v2 = v1; v2 < maxSize; ++v2)
        {
            const auto graph = UndirectedGraphLibrary::Get_CompleteBipartite(v1, v2);
            TestColoring(*graph, 2);
        }
    }
}
