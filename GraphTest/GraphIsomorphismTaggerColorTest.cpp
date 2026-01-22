#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismICompareTest.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerColor.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;

namespace
{
void CheckColoring(const IGraphUs &graph)
{
    const TaggerColor tagger(graph);
    constexpr size_t numPermutation = 5;
    const Permutation trivial = Permutation::CreateTrivial(graph.getNumVertices());

    for (size_t n : Iota::GetRange(numPermutation))
    {
        const auto permutation = Permutation::CreateRandomShuffle(trivial, n);
        const auto graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const TaggerColor taggerPermuted(graphPermuted);
        if (tagger.getGraphTag() != taggerPermuted.getGraphTag())
        {
            std::cout << "LET OP " << graph.getName() << "\n";
        }

        ASSERT_EQ(tagger.getGraphTag(), taggerPermuted.getGraphTag());
    }
}

} // namespace

TEST(GraphIsomorphismTaggerColorTest, CheckTagger)
{
    CompareColorFactory factory;
    GraphTest::CheckTagger(factory);
}

TEST(GraphIsomorphismTaggerColorTest, CheckList)
{
    CompareColorFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_5(), Tag{1, 3, 4, 1, 5, 1, 7, 1, 15, 1});
}

TEST(GraphIsomorphismTaggerColorTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    CompareColorFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 1);

    const TaggerColor tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{2, 1, 2}));
    ASSERT_EQ(tagger.getVertexTag(0), 2);
    ASSERT_EQ(tagger.getVertexTag(1), 1);
    ASSERT_EQ(tagger.getVertexTag(2), 2);

    ASSERT_EQ(tagger.compareOtherGraph(0, tagger, 1), std::weak_ordering::greater);
    ASSERT_EQ(tagger.compareOtherGraph(1, tagger, 0), std::weak_ordering::less);

    CheckColoring(*graph);
}

TEST(GraphIsomorphismTaggerColorTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    CompareColorFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 0);

    const TaggerColor tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{3}));
    ASSERT_EQ(tagger.getVertexTag(0), -1);
    ASSERT_EQ(tagger.getVertexTag(1), -1);
    ASSERT_EQ(tagger.getVertexTag(2), -1);

    ASSERT_EQ(tagger.compareOtherGraph(0, tagger, 1), std::weak_ordering::equivalent);

    CheckColoring(*graph);
}

TEST(GraphIsomorphismTaggerColorTest, Claw)
{
    const auto graph = UndirectedGraphLibrary::Get_Claw();
    CompareColorFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 1);

    const TaggerColor tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{2, 1, 3}));
    ASSERT_EQ(tagger.getVertexTag(0), 1);
    ASSERT_EQ(tagger.getVertexTag(1), 3);
    ASSERT_EQ(tagger.getVertexTag(2), 3);
    ASSERT_EQ(tagger.getVertexTag(3), 3);

    const auto cycle4 = UndirectedGraphLibrary::Get_Cycle(3);
    const TaggerColor taggerCycle4(*cycle4);
    ASSERT_EQ(tagger.getGraphTag() <=> taggerCycle4.getGraphTag(), std::strong_ordering::less);
    ASSERT_EQ(tagger.compareOtherGraph(0, taggerCycle4, 0), std::weak_ordering::greater);
    ASSERT_EQ(taggerCycle4.compareOtherGraph(0, tagger, 0), std::weak_ordering::less);

    CheckColoring(*graph);
}

TEST(GraphIsomorphismTaggerColorTest, Disconnected)
{
    const auto graph = UndirectedGraphFromG6::Create("D?K");
    CheckColoring(*graph);
}
