#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphDefines.h"
#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"
#include "IGraphIsomorphismTagger.h"
#include "Single.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
} // namespace

TEST(GraphIsomorphismGrouping, SomeInts)
{
    const std::vector<int> values{3, 2, 3, 2, 3, 1};
    const Grouping<int> grouping(values, std::less<int>{});
    ASSERT_EQ(grouping.size(), 6);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 2, 3}));
    ASSERT_EQ(grouping().at(0), (std::vector<int>{1}));
    ASSERT_EQ(grouping().at(1), (std::vector<int>{2, 2}));
    ASSERT_EQ(grouping().at(2), (std::vector<int>{3, 3, 3}));
}

TEST(GraphIsomorphismGrouping, CustomCompare)
{
    const std::vector<int> values{1, 2, 1};
    const std::vector<const int *> pointers{&values[0], &values[1], &values[2]};
    auto cmp = [](const int *p1, const int *p2) { return *p1 < *p2; };
    const Grouping<const int *> grouping(pointers, cmp);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{2, 1}));
}

TEST(GraphIsomorphismGrouping, Combine_1)
{
    const std::vector<Vertex> vertices{0, 1, 2, 3, 4};
    const VertexTags vtag0{{1}, {2}, {1}, {2}, {2}};
    const VertexTags vtag1{{1}, {1}, {2}, {1}, {1}};
    auto cmp0 = [&vtag0](Vertex v0, Vertex v1) { return vtag0.at(v0) < vtag0.at(v1); };
    const VertexGrouping grouping0(vertices, cmp0);
    auto cmp1 = [&vtag1](Vertex v0, Vertex v1) { return vtag1.at(v0) < vtag1.at(v1); };
    const VertexGrouping grouping1(vertices, cmp1);

    const auto grouping = VertexGrouping::Combine(grouping0, grouping1);

    ASSERT_EQ(grouping.size(), 5);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 1, 3}));
    ASSERT_EQ(grouping().at(2), (std::vector<Graph::Vertex>{1, 3, 4}));
}

TEST(GraphIsomorphismGrouping, Combine_2)
{
    const std::vector<Vertex> vertices{0, 1, 2, 3};
    const std::vector<Vertex> vtag0{2, 2, 3, 3};
    const std::vector<Vertex> vtag1{2, 3, 2, 3};
    auto cmp0 = [&vtag0](Vertex v0, Vertex v1) { return vtag0.at(v0) < vtag0.at(v1); };
    const VertexGrouping grouping0(vertices, cmp0);
    auto cmp1 = [&vtag1](Vertex v0, Vertex v1) { return vtag1.at(v0) < vtag1.at(v1); };
    const VertexGrouping grouping1(vertices, cmp1);

    const auto grouping = VertexGrouping::Combine(grouping0, grouping1);

    ASSERT_EQ(grouping.size(), 4);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 1, 1, 1}));
    std::set<Vertex> grouped;
    for (const auto &g : grouping())
    {
        grouped.insert(Single(g));
    }
    ASSERT_EQ(grouped.size(), 4);
    ASSERT_EQ(*grouped.begin(), 0);
    ASSERT_EQ(*grouped.rbegin(), 3);
}

TEST(GraphIsomorphismGrouping, Combine_3)
{
    const std::vector<Vertex> vertices{0, 1, 2, 3};
    const std::vector<Vertex> vtag0{2, 2, 2, 3};
    const std::vector<Vertex> vtag1{2, 3, 2, 3};
    auto cmp0 = [&vtag0](Vertex v0, Vertex v1) { return vtag0.at(v0) < vtag0.at(v1); };
    const VertexGrouping grouping0(vertices, cmp0);
    auto cmp1 = [&vtag1](Vertex v0, Vertex v1) { return vtag1.at(v0) < vtag1.at(v1); };
    const VertexGrouping grouping1(vertices, cmp1);

    const auto grouping = VertexGrouping::Combine(grouping0, grouping1);

    ASSERT_EQ(grouping.size(), 4);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{2, 1, 1}));
    std::set<Vertex> grouped;
    for (const auto &g : grouping())
    {
        for (auto v : g)
        {
            grouped.insert(v);
        }
    }
    ASSERT_EQ(grouped.size(), 4);
    ASSERT_EQ(*grouped.begin(), 0);
    ASSERT_EQ(*grouped.rbegin(), 3);
}

TEST(GraphIsomorphismGrouping, Combine_4)
{
    const std::vector<Vertex> vertices{0, 1, 2, 3, 4, 5};
    const VertexTags vtag0{{0}, {1}, {0}, {3}, {4}, {4}};
    const VertexTags vtag1{{1}, {0}, {5}, {3}, {4}, {2}};
    auto cmp0 = [&vtag0](Vertex v0, Vertex v1) { return vtag0.at(v0) < vtag0.at(v1); };
    const VertexGrouping grouping0(vertices, cmp0);
    auto cmp1 = [&vtag1](Vertex v0, Vertex v1) { return vtag1.at(v0) < vtag1.at(v1); };
    const VertexGrouping grouping1(vertices, cmp1);

    const auto grouping = VertexGrouping::Combine(grouping0, grouping1);

    ASSERT_EQ(grouping.size(), 6);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 1, 1, 1, 1, 1}));
}
