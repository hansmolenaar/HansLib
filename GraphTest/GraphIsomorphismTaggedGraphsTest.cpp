#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismTaggedGraphs.h"

using namespace GraphIsomorphism;
using namespace Graph;

TEST(GraphIsomorphismTaggedGraphsTest, Combine)
{
    const std::vector<Vertex> vertices{0,1,2,3};
    const VertexTags vtag0{{1}, {2}, {1}, {2}};
    const VertexTags vtag1{{1}, {1}, {2}, {1}};
    auto cmp0 = [&vtag0]( Vertex v0, Vertex v1){ return vtag0.at(v0) < vtag0.at(v1);};
    const Grouping<Graph::Vertex> grouping0(vertices, cmp0);
    auto cmp1 = [&vtag1]( Vertex v0, Vertex v1){ return vtag1.at(v0) < vtag1.at(v1);};
    const Grouping<Graph::Vertex> grouping1(vertices, cmp1);
  
    const auto grouping = TaggedGraphs::Combine(grouping0, grouping1);

    ASSERT_EQ(grouping.size(), 4);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 1, 2}));
    ASSERT_EQ(grouping().at(2), (std::vector<Graph::Vertex>{1, 3}));
}
