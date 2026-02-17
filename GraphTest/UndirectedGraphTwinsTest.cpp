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
    for (Vertex size = 2; size < 10; ++size)
    {
        const auto graph = UndirectedGraphLibrary::Get_CompleteGraph(size);
        const UndirectedGraphTwins twins(*graph);
        ASSERT_TRUE(twins.getFalseTwins().empty());
        ASSERT_EQ(Single(twins.getTrueTwins()), Iota::GenerateVector(size));
    }
}

TEST(UndirectedGraphTwinTest, Claw)
{
    const auto graph = UndirectedGraphLibrary::Get_Claw();
    const UndirectedGraphTwins twins(*graph);
    ASSERT_TRUE(twins.getTrueTwins().empty());
    ASSERT_EQ(Single(twins.getFalseTwins()).size(), 3);
}

TEST(UndirectedGraphTwinTest, Butterfly)
{
    const auto graph = UndirectedGraphLibrary::Get_Butterfly();
    const UndirectedGraphTwins twins(*graph);
    ASSERT_TRUE(twins.getFalseTwins().empty());
    ASSERT_EQ(twins.getTrueTwins().size(), 2);
    ASSERT_EQ(twins.getTrueTwins().at(0), (std::vector<Vertex>{1, 2}));
    ASSERT_EQ(twins.getTrueTwins().at(1), (std::vector<Vertex>{3, 4}));
}

TEST(UndirectedGraphTwinTest, Fish)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::fish);
    const UndirectedGraphTwins twins(*graph);
    ASSERT_EQ(Single(twins.getFalseTwins()), (std::vector<Vertex>{1, 2}));
    ASSERT_EQ(Single(twins.getTrueTwins()), (std::vector<Vertex>{4, 5}));
}

TEST(UndirectedGraphTwinTest, Cycle4)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(4);
    const UndirectedGraphTwins twins(*graph);
    ASSERT_TRUE(twins.getTrueTwins().empty());
    ASSERT_EQ(twins.getFalseTwins().size(), 2);
    ASSERT_EQ(twins.getFalseTwins().at(0), (std::vector<Vertex>{1, 3}));
    ASSERT_EQ(twins.getFalseTwins().at(1), (std::vector<Vertex>{0, 2}));
}
