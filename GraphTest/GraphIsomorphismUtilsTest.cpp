#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismUtils.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;

TEST(GraphIsomorphismUtilsTest, CondenseSizeSequence_int)
{
    const std::vector<int> sizes{2, 5, 2};
    const auto tag = CondenseSizeSequence(sizes);
    ASSERT_TRUE(str::equal(tag, (Tag{2, 2, 5, 1})));
}

TEST(GraphIsomorphismUtilsTest, CondenseSizeSequence_size_t)
{
    const std::vector<size_t> sizes{2, 5, 2};
    const auto tag = CondenseSizeSequence(sizes);
    ASSERT_TRUE(str::equal(tag, (Tag{2, 2, 5, 1})));
}

#if false
TEST(GraphIsomorphismUtilsTest, ToAdjacencyList)
{
    const auto graph1 = UndirectedGraphFromG6::Create("G}nax{");
    const auto graph2 = UndirectedGraphFromG6::Create("G~rHx{");
    const auto nEdges1 = graph1->getNumEdges();
    const auto nEdges2 = graph2->getNumEdges();
    const auto complement1 = UndirectedGraph::CreateComplement(*graph1);
    const auto complement2 = UndirectedGraph::CreateComplement(*graph2);
    toEdgeList(complement1, std::cout);
    std::cout << "\n\n";
    toEdgeList(complement2, std::cout);
}
#endif
