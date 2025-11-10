#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouper.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "GraphUsc.h"
#include "Permutation.h"
#include "Single.h"
#include "UndirectedGraphDistance.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;
using namespace Utilities;

namespace
{
} // namespace

TEST(GraphIsomorphismGrouperTest, DegreePath3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);

    const auto taggerDegree = TaggerDegree(*graph);
    const Grouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(grouper.countUnique(), 1);
    ASSERT_FALSE(grouper.isResolved());

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{0, 2}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(1)), std::vector<GraphVertex>{1}));

    const Tag dummy{42};
    ASSERT_TRUE(grouper.getGroupMembers(dummy).empty());

    std::vector<Tag> groupTags(3);
    grouper.updateVertexGroupTags(groupTags);
    ASSERT_EQ(Single(groupTags.at(0)), 1);
    ASSERT_EQ(Single(groupTags.at(1)), 2);
    ASSERT_EQ(Single(groupTags.at(2)), 1);
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
    const GraphUsc uscGraph(*graph);
    const TaggerDistance tagger(uscGraph);
    const Grouper grouper(tagger);

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

    const auto graphPermuted = GraphUsc::CreatePermuted(*graph, {1, 0, 2, 3});
    const auto taggerDegreePermuted = TaggerDegree(graphPermuted);
    const Grouper grouperPermuted(taggerDegreePermuted);

    ASSERT_TRUE(Grouper::areEquivalent(grouper, grouperPermuted));
}

TEST(GraphIsomorphismGrouperTest, Path4and5)
{
    const auto graph4 = UndirectedGraphLibrary::Get_Path(4);
    const auto taggerDegree4 = TaggerDegree(*graph4);
    const Grouper grouper4(taggerDegree4);

    const auto graph5 = UndirectedGraphLibrary::Get_Path(5);
    const auto taggerDegree5 = TaggerDegree(*graph5);
    const Grouper grouper5(taggerDegree5);

    ASSERT_FALSE(Grouper::areEquivalent(grouper4, grouper5));
}
