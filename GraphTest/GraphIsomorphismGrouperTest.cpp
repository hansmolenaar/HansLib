#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismVertexGrouper.h"
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

TEST(GraphIsomorphismVertexGrouperTest, DegreePath3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);

    const auto taggerDegree = TaggerDegree(*graph);
    const VertexGrouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(grouper.countUnique(), 1);
    ASSERT_FALSE(grouper.isResolved());

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{0, 2}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(1)), std::vector<GraphVertex>{1}));

    const Tag dummy{42};
    ASSERT_TRUE(grouper.getGroupMembers(dummy).empty());
}

TEST(GraphIsomorphismVertexGrouperTest, DegreeCycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);

    const auto taggerDegree = TaggerDegree(*graph);
    const VertexGrouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), 1);

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{0, 1, 2}));
}

TEST(GraphIsomorphismVertexGrouperTest, Star112)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 1, 2});

    const auto taggerDegree = TaggerDegree(*graph);
    const VertexGrouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), 3);

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{1, 2, 4}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(1)), std::vector<GraphVertex>{3}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(2)), std::vector<GraphVertex>{0}));
}

TEST(GraphIsomorphismVertexGrouperTest, DegreeStar321)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({3, 2, 1});

    const auto taggerDegree = TaggerDegree(*graph);
    const VertexGrouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), 3);

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{3, 5, 6}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(1)), std::vector<GraphVertex>{1, 2, 4}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(2)), std::vector<GraphVertex>{0}));
    ASSERT_FALSE(grouper.isResolved());
}

TEST(GraphIsomorphismVertexGrouperTest, DistanceStar321)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({3, 2, 1});
    const GraphUsc uscGraph(*graph);
    const TaggerDistance tagger(uscGraph);
    const VertexGrouper grouper(tagger);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), graph->getNumVertices());
    ASSERT_TRUE(grouper.isResolved());
}

TEST(GraphIsomorphismVertexGrouperTest, Path4)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(4);
    const auto taggerDegree = TaggerDegree(*graph);
    const VertexGrouper grouper(taggerDegree);

    const auto &tags = grouper.getTags();
    ASSERT_EQ(tags.size(), 2);

    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(0)), std::vector<GraphVertex>{0, 3}));
    ASSERT_TRUE(str::equal(grouper.getGroupMembers(tags.at(1)), std::vector<GraphVertex>{1, 2}));

    const auto graphPermuted = GraphUsc::CreatePermuted(*graph, {1, 0, 2, 3});
    const auto taggerDegreePermuted = TaggerDegree(graphPermuted);
    const VertexGrouper grouperPermuted(taggerDegreePermuted);

    ASSERT_EQ(grouper, grouperPermuted);
}

TEST(GraphIsomorphismVertexGrouperTest, Path4and5)
{
    const auto graph4 = UndirectedGraphLibrary::Get_Path(4);
    const auto taggerDegree4 = TaggerDegree(*graph4);
    const VertexGrouper grouper4(taggerDegree4);

    const auto graph5 = UndirectedGraphLibrary::Get_Path(5);
    const auto taggerDegree5 = TaggerDegree(*graph5);
    const VertexGrouper grouper5(taggerDegree5);

    ASSERT_NE(grouper4, grouper5);
}
