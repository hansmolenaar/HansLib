#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismCheck.h"
#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismVertexComparers.h"
#include "GraphUsc.h"
#include "Permutation.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphTest;
using namespace GraphIsomorphism;

namespace
{
const int numPermutations = 10;

void CheckVertexCompareConsistency(const IGraphUs &graph, GraphIsomorphism::ICompareFactory &factory,
                                   int expectNumUniqueVertices)
{
    const auto comparer = factory.createCompare(graph);
    const auto *vertexCompare = comparer->getVertexCompare();
    if (vertexCompare == nullptr)
    {
        return;
    }
    const auto nVertices = graph.getNumVertices();
    const VertexComparers comparersOrg(std::vector<const IVertexCompare *>{vertexCompare});
    const Grouping<Vertex> grouping(graph.getVertexRange(), VertexLess{*vertexCompare});

    if (expectNumUniqueVertices >= 0)
    {
        ASSERT_EQ(grouping.countUnique(), expectNumUniqueVertices);
    }
    else
    {
        expectNumUniqueVertices = grouping.countUnique();
    }

    const Permutation trivial = Permutation::CreateTrivial(nVertices);
    for (auto n : Iota::GetRange(numPermutations))
    {
        const auto permutation = Permutation::CreateRandomShuffle(trivial, n);
        const UndirectedGraph graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const auto gcomparerPermuted = factory.createCompare(graphPermuted);
        const auto *vertexComparePermuted = gcomparerPermuted->getVertexCompare();
        const Grouping<Vertex> groupingPermuted(graph.getVertexRange(), VertexLess{*vertexComparePermuted});
        ASSERT_EQ(groupingPermuted.countUnique(), expectNumUniqueVertices);

        const VertexComparers comparersPermuted(std::vector<const IVertexCompare *>{vertexComparePermuted});
        ASSERT_FALSE(comparersOrg < comparersPermuted);
    }
}
}; // namespace

void GraphTest::CheckGraphTaggerConsistency(const Graph::IGraphUs &graph, ICompareFactory &factory)
{
    const auto comparer = factory.createCompare(graph);
    const auto *tagger = comparer->getGraphTagger();
    if (tagger == nullptr)
        return;

    const auto &tag = tagger->getGraphTag();
    for (auto n : Iota::GetRange(numPermutations))
    {
        const UndirectedGraph graphPermuted = UndirectedGraph::CreateRandomShuffled(graph, n);
        const auto comparerPermuted = factory.createCompare(graphPermuted);
        const auto *taggerPermuted = comparerPermuted->getGraphTagger();
        ASSERT_EQ(tag, taggerPermuted->getGraphTag());
    }
}

void GraphTest::CheckTaggerConsistency(const IGraphUs &graph, GraphIsomorphism::ICompareFactory &factory,
                                       int expectNumUniqueVertices)
{
    CheckGraphTaggerConsistency(graph, factory);
    CheckVertexCompareConsistency(graph, factory, expectNumUniqueVertices);
}

namespace
{

void CheckTaggerBasics(GraphIsomorphism::ICompareFactory &factory, const IGraphUs &graph)
{
    const auto nVertices = graph.getNumVertices();
    const auto comparer = factory.createCompare(graph);
    Tag tag;
    const auto *graphChecker = comparer->getGraphTagger();

    if (graphChecker != nullptr)
    {
        EXPECT_NO_THROW(tag = graphChecker->getGraphTag());
    }

    const IVertexCompare *vertexComparer = comparer->getVertexCompare();
    if (vertexComparer != nullptr)
    {
        ASSERT_EQ(&vertexComparer->getGraph(), &graph);
        if (nVertices > 0)
        {
            ASSERT_EQ(vertexComparer->compare(0, 0), std::weak_ordering::equivalent);
            ASSERT_EQ(vertexComparer->compareVertexOtherGraph(0, *vertexComparer, 0), std::weak_ordering::equivalent);
        }
    }

    CheckTaggerConsistency(graph, factory);
}

}; // namespace

void GraphTest::CheckTagger(ICompareFactory &factory)
{
    // Test some tiny graphs
    auto graphs = UndirectedGraphFromG6::getGraphs(UndirectedGraphFromG6::getListNumVertices_3());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Null());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Singleton());

    for (const auto &g : graphs)
    {
        CheckTaggerBasics(factory, *g);
    }
}
