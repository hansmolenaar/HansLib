#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismIGraphCompareTest.h"
#include "GraphIsomorphismXGraph.h"
#include "UndirectedGraphFromG6.h"

using namespace GraphIsomorphism;
using namespace Graph;

TEST(GraphIsomorphismXGraphTest, Basics)
{
    XGraphFactory factory;
    GraphTest::CheckComparerBasics(factory);
}

TEST(GraphIsomorphismXGraphTest, Combine)
{
    const std::vector<Vertex> vertices{0, 1, 2, 3};
    const std::vector<Tag> vtag0{{1}, {2}, {1}, {2}};
    const std::vector<Tag> vtag1{{1}, {1}, {2}, {1}};
    auto cmp0 = [&vtag0](Vertex v0, Vertex v1) { return vtag0.at(v0) < vtag0.at(v1); };
    const VertexGrouping grouping0(vertices, cmp0);
    auto cmp1 = [&vtag1](Vertex v0, Vertex v1) { return vtag1.at(v0) < vtag1.at(v1); };
    const VertexGrouping grouping1(vertices, cmp1);

    const auto grouping = VertexGrouping::Combine(grouping0, grouping1);

    ASSERT_EQ(grouping.size(), 4);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 1, 2}));
    ASSERT_EQ(grouping().at(2), (std::vector<Graph::Vertex>{1, 3}));
}

TEST(GraphIsomorphismXGraphTest, Test6)
{
    const auto graph = UndirectedGraphFromG6::Create("EQzg");
    const Comparers tg(*graph);
    const XGraph xg(*graph);
    const auto grouping_tg = tg.getVertexGrouping();
    ASSERT_EQ(grouping_tg.countUnique(), 2);
    const auto grouping_xg = xg.getVertexGrouping();
    ASSERT_EQ(grouping_xg.countUnique(), 6);
}

TEST(GraphIsomorphismXGraphTest, CheckList3)
{
    XGraphFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_3(), Tag{1, 4});
}
