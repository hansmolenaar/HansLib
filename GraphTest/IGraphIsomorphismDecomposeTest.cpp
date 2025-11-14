#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "GraphIsomorphismUtils.h"
#include "IGraphIsomorphismDecompose.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{

GraphVertex GetVertexInRoot(GraphVertex vertex, const IDecompose *decompose, const IDecompose::ToParentMap toParent)
{
    while (toParent.contains(decompose))
    {
        vertex = decompose->getVertexInParent(vertex);
        decompose = toParent.at(decompose);
    }
    return vertex;
}

void CheckDecompose(const IGraphUs &graph, int expectNumLeaves = -1)
{
    const auto decomposeed = IDecompose::Create(graph);
    const auto toParent = IDecompose::GetToParentMap(decomposeed.get());

    int numLeaves = 0;
    std::vector<GraphVertex> vertices;
    for (const auto &itr : toParent)
    {
        if (itr.first->isLeaf())
        {
            numLeaves += 1;
            const IGraphUs &graph = itr.first->getSelf();
            const auto numVertices = graph.getNumVertices();
            for (GraphVertex v = 0; v < numVertices; ++v)
            {
                vertices.emplace_back(GetVertexInRoot(v, itr.first, toParent));
            }
        }
    }

    if (expectNumLeaves > 0)
    {
        ASSERT_EQ(expectNumLeaves, numLeaves);
    }

    // Not vertices lost or added?
    const auto fullNumVertices = graph.getNumVertices();
    str::sort(vertices);
    ASSERT_EQ(vertices.size(), fullNumVertices);
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());
    ASSERT_EQ(vertices.size(), fullNumVertices);
    ASSERT_EQ(vertices.front(), 0);
    ASSERT_EQ(vertices.back(), fullNumVertices - 1);
}

void CheckDecomposeList(const std::vector<std::string> &g6list, Tag expectMultiplicities)
{
    std::vector<std::unique_ptr<Graph::IGraphUs>> graphs = UndirectedGraphFromG6::getGraphs(g6list);
    std::vector<TaggedGraph> taggedGraphs;
    str::transform(graphs, std::back_inserter(taggedGraphs), [](const auto &gp) { return TaggedGraph(*gp); });

    auto cmp = [](const TaggedGraph *lhs, const TaggedGraph *rhs) { return *lhs < *rhs; };
    std::map<const TaggedGraph *, size_t, decltype(cmp)> multiplicityMap(cmp);
    for (const auto &tg : taggedGraphs)
    {
        CheckDecompose(tg.getGraph());
        multiplicityMap[&tg] += 1;
    }

    std::vector<size_t> multiplicities;
    for (const auto &itr : multiplicityMap)
    {
        multiplicities.push_back(itr.second);
    }
    const auto tag = CondenseSizeSequence(multiplicities);
    ASSERT_EQ(tag, expectMultiplicities);
}

void PrintMultipleTags(const std::vector<std::string> &g6list)
{
    const std::vector<std::unique_ptr<Graph::IGraphUs>> graphs = UndirectedGraphFromG6::getGraphs(g6list);
    std::map<GraphTags, std::vector<const IGraphUs *>> multiplicityMap;
    for (const auto &g : graphs)
    {
        multiplicityMap[IDecompose::GetGraphTags(*g)].push_back(g.get());
    }

    std::vector<size_t> multiplicities;
    for (const auto &itr : multiplicityMap)
    {
        if (itr.second.size() > 1)
        {
            std::cout << "size = " << itr.second.size() << "\n";
            for (const auto *g : itr.second)
            {
                const auto tagAgain = IDecompose::GetGraphTags(*g);
                std::cout << g->getName() << "    " << tagAgain << "\n";
            }
            std::cout << "\n";
            std::cout << "\n";
        }
    }
}

} // namespace

TEST(IGraphIsomorphismDecomposeTest, GetToParentMapSingleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const auto decomposeed = IDecompose::Create(*graph);
    const auto toParent = IDecompose::GetToParentMap(decomposeed.get());
    ASSERT_EQ(toParent.size(), 1);
    ASSERT_EQ(toParent.begin()->first, decomposeed.get());
    ASSERT_EQ(toParent.begin()->second, nullptr);

    CheckDecompose(*graph, 1);
}

