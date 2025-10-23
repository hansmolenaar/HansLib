#include <gtest/gtest.h>

#include "Defines.h"
#include "UndirectedGraphDistance.h"
#include "UndirectedGraphLibrary.h"

TEST(UndirectedGraphDistanceTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const UndirectedGraphDistance distances(*graph);
    auto d = distances(static_cast<GraphVertex>(0));

    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{0}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{1}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<GraphVertex>{2}));

    d = distances(static_cast<GraphVertex>(1));
    ASSERT_EQ(d.size(), 2);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{1}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{0, 2}));

    d = distances(static_cast<GraphVertex>(2));
    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{2}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{1}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<GraphVertex>{0}));
}

TEST(UndirectedGraphDistanceTest, Cycle4)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(4);
    const UndirectedGraphDistance distances(*graph);

    const auto d = distances(static_cast<GraphVertex>(1));
    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{1}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{0, 2}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<GraphVertex>{3}));
}

TEST(UndirectedGraphDistanceTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const UndirectedGraphDistance distances(*graph);

    auto d = distances(static_cast<GraphVertex>(0));
    ASSERT_EQ(d.size(), 2);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{0}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{1, 2}));

    d = distances(static_cast<GraphVertex>(1));
    ASSERT_EQ(d.size(), 2);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{1}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{0, 2}));

    d = distances(static_cast<GraphVertex>(2));
    ASSERT_EQ(d.size(), 2);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{2}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{0, 1}));
}

TEST(UndirectedGraphDistanceTest, Star111)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 1, 1});
    const UndirectedGraphDistance distances(*graph);

    auto d = distances(static_cast<GraphVertex>(0));
    ASSERT_EQ(d.size(), 2);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{0}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{1, 2, 3}));

    d = distances(static_cast<GraphVertex>(1));
    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{1}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{0}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<GraphVertex>{2, 3}));

    d = distances(static_cast<GraphVertex>(2));
    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{2}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{0}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<GraphVertex>{1, 3}));

    d = distances(static_cast<GraphVertex>(3));
    ASSERT_EQ(d.size(), 3);
    ASSERT_TRUE(str::equal(d.at(0), std::vector<GraphVertex>{3}));
    ASSERT_TRUE(str::equal(d.at(1), std::vector<GraphVertex>{0}));
    ASSERT_TRUE(str::equal(d.at(2), std::vector<GraphVertex>{1, 2}));

}
