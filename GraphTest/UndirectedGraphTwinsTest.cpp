#include <gtest/gtest.h>

#include "Iota.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UndirectedGraphTwins.h"

using namespace Graph;
using namespace Utilities;

namespace
{

} // namespace

TEST(UndirectedGraphTwinTest, Empty)
{
    const auto graph = UndirectedGraphLibrary::Get_Null();
    const UndirectedGraphTwins twins(*graph);
    ASSERT_TRUE(twins.getTrueTwins().empty());
    ASSERT_TRUE(twins.getFalseTwins().empty());
    ASSERT_EQ(twins.getGraph().getNumVertices(), 0);
}

TEST(UndirectedGraphTwinTest, Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const UndirectedGraphTwins twins(*graph);
    ASSERT_TRUE(twins.getTrueTwins().empty());
    ASSERT_TRUE(twins.getFalseTwins().empty());
}

TEST(UndirectedGraphTwinTest, Disconnected5)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(5);
    const UndirectedGraphTwins twins(*graph);
    ASSERT_TRUE(twins.getTrueTwins().empty());
    ASSERT_TRUE(twins.getFalseTwins().empty());
}

TEST(UndirectedGraphTwinTest, Complete)
{
    for (int size = 2; size < 10; ++size)
    {
        const auto graph = UndirectedGraphLibrary::Get_CompleteGraph(size);
        const UndirectedGraphTwins twins(*graph);
        ASSERT_TRUE(twins.getFalseTwins().empty());
        ASSERT_EQ(Single(twins.getTrueTwins()), Iota::GenerateVector<Vertex>(size, 0));
    }
}

TEST(UndirectedGraphTwinTest, Claw)
{
    const auto graph = UndirectedGraphLibrary::Get_Claw();
    const UndirectedGraphTwins twins(*graph);
    ASSERT_TRUE(twins.getTrueTwins().empty());
    ASSERT_EQ(Single(twins.getFalseTwins()).size(), 3);
}

#if false
TEST(UndirectedGraphTwinTest, Cycles)
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

TEST(UndirectedGraphTwinTest, InterestingCases)
{
    TestColoring(*UndirectedGraphFromG6::Create("H???XjA"), 2);
    TestColoring(*UndirectedGraphFromG6::Create("H????N}"), 2);
    TestColoring(*UndirectedGraphFromG6::Create("H????F|"), 2);
    TestColoring(*UndirectedGraphFromG6::Create("Is?AXW[[?"), 2);
    TestColoring(*UndirectedGraphFromG6::Create("Is?HGtcU?"), 3);
    TestColoring(*UndirectedGraphFromG6::Create("Is?@WxcU?"), 2);
    TestColoring(*UndirectedGraphFromG6::Create("I?AAHGICW"), 3);
    TestColoring(*UndirectedGraphFromG6::Create("I@`??CBNG"), {}); // 5
    TestColoring(*UndirectedGraphFromG6::Create("F?dXw"), {});     // 4
    TestColoring(*UndirectedGraphFromG6::Create("I?COPHAw?"), 3);
}
TEST(UndirectedGraphTwinTest, BiPartite)
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
#endif
