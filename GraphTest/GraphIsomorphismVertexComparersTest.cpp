#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismAllCompareFactories.h"
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

    ASSERT_EQ(comparers.compare(0, 1), std::weak_ordering::greater);
    ASSERT_EQ(comparers.compare(1, 0), std::weak_ordering::less);

    ASSERT_EQ(comparers.compare(1, 2), std::weak_ordering::equivalent); // equal
    ASSERT_EQ(comparers.compare(2, 1), std::weak_ordering::equivalent); // equal

    const VertexComparers other(taggers);
    ASSERT_EQ(comparers.compareOtherGraph(0, other, 1), std::weak_ordering::greater);
    ASSERT_EQ(comparers.compareOtherGraph(1, other, 0), std::weak_ordering::less);

    const Grouping<Vertex> grouping(graph->getVertexRange(), VertexLess{comparers});
    ASSERT_EQ(grouping.countUnique(), 1);
    ASSERT_EQ(grouping.getUniqueValues(), std::vector<Vertex>{0});
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{4, 1}));

    const VertexLess less{comparers};
    ASSERT_TRUE(less(1, 0));
    ASSERT_FALSE(less(0, 1));
}

TEST(GraphIsomorphismVertexComparersTest, Fish)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::fish);
    const TaggerDegree taggerDegree(*graph);
    const TaggerTriangles taggerTriangles(std::make_shared<UndirectedGraphTriangles>(*graph));
    const std::vector<const IVertexCompare *> taggers{&taggerDegree, &taggerTriangles};
    const VertexComparers comparers(taggers);

    ASSERT_EQ(comparers.compare(4, 0), std::weak_ordering::greater);
    ASSERT_EQ(comparers.compare(0, 4), std::weak_ordering::less);

    ASSERT_EQ(comparers.compare(4, 5), std::weak_ordering::equivalent); // equal
    ASSERT_EQ(comparers.compare(5, 4), std::weak_ordering::equivalent); // equal

    const VertexLess less{comparers};
    ASSERT_TRUE(less(0, 4));
    ASSERT_FALSE(less(4, 0));

    const VertexComparers other(taggers);
    ASSERT_EQ(comparers.compareOtherGraph(4, other, 0), std::weak_ordering::greater);
    ASSERT_EQ(comparers.compareOtherGraph(0, other, 4), std::weak_ordering::less);

    const Grouping<Vertex> grouping(graph->getVertexRange(), VertexLess{comparers});
    ASSERT_EQ(grouping.countUnique(), 1);
    ASSERT_EQ(grouping.getUniqueValues(), std::vector<Vertex>{3});
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{3, 2, 1}));
}

TEST(GraphIsomorphismVertexComparersTest, Diamond)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::diamond);
    const auto allComparers = AllCompareFactories().getAllComparers(*graph);
    const std::vector<const IVertexCompare *> allVertexCompare = selectVertexCompare(allComparers);
    const VertexComparers comparers(allVertexCompare);

    const Grouping<Vertex> grouping(graph->getVertexRange(), VertexLess{comparers});
    ASSERT_EQ(grouping.countUnique(), 4);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 1, 1, 1}));
}

TEST(GraphIsomorphismVertexComparersTest, HardCase)
{
    const auto graph = UndirectedGraphFromG6::Create("FJ\\~w");
    const auto allComparers = AllCompareFactories().getAllComparers(*graph);
    const std::vector<const IVertexCompare *> allVertexCompare = selectVertexCompare(allComparers);
    const VertexComparers comparers(allVertexCompare);

    const Grouping<Vertex> grouping(graph->getVertexRange(), VertexLess{comparers});
    ASSERT_EQ(grouping.countUnique(), 2);
}
