#include <gtest/gtest.h>

#include "GraphIsomorphismCheck.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismCheckTest, Check)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 2, 3});

    {
        const auto other = UndirectedGraphLibrary::Get_Path(3);
        ASSERT_EQ(Check{}(*graph, *other), Status::NotIsomorphic);
    }

    {
        const auto other = UndirectedGraphLibrary::Get_Cycle(7);
        ASSERT_EQ(Check{}(*graph, *other), Status::NotIsomorphic);
    }

    {
        const auto other = UndirectedGraphLibrary::Get_Path(7);
        ASSERT_EQ(Check{}(*graph, *other), Status::NotIsomorphic);
    }

    {
        const auto permutation = Permutation::Create(std::vector<Permutation::Entry>{1, 2, 3, 4, 5, 6, 0});
        const auto permuted = UndirectedGraph::CreatePermuted(*graph, permutation);
        ASSERT_EQ(Check{}(*graph, permuted), Status::Undecided);
    }

    {
        ASSERT_EQ(Check{}(*graph, *graph), Status::Isomorphic);
    }
}
