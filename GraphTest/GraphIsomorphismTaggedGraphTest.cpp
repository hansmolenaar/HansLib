#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "GraphIsomorphismTaggerChains.h"
#include "GraphIsomorphismTaggerTriangles.h"
#include "GraphIsomorphismUtils.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;
using namespace Utilities;

namespace
{

void CheckAgainsPermutations(const TaggedGraph &tg)
{
    constexpr int numPermutations = 5;
    const auto &graph = tg.getGraph();
    const Permutation trivial = Permutation::CreateTrivial(graph.getNumVertices());
    for (int n = 0; n < numPermutations; ++n)
    {
        const auto permutation = Permutation::CreateRandomShuffle(trivial, n);
        const auto graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const TaggedGraph pg(graphPermuted);
        ASSERT_TRUE(tg == pg);
        ASSERT_FALSE(tg < pg);
        ASSERT_FALSE(tg > pg);
    }
}

void CheckTaggingForList(const std::vector<std::string> &g6list, Tag expectMultiplicities)
{
    std::vector<std::unique_ptr<Graph::IGraphUs>> graphs = UndirectedGraphFromG6::getGraphs(g6list);
    std::vector<TaggedGraph> taggedGraphs;
    str::transform(graphs, std::back_inserter(taggedGraphs), [](const auto &gp) { return TaggedGraph(*gp); });
    std::vector<const TaggedGraph *> taggedGraphPointers(taggedGraphs.size());
    str::transform(taggedGraphs, taggedGraphPointers.begin(), [](const auto &tg) { return &tg; });

    auto cmp = [](const TaggedGraph *lhs, const TaggedGraph *rhs) { return *lhs < *rhs; };
    const Grouping<const TaggedGraph *> multiplicityMap(taggedGraphPointers, cmp);

    const std::vector<size_t> multiplicities = multiplicityMap.getGroupSizes();
    const auto tag = CondenseSizeSequence(multiplicities);
    ASSERT_EQ(tag, expectMultiplicities);
}

void PrintMultipleTags(const std::vector<std::string> &g6list)
{
    const std::vector<std::unique_ptr<Graph::IGraphUs>> graphs = UndirectedGraphFromG6::getGraphs(g6list);
    std::vector<TaggedGraph> taggedGraphs;
    str::transform(graphs, std::back_inserter(taggedGraphs), [](const auto &gp) { return TaggedGraph(*gp); });
    std::vector<const TaggedGraph *> taggedGraphPointers(taggedGraphs.size());
    str::transform(taggedGraphs, taggedGraphPointers.begin(), [](const auto &tg) { return &tg; });

    auto cmp = [](const TaggedGraph *lhs, const TaggedGraph *rhs) { return *lhs < *rhs; };
    const Grouping<const TaggedGraph *> multiplicityMap(taggedGraphPointers, cmp);

    for (const auto &group : multiplicityMap())
    {
        if (group.size() > 1)
        {
            std::cout << "size = " << group.size() << "\n";
            for (const auto *tg : group)
            {
                std::cout << tg->getGraph().getName() << "\n";
            }
            std::cout << "\n";
            std::cout << "\n";
        }
    }
}

void CheckUniquenessGraphTaggers(const std::vector<std::unique_ptr<Graph::IGraphUs>> &graphs, int expectNumGraphs,
                                 int expectNumUniqueTags)
{
    ASSERT_EQ(graphs.size(), expectNumGraphs);
    std::set<std::vector<Tag>> uniqueTags;
    for (const auto &g : graphs)
    {
        ASSERT_TRUE(!g->isConnected());
        std::vector<Tag> tags;
        const auto allTaggers = GraphIsomorphism::getAllTaggers(*g);
        const auto graphTaggers = GraphIsomorphism::selectGraphTaggers(allTaggers);
        for (const auto *graphTagger : graphTaggers)
        {
            tags.emplace_back(graphTagger->getGraphTag());
        }
        uniqueTags.insert(tags);
    }
    ASSERT_EQ(uniqueTags.size(), expectNumUniqueTags);
}

} // namespace

TEST(GraphIsomorphismTaggedGraphTest, Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const TaggedGraph tg(*graph);
    ASSERT_TRUE(tg == tg);
    ASSERT_FALSE(tg < tg);
    ASSERT_FALSE(tg > tg);
}

TEST(GraphIsomorphismTaggedGraphTest, Path2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const auto permuted = UndirectedGraph::CreatePermuted(*graph, Permutation::Create({1, 0}));
    const TaggedGraph tg(*graph);
    const TaggedGraph pg(permuted);
    ASSERT_TRUE(tg == pg);
    ASSERT_FALSE(tg < pg);
    ASSERT_FALSE(tg > pg);
}

TEST(GraphIsomorphismTaggedGraphTest, DegreePath2and3)
{
    const auto g0 = UndirectedGraphLibrary::Get_Path(2);
    const auto g1 = UndirectedGraphLibrary::Get_Path(3);
    const TaggedGraph tg0(*g0);
    const TaggedGraph tg1(*g1);
    const auto status = TaggedGraph::tryConnect(tg0, tg1);
    ASSERT_EQ(status.getFlag(), Flag::NotIsomorphic);
}

TEST(GraphIsomorphismTaggedGraphTest, DegreePath3)
{
    const auto g0 = UndirectedGraphLibrary::Get_Path(3);
    const auto g1 = UndirectedGraphLibrary::Get_Path(3);
    const TaggedGraph tg0(*g0);
    const TaggedGraph tg1(*g1);
    const auto status = TaggedGraph::tryConnect(tg0, tg1);
    ASSERT_EQ(status.getFlag(), Flag::Isomorphic);
}

