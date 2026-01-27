#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismComparersFactory.h"
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

    ASSERT_EQ(comparers.compareVertexOtherGraph(0, comparers, 1), std::weak_ordering::greater);
    ASSERT_EQ(comparers.compareVertexOtherGraph(1, comparers, 0), std::weak_ordering::less);

    ASSERT_EQ(comparers.compareVertexOtherGraph(1, comparers, 2), std::weak_ordering::equivalent); // equal
    ASSERT_EQ(comparers.compareVertexOtherGraph(2, comparers, 1), std::weak_ordering::equivalent); // equal

    const VertexComparers other(taggers);
    ASSERT_EQ(comparers.compareVertexOtherGraph(0, other, 1), std::weak_ordering::greater);
    ASSERT_EQ(comparers.compareVertexOtherGraph(1, other, 0), std::weak_ordering::less);

    const VertexGrouping grouping(graph->getVertexRange(), VertexLess{comparers});
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

    ASSERT_EQ(comparers.compareVertexOtherGraph(4, comparers, 0), std::weak_ordering::greater);
    ASSERT_EQ(comparers.compareVertexOtherGraph(0, comparers, 4), std::weak_ordering::less);

    ASSERT_EQ(comparers.compareVertexOtherGraph(4, comparers, 5), std::weak_ordering::equivalent); // equal
    ASSERT_EQ(comparers.compareVertexOtherGraph(5, comparers, 4), std::weak_ordering::equivalent); // equal

    const VertexLess less{comparers};
    ASSERT_TRUE(less(0, 4));
    ASSERT_FALSE(less(4, 0));

    const VertexComparers other(taggers);
    ASSERT_EQ(comparers.compareVertexOtherGraph(4, other, 0), std::weak_ordering::greater);
    ASSERT_EQ(comparers.compareVertexOtherGraph(0, other, 4), std::weak_ordering::less);

    const VertexGrouping grouping(graph->getVertexRange(), VertexLess{comparers});
    ASSERT_EQ(grouping.countUnique(), 1);
    ASSERT_EQ(grouping.getUniqueValues(), std::vector<Vertex>{3});
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{3, 2, 1}));
}

TEST(GraphIsomorphismVertexComparersTest, Diamond)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::diamond);
    const auto comparers = ComparersFactory().create(*graph);
    const VertexGrouping grouping = comparers->getVertexGrouping();
    ASSERT_EQ(grouping.countUnique(), 4);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 1, 1, 1}));
}

TEST(GraphIsomorphismVertexComparersTest, HardCase)
{
    const auto graph = UndirectedGraphFromG6::Create("FJ\\~w");
    const auto comparers = ComparersFactory().create(*graph);
    const VertexGrouping grouping = comparers->getVertexGrouping();
    ASSERT_EQ(grouping.countUnique(), 2);
}

TEST(GraphIsomorphismVertexComparersTest, DifferentGroups)
{
    const auto graph1 = UndirectedGraphLibrary::Get_DisconnectedGraph(3);
    const auto graph2 = UndirectedGraphLibrary::Get_Cycle(3);
    const TaggerDegree comparer1(*graph1);
    const TaggerDegree comparer2(*graph2);
    const IVertexCompare &vcomparer1 = comparer1;
    const IVertexCompare &vcomparer2 = comparer2;
    ASSERT_EQ(vcomparer1.getVertexGrouping()(), vcomparer2.getVertexGrouping()());
    ASSERT_EQ(vcomparer1.compareGraph(vcomparer2), std::weak_ordering::less);
}
