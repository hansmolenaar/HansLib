#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismConstruct.h"
#include "Permutation.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismConstructTest, DegreePath2and3)
{
    const auto graphs = std::make_pair(UndirectedGraphLibrary::Get_Path(2), UndirectedGraphLibrary::Get_Path(3));
    const auto status = Construct{}.actionConnected(*graphs.first, *graphs.second);
    ASSERT_EQ(status.getFlag(), Flag::NotIsomorphic);
}

TEST(GraphIsomorphismConstructTest, DegreePathAndStar4)
{
    const auto graphs =
        std::make_pair(UndirectedGraphLibrary::Get_Path(4), UndirectedGraphLibrary::Get_Star({1, 1, 1}));
    const auto status = Construct{}.actionConnected(*graphs.first, *graphs.second);
    ASSERT_EQ(status.getFlag(), Flag::NotIsomorphic);
}

TEST(GraphIsomorphismConstructTest, DegreePath3)
{
    const auto graphs = std::make_pair(UndirectedGraphLibrary::Get_Path(3), UndirectedGraphLibrary::Get_Path(3));
    const auto status = Construct{}.actionConnected(*graphs.first, *graphs.second);
    ASSERT_EQ(status.getFlag(), Flag::Isomorphic);
}

TEST(GraphIsomorphismConstructTest, DegreeStar123)
{
    const auto graphs =
        std::make_pair(UndirectedGraphLibrary::Get_Star({1, 2, 3}), UndirectedGraphLibrary::Get_Star({3, 1, 2}));
    const auto status = Construct{}.actionConnected(*graphs.first, *graphs.second);
    ASSERT_EQ(status.getFlag(), Flag::Isomorphic);
}

// TODO
#if false

TEST(GraphIsomorphismConstructTest, DegreePan3)
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