TEST(GraphIsomorphismTaggedGraphTest, DegreeStar123)
{
    const auto g0 = UndirectedGraphLibrary::Get_Star({1, 2, 3});
    const auto g1 = UndirectedGraphLibrary::Get_Star({3, 1, 2});
    const TaggedGraph tg0(*g0);
    const TaggedGraph tg1(*g1);
    const auto status = TaggedGraph::tryConnect(tg0, tg1);
    ASSERT_EQ(status.getFlag(), Flag::Isomorphic);
}

TEST(GraphIsomorphismTaggedGraphTest, DegreePan3)
{
    const auto g0 = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::pan3);
    const auto g1 = GraphUsc::CreatePermuted(*g0, {2, 1, 0, 3});
    const TaggedGraph tg0(*g0);
    const TaggedGraph tg1(g1);
    const auto status = TaggedGraph::tryConnect(tg0, tg1);
    ASSERT_EQ(status.getFlag(), Flag::Isomorphic);
}

TEST(GraphIsomorphismTaggedGraphTest, SpecialCase1)
{
    const auto g0 = UndirectedGraphFromG6::CreateConnected("FMhXw");
    const auto g1 = UndirectedGraphFromG6::CreateConnected("FDxZg");
    const TaggedGraph tg0(*g0);
    const TaggedGraph tg1(*g1);
    const auto status = TaggedGraph::tryConnect(tg0, tg1);

    const TaggerTriangles taggerTriangles0(std::make_shared<UndirectedGraphTriangles>(*g0));
    const TaggerTriangles taggerTriangles1(std::make_shared<UndirectedGraphTriangles>(*g1));

    ASSERT_EQ(taggerTriangles0.compareOtherGraph(3, taggerTriangles0, 4), std::weak_ordering::equivalent);
    ASSERT_EQ(taggerTriangles1.compareOtherGraph(3, taggerTriangles1, 4), std::weak_ordering::greater);

    const TaggerChains taggerChains0(*g0);
    const TaggerChains taggerChains1(*g1);

    ASSERT_EQ(taggerChains0.getVertexTag(3), (Tag{4, 3}));
    ASSERT_EQ(taggerChains0.getVertexTag(4), (Tag{4, 3}));

    ASSERT_EQ(taggerChains1.getVertexTag(3), (Tag{4, 3}));
    ASSERT_EQ(taggerChains1.getVertexTag(4), (Tag{4, 3}));

    ASSERT_TRUE(tg0 != tg1);
}

TEST(GraphIsomorphismTaggedGraphTest, SpecialCase2)
{
    const auto g0 = UndirectedGraphFromG6::Create("G}hPW{");
    const auto g1 = UndirectedGraphFromG6::Create("G~`HW{");
    const TaggedGraph tg0(*g0);
    const TaggedGraph tg1(*g1);
    const auto grouping0 = tg0.getVertexGrouping();
    const auto grouping1 = tg1.getVertexGrouping();
    ASSERT_TRUE(grouping0.getUniqueValues().empty());
    ASSERT_TRUE(grouping1.getUniqueValues().empty());

    const auto tgCompare = tg0 <=> tg1;
    ASSERT_TRUE(tgCompare != std::weak_ordering::equivalent);
}

TEST(GraphIsomorphismTaggedGraphTest, CheckTaggingList3)
{
    CheckTaggingForList(UndirectedGraphFromG6::getListNumVertices_3(), Tag{1, 4});
}

TEST(GraphIsomorphismTaggedGraphTest, CheckTaggingList4)
{
    CheckTaggingForList(UndirectedGraphFromG6::getListNumVertices_4(), Tag{1, 11});
}

TEST(GraphIsomorphismTaggedGraphTest, CheckTaggingList5)
{
    CheckTaggingForList(UndirectedGraphFromG6::getListNumVertices_5(), {1, 34});
}

TEST(GraphIsomorphismTaggedGraphTest, CheckTaggingList6)
{
    CheckTaggingForList(UndirectedGraphFromG6::getListNumVertices_6(), {1, 155});
}

TEST(GraphIsomorphismTaggedGraphTest, CheckTaggingList7)
{
    CheckTaggingForList(UndirectedGraphFromG6::getListNumVertices_7(), {1, 302});
    // PrintMultipleTags(UndirectedGraphFromG6::getListNumVertices_7());
}

TEST(GraphIsomorphismTaggedGraphTest, CheckTaggingList8)
{
    CheckTaggingForList(UndirectedGraphFromG6::getListNumVertices_8(), {1, 744, 2, 1});
}
TEST(GraphIsomorphismConstructTest, Disconnected5)
{
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_5());
    CheckUniquenessGraphTaggers(graphs, 13, 13);
}

TEST(GraphIsomorphismTaggedGraphTest, Disconnected6)
{
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_6());
    CheckUniquenessGraphTaggers(graphs, 43, 43);
}

TEST(GraphIsomorphismTaggedGraphTest, Disconnected7)
{
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_7());
    CheckUniquenessGraphTaggers(graphs, 35, 35);
}

TEST(GraphIsomorphismTaggedGraphTest, Disconnected8)
{
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_8());
    CheckUniquenessGraphTaggers(graphs, 17, 17);
}

TEST(GraphIsomorphismTaggedGraphTest, Disconnected9)
{
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_9());
    CheckUniquenessGraphTaggers(graphs, 18, 18);
}
