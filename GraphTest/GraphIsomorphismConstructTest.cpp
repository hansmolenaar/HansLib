#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismConstruct.h"
#include "Permutation.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;

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

TEST(GraphIsomorphismConstructTest, DegreePan3)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::pan3);
    const auto permuted =
        GraphUsc::CreatePermuted(*graph, Permutation::Create(std::vector<Permutation::Entry>{2, 1, 0, 3}));
    const auto status = Construct{}.actionConnected(*graph, permuted);
    ASSERT_EQ(status.getFlag(), Flag::Isomorphic);
}
