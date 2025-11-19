#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "GraphIsomorphismTaggerTriangles.h"
#include "GraphIsomorphismUtils.h"
#include "GraphIsomorphismVertexComparers.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;

TEST(GraphIsomorphismVertexComparersTest, DegreeButterfly)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::butterfly);
    const TaggerDegree tagger(*graph);
    const std::vector<const IVertexCompare *> taggers{&tagger};
    const VertexComparers comparers(taggers);

    ASSERT_EQ(&comparers.getGraph(), graph.get());

    ASSERT_FALSE(comparers.less(0, 1));
    ASSERT_TRUE(comparers.less(1, 0));

    ASSERT_FALSE(comparers.less(1, 2)); // equal
    ASSERT_FALSE(comparers.less(2, 1)); // equal

    const VertexComparers other(taggers);
    ASSERT_FALSE(comparers.less(0, other, 1));
    ASSERT_TRUE(comparers.less(1, other, 0));

    const Grouping<GraphVertex> grouping(graph->getVertexRange(), VertexLess{comparers});
    ASSERT_EQ(grouping.countUnique(), 1);
    ASSERT_EQ(grouping.getUniqueValues(), std::vector<GraphVertex>{0});
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{4, 1}));
}

TEST(GraphIsomorphismVertexComparersTest, Fish)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::fish);
    const TaggerDegree taggerDegree(*graph);
    const TaggerTriangles taggerTriangles(*graph);
    const std::vector<const IVertexCompare *> taggers{&taggerDegree, &taggerTriangles};
    const VertexComparers comparers(taggers);

    ASSERT_FALSE(comparers.less(4, 0));
    ASSERT_TRUE(comparers.less(0, 4));

    ASSERT_FALSE(comparers.less(4, 5)); // equal
    ASSERT_FALSE(comparers.less(5, 4)); // equal

    const VertexComparers other(taggers);
    ASSERT_FALSE(comparers.less(4, other, 0));
    ASSERT_TRUE(comparers.less(0, other, 4));

    const Grouping<GraphVertex> grouping(graph->getVertexRange(), VertexLess{comparers});
    ASSERT_EQ(grouping.countUnique(), 1);
    ASSERT_EQ(grouping.getUniqueValues(), std::vector<GraphVertex>{3});
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{3, 2, 1}));
}

TEST(GraphIsomorphismVertexComparersTest, Diamond)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::diamond);
    const auto taggers = getAllTaggers(*graph);
    const std::vector<const IVertexCompare *> allVertexCompare = selectVertexCompare(taggers);
    const VertexComparers comparers(allVertexCompare);

    const Grouping<GraphVertex> grouping(graph->getVertexRange(), VertexLess{comparers});
    ASSERT_EQ(grouping.countUnique(), 4);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 1, 1, 1}));
}
