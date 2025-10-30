#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouper.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "UndirectedGraphDistance.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismGrouperTest, DegreePath3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);

    const auto taggerDegree = TaggerDegree(*graph);
    const Grouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), 2);

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{0, 2}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(1)), std::vector<GraphVertex>{1}));

    const VertexTag dummy{42};
    ASSERT_TRUE(grouper.getGroupMembers(dummy).empty());
}

TEST(GraphIsomorphismGrouperTest, DegreeCycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);

    const auto taggerDegree = TaggerDegree(*graph);
    const Grouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), 1);

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{0, 1, 2}));
}

TEST(GraphIsomorphismGrouperTest, Star112)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 1, 2});

    const auto taggerDegree = TaggerDegree(*graph);
    const Grouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), 3);

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{1, 2, 4}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(1)), std::vector<GraphVertex>{3}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(2)), std::vector<GraphVertex>{0}));
}

TEST(GraphIsomorphismGrouperTest, DegreeStar321)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({3, 2, 1});

    const auto taggerDegree = TaggerDegree(*graph);
    const Grouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), 3);

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{3, 5, 6}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(1)), std::vector<GraphVertex>{1, 2, 4}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(2)), std::vector<GraphVertex>{0}));
    ASSERT_FALSE(grouper.isResolved());
}

TEST(GraphIsomorphismGrouperTest, DistanceStar321)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({3, 2, 1});
    const auto tagger = TaggerDistanceFactory{}.create(*graph);
    const Grouper grouper(*tagger);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), graph->getNumVertices());
    ASSERT_TRUE(grouper.isResolved());
}

TEST(GraphIsomorphismGrouperTest, Path4)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(4);
    const auto taggerDegree = TaggerDegree(*graph);
    const Grouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), 2);

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{0, 3}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(1)), std::vector<GraphVertex>{1, 2}));
}
