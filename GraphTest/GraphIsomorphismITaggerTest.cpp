#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismCheck.h"
#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismVertexComparers.h"
#include "GraphUsc.h"
#include "IGraphIsomorphismVertexCompare.h"
#include "Permutation.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphTest;
using namespace GraphIsomorphism;

namespace
{
const int numPermutations = 10;

void CheckVertexCompareConsistency(const IGraphUs &graph, GraphIsomorphism::ITaggerFactory &factory,
                                   int expectNumUniqueVertices)
{
    const auto gtagger = factory.createTagger(graph);
    const auto *vertexCompare = dynamic_cast<const GraphIsomorphism::IVertexCompare *>(gtagger.get());
    if (vertexCompare == nullptr)
    {
        return;
    }
    const auto nVertices = graph.getNumVertices();
    const VertexComparers comparerOrg(std::vector<const IVertexCompare *>{vertexCompare});
    const Grouping<GraphVertex> grouping(graph.getVertexRange(), VertexLess{*vertexCompare});

    if (expectNumUniqueVertices >= 0)
    {
        ASSERT_EQ(grouping.countUnique(), expectNumUniqueVertices);
    }
    else
    {
        expectNumUniqueVertices = grouping.countUnique();
    }

    const Permutation trivial = Permutation::CreateTrivial(nVertices);
    for (auto n = 0; n < numPermutations; ++n)
    {
        const auto permutation = Permutation::CreateRandomShuffle(trivial, n);
        const UndirectedGraph graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const auto gtaggerPermuted = factory.createTagger(graphPermuted);
        const auto *vertexComparePermuted =
            dynamic_cast<const GraphIsomorphism::IVertexCompare *>(gtaggerPermuted.get());
        const Grouping<GraphVertex> groupingPermuted(graph.getVertexRange(), VertexLess{*vertexComparePermuted});
        ASSERT_EQ(groupingPermuted.countUnique(), expectNumUniqueVertices);

        const VertexComparers comparerPermuted(std::vector<const IVertexCompare *>{vertexComparePermuted});
        ASSERT_FALSE(comparerOrg < comparerPermuted);
    }
}

void CheckGraphTaggerConsistency(const Graph::IGraphUs &graph, ITaggerFactory &factory)
{
    const auto nVertices = graph.getNumVertices();
    const auto gtagger = factory.createTagger(graph);
    const auto *tagger = gtagger->getGraphTagger();
    if (tagger == nullptr)
        return;
    const auto &tag = tagger->getGraphTag();

    const Permutation trivial = Permutation::CreateTrivial(nVertices);
    for (auto n = 0; n < numPermutations; ++n)
    {
        const auto permutation = Permutation::CreateRandomShuffle(trivial, n);
        const UndirectedGraph graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const auto gtaggerPermuted = factory.createTagger(graphPermuted);
        const auto *taggerPermuted = gtaggerPermuted->getGraphTagger();
        ASSERT_EQ(tag, taggerPermuted->getGraphTag());
    }
}
}; // namespace

void GraphTest::CheckTaggerConsistency(const IGraphUs &graph, GraphIsomorphism::ITaggerFactory &factory,
                                       int expectNumUniqueVertices)
{
    CheckGraphTaggerConsistency(graph, factory);
    CheckVertexCompareConsistency(graph, factory, expectNumUniqueVertices);
}

namespace
{

void CheckTaggerBasics(GraphIsomorphism::ITaggerFactory &factory, const IGraphUs &graph)
{
    const auto nVertices = graph.getNumVertices();
    const auto tagger = factory.createTagger(graph);
    Tag tag;
    const auto *graphChecker = tagger->getGraphTagger();

    if (graphChecker != nullptr)
    {
        EXPECT_NO_THROW(tag = graphChecker->getGraphTag());
    }

    const IVertexCompare *vertexComparer = dynamic_cast<const GraphIsomorphism::IVertexCompare *>(tagger.get());
    if (vertexComparer != nullptr)
    {
        ASSERT_EQ(&vertexComparer->getGraph(), &graph);
        if (nVertices > 0)
        {
            ASSERT_EQ(vertexComparer->compare(0, 0), std::weak_ordering::equivalent);
            ASSERT_EQ(vertexComparer->compareOtherGraph(0, *vertexComparer, 0), std::weak_ordering::equivalent);
        }
    }

    CheckTaggerConsistency(graph, factory);
}

}; // namespace

void GraphTest::CheckTagger(ITaggerFactory &factory)
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