TEST(IGraphIsomorphismDecomposeTest, GetToParentMapDisconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto decomposeed = IDecompose::Create(*graph);
    const auto *root = decomposeed.get();
    const auto toParent = IDecompose::GetToParentMap(root);
    ASSERT_EQ(toParent.size(), 3);
    ASSERT_EQ(toParent.at(root), nullptr);

    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, Disconnected2)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(2);
    const auto decomposeed = IDecompose::Create(*graph);
    const auto &tag = Single(decomposeed->getChildTags());
    const auto &children = decomposeed->getChildren(tag);
    ASSERT_EQ(children.size(), 2);

    ASSERT_EQ(children.at(0)->getSelf().getNumVertices(), 1);
    ASSERT_EQ(children.at(0)->getVertexInParent(0), 0);

    ASSERT_EQ(children.at(1)->getSelf().getNumVertices(), 1);
    ASSERT_EQ(children.at(1)->getVertexInParent(0), 1);

    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, EdgePlusVertex)
{
    const auto graph = UndirectedGraphFromG6::Create("BO");
    const auto decomposeed = IDecompose::Create(*graph);
    const auto &tags = decomposeed->getChildTags();
    ASSERT_EQ(tags.size(), 2);

    const auto &children0 = decomposeed->getChildren(tags.at(0));
    ASSERT_EQ(children0.size(), 1);
    ASSERT_EQ(children0.at(0)->getSelf().getNumVertices(), 2);

    const auto &children1 = decomposeed->getChildren(tags.at(1));
    ASSERT_EQ(children1.size(), 1);
    ASSERT_EQ(children1.at(0)->getSelf().getNumVertices(), 1);

    const auto &tagsGrandChildren0 = Single(children0)->getChildTags();
    ASSERT_TRUE(tagsGrandChildren0.empty());
    ASSERT_EQ(Single(children0)->getVertexInParent(0), 0);
    ASSERT_EQ(Single(children0)->getVertexInParent(1), 2);

    const auto &tagsGrandChildren1 = Single(children1)->getChildTags();
    ASSERT_TRUE(tagsGrandChildren1.empty());
    ASSERT_EQ(Single(children1)->getVertexInParent(0), 1);

    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    const auto decomposeed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, Paw)
{
    const auto graph = UndirectedGraphFromG6::Create("Cx");
    const auto decomposeed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, Dart)
{
    const auto graph = UndirectedGraphFromG6::Create("DvC");
    const auto decomposeed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, K5MinE)
{
    const auto graph = UndirectedGraphFromG6::Create("D~k");
    const auto decomposeed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, Gem)
{
    const auto graph = UndirectedGraphFromG6::Create("Dh{");
    const auto decomposeed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 2);
}

TEST(IGraphIsomorphismDecomposeTest, X100)
{
    const auto graph = UndirectedGraphFromG6::Create("FgCNw");
    const auto decomposeed = IDecompose::Create(*graph);
    CheckDecompose(*graph, 3);
}

TEST(IGraphIsomorphismDecomposeTest, JustAskingQuestions)
{
    const std::vector<std::string> names{"F?S~", "FALcw"};
    const auto graph1 = UndirectedGraphFromG6::Create(names.at(0));
    const auto graph2 = UndirectedGraphFromG6::Create(names.at(1));
    TaggerNumbersFactory factory;
    const auto tagger1 = factory.createTagger(*graph1);
    const auto tagger2 = factory.createTagger(*graph2);
    const auto graphTag1 = tagger1->getGraphTagger()->getGraphTag();
    const auto graphTag2 = tagger2->getGraphTagger()->getGraphTag();
    const auto graphTagAll1 = IDecompose::GetGraphTags(*graph1);
    const auto graphTagAll2 = IDecompose::GetGraphTags(*graph2);
    ASSERT_NE(graphTagAll1, graphTagAll2);
    CheckDecomposeList(names, Tag{1, 2});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList3)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_3(), Tag{1, 4});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList4)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_4(), Tag{1, 11});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList5)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_5(), {1, 34});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList6)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_6(), {1, 155});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList7)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_7(), {1, 298, 2, 2});
    // PrintMultipleTags(UndirectedGraphFromG6::getListNumVertices_7());
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList8)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_8(), {1, 715, 2, 14, 3, 1 });
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList9)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_9(), {1, 442, 2, 15, 3, 3});
}

TEST(IGraphIsomorphismDecomposeTest, CheckDecomposeList10)
{
    CheckDecomposeList(UndirectedGraphFromG6::getListNumVertices_10(), {1, 694, 2, 3, 3, 3, 6, 1 });
}
