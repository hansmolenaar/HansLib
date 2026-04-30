#include <gtest/gtest.h>

#include "Defines.h"
#include "Single.h"
#include "UndirectedGraphDistance.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace Utilities;

TEST(UndirectedGraphDistanceTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const UndirectedGraphDistance distances(*graph);
    auto d = distances(static_cast<Vertex>(0));

    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{0}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{1}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<Vertex>{2}));

    d = distances(static_cast<Vertex>(1));
    ASSERT_EQ(d.size(), 2);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{1}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{0, 2}));

    d = distances(static_cast<Vertex>(2));
    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{2}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{1}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<Vertex>{0}));
}

TEST(UndirectedGraphDistanceTest, Cycle4)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(4);
    const UndirectedGraphDistance distances(*graph);

    const auto &d = distances(static_cast<Vertex>(1));
    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{1}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{0, 2}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<Vertex>{3}));
}

TEST(UndirectedGraphDistanceTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const UndirectedGraphDistance distances(*graph);

    auto d = distances(static_cast<Vertex>(0));
    ASSERT_EQ(d.size(), 2);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{0}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{1, 2}));

    d = distances(static_cast<Vertex>(1));
    ASSERT_EQ(d.size(), 2);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{1}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{0, 2}));

    d = distances(static_cast<Vertex>(2));
    ASSERT_EQ(d.size(), 2);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{2}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{0, 1}));
}

TEST(UndirectedGraphDistanceTest, Star111)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 1, 1});
    const UndirectedGraphDistance distances(*graph);

    auto d = distances(static_cast<Vertex>(0));
    ASSERT_EQ(d.size(), 2);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{0}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{1, 2, 3}));

    d = distances(static_cast<Vertex>(1));
    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{1}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{0}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<Vertex>{2, 3}));

    d = distances(static_cast<Vertex>(2));
    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{2}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{0}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<Vertex>{1, 3}));

    d = distances(static_cast<Vertex>(3));
    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<Vertex>{3}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<Vertex>{0}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<Vertex>{1, 2}));
}

TEST(UndirectedGraphDistanceTest, Disconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const UndirectedGraphDistance distances(*graph);

    auto d = distances(static_cast<Vertex>(0));
    ASSERT_EQ(Single(d), std::vector<Vertex>{0});

    d = distances(static_cast<Vertex>(1));
    ASSERT_EQ(Single(d), std::vector<Vertex>{1});
}
