#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismTagCompare.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "Permutation.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

// TODO
#if false
TEST(GraphIsomorphismTagCompareTest, DegreePath2and3)
{
    const auto graphs = std::make_pair(UndirectedGraphLibrary::Get_Path(2), UndirectedGraphLibrary::Get_Path(3));
    const auto taggers = std::make_pair(TaggerDegree(*graphs.first), TaggerDegree(*graphs.second));
    const auto retval = TagCompare{}(taggers);
    ASSERT_EQ(retval.TagCompareStatus, TagCompare::Result::TagStatus::NotEquivalent);
}

TEST(GraphIsomorphismTagCompareTest, DegreePath3and4)
{
    const auto graphs = std::make_pair(UndirectedGraphLibrary::Get_Path(3), UndirectedGraphLibrary::Get_Path(4));
    const auto taggers = std::make_pair(TaggerDegree(*graphs.first), TaggerDegree(*graphs.second));
    const auto retval = TagCompare{}(taggers);
    ASSERT_EQ(retval.TagCompareStatus, TagCompare::Result::TagStatus::NotEquivalent);
}

TEST(GraphIsomorphismTagCompareTest, DegreePathAndStar4)
{
    const auto graphs =
        std::make_pair(UndirectedGraphLibrary::Get_Path(4), UndirectedGraphLibrary::Get_Star({1, 1, 1}));
    const auto taggers = std::make_pair(TaggerDegree(*graphs.first), TaggerDegree(*graphs.second));
    const auto retval = TagCompare{}(taggers);
    ASSERT_EQ(retval.TagCompareStatus, TagCompare::Result::TagStatus::NotEquivalent);
}

TEST(GraphIsomorphismTagCompareTest, DegreePath3)
{
    const auto graphs = std::make_pair(UndirectedGraphLibrary::Get_Path(3), UndirectedGraphLibrary::Get_Path(3));
    const auto taggers = std::make_pair(TaggerDegree(*graphs.first), TaggerDegree(*graphs.second));
    const auto retval = TagCompare{}(taggers);
    ASSERT_EQ(retval.TagCompareStatus, TagCompare::Result::TagStatus::Equivalent);
    ASSERT_EQ(retval.VertexPairs.size(), 1);
    ASSERT_EQ(retval.VertexPairs.front(), (VertexPair{1, 1}));
}

TEST(GraphIsomorphismTagCompareTest, DegreeStar123)
{
    const auto graphs =
        std::make_pair(UndirectedGraphLibrary::Get_Star({1, 2, 3}), UndirectedGraphLibrary::Get_Star({3, 1, 2}));
    const auto taggers = std::make_pair(TaggerDegree(*graphs.first), TaggerDegree(*graphs.second));
    const auto retval = TagCompare{}(taggers);
    ASSERT_EQ(retval.TagCompareStatus, TagCompare::Result::TagStatus::Equivalent);
    ASSERT_EQ(retval.VertexPairs.size(), 1);
    ASSERT_EQ(retval.VertexPairs.front(), (VertexPair{0, 0}));
}

TEST(GraphIsomorphismTagCompareTest, DegreePan3)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const auto permuted =
        UndirectedGraph::CreatePermuted(*graph, Permutation::Create(std::vector<Permutation::Entry>{2, 1, 0, 3}));
    const auto graphs = std::make_pair(*graph, permuted);
    const auto taggers = std::make_pair(TaggerDegree(graphs.first), TaggerDegree(graphs.second));
    const auto retval = TagCompare{}(taggers);
    ASSERT_EQ(retval.TagCompareStatus, TagCompare::Result::TagStatus::Equivalent);
    ASSERT_EQ(retval.VertexPairs.size(), 2);
    ASSERT_EQ(retval.VertexPairs.back(), (VertexPair{2, 0}));
    ASSERT_EQ(retval.VertexPairs.front(), (VertexPair{3, 3}));
}
#endif
