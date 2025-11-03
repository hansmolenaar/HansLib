#include <gtest/gtest.h>

#include "GraphIsomorphismCheck.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismCheckTest, Check1)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 2, 3});

    {
        const auto other = UndirectedGraphLibrary::Get_Path(3);
        ASSERT_FALSE(Check{}(*graph, *other));
    }

    {
        const auto other = UndirectedGraphLibrary::Get_Cycle(7);
        ASSERT_FALSE(Check{}(*graph, *other));
    }

    {
        const auto other = UndirectedGraphLibrary::Get_Path(7);
        ASSERT_FALSE(Check{}(*graph, *other));
    }

    {
        const auto permutation = Permutation::Create(std::vector<Permutation::Entry>{1, 2, 3, 4, 5, 6, 0});
        const auto permuted = UndirectedGraph::CreatePermuted(*graph, permutation);
        ASSERT_FALSE(Check{}(*graph, permuted));
    }

    {
        ASSERT_TRUE(Check{}(*graph, *graph));
    }
}

TEST(GraphIsomorphismCheckTest, Check2)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const auto permutation = Permutation::Create(std::vector<Permutation::Entry>{1, 0, 2});
    const auto permuted = UndirectedGraph::CreatePermuted(*graph, permutation);
    ASSERT_TRUE(Check{}(*graph, permuted));
    const std::vector<VertexPair> perm01{{0, 1}, {1, 0}, {2, 2}};
    ASSERT_TRUE(Check{}(*graph, perm01, permuted));
}

TEST(GraphIsomorphismCheckTest, Check3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(4);
    const auto permutation = Permutation::Create(std::vector<Permutation::Entry>{1, 0, 2, 3});
    const auto permuted = UndirectedGraph::CreatePermuted(*graph, permutation);
    ASSERT_FALSE(Check{}(*graph, permuted));
    const std::vector<VertexPair> perm01{{0, 1}, {1, 0}, {2, 2}, {3, 3}};
    ASSERT_TRUE(Check{}(*graph, perm01, permuted));
}
